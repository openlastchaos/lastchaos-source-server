#include "stdhdrs.h"
#include "CmdMsg.h"
#include "Server.h"
#include "Exp.h"
#include "Log.h"
#include "Battle.h"
#include "hardcoding.h"
#include "WarCastle.h"
#include "doFunc.h"

///////////////////
// 공격 관련 함수들

int ProcAttack(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE damageType, CSkill* skill, int magicindex, int damage)
{
	// 방어자가 공격 당할 수 있는 상태인지 검사
	if (!IsAvailableAttack(of, df))
		return 0;

	// 공격 종류 구분
	if (damageType == MSG_DAMAGE_UNKNOWN)
		return 0;

	const CSkillProto* proto = NULL;
	const CSkillLevelProto* levelproto = NULL;
	const CMagicProto* magic = NULL;
	const CMagicLevelProto* magiclevel = NULL;

	if (skill)
	{
		proto = skill->m_proto;
		if (!GetSkillPrototypes(proto, skill->m_level, magicindex, &levelproto, &magic, &magiclevel))
			return 0;
	}

	// 방어자 공격에 따른 버프 취소
	df->m_assist.CancelSleep();
	df->m_assist.CancelFear();
	df->CancelInvisible();
#ifdef ADULT_SERVER_NEWITEM
	df->m_assist.CancelMantle();
	of->m_assist.CancelMantle();
#endif // ADULT_SERVER_NEWITEM

	// 공격자 공격에 따른 버프 취소
	of->CancelInvisible();

	char flag;
	switch (damageType)
	{
	case MSG_DAMAGE_REFLEX:
	case MSG_DAMAGE_LINK:
		flag = HITTYPE_NORMAL;
		if (damage == 0)
			return 0;
		break;

	default:
		// 명중/회피/크리티컬 구분 플래그
		flag = GetHitType(of, df, damageType);
		// 명중/회피/크리티컬 검사
		if (flag != HITTYPE_MISS)
			flag = SelectHitType(of, df, damageType, flag, magic, magiclevel, true);
		// 데미지 계산
		damage = GetDamage(of, df, damageType, flag, magic, magiclevel);
		break;
	}

	ApplyHateByDamage(of, df, flag, damage);
	ApplyDamage(of, df, damageType, proto, magic, damage, flag);

	if (flag != HITTYPE_MISS)
		ProcAfterHit(of, df);

	if (DEAD(df))
	{
		switch (df->m_type)
		{
		case MSG_CHAR_PC:
			ProcDead(TO_PC(df), of);
			break;
		case MSG_CHAR_NPC:
			ProcDead(TO_NPC(df), of);
			break;
		case MSG_CHAR_PET:
			ProcDead(TO_PET(df), of);
			break;
		case MSG_CHAR_ELEMENTAL:
			ProcDead(TO_ELEMENTAL(df), of);
			break;
#ifdef ATTACK_PET
		case MSG_CHAR_APET:
			ProcDead(TO_APET(df), of );
			break;
#endif //ATTACK_PET
		default:
			break;
		}

		// 여기 오면 df가 메모리에서 해재 될 수 있다
		// 그래서 아예 NULL로 바꿔버린다
		df = NULL;

		return 1;
	}
	else
	{
		// 살아 남으면 데미지 반사를 검사
		if (damageType != MSG_DAMAGE_REFLEX && GetDistance(of, df) <= 25)
		{
			int reflexdam = 0;
			if (GetRandom(1, 10000) <= df->m_avPassiveAddition.hcReflexProb)
			{
				reflexdam += df->m_avPassiveAddition.hcReflex;
				reflexdam += damage * df->m_avPassiveRate.hcReflex / SKILL_RATE_UNIT;
			}
			if (GetRandom(1, 10000) <= df->m_assist.m_avAddition.hcReflexProb)
			{
				reflexdam += df->m_assist.m_avAddition.hcReflex;
				reflexdam += damage * df->m_assist.m_avRate.hcReflex / SKILL_RATE_UNIT;
			}
			if (reflexdam)
			{
				if (ProcAttack(df, of, MSG_DAMAGE_REFLEX, NULL, 0, reflexdam) == 1)
					return -1;
			}
		}
		
#ifdef NIGHTSHADOW_SKILL
		// 테이밍 몬스터는 주인이 때리는 타겟만 때려야 함
		// 여기서 pc가 때리는 npc 저장
		if ( IS_PC(of) && IS_NPC(df) )
		{
			CPC* temp = TO_PC(of);
			temp->SetOwners_target(df);
		}
#endif // NIGHTSHADOW_SKILL
	}

	return 0;
}

// ch, tch가 CELL_EXT 범위 안에 있는가 있으면 true
bool CheckInNearCellExt(CCharacter* ch, CCharacter* tch)
{
	// 디버깅용
	int chType = ch->m_type;
	int tchType = tch->m_type;
	chType = chType;
	tchType = tchType;
	if (ch == NULL)
		return false;
	if (tch == NULL)
		return false;
	if (ch->m_pZone == NULL)
		return false;
	if (tch->m_pZone == NULL)
		return false;
	if (ch->m_pArea == NULL)
		return false;
	if (ch->m_pZone == NULL)
		return false;
	if (ch->m_pZone->m_index != tch->m_pZone->m_index)
		return false;
	if (ch->m_pArea->m_index != tch->m_pArea->m_index)
		return false;

#ifdef MONSTER_COMBO
	if (ch->m_pos.m_yLayer != tch->m_pos.m_yLayer)
		return false;
#endif // MONSTER_COMBO

	if (ABS(ch->m_cellX - tch->m_cellX) <= CELL_EXT && ABS(ch->m_cellZ - tch->m_cellZ) <= CELL_EXT)
		return true;
	
	return false;
}

// npc의 어택리스트중 가장 많은 데미지를 준 pc 반환
CAttackChar* FindMaxDamage(CNPC* npc)
{
	CAttackChar* ret = NULL;
	CAttackChar* attackCh = npc->m_attackList;
	int dam = 0;
	
	while (attackCh)
	{
		if (attackCh->ch)
		{
			// 같은 zone에 없거나 같은 area에 없거나 CELL_EXT(+-5) 범위 안에 없거나
			if ((npc->m_pZone->m_index != attackCh->ch->m_pZone->m_index) || 
				(npc->m_pArea->m_index != attackCh->ch->m_pArea->m_index) ||
				!CheckInNearCellExt(npc, attackCh->ch))
				break;
			
			if (dam <= attackCh->m_damage)
			{
				dam = attackCh->m_damage;
				ret = attackCh;
			}
		}
		attackCh = attackCh->m_next;
	}
	
	return ret;
}

// 우선권 PC 찾기
// target list들의 평균 레벨 구하기
// 찾을때 사용할 데이터
typedef struct __tagPreferencePCData
{
	CPC*		m_pc;		// PC 포인터
	int			m_damage;	// 해당 PC의 데미지

	static int Comp(struct __tagPreferencePCData* d1, struct __tagPreferencePCData* d2)
	{
		if (d1->m_pc == d2->m_pc)
			return 0;
		else
			return -1;
	}
} PPDATA;

#ifdef NEW_DIVISION_EXPSP
CPC* FindPreferencePC(CNPC* npc, int* level, LONGLONG* pnTotalDamage)
#else // #ifdef NEW_DIVISION_EXPSP
CPC* FindPreferencePC(CNPC* npc, int* level)
#endif // #ifdef NEW_DIVISION_EXPSP
{
	CPC* ret = NULL;
	CAttackChar* target = npc->m_attackList;

	int dam = 0;
#ifdef NEW_DIVISION_EXPSP
	*pnTotalDamage = npc->GetTotalDamage();
	LONGLONG nPreferDamage = *pnTotalDamage + npc->m_maxHP * 2 / 10;		// 최대 대미지와 NPC의 최대HP 20%를 합한 값을 아이템 우선권에 사용
#else // #ifdef NEW_DIVISION_EXPSP
	int totalDam = npc->m_totalDamage + (npc->m_maxHP * 20 / 100);
#endif // #ifdef NEW_DIVISION_EXPSP
	int totLevel = 0;
	int	count = 0;

	CLCList<PPDATA*> listAttackPC(PPDATA::Comp);	// NPC 공격에 가담한 PC 리스트
	while (target)
	{
		if (target->ch)
		{
			// 디버깅용
			int npcType = npc->m_type;
			int tchType = target->ch->m_type;
			npcType = npcType;
			tchType = tchType;
			if (npc== NULL)
				break;
			if (target->ch == NULL)
				break;
			if (npc->m_pZone == NULL)
				break;
			if (target->ch->m_pZone == NULL)
				break;
			if (target->ch->m_pArea == NULL)
				break;
			if (npc->m_pZone == NULL)
				break;
			// 같은 zone에 없거나 같은 area에 없거나 CELL_EXT(+-5) 범위 안에 없거나
			if ((npc->m_pZone->m_index != target->ch->m_pZone->m_index) || 
				(npc->m_pArea->m_index != target->ch->m_pArea->m_index) ||
				!CheckInNearCellExt(npc, target->ch))
				break;

			dam = target->m_damage;

			// 선공 pc는 NPC 최대 HP의  20% 보너스!!
			if (target->m_next == NULL)
				dam += npc->m_maxHP * 2 / 10;

			PPDATA ppdata = {NULL, 0};
			switch (target->ch->m_type)
			{
			case MSG_CHAR_PC:
				ppdata.m_pc = target->pc;
				break;

#ifdef ENABLE_PET
			case MSG_CHAR_PET:
				if (TO_PET(target->ch)->GetOwner())
					ppdata.m_pc = TO_PET(target->ch)->GetOwner();
				break;
#endif // ENABLE_PET

			case MSG_CHAR_ELEMENTAL:
				if (TO_ELEMENTAL(target->ch)->GetOwner())
					ppdata.m_pc = TO_ELEMENTAL(target->ch)->GetOwner();
				break;
				
#ifdef ATTACK_PET
			case MSG_CHAR_APET:
				if ( TO_APET(target->ch)->GetOwner() )
					ppdata.m_pc = TO_APET(target->ch)->GetOwner();
				break;
#endif //ATTACK_PET

#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
			case MSG_CHAR_NPC:
				if ( TO_NPC(target->ch)->GetOwner() )
					ppdata.m_pc = TO_NPC(target->ch)->GetOwner();
				break;
#endif  // NIGHTSHADOW_SKILL	
			default:
				break;
			}

			if (ppdata.m_pc)
			{
				void* pos = listAttackPC.FindData(&ppdata);
				if (pos == NULL)
				{
					PPDATA* ppdatanew = new PPDATA;
					ppdatanew->m_pc = ppdata.m_pc;
					ppdatanew->m_damage = dam;
					listAttackPC.AddToTail(ppdatanew);
				}
				else
				{
					PPDATA* ppdatacur = listAttackPC.GetData(pos);
					ppdatacur->m_damage += dam;
				}
			}

			totLevel += target->ch->m_level;
			count++;
		}
		target = target->m_next;
	}

	void* pos = listAttackPC.GetHead();
	while (pos)
	{
		PPDATA* ppdata = listAttackPC.GetData(pos);

		// 50% 이상 데미지 준 PC 반환
#ifdef NEW_DIVISION_EXPSP
		if (ppdata->m_damage > nPreferDamage / 2)
#else // #ifdef NEW_DIVISION_EXPSP
		if (ppdata->m_damage > totalDam / 2)
#endif // #ifdef NEW_DIVISION_EXPSP
			ret = ppdata->m_pc;
		pos = listAttackPC.GetNext(pos);
	}

	while (listAttackPC.GetCount() > 0)
	{
		PPDATA* ppdata = listAttackPC.GetData(listAttackPC.GetHead());
		listAttackPC.Remove(listAttackPC.GetHead());
		delete ppdata;
	}

	if (count == 0)
		*level = -1;
	else
	{
		*level = totLevel / count;
		if (*level < 0)
			*level = MAX_LEVEL;
	}

	return ret;
}

