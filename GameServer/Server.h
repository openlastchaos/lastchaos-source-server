#ifndef __ASERVER_H__
#define __ASERVER_H__

#include "ConfigFile.h"
#include "Zone.h"
#include "DBThread.h"
#include "Item.h"
#include "Skill.h"
#include "Quest.h"
#include "Option.h"
#include "SSkill.h"
#include "Guild.h"
#include "Friend.h"
#include "RareOptionProto.h"
#include "MonsterCombo.h"
#include "ExtremeCube.h"
#include "PacketTime.h"
#include "Expedition.h"
#ifdef SET_ITEM
#include "SetItemProto.h"
#endif // SET_ITEM
#ifdef REQUITAL_EVENT	// 보상 이벤트 
#include "Requital.h"
#endif // REQUITAL_EVENT
#ifdef TRIGER_SYSTEM
#include "TrigerSet.h"
#endif // TRIGER_SYSTEM
#ifdef FACTORY_SYSTEM
#include "Factory.h"
#endif
#ifdef RAID
#include "InZone.h"
#endif //RAID 

#ifdef EVENTSETTING
typedef struct __tagEvent_Info
{
	int				nEventIndex;		// 이벤트 인덱스: 절대 +
	int				nEventData;			// (0 ~ 9999) 0: disable, +: 다양한 용도로 쓰임, 음수 절대 불가
	int				nEventExtra;		// 기타 정보
	bool			bEventNotice;		// 이벤트가 진행중인지 아닌지
} EVENT_INFO;

class CEventSetting
{
private:
	int				m_nCount;
	EVENT_INFO*		m_pEventSettingData;
	EVENT_INFO*		m_pEventSettingBackupData;

public:
	CEventSetting();
	~CEventSetting();
	int				GetEventSetting(int nEventIdx) const;
	int				GetEventExtra(int nEventIdx) const;
	int				GetEventCount() const	{ return m_nCount; }
	bool			IsEventEnable(int nEventIdx);
	bool			Load();
	bool			Load(CNetMsg& msg);
	bool			SetEventData(int nEventIdx, int nEventData, int nEventExtra, bool bEventNotice);
	void			Reset();
	void			Backup();
	void			DeleteBackupData();
	void			RollBack();
};
#endif // EVENTSETTING



typedef struct _tagGMIPData
{
	char	prefix[20];
	int		from;
	int		to;
} GMIPDATA;

class COXQuizData
{
	int				m_nIndex;			// 퀴즈 번호
	CLCString		m_strQuiz;			// 퀴즈 문장
	bool			m_bAnswerOX;		// true : O, false : X

public:
	COXQuizData(int nIndex, const char* strQuiz, bool bAnswerOX)
	: m_strQuiz(256)
	{
		m_nIndex = nIndex;
		m_strQuiz = strQuiz;
		m_bAnswerOX = bAnswerOX;
	}

	int GetQuizIndex() const					{ return m_nIndex; }
	const char* GetQuizString() const			{ return (const char*)m_strQuiz; }
	bool GetAnswer() const						{ return m_bAnswerOX; }

	static int CompOXData(COXQuizData* p1, COXQuizData* p2)
	{
		return p1->GetQuizIndex() - p2->GetQuizIndex();
	}
};

class CGoldenBallEventData
{
	int			m_nStatus;			// 현재 진행 상태

	CLCString	m_strTeam1;			// A팀 이름
	CLCString	m_strTeam2;			// B팀 이름
	int			m_nTeam1Score;		// A팀 점수
	int			m_nTeam2Score;		// B팀 점수

	int			m_nEndYear;			// 종료 시기 : 년
	int			m_nEndMonth;		// 종료 시기 : 월
	int			m_nEndDay;			// 종료 시기 : 일
	int			m_nEndHour;			// 종료 시기 : 시
	int			m_nEndMinute;		// 종료 시기 : 분

	time_t		m_tEndTime;			// 종료 시기 : time_t
	time_t		m_tVoteEndTime;		// 응모 마감 시각 : time_t

public:
	CGoldenBallEventData()
	: m_strTeam1(GOLDENBALL_TEAM_NAME_LENGTH + 1)
	, m_strTeam2(GOLDENBALL_TEAM_NAME_LENGTH + 1)
	{
		m_nStatus = GOLDENBALL_STATUS_NOTHING;

		m_nTeam1Score = 0;
		m_nTeam2Score = 0;

		m_nEndYear = 0;
		m_nEndMonth = 0;
		m_nEndDay = 0;
		m_nEndHour = 0;
		m_nEndMinute = 0;

		m_tEndTime = 0;
		m_tVoteEndTime = 0;
	}

	int GetStatus()					{ return m_nStatus; }

	const char* GetTeam1Name()		{ return m_strTeam1; }
	const char* GetTeam2Name()		{ return m_strTeam2; }
	int GetTeam1Score()				{ return m_nTeam1Score; }
	int GetTeam2Score()				{ return m_nTeam2Score; }

	int GetEndYear()				{ return m_nEndYear; }
	int GetEndMonth()				{ return m_nEndMonth; }
	int GetEndDay()					{ return m_nEndDay; }
	int GetEndHour()				{ return m_nEndHour; }
	int GetEndMinute()				{ return m_nEndMinute; }

	time_t GetEndTime()				{ return m_tEndTime; }
	time_t GetVoteEndTime()			{ return m_tVoteEndTime; }

