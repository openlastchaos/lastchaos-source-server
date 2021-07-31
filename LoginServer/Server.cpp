#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"
#include "CmdMsg.h"
#include "DBCmd.h"

volatile LC_THREAD_T		gThreadIDGameThread;
volatile LC_THREAD_T		gThreadIDDBThread;

CServer gserver;

void ServerSrandom(unsigned long initial_seed);

CServer::CServer()
{
	m_ssock = -1;
	m_desclist = NULL;
	m_messenger = NULL;
	m_connector = NULL;

	FD_ZERO(&m_input_set);
	FD_ZERO(&m_output_set);
	FD_ZERO(&m_exc_set);
	FD_ZERO(&m_null_set);

	m_serverpath = GetServerPath();
	m_maxplayers = 0;
	m_bshutdown = false;
	m_nameserverisslow = true;
	m_breboot = false;

	m_brunconn = NULL;
	m_brunmessenger = false;	// 메신저 수행 플래그
	m_resetPlayerTime = 0;
	m_checkConnector = NULL;

	m_pulse = 0;
	m_pulseServerTime = 0;

	mysql_init(&m_dbuser);
	mysql_init(&m_dbdata);
	mysql_init(&m_dbAuth);

	m_pulseDBPing = 0;

	m_clientversionMin = 0;
	m_clientversionMax = 0;

#ifdef IP_BLOCK_AUTO
	m_autoBlockTable = NULL;
	m_autoBlockPulse = gserver.m_pulse;
#endif

#ifdef RECOMMEND_SERVER_SYSTEM
	m_nRecommendServer = -1;
#endif // RECOMMEND_SERVER_SYSTEM

#if defined (WJKTEST) || defined(LC_USA)
	m_pulseSaveTableCount = 0;
#endif

#ifdef LC_TWN
	m_national = LC_TWN;
#ifdef LC_TWN2
	m_national = LC_TWN2;
#endif
#elif defined (LC_TLD)
	m_national = LC_TLD;
#elif defined (LC_TLD_ENG)
	m_national = LC_TLD_ENG;
#elif defined (LC_JPN)
	m_national = LC_JPN;
#elif defined (LC_MAL)
	m_national = LC_MAL;
#elif defined (LC_USA)
	m_national = LC_USA;
#elif defined (LC_BRZ)
	m_national = LC_BRZ;
#elif defined (LC_HBK)
	m_national = LC_HBK;	
#elif defined (LC_GER)
	m_national = LC_GER;
#elif defined (LC_SPN)
	m_national = LC_SPN;
#elif defined (LC_FRC)
	m_national = LC_FRC;
#elif defined (LC_PLD)
	m_national = LC_PLD;
#elif defined (LC_RUS)
	m_national = LC_RUS;
#else 
	m_national = LC_KOR;

#endif
}

CServer::~CServer()
{
	if (m_serverpath)
		delete[] m_serverpath;

	delete [] m_connector;
	delete [] m_brunconn;
	delete [] m_checkConnector;
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
	puts("Load setting file....");

	CLCString strTemp(1024);

	strTemp.Format("%s%s", m_serverpath, "newStobm.bin");

	if (!m_config.Load(strTemp))
		return false;

	strcpy(g_buf, m_config.Find("Server", "No"));
	if (strlen(g_buf) < 1)
		return false;
	m_subno = atoi(g_buf);

	if (strcmp(m_config.Find("Server", "AllowExternalIP"), "TRUE") == 0)
		m_bOnlyLocal = false;
	else
	{
		FILE* fpCheckOpen = fopen(LOGIN_SERVER_OPEN_CHECK_FILENAME, "r+");
		if (fpCheckOpen)
		{
			m_bOnlyLocal = false;
			fclose(fpCheckOpen);
		}
		else
		{
			m_bOnlyLocal = true;
			CheckInternalIP(m_config.Find("Server", "AllowExternalIP"));
		}
	}

	return true;
}