// 파티원 들에게 경험치 SP 분배
// npc가 죽었을때 party 원에게 Exp, SP 분배
#ifndef NEW_DIVISION_EXPSP
void DivisionExpSPParty(CParty* party, CNPC* npc, CPC* preferencePC)
{
	if (!party || !npc)
		return;
	
	int i;
	int nParty = 0;
	bool bTakeExp[MAX_PARTY_MEMBER];
	float partyDam = 0.0f;					// 파티 총 데미지
	float partyExp = 0.0f, partySP = 0.0f;	// 파티 총 경험치, SP

	float exp = 0.0f;					// 파티원 한사람이 가져갈 경험치 : damExp + sameExp + levelExp
	float sp = 0.0f;					// 파티원 한사람이 가져갈 SP : damSP + sameSP + levelSP

#ifdef EVENT_TEACH_2007
	bool	bTeacherAndStudent = IsTeachAndStudent( pParty );		// 사제간의 파티인지지 검사
#endif // EVENT_TEACH_2007

	memset(bTakeExp, 0, sizeof(bool) * MAX_PARTY_MEMBER);

	CAttackChar* attackCh = npc->m_attackList;

	int partyLevel = 0;
	int partyTotalLevel = 0;
	int partyMaxLevel = 0;
	i = 0;

	// 파티 총데미지 계산
	while (attackCh)
	{
		// 어택리스트에 파티 존재	: attackCh->pc->m_party
		// 파티원					: party == attackCh->pc->m_party && attackCh->pc != party->m_request
		// 근처						: CheckInNearCellExt(attackCh->pc, npc)
		// 살아 있음				: !DEAD(attackCh->pc)
		CPC* pExpPC = NULL;
		switch (attackCh->ch->m_type)
		{
		case MSG_CHAR_PC:
			pExpPC = TO_PC(attackCh->ch);
			break;
		case MSG_CHAR_PET:
			pExpPC = TO_PET(attackCh->ch)->GetOwner();
			break;
		case MSG_CHAR_ELEMENTAL:
			pExpPC = TO_ELEMENTAL(attackCh->ch)->GetOwner();
			break;
#ifdef ATTACK_PET
		case MSG_CHAR_APET:
			pExpPC = TO_APET(attackCh->ch)->GetOwner();
			break;
#endif //ATTACK_PET
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
		case MSG_CHAR_NPC:
			pExpPC = TO_NPC(attackCh->ch)->GetOwner();
			break;
#endif  // NIGHTSHADOW_SKILL
		default:
			break;
		}
		
		if (!pExpPC)
		{
			attackCh = attackCh->m_next;
			continue;
		}

		if (pExpPC->IsParty() && party == pExpPC->m_party && CheckInNearCellExt(pExpPC, npc) && !DEAD(pExpPC))
		{
			// 파티 뎀쥐에 추가,, Exp, SP주었는가 셋팅
			partyDam += (float)attackCh->m_damage;
			attackCh->m_bGiveExpSP = true;
		}

		attackCh = attackCh->m_next;
	}

	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if (party->GetMember(i) && CheckInNearCellExt(party->GetMember(i), npc) && !DEAD(party->GetMember(i)))
		{
			nParty++;

			partyTotalLevel += party->GetMember(i)->m_level;
			if (partyMaxLevel < party->GetMember(i)->m_level)
				partyMaxLevel = party->GetMember(i)->m_level;	// 파티원 최고 레벨

			int idx = party->Find(party->GetMember(i));
			if (idx >= 0)
				bTakeExp[idx] = true;
		}
	}

	// 파티데미지가 0 이거나 0명이거나 레벨합이 0이거나 ?!
	if (partyDam < 1 || nParty < 1 || partyTotalLevel < 1)
		return;

	partyLevel = (partyMaxLevel - (partyTotalLevel / nParty) > 5) ? (partyMaxLevel - 5) : (partyTotalLevel / nParty);

	// 파티 총데미지로 exp, sp 계산
	partyExp = (float)npc->m_proto->m_exp * partyDam / (float)npc->m_totalDamage;
	partySP = (float)npc->m_proto->m_skillPoint * partyDam / (float)npc->m_totalDamage;
	

	// 파티 exp 조정 : 050413
	// 인원수에 따른 보너스 %
	static const int countBonus[MAX_PARTY_MEMBER] = { 15, 30, 40, 50, 60, 70, 70, 70};

	int basic_party_plus_exp;
	int basic_party_plus_sp;

	// bw : 060817 : 파티 시스템 버그 부분, 인원수 보너스도 한명일때 0이도록 수정요함
	/*
	if( nParty == 1 )
	{
		basic_party_plus_exp = 0;
		basic_party_plus_sp = 0;
	}
	else
	*/
	{
		basic_party_plus_exp = PARTY_PLUS_EXP;
		basic_party_plus_sp = PARTY_PLUS_SP;
	}

	partyExp = partyExp + partyExp * (basic_party_plus_exp / 100.0f + (float)countBonus[nParty - 1] / 100.0f);
	partySP = partySP + partySP * (basic_party_plus_sp / 100.0f + (PARTY_PLUS_SP_COUNT * (float)(nParty - 1) / 100.0f));

	// 미리 랜덤 계산 하기
	partyExp *= GetRandom(900, 1100) / 1000.0f;
	partySP *= GetRandom(900, 1100) / 1000.0f;

	// 파티 exp 조정 : 050413
	// 레벨 차에 따른 경험치 페널티 %
	static const int levelPenalty[6] = {95, 90, 75, 60, 45, 10};

#ifdef EVENT_TEACH_2007
	if (bTeacherAndStudent == false)
	{
#endif // EVENT_TEACH_2007
		if (partyLevel > npc->m_level)
		{
			int diff = partyLevel - npc->m_level;
			
			if (diff > 6)
				diff = 6;

			partyExp = partyExp * (float)levelPenalty[diff - 1] / 100.0f;
		}

		//	SP 페널티
		if (party->GetPartyType() == PARTY_TYPE_RANDOM || party->GetPartyType() == PARTY_TYPE_BATTLE)
		{
			if (partyLevel > npc->m_level)
			{
				int diff = partyLevel - npc->m_level;

				bool bSP = false;

				if (diff > MAX_PENALTY_SP_LEVEL)
					bSP = true;

				if (bSP)
					partySP = 1.0f;
				else
					partySP *= (1.0f - (float)diff * DOWN_LEVEL_SP / 100.0f);
			}
			else if (partyLevel < npc->m_level)
				partySP *= (1.0f + (float)(npc->m_level - partyLevel) * UP_LEVEL_SP / 100.0f);
		}
#ifdef EVENT_TEACH_2007
	}
#endif //EVENT_TEACH_2007

	// 파티원에게 Exp, SP 분배
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		// 분배 대상만
		if (!party->GetMember(i) || !bTakeExp[i])
			continue;

		CPC* tpc = party->GetMember(i);

		// 파티 exp 조정 : 050413
		// 경험치
		// 파티타입이 0, 1이면
		//		(( 경험치 * 75 %) * 자신의 lv / 파티원 lv의 총합)) + (exp * 25%) * 1/ 파티원수
		// 2이면 ((경험치 * 70%) * 자신이 몹에게 피해를 준 dam / 몹이 받은 총dam)
		//				+ ((경험치 * 30) * 자신의 lv / 파티원 lv의 총합
		if(party->GetPartyType() == PARTY_TYPE_RANDOM || party->GetPartyType() == PARTY_TYPE_FIRSTGET )
		{
			exp = (partyExp * 75.0f / 100.0f) * ((float)tpc->m_level / (float)partyTotalLevel) 
						+ (partyExp * 25.0f / 100.0f) / (float)nParty;
#ifdef EVENT_TEACH_2007
			if (bTeacherAndStudent == true)
			{
				exp = partyExp / (100.0f * nParty);
			}
#endif // EVENT_TEACH_2007
		}
		else
		{
			float dam = 0.0f;
			attackCh = npc->m_attackList;

			// 자기가 때린 데미지 가져오기
			while (attackCh)
			{
				CPC* pExpPC = NULL;
				switch (attackCh->ch->m_type)
				{
				case MSG_CHAR_PC:
					pExpPC = TO_PC(attackCh->ch);
					break;
				case MSG_CHAR_PET:
					pExpPC = TO_PET(attackCh->ch)->GetOwner();
					break;
				case MSG_CHAR_ELEMENTAL:
					pExpPC = TO_ELEMENTAL(attackCh->ch)->GetOwner();
					break;
#ifdef ATTACK_PET
				case MSG_CHAR_PET:
					pExpPC = TO_APET(attackCh->ch)->GetOwner();
					break;
#endif //ATTACK_PET
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
				case MSG_CHAR_NPC:
					pExpPC = TO_NPC(attackCh->ch)->GetOwner();
					break;
#endif  // NIGHTSHADOW_SKILL	
				default:
					break;
				}
				
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
				if (!pExpPC)
				{
					attackCh = attackCh->m_next;
					continue;
				}
#endif  // NIGHTSHADOW_SKILL	

				if (pExpPC == tpc)
				{
					dam = (float)attackCh->m_damage;
					break;
				}

				attackCh = attackCh->m_next;
			}

			exp = partyExp * 70.0f / 100.0f * dam / partyDam
						+ (partyExp * 30.0f / 100.0f) * tpc->m_level / (float) partyTotalLevel ;
			
		}

		// 파티타입이 0, 2이면 sp = partySP / nParty
		// 
		if (party->GetPartyType() == PARTY_TYPE_RANDOM || party->GetPartyType() == PARTY_TYPE_BATTLE)
		{
			// 균등
			sp = partySP / nParty;
		}
		else
		{
			float dam = 0.0f;
			attackCh = npc->m_attackList;
			// 자기가 때린 데미지 가져오기
			while (attackCh)
			{
				CPC* pExpPC = NULL;
				switch (attackCh->ch->m_type)
				{
				case MSG_CHAR_PC:
					pExpPC = TO_PC(attackCh->ch);
					break;
				case MSG_CHAR_PET:
					pExpPC = TO_PET(attackCh->ch)->GetOwner();
					break;
				case MSG_CHAR_ELEMENTAL:
					pExpPC = TO_ELEMENTAL(attackCh->ch)->GetOwner();
					break;
#ifdef ATTACK_PET
				case MSG_CHAR_PET:
					pExpPC = TO_APET(attackCh->ch)->GetOwner();
					break;
#endif //ATTACK_PET
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
				case MSG_CHAR_NPC:
					pExpPC = TO_NPC(attackCh->ch)->GetOwner();
					break;
#endif  // NIGHTSHADOW_SKILL	
				default:
					break;
				}
				
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
				if (!pExpPC)
				{
					attackCh = attackCh->m_next;
					continue;
				}
#endif  // NIGHTSHADOW_SKILL	

				if (pExpPC == tpc)
				{
					dam = (float)attackCh->m_damage;
					break;
				}

				attackCh = attackCh->m_next;
			}

			// 파티 균등 경험치, SP 미리 구함 (파티원 모두에게 같음)
			sp = PARTY_SP_SAME * partySP / (100.0f * nParty);

			sp += PARTY_SP_DAMAGE * partySP * dam / (100.0f * partyDam);
			sp += PARTY_SP_LEVEL * partySP * tpc->m_level / (100.0f * partyTotalLevel);

#ifdef EVENT_TEACH_2007
			if( !bTeacherAndStudent )
			{
#endif //EVENT_TEACH_2007
			// 레벨 페널티 적용
			if (tpc->m_level > npc->m_level)
			{
				int diff = tpc->m_level - npc->m_level;

				bool bSP = false;

				if (diff > MAX_PENALTY_SP_LEVEL)
					bSP = true;

				if (bSP)
					sp = 1.0f;
				else
					sp *= (1.0f - (float)diff * DOWN_LEVEL_SP / 100.0f);
			}
#ifdef EVENTE_TEACH_2007
			}
			else
			{
#endif // EVENTE_TEACH_2007
			else if (tpc->m_level < npc->m_level)
				sp *= (1.0f + (float)(npc->m_level - tpc->m_level) * UP_LEVEL_SP / 100.0f);

#ifdef EVENTE_TEACH_2007
			}
#endif // EVENTE_TEACH_2007
		}


		
#ifdef EVENT_TEACH_2007
		if (bTeacherAndStudent == false)
		{
#endif //EVENT_TEACH_2007
		// 파티원중 최고레벨보다 15렙 작으면 경험치/SP를 5%만 얻는다
			if (partyMaxLevel - 15 > tpc->m_level)
			{
				exp /= 20;
				sp /= 20;
			}
#ifdef EVENTE_TEACH_2007
		}
