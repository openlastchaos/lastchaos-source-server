#include "stdhdrs.h"
#include "CmdMsg.h"
#include "Server.h"
#include "Exp.h"
#include "Log.h"
#include "Battle.h"
#include "hardcoding.h"
#include "WarCastle.h"

////////////////////
// Function name	: GetHitType
// Description	    : 공격 타입 결정
// Return type		: char 
//                  : 가능한 공격 타입 비트 필드
// Argument         : CCharacter* of
//                  : 공격자
// Argument         : CCharacter* df
//                  : 방어자
char GetHitType(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type)
{
#ifdef ADULT_SERVER_NEW_BALANCE
	if (of && df)
		return GetHitType_Adult(of, df, type);
#endif // ADULT_SERVER_NEW_BALANCE

	// 펙터에 따른 값들
	int ret = 0;

	int prob = 0;
	int prob2 = 0, prob3 = 0;

	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
		switch (of->m_type)
		{		
		case MSG_CHAR_NPC:
			prob = 145 * (10 + of->m_level) * of->m_dex / (10 + df->m_level) / (of->m_dex + df->m_dex);
			break;

		case MSG_CHAR_PC:
			switch (df->m_type)
			{
			case MSG_CHAR_PC:
			case MSG_CHAR_ELEMENTAL:	// 소환수는 PC와 동일
				switch (TO_PC(of)->m_job)
				{
				case JOB_TITAN:
					prob = 200 * (10 + of->m_level) * of->m_dex / (10 + df->m_level) / (of->m_dex + df->m_dex * 45 / 100);
					break;

				case JOB_KNIGHT:
					prob = 200 * (10 + of->m_level) * of->m_dex / (10 + df->m_level) / (of->m_dex + df->m_dex * 76 / 100);
					break;

				default:
					prob = 200 * (10 + of->m_level) * of->m_dex / (10 + df->m_level) / (of->m_dex + df->m_dex);
					break;
				}
				break;

			case MSG_CHAR_NPC:
				switch (TO_PC(of)->m_job)
				{
				case JOB_TITAN:
					prob = 200 * (10 + of->m_level) * of->m_dex / (10 + df->m_level) / (of->m_dex + (df->m_dex * 45 / 100));
					break;

				case JOB_KNIGHT:
					prob = 200 * (10 + of->m_level) * of->m_dex / (10 + df->m_level) / (of->m_dex + (df->m_dex * 75 / 100));
					break;

				case JOB_HEALER:
				case JOB_MAGE:
				case JOB_ROGUE:
				case JOB_SORCERER:
					prob = 200 * (10 + of->m_level) * of->m_dex / (10 + df->m_level) / (of->m_dex + df->m_dex);
					break;
				}
				break;

			case MSG_CHAR_PET:
				prob = 100;
				break;

			default:
				prob = 0;
				break;
			}
			break;

		case MSG_CHAR_PET:
			prob = 100;
			break;

		case MSG_CHAR_ELEMENTAL:		// 소환수는 소서러와 동일
			switch (df->m_type)
			{
			case MSG_CHAR_PC:
			case MSG_CHAR_ELEMENTAL:	// 소환수는 PC와 동일
				prob = 200 * (10 + of->m_level) * of->m_dex / (10 + df->m_level) / (of->m_dex + df->m_dex);
				break;

			case MSG_CHAR_NPC:
				prob = 200 * (10 + of->m_level) * of->m_dex / (10 + df->m_level) / (of->m_dex + df->m_dex);
				break;

			case MSG_CHAR_PET:
				prob = 100;
				break;

			default:
				prob = 0;
				break;
			}
			break;

		default:
			prob = 0;
			break;
		}

		prob *= 100;
		
		prob2 = of->m_avPassiveAddition.hitrate + (prob * of->m_avPassiveRate.hitrate / SKILL_RATE_UNIT);
		prob2 += of->m_assist.m_avAddition.hitrate + (prob * of->m_assist.m_avRate.hitrate / SKILL_RATE_UNIT);
		
		prob3 = df->m_avPassiveAddition.avoid + (prob * df->m_avPassiveRate.avoid / SKILL_RATE_UNIT);
		prob3 += df->m_assist.m_avAddition.avoid + (prob * df->m_assist.m_avRate.avoid / SKILL_RATE_UNIT);
		
		prob += prob2 - prob3;

		break;

	case MSG_DAMAGE_MAGIC:
		switch (of->m_type)
		{
		case MSG_CHAR_NPC:
			prob = 130 * (10 + of->m_level) * of->m_int / (10 + df->m_level) / (of->m_int + df->m_int);
			break;

		case MSG_CHAR_PC:
		case MSG_CHAR_ELEMENTAL:
			prob = 200 * (10 + of->m_level) * of->m_int / (10 + df->m_level) / (of->m_int + df->m_int);
			break;

		case MSG_CHAR_PET:		// 애완동물은 마법 공격 없음
		default:
			prob = 0;
			break;
		}

		prob *= 100;

		prob2 = of->m_avPassiveAddition.magichitrate + (prob * of->m_avPassiveRate.magichitrate / SKILL_RATE_UNIT);
		prob2 += of->m_assist.m_avAddition.magichitrate + (prob * of->m_assist.m_avRate.magichitrate / SKILL_RATE_UNIT);
		
		prob3 = df->m_avPassiveAddition.magicavoid + (prob * df->m_avPassiveRate.magicavoid / SKILL_RATE_UNIT);
		prob3 += df->m_assist.m_avAddition.magicavoid + (prob * df->m_assist.m_avRate.magicavoid / SKILL_RATE_UNIT);
		
		prob += prob2 - prob3;

		break;

	default:
		break;
	}

	// 옵션 수치 적용
	int op = 0;
	switch (type)
	{
	case MSG_DAMAGE_MELEE:
		op = (of->m_opMeleeHitRate + of->m_avPassiveRate.HitRate_2033) - (df->m_opMeleeAvoid + df->m_avPassiveRate.Avoid_2034);
		break;

	case MSG_DAMAGE_RANGE:
		op = (of->m_opRangeHitRate + of->m_avPassiveRate.HitRate_2033) - (df->m_opRangeAvoid + df->m_avPassiveRate.Avoid_2034);
		break;

	case MSG_DAMAGE_MAGIC:
		op = (of->m_opMagicHitRate + of->m_avPassiveRate.HitRate_2033) - (df->m_opResistAvoid + df->m_avPassiveRate.Avoid_2034);
		break;

	default:
		break;
	}

	prob += (op * 100);

	// 레벨 페널티
	// 소환수/펫도 PC와 동일하게
	bool bIsPCof = (IS_PC(of) || IS_PET(of) || IS_ELEMENTAL(of));
	bool bIsPCdf = (IS_PC(df) || IS_PET(df) || IS_ELEMENTAL(df));
	if (bIsPCof && bIsPCdf)
	{
		if (of->m_level < 31)
			prob = prob * ((of->m_level - df->m_level) * 2 + 100) / 100;
		else if (of->m_level < 41)
			prob = prob * ((of->m_level - df->m_level) * 4 + 100) / 100;
		else if (of->m_level < 51)
			prob = prob * ((of->m_level - df->m_level) * 6 + 100) / 100;
		else if (of->m_level < 61)
			prob = prob * ((of->m_level - df->m_level) * 8 + 100) / 100;
		else
			prob = prob * ((of->m_level - df->m_level) * 10 + 100) / 100;
	}

#ifdef LIMIT_AVOID
	if (prob < LIMIT_AVOID)
		prob = LIMIT_AVOID;
#endif // LIMIT_AVOID

	if (GetRandom(1, 10000)  > prob)
		ret = HITTYPE_MISS;
	else
		ret = HITTYPE_WEAK | HITTYPE_NORMAL | HITTYPE_STRONG | HITTYPE_HARD | HITTYPE_CRITICAL | HITTYPE_DEADLY;

	return ret;
}




