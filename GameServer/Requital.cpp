
#include "stdhdrs.h"
#include "Server.h"
#include "DBCmd.h"
#include "CmdMsg.h"

#ifdef REQUITAL_EVENT	// 보상 이벤트 
#include "Requital.h"

CRequital_Event::CRequital_Event()
{
	m_index = 0;			// 
	m_event_name = "";	// 
	m_event_log_ColumnNum = 0;
	m_start_time = 0;
	m_end_time = 0;
	memset(m_item_index, 0, sizeof(int) * REQUITAL_EVENT_ITEM_MAXCOUNT);
	memset(m_item_count, 0, sizeof(int) * REQUITAL_EVENT_ITEM_MAXCOUNT);
}

CRequital_EventList::CRequital_EventList()
{
	m_Requital_EventList = NULL;		
	m_nCount = 0;
}

CRequital_EventList::~CRequital_EventList()
{
	if(m_Requital_EventList)
		delete [] m_Requital_EventList;

	m_Requital_EventList = NULL;
}

bool CRequital_EventList::Load_Requital_Event()
{
	CDBCmd dbcmd;
	dbcmd.Init(&gserver.m_dbdata);
	dbcmd.SetQuery("SELECT a_index, a_event_name, a_event_log_ColumnNum, UNIX_TIMESTAMP(a_start_time) as starttime, UNIX_TIMESTAMP(a_end_time) as endtime, a_item_idx, a_item_count FROM t_requital_setup ORDER BY a_index");

	if (!dbcmd.Open())
		return false;

	m_nCount = dbcmd.m_nrecords;
	m_Requital_EventList = new CRequital_Event[m_nCount];

	// 스킬별 설정
	int i, j;
	CLCString arg(255);

	if(!dbcmd.MoveFirst())
		return true;

	for (i=0; i < m_nCount; i++)
	{
		dbcmd.GetRec("a_index",					m_Requital_EventList[i].m_index);
		dbcmd.GetRec("a_event_name",			m_Requital_EventList[i].m_event_name);
		dbcmd.GetRec("a_event_log_ColumnNum",	m_Requital_EventList[i].m_event_log_ColumnNum);
		dbcmd.GetRec("starttime",				m_Requital_EventList[i].m_start_time);
		dbcmd.GetRec("endtime",					m_Requital_EventList[i].m_end_time);

		dbcmd.GetRec("a_item_idx", arg);
		for (j=0; j < REQUITAL_EVENT_ITEM_MAXCOUNT; j++)
		{
			arg = AnyOneArg(arg, g_buf);		
			m_Requital_EventList[i].m_item_index[j] = atoi(g_buf);
		}

		dbcmd.GetRec("a_item_count", arg);
		for (j=0; j < REQUITAL_EVENT_ITEM_MAXCOUNT; j++)
		{	
			arg = AnyOneArg(arg, g_buf);		
			m_Requital_EventList[i].m_item_count[j] = atoi(g_buf);
		}

		dbcmd.MoveNext();
	}

	return true;
}

CRequital_Event* CRequital_EventList::FindProto(int index)
{
	CRequital_Event proto;
	proto.m_index = index;

	return (CRequital_Event*)bsearch(&proto, m_Requital_EventList, m_nCount, sizeof(CRequital_Event), CompIndex);
}

#endif // REQUITAL_EVENT


