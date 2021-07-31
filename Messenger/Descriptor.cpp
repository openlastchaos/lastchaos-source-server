#include "stdhdrs.h"
#include "Log.h"
#include "Descriptor.h"
#include "Server.h"
#include "CmdMsg.h"

CDescriptor::CDescriptor()
: m_host(HOST_LENGTH + 1)
{
	m_serverNo = -1;
	m_subNo = -1;
	m_zones = NULL;
	m_countZone = 0;

	m_desc = 0;

	m_timestamp = -1;

	m_connected = 0;
	m_connectreq = 0;
	m_logined	= 0;
	m_descnum	= 0;
	m_bclosed	= false;
	m_dbrunning = false;

	m_pPrev = NULL;
	m_pNext = NULL;
}

CDescriptor::~CDescriptor()
{
	if (m_zones)
		delete [] m_zones;
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
			GAMELOG << init("SYS_ERR") << "WriteToDescriptor : no =" << errno << end;
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
			GAMELOG << init("SYS_WARN") << "m_input overflow" << end;
			return -1;
		}

		bytes_read = PerformSocketRead(m_desc, (char*)m_inBuf.GetPoint(), m_inBuf.GetRemain());

		if (bytes_read < 0)	/* Error, disconnect them. */
			return -1;
		else if (bytes_read == 0)	/* Just blocking, no problems. */
			return 0;

		m_inBuf.SetPoint(bytes_read);

		/* at this point, we know we got some data from the read */

		// Packet ľň±â
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

	if (msg.m_mtype != MSG_MSGR_CONNECT)
		return false;

	int version, servernum, subnum, count_zone;

	msg >> version
		>> servernum
		>> subnum
		>> count_zone;

	if (version < SERVER_MIN_VERSION || version > SERVER_MAX_VERSION)
		return false;

	m_serverNo = servernum;
	m_subNo = subnum;

	if (servernum != LOGIN_SERVER_NUM && servernum != CONNECTOR_SERVER_NUM)
	{
		if (count_zone < 1)
			return false;

		m_countZone = count_zone;
		m_zones = new int[m_countZone];

		int i;
		int zone_num;
		for (i = 0; i < m_countZone; i++)
		{
			msg >> zone_num;
			m_zones[i] = zone_num;
		}

		CNetMsg rmsg;

#ifdef RECOMMEND_SERVER_SYSTEM
		MsgrRecommendMsg(rmsg);
		SEND_Q(rmsg, this);
#endif // RECOMMEND_SERVER_SYSTEM

		switch (gserver.m_sGoldenBallData.nStatus)
		{
		case GOLDENBALL_STATUS_VOTE:
			MsgrEventGoldenballVoteMsg(rmsg, 0, gserver.m_sGoldenBallData.strTeam1, gserver.m_sGoldenBallData.strTeam2, gserver.m_sGoldenBallData.nYear, gserver.m_sGoldenBallData.nMonth, gserver.m_sGoldenBallData.nDay, gserver.m_sGoldenBallData.nHour, gserver.m_sGoldenBallData.nMin);
			SEND_Q(rmsg, this);
			break;

		case GOLDENBALL_STATUS_GIFT:
			MsgrEventGoldenballGiftMsg(rmsg, 0, gserver.m_sGoldenBallData.strTeam1, gserver.m_sGoldenBallData.nTeam1Score, gserver.m_sGoldenBallData.strTeam2, gserver.m_sGoldenBallData.nTeam2Score, gserver.m_sGoldenBallData.nYear, gserver.m_sGoldenBallData.nMonth, gserver.m_sGoldenBallData.nDay, gserver.m_sGoldenBallData.nHour, gserver.m_sGoldenBallData.nMin, gserver.m_sGoldenBallData.timeEndVote);
			SEND_Q(rmsg, this);
			break;
		}
	}
	else
	{
		m_countZone = 0;
		m_zones = NULL;

#ifdef RECOMMEND_SERVER_SYSTEM
		if (servernum == LOGIN_SERVER_NUM)
		{
			CNetMsg rmsg;
			MsgrRecommendMsg(rmsg);
			SEND_Q(rmsg, this);
		}
#endif // RECOMMEND_SERVER_SYSTEM
	}

	GAMELOG << init("CONNECTED", m_host)
			<< version << delim
			<< m_serverNo << delim
			<< m_subNo << delim
			<< m_countZone
			<< end;


	STATE(this) = CON_PLAYING;

	return true;
}

int CDescriptor::FindZone(int zone)
{
	int i;
	for (i = 0; i < m_countZone; i++)
	{
		if (m_zones[i] == zone)
			return i;
	}

	return -1;
}
