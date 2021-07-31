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
: m_ssock(-1)
, m_listParty(CParty::CompParty)
#ifdef PARTY_MATCHING
, m_listPartyMatchMember(CPartyMatchMember::CompByIndex)
, m_listPartyMatchParty(CPartyMatchParty::CompByBossIndex)
#endif // PARTY_MATCHING

#ifdef EXPEDITION_RAID
, m_listExped(CExpedition::CompExpedition)
#endif //EXPEDITION_RAID
{
	m_desclist = NULL;
	m_nDesc = 0;
	FD_ZERO(&m_input_set);
	FD_ZERO(&m_output_set);
	FD_ZERO(&m_exc_set);
	FD_ZERO(&m_null_set);

	m_seqGen = 1;

	m_serverpath = GetServerPath();
	m_maxplayers = 0;
	m_bshutdown = false;
	m_nameserverisslow = true;

	m_pulse = 0;
	m_pulseServerTime = 0;

	m_nMoonStoneNas = 0;

	mysql_init(&m_dbchar);
}

CServer::~CServer()
{
	if (m_serverpath)
		delete[] m_serverpath;

	while (m_listParty.GetCount() > 0)
	{
		CParty* pParty = m_listParty.GetData(m_listParty.GetHead());
		m_listParty.Remove(m_listParty.GetHead());
		delete pParty;
	}

#ifdef EXPEDITION_RAID
	while (m_listExped.GetCount() > 0)
	{
		CExpedition* pExped = m_listExped.GetData(m_listExped.GetHead());
		m_listExped.Remove(m_listExped.GetHead());
		delete pExped;
	}
#endif //EXPEDITION_RAID

#ifdef PARTY_MATCHING
	while (m_listPartyMatchMember.GetCount() > 0)
	{
		CPartyMatchMember* pMember = m_listPartyMatchMember.GetData(m_listPartyMatchMember.GetHead());
		m_listPartyMatchMember.Remove(m_listPartyMatchMember.GetHead());
		delete pMember;
	}
	while (m_listPartyMatchParty.GetCount() > 0)
	{
		CPartyMatchParty* pParty = m_listPartyMatchParty.GetData(m_listPartyMatchParty.GetHead());
		m_listPartyMatchParty.Remove(m_listPartyMatchParty.GetHead());
		delete pParty;
	}
#endif // PARTY_MATCHING
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

	m_serverno = atoi(m_config.Find("Server", "Number"));

	return true;
}

bool CServer::InitGame()
{
	ServerSrandom(time(0));
	MakeMath();

	GAMELOG << init("SYSTEM") << "Finding player limit." << end;
	m_maxplayers = GetMaxPlayers();

	GAMELOG << init("SYSTEM") << "Opening mother connection." << end;
	m_ssock = InitSocket();

	GAMELOG << init("SYSTEM") << "Entering game loop." << end;

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
		GAMELOG << init("SYS_ERR") << "Non-positive max player limit!  (Set at" << max_descs << "using" << method << ")" << end;
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
	// 윈속 초기화
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
		GAMELOG << init("SYSTEM") << "Max players set to" << m_maxplayers << end;

		// 소켓 생성
		if ((s = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		{
			puts("SYSERR: Error opening network connection : Winsock error");
			exit(1);
		}
	}
#else
	// 소켓 생성
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		GAMELOG << init("SYS_ERR") << "Error creating socket" << end;
		exit(1);
	}
#endif	/* CIRCLE_WINDOWS */

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
			GAMELOG << init("SYS_ERR") << "setsockopt SO_LINGER" << end;	/* Not fatal I suppose. */
	}
