#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"
#include "DBCmd.h"
#include "CmdMsg.h"
#include "ProcConnMsg.h"


#if defined (USE_TENTER_BILLING)
#include "ProcConnMsg_TLD.inc"
#else
#include "ProcConnMsg_KOR.inc"
#endif

MSG_CONN_ERRORCODE ReadDB(const char* name, const char* pw, int server, int subno, const char* ip, CUser** user, char playmode)
{
// 050226 : bs : 접속 가능 여부 검사
// 050310 : bs : 아이피 제한 없애고 chk_tester 사용하도록
//#ifdef LC_TWN
//	bool bIPPass = true;
//
//	// 대역 리스트에 없으면
//	strcpy(g_buf, ip);
//	if (!CheckIP(g_buf))
//		bIPPass = false;
//#endif
// --- 050310 : bs : 아이피 제한 없애고 chk_tester 사용하도록
// --- 050226 : bs : 접속 가능 여부 검사

// #ifdef CLOSED_BETA_TEST
// 
// 	// SELECT user_code, chk_tester FROM bg_user WHERE user_id='name' AND passwd='pw'
// 	strcpy(g_buf, "SELECT user_code, chk_tester FROM bg_user WHERE user_id='");
// 	strcat(g_buf, name);
// 	strcat(g_buf, "' AND passwd='");
// 	strcat(g_buf, pw);
// 	strcat(g_buf, "'");
// 
// #else 

	// 포털에서 아이디/패스워드 검사
	CLCString sql(MAX_STRING_LENGTH);
#ifdef LC_TWN
	sql.Format("select user_code, chk_tester from bg_user where user_id = '%s' and passwd = '%s' ", name, pw );
#elif LC_TLD
	sql.Format("select a_index, a_specialsw, a_special, a_stash_password from bg_user where a_gametype = 'LC' and a_idname = '%s' and a_passwd = '%s' ", name, pw );
#else
#ifdef EMPAS_LOGIN
#ifdef LC_KOR
	sql.Format("select t1.user_code, t1.jumin, t1.partner_id, unix_timestamp(t1.create_date) as creat FROM bg_user as t1 left join bg_user_active as t2 on t1.user_code = t2.active_code "
		" WHERE t1.user_id='%s' AND ( ( t1.passwd=password('%s') and t1.partner_id != 'B1' ) or (t2.active_code = t1.user_code and t2.active_game = 'LC' and t2.active_passwd = '%s' and unix_timestamp(t2.active_time) + 600 > unix_timestamp(now()) ) ) "
		, name, pw, pw );
#else
	sql.Format("select t1.user_code, t1.jumin, t1.partner_id, unix_timestamp(t1.create_date) as creat FROM bg_user as t1 left join bg_user_active as t2 on t1.user_code = t2.active_code "
		" WHERE t1.user_id='%s' AND ( ( t1.passwd='%s' and t1.partner_id != 'B1' ) or (t2.active_code = t1.user_code and t2.active_game = 'LC' and t2.active_passwd = '%s' and unix_timestamp(t2.active_time) + 600 > unix_timestamp(now()) ) ) "
		, name, pw, pw );
#endif // LC_KOR
#else//#ifdef EMPAS_LOGIN
#ifdef LC_MAL
sql.Format("select user_code, partner_id, unix_timestamp(create_date) as creat FROM bg_user "
		" WHERE user_id='%s' AND passwd='%s'"
			, name, pw );
#else
	sql.Format("select user_code, jumin, partner_id, unix_timestamp(create_date) as creat FROM bg_user "
		" WHERE user_id='%s' AND passwd='%s'"
			, name, pw );
#endif
#endif//#ifdef EMPAS_LOGIN
#endif
/*
#ifdef LC_TWN
	strcpy(g_buf, "SELECT user_code, chk_tester FROM bg_user WHERE user_id='");
#elif defined(LC_TLD)
	strcpy(g_buf, "SELECT a_index, a_specialsw, a_special FROM bg_user WHERE a_gametype = 'LC' and a_idname='");
#else
#ifdef EVENT_PROMOTION_SITE
	strcpy(g_buf, "SELECT user_code, jumin, partner_id, unix_timestamp(create_date) as creat FROM bg_user WHERE chk_service = 'Y' and user_id='");
#else
	strcpy(g_buf, "SELECT user_code, jumin FROM bg_user WHERE user_id='");
#endif
#endif

	strcat(g_buf, name);
#ifdef LC_TLD
	strcat(g_buf, "' AND a_passwd='");
#else
	strcat(g_buf, "' AND passwd='");
#endif
	strcat(g_buf, pw);
	strcat(g_buf, "'");
	*/

// #endif //CLOSED_BETA_TEST
	CDBCmd cmd;
#ifdef	IP_BLOCK_BAND
	CLCString blockSql(1000);
	blockSql.Format(" select a_index from t_ip_block where a_ip = '%s' ", ip );
	cmd.Init(&gserver.m_dbuser);
	cmd.SetQuery(blockSql);
	cmd.Open();

	if( cmd.m_nrecords > 0 )
	{
		return MSG_CONN_ERROR_NOMATCH;
	}

#endif

	cmd.Init(&gserver.m_dbauth);
	cmd.SetQuery(sql);

	if (!cmd.Open())
		return MSG_CONN_ERROR_SYSTEM;

	if (cmd.m_nrecords < 1 || !cmd.MoveFirst())
		return MSG_CONN_ERROR_NOMATCH;


	int portal_idx;
#ifdef LC_TLD
	char m_specialsw;
	char m_special;
	cmd.GetRec("a_index", portal_idx);
	cmd.GetRec("a_specialsw", m_specialsw);
	cmd.GetRec("a_special", m_special);
#else
#ifdef EVENT_PROMOTION_SITE
	int creat_date = 1139482800;
	CLCString proSite(3);
	cmd.GetRec("creat", creat_date);
	cmd.GetRec("partner_id", proSite);

	if( proSite.Length() < 1 )
		proSite = "LC";

#ifdef LC_JPN
	// 2006-02-09 20 이전 가입자 : 1139482800(타임 스탬프)   2006-09-20 이후 가입자 : 1158715200(타임 스탬프 )
	if (  creat_date < 1139482800 && strcmp(proSite, "NM") != 0 )
	{
		proSite = "CL";
	}
#ifdef EVENT_PARTNER_CODENEO
	bool bSetIsCharacter = false;
	if  ( !( creat_date > 1158591600 && creat_date < 1164034800 && strcmp( proSite, "CO" ) == 0) )
	{
		bSetIsCharacter = true;
	}
#endif // EVENT_PARTNER_CODENEO

#endif // LC_JPN

#ifdef EVENT_JPN_2007_NEWSERVER
	bool bNewUser_20070301 = false;
	const int nTime20070301 = 1172674800;
	int nCreateTime = 0;
	cmd.GetRec("creat", nCreateTime);
	if (nCreateTime >= nTime20070301)
		bNewUser_20070301 = true;
#endif // EVENT_JPN_2007_NEWSERVER

#endif//#ifdef EVENT_PROMOTION_SITE
	cmd.GetRec("user_code", portal_idx);
#endif

#ifdef CHARDEL_CHECKID
#ifdef LC_TLD
	CLCString ident(20);
	cmd.GetRec("a_stash_password", ident);
#else//#ifdef LC_TLD
	CLCString ident(20);
	cmd.GetRec("jumin", ident);

#ifdef CHECK_LIMIT_AGE
	bool bAllowLimitAge = true;
	if (ident.Length() == 14)
	{
		if (!CheckBirthDay(ident, CHECK_LIMIT_AGE))
		{
			bAllowLimitAge = false;
			GAMELOG << init("CHECK SID : DENY", name)
					<< "SID" << delim
					<< ident
					<< end;
		}
	}
	else
	{
		GAMELOG << init("CHECK SID : SKIP", name)
				<< "SID" << delim
				<< ident
				<< end;
	}
	// 회사는 나이 검사 로그만 남기고 통과
	if (!bAllowLimitAge 
		&& strncmp(ip, "61.104.44.", strlen("61.104.44."))
		&& strncmp(ip, "211.112.36.", strlen("211.112.36.")) )
		return MSG_CONN_ERROR_NOMATCH;
#endif // CHECK_LIMIT_AGE

	strcpy(g_buf, ident);

	char* pId = NULL;
	pId = strtok(g_buf, "-");
	pId = strtok(NULL, "-");

	ident = pId;
#endif//#ifdef LC_TLD
#endif//#ifdef CHARDEL_CHECKID

#ifdef LC_TWN
// 050310 : bs : 아이피 제한 없애고 chk_tester 사용하도록
//	// ip pass에 실패했을 경우에만 체크 한다.
//	if (!bIPPass)
//	{
// #ifdef CLOSED_BETA_TEST
// 
	CLCString chkTest(3);
	cmd.GetRec("chk_tester", chkTest);
	
	if (strcmp(chkTest, "Y") != 0 )
	{
// --- 050310 : bs : 아이피 제한 없애고 chk_tester 사용하도록
		CDBCmd gameip;
		gameip.Init(&gserver.m_dbuser);
		
		// 처음 점 찾고
		char* p = strchr(ip, '.');
		if (!p)
			return MSG_CONN_ERROR_NOMATCH;
		
		// 두번째 점 찾고
		p++;
		p = strchr(p, '.');
		if (!p)
			return MSG_CONN_ERROR_NOMATCH;
		
		// 세번째 점 찾고
		p++;
		p = strchr(p, '.');
		if (!p)
			return MSG_CONN_ERROR_NOMATCH;
		
		strncpy(g_buf, ip, p - ip);
		g_buf[p - ip] = '\0';		// xxx.xxx.xxx
		strcpy(g_buf2, p + 1);		// xxx
		int n = atoi(g_buf2);		// 끝 자리 숫자로
		
		// 겜 아이피 테이블에서 찾고
		// SELECT * FROM t_game_ip WHERE a_ip_prefix='xxx.xxx.xxx' AND a_enable=1
		strcpy(g_buf3, "SELECT * FROM t_game_ip WHERE a_ip_prefix='");
		strcat(g_buf3, g_buf);
		strcat(g_buf3, "' AND a_enable=1");
		gameip.SetQuery(g_buf3);
		if (!gameip.Open() && gameip.m_nrecords < 1)
			return MSG_CONN_ERROR_NOMATCH;
		
		// 루프 돌면서
		bool bFound = false;
		while (gameip.MoveNext())
		{
			int sip, eip;
			// 시작 끝 안에 들어가는지 검사
			if (gameip.GetRec("a_ip_start", sip) && gameip.GetRec("a_ip_end", eip) && sip <= n && n <= eip)
			{
				bFound = true;
				break;
			}
		}
		
		if (!bFound)
		{
			GAMELOG << init("BLOCK IP", name, ip) << end;
			return MSG_CONN_ERROR_NOMATCH;
		}
	}
// 
// #endif //CLOSED_BETA_TEST
#endif


	// 게임 데이터베이스에서 찾기
	// 이름으로 찾기
#ifdef PREOPEN_CHECK
//0627
	strcpy(g_buf, "SELECT a_portal_index, a_zone_num, a_timestamp, a_enable, a_preopen_chk, UNIX_TIMESTAMP(a_regi_date) AS `createtime` FROM t_users WHERE a_idname='");
#else
#ifdef EVENT_PCBANG_TIME_SAVE
	strcpy(g_buf, "SELECT a_portal_index, a_zone_num, a_timestamp, a_enable, UNIX_TIMESTAMP(a_end_date) AS `endDate`, UNIX_TIMESTAMP(a_regi_date) AS `createtime`, a_pcbang_time FROM t_users WHERE a_idname='");
#else
	strcpy(g_buf, "SELECT a_portal_index, a_zone_num, a_timestamp, a_enable, UNIX_TIMESTAMP(a_regi_date) AS `createtime` FROM t_users WHERE a_idname='");
#endif // #ifdef EVENT_PCBANG

#endif
	strcat(g_buf, name);
	strcat(g_buf, "'");

	cmd.Init(&gserver.m_dbuser);
	cmd.SetQuery(g_buf);

	if (!cmd.Open())
		return MSG_CONN_ERROR_SYSTEM;

	bool bFirstConn = true;

	//0627
	time_t	currenttime;
	time(&currenttime);
	int regi_time = 0;
#ifdef EVENT_PCBANG_TIME_SAVE
	int endDate = 0;
	int pcBangTime = 0;
#endif
	// 050124 - bs - 계정 이름으로 검사하여 포털 인덱스 확인
	if (cmd.m_nrecords > 0 && cmd.MoveNext())
	{
		int oldportalindex;
		cmd.GetRec("a_portal_index", oldportalindex);
#ifdef EVENT_PCBANG_TIME_SAVE
		cmd.GetRec("endDate", endDate);
		if( endDate != 0 && endDate + 60 * 60 * 4 > time(NULL) )
			cmd.GetRec("a_pcbang_time",  pcBangTime);
#endif

		if (oldportalindex != portal_idx)
		{
			// 기존 데이터가 잘못되어 있음
			strcpy(g_buf, "UPDATE t_users SET a_idname='");
			strcat(g_buf, name);
			strcat(g_buf, "_CS");
			IntCat(g_buf, (LONGLONG)time(NULL), false);
			strcat(g_buf, "', a_end_date=NOW() WHERE a_portal_index=");
			IntCat(g_buf, oldportalindex, false);
			cmd.SetQuery(g_buf);
			if (!cmd.Update())
				return MSG_CONN_ERROR_NOMATCH;
			bFirstConn = true;
		}
		else
		{
			bFirstConn = false;
//0627			
			cmd.GetRec("createtime", regi_time);
		}
	}	
	

	if (bFirstConn)
	{
		// 처음 게임에 접속
		// 게임 유저 테이블에 추가
		// INSERT INTO t_users (a_idname, a_passwd, a_portal_index, a_regi_date, a_end_date) VALUES ('name', 'pw', portal_idx, NOW(), NOW())
		strcpy(g_buf, "INSERT INTO t_users (a_idname, a_portal_index, a_regi_date, a_end_date) VALUES ('");
		strcat(g_buf, name);
		strcat(g_buf, "', ");
		IntCat(g_buf, portal_idx, false);
		strcat(g_buf, ", NOW(), NOW())");

		cmd.SetQuery(g_buf);

		if (!cmd.Update())
			return MSG_CONN_ERROR_ALREADY;

#ifdef CHARDEL_CHECKID
		*user = new CUser(portal_idx, name, server, subno, ip, ident);
#else
		*user = new CUser(portal_idx, name, server, subno, ip);
#endif

		(*user)->m_userFlag |= NOVICE;

#ifdef LC_JPN
#ifdef EVENT_PARTNER_CODENEO
		if (bSetIsCharacter)
		{
			(*user)->m_userFlag |= IS_CHARACTER;
		}
#endif // EVENT_PARTNER_CODENEO
#endif	// LC_JPN

#ifdef EVENT_JPN_2007_NEWSERVER
		if (bNewUser_20070301)
			(*user)->m_userFlag |= IS_NEWUSER_JPN_2007_03;
#endif // EVENT_JPN_2007_NEWSERVER
		
#ifdef GMTOOL
		if(playmode == MSG_LOGIN_GM)
		{
			(*user)->m_userFlag |= IS_GM_CHAR;
		}
#endif // GMTOOL

#ifdef EVENT_PROMOTION_SITE
		(*user)->m_proSite = proSite;
#endif//#ifdef EVENT_PROMOTION_SITE
		
		return MSG_CONN_ERROR_SUCCESS;
	}
	else
	{
		// 이전에 플레이 한 적 있는 유저
		int oldzone;
		if (!cmd.GetRec("a_zone_num", oldzone))
			return MSG_CONN_ERROR_SYSTEM;

// 050226 : bs : a_enable 검사
		switch (atoi(cmd.GetRec("a_enable")))
		{
		case 0:
			return MSG_CONN_ERROR_NOMATCH;
		case 1:
			break;
		default:
			return MSG_CONN_ERROR_BLOCKEDUSER;
		}
// --- 050226 : bs : a_enable == 0 검사

		if (oldzone < 0)
		{
#ifdef CHARDEL_CHECKID
			*user = new CUser(portal_idx, name, server, subno, ip, ident);
#else
			*user = new CUser(portal_idx, name, server, subno, ip);
#endif

			cmd.GetRec("a_timestamp", (*user)->m_timestamp);
#ifdef PREOPEN_CHECK
			char preopenchk = 0;
			cmd.GetRec("a_preopen_chk", preopenchk);
			(*user)->m_preopen_chk = preopenchk;
#endif
			//0627
			if(currenttime - regi_time < 64*60*60)
				(*user)->m_userFlag |= NOVICE;

#ifdef LC_JPN
#ifdef EVENT_PARTNER_CODENEO
			if (bSetIsCharacter)
			{
				(*user)->m_userFlag |= IS_CHARACTER;
			}

			if ( !( (*user)->m_userFlag & IS_CHARACTER ) )
			{
				sprintf(g_buf, "select a_is_character from t_character_exist where a_user_idx = '%d'", portal_idx );
				cmd.SetQuery(g_buf);

				if( cmd.Open() && cmd.m_nrecords > 0 )
				{
					cmd.MoveFirst();
					int isCharacter = 0;
					cmd.GetRec( "a_is_character", isCharacter );
					if( isCharacter != 0 )
					{
						(*user)->m_userFlag |= IS_CHARACTER;
					}
				}
			}
#endif // EVENT_PARTNER_CODENEO
#endif // LC_JPN
			
#ifdef EVENT_PCBANG
			sprintf(g_buf, "select a_index from t_pcbang where a_ip = '%s' and a_enable = 1 ", ip );
			cmd.SetQuery(g_buf);

			if( cmd.Open() && cmd.m_nrecords > 0)
			{
				(*user)->m_location = BILL_LOCATION_PCBANG;
				(*user)->m_pcbang_time = pcBangTime;
			}
			else
				(*user)->m_pcbang_time = 0;
#endif//#ifdef EVENT_PCBANG
			
#ifdef EVENT_PCBANG_2ND
			sprintf(g_buf, "select a_index from t_pcbang_2nd where a_ip=md5('%s') and a_enable=1", ip );
			cmd.SetQuery(g_buf);

			if(cmd.Open() && cmd.m_nrecords > 0)
			{
				(*user)->m_location = BILL_LOCATION_PCBANG;

				// 피씨방 로그인 기록 남기기
				CDBCmd cmdPCBang;
				cmdPCBang.Init(&gserver.m_dbuser);

				sprintf(g_buf, "insert into t_pcbang_log values(%d, '%s', %d, now())",  portal_idx, ip, PCBANG_LOGIN);
				cmdPCBang.SetQuery(g_buf);
				cmdPCBang.Update();
			}
#endif // EVENT_PCBANG_2ND

#ifdef EVENT_PCBANG_3ND			
			char ip_prefix[12];
			int ip_cnt = 0, ip_compare = -1;
			char seps[]   = ".";
			char strings[16];
			memcpy(strings, ip, 16);
			char * token = strtok(strings, seps);
			while(token != NULL)
			{
				if(ip_cnt == 0)
				{
					sprintf(ip_prefix, "%s", token);
				}
				else if(ip_cnt == 3)
				{
					ip_compare = atoi(token);
					break;
				}
				else
				{
					sprintf(ip_prefix, "%s.%s", ip_prefix, token);
				}
				
				token = strtok(NULL, seps);
				ip_cnt++;
			}


			sprintf(g_buf, "select a_index from t_game_ip where a_ip_prefix='%s' and a_enable=1 and a_ip_start <= %d and a_ip_end >= %d", 
				ip_prefix, ip_compare, ip_compare);
			cmd.SetQuery(g_buf);

			if(cmd.Open() && cmd.m_nrecords > 0)
			{
				(*user)->m_location = BILL_LOCATION_PCBANG;

				// 피씨방 로그인 기록 남기기
				sprintf(g_buf, "insert into t_pcbang_log values(%d, '%s', %d, now())",  portal_idx, ip, PCBANG_LOGIN);
				cmd.SetQuery(g_buf);
				if(cmd.Update() == false)
				{
					GAMELOG << init("PCBANG_LOGIN DB INSERT FAIL") << portal_idx << delim << ip << end;
				}
				
			}
#endif // EVENT_PCBANG_3ND

#ifdef CASH_ITEM_GIFT
			sprintf(g_buf, "select a_index from t_gift0%d where a_recv_user_idx = %d and a_use_char_idx = 0  and a_server = %d", portal_idx % 10, portal_idx, gserver.m_serverno );
			cmd.SetQuery(g_buf);

			if( cmd.Open() && cmd.m_nrecords > 0 )
			{
				(*user)->m_userFlag |= RECV_GIFT;
			}
#endif

#ifdef LC_TLD
			if( m_specialsw )
			{
				if( m_special )
					(*user)->m_userFlag |= ADULT;
			}
			else
			{
				(*user)->m_userFlag |= ADULT;
			}
#endif
#ifdef EVENT_PROMOTION_SITE
			(*user)->m_proSite = proSite;
#endif//#ifdef EVENT_PROMOTION_SITE

#ifdef EVENT_JPN_2007_NEWSERVER
			if (bNewUser_20070301)
				(*user)->m_userFlag |= IS_NEWUSER_JPN_2007_03;
#endif // EVENT_JPN_2007_NEWSERVER

#ifdef GMTOOL
			if(playmode == MSG_LOGIN_GM)
			{
				(*user)->m_userFlag |= IS_GM_CHAR;
			}
#endif // GMTOOL
			return MSG_CONN_ERROR_SUCCESS;
		}
		else
			return MSG_CONN_ERROR_ALREADY;
	}
}