////////////////////
// Function name	: SelectHitType
// Description	    : 주어진 flag에서 특정 타입을 선택
#ifdef ENABLE_ROGUE_SKILL125_BRZ
char SelectHitType(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel, bool nothelp, int skillLevel)
#else
char SelectHitType(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel, bool nothelp)
#endif // ENABLE_ROGUE_SKILL125_BRZ
{
#define SAFEDIV(a, b)		{ if ((b) < 1) a /= 1; else a /= (b); }		// a = a / b, b가 0이면 1로 간주

	if (magic && magiclevel && magic->m_hittype == MHT_CONSTANT)
	{
		int hitrate = magiclevel->m_nHitrateValue;
		if (magic->m_type == MT_ATTACK)
		{
			int levdiff = of->GetAttackLevel() - of->m_level;
			if (levdiff > 20)
				hitrate = hitrate * 20 / 100;
			else if (levdiff > 15)
				hitrate = hitrate * 55 / 100;
			else if (levdiff > 10)
				hitrate = hitrate * 70 / 100;
			else if (levdiff > 5)
				hitrate = hitrate * 85 / 100;
		}
#ifdef ENABLE_ROGUE_SKILL125_BRZ
		if(magic->m_type == MT_ASSIST && magic->m_subtype == MST_ASSIST_HOLD)
		{
			int diffLevel = df->m_level - of->m_level;
			if(diffLevel > 5)		// 빨간색 몹
			{
				switch(skillLevel)
				{
				case 1:		hitrate = 1000;  break;
				case 2:		hitrate = 1500;  break;
				case 3:		hitrate = 2000;  break;
				case 4:		hitrate = 2500;  break;
				case 5:		hitrate = 3000;  break;
				default:	hitrate = 1000;  break;
				}
			}
			else if(diffLevel > 2)				// 주황색 몹
			{
				switch(skillLevel)
				{
				case 1:		hitrate = 3000;  break;
				case 2:		hitrate = 3500;  break;
				case 3:		hitrate = 4000;  break;
				case 4:		hitrate = 4500;  break;
				case 5:		hitrate = 5000;  break;
				default:	hitrate = 3000;  break;
				}
			}
		}
#endif // ENABLE_ROGUE_SKILL125_BRZ

// 050228 : bs : 보스, 중간보스 스턴 확률 1/10으로 감소
		if (IS_NPC(df) && (TO_NPC(df)->m_flag & (NPC_BOSS | NPC_MBOSS)) && magic->m_type == MT_ASSIST && magic->m_subtype == MST_ASSIST_STURN)
		{
			hitrate /= 10;
			if (hitrate < 1)
				hitrate = 1;
		}
// --- 050228 : bs : 보스, 중간보스 스턴 확률 1/10으로 감소

// 050331 : bs : pvp에서 명중률 증감
// 050402 : bs : 시전, 대상의 파라미터 추가
		hitrate = hitrate * CalcSkillParam(of, df, magic->m_hsp, magic->m_htp) / 100;
		if (hitrate < 1)
			hitrate = 1;

		bool bIsPCof = (IS_PC(of) || IS_PET(of) || IS_ELEMENTAL(of));
		bool bIsPCdf = (IS_PC(df) || IS_PET(df) || IS_ELEMENTAL(df));
		if (bIsPCof && bIsPCdf && nothelp)
			hitrate = ((of->m_level - df->m_level) * 5 + 100) * hitrate / 100;

		hitrate += of->m_avPassiveAddition.hitrate_skill
				+  hitrate * of->m_avPassiveRate.hitrate_skill / SKILL_RATE_UNIT
				+  of->m_assist.m_avAddition.hitrate_skill
				+  hitrate * of->m_assist.m_avRate.hitrate_skill / SKILL_RATE_UNIT;

		if (IS_PC(df) && magic->m_type == MT_ASSIST)
		{
			int nDecreaseHitrate = 0;
			CPC* pPCDefense = TO_PC(df);
			switch (magic->m_subtype)
			{
			case MST_ASSIST_STONE:		nDecreaseHitrate = pPCDefense->m_opResistStone;		break;
			case MST_ASSIST_STURN:		nDecreaseHitrate = pPCDefense->m_opResistSturn;		break;
			case MST_ASSIST_SILENT:		nDecreaseHitrate = pPCDefense->m_opResistSilent;	break;
			}
			if (nDecreaseHitrate > 0)
				hitrate -= hitrate * nDecreaseHitrate / 100;
			if (hitrate < 1)
				hitrate = 1;
		}

		if (GetRandom(1, 10000) <= hitrate)
		{
			char ret = (char)HITTYPE_NORMAL;
			if (magic->m_type == MT_ATTACK)
			{
				switch (magic->m_subtype)
				{
				case MST_ATTACK_CRITICAL:
					ret = (char)HITTYPE_CRITICAL;
					break;
				case MST_ATTACK_DEADLY:
					ret = (char)HITTYPE_DEADLY;
					break;
				case MST_ATTACK_HARD:
					ret = (char)HITTYPE_HARD;
					break;
				default:
#ifdef LC_JPN_SKILL_CRITICAL
					ret = SelectHitType(of, df, type, HITTYPE_WEAK | HITTYPE_NORMAL | HITTYPE_STRONG | HITTYPE_HARD | HITTYPE_CRITICAL | HITTYPE_DEADLY, NULL, NULL, true);
					switch (ret)
					{
					case HITTYPE_WEAK:
						ret = HITTYPE_NORMAL;
						break;
					case HITTYPE_NORMAL:
					case HITTYPE_STRONG:
					case HITTYPE_HARD:
					case HITTYPE_CRITICAL:
					case HITTYPE_DEADLY:
						break;
					}
#endif // LC_JPN_SKILL_CRITICAL
					break;
				}
			}
			return ret;
		}
		else
			return (char)HITTYPE_MISS;
	}

#ifdef ADULT_SERVER_NEW_BALANCE
	return SelectHitType_Adult(of, df, type, flag, magic, magiclevel, nothelp);
#endif // ADULT_SERVER_NEW_BALANCE

	int hitprob = 0;
	int maxprob = 10000;
	int i;

	if (flag & HITTYPE_MISS)
		return HITTYPE_MISS;


	// 각 타입별 확률 : 미스, 노멀을 제외하고 순서대로 [0위크 1스트롱 2하드 3크리티컬 4데들리]
	// pc attack 기준
	int prob_t[5] = {500, 1250, 1000, 900, 600};
	int ret_t[5] = {HITTYPE_WEAK, HITTYPE_STRONG, HITTYPE_HARD, HITTYPE_CRITICAL, HITTYPE_DEADLY};

	// NPC : 하드 크리 데들리 없기 확률 조정
	if (IS_NPC(of))
	{
// 050225 : bs : 보스-크리&데들리, 중간보스-크리 가능하게

		prob_t[0] = 2000;		// 위크			: 20%
		prob_t[1] = 2000;		// 스트롱		: 20%

		CNPC* npc = TO_NPC(of);
		if (npc->m_proto->CheckFlag(NPC_BOSS))
		{
			prob_t[2] = 1000;	// 하드			: 10%
			prob_t[3] = 900;	// 크리			: 9%
			prob_t[4] = 600;	// 데들리		: 6%
		}
		else if (npc->m_proto->CheckFlag(NPC_MBOSS))
		{
			prob_t[2] = 1000;	// 하드			: 10%
			prob_t[3] = 900;	// 크리			: 9%
			prob_t[4] = 0;		// 데들리		: 0%
		}
		else
		{
			prob_t[2] = 0;		// 하드			: 0%
			prob_t[3] = 0;		// 크리			: 0%
			prob_t[4] = 0;		// 데들리		: 0%
		}

// --- 050225 : bs : 보스-크리&데들리, 중간보스-크리 가능하게
	}

// 050401 : bs : plus에 따른 추가 효과
// 각 타입별 확률 : 미스, 노멀을 제외하고 순서대로 [위크 스트롱 하드 크리티컬 데들리]
	if (IS_PC(df))
	{
		CPC* pc = TO_PC(df);
		prob_t[0] += pc->m_plusWeak;		// 방어자 위크 증가
	}
	if (IS_PC(of))
	{
		CPC* pc = TO_PC(of);
		prob_t[1] += pc->m_plusStrong;		// 공격자 강공 증가
		prob_t[2] += pc->m_plusHard;		// 공격자 매강공 증가
		// 크리티컬 확률 증가
		if (pc->m_opAttackCritical > 0)
			prob_t[3] += prob_t[3] * pc->m_opAttackCritical / 100;
	}
// --- 050401 : bs : plus에 따른 추가 효과

	// PC : 크리티컬, 데들리 상승
	prob_t[3] += of->m_avPassiveAddition.critical
			+  prob_t[3] * of->m_avPassiveRate.critical / SKILL_RATE_UNIT;
	prob_t[4] += of->m_avPassiveAddition.deadly
			+  prob_t[4] * of->m_avPassiveRate.deadly / SKILL_RATE_UNIT;
	prob_t[3] += of->m_assist.m_avAddition.critical
			+  prob_t[3] * of->m_assist.m_avRate.critical / SKILL_RATE_UNIT;
	prob_t[4] += of->m_assist.m_avAddition.deadly
			+  prob_t[4] * of->m_assist.m_avRate.deadly / SKILL_RATE_UNIT;

	// 크리 최대 확률 : 레인저 제외
	if (!IS_PC(of) || TO_PC(of)->m_job != JOB_ROGUE || TO_PC(of)->m_job2 != JOB_2ND_RANGER)
	{
		if (prob_t[3] > 2000)
			prob_t[3] = 2000;
	}
	else
	{
		if (prob_t[3] > 5000)
			prob_t[3] = 5000;
	}

	// 데들리 최대 확률
	if (prob_t[4] > 1000)
		prob_t[4] = 1000;

#ifdef ENABLE_WAR_CASTLE
	if (IS_NPC(df))
	{
		CNPC* npc = TO_NPC(df);
		// 맞는 쪽이 수성오브젝트이면 크리, 데들리 없음
		if (npc->m_proto->CheckFlag(NPC_CASTLE_TOWER))
		{
			prob_t[3] = 0;		// 크리			: 0%
			prob_t[4] = 0;		// 데들리		: 0%
		}
	}
#endif

	for (i = 1; i < 5; i++)
		prob_t[i] += prob_t[i - 1];

	if (prob_t[4] > maxprob)
		maxprob = prob_t[4];

	hitprob = GetRandom(1, maxprob);

	for (i = 0; i < 5; i++)
	{
		if (hitprob <= prob_t[i])
			return (char)ret_t[i];
	}

	return (char)HITTYPE_NORMAL;
#undef SAFEDIV
}