	void Vote(const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMin)
	{
		m_nStatus = GOLDENBALL_STATUS_VOTE;

		m_strTeam1 = strTeam1;
		m_strTeam2 = strTeam2;
		m_nTeam1Score = 0;
		m_nTeam2Score = 0;

		m_nEndYear = nYear;
		m_nEndMonth = nMonth;
		m_nEndDay = nDay;
		m_nEndHour = nHour;
		m_nEndMinute = nMin;

		struct tm tmEndTime;
		memset(&tmEndTime, 0, sizeof(tmEndTime));
		tmEndTime.tm_year	= nYear - 1900;
		tmEndTime.tm_mon	= nMonth - 1;
		tmEndTime.tm_mday	= nDay;
		tmEndTime.tm_hour	= nHour;
		tmEndTime.tm_min	= nMin;
		m_tEndTime = mktime(&tmEndTime);
		m_tVoteEndTime = m_tEndTime;
	}

	void Gift(const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMin, time_t timeEndVote)
	{
		m_nStatus = GOLDENBALL_STATUS_GIFT;

		m_strTeam1 = strTeam1;
		m_strTeam2 = strTeam2;
		m_nTeam1Score = nTeam1Score;
		m_nTeam2Score = nTeam2Score;

		m_nEndYear = nYear;
		m_nEndMonth = nMonth;
		m_nEndDay = nDay;
		m_nEndHour = nHour;
		m_nEndMinute = nMin;

		struct tm tmEndTime;
		memset(&tmEndTime, 0, sizeof(tmEndTime));
		tmEndTime.tm_year	= nYear - 1900;
		tmEndTime.tm_mon	= nMonth - 1;
		tmEndTime.tm_mday	= nDay;
		tmEndTime.tm_hour	= nHour;
		tmEndTime.tm_min	= nMin;
		m_tEndTime = mktime(&tmEndTime);
		m_tVoteEndTime = timeEndVote;
	}

	void EndVote()
	{
		m_nStatus = GOLDENBALL_STATUS_VOTE_END;
	}

	void End()
	{
		m_nStatus = GOLDENBALL_STATUS_NOTHING;
	}
};

#ifdef EVENT_OPEN_ADULT_SERVER
class CItemUpgradeData
{
public:
	unsigned short		m_item_idx;
	unsigned short		m_exchange_item_idx;
};

class CItemUpgradeList
{
	CItemUpgradeData*	m_pItemUpgradeData;
	int					m_nCount;

public:
	CItemUpgradeList()
	{
		m_pItemUpgradeData = NULL;
		m_nCount = 0;
	};
	~CItemUpgradeList()
	{
		if(m_pItemUpgradeData)
		{
			delete [] m_pItemUpgradeData;
		}
	};
	static int CompIndex(const void* p1, const void* p2)
	{
		CItemUpgradeData* itemdata1 = (CItemUpgradeData*)p1;
		CItemUpgradeData* itemdata2 = (CItemUpgradeData*)p2;

		return itemdata1->m_item_idx - itemdata2->m_item_idx;
	}

	int					GetCount() { return m_nCount; }
	CItemUpgradeData*	FindIndex(unsigned short index)
	{
		CItemUpgradeData item;
		item.m_item_idx = index;
		item.m_exchange_item_idx = 0;

		return (CItemUpgradeData*)bsearch(&item, m_pItemUpgradeData, m_nCount, sizeof(CItemUpgradeData), CompIndex);
	};
	bool	Load();
};
#endif // EVENT_OPEN_ADULT_SERVER

#ifdef MESSENGER_NEW
class CChatGroup
{
	int m_makeCharIndex;
	int m_chatIndex;

public:

	CLCList<CLCString> m_charNameList;

	CChatGroup(int makeCharIndex, int chatIndex);
	~CChatGroup();

	int GetMakeCharIndex()  { return m_makeCharIndex; }
	int GetChatIndex()		{ return m_chatIndex; }

	void SendGroup(MSG_EX_MESSENGER_TYPE subtype, int chatColor, CLCString charName, CLCString chat);
#ifdef MSG_VER2
	bool CheckSameName(CLCString charName);
#endif // MSG_VER2
	static int CompByIndex(CChatGroup* p1, CChatGroup* p2)
	{
		return p1->m_chatIndex - p2->m_chatIndex;
	}
};
#endif // MESSENGER_NEW

#ifdef EX_GO_ZONE
class CTeleportZoneData
{
public:
	int		m_nNpcIdx;
	int		m_nZone;
	int		m_nExtra;
	int		m_regenX;
	int		m_regenY;
	int		m_regenZ;

	CTeleportZoneData::CTeleportZoneData(int npcidx)
	{
		
		m_nNpcIdx	= npcidx;
		m_nZone		= 0;
		m_nExtra	= 0;
		m_regenX	= 0;
		m_regenY	= 0;
		m_regenZ	= 0;
	}

	int	GetNPCIndex()	{ return m_nNpcIdx; }
	int	GetZoneIndex()	{ return m_nZone;	}
	int	GetExtra()		{ return m_nExtra; } 
	int GetRegenX()	{ return m_regenX; }
	int GetRegenY()	{ return m_regenY;}
	int GetRegenZ()	{ return m_regenZ;}

	static int	CompTeleporter(CTeleportZoneData* p1, CTeleportZoneData* p2)
	{
		return p1->GetNPCIndex() - p2->GetNPCIndex();
	}
};


