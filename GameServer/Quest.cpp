// Quest.cpp: implementation of the CQuest class.
//
//////////////////////////////////////////////////////////////////////
#include "stdhdrs.h"
#include "Server.h"
#include "DBCmd.h"
#include "CmdMsg.h"
#include "Quest.h"
#include "Log.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestProto::CQuestProto()
{
	m_index = 0;
	m_type[0] = -1;
	m_type[1] = -1;

	m_startType = -1;
	m_startData = 0;
	m_prizeNPC = -1;

	// 필요 조건
	m_needMinLevel = 1;
	m_needMaxLevel = 60;
	m_needJob = -1;
	memset(m_needItemIndex, -1, sizeof(int) * QUEST_MAX_NEED_ITEM);
	memset(m_needItemCount, 0, sizeof(int) * QUEST_MAX_NEED_ITEM);

	// 수행 조건
	memset(m_conditionType, -1, sizeof(int) * QUEST_MAX_CONDITION);
	memset(m_conditionIndex, 0, sizeof(int) * QUEST_MAX_CONDITION);
	memset(m_conditionNum, 0, sizeof(int) * QUEST_MAX_CONDITION);
	memset(m_conditionData, 0, sizeof(m_conditionData));

	// 보상 조건
	memset(m_prizeType, -1, sizeof(int) * QUEST_MAX_PRIZE);
	memset(m_prizeIndex, 0, sizeof(int) * QUEST_MAX_PRIZE);
	memset(m_prizeData, 0, sizeof(int) * QUEST_MAX_PRIZE);

		// 보상 조건
	memset(m_optPrizeType, -1, sizeof(int) * QUEST_MAX_PRIZE);
	memset(m_optPrizeIndex, 0, sizeof(int) * QUEST_MAX_PRIZE);
	memset(m_optPrizeData, 0, sizeof(int) * QUEST_MAX_PRIZE);

	m_prequestNum =0;
	m_startNpcZoneNum = 0;
	m_prizeNpcZoneNum = 0;
	m_needExp = 0;
}

//////////////////////////
// CQuestProtoList member

CQuestProtoList::CQuestProtoList()
{
	m_proto = NULL;
	m_nCount = 0;
}

CQuestProtoList::~CQuestProtoList()
{
	if (m_proto)
		delete [] m_proto;

	m_proto = NULL;
}

bool CQuestProtoList::Load()
{
	CDBCmd dbQuest;
	dbQuest.Init(&gserver.m_dbdata);
	dbQuest.SetQuery("SELECT * FROM t_quest WHERE a_enable=1 ORDER BY a_index");
	
	if (!dbQuest.Open())
		return false;
	
	m_nCount = dbQuest.m_nrecords;
	m_proto = new CQuestProto[m_nCount];

	int i, j, k;
	
	if(!dbQuest.MoveFirst())
		return true;
	
	for (i=0; i < m_nCount; i++)
	{
		dbQuest.GetRec("a_index",			m_proto[i].m_index);
		dbQuest.GetRec("a_type1",			m_proto[i].m_type[0]);
		dbQuest.GetRec("a_type2",			m_proto[i].m_type[1]);
		dbQuest.GetRec("a_start_type",		m_proto[i].m_startType);
		dbQuest.GetRec("a_start_data",		m_proto[i].m_startData);
		dbQuest.GetRec("a_prize_npc",		m_proto[i].m_prizeNPC);

		dbQuest.GetRec("a_prequest_num",		m_proto[i].m_prequestNum); 
		dbQuest.GetRec("a_start_npc_zone_num",	m_proto[i].m_startNpcZoneNum);
		dbQuest.GetRec("a_prize_npc_zone_num",	m_proto[i].m_prizeNpcZoneNum);
		dbQuest.GetRec("a_need_exp",		m_proto[i].m_needExp);

		dbQuest.GetRec("a_need_min_level",	m_proto[i].m_needMinLevel);
		dbQuest.GetRec("a_need_max_level",	m_proto[i].m_needMaxLevel);
		dbQuest.GetRec("a_need_job",		m_proto[i].m_needJob);

		for (j=0; j < QUEST_MAX_NEED_ITEM; j++)
		{
			strcpy(g_buf, "a_need_item");		IntCat(g_buf, j, false);
			dbQuest.GetRec(g_buf,			m_proto[i].m_needItemIndex[j]);
			strcpy(g_buf, "a_need_item_count");	IntCat(g_buf, j, false);
			dbQuest.GetRec(g_buf,			m_proto[i].m_needItemCount[j]);
		}
		
		for (j=0; j < QUEST_MAX_CONDITION; j++)
		{
			strcpy(g_buf, "a_condition");	IntCat(g_buf, j, false);	strcat(g_buf, "_type");
			dbQuest.GetRec(g_buf,			m_proto[i].m_conditionType[j]);

			strcpy(g_buf, "a_condition");	IntCat(g_buf, j, false);	strcat(g_buf, "_index");
			dbQuest.GetRec(g_buf,			m_proto[i].m_conditionIndex[j]);
			
			strcpy(g_buf, "a_condition");	IntCat(g_buf, j, false);	strcat(g_buf, "_num");
			dbQuest.GetRec(g_buf,			m_proto[i].m_conditionNum[j]);
			
			for (k=0; k < QUEST_MAX_CONDITION_DATA; k++)
			{
				strcpy(g_buf, "a_condition");	IntCat(g_buf, j, false);
				strcat(g_buf, "_data");			IntCat(g_buf, k, false);
				dbQuest.GetRec(g_buf,			m_proto[i].m_conditionData[j][k]);
			}
		}
		
		for (j=0; j < QUEST_MAX_PRIZE; j++)
		{
			strcpy(g_buf, "a_prize_type");	IntCat(g_buf, j, false);
			dbQuest.GetRec(g_buf,			m_proto[i].m_prizeType[j]);
			strcpy(g_buf, "a_prize_index");	IntCat(g_buf, j, false);
			dbQuest.GetRec(g_buf,			m_proto[i].m_prizeIndex[j]);
			strcpy(g_buf, "a_prize_data");	IntCat(g_buf, j, false);
			dbQuest.GetRec(g_buf,			m_proto[i].m_prizeData[j]);			
		}

		for (j=0; j < QUEST_MAX_PRIZE; j++)
		{
			strcpy(g_buf, "a_opt_prize_type");	IntCat(g_buf, j, false);
			dbQuest.GetRec(g_buf,			m_proto[i].m_optPrizeType[j]);
			strcpy(g_buf, "a_opt_prize_index");	IntCat(g_buf, j, false);
			dbQuest.GetRec(g_buf,			m_proto[i].m_optPrizeIndex[j]);
			strcpy(g_buf, "a_opt_prize_data");	IntCat(g_buf, j, false);
			dbQuest.GetRec(g_buf,			m_proto[i].m_optPrizeData[j]);			
		}

		
		dbQuest.MoveNext();
	}
	
	return true;
}