void CServer::CheckInternalIP(const char* strData)
{
	unsigned int		nIP[8];							// ip 4자리 + 서브넷마스크 4자리
	const char*			pData				= strData;
	unsigned int		nData;
	char				strBuf[256];
	char*				pBuf;
	int					nState				= 0;		// state	: 0 - 아이피 읽기 시작
														//			: 1 - 아이피 둘째 자리
														//			: 2 - 아이피 셋째 자리
														//			: 3 - 아이피 넷째 자리
														//			: 4 - 서브넷 읽기 시작
														//			: 5 - 서브넷 둘째 자리
														//			: 6 - 서브넷 셋째 자리
														//			: 7 - 서브넷 넷째 자리
														//			: 8 - 읽기 완료

	memset(strBuf, 0, sizeof(strBuf));
	pBuf = strBuf;

	while (*pData)
	{
		switch (*pData)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			*pBuf = *pData;
			pBuf++;
			*pBuf = '\0';
			break;

		case '.':
		case '/':
			if (nState != 3 && *pData == '/')
				goto EXIT_FUNC_DEFAULT;
			if (strlen(strBuf) < 1 || strlen(strBuf) > 3)
				goto EXIT_FUNC_DEFAULT;
			nData = (unsigned int)atoi(strBuf);
			if (nData < 0 || nData > 255)
				goto EXIT_FUNC_DEFAULT;
			nIP[nState] = nData;
			nState++;
			memset(strBuf, 0, sizeof(strBuf));
			pBuf = strBuf;
			break;

		default:
			goto EXIT_FUNC_DEFAULT;
		} // switch (*pData)

		pData++;
	} // while (*pData)

	if (strlen(strBuf) < 1 || strlen(strBuf) > 3)
		goto EXIT_FUNC_DEFAULT;
	nData = (unsigned int)atoi(strBuf);
	if (nData < 0 || nData > 255)
		goto EXIT_FUNC_DEFAULT;
	nIP[nState] = nData;
	nState++;

	if (strcmp(strData, m_config.Find("Server", "IP")) == 0)
	{
		// 서버 아이피를 사용할 경우 서브넷은 기본적으로 255.255.255.0
		nIP[4] = 255;
		nIP[5] = 255;
		nIP[6] = 255;
		nIP[7] = 0;
	}
	else if (nState != 8)
	{
		goto EXIT_FUNC_DEFAULT;
	}

	// 여기까지 오면 111.222.123.234/255.255.254.0이면
	// nIP 가 {111, 222, 123, 234, 255, 255, 254, 0} 이 된다
	m_nInternalIPFilter = ((nIP[0] << 24) & 0xff000000)
						+ ((nIP[1] << 16) & 0x00ff0000)
						+ ((nIP[2] <<  8) & 0x0000ff00)
						+ ((nIP[3]      ) & 0x000000ff);
	m_nInternalSubnetMask	= ((nIP[4] << 24) & 0xff000000)
							+ ((nIP[5] << 16) & 0x00ff0000)
							+ ((nIP[6] <<  8) & 0x0000ff00)
							+ ((nIP[7]      ) & 0x000000ff);
	m_nInternalIPFilter &= m_nInternalSubnetMask;

	return ;

EXIT_FUNC_DEFAULT:
	if (strcmp(strData, m_config.Find("Server", "IP")) == 0)
	{
		m_nInternalIPFilter = 0xffffffff;
		m_nInternalSubnetMask = 0xffffffff;
	}
	else
	{
		CheckInternalIP(m_config.Find("Server", "IP"));
	}
}

bool CServer::ConnectDB()
{
	if (!mysql_real_connect (
			&m_dbuser,
			m_config.Find("User DB", "IP"),
			m_config.Find("User DB", "User"),
			m_config.Find("User DB", "Password"),
			m_config.Find("User DB", "DBName"),
			0, NULL, 0))
		return false;

	if (!mysql_real_connect (
			&m_dbdata,
			m_config.Find("Data DB", "IP"),
			m_config.Find("Data DB", "User"),
			m_config.Find("Data DB", "Password"),
			m_config.Find("Data DB", "DBName"),
			0, NULL, 0))
		return false;

	if (!mysql_real_connect (
			&m_dbAuth,
			m_config.Find("Auth DB", "IP"),
			m_config.Find("Auth DB", "User"),
			m_config.Find("Auth DB", "Password"),
			m_config.Find("Auth DB", "DBName"),
			0, NULL, 0))
		return false;

#ifdef CHECK_SECURE_CARD
	if (!mysql_real_connect (
			&m_dbScard,
			m_config.Find("SCard DB", "IP"),
			m_config.Find("SCard DB", "User"),
			m_config.Find("SCard DB", "Password"),
			m_config.Find("SCard DB", "DBName"),
			0, NULL, 0))
		return false;
#endif // CHECK_SECURE_CARD

	return true;
}