MSG_CONN_ERRORCODE WriteDB(int index, int server, int subno, int zone, int timestamp, const char* name, const char* ip, bool bLogin, int pcbangTime)
{
	// 0번 존에 접속한 것으로 일단 설정
	// 이후 다시 업데이트를 통해 실제 존을 설정 (MSG_MENU_START를 받는 시점에서)
	// UPDATE t_users SET a_end_date=NOW(), a_server_num=server, a_subnum=subnum, a_zone_num=zone, a_timestamp=timestamp WHERE a_portal_index=index
	strcpy(g_buf, "UPDATE t_users SET a_end_date=NOW(), a_server_num=");
	IntCat(g_buf, server, false);
	strcat(g_buf, ", a_subnum=");
	IntCat(g_buf, subno, false);
	strcat(g_buf, ", a_zone_num=");
	IntCat(g_buf, zone, false);
	strcat(g_buf, ", a_timestamp=");
	IntCat(g_buf, timestamp, false);
#ifdef PREOPEN_CHECK
	strcat(g_buf, ", a_preopen_chk=1");
#endif // PREOPEN_CHECK
#ifdef EVENT_PCBANG_TIME_SAVE
	strcat(g_buf, ", a_pcbang_time=");
	IntCat(g_buf, pcbangTime, false);
#endif
	strcat(g_buf, " WHERE a_portal_index=");
	IntCat(g_buf, index, false);

	if (bLogin)
		strcat(g_buf, " AND a_zone_num=-1");

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);
	cmd.SetQuery(g_buf);

	if (!cmd.Update())
		return MSG_CONN_ERROR_SYSTEM;

	if (cmd.m_nrecords < 1)
		return MSG_CONN_ERROR_ALREADY;

	if (name && *name && ip && *ip)
	{
		// INSERT INTO t_connect_log (a_idname, a_date, a_ip) VALUES ('name', NOW(), 'ip')
		strcpy(g_buf, "INSERT INTO t_connect_log (a_idname, a_date, a_ip) VALUES ('");
		strcat(g_buf, name);
		strcat(g_buf, "', NOW(), '");
		strcat(g_buf, ip);
		strcat(g_buf, "')");

		cmd.SetQuery(g_buf);

		cmd.Update();
	}

#ifdef UPDATE_USER_TO_MASTERDB
	/////////////////////////////////////////////////////////
	/*
	select ugame_index from bg_user_game
where ugame_user_code = 17 and ugame_game_code = 'LC'
*/
	if(bLogin)
	{
		CDBCmd cmdAuth;
		cmdAuth.Init(&gserver.m_dbauth);
		sprintf(g_buf, "select ugame_index from bg_user_game where ugame_user_code = %d and ugame_game_code = 'LC'", index);
		cmdAuth.SetQuery(g_buf);
		
		if(cmdAuth.Open())
		{
			// 있다.
			if(cmdAuth.MoveFirst())
			{
			/*
			UPDATE bg_user_game SET ugame_login_date= sysdate() WHERE ugame_index = 2
				*/
				int ugame_index;
				cmdAuth.GetRec("ugame_index", ugame_index) ;
				
				sprintf(g_buf, "UPDATE bg_user_game SET Ugame_login_date= sysdate() WHERE Ugame_index = %d", ugame_index);
				cmdAuth.SetQuery(g_buf);
				cmdAuth.Update();
			}
			// 없다.
			else
			{
			/*
			) INSERT INTO bg_user_game 
			(ugame_game_code, ugame_user_code, ugame_join_date, ugame_login_date)
			VALUES ('LC', 17, sysdate(), sysdate())
				*/
				sprintf(g_buf, "INSERT INTO bg_user_game (Ugame_game_code, Ugame_user_code, Ugame_join_date, Ugame_login_date) VALUES ('LC', %d, sysdate(), sysdate())", index);
				cmdAuth.SetQuery(g_buf);
				cmdAuth.Update();
			}
		}
	}
#endif // UPDATE_USER_TO_MASTERDB
	return MSG_CONN_ERROR_SUCCESS;
}

bool WriteDB(const char* name)
{
	// 0번 존에 접속한 것으로 일단 설정
	// 이후 다시 업데이트를 통해 실제 존을 설정 (MSG_MENU_START를 받는 시점에서)
	// UPDATE t_users SET a_end_date=NOW(), a_zone_num=zone WHERE a_name=name
	strcpy(g_buf, "UPDATE t_users SET a_end_date=NOW(), a_zone_num=");
	IntCat(g_buf, -1, false);
	strcat(g_buf, " WHERE a_idname='");
	strcat(g_buf, name);
	strcat(g_buf, "'");

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);
	cmd.SetQuery(g_buf);

	if (!cmd.Update())
		return false;

	return true;
}

void ConnPlayerReq(CDescriptor* d, CNetMsg& msg)
{
	CNetMsg rmsg;
	PlayerRepMsg(rmsg);
	SEND_Q(rmsg, d);
}

void ConnPlaying(CDescriptor* d, CNetMsg& msg)
{
	unsigned char subtype;
	msg.MoveFirst();
	msg >> subtype;

	CLCString id(MAX_ID_NAME_LENGTH + 1);
	unsigned char mode;
	int zone;

	msg >> id >> mode >> zone;

	if (d->m_subno < 1)
	{
		GAMELOG << init("SYS_ERR", id)
				<< "From" << delim << d->m_subno << delim
				<< "No match server or subno" << end;
		return ;
	}

	CUser* user = gserver.m_userList[d->m_subno - 1].Find(id);

	if (!user)
	{
		GAMELOG << init("SYS_ERR", id)
				<< "From" << delim << d->m_subno << delim
				<< "Change playing state: Not Found User" << end;
		return ;
	}

	GAMELOG << init("CHANGE_STATE", id)
			<< "From" << delim << d->m_subno << delim
			<< ((mode == MSG_LOGIN_NEW) ? "PLAYING" : "CHANGE_SERVER")
			<< delim
			<< user->m_zone
			<< delim
			<< zone
			<< end;

	gserver.m_userList[d->m_subno - 1].m_playersPerZone[user->m_zone]--;
	gserver.m_userList[d->m_subno - 1].m_playersPerZone[zone]++;

	user->m_state = (mode == MSG_LOGIN_NEW) ? CONN_PLAYING : CONN_CHANGE_SERVER;
	user->m_zone = zone;
	user->m_checkTics = 0;
#ifdef EVENT_PCBANG_TIME_SAVE
	WriteDB(user->m_index, user->m_server, user->m_subnum, user->m_zone, user->m_timestamp, NULL, NULL, false, user->m_pcbang_time);
#else
	WriteDB(user->m_index, user->m_server, user->m_subnum, user->m_zone, user->m_timestamp, NULL, NULL, false);
#endif

	if (user->m_state == CONN_CHANGE_SERVER)
	{
		// 서버 이동을 완료 시켜주기 위해 응답을 준다
		CNetMsg rmsg;
		MoveServerOKMsg(rmsg, user->m_index);
		SEND_Q(rmsg, d);
	}
}

void ConnPreopenGift(CDescriptor* d, CNetMsg& msg)
{
#ifdef EVENT_PREOPEN_GIFT

	unsigned char subtype;
	msg.MoveFirst();
	msg >> subtype;
	int userindex;
	int charindex;
	char bcancel;
	msg >> userindex
		>> charindex
		>> bcancel;

	if (d->m_subno < 1)
		return ;

	CNetMsg rmsg;

	CUser* user = gserver.m_userList[d->m_subno - 1].Find(userindex);
	if (user == NULL)
	{
		PreopenGiftErrorMsg(rmsg, userindex, charindex, MSG_CONN_ERROR_PREOPEN_GIFT_INSUFF_CONDITION);
		SEND_Q(rmsg, d);
		return ;
	}

	if (bcancel == 0)
	{
		switch (user->m_preopen_chk)
		{
		case 0:
			PreopenGiftErrorMsg(rmsg, userindex, charindex, MSG_CONN_ERROR_PREOPEN_GIFT_INSUFF_CONDITION);
			break ;

		case 1:
			{
				// UPDATE t_users SET a_preopen_chk=2 WHERE a_portal_index=userindex AND a_preopen_chk=1
				strcpy(g_buf, "UPDATE t_users SET a_preopen_chk=2 WHERE a_portal_index=");
				IntCat(g_buf, userindex, false);
				strcat(g_buf, " AND a_preopen_chk=1");

				CDBCmd cmd;
				cmd.Init(&gserver.m_dbuser);
				cmd.SetQuery(g_buf);
				if (cmd.Update() && cmd.m_nrecords > 0)
				{
					PreopenGiftErrorMsg(rmsg, userindex, charindex, MSG_CONN_ERROR_SUCCESS);
					user->m_preopen_chk = 2;
				}
				else
				{
					PreopenGiftErrorMsg(rmsg, userindex, charindex, MSG_CONN_ERROR_PREOPEN_GIFT_ALREADY_GIVE);
				}
			}
			break ;

		default:
			PreopenGiftErrorMsg(rmsg, userindex, charindex, MSG_CONN_ERROR_PREOPEN_GIFT_ALREADY_GIVE);
			break ;
		}
		SEND_Q(rmsg, d);
	}
	else
	{
		// UPDATE t_users SET a_preopen_chk=1 WHERE a_portal_index=userindex AND a_preopen_chk=2
		strcpy(g_buf, "UPDATE t_users SET a_preopen_chk=1 WHERE a_portal_index=");
		IntCat(g_buf, userindex, false);
		strcat(g_buf, " AND a_preopen_chk=2");

		CDBCmd cmd;
		cmd.Init(&gserver.m_dbuser);
		cmd.SetQuery(g_buf);
		cmd.Update();
		user->m_preopen_chk = 1;
	}

#endif // EVENT_PREOPEN_GIFT
}

void ConnLevelupPoint(CDescriptor* d, CNetMsg& msg)
{
#ifdef EVENT_OPENBETA_LEVELUP

	unsigned char subtype;
	msg.MoveFirst();
	msg >> subtype;
	int userindex;
	int point;
	msg >> userindex
		>> point;

	CUser* user = gserver.m_userList[d->m_subno - 1].Find(userindex);
	if (user)
	{
		// UPDATE t_users SET a_eventpoint=a_eventpoint+point WHERE a_portal_index=userindex
		strcpy(g_buf, "UPDATE t_users SET a_eventpoint=a_eventpoint+");
		IntCat(g_buf, point, false);
		strcat(g_buf, " WHERE a_portal_index=");
		IntCat(g_buf, userindex, false);
		CDBCmd cmd;
		cmd.Init(&gserver.m_dbuser);
		cmd.SetQuery(g_buf);
		cmd.Update();
	}

#endif // EVENT_OPENBETA_LEVELUP
}

void ConnStashState(CDescriptor* d, CNetMsg& msg)
{
	if (d->m_bLoginServer || d->m_subno < 1)
		return ;

	CNetMsg rmsg;

	int userindex = -1;
	int charindex = -1;
	msg >> userindex
		>> charindex;

	CUser* user = gserver.m_userList[d->m_subno - 1].Find(userindex);
	if (user)
	{

		strcpy(g_buf, "SELECT password('') = a_stash_password as bpasswd , a_stash_sealed FROM t_users WHERE a_portal_index=");
		IntCat(g_buf, userindex);

		CDBCmd cmd;
		cmd.Init(&gserver.m_dbuser);
		cmd.SetQuery(g_buf);

		if (!cmd.Open() || !cmd.MoveFirst())
		{
			StashStateMsg(rmsg, userindex, charindex, -1, -1);
			SEND_Q(rmsg, d);
		}
		else
		{
			char bpasswd = 0;
			char sealed = 0;
			if (!cmd.GetRec("bpasswd", bpasswd) || !cmd.GetRec("a_stash_sealed", sealed))
			{
				StashStateMsg(rmsg, userindex, charindex, -1, -1);
				SEND_Q(rmsg, d);
			}
			else
			{
				if(bpasswd)
					StashStateMsg(rmsg, userindex, charindex, 0, sealed);
				else
					StashStateMsg(rmsg, userindex, charindex, 1, sealed);

				SEND_Q(rmsg, d);
			}
		}
	}
}

void ConnStashCheckPassword(CDescriptor* d, CNetMsg& msg)
{
	CNetMsg rmsg;

	int userindex = -1;
	int charindex = -1;
	CLCString stashPassword(MAX_STASH_PASSWORD_LENGTH + 1);

	msg >> userindex
		>> charindex
		>> stashPassword;

	CUser* user = gserver.m_userList[d->m_subno - 1].Find(userindex);
	if (user)
	{

		sprintf(g_buf, "SELECT (password('%s') = a_stash_password) as bcorrectpasswd, a_stash_sealed "
			"FROM t_users WHERE a_portal_index=%d", (const char*)stashPassword, userindex );

		CDBCmd cmd;
		cmd.Init(&gserver.m_dbuser);
		cmd.SetQuery(g_buf);

		if (!cmd.Open() || !cmd.MoveFirst())
		{
			StashCheckPasswordMsg(rmsg, userindex, charindex, 2);
			SEND_Q(rmsg, d);
		}
		else
		{
			char bcorrectpasswd = 0;
			char sealed = 0;
			if (!cmd.GetRec("bcorrectpasswd", bcorrectpasswd) || !cmd.GetRec("a_stash_sealed", sealed))
			{
				StashCheckPasswordMsg(rmsg, userindex, charindex, 2);
				SEND_Q(rmsg, d);
			}
			else
			{

				if (sealed)
				{
					StashCheckPasswordMsg(rmsg, userindex, charindex, 2);
					SEND_Q(rmsg, d);
				}
				else if (!bcorrectpasswd)
				{
					StashCheckPasswordMsg(rmsg, userindex, charindex, 1);
					SEND_Q(rmsg, d);
				}
				else
				{
					StashCheckPasswordMsg(rmsg, userindex, charindex, 0);
					SEND_Q(rmsg, d);
				}
			}
		}
	}
}

