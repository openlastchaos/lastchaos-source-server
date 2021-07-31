#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"
#include "DBCmd.h"
#include "ProcConnMsg.h"
#include "CmdMsg.h"

volatile LC_THREAD_T		gThreadIDGameThread;
volatile LC_THREAD_T		gThreadIDDBThread;

CServer gserver;

CServer::CServer()
#ifdef LIMIT_CATALOG
:m_limitSell(NULL)
#endif
{
	m_ssock = -1;
	m_desclist = NULL;
	FD_ZERO(&m_input_set);
	FD_ZERO(&m_output_set);
	FD_ZERO(&m_exc_set);
	FD_ZERO(&m_null_set);
	
	m_serverpath = GetServerPath();
	m_maxplayers = 0;
	m_maxSubServer = 0;
	m_bshutdown = false;
	m_breboot = false;
	m_nameserverisslow = true;
	
	m_pulse = 0;

	struct tm m_now = NOW();
	m_now.tm_min = m_now.tm_min - (m_now.tm_min % PULSE_SAVEMAXCOUNT_NEW);
	m_now.tm_min += PULSE_SAVEMAXCOUNT_NEW;
	m_now.tm_sec = 0;
	m_pulseSaveMax = mktime(&m_now);

	m_now = NOW();
	m_now.tm_min = m_now.tm_min - (m_now.tm_min % PULSE_SAVECOUNT_NEW);
	m_now.tm_min += PULSE_SAVECOUNT_NEW;
	m_now.tm_sec = 0;
	m_pulseSaveCount = mktime(&m_now);
	
	mysql_init(&m_dbuser);
	mysql_init(&m_dbauth);
	
	m_pulseDBPing = 0;

	m_serverAliveTime = PULSE_SERVER_ALIVE;
#ifdef LC_TIME
	bStartLctime = false;
	m_perSecond = PULSE_REAL_SEC;
	m_lcTime = 0;
#endif
#ifdef EXTERNAL_COUNT
	m_extTableCount = 0;
#endif

#ifdef LIMIT_CATALOG
	m_changeLimit = false;
#endif

#ifdef EVENT_GOMDORI_2007
	m_nEventGomdori2007LastUpdate = -1;
	m_nEventGomdori2007LastRead = -1;
	m_nEventGomdori2007Count = 0;
	m_nEventGomdori2007Status = NULL;
	m_nEventGomdori2007StatusCurServer = 0;
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_FLOWERTREE_2007
	m_nEventFlowerTree2007LastUpdate = -1;		
	m_nEventFlowerTree2007LastRead = -1;			
	m_nEventFlowerTree2007Count = 0;			
	m_nEventFlowerTree2007Status = NULL;			
	m_nEventFlowerTree2007StatusCurServer = 0;
#endif //EVENT_FLOWERTREE_2007
	
#ifdef EVENT_XMAS_2007
	m_bEventXmasTimerStart = false;
	m_nEventXmasTreeOldTime = 0; 
	m_nEventXmasTreeNoticeTime = 0;
	m_nEventXmasTreeLastUpdate = -1;	
	m_nEventXmasTreeLastRead = -1;	
	m_unEventXmasTreePoint = 0;
#endif //EVENT_XMAS_2007
}

