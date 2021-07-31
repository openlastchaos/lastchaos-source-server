/************************************
  수정사항 
  수정한 사람: bs
  수정일: 2004-11-18 오후 3:42:52
  설명: PC의 보조 효과 적용 및 레벨업 버그 수정
 ************************************/

#include "stdhdrs.h"
#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "Exp.h"
#include "NetMsg.h"
#include "doFunc.h"
#include "Guild.h"
#include "Battle.h"
#include "Friend.h"
#include "WarCastle.h"
#include "DBCmd.h"

// 순서대로 타이탄, 나이트, 힐러, 메이지, 로그, 소서러 1-20 레벨까지 기본 상승 수치
// 레벨업시 각 스탯 변화
// 스탯은 str, dex, int, con 순서
int levelup_stat[JOBCOUNT][4]	= {
	{3, 1, 1, 3},
	{2, 2, 2, 2},
	{1, 3, 2, 2},
	{1, 3, 3, 1},
	{3, 3, 1, 1},
	{1, 2, 3, 2},
#ifdef NIGHT_SHADOW
	{1, 3, 3, 2},
#endif //NIGHT_SHADOW
};
// 레벨업시 각 스탯당 hp, mp 증가량
int levelup_hp[JOBCOUNT][4]		= {
	{4, 0, 0, 8},
	{4, 0, 0,11},
	{4, 0, 0, 7},
	{3, 0, 0, 8},
	{5, 0, 0,12},
	{5, 0, 0, 5},
#ifdef NIGHT_SHADOW
	{3, 0, 0,11},
#endif //NIGHT_SHADOW
};

int levelup_mp[JOBCOUNT][4]		= {
	{0, 0, 8, 2},
	{0, 0, 6, 2},
	{0, 0, 9, 2},
	{0, 0,10, 1},
	{0, 0, 7, 2},
	{0, 0, 7, 4},
#ifdef NIGHT_SHADOW
	{0, 0, 8, 1},
#endif //NIGHT_SHADOW
};

// 기본 능력치
int base_stat[JOBCOUNT][4]		= {
	{50, 20, 20, 50},
	{38, 28, 28, 38},
	{20, 50, 38, 28},
	{20, 50, 50, 20},
	{38, 50, 20, 28},
	{20, 44, 44, 28},
#ifdef NIGHT_SHADOW
	{10, 50, 50, 30},
#endif //NIGHT_SHADOW
};

#ifdef NIGHT_SHADOW
int base_hp[JOBCOUNT]				= {180, 150, 110,  90, 130, 110, 120};
int base_mp[JOBCOUNT]				= { 68,  86, 108, 122,  56, 128, 120};
#else //NIGHT_SHADOW
int base_hp[JOBCOUNT]				= {180, 150, 110,  90, 130, 110};
int base_mp[JOBCOUNT]				= { 68,  86, 108, 122,  56, 128};
#endif //NIGHT_SHADOW

// 전직 기본 능력치
// 순서대로 하이랜더, 워마스터, 로열나이트, 템플나이트, 아처, 힐러, 위자드, 위치, 어쌔신, 레인저, 엘리멘탈리스트, 스페셜리스트, 나이트 쉐도우
int base_stat2[JOB2COUNT][4]		= {
	{167,  79,  79, 167},
	{167,  79,  79, 167},
	{126, 116, 116, 126},
	{126, 116, 116, 126},
	{ 79, 167, 126, 116},
	{ 79, 167, 126, 116},
	{ 79, 167, 167,  79},
	{ 79, 167, 167,  79},
	{155, 167,  79,  87},
	{155, 167,  79,  87},
	{ 79, 132, 161, 116},
	{ 79, 132, 161, 116},
#ifdef NIGHT_SHADOW
	{ 69, 167, 167, 118},
#endif //NIGHT_SHADOW
};

#ifdef NIGHT_SHADOW
int base_hp2[JOB2COUNT] = {	1584, 1584,	1382, 1382, 962, 962, 739, 739,	1247, 1247,	1045, 1045, 1265 };
int base_mp2[JOB2COUNT]	= { 774, 774, 790, 790, 1076, 1076,	1351, 1351, 587, 587, 1299, 1299, 1134 };
#else
int base_hp2[JOB2COUNT] = {	1584, 1584,	1382, 1382, 962, 962, 739, 739,	1247, 1247,	1045, 1045 };
int base_mp2[JOB2COUNT]	= { 774, 774, 790, 790, 1076, 1076,	1351, 1351, 587, 587, 1299, 1299 };
#endif //NIGHT_SHADOW

int levelup_stat2[JOB2COUNT][4]		= {
	{3, 1, 1, 1},
	{2, 1, 1, 2},
	{2, 1, 2, 1},
	{2, 1, 1, 2},
	{2, 2, 1, 1},
	{2, 1, 2, 1},
	{1, 1, 3, 1},
	{1, 2, 2, 1},
	{1, 3, 1, 1},
	{2, 2, 1, 1},
	{1, 1, 2, 2},
	{1, 2, 2, 1},
#ifdef NIGHT_SHADOW
	{1, 3, 3, 2},
#endif //NIGHT_SHADOW
};

int levelup_hp2[JOB2COUNT]				= {
	10,
	10,
	4,
	4,
	2,
	2,
	1,
	1,
	3,
	3,
	2,
	3,
#ifdef NIGHT_SHADOW
	2,
#endif //NIGHT_SHADOW
};

int levelup_mp2[JOB2COUNT]				= {
	4,
	4,
	2,
	2,
	12,
	12,
	10,
	10,
	2,
	2,
	10,
	10,
#ifdef NIGHT_SHADOW
	11,
#endif //NIGHT_SHADOW
};

// 이동 속도
#ifdef NIGHT_SHADOW
float run_speed[JOBCOUNT]			= {6.0f, 6.0f, 7.0f, 6.0f, 6.5f, 7.0f, 6.5f};
float walk_speed[JOBCOUNT]			= {1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f};
#else //NIGHT_SHADOW
float run_speed[JOBCOUNT]			= {6.0f, 6.0f, 7.0f, 6.0f, 6.5f, 7.0f};
float walk_speed[JOBCOUNT]			= {1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f};
#endif // NIGHT_SHADOW

#ifdef ADULT_SERVER_NEW_BALANCE
// 기본 명중도 : 직업-(근접,원거리,마법)
int nBaseHitrate[JOBCOUNT][3]	= {
	{137, 137, 137},
	{130, 130, 130},
	{142, 142, 131},
	{142, 142, 142},
	{142, 142, 142},
	{137, 137, 137},
#ifdef NIGHT_SHADOW
	{142, 142, 142},
#endif //NIGHT_SHADOW
};
// 기본 회피도 : 직업-(근접,원거리,마법)
int nBaseAvoid[JOBCOUNT][3]	= {
	{20, 20, 20},
	{28, 28, 28},
	{50, 50, 38},
	{50, 50, 50},
	{50, 50, 20},
	{44, 44, 44},
#ifdef NIGHT_SHADOW
	{50, 50, 50},
#endif //NIGHT_SHADOW
};
// 기본 공격 타입
// 물리/마법, 위크/노멀/스트롱/하드/크리/데들리
int nBaseHitTypeProb[2][6] = {
	{ 50, 575, 125, 100,  90,  60},
	{ 50, 575, 125, 100,  90,  60},
};
#endif // ADULT_SERVER_NEW_BALANCE

/////////////
// CPC member
CPC::CPC()
: m_nick(MAX_CHAR_NAME_LENGTH + 1)
#ifdef MESSENGER_NEW
, m_listBlockPC(CBlockPC::CompByIndex)
#endif
, m_listAutoGive(NULL)
, m_listBloodBug(NULL)
#ifdef MONSTER_COMBO
, m_listSaveComboData(NULL)
#endif
#ifdef EX_GO_ZONE
, m_listRegisterNpc(NULL)
#endif
{
	m_type = MSG_CHAR_PC;
	
	m_billReqTime = 0;
	m_listIdx = -1;
	m_bPlaying = false;
	m_bChangeStatus = false;
	m_bImmortal = false;
	m_bChatMonitor = false;
	m_bLoadChar = false;
	m_speedup = 0;

#ifdef TRADE_AGENT
	m_bTradeAgentRegIng = false;	//등록 처리중인가(거래대행)
	m_bTradeAgentCalcIng = false;	//정산 처리중인가(거래대행)
	m_nCalcPageNo		 = 0;		//정산 현재 페이지 번호
#endif	// TRADE_AGENT

#ifdef GMTOOL
	m_bGmMonitor = false;
#endif // GMTOOL

#ifdef ENABLE_SINGLEDUNGEON_DATA
	m_pd3Count = 0;
	m_pd3Time = 0;
	m_pd4Count = 0;
	m_pd4Time = 0;
#endif
	m_recentAtt = MAPATT_BLOCK;

#ifdef RANKER_NOTICE
	m_bRanker = false;
#endif

	m_autoSkillTime = 0;
	m_SkillTime_511 = 0;
	
#ifdef FEEITEM
	m_cashBalance = 0;		// 보유 캐쉬
#endif // FEEITEM
	//0707
	m_secretkey = 0;

	/////////////////////////////////////////////
	// BANGWALL : 2005-06-27 오후 8:52:59
	// Comment : pd4time
	m_pd4StartTime = 0;

	m_loseexp = 0;
	
	m_job = -1;
#ifdef ENABLE_CHANGEJOB
	m_job2 = 0;
#endif
	m_hairstyle = 0;
	m_facestyle = 0;
	
	m_weight = 0;
	m_maxWeight = 0;
	m_admin = 0;

// 050401 : bs : plus에 따른 확률 변수 추가
	m_plusStrong = 0;
	m_plusHard = 0;
	m_plusWeak = 0;
// --- 050401 : bs : plus에 따른 확률 변수 추가
	
	m_statpt_remain = 0;
	m_statpt_str = 0;
	m_statpt_dex = 0;
	m_statpt_int = 0;
	m_statpt_con = 0;
	
	m_state = 0;
	m_pkPenalty = 0;
	m_pkCount = 0;
	m_pkRecoverPulse = 0;
	m_pkmodedelay = 0;
	m_pkPenaltyHP = 0;
	m_pkPenaltyMP = 0;
	m_pkRecoverNPCCount = 0;
	
	m_dbStr = 0;
	m_dbDex = 0;
	m_dbInt = 0;
	m_dbCon = 0;
	
	m_dbHP = 0;
	m_dbMP = 0;

#ifdef ENABLE_WAR_CASTLE
	m_opSturnLevel = 0;
	m_opBloodLevel = 0;
	m_opPoisonLevel = 0;
	m_opSlowLevel = 0;
	m_opMoveLevel = 0;

	m_opSturnIndex = 0;
	m_opBloodIndex = 0;
	m_opPoisonIndex = 0;
	m_opSlowIndex = 0;
	m_opMoveIndex = 0;

	m_bCreateMixItem = false;
#endif

	m_opIncreaseInven = 0;
	m_opMPSteal = 0;
	m_opHPSteal = 0;
	m_opAttackBlind = 0;
	m_opAttackPoison = 0;
	m_opAttackCritical = 0;
	m_opRecoverHP = 0;
	m_opRecoverMP = 0;
	m_opDecreaseSkillDelay = 0;
	m_opDecreaseSkillMP = 0;
	m_opResistStone = 0;
	m_opResistSturn = 0;
	m_opResistSilent = 0;
	m_opBlocking = 0;
	
	m_desc = NULL;
	
	m_lastupdate = PULSE_SAVE_PC;
	m_autochknum[0] = m_autochknum[1] = 0;
	m_silencePulse = gserver.m_pulse;
	m_lastProducePulse = 0;
	m_perSecondPulse = PULSE_REAL_SEC;

	m_hackProduceCount = PULSE_HACK_ATTACK_COUNT;	// 샌상메세지 온 수 저장
	m_ProduceClientTime = 0; // 클라이언트가 보낸 생산시간
	
	// INVENTORY INITIALIZE
	m_invenNormal.Init(this, INVENTORY_NORMAL);
	m_invenQuest.Init(this, INVENTORY_QUEST);
	m_invenEvent.Init(this, INVENTORY_EVENT);
	m_bChangeStash = false;
	
	m_moneyItem = NULL;
	
	memset(m_wearing, 0, sizeof(CItem*) * MAX_WEARING);
	
	m_exchange = NULL;
	m_personalshop = NULL;
	m_party = NULL;
	m_bGiveItem = false;

#ifdef EXPEDITION_RAID
	m_Exped				= NULL;
//	m_nExpedQuitMode	= MSG_EXPED_QUITMODE_UNUSUAL;
//	m_nLastCmdRunTime	= 0;
#endif
	
	m_recoverHPItemTime = 0;
	m_recoverMPItemTime = 0;
	m_recoverHPItemValue = 0;
	m_recoverMPItemValue = 0;
	
	m_reqWarpTime = 0;
	m_reqWarpType = -1;
	m_reqWarpData = -1;

	m_guildInfo = NULL;
	m_regGuild = 0;
	m_guildoutdate = 0;
#ifdef ENABLE_WAR_CASTLE
	m_joinMerac = WCJF_NONE;
#endif

	m_raList = NULL;

	m_changePulse = 0;
	m_changeIndex = -1;

	m_plusEffect = 0;

	memset(m_teachIdx, -1, sizeof(int) * TEACH_MAX_STUDENTS);
	memset(m_teachLevel, 0, sizeof(int) * TEACH_MAX_STUDENTS);
	memset(m_teachJob, -1, sizeof(char) * TEACH_MAX_STUDENTS);
#ifdef ENABLE_CHANGEJOB
	memset(m_teachJob2, 0, sizeof(char) * TEACH_MAX_STUDENTS);
#endif
	memset(m_teachName, 0, sizeof(m_teachName));
	memset(m_teachTime, 0, sizeof(int) * TEACH_MAX_STUDENTS);
	
	m_teachWait = -1;
	m_teachType = MSG_TEACH_NO_TYPE;
	m_bTeacher = false;

	m_fame = 0;
	//0627
	m_cntTeachingStudent = 0;
	m_cntCompleteStudent = 0;
	m_cntFailStudent = 0;
	m_superstone = 0;
	m_guardian   = 0;
	m_displayCanSstone = true;

	m_nMoonStoneSum = 0;
	m_nMoonStoneDigit = -1;

#ifdef EVENT_NEW_MOONSTONE
	m_nMSIndex	= 0;
	m_nMSGiftIndex	= 0;
#endif // EVENT_NEW_MOONSTONE

	m_memposTime = 0;

#ifdef CHARSLOTEXT_CARD
	memset(m_charslotTime, 0, sizeof(m_charslotTime));
#endif //#ifdef CHARSLOTEXT_CARD
	
	m_stashextTime = 0;

#ifdef ENABLE_MESSENGER
	m_nRegFriend = 0;
	m_nCondition = 0;
	m_Friend = NULL;
#endif

#ifdef ETC_EVENT
	m_etcEvent = 0;
#endif

#ifdef MARGADUM_PVP
	m_magadumDamage = 0;	// 마르가둠 pvp시 등수를 가리기 위한 점수
#endif


#ifdef ENABLE_PET
	m_petList = NULL;
	m_pulseLastAttackSkill = 0;
#ifdef NEW_PET_EXP_SYSTEM
#else // NEW_PET_EXP_SYSTEM
	m_bNotMiss = false;
#endif // NEW_PET_EXP_SYSTEM
#endif
	
#ifdef ATTACK_PET
	m_pApetlist = NULL;
#endif //ATTACK_PET

	m_pulseTakeEventXMas2005 = 0;
	m_nTimeEventNewYear2006 = 0;
	m_pulseEventNewYear2006 = 0;

#ifdef EVENT_SEARCHFRIEND
	m_nTimeEventSearchFriend = 0;
	m_pulseEventSearchFriend = 0;
	m_nGoodEventSearchFriend = 0;
	m_nEventSearchFriendListCount = 0;
	m_nGoodEventSearchFriendListGoodCount = 0;
	m_pulseEventSearchFriendList = 0;
	m_nGoodEventSearchFriendGoodNumber = 0;
	m_bGoodEventSearchFriendListImprove = true;
	m_bEventSearchFriendSelect = true;

	memset(m_nEventSearchFriendIndex, -1, sizeof(m_nEventSearchFriendIndex));
	memset(m_nEventSearchFriendNick, 0, sizeof(m_nEventSearchFriendNick));
	memset(m_nGoodEventSearchFriendIndex, -1, sizeof(m_nGoodEventSearchFriendIndex));
	memset(m_nGoodEventSearchFriendNick, 0, sizeof(m_nGoodEventSearchFriendNick));
	memset(m_nGoodEventSearchFriendGood, -1, sizeof(m_nGoodEventSearchFriendGood));
#endif // #ifdef EVENT_SEARCHFRIEND
	m_elementalList = NULL;
	memset(m_pulseElemental, -1, sizeof(m_pulseElemental));

	m_evocationType = EVOCATION_NONE;
	m_pulseEvocation[0] = 0;		// 헬운드 강신 남은 시간
	m_pulseEvocation[1] = 0;		// 엘레넨 강신 남은 시간

#ifdef NEW_DIVISION_EXPSP
	m_nExpDamage = 0;
#endif // #ifdef NEW_DIVISION_EXPSP

#ifdef EVENT_SEARCHFRIEND
	m_serverOld = 0;
#endif // #ifdef LC_KOR

	m_nGoldenBallNoticeStatus = GOLDENBALL_STATUS_NOTHING;

	m_bProcDisconnect = false;
#ifdef CASH_MOONSTONE
	m_cashmoonStoneItemIndex = -1;
	m_cashmoonStoneRemoveCtId = -1;
	m_cashMonnStoneGrade = -1;
	m_cashMoonStoneGiveCtId = -1;
#endif

	m_nLastCollectRequest = 0;

#ifdef RESTRICT_PK
	m_lastPKTime = -1;
#endif // RESTRICT_PK

#ifdef RESTRICT_PVP_SKILL
	m_nRestrictPvPSkillDelaySec = 0;
#endif // RESTRICT_PVP_SKILL

#ifdef EVENT_JPN_2007_NEWSERVER
	m_nEventJPN2007NewServerGift = 0;
#endif // EVENT_JPN_2007_NEWSERVER

#ifdef JPN_GPARA_PROMOTION
	m_nGparaPromotionData = MSG_CONN_GPARA_PROMOTION_COMMIT;
#endif // JPN_GPARA_PROMOTION
	
#ifdef JPN_OCN_GOO_PROMOTION
	m_nOCN_Goo_PromotionData = MSG_CONN_OCN_GOO_PROMOTION_COMMIT;				// 일본 OCN, Goo 이벤트
#endif // JPN_OCN_GOO_PROMOTION
	
#ifdef JPN_MSN_PROMOTION
	m_nMSNPromotionData = MSG_CONN_MSN_PROMOTION_COMMIT;
#endif // JPN_MSN_PROMOTION
	
#ifdef CREATE_EVENT
	m_nCreateEventData = MSG_CONN_CREATEEVENT_COMMIT;
#endif // CREATE_EVENT

#ifdef EVENT_GOMDORI_2007
	m_nEventGomdori2007Win = 0;
	m_nEventGomdori2007Game = 0;
	m_nEventGomdori2007FirstLose = 0;
	m_nEventGomdori2007FirstWin = 0;
	m_nEventGomdori2007Total = 0;
	m_bEventGomdori2007Start = false;
	m_bEventGomdori2007CanSelect = false;
#ifdef LC_BRZ
	m_bEventGomdori2007Drop = gserver.m_pulse;			// 브라질은 pulse 값 저장
#else
	m_bEventGomdori2007Drop = true;
#endif // LC_BRZ
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_INDEPENDENCE_DAY_2007_USA
	m_bEventIndependenceDay2007drop = true;
#endif // EVENT_INDEPENDENCE_DAY_2007_USA
#ifdef NEW_GUILD
	m_GuildExp	= 0;			// 길드포인트로 전환될 경험치
	m_GuildFame = 0;		// 길드 포인트로 전환될 명성치
#endif // NEW_GUILD
#ifdef DRATAN_CASTLE
	m_nRespondTime = 0;
#endif // DRATAN_CASTLE

#ifdef ALTERNATE_MERCHANT
	m_pAlternateMerchant = NULL;
#endif // ALTERNATE_MERCHANT

#ifdef CHAT_BLOCK_USA
	m_shoutPulse = gserver.m_pulse;
#endif // CHAT_BLOCK_USA

#ifdef LAKA_PRESSCORPS
	m_bPressCorps = 0;
#endif // LAKA_PRESSCORPS

//#ifdef IRIS_POINT
//	m_connect_min = 0;
//#endif // IRIS_POINT
	
#ifdef EVENT_PHOENIX// yhj   081105  피닉스 
	m_bPhoenix_Char = 2;
#endif // EVENT_PHOENIX
	
#ifdef NO_CHATTING
	m_nflag = 0;
#endif

#ifdef EX_GO_ZONE
	m_lastsavepos = 0;
#endif // EX_GO_ZONE

#ifdef NIGHTSHADOW_SKILL
	m_Owners_target = NULL;
	m_bTaming_npc = false;
	m_nSoul_Count = 0;
#endif // NIGHTSHADOW_SKILL

#ifdef FACTORY_SYSTEM
	memset(m_sealSkillExp, -1, sizeof(m_sealSkillExp));
#endif
}

CPC::~CPC()
{
	m_assist.ClearAssist(false, false, true, true, true);

#ifdef ENABLE_PET
	if (m_petList)
	{
		CPet* petNext = m_petList;
		CPet* pet;
		while ((pet = petNext))
		{
			petNext = pet->m_nextPet;
			DelAttackList(pet);
			delete pet;
		}
		m_petList = NULL;
	}
#endif

#ifdef ATTACK_PET
	if( m_pApetlist )
	{
		CAPet* next = m_pApetlist;
		CAPet* pTempPet;
		while( (pTempPet = next) )
		{
			next = pTempPet->m_pNextPet;
			delete pTempPet;
			pTempPet = NULL;
		}

		m_pApetlist = NULL;
	}
#endif // ATTACK_PET

	while (m_elementalList)
		UnsummonElemental(m_elementalList);
#ifdef ENABLE_MESSENGER
	if( m_Friend )
	{
		delete m_Friend;
		m_Friend = NULL;
	}
#endif
}


void CPC::SendStatus()
{
	CNetMsg msg;
	StatusMsg(msg, this);
	SEND_Q(msg, m_desc);
	m_bChangeStatus = false;
	
	// 파티에 속해 있으면 파티원에게도 보낸다
	if (IsParty())
	{
		PartyInfoMsg(msg, m_index, m_level, m_hp, m_maxHP, m_mp, m_maxMP, GET_X(this), GET_Z(this), GET_YLAYER(this), m_pZone->m_index);
		m_party->SendToPCInSameZone(m_pZone->m_index, m_pArea->m_index, msg);
	}
}

void CPC::SendElementalStatus()
{
	CNetMsg rmsg;
	// 소환수 상태 정보도
	CElemental* pElemental = m_elementalList;
	while (pElemental)
	{
		if (pElemental->m_bChangeStatus)
		{
			ExElementalStatusMsg(rmsg, pElemental);
			SEND_Q(rmsg, m_desc);
			pElemental->m_bChangeStatus = false;
		}
		pElemental = pElemental->m_nextElemental;
	}
}

void CPC::SendInventory(char resultArrange)
{
	CNetMsg msg;
	
	int i, j;
	int invenType[3] = {INVENTORY_NORMAL, INVENTORY_QUEST, INVENTORY_EVENT};
	
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < MAX_INVENTORY_ROWS; j++)
		{
			if (InventoryMsg(msg, this, resultArrange, invenType[i], j))
				SEND_Q(msg, m_desc);
		}
	}
}

void CPC::SendQuickSlot()
{
	CNetMsg msg;
	int i;
	
	for (i=0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		QuickSlotListMsg(msg, this, i);
		SEND_Q(msg, m_desc);
	}
}

void CPC::Send(CNetMsg& msg)
{
	if (!m_bPlaying)	return ;
	if (m_desc && msg.m_size)
	{
		SEND_Q(msg, m_desc);
		gserver.SendOutput(this);
	}
}

bool CPC::AddMoney(LONGLONG money)
{
	// 수량 변경이 없으면 바로 리턴
	if (money == 0)
		return true;
	
	// 줄어들면
	else if (money < 0)
	{
		// 기존에 돈이 있어야
		if (!m_moneyItem)
			return false;
		
		// 돈 줄이고
		DecreaseFromInventory(this, m_moneyItem, -money);
		
		// 0되면 인벤에서 제거
		if (m_moneyItem->Count() < 1)
		{
			RemoveFromInventory(this, m_moneyItem, true, true);
			m_moneyItem = NULL;
		}
	}
	
	// 늘어나면
	else
	{
		// 새로 만들어서
		CItem* item = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_moneyItem->m_index, -1, 0, 0, money);
		if (!item)
			return false;
		
		// 인벤에 넣고
		if (!AddToInventory(this, item, false, true))
		{
			delete item;
			return false;
		}
		
		// 돈 포인터 설정
		
		// 겹치면 생성한 건 버리기
		if (item->tab() < 0)
		{
			// 방어코드
			if (!m_moneyItem)
			{
				int r, c;
				if (m_invenNormal.FindItem(&r, &c, gserver.m_itemProtoList.m_moneyItem->m_index, 0, 0))
					m_moneyItem = m_invenNormal.GetItem(r, c);
			}
			// 방어코드
			
			delete item;
		}
		
		// 안 겹치면 생성된게 돈으로 설정
		else
			m_moneyItem = item;
	}
	
	return true;
}