class CTeleportTaxData
{
public:
	int		m_nZone;
	int		m_nExtra;
	int		m_nNas;
	char	m_bDungeon;

	CTeleportTaxData()
	{
		m_nZone		= 0;
		m_nExtra	= 0;
		m_nNas		= 0;
		m_bDungeon	= 0;
	}

	int		GetZoneIndex(){ return m_nZone; }
	int		GetExtra(){ return m_nExtra; }
	int		GetTax() { return m_nNas < 0 ? 0 : m_nNas; }
	char	IsDungeon() { return m_bDungeon; }
};

class CTeleportTaxList
{
	CTeleportTaxData*	m_TeleportTax;
	int					m_nCount;

public:
	CTeleportTaxList()
	{
		m_TeleportTax = NULL;
		m_nCount = 0;
	}
	~CTeleportTaxList()
	{
		if(m_TeleportTax)
		{
			delete [] m_TeleportTax;
			m_TeleportTax = NULL;
		}
	}

	int					GetCount() { return m_nCount; }
	static int CompIndex(const void* p1, const void* p2)
	{
		
		CTeleportTaxData* taxdata1 = (CTeleportTaxData*)p1;
		CTeleportTaxData* taxdata2 = (CTeleportTaxData*)p2;

		if(taxdata1 == taxdata2)
			return 0;
		if(taxdata1 == NULL)
			return 1;
		if(taxdata2 == NULL)
			return -1;

		int ret;
		ret = taxdata1->GetZoneIndex() - taxdata2->GetZoneIndex();
		if(ret != 0)
			return ret < 0 ? -1 : 1;

		ret = taxdata1->GetExtra() - taxdata2->GetExtra();
		if(ret != 0)
			return ret < 0 ? -1 : 1;

		return 0;
	}

	CTeleportTaxData* FindTeleportTax(int zone, int extra)
	{
		int i;
		for(i = 0; i < m_nCount ; ++i)
		{
			if(m_TeleportTax && m_TeleportTax[i].m_nZone == zone && m_TeleportTax[i].m_nExtra == extra)
			{
				return &m_TeleportTax[i];
			}
		}
		return NULL;
	}

	bool	Load();

};
#endif // EX_GO_ZONE


#ifdef EVENT_NEW_MOONSTONE


class CMoonStoneRewardData
{
public:
	typedef struct __reward
	{
		int		rewardindex;
		int		rewardcount;
		int		rewardflag;
		float	prob;
	}REWARD;

	CMoonStoneRewardData();
	~CMoonStoneRewardData();

	int	m_nMoonStoneIndex;
	int	m_nTotalProb;

	static int CompRewardByIndex(REWARD* r1, REWARD* r2)
	{
		return r1->rewardindex - r2->rewardindex;
	}

	CLCList<REWARD*>		m_listReward;

	bool Load(int idx);
};

class CMoonStoneReward
{
public:
	CMoonStoneReward();
	~CMoonStoneReward();

	int	m_nRewardDataCount;
	CMoonStoneRewardData* m_pReward;

	bool	Load();
	static int	Comp(const void* p1, const void *p2)
	{
		return ((CMoonStoneRewardData*)p1)->m_nMoonStoneIndex - ((CMoonStoneRewardData*)p2)->m_nMoonStoneIndex;
	}
	CMoonStoneRewardData*	Find(int moonstoneidx)
	{
		CMoonStoneRewardData data;
		data.m_nMoonStoneIndex = moonstoneidx;
		return (CMoonStoneRewardData*)bsearch(&data, m_pReward, m_nRewardDataCount, sizeof(CMoonStoneRewardData), Comp);
	}
};
#endif // EVENT_NEW_MOONSTONE

class CServer
{
public:
	CServer();
	~CServer();

	////////////
	// 소켓 관련
	socket_t	m_ssock;			// 서버 소켓
	CDescriptor* m_desclist;		// 연결된 소켓 리스트
	int			m_nDesc;			// 리스트의 소켓 개수
	CDescriptor* m_messenger;		// 메신저 연결
	CDescriptor* m_connector;		// 빌링 서버 연결
	CDescriptor* m_helper;			// 헬퍼 연결
	CPCList		m_playerList;		// 플레이어 리스트
	fd_set		m_input_set;		// 디스크립터 셋
	fd_set		m_output_set;
	fd_set		m_exc_set;
	fd_set		m_null_set;

#ifdef GUILD_MARK_TABLE
	int m_nGuildMarkTable[3];
#endif // GUILD_MARK_TABLE

#ifdef LC_TLD
	struct tm m_now;
#endif
#ifdef CRYPT_NET_MSG
	unsigned char pDecryptBuffer[CNM_TEMP_BUFFER_LENGTH];
	unsigned char pTmpDecryptBuffer[CNM_TEMP_BUFFER_LENGTH];
#endif // CRYPT_NET_MSG
	////////////
	// 서버 설정
	char*		m_serverpath;		// 모듈 경로
	CConfigFile	m_config;			// 서버 설정

	int			m_national;			// 국가 
	int			m_serverno;			// 서버 번호
	int			m_subno;			// 서브 번호
	CLCString	m_serverAddr;
	int			m_serverPort;

