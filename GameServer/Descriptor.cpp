#include "stdhdrs.h"
#include "Log.h"
#include "Descriptor.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"

CDescriptor::CDescriptor()
: m_idname(MAX_ID_NAME_LENGTH + 1)
, m_passwd(MAX_PWD_LENGTH + 1)
, m_ident(9)
#if defined(EVENT_PROMOTION_SITE)
, m_proSite(3+1)
#endif
, m_host(HOST_LENGTH + 1)
#ifdef HANARO_EVENT
, m_hanaroCode(4)
#endif
{
	m_index = -1;

	m_bwaitdb = false;
	m_updatedb = DB_NOP;
	m_tryBug = false;

	m_desc = 0;

#ifdef CRYPT_NET_MSG

	m_bCryptNetMsg = true;
	CNM_InitKeyValue(&m_nCryptKey);

#ifdef CRYPT_NET_MSG_MANUAL
	CNM_InitKeyValue(&m_nOldCryptKey);
	m_pulseCryptKey = gserver.m_pulse;
	m_nPrepareSeed = 0;
#else // CRYPT_NET_MSG_MANUAL
	CNM_InitKeyValue(&m_nCryptKeyClient);
	m_nPrepareSeed = CNM_MakeSeedForServer(rand(), gserver.m_pulse);
#endif // CRYPT_NET_MSG_MANUAL

#endif // #ifdef CRYPT_NET_MSG

	m_hackCheckPulse = 0;
	m_hackServerPulse = 0;
	m_hackClientPulse = 0;
	m_hackLastClientDiff = 0;
	m_hackCount = 0;

	m_idletics = 0;
	m_commandcount = 0;
	m_checktics = 0;
	m_timestamp = -1;
	m_laststamp = -1;

	m_connected = 0;
	m_connectreq = 0;
	m_logined	= LOGIN_STATE_NOT;
	m_playmode = MSG_LOGIN_NEW;  // default play mode
	m_quitsave = true;
	m_bclosed	= false;
	m_dbrunning = false;
	m_bGoZone = false;
	m_bGoZoneSent = true;
	m_waithelper = false;

	m_pChar = NULL;
	m_pPrev = NULL;
	m_pNext = NULL;
//0704
	memset(m_notice, 0, sizeof(int) * MAX_NOTICE);

#ifdef EVENT_2PAN4PAN_BOX
	m_nEvent2pan4panBox = 0;
	m_event2pan4panBox = NULL;
#endif // #ifdef EVENT_2PAN4PAN_BOX

#ifdef EVENT_2PAN4PAN_GOODS
	m_nEvent2pan4panGoods = 0;
	m_event2pan4panGoods = NULL;
#endif // #ifdef EVENT_2PAN4PAN_GOODS

#ifdef LC_TLD
	m_bSendAdultNotice = false;
#endif
	
#if defined(EVENT_PCBANG) || defined(EVENT_PCBANG_2ND)
	m_location = 0;
#endif //

#ifdef ENABLE_LOGOUT_DELAY
	m_logoutDelay = -1;
#endif // ENABLE_LOGOUT_DELAY

#ifdef USING_NPROTECT
	m_pGGAuth = NULL;
#endif // USING_NPROTECT
	
	m_helperWaitType = 0;
#ifdef NIGHT_SHADOW
	m_bCreate90Lv = false;
#endif //NIGHT_SHADOW
}

