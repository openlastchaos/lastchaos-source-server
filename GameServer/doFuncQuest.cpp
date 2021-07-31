#include "stdhdrs.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Log.h"

///////////////////////
// Quest 관련 함수

void do_Quest(CPC* ch, CNetMsg& msg)
{
	if (DEAD(ch))
		return ;

	msg.MoveFirst();

	unsigned char subtype;

	msg >> subtype;

	switch (subtype)
	{
	case MSG_QUEST_REQUEST:
		do_QuestRequest(ch, msg);
		break;

	case MSG_QUEST_START:
		do_QuestStart(ch, msg);
		break;

	case MSG_QUEST_GIVEUP:
		do_QuestGiveUp(ch, msg);
		break;

	case MSG_QUEST_PRIZE:
		do_QuestPrize(ch, msg);
		break;

	case MSG_QUEST_COMPLETE:
		do_QuestComplete(ch, msg);
		break;
		// BW

	case MSG_QUEST_PD4_RANK_VIEW_REQ:			// 랭크 뷰요청
		do_QuestPD4RankView(ch, msg);
		break;
	case MSG_QUEST_PD4_RANK_REWARD_RANK_REQ:	// 보상 뷰요청
		do_QuestPD4RankRewardView(ch, msg);
		break;
	case MSG_QUEST_PD4_REWARD:					// 보상 요청
		do_QuestPD4RankReward(ch, msg);
		break;

	case MSG_QUEST_COLLECT:
		do_QuestCollect(ch, msg);				// 수집 퀘스트
		break;
	}
}


void do_QuestPD4RankView(CPC* ch, CNetMsg& msg)
{
	/////////////
	// helper에게 랭크 뷰 요청 : BW
	//MSG_HELPER_PD4_RANK_VIEW_REQ : charindex(n) charjob(c)
	//MSG_HELPER_PD4_RANK_VIEW_REP : charindex(n) multicount(c), charname(str), bclear(c), deadmon(n), ctime(ll);
	char charjob;

	msg >> charjob;

	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PD4_RANK_VIEW_REQ
		<< ch->m_index
		<< charjob;

	if (IS_RUNNING_HELPER)
	{
		SEND_Q(msg, gserver.m_helper);
	}
	else
	{
		GAMELOG << init("PD4RANKVIEW REQ FAIL", ch) << end;
	}
	
}
void do_QuestPD4RankRewardView(CPC* ch, CNetMsg& msg)
{
	//////////////////////////
	//헬퍼에게 보상뷰 요청
	//MSG_HELPER_PD4_REWARD_VIEW_REQ : charindex(n) charjob(c)
	//MSG_HELPER_PD4_REWARD_VIEW_REP : charindex(n) multicount(c), charname(str)
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PD4_REWARD_VIEW_REQ
		<< ch->m_index
		<< ch->m_job;

	if (IS_RUNNING_HELPER)
	{
		SEND_Q(msg, gserver.m_helper);
	}
	else
	{
		GAMELOG << init("PD4REWARDVIEW REQ FAIL", ch) << end;
	}
}
void do_QuestPD4RankReward(CPC* ch, CNetMsg& msg)
{
	//////////////////////////////////
	// 헬퍼에게 보상 받을 사람 요청
	// 보상 안해주면 존내 맞는거다 얼씨구
	//MSG_HELPER_PD4_REWARD_REQ : charindex(n)
	//MSG_HELPER_PD4_REWARD_REP : charindex(n) charrank(c) breward(c)
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PD4_REWARD_REQ
		<< ch->m_index;

	if (IS_RUNNING_HELPER)
	{
		SEND_Q(msg, gserver.m_helper);
	}
	else
	{
		GAMELOG << init("PD4REWARD REQ FAIL", ch) << end;
	}
}


void do_QuestRequest(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype;
	int npcIndex;

	msg.MoveFirst();

	msg >> subtype
		>> npcIndex;

	// 해당 Quest npc가 주위에 있는가
	CCharacter* tch = NULL;
	CNPC* npc = NULL;

	int sx = ch->m_cellX - 1;
	int ex = ch->m_cellX + 1;
	int sz = ch->m_cellZ - 1;
	int ez = ch->m_cellZ + 1;

	int x, z;
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= ch->m_pArea->m_size[0])
			continue ;
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= ch->m_pArea->m_size[1])
				continue ;

			tch = ch->m_pArea->m_cell[x][z].m_listChar;

			while (tch)
			{
				if (IS_NPC(tch))
				{
					if (TO_NPC(tch)->m_idNum == npcIndex)
					{
						npc = TO_NPC(tch);
						break;
					}
				}
				tch = tch->m_pCellNext;
			}
		}
	}
	
	// 주위에 해당 npc 없음
	if (!npc)
		return;

	// Quest Request Log
	GAMELOG << init("QUEST REQUEST",  ch)
			<< npcIndex
			<< end;

	// npc가 보여줄 최대 퀘스트 수
	int	count = npc->m_nQuestCount + QUEST_MAX_PC;
	
	// 퀘스트 플래그
	int* flag;
	flag = new int [count];
	memset(flag, -1, sizeof(int) * count);

	// 퀘스트 인덱스
	int* index;
	index = new int [count];
	memset(index, -1, sizeof(int) * count);

	// 퀘스트 보상의 경우
	int i;
	int questCount = 0;

#ifdef QUEST_DATA_EXTEND
	CQuest* pQuest;
	CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		if (	pQuest->IsCompleted()
			&&	pQuest->GetPrizeNPCIndex() == npc->m_proto->m_index
			&&	(pQuest->GetPrizeNPCZone() == -1 || pQuest->GetPrizeNPCZone() == npc->m_pZone->m_index)
			)
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_COMPLETE;
			index[questCount] = pQuest->GetQuestIndex();
			questCount++;
		}
	}
#else // QUEST_DATA_EXTEND
	for (i=0; i < QUEST_MAX_PC; i++)
	{
		if (!ch->m_questList.m_list[i])
			continue;

		// 퀘스트가 있고 수행중이고 완료 했고 보상 npc와 같으면
		if (ch->m_questList.m_list[i] && ch->m_questList.m_bQuest[i] && ch->m_questList.IsCompleted(i) &&
			ch->m_questList.m_list[i]->m_proto->m_prizeNPC == npc->m_proto->m_index
		
			&& (ch->m_questList.m_list[i]->m_proto->m_prizeNpcZoneNum == -1 ||
			(ch->m_questList.m_list[i]->m_proto->m_prizeNpcZoneNum != -1 && (ch->m_questList.m_list[i]->m_proto->m_prizeNpcZoneNum == npc->m_pZone->m_index)))			
		)
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_COMPLETE;
			index[questCount] = ch->m_questList.m_list[i]->m_proto->m_index;
			questCount++;
		}
	}
#endif // QUEST_DATA_EXTEND

	// 퀘스트를 받으려는 경우
	for (i=0; i < npc->m_nQuestCount; i++)
	{
		int j;
		CQuestProto* proto = gserver.m_questProtoList.FindProto(npc->m_quest[i]);

		if (!proto)
			continue;


		if(proto->m_startNpcZoneNum != -1 && (proto->m_startNpcZoneNum != npc->m_pZone->m_index))
		{
			continue;
		}

#ifdef QUEST_DATA_EXTEND
		CQuest* pQuest;
		// 1회용 퀘스트이면서 이미 수행한 퀘스트가 아닌가
		if (proto->m_type[1] == QTYPE_REPEAT_ONCE)
		{
			bool bOnce = false;		// 이미 수행한 1회용 퀘스트 인가
			
			CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL);
			while ((pQuest = pQuestNext))
			{
				pQuestNext = ch->m_questList.GetNextQuest(pQuestNext);
				switch (pQuest->GetQuestState())
				{
				case QUEST_STATE_DONE:
				case QUEST_STATE_ABANDON:
					if (pQuest->GetQuestIndex() == proto->m_index)
						bOnce = true;
					break;
				default:
					break;
				}
				if (bOnce)
					break;;
			}
			if (bOnce)
				continue;
		}

		// 수행중인가
		pQuest = ch->m_questList.FindQuest(proto->m_index);
		// 수행중이면서
		if (pQuest && pQuest->GetQuestState() == QUEST_STATE_RUN)
		{
			// 완료면 ?			if (pQuest->IsCompleted())
				continue;
			// 완료 아니면
			else
			{
				flag[questCount] = (int)MSG_QUEST_FLAG_ING;
				index[questCount] = proto->m_index;
				questCount++;
				continue;
			}
		}
#else // QUEST_DATA_EXTEND
		// 1회용 퀘스트이면서 이미 수행한 퀘스트가 아닌가
		if (proto->m_type[1] == QTYPE_REPEAT_ONCE)
		{
			bool bOnce = false;		// 이미 수행한 1회용 퀘스트 인가
			
			for (j=0; j < QUEST_MAX_PC_COMPLETE; j++)
			{
				if (ch->m_questList.GetDoneQuest(j) == -1)
					break;
				
				if (ch->m_questList.GetDoneQuest(j) == proto->m_index)
				{
					bOnce = true;
					break;
				}
			}
			
			if (bOnce)
				continue;

			// 포기한 퀘스트 인지 체크.
			bOnce = false;
			for (j=0; j < QUEST_MAX_PC_COMPLETE; j++)
			{
				if (ch->m_questList.m_abandonQuest[j] == -1)
					break;
				
				if (ch->m_questList.m_abandonQuest[j] == proto->m_index)
				{
					bOnce = true;
					break;
				}
			}
			if (bOnce)
				continue;
		}

		// 수행중인가
		int idx = ch->m_questList.FindQuest(proto->m_index);

		// 수행중이면서
		if (idx != -1)
		{
			// 완료면 ?			if (ch->m_questList.IsCompleted(idx))
				continue;
			// 완료 아니면
			else
			{
				flag[questCount] = (int)MSG_QUEST_FLAG_ING;
				index[questCount] = proto->m_index;
				questCount++;
				continue;
			}
		}
#endif // QUEST_DATA_EXTEND

		// 필요 레벨 조건
		if (proto->m_needMinLevel > ch->m_level || proto->m_needMaxLevel < ch->m_level)
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_NOT_LEVEL;
			index[questCount] = proto->m_index;
			questCount++;
			continue;
		}

		// 경험치가 부족하다..
		if((proto->m_needMinLevel == ch->m_level) && proto->m_needExp != 0 &&	(ch->m_exp < proto->m_needExp) )
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_NOT_EXP;
			index[questCount] = proto->m_index;
			questCount++;
			continue;
		}

		//선행퀘스트를 수행하였는가...
		if(proto->m_prequestNum != 0)		
		{
			bool bOnce = false;		

#ifdef QUEST_DATA_EXTEND
			CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_DONE);
			while ((pQuest = pQuestNext))
			{
				pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_DONE);
				if (pQuest->GetQuestIndex() == proto->m_prequestNum)
				{
					bOnce = true;
					break;
				}
			}
#else // QUEST_DATA_EXTEND
			for (j=0; j < QUEST_MAX_PC_COMPLETE; j++)
			{
				if (ch->m_questList.m_doneQuest[j] == -1)
					break;
				
				if (ch->m_questList.m_doneQuest[j] == proto->m_prequestNum)
				{
					bOnce = true;
					break;
				}
			}
