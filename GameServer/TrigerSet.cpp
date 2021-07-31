
#include "stdhdrs.h"
#include "Server.h"
#include "DBCmd.h"
#include "CmdMsg.h"

#ifdef TRIGER_SYSTEM	// 트리거 사용
#include "TrigerSet.h"

CTriger::CTriger()
{
	m_index = 0;
	m_trigerset_type = 0;		
	m_triger_type = 0;
	m_group_index = 0;

	m_bUsed = false;
	m_nType_sub = 0;

	m_nItem_index_ItemComposition = 0;
	m_nMeterial_Count = 0;

	m_ItemDrop_index = 0;
	m_ItemDrop_percent = 0;

	m_Killed_Npc_Index = 0;
	m_Killed_Npc_Count = 0;
	m_NeedItem_Index = 0;
	m_NeedItem_Count = 0;

	m_Start_DelayTime = 0;

	m_nSummonNpc_indexCount = 0;

	m_nUseDebuf_indexCount = 0;

	m_Item_Index_removeNPC = 0;
	m_KilledNpc_Index = 0;
	m_KillCount = 0;
}

CTriger::~CTriger()
{
	
}




CTriger_List::CTriger_List()
{
	m_nCount = 0;

	if(m_Triger_List)
		delete[] m_Triger_List;
	m_Triger_List = NULL;
}

CTriger_List::~CTriger_List()
{
	if(m_Triger_List)
		delete[] m_Triger_List;
	m_Triger_List = NULL;
}

CTriger* CTriger_List::FindProto(int index)
{
	CTriger proto;
	proto.m_index = index;

	return (CTriger*)bsearch(&proto, m_Triger_List, m_nCount, sizeof(CTriger), CompIndex);
}

