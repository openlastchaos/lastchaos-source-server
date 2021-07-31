#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "DBCmd.h"

static int InitFDSET(socket_t s1, CDescriptor* slist, fd_set* infds, fd_set* outfds, fd_set* exfds);

// BW
bool b_rankinitupdate;
bool b_rewardinitupdate;
bool b_rewardendupdate;
struct tm rankInitUpdateTime;
struct tm rewardInitUpdateTime;
struct tm rewardEndUpdateTime;

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
	CMsgListNode* msgNode;
	CMsgListNode* msgNodeNext;
	CMsgListNodeServerNode* serverNode;
	CMsgListNodeServerNode* serverNodeNext;
	int dbping = 0;

	////////////////////
	// 테섭 사항
	// 이번주에 초기화될 시간 세팅 : BW
	time_t now1;
	time( &now1 );
	

	rankInitUpdateTime = *localtime( &now1 );
	rewardInitUpdateTime = *localtime( &now1 );
	rewardEndUpdateTime = *localtime( &now1 );
	
	while(1)
	{
		rankInitUpdateTime.tm_mday = rankInitUpdateTime.tm_mday++;
		mktime( &rankInitUpdateTime );

		rewardInitUpdateTime.tm_mday = rewardInitUpdateTime.tm_mday++;
		mktime( &rewardInitUpdateTime );
		

		rewardEndUpdateTime.tm_mday = rewardEndUpdateTime.tm_mday++;
		mktime( &rewardEndUpdateTime );

		if(rankInitUpdateTime.tm_wday == 2) break;
	}
	
	
	rankInitUpdateTime.tm_hour = 8;
	rankInitUpdateTime.tm_min = 0;
	rankInitUpdateTime.tm_sec = 0;

	rewardInitUpdateTime.tm_hour = 8;
	rewardInitUpdateTime.tm_min = 20;
	rewardInitUpdateTime.tm_sec = 0;

	rewardEndUpdateTime.tm_hour = 8;
	rewardEndUpdateTime.tm_min = 10;
	rewardEndUpdateTime.tm_sec = 0;

	b_rankinitupdate = false;
	b_rewardinitupdate = false;
	b_rewardendupdate = false;

	// 시간 값 초기화
	opt_time.tv_usec = OPT_USEC;
	opt_time.tv_sec = 0;
	FD_ZERO(&m_null_set);
	GetTimeofDay(&last_time, (struct timezone *) 0);

	// 메인 루프
	while (true)
	{
		// FD_SET 설정
		maxdesc = InitFDSET(m_ssock, m_desclist, &m_input_set, &m_output_set, &m_exc_set);

		// 이전 루프 처리 시간 구하기
		GetTimeofDay(&before_sleep, (struct timezone *) 0); /* current time */
		TimeDiff(&process_time, &before_sleep, &last_time);

		// 펄스 조정
		// 프로세스 시간이 1초에 10번(OPT_USEC) 수행되는 것을 기준으로 하는데
		// 오래 걸려서 놓친 pulse가 생기면 그 수를 계산하여 서버 펄스를 조정한다
		if (process_time.tv_sec == 0 && process_time.tv_usec < OPT_USEC)
		{
			missed_pulses = 0;
		}
		else
		{
			missed_pulses = process_time.tv_sec * PASSES_PER_SEC;
			missed_pulses += process_time.tv_usec / OPT_USEC;

			// 펄스 계산후 남은 처리 시간은 보존하여 최대한 오차를 줄여간다
			process_time.tv_sec = 0;
			process_time.tv_usec = process_time.tv_usec % OPT_USEC;
		}

		// 이번 루프에 소비될 시간을 구한다
		TimeDiff(&temp_time, &opt_time, &process_time);

		// 이전 처리 시간 + 이번 처리 시간을 구한다.
		TimeAdd(&last_time, &before_sleep, &temp_time);

		// 현재 시각을 구한다.
		GetTimeofDay(&now, (struct timezone *) 0);
		// 얼마나 Sleep 할 것인가를 정한다.
		TimeDiff(&timeout, &last_time, &now);

		// 시간 조정을 위한 딜레이
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

		// 폴링 - 블럭 없이 select로 테스트
		a_NullTime.tv_sec = 0;
		a_NullTime.tv_usec = 0;
		if (select(maxdesc + 1, &m_input_set, &m_output_set, &m_exc_set, &a_NullTime) < 0)
		{
			GAMELOG << init("SYS_ERR") << "Select poll" << end;
			return;
		}

		// 새로운 접속 처리
		if (FD_ISSET(m_ssock, &m_input_set))
			NewDescriptor(m_ssock);

		// 연결되어 있는(입력을 받은) 접속 수
		count = 0;

		// 펄스를 하나 사용하므로 1 증가
		// 최소한 하나의 펄스를 사용
		missed_pulses++;
		if (missed_pulses <= 0) missed_pulses = 1;

		dbping++;
		if (dbping > 300)
		{
			mysql_ping(&m_dbchar);
			dbping = 0;
		}

		// 디스크립터 루프
		dNext = m_desclist;
		while ((d = dNext))
		{
			dNext = d->m_pNext;

			// 현재 상태가 연결이 끊어진 상태이면 스킵
			if (STATE(d) == CON_CLOSE || STATE(d) == CON_DISCONNECT) continue;

			// 디스크립터의 펄스 조정
			if (d->m_timestamp >=0) d->m_timestamp += missed_pulses;

			// 오류 발생 혹은 일정시간 입력이 없으면 연결 해제
			if (FD_ISSET(d->m_desc, &m_exc_set))
			{
				FD_CLR(d->m_desc, &m_input_set);
				FD_CLR(d->m_desc, &m_output_set);

				d->m_bclosed = true;
				CloseSocket(d);
				continue;
			}

			// 입력에서의 오류시 연결 해제
			if (FD_ISSET(d->m_desc, &m_input_set))
			{
				// 입력 쪽에서 접속이 끊어졌을 때..
				if (STATE(d) != CON_DISCONNECT && d->ProcessInput() < 0)
				{
					d->m_bclosed = true;
					CloseSocket(d);
					continue;
				}
			}

			count++;   // 정상적인 연결 수 증가

			// 입력 받은 메시지 처리
			if (!d->m_inQ.Get(inputmsg))
			{
				// 로그인 타임 아웃 검사
				if (STATE(d) == CON_GET_LOGIN)
				{
					d->m_connectreq ++;
					if (d->m_connectreq > 100)
					{
						/* m_input wait time 30 seconds */
						GAMELOG << init("BAD_CONNECTION", d->m_host) << end;

						d->m_bclosed = true;
						CloseSocket (d);
					}
				}
				continue;
			}

			do {

				// 상태별 입력 처리
				if (STATE(d) == CON_GET_LOGIN)
				{
					if (!d->GetLogin(inputmsg))
					{
						inputmsg.MoveFirst();
						int version, servernum, subnum, countzone;
						inputmsg >> version >> servernum >> subnum >> countzone;

						GAMELOG << init("BAD_VERSION", d->m_host) << version << delim << servernum << delim << subnum << delim << countzone << end;

						d->m_bclosed = true;
						CloseSocket(d);
						break;
					}
				}
				else
				{
					CommandInterpreter(d, inputmsg);
				}

			} while (d->m_inQ.Get(inputmsg));

		} // 디스크립터 루프 끝

		if (m_bshutdown)
			break;

		// 메시지 리스트 처리
		msgNodeNext = m_msgList.m_head;
		while ((msgNode = msgNodeNext))
		{
			msgNodeNext = msgNode->m_pNext;

			// 이미 보낸 메시지이면
			if (msgNode->m_bSent)
			{
				// 타임 아웃 검사하고
				// 5초 이상 걸리면 타임 아웃
				if (msgNode->m_sendTime++ > 5 * PULSE_REAL_SEC)
				{
					ProcTimeout(msgNode);
				}
			}
			// 보내기 전이면
			else
			{
				// 보내기

				// 보낸 정보 저장하고
				msgNode->m_bSent = true;
				msgNode->m_sendTime = 0;

				// 대상 서버에 보내기
				serverNodeNext = msgNode->m_head;
				while ((serverNode = serverNodeNext))
				{
					serverNodeNext = serverNode->m_pNext;
					SEND_Q(msgNode->m_msg, serverNode->m_server);
				}

				// 응답이 필요없는 메시지는 버리기
				if (!msgNode->m_bReq)
				{
					REMOVE_FROM_BILIST(msgNode, m_msgList.m_head, m_pPrev, m_pNext);
					delete msgNode;
				}
			}
		}

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

		// 펄스 적용
		m_pulse += missed_pulses;
		// 연결 수 적용
		m_nDesc = count;
		// 펄스 소모 카운트 리셋
		missed_pulses = 0;

		HeartBeat();
		
#ifdef TRADE_AGENT		//거래 대행(경매) 
		// 마감일 체크
		gserver.m_TradeAgentList.CheckFinishday();

#endif	//TRADE_AGENT

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

	GAMELOG << init("SYSTEM") << "Saving All User Data...." << end;

	while ((d = dNext))
	{
		dNext = d->m_pNext;
		CloseSocket(d);
	}

	GAMELOG << init("SYSTEM") << "Closing all sockets." << end;

	CLOSE_SOCKET(m_ssock);

	GAMELOG << init("SYSTEM") << "Normal termination of game." << end;
#ifdef CIRCLE_WINDOWS
	Sleep (10000);
#else
	sleep (10);
#endif
}