#endif // QUEST_DATA_EXTEND
			
			if (!bOnce)//아직 선행 퀘스트를 완료하지 않았다면,
			{
				flag[questCount] = (int)MSG_QUEST_FLAG_NOT_PREQUEST;
				index[questCount] = proto->m_index;
				questCount++;
				continue;
			}
		}

		// 필요 직업 조건
		if (proto->m_needJob != -1 && proto->m_needJob != ch->m_job)
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_NOT_JOB;
			index[questCount] = proto->m_index;
			questCount++;
			continue;
		}

		// 필요 아이템 체크
		int k;
		bool bNeedItem = true;	// 필요아이템 조건 만족

		for (j=0; j < QUEST_MAX_NEED_ITEM; j++)
		{
			if (proto->m_needItemIndex[j] == -1)
				continue;
			
			for (k=0; k < INVENTORY_EVENT; k++)
			{
				CInventory* inven = GET_INVENTORY(ch, k);
				
				if (!inven)
					continue;

				int r, c;
				if (!inven->FindItem(&r, &c, proto->m_needItemIndex[j], 0, 0))
				{
					// 마지막 이벤트 인벤토리에도 없는 경우
					if (k == INVENTORY_EVENT - 1)
					{
						bNeedItem = false;
						break;
					}
					else
						continue;
				}
				
				CItem* item = inven->GetItem(r, c);
				
				if (!item)
				{
					bNeedItem = false;
					break;
				}
				
				// 필요 숫자만큼 없음
				if (item->Count() < proto->m_needItemCount[j])
				{
					bNeedItem = false;
					break;
				}

				break;
			}
		}

		if (!bNeedItem)
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_NOT_ITEM;
			index[questCount] = proto->m_index;
			questCount++;
			continue;
		}

		// 격파, 구출 퀘스트의 경우 싱글던전 여유분 체크
		if (proto->m_type[0] == QTYPE_KIND_DEFEAT || proto->m_type[0] == QTYPE_KIND_SAVE)
		{
			int zoneidx;
			CZone* pZone = NULL;

			switch (proto->m_index)
			{
			case 13:		// 발록 격파
				zoneidx = ZONE_SINGLE_DUNGEON1;
				break;
			case 14:		// 공주 구출
				zoneidx = ZONE_SINGLE_DUNGEON2;
				break;
#ifdef ENABLE_SINGLEDUNGEON_DATA
			case 104:		// 아자카 협곡
				// 하루 3회 까지만
				if (!ch->CheckDungeonData(ZONE_SINGLE_DUNGEON3))
				{
					flag[questCount] = (int)MSG_QUEST_FLAG_OVERCOUNT;
					index[questCount] = proto->m_index;
					questCount++;
					continue ;
				}
				zoneidx = ZONE_SINGLE_DUNGEON3;
				break;
			case 105:		// 차원의 문
				// 하루 3회 까지만
				if (!ch->CheckDungeonData(ZONE_SINGLE_DUNGEON4))
				{
					flag[questCount] = (int)MSG_QUEST_FLAG_OVERCOUNT;
					index[questCount] = proto->m_index;
					questCount++;

					continue ;
				}
				zoneidx = ZONE_SINGLE_DUNGEON4;
				break;
#endif
			}

			zoneidx = gserver.FindZone(zoneidx);
			if (zoneidx == -1)
			{
				flag[questCount] = (int)MSG_QUEST_FLAG_NOTENOUGH_AREA;
				index[questCount] = proto->m_index;
				questCount++;
				continue ;
			}
			else
				pZone = gserver.m_zones + zoneidx;
			
			if (!pZone)
			{
				flag[questCount] = (int)MSG_QUEST_FLAG_NOTENOUGH_AREA;
				index[questCount] = proto->m_index;
				questCount++;
				continue ;
			}
			
			for (j=0 ; j < pZone->m_countArea; j++)
			{
				if (!pZone->m_area[j].m_bEnable)
					break;
			}
			// 싱글던전 모두 꽉차다 목록에 나오지 않도록 continue;
			if (j == pZone->m_countArea)
			{
				flag[questCount] = (int)MSG_QUEST_FLAG_NOTENOUGH_AREA;
				index[questCount] = proto->m_index;
				questCount++;
				continue ;
			}
		}

		// 하드코딩 : 생산스킬습득 퀘스트 새로 나오지 않도록 조정
//		if (proto->m_index == 27 || proto->m_index == 28 || proto->m_index == 29)
//			continue;

		// 초기값 그대로라면 수행 가능
		if (flag[questCount] == -1)
		{
			flag[questCount] = (int)MSG_QUEST_FLAG_CAN;
			index[questCount] = proto->m_index;
			questCount++;
		}
	}

	CNetMsg rMsg;
	QuestNPCListMsg(rMsg, questCount, flag, index);
	SEND_Q(rMsg, ch->m_desc);

	if (flag)
		delete [] flag;
	if (index)
		delete [] index;
}

void do_QuestStart(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype;
	int questIndex;
	CNetMsg rMsg;

	msg.MoveFirst();

	msg >> subtype
		>> questIndex;

	// 10개 초과
#ifdef QUEST_DATA_EXTEND
	if (ch->m_questList.GetCountRun() >= QUEST_MAX_PC)
	{
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_MAX);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
#else // QUEST_DATA_EXTEND
	if (ch->m_questList.m_nCount >= QUEST_MAX_PC)
	{
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_MAX);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
#endif // QUEST_DATA_EXTEND

	CQuestProto* proto = gserver.m_questProtoList.FindProto(questIndex);

	if (!proto)
		return;

	// 시작 조건
	if (proto->m_startType == QSTART_LEVEL)
	{
		if (proto->m_startData > ch->m_level)
		{
			QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_LEVEL);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
	}
	else if(proto->m_startType == QSTART_NPC)
	{
		// NPC와의 셀 2칸
		CNPC* npc = ch->m_pArea->FindProtoNPCInCell(ch, proto->m_startData, false, 2);
		if( !npc )
			return;
	}

	int i;

	// 1회용 퀘스트 이면서 실행했는지
#ifdef QUEST_DATA_EXTEND
	CQuest* pQuest;
	CQuest* pQuestNext;
	if (proto->m_type[1] == QTYPE_REPEAT_ONCE)
	{
		pQuestNext = ch->m_questList.GetNextQuest(NULL);
		while ((pQuest = pQuestNext))
		{
			pQuestNext = ch->m_questList.GetNextQuest(pQuestNext);
			if (pQuest->GetQuestIndex() == proto->m_index)
			{
				QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_ONCE);
				SEND_Q(rMsg, ch->m_desc);
				return;
			}
		}
	}

	// 수행중인가
	pQuest = ch->m_questList.FindQuest(proto->m_index);
	if (pQuest)
	{
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_ALREADY);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
#else // QUEST_DATA_EXTEND
	if (proto->m_type[1] == QTYPE_REPEAT_ONCE)
	{
		for (i=0; i < QUEST_MAX_PC_COMPLETE; i++)
		{
			if (ch->m_questList.GetDoneQuest(i) == -1)
				break;

			if (ch->m_questList.GetDoneQuest(i) == proto->m_index)
			{
				QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_ONCE);
				SEND_Q(rMsg, ch->m_desc);
				return;
			}
		}
	}

	// 수행중인가
	int idx = ch->m_questList.FindQuest(proto->m_index);
	if (idx != -1)
	{
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_ALREADY);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
#endif // QUEST_DATA_EXTEND
	
	// 필요 레벨 조건
	if (proto->m_needMinLevel > ch->m_level || proto->m_needMaxLevel < ch->m_level)
	{
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_LEVEL);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
	
	// 필요 직업 조건
	if (proto->m_needJob != -1 && proto->m_needJob != ch->m_job)
	{
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_JOB);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	// 격파, 구출 퀘스트의 경우 싱글던전 여유분 체크
	// 입장 제한
	if (proto->m_type[0] == QTYPE_KIND_DEFEAT || proto->m_type[0] == QTYPE_KIND_SAVE)
	{
		CZone* pZone = NULL;

		int zoneidx = -1;
		switch (proto->m_index)
		{
		case 13:		// 발록 격파
			zoneidx = ZONE_SINGLE_DUNGEON1;
			break;
		case 14:		// 공주 구출
			zoneidx = ZONE_SINGLE_DUNGEON2;
			break;
#ifdef ENABLE_SINGLEDUNGEON_DATA
		case 104:		// 아자카 협곡
			// 하루 3회 까지만
			if (!ch->CheckDungeonData(ZONE_SINGLE_DUNGEON3))
			{
				QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_MAX);
				SEND_Q(rMsg, ch->m_desc);
				return;
			}
			
			zoneidx = ZONE_SINGLE_DUNGEON3;
			break;
		case 105:		// 차원의 문
			// 하루 3회 까지만
			if (!ch->CheckDungeonData(ZONE_SINGLE_DUNGEON4))
			{
				QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_MAX);
				SEND_Q(rMsg, ch->m_desc);
				return;
			}


			ch->m_pd4StartTime = 10 * 60 * 20 * 3; // 1시간* 60 * 20 * 3

			zoneidx = ZONE_SINGLE_DUNGEON4;
			break;
#endif
		}
		zoneidx = gserver.FindZone(zoneidx);

		if (zoneidx < 0)
		{
			QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_FULL);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}

		pZone = gserver.m_zones + zoneidx;
		
		for (i=0 ; i < pZone->m_countArea; i++)
		{
			if (!pZone->m_area[i].m_bEnable)
				break;
		}
		// 싱글던전 모두 꽉차다
		if (i == pZone->m_countArea)
		{
			QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_FULL);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
	}
	
	// 필요 아이템 체크
	int j;
	for (i=0; i < QUEST_MAX_NEED_ITEM; i++)
	{
		if (proto->m_needItemIndex[i] == -1)
			continue;
		
		for (j=0; j < INVENTORY_EVENT; j++)
		{
			CInventory* inven = GET_INVENTORY(ch, j);
			
			if (!inven)
				continue;
			
			int r, c;
			if (!inven->FindItem(&r, &c, proto->m_needItemIndex[i], 0, 0))
			{
				// 마지막 이벤트 이벤토리에도 없는 경우
				if (j == INVENTORY_EVENT - 1)
				{
					QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_ITEM);
					SEND_Q(rMsg, ch->m_desc);
					return;
				}
				else
					continue;
			}
			
			CItem* item = inven->GetItem(r, c);
			
			if (!item)
				continue;
			
			// 필요 숫자만큼 없음
			if (item->Count() < proto->m_needItemCount[i])
			{
				QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_ITEM);
				SEND_Q(rMsg, ch->m_desc);
				return;
			}

			CNetMsg itemMsg;
			
			// Item 수량 변경
			DecreaseFromInventory(ch, item, proto->m_needItemCount[i]);
			
			if (item->Count() > 0)
			{
				ItemUpdateMsg(itemMsg, item, -proto->m_needItemCount[i]);
				SEND_Q(itemMsg, ch->m_desc);
			}
			else
			{
				ItemDeleteMsg(itemMsg, item);
				SEND_Q(itemMsg, ch->m_desc);
				ch->RemoveItemFromQuickSlot(item);
				RemoveFromInventory(ch, item, true, true);
			}
			break;
		}
	}
	
#ifdef QUEST_DATA_EXTEND
	pQuest = ch->m_questList.AddQuest(proto->m_index);
	if (pQuest == NULL)
		return ;
	ch->m_questList.SetQuestState(pQuest, QUEST_STATE_RUN);
	pQuest->SetComplete1(false);
	pQuest->SetComplete2(false);

	///////////////////////////////////
	// pd4 퀘스트이면 helper 랭킹에 등록 : BW
	//
	if (proto->m_index == 105)
	{
		// char index를 보냄
		CNetMsg pd4RankAddMsg;

		pd4RankAddMsg.Init(MSG_HELPER_COMMAND);
		pd4RankAddMsg << MSG_HELPER_PD4_RANK_ADD
					  << ch->m_index;

		GAMELOG << init("PD4 START", ch) << end;

		if (IS_RUNNING_HELPER)
		{
			SEND_Q(pd4RankAddMsg, gserver.m_helper);
		}
		else
		{
			GAMELOG << init("PD4 QUEST ERROR : RANK ADD ", ch) << end;
		}
	}
	
	// Quest Start Log
	GAMELOG << init("QUEST START",  ch)
			<< pQuest->GetQuestIndex()
			<< end;
	
	// Quest Start Msg
	QuestStartMsg(rMsg, pQuest);
	SEND_Q(rMsg, ch->m_desc);

#else // QUEST_DATA_EXTEND

	CQuest* pQuest = gserver.m_questProtoList.Create(proto->m_index);
	
	if (!pQuest)
		return;

	const CQuestProto* pQuestProto = pQuest->m_proto;
	
	///////////////////////////////////
	// pd4 퀘스트이면 helper 랭킹에 등록 : BW
	//
	if( pQuestProto->m_index == 105)
	{
		// char index를 보냄
		CNetMsg pd4RankAddMsg;

		pd4RankAddMsg.Init(MSG_HELPER_COMMAND);
		pd4RankAddMsg << MSG_HELPER_PD4_RANK_ADD
					  << ch->m_index;

		GAMELOG << init("PD4 START", ch) << end;

		if (IS_RUNNING_HELPER)
		{
			SEND_Q(pd4RankAddMsg, gserver.m_helper);
		}
		else
		{

			GAMELOG << init("PD4 QUEST ERROR : RANK ADD ", ch) << end;
			return;
		}

	}

	// 캐릭터 퀘스트 리스트에 Add
	idx = ch->m_questList.AddQuest(pQuest);
	if (idx < 0)
	{
		delete pQuest;
		return;
	}
	
	ch->m_questList.m_bQuest[idx] = true;
	ch->m_questList.SetComplete(idx, false);
	ch->m_questList.SetComplete2(idx, false);
	
	// Quest Start Log
	GAMELOG << init("QUEST START",  ch)
		<< pQuestProto->m_index
		<< end;
	
	// Quest Start Msg
	QuestStartMsg(rMsg, ch->m_questList.m_list[idx]);
	SEND_Q(rMsg, ch->m_desc);

