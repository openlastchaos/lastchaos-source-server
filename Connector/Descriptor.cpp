#include "stdhdrs.h"
#include "Log.h"
#include "Descriptor.h"
#include "Server.h"
#include "CmdMsg.h"

CDescriptor::CDescriptor()
: m_host(HOST_LENGTH + 1)
, m_ipAddr(HOST_LENGTH + 1)
{
	m_desc = 0;

	m_timestamp = -1;

	m_connected = 0;
	m_connectreq = 0;
	m_descnum	= 0;
	m_playmode = MSG_LOGIN_NEW;  // default play mode
	m_bclosed	= false;
	m_bStartServer = false;
	m_bLoginServer = false;
	m_subno = 0;

	m_waitbill = 0;					// 빌링에 요청 결과 기다림
	m_billtimeout = 0;				// 빌링 대기시간
	
	m_nCountZone = 0;
	m_nZoneIndex = NULL;

	m_pPrev = NULL;
	m_pNext = NULL;

}

CDescriptor::~CDescriptor()
{
	if (m_nZoneIndex)
		delete [] m_nZoneIndex;
	m_nZoneIndex = NULL;
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
	if (!m_outBuf.Add(msg, false, NULL))
#else
	if (!m_outBuf.Add(msg))
#endif // #ifdef CRYPT_NET_MSG
	{
		GAMELOG << init("OUTPUT OVERFLOW")
				<< end;

		m_bclosed = true;
		return ;
	}
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
			GAMELOG << init("SYS_ERR") << "Write to descriptor: errno=" << errno << end;
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
	msg.MoveFirst();

	if (msg.m_mtype != MSG_CONN_CONNECT)
	{
		GAMELOG << init("BAD_CONNECTION", m_host)
				<< end;
		return false;
	}

	int version;
	int serverno, subnum, count_zone;
	CLCString address(HOST_LENGTH + 1);
	int port;
	CLCString pw(MAX_PWD_LENGTH + 1);

	msg >> version
		>> serverno
		>> subnum
		>> address
		>> port
		>> count_zone;

	if (serverno == PLAYER_COUNT_NUM)
	{
		msg >> pw;
		if (strcmp(pw, "tjwjddnjsehd") != 0)
		{
			GAMELOG << init("BAD_CONNECTION", m_host)
				<< version
				<< delim
				<< serverno
				<< delim
				<< subnum
				<< end;

			return false;
		}
	}

	// 버전이 맞는지, 서버 군이 맞는지, 혹은 요청한 녀석이 로그인,메신저 서버인지 검사
	if (version < SERVER_MIN_VERSION
			|| version > SERVER_MAX_VERSION
			|| ((serverno != gserver.m_serverno || subnum < 1 || subnum > gserver.m_maxSubServer) && serverno != LOGIN_SERVER_NUM && serverno != MESSENGER_SERVER_NUM && serverno != PLAYER_COUNT_NUM))
	{
		GAMELOG << init("BAD_CONNECTION", m_host)
				<< version
				<< delim
				<< serverno
				<< delim
				<< subnum
				<< end;
		return false;
	}

	if (serverno != LOGIN_SERVER_NUM && serverno!= PLAYER_COUNT_NUM && serverno != CONNECTOR_SERVER_NUM)
	{
		if (subnum < 1 || subnum > gserver.m_maxSubServer)
			return false;

		if (count_zone < 1)
			return false;
#ifdef LIMIT_CATALOG
		// 팔린 상품 갯수를 각 게임서버로 보냄
		CNetMsg rmsg;
		LimitCatalogMsg(rmsg);
		SEND_Q(rmsg, this);
#endif

		m_nCountZone = count_zone;
		m_nZoneIndex = new int[m_nCountZone];

		int i;
		int zone_num;
		for (i = 0; i < count_zone; i++)
		{
			msg >> zone_num;
			m_nZoneIndex[i] = zone_num;
			gserver.m_userList[subnum - 1].m_playersPerZone[zone_num] = 0;
			if (zone_num == ZONE_START)
				m_bStartServer = true;
		}

		m_subno = subnum;

		// 이전에 같은 서버 같은 존으로 등록한거 있으면 접속 불능
		CDescriptor* p;
		CDescriptor* pNext = gserver.m_desclist;
		bool bDuplicationZone = false;
		while ((p = pNext))
		{
			pNext = p->m_pNext;
			if (!p->m_bLoginServer && p->m_subno > 0 && p->m_subno == m_subno && this != p)
			{
				int thisZoneI;
				int pZoneI;
				for (thisZoneI = 0; !bDuplicationZone && thisZoneI < m_nCountZone; thisZoneI++)
				{
					for (pZoneI = 0; pZoneI < p->m_nCountZone; pZoneI++)
					{
						if (m_nZoneIndex[thisZoneI] == p->m_nZoneIndex[pZoneI])
						{
							bDuplicationZone = true;
							break;
						}
					}
				}
				if (bDuplicationZone)
				{
					return false;
				}
			}
		}
	}
	else if (serverno == LOGIN_SERVER_NUM)
	{
		m_bLoginServer = true;
#ifdef LC_TIME
		if( subnum )
			gserver.bStartLctime = true;
		else
			gserver.bStartLctime = false;
#endif
	}

	m_ipAddr = address;
	m_portNumber = port;

	STATE(this) = CON_PLAYING;

	CNetMsg rmsg;
	PlayerRepMsg(rmsg);

	if(serverno == PLAYER_COUNT_NUM )
		return true;

	CDescriptor* desc = gserver.m_desclist;
	while (desc)
	{
		if (desc->m_bLoginServer)
		{
			SEND_Q(rmsg, desc);
		}
		desc = desc->m_pNext;
	}

	return true;
}