#endif // EVENTE_TEACH_2007


		if (exp < 1.0f)
			exp = 1.0f;
		
		if (sp < 1.0f)
			sp = 1.0f;

		if (tpc == preferencePC)
		{
			// 9월 이벤트 : 경험치 4배
			if (tpc->m_assist.m_avAddition.hcSepExp)
			{
				tpc->m_assist.CureByItemIndex(882);	// 경험치
				exp = exp * 4;
				CNetMsg rmsg;
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_EXP);
				SEND_Q(rmsg, tpc->m_desc);
			}

			// 9월 이벤트 : SP 4배
			if (tpc->m_assist.m_avAddition.hcSepSP)
			{
				tpc->m_assist.CureByItemIndex(883);	// 숙련도
				sp = sp * 4;
				CNetMsg rmsg;
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_SP);
				SEND_Q(rmsg, tpc->m_desc);
			}
		}

#ifdef NEW_ACCERY_ADD
		if( tpc->m_AddProb )
		{
			exp = exp + ( exp * tpc->m_AddProb / 100 );
			sp = sp + ( SP * tpc->m_AddProb / 100 );
		}
#endif // NEW_ACCERY_ADD


		tpc->AddExpSP((LONGLONG)exp, (int)sp, true);

#ifdef ATTACK_PET
		CAPet* apet = tpc->GetAPet();
		if( apet )
		{
			apet->AddExpSP( (LONGLONG)( npc->m_level/5) , 0 );
		}
#endif //ATTACK_PET
			

		if( sp > 1000000 )
		{
			GAMELOG << init("SP_UP_BUG", tpc)
					<< tpc->m_skillPoint << delim
					<< sp << end;
			/*GAMELOG
					<< attackCh->pc->m_str << delim
					<< attackCh->pc->m_dex << delim
					<< attackCh->pc->m_int << delim
					<< attackCh->pc->m_con << delim
					<< sp << delim
					<< attackCh->m_damage << delim
					<< npc->m_totalDamage << delim
					<< attackCh->pc->IsParty() << delim
					<< party->GetPartyType() << delim
					<< partyLevel << delim
					<< partyMaxLevel << delim
					<< partyDam << delim
					<< partySP << end;
					*/
			
		}
	}
}

// npc가 죽었을때 어택리스트들에게 Exp, SP 분배
bool DivisionExpSP(CNPC* npc, CPC* preferencePC)
{
	float exp = 0.0f, sp = 0.0f;

	// 동급 레벨
	float giveExp = (float)npc->m_proto->m_exp;
	float giveSP = (float)npc->m_proto->m_skillPoint;
	
	CAttackChar* attackCh = NULL;
	CAttackChar* attackChNext = npc->m_attackList;
	
	while ((attackCh = attackChNext))
	{
		attackChNext = attackCh->m_next;
		
		if (attackCh->ch)
		{
			// 같은 zone에 없거나 같은 area에 없거나 CELL_EXT 범위 안에 없거나
			if ((npc->m_pZone->m_index != attackCh->ch->m_pZone->m_index) || 
				(npc->m_pArea->m_index != attackCh->ch->m_pArea->m_index) ||
				!CheckInNearCellExt(npc, attackCh->ch))
				continue;
			
			// 경험치와 SP를 받았는가
			if (attackCh->m_bGiveExpSP)
				continue;

			// 지가 때린 데미지가 없으면 ?!
			if (attackCh->m_damage <= 0)
			{
				attackCh->m_bGiveExpSP = true;
				continue;
			}

			CPC* pExpPC = NULL;
			switch (attackCh->ch->m_type)
			{
			case MSG_CHAR_PC:
				pExpPC = TO_PC(attackCh->ch);
				break;
			case MSG_CHAR_PET:
				pExpPC = TO_PET(attackCh->ch)->GetOwner();
				break;
			case MSG_CHAR_ELEMENTAL:
				pExpPC = TO_ELEMENTAL(attackCh->ch)->GetOwner();
				break;
#ifdef ATTACK_PET
			case MSG_CHAR_APET :
				pExpPC = TO_APET(attackCh->ch)->GetOwner();
				break;
#endif //ATTACK_PET
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
			case MSG_CHAR_NPC:
				pExpPC = TO_NPC(attackCh->ch)->GetOwner();
				break;
#endif  // NIGHTSHADOW_SKILL	// 
			default:
				break;
			}
			
			if (!pExpPC) continue;

			// 파티의 경우
			if (pExpPC->IsParty())
				DivisionExpSPParty(pExpPC->m_party, npc, preferencePC);
			
			// 파티가 아닌 경우
			else
			{
				attackCh->m_bGiveExpSP = true;

				// 경험치 페널티 조정 : 050413
				// 레벨 차에 따른 경험치 페널티 %
				static const int levelPenalty[6] = {95, 90, 75, 60, 45, 10};
				
				if (pExpPC->m_level > npc->m_level)
				{
					int diff = pExpPC->m_level - npc->m_level;
					
					if (diff > 6)
						diff = 6;
					
					giveExp = giveExp * (float)levelPenalty[diff - 1] / 100.0f;
				}

				if (pExpPC->m_level > npc->m_level)
				{
					int diff = pExpPC->m_level - npc->m_level;

					bool bSP = false;
					
					if (diff > MAX_PENALTY_SP_LEVEL)
						bSP = true;
					
					if (bSP)
						giveSP = 1.0f;
					else
						giveSP *= (1.0f - (float)diff * DOWN_LEVEL_SP / 100.0f);
				}

				// SP의 경우 상위레벨 적용
				else if (pExpPC->m_level < npc->m_level)
					giveSP *= (1 + (float)(npc->m_level - pExpPC->m_level) * UP_LEVEL_SP / 100.0f);

				exp = giveExp * (float)attackCh->m_damage / (float)npc->m_totalDamage;
				sp = giveSP * (float)attackCh->m_damage / (float)npc->m_totalDamage;

				exp *= GetRandom(900, 1100) / 1000.0f;
				sp *= GetRandom(900, 1100) / 1000.0f;

				if (exp < 1.0f)
					exp = 1.0f;

				if (sp < 1.0f)
					sp = 1.0f;

				if (pExpPC)
				{
					if (pExpPC == preferencePC)
					{
						// 9월 이벤트 : 경험치 4배
						if (pExpPC->m_assist.m_avAddition.hcSepExp)
						{
							pExpPC->m_assist.CureByItemIndex(882);	// 경험치
							exp = exp * 4;
							CNetMsg rmsg;
							EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_EXP);
							SEND_Q(rmsg, pExpPC->m_desc);
						}

						// 9월 이벤트 : SP 4배
						if (pExpPC->m_assist.m_avAddition.hcSepSP)
						{
							pExpPC->m_assist.CureByItemIndex(883);	// 숙련도
							sp = sp * 4;
							CNetMsg rmsg;
							EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_SP);
							SEND_Q(rmsg, pExpPC->m_desc);
						}
					}
#ifdef NEW_ACCERY_ADD
					if( tpc->m_AddProb )
					{
						exp = exp + ( exp * tpc->m_AddProb / 100 );
						sp = sp + ( SP * tpc->m_AddProb / 100 );
					}
#endif // NEW_ACCERY_ADD

					if( sp > 1000000 )
					{
						GAMELOG << init("SP_UP_BUG", pExpPC)
								<< pExpPC->m_skillPoint << delim
								<< sp << end;
					}

					if( pExpPC->AddExpSP((LONGLONG)exp, (int)sp), true)
#ifdef ATTACK_PET
					{
						CAPet* apet = pExpPC->GetAPet();
						if( apet )
						{
							apet->AddExpSP( (LONGLONG)( npc->m_level/5) , 0 );
						}						
						return true
					}
#else
						return true;
#endif //ATTACK_PET

						
					attackCh->m_bGiveExpSP = true;

					pExpPC->m_bChangeStatus = true;
				}
			}
		}
	}
	return false;
}
#endif // #ifndef NEW_DIVISION_EXPSP

// 균등 파티 시 돈 갈라먹기
void DivisionPartyMoney(CPC* pc, CItem* item)
{
	// validation
	if (!pc && !item && item->m_idNum != gserver.m_itemProtoList.m_moneyItem->m_index)
		return;
	
	int i;
	CParty* party = pc->m_party;
	bool bPreference = false;		// 이 파티가 자격이 있는가
	int divCount = 0;				// 돈 갈라먹을 파티 멤버 수
	
	for (i=0; i < MAX_PARTY_MEMBER; i++)
	{
		CPC* pMemberPC = party->GetNearMember(pc, i);
		if (!pMemberPC)
			continue;
		
		if (item->m_preferenceIndex == pMemberPC->m_index)
			bPreference = true;

		divCount++;
	}
	
	if (item->m_preferenceIndex != -1 && !bPreference)
		return;
	
	CNetMsg itemmsg;

	if (divCount < 1)
		return;
	
	// 돈 갈라내기
	bool bTake = false;		// 돈을 받은 사람이 있나?: 없으면 아이템은 바닥에 그대로
	LONGLONG count = item->Count() / (LONGLONG)divCount;
	
	if (count < 1)
		return;
	
	// 파티원에게 분배
	for (i = MAX_PARTY_MEMBER - 1; i >= 0; i--)
	{
		CPC* pMemberPC = party->GetNearMember(pc, i);
		if (!pMemberPC)
			continue;
		
		// 들어갈 인벤토리 결정
		CInventory* inven = GET_INVENTORY(pMemberPC, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
		if (!inven)
			continue ;
		
		// 갈른 돈 만들기
		CItem* money = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_moneyItem->m_index, WEARING_NONE, 0, 0, count);
		
		if (!money)
			continue;
		
		bool bCountable = false;
		// 인벤에 넣기
		if (AddToInventory(pMemberPC, money, true, true))
		{
			bTake = true;
			
			// 겹쳐졌는지 검사
			if (money->tab() == -1)
			{
				bCountable = true;
				
				// 수량 변경 알림
				int r, c;
				if (inven->FindItem(&r, &c, money->m_idNum, money->m_plus, money->m_flag))
				{
					CItem* p = inven->GetItem(r, c);
					if (p)
						ItemUpdateMsg(itemmsg, p, money->Count());
				}
			}
			else
			{
				ItemAddMsg(itemmsg, money);
				// 돈 검사
				if (money->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && pMemberPC->m_moneyItem == NULL)
					pMemberPC->m_moneyItem = money;
			}
			
			// LOG
			GAMELOG << init("ITEM_PICK", pc)
					<< itemlog(money)
					<< end;
			
			SEND_Q(itemmsg, pMemberPC->m_desc);
		}
		else
		{
			CNetMsg smsg;
			SysFullInventoryMsg(smsg, (char)inven->m_tab);
			SEND_Q(smsg, pMemberPC->m_desc);
			
			delete money;
			money = NULL;
			continue ;
		}
		
		if (bCountable)
		{
			delete money;
			money = NULL;
		}
	}
	
	if (bTake)
	{
		// 원래 돈 하나 짜리 아이템 처리
		CNetMsg disappmsg;
		CNetMsg takemsg;
		
		int cx, cz;
		
		cx = item->m_cellX;
		cz = item->m_cellZ;
		
		ItemTakeMsg(takemsg, pc, item);
		ItemDisappearMsg(disappmsg, item);
		
		// 우선권 제거
		item->m_preferenceIndex = -1;
		
		// 땅에서 제거
		pc->m_pArea->ItemFromCell(item);
		
		delete item;
		
		// 메시지 보내고
		pc->m_pArea->SendToCell(takemsg, pc, true);
		pc->m_pArea->SendToCell(disappmsg, GET_YLAYER(pc), cx, cz);
	}
}

