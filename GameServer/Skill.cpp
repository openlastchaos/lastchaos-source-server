#include "stdhdrs.h"
#include "Server.h"
#include "Skill.h"
#include "DBCmd.h"
#include "CmdMsg.h"
#include "Log.h"
#include "Battle.h"

int CAttributeData::GetValue(int type) const
{
	int ret = 0;

	switch (type)
	{
	case AT_FIRE:		ret = m_fire;
	case AT_WATER:		ret = m_water;
	case AT_EARTH:		ret = m_earth;
	case AT_WIND:		ret = m_wind;
	case AT_DARK:		ret = m_dark;
	case AT_LIGHT:		ret = m_light;
	default:			return 0;
	}
	if (ret < -50)
		return -50;
	else if (ret > 50)
		return 50;
	return ret;
}

void CAttributeData::Add(int type, int val, bool bFromItem)
{
	int mval = 0;
	if (bFromItem)
		mval = -val;
	switch (type)
	{
	case AT_FIRE:		m_fire += val;		m_water += mval;	break;
	case AT_WATER:		m_water += val;		m_fire += mval;		break;
	case AT_EARTH:		m_earth += val;		m_wind += mval;		break;
	case AT_WIND:		m_wind += val;		m_earth += mval;	break;
	case AT_DARK:		m_dark += val;		m_light += mval;	break;
	case AT_LIGHT:		m_light += val;		m_dark += mval;		break;
	case AT_ALL:
		if (!bFromItem)
		{
			m_water += val;
			m_wind += val;
			m_earth += val;
			m_fire += val;
			m_dark += val;
			m_light += val;
		}
		break;
	}
}

///////////////////
// CMagicLevelProto
CMagicLevelProto::CMagicLevelProto(int nPower, int nHitrate)
{
	m_nPowerValue = nPower;
	m_nHitrateValue = nHitrate;
}


//////////////
// CMagicProto
#ifdef NIGHTSHADOW_SKILL
CMagicProto::CMagicProto(int index, char maxlevel, char type, char subtype, char damagetype, char hittype, char attribute, int psp, int ptp, int hsp, int htp, int bTogel, CMagicLevelProto** levelproto)
#else
CMagicProto::CMagicProto(int index, char maxlevel, char type, char subtype, char damagetype, char hittype, char attribute, int psp, int ptp, int hsp, int htp, CMagicLevelProto** levelproto)
#endif // NIGHTSHADOW_SKILL
{
	m_index = index;
	m_maxlevel = maxlevel;
	m_type = type;
	m_subtype = subtype;
	m_damagetype = damagetype;
	m_hittype = hittype;
	m_attribute = attribute;
	m_psp = psp;
	m_ptp = ptp;
	m_hsp = hsp;
	m_htp = htp;
#ifdef NIGHTSHADOW_SKILL
	m_bTogel = bTogel;
#endif // NIGHTSHADOW_SKILL
	m_levelproto = levelproto;
}

CMagicProto::~CMagicProto()
{
	if (m_levelproto)
	{
		for (int i = 0; i < m_maxlevel; i++)
		{
			delete m_levelproto[i];
			m_levelproto[i] = NULL;
		}
		m_levelproto = NULL;
	}
}

const CMagicLevelProto* CMagicProto::Level(char level) const
{
	if (level < 1 || level > m_maxlevel)
		return NULL;
	else
		return (const CMagicLevelProto*)m_levelproto[level - 1];
}


///////////////////
// CSkillLevelProto

#ifdef NEW_GUILD
CSkillLevelProto::CSkillLevelProto(int needHP, int needMP, int durtime, int hate, int needItemIndex[2], int needItemCount[2], int learnLevel, int learnSP, int learnSkillIndex[3], char learnSkillLevel[3], int learnItemIndex[3], int learnItemCount[3], int learnstr, int learndex, int learnint, int learncon, int appMagicIndex[3], char appMagicLevel[3], const CMagicProto* magic[3], char magicLevel[3], int learnGP)
{
	int i;
	m_needHP = needHP;
	m_needMP = needMP;
	m_durtime = durtime;
	m_hate = hate;
	for (i = 0; i < MAX_SKILL_NEED_ITEM; i++)
	{
		m_needItemIndex[i] = needItemIndex[i];
		m_needItemCount[i] = needItemCount[i];
	}
	m_learnLevel = learnLevel;
	m_learnSP = learnSP;
	for (i = 0; i < MAX_SKILL_LEARN_SKILL; i++)
	{
		m_learnSkillIndex[i] = learnSkillIndex[i];
		m_learnSkillLevel[i] = learnSkillLevel[i];
	}
	for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
	{
		m_learnItemIndex[i] = learnItemIndex[i];
		m_learnItemCount[i] = learnItemCount[i];
	}
	m_learnstr = learnstr;
	m_learndex = learndex;
	m_learnint = learnint;
	m_learncon = learncon;
	for (i = 0; i < MAX_SKILL_APP_MAGIC; i++)
	{
		m_appMagicIndex[i] = appMagicIndex[i];
		m_appMagicLevel[i] = appMagicLevel[i];
	}
	for (i = 0; i < MAX_SKILL_MAGIC; i++)
	{
		m_magic[i] = magic[i];
		m_magicLevel[i] = magicLevel[i];
	}
	m_learnGP = learnGP;
}
#else
CSkillLevelProto::CSkillLevelProto(int needHP, int needMP, int durtime, int hate, int needItemIndex[2], int needItemCount[2], int learnLevel, int learnSP, int learnSkillIndex[3], char learnSkillLevel[3], int learnItemIndex[3], int learnItemCount[3], int learnstr, int learndex, int learnint, int learncon, int appMagicIndex[3], char appMagicLevel[3], const CMagicProto* magic[3], char magicLevel[3])
{
	int i;
	m_needHP = needHP;
	m_needMP = needMP;
	m_durtime = durtime;
	m_hate = hate;
	for (i = 0; i < MAX_SKILL_NEED_ITEM; i++)
	{
		m_needItemIndex[i] = needItemIndex[i];
		m_needItemCount[i] = needItemCount[i];
	}
	m_learnLevel = learnLevel;
	m_learnSP = learnSP;
	for (i = 0; i < MAX_SKILL_LEARN_SKILL; i++)
	{
		m_learnSkillIndex[i] = learnSkillIndex[i];
		m_learnSkillLevel[i] = learnSkillLevel[i];
	}
	for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
	{
		m_learnItemIndex[i] = learnItemIndex[i];
		m_learnItemCount[i] = learnItemCount[i];
	}
	m_learnstr = learnstr;
	m_learndex = learndex;
	m_learnint = learnint;
	m_learncon = learncon;
	for (i = 0; i < MAX_SKILL_APP_MAGIC; i++)
	{
		m_appMagicIndex[i] = appMagicIndex[i];
		m_appMagicLevel[i] = appMagicLevel[i];
	}
	for (i = 0; i < MAX_SKILL_MAGIC; i++)
	{
		m_magic[i] = magic[i];
		m_magicLevel[i] = magicLevel[i];
	}
}
#endif // NEW_GUILD

//////////////
// CSkillProto
#ifdef ATTACK_PET
#ifdef NIGHTSHADOW_SKILL
CSkillProto::CSkillProto(int index, int job, int job2, char type, int flag, int sorcerer_flag, char maxLevel, float appRange, float fireRange, float fireRange2, float minRange, char targetType, char targetNum, int useState, int useWeaponType0, int useWeaponType1, int useMagicIndex[3], char useMagicLevel[3], int appState, int appWeaponType0, int appWeaponType1, int readyTime, int stillTime, int fireTime, int reuseTime, int selfparam, int targetparam, CSkillLevelProto** levelproto, int nAPetIndex, int soul_consum, int useWearing, int appWearing )
#else
CSkillProto::CSkillProto(int index, int job, int job2, char type, int flag, int sorcerer_flag, char maxLevel, float appRange, float fireRange, float fireRange2, float minRange, char targetType, char targetNum, int useState, int useWeaponType0, int useWeaponType1, int useMagicIndex[3], char useMagicLevel[3], int appState, int appWeaponType0, int appWeaponType1, int readyTime, int stillTime, int fireTime, int reuseTime, int selfparam, int targetparam, CSkillLevelProto** levelproto, int nAPetIndex )
#endif // NIGHTSHADOW_SKILL
#else
CSkillProto::CSkillProto(int index, int job, int job2, char type, int flag, int sorcerer_flag, char maxLevel, float appRange, float fireRange, float fireRange2, float minRange, char targetType, char targetNum, int useState, int useWeaponType0, int useWeaponType1, int useMagicIndex[3], char useMagicLevel[3], int appState, int appWeaponType0, int appWeaponType1, int readyTime, int stillTime, int fireTime, int reuseTime, int selfparam, int targetparam, CSkillLevelProto** levelproto)
#endif//ATTACK_PET
{
	int i;
	
	m_index = index;
	m_job = job;
#ifdef ENABLE_CHANGEJOB
	m_job2 = job2;
#endif
	m_type = type;
	m_flag = flag;
	m_sorcerer_flag = sorcerer_flag;
	m_maxLevel = maxLevel;
	m_appRange = appRange;
	m_fireRange = fireRange;
	m_fireRange2 = fireRange2;
	m_minRange = minRange;
	m_targetType = targetType;
	m_targetNum = targetNum;
	m_useState = useState;
	m_useWeaponType0 = useWeaponType0;
	m_useWeaponType1 = useWeaponType1;
	for (i = 0; i < MAX_SKILL_USE_MAGIC; i++)
	{
		m_useMagicIndex[i] = useMagicIndex[i];
		m_useMagicLevel[i] = useMagicLevel[i];
	}
	m_applyState = appState;
	m_applyWeaponType0 = appWeaponType0;
	m_applyWeaponType1 = appWeaponType1;
	m_readyTime = readyTime;
	m_stillTime = stillTime;
	m_fireTime = fireTime;
	m_reuseTime = reuseTime;
	m_levelproto = levelproto;
	m_selfparam = selfparam;
	m_targetparam = targetparam;
#ifdef ATTACK_PET
	m_nAPetIndex = nAPetIndex;
#endif //ATTACK_PET
#ifdef NIGHTSHADOW_SKILL
	m_soul_consum = soul_consum;
	m_useWearing = useWearing;
	m_appWearing = appWearing;
#endif // NIGHTSHADOW_SKILL
}

