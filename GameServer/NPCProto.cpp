#include "stdhdrs.h"
#include "Log.h"
#include "NPCProto.h"
#include "Server.h"
#include "DBCmd.h"

///////////////////
// ANPCPtoro member

CNPCProto::CNPCProto()
: m_name(MAX_CHAR_NAME_LENGTH + 1)
#ifdef MONSTER_RAID_SYSTEM
, m_listRaidDrop(Comp_RAIDDROPDATA)
#endif // MONSTER_RAID_SYSTEM
{
	m_index = -1;
	m_level = 0;
	m_exp = 0;
	m_price = 0;
	m_sight = 0;
	m_walkSpeed = 0.0f;
	m_runSpeed = 0.0f;
	m_size = 0.0f;
	m_hp = 0;
	m_mp = 0;
	m_attack = 0;
	m_magic = 0;
	m_defense = 0;
	m_resist = 0;
	m_flag = 0;
	m_recoverHP = 0;
	m_recoverMP = 0;
	m_family = -1;
	memset(m_item, -1, sizeof(int) * MAX_NPC_DROPITEM);
	memset(m_itemPercent, 0, sizeof(int) * MAX_NPC_DROPITEM);
	memset(m_product, -1, sizeof(int) * MAX_NPC_PRODUCT);


// 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기
	m_minplus = 0;			// 최소 플러스 수치
	m_maxplus = 0;			// 최대 플러스 수치
	m_probplus = 0;			// 플러스 붙을 확률
// --- 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기

	m_itemCount = 0;

	m_attackSpeed = 0;
	m_attackArea = 0;

	m_nSkill = 0;
	memset(m_skillIndex, 0, sizeof(int) * MAX_NPC_SKILL);
	memset(m_skillLevel, 0, sizeof(int) * MAX_NPC_SKILL);
	memset(m_skillProb, 0, sizeof(int) * MAX_NPC_SKILL);

	m_moveArea = 0;
	
	m_aitype = NPC_AI_NORMAL;
	m_aiflag = 0;
	m_aileader_flag = 0;
	m_aileader_idx = 0;
	m_aileader_count = 0;
	
#ifdef ADULT_SERVER_NEW_BALANCE
	m_nHit = 0;
	m_nDodge = 0;
	m_nMagicAvoid = 0;
#endif // ADULT_SERVER_NEW_BALANCE
}

CNPCProto::~CNPCProto()
{
}



///////////////////////
// CNPCProtoList member

CNPCProtoList::CNPCProtoList()
{
	m_npcProtoList = NULL;
	m_nCount = 0;
	m_virtualIndex = 1;
}

CNPCProtoList::~CNPCProtoList()
{
	if (m_npcProtoList)
		delete [] m_npcProtoList;
	m_npcProtoList = NULL;
	m_nCount = 0;
	m_virtualIndex = 1;
}