bool CPC::AddExpSP(LONGLONG exp, int sp, bool bUseEvent)
{

#ifdef JP_OTAKU_SYSTEM
	if( this->m_etcEvent & OTAKU_SYSTEM )
	{
		return false;
	}
#endif // JP_OTAKU_SYSTEM
	CNetMsg msg;

	if (exp < 0 || sp < 0)
	{
		exp = 0;
		sp = 0;
	}

//#ifdef LIMIT_EXP_INCREASE
	// 경험치 증가 제한
	LONGLONG nOrgExp = exp;
	// SP 증가 제한
	LONGLONG nOrgSP = sp;
//#endif // LIMIT_EXP_INCREASE
	
#ifdef EVENT_PCBANG_2ND	// 넷띠모 피씨방 경험치 150%
 	if(m_desc->m_location == BILL_LOCATION_PCBANG)
 	{
 		exp = exp * 130 / 100;
 	}
#endif // EVENT_PCBANG_2ND

	if (bUseEvent)
	{
#ifdef EVENT_DOUBLE
		// 더블 이벤트
		if (gserver.m_bDoubleEvent)
#ifdef NEW_DOUBLE_GM
		{
#ifdef NEW_DOUBLE_EVENT_AUTO
			if(!gserver.m_bDoubleEventAutoOn)
#endif // NEW_DOUBLE_EVENT_AUTO
			{
#ifdef NEW_DOUBLE_GM_ZONE
				if(gserver.m_bDoubleEventZone == -1 || (gserver.m_bDoubleEventZone != -1 && gserver.m_bDoubleEventZone == m_pZone->m_index) )
#endif // NEW_DOUBLE_GM_ZONE
				{
					exp = exp * gserver.m_bDoubleExpPercent / 100;
					sp = sp * gserver.m_bDoubleSpPercent / 100;
				}
			}
		}
#else
		{
			exp *= 2;
			sp *= 2;
		}
#endif
#endif

#ifdef NEW_DOUBLE_EVENT_AUTO
		if(gserver.m_bDoubleEventAutoOn && m_pZone->m_index == ZONE_COMBO_DUNGEON)
		{
			int oldexp, oldsp;
			oldexp = exp;
			oldsp = sp;

			exp *= 2;
			sp *= 2;

			if(m_admin == 10)
			{
				GAMELOG << init("DoubleEventAuto", this)
						<< "oldexp" << delim 
						<< oldexp << delim
						<< "oldsp" << delim 
						<< oldsp << delim
						<< "exp" << delim 
						<< exp << delim
						<< "sp" << delim 
						<< sp << end;
			}
		}
#endif // NEW_DOUBLE_EVENT_AUTO

		// 경험치 더블 이벤트
		if (gserver.m_bDoubleExpEvent)
		{
			exp = (exp * gserver.m_expPercent ) / 100;
		}

		// 떡국
		if (m_assist.m_avAddition.hcExpPlus)
			exp = exp * NEWYEAR_EVENT_RICESOUP_UP_EXP / 10;

		// 떡만두국
		if (m_assist.m_avAddition.hcSPPlus)
			sp = sp * NEWYEAR_EVENT_RICEMANDO_UP_SP / 10;

		// 수박
		if (m_assist.m_avAddition.hcExpPlus_836)
			exp = exp * 3 / 2;

		// 참외
		if (m_assist.m_avAddition.hcSPPlus_837)
			sp = sp * 2;

		// 경험의스크롤
		if (m_assist.m_avAddition.hcScrollExp)
			exp = exp * 3 / 2;

		// 노력의스크롤
		if (m_assist.m_avAddition.hcScrollSP)
			sp = sp * 3 / 2;

#ifdef EVENT_SEARCHFRIEND
		if((m_bEventSearchFriendSelect == true)&&(m_nEventSearchFriendListCount > 0)&&(m_nTimeEventSearchFriend <= 216000)) //60시간 
			exp = exp * 2;	
#endif //#ifdef EVENT_SEARCHFRIEND

		// 060227 : bs : 유료 경험치 향상제
		if (m_assist.m_avRate.hcCashExpUp > 0)
			exp += exp * m_assist.m_avRate.hcCashExpUp / SKILL_RATE_UNIT;

		// 경험치 포션 : 1.5배
		if (m_assist.m_avAddition.hcExpPlus_S354)
			exp = exp * 3 / 2;

		// 추천서버포션 경험치 상승
		if (m_assist.m_avAddition.hcExpPlus_S355)
			exp = exp * 3 / 2;

		// 추천서버포션 SP 상승
		if (m_assist.m_avAddition.hcSPPlus_S356)
			sp = sp * 3 / 2;

		// 경험치 상승 % 단위 누적 적용
		if (m_assist.m_avAddition.hcExpPlusPer100 > 0)
			exp += exp * m_assist.m_avAddition.hcExpPlusPer100 / 100;

		// 숙련도 상승 %단위 누적 적용
		if (m_assist.m_avAddition.hcSPPlusPer100 > 0)
			sp += sp * m_assist.m_avAddition.hcSPPlusPer100 / 100;

		// 말레이시아 경험치 포션 : 유료 아이템 : 1840, 1841, 1842 : 단위는 %
		if (m_assist.m_avAddition.hcExpPlus_398 > 0)
			exp += exp * m_assist.m_avAddition.hcExpPlus_398 / 100;

		// 케이크 : 2006 크리스마스
		if (m_assist.m_avAddition.hcExpPlus_1975 > 0)
			exp += exp * m_assist.m_avAddition.hcExpPlus_1975 / 100;

		// 눈사람 : 2006 크리스마스
		if (m_assist.m_avAddition.hcExpPlus_1976 > 0)
			exp += exp * m_assist.m_avAddition.hcExpPlus_1976 / 100;

//#ifdef EVENT_INDEPENDENCE_DAY_2007_USA
		if (m_assist.m_avAddition.hcExpSPPlus_2354)
		{
			exp += exp * 50/100;
			sp += sp * 50/100;
		}
//#endif // EVENT_INDEPENDENCE_DAY_2007_USA

//#ifdef EVENT_TLD_MOTHERDAY_2007
		if( m_assist.m_avAddition.hcExpPlus_2388 > 0 )
			exp += exp * m_assist.m_avAddition.hcExpPlus_2388 / 100;

		if( m_assist.m_avAddition.hcExpPlus_2389 > 0 )
			exp += exp * m_assist.m_avAddition.hcExpPlus_2389 / 100;

		if( m_assist.m_avAddition.hcSPPlus_2390 > 0 )
			sp += sp * m_assist.m_avAddition.hcSPPlus_2390 / 100;		
//#endif // EVENT_TLD_MOTHERDAY_2007

#ifdef NON_PVPPANALTY_EXPSPUP
#ifdef MAL_DISABLE_PKPENALTY
		if(gserver.m_bDisablePKPaenalty)
		{
			exp += exp * 10/100;
			sp += sp * 10/100;
		}
#endif // MAL_DISABLE_PKPENALTY
#endif // NON_PVPPANALTY_EXPSPUP

#ifdef EVENT_PCBANG
		if ( this->m_desc && this->m_desc->m_location == BILL_LOCATION_PCBANG )
		{
			exp += exp * 10 / 100;
			sp += sp * 5 / 100;
		}
#endif
	}
	
#ifdef HANARO_EVENT
	if (m_assist.FindBySkillIndex(654))
	{
		if (m_level < 60)
		{
			exp *= 2;
			sp  *= 2;
		}
	}
#endif

#ifdef NEW_DOUBLE_EVENT_AUTO
	if(gserver.m_bDoubleEventAutoOn && m_pZone->IsComboZone())
	{
		if(exp >= nOrgExp * 4)
			exp = nOrgExp * 4;
		if(sp >= nOrgSP * 4)
			sp = (int)(nOrgSP * 4);

		if(m_admin == 10)
		{
			GAMELOG << init("MonsterCombo Event 400 Limit", this)
				<< "OrgEXP" << delim 
				<< nOrgExp << delim
				<< "OrgSP" << delim 
				<< nOrgSP << delim
				<< "EXP" << delim 
				<< exp << delim
				<< "SP" << delim 
				<< sp << end;
		}
	}
	else
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef LIMIT_EXP_INCREASE
	{
#ifdef NEW_CASHITEM_USA_2008
		if (exp >= nOrgExp * (250 + m_assist.m_avAddition.hcExplimitPlus)/100)
			exp = nOrgExp * (250  + m_assist.m_avAddition.hcExplimitPlus)/100;
		if (sp >= nOrgSP * 5 / 2)
			sp = (int)(nOrgSP * 5 / 2);
#else
		if (exp >= nOrgExp * 5 / 2)
			exp = nOrgExp * 5 / 2;
		if (sp >= nOrgSP * 5 / 2)
			sp = (int)(nOrgSP * 5 / 2);
#endif // NEW_CASHITEM_USA_2008
		
		// 플래티늄 아이리스 축복 사용때만 300프로 리미트
		if( this->m_assist.FindByItemIndex(1416) )
		{
			if (m_assist.m_avRate.hcCashExpUp > 0)
				exp = nOrgExp + nOrgExp * m_assist.m_avRate.hcCashExpUp / SKILL_RATE_UNIT;
		}
		
		// << kjban 080125	
		// 플래티늄 숙련의 묘약 사용때만 300% 리미트
		if(m_assist.FindByItemIndex(2453))
		{
			if(m_assist.m_avAddition.hcSPPlusPer100 > 0)
			{
				sp = nOrgSP + nOrgSP * m_assist.m_avAddition.hcSPPlusPer100 / SKILL_RATE_UNIT;
			}
		}
		// >>
	}
#else // #ifndef LIMIT_EXP_INCREASE
	{
#ifdef NEW_CASHITEM_USA_2008

		if (exp >= nOrgExp * (gserver.m_nExpLimit + m_assist.m_avAddition.hcExplimitPlus)/100)
			exp = nOrgExp * (gserver.m_nExpLimit + m_assist.m_avAddition.hcExplimitPlus)/100;
		if (sp >= nOrgSP * gserver.m_nExpLimit / 100)
			sp = (int)(nOrgSP * gserver.m_nExpLimit / 100);

#else
		if (exp >= nOrgExp * gserver.m_nExpLimit / 100)
			exp = nOrgExp * gserver.m_nExpLimit / 100;
		if (sp >= nOrgSP * gserver.m_nExpLimit / 100)
			sp = (int)(nOrgSP * gserver.m_nExpLimit / 100);
#endif // NEW_CASHITEM_USA_2008
		
		if( gserver.m_nExpLimit < 300 )
		{
			// 플래티늄 아이리스 축복 사용때만 300프로 리미트
			if( this->m_assist.FindByItemIndex(1416) )
			{
				if (m_assist.m_avRate.hcCashExpUp > 0)
					exp = nOrgExp + nOrgExp * m_assist.m_avRate.hcCashExpUp / SKILL_RATE_UNIT;
			}

			// 플래티늄 숙련의 묘약 사용때만 300% 리미트
			if(this->m_assist.FindByItemIndex(2453))
			{
				if(m_assist.m_avAddition.hcSPPlusPer100 > 0)
				{
					sp = nOrgSP + nOrgSP * m_assist.m_avAddition.hcSPPlusPer100 / SKILL_RATE_UNIT;
				}
			}
		}
	}
#endif // LIMIT_EXP_INCREASE

#ifdef CHANCE_EVENT
	if ( (gserver.m_bChanceEvent == true)
		&& (gserver.CheckChanceEventLevel(m_level) == true))
	{
		exp = exp * gserver.m_bChanceExpPercent / 100;
		sp = sp * gserver.m_bChanceSpPercent / 100;
	}
#endif // CHANCE_EVENT

	if (m_level <= MAX_LEVEL)
		m_exp += exp;
	if (m_level >= MAX_LEVEL && m_exp > GetLevelupExp(m_level))
		m_exp = GetLevelupExp(m_level);

	if (m_skillPoint + sp > 0)
		m_skillPoint += sp;

#ifdef GUILD_RANKING
	if (m_level > 10 && m_guildInfo != NULL && m_guildInfo->guild())
	{
		m_guildInfo->guild()->IncreaseExpGuild(exp);
	}
#endif // GUILD_RANKING

	// Exp, SP 메시지 전송
	CNetMsg expSPMsg;
	ExpSPMsg(expSPMsg, exp, sp);
	SEND_Q(expSPMsg, m_desc);

	if (m_skillPoint > MAX_SKILLPOINT)
		m_skillPoint = MAX_SKILLPOINT;
	
	int zoneindex = this->m_pZone->m_index;

	if (GetLevelupExp(m_level) <= m_exp && m_level < MAX_LEVEL)
	{
		do {
#ifdef EVENT_NOM_MOVIE
			int nOldLevel = m_level;
#endif

			GAMELOG << init("LEVEL UP", this)
					<< m_level
					<< " ==> "
					<< m_level + 1
					<< end;
			m_exp -= GetLevelupExp(m_level);
			m_level++;
			// stat point 증가
#ifdef ENABLE_CHANGEJOB
			if (m_job2)
			{
				m_statpt_remain += 3;
				if ((m_level - JOB_2ND_LEVEL) % 2 == 0)
					m_statpt_remain += 1;
			}
			else
#endif
				m_statpt_remain++;
			
			// STAT POINT
			StatPointRemainMsg(msg, this);
			SEND_Q(msg, m_desc);
			
			// 레벨업 효과
			LevelUp(true);
			
#ifdef EVENT_NOM_MOVIE
			if (nOldLevel < 20 && m_level >= 20)
			{
				if (IS_RUNNING_CONN)
				{
					CNetMsg rmsg;
					ConnEventNomMsg(rmsg, MSG_CONN_EVENT_NOM_REQ, m_desc->m_index, m_index);
					SEND_Q(rmsg, gserver.m_connector);
				}
			}
#endif // EVENT_NOM_MOVIE


#ifdef PARTY_MATCHING
			if (IS_RUNNING_HELPER)
			{
				HelperPartyMatchMemberChangeInfoMsg(msg, m_index, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL, "", m_level, 0);
				SEND_Q(msg, gserver.m_helper);
#ifdef LC_HBK
				HelperLevelUpLogMsg(msg, this);
				SEND_Q(msg, gserver.m_helper);
#endif // LC_HBK
			}
#endif // PARTY_MATCHING
			
		} while (GetLevelupExp(m_level) <= m_exp);
	
		CalcStatus(true);
		
		Restore();
	}

	if( zoneindex != this->m_pZone->m_index)
		return true;

	return false;
}

void CPC::Restore()
{
	m_hp = m_maxHP;
	m_mp = m_maxMP;
	
	m_bChangeStatus = true;
}

void CPC::InitStat()
{
	int i;
	int level;
	int upstr, updex, upint, upcon;

#ifdef ENABLE_CHANGEJOB
	if (m_job2 == 0)
	{
#endif
		switch (m_job)
		{
		case JOB_TITAN:
		case JOB_KNIGHT:
		case JOB_HEALER:
		case JOB_MAGE:
		case JOB_ROGUE:
		case JOB_SORCERER:
#ifdef NIGHT_SHADOW
		case JOB_NIGHTSHADOW:
#endif //NIGHT_SHADOW
			level = m_level - 1;
			i = m_job;

			upstr = levelup_stat[i][0] * level + m_statpt_str;
			updex = levelup_stat[i][1] * level + m_statpt_dex;
			upint = levelup_stat[i][2] * level + m_statpt_int;
			upcon = levelup_stat[i][3] * level + m_statpt_con;
			
			m_str		= m_dbStr		= base_stat[i][0] + upstr;
			m_dex		= m_dbDex		= base_stat[i][1] + updex;
			m_int		= m_dbInt		= base_stat[i][2] + upint;
			m_con		= m_dbCon		= base_stat[i][3] + upcon;

			m_maxHP		= m_dbHP		= base_hp[i]
										+ upstr * levelup_hp[i][0]
										+ updex * levelup_hp[i][1]
										+ upint * levelup_hp[i][2]
										+ upcon * levelup_hp[i][3];
			m_maxMP		= m_dbMP		= base_mp[i]
										+ upstr * levelup_mp[i][0]
										+ updex * levelup_mp[i][1]
										+ upint * levelup_mp[i][2]
										+ upcon * levelup_mp[i][3];
			
			m_walkSpeed = walk_speed[i];
			m_runSpeed = run_speed[i];
#ifdef NIGHT_SHADOW
			m_flySpeed = 15.0f;		// 기본 나는 속도 
#endif // NIGHT_SHADOW
			break;
		}
#ifdef ENABLE_CHANGEJOB
	}
	else
	{
		switch (m_job)
		{
		case JOB_TITAN:
			switch (m_job2)
			{
			case JOB_2ND_HIGHLANDER:
				i = 0;
				break;
			case JOB_2ND_WARMASTER:
				i = 1;
				break;
			}
			break;
		case JOB_KNIGHT:
			switch (m_job2)
			{
			case JOB_2ND_ROYALKNIGHT:
				i = 2;
				break;
			case JOB_2ND_TEMPLEKNIGHT:
				i = 3;
				break;
			}
			break;
		case JOB_HEALER:
			switch (m_job2)
			{
			case JOB_2ND_ARCHER:
				i = 4;
				break;
			case JOB_2ND_CLERIC:
				i = 5;
				break;
			}
			break;
		case JOB_MAGE:
			switch (m_job2)
			{
			case JOB_2ND_WIZARD:
				i = 6;
				break;
			case JOB_2ND_WITCH:
				i = 7;
				break;
			}
			break;
		case JOB_ROGUE:
			switch (m_job2)
			{
			case JOB_2ND_ASSASSIN:
				i = 8;
				break;
			case JOB_2ND_RANGER:
				i = 9;
				break;
			}
			break;
		case JOB_SORCERER:
			switch (m_job2)
			{
			case JOB_2ND_ELEMENTALLIST:
				i = 10;
				break;
			case JOB_2ND_SPECIALLIST:
				i = 11;
				break;
			}
			break;
#ifdef NIGHT_SHADOW
		case JOB_NIGHTSHADOW:
			i = 12;			// JOB_2ND_NIGHTSHADOW
			break;
#endif // NIGHT_SHADOW
		}

		level = m_level - JOB_2ND_LEVEL;

		upstr = levelup_stat2[i][0] * level;
		updex = levelup_stat2[i][1] * level;
		upint = levelup_stat2[i][2] * level;
		upcon = levelup_stat2[i][3] * level;
		
		m_str		= m_dbStr		= base_stat2[i][0] + upstr + m_statpt_str;
		m_dex		= m_dbDex		= base_stat2[i][1] + updex + m_statpt_dex;
		m_int		= m_dbInt		= base_stat2[i][2] + upint + m_statpt_int;
		m_con		= m_dbCon		= base_stat2[i][3] + upcon + m_statpt_con;

		switch (m_job)
		{
		case JOB_KNIGHT:
			m_maxHP		= m_dbHP		= base_hp2[i] + upcon * levelup_hp2[i] + m_statpt_con * 3 + level * 29;
			m_maxMP		= m_dbMP		= base_mp2[i] + upcon * levelup_mp2[i] + m_statpt_con * 3 + level * 10;
			break;
		case JOB_ROGUE:
			m_maxHP		= m_dbHP		= base_hp2[i] + upcon * levelup_hp2[i] + m_statpt_con * 3 + level * 31;
			m_maxMP		= m_dbMP		= base_mp2[i] + upcon * levelup_mp2[i] + m_statpt_con * 3 + level * 10;
			break;
		case JOB_SORCERER:
			if (m_job2 == JOB_2ND_ELEMENTALLIST)
				m_maxHP	= m_dbHP		= base_hp2[i] + upcon * levelup_hp2[i] + m_statpt_con * 3 + level * 10;
			else
				m_maxHP	= m_dbHP		= base_hp2[i] + upcon * levelup_hp2[i] + m_statpt_con * 3 + level * 32;
			m_maxMP		= m_dbMP		= base_mp2[i] + upcon * levelup_mp2[i] + m_statpt_con * 3 + level * 10;
			break;
#ifdef NIGHT_SHADOW		//공식 검토 필요
		case JOB_NIGHTSHADOW:
			m_maxHP		= m_dbHP		= base_hp2[i] + level * ( 26+m_statpt_con*2 ) + upcon * 2 ;
			m_maxMP		= m_dbMP		= base_mp2[i] + level * ( 10+m_statpt_con*5 ) + upcon * 5 ;

			break;			
#endif //NIGHT_SHADOW
		default:
			m_maxHP		= m_dbHP		= base_hp2[i] + upcon * levelup_hp2[i] + m_statpt_con * 3 + level * 26;
			m_maxMP		= m_dbMP		= base_mp2[i] + upcon * levelup_mp2[i] + m_statpt_con * 3 + level * 10;
			break;
		}
		
		
		m_walkSpeed = walk_speed[(int)m_job];
		m_runSpeed = run_speed[(int)m_job];
#ifdef NIGHT_SHADOW
		m_flySpeed = 15.0f; 
#endif //NIGHT_SHADOW
	}
#endif

#ifdef JP_OTAKU_SYSTEM
	if( m_etcEvent & OTAKU_SYSTEM )
	{
		m_maxHP = 1;
		m_dbHP  = 1;
		m_hp = 1;
		m_maxMP = 1;
		m_dbMP  = 1;
		m_mp	= 1;
	}
#endif
}

/*
	Description
		create item and give it to pc
		
	Param
		IN		int			itemIndex		// item db index
		IN		int			plus			// item plus
		IN		int			flag			// item flag
		IN		LONGLONG	itemNum			// item count 
		IN		bool		ndrop			// can drop field
		IN		bool		nOtion			// can plus option

	Return
		type bool
		if item create and give success, return true, otherwise return false
*/
bool CPC::GiveItem(int itemIndex, int plus, int flag, LONGLONG itemNum, bool bdrop, bool bOption)
{
	CItem* item = NULL;
	item = gserver.m_itemProtoList.CreateItem(itemIndex, -1, plus, flag, itemNum);

	if (!item)
	{
		//////////////////////////////
		// item 생성 실패
//		GAMELOG << init("CREATE_AND_GIVE_ITEM_ERROR CANNOT CREATE", this)
//					<< itemIndex << delim
//					<< itemNum << delim
//					<< end;
		return false;
	}

	bool bIsRare = false;

#ifdef MONSTER_RAID_SYSTEM
	if (item->IsRareItem())
	{
		bOption = false;
		bIsRare = true;
	}
#endif // MONSTER_RAID_SYSTEM

#ifdef SET_ITEM
	if (item->IsOriginItem())
	{
		bOption = false;
		bIsRare = false;
	}
#endif

	if( bOption )
	{ 
		const static int Option_table[5][4] = {
			{ 22, 12 ,13, 18 },		// 근력의 반지 옵션 타입 a_type
			{ 23, 14, 15, 18 },		// 민첩의 목걸이 옵션 타입 a_type
			{ 7, 11, 18, 20 },		// 지혜의 귀걸이 옵션 타입 a_type
			{ 20, 8, 11, 18 },		// 생명의 목걸이 옵션 타입 a_type
			{ 0, 1, 2, 3} };		// 영혼의 반지 옵션 타입 a_type

		int Option_type = 0;
		switch( itemIndex )
		{
		case 1823:
			break;
		case 1824:
			Option_type = 1;
			break;
		case 1825:
			Option_type = 2;
			break;
		case 1826:
			Option_type = 3;
			break;
		case 1827:
			Option_type = 4;
			break;
		}

		for (int i=0; i < MAX_ITEM_OPTION - 1; i++)
		{
			if ( Option_table[Option_type][i] < 0 || Option_table[Option_type][i] >= MAX_NUM_OPTION )
					continue;

			item->m_option[i].m_proto = gserver.m_optionProtoList.FindProto(Option_table[Option_type][i]);
			
			if (!item->m_option[i].m_proto)
				continue;

			item->m_option[i].m_type = Option_table[Option_type][i];
			item->m_option[i].m_level = 4;
			item->m_option[i].m_value = item->m_option[i].m_proto->m_levelValue[item->m_option[i].m_level - 1];
			item->m_option[i].SetDBValue();
			item->m_nOption++;
		}
	}
	else if (!bIsRare)
	{
		if( item->IsAccessary()  && item->m_nOption < 1)
		{
		 	OptionSettingItem( this, item );
		}
 	 }
				
//	if(item->IsAccessary())
//	{
//		OptionSettingItem(this, item);
//	}

	CNetMsg itemmsg;

	// 들어갈 인벤토리 결정
	CInventory* inven = GET_INVENTORY(this, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
	if (!inven)
	{
		//////////////////
		// inven 결정 실패
		return false;
	}
		
	bool bCountable = false;
	// 인벤에 넣기
	if (AddToInventory(this, item, true, true))
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

				delete item;
				item = NULL;
			}
		}
		else
		{
			ItemAddMsg(itemmsg, item);
			// 돈 검사
			if (item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && this->m_moneyItem == NULL)
				this->m_moneyItem = item;
		}
		SEND_Q( itemmsg, this->m_desc);
	}
	else
	{
		if( !bdrop )
		{
//			GAMELOG << init("CREATE_AND_GIVE_ITEM : NotDrop", this)
//					<< itemIndex << delim
//					<< itemNum << delim
//					<< end;
			CNetMsg msg;
			SysFullInventoryMsg(msg, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
			SEND_Q(msg, this->m_desc);
			delete item;
			return false;
		}

		// 인젠토리 꽉차서 못 받을 때
//		GAMELOG << init("CREATE_AND_GIVE_ITEM : DROP", this)
//				<< itemlog(item)
//				<< end;

		// 인벤토리 가득참
		item = this->m_pArea->DropItem(item, this);
		if (!item)
		{
			/////////////////
			// item drop 실패
			return false;
		}
		
			
		CNetMsg dropMsg;
		item->m_preferenceIndex = this->m_index;
		ItemDropMsg(dropMsg, this, item);
		this->m_pArea->SendToCell(dropMsg, this, true);
	}

	return true;
}