////////////////////
// Function name	: GetDamage
// Description	    : 대미지를 얻음
// Return type		: int 
//                  : 얻어진 대미지
// Argument         : CCharacter* of
//                  : 공격자
// Argument         : CCharacter* df
//                  : 방어자
// Argument         : MSG_DAMAGE_TYPE type
//                  : 공격 종류
// Argument         : char flag
//                  : 공격 형태(미스 ~ 데들리)
// Argument         : const CMagicProto* magic
//                  : 사용 효과
// Argument         : const CMagicLevelProto* magiclevel
//                  : 사용 효과 레벨
int GetDamage(const CCharacter* of, const CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel)
{
	double damFactor = 5;
	int statFactor = 15;
	
	if (IS_PC(df))
	{
//		((CPC*)of)->SetPlayerState(PLAYER_STATE_PKMODE);
//		((CPC*)of)->CancelInvisible();
#ifdef ADULT_SERVER
		if( IS_PC(of) )
		{
			if( IsRaList( (CPC*)df, (CPC*)of ) )
			{
				if( ((CPC*)df)->m_raList->m_bAttacker == true )
					damFactor = 3.5;
			}
		}
#endif // ADULT_SERVER
		damFactor = 3.5;
	}

	int ret = 0;

#ifdef NEW_BALANCE
	const int nPvPSkillFactor = 3;
	int minDamage = 1;		// 최소 대미지
	int deadlyDamage = 0;

	// 스킬없는 데들리 데미지를 구한다
	if (magic != NULL)
	{
		deadlyDamage = GetDamage(of, df, of->GetAttackType(NULL), HITTYPE_DEADLY, NULL, NULL);
	}
#endif // #ifdef NEW_BALANCE

	int nMagicPowerAddition = 0;
	int nMagicPowerRate = 0;

	if (flag == HITTYPE_MISS)
		return 0;

#ifdef ENABLE_PET
	if (IS_PET(df))
	{
		return 1;
	}
#endif
	
	if (magic)
	{
		int nMagicPowerValue = magiclevel->m_nPowerValue;

// 050331 : bs : pvp에서는 데미지가 증감함
// 050402 : bs : 공격, 방어자의 스킬 파라미터 추가
		nMagicPowerValue = nMagicPowerValue * CalcSkillParam(of, df, magic->m_psp, magic->m_ptp) / 100;
// --- 050402 : bs : 공격, 방어자의 스킬 파라미터 추가

		bool bIsPCof = (IS_PC(of) || IS_PET(of) || IS_ELEMENTAL(of));
		bool bIsPCdf = (IS_PC(df) || IS_PET(df) || IS_ELEMENTAL(df));
		if (bIsPCof && bIsPCdf)
			nMagicPowerValue = ((of->m_level - df->m_level) * 5 + 100) * nMagicPowerValue / 100;

		if (nMagicPowerValue < 1)
			nMagicPowerValue = 1;
// --- 050331 : bs : pvp에서는 데미지가 증감함

		switch (magic->m_attribute)
		{
		case AT_FIRE:
		case AT_WATER:
		case AT_EARTH:
		case AT_WIND:
		case AT_DARK:
		case AT_LIGHT:
			nMagicPowerValue = nMagicPowerValue * (100 - df->m_attribute.GetValue(magic->m_attribute)) / 100;
			break;
		}

		if (magiclevel)
		{
			switch (magic->m_damagetype)
			{
			case MDT_ONLYPOWER:
#ifdef LC_JPN_SKILL_CRITICAL

#ifdef NEW_BALANCE
				switch (flag)
				{
				case HITTYPE_STRONG:
					return nMagicPowerValue * 13 / 10;
				case HITTYPE_HARD:
					return nMagicPowerValue * 15 / 10;
				case HITTYPE_CRITICAL:
					return nMagicPowerValue * 2;
				case HITTYPE_DEADLY:
					return nMagicPowerValue * 25 / 10;
				default:
					return nMagicPowerValue;
				}
				break;
#else // #ifdef NEW_BALANCE
				return nMagicPowerValue;
#endif // #ifdef NEW_BALANCE

#else // LC_JPN_SKILL_CRITICAL

				return nMagicPowerValue;

#endif // LC_JPN_SKILL_CRITICAL

			case MDT_ADDITION:
				nMagicPowerAddition = nMagicPowerValue;
				break;
			case MDT_RATE:
				nMagicPowerRate = nMagicPowerValue;
				break;
			}
		}
		else
			return 0;
	}

	// 아이템 페널티 관련
	int adef = of->GetAttackLevel() - of->m_level;
	int ddef = df->GetDefenseLevel() - df->m_level;


// 050311 : bs : 물리 공격 종류(근접,원거리)과 스탯을 분리
	int usingstat = 0;
	switch (of->GetUsingStat())
	{
	case USING_STR:
		usingstat = of->m_str;
		break;
	case USING_DEX:
		usingstat = of->m_dex;
		break;
	}
// --- 050311 : bs : 물리 공격 종류(근접,원거리)과 스탯을 분리

	// 최대 제한 걸리는 스탯 상승
	int addattack80 = 0;
	int rateattack80 = 0;

	int warBonusAttack = 100;
	int warBonusDefense = 100;

#ifdef DOUBLE_ATTACK
	int DoubleAttackRate = 0;
#endif //DOUBLE_ATTACK

#ifdef ENABLE_WAR_CASTLE
	CWarCastle* castle = CWarCastle::GetCastleObject(of->m_pZone->m_index);

	if (castle && castle->GetState() != WCSF_NORMAL)
	{
		CPC* pc;
		if (IS_PC(of))
		{
			pc = TO_PC(of);
			if (IS_ATTACK_TEAM(pc->GetJoinFlag(pc->m_pZone->m_index)) 
				&& castle->IsInRegenPoint(pc, true))	
			{
				warBonusAttack = 120;
			}
		}

#ifdef DRATAN_CASTLE
		if( IS_PC(of) )
		{
			pc = TO_PC(of);
			if( castle->IsInInnerCastle( pc ) )
				warBonusAttack = 50;
		}
#endif // DRATAN_CASTLE

		if (IS_PC(df))
		{
			pc = TO_PC(df);
			if (IS_ATTACK_TEAM(pc->GetJoinFlag(pc->m_pZone->m_index)) 
				&& castle->IsInRegenPoint(pc, true) 
				|| IS_DEFENSE_TEAM(pc->GetJoinFlag(pc->m_pZone->m_index))
				&& castle->IsInRegenPoint(pc, false))
			{
				warBonusDefense = 120;
			}
		}
	}
#endif


#ifdef DYNAMIC_DUNGEON
	int nCharRate[10][3] = {	// 공격력, 방어력, 이동 속도
			{-20, -20, 0},
			{-10, -10, 0},
			{0, 0, 0},
			{3, 3, 0},
			{5, 5, 0},
			{7, 7, 0},
			{10, 10, 0},
			{13, 13, 0},
			{16, 16, 0},
			{20, 20, 10},
		};

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	int cidx = -1;
	if( pCastle != 0 && pCastle->m_dvd.GetZone() != 0)
	{
		cidx = pCastle->m_dvd.GetEnvRate()/10;
		if(cidx > 9)
		{
			cidx = 9;
		}
	}
	
#endif // DYNAMIC_DUNGEON

	switch (type)
	{
	case MSG_DAMAGE_MELEE:
		{
			int awdamage	= of->m_eqMelee
							+ of->m_opMelee;

#ifdef ATTACK_PET
			if( IS_APET(of) )
			{
				CAPet* apet = TO_APET(of);
				awdamage += apet->m_nAttack;
			}
#endif //ATTACK_PET

			awdamage	+= of->m_avPassiveAddition.attack
						+  awdamage * of->m_avPassiveRate.attack / SKILL_RATE_UNIT
						+  of->m_avPassiveAddition.attack_dam_melee
						+  awdamage * of->m_avPassiveRate.attack_dam_melee / SKILL_RATE_UNIT;
			addattack80 = of->m_avPassiveAddition.attack80;
			rateattack80 = awdamage * of->m_avPassiveRate.attack80 / SKILL_RATE_UNIT;
				if (addattack80 > 80)
					addattack80 = 80;
				if (rateattack80 > 80)
					rateattack80 = 80;
				awdamage += addattack80 + rateattack80;
			awdamage		+= of->m_assist.m_avAddition.attack
							+  awdamage * of->m_assist.m_avRate.attack / SKILL_RATE_UNIT
							+  of->m_assist.m_avAddition.attack_dam_melee
							+  awdamage * of->m_assist.m_avRate.attack_dam_melee / SKILL_RATE_UNIT;
#ifdef NEW_BALANCE
			addattack80 = of->m_assist.m_avAddition.attack80;
#else
			addattack80 = of->m_assist.m_avRate.attack80;
#endif // #ifdef NEW_BALANCE
			rateattack80 = awdamage * of->m_assist.m_avRate.attack80 / SKILL_RATE_UNIT;
			if (addattack80 > 80)
				addattack80 = 80;
			if (rateattack80 > 80)
				rateattack80 = 80;
			awdamage += addattack80 + rateattack80;
			if (nMagicPowerAddition)
				awdamage	+= nMagicPowerAddition;
			if (nMagicPowerRate)
				awdamage	+= awdamage * nMagicPowerRate / SKILL_RATE_UNIT;


#ifdef DOUBLE_ATTACK
			if( gserver.m_bDoubleAttackEvent )
			{
				DoubleAttackRate = awdamage * gserver.m_AttackPercent / SKILL_RATE_UNIT;
				if( DoubleAttackRate > 200 )
					DoubleAttackRate = 100;
				awdamage += DoubleAttackRate;
			}
#endif // DOUBLE_ATTACK

			int ddefense	= df->m_eqDefense;
			int ddefense2	= df->m_opDMelee;

#ifdef ATTACK_PET
			if( IS_APET(df) )
			{
				CAPet* apet = TO_APET(df);
				ddefense += apet->m_nDefence;
			}
#endif //ATTACK_PET

			ddefense	+= ddefense * df->m_avPassiveRate.defense / SKILL_RATE_UNIT;
			ddefense2	+= df->m_avPassiveAddition.defense
						+  ddefense2 * df->m_avPassiveRate.defense / SKILL_RATE_UNIT;
			ddefense		+= ddefense * df->m_assist.m_avRate.defense / SKILL_RATE_UNIT;
			ddefense2		+= df->m_assist.m_avAddition.defense
							+  ddefense2 * df->m_assist.m_avRate.defense / SKILL_RATE_UNIT;

#ifdef ENABLE_CHANGEJOB
			awdamage		+= of->GetMeleeLevelBonus();
			ddefense2		+= df->GetDefenseLevelBonus();
#endif

#ifdef ADULT_SERVER
			if( IS_PC(of) )
				awdamage	+= awdamage * TO_PC(of)->m_pkLevelAttack / 100;
			if( IS_PC(df) )
				ddefense2	+= ddefense2 * TO_PC(df)->m_pkLevelDefence / 100;
#endif // ADULT_SERVER

			// 무기 레벨과 유저 레벨 차이에 의한 페널티 적용
			if (adef >= 5 && adef <= 8)
				awdamage /= 2;
			else if (adef >= 9 && adef <= 12)
				awdamage = awdamage * 3 / 10;
			else if (adef >= 13)
				awdamage /= 10;

			if (ddef >= 6 && ddef <= 10)
			{
				ddefense /= 2;
				ddefense2 /= 2;
			}
			else if (ddef >= 11 && ddef <= 15)
			{
				ddefense = ddefense * 3 / 10;
				ddefense2 = ddefense2 * 3 / 10;
			}
			else if (ddef >= 16)
			{
				ddefense /= 10;
				ddefense2 /= 10;
			}

			// 공성 보너스
			awdamage = awdamage * warBonusAttack / 100;
			ddefense = ddefense * warBonusDefense / 100;
			ddefense2 = ddefense2 * warBonusDefense / 100;

			// 강신 적용 : 물리방어
			if (IS_PC(df))
			{
				CPC* pc = TO_PC(df);
				switch (pc->m_evocationType)
				{
				case EVOCATION_HELLOUND:
					ddefense = ddefense * 10 / 11;
					ddefense2 = ddefense2 * 10 / 11;
					break;
				case EVOCATION_ELENEN:
					ddefense = ddefense * 10 / 7;
					ddefense2 = ddefense2 * 10 / 7;
					break;
				}
			}

#ifdef ADULT_SERVER_NEW_BALANCE
			if (IS_PC(of))
			{
				switch (TO_PC(of)->m_job)
				{
				case JOB_TITAN:
				case JOB_KNIGHT:
					statFactor = 20;		// 2.0
				default:
					break;
				}
			}
#endif // ADULT_SERVER_NEW_BALANCE
		
#ifdef DYNAMIC_DUNGEON
			if(cidx != -1) 
			{
				if(IS_PC(of) && of->m_pZone->m_index == ZONE_DRATAN_CASTLE_DUNGEON)
				{
					awdamage += awdamage * nCharRate[cidx][0] / 100;
				}

				if(IS_PC(df) && df->m_pZone->m_index == ZONE_DRATAN_CASTLE_DUNGEON)
				{
					ddefense += ddefense * nCharRate[cidx][1] / 100;
					ddefense2 += ddefense2 * nCharRate[cidx][1] / 100;
				}				
			}
#endif // DYNAMIC_DUNGEON

			// 잠수함 : 명중패치
#if defined ADULT_SERVER_NEW_BALANCE
			if( IS_PC(of) )
			{
				float fHit = ((CCharacter*)of)->GetFinalHitRate();
				float fAvoid = ((CCharacter*)df)->GetFinalAvoidRate();
				awdamage = (int) ( awdamage * ( 1 + ( fHit / ( fHit + fAvoid ) ) ) );
				minDamage = usingstat / 3;
			}
			switch (flag)
			{
			case HITTYPE_WEAK:
				ret = (int)(usingstat *  10 / statFactor + (awdamage * 8 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_NORMAL:
				ret = (int)(usingstat *  10 / statFactor + awdamage - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_STRONG:
				ret = (int)(usingstat *  10 / statFactor + (awdamage * 13 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_HARD:
				ret = (int)(usingstat *  10 / statFactor + (awdamage * 15 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_CRITICAL:
				ret = (int)(usingstat *  10 / statFactor + (awdamage * 20 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_DEADLY:
				ret = (int)(usingstat *  10 / statFactor + (awdamage * 25 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			}
#elif defined NEW_BALANCE
			switch (flag)
			{
			case HITTYPE_WEAK:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 8 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_NORMAL:
				ret = (int)(usingstat * 10 / statFactor + awdamage - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_STRONG:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 13 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_HARD:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 15 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_CRITICAL:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 20 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_DEADLY:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 25 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			}
			minDamage = usingstat / 3;

#else // #ifdef NEW_BALANCE
			switch (flag)
			{
			case HITTYPE_WEAK:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 5 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_NORMAL:
				ret = (int)(usingstat * 10 / statFactor + awdamage - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_STRONG:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 13 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_HARD:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 16 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_CRITICAL:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 16 / 10) - ddefense2 / damFactor);
				break;
			case HITTYPE_DEADLY:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 2) - ddefense2 / damFactor);
				break;
			}
#endif
			if (GetRandom(1, 10000) <= df->m_avPassiveAddition.reduceMeleeProb)
				ret = ret - df->m_avPassiveAddition.reduceMelee - ret * df->m_avPassiveRate.reduceMelee / SKILL_RATE_UNIT;
			if (GetRandom(1, 10000) <= df->m_assist.m_avAddition.reduceMeleeProb)
				ret = ret - df->m_assist.m_avAddition.reduceMelee - ret * df->m_assist.m_avRate.reduceMelee / SKILL_RATE_UNIT;

#ifdef NEW_CASHITEM_USA_2008
			if(IS_PC(of))
			{
				CPC* pc = TO_PC(of);
				if(!pc->m_currentSkill && pc->m_assist.m_avAddition.charge_attack)
				{
					ret = ret * pc->m_assist.m_avAddition.charge_attack / SKILL_RATE_UNIT;
					pc->m_assist.CureByItemIndex(2875);
				}
			}
#endif // NEW_CASHITEM_USA_2008
		}
		break;

	case MSG_DAMAGE_RANGE:
		{
			int awdamage	= of->m_eqRange
							+ of->m_opRange;
#ifdef ATTACK_PET
			if( IS_APET(of) )
			{
				CAPet* apet = TO_APET(of);
				awdamage += apet->m_nAttack;
			}
#endif // ATTACK_PET

			awdamage	+= of->m_avPassiveAddition.attack
						+  awdamage * of->m_avPassiveRate.attack / SKILL_RATE_UNIT
						+  of->m_avPassiveAddition.attack_dam_range
						+  awdamage * of->m_avPassiveRate.attack_dam_range / SKILL_RATE_UNIT;
			addattack80 = of->m_avPassiveAddition.attack80;
			rateattack80 = awdamage * of->m_avPassiveRate.attack80 / SKILL_RATE_UNIT;
				if (addattack80 > 80)
					addattack80 = 80;
				if (rateattack80 > 80)
					rateattack80 = 80;
				awdamage += addattack80 + rateattack80;
			awdamage		+= of->m_assist.m_avAddition.attack
							+  awdamage * of->m_assist.m_avRate.attack / SKILL_RATE_UNIT
							+  of->m_assist.m_avAddition.attack_dam_range
							+  awdamage * of->m_assist.m_avRate.attack_dam_range / SKILL_RATE_UNIT;
#ifdef NEW_BALANCE
			addattack80 = of->m_assist.m_avAddition.attack80;
#else
			addattack80 = of->m_assist.m_avRate.attack80;
#endif // #ifdef NEW_BALANCE
			rateattack80 = awdamage * of->m_assist.m_avRate.attack80 / SKILL_RATE_UNIT;
			if (addattack80 > 80)
				addattack80 = 80;
			if (rateattack80 > 80)
				rateattack80 = 80;
			awdamage += addattack80 + rateattack80;
			if (nMagicPowerAddition)
				awdamage	+= nMagicPowerAddition;
			if (nMagicPowerRate)
				awdamage	+= awdamage * nMagicPowerRate / SKILL_RATE_UNIT;

#ifdef DOUBLE_ATTACK
			if( gserver.m_bDoubleAttackEvent )
			{
				DoubleAttackRate = awdamage * gserver.m_AttackPercent / SKILL_RATE_UNIT;
				if( DoubleAttackRate > 200 )
					DoubleAttackRate = 100;
				awdamage += DoubleAttackRate;
			}
#endif // DOUBLE_ATTACK

			int ddefense	= df->m_eqDefense;
			int ddefense2	= df->m_opDRange;

#ifdef ATTACK_PET
			if( IS_APET(df) )
			{
				CAPet* apet = TO_APET(df);
				ddefense += apet->m_nDefence;
			}
#endif //ATTACK_PET

			ddefense	+= ddefense * df->m_avPassiveRate.defense / SKILL_RATE_UNIT;
			ddefense2	+= df->m_avPassiveAddition.defense
						+  ddefense2 * df->m_avPassiveRate.defense / SKILL_RATE_UNIT;
			ddefense		+= ddefense * df->m_assist.m_avRate.defense / SKILL_RATE_UNIT;
			ddefense2		+= df->m_assist.m_avAddition.defense
							+  ddefense2 * df->m_assist.m_avRate.defense / SKILL_RATE_UNIT;

#ifdef ENABLE_CHANGEJOB
			awdamage		+= of->GetRangeLevelBonus();
			ddefense2		+= df->GetDefenseLevelBonus();
#endif
			
#ifdef ADULT_SERVER
			if( IS_PC(of) )
				awdamage	+= awdamage * TO_PC(of)->m_pkLevelAttack / 100;
			if( IS_PC(df) )
				ddefense2	+= ddefense2 * TO_PC(df)->m_pkLevelDefence / 100;
#endif // ADULT_SERVER

			if (adef >= 5 && adef <= 8)
				awdamage /= 2;
			else if (adef >= 9 && adef <= 12)
				awdamage = awdamage * 3 / 10;
			else if (adef >= 13)
				awdamage /= 10;

			if (ddef >= 6 && ddef <= 10)
			{
				ddefense /= 2;
				ddefense2 /= 2;
			}
			else if (ddef >= 11 && ddef <= 15)
			{
				ddefense = ddefense * 3 / 10;
				ddefense2 = ddefense2 * 3 / 10;
			}
			else if (ddef >= 16)
			{
				ddefense /= 10;
				ddefense2 /= 10;
			}

			// 공성 보너스
			awdamage = awdamage * warBonusAttack / 100;
			ddefense = ddefense * warBonusDefense / 100;
			ddefense2 = ddefense2 * warBonusDefense / 100;

			// 강신 적용 : 물리방어
			if (IS_PC(df))
			{
				CPC* pc = TO_PC(df);
				switch (pc->m_evocationType)
				{
				case EVOCATION_HELLOUND:
					ddefense = ddefense * 10 / 11;
					ddefense2 = ddefense2 * 10 / 11;
					break;
				case EVOCATION_ELENEN:
					ddefense = ddefense * 10 / 7;
					ddefense2 = ddefense2 * 10 / 7;
					break;
				}
			}

#ifdef DYNAMIC_DUNGEON
			if(cidx != -1) 
			{
				if(IS_PC(of) && of->m_pZone->m_index == ZONE_DRATAN_CASTLE_DUNGEON)
				{
					awdamage += awdamage * nCharRate[cidx][0] / 100;
				}

				if(IS_PC(df) && df->m_pZone->m_index == ZONE_DRATAN_CASTLE_DUNGEON)
				{
					ddefense += ddefense * nCharRate[cidx][1] / 100;
					ddefense2 += ddefense2 * nCharRate[cidx][1] / 100;
				}				
			}
#endif // DYNAMIC_DUNGEON

			// 잠수함 : 명중패치
#if defined ADULT_SERVER_NEW_BALANCE
			if( IS_PC(of) )
			{
				float fHit = ((CCharacter*)of)->GetFinalHitRate();
				float fAvoid = ((CCharacter*)df)->GetFinalAvoidRate();
				awdamage = (int) ( awdamage * ( 1 + ( fHit / ( fHit + fAvoid ) ) ) );
				minDamage = usingstat / 3;
			}
			switch (flag)
			{
			case HITTYPE_WEAK:
				ret = (int)(usingstat *  10 / statFactor  + (awdamage * 8 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_NORMAL:
				ret = (int)(usingstat *  10 / statFactor + awdamage - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_STRONG:
				ret = (int)(usingstat *  10 / statFactor + (awdamage * 13 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_HARD:
				ret = (int)(usingstat *  10 / statFactor + (awdamage * 15 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_CRITICAL:
				ret = (int)(usingstat *  10 / statFactor + (awdamage * 20 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_DEADLY:
				ret = (int)(usingstat *  10 / statFactor + (awdamage * 25 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			}
#elif defined NEW_BALANCE
			switch (flag)
			{
			case HITTYPE_WEAK:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 8 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_NORMAL:
				ret = (int)(usingstat * 10 / statFactor + awdamage - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_STRONG:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 13 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_HARD:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 15 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_CRITICAL:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 20 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_DEADLY:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 25 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			}
			minDamage = usingstat / 3;
#else // #ifdef NEW_BALANCE
			switch (flag)
			{
			case HITTYPE_WEAK:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 5 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_NORMAL:
				ret = (int)(usingstat * 10 / statFactor + awdamage - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_STRONG:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 13 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_HARD:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 16 / 10) - (ddefense + ddefense2) / damFactor);
				break;
			case HITTYPE_CRITICAL:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 16 / 10) - ddefense2 / damFactor);
				break;
			case HITTYPE_DEADLY:
				ret = (int)(usingstat * 10 / statFactor + (awdamage * 2) - ddefense2 / damFactor);
				break;
			}
#endif // #ifdef NEW_BALANCE

			if (GetRandom(1, 10000) <= df->m_avPassiveAddition.reduceRangeProb)
				ret = ret - df->m_avPassiveAddition.reduceRange - ret * df->m_avPassiveRate.reduceRange / SKILL_RATE_UNIT;
			if (GetRandom(1, 10000) <= df->m_assist.m_avAddition.reduceRangeProb)
				ret = ret - df->m_assist.m_avAddition.reduceRange - ret * df->m_assist.m_avRate.reduceRange / SKILL_RATE_UNIT;

#ifdef NEW_CASHITEM_USA_2008
			if(IS_PC(of))
			{
				CPC* pc = TO_PC(of);
				if(!pc->m_currentSkill && pc->m_assist.m_avAddition.charge_attack)
				{
					ret = ret * pc->m_assist.m_avAddition.charge_attack / SKILL_RATE_UNIT;
					pc->m_assist.CureByItemIndex(2875);
				}
			}
#endif // NEW_CASHITEM_USA_2008
		}
		break;

	case MSG_DAMAGE_MAGIC:
		{
			int aint		= of->m_int;
			int awdamage	= of->m_eqMagic
							+ of->m_opMagic;
#ifdef ATTACK_PET
			if( IS_APET(of) )
			{
				CAPet* apet = TO_APET(of);
				awdamage += apet->m_nMagicAttack;
			}
#endif // ATTACK_PET

#ifndef ADULT_SERVER_NEW_BALANCE
			int ahitrate	= of->m_opMagicHitRate;
#endif // ADULT_SERVER_NEW_BALANCE

			awdamage	+= of->m_avPassiveAddition.magic
						+  awdamage * of->m_avPassiveRate.magic / SKILL_RATE_UNIT;
			awdamage		+= of->m_assist.m_avAddition.magic
							+  awdamage * of->m_assist.m_avRate.magic / SKILL_RATE_UNIT;
			if (nMagicPowerAddition)
				awdamage	+= nMagicPowerAddition;
			if (nMagicPowerRate)
				awdamage	+= awdamage * nMagicPowerRate / SKILL_RATE_UNIT;

#ifdef DOUBLE_ATTACK
			if( gserver.m_bDoubleAttackEvent )
			{
				DoubleAttackRate = awdamage * gserver.m_AttackPercent / SKILL_RATE_UNIT;
				if( DoubleAttackRate > 200 )
					DoubleAttackRate = 100;
				awdamage += DoubleAttackRate;
			}
#endif // DOUBLE_ATTACK

			int dresist		= df->m_eqResist;
			int dresist2	= df->m_opResist;
#ifndef ADULT_SERVER_NEW_BALANCE
			int davoid		= df->m_opResistAvoid;
#endif // ADULT_SERVER_NEW_BALANCE

#ifdef ATTACK_PET
			if( IS_APET(df) )
			{
				CAPet* apet = TO_APET(df);
				dresist += apet->m_nMagicDefence;
			}
#endif //ATTACK_PET
			dresist		+= dresist * df->m_avPassiveRate.resist / SKILL_RATE_UNIT;
			dresist2	+= df->m_avPassiveAddition.resist
						+  dresist2 * df->m_avPassiveRate.resist / SKILL_RATE_UNIT;
			dresist			+= dresist * df->m_assist.m_avRate.resist / SKILL_RATE_UNIT;
			dresist2		+= df->m_assist.m_avAddition.resist
							+  dresist2 * df->m_assist.m_avRate.resist / SKILL_RATE_UNIT;

#ifdef ENABLE_CHANGEJOB
			awdamage		+= of->GetMagicLevelBonus();
			dresist2		+= df->GetResistLevelBonus();
#endif

#ifdef ADULT_SERVER
			if( IS_PC(of) )
				awdamage	+= awdamage * TO_PC(of)->m_pkLevelAttack / 100;
			if( IS_PC(df) )
				dresist2	+= dresist2 * TO_PC(df)->m_pkLevelDefence / 100;
#endif // ADULT_SERVER

			if (adef >= 5 && adef <= 8)
				awdamage /= 2;
			else if (adef >= 9 && adef <= 12)
				awdamage = awdamage * 3 / 10;
			else if (adef >= 13)
				awdamage /= 10;

			if (ddef >= 6 && ddef <= 10)
			{
				dresist /= 2;
				dresist2 /= 2;
			}
			else if (ddef >= 11 && ddef <= 15)
			{
				dresist = dresist * 3 / 10;
				dresist2 = dresist2 * 3 / 10;
			}
			else if (ddef >= 16)
			{
				dresist /= 10;
				dresist2 /= 10;
			}

			// 공성 보너스
			awdamage = awdamage * warBonusAttack / 100;
			dresist = dresist * warBonusDefense / 100;
			dresist2 = dresist2 * warBonusDefense / 100;

			// 강신 적용 : 마공
			if (IS_PC(of))
			{
				CPC* pc = TO_PC(of);
				switch (pc->m_evocationType)
				{
				case EVOCATION_HELLOUND:
					awdamage = awdamage;
					break;

				case EVOCATION_ELENEN:
					awdamage = awdamage * 10 / 13;
					break;
				}
			}

			// 강신 적용 : 마법방어
			if (IS_PC(df))
			{
				CPC* pc = TO_PC(df);
				switch (pc->m_evocationType)
				{
				case EVOCATION_HELLOUND:
					dresist = dresist;
					dresist2 = dresist2;
					break;
				case EVOCATION_ELENEN:
					dresist = dresist * 13 / 10;
					dresist2 = dresist2 * 13 / 10;
					break;
				}
			}

#ifdef ADULT_SERVER_NEW_BALANCE
			if (IS_PC(of))
			{
				switch (TO_PC(of)->m_job)
				{
				case JOB_HEALER:
				case JOB_SORCERER:
					statFactor = 20;		// 2.0
				default:
					break;
				}
			}
#endif // ADULT_SERVER_NEW_BALANCE

#ifdef DYNAMIC_DUNGEON
			if(cidx != -1) 
			{
				if(IS_PC(of) && of->m_pZone->m_index == ZONE_DRATAN_CASTLE_DUNGEON)
				{
					awdamage += awdamage * nCharRate[cidx][0] / 100;
				}

				if(IS_PC(df) && df->m_pZone->m_index == ZONE_DRATAN_CASTLE_DUNGEON)
				{
					dresist += dresist * nCharRate[cidx][1] / 100;
					dresist2 += dresist2 * nCharRate[cidx][1] / 100;
				}
				
			}
#endif // DYNAMIC_DUNGEON

#if defined ADULT_SERVER_NEW_BALANCE
			if( IS_PC(of) )
			{
				float fHit = ((CCharacter*)of)->GetFinalHitRate();
				float fAvoid = ((CCharacter*)df)->GetFinalAvoidRate();
				awdamage = (int) ( awdamage * ( 1 + ( fHit / ( fHit + fAvoid ) ) ) );
				minDamage = aint / 3;
			}
			switch (flag)
			{
			case HITTYPE_WEAK:
				ret = aint * 10 / 15 + ((awdamage * 8 / 10) - (dresist + dresist2)) ;//* (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_NORMAL:
				ret = aint * 10 / 15 + (awdamage - (dresist + dresist2)) ;//* (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_STRONG:
				ret = aint * 10 / 15 + ((awdamage * 13 / 10) - (dresist + dresist2));// * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_HARD:
				ret = aint * 10 / 15 + ((awdamage * 15 / 10) - (dresist + dresist2));// * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_CRITICAL:
				ret = aint * 10 / 15 + ((awdamage * 20 / 10) - (dresist + dresist2));// * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_DEADLY:
				ret = aint * 10 / 15 + ((awdamage * 25 / 10) - (dresist + dresist2));// * (100 + ahitrate - davoid) / 100;
				break;
			}
#elif defined NEW_BALANCE
			switch (flag)
			{
			case HITTYPE_WEAK:
				ret = aint * 10 / 15 + ((awdamage * 8 / 10) - (dresist + dresist2)) * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_NORMAL:
				ret = aint * 10 / 15 + (awdamage - (dresist + dresist2)) * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_STRONG:
				ret = aint * 10 / 15 + ((awdamage * 13 / 10) - (dresist + dresist2)) * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_HARD:
				ret = aint * 10 / 15 + ((awdamage * 15 / 10) - (dresist + dresist2)) * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_CRITICAL:
				ret = aint * 10 / 15 + ((awdamage * 20 / 10) - (dresist + dresist2)) * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_DEADLY:
				ret = aint * 10 / 15 + ((awdamage * 25 / 10) - (dresist + dresist2)) * (100 + ahitrate - davoid) / 100;
				break;
			}
			minDamage = aint / 3;
#else // #ifdef NEW_BALANCE
			switch (flag)
			{
			case HITTYPE_WEAK:
				ret = aint * 10 / 15 + ((awdamage * 5 / 10) - (dresist + dresist2)) * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_NORMAL:
				ret = aint * 10 / 15 + (awdamage - (dresist + dresist2)) * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_STRONG:
				ret = aint * 10 / 15 + ((awdamage * 13 / 10) - (dresist + dresist2)) * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_HARD:
				ret = aint * 10 / 15 + ((awdamage * 16 / 10) - (dresist + dresist2)) * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_CRITICAL:
				ret = aint * 10 / 15 + ((awdamage * 16 / 10) - dresist2) * (100 + ahitrate - davoid) / 100;
				break;
			case HITTYPE_DEADLY:
				ret = aint * 10 / 15 + ((awdamage * 2) - dresist2) * (100 + ahitrate - davoid) / 100;
				break;
			}
#endif // #ifdef NEW_BALANCE

			if (GetRandom(1, 10000) <= df->m_avPassiveAddition.reduceMagicProb)
				ret = ret - df->m_avPassiveAddition.reduceMagic - ret * df->m_avPassiveRate.reduceMagic / SKILL_RATE_UNIT;
			if (GetRandom(1, 10000) <= df->m_assist.m_avAddition.reduceMagicProb)
				ret = ret - df->m_assist.m_avAddition.reduceMagic - ret * df->m_assist.m_avRate.reduceMagic / SKILL_RATE_UNIT;

#ifdef NEW_CASHITEM_USA_2008
			if(IS_PC(of))
			{
				CPC* pc = TO_PC(of);
				if(!pc->m_currentSkill && pc->m_assist.m_avAddition.charge_magic)
				{
					ret = ret * pc->m_assist.m_avAddition.charge_magic / SKILL_RATE_UNIT;
					pc->m_assist.CureByItemIndex(2875);
				}
			}
#endif // NEW_CASHITEM_USA_2008
		}
		break;

	default:
		return 0;
	}

	bool bIsPCof = (IS_PC(of) || IS_PET(of) || IS_ELEMENTAL(of) || IS_APET(of) );
	bool bIsPCdf = (IS_PC(df) || IS_PET(df) || IS_ELEMENTAL(df) || IS_APET(df) );

#if defined( ADULT_SERVER_NEW_BALANCE )
	if (magic == NULL && bIsPCof && bIsPCdf)
		ret = ret + ret * ((of->m_level - df->m_level) / (of->m_level + df->m_level));
#else
	if (magic == NULL && bIsPCof && bIsPCdf)
		ret = ret * ((of->m_level - df->m_level) * 4 + 100) / 100;
#endif

#if defined (LC_JPN) || defined (LC_KOR)
	if (IS_PC(of) && IS_PC(df))
	{
		ret = ret / 3;
	}
#endif

#ifdef EXTREME_CUBE
	if( IS_PC(of) && IS_NPC(df) )
	{
		CPC* pc = TO_PC(of);
		CNPC* npc = TO_NPC(df);

		if(npc->m_proto->m_jobAttr & (1 << pc->m_job))
		{
			ret /= 2;
		}
	}
#endif // EXTREME_CUBE

#ifdef ENABLE_WAR_CASTLE
	if (of->m_assist.m_avAddition.hcAttackTower && IS_PC(of) && IS_NPC(df))
	{
		CNPC* npc = TO_NPC(df);
		if (npc->m_proto->CheckFlag(NPC_CASTLE_TOWER))
		{
			ret *= 2;
		}
	}
#endif

#if defined (EVENT_WHITEDAY_2007) || defined (EVENT_OPEN_ADULT_SERVER)	|| defined (EVENT_MAGIC_CARD)	// 2%의 확률로 10배의 데미지
	if (of->m_assist.m_avAddition.bRorainOfLoveMagic && IS_PC(of) && IS_NPC(df))
	{
		int rate = GetRandom(0,1000);
		if( rate < 20 )
		{
			ret *= 10;
		}
	}
#endif // // defined (EVENT_WHITEDAY_2007) || defined (EVENT_OPEN_ADULT_SERVER) || defined (EVENT_MAGIC_CARD)

#ifdef NEW_CASHITEM_USA_2008
	if (of->m_assist.m_avAddition.hcAttackBerserker && IS_PC(of) && IS_NPC(df))	// 10% 확률로 2배의 데미지
	{
		int rate = GetRandom(1,1000);
		if( rate <= 100 )
		{
			ret *= 2;
		}
	}
#endif // NEW_CASHITEM_USA_2008
	
#ifdef NEW_BALANCE
	// 스킬 사용시
	if (IS_PC(of) && IS_PC(df) && magic != NULL)
	{
		// 계산된 데미지가 데들리보다 클 경우
		if (ret > deadlyDamage)
		{
			// 초과분은 일부만 적용한다
			ret = deadlyDamage + (ret - deadlyDamage) / nPvPSkillFactor;
		}
	}
#endif // #ifdef NEW_BALANCE

#ifndef _DEBUG
	ret += GetRandom(-7, +7);
#endif // #ifndef _DEBUG

#ifdef NEW_BALANCE

#if defined( ADULT_SERVER_NEW_BALANCE ) 
	if (ret <= minDamage)
		ret = minDamage;
#else
	// 최소 대미지 +-10%
	if (ret <= minDamage)
		ret = minDamage - (minDamage - ret) / 5;
#endif

#endif // #ifdef NEW_BALANCE

	if (IS_PC(df) && magic == NULL)
	{
		CPC* pPCDefense = TO_PC(df);
		if (pPCDefense->m_opBlocking > 0)
		{
			if (GetRandom(1, 100) <= pPCDefense->m_opBlocking)
			{
				ret = ret / 2;
			}
		}
	}

	if (ret < 1)
		ret = 1;

#ifdef ADULT_SERVER
	if( IS_PC(df) )
	{
		CPC* pc = TO_PC(df);
		if( pc->IsSetPlayerState( PLAYER_STATE_OTHERZONE ) )		// 속성이 다른 마을에 있으면 방어력 50% 감소
		{
			ret += ret * 50 / 100;
		}
	}
#else // ADULT_SERVER

#ifdef ATTACK_PET
	if( IS_APET(df) )		// 펫이 반항상태이면 방어도 50 하락
	{
		CAPet* apet = TO_APET(df);
		if( apet->GetFaithLevel() > 0 )
			ret += ret * 50 / 100;
	}
	else if ( IS_APET(of) )		// 펫이 반항상태이면 공격력 50 감소
	{
		CAPet* apet = TO_APET(of);
		if( apet->GetFaithLevel() > 0 )
			ret -= ret * 50 / 100;
	}
#endif //ATTACK_PET

	// PK 페널티 : 공격력 하락
#ifndef DISABLE_PKPENALTY

#ifdef FREE_PK_SYSTEM
		if( !gserver.m_bFreePk )
		{
#endif // FREE_PK_SYSTEM

#ifdef MAL_DISABLE_PKPENALTY
	if ( !gserver.m_bDisablePKPaenalty )
	{
#endif // MAL_DISABLE_PKPENALTY
	if (IS_PC(of))
	{
		CPC* pc = TO_PC(of);

		if (pc->m_pkPenalty <= -150)				ret -= ret * 60 / 100;
		else if (pc->m_pkPenalty <= -140)			ret -= ret * 56 / 100;
		else if (pc->m_pkPenalty <= -130)			ret -= ret * 52 / 100;
		else if (pc->m_pkPenalty <= -120)			ret -= ret * 48 / 100;
		else if (pc->m_pkPenalty <= -110)			ret -= ret * 44 / 100;
		else if (pc->m_pkPenalty <= -100)			ret -= ret * 40 / 100;
		else if (pc->m_pkPenalty <=  -90)			ret -= ret * 36 / 100;
		else if (pc->m_pkPenalty <=  -80)			ret -= ret * 32 / 100;
		else if (pc->m_pkPenalty <=  -70)			ret -= ret * 28 / 100;
		else if (pc->m_pkPenalty <=  -60)			ret -= ret * 24 / 100;
		else if (pc->m_pkPenalty <=  -50)			ret -= ret * 20 / 100;
		else if (pc->m_pkPenalty <=  -40)			ret -= ret * 16 / 100;
		else if (pc->m_pkPenalty <=  -30)			ret -= ret * 12 / 100;
		else if (pc->m_pkPenalty <=  -20)			ret -= ret *  8 / 100;
		else if (pc->m_pkPenalty <=  -10)			ret -= ret *  4 / 100;
	}

	if (ret < 1)
		ret = 1;

	// PK 페널티 : 방어력 하락
	if (IS_PC(df))
	{
		CPC* pc = TO_PC(df);

		if (pc->m_pkPenalty <= -150)				ret += ret * 45 / 100;
		else if (pc->m_pkPenalty <= -140)			ret += ret * 40 / 100;
		else if (pc->m_pkPenalty <= -130)			ret += ret * 35 / 100;
		else if (pc->m_pkPenalty <= -120)			ret += ret * 30 / 100;
		else if (pc->m_pkPenalty <= -110)			ret += ret * 25 / 100;
		else if (pc->m_pkPenalty <= -100)			ret += ret * 20 / 100;
		else if (pc->m_pkPenalty <=  -90)			ret += ret * 15 / 100;
		else if (pc->m_pkPenalty <=  -80)			ret += ret * 10 / 100;
		else if (pc->m_pkPenalty <=  -70)			ret += ret *  5 / 100;
	}
#ifdef MAL_DISABLE_PKPENALTY
	}
#endif // MAL_DISABLE_PKPENALTY

#ifdef FREE_PK_SYSTEM
		}
#endif // FREE_PK_SYSTEM

#endif // DISABLE_PKPENALTY

#endif // ADULT_SERVER

		
	if (ret < 1)
		ret = 1;

	return ret;
}

#ifdef ADULT_SERVER_NEW_BALANCE
char GetHitType_PvP(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type)
{
	extern int nBaseHitrate[JOBCOUNT][3];
	extern int nBaseAvoid[JOBCOUNT][3];
	extern int levelup_stat[JOBCOUNT][4];
	extern int levelup_stat2[JOB2COUNT][4];

	const float nHitratePerDex = 1.0f;
	const float nHitratePerInt = 1.0f;
	const float nAvoidPerDex = 0.125f;
	const float nAvoidPerInt = 0.125f;

	if (!IS_PC(of) || !IS_PC(df))
		return HITTYPE_MISS;

	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
	case MSG_DAMAGE_MAGIC:
		break;
	default:
		return HITTYPE_MISS;
	}

	CPC* opc = TO_PC(of);
	CPC* dpc = TO_PC(df);
	
#ifdef TEST_SERVER
	if( opc->m_pos.m_yLayer - dpc->m_pos.m_yLayer >=2 || opc->m_pos.m_yLayer - dpc->m_pos.m_yLayer <= -2 )
		return HITTYPE_MISS;
#endif //TEST_SERVER

	float fHitrate	= nBaseHitrate[opc->m_job][type] + (opc->m_level - dpc->m_level);
	float fAvoid	= nBaseAvoid[dpc->m_job][type] + (dpc->m_level - opc->m_level);

	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
		if (opc->m_job2 == 0)
			fHitrate += nHitratePerDex * (levelup_stat[opc->m_job][1] * (opc->m_level - 1));
		else
			fHitrate += nHitratePerDex * (levelup_stat2[opc->m_job * 2 + opc->m_job2][1] * (opc->m_level - JOB_2ND_LEVEL));
		if (dpc->m_job2 == 0)
			fAvoid += nAvoidPerDex * (levelup_stat[dpc->m_job][1] * (dpc->m_level - 1));
		else
			fAvoid += nAvoidPerDex * (levelup_stat2[dpc->m_job * 2 + dpc->m_job2][1] * (dpc->m_level - JOB_2ND_LEVEL));
		break;
	case MSG_DAMAGE_MAGIC:
		if (opc->m_job2 == 0)
			fHitrate += nHitratePerInt * (levelup_stat[opc->m_job][2] * (opc->m_level - 1));
		else
			fHitrate += nHitratePerInt * (levelup_stat2[opc->m_job * 2 + opc->m_job2][2] * (opc->m_level - JOB_2ND_LEVEL));
		if (dpc->m_job2 == 0)
			fAvoid += nAvoidPerInt * (levelup_stat[dpc->m_job][2] * (dpc->m_level - 1));
		else
			fAvoid += nAvoidPerInt * (levelup_stat2[dpc->m_job * 2 + dpc->m_job2][2] * (dpc->m_level - JOB_2ND_LEVEL));
		break;
	default:
		break;
	}

#ifdef BSTEST // TODO
#pragma message(__FILE__"(1345) : warning: TODO : SKILL")
	/*
	fHitrate += option + (skilladd / 10000.0f) + (fHitrate * skillrate / SKILL_RATE_UNIT);
	fAvoid += option + (skilladd / 10000.0f) + (fAvoid * skillrate / SKILL_RATE_UNIT);
	*/
#endif

	int nHitrate = (int)(fHitrate * 1000);
	int nAvoid = (int)(fAvoid * 1000);

	nHitrate = GetRandom(1, nHitrate);
	nAvoid = GetRandom(1, nAvoid);

	if (nHitrate > nAvoid)
		return HITTYPE_WEAK | HITTYPE_NORMAL | HITTYPE_STRONG | HITTYPE_HARD | HITTYPE_CRITICAL | HITTYPE_DEADLY;
	else
		return HITTYPE_MISS;
}

char GetHitType_Adult(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type)
{
	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
	case MSG_DAMAGE_MAGIC:
		break;
	default:
		return HITTYPE_MISS;
	}

	float fHitrate	= of->GetHitrate(df, type);
	float fAvoid	= df->GetAvoid(of, type);

#ifdef TEST_SERVER
	if( of->m_pos.m_yLayer - df->m_pos.m_yLayer >=2 || of->m_pos.m_yLayer - df->m_pos.m_yLayer <= -2 )
		return HITTYPE_MISS;
#endif //TEST_SERVER

	switch (type)
	{
	case MSG_DAMAGE_MELEE:
		fHitrate += of->m_opMeleeHitRate + of->m_avPassiveAddition.HitRate_2033;
		fHitrate += of->m_avPassiveAddition.hitrate + (fHitrate * of->m_avPassiveRate.hitrate / SKILL_RATE_UNIT);
		fHitrate += of->m_assist.m_avAddition.hitrate + (fHitrate * of->m_assist.m_avRate.hitrate / SKILL_RATE_UNIT);
		fAvoid += df->m_opMeleeAvoid + df->m_avPassiveAddition.Avoid_2034;
		fAvoid += df->m_avPassiveAddition.avoid + (fAvoid * df->m_avPassiveRate.avoid / SKILL_RATE_UNIT);
		fAvoid += df->m_assist.m_avAddition.avoid + (fAvoid * df->m_assist.m_avRate.avoid / SKILL_RATE_UNIT);
		break;
	case MSG_DAMAGE_RANGE:
		fHitrate += of->m_opRangeHitRate + of->m_avPassiveAddition.HitRate_2033;
		fHitrate += of->m_avPassiveAddition.hitrate + (fHitrate * of->m_avPassiveRate.hitrate / SKILL_RATE_UNIT);
		fHitrate += of->m_assist.m_avAddition.hitrate + (fHitrate * of->m_assist.m_avRate.hitrate / SKILL_RATE_UNIT);
		fAvoid += df->m_opRangeAvoid + df->m_avPassiveAddition.Avoid_2034;
		fAvoid += df->m_avPassiveAddition.avoid + (fAvoid * df->m_avPassiveRate.avoid / SKILL_RATE_UNIT);
		fAvoid += df->m_assist.m_avAddition.avoid + (fAvoid * df->m_assist.m_avRate.avoid / SKILL_RATE_UNIT);
		break;
	case MSG_DAMAGE_MAGIC:
		fHitrate += of->m_opMagicHitRate + of->m_avPassiveAddition.HitRate_2033;
		fHitrate += of->m_avPassiveAddition.magichitrate + (fHitrate * of->m_avPassiveRate.magichitrate / SKILL_RATE_UNIT);
		fHitrate += of->m_assist.m_avAddition.magichitrate + (fHitrate * of->m_assist.m_avRate.magichitrate / SKILL_RATE_UNIT);
		fAvoid += df->m_opResistAvoid + df->m_avPassiveAddition.Avoid_2034;
		fAvoid += df->m_avPassiveAddition.magicavoid + (fAvoid * df->m_avPassiveRate.magicavoid / SKILL_RATE_UNIT);
		fAvoid += df->m_assist.m_avAddition.magicavoid + (fAvoid * df->m_assist.m_avRate.magicavoid / SKILL_RATE_UNIT);
		break;
	default:
		break;
	}
	
#if defined(ADULT_SERVER_NEW_BALANCE) && defined(ENABLE_CHANGEJOB)
	if(of->m_type == MSG_CHAR_NPC
		&& df->m_type == MSG_CHAR_PC)
	{	// 31레벨 이상 전직을 한 PC가 NPC에 대한 회피도 증가
		CPC * pPC = TO_PC(df);
		
		if(pPC->m_level >= 31 && pPC->m_job2 != 0)
		{
			int factA = 0;	// 직업에 의한 변수
			int factB = 0;	// 회피에 의한 변수 
			int factC = 0;	// 레벨에 의한 변수
			int factL = pPC->m_level;	// 레벨

			switch (pPC->m_job)
			{
			case JOB_TITAN:
				switch(pPC->m_job2)
				{
					case JOB_2ND_HIGHLANDER:	factA = 0;		break;
					case JOB_2ND_WARMASTER:		factA = 0;		break;
				}
				break;

			case JOB_KNIGHT:
				switch(pPC->m_job2)
				{
					case JOB_2ND_ROYALKNIGHT:	factA = 6;		break;
					case JOB_2ND_TEMPLEKNIGHT:	factA = 6;		break;
				}
				break;

			case JOB_HEALER:
				switch(pPC->m_job2)
				{
					case JOB_2ND_ARCHER:		factA = 12;		break;
					case JOB_2ND_CLERIC:		factA = 8;		break;
				}
				break;

			case JOB_MAGE:
				switch(pPC->m_job2)
				{
					case JOB_2ND_WIZARD:		factA = 10;		break;
					case JOB_2ND_WITCH:			factA = 8;		break;
				}
				break;

			case JOB_ROGUE:
				switch (pPC->m_job2)
				{
					case JOB_2ND_ASSASSIN:		factA = 14;		break;
					case JOB_2ND_RANGER:		factA = 12;		break;
				}
				break;

			case JOB_SORCERER:
				switch (pPC->m_job2)
				{
					case JOB_2ND_ELEMENTALLIST:	factA = 4;		break;
					case JOB_2ND_SPECIALLIST:	factB = 7;		break;
				}
				break;
#ifdef NIGHT_SHADOW
			case JOB_NIGHTSHADOW:
				switch (pPC->m_job2)
				{
					case JOB_2ND_NIGHTSHADOW:	factA = 10;		break;
				}
				break;
#endif //NIGHT_SHADOW
			}	// end switch

			int factTemp = (1 + factA) + (factL - 31);
			if(factTemp > 44)
			{	
				factB = 300;
			}
			else if(factTemp > 30)
			{	
				factB = 200;
			}
			else 
			{	
				factB = 100;
			}			

			factC = (factL - 31);

			int tempfact;
			tempfact = 99 - factA - factC;
			if(tempfact == 0)
				tempfact = 1;

			int factAdd = (1 + factA + factC) * (factB / tempfact);
			if(factAdd > 1200 || factAdd <= 0)
			{
				factAdd = 1200;
			}
			fAvoid += factAdd;
		}	// end if
	}
#endif // ADULT_SERVER_NEW_BALANCE

#ifdef ADULT_SERVER_NEW_BALANCE
	of->SetFinalHitRate( fHitrate );
	df->SetFinalAvoidRate( fAvoid );
#endif //ADULT_SERVER_NEW_BALANCE

	int nHitrate = (int)(fHitrate * 1000);
	int nAvoid = (int)(fAvoid * 1000);

	nHitrate = GetRandom(1, nHitrate);
	nAvoid = GetRandom(1, nAvoid);

	if (nHitrate > nAvoid)
		return HITTYPE_WEAK | HITTYPE_NORMAL | HITTYPE_STRONG | HITTYPE_HARD | HITTYPE_CRITICAL | HITTYPE_DEADLY;
	else
		return HITTYPE_MISS;
}

char SelectHitType_PvP(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel, bool nothelp)
{
	extern int nBaseHitTypeProb[2][6];
	extern int base_stat[JOBCOUNT][4];
#ifdef BSTEST
#pragma message(__FILE__"(1435) : warning: TODO : SKILL")
#endif //BSTEST
	if (magic)
		return HITTYPE_NORMAL;

	CPC* opc = TO_PC(of);
	CPC* dpc = TO_PC(df);
	int nTypeIndex = 0;
	int nOffenseStatBase = 0;
	int nOffenseDexBase = 0;
	int nDefenseDur = 0;
	int nTypeProb[6] = {0,};
	int nType[6] = {HITTYPE_WEAK, HITTYPE_NORMAL, HITTYPE_STRONG, HITTYPE_HARD, HITTYPE_CRITICAL, HITTYPE_DEADLY};
	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
		nTypeIndex = 0;
		nDefenseDur = dpc->m_str / 3;
		nOffenseStatBase = base_stat[opc->m_job][0];
		nOffenseDexBase  = base_stat[opc->m_job][1];
		memcpy(nTypeProb, nBaseHitTypeProb[nTypeIndex], sizeof(int) * 6);
		nTypeProb[0] += (opc->m_level - 1) / 2 + nDefenseDur;
		nTypeProb[1] += (opc->m_level - 1) + nDefenseDur;
		nTypeProb[2] += (opc->m_str - nOffenseStatBase) * 5 / 4;
		nTypeProb[3] += (opc->m_str - nOffenseStatBase);
		nTypeProb[4] += (opc->m_dex - nOffenseDexBase) * 9 / 10;
		nTypeProb[5] += (opc->m_dex - nOffenseDexBase) * 6 / 10;
		break;

	case MSG_DAMAGE_MAGIC:
		nTypeIndex = 1;
		nDefenseDur = dpc->m_str / 3;
		nOffenseStatBase = base_stat[opc->m_job][2];
		nOffenseDexBase  = base_stat[opc->m_job][1];
		memcpy(nTypeProb, nBaseHitTypeProb[nTypeIndex], sizeof(int) * 6);
		nTypeProb[0] += (opc->m_level - 1) / 2 + nDefenseDur;
		nTypeProb[1] += (opc->m_level - 1) + nDefenseDur;
		nTypeProb[2] += (opc->m_int - nOffenseStatBase) * 5 / 4;
		nTypeProb[3] += (opc->m_str - nOffenseStatBase);
		nTypeProb[4] += (opc->m_dex - nOffenseDexBase) * 9 / 10;
		nTypeProb[5] += (opc->m_dex - nOffenseDexBase) * 6 / 10;
		break;

	default:
		return HITTYPE_NORMAL;
	}

	nTypeProb[4] += nTypeProb[4] * opc->m_opAttackCritical / 100;

	nTypeProb[4] += opc->m_avPassiveAddition.critical + (nTypeProb[4] * opc->m_avPassiveRate.critical);
	nTypeProb[4] += opc->m_assist.m_avAddition.critical + (nTypeProb[4] * opc->m_assist.m_avRate.critical);
	nTypeProb[5] += opc->m_avPassiveAddition.deadly + (nTypeProb[5] * opc->m_avPassiveRate.deadly);
	nTypeProb[5] += opc->m_assist.m_avAddition.deadly + (nTypeProb[5] * opc->m_assist.m_avRate.deadly);

	int i;
	int nMax = 0;
	for (i = 0; i < 6; i++)
	{
		nTypeProb[i] = GetRandom(1, nTypeProb[i]);
		if (nTypeProb[i] >= nTypeProb[nMax])
			nMax = i;
	}
	return nType[nMax];
}

char SelectHitType_Adult(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel, bool nothelp)
{
	extern int nBaseHitTypeProb[2][6];
	extern int base_stat[JOBCOUNT][4];
	const int nBaseHitTypeProbForNPC[2][6] = {
		{50, 575, 125, 100, 90, 60},
		{50, 575, 125, 100, 90, 60}
	};
	const int nOffenseStrBaseForNPC = 38;
	const int nOffenseDexBaseForNPC = 28;
	const int nOffenseIntBaseForNPC = 28;
#ifdef BSTEST
#pragma message(__FILE__"(1513) : warning: TODO : SKILL")
#endif //BSTEST
	if (magic)
		return HITTYPE_NORMAL;

	int nTableIndex = 0;
	int nOffenseStatBase = 0;
	int nOffenseDexBase = 0;
	int nDefenseDur = 0;
	int nTypeProb[6] = {0,};
	int nType[6] = {HITTYPE_WEAK, HITTYPE_NORMAL, HITTYPE_STRONG, HITTYPE_HARD, HITTYPE_CRITICAL, HITTYPE_DEADLY};
	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
		nTableIndex = 0;
		nDefenseDur = df->m_con / 3 + df->m_avPassiveAddition.stronger_skill;
		switch (of->m_type)
		{
		case MSG_CHAR_PC:
			nOffenseStatBase = base_stat[TO_PC(of)->m_job][0];
			nOffenseDexBase  = base_stat[TO_PC(of)->m_job][1];
			memcpy(nTypeProb, nBaseHitTypeProb[nTableIndex], sizeof(int) * 6);
			break;
		case MSG_CHAR_NPC:
			nOffenseStatBase = nOffenseStrBaseForNPC;
			nOffenseDexBase  = nOffenseDexBaseForNPC;
			memcpy(nTypeProb, nBaseHitTypeProbForNPC[nTableIndex], sizeof(int) * 6);
			break;
		case MSG_CHAR_ELEMENTAL:
			{
				CPC* pc = TO_ELEMENTAL(of)->GetOwner();
				if (pc)
				{
					nOffenseStatBase = base_stat[pc->m_job][0];
					nOffenseDexBase  = base_stat[pc->m_job][1];
				}
				memcpy(nTypeProb, nBaseHitTypeProb[nTableIndex], sizeof(int) * 6);
			}
			break;
		case MSG_CHAR_PET:
		default:
			break;
		}

		nTypeProb[0] += (of->m_level - 1) / 2 + nDefenseDur;
		nTypeProb[1] += (of->m_level - 1) + nDefenseDur;
		nTypeProb[2] += (of->m_str - nOffenseStatBase) * 5 / 4;
		nTypeProb[3] += (of->m_str - nOffenseStatBase);
		nTypeProb[4] += (of->m_dex - nOffenseDexBase) * 9 / 10;
		nTypeProb[5] += (of->m_dex - nOffenseDexBase) * 6 / 10;
		break;

	case MSG_DAMAGE_MAGIC:
		nTableIndex = 1;
		nDefenseDur = df->m_con / 3 + df->m_avPassiveAddition.stronger_skill;
		switch (of->m_type)
		{
		case MSG_CHAR_PC:
			nOffenseStatBase = base_stat[TO_PC(of)->m_job][2];
			nOffenseDexBase  = base_stat[TO_PC(of)->m_job][1];
			memcpy(nTypeProb, nBaseHitTypeProb[nTableIndex], sizeof(int) * 6);
			break;
		case MSG_CHAR_NPC:
			nOffenseStatBase = nOffenseIntBaseForNPC;
			nOffenseDexBase  = nOffenseDexBaseForNPC;
			memcpy(nTypeProb, nBaseHitTypeProbForNPC[nTableIndex], sizeof(int) * 6);
			break;
		case MSG_CHAR_ELEMENTAL:
			{
				CPC* pc = TO_ELEMENTAL(of)->GetOwner();
				if (pc)
				{
					nOffenseStatBase = base_stat[pc->m_job][0];
					nOffenseDexBase  = base_stat[pc->m_job][1];
				}
				memcpy(nTypeProb, nBaseHitTypeProb[nTableIndex], sizeof(int) * 6);
			}
			break;
		case MSG_CHAR_PET:
		default:
			break;
		}
		nTypeProb[0] += (of->m_level - 1) / 2 + nDefenseDur;
		nTypeProb[1] += (of->m_level - 1) + nDefenseDur;
		nTypeProb[2] += (of->m_int - nOffenseStatBase) * 5 / 4;
		nTypeProb[3] += (of->m_str - nOffenseStatBase);
		nTypeProb[4] += (of->m_dex - nOffenseDexBase) * 9 / 10;
		nTypeProb[5] += (of->m_dex - nOffenseDexBase) * 6 / 10;
		break;

	default:
		return HITTYPE_NORMAL;
	}

	if (IS_PC(of))
		nTypeProb[4] += nTypeProb[4] * TO_PC(of)->m_opAttackCritical / 100;

#ifdef ATTACK_PET
	else if( IS_APET(of) )
	{
		CAPet* apet = TO_APET( of );
		float stronger = df->m_con / 3 + df->m_avPassiveAddition.stronger_skill;
		switch(type)
		{
		case MSG_DAMAGE_MELEE:
		case MSG_DAMAGE_RANGE:
			nTypeProb[3] += (int)(apet->m_pProto->m_nAwful + apet->m_nPlusStr + apet->m_level + ( apet->m_level - df->m_level ));
			break;
		case MSG_DAMAGE_MAGIC:
			nTypeProb[3] += (int)(apet->m_pProto->m_nAwful + apet->m_nPlusInt + apet->m_level + ( apet->m_level - df->m_level ));
			break;
		default :
			break;
		}
		nTypeProb[0] += (int)(apet->m_pProto->m_nWeek + (apet->m_level * 0.5f) + ((df->m_level-apet->m_level)*10) + stronger);
		nTypeProb[1] += (int)(apet->m_pProto->m_nNormal + (apet->m_level * 5.75f) + ((df->m_level-apet->m_level)*10) + stronger );
		nTypeProb[2] += (int)(apet->m_pProto->m_nStrong + (apet->m_level * 1.25f) + ( apet->m_level - df->m_level ) );
		nTypeProb[4] += (int)(apet->m_pProto->m_nCritical + (apet->m_nPlusDex*0.9) + ( apet->m_level*0.9) + ( apet->m_level - df->m_level ) );
		nTypeProb[5] += (int)(apet->m_pProto->m_nDeadly + apet->m_level*0.6 + ( apet->m_level - df->m_level ) );
	}
#endif // ATTACK_PET
	
	nTypeProb[4] += of->m_avPassiveAddition.critical + (nTypeProb[4] * of->m_avPassiveRate.critical);
	nTypeProb[4] += of->m_assist.m_avAddition.critical + (nTypeProb[4] * of->m_assist.m_avRate.critical);
	nTypeProb[5] += of->m_avPassiveAddition.deadly + (nTypeProb[5] * of->m_avPassiveRate.deadly);
	nTypeProb[5] += of->m_assist.m_avAddition.deadly + (nTypeProb[5] * of->m_assist.m_avRate.deadly);

	if (IS_NPC(of))
	{
// 050225 : bs : 보스-크리&데들리, 중간보스-크리 가능하게
		CNPC* npc = TO_NPC(of);		
		if( ! npc->m_proto->CheckFlag(NPC_MBOSS|NPC_BOSS) )
		{
			nTypeProb[4] = 0;		// 크리			: 0%
			nTypeProb[5] = 0;		// 데들리		: 0%
		}
// --- 050225 : bs : 보스-크리&데들리, 중간보스-크리 가능하게
	}

	int i;
	int nMax = 0;
	for (i = 0; i < 6; i++)
	{
		nTypeProb[i] = GetRandom(1, nTypeProb[i]);
		if (nTypeProb[i] >= nTypeProb[nMax])
			nMax = i;
	}
	return nType[nMax];
}

#endif // ADULT_SERVER_NEW_BALANCE