// 균등파티일때 아이템 랜덤으로 가져가지
bool GetItemRandomParty(CPC* pc, CItem* item)
{
	// validation
	if (!pc && !item && !pc->m_party)
		return false;
	
	int i;
	CParty* party = pc->m_party;
	bool bPreference = false;		// 이 파티가 자격이 있는가
	int partyCount = 0;				// 받을 수 있는 자격이 있는 파티원 수
	
	for (i=0; i < MAX_PARTY_MEMBER; i++)
	{
		CPC* pMemberPC = party->GetNearMember(pc, i);
		if (!pMemberPC)
			continue;
		
		if (item->m_preferenceIndex == pMemberPC->m_index)
			bPreference = true;

		partyCount++;
	}
	
	// 우선권이 있는데 파티가 자격없으면 return;
	if (item->m_preferenceIndex != -1 && !bPreference)
	{
		// 우선권이 없습니다 메시지 전송
		CNetMsg smsg;
		SysMsg(smsg, MSG_SYS_NOT_OWNER_ITEM);
		SEND_Q(smsg, pc->m_desc);
		return false;
	}

	// 받을 사람이 없다.
	if (partyCount < 1)
		return false;

	CNetMsg itemmsg;

	bool bGiveItem = false;		// 아이템 가져 갔다!
	int idx;					// 결정된 파티원 인덱스

	int bitfield = 0;			// 결정瑛립?실패한 파티원 인덱스 저장
	bool bAll = false;

	while (!bGiveItem)
	{
		idx = GetRandom(0, MAX_PARTY_MEMBER - 1);

		for (i = 0; i < MAX_PARTY_MEMBER; i++)
		{
			if ((bitfield & (1 << ((idx + i) % MAX_PARTY_MEMBER))) == 0)
				break ;
		}

		// 가져갈 사람이 없음 다 한번씩 받아갔음
		if (i == MAX_PARTY_MEMBER)
		{
			if (bAll)
				return false;

			bAll = true;
			bitfield = 0;
			for (i=0; i < MAX_PARTY_MEMBER; i++)
			{
				CPC* pMemberPC = pc->m_party->GetNearMember(pc, i);
				if (!pMemberPC)
				{
					bitfield |= (1 << i);
					continue;
				}
				pMemberPC->m_bGiveItem = false;
			}

			continue;
		}
		else
			idx = (idx + i) % MAX_PARTY_MEMBER;

		bitfield |= (1 << idx);

		// 파티원이 없으면 continue;
		CPC* pMemberPC = party->GetNearMember(pc, idx);
		if (!pMemberPC)
			continue;

		// 이미 한번 받았으면 continue;
		if (pMemberPC->m_bGiveItem)
			continue;

		// 들어갈 인벤토리 결정
		CInventory* inven = GET_INVENTORY(pMemberPC, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
		if (!inven)
			continue ;

#ifdef EVENT_EGGS_HUNT_2007
		if(item->m_idNum == 2148)
		{
			int r, c;
			if(inven->FindItem(&r, &c, item->m_idNum, -1, -1))
			{
				// EGG 아이템 이미 소유하고 있음
				CNetMsg rmsg;
				EventEggsHunt2007ErrorMsg(rmsg, MSG_EVENT_EGGS_HUNT_2007_ERROR_ALREADY_EXIST);
				SEND_Q(rmsg, pMemberPC->m_desc);
				return false;
			}
		}
#endif // EVENT_EGGS_HUNT_2007

		bool bCountable = false;
		// 인벤에 넣기
		if (AddToInventory(pMemberPC, item, true, true))
		{
			bGiveItem = true;
			
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
			
			// LOG
			GAMELOG << init("ITEM_PICK", pMemberPC)
					<< itemlog(item)
					<< end;
			
			SEND_Q(itemmsg, pMemberPC->m_desc);
		}
		else
		{
			CNetMsg smsg;
			SysFullInventoryMsg(smsg, (char)inven->m_tab);
			SEND_Q(smsg, pMemberPC->m_desc);
			continue ;
		}

		// 원래 아이템 처리
		CNetMsg disappmsg;
		CNetMsg takemsg;
		CNetMsg getmsg;
		
		int cx, cz;
		
		cx = item->m_cellX;
		cz = item->m_cellZ;
		
		ItemTakeMsg(takemsg, pc, item);
		ItemDisappearMsg(disappmsg, item);
		ItemGetMsg(getmsg, pMemberPC, item);
		
		// 우선권 제거
		item->m_preferenceIndex = -1;
		
		// 땅에서 제거
		pc->m_pArea->ItemFromCell(item);

		// 받았다고 셋팅
		pMemberPC->m_bGiveItem = true;
		
		// 메시지 보내고
		pc->m_pArea->SendToCell(takemsg, pc, true);
		pc->m_pArea->SendToCell(disappmsg, GET_YLAYER(pc), cx, cz);
		pc->m_party->SendToPCInSameZone(pc->m_pZone->m_index, pc->m_pArea->m_index, getmsg);
		
		if (bCountable)
		{
			delete item;
			item = NULL;
		}
	}

	return true;
}

// target list 중 hate 수치 가장 높은 target 반환
CAttackChar* GetMaxHateTarget(CNPC* npc)
{
	CAttackChar* ret = NULL;
	CAttackChar* target = npc->m_attackList;
	int hate = 0;
	
	while (target)
	{
		// hate 수치가 0이면 도망간넘 : 타겟리스트에는 있지만 타겟은 아니다.
		if (hate <= target->m_targetHate && target->m_targetHate != 0)
		{
			hate = target->m_targetHate;
			ret = target;
		}
		
		target = target->m_next;
	}
	
	return ret;
}

// tch 의 어택리스트에 ch 정보 추가
// ch가 tch를 공격 ;;;;
CAttackChar* AddAttackList(CCharacter* ch, CCharacter* tch, int hate)
{
	if (ch == tch)
		return NULL;

	bool bCh = true;
	bool bTCh = true;
	
	CAttackChar* chAttackList = ch->m_attackList;
	CAttackChar* tchAttackList = tch->m_attackList;
	CAttackChar* ret = NULL;
	
	// 이미 어택리스에 있지만 다시 어택리스크가 된 경우
	// TargetHate 가 0인 넘이 있다면 도망갔다가 다시 온 경우
	// TargetHate를 초기값으로 셋팅
	while (chAttackList)
	{
		if (chAttackList->ch == tch)
		{
			bCh = false;
			chAttackList->m_targetHate += hate;
			break;
		}
		chAttackList = chAttackList->m_next;
	}
	
	while (tchAttackList)
	{
		if (tchAttackList->ch == ch)
		{
			bTCh = false;
			tchAttackList->m_targetHate += hate;
			ret = tchAttackList;
			break;
		}
		tchAttackList = tchAttackList->m_next;
	}

	CAttackChar* attackCh = NULL;
	CAttackChar* attackTCh = NULL;
	
	if (bCh)
	{
		attackCh = new CAttackChar;
		attackCh->ch = tch;
		ADD_TO_BILIST(attackCh, ch->m_attackList, m_prev, m_next);
	}
	
	if (bTCh)
	{
		attackTCh = new CAttackChar;
		attackTCh->ch = ch;
#ifdef NEW_PK
		attackTCh->m_bFirstAttack = true;
#endif
		ADD_TO_BILIST(attackTCh, tch->m_attackList, m_prev, m_next);
		ret = attackTCh;
	}
	
	if (bCh && bTCh)
	{
		if (IS_NPC(tch))
			attackTCh->m_targetPulse = gserver.m_pulse;
		attackTCh->m_targetHate += hate;
	}

	return ret;
}

// ch 어택리스트에서 제거
void DelAttackList(CCharacter* ch)
{
	if (ch == NULL
		|| ch->m_attackList == NULL)
	{
		return;
	}

	CAttackChar* attCh;
	CAttackChar* attChNext = ch->m_attackList;
	CAttackChar* attTCh;
	CAttackChar* attTChNext;
	
	while ((attCh = attChNext))
	{
		attChNext = attCh->m_next;
		
		attTChNext = attCh->ch->m_attackList;
		while ((attTCh = attTChNext))
		{
			attTChNext = attTCh->m_next;
			if (attTCh->ch == ch)
			{
				REMOVE_FROM_BILIST(attTCh, attCh->ch->m_attackList, m_prev, m_next);
				delete attTCh;
				attTCh = NULL;
				break;
			}
		}
		
		REMOVE_FROM_BILIST(attCh, ch->m_attackList, m_prev, m_next);
		delete attCh;
		attCh = NULL;
	}
}

// ch 어택리스트에서 tch만 제거
void DelTargetFromAttackList(CCharacter* ch, CCharacter* tch)
{
	CAttackChar* attCh;
	CAttackChar* attChNext = ch->m_attackList;
	CAttackChar* attTCh;
	CAttackChar* attTChNext;
	
	while ((attCh = attChNext))
	{
		attChNext = attCh->m_next;

		if (attCh->ch == tch)
		{
			attTChNext = attCh->ch->m_attackList;
			while ((attTCh = attTChNext))
			{
				attTChNext = attTCh->m_next;
				if (attTCh->ch == ch)
				{
					REMOVE_FROM_BILIST(attTCh, attCh->ch->m_attackList, m_prev, m_next);
					delete attTCh;
					attTCh = NULL;
					break;
				}
			}
			
			REMOVE_FROM_BILIST(attCh, ch->m_attackList, m_prev, m_next);
			delete attCh;
			attCh = NULL;
			break;
		}
	}
}

// 정당방위 리스트에 추가
void AddRaList(CPC* of, CPC* df)
{
	// 피해자 정방 리스트에 가해자 추가
	CRaChar* raOf = new CRaChar;
	raOf->m_raTarget = of;
	raOf->m_raPulse = RAMODE_USE_PULSE;
	raOf->m_bAttacker = true;
	ADD_TO_BILIST(raOf, df->m_raList, m_prev, m_next);
	df->SetPlayerState(PLAYER_STATE_RAMODE);

	// 가해자 정방 리스트에 피해자 추가
	CRaChar* raDf = new CRaChar;
	raDf->m_raTarget = df;
	raDf->m_bAttacker = false;
	ADD_TO_BILIST(raDf, of->m_raList, m_prev, m_next);

	CNetMsg rMsg;
	RightAttackMsg(rMsg, of, MSG_RIGHT_ATTACK_ADD);
	SEND_Q(rMsg, of->m_desc);
	SEND_Q(rMsg, df->m_desc);
}

// 정당방위 리스트에서 제거
void DelRaList(CPC* pc, bool bForce)
{
	CRaChar* raPC;
	CRaChar* raPCNext = pc->m_raList;

	CRaChar* raTPC;
	CRaChar* raTPCNext;

	CNetMsg rMsg;
	RightAttackMsg(rMsg, pc, MSG_RIGHT_ATTACK_DEL);
	
	while ((raPC = raPCNext))
	{
		raPCNext = raPC->m_next;
		raTPCNext = raPC->m_raTarget->m_raList;

		while ((raTPC = raTPCNext))
		{
			raTPCNext = raTPC->m_next;

			if (raTPC->m_raTarget == pc)
			{
				if (!raTPC->m_bAttacker && !bForce)
					return;

				SEND_Q(rMsg, raTPC->m_raTarget->m_desc);
				REMOVE_FROM_BILIST(raTPC, raPC->m_raTarget->m_raList, m_prev, m_next);
				delete raTPC;
				raTPC = NULL;
				break;
			}
		}

		SEND_Q(rMsg, raPC->m_raTarget->m_desc);

		REMOVE_FROM_BILIST(raPC, pc->m_raList, m_prev, m_next);
		delete raPC;
		raPC = NULL;
	}
}

// pc 정당방위 리스트에 tpc가 있는가
bool IsRaList(CPC* pc, CPC* tpc)
{
	CRaChar* raList = pc->m_raList;

	while (raList)
	{
		if (raList->m_raTarget == tpc)
			return true;

		raList = raList->m_next;
	}

	return false;
}

#ifdef NEW_PK
bool IsFirstAttackInAttackList(CPC* of, CPC* df)
{
	CAttackChar* pAttackChar = df->m_attackList;
	while (pAttackChar)
	{
		if (pAttackChar->ch == of && pAttackChar->m_bFirstAttack)
			return true;
		pAttackChar = pAttackChar->m_next;
	}
	return false;
}
#endif

// 공격 당하는 npc의 동족찾기 : 있으면 같이 공격
void FindFamilyInNear(CCharacter* of, CNPC* npc)
{
	// Cell 2개 정도에서
	int sx = npc->m_cellX - 2;
	int ex = npc->m_cellX + 2;
	int sz = npc->m_cellZ - 2;
	int ez = npc->m_cellZ + 2;
	
	int x, z;
	
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= npc->m_pArea->m_size[0])
			continue;
		
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= npc->m_pArea->m_size[1])
				continue;
			
			CCharacter* ch = npc->m_pArea->m_cell[x][z].m_listChar;
			while (ch)
			{
				if (IS_NPC(ch) && TO_NPC(ch) != npc)
				{
					CNPC* tnpc = TO_NPC(ch);

					// 동족이고 시야 2배 이내 거리이고 disableTime이 셋팅 되어 있지 않을때
					// attacklist가 비어 있을 때
					if (npc->m_proto->m_family != -1 &&
						tnpc->m_proto->m_family == npc->m_proto->m_family && 
						GetDistance(npc, tnpc) < npc->m_proto->m_sight * 2 &&
						tnpc->m_disableTime == 0 && !tnpc->IsBlind()
						&& tnpc->m_attackList == NULL)
							AddAttackList(of, tnpc, HATE_FIRST_ATTACK);
				}
				ch = ch->m_pCellNext;
			}
		}
	}
}