bool CNPCProtoList::Load()
{
	if (m_npcProtoList)
		delete [] m_npcProtoList;
	m_npcProtoList = NULL;
	m_nCount = 0;
	m_virtualIndex = 1;

	CDBCmd dbNpc;
	dbNpc.Init(&gserver.m_dbdata);
	strcpy(g_buf, "SELECT * FROM t_npc WHERE a_enable=1 ORDER BY a_index");
	dbNpc.SetQuery(g_buf);
	dbNpc.Open();
	m_nCount = dbNpc.m_nrecords;
	m_npcProtoList = new CNPCProto[m_nCount];

	int i = 0;
	int j;
	float scale = 1;
	*g_buf2 = '\0';		// for field
	*g_buf3 = '\0';		// for convert

	while (dbNpc.MoveNext())
	{
		int npcIndex = 0;
		int npcflag  = 0;
		int npcflag1 = 0;		//확장 플래그(추가)

		dbNpc.GetRec("a_index",			npcIndex);
		dbNpc.GetRec("a_flag",			npcflag);
		dbNpc.GetRec("a_flag1",			npcflag1);		//확장 플래그(추가)
#ifdef LC_TWN2
		// 대만은 수집가 릴 (178) 안나옴
		if (npcIndex == 178)
			continue ;
#endif // LC_TWN2

#ifdef MONSTER_RAID_SYSTEM
#else // MONSTER_RAID_SYSTEM
		// MONSTER_RAID_SYSTEM가 아니면 raid 몹 무시
		if ((npcflag & NPC_RAID) != 0)
			continue ;
#endif // MONSTER_RAID_SYSTEM

#ifdef EVENT_XMAS_2006
#else
		if (npcIndex == 334 || npcIndex == 335 || npcIndex == 336)
			continue ;
#endif // EVENT_XMAS_2006

		m_npcProtoList[i].m_index = npcIndex;
		m_npcProtoList[i].m_flag  = npcflag;
		m_npcProtoList[i].m_flag1 = npcflag1;		//확장 플래그(추가)
		dbNpc.GetRec(NPC_NAME_FIELD,	m_npcProtoList[i].m_name, true);
		dbNpc.GetRec("a_level",			m_npcProtoList[i].m_level);
		dbNpc.GetRec("a_exp",			m_npcProtoList[i].m_exp);
		dbNpc.GetRec("a_prize",			m_npcProtoList[i].m_price);
		dbNpc.GetRec("a_sight",			m_npcProtoList[i].m_sight);
		dbNpc.GetRec("a_skill_point",	m_npcProtoList[i].m_skillPoint);
		dbNpc.GetRec("a_walk_speed",	m_npcProtoList[i].m_walkSpeed);
		dbNpc.GetRec("a_run_speed",		m_npcProtoList[i].m_runSpeed);
		dbNpc.GetRec("a_size",			m_npcProtoList[i].m_size);
		dbNpc.GetRec("a_recover_hp",	m_npcProtoList[i].m_recoverHP);
		dbNpc.GetRec("a_recover_mp",	m_npcProtoList[i].m_recoverMP);
		dbNpc.GetRec("a_family",		m_npcProtoList[i].m_family);
#ifdef ADULT_SERVER_NEW_BALANCE
		dbNpc.GetRec("a_hit",			m_npcProtoList[i].m_nHit);
		dbNpc.GetRec("a_dodge",			m_npcProtoList[i].m_nDodge);
		dbNpc.GetRec("a_magicavoid",	m_npcProtoList[i].m_nMagicAvoid);
#endif // ADULT_SERVER_NEW_BALANCE

#ifdef EXTREME_CUBE
		dbNpc.GetRec("a_job_attribute",	m_npcProtoList[i].m_jobAttr);
#endif // EXTREME_CUBE

		// 공성 체널이 아니고  index == 468  이면 &= ~NPC_WARCASTLE
		if( gserver.m_subno != WAR_CASTLE_SUBNUMBER_MERAC && npcIndex==468 )
		{
			m_npcProtoList[i].m_flag = m_npcProtoList[i].m_flag & (~NPC_WARCASTLE);			
		}
		
#ifdef HANARO_EVENT
		if( gserver.m_subno != 3 && npcIndex == 893 )		// 초보지원 NPC 3체널만 
		{
			continue;
		}
#endif //HANARO_EVENT

		// 공성 관련 NPC는 경험치, 나스, 스킬포인트 없음
		if (m_npcProtoList[i].m_flag & (NPC_CASTLE_GUARD | NPC_CASTLE_TOWER))
		{
			m_npcProtoList[i].m_price = 0;
			m_npcProtoList[i].m_exp = 0;
			m_npcProtoList[i].m_skillPoint = 0;
		}

		CItemProto* itemproto = NULL;
		// 공성 관련 npc는 아이템 드롭 없음
		m_npcProtoList[i].m_itemCount = 0;
		int k = 0;
		for(j = 0; j < MAX_NPC_DROPITEM && !(m_npcProtoList[i].m_flag & (NPC_CASTLE_GUARD | NPC_CASTLE_TOWER)); j++)
		{
			strcpy(g_buf, "a_item_");			IntCat(g_buf, j, false);
			dbNpc.GetRec(g_buf,		m_npcProtoList[i].m_item[j-k]);

			if (m_npcProtoList[i].m_item[j-k] == -1 )
				break;

			itemproto = gserver.m_itemProtoList.FindIndex(m_npcProtoList[i].m_item[j-k]);
#ifdef CREATE_SORCERER
#else
			if( itemproto )
			{
				if( (itemproto->m_jobFlag & ( 1 << 5 ) ) && (itemproto->m_typeIdx == 0|| itemproto->m_typeIdx == 1) )
				{
					m_npcProtoList[i].m_item[j-k] = -1;
					m_npcProtoList[i].m_itemPercent[j-k] = 0;
					k++;
					continue;
				}
			}
#endif
			strcpy(g_buf, "a_item_percent_");	IntCat(g_buf, j, false);
			dbNpc.GetRec(g_buf,		m_npcProtoList[i].m_itemPercent[j-k]);

			// 일본 50레벨 이상 무기, 방어구는 드롭확률 1/2로 
#if defined (LC_JPN) || defined (LC_TLD)
			if( itemproto )
			{
				if( itemproto->GetItemProtoLevel() >= 50 && (itemproto->m_typeIdx == 0|| itemproto->m_typeIdx == 1) )
				{
					m_npcProtoList[i].m_itemPercent[j-k] /= 2;
				}
			}
#endif
			m_npcProtoList[i].m_itemCount++;
		}

		dbNpc.GetRec("a_move_area",		m_npcProtoList[i].m_moveArea);
		dbNpc.GetRec("a_attack_area",	m_npcProtoList[i].m_attackArea);
		dbNpc.GetRec("a_str",			m_npcProtoList[i].m_str);
		dbNpc.GetRec("a_dex",			m_npcProtoList[i].m_dex);
		dbNpc.GetRec("a_int",			m_npcProtoList[i].m_int);
		dbNpc.GetRec("a_con",			m_npcProtoList[i].m_con);
		dbNpc.GetRec("a_hp",			m_npcProtoList[i].m_hp);
		dbNpc.GetRec("a_mp",			m_npcProtoList[i].m_mp);
		dbNpc.GetRec("a_attackSpeed",	m_npcProtoList[i].m_attackSpeed);
		dbNpc.GetRec("a_attackType",	m_npcProtoList[i].m_attackType);
		dbNpc.GetRec("a_attack",		m_npcProtoList[i].m_attack);
		dbNpc.GetRec("a_magic",			m_npcProtoList[i].m_magic);
		dbNpc.GetRec("a_defense",		m_npcProtoList[i].m_defense);
		dbNpc.GetRec("a_resist",		m_npcProtoList[i].m_resist);
		dbNpc.GetRec("a_attacklevel",	m_npcProtoList[i].m_attacklevel);
		dbNpc.GetRec("a_defenselevel",	m_npcProtoList[i].m_defenselevel);


// 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기
		dbNpc.GetRec("a_minplus",		m_npcProtoList[i].m_minplus);
		dbNpc.GetRec("a_maxplus",		m_npcProtoList[i].m_maxplus);
		dbNpc.GetRec("a_probplus",		m_npcProtoList[i].m_probplus);
// --- 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기

		// NPC AI
		dbNpc.GetRec("a_aitype",		m_npcProtoList[i].m_aitype);
		dbNpc.GetRec("a_aiflag",		m_npcProtoList[i].m_aiflag);
		dbNpc.GetRec("a_aileader_flag",	m_npcProtoList[i].m_aileader_flag);
		dbNpc.GetRec("a_aileader_idx",	m_npcProtoList[i].m_aileader_idx);
		dbNpc.GetRec("a_aileader_count",m_npcProtoList[i].m_aileader_count);

		// 크기조정 : 스케일에 맞춰서
		dbNpc.GetRec("a_scale",			scale);
		m_npcProtoList[i].m_size *= scale;
		m_npcProtoList[i].m_attackArea *= scale;
		// ---

		// 테스트 서버용 경험치, Nas 조정

#ifdef MAL_TEST_SERVER
		m_npcProtoList[i].m_exp *= 3;
#endif
#ifdef TEST_SERVER
//		m_npcProtoList[i].m_price *= 3;
//		m_npcProtoList[i].m_exp *= 3;
//		m_npcProtoList[i].m_skillPoint *= 3;
#endif

#ifdef EVENT_FLOWERTREE_2007
		if ( m_npcProtoList[i].m_index == 342 ) // 꽃나무
			m_npcProtoList[i].m_hp = 0 ;
#endif // EVENT_FLOWERTREE_2007


// 050311 : bs : 대만/중국에서 바알은 체력 회복이 3배
#if defined(LC_TWN) || defined(LC_CHN)
		if (m_npcProtoList[i].m_index == 152)
		{
			m_npcProtoList[i].m_recoverHP *= 3;
		}
#endif
// --- 050311 : bs : 대만/중국에서 바알은 체력 회복이 3배

		for (j=0; j < MAX_NPC_PRODUCT; j++)
		{
			strcpy(g_buf, "a_product");			IntCat(g_buf, j, false);
			dbNpc.GetRec(g_buf,			m_npcProtoList[i].m_product[j]);
		}

		// 스킬 입력
		CLCString npcskill(256);
		for (j = 0; j < MAX_NPC_SKILL; j++)
		{
			sprintf(g_buf, "a_skill%d", j);
			if (dbNpc.GetRec(g_buf, npcskill))
			{
				const char* p = npcskill;
				p = AnyOneArg(p, g_buf);
				if (strlen(g_buf) && atoi(g_buf) > 0)
				{
					m_npcProtoList[i].m_skillIndex[j] = atoi(g_buf);
					p = AnyOneArg(p, g_buf);
					m_npcProtoList[i].m_skillLevel[j] = atoi(g_buf);
					p = AnyOneArg(p, g_buf);
					m_npcProtoList[i].m_skillProb[j] = atoi(g_buf);
				}
				else
				{
					m_npcProtoList[i].m_skillIndex[j] = -1;
					m_npcProtoList[i].m_skillLevel[j] = 0;
					m_npcProtoList[i].m_skillProb[j] = 0;
				}
			}
		}

		i++;
	}

	m_nCount = i;

#ifdef MONSTER_RAID_SYSTEM
	for (i = 0; i < m_nCount; i++)
	{
		sprintf(g_buf, "SELECT * FROM t_npc_dropraid WHERE a_npc_index=%d ORDER BY a_item_index", m_npcProtoList[i].m_index);
		dbNpc.SetQuery(g_buf);
		if (!dbNpc.Open())
			return false;
		RAIDDROPDATA rdd;
		while (dbNpc.MoveNext())
		{
			dbNpc.GetRec("a_item_index", rdd.itemindex);
			dbNpc.GetRec("a_flag", rdd.flag);
			dbNpc.GetRec("a_count", rdd.count);
			dbNpc.GetRec("a_prob", rdd.prob);		
			
#ifdef EXPEDITION_RAID
			dbNpc.GetRec("a_spec_item_index1"	, rdd.spec_itemindex1);
			dbNpc.GetRec("a_spec_item_index2"	, rdd.spec_itemindex2);
			dbNpc.GetRec("a_spec_item_index3"	, rdd.spec_itemindex3);
			dbNpc.GetRec("a_spec_item_index4"	, rdd.spec_itemindex4);
			dbNpc.GetRec("a_spec_item_index5"	, rdd.spec_itemindex5);
			dbNpc.GetRec("a_spec_item_index6"	, rdd.spec_itemindex6);
			dbNpc.GetRec("a_spec_item_index7"	, rdd.spec_itemindex7);
			dbNpc.GetRec("a_spec_item_index8"	, rdd.spec_itemindex8);
			dbNpc.GetRec("a_spec_item_index9"	, rdd.spec_itemindex9);
			dbNpc.GetRec("a_spec_item_index10"	, rdd.spec_itemindex10);
			dbNpc.GetRec("a_spec_item_index11"	, rdd.spec_itemindex11);
			dbNpc.GetRec("a_spec_item_index12"	, rdd.spec_itemindex12);
			dbNpc.GetRec("a_spec_item_index13"	, rdd.spec_itemindex13);
			dbNpc.GetRec("a_spec_item_index14"	, rdd.spec_itemindex14);
			dbNpc.GetRec("a_spec_min"			, rdd.spec_Min);
			dbNpc.GetRec("a_spec_max"			, rdd.spec_Max);
			dbNpc.GetRec("a_spec_count"			, rdd.spec_count);			
			dbNpc.GetRec("a_spec_prob"			, rdd.spec_prob);
			dbNpc.GetRec("a_spec_flag"			, rdd.spec_flag);
#endif //EXPEDITION_RAID

			m_npcProtoList[i].m_listRaidDrop.AddToTail(rdd);
		}
	}
#endif // MONSTER_RAID_SYSTEM

	return true;
}