void CPC::LevelUp(bool bSendEffect)
{

#ifdef JP_OTAKU_SYSTEM
	if( this->m_etcEvent & OTAKU_SYSTEM )
	{
		return;
	}
#endif // JP_OTAKU_SYSTEM
	CNetMsg msg;

#ifdef LIMIT_ITEM_M35
	if(m_level > 35)
	{	// 35레벨 초과시 초보자용 목걸이 벗기기
		int newbie_item[6] = {
			2669,	// 타이탄 목걸이
			2670,	// 나이트 목걸이
			2671,	// 힐러 목걸이
			2672,	// 메이지 목걸이
			2673,	// 로그 목걸이
			2674,	// 소서러 목걸이
		};
		
		int wear_position = -1;
		for(int i=WEARING_ACCESSORY1; i <= WEARING_ACCESSORY3; i++)
		{
			if(m_wearing[i] != NULL
				&& m_wearing[i]->m_itemProto != NULL
				&& m_wearing[i]->m_itemProto->m_index == newbie_item[m_job])
			{
				wear_position = i;
				break;
			}
		}

		if(wear_position != -1)
		{
			CItem * pItem = m_wearing[wear_position];
			m_wearing[wear_position] = NULL;
			pItem->m_wearPos = WEARING_NONE;
			
			CNetMsg rmsg;
			ItemWearMsg(rmsg, wear_position, NULL, pItem);
			SEND_Q(rmsg, m_desc);
			WearingMsg(rmsg, this, wear_position, -1, 0);
			m_pArea->SendToCell(rmsg, this, false);
		}
	}
#endif // LIMIT_ITEM_M35

	InitStat();

	CalcStatus(true);

	// 레벨업 효과 메시지 전송
	if (bSendEffect)
	{
		EffectEtcMsg(msg, this, MSG_EFFECT_ETC_LEVELUP);
		this->m_pArea->SendToCell(msg, this, true);
	}
#ifdef IRIS_POINT
	if( IS_RUNNING_CONN && m_desc )
	{
		int nIpoint = 0;
		if( m_level < 21 )			nIpoint=1;
		else if( m_level < 41 )		nIpoint=2;
		else if( m_level < 51 )		nIpoint=4;
		else if( m_level < 61 )		nIpoint=8;
		else if( m_level < 71 )		nIpoint=15;
		else if( m_level < 81 )		nIpoint=25;
		else if( m_level < 91 )		nIpoint=40;
		else if( m_level < 101 )	nIpoint=70;
		else if( m_level < 111 )	nIpoint=100;
		else if( m_level < 121 )	nIpoint=150;
		else if( m_level < 131 )	nIpoint=250;
		else if( m_level < 141 )	nIpoint=400;
		else						nIpoint=600;

		if( m_level%10 == 0 )
		{
			m_level<95?nIpoint*=10:nIpoint*=3;
		}

		CNetMsg ipointMsg;
		ConnIPointAddMsg( ipointMsg, m_desc->m_index, m_index,  MSG_IPOINT_TYPE_LEVELUP, nIpoint );
		SEND_Q(ipointMsg,gserver.m_connector );
	}
#endif //IRIS_POINT


#ifdef EVENT_PROMOTION_SITE	// 한국 프로모션 사이트 이벤트 (B1 엠파스, 아이템베이 IT); 4 군만 적용 
#ifdef EVENT_NEW_SERVER_2006_NEWSERVER
	if (this->m_desc 
		&& gserver.m_serverno == EVENT_NEW_SERVER_2006_SERVERNO 
		&& (strcmp(this->m_desc->m_proSite, "B1")  == 0 || strcmp(this->m_desc->m_proSite, "IT")  == 0 ))
	{
		bool bGive = false;
		bool eventLevel = false;
		switch(this->m_level)
		{
		case 30:
			eventLevel = true;
			if( this->GiveItem(19, 0, 0, 1000000, true) )
				bGive = true;
			break;
		case 20:
			eventLevel = true;
			if( this->GiveItem(19, 0, 0, 500000, true) )
				bGive = true;
			break;
		case 10:
			eventLevel = true;
			if( this->GiveItem(19, 0, 0, 300000, true) )
				bGive = true;
			break;
		default:
			break;
		}

		if( bGive )
		{
			GAMELOG << init("EMPAS_ITEMBAY_EVENT", this)
					<< this->m_level << delim
					<< this->m_desc->m_proSite
					<< end;
		}
		else if( eventLevel )
		{
			GAMELOG << init("EMPAS_ITEMBAY_EVENT_ERROR", this)
					<< this->m_level << delim
					<< this->m_desc->m_proSite
					<< end;
		}
	}
#endif // EVENT_NEW_SERVER_2006_NEWSERVER
#endif // EVENT_PROMOTION_SITE

#if defined(EVENT_NEWSERVER_BASTARD) && !defined(LC_MAL)
	static const int		nNewServerNo = 5;
	if (gserver.m_serverno == nNewServerNo && m_level == 35)
	{
		if (GiveItem(19, 0, 0, 1000000, true))
		{
			GAMELOG << init("EVENT NEWSERVER BASTARD LEVEL UP", this)
					<< m_level
					<< end;
		}
		else
		{
			GAMELOG << init("EVENT NEWSERVER BASTARD LEVEL UP ERROR", this)
					<< m_level
					<< end;
		}
	}
#endif // EVENT_NEWSERVER_BASTARD

	// 사제 시스템 체크
	if ( MSG_TEACH_STUDENT_TYPE == m_teachType )
	{
		CNetMsg rmsg;
		CPC* tch = gserver.m_playerList.Find(m_teachIdx[0]);

		if(TEACH_LEVEL_SUCCESS <= m_level)
		{
			if (tch)
			{
				TeachStudentLevelUPMsg(rmsg, this);
				SEND_Q(rmsg, tch->m_desc);
			}

#ifdef EVENT_TEACH_NAS
			if(m_level > TEACH_PRIZE_LEVEL)
			{
				// 나스지급				
				TeachStudentLevelUPMsg(rmsg, this);
				SEND_Q(rmsg, m_desc);
				this->GiveItem(19, 0, 0, 10000);
			}
#endif
		}

		if (TEACH_LEVEL_SUCCESS <= m_level)
		{
			//이벤트검 지급
#ifdef NIGHT_SHADOW
			int itemindex[JOBCOUNT] = { 460, 427, 462, 461, 951, 1069, 427 };
#else
			int itemindex[JOBCOUNT] = { 460, 427, 462, 461, 951, 1069 };
#endif //NIGHT_SHADOW


#ifdef EVENT_TEACH_LIGHT_USER
			if(true == GiveItem(itemindex[(int)this->m_job], 15, 0, 1, true))
#else
			if(true == GiveItem(itemindex[(int)this->m_job], 0, 0, 1, true))
#endif // EVENT_TEACH_LIGHT_USER
			{
				GAMELOG << init("TEACH_EVENT_ITEM_GIVE", this)
						<< itemindex[(int)this->m_job]
						<< end;
			}
#ifdef EVENT_CHILDERN_DAY
			if(true == GiveItem(19, 0, 0, 500000, true))
			{
				GAMELOG << init("EVENT_CHILDERN_DAY", this)
						<< 19
						<< end;
			}
#endif // EVENT_CHILDERN_DAY
			
#ifdef EVENT_TEACH_2007
			// 견장 지급			
			if(true == GiveItem(2329, 0, 0, 1, true))
			{
				GAMELOG << init("TEACH_EVENT_ITEM_GIVE", this)
						<< " GIVE YELLOW STRAP";

				if(IS_RUNNING_HELPER)
				{
					HelperTeachAddFlowerMsg(rmsg, m_teachIdx[0]);
					SEND_Q(rmsg, gserver.m_helper);
				}				
			}
#endif	// EVENT_TEACH_2007

			TeachEndMsg(rmsg, m_teachIdx[0], m_teachName[0], m_index, this->GetName(), MSG_TEACH_END_SUCCESS);
			SEND_Q(rmsg, m_desc);

			// Helper에 명성치 추가 요청
// 			if(IS_RUNNING_HELPER)
// 			{
// #if defined (NEW_GUILD)
// 				if( this->m_guildInfo && this->m_guildInfo->guild() )
// 				{
// 					if( this->m_guildInfo->GetcontributeFame() != 0 )
// 					{
// 						int guildindex = this->m_guildInfo->guild()->index();
// 						int GuildFame = this->m_GuildFame;
// 						CGuild* guild = gserver.m_guildlist.findguild( guildindex );
// 						GuildFame = GuildFame + ( TEACH_FAME_PRIZE / this->m_guildInfo->GetcontributeFame() / 100 );
// 						this->m_GuildFame += GuildFame;	
// 						int GP = 0;
// 						int Fame = TEACH_FAME_PRIZE;
// 						if( this->m_GuildFame > 10 )
// 						{
// 							GP = this->m_GuildFame / 10;
// 							Fame = TEACH_FAME_PRIZE - ( TEACH_FAME_PRIZE / this->m_guildInfo->GetcontributeFame() / 100 );
// 							this->m_GuildFame = this->m_GuildFame % 10;
// 							//gserver.m_guildlist.findguild( guildindex )->AddGuildPoint( GP );
// 							guild->AddGuildPoint( GP );
// 							this->m_guildInfo->AddCumulatePoint( GP );
// 							
// 							HelperTeachFameUpReqMsg(rmsg, m_teachIdx[0], this->m_index, this->GetName(), Fame );
// 						}
// 						else
// 						{
// 							HelperTeachFameUpReqMsg(rmsg, m_teachIdx[0], this->m_index, this->GetName(), Fame);
// 						}				
// 
// 						if (IS_RUNNING_HELPER)
// 						{
// 							CNetMsg guildPointMsg;
// 							CNetMsg guildMemberPointMsg;
// 							HelperNewGuildPointUpdate( guildPointMsg, this->m_index, guild->index(), GP );
// 							SEND_Q(guildPointMsg, gserver.m_helper);
// 
// 							HelperSaveGuildMemberPointMsg( guildMemberPointMsg, guild->index(), this->m_index, this->m_guildInfo->GetcumulatePoint() );
// 							SEND_Q( guildMemberPointMsg, gserver.m_helper );
// 						}
// 
// 					}
// 				}
// 				else
// 				{
// 					HelperTeachFameUpReqMsg(rmsg, m_teachIdx[0], this->m_index, this->GetName(), TEACH_FAME_PRIZE);
// 				}
// #elif defined (EVENT_TEACH) || defined ( EVENT_CHILDERN_DAY )
// 				HelperTeachFameUpReqMsg(rmsg, m_teachIdx[0], this->m_index, this->GetName(), TEACH_FAME_PRIZE * 2);
// #else
// 				HelperTeachFameUpReqMsg(rmsg, m_teachIdx[0], this->m_index, this->GetName(), TEACH_FAME_PRIZE);
// #endif

// <--! edit by kjban, 080228
			//  주는 명성치
			int Fame = TEACH_FAME_PRIZE;

			// Helper에 명성치 추가 요청
			if(IS_RUNNING_HELPER)
			{
#if defined (EVENT_TEACH) || defined ( EVENT_CHILDERN_DAY )
				Fame = Fame * 2;				
#endif

#ifdef NEW_GUILD	
/*
길드 마스터가 길드워의 명성치 상납 정도를 설정해두면, 
그 이후부터 길드원이 획득하는 명성치의 일부가 아래와 같이 계산되어 자동으로 길드포인트로 전환된다.

  명성치의 길드 포인트 전환 비율 = 상납될 명성치 10 당 GP 1

길드에 가입한 캐릭터는 상압 정도가 설정된 이후부터 획득한 명성치 중 상납될 명성치가 따로 기억되어야 하며, 
상납될 명성치가 쌓여 10이 넘을 때 마다 자동으로 GP 1로 변환하여 길드포인트로 등록된다.

  Ex. 나의 명성치 상납 % -> 50%
	견습생 2명을 졸업 시킬시 획득하는 명성치가 20 이라고 가정
	실제 캐릭터가 획득하는 명성치 = 20 * 0.5 =10
	길드 포인트로 상납될 경험치 = 20 * 0.5 = 10 따라서 GP 1

해당 캐릭터가 길드에서 탈퇴해도 이미 변환되어 등록된 GP는 회수되지 않으며, 
10이 되지 않아 변환 되지 않고 남아 있는 상납 명성치는 초기화 되지 않고 그대로 보관된다.
*/
				if(tch && NULL != tch->m_guildInfo 
					&& NULL != tch->m_guildInfo->guild())
				{	// 길드가 있는 경우
					// 1-1. 길마가 정한 상납 명성치 %가 존재
					if(0 != tch->m_guildInfo->GetcontributeFame())
					{	
						int guildindex = tch->m_guildInfo->guild()->index();
						CGuild * guild = gserver.m_guildlist.findguild(guildindex);

						// 길드에 상납하는 명성치
						int GuildFame = Fame * tch->m_guildInfo->GetcontributeFame() / 100;	
						// 캐릭터가 받는 명성치
						Fame = Fame - GuildFame;						
													
						// 상납될 GP
						tch->m_GuildFame += GuildFame;
						int GP = tch->m_GuildFame / 10;
						// 상납 변환 대기 명성치
						tch->m_GuildFame = tch->m_GuildFame % 10;

						// 상납될 GP가 있는 경우
						if(GP > 0 )
						{													
							guild->AddGuildPoint( GP );				// 길드에 GP 추가
							tch->m_guildInfo->AddCumulatePoint( GP );	// 자신이 바친 GP 총합

							CNetMsg guildPointMsg;
							CNetMsg guildMemberPointMsg;
							HelperNewGuildPointUpdate( guildPointMsg, tch->m_index, guild->index(), GP );
							SEND_Q(guildPointMsg, gserver.m_helper);

							HelperSaveGuildMemberPointMsg( guildMemberPointMsg, guild->index(), tch->m_index, GP );
							SEND_Q( guildMemberPointMsg, gserver.m_helper );
						}	// end if 상납될 GP가 있는 경우
					}	// end if 길마가 정한 상납 명성치 %가 존재
				}	// end if 길드가 있는 경우 
#endif // NEW_GUILD

				HelperTeachFameUpReqMsg(rmsg, m_teachIdx[0], this->m_index, this->GetName(), Fame);
// !-->
				SEND_Q(rmsg, gserver.m_helper);

				GAMELOG << init("TEACH_SUCCESS_HELPER", this)
						<< m_teachName[0] << delim
						<< end;

				
				// 혼자 셋팅 해제
				m_teachIdx[0] = -1;
				m_teachJob[0] = -1;
#ifdef ENABLE_CHANGEJOB
				m_teachJob2[0] = 0;
#endif
				m_teachLevel[0] = 0;
				m_teachName[0][0]= '\0';
				m_teachTime[0] = 0;
				m_teachType = MSG_TEACH_NO_TYPE;

			}				
		}

	}

	if((m_teachType == MSG_TEACH_STUDENT_TYPE 
		|| m_teachType == MSG_TEACG_NO_STUDENT_TYPE) 
		&& m_level >= TEACH_LEVEL_SUCCESS)
	{
		m_teachType = MSG_TEACH_NO_TYPE;
	}

	// 레벨업으로 시작하는 퀘스트 검색
	int i;
	int count = 0;
	for (i=0; i < gserver.m_questProtoList.m_nCount; i++)
	{
		if (gserver.m_questProtoList.m_proto[i].m_startType == QSTART_LEVEL && gserver.m_questProtoList.m_proto[i].m_startData == m_level)
		{
			// 필요 레벨 조건
			if (gserver.m_questProtoList.m_proto[i].m_needMinLevel > m_level || gserver.m_questProtoList.m_proto[i].m_needMaxLevel < m_level)
				continue;
			
			// 필요 직업 조건
			if (gserver.m_questProtoList.m_proto[i].m_needJob != -1 && gserver.m_questProtoList.m_proto[i].m_needJob != m_job)
				continue;

			// 하드코딩 : 퍼스널 던전 퀘스트 , 튜토리얼 퀘스트 제외
			if (gserver.m_questProtoList.m_proto[i].m_index == 42 || gserver.m_questProtoList.m_proto[i].m_index == 45)
				continue;

			count++;
		}
	}

	if (count > 0)
	{
		int* qIndex;
		qIndex = new int [count];
		int idx = 0;
		for (i=0; i < gserver.m_questProtoList.m_nCount; i++)
		{
			if (gserver.m_questProtoList.m_proto[i].m_startType == QSTART_LEVEL && gserver.m_questProtoList.m_proto[i].m_startData == m_level)
			{
				// 필요 레벨 조건
				if (gserver.m_questProtoList.m_proto[i].m_needMinLevel > m_level || gserver.m_questProtoList.m_proto[i].m_needMaxLevel < m_level)
					continue;
				
				// 필요 직업 조건
				if (gserver.m_questProtoList.m_proto[i].m_needJob != -1 && gserver.m_questProtoList.m_proto[i].m_needJob != m_job)
					continue;

				qIndex[idx] = gserver.m_questProtoList.m_proto[i].m_index;
				idx++;
			}
		}

		CNetMsg msg;
		QuestCreateListMsg(msg, count, qIndex);
		SEND_Q(msg, m_desc);
	}

	// 레벨이 4이상 나는 퀘스트는 포기시킨다.
#ifdef QUEST_DATA_EXTEND
	CQuest* pQuest;
	CQuest* pQuestNext = m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		if(!pQuest->IsCompleted() && pQuest->GetQuestProto()->m_needMaxLevel + 4 < m_level)
		{
			do_QuestGiveUp(this, pQuest);
		}
	}
#else // QUEST_DATA_EXTEND
	for (i=0; i < QUEST_MAX_PC; i++)
	{
		if (!m_questList.m_list[i])
			continue;
	
		if(!m_questList.IsCompleted(i) &&  m_questList.m_list[i]->m_proto->m_needMaxLevel +4 < m_level)
		{
			do_QuestGiveUp(this, m_questList.m_list[i]);
		}
	}
#endif // QUEST_DATA_EXTEND

	// 싱글던전입장권 제공
	if (m_level >= S2_TICKET_LEVEL_START && m_level <= S2_TICKET_LEVEL_END)
	{
		CItem* item = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_sTicketItem2->m_index, -1, 0 ,0, 1);
		
		if (!item)
			return;
		
		CNetMsg itemmsg;
		
		// 들어갈 인벤토리 결정
		CInventory* inven = GET_INVENTORY(this, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
		if (!inven)
			return;
		
		bool bCountable = false;
		// 인벤에 넣기
		if (AddToInventory(this, item, true, true))
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
			item = m_pArea->DropItem(item, this);
			if (!item)
				return;

			CNetMsg dropMsg;
			item->m_preferenceIndex = m_index;
			ItemDropMsg(dropMsg, this, item);
			m_pArea->SendToCell(dropMsg, this, true);
		}
		
		// 메시지 보내고
		SEND_Q(itemmsg, m_desc);
		
		// Item LOG
		GAMELOG << init("ITEM_PICK_LEVELUP", this)
			<< itemlog(item)
			<< end;
		
		if (bCountable)
			delete item;
	}

#ifdef EVENT_OPENBETA_LEVELUP
	if (m_level >= 5)
	{
		if (IS_RUNNING_CONN)
		{
			int point;
			if (m_level > 10)
				point = 2 * m_level;
			else
				point = m_level;
			CNetMsg rmsg;
			ConnLevelupPointMsg(rmsg, m_desc->m_index, point);
			SEND_Q(rmsg, gserver.m_connector);

			GAMELOG << init("EVENT OPENBETA", this)
					<< "Level" << delim << m_level << delim
					<< "Point" << point << end;
		}
	}
#endif // EVENT_OPENBETA_LEVELUP

#ifdef EVENT_LATTO
	if (gserver.m_bLattoEvent)
	{
		/////////////////////////////
		// Exp, SP Event

		CNetMsg rmsg;
		if (GetRandom(1, 10000) <= LATTO_EVENT_EXPSP_PROB)
		{
			int upExpPer = LATTO_EVENT_EXP_PERCENT - m_level;
			int upSP = m_level;

			if (upExpPer < 1)
				upExpPer = 1;

			LONGLONG exp = GetLevelupExp(m_level) * (LONGLONG)upExpPer / (LONGLONG)100;
			int sp = upSP * 10000;

			m_exp += exp;
			m_skillPoint += sp;
			
			EventLattoMsg(rmsg, MSG_EVENT_LATTO_EXPSP_SUCCESS, upExpPer, upSP);
			SEND_Q(rmsg, m_desc);
		}
		
		/////////////////////////////
		// 복주머니 아이템
		if (m_party && m_party->m_partyType == 0 && m_level >= LATTO_EVENT_PARTY_LEVEL_MIN && m_level <= LATTO_EVENT_PARTY_LEVEL_MAX)
		{
			if (GetRandom(1, 10000) <= LATTO_EVENT_PARTY_PROB)
			{
				int i;
				for (i=0; i < MAX_PARTY_MEMBER; i++)
				{
					if (m_party->m_member[i] && CheckInNearCellExt(m_party->m_member[i], this) && !DEAD(m_party->m_member[i]))
					{
						// 복주머니 : 507
						CItem* item = gserver.m_itemProtoList.CreateItem(507, -1, 0 ,0, 1);
						
						if (!item)
							return;
						
						CNetMsg itemmsg;
						
						// 들어갈 인벤토리 결정
						CInventory* inven = GET_INVENTORY(m_party->m_member[i], GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
						if (!inven)
							return;
						
						bool bCountable = false;
						// 인벤에 넣기
						if (inven->AddItem(item, m_party->m_member[i], true, true))
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
							item = m_pArea->DropItem(item, m_party->m_member[i]);
							if (!item)
								return;
							
							CNetMsg dropMsg;
							item->m_preferenceIndex = m_index;
							ItemDropMsg(dropMsg, m_party->m_member[i], item);
							m_pArea->SendToCell(dropMsg, m_party->m_member[i], true);
						}
						
						// 메시지 보내고
						SEND_Q(itemmsg, m_party->m_member[i]->m_desc);
						
						// Item LOG
						GAMELOG << init("ITEM_PICK_LEVELUP", m_party->m_member[i])
							<< itemlog(item)
							<< end;
						
						if (bCountable)
							delete item;

						CNetMsg rmsg;
						EventLattoMsg(rmsg, MSG_EVENT_LATTO_PARTY_SUCCESS);
						SEND_Q(rmsg, m_party->m_member[i]->m_desc);
					}
				}
			}
		}
	}
#endif
#ifdef EVENT_CLOSEBETA_LEVELUP
	{
		// 최대 4개를 지급
		bool bGive = false;
		CItem* item[4] = { NULL, NULL, NULL, NULL };
		switch (m_level)
		{
		case 10:
			item[0] = gserver.m_itemProtoList.CreateItem(391, -1, 0, 0, 5);
			item[1] = gserver.m_itemProtoList.CreateItem(511, -1, 0, 0, 5);
			bGive = true;
			break;
		case 15:
			item[0] = gserver.m_itemProtoList.CreateItem(510, -1, 0, 0, 5);
			item[1] = gserver.m_itemProtoList.CreateItem(511, -1, 0, 0, 5);
			bGive = true;
			break;
		case 19:
			item[0] = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_moneyItem->m_index, -1, 0, 0, 100000);
			bGive = true;
			break;
		case 20:
			item[0] = gserver.m_itemProtoList.CreateItem(499, -1, 0, 6, 5);
			item[1] = gserver.m_itemProtoList.CreateItem(501, -1, 0, 6, 5);
			bGive = true;
			break;
		case 25:
			item[0] = gserver.m_itemProtoList.CreateItem( 45, -1, 0, 0, 5);
			item[1] = gserver.m_itemProtoList.CreateItem(485, -1, 0, 0, 5);
			item[2] = gserver.m_itemProtoList.CreateItem(498, -1, 0, 6, 5);
			item[3] = gserver.m_itemProtoList.CreateItem(500, -1, 0, 6, 5);
			bGive = true;
			break;
		case 29:
			item[0] = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_moneyItem->m_index, -1, 0, 0, 300000);
			bGive = true;
			break;
		case 30:
			item[0] = gserver.m_itemProtoList.CreateItem(499, -1, 0, 12, 5);
			item[1] = gserver.m_itemProtoList.CreateItem(501, -1, 0, 12, 5);
			item[2] = gserver.m_itemProtoList.CreateItem(498, -1, 0, 12, 5);
			item[3] = gserver.m_itemProtoList.CreateItem(500, -1, 0, 12, 5);
			bGive = true;
			break;
		}

		if (bGive)
		{
			CNetMsg rmsg;

			GAMELOG << init("CLOSED BETA LEVEL UP EVENT", m_name, m_desc->m_idname)
					<< "LEVEL" << delim
					<< m_level;
			int i;
			for (i = 0; i < 4; i++)
			{
				if (item[i])
				{
					GAMELOG << delim << "ITEM " << (i + 1)
					    	<< delim << itemlog(item[i]);
					
					if (AddToInventory(this, item[i], true, true))
					{
						GAMELOG << delim << "INVEN";
						if (item[i]->tab() < 0)
						{
							int r, c;
							CInventory* inven = GET_INVENTORY(this, GET_TAB(item[i]->m_itemProto->m_typeIdx, item[i]->m_itemProto->m_subtypeIdx));
							if (inven)
							{
								if (inven->FindItem(&r, &c, item[i]->m_itemProto->m_index, item[i]->m_plus, item[i]->m_flag))
								{
									LONGLONG updateCount = item[i]->Count();
									CItem* curItem = inven->GetItem(r, c);
									if (curItem)
									{
										ItemUpdateMsg(rmsg, curItem, updateCount);
										SEND_Q(rmsg, m_desc);
									}
								}
							}
							delete item[i];
							item[i] = NULL;
						}
						else
						{
							ItemAddMsg(rmsg, item[i]);
							SEND_Q(rmsg, m_desc);
						}
					}
					else
					{
						GAMELOG << delim << "DROP";
						
						m_pArea->DropItem(item[i], this);
						item[i]->m_preferenceIndex = m_index;
						ItemDropMsg(rmsg, this, item[i]);
						m_pArea->SendToCell(rmsg, GET_YLAYER(item[i]), item[i]->m_cellX, item[i]->m_cellZ);
					}
				}
			}

			GAMELOG << end;
		}
	}
#endif // #ifdef EVENT_CLOSEBETA_LEVELUP
	
#ifdef HANARO_EVENT
	if (m_level == 31 || m_level == 60)
	{
		int* pItemIndex = NULL;
		int  nItemIndex = 0;
		int  nItemCount = 7;
		int  nItemPlus = 0;
		
		static int nTitanItem[2][7] = {
			{321, 313, 315, 317, 319, 608, 310},			// 31 레벨 달성
			{1349, 1350, 1351, 1352, 1353, 1333, 1334}		// 60 레벨 달성
		};
		
		static int nNightItem[2][8] = {
			{336, 328, 330, 332, 334, 617, 325, 393},
			{1343, 1344, 1345, 1346, 1347, 1348, 1332, 1331}
		};
		
		static int nHealerItem[2][7] = {
			{355, 347, 349, 351, 353, 626, 344},
			{1354, 1355, 1356, 1357, 1358, 1336, 1335}
		};
		
		static int nMageItem[2][7] = {
			{390, 370, 375, 380, 385, 635, 364},
			{1359, 1360, 1361, 1362, 1363, 1337, 1338}
		};
		
		static int nRogueItem[2][7] = {
			{569, 571, 573, 575, 577, 538, 643},
			{1364, 1365, 1366, 1367, 1368, 1339, 1340}
		};
		
		static int nSorcererItem[2][7] = {
			{1046, 1006, 1016, 1026, 1036, 996, 984},
			{1374, 1375, 1376, 1377, 1378, 1341, 1342}
		};
		
		if (m_level == 31)
		{
			nItemPlus = 10;
			nItemIndex = 0;
		}
		else if (m_level == 60)
		{
			nItemPlus = 5;
			nItemIndex = 1;
		}
		
		switch (m_job)
		{
		case JOB_TITAN:
			pItemIndex = nTitanItem[nItemIndex];
			break;
		case JOB_KNIGHT:
			pItemIndex = nNightItem[nItemIndex];
			nItemCount = 8;
			break;
		case JOB_HEALER:
			pItemIndex = nHealerItem[nItemIndex];
			break;
		case JOB_MAGE:
			pItemIndex = nMageItem[nItemIndex];
			break;
		case JOB_ROGUE:
			pItemIndex = nRogueItem[nItemIndex];
			break;
		case JOB_SORCERER:
			pItemIndex = nSorcererItem[nItemIndex];
			break;
		default:
			nItemCount = 0;
		}
		
		int i = 0;
		for (i = 0; i < nItemCount; i++)
		{
			// 아이템 지급
			if (!GiveItem(pItemIndex[i], nItemPlus, 0, 1, false, false))
			{
				GAMELOG << init("HANARO_EVENT ERROR", this) << "ITEM" << delim << pItemIndex[i] << end;
				
				// 롤백 
				int r, c;
				for( ; i > 0; i--)
				{
					if (m_invenNormal.FindItem(&r, &c, pItemIndex[i], 0, 0))
					{
						CItem* item = m_invenNormal.GetItem(r, c);
						if(item)
						{
							CNetMsg itemMsg;
							
							// Item 수량 변경
							DecreaseFromInventory(this, item, 1);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -1);
								SEND_Q(itemMsg, m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, m_desc);
								RemoveItemFromQuickSlot(item);
								RemoveFromInventory(this, item, true, true);
							}
						}
					} 
				}// 롤백 end
				break;
			}
			
			GAMELOG << init("HANARO_EVENT", this) << "ITEM" << delim << pItemIndex[i] << end;
		} // for
	}
