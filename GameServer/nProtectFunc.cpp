#include "stdhdrs.h"
#include "Server.h"
#include "Log.h"

#ifdef USING_NPROTECT

#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0L
#endif // ERROR_SUCCESS

bool InitNProtectGameGuard()
{
	unsigned long dlRet = InitGameguardAuth(NPROTECT_PATH, NPROTECT_ACTIVE_NUM, true, NPROTECT_LOG);
	if (dlRet != ERROR_SUCCESS)
	{
		GAMELOG << init("NPROTECT ERROR")
				<< "InitGameguardAuth()" << delim
				<< (int)dlRet
				<< end;
		return false;
	}
	return true;
}

GGAUTHS_API void NpLog(int nMode, char* strMsg)
{
	if (nMode & NPLOG_DEBUG)
	{
		GAMELOG << init("NPROTECT DEBUG")
				<< strMsg
				<< end;
	}
	else
	{
		GAMELOG << init("NPROTECT LOG")
				<< strMsg
				<< end;
	}
}

GGAUTHS_API void GGAuthUpdateCallback(PGG_UPREPORT report)
{
	GAMELOG << init("NPROTECT AUTH UPDATE");
	if (report->nType == 1)
		GAMELOG << "GAMEGUARD VER" << delim;
	else
		GAMELOG << "PROTOCOL NUM" << delim;
	GAMELOG << (int)report->dwBefore << delim
			<< (int)report->dwNext
			<< end;
}

void ExNProtectMsg(CNetMsg& msg, GG_AUTH_DATA* pAuthData)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_NPROTECT
		<< pAuthData->dwIndex
		<< pAuthData->dwValue1
		<< pAuthData->dwValue2
		<< pAuthData->dwValue3;
}

void do_ExNProtect(CPC* ch, CNetMsg& msg)
{
#ifdef _DEBUG
		*g_buf2 = '\0';
		sprintf(g_buf2, "c->s: size: %d, data: ", msg.m_size);
		int ttt;
		for (ttt = 0; ttt < msg.m_size; ttt++)
		{
			sprintf(g_buf, "%02x ", msg.m_buf[ttt]);
			strcat(g_buf2, g_buf);
		}
		strcat(g_buf2, "\n");
		OutputDebugString(g_buf2);
#endif // _DEBUG
	char buf[2048];
	memset(buf, 0, 2048);
	if (ch->m_desc->m_pGGAuth->CheckAuthMsg(msg, buf) == false)
	{
		GAMELOG << init("NPROTECT ERROR: CheckAuthAnswer() ") << buf << end;
		gserver.CloseSocket(ch->m_desc, false);
		return ;
	}
}

CNProtectAuth::CNProtectAuth()
{
	m_pCSAuth = new CCSAuth2;
	m_pCSAuth->Init();
	m_nTime = -1;
	m_bAnswer = false;
}

CNProtectAuth::~CNProtectAuth()
{
	if (m_pCSAuth)
	{
		m_pCSAuth->Close();
		delete m_pCSAuth;
		m_pCSAuth = NULL;
	}
}

bool CNProtectAuth::MakeGameGuardAuthQuery(CNetMsg& msg)
{
	if (m_pCSAuth->GetAuthQuery() != ERROR_SUCCESS)
		return false;
	ExNProtectMsg(msg, &m_pCSAuth->m_AuthQuery);
	return true;
}

bool CNProtectAuth::CheckAuthMsg(CNetMsg & msg, char * buf)
{
	msg.MoveFirst();
	if (msg.m_mtype != MSG_EXTEND)
	{
		return false;
	}

	int nSubType = 0;
	msg >> nSubType;
	if (nSubType != MSG_EX_NPROTECT)
	{
		return false;
	}

	GG_AUTH_DATA authData;
	msg >> authData.dwIndex
		>> authData.dwValue1
		>> authData.dwValue2
		>> authData.dwValue3;
	memcpy(&m_pCSAuth->m_AuthAnswer, &authData, sizeof(authData));

	m_bAnswer = true;

	unsigned long nRet = m_pCSAuth->CheckAuthAnswer();
	if (nRet != ERROR_SUCCESS)
	{
		sprintf(buf, " [ERRCODE:%ld] Query : %08X %08X %08X %08X  :: Answer : %08X %08X %08X %08X", nRet, 
			(unsigned int)m_pCSAuth->m_AuthQuery.dwIndex, 
			(unsigned int)m_pCSAuth->m_AuthQuery.dwValue1, 
			(unsigned int)m_pCSAuth->m_AuthQuery.dwValue2, 
			(unsigned int)m_pCSAuth->m_AuthQuery.dwValue3,
			(unsigned int)m_pCSAuth->m_AuthAnswer.dwIndex, 
			(unsigned int)m_pCSAuth->m_AuthAnswer.dwValue1, 
			(unsigned int)m_pCSAuth->m_AuthAnswer.dwValue2, 
			(unsigned int)m_pCSAuth->m_AuthAnswer.dwValue3);

		return false;
	}

	return true;
}

void CNProtectAuth::SetNextAuthQueryTime(int nTime)
{
	m_nTime = nTime;
	m_bAnswer = false;
}

bool CNProtectAuth::CheckNextQueryTime()
{
	if (m_nTime > 0)
		m_nTime--;
	if (m_nTime < 1)
		return true;
	else
		return false;
}

#endif // USING_NPROTECT