////////////////////
// Function name	: GetSkillPrototypes
// Description	    : proto, level, magicindex로 각각의 프로토타입을 얻는다
// Return type		: bool 
//                  : 정상일때 true
bool GetSkillPrototypes(const CSkillProto* proto, int level, int magicindex, const CSkillLevelProto** levelproto, const CMagicProto** magic, const CMagicLevelProto** magiclevel)
{
	*levelproto = NULL;
	*magic = NULL;
	*magiclevel = NULL;

	if (proto == NULL)
		return false;

	if (magicindex < 0 || magicindex >= MAX_SKILL_MAGIC)
		return false;

	*levelproto = proto->Level(level);
	if (*levelproto == NULL)
		return false;

	*magic = (*levelproto)->m_magic[magicindex];
	if (*magic == NULL)
		return false;

	*magiclevel = (*magic)->Level((*levelproto)->m_magicLevel[magicindex]);
	if (*magiclevel == NULL)
		return false;

	return true;
}

// of -> df에게 hate만큼 HATE 수치 증가
CAttackChar* ApplyHate(CCharacter* of, CCharacter* df, int hate, bool bApplyFamily)
{
	// 선공 검사
	bool bFirstAttack = false;

	// df의 리스트가 비었을 때에만 선공
	if (df->m_attackList == NULL)
	{
		bFirstAttack = true;
		hate += HATE_FIRST_ATTACK;
	}

	// 리스트에 추가
	CAttackChar* ret = AddAttackList(of, df, hate);

	// 동족에게 적용 : df == NPC일 경우
	if (bApplyFamily && IS_NPC(df))
		FindFamilyInNear(of, TO_NPC(df));

	return ret;
}


////////////////////
// Function name	: ApplyHateByDamage
// Description	    : of가 df를 damage만큼 공격시 해당하는 hate 수치를 적용
void ApplyHateByDamage(CCharacter* of, CCharacter* df, char hittype, int damage)
{
	if (hittype != HITTYPE_MISS)
	{
		int hate = 0;

		// of가 NPC일 경우 hate 증가 안함
		// damage에 따른 hate 수치 계산
		if (!IS_NPC(of))
		{
			hate += 10000 * damage / df->m_maxHP;
			if (hate < 1)
				hate = 1;
		}

		CAttackChar* p = ApplyHate(of, df, hate, true);
		if (p)
		{
			if (p->m_damage + damage > 1)
				p->m_damage += damage;
		}

#ifndef NEW_DIVISION_EXPSP
		// 대상의 전체 데미지 누적 갱신
		if (IS_NPC(df))
			TO_NPC(df)->m_totalDamage += damage;
#endif // #ifndef NEW_DIVISION_EXPSP
	}
}

////////////////////
// Function name	: ApplyDamage
// Description	    : of가 df를 damageType형태의 proto를 가지고 flag형태로 damage만큼 공격
//                  : 해당하는 damage메시지와 proto에 다른 effect메시지를 전달
void ApplyDamage(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE damageType, const CSkillProto* proto, const CMagicProto* magic, int damage, char flag)
{
	CNetMsg rmsg;
	int realdamage = 0;

	// PvP에서는 데미지 25%만
	bool bIsPCof = (IS_PC(of) || IS_PET(of) || IS_ELEMENTAL(of) || IS_APET(of) );
	bool bIsPCdf = (IS_PC(df) || IS_PET(df) || IS_ELEMENTAL(df) || IS_APET(df) );
	if (bIsPCof && bIsPCdf)
		damage /= 4;

	if (flag != HITTYPE_MISS)
	{
		if (damage < 1)
			damage = 1;

#ifdef ENABLE_PET
		if (IS_PET(df))
		{
			damage = 1;
			// 050309: bw 배고픔 20프로 미만일때 데미지 2씩 감소 
			CPet* p = TO_PET(df);
			if( p )
			{
				if( p->GetHungryPoint() < 20 )
					damage = 2;
			}
		}
#endif
	}
	else
		damage = 0;

	if (damage > 0)
	{
		// 절망 처리
		if (df->m_assist.m_state & AST_DESPAIR)
		{
			damage += df->m_assist.m_avAddition.despair
					+ damage * df->m_assist.m_avRate.despair / SKILL_RATE_UNIT;
			df->m_assist.CancelDespair();
		}

		if (df->m_linkSource && df != df->m_linkSource && CheckInNearCellExt(df, df->m_linkSource))
		{
			int linkdamage = damage / 2;
			damage -= linkdamage;
			if (linkdamage > 0)
				ProcAttack(of, df->m_linkSource, MSG_DAMAGE_LINK, NULL, 0, linkdamage);
		}

		if (IS_ELEMENTAL(df))
		{
			CElemental* pElemental = TO_ELEMENTAL(df);
			CPC* pPC = pElemental->GetOwner();
			if (pPC && (pPC->m_assist.m_state & AST_MANASCREEN) && CheckInNearCellExt(pElemental, pPC))
			{
				damage -= pPC->m_assist.m_avAddition.manascreen
						+ damage * pPC->m_assist.m_avRate.manascreen / SKILL_RATE_UNIT;
				pPC->m_mp -= pPC->m_maxMP * 3 / 100;
				if (pPC->m_mp <= 0)
				{
					pPC->m_mp = 0;
					pPC->m_assist.CancelManaScreen();
				}
				pPC->m_bChangeStatus = true;
			}
		}

		if (damage < 1)
			damage = 1;

		// HP 감소
		if (damage > df->m_hp)
			realdamage = df->m_hp;
		else
			realdamage = damage;

		bool bSendOFStatusMsg = false;

		// HP/MP 스틸
		if (IS_PC(of) && realdamage > 0)
		{
			CPC* pPCOffense = TO_PC(of);
			
#ifdef HP_STEEL_POTION
			if (pPCOffense->m_assist.m_avAddition.hcHPSteelPotion == true )
			{
				int prob = GetRandom( 1, 100 );
				if( prob > 70 ) // 30%
				{
					int nStealHP = realdamage * 5 / 100;
					if( nStealHP > 0 )
					{
						pPCOffense->m_hp += nStealHP;
						if( pPCOffense->m_hp > pPCOffense->m_maxHP )
							pPCOffense->m_hp = pPCOffense->m_maxHP;
						pPCOffense->m_bChangeStash = true;
						bSendOFStatusMsg = true;
						
						EffectEtcMsg(rmsg, pPCOffense, MSG_EFFECT_ETC_HP_STEEL);
						pPCOffense->m_pArea->SendToCell(rmsg, pPCOffense, true);
					}
				}
			}
			else
#endif // HP_STEEL_POTION
			if (pPCOffense->m_opHPSteal > 0)
			{
				if (GetRandom(1, 100) > 95) // 5%
				{
					int nStealHP = realdamage * pPCOffense->m_opHPSteal / 100;
					if (nStealHP > 0)
					{
						pPCOffense->m_hp += nStealHP;
						if (pPCOffense->m_hp > pPCOffense->m_maxHP)
							pPCOffense->m_hp = pPCOffense->m_maxHP;
						pPCOffense->m_bChangeStatus = true;
						bSendOFStatusMsg = true;
					}
				}
			}
			
#ifdef MP_STEEL_POTION
			if (pPCOffense->m_assist.m_avAddition.hcMPSteelPotion == true )
			{
				int prob = GetRandom( 1, 100 );
				if( prob > 70 ) // 30%
				{
					int nStealMP = pPCOffense->m_maxMP * 10 / 100;
					if (nStealMP > 0)
					{
						pPCOffense->m_mp += nStealMP;
						if (pPCOffense->m_mp > pPCOffense->m_maxMP)
							pPCOffense->m_mp = pPCOffense->m_maxMP;
						pPCOffense->m_bChangeStatus = true;
						bSendOFStatusMsg = true;
						
						EffectEtcMsg(rmsg, pPCOffense, MSG_EFFECT_ETC_MP_STEEL);
						pPCOffense->m_pArea->SendToCell(rmsg, pPCOffense, true);
					}
				}
			}		
			else
#endif // MP_STEEL_POTION
			if (pPCOffense->m_opMPSteal > 0)
			{
				if (GetRandom(1, 100) > 95) // 5%
				{
					int nStealMP = realdamage * pPCOffense->m_opMPSteal / 100;
					if (nStealMP > 0)
					{
						pPCOffense->m_mp += nStealMP;
						if (pPCOffense->m_mp > pPCOffense->m_maxMP)
							pPCOffense->m_mp = pPCOffense->m_maxMP;
						pPCOffense->m_bChangeStatus = true;
						bSendOFStatusMsg = true;
					}
				}
			}
		}


		if (magic)
		{
			switch (magic->m_type)
			{
			case MT_ATTACK:
				switch (magic->m_subtype)
				{
				case MST_ATTACK_DRAIN:
					if (!IS_PET(of) && !IS_PET(df))
					{
						if (IS_PC(of) && IS_PC(df))
							of->m_hp += realdamage / 2;
						else
							of->m_hp += realdamage;
						if (of->m_hp > of->m_maxHP)
							of->m_hp = of->m_maxHP;
						
						if (IS_PC(of))
						{
							CPC* opc = TO_PC(of);
							opc->m_bChangeStatus = true;
						}
						bSendOFStatusMsg = true;
					}
					break;
#ifdef NIGHTSHADOW_SKILL
				case MST_ATTACK_ONESHOTKILL:
					// 15% 미만이면 즉사가 적용됨
					if ( IS_NPC(df) && df->m_hp <= df->m_maxHP * 15 / 100 )
					{
						damage = df->m_hp;
					}
					break;
#endif // NIGHTSHADOW_SKILL
				}
				break;
			}
		}

#ifdef ATTACK_PET
		if ( IS_APET(df) )
		{
			CAPet* apet = TO_APET(df);
			if( apet )
			{
				ExAPetStatusMsg( rmsg, apet);
				SEND_Q( rmsg, apet->GetOwner()->m_desc );
			}
		}
#endif // ATTACK_PET

		if (bSendOFStatusMsg)
		{
			CharStatusMsg(rmsg, of, 0);
			df->m_pArea->SendToCell(rmsg, of, true);
		}

		df->m_hp -= damage;
#ifdef NIGHTSHADOW_SKILL
		if ( of->m_assist.m_state & AST_INVERSE_DAMAGE && damageType != MSG_DAMAGE_COMBO )
		{
			// 역 데미지 디버프에 걸린 몬스터이면 몬스터 콤보 이펙트 데미지를 제외한 모든 데미지를 회복으로 바꾼다.
			df->m_hp += damage * 2;
		}
		if (df->m_hp >= df->m_maxHP)
			df->m_hp = df->m_maxHP;
#endif // NIGHTSHADOW_SKILL
		if (df->m_hp <= 0)
			df->m_hp = 0;
	}

	// 결과 메시지
	if (proto)
		DamageMsg(rmsg, of, df, damageType, proto->m_index, damage, flag);
	else
		DamageMsg(rmsg, of, df, damageType, -1, damage, flag);
	df->m_pArea->SendToCell(rmsg, df, true);
}