#endif
}

void CPC::RemoveItemFromQuickSlot(CItem* item)
{
	int i, j;
	for (i=0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		for (j=0; j < QUICKSLOT_MAXSLOT; j++)
		{
			if (item == m_quickSlot[i].m_item[j])
			{
				m_quickSlot[i].m_item[j] = NULL;
				m_quickSlot[i].m_slotType[j] = QUICKSLOT_TYPE_EMPTY;
				CNetMsg slotMsg;
				QuickSlotAddMsg(slotMsg, this, (char)i, (char)j);
				SEND_Q(slotMsg, m_desc);
			}
		}
	}
}

void CPC::RemoveSkillFromQuickSlot(CSkill* pSkill)
{
	int i, j;
	for (i=0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		for (j=0; j < QUICKSLOT_MAXSLOT; j++)
		{
			if (pSkill->m_proto->m_index == m_quickSlot[i].m_skillType[j])
			{
				m_quickSlot[i].m_skillType[j] = -1;
				m_quickSlot[i].m_slotType[j] = QUICKSLOT_TYPE_EMPTY;

				CNetMsg slotMsg;
				QuickSlotAddMsg(slotMsg, this, (char)i, (char)j);
				SEND_Q(slotMsg, m_desc);
			}
		}
	}
}

void CPC::SetMaxWeight(bool bReset)
{
	// 캐릭터 무게 셋팅
	if (bReset)
		m_maxWeight = 0;
	m_maxWeight += (m_str + 300) * 20;
	m_maxWeight += m_maxWeight * m_opIncreaseInven / 100;
}

bool CPC::CheckWeight(LONGLONG weight, bool bSendWarning, bool bCheckWeight, bool bCalcStatus)
{
	if (!bCheckWeight)
		bSendWarning = false;

	if (weight == 0)
		return true;

	if (weight < 0)
		bCheckWeight = false;

	weight += m_weight;
	
	// 150% 초과시에는 false
	if (weight > m_maxWeight * 15 / 10)
	{
		if (bSendWarning)
		{
			CNetMsg errMsg;
			SysMsg(errMsg, MSG_SYS_OVER_WEIGHT);
			SEND_Q(errMsg, m_desc);
		}
		if (bCheckWeight)
			return false;
	}
	
	m_weight = weight;

	if (bCalcStatus)
		CalcStatus(true);
	
	int overload = weight - m_maxWeight;
	if (overload > 0)
	{
		if (bSendWarning)
		{
			CNetMsg errMsg;
			SysMsg(errMsg, MSG_SYS_WARNING_WEIGHT);
			SEND_Q(errMsg, m_desc);
		}
	}
	
	m_bChangeStatus = true;
	
	return true;
}

void CPC::RecoverPC()
{
	if (DEAD(this))
		return ;

	if (m_hp == m_maxHP && m_mp == m_maxMP)
		return ;
	
	int rhp = m_recoverHP;
	int rmp = m_recoverMP;

	if (IsSetPlayerState(PLAYER_STATE_MOVING) || IsCombatMode() || m_currentSkill != NULL)
	{
		// 이동/공격
		rhp = 1;
		rmp = 1;
	}
	else if (!IsSetPlayerState(PLAYER_STATE_SITDOWN) && !(m_assist.m_state & AST_FAKEDEATH))
	{
		// 앉기 외에
		rhp = rhp * 25 / 100;
		rmp = rmp * 25 / 100;
	}
	rhp += m_avPassiveAddition.recoverhp;
	rhp += rhp * m_avPassiveRate.recoverhp / SKILL_RATE_UNIT;
	rmp += m_avPassiveAddition.recovermp;
	rmp += rmp * m_avPassiveRate.recovermp / SKILL_RATE_UNIT;
	rhp += m_assist.m_avAddition.recoverhp;
	rhp += rhp * m_assist.m_avRate.recoverhp / SKILL_RATE_UNIT;
	rmp += m_assist.m_avAddition.recovermp;
	rmp += rmp * m_assist.m_avRate.recovermp / SKILL_RATE_UNIT;


	if (m_opRecoverHP > 0)
		rhp += rhp * m_opRecoverHP / 100;
	if (m_opRecoverMP > 0)
		rmp += rmp * m_opRecoverMP / 100;

	
	m_hp += rhp;
	m_mp += rmp;
	
	if (m_hp > m_maxHP || m_hp <= 0)
		m_hp = m_maxHP;
	if (m_mp > m_maxMP || m_mp <= 0)
		m_mp = m_maxMP;
	
	m_bChangeStatus = true;

	// 소환수 회복
	CNetMsg rmsg;
	CElemental* pElemental = m_elementalList;
	while (pElemental)
	{
		if (pElemental->m_hp < pElemental->m_maxHP)
		{
			pElemental->m_hp += 1;
			CharStatusMsg(rmsg, pElemental, 0);
			pElemental->m_pArea->SendToCell(rmsg, pElemental, true);
			ExElementalStatusMsg(rmsg, pElemental);
			SEND_Q(rmsg, m_desc);
		}
		pElemental = pElemental->m_nextElemental;
	}
}

void CPC::GoMemPos(int slot)
{
	if (m_mempos.m_data[slot] == NULL)
		return ;
	
	int zone = m_mempos.m_data[slot]->m_zone;
	char ylayer = m_mempos.m_data[slot]->m_ylayer;
	float x = m_mempos.m_data[slot]->m_x;
	float z = m_mempos.m_data[slot]->m_z;
	
	GoZone(this, zone, ylayer, x, z);
}

#ifdef PRIMIUM_MEMORYBOOK
void CPC::GoMemPosPlus(int slot)
{
	if (m_memposplus.m_data[slot] == NULL)
		return ;
	
	int zone = m_memposplus.m_data[slot]->m_zone;
	char ylayer = m_memposplus.m_data[slot]->m_ylayer;
	float x = m_memposplus.m_data[slot]->m_x;
	float z = m_memposplus.m_data[slot]->m_z;
	
	GoZone(this, zone, ylayer, x, z);
}
#endif	// PRIMIUM_MEMORYBOOK

void CPC::CalcStatus(bool bSend)
{
	// 원상태로
	m_maxHP = m_dbHP;
	m_maxMP = m_dbMP;
	m_str = m_dbStr;
	m_dex = m_dbDex;
	m_int = m_dbInt;
	m_con = m_dbCon;
	m_runSpeed = run_speed[(int)m_job];
	m_walkSpeed = walk_speed[(int)m_job];
#ifdef NIGHT_SHADOW
	m_flySpeed = 15.0f;
#endif // NIGHT_SHADOW

	m_attackSpeed = 20;
	m_magicSpeed = 0;
	m_skillSpeed = 0;
	m_statPall = 0;
	// << 071226 kjban add
	m_statPall_per = 0;
	// >>

	m_attribute.Reset();

	switch (m_job)
	{
	case JOB_TITAN:
		m_attackRange = ATTACK_RANGE_TITAN;
		m_recoverHP = ((80 * 3 / 2) + m_level) / 10;
		m_recoverMP = ((120 * 1) + m_level) / 10;
		break;
	case JOB_KNIGHT:
		m_attackRange = ATTACK_RANGE_KNIGHT;
		m_recoverHP = ((80 * 3 / 2) + m_level) / 10;
		m_recoverMP = ((120 * 1) + m_level) / 10;
		break;
	case JOB_HEALER:
		m_attackRange = ATTACK_RANGE_HEALER;
		m_recoverHP = ((80 * 3 / 2) + m_level) / 10;
		m_recoverMP = ((120 * 3 / 2) + m_level) / 10;
		break;
	case JOB_MAGE:
		m_attackRange = ATTACK_RANGE_MAGE;
		m_recoverHP = ((80 * 1) + m_level) / 10;
		m_recoverMP = ((120 * 2) + m_level) / 10;
		break;
	case JOB_ROGUE:
		if (m_wearing[WEARING_WEAPON] && m_wearing[WEARING_WEAPON]->IsRangeWeapon())
			m_attackRange = ATTACK_RANGE_ROGUE_2;
		else
			m_attackRange = ATTACK_RANGE_ROGUE_1;
		m_recoverHP = ((80 * 3 / 2) + m_level) / 10;
		m_recoverMP = ((120 * 1) + m_level) / 10;
		break;
	case JOB_SORCERER:
		m_attackRange = ATTACK_RANGE_SORCERER;
		switch (m_evocationType)
		{
		case EVOCATION_HELLOUND:
			m_attackRange = ATTACK_RANGE_SORCERER_HELLOUND;
			break;
		case EVOCATION_ELENEN:
			m_attackRange = ATTACK_RANGE_SORCERER_ELENEN;
			break;
		}
		m_recoverHP = ((80 * 1) + m_level) / 10;
		m_recoverMP = ((120 * 1) + m_level) / 10;
		break;
#ifdef NIGHT_SHADOW
	case JOB_NIGHTSHADOW :
		{
			m_attackRange = ATTACK_RANGE_NIGHTSHADOW ;
			m_recoverHP = ((80 * 1) + m_level) / 10;
			m_recoverMP = ((120 * 2) + m_level) / 10;
		} break;
#endif //NIGHT_SHADOW
	}

	m_opStr = 0;
	m_opDex = 0;
	m_opInt = 0;
	m_opCon = 0;

	m_opMelee = 0;
	m_opRange = 0;
	m_opMeleeHitRate = 0;
	m_opRangeHitRate =0;

	m_opDMelee = 0;
	m_opDRange = 0;
	m_opMeleeAvoid = 0;
	m_opRangeAvoid =0;

	m_opMagic = 0;
	m_opMagicHitRate = 0;
	m_opResist = 0;
	m_opResistAvoid = 0;

#ifdef ENABLE_WAR_CASTLE
	m_opSturnLevel = 0;
	m_opBloodLevel = 0;
	m_opPoisonLevel = 0;
	m_opSlowLevel = 0;
	m_opMoveLevel = 0;

	m_opSturnIndex = 0;
	m_opBloodIndex = 0;
	m_opPoisonIndex = 0;
	m_opSlowIndex = 0;
	m_opMoveIndex = 0;
#endif

	m_opIncreaseInven = 0;
	m_opMPSteal = 0;
	m_opHPSteal = 0;
	m_opAttackBlind = 0;
	m_opAttackPoison = 0;
	m_opAttackCritical = 0;
	m_opRecoverHP = 0;
	m_opRecoverMP = 0;
	m_opDecreaseSkillDelay = 0;
	m_opDecreaseSkillMP = 0;
	m_opResistStone = 0;
	m_opResistSturn = 0;
	m_opResistSilent = 0;
	m_opBlocking = 0;

	m_plusStrong = 0;
	m_plusHard = 0;
	m_plusWeak = 0;

	m_maxWeight = 0;

	// 아이템 적용
	ApplyItemValue(bSend);

	// 패시브 스킬 적용
	ApplyPassive();

#ifdef NEW_UI
	// runspeed
	m_runSpeed_org = m_runSpeed;
	// attackspeed
	m_attackSpeed_org = m_attackSpeed;
	// hp, mp
	m_maxHP_org = m_maxHP;
	m_maxMP_org = m_maxMP;
#endif // NEW_UI

	m_assist.Apply();

	if(m_statPall)
	{
		if(m_job2 == 0)
		{
			m_maxHP += m_statPall * levelup_hp[(int)m_job][0]; //힘
			m_maxMP += m_statPall * levelup_mp[(int)m_job][2]; //지혜
			m_maxHP += m_statPall * levelup_hp[(int)m_job][3]; //체질
			m_maxMP += m_statPall * levelup_mp[(int)m_job][3]; //체질
		}
		else
		{
			m_maxHP += m_statPall * levelup_hp2[ (int)m_job*2 + (int)m_job2 -1 ]; //체질
			m_maxMP += m_statPall * levelup_mp2[ (int)m_job*2 + (int)m_job2 -1 ]; //체질
		}

		if(m_maxHP < 1)
			m_maxHP = 1;
		if(m_maxMP < 1)
			m_maxMP = 1;

		if(m_hp > m_maxHP)
			m_hp = m_maxHP;
		if(m_mp > m_maxMP)
			m_mp = m_maxMP;

		m_opStr += m_statPall;
		m_opDex += m_statPall;
		m_opInt += m_statPall;
		m_opCon += m_statPall;
	}
	// << 071226 kjban add
	if(m_statPall_per)
	{
		if(m_job2 == 0)
		{
			m_maxHP += levelup_hp[(int)m_job][0] * m_str*m_statPall_per/100; //힘
			m_maxMP += levelup_mp[(int)m_job][2] * m_int*m_statPall_per/100; //지혜
			m_maxHP += levelup_hp[(int)m_job][3] * m_con*m_statPall_per/100; //체질
			m_maxMP += levelup_mp[(int)m_job][3] * m_con*m_statPall_per/100; //체질
		}
		else
		{
			m_maxHP += levelup_hp2[ (int)m_job*2 + (int)m_job2 -1 ] * m_con*m_statPall_per/100; //체질
			m_maxMP += levelup_mp2[ (int)m_job*2 + (int)m_job2 -1 ] * m_con*m_statPall_per/100; //체질
		}

		if(m_maxHP < 1)
			m_maxHP = 1;
		if(m_maxMP < 1)
			m_maxMP = 1;

		if(m_hp > m_maxHP)
			m_hp = m_maxHP;
		if(m_mp > m_maxMP)
			m_mp = m_maxMP;

		m_opStr += m_str*m_statPall_per/100;
		m_opDex += m_dex*m_statPall_per/100;
		m_opInt += m_int*m_statPall_per/100;
		m_opCon += m_con*m_statPall_per/100;
	}
	// >>
	// 무게 적용
	SetMaxWeight(false);
	if (m_weight > m_maxWeight)
	{
		m_dex -= (m_weight - m_maxWeight) / 30;
		if (m_dex < 0)
			m_dex = 0;
	}

	// 맨손은 원래 속도로
	if (m_wearing[WEARING_WEAPON] == NULL)
		m_attackSpeed = 20;

	// TODO : 하드코딩
	if (m_admin > 0 && m_speedup != 0)
	{
		m_runSpeed = m_speedup / 10.0f;
	}
	// --- TODO : 하드코딩
	
	// 강신 적용 : HP
	switch (m_evocationType)
	{
	case EVOCATION_HELLOUND:
		m_maxHP = m_maxHP + 2 * m_con;
		break;
	case EVOCATION_ELENEN:
		m_maxHP = m_maxHP + m_con * 15 / 10;
		break;
	}

	m_bChangeStatus = true;

#if defined ( LC_JPN ) 	
	if (GetMapAttr() == MAPATT_FREEPKZONE && !IsSetPlayerState(PLAYER_STATE_PKMODEDELAY | PLAYER_STATE_PKMODE))			
#else
	if (GetMapAttr() == MAPATT_FREEPKZONE && m_level > PKMODE_LIMIT_LEVEL && !IsSetPlayerState(PLAYER_STATE_PKMODEDELAY | PLAYER_STATE_PKMODE))
#endif
	{
		ResetPlayerState(PLAYER_STATE_PKMODEDELAY | PLAYER_STATE_PKMODE);
		SetPlayerState(PLAYER_STATE_PKMODE);
		//CancelInvisible();

		CNetMsg rmsg;
		ActionMsg(rmsg, this, ACTION_GENERAL, AGT_PKMODE);
		m_pArea->SendToCell(rmsg, this, true);
	}

}

bool CPC::UseStatPoint(MSG_STATPOINT_USE_TYPE type, int* val)
{
	// 남은 스탯 포인트 검사
	if (m_statpt_remain < 1)
		return false;
	
	// 스탯 포인트 증가
	int j;
	switch (type)
	{
	case MSG_STATPOINT_USE_STR:
		j = 0;
		m_statpt_str++;
		m_dbStr++;
		*val = 1;
		break;
	case MSG_STATPOINT_USE_DEX:
		j = 1;
		m_statpt_dex++;
		m_dbDex++;
		*val = 1;
		break;
	case MSG_STATPOINT_USE_INT:
		j = 2;
		m_statpt_int++;
		m_dbInt++;
		*val = 1;
		break;
	case MSG_STATPOINT_USE_CON:
		j = 3;
		m_statpt_con++;
		m_dbCon++;
		*val = 1;
		break;
	default:
		return false;
	}
	
	m_statpt_remain--;

	InitStat();
	
	CalcStatus(true);
	
	return true;
}