CSkillProto::~CSkillProto()
{
	if (m_levelproto)
	{
		for (int i = 0; i < m_maxLevel; i++)
		{
			delete m_levelproto[i];
			m_levelproto[i] = NULL;
		}
		delete [] m_levelproto;
		m_levelproto = NULL;
	}
}

const CSkillLevelProto* CSkillProto::Level(char level) const
{
	if (level < 1 || level > m_maxLevel)
		return NULL;
	else
		return (const CSkillLevelProto*)m_levelproto[(int)level - 1];
}

bool CSkillProto::IsActive() const
{
	switch (m_type)
	{
	case ST_MELEE:
	case ST_RANGE:
	case ST_MAGIC:
		return true;
		
	case ST_PASSIVE:
	case ST_PET_COMMAND:
	case ST_PET_SKILL_PASSIVE:
		return false;
	}
	return false;
}

bool CSkillProto::IsPassive() const
{
	switch (m_type)
	{
	case ST_MELEE:
	case ST_RANGE:
	case ST_MAGIC:
	case ST_PET_COMMAND:
	case ST_PET_SKILL_PASSIVE:
		return false;
		
	case ST_PASSIVE:
#ifdef NEW_GUILD
	case ST_GUILD_SKILL_PASSIVE:
#endif // NEW_GUILD
		return true;
	}
	return false;
}

#ifdef FACTORY_SYSTEM
bool CSkillProto::IsSeal() const
{
	if (m_type == ST_SEAL)
		return true;
	return false;
}
#endif // FACTORY_SYSTEM

bool CSkillProto::IsSameJob(CPC* pc) const
{
	// 직업 검사
	if (m_job == pc->m_job)
	{
#ifdef ENABLE_CHANGEJOB
		if (m_job2)
		{
			if (m_job2 == pc->m_job2)
				return true;
			else
				return false;
		}
#endif
		return true;
	}
	else
		return false;
}

#ifdef ENABLE_PET
bool CSkillProto::IsSameJob(CPet* pet) const
{
	if (pet == NULL)
		return false;

	if (m_job != JOB_PET)
		return false;

	switch (m_job2)
	{
	case JOB_2ND_PET_HORSE:
		if (!(pet->GetPetType() == PET_TYPE_HORSE
			|| pet->GetPetType() == PET_TYPE_BLUE_HORSE
			|| pet->GetPetType() == PET_TYPE_UNKOWN_HORSE))
			return false;
		if (pet->IsMountType())
			return false;
		break;
	case JOB_2ND_PET_DRAGON:
		if (!(pet->GetPetType() == PET_TYPE_DRAGON 
			|| pet->GetPetType() == PET_TYPE_PINK_DRAGON
			|| pet->GetPetType() == PET_TYPE_UNKOWN_DRAGON))
			return false;
		if (pet->IsMountType())
			return false;
		break;
	case JOB_2ND_PET_HORSE_MOUNT:
		if (!(pet->GetPetType() == PET_TYPE_HORSE
			|| pet->GetPetType() == PET_TYPE_BLUE_HORSE
			|| pet->GetPetType() == PET_TYPE_UNKOWN_HORSE))
			return false;
		if (!pet->IsMountType())
			return false;
		break;
	case JOB_2ND_PET_DRAGON_MOUNT:
		if (!(pet->GetPetType() == PET_TYPE_DRAGON 
			|| pet->GetPetType() == PET_TYPE_PINK_DRAGON
			|| pet->GetPetType() == PET_TYPE_UNKOWN_DRAGON))
			return false;
		if (!pet->IsMountType())
			return false;
		break;
	default:
		return false;
	}

	return true;
}
#endif

#ifdef ATTACK_PET
bool CSkillProto::IsSameJob(CAPet* apet) const
{
	// 직업 검사
	if ( !apet )
		return false;

	if ( m_job2!=0 && m_job2 != apet->m_pProto->m_Type )
		return false;

	if ( m_nAPetIndex == 0 || m_nAPetIndex == apet->m_pProto->Index() )
	{
		return true;
	}
	else
		return false;
}
#endif //ATTACK_PET

float CSkillProto::GetFireRange(CCharacter* ch) const
{
	if (IS_PC(ch))
	{
		CPC* pc = TO_PC(ch);
		if (pc->m_wearing[WEARING_WEAPON])
		{
			if (!pc->m_wearing[WEARING_WEAPON]->IsWeaponType())
				return m_fireRange;

			int sub = pc->m_wearing[WEARING_WEAPON]->m_itemProto->m_subtypeIdx;
			if (sub == m_useWeaponType0)
				return m_fireRange;
			else if (sub == m_useWeaponType1)
				return m_fireRange2;
			else
				return m_fireRange;
		}
		else
			return m_fireRange;
	}
	else if (IS_NPC(ch))
	{
		// NPC의 경우 셀프스킬은 사정거리가 적용범위까지 포함
		// 파티의 경우 999미터
		switch (m_targetType)
		{
		case STT_SELF_RANGE:
			return m_appRange;

		case STT_PARTY_ONE:
		case STT_PARTY_ALL:
			return 999.0f;

		case STT_SELF_ONE:
		case STT_TARGET_ONE:
		case STT_TARGET_RANGE:
		case STT_TARGET_D120:
		case STT_TARGET_RECT:
		default:
			return m_fireRange;
		}
	}
#ifdef LC_USA
	else if(IS_ELEMENTAL(ch))
	{
		CElemental* elemental = TO_ELEMENTAL(ch);
		if(elemental)
		{
			CPC* pc = elemental->GetOwner();
			if(!pc)
				return m_fireRange;
			
			if (pc->m_wearing[WEARING_WEAPON])
			{
				if (!pc->m_wearing[WEARING_WEAPON]->IsWeaponType())
					return m_fireRange;
				
				int sub = pc->m_wearing[WEARING_WEAPON]->m_itemProto->m_subtypeIdx;
				if (sub == m_useWeaponType0)
					return m_fireRange;
				else if (sub == m_useWeaponType1)
					return m_fireRange2;
				else
					return m_fireRange;
			}
			else
				return m_fireRange;
		}
		else
			return m_fireRange;
	}
#endif // LC_USA
#ifdef ATTACK_PET
	else if( IS_APET(ch) )
	{
//		무기 부분 처리 필요
//		CPC* pc = TO_PC(ch);
//		if (pc->m_wearing[WEARING_WEAPON])
//		{
//			if (!pc->m_wearing[WEARING_WEAPON]->IsWeaponType())
//				return m_fireRange;
//			int sub = pc->m_wearing[WEARING_WEAPON]->m_itemProto->m_subtypeIdx;
//			if (sub == m_useWeaponType0)
//				return m_fireRange;
//			else if (sub == m_useWeaponType1)
//				return m_fireRange2;
//			else
//				return m_fireRange;
//		}
//		else
			return m_fireRange;
	}
#endif // ATTACK_PET
	else
		return 1.0f;
}

float CSkillProto::GetMinRange() const
{
	return m_minRange;
}

int CSkillProto::CheckUseState(CCharacter* ch) const
{
	CPC* pc = NULL;

	if (IS_PC(ch))
		pc = TO_PC(ch);

	if (m_useState & SCT_DEATH)
	{
		if (!DEAD(ch))
			return SCT_DEATH;
	}

	if (pc && (m_useState & SCT_WEAPON))
	{
		// 미착용
		if (pc->m_wearing[WEARING_WEAPON] == NULL)
			return SCT_WEAPON;
		// 무기 아님
		if (!pc->m_wearing[WEARING_WEAPON]->IsWeaponType())
			return SCT_WEAPON;
		// 허용 무기 아님
		int sub = pc->m_wearing[WEARING_WEAPON]->m_itemProto->m_subtypeIdx;
		if ((m_useWeaponType0 >= 0 || m_useWeaponType1 >= 0) && (sub != m_useWeaponType0 && sub != m_useWeaponType1))
			return SCT_WEAPON;
	}

	if (pc && (m_useState & SCT_SHIELD))
	{
		// 미착용
		if (pc->m_wearing[WEARING_SHIELD] == NULL)
			return SCT_SHIELD;
		// 방패 아님
		if (pc->m_wearing[WEARING_SHIELD]->m_itemProto->m_typeIdx != ITYPE_WEAR)
			return SCT_SHIELD;
		if (pc->m_wearing[WEARING_SHIELD]->m_itemProto->m_subtypeIdx != IWEAR_SHIELD)
			return SCT_SHIELD;
	}

	if (pc)
	{
		switch (m_useState & (SCT_SITDOWN | SCT_STAND))
		{
		case SCT_SITDOWN:
			if (!pc->IsSetPlayerState(PLAYER_STATE_SITDOWN))
				return SCT_SITDOWN;
			break;

		case SCT_STAND:
			if (pc->IsSetPlayerState(PLAYER_STATE_SITDOWN))
				return SCT_STAND;
			break;
		}
	}

	if ((m_useState & SCT_PEACEZONE) && ch->IsInPeaceZone(false))
		return SCT_PEACEZONE;
	
	// 사용 조건 검사 : 효과 상태
	if (pc)
	{
		int i;
		for (i = 0; i < MAX_SKILL_USE_MAGIC; i++)
		{
			if (m_useMagicIndex[i] < 0)
				continue ;
			if (!pc->m_assist.Find(m_useMagicIndex[i], m_useMagicLevel[i]))
				return -1;
		}
	}

	return 0;
}


//////////////////
// CMagicProtoList
CMagicProtoList::CMagicProtoList()
{
	m_list = NULL;
	m_count = 0;
}

CMagicProtoList::~CMagicProtoList()
{
	if (m_list)
	{
		for (int i = 0; i < m_count; i++)
		{
			delete m_list[i];
			m_list[i] = NULL;
		}
		delete [] m_list;
		m_list = NULL;
	}
}

