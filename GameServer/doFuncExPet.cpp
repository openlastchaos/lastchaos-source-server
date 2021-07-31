#include "stdhdrs.h"
#include "Server.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Battle.h"

#ifdef ENABLE_PET

void do_ExPetCall(CPC* ch, CNetMsg& msg)
{
	int index;
	msg >> index;
	CPet* pet = ch->GetPet(index);
	if (!pet)
		return ;
	if (!IS_IN_CELL(pet))
		return ;
	pet->Disappear();

	GET_X(pet) = GET_X(ch);
	GET_Z(pet) = GET_Z(ch);
	GET_R(pet) = GET_H(ch);
	GET_YLAYER(pet) = GET_YLAYER(ch);

	pet->Appear(true);

	CNetMsg rmsg;
	ExPetStatusMsg(rmsg, pet);
}

void do_ExPetLearn(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int skillindex = -1;
	msg >> skillindex;

	if (ch->m_wearing[WEARING_PET] == NULL)
	{
		ExPetLearnMsg(rmsg, 0, 0, MSG_EX_PET_LEARN_ERROR_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CPet* pet = ch->GetPet();
	if (pet == NULL)
	{
		ExPetLearnMsg(rmsg, 0, 0, MSG_EX_PET_LEARN_ERROR_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	const CSkillProto* skillProto = gserver.m_skillProtoList.Find(skillindex);
	if (skillProto == NULL)
		return ;
	if (skillProto->m_type != ST_PET_COMMAND && skillProto->m_type != ST_PET_SKILL_PASSIVE && skillProto->m_type != ST_PET_SKILL_ACTIVE)
		return ;

	// 펫이 해당 스킬을 가지고 있는지 검사
	CSkill* curSkill = pet->FindSkill(skillindex);
	int nextlevel = (curSkill == NULL) ? 1 : curSkill->m_level + 1;

	const CSkillLevelProto* skillLevelProto = skillProto->Level(nextlevel);
	if (skillLevelProto == NULL)
		return ;
	if (skillProto->m_maxLevel < nextlevel)
		return ;

	// 조건 검사 : 직업
	if (!skillProto->IsSameJob(pet))
		return ;

	// 조건 검사 : 레벨
	if (skillLevelProto->m_learnLevel > pet->m_level)
	{
		ExPetLearnMsg(rmsg, 0, 0, MSG_EX_PET_LEARN_ERROR_LEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 조건 검사 : 포인트 : 기술 포인트는 SP와 달리 단귀가 1로 통일
	if (skillLevelProto->m_learnSP > pet->GetAbilityPoint())
	{
		ExPetLearnMsg(rmsg, 0, 0, MSG_EX_PET_LEARN_ERROR_POINT);
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
			ExPetLearnMsg(rmsg, 0, 0, MSG_EX_PET_LEARN_ERROR_ITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		item[i] = ch->m_invenNormal.GetItem(r, c);
		if (!item[i] || item[i]->Count() < skillLevelProto->m_learnItemCount[i])
		{
			ExPetLearnMsg(rmsg, 0, 0, MSG_EX_PET_LEARN_ERROR_ITEM);
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
		pet->AddSkill(curSkill);
	}
	else
	{
		// 기존 스킬 레벨 업
		curSkill->m_level++;
	}

	// 포인트 소모
	pet->DecreaseAbilityPoint(skillLevelProto->m_learnSP);
	ExPetStatusMsg(rmsg, pet);
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
	ExPetLearnMsg(rmsg, curSkill->m_proto->m_index, curSkill->m_level, MSG_EX_PET_LEARN_ERROR_OK);
	SEND_Q(rmsg, ch->m_desc);

	// 스킬 효과 적용
	ch->CalcStatus(true);

	// 게임 로그
	// TODO : petlog
	GAMELOG << init("PET SKILL LEARN", ch)
			<< "PET" << delim
			<< pet->m_index << delim
			<< pet->GetPetTypeGrade() << delim
			<< "SKILL" << delim
			<< curSkill->m_proto->m_index << delim
			<< curSkill->m_level
			<< end;
}

void do_ExPetResetSkill(CPC* ch, CNetMsg& msg)
{
	if (ch->m_wearing[WEARING_PET] == NULL)
		return ;

	if (ch->m_currentSkill)		// 080408 수정
		return ;

	CPet* pet = ch->GetPet();
	if (pet == NULL)
		return ;

	if (!pet->ResetSkill())
		return ;

	CNetMsg rmsg;

	ExPetResetSkillMsg(rmsg);
	SEND_Q(rmsg, ch->m_desc);
	ExPetStatusMsg(rmsg, pet);
	SEND_Q(rmsg, ch->m_desc);
	ExPetSkillListMsg(rmsg, pet);
	SEND_Q(rmsg, ch->m_desc);

	// 게임 로그
	// TODO : petlog
	GAMELOG << init("PET SKILL RESET", ch)
			<< "PET" << delim
			<< pet->m_index << delim
			<< pet->GetPetTypeGrade()
			<< end;
}

void do_ExPetChangeMount(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	// 착용중인 펫이 있나?
	CPet* pet = ch->GetPet();
	if (!pet)
	{
		ExPetChangeMountMsg(rmsg, MSG_EX_PET_CHANGE_MOUNT_ERROR_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 착용중인 펫의 레벨이 PET_MOUNT_LEVEL이상인가?
	if (pet->m_level < PET_MOUNT_LEVEL)
	{
		ExPetChangeMountMsg(rmsg, MSG_EX_PET_CHANGE_MOUNT_ERROR_LEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 현재 성인 상태인가
	if (pet->GetPetGrade() != PET_GRADE_ADULT)
	{
		ExPetChangeMountMsg(rmsg, MSG_EX_PET_CHANGE_MOUNT_ERROR_GRADE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

#ifdef PET_TURNTO_NPC	// 펫 변신 취소
	if( pet->GetPetTurnToNpc() > 0 )
	{
		pet->SetPetTurnToNpc(0);
		HelperPetTurnToNPCMsg(rmsg, ch->m_index, pet->m_index, pet->GetPetTurnToNpc() , pet->GetPetTurnToNpcSize() );
		SEND_Q(rmsg, gserver.m_helper );
	}
#endif //PET_TURNTO_NPC

	// 펫 Disappear 보내기
	pet->Disappear();

	// 펫 타입 변경
	pet->ChangeToMount();

	// 펫 스킬 초기화 : 포인트 환원
	pet->ResetSkill();

	pet->Mount(true);
	// 결과 전달
	ExPetChangeMountMsg(rmsg, MSG_EX_PET_CHANGE_MOUNT_ERROR_OK);
	SEND_Q(rmsg, ch->m_desc);

	// 펫 정보 전달
	ExPetStatusMsg(rmsg, pet);
	SEND_Q(rmsg, ch->m_desc);
	ExPetSkillListMsg(rmsg, pet);
	SEND_Q(rmsg, ch->m_desc);

	// PC 마운트 전달
	ExPetMountMsg(rmsg, ch->m_index, pet->GetPetTypeGrade());
#ifdef PET_DIFFERENTIATION_ITEM
	rmsg << pet->GetPetColor();
#endif // PET_DIFFERENTIATION_ITEM
#ifdef PET_TURNTO_NPC
	rmsg << pet->GetPetTurnToNpc();	
#endif //PET_TURNTO_NPC			
	ch->m_pArea->SendToCell(rmsg, ch, true);

	ch->m_bChangeStatus = true;

	// 게임 로그
	// TODO : petlog
	GAMELOG << init("PET CHANGE MOUNT", ch)
			<< "PET" << delim
			<< pet->m_index << delim
			<< pet->GetPetTypeGrade()
			<< end;
}

void do_ExPetCommand(CPC* ch, CNetMsg& msg)
{
	int petindex = -1;
	int commandindex = -1;
	char targettype = -1;
	int targetindex = -1;

	msg >> petindex
		>> commandindex
		>> targettype
		>> targetindex;

	CPet* pet = ch->GetPet();
	if (!pet || pet->m_index != petindex || !pet->IsSummon() || !pet->m_pArea)
		return ;

	CSkill* skill = pet->FindSkill(commandindex);
	if (!skill || skill->m_proto->m_type != ST_PET_COMMAND)
		return ;

	CNetMsg rmsg;

	ExPetCommandMsg(rmsg, petindex, commandindex, targettype, targetindex);
	pet->m_pArea->SendToCell(rmsg, pet, true);
}

void do_ExPetMixItem(CPC* ch, CNetMsg& msg)
{

	typedef struct __tagMixData
	{
		int needitem[3];		// 재료 3종
		int prob;				// 확률
		int itemindex;			// 생성 아이템
		int method;				// 생성 방법
	} PET_MIX_DATA;

	static const int nSizeMixTable = 74;

	static const PET_MIX_DATA mixTable[nSizeMixTable] = {
		{{895, 896, 897}, 70, 887,  0},				// 한손검
		{{896, 897, 898}, 70, 907,  0},				// 이도류
		{{897, 898, 899}, 70, 908,  0},				// 대검
		{{898, 899, 901}, 70, 909,  0},				// 도끼
		{{899, 901, 902}, 70, 910,  0},				// 힐러 완드
		{{901, 902, 903}, 70, 911,  0},				// 활
		{{902, 903, 904}, 70, 912,  0},				// 숏스테프
		{{903, 904, 905}, 70, 913,  0},				// 스테프
		{{904, 905, 900}, 70, 914,  0},				// 단검
		{{905, 900, 906}, 70, 915,  0},				// 석궁
		{{900, 906, 895}, 70, 916,  0},				// 사이드
		{{906, 895, 896}, 70, 917,  0},				// 폴암
		{{886, 889, 894}, 25, 918,  0},				// 투구
		{{888, 891, 894}, 75, 918,  1},				// 투구
		{{891, 892, 894}, 25, 919,  0},				// 상의
		{{893, 889, 894}, 75, 919,  1},				// 상의
		{{886, 891, 894}, 25, 920,  0},				// 하의
		{{890, 892, 894}, 75, 920,  1},				// 하의
		{{889, 892, 894}, 25, 921,  0},				// 부츠
		{{890, 886, 894}, 75, 921,  1},				// 부츠
		{{889, 892, 894}, 25, 922,  0},				// 장갑
		{{888, 886, 894}, 75, 922,  1},				// 장갑
		{{886, 889, 894}, 25, 923,  0},				// 투구
		{{888, 891, 894}, 75, 923,  1},				// 투구
		{{891, 892, 894}, 25, 924,  0},				// 상의
		{{893, 889, 894}, 75, 924,  1},				// 상의
		{{886, 891, 894}, 25, 925,  0},				// 하의
		{{890, 892, 894}, 75, 925,  1},				// 하의
		{{889, 892, 894}, 25, 926,  0},				// 부츠
		{{890, 886, 894}, 75, 926,  1},				// 부츠
		{{889, 892, 894}, 25, 927,  0},				// 장갑
		{{888, 886, 894}, 75, 927,  1},				// 장갑
		{{886, 892, 894}, 25, 1280, 0},				// 방패
		{{893, 891, 894}, 75, 1280, 1},				// 방패
		{{886, 889, 894}, 25, 928,  0},				// 투구
		{{888, 891, 894}, 75, 928,  1},				// 투구
		{{891, 892, 894}, 25, 929,  0},				// 상의
		{{893, 889, 894}, 75, 929,  1},				// 상의
		{{886, 891, 894}, 25, 930,  0},				// 하의
		{{890, 892, 894}, 75, 930,  1},				// 하의
		{{889, 892, 894}, 25, 931,  0},				// 부츠
		{{890, 886, 894}, 75, 931,  1},				// 부츠
		{{889, 892, 894}, 25, 932,  0},				// 장갑
		{{888, 886, 894}, 75, 932,  1},				// 장갑
		{{886, 889, 894}, 25, 933,  0},				// 투구
		{{888, 891, 894}, 75, 933,  1},				// 투구
		{{891, 892, 894}, 25, 934,  0},				// 상의
		{{893, 889, 894}, 75, 934,  1},				// 상의
		{{886, 891, 894}, 25, 935,  0},				// 하의
		{{890, 892, 894}, 75, 935,  1},				// 하의
		{{889, 892, 894}, 25, 936,  0},				// 부츠
		{{890, 886, 894}, 75, 936,  1},				// 부츠
		{{889, 892, 894}, 25, 937,  0},				// 장갑
		{{888, 886, 894}, 75, 937,  1},				// 장갑
		{{886, 889, 894}, 25, 938,  0},				// 투구
		{{888, 891, 894}, 75, 938,  1},				// 투구
		{{891, 892, 894}, 25, 939,  0},				// 상의
		{{893, 889, 894}, 75, 939,  1},				// 상의
		{{886, 891, 894}, 25, 940,  0},				// 하의
		{{890, 892, 894}, 75, 940,  1},				// 하의
		{{889, 892, 894}, 25, 941,  0},				// 부츠
		{{890, 886, 894}, 75, 941,  1},				// 부츠
		{{889, 892, 894}, 25, 942,  0},				// 장갑
		{{888, 886, 894}, 75, 942,  1},				// 장갑
		{{886, 889, 894}, 25, 943,  0},				// 투구
		{{888, 891, 894}, 75, 943,  1},				// 투구
		{{891, 892, 894}, 25, 944,  0},				// 상의
		{{893, 889, 894}, 75, 944,  1},				// 상의
		{{886, 891, 894}, 25, 945,  0},				// 하의
		{{890, 892, 894}, 75, 945,  1},				// 하의
		{{889, 892, 894}, 25, 946,  0},				// 부츠
		{{890, 886, 894}, 75, 946,  1},				// 부츠
		{{889, 892, 894}, 25, 947,  0},				// 장갑
		{{888, 886, 894}, 75, 947,  1},				// 장갑
	};

	CNetMsg rmsg;

	int		nWantItemIndex = 0;		// 만들고자 하는 아이템
	int		nMethod = 0;			// 만드는 방법
	int		nIndexTable = 0;		// 해당 제작 방법 인덱스
	int		nLoopItem;				// 재료 아이템 루프용
	CItem*	pItem[3];				// 재료 아이템

	msg >> nWantItemIndex
		>> nMethod;

	// 테이블에서 검색
	for (nIndexTable = 0; nIndexTable < nSizeMixTable; nIndexTable++)
	{
		// 만들고자 하는 아이템과 방법으로 검색
		if (mixTable[nIndexTable].itemindex == nWantItemIndex && mixTable[nIndexTable].method == nMethod)
			break;
	}

	// 찾는 것이 없으면
	if (nIndexTable == nSizeMixTable)
	{
		ExPetMixItemMsg(rmsg, MSG_EX_PET_MIX_ITEM_ERROR_NOMIX);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 해당 재료를 찾음
	for (nLoopItem = 0; nLoopItem < 3; nLoopItem++)
	{
		int r, c;
		if (ch->m_invenNormal.FindItem(&r, &c, mixTable[nIndexTable].needitem[nLoopItem], 0, 0))
		{
			pItem[nLoopItem] = ch->m_invenNormal.GetItem(r, c);
			if (pItem[nLoopItem] == NULL)
			{
				ExPetMixItemMsg(rmsg, MSG_EX_PET_MIX_ITEM_ERROR_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
		else
		{
			ExPetMixItemMsg(rmsg, MSG_EX_PET_MIX_ITEM_ERROR_NOITEM);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	// 재료 감소
	for (nLoopItem = 0; nLoopItem < 3; nLoopItem++)
	{
		if (pItem[nLoopItem]->Count() > 1)
		{
			// 감소
			DecreaseFromInventory(ch, pItem[nLoopItem], 1);
			ItemUpdateMsg(rmsg, pItem[nLoopItem], -1);
		}
		else
		{
			// 삭제
			ItemDeleteMsg(rmsg, pItem[nLoopItem]);
			RemoveFromInventory(ch, pItem[nLoopItem], true, true);
		}
		SEND_Q(rmsg, ch->m_desc);
	}

	// 확률 적용
	if (GetRandom(1, 100) <= mixTable[nIndexTable].prob)
	{
		// 아이템 생성
		CItem* pMixItem = gserver.m_itemProtoList.CreateItem(nWantItemIndex, -1, 0, 0, 1);
		bool bDrop = false;
		if (pMixItem)
		{
			GAMELOG << init("ITEM MIX PET", ch)
					<< "CONSUME" << delim
					<< mixTable[nIndexTable].needitem[0] << delim
					<< mixTable[nIndexTable].needitem[1] << delim
					<< mixTable[nIndexTable].needitem[2] << delim
					<< "NEW ITEM" << delim
					<< itemlog(pMixItem) << delim;

			if (AddToInventory(ch, pMixItem, true, true))
			{
				if (pMixItem->tab() < 0)
				{
					int r, c;
					if (ch->m_invenNormal.FindItem(&r, &c, pMixItem->m_idNum, 0, 0))
					{
						delete pMixItem;
						pMixItem = ch->m_invenNormal.GetItem(r, c);
						ItemUpdateMsg(rmsg, pMixItem, 1);
						SEND_Q(rmsg, ch->m_desc);
					}
				}
				else
				{
					ItemAddMsg(rmsg, pMixItem);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
			else
			{
				ch->m_pArea->DropItem(pMixItem, ch);
				pMixItem->m_preferenceIndex = ch->m_index;
				ItemDropMsg(rmsg, ch, pMixItem);
				ch->m_pArea->SendToCell(rmsg, GET_YLAYER(pMixItem), pMixItem->m_cellX, pMixItem->m_cellZ);
				bDrop = true;
			}

			GAMELOG << ((bDrop) ? "DROP" : "GIVE")
					<< end;

			ExPetMixItemMsg(rmsg, MSG_EX_PET_MIX_ITEM_ERROR_OK);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			GAMELOG << init("ITEM MIX PET : FATAL ERROR", ch)
					<< "CONSUME" << delim
					<< mixTable[nIndexTable].needitem[0] << delim
					<< mixTable[nIndexTable].needitem[1] << delim
					<< mixTable[nIndexTable].needitem[2]
					<< end;

			ExPetMixItemMsg(rmsg, MSG_EX_PET_MIX_ITEM_ERROR_FAIL);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else
	{
		// 실패 알림
		ExPetMixItemMsg(rmsg, MSG_EX_PET_MIX_ITEM_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExPetWarpTown(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (!ch->GetPet() || !ch->GetPet()->IsMount())
		return ;

	int zone, extra;
	int i = gserver.FindNearestZone(ch->m_pZone->m_index, GET_X(ch), GET_Z(ch), &zone, &extra);
	if (i == -1)
		return ;

	// 동일 싱글 던전으로는 이동 불가
	if ((ch->m_pZone->IsPersonalDungeon() || ch->m_pZone->IsGuildRoom()) && ch->m_pZone->m_index == zone)
		return ;

	CZone* pZone = gserver.m_zones + i;

	if (extra < 0 || extra >= pZone->m_countZonePos)
		return ;

	GoZone(ch, zone,
			pZone->m_zonePos[extra][0],														// ylayer
			GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
			GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
}

void do_ExPetChangeItem(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	// 펫을 착용하고 있는가?
	CPet* pet = ch->GetPet();
	if (!pet)
	{
		ExPetChangeItemMsg(rmsg, MSG_EX_PET_CHANGE_ITEM_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 펫을 아이템으로 교환
	bool bSuccess = DropPetItem(pet);

	// TODO : petlog
	GAMELOG << init("PET CHANGE ITEM", ch)
			<< "PET" << delim
			<< pet->m_index << delim
			<< "TYPE" << delim
			<< pet->GetPetType() << delim
			<< "LEVEL" << delim
			<< pet->m_level << delim
			<< "SUCCESS" << delim
			<< ((bSuccess) ? 1 : 0)
			<< end;

	// 펫 제거
	ch->DelPet(pet->m_index);

	if (bSuccess)
		ExPetChangeItemMsg(rmsg, MSG_EX_PET_CHANGE_ITEM_ERROR_OK);
	else
		ExPetChangeItemMsg(rmsg, MSG_EX_PET_CHANGE_ITEM_ERROR_NOITEM);

	ch->SaveNow(false);

	SEND_Q(rmsg, ch->m_desc);
}

void do_ExPetRebirth(CPC* ch, CNetMsg& msg)
{
	CNetMsg		rmsg;
	int			nPetIndex = 0;
	CPet*		pet;
	LONGLONG	nNeedNas;

	msg >> nPetIndex;

	pet = ch->GetPet(nPetIndex);

	if (pet == NULL)
	{
		// 펫 없으면 오류
		ExPetRebirthMsg(rmsg, nPetIndex, MSG_EX_PET_REBIRTH_ERROR_NOPET);
	}
	else
	{
		// 펫이 있으면
		if (pet->GetRemainRebirthTime() < 1)
		{
			// 살아 있으면 오류
			ExPetRebirthMsg(rmsg, nPetIndex, MSG_EX_PET_REBIRTH_ERROR_NODEAD);
		}
		else
		{
			bool bRebirth = true;

			// 필요나스 구함
			nNeedNas = (pet->m_level - 1) * pet->m_level * pet->m_level * 7;

			if (nNeedNas > 0)
			{
				// 나스가 필요하면 소지금과 비교
				if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < nNeedNas)
				{
					// 소지금 부족하면 오류
					ExPetRebirthMsg(rmsg, nPetIndex, MSG_EX_PET_REBIRTH_ERROR_NOMONEY);
					bRebirth = false;
				}
				else
				{
					// 소지금 충분하면 비용 적용
					if (ch->m_moneyItem->Count() > nNeedNas)
					{
						DecreaseFromInventory(ch, ch->m_moneyItem, nNeedNas);
						ItemUpdateMsg(rmsg, ch->m_moneyItem, -nNeedNas);
						SEND_Q(rmsg, ch->m_desc);
					}
					else
					{
						ItemDeleteMsg(rmsg, ch->m_moneyItem);
						SEND_Q(rmsg, ch->m_desc);
						RemoveFromInventory(ch, ch->m_moneyItem, true, true);
					} // 나스 소비

				} // 소지 나스 검사

			} // 필요 나스 검사

			if (bRebirth)
			{
				// 비용 적용후 펫 부활
				pet->SetRemainRebirthTime(0);
				pet->m_hp = 30;
				ExPetStatusMsg(rmsg, pet);
				SEND_Q(rmsg, ch->m_desc);

				// TODO : petlog
				GAMELOG << init("PET REBIRTH", ch)
						<< "PET" << delim
						<< pet->m_index << delim
						<< "TYPE" << delim
						<< pet->GetPetType() << delim
						<< "LEVEL" << delim
						<< pet->m_level << delim
						<< "MONEY" << delim
						<< nNeedNas
						<< end;

				ExPetRebirthMsg(rmsg, nPetIndex, MSG_EX_PET_REBIRTH_ERROR_OK);
			}

		} // 펫 사망 검사

	} // 펫 존재 검사

	SEND_Q(rmsg, ch->m_desc);
}

#ifdef PET_NAME_CHANGE
void do_PetNameChange(CPC* ch, CNetMsg& msg)
{
	CNetMsg		rmsg;
	int			nPetIndex = 0;
	CLCString	strPetName( 30 );
	CPet*		pet;

	msg >> nPetIndex
		>> strPetName;

	bool bApet = false;
		
	if( strlen( strPetName ) < 4 || strlen(strPetName) > 16  )
	{
		PetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_FAIL, nPetIndex, strPetName );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if( strinc( strPetName, "'" ) || strinc( strPetName, " " ) )
	{
		PetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_FAIL, nPetIndex, strPetName );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	int r, c;
	if( !ch->m_invenNormal.FindItem(  &r, &c, 2360, 0, 0 ) )
	{
		PetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_ITEM, nPetIndex, strPetName );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}
#ifdef ATTACK_PET
	CAPet* apet = ch->GetAPet();
	if( apet )
		bApet = true;		
#endif // ATTACK_PET

	pet = ch->GetPet( nPetIndex );
	if( !bApet && pet == NULL )
	{
		
		PetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_NOPET, nPetIndex, strPetName );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

#ifdef ATTACK_PET
	if( bApet )
	{
		CItem* pItem = ch->m_invenNormal.GetItem( r, c );
		if( pItem )
		{
			if (pItem->Count() > 1)
			{
				DecreaseFromInventory(ch, pItem, 1);
				ItemUpdateMsg(rmsg, pItem, -1);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemDeleteMsg(rmsg, pItem);
				SEND_Q(rmsg, ch->m_desc);
				ch->RemoveItemFromQuickSlot(pItem);
				RemoveFromInventory(ch, pItem, true, true);
			}

			apet->m_name = strPetName;
		}
	}
	else
#endif // ATTACK_PET
	{
		HelperPetNameChange( rmsg, ch->m_index, pet->m_index, strPetName );
		SEND_Q( rmsg, gserver.m_helper );
	}
}

#endif // PET_NAME_CHANGE

#endif // #ifdef ENABLE_PET