void CPC::ApplyItemValue(bool bSend)
{
	// TODO : 플러스 수치/옵션 고려
	CItem* item;
	int i, j;

	// 무기
	item = m_wearing[WEARING_WEAPON];
	m_eqMelee = m_eqRange = m_eqMagic = 0;
	if (item && !(item->m_flag & FLAG_ITEM_SEALED))
	{
		if (!item)
		{
		}
#ifdef ENABLE_WAR_CASTLE
		// 성주 무기는 따로 처리
		else if (item->m_itemProto->m_flag & ITEM_FLAG_LORD)
		{
			m_eqMelee = m_eqRange = m_eqMagic = 0;

			switch (item->m_itemProto->m_subtypeIdx)
			{
#if defined (LC_BRZ)
			case IWEAPON_BIGSWORD:
				m_eqMelee = (int)(20 + ((m_level - 1 + 5) * 3) * pow(1.06, 12));
				break;

			case IWEAPON_AXE:
				m_eqMelee = (int)(23 + ((m_level - 1 + 5) * 3.3) * pow(1.06, 12));
				break;

			case IWEAPON_TWOSWORD:
				m_eqMelee = (int)(19 + ((m_level - 1 + 5) * 2.8) * pow(1.06, 12));
				break;

			case IWEAPON_NIGHT:
				m_eqMelee = (int)(16 + ((m_level - 1 + 5) * 2.4) * pow(1.06, 12));
				break;

			case IWEAPON_BOW:
				m_eqRange = (int)(14 + ((m_level - 1 + 5) * 2.5) * pow(1.06, 12));
				break;

			case IWEAPON_WAND:
				m_eqMagic = (int)(10 + ((m_level - 1 + 5) * 1.9) * pow(1.06, 12));
				break;

			case IWEAPON_STAFF:
				m_eqMagic = (int)(10 + ((m_level - 1 + 5) * 3.5) * pow(1.06, 12));
				break;

			case IWEAPON_SHORTSTAFF:
				m_eqMagic = (int)(10 + ((m_level - 1 + 5) * 3) * pow(1.06, 12));
				break;

			case IWEAPON_SHORTGUM:
				m_eqMelee = (int)(10 + ((m_level - 1 + 5) * 2.9) * pow(1.06, 12));
				break;

			case IWEAPON_CROSSBOW:
				m_eqRange = (int)(10 + ((m_level - 1 + 5) * 2.5) * pow(1.06, 12));
				break;

			case IWEAPON_SCYTHE:
				m_eqMagic = (int)(18 + ((m_level - 1 + 5) * 1.5) * pow(1.06, 12));
				break;

			case IWEAPON_POLEARM:
				m_eqMagic = (int)(20 + ((m_level - 1 + 5) * 2.1) * pow(1.06, 12));
				break;
#else
			case IWEAPON_BIGSWORD:
				m_eqMelee = (int)(20 + ((m_level - 1) * 3) * pow(1.06, 12));
				break;

			case IWEAPON_AXE:
				m_eqMelee = (int)(23 + ((m_level - 1) * 3.3) * pow(1.06, 12));
				break;

			case IWEAPON_TWOSWORD:
				m_eqMelee = (int)(19 + ((m_level - 1) * 2.8) * pow(1.06, 12));
				break;

			case IWEAPON_NIGHT:
				m_eqMelee = (int)(16 + ((m_level - 1) * 2.4) * pow(1.06, 12));
				break;

			case IWEAPON_BOW:
				m_eqRange = (int)(14 + ((m_level - 1) * 2.5) * pow(1.06, 12));
				break;

			case IWEAPON_WAND:
				m_eqMagic = (int)(10 + ((m_level - 1) * 1.9) * pow(1.06, 12));
				break;

			case IWEAPON_STAFF:
				m_eqMagic = (int)(10 + ((m_level - 1) * 3.5) * pow(1.06, 12));
				break;

			case IWEAPON_SHORTSTAFF:
				m_eqMagic = (int)(10 + ((m_level - 1) * 3) * pow(1.06, 12));
				break;

			case IWEAPON_SHORTGUM:
				m_eqMelee = (int)(10 + ((m_level - 1) * 2.9) * pow(1.06, 12));
				break;

			case IWEAPON_CROSSBOW:
				m_eqRange = (int)(10 + ((m_level - 1) * 2.5) * pow(1.06, 12));
				break;

			case IWEAPON_SCYTHE:
				m_eqMagic = (int)(18 + ((m_level - 1) * 1.5) * pow(1.06, 12));
				break;

			case IWEAPON_POLEARM:
				m_eqMagic = (int)(20 + ((m_level - 1) * 2.1) * pow(1.06, 12));
				break;
#endif // LC_BRZ
			}
			m_attackSpeed = item->m_itemProto->m_num2;
		}
#endif // #ifdef ENABLE_WAR_CASTLE

		else
		{
			bool bNormalItem = true;
			CItem*	pItemNormal = item;
			int		nNormalPlus = item->m_plus;

#ifdef ENABLE_PET
			static const int nPetUniqueItemCount = 12;
			static const int nPetUniqueItemIndex[nPetUniqueItemCount] = {
				887,	// 한손검
				907,	// 이도류
				908,	// 대검
				909,	// 도끼
				910,	// 힐러완드
				911,	// 활
				912,	// 숏스테프
				913,	// 스테프
				914,	// 단검
				915,	// 석궁
				916,	// 사이드
				917		// 폴암
			};
			for (i = 0; i < nPetUniqueItemCount; i++)
			{
				if (item->m_idNum == nPetUniqueItemIndex[i])
					break;
			}
			if (i != nPetUniqueItemCount)
			{
				bNormalItem = false;
				m_eqMelee = m_eqRange = m_eqMagic = 0;
				m_attackSpeed = item->m_itemProto->m_num2;
				switch (item->m_itemProto->m_subtypeIdx)
				{
				case IWEAPON_BIGSWORD:
					m_eqMelee = (int)(20 + ((m_level - 1) * 3) * pow(1.06, 8));
					break;

				case IWEAPON_AXE:
					m_eqMelee = (int)(23 + ((m_level - 1) * 3.3) * pow(1.06, 8));
					break;

				case IWEAPON_TWOSWORD:
					m_eqMelee = (int)(19 + ((m_level - 1) * 2.8) * pow(1.06, 8));
					break;

				case IWEAPON_NIGHT:
					m_eqMelee = (int)(16 + ((m_level - 1) * 2.4) * pow(1.06, 8));
					break;

				case IWEAPON_BOW:
					m_eqRange = (int)(14 + ((m_level - 1) * 2.5) * pow(1.06, 8));
					break;

				case IWEAPON_WAND:
					m_eqMagic = (int)(10 + ((m_level - 1) * 1.9) * pow(1.06, 8));
					break;

				case IWEAPON_SHORTSTAFF:
					m_eqMagic = (int)(10 + ((m_level - 1) * 3) * pow(1.06, 8));
					break;

				case IWEAPON_CROSSBOW:
					m_eqRange = (int)(10 + ((m_level - 1) * 2.5) * pow(1.06, 8));
					break;

				case IWEAPON_SHORTGUM:
					m_eqMelee = (int)(10 + ((m_level - 1) * 2.9) * pow(1.06, 8));
					break;

#if defined ( LC_KOR ) || defined ( LC_TLD )
				case IWEAPON_STAFF:
					m_eqMagic = (int)(14 + ((m_level - 1) * 3.5) * pow(1.06, 8));
					break;

				case IWEAPON_SCYTHE:
					m_eqMagic = (int)(20 + ((m_level - 1) * 2.9) * pow(1.06, 8));
					break;

				case IWEAPON_POLEARM:
					m_eqMagic = (int)(18 + ((m_level - 1) * 2.2) * pow(1.06, 8));
					break;
#else
				case IWEAPON_STAFF:
					m_eqMagic = (int)(10 + ((m_level - 1) * 3.5) * pow(1.06, 8));
					break;

				case IWEAPON_SCYTHE:
					m_eqMagic = (int)(18 + ((m_level - 1) * 1.5) * pow(1.06, 8));
					break;

				case IWEAPON_POLEARM:
					m_eqMagic = (int)(20 + ((m_level - 1) * 2.1) * pow(1.06, 8));
					break;
#endif // LC_KOR
				default:
					break;
				}
			}
#endif // #ifdef ENABLE_PET

			// 결합 아이템
			if (bNormalItem && (item->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE) && (item->m_flag && FLAG_ITEM_COMPOSITION))
			{
				CItem* pItemEquip = m_invenNormal.GetItem(item->m_nCompositeItem);
				if (pItemEquip)
				{
					pItemNormal = pItemEquip;
				}
				if (nNormalPlus > 15)
					nNormalPlus = 15;
			}

			if (bNormalItem && pItemNormal)
			{
				// +11부터는 1.07, 그외 1.06
				double plusFactor = 1.06;
				if (nNormalPlus >= 11)
					plusFactor = 1.07;

				// 해당 값만 설정
				if (pItemNormal->IsRangeWeapon())
				{
					// 활이면 원거리
					m_eqRange = (int)(pItemNormal->m_itemProto->m_num0 * pow(plusFactor, nNormalPlus));
				}
				else
				{
					// 그외 무기는 모두 근접
					m_eqMelee = (int)(pItemNormal->m_itemProto->m_num0 * pow(plusFactor, nNormalPlus));
				}
				m_eqMagic = (int)(pItemNormal->m_itemProto->m_num1 * pow(plusFactor, nNormalPlus));
				m_attackSpeed = pItemNormal->m_itemProto->m_num2;

				bool bIsRare = false;
#ifdef MONSTER_RAID_SYSTEM
				if (pItemNormal->IsRareItem())
				{
					bIsRare = true;
				}
#endif // MONSTER_RAID_SYSTEM

				if (bIsRare)
				{
#ifdef MONSTER_RAID_SYSTEM
					if (pItemNormal->m_pRareOptionProto)
					{
						for (i = 0; i < MAX_RARE_OPTION_SETTING; i++)
						{
							const RAREOPTIONDATA* rod = pItemNormal->m_pRareOptionProto->GetData(i);
							if (pItemNormal->m_nRareOptionBit & rod->rarebit)
							{
								COption::ApplyOptionValue(this, rod->optiontype, rod->optionval, pItemNormal);
							}
						}
						// 스탯 상승 적용
						COption::ApplyOptionValue(this, OPTION_DAMAGE_UP,  pItemNormal->m_pRareOptionProto->GetDamageUp(), NULL);
						COption::ApplyOptionValue(this, OPTION_DEFENSE_UP, pItemNormal->m_pRareOptionProto->GetDefenseUp(), NULL);
						COption::ApplyOptionValue(this, OPTION_MAGIC_UP,   pItemNormal->m_pRareOptionProto->GetMagicUp(), NULL);
						COption::ApplyOptionValue(this, OPTION_RESIST_UP,  pItemNormal->m_pRareOptionProto->GetResistUp(), NULL);
					}
#endif // MONSTER_RAID_SYSTEM
				}
#ifdef SET_ITEM
				else if (pItemNormal->IsOriginItem())
				{
					ApplyOriginOption(pItemNormal);
					pItemNormal->m_nSetOption = 0;
				}
#endif
				else
				{
					for (i=0; i < MAX_WEAPON_OPTION; i++)
					{
						if (pItemNormal->m_option[i].m_level <= 0)
							continue;
						pItemNormal->m_option[i].ApplyOptionValue(this, pItemNormal);
					}
				}

				// 공격력 상승 : 소서러 추가시 마공
				if (nNormalPlus >= 6)
				{
					switch (pItemNormal->m_itemProto->m_subtypeIdx)
					{
					case IWEAPON_NIGHT:
					case IWEAPON_CROSSBOW:
					case IWEAPON_BIGSWORD:
					case IWEAPON_AXE:
					case IWEAPON_BOW:
					case IWEAPON_SHORTGUM:
					case IWEAPON_TWOSWORD:
					case IWEAPON_SOUL:
						if (pItemNormal->IsRangeWeapon())
							m_eqRange += 6;
						else
							m_eqMelee += 6;
						break;

					case IWEAPON_WAND:
					case IWEAPON_STAFF:
					case IWEAPON_SHORTSTAFF:
					case IWEAPON_SCYTHE:
					case IWEAPON_POLEARM:
						m_eqMagic += 4;
						break;
					}
				}

				// 명중 or 사정거리 증가 : 소서러 추가시 사거리
				if (nNormalPlus >=8)
				{
					switch (pItemNormal->m_itemProto->m_subtypeIdx)
					{
					case IWEAPON_NIGHT:
					case IWEAPON_BIGSWORD:
					case IWEAPON_AXE:
					case IWEAPON_SHORTGUM:
					case IWEAPON_TWOSWORD:
					case IWEAPON_SCYTHE:
					case IWEAPON_POLEARM:
					case IWEAPON_SOUL:
						// 옵션의 확률 단위는 %
						m_opMeleeHitRate += 3;
						m_opRangeHitRate += 3;
						m_opMagicHitRate += 3;
						break;

					case IWEAPON_CROSSBOW:
					case IWEAPON_STAFF:
					case IWEAPON_SHORTSTAFF:
					case IWEAPON_BOW:
					case IWEAPON_WAND:
						// 기본공격만 2m 증가
						m_attackRange += 2.0f;
						break;
					}
				}

				// 강공 확률 증가
				if (nNormalPlus >= 10)
					m_plusStrong += 300;

				// 매강공 확률 증가
				if (nNormalPlus >= 12)
					m_plusHard += 400;

				// +15이상은 추가 공격력
				if (nNormalPlus >= 15)
				{
					switch (pItemNormal->m_itemProto->m_subtypeIdx)
					{
					case IWEAPON_NIGHT:
					case IWEAPON_CROSSBOW:
					case IWEAPON_BIGSWORD:
					case IWEAPON_AXE:
					case IWEAPON_BOW:
					case IWEAPON_SHORTGUM:
					case IWEAPON_TWOSWORD:
					case IWEAPON_SOUL:
						if (pItemNormal->IsRangeWeapon())
							m_eqRange += 75;
						else
							m_eqMelee += 75;
						break;

					case IWEAPON_WAND:
					case IWEAPON_STAFF:
					case IWEAPON_SHORTSTAFF:
					case IWEAPON_SCYTHE:
					case IWEAPON_POLEARM:
						m_eqMagic += 50;
						break;
					}
				}
			}
		}
	}
	
	// 방어구
	m_eqDefense = 0;
	m_eqResist = 0;
#ifdef ENABLE_BACKWING
	const int amor_count = 7;
	static int armors[] = {WEARING_HELMET, WEARING_ARMOR_UP, WEARING_ARMOR_DOWN, WEARING_SHIELD, WEARING_GLOVE, WEARING_BOOTS, WEARING_BACKWING };
#else
	const int amor_count = 6;
	static int armors[] = {WEARING_HELMET, WEARING_ARMOR_UP, WEARING_ARMOR_DOWN, WEARING_SHIELD, WEARING_GLOVE, WEARING_BOOTS};
#endif //ENABLE_BACKWING
	for (i = 0; i < amor_count; i++)
	{
		item = m_wearing[armors[i]];
		if (item && item->IsArmorType() && !(item->m_flag & FLAG_ITEM_SEALED))
		{
			bool bNormalItem = true;
			CItem*	pItemNormal = item;
			int		nNormalPlus = item->m_plus;

#ifdef ENABLE_PET
			static const int nPetUniqueItemCount = 31;
			// itemIndex, f1, f2 : (f1 + (f2 * level)) * pow(1.06, 6)
			static const int nPetUniqueItemIndex[nPetUniqueItemCount][3] = {
				{918,  10, 20},			// 헬멧,타이탄
				{919,  16, 20},			// 상의,
				{920,  12, 20},			// 하의,
				{921,  10, 20},			// 부츠,
				{922,   8, 20},			// 장갑,
				{923,  14, 15},			// 헬멧,나이트
				{924,  20, 15},			// 상의,
				{925,  16, 15},			// 하의,
				{926,  14, 15},			// 부츠,
				{927,  14, 15},			// 장갑,
				{1280, 24, 30},			// 방패,
				{928,  10, 10},			// 헬멧,메이지
				{929,  12, 15},			// 상의,
				{930,  12, 15},			// 하의,
				{931,  10, 20},			// 부츠,
				{932,  10, 20},			// 장갑,
				{933,  14, 15},			// 헬멧,로그
				{934,  20, 20},			// 상의,
				{935,  16, 20},			// 하의,
				{936,  14, 15},			// 부츠,
				{937,  14, 15},			// 장갑,
				{938,  10, 10},			// 헬멧,힐러
				{939,  12, 10},			// 상의,
				{940,  12, 10},			// 하의,
				{941,  10, 10},			// 부츠,
				{942,  10, 10},			// 장갑,
				{943,  12, 16},			// 헬멧,소서러
				{944,  14, 18},			// 상의,
				{945,  14, 18},			// 하의,
				{946,  12, 16},			// 부츠,
				{947,  12, 16},			// 장갑,
			};
			int j;
			for (j = 0; j < nPetUniqueItemCount; j++)
			{
				if (item->m_idNum == nPetUniqueItemIndex[j][0])
					break;
			}
			if (j != nPetUniqueItemCount)
			{
				bNormalItem = false;
				m_eqDefense += (int)((nPetUniqueItemIndex[j][1] + (nPetUniqueItemIndex[j][2] * m_level / 10)) * pow(1.06, 6));
//				m_eqResist  += (int)((nPetUniqueItemIndex[j][1] + (nPetUniqueItemIndex[j][2] * m_level / 10)) * pow(1.06, 6));
			}
#endif // #ifdef ENABLE_PET

			// 결합 아이템
			if (bNormalItem && (item->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE) && (item->m_flag && FLAG_ITEM_COMPOSITION))
			{
				CItem* pItemEquip = m_invenNormal.GetItem(item->m_nCompositeItem);
				if (pItemEquip)
				{
					pItemNormal = pItemEquip;
				}
				if (nNormalPlus > 15)
					nNormalPlus = 15;
			}

			if (bNormalItem)
			{
				// +11부터는 1.07, 그외 1.06
				double plusFactor = 1.06;
				if (nNormalPlus >= 11)
					plusFactor = 1.07;

				m_eqDefense += (int)(pItemNormal->m_itemProto->m_num0 * pow(plusFactor, nNormalPlus));
				m_eqResist += (int)(pItemNormal->m_itemProto->m_num1 * pow(plusFactor, nNormalPlus));

				bool bIsRare = false;
#ifdef MONSTER_RAID_SYSTEM
				if (pItemNormal->IsRareItem())
					bIsRare = true;
#endif // MONSTER_RAID_SYSTEM

				if (bIsRare)
				{
#ifdef MONSTER_RAID_SYSTEM
					if (pItemNormal->m_pRareOptionProto)
					{
						for (j = 0; j < MAX_RARE_OPTION_SETTING; j++)
						{
							const RAREOPTIONDATA* rod = pItemNormal->m_pRareOptionProto->GetData(j);
							if (pItemNormal->m_nRareOptionBit & rod->rarebit)
							{
								COption::ApplyOptionValue(this, rod->optiontype, rod->optionval, pItemNormal);
							}
						}
						// 스탯 상승 적용
						COption::ApplyOptionValue(this, OPTION_DAMAGE_UP,  pItemNormal->m_pRareOptionProto->GetDamageUp(), NULL);
						COption::ApplyOptionValue(this, OPTION_DEFENSE_UP, pItemNormal->m_pRareOptionProto->GetDefenseUp(), NULL);
						COption::ApplyOptionValue(this, OPTION_MAGIC_UP,   pItemNormal->m_pRareOptionProto->GetMagicUp(), NULL);
						COption::ApplyOptionValue(this, OPTION_RESIST_UP,  pItemNormal->m_pRareOptionProto->GetResistUp(), NULL);
					}
#endif // MONSTER_RAID_SYSTEM
				}
#ifdef SET_ITEM
				else if (pItemNormal->IsOriginItem())
				{
					ApplyOriginOption(pItemNormal);
					pItemNormal->m_nSetOption = 0;
				}
#endif
				else
				{
					for (j=0; j < MAX_WEAPON_OPTION; j++)
					{
						if (pItemNormal->m_option[j].m_level <= 0)
							continue;
						pItemNormal->m_option[j].ApplyOptionValue(this, pItemNormal);
					}
				}

// 050401 : bs : plus에 따른 추가 효과
				if (pItemNormal->m_itemProto->m_subtypeIdx != IWEAR_SHIELD)
				{
					// 소지무게 500 증가
					if (nNormalPlus >=6)
						m_maxWeight += 500;

					// 추가 방어력 10
					if (nNormalPlus >=8)
						m_eqDefense += 10;

					// 추가 마방 10
					if (nNormalPlus >=10)
						m_eqResist += 10;

					// 위크 확률 5%
					if (nNormalPlus >=12)
						m_plusWeak += 500;
				}
// --- 050401 : bs : plus에 따른 추가 효과

				if (nNormalPlus >= 15)
				{
					m_eqDefense += 100;
					m_eqResist += 50;
				}
			}
		}
	}

	// 악세사리
	for (i=0; i < 3; i++)
	{
		// 캐쉬 악세서리 능력치 중복 안되게
		int cashNotDup = 0;
		item = m_wearing[i + 7];
		if (item && !(item->m_flag & FLAG_ITEM_SEALED))
		{
			// 060227 : bs : 악서서리
			time_t timeCur;
			time(&timeCur);
			struct tm tmCur = *localtime(&timeCur);
			int nCurTime = tmCur.tm_hour * 100 + tmCur.tm_min;

			switch (item->m_idNum)
			{
			case 1301:		// 18:00 - 02:00, 명중 20%
				if ((nCurTime >= 1800 || nCurTime < 200) && !(cashNotDup & ( 1 << 1 )) )
				{
					m_opMeleeHitRate += (int)(m_opMeleeHitRate * 0.2);
					m_opRangeHitRate += (int)(m_opRangeHitRate * 0.2);
					m_opMagicHitRate += (int)(m_opMagicHitRate * 0.2);
					cashNotDup = cashNotDup | ( 1 << 1 );
				}
				break;

			case 1302:		// 10:00 - 18:00, 명중 20%
				if ((nCurTime >= 1000 && nCurTime < 1800) && !(cashNotDup & ( 1 << 2 )) )
				{
					m_opMeleeHitRate += (int)(m_opMeleeHitRate * 0.2);
					m_opRangeHitRate += (int)(m_opRangeHitRate * 0.2);
					m_opMagicHitRate += (int)(m_opMagicHitRate * 0.2);
					cashNotDup = cashNotDup | ( 1 << 2 );
				}
				break;

			case 1303:		// 02:00 - 10:00, 명중 20%
				if ((nCurTime >= 200 && nCurTime < 1000) && !(cashNotDup & ( 1 << 3 )) )
				{
					m_opMeleeHitRate += (int)(m_opMeleeHitRate * 0.2);
					m_opRangeHitRate += (int)(m_opRangeHitRate * 0.2);
					m_opMagicHitRate += (int)(m_opMagicHitRate * 0.2);
					cashNotDup = cashNotDup | ( 1 << 3 );
				}
				break;

			case 1304:		// 18:00 - 02:00, 회피 10%
				if ((nCurTime >= 1800 || nCurTime < 200) && !(cashNotDup & ( 1 << 4 )) )
				{
					m_opMeleeAvoid += (int)(m_opMeleeAvoid * 0.1);
					m_opRangeAvoid += (int)(m_opRangeAvoid * 0.1);
					m_opResistAvoid += (int)(m_opResistAvoid * 0.1);
					cashNotDup = cashNotDup | ( 1 << 4 );
				}
				break;

			case 1305:		// 10:00 - 18:00, 회피 10%
				if ((nCurTime >= 1000 && nCurTime < 1800) && !(cashNotDup & ( 1 << 5 )) )
				{
					m_opMeleeAvoid += (int)(m_opMeleeAvoid * 0.1);
					m_opRangeAvoid += (int)(m_opRangeAvoid * 0.1);
					m_opResistAvoid += (int)(m_opResistAvoid * 0.1);
					cashNotDup = cashNotDup | ( 1 << 5 );
				}
				break;

			case 1306:		// 02:00 - 10:00, 회피 10%
				if ( (nCurTime >= 200 && nCurTime < 1000) && !(cashNotDup & ( 1 << 6 )) )
				{
					m_opMeleeAvoid += (int)(m_opMeleeAvoid * 0.1);
					m_opRangeAvoid += (int)(m_opRangeAvoid * 0.1);
					m_opResistAvoid += (int)(m_opResistAvoid * 0.1);
					cashNotDup = cashNotDup | ( 1 << 6 );
				}
				break;

#ifdef NEW_ACCERY_ADD
			case 2039:		// 건강의 반지 HP 150증가
				if( !(cashNotDup & ( 1 << 7 )) )
				{
					m_maxHP += 150;
					cashNotDup = cashNotDup | ( 1 << 7 );
				}
				break;
			
			case 2040:		// 마나 잼  MP 150 증가
				if( !(cashNotDup & ( 1 << 8 )) )
				{
					m_maxMP += 150;
					cashNotDup = cashNotDup | ( 1 << 8 );
				}
				break;
			case 2041:		// 경험의 목걸이 획득한 경험치 5%증가
			case 2042:		// SP 부스터
				if( !(cashNotDup & ( 1 << 9 )) )
				{
					m_AddProb = 5;
					cashNotDup = cashNotDup | ( 1 << 9 );
				}
				break;
#endif // NEW_ACCERY_ADD
			default:
				{
					bool bIsRare = false;
#ifdef MONSTER_RAID_SYSTEM
					if (item->IsRareItem())
						bIsRare = true;
#endif // MONSTER_RAID_SYSTEM

					if (bIsRare)
					{
#ifdef MONSTER_RAID_SYSTEM
						if (item->m_pRareOptionProto)
						{
							for (j = 0; j < MAX_RARE_OPTION_SETTING; j++)
							{
								const RAREOPTIONDATA* rod = item->m_pRareOptionProto->GetData(j);
								if (item->m_nRareOptionBit & rod->rarebit)
								{
									COption::ApplyOptionValue(this, rod->optiontype, rod->optionval, item);
								}
							}
							// 스탯 상승 적용
							COption::ApplyOptionValue(this, OPTION_DAMAGE_UP,  item->m_pRareOptionProto->GetDamageUp(), NULL);
							COption::ApplyOptionValue(this, OPTION_DEFENSE_UP, item->m_pRareOptionProto->GetDefenseUp(), NULL);
							COption::ApplyOptionValue(this, OPTION_MAGIC_UP,   item->m_pRareOptionProto->GetMagicUp(), NULL);
							COption::ApplyOptionValue(this, OPTION_RESIST_UP,  item->m_pRareOptionProto->GetResistUp(), NULL);
						}
#endif // MONSTER_RAID_SYSTEM
					}
#ifdef SET_ITEM
					else if (item->IsOriginItem())
					{
						ApplyOriginOption(item);
						item->m_nSetOption = 0;
					}
#endif
					else
					{
						for (j=0; j < MAX_WEAPON_OPTION; j++)
						{
							if (item->m_option[j].m_level <= 0)
								continue ;
							if (item->m_itemProto->m_maxUse != -1 && item->m_used <= 0)
								continue;
							item->m_option[j].ApplyOptionValue(this, item);
						}
					}
				}
				break;
			}
		}
	}

#ifdef SET_ITEM
	for (i = 0; i < MAX_WEARING; i++)
	{
		CItem * pItem = m_wearing[i];

		if (!pItem || !pItem->m_pSetItemProto)
			continue;

		if (!pItem->m_nSetOption)
		{
			ApplySetItemValue(pItem);
		}
	}
#endif

	if (bSend)
		m_bChangeStatus = true;
}

// 착용 아이템 중 무기, 방어구에 옵션을 붙일 수 있도록 선택
int CPC::SettingItemOption()
{
	int enableIndex[WEARING_BOOTS + 1];	// 옵션 붙일 수 있는 아이템 인덱스
	int	nEnable = 0;					// 옵션 붙일 수 있는 아이템 몇개인가?

	int i;
	for (i = WEARING_HELMET; i <= WEARING_BOOTS; i++)
	{
		if (m_wearing[i])
		{
			// 무기 : 무기 최대 옵션 가능 초과 체크
			if (m_wearing[i]->IsWeaponType())
			{
				if (m_wearing[i]->m_nOption >= MAX_WEAPON_OPTION)
					continue;
				if (m_wearing[i]->m_itemProto->m_subtypeIdx == IWEAPON_MINING ||
					m_wearing[i]->m_itemProto->m_subtypeIdx == IWEAPON_GATHERING ||
					m_wearing[i]->m_itemProto->m_subtypeIdx == IWEAPON_CHARGE)
					continue;
				// 성주 전용 아이템은 제외
				if (m_wearing[i]->m_itemProto->m_flag & ITEM_FLAG_LORD)
					continue ;
			}
			// 방어구 : 방어구 최대 옵션 가능 초과 체크
			else if (m_wearing[i]->IsArmorType())
			{
				if (m_wearing[i]->m_nOption >= MAX_ARMOR_OPTION)
					continue;
			}

			// 업그레이드 불능은 블옵 불가
			if (!m_wearing[i]->CanBloodGem())
				continue ;

			// 이미 옵션 붙일 수 있는 상태인것은 제외
			if (m_wearing[i]->m_flag & FLAG_ITEM_OPTION_ENABLE)
				continue;

			// 무기나 방어구 일때만
			if (m_wearing[i]->IsWeaponType() || m_wearing[i]->IsArmorType())
			{
				enableIndex[nEnable] = i;
				nEnable++;
			}
		}
	}

	// 옵션 붙일 수 있는 아이템 없음
	if (nEnable == 0)
		return -1;

	int where = GetRandom(0, nEnable - 1);

	if (!m_wearing[enableIndex[where]])
		return -1;

	// 아이템 옵션을 붙일 수 있는 상태로 flag 값 셋팅
	m_wearing[enableIndex[where]]->m_flag |= FLAG_ITEM_OPTION_ENABLE;

	CNetMsg msg;
	SysBloodItemMsg(msg, m_wearing[enableIndex[where]]->m_idNum);
	SEND_Q(msg, m_desc);
	ItemUpdateMsg(msg, m_wearing[enableIndex[where]], 0);
	SEND_Q(msg, m_desc);

	return where;
}

int CPC::CanWarp()
{
	// 죽었을때
	if (DEAD(this))
		return MSG_SYS_CANT_WARP_STATE;
	
	// 앉아 있을때
	if (m_state & PLAYER_STATE_SITDOWN)
		return MSG_SYS_CANT_WARP_STATE;
	
	// 다른 이동장치 사용중
	if (m_state & PLAYER_STATE_WARP)
		return MSG_SYS_CANT_WARP_OTHER;
	
	// 스킬 사용중
	if (m_currentSkill)
		return MSG_SYS_CANT_WARP_STATE;
	
	// 교환 중
	if (m_exchange)
		return MSG_SYS_CANT_WARP_STATE;
	
	// 파티 신청중
	if (m_party && m_party->GetBossIndex() == m_index && m_party->GetRequestIndex() > 0)
		return MSG_SYS_CANT_WARP_STATE;
	
	return 0;
}

CSkill* CPC::FindSkill(int index)
{
	CSkill* ret;
	ret = m_activeSkillList.Find(index);
	if (ret)
		return ret;
	ret = m_passiveSkillList.Find(index);
	if (ret)
		return ret;
	ret = m_etcSkillList.Find(index);
	if (ret)
		return ret;
#ifdef FACTORY_SYSTEM
	ret = m_sealSkillList.Find(index);
	if (ret)
		return ret;
#endif
	return NULL;
}

void CPC::ApplyPassive()
{
	// 초기화
	memset(&m_avPassiveAddition, 0, sizeof(m_avPassiveAddition));
	memset(&m_avPassiveRate, 0, sizeof(m_avPassiveRate));

	// 패시브 리스트에서 적용
	CSkill* pSkill;
	void* pos = m_passiveSkillList.GetHeadPosition();
	while (pos)
	{
		pSkill = m_passiveSkillList.GetNext(pos);

		if (pSkill && pSkill->m_proto)
		{
			bool bApply = true;
			// 소서러 플래그가 있으면 강신 상태에 따라 적용 결정
			if (pSkill->m_proto->CheckSorcererFlag(SSF_APP_ALL))
			{
				switch (m_evocationType)
				{
				case EVOCATION_NONE:
					if (!pSkill->m_proto->CheckSorcererFlag(SSF_APP_CHARACTER))
						bApply = false;
					break ;
				case EVOCATION_HELLOUND:
					if (!pSkill->m_proto->CheckSorcererFlag(SSF_APP_HELLOUND))
						bApply = false;
					break;
				case EVOCATION_ELENEN:
					if (!pSkill->m_proto->CheckSorcererFlag(SSF_APP_ELENEN))
						bApply = false;
					break;
				default:
					bApply = false;
					break;
				}
			}
			if (bApply)
				ApplyPassiveSkill(pSkill, 100);
		}
	}

	// 펫의 패시브 스킬
#ifdef ENABLE_PET
	if (m_wearing[WEARING_PET])
	{
		CPet* pet = GetPet();
		// 050309: bw 펫이 멀리 있으면 스킬 적용 안함 20미터 
		if (pet && pet->GetDistanceFromOwner() < 20 && (pet->IsSummon() || pet->IsMount()))
		{
			pos = pet->GetSkillList()->GetHeadPosition();
			while (pos)
			{
				pSkill = pet->GetSkillList()->GetNext(pos);
				if (pSkill->m_proto->m_type == ST_PET_SKILL_PASSIVE)
					ApplyPassiveSkill(pSkill, pet->GetSympathyPoint() * 100 / PET_MAX_SYMPATHY);		// 교감도 비율로 적용
			}
		}
	}
#endif

	ApplyAssistData(&m_avPassiveAddition, &m_avPassiveRate);
}

int CPC::GetAttackLevel() const
{
	int attacklevel = 0;

	if (m_wearing[WEARING_WEAPON])
	{
		CItem* pItem = m_wearing[WEARING_WEAPON];
		if ((pItem->m_flag & FLAG_ITEM_COMPOSITION) && (pItem->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE))
			attacklevel = m_invenNormal.GetItemLevel(pItem->m_nCompositeItem, false);
		else
			attacklevel = pItem->GetItemLevel(false);
	}

	return attacklevel;
}

int CPC::GetDefenseLevel() const
{
	static const int armorscount = 6;
	static int armors[armorscount] = { WEARING_HELMET, WEARING_ARMOR_UP, WEARING_ARMOR_DOWN, WEARING_SHIELD, WEARING_GLOVE, WEARING_BOOTS };

	int defenselevel = 0;
	bool bWear = false;

	int i;

	for (i = 0; i < armorscount; i++)
	{
		if (m_wearing[armors[i]])
		{
			CItem* pItem = m_wearing[armors[i]];
			int nThisItemLevel = pItem->GetItemLevel(false);
			if ((pItem->m_flag & FLAG_ITEM_COMPOSITION) && (pItem->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE))
				nThisItemLevel = m_invenNormal.GetItemLevel(pItem->m_nCompositeItem, false);

			if (defenselevel < nThisItemLevel)
				defenselevel = nThisItemLevel;
			bWear = true;
		}
	}

	if (!bWear)
	{
		defenselevel = m_level * 2;
	}

	return defenselevel;
}

MSG_DAMAGE_TYPE CPC::GetAttackType(const CSkillProto* proto) const
{
	// 프로토가 MAGIC일때만 스킬 속성을 따르고
	// 그외에서는 착용 무기를 따른다
	if (proto)
	{
		switch (proto->m_type)
		{
		case ST_MAGIC:
			return MSG_DAMAGE_MAGIC;
		default:
			break;
		}
	}

	// 기본 공격이 MAGIC일 때 처리
	if (m_wearing[WEARING_WEAPON])
	{
		if (m_wearing[WEARING_WEAPON]->IsRangeWeapon())
			return MSG_DAMAGE_RANGE;
		else if (m_wearing[WEARING_WEAPON]->IsMagicWeapon())
			return MSG_DAMAGE_MAGIC;
		else
			return MSG_DAMAGE_MELEE;
	}

	return MSG_DAMAGE_MELEE;
}

int CPC::GetPKName()
{
	if (m_pkCount < -90)		return PK_NAME_CHAOSLEGION;
	else if (m_pkCount < -70)	return PK_NAME_NECROKNIGHT;
	else if (m_pkCount < -50)	return PK_NAME_OUTLAW;
	else if (m_pkCount < -30)	return PK_NAME_FEARCASTER;
	else if (m_pkCount < -10)	return PK_NAME_MURDERER;
	else if (m_pkCount <= 10)	return PK_NAME_NORMAL;
	else if (m_pkCount <= 30)	return PK_NAME_AVENGER;
	else if (m_pkCount <= 50)	return PK_NAME_LIGHTPORTER;
	else if (m_pkCount <= 70)	return PK_NAME_GUARDIAN;
	else if (m_pkCount <= 90)	return PK_NAME_SAINTKNIGHT;
	else						return PK_NAME_DIVINEPROTECTOR;
}

bool CPC::CanMove()
{
	if (IsSetPlayerState(PLAYER_STATE_SITDOWN))
		return false;
	if (IsSetPlayerState(PLAYER_STATE_WARP))
		return false;
	if (m_personalshop != NULL)
		return false;

	return CCharacter::CanMove();
}

