#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"

////////////////
// 죽었을때 부활
#if defined (EX_GO_ZONE_REBIRTH)

void do_Rebirth(CPC* ch, CNetMsg& msg)
{
	// 주문서 인덱스 주문서 사용여부
	// 부활 장소 0: 마지막에 저장된 존	1: 제자리 부활
	// itemindex = 0, buse = 0: 부활진지 1: 가까운 마을
	int itemindex = 0;
	char buse = 0;
	char bhere = 1;

	int lastsavezone	= 0;

#ifdef REBIRTH_FIELD_DONGEON
	char bDeadBy = 0;		// 0:PC 1:NPC
#endif // REBIRTH_FIELD_DONGEON

	if( msg.CanRead(sizeof(itemindex)) )
	{
		msg >> itemindex
			>> buse
			>> bhere;
	}

#ifdef REBIRTH_FIELD_DONGEON
	msg >> bDeadBy;
#endif // REBIRTH_FIELD_DONGEON
	// 죽은 사람은 부활 메시지를 보내면 그 사람의 입력버퍼를 비운다
	ch->m_desc->m_inBuf.Clear();
	ch->m_desc->m_inQ.Clear();

	if (!DEAD(ch))
	{
		GAMELOG << init("REBIRTH PC FAIL NOT DEAD", ch) << end;
		return;
	}
	else
	{
		GAMELOG << init("REBIRTH PC", ch) << end;
	}
	
	// 현재 존에 따라 설정
	if (ch->m_pZone == NULL)
		return ;

	int zone; // = ZONE_START;
	int extra = 0;

	if(ch->GetLastSaveZone(&lastsavezone) == -1)
	{
		lastsavezone = ZONE_START;
	//	lastsaveextra = gserver.FindExtraByZoneIndex(ch->IsChaotic(), lastsavezone);
	}

	switch (ch->m_pZone->m_index)
	{
	case ZONE_START:
	case ZONE_DRATAN:
	case ZONE_MERAC:
	case ZONE_EGEHA:
	case ZONE_STREIANA:
	case ZONE_FREE_PK_DUNGEON:
		// 마지막에 저장된 위치로 보낸다.
		zone = lastsavezone;
		extra = 0;
// 공성진행중 처리
#ifdef ENABLE_WAR_CASTLE
		if (ch->m_pZone->m_index == ZONE_MERAC)
		{
			CWarCastle* castle = CWarCastle::GetCastleObject(CWarCastle::GetCurSubServerCastleZoneIndex());
			if (castle && castle->GetState() != WCSF_NORMAL)
			{
				zone = castle->GetZoneIndex();		
				extra = castle->GetRegenPoint(ch->GetJoinFlag(castle->GetZoneIndex()), ch);
			}
		}
#endif // ENABLE_WAR_CASTLE

#ifdef DRATAN_CASTLE
		if (ch->m_pZone->m_index == ZONE_DRATAN)
		{
			CDratanCastle * pCastle = CDratanCastle::CreateInstance();
			if (pCastle && pCastle->GetState() != WCSF_NORMAL 
				&& ch->GetJoinFlag(pCastle->GetZoneIndex()) != WCJF_NONE
				&& ch->GetMapAttr() == MAPATT_WARZONE)
			{
				zone = pCastle->GetZoneIndex();
				if (zone == ZONE_DRATAN && itemindex == 0 && buse == 1)
				{	// 간이 마을
					extra = 27;
				}
				else
				{	// 부활 진지
					extra = pCastle->GetRegenPoint(ch->GetJoinFlag(pCastle->GetZoneIndex()), ch);
				}
			}
		}
#endif // 	DRATAN_CASTLE

		break;
		
	case ZONE_SINGLE_DUNGEON_TUTORIAL:
	case ZONE_DUNGEON1:
	case ZONE_DUNGEON2:
	case ZONE_DUNGEON3:
	case ZONE_DUNGEON4:
	case ZONE_DRATAN_CASTLE_DUNGEON:
	case ZONE_SPRIT_CAVE:
	case ZONE_QUANIAN_CAVE:
	case ZONE_GOLEM_CAVE:
		/* dungeon */
	case ZONE_EBONY_MINE:
	case ZONE_MISTY_CANYON:
	case ZONE_FLORAIM_CAVE:
		{
			zone = ch->m_pZone->m_index;
			extra = 0;
#ifdef REBIRTH_FIELD_DONGEON
			if(!itemindex && !bDeadBy)
			{
				zone = lastsavezone;
				extra = 0;
			}
#endif // REBIRTH_FIELD_DONGEON
		}
		break;
	case ZONE_PK_TOURNAMENT:
		{
			zone = ch->m_pZone->m_index;
			extra = 0;
		}
		break;
	case ZONE_EGEHA_DUNGEON_1:
	case ZONE_EGEHA_DUNGEON_8:
	case ZONE_EGEHA_DUNGEON_9:
	case ZONE_EGEHA_DUNGEON_10:
		zone = ZONE_EGEHA_DUNGEON_1;
		extra = 0;
#ifdef REBIRTH_FIELD_DONGEON
		if(!itemindex && !bDeadBy)
		{
			zone = lastsavezone;
			extra = 0;
		}
#endif // REBIRTH_FIELD_DONGEON
		break;

	case ZONE_SINGLE_DUNGEON1:
	case ZONE_SINGLE_DUNGEON2:
	case ZONE_SINGLE_DUNGEON3:
	case ZONE_SINGLE_DUNGEON4:
		{
			bool bPDQuest = false;
			CQuest* pQuest = NULL;
			const CQuestProto* pQuestProto = NULL;
			int nQuestValue = 0;

#ifdef QUEST_DATA_EXTEND
			CQuest* pQuestNext = NULL;
			pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
			while ((pQuest = pQuestNext))
			{
				pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
				switch (pQuest->GetQuestType0())
				{
				case QTYPE_KIND_DEFEAT:
				case QTYPE_KIND_SAVE:
					bPDQuest = true;
					pQuestProto = pQuest->GetQuestProto();
					nQuestValue = pQuest->GetQuestValue(0);
					break;
				default:
					break;
				}
				if (bPDQuest)
					break;
			}
#else // QUEST_DATA_EXTEND
			int i;
			for (i=0; i < QUEST_MAX_PC; i++)
			{
				if (ch->m_questList.m_list[i] && ch->m_questList.m_bQuest[i] &&
					(ch->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_DEFEAT ||
					ch->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_SAVE))
				{
					bPDQuest = true;
					pQuest = ch->m_questList.m_list[i];
					pQuestProto = pQuest->m_proto;
					nQuestValue = pQuest->m_currentData[0];
					break;
				}
			}
#endif // QUEST_DATA_EXTEND

			if (bPDQuest)
			{
				////////////////////////////////////////
				// helper 실패 보내기
				
				///////////////////////////////////
				// pd4 퀘스트이면 helper 랭킹에 등록 : BW
				//
				// char index를 보냄
				if( pQuestProto->m_index == 105)
				{
					CNetMsg pd4RankEndMsg;
					
					pd4RankEndMsg.Init(MSG_HELPER_COMMAND);
					pd4RankEndMsg	<< MSG_HELPER_PD4_RANK_END
									<< ch->m_index
									<< nQuestValue
									<< 0;
					
					GAMELOG << init("PD4 END", ch)
							<< nQuestValue << end;

					if (IS_RUNNING_HELPER)
					{
						SEND_Q(pd4RankEndMsg, gserver.m_helper);
					}
					else
					{
						GAMELOG << init("PD4 QUEST ERROR : CANNOT FIND HELPER")
								<< ch->m_index << end;
					}
				}
				else if (pQuestProto->m_type[0] == QTYPE_KIND_SAVE)
				{
#ifdef QUEST_DATA_EXTEND
					// 아이템 삭제
					pQuest->RemoeQuestItem(ch);
#endif // QUEST_DATA_EXTEND
				}

				// Quest Fail Log
				GAMELOG << init("QUEST FAIL", ch)
						<< pQuestProto->m_index
						<< end;
				
				CNetMsg rMsg;
				QuestFailMsg(rMsg, pQuest);
				ch->m_questList.DelQuest(ch, pQuest);
				
				SEND_Q(rMsg, ch->m_desc);
			}

			// 성향에 맞추어서 extra 값을 결정한다.
			zone = ZONE_START;
			extra = 0;
		}
		break;

#ifdef MONSTER_COMBO
	case ZONE_COMBO_DUNGEON:
		zone = ch->m_pZone->m_index;
		extra = 0;
		break;
#endif // MONSTER_COMBO

	case ZONE_EXTREME_CUBE:
		zone = ZONE_MERAC;
		extra = 0;
		break;
	case ZONE_OXQUIZROOM:
	case ZONE_GUILDROOM:
	default:
		zone = ZONE_START;
		extra = 0;
		break;
	}

	// HP, MP 회복 하고
	// 대략 반만 회복
	ch->m_hp = ch->m_dbHP / 2;
	
	// MP는 반이상 있으면 그대로..
	if (ch->m_mp < ch->m_dbMP / 2)
		ch->m_mp = ch->m_dbMP / 2;

	// 주문서에 따라 사망 경험치/ sp 복구
	if( itemindex && ch->m_assist.FindByItemIndex(itemindex) && buse)
	{
		int		nRecoverExp = 0;
		int		nRecoverSP  = 0;
		if(itemindex == 846
#ifdef LIMIT_ITEM_M35			
			|| itemindex == 2667 // 초보자용 부활 주문서  
#endif // LIMIT_ITEM_M35
			)
		{
			ch->m_exp += ch->m_loseexp;
			ch->m_skillPoint += ch->m_losesp;
			nRecoverExp = ch->m_loseexp;
			nRecoverSP = ch->m_losesp;
		}
		else if(itemindex == 845 || itemindex == 2036)
		{
			ch->m_skillPoint += ch->m_losesp;
			nRecoverSP = ch->m_losesp;
		}
		else if(itemindex == 844 || itemindex == 2035)
		{
			ch->m_exp += ch->m_loseexp;
			nRecoverExp = ch->m_loseexp;
		}

#ifdef EVENT_PHOENIX // 피닉스 부활 아이템, 패널티 복구 효과는 없음
		else if ( itemindex == 3218 )
		{
			//ch->m_exp += ch->m_loseexp;
			//ch->m_skillPoint += ch->m_losesp;
			//nRecoverExp = ch->m_loseexp;
			//nRecoverSP = ch->m_losesp;
		}
#endif //EVENT_PHOENIX

		ch->m_assist.CureByItemIndex(itemindex);

		GAMELOG << init("REBIRTH RECOVER EXP SP", ch)
				<< "EXP" << delim
				<< nRecoverExp << delim
				<< ch->m_exp << delim
				<< "SP" << delim
				<< nRecoverSP << delim
				<< ch->m_skillPoint
				<< end;
	}
	else
	{
		bhere = false;
	}

	/*
	// 주문서에 따라 버프 적용
	if( itemindex && !buse)
	{	
		CItemProto *itemproto = gserver.m_itemProtoList.FindIndex(itemindex);

		CSkill* skill = NULL;
		skill = gserver.m_skillProtoList.Create(itemproto->m_num0, itemproto->m_num1);
		if(skill)
			ApplySkill(ch, ch, skill, itemproto->m_index);

		skill = NULL;
		itemproto = NULL;
		delete skill;
		delete itemproto;
	}
	*/

	ch->m_loseexp = 0;
	ch->m_losesp = 0;


	// rebirth MSG 에코 하고
	CNetMsg rmsg;
	RebirthMsg(rmsg, ch);
	if (ch->m_pArea)
		ch->m_pArea->SendToCell(rmsg, ch, true);

	ch->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING | PLAYER_STATE_WARP | PLAYER_STATE_PKMODE);

	ch->CalcStatus(true);
	ch->SaveNow(false);
	
	ch->m_bChangeStatus = true;

	if(bhere)
	{
		GoZone(ch, 
			ch->m_pZone->m_index, 
			ch->m_pos.m_yLayer, 
			ch->m_pos.m_x, 
			ch->m_pos.m_z);
	}
	else
	{
		CZone* pZone = gserver.m_zones + gserver.FindZone(zone);
		GoZone(ch, 
			zone,
			pZone->m_zonePos[extra][0], 
			GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,
			GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);
	}
}