#endif // QUEST_DATA_EXTEND
	
	// 퀘스트 시작 시 뭔가 하는 것들
	switch (proto->m_type[0])
	{
		// 전달 퀘스트
	case QTYPE_KIND_DELIVERY:
		{
			//  [2/28/2007 KwonYongDae] 153번 퀘스트 인경우 아이템 지급 금지 응급 처치 
			if( questIndex == 153 )
			{
				
				ch->m_questList.CheckComplete( ch );		// 퀘스트 완료 체크 				
				break;
			}

			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (proto->m_conditionType[i] == -1)
					continue;
				
				switch (proto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
				case QCONDITION_ITEM_NORMAL:
					{
						CItem* item = NULL;
						item = gserver.m_itemProtoList.CreateItem(proto->m_conditionIndex[i], -1, 0, 0, proto->m_conditionNum[i]);
						
						if (!item)
							continue;
						
						CNetMsg itemmsg;
						
						// 들어갈 인벤토리 결정
						CInventory* inven = GET_INVENTORY(ch, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
						if (!inven)
							continue ;
						
						bool bCountable = false;
						// 인벤에 넣기
						if (AddToInventory(ch, item, true, true))
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
								ItemAddMsg(itemmsg, item);
						}
						else
						{
							// 인젠토리 꽉차서 못 받을 때
							GAMELOG << init("QUEST ERROR", ch)
								<< proto->m_index << delim
								<< (int)MSG_QUEST_ERR_DELIVERY_FULL
								<< end;
							
							QuestErrorMsg(rMsg, MSG_QUEST_ERR_DELIVERY_FULL);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rMsg, ch->m_desc);
							delete item;
							return;
						}
						
						// 메시지 보내고
						SEND_Q(itemmsg, ch->m_desc);
						
						// Item LOG
						GAMELOG << init("ITEM_PICK_QUESTITEM", ch)
								<< itemlog(item)
								<< end;
						
						if (bCountable)
							delete item;

#ifdef QUEST_DATA_EXTEND
						pQuest->SetQuestValue(i, proto->m_conditionNum[i]);
#else
						pQuest->m_currentData[i] = proto->m_conditionNum[i];
#endif // QUEST_DATA_EXTEND
					}
					break;
					
				default:
					break;
				}
			}
			
			//ch->m_questList.SetComplete(idx, true);	
#ifdef QUEST_DATA_EXTEND
			pQuest->SetComplete2(true);
			
			// Complete Msg 최초전송인경우에만 전송
			if (!pQuest->IsCompleted())
			{
				if (pQuest->GetComplete2())
				{
					// Quest Complete Log
					GAMELOG << init("QUEST COMPLETE (do_QuestStart)", ch)
							<< pQuest->GetQuestIndex()
							<< end;
					
					// 완료 메시지 
					CNetMsg completeMsg;
					QuestCompleteMsg(completeMsg, pQuest);
					SEND_Q(completeMsg, ch->m_desc);
				}
				pQuest->SetComplete1(true);
			}
#else // QUEST_DATA_EXTEND
			ch->m_questList.SetComplete2(idx, true);	
			
			// Complete Msg 최초전송인경우에만 전송
			if (!ch->m_questList.IsCompleted(idx))
			{
				if(ch->m_questList.m_bComplete2[idx])
				{
				// Quest Complete Log
				GAMELOG << init("QUEST COMPLETE (do_QuestStart)", ch)
					<< ch->m_questList.m_list[idx]->m_proto->m_index
					<< end;
				
				// 완료 메시지 
				CNetMsg completeMsg;
				QuestCompleteMsg(completeMsg, ch->m_questList.m_list[idx]);
				SEND_Q(completeMsg, ch->m_desc);
				}
				ch->m_questList.SetComplete(idx, true);
			}
#endif // QUEST_DATA_EXTEND
		}
		break;

		// 채굴, 채집, 차지 체험 퀘스트
	case QTYPE_KIND_MINING_EXPERIENCE:
		{
			// 생산도구 지급
			CItem* tool = NULL;

			for (i=0; i < gserver.m_itemProtoList.m_nCount; i++)
			{
				if (gserver.m_itemProtoList.m_protoItems[i].m_typeIdx != ITYPE_WEAPON)
					continue;
				if (gserver.m_itemProtoList.m_protoItems[i].m_subtypeIdx != IWEAPON_MINING)
					continue;
				if (!(gserver.m_itemProtoList.m_protoItems[i].m_jobFlag & (1 << ch->m_job)))
					continue;

				tool = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_protoItems[i].m_index, -1, 0, 0, 1);
				break;
			}

			if (!tool)
				return;

			CNetMsg itemmsg;
			
			// 인벤에 넣기
			if (AddToInventory(ch, tool, true, true))
					ItemAddMsg(itemmsg, tool);
			else
			{
				// Drop Msg 보내기
				tool = ch->m_pArea->DropItem(tool, ch);
				if (!tool)
					return;

				CNetMsg dropMsg;
				tool->m_preferenceIndex = ch->m_index;
				ItemDropMsg(dropMsg, ch, tool);
				ch->m_pArea->SendToCell(dropMsg, GET_YLAYER(tool), tool->m_cellX, tool->m_cellZ);
			}
			
			// 메시지 보내고
			SEND_Q(itemmsg, ch->m_desc);
			
			// Item LOG
			GAMELOG << init("ITEM_PICK_QUESTITEM", ch)
					<< itemlog(tool)
					<< end;

#ifdef QUEST_DATA_EXTEND
			pQuest->SetComplete2(true);
#else
			ch->m_questList.SetComplete2(idx, true);
#endif // QUEST_DATA_EXTEND
			/*
			// 생산 스킬 지급
			CSSkillProto* proto = NULL;
			for (i=0; i < gserver.m_sSkillProtoList.m_nCount; i++)
			{
				if (gserver.m_sSkillProtoList.m_proto[i].m_type != SSKILL_MINING)
					continue;
				if (gserver.m_sSkillProtoList.m_proto[i].m_preference != 0)
					continue;

				proto = gserver.m_sSkillProtoList.FindProto(gserver.m_sSkillProtoList.m_proto[i].m_index);
				break;
			}

			if (!proto)
				return;
			
			CSSkill* sskill = ch->m_sSkillList.Find(proto->m_index);
			
			// 이미 배운 스킬 이라면
			if (sskill)
				return;
			
			sskill = new CSSkill(proto, 1);
			if (!sskill)
				return;
			
			bool bAdd = true;
			
			CSSkillNode* node;
			CSSkillNode* nodeNext = ch->m_sSkillList.m_head;
			while ((node = nodeNext))
			{
				nodeNext = nodeNext->m_next;
				
				// 타입이 같고
				if (sskill->m_proto->m_preference != -1 && node->m_sskill->m_proto->m_type == sskill->m_proto->m_type)
				{
					//우선순위가 높은 특수스킬일 가지고 있다면 ?					if (node->m_sskill->m_proto->m_preference >= sskill->m_proto->m_preference)
					{
						bAdd = false;
						break;
					}
				}
			}
			
			if (bAdd)
			{
				if (!ch->m_sSkillList.Add(sskill))
					return;
				
				// 스킬 배움 메시지 전송
				CNetMsg msg;
				SSkillLearnMsg(msg, sskill);
				SEND_Q(msg, ch->m_desc);
				
				GAMELOG << init("QUEST PRIZE")
					<< ch->m_name << delim
					<< pQuestProto->m_index << delim
					<< QPRIZE_SSKILL << delim
					<< pQuestProto->m_prizeIndex[i] << delim
					<< pQuestProto->m_prizeData[i]
					<< end;
			}
			*/
		}
		break;

	case QTYPE_KIND_GATHERING_EXPERIENCE:
		{
			// 생산도구 지급
			CItem* tool = NULL;

			for (i=0; i < gserver.m_itemProtoList.m_nCount; i++)
			{
				if (gserver.m_itemProtoList.m_protoItems[i].m_typeIdx != ITYPE_WEAPON)
					continue;
				if (gserver.m_itemProtoList.m_protoItems[i].m_subtypeIdx != IWEAPON_GATHERING)
					continue;
				if (!(gserver.m_itemProtoList.m_protoItems[i].m_jobFlag & (1 << ch->m_job)))
					continue;

				tool = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_protoItems[i].m_index, -1, 0, 0, 1);
				break;
			}

			if (!tool)
				return;

			CNetMsg itemmsg;
			
			// 인벤에 넣기
			if (AddToInventory(ch, tool, true, true))
					ItemAddMsg(itemmsg, tool);
			else
			{
				tool = ch->m_pArea->DropItem(tool, ch);
				if (!tool)
					return;

				// Drop Msg 보내기
				CNetMsg dropMsg;
				tool->m_preferenceIndex = ch->m_index;
				ItemDropMsg(dropMsg, ch, tool);
				ch->m_pArea->SendToCell(dropMsg, GET_YLAYER(tool), tool->m_cellX, tool->m_cellZ);
			}
			
			// 메시지 보내고
			SEND_Q(itemmsg, ch->m_desc);
			
			// Item LOG
			GAMELOG << init("ITEM_PICK_QUESTITEM", ch)
					<< itemlog(tool)
					<< end;

#ifdef QUEST_DATA_EXTEND
			pQuest->SetComplete2(true);
#else
			ch->m_questList.SetComplete2(idx, true);
#endif // QUEST_DATA_EXTEND

			/*
			// 생산 스킬 지급
			CSSkillProto* proto = NULL;
			for (i=0; i < gserver.m_sSkillProtoList.m_nCount; i++)
			{
				if (gserver.m_sSkillProtoList.m_proto[i].m_type != SSKILL_GATHERING)
					continue;
				if (gserver.m_sSkillProtoList.m_proto[i].m_preference != 0)
					continue;

				proto = gserver.m_sSkillProtoList.FindProto(gserver.m_sSkillProtoList.m_proto[i].m_index);
				break;
			}

			if (!proto)
				return;
			
			CSSkill* sskill = ch->m_sSkillList.Find(proto->m_index);
			
			// 이미 배운 스킬 이라면
			if (sskill)
				return;
			
			sskill = new CSSkill(proto, 1);
			if (!sskill)
				return;

			bool bAdd = true;
			
			CSSkillNode* node;
			CSSkillNode* nodeNext = ch->m_sSkillList.m_head;
			while ((node = nodeNext))
			{
				nodeNext = nodeNext->m_next;
				
				// 타입이 같고
				if (sskill->m_proto->m_preference != -1 && node->m_sskill->m_proto->m_type == sskill->m_proto->m_type)
				{
					//우선순위가 높은 특수스킬일 가지고 있다면 ?					if (node->m_sskill->m_proto->m_preference >= sskill->m_proto->m_preference)
					{
						bAdd = false;
						break;
					}
				}
			}
			
			if (bAdd)
			{
				if (!ch->m_sSkillList.Add(sskill))
					return;
				
				// 스킬 배움 메시지 전송
				CNetMsg msg;
				SSkillLearnMsg(msg, sskill);
				SEND_Q(msg, ch->m_desc);
				
				GAMELOG << init("QUEST PRIZE")
					<< ch->m_name << delim
					<< pQuestProto->m_index << delim
					<< QPRIZE_SSKILL << delim
					<< pQuestProto->m_prizeIndex[i] << delim
					<< pQuestProto->m_prizeData[i]
					<< end;
			}
			*/
		}
		break;

	case QTYPE_KIND_CHARGE_EXPERIENCE:
		{
			// 생산도구 지급
			CItem* tool = NULL;

			for (i=0; i < gserver.m_itemProtoList.m_nCount; i++)
			{
				if (gserver.m_itemProtoList.m_protoItems[i].m_typeIdx != ITYPE_WEAPON)
					continue;
				if (gserver.m_itemProtoList.m_protoItems[i].m_subtypeIdx != IWEAPON_CHARGE)
					continue;
				if (!(gserver.m_itemProtoList.m_protoItems[i].m_jobFlag & (1 << ch->m_job)))
					continue;

				tool = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_protoItems[i].m_index, -1, 0, 0, 1);
				break;
			}

			if (!tool)
				return;

			CNetMsg itemmsg;
			
			// 인벤에 넣기
			if (AddToInventory(ch, tool, true, true))
					ItemAddMsg(itemmsg, tool);
			else
			{
				tool = ch->m_pArea->DropItem(tool, ch);
				if (!tool)
					return;

				// Drop Msg 보내기
				CNetMsg dropMsg;
				tool->m_preferenceIndex = ch->m_index;
				ItemDropMsg(dropMsg, ch, tool);
				ch->m_pArea->SendToCell(dropMsg, GET_YLAYER(tool), tool->m_cellX, tool->m_cellZ);
			}
			
			// 메시지 보내고
			SEND_Q(itemmsg, ch->m_desc);
			
			// Item LOG
			GAMELOG << init("ITEM_PICK_QUESTITEM", ch)
					<< itemlog(tool)
					<< end;

#ifdef QUEST_DATA_EXTEND
			pQuest->SetComplete2(true);
#else
			ch->m_questList.SetComplete2(idx, true);
