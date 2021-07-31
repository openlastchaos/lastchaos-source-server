#include "stdhdrs.h"
#include "Log.h"
#include "Exp.h"
#include "Server.h"
#include "Cmd.h"
#include "CmdMsg.h"
#include "DBCmd.h"
#include "doFunc.h"
#include "Battle.h"
#include "WarCastle.h"
#include "nProtectFunc.h"
#include "DratanCastle.h"

volatile LC_THREAD_T		gThreadIDGameThread;
volatile LC_THREAD_T		gThreadIDDBThread;
CServer gserver;
CCmdList gcmdlist;
CCmdList gexcmdlist;

#ifdef PACKET_TIME_CHECK
CPacketTime gPacketTime;
#endif

void ServerSrandom(unsigned long initial_seed);

CServer::CServer()
: m_serverAddr(HOST_LENGTH + 1)
#ifdef JP_OTAKU_SYSTEM
, m_strGMCommandOTAKUSpeedUp(20)
, m_strGMCommandOTAKUImmortal(20)
#endif // JP_OTAKU_SYSTEM
, m_listParty(CParty::CompParty)
#ifdef PARTY_MATCHING
, m_listPartyMatchMember(CPartyMatchMember::CompByIndex)
, m_listPartyMatchParty(CPartyMatchParty::CompByBossIndex)
#endif // PARTY_MATCHING
#ifdef MESSENGER_NEW
, m_chatList(NULL)
#endif
#ifdef LIMIT_CATALOG
, m_limitCT(NULL)
#endif
#ifdef ENABLE_OXQUIZ
, m_listOXQuiz(COXQuizData::CompOXData)
#endif // ENABLE_OXQUIZ

#ifdef EX_GO_ZONE
, m_listTeleportZone(CTeleportZoneData::CompTeleporter)
#endif // EX_GO_ZONE

#ifdef EXPEDITION_RAID
, m_listExped(CExpedition::CompExpedition)
#endif //EXPEDITION_RAID
{
	m_ssock = (socket_t)-1;
	m_desclist = NULL;
	m_nDesc = 0;
	m_connector = NULL;
	m_messenger = NULL;
	m_helper = NULL;
	FD_ZERO(&m_input_set);
	FD_ZERO(&m_output_set);
	FD_ZERO(&m_exc_set);
	FD_ZERO(&m_null_set);
	
	m_serverpath = GetServerPath();
	m_serverno = -1;
	m_subno = -1;
	m_maxplayers = 0;
	m_bReboot = false;
	m_nameserverisslow = true;
	m_bShutdown = false;

	m_pulseReboot = -1;
	m_pulseShutdown = -1;
	m_pulseEndGame = -1;

#ifdef USING_GMIP_TABLE
	m_nGMIP = 0;
	m_GMIP = NULL;
#endif
	m_bLoop = 0;
	m_descLoop = NULL;
	m_observer = NULL;
	memset(m_chatmonitor, 0, sizeof(CDescriptor*) * 20);

#ifdef GMTOOL
	m_gmtool = NULL;
#endif // GMTOOL
	
	m_pulse = 0;
#ifdef LC_TIME
	GetServerTime();
#else
	m_gameTime = GetServerTime();
#endif
	m_resetWeather = 0;
	m_resetAdjustItem = 0;
	
	mysql_init(&m_dbchar);
	mysql_init(&m_dbdata);
#ifdef ENABLE_WAR_CASTLE
	mysql_init(&m_dbcastle);
#endif
	
	m_brunconnector = false;
	m_brunmessenger = false;	// 메신저 수행 플래그
	
	m_numZone = 0;
	m_zones = NULL;

	m_clientversionMin = 0;
	m_clientversionMax = 0;

	m_bDoubleEvent = false;
	
#ifdef NEW_DOUBLE_GM_ZONE
	m_bDoubleEventZone = -1;
#endif // NEW_DOUBLE_GM_ZONE

#ifdef GUILD_MARK_TABLE
	for(int i=0; i < 3; i++)
	{
		m_nGuildMarkTable[i] = 0;
	}	
#endif // GUILD_MARK_TABLE

#ifdef NEW_DOUBLE_GM
	m_bDoubleNasPercent = DEFAULT_NAS_PERCENT;
	m_bDoubleNasGetPercent = DEFAULT_NAS_GET_PERCENT;
	m_bDoubleExpPercent = DEFAULT_EXP_PERCENT;
	m_bDoubleSpPercent = DEFAULT_SP_PERCENT;
	m_bDoubleProducePercent = DEFAULT_PRO_PERCENT;
	m_bDoubleProduceNum = DEFAULT_PRO_GET_NUMBER;
#endif

#ifdef CREATE_EVENT
	ClearCreateEvent();	
#endif // CREATE_EVENT

#ifdef EVENT_DROPITEM
	m_bDropItem			= false;
	m_bDropItemNpcIdx	= -1;
	m_bDropItemIdx		= -1;
	m_bDropProb			= 0;
#endif // EVENT_DROPITEM

#ifdef CHANCE_EVENT
	m_bChanceEvent = false;
	m_bChanceSlevel = -1;
	m_bChanceElevel = -1;
	m_bChanceNasPercent = DEFAULT_NAS_PERCENT;
	m_bChanceNasGetPercent = DEFAULT_NAS_GET_PERCENT;
	m_bChanceExpPercent = DEFAULT_EXP_PERCENT;
	m_bChanceSpPercent = DEFAULT_SP_PERCENT;
	m_bChanceProducePercent = DEFAULT_PRO_PERCENT;
	m_bChanceProduceNum = DEFAULT_PRO_GET_NUMBER;
#endif // CHANCE_EVENT

#ifdef EVENT_COLLECT_BUG_DROP
#ifdef LC_JPN
	m_CollectBugEventPercent = 50;
#else
	m_CollectBugEventPercent = 10;
#endif // LC_JPN
#endif // EVENT_COLLECT_BUG_DROP

	m_bDoubleExpEvent = false;
	m_expPercent = 100;
	m_bLattoEvent = false;
	m_bNewYearEvent = false;
	m_bValentineEvent = false;
#ifdef EVENT_LETTER
	m_bLetterEvent = false;
#endif

#ifdef EVENT_XMAS_2005
	m_bXMas2005Event = EVENT_XMAS_2005_DEFAULT;
#endif

#ifdef EVENT_NEWYEAR_2006
	m_bNewYear2006Event = EVENT_NEWYEAR_2006_TIME;
#endif

#ifdef EVENT_OPEN_BETA_TLD
	m_bOpenEvent = false;
	m_bSoilDrop = 8;
#endif
#ifdef EVENT_FLOWER
	m_bFlowerEvent = false;
	m_bFlower = 0;
#endif
#ifdef LC_TLD
	time_t t = 0;
	memcpy(&m_now, localtime(&t), sizeof( struct tm ) );
	//m_now = localtime(&t);
#endif

#ifdef EVENT_SEARCHFRIEND
	m_bSearchFriendEvent = EVENT_SEARCHFRIEND_TIME;
#endif

#ifdef EVENT_MOONSTONE
	m_nMoonStoneNas = 0;
	m_bMoonStoneEvent = false;
#endif

#ifdef EVENT_JPN_2007_NEWSERVER
	m_bEventJPN2007NewServer = false;
#endif // EVENT_JPN_2007_NEWSERVER

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
	m_bEventJPN2007TreasureboxAddItem = false;	// 일본 2007년 신규 서버 오픈시 보물상자 이벤트 추가 아이템 추가
#endif //EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

	m_nItemDropEventRate = 100;

	m_itemDropProb = 100;
	m_moneyDropProb = 100;

#ifdef ENABLE_STATISTICS
	m_statisticsItemBuy = 0;
	m_statisticsItemSell = 0;
	m_statistics152 = 0;
	m_statistics153 = 0;
	m_statistics154 = 0;
	m_statistics155 = 0;
	m_statistics156 = 0;
	m_statistics157 = 0;
	m_statistics158 = 0;
	m_statistics159 = 0;
	m_statistics160 = 0;
	m_statistics161 = 0;
	m_statistics162 = 0;
	m_statistics163 = 0;
	m_statistics197 = 0;
	m_statistics198 = 0;
	m_statistics199 = 0;
	m_statisticsPulse = 0;
#endif // #ifdef ENABLE_STATISTICS

#ifdef ENABLE_WAR_CASTLE
#ifdef TLD_WAR_TEST
	m_warnotice = false;
#endif
	m_taxItem = 0;
	m_taxProduceCastle = 0;
	m_taxSavePulse = 0;
	m_taxDivGuild = -1;
#endif // #ifdef ENABLE_WAR_CASTLE
	
#ifdef DRATAN_CASTLE
	m_taxItemDratan = 0;
	m_taxProduceCastleDratan = 0;
#endif // DRATAN_CASTLE

#ifdef EVENT_XMAS_2007
	m_unEventXmasTreePoint = 0;
#endif // EVENT_XMAS_2007

#ifdef EVENT_VALENTINE_2006
	m_bEventValentine2006 = EVENT_VALENTINE_2006_START;
#endif // #ifdef EVENT_VALENTINE_2006

#ifdef ENABLE_OXQUIZ
	m_bEventOX = false;
#endif // ENABLE_OXQUIZ

#ifdef RECOMMEND_SERVER_SYSTEM
	m_bRecommend = false;
	m_pulseRecommendMoonstone = m_pulse;
#endif // RECOMMEND_SERVER_SYSTEM

#ifdef LC_TWN
	m_national = LC_TWN;
#ifdef LC_TWN2
	m_national = LC_TWN2;
#endif
#elif defined (LC_TLD)
	m_national = LC_TLD;
#elif defined (LC_TLD_ENG)
	m_national = LC_TLD_ENG;
#elif defined (LC_JPN)
	m_national = LC_JPN;
#elif defined (LC_MAL)
	m_national = LC_MAL;
#elif defined (LC_USA)
	m_national = LC_USA;
#elif defined (LC_BRZ)
	m_national = LC_BRZ;
#elif defined (LC_HBK)
	m_national = LC_HBK;	
#elif defined (LC_GER)
	m_national = LC_GER;
#elif defined (LC_SPN)
	m_national = LC_SPN;
#elif defined (LC_FRC)
	m_national = LC_FRC;
#elif defined (LC_PLD)
	m_national = LC_PLD;	
#elif defined (LC_RUS)
	m_national = LC_RUS;	
#else 
	m_national = LC_KOR;
#endif

#ifdef EVENT_PROMOTION_SITE
	m_bpSiteGive = false;
	m_bpSiteCount = 300;
#endif

	m_bLoadPartyInfo = false;

	m_pulseProcHeartBeatPerSec = 0;

#ifdef GUILD_RANKING
	m_nPulseSaveExpGuild = PULSE_GUILD_RANKING_UPDATE;
#endif // GUILD_RANKING
#ifdef NON_PK_SYSTEM
	m_bNonPK = false;
#endif	// NON_PK_SYSTEM

#ifdef FREE_PK_SYSTEM
	m_bFreePk = false;
#endif

#ifdef MAL_DISABLE_PKPENALTY
	m_bDisablePKPaenalty = false;
#endif //MAL_DISABLE_PKPENALTY

#ifdef USING_NPROTECT
	m_pulseUpdateGG = PULSE_NPROTECT_UPDATE;
#endif // USING_NPROTECT

#ifdef DOUBLE_ITEM_DROP
	m_bDoubleItemEvent = false;
	m_bDoubleItemPercent = 0;
#endif	// DOUBLE_ITEM_DROP
	
#ifndef LIMIT_EXP_INCREASE
#if defined (LC_JPN)
	m_nExpLimit = 10000;
#else
	m_nExpLimit = DEFAULT_LIMIT_EXP;
#endif // (LC_JPN)
#endif // LIMIT_EXP_INCREASE
	
#ifdef MONSTER_COMBO
	m_comboZone = NULL;
#endif 

#ifdef NEW_DOUBLE_EVENT_AUTO
#if defined (LC_JPN)
	m_bDoubleEventAuto = false;
#else
	m_bDoubleEventAuto = true;
#endif
	m_bDoubleEventAutoOn = false;
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef UPGRADE_EVENT
	m_nUpgradeProb = 100;
	m_bUpgradeEvent = false;
#endif // UPGRADE_EVENT
}

CServer::~CServer()
{
#ifdef USING_GMIP_TABLE
	if (m_GMIP)
		delete [] m_GMIP;
	m_GMIP = NULL;
	m_nGMIP = 0;
#endif
	if (m_zones)
		delete[] m_zones;
	m_zones = NULL;
	m_numZone = 0;
	if (m_serverpath)
		delete[] m_serverpath;
	m_serverpath = NULL;

	while (m_listParty.GetCount() > 0)
	{
		CParty* pParty = m_listParty.GetData(m_listParty.GetHead());
		m_listParty.Remove(m_listParty.GetHead());
		delete pParty;
	}

#ifdef EXPEDITION_RAID 	
	while (m_listExped.GetCount() > 0)
	{
		CExpedition* pExped = m_listExped.GetData(m_listExped.GetHead());
		m_listExped.Remove(m_listExped.GetHead());
		delete pExped;
	}
#endif //EXPEDITION_RAID

#ifdef PARTY_MATCHING
	while (m_listPartyMatchMember.GetCount() > 0)
	{
		CPartyMatchMember* pMember = m_listPartyMatchMember.GetData(m_listPartyMatchMember.GetHead());
		m_listPartyMatchMember.Remove(m_listPartyMatchMember.GetHead());
		delete pMember;
	}

	while (m_listPartyMatchParty.GetCount() > 0)
	{
		CPartyMatchParty* pParty = m_listPartyMatchParty.GetData(m_listPartyMatchParty.GetHead());
		m_listPartyMatchParty.Remove(m_listPartyMatchParty.GetHead());
		delete pParty;
	}
#endif // PARTY_MATCHING

#ifdef ENABLE_OXQUIZ
	while (m_listOXQuiz.GetCount() > 0)
	{
		COXQuizData* pQuiz = m_listOXQuiz.GetData(m_listOXQuiz.GetHead());
		delete pQuiz;
		m_listOXQuiz.Remove(m_listOXQuiz.GetHead());
	}
#endif // ENABLE_OXQUIZ

}

char* CServer::GetServerPath()
{
	char szBuffer[1024];
#ifdef CIRCLE_WINDOWS
	GetModuleFileName(::GetModuleHandle(NULL), szBuffer, 1000);
	int path_len = strlen(szBuffer);
	int i;
	
	for (i = path_len - 1; i >= 0; i-- ) {
		if (szBuffer[i] == '\\')  {
			szBuffer[i+1] = '\0';
			break;
		}
	}
	if (i < 0)
		return false;
#else
	getcwd (szBuffer, 512);
	strcat (szBuffer, "/");
#endif
	
	char* ret = new char[strlen(szBuffer) + 1];
	strcpy(ret, szBuffer);
	return ret;
}

bool CServer::LoadSettingFile()
{
	puts("Load setting file....");
	
	CLCString strTemp(1024);
	
	strTemp.Format("%s%s", m_serverpath, "data/newStobm.bin");
	
	if (!m_config.Load(strTemp))
		return false;
	
	m_serverno = atoi(m_config.Find("Server", "Number"));
	m_subno = atoi(m_config.Find("Server", "SubNumber"));
	
#ifdef  NON_PK_SYSTEM 

#if defined ( LC_JPN )
	if( m_subno == 4 || m_subno == 2 )
	{
		m_bNonPK = true;
	}
#elif defined ( LC_USA )
	if( (m_serverno == 1 && (m_subno == 5 || m_subno == 6)) ||
		(m_serverno == 2 && (m_subno == 3 || m_subno == 4)) ||
		(m_serverno == 3 && (m_subno == 3 || m_subno == 4)) ||
		(m_serverno == 4 && (m_subno == 3 || m_subno == 4)) ||
		(m_serverno == 5 && (m_subno == 3 || m_subno == 4)) )
	{
		m_bNonPK = true;
	}
#elif defined(LC_GER) || defined (LC_EUR)
	if(m_subno == 2 || m_subno == 5)
	{
		m_bNonPK = true;
	}
#elif defined(LC_MAL)
	if(m_subno == 3)
	{
		m_bNonPK = true;
	}
#endif
#endif  // NON_PK_SYSTEM

#if defined FREE_PK_SYSTEM
#if defined ( LC_TLD )
	if( m_serverno == 4 )
		m_bFreePk = true;
#endif // LC_TLD		
#endif // FREE_PK_SYSTEM

#if defined MAL_DISABLE_PKPENALTY
	
#if defined ( LC_MAL )
	if( m_serverno == 1 && ( m_subno == 3 || m_subno == 4) )
#elif defined ( LC_BRZ )
	if(m_subno == 2)
#endif // LC_MAL
		m_bDisablePKPaenalty = true;

#if defined (HSTEST)
	m_bDisablePKPaenalty = true;
#endif //HSTEST

#endif

	if (strcmp(m_config.Find("Server", "AllowExternalIP"), "FALSE") == 0)
		m_bOnlyLocal = true;
	else
		m_bOnlyLocal = false;

#ifdef NON_PK_SYSTEM	
	if (strcmp(m_config.Find("Server", "NON_PK"), "TRUE") == 0)
		m_bNonPK = true;
	//else
	//	m_bNonPK = false;
#endif

#ifdef GMTOOL
	if(gserver.m_serverno == 1 && gserver.m_subno == 1)
	{
		const char* p = m_config.Find("GMTOOL", "count");
		if(!p)
			return false;
		m_nGMToolCount = atoi(p);
		if(m_nGMToolCount <= 0)
		{
			m_gmtool = NULL;
		}
		else
		{
			m_gmtool = new CDescriptor*[m_nGMToolCount];
			int i;
			for(i = 0; i < m_nGMToolCount; ++i) m_gmtool[i] = NULL;
		}
	}
#endif // GMTOOL
	
	return true;
}

bool CServer::LoadSettings()
{
	int i;

	GAMELOG << init("SYSTEM")
		<< "Load settings...."
		<< end;

	// version information loading
	GAMELOG << init("SYSTEM")
			<< "Client Version Loading..."
			<< end;

	CDBCmd dbcmd;
	dbcmd.Init(&m_dbdata);
	dbcmd.SetQuery("SELECT a_min, a_max FROM t_clientversion");
	if (!dbcmd.Open() || !dbcmd.MoveFirst() || !dbcmd.GetRec("a_min", m_clientversionMin) || !dbcmd.GetRec("a_max", m_clientversionMax))
		return false;

#ifdef EVENTSETTING
	if(!m_EventSetting.Load())
		return false;

	if(!LoadNotice())
		return false;
#endif // EVENTSETTING

#ifdef TEST_SERVER

	if (DropProbLoad())
	{
		GAMELOG << init("SYSTEM")
			<< "Drop Prob Loading..."
			<< end;
	}

#endif
	
	// item proto loading
	GAMELOG << init("SYSTEM")
			<< "Item Loading..."
			<< end;
	if (!m_itemProtoList.Load())
		return false;

#ifdef SET_ITEM
	GAMELOG << init("SYSTEM")
			<< "Set Item Loading..."
			<< end;
	if (!m_setItemProtoList.Load())
		return false;
#endif

#ifdef FACTORY_SYSTEM
	GAMELOG << init("SYSTEM")
			<< "Factory Item Loading..."
			<< end;
	if (!m_factoryItemProtoList.Load())
		return false;
#endif

#ifdef FEEITEM
	GAMELOG << init("SYSTEM")
			<< "CashItem Loading..."
			<< end;
	if( !m_catalogList.Load() )
		return false;
#endif // FEEITEM
	// Apet proto loading

#ifdef ATTACK_PET
	GAMELOG << init("SYSTEM")
		<< "Apet Loading..."
		<< end;
	if (!m_pApetlist.Load())
		return false;
#endif // ATTACK_PET
	
	// skill proto loading
	GAMELOG << init("SYSTEM")
		<< "Skill Loading..."
		<< end;
	if (!m_magicProtoList.Load())
		return false;
	if (!m_skillProtoList.Load())
		return false;
	
	// npc proto loading
	GAMELOG << init("SYSTEM")
		<< "Npc Loading..."
		<< end;
	if (!m_npcProtoList.Load())
		return false;
	
	// quest proto loading
	GAMELOG << init("SYSTEM")
		<< "Quest Loading..."
		<< end;
	if (!m_questProtoList.Load())
		return false;
	
	// Option Proto Loading
	GAMELOG << init("SYSTEM")
		<< "Option Loading..."
		<< end;
	if (!m_optionProtoList.Load())
		return false;

#ifdef MONSTER_RAID_SYSTEM
	// rare option proto loading
	GAMELOG << init("SYSTEM")
			<< "Rare Option Loading..."
			<< end;
	if (!m_rareOptionList.Load())
		return false;
#endif // MONSTER_RAID_SYSTEM

	// SSpecial Skill Proto Loading
	GAMELOG << init("SYSTEM")
		<< "Special Skill Loading..."
		<< end;

	if (!m_sSkillProtoList.Load())
		return false;

	// OX Quiz Loading
	GAMELOG << init("SYSTEM")
		<< "OX Quiz Loading..."
		<< end;

#ifdef ENABLE_OXQUIZ
	if (!LoadOXQuiz())
		return false;
#endif // ENABLE_OXQUIZ

#ifdef EVENT_NEW_MOONSTONE
	if( !m_moonstoneReward.Load())
		return false;
#endif // EVENT_NEW_MOONSTONE
	
#ifdef REQUITAL_EVENT	// 보상 이벤트 
	GAMELOG << init("SYSTEM")
			<< "Requital_Event Loading..."
			<< end;
	if( !m_CRequital_EventList.Load_Requital_Event())
		return false;
#endif // REQUITAL_EVENT

#ifdef TRIGER_SYSTEM	// 트리거 사용
	GAMELOG << init("SYSTEM")
			<< "Triger System Loading..."
			<< end;
	if( !m_CTriger_List.Load_Triger())
		return false;
#endif // TRIGER_SYSTEM

#ifdef EX_GO_ZONE
	GAMELOG << init("SYSTEM")
			<< "Teleporter Loading..."
			<< end;
	if(!LoadTeleportZone())
		return false;

	GAMELOG <<  init("SYSTEM")
			<< "Teleport Tax Loading..."
			<< end;

	if(!m_TeleportTaxList.Load())
		return false;

#endif // EX_GO_ZONE
	
#ifdef EVENT_OPEN_ADULT_SERVER
	GAMELOG << init("SYSTEM")
			<< "Open AdutServer Event Item Loading..."
			<< end;
	if(!m_ItemUpgradeList.Load())
		return false;
#endif // EVENT_OPEN_ADULT_SERVER
	// Loading Zone
	m_numZone = atoi(m_config.Find("Zones", "Count"));
	if (m_numZone == 0)
		return false;
	m_zones = new CZone[m_numZone];
	for (i = 0; i < m_numZone; i++)
	{
		if (!m_zones[i].LoadZone(i))
			return false;

#ifdef DRATAN_CASTLE
#ifdef DYNAMIC_DUNGEON
		if(m_zones[i].m_index == 21)
		{	// 공성 보상 던전
			CDratanCastle * pCastle = CDratanCastle::CreateInstance();
			if( pCastle != 0)
			{
				pCastle->m_dvd.SetZone( &m_zones[i] );
			}
		}
#endif //DYNAMIC_DUNGEON
#endif // DRATAN_CASTLE
		
		// Shop Proto Loading
		if (!m_zones[i].LoadShop())
			return false;
	}
	
	for (i = 0; i < m_numZone; i++)
	{
		if (!m_zones[i].LoadNPC())
			return false;
	}
	
#ifdef RAID
	int nInCount,nInRoom;
	nInCount = nInRoom = 0;

	nInCount = atoi(m_config.Find("InZones", "Count"));
	nInRoom  = atoi(m_config.Find("InZones", "Room"));

	m_numInZone = nInCount * nInRoom;	  //인존개수:암흑의제단,예배당(2) * 룸수(20)
	m_inzones	= new CZone[m_numInZone];

	//암흑의제단
	//-->
	int nLoadSeq,nInZoneSeq,nRoomNo;

	nLoadSeq = 0; nRoomNo = 1;
	for (nInZoneSeq = 0; nInZoneSeq < nInRoom; nInZoneSeq++)
	{
		if (!m_inzones[nInZoneSeq].LoadInZone(nLoadSeq))
			return false;

		m_inzones[nInZoneSeq].SetInZone(nRoomNo, MSG_JOINTYPE_EXPED, MSG_RECYCLETYPE_7DAY);

		if (!m_inzones[nInZoneSeq].LoadNPC())
			return false;

		nRoomNo++;
	}
	//<--

	//예배당
	//-->
	nLoadSeq = 1;  nRoomNo = 1;
	for (nInZoneSeq; nInZoneSeq < (nInRoom * 2); nInZoneSeq++)
	{
		if (!m_inzones[nInZoneSeq].LoadInZone(nLoadSeq))
			return false;

		m_inzones[nInZoneSeq].SetInZone(nRoomNo, MSG_JOINTYPE_EXPED, MSG_RECYCLETYPE_7DAY);

		if (!m_inzones[nInZoneSeq].LoadNPC())
			return false;

		nRoomNo++;
	}
	//<--
#endif //RAID


#ifdef MONSTER_COMBO
	GAMELOG << init("SYSTEM")
			<< "MISSIONCASE Loading..."
			<< end;

	if(!m_missionCaseList.LoadList())
		return false;
#endif 

#ifdef ENABLE_WAR_CASTLE
	GAMELOG << init("Load Castle Information...") << end;
	if (!LoadCastleData())
	{
		GAMELOG << init("ERROR: CANNOT LOAD CASTLE INFO") << end;
		return false;
	}
#endif

#ifdef EXTREME_CUBE
	GAMELOG << init("SYSTEM")
			<< "Load Cube Data"
			<< end;

	if(!m_extremeCube.Load())
	{
		GAMELOG << init("ERROR: CANNOT LOAD EXTREME CUBE DATA") << end;
		return false;
	}

#endif // EXTREME_CUBE

#ifdef USING_GMIP_TABLE
	LoadGMIPTable();
#endif
	
#ifdef EVENTSETTING
#else

#ifdef EVENT_LATTO
	gserver.m_bLattoEvent = true;
#endif

#ifdef EVENT_NEWYEAR
	gserver.m_bNewYearEvent= true;
#endif

#ifdef EVENT_VALENTINE
	gserver.m_bValentineEvent= true;
#endif

#ifdef EVENT_WHITEDAY
	gserver.m_bWhiteDayEvent = true;
#endif

#ifdef EVENT_LETTER
	m_bLetterEvent = true;
#endif

#ifdef EVENT_MOONSTONE
	gserver.m_bMoonStoneEvent = true;
#endif

#ifdef EVENT_RICESOUP
	m_bRiceShoupEvent = true;
#endif

//0704 이벤트에 관한 공지 세팅.
#ifdef NOTICE_EVENT
	i =0;
	memset(m_aNotice, 0, sizeof(int) * 5);

#ifdef EVENT_TREASUREBOX
	m_aNotice[i] = EVENT_TREASUREBOX;
	
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
	//FILE* fpEventJPN2007TreasureBox = fopen(".event_2007_treasurebox", "rt");
	FILE* fpEventJPN2007TreasureBox = fopen("event_2007_treasurebox", "rt");

	if (fpEventJPN2007TreasureBox)
	{
		char buf[10] = "";
		fgets(buf, 10, fpEventJPN2007TreasureBox);
		fclose(fpEventJPN2007TreasureBox);
		buf[9] = '\0';
		if (atoi(buf) == m_serverno)
		{
			m_bEventJPN2007TreasureboxAddItem = true;
			m_aNotice[i] = EVENT_JPN_2007_TREASUREBOX_ADD_ITEM;
		}	
	}
#endif // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

	i++;
#endif // #ifdef EVENT_TREASUREBOX
	
#if defined(EVENT_TREASUREBOX_RED) && !defined(EVENT_SUMMER_2008)
	m_aNotice[i] = EVENT_TREASUREBOX_RED;
	i++;
#endif // EVENT_TREASUREBOX_RED

//#ifdef EVENT_TEACH
//	m_aNotice[i] = EVENT_TEACH;
//	i++;
//#endif // #ifdef EVENT_TEACH

#ifdef EVENT_FRUIT_WATERMELON
	m_aNotice[i] = EVENT_FRUIT_WATERMELON;
	i++;
#endif // #ifdef EVENT_FRUIT_WATERMELON

#ifdef EVENT_FRUIT_MELON
	m_aNotice[i] = EVENT_FRUIT_MELON;
	i++;
#endif // #ifdef EVENT_FRUIT_MELON

#ifdef EVENT_FRUIT_PLUM
	m_aNotice[i] = EVENT_FRUIT_PLUM;
	i++;
#endif // #ifdef EVENT_FRUIT_PLUM

#ifdef EVENT_CHUSEOK
	m_aNotice[i] = EVENT_CHUSEOK;
	i++;
#endif // #ifdef EVENT_CHUSEOK

#ifdef EVENT_SEPTEMBER
	m_aNotice[i] = EVENT_SEPTEMBER;
	i++;
#endif // #ifdef EVENT_SEPTEMBER

#ifdef EVENT_PEPERO
	m_aNotice[i] = EVENT_PEPERO;
	i++;
#endif // #ifdef EVENT_PEPERO

#ifdef EVENT_XMAS_2005
	m_aNotice[i] = EVENT_XMAS_2005;
	i++;
#endif // #ifdef EVENT_XMAS_2005

#ifdef EVENT_NEWYEAR_2006
	m_aNotice[i] = EVENT_NEWYEAR_2006;
	i++;
#endif // #ifdef EVENT_NEWYEAR_2006

#ifdef EVENT_CHANGE_ARMOR_2005
	m_aNotice[i] = EVENT_CHANGE_ARMOR_2005;
	i++;
#endif // #ifdef EVENT_CHANGE_ARMOR_2005

#ifdef EVENT_SEARCHFRIEND
	m_aNotice[i] = EVENT_SEARCHFRIEND;
	i++;	
#endif // #ifdef EVENT_SEARCHFRIEND

#ifdef EVENT_VALENTINE_2006
	m_aNotice[i] = EVENT_VALENTINE_2006;
	i++;	
#endif // #ifdef EVENT_VALENTINE_2006

#ifdef EVENT_WHITEDAY_2006
	m_aNotice[i] = EVENT_WHITEDAY_2006;
	i++;	
#endif // #ifdef EVENT_WHITEDAY_2006

#ifdef EVENT_NEW_SERVER_2006_OLDSERVER
#ifdef BSTEST
	m_aNotice[i] = EVENT_NEW_SERVER_2006_OLDSERVER;
	i++;
#else // BSTEST
	if (gserver.m_serverno <= EVENT_NEW_SERVER_2006_SERVERNO)
	{
		m_aNotice[i] = EVENT_NEW_SERVER_2006_OLDSERVER;
		i++;
	}
#endif // BSTEST
#endif // EVENT_NEW_SERVER_2006_OLDSERVER

#ifdef EVENT_NEW_SERVER_2006_NEWSERVER
#ifdef BSTEST
	m_aNotice[i] = EVENT_NEW_SERVER_2006_NEWSERVER;
	i++;
#else // BSTEST
	if (gserver.m_serverno >= EVENT_NEW_SERVER_2006_SERVERNO)
	{
		m_aNotice[i] = EVENT_NEW_SERVER_2006_NEWSERVER;
		i++;
	}
#endif // BSTEST
#endif // EVENT_NEW_SERVER_2006_NEWSERVER

#ifdef EVENT_OX_QUIZ
	m_aNotice[i] = EVENT_OX_QUIZ;
	i++;
#endif // EVENT_OX_QUIZ

#ifdef EVENT_WORLDCUP_2006
	m_aNotice[i] = EVENT_WORLDCUP_2006;
	i++;
#endif // EVENT_WORLDCUP_2006

#ifdef EVENT_OPEN_BETA_TLD
	struct tm start = NOW();
	struct tm stop = NOW();

	start.tm_year = 2005 - 1900;
	start.tm_mon = 10;
	start.tm_mday = 18;
	start.tm_hour = 0;
	start.tm_min = 0;
	start.tm_sec = 0;
	
	stop.tm_year = 2005 - 1900;
	stop.tm_mon = 11;
	stop.tm_mday = 6;
	stop.tm_hour = 0;
	stop.tm_min = 0;
	stop.tm_sec = 0;

	time_t t_now = mktime(&tm_now);
	if( mktime(&start) <= t_now && mktime(&stop) > t_now)
	{
		m_bOpenEvent = true;

		// 0 : 모두 드롭 1: 배양토 2: 레드 3: 푸른 4: 황금 5: 배양,레드 6:레드,푸른 7:푸른,골드 8: 모두 안 드롭
		if(tm_now.tm_mday >= 18 && tm_now.tm_mday < 21)
			m_bSoilDrop = 1;
		if(tm_now.tm_mday >= 21 && tm_now.tm_mday < 23)
			m_bSoilDrop = 5;
		if(tm_now.tm_mday >= 23 && tm_now.tm_mday < 25)
			m_bSoilDrop = 2;
		if(tm_now.tm_mday >= 25 && tm_now.tm_mday < 27)
			m_bSoilDrop = 6;
		if(tm_now.tm_mday >= 27 && tm_now.tm_mday < 28)
			m_bSoilDrop = 3;
		if(tm_now.tm_mday >= 28 && tm_now.tm_mday < 30)
			m_bSoilDrop = 7;
		if(tm_now.tm_mday == 30 || (tm_now.tm_mday >= 1 && tm_now.tm_mday < 3))
			m_bSoilDrop = 4;
		
		if(tm_now.tm_mday == 3 || tm_now.tm_mday == 4 || tm_now.tm_mday == 5)
		{
			m_bSoilDrop = 0;
		}

	}
#endif
#ifdef EVENT_FLOWER
	if ( tm_now.tm_year == (2005 - 1900) && tm_now.tm_mon == 11 && tm_now.tm_mday >= 5 && tm_now.tm_mday <= 12 )
	{
		m_bFlowerEvent = true;
		if ( tm_now.tm_mday == 5 )
			m_bFlower = 1;
		else
			m_bFlower = 0;
	}
#endif
#ifdef EVENT_SAKURA
	m_aNotice[i] = EVENT_SAKURA;
	i++;
#endif

#if defined(EVENT_RAIN_2006) && !defined(EVENT_SUMMER_2008)
	m_aNotice[i] = EVENT_RAIN_2006;
	i++;
#endif // EVENT_RAIN_2006 && && !defined(EVENT_SUMMER_2008)

#ifdef EVENT_TLD_BUDDHIST
	m_aNotice[i] = EVENT_TLD_BUDDHIST;
	i++;
#endif // EVENT_TLD_BUDDHIST

#if defined(EVENT_COLLECT_BUG) && defined(EVENT_COLLECT_BUG_DROP)
	m_aNotice[i] = EVENT_COLLECT_BUG;	
	i++;
#endif // #if defined(EVENT_COLLECT_BUG) && defined(EVENT_COLLECT_BUG_DROP)

#ifdef EVENT_NEWSERVER_BASTARD
#if defined(BSTEST) || defined(LC_MAL)
	static const int		nNewServerNo = 1;
#else // BSTEST
	static const int		nNewServerNo = 5;
#endif // BSTEST
	if (m_serverno == nNewServerNo)
		m_aNotice[i] = EVENT_NEWSERVER_BASTARD;
	else
		m_aNotice[i] = EVENT_NEWSERVER_BASTARD_OLD_NOTICE;
	i++;
#endif // EVENT_NEWSERVER_BASTARD

#ifdef EVENT_CHUSEOK_2006
	m_aNotice[i] = EVENT_CHUSEOK_2006;
	i++;
#endif // EVENT_CHUSEOK_2006

#ifdef EVENT_HALLOWEEN_2006
#ifdef LC_BRZ
#else
	m_aNotice[i] = EVENT_HALLOWEEN_2006;
	i++;
#endif // LC_BRZ

#endif // EVENT_HALLOWEEN_2006

#ifdef EVENT_XMAS_2006
#if defined (LC_BRZ)
#else
	m_aNotice[i] = EVENT_XMAS_2006;
	i++;
#endif // defined (LC_BRZ)
#endif // EVENT_XMAS_2006
		
#ifdef EVENT_CHILDERN_DAY
	m_aNotice[i] = EVENT_CHILDERN_DAY;
	i++;
#endif // EVENT_CHILDERN_DAY
	
#ifdef EVENT_VALENTINE_2007
	m_aNotice[i] = EVENT_VALENTINE_2007;
	i++;
#endif	// EVENT_VALENTINE_2007

#ifdef EVENT_WHITEDAY_2007
	m_aNotice[i] = EVENT_WHITEDAY_2007;
	i++;
#endif // WHITE_DAY [3/6/2007 KwonYongDae]

#ifdef EVENT_TLD_2007_SONGKRAN
	m_aNotice[i] = EVENT_TLD_2007_SONGKRAN;
	i++;
#endif //EVENT_TLD_2007_SONGKRAN

#ifdef EVENT_EGGS_HUNT_2007
	m_aNotice[i] = EVENT_EGGS_HUNT_2007;
	i++;
#endif // EVENT_EGGS_HUNT_2007

#ifdef EVENT_2007_PARENTSDAY
	m_aNotice[i] = EVENT_2007_PARENTSDAY;
	i++;
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_JPN_2007_NEWSERVER

	//FILE* fpEventJPN2007NewServer = fopen(".event_2007_newserver", "rt");
	FILE* fpEventJPN2007NewServer = fopen("event_2007_newserver", "rt");

	if (fpEventJPN2007NewServer)
	{
		char buf[10] = "";
		fgets(buf, 10, fpEventJPN2007NewServer);
		fclose(fpEventJPN2007NewServer);
		buf[9] = '\0';
		if (atoi(buf) == m_serverno)
		{
			CDBCmd cmdCheckCharDBForEvent;
			cmdCheckCharDBForEvent.Init(&m_dbchar);

			// 필드 검사
			cmdCheckCharDBForEvent.SetQuery("SELECT a_index, a_userindex, a_charindex, a_date, a_give FROM t_event_2007_newserver LIMIT 1");
			if (!cmdCheckCharDBForEvent.Open())
			{
				GAMELOG << init("Error: Character DB Check: Not Found t_event_2007_newserver!!") << end;
				return false;
			}

			// 현재 수 검사
			cmdCheckCharDBForEvent.SetQuery("SELECT COUNT(a_index) AS a_cnt FROM t_event_2007_newserver");
			if (cmdCheckCharDBForEvent.Open() && cmdCheckCharDBForEvent.MoveFirst())
			{
				int nCountEvent = 0;
				cmdCheckCharDBForEvent.GetRec("a_cnt", nCountEvent);
				if (nCountEvent < EVENT_JPN_2007_NEWSERVER_NAS10K)
				{
					m_bEventJPN2007NewServer = true;
				}
			}
		}
	}
#endif // EVENT_JPN_2007_NEWSERVER

#ifdef EVENT_GOMDORI_2007

#ifdef LC_BRZ
#else
	m_aNotice[i] = EVENT_GOMDORI_2007;
	i++;
#endif // LC_BRZ

#endif // EVENT_GOMDORI_2007

#ifdef EVENT_CHILDRENSDAY_2007
	m_aNotice[i] = EVENT_CHILDRENSDAY_2007;
	i++;
#endif // EVENT_CHILDRENSDAY_2007

#ifdef EVENT_FLOWERTREE_2007
#ifndef EVENT_NODROP_FLOWERTREE_2007
	m_aNotice[i] = EVENT_FLOWERTREE_2007;
	i++;
#endif //EVENT_NODROP_FLOWERTREE_2007
#endif //EVENT_FLOWERTREE_2007

#ifdef EVENT_TEACH_2007
	m_aNotice[i] = EVENT_TEACH_2007;
	i++;
#endif // EVENT_TEACH_2007

#ifdef EVENT_UCC_2007
	m_aNotice[i] = EVENT_UCC_2007;
	i++;
#endif // EVENT_UCC_2007

#ifdef EVENT_INDEPENDENCE_DAY_2007_USA
	m_aNotice[i] = EVENT_INDEPENDENCE_DAY_2007_USA;
	i++;
#endif // EVENT_INDEPENDENCE_DAY_2007_USA


#ifdef EVENT_SUMMER_VACATION_2007
	m_aNotice[i] = EVENT_SUMMER_VACATION_2007;
	i++;
#endif // EVENT_SUMMER_VACATION_2007

#ifdef EVENT_TLD_MOTHERDAY_2007
	m_aNotice[i] = EVENT_TLD_MOTHERDAY_2007;
	i++;
#endif // EVENT_TLD_MOTHERDAY_2007

#if defined(EVENT_OPEN_ADULT_SERVER) && !(EVENT_SUMMER_2008)
	m_aNotice[i] = EVENT_OPEN_ADULT_SERVER;
	i++;
#endif // EVENT_OPEN_ADULT_SERVER
	
#ifdef EVENT_LC_1000DAY 
	m_aNotice[i] = EVENT_LC_1000DAY;
	i++;
#endif // EVENT_RICHYEAR_2007

#ifdef EVENT_RICHYEAR_2007
	m_aNotice[i] = EVENT_RICHYEAR_2007;
	i++;
#endif // EVENT_RICHYEAR_2007


#ifdef EVENT_HALLOWEEN_2007
	m_aNotice[i] = EVENT_HALLOWEEN_2007;
	i++;
#endif // EVENT_HALLOWEEN_2007

#ifdef EVENT_XMAS_2007
	m_aNotice[i] = EVENT_XMAS_2007;
	i++;
#endif //EVENT_XMAS_2007
	
#ifdef NEWYEAR_EVENT_2008
	m_aNotice[i] = NEWYEAR_EVENT_2008;
	i++;
#endif // NEWYEAR_EVENT_2008

#ifdef SAKURA_EVENT_2008
	m_aNotice[i] = SAKURA_EVENT_2008;
	i++;
#endif // SAKURA_EVENT_2008
	
#ifdef CHAOSBALL

#ifdef LC_BRZ
#else
	m_aNotice[i] = CHAOSBALL;
	i++;
#endif // LC_BRZ

#endif

#ifdef EVENT_SUMMER_2008
	m_aNotice[i] = EVENT_SUMMER_2008;
	i++;
#endif //EVENT_SUMMER_2008
/*
#ifdef EVENT_PHOENIX
	m_aNotice[i] = EVENT_PHOENIX;
	i++;
#endif // EVENT_PHOENIX
*/
#endif // #ifdef NOTICE_EVENT

	if (i > MAX_NOTICE)
	{
		GAMELOG << init("SYSTEM ERROR: OVERFLOW NOTICE") << end;
		return false;
	}

#endif // EVENTSETTING

#ifdef MONSTER_COMBO
	i = FindZone(ZONE_COMBO_DUNGEON);
	if(i == -1)
	{
		GAMELOG << init("SYSTEM ERROR: NOT FOUND COMBO DUNGEON") << end;
		return false;
	}
	m_comboZone = m_zones + i;
#endif // MONSTER_COMBO

	return true;
}

