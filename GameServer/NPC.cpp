#include "stdhdrs.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "Exp.h"
#include "NetMsg.h"
#include "doFunc.h"

extern void DelAttackList(CCharacter* ch);

//////////////
// CNPC member
CNPC::CNPC()
#ifdef ALTERNATE_MERCHANT
:m_pstrOwnerName(MAX_CHAR_NAME_LENGTH + 1), m_pstrShopName(PS_MAX_SHOPNAME + 1)
#endif // ALTERNATE_MERCHANT
{
	m_type = MSG_CHAR_NPC;

	m_proto = NULL;
	m_idNum = -1;
	m_moveArea = 0;
	m_flag = 0;
	m_delay = 0;
	m_pulseAttack = 0;
	m_regenX = 0;
	m_regenY = 0;
	m_regenZ = 0;
	m_disableTime = 0;
	m_bStop = true;
	m_pNPCPrev = NULL;
	m_pNPCNext = NULL;
	m_aipulse = gserver.m_pulse;
	m_attackType = MSG_DAMAGE_MELEE;
	m_postregendelay = gserver.m_pulse;

	m_moveDir = 0;
	m_nBlockAI = 0;
	m_bChangeDir = false;
	m_bFail45 = false;

	m_nBlockRegen = 0;

	m_bMoveRun = false;

	// 050221 : bs : NPC 스킬
	memset(m_skills, 0, sizeof(CSkill*) * MAX_NPC_SKILL);
	// --- 050221 : bs : NPC 스킬

	memset(m_quest, 0, sizeof(int) * QUEST_MAX_NPC);
	m_nQuestCount = 0;

	m_regenInfo = NULL;
#ifndef NEW_DIVISION_EXPSP
	m_totalDamage = 0;
#endif // #ifndef NEW_DIVISION_EXPSP

#ifdef MONSTER_RAID_SYSTEM
#ifdef LC_JPN	// 일본은 3~4시간 마다 텔레포트
	m_nRaidMoveTime = gserver.m_pulse + GetRandom(PULSE_REAL_HOUR * 3, PULSE_REAL_HOUR * 4);
#else		
	m_nRaidMoveTime = gserver.m_pulse + GetRandom(PULSE_MONSTER_RAID_MOVE * 9 / 10, PULSE_MONSTER_RAID_MOVE * 11 / 10);
#endif
	m_bRaidNPCSummon = false;
#endif // MONSTER_RAID_SYSTEM

#ifdef EVENT_XMAS_2006
	m_nEventXMasMoveTime = gserver.m_pulse + PULSE_XMAS_2006_MOVE;
#endif // EVENT_XMAS_2006
	
#ifdef MOB_SCROLL
	m_MobScrollType = -1;
	m_NextMobIndex = -1;
	m_UseCharIndex = -1;
#endif // MOB_SCROLL

#ifdef MONSTER_COMBO
	m_coinidx = 0;
	m_coincount = 0;
#endif // MONSTER_COMBO

#ifdef MONSTER_AI
	m_nOldDist = 0;
	m_pulseMoveLock = 0;
	m_bMoveLock = false;
	m_bMoveToRegen = false;
#endif

#ifdef EXTREME_CUBE
	m_bCubeRegen = false;
#endif
	
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
	m_Mob_State = 0;						
	m_owner	= NULL;						
#endif  // NIGHTSHADOW_SKILL	
	
#ifdef RAID_MONSTER_SKILL  // 레이드 몬스터 스킬
	m_calling_npc = NULL;
#endif //RAID_MONSTER_SKILL
}

CNPC::~CNPC()
{	
	m_assist.ClearAssist(false, false, true, true, true);

	m_proto = NULL;
	m_regenInfo = NULL;

	int i;
	for (i = 0; i < MAX_NPC_SKILL; i++)
	{
		if (m_skills[i])
		{
			if (m_currentSkill == m_skills[i])
				m_currentSkill = NULL;
			delete m_skills[i];
		}
		m_skills[i] = NULL;
	}
	if (m_currentSkill)
		delete m_currentSkill;
	m_currentSkill = NULL;
	
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
	m_Mob_State = 0;						
	m_owner	= NULL;						
#endif  // NIGHTSHADOW_SKILL

#ifdef RAID_MONSTER_SKILL  // 레이드 몬스터 스킬
	m_calling_npc = NULL;
#endif //RAID_MONSTER_SKILL
}