CQuestProto* CQuestProtoList::FindProto(int index)
{
	CQuestProto proto;
	proto.m_index = index;
	
	return (CQuestProto*)bsearch(&proto, m_proto, m_nCount, sizeof(CQuestProto), CompIndex);
}

#ifdef QUEST_DATA_EXTEND
#else
CQuest* CQuestProtoList::Create(int index)
{
	CQuestProto* proto = FindProto(index);

	if (!proto)
		return NULL;

	return new CQuest(proto);
}
#endif // QUEST_DATA_EXTEND





#ifdef QUEST_DATA_EXTEND

///////////////
// class CQuest
CQuest::CQuest(const CQuestProto* pQuestProto)
{
	m_pQuestProto = pQuestProto;
	m_cQuestState = QUEST_STATE_INIT;
	memset(m_nQuestValue, 0, sizeof(m_nQuestValue));
	m_bComplete1 = false;
	m_bComplete2 = false;

	m_pPrev = NULL;
	m_pNext = NULL;
}

CQuest::~CQuest()
{
	m_pQuestProto = NULL;
	m_cQuestState = QUEST_STATE_INIT;
	m_bComplete1 = false;
	m_bComplete2 = false;

	m_pPrev = NULL;
	m_pNext = NULL;
}

void CQuest::QuestUpdateData(CPC* pPC, CNPC* pNPC)
{
	int i, j;
	
	// 혼자 npc를 다잡았어야 진행!! 구출 퀘스트는 제외 
	// TODO : npc를 혼자 다 안잡아도 막타만 치면 되도록 한다!!!
	//if (npc->m_attackList->m_damage < npc->m_totalDamage && this->m_proto->m_type[0] != QTYPE_KIND_SAVE)
	//	return;
	
	// 이미 완료했으면 return
	if (IsCompleted())
		return ;

	// CONDITION에 따라 검사 처음에 모두 false로 셋팅
	// 조건 검사 하면서 만족하는 조건만 true로 셋팅
	bool bComplete[QUEST_MAX_CONDITION];
	memset(bComplete, 0, sizeof(bComplete));
	
	switch (GetQuestType0())
	{
		// 수집형 퀘스트
	case QTYPE_KIND_COLLECTION:
	case QTYPE_KIND_MINING_EXPERIENCE:
	case QTYPE_KIND_GATHERING_EXPERIENCE:
	case QTYPE_KIND_CHARGE_EXPERIENCE:
		{			
			for (i = 0; i < QUEST_MAX_CONDITION; i++)
			{				
				if (m_pQuestProto->m_conditionType[i] == -1)
				{
					bComplete[i] = true;
					continue;
				}

				switch (m_pQuestProto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						bool bDropNpc = false;
						
						// condition 부가 데이타 중에 때린 pNPC가 있으면 해당 아이템 인벤으로
						for (j = 0; j < QUEST_MAX_ITEM_DROP_MONSTER; j++)
						{
							if (m_pQuestProto->m_conditionData[i][j] == pNPC->m_proto->m_index)
							{
								bDropNpc = true;
								break;
							}
						}
						
						// 수행조건에 해당하는 pNPC를 때린것이 아님 리턴!
						if (!bDropNpc)
							return;
						
						// 조건보다 이상이면 더이상 아이템 안줌
						if (GetQuestValue(i) >= m_pQuestProto->m_conditionNum[i])
						{
							bComplete[i] = true;
							continue;
						}
						
						CItem* item = NULL;
						if (GetRandom(1, 10000) <= m_pQuestProto->m_conditionData[i][QUEST_MAX_CONDITION_DATA-1])
							item = gserver.m_itemProtoList.CreateItem(m_pQuestProto->m_conditionIndex[i], -1, 0, 0, 1);
						
						if (!item)
							continue;
						
						CNetMsg itemmsg;
						
						// 들어갈 인벤토리 결정
						CInventory* inven = GET_INVENTORY(pPC, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
						if (!inven)
							continue ;
						
						bool bCountable = false;
						// 인벤에 넣기
						if (AddToInventory(pPC, item, true, true))
						{
							// 겹쳐졌는지 검사
							if (item->tab() == -1)
							{
								bCountable = true;
								
								// 수량 변경 알림
								int r, c;
								if (inven->FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
								{
									CItem* p = inven->GetItem(r, c);
									if (p)
									{
										ItemUpdateMsg(itemmsg, p, item->Count());
									}
								}
							}
							else
							{
								ItemAddMsg(itemmsg, item);
								// 돈 검사
								if (item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index 
									&& pPC->m_moneyItem == NULL)
								{
									pPC->m_moneyItem = item;
								}
							}
						}
						else
						{
							delete item;
							item = NULL;

							CNetMsg smsg;
							SysFullInventoryMsg(smsg, (char)inven->m_tab);
							SEND_Q(smsg, pPC->m_desc);
							return ;
						}
											
						// 메시지 보내고			
						SEND_Q(itemmsg, pPC->m_desc);
							
						// 이미 완료했으면 return
						if (IsCompleted())
							break;

						IncreaseQuestValue(i);
						bComplete[i] = false;											
						
						CNetMsg statusMsg;
						QuestStatusMsg(statusMsg, this);
						SEND_Q(statusMsg, pPC->m_desc);
						
						// LOG
						GAMELOG << init("ITEM_PICK_QUESTITEM", pPC)
							<< itemlog(item)
							<< end;
						
						if (bCountable)
							delete item;
						
						if (GetQuestValue(i) == m_pQuestProto->m_conditionNum[i])
						{
							bComplete[i] = true;
							continue;
						}
						else if (GetQuestValue(i) < m_pQuestProto->m_conditionNum[i])
							bComplete[i] = false;
						else
							continue;				

					}
					break;
				}
			}
		}
		break;
		
		// 전달 퀘스트
	case QTYPE_KIND_DELIVERY:
		break;
		
		// 반복 퀘스트
	case QTYPE_KIND_REPEAT:
		{			
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (m_pQuestProto->m_conditionType[i] == -1)
				{
					bComplete[i] = true;
					continue;
				}

				switch (m_pQuestProto->m_conditionType[i])
				{
				case QCONDITION_NPC:
					{
						if (m_pQuestProto->m_conditionIndex[i] == pNPC->m_proto->m_index)
						{
							if (GetQuestValue(i) < m_pQuestProto->m_conditionNum[i])
							{
								bComplete[i] = false;
								IncreaseQuestValue(i);
								
								CNetMsg statusMsg;
								QuestStatusMsg(statusMsg, this);
								SEND_Q(statusMsg, pPC->m_desc);
							}
							
							// 같으면 bComplete true 셋팅
							if (GetQuestValue(i) == m_pQuestProto->m_conditionNum[i])
							{
								bComplete[i] = true;
								continue;
							}
							else if (GetQuestValue(i) < m_pQuestProto->m_conditionNum[i])
								bComplete[i] = false;
							else
								continue;
						}
						else if(GetQuestValue(i) == m_pQuestProto->m_conditionNum[i])
						{
							bComplete[i] = true;
							continue;
						}
					}
					break;
				}
			}
		}
		break;
		
		// 싱글던전2 구출 퀘스트
	case QTYPE_KIND_SAVE:
		{
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (m_pQuestProto->m_conditionType[i] == -1)
					continue;

				switch (m_pQuestProto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						// 공격한 몹이 죽었고 아이템 드롭 몬스터(보스)이면 아이템 지급
						if (DEAD(pNPC) && pNPC->m_proto->m_index == m_pQuestProto->m_conditionData[i][0])
						{
							CItem * item = gserver.m_itemProtoList.CreateItem(m_pQuestProto->m_conditionIndex[i], -1, 0, 0, 1);
							
							if (!item)
								continue;
							
							CNetMsg itemmsg;
							
							// 들어갈 인벤토리 결정
							CInventory* inven = GET_INVENTORY(pPC, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
							if (!inven)
								continue ;
							
							bool bCountable = false;
							// 인벤에 넣기
							if (AddToInventory(pPC, item, true, true))
							{
								// 겹쳐졌는지 검사
								if (item->tab() == -1)
								{
									bCountable = true;
									
									// 수량 변경 알림
									int r, c;
									if (inven->FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
									{
										CItem* p = inven->GetItem(r, c);
										if (p)
											ItemUpdateMsg(itemmsg, p, item->Count());
									}
								}
								else
								{
									ItemAddMsg(itemmsg, item);
									// 돈 검사
									if (item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && pPC->m_moneyItem == NULL)
										pPC->m_moneyItem = item;
								}
							}
							else
							{
								// 인젠토리 꽉차서 못 받을 때 Drop
								item = pNPC->m_pArea->DropItem(item, pNPC);
								if (!item)
									return;

								CNetMsg dropMsg;
								item->m_preferenceIndex = pPC->m_index;
								ItemDropMsg(dropMsg, pNPC, item);
								pNPC->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
							}
							
							// 메시지 보내고
							SEND_Q(itemmsg, pPC->m_desc);

							IncreaseQuestValue(i);

							// LOG
							GAMELOG << init("ITEM_PICK_QUESTITEM", pPC)
									<< itemlog(item)
									<< end;
							
							if (bCountable)
								delete item;

							// 구출 퀘스트의 경우 그냥 리턴
							//return; //왜 그냥 리턴을 하지???
						}
					}
					break;				
				}
			}
		}
		break;

		// 싱글던전1 격파 퀘스트
	case QTYPE_KIND_DEFEAT:
		{
			/////////////////////////////////////////
			//싱글 던젼 4 처리 : BW
			//
			if(this->m_pQuestProto->m_index == 105)
			{
				
				if(( pNPC->m_proto->m_index == 210
					|| pNPC->m_proto->m_index == 211
					|| pNPC->m_proto->m_index == 212
					|| pNPC->m_proto->m_index == 213
					|| pNPC->m_proto->m_index == 214 ) && DEAD(pNPC) )
				{
					IncreaseQuestValue(0);
					// 랭킹 디비 마리수 업데이트
				}

				if(m_pQuestProto->m_conditionNum[0] <= GetQuestValue(0))
				{
					for(i = 0; i < QUEST_MAX_CONDITION; i++) 
						bComplete[i] = true;

					// 랭킹 디비 클리어/ 시간 업데이트
				}

				break;
			}


			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (m_pQuestProto->m_conditionType[i] == -1)
				{
					bComplete[i] = true;
					continue;
				}

				switch (m_pQuestProto->m_conditionType[i])
				{
				case QCONDITION_NPC:
					{
						// 공격하는 몹이 싱글던전 완료조건 몹의 인덱스와 같고 죽었으면
						if (m_pQuestProto->m_conditionIndex[i] == pNPC->m_proto->m_index && DEAD(pNPC))
						{
							bComplete[i] = true;
							IncreaseQuestValue(i);
						}
					}
					break;				
				}
			}
		}
		break;
		
	default:
		return;
		break;
	}

	// bComplete가 모두 true로 셋팅 되있어야 ㅇㅋ
	for (i=0; i < QUEST_MAX_CONDITION; i++)
	{
		if (!bComplete[i])
			return;
	}


	///////////////////////////////////
	// pd4 퀘스트이면 helper end : BW
	// MSG_HELPER_PD4_RANK_END,			// pd4 종료			 : charIndex(n) deadmonNum(n) bclear(c)
	// char index를 보냄
	if(this->m_pQuestProto->m_index == 105)
	{
		CNetMsg pd4RankEndMsg;

		pd4RankEndMsg.Init(MSG_HELPER_COMMAND);
		pd4RankEndMsg << MSG_HELPER_PD4_RANK_END
					  << pPC->m_index
					  << GetQuestValue(0)
					  << (char) 1;

		GAMELOG << init("PD4 END", pPC)
				<< GetQuestValue(0) << end;


		if (IS_RUNNING_HELPER)
		{
			SEND_Q(pd4RankEndMsg, gserver.m_helper);
		}
		else
		{
			GAMELOG << init("PD4 QUEST ERROR : CANNOT FIND HELPER")
					<< pPC->m_index << end;

			return;
		}
	}

	
	// Complete Msg 최초전송인경우에만 전송
	if (!IsCompleted())
	{
		if(GetComplete2())
		{
			// Quest Complete Log
			GAMELOG << init("QUEST COMPLETE (QuestUpdateData)", pPC)
					<< m_pQuestProto->m_index
					<< end;
		
			// 완료 메시지 
			CNetMsg completeMsg;
			QuestCompleteMsg(completeMsg, this);
			SEND_Q(completeMsg, pPC->m_desc);
		}
		SetComplete1(true);
	}
}