bool CServer::ConnectDB()
{
	if (!mysql_real_connect (
		&m_dbchar,
		m_config.Find("Char DB", "IP"),
		m_config.Find("Char DB", "User"),
		m_config.Find("Char DB", "Password"),
		m_config.Find("Char DB", "DBName"),
		0, NULL, 0))
		return false;
	
	if (!mysql_real_connect (
		&m_dbdata,
		m_config.Find("Data DB", "IP"),
		m_config.Find("Data DB", "User"),
		m_config.Find("Data DB", "Password"),
		m_config.Find("Data DB", "DBName"),
		0, NULL, 0))
		return false;

#ifdef ENABLE_WAR_CASTLE
	if (!mysql_real_connect (
		&m_dbcastle,
		m_config.Find("Char DB", "IP"),
		m_config.Find("Char DB", "User"),
		m_config.Find("Char DB", "Password"),
		m_config.Find("Char DB", "DBName"),
		0, NULL, 0))
		return false;
#endif
	
	// 태국 상품 테이블을 마스터 디비로 놓을거임
#ifdef USE_TENTER_BILLING
	if (!mysql_real_connect (
		&m_dbcatal,
		m_config.Find("Catalog DB", "IP"),
		m_config.Find("Catalog DB", "User"),
		m_config.Find("Catalog DB", "Password"),
		m_config.Find("Catalog DB", "DBName"),
		0, NULL, 0))
		return false;
#endif

	CDBCmd cmdCheckCharDB;
	cmdCheckCharDB.Init(&m_dbchar);

#ifdef ETC_EVENT
	cmdCheckCharDB.SetQuery("SELECT a_etc_event FROM t_characters LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_characters.a_etc_event!!") << end;
		return false;
	}
#endif

#ifdef EVENT_NEWYEAR_2006
	cmdCheckCharDB.SetQuery("SELECT a_char_index, a_total_time FROM t_event_newyear2006 LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_event_newyear2006!!") << end;
		return false;
	}
#endif // #ifdef EVENT_NEWYEAR_2006

#ifdef EVENT_SEARCHFRIEND
	cmdCheckCharDB.SetQuery("SELECT a_char_index, a_dormant_index, a_dormant_name,"
		"a_dormant_nick, a_dormant_total_time FROM t_event_searchfriend LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_event_searchfriend!!") << end;
		return false;
	}
#endif // #ifdef EVENT_SEARCHFRIEND

// 060221 : bs : 펫 사망시간 필드 검사
#ifdef ENABLE_PET
	cmdCheckCharDB.SetQuery("SELECT a_time_rebirth FROM t_pet LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_pet.a_time_rebirth!!") << end;
		return false;
	}
#endif // #ifdef ENABLE_PET

	// 060227 : bs : 유료화 아이템 2차
	cmdCheckCharDB.SetQuery("SELECT a_index, a_char_index, a_item_index, a_end_time, a_skill_index, a_skill_level, a_hit0, a_hit1, a_hit2 FROM t_assist_abstime LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_assist_abstime!") << end;
		return false;
	}
	// 060227 : bs : 유료화 아이템 2차

	// 자동 지급 테이블

	cmdCheckCharDB.SetQuery("SELECT a_index, a_char_index, a_item_index, a_item_plus, a_item_flag, a_item_used, a_item_count, a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4 FROM t_auto_give LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_auto_give!") << end;
		return false;
	}


#ifdef ENABLE_CHARACTER_DELETE_DELAY
	// 캐릭터 삭제 딜레이 필드
	cmdCheckCharDB.SetQuery("SELECT a_deletedelay FROM t_characters LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_characters.a_deletedelay!") << end;
		return false;
	}
#endif // ENABLE_CHARACTER_DELETE_DELAY

	cmdCheckCharDB.SetQuery("SELECT "CASH_ITEM_DATE_FIELD_MEMPOS", "CASH_ITEM_DATE_FIELD_CHAR_SLOT0", "CASH_ITEM_DATE_FIELD_CHAR_SLOT1", "CASH_ITEM_DATE_FIELD_STASHEXT" FROM t_cashItemdate LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_cashItemdate.a_xxx_new!") << end;
		return false;
	}

#ifdef QUEST_DATA_EXTEND
	int nQuestTable;
	for (nQuestTable = 0; nQuestTable < 10; nQuestTable++)
	{
		sprintf(g_buf, "SELECT a_char_index,a_quest_index,a_state,a_value0,a_value1,a_value2 FROM t_questdata%02d LIMIT 1", nQuestTable);
		cmdCheckCharDB.SetQuery(g_buf);
		if (!cmdCheckCharDB.Open())
		{
			GAMELOG << init("Error: Character DB Check: Not Found t_questdata") << nQuestTable << end;
			return false;
		}
	}
#endif // QUEST_DATA_EXTEND

#ifdef RESTRICT_PK
	cmdCheckCharDB.SetQuery("SELECT a_lastpktime FROM t_characters LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_characters.a_lastpktime") << end;
		return false;
	}
#endif // RESTRICT_PK

#ifdef COMPOSITE_TIME
	for (int nInvenTable = 0; nInvenTable < 10; nInvenTable++)
	{
		sprintf(g_buf, "SELECT a_used0_2, a_used1_2, a_used2_2, a_used3_2, a_used4_2 FROM t_inven%02d LIMIT 1", nInvenTable);
		cmdCheckCharDB.SetQuery(g_buf);
		if (!cmdCheckCharDB.Open())
		{
			GAMELOG << init("Error: Character DB Check: Not Found t_inven") << nInvenTable << ".a_used_2" << end;
			return false;
		}
	}

	for (int nStashTable = 0; nStashTable < 10; nStashTable++)
	{
		sprintf(g_buf, "SELECT a_used_2 FROM t_stash%02d LIMIT 1", nStashTable);
		cmdCheckCharDB.SetQuery(g_buf);
		if (!cmdCheckCharDB.Open())
		{
			GAMELOG << init("Error: Character DB Check: Not Found t_stash") << nStashTable << ".a_used_2" << end;
			return false;
		}
	}

	cmdCheckCharDB.SetQuery("SELECT a_item_used_2 FROM t_auto_give LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_auto_give.a_item_used_2") << end;
		return false;
	}
#endif	// COMPOSITE_TIME

#ifdef PRIMIUM_MEMORYBOOK
	cmdCheckCharDB.SetQuery("SELECT * FROM t_mempos_plus LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_mempos_plus") << end;
		return false;
	}	
#endif	// PRIMIUM_MEMORYBOOK


#ifdef JP_OTAKU_SYSTEM
	CDBCmd cmdCheckOTAKU;
	cmdCheckOTAKU.Init(&m_dbdata);
	cmdCheckOTAKU.SetQuery("SELECT a_index, a_string_jpn FROM t_string WHERE a_index IN (3221, 3222) ORDER BY a_index");
	if (cmdCheckOTAKU.Open())
	{
		if (cmdCheckOTAKU.MoveNext())
		{
			cmdCheckOTAKU.GetRec("a_string_jpn", m_strGMCommandOTAKUSpeedUp);
		}
		if (cmdCheckOTAKU.MoveNext())
		{
			cmdCheckOTAKU.GetRec("a_string_jpn", m_strGMCommandOTAKUImmortal);
		}
	}
#endif // JP_OTAKU_SYSTEM

#ifdef EVENT_TEACH_2007
	CDBCmd cmdCheckEventTeach2007;
	cmdCheckEventTeach2007.Init(&m_dbchar);
	cmdCheckEventTeach2007.SetQuery("SELECT a_index, a_addflower FROM t_event_teach2007 LIMIT 1");
	if (cmdCheckEventTeach2007.Open() == false)
	{
		GAMELOG << init("Error: Char DB Check : Not Found t_event_teach2007") << end;
		return false;
	}
#endif // EVENT_TEACH_2007

#ifdef EVENT_GOMDORI_2007
	CDBCmd cmdCheckEventGomdori2007;
	cmdCheckEventGomdori2007.Init(&m_dbchar);
	cmdCheckEventGomdori2007.SetQuery("SELECT a_char_index, a_first_lose, a_first_win, a_total FROM t_event_gomdori_2007 LIMIT 1");
	if (!cmdCheckEventGomdori2007.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_event_gomdori_2007") << end;
		return false;
	}
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_PARENTSDAY_2007
	CDBCmd cmdCheckEventParentsday2007;
	cmdCheckEventParentsday2007.Init(&m_dbchar);
	cmdCheckEventParentsday2007.SetQuery("SELECT * FROM t_event_2007_parentsday LIMIT 1");
	if (!cmdCheckEventParentsday2007.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_event_2007_parentsday") << end;
		return false;
	}

	CDBCmd cmdCheckEventMay2007;
	cmdCheckEventMay2007.Init(&m_dbchar);
	cmdCheckEventMay2007.SetQuery("SELECT * FROM t_event_may2007 LIMIT 1");
	if (!cmdCheckEventMay2007.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_event_may2007") << end;
		return false;
	}
#endif // EVENT_PARENTSDAY_2007

#ifdef PET_NAME_CHANGE
	CDBCmd cmdCheckPetName;
	cmdCheckPetName.Init( &m_dbchar );
	cmdCheckPetName.SetQuery( "SELECT * FROM t_pet_name LIMIT 1" );
	if( !cmdCheckPetName.Open() )
	{
		GAMELOG << init("Error: Char DB Check : Not Found t_pet_name") << end;
			return false;
	}
#endif // PET_NAME_CHANGE


#ifdef NEW_GUILD
	CDBCmd cmdNewGuild;
	cmdNewGuild.Init(&m_dbchar);
	cmdNewGuild.SetQuery( "SELECT * FROM t_characters_guildpoint LIMIT 0" );
	if( !cmdNewGuild.Open() )
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_characters_guildpoint") << end;
		return false;
	}

	cmdNewGuild.SetQuery("SELECT * FROM t_extend_guild LIMIT 0");
	if (!cmdNewGuild.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_extend_guild") << end;
		return false;
	}

	cmdNewGuild.SetQuery("SELECT * FROM t_extend_guildmember LIMIT 0");
	if (!cmdNewGuild.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_extend_guildmember") << end;
		return false;
	}

	cmdNewGuild.SetQuery("SELECT * FROM t_guild_notice LIMIT 0");
	if (!cmdNewGuild.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_guild_notice") << end;
		return false;
	}
#endif // NEW_GUILD
	
#ifdef CREATE_EVENT
	CDBCmd cmdCreateEvent;
	cmdCreateEvent.Init(&m_dbdata);
	cmdCreateEvent.SetQuery( "SELECT * FROM create_event LIMIT 1" );
	if( !cmdCreateEvent.Open() )
	{
		GAMELOG << init("Error: Char DB Check : Not Found create_event") << end;
			return false;
	}
#endif // CREATE_EVENT


#ifdef DRATAN_CASTLE
	CDBCmd cmdNewCastleWar;
	cmdNewCastleWar.Init(&m_dbchar);
	cmdNewCastleWar.SetQuery( "SELECT * FROM t_castle_guard LIMIT 0" );
	if( !cmdNewCastleWar.Open() )
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_castle_guard") << end;
		return false;
	}

	cmdNewCastleWar.SetQuery("SELECT * FROM t_castle_tower LIMIT 0");
	if (!cmdNewCastleWar.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_castle_tower") << end;
		return false;
	}

	cmdNewCastleWar.SetQuery("SELECT * FROM t_castle_reinforce LIMIT 0");
	if (!cmdNewCastleWar.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_castle_reinforce") << end;
		return false;
	}

	cmdNewCastleWar.SetQuery("SELECT * FROM t_castle_rebrith LIMIT 0");
	if (!cmdNewCastleWar.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_castle_rebrith") << end;
		return false;
	}
#endif // DRATAN_CASTLE

#ifdef PET_DIFFERENTIATION_ITEM
	CDBCmd cmdPetColor;
	cmdPetColor.Init( &m_dbchar );
	cmdPetColor.SetQuery( "SELECT a_color FROM t_pet LIMIT 1" );
	if( !cmdPetColor.Open() )
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_pet") <<  ".a_color" << end;
		return false;
	}
#endif //PET_DIFFERENTIATION_ITEM

#ifdef EVENT_OPEN_ADULT_SERVER
	CDBCmd cmdEventAdultServer;
	cmdEventAdultServer.Init(&m_dbchar);
	cmdEventAdultServer.SetQuery("SELECT * FROM t_event_adultserver");
	if( !cmdEventAdultServer.Open() )
	{
		GAMELOG << init("Error: Char DB Check : Not Found t_event_adultserver") << end;
		return false;
	}
#endif // EVENT_OPEN_ADULT_SERVER
	return true;
}


void CServer::DisconnectDB(bool bchar)
{
	if (bchar)
	{
		mysql_close (&m_dbchar);
#ifdef ENABLE_WAR_CASTLE
		mysql_close (&m_dbcastle);
#endif
	}
	else
	{
		mysql_close (&m_dbdata);
	}
}

bool CServer::CreateDBThread()
{
	GAMELOG	<< init("SYSTEM")
			<< "DB-Thread Initialized..."
			<< end;

	return m_dbthread.CreateThread();
}

bool CServer::InitGame()
{

	if (MSG_MSGR_END_MSG >= 255)
	{
		GAMELOG << init("ERROR MESSENGER MESSAGE IS FULL") << end;
		return false;
	}

	if (MSG_CONN_END_MSG >= 255)
	{
		GAMELOG << init("ERROR CONNECTOR MESSAGE IS FULL") << end;
		return false;
	}

	if (MSG_HELPER_END_MSG >= 255)
	{
		GAMELOG << init("ERROR HELPER REQ/REP MESSAGE IS FULL") << end;
		return false;
	}

	if (MSG_EVENT_END_MSG >= 255)
	{
		GAMELOG << init("ERROR EVENT MESSAGE IS FULL") << end;
		return false;
	}

	if (MSG_FAIL_END_MSG >= 255)
	{
		GAMELOG << init("ERROR FAIL MESSAGE IS FULL") << end;
		return false;
	}

	if (MSG_SYS_END_MSG >= 255)
	{
		GAMELOG << init("ERROR SYSTEM MESSAGE IS FULL") << end;
		return false;
	}

	if (MSG_ITEM_END_MSG >= 255)
	{
		GAMELOG << init("ERROR ITEM MESSAGE IS FULL") << end;
		return false;
	}

	ServerSrandom(time(0));
	MakeMath();
	
	InitExp();

	gcmdlist.AddMessage();
	gexcmdlist.AddExMessage();
	
	GAMELOG << init("SYSTEM")
		<< "Finding player limit."
		<< end;
	m_maxplayers = GetMaxPlayers();
	
	GAMELOG << init("SYSTEM")
		<< "Opening mother connection."
		<< end;
	m_ssock = InitSocket();
	
	if (!MakeMessengerClient())
	{
		if (m_messenger)
		{
			m_messenger->CloseSocket();
			delete m_messenger;
			m_messenger = NULL;
		}
	}
	
	
#ifdef PROC_BILLING
	if (!MakeBillingClient())
	{
		if (m_connector)
		{
			m_connector->CloseSocket();
			delete m_connector;
			m_connector = NULL;
		}
	}
#endif
	
	if (!MakeHelperClient())
	{
		if (m_helper)
		{
			m_helper->CloseSocket();
			delete m_helper;
			m_helper = NULL;
		}
	}

	SetHeaders();
	SendMessengerHeader();
	SendHelperHeader();
	SendBillingHeader();

#ifdef NEW_DOUBLE_GM_SAVEFILE
	FILE* fpDoubleEvent = fopen(NEW_DOUBLE_GM_SAVEFILE, "rt");
	if (fpDoubleEvent)
	{
		int 		nDoubleNasPercent;
		int 		nDoubleNasGetPercent;
		int 		nDoubleExpPercent;
		int 		nDoubleSpPercent;
		int 		nDoubleProducePercent;
		int 		nDoubleProduceNum;

		bool bFail = false;

		if (fgets(g_buf, 256, fpDoubleEvent) == NULL)		bFail = true;
		nDoubleNasPercent		= atoi(g_buf);

		if (fgets(g_buf, 256, fpDoubleEvent) == NULL)		bFail = true;
		nDoubleNasGetPercent	= atoi(g_buf);

		if (fgets(g_buf, 256, fpDoubleEvent) == NULL)		bFail = true;
		nDoubleExpPercent		= atoi(g_buf);

		if (fgets(g_buf, 256, fpDoubleEvent) == NULL)		bFail = true;
		nDoubleSpPercent		= atoi(g_buf);

		if (fgets(g_buf, 256, fpDoubleEvent) == NULL)		bFail = true;
		nDoubleProducePercent	= atoi(g_buf);

		if (fgets(g_buf, 256, fpDoubleEvent) == NULL)		bFail = true;
		nDoubleProduceNum		= atoi(g_buf);

		if (!bFail)
		{
			m_bDoubleEvent = true;

			m_bDoubleNasPercent			= nDoubleNasPercent;
			m_bDoubleNasGetPercent		= nDoubleNasGetPercent;
			m_bDoubleExpPercent			= nDoubleExpPercent;
			m_bDoubleSpPercent			= nDoubleSpPercent;
			m_bDoubleProducePercent		= nDoubleProducePercent;
			m_bDoubleProduceNum			= nDoubleProduceNum;

			if ( m_bDoubleNasPercent < 0
				|| m_bDoubleNasGetPercent  < 0
				|| m_bDoubleExpPercent  < 0
				|| m_bDoubleSpPercent  < 0
				|| m_bDoubleProducePercent  < 0
				|| m_bDoubleProduceNum  < 0 
				|| m_bDoubleNasPercent > 200
				|| m_bDoubleNasGetPercent > 200 
				|| m_bDoubleExpPercent  > 200
				|| m_bDoubleSpPercent  > 200
				|| m_bDoubleProducePercent  > 200
				|| m_bDoubleProduceNum  > 10 )
			{
				m_bDoubleNasPercent = DEFAULT_NAS_PERCENT;
				m_bDoubleNasGetPercent = DEFAULT_NAS_GET_PERCENT;
				m_bDoubleExpPercent = DEFAULT_EXP_PERCENT;
				m_bDoubleSpPercent = DEFAULT_SP_PERCENT;
				m_bDoubleProducePercent = DEFAULT_PRO_PERCENT;
				m_bDoubleProduceNum = DEFAULT_PRO_GET_NUMBER;

				GAMELOG << init("WARNING") << "USING DEFAULT DOUBLE EVENT VALUE" << end;
			}
		}

		fclose(fpDoubleEvent);
		fpDoubleEvent = NULL;
	}
#endif // NEW_DOUBLE_GM_SAVEFILE

#ifdef CHANCE_EVENT
	FILE * fpChanceEvent = fopen(".chanceevent", "rt");
	if (fpChanceEvent)
	{
		int nChanceSlevel;
		int nChanceElevel;
		int nChanceNasPercent;
		int nChanceNasGetPercent;
		int nChanceExpPercent;
		int nChanceSpPercent;
		int nChanceProducePercent;
		int nChanceProduceNum;

		bool bFail = false;

		if (fgets(g_buf, 256, fpChanceEvent) == NULL)
		{
			bFail = true;
		}
		nChanceSlevel = atoi(g_buf);

		if (fgets(g_buf, 256, fpChanceEvent) == NULL)
		{
			bFail = true;
		}
		nChanceElevel = atoi(g_buf);

		if (fgets(g_buf, 256, fpChanceEvent) == NULL)
		{
			bFail = true;
		}
		nChanceNasPercent = atoi(g_buf);

		if (fgets(g_buf, 256, fpChanceEvent) == NULL)
		{
			bFail = true;
		}
		nChanceNasGetPercent = atoi(g_buf);

		if (fgets(g_buf, 256, fpChanceEvent) == NULL)		
		{
			bFail = true;
		}
		nChanceExpPercent = atoi(g_buf);

		if (fgets(g_buf, 256, fpChanceEvent) == NULL)
		{
			bFail = true;
		}
		nChanceSpPercent = atoi(g_buf);

		if (fgets(g_buf, 256, fpChanceEvent) == NULL)		
		{
			bFail = true;
		}
		nChanceProducePercent = atoi(g_buf);

		if (fgets(g_buf, 256, fpChanceEvent) == NULL)
		{
			bFail = true;
		}
		nChanceProduceNum = atoi(g_buf);

		if (!bFail)
		{
			m_bChanceEvent = true;

			m_bChanceSlevel				= nChanceSlevel;
			m_bChanceElevel				= nChanceElevel;
			m_bChanceNasPercent			= nChanceNasPercent;
			m_bChanceNasGetPercent		= nChanceNasGetPercent;
			m_bChanceExpPercent			= nChanceExpPercent;
			m_bChanceSpPercent			= nChanceSpPercent;
			m_bChanceProducePercent		= nChanceProducePercent;
			m_bChanceProduceNum			= nChanceProduceNum;

			if (gserver.m_bChanceSlevel < 1
				|| gserver.m_bChanceSlevel > MAX_LEVEL
				|| gserver.m_bChanceElevel < 1
				|| gserver.m_bChanceElevel > MAX_LEVEL
				|| (gserver.m_bChanceSlevel > gserver.m_bChanceElevel))
			{
				gserver.m_bChanceSlevel = 1;
				gserver.m_bChanceElevel = MAX_LEVEL;
			}

			if (gserver.m_bChanceNasPercent < 0
				|| gserver.m_bChanceNasGetPercent  < 0
				|| gserver.m_bChanceExpPercent  < 0
				|| gserver.m_bChanceSpPercent  < 0
				|| gserver.m_bChanceProducePercent  < 0
				|| gserver.m_bChanceProduceNum  < 0 
				|| gserver.m_bChanceNasPercent > 200
				|| gserver.m_bChanceNasGetPercent > 200 
				|| gserver.m_bChanceExpPercent  > 200
				|| gserver.m_bChanceSpPercent  > 200
				|| gserver.m_bChanceProducePercent  > 200
				|| gserver.m_bChanceProduceNum  > 10)
			{
				gserver.m_bChanceNasPercent = DEFAULT_NAS_PERCENT;
				gserver.m_bChanceNasGetPercent = DEFAULT_NAS_GET_PERCENT;
				gserver.m_bChanceExpPercent = DEFAULT_EXP_PERCENT;
				gserver.m_bChanceSpPercent = DEFAULT_SP_PERCENT;
				gserver.m_bChanceProducePercent = DEFAULT_PRO_PERCENT;
				gserver.m_bChanceProduceNum = DEFAULT_PRO_GET_NUMBER;
			
				GAMELOG << init("WARNING") << "USING DEFAULT DOUBLE EVENT VALUE" << end;
			}
		}

		fclose(fpChanceEvent);
		fpChanceEvent = NULL;
	}
#endif // CHANCE_EVENT

#ifdef CREATE_EVENT
	LoadCreateEvent();
#endif // CREATE_EVENT

#ifdef USING_NPROTECT
	if (!InitNProtectGameGuard())
		return false;
#endif // USING_NPROTECT
	
#ifdef EVENT_XMAS_2007
	if( IS_RUNNING_CONN )
	{
		CNetMsg rmsg;
		ConnEventXmas2007Msg( rmsg, MSG_CONN_EVENT_XMASTREE_POINT );
		SEND_Q( rmsg, gserver.m_connector );
	}
#endif //EVENT_XMAS_2007

	GAMELOG << init("SYSTEM")
		<< "Entering game loop."
		<< end;
	
	return true;
}