#endif

	/* Clear the structure */
	memset((char *)&sa, 0, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons((unsigned short)atoi(gserver.m_config.Find("Server", "Port")));
	sa.sin_addr = *(GetBindAddr());

	if (bind(s, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		GAMELOG << init("SYS_ERR") << "bind" << end;
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
		GAMELOG << init("SYS_ERR") << "setsockopt SNDBUF" << end;
		return (-1);
	}

# if 0
	if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *) &opt, sizeof(opt)) < 0)
	{
		GAMELOG << init("SYS_ERR") << "setsockopt RCVBUF" << end;
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
		GAMELOG << init("SYSTEM") << "Binding only to IP address" << inet_ntoa(bind_addr) << end;
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
		GAMELOG << init("SYS_ERR") << "Fatal error executing nonblock (CServer.cpp)" << end;
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
			GAMELOG << init("SYS_ERR") << "gethostbyaddr" << end;

		/* find the numeric site address */
		newd->m_host.CopyFrom((char*)inet_ntoa(peer.sin_addr), HOST_LENGTH);
	} else {
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
	if (d->m_logined >= 1 && STATE(d) != CON_DISCONNECT)
	{
		STATE(d) = CON_DISCONNECT;
		d->m_logined = 2;		// m_logined but not play ==> want to disconnect
		return;
	}
	else
	{
		if (STATE(d) != CON_CLOSE && STATE(d) != CON_DISCONNECT)
		{
			const char* kstrExceptionIP= "222.234.226.20";
			if( strncmp( d->m_host, kstrExceptionIP, d->m_host.Length() ) != 0 )
			{
				GAMELOG << init("SYS_ERR") << "Close Socket Error :" << STATE(d) << delim << d->m_host << end;
			}
		}
	}

	REMOVE_FROM_BILIST(d, m_desclist, m_pPrev, m_pNext);
	d->CloseSocket();

	m_msgList.RemoveServer(d);

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

bool CServer::ConnectDB()
{
	if (!mysql_real_connect (
		&m_dbchar,
		m_config.Find("Char DB", "IP"),
		m_config.Find("Char DB", "User"),
		m_config.Find("Char DB", "Password"),
		m_config.Find("Char DB", "DBName"),
		0, NULL, 0))
		return false;
	return true;
}

void CServer::DisconnectDB()
{
	mysql_close (&m_dbchar);
}

bool CServer::LoadSettings()
{
	GAMELOG << init("SYSTEM")
			<< "Load settings...."
			<< end;

	// LOADING
	GAMELOG << init("SYSTEM")
			<< "Load Guild...."
			<< end;

	CDBCmd cmd;
	cmd.Init(&m_dbchar);

#ifdef NEW_GUILD

	// check t_extend_guild, t_extend_guildmember, t_characters_guildpoint
	//<===! kjban add 
	CDBCmd cmdNewGuild;
	int cnt;

	cmdNewGuild.Init(&m_dbchar);
	cmdNewGuild.SetQuery( "SELECT count(*) FROM t_characters_guildpoint" );
	if( !cmdNewGuild.Open() )
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_characters_guildpoint") << end;
		return false;
	}

	cmdNewGuild.MoveFirst();
	cmdNewGuild.GetRec(0, cnt);
	
	if(cnt == 0)
	{	// 테이블에 내용이 없을때
		cmdNewGuild.SetQuery("insert into t_characters_guildpoint select a_index, 0, 0 from t_characters");
		cmdNewGuild.Open();
	}

	cmdNewGuild.SetQuery("SELECT count(*) FROM t_extend_guild");
	if (!cmdNewGuild.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_extend_guild") << end;
		return false;
	}

	cmdNewGuild.MoveFirst();
	cmdNewGuild.GetRec(0, cnt);
	
	if(cnt == 0)
	{	// 테이블에 내용이 없을때
		cmdNewGuild.SetQuery("insert into t_extend_guild select a_index, 0, 5+a_level*5, 0, '', '', '' from t_guild");
		cmdNewGuild.Open();
	}

	cmdNewGuild.SetQuery("SELECT count(*) FROM t_extend_guildmember");
	if (!cmdNewGuild.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_extend_guildmember") << end;
		return false;
	}

	cmdNewGuild.MoveFirst();
	cmdNewGuild.GetRec(0, cnt);
	
	if(cnt == 0)
	{	// 테이블에 내용이 없을때
		cmdNewGuild.SetQuery("insert into t_extend_guildmember select a_guild_index, a_char_index, '', 0, 0, 0 from t_guildmember");
		cmdNewGuild.Open();
	}
	//!===>

	cmd.SetQuery( "SELECT g.*, eg.a_guild_point, eg.a_guild_incline, eg.a_guild_maxmember, eg.a_guild_land, eg.a_skill_index, eg.a_skill_level"
				  " FROM t_guild AS g, t_extend_guild AS eg WHERE g.a_index = eg.a_guild_index AND g.a_enable = 1 ORDER BY g.a_index " );
#else
	cmd.SetQuery("SELECT * FROM t_guild WHERE a_enable=1 ORDER BY a_index");
#endif // NEW_GUILD
	if (!cmd.Open())
	{
		GAMELOG << init("SYS_ERR")
				<< "Cannot load guild"
				<< end;
		return false;
	}

	CDBCmd cmd2;
	cmd2.Init(&m_dbchar);
	while (cmd.MoveNext())
	{
		int guildindex;
		CLCString guildname(MAX_GUILD_NAME_LENGTH + 1);
		int guildlevel;

		int battleIndex;
		int battlePrize;
		int battleZone;
		int battleTime;
		int battleKillCount;
		int battleState;

		cmd.GetRec("a_index", guildindex);
		cmd.GetRec("a_name", guildname);
		cmd.GetRec("a_level", guildlevel);

		cmd.GetRec("a_battle_index",	battleIndex);
		cmd.GetRec("a_battle_prize",	battlePrize);
		cmd.GetRec("a_battle_zone",		battleZone);
		cmd.GetRec("a_battle_time",		battleTime);
		cmd.GetRec("a_battle_killcount",battleKillCount);
		cmd.GetRec("a_battle_state",	battleState);

#ifdef NEW_GUILD
		int guildpoint;
		int incline;
		CLCString	guildland(256);
		CLCString	guildskillindex(256);
		CLCString	guildskillLevel(256);
		int	maxmember;
// 		char tbuf[256];
//		const char* pland = guildland;
		int land[256];
		
		memset( land, -1, sizeof(land) );

		cmd.GetRec( "a_guild_point", guildpoint );
		cmd.GetRec( "a_guild_incline", incline );
		cmd.GetRec( "a_guild_maxmember", maxmember );

		CDBCmd Castle;
		Castle.Init(&m_dbchar);
		CLCString sql( 256 );
		sql.Format( "SELECT a_zone_index  FROM t_castle  WHERE a_owner_guild_index = %d", guildindex );

		Castle.SetQuery( sql );
		if (!Castle.Open() )
		{
			GAMELOG << init("SYS_ERR")
					<< "Cannot load castle"
					<< end;
			return false;
		}	
		int landcount = Castle.m_nrecords;
		if( landcount > 0 && Castle.MoveFirst() )
		{
			int i =0;			
		
			do
			{
				Castle.GetRec( "a_zone_index", land[i] ); 
				i++;
			}while( Castle.MoveNext() );
		}

//#endif // NEW_GUILD
		
		sprintf(g_buf, "SELECT g.*, eg.a_position_name, eg.a_contribute_exp, eg.a_contribute_fame, a_point"
			" FROM t_guildmember AS g, t_extend_guildmember AS eg "
			" WHERE g.a_guild_index = eg.a_guild_index AND g.a_char_index = eg.a_char_index AND g.a_guild_index =%d ", guildindex);
	
#else
		sprintf(g_buf, "SELECT * FROM t_guildmember WHERE a_guild_index=%d order by a_char_index", guildindex);		
#endif // NEW_GUILD
		cmd2.SetQuery(g_buf);
		if (!cmd2.Open())
		{
			GAMELOG << init("SYS_ERR")
					<< "Cannot load guild"
					<< end;
			return false;
		}

		if (cmd2.m_nrecords == 0)
		{
			DeleteGuild(guildindex);
			continue ;
		}

		// TODO : GUILD : 차후 트리 구조 고려
		CGuild * guild = m_guildlist.create(guildindex, guildname, guildlevel, battleIndex, battlePrize, battleTime, battleZone, battleKillCount, battleState);

		if (guild == NULL)
			continue ;

		m_guildlist.add(guild);
#ifdef NEW_GUILD
		GAMELOG << init("NEW_GUILD" ) 
			<< "guildIndex" << delim << guild->index() << delim 
			<< "guildPoint" << delim << guildpoint << delim 
			<< "incline" << delim << incline << delim
			<< "landcount" << delim << landcount << delim
			<< "Maxmember" << delim << maxmember << delim;	
#ifdef NEW_GUILD_BUG_FIX
		const char* pIndex = guildskillindex;
		const char* pLevel = guildskillLevel;
		char tbuf[256];

		int sindex[256];
		int sLevel[256];
		memset( sindex, 0, 256 );
		memset( sLevel, 0, 256 );
	
		cmd.GetRec( "a_skill_index", guildskillindex );
		cmd.GetRec( "a_skill_level", guildskillLevel );

		int roopcount = 0;
		while (*pIndex && *pLevel)
		{
			int i, l;
			pIndex = AnyOneArg(pIndex, tbuf);
			i = atoi(tbuf);
			pLevel = AnyOneArg(pLevel, tbuf);
			l = atoi(tbuf);

			sindex[roopcount] = i;
			sLevel[roopcount] = l;
			roopcount++;
		}		

		if( sindex[1] == 444 && sLevel[1] > 0 && sLevel[1] < 6 )
		{	
			// 일단 정확한 레벨에 의한 MAXMEMBER 확인
			int bugFixMaxMeber = 0;
			if( sLevel[1] * 5 + 30 != maxmember )
			{
				bugFixMaxMeber = sLevel[1] * 5 + 30;
			
				CDBCmd bugFixCmd;
				bugFixCmd.Init(&m_dbchar);
				sprintf(g_buf, "SELECT count(a_guild_index) as memberCount FROM t_guildmember WHERE a_guild_index=%d group by a_guild_index", guild->index() );		
				bugFixCmd.SetQuery(g_buf);
				int bugFixLevel=sLevel[1];
				if( bugFixCmd.Open() && bugFixCmd.MoveNext() )
				{
					int nMemberCount=0;
					bugFixCmd.GetRec("memberCount", nMemberCount );
					if( nMemberCount > bugFixMaxMeber )		// 버그 조정
					{
						if ( nMemberCount>91 && nMemberCount <= 100 )		{ bugFixLevel = 10; bugFixMaxMeber=100; }
						else if ( nMemberCount>82 && nMemberCount <= 91 )	{ bugFixLevel = 9; bugFixMaxMeber=91; }
						else if ( nMemberCount>73 && nMemberCount <= 82 )	{ bugFixLevel = 8; bugFixMaxMeber=82; }
						else if ( nMemberCount>64 && nMemberCount <= 73 )	{ bugFixLevel = 7; bugFixMaxMeber=73; }
						else if ( nMemberCount>55 && nMemberCount <= 64 )	{ bugFixLevel = 6; bugFixMaxMeber=64; }
						else if ( nMemberCount>50 && nMemberCount <= 55 )	{ bugFixLevel = 5; bugFixMaxMeber=55; }
						else if ( nMemberCount>45 && nMemberCount <= 50 )	{ bugFixLevel = 4; bugFixMaxMeber=50; }
						else if ( nMemberCount>40 && nMemberCount <= 45 )	{ bugFixLevel = 3; bugFixMaxMeber=45; }
						else if ( nMemberCount>35 && nMemberCount <= 40 )	{ bugFixLevel = 2; bugFixMaxMeber=40; }
						else if ( nMemberCount>30 && nMemberCount <= 35 )	{ bugFixLevel = 1; bugFixMaxMeber=35; }
					}
				}

				// UPDATE BugFixLevel BugFixMaxMember
				maxmember = bugFixMaxMeber;

				sprintf(g_buf, "UPDATE t_extend_guild SET a_guild_maxmember=%d, a_skill_level='1 %d' WHERE a_guild_index=%d ", bugFixMaxMeber, bugFixLevel,  guild->index() );		
				bugFixCmd.SetQuery(g_buf);
				bugFixCmd.Update();				
			}
		}
#endif // NEW_GUILD_BUG_FIX
		guild->guildPoint( guildpoint );
		guild->incline( incline );
		guild->landcount( landcount );
		if( landcount >  0 )
			guild->land( landcount, land );
		guild->setMaxmember( maxmember );

		
#endif // NEW_GUILD

		bool bFoundBoss = false;

		while (cmd2.MoveNext())
		{
			int charindex;
			CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
			int pos;

			cmd2.GetRec("a_char_index", charindex);
			cmd2.GetRec("a_char_name", charname);
			cmd2.GetRec("a_pos", pos);


			int listidx = guild->addmember(charindex, charname);
			switch (pos)
			{
			case MSG_GUILD_POSITION_BOSS:
				bFoundBoss = true;
				guild->changeboss(listidx);
				break;

			case MSG_GUILD_POSITION_OFFICER:
				guild->appoint(listidx);
				break;
			}
#ifdef NEW_GUILD
			CLCString positionname( GUILD_POSITION_NAME + 1 );
			//char positionname[GUILD_POSITION_NAME+1];
			int	contributeExp;
			int contributeFame;
			int point;

			cmd2.GetRec( "a_position_name", positionname );
			cmd2.GetRec( "a_contribute_exp", contributeExp );
			cmd2.GetRec( "a_contribute_fame", contributeFame );
			cmd2.GetRec( "a_point", point );

			guild->member(listidx)->contributeExp( contributeExp );
			guild->member(listidx)->contributeFame( contributeFame );
			guild->member(listidx)->positionName( positionname );
			guild->member(listidx)->cumulatePoint( point );
#endif // NEW_GUILD

		}

		if (!bFoundBoss)
		{
			if (guild->member(0))
				guild->changeboss(0);
		}
	}

#ifdef GUILD_MARK_TABLE
	CDBCmd cmd3;
	cmd3.Init(&m_dbchar);	
	sprintf(g_buf, "SELECT * FROM t_guildmark WHERE a_server=%d", gserver.m_serverno);
	cmd3.SetQuery(g_buf);

	if (!cmd3.Open())
	{
		GAMELOG << init("SYS_ERR")
				<< "Cannot load guildmark"
				<< end;
		return false;
	}
	
	int rank = 0, idx = 0;
	while(cmd3.MoveNext())
	{
		cmd3.GetRec("a_rank", rank);
		cmd3.GetRec("a_idx", idx);
		if( rank < 4 && rank > 0)
		{
			gserver.m_nGuildMarkTable[rank-1] = idx;
		}
	}

#endif // GUILD_MARK_TABLE

#ifdef REWARD_IDC2007
	cmd.SetQuery("SELECT * FROM t_reward_idc2007 LIMIT 1");
	if(!cmd.Open())
	{
		GAMELOG << init("SYS_ERR")
			<< "cannot load t_reward_idc2007"
			<< end;
		return false;
	}
#endif // REWARD_IDC2007

#ifdef EXTREME_CUBE
	cmd.SetQuery("SELECT * FROM t_cubepoint LIMIT 1");
	if(!cmd.Open())
	{
		GAMELOG << init("SYS_ERR")
				<< "cannot load t_cubepoint"
				<< end;
		return false;
	}
	cmd.SetQuery("SELECT * FROM t_cubepoint_personal LIMIT 1");
	if(!cmd.Open())
	{
		GAMELOG << init("SYS_ERR")
				<< "cannot load t_cubepoint_personal"
				<< end;
		return false;
	}
#endif // EXTREME_CUBE

#ifdef SAVE_MOONSTONE_MIX
	cmd.SetQuery("SELECT * FROM t_event_moonstone LIMIT 1");
	if(!cmd.Open())
	{
		GAMELOG << init("SYS_ERR")
				<< "cannot load t_event_moonstone"
				<< end;
		return false;
	}

#endif // SAVE_MOONSTONE_MIX
	

#ifdef TRADE_AGENT		//거래 대행(경매) 

	// LOADING
	GAMELOG << init("SYSTEM")
			<< "Load TradeAgent...."
			<< end;

	// DB 데이타 로드 
	gserver.m_TradeAgentList.Load();

#endif	//TRADE_AGENT


	return true;
}

