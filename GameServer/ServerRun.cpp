#include "stdhdrs.h"
#include "Log.h"
#include "Exp.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"

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

	/* initialize various time values */
	opt_time.tv_usec = OPT_USEC;
	opt_time.tv_sec = 0;
	FD_ZERO(&m_null_set);
	GetTimeofDay(&last_time, (struct timezone *) 0);

	/* The Main Loop.  The Big Cheese.  The Top Dog.  The Head Honcho.  The.. */
	while (!m_bReboot)
	{
//		try
//		{
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
			
			/*
			* If we were asleep for more than one pass, count missed pulses and sleep
			* until we're resynchronized with the next upcoming m_pulse.
			*/
			if (process_time.tv_sec == 0 && process_time.tv_usec < OPT_USEC) {
				missed_pulses = 0;
			} else {
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
			a_NullTime.tv_sec = 0;
			a_NullTime.tv_usec = 0;
			
			/* If there are new connections waiting, accept them. */
			if (FD_ISSET(m_ssock, &m_input_set))
				NewDescriptor(m_ssock);
			
			/* Process commands we just read from process_input */
			count = 0;
			
			/*
			* Now, we execute as many pulses as necessary--just one if we haven't
			* missed any pulses, or make up for lost time if we missed a few
			* pulses by sleeping for too long.
			*/
			missed_pulses++;
			
			if (missed_pulses <= 0) missed_pulses = 1;

#ifdef LC_TLD
			if(m_now.tm_mday != NOW().tm_mday )
			{
				m_now = NOW();
#ifdef EVENT_OPEN_BETA_TLD
				if( m_bOpenEvent )
				{
					
					if ( m_now.tm_year == (2005 - 1900) 
						&& ( (m_now.tm_mon == 10 && m_now.tm_mday >= 18 ) 
						   ||(m_now.tm_mon == 11  && m_now.tm_mday <= 5 ) ) )
					{
						// 0 : 모두 드롭 1: 배양토 2: 레드 3: 푸른 4: 황금 5: 배양,레드 6:레드,푸른 7:푸른,골드 8: 모두 안 드롭
						if(m_now.tm_mday >= 18 && m_now.tm_mday < 21)
							m_bSoilDrop = 1;
						if(m_now.tm_mday >= 21 && m_now.tm_mday < 23)
							m_bSoilDrop = 5;
						if(m_now.tm_mday >= 23 && m_now.tm_mday < 25)
							m_bSoilDrop = 2;
						if(m_now.tm_mday >= 25 && m_now.tm_mday < 27)
							m_bSoilDrop = 6;
						if(m_now.tm_mday >= 27 && m_now.tm_mday < 28)
							m_bSoilDrop = 3;
						if(m_now.tm_mday >= 28 && m_now.tm_mday < 30)
							m_bSoilDrop = 7;
						if(m_now.tm_mday == 30 || (m_now.tm_mday >= 1 && m_now.tm_mday < 3))
							m_bSoilDrop = 4;
						
						if(m_now.tm_mday == 3 || m_now.tm_mday == 4 || m_now.tm_mday == 5)
						{
							m_bSoilDrop = 0;
						}
					}
				}
#endif
#ifdef EVENT_FLOWER
				if( m_bFlowerEvent )
				{
					if ( m_now.tm_year == (2005 - 1900) && m_now.tm_mon == 11 && m_now.tm_mday >= 5 && m_now.tm_mday <= 12 )
					{
						// 전부 드롭
						if ( m_now.tm_mday == 5 )
							m_bFlower = 1;
						else
							m_bFlower = 0;
					}
				}
#endif
#ifdef EVENT_VALENTINE_2006
				if ( m_now.tm_year == (2006 - 1900) && m_now.tm_mon == 1 && m_now.tm_mday >= 13 && m_now.tm_mday <= 20 )
				{
					 gserver.m_bEventValentine2006 = true;
			
				}
#endif
			}

			m_now = NOW();
			int i;
			i = 0;
			bool bFind;
			bFind= false;

#ifdef EVENT_OPEN_BETA_TLD
			bFind= false;
			for(i = 0; i < 5 && m_aNotice[i] != 0;i++)
			{
				if( m_aNotice[i] == EVENT_OPEN_BETA_TLD )
					bFind = true;
			}

			if( m_bOpenEvent )
			{
				if(!bFind)
					m_aNotice[--i] = EVENT_OPEN_BETA_TLD;
			}
			else
			{
				if(bFind)
					m_aNotice[--i] = 0;
			}
#endif // #ifdef EVENT_OPEN_BETA_TLD

#ifdef EVENT_XMAS_2005
			bFind= false;
			for(i = 0; i < 5 && m_aNotice[i] != 0;i++)
			{
				if( m_aNotice[i] == EVENT_XMAS_2005 )
					bFind = true;
			}

			if( m_bXMas2005Event )
			{
				if(!bFind)
					m_aNotice[i] = EVENT_XMAS_2005;
			}
			else
			{
				if(bFind)
					m_aNotice[--i] = 0;
			}
#endif // #ifdef EVENT_XMAS_2005

#ifdef EVENT_NEWYEAR_2006
			bFind= false;
			for(i = 0; i < 5 && m_aNotice[i] != 0;i++)
			{
				if( m_aNotice[i] == EVENT_NEWYEAR_2006 )
					bFind = true;
			}

			if( m_bNewYear2006Event )
			{
				if(!bFind)
					m_aNotice[i] = EVENT_NEWYEAR_2006;
			}
			else
			{
				if(bFind)
					m_aNotice[--i] = 0;
			}
	
#endif // #ifdef EVENT_NEWYEAR_2006

#ifdef EVENT_VALENTINE_2006
			bFind= false;
			for(i = 0; i < 5 && m_aNotice[i] != 0;i++)
			{
				if( m_aNotice[i] == EVENT_VALENTINE_2006 )
					bFind = true;
			}

			if( gserver.m_bEventValentine2006 )
			{
				if(!bFind)
					m_aNotice[i] = EVENT_VALENTINE_2006;
			}
			else
			{
				if(bFind)
					m_aNotice[--i] = 0;
			}
#endif // #ifdef EVENT_XMAS_2005

#endif// LC_TLD

			dNext = m_desclist;
			while (m_bLoadPartyInfo && (d = dNext))
			{
				dNext = d->m_pNext;
				
				if (STATE(d) == CON_CLOSE || STATE(d) == CON_DISCONNECT) continue;
				
#ifdef ENABLE_LOGOUT_DELAY
				if (d->m_logoutDelay != -1 && d->m_logoutDelay != 0)
				{
//					GAMELOG << init("LOGOUT DELAY");
//					if(d->m_pChar)
//					{
//						GAMELOG << d->m_pChar->GetName() << delim;
//					}
			
//					GAMELOG << d->m_host << end;
					
					CloseSocket(d, false);
					continue ;
				}
#endif // ENABLE_LOGOUT_DELAY

				if (d->m_timestamp >=0) d->m_timestamp += missed_pulses;
				d->m_idletics++;
				
				/* Process descriptors with m_input pending */
				if (FD_ISSET(d->m_desc, &m_exc_set) || d->m_idletics > PULSE_CONNECT_TIMEOUT)
				{					
					GAMELOG << init("NO INPUT")
							<< d->m_index << delim
							<< d->m_idname
							<< end;

					FD_CLR(d->m_desc, &m_input_set);
					FD_CLR(d->m_desc, &m_output_set);
					d->m_idletics = 0;
					
					d->m_bclosed = true;
					CloseSocket(d, false);
					continue;
				}
				
				if (WAITTING_DB(d))
					continue ;

				if (d->m_waithelper)
				{
					d->m_helpertimeout++;
					if (d->m_helpertimeout > 600)
					{
						GAMELOG << init("HELPER ERROR", d->m_idname)
								<< "TIMEOUT" << delim << d->m_helperWaitType << end;
						CloseSocket(d, false);
					}
					continue ;
				}

				CMutex mutex(&d->m_mutexobj, false);
				if (!mutex.IsLock())
					continue ;
				
				/* Kick out the freaky folks in the exception set and marked for close */
				if (FD_ISSET(d->m_desc, &m_input_set)) {
					//입력 쪽에서 접속이 끊어졌을 때..
#if defined (_DEBUG)
					if (STATE(d) != CON_DISCONNECT && d->ProcessInput(99999) < 0)
#elif defined(LC_USA) || defined(LC_GER)
					if (STATE(d) != CON_DISCONNECT && d->ProcessInput(40) < 0)
#else
					if (STATE(d) != CON_DISCONNECT && d->ProcessInput(20) < 0)
#endif
					{
						GAMELOG << init("FULL INPUT")
								<< d->m_index << delim
								<< d->m_idname
								<< end;

						d->m_bclosed = true;
						CloseSocket(d, false);
						continue;
					}
				}
				
				count ++;   // check current connection count 
				
				if (STATE(d) == CON_WAIT_IDEN_MSG)
				{
					d->m_connectreq++;
					if (d->m_connectreq > 600)
					{
						/* m_input wait time 30 seconds */
						GAMELOG << init("CONN_ERR", d->m_idname)
							<< "Timeout"
							<< end;
						
						//"fail 접속실패. 다시 시도해 주세요.\n",
						CNetMsg failmsg;
						FailMsg(failmsg, MSG_FAIL_CONNECT_SERVER);
						SEND_Q(failmsg, d);
						SendOutput(d);
						d->m_bclosed = true;
						CloseSocket(d, false);
					} 
					continue;
				}
				else if(STATE(d) == CON_GET_IDEN_MSG)
				{
					d->m_dbOut.Clear();
					
					if (!DisConnectLogedIn(d, true))
					{
						// 연결 검사.
						WAITTING_DB(d) = true;
						STATE(d) = CON_WAIT_USER_DB;
						m_dbthread.m_queue.AddToQ(d);
					}
					continue;
				}
				else if (STATE(d) == CON_WAIT_USER_DB)
				{
					if (!d->m_dbOut.Get(inputmsg))
					{
						CNetMsg failmsg;
						FailMsg(failmsg, MSG_FAIL_CHECK_CHAR);
						SEND_Q(failmsg, d);
						SendOutput (d);
						STATE(d) = CON_CLOSE;
					}
					else
					{
						do
						{
							SEND_Q(inputmsg, d);
							SendOutput (d);
						} while (d->m_dbOut.Get(inputmsg));
						
#ifdef GMTOOL
						if(d->m_userFlag & IS_GM_CHAR)
						{
							GMToolCharPrePlay(d);
#ifndef CRYPT_NET_MSG_MANUAL
					//	CNM_MakeKeyFromSeed(&d->m_nCryptKey, d->m_nPrepareSeed);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
						}
						else
						{
#endif // GMTOOL
						STATE(d) = CON_MENU;
						d->m_idletics = 0;

#ifndef CRYPT_NET_MSG_MANUAL
						CNM_MakeKeyFromSeed(&d->m_nCryptKey, d->m_nPrepareSeed);
#endif // #ifndef CRYPT_NET_MSG_MANUAL

#ifdef GMTOOL
						}
#endif // GMTOOL
					}
					continue;
				}
				else if (STATE(d) == CON_WAIT_NEW_CHAR_DB || STATE(d) == CON_WAIT_DEL_CHAR_DB || STATE(d) == CON_WAIT_DEL_CANCEL_CHAR_DB)
				{
					if (d->m_dbOut.Get(inputmsg))
					{
						SEND_Q(inputmsg, d);
						SendOutput (d);
						d->m_dbOut.Clear();
						
						d->m_idletics = 0;
						WAITTING_DB(d) = true;
						if (STATE(d) == CON_WAIT_NEW_CHAR_DB)
						{
							GAMELOG	<< init("CHAR_CREATE", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
									<< end;
						}
						if (STATE(d) == CON_WAIT_DEL_CHAR_DB)
						{
							GAMELOG << init("CHAR_DELETE", d->m_idname)
									<< "Char Index" << delim << d->m_pChar->m_index << delim
									<< "IP" << delim << d->m_host
									<< end;

						}
						if (STATE(d) == CON_WAIT_DEL_CANCEL_CHAR_DB)
						{
							GAMELOG << init("CHAR_DELETE_CANCEL", d->m_idname)
									<< "Char Index" << delim << d->m_pChar->m_index << delim
									<< "IP" << delim << d->m_host
									<< end;
						}
						STATE(d) = CON_WAIT_USER_DB;
						m_dbthread.m_queue.AddToQ(d);   // to CON_MENU...
					}
					else
					{
						CNetMsg failmsg;
						FailMsg(failmsg, MSG_FAIL_SYSTEM_ERROR);
						SEND_Q(failmsg, d);
						SendOutput (d);
						STATE(d) = CON_CLOSE;
					}
					continue;
				}
				else if (STATE(d) == CON_WAIT_PLAY_CHAR_DB)
				{
					if (!d->m_dbOut.Get(inputmsg))
					{
						CNetMsg failmsg;
						FailMsg(failmsg, MSG_FAIL_NOT_EXIST_CHAR);
						SEND_Q(failmsg, d);
						SendOutput (d);
						STATE(d) = CON_CLOSE;
					}
					else
					{
						do
						{
							SEND_Q(inputmsg, d);
							SendOutput(d);
						} while (d->m_dbOut.Get(inputmsg));

						if (inputmsg.m_mtype == MSG_FAIL)
						{
							STATE(d) = CON_CLOSE;
						}
						else
						{
							// inputmsg가 MSG_DB_OTHER_SERVER 이면 접속 해제
							if (inputmsg.m_mtype == MSG_DB)
							{
								unsigned char result;
								inputmsg.MoveFirst();
								inputmsg >> result;
								if (result == MSG_DB_OTHER_SERVER)
								{
									d->m_bGoZone = true;
									STATE(d) = CON_MOVESERVER;
								}
								else
								{
									// 길드 정보를 가져오기
									CGuildMember* member = m_guildlist.findmember(d->m_pChar->m_index);
									if (member == NULL)
									{
										CNetMsg rmsg;
#ifdef NEW_GUILD
										HelperGuildLoadReq(rmsg, d->m_idname, d->m_pChar->m_index, gserver.m_subno, d->m_pChar->m_pZone->m_index );
#else
										HelperGuildLoadReq(rmsg, d->m_idname, d->m_pChar->m_index);
#endif // NEW_GUILD

										if (IS_RUNNING_HELPER)
										{
											SEND_Q(rmsg, m_helper);
											d->WaitHelperReply(true, 1);
										}
									}
									else
									{
										CNetMsg rmsg;
										HelperTeachLoadReq(rmsg, d->m_idname, d->m_pChar->m_index);
										if(IS_RUNNING_HELPER)
										{
											SEND_Q(rmsg, m_helper);
											d->WaitHelperReply(true, 2);
										}
									}

									STATE(d) = CON_PREPARE_PLAY;
									d->m_idletics = 0;
								}
							}
						}
					}
					continue;
				}

				///////////////////////////////
				// 플레이 중인 사용자 처리 부분
				
				if (STATE(d) == CON_PLAYING && m_playerList.Playing(d->m_pChar))
				{
#ifdef LC_TLD
					if(!(d->m_userFlag & ADULT ))
					{
						if( m_now.tm_hour >= NOTADULTNOTICE )
						{
							if( m_now.tm_hour < NOTADULTMAX )
							{
								if( !d->m_bSendAdultNotice && ((m_now.tm_min % NOTADULTNOTICE_INTERVAL) == 0 ))
								{
									// 10시가 되면 접속이 끊길 거라고 알림
									CNetMsg rmsg;
									SysMsg(rmsg, MSG_SYS_NOT_ADULT);

									time_t t_now;
									time(&t_now);

									struct tm distime = NOW();
									distime.tm_hour = NOTADULTMAX;
									distime.tm_min = 0;
									distime.tm_sec = 0;

									time_t t_distime = mktime(&distime);

									rmsg << (int) ((t_distime - t_now) / 60);

									SEND_Q(rmsg, d);
									d->m_bSendAdultNotice = true;

								}
								else
								{
									if( d->m_bSendAdultNotice && ((m_now.tm_min % NOTADULTNOTICE_INTERVAL) != 0) )
										d->m_bSendAdultNotice = false;
								}
							}
							else
							{
								GAMELOG << init("NO ADULT")
										<< d->m_index << delim
										<< d->m_idname
										<< end;

								// 기냥 디스 커넥트 
								d->m_bclosed = true;
								CloseSocket (d, true);
								continue;
							}
						}
					}
#endif

					DecreaseTimeForPC(d->m_pChar);
					
					// 오토 세이브
					if (d->m_pChar->m_lastupdate <= 0)
					{
						d->m_pChar->m_lastupdate = PULSE_SAVE_PC;
						// 핵검사 카운트 리셋
						// 핵 카운트가 리셋되는 경우가 다른 곳에 없음으로, PULSE_SAVE_PC 동안 핵시도가 초과되지 않으면 리셋하도록 한다
						d->m_hackCount = 0;
						WAITTING_DB(d) = true;
						UPDATING_DB(d) = DB_UPDATE;
						m_dbthread.m_queue.AddToQ(d);
#ifdef LOG_INDEX
						GAMELOG << init("CHAR_UPDATE", d->m_pChar);
#else						
						GAMELOG << init("CHAR_UPDATE", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname);
#endif // LOG_INDEX
						if (d->m_pChar->m_moneyItem)
							GAMELOG << d->m_pChar->m_moneyItem->Count();
						else
							GAMELOG << 0;
						GAMELOG << end;
					}
					
					if (d->IsHackResponseTimeout())
					{
						GAMELOG << init("HACK PULSE", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
							<< "NOT RESPONSE"
							<< end;
						d->SetHackCheckPulse();
#ifndef NOKICK_HACKUSER
#ifndef _DEBUG
						d->m_bclosed = true;
#endif
#endif
					}
				}
				
				// 여기까지 플레이 처리
				///////////////////////
				
				if (!d->m_inQ.Get(inputmsg))
				{
					if (STATE(d) == CON_GET_LOGIN)
					{
						d->m_connectreq ++;
						if (d->m_connectreq > 100)
						{
							/* m_input wait time 30 seconds */
							GAMELOG << init("BAD_CONNECTION")
								<< d->m_host
								<< end;
							
							d->m_bclosed = true;
							CloseSocket (d, true);
						}
					}
					continue;
				}

#ifdef COMPRESSION_MESSAGE
	//0707
				CNetMsg nmUnpackMessage(inputmsg.GetType(),inputmsg.GetSecretKey());
				inputmsg.UnpackDefault(nmUnpackMessage);

				inputmsg = nmUnpackMessage;
#endif

#ifdef CRYPT_NET_MSG
#ifdef CRYPT_NET_MSG_MANUAL
				int nDecryptBuffer = CNM_Decrypt(inputmsg.m_buf, inputmsg.m_size, &d->m_nCryptKey, pDecryptBuffer, pTmpDecryptBuffer);
#else // CRYPT_NET_MSG_MANUAL
				int nDecryptBuffer = CNM_Decrypt(inputmsg.m_buf, inputmsg.m_size, &d->m_nCryptKeyClient, pDecryptBuffer, pTmpDecryptBuffer);
#endif // CRYPT_N
				if (nDecryptBuffer <= 0)
				{
#ifdef CRYPT_NET_MSG_MANUAL
					// 이전 키값이 현재와 다르고 임계 시간 내이면 이전 키로 복호 시도
					if (d->m_nCryptKey != d->m_nOldCryptKey && m_pulse - d->m_pulseCryptKey <= PULSE_KEY_CHANGE_THRESHOLD)
						nDecryptBuffer = CNM_Decrypt(inputmsg.m_buf, inputmsg.m_size, &d->m_nOldCryptKey, pDecryptBuffer, pTmpDecryptBuffer);

					if (nDecryptBuffer <= 0)
						inputmsg.Init(MSG_UNKNOWN);
					else
						inputmsg.WriteRaw(pDecryptBuffer, nDecryptBuffer);
#else // CRYPT_NET_MSG_MANUAL
					inputmsg.Init(MSG_UNKNOWN);
					if( d->m_pChar )
						GAMELOG << init("FAIL DECRYPT", d->m_pChar) << "state" << delim << STATE(d) << end;
					else
						GAMELOG << init("FAIL DECRYPT (NOT JOIN)", d->m_idname) << end;
					CloseSocket(d, false);
#endif // CRYPT_NET_MSG_MANUAL
				}
				else
				{
#ifdef CRYPT_NET_MSG_MANUAL
					// 현재 것으로 복호 성공하면 이후에는 이전 키로 복호 시도 안함
					CNM_CopyKey(&d->m_nOldCryptKey, &d->m_nCryptKey);
#else // CRYPT_NET_MSG_MANUAL
					CNM_NextKey(&d->m_nCryptKeyClient);
#endif // CRYPT_NET_MSG_MANUAL
					inputmsg.WriteRaw(pDecryptBuffer, nDecryptBuffer);
				}
#endif // #ifdef CRYPT_NET_MSG

				// 들어온 입력 다 처리하기
				// TODO : 최대 몇개를 처리해 줄것인가??? -> CommandInterpreter에서 카운트로 검사
//				do
//				{
					d->m_idletics = 0;
					
					if (STATE(d) == CON_PLAYING)
					{
						if (m_playerList.Playing (d->m_pChar))
						{

//0707
#ifdef SECRET_KEY
							if(inputmsg.GetType() != MSG_PULSE)
							{
								int secretkey;
								secretkey = inputmsg.GetSecretKey();
								
								if(d->m_pChar->m_secretkey != ((secretkey >> 1) - d->m_pChar->m_job))//만약에 키값이 맞지 않는다면,	
								{
									GAMELOG << init("NO MATCHING SECRET KEY", d->m_pChar)
										<< d->m_host
										<< end;

									//d->IncreaseHackCount(1);
									continue;
								}
							}
#endif

							CommandInterpreter(d, inputmsg);
						}
						else
						{
							GAMELOG << init("SYS_ERR", d->m_idname)
								<< "The state is playing, but character can't find player list!"
								<< end;
							
							STATE(d) = CON_CLOSE;
							d->m_bclosed = true;
							break;
						}
						if (m_bReboot)
							break;
					}
					else if (STATE(d) == CON_GET_LOGIN)
					{
						if (d->GetLogin(inputmsg))
						{
							d->m_idletics = 0;
							WAITTING_DB (d) = false;
							if (IS_RUNNING_CONN)
							{
								// 로그인 요청 보내기
								CNetMsg cmsg;
								ConnLoginMsg(cmsg, d);
								SEND_Q(cmsg, m_connector);
								
								STATE(d) = CON_WAIT_IDEN_MSG; //메세지 대기 모드 
								d->m_connectreq = 0;
							}
							else
							{
								//빌링 없음 겜 못해 
								//"fail 접속실패. 다시 시도해 주세요.\n",
								CNetMsg failmsg;
								FailMsg(failmsg, MSG_FAIL_CONNECT_SERVER);
								SEND_Q(failmsg, d);
								SendOutput (d);
								STATE(d) = CON_CLOSE;
							}
							break;
						}
					}
					
					else if (STATE(d) == CON_MENU && inputmsg.m_mtype == MSG_MENU)
					{
						if (d->StartGame(inputmsg))
						{
							m_dbthread.m_queue.AddToQ(d);
						}
						else if (d->CharNew(inputmsg))
						{
							m_dbthread.m_queue.AddToQ(d);
						}
						else if (d->CharDel(inputmsg))
						{
							m_dbthread.m_queue.AddToQ(d);
						}
						else
						{
						}
					}
					else if (STATE(d) == CON_PREPARE_PLAY && inputmsg.m_mtype == MSG_START_GAME)
					{
						GAMELOG << init("CHAR_LOAD", d->m_idname)
#ifdef LOG_INDEX
								<< "USER INDEX" << delim
								<< d->m_index << delim
#endif // LOG_INDEX									
								<< "NAME" << delim
								<< d->m_pChar->m_name << delim
								<< "NICK" << delim
								<< d->m_pChar->m_nick << delim
#ifdef LOG_INDEX
								<< "CHAR INDEX" << delim
								<< d->m_pChar->m_index << delim
#endif // LOG_INDEX									
								<< "JOB" << delim
								<< d->m_pChar->m_job << delim
								<< "LEVEL" << delim
								<< d->m_pChar->m_level << delim
								<< "EXP" << delim
								<< d->m_pChar->m_exp << delim
								<< "MONEY" << delim;
						if (d->m_pChar->m_moneyItem)
							GAMELOG << d->m_pChar->m_moneyItem->Count();
						else
							GAMELOG << 0;

						GAMELOG << end;

						CharPrePlay(d);
#if defined( ENABLE_WAR_CASTLE ) &&  defined( DRATAN_CASTLE ) 						
						// 성주 정보 전송
						CNetMsg msg;
						CastleOwnerInfoMsg(msg);
						SEND_Q(msg, d);
#endif // DRATAN_CASTLE						
						
						// 컨넥터에 정확한 존 번호를 설정하고 플레이 상태를 playing으로 설정한다
						if (IS_RUNNING_CONN)
						{
							CNetMsg cmsg;
							ConnPlayingMsg(cmsg, d, MSG_LOGIN_NEW);
							SEND_Q(cmsg, m_connector);
#ifdef IRIS_POINT
							ConnIPointAddMsg(cmsg, d->m_index, d->m_pChar->m_index, MSG_IPOINT_TYPE_ATTENDANCE, 0 );
							if( d->m_pChar->m_guildInfo && d->m_pChar->m_guildInfo->guild() )
								cmsg << d->m_pChar->m_guildInfo->guild()->index();
							else 
								cmsg << 0;
							cmsg << d->m_pChar->m_nick;
							SEND_Q(cmsg, m_connector);
#endif //IRIS_POINT
						}
					}
					
//				} while (d->m_inQ.Get(inputmsg));
				
#ifdef CRYPT_NET_MSG
#ifdef CRYPT_NET_MSG_MANUAL
					// 키값 변경
					if (STATE(d) == CON_PLAYING && m_playerList.Playing (d->m_pChar) && m_pulse - d->m_pulseCryptKey >= PULSE_KEY_CHANGE)
					{
						d->m_pulseCryptKey = m_pulse;
						CNM_CopyKey(&d->m_nOldCryptKey, &d->m_nCryptKey);

						CNM_KEY_TYPE nTempKey;
						CNM_CopyKey(&nTempKey, &d->m_nCryptKey);
						CNetMsg nmKeyChange;
						ExKeyChangeMsg(nmKeyChange, nTempKey);
						SEND_Q(nmKeyChange, d);

						CNM_CopyKey(&d->m_nCryptKey, &nTempKey);
					}
#endif // CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG

			} // 사용자 루프 끝
			
			
			// MSG_STATUS를 보낼지 말지 결정
			dNext = m_desclist;
			while (m_bLoadPartyInfo && (d = dNext))
			{
				dNext = d->m_pNext;
				
				if (!d->m_bclosed && STATE(d) == CON_PLAYING && m_playerList.Playing (d->m_pChar))
				{
					if (d->m_pChar->m_bChangeStatus)
					d->m_pChar->SendStatus();
					d->m_pChar->SendElementalStatus();
				}
			} // MSG_STATUS를 보낼지 말지 결정
		
		
		
			///////////////////////////////
			// 서버 종료 처리
			if (m_pulseShutdown != -1)
			{
				m_pulseShutdown--;
				if (m_pulseShutdown <= 0)
				{
					CNetMsg rmsg;
					ConnRebootReqMsg(rmsg);
					if (IS_RUNNING_CONN) SEND_Q(rmsg, m_connector);
					HelperShutdownMsg(rmsg);
					if (IS_RUNNING_HELPER) SEND_Q(rmsg, m_helper);
					m_pulseShutdown = -1;
				}
				else if (m_pulseShutdown % 10 == 0 && m_pulseShutdown <= 10 * PULSE_REAL_SEC)
				{
					// TODO : 알려주기
					
					CNetMsg rmsg;
					sprintf(g_buf, "%d", m_pulseShutdown / PULSE_REAL_SEC);
					SayMsg(rmsg, MSG_CHAT_NOTICE_SERVERDOWN, 0, "", "", g_buf);
					for (int i = 0; i < m_playerList.m_max; i++)
					{
						if (m_playerList.m_pcList[i])
						{
							SEND_Q(rmsg, m_playerList.m_pcList[i]->m_desc);
						}
					}
				}
			}

			if (m_pulseReboot != -1)
			{
				m_pulseReboot--;
				if (m_pulseReboot <= 0)
				{
					m_bReboot = true;
					m_bShutdown = false;
					
					GAMELOG << init("shutdown by command") << end;
				}
				else if (m_pulseReboot % 10 == 0 && m_pulseReboot <= 10 * PULSE_REAL_SEC)
				{
					// TODO : 알려주기
					sprintf(g_buf, "%d", m_pulseReboot / PULSE_REAL_SEC);
					CNetMsg rmsg;
					SayMsg(rmsg, MSG_CHAT_NOTICE_SERVERDOWN, 0, "", "", g_buf);
					for (int i = 0; i < m_playerList.m_max; i++)
					{
						if (m_playerList.m_pcList[i])
						{
							SEND_Q(rmsg, m_playerList.m_pcList[i]->m_desc);
						}
					}
					
				}
			}

			if (m_pulseEndGame != -1)
			{
				m_pulseEndGame--;
				if (m_pulseEndGame <= 0)
				{
					CNetMsg rmsg;
					MsgrEndGameReqMsg(rmsg);
					if (IS_RUNNING_MSGR) SEND_Q(rmsg, m_messenger);
					m_pulseEndGame = -1;
				}
			}
		
		
		
			///////////////////////////
			// 메신저 처리
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
					
					if (ret_sel < 0)
					{
						m_messenger->m_bclosed = true;
					}
					
					else if (FD_ISSET(m_messenger->m_desc, &ex_msgr))
					{
						m_messenger->m_bclosed = true;
					}
					
					else
					{
						if (FD_ISSET(m_messenger->m_desc, &in_msgr))
						{
							if (m_messenger->ProcessInput(99999) < 0)
								m_messenger->m_bclosed = true;
						} // end input fd set
						
						if (!m_messenger->m_bclosed)
						{
							int j;
							CNetMsg msgrmsg;

							for (j = 0; j <= PROC_MESSENGER_MSG_PER_PASS && m_messenger->m_inQ.Get(msgrmsg); j++)
							{
								ProcessMessengerMessage(msgrmsg);
							}

							if (FD_ISSET(m_messenger->m_desc, &out_msgr))
							{
								m_messenger->ProcessOutput();
							} // end output fd set
						}
						
					} // end if (ret_sel)
					
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
		
		
			///////////////////////////
			// 헬퍼 처리
			if (!m_brunhelper && m_helper)
			{
				if (m_helper->m_bclosed)
				{
					m_helper->CloseSocket();
					delete m_helper;
					m_helper = NULL;
				}
				else
				{
					// FD SET 설정
					fd_set in_helper, out_helper, ex_helper;
					FD_ZERO(&in_helper);
					FD_ZERO(&out_helper);
					FD_ZERO(&ex_helper);
					FD_SET(m_helper->m_desc, &in_helper);
					FD_SET(m_helper->m_desc, &out_helper);
					FD_SET(m_helper->m_desc, &ex_helper);
					
					// 폴링
					a_NullTime.tv_sec = 0;
					a_NullTime.tv_usec = 0;
					int ret_sel = select(m_helper->m_desc + 1, &in_helper, &out_helper, &ex_helper, &a_NullTime);
					a_NullTime.tv_sec = 0;
					a_NullTime.tv_usec = 0;
					
					if (ret_sel < 0)
					{
						m_helper->m_bclosed = true;
						m_bReboot = true;
						m_bShutdown = true;
						
						GAMELOG << init("shutdown by helper") << delim << "ret_sel" << end;
					}
					
					else if (FD_ISSET(m_helper->m_desc, &ex_helper))
					{
						m_helper->m_bclosed = true;
						m_bReboot = true;
						m_bShutdown = true;

						GAMELOG << init("shutdown by helper") << delim << "fd_isset ex_helper" << end;
					}
					
					else
					{
						if (FD_ISSET(m_helper->m_desc, &in_helper))
						{
							if (m_helper->ProcessInput(99999) < 0)
							{
								m_helper->m_bclosed = true;
								m_bReboot = true;
								m_bShutdown = true;

								GAMELOG << init("shutdown by helper") << delim << "ProcessInput" << end;
							}
						} // end input fd set

						if (!m_helper->m_bclosed)
						{
//							int j;
							CNetMsg helpermsg;


// 050228 : bs : Helper의 메시지는 몇개가 오든 무조건 처리하도록 수정
							while (m_helper->m_inQ.Get(helpermsg))
							{
								ProcessHelperMessage(helpermsg);
							}

							if (FD_ISSET(m_helper->m_desc, &out_helper))
							{
								m_helper->ProcessOutput();
							} // end output fd set
						}
						
					} // end if (ret_sel)
					
				} // end if (m_helper->m_bclosed)
				
			} // end if (!m_brunhelper && m_helper)
			else if (!m_brunhelper)
			{
#ifdef CIRCLE_WINDOWS
				m_brunhelper = (_beginthread(HelperConnect, 0, NULL) > 0);
#else
				pthread_t threadHelper;
				m_brunhelper = (pthread_create(&threadHelper, NULL, HelperConnect, NULL)) ? false : true;
#endif
			}
		
		//////////////////////////
		// 컨넥터 처리
#ifdef PROC_BILLING
			if (!m_brunconnector && m_connector)
			{
				if (m_connector->m_bclosed)
				{
					m_connector->CloseSocket();
					delete m_connector;
					m_connector = NULL;
				}
				else
				{
					// FD SET 설정
					fd_set in_conn, out_conn, ex_conn;
					FD_ZERO(&in_conn);
					FD_ZERO(&out_conn);
					FD_ZERO(&ex_conn);
					FD_SET(m_connector->m_desc, &in_conn);
					FD_SET(m_connector->m_desc, &out_conn);
					FD_SET(m_connector->m_desc, &ex_conn);
					
					// 폴링
					a_NullTime.tv_sec = 0;
					a_NullTime.tv_usec = 0;
					int ret_sel = select(m_connector->m_desc + 1, &in_conn, &out_conn, &ex_conn, &a_NullTime);
					a_NullTime.tv_sec = 0;
					a_NullTime.tv_usec = 0;
					
					if (ret_sel < 0)
					{
						m_connector->m_bclosed = true;
						m_bReboot = true;
						m_bShutdown = true;

						GAMELOG << init("shutdown by connector") << delim << "ret_sel" << end;
					}
					
					else if (FD_ISSET(m_connector->m_desc, &ex_conn))
					{
						m_connector->m_bclosed = true;
						m_bReboot = true;
						m_bShutdown = true;

						GAMELOG << init("shutdown by connector") << delim << "fd_isset ex_conn" << end;
					}
					
					else
					{
						if (FD_ISSET(m_connector->m_desc, &in_conn))
						{
							if (m_connector->ProcessInput(99999) < 0)
							{
								m_connector->m_bclosed = true;
								m_bReboot = true;
								m_bShutdown = true;

								GAMELOG << init("shutdown by connector") << delim << "ProcessInput" << end;
							}
						} // end input fd set
						
						if (!m_connector->m_bclosed)
						{
							int j;
							CNetMsg connmsg;

							for (j = 0; j <= PROC_CONNECTOR_MSG_PER_PASS && m_connector->m_inQ.Get(connmsg); j++)
							{
								ProcessBilling(connmsg);
							}

							if (FD_ISSET(m_connector->m_desc, &out_conn))
							{
								m_connector->ProcessOutput();
							} // end output fd set
						}
						
					} // end if (ret_sel)
					
				} // end if (m_connector->m_bclosed)
				
			} // end if (!m_brunconnector && m_connector)
		
			else if (!m_brunconnector)
			{
#ifdef CIRCLE_WINDOWS
				m_brunconnector = (_beginthread(BillingConnect, 0, this) > 0);
#else
				pthread_t threadBilling;
				m_brunconnector = (pthread_create(&threadBilling, NULL, BillingConnect, this)) ? false : true;
#endif
			}
		
#endif
			if (m_bReboot)
				break;
			

			//////////////////////
			// 큐에 있는 내용 쓰기
			dNext = m_desclist;
			while (m_bLoadPartyInfo && (d = dNext))
			{
				dNext = d->m_pNext;
				
				/* Kick out folks in the CON_CLOSE or CON_DISCONNECT state */
				if (d->m_bclosed || STATE(d) == CON_CLOSE || STATE(d) == CON_DISCONNECT)
				{
					if (d->m_outBuf.GetNextPoint())
					{
						if (FD_ISSET(d->m_desc, &m_output_set))
						{
							/* Output for this player is ready */
							if (!(d->m_bclosed))
								d->ProcessOutput();
						}
					}
					CloseSocket(d, false);
					continue;
				}
				
				if (d->m_outBuf.GetNextPoint())
				{
					if (FD_ISSET(d->m_desc, &m_output_set))
					{
						/* Output for this player is ready */
						if (STATE(d) == CON_PLAYING && d->m_pChar)
						{
							if (!d->m_pChar->m_bPlaying) continue;
						}
						if (!(d->m_bclosed) && d->ProcessOutput() < 0)
						{
							d->m_bclosed = true;
							CloseSocket(d, false);
							continue;
						}
					}
				}
				else if (d->m_bGoZone && d->m_bGoZoneSent)
				{
					// 보낼것이 없고 존이동이면 접종처리
					d->m_bclosed = true;
					CloseSocket(d, false);
					continue;
				}
			}
			

			m_pulse += missed_pulses;
#ifdef LC_TIME
			GetServerTime();
#else
			m_gameTime = GetServerTime();
#endif
			m_nDesc = count;
			HeartBeat();
			missed_pulses = 0;
//		}
//		catch (...)
//		{
//		}
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

	GAMELOG << init("SYSTEM")
			<< "Saving All User Data...."
			<< end;

	while ((d = dNext))
	{
		dNext = d->m_pNext;
		CloseSocket(d, true);
	}

	m_dbthread.m_queue.m_dbreqend = true;

	while (!m_dbthread.m_queue.m_dbend)
	{
		GAMELOG << init("SYSTEM")
				<< "Waiting for Quit DB ..."
				<< end;
#ifdef CIRCLE_WINDOWS
		Sleep (1000);
#else
		sleep (1);
#endif
	}

	GAMELOG << init("SYSTEM")
			<< "Closing all sockets."
			<< end;

	CLOSE_SOCKET(m_ssock);

	if (m_messenger)
	{
		if (m_messenger->m_desc)
			CLOSE_SOCKET(m_messenger->m_desc);
		delete m_messenger;
		m_messenger = NULL;
	}

	if (m_helper)
	{
		if (m_helper->m_desc)
			CLOSE_SOCKET(m_helper->m_desc);
		delete m_helper;
		m_helper = NULL;
	}

	if (m_connector)
	{
		delete m_connector;
		m_connector = NULL;
	}

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

void CServer::HeartBeat()
{
#ifdef USING_NPROTECT
	if (m_pulseUpdateGG > 0)
		m_pulseUpdateGG--;
	else
	{
		m_pulseUpdateGG = PULSE_NPROTECT_UPDATE;
		GGAuthUpdateTimer();
	}
#endif // USING_NPROTECT
	// 매초 한번씩 할 것들
	if (m_pulse - m_pulseProcHeartBeatPerSec >= PASSES_PER_SEC)
	{
		m_pulseProcHeartBeatPerSec = m_pulse;

		// 공성 NPC 검사
#ifdef ENABLE_WAR_CASTLE
		CheckWarCastle();

		// 세금 징수 및 분배
		ProcTax();
#endif // #ifdef ENABLE_WAR_CASTLE

		// 몹 컨트롤, 바닥 아이템 조정
		ProcMobItemControl();

		// 날씨 변화 : 30분에 한번 씩
		ChangeWeather();

#ifdef ENABLE_STATISTICS
		// 통계
		ProcStatistics();
#endif

#ifdef ENABLE_OXQUIZ
		ProcOXQuiz();
#endif // ENABLE_OXQUIZ

#ifdef RECOMMEND_SERVER_SYSTEM
		ProcRecommendServerMoonstone();
#endif // RECOMMEND_SERVER_SYSTEM

		ProcGoldenBall();

#ifdef GUILD_RANKING
		ProcGuildRanking();
#endif // GUILD_RANKING

#ifdef MONSTER_COMBO
		ProcMonsterCombo();
#endif // MONSTER_COMBO

#ifdef NEW_DOUBLE_EVENT_AUTO
		NewDoubleEventAuto();
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef EXTREME_CUBE
		m_extremeCube.CheckCubeSpace();
#endif // EXTREME_CUBE
		
	}

	// loop count 처리
	ProcLoopCount();
}

#ifdef CIRCLE_WINDOWS
void CServer::MessengerConnect(void* /* lpvoid */)
{
	gserver.m_brunmessenger = true;
	while (true)
	{
		if (gserver.MakeMessengerClient())
		{
			gserver.SendMessengerHeader();
			gserver.m_brunmessenger = false;
			return ;
		}
		else if (gserver.m_messenger)
			gserver.m_messenger->m_bclosed = true;
		Sleep(2000);
	}
}
#else
void* CServer::MessengerConnect(void* lpvoid)
{
	gserver.m_brunmessenger = true;
	while (true)
	{
		if (gserver.MakeMessengerClient())
		{
			gserver.SendMessengerHeader();
			gserver.m_brunmessenger = false;
			pthread_exit(NULL);
			return NULL;
		}
		else if (gserver.m_messenger)
			gserver.m_messenger->m_bclosed = true;
		sleep(2);
	}
}
#endif

#ifdef CIRCLE_WINDOWS
void CServer::HelperConnect(void* /* lpvoid */)
{
	gserver.m_brunhelper = true;
	while (true)
	{
		if (gserver.MakeHelperClient())
		{
			gserver.SendHelperHeader();
			gserver.m_brunhelper = false;
			return ;
		}
		else if (gserver.m_helper)
			gserver.m_helper->m_bclosed = true;
		Sleep(2000);
	}
}
#else
void* CServer::HelperConnect(void* lpvoid)
{
	gserver.m_brunhelper = true;
	while (true)
	{
		if (gserver.MakeHelperClient())
		{
			gserver.SendHelperHeader();
			gserver.m_brunhelper = false;
			pthread_exit(NULL);
			return NULL;
		}
		else if (gserver.m_helper)
			gserver.m_helper->m_bclosed = true;
		sleep(2);
	}
}
#endif

#ifdef ENABLE_WAR_CASTLE
void CServer::ProcTax()
{
	// 1분 단위로 castle에 저장을 하다가
	// 매일 오전 8시에 helper로 요청하고
	// 헬퍼의 응답이 오면 castle에서 제거, 제거후 받을 대상을 갱신
	if (m_taxSavePulse + PULSE_REAL_MIN <= m_pulse)
	{
		mysql_ping(&m_dbcastle);

		m_taxSavePulse = m_pulse;
		SaveTax();

		if (IS_RUNNING_HELPER)
		{
			struct tm tmCur = NOW();
			// 요일 검사
			if (m_taxDivGuild == -1 || m_taxDivGuild != tmCur.tm_wday)
			{
				// 월요일 0시에 세금 받을 길드 변경
				if (tmCur.tm_wday == 1 && tmCur.tm_hour >= 0)
					ChangeTaxGuild();

				// 8시 검사
				if (tmCur.tm_hour >= 8)
				{
					m_taxDivGuild = tmCur.tm_wday;
					DivideTax();
				}
			}
		}
	}
}
#endif

void CServer::ProcMobItemControl()
{
	int i, j;

	// 몹 컨트롤, 바닥 아이템 조정
	for (i=0; i < this->m_numZone; i++)
	{
		if (m_zones[i].m_bRemote)
			continue ;
		for (j=0; j< m_zones[i].m_countArea; j++)
		{
			if (m_zones[i].m_area[j].m_bEnable)
			{
				// 싱글 던전이 아닐경우만 옵 행동 루틴
				if (!(m_zones[i].IsPersonalDungeon() || m_zones[i].IsGuildRoom()))
				{
					if (m_pulse - m_zones[i].m_area[j].m_pulseNPCAI >= PULSE_NPC_AI)
					{
						m_zones[i].m_area[j].m_pulseNPCAI = m_pulse;
						m_zones[i].m_area[j].MobActivity();			// 몹 행동
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
						m_zones[i].m_area[j].Monster_synchronization( m_zones[i].m_area[j].m_nNPC_Killed_NPC );	// 몹 리스트 동기화
#endif  // NIGHTSHADOW_SKILL	
					}

					if (m_pulse - m_zones[i].m_area[j].m_pulseNPCRegen >= PULSE_NPC_REGEN)
					{
						m_zones[i].m_area[j].m_pulseNPCRegen = m_pulse;
						m_zones[i].m_area[j].MobRegen();			// 몹 리젠
					}
				}
				m_zones[i].m_area[j].GroundItemControl();	// 떨어진 아이템 정리
			}
		}
	}


	// 사라진 아이템 숫자 로그 출력 : 1분에 한번
	if (m_pulse - m_resetAdjustItem >= PULSE_REAL_MIN)
	{
		for (i=0; i < m_numZone; i++)
		{
			for (j=0; j < m_zones[i].m_countArea; j++)
			{
				if (m_zones[i].m_area[j].m_bEnable && m_zones[i].m_area[j].m_nJunkItems)
				{
					// ITEM_ADJUST Log
					GAMELOG << init("ITEM_ADJUST")
							<< m_zones[i].m_index << delim
							<< m_zones[i].m_area[j].m_index << delim
							<< m_zones[i].m_area[j].m_nJunkItems
							<< end;

					m_zones[i].m_area[j].m_nJunkItems = 0;
				}
			}
		}
		m_resetAdjustItem = m_pulse;
	}

}

void CServer::ChangeWeather()
{
	int i;

	// 날씨 변화 : 30분에 한번 씩
	if (m_pulse - m_resetWeather >= PULSE_REAL_HALF)
	{
		for (i=0; i < this->m_numZone; i++)
		{
			if (m_zones[i].m_bRemote)
				continue ;
			m_zones[i].ChangeWeather();
		}

		m_resetWeather = m_pulse;
	}
}

void CServer::ProcLoopCount()
{
	if (m_bLoop)
	{
		struct timeval tvCurrent, tvDiff;
		GetTimeofDay(&tvCurrent, NULL);
		TimeDiff(&tvDiff, &tvCurrent, &m_tvLoop);
		if (tvDiff.tv_sec >= 10)
		{
			if(m_bLoop == 1)
			{
				if (m_descLoop)
				{
					strcpy(g_buf, "Loop Count (10 sec): ");
					IntCat(g_buf, m_nLoop, false);

					CNetMsg rmsg;
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);

					SEND_Q(rmsg, m_descLoop);
				}

				m_bLoop = 0;
				m_descLoop = NULL;
			}
#ifdef GMTOOL
			else if(m_bLoop == 2)
			{
				strcpy(g_buf, "Loop Count (10 sec): ");
				IntCat(g_buf, m_nLoop, false);

				if(IS_RUNNING_MSGR)
				{
					CNetMsg rmsg;
					MsgLoopCountRep(rmsg, 1, 1, gserver.m_serverno, gserver.m_subno, g_buf);
					SEND_Q(rmsg, gserver.m_messenger);
				}
				m_bLoop = 0;
			}
#endif // GMTOOL
		}
		else
			m_nLoop++;
	}

}

#ifdef ENABLE_STATISTICS
void CServer::ProcStatistics()
{
	if (m_statisticsPulse == 0 || m_statisticsPulse + PULSE_REAL_MIN * 5 < m_pulse)
	{
		m_statisticsPulse = m_pulse;
		GAMELOG << init("STATISTICS")
				<< "ITEM_BUY" << delim
				<< m_statisticsItemBuy << delim
				<< "ITEM_SELL" << delim
				<< m_statisticsItemSell << delim
				<< "PRODUCE" << delim
				<< m_statistics152 << delim
				<< m_statistics153 << delim
				<< m_statistics154 << delim
				<< m_statistics155 << delim
				<< m_statistics156 << delim
				<< m_statistics157 << delim
				<< m_statistics158 << delim
				<< m_statistics159 << delim
				<< m_statistics160 << delim
				<< m_statistics161 << delim
				<< m_statistics162 << delim
				<< m_statistics163 << delim
				<< m_statistics197 << delim
				<< m_statistics198 << delim
				<< m_statistics199
				<< end;
	}
}
#endif // ENABLE_STATISTICS

#ifdef ENABLE_OXQUIZ
void CServer::ProcOXQuiz()
{
	// OX 퀴즈
	if (m_bEventOX)
	{
		CNetMsg rmsg;

	time_t timeCurrent;
	time(&timeCurrent);

		// OX 퀴즈 시작 알림
		if (!m_bOXQuizNoticeStart)
		{
			if (timeCurrent >= m_timeEventOXStart)
			{
				m_bOXQuizNoticeStart = true;
				int idxZone = gserver.FindZone(ZONE_OXQUIZROOM);
				if (idxZone != -1)
				{
					CZone* pZone = gserver.m_zones + idxZone;
					if (!pZone->m_bRemote)
					{
						EventOXQuizMsg(rmsg, MSG_EVENT_OXQUIZ_START);
						pZone->m_area[0].SendToAllClient(rmsg);
					}
				}

				GAMELOG << init("OXEVENT START")
						<< end;
			}
		}

		// OX 퀴즈 정답 알림
		if (m_nOXQuizRealNo != -1)
		{
			if (timeCurrent >= m_timeOXQuizLimit)
			{
				COXQuizData* qdFind = new COXQuizData(gserver.m_nOXQuizRealNo, "", false);
				void* pos = gserver.m_listOXQuiz.FindData(qdFind);
				delete qdFind;
				qdFind = gserver.m_listOXQuiz.GetData(pos);

				int idxZone = gserver.FindZone(ZONE_OXQUIZROOM);
				if (idxZone != -1)
				{
					CZone* pZone = gserver.m_zones + idxZone;
					if (!pZone->m_bRemote)
					{
						EventOXQuizAnswerMsg(rmsg, m_nOXQuizSeqNo, qdFind->GetAnswer());
						pZone->m_area[0].SendToAllClient(rmsg);

						// 상품 지급 결정
						int nItemIndex = -1;
						LONGLONG nItemCount = 0;
						switch (m_nOXQuizSeqNo)
						{
#ifdef LC_TLD
						case  7:	nItemIndex = 723;	nItemCount = 5;		break;
						case 10:	nItemIndex = 85;	nItemCount = 2;		break;
#elif defined(LC_JPN)
						// 5문제 정답시 문스톤 5개,  10문 정답시 부스터 5개,15문 정답시 사탕 20개
						case 5:		nItemIndex = 723;	nItemCount = 5;		break;
						case 10:	nItemIndex = 786;	nItemCount = 5;		break;
//						case 15:	nItemIndex = 556;	nItemCount = 20;	break;
						case 15:	nItemIndex = 85;	nItemCount = 2;		break;	// 고급제련석 2개
#elif defined(LC_KOR)
						// 2008년 4월 2일 이전 보상으로 변경
						//	1단계 - 10문제까지 맞추면 문스톤 5개
						//	2단계 - 15문제까지 맞추면 노력의 스크롤 5개
						//	3단계 - 18문제까지 맞추면 노력의 스크롤 10개
						//	최종단계 - 20문제까지 모두 맞추면 고급제련석 3개
 						case 10:	nItemIndex = 723;	nItemCount = 5;		break;
 						case 15:	nItemIndex = 973;	nItemCount = 5;		break;
 						case 18:	nItemIndex = 973;	nItemCount = 10;	break;
						case 20:	nItemIndex = 85;	nItemCount = 3;		break;
						// 2007년 12월 4일 변경
						//	1단계 - 10문제까지 맞추면 최하급 문스톤 6개
						//	2단계 - 15문제까지 맞추면 하급 문스톤 4개
						//	3단계 - 18문제까지 맞추면 문스톤 3개
						//	최종단계 - 20문제까지 모두 맞추면 상급 문스톤 2개
//						case 10:	nItemIndex = 2545;	nItemCount = 6;		break;
//						case 15:	nItemIndex = 2546;	nItemCount = 4;		break;
//						case 18:	nItemIndex = 723;	nItemCount = 3;		break;
//						case 20:	nItemIndex = 2547;	nItemCount = 2;		break;
#elif defined(LC_GER)
						case 10:	nItemIndex = 786;	nItemCount = 10;	break;
						case 15:	nItemIndex = 723;	nItemCount =  5;	break;
						case 18:	nItemIndex = 723;	nItemCount = 10;	break;
						case 20:	nItemIndex = 85;	nItemCount =  1;	break;							
#elif defined(LC_BRZ)
						case 10:	nItemIndex = 723;	nItemCount = 10;	break;  // 문스톤
						case 15:	nItemIndex = 2843;	nItemCount = 10;	break;  // 카오스볼
						case 18:	nItemIndex = 85;	nItemCount = 5;		break;  // 고제
						case 20:	nItemIndex = 85;	nItemCount = 10;	break;	// 고제						
#else
						case 10:	nItemIndex = 786;	nItemCount = 10;	break;
						case 15:	nItemIndex = 723;	nItemCount = 10;	break;
						case 18:	nItemIndex = 85;	nItemCount = 5;		break;
						case 20:	nItemIndex = 85;	nItemCount = 10;	break;
#endif // LC_TLD
						}

						// 오답자 내보내기
						int nExtra = (qdFind->GetAnswer()) ? 1 : 2;
						float left		= pZone->m_zonePos[nExtra][1] / 2.0;
						float top		= pZone->m_zonePos[nExtra][2] / 2.0;
						float right		= pZone->m_zonePos[nExtra][3] / 2.0;
						float bottom	= pZone->m_zonePos[nExtra][4] / 2.0;
						CZone* pStartZone = gserver.m_zones + gserver.FindZone(ZONE_START);
						int cx, cz;
						for (cx = 0; cx < pZone->m_area[0].m_size[0]; cx++)
						{
							for (cz = 0; cz < pZone->m_area[0].m_size[1]; cz++)
							{
								CCharacter* p;
								CCharacter* pNext = pZone->m_area[0].m_cell[cx][cz].m_listChar;
								while ((p = pNext))
								{
									pNext = pNext->m_pCellNext;
									if (IS_PC(p))
									{
										CPC* pPC = TO_PC(p);
										if (pPC->m_admin < 2)
										{
											if (!(left < GET_X(pPC) && GET_X(pPC) < right && top < GET_Z(pPC) && GET_Z(pPC) < bottom))
											{
												GAMELOG << init("EVENT OX QUIZ INCORRECT", pPC)
														<< "COUNT" << delim
														<< m_nOXQuizSeqNo << delim
														<< "QUIZ" << delim
														<< qdFind->GetQuizIndex() << delim
														<< qdFind->GetQuizString() << delim
														<< qdFind->GetAnswer()
														<< end;
												GoZone(pPC, ZONE_START,
														pStartZone->m_zonePos[0][0],													// ylayer
														GetRandom(pStartZone->m_zonePos[0][1], pStartZone->m_zonePos[0][3]) / 2.0f,		// x
														GetRandom(pStartZone->m_zonePos[0][2], pStartZone->m_zonePos[0][4]) / 2.0f);	// z
											}
											else
											{
												GAMELOG << init("EVENT OX QUIZ CORRECT", pPC)
														<< "COUNT" << delim
														<< m_nOXQuizSeqNo << delim
														<< "QUIZ" << delim
														<< qdFind->GetQuizIndex() << delim
														<< qdFind->GetAnswer()
														<< end;
												if (nItemIndex != -1)
												{
													CItem* pItem = m_itemProtoList.CreateItem(nItemIndex, -1, 0, 0, nItemCount);
													if (pItem)
													{
														if (AddToInventory(pPC, pItem, true, true))
														{
															GAMELOG << init("EVENT OX QUIZ GIFT", pPC)
																	<< "COUNT" << delim
																	<< m_nOXQuizSeqNo << delim
																	<< "ITEM" << delim
																	<< itemlog(pItem)
																	<< end;
															if (pItem->tab() < 0)
															{
																delete pItem;
																int r, c;
																if (pPC->m_invenNormal.FindItem(&r, &c, nItemIndex, 0, 0))
																{
																	pItem = pPC->m_invenNormal.GetItem(r, c);
																	ItemUpdateMsg(rmsg, pItem, nItemCount);
																	SEND_Q(rmsg, pPC->m_desc);
																}
															}
															else
															{
																ItemAddMsg(rmsg, pItem);
																SEND_Q(rmsg, pPC->m_desc);
															}
														}
														else
														{
															GAMELOG << init("EVENT OX QUIZ GIFT FAIL", pPC)
																	<< "COUNT" << delim
																	<< m_nOXQuizSeqNo << delim
																	<< "ITEM" << delim
																	<< itemlog(pItem)
																	<< end;
															delete pItem;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}

				m_nOXQuizSeqNo++;
				m_nOXQuizGMCharIndex = -1;
				m_nOXQuizRealNo = -1;
			}
		}
	}
}
#endif // ENABLE_OXQUIZ

#ifdef RECOMMEND_SERVER_SYSTEM
void CServer::ProcRecommendServerMoonstone()
{
	// 추천서버에서만
	if (!m_bRecommend)
	{
		m_pulseRecommendMoonstone = m_pulse;
		return ;
	}

	// 한시간에 한번
#ifdef BSTEST
	if (m_pulse - m_pulseRecommendMoonstone >= PULSE_REAL_MIN)
#else
	if (m_pulse - m_pulseRecommendMoonstone >= PULSE_REAL_HOUR)
#endif
	{
		CLCList<int> listTargetPC(NULL);
		int i;
		for (i = 0; i < m_playerList.m_max; i++)
		{
			if (m_playerList.m_pcList[i])
			{
				// 31레벨 이상 비피스존 유저대상
				
				if ( m_playerList.m_pcList[i]->m_level > RECOMMEND_SERVER_LEVEL 
					&& !m_playerList.m_pcList[i]->IsInPeaceZone(true) 
					&& m_playerList.m_pcList[i]->m_admin < 2							// 운영자 제외
				)
				{
					listTargetPC.AddToTail(i);
				}
			}
		}

		if (listTargetPC.GetCount() > 0)
		{
			int nSkipCount = GetRandom(0, listTargetPC.GetCount() - 1);
			void* pos = listTargetPC.GetPosition(nSkipCount);
			CItem* pItem = gserver.m_itemProtoList.CreateItem(723, -1, 0, 0, 10);
			if (pItem)
			{
				void* posTarget = pos;
				bool bGive = false;
				// pos에서 순차적으로 문스톤 10개를 지급할 수 있는 유저를 찾아 주고 알린다
				while (!bGive)
				{
					CPC* pTargetPC = m_playerList.m_pcList[listTargetPC.GetData(posTarget)];
					if (AddToInventory(pTargetPC, pItem, true, true))
					{
						CNetMsg rmsg;

						if (pItem->tab() < 0)
						{
							// 겹침
							int r = -1, c = -1;
							pTargetPC->m_invenNormal.FindItem(&r, &c, 723, 0, 0);
							CItem* pPrevItem = pTargetPC->m_invenNormal.GetItem(r, c);
							if (pPrevItem)
							{
								ItemUpdateMsg(rmsg, pPrevItem, pItem->Count());
							}
							delete pItem;
							pItem = NULL;
						}
						else
						{
							// 추가
							ItemAddMsg(rmsg, pItem);
							pItem = NULL;
						}
						SEND_Q(rmsg, pTargetPC->m_desc);

						if (IS_RUNNING_MSGR)
						{
							MsgrRecommendMoonstoneMsg(rmsg, pTargetPC->m_index, pTargetPC->GetName());
							SEND_Q(rmsg, m_messenger);
						}
						else
						{
							EventRecommendMoonstoneMsg(rmsg, pTargetPC->m_index, pTargetPC->GetName());
							m_playerList.SendToAll(rmsg);
						}

						GAMELOG << init("RECOMMEND SERVER MOONSTONE", pTargetPC)
								<< end;
						bGive = true;
					}
					else
					{
						posTarget = listTargetPC.GetNext(posTarget);
						if (posTarget == NULL)
							posTarget = listTargetPC.GetHead();
						if (posTarget == pos)
						{
							GAMELOG << init("RECOMMEND SERVER MOONSTONE NOBODY")
									<< end;
							bGive = true;
							delete pItem;
							pItem = NULL;
						}
					}
				}
			}
		}

		m_pulseRecommendMoonstone = m_pulse;
	}
}
#endif // RECOMMEND_SERVER_SYSTEM

// 골든볼 종료 처리
void CServer::ProcGoldenBall()
{
	if (m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_NOTHING)
	{
		time_t tCurrent;
		time(&tCurrent);
		if (m_clGoldenBall.GetEndTime() <= tCurrent)
		{
			CNetMsg rmsg;

			switch (m_clGoldenBall.GetStatus())
			{
			case GOLDENBALL_STATUS_VOTE:
				EventGoldenballVoteEndMsg(rmsg, m_clGoldenBall.GetTeam1Name(), m_clGoldenBall.GetTeam2Name());
				m_playerList.SendToAll(rmsg);
				m_clGoldenBall.EndVote();
				GAMELOG << init("EVENT GOLDEN BALL STATE CHANGE : VOTE => VOTE END") << end;
				break;
			case GOLDENBALL_STATUS_GIFT:
				EventGoldenballGiftEndMsg(rmsg, m_clGoldenBall.GetTeam1Name(), m_clGoldenBall.GetTeam2Name());
				m_playerList.SendToAll(rmsg);
				m_clGoldenBall.End();
				GAMELOG << init("EVENT GOLDEN BALL STATE CHANGE : GIFT => END") << end;
				if (IS_RUNNING_MSGR)
				{
					MsgrEventGoldenballEndMsg(rmsg, 0);
					SEND_Q(rmsg, m_messenger);
				}
				break;
			}
		}
	}
}

#ifdef GUILD_RANKING
// 길드 랭킹용 경험치 업데이트
void CServer::ProcGuildRanking()
{
	if (IS_RUNNING_HELPER && m_nPulseSaveExpGuild <= 0)
	{
		CNetMsg rmsg;

		m_nPulseSaveExpGuild = PULSE_GUILD_RANKING_UPDATE;

		GAMELOG << init("GUILD RANKING SAVE EXP START") << end;

		CGuild* pGuild = m_guildlist.head();
		while (pGuild)
		{
			if (pGuild->GetExpGuild() >= 1.0)
			{
				HelperSaveExpGuildMsg(rmsg, pGuild->index(), pGuild->GetExpGuild());
				SEND_Q(rmsg, m_helper);
				GAMELOG << init("GUILD RANKING SAVE EXP")
						<< "GUILD" << delim
						<< pGuild->index() << delim
						<< "EXP" << delim
						<< (float)pGuild->GetExpGuild()
						<< end;
				pGuild->ResetExpGuild();
			}
			pGuild = pGuild->nextguild();
		}

		GAMELOG << init("GUILD RANKING SAVE EXP END") << end;
	}
	else
	{
		m_nPulseSaveExpGuild--;
	}
}
#endif // GUILD_RANKING


#ifdef NEW_DOUBLE_EVENT_AUTO
void CServer::NewDoubleEventAuto()
{
	if(!m_bDoubleEventAuto)
	{
		m_bDoubleEventAutoOn = false;
		return ;
	}

	struct tm time;
	time = NOW();
#ifdef TEST_SERVER
	static bool bAuto = false;
	int min;
	min = time.tm_min;
	if(min%20 == 0)
	{
		if(bAuto == false)
		{
			if(m_bDoubleEventAutoOn)
			{
				m_bDoubleEventAutoOn = false;
				GAMELOG << init("ComboDoubleEvent Stop")
						<< end;
			}
			else
			{
				m_bDoubleEventAutoOn = true;
				GAMELOG << init("ComboDoubleEvent Start")
						<< end;
			}
			
			bAuto = true;
		}
	}
	else
	{
		bAuto = false;
	}
#else
	switch(time.tm_hour)		// 21시 ~ 24시 콤보존 더블이벤트
	{
	case 21:
	case 22:
	case 23:
		{
			if(m_bDoubleEventAutoOn == false)
			{
				GAMELOG << init("MomboDoubleEvent Start")
						<< end;
			}
			m_bDoubleEventAutoOn = true;
		}
		break;
	default:
		{
			if(m_bDoubleEventAutoOn == true)
			{
				GAMELOG << init("MomboDoubleEvent Stop")
						<< end;
			}
			m_bDoubleEventAutoOn = false;
		}
		break;
	}
#endif // TEST_SERVER
}
#endif // NEW_DOUBLE_EVENT_AUTO