void CServer::CommandInterpreter(CDescriptor* d, CNetMsg& msg)
{
	msg.MoveFirst();
	switch (msg.m_mtype)
	{
	case MSG_CONN_REQ:
		{
			unsigned char subtype;
			msg >> subtype;
			switch (subtype)
			{
#ifdef EVENT_XMAS_2007
			case MSG_CONN_EVENT_XMAS_2007 :
				ConnEventXmas2007( d, msg );
				break;
#endif //EVENT_XMAS_2007

#ifdef EVENT_SSHOT_2007
			case MSG_CONN_EVENT_SSHOT_2007 :
				ConnEvnetSshot2007( d, msg );
				break;
#endif //EVENT_SSHOT_2007

#ifdef EVENT_BJMONO_2007
			case MSG_CONN_EVENT_BJMONO_2007 :
				ConnEvnetBjMono2007( d, msg );
				break;
#endif // EVENT_BJMONO_2007

#ifdef EVENT_FLOWERTREE_2007
			case MSG_CONN_EVENT_FLOWERTREE_2007 :
				ConnEventFlowerTree2007( d, msg);
				break;
#endif //EVENT_FLOWERTREE_2007

#ifdef EVENT_GOMDORI_2007
			case MSG_CONN_EVENT_GOMDORI_2007:
				ConnEventGomdori2007(d, msg);
				break;
#endif // EVENT_GOMDORI_2007

			case MSG_CONN_GPARA_PROMOTION:
#ifdef JPN_GPARA_PROMOTION
				ConnGparaPromotion(d, msg);
#endif // JPN_GPARA_PROMOTION
				break;
			
			case MSG_CONN_OCN_GOO_PROMOTION:
#ifdef JPN_OCN_GOO_PROMOTION
				ConnOCN_GooPromotion(d, msg);
#endif // JPN_OCN_GOO_PROMOTION
				break;
				
			case MSG_CONN_MSN_PROMOTION:
#ifdef JPN_MSN_PROMOTION
				ConnMSNPromotion(d, msg);
#endif // JPN_MSN_PROMOTION
				break;
				
			case MSG_CONN_CREATEEVENT:
#ifdef CREATE_EVENT
				ConnCreateEvent(d, msg);
#endif // CREATE_EVENT
				break;

#ifdef CASH_ITEM_GIFT
			case MSG_CONN_CASHITEM_GIFT:											
				ConnCashItemGift(d, msg);
				break;
			case MSG_CONN_CASHITEM_GIFT_SENDHISTORY:  										 
				ConnCashItemSendHistory(d, msg);
				break;
			case MSG_CONN_CASHITEM_GIFT_RECVHISTORY:  
				ConnCashItemRecvHistory(d, msg);
				break;
			case MSG_CONN_CASHITEM_GIFT_RECVLIST:     
				ConnCashItemRecvList(d, msg);
				break;
			case MSG_CONN_CASHITEM_GIFT_RECV:
				ConnCashItemRecv(d, msg);
				break;
#endif
#ifdef LIMIT_CATALOG
			case MSG_CONN_LIMIT_CATALOG:
				ConnSetLimit(d, msg);
				break;
#endif
#ifdef EVENT_PACKAGE_ITEM
			case MSG_CONN_COUPON_USE:
				ConnCouponUse(d, msg);
				break;
			case MSG_CONN_COUPON_CONFIRM:
				ConnCouponConfirm(d, msg);
				break;
#endif

#ifdef NEW_SERIAL_PACKAGE_EVENT
//			case MSG_CONN_COUPON_CONFIRM_NEW:
//				ConnCouponConfirm_new(d, msg);
//				break;
			case MSG_CONN_COUPON_CONFIRM:
				ConnCouponConfirm_new(d, msg);
				break;

			case MSG_CONN_COUPON_USE_NEW:
				ConnCouponUse_new(d, msg);
				break;
#endif // NEW_SERIAL_PACKAGE_EVENT
#ifdef EVENT_PCBANG // testcode : 운영자 명령만 씀
			case MSG_CONN_PCBANG_ITEM_GIVE:
				{
					int userindex;
					int time;
					msg >> userindex
						>> time;

					if( time < 1 || time > 4 )
						break;

					CUser* user = gserver.m_userList[d->m_subno - 1].Find(userindex);
					if( user )
					{
						user->m_pcbang_time = time * PULSE_PCBANG_ITEM - 10;
					}
				}
				break;
#endif//#ifdef EVENT_PCBANG
			case MSG_CONN_RANKER_REQ:
				ConnRankerReq(d, msg);
				break;
			case MSG_CONN_EVENT_2PAN4PAN_MONEY_REQ:
				ConnEvent2pan4panMoneyReq(d, msg);
				break;

			case MSG_CONN_EVENT_2PAN4PAN_BOX_REQ:
				ConnEvent2pan4panBoxReq(d, msg);
				break;
			case MSG_CONN_EVENT_2PAN4PAN_BOX_COMMIT:
				ConnEvent2pan4panBoxCommit(d, msg);
				break;

			case MSG_CONN_EVENT_2PAN4PAN_GOODS_REQ:
				ConnEvent2pan4panGoodsReq(d, msg);
				break;
			case MSG_CONN_EVENT_2PAN4PAN_GOODS_COMMIT:
				ConnEvent2pan4panGoodsCommit(d, msg);
				break;

			case MSG_CONN_EVENT_2PAN4PAN_LETTER_REQ:
				ConnEvent2pan4panLetterReq(d, msg);
				break;
			case MSG_CONN_EVENT_2PAN4PAN_LETTER_ROLLBACK_REQ:
				ConnEvent2pan4panLetterRollbackReq(d, msg);
				break;

			case MSG_CONN_CASHITEM_PURCHASEHISTORY_REQ:
				ConnCashItemPurchaseHistoryReq(d, msg);
				break;
			case MSG_CONN_CASHITEM_PURCHASELIST_REQ:
				ConnCashItemPurchaselistReq(d, msg);
				break;
			case MSG_CONN_CASHITEM_BRING_REQ:
				ConnCashItemBringReq(d, msg);
				break;
			case MSG_CONN_CASHITEM_BALANCE_REQ:
				ConnCashItemBalanceReq(d, msg);
				break;
			case MSG_CONN_CASHITEM_PURCHASE_REQ:
				ConnCashItemPurchaseReq(d, msg);
				break;
#ifdef CASH_ITEM_COUPON
			case MSG_CONN_CASHITEM_COUPON :
				ConnCashItemCouponListReq(d,msg);
				break;
			case MSG_CONN_CASHITEM_PURCHASE_WITH_COUPON :
				ConnCashItemPurchaseByCoupon(d,msg);
				break;
#endif //CASH_ITEM_COUPON
			case MSG_CONN_STASH_SET_NEW_PASSWORD:
				ConnStashNewPassword(d, msg);
				break;
			case MSG_CONN_STASH_DELETE_PASSWORD:
				ConnStashDeletePassword(d, msg);
				break;

			case MSG_CONN_STASH_SEAL:
				ConnStashSeal(d, msg);
				break;

			case MSG_CONN_STASH_CHANGE_PASSWORD:
				ConnStashChangePassword(d, msg);
				break;

			case MSG_CONN_STASH_CHECK_PASSWORD:
				ConnStashCheckPassword(d, msg);
				break;

			case MSG_CONN_STASH_STATE:
				ConnStashState(d, msg);
				break;

			case MSG_CONN_LEVELUP_POINT:
				ConnLevelupPoint(d, msg);
				break;

			case MSG_CONN_PREOPEN_GIFT:
				ConnPreopenGift(d, msg);
				break;

			case MSG_CONN_LOGIN_REQ:
				ConnLogin(d, msg);
				break;
				
			case MSG_CONN_LOGOUT_REQ:
				ConnLogout(d, msg);
				break;
				
			case MSG_CONN_BLOGIN_REQ:
				ConnBLogin(d, msg);
				break;
				
			case MSG_CONN_BLOGOUT_REQ:
				ConnBLogout(d, msg);
				break;
				
			case MSG_CONN_PLAYER_REQ:
				ConnPlayerReq(d, msg);
				break;
				
			case MSG_CONN_PLAYING:
				ConnPlaying(d, msg);
				break;
				
			case MSG_CONN_REBOOT_REQ:
				m_bshutdown = true;
				m_breboot = true;
				break;
#ifdef EVENT_PARTNER_CODENEO
			case MSG_CONN_EVENT_PARTNER_CODENEO_REQ:
				ConnEventPartnerCodeNEOReq( d, msg );
				break;
#endif

#ifdef EVENT_OPEN_BETA_GIFT
			case MSG_CONN_OPEN_BETA_GIFT:
				ConnOpenBetaGift(d, msg);
				break;
#endif 
				
#ifdef GMTOOL
			case MSG_CONN_GMTOOL_GOMMAND:
				ConnGMToolCommand( d, msg );
				break;
			case MSG_CONN_GMTOOL_CHATTING:
				ConnGMToolChatting( d, msg );
				break;
			case MSG_CONN_GMTOOL_CHATMONITOR:
				ConnGMToolChatMonitor( d, msg );
				break;
#endif // GMTOOL

#ifdef NEW_GUILD_POINT_RANKING_NOTICE
			case MSG_CONN_GUILD_POINT_RANKER_REQ:
				ConnGuildPointRankingReq( d, msg );
				break;
#endif // NEW_GUILD_POINT_RANKING_NOTICE

#ifdef GIFT_EVENT_2007
			case MSG_CONN_EVENT_GIFT_2007:
				ConnEventGift2007(d, msg);
				break;
#endif // GIFT_EVENT_2007

#ifdef CLIENT_RESTART
			case MSG_CONN_CLIENT_RESTART :
				ConnClientRestart(d, msg);
				break;
#endif //CLIENT_RESTART
			
#ifdef ATTENDANCE_EVENT_REWARD
			case MSG_CONN_ATTENDANCE_COUNT :
				ConnAttendanceEventReward(d,msg);
				break;
#endif //ATTENDANCE_EVENT_REWARD

#ifdef IRIS_POINT
			case MSG_CONN_ADD_IPOINT:
				ConnIpointAddMsg(d,msg);
				break;
#endif //IRIS_POINT

#ifdef EVENT_NOM_MOVIE
			case MSG_CONN_EVENT_NOM:
				ConnEventNomMsg(d, msg);
				break;
#endif // EVENT_NOM_MOVIE
			
#if defined (CHAR_LOG) && defined (LC_KOR)
			case MSG_CONN_CHAR_LOG:
				ConnCharLogMsg(d, msg);
					break;
#endif // #if defined (CHAR_LOG) && defined (LC_KOR)
		
#ifdef REQUITAL_EVENT	// 보상 이벤트 
			case MSG_CONN_EVENT_REQUITAL:
				ConnEventRequitalMsg(d, msg);
					break;
#endif // REQUITAL_EVENT
			
			default:
				goto ERROR_INVALID_COMMAND;
			}
		}
		break;
		
	default:
		goto ERROR_INVALID_COMMAND;
	}
	
	return ;
	
ERROR_INVALID_COMMAND:
	GAMELOG << init("SYS_ERR") << "Invalid Command: " << msg.m_mtype << "(" << msg.m_buf[1] << ")" << end;
}