void CServer::SendToAllGameServer(CNetMsg& msg)
{
	CDescriptor* d = m_desclist;
	while (d)
	{
		if (d->m_countZone && STATE(d) == CON_PLAYING)
		{
			SEND_Q(msg, d);
		}
		d = d->m_pNext;
	}
}

bool CServer::DeleteGuild(int guildindex)
{
	// DB
	struct tm ti = NOW();
	sprintf(g_buf, "UPDATE t_guild SET a_name=CONCAT(a_name,'_%02d%02d%02d%02d%02d%02d'), a_enable=0, a_recentdate=NOW() WHERE a_index=%d",
			ti.tm_year, ti.tm_mon, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec, guildindex);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
		return false;

	return true;
}

bool CServer::DeleteGuildMember(int guildindex, const char* guildname, int charindex, const char* charname, bool bKick)
{
	// DB
	sprintf(g_buf, "DELETE FROM t_guildmember WHERE a_guild_index=%d AND a_char_index=%d", guildindex, charindex);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
		return false;

#ifdef NEW_GUILD
	sprintf(g_buf, "DELETE FROM t_extend_guildmember WHERE a_guild_index=%d AND a_char_index=%d", guildindex, charindex);
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
		return false;
#endif // NEW_GUILD

	sprintf(g_buf, "UPDATE t_guild SET a_recentdate=NOW() WHERE a_index=%d", guildindex);
	cmd.SetQuery(g_buf);
	cmd.Update();

	if (bKick)
	{
		GAMELOG << init("GUILD MEMBER KICK")
				<< guildindex << delim
				<< guildname << delim
				<< "CHARACTER" << delim
				<< charindex << delim
				<< charname
				<< end;
	}
	else
	{
		GAMELOG << init("GUILD MEMBER OUT")
				<< guildindex << delim
				<< guildname << delim
				<< "CHARACTER" << delim
				<< charindex << delim
				<< charname
				<< end;
	}

	return true;
}