bool CMagicProtoList::Load()
{
	CDBCmd cmd1;
	CDBCmd cmd2;
	cmd1.Init(&gserver.m_dbdata);
	cmd2.Init(&gserver.m_dbdata);

	cmd1.SetQuery("SELECT * FROM t_magic ORDER BY a_index");
	if (!cmd1.Open())
		return false;
	
	int i, j;
	m_count = cmd1.m_nrecords;
	if (m_count)
	{
		m_list = new CMagicProto*[m_count];
		memset(m_list, 0, sizeof(*m_list) * m_count);
		
		i = 0;
		while (cmd1.MoveNext())
		{
			int index;
			char maxlevel;
			char type;
			char subtype;
			char damagetype;
			char hittype;
			char attribute;
			int psp;
			int ptp;
			int hsp;
			int htp;
#ifdef NIGHTSHADOW_SKILL
			int bTogel;
#endif // NIGHTSHADOW_SKILL
			
			cmd1.GetRec("a_index", index);
			cmd1.GetRec("a_maxlevel", maxlevel);
			cmd1.GetRec("a_type", type);
			cmd1.GetRec("a_subtype", subtype);
			cmd1.GetRec("a_damagetype", damagetype);
			cmd1.GetRec("a_hittype", hittype);
			cmd1.GetRec("a_attribute", attribute);
			cmd1.GetRec("a_psp", psp);
			cmd1.GetRec("a_ptp", ptp);
			cmd1.GetRec("a_hsp", hsp);
			cmd1.GetRec("a_htp", htp);
#ifdef NIGHTSHADOW_SKILL
			cmd1.GetRec("a_togle", bTogel);
#endif // NIGHTSHADOW_SKILL
			sprintf(g_buf, "SELECT * FROM t_magicLevel WHERE a_index=%d ORDER BY a_level", index);
			
			cmd2.SetQuery(g_buf);
			if (!cmd2.Open())
				return false;
			
			CMagicLevelProto** levelproto = new CMagicLevelProto*[maxlevel];
			memset(levelproto, 0, sizeof(*levelproto) * maxlevel);
			j = 0;
			while (cmd2.MoveNext())
			{
				int power;
				int hitrate;
				
				cmd2.GetRec("a_power", power);
				cmd2.GetRec("a_hitrate", hitrate);
				
				levelproto[j] = new CMagicLevelProto(power, hitrate);
				
				j++;
			}

#ifdef NIGHTSHADOW_SKILL
			m_list[i] = new CMagicProto(index, maxlevel, type, subtype, damagetype, hittype, attribute, psp, ptp, hsp, htp, bTogel, levelproto);
#else
			m_list[i] = new CMagicProto(index, maxlevel, type, subtype, damagetype, hittype, attribute, psp, ptp, hsp, htp, levelproto);
#endif // NIGHTSHADOW_SKILL
			
			
			i++;
		}
	}
	
	return true;
}

const CMagicProto* CMagicProtoList::Find(int index)
{
	int min = 0;
	int max = m_count - 1;
	while (min <= max)
	{
		int i = (min + max) / 2;
		
		// 찾기
		if (m_list[i]->m_index == index)
			return (const CMagicProto*)(m_list[i]);
		
		else if (m_list[i]->m_index < index)
		{
			// i 뒤에 있다
			min = i + 1;
		}
		else
		{
			// i 앞에 있다
			max = i - 1;
		}
	}
	
	return NULL;
}


/////////
// CSkill
CSkill::CSkill(const CSkillProto* proto, char level)
{
	m_proto = proto;
	m_level = level;
	
	m_state = SKILL_CON_NORMAL;
	m_usetime = 0;
	m_targetType = MSG_CHAR_UNKNOWN;
	m_targetIndex = -1;
}

bool CSkill::IsReady(CCharacter* ch)
{
	if (m_state != SKILL_CON_NORMAL)
		return false;
	if (m_proto == NULL)
		return false;
	int reusetime = m_proto->m_reuseTime;
	reusetime += ch->m_magicSpeed;
	if (IS_PC(ch))
	{
		CPC* pPC = TO_PC(ch);
		if (pPC->m_opDecreaseSkillDelay > 0)
		{
			reusetime -= reusetime * pPC->m_opDecreaseSkillDelay / 100;
		}
	}
	if (gserver.m_pulse - m_usetime < reusetime)
		return false;
	return true;
}

void CSkill::Ready(CCharacter* tch)
{
	if (tch && tch->IsInvisible() && m_proto->m_index != 131)
		tch->CancelInvisible();

	m_state = SKILL_CON_READY;
	m_usetime = gserver.m_pulse;
	if (tch)
	{
		m_targetType = tch->m_type;
		m_targetIndex = tch->m_index;
	}
	else
	{
		m_targetType = MSG_CHAR_UNKNOWN;
		m_targetIndex = -1;
	}
}

void CSkill::Fire()
{
	m_state = SKILL_CON_FIRE;
}

void CSkill::Cancel(CCharacter* pCharSkill)
{
	m_state = SKILL_CON_NORMAL;
	m_targetType = MSG_CHAR_UNKNOWN;
	m_targetIndex = -1;
	if (IS_NPC(pCharSkill))
	{
		CNPC* pNPC = TO_NPC(pCharSkill);
		int i;
		for (i = 0; i < MAX_NPC_SKILL; i++)
		{
			if (this == pNPC->m_skills[i])
				return ;
		}
		delete this;
	}
}


//////////////////
// CSkillProtoList
CSkillProtoList::CSkillProtoList()
{
	m_list = NULL;
	m_count = 0;
}

CSkillProtoList::~CSkillProtoList()
{
	if (m_list)
	{
		for (int i = 0; i < m_count; i++)
		{
			delete m_list[i];
			m_list[i] = NULL;
		}
		delete [] m_list;
		m_list = NULL;
	}
}

