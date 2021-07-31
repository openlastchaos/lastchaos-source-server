#include "stdhdrs.h"
#include "Server.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Battle.h"

#ifdef ATTACK_PET

void do_ExApetSkillLean( CPC* ch, CNetMsg& msg );
void do_ExApetSkillListReq( CPC* ch, CNetMsg& msg);
void do_ExApetRebirth( CPC* ch, CNetMsg& msg);
void do_ExApetItemWear( CPC* ch, CNetMsg& msg);
void do_ExApetStatUp( CPC* ch, CNetMsg& msg);
void do_ExApetInfo( CPC* ch, CNetMsg& msg );

#ifdef APET_AI
void do_ExApetAIListReq(CPC* ch, CNetMsg& msg);
void do_ExApetAIOnOffReq(CPC* ch, CNetMsg& msg);
void do_ExApetAIUpdateReq(CPC* ch, CNetMsg& msg);
#endif

// 공격형 펫 관련
void do_AttackPet(CPC* ch, CNetMsg& msg )
{
	unsigned char subtype;

	msg >> subtype;

	switch( subtype )
	{
	case MSG_SUB_REBIRTH :
		do_ExApetRebirth( ch, msg );
		break;
	case MSG_SUB_SKILLLEAN :
		do_ExApetSkillLean( ch, msg );
		break;
	case MSG_SUB_SKILLLIST:
		do_ExApetSkillListReq( ch, msg );
		break;
	case MSG_SUB_ITEM_WEAR:
		do_ExApetItemWear( ch, msg );
		break;
	case MSG_SUB_STATUP :
		do_ExApetStatUp( ch, msg );
		break;
	case MSG_SUB_APET_INFO :
		do_ExApetInfo(ch, msg);
		break;
#ifdef APET_AI
	case MSG_SUB_AI_LIST_REQ:
		do_ExApetAIListReq(ch, msg);
		break;
	case MSG_SUB_AI_UPDATE_REQ:
		do_ExApetAIUpdateReq(ch, msg);
		break;
	case MSG_SUB_AI_ONOFF_REQ:
		do_ExApetAIOnOffReq(ch, msg);
		break;
#endif
	default :
		break;
	}
}

void do_ExApetStatUp( CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	char	stat_code;
	msg >>	stat_code;

	CAPet* apet = ch->GetAPet();
	if( !apet )
		return;

	if( apet->m_nRemainStat < 1 )
		return;

	// 0: str, 1:con, 2: dex, 3:Int
	switch( stat_code )
	{
	case 0:	// str
		apet->m_nPlusStr++;
		break;
	case 1: // con
		apet->m_nPlusCon++;
		break;
	case 2: // dex
		apet->m_nPlusDex++;
		break;
	case 3:	// Int
		apet->m_nPlusInt++;
		break;
	default :
		return;
	}

	apet->m_nRemainStat--;

	apet->CalcStatus(false);
	ExAPetStatusMsg( rmsg, apet );
	SEND_Q( rmsg, ch->m_desc );
}