bool CPC::CanAttack()
{
	if (IsSetPlayerState(PLAYER_STATE_SITDOWN))
		return false;
	if (IsSetPlayerState(PLAYER_STATE_WARP))
		return false;
	if (IsSetPlayerState(PLAYER_STATE_MOVING))
		return false;
	if (IsSetPlayerState(PLAYER_STATE_CHANGE))
		return false;

	return CCharacter::CanAttack();
}

bool CPC::CanSpell()
{
	if (IsSetPlayerState(PLAYER_STATE_SITDOWN))
		return false;
	if (IsSetPlayerState(PLAYER_STATE_WARP))
		return false;
// TODO : 여기 스펠 가능 여부 검사 부분 나중에 다시 부활하기
//	if (IsSetPlayerState(PLAYER_STATE_MOVING))
//		return false;

	return CCharacter::CanSpell();
}

void CPCList::SendToAll(CNetMsg& msg)
{
	int i;

	for (i = 0; i < m_max; i++)
	{
		if (m_pcList[i])
		{
			SEND_Q(msg, m_pcList[i]->m_desc);
		}
	}
}

//0627
void CPCList::SendToAllTeacher(CNetMsg& msg)
{
	int i;

	for (i = 0; i < m_max; i++)
		if (m_pcList[i] && m_pcList[i]->m_bTeacher)
		{
			SEND_Q(msg, m_pcList[i]->m_desc);
		}
}

void CPCList::SendGuildWarPointMsg(CWarCastle* castle, int zoneindex)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	int i;
	for (i = 0; i < m_max; i++)
	{
		if (m_pcList[i] && m_pcList[i]->GetJoinFlag(zoneindex) != WCJF_NONE && m_pcList[i]->m_pZone && m_pcList[i]->m_pZone->m_index == zoneindex)
		{
			GuildWarPointMsg(rmsg, m_pcList[i], castle);
			SEND_Q(rmsg, m_pcList[i]->m_desc);
		}
	}
#endif
}

void CPCList::SendToWarJoinUser(CNetMsg& msg, int zoneindex, bool bInZone)
{
#ifdef ENABLE_WAR_CASTLE
	int i;
	for (i = 0; i < m_max; i++)
	{
		if (m_pcList[i] && m_pcList[i]->GetJoinFlag(zoneindex) != WCJF_NONE)
		{
			if (!bInZone || (m_pcList[i]->m_pZone && m_pcList[i]->m_pZone->m_index == zoneindex))
				SEND_Q(msg, m_pcList[i]->m_desc);
		}
	}
#endif
}

int CPC::CanChange()
{
	// 죽었을때
	if (DEAD(this))
		return MSG_CHANGE_ERR_DEAD;
	
	// 앉아 있을때
	if (m_state & PLAYER_STATE_SITDOWN)
		return MSG_CHANGE_ERR_SITDOWN;
	
	// 다른 이동장치 사용중
	if (m_state & PLAYER_STATE_WARP)
		return MSG_CHANGE_ERR_WRAP;
	
	// 스킬 사용중
	if (m_currentSkill)
		return MSG_CHANGE_ERR_SKILL;
	
	// 교환 중
	if (m_exchange)
		return MSG_CHANGE_ERR_EXCHANGE;
	
	// 파티 신청중
	if (m_party && m_party->GetBossIndex() == m_index && m_party->GetRequestIndex() > 0)
		return MSG_CHANGE_ERR_PARTY;



	//PK중
#if defined ( FREE_PK_SYSTEM )
	if( !gserver.m_bFreePk )
	{
		if (m_state & PLAYER_STATE_PKMODE || m_state & PLAYER_STATE_RAMODE || m_state & PLAYER_STATE_PKMODEDELAY)
		return MSG_CHANGE_ERR_PK;
	}
#else
	if (m_state & PLAYER_STATE_PKMODE || m_state & PLAYER_STATE_RAMODE || m_state & PLAYER_STATE_PKMODEDELAY)
		return MSG_CHANGE_ERR_PK;
#endif // FREE_PK_SYSTEM

	// 전투중
	if (IsCombatMode())
		return MSG_CHANGE_ERR_BATTLE;

	// 개인상점중
	if (m_personalshop)
		return MSG_CHANGE_ERR_PERSONAL_SHOP;

	return -1;
}

void CPC::do_QuestGiveUp(CPC* ch, CQuest* quest)
{
	CNetMsg rMsg;


//	int idx = ch->m_questList.FindQuest(questIndex);

//	CQuest* quest = ch->m_questList.m_list[idx];

	if (!quest)
		return;

#ifdef QUEST_DATA_EXTEND
	const CQuestProto* pQuestProto = quest->GetQuestProto();
#else // QUEST_DATA_EXTEND
	const CQuestProto* pQuestProto = quest->m_proto;
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

#ifdef QUEST_DATA_EXTEND
				int nQuestValue = quest->GetQuestValue(i);
#else // QUEST_DATA_EXTEND
				int nQuestValue = quest->m_currentData[i];
#endif // QUEST_DATA_EXTEND

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
						
						// item 수량 변경
						CNetMsg itemmsg;
						DecreaseFromInventory(ch, item, nQuestValue);
						
						if (item->Count() > 0)
						{
							ItemUpdateMsg(itemmsg, item, -nQuestValue);
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

	if (pQuestProto->m_type[0] == QTYPE_KIND_DEFEAT || pQuestProto->m_type[0] == QTYPE_KIND_SAVE || pQuestProto->m_type[0] == QTYPE_KIND_TUTORIAL)
		bGoZone = true;

	// Quest GiveUp Log
	GAMELOG << init("QUEST FORCE GIVEUP", ch)
			<< pQuestProto->m_index
			<< end;

// 여기서 포기한 퀘스트 인덱스를 저장하여 다음에는 받지 못하도록 한다.
// 포기한 퀘스트를 저장한다.
#ifdef QUEST_DATA_EXTEND
	if (pQuestProto->m_type[1] == QTYPE_REPEAT_ONCE)
	{
		ch->m_questList.SetQuestState(quest, QUEST_STATE_ABANDON);
	}
	else
	{
		ch->m_questList.DelQuest(ch, quest);
	}

	QuestForceGiveUpMsg(rMsg, quest);
	SEND_Q(rMsg, ch->m_desc);
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

	QuestForceGiveUpMsg(rMsg, quest);
	ch->m_questList.DelQuest(ch, quest);
	SEND_Q(rMsg, ch->m_desc);
#endif // QUEST_DATA_EXTEND


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

bool AddToInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int row, int col)
{
	if (item == NULL)
		return true;
/*

  */
	
	//0607 만약 일반탭이라면 퀘스트 조건 아이템 체크
	int tab = GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx);

	CInventory* inven = GET_INVENTORY(ch, tab);
	if (inven == NULL)
		return true;

#ifdef SET_ITEM
	if (!inven->AddItem(item, ch, bSendWarning, bCheckWeight, false, row, col))
		return false;
#else
	if (!inven->AddItem(item, ch, bSendWarning, bCheckWeight, true, row, col))
		return false;
#endif

//0607
	int r, c;
	if ( (tab == INVENTORY_NORMAL || tab == INVENTORY_QUEST ) //  [2/28/2007 KwonYongDae]
		&& inven->FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
	{
		// 겹치는 아이템 수량 변경하고 item의 tab, row, col 세팅해서 리턴
		CItem* p = inven->GetItem(r, c);
		if (p)
		{
			CheckQuestCondition(ch, p, p->Count());
		}
	}
	
	return true;
}

bool AddToPreInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int tab, int row, int col)
{
	if (item == NULL)
		return true;		

	CInventory* inven = GET_INVENTORY(ch, tab);
	if (inven == NULL)
		return true;

	if (!inven->AddItem(item, ch, bSendWarning, bCheckWeight, true, row, col))
		return false;

	int r, c;
	if (tab == INVENTORY_NORMAL && inven->FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
	{
		// 겹치는 아이템 수량 변경하고 item의 tab, row, col 세팅해서 리턴
		CItem* p = inven->GetItem(r, c);
		if (p)
		{
			CheckQuestCondition(ch, p, p->Count());
		}
	}
	
	return true;
}

bool DecreaseFromInventory(CPC* ch, CItem* item, LONGLONG count)
{
	if (item == NULL || item->m_itemProto == NULL)
		return true;

	int tab = GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx);

	CInventory* inven = GET_INVENTORY(ch, tab);
	if (inven == NULL)
		return true;

	if (!inven->Decrease(item, count))
		return false;

		int r, c;
	if (tab == INVENTORY_NORMAL && inven->FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
	{
		// 겹치는 아이템 수량 변경하고 item의 tab, row, col 세팅해서 리턴
		CItem* p = inven->GetItem(r, c);
		if (p)
		{
			CheckQuestCondition(ch, p, p->Count());
		}
	}

	return true;
}

bool RemoveFromInventory(CPC* ch, int tab, int row, int col, bool bFreeMem, bool removeQuickSlot)
{
	CInventory* inven = GET_INVENTORY(ch, tab);
	if (inven == NULL)
		return false;

	CItem* item = inven->GetItem(row, col);

	return RemoveFromInventory(ch, item, bFreeMem, removeQuickSlot);
}

bool RemoveFromInventory(CPC* ch, CItem* item, bool bFreeMem, bool removeQuickSlot)
{
	if (item == NULL || item->m_itemProto == NULL)
		return false;

	int tab = GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx);

	CInventory* inven = GET_INVENTORY(ch, tab);
	if (inven == NULL)
		return false;

	if (tab == INVENTORY_NORMAL)
	{		
		CheckQuestCondition(ch, item, 0);
	}

	// 써치 라이프 버프 삭제
	if (item->m_itemProto->m_index >= 2461 && item->m_itemProto->m_index <= 2463)
		ch->m_assist.CureBySkillIndex(475);

	if (!inven->Remove(item, bFreeMem, removeQuickSlot))
		return false;

	return true;
}

//0608
void CheckQuestCondition(CPC* ch, CItem* item, LONGLONG itemSum)
{
	//  [2/23/2007 KwonYongDae]
	if( (item->m_itemProto->m_typeIdx == ITYPE_ETC && ( item->m_itemProto->m_subtypeIdx == IETC_MATERIAL || item->m_itemProto->m_subtypeIdx == IETC_PROCESS || item->m_itemProto->m_subtypeIdx == IETC_QUEST ) )
		|| ( ( item->m_itemProto->m_typeIdx == ITYPE_WEAPON  ) && ( item->m_itemProto->m_subtypeIdx == IWEAPON_BIGSWORD ) )  
#ifdef ATTACK_PET
		|| ( item->m_idNum == 2714 ) 
#endif //ATTACK_PET
		)
	{
		
#ifdef QUEST_DATA_EXTEND

		CQuest* pQuest;
		CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
		while ((pQuest = pQuestNext))
		{
			pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
			bool bSkip;
			switch (pQuest->GetQuestType0())
			{
			case QTYPE_KIND_REPEAT:
			case QTYPE_KIND_COLLECTION:
			case QTYPE_KIND_DELIVERY:
#ifdef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae]
			case QTYPE_KIND_PROCESS_EXPERIENCE : 
			case QTYPE_KIND_MAKE_EXPERIENCE :
#endif // EXPRIENCE_QUEST
				bSkip = false;
				break;
			default:
				bSkip = true;
				break;
			}
			if (!bSkip)
			{
				const CQuestProto* pQuestProto = pQuest->GetQuestProto();
				bool bComplete[QUEST_MAX_CONDITION];
				memset(bComplete, false, sizeof(bool) * QUEST_MAX_CONDITION);
				
				for(int j = 0; j < QUEST_MAX_CONDITION; j++)
				{
					if(pQuestProto->m_conditionType[j] == QCONDITION_ITEM_NORMAL )
					{	
						if(pQuestProto->m_conditionIndex[j] == item->m_itemProto->m_index)
						{
							// LONGLONG itemSum = item->Count();
							
							if (itemSum < pQuestProto->m_conditionNum[j])
							{
								bComplete[j] = false;
								pQuest->SetQuestValue(j, itemSum);
								if (pQuest->GetComplete2())
								{
									pQuest->SetComplete2(false);
								}
								
								CNetMsg statusMsg;
								QuestUnCompleteMsg(statusMsg, pQuest);
								SEND_Q(statusMsg, ch->m_desc);
							
								QuestStatusMsg(statusMsg, pQuest);
								SEND_Q(statusMsg, ch->m_desc);
							}
							
							// 같으면 bComplete true 셋팅
							if (itemSum >= pQuestProto->m_conditionNum[j])
							{
								bComplete[j] = true;
								
								pQuest->SetQuestValue(j, itemSum);				
								if(pQuest->GetComplete2())
								{
									pQuest->SetComplete2(false);
								}
								
								CNetMsg statusMsg;
								QuestStatusMsg(statusMsg, pQuest);
								SEND_Q(statusMsg, ch->m_desc);
								
								continue;
							}
							else if (itemSum < pQuestProto->m_conditionNum[j])
							{
								bComplete[j] = false;
							}
							else
								continue;
						}
						else if(pQuestProto->m_conditionType[j] == -1)
						{
							bComplete[j] = true;
							continue;
						}
						else if((pQuestProto->m_conditionType[j] == QCONDITION_ITEM 
							||pQuestProto->m_conditionIndex[j] == QCONDITION_NPC) && pQuest->GetComplete1())
						{
							bComplete[j] = true;
							continue;
						}
						else if(pQuestProto->m_conditionType[j] == QCONDITION_ITEM_NORMAL 
							&& (pQuestProto->m_conditionNum[j] == pQuest->GetQuestValue(j)))
						{
							bComplete[j] = true;
							continue;
						}
						else
						{
							bComplete[j] = false;
							continue;
						}
					}	
#ifdef EXPRIENCE_QUEST
					else if ( pQuestProto->m_conditionType[j] == QCONDITION_ITEM )
					{
						if(pQuestProto->m_conditionIndex[j] == item->m_itemProto->m_index)
						{
							if (itemSum >= pQuestProto->m_conditionNum[j])
							{
								bComplete[j] = true;
								
								pQuest->SetQuestValue(j, itemSum);
								pQuest->SetComplete1(true);			
								if(pQuest->GetComplete2())
								{
									pQuest->SetComplete2(false);
								}
								
								CNetMsg statusMsg;
								QuestStatusMsg(statusMsg, pQuest);
								SEND_Q(statusMsg, ch->m_desc);
								
								continue;
							}
						}
					}
#endif //EXPRIENCE_QUEST
					else
					{
						bComplete[j] = true;
					}
				}
				
				if(pQuest->IsCompleted())//이미 완료된 퀘스트는 skip
				{
					continue;
				}

				for (int k=0; k < QUEST_MAX_CONDITION; k++)
				{
					if (!bComplete[k])
						break;

					if(k == QUEST_MAX_CONDITION-1)
					{
						pQuest->SetComplete2(true);
						
						if(pQuest->GetComplete1())//퀘스트 아이템도 완료가 되었다면,
						{
							GAMELOG << init("QUEST COMPLETE (CheckQeustCondition)", ch)
									<< pQuestProto->m_index
									<< end;
							
							// 완료 메시지 
							CNetMsg completeMsg;
							QuestCompleteMsg(completeMsg, pQuest);
							SEND_Q(completeMsg, ch->m_desc);
						}
					}
				}				
			}
		}
#else // QUEST_DATA_EXTEND

		for (int i=0; i < QUEST_MAX_PC; i++)
		{
			if (!ch->m_questList.m_list[i])
				continue;
			
			if (ch->m_questList.m_list[i] && ch->m_questList.m_bQuest[i] && 
				
				(ch->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_REPEAT 
				|| ch->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_COLLECTION
				|| ch->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_DELIVERY)
			)
			{
				bool bComplete[QUEST_MAX_CONDITION];
				memset(bComplete, false, sizeof(bool) * QUEST_MAX_CONDITION);
				
				for(int j = 0; j < QUEST_MAX_CONDITION; j++)
				{
					if(ch->m_questList.m_list[i]->m_proto->m_conditionType[j] == QCONDITION_ITEM_NORMAL)
					{	
						if(ch->m_questList.m_list[i]->m_proto->m_conditionIndex[j] == item->m_itemProto->m_index)
						{
							// LONGLONG itemSum = item->Count();
							
							if (itemSum < ch->m_questList.m_list[i]->m_proto->m_conditionNum[j])
							{
								bComplete[j] = false;
								ch->m_questList.m_list[i]->m_currentData[j] = itemSum;
								
								if(ch->m_questList.m_bComplete2[i])
								{
									ch->m_questList.SetComplete2(i, false);
								}
								
								CNetMsg statusMsg;
								QuestUnCompleteMsg(statusMsg, ch->m_questList.m_list[i]);
								SEND_Q(statusMsg, ch->m_desc);
							
								QuestStatusMsg(statusMsg, ch->m_questList.m_list[i]);
								SEND_Q(statusMsg, ch->m_desc);
							}
							
							// 같으면 bComplete true 셋팅
							if (itemSum >= ch->m_questList.m_list[i]->m_proto->m_conditionNum[j])
							{
								bComplete[j] = true;
								
								ch->m_questList.m_list[i]->m_currentData[j] = itemSum;
								
								if(ch->m_questList.m_bComplete2[i])
								{
									ch->m_questList.SetComplete2(i, false);
								}
								
								CNetMsg statusMsg;
								QuestStatusMsg(statusMsg, ch->m_questList.m_list[i]);
								SEND_Q(statusMsg, ch->m_desc);
								
								continue;
							}
							else if (itemSum < ch->m_questList.m_list[i]->m_proto->m_conditionNum[j])
							{
								bComplete[j] = false;
							}
							else
								continue;
						}
						else if(ch->m_questList.m_list[i]->m_proto->m_conditionType[j] == -1)
						{
							bComplete[j] = true;
							continue;
						}
						else if((ch->m_questList.m_list[i]->m_proto->m_conditionType[j] == QCONDITION_ITEM 
							||ch->m_questList.m_list[i]->m_proto->m_conditionIndex[j] == QCONDITION_NPC) && ch->m_questList.m_bComplete[i])
						{
							bComplete[j] = true;
							continue;
						}
						else if(ch->m_questList.m_list[i]->m_proto->m_conditionType[j] == QCONDITION_ITEM_NORMAL 
							&& (ch->m_questList.m_list[i]->m_proto->m_conditionNum[j] == ch->m_questList.m_list[i]->m_currentData[j]))
						{
							bComplete[j] = true;
							continue;
						}
						else
						{
							bComplete[j] = false;
							continue;
						}
					}					
					else
					{
						bComplete[j] = true;
					}
				}
				
				if(ch->m_questList.IsCompleted(i))//이미 완료된 퀘스트는 skip
				{
					continue;
				}

				for (int k=0; k < QUEST_MAX_CONDITION; k++)
				{
					if (!bComplete[k])
						break;

					if(k == QUEST_MAX_CONDITION-1)
					{
						ch->m_questList.SetComplete2(i, true);
						
						if(ch->m_questList.m_bComplete[i])//퀘스트 아이템도 완료가 되었다면,
						{
							GAMELOG << init("QUEST COMPLETE (CheckQeustCondition)", ch)
								<< ch->m_questList.m_list[i]->m_proto->m_index
								<< end;
							
							// 완료 메시지 
							CNetMsg completeMsg;
							QuestCompleteMsg(completeMsg, ch->m_questList.m_list[i]);
							SEND_Q(completeMsg, ch->m_desc);
						}
					}
				}				
			}
			
		} // for

#endif // QUEST_DATA_EXTEND
	}				
	

}

bool CPC::CheckDungeonData(int zone)
{
#if !defined (TEST_SERVER) && defined(ENABLE_SINGLEDUNGEON_DATA)
	CQuestProto* qproto = NULL;
	switch (zone)
	{
	case ZONE_SINGLE_DUNGEON3:
		qproto = gserver.m_questProtoList.FindProto(104);
		if (m_pd3Count >= 5)
			return false;
		break;

	case ZONE_SINGLE_DUNGEON4:
		qproto = gserver.m_questProtoList.FindProto(105);
		if (m_pd4Count >= 3)
			return false;
		break;
	}

	if( this->m_admin < 2 && qproto && ( this->m_level < qproto->m_needMinLevel || this->m_level > qproto->m_needMaxLevel ) )
		return false;
#endif

	return true;
}

int CPC::GetJoinFlag(int zoneindex)
{
#ifdef ENABLE_WAR_CASTLE
	switch (zoneindex)
	{
	case ZONE_MERAC:
		return m_joinMerac;
#ifdef DRATAN_CASTLE
	case ZONE_DRATAN:
		return m_joinDratan;
#endif // DRATAN_CASTLE
	default:
		return WCJF_NONE;
	}
#else
	return 0;
#endif	// ENABLE_WAR_CASTLE
}

void CPC::SetJoinFlag(int zoneindex, int value)
{
#ifdef ENABLE_WAR_CASTLE
	switch (zoneindex)
	{
	case ZONE_MERAC:
		m_joinMerac = value;
		return;
#ifdef DRATAN_CASTLE
	case ZONE_DRATAN:
		m_joinDratan = value;
		return;
#endif // DRATAN_CASTLE
	default:
		return ;
	}
#endif
}

bool CPC::InitSkill(CSkillList *skillList, bool isPassive)
{

	CSkill *skill = NULL;
	LONGLONG skillPoint = 0;
	char skillLevel = 0;
	int skillCount = 0;

	if(!skillList)
	{
		// 초기화할 스킬이 없음을 메세지보냄
		return false;
	}

	skillCount = skillList->count();

	CSkillListNode* node = NULL;

	node = (CSkillListNode *) skillList->GetHeadPosition();
	
	if(!node)
	{
		// 초기화할 스킬이 없음을 메세지보냄
		return false;
	}
	skill = node->skill();
	for(int i = 0; i < skillCount; i ++)
	{	

		skillLevel = skill->m_level;
		for(;skillLevel > 0; skillLevel--)
		{
			skillPoint += skill->m_proto->Level(skillLevel)->m_learnSP * 10000;
		}
		node = node->next();
		if(!node) break;
		skill = node->skill();
	}
	
	skillList->Clear();

	if( !isPassive )
	{
		
		switch (this->m_job)
		{
		case JOB_TITAN:
			skill = gserver.m_skillProtoList.Create(1);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;

		case JOB_KNIGHT:
			skill = gserver.m_skillProtoList.Create(14);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;

		case JOB_HEALER:
			skill = gserver.m_skillProtoList.Create(27);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			skill = gserver.m_skillProtoList.Create(31);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;

		case JOB_MAGE:
			skill = gserver.m_skillProtoList.Create(53);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;

		case JOB_ROGUE:
			skill = gserver.m_skillProtoList.Create(116);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;

		case JOB_SORCERER:
			skill = gserver.m_skillProtoList.Create(292);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;
		default:
			break;
		}
	}
	this->m_skillPoint += skillPoint;

	skill = NULL;
	delete skill;

	return true;


}

bool CPC::InitSSkill()
{

	CSSkill *skill = NULL;
	CSSkillNode* node = NULL;
	LONGLONG skillPoint = 0;
	char skillLevel = 0;
	int skillCount = 0;

	skillCount = m_sSkillList.m_nCount;

	if(!skillCount) return false;

	node = m_sSkillList.m_head;
	
	skill = node->m_sskill;
	for(int i = 0; i < skillCount; i ++)
	{	
		bool bAddSkillPoint = true;
		while(skill)
		{
			if(bAddSkillPoint)
			{
				skillLevel = skill->m_level - 1;
				for(;skillLevel >= 0; skillLevel--)
				{
					// TODO: 스킬 포인트 계산 및 SSkill포인트 구하기 위한 알고리즘 필요
					skillPoint += skill->m_proto->m_needSP[(int)skillLevel] * 10000;
				}
			}

			if(skill->m_proto->m_needSSkill != -1) 
			{
				skill = gserver.m_sSkillProtoList.Create(skill->m_proto->m_needSSkill, skill->m_proto->m_needSSkillLevel);

				CSSkillNode* node2 = NULL;
				bAddSkillPoint = true;
				node2 = m_sSkillList.m_head;
				while(node2)
				{
					if(node2->m_sskill)
					{
						if(node2->m_sskill->m_proto->m_index == skill->m_proto->m_index)
						{
							bAddSkillPoint = false;
							break;
						}
					}
					node2 = node2->m_next;
				}
			}
			else
				break;

		}

		//m_sSkillList.Remove(node->m_sskill);
		node = node->m_next;

		if(!node) break;
		skill = node->m_sskill;
	}
	
	m_sSkillList.Init();
	
	
	skill = gserver.m_sSkillProtoList.Create(1);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	skill = gserver.m_sSkillProtoList.Create(5);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	skill = gserver.m_sSkillProtoList.Create(9);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	skill = gserver.m_sSkillProtoList.Create(13);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	skill = gserver.m_sSkillProtoList.Create(18);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	skill = gserver.m_sSkillProtoList.Create(23);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	skill = gserver.m_sSkillProtoList.Create(44);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;
	
	
	this->m_skillPoint += skillPoint;

	skill = NULL;
	node = NULL;
	delete skill;
	delete node;

	return true;

}
bool CPC::IsCombatMode()
{
	return ((gserver.m_pulse - m_lastAttackPulse) < PULSE_ATTACKMODE_DELAY);
}

#ifdef ENABLE_PET
void CPC::UpdatePetValue()
{
	// 060221 : bs : 피스존/공성지역에서는 경험치/교감도/배고픔 변화 없음
	if (IsInPeaceZone(true) || GetMapAttr() == MAPATT_WARZONE)
		return ;

	CNetMsg rmsg;

	CPet* p = m_petList;
	while (p)
	{
		// 060221 : bs : 펫 봉인 상태 추가
		if (p->GetRemainRebirthTime() > 0)
		{
			// 공격중일때
			if (IsCombatMode() && (gserver.m_pulse - m_pulseLastAttackSkill) < PULSE_ATTACKMODE_DELAY)
			{
				p->SetRemainRebirthTime(p->GetRemainRebirthTime() - 1);
				if (p->GetRemainRebirthTime() < 1)
				{
					// TODO : petlog
					GAMELOG << init("PET REBIRTH AUTO", this)
							<< "PET" << delim
							<< p->m_index << delim
							<< "TYPE" << delim
							<< p->GetPetType() << delim
							<< "LEVEL" << delim
							<< p->m_level
							<< end;
					p->m_hp = 30;
				}
				ExPetStatusMsg(rmsg, p);
				SEND_Q(rmsg, m_desc);
			}
		}
		else
		{
			// 060221 : bs : 피스존/공성지역에서는 경험치/교감도/배고픔 변화 없음
			if (!p->IsSummon() || !(p->IsInPeaceZone(true) || p->GetMapAttr() == MAPATT_WARZONE))
			{
				// 050309: bw 펫이 20미터 이상 떨어지거나 안에 들어오는 순간 거시기 calcstatus호출
				// 배고픔 0이면 경험치 고정, 스킬 유지, 교감도 5분에 1포인트 하락
				bool bexp = false;
				bool bchangenear = false;
				bool bnear = false;

				if( p->GetDistanceFromOwner() < 20 )
				{
					if ( p->WasNear() )
						bchangenear = false;
					else
						bchangenear = true;
					bnear = true;
				}
				else
				{
					if( p->WasNear() )
						bchangenear = true;
					else
						bchangenear = false;
				}
		
#ifdef NEW_PET_EXP_SYSTEM
#else // NEW_PET_EXP_SYSTEM
				// 배고픔이 0 아니고 근처에 있으면 
				if( p->IsActive() && bnear)
					 bexp = p->AddExp();
#endif // NEW_PET_EXP_SYSTEM

				bool bsympathy = p->UpdateSympathyPoint(bnear);
				bool bhungry = p->DecreaseHungryPoint();

				if (bexp || bsympathy || bhungry)
				{
					ExPetStatusMsg(rmsg, p);
					SEND_Q(rmsg, m_desc);
				}

				if( bsympathy || bchangenear)
				{
					CPC* pc = p->GetOwner();
					pc->CalcStatus(true);
					p->SetWasNear(bnear);
				}
			}
		}

		p = p->m_nextPet;
	}
}