void CQuest::RemoeQuestItem(CPC* pc)
{
	int k;
	for (k = 0; k < QUEST_MAX_CONDITION; k++)
	{
		// 수행조건 아이템을 찾고
		if (m_pQuestProto->m_conditionType[k] == QCONDITION_ITEM)
		{
			// 전달 퀘스트 아이템은 퀘스트 인벤에서만 !!!
			int r, c;
			if (!pc->m_invenQuest.FindItem(&r, &c, m_pQuestProto->m_conditionIndex[k], 0, 0))
				continue ;
			
			// 찾았으면 수량 확인 해서 줄이기
			CItem* item = pc->m_invenQuest.GetItem(r, c);
			
			if (!item)
				continue ;
			
			// 필요 숫자 만큼 없음
			if (item->Count() < m_pQuestProto->m_conditionNum[k])
				continue ;

			CNetMsg itemMsg;
			
			// Item 수량 변경
			DecreaseFromInventory(pc, item, m_pQuestProto->m_conditionNum[k]);
			
			if (item->Count() > 0)
			{
				ItemUpdateMsg(itemMsg, item, -m_pQuestProto->m_conditionNum[k]);
				SEND_Q(itemMsg, pc->m_desc);
			}
			else
			{
				ItemDeleteMsg(itemMsg, item);
				SEND_Q(itemMsg, pc->m_desc);
				pc->RemoveItemFromQuickSlot(item);
				RemoveFromInventory(pc, item, true, true);
			}
		}
	}
}

