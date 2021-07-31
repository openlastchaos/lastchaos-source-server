#include "stdhdrs.h"
#include "Log.h"
#include "Descriptor.h"
#include "Server.h"
#include "CmdMsg.h"
#include "DBCmd.h"
#include "Utils.h"
#include "IPCheck.h"

CDescriptor::CDescriptor()
: m_idname(MAX_ID_NAME_LENGTH + 1)
, m_passwd(MAX_PWD_LENGTH + 1)
#ifdef CHECK_SECURE_CARD
, m_cardstr(47 + 1)
#endif // CHECK_SECURE_CARD
, m_host(HOST_LENGTH + 1)
{
	m_index = -1;

	m_serverNo = 0;
	m_nMaxServer = 0;

	m_playerNum = NULL;
	m_serverSubNo = NULL;
	m_ipAddr = NULL;
	m_portNumber = NULL;

	m_desc = 0;

#ifdef CRYPT_NET_MSG

	m_bCryptNetMsg = true;
	CNM_InitKeyValue(&m_nCryptKey);

#ifndef CRYPT_NET_MSG_MANUAL
	CNM_InitKeyValue(&m_nCryptKeyClient);
#endif // #ifndef CRYPT_NET_MSG_MANUAL

#endif // #ifdef CRYPT_NET_MSG

	m_pulseConnected = 0;

	m_timestamp = -1;

	m_connected = 0;
	m_connectreq = 0;
	m_bclosed	= false;

	m_pPrev = NULL;
	m_pNext = NULL;



#ifdef CHECK_SECURE_CARD
	for(int i = 0; i < 4; i++)
	{
		m_scard[i] = 0;
	}
#endif // CHECK_SECURE_CARD
}

CDescriptor::~CDescriptor()
{
	delete [] m_playerNum;
	delete [] m_serverSubNo;
	delete [] m_ipAddr;
	delete [] m_portNumber;
}

void CDescriptor::CloseSocket()
{
	CLOSE_SOCKET(m_desc);
	FlushQueues();
}

void CDescriptor::FlushQueues()
{
	m_inBuf.Clear();
	m_inQ.Clear();
	m_outBuf.Clear();
}

void CDescriptor::WriteToOutput(CNetMsg& msg)
{
	if (msg.m_mtype == MSG_UNKNOWN)
		return ;
#ifdef CRYPT_NET_MSG
	if (!m_outBuf.Add(msg, m_bCryptNetMsg, &m_nCryptKey))
#else
	if (!m_outBuf.Add(msg))
#endif // #ifdef CRYPT_NET_MSG
	{
		GAMELOG << init("OUTPUT OVERFLOW")
				<< end;

		m_bclosed = true;
		return ;
	}
#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_NextKey(&m_nCryptKey);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // CRYPT_NET_MSG
	ProcessOutput();
}

int CDescriptor::ProcessOutput()
{
	int result;
	unsigned char* p;

	while ((p = m_outBuf.GetNextPoint()))
	{
		result = WriteToDescriptor(m_desc, (char*)p, m_outBuf.GetRemain());
		if (result < 0)
		{
			m_bclosed = true;
			return -1;
		}
		else if (result == 0 || m_outBuf.Update(result))
			return 0;
	}

	return 0;
}

int CDescriptor::WriteToDescriptor(socket_t m_desc, const char* buf, int length)
{
	int nWrite;

	if (length == 0)
		return 0;

#ifdef CIRCLE_WINDOWS
	if ( (nWrite = send(m_desc, buf, length, 0)) <= 0)
	{
		if (nWrite == 0) return -1;
		if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAEINTR)
			return 0;
#else
	if ( (nWrite = write(m_desc, buf, length)) <= 0)
	{
		if (nWrite == 0) return -1;
	#ifdef EAGAIN		/* POSIX */
		if (errno == EAGAIN) return 0;
	#endif

	#ifdef EWOULDBLOCK	/* BSD */
		if (errno == EWOULDBLOCK) return 0;
	#endif
#endif
		if (errno != 104)
		{
			GAMELOG << init("SYS_ERR")
					<< "WriteToDescriptor : no ="
					<< errno
					<< end;
		}
		return -1;
	}

	return nWrite;
}