void ConnStashChangePassword(CDescriptor* d, CNetMsg& msg)
{
	CNetMsg rmsg;

	int userindex = -1;
	int charindex = -1;
	CLCString oldpw(MAX_STASH_PASSWORD_LENGTH + 1);
	CLCString newpw(MAX_STASH_PASSWORD_LENGTH + 1);

	msg >> userindex
		>> charindex
		>> oldpw
		>> newpw;

	CUser* user = gserver.m_userList[d->m_subno - 1].Find(userindex);
	if (user)
	{
		strcpy(g_buf, "SELECT a_stash_passwd, a_stash_sealed FROM t_users WHERE a_portal_index=");
		IntCat(g_buf, userindex);

		CDBCmd cmd;
		cmd.Init(&gserver.m_dbuser);
		cmd.SetQuery(g_buf);

		if (!cmd.Open() || !cmd.MoveFirst())
		{
			StashChangePasswordMsg(rmsg, userindex, charindex, newpw, 2);
			SEND_Q(rmsg, d);
		}
		else
		{
			CLCString pw(MAX_STASH_PASSWORD_LENGTH + 1);
			char sealed = 0;
			if (!cmd.GetRec("a_stash_passwd", pw) || !cmd.GetRec("a_stash_sealed", sealed))
			{
				StashChangePasswordMsg(rmsg, userindex, charindex, newpw, 2);
				SEND_Q(rmsg, d);
			}
			else
			{
				if (sealed)
				{
					StashChangePasswordMsg(rmsg, userindex, charindex, newpw, 2);
					SEND_Q(rmsg, d);
				}
				else if (strcmp(pw, oldpw))
				{
					StashChangePasswordMsg(rmsg, userindex, charindex, newpw, 1);
					SEND_Q(rmsg, d);
				}
				else
				{
					strcpy(g_buf, "UPDATE t_users SET a_stash_passwd='");
					strcat(g_buf, newpw);
					strcat(g_buf, "' WHERE a_portal_index=");
					IntCat(g_buf, userindex);
					strcat(g_buf, " AND a_stash_sealed=");
					IntCat(g_buf, sealed);
					strcat(g_buf, " AND a_stash_passwd='");
					strcat(g_buf, oldpw);
					strcat(g_buf, "'");

					cmd.SetQuery(g_buf);
					if (!cmd.Update())
					{
						StashChangePasswordMsg(rmsg, userindex, charindex, newpw, 1);
						SEND_Q(rmsg, d);
					}
					else
					{
						StashChangePasswordMsg(rmsg, userindex, charindex, newpw, 0);
						SEND_Q(rmsg, d);
					}
				}
			}
		}
	}
}

void ConnStashSeal(CDescriptor* d, CNetMsg& msg)
{
	CNetMsg rmsg;

	int userindex = -1;
	int charindex = -1;

	msg >> userindex
		>> charindex;

	CUser* user = gserver.m_userList[d->m_subno - 1].Find(userindex);
	if (user)
	{
		strcpy(g_buf, "SELECT a_stash_sealed FROM t_users WHERE a_portal_index=");
		IntCat(g_buf, userindex);

		CDBCmd cmd;
		cmd.Init(&gserver.m_dbuser);
		cmd.SetQuery(g_buf);

		if (!cmd.Open() || !cmd.MoveFirst())
		{
			StashSealMsg(rmsg, userindex, charindex, 2);
			SEND_Q(rmsg, d);
		}
		else
		{
			char sealed = 0;
			if (!cmd.GetRec("a_stash_sealed", sealed))
			{
				StashSealMsg(rmsg, userindex, charindex, 2);
				SEND_Q(rmsg, d);
			}
			else
			{
				if (sealed)
				{
					StashSealMsg(rmsg, userindex, charindex, 1);
					SEND_Q(rmsg, d);
				}
				else
				{
					strcpy(g_buf, "UPDATE t_users SET a_stash_sealed=1 WHERE a_portal_index=");
					IntCat(g_buf, userindex);
					strcat(g_buf, " AND a_stash_sealed=");
					IntCat(g_buf, sealed);

					cmd.SetQuery(g_buf);
					if (!cmd.Update())
					{
						StashSealMsg(rmsg, userindex, charindex, 2);
						SEND_Q(rmsg, d);
					}
					else
					{
						StashSealMsg(rmsg, userindex, charindex, 0);
						SEND_Q(rmsg, d);
					}
				}
			}
		}
	}
}

void ConnStashDeletePassword(CDescriptor* d, CNetMsg& msg)
{
	// 주민번호를 얻어와서 idnum과 correct면 삭제하고 0, 아니면 1을 보낸다
	// t_user a_stash_passwd
	CNetMsg rmsg;

	int userindex = -1;
	int charindex = -1;

	CLCString stashPassword(MAX_STASH_PASSWORD_LENGTH + 1);
	
	msg >> userindex
		>> charindex
		>> stashPassword;

	CUser* user = gserver.m_userList[d->m_subno - 1].Find(userindex);
	if (user)
	{

		CDBCmd userCmd;
		userCmd.Init(&gserver.m_dbuser);

#ifdef LC_TWN
		sprintf(g_buf, "update t_users set a_stash_password='' where a_stash_passwd = password('%s') and a_portal_index = %d",  (const char*) stashPassword, userindex);
		userCmd.SetQuery(g_buf);

#else
		CLCString username(MAX_ID_NAME_LENGTH + 1);
		CLCString s_idnum(50);

		CDBCmd authCmd;
		authCmd.Init(&gserver.m_dbauth);

		sprintf(g_buf, "select a_idname from t_users where a_portal_index = %d", userindex);
		userCmd.SetQuery(g_buf);
		if( !userCmd.Open() || !userCmd.MoveFirst() )
		{
			// vaild 2로 send
			StashDeletePasswordMsg(rmsg, userindex, charindex, 2);
			SEND_Q(rmsg, d);
			return;
		}

		if( !userCmd.GetRec("a_idname", username) )
		{
			StashDeletePasswordMsg(rmsg, userindex, charindex, 2);
			SEND_Q(rmsg, d);
			return;
		}

#ifdef LC_TLD
		sprintf(g_buf, "select a_cid from bg_user where a_idname = '%s'", (const char*)username);
#else

		sprintf(g_buf, "select jumin from bg_user where user_id = '%s'", (const char*)username);
#endif // LC_TLD
		authCmd.SetQuery(g_buf);

		if( !authCmd.Open() || !authCmd.MoveFirst() )
		{
			StashDeletePasswordMsg(rmsg, userindex, charindex, 2);
			SEND_Q(rmsg, d);
			return;
		}

#ifdef LC_TLD
		if( !authCmd.GetRec("a_cid", s_idnum) )
#else
		if( !authCmd.GetRec("jumin", s_idnum) )
#endif // LC_TLD
		{
			StashDeletePasswordMsg(rmsg, userindex, charindex, 2);
			SEND_Q(rmsg, d);
			return;
		}
#ifdef LC_TLD

#else
		strcpy(g_buf, s_idnum);
		char * token;
		token = strtok(g_buf, "-");
		token = strtok(NULL, "-");
		s_idnum = token;
#endif // LC_TLD

		if( s_idnum.Length() < 1)
		{
			StashDeletePasswordMsg(rmsg, userindex, charindex, 2);
			SEND_Q(rmsg, d);
			return;
		}

		if( strcmp(s_idnum, stashPassword) )
		{
			StashDeletePasswordMsg(rmsg, userindex, charindex, 1);
			SEND_Q(rmsg, d);
			return;

		}

		sprintf(g_buf, "update t_users set a_stash_password = '' where a_portal_index = %d", userindex);
		userCmd.SetQuery(g_buf);
#endif

		if( !userCmd.Update() )
		{
			StashDeletePasswordMsg(rmsg, userindex, charindex, 2);
			SEND_Q(rmsg, d);
			return;
		}

		StashDeletePasswordMsg(rmsg, userindex, charindex, 0);
		SEND_Q(rmsg, d);

		return;
	}		
	StashDeletePasswordMsg(rmsg, userindex, charindex, 2);
	SEND_Q(rmsg, d);
}

void ConnStashNewPassword(CDescriptor* d, CNetMsg& msg)
{
	CNetMsg rmsg;

	int userindex = -1;
	int charindex = -1;
	CLCString oldpw(MAX_STASH_PASSWORD_LENGTH + 1);
	CLCString newpw(MAX_STASH_PASSWORD_LENGTH + 1);

	msg >> userindex
		>> charindex
		>> oldpw;

	if( msg.CanRead(1) )
	{
		msg >> newpw;
	}

	CUser* user = gserver.m_userList[d->m_subno - 1].Find(userindex);
	if (user)
	{
		CDBCmd cmd;

		cmd.Init(&gserver.m_dbuser);

		// 인스리아 a_stash_passwd 필드를 비밀번호 푸는 암호로 사용
#ifdef LC_TWN
		sprintf(g_buf, "UPDATE t_users SET a_stash_password=password('%s'), a_stash_passwd=password('%s') WHERE a_portal_index=%d", (const char*) oldpw, (const char*) newpw, userindex);
#else
		sprintf(g_buf, "UPDATE t_users SET a_stash_password=password('%s') WHERE a_portal_index=%d", (const char*)oldpw, userindex);
#endif


		cmd.SetQuery(g_buf);
		if (!cmd.Update())
		{
			StashSetNewPasswordMsg(rmsg, userindex, charindex, 1);
			SEND_Q(rmsg, d);
		}
		else
		{
			if(oldpw == '\0')
			{
				StashSetNewPasswordMsg(rmsg, userindex, charindex, 2);
				SEND_Q(rmsg, d);
			}
			else
			{
				StashSetNewPasswordMsg(rmsg, userindex, charindex, 0);
				SEND_Q(rmsg, d);
			}
		}
		
	}
}

void ConnEvent2pan4panLetterReq(CDescriptor* d, CNetMsg& msg)
{
	CNetMsg rmsg;

	int userindex = -1;
	int charindex = -1;
	int wordtype = -1;

	msg >> userindex
		>> charindex
		>> wordtype;

	char* fname = "";
	switch (wordtype)
	{
	case MSG_EVENT_2PAN4PAN_WORD_LASTCHAOS:		fname = "a_lastchaos";		break;
	case MSG_EVENT_2PAN4PAN_WORD_2PAN4PAN:		fname = "a_2pan4pan"; 		break;
	case MSG_EVENT_2PAN4PAN_WORD_SSAWAR:		fname = "a_ssawar";   		break;
	case MSG_EVENT_2PAN4PAN_WORD_LAGHAIM:		fname = "a_laghaim";  		break;
	case MSG_EVENT_2PAN4PAN_WORD_NORABA:		fname = "a_noraba";   		break;
	}

	sprintf(g_buf,
		"UPDATE t_event_2pan4pan SET %s=1 WHERE a_user_index=%d AND %s=0"
		, fname
		, userindex
		, fname);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);
	cmd.SetQuery(g_buf);

	if (!cmd.Update())
	{
		Event2pan4panLetterRepMsg(rmsg, userindex, charindex, MSG_EVENT_2PAN4PAN_WORD_ALREADY);
		SEND_Q(rmsg, d);
		return ;
	}

	if (cmd.m_nrecords < 1)
	{
		sprintf(g_buf,
			"INSERT INTO t_event_2pan4pan"
			" (a_user_index, %s)"
			" VALUES"
			" (%d, 1)"
			, fname
			, userindex);

		cmd.SetQuery(g_buf);
		if (!cmd.Update())
		{
			Event2pan4panLetterRepMsg(rmsg, userindex, charindex, MSG_EVENT_2PAN4PAN_WORD_ALREADY);
			SEND_Q(rmsg, d);
			return ;
		}
	}

	Event2pan4panLetterRepMsg(rmsg, userindex, charindex, (MSG_EVENT_2PAN4PAN_WORD_TYPE)wordtype);
	SEND_Q(rmsg, d);
}

void ConnEvent2pan4panLetterRollbackReq(CDescriptor* d, CNetMsg& msg)
{
	CNetMsg rmsg;

	int userindex = -1;
	int charindex = -1;
	int wordtype = -1;

	msg >> userindex
		>> charindex
		>> wordtype;

	char* fname = "";
	switch (wordtype)
	{
	case MSG_EVENT_2PAN4PAN_WORD_LASTCHAOS:		fname = "a_lastchaos";		break;
	case MSG_EVENT_2PAN4PAN_WORD_2PAN4PAN:		fname = "a_2pan4pan"; 		break;
	case MSG_EVENT_2PAN4PAN_WORD_SSAWAR:		fname = "a_ssawar";   		break;
	case MSG_EVENT_2PAN4PAN_WORD_LAGHAIM:		fname = "a_laghaim";  		break;
	case MSG_EVENT_2PAN4PAN_WORD_NORABA:		fname = "a_noraba";   		break;
	}

	sprintf(g_buf,
		"UPDATE t_event_2pan4pan SET %s=0 WHERE a_user_index=%d AND %s=1"
		, fname
		, userindex
		, fname);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);
	cmd.SetQuery(g_buf);
	cmd.Update();

	Event2pan4panLetterRollbackRepMsg(rmsg, userindex, charindex, (MSG_EVENT_2PAN4PAN_WORD_TYPE)wordtype);
	SEND_Q(rmsg, d);
}

void ConnEvent2pan4panBoxReq(CDescriptor* d, CNetMsg& msg)
{
#ifdef EVENT_2PAN4PAN_BOX
	CNetMsg rmsg;

	int userindex;
	int charindex;

	msg >> userindex
		>> charindex;

	sprintf(g_buf,
		"SELECT egoods_index, egoods_item_no, egoods_cnt"
		" FROM bg_game_event_goods"
		" WHERE egoods_user_code=%d AND egoods_site_code='LC' AND egoods_enable=1 AND egoods_event_name='BOX' LIMIT 20",
		userindex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbevent);
	cmd.SetQuery(g_buf);
	if (cmd.Open() && cmd.MoveFirst())
	{
		int goodsindex[20];
		int itemindex[20];
		LONGLONG itemcount[20];
		int i = 0;
		do
		{
			cmd.GetRec("egoods_index",		goodsindex[i]);
			cmd.GetRec("egoods_item_no",	itemindex[i]);
			cmd.GetRec("egoods_cnt",		itemcount[i]);
			i++;
		} while (cmd.MoveNext());
		Event2pan4panBoxRepMsg(rmsg, userindex, charindex, i, goodsindex, itemindex, itemcount);
		SEND_Q(rmsg, d);
	}
	else
	{
		Event2pan4panBoxRepMsg(rmsg, userindex, charindex, 0, NULL, NULL, NULL);
		SEND_Q(rmsg, d);
	}
#endif // #ifdef EVENT_2PAN4PAN_BOX
}

void ConnEvent2pan4panBoxCommit(CDescriptor* d, CNetMsg& msg)
{
#ifdef EVENT_2PAN4PAN_BOX
	CNetMsg rmsg;

	int userindex;
	int charindex;
	int count;
	int goodsindex[20] = {0, };

	msg >> userindex
		>> charindex
		>> count;

	if (count < 1)
	{
		Event2pan4panBoxCommitMsg(rmsg, userindex, charindex, 0, NULL);
		SEND_Q(rmsg, d);
		return ;
	}

	strcpy(g_buf, "UPDATE bg_game_event_goods SET egoods_enable=0, egoods_use_date=NOW() WHERE egoods_index IN (");

	int i;
	for (i =0; i < count; i++)
	{
		msg >> goodsindex[i];
		if (i > 0)
			strcpy(g_buf2, ",%d");
		else
			strcpy(g_buf2, "%d");
		sprintf(g_buf3, g_buf2, goodsindex[i]);
		strcat(g_buf, g_buf3);
	}

	strcat(g_buf, ")");

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbevent);
	cmd.SetQuery(g_buf);

	if (cmd.Update())
	{
		Event2pan4panBoxCommitMsg(rmsg, userindex, charindex, count, goodsindex);
		SEND_Q(rmsg, d);
	}
	else
	{
		Event2pan4panBoxCommitMsg(rmsg, userindex, charindex, 0, NULL);
		SEND_Q(rmsg, d);
	}
#endif // #ifdef EVENT_2PAN4PAN_BOX
}

void ConnEvent2pan4panGoodsReq(CDescriptor* d, CNetMsg& msg)
{
#ifdef EVENT_2PAN4PAN_GOODS
	CNetMsg rmsg;

	int userindex;
	int charindex;

	msg >> userindex
		>> charindex;

	sprintf(g_buf,
		"SELECT egoods_index, egoods_item_no, egoods_cnt"
		" FROM bg_game_event_goods"
		" WHERE egoods_user_code=%d AND egoods_site_code='LC' AND egoods_enable=1 AND egoods_event_name='CARD' LIMIT 20",
		userindex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbevent);
	cmd.SetQuery(g_buf);
	if (cmd.Open() && cmd.MoveFirst())
	{
		int goodsindex[20];
		int itemindex[20];
		LONGLONG itemcount[20];
		int i = 0;
		do
		{
			cmd.GetRec("egoods_index",		goodsindex[i]);
			cmd.GetRec("egoods_item_no",	itemindex[i]);
			cmd.GetRec("egoods_cnt",		itemcount[i]);
			i++;
		} while (cmd.MoveNext());
		Event2pan4panGoodsRepMsg(rmsg, userindex, charindex, i, goodsindex, itemindex, itemcount);
		SEND_Q(rmsg, d);
	}
	else
	{
		Event2pan4panGoodsRepMsg(rmsg, userindex, charindex, 0, NULL, NULL, NULL);
		SEND_Q(rmsg, d);
	}
#endif // #ifdef EVENT_2PAN4PAN_GOODS
}

void ConnEvent2pan4panGoodsCommit(CDescriptor* d, CNetMsg& msg)
{
#ifdef EVENT_2PAN4PAN_GOODS
	CNetMsg rmsg;

	int userindex;
	int charindex;
	int count;
	int goodsindex[20] = {0, };

	msg >> userindex
		>> charindex
		>> count;

	if (count < 1)
	{
		Event2pan4panGoodsCommitMsg(rmsg, userindex, charindex, 0, NULL);
		SEND_Q(rmsg, d);
		return ;
	}

	strcpy(g_buf, "UPDATE bg_game_event_goods SET egoods_enable=0, egoods_use_date=NOW() WHERE egoods_index IN (");

	int i;
	for (i =0; i < count; i++)
	{
		msg >> goodsindex[i];
		if (i > 0)
			strcpy(g_buf2, ",%d");
		else
			strcpy(g_buf2, "%d");
		sprintf(g_buf3, g_buf2, goodsindex[i]);
		strcat(g_buf, g_buf3);
	}

	strcat(g_buf, ")");

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbevent);
	cmd.SetQuery(g_buf);

	if (cmd.Update())
	{
		Event2pan4panGoodsCommitMsg(rmsg, userindex, charindex, count, goodsindex);
		SEND_Q(rmsg, d);
	}
	else
	{
		Event2pan4panGoodsCommitMsg(rmsg, userindex, charindex, 0, NULL);
		SEND_Q(rmsg, d);
	}