CDescriptor::~CDescriptor()
{
#ifdef EVENT_2PAN4PAN_BOX
	m_nEvent2pan4panBox = 0;
	delete [] m_event2pan4panBox;
	m_event2pan4panBox = NULL;
#endif // #ifdef EVENT_2PAN4PAN_BOX
	
#ifdef EVENT_2PAN4PAN_GOODS
	m_nEvent2pan4panGoods = 0;
	delete [] m_event2pan4panGoods;
	m_event2pan4panGoods = NULL;
#endif // #ifdef EVENT_2PAN4PAN_GOODS
	
	if (m_pChar)
		delete m_pChar;

	m_pChar = NULL;

#ifdef USING_NPROTECT
	if (m_pGGAuth)
	{
		delete m_pGGAuth;
		m_pGGAuth = NULL;
	}
#endif // USING_NPROTECT
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
	if (m_bclosed || STATE(this) == CON_CLOSE)
		return ;
#ifdef CRYPT_NET_MSG
	if (!m_outBuf.Add(msg, m_bCryptNetMsg, &m_nCryptKey))
#else
	if (!m_outBuf.Add(msg))
#endif // #ifdef CRYPT_NET_MSG
	{
		if (m_pChar)
			GAMELOG << init("OUTPUT OVERFLOW", m_pChar->m_name, m_pChar->m_nick, m_idname);
		else if (m_idname[0])
			GAMELOG << init("OUTPUT OVERFLOW", m_idname);
		else
			GAMELOG << init("OUTPUT OVERFLOW");

		GAMELOG << end;

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
			if (m_pChar) m_pChar->m_bPlaying = false;
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
		if (nWrite == 0) 
		{
			GAMELOG << init("WriteToDescriptor Error")
					<< end;
			return -1;
		}
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

int CDescriptor::ProcessInput(int maxqueuesize)
{
	bool bGetPacket = false;
	ssize_t bytes_read;

	do
	{
		if (m_inBuf.GetRemain() <= 0)
		{
			if (m_pChar)
			{
				GAMELOG << init("SYS_WARN", m_pChar)
						<< "input overflow"
						<< end;
			}
			else
			{
				GAMELOG << init("SYS_WARN", m_idname)
						<< "input overflow"
						<< end;
			}
			return -1;
		}

		bytes_read = PerformSocketRead(m_desc, (char*)m_inBuf.GetPoint(), m_inBuf.GetRemain());

		if (bytes_read < 0)	/* Error, disconnect them. */
		{
			GAMELOG << init("bytes_read < 0") << delim << bytes_read << end;
			return -1;
		}
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
				if (m_inQ.Add(m, maxqueuesize))
					bGetPacket = true;
				else
				{
					GAMELOG << init("m_inQ.Add fail") << end;
					bFail = true;
					bStop = true;
				}
				break;
			case 1:
				bStop = true;
				break;
			case -1:
				GAMELOG << init("GetMessage fail") << end;
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
	if (ret == 0)
	{
		GAMELOG << init("PerformSocketRead") << delim << "read error" << delim << ret << end;
		return -1;
	}

	/*
	* read returned a value < 0: there was an error
	*/

#if defined(CIRCLE_WINDOWS)	/* Windows */
	if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAEINTR)
		return 0;
	CLCString tmp(100);
	tmp.Format("%d\n", WSAGetLastError());
	OutputDebugString(tmp);
#else

#ifdef EINTR		/* Interrupted system call - various platforms */
	if (errno == EINTR)
	{
		GAMELOG << init("PerformSocketRead") << delim << "errno" << delim << "EINTR" << end;
		return 0;
	}
#endif

#ifdef EAGAIN		/* POSIX */
	if (errno == EAGAIN)
	{
		GAMELOG << init("PerformSocketRead") << delim << "errno" << delim << "EAGAIN" << end;
		return 0;
	}
#endif

#ifdef EWOULDBLOCK	/* BSD */
	if (errno == EWOULDBLOCK)
	{
		GAMELOG << init("PerformSocketRead") << delim << "errno" << delim << "EWOULDBLOCK" << end;
		return 0;
	}
#endif /* EWOULDBLOCK */

#ifdef EDEADLK		/* Macintosh */
	if (errno == EDEADLK)
	{
		GAMELOG << init("PerformSocketRead") << delim << "errno" << delim << "EDEADLK" << end;
		return 0;
	}
#endif
	
	GAMELOG << init("PerformSocketRead") << delim << errno << end;

#endif /* CIRCLE_WINDOWS */

	/*
	* We don't know what happened, cut them off. This qualifies for
	* a SYSERR because we have no idea what happened at this point.
	*/
	return -1;
}

bool CDescriptor::GetLogin(CNetMsg& msg)
{
	CNetMsg failmsg;

	try
	{
		msg.MoveFirst();

		if (msg.m_mtype != MSG_LOGIN)
			throw MSG_FAIL_CONNECT_SERVER;		// 잘못된 입력

		if (gserver.m_playerList.m_cur > MAX_PLAYER_LIST)
			throw MSG_FAIL_MANY_CONNECT;		// 접속 요청 초과

		int version = 0;
		unsigned char mode = 0;
		unsigned char nation;	// 국가 코드 체크 
		CLCString id(MAX_ID_NAME_LENGTH + 1);
		CLCString pw(MAX_PWD_LENGTH + 1);

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

#ifdef HANARO_EVENT 
		// 하나로포스-HF, 브로드밴드-SK
		CLCString proSite(4);
		msg >> proSite;
		m_hanaroCode = proSite;
#endif

		id.Trim();
		pw.Trim();

		GAMELOG << init("LOGIN REQ", id)
				<< "VERSION" << delim << version << delim
				<< "MODE" << delim << mode << delim
				<< "IP" << delim << m_host << delim
				<< "NATION" << delim << nation
#ifdef HANARO_EVENT
				<< delim << "PROSITE" << delim << m_hanaroCode
#endif
				<< end;
#ifdef IP_BLOCK_BAND

		// IP 검사용
		CLCString temp(50);
		CLCString blockBand(50);

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

					throw MSG_FAIL_LOGINSERV_NO_SERVICE;
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
						throw MSG_FAIL_LOGINSERV_NO_SERVICE;
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
						throw MSG_FAIL_LOGINSERV_NO_SERVICE;
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
						throw MSG_FAIL_LOGINSERV_NO_SERVICE;
#endif
					}					
				}

			}
		}
