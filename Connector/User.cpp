#include "stdhdrs.h"
#include "Server.h"
#include "BillingClient.h"
#include "User.h"
#include "Log.h"

CUser::CUser(int index, const char* name, int server, int subnum, const char* ip, const char* ident)
: m_name(MAX_ID_NAME_LENGTH + 1)
, m_ip(HOST_LENGTH + 1)
#ifdef EVENT_PROMOTION_SITE
,m_proSite(3)
#endif//#ifdef EVENT_PROMOTION_SITE
#ifdef CHARDEL_CHECKID
, m_identification(8)
#endif
{
	m_index = index;
	m_name = name;
	m_server = server;
	m_subnum = subnum;
	m_zone = 0;
	m_ip = ip;
	m_state = CONN_WAIT_IDEN;
	m_listindex = -1;
	m_timestamp = 0;
	m_checkTics = 0;

	m_preopen_chk = 0;

	m_usertype = BILL_USER_FREE;
	m_timeremain = 0;
	m_paytype = BILL_PAY_FREE;
	m_location = BILL_LOCATION_HOME;
//0627
	m_userFlag = 0;

#ifdef EVENT_PCBANG
	m_pcbang_time = 0;
#endif

#ifdef CHARDEL_CHECKID
	if( ident )
		m_identification = ident;
#endif
}

CUserList::CUserList()
{
	m_last = 0;
	m_count = 0;
	m_users = new CUser*[MAX_PLAYING];
	memset(m_users, 0, sizeof(CUser*) * MAX_PLAYING);
	memset(m_playersPerZone, -1, sizeof(int) * MAX_ZONES);
#ifdef EXTERNAL_COUNT
	m_exCount = NULL;
#endif
}

CUserList::~CUserList()
{
	if (m_users)
	{
		for (int i = 0; i <= m_last; i++)
			if (m_users[i])
				delete m_users[i];
		delete [] m_users;
	}
}

CUser* CUserList::Add(CUser* user)
{
	if (m_count >= MAX_PLAYING)
		return NULL;

	int i;
	for (i = 0; i <= m_last; i++)
	{
		if (m_users[i] == NULL)
			break;
	}

	m_users[i] = user;
	user->m_listindex = i;
	m_count++;
	if (m_last < i)
		m_last = i;
#ifdef EXTERNAL_COUNT
	if( !m_exCount )
	{
		m_exCount = new int[gserver.m_extTableCount];
		memset(m_exCount, 0, sizeof(m_exCount) );
	}

	for(int j = 0; j < gserver.m_extTableCount; j++)
	{
		if( strcmp(user->m_proSite, gserver.m_extTableList[j]) == 0 )
		{
			m_exCount[j]++;
		}
	}
	/*
	if( strcmp(user->m_proSite, "NM") == 0 )
		m_nmCount++;
		*/
#endif

	GAMELOG << init("PLAYER_ADD", m_users[i]->m_name)
			<< i
			<< end;

	return m_users[i];
}

bool CUserList::Remove(CUser* user, bool bFree)
{
	if (user->m_listindex < 0 || user->m_listindex >= MAX_PLAYING || m_users[user->m_listindex] != user)
		return false;

	GAMELOG << init("PLAYER_DEL", user->m_name)
			<< user->m_listindex
			<< end;

	m_users[user->m_listindex] = NULL;
	m_count--;
#ifdef EXTERNAL_COUNT
	for(int j = 0; j < gserver.m_extTableCount; j++)
	{
		if( strcmp(user->m_proSite, gserver.m_extTableList[j]) == 0 )
		{
			m_exCount[j]--;
		}
	}
#endif
	if (bFree)
		delete user;

	return true;
}

CUser* CUserList::Find(const char* name)
{
	int i;
	for (i = 0; i <= m_last; i++)
	{
		if (m_users[i])
		{
			if (strcmp2(m_users[i]->m_name, name) == 0)
				return m_users[i];
		}
	}
	return NULL;
}

CUser* CUserList::Find(int userindex)
{
	int i;
	for (i = 0; i <= m_last; i++)
	{
		if (m_users[i])
		{
			if (m_users[i]->m_index == userindex)
				return m_users[i];
		}
	}
	return NULL;
}