#endif // #ifdef EVENT_2PAN4PAN_GOODS
}

void ConnEvent2pan4panMoneyReq(CDescriptor* d, CNetMsg& msg)
{
#ifdef EVENT_2PAN4PAN_MONEY
	LONGLONG money[4] = {100000, 50000000, 20000, 10000000};		// 맞고 10만, 포커 5000만, 맞고 2만, 포커 1000만
	CNetMsg rmsg;

	int userindex;
	int charindex;
	int itemindex;
	int typeindex = 0;
	int gametype = 0;

	msg >> userindex
		>> charindex
		>> itemindex;

	switch (itemindex)
	{
	case 1051:		typeindex = 0;		gametype = 1;	break;
	case 1052:		typeindex = 1;		gametype = 2;	break;
	case 1055:		typeindex = 2;		gametype = 1;	break;
	case 1054:		typeindex = 3;		gametype = 2;	break;
	default:
		Event2pan4panMoneyRepMsg(rmsg, userindex, charindex, 0);
		SEND_Q(rmsg, d);
		return ;
	}

#ifdef CIRCLE_WINDOWS
	sprintf(g_buf, "INSERT INTO bg_game_event (user_code,game_money,join_game_type,bg_game_type,update_date) VALUES (%d, %I64d, 'LC', %d, NOW())", userindex, money[typeindex], gametype);
#else
	sprintf(g_buf, "INSERT INTO bg_game_event (user_code,game_money,join_game_type,bg_game_type,update_date) VALUES (%d, %lld, 'LC', %d, NOW())", userindex, money[typeindex], gametype);
#endif

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbevent);
	cmd.SetQuery(g_buf);

	if (cmd.Update())
	{
		Event2pan4panMoneyRepMsg(rmsg, userindex, charindex, itemindex);
		SEND_Q(rmsg, d);
	}
	else
	{
		Event2pan4panMoneyRepMsg(rmsg, userindex, charindex, 0);
		SEND_Q(rmsg, d);
	}
#endif // #ifdef EVENT_2PAN4PAN_MONEY
}

void ConnRankerReq(CDescriptor* d, CNetMsg& msg)
{
	int charindex;

	msg	>> charindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	sprintf(g_buf, "select a_rank from t_ranking where a_server = %d and a_rank <= 10 and a_charindex = %d and a_date > (now() - interval 1 day ) ", gserver.m_serverno, charindex);
	
	cmd.SetQuery(g_buf);
	if( !cmd.Open() )
	{
		//DB error
		return;
	}

	if( cmd.m_nrecords > 0 )
	{
		// 랭커라는 메세지
		CNetMsg rmsg;
		RankerRepMsg(rmsg, charindex);
		SEND_Q(rmsg, d);
	}
}
#ifdef NEW_SERIAL_PACKAGE_EVENT
void ConnCouponConfirm_new( CDescriptor* d, CNetMsg& msg )
//void ConnCouponConfirm_new( CDescriptor* d, int charIndex , const char*  serial )
{
	const int SERIAL_MAX = 25;
	const int SECTION_MAX_SIZE = 30;

	int charIndex;
	CLCString serial(SERIAL_MAX);


	msg >> charIndex
		>> serial;

	int promotionIndex;
	CLCString sectionString(SECTION_MAX_SIZE);
	int sectionType;
	int useState;
	unsigned int startDate;
	unsigned int endDate;
	unsigned int useDate;

	if( !serial || !charIndex )
		return;
	
	CDBCmd cmd;
	CLCString sql( 1024 );
	cmd.Init( &gserver.m_dbuser );
	//SELECT a_index, a_use, a_section_type, a_section_string, UNIX_TIMESTAMP(a_start_date) AS start_date, UNIX_TIMESTAMP(a_end_date) as end_date 
	//FROM t_new_promotion WHERE a_enable = 1 AND a_key_string = 'Serial_Number' 
	//AND a_use_state = 0 AND UNIX_TIMESTAMP( a_start_date ) <= UNIX_TIMESTAMP(now()) AND UNIX_TIMESTAMP( a_end_date ) >= UNIX_TIMESTAMP(now())"  )
	sql.Format( "SELECT a_index, a_use_state, a_section_type, a_section_string, UNIX_TIMESTAMP(a_start_date) AS start_date, " 
				"UNIX_TIMESTAMP(a_end_date) as end_date, UNIX_TIMESTAMP(a_use_date) as use_date "
				"FROM t_new_promotion WHERE a_enable = 1 AND a_key_string = '%s' "
				"AND UNIX_TIMESTAMP( a_start_date ) <= UNIX_TIMESTAMP(now()) AND "
				"UNIX_TIMESTAMP( a_end_date ) >= UNIX_TIMESTAMP(now())", (const char*)serial );

	cmd.SetQuery( sql );

	CNetMsg rmsg;
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		CouponConfirmMsg_new( rmsg, charIndex, MSG_EVENT_ERROR_NOT_COUPON ); // 쿠폰 없음 에러 메세지
		SEND_Q( rmsg, d );
		return;
	}

	cmd.GetRec( "a_index", promotionIndex );
	cmd.GetRec( "a_use_state", useState );
	cmd.GetRec( "a_section_type", sectionType );
	cmd.GetRec( "a_section_string", sectionString );
	cmd.GetRec( "start_date", startDate );
	cmd.GetRec( "end_date", endDate );
	cmd.GetRec( "use_date", useDate );

	if( useDate != 0 )
	{
	
		CouponConfirmMsg_new( rmsg, charIndex, MSG_EVENT_ERROR_ALREADY_USE_COUNPON ); // 쿠폰 없음 에러 메세지
		SEND_Q( rmsg, d );
		return;
	}



	// get Item in section type
	CDBCmd giftItemCmd;
	giftItemCmd.Init( &gserver.m_dbuser );
	// SELECT a_item_idx, a_item_count, a_item_plus, a_item_flag, a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4
	// FROM t_promotion_item WHERE a_section_type = sectionType;
//	sql.Format( "SELECT a_item_idx, a_item_count, a_item_plus, a_item_flag, a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4"
	sql.Format( "SELECT a_item_idx, a_item_count, a_item_plus, a_item_flag "
		"FROM t_new_promoitem WHERE a_section_type = %d", sectionType );
	giftItemCmd.SetQuery( sql );
	if( !giftItemCmd.Open() || !giftItemCmd.MoveFirst() )
	{
		CouponConfirmMsg_new( rmsg, charIndex, MSG_EVENT_ERROR_NOT_COUPON ); // Item이 없음
		SEND_Q( rmsg, d) ;
		return;
	}

	const int GIFT_ITEM_COUNT = giftItemCmd.m_nrecords;
	PromotionItem* pPromoItem = new PromotionItem[ GIFT_ITEM_COUNT ];
	int i = 0;
	do
	{
		giftItemCmd.GetRec( "a_item_idx", pPromoItem[i].m_itemIndex );
		giftItemCmd.GetRec( "a_item_count", pPromoItem[i].m_itemCount );
		giftItemCmd.GetRec( "a_item_plus", pPromoItem[i].m_itemPlus );
		giftItemCmd.GetRec( "a_item_flag", pPromoItem[i].m_itemFlag );
//		giftItemCmd.GetRec( "a_item_option0", pPromoItem[i].m_itemOption0 );
//		giftItemCmd.GetRec( "a_item_option1", pPromoItem[i].m_itemOption1 );
//		giftItemCmd.GetRec( "a_item_option2", pPromoItem[i].m_itemOption2 );
//		giftItemCmd.GetRec( "a_item_option3", pPromoItem[i].m_itemOption3 );
//		giftItemCmd.GetRec( "a_item_option4", pPromoItem[i].m_itemOption4 );
		i++;
	}while( giftItemCmd.MoveNext() );

	CouponConfirmMsg_new( rmsg, charIndex, MSG_EVENT_ERROR_COUPON_SUCCESS, promotionIndex, sectionType, GIFT_ITEM_COUNT, pPromoItem );
	SEND_Q( rmsg, d );
	delete [] pPromoItem;

}


void ConnCouponUse_new(CDescriptor* d, CNetMsg& msg)
{
	int charindex;
	int promotionIndex, promotionType, GiftItemCount;

	msg >> charindex
		>> promotionIndex
		>> promotionType
		>> GiftItemCount;

	PromotionItem* pPromoItem = new PromotionItem[ GiftItemCount ];
	int RoopCount;

	for( RoopCount = 0; RoopCount < GiftItemCount; RoopCount++ )
	{
		msg >> pPromoItem[RoopCount].m_itemIndex
			>> pPromoItem[RoopCount].m_itemCount
			>> pPromoItem[RoopCount].m_itemPlus
			>> pPromoItem[RoopCount].m_itemFlag;
//			>> pPromoItem[RoopCount].m_itemOption0
//			>> pPromoItem[RoopCount].m_itemOption1
//			>> pPromoItem[RoopCount].m_itemOption2
//			>> pPromoItem[RoopCount].m_itemOption3
//			>> pPromoItem[RoopCount].m_itemOption4;
	}

	CNetMsg rmsg;

	if( promotionIndex == -1 )
	{
		CouponUseMsg_new(rmsg, charindex, MSG_EVENT_ERROR_ALREADY_USE_COUNPON, promotionType, GiftItemCount, pPromoItem );
		SEND_Q(rmsg, d);

		GAMELOG << init("COUPON_USE_INDEX_ERROR")
				<< charindex << delim
				<< promotionIndex << end;
		return;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);
	CLCString sql(1024);
	//UPDATE t_new_promotion set a_use =  1, a_use_date = now() WHERE a_index = 'promotionIndex' AND a_section_type = 'promotionType';
	sql.Format( "UPDATE t_new_promotion SET a_use_state = 1, a_use_date = now() WHERE a_index  ='%d' AND a_section_type = '%d'", promotionIndex, promotionType );
	cmd.SetQuery( sql );
	if( !cmd.Update() )
	{
		CouponUseMsg_new( rmsg, charindex, MSG_EVENT_ERROR_ALREADY_USE_COUNPON, promotionType, GiftItemCount, pPromoItem );
		SEND_Q( rmsg, d );
		GAMELOG << init("COUPON_USE_UPDATE_ERROR")
			<< charindex << delim
			<< promotionIndex << end;
		return;
	}

	GAMELOG << init("COUPON_USE")
		<< charindex << delim
		<< promotionIndex << delim
		<< promotionType << end;

	CouponUseMsg_new( rmsg, charindex, MSG_EVENT_ERROR_COUPON_SUCCESS, promotionType );
	SEND_Q( rmsg, d );
}

#endif //NEW_SERIAL_PACKAGE_EVENT

#ifdef EVENT_PACKAGE_ITEM
void ConnCouponConfirm(CDescriptor* d, CNetMsg& msg)
{
	const int KIND_STRIMG_SIZE = 30;
	int charindex;
	CLCString coupon(21);
	CLCString kind(KIND_STRIMG_SIZE);
	int type = -1;
	int useDate;
#ifdef LC_MAL
	int KeyIndex = 0;
#endif

	msg >> charindex
		>> coupon;

	if( !coupon || !charindex) 
		return;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	sprintf(g_buf, "select a_index, unix_timestamp(a_use_date) as buse, a_kind from t_key where a_enable = 1 and a_strkey = '%s' ", (const char*) coupon);
	
	cmd.SetQuery(g_buf);

	CNetMsg rmsg;
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		// 쿠폰 없음 에러 메세지
#ifdef NEW_SERIAL_PACKAGE_EVENT
		ConnCouponConfirm_new( d, charindex, coupon );
#else // NEW_SERIAL_PACKAGE_EVENT
		CouponConfirmMsg(rmsg, charindex, MSG_EVENT_ERROR_NOT_COUPON);
		SEND_Q(rmsg, d);		
#endif
		return;
	}

	cmd.GetRec("buse", useDate);
	cmd.GetRec("a_kind", kind);
//	cmd.GetRec("a_type", type);

#ifdef LC_MAL
	cmd.GetRec( "a_index", KeyIndex );
#endif

	if( useDate > 0)
	{
		// 이미 사용한 쿠폰
		CouponConfirmMsg(rmsg, charindex, MSG_EVENT_ERROR_ALREADY_USE_COUNPON);
		SEND_Q(rmsg, d);
		return;
	}

#ifdef LC_MAL
	if( strcmp(kind, "Promo Package 1 October 2006") == 0 ||
		strcmp(kind, "Promo Package 2 October 2006") == 0 ||
		strcmp(kind, "Promo Package 3 October 2006") == 0 ||
		strcmp(kind, "Promo Package 4 October 2006") == 0 ||
		strcmp(kind, "Promo Package 5 October 2006") == 0 ||
		strcmp(kind, "Promo Package 6 October 2006") == 0)
	{
		if( ( KeyIndex >= 1 ) && ( KeyIndex <= 1200 ) )
		{
			type = 1;
		}
		else if( ( KeyIndex >= 1201 ) && ( KeyIndex <= 2100 ) )
		{
			type = 2;
		}
		else if( ( KeyIndex >= 2101 ) && ( KeyIndex < 2550 ) )
		{
			type = 3;
		}
		else if( ( KeyIndex >= 2551 ) && ( KeyIndex <= 2850 ) )
		{
			type = 4;
		}
		else if( ( KeyIndex >= 2851 ) && ( KeyIndex <= 3000 ) )
		{
			type = 5;
		}
	}	
	else if ( strcmp( kind, "January Starter Pack 1" ) == 0 )
	{
		type = 6;
	}
	else if ( strcmp( kind, "January Starter Pack 2" ) == 0 )
	{
		type = 7;
	}
	else if ( strcmp( kind, "Lucky Draw Promo Pack 1" ) == 0 )
	{
		type = 8;
	}
	else if ( strcmp( kind, "Lucky Draw Promo Pack 2" ) == 0 )
	{
		type = 9;
	}
	else if ( strcmp( kind, "CNY Promo Pack 1" ) == 0 )
	{
		type = 10;
	}
	else if ( strcmp( kind, "CNY Promo Pack 2" ) == 0 )
	{
		type = 11;
	}
	else if ( strcmp( kind, "Titan Promo A" ) == 0 )
	{
		type = 12;
	}
	else if ( strcmp( kind, "Knight Promo B" ) == 0 )
	{
		type = 13; 
	}
	else if ( strcmp( kind, "Healer Promo C" ) == 0 )
	{
		type = 14;
	}
	else if ( strcmp( kind, "Mage Promo D" ) == 0 )
	{
		type = 15;
	}
	else if ( strcmp( kind, "Rogue Promo E" ) == 0 )
	{
		type = 16;
	}
	else if ( strcmp( kind, "Summoner Promo F" ) == 0 )
	{
		type = 17;
	}
	else if ( strcmp( kind, "Heavens Pack" ) == 0 )
	{
		type = 18;
	}
	else if ( strcmp( kind, "Streiana Pack A" ) == 0 )
	{
		type = 19;
	}	

#elif defined ( LC_HBK )
	if( strcmp( kind, "Newbie package Feb 2007" ) == 0 )
	{
		type = 1;
	}
	else if( strcmp( kind, "Special package March 2007" ) == 0 )
	{
		type = 2;
	}
	else if( strcmp( kind, "Newbie package April 2007" ) == 0 )
	{
		type = 3;
	}
	else if( strcmp( kind, "Exclusive package July 2007" ) == 0 )
	{
		type = 4;
	}
	else if( strcmp( kind, "Special package October 2007") == 0)
	{
		type = 5;
	}
// << kjban 080121
	else if( strcmp( kind, "Anniversary Pack Feb 2008") == 0)
	{
		type = 6;
	}
// >>	
#else  // 말레이시아, 홍콩을 제외한 나라
	if( strcmp(kind, "open1") == 0)
	{
		type = 1;
	}
	else if( strcmp(kind, "event2") == 0)
	{
		type = 2;
	}
	else if( strcmp(kind, "event3") == 0)
	{
		type = 3;
	}
#endif
#if !defined(LC_GER)
	else
	{
		// 쿠폰 없음 에러 메세지
		CouponConfirmMsg(rmsg, charindex, MSG_EVENT_ERROR_NOT_COUPON);
		SEND_Q(rmsg, d);
		return;
	}
#endif

	// 맞다 메세지
	CouponConfirmMsg(rmsg, charindex, MSG_EVENT_ERROR_COUPON_SUCCESS, atoi(cmd.GetRec("a_index")), type);
	SEND_Q(rmsg, d);

}