bool CSkillProtoList::Load()
{
	CDBCmd cmd1;
	CDBCmd cmd2;
	cmd1.Init(&gserver.m_dbdata);
	cmd2.Init(&gserver.m_dbdata);
	cmd1.SetQuery("SELECT * FROM t_skill WHERE a_job!=-1 ORDER BY a_index");
	if (!cmd1.Open())
		return false;
	m_count = cmd1.m_nrecords;
	if (m_count)
	{
		m_list = new CSkillProto*[m_count];
		memset(m_list, 0, sizeof(*m_list) * m_count);
	}
	
	int i, j;
	i = 0;
	while (cmd1.MoveNext())
	{
		int index;
		int job;
		int job2 = 0;
		char type;
		int flag;
		int sorcerer_flag;
		char maxLevel;
		float appRange;
		float fireRange;
		float fireRange2;
		float minRange;
		char targetType;
		char targetNum;
		int useState;
		int useWeapon0;
		int useWeapon1;
		int useMagicIndex[3];
		char useMagicLevel[3];
		int appState;
		int appWeapon0;
		int appWeapon1;
		int readyTime;
		int stillTime;
		int fireTime;
		int reuseTime;
		int selfparam;
		int targetparam;
		CSkillLevelProto** levelproto;
#ifdef ATTACK_PET
		int	nAPetIndex;
#endif //ATTACK_PET
#ifdef NIGHTSHADOW_SKILL
		int soul_consum;
		int useWearing;
		int appWearing;
#endif // NIGHTSHADOW_SKILL
		
		cmd1.GetRec("a_index", index);
		cmd1.GetRec("a_job", job);
#ifdef ENABLE_CHANGEJOB
		cmd1.GetRec("a_job2", job2);
#endif
		cmd1.GetRec("a_type", type);
		cmd1.GetRec("a_flag", flag);
		cmd1.GetRec("a_sorcerer_flag", sorcerer_flag);
		cmd1.GetRec("a_maxLevel", maxLevel);
		cmd1.GetRec("a_appRange", appRange);
		cmd1.GetRec("a_fireRange", fireRange);
		cmd1.GetRec("a_fireRange2", fireRange2);
		cmd1.GetRec("a_minRange", minRange);
		cmd1.GetRec("a_targetType", targetType);
		cmd1.GetRec("a_targetNum", targetNum);
		cmd1.GetRec("a_useState", useState);
		cmd1.GetRec("a_useWeaponType0", useWeapon0);
		cmd1.GetRec("a_useWeaponType1", useWeapon1);
		cmd1.GetRec("a_useMagicIndex1", useMagicIndex[0]);
		cmd1.GetRec("a_useMagicLevel1", useMagicLevel[0]);
		cmd1.GetRec("a_useMagicIndex2", useMagicIndex[1]);
		cmd1.GetRec("a_useMagicLevel2", useMagicLevel[1]);
		cmd1.GetRec("a_useMagicIndex3", useMagicIndex[2]);
		cmd1.GetRec("a_useMagicLevel3", useMagicLevel[2]);
		cmd1.GetRec("a_appState", appState);
		cmd1.GetRec("a_appWeaponType0", appWeapon0);
		cmd1.GetRec("a_appWeaponType1", appWeapon1);
		cmd1.GetRec("a_readyTime", readyTime);
		cmd1.GetRec("a_stillTime", stillTime);
		cmd1.GetRec("a_fireTime", fireTime);
		cmd1.GetRec("a_reuseTime", reuseTime);
		cmd1.GetRec("a_selfparam", selfparam);
		cmd1.GetRec("a_targetparam", targetparam);
#ifdef ATTACK_PET
		cmd1.GetRec("a_apet_index", nAPetIndex);
#endif //ATTACK_PET
#ifdef NIGHTSHADOW_SKILL
		cmd1.GetRec("a_soul_consum", soul_consum);
		cmd1.GetRec("a_use_needWearingType", useWearing);
		cmd1.GetRec("a_app_needWearingType", appWearing);
#endif // NIGHTSHADOW_SKILL

		sprintf(g_buf, "SELECT * FROM t_skillLevel WHERE a_index=%d ORDER BY a_level", index);
		cmd2.SetQuery(g_buf);
		if (!cmd2.Open())
			return false;
		
		levelproto = new CSkillLevelProto*[(int)maxLevel];
		memset(levelproto, 0, sizeof(*levelproto) * maxLevel);
		j = 0;
		while (cmd2.MoveNext())
		{
			int needHP;
			int needMP;
			int durtime;
			int hate;
			int needItemIndex[2];
			int needItemCount[2];
			int learnLevel;
			int learnSP;
			int learnSkillIndex[3];
			char learnSkillLevel[3];
			int learnItemIndex[3];
			int learnItemCount[3];
			int appMagicIndex[3];
			char appMagicLevel[3];
			int magicIndex[3];
			char magicLevel[3];
			int learnstr = 0;
			int learndex = 0;
			int learnint = 0;
			int learncon = 0;
#ifdef NEW_GUILD
			int learnGP;
#endif // NEW_GUILD
			
			cmd2.GetRec("a_needHP", needHP);
			cmd2.GetRec("a_needMP", needMP);
			cmd2.GetRec("a_durtime", durtime);
			cmd2.GetRec("a_hate", hate);
			cmd2.GetRec("a_needItemIndex1", needItemIndex[0]);
			cmd2.GetRec("a_needItemCount1", needItemCount[0]);
			cmd2.GetRec("a_needItemIndex2", needItemIndex[1]);
			cmd2.GetRec("a_needItemCount2", needItemCount[1]);
			cmd2.GetRec("a_learnLevel", learnLevel);
			cmd2.GetRec("a_learnSP", learnSP);
			cmd2.GetRec("a_learnSkillIndex1", learnSkillIndex[0]);
			cmd2.GetRec("a_learnSkillLevel1", learnSkillLevel[0]);
			cmd2.GetRec("a_learnSkillIndex2", learnSkillIndex[1]);
			cmd2.GetRec("a_learnSkillLevel2", learnSkillLevel[1]);
			cmd2.GetRec("a_learnSkillIndex3", learnSkillIndex[2]);
			cmd2.GetRec("a_learnSkillLevel3", learnSkillLevel[2]);
			cmd2.GetRec("a_learnItemIndex1", learnItemIndex[0]);
			cmd2.GetRec("a_learnItemCount1", learnItemCount[0]);
			cmd2.GetRec("a_learnItemIndex2", learnItemIndex[1]);
			cmd2.GetRec("a_learnItemCount2", learnItemCount[1]);
			cmd2.GetRec("a_learnItemIndex3", learnItemIndex[2]);
			cmd2.GetRec("a_learnItemCount3", learnItemCount[2]);
			cmd2.GetRec("a_appMagicIndex1", appMagicIndex[0]);
			cmd2.GetRec("a_appMagicLevel1", appMagicLevel[0]);
			cmd2.GetRec("a_appMagicIndex2", appMagicIndex[1]);
			cmd2.GetRec("a_appMagicLevel2", appMagicLevel[1]);
			cmd2.GetRec("a_appMagicIndex3", appMagicIndex[2]);
			cmd2.GetRec("a_appMagicLevel3", appMagicLevel[2]);
			cmd2.GetRec("a_magicIndex1", magicIndex[0]);
			cmd2.GetRec("a_magicLevel1", magicLevel[0]);
			cmd2.GetRec("a_magicIndex2", magicIndex[1]);
			cmd2.GetRec("a_magicLevel2", magicLevel[1]);
			cmd2.GetRec("a_magicIndex3", magicIndex[2]);
			cmd2.GetRec("a_magicLevel3", magicLevel[2]);
			cmd2.GetRec("a_learnstr", learnstr);
			cmd2.GetRec("a_learndex", learndex);
			cmd2.GetRec("a_learnint", learnint);
			cmd2.GetRec("a_learncon", learncon);
#ifdef NEW_GUILD
			cmd2.GetRec("a_learnGP", learnGP );
#endif // NEW_GUILD

			// 효과 리스트에서 해당 효과 찾기
			const CMagicProto* magic[3];
			magic[0] = gserver.m_magicProtoList.Find(magicIndex[0]);
			magic[1] = gserver.m_magicProtoList.Find(magicIndex[1]);
			magic[2] = gserver.m_magicProtoList.Find(magicIndex[2]);
			
#ifdef NEW_GUILD
			levelproto[j] = new CSkillLevelProto(needHP, needMP, durtime, hate, needItemIndex, needItemCount, learnLevel, learnSP, learnSkillIndex, learnSkillLevel, learnItemIndex, learnItemCount, learnstr, learndex, learnint, learncon, appMagicIndex, appMagicLevel, magic, magicLevel, learnGP );
#else
			levelproto[j] = new CSkillLevelProto(needHP, needMP, durtime, hate, needItemIndex, needItemCount, learnLevel, learnSP, learnSkillIndex, learnSkillLevel, learnItemIndex, learnItemCount, learnstr, learndex, learnint, learncon, appMagicIndex, appMagicLevel, magic, magicLevel);
#endif // NEW_GUILD

			j++;
		}
#ifdef ATTACK_PET
#ifdef NIGHTSHADOW_SKILL
		m_list[i] = new CSkillProto(index, job, job2, type, flag, sorcerer_flag, maxLevel, appRange, fireRange, fireRange2, minRange, targetType, targetNum, useState, useWeapon0, useWeapon1, useMagicIndex, useMagicLevel, appState, appWeapon0, appWeapon1, readyTime, stillTime, fireTime, reuseTime, selfparam, targetparam, levelproto, nAPetIndex, soul_consum, useWearing, appWearing);
#else
		m_list[i] = new CSkillProto(index, job, job2, type, flag, sorcerer_flag, maxLevel, appRange, fireRange, fireRange2, minRange, targetType, targetNum, useState, useWeapon0, useWeapon1, useMagicIndex, useMagicLevel, appState, appWeapon0, appWeapon1, readyTime, stillTime, fireTime, reuseTime, selfparam, targetparam, levelproto, nAPetIndex );
#endif // NIGHTSHADOW_SKILL
#else
		m_list[i] = new CSkillProto(index, job, job2, type, flag, sorcerer_flag, maxLevel, appRange, fireRange, fireRange2, minRange, targetType, targetNum, useState, useWeapon0, useWeapon1, useMagicIndex, useMagicLevel, appState, appWeapon0, appWeapon1, readyTime, stillTime, fireTime, reuseTime, selfparam, targetparam, levelproto);
#endif //ATTACK_PET
		
		i++;
	}
	
	return true;
}

const CSkillProto* CSkillProtoList::Find(int skillIndex)
{
	int min = 0;
	int max = m_count - 1;
	while (min <= max)
	{
		int i = (min + max) / 2;
		
		// 찾기
		if (m_list[i]->m_index == skillIndex)
			return (const CSkillProto*)(m_list[i]);
		
		else if (m_list[i]->m_index < skillIndex)
		{
			// i 뒤에 있다
			min = i + 1;
		}
		else
		{
			// i 앞에 있다
			max = i - 1;
		}
	}
	
	return NULL;
}

CSkill* CSkillProtoList::Create(int skillIndex, int level)
{
	const CSkillProto* proto = Find(skillIndex);
	if (proto == NULL)
		return NULL;
	if (proto->Level(level) == NULL)
		return NULL;
	
	return new CSkill(proto, level);
}


/////////////////
// CSkillListNode
CSkillListNode::CSkillListNode(CSkill* skill)
{
	m_skill = skill;
	m_prev = m_next = NULL;
}

CSkillListNode* CSkillListNode::prev()
{
	return m_prev;
}

void CSkillListNode::prev(CSkillListNode* p)
{
	m_prev = p;
}

CSkillListNode* CSkillListNode::next()
{
	return m_next;
}

void CSkillListNode::next(CSkillListNode* p)
{
	m_next = p;
}

CSkill* CSkillListNode::skill()
{
	return m_skill;
}

void CSkillListNode::skill(CSkill* p)
{
	m_skill = p;
}


/////////////
// CSkillList
CSkillList::CSkillList()
{
	m_head = m_tail = NULL;
	m_count = 0;
}

CSkillList::~CSkillList()
{
	Clear();
}

bool CSkillList::Add(CSkill*& skill)
{
	if (skill == NULL)
		return false;

	CSkill* p = Find(skill->m_proto->m_index);
	if (p)
	{
		p->m_level = skill->m_level;
		delete skill;
		skill = p;
		return true;
	}
	else
	{
		CSkillListNode* node = new CSkillListNode(skill);
		
		if (m_head)
		{
			m_tail->next(node);
			node->prev(m_tail);
			m_tail = node;
			m_count++;
			return true;
		}
		else
		{
			m_head = m_tail = node;
			m_count = 1;
			return true;
		}
	}
}

bool CSkillList::Remove(CSkill* skill)
{
	if (skill == NULL)
		return false;
	
	CSkillListNode* pNode = m_head;

	while(pNode)
	{
		if (pNode->skill()->m_proto->m_index == skill->m_proto->m_index)
		{
			CSkillListNode* pPrev = pNode->prev();
			CSkillListNode* pNext = pNode->next();

			if (pPrev)
			{
				if (pNext)
				{
					pPrev->next(pNext);
					pNext->prev(pPrev);
				}
				else
				{
					pPrev->next(NULL);
					m_tail = pPrev;
				}
			}
			else
			{
				if (pNext)
				{
					pNext->prev(NULL);
					m_head = pNext;
				}
				else
				{
					m_head = m_tail = NULL;
				}
			}

			pNode->prev(NULL);
			pNode->next(NULL);

			delete pNode;
			pNode = NULL;

			m_count--;

			break;
		}
		pNode = pNode->next();
	}
	return true;
}

CSkill* CSkillList::Find(int index)
{
	CSkillListNode* p = m_head;
	while (p)
	{
		if (p->skill()->m_proto->m_index == index)
			return p->skill();
		else
			p = p->next();
	}
	
	return NULL;
}

int CSkillList::count()
{
	return m_count;
}

char* CSkillList::GetIndexString(char* buf)
{
	*buf = '\0';
	CSkillListNode* p = m_head;
	while (p)
	{
#ifdef NEWYEAR_EVENT_2008
		if(p->skill()->m_proto->m_index != 491)
		{
#endif // NEWYEAR_EVENT_2008
			IntCat(buf, p->skill()->m_proto->m_index, true);
#ifdef NEWYEAR_EVENT_2008
		}
#endif // NEWYEAR_EVENT_2008
		p = p->next();
	}
	return buf;
}

char* CSkillList::GetLevelString(char* buf)
{
	*buf = '\0';
	CSkillListNode* p = m_head;
	while (p)
	{
		IntCat(buf, p->skill()->m_level, true);
		p = p->next();
	}
	return buf;
}

void* CSkillList::GetHeadPosition()
{
	return (void*)m_head;
}

CSkill* CSkillList::GetNext(void*& p)
{
	CSkillListNode* node = (CSkillListNode*)p;
	
	if (node == NULL)
	{
		p = NULL;
		return NULL;
	}
	
	p = (void*)node->next();
	return node->skill();
}

