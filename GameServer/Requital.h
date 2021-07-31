
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Config.h"

#ifdef REQUITAL_EVENT	// 보상 이벤트 

#ifndef __REQUITAL_EVENT__
#define __REQUITAL_EVENT__
class CRequital_Event
{
public:

	// DB
	int		m_index;			// DB Index
	CLCString	m_event_name;	// 
	int		m_event_log_ColumnNum;
	int		m_start_time;
	int		m_end_time;
	int		m_item_index[REQUITAL_EVENT_ITEM_MAXCOUNT];
	int		m_item_count[REQUITAL_EVENT_ITEM_MAXCOUNT];

	CRequital_Event();
};

class CRequital_EventList  
{
public:
	CRequital_EventList();
	~CRequital_EventList();

	CRequital_Event*	m_Requital_EventList;		
	int					m_nCount;		

	bool				Load_Requital_Event();
	CRequital_Event*	FindProto(int index);

protected:
	static int CompIndex(const void* p1, const void* p2)
	{
		CRequital_Event* i1 = (CRequital_Event*)p1;
		CRequital_Event* i2 = (CRequital_Event*)p2;

		return i1->m_index - i2->m_index;
	}
};
#endif // __REQUITAL_EVENT__
#endif // REQUITAL_EVENT