int CServer::GetMaxPlayers()
{
#ifndef CIRCLE_UNIX
	return MAX_PLAYING;
#else
	int max_descs = 0;
	const char *method;
	/*
	* First, we'll try using getrlimit/setrlimit.  This will probably work
	* on most systems.  HAS_RLIMIT is defined in sysdep.h.
	*/
#ifdef HAS_RLIMIT
	{
		struct rlimit limit;
		
		/* find the limit of file descs */
		method = "rlimit";
		if (getrlimit(RLIMIT_NOFILE, &limit) < 0)
		{
			GAMELOG << init("SYS_ERR") << "calling getrlimit" << end;
			exit(1);
		}
		
		/* set the current to the maximum */
		limit.rlim_cur = limit.rlim_max;
		if (setrlimit(RLIMIT_NOFILE, &limit) < 0)
		{
			GAMELOG << init("SYS_ERR") << "calling setrlimit" << end;
			exit(1);
		}
#ifdef RLIM_INFINITY
		if (limit.rlim_max == RLIM_INFINITY)
			max_descs = MAX_PLAYING + NUM_RESERVED_DESCS;
		else
			max_descs = MIN(MAX_PLAYING + NUM_RESERVED_DESCS, limit.rlim_max);
#else
		max_descs = MIN(MAX_PLAYING + NUM_RESERVED_DESCS, limit.rlim_max);
#endif
	}
#elif defined (OPEN_MAX) || defined(FOPEN_MAX)
#if !defined(OPEN_MAX)
#define OPEN_MAX FOPEN_MAX
#endif
	method = "OPEN_MAX";
	max_descs = OPEN_MAX;		/* Uh oh.. rlimit didn't work, but we have
	* OPEN_MAX */
#elif defined (_SC_OPEN_MAX)
	/*
	* Okay, you don't have getrlimit() and you don't have OPEN_MAX.  Time to
	* try the POSIX sysconf() function.  (See Stevens' _Advanced Programming
	* in the UNIX Environment_).
	*/
	method = "POSIX sysconf";
	errno = 0;
	if ((max_descs = sysconf(_SC_OPEN_MAX)) < 0) {
		if (errno == 0)
			max_descs = m_MaxPlaying + NUM_RESERVED_DESCS;
		else
		{
			GAMELOG << init("SYS_ERR") << "Error calling sysconf" << end;
			exit(1);
		}
	}
#else
	/* if everything has failed, we'll just take a guess */
	method = "random guess";
	max_descs = m_MaxPlaying + NUM_RESERVED_DESCS;
#endif
	
	/* now calculate max _players_ based on max descs */
	max_descs = MIN(MAX_PLAYING, max_descs - NUM_RESERVED_DESCS);
	
	if (max_descs <= 0)
	{
		GAMELOG << init("SYS_ERR") << "Non-positive max player limit!  (Set at" << max_descs << "using" << method << ")." << end;
		exit(1);
	}
	
	GAMELOG << init("SYSTEM") << "Setting player limit to" << max_descs << "using" << method << "." << end;
	
	return (max_descs);
#endif /* CIRCLE_UNIX */
}

socket_t CServer::InitSocket()
{
	socket_t s;
	struct sockaddr_in sa;
	int opt;
	
#ifdef CIRCLE_WINDOWS
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		
		wVersionRequested = MAKEWORD(1, 1);
		
		if (WSAStartup(wVersionRequested, &wsaData) != 0)
		{
			puts("SYSERR: WinSock not available!");
			exit(1);
		}
		if ((wsaData.iMaxSockets - 4) < m_maxplayers)
		{
			m_maxplayers = wsaData.iMaxSockets - 4;
		}
		GAMELOG << init("SYSTEM")
			<< "Max players set to "
			<< m_maxplayers
			<< end;
		
		if ((s = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		{
			puts("SYSERR: Error opening network connection : Winsock error");
			exit(1);
		}
	}
#else
	/*
	* Should the first argument to socket() be AF_INET or PF_INET?  I don't
	* know, take your pick.  PF_INET seems to be more widely adopted, and
	* Comer (_Internetworking with TCP/IP_) even makes a point to say that
	* people erroneously use AF_INET with socket() when they should be using
	* PF_INET.  However, the man pages of some systems indicate that AF_INET
	* is correct; some such as ConvexOS even say that you can use either one.
	* All implementations I've seen define AF_INET and PF_INET to be the same
	* number anyway, so the point is (hopefully) moot.
	*/
	
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		GAMELOG << init("SYS_ERR") << "Error creating socket" << end;
		exit(1);
	}
#endif				/* CIRCLE_WINDOWS */
	
#if defined(SO_REUSEADDR) && !defined(CIRCLE_MACINTOSH)
	opt = 1;
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) < 0)
	{
		puts("SYSERR: setsockopt REUSEADDR");
		exit(1);
	}
#endif
	
	SetSendbuf(s);
	
	/*
	* The GUSI sockets library is derived from BSD, so it defines
	* SO_LINGER, even though setsockopt() is unimplimented.
	*	(from Dean Takemori <dean@UHHEPH.PHYS.HAWAII.EDU>)
	*/
#if defined(SO_LINGER) && !defined(CIRCLE_MACINTOSH)
	{
		struct linger ld;
		
		ld.l_onoff = 0;
		ld.l_linger = 0;
		if (setsockopt(s, SOL_SOCKET, SO_LINGER, (char *) &ld, sizeof(ld)) < 0)
			GAMELOG << init("SYS_ERR")
			<< "setsockopt SO_LINGER"
			<< end;	/* Not fatal I suppose. */
	}
#endif
	
	/* Clear the structure */
	memset((char *)&sa, 0, sizeof(sa));
	
	sa.sin_family = AF_INET;
	sa.sin_port = htons((unsigned short)atoi(gserver.m_config.Find("Server", "Port")));
	sa.sin_addr = *(GetBindAddr());
	
	if (bind(s, (struct sockaddr *) &sa, sizeof(sa)) < 0)
	{
		puts("SYSERR: bind");
		CLOSE_SOCKET(s);
		FILE *fp = fopen (".shutdown", "w");
		fclose (fp);
		exit(1);
	}
	Nonblock(s);
	listen(s, 57);
	
	m_serverAddr = m_config.Find("Server", "IP");
	m_serverPort = atoi(m_config.Find("Server", "Port"));
	
	return (s);
}

int CServer::SetSendbuf(socket_t s)
{
#if defined(SO_SNDBUF) && !defined(CIRCLE_MACINTOSH)
	int opt = MAX_SOCK_BUF;
	
	if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *) &opt, sizeof(opt)) < 0)
	{
		GAMELOG << init("SYS_ERR")
			<< "setsockopt SNDBUF"
			<< end;
		return (-1);
	}
	
#if 0
	if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *) &opt, sizeof(opt)) < 0)
	{
		GAMELOG << init("SYS_ERR")
			<< "setsockopt RCVBUF"
			<< end;
		return (-1);
	}
# endif
	
#endif
	
	return (0);
}

struct in_addr* CServer::GetBindAddr()
{
	static struct in_addr bind_addr;
	
	memset(&bind_addr, 0, sizeof(bind_addr));
	
	unsigned long addr = inet_addr(m_config.Find("Server", "IP"));
	if (addr < 0)
	{
		bind_addr.s_addr = htonl(INADDR_ANY);
		GAMELOG << init("SYS_ERR")
			<< "Invalid IP address"
			<< end;
	}
	else
		bind_addr.s_addr = addr;
	
	/* Put the address that we've finally decided on into the logs */
	if (bind_addr.s_addr == htonl(INADDR_ANY))
		GAMELOG << init("SYSTEM")
		<< "Binding to all IP interfaces on this m_host."
		<< end;
	else
	{
		GAMELOG << init("SYSTEM")
			<< "Binding only to IP address "
			<< inet_ntoa(bind_addr)
			<< end;
	}
	
	return (&bind_addr);
}