void ConnCouponUse(CDescriptor* d, CNetMsg& msg)
{
	int charindex;
	int cIndex, type;

#ifdef EVENT_PACKAGE_ITEM_DBLOG
	CLCString idname(MAX_ID_NAME_LENGTH + 1);
	CLCString nick(MAX_CHAR_NAME_LENGTH + 1);
	int serverno;
#endif

#ifdef LC_HBK
	int randidx1, randcnt1, randidx2, randcnt2;
#endif // LC_HBK

	msg >> charindex
		>> cIndex
#ifdef EVENT_PACKAGE_ITEM_DBLOG
		>> type
		>> idname
		>> nick
		>> serverno;
#ifdef LC_HBK
	msg >> randidx1
		>> randcnt1
		>> randidx2
		>> randcnt2;
#endif // LC_HBK
#else
		>> type;
#ifdef LC_HBK
	msg >> randidx1
		>> randcnt1
		>> randidx2
		>> randcnt2;
#endif // LC_HBK
#endif

	CNetMsg rmsg;

	if( cIndex == -1 )
	{
#ifdef LC_HBK
		CouponUseMsg(rmsg, charindex, MSG_EVENT_ERROR_ALREADY_USE_COUNPON, randidx1, randcnt1, randidx2, randcnt2, type);
#else
		CouponUseMsg(rmsg, charindex, MSG_EVENT_ERROR_ALREADY_USE_COUNPON, type);
#endif // LC_HBK
		SEND_Q(rmsg, d);

		GAMELOG << init("COUPON_USE_INDEX_ERROR")
				<< charindex << delim
				<< cIndex << end;
		return;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	sprintf(g_buf, "update t_key set a_use_date = now() where a_index = %d and a_use_date = 0 ", cIndex);
	cmd.SetQuery(g_buf);

	if( !cmd.Update() )
	{
#ifdef LC_HBK
		CouponUseMsg(rmsg, charindex, MSG_EVENT_ERROR_ALREADY_USE_COUNPON, randidx1, randcnt1, randidx2, randcnt2, type);
#else
		CouponUseMsg(rmsg, charindex, MSG_EVENT_ERROR_ALREADY_USE_COUNPON, type);
#endif // LC_HBK
		SEND_Q(rmsg, d);

		GAMELOG << init("COUPON_USE_UPDATE_ERROR")
				<< charindex << delim
				<< cIndex << end;
		return;
	}
#ifdef LC_HBK
	CouponUseMsg(rmsg, charindex, MSG_EVENT_ERROR_COUPON_SUCCESS, randidx1, randcnt1, randidx2, randcnt2, type);
#else // LC_HBK
	CouponUseMsg(rmsg, charindex, MSG_EVENT_ERROR_COUPON_SUCCESS, type);
#endif // LC_HBK
	SEND_Q(rmsg, d);
	
	CLCString coupon(21);
	CLCString kind(30);

	sprintf(g_buf, "select a_strkey, a_kind from t_key where a_index = %d ", cIndex);
	cmd.SetQuery(g_buf);
	if( cmd.Open() && cmd.MoveFirst() )
	{
		cmd.GetRec("a_strkey", coupon);
		cmd.GetRec("a_kind", kind);

		GAMELOG << init("COUPON_USE")
				<< charindex << delim
				<< cIndex << delim
				<< coupon << delim
				<< kind << end;
	}

#ifdef EVENT_PACKAGE_ITEM_DBLOG
// 디비 테이블에 로그 남기기
// t_key_log
// a_id : a_nick : a_serial : a_serverno : a_usetime
// 유저아이디 : 캐릭터 이름 : 시리얼넘버 : 서버군번호 : 사용시간 
	sprintf(g_buf, "insert into t_key_log values('%s', '%s', '%s', %d, now())", 
		(const char *)idname, (const char *)nick, (const char *)coupon, serverno);
	cmd.SetQuery(g_buf);
	cmd.Update();
#endif
}
#endif // EVENT_PACKAGE_ITEM

#ifdef EVENT_PARTNER_CODENEO
void ConnEventPartnerCodeNEOReq( CDescriptor* d, CNetMsg& msg )
{
	int userindex, ischaracter;
	int dbuserindex;

	msg >> userindex
		>> ischaracter;
	
	GAMELOG << init("EVENT_PARTNER_CODENEO")
			<< userindex << delim
			<< ischaracter << end;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	sprintf(g_buf, "SELECT a_user_idx, a_is_character FROM t_character_exist WHERE a_user_idx = '%d'", userindex );
	cmd.SetQuery(g_buf);

	CNetMsg rmsg;
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		sprintf(g_buf, "INSERT INTO t_character_exist (a_user_idx, a_is_character) VALUES (%d, %d)", userindex, ischaracter );
		cmd.SetQuery(g_buf);
		cmd.Update();
		return;
	}
	else
	{
		cmd.GetRec("a_user_idx", dbuserindex);
		
		sprintf(g_buf, "UPDATE t_character_exist SET a_is_character = '1' where a_user_idx = '%d'", userindex );
		cmd.SetQuery(g_buf);
		if( !cmd.Update() )
		{
			GAMELOG << init("UPDATE T_CHARACTER_EXIST QUERRY FAILED")
			<< userindex << delim
			<< ischaracter << end;
		}
		return;
	}
	

	
}
#endif

#ifdef LIMIT_CATALOG
void ConnSetLimit(CDescriptor* d, CNetMsg& msg)
{
	int ctid, moreSell;

	msg >> ctid
		>> moreSell;

	CLimitSell* limit = new CLimitSell(ctid, -moreSell);

	if( !limit )
		return;
	void *pos = gserver.m_limitSell.FindDataWithCompareFunction(limit, CLimitSell::CompByIndex);
	if( pos )
	{
		delete limit;
		limit = gserver.m_limitSell.GetData(pos);
		if( limit )
		{
			limit->moreSell(moreSell);
			CNetMsg smsg;
			LimitCatalogMsg(smsg, limit);
			gserver.SendAllGameServer(smsg);
			gserver.m_changeLimit = true;
		}
	}
	else
	{
		gserver.m_limitSell.AddToTail(limit);
		CNetMsg smsg;
		LimitCatalogMsg(smsg, limit);
		gserver.SendAllGameServer(smsg);
		gserver.m_changeLimit = true;
	}
}
#endif


#ifdef EVENT_OPEN_BETA_GIFT
void ConnOpenBetaGift(CDescriptor* d, CNetMsg& msg)
{
	unsigned char subtype;
	msg.MoveFirst();
	msg >> subtype;
	int userindex;
	int charindex;
//	char bcancel;
	msg >> userindex
		>> charindex;
//		>> bcancel;

	if (d->m_subno < 1)
		return ;

	CNetMsg rmsg;

	CUser* user = gserver.m_userList[d->m_subno - 1].Find(userindex);
	if (user == NULL)
	{
		PreopenGiftErrorMsg(rmsg, userindex, charindex, MSG_CONN_ERROR_OPEN_BETA_GIFT_INSUFF_CONDITION);
		SEND_Q(rmsg, d);
		return ;
	}

//	if (bcancel == 0)
//	{

	
	sprintf(g_buf, "INSERT INTO t_event_openbeta ( a_useridx, a_charidx, a_givedate ) VALUES ( %d, %d, NOW() )", userindex, charindex );
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbuser );
	cmd.SetQuery( g_buf );
	if ( !cmd.Update() )
	{
		OpenBetaGiftErrMsg( rmsg, userindex, charindex, MSG_CONN_ERROR_OPEN_BETA_GIFT_ALREADY_GIVE );

	}
	else 
	{
		OpenBetaGiftErrMsg( rmsg, userindex, charindex, MSG_CONN_ERROR_SUCCESS );
	}
	SEND_Q( rmsg, d );
//		switch (user->m_preopen_chk)
//		{
//		case 0:
//			PreopenGiftErrorMsg(rmsg, userindex, charindex, MSG_CONN_ERROR_OPEN_BETA_GIFT_INSUFF_CONDITION);
//			break ;
//
//		case 1:
//			{
//				// UPDATE t_users SET a_preopen_chk=2 WHERE a_portal_index=userindex AND a_preopen_chk=1
//				strcpy(g_buf, "UPDATE t_users SET a_preopen_chk=2 WHERE a_portal_index=");
//				IntCat(g_buf, userindex, false);
//				strcat(g_buf, " AND a_preopen_chk=1");
//
//				CDBCmd cmd;
//				cmd.Init(&gserver.m_dbuser);
//				cmd.SetQuery(g_buf);
//				if (cmd.Update() && cmd.m_nrecords > 0)
//				{
//					PreopenGiftErrorMsg(rmsg, userindex, charindex, MSG_CONN_ERROR_SUCCESS);
//					user->m_preopen_chk = 2;
//				}
//				else
//				{
//					PreopenGiftErrorMsg(rmsg, userindex, charindex, MSG_CONN_ERROR_OPEN_BETA_GIFT_ALREADY_GIVE);
//				}
//			}
//			break ;
//
//		default:
//			PreopenGiftErrorMsg(rmsg, userindex, charindex, MSG_CONN_ERROR_OPEN_BETA_GIFT_ALREADY_GIVE);
//			break ;
//		}
//		SEND_Q(rmsg, d);
//	}
//	else
//	{
////		// UPDATE t_users SET a_preopen_chk=1 WHERE a_portal_index=userindex AND a_preopen_chk=2
////		strcpy(g_buf, "UPDATE t_users SET a_preopen_chk=1 WHERE a_portal_index=");
////		IntCat(g_buf, userindex, false);
////		strcat(g_buf, " AND a_preopen_chk=2");
////
////		CDBCmd cmd;
////		cmd.Init(&gserver.m_dbuser);
////		cmd.SetQuery(g_buf);
////		cmd.Update();
////		user->m_preopen_chk = 1;
//	}
}
#endif // EVENT_OPEN_BETA_GIFT