#ifdef ENABLE_CHANGEJOB
int CSkillList::RemoveSkillFor2ndJob()
{
	CSkillListNode* p;
	CSkillListNode* pn = m_head;

	int spendSkillPoint=0;
	int skillLevel=0;

	while ((p = pn))
	{
		pn = p->next();

		CSkill* s = p->skill();

		if (s->m_proto->Get2ndJob())
		{
			if (p->prev())
			{
				p->prev()->next(p->next());
			}
			if (p->next())
			{
				p->next()->prev(p->prev());
			}
			if (p == m_head)
				m_head = p->next();
			if (p == m_tail)
				m_tail = p->prev();
			m_count--;
			p->next(NULL);
			p->prev(NULL);

			skillLevel = s->m_level;
			for(;skillLevel > 0; skillLevel--)
			{
				spendSkillPoint += s->m_proto->Level(skillLevel)->m_learnSP * 10000;
			}
			delete p;
		}
	}

	return spendSkillPoint;
}
#endif

void CSkillList::Clear()
{
	m_count = 0;
	while (m_head)
	{
		CSkillListNode* node = m_head->next();
		delete m_head;
		m_head = node;
	}
	m_head = m_tail = NULL;
}


///////////////////////
// ApplySkill Functions


/////////////////////
// Function name	: ApplyMagic
// Description	    : 효과 적용
// Argument         : CCharacter* ch
//                  : 사용하는 캐릭터 캐릭터
// Argument         : CCharacter* tch
//                  : 적용되는 캐릭터
// Argument         : const CMagicProto* magic
//                  : 효과 프로토
// Argument         : const CMagicLevelProto* magiclevel
//                  : 효과 레벨 설정
// Argument         : bool& bApply
//                  : 효과 적용 여부, 적용하면 true, 아니면 false로 세팅됨
void ApplyMagic(CCharacter* ch, CCharacter* tch, CSkill* skill, int magicindex, bool bForce, bool& bApply)
{
	const CSkillProto* proto = skill->m_proto;
	if (proto == NULL)
		return ;
	const CSkillLevelProto* levelproto = proto->Level(skill->m_level);
	if (levelproto == NULL)
		return ;

	if (magicindex < 0 || magicindex >= MAX_SKILL_MAGIC)
		return ;

	const CMagicProto* magic = levelproto->m_magic[magicindex];
	if (magic == NULL)
		return ;

	const CMagicLevelProto* magiclevel = magic->Level(levelproto->m_magicLevel[magicindex]);
	if (magiclevel == NULL)
		return ;

#ifdef ENABLE_PET
	if (IS_PET(tch))
	{
		if (proto->m_flag & SF_HELP)
			return ;
		if (levelproto->m_durtime != 0)
			return ;
	}
#endif
	
#ifdef ATTACK_PET
	if( IS_APET(tch) )
	{
		int debug_test=0;
		debug_test =1;
	}
#endif // ATTACK_PET

	CNetMsg rmsg;

	// 명중 판단
#ifdef ENABLE_ROGUE_SKILL125_BRZ
	if (!bForce && SelectHitType(ch, tch, MSG_DAMAGE_MAGIC, HITTYPE_MISS | HITTYPE_NORMAL, magic, magiclevel, (skill->m_proto->m_flag & SF_NOTHELP) ? true : false, skill->m_level) == HITTYPE_MISS)
#else
	if (!bForce && SelectHitType(ch, tch, MSG_DAMAGE_MAGIC, HITTYPE_MISS | HITTYPE_NORMAL, magic, magiclevel, (skill->m_proto->m_flag & SF_NOTHELP) ? true : false) == HITTYPE_MISS)
#endif // ENABLE_ROGUE_SKILL125_BRZ
	{
#ifdef ENABLE_ROGUE_SKILL125_BRZ
		if(magic->m_index != 173)
#endif // ENABLE_ROGUE_SKILL125_BRZ
		bApply = true;
		return ;
	}

	int nPowerValue = magiclevel->m_nPowerValue;
	// 시전, 대상의 파라미터 추가
	nPowerValue = nPowerValue * CalcSkillParam(ch, tch, magic->m_psp, magic->m_ptp) / 100;

	// 즉효성 보조 효과들
	switch (magic->m_type)
	{

#ifdef SAFEGUARD	
	case MT_ASSIST:
		switch (magic->m_subtype)
		{
		case MST_ASSIST_SAFEGUARD:
			if (IS_PC(tch))
			{	// PC에게만 시전 가능
				CPC * pc = TO_PC(tch);
				if (pc != NULL)
				{
					pc->m_bImmortal = true;
					pc->m_assist.m_state |= AST_SAFEGUARD;
//					pc->m_bChangeStatus = true;
//					CharStatusMsg(rmsg, tch, AST_SAFEGUARD);
//					tch->m_pArea->SendToCell(rmsg, tch, true);
				}				
			}
			break;
#ifdef NIGHTSHADOW_SKILL
		case MST_ASSIST_TAMING:
			// 테이밍이 되는 npc는 주인을 넣어 주어야 한다.
			if ( IS_NPC(tch) && IS_PC(ch) )
			{
				CNPC* pNPC = TO_NPC(tch);
				CPC*  pPC =	 TO_PC(ch);
				if ( pNPC && pPC )
				{
					pNPC->SetOwner(pPC);
				}
			}
#endif // NIGHTSHADOW_SKILL
		}
		break;
#endif // SAFEGUARD

	case MT_RECOVER:
		if (DEAD(tch))
			break;

		switch (magic->m_subtype)
		{
		case MST_RECOVER_HP:
			switch (magic->m_damagetype)
			{
			case MDT_ONLYPOWER:
				tch->m_hp += nPowerValue;
				bApply = true;
				break;

			case MDT_ADDITION:
				GAMELOG << init("SYS_ERR")
						<< "INVALID MAGIC DATA"
						<< delim
						<< magic->m_index
						<< end;
				return ;

			case MDT_RATE:
				tch->m_hp += tch->m_maxHP * nPowerValue / SKILL_RATE_UNIT;
				bApply = true;
				break;
			}

			if (tch->m_hp > tch->m_maxHP)
				tch->m_hp = tch->m_maxHP;

			if (IS_PC(tch))
			{
				CPC* pc = TO_PC(tch);
				pc->m_bChangeStatus = true;
			}
			else if (IS_ELEMENTAL(tch))
			{
				CElemental* pElemental = TO_ELEMENTAL(tch);
				pElemental->m_bChangeStatus = true;
			}
#ifdef ATTACK_PET
			if( IS_APET(tch) )
			{
				CAPet* apet = TO_APET(tch);
				ExAPetStatusMsg(rmsg, apet );
				apet->m_pArea->SendToCell(rmsg, apet->GetOwner(), true);
			}
			else
			{
				CharStatusMsg(rmsg, tch, 0);
				tch->m_pArea->SendToCell(rmsg, tch, true);
			}
#endif//ATTACK_PET
			break;

		case MST_RECOVER_MP:
			switch (magic->m_damagetype)
			{
			case MDT_ONLYPOWER:
				tch->m_mp += nPowerValue;
				bApply = true;
				break;

			case MDT_ADDITION:
				GAMELOG << init("SYS_ERR")
						<< "INVALID MAGIC DATA"
						<< delim
						<< magic->m_index
						<< end;
				return ;

			case MDT_RATE:
				tch->m_mp += tch->m_maxMP * nPowerValue / SKILL_RATE_UNIT;
				bApply = true;
				break;
			}

			if (tch->m_mp > tch->m_maxMP)
				tch->m_mp = tch->m_maxMP;

			if (IS_PC(tch))
			{
				CPC* pc = TO_PC(tch);
				pc->m_bChangeStatus = true;
			}
			else if (IS_ELEMENTAL(tch))
			{
				CElemental* pElemental = TO_ELEMENTAL(tch);
				pElemental->m_bChangeStatus = true;
			}
#ifdef ATTACK_PET
			if( IS_APET(tch) )
			{
				CAPet* apet = TO_APET(tch);
				ExAPetStatusMsg(rmsg, apet );
				tch->m_pArea->SendToCell(rmsg, apet->GetOwner(), true);
			}
			else
			{
				CharStatusMsg(rmsg, tch, 0);
				tch->m_pArea->SendToCell(rmsg, tch, true);
			}
#endif//ATTACK_PET
			break;

#ifdef ATTACK_PET
		case MST_RECOVER_STM :
			{
				if( IS_APET(tch) )
				{
					CAPet* apet = TO_APET( tch );
					switch (magic->m_damagetype)
					{
					case MDT_ONLYPOWER:
						apet->AddStamina(nPowerValue);
						bApply = true;
						break;

					case MDT_ADDITION:
						GAMELOG << init("SYS_ERR") << "INVALID MAGIC DATA" << delim
								<< magic->m_index << end;
						return ;

					case MDT_RATE:
						apet->AddStamina( apet->m_pProto->m_nMaxStm * nPowerValue / SKILL_RATE_UNIT );
						bApply = true;
						break;
					}
					
					ExAPetStatusMsg(rmsg, apet );
					tch->m_pArea->SendToCell(rmsg, apet->GetOwner(), true);
				}
			}break;

		case MST_RECOVER_FAITH :
			{
				if( IS_APET(tch) )
				{
					CAPet* apet = TO_APET( tch );
					switch (magic->m_damagetype)
					{
					case MDT_ONLYPOWER:
						apet->AddFaith(nPowerValue);
						bApply = true;
						break;

					case MDT_ADDITION:
						GAMELOG << init("SYS_ERR") << "INVALID MAGIC DATA" << delim
								<< magic->m_index << end;
						return ;

					case MDT_RATE:
						apet->AddFaith( apet->m_pProto->m_nMaxFaith * nPowerValue / SKILL_RATE_UNIT );
						bApply = true;
						break;
					}

					ExAPetStatusMsg(rmsg, apet );
					tch->m_pArea->SendToCell(rmsg, apet->GetOwner(), true);
				}
			}break;
#endif//ATTACK_PET	
		}
		break;	
	case MT_CURE:
		switch (magic->m_subtype)
		{
		case MST_CURE_BLIND:
			tch->m_assist.CureAssist(MST_ASSIST_BLIND, levelproto->m_magicLevel[magicindex]);
			bApply = true;
			break;

		case MST_CURE_POISON:
			tch->m_assist.CureAssist(MST_ASSIST_POISON, levelproto->m_magicLevel[magicindex]);
			bApply = true;
			break;

		case MST_CURE_HOLD:
			tch->m_assist.CureAssist(MST_ASSIST_HOLD, levelproto->m_magicLevel[magicindex]);
			bApply = true;
			break;

		case MST_CURE_CONFUSION:
			tch->m_assist.CureAssist(MST_ASSIST_CONFUSION, levelproto->m_magicLevel[magicindex]);
			bApply = true;
			break;

		case MST_CURE_STONE:
			tch->m_assist.CureAssist(MST_ASSIST_STONE, levelproto->m_magicLevel[magicindex]);
			bApply = true;
			break;

		case MST_CURE_SILENT:
			tch->m_assist.CureAssist(MST_ASSIST_SILENT, levelproto->m_magicLevel[magicindex]);
			bApply = true;
			break;

		case MST_CURE_BLOOD:
			tch->m_assist.CureAssist(MST_ASSIST_BLOOD, levelproto->m_magicLevel[magicindex]);
			bApply = true;
			break;

		case MST_CURE_REBIRTH:
			if (DEAD(tch))
			{
#ifdef DRATAN_CASTLE
				CDratanCastle * pCastle = CDratanCastle::CreateInstance();
				if (pCastle != NULL)
				{
					if (IS_PC(tch))
					{
						CPC * pc = TO_PC(tch);
						if (pc != NULL
							&& pc->GetJoinFlag(ZONE_DRATAN) != WCJF_NONE
							&& pc->GetMapAttr() == MAPATT_WARZONE
							&& pCastle->GetState() != WCSF_NORMAL
							&& pc->m_pZone->m_index == ZONE_DRATAN)
						{
							switch(pc->GetJoinFlag(ZONE_DRATAN))
							{
							case WCJF_ATTACK_GUILD:
								{
									// 부활진지가 없으면 사용불가
									int gindex = pc->m_guildInfo->guild()->index();
									int i;
									for(i = 0; i < 7; i++)
									{
										if (pCastle->m_nRebrithGuild[i] == gindex)		
										{
											break;
										}
									}

									if(i == 7)
									{
										if(IS_PC(ch))
										{
											CPC * ppc = TO_PC(ch);
											if(ppc)
											{
												SkillErrorMsg(rmsg, MSG_SKILL_ERROR_CANNOT_SPELL, skill->m_proto->m_index, 0);
												SEND_Q(rmsg, ppc->m_desc);										
											}
										}										
										return;
									}
									
									break;
								}

							case WCJF_OWNER:
							case WCJF_DEFENSE_GUILD:
								// 워프타워가 없으면 사용불가
								if( pCastle->GetTowerStatus(7) == 0)
								{
									if(IS_PC(ch))
									{
										CPC * ppc = TO_PC(ch);
										if(ppc)
										{
											SkillErrorMsg(rmsg, MSG_SKILL_ERROR_CANNOT_SPELL, skill->m_proto->m_index, 0);
											SEND_Q(rmsg, ppc->m_desc);										
										}
									}	
									return;
								}								
								break;
							}
						}
					}
				}
#endif // DRATAN_CASTLE
				bApply = true;

				tch->m_hp = tch->m_maxHP / 2;
				if (tch->m_mp < tch->m_maxMP / 2)
					tch->m_mp = tch->m_maxMP / 2;

				// 살아난거 알리기
				CNetMsg rmsg;
				RebirthMsg(rmsg, tch);
				if (tch->m_pArea)
					tch->m_pArea->SendToCell(rmsg, tch, true);

				if (IS_PC(tch))
				{
					CPC* pc = TO_PC(tch);
					GoZone(pc, 
						pc->m_pZone->m_index, 
						pc->m_pos.m_yLayer, 
						pc->m_pos.m_x, 
						pc->m_pos.m_z);

					switch (magic->m_damagetype)
					{
					case MDT_RATE:
						if (!pc->IsChaotic())
							pc->m_exp += (LONGLONG)(pc->m_loseexp * 1.0 * nPowerValue / SKILL_RATE_UNIT + 0.5);
						break;

					default:
						if (!pc->IsChaotic())
							pc->m_exp += (pc->m_loseexp < nPowerValue) ? pc->m_loseexp : nPowerValue;
						break;
					}
					pc->m_loseexp = 0;
					pc->CalcStatus(true);
					pc->SaveNow(false);
				}
			}
			break;

		case MST_CURE_INVISIBLE:
			if (tch->IsInvisible())
			{
				tch->m_assist.CureAssist(MST_ASSIST_INVISIBLE, levelproto->m_magicLevel[magicindex]);
			}
			bApply = true;
			break;

		case MST_CURE_STURN:
			tch->m_assist.CureAssist(MST_ASSIST_STURN, levelproto->m_magicLevel[magicindex]);
			bApply = true;
			break;

		case MST_CURE_SLOTH:
			tch->m_assist.CureAssist(MST_ASSIST_SLOTH, levelproto->m_magicLevel[magicindex]);
			bApply = true;
			break;
			
		case MST_CURE_DISEASE:
			tch->m_assist.CureAssist(MST_ASSIST_DISEASE, levelproto->m_magicLevel[magicindex]);
			bApply = true;
			break;

		case MST_CURE_CURSE :
			tch->m_assist.CureAssist(MST_ASSIST_CURSE, levelproto->m_magicLevel[magicindex]);
			bApply = true;
			break;

		case MST_CURE_NOTHELP:
			{
				// not 헬프가 아닌 버프를 모두 치료
				tch->m_assist.ClearAssist(true, false, false, true, false);
			}
			bApply = true;
			break;



		}
		break;
	
	case MT_OTHER:
		switch (magic->m_subtype)
		{
		case MST_OTHER_SKILLCANCEL:
			if (tch->m_currentSkill && tch->m_currentSkill->m_state == SKILL_CON_READY)
			{
				tch->m_currentSkill->Cancel(tch);
				tch->m_currentSkill = NULL;
				SkillCancelMsg(rmsg, tch);
				if (tch->m_pArea)
					tch->m_pArea->SendToCell(rmsg, tch, true);
			}
			bApply = true;
			break;

		case MST_OTHER_TELEKINESIS:
#if defined(LC_JPN) || defined(LC_BRZ)
			if (IS_NPC(tch))
			{
				CNPC * pTarget = TO_NPC(tch);
				if (pTarget != NULL
					&& pTarget->m_proto->CheckFlag(NPC_CASTLE_TOWER) != 0)
				{
					bApply = false;
					break;
				}
			}
#endif // LC_JPN		
			
#ifdef DRATAN_CASTLE
			if (IS_NPC(tch))
			{
				CNPC * pNPC = TO_NPC(tch);
				if(pNPC != NULL 
					&& pNPC->m_proto->CheckFlag(NPC_WARCASTLE) == 0
					&& pNPC->m_proto->CheckFlag(NPC_CASTLE_TOWER) == 0)
				{
#endif // DRATAN_CASTLE
					GoTo(tch, GET_YLAYER(ch), GET_X(ch), GET_Z(ch), GET_H(ch), GET_R(tch));
					bApply = true;
#ifdef DRATAN_CASTLE
				}
			}
#ifdef LC_USA
			else if(IS_PC(tch))
			{
				CPC* pc = TO_PC(tch);
				CWarCastle* castle = CWarCastle::GetCastleObject(pc->m_pZone->m_index);
				if(castle)
				{	
					if(pc->m_pArea->GetAttr( pc->m_pos.m_yLayer, pc->m_pos.m_x, pc->m_pos.m_z ) != MAPATT_WARZONE &&
						 pc->m_pZone->IsWarZone(pc->m_pos.m_x, pc->m_pos.m_z) == false)
					{
						GoTo(tch, GET_YLAYER(ch), GET_X(ch), GET_Z(ch), GET_H(ch), GET_R(tch));
						bApply = true;
					}
				}
				else
				{
					GoTo(tch, GET_YLAYER(ch), GET_X(ch), GET_Z(ch), GET_H(ch), GET_R(tch));
					bApply = true;
				}
			}
			else
			{
				GoTo(tch, GET_YLAYER(ch), GET_X(ch), GET_Z(ch), GET_H(ch), GET_R(tch));
				bApply = true;
			}
#endif // LC_USA
#endif // DRATAN_CASTLE
			break;

#ifdef ENABLE_PET
		case MST_OTHER_TOUNT:
			if (ch != tch && IS_NPC(tch))
			{
				// 시전자와 대상이 다를때, 대상의 최고 hate 수치 + 스킬 파워로 시전자 hate가 결정되며, 최고 수치의 해당자는 시전자의 hate로 설정
				// 시전자의 대상에대한 hate => a
				// 대상의 최고 hate 수치 => b
				// 대상의 최고 hate 캐릭터 => c
				// c.hate = a
				// a = b + skill
				CAttackChar* p = tch->m_attackList;
				int nMaxHate = 0;
				CAttackChar* pMaxHate = NULL;
				int nCurHate = 0;
				CAttackChar* pCurHate = NULL;
				while (p)
				{
					if (nMaxHate < p->m_targetHate)
					{
						nMaxHate = p->m_targetHate;
						pMaxHate = p;
					}
					if (ch == p->ch)
					{
						nCurHate = p->m_targetHate;
						pCurHate = p;
					}
					p = p->m_next;
				}

				if (pMaxHate)
					pMaxHate->m_targetHate = nCurHate;
				if (pCurHate)
					pCurHate->m_targetHate = 0;

				ApplyHate(ch, tch, nMaxHate + nPowerValue, false);

				bApply = true;
			}
			break;
#endif // #ifdef ENABLE_PET

		case MST_OTHER_SUMMON:
			if (IS_PC(ch))
			{
				CPC* pc = TO_PC(ch);
				pc->SummonElemental(magiclevel->m_nPowerValue);
				bApply = true;
			}
			
#ifdef RAID_MONSTER_SKILL
			else if ( IS_NPC(ch) )
			{
				CNPC* pNPC = TO_NPC(ch);

				// 소환 설정이 입력 되어 있으면
				if ( pNPC->m_proto->m_aileader_idx > 0 && pNPC->m_proto->m_aileader_count > 0 )
				{
					for(int i = 0; i < pNPC->m_proto->m_aileader_count; i++)
					{
						float x = 0.0f;
						float z = 0.0f;
						int nTry = 0;
						do
						{
							x = GET_X(pNPC) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 30) / 10.0f;
							z = GET_Z(pNPC) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 30) / 10.0f;
							switch (pNPC->m_pArea->GetAttr(GET_YLAYER(pNPC), x, z))
							{
							case MAPATT_FIELD:
							case MAPATT_STAIR_UP:
							case MAPATT_STAIR_DOWN:
								break;
							default:
								x = 0.0f;
								z = 0.0f;
								break;
							}
							nTry++;
						} while (nTry <= 10 && x < 0.1f && z < 0.1f);
						if (nTry > 10)
						{
							x = GET_X(pNPC);
							z = GET_Z(pNPC);
						}

						CNPC* rnpc = gserver.m_npcProtoList.Create(pNPC->m_proto->m_aileader_idx, NULL);
						if (!rnpc)
							continue ;

						GET_X(rnpc) = x;
						GET_YLAYER(rnpc) = GET_YLAYER(pNPC);
						GET_Z(rnpc) = z;
						GET_R(rnpc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
						rnpc->m_disableTime = 0;
						rnpc->m_postregendelay = 0;
						
						rnpc->m_regenX = GET_X(rnpc);
						rnpc->m_regenY = GET_YLAYER(rnpc);
						rnpc->m_regenZ = GET_Z(rnpc);

						int cx, cz;
						pNPC->m_pArea->AddNPC(rnpc);
						pNPC->m_pArea->PointToCellNum(GET_X(rnpc), GET_Z(rnpc), &cx, &cz);
						pNPC->m_pArea->CharToCell(rnpc, GET_YLAYER(rnpc), cx, cz);
						
						CNetMsg appearNPCMsg;
						AppearMsg(appearNPCMsg, rnpc, true);
						pNPC->m_pArea->SendToCell(appearNPCMsg, GET_YLAYER(rnpc), cx, cz);
					}
				}
			}
#endif // RAID_MONSTER_SKILL
			break;

		case MST_OTHER_EVOCATION:
			if (IS_PC(ch))
			{
				CPC* pc = TO_PC(ch);
				pc->Evocation(magiclevel->m_nPowerValue);
				bApply = true;
			}
			break;

		case MST_OTHER_TARGETFREE:
			if (IS_NPC(tch))
			{
				CNPC* pNPCTarget = TO_NPC(tch);
				DelTargetFromAttackList(pNPCTarget, ch);
				if (pNPCTarget->m_attackList == NULL)
					pNPCTarget->m_postregendelay = gserver.m_pulse;
				bApply = true;
			}
			break;	

#ifdef NIGHTSHADOW_SKILL
		case MST_OTHER_SOUL_DRAIN:
			if ( IS_PC(ch) )
			{
				CPC* pPC = TO_PC(ch);
				int count = 0;

				// 가지고 있는 영혼의 개수를 한개 늘린다.
				if(pPC)
				{
					count = pPC->GetSoul_Count();
					count++;
					pPC->SetSoul_Count(count);
				}
			}
			break;

		case MST_OTHER_FLY:
			if ( IS_PC(ch) )
			{
				CPC* pPC = TO_PC(ch);
				
				// 날기에 관한 효과가 추가되어야 한다. 아직 미정
				if(pPC)
				{
					
				}
			}
			break;
#endif // NIGHTSHADOW_SKILL

#ifdef RAID_MONSTER_SKILL
		case MST_OTHER_CALL:
			if ( IS_NPC(tch) && IS_NPC(ch) )
			{
				// 몬스터 행동을 호출됨 상태로 변환
				CNPC* ptNPC = TO_NPC(tch);  // 호출 당한 npc
				CNPC* pNPC = TO_NPC(ch);    // 호출 한 npc
				
				// 둘다 있을 때에만
				if( pNPC && ptNPC )
				{
					ptNPC->Set_MobFlag( STATE_MONSTER_CALLING );
					ptNPC->m_calling_npc = pNPC;
				}	
			}
			break;
#endif //RAID_MONSTER_SKILL	
		}
		break;
		
	}
}