CNPC* CNPCProtoList::Create(int npcindex, CNPCRegenInfo* regenInfo)
{
	CNPCProto* proto = FindProto(npcindex);
	if (!proto)
			{
		GAMELOG << init("SYS_ERR")
				<< "Can't Create NPC Num" << delim
				<< npcindex
				<< end;
		return NULL;
	}

	CNPC* npc = new CNPC;
			
	npc->m_regenInfo = regenInfo;

	// CNPC 속성
	npc->m_proto		= proto;
	npc->m_idNum		= proto->m_index;
	npc->m_moveArea		= proto->m_moveArea;
	npc->m_flag			= proto->m_flag;

	npc->ResetStat();

	// CCharacter 속성
	npc->m_index		= m_virtualIndex++;
	npc->m_name			= proto->m_name;
	npc->m_level		= proto->m_level;
	npc->m_exp			= proto->m_exp;
	npc->InitPointsToMax();		// 초기 최대 HP, MP 셋팅
	npc->m_skillPoint	= proto->m_skillPoint;

	npc->m_moveDir		= (GetRandom(0, 1) == 0) ? -1 : 1;

	// 퀘스트 NPC일 경우
	if (npc->m_proto->m_flag & NPC_QUEST)
	{
		int i;
		for (i=0; i < gserver.m_questProtoList.m_nCount; i++)
		{
			CQuestProto* questProto = gserver.m_questProtoList.m_proto + i;

			if (!questProto)
				continue;

			if (questProto->m_startType == QSTART_NPC  && questProto->m_startData == npc->m_idNum)
			{
				npc->m_quest[npc->m_nQuestCount] = questProto->m_index;
				npc->m_nQuestCount++;
			}
		}
	}

	// 050221 : bs : NPC 스킬 생성
	int i;
	for (i = 0; i < MAX_NPC_SKILL; i++)
	{
		if (proto->m_skillIndex[i] > 0)
		{
			npc->m_skills[i] = gserver.m_skillProtoList.Create(proto->m_skillIndex[i], proto->m_skillLevel[i]);
		}
	}
	// --- 050221 : bs : NPC 스킬 생성

	return npc;

}

// 리스트에서 찾기
CNPCProto* CNPCProtoList::FindProto(int npc_idx)
{
	CNPCProto proto;
	proto.m_index = npc_idx;

	return (CNPCProto*)bsearch(&proto, m_npcProtoList, m_nCount, sizeof(CNPCProto), CompIndex);
}