#ifdef JPN_GPARA_PROMOTION
void ConnGparaPromotion(CDescriptor* d, CNetMsg& msg)
{
	CNetMsg		rmsg;
	int			nSubType = 0;
	int			nUserIndex = 0;
	int			nCharIndex = 0;

	msg >> nSubType
		>> nUserIndex
		>> nCharIndex;

	CLCString	strPartnerID(10);
	int			nCreateTime = 0;
	int			nBaseTime = 0;

	CDBCmd cmdAuth;
	CDBCmd cmdUser;
	cmdAuth.Init(&gserver.m_dbauth);
	cmdUser.Init(&gserver.m_dbuser);
	switch (nSubType)
	{
	case MSG_CONN_GPARA_PROMOTION_QUERY:
		// bg_user에서 가입일 및 가입코드 검사
		sprintf(g_buf, "SELECT partner_id, UNIX_TIMESTAMP(create_date) AS a_create_time, UNIX_TIMESTAMP('2007-03-01') AS a_base_time FROM bg_user WHERE user_code=%d", nUserIndex);
		cmdAuth.SetQuery(g_buf);
		if (!cmdAuth.Open() || !cmdAuth.MoveFirst())
		{
			ConnGparaPromotionMsg(rmsg, MSG_CONN_GPARA_PROMOTION_DENY, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			return ;
		}
		cmdAuth.GetRec("partner_id",		strPartnerID);
		cmdAuth.GetRec("a_create_time",		nCreateTime);
		cmdAuth.GetRec("a_base_time",		nBaseTime);
		if (strcmp(strPartnerID, JPN_GPARA_PROMOTION) != 0 || nBaseTime > nCreateTime)
		{
			ConnGparaPromotionMsg(rmsg, MSG_CONN_GPARA_PROMOTION_DENY, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			return ;
		}

		// t_gpara_promotion_2007에서 지급여부 검사
		sprintf(g_buf, "SELECT * FROM t_gpara_promotion_2007 WHERE a_user_index=%d", nUserIndex);
		cmdUser.SetQuery(g_buf);
		if (!cmdUser.Open() || cmdUser.MoveFirst())
		{
			ConnGparaPromotionMsg(rmsg, MSG_CONN_GPARA_PROMOTION_DENY, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			return ;
		}

		ConnGparaPromotionMsg(rmsg, MSG_CONN_GPARA_PROMOTION_ALLOW, nUserIndex, nCharIndex);
		SEND_Q(rmsg, d);
		break;

	case MSG_CONN_GPARA_PROMOTION_COMMIT:
		// t_gpara_promotion_2007에 저장
		sprintf(g_buf, "INSERT INTO t_gpara_promotion_2007 (a_user_index, a_server, a_char_index) VALUES (%d, %d, %d)", nUserIndex, gserver.m_serverno, nCharIndex);
		cmdUser.SetQuery(g_buf);
		if (!cmdUser.Update())
		{
			GAMELOG << init("GPARA GIFT")
					<< "ERROR : CANNOT INSERT" << delim
					<< mysql_error(&gserver.m_dbuser)
					<< end;
		}
		break;
	}
}
#endif // JPN_GPARA_PROMOTION

#ifdef JPN_OCN_GOO_PROMOTION
void ConnOCN_GooPromotion(CDescriptor* d, CNetMsg& msg)
{
	CNetMsg		rmsg;
	int			nSubType = 0;
	int			nUserIndex = 0;
	int			nCharIndex = 0;

	msg >> nSubType
		>> nUserIndex
		>> nCharIndex;

	CLCString	strPartnerID(10);
	int			nCreateTime = 0;
	int			nBaseTime = 0;

	CDBCmd cmdAuth;
	CDBCmd cmdUser;
	cmdAuth.Init(&gserver.m_dbauth);
	cmdUser.Init(&gserver.m_dbuser);
	switch (nSubType)
	{
	case MSG_CONN_OCN_GOO_PROMOTION_QUERY:
		// bg_user에서 가입일 및 가입코드 검사
		sprintf(g_buf, "SELECT partner_id, UNIX_TIMESTAMP(create_date) AS a_create_time, UNIX_TIMESTAMP('2007-03-01') AS a_base_time FROM bg_user WHERE user_code=%d", nUserIndex);
		cmdAuth.SetQuery(g_buf);
		if (!cmdAuth.Open() || !cmdAuth.MoveFirst())
		{
			ConnOCN_GooPromotionMsg(rmsg, MSG_CONN_OCN_GOO_PROMOTION_DENY, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			return ;
		}
		cmdAuth.GetRec("partner_id",		strPartnerID);
		cmdAuth.GetRec("a_create_time",		nCreateTime);
		cmdAuth.GetRec("a_base_time",		nBaseTime);
		if ( (strcmp(strPartnerID, "OL") != 0 
			   && strcmp(strPartnerID, "GL") != 0  
			   && strcmp(strPartnerID, "BC") != 0   
			   && strcmp(strPartnerID, "JG") != 0  
			  ) 
			  || nBaseTime > nCreateTime 
			)
		{
			ConnOCN_GooPromotionMsg(rmsg, MSG_CONN_OCN_GOO_PROMOTION_DENY, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			return ;
		}

		// t_gpara_promotion_2007에서 지급여부 검사
		sprintf(g_buf, "SELECT * FROM t_ocn_goo_promotion_2008 WHERE a_user_index=%d", nUserIndex);
		cmdUser.SetQuery(g_buf);
		if (!cmdUser.Open() || cmdUser.MoveFirst())
		{
			ConnOCN_GooPromotionMsg(rmsg, MSG_CONN_OCN_GOO_PROMOTION_DENY, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			return ;
		}

		ConnOCN_GooPromotionMsg(rmsg, MSG_CONN_OCN_GOO_PROMOTION_ALLOW, nUserIndex, nCharIndex);
		SEND_Q(rmsg, d);
		break;

	case MSG_CONN_OCN_GOO_PROMOTION_COMMIT:
		// t_gpara_promotion_2007에 저장
		sprintf(g_buf, "INSERT INTO t_ocn_goo_promotion_2008 (a_user_index, a_server, a_char_index) VALUES (%d, %d, %d)", nUserIndex, gserver.m_serverno, nCharIndex);
		cmdUser.SetQuery(g_buf);
		if (!cmdUser.Update())
		{
			GAMELOG << init("GPARA GIFT")
					<< "ERROR : CANNOT INSERT" << delim
					<< mysql_error(&gserver.m_dbuser)
					<< end;
		}
		break;
	}
}
#endif // JPN_OCN_GOO_PROMOTION

#ifdef JPN_MSN_PROMOTION
void ConnMSNPromotion(CDescriptor* d, CNetMsg& msg)
{
	CNetMsg		rmsg;
	int			nSubType = 0;
	int			nUserIndex = 0;
	int			nCharIndex = 0;

	msg >> nSubType
		>> nUserIndex
		>> nCharIndex;

	int			nCreateTime = 0;
	int			nBaseTime = 0;

	CDBCmd cmdAuth;
	CDBCmd cmdUser;
	cmdAuth.Init(&gserver.m_dbauth);
	cmdUser.Init(&gserver.m_dbuser);
	switch (nSubType)
	{
	case MSG_CONN_MSN_PROMOTION_QUERY:
		// bg_user에서 가입일 검사
		sprintf(g_buf, "SELECT UNIX_TIMESTAMP(create_date) AS a_create_time, UNIX_TIMESTAMP('2007-05-25') AS a_base_time FROM bg_user WHERE user_code=%d", nUserIndex);
//		sprintf(g_buf, "SELECT UNIX_TIMESTAMP(create_date) AS a_create_time, UNIX_TIMESTAMP('2007-05-18') AS a_base_time FROM bg_user WHERE user_code=%d", nUserIndex);
		cmdAuth.SetQuery(g_buf);
		if (!cmdAuth.Open() || !cmdAuth.MoveFirst())
		{
			ConnMSNPromotionMsg(rmsg, MSG_CONN_MSN_PROMOTION_DENY, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			return ;
		}
		
		cmdAuth.GetRec("a_create_time",		nCreateTime);
		cmdAuth.GetRec("a_base_time",		nBaseTime);
		if (nBaseTime > nCreateTime)
		{
			ConnMSNPromotionMsg(rmsg, MSG_CONN_MSN_PROMOTION_DENY, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			return ;
		}

		// t_msn_promotion_2007에서 지급여부 검사
		sprintf(g_buf, "SELECT * FROM t_msn_promotion_2007 WHERE a_user_index=%d", nUserIndex);
		cmdUser.SetQuery(g_buf);
		if (!cmdUser.Open() || cmdUser.MoveFirst())
		{
			ConnMSNPromotionMsg(rmsg, MSG_CONN_MSN_PROMOTION_DENY, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			return ;
		}

		ConnMSNPromotionMsg(rmsg, MSG_CONN_MSN_PROMOTION_ALLOW, nUserIndex, nCharIndex);
		SEND_Q(rmsg, d);
		break;

	case MSG_CONN_MSN_PROMOTION_COMMIT:
		// t_msn_promotion_2007에 저장
		sprintf(g_buf, "INSERT INTO t_msn_promotion_2007 (a_user_index, a_server, a_char_index) VALUES (%d, %d, %d)", nUserIndex, gserver.m_serverno, nCharIndex);
		cmdUser.SetQuery(g_buf);
		if (!cmdUser.Update())
		{
			GAMELOG << init("MSN GIFT")
					<< "ERROR : CANNOT INSERT" << delim
					<< mysql_error(&gserver.m_dbuser)
					<< end;
		}
		break;
	}
}
#endif // JPN_MSN_PROMOTION

#ifdef CREATE_EVENT
void ConnCreateEvent(CDescriptor* d, CNetMsg& msg)
{
	CNetMsg		rmsg;
	int			nSubType = 0;
	int			nUserIndex = 0;
	int			nCharIndex = 0;
	int			stime = 0;
	int			etime = 0;
	int			tableid = 0;

	msg >> nSubType
		>> nUserIndex
		>> nCharIndex
		>> stime
		>> etime
		>> tableid;

	int		nCreateTime = 0;
	
	CDBCmd cmdAuth;
	CDBCmd cmdUser;

	cmdAuth.Init(&gserver.m_dbauth);
	cmdUser.Init(&gserver.m_dbuser);

	switch (nSubType)
	{
	case MSG_CONN_CREATEEVENT_CREATETABLE:
		// 테이블 생성 
		sprintf(g_buf, "CREATE TABLE t_createevent_%d(a_user_index int primary key, a_server int, a_char_index int)", tableid);
		cmdUser.SetQuery(g_buf);
		if(cmdUser.Update() == true)
		{
			ConnCreateEventMsg(rmsg, MSG_CONN_CREATEEVENT_CREATETABLE, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			
			GAMELOG << init("CREATE EVENT TABLE CREATE") << end;
		}
		else
		{
			GAMELOG << init("CREATE EVENT TABLE CREATE FAIL") << end;
		}
		break;

	case MSG_CONN_CREATEEVENT_QUERY:
		// bg_user에서 가입일 검사
		sprintf(g_buf, "SELECT UNIX_TIMESTAMP(create_date) AS a_create_time FROM bg_user WHERE user_code=%d", nUserIndex);
		cmdAuth.SetQuery(g_buf);		
		if (!cmdAuth.Open() || !cmdAuth.MoveFirst())
		{
			ConnCreateEventMsg(rmsg, MSG_CONN_CREATEEVENT_DENY, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			return ;
		}
		
		cmdAuth.GetRec("a_create_time",		nCreateTime);
		
		if(stime > nCreateTime
			|| etime < nCreateTime)
		{
			ConnCreateEventMsg(rmsg, MSG_CONN_CREATEEVENT_DENY, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			return ;
		}

		// 지급여부 검사
		sprintf(g_buf, "SELECT * FROM t_createevent_%d WHERE a_user_index=%d", tableid, nUserIndex);
		cmdUser.SetQuery(g_buf);
		if (!cmdUser.Open() || cmdUser.MoveFirst())
		{
			ConnCreateEventMsg(rmsg, MSG_CONN_CREATEEVENT_DENY, nUserIndex, nCharIndex);
			SEND_Q(rmsg, d);
			return ;
		}

		ConnCreateEventMsg(rmsg, MSG_CONN_CREATEEVENT_ALLOW, nUserIndex, nCharIndex);
		SEND_Q(rmsg, d);
		break;

	case MSG_CONN_CREATEEVENT_COMMIT:
		sprintf(g_buf, "INSERT INTO t_createevent_%d(a_user_index, a_server, a_char_index) VALUES (%d, %d, %d)", 
			tableid, nUserIndex, gserver.m_serverno, nCharIndex);
		cmdUser.SetQuery(g_buf);
		if (!cmdUser.Update())
		{
			GAMELOG << init("CREATE EVENT GIFT")
					<< "ERROR : CANNOT INSERT" << delim
					<< mysql_error(&gserver.m_dbuser)
					<< end;
		}
		break;
	}
}
#endif // CREATE_EVENT


#ifdef EVENT_GOMDORI_2007
void ConnEventGomdori2007(CDescriptor* d, CNetMsg& msg)
{
	unsigned char			cSubType;
	int						nCharIndex;
	int						nTimeDiff;
	struct tm				tmCur;
	CDBCmd					cmdUser;
	CNetMsg					rmsg;

	msg >> cSubType;

	cmdUser.Init(&gserver.m_dbuser);

	switch (cSubType)
	{
	case MSG_CONN_EVENT_GOMDORI_2007_COUNT:
		gserver.m_nEventGomdori2007StatusCurServer++;
		break;

	case MSG_CONN_EVENT_GOMDORI_2007_STATUS:
		msg >> nCharIndex;
		tmCur = NOW();
		nTimeDiff = tmCur.tm_min - gserver.m_nEventGomdori2007LastRead;
		if (nTimeDiff < 0)
			nTimeDiff += 60;
		if (nTimeDiff >= 5 || gserver.m_nEventGomdori2007LastRead < 0)
		{
			gserver.m_nEventGomdori2007LastRead = tmCur.tm_min;
			gserver.m_nEventGomdori2007Count = 0;
			if (gserver.m_nEventGomdori2007Status)
				delete [] gserver.m_nEventGomdori2007Status;
			gserver.m_nEventGomdori2007Status = NULL;

			sprintf(g_buf, "SELECT * FROM t_event_gomdori_count ORDER BY a_server");
			cmdUser.SetQuery(g_buf);
			if (cmdUser.Open())
			{
				gserver.m_nEventGomdori2007Count = cmdUser.m_nrecords;
				if (gserver.m_nEventGomdori2007Count > 0)
				{
					gserver.m_nEventGomdori2007Status = new int[gserver.m_nEventGomdori2007Count];
					int i = 0;
					while (cmdUser.MoveNext())
					{
						cmdUser.GetRec("a_count", gserver.m_nEventGomdori2007Status[i]);
						i++;
					}
				}
			}
		}
		ConnEventGomdori2007StatusMsg(rmsg, nCharIndex, gserver.m_nEventGomdori2007Count, gserver.m_nEventGomdori2007Status);
		SEND_Q(rmsg, d);
		break;

	default:
		return ;
	}
}
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_FLOWERTREE_2007
void ConnEventFlowerTree2007(CDescriptor* d, CNetMsg& msg)
{
	unsigned char	cSubType =0;
	int				nCharIndex = 0;
	CDBCmd			cmdAuth;

	struct tm		currentTime;
	int				diffTime;
	CNetMsg			rmsg;

	msg >> cSubType;
	msg >> nCharIndex;

	cmdAuth.Init(&gserver.m_dbuser);

	switch( cSubType )
	{
	case MSG_CONN_EVENT_FLOWERTREE_2007_ADDPOINT:  // 포인트 증가 
		{
			unsigned int point = 0;
			unsigned int unOldPoint = 0;
			msg >> point;		

			unOldPoint = gserver.m_nEventFlowerTree2007StatusCurServer;
			gserver.m_nEventFlowerTree2007StatusCurServer += point;
			int nServerIndex = gserver.m_serverno %100 ;
			if( nServerIndex > gserver.m_nEventFlowerTree2007Count )
			{
				GAMELOG << init("EVENT  FLOWERTREE SERVER LIST COUNT OVERFLOW ( MAX 99 )") << end;
				return;
			}
			gserver.m_nEventFlowerTree2007Status[nServerIndex-1] = gserver.m_nEventFlowerTree2007StatusCurServer;

			unsigned int updatePoint[10] = { 100,2000,3000,4000,5000,6000,7000,8000,9000,20000 };
			// 전 체널에 나무 업데이트 메세지
			for ( int i=0 ; i < 10; i++ )
			{
				if( unOldPoint < updatePoint[i] && gserver.m_nEventFlowerTree2007StatusCurServer >= updatePoint[i] )
				{
					ConnEventFlowerTree2007Msg( rmsg , MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT, 0) ;
					rmsg << gserver.m_nEventFlowerTree2007StatusCurServer;
					gserver.SendAllGameServer(rmsg);
					break;
				}
			}
		} break;

	case MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT :		// 자신의 나무 포인트 요청
		{
			ConnEventFlowerTree2007Msg( rmsg , MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT, nCharIndex ) ;
			rmsg << gserver.m_nEventFlowerTree2007StatusCurServer;
			SEND_Q(rmsg, d);
			return;
		} break;

	case MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT_RANK:
		{
			currentTime = NOW();
			diffTime = currentTime.tm_min - gserver.m_nEventFlowerTree2007LastRead;
			if (diffTime < 0)
				diffTime += 60;
			if (diffTime >= 5 || gserver.m_nEventFlowerTree2007LastRead < 0)
			{
				gserver.m_nEventFlowerTree2007LastRead = currentTime.tm_min;
				gserver.m_nEventFlowerTree2007Count = 0;
				if (gserver.m_nEventFlowerTree2007Status)
					delete [] gserver.m_nEventFlowerTree2007Status;
				gserver.m_nEventFlowerTree2007Status = NULL;

				sprintf(g_buf, "SELECT * FROM t_event_flowertree_point ORDER BY a_server");
				cmdAuth.SetQuery(g_buf);
				if (cmdAuth.Open())
				{
					gserver.m_nEventFlowerTree2007Count = cmdAuth.m_nrecords;
					if (gserver.m_nEventFlowerTree2007Count > 0)
					{
						gserver.m_nEventFlowerTree2007Status = new unsigned int[gserver.m_nEventFlowerTree2007Count];
						int i = 0;
						while (cmdAuth.MoveNext())
						{
							cmdAuth.GetRec("a_point", gserver.m_nEventFlowerTree2007Status[i]);
							i++;
						}
					}
				}
			}
			
			ConnEventFlowerTree2007Msg( rmsg , MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT_RANK, nCharIndex ) ;
			rmsg <<  gserver.m_nEventFlowerTree2007Count;
			for( int i=0 ; i< gserver.m_nEventFlowerTree2007Count ; i++ )
			{
				rmsg << gserver.m_nEventFlowerTree2007Status[i];
			}
			SEND_Q(rmsg, d);
		}break;
	}
}
#endif //EVENT_FLOWERTREE_2007

#ifdef GMTOOL
void ConnGMToolCommand( CDescriptor* d, CNetMsg& msg )
{
	int gmindex;
	CLCString gmname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString gmcommand(1000);

	char name[100], command[1000];
	
	msg >> gmindex
		>> gmname
		>> gmcommand;

	memset(name, 0, 100);
	memset(command, 0, 1000);
	strcpy(g_buf, (const char*)gmname);
	mysql_real_escape_string(&gserver.m_dbuser, name, g_buf, strlen(g_buf));
	strcpy(g_buf, (const char*)gmcommand);
	mysql_real_escape_string(&gserver.m_dbuser, command, g_buf, strlen(g_buf));

	char sql[4096];
	memset(sql, 0, 4096);
	sprintf(sql, "INSERT INTO t_gmtool_command (`a_gmindex`, `a_gmname`, `a_date`, `a_gmcommand`) VALUES(%d, '%s', NOW(), '%s')",
				gmindex, name, command);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);
	cmd.SetQuery(sql);
	cmd.Update();
}

void ConnGMToolChatMonitor( CDescriptor* d, CNetMsg& msg )
{
	int charindex;
	unsigned char chattype;
	int server, sub;
	CLCString chat(1000);
	CLCString MonitorName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> charindex
		>> MonitorName
		>> chattype
		>> server
		>> sub
		>> chat;

	char sql[4096];
	char strEscapeChat[1000];
	memset(strEscapeChat, 0, 1000);
	mysql_real_escape_string(&gserver.m_dbuser, strEscapeChat, chat, strlen(chat));

	memset(sql, 0, 4096);
	sprintf(sql, "INSERT INTO t_gmtool_chatmonitor (`a_charindex`, `a_server`, `a_subno`, `a_type`, `a_chat`, `a_date`) VALUES(%d, %d, %d, %d, '%s', NOW())",
						charindex, server, sub, chattype, strEscapeChat);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);
	cmd.SetQuery(sql);
	cmd.Update();
}

void ConnGMToolChatting( CDescriptor* d, CNetMsg& msg )
{
	int server, sub, gmindex;
	CLCString gmname(MAX_CHAR_NAME_LENGTH + 1), username(MAX_CHAR_NAME_LENGTH + 1);
	CLCString chat(1000);
	msg >> server
		>> sub
		>> gmindex
		>> gmname
		>> username
		>> chat;

	char sql[4096];
	char strEscapeGMName[MAX_CHAR_NAME_LENGTH + 1], strEscapeUserName[MAX_CHAR_NAME_LENGTH + 1];
	char strEscapeChat[1000];
	memset(strEscapeGMName, 0, MAX_CHAR_NAME_LENGTH + 1);
	memset(strEscapeUserName, 0, MAX_CHAR_NAME_LENGTH + 1);
	memset(strEscapeChat, 0, 1000);

	mysql_real_escape_string(&gserver.m_dbuser, strEscapeGMName, (const char*)gmname, gmname.Length());
	mysql_real_escape_string(&gserver.m_dbuser, strEscapeUserName, (const char*)username, username.Length());
	mysql_real_escape_string(&gserver.m_dbuser, strEscapeChat, (const char*)chat, chat.Length());

	memset(sql, 0, 4096);
	sprintf(sql,"INSERT INTO t_gmtool_chatting (`a_server`, `a_sub`, `a_gmindex`, `a_gmname`, `a_charname`, `a_chat`, `a_date`) VALUES(%d, %d, %d, '%s', '%s', '%s', NOW())",
								server, sub, gmindex, strEscapeGMName, strEscapeUserName, strEscapeChat);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);
	cmd.SetQuery(sql);
	cmd.Update();
}
#endif // GMTOOL
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
void ConnGuildPointRankingReq( CDescriptor* d, CNetMsg& msg )
{
	CNetMsg rmsg;

	int guildindex = 0;
	int charindex = 0;

	msg	>> guildindex
		>> charindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	sprintf(g_buf, "select a_rank from t_guild_point_rank_all where a_server = %d and a_guild_index = %d", gserver.m_serverno, guildindex );
	cmd.SetQuery(g_buf);
	if( !cmd.Open()|| !cmd.MoveFirst() )
	{
		//DB error
		GuildPointRankerRepMsg(rmsg, charindex, guildindex, -1 );
		SEND_Q(rmsg, d);
		return;
	}

	int ranking;
	cmd.GetRec( "a_rank", ranking );
	
	if( cmd.m_nrecords > 0 )
	{
		// 랭커라는 메세지		
		GuildPointRankerRepMsg(rmsg, charindex, guildindex, ranking);
		SEND_Q(rmsg, d);
	}
}
#endif // NEW_GUILD_POINT_RANKING_NOTICE

#ifdef GIFT_EVENT_2007
void ConnEventGift2007(CDescriptor * d, CNetMsg & msg)
{
	unsigned char cSubType = 0;
	int userindex = 0, charindex = 0;
	msg >> cSubType >> userindex >> charindex;

	CNetMsg rmsg;
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	switch(cSubType)
	{
	case MSG_CONN_EVENT_GIFT_2007_CHECK:
		{ // 지급 확인
#ifdef ADULT_SERVER
			// 캐릭터당 지급 확인
			sprintf(g_buf, "SELECT * FROM t_event_gift_2007_adult WHERE a_char_index=%d and a_server_no=%d", charindex, gserver.m_serverno);
#else
			// 유저당 지급 확인
			sprintf(g_buf, "SELECT * FROM t_event_gift_2007 WHERE a_user_index=%d", userindex);
#endif // ADULT_SERVER

			cmd.SetQuery(g_buf);
			if (cmd.Open())
			{
				if (cmd.m_nrecords > 0)
				{	// 이미 지급
					EventGift2007Msg(rmsg, MSG_CONN_EVENT_GIFT_2007_CHECK_DENY, userindex, charindex);
					SEND_Q(rmsg, d);
				}
				else
				{
					EventGift2007Msg(rmsg, MSG_CONN_EVENT_GIFT_2007_CHECK_ALLOW, userindex, charindex);
					SEND_Q(rmsg, d);
				}

			}
			else
			{	// db error
				EventGift2007Msg(rmsg, MSG_CONN_EVENT_GIFT_2007_CHECK_DBERROR, userindex, charindex);
				SEND_Q(rmsg, d);
			}
		}
		
		break;

	case MSG_CONN_EVENT_GIFT_2007_GIVE:
		{ // 지급
#ifdef ADULT_SERVER
			// 캐릭터당 지급 확인
			sprintf(g_buf, "INSERT INTO t_event_gift_2007_adult(a_server_no, a_user_index, a_char_index) VALUES(%d, %d, %d)", 
				gserver.m_serverno, userindex, charindex);
#else
			// 유저당 지급 확인
			sprintf(g_buf, "INSERT INTO t_event_gift_2007(a_server_no, a_user_index, a_char_index) VALUES(%d, %d, %d)", 
				gserver.m_serverno, userindex, charindex);
#endif // ADULT_SERVER

			cmd.SetQuery(g_buf);
			cmd.Update();
		}
		
		break;
	}
}
#endif // GIFT_EVENT_2007

#ifdef EVENT_BJMONO_2007
void ConnEvnetBjMono2007(CDescriptor * d, CNetMsg & msg)
{
	// 받을수 있는 응모권 갯수 확인해서
	// 응모권 개수 통보
	unsigned char cSubType = 0;
	int userindex = 0, charindex = 0;
	msg >> cSubType >> userindex >> charindex;

	CNetMsg rmsg;
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	switch( cSubType )
	{
	case MSG_CONN_EVENT_BJMONO_TICKET_CHECK :
		{
			int nTotalTicketCount = 0, nUsedTicketCount = 0;
			sprintf(g_buf, "SELECT * FROM t_event_bjmono_2007 WHERE a_user_index=%d", userindex);
			cmd.SetQuery(g_buf);
			if (cmd.Open() && cmd.MoveFirst())
			{
				cmd.GetRec("a_ticket_count", nTotalTicketCount );
				cmd.GetRec("a_used_ticket", nUsedTicketCount );

				ConnEventBjMono2007Msg(rmsg, charindex, nTotalTicketCount - nUsedTicketCount );
				SEND_Q(rmsg, d);
			}
		}break;
	case MSG_CONN_EVENT_BJMONO_TICKET_UPDATE :
		{
			int nTicketCount;
			msg >> nTicketCount;
			sprintf(g_buf, "UPDATE t_event_bjmono_2007 set a_used_ticket=a_used_ticket+%d WHERE a_user_index=%d", nTicketCount, userindex);
			cmd.SetQuery(g_buf);
			cmd.Update();
		}break;
	}
	
}
#endif // EVENT_BJMONO_2007

#ifdef EVENT_SSHOT_2007
void ConnEvnetSshot2007( CDescriptor * d, CNetMsg & msg)
{
	unsigned char cSubType = 0;
	int userindex = 0, charindex = 0;
	msg >> cSubType >> userindex >> charindex;

	CNetMsg rmsg;
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	switch( cSubType )
	{
	case MSG_CONN_EVENT_SSHOT_CHECK :
		{
			short	nUsed;
			int		errorcode = 0;
			sprintf(g_buf, "SELECT * FROM t_event_sshot_2007 WHERE a_user_index=%d", userindex);
			cmd.SetQuery(g_buf);
			if ( cmd.Open() && cmd.MoveFirst() )
			{
				cmd.GetRec("a_used", nUsed );

				if( nUsed != 0 ) // 사용한적 있음
				{ errorcode = 1; }
			}
			else
			{
				errorcode = 2;
			}
			ConnEventSshot2007Msg(rmsg, charindex, errorcode );
			SEND_Q(rmsg, d);
		}break;

	case MSG_CONN_EVENT_SSHOT_UPDATE :
		{
			sprintf(g_buf, "UPDATE t_event_sshot_2007 set a_used=1, a_update_date=NOW() WHERE a_user_index=%d", userindex);
			cmd.SetQuery(g_buf);
			cmd.Update();
		}break;
	}
}
#endif //EVENT_SSHOT_2007

#ifdef EVENT_XMAS_2007
void ConnEventXmas2007( CDescriptor* d, CNetMsg& msg )
{
// KYD TO DO
	// 현재 트리 포인트 요청
	// 자신의 서버 포인트 적립
	//	MSG_CONN_EVENT_XMASTREE_ADDPOINT,	// REQ : (un)point, 
	CNetMsg			rmsg;
	unsigned char	cSubType = 0;
	msg >> cSubType ;

	switch( cSubType )
	{
	case MSG_CONN_EVENT_XMASTREE_ADDPOINT :
		{
			unsigned int unPoint, unOldPoint;
			msg >> unPoint;

			unOldPoint = gserver.m_unEventXmasTreePoint;
			gserver.m_unEventXmasTreePoint += unPoint;

			// 1000 Point 마다 전 체널에 아이템 드랍 & 전체널에 나무 업데이트 MSG
			int nDropCount = (int)(gserver.m_unEventXmasTreePoint/1000) - (int)(unOldPoint/1000);
			if ( nDropCount > 0 )
			{
				// Send TreePoint to AllGameServer
				ConnEventXmas2007Msg( rmsg , MSG_CONN_EVENT_XMASTREE_POINT) ;
				rmsg << gserver.m_unEventXmasTreePoint;
				gserver.SendAllGameServer(rmsg);

				// ItemDrop Count Setting
				gserver.m_nEventXmasTreeNoticeTime += (nDropCount * 6);
				//  TimerToGameServer 에서 계산
			}
		} break;
	case MSG_CONN_EVENT_XMASTREE_POINT :
		{
			ConnEventXmas2007Msg( rmsg , MSG_CONN_EVENT_XMASTREE_POINT) ;
			rmsg << gserver.m_unEventXmasTreePoint;
			SEND_Q(rmsg, d);
		}break;
	}
}
#endif //EVENT_XMAS_2007

#ifdef ATTENDANCE_EVENT
void WriteAttendance(int userindex, int server)
{
	CLCString sql(1024);
	sql.Format("SELECT * FROM TBL_Attendance WHERE User_Idx = %d AND (Date_Time >= CURDATE() AND Date_Time < DATE_ADD(CURDATE(), INTERVAL 1 DAY) )", userindex );
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbevent);
	cmd.SetQuery(sql);
	if(cmd.Open())
	{
		if(!cmd.MoveFirst())
		{
			sql.Format("INSERT INTO TBL_Attendance (User_Idx, Server_ID, Date_Time) VALUES (%d, %d, NOW())", userindex, server);
			cmd.SetQuery(sql);
			if(cmd.Update())
			{
				GAMELOG << init("CHECK_ATTENDANCE") << delim
						<< userindex << delim
						<< server << end;
			}
			else
			{
				GAMELOG << init("UPDATE_FAILED_TBL_ATTENDANCE") << delim
						<< userindex << delim
						<< server << end;
			}

			sql.Format("INSERT INTO t_event_attendance (a_user_idx, a_count, a_reward) VALUES( %d, 1, 0 )", userindex);
			
			CDBCmd cmdUser;
			cmdUser.Init(&gserver.m_dbuser);
			cmdUser.SetQuery(sql);
			if(!cmdUser.Update())
			{
				sql.Format("UPDATE t_event_attendance SET a_count = a_count + 1 WHERE a_user_idx = %d", userindex);
				cmdUser.SetQuery(sql);
				if(!cmdUser.Update())
				{
					GAMELOG << init("UPDATE FAILED t_event_attendance")
							<< userindex
							<< end;
				}
			}
		}
	}
	else
	{
	 	GAMELOG << init("OPEN_FAILED_TBL_ATTENDANCE") << delim
				<< userindex << delim
				<< server << end;
	}
}
#endif // ATTENDANCE_EVENT

#ifdef CLIENT_RESTART
void ConnClientRestart( CDescriptor* d, CNetMsg& msg )
{
	CLCString userid(25);
	msg >> userid;
	// masterdb >> bg_user_active 에 임시 패시워드 연장
	// "update bg_user_active set active_time=now() where active_game='LC' and active_id='%s' ", userid

	CNetMsg rmsg;
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbauth);
	sprintf(g_buf, "UPDATE bg_user_active SET active_time=NOW() WHERE active_game='LC' and active_id='%s' ", (const char*)userid );
	cmd.SetQuery(g_buf);
	cmd.Update();
}
#endif //CLIENT_RESTART

#ifdef ATTENDANCE_EVENT_REWARD
// 보상 여부 확인 
// return charIndex, count
void ConnAttendanceEventReward( CDescriptor* d, CNetMsg& msg )
{
	int userIndex, charIndex;
	unsigned char subtype;
	CLCString sql(1024);

	msg >> subtype >> userIndex >> charIndex ;

	switch( subtype )
	{
	case MSG_ATTENDANCE_COUNT:
		{
			int attendanceCount = 0;
			CDBCmd cmdUser;
			cmdUser.Init(&gserver.m_dbuser);
			sql.Format("SELECT a_reward FROM t_event_attendance WHERE a_user_idx = %d", userIndex);
			cmdUser.SetQuery(sql);
			if ( cmdUser.Open() && cmdUser.MoveFirst() )
			{
				int reward;
				cmdUser.GetRec("a_reward", reward );
				if( reward == 0 )
				{
					sql.Format("SELECT count(Idx) as count FROM TBL_Attendance WHERE User_Idx = %d AND Date_Time < '2008-05-16' and Server_ID=1 group by User_Idx", userIndex );
					CDBCmd cmd;
					cmd.Init(&gserver.m_dbevent);
					cmd.SetQuery(sql);
					if( cmd.Open() && cmd.MoveFirst() )
					{
						cmd.GetRec("count", attendanceCount );
					}
				}
			}

			CNetMsg rmsg;
			ConnEventAttendanceRewardMsg( rmsg , charIndex, attendanceCount );
			SEND_Q(rmsg, d);
		}break;

	case MSG_ATTENDANCE_UPDATE:
		{
			CDBCmd cmdUser;
			cmdUser.Init(&gserver.m_dbuser);
			sql.Format("UPDATE t_event_attendance SET a_reward=1 WHERE a_user_idx = %d", userIndex);
			cmdUser.SetQuery(sql);
			cmdUser.Update();			
		}break;		
	}
}

#endif // ATTENDANCE_EVENT_REWARD

#ifdef IRIS_POINT
bool AddIpoint ( int user_index,int char_index, int type,  int nPoint )
{
	// 웹팀에서 요구하는 다양한 방법으로 포인트를 적립
	const char desc[11][128] = { "TEACHER","LEVELUP","DUNGEON","CASTLE","ATTENDANCE","PLAY_TIME","ACC_ATTANDANCE","ETC","DELETE_CHARACTER","CANCLE_DELETE_CHARACTER","USE_ITEM" };
	int web_code;
	int nPrePoint=0;
	int nGetPoint = nPoint;

	switch( type )
	{
	case MSG_IPOINT_TYPE_PLAYTIME:
		web_code = 1;
		break;
	case MSG_IPOINT_TYPE_ACC_ATTENDANCE:
	case MSG_IPOINT_TYPE_ATTENDANCE:
		web_code = 2;
		break;
	case MSG_IPOINT_TYPE_DELETE_CHAR:
	case MSG_IPOINT_TYPE_CANCEL_DELETE_CHAR:
		web_code = 17;
		break;
	case MSG_IPOINT_TYPE_USEITEM :
	case MSG_IPOINT_TYPE_TEACHER :		
	case MSG_IPOINT_TYPE_LEVELUP :
	case MSG_IPOINT_TYPE_PD4:
	default:
		web_code = 3;
		break;
	}
	CDBCmd eventdb_cmd;
	CLCString sql(1024);
	eventdb_cmd.Init(&gserver.m_dbevent);

	sql.Format("SELECT mpt_user_point FROM TBL_MemberPoint WHERE mpt_user_idx=%d", user_index );
	eventdb_cmd.SetQuery(sql);
	if( eventdb_cmd.Open() )
	{
		if( !eventdb_cmd.MoveFirst() )
		{
			CDBCmd		userdb_cmd;
			CLCString	idname(31);
			userdb_cmd.Init(&gserver.m_dbuser);
			sql.Format("SELECT a_idname FROM t_users WHERE a_portal_index=%d" , user_index );
			userdb_cmd.SetQuery(sql);
			if( userdb_cmd.Open() && userdb_cmd.MoveFirst() )
			{
				userdb_cmd.GetRec("a_idname", idname );
			}
			
			sql.Format("INSERT INTO TBL_MemberPoint (mpt_user_idx, mpt_user_id, mpt_user_point, mpt_get_point, mpt_update_date) "
				"VALUES ( %d, '%s' , 0, 0, NOW() )" , user_index, (const char*)idname );
			eventdb_cmd.SetQuery(sql);
			eventdb_cmd.Update();
		}
		else
		{
			eventdb_cmd.GetRec("mpt_user_point",nPrePoint);
		}
	}

	// 로그 기록
	sql.Format("INSERT INTO TBL_PointLog ( ptl_point_code, ptl_user_idx, ptl_char_idx, ptl_server, ptl_prev_point, ptl_get_point, ptl_point, ptl_desc, ptl_insert_time ) "
		"VALUES( %d, %d, %d, %d, %d, %d, %d, '%s', NOW() )" , web_code , user_index, char_index,
#ifdef ADULT_SERVER
		101,
#else
		1,
#endif 
		nPrePoint, nPoint, nPrePoint+nPoint, desc[type] );
	eventdb_cmd.SetQuery(sql);
	eventdb_cmd.Update();

	int delete_get_point=0;
	if( type == MSG_IPOINT_TYPE_DELETE_CHAR ) // 캐릭터 삭제
	{
		sql.Format("SELECT * FROM TBL_CharacterPoint"
#ifdef ADULT_SERVER
					"_apron "
#else
					"_bliss "
#endif //ADULT_SERVER
					" WHERE cpt_user_idx=%d AND cpt_char_idx=%d " , user_index, char_index );
		eventdb_cmd.SetQuery(sql);
		if( eventdb_cmd.Open() && eventdb_cmd.MoveFirst() )
		{
			eventdb_cmd.GetRec("cpt_get_point", delete_get_point );			
		}

		sql.Format("UPDATE TBL_CharacterPoint"
#ifdef ADULT_SERVER
					"_apron "
#else
					"_bliss "
#endif //ADULT_SERVER
					"SET cpt_get_point=0, cpt_update_date=NOW(), cpt_del_point=%d WHERE cpt_user_idx=%d AND cpt_char_idx=%d " 
					,delete_get_point, user_index, char_index );
		eventdb_cmd.SetQuery(sql);
		eventdb_cmd.Update();

		sql.Format("UPDATE TBL_MemberPoint SET mpt_get_point=mpt_get_point-%d, mpt_update_date=NOW() WHERE mpt_user_idx=%d ",
					delete_get_point , user_index ); 
		eventdb_cmd.SetQuery(sql);
		eventdb_cmd.Update();
	}
	else if ( type == MSG_IPOINT_TYPE_CANCEL_DELETE_CHAR ) // 캐릭터 삭제 취소
	{
		sql.Format("SELECT * FROM TBL_CharacterPoint"
#ifdef ADULT_SERVER
					"_apron "
#else
					"_bliss "
#endif //ADULT_SERVER
					" WHERE cpt_user_idx=%d AND cpt_char_idx=%d " , user_index, char_index );
		eventdb_cmd.SetQuery(sql);
		if( eventdb_cmd.Open() && eventdb_cmd.MoveFirst() )
		{
			eventdb_cmd.GetRec("cpt_del_point", delete_get_point );			
		}

		sql.Format("UPDATE TBL_CharacterPoint"
#ifdef ADULT_SERVER
					"_apron "
#else
					"_bliss "
#endif //ADULT_SERVER
					" SET cpt_get_point=%d, cpt_update_date=NOW(), cpt_del_point=0 WHERE cpt_user_idx=%d AND cpt_char_idx=%d " 
					, delete_get_point, user_index, char_index );
		eventdb_cmd.SetQuery(sql);
		eventdb_cmd.Update();

		sql.Format("UPDATE TBL_MemberPoint SET mpt_get_point=mpt_get_point+%d, mpt_update_date=NOW() WHERE mpt_user_idx=%d ",
					delete_get_point , user_index ); 
		eventdb_cmd.SetQuery(sql);
		eventdb_cmd.Update();
	}
	else
	{
		// 캐릭터 추가
		sql.Format("UPDATE TBL_CharacterPoint"
#ifdef ADULT_SERVER
					"_apron "
#else
					"_bliss "
#endif //ADULT_SERVER
					" SET cpt_point=cpt_point+%d , cpt_get_point=cpt_get_point+%d, cpt_update_date=NOW() WHERE cpt_user_idx=%d AND cpt_char_idx=%d " 
					, nPoint, nGetPoint, user_index, char_index );
		eventdb_cmd.SetQuery(sql);
		eventdb_cmd.Update();

		// 멤버에 추가
		sql.Format("UPDATE TBL_MemberPoint SET mpt_user_point=%d , mpt_get_point=mpt_get_point+%d, mpt_update_date=NOW() WHERE mpt_user_idx=%d ",
					nPrePoint+nPoint , nGetPoint , user_index ); 
		eventdb_cmd.SetQuery(sql);
		eventdb_cmd.Update();	
	}

	GAMELOG << init("ADD IRIS POINT : " ) << " UserIndex:" << user_index << " Point:" << nPoint
			<< " Type:" <<  type << end;
	

	return true;
}

void ConnIpointAddMsg( CDescriptor* d, CNetMsg& msg )
{
	int user_index, char_index, type;
	int nIpoint;
	msg >> user_index >> char_index >> type >> nIpoint;

	CDBCmd eventdb_cmd;
	CLCString sql(1024);
	eventdb_cmd.Init(&gserver.m_dbevent);

	bool bAddPoint = false;

	switch( type )
	{
	case MSG_IPOINT_TYPE_ATTENDANCE :	// 출석 체크 
		{
			// 출석  pnt_code : 2
			// 캐릭터 정보 없으면 캐릭터 정보 저장
			int guild_index;
			CLCString char_name(MAX_CHAR_NAME_LENGTH+1);
			msg >> guild_index >> char_name;

			sql.Format("SELECT * FROM TBL_CharacterPoint"
#ifdef ADULT_SERVER
				"_apron "
#else
				"_bliss "
#endif //ADULT_SERVER
				" WHERE cpt_user_idx=%d AND cpt_char_idx=%d"
				, user_index, char_index );
			eventdb_cmd.SetQuery(sql);
			if( eventdb_cmd.Open() && eventdb_cmd.MoveFirst() )
			{
				sql.Format("UPDATE TBL_CharacterPoint"
#ifdef ADULT_SERVER
				"_apron "
#else
				"_bliss "
#endif //ADULT_SERVER
				"SET cpt_guild_idx=%d, cpt_char_name='%s' ,cpt_update_date=NOW() WHERE cpt_user_idx=%d AND cpt_char_idx=%d"
				, guild_index, (const char*) char_name, user_index, char_index );
				eventdb_cmd.SetQuery(sql);
				eventdb_cmd.Update();
			}
			else
			{
				sql.Format("INSERT INTO TBL_CharacterPoint"
#ifdef ADULT_SERVER
				"_apron "
#else
				"_bliss "
#endif //ADULT_SERVER
				" ( cpt_user_idx, cpt_char_idx, cpt_guild_idx, cpt_char_name, cpt_update_date ) VALUES "
				" ( %d, %d, %d, '%s', NOW() )",
				user_index, char_index, guild_index, (const char*)char_name );
				eventdb_cmd.SetQuery(sql);
				eventdb_cmd.Update();
			}

			sql.Format("SELECT * FROM TBL_PlayTime WHERE pla_user_idx=%d AND TO_DAYS(pla_insert_time)=TO_DAYS(CURDATE()) ", user_index );
			eventdb_cmd.SetQuery(sql);
			if( eventdb_cmd.Open() )
			{
				if( !eventdb_cmd.MoveFirst() )
				{
					int acc_count =0;
					sql.Format(" SELECT pla_acc_count FROM TBL_PlayTime WHERE pla_user_idx=%d "
						" AND TO_DAYS(pla_insert_time) = TO_DAYS( DATE_SUB( NOW(), INTERVAL 1 DAY ) )", user_index );
					eventdb_cmd.SetQuery(sql);
					if( eventdb_cmd.Open() && eventdb_cmd.MoveFirst() )
					{
						eventdb_cmd.GetRec("pla_acc_count",acc_count );
						acc_count++;
						if( acc_count > 5)
							acc_count=5;
					}

					sql.Format(" INSERT INTO TBL_PlayTime ( pla_user_idx, pla_play_time, pla_acc_count, pla_insert_time )"
						" VALUES ( %d, 0, %d, NOW() )", user_index , acc_count );
					eventdb_cmd.SetQuery(sql);
					eventdb_cmd.Update();

					if( acc_count >= 5 )
						bAddPoint = AddIpoint( user_index, char_index, MSG_IPOINT_TYPE_ATTENDANCE, 30 );	// 5일이상 출석시 20개 추가
					else
						bAddPoint = AddIpoint( user_index, char_index, MSG_IPOINT_TYPE_ATTENDANCE, 10 );	//출석 10개

				}
			}
			return;
		}
		break;
	case MSG_IPOINT_TYPE_PLAYTIME:	// 접속 누적 시간
		{
			return;
			/*
			AddIpoint( user_index, char_index, MSG_IPOINT_TYPE_PLAYTIME, 1 );
			sql.Format(" UPDATE TBL_PlayTime SET pla_play_time=pla_play_time+60 , pla_insert_time=NOW()"
						"WHERE pla_user_idx=%d AND TO_DAYS(pla_insert_time)=TO_DAYS(CURDATE()) ", user_index );
			eventdb_cmd.SetQuery(sql);

			if( !eventdb_cmd.Update() )		// 24시가 지나면 데이타가 없다.
			{
				int acc_count =0;
				sql.Format(" SELECT pla_acc_count FROM TBL_PlayTime WHERE pla_user_idx=%d "
					" AND TO_DAYS(pla_insert_time) = TO_DAYS( DATE_SUB( NOW(), INTERVAL 1 DAY ) )", user_index );
				eventdb_cmd.SetQuery(sql);
				if( eventdb_cmd.Open() && eventdb_cmd.MoveFirst() )
				{
					eventdb_cmd.GetRec("pla_acc_count",acc_count );
					acc_count++;
					if( acc_count > 5)
						acc_count=5;
				}

				// 플레이타임 10분 지급
				sql.Format(" INSERT INTO TBL_PlayTime ( pla_user_idx, pla_play_time, pla_acc_count, pla_insert_time )"
					" VALUES ( %d, 60, %d, NOW() )", user_index , acc_count );
				eventdb_cmd.SetQuery(sql);
				eventdb_cmd.Update();

				if( acc_count >= 5 )
					AddIpoint( user_index, char_index, MSG_IPOINT_TYPE_ATTENDANCE, 30 );	// 5일이상 출석시 20개 추가
				else
					AddIpoint( user_index, char_index, MSG_IPOINT_TYPE_ATTENDANCE, 10 );	//출석 10개

			}
			*/
		}
		break;
	case MSG_IPOINT_TYPE_TEACHER :		
	case MSG_IPOINT_TYPE_LEVELUP :
	case MSG_IPOINT_TYPE_PD4:
	case MSG_IPOINT_TYPE_DELETE_CHAR:
	case MSG_IPOINT_TYPE_CANCEL_DELETE_CHAR:
	case MSG_IPOINT_TYPE_USEITEM:
	default:
		{								// Game Play : 3
			bAddPoint = AddIpoint( user_index, char_index, type, nIpoint );
		}
		break;
	}

	// 유저에게 포인트 알림메세지
	if( bAddPoint && nIpoint > 0 )
	{
		CNetMsg ipointMsg;
		ConnIpointMessageMsg( ipointMsg, char_index, nIpoint );
		SEND_Q(ipointMsg, d);
	}
}
#endif //IRIS_POINT

#ifdef EVENT_NOM_MOVIE
void ConnEventNomMsg(CDescriptor* d, CNetMsg& msg)
{
	int nUserIdx = 0;
	int nCharIdx = 0;
	unsigned char nSubType = 0;

	msg >> nSubType >> nUserIdx >> nCharIdx;

	// 응모 요청
	if (MSG_CONN_EVENT_NOM_REQ == nSubType)
	{
		CDBCmd cmd;
		cmd.Init(&gserver.m_dbuser);
		
		CLCString query(1024);
		query.Format("select unix_timestamp(a_regi_date) as regi_date from t_users where a_portal_index = %d", nUserIdx);
		cmd.SetQuery(query);

		if (!cmd.Open() || !cmd.MoveFirst())
		{
			GAMELOG << init("EVENT_NOM_FAIL-regi_date : ") << "USER" << delim << nUserIdx << end;
			return;
		}
		
		int nRegTiem = 0;
		cmd.GetRec("regi_date", nRegTiem);
		
		// 이벤트 기간 : 2008년 7월09일 ~ 7월31일
		if (nRegTiem < 1215529200 || nRegTiem > 1217430000)
		{
			GAMELOG << init("EVENT_NOM_FAIL-No Event Date : ") << "USER" << delim << nUserIdx << end;
			return;
		}
		
		CDBCmd eventCmd;
		eventCmd.Init(&gserver.m_dbevent);
		
		CLCString event_query(1024);
		event_query.Format("select lottery_yn, unix_timestamp(lottery_date) as lotteryDate, lottery_give from t_event_nom where user_index = %d", nUserIdx);
		eventCmd.SetQuery(event_query);
		
		if (!eventCmd.Open())
		{
			GAMELOG << init("EVENT_NOM_FAIL-select event : ") << "USER" << delim << nUserIdx << end;
			return;
		}
		
		int nLotteryYN		= 0;
		int nLotteryDate	= 0;
		int nLotteryGive	= 1;
		
		if (eventCmd.m_nrecords > 0 && eventCmd.MoveFirst())
		{
			eventCmd.GetRec("lottery_yn",	nLotteryYN	);
			eventCmd.GetRec("lotteryDate",	nLotteryDate);
			eventCmd.GetRec("lottery_give", nLotteryGive);
		}

		if (nLotteryDate > 0)
		{
			CNetMsg rmsg;
			ConnEventNomMsg(rmsg, nCharIdx, MSG_CONN_EVENT_ERROR_ALREADY_APPLY);
			SEND_Q(rmsg, d);
		}

		// 복권 미지급 시 - 지급 : 1, 미지급 : 0
		if (nLotteryGive == 0)
		{
			event_query.Format("update t_event_nom set lottery_give = 1 where user_index = %d", nUserIdx);
			eventCmd.SetQuery(event_query);
			if (!eventCmd.Update())
			{
				GAMELOG << init("EVENT_NOM_FAIL-update event : ") << "USER" << delim << nUserIdx << end;
				return;
			}
		}
		else
		{
			event_query.Format("insert into t_event_nom values (%d, %d, 0, NOW(), 1)", nUserIdx, nCharIdx);
			eventCmd.SetQuery(event_query);
			if (!eventCmd.Update())
			{
				GAMELOG << init("EVENT_NOM_FAIL-insert event : ") << "USER" << delim << nUserIdx << end;
				return;
			}
		}

		CNetMsg rmsg;
		ConnEventNomMsg(rmsg, nCharIdx, MSG_CONN_EVENT_NOM_OK);
		SEND_Q(rmsg, d);

		return;
	}
	
	// 복권 미지급
	if (MSG_CONN_EVENT_NOM_LOTTERY_NO_GIVE == nSubType)
	{
		CDBCmd cmd;
		CLCString query(1024);

		query.Format("update t_event_nom set lottery_give = 0 where user_index = %d", nUserIdx);
		cmd.Init(&gserver.m_dbuser);
		cmd.SetQuery(query);
		if (!cmd.Update())
		{
			GAMELOG << init("EVENT_NOM_FAIL-LOTTERY_NO_GIVE : ") << "USER" << delim << nUserIdx << end;
			return;
		}
	}
	else if (MSG_CONN_EVENT_NOM_MOVIE_TICKET_REQ == nSubType)
	{
		CDBCmd cmd;
		CLCString query(1024);

		cmd.Init(&gserver.m_dbevent);
/*
		// 티켓 등록 여부
		query.Format("select count(*) as tkt_cnt from TBL_Ticket where tkt_user_idx = %d", nUserIdx);
		cmd.SetQuery(query);
		
		if (!cmd.Open() || !cmd.MoveFirst())
			return;
		
		int nCount = 0;
		cmd.GetRec("tkt_cnt", nCount);
		
		if (nCount > 0)
			return;
*/
		// 티켓 사용
		query.Format("update TBL_Ticket set tkt_user_idx = %d, tkt_issue_date = NOW(), tkt_used = 'Y' where tkt_used = 'N' and tkt_game_type = 'LC' limit 1", nUserIdx);
		cmd.SetQuery(query);
		if (!cmd.Update())
		{
			GAMELOG << init("EVENT_NOM_FAIL: ") << "USER" << delim << nUserIdx << end;
			return;
		}

		CNetMsg rmsg;
		ConnEventNomMsg(rmsg, nCharIdx, MSG_CONN_EVENT_NOM_MOVIE_TICKET_OK);
		SEND_Q(rmsg, d);
	}
}
#endif

#ifdef REQUITAL_EVENT	// 보상 이벤트 
void ConnEventRequitalMsg(CDescriptor* d, CNetMsg& msg)
{
	int nUserIdx = 0;
	int nCharIdx = 0;
	int nCheck_eventType= -1;
	CLCString User_name(20);
	CLCString Char_name(20);

	msg >> nUserIdx >> nCharIdx >> nCheck_eventType >> User_name >> Char_name;

	// 서버 오류 보상 검색
	if ( nCheck_eventType == 0 )
	{
		CDBCmd userCmd;
		userCmd.Init(&gserver.m_dbuser);
		
		CLCString query(1024);
		query.Format("select * from t_requital_check0 where a_user_index = %d", nUserIdx);
		userCmd.SetQuery(query);

		// 없으면 실패 
		if (!userCmd.Open() || !userCmd.MoveFirst())
		{
			CNetMsg rmsg;
			ConnEventRequitalMsg(rmsg, nCharIdx, nCheck_eventType, MSG_CONN_EVENT_REQUITAL_FAIL);
			SEND_Q(rmsg, d);
			return;
		}
	} // if

	// 출석 체크 보상 검색
	else if ( nCheck_eventType == 1 )
	{
		CDBCmd userCmd;
		userCmd.Init(&gserver.m_dbuser);
		
		CLCString query(1024);
		query.Format("select * from t_requital_check1 where a_user_index = %d", nUserIdx);
		userCmd.SetQuery(query);
		
		if (!userCmd.Open() || !userCmd.MoveFirst())
		{
			CNetMsg rmsg;
			ConnEventRequitalMsg(rmsg, nCharIdx, nCheck_eventType, MSG_CONN_EVENT_REQUITAL_FAIL);
			SEND_Q(rmsg, d);
			return;
		}
	} // else if

	int eventCheck_0= 0;
	int eventCheck_1= 0;
	CDBCmd userCmd2;
	userCmd2.Init(&gserver.m_dbuser);

	CLCString user_query2(1024);
	user_query2.Format("select a_user_idx, a_eventCheck_0, a_eventCheck_1 from t_requital_log where a_user_idx = %d", nUserIdx);
	userCmd2.SetQuery(user_query2);
	
	if ( !userCmd2.Open() )
	{
		GAMELOG << init("EVENT_REQUITAL_FAIL-select user 2 : ") << "USER" << delim << nUserIdx << end;
		return;
	}
	else if ( userCmd2.MoveFirst() && userCmd2.m_nrecords > 0 )
	{
		userCmd2.GetRec("a_eventCheck_0", eventCheck_0);					// 보상이벤트 1
		userCmd2.GetRec("a_eventCheck_1", eventCheck_1);					// 보상이벤트 2

		// 보상 이벤트 0이면
		if( nCheck_eventType == 0 )
		{
			// 만약 받았으면 제외
			if ( eventCheck_0 > 0 )
			{
				CNetMsg rmsg;
				ConnEventRequitalMsg(rmsg, nCharIdx, nCheck_eventType, MSG_CONN_EVENT_REQUITAL_ALREADY_APPLY);
				SEND_Q(rmsg, d);
				return;
			}
		}

		// 보상 이벤트 1이면
		if ( nCheck_eventType == 1 )
		{
			// 만약 받았으면 제외
			if ( eventCheck_1 > 0 )
			{
				CNetMsg rmsg;
				ConnEventRequitalMsg(rmsg, nCharIdx, nCheck_eventType, MSG_CONN_EVENT_REQUITAL_ALREADY_APPLY);
				SEND_Q(rmsg, d);
				return;
			}
		}
	}

	// 여기까지 오면 성공인 유저.. 각 상황에 따라 쿼리가 달라짐
	CLCString update_query(1024);
	CDBCmd updateCmd;
	updateCmd.Init(&gserver.m_dbuser);

	// 만약 로그에서 검색이 되었다면
	if ( userCmd2.m_nrecords )
	{
		// 보상이벤트 0을 받았고 1를 요청한 것이면
		if ( eventCheck_0 > 0 && nCheck_eventType == 1 )
		{
			update_query.Format("update t_requital_log SET a_eventCheck_1= %d where a_user_idx = %d", 1, nUserIdx);
		}

		// 보상이벤트 1를 받았고 0을 요청한 것이면
		else if ( eventCheck_1 > 0 && nCheck_eventType == 0 )
		{
			update_query.Format("update t_requital_log SET a_eventCheck_0= %d where a_user_idx = %d", 1, nUserIdx);
		}	
	}
	else // 검색이 없으면 데이터를 넣어 줌
	{
		// 보상 이벤트 0
		if ( nCheck_eventType == 0 )
		{
			update_query.Format("insert into t_requital_log (a_user_idx, a_user_name, a_char_idx, a_char_name, a_eventCheck_0) values (%d, '%s', %d, '%s', %d) ", nUserIdx, (const char*)User_name ,nCharIdx , (const char*)Char_name, 1);
		}

		// 보상 이벤트 1
		else if ( nCheck_eventType == 1 )
		{
			update_query.Format("insert into t_requital_log (a_user_idx, a_user_name, a_char_idx, a_char_name, a_eventCheck_1) values (%d, '%s', %d, '%s', %d) ", nUserIdx, (const char*)User_name ,nCharIdx , (const char*)Char_name, 1);
		}
	}
	
	updateCmd.SetQuery(update_query);

	updateCmd.BeginTrans();
	if( !updateCmd.Update() )
	{
		// 업데이트 실패임 다시~
		updateCmd.Rollback();
		CNetMsg rmsg;
		ConnEventRequitalMsg(rmsg, nCharIdx, nCheck_eventType, MSG_CONN_EVENT_REQUITAL_FAIL);
		SEND_Q(rmsg, d);
		return;
	}
	updateCmd.Commit();

	// 성공~
	CNetMsg rmsg;
	ConnEventRequitalMsg(rmsg, nCharIdx, nCheck_eventType, MSG_CONN_EVENT_REQUITAL_OK);
	SEND_Q(rmsg, d);
	return;
}
#endif // REQUITAL_EVENT

#if defined (CHAR_LOG) && defined (LC_KOR)
void ConnCharLogMsg( CDescriptor* d, CNetMsg& msg)
{
	int			char_index;
	int			user_index;
	CLCString	id_name(MAX_ID_NAME_LENGTH+1);
	CLCString	char_name(MAX_CHAR_NAME_LENGTH + 1);
	CLCString	char_nick(MAX_CHAR_NAME_LENGTH + 1);
	CLCString	guild_name( MAX_GUILD_NAME_LENGTH+1 );
	int			char_level;
	LONGLONG	char_exp;
	char		char_job, char_job2;
	int			char_str, char_dex, char_int, char_con;
	int			char_sp;
	LONGLONG	char_money;
	int			char_teach_idx;

	msg >> char_index
		>> user_index
		>> id_name
		>> char_name
		>> char_nick
		>> guild_name
		>> char_level
		>> char_exp
		>> char_job
		>> char_job2
		>> char_str
		>> char_dex
		>> char_int
		>> char_con
		>> char_sp
		>> char_money
		>> char_teach_idx;

	int create_server = gserver.m_serverno;
#ifdef ADULT_SERVER 
	create_server += 10;
#endif //ADULT_SERVER

	CDBCmd cmd;
	cmd.Init(&gserver.m_dblog);
		
	CLCString query(1024);


	query.Format("SELECT * FROM t_character_log WHERE a_index=%d", char_index );
	cmd.SetQuery(query);
	if( cmd.Open() && cmd.MoveFirst() )
	{
#ifdef CIRCLE_WINDOWS
		query.Format("UPDATE t_character_log SET a_datestamp=NOW(), a_nick='%s', a_guild_name='%s', a_level=%d, a_exp=%I64d, "
			" a_job2=%d, a_str=%d, a_dex=%d, a_int=%d, a_con=%d, a_skill_point=%d, a_nas=%I64d , a_teach_index=%d WHERE a_index = %d ", 
			 (const char*)char_nick, (const char*)guild_name, char_level, char_exp, char_job2, char_str, char_dex, char_int, char_int,
			char_sp, char_money, char_teach_idx,  char_index );
#else
		query.Format("UPDATE t_character_log SET a_datestamp=NOW(), a_nick='%s', a_guild_name='%s', a_level=%d, a_exp=%lld, "
			" a_job2=%d, a_str=%d, a_dex=%d, a_int=%d, a_con=%d, a_skill_point=%d, a_nas=%lld , a_teach_index=%d WHERE a_index = %d ", 
			 (const char*)char_nick, (const char*)guild_name, char_level, char_exp, char_job2, char_str, char_dex, char_int, char_int,
			char_sp, char_money, char_teach_idx,  char_index );
#endif
		cmd.SetQuery(query);
		cmd.Update();
	}
	else
	{
#ifdef CIRCLE_WINDOWS
		query.Format("INSERT INTO t_character_log ( "
			" a_index, a_user_index, a_user_id, a_server, a_datestamp, a_name, a_nick, a_guild_name, "
			" a_level, a_exp , a_job, a_job2, a_str, a_dex, a_int, a_con, a_skill_point, a_nas, a_teach_index ) VALUES ( "
			" %d , %d, '%s', %d, NOW(), '%s', '%s', '%s', %d, %I64d, %d, %d, %d, %d, %d, %d, %d, %I64d, %d );" , 
#else
		query.Format("INSERT INTO t_character_log ( "
			" a_index, a_user_index, a_user_id, a_server, a_datestamp, a_name, a_nick, a_guild_name, "
			" a_level, a_exp , a_job, a_job2, a_str, a_dex, a_int, a_con, a_skill_point, a_nas, a_teach_index ) VALUES ( "
			" %d , %d, '%s', %d, NOW(), '%s', '%s', '%s', %d, %lld, %d, %d, %d, %d, %d, %d, %d, %lld, %d ); " , 
#endif
			char_index, user_index,  (const char*)id_name, create_server, (const char*)char_name, (const char*)char_nick, (const char*)guild_name,
			char_level,	char_exp, char_job, char_job2, char_str, char_dex, char_int, char_con,  char_sp, char_money, char_teach_idx );
		cmd.SetQuery(query);
		cmd.Update();
	}
}
#endif //#if defined (CHAR_LOG) && defined (LC_KOR)
