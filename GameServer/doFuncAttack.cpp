#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Battle.h"
#include "DratanCastle.h"

// 믈리 어택
//N1InPersonalDungeon : BW

void do_pd_Attack(CPC* pc, CNetMsg& msg)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(pc);
	}
#endif // DRATAN_CASTLE

	//MSG_PD_ATTACK	: targetchartype(c) targetindex(n) attackType(c) multicount(c) multiindex(n:multicount) 
	char tCharType, attackType, multicount = 0;
	int tIndex, multiIndex = -1;

	msg >> tCharType
		>> tIndex
		>> attackType
		>> multicount;

	if (multicount > 20) multicount = 20;

	// 대상 검색 : 인접 셀에서만
	CArea* area = pc->m_pArea;
	if (!area) return ;
	
	CCharacter* tch = area->FindCharInCell(pc, tIndex, (MSG_CHAR_TYPE)tCharType);
	if(!tch) return;

	int preIndex = -1;
	do
	{
		multicount--;
		if(multicount < 0) return ;

		preIndex = multiIndex;

		msg >> multiIndex;

		if(preIndex == multiIndex)
		{
			// 가까운 마을로
			int nearZone;
			int nearZonePos;
			int i = gserver.FindNearestZone(pc->m_pZone->m_index, GET_X(pc), GET_Z(pc), &nearZone, &nearZonePos);
			if (i == -1)
				return ;
			
			CZone* pZone = gserver.m_zones + i;
			
			GoZone(pc, nearZone,
				pZone->m_zonePos[nearZonePos][0],															// ylayer
				GetRandom(pZone->m_zonePos[nearZonePos][1], pZone->m_zonePos[nearZonePos][3]) / 2.0f,		// x
				GetRandom(pZone->m_zonePos[nearZonePos][2], pZone->m_zonePos[nearZonePos][4]) / 2.0f);		// z

			return;
		}

		CCharacter* ch = area->FindCharInCell(pc, multiIndex, (MSG_CHAR_TYPE) MSG_CHAR_NPC);

		if(!ch) continue;

		if( !IS_NPC(ch) )
		{
			CPC* bugPC = NULL;
			if( IS_ELEMENTAL(ch) )
			{
				CElemental *ele = TO_ELEMENTAL(ch);
				bugPC = ele->GetOwner();
			}
#ifdef ENABLE_PET
			if( IS_PET(ch) )
			{
				CPet* pet = TO_PET(ch);
				bugPC = pet->GetOwner();
			}
#endif
			 if( IS_PC(ch) )
			 {
				 bugPC = TO_PC(ch);
			 }

			 if( !bugPC )
				 return;

			// 가까운 마을로
			int nearZone;
			int nearZonePos;
			int i = gserver.FindNearestZone(bugPC->m_pZone->m_index, GET_X(bugPC), GET_Z(bugPC), &nearZone, &nearZonePos);
			if (i == -1)
				return ;
			
			CZone* pZone = gserver.m_zones + i;
			
			GoZone(bugPC, nearZone,
				pZone->m_zonePos[nearZonePos][0],															// ylayer
				GetRandom(pZone->m_zonePos[nearZonePos][1], pZone->m_zonePos[nearZonePos][3]) / 2.0f,		// x
				GetRandom(pZone->m_zonePos[nearZonePos][2], pZone->m_zonePos[nearZonePos][4]) / 2.0f);		// z

			GAMELOG << init("PD_BUG", bugPC)
					<< end;
			return;
		}

		int ret;
		ret = ProcAttack(ch, tch, ch->GetAttackType(NULL), NULL, 0);
		if (ret == -1)
			return ;
	}while(multicount);

}


void do_Attack(CPC* pc, CNetMsg& msg)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(pc);
	}
