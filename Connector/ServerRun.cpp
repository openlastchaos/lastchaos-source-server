#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"
#include "DBCmd.h"


static int InitFDSET(socket_t s1, CDescriptor* slist, fd_set* infds, fd_set* outfds, fd_set* exfds);

void CServer::Run()
{
	struct timeval last_time, opt_time, process_time, temp_time;
	struct timeval before_sleep, now, timeout;
	CNetMsg inputmsg;
	CDescriptor* d;
	CDescriptor* dNext;
	int missed_pulses;
	int maxdesc;
	int count=0;
	int count_pulse = PULSE_REAL_HOUR;

	// 시간 관련 변수 초기화
	opt_time.tv_usec = OPT_USEC;
	opt_time.tv_sec = 0;
	FD_ZERO(&m_null_set);
	GetTimeofDay(&last_time, (struct timezone *) 0);

	// 메인 루프
	while (!m_bshutdown)
	{
		// 디스크립터 셋 초기화
		maxdesc = InitFDSET(m_ssock, m_desclist, &m_input_set, &m_output_set, &m_exc_set);

		// 이전 루프 소요 시간을 가지고 루프를 일정시간에 한번 수행하도록 조정
		// 현재 시간 구하기
		GetTimeofDay(&before_sleep, (struct timezone *) 0);
		// 이전 루프 시간과 현재 시간의 차이로 이전 루프 처리 소요 시간을 구함
		TimeDiff(&process_time, &before_sleep, &last_time);

		// 놓친 펄스 수 구하기
		// 펄스(틱)은 OPT_USEC 당 1이 무조건 증가해야함
		// 처리 시간이 OPT_USEC(루프 1회의 소요 시간 경계치)보다 작으면 놓친 시간 없음
		if (process_time.tv_sec == 0 && process_time.tv_usec < OPT_USEC)
		{
			missed_pulses = 0;
		}
		// 몇개나 놓쳤는지 계산
		else
		{
			missed_pulses = process_time.tv_sec * PASSES_PER_SEC;
			missed_pulses += process_time.tv_usec / OPT_USEC;
			process_time.tv_sec = 0;
			process_time.tv_usec = process_time.tv_usec % OPT_USEC;
		}

		// 얼마나 쉬어야 할지 계산
		// temp = 틱소요 - 이전처리시간
		TimeDiff(&temp_time, &opt_time, &process_time);
		// last = 루프시작시간 + temp
		TimeAdd(&last_time, &before_sleep, &temp_time);
		// 현재 시간 구하기
		GetTimeofDay(&now, (struct timezone *) 0);
		// timeout = last - 현재시간
		TimeDiff(&timeout, &last_time, &now);

		// timeout 만큼 쉬기
		do
		{
			if (timeout.tv_sec < 0)
			{
				GAMELOG << init("SYS_ERR")
						<< "Timeout value is negative"
						<< end;
				timeout.tv_sec = 0;

			}
			else if (timeout.tv_sec == 0 && timeout.tv_usec < 0)
			{
				GAMELOG << init("SYS_ERR")
						<< "Timeout value is negative"
						<< end;
				timeout.tv_usec = 0;
			}
			ServerSleep(&timeout);
			GetTimeofDay(&now, (struct timezone *) 0);
			TimeDiff(&timeout, &last_time, &now);
		} while (timeout.tv_usec || timeout.tv_sec);

		// 블럭킹 없이 폴링
		a_NullTime.tv_sec = 0;
		a_NullTime.tv_usec = 0;
		if (select(maxdesc + 1, &m_input_set, &m_output_set, &m_exc_set, &a_NullTime) < 0)
		{
			GAMELOG << init("SYS_ERR") << "Select poll" << end;
			return;
		}
		a_NullTime.tv_sec = 0;
		a_NullTime.tv_usec = 0;

		// 새로운 소켓 연결을 처리
		if (FD_ISSET(m_ssock, &m_input_set))
			NewDescriptor(m_ssock);

		// 접속 수 로그 남기기
		if (count_pulse < 0)
		{
			count_pulse = PULSE_REAL_HOUR;
			GAMELOG << init("CONNECTIONS")
					<< count
					<< end;
		}
		else
		{
			count_pulse--;
		}
		count = 0;

		// 펄스 증가 최소 1개로 조정
		missed_pulses++;
		if (missed_pulses <= 0) missed_pulses = 1;

		// 디스크립터 루프
		dNext = m_desclist;
		while ((d = dNext))
		{
			dNext = d->m_pNext;

			// 닫고 있을 때는 무시
			if (STATE(d) == CON_CLOSE || STATE(d) == CON_DISCONNECT) continue;

			// 시간 조정
			if (d->m_timestamp >=0) d->m_timestamp += missed_pulses;

// 050224 : bs : 오류 상태 소켓 바로 닫기 추가
			if (FD_ISSET(d->m_desc, &m_exc_set))
			{
				d->m_bclosed = true;
				CloseSocket(d);
				continue;
			}
// --- 050224 : bs : 오류 상태 소켓 바로 닫기 추가

			if (FD_ISSET(d->m_desc, &m_input_set))
			{
				// 입력 오류시 -> 접속 끊김
				if (STATE(d) != CON_DISCONNECT && d->ProcessInput() < 0)
				{
					d->m_bclosed = true;
					CloseSocket(d);
					continue;
				}
			}

			if (d->m_waitbill)
			{
				d->m_billtimeout++;
				if (d->m_billtimeout > 600)
				{
					GAMELOG << init("BILLING ERROR")
							<< "TIMEOUT" << end;

					// 롤백 시리얼에 해당하는 것 로올배액

					CDBCmd cmd;
					cmd.Init(&m_dbuser);
					sprintf(g_buf, "delete from t_purchase where a_serial = %d", d->m_waitbill);
					cmd.SetQuery(g_buf);

					if( !cmd.Update() )
					{
						GAMELOG << init("CASH_PURCHASE_REP_ERROR-RollBack-update fail")
								<< d->m_waitbill
								<< end;
					}

					d->WaitBillReply(0);
				}
				continue ;
			}

			// 정상 연결 수 증가
			count ++;

			// 입력 메시지 가져오기
			if (!d->m_inQ.Get(inputmsg))
			{
				// 입력없을 경우

				// 로그인 상태일때
				if (STATE(d) == CON_GET_LOGIN)
				{
					// 타임 아웃 처리
					d->m_connectreq ++;
					if (d->m_connectreq > 3600)
					{
						GAMELOG << init("BAD_CONNECTION", d->m_host)
								<< end;

						d->m_bclosed = true;
						CloseSocket (d);
					}
				}
				continue;
			}

			do {

				// 입력 메시지를 받아서

				// 진행 중일때
				if (STATE(d) == CON_PLAYING)
				{
					// 명령 처리 루틴으로
					CommandInterpreter(d, inputmsg);
					if (m_bshutdown)
						break;
				}

				// 로그인 상태일때
				else if (STATE(d) == CON_GET_LOGIN)
				{
					// 로그인 처리로
					if (!d->GetLogin(inputmsg))
					{
						// 로그인 실패
						d->m_bclosed = true;
						CloseSocket(d);
							break;
					}
					else
					{
						// 로그인 성공
						GAMELOG << init("CONNECTED", d->m_host)
								<< d->m_bLoginServer
								<< delim
								<< d->m_subno
								<< end;
					}
				}

			} while (d->m_inQ.Get(inputmsg));

		} // 디스크립터 루프 끝


		// 각 플레이어 상태를 체크하고 그에 따른 행동을 취함
		CheckPlayers();


		// 빌링 시스템 연결 처리
		// 빌링 시스템과 연결 중일때
		if (m_billing.IsRunning())
		{
			// FD SET 설정
			fd_set in_conn, out_conn, ex_conn;
			FD_ZERO(&in_conn);
			FD_ZERO(&out_conn);
			FD_ZERO(&ex_conn);
			FD_SET(m_billing.m_desc, &in_conn);
			FD_SET(m_billing.m_desc, &out_conn);
			FD_SET(m_billing.m_desc, &ex_conn);

			// 폴링
			a_NullTime.tv_sec = 0;
			a_NullTime.tv_usec = 0;
			int ret_sel = select(m_billing.m_desc + 1, &in_conn, &out_conn, &ex_conn, &a_NullTime);
			a_NullTime.tv_sec = 0;
			a_NullTime.tv_usec = 0;

			// 폴링 실패는 연결 해제로, 오류 셋에 속해도 연결 해제
			if (ret_sel < 0 || FD_ISSET(m_billing.m_desc, &ex_conn))
				m_billing.Close();

			else
			{
// 050224 : bs : 입력 처리 버그 수정(밀려있는 메시지 처리 불가능 부분 수정)
				bool bsuccess = true;

				// 입력 셋일때
				if (FD_ISSET(m_billing.m_desc, &in_conn))
				{
					// 입력 오류 검사
					if (!m_billing.ProcessInput())
					{
						bsuccess = false;
						m_billing.Close();
					}
				} // end input fd set

				if (bsuccess)
				{
					int i;
					for (i = 0; i <= PROC_CONNECTOR_INPUT_PER_PASS; i++)
					{
						// 입력은 있었지만 메시지 하나를 완성 못하면 의미가 없지
						// 오류는 아니니 소켓은 안 닫음
						/*if (!m_billing.ProcessInputQueue())
							break;*/

						// 입력큐에 있는 메시지 처리
						m_billing.ProcessBilling();
					}

					// 출력 셋일때
					if (FD_ISSET(m_billing.m_desc, &out_conn))
					{
						// 쓰기
						m_billing.ProcessOutput();
					} // end output fd set
				}
// --- 050224 : bs : 입력 처리 버그 수정(밀려있는 메시지 처리 불가능 부분 수정)

			} // end if (ret_sel)
		}

		// 접속이 안되어 있고, 현재 접속 시도 중이 아닐때
		else if (!m_billing.IsTrying())
		{
			// 쓰레드를 만들어서 계속적인 접속 시도
			m_billing.m_btrying = true;
#ifdef CIRCLE_WINDOWS
			_beginthread(CBillingClient::BillingConnect, 0, &m_billing);
#else
			pthread_t threadBilling;
			pthread_create(&threadBilling, NULL, CBillingClient::BillingConnect, &m_billing);
#endif
		}


		// 셧다운 검사
		if (m_bshutdown)
			break;

		// 각 디스크립터의 출력 큐에 있는 내용 쓰기
		dNext = m_desclist;
		while ((d = dNext))
		{
			dNext = d->m_pNext;

			// 접속종료시켜야 할 디스크립터 검사
			if (d->m_bclosed || STATE(d) == CON_CLOSE || STATE(d) == CON_DISCONNECT)
			{
				CloseSocket(d);
				continue;
			}

			// 출력할 데이터 있는 디스크립터는 쓰기
			if (d->m_outBuf.GetNextPoint() && FD_ISSET(d->m_desc, &m_output_set))
			{
				// 오류 검사
				if (!(d->m_bclosed) && d->ProcessOutput() < 0)
				{
					d->m_bclosed = true;
					CloseSocket(d);
					continue;
				}
			}
		}

		// 펄스 증가
		m_pulse += missed_pulses;
		HeartBeat();
		missed_pulses = 0;
	}
}