#ifdef NEWYEAR_EVENT_2008
void ApplyNewYearSkill(CPC* ch, CPC * tch)
{
	if(ch == NULL || tch == NULL)
	{	// 시전자와 피시전자 체크
		return;
	}

	if(tch && tch->m_assist.FindBySkillIndex(492))
	{	// 버프는 하시간에 한번만 이를 위해서 시전시 492 스킬도 같이 시전
		return;
	}

	// tch의 허락 요청
	CNetMsg rmsg;
	NewYearSkillReqMsg(rmsg, ch);
	SEND_Q(rmsg, tch->m_desc);
}
#endif // NEWYEAR_EVENT_2008

/////////////////////
// Function name	: ApplySkill
// Description	    : 스킬 적용, 시전자 사망시 -1 리턴, 대상 사망시 1 리턴, 그외 0
// Argument         : CCharacter* ch
//                  : 시전자
// Argument         : CCharacter* tch
//                  : 대상
// Argument         : CSkill* skill
//                  : 스킬
// Argument         : int itemidx
//                  : 아이템에 의한 스킬일 경우 아이템 인덱스, 그외는 -1
// Argument         : bool& bApply
//                  : 스킬이 정상적으로 적용되면 true, 아니면 false
int ApplySkill(CCharacter* ch, CCharacter* tch, CSkill* skill, int itemidx, bool& bApply)
{
	bApply = false;
	if( !ch || !tch )
		return 0;

	// 060227 : bs : 중복안되는 스킬인지 검사
	if ((skill->m_proto->m_flag & SF_NOTDUPLICATE) && ch->m_assist.FindBySkillIndex(skill->m_proto->m_index))
		return 0;

#ifdef ENABLE_PET
	if ((skill->m_proto->m_flag & SF_NOTHELP) && !ch->IsInPeaceZone(true))
	{
		CPC* pc = NULL;
		switch (ch->m_type)
		{
		case MSG_CHAR_PC:
			pc = TO_PC(ch);
			break;

#ifdef ENABLE_PET
		case MSG_CHAR_PET:
			pc = TO_PET(ch)->GetOwner();
			break;
#endif // #ifdef ENABLE_PET

		case MSG_CHAR_ELEMENTAL:
			pc = TO_ELEMENTAL(ch)->GetOwner();
			break;

#ifdef ATTACK_PET
		case MSG_CHAR_APET:
			pc = TO_APET(ch)->GetOwner();
			break;
#endif //ATTACK_PET

		default:
			break;
		}
		if ( pc != NULL
			&& tch != NULL
			&& IS_NPC(tch)
			&& pc->m_level - tch->m_level <= 5)
		{
			CNPC * pTemp = TO_NPC(tch);
			if (pTemp != NULL
				&& pTemp->m_proto->m_index != 303 /*악마의 묘지*/)			
			{
				pc->m_pulseLastAttackSkill = gserver.m_pulse;
			}
		}
	}
#endif // #ifdef ENABLE_PET

#ifdef MONSTER_RAID_SYSTEM
	// NPC가 공격을 해도 전투 상태를 저장
	if (IS_NPC(ch))
#ifdef LC_JPN	// 일본은 3~4시간 마다 텔레포트
		TO_NPC(ch)->m_nRaidMoveTime = gserver.m_pulse + GetRandom(PULSE_REAL_HOUR * 3, PULSE_REAL_HOUR * 4);
#else			
		TO_NPC(ch)->m_nRaidMoveTime = gserver.m_pulse + GetRandom(PULSE_MONSTER_RAID_MOVE * 9 / 10, PULSE_MONSTER_RAID_MOVE * 11 / 10);
#endif	// LC_JPN

	// NPC가 맞아도 저장
	if (IS_NPC(tch))
#ifdef LC_JPN	// 일본은 3~4시간 마다 텔레포트
		TO_NPC(tch)->m_nRaidMoveTime = gserver.m_pulse + GetRandom(PULSE_REAL_HOUR * 3, PULSE_REAL_HOUR * 4);
#else			
		TO_NPC(tch)->m_nRaidMoveTime = gserver.m_pulse + GetRandom(PULSE_MONSTER_RAID_MOVE * 9 / 10, PULSE_MONSTER_RAID_MOVE * 11 / 10);
#endif // LC_JPN
#endif // MONSTER_RAID_SYSTEM

#ifdef EVENT_XMAS_2006
	if (IS_NPC(ch))
		TO_NPC(ch)->m_nEventXMasMoveTime = gserver.m_pulse + PULSE_XMAS_2006_MOVE;
	if (IS_NPC(tch))
		TO_NPC(tch)->m_nEventXMasMoveTime = gserver.m_pulse + PULSE_XMAS_2006_MOVE;
#endif // EVENT_XMAS_2006

	if (IS_PC(tch))
	{
		CPC* pc = TO_PC(tch);
		if (pc->m_bImmortal || pc->IsDisable())
			return 0;
	}

	const CSkillProto* proto;
	const CSkillLevelProto* levelproto;
	
	int i;
	
	if ((proto = skill->m_proto) == NULL)
		return 0;
	if ((levelproto = proto->Level(skill->m_level)) == NULL)
		return 0;

	if (proto->m_flag & SF_SINGLEMODE)
	{
		if (!ch->m_pZone->IsPersonalDungeon())
			return 0;
	}

	// SSF_APP_ 검사
	if (proto->CheckSorcererFlag(SSF_APP_ALL))
	{
		if (tch->m_type == MSG_CHAR_ELEMENTAL)
		{
			CElemental* pElemental = TO_ELEMENTAL(tch);
			switch (pElemental->GetElementalType())
			{
			case ELEMENTAL_FIRE:
				if (!proto->CheckSorcererFlag(SSF_APP_FIRE))
					return 0;
				break;
			case ELEMENTAL_WIND:
				if (!proto->CheckSorcererFlag(SSF_APP_WIND))
					return 0;
				break;
			case ELEMENTAL_EARTH:
				if (!proto->CheckSorcererFlag(SSF_APP_EARTH))
					return 0;
				break;
			case ELEMENTAL_WATER:
				if (!proto->CheckSorcererFlag(SSF_APP_WATER))
					return 0;
				break;
			default:
				return 0;
			}
		}
#ifdef ATTACK_PET
		else if ( tch->m_type == MSG_CHAR_APET )
		{

		}
#endif //ATTACK_PET
		else
		{
			if (!proto->CheckSorcererFlag(SSF_APP_CHARACTER))
				return 0;
		}
	}

	// 그 어떤 스킬도 피스존에서 다른 지역으로 사용 불가
#ifdef ADULT_SERVER
	if ( IS_NPC(tch) ) // NPC 일때만
	{
#endif // ADULT_SERVER
		if (ch->IsInPeaceZone(true) && !tch->IsInPeaceZone(false)
			&& skill->m_proto->m_index != 511) ///=== 511, 스트레이아나 맵 스킬 제외
		{
			return 0;
		}
#ifdef ADULT_SERVER
	}
#endif //ADULT_SERVER

	bool bHelp = (proto->m_flag & SF_HELP) ? true : false;
	bool bNotHelp = (proto->m_flag & SF_NOTHELP) ? true : false;
	// !((help on && 적아님) || (nothelp && 적)) => 스킬 사용 불가
	if (!((bHelp && !ch->IsEnemy(tch)) || (bNotHelp && ch->IsEnemy(tch)))
		&& skill->m_proto->m_index != 511) ///=== 511, 스트레이아나 맵 스킬 제외
	{
		return 0;
	}

#ifndef ADULT_SERVER
	// NOT HELP는 피스존에서 사용 불가
	if ((bNotHelp && ch->IsInPeaceZone(true))
		&& skill->m_proto->m_index != 511) ///=== 511, 스트레이아나 맵 스킬 제외
	{
		return 0;
	}
#endif	//ADULT_SERVER

#ifdef RESTRICT_PK
	if((bHelp && IS_PC(tch) && TO_PC(tch)->IsChaotic())
		&& skill->m_proto->m_index != 511) ///=== 511, 스트레이아나 맵 스킬 제외
	{
		if (!IS_PC(ch))					return 0;
		if (!TO_PC(ch)->IsChaotic())	return 0;
	}
#endif // RESTRICT_PK

	if (!ch->CanVisible(tch))
	{
		// TODO : 인비저블을 해제하는 스킬 추가시 인덱스 검사하여, 해제 스킬은 사용가능토록
		if (proto->m_index != 146)
			return 0;
	}

	// 스킬 사용일 경우 스킬 이펙트 메시지 보냄
	if (proto)
	{
		CNetMsg sEffectMsg;
		EffectSkillMsg(sEffectMsg, tch, proto);
		tch->m_pArea->SendToCell(sEffectMsg, tch, true);
	}
	
	// 적용조건 검사
	if (!tch->CanApplySkill(proto, levelproto))
		return 0;

	// ch와 tch가 틀릴때
	if (ch != tch)
	{
		// hate 적용
		// not help는 타겟에게 바로 적용
		if (bNotHelp)
		{

			if (IS_PC(ch) && IS_PC(tch))
			{
				CPC* pc = TO_PC(ch);
				CPC* tpc = TO_PC(tch);
				if (!pc->CanPvP(tpc, true))
					return 0;
			}
			ApplyHate(ch, tch, levelproto->m_hate, true);
		}
		// help는 타겟의 attack list의 대상들에게 적용
		else if (bHelp)
		{
			// ach : tch가 공격한 리스트
			CAttackChar* ach = tch->m_attackList;
			while (ach)
			{
				ApplyHate(ch, ach->ch, levelproto->m_hate, true);
				ach = ach->m_next;
			}
		}
	}

	bool bHit[MAX_SKILL_MAGIC] = {false, false, false};
	for (i = 0; i < MAX_SKILL_MAGIC; i++)
	{
		const CMagicProto* magic = levelproto->m_magic[i];
		if (magic == NULL)
			continue ;
		const CMagicLevelProto* magiclevel = magic->Level(levelproto->m_magicLevel[i]);
		if (magiclevel == NULL)
			continue ;

		if (itemidx < 0)
#ifdef ENABLE_ROGUE_SKILL125_BRZ
			bHit[i] = (SelectHitType(ch, tch, MSG_DAMAGE_MAGIC, HITTYPE_MISS | HITTYPE_NORMAL, magic, magiclevel, (skill->m_proto->m_flag & SF_NOTHELP) ? true : false, skill->m_level) != HITTYPE_MISS);
#else
			bHit[i] = (SelectHitType(ch, tch, MSG_DAMAGE_MAGIC, HITTYPE_MISS | HITTYPE_NORMAL, magic, magiclevel, (skill->m_proto->m_flag & SF_NOTHELP) ? true : false) != HITTYPE_MISS);
#endif // ENABLE_ROGUE_SKILL125_BRZ
		else
			bHit[i] = true;

		int retAttack = 0;
		switch (magic->m_type)
		{
		case MT_ATTACK:
			// 공격은 무조건 정상 적용으로
			bApply = true;

			retAttack = ProcAttack(ch, tch, ch->GetAttackType(proto), skill, i);
			if (retAttack)
				return retAttack;
			break;

		case MT_ASSIST:
			switch (magic->m_subtype)
			{
			case MST_ASSIST_DAMAGELINK:
				if (ch == tch)
					return 0;
				break;
			}
			// 트루시잉 코덱스와 상급 트루시잉 코덱스
			if (itemidx == 677 || itemidx == 3579)
				ApplyMagic(ch, tch, skill, i, true, bApply);
			else
				ApplyMagic(ch, tch, skill, i, false, bApply);
			break;

		case MT_STAT:
		case MT_ATTRIBUTE:
		case MT_RECOVER:
		case MT_CURE:
		case MT_OTHER:
			// 트루시잉 코덱스와 상급 트루시잉 코덱스
			if (itemidx == 677 || itemidx == 3579)
				ApplyMagic(ch, tch, skill, i, true, bApply);
			else
				ApplyMagic(ch, tch, skill, i, false, bApply);
			break;
		}
	}

	int nBlessAdd = 0;
	int nBlessRate = 0;
	if (bHelp && itemidx < 1 && (tch->m_assist.m_state & AST_BLESS))
	{
		nBlessAdd = tch->m_assist.m_avAddition.bless;
		nBlessRate = tch->m_assist.m_avRate.bless;
	}

	// 효과적용 : assist에서 durtime이 있는 스킬은 추가되고 없으면 안되니까 무조건 추가호출
#ifdef ENABLE_WAR_CASTLE
	// 수호탑, 권좌에는 버프 안걸림
	bool bCastleObject = false;
	if (IS_NPC(tch))
	{
		CNPC* npc = TO_NPC(tch);
		if (npc->m_proto->CheckFlag(NPC_CASTLE_TOWER | NPC_LORD_SYMBOL))
			bCastleObject = true;
	}
	if (!bCastleObject)
	{
#ifdef ENABLE_ROGUE_SKILL125_BRZ
		int remain = -1;
		if(skill->m_proto->m_index == 125)
		{
			int diffLevel = tch->m_level - ch->m_level;
			if(diffLevel > 5)			remain = 80;
			else if(diffLevel > 2)		remain = 100;
		}
		if (tch->m_assist.Add(ch, itemidx, proto, skill->m_level, bHit, true, remain, CalcSkillParam(ch, tch, proto->m_selfparam, proto->m_targetparam), nBlessAdd, nBlessRate))
			bApply = true;
#else
		if (tch->m_assist.Add(ch, itemidx, proto, skill->m_level, bHit, true, -1, CalcSkillParam(ch, tch, proto->m_selfparam, proto->m_targetparam), nBlessAdd, nBlessRate))
			bApply = true;
#endif // ENABLE_ROGUE_SKILL125_BRZ
	}
#else
	if (tch->m_assist.Add(ch, itemidx, proto, skill->m_level, bHit, true, -1, CalcSkillParam(ch, tch, proto->m_selfparam, proto->m_targetparam), nBlessAdd, nBlessRate))
		bApply = true;
#endif

	if (bApply && (nBlessAdd || nBlessRate))
	{
		tch->m_assist.CancelBless();
	}

	if (!ch->CanVisible(tch) && (proto->m_flag & SF_NOTHELP))
		tch->CancelInvisible();

	return 0;
}