void CServer::CheckPlayers()
{
	int i, sub;
	
	// 모든 유저를 돌면서
	for (sub = 0; sub < m_maxSubServer; sub++)
	{
		for (i = 0; i <= m_userList[sub].m_last; i++)
		{
			CUser* user = m_userList[sub].m_users[i];
			if (user == NULL)
				continue ;
			// 인덱스가 정상인 넘을 대상으로
			if (user->m_index >= 0)
			{
				// 접속 시간 증가 시키고
				user->m_timestamp++;
#ifdef EVENT_PCBANG
				if( user->m_location == BILL_LOCATION_PCBANG )
				{
					user->m_pcbang_time++;

					if( user->m_pcbang_time % PULSE_PCBANG_ITEM == 0)
					{
#ifdef LC_TLD
						// 해당 서브 서버를 찾는다.
						CDescriptor* p;
						CDescriptor* pNext = gserver.m_desclist;
						bool bFind = false;
						while ((p = pNext))
						{
							pNext = p->m_pNext;
							if( (p->m_subno - 1) == sub )
							{
								bFind = true;
								break;
							}
						}
						// 해당 서브 서버로 아이템 지급 메세지를 보내고 pcbangtime을 초기화 
						CNetMsg rmsg;
						
						if( bFind && p )
						{
							PcbangItemGiveMsg(rmsg, user->m_name, (user->m_pcbang_time / PULSE_PCBANG_ITEM) - 1);
							SEND_Q(rmsg, p);
						}
#else // LC_TLD
						// 해당 서브 서버로 아이템 지급 메세지를 보내고 pcbangtime을 초기화 
					
						CNetMsg rmsg;
						PcbangItemGiveMsg(rmsg, user->m_name, (user->m_pcbang_time / PULSE_PCBANG_ITEM) - 1);
						SEND_Q(rmsg, user->m_descserver);
#endif // LC_TLD

						if( user->m_pcbang_time >= PULSE_PCBANG_ITEM * 4 )
							user->m_pcbang_time = PULSE_PCBANG_ITEM * 3 + 1;
					}
				}
#endif

				
				// 플레이 상태가 아닌 넘은 타임아웃 조사해서 종료시키기
				if (user->m_state != CONN_PLAYING)
				{
					user->m_checkTics++;
					
					// 타임 아웃 검사
					if (user->m_checkTics > PULSE_CONNECT_TIMEOUT)
					{
						// 로그아웃 처리
						GAMELOG << init("DEL_PLIST", user->m_name) << end;
						ProcessLogout(user);
					}
				}
			}
		}
	}
}

void CServer::ProcessLogout(CUser* user)
{
	if (user == NULL)
		return ;
	
	int subno = user->m_subnum;
	
	GAMELOG << init("LOGOUT", user->m_name)
		<< end;
	
	// 존별 사용자 수 감소
	m_userList[subno - 1].m_playersPerZone[user->m_zone]--;
	
	// 빌링에 로그아웃 알리기
	if (m_billing.IsRunning())
		m_billing.Logout(user->m_name);
	
	// DB에 저장
#ifdef EVENT_PCBANG_TIME_SAVE
	if (WriteDB(user->m_index, user->m_server, subno, -1, user->m_timestamp, NULL, NULL, false, user->m_pcbang_time) != MSG_CONN_ERROR_SUCCESS)
#else
	if (WriteDB(user->m_index, user->m_server, subno, -1, user->m_timestamp, NULL, NULL, false) != MSG_CONN_ERROR_SUCCESS)
#endif
		GAMELOG << init("SYS_ERR", user->m_name) << "Cannot Update DB for Logout" << end;
	
#ifdef EVENT_PCBANG_2ND
	if(user->m_location == BILL_LOCATION_PCBANG)
	{
		// 피씨방 로그 아웃 기록 남기기
		CDBCmd cmd;
		cmd.Init(&gserver.m_dbauth);

		sprintf(g_buf, "insert into t_pcbang_log values(%d, '%s', %d, now())",  user->m_index, (const char *)user->m_ip, PCBANG_LOGOUT);
		cmd.SetQuery(g_buf);
		cmd.Update();
	}
#endif // EVENT_PCBANG_2ND

	// 유저를 리스트에서 삭제
	m_userList[subno - 1].Remove(user);
}

void ServerSrandom(unsigned long initial_seed);

CServer::~CServer()
{
	if (m_serverpath)
		delete[] m_serverpath;

#ifdef EVENT_GOMDORI_2007
	if (m_nEventGomdori2007Status)
		delete [] m_nEventGomdori2007Status;
#endif // EVENT_GOMDORI_2007
}

char* CServer::GetServerPath()
{
	char szBuffer[1024];
#ifdef CIRCLE_WINDOWS
	GetModuleFileName(::GetModuleHandle(NULL), szBuffer, 1000);
	int path_len = strlen(szBuffer);
	int i;
	
	for (i = path_len - 1; i >= 0; i-- ) {
		if (szBuffer[i] == '\\')  {
			szBuffer[i+1] = '\0';
			break;
		}
	}
	if (i < 0)
		return false;
#else
	getcwd (szBuffer, 512);
	strcat (szBuffer, "/");
#endif
	
	char* ret = new char[strlen(szBuffer) + 1];
	strcpy(ret, szBuffer);
	return ret;
}

bool CServer::LoadSettingFile()
{
	puts("Loading Setting File...");
	
	CLCString strTemp(1024);
	
	strTemp.Format("%s%s", m_serverpath, "newStobm.bin");
	
	if (!m_config.Load(strTemp))
		return false;
	
	m_serverno = atoi(m_config.Find("Server", "Number"));
	m_maxSubServer = atoi(m_config.Find("Server", "MaxSubServer"));
	if (m_maxSubServer < 1)
		return false;
	
	return true;
}