#endif

#ifndef _DEBUG
#ifndef _CONSOLE
		if (version < gserver.m_clientversionMin || version > gserver.m_clientversionMax)
		{
			GAMELOG << init("INVALID_VERSION", id)
					<< version
					<< end;
			throw MSG_FAIL_WRONG_VERSION;
		}
#endif
#endif
		if( nation != gserver.m_national )
		{
			GAMELOG << init("INVALID_NATION", id)
					<< nation
					<< end;
			throw MSG_FAIL_WRONG_VERSION;
		}

		if (mode == MSG_LOGIN_NEW)
			PLAYMODE(this) = MSG_LOGIN_NEW;
#ifdef GMTOOL
		else if(mode == MSG_LOGIN_GM)
		{
			PLAYMODE(this) = MSG_LOGIN_GM;
			if( gserver.m_serverno != 1 || gserver.m_subno != 1)
				throw MSG_FAIL_LOGINSERV_NO_SERVICE;
		}
#endif // GMTOOL
		else
			PLAYMODE(this) = MSG_LOGIN_RE;


		if (strinc(id, "'"))
			throw MSG_FAIL_WRONG_CHAR;			// 아이디에 ' 들어감

		int len = strlen(id);
		if (len < 3 || len > MAX_ID_NAME_LENGTH)
			throw MSG_FAIL_WRONG_CHAR;

		m_idname = id;

		if (strinc(pw, "'"))
			throw MSG_FAIL_WRONG_PASSWORD;		// 암호에 ' 들어감

		len = strlen(pw);
		if (len < 4 || len > MAX_PWD_LENGTH)
			throw MSG_FAIL_WRONG_PASSWORD;

		m_passwd = pw;

		if (gserver.FindConnectIdName(m_idname, this))
		{
			gserver.DisConnectLogedIn(this, false);
			throw MSG_FAIL_ALREADY_CONNECT;
		}

#ifdef LC_KOR
		if (gserver.m_bOnlyLocal)
		{
			if (strcmp2(m_idname, "guest01") && strcmp2(m_idname, "guest02") && strcmp2(m_idname, "guest03") && strcmp2(m_idname, "cbx030"))
			{
				if (strncmp(m_host, "61.104.44.", strlen("61.104.44.")) 
					&& strncmp(m_host, "222.106.51.", strlen("222.106.51."))
					//&& strncmp(m_host, "211.112.36.", strlen("211.112.36.")) )		// 운영팀 새로운 IP추가
					&& strncmp(m_host, "124.137.195.", strlen("124.137.195.")) )		// 운영팀 IP 변경 080404
				{
					GAMELOG << init("SYS_ERR")
							<< "ONLY LOCAL"
							<< m_host
							<< end;
					throw MSG_FAIL_LOGINSERV_NO_SERVICE;
				}
			}
		}
#endif

// 050226 : bs : 일단 국내와 중국만 아이피 검사
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
// --- 050226 : bs : 일단 국내와 중국만 아이피 검사

		WAITTING_DB(this) = true;
		STATE(this) = CON_WAIT_IDEN_MSG;

		return true;
	}
	catch (MSG_FAIL_TYPE failtype)
	{
		CNetMsg smsg;
		FailMsg(smsg, failtype);
		SEND_Q(failmsg, this);
		gserver.SendOutput(this);
		STATE(this) = CON_CLOSE;
		return false;
	}
}

bool CDescriptor::StartGame(CNetMsg& msg)
{
	msg.MoveFirst();

	unsigned char mode = 0;
	int index = 0;

	msg >> mode
		>> index;

	if (mode != MSG_MENU_START)
		return false;

	if (m_pChar)
	{
		delete m_pChar;
		m_pChar = NULL;
	}

	m_pChar = new CPC;
	m_pChar->m_index = index;
	m_pChar->m_desc = this;

	WAITTING_DB(this) = true;
	STATE(this) = CON_WAIT_PLAY_CHAR_DB;
	
	return true;
}