int CDescriptor::ProcessInput()
{
	bool bGetPacket = false;
	ssize_t bytes_read;

	do
	{
		if (m_inBuf.GetRemain() <= 0)
		{
			GAMELOG << init("SYS_WARN")
					<< "m_input overflow"
					<< end;
			return -1;
		}

		bytes_read = PerformSocketRead(m_desc, (char*)m_inBuf.GetPoint(), m_inBuf.GetRemain());

		if (bytes_read < 0)	/* Error, disconnect them. */
			return -1;
		else if (bytes_read == 0)	/* Just blocking, no problems. */
			return 0;

		m_inBuf.SetPoint(bytes_read);

		/* at this point, we know we got some data from the read */

		// Packet 얻기
		CNetMsg m;
		bool bStop = false;
		bool bFail = false;
		while (!bStop)
		{
			switch (m_inBuf.GetMessage(m))
			{
			case 0:
				m_inQ.Add(m);
				bGetPacket = true;
				break;
			case 1:
				bStop = true;
				break;
			case -1:
				bFail = true;
				bStop = true;
				break;
			}
		}

		if (bFail)
		{
			return -1;
		}

		/*
		 * on some systems such as AIX, POSIX-standard nonblocking I/O is broken,
		 * causing the MUD to hang when it encounters m_input not terminated by a
		 * newline.  This was causing hangs at the Password: prompt, for example.
		 * I attempt to compensate by always returning after the _first_ read, instead
		 * of looping forever until a read returns -1.  This simulates non-blocking
		 * I/O because the result is we never call read unless we know from select()
		 * that data is ready (process_input is only called if select indicates that
		 * this descriptor is in the read set).  JE 2/23/95.
		 */
#if !defined(POSIX_NONBLOCK_BROKEN)
	} while (!bGetPacket);
#else
	} while (0);

	if (!bGetPacket)
		return (0);
#endif /* POSIX_NONBLOCK_BROKEN */

	return 1;
}

ssize_t CDescriptor::PerformSocketRead(socket_t m_desc, char *read_point, size_t space_left)
{
	ssize_t ret;

#if defined(CIRCLE_ACORN)
	ret = recv(m_desc, read_point, space_left, MSG_DONTWAIT);
#elif defined(CIRCLE_WINDOWS)
	ret = recv(m_desc, read_point, space_left, 0);
#else
	ret = read(m_desc, read_point, space_left);
#endif

	/* Read was successful. */
	if (ret > 0) return ret;

	/* read() returned 0, meaning we got an EOF. */
	if (ret == 0) return -1;

	/*
	* read returned a value < 0: there was an error
	*/

#if defined(CIRCLE_WINDOWS)	/* Windows */
	if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAEINTR)
		return 0;
#else

#ifdef EINTR		/* Interrupted system call - various platforms */
	if (errno == EINTR)
		return 0;
#endif

#ifdef EAGAIN		/* POSIX */
	if (errno == EAGAIN)
		return 0;
#endif

#ifdef EWOULDBLOCK	/* BSD */
	if (errno == EWOULDBLOCK)
		return 0;
#endif /* EWOULDBLOCK */

#ifdef EDEADLK		/* Macintosh */
	if (errno == EDEADLK)
		return 0;
#endif

#endif /* CIRCLE_WINDOWS */

	/*
	* We don't know what happened, cut them off. This qualifies for
	* a SYSERR because we have no idea what happened at this point.
	*/
	return -1;
}