///////////////////
// class CQuestList
CQuestList::CQuestList()
{
	m_head = NULL;
	m_nCountRun = 0;
	m_nCountDone = 0;
	m_nCountAbandon = 0;
	m_nCountComplete = 0;
}

CQuestList::~CQuestList()
{
	while (m_head)
	{
		CQuest*		pQuest = m_head;
		m_head = m_head->GetNextQuest();
		delete pQuest;
	}
	m_nCountRun = 0;
	m_nCountDone = 0;
	m_nCountAbandon = 0;
	m_nCountComplete = 0;
}

CQuest* CQuestList::AddQuest(int nQuestIndex)
{
	const CQuestProto* pQuestProto = gserver.m_questProtoList.FindProto(nQuestIndex);
	if (pQuestProto == NULL)
		return NULL;
	CQuest* pQuest = new CQuest(pQuestProto);
	pQuest->SetNextQuest(m_head);
	if (m_head)
		m_head->SetPrevQuest(pQuest);
	m_head = pQuest;
	return pQuest;
}

bool CQuestList::DelQuest(CPC* pPC, CQuest* pQuest)
{
	CQuest* pDelQuest;
	CQuest* pDelQuestNext = GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pDelQuest = pDelQuestNext))
	{
		pDelQuestNext = GetNextQuest(pDelQuestNext, QUEST_STATE_RUN);
		if (pDelQuest == pQuest)
		{
			CQuest* pPrevQuest = pDelQuest->GetPrevQuest();
			CQuest* pNextQuest = pDelQuest->GetNextQuest();
			if (pPrevQuest)
				pPrevQuest->SetNextQuest(pNextQuest);
			if (pNextQuest)
				pNextQuest->SetPrevQuest(pPrevQuest);
			if (pDelQuest == m_head)
				m_head = m_head->GetNextQuest();
			m_nCountRun--;
			return true;
		}
	}
	return false;
}

CQuest* CQuestList::FindQuest(int nQuestIndex)
{
	CQuest* pFindQuest;
	CQuest* pFindQuestNext = GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pFindQuest = pFindQuestNext))
	{
		pFindQuestNext = GetNextQuest(pFindQuestNext, QUEST_STATE_RUN);
		if (pFindQuest->GetQuestIndex() == nQuestIndex)
			return pFindQuest;
	}
	return NULL;
}

//  [2/23/2007 KwonYongDae]
// 퀘스트 타입 0 이 진행중 이면 true 
bool CQuestList::IsQuestType0( int qType0 )
{
	CQuest* pFindQuest;
	CQuest* pFindQuestNext = GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pFindQuest = pFindQuestNext))
	{
		pFindQuestNext = GetNextQuest(pFindQuestNext, QUEST_STATE_RUN);
		if ( pFindQuest->GetQuestType0() == qType0 && !pFindQuest->IsCompleted() )
			return true;
	}
	return false;
}

void CQuestList::SetQuestState(CQuest* pQuest, char cQuestState)
{
	CQuest* pFindQuest;
	CQuest* pFindQuestNext = GetNextQuest(NULL);
	while ((pFindQuest = pFindQuestNext))
	{
		pFindQuestNext = GetNextQuest(pFindQuestNext);
		if (pFindQuest == pQuest)
		{
			switch (pFindQuest->GetQuestState())
			{
			case QUEST_STATE_RUN:			m_nCountRun--;			break;
			case QUEST_STATE_DONE:			m_nCountDone--;			break;
			case QUEST_STATE_ABANDON:		m_nCountAbandon--;		break;
			default:												break;
			}
			pFindQuest->SetQuestState(cQuestState);
			switch (pFindQuest->GetQuestState())
			{
			case QUEST_STATE_RUN:			m_nCountRun++;			break;
			case QUEST_STATE_DONE:			m_nCountDone++;			break;
			case QUEST_STATE_ABANDON:		m_nCountAbandon++;		break;
			default:												break;
			}
			return ;
		}
	}
}

CQuest* CQuestList::GetNextQuest(CQuest* pCurrentQuest, char cQuestState)
{
	if (pCurrentQuest == NULL)
		pCurrentQuest = m_head;
	else
		pCurrentQuest = pCurrentQuest->GetNextQuest();
	while (pCurrentQuest)
	{
		if (cQuestState == QUEST_STATE_ALL || pCurrentQuest->GetQuestState() == cQuestState)
			return pCurrentQuest;
		pCurrentQuest = pCurrentQuest->GetNextQuest();
	}
	return NULL;
}