void CServer::DisconnectDB()
{
	mysql_close (&m_dbuser);
	mysql_close (&m_dbAuth);

#ifdef CHECK_SECURE_CARD
	mysql_close(&m_dbScard);
#endif // CHECK_SECURE_CARD
}

bool CServer::InitGame()
{
	// version information loading
	GAMELOG << init("SYSTEM")
			<< "Client Version Loading..."
			<< end;

	CDBCmd dbcmd;
	dbcmd.Init(&m_dbdata);
	dbcmd.SetQuery("SELECT a_min, a_max FROM t_clientversion");
	if (!dbcmd.Open() || !dbcmd.MoveFirst() || !dbcmd.GetRec("a_min", m_clientversionMin) || !dbcmd.GetRec("a_max", m_clientversionMax))
		return false;
	dbcmd.Close();

	GAMELOG << init("SYSTEM")
			<< "Finding player limit."
			<< end;

	m_maxplayers = GetMaxPlayers();

	GAMELOG << init("SYSTEM")
			<< "Opening mother connection."
			<< end;

	m_ssock = InitSocket();

	if (!MakeMessengerClient())
	{
		if (m_messenger)
		{
			m_messenger->CloseSocket();
			delete m_messenger;
			m_messenger = NULL;
		}
	}

	SendMessengerHeader();

	m_nConnector = atoi(m_config.Find("Connector Server", "Count"));
	if (m_nConnector == 0)
		return false;

	// 컨넥터 숫자 만큼 : 서버군 숫자
	m_connector = new CDescriptor*[m_nConnector];
	memset(m_connector, 0, sizeof(CDescriptor*) * m_nConnector);
	m_brunconn = new bool[m_nConnector];
	m_checkConnector = new int[m_nConnector];

	int i;
	for (i=0; i < m_nConnector; i++)
	{
		m_brunconn[i] = false;
		m_checkConnector[i] = 0;

		if (!MakeConnectorClient(i))
		{
			if (m_connector[i])
			{
				m_connector[i]->CloseSocket();
				delete m_connector[i];
				m_connector[i] = NULL;
			}
		}

		SendConnectorHeader(i);
	}

	GAMELOG << init("SYSTEM")
			<< "Entering Login Server..."
			<< end;

	return true;
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
			GAMELOG << init("SYS_ERR") << "calling getrlimit" << end;
			exit(1);
		}

		/* set the current to the maximum */
		limit.rlim_cur = limit.rlim_max;
		if (setrlimit(RLIMIT_NOFILE, &limit) < 0)
		{
			GAMELOG << init("SYS_ERR") << "calling setrlimit" << end;
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
			GAMELOG << init("SYS_ERR") << "Error calling sysconf" << end;
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

	GAMELOG << init("SYSTEM") << "Setting player limit to " << max_descs << " using " << method << "." << end;

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
			puts("SYSERR: WinSock not available!");
			exit(1);
		}
		if ((wsaData.iMaxSockets - 4) < m_maxplayers)
		{
			m_maxplayers = wsaData.iMaxSockets - 4;
		}
		GAMELOG << init("SYSTEM")
				<< "Max players set to "
				<< m_maxplayers
				<< end;

		if ((s = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		{
			puts("SYSERR: Error opening network connection : Winsock error");
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
		GAMELOG << init("SYS_ERR") << "Error creating socket" << end;
		exit(1);
	}
#endif				/* CIRCLE_WINDOWS */

#if defined(SO_REUSEADDR) && !defined(CIRCLE_MACINTOSH)
	opt = 1;
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) < 0)
	{
		puts("SYSERR: setsockopt REUSEADDR");
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
		GAMELOG << init("SYS_ERR")
					<< "setsockopt SO_LINGER"
					<< end;	/* Not fatal I suppose. */
	}