void CNPC::DeleteNPC()
{
	DelAttackList(this);
	delete this;
}

void CNPC::InitAIVar()
{
	m_bChangeDir = false;
	m_nBlockAI = 0;
	m_bFail45 = false;
}

void CNPC::ResetStat()
{
	m_maxHP			= m_proto->m_hp;
	m_maxMP			= m_proto->m_mp;
	m_walkSpeed		= m_proto->m_walkSpeed;
	m_runSpeed		= m_proto->m_runSpeed;
	m_recoverHP		= m_proto->m_recoverHP;
	m_recoverMP		= m_proto->m_recoverMP;
	m_str			= m_proto->m_str;
	m_dex			= m_proto->m_dex;
	m_int			= m_proto->m_int;
	m_con			= m_proto->m_con;
	m_attackSpeed	= m_proto->m_attackSpeed;
	m_attackType	= m_proto->m_attackType;
	if (m_attackType == MSG_DAMAGE_MELEE)
		m_eqMelee	= m_proto->m_attack;
	else if (m_attackType == MSG_DAMAGE_RANGE)
		m_eqRange	= m_proto->m_attack;
	m_eqMagic		= m_proto->m_magic;
	m_eqDefense		= m_proto->m_defense;
	m_eqResist		= m_proto->m_resist;
	m_attackRange	= m_proto->m_attackArea;
	m_attribute.Reset();

	// 리젠되는 NPC의 파라미터를 리젠 정보에서 가져와 조정
	if (m_regenInfo)
	{
		m_maxHP += m_regenInfo->m_paramHP;
	}
}

int CNPC::GetAttackLevel() const
{
	return m_proto->m_attacklevel;
}

int CNPC::GetDefenseLevel() const
{
	return m_proto->m_defenselevel;
}

MSG_DAMAGE_TYPE CNPC::GetAttackType(const CSkillProto* proto) const
{
	if (proto)
	{
		switch (proto->m_type)
		{
		case ST_MELEE:
			return MSG_DAMAGE_MELEE;
		case ST_RANGE:
			return MSG_DAMAGE_RANGE;
		case ST_MAGIC:
			return MSG_DAMAGE_MAGIC;
		}
	}

	return (MSG_DAMAGE_TYPE)m_proto->m_attackType;
}

CSkill* CNPC::FindSkill(int idx)
{
	int i;
	for (i = 0; i < MAX_NPC_SKILL; i++)
	{
		if (m_skills[i])
		{
			if (m_skills[i]->m_proto->m_index == idx)
				return m_skills[i];
		}
	}
	return NULL;
}

bool CNPC::CanMove()
{
	if (!m_proto->CheckFlag(NPC_MOVING))
		return false;
	return CCharacter::CanMove();
}

void CNPC::CalcStatus(bool bSend)
{
	ResetStat();
	m_assist.Apply();
}
#ifdef NEW_DIVISION_EXPSP
LONGLONG CNPC::GetTotalDamage()
{
	LONGLONG ret = 0;
	CAttackChar* pAttackChar = m_attackList;
	while (pAttackChar)
	{
		if (pAttackChar->m_damage > 0)
			ret += pAttackChar->m_damage;

		pAttackChar = pAttackChar->m_next;
	}

	return ret;
}
#endif // #ifdef NEW_DIVISION_EXPSP

bool CNPC::CanAttack()
{
#ifndef LC_BRZ
	if (m_assist.m_state & AST_HOLD)
		return false;
#endif // LC_BRZ
	return CCharacter::CanAttack();
}


#ifdef ADULT_SERVER_NEW_BALANCE
float CNPC::GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type)
{
	return 1.0f * m_proto->m_nHit + (m_level - df->m_level);
}

float CNPC::GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type)
{
	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
		return 1.0f * m_proto->m_nDodge + (m_level - of->m_level);
	case MSG_DAMAGE_MAGIC:
		return 1.0f * m_proto->m_nMagicAvoid + (m_level - of->m_level);
	default:
		return 0.0f;
	}
}
#endif // ADULT_SERVER_NEW_BALANCE

#ifdef MONSTER_AI
float CNPC::GetDistToRegen()
{
	float xx, zz, hh;
	xx = m_regenX;
	zz = m_regenZ;
	hh = m_pArea->GetHeight(GET_YLAYER(this), m_regenX, m_regenZ);
	
	return GetDistance(xx, zz, hh, this);
}
#endif


