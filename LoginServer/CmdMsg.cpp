#include "stdhdrs.h"
#include "CmdMsg.h"
#include "Server.h"
#include "DBCmd.h"


void FailMsg(CNetMsg& msg, MSG_FAIL_TYPE failtype)
{
	msg.Init(MSG_FAIL);
	msg << (unsigned char)failtype;
}

void LogoutReqMsg(CNetMsg& msg, int server, int subnum, int zone, CDescriptor* d)
{
	msg.Init(MSG_MSGR_REQ);

	msg << 0
		<< server
		<< subnum
		<< zone
		<< (unsigned char)MSG_MSGR_LOGOUT_REQ
		<< d->m_idname
		<< d->m_passwd;
}

void PlayerReqMsg(CNetMsg& msg)
{
	msg.Init(MSG_CONN_REQ);

	msg << (unsigned char)MSG_CONN_PLAYER_REQ;
}

void PlayerNumMsg(CNetMsg& msg, int recentServer, int recentSubNum, int idx)
{
	msg.Init(MSG_LOGINSERV_PLAYER);

	msg << recentServer
		<< recentSubNum
		<< gserver.m_nConnector
		<< (idx + 1);

	if (gserver.m_connector[idx] && !gserver.m_brunconn[idx])
	{
		msg << gserver.m_connector[idx]->m_serverNo
			<< gserver.m_connector[idx]->m_nMaxServer;

#ifdef RECOMMEND_SERVER_SYSTEM
		if (gserver.m_connector[idx]->m_serverNo == gserver.m_nRecommendServer)
			msg << (char)1;
		else
			msg << (char)0;
#endif // RECOMMEND_SERVER_SYSTEM

		int i;
		for (i=0; i < gserver.m_connector[idx]->m_nMaxServer; i++)
		{
			msg << gserver.m_connector[idx]->m_serverSubNo[i]
				<< gserver.m_connector[idx]->m_playerNum[i] + 1999
				<< gserver.m_connector[idx]->m_ipAddr[i]
				<< gserver.m_connector[idx]->m_portNumber[i];
		}
	}
	else
	{
		strcpy(g_buf, "Connector_");
		IntCat(g_buf, idx, false);
		int sno = atoi(gserver.m_config.Find(g_buf, "Number"));
		int max = atoi(gserver.m_config.Find(g_buf, "MaxSubNumber"));
		msg << sno
			<< max;

#ifdef RECOMMEND_SERVER_SYSTEM
		if (sno == gserver.m_nRecommendServer)
			msg << (char)1;
		else
			msg << (char)0;
#endif // RECOMMEND_SERVER_SYSTEM

		int i;
		for (i=0; i < gserver.m_connector[idx]->m_nMaxServer; i++)
		{
			int subno = i + 1;
			int playernum = -1;
			int port = 0;
			msg << subno
				<< playernum + 1999
				<< ""
				<< port;
		}
	}
}

void ConLogoutReqMsg(CNetMsg& msg, char* id)
{
	msg.Init(MSG_CONN_REQ);

	msg << (unsigned char)MSG_CONN_LOGOUT_REQ
		<< id;
}

void PlayerRealIP( CNetMsg& msg, const char* ip )
{
//	msg.Init( MSG_EXTEND );
//	msg << MSG_EX_PLAYER_REALIP
	msg.Init( MSG_LOGIN );
	msg << ip;
}

#ifdef CHECK_SECURE_CARD
void CheckSecureCardMsg(CNetMsg & msg, CDescriptor * desc)
{
	msg.Init(MSG_EXTEND);
	msg << (unsigned long)MSG_EX_SECURE_CARD;
	for(int i = 0; i < 4; i++)
	{
		msg << desc->m_scard[i];
	}
}
#endif // CHECK_SECURE_CARD