	int			m_maxplayers;		// 허용 가능 최대 플레이어 수
	bool		m_bReboot;			// 리부트 자동 재시작 
	bool		m_nameserverisslow; // 왜 쓰는 것일까?
	bool		m_bShutdown;			// 풩好?
	TIMESTRUCT	m_serverTime;		// 서버 시간
	int			m_pulseReboot;		// 리부트까지 남은 시간
	int			m_pulseShutdown;	// 셧다운까지 남은 시간
	int			m_pulseEndGame;		// 전체 서버 다운까지 남은 시간
	int			m_clientversionMin;	// 클라이언트 버전
	int			m_clientversionMax;	// 클라이언트 버전
	bool		m_bOnlyLocal;		// 외부 접속 허용 여부
	int			m_pulseProcHeartBeatPerSec;		// HeartBeat()에서 매초 처리를 위한 변수
#ifdef ENABLE_STATISTICS
	LONGLONG	m_statisticsItemBuy;
	LONGLONG	m_statisticsItemSell;
	LONGLONG	m_statistics152;
	LONGLONG	m_statistics153;
	LONGLONG	m_statistics154;
	LONGLONG	m_statistics155;
	LONGLONG	m_statistics156;
	LONGLONG	m_statistics157;
	LONGLONG	m_statistics158;
	LONGLONG	m_statistics159;
	LONGLONG	m_statistics160;
	LONGLONG	m_statistics161;
	LONGLONG	m_statistics162;
	LONGLONG	m_statistics163;
	LONGLONG	m_statistics197;
	LONGLONG	m_statistics198;
	LONGLONG	m_statistics199;
	int			m_statisticsPulse;
#define STATISTICS(x, v)		gserver.m_statistics##x += (v)
#endif // #ifdef ENABLE_STATISTICS

#ifdef ENABLE_WAR_CASTLE
#ifdef TLD_WAR_TEST
	bool		m_warnotice;
#endif
	LONGLONG	m_taxItem;			// 성주에게 줄 세금 : 상점 판매,구매
	LONGLONG	m_taxProduceCastle;	// 성주에게 줄 세금 : 생산
	int			m_taxSavePulse;		// 세금 갱신 주기 : 성 정보 갱신
	int			m_taxDivGuild;		// 길드에 세금 전달 했는지 여부 : 전달 후 해당 요일을 설정, -1이면 전달 한적 없는 상태
#endif // #ifdef ENABLE_WAR_CASTLE
	
#ifdef DRATAN_CASTLE
	LONGLONG	m_taxItemDratan;			// 성주에게 줄 세금 : 상점 판매,구매, 공성 입장료
	LONGLONG	m_taxProduceCastleDratan;	// 성주에게 줄 세금 : 생산
#endif // DRATAN_CASTLE

	bool		m_bLoadPartyInfo;	// 헬퍼로부터 현재 파티 정보를 모두 받았는지 저장


	///////////////////
	// 운영자 명령 관련
#ifdef USING_GMIP_TABLE
	int				m_nGMIP;			// GM IP 설정 수
	GMIPDATA*		m_GMIP;				// GM IP 설정 테이블
#endif // USING_GMIP_TABLE
#ifdef JP_OTAKU_SYSTEM
	CLCString		m_strGMCommandOTAKUSpeedUp;		// 돌격
	CLCString		m_strGMCommandOTAKUImmortal;	// 무적
#endif // JP_OTAKU_SYSTEM

	struct timeval	m_tvLoop;			// 루프 시간 측정용
	int				m_nLoop;			// 루프 카운트
	int				m_bLoop;			// 측정 시작 여부
	CDescriptor*	m_descLoop;			// 요청 디스크립터
	CDescriptor*	m_observer;			// 옵저버
	CDescriptor*	m_chatmonitor[20];	// 채팅 모니터

#ifdef GMTOOL
	CDescriptor**	m_gmtool;
	int				m_nGMToolCount;
	void			GMToolCharPrePlay(CDescriptor* d);			// 지엠툴로 접속했을 때 
#endif // GMTOOL


	///////////////////
	// 전체 파티 리스트
	CLCList<CParty*>	m_listParty;	// 서버내 전체 파티 리스트

#ifdef EXPEDITION_RAID
	// 전체 원정대 리스트
	CLCList<CExpedition*>	m_listExped;	// 서버내 전체 원정대 리스트
#endif //EXPEDITION_RAID
	

#ifdef PARTY_MATCHING
	///////////////////
	// 파티 매칭 테이블
	CLCList<CPartyMatchMember*>		m_listPartyMatchMember;		// 일반 매칭
	CLCList<CPartyMatchParty*>		m_listPartyMatchParty;		// 파티 매칭
#endif // PARTY_MATCHING

	// 새로운 메신저 채팅방
#ifdef MESSENGER_NEW
	CLCList<CChatGroup*>			m_chatList;
	// 1 : 맴버만 추가 0 : 채팅방 생성 -1 : 실패
	int		addChatGroup(int& m_makeCharIndex, int& chatIndex,  CLCString charName);
	void	delChatGroup(int m_makeCharIndex, int chatIndex, CLCString charName);
	void	SendChatGroup(MSG_EX_MESSENGER_TYPE subtype, int m_makeCharIndex, int chatIndex, int chatColor, CLCString charName, CLCString chat);
	void*	FindChatGroup(int makeCharIndex, int chatIndex);
	void	discCharChatGroup(CLCString charName);
	int		GetMaxChatIndexPerPC(int makeCharIndex);

#endif // MESSENGER_NEW