bool CTriger_List::Load_Triger()
{

	// 1. 일단 전체 triger를 받아온다.
	CDBCmd dbcmd;
	dbcmd.Init(&gserver.m_dbdata);
	dbcmd.SetQuery("SELECT a_index, a_trigerset_type, a_trigerset_name, a_triger_touchID, a_triger_playID, a_triger_type, a_triger_data, a_group_index, a_group_name FROM t_triger_set ORDER BY a_triger_touchID");
	if (!dbcmd.Open())
	{
		GAMELOG << init("Load_Triger step 1")
				<< end;
		return false;
	}
		
	m_nCount = dbcmd.m_nrecords;
	m_Triger_List = new CTriger[m_nCount];
	int i;

	if(!dbcmd.MoveFirst())
	{
		GAMELOG << init("Load_Triger : ZERO TRIGERS")
				<< end;
		return true;
	}

	for (i=0; i < m_nCount; i++)
	{
		dbcmd.GetRec("a_index",				m_Triger_List[i].m_index);
		dbcmd.GetRec("a_trigerset_type",	m_Triger_List[i].m_trigerset_type);
		dbcmd.GetRec("a_trigerset_name",	m_Triger_List[i].m_trigerset_name);
		dbcmd.GetRec("a_triger_touchID",	m_Triger_List[i].m_triger_touchID);
		dbcmd.GetRec("a_triger_playID",		m_Triger_List[i].m_triger_playID);
		dbcmd.GetRec("a_triger_type",		m_Triger_List[i].m_triger_type);
		dbcmd.GetRec("a_triger_data",		m_Triger_List[i].m_triger_data);
		dbcmd.GetRec("a_group_index",		m_Triger_List[i].m_group_index);
		dbcmd.GetRec("a_group_name",		m_Triger_List[i].m_group_name);

		dbcmd.MoveNext();
	}

	// 2. 랜덤 그룹으로 묶여있는 triger의 종류 가짓 수를 확인한다.
	dbcmd.SetQuery("SELECT DISTINCT a_group_index FROM t_triger_set ");
	if (!dbcmd.Open())
	{
		GAMELOG << init("Load_Triger step 2")
				<< end;
		return false;
	}

	// 1을 빼는 이유는 group_index == 0 이면 그룹이 아닌 단독 triger이라서 제외시키기 때문
	int nCount_randomGroup = dbcmd.m_nrecords - 1;
	int j, number = 1;

	// 3. triger 가짓 수를 확인 후 같은 랜덤 그룹의 triger는 섞어준다.
	for (j=0; j < nCount_randomGroup; j++)
	{	
		char buf[1000];
		strcpy(buf, "SELECT a_index, a_trigerset_type, a_trigerset_name, a_triger_touchID, a_triger_playID, a_triger_type, a_triger_data, a_group_index, a_group_name FROM t_triger_set WHERE a_group_index =");
		IntCat(buf, number);
		strcat(buf, " ORDER BY a_triger_touchID");
		dbcmd.SetQuery(buf);
		
		if (!dbcmd.Open())
		{
			GAMELOG << init("Load_Triger step 3_")
					<< number
					<< end;
			return false;
		}

		int nCount_trigers = dbcmd.m_nrecords;
		CTriger* RandomGroup_List = new CTriger[nCount_trigers];

		if(!dbcmd.MoveFirst())
		{
			GAMELOG << init("Load_Triger : ZERO GROUP TRIGER ")
					<< number
					<< end;
			number++;	
			continue;
		}

		// 임시로 만든 RandomGroup_List에 동일한 그룹을 담는다.
		for (i=0; i < nCount_trigers; i++)
		{
			dbcmd.GetRec("a_index",				RandomGroup_List[i].m_index);
			dbcmd.GetRec("a_trigerset_type",	RandomGroup_List[i].m_trigerset_type);
			dbcmd.GetRec("a_trigerset_name",	RandomGroup_List[i].m_trigerset_name);
			dbcmd.GetRec("a_triger_touchID",	RandomGroup_List[i].m_triger_touchID);
			dbcmd.GetRec("a_triger_playID",		RandomGroup_List[i].m_triger_playID);
			dbcmd.GetRec("a_triger_type",		RandomGroup_List[i].m_triger_type);
			dbcmd.GetRec("a_triger_data",		RandomGroup_List[i].m_triger_data);
			dbcmd.GetRec("a_group_index",		RandomGroup_List[i].m_group_index);
			dbcmd.GetRec("a_group_name",		RandomGroup_List[i].m_group_name);

			dbcmd.MoveNext();
		}

		// 100번을 swap으로 서로 바꾸어준다.
		for (i=0; i < 100; i++)
		{
			int swapTrigerNum1 = GetRandom(0, nCount_trigers - 1);
			int swapTrigerNum2 = GetRandom(0, nCount_trigers - 1);

			if(swapTrigerNum1 == swapTrigerNum2)
				continue;
			
			// 두 triger를 섞는 함수
			Swap_Triger(&RandomGroup_List[swapTrigerNum1], &RandomGroup_List[swapTrigerNum2]);
		}

		int k=0;
		// 섞은 임시의 RandomGroup_List를 m_Triger_List의 해당 triger에 찾아서 넣어준다.
		for (i=0; i < nCount_trigers; i++)
		{
			// 전체 triger list에서
			for (k=0; k < m_nCount; k++)
			{
				// 찾았으면 데이터를 바꾸어준다.
				if ( m_Triger_List[k].m_index == RandomGroup_List[i].m_index )
				{
					m_Triger_List[k].m_triger_type = RandomGroup_List[i].m_triger_type;
					m_Triger_List[k].m_triger_data = RandomGroup_List[i].m_triger_data;
					break;
				}
			}
		}

		delete[] RandomGroup_List;
		RandomGroup_List = NULL;

		number++;		// 다음 랜덤 그룹
	} // for

	CLCString arg(255);
	// 4. triger의 m_triger_data를 type에 맞추어서 쪼개어 저장한다.
	for (i=0; i < m_nCount; i++)
	{
		arg = m_Triger_List[i].m_triger_data;

		switch(m_Triger_List[i].m_triger_type)
		{
		case ITEM_COMPOSITION:
			arg = AnyOneArg(arg, g_buf);		
			m_Triger_List[i].m_nItem_index_ItemComposition = atoi(g_buf);
			arg = AnyOneArg(arg, g_buf);		
			m_Triger_List[i].m_nMeterial_Count = atoi(g_buf);
			for( j=0; j < m_Triger_List[i].m_nMeterial_Count; j++ )
			{
				arg = AnyOneArg(arg, g_buf);		
				m_Triger_List[i].m_nMeterial_index[j] = atoi(g_buf);
			}
			break;

		case ITEM_DROP:
			arg = AnyOneArg(arg, g_buf);		
			m_Triger_List[i].m_ItemDrop_index = atoi(g_buf);
			arg = AnyOneArg(arg, g_buf);		
			m_Triger_List[i].m_ItemDrop_percent = atoi(g_buf);
			break;

		case OPEN_THE_DOOR:
			arg = AnyOneArg(arg, g_buf);		
			m_Triger_List[i].m_nType_sub = atoi(g_buf);
			if ( m_Triger_List[i].m_nType_sub == SUBTYPE_NONE_OPEN_THE_DOOR )
			{
				
			}
			else if ( m_Triger_List[i].m_nType_sub == SUBTYPE_NPC_OPEN_THE_DOOR )
			{
				arg = AnyOneArg(arg, g_buf);		
				m_Triger_List[i].m_Killed_Npc_Index = atoi(g_buf);
				arg = AnyOneArg(arg, g_buf);		
				m_Triger_List[i].m_Killed_Npc_Count = atoi(g_buf);
			}
			else if ( m_Triger_List[i].m_nType_sub == SUBTYPE_ITEM_OPEN_THE_DOOR )
			{
				arg = AnyOneArg(arg, g_buf);		
				m_Triger_List[i].m_NeedItem_Index = atoi(g_buf);
				arg = AnyOneArg(arg, g_buf);		
				m_Triger_List[i].m_NeedItem_Count = atoi(g_buf);
			}
			break;

		case START_RAID:
			arg = AnyOneArg(arg, g_buf);		
			m_Triger_List[i].m_Start_DelayTime = atoi(g_buf);
			break;

		case MESSAGE_PRINT:
			break;

		case SUMMON_NPC:
			arg = AnyOneArg(arg, g_buf);		
			m_Triger_List[i].m_nSummonNpc_indexCount = atoi(g_buf);
			for( j=0; j < m_Triger_List[i].m_nSummonNpc_indexCount; j++ )
			{
				arg = AnyOneArg(arg, g_buf);		
				m_Triger_List[i].m_nSummonNpc_index[j] = atoi(g_buf);
				arg = AnyOneArg(arg, g_buf);		
				m_Triger_List[i].m_nSummonNpc_count[j] = atoi(g_buf);
			}
			break;

		case USE_DEBUF:
			arg = AnyOneArg(arg, g_buf);		
			m_Triger_List[i].m_nUseDebuf_indexCount = atoi(g_buf);
			for( j=0; j < m_Triger_List[i].m_nSummonNpc_indexCount; j++ )
			{
				arg = AnyOneArg(arg, g_buf);		
				m_Triger_List[i].m_nUseDebuf_index[j] = atoi(g_buf);
			}
			break;

		case ACTIVE_OBJECT:
			break;	

		case REMOVE_NPC:
			arg = AnyOneArg(arg, g_buf);		
			m_Triger_List[i].m_nType_sub = atoi(g_buf);
			if ( m_Triger_List[i].m_nType_sub == SUBTYPE_NONE_REMOVE_NPC )
			{
				
			}
			else if ( m_Triger_List[i].m_nType_sub == SUBTYPE_ITEM_REMOVE_NPC )
			{
				arg = AnyOneArg(arg, g_buf);		
				m_Triger_List[i].m_Item_Index_removeNPC = atoi(g_buf);
				arg = AnyOneArg(arg, g_buf);		
				m_Triger_List[i].m_KilledNpc_Index = atoi(g_buf);
				arg = AnyOneArg(arg, g_buf);		
				m_Triger_List[i].m_KillCount = atoi(g_buf);
			}
			break;	

		default:
			GAMELOG << init("Load_Triger Error : switch() ")
					<< end;
			return false;
			break;

		}	// switch
	}	// for

	return true;
}

void CTriger_List::Swap_Triger( CTriger* triger1, CTriger* triger2 )
{
	int temp_type;
	CLCString temp_data;

	temp_type = triger1->m_triger_type;
	temp_data = triger1->m_triger_data;

	triger1->m_triger_type = triger2->m_triger_type;
	triger1->m_triger_data = triger2->m_triger_data;

	triger2->m_triger_type = temp_type;
	triger2->m_triger_data = temp_data;
}

#endif // TRIGER_SYSTEM