#endif

	/* Clear the structure */
	memset((char *)&sa, 0, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons((unsigned short)atoi(gserver.m_config.Find("Server", "Port")));
	sa.sin_addr = *(GetBindAddr());

	if (bind(s, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		puts("SYSERR: bind");
		CLOSE_SOCKET(s);
		FILE *fp = fopen (".shutdown", "w");
		fclose (fp);
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
		GAMELOG << init("SYS_ERR")
				<< "setsockopt SNDBUF"
				<< end;
		return (-1);
	}

# if 0
	if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *) &opt, sizeof(opt)) < 0)
	{
		GAMELOG << init("SYS_ERR")
				<< "setsockopt RCVBUF"
				<< end;
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
			GAMELOG << init("SYS_ERR")
					<< "Invalid IP address"
					<< end;
		}
		else
			bind_addr.s_addr = addr;
	}

	/* Put the address that we've finally decided on into the logs */
	if (bind_addr.s_addr == htonl(INADDR_ANY))
		GAMELOG << init("SYSTEM")
				<< "Binding to all IP interfaces on this m_host."
				<< end;
	else
	{
		GAMELOG << init("SYSTEM")
				<< "Binding only to IP address "
				<< inet_ntoa(bind_addr)
				<< end;
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
		GAMELOG << init("SYSTEM")
				<< "SYSERR: Fatal error executing nonblock (CServer.cpp)"
				<< end;
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
			GAMELOG << init("SYS_ERR") << "Select sleep!!" << end;
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
			GAMELOG << init("SYS_ERR")
					<< "gethostbyaddr"
					<< end;

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

	/* prepend to list */
	ADD_TO_BILIST(newd, m_desclist, m_pPrev, m_pNext);

	newd->m_pulseConnected = m_pulse;

	return (0);
}