void CQuestList::CheckComplete(CPC* pc)
{
	int j;
	CQuest* pQuestNext;
	CQuest* pQuest;
	const CQuestProto* pQuestProto;
	pQuestNext = GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		pQuestProto = pQuest->GetQuestProto();

		switch (pQuest->GetQuestType0())
		{
		case QTYPE_KIND_REPEAT:
		case QTYPE_KIND_COLLECTION:
		case QTYPE_KIND_MINING_EXPERIENCE:
		case QTYPE_KIND_GATHERING_EXPERIENCE:
		case QTYPE_KIND_CHARGE_EXPERIENCE:
#ifdef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae]
		case QTYPE_KIND_PROCESS_EXPERIENCE :		//  [2/22/2007 KwonYongDae] QuestCheck
		case QTYPE_KIND_MAKE_EXPERIENCE :
#endif // EXPRIENCE_QUEST
			{
				bool complete = true;
				bool complete2 = true;

				for (j=0; j < QUEST_MAX_CONDITION; j++)
				{
					if (pQuestProto->m_conditionType[j] != QCONDITION_ITEM_NORMAL)
						continue;
					
					if (pQuest->GetQuestValue(j) >= pQuestProto->m_conditionNum[j])
						continue;

					complete2 = false;
					break;
				}

				for (j=0; j < QUEST_MAX_CONDITION; j++)
				{
					//if (pQuestProto->m_conditionType[j] != QCONDITION_ITEM)
					if (pQuestProto->m_conditionType[j] == -1 || pQuestProto->m_conditionType[j] == QCONDITION_ITEM_NORMAL)
						continue;
					
					if (pQuest->GetQuestValue(j) >= pQuestProto->m_conditionNum[j])
						continue;

					complete = false;
					break;
				}
// 원래 코드 : 050503 : BS 수정
//					memset(bComplete, false, sizeof(bool) * QUEST_MAX_CONDITION);
//
//					for (j=0; j < QUEST_MAX_CONDITION; j++)
//					{
//						if (pQuestProto->m_conditionType[j] == -1)
//						{
//							bComplete[j] = true;
//							continue;
//						}
//						
//						if (m_list[i]->m_currentData[j] >= pQuestProto->m_conditionNum[j])
//							bComplete[j] = true;
//					}
//					
//					for (j=0; j < QUEST_MAX_CONDITION; j++)
//					{
//						if (!bComplete[j])
//							complete = false;
//					}
// --- 원래 코드 : 050503 : BS 수정

				if(complete2)
				{
					pQuest->SetComplete2(true);
				}
				if (complete)
				{
					if(complete2) 
					{
						CNetMsg msg;
						QuestCompleteMsg(msg, pQuest);
						SEND_Q(msg, pc->m_desc);
					}

					pQuest->SetComplete1(true);
				}
			}
			break;

		case QTYPE_KIND_DELIVERY:
			{
				bool bcomplete,bcomplete2;
				bcomplete = bcomplete2 = TRUE;

				for (j = 0; j < QUEST_MAX_CONDITION; j++)
				{						
					if (pQuestProto->m_conditionType[j] == QCONDITION_ITEM_NORMAL ) // [KwonYongDae] 보통아이템 일때만 검사
					{				
						int r, c;
						if (!pc->m_invenNormal.FindItem(&r, &c, pQuestProto->m_conditionIndex[j], 0, 0) )
						{
							//DelQuest(pc, m_list[i]);
							//return;
							bcomplete2 = FALSE;
							break;
						}

						CItem* item = pc->m_invenNormal.GetItem(r, c);
						
						if (!item)
						{
							//DelQuest(pc, m_list[i]);
							//return;
							bcomplete2 = FALSE;
							break;
						}
						
						// 필요 숫자 만큼 없음
						if (item->Count() < pQuestProto->m_conditionNum[j])
						{
							//DelQuest(pc, m_list[i]);
							//return;
							bcomplete2 = FALSE;
							break;
						}					
					}
#ifdef EXPRIENCE_QUEST	//  [3/1/2007 KwonYongDae]
					else if ( pQuestProto->m_conditionType[j] == QCONDITION_ITEM )	//퀘스트 아이템
					{
						int r, c;
						if ( !pc->m_invenQuest.FindItem(&r, &c, pQuestProto->m_conditionIndex[j], 0, 0) )
						{
							bcomplete2 = FALSE;
							break;
						}

						CItem* item = pc->m_invenQuest.GetItem(r, c);
						
						if (!item)
						{
							bcomplete2 = FALSE;
							break;
						}
						
						if (item->Count() < pQuestProto->m_conditionNum[j])
						{						
							bcomplete2 = FALSE;
							break;
						}					
					}
#endif // EXPRIENCE_QUEST
				}

				if(bcomplete2)
				{
					pQuest->SetComplete2(true);
				}

				for (j = 0; j < QUEST_MAX_CONDITION; j++)
				{
					// 전달 퀘스트는 아이템이 
					if (pQuestProto->m_conditionType[j] == QCONDITION_ITEM ) // [KwonYongDae] npc일때만 검사
					{
						// 전달 퀘스트 아이템은 퀘스트 인벤에서만 !!!
						int r, c;
						if (!pc->m_invenQuest.FindItem(&r, &c, pQuestProto->m_conditionIndex[j], 0, 0) )
						{
							//DelQuest(pc, m_list[i]);
							//return;
							bcomplete = FALSE;
							break;
						}

						// 찾았으면 수량 확인 해서 줄이기
						CItem* item = pc->m_invenQuest.GetItem(r, c);
						
						if (!item)
						{
							//DelQuest(pc, m_list[i]);
							//return;
							bcomplete = FALSE;
							break;
						}
						
						// 필요 숫자 만큼 없음
						if (item->Count() < pQuestProto->m_conditionNum[j])
						{
							//DelQuest(pc, m_list[i]);
							//return;
							bcomplete = FALSE;
							break;
						}						
					}			
					
				}

				if(bcomplete)
				{
					pQuest->SetComplete1(true);
				}

				if(bcomplete && bcomplete2)
				{
					CNetMsg msg;
					QuestCompleteMsg(msg, pQuest);
					SEND_Q(msg, pc->m_desc);
				}

				//CNetMsg msg;
				//QuestCompleteMsg(msg, pQuest);
				//SEND_Q(msg, pc->m_desc);

				//m_bComplete[i] = true;
			}
			break;

		case QTYPE_KIND_DEFEAT :
		case QTYPE_KIND_SAVE :
			{
				bool complete = true;
				for (j=0; j < QUEST_MAX_CONDITION; j++)
				{
					if (pQuestProto->m_conditionType[j] != QCONDITION_ITEM && pQuestProto->m_conditionType[j] != QCONDITION_NPC)
						continue;
					
					if (pQuest->GetQuestValue(j) >= pQuestProto->m_conditionNum[j])
						continue;
					
					complete = false;
					break;
				}

				if(complete)
				{
					pQuest->SetComplete1(true);			//싱글던전 퀘스트이므로..
					pQuest->SetComplete2(true);

					CNetMsg msg;
					QuestCompleteMsg(msg, pQuest);
					SEND_Q(msg, pc->m_desc);
				}

			}
			break;

		default:
			break;
		}
	}
}

#else // QUEST_DATA_EXTEND

//////////////////////////
// CQuest member
CQuest::CQuest(CQuestProto* proto)
{
	m_proto = proto;
	memset(m_currentData, 0, sizeof(int) * QUEST_MAX_CONDITION);
}