#endif // QUEST_DATA_EXTEND

			/*
			// 생산 스킬 지급
			CSSkillProto* proto = NULL;
			for (i=0; i < gserver.m_sSkillProtoList.m_nCount; i++)
			{
				if (gserver.m_sSkillProtoList.m_proto[i].m_type != SSKILL_CHARGE)
					continue;
				if (gserver.m_sSkillProtoList.m_proto[i].m_preference != 0)
					continue;

				proto = gserver.m_sSkillProtoList.FindProto(gserver.m_sSkillProtoList.m_proto[i].m_index);
				break;
			}

			if (!proto)
				return;
			
			CSSkill* sskill = ch->m_sSkillList.Find(proto->m_index);
			
			// 이미 배운 스킬 이라면
			if (sskill)
				return;
			
			sskill = new CSSkill(proto, 1);
			if (!sskill)
				return;
			
			bool bAdd = true;
			
			CSSkillNode* node;
			CSSkillNode* nodeNext = ch->m_sSkillList.m_head;
			while ((node = nodeNext))
			{
				nodeNext = nodeNext->m_next;
				
				// 타입이 같고
				if (sskill->m_proto->m_preference != -1 && node->m_sskill->m_proto->m_type == sskill->m_proto->m_type)
				{
					//우선순위가 높은 특수스킬일 가지고 있다면 ?					if (node->m_sskill->m_proto->m_preference >= sskill->m_proto->m_preference)
					{
						bAdd = false;
						break;
					}
				}
			}
			
			if (bAdd)
			{
				if (!ch->m_sSkillList.Add(sskill))
					return;
				
				// 스킬 배움 메시지 전송
				CNetMsg msg;
				SSkillLearnMsg(msg, sskill);
				SEND_Q(msg, ch->m_desc);
				
				GAMELOG << init("QUEST PRIZE")
					<< ch->m_name << delim
					<< pQuestProto->m_index << delim
					<< QPRIZE_SSKILL << delim
					<< pQuestProto->m_prizeIndex[i] << delim
					<< pQuestProto->m_prizeData[i]
					<< end;
			}
			*/
		}
		break;
	// 시작시 아이템을 제공하는 퀘스트 
#ifdef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae] Quest Start
	case QTYPE_KIND_PROCESS_EXPERIENCE :		
	case QTYPE_KIND_MAKE_EXPERIENCE :		
		{
			// QUEST 150 : 구리 가공 확률만 100%
			CItem * itemOfferd = NULL ;
			int		iOfferdIndex = -1;
			//int		itemOfferdIndex = -1;
			switch ( questIndex )
			{
			case 150 :	iOfferdIndex = 269;	break;		// 스톤 정제 체험 - 스톤온 정련서 
			case 151 :	iOfferdIndex = 279;	break;		// 원소 정제 체험 - E등급 원소 정제서 
			case 152 :	iOfferdIndex = 274;	break;		// 식물 가공 체험 - 크락의 노란잎 가공서
			case 154 :	iOfferdIndex = 397;	break;		// 무기 제작 체험 - 25레벨 무기제작서 
			default :	iOfferdIndex = -1;
			}
			if( iOfferdIndex > 0 )
				itemOfferd = gserver.m_itemProtoList.CreateItem( iOfferdIndex, -1, 0, 0, 1 );

			if( !itemOfferd )
				return;

			CNetMsg itemmsg;

			if( AddToInventory(ch, itemOfferd,true,true) )
			{
				if (itemOfferd->tab() < 0)
				{
					int r, c;
					if (ch->m_invenNormal.FindItem(&r, &c, iOfferdIndex, 0, 0))
					{
						delete itemOfferd;
						itemOfferd = ch->m_invenNormal.GetItem(r, c);
						ItemUpdateMsg( itemmsg, itemOfferd, itemOfferd->Count() );
						SEND_Q(itemmsg, ch->m_desc);
					}
				}
				else
				{
					ItemAddMsg(itemmsg, itemOfferd);
					SEND_Q(itemmsg, ch->m_desc);
				}
			}
			else
			{
				itemOfferd = ch->m_pArea->DropItem( itemOfferd, ch );

				if( !itemOfferd )
					return;

				// Drop Msg 보내기
				CNetMsg dropMsg;
				itemOfferd->m_preferenceIndex = ch->m_index;
				ItemDropMsg(dropMsg, ch, itemOfferd);
				ch->m_pArea->SendToCell(dropMsg, GET_YLAYER(itemOfferd), itemOfferd->m_cellX, itemOfferd->m_cellZ);
			}

			// Item LOG
			GAMELOG << init("ITEM_PICK_QUESTITEM", ch)
					<< itemlog(itemOfferd)
					<< end;	

#ifdef QUEST_DATA_EXTEND
			pQuest->SetComplete2(true);
#else
			ch->m_questList.SetComplete2(idx, true);
#endif // QUEST_DATA_EXTEND			
		}
		break;
#endif // EXPRIENCE_QUEST

	case QTYPE_KIND_REPEAT:
	case QTYPE_KIND_COLLECTION:
		{
			int bQuestItem,bNormalItem,bNpc;
			bQuestItem = bNormalItem = bNpc = 0;
#ifdef QUEST_DATA_EXTEND
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (pQuest->GetQuestProto()->m_conditionType[i] == -1)
					continue;
												
				if(pQuest->GetQuestProto()->m_conditionType[i] == QCONDITION_ITEM)
				{				
					bQuestItem++;
				}													
			}
			if(bQuestItem==0)
			{
				for (i=0; i < QUEST_MAX_CONDITION; i++)
				{
					if(pQuest->GetQuestProto()->m_conditionType[i] == QCONDITION_NPC)
					{												
						bNpc++;
					}				
				}
				
				if(bNpc==0)
				{
					pQuest->SetComplete1(true);
				}
				//ch->m_questList.SetComplete(idx, true);	
			}

			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if(pQuest->GetQuestProto()->m_conditionType[i] == QCONDITION_ITEM_NORMAL)
				{
					int r, c;
					if (ch->m_invenNormal.FindItem(&r, &c, pQuest->GetQuestProto()->m_conditionIndex[i], 0, 0))
					{
						CItem* p = ch->m_invenNormal.GetItem(r, c);
						if (p)
						{
							CheckQuestCondition(ch, p, p->Count());
						}
					}
					
					bNormalItem++;
				}				
			}
						
			if(bNormalItem==0)
			{
				pQuest->SetComplete2(true);
			}
#else // QUEST_DATA_EXTEND
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (pQuestProto->m_conditionType[i] == -1)
					continue;
												
				if(pQuestProto->m_conditionType[i] == QCONDITION_ITEM)
				{				
					bQuestItem++;
				}													
			}
			if(bQuestItem==0)
			{
				for (i=0; i < QUEST_MAX_CONDITION; i++)
				{
					if(pQuestProto->m_conditionType[i] == QCONDITION_NPC)
					{												
						bNpc++;
					}				
				}
				
				if(bNpc==0)
				{
					ch->m_questList.SetComplete(idx, true);
				}
				//ch->m_questList.SetComplete(idx, true);	
			}

			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if(pQuestProto->m_conditionType[i] == QCONDITION_ITEM_NORMAL)
				{
					int r, c;
					if (ch->m_invenNormal.FindItem(&r, &c, pQuestProto->m_conditionIndex[i], 0, 0))
					{
						CItem* p = ch->m_invenNormal.GetItem(r, c);
						if (p)
						{
							CheckQuestCondition(ch, p, p->Count());
						}
					}
					
					bNormalItem++;
				}				
			}
						
			if(bNormalItem==0)
			{
				ch->m_questList.SetComplete2(idx, true);
			}
#endif // QUEST_DATA_EXTEND
		}
		break;
	case QTYPE_KIND_DEFEAT :
	case QTYPE_KIND_SAVE :
#ifdef QUEST_DATA_EXTEND
		pQuest->SetComplete2(true);
#else // QUEST_DATA_EXTEND
		ch->m_questList.SetComplete2(idx, true);
#endif // QUEST_DATA_EXTEND
		break;
	default:
		break;
	}
}


void do_QuestGiveUp(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype;
	int questIndex;
	CNetMsg rMsg;

	msg.MoveFirst();

	msg >> subtype
		>> questIndex;

	CQuest* pQuest;

#ifdef QUEST_DATA_EXTEND

	pQuest = ch->m_questList.FindQuest(questIndex);

	// 없는 퀘스트 
	if (pQuest == NULL)
	{
		CQuestProto* proto = gserver.m_questProtoList.FindProto(questIndex);

		if(!proto)
			return;

		if(proto->m_type[1] == QTYPE_REPEAT_ONCE)
		{
			// Quest GiveUp Log
			GAMELOG << init("QUEST GIVEUP", ch)
					<< questIndex
					<< end;

			pQuest = ch->m_questList.AddQuest(questIndex);
			if (pQuest)
				ch->m_questList.SetQuestState(pQuest, QUEST_STATE_ABANDON);
		}
		
		QuestGiveUpMsg(rMsg, questIndex);
		
		SEND_Q(rMsg, ch->m_desc);
		
		return;
	}

	if (pQuest->GetQuestState() != QUEST_STATE_RUN)
		return;

	const CQuestProto* pQuestProto = pQuest->GetQuestProto();

#else // QUEST_DATA_EXTEND

	int idx = ch->m_questList.FindQuest(questIndex);

	// 없는 퀘스트 
	if (idx < 0)
	{
		//if(questIndex < 0)
		//	return;

		CQuestProto* proto = gserver.m_questProtoList.FindProto(questIndex);

		if(!proto)
			return;

		if(proto->m_type[1] == QTYPE_REPEAT_ONCE)
		{
			// Quest GiveUp Log
			GAMELOG << init("QUEST GIVEUP", ch)
				<< questIndex
				<< end;
			
			
			for (int i=0; i < QUEST_MAX_PC_COMPLETE; i++)
			{
				// 비어 있는 곳에 순서대로..
				if (ch->m_questList.m_abandonQuest[i] == -1)
				{
					ch->m_questList.m_abandonQuest[i] = proto->m_index;
					break;
				}
			}
			
		}
		
		QuestGiveUpMsg(rMsg, questIndex);
		
		SEND_Q(rMsg, ch->m_desc);
		
		return;
	}

	pQuest = ch->m_questList.m_list[idx];

	if (!pQuest)
		return;

	const CQuestProto* pQuestProto = pQuest->m_proto;

#endif // QUEST_DATA_EXTEND

	int i;

	switch (pQuestProto->m_type[0])
	{
		// 수집, 전달, 구출 퀘스트
	case QTYPE_KIND_COLLECTION:
	case QTYPE_KIND_DELIVERY:
	case QTYPE_KIND_SAVE:
	case QTYPE_KIND_MINING_EXPERIENCE:
	case QTYPE_KIND_GATHERING_EXPERIENCE:
	case QTYPE_KIND_CHARGE_EXPERIENCE:
#ifdef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae] Giveup
	case QTYPE_KIND_PROCESS_EXPERIENCE :		
	case QTYPE_KIND_MAKE_EXPERIENCE :	
#endif // EXPRIENCE_QUEST
		{
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (pQuestProto->m_conditionType[i] == -1)
					continue;

				switch (pQuestProto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						// 퀘스트 인벤토리에서만
						int r, c;
						if (!ch->m_invenQuest.FindItem(&r, &c, pQuestProto->m_conditionIndex[i], 0, 0))
							continue;
						
						CItem* item = ch->m_invenQuest.GetItem(r, c);
						if (!item)
							continue;

#ifdef QUEST_DATA_EXTEND
						int nValue = pQuest->GetQuestValue(i);
#else // QUEST_DATA_EXTEND
						int nValue = pQuest->m_currentData[i];
#endif // QUEST_DATA_EXTEND
						// item 수량 변경
						CNetMsg itemmsg;
						DecreaseFromInventory(ch, item, nValue);
						
						if (item->Count() > 0)
						{
							ItemUpdateMsg(itemmsg, item, -nValue);
							SEND_Q(itemmsg, ch->m_desc);
						}
						else
						{
							ItemDeleteMsg(itemmsg, item);
							SEND_Q(itemmsg, ch->m_desc);
							ch->RemoveItemFromQuickSlot(item);
							RemoveFromInventory(ch, item, true, true);
						}
					}
					break;
				default:
					break;
				}
			}
		}
		break;

	default:
		break;
	}

	bool bGoZone = false;

	if (ch->m_pZone->IsPersonalDungeon() && (pQuestProto->m_type[0] == QTYPE_KIND_DEFEAT || pQuestProto->m_type[0] == QTYPE_KIND_SAVE || pQuestProto->m_type[0] == QTYPE_KIND_TUTORIAL))
	{
		bGoZone = true;
	}

	// Quest GiveUp Log
	GAMELOG << init("QUEST GIVEUP", ch)
			<< pQuestProto->m_index
			<< end;

	// 여기서 포기한 퀘스트 인덱스를 저장하여 다음에는 받지 못하도록 한다.
	// 포기한 퀘스트를 저장한다.
#ifdef QUEST_DATA_EXTEND
	if (pQuestProto->m_type[1] == QTYPE_REPEAT_ONCE)
	{
		ch->m_questList.SetQuestState(pQuest, QUEST_STATE_ABANDON);
	}
#else // QUEST_DATA_EXTEND
	if (pQuestProto->m_type[1] == QTYPE_REPEAT_ONCE)
	{
		for (i=0; i < QUEST_MAX_PC_COMPLETE; i++)
		{
			// 비어 있는 곳에 순서대로..
			if (ch->m_questList.m_abandonQuest[i] == -1)
			{
				ch->m_questList.m_abandonQuest[i] = pQuestProto->m_index;
				break;
			}
		}
	}