bool CDescriptor::CharNew(CNetMsg& msg)
{
	msg.MoveFirst();

	unsigned char mode = 0;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	char job = -1;
	char hairstyle = 1;
	char facestyle = 1;

	msg >> mode
		>> name
		>> job
		>> hairstyle
		>> facestyle;

#ifndef CREATE_SORCERER
	if( job == JOB_SORCERER )
		return false;
#endif

#ifdef NIGHT_SHADOW
	if( job == JOB_NIGHTSHADOW && !m_bCreate90Lv )
		return false;
#endif // NIGHT_SHADOW

	if (mode != MSG_MENU_NEW)
		return false;

	if (m_pChar)
	{
		delete m_pChar;
		m_pChar = NULL;
	}

	m_pChar = new CPC;

	// 이름이 0 보다 작으면 안된다.
	if (strlen(name) < 4 || strlen(name) > 16)
	{
		CNetMsg failmsg;
		FailMsg(failmsg, MSG_FAIL_WRONG_CHAR);
		SEND_Q(failmsg, this);
		return false;
	}
	// 이름에 ' 가 들어 있으면 안된다.
	if (strinc(name, "'") || strinc(name, " "))
	{
		CNetMsg failmsg;
		FailMsg(failmsg, MSG_FAIL_WRONG_CHAR);
		SEND_Q(failmsg, this);
		return false;
	}

	m_pChar->m_name = name;

	// 유효성 검사
	int haircount = 0;
	int facecount = 0;
	switch (job)
	{
	case JOB_TITAN:		haircount = HAIR_TITAN;		facecount = FACE_TITAN;		break;
	case JOB_MAGE:		haircount = HAIR_MAGE;		facecount = FACE_MAGE;		break;
	case JOB_HEALER:	haircount = HAIR_HEALER;	facecount = FACE_HEALER;	break;
	case JOB_KNIGHT:	haircount = HAIR_KNIGHT;	facecount = FACE_KNIGHT;	break;
	case JOB_ROGUE:		haircount = HAIR_ROGUE;		facecount = FACE_ROGUE;		break;
	case JOB_SORCERER:	haircount = HAIR_SORCERER;	facecount = FACE_SORCERER;	break;
	}
	if (hairstyle < 1 || hairstyle > haircount || facestyle < 1 || facestyle > facecount)
	{
		CNetMsg failmsg;
		FailMsg(failmsg, MSG_FAIL_DB_NEW_CHAR);
		SEND_Q(failmsg, this);
		return false;
	}

	m_pChar->m_job = job;
	m_pChar->m_hairstyle = hairstyle;
	m_pChar->m_facestyle = facestyle;

	m_pChar->m_level = 1;
	m_pChar->LevelUp(false);

	WAITTING_DB(this) = true;
	STATE(this) = CON_WAIT_NEW_CHAR_DB;

	return true;
}

bool CDescriptor::CharDel(CNetMsg& msg)
{
	msg.MoveFirst();

	unsigned char mode = 0;
	int index = 0;

	msg >> mode
		>> index;

	if (mode != MSG_MENU_DEL)
		return false;

#ifdef CHARDEL_CHECKID
	CLCString ident(9);
	msg >> ident;

	if( 
#ifdef LC_KOR
		!( m_proSite.Length() > 0 && ( strcmp(m_proSite, "IT") == 0 || strcmp(m_proSite, "B1") == 0 ) ) &&  
#endif
		strcmp(this->m_ident, ident) != 0 )
	{
		CNetMsg rmsg;
		FailMsg(rmsg, MSG_FAIL_WRONG_IDENTIFICATION);
		SEND_Q(rmsg, this);
		return false;
	}
#endif
#ifdef EVENT_NEW_SERVER_2006_NEWSERVER
	if( this->m_proSite.Length() > 0 && ( strcmp(this->m_proSite, "IT") == 0 || strcmp(this->m_proSite, "B1") == 0 ) )
	{
		CNetMsg rmsg;
		FailMsg(rmsg, MSG_FAIL_WRONG_IDENTIFICATION);
		SEND_Q(rmsg, this);
		return false;
	}
#endif

#ifdef ENABLE_CHARACTER_DELETE_DELAY
	char bCancelDelete = 0;
	msg >> bCancelDelete;
#endif // ENABLE_CHARACTER_DELETE_DELAY

	if (m_pChar)
	{
		delete m_pChar;
		m_pChar = NULL;
	}

	m_pChar = new CPC;
	m_pChar->m_index = index;

#if defined (LC_GER)
	if (!bCancelDelete)
	{
		time_t t;
		m_pChar->m_guildoutdate = time(&t) / 60 / 60 / 24;
		m_pChar->SaveNow(false);
	}
#endif

	WAITTING_DB(this) = true;
#ifdef ENABLE_CHARACTER_DELETE_DELAY
	if (bCancelDelete)
		STATE(this) = CON_WAIT_DEL_CANCEL_CHAR_DB;
	else
		STATE(this) = CON_WAIT_DEL_CHAR_DB;
#else // ENABLE_CHARACTER_DELETE_DELAY
	STATE(this) = CON_WAIT_DEL_CHAR_DB;
#endif // ENABLE_CHARACTER_DELETE_DELAY
	
#ifdef IRIS_POINT
	CNetMsg ipoint_msg;
	if (bCancelDelete)
		ConnIPointAddMsg( ipoint_msg, m_index, index, MSG_IPOINT_TYPE_CANCEL_DELETE_CHAR, 0 );
	else
		ConnIPointAddMsg( ipoint_msg, m_index, index, MSG_IPOINT_TYPE_DELETE_CHAR , 0 );
	SEND_Q( ipoint_msg, gserver.m_connector );
#endif //IRIS_POINT
	return true;
}