/////////////////////
// Function name	: ApplySkillParty
// Description	    : 파티에 적용 시전자 포함
// Argument         : CPC* ch
//                  : 시전자
// Argument         : CSkill* skill
//                  : 스킬
// Argument         : int itemidx
//                  : 아이템에 의한 스킬일 경우 아이템 인덱스, 그외는 -1
// Argument         : bool& bApply
//                  : 스킬이 정상적으로 적용되면 true, 아니면 false
void ApplySkillParty(CPC* ch, CSkill* skill, int itemidx, bool& bApply)
{
	if (ch->m_pArea == NULL || ch->m_pZone == NULL)
		return ;

	if (!ch->IsParty())
	{
		ApplySkill(ch, ch, skill, itemidx, bApply);
		return ;
	}

	bool bApplyParty = false;
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if (ch->m_party->GetNearMember(ch, i))
		{
			int retApplySkill = ApplySkill(ch, ch->m_party->GetNearMember(ch, i), skill, itemidx, bApply);
			// 파티원 중 한명에게라도 bApply가 true가 되면 반환되는 bApply도 true로 한다.
			if (bApply)
				bApplyParty = true;
			if (retApplySkill == -1)
			{
				bApply = bApplyParty;
				return ;
			}
		}
	}

	bApply = bApplyParty;
}