void CQuest::QuestUpdateData(CPC* pc, CNPC* npc)
{
	int i, j;
	
	// 혼자 npc를 다잡았어야 진행!! 구출 퀘스트는 제외 
	// TODO : npc를 혼자 다 안잡아도 막타만 치면 되도록 한다!!!
	//if (npc->m_attackList->m_damage < npc->m_totalDamage && this->m_proto->m_type[0] != QTYPE_KIND_SAVE)
	//	return;
	
	int idx = pc->m_questList.FindQuest(m_proto->m_index);
	
	// 없거나 이미 완료했으면 return
	if (idx < 0 || pc->m_questList.IsCompleted(idx))
		return;

	// CONDITION에 따라 검사 처음에 모두 false로 셋팅
	// 조건 검사 하면서 만족하는 조건만 true로 셋팅
	bool bComplete[QUEST_MAX_CONDITION];
	memset(bComplete, false, sizeof(bool) * QUEST_MAX_CONDITION);
	
	switch (m_proto->m_type[0])
	{
		// 수집형 퀘스트
	case QTYPE_KIND_COLLECTION:
	case QTYPE_KIND_MINING_EXPERIENCE:
	case QTYPE_KIND_GATHERING_EXPERIENCE:
	case QTYPE_KIND_CHARGE_EXPERIENCE:
#ifdef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae]
	case QTYPE_KIND_PROCESS_EXPERIENCE :		//  [2/22/2007 KwonYongDae]
	case QTYPE_KIND_MAKE_EXPERIENCE :			//  [2/23/2007 KwonYongDae]
#endif // EXPRIENCE_QUEST	
		{			
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{				
				if (m_proto->m_conditionType[i] == -1)
				{
					bComplete[i] =true;
					continue;
				}

				switch (m_proto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						bool bDropNpc = false;
						
						// condition 부가 데이타 중에 때린 npc가 있으면 해당 아이템 인벤으로
						for (j=0; j < QUEST_MAX_ITEM_DROP_MONSTER; j++)
						{
							if (m_proto->m_conditionData[i][j] == npc->m_proto->m_index)
							{
								bDropNpc = true;
								break;
							}
						}
						
						// 수행조건에 해당하는 npc를 때린것이 아님 리턴!
						if (!bDropNpc)
							return;
						
						// 조건보다 이상이면 더이상 아이템 안줌
						if (m_currentData[i] >= m_proto->m_conditionNum[i])
						{
							bComplete[i] = true;
							continue;
						}
						
						CItem* item = NULL;
						if (GetRandom(1, 10000) <= m_proto->m_conditionData[i][QUEST_MAX_CONDITION_DATA-1])
							item = gserver.m_itemProtoList.CreateItem(m_proto->m_conditionIndex[i], -1, 0, 0, 1);
						
						if (!item)
							continue;
						
						CNetMsg itemmsg;
						
						// 들어갈 인벤토리 결정
						CInventory* inven = GET_INVENTORY(pc, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
						if (!inven)
							continue ;
						
						bool bCountable = false;
						// 인벤에 넣기
						if (AddToInventory(pc, item, true, true))
						{
							// 겹쳐졌는지 검사
							if (item->tab() == -1)
							{
								bCountable = true;
								
								// 수량 변경 알림
								int r, c;
								if (inven->FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
								{
									CItem* p = inven->GetItem(r, c);
									if (p)
										ItemUpdateMsg(itemmsg, p, item->Count());
								}
							}
							else
							{
								ItemAddMsg(itemmsg, item);
								// 돈 검사
								if (item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && pc->m_moneyItem == NULL)
									pc->m_moneyItem = item;
							}
						}
						else
						{
							delete item;
							item = NULL;

							CNetMsg smsg;
							SysFullInventoryMsg(smsg, (char)inven->m_tab);
							SEND_Q(smsg, pc->m_desc);
							return ;
						}

						m_currentData[i]++;
						bComplete[i] = false;
						
						// 메시지 보내고
						SEND_Q(itemmsg, pc->m_desc);
						
						CNetMsg statusMsg;
						QuestStatusMsg(statusMsg, pc->m_questList.m_list[idx]);
						SEND_Q(statusMsg, pc->m_desc);
						
						// LOG
						GAMELOG << init("ITEM_PICK_QUESTITEM", pc)
							<< itemlog(item)
							<< end;
						
						if (bCountable)
							delete item;
						
						if (m_currentData[i] == m_proto->m_conditionNum[i])
						{
							bComplete[i] = true;
							continue;
						}
						else if (m_currentData[i] < m_proto->m_conditionNum[i])
							bComplete[i] = false;
						else
							continue;
					}
					break;
				}
			}
		}
		break;
		
		// 전달 퀘스트
	case QTYPE_KIND_DELIVERY:
		break;
		
		// 반복 퀘스트
	case QTYPE_KIND_REPEAT:
		{			
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (m_proto->m_conditionType[i] == -1)
				{
					bComplete[i] = true;
					continue;
				}

				switch (m_proto->m_conditionType[i])
				{
				case QCONDITION_NPC:
					{
						if (m_proto->m_conditionIndex[i] == npc->m_proto->m_index)
						{
							if (m_currentData[i] < m_proto->m_conditionNum[i])
							{
								bComplete[i] = false;
								m_currentData[i]++;
								
								CNetMsg statusMsg;
								QuestStatusMsg(statusMsg, pc->m_questList.m_list[idx]);
								SEND_Q(statusMsg, pc->m_desc);
							}
							
							// 같으면 bComplete true 셋팅
							if (m_currentData[i] == m_proto->m_conditionNum[i])
							{
								bComplete[i] = true;
								continue;
							}
							else if (m_currentData[i] < m_proto->m_conditionNum[i])
								bComplete[i] = false;
							else
								continue;
						}
						else if(m_currentData[i] == m_proto->m_conditionNum[i])
						{
							bComplete[i] = true;
							continue;
						}
					}
					break;
				}
			}
		}
		break;
		
		// 싱글던전2 구출 퀘스트
	case QTYPE_KIND_SAVE:
		{
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (m_proto->m_conditionType[i] == -1)
					continue;

				switch (m_proto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						// 공격한 몹이 죽었고 아이템 드롭 몬스터(보스)이면 아이템 지급
						if (DEAD(npc) && npc->m_proto->m_index == m_proto->m_conditionData[i][0])
						{
							CItem* item = item = gserver.m_itemProtoList.CreateItem(m_proto->m_conditionIndex[i], -1, 0, 0, 1);
							
							if (!item)
								continue;
							
							CNetMsg itemmsg;
							
							// 들어갈 인벤토리 결정
							CInventory* inven = GET_INVENTORY(pc, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
							if (!inven)
								continue ;
							
							bool bCountable = false;
							// 인벤에 넣기
							if (AddToInventory(pc, item, true, true))
							{
								// 겹쳐졌는지 검사
								if (item->tab() == -1)
								{
									bCountable = true;
									
									// 수량 변경 알림
									int r, c;
									if (inven->FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
									{
										CItem* p = inven->GetItem(r, c);
										if (p)
											ItemUpdateMsg(itemmsg, p, item->Count());
									}
								}
								else
								{
									ItemAddMsg(itemmsg, item);
									// 돈 검사
									if (item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && pc->m_moneyItem == NULL)
										pc->m_moneyItem = item;
								}
							}
							else
							{
								// 인젠토리 꽉차서 못 받을 때 Drop
								item = npc->m_pArea->DropItem(item, npc);
								if (!item)
									return;

								CNetMsg dropMsg;
								item->m_preferenceIndex = pc->m_index;
								ItemDropMsg(dropMsg, npc, item);
								npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
							}
							
							// 메시지 보내고
							SEND_Q(itemmsg, pc->m_desc);

							m_currentData[i]++;

							// LOG
							GAMELOG << init("ITEM_PICK_QUESTITEM", pc)
									<< itemlog(item)
									<< end;
							
							if (bCountable)
								delete item;

							// 구출 퀘스트의 경우 그냥 리턴
							//return; //왜 그냥 리턴을 하지???
						}
					}
					break;				
				}
			}
		}
		break;

		// 싱글던전1 격파 퀘스트
	case QTYPE_KIND_DEFEAT:
		{
			/////////////////////////////////////////
			//싱글 던젼 4 처리 : BW
			//
			if(this->m_proto->m_index == 105)
			{
				
				if(( npc->m_proto->m_index == 210
					|| npc->m_proto->m_index == 211
					|| npc->m_proto->m_index == 212
					|| npc->m_proto->m_index == 213
					|| npc->m_proto->m_index == 214 ) && DEAD(npc) )
				{
					this->m_currentData[0]++;
					// 랭킹 디비 마리수 업데이트
				}

				if(m_proto->m_conditionNum[0] <= this->m_currentData[0])
				{
					for(i = 0; i < QUEST_MAX_CONDITION; i++) 
						bComplete[i] = true;

					// 랭킹 디비 클리어/ 시간 업데이트
				}

				break;
			}


			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (m_proto->m_conditionType[i] == -1)
				{
					bComplete[i] = true;
					continue;
				}

				switch (m_proto->m_conditionType[i])
				{
				case QCONDITION_NPC:
					{
						// 공격하는 몹이 싱글던전 완료조건 몹의 인덱스와 같고 죽었으면
						if (m_proto->m_conditionIndex[i] == npc->m_proto->m_index && DEAD(npc))
						{
							bComplete[i] = true;
							m_currentData[i]++;
						}
					}
					break;				
				}
			}
		}
		break;
		
	default:
		return;
		break;
	}

	// bComplete가 모두 true로 셋팅 되있어야 ㅇㅋ
	for (i=0; i < QUEST_MAX_CONDITION; i++)
	{
		if (!bComplete[i])
			return;
	}


	///////////////////////////////////
	// pd4 퀘스트이면 helper end : BW
	// MSG_HELPER_PD4_RANK_END,			// pd4 종료			 : charIndex(n) deadmonNum(n) bclear(c)
	// char index를 보냄
	if(this->m_proto->m_index == 105)
	{
		CNetMsg pd4RankEndMsg;

		pd4RankEndMsg.Init(MSG_HELPER_COMMAND);
		pd4RankEndMsg << MSG_HELPER_PD4_RANK_END
					  << pc->m_index
					  << this->m_currentData[0]
					  << (char) 1;

		GAMELOG << init("PD4 END", pc)
				<< this->m_currentData[0] << end;


		if (IS_RUNNING_HELPER)
		{
			SEND_Q(pd4RankEndMsg, gserver.m_helper);
		}
		else
		{
			GAMELOG << init("PD4 QUEST ERROR : CANNOT FIND HELPER")
					<< pc->m_index << end;

			return;
		}
	}

	
	// Complete Msg 최초전송인경우에만 전송
	if (!pc->m_questList.IsCompleted(idx))
	{
		if(pc->m_questList.m_bComplete2[idx])
		{
		// Quest Complete Log
		GAMELOG << init("QUEST COMPLETE (QuestUpdateData)", pc)
			<< pc->m_questList.m_list[idx]->m_proto->m_index
			<< end;
		
		// 완료 메시지 
		CNetMsg completeMsg;
		QuestCompleteMsg(completeMsg, pc->m_questList.m_list[idx]);
		SEND_Q(completeMsg, pc->m_desc);
		}
		pc->m_questList.SetComplete(idx, true);
	}
}