	///////////
	// 아이템 관련
	CItemList			m_itemProtoList;		// 전체 아이템 리스트

#ifdef FEEITEM
	CCatalogList m_catalogList;
#ifdef LIMIT_CATALOG
	CLCList<int> m_limitCT;
#endif // LIMIT_CATALOG
#endif // FEEITEM

#ifdef SET_ITEM
	CSetItemProtoList	m_setItemProtoList;		// 세트 아이템 리스트
#endif // SET_ITEM

#ifdef FACTORY_SYSTEM
	CFactoryProtoList	m_factoryItemProtoList;		// 제작 아이템 리스트
#endif

	//////////
	// 틱 관련
	int			m_pulse;			// 틱 카운터
	int			m_gameTime;			// 게임 시간
#ifdef LC_TIME
	int			m_lcStartTime;		// 게임 시간
#endif
	int			m_resetWeather;		// 날씨 변화 : 현재 30분
	int			m_resetAdjustItem;	// Adjust Item 로그 출력 : 1분

	//////////
	// DB 연결
	MYSQL		m_dbchar;			// 캐릭터 데이터
	MYSQL		m_dbdata;			// 게임 데이터
#ifdef ENABLE_WAR_CASTLE
	MYSQL		m_dbcastle;			// 공성 데이터
#endif
#ifdef USE_TENTER_BILLING
	MYSQL		m_dbcatal;			// 게임 카탈로그 디비
#endif

	//////////////
	// 쓰레드 관련
	CDBThread	m_dbthread;			// DB 쓰레드
	bool		m_brunconnector;		// 빌링 수행 플래그
	bool		m_brunmessenger;	// 메신저 수행 플래그
	bool		m_brunhelper;		// 헬퍼 수행 플래그

	// 메신저 관련
	CNetMsg		m_messengerConnMsg;	// 메신저 연결 메시지

	// 컨넥터 관련
	CNetMsg		m_connectorConnMsg;	// 컨넥터 연결 메시지

	// 헬퍼 접속 메시지
	CNetMsg		m_helperConnMsg;

	//////////
	// 존 관련
	int			m_numZone;			// 존의 개수
	CZone*		m_zones;			// 존 배열
	
#ifdef RAID
	//////////
	// 인존 관련
	int			m_numInZone;		// 존의 개수
	CInZone*	m_inzones;			// 존 배열
#endif //RAID

	//////////////
	// NPC 관련
	CNPCProtoList m_npcProtoList;	// NPC 기본 리스트

	////////////
	// 스킬 관련
	CMagicProtoList		m_magicProtoList;
	CSkillProtoList		m_skillProtoList;

	//////////////
	// Quest 관련
	CQuestProtoList m_questProtoList;	// Quest Proto List

	//////////////
	// Option 관련
	COptionProtoList m_optionProtoList;	// Option Proto List
#ifdef MONSTER_RAID_SYSTEM
	CRareOptionProtoList m_rareOptionList; // 레어 옵션
#endif // MONSTER_RAID_SYSTEM

	/////////////////////
	// Special Skill 관련
	CSSkillProtoList m_sSkillProtoList;	// Special Skill Proto List

	/////////////
	// 길드리스트
	CGuildList		m_guildlist;
#ifdef GUILD_RANKING
	// 길드 랭킹용 경험치 업데이트 주기
	int				m_nPulseSaveExpGuild;
#endif // GUILD_RANKING

#ifdef EVENTSETTING
	CEventSetting	m_EventSetting;
#endif // EVENTSETTING

#ifdef EVENT_OPEN_ADULT_SERVER
	CItemUpgradeList	m_ItemUpgradeList;
#endif // EVENT_OPEN_ADULT_SERVER

#ifdef ATTACK_PET		// 공격형 펫 시스템
	CAPetProtoList	m_pApetlist;
#endif // ATTACK_PET

#ifdef REQUITAL_EVENT	// 보상 이벤트 
	CRequital_EventList m_CRequital_EventList;
#endif // REQUITAL_EVENT

#ifdef TRIGER_SYSTEM
	CTriger_List	m_CTriger_List;
#endif // TRIGER_SYSTEM

	//////////////
	// 이벤트
	// 050120 : 경험치, 나스, SP, 생산품 2배 이벤트
	bool		m_bDoubleEvent;
#ifdef NEW_DOUBLE_GM_ZONE
	int		m_bDoubleEventZone;
#endif // NEW_DOUBLE_GM_ZONE

#ifdef EVENT_DROPITEM
	bool	m_bDropItem;
	int		m_bDropItemNpcIdx;
	int		m_bDropItemIdx;
	int		m_bDropProb;
#endif // EVENT_DROPITEM

#ifdef UPGRADE_EVENT
	bool    m_bUpgradeEvent;
	int		m_nUpgradeProb;
#endif // UPGRADE_EVENT

#ifdef CREATE_EVENT
	bool m_bCreateEvent;
	int	m_nCreateTableIndex;
	int m_nCreateEventStartTime;
	int m_nCreateEventEndTime;
	int m_nCreateEventItemIdx[10];
	int m_nCreateEventItemCnt[10];