void do_ExApetRebirth( CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int row, col;
	msg >> row >> col;

	CItem *petItem = ch->m_invenNormal.GetItem( row, col );
	if( petItem )
	{
		if( petItem->IsAPet() )
		{
			int petIndex = petItem->m_plus;
			CAPet* apet = ch->GetAPet(petIndex);
			if( apet )
			{
				// 봉인 여부
				if( !(petItem->m_flag & FLAG_ITEM_SEALED) )
				{
					ExAPetFuntionMsg( rmsg, MSG_SUB_REBIRTH, NULL , 3 );		// 봉인되지 않은 아이템 
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
				// 필요 나스 처리  //////////////////////////////////////////////////////
				int nNeedMoney = (apet->m_level-1)*((apet->m_level*apet->m_level*8)+20000);
				if( nNeedMoney != 0 )		// 필요 금액이 0 이면  그냥해주기
				{
					if( ch->m_moneyItem == NULL  || ch->m_moneyItem->Count() < nNeedMoney )
					{
						// 돈이 부족함
						ExAPetFuntionMsg( rmsg, MSG_SUB_REBIRTH, NULL , 2 );		// 돈이 부족합니다. 
						SEND_Q( rmsg, ch->m_desc );
						return;
					}

					if (ch->m_moneyItem->Count() > nNeedMoney)
					{
						DecreaseFromInventory(ch, ch->m_moneyItem, nNeedMoney);
						ItemUpdateMsg(rmsg, ch->m_moneyItem, -nNeedMoney);
						SEND_Q(rmsg, ch->m_desc);
					}
					else
					{
						ItemDeleteMsg(rmsg, ch->m_moneyItem);
						SEND_Q(rmsg, ch->m_desc);
						RemoveFromInventory(ch, ch->m_moneyItem, true, true);
					}
				}
				//////////////////////////////////////////////////////////////////////////

				/// 회복 /////////////////////////////////////////////////////////////////
				apet->m_hp = apet->m_maxHP * 40 / 100;					// HP 40% 회복
				petItem->m_flag &= ~FLAG_ITEM_SEALED;
				ItemUpdateMsg(rmsg, petItem, 0);
				SEND_Q(rmsg, ch->m_desc);
				//////////////////////////////////////////////////////////////////////////

				ExAPetFuntionMsg( rmsg, MSG_SUB_REBIRTH, NULL , 0 );		// 봉인 해제 성공 
				SEND_Q( rmsg, ch->m_desc );
			}
		}
		else			// 공격형 펫이 아님
		{
			ExAPetFuntionMsg( rmsg, MSG_SUB_REBIRTH, NULL , 1 );		// 펫이 아닙니다.  
			SEND_Q( rmsg, ch->m_desc );
		}
	}
}

void do_ExApetSkillListReq( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;
	CAPet* apet = ch->GetAPet();
	if( apet )
	{
		ExAPetFuntionMsg( rmsg, MSG_SUB_SKILLLIST, apet , 0 );
		SEND_Q( rmsg, ch->m_desc );
	}
	else
	{
		ExAPetFuntionMsg( rmsg, MSG_SUB_SKILLLIST, apet , 1 );		// FAIL
		SEND_Q( rmsg, ch->m_desc );
	}
	return;	
}

void do_ExApetSkillLean( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;

	int skillindex = -1;
	msg >> skillindex;

	CAPet* apet = ch->GetAPet();
	if (apet == NULL)
	{
		ExAPetFuntionMsg( rmsg, MSG_SUB_SKILLLEAN, NULL , 1 );		// FAIL
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	const CSkillProto* skillProto = gserver.m_skillProtoList.Find(skillindex);
	if (skillProto == NULL)
		return ;

	// 펫이 해당 스킬을 가지고 있는지 검사
	CSkill* curSkill = apet->FindSkill(skillindex);
	int nextlevel = (curSkill == NULL) ? 1 : curSkill->m_level + 1;

	const CSkillLevelProto* skillLevelProto = skillProto->Level(nextlevel);
	if (skillLevelProto == NULL)
		return ;
	if (skillProto->m_maxLevel < nextlevel)
		return ;

	// 조건 검사 : 직업
	if ( ! skillProto->IsSameJob(apet) )
	{
		ExAPetFuntionMsg( rmsg, MSG_SUB_SKILLLEAN, NULL , 1 );		// FAIL
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 조건 검사 : 레벨
	if (skillLevelProto->m_learnLevel > apet->m_level)
	{
		ExAPetFuntionMsg( rmsg, MSG_SUB_SKILLLEAN, NULL , 2 );		// 레벨이 맞지 않음
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 조건 검사 : 포인트 : 기술 포인트는 SP와 달리 단위가 1로 통일
	if (skillLevelProto->m_learnSP > apet->m_nSP )
	{
		ExAPetFuntionMsg( rmsg, MSG_SUB_SKILLLEAN, NULL , 3 );		// 스킬포인트 부족
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 조건 검사 : 아이템
	int i;
	CItem* item[MAX_SKILL_LEARN_ITEM];
	for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
	{
		item[i] = NULL;
		if (skillLevelProto->m_learnItemIndex[i] < 0)
			continue ;
		int r, c;
		if (!ch->m_invenNormal.FindItem(&r, &c, skillLevelProto->m_learnItemIndex[i], 0, 0))
		{
			ExAPetFuntionMsg( rmsg, MSG_SUB_SKILLLEAN, NULL , 4 );		// 필요한 아이템 없음
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		item[i] = ch->m_invenNormal.GetItem(r, c);
		if (!item[i] || item[i]->Count() < skillLevelProto->m_learnItemCount[i])
		{
			ExAPetFuntionMsg( rmsg, MSG_SUB_SKILLLEAN, NULL , 5 );		// 필요한 아이템이 모자람
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	// 스킬 추가
	if (curSkill == NULL)
	{
		// 신규 추가
		curSkill = gserver.m_skillProtoList.Create(skillindex);
		if (!curSkill)
			return ;
		apet->m_skillList.Add(curSkill);
	}
	else
	{
		// 기존 스킬 레벨 업
		curSkill->m_level++;
	}

	// 포인트 소모
	apet->m_nSP -= skillLevelProto->m_learnSP;
	ExAPetStatusMsg(rmsg, apet);
	SEND_Q(rmsg, ch->m_desc);

	// 아이템 소모
	for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
	{
		if (item[i] && skillLevelProto->m_learnItemCount[i] > 0)
		{
			DecreaseFromInventory(ch, item[i], skillLevelProto->m_learnItemCount[i]);
			if (item[i]->Count() < 1)
			{
				ItemDeleteMsg(rmsg, item[i]);
				SEND_Q(rmsg, ch->m_desc);
				RemoveFromInventory(ch, item[i], true, true);
			}
			else
			{
				ItemUpdateMsg(rmsg, item[i], -skillLevelProto->m_learnItemCount[i]);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}

	// 결과 알리기
	ExAPetFuntionMsg( rmsg, MSG_SUB_SKILLLEAN, NULL , 0 );		// 배우기 성공
	rmsg << skillindex << (char)nextlevel;						// 성공시만 스킬인덱스 , 스킬레벨 추가 
	SEND_Q(rmsg, ch->m_desc);

	// 스킬 리스트 갱신
	ExAPetFuntionMsg( rmsg, MSG_SUB_SKILLLIST, apet , 0 );		// 배우기 성공
	SEND_Q(rmsg, ch->m_desc);

	// 게임 로그
	// TODO : petlog
	GAMELOG << init("ATTACK PET SKILL LEARN", ch)
			<< "ATTACK_PET" << delim
			<< apet->m_index << delim
			<< apet->m_name << delim
			<< "SKILL" << delim
			<< curSkill->m_proto->m_index << delim
			<< curSkill->m_level
			<< end;
}

void do_ExApetItemWear( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;
	char wear_pos, tab1, row1, col1;
	int itemidx1;
	int	itemidx2 = -1;
	
	msg >> wear_pos
		>> tab1 >> row1 >> col1 >> itemidx1;

	if (itemidx1 < 0)
	{
		row1 = -1;
		col1 = -1;
	}

	CAPet* apet = ch->GetAPet();
	if( apet == NULL )
	{
		// 펫을 착용하시고 아이템을 입으시오
		return;
	}

	if ( wear_pos < 0 || wear_pos > APET_WEARPOINT )
	{
		SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CInventory* inven1 = GET_INVENTORY(ch, tab1);
	CItem * item1 = (inven1) ? inven1->GetItem(row1, col1) : NULL;	// 입는 아이템

	// tab1, row1, col1 이 모두 0 면 그냥 벗기
	if( tab1 == -1 && row1 == -1 && col1 == -1 )
	{
		itemidx2 = itemidx1;
		itemidx1 = -1;
		item1 = NULL;
	}

	// 착용 위치가 올바른지 검사
	if (item1 && item1->m_itemProto->m_wearing != wear_pos)
	{
		SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 입는 아이템이 결합된 아이템이면 불가능
	if (item1 && !item1->CanWear())
	{
		SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

#ifdef MONSTER_RAID_SYSTEM
	if(  item1 && item1->IsRareItem() && apet->m_level < item1->m_itemProto->GetItemProtoLevel() )
	{
		SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif

	CItem * item2 = apet->m_wearing[(int)wear_pos];		// 벗는 아이템

	// 둘다 NULL 이면 무시
	if (!item1 && !item2)
	{
		SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (!item1)
	{
		// 벗기이면 무조건 위치가 올바르게 전달되야 함
		if (item2 != NULL && item2->m_itemProto->m_wearing != wear_pos)
		{
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}
	
	if (itemidx1 == -1)
	{
		// 벗기
		if (!item2)
		{
			// 빈자리에 빈자리 입히기는 무시
			return ;
		}

		// 방어코드 : 원래 벗었었는데 또 벗을라 하믄 리턴
		if (item2->m_wearPos == WEARING_NONE)
			return;

		// 벗기 펫 인벤 의 아이템을 캐릭터 인벤에 Item 넣어 준다
		// item2 를 인벤에 추가 
		if( ch->m_invenNormal.GetSpace() > 0 && AddToInventory(ch, item2, false, false) )
		{
			item2->m_wearPos = WEARING_NONE;
			ItemAddMsg(rmsg, item2);
			SEND_Q(rmsg, ch->m_desc);
			apet->m_wearing[(int)wear_pos] = NULL;
		}
		else
		{
			// 인벤에 공간 없음 
			SysMsg(rmsg, MSG_SYS_FULL_INVENTORY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		ItemAPetWearMsg( rmsg, wear_pos, NULL, item2);
		SEND_Q(rmsg, ch->m_desc);		
	}
	else
	{
		// 입기
		if (!item1)
		{
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		if (!(item1->m_itemProto->m_jobFlag & (1 << JOB_APET)))
		{
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 방어코드 : 원래 벗겨져 있던게 아니면 리턴
		if (item1->m_wearPos != WEARING_NONE)
			return;

		if (item2)
		{
			// 현재꺼 벗기기
			// 캐릭터 인벤에 넣기
			if( ch->m_invenNormal.GetSpace() > 0 &&  AddToInventory(ch, item2, false, false) )
			{
				item2->m_wearPos = WEARING_NONE;
				apet->m_wearing[(int)wear_pos] = NULL;
				ItemAddMsg(rmsg, item2);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				// 인벤에 공간 없음 
				SysMsg(rmsg, MSG_SYS_FULL_INVENTORY);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		}

		// 인벤에서 빼고 펫인벤 에 넣고 //////////////////////////////////////////
		apet->m_wearing[(int)wear_pos] = item1;
		ItemDeleteMsg(rmsg, item1);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, item1, false, true);
		item1->m_wearPos = wear_pos;
		//////////////////////////////////////////////////////////////////////////
		AddItemExAPetMsg(rmsg, item1);
		SEND_Q(rmsg, ch->m_desc);

		ItemAPetWearMsg(rmsg, wear_pos, item1, item2);
		SEND_Q(rmsg, ch->m_desc);
	}

	apet->ApplyItemValue();

	// 주위에 입은 정보 알림
	ItemAPetWearingMsg( rmsg, apet );
	ch->m_pArea->SendToCell(rmsg, ch, false);

	ExAPetStatusMsg( rmsg, apet );
	ch->m_pArea->SendToCell(rmsg, ch, true);
}

void do_ExApetInfo( CPC* ch, CNetMsg& msg )
{
	int pet_index;
	int char_index;

	msg >> char_index
		>> pet_index;

	CPC* tpc = gserver.m_playerList.Find( char_index );
	if( tpc == NULL )
		return;

	CAPet* apet = tpc->GetAPet( pet_index );
	if( apet )
	{
		apet->CalcStatus(false);

		CNetMsg rmsg;
		ExApetSellInfo( rmsg, apet );
		SEND_Q( rmsg, ch->m_desc );
	}

}

#ifdef APET_AI
void do_ExApetAIListReq( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;
	CAPet * pAPet = ch->GetAPet();
	if (!pAPet)
		return;

	APetAIListMsg(rmsg, pAPet);
	SEND_Q(rmsg, ch->m_desc);
}
void do_ExApetAIOnOffReq(CPC* ch, CNetMsg& msg)
{
	char cEnable;
	unsigned char ucError = MSG_APET_ERROR_OK;

	msg >> cEnable;

	CNetMsg rmsg;
	CAPet * pAPet = ch->GetAPet();
	if (pAPet)
		pAPet->m_cAI = cEnable;
	else
		ucError = MSG_APET_ERROR;
	
	APetAIOnOffMsg(rmsg, pAPet, ucError);
	SEND_Q(rmsg, ch->m_desc);
	
//	if (pAPet->m_cAI)
//	{
//		APetAIListMsg(rmsg, pAPet);
//		SEND_Q(rmsg, ch->m_desc);
//	}
}
void do_ExApetAIUpdateReq(CPC* ch, CNetMsg& msg)
{
	CAPet * pAPet = ch->GetAPet();
	if (!pAPet)
		return;
	
	int nSlot = 0, nAICount = 0;
	int nAIindex[APET_MAX_AI] = {0,};
	unsigned char ucError = MSG_APET_ERROR_OK;
	APET_AI_DATA tAI[APET_MAX_AI];
	memset(tAI, -1, sizeof(APET_AI_DATA) * APET_MAX_AI);
	
	msg >> nSlot >> nAICount;

	if (nSlot > pAPet->m_nAISlot || nSlot > APET_MAX_AI)
		return;

	int i = 0;
	for (i = 0; i < nAICount; i++)
	{
		msg >> nAIindex[i];
		msg >> tAI[i].nItemNum
			>> tAI[i].nSkillType
			>> tAI[i].nSkillIdx
			>> tAI[i].cUse;
	} 

	memset(pAPet->m_tAIData, -1, sizeof(APET_AI_DATA) * APET_MAX_AI);
	for (i = 0; i < APET_MAX_AI; i++)
	{
		pAPet->m_tAIData[i].cUse = 0;
		pAPet->m_tAIData[i].pItem = NULL;
	}

	int nIndex = 0;
	for (i = 0; i < nAICount; i++)
	{
		if ((nAIindex[i] < 0 || nAIindex[i] > APET_MAX_AI))
		{
			ucError = MSG_APET_ERR_WRONG_AI;
			GAMELOG << init("APET_AI_UPDATE", ch) << delim << "ai count : " << delim << nSlot << end;
			break;
		}

		int r, c;
		CItem * pItem = NULL;
		if (ch->m_invenNormal.FindItem(&r, &c, tAI[i].nItemNum, -1, -1))
			pItem = ch->m_invenNormal.GetItem(r, c);

		if (!pItem || (pItem && (pItem->m_itemProto->m_typeIdx != ITYPE_ETC || pItem->m_itemProto->m_subtypeIdx != IETC_PET_AI)))
		{
			ucError = MSG_APET_ERR_NOT_HAVE_ITEM;
			GAMELOG << init("APET_AI_UPDATE ITEM", ch) << delim << "ai count : " << delim << nSlot << delim;

			if (pItem)
			{
				GAMELOG << "item index : " << pItem->m_itemProto->m_index << delim
						<< "skill index : " << tAI[i].nSkillIdx << end;
			}
			break;
		}

		if (tAI[i].nSkillType == 0 && !pAPet->FindSkill(tAI[i].nSkillIdx))
		{
			ucError = MSG_APET_ERR_NOT_HAVE_ITEM;
			GAMELOG << init("APET_AI_UPDATE SKILL", ch) << delim << "ai count : " << delim << nSlot << delim
					<< "item index : " << pItem->m_itemProto->m_index << delim
					<< "skill index : " << tAI[i].nSkillIdx << end;
			break;
		}

		nIndex = nAIindex[i];
		pAPet->m_tAIData[nIndex].pItem		= pItem;
		pAPet->m_tAIData[nIndex].nItemNum	= pItem->m_itemProto->m_index;
		pAPet->m_tAIData[nIndex].nSkillIdx	= tAI[i].nSkillIdx;
		pAPet->m_tAIData[nIndex].nSkillType	= tAI[i].nSkillType;
		pAPet->m_tAIData[nIndex].cUse		= tAI[i].cUse;
	}
	pAPet->m_nAISlot = nSlot;
	pAPet->m_nAICount = nAICount;

	CNetMsg rmsg;
	rmsg.Init(MSG_EXTEND);
	rmsg << MSG_EX_ATTACK_PET
		 << (unsigned char)MSG_SUB_AI_UPDATE_REP
		 << ucError;
	SEND_Q(rmsg, ch->m_desc);
	
	pAPet->m_nAICount = nAICount;
	
	APetAIListMsg(rmsg, pAPet);
	SEND_Q(rmsg, ch->m_desc);
}
#endif// APET_AI
#endif // ATTACK_PET