#endif // QUEST_DATA_EXTEND

	QuestGiveUpMsg(rMsg, pQuest);
	ch->m_questList.DelQuest(ch, pQuest);
	SEND_Q(rMsg, ch->m_desc);

	// 격파, 구출 테스트의 경우 존이동
	if (bGoZone)
	{
		// 가까운 마을로
		int nearZone;
		int nearZonePos;
		int i = gserver.FindNearestZone(ch->m_pZone->m_index, GET_X(ch), GET_Z(ch), &nearZone, &nearZonePos);
		if (i == -1)
			return ;
		
		CZone* pZone = gserver.m_zones + i;
		
		GoZone(ch, nearZone,
			pZone->m_zonePos[nearZonePos][0],															// ylayer
			GetRandom(pZone->m_zonePos[nearZonePos][1], pZone->m_zonePos[nearZonePos][3]) / 2.0f,		// x
			GetRandom(pZone->m_zonePos[nearZonePos][2], pZone->m_zonePos[nearZonePos][4]) / 2.0f);		// z
	}

	// 포기했으면 싱글던전 입장권 받을 기회 줄인다. 최소 0
#ifdef QUEST_DATA_EXTEND
	ch->m_questList.DecreaseQuestComepleteCount();
#else // QUEST_DATA_EXTEND
	ch->m_questList.m_nComplete--;

	if (ch->m_questList.m_nComplete < 1)
		ch->m_questList.m_nComplete = 0;
#endif // QUEST_DATA_EXTEND
}