bool CServer::ConnectDB()
{
	if (!mysql_real_connect(
		&m_dbuser,
		m_config.Find("User DB", "IP"),
		m_config.Find("User DB", "User"),
		m_config.Find("User DB", "Password"),
		m_config.Find("User DB", "DBName"),
		0, NULL, 0))
		return false;
	
	if (!mysql_real_connect(
		&m_dbauth,
		m_config.Find("Auth DB", "IP"),
		m_config.Find("Auth DB", "User"),
		m_config.Find("Auth DB", "Password"),
		m_config.Find("Auth DB", "DBName"),
		0, NULL, 0))
		return false;

#if defined (EVENT_2PAN4PAN_GOODS) || defined (ATTENDANCE_EVENT)  || defined ( ATTENDANCE_EVENT_REWARD ) || defined ( IRIS_POINT ) || defined (EVENT_NOM_MOVIE)
	if (!mysql_real_connect(
		&m_dbevent,
		m_config.Find("Event DB", "IP"),
		m_config.Find("Event DB", "User"),
		m_config.Find("Event DB", "Password"),
		m_config.Find("Event DB", "DBName"),
		0, NULL, 0))
		return false;
#endif // #if defined (EVENT_2PAN4PAN_GOODS) || defined (ATTENDANCE_EVENT) || defined ( ATTENDANCE_EVENT_REWARD ) || defined ( IRIS_POINT )
	
#if defined ( CHAR_LOG ) && defined ( LC_KOR )
	if (!mysql_real_connect(
		&m_dblog,
		m_config.Find("Log DB", "IP"),
		m_config.Find("Log DB", "User"),
		m_config.Find("Log DB", "Password"),
		m_config.Find("Log DB", "DBName"),
		0, NULL, 0))
		return false;
#endif //#if defined ( CHAR_LOG ) && defined ( LC_KOR )

	CDBCmd cmdUser;
	cmdUser.Init(&m_dbuser);

#ifdef JPN_GPARA_PROMOTION
	cmdUser.SetQuery("SELECT a_index, a_user_index, a_server, a_char_index FROM t_gpara_promotion_2007 LIMIT 0");
	if (!cmdUser.Open())
	{
		GAMELOG << init("ERROR : GPARA TABLE NOT FOUND: t_gpara_promotion_2007") << end;
		return false;
	}
#endif // JPN_GPARA_PROMOTION
	
#ifdef JPN_OCN_GOO_PROMOTION
	cmdUser.SetQuery("SELECT a_index, a_user_index, a_server, a_char_index FROM t_ocn_goo_promotion_2008 LIMIT 0");
	if (!cmdUser.Open())
	{
		GAMELOG << init("ERROR : OCN_GOO TABLE NOT FOUND: t_ocn_goo_promotion_2008") << end;
		return false;
	}
#endif // JPN_OCN_GOO_PROMOTION

#ifdef JPN_MSN_PROMOTION
	cmdUser.SetQuery("SELECT a_index, a_user_index, a_server, a_char_index FROM t_msn_promotion_2007 LIMIT 0");
	if (!cmdUser.Open())
	{
		GAMELOG << init("ERROR : MSN TABLE NOT FOUND: t_gpara_promotion_2007") << end;
		return false;
	}
#endif // JPN_GPARA_PROMOTION

#ifdef NEW_SERIAL_PACKAGE_EVENT
	cmdUser.SetQuery("SELECT * FROM t_new_promotion LIMIT 0");
	if( !cmdUser.Open())
	{
		GAMELOG << init("ERROR : NEW PROMOTION TABLE NOT FOUND: t_new_promotion") << end;
		return false;
	}

	cmdUser.SetQuery("SELECT * FROM t_new_promoitem LIMIT 0");
	if( !cmdUser.Open() )
	{
		GAMELOG << init("ERROR : NEW PROMOTION ITEM TABLE NOT FOUND:t_new_promoItem") << end;
		return false;
	}
#endif // NEW_SERIAL_PACKAGE_EVENT

#ifdef EVENT_GOMDORI_2007
	sprintf(g_buf, "SELECT * FROM t_event_gomdori_count WHERE a_server=%d", gserver.m_serverno);
	cmdUser.SetQuery(g_buf);
	if (cmdUser.Open())
	{
		struct tm tmCur = NOW();
		gserver.m_nEventGomdori2007LastUpdate = tmCur.tm_min - tmCur.tm_min % 5;
		if (cmdUser.MoveFirst())
			cmdUser.GetRec("a_count", gserver.m_nEventGomdori2007StatusCurServer);
		else
		{
			sprintf(g_buf, "INSERT INTO t_event_gomdori_count (a_server, a_updatedate) VALUES (%d, NOW())", gserver.m_serverno);
			cmdUser.SetQuery(g_buf);
			cmdUser.Update();
		}
	}
	else
	{
		GAMELOG << init("ERROR : EVENT GOMDORI 2007 TABLE NOT FOUND: t_event_gomdori_count") << end;
		return false;
	}
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_FLOWERTREE_2007
	sprintf(g_buf, "SELECT * FROM t_event_flowertree_point WHERE a_server=%d", gserver.m_serverno);
	cmdUser.SetQuery(g_buf);
	if ( cmdUser.Open() )
	{
		struct tm tmCur = NOW();
		gserver.m_nEventFlowerTree2007LastUpdate = tmCur.tm_min - tmCur.tm_min % 5;
		if (cmdUser.MoveFirst())
			cmdUser.GetRec("a_point", gserver.m_nEventFlowerTree2007StatusCurServer);
		else
		{
			sprintf(g_buf, "INSERT INTO t_event_flowertree_point (a_server, a_lastupdate) VALUES (%d, NOW())", gserver.m_serverno);
			cmdUser.SetQuery(g_buf);
			cmdUser.Update();
		}
	}
	else
	{
		GAMELOG << init("ERROR : EVENT FLOWERTREE 2007 TABLE NOT FOUND: t_event_flowertree_point") << end;
		return false;
	}
#endif // EVENT_FLOWERTREE_2007

#ifdef GIFT_EVENT_2007
#ifdef ADULT_SERVER
	sprintf(g_buf, "SELECT * FROM t_event_gift_2007_adult LIMIT 0");
#else
	sprintf(g_buf, "SELECT * FROM t_event_gift_2007 LIMIT 0");
#endif // ADULT_SERVER

	cmdUser.SetQuery(g_buf);
	if (cmdUser.Open() == false)
	{
#ifdef ADULT_SERVER
		GAMELOG << init("ERROR : EVENT GIFT 2007 TABLE NOT FOUND: t_event_gift_2007_adult") << end;
#else
		GAMELOG << init("ERROR : EVENT GIFT 2007 TABLE NOT FOUND: t_event_gift_2007") << end;
#endif // ADULT_SERVER
		return false;
	}
#endif // GIFT_EVENT_2007

#ifdef EVENT_XMAS_2007
	sprintf(g_buf, "SELECT * FROM t_event_xmastree_point WHERE a_server=%d", gserver.m_serverno);
	cmdUser.SetQuery(g_buf);
	if ( cmdUser.Open() )
	{
		struct tm tmCur = NOW();
		gserver.m_nEventXmasTreeLastUpdate = tmCur.tm_min - tmCur.tm_min % 5;
		if (cmdUser.MoveFirst())
			cmdUser.GetRec("a_point", gserver.m_unEventXmasTreePoint );
		else
		{
			sprintf(g_buf, "INSERT INTO t_event_xmastree_point (a_server, a_lastupdate) VALUES (%d, NOW())", gserver.m_serverno);
			cmdUser.SetQuery(g_buf);
			cmdUser.Update();
		}
	}
	else
	{
		GAMELOG << init("ERROR : EVENT XMAS 2007 TABLE NOT FOUND: t_event_xmastree_point") << end;
		return false;
	}
#endif // EVENT_XMAS_2007

	return true;
}