void CServer::ProcTimeout(CMsgListNode* msgNode)
{
	int seq, server, subno, zone;
	unsigned char subtype;

	CNetMsg& msg = msgNode->m_msg;

	if (msg.m_mtype != MSG_HELPER_REQ)
		return ;

	msg.MoveFirst();
	msg >> seq
		>> server >> subno >> zone
		>> subtype;

	GAMELOG << init("TIMEOUT")
			<< "server" << delim << server << delim << "sub" << delim << subno << delim
			<< "messege" << delim << msg.m_mtype << delim << subtype << end;

	// 요청했던 서버 찾기
	CDescriptor* d;
	CDescriptor* dNext = m_desclist;

	while ((d = dNext))
	{
		dNext = d->m_pNext;

		if (d == msgNode->m_reqServer)
			break;
	}

	if (d)
	{
		// 메시지에 따라 다른 처리
		switch (subtype)
		{
		case MSG_HELPER_WHISPER_REQ:
			{
				int sidx;
				CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
				msg >> sidx >> sname;

				CNetMsg mmsg;
				HelperWhisperNotfoundMsg(mmsg, seq, server, subno, zone, sidx, sname);

				SEND_Q(mmsg, msgNode->m_reqServer);
			}
			break;
		}
	}

	// 노드 지우기
	REMOVE_FROM_BILIST(msgNode, m_msgList.m_head, m_pPrev, m_pNext);
	delete msgNode;
}