void CDescriptor::SetHackCheckPulse()
{
	m_hackCheckPulse = gserver.m_pulse;
	m_hackServerPulse = 0;
	m_hackClientPulse = 0;
	m_hackLastClientDiff = 0;
}

bool CDescriptor::IsHackResponseTimeout()
{
	return (gserver.m_pulse - m_hackCheckPulse > PULSE_HACK_CHECK_TIMEOUT);
}

bool CDescriptor::CheckHackPulse(int clientPulse)
{
	// 서버 차이와 클라이언트 차이가 15초 이상이거나
	// 클라이언트의 차이가 이전 클라이언트의 차이와 30초가 경과 안되면(펄스를 자주 보낸경우)

	m_hackCheckPulse = gserver.m_pulse;

	// 처음 수행되거나, 1시간 경과시 값 갱신
	if (m_hackServerPulse < 1 || gserver.m_pulse - m_hackServerPulse > PULSE_HACK_PULSE_REFRESH)
	{
		m_hackServerPulse = gserver.m_pulse;
		m_hackClientPulse = clientPulse;
		return false;
	}

	int serverdiff = gserver.m_pulse - m_hackServerPulse;
	int clientdiff = clientPulse - m_hackClientPulse;
	int csdiff = clientdiff - serverdiff;

	bool bHack = false;

	// 핵검사 : 서버-클라이언트 시간 차이 검사
	if (csdiff > PULSE_HACK_PULSE_THRESHOLD)
	{
		if (m_pChar)
		{
			GAMELOG << init("HACK_PULSE", m_pChar)
					<< serverdiff
					<< delim
					<< clientdiff
					<< end;
		}
		else
		{
			GAMELOG << init("HACK_PULSE", m_idname)
					<< serverdiff
					<< delim
					<< clientdiff
					<< end;
		}
		bHack = true;
	}
	else if (ABS(m_hackLastClientDiff - clientdiff) < PULSE_HACK_CLIENT_THRESHOLD)
	{
		GAMELOG << init("HACK_PACKET", m_pChar)
				<< serverdiff
				<< delim
				<< clientdiff
				<< end;
		bHack = true;
	}

	if (bHack)
	{
		m_hackServerPulse = gserver.m_pulse;
		m_hackClientPulse = clientPulse;
		if (IncreaseHackCount(4))
			return true;
		else
			return false;
	}

	m_hackLastClientDiff = clientdiff;

	return false;
}

bool CDescriptor::IncreaseHackCount(int n)
{
	m_hackCount += n;

	if (m_pChar)
	{
		GAMELOG << init("HACK COUNT", m_pChar)
				<< m_hackCount
				<< end;
	}
	else
	{
		GAMELOG << init("HACK COUNT", m_idname)
				<< m_hackCount
				<< end;
	}

	if (m_hackCount > 10)
	{
#ifndef NOKICK_HACKUSER
#ifndef _DEBUG
#ifndef _CONSOLE
		m_bclosed = true;
#endif
#endif
#endif
		strcpy(g_buf, "HACK USER: ");
		IntCat(g_buf, gserver.m_serverno);
		strcat(g_buf, "-");
		IntCat(g_buf, gserver.m_subno);
		strcat(g_buf, ": ");
		strcat(g_buf, m_idname);
		strcat(g_buf, " : ");
		if (m_pChar)
			strcat(g_buf, m_pChar->m_name);
		else
			strcat(g_buf, "No select char");
		CNetMsg rmsg;
		MsgrObserverMsg(rmsg, -1, -1, -1, -1, g_buf);
		if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
		return true;
	}

	return false;
}
