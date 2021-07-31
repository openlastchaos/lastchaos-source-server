#include "stdhdrs.h"
#include "Server.h"
#include "Log.h"
#include "CmdMsg.h"

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

	/* initialize various time values */
	opt_time.tv_usec = OPT_USEC;
	opt_time.tv_sec = 0;
	FD_ZERO(&m_null_set);
	GetTimeofDay(&last_time, (struct timezone *) 0);

	/* The Main Loop.  The Big Cheese.  The Top Dog.  The Head Honcho.  The.. */
	while (!m_bshutdown)
	{
		/* Set up the m_input, m_output, and exception sets for select(). */
		maxdesc = InitFDSET(m_ssock, m_desclist, &m_input_set, &m_output_set, &m_exc_set);

		/*
		* At this point, we have completed all m_input, m_output and heartbeat
		* activity from the previous iteration, so we have to put ourselves
		* to sleep until the next 0.1 second tick.  The first step is to
		* calculate how long we took processing the previous iteration.
		*/
    
		GetTimeofDay(&before_sleep, (struct timezone *) 0); /* current time */
		TimeDiff(&process_time, &before_sleep, &last_time);

		// 펄스 조정
		if (process_time.tv_sec == 0 && process_time.tv_usec < OPT_USEC)
		{
			missed_pulses = 0;
		} 
		else
		{
			missed_pulses = process_time.tv_sec * PASSES_PER_SEC;
			missed_pulses += process_time.tv_usec / OPT_USEC;
			process_time.tv_sec = 0;
			process_time.tv_usec = process_time.tv_usec % OPT_USEC;
		}

		/* Calculate the time we should wake up */
		TimeDiff(&temp_time, &opt_time, &process_time);
		TimeAdd(&last_time, &before_sleep, &temp_time);

		/* Now keep sleeping until that time has come */
		GetTimeofDay(&now, (struct timezone *) 0);
		TimeDiff(&timeout, &last_time, &now);

		/* Go to sleep */
		do {
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

		/* Poll (without blocking) for new m_input, m_output, and exceptions */
		a_NullTime.tv_sec = 0;
		a_NullTime.tv_usec = 0;
		if (select(maxdesc + 1, &m_input_set, &m_output_set, &m_exc_set, &a_NullTime) < 0)
		{
			GAMELOG << init("SYS_ERR")
					<< "Select poll"
					<< end;
			return;
		}

		/* If there are new connections waiting, accept them. */
		if (FD_ISSET(m_ssock, &m_input_set))
			NewDescriptor(m_ssock);

		/*
		* Now, we execute as many pulses as necessary--just one if we haven't
		* missed any pulses, or make up for lost time if we missed a few
		* pulses by sleeping for too long.
		*/
		missed_pulses++;

		if (missed_pulses <= 0) missed_pulses = 1;

		dNext = m_desclist;
		while ((d = dNext))
		{
			dNext = d->m_pNext;

			if (STATE(d) == CON_CLOSE || STATE(d) == CON_DISCONNECT) continue;

			if (d->m_timestamp >=0) d->m_timestamp += missed_pulses;

			/* Process descriptors with m_input pending */
			if (FD_ISSET(d->m_desc, &m_exc_set))
			{
				FD_CLR(d->m_desc, &m_input_set);
				FD_CLR(d->m_desc, &m_output_set);

				d->m_bclosed = true;
				CloseSocket(d);
				continue;
			}

			/* Kick out the freaky folks in the exception set and marked for close */
			if (FD_ISSET(d->m_desc, &m_input_set)) {
				if (STATE(d) != CON_DISCONNECT && d->ProcessInput() < 0) {
					d->m_bclosed = true;
					CloseSocket(d);
					continue;
				}
			}

			if (!d->m_inQ.Get(inputmsg))
			{
				// 로그인 타임 아웃 검사
				if (STATE(d) == CON_GET_LOGIN)
				{
					d->m_connectreq ++;
#ifdef _DEBUG
					if (d->m_connectreq > PULSE_REAL_MIN)
#else // _DEBUG
					if (d->m_connectreq > PULSE_REAL_SEC * 10)
#endif // _DEBUG
					{
						/* m_input wait time 30 seconds */
						GAMELOG << init("BAD_CONNECTION")
								<< d->m_host
								<< end;

						d->m_bclosed = true;
						CloseSocket (d);
					}
				}
				continue;
			}

			// 소켓 연결은 최대 1분
			if (m_pulse - d->m_pulseConnected > PULSE_REAL_MIN)
			{
				GAMELOG << init("CONNECTION TIMEOUT")
						<< d->m_host
						<< end;

				d->m_bclosed = true;
				CloseSocket (d);
				continue ;
			}

//0707
#ifdef COMPRESSION_MESSAGE	
				CNetMsg nmUnpackMessage(inputmsg.GetType());
				inputmsg.UnpackDefault(nmUnpackMessage);

				inputmsg = nmUnpackMessage;
#endif

#ifdef CRYPT_NET_MSG
#ifdef CRYPT_NET_MSG_MANUAL
				int nDecryptBuffer = CNM_Decrypt(inputmsg.m_buf, inputmsg.m_size, &d->m_nCryptKey, pDecryptBuffer, pTmpDecryptBuffer);
#else // CRYPT_NET_MSG_MANUAL
				int nDecryptBuffer = CNM_Decrypt(inputmsg.m_buf, inputmsg.m_size, &d->m_nCryptKeyClient, pDecryptBuffer, pTmpDecryptBuffer);
#endif // CRYPT_NET_MSG_MANUAL
				if (nDecryptBuffer <= 0)
				{
					inputmsg.Init(MSG_UNKNOWN);
					CloseSocket(d);
				}
				else
				{
#ifndef CRYPT_NET_MSG_MANUAL
					CNM_NextKey(&d->m_nCryptKeyClient);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
					inputmsg.WriteRaw(pDecryptBuffer, nDecryptBuffer);
				}
#endif // #ifdef CRYPT_NET_MSG

			do {

				if (STATE(d) == CON_GET_LOGIN)
				{
					if (!d->GetLogin(inputmsg))
					{
						inputmsg.MoveFirst();
						int version;
						unsigned char mode;
						CLCString id(MAX_ID_NAME_LENGTH + 1);
						CLCString pw(MAX_PWD_LENGTH + 1);
						
						inputmsg >> version >> mode >> id >> pw;

						GAMELOG << init("BAD_CONNECTION")
								<< d->m_host << delim
								<< version << delim << mode
								<< end;

						d->m_bclosed = true;
						CloseSocket(d);
						continue;
					}
				}
#ifdef CHECK_SECURE_CARD
				else if (STATE(d) == CON_WAIT_SCARD)
				{
					if (!d->GetSCard(inputmsg))
					{
						GAMELOG << init("SCARD BAD_CONNECTION")
								<< d->m_host
								<< end;

						d->m_bclosed = true;
						CloseSocket(d);
						continue;
					}
				}
#endif // CHECK_SECURE_CARD
			
			} while (d->m_inQ.Get(inputmsg));

		} // 디스크립터 루프 끝

		if (!m_brunmessenger && m_messenger)
		{
			if (m_messenger->m_bclosed)
			{
				m_messenger->CloseSocket();
				delete m_messenger;
				m_messenger = NULL;
			}
			else
			{
				// FD SET 설정
				fd_set in_msgr, out_msgr, ex_msgr;
				FD_ZERO(&in_msgr);
				FD_ZERO(&out_msgr);
				FD_ZERO(&ex_msgr);
				FD_SET(m_messenger->m_desc, &in_msgr);
				FD_SET(m_messenger->m_desc, &out_msgr);
				FD_SET(m_messenger->m_desc, &ex_msgr);

				// 폴링
				a_NullTime.tv_sec = 0;
				a_NullTime.tv_usec = 0;
				int ret_sel = select(m_messenger->m_desc + 1, &in_msgr, &out_msgr, &ex_msgr, &a_NullTime);
				a_NullTime.tv_sec = 0;
				a_NullTime.tv_usec = 0;

				// select 오류
				if (ret_sel < 0 || FD_ISSET(m_messenger->m_desc, &ex_msgr))
				{
					m_messenger->m_bclosed = true;
				}
				else
				{
					if (FD_ISSET(m_messenger->m_desc, &in_msgr))
					{
						if (m_messenger->ProcessInput() < 0)
							m_messenger->m_bclosed = true;
					}

					if (!m_messenger->m_bclosed)
					{
						int j;
						CNetMsg msgrmsg;
						for (j = 0; j <= PROC_MESSENGER_MSG_PER_PASS && m_messenger->m_inQ.Get(msgrmsg); j++)
						{
							ProcMessenger(msgrmsg);
						}

						if (FD_ISSET(m_messenger->m_desc, &out_msgr))
						{
							m_messenger->ProcessOutput();
						} // end output fd set
					}
				}

			} // end if (m_messenger->m_bclosed)
			
		} // end if (!m_brunmessenger && m_messenger)
		else if (!m_brunmessenger)
		{
#ifdef CIRCLE_WINDOWS
			m_brunmessenger = (_beginthread(MessengerConnect, 0, NULL) > 0);
#else
			pthread_t threadMessenger;
			m_brunmessenger = (pthread_create(&threadMessenger, NULL, MessengerConnect, NULL)) ? false : true;
#endif
		}

		// 컨넥터섭
		int i;
		for (i=0; i < m_nConnector; i++)
		{
			if (!m_brunconn[i] && m_connector[i])
			{
				if (m_connector[i]->m_bclosed)
				{
					m_connector[i]->CloseSocket();
					delete m_connector[i];
					m_connector[i] = NULL;
				}
				else
				{
					// FD SET 설정
					fd_set in_conn, out_conn, ex_conn;
					FD_ZERO(&in_conn);
					FD_ZERO(&out_conn);
					FD_ZERO(&ex_conn);
					FD_SET(m_connector[i]->m_desc, &in_conn);
					FD_SET(m_connector[i]->m_desc, &out_conn);
					FD_SET(m_connector[i]->m_desc, &ex_conn);
					
					// 폴링
					a_NullTime.tv_sec = 0;
					a_NullTime.tv_usec = 0;
					int ret_sel = select(m_connector[i]->m_desc + 1, &in_conn, &out_conn, &ex_conn, &a_NullTime);
					a_NullTime.tv_sec = 0;
					a_NullTime.tv_usec = 0;
					
					if (ret_sel < 0 || FD_ISSET(m_connector[i]->m_desc, &ex_conn))
					{
						m_connector[i]->m_bclosed = true;
					}
					else
					{
						if (FD_ISSET(m_connector[i]->m_desc, &in_conn))
						{
							if (m_connector[i]->ProcessInput() < 0)
								m_connector[i]->m_bclosed = true;
						}
						if (!m_connector[i]->m_bclosed)
						{
							int k;
							CNetMsg connmsg;
							for (k = 0; k <= PROC_CONNECTOR_MSG_PER_PASS && m_connector[i]->m_inQ.Get(connmsg); k++)
							{
								ProcConnector(connmsg);
							}

							if (FD_ISSET(m_connector[i]->m_desc, &out_conn))
							{
								m_connector[i]->ProcessOutput();
							} // end output fd set
						}
					}
					
				} // end if (m_messenger->m_bclosed)
				
			} // end if (!m_brunmessenger && m_messenger)
			else if (!m_brunconn[i])
			{
#ifdef CIRCLE_WINDOWS
				m_brunconn[i] = (_beginthread(ConnectorConnect, 0, (void*)i) > 0);
#else
				pthread_t threadConnector;
				m_brunconn[i] = (pthread_create(&threadConnector, NULL, ConnectorConnect, (void*)i)) ? false : true;
#endif
			}
		} // end for (i=0; i < m_nConnector; i++)

		if (m_bshutdown)
			break;

		// 패킷 보내기 디스크립터 루프
		dNext = m_desclist;
		while ((d = dNext))
		{
			dNext = d->m_pNext;

			// 연결 끊기
			if (d->m_bclosed || STATE(d) == CON_CLOSE || STATE(d) == CON_DISCONNECT)
			{
				CloseSocket(d);
				continue;
			}

			// 보내기
			if (d->m_outBuf.GetNextPoint() && FD_ISSET(d->m_desc, &m_output_set))
			{
				// 보내기 오류 처리
				if (!(d->m_bclosed) && d->ProcessOutput() < 0)
				{
					d->m_bclosed = true;
					CloseSocket(d);
					continue;
				}
			}
		} // 보내기 루프 끝

		m_pulse += missed_pulses;
		SendConnectorReqPlayer();
#if defined (WJKTEST) || defined (LC_USA)
		WriteDBPlayerCount();
#endif // #if defined (WJKTEST) || defined (LC_USA)

		if (m_pulseDBPing > 300)
		{
			mysql_ping(&m_dbAuth);
			mysql_ping(&m_dbuser);
			m_pulseDBPing = 0;
		}
		else
			m_pulseDBPing++;

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
	CDescriptor* d;
	CDescriptor* dNext = m_desclist;

	while ((d = dNext))
	{
		dNext = d->m_pNext;
		CloseSocket(d);
	}

	GAMELOG << init("SYSTEM")
			<< "Closing all sockets."
			<< end;

	CLOSE_SOCKET(m_ssock);
	
	GAMELOG << init("SYSTEM")
			<< "Normal termination of game."
			<< end;

	puts("Normal termination of game.");

#ifdef CIRCLE_WINDOWS
	Sleep (10000);
#else
	sleep (10);
#endif
}


void CServer::SendConnectorReqPlayer()
{
	int i, j;

	// 플레이어 수 요청 : 처음이랑 1분에 한번 : 수정 : 10초
	if (m_resetPlayerTime == 0 || m_pulse - m_resetPlayerTime >= 10 * PULSE_REAL_SEC)
	{
		m_resetPlayerTime = m_pulse;
		CNetMsg playerMsg;
		PlayerReqMsg(playerMsg);

		for (i = 0; i < m_nConnector; i++)
		{
			if (m_connector[i] == NULL || m_connector[i]->m_bclosed || m_brunconn[i])
				continue ;

			SEND_Q(playerMsg, m_connector[i]);
			m_checkConnector[i] = m_pulse;
		}
	}

	// 컨넥터 타임아웃 : 서버 다운 체크
	for (i = 0; i < m_nConnector; i++)
	{
		if (m_connector[i] == NULL || m_brunconn[i])
			continue ;

		// 타임 아웃이 걸려있고 0으로 셋팅되어 있지 않으면 무조건 섭다 : 수정 20초
		if (m_checkConnector[i] != 0  && m_pulse - m_checkConnector[i] >=  20 * PULSE_REAL_SEC)
		{
			for (j=0; j < m_connector[i]->m_nMaxServer; j++)
			{
				m_connector[i]->m_playerNum[j] = -1;
			}
		}
	}
}