bool CDescriptor::GetLogin(CNetMsg& msg)
{
#ifdef CHECK_SECURE_CARD
	if (msg.m_mtype == MSG_EXTEND)
	{
		return GetSCard(msg);
	}
#endif // CHECK_SECURE_CARD
	CNetMsg failmsg;

#ifdef BRZ_FAIL_PASSWORD
	bool bSaveFailCount = false;
#endif // BRZ_FAIL_PASSWORD

	try
	{

// 050226 : bs : 국내와 중국만 아이피 검사
#if defined(LC_KOR) || defined(LC_CHN)
		if (!CheckIP(m_host))
		{
			GAMELOG << init("SYS_ERR")
					<< "BLOCK IP"
					<< m_host
					<< end;
			throw MSG_FAIL_LOGINSERV_NO_SERVICE;

		}
#endif
// --- 050226 : bs : 국내와 중국만 아이피 검사

		msg.MoveFirst();

		if (msg.m_mtype != MSG_LOGIN)
			throw MSG_FAIL_LOGIN_SERVER;		// 잘못된 입력

		//if (gserver.m_playerList.m_cur > MAX_PLAYER_LIST)
		//	throw MSG_FAIL_MANY_CONNECT;		// 접속 요청 초과

		int version;
		unsigned char mode;
		unsigned char nation;	// 국가 코드 체크 
		CLCString id(MAX_ID_NAME_LENGTH + 1);
		CLCString pw(MAX_PWD_LENGTH + 1);

#ifdef CHECKIP_USA
		int IsUSA;
#endif // CHECKIP_USA

		msg >> version
			>> mode
			>> id
			>> pw
			>> nation;

#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
		unsigned int nSeed;
		msg >> nSeed;
		CNM_MakeKeyFromSeed(&m_nCryptKeyClient, nSeed);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // CRYPT_NET_MSG

#ifndef _DEBUG 
#ifndef _CONSOLE
		if (version < gserver.m_clientversionMin || version > gserver.m_clientversionMax)
			throw MSG_FAIL_LOGINSERV_WRONG_VERSION;
#endif
#endif

		if( nation != gserver.m_national )
		{
			GAMELOG << init("INVALID_NATION", id)
					<< nation
					<< end;
			throw MSG_FAIL_WRONG_VERSION;
		}

		if (gserver.m_bOnlyLocal)
		{
#ifdef LC_KOR
			// 특수 계정이 아니고
			if (strcmp(id, "guest01") && strcmp(id, "guest02") && strcmp(id, "guest03") && strcmp(id, "cbx030"))
			{
#endif // LC_KOR
				// 티엔터 PC 방도 아니고
				if (strncmp(m_host, "61.104.44.", strlen("61.104.44.")) 
					&& strncmp(m_host, "222.106.51.", strlen("222.106.51."))
					//&& strncmp(m_host, "211.112.36.", strlen("211.112.36.")) )		//운영팀 추가
					&& strncmp(m_host, "124.137.195.", strlen("124.137.195.")) )		// 운영팀 IP 변경 080404
				{
					// GM 계정도 아니고
					if (strcmp(gserver.m_config.Find("GM ID", id), "") == 0)
					{
						int nIP[4];
						int nNetOrder = 0;
						IPtoi(m_host, nIP + 0, nIP + 1, nIP + 2, nIP + 3);
						nNetOrder	= ((nIP[0] << 24) & 0xff000000)
									+ ((nIP[1] << 16) & 0x00ff0000)
									+ ((nIP[2] <<  8) & 0x0000ff00)
									+ ((nIP[3]      ) & 0x000000ff);
						if ((nNetOrder & gserver.m_nInternalSubnetMask) != gserver.m_nInternalIPFilter)
						{
							GAMELOG << init("SYS_ERR")
									<< "ONLY LOCAL"
									<< m_host
									<< end;
							throw MSG_FAIL_LOGINSERV_NO_SERVICE;
						}
					}
				}
#ifdef LC_KOR
			}
#endif // LC_KOR
		}

#ifdef IP_BLOCK_AUTO
		LONGLONG regdate;
		if( strcmp(pw, "releaseblock") == 0)
		{
			AUTO_BLOCK_INFO* p = gserver.m_autoBlockTable;
			while(p)
			{
				if( strcmp(id, p->ip) == 0 )
				{
					if(p->prev)
					{
						p->prev->next = p->next;
						if(p->next) p->next->prev = p->prev;
					}
					else
					{
						gserver.m_autoBlockTable = p->next;
						if( gserver.m_autoBlockTable ) gserver.m_autoBlockTable->prev = NULL;
					}
				}
				p = p->next;
			}
		}
#endif

		// 로긴 모드 에러
		//if (mode != MSG_LOGIN_NEW)
		//	throw MSG_FAIL_LOGIN_NEW;

		if (strinc(id, "'"))
			throw MSG_FAIL_LOGINSERV_WRONG_CHAR;			// 아이디에 ' 들어감

		int len = strlen(id);
		if (len < 3 || len > MAX_ID_NAME_LENGTH)
			throw MSG_FAIL_LOGINSERV_WRONG_CHAR;

		m_idname = id;

		
#ifdef BRZ_FAIL_PASSWORD
		int blocktime;
		int nowtime;
		int userindex;
		CLCString sqlblock(1024);

		sqlblock.Format("SELECT a_portal_index, a_block_time, UNIX_TIMESTAMP(NOW()) as now_time FROM t_users WHERE a_idname = '%s'", (const char*)m_idname);
		CDBCmd dbUser_block;
		dbUser_block.Init(&gserver.m_dbuser);
		dbUser_block.SetQuery(sqlblock);
		if(dbUser_block.Open())
		{
			if(dbUser_block.MoveFirst())
			{
				if(dbUser_block.GetRec("a_portal_index", userindex)
				&& dbUser_block.GetRec("a_block_time", blocktime) 
				&& dbUser_block.GetRec("now_time", nowtime))
				{
					if(blocktime != 0 && blocktime > nowtime)
					{
						// 블럭됐음을 알림
						 throw MSG_FAIL_LOGINSERV_BLOCK_USER;
					}
					else if(blocktime != 0 && blocktime <= nowtime)
					{
						// 30분 시간초과 블럭해제
						sqlblock.Format("UPDATE t_users SET a_block_time = 0, a_fail_count = 0 WHERE a_portal_index = %d AND a_idname = '%s'", userindex, (const char*)m_idname);
						dbUser_block.SetQuery(sqlblock);
						if(dbUser_block.Update())
						{
							GAMELOG << init("UPDATE t_users BLOCK_TIME")
									<< m_idname
									<< end;
						}
						else
						{
							GAMELOG << init("FAILED t_users BLOCK_TIME")
									<< m_idname
									<< end;
						}
					}
				}
			}
		}
#endif // BRZ_FAIL_PASSWORD

		if (strinc(pw, "'"))
		{
#ifdef BRZ_FAIL_PASSWORD
			bSaveFailCount = true;
#endif // BRZ_FAIL_PASSWORD
			throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;		// 암호에 ' 들어감
		}

		len = strlen(pw);
		if (len < 4 || len > MAX_PWD_LENGTH)
		{
#ifdef BRZ_FAIL_PASSWORD
			bSaveFailCount = true;
#endif // BRZ_FAIL_PASSWORD
			throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;
		}

		m_passwd = pw;

		CLCString sql(1024);
		// 인증 DB 접속 ID, PW 검사
#ifdef LC_TLD
		sql.Format("select a_index, a_idname, a_passwd, a_gametype, a_specialsw, a_special from bg_user where a_idname='%s' and a_passwd='%s' and a_gametype = 'LC' ", (const char*)m_idname, (const char*)m_passwd);
#else // #ifdef LC_TLD
#ifdef EMPAS_LOGIN
#ifdef CHECK_LIMIT_AGE
#ifdef LC_KOR
		sql.Format("SELECT t1.user_code, t1.user_id, t1.passwd, t1.jumin FROM bg_user as t1 left join bg_user_active as t2 on t1.user_code = t2.active_code "
			"WHERE t1.user_id='%s' AND ( ( t1.passwd=password('%s') and t1.partner_id != 'B1' ) or (t2.active_code = t1.user_code and t2.active_game = 'LC' and t2.active_passwd = '%s' and unix_timestamp(t2.active_time) + 600 > unix_timestamp(now()) ) ) "
			, (const char*)m_idname, (const char*)m_passwd, (const char*)m_passwd);
#else
		sql.Format("SELECT t1.user_code, t1.user_id, t1.passwd, t1.jumin FROM bg_user as t1 left join bg_user_active as t2 on t1.user_code = t2.active_code "
			"WHERE t1.user_id='%s' AND ( ( t1.passwd=password('%s') and t1.partner_id != 'B1' ) or (t2.active_code = t1.user_code and t2.active_game = 'LC' and t2.active_passwd = '%s' and unix_timestamp(t2.active_time) + 600 > unix_timestamp(now()) ) ) "
			, (const char*)m_idname, (const char*)m_passwd, (const char*)m_passwd);
#endif //LC_KOR
#else // CHECK_LIMIT_AGE
		sql.Format("SELECT t1.user_code, t1.user_id, t1.passwd FROM bg_user as t1 left join bg_user_active as t2 on t1.user_code = t2.active_code "
			"WHERE t1.user_id='%s' AND ( ( t1.passwd='%s' and t1.partner_id != 'B1' ) or (t2.active_code = t1.user_code and t2.active_game = 'LC' and t2.active_passwd = '%s' and unix_timestamp(t2.active_time) + 600 > unix_timestamp(now()) ) ) "
			, (const char*)m_idname, (const char*)m_passwd, (const char*)m_passwd);
#endif // CHECK_LIMIT_AGE
#else //#ifdef EMPAS
		sql.Format("SELECT user_code, user_id, passwd FROM bg_user WHERE user_id='%s' AND passwd='%s'", (const char*)m_idname, (const char*)m_passwd);
#endif //#ifdef EMPAS
#if defined (LC_KOR ) || defined (LC_MAL)
		sql += " AND chk_service = 'Y' ";
#endif
		
#endif // #ifndef LC_TLD
		
		CDBCmd dbAuth;
		dbAuth.Init(&gserver.m_dbAuth);
		dbAuth.SetQuery(sql);
		
		if (!dbAuth.Open())
		{
			// 패스워드 틀림
#ifdef BRZ_FAIL_PASSWORD
			bSaveFailCount = true;
#endif // BRZ_FAIL_PASSWORD
			throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;
		}

		if (!dbAuth.MoveFirst())
		{
			// 패스워드 틀림
#ifdef BRZ_FAIL_PASSWORD
			bSaveFailCount = true;
#endif // BRZ_FAIL_PASSWORD
			throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;
		}

		int usercode;
#ifdef LC_TLD
		// 태국 미성년자 접속 금지
		char m_specialsw;
		char m_special;
		dbAuth.GetRec("a_index", usercode);	
		dbAuth.GetRec("a_specialsw", m_specialsw);	
		dbAuth.GetRec("a_special", m_special);
		
		if( m_specialsw && !m_special)
		{
			struct tm now = NOW();

			if( now.tm_hour >= NOTADULTMAX || now.tm_hour < NOTADULTMIN )
				throw MSG_FAIL_LOGINSERV_NOT_ADULT;
		}
#else
		dbAuth.GetRec("user_code", usercode);
#endif

#ifdef CHECK_LIMIT_AGE
		CLCString strJumin(20);
		bool bAllow = true;
		// 주민 번호를 가져오고
		if (dbAuth.GetRec("jumin", strJumin) && strJumin.Length() == 14)
		{
			if (!CheckBirthDay(strJumin, CHECK_LIMIT_AGE))
			{
				bAllow = false;
				GAMELOG << init("CHECK SID : DENY", m_idname)
						<< "SID" << delim
						<< strJumin
						<< end;
			}
		}
		else
		{
			GAMELOG << init("CHECK SID : SKIP", m_idname)
					<< "SID" << delim
					<< strJumin
					<< end;
		}
		// 회사는 나이 검사 로그만 남기고 통과
		if (!bAllow && strncmp(m_host, "61.104.44.", strlen("61.104.44."))
					//&& strncmp(m_host, "211.112.36.", strlen("211.112.36.")) )
					&& strncmp(m_host, "124.137.195.", strlen("124.137.195.")) ) // 운영팀 IP변경
			throw MSG_FAIL_LOGINSERV_WRONG_PASSWORD;
#endif // CHECK_LIMIT_AGE

		// IP 검사용
		CLCString temp(50);
		CLCString blockBand(50);

		// user DB

// 050226 : bs : a_enable 검사 추가
		sql.Format("SELECT unix_timestamp(a_regi_date) as a_regtime, a_enable, a_server_num, a_subnum, a_zone_num FROM t_users WHERE a_portal_index=%d", usercode);

		CNetMsg playerMsg;

		CDBCmd dbUser;
		dbUser.Init(&gserver.m_dbuser);
		dbUser.SetQuery(sql);

		if (!dbUser.Open())
			goto FIRST_CONNECT;

		if (!dbUser.MoveFirst())
			goto FIRST_CONNECT;

		int enable;
		if (!dbUser.GetRec("a_enable", enable) || enable != 1)
#ifdef SECURE_SYSTEM_HBK
		{
			if(enable == 0) throw MSG_FAIL_LOGINSERV_BLOCK_ACCOUNT;
			else if(enable == 2) throw MSG_FAIL_LOGINSERV_USE_SECURE_SYSTEM;
			else throw MSG_FAIL_LOGINSERV_BLOCK_CHAR;
		}
#else
			throw MSG_FAIL_LOGINSERV_BLOCK_CHAR;
#endif // SECURE_SYSTEM_HBK

// --- 050226 : bs : a_enable 검사 추가

		///////////////
		// 아이피 검사
#ifdef IP_BLOCK_BAND

		char blockTemp[50];
		int blockCount;

		char *token;

		temp = gserver.m_config.Find("Block IP", "Number");

		if(strcmp(temp, ""))
		{
			blockCount = atoi(temp);

			for(int j = 0; j < blockCount; j++)
			{
				// 클라이언트 ip와 비교

				temp.Format("IP%d", j);

				if(strcmp(this->m_host, gserver.m_config.Find("Block IP", temp)) == 0)
				{
					GAMELOG << init("BLOCK IP", id)
							<< m_host
							<< end;

					throw MSG_FAIL_CONNECT_SERVER;
				}

			}
		}

		// Band1=A-211.090.000.000-211.103.255.255
		temp = gserver.m_config.Find("Block Band", "Number");

		if(strcmp(temp, ""))
		{
			blockCount = atoi(temp);

			for(int j = 0; j < blockCount; j++)
			{
				// 클라이언트 ip와 비교

				temp.Format("Band%d", j);
				blockBand = gserver.m_config.Find("Block Band", temp);
				strcpy(blockTemp, blockBand);
				
				token = strtok(blockTemp, "-");		if(token == NULL) continue;
				temp = token;

				int ipStart[4] = {0, 0, 0, 0};
				int ipEnd[4] = {0, 0, 0, 0};
				int ipHost[4] = {0, 0, 0, 0};
				token = strtok(NULL, "-");			if(token == NULL) continue;
				IPtoi(token, ipStart, ipStart + 1, ipStart + 2, ipStart + 3);
				token = strtok(NULL, "-");			if(token == NULL) continue;
				IPtoi(token, ipEnd, ipEnd + 1, ipEnd + 2, ipEnd + 3);
				IPtoi(m_host, ipHost, ipHost + 1, ipHost + 2, ipHost + 3);

				if( !strcmp(temp, "A") )
				{
					if (ipStart[0] == ipHost[0] && ipStart[1] <= ipHost[1] && ipHost[1] <= ipEnd[1])
					{
						GAMELOG << init("BLOCK Band", id)
								<< blockBand << delim
								<< m_host
								<< end;

#ifdef IP_BLOCKNOKICK_BAND
						break;
#else
						throw MSG_FAIL_CONNECT_SERVER;
#endif
					}

				}

				if( !strcmp(temp, "B") )
				{
					if (ipStart[0] == ipHost[0] && ipStart[1] == ipHost[1] && ipStart[2] <= ipHost[2] && ipHost[2] <= ipEnd[2])
					{
						GAMELOG << init("BLOCK Band", id)
								<< blockBand << delim
								<< m_host
								<< end;

#ifdef IP_BLOCKNOKICK_BAND
						break;
#else
						throw MSG_FAIL_CONNECT_SERVER;
#endif
					}
					
				}

				if( !strcmp(temp, "C") )
				{
					if (ipStart[0] == ipHost[0] && ipStart[1] == ipHost[1] && ipStart[1] == ipHost[1] && 
						ipStart[3] <= ipHost[3] && ipHost[3] <= ipEnd[3])
					{
						GAMELOG << init("BLOCK Band", id)
								<< blockBand << delim
								<< m_host
								<< end;
#ifdef IP_BLOCKNOKICK_BAND
						break;
#else
						throw MSG_FAIL_CONNECT_SERVER;
#endif
					}					
				}

			}
		}
#endif

#ifdef IP_BLOCK_AUTO
		dbUser.GetRec("a_regtime", regdate);

		time_t notime, now;
		struct tm noblockDate;
		time(&now);
		noblockDate = *localtime(&now);
		noblockDate.tm_year = 105;
		noblockDate.tm_mon = 5;
		noblockDate.tm_mday = 20;
		noblockDate.tm_hour = 0;
		noblockDate.tm_min = 0;
		noblockDate.tm_sec = 0;
		notime = mktime(&noblockDate);

		if(notime > regdate)
		{


			// 아이피 검사를 통과하면
			// 시도 회수 검사에 들어감
			// 1. 리셋해야하나 검사
			// 2. 제외 아이피 검사
			// 3. 테이블에서 시도 회수 검사
			// 4. 테이블 갱신
			if (gserver.m_autoBlockPulse + PULSE_REAL_HOUR * atoi(gserver.m_config.Find("Auto Block", "Refresh")) < gserver.m_pulse)
			{
				gserver.m_autoBlockPulse = gserver.m_pulse;
				while (gserver.m_autoBlockTable)
				{
					AUTO_BLOCK_INFO* p = gserver.m_autoBlockTable;
					gserver.m_autoBlockTable = gserver.m_autoBlockTable->next;
					delete p;
				}
			}

			sprintf(g_buf, "ExIP%s", (const char*)m_host);
			sprintf(g_buf2, "ExID%s", (const char*)id);
			if ( strlen(gserver.m_config.Find("Auto Block", g_buf)) == 0 && strlen(gserver.m_config.Find("Block ExID", g_buf2)) == 0 )
			{
				// 제외 아님
				int count = 0;
				bool bFound = false;
				AUTO_BLOCK_INFO* p = gserver.m_autoBlockTable;
				while (p)
				{
					if (strcmp(p->ip, m_host) == 0)
					{
						count++;
						if (strcmp(p->id, id) == 0)
						{
							bFound = true;
						}
					}
					p = p->next;
				}

				if (count >= atoi(gserver.m_config.Find("Auto Block", "MaxTry")))
				{
					GAMELOG << init("AUTO BLOCK", id)
							<< m_host << delim
							<< count
							<< end;
#ifdef IP_BLOCKNOKICK_AUTO
						
#else
						throw MSG_FAIL_CONNECT_SERVER;
#endif
				}
				else
				{
					if (!bFound)
					{
						p = new AUTO_BLOCK_INFO;
						strcpy(p->ip, m_host);
						strcpy(p->id, id);
						p->prev = NULL;
						p->next = gserver.m_autoBlockTable;
						if( gserver.m_autoBlockTable ) gserver.m_autoBlockTable->prev = p;
						gserver.m_autoBlockTable = p;
					}
				}
			}
		}

#endif

#ifdef HSTEST		
		PlayerRealIP( playerMsg, m_host );
		SEND_Q( playerMsg, this );
		gserver.SendOutput(this);
#endif // HSTEST

		int server;
		int subnum;
		int zone;
		if (!dbUser.GetRec("a_server_num", server))
			goto FIRST_CONNECT;
		if(!dbUser.GetRec("a_subnum", subnum))
			goto FIRST_CONNECT;
		if (!dbUser.GetRec("a_zone_num", zone))
			goto FIRST_CONNECT;

		if (zone >= 0)
		{
			// 이미 접속되어 있음
			// 메신저에게 접종 처리 요청 메세지
			CNetMsg logoutMsg;
			LogoutReqMsg(logoutMsg, server, subnum, zone, this);
			SEND_Q(logoutMsg, gserver.m_messenger);
			
			throw MSG_FAIL_LOGINSERV_ALREADY_CONNECT;
		}

		
#ifdef CHECK_SECURE_CARD  ///=== 홍콩 보안카드
		// 보안 카드 사용자 인지 체크
		if (IsCardUser(id) == true)
		{
			STATE(this) = CON_WAIT_SCARD;
			return true;
		}
#endif // CHECK_SECURE_CARD

#ifdef CHECKIP_USA
		IsUSA = CheckIPForUSA(m_host);
#endif // CHECKIP_USA
		
		GAMELOG << init("LOGIN", id)
			<< m_host
#ifdef CHECKIP_USA
			<< delim
			<< IsUSA
#endif // CHECKIP_USA
			<< end;

		int i;
		for (i=0; i < gserver.m_nConnector; i++)
		{
			// 사용자 수 전송
			PlayerNumMsg(playerMsg, server, subnum, i);
			SEND_Q(playerMsg, this);
			gserver.SendOutput(this);
		}

		STATE(this) = CON_PLAYING;

#ifdef BRZ_FAIL_PASSWORD
		sqlblock.Format("UPDATE t_users SET a_block_time = 0, a_fail_count = 0 WHERE a_portal_index = %d", usercode);
		dbUser.SetQuery(sqlblock);
		dbUser.Update();
#endif // BRZ_FAIL_PASSWORD
		return true;

FIRST_CONNECT:

#ifdef CHECK_SECURE_CARD  ///=== 홍콩 보안카드
		// 보안 카드 사용자 인지 체크
		if (IsCardUser(id) == true)
		{
			STATE(this) = CON_WAIT_SCARD;
			return true;
		}
#endif // CHECK_SECURE_CARD

#ifdef CHECKIP_USA
 		IsUSA = CheckIPForUSA(m_host);
#endif // CHECKIP_USA

		GAMELOG << init("LOGIN", m_idname)
				<< m_host
#ifdef CHECKIP_USA
				<< delim
				<< IsUSA
#endif // CHECKIP_USA
				<< end;

		for (i=0; i < gserver.m_nConnector; i++)
		{
			// 처음 접속 사용자 수 전송
			PlayerNumMsg(playerMsg, -1, -1, i);
			SEND_Q(playerMsg, this);
			gserver.SendOutput(this);
		}

		STATE(this) = CON_PLAYING;

		return true;
	}
	catch (MSG_FAIL_TYPE failtype)
	{
		FailMsg(failmsg, failtype);
		SEND_Q(failmsg, this);
		gserver.SendOutput(this);
		STATE(this) = CON_CLOSE;

#ifdef BRZ_FAIL_PASSWORD
		if(failtype == MSG_FAIL_LOGINSERV_WRONG_PASSWORD)
		{
			CLCString sql(1024);
			int blocktime;
			int failcount;
			sql.Format("SELECT a_block_time, a_fail_count FROM t_users WHERE a_idname = '%s'", (const char*)m_idname);
			
			CDBCmd dbUser;
			dbUser.Init(&gserver.m_dbuser);
			dbUser.SetQuery(sql);
			if(dbUser.Open() && dbUser.MoveFirst())
			{
				if(dbUser.GetRec("a_block_time", blocktime) &&
					dbUser.GetRec("a_fail_count", failcount))
				{
					failcount++;
					if(failcount >= 3)		// 30간 블럭
					{
						sql.Format("UPDATE t_users SET a_block_time = unix_timestamp(NOW()) + 60 * 30, a_fail_count = 0 WHERE a_idname = '%s'" , (const char*)m_idname);
						dbUser.SetQuery(sql);
						if(dbUser.Update())
						{
							GAMELOG << init("UPDATE t_users 30M BLOCK")
									<< m_idname
									<< end;
						}
						else
						{
							GAMELOG << init("FAILED t_users 30M BLOCK")
									<< m_idname
									<< end;
						}
					}
					else
					{
						sql.Format("UPDATE t_users SET a_block_time = 0, a_fail_count = %d WHERE a_idname = '%s'", failcount, (const char*)m_idname);
						dbUser.SetQuery(sql);
						if(dbUser.Update())
						{
							GAMELOG << init("UPDATE t_users INCREASE")
								<< "Count: "
								<< failcount << " "
								<< "ID: "
								<< m_idname
								<< end;
						}
						else
						{
							GAMELOG << init("FAILED t_users INCREASE")
								<< " Count: "
								<< failcount << " "
								<< "ID: "
								<< m_idname
								<< end;
						}
					}
				}
			}
		}
#endif // BRZ_FAIL_PASSWORD

		return false;
	}
}

