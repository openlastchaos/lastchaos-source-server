#include "stdhdrs.h"

#include "Server.h"
#include "BillingClient.h"
#include "User.h"
#include "Log.h"
CUser::CUser(int index, const char* name, int server, int subnum, const char* ip, const char* ident)
	: m_name(MAX_ID_NAME_LENGTH + 1)
	, m_ip(HOST_LENGTH + 1)
	,m_proSite(3)
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

#ifdef CHARDEL_CHECKID
	if( ident )
		m_identification = ident;
#endif
}
//