	bool LoadCreateEvent();
	void ClearCreateEvent();
#endif // CREATE_EVENT

#ifdef CHANCE_EVENT
	// 찬스 이벤트: 특정 레벨대만 혜택
	bool		m_bChanceEvent;
	int			m_bChanceSlevel;
	int			m_bChanceElevel;
	int			m_bChanceNasPercent;
	int			m_bChanceNasGetPercent;
	int			m_bChanceExpPercent;
	int			m_bChanceSpPercent;
	int			m_bChanceProducePercent;
	int			m_bChanceProduceNum;
	bool		CheckChanceEventLevel(int level); 
#endif // CHANCE_EVENT

#ifdef NEW_DOUBLE_GM
	int			m_bDoubleNasPercent;
	int			m_bDoubleNasGetPercent;
	int			m_bDoubleExpPercent;
	int			m_bDoubleSpPercent;
	int			m_bDoubleProducePercent;
	int			m_bDoubleProduceNum;
#endif
#ifdef EVENT_COLLECT_BUG_DROP
	int			m_CollectBugEventPercent;
#endif
#ifdef EVENT_HALLOWEEN_2006
	int			m_HalloweenEventPercent;
#endif

	bool		m_bDoubleExpEvent;
	int			m_expPercent;
	bool		m_bLattoEvent;
	bool		m_bNewYearEvent;
	bool		m_bValentineEvent;
	bool		m_bWhiteDayEvent;
#ifdef EVENT_RICESOUP
	bool		m_bRiceShoupEvent;
#endif
#ifdef EVENT_LETTER
	bool		m_bLetterEvent;
#endif

#ifdef EVENT_XMAS_2005
	bool		m_bXMas2005Event;
#endif // #ifdef EVENT_XMAS_2005
#ifdef 	EVENT_NEWYEAR_2006
	bool		m_bNewYear2006Event;
#endif // #ifdef 	EVENT_NEWYEAR_2006

#ifdef 	EVENT_SEARCHFRIEND //휴면 케릭
	bool		m_bSearchFriendEvent;
#endif //#ifdef 	EVENT_SEARCHFRIEND

#ifdef EVENT_OPEN_BETA_TLD
	bool		m_bOpenEvent;
	int			m_bSoilDrop;
#endif
#ifdef EVENT_FLOWER
	bool		m_bFlowerEvent;
	int			m_bFlower; // 0 : all 1 : 방어력 2 : 공격력 3: 낮은 방어력 4: 낮은 공격력 5: 높은 방어력 6: 높은 공격력
#endif
#ifdef MARGADUM_PVP
	bool		m_bMargadumPvp;
	int			m_bMargadumPvpLevel; // 0 : all 1 : 방어력 2 : 공격력 3: 낮은 방어력 4: 낮은 공격력 5: 높은 방어력 6: 높은 공격력
#endif
#ifdef EVENT_PROMOTION_SITE
	bool		m_bpSiteGive;
	int			m_bpSiteCount;
#endif 

	// 아이템, 나스 드롭율 조정 변수
	int			m_itemDropProb;
	int			m_moneyDropProb;
//0704
	int			m_aNotice[5]; //이벤트에 관한 공지.

#if defined EVENT_MOONSTONE
	int			m_nMoonStoneNas;
	bool		m_bMoonStoneEvent;
#elif defined EVENT_NEW_MOONSTONE
	int			m_nMoonStoneNas;
#endif // EVENT_MOONSTONE

	int			m_nItemDropEventRate;

#ifdef EVENT_VALENTINE_2006
	bool		m_bEventValentine2006;		// 발렌타인 이벤트
#endif // #ifdef EVENT_VALENTINE_2006

#ifdef EVENT_JPN_2007_NEWSERVER
	int			m_bEventJPN2007NewServer;	// 일본 2007년 3월 신규 서버 오픈 접속자 이벤트
#endif // EVENT_JPN_2007_NEWSERVER

#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
	int			m_bEventJPN2007TreasureboxAddItem;	// 일본 2007년 신규 서버 오픈시 보물상자 이벤트 추가 아이템 추가
#endif //EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

#ifdef ENABLE_OXQUIZ
	bool		m_bEventOX;					// OX 퀴즈 활성화, start에서 true가 되고, end에서 false
	time_t		m_timeEventOXStart;			// OX 퀴즈 이벤트 시작 시간
											// m_bEventOX가 true이고 m_timeEventOXStart 이전이면 입장 가능
	CLCList<COXQuizData*> m_listOXQuiz;		// 퀴즈 리스트
	int			m_nOXQuizSeqNo;				// 퀴즈 순차 번호, start에서 1로 되고 매 문제마다 1씩 증가한다
	bool		m_bOXQuizNoticeStart;		// 퀴즈 시작 알림 여부, start에서 false되고 시작 알리고 나서 true 됨
	time_t		m_timeOXQuizLimit;			// 퀴즈 제한 시간
	int			m_nOXQuizRealNo;			// 현재 진행중인 퀴즈 번호, 없으면 -1
	int			m_nOXQuizGMCharIndex;		// 현재 퀴즈를 낸 GM
#endif // ENABLE_OXQUIZ

#ifdef RECOMMEND_SERVER_SYSTEM
	bool		m_bRecommend;				// 추천 서버 설정 여부
	int			m_pulseRecommendMoonstone;	// 추천 서버 문스톤 지급 시간
#endif // RECOMMEND_SERVER_SYSTEM


#ifdef NON_PK_SYSTEM
	bool		m_bNonPK;
#endif	// NON_PK_SYSTEM

#ifdef FREE_PK_SYSTEM
	bool		m_bFreePk;
#endif

#ifdef MAL_DISABLE_PKPENALTY
	bool		m_bDisablePKPaenalty;	
#endif // MAL_DISABLE_PKPENALTY

#ifdef USING_NPROTECT
	int			m_pulseUpdateGG;				// nProtect GameGuard 모듈 업데이트 주기
#endif // USING_NPROTECT

#ifdef NEW_GUILD
	// 길드 랭킹용 경험치 업데이트 주기
	int			m_nPulseSaveGuildPoint;
#endif // NEW_GUILD

#ifdef  DOUBLE_ITEM_DROP
	bool		m_bDoubleItemEvent;
	int			m_bDoubleItemPercent;

#endif // DOUBLE_ITEM_DROP

#ifdef DOUBLE_PET_EXP
	bool		m_bDoublePetExpEvent;
	int			m_PetExpPercent;
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
	bool		m_bDoubleAttackEvent;
	int			m_AttackPercent;
#endif // DOUBLE_ATTACK

#ifdef NEW_DOUBLE_EVENT_AUTO
	bool		m_bDoubleEventAuto;
	bool		m_bDoubleEventAutoOn;
#endif // NEW_DOUBLE_EVENT_AUTO
	
#ifdef DRATAN_CASTLE
	int m_nDVDCharAttackRate;
	int m_nDVDCharDefanseRate;
	int m_nDVDCharMoveSpeedRate;
#endif // DRATAN_CASTLE

#ifdef EVENT_XMAS_2007
	unsigned int m_unEventXmasTreePoint;
#endif //EVENT_XMAS_2007

#ifndef LIMIT_EXP_INCREASE
	int m_nExpLimit;
#endif // LIMIT_EXP_INCREASE