////////////////////
// Function name	: CalcSkillParam
// Description	    : ch가 tch에세 스킬 사용시, 파리미터의 최종 값(1/100단위)를 구한다
int CalcSkillParam(const CCharacter* ch, const CCharacter* tch, int selfparam, int targetparam)
{
	if (!IS_PC(ch) || !IS_PC(tch))
		return 100;

	int sparam = 0;
	int tparam = 0;

	if (ch)
	{
		switch (selfparam)
		{
		case SPARAM_STR:	sparam = ch->m_str;		break;
		case SPARAM_DEX:	sparam = ch->m_dex;		break;
		case SPARAM_INT:	sparam = ch->m_int;		break;
		case SPARAM_CON:	sparam = ch->m_con;		break;
		}
	}
	if (tch)
	{
		switch (targetparam)
		{
		case SPARAM_STR:	tparam = tch->m_str;	break;
		case SPARAM_DEX:	tparam = tch->m_dex;	break;
		case SPARAM_INT:	tparam = tch->m_int;	break;
		case SPARAM_CON:	tparam = tch->m_con;	break;
		}
	}

	int param = 100 + (sparam - tparam) / 2;
	if (param < SKILL_PARAM_MIN)
		return SKILL_PARAM_MIN;
	if (param > SKILL_PARAM_MAX)
		return SKILL_PARAM_MAX;
	return param;
}