void CServer::SendToAllSubServer(CNetMsg& msg, int nSubNo)
{
	CDescriptor* d = m_desclist;
	while (d)
	{
		if (d->m_countZone && STATE(d) == CON_PLAYING && d->m_subNo == nSubNo)
		{
			SEND_Q(msg, d);
		}
		d = d->m_pNext;
	}
}

CParty* CServer::FindPartyByBossIndex(int nSubNo, int nBossIndex)
{
	CParty partyFind(nSubNo, 0, nBossIndex, "", 0, "");
	return m_listParty.GetData(m_listParty.FindData(&partyFind));
}

CParty* CServer::FindPartyByMemberIndex(int nSubNo, int nMemberIndex, bool bIncludeRequest)
{
	void* pos = m_listParty.GetHead();
	while (pos)
	{
		CParty* pParty = m_listParty.GetData(pos);
		if (pParty->GetSubNo() == nSubNo)
		{
			if (pParty->FindMember(nMemberIndex) != -1)
				return pParty;
			if (bIncludeRequest && pParty->GetRequestIndex() == nMemberIndex)
				return pParty;
		}
		pos = m_listParty.GetNext(pos);
	}
	return NULL;
}

#ifdef EXPEDITION_RAID
CExpedition* CServer::FindExpedByBossIndex(int nSubNo, int nBossIndex)
{
	CExpedition ExpedFind(nSubNo, 0, nBossIndex, "", 0, 0);
	return m_listExped.GetData(m_listExped.FindData(&ExpedFind));
}