static int InitFDSET(socket_t s1, CDescriptor* slist, fd_set* infds, fd_set* outfds, fd_set* exfds)
{
	FD_ZERO(infds);
	FD_ZERO(outfds);
	FD_ZERO(exfds);
	FD_SET(s1, infds);

	int maxdesc = s1;
	CDescriptor* d;
	CDescriptor* dNext = slist;

	while ((d = dNext))
	{
		dNext = d->m_pNext;

#ifndef CIRCLE_WINDOWS
		if (d->m_desc > maxdesc)
			maxdesc = d->m_desc;
#endif
		FD_SET(d->m_desc, infds);
		FD_SET(d->m_desc, outfds);
		FD_SET(d->m_desc, exfds);
	}

	return maxdesc;
}

void CServer::Close()
{
	int i;
	i = 0;
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

#ifdef EVENT_XMAS_2007
	sprintf(g_buf, "REPLACE INTO t_event_flowertree_point (a_server, a_point, a_updatedate) VALUES (%d, %d, NOW())", gserver.m_serverno, gserver.m_unEventXmasTreePoint );
	cmd.SetQuery(g_buf);
	cmd.Update();
#endif //EVENT_XMAS_2007

#ifdef EVENT_FLOWERTREE_2007
	sprintf(g_buf, "REPLACE INTO t_event_flowertree_point (a_server, a_point, a_updatedate) VALUES (%d, %d, NOW())", gserver.m_serverno, gserver.m_nEventFlowerTree2007StatusCurServer);
	cmd.SetQuery(g_buf);
	cmd.Update();
#endif //EVENT_FLOWERTREE_2007

#ifdef EVENT_GOMDORI_2007
	sprintf(g_buf, "REPLACE INTO t_event_gomdori_count (a_server, a_count, a_updatedate) VALUES (%d, %d, NOW())", gserver.m_serverno, gserver.m_nEventGomdori2007StatusCurServer);
	cmd.SetQuery(g_buf);
	cmd.Update();
#endif // EVENT_GOMDORI_2007

#ifdef EXTERNAL_COUNT
	for(int s = 0; s < gserver.m_maxSubServer; s++)
	{
		
		for(i = 0; i < gserver.m_extTableCount; i++)
		{
			StrLwr(gserver.m_extTableList[i]);
			sprintf(g_buf, "insert into t_connect_%s ( a_count, a_date, a_server, a_sub_num ) "
				"values ( 0, NOW(), %d, %d) ", gserver.m_extTableList[i], m_serverno, s+1);

			StrUpr(gserver.m_extTableList[i]);

			cmd.SetQuery(g_buf);
			cmd.Update();
		}
		/*
		sprintf(g_buf, "insert into t_connect_nm ( a_count, a_date, a_server, a_sub_num ) "
			"values ( 0, NOW(), %d, %d) ", m_serverno, s+1);

		cmd.SetQuery(g_buf);
		cmd.Update();

		//GAMELOG << init("NM_COUNT") << "Sub" << delim << (s+1) << delim << m_userList[s].m_nmCount << end;
		*/
	}
#endif
#ifdef LIMIT_CATALOG
	// 기존에 리스트를 모두 지우고
	cmd.BeginTrans();
	sprintf(g_buf, "delete from t_ct_limit where a_server = %d ", gserver.m_serverno);
	cmd.SetQuery(g_buf);
	cmd.Update();
	// 한정 상품이 팔린 갯수를 디비에 저장
	int count;
	void* pos;
	bool bSuccess = true;
	count = gserver.m_limitSell.GetCount();
	pos = gserver.m_limitSell.GetHead();
	CLimitSell* limit;
	for(i = 0; i < count; i++)
	{
		if( !pos ) 
			continue;
		limit = gserver.m_limitSell.GetData(pos);
		
		sprintf(g_buf, "insert into t_ct_limit (a_server, a_ct_id, a_sell ) values( %d, %d, %d) ", gserver.m_serverno, limit->GetIndex(), limit->GetSell() );
		cmd.SetQuery(g_buf);

		if( !cmd.Update() )
		{
			bSuccess = false;
		}

		pos = gserver.m_limitSell.GetNext(pos);
	}

	if( bSuccess )
	{
		cmd.Commit();
		GAMELOG << init("SYSTEM")
				<< "t_ct_limt UPDATED" << end;
	}
	else
	{
		cmd.Rollback();
		GAMELOG << init("SYSTEM")
				<< "t_ct_limit INSERT FAILED ROLLBACK" << end;
	}
#endif

	CDescriptor* d;
	CDescriptor* dNext = m_desclist;

	while ((d = dNext))
	{
		dNext = d->m_pNext;
		CloseSocket(d);
	}

	GAMELOG << init("SYSTEM") << "Closing all sockets." << end;

	CLOSE_SOCKET(m_ssock);

	InitUserData();

	GAMELOG << init("SYSTEM") << "Normal termination of game." << end;

	puts("Normal termination of game.");
#ifdef CIRCLE_WINDOWS
	Sleep (10000);
#else
	sleep (10);
#endif
}