#else
void do_Rebirth(CPC* ch, CNetMsg& msg)
{
	// 주문서 인덱스 주문서 사용여부, 제자리 부활
	int itemindex = 0;
	char buse = 0;
	char bhere = 0;


	if( msg.CanRead(sizeof(itemindex)) )
	{
		msg >> itemindex
			>> buse
			>> bhere;
	}
	// 죽은 사람은 부활 메시지를 보내면 그 사람의 입력버퍼를 비운다
	ch->m_desc->m_inBuf.Clear();
	ch->m_desc->m_inQ.Clear();

	if (!DEAD(ch))
	{
		GAMELOG << init("REBIRTH PC FAIL NOT DEAD", ch) << end;
		return;
	}
	else
	{
		GAMELOG << init("REBIRTH PC", ch) << end;
	}
	
	// 현재 존에 따라 설정
	if (ch->m_pZone == NULL)
		return ;

	int zone = ZONE_START;
	int extra = 0;
	switch (ch->m_pZone->m_index)
	{
	case ZONE_START:
	case ZONE_DUNGEON1:
	case ZONE_DUNGEON2:
	case ZONE_DRATAN:
	case ZONE_SINGLE_DUNGEON_TUTORIAL:
	case ZONE_DUNGEON3:
	case ZONE_MERAC:
	case ZONE_DUNGEON4:
	case ZONE_EGEHA:
	case ZONE_EGEHA_DUNGEON_1:
	case ZONE_DRATAN_CASTLE_DUNGEON:
#ifdef MONSTER_COMBO
	case ZONE_COMBO_DUNGEON:
#endif // MONSTER_COMBO
	case ZONE_STREIANA:
	case ZONE_PK_TOURNAMENT:
	case ZONE_SPRIT_CAVE:
	case ZONE_QUANIAN_CAVE:
	case ZONE_GOLEM_CAVE:
	case ZONE_EBONY_MINE:
	case ZONE_MISTY_CANYON:
	case ZONE_FLORAIM_CAVE:
		zone = ch->m_pZone->m_index;
		extra = 0;
// 공성진행중 처리
#ifdef ENABLE_WAR_CASTLE
		if (ch->m_pZone->m_index == ZONE_MERAC)
		{
			CWarCastle* castle = CWarCastle::GetCastleObject(CWarCastle::GetCurSubServerCastleZoneIndex());
			if (castle && castle->GetState() != WCSF_NORMAL)
			{
				zone = castle->GetZoneIndex();		
				extra = castle->GetRegenPoint(ch->GetJoinFlag(castle->GetZoneIndex()), ch);
			}
		}
#endif // ENABLE_WAR_CASTLE

#ifdef DRATAN_CASTLE
		if (ch->m_pZone->m_index == ZONE_DRATAN)
		{
			CDratanCastle * pCastle = CDratanCastle::CreateInstance();
			if (pCastle && pCastle->GetState() != WCSF_NORMAL 
				&& ch->GetJoinFlag(pCastle->GetZoneIndex()) != WCJF_NONE
				&& ch->GetMapAttr() == MAPATT_WARZONE)
			{
				zone = pCastle->GetZoneIndex();
				if (zone == ZONE_DRATAN && itemindex == 0 && buse == 1)
				{	// 간이 마을
					extra = 27;
				}
				else
				{	// 부활 진지
					extra = pCastle->GetRegenPoint(ch->GetJoinFlag(pCastle->GetZoneIndex()), ch);
				}
			}
		}
#endif // 	DRATAN_CASTLE

		break;

	case ZONE_EGEHA_DUNGEON_8:
	case ZONE_EGEHA_DUNGEON_9:
	case ZONE_EGEHA_DUNGEON_10:
		zone = ZONE_EGEHA_DUNGEON_1;
		extra = 0;
		break;

	case ZONE_FREE_PK_DUNGEON:
		zone = ZONE_START;
		extra = 0;
		break;

	case ZONE_SINGLE_DUNGEON1:
	case ZONE_SINGLE_DUNGEON2:
	case ZONE_SINGLE_DUNGEON3:
	case ZONE_SINGLE_DUNGEON4:
		{
			bool bPDQuest = false;
			CQuest* pQuest = NULL;
			const CQuestProto* pQuestProto = NULL;
			int nQuestValue = 0;

#ifdef QUEST_DATA_EXTEND
			CQuest* pQuestNext = NULL;
			pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
			while ((pQuest = pQuestNext))
			{
				pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
				switch (pQuest->GetQuestType0())
				{
				case QTYPE_KIND_DEFEAT:
				case QTYPE_KIND_SAVE:
					bPDQuest = true;
					pQuestProto = pQuest->GetQuestProto();
					nQuestValue = pQuest->GetQuestValue(0);
					break;
				default:
					break;
				}
				if (bPDQuest)
					break;
			}
#else // QUEST_DATA_EXTEND
			int i;
			for (i=0; i < QUEST_MAX_PC; i++)
			{
				if (ch->m_questList.m_list[i] && ch->m_questList.m_bQuest[i] &&
					(ch->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_DEFEAT ||
					ch->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_SAVE))
				{
					bPDQuest = true;
					pQuest = ch->m_questList.m_list[i];
					pQuestProto = pQuest->m_proto;
					nQuestValue = pQuest->m_currentData[0];
					break;
				}
			}
#endif // QUEST_DATA_EXTEND

			if (bPDQuest)
			{
				////////////////////////////////////////
				// helper 실패 보내기
				
				///////////////////////////////////
				// pd4 퀘스트이면 helper 랭킹에 등록 : BW
				//
				// char index를 보냄
				if( pQuestProto->m_index == 105)
				{
					CNetMsg pd4RankEndMsg;
					
					pd4RankEndMsg.Init(MSG_HELPER_COMMAND);
					pd4RankEndMsg	<< MSG_HELPER_PD4_RANK_END
									<< ch->m_index
									<< nQuestValue
									<< 0;
					
					GAMELOG << init("PD4 END", ch)
							<< nQuestValue << end;

					if (IS_RUNNING_HELPER)
					{
						SEND_Q(pd4RankEndMsg, gserver.m_helper);
					}
					else
					{
						GAMELOG << init("PD4 QUEST ERROR : CANNOT FIND HELPER")
								<< ch->m_index << end;
					}
				}
				else if (pQuestProto->m_type[0] == QTYPE_KIND_SAVE)
				{
#ifdef QUEST_DATA_EXTEND
					// 아이템 삭제
					pQuest->RemoeQuestItem(ch);
#endif // QUEST_DATA_EXTEND
				}

				// Quest Fail Log
				GAMELOG << init("QUEST FAIL", ch)
						<< pQuestProto->m_index
						<< end;
				
				CNetMsg rMsg;
				QuestFailMsg(rMsg, pQuest);
				ch->m_questList.DelQuest(ch, pQuest);
				
				SEND_Q(rMsg, ch->m_desc);
			}
			zone = ZONE_START;
			extra = 0;
		}
		break;

	case ZONE_EXTREME_CUBE:
		zone = ZONE_MERAC;
		extra = 0;
		break;

	case ZONE_OXQUIZROOM:
	case ZONE_GUILDROOM:
	default:
		zone = ZONE_START;
		extra = 0;
		break;
	}

	// HP, MP 회복 하고
	// 대략 반만 회복
	ch->m_hp = ch->m_dbHP / 2;
	
	// MP는 반이상 있으면 그대로..
	if (ch->m_mp < ch->m_dbMP / 2)
		ch->m_mp = ch->m_dbMP / 2;

	// 주문서에 따라 사망 경험치/ sp 복구
	if( itemindex && ch->m_assist.FindByItemIndex(itemindex) && buse)
	{
		LONGLONG	nRecoverExp = 0;
		LONGLONG	nRecoverSP  = 0;
		if(itemindex == 846	// 부활 주문서
#ifdef LIMIT_ITEM_M35
			|| itemindex == 2667 // 초보자용 부활 주문서
#endif // LIMIT_ITEM_M35
			)
		{
			ch->m_exp += ch->m_loseexp;
			ch->m_skillPoint += ch->m_losesp;
			nRecoverExp = ch->m_loseexp;
			nRecoverSP = ch->m_losesp;
		}
		else if(itemindex == 845  // 숙련도복구 주문서
			|| itemindex == 2036) // 럭키 숙련도복구 주문서
		{
			ch->m_skillPoint += ch->m_losesp;
			nRecoverSP = ch->m_losesp;
		}
		else if(itemindex == 844  // 경험치복구 주문서
			|| itemindex == 2035) // 럭키 경험치복구 주문서
		{
			ch->m_exp += ch->m_loseexp;
			nRecoverExp = ch->m_loseexp;
		}
#ifdef EVENT_PHOENIX // 피닉스 부활 아이템, 패널티 복구 효과는 없음
		else if ( itemindex == 3218 )
		{
			//ch->m_exp += ch->m_loseexp;
			//ch->m_skillPoint += ch->m_losesp;
			//nRecoverExp = ch->m_loseexp;
			//nRecoverSP = ch->m_losesp;
		}
#endif //EVENT_PHOENIX

		ch->m_assist.CureByItemIndex(itemindex);

		GAMELOG << init("REBIRTH RECOVER EXP SP", ch)
				<< "EXP" << delim
				<< nRecoverExp << delim
				<< ch->m_exp << delim
				<< "SP" << delim
				<< nRecoverSP << delim
				<< ch->m_skillPoint
				<< end;
	}
	else
	{
		bhere = false;
	}

	/*
	// 주문서에 따라 버프 적용
	if( itemindex && !buse)
	{	
		CItemProto *itemproto = gserver.m_itemProtoList.FindIndex(itemindex);

		CSkill* skill = NULL;
		skill = gserver.m_skillProtoList.Create(itemproto->m_num0, itemproto->m_num1);
		if(skill)
			ApplySkill(ch, ch, skill, itemproto->m_index);

		skill = NULL;
		itemproto = NULL;
		delete skill;
		delete itemproto;
	}
	*/

	ch->m_loseexp = 0;
	ch->m_losesp = 0;


	// rebirth MSG 에코 하고
	CNetMsg rmsg;
	RebirthMsg(rmsg, ch);
	if (ch->m_pArea)
		ch->m_pArea->SendToCell(rmsg, ch, true);

	ch->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING | PLAYER_STATE_WARP | PLAYER_STATE_PKMODE);

	ch->CalcStatus(true);
	ch->SaveNow(false);
	
	ch->m_bChangeStatus = true;

	if(bhere)
	{
		GoZone(ch, 
			ch->m_pZone->m_index, 
			ch->m_pos.m_yLayer, 
			ch->m_pos.m_x, 
			ch->m_pos.m_z);
	}
	else
	{
		CZone* pZone = gserver.m_zones + gserver.FindZone(zone);
		GoZone(ch, 
			zone, 
			pZone->m_zonePos[extra][0], 
			GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f, 
			GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);
	}
}

#endif // EX_GO_ZONE_REBIRTH