CExpedition* CServer::FindExpedByMemberIndex(int nSubNo, int nMemberIndex, bool bIncludeRequest)
{
	void* pos = m_listExped.GetHead();
	while (pos)
	{
		CExpedition* pExped = m_listExped.GetData(pos);
		if (pExped->GetSubNo() == nSubNo)
		{
			if (pExped->FindMemberListIndex(nMemberIndex) != -1)
				return pExped;
			if (bIncludeRequest && pExped->GetRequestIndex() == nMemberIndex)
				return pExped;
		}
		pos = m_listExped.GetNext(pos);
	}
	return NULL;
}
#endif


#ifdef PARTY_MATCHING
CPartyMatchMember* CServer::FindPartyMatchMemberByCharIndex(int nSubNo, int nCharIndex)
{
	CPartyMatchMember matchFind(nSubNo, nCharIndex, "", 0, 0, 0, 0);
	return m_listPartyMatchMember.GetData(m_listPartyMatchMember.FindData(&matchFind));
}

CPartyMatchParty* CServer::FindPartyMatchPartyByBossIndex(int nSubNo, int nBossIndex)
{
	CPartyMatchParty matchFind(nSubNo, nBossIndex, "", 0, 0, 0, 0, false, "");
	return m_listPartyMatchParty.GetData(m_listPartyMatchParty.FindData(&matchFind));
}
#endif // PARTY_MATCHING