void do_QuestPrize(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype;
	int questIndex, npcIndex;

	CNetMsg expSPMsg;

	msg.MoveFirst();

	msg >> subtype
		>> questIndex
		>> npcIndex;

#ifdef QUEST_DATA_EXTEND

	CQuest* pQuest = ch->m_questList.FindQuest(questIndex);
	if (pQuest == NULL || pQuest->GetQuestState() != QUEST_STATE_RUN)
		return ;

	if (!pQuest->IsCompleted())
		return ;

	const CQuestProto* pQuestProto = pQuest->GetQuestProto();

#else // QUEST_DATA_EXTEND

	int idx = ch->m_questList.FindQuest(questIndex);

	// 없는 퀘스트
	if (idx < 0)
		return;

	CQuest* pQuest = ch->m_questList.m_list[idx];

	if (!pQuest)
		return;

	// pQuest complete check
	if (!ch->m_questList.IsCompleted(idx))
		return;

	const CQuestProto* pQuestProto = pQuest->m_proto;

#endif // QUEST_DATA_EXTEND

	// pQuest 보상을 받을 npc가 맞는지
	if (pQuestProto->m_prizeNPC != npcIndex)
		return;

	CNPC* npc = ch->m_pArea->FindProtoNPCInCell(ch, pQuestProto->m_prizeNPC, false, 2);
	if( !npc )
		return;

	int i;
	CNetMsg rMsg;

	switch (pQuestProto->m_type[0])
	{
		// 전달 퀘스트
		// 수집 퀘스트
		// 구출 퀘스트
	case QTYPE_KIND_COLLECTION:
	case QTYPE_KIND_DELIVERY:
	case QTYPE_KIND_SAVE:
	case QTYPE_KIND_MINING_EXPERIENCE:
	case QTYPE_KIND_GATHERING_EXPERIENCE:
	case QTYPE_KIND_CHARGE_EXPERIENCE:
#ifdef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae] 보상
	case QTYPE_KIND_PROCESS_EXPERIENCE :
	case QTYPE_KIND_MAKE_EXPERIENCE :		
#endif // EXPRIENCE_QUEST
		{
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				// 아예 없으면 
				if (pQuestProto->m_conditionType[i] == -1)
					continue;
				
				switch (pQuestProto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						// 전달 퀘스트 아이템은 퀘스트 인벤에서만 !!!
						int r, c;
						if (!ch->m_invenQuest.FindItem(&r, &c, pQuestProto->m_conditionIndex[i], 0, 0))
						{							

							// Quest FAIL Log
							GAMELOG << init("QUEST FAIL", ch)
								<< pQuestProto->m_index
								<< end;
							
							QuestFailMsg(rMsg, pQuest);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rMsg, ch->m_desc);
							return;
						}
						
						// 찾았으면 수량 확인 해서 줄이기
						CItem* item = ch->m_invenQuest.GetItem(r, c);
						
						if (!item)
						{
							// Quest FAIL Log
							GAMELOG << init("QUEST FAIL", ch)
								<< pQuestProto->m_index
								<< end;
							
							QuestFailMsg(rMsg, pQuest);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rMsg, ch->m_desc);
							return;
						}
						
						// 필요 숫자 만큼 없음
						if (item->Count() < pQuestProto->m_conditionNum[i])
						{
							// Quest FAIL Log
							GAMELOG << init("QUEST FAIL", ch)
								<< pQuestProto->m_index
								<< end;
							
							QuestFailMsg(rMsg, pQuest);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rMsg, ch->m_desc);
							return;
						}
						CNetMsg itemMsg;
						
						// Item 수량 변경
						DecreaseFromInventory(ch, item, pQuestProto->m_conditionNum[i]);
						
						if (item->Count() > 0)
						{
							ItemUpdateMsg(itemMsg, item, -pQuestProto->m_conditionNum[i]);
							SEND_Q(itemMsg, ch->m_desc);
						}
						else
						{
							ItemDeleteMsg(itemMsg, item);
							SEND_Q(itemMsg, ch->m_desc);
							ch->RemoveItemFromQuickSlot(item);
							RemoveFromInventory(ch, item, true, true);
						}
					}
					break;

				case QCONDITION_ITEM_NORMAL:
					{
						// 펫 아이템 체크.. (펫을 가지고 있다면 보상을 받지 않고 무시한다. return
						if( ch->m_petList )
						{
							int petCount = 0;
							CPet* p = ch->m_petList;
							while (p)
							{
								petCount++;
								p = p->m_nextPet;
							}

							if( (pQuest->IsPetQuest() ) && petCount >= MAX_OWN_PET )
							{
								CNetMsg rmsg;
								QuestErrorMsg(rmsg, MSG_QUEST_ERR_PET_NOT_HAVE_ONEMORE);
								SEND_Q(rmsg, ch->m_desc);
								return;
							}
						}

						//일반아이템 체크.
						int r, c;
						if (!ch->m_invenNormal.FindItem(&r, &c, pQuestProto->m_conditionIndex[i], 0, 0))
						{
							// Quest FAIL Log
							GAMELOG << init("QUEST FAIL", ch)
								<< pQuestProto->m_index
								<< end;

							QuestFailMsg(rMsg, pQuest);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rMsg, ch->m_desc);
							return;
						}
						// 찾았으면 수량 확인 해서 줄이기
						CItem* item = ch->m_invenNormal.GetItem(r, c);
						
						if (!item)
						{
							// Quest FAIL Log
							GAMELOG << init("QUEST FAIL", ch)
								<< pQuestProto->m_index
								<< end;
							
							QuestFailMsg(rMsg, pQuest);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rMsg, ch->m_desc);
							return;
						}
						
						// 필요 숫자 만큼 없음
						if (item->Count() < pQuestProto->m_conditionNum[i])
						{
							// Quest FAIL Log
							GAMELOG << init("QUEST FAIL", ch)
								<< pQuestProto->m_index
								<< end;
							
							QuestFailMsg(rMsg, pQuest);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rMsg, ch->m_desc);
							return;
						}
						CNetMsg itemMsg;
						
						// Item 수량 변경
						DecreaseFromInventory(ch, item, pQuestProto->m_conditionNum[i]);
						
						if (item->Count() > 0)
						{
							ItemUpdateMsg(itemMsg, item, -pQuestProto->m_conditionNum[i]);
							SEND_Q(itemMsg, ch->m_desc);
						}
						else
						{
							ItemDeleteMsg(itemMsg, item);
							SEND_Q(itemMsg, ch->m_desc);
							ch->RemoveItemFromQuickSlot(item);
							RemoveFromInventory(ch, item, true, true);
						}
						
					}
					break;

				default:
					break;
				}
			}
		}
		break;

	default:
		break;
	}

	for (i=0; i < QUEST_MAX_PRIZE; i++)
	{
		if (pQuestProto->m_prizeType[i] == -1)
			continue;

		switch (pQuestProto->m_prizeType[i])
		{
		case QPRIZE_ITEM:
			// 보상 아이템 주기
			{
				CItem* item = NULL;
				item = gserver.m_itemProtoList.CreateItem(pQuestProto->m_prizeIndex[i], -1, 0, 0, pQuestProto->m_prizeData[i]);

				if (!item)
					continue;
				
				if(item->IsAccessary() && item->m_nOption < 1)
				{
					OptionSettingItem(ch, item);
				}

				CNetMsg itemmsg;

				// 들어갈 인벤토리 결정
				CInventory* inven = GET_INVENTORY(ch, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
				if (!inven)
					continue ;
#ifdef ATTACK_PET
				if( item->IsAPet() )
				{
					CNetMsg rmsg;

					if( ch->m_invenNormal.GetSpace() < 1 )
					{
						GAMELOG << init("QUEST ERROR", ch)
								<< pQuestProto->m_index << delim
								<< (int)MSG_QUEST_ERR_PRIZE_FULL
								<< end;

						QuestErrorMsg(rmsg, MSG_QUEST_ERR_DELIVERY_FULL);
						SEND_Q( rmsg, ch->m_desc);
						continue;	//실폐
					}
					
					HelperAttackPetMsg( rmsg, MSG_HELPER_APET_CREATE_REQ, ch->m_index );
					rmsg << item->m_itemProto->m_num0;
					SEND_Q( rmsg, gserver.m_helper );
					break;
				}
#endif // ATTACK_PET
				bool bCountable = false;
				// 인벤에 넣기
				if (AddToInventory(ch, item, true, true))
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
						if (item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && ch->m_moneyItem == NULL)
							ch->m_moneyItem = item;
					}
					// 메시지 보내고
					SEND_Q(itemmsg, ch->m_desc);
				}
				else
				{
					// 인젠토리 꽉차서 못 받을 때
					// Quest Error Log
					GAMELOG << init("QUEST ERROR", ch)
						<< pQuestProto->m_index << delim
						<< (int)MSG_QUEST_ERR_PRIZE_FULL
						<< end;

					// 펫 아이템일 경우 꽉 차면 드롭시키지 않고 피리를 다시 집어 넣고 컨티뉴
					if( item->IsPet() )
					{
						int egg_index = 0;

						switch( item->m_itemProto->m_index )
						{
							case PET_HORSE_ITEM_INDEX:
								egg_index =	PET_HORSE_EGG_INDEX;
								break;
							case PET_DRAGON_ITEM_INDEX:
								egg_index =	PET_DRAGON_EGG_INDEX;
								break;
							case PET_BLUE_HORSE_ITEM_INDEX:
								egg_index =	PET_BLUE_HORSE_EGG_INDEX;
								break;
							case PET_PINK_DRAGON_ITEM_INDEX:
								egg_index =	PET_PINK_DRAGON_EGG_INDEX;
								break;
							case PET_UNKOWN_HORSE_ITEM_INDEX:
								egg_index =	PET_UNKOWN_HORSE_EGG_INDEX;
								break;
							case PET_UNKOWN_DRAGON_ITEM_INDEX:
								egg_index =	PET_UNKOWN_DRAGON_EGG_INDEX;
								break;
							default :
								continue;
						}

						CNetMsg rmsg;
						//return;
						
						// Item 수량 변경
						DecreaseFromInventory(ch, item, pQuestProto->m_prizeData[i]);
						
						if (item->Count() > 0)
						{
							ItemUpdateMsg(rmsg, item, -pQuestProto->m_prizeData[i]);
							SEND_Q(rmsg, ch->m_desc);
						}
						else
						{
							ItemDeleteMsg(rmsg, item);
							SEND_Q(rmsg, ch->m_desc);
							ch->RemoveItemFromQuickSlot(item);
							RemoveFromInventory(ch, item, true, true);
						}
						ch->GiveItem(egg_index, 0, 0, 1, true);
						QuestErrorMsg(rmsg, MSG_QUEST_ERR_DELIVERY_FULL);
						SEND_Q( rmsg, ch->m_desc);

						continue;
					}
					// 인벤토리 가득참
					item = ch->m_pArea->DropItem(item, ch);
					if (!item)
						continue;
					
					CNetMsg dropMsg;
					item->m_preferenceIndex = ch->m_index;
					ItemDropMsg(dropMsg, ch, item);
					ch->m_pArea->SendToCell(dropMsg, ch, true);

					QuestErrorMsg(rMsg, MSG_QUEST_ERR_PRIZE_FULL);
					SEND_Q(rMsg, ch->m_desc);
				}
				// Item LOG
				GAMELOG << init("ITEM_PICK_QUESTPRIZE", ch)
						<< itemlog(item)
						<< end;

				// Quest Prize Log
				GAMELOG << init("QUEST PRIZE", ch)
						<< pQuestProto->m_index << delim
						<< QPRIZE_ITEM << delim
						<< pQuestProto->m_prizeIndex[i] << delim
						<< pQuestProto->m_prizeData[i]
						<< end;

				if (bCountable)
					delete item;

				// 펫 퀘스트 일 경우 헬퍼에 생성 요청 : 할일
				if ( pQuest->IsPetQuest() )  
				{
					CNetMsg rmsg;
					if( IS_RUNNING_HELPER )
					{
						switch( item->m_itemProto->m_index )
						{
							case PET_HORSE_ITEM_INDEX:
								HelperPetCreateReqMsg(rmsg, ch->m_index, (PET_TYPE_HORSE | PET_GRADE_CHILD) );
								break;
							case PET_DRAGON_ITEM_INDEX:
								HelperPetCreateReqMsg(rmsg, ch->m_index, (PET_TYPE_DRAGON | PET_GRADE_CHILD) );
								break;
							case PET_BLUE_HORSE_ITEM_INDEX:
								HelperPetCreateReqMsg(rmsg, ch->m_index, (PET_TYPE_BLUE_HORSE | PET_GRADE_CHILD) );
								break;
							case PET_PINK_DRAGON_ITEM_INDEX:
								HelperPetCreateReqMsg(rmsg, ch->m_index, (PET_TYPE_PINK_DRAGON | PET_GRADE_CHILD) );
								break;
							case PET_UNKOWN_HORSE_ITEM_INDEX:
								HelperPetCreateReqMsg(rmsg, ch->m_index, (PET_TYPE_UNKOWN_HORSE | PET_GRADE_CHILD) );
								break;
							case PET_UNKOWN_DRAGON_ITEM_INDEX:
								HelperPetCreateReqMsg(rmsg, ch->m_index, (PET_TYPE_UNKOWN_DRAGON | PET_GRADE_CHILD) );
								break;
							default :
								continue;
						}

						SEND_Q(rmsg, gserver.m_helper);
						ch->m_desc->WaitHelperReply(true, 21);
					}
					else
					{
						switch( item->m_itemProto->m_index )
						{
							case PET_HORSE_ITEM_INDEX:
								ch->GiveItem(PET_HORSE_EGG_INDEX, 0,0, 1, true);
								break;
							case PET_DRAGON_ITEM_INDEX:
								ch->GiveItem(PET_DRAGON_EGG_INDEX, 0,0, 1, true);
								break;
							case PET_BLUE_HORSE_ITEM_INDEX:
								ch->GiveItem(PET_BLUE_HORSE_EGG_INDEX, 0,0, 1, true);
								break;
							case PET_PINK_DRAGON_ITEM_INDEX:
								ch->GiveItem(PET_PINK_DRAGON_EGG_INDEX, 0,0, 1, true);
								break;
							case PET_UNKOWN_HORSE_ITEM_INDEX:
								ch->GiveItem(PET_UNKOWN_HORSE_EGG_INDEX, 0,0, 1, true);
								break;
							case PET_UNKOWN_DRAGON_ITEM_INDEX:
								ch->GiveItem(PET_UNKOWN_DRAGON_EGG_INDEX, 0,0, 1, true);
								break;
							default :
								continue;
						}

						if( item )
						{
							CNetMsg rmsg;
							DecreaseFromInventory(ch, item, 1);
						
							if (item->Count() > 0)
							{
								ItemUpdateMsg(rmsg, item, -1);
								SEND_Q(rmsg, ch->m_desc);
							}
							else
							{
								ItemDeleteMsg(rmsg, item);
								SEND_Q(rmsg, ch->m_desc);
								ch->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(ch, item, true, true);
							}
						}

						FailMsg(rmsg, MSG_FAIL_SYSTEM_ERROR);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}
				}
			}

			break;

		case QPRIZE_MONEY:
			{
				bool bHaveMoney = false;

				if (ch->m_moneyItem)
					bHaveMoney = true;

				if (!ch->AddMoney(pQuestProto->m_prizeData[i]))
				{
					// 인젠토리 꽉차서 못 받을 때
					// Quest Error Log
					GAMELOG << init("QUEST ERROR", ch)
							<< pQuestProto->m_index << delim
							<< (int)MSG_QUEST_ERR_PRIZE_FULL
							<< end;

					CItem* item = NULL;
					item = gserver.m_itemProtoList.CreateItem(pQuestProto->m_prizeIndex[i], -1, 0, 0, pQuestProto->m_prizeData[i]);

					item = ch->m_pArea->DropItem(item, ch);
					if (!item)
						continue;
					
					CNetMsg dropMsg;
					item->m_preferenceIndex = ch->m_index;
					ItemDropMsg(dropMsg, ch, item);
					ch->m_pArea->SendToCell(dropMsg, ch, true);

					// 인벤토리 가득참
					QuestErrorMsg(rMsg, MSG_QUEST_ERR_PRIZE_FULL);
					SEND_Q(rMsg, ch->m_desc);

					GAMELOG << init("ITEM_DROP_QUESTPRIZE", ch)
							<< itemlog(item)
							<< end;
				}
				else
				{
					if (!ch->m_moneyItem)
						continue;

					CNetMsg itemmsg;
					if (bHaveMoney)
						ItemUpdateMsg(itemmsg, ch->m_moneyItem, pQuestProto->m_prizeData[i]);
					else
						ItemAddMsg(itemmsg, ch->m_moneyItem);

					// 메시지 보내고
					SEND_Q(itemmsg, ch->m_desc);
					// Item LOG
					GAMELOG << init("ITEM_PICK_QUESTPRIZE", ch)
							<< itemlog(ch->m_moneyItem)
							<< end;
				}
				
				// Quest Prize Log
				GAMELOG << init("QUEST PRIZE", ch)
						<< pQuestProto->m_index << delim
						<< QPRIZE_MONEY << delim
						<< pQuestProto->m_prizeIndex[i] << delim
						<< pQuestProto->m_prizeData[i]
						<< end;
			}
			break;
			
		case QPRIZE_EXP:
			// 보상 경험치 주기
			ch->AddExpSP((LONGLONG)pQuestProto->m_prizeData[i], 0, false);
			
			// Quest Prize Log
			GAMELOG << init("QUEST PRIZE", ch)
					<< pQuestProto->m_index << delim
					<< QPRIZE_EXP << delim
					<< pQuestProto->m_prizeIndex[i] << delim
					<< pQuestProto->m_prizeData[i]
					<< end;
			break;
			
		case QPRIZE_SP:
			// 보상 SP 주기
			ch->AddExpSP((LONGLONG)0, pQuestProto->m_prizeData[i] * 10000, false);

			GAMELOG << init("QUEST PRIZE", ch)
					<< pQuestProto->m_index << delim
					<< QPRIZE_SP << delim
					<< pQuestProto->m_prizeIndex[i] << delim
					<< pQuestProto->m_prizeData[i]
					<< end;
			break;

		case QPRIZE_SKILL:
			break;

		case QPRIZE_SSKILL:
			{
				// sskill index : pQuestProto->m_prizeIndex[i]
				// sskill level : pQuestProto->m_prizeData[i]
				CSSkillProto* proto = gserver.m_sSkillProtoList.FindProto(pQuestProto->m_prizeIndex[i]);
				if (!proto)
					continue;

				CSSkill* sskill = ch->m_sSkillList.Find(proto->m_index);

				// 이미 배운 스킬 이라면 Skip : 설마 보상으로 레벨2짜리를 줄까?
#ifndef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae]
				//  Level 2스킬 보상으로 인한 ifndef 처리
				if (sskill)
					continue;
#endif // EXPRIENCE_QUEST

				sskill = new CSSkill(proto, pQuestProto->m_prizeData[i]);
				if (!sskill)
					continue;

				bool bAdd = true;		// 추가하는가?

				CSSkillNode* node;
				CSSkillNode* nodeNext = ch->m_sSkillList.m_head;
				while ((node = nodeNext))
				{
					nodeNext = nodeNext->m_next;
					
					// 타입이 같고
// 					if (sskill->m_proto->m_preference != -1 && node->m_sskill->m_proto->m_type == sskill->m_proto->m_type)
// 					{
// 						//우선순위가 높은 특수스킬일 가지고 있다면 ?// 						if (node->m_sskill->m_proto->m_preference >= sskill->m_proto->m_preference)
// 						{
// 							bAdd = false;
// 						}
// 						else if (node->m_sskill->m_proto->m_preference < sskill->m_proto->m_preference)
// 						{					
// 							// 스킬 삭제
// 							ch->m_sSkillList.Remove(node->m_sskill);
// 						}
//					}

#ifdef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae] 같은 스킬 검사
//					else 
					if ( (sskill->m_proto->m_index == node->m_sskill->m_proto->m_index) )
					{
						if (sskill->m_level > node->m_sskill->m_level)	// 습득 레벨 보다 높다.
							node->m_sskill->m_level = sskill->m_level;
						bAdd = false;
					}
#endif // EXPRIENCE_QUEST
				}

				if (bAdd)
				{
					if (!ch->m_sSkillList.Add(sskill))
						continue;

					// 스킬 배움 메시지 전송
					CNetMsg msg;
					SSkillLearnMsg(msg, sskill);
					SEND_Q(msg, ch->m_desc);
					
					GAMELOG << init("QUEST PRIZE", ch)
						<< pQuestProto->m_index << delim
						<< QPRIZE_SSKILL << delim
						<< pQuestProto->m_prizeIndex[i] << delim
						<< pQuestProto->m_prizeData[i]
						<< end;
				}
				else
				{
					if (sskill)
						delete sskill;
				}

			}
			break;

		default:
			continue;
		}
	}

		// 추가 보상 아이템 
	int optItemIndex;
	int plus;
	msg >> optItemIndex;				
	msg >> plus;

	if(optItemIndex != -1)
	{					
		//추가 아이템은 하나뿐이다.
		bool bFind = FALSE;
		bool bFoundSameType = FALSE;
		CItemProto* pItemProtoNeed = gserver.m_itemProtoList.FindIndex(optItemIndex);
		
		for (i=0; i < QUEST_MAX_PRIZE; i++)
		{
			if (pQuestProto->m_optPrizeType[i] == -1)
			{
				continue;
			}
			else if(pQuestProto->m_optPrizeIndex[i] == optItemIndex)
			{
				bFind = TRUE;
				bFoundSameType = TRUE;
				break;
			}
			else if (pItemProtoNeed)
			{
				CItemProto* pItemProtoFind = gserver.m_itemProtoList.FindIndex(pQuestProto->m_optPrizeIndex[i]);
				
				if (   pItemProtoFind
					&& pItemProtoFind->m_typeIdx == pItemProtoNeed->m_typeIdx
					&& pItemProtoFind->m_subtypeIdx == pItemProtoNeed->m_subtypeIdx
					&& pItemProtoFind->GetItemProtoLevel() == pItemProtoNeed->GetItemProtoLevel())
				{
					bFoundSameType = TRUE;
					//break;
				}
			}
		}
		if(!bFind)
		{
			CItemProto* proto = gserver.m_itemProtoList.FindIndex(optItemIndex);
			
			if(bFoundSameType && proto && proto->m_typeIdx == ITYPE_WEAR)
			{
				
				CItem* item = NULL;
				//	item = gserver.m_itemProtoList.CreateItem(pQuestProto->m_optPrizeIndex[i], -1, 0, 0, pQuestProto->m_optPrizeData[i]);
				//	item = gserver.m_itemProtoList.CreateItem(optItemIndex, -1, 0, 0, pQuestProto->m_optPrizeData[i]);
				
				if(plus < 0 || plus > 4) // +4이상의 아이템을 보상으로 주진 않는다. //  [2/28/2007 KwonYongDae]
				{
					plus = 0;
				}
				
				item = gserver.m_itemProtoList.CreateItem(optItemIndex, -1, plus, 0, 1);	// 방어구라서 count는 무조건 1로 함
				
				if (!item)
				{
					QuestErrorMsg(rMsg, MSG_QUEST_ERR_PRZIE_ITEM);
					SEND_Q(rMsg, ch->m_desc);
					return;
				}
				
				if(item->IsAccessary() && item->m_nOption < 1)
				{
					OptionSettingItem(ch, item);
				}

				CNetMsg itemmsg;
				
				// 들어갈 인벤토리 결정
				CInventory* inven = GET_INVENTORY(ch, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
				if (!inven)
				{
					QuestErrorMsg(rMsg, MSG_QUEST_ERR_PRZIE_ITEM);
					SEND_Q(rMsg, ch->m_desc);
					return;
				}
				
				bool bCountable = false;
				// 인벤에 넣기
				if (AddToInventory(ch, item, true, true))
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
						if (item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && ch->m_moneyItem == NULL)
							ch->m_moneyItem = item;
					}
				}
				else
				{
					// 인젠토리 꽉차서 못 받을 때
					// Quest Error Log
					GAMELOG << init("QUEST ERROR", ch)
							<< pQuestProto->m_index << delim
							<< (int)MSG_QUEST_ERR_PRIZE_FULL
							<< end;
					
					// 인벤토리 가득참
					item = ch->m_pArea->DropItem(item, ch);
					if (!item)
						return;
					
					CNetMsg dropMsg;
					item->m_preferenceIndex = ch->m_index;
					ItemDropMsg(dropMsg, ch, item);
					ch->m_pArea->SendToCell(dropMsg, ch, true);
					
					QuestErrorMsg(rMsg, MSG_QUEST_ERR_PRIZE_FULL);
					SEND_Q(rMsg, ch->m_desc);
					//return;
				}
				
				// 메시지 보내고
				SEND_Q(itemmsg, ch->m_desc);
				
				// Item LOG
				GAMELOG << init("ITEM_PICK_QUESTPRIZE", ch)
						<< itemlog(item)
						<< end;
				
// Quest Prize Log //  [2/19/2009 KwonYongDae] i 사용할 수 없음
//				GAMELOG << init("QUEST PRIZE", ch)
//					<< pQuestProto->m_index << delim
//					<< QPRIZE_ITEM << delim
//					<< pQuestProto->m_optPrizeIndex[i] << delim
//					<< pQuestProto->m_optPrizeData[i]
//					<< end;
				
				if (bCountable)
					delete item;
				
				
			}
			else //없는 아이템이다.
			{
				//return;
			}
		}
		else
		{
			if (pQuestProto->m_optPrizeType[i] == -1)
				return;
			
			switch (pQuestProto->m_optPrizeType[i])
			{
			case QPRIZE_ITEM:
				// 추가 보상 아이템 주기
				{
					CItem* item = NULL;
					//	item = gserver.m_itemProtoList.CreateItem(pQuestProto->m_optPrizeIndex[i], -1, 0, 0, pQuestProto->m_optPrizeData[i]);
					//	item = gserver.m_itemProtoList.CreateItem(optItemIndex, -1, 0, 0, pQuestProto->m_optPrizeData[i]);
					
					if(plus < 0 || plus > 4) // +4 이상의 아이템을 보상으로 주진 않는다.
					{
						plus = 0;
					}
					
					item = gserver.m_itemProtoList.CreateItem(optItemIndex, -1, plus, 0, pQuestProto->m_optPrizeData[i]);
					
					if (!item)
					{
						QuestErrorMsg(rMsg, MSG_QUEST_ERR_PRZIE_ITEM);
						SEND_Q(rMsg, ch->m_desc);
						return;
					}
					
					if(item->IsAccessary() && item->m_nOption < 1)
					{
						OptionSettingItem(ch, item);
					}
					
					CNetMsg itemmsg;
					
					// 들어갈 인벤토리 결정
					CInventory* inven = GET_INVENTORY(ch, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
					if (!inven)
					{
						QuestErrorMsg(rMsg, MSG_QUEST_ERR_PRZIE_ITEM);
						SEND_Q(rMsg, ch->m_desc);
						return;
					}
					
					bool bCountable = false;
					// 인벤에 넣기
					if (AddToInventory(ch, item, true, true))
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
							if (item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && ch->m_moneyItem == NULL)
								ch->m_moneyItem = item;
						}
					}
					else
					{
						// 인젠토리 꽉차서 못 받을 때
						// Quest Error Log
						GAMELOG << init("QUEST ERROR", ch)
							<< pQuestProto->m_index << delim
							<< (int)MSG_QUEST_ERR_PRIZE_FULL
							<< end;
						
						// 인벤토리 가득참
						item = ch->m_pArea->DropItem(item, ch);
						if (!item)
							return;
						
						CNetMsg dropMsg;
						item->m_preferenceIndex = ch->m_index;
						ItemDropMsg(dropMsg, ch, item);
						ch->m_pArea->SendToCell(dropMsg, ch, true);
						
						QuestErrorMsg(rMsg, MSG_QUEST_ERR_PRIZE_FULL);
						SEND_Q(rMsg, ch->m_desc);
					}
					
					// 메시지 보내고
					SEND_Q(itemmsg, ch->m_desc);
					
					// Item LOG
					GAMELOG << init("ITEM_PICK_QUESTPRIZE", ch)
							<< itemlog(item)
							<< end;
					
					// Quest Prize Log
					GAMELOG << init("QUEST PRIZE", ch)
						<< pQuestProto->m_index << delim
						<< QPRIZE_ITEM << delim
						<< pQuestProto->m_optPrizeIndex[i] << delim
						<< pQuestProto->m_optPrizeData[i]
						<< end;
					
					if (bCountable)
						delete item;
				}
				
				break;
			case QPRIZE_MONEY:
				{
					bool bHaveMoney = false;
					
					if (ch->m_moneyItem)
						bHaveMoney = true;
					
					if (!ch->AddMoney(pQuestProto->m_prizeData[i]))
					{
						// 인젠토리 꽉차서 못 받을 때
						// Quest Error Log
						GAMELOG << init("QUEST ERROR", ch)
							<< pQuestProto->m_index << delim
							<< (int)MSG_QUEST_ERR_PRIZE_FULL
							<< end;

						CItem* item = NULL;
						item = gserver.m_itemProtoList.CreateItem(pQuestProto->m_prizeIndex[i], -1, 0, 0, pQuestProto->m_prizeData[i]);

						item = ch->m_pArea->DropItem(item, ch);
						if (!item)
							return;
						
						CNetMsg dropMsg;
						item->m_preferenceIndex = ch->m_index;
						ItemDropMsg(dropMsg, ch, item);
						ch->m_pArea->SendToCell(dropMsg, ch, true);

						// 인벤토리 가득참
						QuestErrorMsg(rMsg, MSG_QUEST_ERR_PRIZE_FULL);
						SEND_Q(rMsg, ch->m_desc);

						GAMELOG << init("ITEM_DROP_QUESTPRIZE", ch)
								<< itemlog(item)
								<< end;
					}
					else
					{
						if (!ch->m_moneyItem)
							return;
						
						CNetMsg itemmsg;
						if (bHaveMoney)
							ItemUpdateMsg(itemmsg, ch->m_moneyItem, pQuestProto->m_prizeData[i]);
						else
							ItemAddMsg(itemmsg, ch->m_moneyItem);
						
						// 메시지 보내고
						SEND_Q(itemmsg, ch->m_desc);
						// Item LOG
						GAMELOG << init("ITEM_PICK_QUESTPRIZE", ch)
								<< itemlog(ch->m_moneyItem)
								<< end;
					}
					
					// Quest Prize Log
					GAMELOG << init("QUEST PRIZE", ch)
						<< pQuestProto->m_index << delim
						<< QPRIZE_MONEY << delim
						<< pQuestProto->m_prizeIndex[i] << delim
						<< pQuestProto->m_prizeData[i]
						<< end;
				}
				break;
				
			case QPRIZE_EXP:
				// 보상 경험치 주기
				ch->AddExpSP((LONGLONG)pQuestProto->m_prizeData[i], 0, false);
				
				// Quest Prize Log
				GAMELOG << init("QUEST PRIZE", ch)
					<< pQuestProto->m_index << delim
					<< QPRIZE_EXP << delim
					<< pQuestProto->m_prizeIndex[i] << delim
					<< pQuestProto->m_prizeData[i]
					<< end;
				break;
				
			case QPRIZE_SP:
				// 보상 SP 주기
				ch->AddExpSP((LONGLONG)0, pQuestProto->m_prizeData[i] * 10000, false);
				
				GAMELOG << init("QUEST PRIZE", ch)
					<< pQuestProto->m_index << delim
					<< QPRIZE_SP << delim
					<< pQuestProto->m_prizeIndex[i] << delim
					<< pQuestProto->m_prizeData[i]
					<< end;
				break;
				
			case QPRIZE_SKILL:
				break;
				
				
			default: //추가보상은 아이템만 가능하다.
				QuestErrorMsg(rMsg, MSG_QUEST_ERR_PRIZE_FULL);
				SEND_Q(rMsg, ch->m_desc);
				//return;
			}
		
		}//find의 else
		
	}
  
	// Prize Msg Echo
	QuestPrizeMsg(rMsg, pQuest);
	SEND_Q(rMsg, ch->m_desc);
	
	// 1회용 퀘스트 저장
	if (pQuestProto->m_type[1] == QTYPE_REPEAT_ONCE)
	{
#ifdef QUEST_DATA_EXTEND
		ch->m_questList.SetQuestState(pQuest, QUEST_STATE_DONE);
#else // QUEST_DATA_EXTEND
		ch->m_questList.SetDoneQuest(pQuestProto->m_index);
#endif // QUEST_DATA_EXTEND
	}

	// 캐릭터 리스트에서 삭제