bool CServer::InitGame()
{
	m_userList = new CUserList[m_maxSubServer];
	
	ServerSrandom(time(0));
	MakeMath();
	InitUserData();
	
	GAMELOG << init("SYSTEM") << "Finding Connection Limit" << end;
	m_maxplayers = GetMaxPlayers();
	
	GAMELOG << init("SYSTEM") << "Initialize Server Socket" << end;
	m_ssock = InitSocket();
	
	CDBCmd cmd;
	int i;


	i = 0;
	cmd.Init(&gserver.m_dbuser);
#ifdef FEEITEM
	GAMELOG << init("SYSTEM") << "Initialize t_purchase" << end;

	cmd.Init(&gserver.m_dbuser);

	for(i = 0; i < 10; i++)
	{
		sprintf(g_buf, "select * from t_purchase0%d where a_confirm = 0 ", i);
		cmd.SetQuery(g_buf);
		if ( cmd.Open() && cmd.MoveFirst() )
		{
			int index;
			int user_idx;
			char server;
			int ctid;
			int serial;
			do
			{
				cmd.GetRec("a_index", index);
				cmd.GetRec("a_user_idx", user_idx);
				cmd.GetRec("a_server", server);
				cmd.GetRec("a_ctid", ctid);
				cmd.GetRec("a_serial", serial);
				
				GAMELOG << init("DELETE NOT CONFIRM ITEM")
						<< index << delim
						<< user_idx << delim
						<< server << delim
						<< ctid << delim
						<< serial << delim
						<< end;
			} while( cmd.MoveNext() );
		}

		sprintf(g_buf, "delete from t_purchase0%d where a_confirm = 0 ", i);
		cmd.SetQuery(g_buf);

		if( !cmd.Update() )
		{
			GAMELOG << init("delete ERROR")
					<< end;
		}
	}
#endif
#ifdef EXTERNAL_COUNT
	CLCList<int> tableIndexList(NULL);

	for( i = 0;i < EXTERNAL_COUNT_MAX; i++)
	{
		if( EXTERNAL_COUNT & ( 1 << i ) )
		{
			m_extTableCount++;
			tableIndexList.AddToTail(i);
		}
	}

	sprintf(g_buf, "select a_partner_id from t_count_external_id where a_index in ( ");
	CLCString temp(1024);
	void* pos;
	
	pos = tableIndexList.GetHead();

	for(i = 0; i < tableIndexList.GetCount() - 1; i++)
	{
		temp.Format("%d, ", tableIndexList.GetData(pos) );
		StrCat(g_buf, temp);
		pos = tableIndexList.GetNext(pos);
	}

	temp.Format("%d ) order by a_index ", tableIndexList.GetData(pos) );
	StrCat(g_buf, temp);

	cmd.Init(&gserver.m_dbuser);
	cmd.SetQuery(g_buf);
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		GAMELOG << init("SYSTEM_ERROR") << "CANNOT FIND DEFINED PARTNER_ID" << end;
		return false;
	}

	CLCString partner(2+1);
	do
	{
		cmd.GetRec("a_partner_id", partner);
		strcpy(m_extTableList[i], (const char*) partner);
	}while (cmd.MoveNext() );

	for(i = 0; i < m_extTableCount; i++)
	{
		StrLwr(this->m_extTableList[i]);
		sprintf(g_buf, "select a_index from t_connect_%s ", this->m_extTableList[i]);
		StrUpr(this->m_extTableList[i]);

		cmd.SetQuery(g_buf);
		if( !cmd.Open() )
		{
			GAMELOG << init("SYSTEM_ERROR") << "Cannot find t_connect_nm" << end;
			return false;
		}
	}
	
#endif
#ifdef LC_TIME
	// 라카력 로딩 
	int nStartTime = 0;
#ifdef TEST_SERVER
	cmd.SetQuery("select * from t_lc_time where a_server = 1000 ");
#else
	cmd.SetQuery("select * from t_lc_time where a_server = 0 ");
#endif
	if( !cmd.Open() )
	{
		GAMELOG << init("SYSTEM_ERROR") << "Cannot find t_lc_time" << end;
		return false;
	}

	if( cmd.MoveFirst() )
	{
		if( !cmd.GetRec("a_time", nStartTime) )
			return false;
		this->m_startTime = nStartTime;
	}
	sprintf(g_buf, "select * from t_lc_time where a_server = %d", this->m_serverno);
	cmd.SetQuery(g_buf);
	if( !cmd.Open() )
	{
		GAMELOG << init("SYSTEM_ERROR") << "Cannot find t_lc_time" << end;
		return false;
	}

	if( cmd.MoveFirst() )
	{
		int nTime;
		if( !cmd.GetRec("a_time", nTime) )
			return false;

		this->m_lcTime = nTime + this->m_startTime;
	}
#endif

	// 팔린 상품 갯수 로딩
#ifdef LIMIT_CATALOG
	sprintf(g_buf, "select * from t_ct_limit where a_server = %d ", this->m_serverno);
	cmd.SetQuery(g_buf);
	if( !cmd.Open() )
	{
		GAMELOG << init("SYSTEM_ERROR") << "Cannot find t_ct_limit" << end;
		return false;
	}

	CLimitSell* sellList;
	int ctid, sell;
	while( cmd.MoveNext() )
	{
		if( !cmd.GetRec("a_ct_id", ctid)
			|| !cmd.GetRec("a_sell", sell) )
		{
			GAMELOG << init("SYSTEM_ERROR") << "t_ct_limit RECORD ERROR CANNOT GET" << end;
			return false;
		}

		sellList = new CLimitSell(ctid, sell, false);

		gserver.m_limitSell.AddToTail(sellList);
	}
#endif
	GAMELOG << init("SYSTEM") << "Entering Connector Loop" << end;
	
	
	return true;
}

void CServer::InitUserData()
{
	// t_user의 연결 초기화
	// UPDATE t_users SET a_zone_num=-1 WHERE a_server_num=serverno AND a_subnum=subnum
	strcpy(g_buf, "UPDATE t_users SET a_zone_num=-1 WHERE a_server_num=");
	IntCat(g_buf, m_serverno, false);
	CDBCmd cmd;
	cmd.Init(&m_dbuser);
	cmd.SetQuery(g_buf);
	cmd.Update();
	
}

int CServer::GetMaxPlayers()
{
#ifndef CIRCLE_UNIX
	return MAX_PLAYING;
#else
	int max_descs = 0;
	const char *method;
	/*
	* First, we'll try using getrlimit/setrlimit.  This will probably work
	* on most systems.  HAS_RLIMIT is defined in sysdep.h.
	*/
#ifdef HAS_RLIMIT
	{
		struct rlimit limit;
		
		/* find the limit of file descs */
		method = "rlimit";
		if (getrlimit(RLIMIT_NOFILE, &limit) < 0)
		{
			GAMELOG << init("SYS_ERR") << "Calling getrlimit()" << end;
			exit(1);
		}
		
		/* set the current to the maximum */
		limit.rlim_cur = limit.rlim_max;
		if (setrlimit(RLIMIT_NOFILE, &limit) < 0)
		{
			GAMELOG << init("SYS_ERR") << "Calling setrlimit()" << end;
			exit(1);
		}
#ifdef RLIM_INFINITY
		if (limit.rlim_max == RLIM_INFINITY)
			max_descs = MAX_PLAYING + NUM_RESERVED_DESCS;
		else
			max_descs = MIN(MAX_PLAYING + NUM_RESERVED_DESCS, limit.rlim_max);
#else
		max_descs = MIN(MAX_PLAYING + NUM_RESERVED_DESCS, limit.rlim_max);
#endif
	}
#elif defined (OPEN_MAX) || defined(FOPEN_MAX)
#if !defined(OPEN_MAX)
#define OPEN_MAX FOPEN_MAX
#endif
	method = "OPEN_MAX";
	max_descs = OPEN_MAX;		/* Uh oh.. rlimit didn't work, but we have
	* OPEN_MAX */
#elif defined (_SC_OPEN_MAX)
	/*
	* Okay, you don't have getrlimit() and you don't have OPEN_MAX.  Time to
	* try the POSIX sysconf() function.  (See Stevens' _Advanced Programming
	* in the UNIX Environment_).
	*/
	method = "POSIX sysconf";
	errno = 0;
	if ((max_descs = sysconf(_SC_OPEN_MAX)) < 0) {
		if (errno == 0)
			max_descs = m_MaxPlaying + NUM_RESERVED_DESCS;
		else
		{
			GAMELOG << init("SYS_ERR") << "Calling sysconf()" << end;
			exit(1);
		}
	}
#else
	/* if everything has failed, we'll just take a guess */
	method = "random guess";
	max_descs = m_MaxPlaying + NUM_RESERVED_DESCS;
#endif
	
	/* now calculate max _players_ based on max descs */
	max_descs = MIN(MAX_PLAYING, max_descs - NUM_RESERVED_DESCS);
	
	if (max_descs <= 0)
	{
		GAMELOG << init("SYS_ERR") << "Non-positive max player limit!  (Set at" << max_descs << "using" << method << ")." << end;
		exit(1);
	}
	
	GAMELOG << init("SYSTEM") << "Setting player limit to" << max_descs << "using" << method << "." << end;
	
	return (max_descs);
#endif /* CIRCLE_UNIX */
}