void CServer::SendGuildMemberList(CGuild* guild, CDescriptor* desc)
{
	const int maxmember = 20;

	CGuildMember* guildmember[maxmember];
	CGuildMember* member;
	CNetMsg rmsg;

	int startidx = 0;			// 원본 데이터 index
	int count;					// 1회 전송시 데이터 count
	int sendcount = 0;			// 전체 전송완료한 데이터 count
	int sendmaxmember = guild->membercount();			// 데이터 보내야할 전체 길드멤버 count

	do
	{
		count = 0;
		memset(guildmember, 0, sizeof(CGuildMember*) * maxmember);
		for( ; startidx < GUILD_MAX_MEMBER && count < maxmember; ++startidx)
		{
			member = guild->member(startidx);
			if(member)
			{
				guildmember[count] = member;
				count++;
			}
		}
		
		if(count > 0)
		{
			rmsg.Init(MSG_HELPER_COMMAND);
			rmsg << MSG_HELPER_GUILD_MEMBER_LIST
				<< guild->index()
				<< count;
			
			int i;
			for(i = 0; i < count; ++i)
			{
				if(guildmember[i])
				{
					rmsg << guildmember[i]->charindex()
						 << guildmember[i]->GetName()
						 << guildmember[i]->pos()
						 << guildmember[i]->online();
				}
				else
				{
					// 로그찍기 : error

				}
			}
			
			SEND_Q(rmsg, desc);

			sendcount += count;
		}

	}while( (startidx < GUILD_MAX_MEMBER) && sendcount < sendmaxmember);
}