#ifdef CHECK_SECURE_CARD
bool CDescriptor::IsCardUser(CLCString id)
{
	CLCString sql(1024);
	sql.Format("select b.card_str as cardstr from mat_user a left join mat_id b on a.card_id=b.card_id where a.username='%s'", (const char *)id);

	CDBCmd dbScard;
	dbScard.Init(&gserver.m_dbScard);
	dbScard.SetQuery((const char *)sql);

	if (dbScard.Open() == true && dbScard.MoveFirst() == true)
	{
		dbScard.GetRec("cardstr", m_cardstr);

		// 보안 카드 확인 전송
		// 랜덤 번호 생성
		for(int i=0; i<4; i++)
		{
			m_scard[i] = GetRandom(0, 47);
			bool bRetry = true;
			while(bRetry)
			{
				bRetry = false;

				for(int j=0; j<i; j++)
				{
					if (m_scard[j] == m_scard[i])
					{
						m_scard[i] = GetRandom(0, 47);
						bRetry = true;
						break;
					}					
				}	// for j
			} // while bRetry
		}	// for i
		
		CNetMsg rmsg;
		CheckSecureCardMsg(rmsg, this);
		SEND_Q(rmsg, this);

		return true;
	}

	return false;
}

bool CDescriptor::GetSCard(CNetMsg & msg)
{
	if (msg.m_mtype != MSG_EXTEND)
	{
		return false;
	}

	CNetMsg failmsg;
	CNetMsg playerMsg;

	bool bCheck = true;
	// 보안 카드 검사
	unsigned long subtype;
	unsigned char scard[4];
	msg.MoveFirst();
	msg >> subtype;
	if (subtype != MSG_EX_SECURE_CARD)
	{
		return false;
	}

	for(int i=0; i<4; i++)
	{
		msg >> scard[i];
		if (m_cardstr[(int)m_scard[i]] != scard[i])
		{
			bCheck = false;
			break;
		}
	}

	if (bCheck == true)
	{
		GAMELOG << init("LOGIN", m_idname)
					<< m_host
					<< end;

		for (int i=0; i < gserver.m_nConnector; i++)
		{
			// 처음 접속 사용자 수 전송
			PlayerNumMsg(playerMsg, -1, -1, i);
			SEND_Q(playerMsg, this);
			gserver.SendOutput(this);
		}

		STATE(this) = CON_PLAYING;
		return true;
	}

	FailMsg(failmsg, MSG_FAIL_SCARD_NOT_MATCHING);
	SEND_Q(failmsg, this);
	gserver.SendOutput(this);
	STATE(this) = CON_CLOSE;
	return false;
}
#endif // CHECK_SECURE_CARD