socket_t CServer::InitSocket()
{
	socket_t s;
	struct sockaddr_in sa;
	int opt;
	
#ifdef CIRCLE_WINDOWS
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		
		wVersionRequested = MAKEWORD(1, 1);
		
		if (WSAStartup(wVersionRequested, &wsaData) != 0)
		{
			puts("Error: WinSock not available!");
			exit(1);
		}
		if ((wsaData.iMaxSockets - 4) < m_maxplayers)
		{
			m_maxplayers = wsaData.iMaxSockets - 4;
		}
		GAMELOG << init("SYSTEM") << "Max players set to " << m_maxplayers << end;
		
		if ((s = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		{
			puts("Error: Create server socket");
			exit(1);
		}
	}
#else
	/*
	* Should the first argument to socket() be AF_INET or PF_INET?  I don't
	* know, take your pick.  PF_INET seems to be more widely adopted, and
	* Comer (_Internetworking with TCP/IP_) even makes a point to say that
	* people erroneously use AF_INET with socket() when they should be using
	* PF_INET.  However, the man pages of some systems indicate that AF_INET
	* is correct; some such as ConvexOS even say that you can use either one.
	* All implementations I've seen define AF_INET and PF_INET to be the same
	* number anyway, so the point is (hopefully) moot.
	*/
	
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		GAMELOG << init("SYS_ERR") << "Create server socket" << end;
		exit(1);
	}
#endif				/* CIRCLE_WINDOWS */
	
#if defined(SO_REUSEADDR) && !defined(CIRCLE_MACINTOSH)
	opt = 1;
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) < 0)
	{
		puts("Error: setsockopt(): REUSEADDR");
		exit(1);
	}
#endif
	
	SetSendbuf(s);
	
	/*
	* The GUSI sockets library is derived from BSD, so it defines
	* SO_LINGER, even though setsockopt() is unimplimented.
	*	(from Dean Takemori <dean@UHHEPH.PHYS.HAWAII.EDU>)
	*/
#if defined(SO_LINGER) && !defined(CIRCLE_MACINTOSH)
	{
		struct linger ld;
		
		ld.l_onoff = 0;
		ld.l_linger = 0;
		if (setsockopt(s, SOL_SOCKET, SO_LINGER, (char *) &ld, sizeof(ld)) < 0)
			GAMELOG << init("SYS_ERR") << "setsockopt(): SO_LINGER" << end;	/* Not fatal I suppose. */
	}
#endif
	
	/* Clear the structure */
	memset((char *)&sa, 0, sizeof(sa));
	
	sa.sin_family = AF_INET;
	sa.sin_port = htons((unsigned short)atoi(m_config.Find("Server", "Port")));
	sa.sin_addr = *(GetBindAddr());
	
	if (bind(s, (struct sockaddr *) &sa, sizeof(sa)) < 0)
	{
		puts("Error: bind");
		CLOSE_SOCKET(s);
		exit(1);
	}
	Nonblock(s);
	listen(s, 57);
	
	return (s);
}

int CServer::SetSendbuf(socket_t s)
{
#if defined(SO_SNDBUF) && !defined(CIRCLE_MACINTOSH)
	int opt = MAX_SOCK_BUF;
	
	if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *) &opt, sizeof(opt)) < 0)
	{
		puts("Error: setsockopt(): SNDBUF");
		return (-1);
	}
	
# if 0
	if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *) &opt, sizeof(opt)) < 0)
	{
		puts("Error: setsockopt(): RCVBUF");
		return (-1);
	}
# endif
	
#endif
	
	return (0);
}

struct in_addr* CServer::GetBindAddr()
{
	static struct in_addr bind_addr;
	
	memset(&bind_addr, 0, sizeof(bind_addr));
	
	if (strcmp(m_config.Find("Server", "IP"), "ALL") == 0)
		bind_addr.s_addr = htonl(INADDR_ANY);
	else
	{
		unsigned long addr = inet_addr(m_config.Find("Server", "IP"));
		if (addr < 0)
		{
			bind_addr.s_addr = htonl(INADDR_ANY);
			GAMELOG << init("SYS_ERR") << "Invalid IP address" << end;
		}
		else
			bind_addr.s_addr = addr;
	}
	
	/* Put the address that we've finally decided on into the logs */
	if (bind_addr.s_addr == htonl(INADDR_ANY))
		GAMELOG << init("SYSTEM") << "Binding to all IP interfaces on this m_host." << end;
	else
	{
		GAMELOG << init("SYSTEM") << "Binding only to IP address " << inet_ntoa(bind_addr) << end;
	}
	
	return (&bind_addr);
}