////////////////////////////
// CQuestList member
CQuestList::CQuestList()
{
	memset(m_list, 0, sizeof(CQuest*) * QUEST_MAX_PC);
	memset(m_bQuest, 0, sizeof(bool) * QUEST_MAX_PC);
	memset(m_bComplete, 0, sizeof(bool) * QUEST_MAX_PC);
	memset(m_bComplete2, false, sizeof(bool) * QUEST_MAX_PC);
	
	m_nCount= 0;

	memset(m_doneQuest, -1, sizeof(int) * QUEST_MAX_PC_COMPLETE);
	memset(m_abandonQuest, -1, sizeof(int) * QUEST_MAX_PC_COMPLETE);

	m_nComplete = 0;

	m_doneCnt = 0;	
	m_abandonCnt = 0; //포기 퀘스트 갯수
}

CQuestList::~CQuestList()
{
	for (int i = 0; i < QUEST_MAX_PC; i++)
	{
		if (m_list[i])
		{
			delete m_list[i];
			m_list[i] = NULL;
		}
	}
}

int CQuestList::AddQuest(CQuest* quest)
{
	int i;
	for (i=0; i < QUEST_MAX_PC; i++)
	{
		if (!m_list[i])
		{
			m_list[i] = quest;
			m_nCount++;
			m_bComplete[i] = false;
			m_bComplete2[i] = false;
			m_bQuest[i] = false;

			return i;
		}	
	}
	
	return -1;
}

int CQuestList::FindQuest(int index)
{
	int i;
	for (i=0; i < QUEST_MAX_PC; i++)
	{
		if (m_list[i] && m_list[i]->m_proto->m_index == index && m_bQuest[i])
			return i;
	}

	return -1;
}

bool CQuestList::DelQuest(CPC* pc, CQuest* quest)
{
	int i;
	
	for (i=0; i < QUEST_MAX_PC; i++)
	{
		if (m_list[i] && m_list[i]->m_proto->m_index == quest->m_proto->m_index)
		{
			quest = NULL;
			delete m_list[i];
			m_list[i] = NULL;
			m_nCount--;
			m_bComplete[i] = false;
			m_bComplete2[i] = false;
			m_bQuest[i] = false;

			return true;
		}
	}
	
	return false;
}

char* CQuestList::GetIndexString(char* buf)
{
	*buf = '\0';
	int i;
	for (i = 0; i < QUEST_MAX_PC; i++)
	{
		if (m_list[i] && m_bQuest[i])
		{
			// 격파,구출 퀘스트 저장 하지 않는다
			if ((m_list[i]->m_proto->m_type[0] == QTYPE_KIND_SAVE || m_list[i]->m_proto->m_type[0] == QTYPE_KIND_DEFEAT) && !m_bComplete[i])
				continue;
			
			IntCat(buf, m_list[i]->m_proto->m_index);
		}
	}
	return buf;
}

char* CQuestList::GetValueString(char* buf)
{
	*buf = '\0';
	int i, j;
	for (i = 0; i < QUEST_MAX_PC; i++)
	{
		if (m_list[i] && m_bQuest[i])
		{
			// 격파,구출 퀘스트 저장 하지 않는다
			if ((m_list[i]->m_proto->m_type[0] == QTYPE_KIND_SAVE || m_list[i]->m_proto->m_type[0] == QTYPE_KIND_DEFEAT) && !m_bComplete[i])
				continue;
			
			for (j=0; j < QUEST_MAX_CONDITION; j++)
				IntCat(buf, m_list[i]->m_currentData[j]);
		}
	}
	return buf;
}

char* CQuestList::GetCompleteIndexString(char* buf)
{
	*buf = '\0';

	int i;
	for (i = 0; i < QUEST_MAX_PC_COMPLETE; i++)
	{
		if (m_doneQuest[i] == -1)
			break;

		IntCat(buf, m_doneQuest[i]);
	}

	return buf;
}