CPet* CPC::GetPet(int index)
{
	if (index == 0)
	{
		if (m_wearing[WEARING_PET])
		{
			index = m_wearing[WEARING_PET]->m_plus;
		}
		else
			return NULL;
	}

	CPet* p = m_petList;
	while (p)
	{
		if (p->m_index == index)
			return p;

		p = p->m_nextPet;
	}

	return NULL;
}

void CPC::DelPet(int petIndex)
{
	// 펫 리스트에서 삭제 대상을 찾아
	// 아이템 리스트에서 해당 악세 삭제
	// 펫을 리스트에서 제거
	// 펫 삭제

	CNetMsg rmsg;
	CPet* petNext = m_petList;
	CPet* pet;
	while ((pet = petNext))
	{
		petNext = pet->m_nextPet;

		if (pet->m_index == petIndex)
		{
			int itemdbindex = 0;
			const char* petTypeName;
			switch (pet->GetPetType())
			{
				/*
#define PET_HORSE_ITEM_INDEX			871				// 말 아이템 인덱스
#define PET_DRAGON_ITEM_INDEX			872				// 용 아이템 인덱스
#define PET_BLUE_HORSE_ITEM_INDEX		1710			// 말 아이템 인덱스
#define PET_PINK_DRAGON_ITEM_INDEX		1711			// 용 아이템 인덱스
#define PET_UNKOWN_HORSE_ITEM_INDEX		1713			// 말 아이템 인덱스
#define PET_UNKOWN_DRAGON_ITEM_INDEX	1712			// 용 아이템 인덱스
				*/
			case PET_TYPE_HORSE:
				itemdbindex = PET_HORSE_ITEM_INDEX;
				petTypeName = "HORSE";
				break;
			case PET_TYPE_BLUE_HORSE:
				itemdbindex = PET_BLUE_HORSE_ITEM_INDEX;
				petTypeName = "BLUE_HORSE";
				break;
			case PET_TYPE_UNKOWN_HORSE:
				itemdbindex = PET_UNKOWN_HORSE_ITEM_INDEX;
				petTypeName = "UNKOWN_HORSE";
				break;
			case PET_TYPE_DRAGON:
				itemdbindex = PET_DRAGON_ITEM_INDEX;
				petTypeName = "DRAGON";
				break;
			case PET_TYPE_PINK_DRAGON:
				itemdbindex = PET_PINK_DRAGON_ITEM_INDEX;
				petTypeName = "PINK_DRAGON";
				break;
			case PET_TYPE_UNKOWN_DRAGON:
				itemdbindex = PET_UNKOWN_DRAGON_ITEM_INDEX;
				petTypeName = "UNKOWN_DRAGON";
				break;
			}

			// 소환되어 있는 펫은 셀에서 제거
			if (pet->IsSummon())
			{
				pet->Disappear();
			}
			// 타고 있으면 내리기
			else if (pet->IsMount())
			{
				pet->Mount(false);
			}

			int r, c;
			if (m_invenNormal.FindItem(&r, &c, itemdbindex, petIndex, -1))
			{
				CItem* item = m_invenNormal.GetItem(r, c);
				if (pet->IsWearing())
				{
					ItemWearMsg(rmsg, WEARING_PET, NULL, item);
					SEND_Q(rmsg, m_desc);
					m_wearing[WEARING_PET] = NULL;
				}
				ItemDeleteMsg(rmsg, item);
				SEND_Q(rmsg, m_desc);
				RemoveFromInventory(this, item, true, true);
			}

			// TODO : petlog
			GAMELOG << init("PET DELETE", m_name, m_desc->m_idname)
					<< "TYPE" << delim
					<< petTypeName << delim
					<< "GRADE" << delim
					<< pet->GetPetGrade() << delim
					<< "LEVEL" << delim
					<< pet->m_level << delim
					<< "EXP" << delim
					<< pet->m_exp
					<< end;

			// 펫 리스트에서 삭제
			REMOVE_FROM_BILIST(pet, m_petList, m_prevPet, m_nextPet);
			DelAttackList(pet);
			delete pet;

			break;
		}
	}
}

#endif // #ifdef ENABLE_PET

bool CPC::CanPvP(CCharacter* target, bool bIgnoreInvisible)
{
	if (!target)								return false;

	CPC* tpc = NULL;
	switch (target->m_type)
	{
	case MSG_CHAR_PC:			tpc = TO_PC(target);						break;

#ifdef ENABLE_PET
	case MSG_CHAR_PET:			tpc = TO_PET(target)->GetOwner();			break;
#endif

	case MSG_CHAR_ELEMENTAL:	tpc = TO_ELEMENTAL(target)->GetOwner();		break;

	default:					break;
	}

	if (!tpc)
		return true;

	// 사망검사
	if (DEAD(this))								return false;
	if (DEAD(target))							return false;

	// 운영자 검사
	if (m_bImmortal)							return false;
	if (tpc->m_bImmortal)						return false;

	bool bSkipLevel = false;

#ifdef ENABLE_WAR_CASTLE
	// 공성전 지역에서 공성 진행시에는 레벨 검사 안함
	if (m_pZone->m_index == target->m_pZone->m_index && m_pZone->m_index == CWarCastle::GetCurSubServerCastleZoneIndex() && GetMapAttr() == target->GetMapAttr() && GetMapAttr() == MAPATT_WARZONE)
	{
		CWarCastle* castle = CWarCastle::GetCastleObject(m_pZone->m_index);
		if (castle && castle->GetState() != WCSF_NORMAL)
		{
			bSkipLevel = true;
		}
	}
#endif // #ifdef ENABLE_WAR_CASTLE

#ifdef LC_JPN
	if( this->GetMapAttr() == MAPATT_FREEPKZONE )
	{
		bSkipLevel = true;
	}
#endif

#ifdef FREE_PK_SYSTEM
	bSkipLevel = true;
#endif // FREE_PK_SYSTEM
	if (!bSkipLevel && tpc->m_level <= PKMODE_LIMIT_LEVEL)
	{
		// 대상 케릭의 레벨이 거시기허요 메세지 보냄
		CNetMsg rmsg;
		SysMsg(rmsg, MSG_SYS_TARGET_PKLIMITLEVEL);
		SEND_Q(rmsg, this->m_desc);
		return false;
	}

	// 적으로 인식 가능?
	if (!IsEnemy(tpc))						return false;

	// 보이는지 검사
	if (!bIgnoreInvisible && !CanVisible(target))
		return false;

	// 레벨 검사
	if (!bSkipLevel && m_level <= PKMODE_LIMIT_LEVEL)	return false;

	// 행동 불능 검사
	if (IsDisable())							return false;
	if (tpc->IsDisable())						return false;

	// 피스존 검사 : 공격자는 속성맵 BLOCK 포함
	if (IsInPeaceZone(true))					return false;

	// 피스존 검사 : 방어자는 속성맵 BLOCK 미포함
	if (target->IsInPeaceZone(false))			return false;

	// PK로 시도시에만 정방
	if (IsSetPlayerState(PLAYER_STATE_PKMODE) && !tpc->IsSetPlayerState(PLAYER_STATE_PKMODE))
	{
		// 서로 리스트에 없을 때 
		if (!IsRaList(this, tpc))
		{
			AddRaList(this, tpc);
		}
		else
		{
			CRaChar* raList = tpc->m_raList;

			while (raList)
			{
				if (raList->m_raTarget == this && raList->m_bAttacker)
				{
					raList->m_raPulse = RAMODE_USE_PULSE;
					break;
				}
				raList = raList->m_next;
			}
		}
	}

	return true;
}

void CPC::CancelChange()
{
	if (IsSetPlayerState(PLAYER_STATE_CHANGE))
	{
		m_changeIndex = -1;
		m_changePulse = 0;

		ResetPlayerState(PLAYER_STATE_CHANGE);

		if (m_pArea)
		{
			CNetMsg rmsg;
			ChangeStopMsg(rmsg, this);
			m_pArea->SendToCell(rmsg, this, true);
		}
	}
}

CElemental* CPC::SummonElemental(char elementalType)
{
	CNetMsg rmsg;

	switch (elementalType)
	{
	case ELEMENTAL_FIRE:
	case ELEMENTAL_WIND:
	case ELEMENTAL_EARTH:
	case ELEMENTAL_WATER:
		m_pulseElemental[(int)elementalType] = gserver.m_pulse;
		break;
	default:
		return NULL;
	}

	CElemental* elemental = new CElemental(this, elementalType);
	ADD_TO_BILIST(elemental, m_elementalList, m_prevElemental, m_nextElemental);

	// 좌표 지정
	GET_R(elemental) = GET_R(this);
	GET_YLAYER(elemental) = GET_YLAYER(this);

	// 적정 좌표 구하기
	float x, z;
	int nTry = 10;
	while (nTry--)
	{
		x = GET_X(this) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
		z = GET_Z(this) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
		if (m_pArea->GetAttr(GET_YLAYER(this), x, z) != MAPATT_BLOCK)
			break;
	}

	GET_X(elemental) = x;
	GET_Z(elemental) = z;
	int cx, cz;
	m_pArea->PointToCellNum(x, z, &cx, &cz);
	m_pArea->CharToCell(elemental, GET_YLAYER(elemental), cx, cz);

	// 패시브 적용을 위해
	elemental->CalcStatus(true);

	// 나타나기
	AppearMsg(rmsg, elemental, true);
	m_pArea->SendToCell(rmsg, GET_YLAYER(elemental), cx, cz);

	// 주인에게 소환 알리기
	ExElementalStatusMsg(rmsg, elemental);
	SEND_Q(rmsg, m_desc);

	return elemental;
}

void CPC::UnsummonElemental(CElemental* elemental)
{
	CNetMsg rmsg;

	ExElementalDeleteMsg(rmsg, elemental);
	SEND_Q(rmsg, m_desc);

	CElemental* pElemental;
	CElemental* pElementalNext = m_elementalList;

	while ((pElemental = pElementalNext))
	{
		pElementalNext = pElemental->m_nextElemental;
		if (pElemental == elemental)
		{
			switch (elemental->GetElementalType())
			{
			case ELEMENTAL_FIRE:
			case ELEMENTAL_WIND:
			case ELEMENTAL_EARTH:
			case ELEMENTAL_WATER:
				m_pulseElemental[(int)elemental->GetElementalType()] = gserver.m_pulse;
				break;
			}

			DelAttackList(elemental);
			DisappearMsg(rmsg, elemental);
			elemental->m_pArea->SendToCell(rmsg, elemental, true);
			elemental->m_pArea->CharFromCell(elemental, true);
			REMOVE_FROM_BILIST(elemental, m_elementalList, m_prevElemental, m_nextElemental);
			delete elemental;
			return ;
		}
	}
}

bool CPC::CanSummonElemental(char newElementalType, char* duplicationType, int* nRemainTimeSummon, bool* errElementalType)
{
	*duplicationType = -1;
	*nRemainTimeSummon = 0;
	*errElementalType = false;

	int pulseSummon = -1;
	char elementalType[3];		// { 소환하려는 타입, 중복 불가 1, 중복 불가 2 }
	elementalType[0] = newElementalType;

	switch (elementalType[0])
	{
	case ELEMENTAL_FIRE:
	case ELEMENTAL_WIND:
		// 대지, 물과는 불가능
		elementalType[1] = ELEMENTAL_EARTH;
		elementalType[2] = ELEMENTAL_WATER;
		break;

	case ELEMENTAL_EARTH:
	case ELEMENTAL_WATER:
		// 불, 바람과는 불가능
		elementalType[1] = ELEMENTAL_FIRE;
		elementalType[2] = ELEMENTAL_WIND;
		break;

	default:
		*errElementalType = true;
		return false;
	}

	// 중복 검사
	CElemental* elemental = m_elementalList;
	while (elemental)
	{
		int i;
		for (i = 0; i < 3; i++)
		{
			if (elemental->GetElementalType() == elementalType[i])
			{
				*duplicationType = elemental->GetElementalType();
				return false;
			}
		}
		elemental = elemental->m_nextElemental;
	}

	// 시간 검사
	pulseSummon = m_pulseElemental[(int)elementalType[0]];
	if (pulseSummon >= 0 && pulseSummon + SUMMON_DELAY >= gserver.m_pulse)
	{
		*nRemainTimeSummon = (pulseSummon + SUMMON_DELAY - gserver.m_pulse) / PULSE_REAL_SEC;
		return false;
	}

	return true;
}

CElemental* CPC::GetElemental(int elementalindex)
{
	CElemental* pElemental = m_elementalList;
	while (pElemental)
	{
		if (pElemental->m_index == elementalindex)
			return pElemental;
		pElemental = pElemental->m_nextElemental;
	}
	return NULL;
}

void CPC::Evocation(int type)
{
	switch (type)
	{
	case EVOCATION_HELLOUND:
		if (m_pulseEvocation[0] == 0)
			m_pulseEvocation[0] = gserver.m_pulse;
		m_evocationType = type;
		break;

	case EVOCATION_ELENEN:
		m_evocationType = type;
		if (m_pulseEvocation[1] == 0)
			m_pulseEvocation[1] = gserver.m_pulse;
		break;

	default:
		return ;
	}

	CalcStatus(true);

	if (!m_pArea)
		return ;

	CNetMsg rmsg;

	ExEvocationStartMsg(rmsg, this);
	m_pArea->SendToCell(rmsg, this, true);
}

void CPC::Unevocation()
{
	m_evocationType = EVOCATION_NONE;

	CalcStatus(true);

	if (!m_pArea)
		return ;

	CNetMsg rmsg;

	ExEvocationStopMsg(rmsg, this);
	m_pArea->SendToCell(rmsg, this, true);
}

int CPC::GetRemainEvocation(bool bSec)
{
	int ret = 0;
	switch (m_evocationType)
	{
	case EVOCATION_HELLOUND:
		ret = (EVOCATION_DURATION - (gserver.m_pulse - m_pulseEvocation[0]));
		break;
	case EVOCATION_ELENEN:
		ret = (EVOCATION_DURATION - (gserver.m_pulse - m_pulseEvocation[1]));
		break;
	default:
		return 0;
	}
	if (bSec)
		return ret / 10;
	else
		return ret;
}

void CPC::ProcDisconnect(bool bDisconnect, bool bGoto)
{
	if (m_bProcDisconnect)
		return ;
	if (bDisconnect)
		m_bProcDisconnect = true;

	CNetMsg		rmsg;
	CArea*		pCurArea = m_pArea;
	CZone*		pCurZone = m_pZone;
#ifdef MONSTER_COMBO
	CParty*		pParty = NULL;
#endif // MONSTER_COMBO

#ifdef EXTREME_CUBE
	CGuildMember*		pGuildInfo = NULL;
#endif // EXTREME_CUBE

	if (!m_bLoadChar)
		return ;

#if defined ( CHAR_LOG ) && defined ( LC_KOR )
	if( bDisconnect  && IS_RUNNING_CONN && m_admin < 5 )
	{
		CNetMsg charlog;
		ConnCharLogMsg( charlog, this );
		SEND_Q(charlog, gserver.m_connector);
	}	
#endif //#if defined ( CHAR_LOG ) && defined ( LC_KOR )			

#if defined (MONSTER_COMBO) || defined (EXTREME_CUBE)
	
	pParty = gserver.FindPartyByMemberIndex(m_index, false);

	if(pParty)
	{
		if( !(pParty->GetRequestIndex() != m_index && pParty->GetMemberCount() > 1) )
		{
			pParty = NULL;
		}
	}

	// 콤보존과 관련된 콤보스킬 모두 제거
	m_assist.CureAssist(SF_COMBO);
	// 콤보편집중 종료처리
	if(bDisconnect)
		m_listSaveComboData.RemoveAll();
#endif // MONSTER_COMBO

#ifdef EXTREME_CUBE
	pGuildInfo = m_guildInfo;
#endif// EXTREME_CUBE

	// 길드 가입 신청 상태 해제
	if (m_regGuild)
	{
		if (m_guildInfo)
			GuildRegistCancelMsg(rmsg, false);
		else
			GuildRegistCancelMsg(rmsg, true);
		do_Guild(this, rmsg);
	}

	// 길드장이고 길드전 관련 신청중이면 취소
	if (m_guildInfo && m_guildInfo->guild() && m_guildInfo->guild()->boss() && m_guildInfo->guild()->boss()->charindex() == m_index)
	{
		CGuild* guild = m_guildInfo->guild();
		if (guild && (guild->battleState() == GUILD_BATTLE_STATE_START_WAIT || guild->battleState() == GUILD_BATTLE_STATE_STOP_WAIT))
		{
			// 길드 신청중일때
			if (guild->battleState() == GUILD_BATTLE_STATE_START_WAIT)
			{
				GuildBattleReqRejectMsg(rmsg, m_index);
				do_Guild(this, rmsg);
			}
			// 길드 중단 신청 중일때
			else if (guild->battleState() == GUILD_BATTLE_STATE_STOP_WAIT)
			{
				GuildBattleStopRejectMsg(rmsg, m_index);
				do_Guild(this, rmsg);
			}
		}
	}

	// 교환 중이면 취소하기
	if (m_exchange)
	{
		ExchangeReqMsg(rmsg, MSG_EXCHANGE_REQ_REJECT, NULL, NULL);
		do_Exchange(this, rmsg);
	}

	// 사제 신청중이면 취소하기
	if (m_teachWait != -1)
	{
		TeachTeacherReqRejectMsg(rmsg, this);
		do_Teach(this, rmsg);
	}

	// 개인 상점 취소
	if (m_personalshop)
	{
		delete m_personalshop;
		m_personalshop = NULL;
	}

	if(m_nMoonStoneDigit != -1) //문스톤을 돌리는 중이었다..
	{
		gserver.MoonStoneEndProcess(this);
	}

	// 다른 존으로 이동하거나 접속 종료시에만 처리되는 루틴
	if (!bGoto)
	{
		// 길드 가입자는 오프라인 알리기
		if (bDisconnect && m_guildInfo && m_guildInfo->GetPC() != NULL)
		{
			m_guildInfo->online(0);
			HelperGuildOnline(rmsg, m_guildInfo);
#ifdef NEW_GUILD
			rmsg << (int)0;
#endif // NEW_GUILD			
			m_guildInfo->SetPC(NULL);
			if (IS_RUNNING_HELPER) SEND_Q(rmsg, gserver.m_helper);
		}

#ifdef ENABLE_PET
		// 펫 사라짐
		CPet* petNext = m_petList;
		CPet* pet;
		while ((pet = petNext))
		{
			petNext = pet->m_nextPet;
			if (pet->IsSummon())
			{
				pet->Disappear();
			}
		}
#endif // ENABLE_PET

#ifdef ATTACK_PET
		CAPet* apetNext = m_pApetlist;
		CAPet* apet;
		while ((apet = apetNext))
		{
			apetNext = apet->m_pNextPet;
			if (apet->IsSummon())
			{
				apet->Disappear();
			}
		}
#endif //ATTACK_PET

		// 소환수 제거
		while (m_elementalList)
			UnsummonElemental(m_elementalList);

		// 변신해제
		if (IsSetPlayerState(PLAYER_STATE_CHANGE) && m_changeIndex != -1)
			CancelChange();

		// 파티 처리
		// 접속 종료시에만 처리, 존이동 등은 GoZone()에서 처리함
		if (m_party != NULL && bDisconnect)
		{
			// 초대하거나, 초대받은 사람일 경우 거절로 처리
			if (m_party->GetRequestIndex() > 0 && (m_party->GetBossIndex() == m_index || m_party->GetRequestIndex() == m_index))
			{
				PartyMsg(rmsg, MSG_PARTY_REJECT);
				do_Party(this, rmsg);
			}
			// 존이동이 아니면 파티 탈퇴로 처리
			else if (!m_desc->m_bGoZone)
			{
				PartyMsg(rmsg, MSG_PARTY_QUIT);
				do_Party(this, rmsg);
			}
			// 존이동이면 파티 정보에서 포인터만 지운다
			m_party->SetMemberPCPtr(m_index, NULL);
			m_party = NULL;
		}

#ifdef EXPEDITION_RAID
		// 원정대 처리
		if (m_Exped != NULL && bDisconnect)
		{
			if (m_Exped->GetRequestIndex() > 0 && (m_Exped->GetBossIndex() == m_index || m_Exped->GetRequestIndex() == m_index))
			{
				ExpedMsg(rmsg, MSG_REJECT_REQ);
				do_Expedition(this, rmsg);
			}
			else if (!m_desc->m_bGoZone)
			{
				// 원정대 나감 처리(정상,비정상 구분)
				ExpedQuitReqMsg(rmsg, MSG_EXPED_QUITMODE_NORMAL);
				do_Expedition(this, rmsg);
			}

			m_Exped->SetMemberPCPtr(m_index, NULL);
			m_Exped = NULL;
		}
#endif //EXPEDITION_RAID
		
#ifdef PARTY_MATCHING
		if (bDisconnect && !m_desc->m_bGoZone)
		{
			// 파티 매칭에서 제거
			rmsg.Init(MSG_EXTEND);
			rmsg << MSG_EX_PARTY_MATCH
				 << MSG_EX_PARTY_MATCH_DEL_REQ;
			do_Extend(this, rmsg);
		}
#endif // PARTY_MATCHING

#ifdef ENABLE_MESSENGER
		// 친구 처리
		if (m_Friend)
		{
			// 친구등록 요청중이거나 요청받고 있는주이었다면..
			if (m_nRegFriend != 0)
			{
				CPC* pRegFriendPC = gserver.m_playerList.Find(m_nRegFriend);
				
				if (pRegFriendPC)
				{
					FriendRegistCancelMsg(rmsg);
					SEND_Q(rmsg, pRegFriendPC->m_desc);
					pRegFriendPC->m_nRegFriend = 0;
				}
				
				m_nRegFriend = 0;
			}

			if (bDisconnect)
			{
				m_nCondition = 0;

				HelperFriendSetConditionMsg(rmsg, m_index, m_nCondition, -1, this);
				if (IS_RUNNING_HELPER) SEND_Q(rmsg, gserver.m_helper);

				//delete m_Friend;
				//m_Friend = NULL;
			}
		}
#endif

		if (pCurZone && pCurArea)
		{
			// 길드룸에서 나가는거라면
			if (pCurZone->IsGuildRoom())
			{
				if (pCurArea->GetPCCount(0, 0, MAX(pCurArea->m_size[0], pCurArea->m_size[1])) < 1)
				{
					pCurArea->m_guildIndex = -1;
					pCurZone->SetDisableArea(pCurArea->m_index);
					pCurArea = NULL;
				}
			}

			// 퍼던이면
			else if (pCurZone->IsPersonalDungeon())
			{
				pCurZone->SetDisableArea(pCurArea->m_index);
				pCurArea = NULL;
				
				// 싱글전전 퀘스트를 수행중인 경우 존이동시
				// 퀘스트 취소 처리
#ifdef QUEST_DATA_EXTEND

				CQuest* pQuestNext = m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
				CQuest* pQuest;
				const CQuestProto* pQuestProto;
				while ((pQuest = pQuestNext))
				{
					pQuestNext = m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);

					// 퀘스트를 하고 수행중이고 완료가 아니고
					if (!pQuest->IsCompleted())
					{
						pQuestProto = pQuest->GetQuestProto();
						// 발록 격파 퀘스트
						if (pQuestProto->m_type[0] == QTYPE_KIND_DEFEAT)
						{
							// Quest Fail Log
							GAMELOG << init("QUEST FAIL", this)
									<< pQuestProto->m_index
									<< end;
							
							// 없으면 바로 실패
							QuestFailMsg(rmsg, pQuest);
							if (!bDisconnect) SEND_Q(rmsg, m_desc);
							m_questList.DelQuest(this, pQuest);
							goto EXIT_PROC_PD;
						}
						// 공주 구출 퀘스트
						else if (pQuestProto->m_type[0] == QTYPE_KIND_SAVE)
						{
							// 아이템 삭제
							pQuest->RemoeQuestItem(this);

							// Quest Fail Log
							GAMELOG << init("QUEST FAIL",  this)
									<< pQuestProto->m_index
									<< end;

							QuestFailMsg(rmsg, pQuest);
							if (!bDisconnect) SEND_Q(rmsg, m_desc);
							m_questList.DelQuest(this, pQuest);
							goto EXIT_PROC_PD;
						}
						else
							continue;
					}
				} // while ((pQuest = pQuestNext))

#else // QUEST_DATA_EXTEND

				int j;
				for (j=0; j < QUEST_MAX_PC; j++)
				{
					// 퀘스트를 하고 수행중이고 완료가 아니고
					if (m_questList.m_list[j] && m_questList.m_bQuest[j] && !m_questList.IsCompleted(j))
					{
						// 발록 격파 퀘스트
						if (m_questList.m_list[j]->m_proto->m_type[0] == QTYPE_KIND_DEFEAT)
						{
							// Quest Fail Log
							GAMELOG << init("QUEST FAIL", this)
									<< m_questList.m_list[j]->m_proto->m_index
									<< end;
							
							// 없으면 바로 실패
							QuestFailMsg(rmsg, m_questList.m_list[j]);
							if (!bDisconnect) SEND_Q(rmsg, m_desc);
							m_questList.DelQuest(this, m_questList.m_list[j]);
							goto EXIT_PROC_PD;
						}
						// 공주 구출 퀘스트
						else if (m_questList.m_list[j]->m_proto->m_type[0] == QTYPE_KIND_SAVE)
						{
							int k;
							for (k=0; k < QUEST_MAX_CONDITION; k++)
							{
								if (m_questList.m_list[j]->m_proto->m_conditionType[k] == -1)
									continue;
								
								// 수행조건 아이템을 찾고
								if (m_questList.m_list[j]->m_proto->m_conditionType[k] == QCONDITION_ITEM)
								{
									// 전달 퀘스트 아이템은 퀘스트 인벤에서만 !!!
									int r, c;
									if (!m_invenQuest.FindItem(&r, &c, m_questList.m_list[j]->m_proto->m_conditionIndex[k], 0, 0))
									{
										// Quest Fail Log
										GAMELOG << init("QUEST FAIL",  this)
											<< m_questList.m_list[j]->m_proto->m_index
											<< end;
										
										// 없으면 바로 실패
										QuestFailMsg(rmsg, m_questList.m_list[j]);
										if (!bDisconnect) SEND_Q(rmsg, m_desc);
										m_questList.DelQuest(this, m_questList.m_list[j]);
										goto EXIT_PROC_PD;
									}
									else
									{
										// 찾았으면 수량 확인 해서 줄이기
										CItem* item = m_invenQuest.GetItem(r, c);
										
										if (!item)
										{
											// Quest Fail Log
											GAMELOG << init("QUEST FAIL", this)
													<< m_questList.m_list[j]->m_proto->m_index
													<< end;
											
											// 없으면 바로 실패
											QuestFailMsg(rmsg, m_questList.m_list[j]);
											if (!bDisconnect) SEND_Q(rmsg, m_desc);
											m_questList.DelQuest(this, m_questList.m_list[j]);
											goto EXIT_PROC_PD;
										}
										else
										{
											// 필요 숫자 만큼 없음
											if (item->Count() < m_questList.m_list[j]->m_proto->m_conditionNum[k])
											{
												// Quest Fail Log
												GAMELOG << init("QUEST FAIL", this)
														<< m_questList.m_list[j]->m_proto->m_index
														<< end;
												
												// 없으면 바로 실패
												QuestFailMsg(rmsg, m_questList.m_list[j]);
												if (!bDisconnect) SEND_Q(rmsg, m_desc);
												m_questList.DelQuest(this, m_questList.m_list[j]);
												goto EXIT_PROC_PD;
											}
											else
											{
												// Item 수량 변경
												DecreaseFromInventory(this, item, m_questList.m_list[j]->m_proto->m_conditionNum[k]);
												
												if (item->Count() > 0)
												{
													ItemUpdateMsg(rmsg, item, -m_questList.m_list[j]->m_proto->m_conditionNum[k]);
													if (!bDisconnect) SEND_Q(rmsg, m_desc);
												}
												else
												{
													ItemDeleteMsg(rmsg, item);
													if (!bDisconnect) SEND_Q(rmsg, m_desc);
													RemoveItemFromQuickSlot(item);
													RemoveFromInventory(this, item, true, true);
												}
												
												// Quest Fail Log
												GAMELOG << init("QUEST FAIL", this)
														<< m_questList.m_list[j]->m_proto->m_index
														<< end;
												
												// 없으면 바로 실패
												QuestFailMsg(rmsg, m_questList.m_list[j]);
												if (!bDisconnect) SEND_Q(rmsg, m_desc);
												m_questList.DelQuest(this, m_questList.m_list[j]);
												goto EXIT_PROC_PD;
											}
										}
									}
								}
							}
							continue ;
						}
						else
							continue;
					}
				} // for (j=0; j < QUEST_MAX_PC; j++)

#endif // QUEST_DATA_EXTEND

			} // 퍼던이면
EXIT_PROC_PD:
			;
		}


	} // 다른 존으로 이동하거나 접속 종료시에만 처리되는 루틴

	// Attack List 초기화
	DelAttackList(this);

	// 자신이 사라짐을 알림
	if (pCurArea && IS_IN_CELL(this))
	{
		if (bDisconnect)
			SendDisappearAllInCell(false);
		else
			SendDisappearAllInCell(true);

		// 셀에서 제거
		pCurArea->CharFromCell(this, true);
#ifdef MONSTER_COMBO
		// 연결이 끊기거나 다른 존으로 이동할 때 콤보존 처리
		if(!bGoto && pCurZone->IsComboZone())
		{
			// 콤보던전에 pc가 전혀 없으면 콤보존 해체
			if(pCurArea->GetPCCount(0, 0, 1) < 1)
			{
				GAMELOG << init("CLOSE COMBO AREA", this)
						<< "AREA INDEX: " << pCurArea->m_index
						<< "COMBO INDEX: " << pCurArea->m_monsterCombo->m_nIndex
						<< end;

				// 콤보던전 해체될 때 파티가 있으면
				if(pParty)
					pParty->m_comboAreaIndex = -1;

				// 존에 있는 콤보정보 해제
				pCurArea->m_monsterCombo->InitAllVar();
				
				// 방문 닫음
				pCurZone->SetDisableArea(pCurArea->m_index);
				pCurArea = NULL;

			}
		}
#endif // MONSTER_COMBO
	}