#ifdef QUEST_DATA_EXTEND
	else
		ch->m_questList.DelQuest(ch, pQuest);
#else // QUEST_DATA_EXTEND
	ch->m_questList.DelQuest(ch, pQuest);
#endif // QUEST_DATA_EXTEND

	bool bTicket = true;
	// 싱글던전 퀘스트이면 입장권 nComplete 증가 하지 않는다
	if (pQuestProto->m_type[0] == QTYPE_KIND_DEFEAT || pQuestProto->m_type[0] == QTYPE_KIND_SAVE)
		bTicket = false;

	if (bTicket)
	{
		// 온전하게 보상 받았다면
#ifdef QUEST_DATA_EXTEND
		ch->m_questList.IncreaseQuestComepleteCount();
#else // QUEST_DATA_EXTEND
		ch->m_questList.m_nComplete++;
#endif // QUEST_DATA_EXTEND
		
		// 5회 이상이고 싱글던전1,2의 레벨 조건이면
#ifdef QUEST_DATA_EXTEND
		if (ch->m_questList.GetQuestComepleteCount() >= S2_TICKET_QUEST_DONE && ch->m_level >= S2_TICKET_LEVEL_START && ch->m_level <= S2_TICKET_LEVEL_END)
#else // QUEST_DATA_EXTEND
		if (ch->m_questList.m_nComplete >= S2_TICKET_QUEST_DONE && ch->m_level >= S2_TICKET_LEVEL_START && ch->m_level <= S2_TICKET_LEVEL_END)
#endif // QUEST_DATA_EXTEND
		{
#ifdef QUEST_DATA_EXTEND
			ch->m_questList.ResetQuestComepleteCount();
#else // QUEST_DATA_EXTEND
			ch->m_questList.m_nComplete = 0;
#endif // QUEST_DATA_EXTEND

			CItem* item = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_sTicketItem2->m_index, -1, 0 ,0, 1);
			
			if (!item)
				return;
			
			CNetMsg itemmsg;
			
			// 들어갈 인벤토리 결정
			CInventory* inven = GET_INVENTORY(ch, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
			if (!inven)
				return;
			
			bool bCountable = false;
			// 인벤에 넣기
			if (AddToInventory(ch, item, true, true))
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
					ItemAddMsg(itemmsg, item);
			}
			else
			{				
				// 인젠토리 꽉차서 못 받을 때 Drop
				item = ch->m_pArea->DropItem(item, ch);
				if (!item)
					return;

				CNetMsg dropMsg;
				item->m_preferenceIndex = ch->m_index;
				ItemDropMsg(dropMsg, ch, item);
				ch->m_pArea->SendToCell(dropMsg, ch, true);
				return ;
			}
			
			// 메시지 보내고
			SEND_Q(itemmsg, ch->m_desc);
			
			// Item LOG
			GAMELOG << init("ITEM_PICK_QUESTPRIZE", ch)
					<< itemlog(item)
					<< end;
			
			if (bCountable)
				delete item;
		}
	}


}

void do_QuestComplete(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype;
	int questIndex;
	CNetMsg errMsg;

	msg.MoveFirst();

	msg >> subtype
		>> questIndex;

#ifdef QUEST_DATA_EXTEND

	CQuest* pQuest = ch->m_questList.FindQuest(questIndex);

	if (!pQuest || pQuest->GetQuestState() != QUEST_STATE_RUN)
		return ;

	const CQuestProto* pQuestProto = pQuest->GetQuestProto();

#else // QUEST_DATA_EXTEND

	int idx = ch->m_questList.FindQuest(questIndex);

	// 없는 퀘스트
	if (idx < 0)
		return;

	CQuest* pQuest = ch->m_questList.m_list[idx];

	if (!pQuest)
		return ;

	const CQuestProto* pQuestProto = pQuest->m_proto;

#endif // QUEST_DATA_EXTEND

	if (!pQuestProto)
		return ;

	int i;
	CNetMsg rMsg;

	switch (pQuestProto->m_type[0])
	{
	// 구출 퀘스트 완료 Validation 검사!!
	case QTYPE_KIND_SAVE:
		{
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				// 아예 없으면 
				if (pQuestProto->m_conditionType[i] == -1)
					continue;
				
				switch (pQuestProto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						// 전달 퀘스트 아이템은 퀘스트 인벤에서만 !!!
						int r, c;
						if (!ch->m_invenQuest.FindItem(&r, &c, pQuestProto->m_conditionIndex[i], 0, 0))
						{
							// Quest FAIL Log
							GAMELOG << init("QUEST FAIL", ch)
								<< pQuestProto->m_index
								<< end;
							
							QuestFailMsg(rMsg, pQuest);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rMsg, ch->m_desc);
							return;
						}
						
						// 찾았으면 수량 확인 해서 줄이기
						CItem* item = ch->m_invenQuest.GetItem(r, c);
						
						if (!item)
						{
							// Quest FAIL Log
							GAMELOG << init("QUEST FAIL", ch)
								<< pQuestProto->m_index
								<< end;
							
							QuestFailMsg(rMsg, pQuest);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rMsg, ch->m_desc);
							return;
						}
						
						// 필요 숫자 만큼 없음
						if (item->Count() < pQuestProto->m_conditionNum[i])
						{
							// Quest FAIL Log
							GAMELOG << init("QUEST FAIL", ch)
								<< pQuestProto->m_index
								<< end;
							
							QuestFailMsg(rMsg, pQuest);
							ch->m_questList.DelQuest(ch, pQuest);
							SEND_Q(rMsg, ch->m_desc);
							return;
						}
					}
					break;
				case QCONDITION_NPC:
					{
#ifdef QUEST_DATA_EXTEND
						pQuest->IncreaseQuestValue(i);
#else // QUEST_DATA_EXTEND
						pQuest->m_currentData[i]++;
#endif // QUEST_DATA_EXTEND
					}
					break;
				default:
					break;
				}
			}
		}
		break;

	case QTYPE_KIND_TUTORIAL:
		{
			CItem* item = NULL;

			for (i=0; i < gserver.m_itemProtoList.m_nCount; i++)
			{
				if (gserver.m_itemProtoList.m_protoItems[i].m_typeIdx != ITYPE_WEAR)
					continue;
				if (gserver.m_itemProtoList.m_protoItems[i].m_subtypeIdx != IWEAR_ARMOR)
					continue;
				if (gserver.m_itemProtoList.m_protoItems[i].GetItemProtoLevel() != 6)
					continue;
				if (!(gserver.m_itemProtoList.m_protoItems[i].m_jobFlag & (1 << ch->m_job)))
					continue;

				item = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_protoItems[i].m_index, -1, 0, 0, 1);
				break;
			}

			if (!item)
				return;

			CNetMsg itemmsg;
			
			// 인벤에 넣기
			if (AddToInventory(ch, item, true, true))
				ItemAddMsg(itemmsg, item);
			else
			{
				item = ch->m_pArea->DropItem(item, ch);
				if (!item)
					return;

				// Drop Msg 보내기
				CNetMsg dropMsg;
				item->m_preferenceIndex = ch->m_index;
				ItemDropMsg(dropMsg, ch, item);
				ch->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			}
			
			// 메시지 보내고
			SEND_Q(itemmsg, ch->m_desc);
			
			// Item LOG
			GAMELOG << init("ITEM_PICK_QUESTPRIZE", ch)
					<< itemlog(item)
					<< end;
			