////////////////////
// Function name	: CalcPKPoint
// Description	    : of에 의해 df가 사망시 of, df의 성향 수치 계산
//                  : bDeadPet가 true이면 Pet 사망에 의한 처리
void CalcPKPoint(CPC* of, CPC* df, bool bDeadPet)
{

#ifdef ADULT_SERVER
	int nPkBagicPenalty = -5;
	int nPkPlusPenalty = 0;
	int nPkPlusCount = 0;

	if( of->m_level <= PKMODE_LIMIT_LEVEL ) // 렙이 15이하이면 페널티 없음
		return;

	CRaChar* raList = of->m_raList;

	// 진사람 처리
	while (raList)
	{
		if ( raList->m_raTarget == df && raList->m_bAttacker )		// 정당방위 리스트에 선빵이면 기본 페널티 -5 받기
		{
			if( of->IsChaotic() && !df->IsChaotic() )
				nPkBagicPenalty= 0;
			df->m_pkPenalty += nPkBagicPenalty;
		
			CNetMsg rmsg;
			CharStatusMsg(rmsg, df, 0);
			df->m_pArea->SendToCell(rmsg, df, false);
			df->m_bChangeStatus = true;

			char cChangeForDead = df->IsUpdateKing();
			if( cChangeForDead )
			{
				CNetMsg KingChangeMsg;		// MSG_HELPER_UPDATE_CHAOKING  bChao : 1(Guardian),2(ChaoKing) , index , penalty
				HelperUpdateKing( KingChangeMsg, cChangeForDead, of->m_index, of->m_pkPenalty );
				SEND_Q( KingChangeMsg, gserver.m_helper );
			}
		}
		raList = raList->m_next;
	}

	raList = of->m_raList;
	while (raList)
	{
		if (raList->m_raTarget == df && !raList->m_bAttacker )		// df 정당방위 리스트에 of가 있고, of가 선빵이 아니면 ( 무고한 희생자 )
		{
			if( raList->m_bRevenge == true )		// 죽은 사람이 상대를 때린적이 있으면 
			{
				nPkBagicPenalty += -10;
			}
			else
			{
				nPkBagicPenalty += -15;
			}
			break;
		}
		raList = raList->m_next;
	}

	if( raList == NULL )  // 정당방이 리스트에 없으면
		return;

	if( of->IsChaotic() )	// 카오 추가 페널티
	{
		nPkPlusCount = 1;

		// 추가 페널티
		if( df->IsChaotic() )	// 카오가 카오를 죽임
		{
			nPkPlusPenalty = -5;
		}
		else
		{
			switch( df->GetPKLevel() ) 
			{
			case PK_NAME_NORMAL	:			nPkPlusPenalty = 0;		break;	
			case PK_NAME_HUNTER	:			nPkPlusPenalty = -2;	break;
			case PK_NAME_HUNTER_MASTER :	nPkPlusPenalty = -5;	break;
			case PK_NAME_KNIGHT	:			nPkPlusPenalty = -10;	break;
			case PK_NAME_SAINT_KNIGHT :		nPkPlusPenalty = -15;	break;
			case PK_NAME_GUARDIAN :			nPkPlusPenalty = -20;	break;
			}
		}		
	}
	else	// 헌터 추가 페널티
	{
		// 추가 페널티
		if( !df->IsChaotic() )	// 헌터가 일반유저를 죽임
		{
			nPkPlusCount = 1;

			if( of->GetPKLevel() == PK_NAME_NORMAL ) // 공격이 일반유저
			{ nPkPlusPenalty = 0; }
			else
			{ nPkPlusPenalty = -20;	}
		}
		else	// 헌터가 카오를 죽임
		{
			nPkPlusCount = 0;

			switch( df->GetPKLevel() )
			{
			case PK_NAME_OUTLAW	:		nPkPlusPenalty = 25;	break;
			case PK_NAME_MURDERER :		nPkPlusPenalty = 30;	break;
			case PK_NAME_ASSESIN :		nPkPlusPenalty = 35;	break;
			case PK_NAME_DARK_KNIGHT:	nPkPlusPenalty = 40;	break;
			case PK_NAME_CHAOS_KING:	nPkPlusPenalty = 50;	break;
			}
		}		
	}

	// 추가 페널티
	of->m_pkCount += nPkPlusCount;
	if( of->m_pkCount > MAX_PK_COUNT )
		of->m_pkCount = MAX_PK_COUNT;

	if( of->IsChaotic() )
	{ of->m_pkPenalty += nPkBagicPenalty + nPkPlusPenalty - (of->m_pkCount/10);	}	// 카오 페널티 계산
	else
	{ of->m_pkPenalty += nPkBagicPenalty + nPkPlusPenalty; }	// 일반 페널티 계산 

	// 킹 변경
	char cChange = of->IsUpdateKing();
	if( cChange )
	{
		CNetMsg KingChangeMsg;		// MSG_HELPER_UPDATE_CHAOKING  bChao : 1(Guardian),2(ChaoKing) , index , penalty
		HelperUpdateKing( KingChangeMsg, cChange, of->m_index, of->m_pkPenalty );
		SEND_Q( KingChangeMsg, gserver.m_helper );
	}
#ifdef ADULT_QUEST
	CQuest* pQuest;
	CQuest* pQuestNext = of->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = of->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		switch ( pQuest->GetQuestType0() )
		{
		case QTYPE_KIND_PK:
			pQuest->QuestUpdateData(of, df);
			break;
		default:
			break;
		}
	}
	of->m_questList.CheckComplete(of);
#endif // ADULT_QUEST
#else // ADULT_SERVER

#ifndef DISABLE_PKPENALTY
	// 이전에 카오인지 검사

#ifdef FREE_PK_SYSTEM
if( !gserver.m_bFreePk )
	{
#endif // FREE_PK_SYSTEM

#ifdef MAL_DISABLE_PKPENALTY
	if( !gserver.m_bDisablePKPaenalty )
	{
#endif // MAL_DISABLE_PKPENALTY

		bool isPKChar = (of->m_pkPenalty < -9) ? true : false;

		if (!df->IsChaotic() || bDeadPet)
		{
			// 멀쩡한 사람 죽임 또는 펫 죽임
	#ifdef RESTRICT_PK
			if (of->IsChaotic())
				of->m_pkPenalty -= 10;
			else
				of->m_pkPenalty = -10;
	#else // RESTRICT_PK
			of->m_pkPenalty -= 10;
			if (!df->IsSetPlayerState(PLAYER_STATE_PKMODE))
				of->m_pkPenalty -= 10;
	#endif // RESTRICT_PK

			of->m_pkCount -= 1;

			if (of->m_pkPenalty < -155)
				of->m_pkPenalty = -155;
			if (of->m_pkCount < -110)
				of->m_pkCount = -110;
		}
		else
		{
			// 나쁜넘 죽임

			// 동렙 이상이면
			if (df->m_level >= of->m_level)
			{
	#ifdef RESTRICT_PK
				if (of->m_lastPKTime == -1 || of->m_lastPKTime + RESTRICT_PK <= gserver.m_gameTime || !of->IsChaotic())
				{
	#endif // RESTRICT_PK
	#if defined ( NON_PK_SYSTEM )
					if( !gserver.m_bNonPK )
						of->m_pkPenalty += 5;
	#else
					of->m_pkPenalty += 5;
	#endif	// NON_PK_SYSTEM
					if (of->m_pkPenalty > 155)
						of->m_pkPenalty = 155;
	#ifdef RESTRICT_PK
				}
	#endif // RESTRICT_PK
				of->m_pkCount += 1;

				if (of->m_pkCount > 110)
					of->m_pkCount = 110;
			}
		}

		if (of->m_pkPenalty < -9)
		{
			if (!isPKChar || of->m_pkRecoverPulse <= 0)
				of->m_pkRecoverPulse = PULSE_REAL_HOUR;
		}
#ifdef MAL_DISABLE_PKPENALTY
	}
#endif // MAL_DISABLE_PKPENALTY

#ifdef FREE_PK_SYSTEM
	}
#endif // FREE_PK_SYSTEM

#endif  // DISABLE_PKPANELTY

	of->m_bChangeStatus = true;

#endif // ADULT_SERVER

	// 페널티 수치 변경 알리기
	CNetMsg rmsg;
	CharStatusMsg(rmsg, of, 0);
	of->m_pArea->SendToCell(rmsg, of, false);
	of->m_bChangeStatus = true;
}

#ifdef ENABLE_WAR_CASTLE
void CalcWarPoint(CCharacter* of, CCharacter* df)
{
	// 공성이 이루어지고 있는 존에서
	int zoneindex = of->m_pZone->m_index;
	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
	if (castle == NULL)
		return ;
		
	// 공성중에 길드장 사망시 포인트 감소 : 10 %
	if (IS_PC(df))
	{
		CPC* dpc = TO_PC(df);
		if (dpc->m_guildInfo && dpc->m_guildInfo->guild()->boss()->charindex() == dpc->m_index)
		{
			if (dpc->GetJoinFlag(dpc->m_pZone->m_index) == WCJF_ATTACK_GUILD)
			{
				if (castle->GetState() == WCSF_WAR_CASTLE)
				{
					if (castle->IsTop3Guild(dpc->m_guildInfo->guild()->index()))
					{
						int point = castle->GetTeamPoint(dpc->m_guildInfo->guild()->index());
						point = (int)(point / 10.0 + 0.5);
						castle->IncreaseTeamPoint(dpc->m_guildInfo->guild()->index(), -point);
						gserver.m_playerList.SendGuildWarPointMsg(castle, dpc->m_pZone->m_index);
					}
				}
			}
		}
	} // 공성중에 길드장 사망시 포인트 감소 : 10 %

	if (castle->GetState() != WCSF_WAR_FIELD)
		return ;

	int mode = 0;			// 포인트 계산 방법, 없음(0), 수성1포인트(1), 공성1포인트(2), 공성20포인트(3)

	// 죽인 사람이 PC인가?
	if (IS_PC(of))
	{
		CPC* opc = TO_PC(of);

		if (IS_DEFENSE_TEAM(opc->GetJoinFlag(zoneindex)))
		{
			// 수성측이 죽인 거라면, 죽은 사람이 공성 측인가?
			if (IS_PC(df))
			{
				CPC* dpc = TO_PC(df);
				if (IS_ATTACK_TEAM(dpc->GetJoinFlag(zoneindex)))
				{
					mode = 1;
				}
			}
		}
		else if (opc->GetJoinFlag(zoneindex) == WCJF_ATTACK_GUILD)
		{
			// 공성길드원이 죽인 거라면
			// PC인가? 수호병인가
			if (IS_PC(df))
			{
				CPC* dpc = TO_PC(df);
				if (dpc->GetJoinFlag(zoneindex) != WCJF_NONE)
				{
					mode = 2;
				}
			}
			else if (IS_NPC(df))
			{
				CNPC* dnpc = TO_NPC(df);
				if (dnpc->m_proto->CheckFlag(NPC_CASTLE_GUARD))
				{
					mode = 3;
				}
			}
		}
	}
	else if (IS_NPC(of))
	{
		// 수호병이 죽였다면
		CNPC* onpc = TO_NPC(of);
		if (onpc->m_proto->CheckFlag(NPC_CASTLE_GUARD))
		{
			// 죽은게 공성측
			if (IS_PC(df))
			{
				CPC* dpc = TO_PC(df);
				if (IS_ATTACK_TEAM(dpc->GetJoinFlag(zoneindex)))
				{
					mode = 1;
				}
			}
		}
	}
	else
		return ;

	switch (mode)
	{
	case 1:			// 수성측 1포인트
		{
			int totalDamage = 0;
			int damage = 0;	// 수성측이 준 대미지 총합

			// PC를 공격했던 캐릭터 루프를 돌면서 수성측이 준 대미지 총합을 구한다
			CAttackChar* p = df->m_attackList;
			while (p)
			{
				if (p->ch)
				{
					totalDamage += p->m_damage;

					switch (p->ch->m_type)
					{
					case MSG_CHAR_PC:
						// 해당 존 수성측 유저라면
						if (IS_DEFENSE_TEAM(p->pc->GetJoinFlag(zoneindex)))
							damage += p->m_damage;
						break;
					case MSG_CHAR_NPC:
						// 수호병이라면
						if (p->npc->m_proto->CheckFlag(NPC_CASTLE_GUARD))
							damage += p->m_damage;
						break;
					default:
						return ;
					}
				}	// if (p->ch)

				p = p->m_next;
			}	// while (p)

			// 체력의 절반 이상 대미지를 준경우 포인트 증가
			if (totalDamage / 2 <= damage && damage > 0)
			{
				castle->IncreaseTeamPoint(0, WCFP_PLAYER);
			}
		}
		break;

	case 2:			// 공성측 1포인트
	case 3:			// 공성측 5포인트
		{
			int totalDamage = 0;
			int increasePoint = 0;
			if (mode == 2)
				increasePoint = WCFP_PLAYER;
			else
				increasePoint = WCFP_CASTLE_GUARD;

			int countAttackGuild = castle->GetCountAttackGuild();		// 공성 길드 수
			int* indexAttackGuild = new int[countAttackGuild];			// 공성 길드 인덱스
			int* damagePerAttackGuild = new int[countAttackGuild];		// 공성 길드별 대미지
			int maxDamageGuild = 0;										// 대미지를 가장 많이 준 길드 배열 인덱스
			int damageAttackTeam = 0;									// 공성측(길드, 용병)이 준 대미지 총합

			// 배열 초기화
			memset(indexAttackGuild, 0, sizeof(int) * countAttackGuild);
			memset(damagePerAttackGuild, 0, sizeof(int) * countAttackGuild);

			// df 공격했던 캐릭터 루프를 돌면서
			CAttackChar* p = df->m_attackList;
			while (p)
			{
				if (p->ch)
				{
					totalDamage += p->m_damage;

					if (IS_PC(p->ch))
					{
						// 해당 존 공성측 유저 중에, 길드 정보가 있는 유저이면
						if (p->pc->GetJoinFlag(zoneindex) == WCJF_ATTACK_GUILD && p->pc->m_guildInfo && p->pc->m_guildInfo->guild())
						{
							int i;
							for (i = 0; i < countAttackGuild; i++)
							{
								if (indexAttackGuild[i] == p->pc->m_guildInfo->guild()->index() || indexAttackGuild[i] == 0)
								{
									// 해당 길드 대미지 합계에 더하고
									indexAttackGuild[i] = p->pc->m_guildInfo->guild()->index();
									damagePerAttackGuild[i] += p->m_damage;

									// 공격측 길드 중에 가장 대미지를 많이 준 길드를 계산
									if (i != 0 && damagePerAttackGuild[i] >= damagePerAttackGuild[maxDamageGuild])
										maxDamageGuild = i;

									break;	// break for
								}
							}
						}
						if (IS_ATTACK_TEAM(p->pc->GetJoinFlag(zoneindex)))
							damageAttackTeam += p->m_damage;
					}
				}	// if (p->ch)

				p = p->m_next;
			}	// while (p)

			// 누적 대미지(또는 최대 체력)와 공격팀의 전체 대미지를 비교해서 공격팀이 50% 이상을 차지해야 포인트가 증가
			if (totalDamage / 2 <= damageAttackTeam && indexAttackGuild[maxDamageGuild] != 0)
			{
				castle->IncreaseTeamPoint(indexAttackGuild[maxDamageGuild], increasePoint);
			}

			delete [] indexAttackGuild;
			delete [] damagePerAttackGuild;
		}
		break;
	default:
		return ;
	}

	gserver.m_playerList.SendGuildWarPointMsg(castle, zoneindex);
}
#endif // #ifdef ENABLE_WAR_CASTLE