#ifdef EXTREME_CUBE
	// CCMemList 에서 PC 삭제
	if(pCurZone != NULL && !bGoto && pCurZone->IsExtremeCube())
	{
		CCubeMemList* memlist = NULL;
		if(gserver.m_extremeCube.IsGuildCubeTime())
		{
			if(pGuildInfo && pGuildInfo->guild())
			{
				memlist = gserver.m_extremeCube.FindMemList(pGuildInfo->guild());
			}
		}
		else
		{
			if(pParty)
			{
				memlist = gserver.m_extremeCube.FindMemList(pParty);
			}
			else
			{
				memlist = NULL;
			}
		}

		if(memlist)
		{
			memlist->DelPC(this);
		}
	}
#endif // EXTREME_CUBE

#ifdef MESSENGER_NEW
	gserver.discCharChatGroup(this->GetName() );
#endif


	// 다른 존으로 이동하거나 접속 종료시에만 처리되는 루틴
	if (!bGoto)
	{
		// 레이어 존에 있을 경우 스타트 존으로 변경, 존이동이 아닐때에만
		if (pCurZone && bDisconnect)
		{
			if (!m_desc->m_bGoZone && pCurZone->m_countArea > 1)
			{
				// 스타트존 지점
				m_pZone				= gserver.m_zones + gserver.FindZone(ZONE_START);
				m_pArea				= NULL;
				GET_YLAYER(this)	= m_pZone->m_zonePos[0][0];
				GET_R(this)			= 0.0f;
				GET_X(this)			= GetRandom(m_pZone->m_zonePos[0][1], m_pZone->m_zonePos[0][3]) / 2.0f;
				GET_Z(this)			= GetRandom(m_pZone->m_zonePos[0][2], m_pZone->m_zonePos[0][4]) / 2.0f;
			}
		}

		// 접속 종료시만 처리
		if( bDisconnect )
		{
			m_assist.CureAssist(SF_LOGOUT);		
		}
	} // 다른 존으로 이동하거나 접속 종료시에만 처리되는 루틴
}

void CPC::OutputDBItemLog()
{
	while (m_listAutoGive.GetCount() > 0)
	{
		AUTO_GIVE_DATA agdItem = m_listAutoGive.GetData(m_listAutoGive.GetHead());
		if (m_bLoadChar)
			GAMELOG << init("AUTO GIVE ITEM", this);
		else
			GAMELOG << init("AUTO GIVE ITEM FAIL", this);
		GAMELOG << "ITEM" << delim
				<< "INDEX" << delim
				<< agdItem.nIndex << delim
				<< "PLUS" << delim
				<< agdItem.nPlus << delim
				<< "USED" << delim
				<< agdItem.nUsed << delim
				<< "FLAG" << delim
				<< agdItem.nFlag << delim
				<< "COUNT" << delim
				<< agdItem.nCount << delim
				<< "OPTION" << delim
				<< ((agdItem.nOption[0] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
				<< (agdItem.nOption[0] & 0x00ff) << delim
				<< ((agdItem.nOption[1] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
				<< (agdItem.nOption[1] & 0x00ff) << delim
				<< ((agdItem.nOption[2] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
				<< (agdItem.nOption[2] & 0x00ff) << delim
				<< ((agdItem.nOption[3] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
				<< (agdItem.nOption[3] & 0x00ff) << delim
				<< ((agdItem.nOption[4] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
				<< (agdItem.nOption[4] & 0x00ff) << delim
				<< "SERIAL" << delim
				<< agdItem.strSerial
				<< end;
		m_listAutoGive.Remove(m_listAutoGive.GetHead());
	}

	while (m_listBloodBug.GetCount() > 0)
	{
		BLOOD_BUG_DATA bugdata = m_listBloodBug.GetData(m_listBloodBug.GetHead());
		if (m_bLoadChar)
			GAMELOG << init("UNIQUE OPTION BUG FIX", this);
		else
			GAMELOG << init("UNIQUE OPTION BUG FIX FAIL", this);
		GAMELOG << "ITEM" << delim
				<< "INDEX" << delim
				<< bugdata.nIndex << delim
				<< "FLAG" << delim
				<< bugdata.nFlag << delim
				<< "OPTION" << delim
				<< bugdata.nOptionCount << delim
				<< ((bugdata.nOption[0] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
				<< (bugdata.nOption[0] & 0x00ff) << delim
				<< ((bugdata.nOption[1] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
				<< (bugdata.nOption[1] & 0x00ff) << delim
				<< ((bugdata.nOption[2] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
				<< (bugdata.nOption[2] & 0x00ff) << delim
				<< ((bugdata.nOption[3] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
				<< (bugdata.nOption[3] & 0x00ff) << delim
				<< ((bugdata.nOption[4] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
				<< (bugdata.nOption[4] & 0x00ff) << delim
				<< "SERIAL" << delim
				<< bugdata.strSerial
				<< end;
		m_listBloodBug.Remove(m_listBloodBug.GetHead());
	}
}

#ifdef FEEITEM
void CPC::ChangeName(const char* name)
{
	this->m_nick = name;

	CNetMsg rmsg;

	NameChangeRepMsg(rmsg, MSG_EX_NAMECHANGE_ERROR_SUCCESS, name, 0);
	SEND_Q(rmsg, m_desc);

	for(int i = 0; m_Friend != NULL && i < m_Friend->GetFriendCount(); i++)
	{
		CFriendMember* m_fmember = m_Friend->GetFriendMember(i);

		if( m_fmember == NULL ) continue;

		CPC* fpc = gserver.m_playerList.Find( m_fmember->GetChaIndex() );
		if( fpc)
		{
			CNetMsg fmsg;
			NameChangeFriendMsg(fmsg, m_index, GetName(), m_job );
			SEND_Q(fmsg, fpc->m_desc);
		}
	}

	// 길드원 전체에게 메세지 보냄
	if( m_guildInfo )
	{
		//member nick부분 삭제 : bw 060403
		//pc->m_guildInfo->nick(pc->GetName() );
		m_guildInfo->name(GetName() );

		NameChangeGuildMemberMsg(rmsg, m_index, GetName(), m_job);
		CGuild* guild = m_guildInfo->guild();
		if( guild )
			guild->SendToAll(rmsg);
	}

	// 자신의 주위의 모든 케릭터에게 어피어 메세지를 보낸다.
	DisappearMsg(rmsg, this);
	m_pArea->SendToCell(rmsg, this);
	AppearMsg(rmsg, this, true);
	m_pArea->SendToCell(rmsg, this);

#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
	if( m_guildInfo && m_guildInfo->guild() )
	{
		GuildPointRankingMsg( rmsg, m_index, m_guildInfo->guild()->index(), m_guildInfo->guild()->GetGuildPointRanking() );
		m_pArea->SendToCell( rmsg, this );
	}
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD
	
#ifdef ADULT_SERVER_NEWITEM
//	bAccessOfPeace = false;
//	m_lastpulseCurse = 0;
//	m_lastpulseDespair = 0;
#endif // ADULT_SERVER_NEWITEM
	
#ifdef DYNAMIC_DUNGEON
	GuildNameColorStateMsg(rmsg, this ); 
	m_pArea->SendToCell( rmsg, this );
#endif //DYNAMIC_DUNGEON

}
#endif // FEEITEM

#ifdef MESSENGER_NEW
bool CPC::AddBlockPC(int charIndex, const char* name)
{
	if( charIndex < 0 || strcmp(name, "" ) == 0 )
		return false;

	if( this->IsBlockPC(charIndex, name) )
		return false;

	CBlockPC* temp = new CBlockPC(charIndex, name);

	if( temp )
	{
		m_listBlockPC.AddToTail(temp);
		return true;
	}

	return false;
}

void CPC::ReleaseBlockPC(int charIndex, CLCString& name)
{
	CBlockPC* temp = new CBlockPC(charIndex, "");

	void* pos = this->m_listBlockPC.FindData(temp);

	if( pos )
	{
		delete temp;
		temp = this->m_listBlockPC.GetData(pos);
		name = temp->GetName();

		m_listBlockPC.Remove(pos);

	}

}

bool CPC::IsBlockPC(int charIndex, const char* name)
{
	CBlockPC* temp = new CBlockPC(charIndex, name);

	void* pos = this->m_listBlockPC.FindData(temp);

	if( pos )
	{
		delete temp;
		temp = this->m_listBlockPC.GetData(pos);

		if( temp )
		{
			temp->SetName(name);
		}
		return true;
	}

	return false;
}

void CPC::GetBlockListString(CLCString& blockIndexList, CLCString& blockNameList)
{
	void* pos = this->m_listBlockPC.GetHead();
	int count = this->m_listBlockPC.GetCount();

	CBlockPC* bPc = NULL;
	for(int i = 0; i < count; i++)
	{
		bPc = this->m_listBlockPC.GetData(pos);

		if( bPc )
		{
			sprintf(g_buf, "%d", bPc->GetIndex() );
			blockIndexList += g_buf;
			blockIndexList += " ";
			blockNameList += bPc->GetName();
			blockNameList += " ";
		}

		pos = this->m_listBlockPC.GetNext(pos);
	}
}
#endif // MESSENGER_NEW

#ifdef ADULT_SERVER_NEW_BALANCE
float CPC::GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type)
{
	extern int nBaseHitrate[JOBCOUNT][3];
	extern int levelup_stat[JOBCOUNT][4];
	extern int levelup_stat2[JOB2COUNT][4];

	const float nHitratePerDex = 1.0f;
	const float nHitratePerInt = 1.0f;

	float fHitrate	= nBaseHitrate[m_job][type] + (m_level - df->m_level);

	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
		if (m_job2 < 1)
			fHitrate += nHitratePerDex * (levelup_stat[m_job][1] * (m_level - 1));
		else
		{
#if defined ( NIGHT_SHADOW ) 
			fHitrate += nHitratePerDex * (levelup_stat2[m_job * 2 + m_job2 -1 ][1] * (m_level - JOB_2ND_LEVEL));
			fHitrate += nHitratePerDex * (levelup_stat[m_job][1] * (JOB_2ND_LEVEL - 1));
#else
			fHitrate += nHitratePerDex * (levelup_stat2[m_job * 2 + m_job2 ][1] * (m_level - JOB_2ND_LEVEL));
			fHitrate += nHitratePerDex * (levelup_stat[m_job][1] * (JOB_2ND_LEVEL - 1));
#endif //NIGHT_SHADOW
		}
		break;
	case MSG_DAMAGE_MAGIC:
		if (m_job2 < 1)
			fHitrate += nHitratePerInt * (levelup_stat[m_job][2] * (m_level - 1));
		else
		{
#if defined ( NIGHT_SHADOW ) 
			fHitrate += nHitratePerInt * (levelup_stat2[m_job * 2 + m_job2 -1 ][2] * (m_level - JOB_2ND_LEVEL));
		
			fHitrate += nHitratePerInt * (levelup_stat[m_job][2] * (JOB_2ND_LEVEL - 1));
#else
			fHitrate += nHitratePerInt * (levelup_stat2[m_job * 2 + m_job2 ][2] * (m_level - JOB_2ND_LEVEL));
			fHitrate += nHitratePerInt * (levelup_stat[m_job][2] * (JOB_2ND_LEVEL - 1));
#endif //NIGHT_SHADOW
		}
		break;
	default:
		break;
	}

	if( fHitrate < 0 )
		fHitrate = 0;

	return fHitrate;
}

float CPC::GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type)
{
	extern int nBaseAvoid[JOBCOUNT][3];
	extern int levelup_stat[JOBCOUNT][4];
	extern int levelup_stat2[JOB2COUNT][4];

	const float nAvoidPerDex = 1.0f;
	const float nAvoidPerInt = 1.0f;

	float fAvoid	= nBaseAvoid[m_job][type] + ((m_level - of->m_level) * 10 ) ;

	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
		if (m_job2 < 1)
			fAvoid += nAvoidPerDex * (levelup_stat[m_job][1] * (m_level - 1));
		else
		{
#if defined ( NIGHT_SHADOW ) 
			fAvoid += nAvoidPerDex * (levelup_stat2[m_job * 2 + m_job2 - 1][1] * (m_level - JOB_2ND_LEVEL));
			fAvoid += nAvoidPerDex * (levelup_stat[m_job][1] * (JOB_2ND_LEVEL - 1));
#else
			fAvoid += nAvoidPerDex * (levelup_stat2[m_job * 2 + m_job2 ][1] * (m_level - JOB_2ND_LEVEL));
			fAvoid += nAvoidPerDex * (levelup_stat[m_job][1] * (JOB_2ND_LEVEL - 1));
#endif //NIGHT_SHADOW
		}
		break;
	case MSG_DAMAGE_MAGIC:
		if (m_job2 < 1)
			fAvoid += nAvoidPerInt * (levelup_stat[m_job][2] * (m_level - 1));
		else
		{
#if defined ( NIGHT_SHADOW ) 
			fAvoid += nAvoidPerInt * (levelup_stat2[m_job * 2 + m_job2 - 1][2] * (m_level - JOB_2ND_LEVEL));
			fAvoid += nAvoidPerInt * (levelup_stat[m_job][2] * (JOB_2ND_LEVEL - 1));
#else
			fAvoid += nAvoidPerInt * (levelup_stat2[m_job * 2 + m_job2 ][2] * (m_level - JOB_2ND_LEVEL));
			fAvoid += nAvoidPerInt * (levelup_stat[m_job][2] * (JOB_2ND_LEVEL - 1));
#endif //NIGHT_SHADOW
		}
		break;
	default:
		break;
	}

	if( fAvoid < 0 )
		fAvoid = 0;

	return fAvoid;
}
#endif // ADULT_SERVER_NEW_BALANCE

#ifdef ATTACK_PET
void CPC::UpdateAPetValue()
{
	CAPet* p = m_pApetlist;
	while (p)
	{
		// 설정치 업데이트
		bool bUpdate = false;
		if( p->DecreaseStat() )
			bUpdate = true;
			
		if( p->IncreaseStat() )
			bUpdate = true;

		if( bUpdate ) 
		{
			CNetMsg rmsg;
			ExAPetStatusMsg(rmsg, p);
			SEND_Q(rmsg, m_desc);
		}
		
		p->CheckItemTime();

		p = p->m_pNextPet;
	}

}

CAPet* CPC::GetAPet(int index)
{
	if (index == 0)
	{
		if (m_wearing[WEARING_PET])
		{
			index = m_wearing[WEARING_PET]->m_plus;
		}
		else
			return NULL;
	}

	CAPet* p = m_pApetlist;
	while (p)
	{
		if (p->m_index == index)
			return p;

		p = p->m_pNextPet;
	}

	return NULL;
}

void CPC::DelAPet(int petIndex)
{
	CNetMsg rmsg;
	CAPet* petNext = m_pApetlist;
	CAPet* pet;
	while ((pet = petNext))
	{
		petNext = pet->m_pNextPet;
		if (pet->m_index == petIndex)
		{
			// 아이템 입고있으면 벗기
			// TODO : petlog
			// 펫 리스트에서 삭제
			if(pet->IsSummon())
			{
				pet->Disappear();
			}
			
			int r, c;
			if (m_invenNormal.FindItem(&r, &c, pet->m_pProto->m_nItemIndex, petIndex, -1))
			{
				CItem* item = m_invenNormal.GetItem(r, c);
				if (pet->IsWearing())
				{
					ItemWearMsg(rmsg, WEARING_PET, NULL, item);
					SEND_Q(rmsg, m_desc);
					m_wearing[WEARING_PET] = NULL;
				}
				ItemDeleteMsg(rmsg, item);
				SEND_Q(rmsg, m_desc);
				RemoveFromInventory(this, item, true, true);
			}
			
			GAMELOG << init("APET DELETE", m_name, m_desc->m_idname)
					<< "TYPE" << delim
					<< pet->m_name << delim
					<< "LEVEL" << delim
					<< pet->m_level << delim
					<< "EXP" << delim
					<< pet->m_exp
					<< end;

			REMOVE_FROM_BILIST(pet, m_pApetlist, m_pPrevPet, m_pNextPet);
			DelAttackList(pet);
			delete pet;
			pet = NULL;

			break;
		}
	}
}
#endif // #ifdef ATTACK_PET

#ifdef EX_GO_ZONE
int CPC::GetLastSaveZone(int* zone)
{
	if(!zone)
		return -1;

	CTeleportZoneData* pTelZone = new CTeleportZoneData(m_lastsavepos);
	void* pos = gserver.m_listTeleportZone.FindData(pTelZone);
	delete pTelZone;

	// 데이터가 없으면
	if(!pos)
	{
		*zone = ZONE_START;
		return gserver.FindZone(*zone);
	}

	CTeleportZoneData* npcZoneData = gserver.m_listTeleportZone.GetData(pos);
	return gserver.FindZone(npcZoneData->GetZoneIndex());
}
#endif // EX_GO_ZONE

#ifdef SET_ITEM
void CPC::ApplyOriginOption(CItem * pItem)
{
	// 0 : 귀속 설정, 1~6 : 옵션, 7~9 : 스킬
	for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
	{
		int nIndex = pItem->GetOriginIndex(i);
		if (nIndex <= 0)
			continue;
		
		COptionProto* pOptionProto = gserver.m_optionProtoList.FindProto(nIndex);
		if (pOptionProto)
		{
			int nValue = 0;
			int nLevel = pItem->GetOriginLevel(i);
			if (pOptionProto->m_levelValue[nLevel- 1])
				nValue = pOptionProto->m_levelValue[nLevel - 1];

			COption::ApplyOptionValue(this, pOptionProto->m_type, nValue, pItem);
		}
	}

	for (i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
	{
		int nIndex = pItem->GetOriginIndex(i);
		if (nIndex <= 0)
			continue;
		
		CSkill * pSkill = gserver.m_skillProtoList.Create(nIndex, pItem->GetOriginLevel(i));
		if(pSkill)
		{
			if (pSkill->m_proto->IsActive())
			{
				m_activeSkillList.Add(pSkill);
			}
			else if (pSkill->m_proto->IsPassive())	
			{
				m_passiveSkillList.Add(pSkill);
			}
			else
			{
				m_etcSkillList.Add(pSkill);
			}
		}
	}
}

void CPC::ApplySetItemValue(CItem * pItem)
{
	if (!pItem->m_pSetItemProto || pItem->m_nSetWearCnt <= 0)
		return;

	for (int i = 0; i < pItem->m_pSetItemProto->GetOptionCnt(); i++)
	{
		const SETITEMOPTION* pSetOption = pItem->m_pSetItemProto->GetOption(i);

		if (!pSetOption)
			continue;

		if (pItem->m_nSetWearCnt < pSetOption->nWearCnt)
			continue;

		if (pSetOption->nType == SET_ITEM_OPTION)
		{
			COptionProto* pOptionProto = gserver.m_optionProtoList.FindProto(pSetOption->nIndex);
			if (pOptionProto)
			{
				int nValue = 0;
				int nLevel = pSetOption->nLevel;
				if (pOptionProto->m_levelValue[nLevel - 1])
					nValue = pOptionProto->m_levelValue[nLevel -1];
				
				COption::ApplyOptionValue(this, pOptionProto->m_type, nValue, pItem);
			}
			
			pItem->m_nSetOption = 1;
		}
		else if (pSetOption->nType == SET_ITEM_SKILL)
		{
			CSkill * pSkill = gserver.m_skillProtoList.Create(pSetOption->nIndex, pSetOption->nLevel);
			if(pSkill)
			{
				if (pSkill->m_proto->IsActive())
				{
					m_activeSkillList.Add(pSkill);
				}
				else if (pSkill->m_proto->IsPassive())	
				{
					m_passiveSkillList.Add(pSkill);
				}
				else
				{
					m_etcSkillList.Add(pSkill);
				}
			}

			pItem->m_nSetOption = 2;
		}
	}
	
	if (pItem->m_nSetOption)
	{
		for (i = 0; i < MAX_WEARING; i++)
		{
			if (!m_wearing[i] || !m_wearing[i]->m_pSetItemProto)
				continue;
						
			if (pItem->m_itemProto->GetSetIndex() == m_wearing[i]->m_itemProto->GetSetIndex())
			{
				m_wearing[i]->m_nSetOption = pItem->m_nSetOption;
			}
		}
	}
}

void CPC::RemoveSetItemValue(CItem * pItem)
{
	if (!pItem->m_pSetItemProto || pItem->m_nSetWearCnt <= 0)
		return;

	for (int i = 0; i < pItem->m_pSetItemProto->GetOptionCnt(); i++)
	{
		const SETITEMOPTION* pSetOption = pItem->m_pSetItemProto->GetOption(i);

		if (!pSetOption)
			continue;

		if (pItem->m_nSetWearCnt <= pSetOption->nWearCnt && pSetOption->nType == SET_ITEM_SKILL)
		{
			CSkill * pSkill = gserver.m_skillProtoList.Create(pSetOption->nIndex, pSetOption->nLevel);
			if(pSkill)
			{
				if (pSkill->m_proto->IsActive())
				{
					CNetMsg rmsg;
					pSkill->Cancel(this);
					SkillCancelMsg(rmsg, this);
					m_pArea->SendToCell(rmsg, this, true);
					
					RemoveSkillFromQuickSlot(pSkill);
					m_activeSkillList.Remove(pSkill);
				}
				else if (pSkill->m_proto->IsPassive())	
				{
					m_passiveSkillList.Remove(pSkill);
				}
				else
				{
					m_etcSkillList.Remove(pSkill);
				}
			}
		}
	}
}

void CPC::CountSetItem(CItem* pItem, bool bPlus /* = true */)
{
	CItem * pSameItem = NULL;

	for (int i = 0; i < MAX_WEARING; i++)
	{
		if (!m_wearing[i])
			continue;

		if (pItem == m_wearing[i])
			continue;

		if (pItem->m_itemProto->GetSetIndex() == m_wearing[i]->m_itemProto->GetSetIndex())
		{
			if (bPlus)
			{
				m_wearing[i]->m_nSetWearCnt += 1;
				if (!pSameItem)
					pSameItem = m_wearing[i];
				//if (nCount <= 1)
				//	nCount = m_wearing[i]->m_nSetWearCnt;
			}
			else
			{
				m_wearing[i]->m_nSetWearCnt -= 1;
			}
		}
	}

	if (bPlus)
	{
		if (pSameItem)
		{
			pItem->m_nSetWearCnt = pSameItem->m_nSetWearCnt;
		}
		else
		{
			pItem->m_nSetWearCnt = 1;
		}
	}
}
#endif