char* CQuestList::GetAbandonIndexString(char* buf)
{
	*buf = '\0';

	int i;
	for (i = 0; i < QUEST_MAX_PC_COMPLETE; i++)
	{
		if (m_abandonQuest[i] == -1)
			break;

		IntCat(buf, m_abandonQuest[i]);
	}

	return buf;
}

void CQuestList::CheckComplete(CPC* ch)
{
	int i, j;

//	bool bComplete[QUEST_MAX_CONDITION];
	
	for (i=0; i < QUEST_MAX_PC; i++)
	{
		if (m_list[i] && m_bQuest[i])
		{
			switch (m_list[i]->m_proto->m_type[0])
			{
			case QTYPE_KIND_REPEAT:
			case QTYPE_KIND_COLLECTION:
			case QTYPE_KIND_MINING_EXPERIENCE:
			case QTYPE_KIND_GATHERING_EXPERIENCE:
			case QTYPE_KIND_CHARGE_EXPERIENCE:
#ifdef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae]
			case QTYPE_KIND_PROCESS_EXPERIENCE :	
			case QTYPE_KIND_MAKE_EXPERIENCE :		
#endif // EXPRIENCE_QUEST	
				{
					bool complete = true;
					bool complete2 = true;

					for (j=0; j < QUEST_MAX_CONDITION; j++)
					{
						if (m_list[i]->m_proto->m_conditionType[j] != QCONDITION_ITEM_NORMAL)
							continue;
						
						if (m_list[i]->m_currentData[j] >= m_list[i]->m_proto->m_conditionNum[j])
							continue;

						complete2 = false;
						break;
					}

					for (j=0; j < QUEST_MAX_CONDITION; j++)
					{
						//if (m_list[i]->m_proto->m_conditionType[j] != QCONDITION_ITEM)
						if (m_list[i]->m_proto->m_conditionType[j] == -1 || m_list[i]->m_proto->m_conditionType[j] == QCONDITION_ITEM_NORMAL)
							continue;
						
						if (m_list[i]->m_currentData[j] >= m_list[i]->m_proto->m_conditionNum[j])
							continue;

						complete = false;
						break;
					}
// 원래 코드 : 050503 : BS 수정
//					memset(bComplete, false, sizeof(bool) * QUEST_MAX_CONDITION);
//
//					for (j=0; j < QUEST_MAX_CONDITION; j++)
//					{
//						if (m_list[i]->m_proto->m_conditionType[j] == -1)
//						{
//							bComplete[j] = true;
//							continue;
//						}
//						
//						if (m_list[i]->m_currentData[j] >= m_list[i]->m_proto->m_conditionNum[j])
//							bComplete[j] = true;
//					}
//					
//					for (j=0; j < QUEST_MAX_CONDITION; j++)
//					{
//						if (!bComplete[j])
//							complete = false;
//					}
// --- 원래 코드 : 050503 : BS 수정

					if(complete2)
					{
						m_bComplete2[i] = true;
					}
					if (complete)
					{
						if(complete2) 
						{
						CNetMsg msg;
						QuestCompleteMsg(msg, m_list[i]);
						SEND_Q(msg, ch->m_desc);
						}
						
						m_bComplete[i] = true;
					}
				}
				break;

			case QTYPE_KIND_DELIVERY:
				{
					bool bcomplete,bcomplete2;
					bcomplete = bcomplete2 = TRUE;

					for (j = 0; j < QUEST_MAX_CONDITION; j++)
					{						
						if (m_list[i]->m_proto->m_conditionType[j] != QCONDITION_ITEM_NORMAL)
							continue;
					
						int r, c;
						if (!ch->m_invenNormal.FindItem(&r, &c, m_list[i]->m_proto->m_conditionIndex[j], 0, 0))
						{
							//DelQuest(ch, m_list[i]);
							//return;
							bcomplete2 = FALSE;
							break;
						}

						CItem* item = ch->m_invenNormal.GetItem(r, c);
						
						if (!item)
						{
							//DelQuest(ch, m_list[i]);
							//return;
							bcomplete2 = FALSE;
							break;
						}
						
						// 필요 숫자 만큼 없음
						if (item->Count() < m_list[i]->m_proto->m_conditionNum[j])
						{
							//DelQuest(ch, m_list[i]);
							//return;
							bcomplete2 = FALSE;
							break;
						}					
					}

					if(bcomplete2)
					{
						m_bComplete2[i] = true;
					}

					for (j = 0; j < QUEST_MAX_CONDITION; j++)
					{
						// 전달 퀘스트는 아이템이 아니면 무조건 ok
						if (m_list[i]->m_proto->m_conditionType[j] != QCONDITION_ITEM)
							continue;

						// 전달 퀘스트 아이템은 퀘스트 인벤에서만 !!!
						int r, c;
						if (!ch->m_invenQuest.FindItem(&r, &c, m_list[i]->m_proto->m_conditionIndex[j], 0, 0))
						{
							//DelQuest(ch, m_list[i]);
							//return;
							bcomplete = FALSE;
							break;
						}

						// 찾았으면 수량 확인 해서 줄이기
						CItem* item = ch->m_invenQuest.GetItem(r, c);
						
						if (!item)
						{
							//DelQuest(ch, m_list[i]);
							//return;
							bcomplete = FALSE;
							break;
						}
						
						// 필요 숫자 만큼 없음
						if (item->Count() < m_list[i]->m_proto->m_conditionNum[j])
						{
							//DelQuest(ch, m_list[i]);
							//return;
							bcomplete = FALSE;
							break;
						}
						
					}

					if(bcomplete)
					{
						m_bComplete[i] = true;
					}

					if(bcomplete && bcomplete2)
					{
						CNetMsg msg;
						QuestCompleteMsg(msg, m_list[i]);
						SEND_Q(msg, ch->m_desc);
					}

					//CNetMsg msg;
					//QuestCompleteMsg(msg, m_list[i]);
					//SEND_Q(msg, ch->m_desc);

					//m_bComplete[i] = true;
				}
				break;

			case QTYPE_KIND_DEFEAT :
			case QTYPE_KIND_SAVE :
				{
					bool complete = true;
					for (j=0; j < QUEST_MAX_CONDITION; j++)
					{
						if (m_list[i]->m_proto->m_conditionType[j] != QCONDITION_ITEM && m_list[i]->m_proto->m_conditionType[j] != QCONDITION_NPC)
							continue;
						
						if (m_list[i]->m_currentData[j] >= m_list[i]->m_proto->m_conditionNum[j])
							continue;
						
						complete = false;
						break;
					}

					if(complete)
					{
						m_bComplete[i] = true; //싱글던전 퀘스트이므로..
						m_bComplete2[i] = true;

						CNetMsg msg;
						QuestCompleteMsg(msg, m_list[i]);
						SEND_Q(msg, ch->m_desc);
					}

				}
				break;

			default:
				break;
			}
		}
	}
}

#endif // QUEST_DATA_EXTEND