#ifdef ENABLE_PET
void ProcDeathPet(CPet* pet, const char* attackerType, int attackerIndex, const char* attackerName)
{
	DelAttackList(pet);

	CNetMsg rmsg;

	CPC* owner = pet->GetOwner();
	const char* ownerName = "NO OWNER";
	const char* ownerNick = "NO OWNER";
	const char* ownerID = "NO OWNER";
	if (owner)
	{
		ownerNick = (owner->IsNick()) ? owner->GetName() : ownerNick;
		ownerName = owner->m_name;
		ownerID = owner->m_desc->m_idname;
	}

	// TODO : petlog
	GAMELOG << init("PET DEAD")
			<< "PET" << delim
			<< pet->GetPetTypeGrade() << delim
			<< "INDEX" << delim
			<< pet->m_index << delim
			<< "LEVEL" << delim
			<< pet->m_level << delim
			<< "OWNER" << delim
			<< ownerName << delim
			<< ownerNick << delim
			<< ownerID << delim
			<< "ATTACKER" << delim
			<< "TYPE" << delim
			<< attackerType << delim
			<< "INDEX" << delim
			<< attackerIndex << delim
			<< "NAME" << delim
			<< attackerName << delim
			<< end;

	// TODO : DELETE
//	// 펫 사망시 아이템 지급
//	DropPetItem(pet);
//	const int nHorseDropList = 8;
//	const int nDragonDropList = 12;
//	int horseDropList[nHorseDropList][2] = {
//		{886, 25},			// 말굽
//		{888, 10},			// 말의갈기
//		{889, 25},			// 말총
//		{890, 10},			// 말의등뼈
//		{891, 25},			// 말의어금니
//		{892, 25},			// 말의피
//		{893, 10},			// 말가죽
//		{894, 50}			// 말의힘줄
//	};
//	int dragonDropList[nDragonDropList][2] = {
//		{895, 20},			// 발톱
//		{896, 20},			// 날개
//		{897, 20},			// 송곳니
//		{898, 20},			// 눈알
//		{899, 20},			// 뿔
//		{900, 20},			// 힘줄
//		{901, 20},			// 가죽
//		{902, 20},			// 꼬리
//		{903, 20},			// 피
//		{904, 20},			// 심장
//		{905, 20},			// 루비
//		{906, 20},			// 등뼈
//	};
//
//	// 펫 종류에 따라 드롭 테이블 변경
//	bool bNoDrop = true;
//	int i;
//	int prob = GetRandom(1, 10000);
//	int selIndex = 0;	// 아이템 인덱스
//
//	if (owner)
//	{
//		switch (pet->GetPetType())
//		{
//		case PET_TYPE_HORSE:
//			for (i = 0; i < nHorseDropList; i++)
//			{
//				horseDropList[i][1] = horseDropList[i][1] * pet->m_level / 2;
//				if (i > 0)
//					horseDropList[i][1] += horseDropList[i - 1][1];
//
//				if (prob <= horseDropList[i][1])
//				{
//					selIndex = horseDropList[i][0];
//					bNoDrop = false;
//					break;
//				}
//			}
//			break;
//		case PET_TYPE_DRAGON:
//			for (i = 0; i < nDragonDropList; i++)
//			{
//				dragonDropList[i][1] = dragonDropList[i][1] * pet->m_level / 2;
//				if (i > 0)
//					dragonDropList[i][1] += dragonDropList[i - 1][1];
//
//				if (prob <= dragonDropList[i][1])
//				{
//					selIndex = dragonDropList[i][0];
//					bNoDrop = false;
//					break;
//				}
//			}
//			break;
//		default:
//			bNoDrop = true;
//		}
//	}
//
//	if (!bNoDrop)
//	{
//		CItem* dropItem = gserver.m_itemProtoList.CreateItem(selIndex, -1, 0, 0, 1);
//		if (dropItem)
//		{
//			bool bDrop = false;
//			if (!AddToInventory(owner, dropItem, true, true))
//			{
//				pet->m_pArea->DropItem(dropItem, pet);
//				dropItem->m_preferenceIndex = owner->m_index;
//				ItemDropMsg(rmsg, pet, dropItem);
//				pet->m_pArea->SendToCell(rmsg, GET_YLAYER(dropItem), dropItem->m_cellX, dropItem->m_cellZ);
//				bDrop = true;
//			}
//			else
//			{
//				if (dropItem->tab() < 0)
//				{
//					int r, c;
//					if (owner->m_invenNormal.FindItem(&r, &c, dropItem->m_itemProto->m_index, 0, 0))
//					{
//						CItem* prev = owner->m_invenNormal.GetItem(r, c);
//						ItemUpdateMsg(rmsg, prev, 1);
//						SEND_Q(rmsg, owner->m_desc);
//					}
//					delete dropItem;
//				}
//				else
//				{
//					ItemAddMsg(rmsg, dropItem);
//					SEND_Q(rmsg, owner->m_desc);
//				}
//			}
//
//			GAMELOG << init("PET ITEM DROP")
//					<< "PET" << delim
//					<< pet->GetPetTypeGrade() << delim
//					<< "INDEX" << delim
//					<< pet->m_index << delim
//					<< "LEVEL" << delim
//					<< pet->m_level << delim
//					<< "OWNER" << delim
//					<< ownerName << delim
//					<< ownerNick << delim
//					<< ownerID << delim
//					<< "ITEM INDEX" << delim
//					<< dropItem->m_itemProto->m_index << delim
//					<< "ITEM NAME" << delim
//					<< dropItem->m_itemProto->m_name << delim
//					<< ((bDrop) ? "DROP" : "GIVE")
//					<< end;
//		}
//	}

	// 060221 : bs : 애완동물 사망시 착용 해제하고 이후 일정 시간동안 착용 불능
	if (owner)
	{
		// 착용 해제
		ItemWearMsg(rmsg, WEARING_PET, NULL, NULL);
		do_ItemWear(owner, rmsg);
		// 사망 설정
		pet->SetRemainRebirthTime();
		// 펫 상태 보냄
		ExPetStatusMsg(rmsg, pet);
		SEND_Q(rmsg, owner->m_desc);
	}

//	if (owner)
//		owner->DelPet(pet->m_index);

// TODO : DELETE
//	CPC* pc = pet->GetOwner();
//	int ownerIndex = 0;
//	if (pc)
//		ownerIndex = pc->m_index;
//
//	if (IS_RUNNING_HELPER)
//	{
//		HelperPetDeleteReqMsg(rmsg, pet->m_index, ownerIndex);
//		SEND_Q(rmsg, gserver.m_helper);
//		if (pc)
//			pc->m_desc->WaitHelperReply(true);
//	}
//	else
//	{
//		if (pc)
//			pc->DelPet(pet->m_index);
//		else
//			delete pet;
//	}
}

#endif // #ifdef ENABLE_PET

void ProcFollowNPC(CNPC* npc)
{
	CNetMsg rmsg;

	// 리더 플래그 0
	if (npc->m_proto->CheckLeaderBit(0) && npc->m_proto->m_family != -1)
	{
		// 주위에서 탱커 우선으로 최대 5인 HP/MP 회복
		int sx = npc->m_cellX - CELL_EXT;
		int ex = npc->m_cellX + CELL_EXT;
		int sz = npc->m_cellZ - CELL_EXT;
		int ez = npc->m_cellZ + CELL_EXT;
		
		int x, z;
		float dist[5] = {999.9f, 999.9f, 999.9f, 999.9f, 999.9f};
		CNPC* ret[5] = {NULL, NULL, NULL, NULL, NULL};
		
		int i = 0;
		// 일단 다른 층은 검사 안한다
		for (x = sx; x <= ex; x++)
		{
			if (x < 0 || x >= npc->m_pArea->m_size[0]) continue;
			for (z = sz; z <= ez; z++)
			{
				if (z < 0 || z >= npc->m_pArea->m_size[1]) continue;
				CCharacter *pChar;
				for (pChar = npc->m_pArea->m_cell[x][z].m_listChar; pChar; pChar = pChar->m_pCellNext)
				{
					// NPC만, 자신은 제외, 죽은넘 제외, 다른층 제외
					if (!IS_NPC(pChar) || pChar == npc || DEAD(pChar) || ABS(GET_YLAYER(npc) - GET_YLAYER(pChar)) >= 2)
						continue ;

					CNPC* tnpc = TO_NPC(pChar);

					// 동족이고
					if (tnpc->m_proto->m_family != npc->m_proto->m_family)
						continue ;

					// 기존과 타입검사 : 탱커, 자신의타입, 그외, 없음
					// 타입이 같으면 거리로 비교
					bool bReqSort = false;
					if (i < 5)
					{
						dist[i] = GetDistance(npc, tnpc);
						ret[i] = tnpc;
						i++;
						if (i == 5)
							bReqSort = true;
					}
					else
					{
						for (i = 0; i < 5; i++)
						{
							int resComp = AIComp(npc, tnpc, ret[i]);
							if (resComp < 0 || (resComp == 0 && dist[i] > GetDistance(npc, tnpc)))
							{
								dist[i] = GetDistance(npc, tnpc);
								ret[i] = tnpc;
								bReqSort = true;
								break;
							}
						}
					}

					// 거리 제일 먼 녀석이 앞에오게 정렬
					if (bReqSort)
					{
						int k;
						int j;
						int mindist;
						for (k = 0; k < 5 - 1; k++)
						{
							mindist = 0;
							for (j = 1; j < 5 - i; j++)
							{
								if (dist[j] < dist[mindist])
									mindist = j;
							}
							float tmpdist = dist[5 - k - 1];
							dist[5 - k - 1] = dist[mindist];
							dist[mindist] = tmpdist;
							CNPC* tmpNPC = ret[5 - k - 1];
							ret[5 - k - 1] = ret[mindist];
							ret[mindist] = tmpNPC;
						}
					}
				}
			}
		}

		for (i = 0; i < 5; i++)
		{
			if (ret[i])
			{
				ret[i]->m_hp = ret[i]->m_maxHP;
				ret[i]->m_mp = ret[i]->m_maxMP;
				CharStatusMsg(rmsg, ret[i], 0);
				ret[i]->m_pArea->SendToCell(rmsg, ret[i]);
			}
		}
	} // --- 리더 플래그 0

	// 리더 사망시 몹 소환
	if (npc->m_proto->m_aileader_idx > 0 && npc->m_proto->m_aileader_count > 0)
	{
		int i;
		for (i = 0; i < npc->m_proto->m_aileader_count; i++)
		{
			float x = 0, z = 0;
			int j;
			for (j = 0; j < 10; j++)
			{
				x = GET_X(npc) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 30) / 10.0f;
				z = GET_Z(npc) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 30) / 10.0f;

				switch (npc->m_pArea->GetAttr(GET_YLAYER(npc), x, z))
				{
				case MAPATT_FIELD:
				case MAPATT_STAIR_UP:
				case MAPATT_STAIR_DOWN:
					j = 9999;
					break;
				}
			}

			CNPC* rnpc = gserver.m_npcProtoList.Create(npc->m_proto->m_aileader_idx, NULL);
			if (!rnpc)
				return ;

			GET_X(rnpc) = x;
			GET_YLAYER(rnpc) = GET_YLAYER(npc);
			GET_Z(rnpc) = z;
			GET_R(rnpc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			
			rnpc->m_regenX = GET_X(rnpc);
			rnpc->m_regenY = GET_YLAYER(rnpc);
			rnpc->m_regenZ = GET_Z(rnpc);

			int cx, cz;
			npc->m_pArea->AddNPC(rnpc);
			npc->m_pArea->PointToCellNum(GET_X(rnpc), GET_Z(rnpc), &cx, &cz);
			npc->m_pArea->CharToCell(rnpc, GET_YLAYER(rnpc), cx, cz);
			
			CNetMsg appearNPCMsg;
			AppearMsg(appearNPCMsg, rnpc, true);
			npc->m_pArea->SendToCell(appearNPCMsg, GET_YLAYER(rnpc), cx, cz);
		}
	}
}