#endif // DRATAN_CASTLE

	char tCharType, aCharType, attackType, multicount = 0;
	int tIndex, aIndex;

	msg >> aCharType
		>> aIndex
		>> tCharType
		>> tIndex
		>> attackType
		>> multicount;

	// 동시 공격은 최대 5
	if (multicount > 5)
		multicount = 5;

	// 대상 검색 : 인접 셀에서만
	CArea* area = pc->m_pArea;
	if (!area) return ;
	CCharacter* ch = area->FindCharInCell(pc, aIndex, (MSG_CHAR_TYPE)aCharType);
	if (ch == NULL)
		return ;
	CCharacter* tch = area->FindCharInCell(ch, tIndex, (MSG_CHAR_TYPE)tCharType);
	if (tch == NULL)
		return ;

	// 공격자가 PC이면 자신의 캐릭만 조정
	if (IS_PC(ch) && ch != pc)
		return ;

	switch (ch->m_type)
	{
	case MSG_CHAR_PC:
		{
			// 공격 거리 검사
			if (GetDistance(ch, tch) > ch->m_attackRange * 2)
				return ;

			// 공속 검사
			if ( ch->ChekAttackType() && ch->CheckHackAttack(pc))
				return ;

			// 펫 타고 있으면 불가능
#ifdef ENABLE_PET
			if (pc->GetPet() && pc->GetPet()->IsMount())
				return ;
#endif // #ifdef ENABLE_PET

#ifdef ENABLE_WAR_CASTLE
			// 공성 아이템 착용시 해당 스킬 발동
			int mixSkillIndex[] = { pc->m_opSturnIndex, pc->m_opBloodIndex, pc->m_opPoisonIndex, pc->m_opSlowIndex, pc->m_opMoveIndex };
			int mixSkillLevel[] = { pc->m_opSturnLevel, pc->m_opBloodLevel, pc->m_opPoisonLevel, pc->m_opSlowLevel, pc->m_opMoveLevel };
			CSkill* skillMixItem = NULL;
			int i;
			int bStop = 0;
			for (i = 0; i < 5; i++)
			{
				if (mixSkillIndex[i] > 0 && mixSkillLevel[i] > 0)
				{
					skillMixItem = gserver.m_skillProtoList.Create(mixSkillIndex[i], mixSkillLevel[i]);
					if (skillMixItem)
					{
						bool bApply;
						bStop = ApplySkill(ch, tch, skillMixItem, -1, bApply);
					}
					delete skillMixItem;
					skillMixItem = NULL;
					if (bStop != 0)
						return ;
				}
			}
#endif // ENABLE_WAR_CASTLE

			if (IS_PC(ch))
			{
				CPC* pPCAttacker = TO_PC(ch);

				// 암흑 공격
				if (pPCAttacker->m_opAttackBlind > 0)
				{
					CSkill* pSkillBlind = gserver.m_skillProtoList.Create(415, pPCAttacker->m_opAttackBlind);
					int nRetApplySkill = 0;
					if (pSkillBlind)
					{
						bool bApply;
						nRetApplySkill = ApplySkill(ch, tch, pSkillBlind, -1, bApply);
					}
					delete pSkillBlind;
					pSkillBlind = NULL;
					if (nRetApplySkill != 0)
						return ;
				}

				// 독 공격
				if (pPCAttacker->m_opAttackPoison > 0)
				{
					CSkill* pSkillPoison = gserver.m_skillProtoList.Create(414, pPCAttacker->m_opAttackPoison);
					int nRetApplySkill = 0;
					if (pSkillPoison)
					{
						bool bApply;
						nRetApplySkill = ApplySkill(ch, tch, pSkillPoison, -1, bApply);
					}
					delete pSkillPoison;
					pSkillPoison = NULL;
					if (nRetApplySkill != 0)
						return ;
				}
			}
		} // PC 검사

		break;


	case MSG_CHAR_PET:
	case MSG_CHAR_ELEMENTAL:
		// TODO : 소환수 공속 검사
		// 공격 거리 검사
		if (GetDistance(ch, tch) > ch->m_attackRange * 2)
			return ;

		// 공속 검사
		if (ch->CheckHackAttack(pc))
			return ;

		break;

	default:
		break;
	}

	if (DEAD(ch) || TO_PC(pc)->m_bImmortal || !ch->CanAttack() || pc->IsDisable())	return ;
#ifdef NIGHTSHADOW_SKILL
	// 결계걸린 대상은 공격 못한다.
	if ( tch->m_assist.m_state & AST_FREEZE )	return;