void CServer::SendExtendGuildMemberList(CGuild* guild, CDescriptor* desc)
{
	const int maxmember = 20;

	CGuildMember* guildmember[maxmember];
	CGuildMember* member;
	CNetMsg rmsg;

	int startidx = 0;
	int count;
	int sendcount = 0;
	int sendmaxmember = guild->membercount();

	do
	{
		count = 0;
		memset(guildmember, 0, sizeof(CGuildMember*) * maxmember);
		for( ; startidx < GUILD_MAX_MEMBER && count < maxmember; ++startidx)
		{
			member = guild->member(startidx);
			if(member)
			{
				guildmember[count] = member;
				count++;
			}
		}
		
		if(count > 0)
		{
			rmsg.Init(MSG_HELPER_COMMAND);
			rmsg << MSG_HELPER_NEW_GUILD_MEMBER_LIST
				<< guild->index()
				<< count;
			
			int i;
			for(i = 0; i < count; ++i)
			{
				if(guildmember[i])
				{
					rmsg << guildmember[i]->charindex()
						 << guildmember[i]->GetcontributeExp()
						 << guildmember[i]->GetcontributeFame()
						 << guildmember[i]->GetcumulatePoint()
						 << guildmember[i]->GetPositionName()
						 << guildmember[i]->GetChannel()
						 << guildmember[i]->GetZoneNum();
				}
				else
				{
					// 로그찍기 error
				}
			}
			
			SEND_Q(rmsg, desc);

			sendcount += count;
		}

	}while( (startidx < GUILD_MAX_MEMBER) && sendcount < sendmaxmember);
}