	CGoldenBallEventData	m_clGoldenBall;

#ifdef ALTERNATE_MERCHANT
	CAlternateMerchantList	m_AlternateMerchantList;
#endif // ALTERNATE_MERCHANT

#ifdef EX_GO_ZONE
	CLCList<CTeleportZoneData*> m_listTeleportZone;
	CTeleportTaxList	m_TeleportTaxList;
	bool LoadTeleportZone();
#endif 

	bool DropProbLoad();

	/////////////////
	// 서버 환경 관련
	char* GetServerPath();			// 경로 얻기
	bool LoadSettingFile();			// 설정 파일 읽기
	bool LoadSettings();			// 설정 읽기
	bool ConnectDB();				// DB 연결
	void DisconnectDB(bool bchar);	// DB 연결 끊기, bchar가 false이면 user와 data, true이면 char
	bool CreateDBThread();			// DB 스레드 생성
	bool InitGame();				// 게임 초기화
	int GetMaxPlayers();			// 허용 가능 최대 플레이어 수 구하기
	int GetServerTime();		// 현재 시간 대비 서버시간 설정
	int FindZone(int zone);			// 존 찾기
	// zone의 x, z의 위치에서 가까운 마을 찾기
	int FindNearestZone(int zone, float x, float z, int* nearZone, int* nearZonePos);

#ifdef EVENTSETTING
	bool LoadNotice();
#endif // EVENTSETTING
	///////////////////
	// 운영자 명령 관련
#ifdef USING_GMIP_TABLE
	void LoadGMIPTable();
#endif

	////////////
	// 빌링 관련
	bool MakeBillingClient();				// 빌링 클라이언트 생성
	void SetHeaders();						// 빌링/메신저 헤더 생성
	void SendBillingHeader();				// 빌링에 헤더 전달
	void ProcessBilling(CNetMsg& msg);		// 빌링 처리
#ifdef CIRCLE_WINDOWS
	static void BillingConnect(void* lParam);	// 빌링 연결 시도
#else
	static void* BillingConnect(void* lParam);
#endif


	//////////////
	// 메신저 관련
	bool MakeMessengerClient();				// 메신저 클라이언트 생성
	void SendMessengerHeader();				// 메신저 헤더 보냄
#ifdef CIRCLE_WINDOWS						// 메신저 클라이언트 생성 쓰레드
	static void MessengerConnect(void* lpvoid);
#else
	static void* MessengerConnect(void* lpvoid);
#endif
	void ProcessMessengerMessage(CNetMsg& msg);	// 메신저 메시지 처리
	void ProcessEventMessage(CNetMsg& msg);		// 이벤트 메시지 처리


	//////////////
	// 헬퍼 관련
	bool MakeHelperClient();					// 헬퍼 클라이언트 생성
	void SendHelperHeader();					// 헬퍼 헤더 보냄
#ifdef CIRCLE_WINDOWS							// 헬퍼 클라이언트 생성 쓰레드
	static void HelperConnect(void* lpvoid);
#else
	static void* HelperConnect(void* lpvoid);
#endif
	void ProcessHelperMessage(CNetMsg& msg);	// 헬퍼 메시지 처리


	/////////////////
	// 소켓 설정 관련
	socket_t InitSocket();								// 서버 소켓 초기화
	static int SetSendbuf(socket_t s);					// 송신 버퍼 크기 설정
	struct in_addr* GetBindAddr();						// 반인드 할 주소 선택
	static void Nonblock(socket_t s);					// 비블럭 소켓


	/////////////////
	// 서버 수행 관련
	void Run();				// 서버 수행
	void Close();			// 수행 종료
	static void ServerSleep(struct timeval* timeout);	// 서버 잠시 재우기
	void HeartBeat();		// 1초에 10번씩 매번 틱마다 수행되는 함수