#endif // NIGHTSHADOW_SKILL

	// 대상이 NPC일때만 멀티 공격
	if (!IS_NPC(tch))
		multicount = 0;

	// 최소한 공격 1회 이상
	bool bAttacked = false;
	bool bAttackedPet = false;		// 애완동물은 NPC상대시 레벨 검사

	// 멀티 공격 검사용
	CLCList<int> listMultiTarget(NULL);

	int multitarget;
	while (tch)
	{
		bool bBlocked = false;

		// NPC를 공격할 때에만 속성맵 검사
		if (IS_NPC(tch))
		{
			char tempy = GET_YLAYER(ch);
			bBlocked = (!area->IsNotBlocked(ch, tch, true, tempy));
		}

		int ret = 0;
		if (!bBlocked)
		{
			if (IS_PC(ch) && ch->IsEnemy(tch))
			{
				bAttacked = true;
#ifdef MONSTER_AI
				if (tch != NULL	&& IS_NPC(tch))
				{
					CNPC * pTemp = TO_NPC(tch);
					if (pTemp != NULL)
					{
						if (ch->m_level - tch->m_level <= 5 && pTemp->m_proto->m_index != 303 /*악마의 묘지*/)
						{
							bAttackedPet = true;
						}

						if (pTemp->m_bMoveLock || pTemp->m_bMoveToRegen)
						{
							pTemp->m_bMoveToRegen = false;
							pTemp->m_bMoveLock = false;
							pTemp->m_pulseMoveLock = 0;
							pTemp->m_postregendelay = 0;
						}

#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
						// 임시로 테스트를 위한 설정, 한대 때리면 테이밍 됨
						pTemp->SetOwner(pc);
						pTemp->Set_MobFlag( STATE_MONSTER_TAMING );
#endif  // NIGHTSHADOW_SKILL	
					}
				}
#else
				if (tch != NULL	&& IS_NPC(tch) && ch->m_level - tch->m_level <= 5)
				{
					CNPC * pTemp = TO_NPC(tch);
					if (pTemp != NULL && pTemp->m_proto->m_index != 303 /*악마의 묘지*/)
					{
						bAttackedPet = true;
					}

#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
						// 임시로 테스트를 위한 설정, 한대 때리면 테이밍 됨
						pTemp->SetOwner(pc);
						pTemp->Set_MobFlag( STATE_MONSTER_TAMING );
#endif  // NIGHTSHADOW_SKILL	

				}
#endif
			}
			listMultiTarget.AddToTail(tch->m_index);
			ret = ProcAttack(ch, tch, ch->GetAttackType(NULL), NULL, 0);
		}

		if (ret == -1)
		{
			tch = NULL;
			continue ;
		}

		if (area->m_zone->IsPersonalDungeon())
		{
			if (multicount && !DEAD(ch))
			{
				multicount--;
				msg >> multitarget;
				tch = area->FindCharInCell(ch, multitarget, (MSG_CHAR_TYPE)tCharType);
				if (tch)
				{
					if (GetDistance(ch, tch) > ch->m_attackRange * 2)
						tch = NULL;
					if (tch && listMultiTarget.FindData(tch->m_index) != NULL)
					{
						GAMELOG << init("HACK ATTACK MULTI TARGET", pc)
								<< "ZONE" << delim
								<< pc->m_pZone->m_index << delim
								<< "TARGET" << delim
								<< tCharType << delim
								<< multitarget
								<< end;
						if (pc->m_desc->IncreaseHackCount(1))
							return ;
						tch = NULL;
					}
				}
			}
			else
				tch = NULL;
		}
		else
			tch = NULL;
	}

#ifdef EVENT_NEWYEAR_2006_TIME
	// 공격시 이벤트 시간 갱신 검사
	if (gserver.m_bNewYear2006Event && bAttacked && !ch->IsInPeaceZone(true))
		pc->m_pulseEventNewYear2006 = gserver.m_pulse;
#endif // #ifdef EVENT_NEWYEAR_2006_TIME

#ifdef ENABLE_PET
	if (bAttackedPet && !ch->IsInPeaceZone(true))
		pc->m_pulseLastAttackSkill = gserver.m_pulse;
#endif // #ifdef ENABLE_PET

#ifdef EVENT_SEARCHFRIEND_TIME
	// 공격시 이벤트 시간 갱신 검사
	if (gserver.m_bSearchFriendEvent && (pc->m_nEventSearchFriendListCount >= 1)
		&& (pc->m_bEventSearchFriendSelect == true) && (pc->m_nTimeEventSearchFriend <= 216000) 
		&& bAttacked && !ch->IsInPeaceZone(true))
		pc->m_pulseEventSearchFriend = gserver.m_pulse;
#endif // #ifdef EVENT_SEARCHFRIEND_TIME
}