#if defined(CIRCLE_WINDOWS)
void CServer::Nonblock(socket_t s)
{
	unsigned long val = 1;
	ioctlsocket(s, FIONBIO, &val);
}
#else
#  ifndef O_NONBLOCK
#    define O_NONBLOCK O_NDELAY
#  endif
void CServer::Nonblock(socket_t s)
{
	int flags;
	flags = fcntl(s, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if (fcntl(s, F_SETFL, flags) < 0)
	{
		GAMELOG << init("SYS_ERR") << "Fatal error excuting nonblock (CServer.cpp)" << end;
		
		exit(1);
	}
}
#endif

#ifdef CIRCLE_WINDOWS
void CServer::ServerSleep(struct timeval* timeout)
{
	Sleep(timeout->tv_sec * 1000 + timeout->tv_usec / 1000);
}
#else
void CServer::ServerSleep(struct timeval* timeout)
{
	if (select(0, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, timeout) < 0)
	{
		if (errno != EINTR)
		{
			GAMELOG << init("SYS_ERR") << "Select sleep" << end;
			exit(1);
		}
	}
}
#endif /* end of CIRCLE_WINDOWS */

int CServer::NewDescriptor(int s)
{
	socket_t m_desc;
	socklen_t i;
	static int last_desc = 0;	/* last descriptor number */
	CDescriptor* newd;
	struct sockaddr_in peer;
	struct hostent *from;
	
	i = sizeof(peer);
	if ((m_desc = accept(s, (struct sockaddr *)&peer, &i)) == INVALID_SOCKET)
		return (-1);
	
	/* keep it from blocking */
	Nonblock(m_desc);
	
	/* set the send buffer size */
	if (SetSendbuf(m_desc) < 0)
	{
		CLOSE_SOCKET(m_desc);
		return (0);
	}
	
	/* create a new descriptor */
	newd = new CDescriptor;
	
	/* find the sitename */
	if (m_nameserverisslow
		|| !(from = gethostbyaddr((char*)&peer.sin_addr, sizeof(peer.sin_addr), AF_INET)))
	{
		/* resolution failed */
		if (!m_nameserverisslow)
			GAMELOG << init("SYS_ERR") << "Calling gethostbyaddr()" << end;
		
		/* find the numeric site address */
		newd->m_host.CopyFrom((char*)inet_ntoa(peer.sin_addr), HOST_LENGTH);
	}
	else
	{
		newd->m_host.CopyFrom(from->h_name, HOST_LENGTH);
	}
	
	/* initialize descriptor data */
	newd->m_desc = m_desc;
	STATE(newd) = CON_GET_LOGIN;
	
	/*
	* This isn't exactly optimal but allows us to make a design choice.
	* Do we embed the history in CDescriptor or keep it dynamically
	* allocated and allow a user defined history size?
	*/
	if (++last_desc == 100000000)
		last_desc = 1;
	newd->m_descnum = last_desc;
	
	/* prepend to list */
	ADD_TO_BILIST(newd, m_desclist, m_pPrev, m_pNext);
	
	return (0);
}

void CServer::CloseSocket(CDescriptor* d)
{
	// 로그 남기고 사용자 정리
	GAMELOG << init("DISCONNECT", d->m_host)
		<< end;
	
	// 사용자 루프 돌면서
	if (!d->m_bLoginServer && d->m_subno > 0)
	{
		int i;
		for (i = 0; i <= m_userList[d->m_subno - 1].m_last; i++)
		{
			// 현재 접속 종료된 서버의 사용자이면
			if (m_userList[d->m_subno - 1].m_users[i] && m_userList[d->m_subno - 1].m_users[i]->m_descserver == d)
			{
				// 로그아웃 처리
				CNetMsg msg;
				msg.Init(MSG_CONN_REQ);
				msg << (unsigned char)MSG_CONN_LOGOUT_REQ
					<< m_userList[d->m_subno - 1].m_users[i]->m_name;
				ConnLogout(d, msg);
			}
		}
	}
	
	REMOVE_FROM_BILIST(d, m_desclist, m_pPrev, m_pNext);
	d->CloseSocket();
	
	delete d;
}

void CServer::SendOutput(CDescriptor* d)
{
	if (d == NULL) return;
	if (d->m_bclosed) return;
	if (d->m_outBuf.GetNextPoint())
	{
		if (FD_ISSET(d->m_desc, &m_output_set))
		{
			if (d->ProcessOutput() < 0)
				d->m_bclosed = true;
		}
	}
}

bool CServer::DisConnectLogedIn(CDescriptor* my_d)
{
	CDescriptor* d;
	CDescriptor* dNext = m_desclist;
	
	while ((d = dNext))
	{
		dNext = d->m_pNext;
		STATE(d) = CON_CLOSE;
	}
	return false;
}

LONGLONG CServer::GetServerTime()
{
	time_t ct;
	struct tm *ti;
	
	ct = time(&ct);
	ti = localtime(&ct);
	m_serverTime.year	= ti->tm_year + 1900;
	m_serverTime.month	= ti->tm_mon + 1;
	m_serverTime.day	= ti->tm_mday;
	m_serverTime.hour	= ti->tm_hour;
	m_serverTime.min	= ti->tm_min;
	m_serverTime.sec	= ti->tm_sec;
	
	return (LONGLONG)time(NULL) * 60;
}

void CServer::SavePlayersCount()
{
time_t now;
	time(&now);
	
	if (m_pulseSaveCount > now && m_pulseSaveMax > now)
		return ;
	
	CDBCmd cmd;
	int i;
	int s;
	
	cmd.Init(&m_dbuser);
	
	// 존별 사용자 수 저장
	for (s = 0; s < m_maxSubServer; s++)
	{
#ifdef EXTERNAL_COUNT
		if( m_pulseSaveCount <= now )
		{
			for(i = 0; i < gserver.m_extTableCount; i++)
			{
				if( m_userList[s].m_exCount && m_userList[s].m_exCount[i] > 0 )
				{
					StrLwr(gserver.m_extTableList[i]);
					sprintf(g_buf, "insert into t_connect_%s ( a_count, a_date, a_server, a_sub_num ) "
						"values ( %d, FROM_UNIXTIME(%d), %d, %d) ", gserver.m_extTableList[i], m_userList[s].m_exCount[i], (int) m_pulseSaveCount, m_serverno, s+1);

					StrUpr(gserver.m_extTableList[i]);

					cmd.SetQuery(g_buf);
					cmd.Update();

					GAMELOG << init("EX_COUNT") << "Company" << delim << gserver.m_extTableList[i] << delim 
							<< "Sub" << delim << (s+1) << delim << m_userList[s].m_exCount[i] << end;
				}
				
			}
#ifdef LIMIT_CATALOG
			// 팔릿 갯수에 벼화가 있으면 저장
			if(gserver.m_changeLimit)
			{
				// 한정 판매 팔린 수량도 5분마다 일단 저장
				// 한정 상품이 팔린 갯수를 디비에 저장
				int count;
				void* pos;
				count = gserver.m_limitSell.GetCount();
				pos = gserver.m_limitSell.GetHead();
				CLimitSell* limit;
				for(i = 0; i < count; i++)
				{
					if( !pos ) 
						continue;
					limit = gserver.m_limitSell.GetData(pos);
					if( limit->IsChange() )
					{
						sprintf(g_buf, " update t_ct_limit set a_sell = %d where a_server = %d and a_ct_id = %d ", limit->GetSell(), gserver.m_serverno, limit->GetIndex() );
						cmd.SetQuery(g_buf);
						// 업데이트가 실패하면 현재 등록안된 상품이니까 인서트
						if( !cmd.Update() || cmd.m_nrecords < 1)
						{
							sprintf(g_buf, "insert into t_ct_limit (a_server, a_ct_id, a_sell ) values( %d, %d, %d) ", gserver.m_serverno, limit->GetIndex(), limit->GetSell() );
							cmd.SetQuery(g_buf);
							cmd.Update();
						}
					}

					pos = gserver.m_limitSell.GetNext(pos);
				}
				gserver.m_changeLimit = false;
				GAMELOG << init("SYSTEM")
						<< "t_ct_limt UPDATED" << end;
			}
#endif
		}
		/*
		if( m_userList[s].m_nmCount > 0 && )
		{
			sprintf(g_buf, "insert into t_connect_nm ( a_count, a_date, a_server, a_sub_num ) "
				"values ( %d, FROM_UNIXTIME(%d), %d, %d) ", m_userList[s].m_nmCount, (int) m_pulseSaveCount, m_serverno, s+1);

			cmd.SetQuery(g_buf);
			cmd.Update();

			GAMELOG << init("NM_COUNT") << "Sub" << delim << (s+1) << delim << m_userList[s].m_nmCount << end;
		}
		*/
#endif
		for (i = 0; i < MAX_ZONES; i++)
		{
			if (m_userList[s].m_playersPerZone[i] < 0)
				continue ;
			
			// t_connect_count 갱신: 10분
			if (m_pulseSaveCount <= now)
			{
				// INSERT INTO t_connect_count (a_count, a_date, a_server, a_sub_num, a_zone_num) VALUES (count, date, server, subno, zoneno)
				strcpy(g_buf, "INSERT INTO t_connect_count (a_count, a_date, a_server, a_sub_num, a_zone_num) VALUES (");
				IntCat(g_buf, m_userList[s].m_playersPerZone[i], false);
				strcat(g_buf, ", FROM_UNIXTIME(");
				IntCat(g_buf, (int) m_pulseSaveCount);
				strcat(g_buf, "), ");
				//strcat(g_buf, ", date_format(now(), '%Y-%m-%d %H:%i:00'), ");
				IntCat(g_buf, m_serverno, false);
				strcat(g_buf, ", ");
				IntCat(g_buf, s + 1, false);
				strcat(g_buf, ", ");
				IntCat(g_buf, i, false);
				strcat(g_buf, ")");
				
				cmd.SetQuery(g_buf);
				cmd.Update();
				
				GAMELOG << init("COUNT") << "Sub" << delim << (s + 1) << delim << "Zone" << delim << i << delim << m_userList[s].m_playersPerZone[i] << end;
				
			}
			
			// t_connect_max 갱신: 2분
			if (m_pulseSaveMax <= now)
			{
				// 오늘 날짜로 찾고
				// SELECT * FROM t_connect_max WHERE a_date=CURDATE() AND a_server=server AND a_sub_num=subno AND a_zone_num=zone
				strcpy(g_buf, "SELECT * FROM t_connect_max WHERE a_date=CURDATE() AND a_server=");
				IntCat(g_buf, m_serverno, false);
				strcat(g_buf, " AND a_sub_num=");
				IntCat(g_buf, s + 1, false);
				strcat(g_buf, " AND a_zone_num=");
				IntCat(g_buf, i, false);
				
				cmd.SetQuery(g_buf);
				
				if (cmd.Open())
				{
					if (cmd.MoveNext())
					{
						// 있으면 카운트 비교해서 큰수 저장하고
						LONGLONG index;
						int countmax = 0;
						cmd.GetRec("a_index", index);
						cmd.GetRec("a_count_max", countmax);
						if (countmax < m_userList[s].m_playersPerZone[i])
							countmax = m_userList[s].m_playersPerZone[i];
						// 업데이트
						// UPDATE t_connect_max SET a_count=count, a_count_max=countmax WHERE a_index=index
						strcpy(g_buf, "UPDATE t_connect_max SET a_count=");
						IntCat(g_buf, m_userList[s].m_playersPerZone[i], false);
						strcat(g_buf, ", a_count_max=");
						IntCat(g_buf, countmax, false);
						strcat(g_buf, " WHERE a_index=");
						IntCat(g_buf, index, false);
						
						cmd.SetQuery(g_buf);
						cmd.Update();
					}
					else
					{
						// 없으면 추가
						// INSERT INTO t_connect_max (a_count, a_count_max, a_date, a_server, a_sub_num, a_zone_num) VALUES (count, countmax, CURDATE(), server, subno, zone)
						strcpy(g_buf, "INSERT INTO t_connect_max (a_count, a_count_max, a_date, a_server, a_sub_num, a_zone_num) VALUES (");
						IntCat(g_buf, m_userList[s].m_playersPerZone[i], false);
						strcat(g_buf, ", ");
						IntCat(g_buf, m_userList[s].m_playersPerZone[i], false);
						strcat(g_buf, ", CURDATE(), ");
						IntCat(g_buf, m_serverno, false);
						strcat(g_buf, ", ");
						IntCat(g_buf, s + 1, false);
						strcat(g_buf, ", ");
						IntCat(g_buf, i, false);
						strcat(g_buf, ")");
						
						cmd.SetQuery(g_buf);
						cmd.Update();
					}
					
				} // if (cmd.Open())
				
			} // if (m_pulseSaveMax < 1)
		} // for (i = 0; i < MAX_ZONES; i++)
	}

	struct tm *saveCount;
	if (m_pulseSaveCount <= now)
	{
		saveCount = localtime(&m_pulseSaveCount);
		if( saveCount )
		{
			saveCount->tm_min += PULSE_SAVECOUNT_NEW;
			m_pulseSaveCount = mktime(saveCount);
		}
			
	}
	if (m_pulseSaveMax <= now)
	{
		saveCount = localtime(&m_pulseSaveMax);
		if( saveCount )
		{
			saveCount->tm_min += PULSE_SAVEMAXCOUNT_NEW;
			m_pulseSaveMax = mktime(saveCount);
		}
			
	}
}

void CServer::SendAllGameServer(CNetMsg& msg)
{
	CDescriptor* desc = gserver.m_desclist;
	while (desc)
	{
		if( desc->m_subno >= 1 && desc->m_subno <= gserver.m_maxSubServer )
			SEND_Q(msg, desc);
		desc = desc->m_pNext;
	}
}

#ifdef EVENT_XMAS_2007
void CServer::TimerToGameServer()
{
	// 시간에 따라 공지 날리기
	struct tm currentTime = NOW();
	int nXMasTreeTimeDiff;
	CNetMsg rmsg;
	if( m_nEventXmasTreeNoticeTime > 0 )
	{
		// 5분 타이머 체크 시작
		if( !m_bEventXmasTimerStart && m_nEventXmasTreeNoticeTime%6 == 5 )
		{
			// 5분 남음 공지
			ConnEventXmas2007Msg( rmsg , MSG_CONN_EVENT_XMASTREE_ITEMDROP) ;
			rmsg << (int) 5;		// 5분 남았음 MSG
			gserver.SendAllGameServer(rmsg);
			m_bEventXmasTimerStart = true;

			GAMELOG << init("5 Min Notice XMAS EVENT ITEM : Remain Time " ) <<  m_nEventXmasTreeNoticeTime << end; 
		}

		if( m_bEventXmasTimerStart && m_nEventXmasTreeNoticeTime%6 == 1 )
		{
			// ITEM DROP
			ConnEventXmas2007Msg( rmsg , MSG_CONN_EVENT_XMASTREE_ITEMDROP) ;
			rmsg << (int) 0;		// 0분 남았음 MSG ITEM DROP
			gserver.SendAllGameServer(rmsg);
			
			GAMELOG << init("DROP XMAS EVENT ITEM : Remain Time " ) <<  m_nEventXmasTreeNoticeTime << end; 

			if( m_nEventXmasTreeNoticeTime == 1 )
			{
				m_nEventXmasTreeNoticeTime =0;
			}
			m_bEventXmasTimerStart = false;
		}

		nXMasTreeTimeDiff = currentTime.tm_min - m_nEventXmasTreeOldTime ;
		if (nXMasTreeTimeDiff < 0)
			nXMasTreeTimeDiff += 60;
		m_nEventXmasTreeNoticeTime -= nXMasTreeTimeDiff;
	}
	m_nEventXmasTreeOldTime = currentTime.tm_min;
}
#endif // EVENT_XMAS_2007