	///////////////////////
	// 클라이언트 소켓 관련
	int NewDescriptor(int s);					// 새로운 클라이언트 받음
	void CloseSocket(CDescriptor* d, bool bForce);		// 디스크립터 종료, bForce: 15초 딜레이 무시 여부
	void SendOutput(CDescriptor* d);			// 데이터 보내기
	void SendOutput(CCharacter* ch);
	bool DisConnectLogedIn(CDescriptor* my_d, bool bForce);	// 연결 끊어짐 처리
	CDescriptor* FindConnectIdName(const char* idname, CDescriptor* md); // 접속자에서 id 찾기
	CDescriptor* FindUser(int userindex);		// 접속자에서 userindex 찾기


	/////////////////
	// 명령 처리 관련
	void CommandInterpreter(CDescriptor* d, CNetMsg& msg); // 명렁 처리


	//////////////
	// 캐릭터 관련
	void CharPrePlay(CDescriptor* d);			// 캐릭터 준비
	void DecreaseTimeForPC(CPC* ch);			// 캐릭터 관련 시간 감소

	void MoonStoneEndProcess(CPC* ch);

#ifdef ENABLE_WAR_CASTLE
	// 세액 증가 : 판매/구매
	void AddTaxItem(LONGLONG val)			{ if (val > 0 && m_taxItem + val > 0) m_taxItem += val; }
	// 세액 증가 : 생산
	void AddTaxProduceCastle(LONGLONG val)	{ if (val > 0 && m_taxProduceCastle + val > 0) m_taxProduceCastle += val; }
	// 세액 초기화
	void ResetTax()							{ m_taxItem = m_taxProduceCastle = 0;}
#ifdef DRATAN_CASTLE
	void ResetTaxDratan()					{ m_taxItemDratan = m_taxProduceCastleDratan = 0;}
	// 세액 증가 : 판매/구매
	void AddTaxItemDratan(LONGLONG val)			{ if (val > 0 && m_taxItemDratan + val > 0) m_taxItemDratan += val; }
	// 세액 증가 : 생산
	void AddTaxProduceCastleDratan(LONGLONG val)	{ if (val > 0 && m_taxProduceCastleDratan + val > 0) m_taxProduceCastleDratan += val; }
#endif // DRATAN_CASTLE
	// 세액 -> 성정보
	void SaveTax();
	// 세금 -> 길드
	void DivideTax();
	// 세금 받을 길드 변경
	void ChangeTaxGuild();
#endif // #ifdef ENABLE_WAR_CASTLE

	// 파티 리스트 노드 찾기 : 보스 인덱스 사용
	CParty* FindPartyByBossIndex(int nBossIndex);

	// 파티 리스트 노드 찾기 : 멤버 인덱스 사용
	CParty* FindPartyByMemberIndex(int nMemberIndex, bool bIncludeRequest);

#ifdef EXPEDITION_RAID
	// 파티 리스트 노드 찾기 : 보스 인덱스 사용
	CExpedition* FindExpedByBossIndex(int nBossIndex);

	// 파티 리스트 노드 찾기 : 멤버 인덱스 사용
	CExpedition* FindExpedByMemberIndex(int nMemberIndex, bool bIncludeRequest);
#endif //EXPEDITION_RAID

	
#ifdef PARTY_MATCHING
	CPartyMatchMember* FindPartyMatchMemberByCharIndex(int nCharIndex);
	CPartyMatchParty* FindPartyMatchPartyByBossIndex(int nBossIndex);
#endif // PARTY_MATCHING

#ifdef ENABLE_OXQUIZ
	// OX Quiz Loading
	bool LoadOXQuiz();
#endif // ENABLE_OXQUIZ

#ifdef EVENT_NEW_MOONSTONE
	CMoonStoneReward m_moonstoneReward;
#endif // EVENT_NEW_MOONSTONE

private:

#ifdef ENABLE_WAR_CASTLE
	// 세금 처리
	void ProcTax();
#endif // #ifdef ENABLE_WAR_CASTLE

	// 몹, 아이템 컨트롤
	void ProcMobItemControl();

	// 날씨 변화
	void ChangeWeather();

	// loop count 처리
	void ProcLoopCount();

#ifdef ENABLE_STATISTICS
	// 상점 거래등 세금 자료 통계
	void ProcStatistics();
#endif // ENABLE_STATISTICS

#ifdef ENABLE_OXQUIZ
	// OX 퀴즈 처리
	void ProcOXQuiz();
#endif // ENABLE_OXQUIZ

#ifdef RECOMMEND_SERVER_SYSTEM
	// 추천서버 문스톤 처리
	void ProcRecommendServerMoonstone();
#endif // RECOMMEND_SERVER_SYSTEM

	// 골든볼 종료 처리
	void ProcGoldenBall();

#ifdef GUILD_RANKING
	// 길드 랭킹용 경험치 업데이트
	void ProcGuildRanking();
#endif // GUILD_RANKING

#ifdef MONSTER_COMBO
public:
	void ProcMonsterCombo();
	CZone* m_comboZone;
	CMIssionCaseList m_missionCaseList;
#endif // MONSTER_COMBO

#ifdef NEW_DOUBLE_EVENT_AUTO
	void NewDoubleEventAuto();
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef EXTREME_CUBE
	CExtremeCube m_extremeCube;
#endif // EXTREME_CUBE
};

extern CServer gserver;

#ifdef PACKET_TIME_CHECK
extern CPacketTime gPacketTime;
#endif

extern volatile LC_THREAD_T		gThreadIDGameThread;
extern volatile LC_THREAD_T		gThreadIDDBThread;

#endif