void CServer::HeartBeat()
{
	//	GetTimeofDay( &m_last_time, 0 );
	SavePlayersCount();
	
	if (m_pulseDBPing > 300)
	{
		m_pulseDBPing = 0;
		mysql_ping(&m_dbauth);
		mysql_ping(&m_dbuser);
#if defined (EVENT_2PAN4PAN_GOODS) || defined (ATTENDANCE_EVENT) || defined ( ATTENDANCE_EVENT_REWARD ) || defined ( IRIS_POINT ) || defined (EVENT_NOM_MOVIE)
		mysql_ping(&m_dbevent);
#endif // defined (EVENT_2PAN4PAN_GOODS) || defined (ATTENDANCE_EVENT) || defined ( ATTENDANCE_EVENT_REWARD ) || defined ( IRIS_POINT )
#if defined ( CHAR_LOG ) && defined ( LC_KOR )
		mysql_ping(&m_dblog);
#endif // #if defined ( CHAR_LOG ) && defined ( LC_KOR )
	}
	else
	{
		m_pulseDBPing++;
	}
	
#if !defined (USE_TENTER_BILLING)
	if( gserver.m_billing.IsRunning() )
	{
		gserver.m_serverAliveTime--;
		
		if( gserver.m_serverAliveTime < 0 )
		{
			// 빌링서버에 얼라이브 패킷을 보낸다.
			CBPacket pack;
			pack.Init(MSG_BILLITEM_SERVERLIVE);
			gserver.m_billing.WriteToOutput(pack);
		}
	}
#endif

#ifdef LC_TIME
	//	if( m_perSecond > 0 )
	//		m_perSecond--;
	//	if( m_perSecond <= 0 )
	{
		// 해당 서브 서버를 찾는다.
		CDescriptor* p;
		CDescriptor* pNext = gserver.m_desclist;
		int count = 0;
		while ((p = pNext))
		{
			pNext = p->m_pNext;
			if( p->m_subno > 0 && p->m_subno <= gserver.m_maxSubServer )
			{
				count++;
				if( m_oldLcTime != m_lcTime )
				{
					CNetMsg msg;
					msg.Init(MSG_CONN_REP);
					msg << (char) MSG_CONN_LCTIME
						<< m_lcTime
						<< m_startTime;
					SEND_Q(msg, p);
					
					if( count == gserver.m_maxSubServer )
					{
						m_oldLcTime = m_lcTime;
//						GAMELOG << init("SERVER TIME CHECK") << m_lcTime << delim << m_startTime << end;
					}
				}
			}
		}
		
		if( bStartLctime && count >= gserver.m_maxSubServer )
		{
			struct timeval interval_time;
			GetTimeofDay( &m_cur_time, 0 );
			
			if( m_last_time.tv_sec == 0 )
				m_last_time = m_cur_time;
			
			TimeDiff( &interval_time, &m_cur_time, &m_last_time );
			if( interval_time.tv_sec > 0 )
			{
				m_last_time = m_cur_time;
				if( (m_last_time.tv_usec - interval_time.tv_usec) >= 0 )
				{
					m_last_time.tv_usec = m_last_time.tv_usec - interval_time.tv_usec;
				}
				else
				{
					m_last_time.tv_sec--;
					m_last_time.tv_usec = 1000000 - ( interval_time.tv_usec - m_last_time.tv_usec );
				}
			}
			m_lcTime += interval_time.tv_sec;			// 라카력
			
			// 라카력 저장 주기는? 한 2분30?
			if( !(m_lcTime % LCHOUR) )
			{
				CDBCmd cmd;
				cmd.Init(&gserver.m_dbuser);
				sprintf(g_buf, "update t_lc_time set a_time = %d where a_server = %d ", m_lcTime - this->m_startTime, m_serverno);
				
				cmd.SetQuery(g_buf);
				cmd.Update();
			}
		}
	}
#endif
	
#ifdef EVENT_GOMDORI_2007
	struct tm tmCur = NOW();
	int nTimeDiff = tmCur.tm_min - gserver.m_nEventGomdori2007LastUpdate;
	if (nTimeDiff < 0)
		nTimeDiff += 60;
	if (nTimeDiff >= 5)
	{
		gserver.m_nEventGomdori2007LastUpdate = tmCur.tm_min;
		CDBCmd cmd;
		cmd.Init(&gserver.m_dbuser);
		sprintf(g_buf, "REPLACE INTO t_event_gomdori_count (a_server, a_count, a_updatedate) VALUES (%d, %d, NOW())", gserver.m_serverno, gserver.m_nEventGomdori2007StatusCurServer);
		cmd.SetQuery( g_buf );
		cmd.Update();
		
	}
#endif // EVENT_GOMDORI_2007
	
#ifdef EVENT_FLOWERTREE_2007
	struct tm currentTime = NOW();
	int nFlowerTreeTimeDiff = currentTime.tm_min - gserver.m_nEventFlowerTree2007LastUpdate;
	if (nFlowerTreeTimeDiff < 0)
		nFlowerTreeTimeDiff += 60;
	if (nFlowerTreeTimeDiff >= 5)
	{
		gserver.m_nEventFlowerTree2007LastUpdate = currentTime.tm_min;
		CDBCmd cmd;
		cmd.Init(&gserver.m_dbuser);
		sprintf(g_buf, "REPLACE INTO t_event_flowertree_point (a_server, a_point,  a_lastupdate) VALUES (%d, %d, NOW())", gserver.m_serverno, gserver.m_nEventFlowerTree2007StatusCurServer);
		cmd.SetQuery( g_buf );
		cmd.Update();
	}
#endif // EVENT_FLOWERTREE_2007
	
#ifdef EVENT_XMAS_2007
	struct tm currentTime = NOW();
	int nXMasTreeTimeDiff = currentTime.tm_min - gserver.m_nEventXmasTreeLastUpdate;
	if (nXMasTreeTimeDiff < 0)
		nXMasTreeTimeDiff += 60;
	if ( nXMasTreeTimeDiff >= 5)
	{
		gserver.m_nEventXmasTreeLastUpdate = currentTime.tm_min;
		CDBCmd cmd;
		cmd.Init(&gserver.m_dbuser);
		sprintf(g_buf, "REPLACE INTO t_event_xmastree_point (a_server, a_point,  a_lastupdate) VALUES (%d, %d, NOW())", gserver.m_serverno, gserver.m_unEventXmasTreePoint);
		cmd.SetQuery( g_buf );
		cmd.Update();
	}
	
	TimerToGameServer();
	
#endif // EVENT_XMAS_2007
	//		m_perSecond = PULSE_REAL_SEC;
}