#ifdef QUEST_DATA_EXTEND
			// Complete Msg 최초전송인경우에만 전송
			if (!pQuest->IsCompleted())
			{
				//if(ch->m_questList.m_bComplete2[idx])
				{
					// Quest Complete Log
					GAMELOG << init("QUEST COMPLETE (do_QuestComplete)", ch)
							<< pQuestProto->m_index
							<< end;
					
					// 완료 메시지 
					CNetMsg completeMsg;
					QuestCompleteMsg(completeMsg, pQuest);
					SEND_Q(completeMsg, ch->m_desc);
				}
				pQuest->SetComplete1(true);
			}

			// Quest Prize Log
			GAMELOG << init("QUEST PRIZE",  ch->m_nick, ch->m_nick)
					<< pQuestProto->m_index << delim
					<< QPRIZE_MONEY << delim
					<< pQuestProto->m_prizeIndex[0] << delim
					<< pQuestProto->m_prizeData[0]
					<< end;

			// Prize Msg Echo
			QuestPrizeMsg(rMsg, pQuest);
			SEND_Q(rMsg, ch->m_desc);
			
			// 1회용 퀘스트 저장
			if (pQuestProto->m_type[1] == QTYPE_REPEAT_ONCE)
			{
				ch->m_questList.SetQuestState(pQuest, QUEST_STATE_DONE);
			}
			else
			{
				ch->m_questList.DelQuest(ch, pQuest);
			}
#else // QUEST_DATA_EXTEND
			// Complete Msg 최초전송인경우에만 전송
			if (!ch->m_questList.IsCompleted(idx))
			{
				//if(ch->m_questList.m_bComplete2[idx])
				{
				// Quest Complete Log
				GAMELOG << init("QUEST COMPLETE (do_QuestComplete)", ch)
					<< pQuestProto->m_index
					<< end;
				
				// 완료 메시지 
				CNetMsg completeMsg;
				QuestCompleteMsg(completeMsg, pQuest);
				SEND_Q(completeMsg, ch->m_desc);
				}
				ch->m_questList.SetComplete(idx, true);
			}

			// Quest Prize Log
			GAMELOG << init("QUEST PRIZE",  ch->m_nick, ch->m_nick)
					<< pQuestProto->m_index << delim
					<< QPRIZE_MONEY << delim
					<< pQuestProto->m_prizeIndex[0] << delim
					<< pQuestProto->m_prizeData[0]
					<< end;

			// Prize Msg Echo
			QuestPrizeMsg(rMsg, pQuest);
			SEND_Q(rMsg, ch->m_desc);
			
			// 1회용 퀘스트 저장
			if (pQuestProto->m_type[1] == QTYPE_REPEAT_ONCE)
			{
				ch->m_questList.SetDoneQuest(pQuestProto->m_index);
			}
			// 캐릭터 리스트에서 삭제
			ch->m_questList.DelQuest(ch, pQuest);
#endif // QUEST_DATA_EXTEND
			return;
		}
		break;

	default:
		return;
	}

#ifdef QUEST_DATA_EXTEND
	// Complete Msg 최초전송인경우에만 전송
	if (!pQuest->IsCompleted())
	{
		if(pQuest->GetComplete2())
		{
			// Quest Complete Log
			GAMELOG << init("QUEST COMPLETE (do_QuestComplete2)", ch)
					<< pQuestProto->m_index
					<< end;
			
			// 완료 메시지 
			CNetMsg completeMsg;
			QuestCompleteMsg(completeMsg, pQuest);
			SEND_Q(completeMsg, ch->m_desc);
		}
		pQuest->SetComplete1(true);
	}
#else // QUEST_DATA_EXTEND
	// Complete Msg 최초전송인경우에만 전송
	if (!ch->m_questList.IsCompleted(idx))
	{
		if(ch->m_questList.m_bComplete2[idx])
		{
		// Quest Complete Log
		GAMELOG << init("QUEST COMPLETE (do_QuestComplete2)", ch)
			<< pQuestProto->m_index
			<< end;
		
		// 완료 메시지 
		CNetMsg completeMsg;
		QuestCompleteMsg(completeMsg, pQuest);
		SEND_Q(completeMsg, ch->m_desc);
		}
		ch->m_questList.SetComplete(idx, true);
	}
#endif // QUEST_DATA_EXTEND
}

CItem* OptionSettingItem(CPC* ch, CItem* item)
{
#ifdef MONSTER_RAID_SYSTEM
		if (item->IsRareItem())
			return item;
#endif // MONSTER_RAID_SYSTEM

#ifdef SET_ITEM
		if (item->IsOriginItem())
			return item;
#endif // SET_ITEM
		
	if( (item->m_itemProto->m_flag & ITEM_FLAG_CASH ))
		return item;

	// 악세사리 일때 Option Setting

	// 순서대로 5,4,3,1개 붙을 확률
	const int factor[] = {10, 40, 80, 100};
	const int limit[] = {500, 2000, 4000, 5000};

	int prob[MAX_ACCESSORY_OPTION-1];
	int i, j;

	for (i=0; i < MAX_ACCESSORY_OPTION - 1; i++)
	{
		if ((prob[i] = ch->m_level * factor[i]) > limit[0])
			prob[i] = limit[i];
	}

	// 옵션 수 결정
	int result = GetRandom(1, 10000);
	int num = 2;	// default 2개
	int* type;
	
	if (result <= prob[0])
		num = 5;
	else if (result <= prob[1])
		num = 4;
	else if (result <= prob[2])
		num = 3;
	else if (result <= prob[3])
		num = 1;
	else 
		num = 2;

	type = new int[num];

	LONGLONG bitfield = 0;		// TODO : 옵션이 64개 넘으면 고쳐야 함!
	const int maxnumoption = 24;	// TODO : 나중에 MAX_NUM_OPTION으로 대체

	// 옵션 타입 결정
	i = 0;
	while (i < num)
	{
		// type[i] 결정
		type[i] = GetRandom(0, maxnumoption - 1);

		// type[i]가 중복인 검사
		// 중복이면 type[i]를 1씩 증가하면서 중복 검사
		for (j = 0; j < maxnumoption; j++)
		{
			if ((bitfield & ((LONGLONG)1 << ((type[i] + j) % maxnumoption))) == 0)
				break;
		}

		// 더이상 옵션을 붙일 수 없음
		if (j == maxnumoption)
			num = i;
		else
			type[i] = (type[i] + j) % maxnumoption;

		bitfield |= ((LONGLONG)1 << type[i]);

		// 옵션번호로 붙을 수 있는 옵션인지 검사
		COptionProto* proto = gserver.m_optionProtoList.FindProto(type[i]);
		if (!proto)
			continue;

		// 악세사리중에서 붙을 수 없으면 다시
		if (!((1 << item->m_itemProto->m_subtypeIdx) & proto->m_accessoryType))
			continue ;

		i++;
	}

	// 옵션 부여
	for (i=0; i < num; i++)
	{
		COptionProto* proto = gserver.m_optionProtoList.FindProto(type[i]);
		
		if (!proto)
			continue;
		
		item->m_nOption++;
		// Level, Value 셋팅
		item->m_option[i].MakeOptionValue(proto, ch->m_level, num);
	}

	delete [] type;


	return item;
}

void do_QuestCollect(CPC* ch, CNetMsg& msg)
{
	CNetMsg		rmsg;
	int			nNPCIndex;
	int			nItemIndex = -1;

	msg >> nNPCIndex;

	// 이전 수집 시간 검사 : 2초(PULSE_PRODUCE_DELAY)
	if (gserver.m_pulse - ch->m_nLastCollectRequest < PULSE_PRODUCE_DELAY - PULSE_HACK_ATTACK_THRESHOLD)
		return ;

	// NPC 찾기
	CNPC* pNPC = (CNPC*)ch->m_pArea->FindCharInCell(ch, nNPCIndex, MSG_CHAR_NPC);
	if (!pNPC)
		return ;

	// NPC와의 거리 검사 : 5미터
	if (GetDistance(ch, pNPC) >= 5.0f)
		return ;

	// 이전 NPC와 동일 여부 검사
	if (ch->m_nCollectRequestNPCIndex != nNPCIndex)
		ch->m_nCollectRequestCount = 0;

	// 변수값 설정
	ch->m_nLastCollectRequest = gserver.m_pulse;
	ch->m_nCollectRequestNPCIndex = nNPCIndex;
	ch->m_nCollectRequestCount++;

	// 아이템 습득 여부 결정 : 4회
	if (ch->m_nCollectRequestCount >= 4)
	{
		// 변수 리셋
		ch->m_nLastCollectRequest = 0;

		// 아이템 결정
		int nMaxRandom = MAX_NPC_PRODUCT - 1;
		while (nMaxRandom >= 0)
		{
			int nProb = GetRandom(0, nMaxRandom);
			if (pNPC->m_proto->m_product[nProb] <= 0)
				nMaxRandom--;
			else
			{
				nItemIndex = pNPC->m_proto->m_product[nProb];
				break ;
			}
		}

		if (nItemIndex > 0)
		{
			// 아이템 생성
			CItem* pItem = gserver.m_itemProtoList.CreateItem(nItemIndex, -1, 0, 0, 1);
			if (pItem)
			{
				// 아이템 지급
				if (AddToInventory(ch, pItem, true, true))
				{
					GAMELOG << init("QUEST COLLECT SUCCESS", ch)
							<< "ITEM" << delim
							<< itemlog(pItem)
							<< end;

					if (pItem->tab() < 0)
					{
						CInventory* pInven = GET_INVENTORY(ch, GET_TAB(pItem->m_itemProto->m_typeIdx, pItem->m_itemProto->m_subtypeIdx));
						if (pInven)
						{
							int nRow, nCol;
							pInven->FindItem(&nRow, &nCol, pItem->m_idNum, pItem->m_plus, pItem->m_flag);
							CItem* pPrevItem = pInven->GetItem(nRow, nCol);
							ItemUpdateMsg(rmsg, pPrevItem, pItem->Count());
							SEND_Q(rmsg, ch->m_desc);
						}
						delete pItem;
						pItem = NULL;
					}
					else
					{
						ItemAddMsg(rmsg, pItem);
						SEND_Q(rmsg, ch->m_desc);
						pItem = NULL;
					}
				}
				else
				{
					delete pItem;
					pItem = NULL;
					nItemIndex = 0;
					GAMELOG << init("QUEST COLLECT FAIL" , ch)
							<< "FULL INVEN"
							<< end;
				}
			}
			else
			{
				nItemIndex = 0;
			}
		}
		else
		{
			nItemIndex = 0;
		}

		// 결과 알림
		QuestCollectMsg(rmsg, nNPCIndex, ch->m_index, nItemIndex);
		pNPC->m_pArea->SendToCell(rmsg, pNPC, true);

		// NPC 사망
		DelAttackList(pNPC);
		ch->m_pArea->CharFromCell(pNPC, true);
		ch->m_pArea->DelNPC(pNPC);
		pNPC = NULL;
	}
	else
	{
		// 결과 알림
		QuestCollectMsg(rmsg, nNPCIndex, ch->m_index, nItemIndex);
		pNPC->m_pArea->SendToCell(rmsg, pNPC, true);
	}
}