#if defined(CIRCLE_WINDOWS)
void CServer::Nonblock(socket_t s)
{
	unsigned long val = 1;
	ioctlsocket(s, FIONBIO, &val);
}
#else
#  ifndef O_NONBLOCK
#    define O_NONBLOCK O_NDELAY
#  endif
void CServer::Nonblock(socket_t s)
{
	int flags;
	flags = fcntl(s, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if (fcntl(s, F_SETFL, flags) < 0)
	{
		GAMELOG << init("SYS_ERR")
			<< "Fatal error executing nonblock (CServer.cpp)"
			<< end;
		exit(1);
	}
}
#endif

bool CServer::MakeBillingClient()
{
	struct sockaddr_in msg_socket;
	if (m_connector)
	{
		if(m_connector->m_bclosed)
		{ 
			m_connector->CloseSocket();
			delete m_connector;
			m_connector = NULL;
		}
	}
	
	m_connector = new CDescriptor;
#ifdef CRYPT_NET_MSG
	m_connector->m_bCryptNetMsg = false;
#endif // #ifdef CRYPT_NET_MSG
	
	if ((m_connector->m_desc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		puts ("SYSERR: Error opening network connection : Winsock error");
		return false;
	}
	
	if (m_connector->m_desc <= 0) return false;
	memset(&msg_socket, 0, sizeof(msg_socket));
	msg_socket.sin_family = AF_INET;
	msg_socket.sin_addr.s_addr = inet_addr (m_config.Find("Connector Server", "IP"));
	msg_socket.sin_port = htons((unsigned short)atoi(m_config.Find("Connector Server", "Port")));
	
	if (connect(m_connector->m_desc, (struct sockaddr *)&msg_socket, sizeof(msg_socket)) < 0)
	{
		puts ("SYSERR: Cannot connect Connector Server!!!");
		return false;
	}
	
	m_connector->m_idletics = 0;
	Nonblock (m_connector->m_desc);
	SetSendbuf(m_connector->m_desc);
	
	return true;
}

void CServer::SetHeaders()
{
	int* zones = new int[m_numZone];
	int count = 0;
	int i;
	for (i = 0; i < m_numZone; i++)
	{
		if (m_zones[i].m_bRemote)
			continue ;
		zones[count++] = m_zones[i].m_index;
	}
	
	MsgrConnectMsg(m_messengerConnMsg, SERVER_VERSION, m_serverno, m_subno, count, zones);
	ConnConnectMsg(m_connectorConnMsg, SERVER_VERSION, m_serverno, m_subno, count, zones);
	HelperConnectMsg(m_helperConnMsg, SERVER_VERSION, m_serverno, m_subno, count, zones);
	delete [] zones;
	zones = NULL;
}

void CServer::SendBillingHeader()
{
	if (m_connector && !m_connector->m_bclosed)
	{
		SEND_Q(m_connectorConnMsg, m_connector);
		
		if (m_connector->m_desc < 0) return ;
		if (m_connector->m_outBuf.GetNextPoint())
		{
			if (m_connector->ProcessOutput() < 0)
			{
				GAMELOG << init("SYS_ERR") << "Connector Disconnected..." << end;
				m_connector->m_bclosed = true;
			}
		}
	}
}

#ifdef CIRCLE_WINDOWS
void CServer::ServerSleep(struct timeval* timeout)
{
	Sleep(timeout->tv_sec * 1000 + timeout->tv_usec / 1000);
}
#else
void CServer::ServerSleep(struct timeval* timeout)
{
	if (select(0, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, timeout) < 0)
	{
		if (errno != EINTR)
		{
			GAMELOG << init("SYS_ERR") << "Select sleep!!" << end;
			exit(1);
		}
	}
}
#endif /* end of CIRCLE_WINDOWS */

int CServer::NewDescriptor(int s)
{
	socket_t m_desc;
	socklen_t i;
	CDescriptor* newd;
	struct sockaddr_in peer;
	struct hostent *from;
	
	i = sizeof(peer);
	if ((m_desc = accept(s, (struct sockaddr *)&peer, &i)) == INVALID_SOCKET)
		return (-1);
	
	/* keep it from blocking */
	Nonblock(m_desc);
	
	/* set the send buffer size */
	if (SetSendbuf(m_desc) < 0)
	{
		CLOSE_SOCKET(m_desc);
		return (0);
	}
	
	/* create a new descriptor */
	newd = new CDescriptor;
	
	/* find the sitename */
	if (m_nameserverisslow
		|| !(from = gethostbyaddr((char*)&peer.sin_addr, sizeof(peer.sin_addr), AF_INET)))
	{
		/* resolution failed */
		if (!m_nameserverisslow)
			GAMELOG << init("SYS_ERR")
			<< "gethostbyaddr"
			<< end;
		
		/* find the numeric site address */
		newd->m_host.CopyFrom((char*)inet_ntoa(peer.sin_addr), HOST_LENGTH);
	}
	else
	{
		newd->m_host.CopyFrom(from->h_name, HOST_LENGTH);
	}
	
	/* initialize descriptor data */
	newd->m_desc = m_desc;
	newd->m_idletics = 0;
	newd->m_checktics = m_pulse;
	STATE(newd) = CON_GET_LOGIN;
	
	/* prepend to list */
	ADD_TO_BILIST(newd, m_desclist, m_pPrev, m_pNext);
	
	return (0);
}

void CServer::CloseSocket(CDescriptor* d, bool bForce)
{
#ifdef ENABLE_LOGOUT_DELAY
	if (!bForce)
	{
		if (d->m_pChar && d->m_pChar->m_bLoadChar && d->m_pChar->m_admin < 2)
		{
			int curTime;
			time((time_t*)&curTime);

			if (d->m_logoutDelay == -1)
			{
				// 현재 로그아웃 대기 시간이 -1이면 새로 설정 후 리턴
				d->m_logoutDelay = curTime + ENABLE_LOGOUT_DELAY;
				return ;
			}
			else if (d->m_logoutDelay > curTime)
			{
				// 로그아웃 대기 시간 중에는 무시
				return ;
			}
			else
			{
				d->m_logoutDelay = 0;
			}
		}
		else
		{
			d->m_logoutDelay = 0;
		}
	}
	else
	{
		d->m_logoutDelay = 0;
	}
#endif // ENABLE_LOGOUT_DELAY

	if (d->m_tryBug)
	{
		GAMELOG << init("TRY DISCONNECT BUG", d->m_idname)
				<< "IP" << delim
				<< d->m_host
				<< end;
		d->m_tryBug = false;
	}

	if (m_descLoop == d)
	{
		m_descLoop = NULL;
		m_bLoop = 0;
	}

	if (m_observer)
		m_observer = NULL;

	int i;
	for (i = 0; i < 20; i++)
	{
		if (m_chatmonitor[i] == d)
		{
			m_chatmonitor[i] = NULL;
		}
	}
#ifdef GMTOOL
	for(i = 0; i < gserver.m_nGMToolCount; ++i)
	{
		if(gserver.m_gmtool[i] == d)
		{
			gserver.m_gmtool[i] = NULL;
		}
	}
#endif // GMTOOL

	if (d->m_pChar)
	{
		d->m_pChar->ProcDisconnect(true, false);
		d->m_pChar->OutputDBItemLog();
	}
	
	if (STATE(d) == CON_PLAYING && d->m_pChar)
	{
		if (m_playerList.Playing (d->m_pChar))
		{
			CNetMsg msg;

			d->m_pChar->m_bPlaying = false;
			// remove form player list..
			m_playerList.Remove(d->m_pChar);

			STATE(d) = CON_DISCONNECT;

			// 돈 가진거 로그 남기기
#ifdef LOG_INDEX
			GAMELOG << init("HAVE_MONEY", d->m_pChar)
#else			
			GAMELOG << init("HAVE_MONEY", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
#endif // LOG_INDEX
					<< ((d->m_pChar->m_moneyItem) ? d->m_pChar->m_moneyItem->Count() : 0)
					<< end;

			// 착용 아이템 로그 남기기
			for (i = 0; i < MAX_WEARING; i++)
			{
				if (d->m_pChar->m_wearing[i])
				{
					CItem* p = d->m_pChar->m_wearing[i];
#ifdef LOG_INDEX
					GAMELOG << init("WEAR ITEM", d->m_pChar)
#else
					GAMELOG << init("WEAR ITEM", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
#endif // LOG_INDEX
							<< itemlog(p)
							<< end;
				}
			}


#ifdef ENABLE_PET
			// 소유 펫 로그 남기기
			CPet* petNext = d->m_pChar->m_petList;
			CPet* pet;
			while ((pet = petNext))
			{
				petNext = pet->m_nextPet;
				// TODO : petlog
				GAMELOG << init("PET INFO", d->m_pChar)
						<< "PET" << delim
						<< pet->m_index << delim
						<< pet->GetPetType() << delim
						<< pet->GetPetGrade() << delim
						<< "LEVEL" << delim
						<< pet->m_level << delim
						<< "AP" << delim
						<< pet->GetAbilityPoint()
						<< end;
			}
#endif
#ifdef ATTACK_PET
			CAPet *apet_next = d->m_pChar->m_pApetlist;
			CAPet *apet;
			while ((apet = apet_next))
			{
				apet_next = apet->m_pNextPet;
				GAMELOG << init("APET_INPO", d->m_pChar )
					<< "APET" << apet->m_index << delim << apet->m_name << delim << apet->m_pProto->Index() << delim
					<< "LEVEL" << delim << apet->m_level << delim << "EXP" << delim << apet->m_exp << end ;
			}
#endif // ATTACK_PET
#ifdef FEEITEM
			// 캐쉬 아이템 버프 로그 남기기
			int nAssistABSTypeCount = d->m_pChar->m_assist.GetABSTimeTypeList(NULL);
			if (nAssistABSTypeCount > 0)
			{
				int* nAssistABSItemIndex = new int[nAssistABSTypeCount];
				int* nAssistABSSkillIndex = new int[nAssistABSTypeCount];
				int* nAssistABSSkillLevel = new int[nAssistABSTypeCount];
				char* nAssistABSHit0 = new char[nAssistABSTypeCount];
				char* nAssistABSHit1 = new char[nAssistABSTypeCount];
				char* nAssistABSHit2 = new char[nAssistABSTypeCount];
				int* nAssistABSEndTime = new int[nAssistABSTypeCount];

				d->m_pChar->m_assist.GetABSTimeTypeList(nAssistABSItemIndex, nAssistABSSkillIndex, nAssistABSSkillLevel, nAssistABSHit0, nAssistABSHit1, nAssistABSHit2, nAssistABSEndTime);
				for( i = 0; i < nAssistABSTypeCount; i++)
				{
					GAMELOG << init("CASH_ABS_ASSIST", d->m_pChar)
						   << nAssistABSItemIndex[i] << delim
						   << nAssistABSSkillIndex[i] << delim
						   << nAssistABSSkillLevel[i] << delim
						   << nAssistABSHit0[i] << delim
						   << nAssistABSHit1[i] << delim
						   << nAssistABSHit2[i] << delim
						   << nAssistABSEndTime[i] << end;
				}

			}
			// 메모리 스크롤 개인 창고 확장 카드 남은 시간 로그 남기기
			GAMELOG << init("CASH_MEMPOS_STASH_TIME", d->m_pChar)
					<< d->m_pChar->m_memposTime << delim
					<< d->m_pChar->m_stashextTime << end;
#endif // FEEITEM

#ifdef LOG_INDEX
			GAMELOG << init("DISCONNECT", d->m_pChar)
#else			
			GAMELOG << init("DISCONNECT", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
#endif // LOG_INDEX
				<< end;
			
			UPDATING_DB(d) = DB_UPDATE;
			WAITTING_DB(d) = true;
			d->m_quitsave = false;
			m_dbthread.m_queue.AddToQ(d);
			return;
		}
		else if (STATE(d) != CON_MOVESERVER_WAIT)
		{
			GAMELOG << init("SYS_ERR", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
				<< "Cannot save DATA : "
				<< STATE(d)
				<< delim
				<< UPDATING_DB(d)
				<< delim
				<< (int)WAITTING_DB(d)
				<< end;
		}
	}
	else if (d->m_logined != LOGIN_STATE_FAIL && d->m_logined != LOGIN_STATE_NOT && STATE(d) != CON_DISCONNECT && STATE(d) != CON_MOVESERVER_WAIT)
	{
		STATE(d) = CON_DISCONNECT;
		d->m_logined = LOGIN_STATE_NOPLAY;
		
		// Only change game flag
		// -- 무조건 디비 업데이트 큐니까 별로 상관 없을 듯...
		GAMELOG << init("DISCONNECT", "No Play", d->m_idname)
#ifdef LOG_INDEX
			<< d->m_index 
#endif // LOG_INDEX
			<< delim
			<< STATE(d)
			<< delim
			<< UPDATING_DB(d)
			<< delim
			<< (int)WAITTING_DB(d)
			<< end;
		
		UPDATING_DB(d) = DB_UPDATE;
		WAITTING_DB(d) = true;
		d->m_quitsave = false;
		m_dbthread.m_queue.AddToQ(d);
		return;
	}
	else
	{
		if (STATE(d) != CON_CLOSE && STATE(d) != CON_DISCONNECT)
		{
			GAMELOG << init("SYS_ERR", d->m_host, d->m_idname)
				<< STATE(d)
				<< delim
				<< UPDATING_DB(d)
				<< delim
				<< (int)WAITTING_DB(d)
				<< end;
		}
	}
	
	if (STATE(d) == CON_DISCONNECT && !d->m_quitsave)
	{
		if (WAITTING_DB(d) || (UPDATING_DB(d) > DB_NOP))
			return;
	}
	if (d->m_dbrunning) return;
	if (!d->m_quitsave) return;
	
	// 컨넥터가 살아 있으면
	if (IS_RUNNING_CONN)
	{
		// 플레이 도중 서버 이동이면
		if (d->m_pChar && d->m_pChar->m_pZone && d->m_pChar->m_pZone->m_bRemote)
		{
			if (STATE(d) != CON_MOVESERVER_WAIT)
			{
				// 컨넥터에 서버 이동 보내기
				CNetMsg cmsg;
				ConnPlayingMsg(cmsg, d, MSG_LOGIN_RE);
				SEND_Q(cmsg, m_connector);
				STATE(d) = CON_MOVESERVER_WAIT;
			}
			if (!d->m_bGoZoneSent)
				return ;
		}
		
		// 플레이 하지 않았거나, 서버 이동이 아니면
		// 아이디 받았고 로그인 실패 유저 아니면 로그아웃 보내기
		else if (d->m_idname[0] && d->m_logined != LOGIN_STATE_FAIL && d->m_logined != LOGIN_STATE_NOT)
		{
			// 로그아웃 보내기
			CNetMsg cmsg;
			ConnLogoutMsg(cmsg, d->m_idname);
			SEND_Q(cmsg, m_connector);
		}
	}
	
	REMOVE_FROM_BILIST(d, m_desclist, m_pPrev, m_pNext);
	d->CloseSocket();

	delete d;
}

void CServer::SendOutput(CDescriptor* d)
{
	if (d == NULL) return;
	if (d->m_bclosed) return;
	if (d->m_outBuf.GetNextPoint())
	{
		if (FD_ISSET(d->m_desc, &m_output_set))
		{
			if (d->ProcessOutput() < 0)
				d->m_bclosed = true;
		}
	}
}

void CServer::SendOutput(CCharacter* ch)
{
	if (IS_PC(ch))
		SendOutput(TO_PC(ch)->m_desc);
}

bool CServer::DisConnectLogedIn(CDescriptor* my_d, bool bForce)
{
	CDescriptor* d;
	CDescriptor* dNext = m_desclist;
	
	while ((d = dNext))
	{
		dNext = d->m_pNext;
		if (d != my_d && !strcmp2(d->m_idname, my_d->m_idname) && STATE(d) != CON_GET_LOGIN)
		{
			if (STATE(d) == CON_PLAYING && m_playerList.Playing(d->m_pChar))
			{
//				d->m_pChar->m_bPlaying = false;
//				m_playerList.Remove(d->m_pChar);
//				CNetMsg msg;
//				DisappearMsg(msg, d->m_pChar);
//				d->m_pChar->m_pArea->SendToCell(msg, d->m_pChar);
//				d->m_pChar->m_pArea->CharFromCell(d->m_pChar);
//				STATE(d) = CON_DISCONNECT;
//				
#ifdef LOG_INDEX
				GAMELOG << init("DISCONNECT", d->m_pChar)
#else
				GAMELOG << init("DISCONNECT", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
#endif // LOG_INDEX
					<< "Duplication"
					<< end;

				CloseSocket(d, bForce);
//				d->m_pChar->m_lastupdate = PULSE_SAVE_PC;
//				UPDATING_DB(d) = DB_UPDATE;
//				WAITTING_DB(d) = true;
//				m_dbthread.m_queue.AddToQ(d);
			}
			else if (STATE(d) != CON_DISCONNECT)
			{
				STATE(d) = CON_CLOSE;
			}
			return true;
		}
	}
	return false;
}

void CServer::CommandInterpreter(CDescriptor* d, CNetMsg& msg)
{
	CPC* ch = d->m_pChar;

#ifdef USING_NPROTECT
	if (d->m_pGGAuth == NULL)
	{
		CNetMsg authmsg;
		d->m_pGGAuth = new CNProtectAuth;
		if (!d->m_pGGAuth->MakeGameGuardAuthQuery(authmsg))
		{
			GAMELOG << init("NPROTECT AUTH FAIL 0", d->m_pChar) << end;
			gserver.CloseSocket(d, true);
			return ;
		}
#ifdef _DEBUG
		*g_buf2 = '\0';
		sprintf(g_buf2, "s->c: size: %d, data: ", authmsg.m_size);
		int ttt;
		for (ttt = 0; ttt < authmsg.m_size; ttt++)
		{
			sprintf(g_buf, "%02x ", authmsg.m_buf[ttt]);
			strcat(g_buf2, g_buf);
		}
		strcat(g_buf2, "\n");
		OutputDebugString(g_buf2);
#endif // _DEBUG

		SEND_Q(authmsg, d);
		d->m_pGGAuth->SetNextAuthQueryTime(NPROTECT_SECOND_QUERY_TIME);
	}
#endif // USING_NPROTECT
	
	if (!ch) return;
	int cmd_num = gcmdlist.Find(ch, msg.m_mtype);
	if (cmd_num < 0) // invalid command.. 
	{
		GAMELOG << init("SYS_WARN", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
			<< "Invalid command" << delim
			<< msg.m_mtype
			<< end;
		return;
	}

	if (m_pulse - d->m_checktics >= PULSE_REAL_SEC)
	{
		// 1초마다 공격 및 명령 카운터를 초기화 한다.
		// 즉, 1초에 몇개의 명령을 수행할 수 있는지 정하고
		// 이곳에서 초기화 되기 전에 명령의 개수를 넘기는 경우 핵으로 간주하면 된다
		if (d->m_commandcount > 10)
		{
			if (d->m_pChar)
				GAMELOG << init("HACK COMMAND", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname);
			else
				GAMELOG << init("HACK COMMAND", d->m_idname);
			GAMELOG << d->m_commandcount
					<< end;

			// TODO : 핵 카운트 증가
			// d->IncreaseHackCount();
		}
		d->m_checktics = m_pulse;
		d->m_commandcount = 0;
	}
	
	d->m_commandcount++;
	
	gcmdlist.Run(cmd_num, ch, msg);
}

CDescriptor* CServer::FindConnectIdName(const char* m_idname, CDescriptor* md)
{
	CDescriptor* d;
	CDescriptor* dNext;
	
	if (md == NULL)
	{
		dNext = m_desclist;
		while ((d = dNext))
		{
			dNext = d->m_pNext;
			if (d->m_bclosed) continue;
			if (!strcmp2(d->m_idname, m_idname))
				return d;
		}
	}
	else
	{
		dNext = m_desclist;
		while ((d = dNext))
		{
			dNext = d->m_pNext;
			if (d == md) continue;
			if (STATE(d) == CON_CLOSE || STATE(d) == CON_DISCONNECT) continue;
			if (d->m_bclosed) continue;
			if (!strcmp2(d->m_idname, m_idname))
				return d;
		}
	}
	
	return NULL;
}

CDescriptor* CServer::FindUser(int userindex)
{
	CDescriptor* d;
	for (d = m_desclist; d; d = d->m_pNext)
	{
		switch (STATE(d))
		{
		case CON_PLAYING:
		case CON_PREPARE_PLAY:
			if (d->m_index == userindex)
				return d;
			break;
		}
	}
	
	return NULL;
}

#ifdef CIRCLE_WINDOWS //빌링 
void CServer::BillingConnect(void* lParam)
{
	CServer* _this = (CServer*)lParam;
	_this->m_brunconnector = true;
	while (true)
	{
		if (_this->MakeBillingClient())
		{
			_this->SendBillingHeader();
			_this->m_brunconnector = false;
			_endthread();
			return ;
		}
		else
			if (_this->m_connector)
				_this->m_connector->m_bclosed = true;
			
			Sleep(2000);
	}
}
#else
void* CServer::BillingConnect(void* lParam)
{
	CServer* _this = (CServer*)lParam;
	_this->m_brunconnector = true;
	while (true)
	{
		if (_this->MakeBillingClient())
		{
			_this->SendBillingHeader();
			_this->m_brunconnector = false;
			pthread_exit(NULL);
			return NULL;
		}
		else
			if (_this->m_connector)
				_this->m_connector->m_bclosed = true;
			
			sleep(2);
	}
	return NULL;
}
#endif

void CServer::DecreaseTimeForPC(CPC* ch)
{
	if (ch->m_perSecondPulse > 0)
		ch->m_perSecondPulse--;

	/////////////////////////////////////////////
	// BANGWALL : 2005-06-27 오후 7:09:33
	// Comment : pd4 시간처리

	if(ch->m_pd4StartTime > 0)
	{
		ch->m_pd4StartTime--;
		if(ch->m_pd4StartTime <= 0 )
		{
			CNetMsg msg;
#ifdef QUEST_DATA_EXTEND
			CQuest* pQueset = ch->m_questList.FindQuest(105);
			if (pQueset && pQueset->GetQuestState() == QUEST_STATE_RUN)
			{
				QuestFailMsg(msg, pQueset);
				SEND_Q(msg, ch->m_desc);
				
				//do_Quest(ch, msg);

				///////////////////////////////////
				// pd4 퀘스트이면 helper 랭킹에 등록 : BW
				//
				// char index를 보냄
				CNetMsg pd4RankEndMsg;

				pd4RankEndMsg.Init(MSG_HELPER_COMMAND);
				pd4RankEndMsg << MSG_HELPER_PD4_RANK_END
							  << ch->m_index
							  << pQueset->GetQuestValue(0)
							  << 0;

				GAMELOG << init("PD4 END", ch->m_name, ch->m_nick, ch->m_desc->m_idname)
						<< pQueset->GetQuestValue(0) << end;

				if (IS_RUNNING_HELPER)
				{
					SEND_Q(pd4RankEndMsg, gserver.m_helper);
				}
				else
				{
					GAMELOG << init("PD4 QUEST ERROR : RANK END ERROR NOT HELPER")
							<< ch->m_index << end;
				}
				
			}
#else // QUEST_DATA_EXTEND
			int idx = ch->m_questList.FindQuest(105);
			if (idx != -1)
			{
				QuestFailMsg(msg, ch->m_questList.m_list[idx]);
				SEND_Q(msg, ch->m_desc);
				
				//do_Quest(ch, msg);

				///////////////////////////////////
				// pd4 퀘스트이면 helper 랭킹에 등록 : BW
				//
				// char index를 보냄
				CNetMsg pd4RankEndMsg;

				pd4RankEndMsg.Init(MSG_HELPER_COMMAND);
				pd4RankEndMsg << MSG_HELPER_PD4_RANK_END
							  << ch->m_index
							  << ch->m_questList.m_list[idx]->m_currentData[0]
							  << 0;

				GAMELOG << init("PD4 END", ch->m_name, ch->m_nick, ch->m_desc->m_idname)
						<< ch->m_questList.m_list[idx]->m_currentData[0] << end;

				if (IS_RUNNING_HELPER)
				{
					SEND_Q(pd4RankEndMsg, gserver.m_helper);
				}
				else
				{
					GAMELOG << init("PD4 QUEST ERROR : RANK END ERROR NOT HELPER")
							<< ch->m_index << end;
				}
				
			}
#endif // QUEST_DATA_EXTEND

			ch->m_pd4StartTime = 0;
		}

	}

	ch->m_lastupdate--;			// 자동 저장

	if (ch->IsSetPlayerState(PLAYER_STATE_PKMODEDELAY))
	{
		if (ch->m_pkmodedelay > 0)
			ch->m_pkmodedelay--;
		else
		{
			ch->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
			ch->m_pkmodedelay = 0;
			CNetMsg rmsg;
			ActionMsg(rmsg, ch, 0, AGT_PKMODE);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}
	}

	// 사제 시스템 시간 체크
	if (ch->m_teachType != MSG_TEACH_NO_TYPE)
	{
		time_t ti = time(0);
		// 선생
		if (ch->m_teachType == MSG_TEACH_TEACHER_TYPE)
		{
			// 초고제 받으라는 메세지
			if( ch->m_displayCanSstone
				&& ((ch->m_fame >= 200  && ch->m_superstone < 1 )
				|| (ch->m_fame >= 300 && ch->m_superstone < 2 )
				|| (ch->m_fame >= 500 && ch->m_superstone < 3)
				|| (ch->m_fame >= 800 && ch->m_superstone < 4)
				|| (ch->m_fame >= 1200 && ch->m_superstone < 5)
				|| (ch->m_fame >= 1700 && ch->m_superstone < 6)
				|| (ch->m_fame >= 2300 && ch->m_superstone < 7)) )
			{
				CNetMsg rmsg;
				SysEnableSuperStoneMsg(rmsg, ch->m_fame);
				SEND_Q(rmsg, ch->m_desc);
				ch->m_displayCanSstone = false;
			}				

			int i;
			for (i=0; i < TEACH_MAX_STUDENTS; i++)
			{
				if (ch->m_teachIdx[i] == -1)
					continue;

				// 시간 오바
				if (ti - ch->m_teachTime[i] > TEACH_LIMIT_SEC)
				{
					CNetMsg rmsg;

					TeachEndMsg(rmsg, ch->m_index, ch->GetName(), ch->m_teachIdx[i], ch->m_teachName[i], MSG_TEACH_END_FAIL);
					SEND_Q(rmsg, ch->m_desc);

					// 선생 디비만 해제
					if(IS_RUNNING_HELPER)
					{
						HelperTeachTimeover(rmsg, CANCELTEACHER, ch->m_index, ch->m_teachIdx[i]);
						SEND_Q(rmsg, gserver.m_helper);
					}

					GAMELOG << init("TEACH_FAIL_TIMEOVER")
							<< "STU_INDEX"  << delim 
							<< ch->m_teachIdx[i] << delim
							<< "STU_NAME" << delim
							<< ch->m_teachName[i] << delim
							<< "STU_TIME" << delim
							<< ch->m_teachTime[i] << delim
							<< ch->m_name << delim
							<< ch->m_nick << delim
							<< ch->m_desc->m_idname << delim
							<< ch->m_fame << end;

					// 혼자 셋팅해제
					ch->m_teachIdx[i] = -1;
					ch->m_teachJob[i] = -1;
#ifdef ENABLE_CHANGEJOB
					ch->m_teachJob2[i] = 0;
#endif
					ch->m_teachLevel[i] = 0;
					ch->m_teachName[i][0] = '\0';
					ch->m_teachTime[i] = 0;
					
					//0627					
					ch->m_cntFailStudent++;					

					// teachType 검사
					bool bTeacher = false;
					
					int j;
					for (j=0; j < TEACH_MAX_STUDENTS; j++)
					{
						if (ch->m_teachIdx[j] != -1)
						{
							bTeacher = true;
							break;
						}
					}
					if (!bTeacher)
						ch->m_teachType = MSG_TEACH_NO_TYPE;
				}
			}
		}
		//학생
		else if (ch->m_teachType == MSG_TEACH_STUDENT_TYPE)
		{
			// 시간 오바
			if (ti - ch->m_teachTime[0] > TEACH_LIMIT_SEC)
			{
				CNetMsg rmsg;
				TeachEndMsg(rmsg, ch->m_teachIdx[0], ch->m_teachName[0], ch->m_index, ch->GetName(), MSG_TEACH_END_FAIL);
				SEND_Q(rmsg, ch->m_desc);

				// 학생 디비만 해제
				if(IS_RUNNING_HELPER)
				{
					HelperTeachTimeover(rmsg, CANCELSTUDENT, ch->m_teachIdx[0], ch->m_index);
					SEND_Q(rmsg, gserver.m_helper);
				}
         
				// 혼자 셋팅 해제
				ch->m_teachIdx[0] = -1;
				ch->m_teachJob[0] = -1;
#ifdef ENABLE_CHANGEJOB
				ch->m_teachJob2[0] = 0;
#endif
				ch->m_teachLevel[0] = 0;
				ch->m_teachName[0][0] = '\0';
				ch->m_teachTime[0] = 0;
				if (ch->m_level > TEACH_LEVEL_STUDENT)
					ch->m_teachType = MSG_TEACH_NO_TYPE;
				else
					ch->m_teachType = MSG_TEACG_NO_STUDENT_TYPE;
			}
		}
	}

	// 정당방위일때
	if (ch->IsSetPlayerState(PLAYER_STATE_RAMODE))
	{
		// 정당방위 리스트가 있다
		bool bHaveTarget = false;

		CRaChar* raList;
		CRaChar* raListNext = ch->m_raList;

		while ((raList = raListNext))
		{
			raListNext = raList->m_next;

			// 가해자만 해당
			if (!raList->m_bAttacker)
				continue;

			raList->m_raPulse --;

			if (raList->m_raPulse < 1)
			{
				DelRaList(raList->m_raTarget);
				continue;
			}

			if (raList->m_raPulse == RAMODE_DELAY_PULSE)
			{
				bHaveTarget = true;
				CNetMsg rMsg;
				RightAttackMsg(rMsg, raList->m_raTarget, MSG_RIGHT_ATTACK_DELAY);
				SEND_Q(rMsg, ch->m_desc);
				SEND_Q(rMsg, raList->m_raTarget->m_desc);
				continue;
			}
			bHaveTarget = true;
		}

		if (!bHaveTarget)
			ch->ResetPlayerState(PLAYER_STATE_RAMODE);
	}

	// 변신중일때
	if (ch->IsSetPlayerState(PLAYER_STATE_CHANGE) && ch->m_changeIndex != -1)
	{
		ch->m_changePulse--;
		if (ch->m_changePulse < 1)
			ch->CancelChange();
	}

	// 1시간에 PK 성향 1포인트 회복
	if (ch->m_pkRecoverPulse != 0 && ch->m_pArea)
	{
		ch->m_pkRecoverPulse--;
		if (ch->m_pkRecoverPulse <= 0)
		{
			if (ch->m_pkPenalty < 0)
			{
#ifdef RESTRICT_PK
				if (ch->m_lastPKTime == -1 || ch->m_lastPKTime + RESTRICT_PK <= gserver.m_gameTime)
				{
#endif // RESTRICT_PK
#if defined (NON_PK_SYSTEM)
					if( !gserver.m_bNonPK )
						ch->m_pkPenalty++;

#else
					ch->m_pkPenalty++;
#endif
					CNetMsg rmsg;
					CharStatusMsg(rmsg, ch, 0);
					ch->m_pArea->SendToCell(rmsg, ch, false);
					ch->m_bChangeStatus = true;
#ifdef RESTRICT_PK
				}
#endif // RESTRICT_PK
			}
			if (ch->m_pkPenalty < 0)
				ch->m_pkRecoverPulse = PULSE_REAL_HOUR;
			else
				ch->m_pkRecoverPulse = 0;
		}
	}
	
	// 스킬 취소 상태로
	// 죽거나
	// 스킬 발동이후 시간 지났을때
	if (ch->m_currentSkill
		&& (DEAD(ch) ||	(ch->m_currentSkill->m_state == SKILL_CON_FIRE && m_pulse - ch->m_currentSkill->m_usetime > ch->m_currentSkill->m_proto->m_readyTime + ch->m_currentSkill->m_proto->m_stillTime + ch->m_currentSkill->m_proto->m_fireTime)))
	{
		ch->m_currentSkill->Cancel(ch);
		ch->m_currentSkill = NULL;
	}
	
	// assist 시간 감소
	ch->m_assist.DecreaseTime();
	
	// pc 상태 회복
	if (!DEAD(ch) && m_pulse - ch->m_recoverPulse >= PC_RECOVER_PULSE)
	{
		ch->RecoverPC();
		ch->m_recoverPulse = m_pulse;
	}
	
	// 아이템 회복 처리
	if (ch->m_recoverHPItemTime > 0)
	{
		if (DEAD(ch))
			ch->m_recoverHPItemTime = 0;
		else
		{
			ch->m_recoverHPItemTime--;
			if (ch->m_recoverHPItemTime % 10 == 0)
			{
				ch->m_hp += ch->m_recoverHPItemValue;
				if (ch->m_hp > ch->m_maxHP)
					ch->m_hp = ch->m_maxHP;
				ch->m_bChangeStatus = true;
			}
		}
	}
	
	if (ch->m_recoverMPItemTime > 0)
	{
		if (DEAD(ch))
			ch->m_recoverMPItemTime = 0;
		else
		{
			ch->m_recoverMPItemTime--;
			if (ch->m_recoverMPItemTime % 10 == 0)
			{
				ch->m_mp += ch->m_recoverMPItemValue;
				if (ch->m_mp > ch->m_maxMP)
					ch->m_mp = ch->m_maxMP;
				ch->m_bChangeStatus = true;
			}
		}
	}

	// 빌링 아이템 처리
	if ( ch->m_billReqTime > 0)
	{
		ch->m_billReqTime--;
		if ( ch->m_billReqTime <= 0)
		{
			ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
			ch->m_billReqTime = 0;
		}
	}
	
	// 순간 이동 처리
	if (ch->m_reqWarpTime > 0)
	{
		if (DEAD(ch))
		{
			ch->m_reqWarpTime = 0;
			ch->m_reqWarpType = -1;
			ch->m_reqWarpData = -1;
			ch->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING | PLAYER_STATE_WARP);
		}
		else
		{
			ch->m_reqWarpTime--;
			if (ch->m_reqWarpTime <= 0)
			{
				CNetMsg wmsg;
				WarpEndMsg(wmsg, ch);
				ch->m_pArea->SendToCell(wmsg, ch, true);

				switch (ch->m_reqWarpType)
				{
				case IONCE_WARP_RETURN:
					{
						int zone;
						int zonepos;
						int zone_offset;
						
						// 태국 pvp 존 이동 아이템이
						// 귀환 스크롤의 num1을 존번호로 씁니당.
						// 고로 요거 추가합니당 : bw
						if(ch->m_reqWarpData != -1 )
						{
							zone = ch->m_reqWarpData;
							zonepos = 0;
							zone_offset = gserver.FindZone(zone);
						}
						else
						{
#if defined (EX_GO_ZONE)
							zone_offset = ch->GetLastSaveZone(&zone);
							zonepos = 0;
#else
							zone_offset = gserver.FindNearestZone(ch->m_pZone->m_index, GET_X(ch), GET_Z(ch), &zone, &zonepos);
#endif // EX_GO_ZONE

#ifdef MONSTER_COMBO
							if(ch->m_pZone->IsComboZone())
							{
								zone = ZONE_COMBO_DUNGEON;
								zone_offset = gserver.FindZone(ZONE_COMBO_DUNGEON);
								zonepos = 0;
							}
#endif // MONSTER_COMBO
						}

						if (zone_offset == -1)
						{
							zone = ZONE_START;
							zone_offset = gserver.FindZone(zone);
							zonepos = 0;
						}

						if (zone_offset != -1)
						{
							// 060109 : BS : BEGIN : 존이동 메시지 사용하지 않고 직접 호출
							CZone* pZone = gserver.m_zones + zone_offset;
							GoZone(ch, zone,
									pZone->m_zonePos[zonepos][0],														// ylayer
									GetRandom(pZone->m_zonePos[zonepos][1], pZone->m_zonePos[zonepos][3]) / 2.0f,		// x
									GetRandom(pZone->m_zonePos[zonepos][2], pZone->m_zonePos[zonepos][4]) / 2.0f);		// z
//
//							CNetMsg rmsg;
//							GoZoneMsg(rmsg, zone, zonepos, "", 0);
//							do_GoZone(ch, rmsg);
							// 060109 : BS : END
						}
					}
					break;
					
				case IONCE_WARP_MEMPOS:
					ch->GoMemPos(ch->m_reqWarpData);
					break;
#ifdef PRIMIUM_MEMORYBOOK
				case IONCE_WARP_MEMPOSPLUS:
					ch->GoMemPosPlus(ch->m_reqWarpData);
					break;
#endif // PRIMIUM_MEMORYBOOK
				}

				ch->m_reqWarpTime = 0;
				ch->m_reqWarpType = -1;
				ch->m_reqWarpData = -1;
				ch->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING | PLAYER_STATE_WARP);
			}
		}
	}

#ifdef ENABLE_SINGLEDUNGEON_DATA
	// 퍼스널 던전 입장 카운트 리셋
	time_t pdtime;
	struct tm tm1 = NOW();

	struct tm tm2;

	if ( (ch->m_pd3Count && ch->m_pd3Time) || (ch->m_pd3Count && ch->m_pd3Time == 0))
	{
		pdtime = ch->m_pd3Time;
		memcpy(&tm2, localtime(&pdtime), sizeof(struct tm));
		if (tm1.tm_yday != tm2.tm_yday)
			ch->m_pd3Count = 0;
	}
	if ( (ch->m_pd4Count && ch->m_pd4Time) || (ch->m_pd4Count && ch->m_pd4Time == 0))
	{
		pdtime = ch->m_pd4Time;
		memcpy(&tm2, localtime(&pdtime), sizeof(struct tm));
		if (tm1.tm_yday != tm2.tm_yday)
			ch->m_pd4Count = 0;
	}
#endif // #ifdef ENABLE_SINGLEDUNGEON_DATA


	////////////////////////
	// 매초 검사해야 할 루틴
	if (ch->m_perSecondPulse == 0)
	{
#ifdef ENABLE_OXQUIZ
		// OX 퀴즈 종료후 돌아가기
		if (ch->m_pZone->IsOXQuizRoom() && ch->m_admin < 2)
		{
			if (!gserver.m_bEventOX)
			{
				CZone* pStartZone = gserver.m_zones + gserver.FindZone(ZONE_START);
				GoZone(ch, ZONE_START,
						pStartZone->m_zonePos[0][0],													// ylayer
						GetRandom(pStartZone->m_zonePos[0][1], pStartZone->m_zonePos[0][3]) / 2.0f,		// x
						GetRandom(pStartZone->m_zonePos[0][2], pStartZone->m_zonePos[0][4]) / 2.0f);	// z
				return ;
			}
		}
#endif // ENABLE_OXQUIZ

#ifndef _DEBUG
		if( (gserver.m_gameTime % LCHOUR) < 10 )
		{
#endif // _DEBUG
			CNetMsg msg;
			EnvTimeMsg(msg);
			SEND_Q(msg, ch->m_desc);
#ifndef _DEBUG
		}
#endif // _DEBUG

#ifdef ENABLE_WAR_CASTLE
		// 공성/수성 리젠 장소에서 회복 증가
		if (!DEAD(ch) && ch->m_pZone && ch->GetMapAttr() == MAPATT_WARZONE)
		{
			int joinflag = ch->GetJoinFlag(ch->m_pZone->m_index);
			if (joinflag != WCJF_NONE)
			{
				CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
				if (castle)
				{
					if (castle->GetState() != WCSF_NORMAL)
					{
						bool bIsIn = false;
						if (IS_ATTACK_TEAM(joinflag))
						{
							if (castle->IsInRegenPoint(ch, true))
								bIsIn = true;
						}
						else if (IS_DEFENSE_TEAM(joinflag))
						{
							if (castle->IsInRegenPoint(ch, false))
								bIsIn = true;
						}
						if (bIsIn)
						{
							ch->m_hp += ch->m_maxHP / 20;
							ch->m_mp += ch->m_maxMP / 20;
							if (ch->m_hp > ch->m_maxHP)
								ch->m_hp = ch->m_maxHP;
							if (ch->m_mp > ch->m_maxMP)
								ch->m_mp = ch->m_maxMP;
							ch->m_bChangeStatus = true;
							CNetMsg rmsg;
							CharStatusMsg(rmsg, ch, 0);
							ch->m_pArea->SendToCell(rmsg, ch, true);
						}
					}
				}
			}
		}

		if (ch->m_bCreateMixItem)
		{
			CWarCastle* castle = CWarCastle::GetCastleObject(CWarCastle::GetCurSubServerCastleZoneIndex());
			if (!castle || !castle->IsRegenShop())
			{
				CNetMsg rmsg;
				CInventory* invens[3];
				invens[0] = GET_INVENTORY(ch, INVENTORY_NORMAL);
				invens[1] = GET_INVENTORY(ch, INVENTORY_QUEST);
				invens[2] = GET_INVENTORY(ch, INVENTORY_EVENT);
				int i;
				for (i = 0; i < 3; i++)
				{
					int j;
					for (j = 0; j < MAX_INVENTORY_ROWS; j++)
					{
						int k;
						for (k = 0; k < ITEMS_PER_ROW; k++)
						{
							CItem* item = invens[i]->GetItem(j, k);
							if (item && (item->m_itemProto->m_flag & ITEM_FLAG_MIX))
							{
								ItemDeleteMsg(rmsg, item);
								SEND_Q(rmsg, ch->m_desc);
								RemoveFromInventory(ch, item, true, true);
							}
						}
					}
				}
				ch->m_bCreateMixItem = false;
			}
		}
#endif // #ifdef ENABLE_WAR_CASTLE

#ifdef ENABLE_PET
		// 펫 상태 갱신
		ch->UpdatePetValue();
#endif // #ifdef ENABLE_PET
#ifdef ATTACK_PET
		ch->UpdateAPetValue();
#endif //ATTACK_PET

#ifdef EVENT_NEWYEAR_2006_TIME
		// 2006 신년이벤트
		if (m_bNewYear2006Event && ch->m_pulseEventNewYear2006 + PULSE_REAL_MIN * 5 >= gserver.m_pulse)
			ch->m_nTimeEventNewYear2006++;
#endif // #ifdef EVENT_NEWYEAR_2006_TIME
#ifdef EVENT_SEARCHFRIEND_TIME
		// 휴면 케릭 이벤트
		if (((ch->m_bEventSearchFriendSelect == true) 
			&& (ch->m_nEventSearchFriendListCount >= 1)
			&& ch->m_pulseEventSearchFriend + PULSE_REAL_MIN * 5 >= gserver.m_pulse)
			&& (ch->m_nTimeEventSearchFriend <= 216000))
		{
			ch->m_nTimeEventSearchFriend++;
			
			if ((ch->m_nTimeEventSearchFriend % 3600) == 0)
			{
				//1시간 간격으로 휴면케릭 사냥시 알림.
				CNetMsg rmsg;
				ch->m_bGoodEventSearchFriendListImprove = true;
				EventHelperSearchFriendOneTimeCheckReqMsg(rmsg, ch->m_nTimeEventSearchFriend, ch->m_index);
				SEND_Q(rmsg, m_helper);
			}
		}
		//보상여부 갱신 되는 부분.
		if((ch->m_bGoodEventSearchFriendListImprove == false)&& ch->m_pulseEventSearchFriendList + PULSE_REAL_MIN * 5 <= gserver.m_pulse)
		{
			ch->m_bGoodEventSearchFriendListImprove = true;
		}
#endif // #ifdef EVENT_SEARCHFRIEND_TIME

		ch->m_invenNormal.CheckItemTime();
		ch->m_invenQuest.CheckItemTime();
		ch->m_invenEvent.CheckItemTime();

#ifdef RESTRICT_PVP_SKILL
		if (ch->m_nRestrictPvPSkillDelaySec > 0)
			ch->m_nRestrictPvPSkillDelaySec--;
#endif // RESTRICT_PVP_SKILL

#ifdef USING_NPROTECT
		if (ch->m_desc->m_pGGAuth && ch->m_desc->m_pGGAuth->CheckNextQueryTime())
		{
			if (!ch->m_desc->m_pGGAuth->m_bAnswer)
			{
				GAMELOG << init("NPROTECT Answer FAIL 1", ch) << end;
				CloseSocket(ch->m_desc, false);
				return ;
			}
			CNetMsg rmsg;
			if (!ch->m_desc->m_pGGAuth->MakeGameGuardAuthQuery(rmsg))
			{
				GAMELOG << init("NPROTECT AUTH FAIL 1", ch) << end;
				CloseSocket(ch->m_desc, false);
				return ;
			}
			SEND_Q(rmsg, ch->m_desc);
			ch->m_desc->m_pGGAuth->SetNextAuthQueryTime(NPROTECT_QUERY_TIME);
		}
#endif // USING_NPROTECT
	}
	// if (ch->m_perSecondPulse == 0)
	/////////////////////////////////




	if (ch->m_perSecondPulse <= 0)
		ch->m_perSecondPulse = PULSE_REAL_SEC;

	if (ch->GetMapAttr() != ch->m_recentAtt)
	{
		// PK 존에 들어가면 PK 바로 켜기
		if (ch->GetMapAttr() == MAPATT_FREEPKZONE
#ifdef LC_JPN
			)
#else
			&& ch->m_level > PKMODE_LIMIT_LEVEL)
#endif
		{
			ch->ResetPlayerState(PLAYER_STATE_PKMODEDELAY | PLAYER_STATE_PKMODE);
			ch->SetPlayerState(PLAYER_STATE_PKMODE);
			//ch->CancelInvisible();

			CNetMsg rmsg;
			ActionMsg(rmsg, ch, ACTION_GENERAL, AGT_PKMODE);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}


#ifdef ENABLE_WAR_CASTLE
		// 지역이 바뀌면 아이템 정보 갱신을 위해 재계산은 한다 : 공성 전용 아이템을 위해 필요
		if (ch->GetMapAttr() == MAPATT_WARZONE || ch->m_recentAtt == MAPATT_WARZONE)
			ch->CalcStatus(true);
#endif
		ch->m_recentAtt = ch->GetMapAttr();
		ch->m_bChangeStatus = true;
	}

#ifdef ENABLE_WAR_CASTLE
	// 공성지역이 아닌 곳에서 저레벨 PvP 모드 해제
	if (ch->m_level <= PKMODE_LIMIT_LEVEL && ch->IsSetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY))
	{
		bool bPvPOff = true;
		if (ch->m_pZone->m_index == CWarCastle::GetCurSubServerCastleZoneIndex() && ch->GetMapAttr() == MAPATT_WARZONE)
		{
			CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
			if (castle && castle->GetState() != WCSF_NORMAL)
				bPvPOff = false;
		}
#ifdef LC_JPN
		if ( ch->GetMapAttr() == MAPATT_FREEPKZONE )
			bPvPOff = false;
#endif
#ifdef FREE_PK_SYSTEM
		if( gserver.m_bFreePk )
			bPvPOff = false;
#endif // FREE_PK_SYSTEM
		if (bPvPOff)
		{
			ch->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
			CNetMsg rmsg;
			ActionMsg(rmsg, ch, ACTION_GENERAL, AGT_PKMODE);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}
	}
#endif

#ifdef LC_TLD
	if( (ch->m_etcEvent & GUILD_EORROR_BUG) && !(ch->m_lastupdate - PULSE_SAVE_PC + PULSE_REAL_MIN/6))
	{
		if(ch->GiveItem(19, 0, 0, GUILD_LEVEL1_NEED_MONEY, true)  )
		{
			ch->AddExpSP(0, GUILD_LEVEL1_NEED_SP * 10000, false);

			CNetMsg rmsg;
			EventErrorMsg(rmsg, MSG_EVENT_GUILD_REWARD);

			// 길드 레벨 2보상
			if( ch->m_index == 73
				|| ch->m_index == 22384
				|| ch->m_index == 1463
				|| ch->m_index == 27062
				|| ch->m_index == 5225
				|| ch->m_index == 10142
				|| ch->m_index == 377
				|| ch->m_index == 105
				|| ch->m_index == 6468
				|| ch->m_index == 2823
				|| ch->m_index == 18409
				|| ch->m_index == 38070
				|| ch->m_index == 12014
				|| ch->m_index == 14055
				|| ch->m_index == 31258
				|| ch->m_index == 7814
				|| ch->m_index == 5196
				|| ch->m_index == 23381
				|| ch->m_index == 21183
				|| ch->m_index == 8366
				|| ch->m_index == 15062
				|| ch->m_index == 48
				|| ch->m_index == 7357
				|| ch->m_index == 11240
				|| ch->m_index == 2287
				|| ch->m_index == 25317 )
			{
				ch->GiveItem(19, 0, 0, GUILD_LEVEL2_NEED_MONEY, true);
				ch->AddExpSP(0, GUILD_LEVEL2_NEED_SP * 10000, false);
				rmsg << (LONGLONG) GUILD_LEVEL2_NEED_MONEY
					 << (LONGLONG) GUILD_LEVEL2_NEED_SP;

				GAMELOG << init("GUILD_BUG_REWARD", ch->m_name, ch->m_desc->m_idname)
						<< 2 << end;

			}
			// 길드 레벨 3 보상
			else if( ch->m_index == 12707
					|| ch->m_index == 2056
					|| ch->m_index == 3472
					|| ch->m_index == 17520
					|| ch->m_index == 898
					|| ch->m_index == 3037
					|| ch->m_index == 286
					|| ch->m_index == 12160 )
			{
				ch->GiveItem(19, 0, 0, GUILD_LEVEL2_NEED_MONEY, true);
				ch->AddExpSP(0, GUILD_LEVEL2_NEED_SP * 10000, false);
				ch->GiveItem(19, 0, 0, GUILD_LEVEL3_NEED_MONEY, true);
				ch->AddExpSP(0, GUILD_LEVEL3_NEED_SP * 10000, false);

				rmsg << (LONGLONG) (GUILD_LEVEL2_NEED_MONEY + GUILD_LEVEL3_NEED_MONEY)
					 << (LONGLONG) (GUILD_LEVEL2_NEED_SP + GUILD_LEVEL3_NEED_MONEY);

				GAMELOG << init("GUILD_BUG_REWARD", ch->m_name, ch->m_desc->m_idname)
						<< 3 << end;
			}
			// 일반 보상
			else
			{
				rmsg << (LONGLONG) GUILD_LEVEL1_NEED_MONEY
					 << (LONGLONG) GUILD_LEVEL1_NEED_SP;

				GAMELOG << init("GUILD_BUG_REWARD", ch->m_name, ch->m_desc->m_idname)
						<< 1 << end;
			}

			SEND_Q(rmsg, ch->m_desc);
			ch->m_etcEvent = ch->m_etcEvent &~ GUILD_EORROR_BUG;
		}
	}
#endif

#ifdef AUTOSKILL
	
	ch->m_autoSkillTime--;
	if( ch->m_hp < ch->m_maxHP / 2)
	{
		if( ch->m_activeSkillList.Find( 152 ) )
		{
			if( ch->m_assist.Find(194, 1)
				|| ch->m_assist.Find(194, 2)
				|| ch->m_assist.Find(194, 3)
				|| ch->m_assist.Find(194, 4)
				|| ch->m_assist.Find(194, 5) )
			{
		
			}
			else
			{
				if ( ch->m_autoSkillTime < 0 )
				{
					ch->m_autoSkillTime = PULSE_AUTO_SKILL_DECREASE;
					CNetMsg rmsg;
					SkillAutoUseMsg(rmsg, 152);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}
	}
	else
	{
		if( ch->m_autoSkillTime < -PULSE_AUTO_SKILL_DECREASE)
			ch->m_autoSkillTime = 0;
	}
	
	
#endif

	CElemental* pElemental = NULL;
	CElemental* pElementalNext = ch->m_elementalList;
	while ((pElemental = pElementalNext))
	{
		pElementalNext = pElemental->m_nextElemental;
		if (pElemental->DecreaseRemainTime())
			ch->UnsummonElemental(pElemental);
		else
			pElemental->m_assist.DecreaseTime();
	}

	if (ch->m_evocationType == 0)
	{
		// 강신이 끝나고 5분이 지나면 시간 초기화
		if (ch->m_pulseEvocation[0] > 0 && ch->m_pulseEvocation[0] + EVOCATION_DURATION + EVOCATION_DELAY <= gserver.m_pulse)
			ch->m_pulseEvocation[0] = 0;
		if (ch->m_pulseEvocation[1] > 0 && ch->m_pulseEvocation[1] + EVOCATION_DURATION + EVOCATION_DELAY <= gserver.m_pulse)
			ch->m_pulseEvocation[1] = 0;
	}
	else
	{
		// 강신 지속 끝나는 것 검사
		if (ch->GetRemainEvocation(false) < 1)
			ch->Unevocation();
	}

#ifdef ETC_EVENT_UNIFY_NAMECHANGE
	if( ch->m_etcEvent & ETC_EVENT_UNIFY_NAMECHANGE )
	{
		if( !ch->GiveItem(1120, 0, 0, 1, true) )
		{
			GAMELOG << init("UNIFY_NAMECHANGE_ITEM_ERROR", ch) << end;
		}

		GAMELOG << init("UNIFY_NAMECHANGE_ITEM", ch) << end;
		ch->m_etcEvent = ch->m_etcEvent &~ ETC_EVENT_UNIFY_NAMECHANGE;
	}
#endif // #ifdef ETC_EVENT_UNIFY_NAMECHANGE

#ifdef ETC_EVENT_UNIFY_GUILDNAMECHANGE
	if( ch->m_etcEvent & ETC_EVENT_UNIFY_GUILDNAMECHANGE )
	{
		if( !ch->GiveItem(843, 0, 0, 1, true) )
		{
			GAMELOG << init("UNIFY_GUILDNAMECHANGE_ITEM_ERROR", ch) << end;
		}

		GAMELOG << init("UNIFY_GUILDNAMECHANGE_ITEM", ch) << end;
		ch->m_etcEvent = ch->m_etcEvent &~ ETC_EVENT_UNIFY_GUILDNAMECHANGE;
	}
#endif

#ifdef AUTO_POTION	
	int hp = ch->m_hp;
	int maxhp = ch->m_maxHP;
	
	int mp = ch->m_mp;
	int maxmp = ch->m_maxMP;

	int posioncount = 0;
	CNetMsg useItemMsg;

	bool bHpRemain = false;	// 쿨타임
	bool bMpRemain = false;	// 쿨타임

	if (hp < maxhp * 0.5 && hp > 0)
	{	// 체력이 절반 이하

		CAssistData *  outData = NULL;
		bool outHelp = false;
		ch->m_assist.FindByType(MT_ASSIST, MST_ASSIST_HP, &outHelp, &outData);


		if (outHelp == true 
			&& outData != NULL
			&& outData->m_remain > 0)
		{	// 체력 회복 물약을 먹었던 경우
			bHpRemain = true;
		}
		
		if (bHpRemain == false)
		{
			for( int k = WEARING_ACCESSORY1; k <= WEARING_ACCESSORY3; k++ )
			{
				if( ch->m_wearing[k] )
				{
					if( ch->m_wearing[k]->m_itemProto->m_index == ONE_PERIOD_ITEM 
						|| ch->m_wearing[k]->m_itemProto->m_index == SEVEN_PERIOD_ITEM 
						|| ch->m_wearing[k]->m_itemProto->m_index == THIRTY_PERIOD_ITEM 
						|| ch->m_wearing[k]->m_itemProto->m_index == 2610 )
					{
						CInventory* inven = GET_INVENTORY( ch, INVENTORY_NORMAL );
						int i, j;
						CItem* item;

						for( i = 0; i < MAX_INVENTORY_ROWS; i++ )
						{
							for( j = 0; j < ITEMS_PER_ROW; j++ )
							{
								item = inven->GetItem( i, j );
								if( item )
								{
									const CItemProto* itemProto = item->m_itemProto;
									if( itemProto->m_typeIdx != ITYPE_POTION || itemProto->m_subtypeIdx != IPOTION_HP )
									{
										continue;
									}
									else
									{										
										ItemUseMsg( useItemMsg, ch, INVENTORY_NORMAL, i, j, item->m_index, item->m_idNum );
										do_Item(ch, useItemMsg );
										posioncount++;
										goto USE_HP_POTION;
									}
								}
							} // for( j = 0; j < ITEMS_PER_ROW; j++ )
						} // for( i = 0; i < invencount; i++ )
					}
				} //if( ch->m_wearing[i] )
			} // for( i ; i <= WEARING_ACCESSORY3; i++ )
		}
	}

USE_HP_POTION:
	if (mp < maxmp * 0.5 )
	{	// 마나가 절반 이하
		CAssistData *  outData = NULL;
		bool outHelp = false;
		ch->m_assist.FindByType(MT_ASSIST, MST_ASSIST_MP, &outHelp, &outData);


		if (outHelp == true 
			&& outData != NULL
			&& outData->m_remain > 0)
		{	// 마나 회복 물약을 먹었던 경우
			bMpRemain = true;
		}

		if (bMpRemain == false)
		{
			for( int k = WEARING_ACCESSORY1; k <= WEARING_ACCESSORY3; k++ )
			{
				if( ch->m_wearing[k] )
				{
					if( ch->m_wearing[k]->m_itemProto->m_index == ONE_PERIOD_ITEM 
						|| ch->m_wearing[k]->m_itemProto->m_index == SEVEN_PERIOD_ITEM 
						|| ch->m_wearing[k]->m_itemProto->m_index == THIRTY_PERIOD_ITEM 
						|| ch->m_wearing[k]->m_itemProto->m_index == 2610 )
					{
						CInventory * inven = GET_INVENTORY( ch, INVENTORY_NORMAL );
						int i, j;
						CItem* item;

						for( i = 0; i < MAX_INVENTORY_ROWS; i++ )
						{
							for( j = 0; j < ITEMS_PER_ROW; j++ )
							{
								item = inven->GetItem( i, j );
								if( item )
								{
									const CItemProto* itemProto = item->m_itemProto;
									if( itemProto->m_typeIdx != ITYPE_POTION || itemProto->m_subtypeIdx != IPOTION_MP )
									{
										continue;
									}
									else
									{										
										ItemUseMsg( useItemMsg, ch, INVENTORY_NORMAL, i, j, item->m_index, item->m_idNum );
										do_Item(ch, useItemMsg );
										posioncount++;
										goto USE_MP_POTION;
									}
								}
							} // for( j = 0; j < ITEMS_PER_ROW; j++ )
						} // for( i = 0; i < invencount; i++ )	
					}
				} //if( ch->m_wearing[i] )
			} // for( i ; i <= WEARING_ACCESSORY3; i++ )
		}
	}

USE_MP_POTION:
	if ( posioncount == 0 
		&&
			((hp < maxhp * 0.5 && hp > 0) || (mp < maxmp * 0.5))
		&&  (bHpRemain == false && bMpRemain == false)
		)
		
	{	// 체력이나 마나가 50% 이하에서 사용할 포션이 없을 경우
		bool bWare = true;
		for( int k = WEARING_ACCESSORY1; k <= WEARING_ACCESSORY3; k++ )
		{
			if( ch->m_wearing[k] )
			{
				if( ch->m_wearing[k]->m_itemProto->m_index == ONE_PERIOD_ITEM 
					|| ch->m_wearing[k]->m_itemProto->m_index == SEVEN_PERIOD_ITEM 
					|| ch->m_wearing[k]->m_itemProto->m_index == THIRTY_PERIOD_ITEM 
					|| ch->m_wearing[k]->m_itemProto->m_index == 2610 )
				{
					CNetMsg rmsg;
					char wear_pos = ch->m_wearing[k]->m_wearPos;
					
					ch->m_assist.CureByItemIndex(ch->m_wearing[k]->m_itemProto->m_index);

					ItemWearMsg(rmsg,  wear_pos, NULL, ch->m_wearing[k]);
					SEND_Q(rmsg, ch->m_desc);
					ch->m_wearing[k]->m_wearPos = WEARING_NONE;
					ch->m_wearing[k] = NULL;

					bWare = false;
				}	
			}
		}

		if(bWare == false)
		{	// 자동 물약 사용 아이템이 벗겨진 경우
			SysMsg( useItemMsg, MSG_SYS_NO_AUTO_ITEM );
			SEND_Q(useItemMsg, ch->m_desc);
		}
	}
#endif // AUTO_POTION
	
	// 스트레이아나 독
	ch->m_SkillTime_511++;
	if(ch->m_SkillTime_511 >= 2 * PULSE_REAL_MIN)
	{
		ch->m_SkillTime_511 = 0;
		if (ch->m_pZone->m_index == ZONE_STREIANA)
		{
			if(ch->m_job2)
			{
				CItemProto* pItem = gserver.m_itemProtoList.FindIndex(2859);
				if(pItem && ch->m_assist.FindBySkillIndex(pItem->m_num0))
					return;

				CSkill * pSkill = gserver.m_skillProtoList.Create(511, 1) ;
				if (pSkill && ch->CanApplySkill(pSkill->m_proto, pSkill->m_proto->Level(pSkill->m_level)))
				{
					bool bApply = false;
					ApplySkill(ch, ch, pSkill, -1 , bApply);
				}
			}
		}
	} 
}

int CServer::GetServerTime()
{
	struct tm ti = NOW();
	m_serverTime.year	= ti.tm_year + 1900;
	m_serverTime.month	= ti.tm_mon + 1;
	m_serverTime.day	= ti.tm_mday;
	m_serverTime.hour	= ti.tm_hour;
	m_serverTime.min	= ti.tm_min;
	m_serverTime.sec	= ti.tm_sec;

	time_t tmCur;
	time(&tmCur);
	return (LONGLONG)tmCur;
}

bool CServer::MakeMessengerClient()
{
	struct sockaddr_in msg_socket;
	
	if (m_messenger)
	{
		if (m_messenger->m_bclosed)
		{
			m_messenger->CloseSocket();
			delete m_messenger;
			m_messenger = NULL;
		}
	}
	
	m_messenger = new CDescriptor;
#ifdef CRYPT_NET_MSG
	m_messenger->m_bCryptNetMsg = false;
#endif // #ifdef CRYPT_NET_MSG
	
	if ((m_messenger->m_desc = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		puts("SYSERR: Error opening network connection: socket error");
		return false;
	}
	
	memset (&msg_socket, 0, sizeof(msg_socket));
	msg_socket.sin_family = AF_INET;
	msg_socket.sin_addr.s_addr = inet_addr (gserver.m_config.Find("Messenger Server", "IP"));
	msg_socket.sin_port = htons(atoi(gserver.m_config.Find("Messenger Server", "Port")));
	
	if (connect(m_messenger->m_desc, (struct sockaddr*)&msg_socket, sizeof(msg_socket)) < 0)
	{
		puts ("SYSERR: Cannot connect Messenger...");
		return false;
	}
	
	m_messenger->m_idletics = 0;
	Nonblock(m_messenger->m_desc);
	SetSendbuf(m_messenger->m_desc);
	
	return true;
}

bool CServer::MakeHelperClient()
{
	struct sockaddr_in msg_socket;
	
	if (m_helper)
	{
		if (m_helper->m_bclosed)
		{
			m_helper->CloseSocket();
			delete m_helper;
			m_helper = NULL;
		}
	}
	
	m_helper = new CDescriptor;
#ifdef CRYPT_NET_MSG
	m_helper->m_bCryptNetMsg = false;
#endif // #ifdef CRYPT_NET_MSG
	
	if ((m_helper->m_desc = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		puts("SYSERR: Error opening network connection: socket error");
		return false;
	}
	
	memset (&msg_socket, 0, sizeof(msg_socket));
	msg_socket.sin_family = AF_INET;
	msg_socket.sin_addr.s_addr = inet_addr (gserver.m_config.Find("Helper Server", "IP"));
	msg_socket.sin_port = htons(atoi(gserver.m_config.Find("Helper Server", "Port")));
	
	if (connect(m_helper->m_desc, (struct sockaddr*)&msg_socket, sizeof(msg_socket)) < 0)
	{
		puts ("SYSERR: Cannot connect Helper...");
		return false;
	}
	
	m_helper->m_idletics = 0;
	Nonblock(m_helper->m_desc);
	SetSendbuf(m_helper->m_desc);

	return true;
}

void CServer::SendMessengerHeader()
{
	if (m_messenger && !m_messenger->m_bclosed)
	{
		SEND_Q(m_messengerConnMsg, m_messenger);
		
		if (m_messenger->m_desc < 0) return ;
		if (m_messenger->m_outBuf.GetNextPoint())
		{
			if (m_messenger->ProcessOutput() < 0)
			{
				GAMELOG << init("SYS_ERR")
					<< "Messenger Disconnected..."
					<< end;
				m_messenger->m_bclosed = true;
			}
		}
	}
}

void CServer::SendHelperHeader()
{
	if (m_helper && !m_helper->m_bclosed)
	{
		SEND_Q(m_helperConnMsg, m_helper);
		
		if (m_helper->m_desc < 0) return ;
		if (m_helper->m_outBuf.GetNextPoint())
		{
			if (m_helper->ProcessOutput() < 0)
			{
				GAMELOG << init("SYS_ERR")
					<< "Helper Disconnected..."
					<< end;
				m_helper->m_bclosed = true;
			}
		}
	}
}

int CServer::FindZone(int zone)
{
	int i;
	for (i = 0; i < m_numZone; i++)
	{
		if (m_zones[i].m_index == zone)
			return i;
	}
	return -1;
}

#ifdef RAID
int CServer::FindInZone(int zone, int room)
{
	int i;
	for (i = 0; i < m_numInZone; i++)
	{
		if (m_inzones[i].m_index == zone && m_inzones[i].GetRoomNum() == room)
			return i;
	}
	return -1;
}
#endif //RAID

bool CServer::DropProbLoad()
{
	CDBCmd cmd;
	cmd.Init(&m_dbdata);

	cmd.SetQuery("SELECT * FROM t_drop_prob");

	if (!cmd.Open() || !cmd.MoveFirst())
		return false;

	if (!cmd.GetRec("a_item_prob",	m_itemDropProb) || !cmd.GetRec("a_money_prob",	m_moneyDropProb))
		return false;

	// 최대 300 최소 50 제한
	if (m_itemDropProb > 300)
		m_itemDropProb = 300;

	if (m_itemDropProb < 50)
		m_itemDropProb = 50;

	if (m_moneyDropProb > 300)
		m_moneyDropProb = 300;

	if (m_moneyDropProb < 50)
		m_moneyDropProb = 50;

	return true;
}

#ifdef USING_GMIP_TABLE
void CServer::LoadGMIPTable()
{
	int count;
	GMIPDATA* data = NULL;
	CDBCmd cmd;
	int i;

	cmd.Init(&m_dbdata);
	cmd.SetQuery("SELECT a_prefix, a_from, a_to FROM t_gm_ip");

	if (cmd.Open())
	{
		count = cmd.m_nrecords;
		if (count > 0)
		{
			i = 0;
			data = new GMIPDATA[count];
			memset(data, 0, sizeof(GMIPDATA) * count);
			while (cmd.MoveNext())
			{
				cmd.GetRec(0, data[i].prefix);
				cmd.GetRec(1, data[i].from);
				cmd.GetRec(2, data[i].to);
				i++;
			}
		}

		if (m_GMIP)
			delete [] m_GMIP;
		m_nGMIP = count;
		m_GMIP = data;
	}
}
#endif

//0502 kwon
void CServer::MoonStoneEndProcess(CPC* ch)
{

#if defined (EVENT_MOONSTONE)
	if(ch->m_nMoonStoneDigit == -1 || ch->m_nMoonStoneSum == 0)
	{		
		return;
	}
				
	//문스톤이 있는가
	int r, c;
	if (!ch->m_invenNormal.FindItem(&r, &c, 723 /*문스톤 */, 0, 0))
	{
		ch->m_nMoonStoneSum = 0;
		ch->m_nMoonStoneDigit = -1;				
		return;
	}			
				
	CItem* item = ch->m_invenEvent.GetItem(r, c);
				
	if (!item || item->Count() < 1)
	{
		ch->m_nMoonStoneSum = 0;
		ch->m_nMoonStoneDigit = -1;									
		return;
	}
				
	// 문스톤 item 수량 변경
	CNetMsg itemmsg;
				
	if (item->Count() > 1)
	{
		DecreaseFromInventory(ch, item, 1);
	}
	else
	{
		RemoveFromInventory(ch, item, true, true);
	}
				
	CItem* gift = NULL;				

	if(ch->m_nMoonStoneSum == 1)
	{
		switch(ch->m_nMoonStoneDigit)
		{
		case 0: //꽝.
			{																					
				return;
			}
			break;
		case 1://철 5개
			{
				gift = gserver.m_itemProtoList.CreateItem(213, -1, 0, 0, 5);
				if (!gift)
					return;													
			}	
			break;
		case 2://마노 5개
			{
				gift = gserver.m_itemProtoList.CreateItem(208, -1, 0, 0, 5);
				if (!gift)
					return;						
			}		
			break;
		case 3://크락 파란잎5개
			{
				gift = gserver.m_itemProtoList.CreateItem(197, -1, 0, 0, 5);
				if (!gift)
					return;							
			}		
			break;
		case 4://c등급 원소 5개
			{
				gift = gserver.m_itemProtoList.CreateItem(159, -1, 0, 0, 5);
				if (!gift)
					return;							
			}			
			break;
		case 5://소형 회복제 5개
			{
				gift = gserver.m_itemProtoList.CreateItem(43, -1, 0, 0, 5);
				if (!gift)
					return;							
			}			
			break;
		case 6://중형 회복제 5개
			{
				gift = gserver.m_itemProtoList.CreateItem(44, -1, 0, 0, 3);
				if (!gift)
					return;							
			}			
			break;
		}
	}
	else if(ch->m_nMoonStoneSum == 2)
	{
		switch(ch->m_nMoonStoneDigit)
		{
		case 0: //소형 마나 회복제 3개
			{
				gift = gserver.m_itemProtoList.CreateItem(484, -1, 0, 0, 3);
				if (!gift)
					return;						
			}	
			break;
		case 1: //방어,마법방어 향상 포션 3개.
			{
				gift = gserver.m_itemProtoList.CreateItem(511, -1, 0, 0, 3);
				
				if (!gift)
					return;					
			}		
			break;
		case 2: //공격,마법공격 향상 포션 3개.
			{
				gift = gserver.m_itemProtoList.CreateItem(510, -1, 0, 0, 3);
				
				if (!gift)
					return;						
			}			
			break;
		case 3: //대형회복제 3개
			{
				gift = gserver.m_itemProtoList.CreateItem(45, -1, 0, 0, 3);
				
				if (!gift)
					return;						
			}				
			break;
		case 4: //하급 경험의 결정 
			{
				gift = gserver.m_itemProtoList.CreateItem(671, -1, 0, 0, 1);
				
				if (!gift)
					return;						
			}		
			break;
		case 5: //대형회복제 5개
			{
				gift = gserver.m_itemProtoList.CreateItem(45, -1, 0, 0, 5);
				
				if (!gift)
					return;						
			}	
			break;
		case 6: // 경험치 25000
			{
				ch->AddExpSP((LONGLONG)25000, 0, false);
			}
			break;
		}
	}
	else if(ch->m_nMoonStoneSum == 3)
	{
		switch(ch->m_nMoonStoneDigit)
		{
		case 0: //명성치 +10
			{
				ch->m_fame = ch->m_fame + 10; //0627				
			}
			break;
		case 1: //대형마나회복포션3개
			{
				gift = gserver.m_itemProtoList.CreateItem(724, -1, 0, 0, 3);
				
				if (!gift)
					return;						
			}		
			break;
		case 2: //대형마나회복포션5개
			{
				gift = gserver.m_itemProtoList.CreateItem(724, -1, 0, 0, 5);
				
				if (!gift)
					return;						
			}			
			break;
		case 3: //숙련도 +10
			{
				ch->AddExpSP((LONGLONG)0, 100000, false);//숙련도는 10*10000
			}	
			break;
		case 4: //경험치 50000
			{
				ch->AddExpSP((LONGLONG)50000, 0, false);
			}
			break;
		case 5: //용서의 눈물
			{
				gift = gserver.m_itemProtoList.CreateItem(676, -1, 0, 0, 1);
				
				if (!gift)
					return;							
			}	
			break;
		case 6: // 구원의 눈물
			{
				gift = gserver.m_itemProtoList.CreateItem(675, -1, 0, 0, 1);
				
				if (!gift)
					return;						
			}	
			break;
		}
	}
	else if(ch->m_nMoonStoneSum == 4)
	{
		switch(ch->m_nMoonStoneDigit)
		{
		case 0: //용서의 눈물
			{
				gift = gserver.m_itemProtoList.CreateItem(676, -1, 0, 0, 1);
				
				if (!gift)
					return;						
			}	
			break;
		case 1: // 구원의 눈물
			{
				gift = gserver.m_itemProtoList.CreateItem(675, -1, 0, 0, 1);
				
				if (!gift)
					return;							
			}	
			break;
		case 2: //중급경험의 결정
			{
				gift = gserver.m_itemProtoList.CreateItem(672, -1, 0, 0, 1);
				
				if (!gift)
					return;							
			}		
			break;
		case 3: //경험치 75000
			{
				ch->AddExpSP((LONGLONG)75000, 0, false);
			}	
			break; 
		case 4: //경험치 100000
			{
				ch->AddExpSP((LONGLONG)100000, 0, false);
			}	
			break;
		case 5: //상급 경험의 결정
			{
				gift = gserver.m_itemProtoList.CreateItem(673, -1, 0, 0, 1);
				
				if (!gift)
					return;						
			}		
			break;
		case 6: //노력의 결정
			{
				gift = gserver.m_itemProtoList.CreateItem(674, -1, 0, 0, 1);
				
				if (!gift)
					return;						
			}			
			break;
		}
	}
	else if(ch->m_nMoonStoneSum == 5)
	{
		switch(ch->m_nMoonStoneDigit)
		{
		case 0: //고급제련석 1
			{
				gift = gserver.m_itemProtoList.CreateItem(85, -1, 0, 0, 1);
				if (!gift)
					return;						
			}
			break;
		case 1: //고급제련석 2
			{
				gift = gserver.m_itemProtoList.CreateItem(85, -1, 0, 0, 2);
				if (!gift)
					return;						
			}
			break;
		case 2: //고급제련석 3
			{
				gift = gserver.m_itemProtoList.CreateItem(85, -1, 0, 0, 3);
				if (!gift)
					return;							
			}	
			break;
		case 3: // 41레벨무기
		case 4: //+3 41레벨 무기
		case 5: //+5 41레벨 무기
			{
				int itemidx = -1;
				int plus = 0;
				switch (ch->m_job)
				{
				case JOB_TITAN:
					itemidx = 793;//대검
					break;
				case JOB_KNIGHT:
					itemidx = 800;//기사도
					break;
				case JOB_HEALER:
					itemidx = 808;//활
					break;
				case JOB_MAGE:
					itemidx = 815;//숏스테프
					break;
				case JOB_ROGUE:
					itemidx = 822;//단검
					break;
				case JOB_SORCERER:
					itemidx = 987;//소서러
					break;
				}
				
				if(ch->m_nMoonStoneDigit == 5)
				{
					plus = 5;
				}
				else if(ch->m_nMoonStoneDigit == 4)
				{
					plus = 3;
				}
				
				gift = gserver.m_itemProtoList.CreateItem(itemidx, -1, plus, 0, 1);
				if (!gift)
					return;													
			}
			break;
			
//		case 6: // 잭팟.
//			{				
//				CNetMsg hmsg;
//				HelperEventMoonStoneJackPotReqMsg(hmsg, ch->m_index);
//				SEND_Q(hmsg, gserver.m_helper);
//
//				return;
//
//			}
//			
//			break;
		}
	}

	ch->m_nMoonStoneSum = 0;
	ch->m_nMoonStoneDigit = -1;
	
	if (!gift)
		return;		
	
	// 들어갈 인벤토리 결정
	CInventory* inven = GET_INVENTORY(ch, GET_TAB(gift->m_itemProto->m_typeIdx, gift->m_itemProto->m_subtypeIdx));
	if (!inven)
		return ;
	
	bool bCountable = false;
	// 인벤에 넣기
	if (AddToInventory(ch, gift, true, true))
	{
		// 겹쳐졌는지 검사
		if (gift->tab() == -1)
		{
			bCountable = true;
		}
		else
		{
			// 돈 검사
			if (gift->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && ch->m_moneyItem == NULL)
				ch->m_moneyItem = gift;
		}
	}

	// Item LOG
	GAMELOG << init("CHANGE_LUCKYBAG", ch)
		<< itemlog(gift)
		<< end;
	
	if (bCountable)
		delete gift;

#elif defined (EVENT_NEW_MONNSTONE)
#endif
}

#ifdef ENABLE_WAR_CASTLE
// 세액 -> 성정보
void CServer::SaveTax()
{
#ifdef TLD_WAR_TEST
	ResetTax();
#else //#ifdef TLD_WAR_TEST
	CDBCmd cmd;
	cmd.Init(&m_dbcastle);

#ifdef CIRCLE_WINDOWS
	sprintf(g_buf, "UPDATE t_castle SET a_tax_item = a_tax_item + %I64d, a_tax_produce = a_tax_produce + %I64d WHERE a_zone_index=7", m_taxItem / 3, m_taxProduceCastle / 3);
#else
	sprintf(g_buf, "UPDATE t_castle SET a_tax_item = a_tax_item + %lld,  a_tax_produce = a_tax_produce + %lld WHERE a_zone_index=7",  m_taxItem / 3, m_taxProduceCastle / 3);
#endif

#ifdef LC_TLD // 태국 세금 1/3로 한다
	m_taxItem /= 3;
	m_taxProduceCastle /= 3;
#endif

	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		GAMELOG << init("MERAC TAX SAVE FAIL")
				<< "ITEM" << delim
				<< (m_taxItem / 3) << delim
				<< "PRODUCE" << delim
				<< (m_taxProduceCastle / 3)
				<< end;
	}
	else
	{
		GAMELOG << init("MERAC TAX SAVE SUCCESS")
				<< "ITEM" << delim
				<< (m_taxItem / 3) << delim
				<< "PRODUCE" << delim
				<< (m_taxProduceCastle / 3)
				<< end;
		ResetTax();
	}
#endif //#ifdef TLD_WAR_TEST

#ifdef DRATAN_CASTLE
	CDBCmd cmd2;
	cmd2.Init(&m_dbcastle);

#ifdef CIRCLE_WINDOWS
	sprintf(g_buf, "UPDATE t_castle SET a_tax_item = a_tax_item + %I64d, a_tax_produce = a_tax_produce + %I64d WHERE a_zone_index=4", 
		m_taxItemDratan / 3, m_taxProduceCastleDratan / 3);
#else
	sprintf(g_buf, "UPDATE t_castle SET a_tax_item = a_tax_item + %lld,  a_tax_produce = a_tax_produce + %lld WHERE a_zone_index=4",  
		m_taxItemDratan / 3, m_taxProduceCastleDratan / 3);
#endif

	cmd2.SetQuery(g_buf);
	if (!cmd2.Update())
	{
		GAMELOG << init("DRATAN TAX SAVE FAIL")
				<< "ITEM" << delim
				<< (m_taxItemDratan / 3) << delim
				<< "PRODUCE" << delim
				<< (m_taxProduceCastleDratan / 3)
				<< end;
	}
	else
	{
		GAMELOG << init("DRATAN TAX SAVE SUCCESS")
				<< "ITEM" << delim
				<< (m_taxItemDratan / 3) << delim
				<< "PRODUCE" << delim
				<< (m_taxProduceCastleDratan / 3)
				<< end;
		ResetTaxDratan();
	}
#endif // DRATAN_CASTLE
}

// 세금 -> 길드
void CServer::DivideTax()
{

#ifdef TLD_WAR_TEST
#else
	// 성세금
	CDBCmd cmd;
	cmd.Init(&m_dbcastle);

	int zoneindex = CWarCastle::GetCurSubServerCastleZoneIndex();
	if (zoneindex < 0)
		return ;

	sprintf(g_buf, "SELECT a_tax_guild_index, a_tax_item, a_tax_produce, a_tax_wday FROM t_castle WHERE a_zone_index = %d", ZONE_MERAC);
	cmd.SetQuery(g_buf);

	if (cmd.Open() && cmd.MoveFirst())
	{
		int guildindex;
		LONGLONG taxItem;
		LONGLONG taxProduce;
		int wday;
		cmd.GetRec("a_tax_guild_index", guildindex);
		cmd.GetRec("a_tax_item", taxItem);
		cmd.GetRec("a_tax_produce", taxProduce);
		cmd.GetRec("a_tax_wday", wday);

		// 최근 저장 요일이 오늘이 아닐 경우에만
		struct tm tmCur = NOW();
		if (wday != tmCur.tm_wday)
		{
			// 헬퍼에서 최소 금액을 보장한다
			CNetMsg rmsg;
			HelperGuildStashSaveTaxReqMsg(rmsg, guildindex, ZONE_MERAC, taxItem, taxProduce);
			SEND_Q(rmsg, m_helper);
		}
	}
	else
	{
		GAMELOG << init("MERAC TAX DIVIDE FAIL")
				<< end;
	}
#endif

#ifdef DRATAN_CASTLE
	CDBCmd cmd2;
	cmd2.Init(&m_dbcastle);

	sprintf(g_buf, "SELECT a_tax_guild_index, a_tax_item, a_tax_produce, a_tax_wday FROM t_castle WHERE a_zone_index = %d", ZONE_DRATAN);
	cmd2.SetQuery(g_buf);

	if (cmd2.Open() && cmd2.MoveFirst())
	{
		int guildindex;
		LONGLONG taxItem;
		LONGLONG taxProduce;
		int wday;
		cmd2.GetRec("a_tax_guild_index", guildindex);
		cmd2.GetRec("a_tax_item", taxItem);
		cmd2.GetRec("a_tax_produce", taxProduce);
		cmd2.GetRec("a_tax_wday", wday);

		// 최근 저장 요일이 오늘이 아닐 경우에만
		struct tm tmCur = NOW();
		if (wday != tmCur.tm_wday)
		{
			// 헬퍼에서 최소 금액을 보장한다
			CNetMsg rmsg;
			HelperGuildStashSaveTaxReqMsg(rmsg, guildindex, ZONE_DRATAN, taxItem, taxProduce);
			SEND_Q(rmsg, m_helper);
		}
	}
	else
	{
		GAMELOG << init("DRATAN TAX DIVIDE FAIL")
				<< end;
	}
#endif // DRATAN_CASTLE
}

void CServer::ChangeTaxGuild()
{
	CDBCmd cmd;
	cmd.Init(&m_dbcastle);
	sprintf(g_buf, "UPDATE t_castle SET a_tax_item = 0, a_tax_produce = 0, a_tax_guild_index = a_owner_guild_index WHERE a_zone_index = %d AND a_tax_guild_index != a_owner_guild_index", CWarCastle::GetCurSubServerCastleZoneIndex());
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		GAMELOG << init("MARAC TAX GUILD RESET FAIL")
				<< "ZONE" << delim
				<< CWarCastle::GetCurSubServerCastleZoneIndex()
				<< end;
	}
	else
	{
		GAMELOG << init(" MARAC TAX GUILD RESET SUCCESS")
				<< "ZONE" << delim
				<< CWarCastle::GetCurSubServerCastleZoneIndex()
				<< end;
	}

#ifdef DRATAN_CASTLE
	CDBCmd cmd2;
	cmd2.Init(&m_dbcastle);
	sprintf(g_buf, "UPDATE t_castle SET a_tax_item = 0, a_tax_produce = 0, a_tax_guild_index = a_owner_guild_index WHERE a_zone_index = %d AND a_tax_guild_index != a_owner_guild_index", ZONE_DRATAN);
	cmd2.SetQuery(g_buf);
	if (!cmd2.Update())
	{
		GAMELOG << init("DRATAN TAX GUILD RESET FAIL")
				<< "ZONE" << delim
				<< CWarCastle::GetCurSubServerCastleZoneIndex()
				<< end;
	}
	else
	{
		GAMELOG << init("DRATAN TAX GUILD RESET SUCCESS")
				<< "ZONE" << delim
				<< CWarCastle::GetCurSubServerCastleZoneIndex()
				<< end;
	}
#endif // DRATAN_CASTLE
}
#endif // #ifdef ENABLE_WAR_CASTLE

int CServer::FindNearestZone(int zone, float x, float z, int* nearZone, int* nearZonePos)
{
	// x, z가 음수이면 x, z 무시해야 함
	// 기본 위치
	*nearZone = ZONE_START;
	*nearZonePos = 0;

	switch (zone)
	{
	case ZONE_PK_TOURNAMENT:
		*nearZone = ZONE_PK_TOURNAMENT;
		*nearZonePos = 0;
		break;

	case ZONE_STREIANA:
	case ZONE_SPRIT_CAVE :
	case ZONE_QUANIAN_CAVE	:
	case ZONE_GOLEM_CAVE :
		*nearZone = ZONE_STREIANA;
		*nearZonePos = 0;
		break;

	case ZONE_SINGLE_DUNGEON_TUTORIAL:	// 쥬노 2번에서 시작
		*nearZone = ZONE_START;
#ifdef LC_KOR
		*nearZonePos = 2;
#else
		*nearZonePos = 0;
#endif
		break;

	case ZONE_DUNGEON4:
	case ZONE_DUNGEON2:
	case ZONE_DRATAN:
	case ZONE_DRATAN_CASTLE_DUNGEON:
		*nearZone = ZONE_DRATAN;
		*nearZonePos = 0;
		break;

	case ZONE_MERAC:
	case ZONE_DUNGEON3:
	case ZONE_MISTY_CANYON:
	case ZONE_EXTREME_CUBE:
		*nearZone = ZONE_MERAC;
		*nearZonePos = 0;
		break;

	case ZONE_EGEHA:
	case ZONE_EGEHA_DUNGEON_1:
	case ZONE_EGEHA_DUNGEON_8:
	case ZONE_EGEHA_DUNGEON_9:
	case ZONE_EGEHA_DUNGEON_10:
	case ZONE_FLORAIM_CAVE :
		*nearZone = ZONE_EGEHA;
		*nearZonePos = 0;
		break;

#ifdef MONSTER_COMBO
	case ZONE_COMBO_DUNGEON:
		*nearZone = ZONE_COMBO_DUNGEON;
		*nearZonePos = 0;
		break;
#endif // MONSTER_COMBO

	case ZONE_START:	// 쥬노 시작
	case ZONE_DUNGEON1:
	case ZONE_SINGLE_DUNGEON1:
	case ZONE_SINGLE_DUNGEON2:
	case ZONE_GUILDROOM:
	case ZONE_SINGLE_DUNGEON3:
	case ZONE_SINGLE_DUNGEON4:
	case ZONE_OXQUIZROOM:
	case ZONE_EBONY_MINE:
	default:
		*nearZone = ZONE_START;
		*nearZonePos = 0;
		break;

	}

	return FindZone(*nearZone);
}

CParty* CServer::FindPartyByBossIndex(int nBossIndex)
{
	CParty partyFind(0, nBossIndex, "", NULL, 0, "");
	void* pos = m_listParty.FindData(&partyFind);
	if (pos == NULL)
		return NULL;
	else
		return m_listParty.GetData(pos);
}

CParty* CServer::FindPartyByMemberIndex(int nMemberIndex, bool bIncludeRequest)
{
	void* pos = m_listParty.GetHead();
	while (pos)
	{
		CParty* pParty = m_listParty.GetData(pos);
		if (pParty->FindMember(nMemberIndex) != -1)
			return pParty;
		if (bIncludeRequest && pParty->GetRequestIndex() == nMemberIndex)
			return pParty;
		pos = m_listParty.GetNext(pos);
	}
	return NULL;
}

#ifdef EXPEDITION_RAID
CExpedition* CServer::FindExpedByBossIndex( int nBossIndex)
{
	CExpedition ExpedFind(0, nBossIndex, "", 0, 0, NULL);
	void* pos = m_listExped.FindData(&ExpedFind);
	if (pos == NULL)
		return NULL;
	else
		return m_listExped.GetData(pos);
}

CExpedition* CServer::FindExpedByMemberIndex(int nMemberIndex, bool bIncludeRequest)
{
	void* pos = m_listExped.GetHead();
	while (pos)
	{
		CExpedition* pExped = m_listExped.GetData(pos);
		if (pExped->FindMemberListIndex(nMemberIndex) != -1)
			return pExped;
		if (bIncludeRequest && pExped->GetRequestIndex() == nMemberIndex)
			return pExped;
		pos = m_listExped.GetNext(pos);
	}
	return NULL;
}
#endif

#ifdef PARTY_MATCHING
CPartyMatchMember* CServer::FindPartyMatchMemberByCharIndex(int nCharIndex)
{
	CPartyMatchMember matchMemberFind(nCharIndex, "", 0, 0, 0, 0);
	void* pos = m_listPartyMatchMember.FindData(&matchMemberFind);
	if (pos == NULL)
		return NULL;
	else
		return m_listPartyMatchMember.GetData(pos);
}

CPartyMatchParty* CServer::FindPartyMatchPartyByBossIndex(int nBossIndex)
{
	CPartyMatchParty matchPartyFind(nBossIndex, "", 0, 0, 0, 0, false, "");
	void* pos = m_listPartyMatchParty.FindData(&matchPartyFind);
	if (pos == NULL)
		return NULL;
	else
		return m_listPartyMatchParty.GetData(pos);
}
#endif // PARTY_MATCHING

#ifdef ENABLE_OXQUIZ
bool CServer::LoadOXQuiz()
{
	CLCString sql(1024);
#ifdef LC_KOR
	sql.Format("SELECT a_index, a_question, a_answer FROM t_oxquiz WHERE a_country=%d ORDER BY a_index", OX_QUIZ_COUNTRY_CODE);
#else
	sql.Format("SELECT a_index, a_question, a_answer FROM t_oxquiz WHERE a_country=%d ORDER BY a_index", m_national);
#endif
	CDBCmd cmd;
	cmd.Init(&m_dbdata);
	cmd.SetQuery(sql);
	if (!cmd.Open())
		return false;

	while (cmd.MoveNext())
	{
		int nIndex;
		CLCString strQuestion(256);
		CLCString strAnswer(2);

		if (
			   !cmd.GetRec("a_index", nIndex)
			|| !cmd.GetRec("a_question", strQuestion)
			|| !cmd.GetRec("a_answer", strAnswer)
			)
			return false;

		COXQuizData* pQuiz = new COXQuizData(nIndex, strQuestion, (strcmp(strAnswer, "O") == 0) ? true : false);
		m_listOXQuiz.AddToTail(pQuiz);
	}

	return true;
}
#endif // ENABLE_OXQUIZ

#ifdef EVENT_OPEN_ADULT_SERVER
bool CItemUpgradeList::Load()
{
	CLCString sql(1024);
	sql.Format("SELECT * FROM t_event_adultserver_item ORDER BY a_index");

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbdata);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;

	m_nCount = cmd.m_nrecords;
	m_pItemUpgradeData = new CItemUpgradeData[m_nCount];

	int i = 0;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_index", m_pItemUpgradeData[i].m_item_idx);
		cmd.GetRec("a_exchage_index", m_pItemUpgradeData[i].m_exchange_item_idx);
		i++;
	}

	if(i != m_nCount)
		return false;

	return true;
}
#endif // EVENT_OPEN_ADULT_SERVER

#ifdef MESSENGER_NEW
int CServer::addChatGroup(int& makeCharIndex, int& chatIndex, CLCString charName)
{
	CChatGroup* chatGroup = NULL;

	void* pos = this->FindChatGroup(makeCharIndex, chatIndex);

	if( pos )
	{	// 세션이 만들어진 상태
		chatGroup = gserver.m_chatList.GetData(pos);
		pos = chatGroup->m_charNameList.FindData(charName);

		if( !pos )
		{
#ifdef MSG_VER2
			if( chatGroup->CheckSameName(charName) == false )
			{	// 중복 체크
				return 0;
			}
#endif // MSG_VER2
			chatGroup->m_charNameList.AddToTail(charName);
#ifdef MSG_VER2
			if( chatGroup->m_charNameList.GetCount() == 1 )
			{	// 처음 생성시
				return -1;
			}
#endif // MSG_VER2
			return 1;
		}
	}
	else
	{	
		chatIndex = this->GetMaxChatIndexPerPC(makeCharIndex)+1;
		chatGroup = new CChatGroup(makeCharIndex, chatIndex );
		chatGroup->m_charNameList.AddToTail(charName);
		this->m_chatList.AddToHead(chatGroup);

		return 0;
	}

	return -1;
}

void CServer::discCharChatGroup(CLCString charName)
{
	void *pos = this->m_chatList.GetHead();
	int count = this->m_chatList.GetCount();

	CChatGroup* chatGroup = NULL;

	CNetMsg rmsg;
	for(int i = 0; i < count; i++)
	{
		chatGroup = this->m_chatList.GetData(pos);

		if( chatGroup )
		{
			CNetMsg rmsg;
			MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_DEL, chatGroup->GetMakeCharIndex(), chatGroup->GetChatIndex(), 0, charName );
			SEND_Q(rmsg, gserver.m_messenger);
		}

		pos = this->m_chatList.GetNext(pos);
	}
}

void CServer::delChatGroup(int makeCharIndex, int chatIndex, CLCString charName)
{
	void* pos = this->FindChatGroup(makeCharIndex, chatIndex);
	void* posName = NULL;

	CChatGroup* chatGroup = NULL;
	if( pos )
	{		
		chatGroup = gserver.m_chatList.GetData(pos);

		int count = chatGroup->m_charNameList.GetCount();
		posName = chatGroup->m_charNameList.GetHead();

		CLCString name(MAX_CHAR_NAME_LENGTH+1);
		for(int i = 0; i < count; i++)
		{
			name = chatGroup->m_charNameList.GetData(posName);

			if( strcmp(name, charName) == 0 )
				break;
			posName = chatGroup->m_charNameList.GetNext(posName);
		}

		if( posName )
		{
			chatGroup->m_charNameList.Remove(posName);
			if ( chatGroup->m_charNameList.GetCount() < 1 )
			{	// 방원이 존재
				gserver.m_chatList.Remove(pos);
			}
		}		
	}
}

void CServer::SendChatGroup(MSG_EX_MESSENGER_TYPE subtype, int makeCharIndex, int chatIndex, int chatColor, CLCString charName, CLCString chat)
{
	void* pos = this->FindChatGroup(makeCharIndex, chatIndex);
	if( pos )
	{
		CChatGroup* chatGroup = gserver.m_chatList.GetData(pos);
		if( chatGroup )
		{
			chatGroup->SendGroup(subtype, chatColor, charName, chat);
		}
	}
}

void* CServer::FindChatGroup(int makeCharIndex, int chatIndex)
{
	int count, i;
	void* pos;

	count = gserver.m_chatList.GetCount();
	pos = gserver.m_chatList.GetHead();

	CChatGroup* chatGroup = NULL;
	for(i = 0; i < count; i++)
	{
		chatGroup = gserver.m_chatList.GetData(pos);

		if( chatGroup->GetMakeCharIndex() == makeCharIndex && chatGroup->GetChatIndex() == chatIndex )
			return pos;
		pos = gserver.m_chatList.GetNext(pos);
	}

	return NULL;
}

int CServer::GetMaxChatIndexPerPC(int makeCharIndex)
{
	int count, i, max;
	void* pos;

	count = gserver.m_chatList.GetCount();
	pos = gserver.m_chatList.GetHead();

	max = 0;
	CChatGroup* chatGroup = NULL;
	for(i = 0; i < count; i++)
	{
		chatGroup = gserver.m_chatList.GetData(pos);

		if( chatGroup->GetMakeCharIndex() == makeCharIndex && chatGroup->GetChatIndex() > max )
			max = chatGroup->GetChatIndex();
		pos = gserver.m_chatList.GetNext(pos);
	}

	return max;
}

#ifdef MSG_VER2
bool CChatGroup::CheckSameName(CLCString charName)
{
	// 같은 이름이 있는지 검사
	int count = m_charNameList.GetCount();
	CLCString name(MAX_CHAR_NAME_LENGTH+1);
	void * posName = m_charNameList.GetHead();
	for(int i = 0; i < count; i++)
	{
		name = m_charNameList.GetData(posName);
		if( strcmp(name, charName) == 0 )
		{
			return false;
		}

		posName = m_charNameList.GetNext(posName);
	}

	return true;
}
#endif //MSG_VER2

void CChatGroup::SendGroup(MSG_EX_MESSENGER_TYPE subtype, int chatColor, CLCString charName, CLCString chat)
{
	CNetMsg rmsg;
	rmsg.Init(MSG_EXTEND);
	rmsg << MSG_EX_MESSENGER
		 << (unsigned char) subtype
		 << this->GetMakeCharIndex()
		 << this->GetChatIndex();

	if( subtype == MSG_EX_MESSENGER_CHAT)
		rmsg << chatColor;
	rmsg << charName
		 << chat;

	CNetMsg lmsg;
	if( subtype == MSG_EX_MESSENGER_INVITE )
	{
		lmsg.Init(MSG_EXTEND);
		lmsg << MSG_EX_MESSENGER
			 << (unsigned char) MSG_EX_MESSENGER_CHARLIST
			 << this->GetMakeCharIndex()
			 << this->GetChatIndex()
			 << this->m_charNameList.GetCount();
	}

	void* pos = NULL;
	int count, i;

	pos = this->m_charNameList.GetHead();
	count = this->m_charNameList.GetCount();

	CLCString SendcharName(MAX_CHAR_NAME_LENGTH + 1);
	for(i = 0; i < count; i++)
	{
		SendcharName = this->m_charNameList.GetData(pos);

		if( strcmp( SendcharName, "" ) != 0 )
		{
			CPC* pc = gserver.m_playerList.Find( SendcharName, true );
			if( pc )
			{
				if( subtype == MSG_EX_MESSENGER_INVITE )
					lmsg << SendcharName;

				SEND_Q(rmsg, pc->m_desc);
			}
		}
		pos = this->m_charNameList.GetNext(pos);
	}

	if( subtype == MSG_EX_MESSENGER_INVITE)
	{
		CPC* pc = gserver.m_playerList.Find( charName, true );
		if( pc )
			SEND_Q(lmsg, pc->m_desc);
	}
}

CChatGroup::CChatGroup(int makeCharIndex, int chatIndex) :  m_charNameList(NULL)
{
	m_makeCharIndex = makeCharIndex;
	m_chatIndex = chatIndex;
}

CChatGroup::~CChatGroup()
{
}

#endif // MESSENGER_NEW

void CServer::CharPrePlay(CDescriptor* d)
{
	d->m_pChar->m_bPlaying = true;

	d->m_pChar->ResetPlayerState(~0);
	d->m_pChar->m_regGuild = 0;

	CNetMsg msg;

	d->m_pChar->m_invenNormal.CheckCompositeValidation();

//#if defined(FORCE_START_ZONE) && !defined(LC_KOR)
	
	// 죽은 상태면 부활
	if (DEAD(d->m_pChar))
	{
		d->m_pChar->m_hp= d->m_pChar->m_dbHP / 2;
		
		// MP는 반이상 있으면 그대로..
		if (d->m_pChar->m_mp < d->m_pChar->m_dbMP / 2)
			d->m_pChar->m_mp = d->m_pChar->m_dbMP / 2;
	}	
//#endif // #if defined(FORCE_START_ZONE) && !defined(LC_KOR)

	CArea* area = d->m_pChar->m_pArea;
	int i;

	// 하드코딩 : 튜토리얼 퀘스트 수행중인지 검사 후 여유 영역이 있는가 검사
#ifdef QUEST_DATA_EXTEND
	CQuest* pQuest;
	CQuest* pQuestNext = d->m_pChar->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = d->m_pChar->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		if (pQuest->GetQuestType0() == QTYPE_KIND_TUTORIAL)
		{
			int tmp = gserver.FindZone(ZONE_SINGLE_DUNGEON_TUTORIAL);
			if (tmp == -1)
			{
				d->m_bclosed = true;
				return;
			}

			d->m_pChar->m_pZone = gserver.m_zones + tmp;
			int idx = d->m_pChar->m_pZone->SetEnableArea();
			
			// 빈 영억 없음 : 접속을 끊는다
			if (idx == -1)
			{
				d->m_bclosed = true;
				return;
			}

			d->m_pChar->m_pArea = d->m_pChar->m_pZone->m_area + idx;
			area = d->m_pChar->m_pArea;
			
			GET_YLAYER(d->m_pChar)	= d->m_pChar->m_pZone->m_zonePos[0][0];
			GET_R(d->m_pChar)		= 0.0f;
			GET_X(d->m_pChar) = d->m_pChar->m_pZone->m_zonePos[0][1] / 2;
			GET_Z(d->m_pChar) = d->m_pChar->m_pZone->m_zonePos[0][2] / 2;
			break;
		}
	}
#else // QUEST_DATA_EXTEND
	for (i=0; i < QUEST_MAX_PC; i++)
	{
		if (!d->m_pChar->m_questList.m_list[i])
			continue;

		if (d->m_pChar->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_TUTORIAL && d->m_pChar->m_questList.m_bQuest[i])
		{
			int tmp = gserver.FindZone(ZONE_SINGLE_DUNGEON_TUTORIAL);
			if (tmp == -1)
			{
				d->m_bclosed = true;
				return;
			}

			d->m_pChar->m_pZone = gserver.m_zones + tmp;
			int idx = d->m_pChar->m_pZone->SetEnableArea();
			
			// 빈 영억 없음 : 접속을 끊는다
			if (idx == -1)
			{
				d->m_bclosed = true;
				return;
			}

			d->m_pChar->m_pArea = d->m_pChar->m_pZone->m_area + idx;
			area = d->m_pChar->m_pArea;
			
			GET_YLAYER(d->m_pChar)	= d->m_pChar->m_pZone->m_zonePos[0][0];
			GET_R(d->m_pChar)		= 0.0f;
			GET_X(d->m_pChar) = d->m_pChar->m_pZone->m_zonePos[0][1] / 2;
			GET_Z(d->m_pChar) = d->m_pChar->m_pZone->m_zonePos[0][2] / 2;
			break;
		}
	}
#endif // QUEST_DATA_EXTEND

	// 길드 정보 세팅
	bool bSendGuildMsg = false;
	CGuildMember* member = m_guildlist.findmember(d->m_pChar->m_index);
	if (member && member->guild())
	{
		if (!d->m_pChar->m_guildInfo)
			bSendGuildMsg = true;
		d->m_pChar->m_guildInfo = member;
	}
	
#ifdef ENABLE_WAR_CASTLE
	// 공성 정보 반영
	CWarCastle::CheckJoinAll(d->m_pChar);
#endif // #ifdef ENABLE_WAR_CASTLE

#ifdef ENABLE_WAR_CASTLE
	// 공성 참여 유저가 공성 지역으로 이동하면 공성 리젠포인트로 이동 묻기
	int posPromptMove = -1;
	int zonePromptMove = -1;
	int castlezone = CWarCastle::GetCurSubServerCastleZoneIndex();

	// 현재 서버의 공성 정보
	CWarCastle* castle = CWarCastle::GetCastleObject(castlezone);
	if (d->m_pChar->m_bLoadChar == false
		&& castle != NULL
		&& d->m_pChar->GetJoinFlag(castlezone) != WCJF_NONE)
	{
		time_t ct;
		time(&ct);
		int nt = castle->GetNextWarTime();
		if (ct + 10 * 60 >= nt || castle->GetState() != WCSF_NORMAL)
		{
			zonePromptMove = castlezone;
			posPromptMove = castle->GetRegenPoint(d->m_pChar->GetJoinFlag(castlezone), d->m_pChar);
		}
	}
#endif // ENABLE_WAR_CASTLE

#ifdef DRATAN_CASTLE
	// 공성 참여 유저가 공성 지역으로 이동하면 공성 리젠포인트로 이동 묻기
	int posPromptMove_Dratan = -1;
	int zonePromptMove_Dratan = -1;
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	// 현재 서버의 공성 정보
	if (d->m_pChar->m_bLoadChar == false
		&& pCastle != NULL 
		&& d->m_pChar->GetJoinFlag(pCastle->GetZoneIndex()) != WCJF_NONE
		&& gserver.m_subno == pCastle->GetCurSubServerCastleZoneIndex())
	{
		time_t ct;
		time(&ct);
		int nt = pCastle->GetNextWarTime();
		if (ct + 10 * 60 >= nt 
			|| pCastle->GetState() != WCSF_NORMAL)
		{
			zonePromptMove_Dratan = pCastle->GetZoneIndex();
			posPromptMove_Dratan = pCastle->GetRegenPoint(d->m_pChar->GetJoinFlag(pCastle->GetZoneIndex()), d->m_pChar);
		}
	}
#endif // DRATAN_CASTLE

	m_playerList.Add(d->m_pChar);

#ifdef RANDOM_ZUNO
	if(NULL != d->m_pChar->m_pZone
		&& ZONE_START == d->m_pChar->m_pZone->m_index
		&& false == d->m_pChar->m_bLoadChar)
	{
		static int nResponse[5][4] =
		{
			{1243, 1253, 951, 961},
			{1139, 1149, 1083, 1088},
			{1194, 1204, 1032, 1042},
			{1084, 1094, 1045, 1055},
			{1131, 1141, 906, 916},
		};

		int idx = GetRandom(0, 4);
		d->m_pChar->m_pos.m_x = GetRandom(nResponse[idx][0], nResponse[idx][1]);
		d->m_pChar->m_pos.m_z = GetRandom(nResponse[idx][2], nResponse[idx][3]);
	}
#endif // RANDOM_ZUNO
	
	int cx, cz;
	area->PointToCellNum(GET_X(d->m_pChar), GET_Z(d->m_pChar), &cx, &cz);
	area->CharToCell(d->m_pChar, GET_YLAYER(d->m_pChar), cx, cz);

	// 캐릭터에게 처음 시작하는 위치를 알림.
	AtMsg(msg, d->m_pChar);
	SEND_Q(msg, d);
	
	
	// 시간 알리기
	EnvTimeMsg(msg);
	SEND_Q(msg, d);
	
	// 길드 정보
	if (d->m_pChar->m_guildInfo)
	{
		if (bSendGuildMsg)
		{
			d->m_pChar->m_guildInfo->online(1);
			d->m_pChar->m_guildInfo->SetPC(d->m_pChar);
			HelperGuildOnline(msg, d->m_pChar->m_guildInfo);
#ifdef NEW_GUILD
			msg << d->m_pChar->m_pArea->m_zone->m_index;
#endif // NEW_GUILD
			if (IS_RUNNING_HELPER) SEND_Q(msg, m_helper);

			GAMELOG << init("GuildMember Class Size") << (int)sizeof( CGuildMember ) << end;
		}

		GuildInfoMsg(msg, d->m_pChar);
		SEND_Q(msg, d);
		GuildListMsg(msg, d->m_pChar);
		SEND_Q(msg, d);
	}


	
	// STAT POINT
	StatPointRemainMsg(msg, d->m_pChar);
	SEND_Q(msg, d);

	// 하드코딩 : 복주머니 이벤트 탭에서 노말 탭으로
	int r, c;
	if (d->m_pChar->m_invenEvent.FindItem(&r, &c, 507, 0, 0))
	{
		CItem* item = d->m_pChar->m_invenEvent.GetItem(r, c);
		if (item)
		{
			RemoveFromInventory(d->m_pChar, item, false, true);
			if (!AddToInventory(d->m_pChar, item, false, false))
			{
				item = d->m_pChar->m_pArea->DropItem(item, d->m_pChar);
				if (item)
				{
					CNetMsg dropMsg;
					item->m_preferenceIndex = d->m_pChar->m_index;
					ItemDropMsg(dropMsg, d->m_pChar, item);
					d->m_pChar->m_pArea->SendToCell(dropMsg, d->m_pChar, true);
					SEND_Q(dropMsg, d->m_pChar->m_desc);
				}
			}
		}
	}

	// 하드코딩 : 0짜리 제련석 40레벨로 바꿈
	if (d->m_pChar->m_invenNormal.FindItem(&r, &c, 84, 0, 0))
	{
		CItem* item = d->m_pChar->m_invenNormal.GetItem(r, c);
		if (item)
		{
			item->m_flag = 40;
			CNetMsg rmsg;
			ItemUpdateMsg(rmsg, item, 0);
			SEND_Q(rmsg, d->m_pChar->m_desc);
		}
	}

#ifdef ENABLE_WAR_CASTLE
	// 성주 무기 빼앗거나 지급
	bool bProcLordItem = false;
	if (d->m_pChar->m_guildInfo && d->m_pChar->m_guildInfo->guild())
	{
		int ownZoneCount;
		int* ownZoneIndex = CWarCastle::GetOwnCastle(d->m_pChar->m_guildInfo->guild()->index(), &ownZoneCount);
		if (ownZoneIndex)
		{
			time_t curtime;
			time(&curtime);

			int i;
			for (i = 0; i < ownZoneCount; i++)
			{
				CWarCastle* castle = CWarCastle::GetCastleObject(ownZoneIndex[i]);
				if (castle)
				{
					// 060116 : BS : BEGIN : 공성 시작해도 칼 회수 안하게
//					int nexttime = castle->GetNextWarTime();
					if (castle->GetOwnerCharIndex() == d->m_pChar->m_index)
					{
						// 060116 : BS : BEGIN : 공성 시작해도 칼 회수 안하게
//						// 공성 시작 5분전에서 공성 진행중이면 회수
//						if (castle->GetState() != WCSF_NORMAL || curtime + 5 * 60 >= nexttime)
//						{
//							castle->TakeLordItem(d->m_pChar);
//						}
//
//						// 일반 상태에서 없으면 지급
//						else
//						{
							castle->GiveLordItem(d->m_pChar);
//						}
						// 060116 : BS : END : 공성 시작해도 칼 회수 안하게
						bProcLordItem = true;
					}
				}
			}
			delete [] ownZoneIndex;
		}
	}
	// 성주가 아니라서 아무 처리 안했으면 성주 전용 아이템 찾아서 지우기
	if (!bProcLordItem)
	{
		int i;
		for (i = 0; i < gserver.m_itemProtoList.m_nCount; i++)
		{
			if (gserver.m_itemProtoList.m_protoItems[i].m_flag & ITEM_FLAG_LORD)
			{
				int r, c;
				while (d->m_pChar->m_invenNormal.FindItem(&r, &c, gserver.m_itemProtoList.m_protoItems[i].m_index, 0, 0))
				{
					CItem* item = d->m_pChar->m_invenNormal.GetItem(r, c);
					if (item->m_wearPos != WEARING_NONE)
					{
						ItemWearMsg(msg, item->m_wearPos, NULL, item);
						SEND_Q(msg, d->m_pChar->m_desc);
						if (item->m_wearPos >= WEARING_SHOW_START && item->m_wearPos <= WEARING_SHOW_END && d->m_pChar->m_pArea)
						{
							WearingMsg(msg, d->m_pChar, item->m_wearPos, -1, 0);
							d->m_pChar->m_pArea->SendToCell(msg, d->m_pChar, true);
						}
						d->m_pChar->m_wearing[(int)item->m_wearPos] = NULL;
						item->m_wearPos = WEARING_NONE;
					}

					ItemDeleteMsg(msg, item);
					SEND_Q(msg, d->m_pChar->m_desc);
					RemoveFromInventory(d->m_pChar, item, true, true);
				}
			}
		}
	}
#endif // #ifdef ENABLE_WAR_CASTLE

#ifdef ENABLE_PET
	CPet* pet = d->m_pChar->m_petList;
	while (pet)
	{
#ifdef IMSO
		if (!(d->m_pChar->m_pZone->m_bCanSummonPet))
		{
			ItemWearMsg(msg, WEARING_PET, NULL, NULL);
			do_ItemWear(d->m_pChar, msg);
			ExPetStatusMsg(msg, pet);
			SEND_Q(msg, d);
		}
		else
#else
		pet->ResetSummonMountFlag();

		ExPetStatusMsg(msg, pet);
		SEND_Q(msg, d);
		ExPetSkillListMsg(msg, pet);
		SEND_Q(msg, d);
#endif
		{
			// 소환중인 펫이 마운트 타입이 아니면 Appear 시키기
			if (pet->IsWearing())
			{
#ifdef IMSO
				pet->ResetSummonMountFlag();
				ExPetStatusMsg(msg, pet);
				SEND_Q(msg, d);
				ExPetSkillListMsg(msg, pet);
				SEND_Q(msg, d);
#endif 

				if (pet->IsMountType() && d->m_pChar->m_pZone->m_bCanMountPet)
					pet->Mount(true);
				else if (!pet->IsMountType() && d->m_pChar->m_pZone->m_bCanSummonPet)
					pet->Appear(false);
				else
				{
					//d->m_pChar->m_wearing[WEARING_PET]->m_wearPos = WEARING_NONE;
					//d->m_pChar->m_wearing[WEARING_PET] = NULL;
				}
			}
		}
		pet = pet->m_nextPet;
	}
#endif // #ifdef ENABLE_PET
#ifdef ATTACK_PET
	CAPet* apet = d->m_pChar->m_pApetlist;
	while ( apet )
	{
		// Pet 상태 MSG
		if( apet->IsWearing() )
		{
			apet->Appear(false);
			CNetMsg rmsg;
			ExAPetStatusMsg(msg, apet);
			SEND_Q(msg, d);
			ExAPetFuntionMsg(msg, MSG_SUB_SKILLLIST, apet, 0 );
			SEND_Q(msg, d);

#ifdef APET_AI
			APetAIOnOffMsg(rmsg, apet, MSG_APET_ERROR_OK);
			SEND_Q(rmsg, apet->GetOwner()->m_desc);
			
			APetAIListMsg(rmsg, apet);
			SEND_Q(rmsg, apet->GetOwner()->m_desc);
#endif
		}
		apet = apet->m_pNextPet;
	}
#endif //ATTACK_PET

	// 골든볼 이벤트 응모 카드 회수 : 이벤트 진행 중이 아니면
	if (m_clGoldenBall.GetStatus() == GOLDENBALL_STATUS_NOTHING)
	{
		int nRowCard, nColCard;
		if (d->m_pChar->m_invenEvent.FindItem(&nRowCard, &nColCard, GOLDENBALL_CARD_INDEX, -1, -1))
		{
			CItem* pItemCard = d->m_pChar->m_invenEvent.GetItem(nRowCard, nColCard);
			if (pItemCard)
			{
				GAMELOG << init("GOLDEN BALL EVENT RECALL CARD", d->m_pChar)
						<< itemlog(pItemCard)
						<< end;
				RemoveFromInventory(d->m_pChar, pItemCard, true, true);
			}
		}
	}

	d->m_pChar->ResetPlayerState(PLAYER_STATE_SUPPORTER);
	for (i = WEARING_ACCESSORY1; i <= WEARING_ACCESSORY3; i++)
	{
		if (d->m_pChar->m_wearing[i])
		{
			// 서포터 아이템
			if (d->m_pChar->m_wearing[i]->m_itemProto->m_index == 1912)
				d->m_pChar->SetPlayerState(PLAYER_STATE_SUPPORTER);
		}
	}

#ifdef EVENT_JPN_2007_NEWSERVER
	if (d->m_pChar->m_nEventJPN2007NewServerGift > 0)
	{
		LONGLONG nMoney = 0;
		if (d->m_pChar->m_moneyItem)
			nMoney = d->m_pChar->m_moneyItem->Count();
		LONGLONG nAddMoney = 0;

		if (d->m_pChar->m_nEventJPN2007NewServerGift <= EVENT_JPN_2007_NEWSERVER_NAS100K)
			nAddMoney = 100000;
		else if (d->m_pChar->m_nEventJPN2007NewServerGift <= EVENT_JPN_2007_NEWSERVER_NAS10K)
			nAddMoney = 10000;
		if (nAddMoney > 0)
		{
			GAMELOG << init("EVENT NEWSERVER 2007", d->m_pChar)
					<< "RANK" << delim
					<< d->m_pChar->m_nEventJPN2007NewServerGift << delim
					<< "BEFORE" << delim
					<< nMoney << delim;
			d->m_pChar->AddMoney(nAddMoney);
			if (d->m_pChar->m_moneyItem)
				nMoney = d->m_pChar->m_moneyItem->Count();
			GAMELOG << "AFTER" << delim
					<< nMoney
					<< end;
		}
		d->m_pChar->m_nEventJPN2007NewServerGift = 0;
	}
#endif // EVENT_JPN_2007_NEWSERVER
 
#if defined(EVENT_NOM_MOVIE) && !defined( EVENT_NOM_MOVIE_REWARD )
	if (d->m_pChar->m_level >= 20)
	{
		if (IS_RUNNING_CONN)
		{
			CNetMsg rmsg;
			ConnEventNomMsg(rmsg, MSG_CONN_EVENT_NOM_REQ, d->m_index, d->m_pChar->m_index);
			SEND_Q(rmsg, gserver.m_connector);
		}
	}
#endif

	// inven
	d->m_pChar->SendInventory(0);

	// status
	d->m_pChar->SendStatus();
	
	// quest
	QuestPCListMsg(msg, d->m_pChar);
	SEND_Q(msg, d);


	QuestCompleteListMsg(msg, d->m_pChar);
	SEND_Q(msg, d);

	QuestAbandonListMsg(msg, d->m_pChar);
	SEND_Q(msg, d);


	// quest complete check
	d->m_pChar->m_questList.CheckComplete(d->m_pChar);
	
	// skill
	SkillListMsg(msg, d->m_pChar);
	SEND_Q(msg, d);

	// sskill
	SSkillListMsg(msg, d->m_pChar);
	SEND_Q(msg, d);
	
	// Quick Slot MSG
	d->m_pChar->SendQuickSlot();
	
	// mempos
	MemPosListMsg(msg, d->m_pChar);
	SEND_Q(msg, d);

#ifdef PRIMIUM_MEMORYBOOK
	// mempos
	MemPosPlusListMsg(msg, d->m_pChar);
	SEND_Q(msg, d);
#endif	// PRIMIUM_MEMORYBOOK
	
	// assist
	AssistListMsg(msg, d->m_pChar);
	SEND_Q(msg, d);

	// 사제시스템
	TeachInfoMsg(msg, d->m_pChar);
	SEND_Q(msg, d);


	//0627
	if((d->m_userFlag & NOVICE) && d->m_pChar->m_teachType == MSG_TEACH_NO_TYPE)
	{
		//모든 사제에게 신규유저가 접속 했음을 알린다.
		NoviceNotifyMsg(msg, d->m_pChar->GetName() );

		m_playerList.SendToAllTeacher(msg);
		d->m_userFlag = d->m_userFlag &~ NOVICE;
	}
	
	// 길드 정보
	if (d->m_pChar->m_guildInfo)
	{
		// 길드전 일때 status msg 전송
		CGuild* g = d->m_pChar->m_guildInfo->guild();
		if (g && g->battleState() != GUILD_BATTLE_STATE_PEACE)
		{
			CGuild* g2 = gserver.m_guildlist.findguild(g->battleIndex());
			if (g2)
			{
				if (g->battleState() == GUILD_BATTLE_STATE_PRIZE)
				{
					if (g->boss() && g->boss()->charindex() == d->m_pChar->m_index)
					{
						bool bHaveMoney = false;
						CNetMsg itemmsg;

						if (d->m_pChar->m_moneyItem)
							bHaveMoney = true;
						
						d->m_pChar->AddMoney(g->battlePrize());
						
						if (bHaveMoney)
							ItemUpdateMsg(itemmsg, d->m_pChar->m_moneyItem, g->battlePrize());
						else
							ItemAddMsg(itemmsg, d->m_pChar->m_moneyItem);
						
						SEND_Q(itemmsg, d->m_pChar->m_desc);
						
						if (IS_RUNNING_HELPER)
						{
							HelperGuildBattlePeaceReqMsg(msg, g);
							SEND_Q(msg, gserver.m_helper);
						}
					}
				}
				else
				{
					GuildBattleStatusMsg(msg, g->index(), g->name(), g->killCount(), g2->index(), g2->name(), g2->killCount(), g->battleTime(), g->battleZone());
					SEND_Q(msg, d);
				}
			}
		}
	}

#ifdef REWARD_IDC2007
	if(IS_RUNNING_HELPER)
	{
		HelerRewardIDC2007Msg(msg, d->m_index);
		SEND_Q(msg, gserver.m_helper);
	}

#endif // REWARD_IDC2007

#ifdef NOTICE_EVENT
//0704  이벤트 공지 날리기.
	for(i =0; i< MAX_NOTICE; i++)
	{
		if(d->m_notice[i] != 0)
		{
			NoticeInfoMsg(msg, d->m_notice[i]);
			SEND_Q(msg, d);

			d->m_notice[i] = 0; 
		}
	} 
#endif

#ifdef EVENT_SEARCHFRIEND
	//휴면 이벤트 부분으로 등록보낸 케릭들의 리스트를 올림.
	if((d->m_pChar->m_bEventSearchFriendSelect == false)) 
	{
		if (d->m_pChar->m_nEventSearchFriendListCount <= 20)
		{
			EventDormantSearchFriendSelectMsg(msg, d->m_pChar->m_nEventSearchFriendListCount, d->m_pChar->m_nEventSearchFriendIndex
					,d->m_pChar->m_nEventSearchFriendNick, 0, d->m_pChar->m_nEventSearchFriendListCount );
			SEND_Q(msg, d);
		}
		else
		{
			EventDormantSearchFriendSelectMsg(msg, 20, d->m_pChar->m_nEventSearchFriendIndex
					,d->m_pChar->m_nEventSearchFriendNick, 20, d->m_pChar->m_nEventSearchFriendListCount );
			SEND_Q(msg, d);
			if (d->m_pChar->m_nEventSearchFriendListCount <= 40)
			{						
				EventDormantSearchFriendSelectMsg(msg, d->m_pChar->m_nEventSearchFriendListCount - 20, d->m_pChar->m_nEventSearchFriendIndex + 20
						,d->m_pChar->m_nEventSearchFriendNick + 20, 20, d->m_pChar->m_nEventSearchFriendListCount );
				SEND_Q(msg, d);
			}
			else
			{
				EventDormantSearchFriendSelectMsg(msg, 20, d->m_pChar->m_nEventSearchFriendIndex + 20
						,d->m_pChar->m_nEventSearchFriendNick + 20, 20, d->m_pChar->m_nEventSearchFriendListCount );
				SEND_Q(msg, d);
				EventDormantSearchFriendSelectMsg(msg, d->m_pChar->m_nEventSearchFriendListCount - 40, d->m_pChar->m_nEventSearchFriendIndex + 40
						,d->m_pChar->m_nEventSearchFriendNick + 40, 40, d->m_pChar->m_nEventSearchFriendListCount );
				SEND_Q(msg, d);
			}
		}
	}
#endif // #ifdef EVENT_SEARCHFRIEND


	// appear
		AppearMsg(msg, d->m_pChar, true);
		area->SendToCell(msg, d->m_pChar);
		
		// 방 주위의 캐릭터들의 정보를 보내줌...
		area->SendCellInfo(d->m_pChar, false);
		
		// GO_ZONE 효과 메시지 전송
		EffectEtcMsg(msg, d->m_pChar, MSG_EFFECT_ETC_GOZONE);
		area->SendToCell(msg, d->m_pChar, true);
	
#ifdef ENABLE_MESSENGER
// 여기에 메신저 초기데이타 보낸다..
#ifdef MESSENGER_NEW
	//MSG_EX_MESSENGER_GROUP_LIST, // 그룹 리스트 보내기 : count(n) gIndex(n) gName(str)
	if( d->m_pChar->m_Friend)
	{
		msg.Init(MSG_EXTEND);
		msg << MSG_EX_MESSENGER
			<< (unsigned char) MSG_EX_MESSENGER_GROUP_LIST
			<< d->m_pChar->m_Friend->GetGroupCount();

		CLCString gIndexList(255+1);
		CLCString gNameList(255+1);

		int gIndex = 0;

		d->m_pChar->m_Friend->GetGroupIndexString(gIndexList, gNameList);

		const char* pgName = (const char*) gNameList;
		const char* pgIndex = (const char*) gIndexList;

		while(*pgIndex && *pgName)
		{
			pgIndex = AnyOneArg(pgIndex, g_buf);
			gIndex = atoi(g_buf); g_buf[0] = '\0';
			pgName = AnyOneArg(pgName, g_buf);

			msg << gIndex
				<< g_buf;

			g_buf[0] = '\0';
		}

		SEND_Q(msg, d);

	}
		
#endif
	FriendListMsg(msg, d);

	if(d->m_pChar->m_Friend)
	{
		d->m_pChar->m_nCondition = 1;
		HelperFriendSetConditionMsg(msg,d->m_pChar->m_index, d->m_pChar->m_nCondition, -1, d->m_pChar);				
		if (IS_RUNNING_HELPER) SEND_Q(msg, m_helper);
	}
#ifdef MESSENGER_NEW
	// 블럭리스트 보내기
	BlockPCListMsg(msg, d);
#endif
#endif // #ifdef ENABLE_MESSENGER

#ifdef ENABLE_WAR_CASTLE
	// 신청 가능 시간이면 확정 메시지를
	// 공성전 진행 중이면 진행 메시지를 보낸다
	///=== 메세지 막기 kjtest
#if defined (LC_JPN) || defined (LC_HBK)
	if (gserver.m_subno == WAR_CASTLE_SUBNUMBER_MERAC)
#endif
	{
		castle = CWarCastle::GetCastleObject(CWarCastle::GetCurSubServerCastleZoneIndex());
		if (castle)
		{
			if (castle->GetState() == WCSF_NORMAL)
			{
				if (castle->IsJoinTime())
				{
#ifdef TLD_WAR_TEST
					if( m_warnotice )
#endif
					{
						struct tm nextWarTime;
						castle->GetNextWarTime(&nextWarTime, true);
						GuildWarNoticeTimeMsg(msg, ZONE_MERAC, nextWarTime.tm_mon, nextWarTime.tm_mday, nextWarTime.tm_hour, nextWarTime.tm_min);					
						SEND_Q(msg, d);
					}
				}
			}
			else
			{
				GuildWarCastleStateMsg(msg, ZONE_MERAC, d->m_pChar, castle);
				SEND_Q(msg, d);
			}
		}
		
		// 이동 묻기
		if (zonePromptMove != -1)
		{
			WarpPromptMsg(msg, zonePromptMove, posPromptMove);		
			SEND_Q(msg, d);
		}
	}
#endif // ENABLE_WAR_CASTLE

#ifdef DRATAN_CASTLE
//#ifdef KJTEST
	// 신청 가능 시간이면 확정 메시지를
	// 공성전 진행 중이면 진행 메시지를 보낸다
#if defined (LC_JPN) || defined (LC_HBK)
	if (gserver.m_subno == WAR_CASTLE_SUBNUMBER_MERAC)
#endif
	{
		pCastle = CDratanCastle::CreateInstance();
		if (pCastle)
		{
			if (pCastle->GetState() == WCSF_NORMAL)
			{
				if (pCastle->IsJoinTime())
				{
					struct tm nextWarTime;
					pCastle->GetNextWarTime(&nextWarTime, true);
					GuildWarNoticeTimeMsg(msg, ZONE_DRATAN, nextWarTime.tm_mon, nextWarTime.tm_mday, nextWarTime.tm_hour, nextWarTime.tm_min);
					SEND_Q(msg, d);
				}
			}
			else
			{
				GuildWarCastleStateMsg(msg, ZONE_DRATAN, d->m_pChar, pCastle);
				SEND_Q(msg, d);
			}
		}
		
		// 이동 묻기
		if (zonePromptMove_Dratan != -1)
		{
			WarpPromptMsg(msg, zonePromptMove_Dratan, posPromptMove_Dratan);
			SEND_Q(msg, d);
		}
	}
//#endif // KJTEST
#endif // DRATAN_CASTLE	
	
#ifdef LOG_INDEX
	GAMELOG << init("JOIN", d->m_pChar)
#else	
	GAMELOG << init("JOIN", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
#endif // LOG_INDEX
		<< d->m_pChar->m_pZone->m_index	<< delim
		<< d->m_host
		<< end;
	
	// 캐릭터 시간 정보 초기화
	d->m_pChar->m_lastupdate = PULSE_SAVE_PC;
	d->m_pChar->m_autochknum[0] = d->m_pChar->m_autochknum[1] = 0;
	d->SetHackCheckPulse();

#ifdef RANKER_NOTICE
#ifdef LC_JPN
	if (!d->m_pChar->m_bLoadChar)
	{	// 일본은 로그인 시에만 랭커 입장 표시를 한다.
#endif // LC_JPN	
	if (IS_RUNNING_CONN)
	{
		ConnRankerReqMsg(msg, d->m_pChar->m_index, d->m_pChar->m_index);
		SEND_Q(msg, gserver.m_connector);
	}
#ifdef LC_JPN
	}
#endif // LC_JPN
#endif // RANKER_NOTICE

#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
	if( IS_RUNNING_CONN )
	{
		if( d->m_pChar->m_guildInfo && d->m_pChar->m_guildInfo->guild() )
		{
			ConnGuildPointRankerReqMsg( msg, d->m_pChar->m_guildInfo->guild()->index(), d->m_pChar->m_index );
			SEND_Q(msg, gserver.m_connector );
		}
	}
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD

	// 파티 리스트에서 현재 유저의 파티 정보를 찾아 설정한다
	d->m_pChar->m_party = FindPartyByMemberIndex(d->m_pChar->m_index, false);
	if (d->m_pChar->IsParty())
	{
		d->m_pChar->m_party->SetMemberPCPtr(d->m_pChar->m_index, d->m_pChar);

		if (!d->m_pChar->m_bLoadChar)
		{
			// 처음 접속에 파티가 있으면
			for (i = 0; i < MAX_PARTY_MEMBER; i++)
			{
				const CPartyMember* pMember = d->m_pChar->m_party->GetMemberByListIndex(i);

				if (pMember && pMember->GetCharIndex() != d->m_pChar->m_index)
				{
					PartyAddMsg(msg, pMember->GetCharIndex(), pMember->GetCharName(), pMember->GetMemberPCPtr(), ((d->m_pChar->m_party->GetBossIndex() == pMember->GetCharIndex()) ? 1 : 0));
					SEND_Q(msg, d);
				}
			}
		}

		PartyInfoMsg(msg, d->m_pChar->m_index, d->m_pChar->m_level, d->m_pChar->m_hp, d->m_pChar->m_maxHP, d->m_pChar->m_mp, d->m_pChar->m_maxMP, GET_X(d->m_pChar), GET_Z(d->m_pChar), GET_YLAYER(d->m_pChar), d->m_pChar->m_pZone->m_index);
		d->m_pChar->m_party->SendToAllPC(msg, d->m_pChar->m_index);
	}
	else
	{
		// 파티 없다고 알려줌
		PartyMsg(msg, MSG_PARTY_EMPTY);
		SEND_Q(msg, d);
	}

	d->m_pChar->OutputDBItemLog();


#ifdef EXPEDITION_RAID		
	// 원정대 리스트에서 현재 유저의 원정대 정보를 찾아 설정한다
	d->m_pChar->m_Exped = FindExpedByMemberIndex(d->m_pChar->m_index, false);

	if(d->m_pChar->m_Exped && 
	   d->m_pChar->m_Exped->GetRequestIndex() != d->m_pChar->m_index && 
	   d->m_pChar->m_Exped->GetMemberCount() > 1)
	{
		d->m_pChar->m_Exped->SetMemberPCPtr(d->m_pChar->m_index, d->m_pChar);

		if (!d->m_pChar->m_bLoadChar)
		{
			// 처음 접속에 파티가 있으면
			for(int i = 0; i < MAX_EXPED_GROUP; i++)
			{
				for(int j = 0; j < MAX_EXPED_GMEMBER; j++)
				{
					const CExpedMember* pMember = d->m_pChar->m_Exped->GetMemberByListIndex(i,j);

					if (pMember && pMember->GetCharIndex() != d->m_pChar->m_index)
					{
						//나에게 전송
						ExpedAddMsg(msg, pMember->GetCharIndex(), pMember->GetCharName(), i, j, pMember->GetListIndex(), pMember->GetMemberPCPtr(), ((d->m_pChar->m_Exped->GetBossIndex() == pMember->GetCharIndex()) ? 1 : 0));
						SEND_Q(msg, d);
					}
				}
			}
		}

		//다른 대원에게 전송 내 정보 전송 		
		const CExpedMember* pMember = d->m_pChar->m_Exped->GetMemberByCharIndex(d->m_pChar->m_index);
		if(pMember)
		{
			ExpedInfoMsg(msg, d->m_pChar->m_index,pMember->GetGroupType(), d->m_pChar->m_level, d->m_pChar->m_hp, d->m_pChar->m_maxHP, d->m_pChar->m_mp, d->m_pChar->m_maxMP, GET_X(d->m_pChar), GET_Z(d->m_pChar), GET_YLAYER(d->m_pChar), d->m_pChar->m_pZone->m_index);
			d->m_pChar->m_Exped->SendToAllPC(msg, d->m_pChar->m_index);
		}

		if(IS_RUNNING_HELPER)
		{
			HelperExpedRejoinReqMsg(msg,d->m_pChar->m_Exped->GetBossIndex(), d->m_pChar->m_index);
			SEND_Q(msg, gserver.m_helper);
		}
	}
#endif //EXPEDITION_RAID

	
#ifdef PARTY_MATCHING
	if (IS_RUNNING_HELPER)
	{
		HelperPartyMatchMemberChangeInfoMsg(msg, d->m_pChar->m_index, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE, "", d->m_pChar->m_level, d->m_pChar->m_pZone->m_index);
		SEND_Q(msg, gserver.m_helper);
	}
#endif // PARTY_MATCHING

	if (d->m_pChar->m_nGoldenBallNoticeStatus != m_clGoldenBall.GetStatus())
	{
		d->m_pChar->m_nGoldenBallNoticeStatus = m_clGoldenBall.GetStatus();

		switch (m_clGoldenBall.GetStatus())
		{
		case GOLDENBALL_STATUS_VOTE:
			EventGoldenballVoteStartMsg(msg, m_clGoldenBall.GetTeam1Name(), m_clGoldenBall.GetTeam2Name(), m_clGoldenBall.GetEndYear(), m_clGoldenBall.GetEndMonth(), m_clGoldenBall.GetEndDay(), m_clGoldenBall.GetEndHour(), m_clGoldenBall.GetEndMinute());
			SEND_Q(msg, d);
			break;

		case GOLDENBALL_STATUS_VOTE_END:
			EventGoldenballVoteEndMsg(msg, m_clGoldenBall.GetTeam1Name(), m_clGoldenBall.GetTeam2Name());
			SEND_Q(msg, d);
			break;

		case GOLDENBALL_STATUS_GIFT:
			EventGoldenballGiftStartMsg(msg, m_clGoldenBall.GetTeam1Name(), m_clGoldenBall.GetTeam1Score(), m_clGoldenBall.GetTeam2Name(), m_clGoldenBall.GetTeam2Score(), m_clGoldenBall.GetEndYear(), m_clGoldenBall.GetEndMonth(), m_clGoldenBall.GetEndDay(), m_clGoldenBall.GetEndHour(), m_clGoldenBall.GetEndMinute());
			SEND_Q(msg, d);
			break;

		default:
			break;
		}
	}
#ifdef CASH_ITEM_GIFT
	CashItemGiftRecvNoticeRepMsg(msg, (d->m_userFlag & RECV_GIFT) ? 1 : 0);
	SEND_Q(msg, d);
#endif

#ifdef EVENT_PARTNER_CODENEO
	if( !(d->m_userFlag & IS_CHARACTER) && strcmp( d->m_proSite, "CO" ) == 0 )
	{
		if( !d->m_pChar->GiveItem( 1819, 0, 0, 15, false ) )
		{
			GAMELOG << init("CODENEO_EVENT_GIVE_ITEM_FAILED", d->m_pChar)
				<< "USER_INDEX" << delim
				<< d->m_index << delim
				<< "PROSITE" << delim
				<< d->m_proSite << delim
				<< end;

				SysMsg( msg, MSG_SYS_FULL_INVENTORY );
				SEND_Q( msg, d );
		}
		GAMELOG << init("CODENEO_EVENT_GIVE_ITEM_FAILED", d->m_pChar)
			<< "ITEM_INDEX" << delim
			<< 1819 << delim
			<< "COUNT" << delim
			<< 15 << delim
			<< end;

		if( !d->m_pChar->GiveItem( 1820, 0, 0, 15, false ) )
		{
			GAMELOG << init("CODENEO_EVENT_GIVE_ITEM_FAILED", d->m_pChar)
				<< "USER_INDEX" << delim
				<< d->m_index << delim
				<< "PROSITE" << delim
				<< d->m_proSite << delim
				<< end;
			SysMsg( msg, MSG_SYS_FULL_INVENTORY );
			SEND_Q( msg, d );
		}
		GAMELOG << init("CODENEO_EVENT_GIVE_ITEM_FAILED", d->m_pChar)
			<< "ITEM_INDEX" << delim
			<< 1820 << delim
			<< "COUNT" << delim
			<< 15 << delim
			<< end;

		if( !d->m_pChar->GiveItem( 1821, 0, 0, 15, false ) )
		{
			GAMELOG << init("CODENEO_EVENT_GIVE_ITEM_FAILED", d->m_pChar)
				<< "USER_INDEX" << delim
				<< d->m_index << delim
				<< "PROSITE" << delim
				<< d->m_proSite << delim
				<< end;
			SysMsg( msg, MSG_SYS_FULL_INVENTORY );
			SEND_Q( msg, d );
		}
		GAMELOG << init("CODENEO_EVENT_GIVE_ITEM_FAILED", d->m_pChar)
			<< "ITEM_INDEX" << delim
			<< 1821 << delim
			<< "COUNT" << delim
			<< 15 << delim
			<< end;

		if( !d->m_pChar->GiveItem( 1822, 0, 0, 15, false ) )
		{
			GAMELOG << init("CODENEO_EVENT_GIVE_ITEM_FAILED", d->m_pChar)
				<< "USER_INDEX" << delim
				<< d->m_index << delim
				<< "PROSITE" << delim
				<< d->m_proSite << delim
				<< end;

			SysMsg( msg, MSG_SYS_FULL_INVENTORY );
			SEND_Q( msg, d );
		}
		GAMELOG << init("CODENEO_EVENT_GIVE_ITEM_SUCCESS", d->m_pChar)
			<< "ITEM_INDEX" << delim
			<< 1822 << delim
			<< "COUNT" << delim
			<< 15 << delim
			<< end;

		ConnEventPartnerCodeNEOReqMsg( msg, d->m_index, 1 );
		SEND_Q( msg, m_connector );
	}
#endif

#ifdef RESTRICT_PK
	if (d->m_pChar->IsChaotic())
	{
		if (d->m_pChar->m_lastPKTime == -1)
			d->m_pChar->m_lastPKTime = gserver.m_gameTime;
	}
	else
	{
		d->m_pChar->m_lastPKTime = -1;
	}
#endif // RESTRICT_PK

	ExPlayerStateChangeMsg(msg, d->m_pChar);
	SEND_Q(msg, d);

	d->m_pChar->m_bLoadChar = true;

	STATE(d) = CON_PLAYING;
#ifdef JP_OTAKU_SYSTEM
	if( d->m_pChar->m_etcEvent & OTAKU_SYSTEM )
	{
		CNetMsg OtakuMSG;
		GMWhoAmIMsg( OtakuMSG, d->m_pChar );
		SEND_Q( OtakuMSG, d );
	}
#endif //JP_OTAKU_SYSTEM

#ifdef JPN_GPARA_PROMOTION
	if (d->m_pChar->m_nGparaPromotionData == MSG_CONN_GPARA_PROMOTION_QUERY && IS_RUNNING_CONN)
	{
		ConnGparaPromotionMsg(msg, MSG_CONN_GPARA_PROMOTION_QUERY, d->m_index, d->m_pChar->m_index);
		SEND_Q(msg, m_connector);
	}
#endif // JPN_GPARA_PROMOTION

#ifdef JPN_OCN_GOO_PROMOTION
	if (d->m_pChar->m_nGparaPromotionData == MSG_CONN_OCN_GOO_PROMOTION_QUERY && IS_RUNNING_CONN)
	{
		ConnOCN_GooPromotionMsg(msg, MSG_CONN_OCN_GOO_PROMOTION_QUERY, d->m_index, d->m_pChar->m_index);
		SEND_Q(msg, m_connector);
	}
#endif // JPN_OCN_GOO_PROMOTION
	
#ifdef JPN_MSN_PROMOTION
	if (d->m_pChar->m_nMSNPromotionData == MSG_CONN_MSN_PROMOTION_QUERY && IS_RUNNING_CONN)
	{
		ConnMSNPromotionMsg(msg, MSG_CONN_MSN_PROMOTION_QUERY, d->m_index, d->m_pChar->m_index);
		SEND_Q(msg, m_connector);
	}
#endif // JPN_MSN_PROMOTION

#ifdef CREATE_EVENT
	if (d->m_pChar->m_nCreateEventData == MSG_CONN_CREATEEVENT_QUERY && IS_RUNNING_CONN)
	{
		ConnCreateEventMsg(msg, MSG_CONN_CREATEEVENT_QUERY, d->m_index, d->m_pChar->m_index);
		SEND_Q(msg, m_connector);
	}
#endif // CREATE_EVENT

#ifdef FREE_PK_SYSTEM
	if( gserver.m_bFreePk )
	{
		FreePKMsg(msg);
		SEND_Q( msg, d );
	}
#endif

#ifdef EVENT_FLOWERTREE_2007
	if( IS_RUNNING_CONN )
	{
		ConnEventFlowerTree2007Msg( msg, MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT, d->m_pChar->m_index );
		SEND_Q(msg, m_connector);
	}	
#endif // EVENT_FLOWERTREE_2007
	
#ifdef EVENT_XMAS_2007
	CSkill *pSkill = gserver.m_skillProtoList.Create( 490 , 1 );
	if( pSkill )
	{
		bool bApply = false ;
		ApplySkill( d->m_pChar, d->m_pChar, pSkill, -1 , bApply );
		if( !bApply )
		{
			GAMELOG << init("EVENT XMAS SKILL FAILED (LOGIN) ", d->m_pChar ) << end;// 스킬 적용 실패
		}
	}
#else
	if(d->m_pChar->m_assist.FindBySkillIndex(490))
	{
		d->m_pChar->m_assist.CureBySkillIndex(490);
	}
#endif //EVENT_XMAS_2007

#ifdef EVENT_PCBANG_2ND
	if(d->m_location == BILL_LOCATION_PCBANG)
	{
		CSkill * pSkill = gserver.m_skillProtoList.Create( 493 , 1 );
		if(pSkill != NULL)
		{
			bool bApply = false;
			ApplySkill(d->m_pChar, d->m_pChar, pSkill, -1 , bApply);
			if(bApply == false)
			{
				GAMELOG << init("EVENT_PCBANG_2NDS SKILL FAILED (LOGIN) ", d->m_pChar ) << end;// 스킬 적용 실패
			}
		}
	}
#endif // EVENT_PCBANG_2ND
	
#ifdef EVENT_PHOENIX   // 피닉스 이벤트  yhj
	// 피닉스 케릭터 이면서 100 레벨이 넘지 않는다면 피닉스 버프를 준다.
	if( d->m_pChar->m_bPhoenix_Char == 1 && d->m_pChar->m_level <= 100 )
	{
		CSkill * pSkill1 = gserver.m_skillProtoList.Create( 516, 1 );
		if(pSkill1 != NULL)
		{
			bool bApply = false;
			ApplySkill(d->m_pChar, d->m_pChar, pSkill1, -1 , bApply);
			if(bApply == false)
			{
				GAMELOG << init("EVENT_PHOENIX SKILL FAILED (LOGIN) ", d->m_pChar ) << end;// 스킬 적용 실패
			}
		}
	}
#endif
	
#ifdef DRATAN_CASTLE
	if (pCastle != NULL && d->m_pChar != NULL && d->m_pChar->m_pZone->m_index == ZONE_DRATAN)
	{	// 공성중이면 부활 진지 정보 전송
		CastletowerQuartersListMsg(msg, pCastle);
		SEND_Q(msg, d);
	}
	
#endif //DRATN_CASTLE
	
#ifdef ATTENDANCE_EVENT_REWARD
// 출석이벤트 보상 확인 
// ->CONNECT		// UserIndex, CharIndex
	if( IS_RUNNING_CONN )
	{
		ConnEventAttendanceRewardMsg( msg, MSG_ATTENDANCE_COUNT, d->m_index,  d->m_pChar->m_index );
		SEND_Q(msg, m_connector);
	}		
#endif //ATTENDANCE_EVENT_REWARD
	
#ifdef NIGHT_SHADOW
	switch( d->m_pChar->m_job )
	{
	case JOB_TITAN:
	case JOB_KNIGHT:
	case JOB_MAGE:
	case JOB_ROGUE:
	case JOB_SORCERER:
	default:
		d->m_pChar->m_attacktype = ATTACK_TYPE_NORMAL;
		break;
	case JOB_HEALER:
	case JOB_NIGHTSHADOW:
		d->m_pChar->m_attacktype = ATTACK_TYPE_DOUBLE;
		break;
	}
#endif //NIGHT_SHADOW

} // CharPrePlay

#ifdef EVENTSETTING
bool CServer::LoadNotice()
{
// NOTICE START
	int i;
#ifdef EVENT_LATTO
	gserver.m_bLattoEvent = true;
#endif

#ifdef EVENT_NEWYEAR
	gserver.m_bNewYearEvent= true;
#endif

#ifdef EVENT_VALENTINE
	gserver.m_bValentineEvent= true;
#endif

#ifdef EVENT_WHITEDAY
	gserver.m_bWhiteDayEvent = true;
#endif

#ifdef EVENT_LETTER
	m_bLetterEvent = true;
#endif

#ifdef EVENT_MOONSTONE
	gserver.m_bMoonStoneEvent = true;
#endif

#ifdef EVENT_RICESOUP
	m_bRiceShoupEvent = true;
#endif

//0704 이벤트에 관한 공지 세팅.
#ifdef NOTICE_EVENT
	i =0;
	memset(m_aNotice, 0, sizeof(int) * 5);

#ifdef EVENT_TREASUREBOX
	m_aNotice[i] = EVENT_TREASUREBOX;
	
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
	//FILE* fpEventJPN2007TreasureBox = fopen(".event_2007_treasurebox", "rt");
	FILE* fpEventJPN2007TreasureBox = fopen("event_2007_treasurebox", "rt");

	if (fpEventJPN2007TreasureBox)
	{
		char buf[10] = "";
		fgets(buf, 10, fpEventJPN2007TreasureBox);
		fclose(fpEventJPN2007TreasureBox);
		buf[9] = '\0';
		if (atoi(buf) == m_serverno)
		{
			m_bEventJPN2007TreasureboxAddItem = true;
			m_aNotice[i] = EVENT_JPN_2007_TREASUREBOX_ADD_ITEM;
		}	
	}
#endif // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

	i++;
#endif // #ifdef EVENT_TREASUREBOX


#if defined(EVENT_TREASUREBOX_RED) && !defined(EVENT_SUMMER_2008)
	m_aNotice[i] = EVENT_TREASUREBOX_RED;
	i++;
#endif // EVENT_TREASUREBOX_RED

//#ifdef EVENT_TEACH
//	m_aNotice[i] = EVENT_TEACH;
//	i++;
//#endif // #ifdef EVENT_TEACH

#ifdef EVENT_FRUIT_WATERMELON
	m_aNotice[i] = EVENT_FRUIT_WATERMELON;
	i++;
#endif // #ifdef EVENT_FRUIT_WATERMELON

#ifdef EVENT_FRUIT_MELON
	m_aNotice[i] = EVENT_FRUIT_MELON;
	i++;
#endif // #ifdef EVENT_FRUIT_MELON

#ifdef EVENT_FRUIT_PLUM
	m_aNotice[i] = EVENT_FRUIT_PLUM;
	i++;
#endif // #ifdef EVENT_FRUIT_PLUM

#ifdef EVENT_CHUSEOK
	m_aNotice[i] = EVENT_CHUSEOK;
	i++;
#endif // #ifdef EVENT_CHUSEOK

#ifdef EVENT_SEPTEMBER
	m_aNotice[i] = EVENT_SEPTEMBER;
	i++;
#endif // #ifdef EVENT_SEPTEMBER

#ifdef EVENT_PEPERO
	m_aNotice[i] = EVENT_PEPERO;
	i++;
#endif // #ifdef EVENT_PEPERO

#ifdef EVENT_XMAS_2005
	m_aNotice[i] = EVENT_XMAS_2005;
	i++;
#endif // #ifdef EVENT_XMAS_2005

#ifdef EVENT_NEWYEAR_2006
	m_aNotice[i] = EVENT_NEWYEAR_2006;
	i++;
#endif // #ifdef EVENT_NEWYEAR_2006

#ifdef EVENT_CHANGE_ARMOR_2005
	m_aNotice[i] = EVENT_CHANGE_ARMOR_2005;
	i++;
#endif // #ifdef EVENT_CHANGE_ARMOR_2005

#ifdef EVENT_SEARCHFRIEND
	m_aNotice[i] = EVENT_SEARCHFRIEND;
	i++;	
#endif // #ifdef EVENT_SEARCHFRIEND

#ifdef EVENT_VALENTINE_2006
	m_aNotice[i] = EVENT_VALENTINE_2006;
	i++;	
#endif // #ifdef EVENT_VALENTINE_2006

#ifdef EVENT_WHITEDAY_2006
	m_aNotice[i] = EVENT_WHITEDAY_2006;
	i++;	
#endif // #ifdef EVENT_WHITEDAY_2006

#ifdef EVENT_NEW_SERVER_2006_OLDSERVER
#ifdef BSTEST
	m_aNotice[i] = EVENT_NEW_SERVER_2006_OLDSERVER;
	i++;
#else // BSTEST
	if (gserver.m_serverno <= EVENT_NEW_SERVER_2006_SERVERNO)
	{
		m_aNotice[i] = EVENT_NEW_SERVER_2006_OLDSERVER;
		i++;
	}
#endif // BSTEST
#endif // EVENT_NEW_SERVER_2006_OLDSERVER

#ifdef EVENT_NEW_SERVER_2006_NEWSERVER
#ifdef BSTEST
	m_aNotice[i] = EVENT_NEW_SERVER_2006_NEWSERVER;
	i++;
#else // BSTEST
	if (gserver.m_serverno >= EVENT_NEW_SERVER_2006_SERVERNO)
	{
		m_aNotice[i] = EVENT_NEW_SERVER_2006_NEWSERVER;
		i++;
	}
#endif // BSTEST
#endif // EVENT_NEW_SERVER_2006_NEWSERVER

#ifdef EVENT_OX_QUIZ
	m_aNotice[i] = EVENT_OX_QUIZ;
	i++;
#endif // EVENT_OX_QUIZ

#ifdef EVENT_WORLDCUP_2006
	m_aNotice[i] = EVENT_WORLDCUP_2006;
	i++;
#endif // EVENT_WORLDCUP_2006

#ifdef EVENT_OPEN_BETA_TLD
	struct tm start = NOW();
	struct tm stop = NOW();

	start.tm_year = 2005 - 1900;
	start.tm_mon = 10;
	start.tm_mday = 18;
	start.tm_hour = 0;
	start.tm_min = 0;
	start.tm_sec = 0;
	
	stop.tm_year = 2005 - 1900;
	stop.tm_mon = 11;
	stop.tm_mday = 6;
	stop.tm_hour = 0;
	stop.tm_min = 0;
	stop.tm_sec = 0;

	time_t t_now = mktime(&tm_now);
	if( mktime(&start) <= t_now && mktime(&stop) > t_now)
	{
		m_bOpenEvent = true;

		// 0 : 모두 드롭 1: 배양토 2: 레드 3: 푸른 4: 황금 5: 배양,레드 6:레드,푸른 7:푸른,골드 8: 모두 안 드롭
		if(tm_now.tm_mday >= 18 && tm_now.tm_mday < 21)
			m_bSoilDrop = 1;
		if(tm_now.tm_mday >= 21 && tm_now.tm_mday < 23)
			m_bSoilDrop = 5;
		if(tm_now.tm_mday >= 23 && tm_now.tm_mday < 25)
			m_bSoilDrop = 2;
		if(tm_now.tm_mday >= 25 && tm_now.tm_mday < 27)
			m_bSoilDrop = 6;
		if(tm_now.tm_mday >= 27 && tm_now.tm_mday < 28)
			m_bSoilDrop = 3;
		if(tm_now.tm_mday >= 28 && tm_now.tm_mday < 30)
			m_bSoilDrop = 7;
		if(tm_now.tm_mday == 30 || (tm_now.tm_mday >= 1 && tm_now.tm_mday < 3))
			m_bSoilDrop = 4;
		
		if(tm_now.tm_mday == 3 || tm_now.tm_mday == 4 || tm_now.tm_mday == 5)
		{
			m_bSoilDrop = 0;
		}

	}
#endif
#ifdef EVENT_FLOWER
	if ( tm_now.tm_year == (2005 - 1900) && tm_now.tm_mon == 11 && tm_now.tm_mday >= 5 && tm_now.tm_mday <= 12 )
	{
		m_bFlowerEvent = true;
		if ( tm_now.tm_mday == 5 )
			m_bFlower = 1;
		else
			m_bFlower = 0;
	}
#endif
#ifdef EVENT_SAKURA
	m_aNotice[i] = EVENT_SAKURA;
	i++;
#endif

#if defined(EVENT_RAIN_2006) && !defined(EVENT_SUMMER_2008)
	m_aNotice[i] = EVENT_RAIN_2006;
	i++;
#endif // EVENT_RAIN_2006 && !defined(EVENT_SUMMER_2008)

#ifdef EVENT_TLD_BUDDHIST
	m_aNotice[i] = EVENT_TLD_BUDDHIST;
	i++;
#endif // EVENT_TLD_BUDDHIST

#if defined(EVENT_COLLECT_BUG) && defined(EVENT_COLLECT_BUG_DROP)
	m_aNotice[i] = EVENT_COLLECT_BUG;
	i++;
#endif // #if defined(EVENT_COLLECT_BUG) && defined(EVENT_COLLECT_BUG_DROP)

#ifdef EVENT_NEWSERVER_BASTARD
#if defined(BSTEST) || defined(LC_MAL)
	static const int		nNewServerNo = 1;
#else // BSTEST
	static const int		nNewServerNo = 5;
#endif // BSTEST
	if (m_serverno == nNewServerNo)
		m_aNotice[i] = EVENT_NEWSERVER_BASTARD;
	else
		m_aNotice[i] = EVENT_NEWSERVER_BASTARD_OLD_NOTICE;
	i++;
#endif // EVENT_NEWSERVER_BASTARD

#ifdef EVENT_CHUSEOK_2006
	m_aNotice[i] = EVENT_CHUSEOK_2006;
	i++;
#endif // EVENT_CHUSEOK_2006

#ifdef EVENT_HALLOWEEN_2006
#ifdef LC_BRZ
#else
	m_aNotice[i] = EVENT_HALLOWEEN_2006;
	i++;
#endif // LC_BRZ
	
#endif // EVENT_HALLOWEEN_2006

#ifdef EVENT_XMAS_2006
#if defined (LC_BRZ)
#else
	m_aNotice[i] = EVENT_XMAS_2006;
	i++;
#endif // defined (LC_BRZ)
#endif // EVENT_XMAS_2006
		
#ifdef EVENT_CHILDERN_DAY
	m_aNotice[i] = EVENT_CHILDERN_DAY;
	i++;
#endif // EVENT_CHILDERN_DAY
	
#ifdef EVENT_VALENTINE_2007
	m_aNotice[i] = EVENT_VALENTINE_2007;
	i++;
#endif	// EVENT_VALENTINE_2007

#ifdef EVENT_WHITEDAY_2007
	m_aNotice[i] = EVENT_WHITEDAY_2007;
	i++;
#endif // WHITE_DAY [3/6/2007 KwonYongDae]

#ifdef EVENT_TLD_2007_SONGKRAN
	m_aNotice[i] = EVENT_TLD_2007_SONGKRAN;
	i++;
#endif //EVENT_TLD_2007_SONGKRAN

#ifdef EVENT_EGGS_HUNT_2007
	m_aNotice[i] = EVENT_EGGS_HUNT_2007;
	i++;
#endif // EVENT_EGGS_HUNT_2007

#ifdef EVENT_2007_PARENTSDAY
	m_aNotice[i] = EVENT_2007_PARENTSDAY;
	i++;
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_JPN_2007_NEWSERVER

	//FILE* fpEventJPN2007NewServer = fopen(".event_2007_newserver", "rt");
	FILE* fpEventJPN2007NewServer = fopen("event_2007_newserver", "rt");

	if (fpEventJPN2007NewServer)
	{
		char buf[10] = "";
		fgets(buf, 10, fpEventJPN2007NewServer);
		fclose(fpEventJPN2007NewServer);
		buf[9] = '\0';
		if (atoi(buf) == m_serverno)
		{
			CDBCmd cmdCheckCharDBForEvent;
			cmdCheckCharDBForEvent.Init(&m_dbchar);

			// 필드 검사
			cmdCheckCharDBForEvent.SetQuery("SELECT a_index, a_userindex, a_charindex, a_date, a_give FROM t_event_2007_newserver LIMIT 1");
			if (!cmdCheckCharDBForEvent.Open())
			{
				GAMELOG << init("Error: Character DB Check: Not Found t_event_2007_newserver!!") << end;
				return false;
			}

			// 현재 수 검사
			cmdCheckCharDBForEvent.SetQuery("SELECT COUNT(a_index) AS a_cnt FROM t_event_2007_newserver");
			if (cmdCheckCharDBForEvent.Open() && cmdCheckCharDBForEvent.MoveFirst())
			{
				int nCountEvent = 0;
				cmdCheckCharDBForEvent.GetRec("a_cnt", nCountEvent);
				if (nCountEvent < EVENT_JPN_2007_NEWSERVER_NAS10K)
				{
					m_bEventJPN2007NewServer = true;
				}
			}
		}
	}
#endif // EVENT_JPN_2007_NEWSERVER
	
#ifdef EVENT_GOMDORI_2007
	m_aNotice[i] = EVENT_GOMDORI_2007;
	i++;
#endif // EVENT_GOMDORI_2007
	
#ifdef EVENT_CHILDRENSDAY_2007
	m_aNotice[i] = EVENT_CHILDRENSDAY_2007;
	i++;
#endif // EVENT_CHILDRENSDAY_2007

#ifdef EVENT_FLOWERTREE_2007
#ifndef EVENT_NODROP_FLOWERTREE_2007
	m_aNotice[i] = EVENT_FLOWERTREE_2007;
	i++;
#endif //EVENT_NODROP_FLOWERTREE_2007
#endif //EVENT_FLOWERTREE_2007

#ifdef EVENT_TEACH_2007
	m_aNotice[i] = EVENT_TEACH_2007;
	i++;
#endif // EVENT_TEACH_2007

#ifdef EVENT_CHILDRENSDAY_2007
	m_aNotice[i] = EVENT_CHILDRENSDAY_2007;
	i++;
#endif // EVENT_CHILDRENSDAY_2007

#ifdef EVENT_UCC_2007
	m_aNotice[i] = EVENT_UCC_2007;
	i++;
#endif // EVENT_UCC_2007

#ifdef EVENT_TLD_MOTHERDAY_2007
	m_aNotice[i] = EVENT_TLD_MOTHERDAY_2007;
	i++;
#endif // EVENT_TLD_MOTHERDAY_2007
	

#if defined(EVENT_OPEN_ADULT_SERVER) && !(EVENT_SUMMER_2008)
	m_aNotice[i] = EVENT_OPEN_ADULT_SERVER;
	i++;
#endif // EVENT_OPEN_ADULT_SERVER


#ifdef EVENT_LC_1000DAY 
	m_aNotice[i] = EVENT_LC_1000DAY;
	i++;
#endif // EVENT_RICHYEAR_2007

#ifdef EVENT_RICHYEAR_2007
	m_aNotice[i] = EVENT_RICHYEAR_2007;
	i++;
#endif // EVENT_RICHYEAR_2007

#ifdef EVENT_HALLOWEEN_2007
	m_aNotice[i] = EVENT_HALLOWEEN_2007;
	i++;
#endif // EVENT_HALLOWEEN_2007

#ifdef EVENT_XMAS_2007
	m_aNotice[i] = EVENT_XMAS_2007;
	i++;
#endif //EVENT_XMAS_2007

#ifdef NEWYEAR_EVENT_2008
	m_aNotice[i] = NEWYEAR_EVENT_2008;
	i++;
#endif // NEWYEAR_EVENT_2008

#ifdef SAKURA_EVENT_2008
	m_aNotice[i] = SAKURA_EVENT_2008;
	i++;
#endif // SAKURA_EVENT_2008

#ifdef CHAOSBALL
	m_aNotice[i] = CHAOSBALL;
	i++;
#endif

#ifdef EVENT_SUMMER_2008
	m_aNotice[i] = EVENT_SUMMER_2008;
	i++;
#endif //EVENT_SUMMER_2008
/*
#ifdef EVENT_PHOENIX
	m_aNotice[i] = EVENT_PHOENIX;
	i++;
#endif // EVENT_PHOENIX
*/
#endif // #ifdef NOTICE_EVENT

	if (i > MAX_NOTICE)
	{
		GAMELOG << init("SYSTEM ERROR: OVERFLOW NOTICE") << end;
		return false;
	}
// NOTICE END
	return true;
}
#endif // EVENTSETTING


#ifdef EVENTSETTING
CEventSetting::CEventSetting()
{
	m_pEventSettingData = NULL;
	m_pEventSettingBackupData = NULL;
	m_nCount = 0;
}

CEventSetting::~CEventSetting()
{
	if(m_pEventSettingData)
	{
		delete[] m_pEventSettingData;
		m_pEventSettingData = NULL;
		m_nCount			= 0;
	}
	if(m_pEventSettingBackupData)
	{
		delete[] m_pEventSettingBackupData;
		m_pEventSettingBackupData = NULL;
	}
}

int CEventSetting::GetEventSetting(int nEventIdx) const
{
	if(nEventIdx > 0 && nEventIdx < m_nCount)
	{
		return m_pEventSettingData[m_nCount].nEventData;
	}
	return 0;
}

int CEventSetting::GetEventExtra(int nEventIdx) const
{
	if(nEventIdx > 0 && nEventIdx < m_nCount)
	{
		return m_pEventSettingData[nEventIdx].nEventExtra;
	}
	return 0;
}

void CEventSetting::Reset()
{
	if(m_pEventSettingData)
	{
		memset(m_pEventSettingData, 0, sizeof(m_pEventSettingData));
	}
	else
	{
		m_pEventSettingData = NULL;
	}
}

void CEventSetting::Backup()
{
	if(m_pEventSettingData)
	{
		if(m_pEventSettingBackupData)
		{
			delete[] m_pEventSettingBackupData;
			m_pEventSettingBackupData = NULL;
		}
		m_pEventSettingBackupData = new EVENT_INFO[m_nCount];
		memcpy(m_pEventSettingBackupData, m_pEventSettingData, sizeof(EVENT_INFO) * m_nCount);
	}
}

void CEventSetting::RollBack()
{
	if(m_pEventSettingBackupData)
	{
		Reset();
		memcpy(m_pEventSettingData, m_pEventSettingBackupData, sizeof(EVENT_INFO) * m_nCount);
		DeleteBackupData();
	}
}

void CEventSetting::DeleteBackupData()
{
	if(m_pEventSettingBackupData)
	{
		delete[] m_pEventSettingBackupData;
		m_pEventSettingBackupData = NULL;
	}
}

bool CEventSetting::IsEventEnable(int nEventIdx)
{
	if(nEventIdx > 0 && nEventIdx < m_nCount)
	{
		return m_pEventSettingData[nEventIdx].bEventNotice;
	}
	return false;
}

bool CEventSetting::Load()
{
	strcpy(g_buf, "SELECT * FROM t_eventsettings ORDER BY a_event_idx DESC");
	CDBCmd dbcmd;
	dbcmd.Init(&gserver.m_dbdata);
	dbcmd.SetQuery(g_buf);
	if(!dbcmd.Open())
		return false;

	if(dbcmd.MoveFirst())
	{
		int count;
		dbcmd.GetRec("a_event_idx", count);
		m_pEventSettingData = new EVENT_INFO[count + 1];
		memset(m_pEventSettingData, 0, sizeof(m_pEventSettingData));

		m_nCount = count + 1;
	}
	else
	{
		m_nCount = 0;
		m_pEventSettingData = NULL;
		return true;
	}

	if(dbcmd.MoveFirst())
	{
		int EventIdx;
		int EventNotice;
		int EventEnable;
		int EventExtra;
		int TotalEventCount = 0;
		do
		{
			dbcmd.GetRec("a_event_idx", EventIdx);
			dbcmd.GetRec(EVENT_ENABLE_FIELD, EventEnable); 
			dbcmd.GetRec(EVENT_EXTRA_FILED, EventExtra);
			dbcmd.GetRec("a_notice", EventNotice);

			if(EventIdx <= 0 || EventIdx >= m_nCount)
				return false;
			if(!m_pEventSettingData || m_pEventSettingData[EventIdx].nEventIndex > 0)
				return false;
			if(EventEnable < 0 || EventEnable > 9999)
				return false;

			m_pEventSettingData[EventIdx].nEventIndex	= EventIdx;
			m_pEventSettingData[EventIdx].nEventData	= EventEnable;
			m_pEventSettingData[EventIdx].nEventExtra	= EventExtra;

			int min, max;
			min = (EventNotice >> 16) & 0xffff;
			max = EventNotice & 0xffff;
			if(min <= 0 || min > 9999 || max <= 0 || max > 9999)
				return false;
			if(EventEnable >= min && EventEnable <= max)
			{
				m_pEventSettingData[EventIdx].bEventNotice = true;
				if(EventEnable >= MAX_NOTICE)
				{
					GAMELOG << init("SYSTEM ERROR: OVERFLOW NOTICE") << end;
					return false;
				}
				EventEnable++;
			}
			else
			{
				m_pEventSettingData[EventIdx].bEventNotice = false;
			}
		}while(dbcmd.MoveNext());
	}
	else
	{
		return false;
	}

	return true;
}

bool CEventSetting::Load(CNetMsg& msg)
{
	if (!mysql_real_connect ( 
				&gserver.m_dbdata,
				gserver.m_config.Find("Data DB", "IP"),
				gserver.m_config.Find("Data DB", "User"),
				gserver.m_config.Find("Data DB", "Password"),
				gserver.m_config.Find("Data DB", "DBName"), 0, NULL, 0))
	{
		return false;
	}

	CLCString sql(1024);
	sql.Format("SELECT * FROM t_eventsettings WHERE %s > 0 AND %s < 10000 ORDER BY a_event_idx", EVENT_ENABLE_FIELD, EVENT_ENABLE_FIELD);

	CDBCmd dbcmd;
	dbcmd.Init(&gserver.m_dbdata);
	dbcmd.SetQuery(g_buf);
	if(!dbcmd.Open())
		return false;
	
	int nEventIdx, nEventEnable, nEventExtra, nEventNotice, bIsEventNotice;
	int nNoticeCount = 0;

	int nCount = dbcmd.m_nrecords;

	
	msg << nCount;
	while(dbcmd.MoveNext())
	{
		dbcmd.GetRec("a_event_idx", nEventIdx);
		dbcmd.GetRec(EVENT_ENABLE_FIELD, nEventEnable); 
		dbcmd.GetRec(EVENT_EXTRA_FILED, nEventExtra);
		dbcmd.GetRec("a_notice", nEventNotice);

		int min, max;
		min = (nEventNotice >> 16) & 0xffff;
		max = nEventNotice & 0xffff;
		if(min <= 0 || min > 9999 || max <= 0 || max > 9999)
			return false;
		if(nEventNotice >= min && nEventNotice <= max)
		{
			if(nNoticeCount >= MAX_NOTICE)
				return false;
			nNoticeCount++;
			bIsEventNotice = true;
		}
		else
		{
			bIsEventNotice = false;
		}

		msg << nEventIdx
			<< nEventEnable
			<< nEventExtra
			<< nEventNotice;
	}
	mysql_close(&gserver.m_dbdata);
	return true;
}


bool CEventSetting::SetEventData(int nEventIdx, int nEventData, int nEventExtra, bool bEventNotice)
{
	if(m_pEventSettingData && nEventIdx > 0 && nEventIdx < m_nCount)
	{
		m_pEventSettingData[nEventIdx].nEventIndex	= nEventIdx;
		m_pEventSettingData[nEventIdx].nEventData	= nEventData;
		m_pEventSettingData[nEventIdx].nEventExtra	= nEventExtra;
		m_pEventSettingData[nEventIdx].bEventNotice	= bEventNotice;
		return true;
	}
	
	return false;
}
#endif // EVENTSETTING

#ifdef CHANCE_EVENT
bool CServer::CheckChanceEventLevel(int level) 
{ 
	if ((level >= m_bChanceSlevel) 
		&& (level <= m_bChanceElevel))
	{ 
		return true; 
	}
	return false; 
}
#endif // CHANCE_EVENT

#ifdef GMTOOL
void CServer::GMToolCharPrePlay(CDescriptor* d)
{
	int i;
	char name[MAX_CHAR_NAME_LENGTH + 1];
	char id[MAX_ID_NAME_LENGTH + 1];
	memset(name, 0, MAX_CHAR_NAME_LENGTH + 1);
	for(i = 0; i < gserver.m_nGMToolCount; ++i)
	{
		sprintf(g_buf, "GMTOOL_%d", i);
		if(gserver.m_config.Find(g_buf, "ID"))
		{
			memset(id, 0, MAX_ID_NAME_LENGTH + 1);
			strcpy(id, gserver.m_config.Find(g_buf, "ID"));
			if(strcmp(id, d->m_idname) == 0)
			{
				strcpy(name, gserver.m_config.Find(g_buf, "NAME"));
				break;
			}
		}
	}
	
	if(i == gserver.m_nGMToolCount)
	{
		d->m_bclosed = true;
		return ;
	}
	
	sprintf(g_buf,
		"SELECT * FROM t_characters WHERE a_user_index = %d AND a_server = %d"
		" AND a_name = '%s'"
		" AND a_enable=1", d->m_index, 1, name);
	
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);
	if(!cmd.Open())
	{
		d->m_bclosed = true;
		return ;
	}
	
	if(cmd.MoveFirst())
	{
		if(d->m_pChar)
		{
			delete d->m_pChar;
			d->m_pChar = NULL;
		}
		
		int index;
		CLCString name(100), nick(100);
		int admin;
		int nZone, nArea, nYlayer, nJob;
		
		cmd.GetRec("a_index", index);
		cmd.GetRec("a_name", name);
		cmd.GetRec("a_nick", nick);
		cmd.GetRec("a_admin", admin);
		cmd.GetRec("a_was_zone", nZone);
		cmd.GetRec("a_was_area", nArea);
		cmd.GetRec("a_was_yLayer", nYlayer);
		cmd.GetRec("a_job", nJob);
		
		d->m_pChar = new CPC;
		d->m_pChar->m_index = index;
		d->m_pChar->m_desc = d;
		
		d->m_pChar->m_bPlaying = true;
		d->m_pChar->ResetPlayerState(~0);
		d->m_pChar->m_regGuild = 0;
		d->m_pChar->m_admin = admin;
		d->m_pChar->m_nick = nick;
		d->m_pChar->m_name = name;
		d->m_pChar->m_job = nJob;
		
		int i;
		i = gserver.FindZone(ZONE_START);
		nYlayer = 0;
		d->m_pChar->m_pZone		= gserver.m_zones + i;
		d->m_pChar->m_pArea		= d->m_pChar->m_pZone->m_area;
		GET_YLAYER(d->m_pChar)	= d->m_pChar->m_pZone->m_zonePos[0][0];
		GET_R(d->m_pChar)		= 0.0f;
		
		int cx, cz;
		CArea* area = d->m_pChar->m_pArea;
		area->PointToCellNum(GET_X(d->m_pChar), GET_Z(d->m_pChar), &cx, &cz);
		area->CharToCell(d->m_pChar, GET_YLAYER(d->m_pChar), cx, cz);
		
		//GET_YLAYER(d->m_pChar)	= nYlayer;
		
		gserver.m_playerList.Add(d->m_pChar);
		d->m_pChar->m_bLoadChar = true;
		d->m_pChar->m_bPlaying = true;
		d->m_pChar->ResetPlayerState(~0);
		d->m_pChar->m_regGuild = 0;
		d->m_pChar->m_lastupdate = PULSE_SAVE_PC;
		d->m_pChar->m_autochknum[0] = d->m_pChar->m_autochknum[1] = 0;
		d->SetHackCheckPulse();
		STATE(d) = CON_PLAYING;
		
		d->m_pChar->m_secretkey = GetRandom(10,100);
		int key = ((d->m_pChar->m_index + d->m_pChar->m_secretkey) << 1);
		
		CNetMsg rmsg;
		rmsg.Init(MSG_LOGIN);
		rmsg << d->m_pChar->m_index
			<< d->m_pChar->m_name
			<< d->m_nPrepareSeed
			<< d->m_pChar->m_job
			<< key;
		
		SEND_Q(rmsg, d);
	}
	else
	{
		d->m_bclosed = true;
		return ;
	}
	
	if(gserver.m_nGMToolCount > 0)
	{
		int i;
		for(i = 0; i < gserver.m_nGMToolCount;++i)
		{
			if(gserver.m_gmtool[i] == NULL)
			{
				gserver.m_gmtool[i] = d;
				break;
			}
		}
		if(i == gserver.m_nGMToolCount)
		{
			d->m_bclosed = true;
			return ;
		}
	}
	else
	{
		d->m_bclosed = true;
		return ;
	}
}
#endif // GMTOOL

#ifdef CREATE_EVENT
bool CServer::LoadCreateEvent()
{
	GAMELOG << init("CREATE EVENT Loading... ") << end;

	sprintf(g_buf, "select idx, UNIX_TIMESTAMP(start_time) as stime, UNIX_TIMESTAMP(end_time) as etime, "
		"item_idx0, item_cnt0, item_idx1, item_cnt1, item_idx2, item_cnt2, item_idx3, item_cnt3, "
		"item_idx4, item_cnt4, item_idx5, item_cnt5, item_idx6, item_cnt6, item_idx7, item_cnt7,"
		"item_idx8, item_cnt8, item_idx9, item_cnt9, table_created "
//		"FROM create_event WHERE start_time <= now() AND end_time > now() AND national_code=%d "
		"FROM create_event WHERE national_code=%d "
		"ORDER BY idx LIMIT 1", gserver.m_national);
	CDBCmd cmd;
	cmd.Init(&m_dbdata);
	cmd.SetQuery(g_buf);

	if(cmd.Open() == true)
	{
		if(cmd.MoveFirst() == true)
		{
			gserver.m_bCreateEvent = true;
			
			cmd.GetRec("idx", gserver.m_nCreateTableIndex);
			cmd.GetRec("stime", gserver.m_nCreateEventStartTime);
			cmd.GetRec("etime", gserver.m_nCreateEventEndTime);

			char idx[10], cnt[10];
			for(int i = 0; i < 10; i++)
			{	
				sprintf(idx, "item_idx%d", i);
				sprintf(cnt, "item_cnt%d", i);
				cmd.GetRec(idx, gserver.m_nCreateEventItemIdx[i]);
				cmd.GetRec(cnt, gserver.m_nCreateEventItemCnt[i]);
			}

			unsigned char bCreatedTable = 0;
			cmd.GetRec("table_created", bCreatedTable);

			GAMELOG << init("CREATE EVENT Loading...[ok] ") << delim
				<< gserver.m_nCreateTableIndex << delim
				<< gserver.m_nCreateEventStartTime << delim
				<< gserver.m_nCreateEventEndTime << end;

			if(bCreatedTable == 0)
			{	// 아이템 받은 인덱스 저장 테이블 생성				
				CNetMsg msg;
				ConnCreateEventMsg(msg, MSG_CONN_CREATEEVENT_CREATETABLE, 0, 0);
				SEND_Q(msg, gserver.m_connector);
			}

			return true;
		}		
	}	

	GAMELOG << init("CREATE EVENT Loading...[failed] ") << end;
	ClearCreateEvent();

	return false;
}

void CServer::ClearCreateEvent()
{
	m_bCreateEvent = false;
	m_nCreateTableIndex = 0;
	m_nCreateEventStartTime = 0;
	m_nCreateEventEndTime = 0;
	for(int i=0; i < 10; i++)
	{
		m_nCreateEventItemIdx[i] = 0;
		m_nCreateEventItemCnt[i] = 0;
	}
}
#endif // CREATE_EVENT


#ifdef EVENT_NEW_MOONSTONE
CMoonStoneRewardData::CMoonStoneRewardData():
m_listReward(CompRewardByIndex)
{
}

CMoonStoneRewardData::~CMoonStoneRewardData()
{
	while(m_listReward.GetCount() > 0)
	{
		REWARD* reward = m_listReward.GetData(m_listReward.GetHead());
		delete reward;
		m_listReward.Remove(m_listReward.GetHead());
	}
}

bool CMoonStoneRewardData::Load(int idx)
{
	CLCString sql(1024);
	sql.Format("SELECT * FROM t_moonstone_reward WHERE a_type = %d ORDER BY a_giftindex", idx);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbdata);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;

	m_nMoonStoneIndex = idx;
	m_nTotalProb = 0;
	int giftindex, giftcount, giftflag;
	float giftprob;
	while(cmd.MoveNext())
	{
		REWARD* reward = new REWARD;
		
		cmd.GetRec("a_giftindex", giftindex);
		cmd.GetRec("a_giftcount", giftcount);
		cmd.GetRec("a_giftflag", giftflag);
		cmd.GetRec("a_giftprob", giftprob);

		reward->rewardindex = giftindex;
		reward->rewardcount = giftcount;
		reward->prob		= giftprob;
		reward->rewardflag	= giftflag;

		m_listReward.AddToTail(reward);
		m_nTotalProb += (int)(giftprob * 10000.0);
	}

#ifdef WJKTEST
#endif // WJKTEST
	return true;
}

CMoonStoneReward::CMoonStoneReward()
{
	m_pReward = NULL;
	m_nRewardDataCount = 0;
}

CMoonStoneReward::~CMoonStoneReward()
{
	if(m_pReward)
	{
		delete[] m_pReward;
		m_pReward = NULL;
	}

	m_nRewardDataCount = 0;
}

bool CMoonStoneReward::Load()
{
	CLCString sql(1024);
	sql.Format("SELECT distinct a_type FROM t_moonstone_reward ORDER BY a_type");
	CDBCmd cmd, cmddata;
	cmd.Init(&gserver.m_dbdata);
	cmddata.Init(&gserver.m_dbdata);

	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;

	if(cmd.m_nrecords == 0)
		return false;
	
	int i = 0;
	int type;
	m_nRewardDataCount = cmd.m_nrecords;
	m_pReward = new CMoonStoneRewardData[cmd.m_nrecords];
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_type", type);
		if(!m_pReward[i].Load(type))
			return false;
		i++;
	}
#ifdef WJKTEST

#endif // WJKTEST
	return true;
}