void CServer::CloseSocket(CDescriptor* d)
{
	if (STATE(d) != CON_DISCONNECT)
	{
		STATE(d) = CON_DISCONNECT;
		return;
	}
	else
	{
		if (STATE(d) != CON_CLOSE && STATE(d) != CON_DISCONNECT)
		{
			GAMELOG << init("SYS_ERR")
					<< "Close Socket Error :"
					<< STATE(d)
					<< end;
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
		if (d != my_d && STATE(d) != CON_GET_LOGIN)
		{
			if (STATE(d) != CON_DISCONNECT)
			{
				STATE(d) = CON_CLOSE;
			}
			return true;
		}
	}
	return false;
}

void CServer::SetServerTime()
{
	time_t ct;
	struct tm *ti;
	
	ct = time(0);
	ti = localtime(&ct);
	m_serverTime.year	= (ti->tm_year % 100) % 62;
	m_serverTime.month	= (ti->tm_mon + 1) % 62;
	m_serverTime.day	= ti->tm_mday % 62;
	m_serverTime.hour	= ti->tm_hour % 62;
	m_serverTime.min	= ti->tm_min % 62;
	m_serverTime.sec	= ti->tm_sec % 62;
}

bool CServer::MakeConnectorClient(int i)
{
	struct sockaddr_in msg_socket;
	
	if (m_connector[i])
	{
		if(m_connector[i]->m_bclosed)
		{ 
			m_connector[i]->CloseSocket();
			delete m_connector[i];
			m_connector[i] = NULL;
		}
		else
			return false;
	}

	m_connector[i] = new CDescriptor;
#ifdef CRYPT_NET_MSG
	m_connector[i]->m_bCryptNetMsg = false;
#endif // #ifdef CRYPT_NET_MSG

	CLCString tmp(100);
	tmp.Format("Connector_%d", i);

	m_connector[i]->m_serverNo = atoi(m_config.Find(tmp, "Number"));
	m_connector[i]->m_nMaxServer = atoi(m_config.Find(tmp, "MaxSubNumber"));

	m_connector[i]->m_playerNum = new int[m_connector[i]->m_nMaxServer];
	m_connector[i]->m_portNumber = new int[m_connector[i]->m_nMaxServer];
	m_connector[i]->m_ipAddr = new char*[m_connector[i]->m_nMaxServer];
	m_connector[i]->m_serverSubNo = new int[m_connector[i]->m_nMaxServer];

	int j;
	for (j=0; j < m_connector[i]->m_nMaxServer; j++)
	{
		m_connector[i]->m_playerNum[j] = -1;
		m_connector[i]->m_serverSubNo[j] = j + 1;
		m_connector[i]->m_ipAddr[j] = "";
		m_connector[i]->m_portNumber[j] = -1;
	}

	if ((m_connector[i]->m_desc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		puts ("SYSERR: Error opening network connection : Winsock error");
		return false;
	}
	
	if (m_connector[i]->m_desc <= 0) return false;
	memset(&msg_socket, 0, sizeof(msg_socket));
	msg_socket.sin_family = AF_INET;

	msg_socket.sin_addr.s_addr = inet_addr (m_config.Find(tmp, "IP"));
	msg_socket.sin_port = htons(atoi(m_config.Find(tmp, "Port")));

	if (connect(m_connector[i]->m_desc, (struct sockaddr *)&msg_socket, sizeof(msg_socket)) < 0)
	{
		tmp.Format("SYSERR: Cannot connect Connector_%d Server!!!", i);
		puts (tmp);
		/*GAMELOG << init("SYS_ERR")
				<< "Cannot connect Connector..."
				<< i
				<< end;*/
		return false;
	}

	Nonblock(m_connector[i]->m_desc);
	SetSendbuf(m_connector[i]->m_desc);

	return true;
}

void CServer::SendConnectorHeader(int i)
{
	CNetMsg msg;
	msg.Init(MSG_CONN_CONNECT);
	msg		<< SERVER_VERSION
			<< (int)LOGIN_SERVER_NUM;	// 서버군번호 수정
#ifdef LC_TIME
	if( m_bOnlyLocal )
		msg		<< (int) 0;
	else
		msg		<< (int) 1;
#else
	msg		<< (int)0;					// 서브 넘버

#endif

	if (m_connector[i] && !m_connector[i]->m_bclosed)
	{
		SEND_Q(msg, m_connector[i]);
		
		if (!m_connector[i]) return;
		if (m_connector[i]->m_desc < 0) return ;
		if (m_connector[i]->m_bclosed) return ;
		if (m_connector[i]->m_outBuf.GetNextPoint())
		{
			if (m_connector[i]->ProcessOutput() < 0)
				m_connector[i]->m_bclosed = true;
		}

		PlayerReqMsg(msg);
		SEND_Q(msg, m_connector[i]);
	}
}

bool CServer::MakeMessengerClient()
{
	struct sockaddr_in msg_socket;

	if (m_messenger)
	{
		if (m_messenger->m_bclosed)
		{
			m_messenger->CloseSocket();
			delete m_messenger;
			m_messenger = NULL;
		}
	}

	m_messenger = new CDescriptor;
#ifdef CRYPT_NET_MSG
	m_messenger->m_bCryptNetMsg = false;
#endif // #ifdef CRYPT_NET_MSG

	if ((m_messenger->m_desc = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		puts("SYSERR: Error opening network connection: socket error");
		return false;
	}

	memset (&msg_socket, 0, sizeof(msg_socket));
	msg_socket.sin_family = AF_INET;
	msg_socket.sin_addr.s_addr = inet_addr (gserver.m_config.Find("Messenger Server", "IP"));
	msg_socket.sin_port = htons(atoi(gserver.m_config.Find("Messenger Server", "Port")));

	if (connect(m_messenger->m_desc, (struct sockaddr*)&msg_socket, sizeof(msg_socket)) < 0)
	{
		puts ("SYSERR: Cannot connect Messenger...");
		return false;
	}

	Nonblock(m_messenger->m_desc);
	SetSendbuf(m_messenger->m_desc);

	return true;
}

void CServer::SendMessengerHeader()
{
	CNetMsg conMsg;
	conMsg.Init(MSG_MSGR_CONNECT);
	conMsg	<< SERVER_VERSION
			<< (int)LOGIN_SERVER_NUM	// 서버군번호
			<< (int)0					// 서브 넘버
			<< 0;						// 카운트존
	
	if (m_messenger && !m_messenger->m_bclosed)
	{
		SEND_Q(conMsg, m_messenger);

		if (!m_messenger) return;
		if (m_messenger->m_desc < 0) return ;
		if (m_messenger->m_bclosed) return ;
		if (m_messenger->m_outBuf.GetNextPoint())
		{
			if (m_messenger->ProcessOutput() < 0)
				m_messenger->m_bclosed = true;
		}
	}
}

#ifdef CIRCLE_WINDOWS //컨넥터 
void CServer::ConnectorConnect(void* i)
{
	int idx = (int)i;
	gserver.m_brunconn[idx] = true;
	while (true)
	{
		if (gserver.MakeConnectorClient(idx))
		{
			gserver.SendConnectorHeader(idx);
			gserver.m_brunconn[idx] = false;
			_endthread();
			return ;
		}
		else
			if (gserver.m_connector[idx])
				gserver.m_connector[idx]->m_bclosed = true;

		Sleep(2000);
	}
}
#else
void* CServer::ConnectorConnect(void* i)
{
	int idx = (int)i;
	gserver.m_brunconn[idx] = true;
	while (true)
	{
		if (gserver.MakeConnectorClient(idx))
		{
			gserver.SendConnectorHeader(idx);
			gserver.m_brunconn[idx] = false;
			pthread_exit(NULL);
			return NULL;
		}
		else
			if (gserver.m_connector[idx])
				gserver.m_connector[idx]->m_bclosed = true;

		sleep(2);
	}
	return NULL;
}
#endif



#ifdef CIRCLE_WINDOWS
void CServer::MessengerConnect(void* lpvoid)
{
	gserver.m_brunmessenger = true;
	while (true)
	{
		if (gserver.MakeMessengerClient())
		{
			gserver.SendMessengerHeader();
			gserver.m_brunmessenger = false;
			_endthread();
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

void CServer::ProcConnector(CNetMsg& msg)
{
	msg.MoveFirst();

	if (msg.m_mtype != MSG_CONN_REP)
		return;

	// MSG_CONN_PLAYER_REP,		// 서버군별 유저수 응답	: serverno(n) serversubno(n) usercount(n)
	char subtype;

	msg >> subtype;

	switch (subtype)
	{
	case MSG_CONN_PLAYER_REP:
		{
		int serverCount, server, subNo, player, port;
			CLCString ip(HOST_LENGTH + 1);
		int i, j, k;

		// 서브 서버 숫자 더 작은걸로 선택(컨넥터, 로긴서버중)
		int maxServer;

		msg >> server
			>> serverCount;

		for (i=0; i < m_nConnector; i++)
		{
			// 서버군 번호가 같은 인덱스 찾음
			if (m_connector[i] && !m_brunconn[i])
			{
				if (server == m_connector[i]->m_serverNo)
				{
					// 더 작은걸로
					if (serverCount < m_connector[i]->m_nMaxServer)
						maxServer = serverCount;
					else
						maxServer = m_connector[i]->m_nMaxServer;

					break;
				}
			}
		}

		if (i != m_nConnector)
		{
			for (j=0; j < maxServer; j++)
			{
				msg >> subNo
					>> player
					>> ip
					>> port;

#ifdef SETTING_IF_INNER_IP // 외부 아이피와 내부 아이피가 나뉘면 사용 ...yhj
				ip.Format("%s", m_config.Find("Connector Server", "EX_IP") );
#endif // SETTING_IF_INNER_IP				

				// 로긴서버의 서브 서버 번호 중에서 찾기
				for (k=0; k < m_connector[i]->m_nMaxServer; k++)
				{
					if (subNo == m_connector[i]->m_serverSubNo[k])
					{
						m_connector[i]->m_playerNum[k] = player;
						m_connector[i]->m_ipAddr[k] = new char[HOST_LENGTH + 1];
						strcpy(m_connector[i]->m_ipAddr[k], ip);
						m_connector[i]->m_portNumber[k] = port;
					}
				}
			}
		}
		}
		break;

	default:
		break;
	}
}

void CServer::ProcMessenger(CNetMsg& msg)
{
	msg.MoveFirst();

	int seq, server, subno, zone;
	char subtype;

	msg >> seq
		>> server
		>> subno
		>> zone
		>> subtype;

	switch (msg.m_mtype)
	{
	case MSG_MSGR_REP:
		{
//			int i, j;
			
			switch (subtype)
			{
			case MSG_MSGR_LOGOUT_REP:
				{
					char success;
					CLCString id(MAX_ID_NAME_LENGTH + 1);
					
					msg >> success
						>> id;
					
					GAMELOG << init("LOGIN ERROR", id)
							<< "CANNOT DISCONNECT ALREADY CONNECTION"
							<< end;
				}
				
				break;
				
			default:
				break;
			}
		}
		break;

	case MSG_MSGR_REQ:
		{
			switch (subtype)
			{
#ifdef RECOMMEND_SERVER_SYSTEM
			case MSG_MSGR_RECOMMEND:
				{
					int nGMCharIndex;
					int nRecommendServer;

					msg >> nGMCharIndex
						>> nRecommendServer;

					m_nRecommendServer = nRecommendServer;
				}
				break;
#endif // RECOMMEND_SERVER_SYSTEM

			case MSG_MSGR_ENDGAME_REQ:
				m_bshutdown = true;
				m_breboot = true;
				break;

			case MSG_MSGR_LOGIN_SERVER:
				{
					int nCode;
					msg >> nCode;
					switch (nCode)
					{
					case 0:		// close
						if (!m_bOnlyLocal)
						{
							remove(LOGIN_SERVER_OPEN_CHECK_FILENAME);
							m_bshutdown = true;
							m_breboot = false;
						}
						break;
					case 1:		// open
						if (m_bOnlyLocal)
						{
							FILE* fpCheckFile = fopen(LOGIN_SERVER_OPEN_CHECK_FILENAME, "w");
							fclose(fpCheckFile);
							m_bshutdown = true;
							m_breboot = false;
						}
						break;
					case 2:		// reboot
						m_bshutdown = true;
						m_breboot = false;
						break;
					}
				}
				break;

			default:
				break;
			}
		}
		break;

	default:
		break;
	}
}




#if defined (WJKTEST) || defined(LC_USA)
void CServer::WriteDBPlayerCount()
{
	if(m_pulseSaveTableCount <= m_pulse)
	{
		CDBCmd dbUser;
		dbUser.Init(&gserver.m_dbuser);
		
		int i, k;
		for(i = 0; i < m_nConnector; ++i)
		{
			if(m_connector[i])
			{
				for(k = 0; k < m_connector[i]->m_nMaxServer; ++k)
				{
					
					sprintf(g_buf,"REPLACE INTO t_connect_status (a_server, a_sub_num, a_count) VALUES ( %d, %d, %d )", 
						m_connector[i]->m_serverNo, m_connector[i]->m_serverSubNo[k], m_connector[i]->m_playerNum[k]);
					
					dbUser.SetQuery(g_buf);
					dbUser.Update();
				}
			}
		}

		m_pulseSaveTableCount  +=  60 * 5 * PASSES_PER_SEC;
	}
}

void CServer::BlockIPLog( const char* user_ip )
{
	CDBCmd dbUser;
	dbUser.Init( &gserver.m_dbuser );

	sprintf( g_buf, "INSERT INTO t_ip_log (a_ip, a_date) VALUES ( '%s', NOW() )", user_ip );
	dbUser.SetQuery( g_buf );
	dbUser.Update();
}
#endif	// #if defined (WJKTEST) || defined(LC_USA)