void CServer::HeartBeat()
{


	///////////////////////////////
	// pd4 랭킹 초기화 : BW
	// 화요일 AM0:00 rankinit 
	// 화요일 AM 11:00 rewardinit
	// 화요일 AM 09:00 rewardend
	//테섭 코드
	struct tm when;
	time_t now;
	time( &now );
	when = *localtime( &now );

	if( ( mktime(&rankInitUpdateTime) <= mktime(&when)) && b_rankinitupdate == true)
	{ 
		b_rankinitupdate = false;
	}
	if( ( mktime(&rewardInitUpdateTime ) <= mktime(&when)) && b_rewardinitupdate == true)
	{
		b_rewardinitupdate = false;
	}
	if( ( mktime(&rewardEndUpdateTime) <= mktime(&when)) && b_rewardendupdate == true)
	{
		b_rewardendupdate = false;
	}

	if( ( mktime(&rankInitUpdateTime) <= mktime(&when)) && b_rankinitupdate == false)
	{

		while(1)
		{
			rankInitUpdateTime.tm_mday++;
			mktime( &rankInitUpdateTime );
			if(rankInitUpdateTime.tm_wday == 2) break;
		}

		// 보낼 메시지 없음
		CNetMsg rmsg;
		do_CommandPD4RankInit(rmsg, (CDescriptor*) 0);

		rankInitUpdateTime.tm_hour = 8;
		rankInitUpdateTime.tm_min = 0;
		rankInitUpdateTime.tm_sec = 0;

		mktime(&rankInitUpdateTime);

//		printf("%s\n", asctime(&rankInitUpdateTime) );
		GAMELOG << init("PD4 RANK INIT") << end;
		b_rankinitupdate = true;

	}

	if( ( mktime(&rewardEndUpdateTime) <= mktime(&when)) && b_rewardendupdate == false)
	{
		while(1)
		{
			rewardEndUpdateTime.tm_mday++;
			mktime( &rewardEndUpdateTime );
			if(rewardEndUpdateTime.tm_wday == 2) break;
		}

		// 보낼 메시지 없음
		CNetMsg rmsg;
		do_CommandPD4RewardEnd(rmsg, (CDescriptor*) 0);

		rewardEndUpdateTime.tm_hour = 8;
		rewardEndUpdateTime.tm_min = 10;
		rewardEndUpdateTime.tm_sec = 0;
		mktime(&rewardEndUpdateTime);
		
		GAMELOG << init("PD4 REWARD END") << end;
		b_rewardendupdate = true;
	}

	if( ( mktime(&rewardInitUpdateTime) <= mktime(&when)) && b_rewardinitupdate == false)
	{
		while(1)
		{
			rewardInitUpdateTime.tm_mday++;
			mktime( &rewardInitUpdateTime );
			if(rewardInitUpdateTime.tm_wday == 2) break;

		}

		// 보낼 메시지 없음
		CNetMsg rmsg;
		do_CommandPD4RewardInit(rmsg, (CDescriptor*) 0);

		rewardInitUpdateTime.tm_hour = 8;
		rewardInitUpdateTime.tm_min = 20;
		rewardInitUpdateTime.tm_sec = 0;
		mktime(&rewardInitUpdateTime);

		GAMELOG << init("PD4 REWARD INIT") << end;
		b_rewardinitupdate = true;
	}


	// 길드전 중인 길드 시간 체크
	CGuild* g;
	CGuild* gNext = m_guildlist.head();

	while ((g = gNext))
	{
		gNext = gNext->nextguild();

		if (g->battleState() == GUILD_BATTLE_STATE_PEACE)
			continue;

		int diffPulse = m_pulse - g->battlePulse();
		int battleTime = g->battleTime() - diffPulse;

		// 길드전 중일때
		switch (g->battleState())
		{
		case GUILD_BATTLE_STATE_START_WAIT:
		case GUILD_BATTLE_STATE_STOP_WAIT:
			break;

		case GUILD_BATTLE_STATE_WAIT:

			if (diffPulse > GUILD_BATTLE_WAIT_TIME)
			{
				int settime = g->battleTime() - GUILD_BATTLE_WAIT_TIME;

				// 길드전 시작요청
				CGuild* guild = gserver.m_guildlist.findguild(g->battleIndex());
				if (!guild)
					continue;

				// DB UPDATE
				sprintf (g_buf, "UPDATE t_guild SET a_battle_time=%d, a_battle_state=%d WHERE a_index=%d",
					settime, GUILD_BATTLE_STATE_ING, g->index());
				CDBCmd cmd;
				cmd.Init(&gserver.m_dbchar);
				cmd.SetQuery(g_buf);
				if (!cmd.Update())
				{
					continue;
				}
				
				sprintf (g_buf, "UPDATE t_guild SET a_battle_time=%d, a_battle_state=%d WHERE a_index=%d",
					settime, GUILD_BATTLE_STATE_ING, guild->index());
				cmd.Init(&gserver.m_dbchar);
				cmd.SetQuery(g_buf);
				if (!cmd.Update())
				{
					continue;
				}

				// 세부 셋팅
				g->BattleTime(settime);
				g->BattleState(GUILD_BATTLE_STATE_ING);
				g->BattlePulse(m_pulse);

				guild->BattleTime(settime);
				guild->BattleState(GUILD_BATTLE_STATE_ING);
				guild->BattlePulse(m_pulse);

				GAMELOG << init("GUILD BATTLE START")
						<< g->index() << delim
						<< g->name() << delim
						<< "<==>" << delim
						<< guild->index() << delim
						<< guild->name() << delim
						<< g->battlePrize() << delim
						<< g->battleZone() << delim
						<< g->battleTime() << delim
						<< end;

				CNetMsg rmsg;
				HelperGuildBattleStartMsg(rmsg, g, guild);
				gserver.SendToAllGameServer(rmsg);
			}
			break;

		case GUILD_BATTLE_STATE_ING:

			// 길드전 끝
			if (battleTime < 1)
			{
				CGuild* guild = gserver.m_guildlist.findguild(g->battleIndex());

				if (!guild)
					continue;

				CNetMsg tmsg;
				tmsg.Init(MSG_HELPER_COMMAND);
				tmsg	<< MSG_HELPER_GUILD_BATTLE_STOP_REQ
						<< g->index()
						<< guild->index();
				
				do_Command(tmsg, NULL);
			}

			// 길드전 상황 보고 1분
			if (diffPulse > 1200)
			{
				CGuild* guild = gserver.m_guildlist.findguild(g->battleIndex());
				if (!guild)
					continue;

				// DB UPDATE
				sprintf (g_buf, "UPDATE t_guild SET a_battle_time=%d WHERE a_index=%d",
					battleTime, g->index());
				CDBCmd cmd;
				cmd.Init(&gserver.m_dbchar);
				cmd.SetQuery(g_buf);
				if (!cmd.Update())
				{
					continue;
				}
				
				sprintf (g_buf, "UPDATE t_guild SET a_battle_time=%d WHERE a_index=%d",
					battleTime, guild->index());
				cmd.Init(&gserver.m_dbchar);
				cmd.SetQuery(g_buf);
				if (!cmd.Update())
				{
					continue;
				}

				g->BattleTime(battleTime);
				g->BattlePulse(m_pulse);

				guild->BattleTime(battleTime);
				guild->BattlePulse(m_pulse);

				CNetMsg rmsg;
				HelperGuildBattleStatusMsg(rmsg, g, guild);
				gserver.SendToAllGameServer(rmsg);
			}

			break;
		}
	}
}