int AIComp(CNPC* base, CNPC* n1, CNPC* n2)
{
	if (n1 == NULL && n2 == NULL)
		return 0;
	if (n1 == NULL)
		return 1;
	if (n2 == NULL)
		return -1;

	int typeb = base->m_proto->m_aitype;
	int type1 = n1->m_proto->m_aitype;
	int type2 = n2->m_proto->m_aitype;

	if (type1 == type2)
		return 0;

	// 일반 몹은 최하 우선순위
	if (type1 != NPC_AI_NORMAL && type2 == NPC_AI_NORMAL)
		return -1;
	if (type1 == NPC_AI_NORMAL && type2 != NPC_AI_NORMAL)
		return 1;

	// 탱커 1순위
	if (type1 == NPC_AI_TANKER)			return -1;
	if (type2 == NPC_AI_TANKER)			return 1;
	// 동일타입 2순위
	if (type1 == typeb)					return -1;
	if (type2 == typeb)					return 1;
	// 그외 타입은 동일 순위
	return 0;
}

// of가 df를 죽이면 PK인지 검사
bool IsPK(CPC* of, CCharacter* df)
{
#ifdef DISABLE_PKPENALTY
	return false;
#endif // #ifdef DISABLE_PKPENALTY

#ifdef MAL_DISABLE_PKPENALTY
	if( gserver.m_bDisablePKPaenalty )
		return false;
#endif // MAL_DISALBE_PKPENALTY

#ifdef FREE_PK_SYSTEM
		if( gserver.m_bFreePk )
			return false;
#endif // FREE_PK_SYSTEM

	bool bPvP = false;
	CPC* dpc = NULL;
	switch (df->m_type)
	{
	case MSG_CHAR_PC:
		dpc = TO_PC(df);
		bPvP = true;
		break;
	case MSG_CHAR_PET:
		dpc = TO_PET(df)->GetOwner();
		break;
	case MSG_CHAR_ELEMENTAL:
		dpc = TO_ELEMENTAL(df)->GetOwner();
		break;
#ifdef ATTACK_PET
	case MSG_CHAR_APET:
		dpc = TO_APET(df)->GetOwner();
		break;
#endif //ATTACK_PET
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
	case MSG_CHAR_NPC:
		dpc = TO_NPC(df)->GetOwner();
		break;
#endif  // NIGHTSHADOW_SKILL	
	default:
		return false;
	}

	if (dpc == NULL)
		return false;

	// 기본적으로 죽는건 패널티가 폴스고, 페널티 주는건 트루
	// 뒈진넘이 pk모드 이거나 카오면 둘다 트루
	// 뒈진넘이 정방 리스트에 있고 카오가 아니고 공격자도 카오가 아니면 카오성향 수치를 계산하지 않는다
	// pk 패널티를 주는것 뿐만 아니라 성향회복에도 관계되므로 성향 회복이나 패널티등 어느것에라도 걸리면 true

	// of의 정당방위 리스트에 dpc가 있는가 : 정당방위 - 리스트에서 삭제후 Point 페널티 없음
	CRaChar* raList = of->m_raList;
	while (raList)
	{
		if (raList->m_raTarget == dpc)
		{
			// 카오가 일반을 정당방위로 죽일때 패널티를 위한 of
			// 일반이 카오를 정당방위로 죽일때 패널티 및 성향 상승을 위한 dpc
			// 둘다 일반이고 죽은넘이 공격했을때 m_bAttacker
#ifdef ADULT_SERVER
			if ( dpc->m_level <= PKMODE_LIMIT_LEVEL ||  of->m_level <= PKMODE_LIMIT_LEVEL )
#else
			if (raList->m_bAttacker && !dpc->IsChaotic() && !of->IsChaotic() )
#endif
				return false;

			break;
		}
			raList = raList->m_next;
	}

#ifdef LC_TLD
		// 둘다 카오가 아니고 칼키고 있으면 태국은 성향 수치 변화없음
		if( dpc->IsSetPlayerState(PLAYER_STATE_PKMODE) && of->IsSetPlayerState(PLAYER_STATE_PKMODE) && !of->IsChaotic() && !dpc->IsChaotic() )
			return false;
#endif

	// 길드전
	if (of->m_guildInfo && (of->m_guildInfo->guild()->battleState() == GUILD_BATTLE_STATE_ING) &&
		dpc->m_guildInfo && (dpc->m_guildInfo->guild()->battleState() == GUILD_BATTLE_STATE_ING))
	{
		if (of->m_guildInfo->guild()->battleIndex() == dpc->m_guildInfo->guild()->index() && 
			dpc->m_guildInfo->guild()->battleIndex() == of->m_guildInfo->guild()->index())
			return false;
	}

#ifdef ENABLE_WAR_CASTLE
		// 공성 도중 사망은 패널티 없음
		CWarCastle* castle = CWarCastle::GetCastleObject(of->m_pZone->m_index);
		if (castle)
		{
			if (castle->GetState() != WCSF_NORMAL)
			{
				if (of->GetMapAttr() == MAPATT_WARZONE &&
					df->GetMapAttr() == MAPATT_WARZONE)
					return false;
			}
		}
#endif

	/////////////////////////////////////////////
	// BANGWALL : 2005-07-18 오전 11:27:24
	// Comment : freepkzone 패널티 없음
	// 공격자와 방어자가 모두 freepkzone에 있으면 pkpenalty 없음
	if( of->GetMapAttr() == df->GetMapAttr() && of->GetMapAttr() == MAPATT_FREEPKZONE)
		return false;

	return true;
}
// npc attackList중 어택 제한 Pulse 초과한 경우 어택리스트에서 삭제
// hate 수치 가장 높은 pc 반환
// 
CCharacter* CheckAttackPulse(CNPC* npc)
{
	CAttackChar* target;
	CAttackChar* targetNext = npc->m_attackList;

	int hate = 0;
	CCharacter* retHate = NULL;
	int lowhp = 0x7fffffff;
	int lowhp_hate = 0;
	CCharacter* retLowHP = NULL;

	int checkPulse;

	while ((target = targetNext))
	{
		targetNext = targetNext->m_next;

		// 미네랄, 농작물, 에너지
		if (npc->m_proto->CheckFlag(NPC_MINERAL | NPC_CROPS | NPC_ENERGY))
			checkPulse = NPC_PRODUCE_DELETE_PULSE;
		// 그외 기타 NPC
		else
			checkPulse = NPC_ATTACK_DELETE_PULSE;

		// 타겟이 때린 데미지가 있고 Pulse가 초과한 경우 리스트에서 삭제
		if (target->m_damage > 0 && gserver.m_pulse - target->m_targetPulse > checkPulse)
		{
			// target의 attackList에서 npc 삭제
			CAttackChar* tmp;
			CAttackChar* tmpNext = target->ch->m_attackList;
			while ((tmp = tmpNext))
			{
				tmpNext = tmpNext->m_next;

				if (tmp->ch == npc)
				{
					REMOVE_FROM_BILIST(tmp, target->ch->m_attackList, m_prev, m_next);
					delete tmp;
					tmp = NULL;
				}
			}
#ifndef NEW_DIVISION_EXPSP			
			npc->m_totalDamage -= target->m_damage;
#endif // #ifndef NEW_DIVISION_EXPSP

			REMOVE_FROM_BILIST(target, npc->m_attackList, m_prev, m_next);
			delete target;
			target = NULL;
			continue;
		}

		// 050315 : bs : 타겟 볼수 없으면 스킵
		if (!target)
			continue ;
		if (!npc->CanVisible(target->ch))
			continue ;

		if ((target->ch->m_assist.m_state & AST_FAKEDEATH))
			continue ;

		// 시야범위안에 없으면 타겟이 될 수 없다.
		if (!CheckInNearCellExt(npc, target->ch))
			continue ;

		// 층이 다르고 높이가 5미터 이상이면 무시
		if (ABS(GET_YLAYER(npc) - GET_YLAYER(target->ch)) > 1 && ABS(GET_H(npc) - GET_H(target->ch)) > 5.0f)
			continue ;

#ifdef NO_TARGET_IN_PEACEZONE
		// 피스존 안에서는 타겟이 될 수 없다
		if (target->ch->IsInPeaceZone(false))
			continue ;
#endif // NO_TARGET_IN_PEACEZONE

		if (hate <= target->m_targetHate && target->m_targetHate != 0)
		{
			hate = target->m_targetHate;
			retHate = target->ch;
		}
		if (lowhp > target->ch->m_hp && target->m_targetHate != 0)
		{
			lowhp = target->ch->m_hp;
			lowhp_hate = target->m_targetHate;
			retLowHP = target->ch;
		}
	}

	switch (npc->m_proto->m_aitype)
	{
	case NPC_AI_TANKER:
		// 자신의 체력이 20% 미만이면 hate 높은 대상으로부터 도망 : flag 2
		if ( npc->m_proto->CheckAIBit(2) && npc->m_hp < npc->m_maxHP * 20 / 100 )
			return retHate;

		// 피가 제일 적은 대상이 최대 hate를 갖는 대상의 hate 절반 이상이면 피 적은 녀석을 공격 : flag 0
		if ( npc->m_proto->CheckAIBit(0) && hate  / 2 < lowhp_hate )
		{
			// 체력이 적은 대상과 거리가 10미터 이상 차이나면 hate 높은 대상을 공격 : flag 1
			if ( npc->m_proto->CheckAIBit(1) && GetDistance(retLowHP, npc) >= 10.0f )
				return retHate;

			// 체력 낮은 적과 hate 높은 적이 HP가 같으면 hate 높은 적
			if (retHate && retLowHP && retHate->m_hp == retLowHP->m_hp)
				return retHate;

			return retLowHP;
		}

		return retHate;

	case NPC_AI_NORMAL:
	case NPC_AI_DAMAGEDEALER:
	case NPC_AI_HEALER:
	default:
		return retHate;
	}
}

#ifdef EVENT_TEACH_2007
bool IsTeachAndStudent( CParty* pParty )
{
	bool bTeachAndStudent = false;

	CPC* pBoss = gserver.m_playerList.Find( pParty->GetBossIndex() );
	if( pBoss == NULL )
		return false;
	if( pBoss->m_teachType == MSG_TEACH_TEACHER_TYPE )
	{
		for( int i = 1; i < MAX_PARTY_MEMBER; i++ )
		{
			int partyCharidx = pParty->GetMemberCharIndex( i );
			if( partyCharidx == -1 )
				continue;

			CPC* pPartyMember = gserver.m_playerList.Find( partyCharidx );
			if( pPartyMember == NULL )
				continue;

			if( pPartyMember->m_teachType != MSG_TEACH_STUDENT_TYPE )
			{
				bTeachAndStudent = false;
				break;
			}
			else 
			{
				for( int num =  0; num < TEACH_MAX_STUDENTS; num++ )
				{
					if( pBoss->m_teachIdx[num] == partyCharidx )
						bTeachAndStudent = true;
					else
					{
						bTeachAndStudent = false;
						break;
					}
				}
				break;
			}

		
		}
	}
	if( !bTeachAndStudent )
		return false;

	return true;
}
#endif //EVENT_TEACH_2007