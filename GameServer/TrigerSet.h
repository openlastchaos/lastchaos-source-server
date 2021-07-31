#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Config.h"

#ifdef TRIGER_SYSTEM	// 트리거 사용

#ifndef __TRIGER_SYSTEM__
#define __TRIGER_SYSTEM__
class CTriger
{
public:

/////////////////////// 공통 변수
	int			m_index;								// DB Index
	int			m_trigerset_type;						// 트리거 속성
	CLCString	m_trigerset_name;
	CLCString	m_triger_touchID;						// 이것을 식별자로 사용
	CLCString	m_triger_playID;
	int			m_triger_type;
	CLCString	m_triger_data;
	int			m_group_index;
	CLCString	m_group_name;

	bool	m_bUsed;									// 이미 트리거가 발동하였는지 여부
	int		m_nType_sub;								// 서브 타입이 있다면 이것을 사용


////////////////// 각 트리거별 변수

	/////// ITEM_COMPOSITION
	int		m_nItem_index_ItemComposition;				// 합성된 아이템 인덱스
	int		m_nMeterial_Count;							// 사용 아이템 개수
	int		m_nMeterial_index[MAX_TRIGER_ARRAYCOUNT];	// 사용 아이템 인덱스

	/////// ITEM_DROP
	int		m_ItemDrop_index;							// 드랍되는 아이템 인덱스
	int		m_ItemDrop_percent;							// 드랍되는 확률, 1 ~ 100%

	/////// OPEN_THE_DOOR	- 서브 타입 사용
	int		m_Killed_Npc_Index;							// 문을 열기 위해 죽여야 하는 npc 종류 
	int		m_Killed_Npc_Count;							// 문을 열기 위해 죽여야 하는 npc 숫자
	int		m_NeedItem_Index;							// 문을 열기 위해 얻어야 하는 item 종류 
	int		m_NeedItem_Count;							// 문을 열기 위해 얻어야 하는 item 숫자

	/////// START_RAID
	int		m_Start_DelayTime;							// 레이드 시작하기 전에 delay, 1 ~ 59 초

	/////// MESSAGE_PRINT

	/////// SUMMON_NPC
	int		m_nSummonNpc_indexCount;					// 소환되는 npc 종류 수
	int		m_nSummonNpc_index[MAX_TRIGER_ARRAYCOUNT];	// 소환되는 npc 인덱스
	int		m_nSummonNpc_count[MAX_TRIGER_ARRAYCOUNT];	// 소환되는 npc 숫자

	/////// USE_DEBUF
	int		m_nUseDebuf_indexCount;						// 디버프 걸리는 종류 수
	int		m_nUseDebuf_index[MAX_TRIGER_ARRAYCOUNT];	// 디버프 걸리는 인덱스

	/////// ACTIVE_OBJECT

	/////// REMOVE_NPC	- 서브 타입 사용
	int		m_Item_Index_removeNPC;						// 죽이는데 사용되는 item index
	int		m_KilledNpc_Index;							// item 당 죽는 npc 수
	int		m_KillCount;								// 죽는 npc index
	

	CTriger();
	~CTriger();
};

class CTriger_List
{
public:

	// DB
	CTriger*	m_Triger_List;
	int			m_nCount;

	bool		Load_Triger();
	CTriger*	FindProto(int index);

	CTriger_List();
	~CTriger_List();

protected:
	static int CompIndex(const void* p1, const void* p2)
	{
		CTriger* i1 = (CTriger*)p1;
		CTriger* i2 = (CTriger*)p2;

		return i1->m_index - i2->m_index;
	}

private:
	void		Swap_Triger(CTriger* triger1, CTriger* triger2);

};

#endif // __TRIGER_SYSTEM__
#endif // TRIGER_SYSTEM