#endif // EVENT_NEW_MOONSTONE

#ifdef MONSTER_COMBO
void CServer::ProcMonsterCombo()
{
	int i;
	for(i = 0; i < m_comboZone->m_countArea; ++i)
	{
		if(m_comboZone->m_area[i].m_bEnable)
			m_comboZone->m_area[i].RunComboZone();
	}
}
#endif // MONSTER_COMBO

#ifdef EX_GO_ZONE
bool CServer::LoadTeleportZone()
{
	CLCString sql(1000);
	sql.Format("SELECT * FROM t_teleporter_zoneinfo ORDER BY a_npc_idx");

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbdata);
	cmd.SetQuery(sql);

	if(!cmd.Open())
		return false;

	int	npcidx;
	int	zone;
	int	extra;
	int regenx, regenz, ylayer;
	while(cmd.MoveNext())
	{
		if(  !cmd.GetRec("a_npc_idx", npcidx)
		    || !cmd.GetRec("a_zone", zone)
		    || !cmd.GetRec("a_extra", extra)
			|| !cmd.GetRec("a_npc_regen_x", regenx)
			|| !cmd.GetRec("a_npc_regen_z", regenz)
			|| !cmd.GetRec("a_npc_layer", ylayer))
		    return false;

		CTeleportZoneData* pTeleporter = new CTeleportZoneData(npcidx);
		pTeleporter->m_nExtra	= extra;
		pTeleporter->m_nZone	= zone;
		pTeleporter->m_regenX	= regenx;
		pTeleporter->m_regenZ	= regenz;
		pTeleporter->m_regenY	= ylayer;
		m_listTeleportZone.AddToTail(pTeleporter);
	}
	return true;
}


bool CTeleportTaxList::Load()
{
	CLCString sql(1000);
	sql.Format("SELECT * FROM t_teleport_tax ORDER BY a_zone, a_extra");

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbdata);
	cmd.SetQuery(sql);

	if(!cmd.Open())
		return false;

	m_nCount = cmd.m_nrecords;
	m_TeleportTax = new CTeleportTaxData[m_nCount];

	char dungeon;
	int zone, extra, tax;
	int i = 0;
	while(cmd.MoveNext())
	{
		if(   !cmd.GetRec("a_zone", zone)
			||!cmd.GetRec("a_extra", extra)
			||!cmd.GetRec("a_nas", tax)
			||!cmd.GetRec("a_dungeon", dungeon))
			return false;

		m_TeleportTax[i].m_nZone = zone;
		m_TeleportTax[i].m_nExtra = extra;
		m_TeleportTax[i].m_nNas = tax;
		m_TeleportTax[i].m_bDungeon = dungeon;

		i++;
	}

	return true;
}
#endif // EX_GO_ZONE