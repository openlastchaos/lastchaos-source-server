#ifndef __ACHARACTER_H__
#define __ACHARACTER_H__

#include "MessageType.h"
#include "NPCProto.h"

#ifdef ATTACK_PET		// 공격형 펫 시스템
#include "APetProto.h"
#endif // ATTACK_PET

#include "NPCRegenInfo.h"
#include "Item.h"
#include "CPos.h"
#include "Inventory.h"
#include "ExchangeItems.h"
#include "MemPos.h"
#include "Party.h"
#include "Skill.h"
#include "Assist.h"
#include "Quest.h"
#include "SSkill.h"
#include "PersonalShop.h"
#include "Stash.h"
#include "NetMsg.h"
#include "Expedition.h"

#ifdef FACTORY_SYSTEM
#include "Factory.h"			// 제작 시스템
#endif


typedef struct __tagAutoGiveData
{
	int			nIndex;
	int			nPlus;
	int			nUsed;
	int			nFlag;
	LONGLONG	nCount;
	short		nOption[MAX_ITEM_OPTION];
	char		strSerial[MAX_SERIAL_LENGTH + 1];
} AUTO_GIVE_DATA;

typedef struct __tagBloodBugData
{
	int			nIndex;
	int			nFlag;
	int			nOptionCount;
	short		nOption[MAX_ITEM_OPTION];
	char		strSerial[MAX_SERIAL_LENGTH + 1];
} BLOOD_BUG_DATA;

#ifdef MONSTER_COMBO
typedef struct __tagMonsterComboData
{
	int			nIndex;
	int			nStep;
}MONSTER_COMBO_DATA;
#endif // MONSTER_COMBO

#ifdef FACTORY_SYSTEM
typedef struct __tagSealSkillData
{
	int			nSkillIdx;
	CSkill *    pSkill;
	LONGLONG	llExp;
}SEAL_SKILL_DATA;
#endif // FACTORY_SYSTEM

class CDescriptor;
class CZone;
class CArea;
class CNetMsg;
class CGuild;
class CGuildMember;
class CWarCastle;
class CPet;
class CElemental;

#ifdef ATTACK_PET
class CAPet;
#endif //ATTACK_PET

#ifdef ENABLE_MESSENGER
class CFriend;
class CFriendMember;
#endif

#ifdef ALTERNATE_MERCHANT
class CAlternateMerchant;
#endif // ALTERNATE_MERCHANT

extern void DelAttackList(CCharacter* ch);
extern void DelRaList(CPC* pc, bool bForce = false);

#ifdef MESSENGER_NEW
class CBlockPC
{
	int m_index;
	CLCString m_name;

public:
	CBlockPC(int index, const char* name) : m_name(MAX_CHAR_NAME_LENGTH +1) { m_index = index; m_name = name; }
	~CBlockPC() {};

	int GetIndex() { return m_index; }
	const char* GetName() { return m_name; }
	void SetName(const char* name) { m_name = name; }

	static int CompByIndex(CBlockPC* p1, CBlockPC* p2)
	{
		return p1->GetIndex() - p2->GetIndex();
	}
};
#endif

class CQuickSlot
{
public:
	CQuickSlot()
	{
		memset(m_slotType, -1, sizeof(int) * QUICKSLOT_MAXSLOT);
		memset(m_skillType, -1, sizeof(int) * QUICKSLOT_MAXSLOT);
		memset(m_actionType, -1, sizeof(int) * QUICKSLOT_MAXSLOT);
		memset(m_item, 0, sizeof(CItem*) * QUICKSLOT_MAXSLOT);
	}

	// 슬롯 타입 : 액션, 스킬, 아이템 중 하나
	int		m_slotType[QUICKSLOT_MAXSLOT];

	// 퀵슬롯 내용
	int		m_skillType[QUICKSLOT_MAXSLOT];	// 스킬 타입
	int		m_actionType[QUICKSLOT_MAXSLOT];// 액션 타입
	CItem*	m_item[QUICKSLOT_MAXSLOT];		// 아이템 종류
};


// Attack List Class
class CAttackChar
{
public:
	CAttackChar()
	{
		ch = NULL;
		pc = NULL;
		npc = NULL;
		
		m_next = NULL;
		m_prev = NULL;

		m_damage = 0;
		m_targetHate = 0;
		m_targetPulse = 0;

		m_bGiveExpSP = false;
#ifdef NEW_PK
		m_bFirstAttack = false;
#endif
	}
	~CAttackChar()
	{
	}

	// pc
	int			m_damage;		// 데미지
	int			m_targetHate;	// 타겟 수치 : NPC가 공격시 참조 값
								// 타겟을 놓아주 : 멀어지거나 공격안하면 줄어

	int			m_targetPulse;	// 타겟 펄스 : 3분동안 캐릭터가 때리지 않으면 어택리스트에서 삭제
	bool		m_bGiveExpSP;	// 경험치와 SP를 받았는가
#ifdef NEW_PK
	bool		m_bFirstAttack;	// ch가 먼저 때렸나?
#endif

	union
	{
		CCharacter*	ch;
		CPC*		pc;
		CNPC*		npc;
	};

	CAttackChar*	m_next;
	CAttackChar*	m_prev;

};

// 정당방위 List Class
class CRaChar
{
public:
	CRaChar()
	{
		m_raTarget = NULL;
		m_raPulse = 0;
		m_bAttacker = false;
	}
	~CRaChar()
	{
	}

	CPC*		m_raTarget;
	int			m_raPulse;
	bool		m_bAttacker;	// 가해자 : true 피해자 false

	CRaChar*	m_next;
	CRaChar*	m_prev;
};

//////////////////
// Character class
class CCharacter  
{
private:
	
#ifdef ADULT_SERVER_NEW_BALANCE
	float	m_nFinalHitrate;			// 계산된 명중율  ( 반드시 계산완료가 보장되었을 때 사용 )
	float	m_nFinalAvoidrate;			// 계산된 회피율  ( 반드시 계산완료가 보장되었을 때 사용 )
#endif // ADULT_SERVER_NEW_BALANCE

public:
	CCharacter();
	virtual ~CCharacter();

	MSG_CHAR_TYPE	m_type;		// 종류

	// NPC, PC 공통 데이터
	int			m_index;		// 고유번호
	CLCString	m_name;			// 이름
	int			m_level;		// 렙

	LONGLONG	m_exp;			// 경험치

	int			m_skillPoint;	// 스킬포인트

	int			m_hp;			// 체력
	int			m_maxHP;
	int			m_mp;			// 마나
	int			m_maxMP;

	int			m_str;			// 힘
	int			m_dex;			// 민첩
	int			m_int;			// 지혜
	int			m_con;			// 체질

	int			m_eqMelee;		// 무기 수치	: 근거리	: NUM0
	int			m_eqRange;		// 무기 수치	: 원거리	: NUM0 : IWEAPON_BOW
	int			m_eqMagic;		// 무기 수치	: 마법		: NUM1
	int			m_eqDefense;	// 방어구 수치	: 물리		: NUM0
	int			m_eqResist;		// 방어구 수치	: 마법		: NUM1

	char		m_attackSpeed;	// 공격속도 : 무기의 값에 따름 : 0.1초 단위 : 공격당 소요되는 시간 : +면 시간이 감소
	char		m_magicSpeed;	// 마법속도 : 마법 재사용 시간 줄고 늠: 0.1초 단위 : -면 시간이 감소
	int			m_skillSpeed;	// 스킬속도 : 스킬 시전 속도가 줄고 늠 
	int			m_statPall;
	// << 071226 kjban add
	int			m_statPall_per;
	// >>

	float		m_walkSpeed;	// 걷기 속도
	float		m_runSpeed;		// 뛰기 속도
#ifdef NIGHT_SHADOW
	float		m_flySpeed;		// 날기 속도
#endif //NIGHT_SHADOW
	float		m_attackRange;	// 공격거리

	int			m_recoverHP;	// 체력 회복
	int			m_recoverMP;	// 마나 회복
	int			m_recoverPulse;	// 상태회복 pulse : HP, MP

	CPos		m_pos;			// 좌표 (x, z, h, r, yLayer)

	int			m_cellX;		// 셀 위치
	int			m_cellZ;

	CAssist		m_assist;		// 보조효과
	ASSISTVALUE	m_avPassiveAddition;	// 패시브 효과 : add
	ASSISTVALUE	m_avPassiveRate;		// 패시브 효과 : rate

	CAttackChar* m_attackList;	//  어택 리스트

	CCharacter* m_pCellPrev;	// 셀 리스트 링크
	CCharacter* m_pCellNext;
	CZone*		m_pZone;		// 존
	CArea*		m_pArea;		// 영역

	bool		m_bVisible;		// 보이고 안보이고

	CAttributeData m_attribute;	// 속성값 : 스탯 계산시 초기화 -> 아이템 적용(PC) -> 스킬 적용 -> 최종값 저장(this data)
	CSkill*		m_currentSkill;		// 현재 사용중인 스킬
	CCharacter*	m_linkSource;	// 나한테 링크 건 캐릭
	CCharacter* m_linkTarget;	// 나가 링크 건 캐릭

	////////////////////////////////
	// Option Value
	int			m_opStr;			// 힘 증가
	int			m_opDex;			// 민첩 증가
	int			m_opInt;			// 지혜 증가
	int			m_opCon;			// 체질 증가

	int			m_opMelee;			// 근접 공격 증가
	int			m_opRange;			// 원격 공격 증가
	int			m_opMeleeHitRate;	// 근접 명중률 증가
	int			m_opRangeHitRate;	// 원격 명중률 증가

	int			m_opDMelee;			// 근접 공격 감소
	int			m_opDRange;			// 원격 공격 감소
	int			m_opMeleeAvoid;		// 근접 명중률 감소
	int			m_opRangeAvoid;		// 원격 명중률 감소

	int			m_opMagic;			// 마법 공격 증가
	int			m_opMagicHitRate;	// 마법 명중률 상승

	int			m_opResist;			// 마법 공격 감소
	int			m_opResistAvoid;	// 마법 명중률 감소

	// 공속 검사 관련
	int		m_hackAttackCount;		// 어택메세지 온 수 저장
	int		m_AttackServerTime;		// 공속에 따른 최대 어택 시간 
	int		m_AttackClientTime;		// 클라이언트가 보낸 어택시간
	int		m_lastAttackPulse;		// 최근 공격 메시지 온 시간

	int		m_pulseDisable;			// 캐릭터 행동 불능 시간 설정 : 설정된 시간 이후로 행동 가능

#ifdef NEW_ACCERY_ADD
	////////////////////////////////////
	// 악세사리 경험치 sp 추가율 
	LONGLONG	m_AddProb;
#endif //NEW_ACCERY_ADD
	int				cooltime_2142;			// 건강의 물약
	int				cooltime_2143;			// 지력의 물약

#ifdef COMPETITION_ITEM
	int		m_cooltime_Competition;	// 대전용 아이템 

#endif // COMPETITION_ITEM
	
#ifdef MOB_SCROLL
	int MobScroll;						// 몹 소환서 사용 여부
#endif // MOB_SCROLL
	
#ifdef NIGHT_SHADOW
	char	m_attacktype;
	char	m_attackcount;
	bool	ChekAttackType();			// 공격타입에따라서 스피드핵 검사 여부
#endif //NIGHT_SHADOW
	
	void Move(float tx, float tz, float th, float tr, int ylayer)	// 이동
	{
		m_pos = CPos(tx, tz, th, tr, ylayer);
	}

	void InitPointsToMax()		// hp, mp, ep, st를 최대치로 설정
	{
		m_hp = m_maxHP;
		m_mp = m_maxMP;
	}

	void SetVisible();			// 보이고 안보이는 상태를 토글시킴

	/////////////////////
	// Function name	: SendDisappearAllInCell
	// Description	    : 캐릭터 주변의 모든 캐릭터+아이템이 사라지도록 알림
	// Argument         : bool bSendOtherDisappear
	//                  : 다른 사람이 사라짐을 자신에게 알릴 것인지 여부
	void SendDisappearAllInCell(bool bSendOtherDisappear);


	////////////////////
	// Function name	: CanApplySkill
	// Description	    : 스킬 적용 가능 검사
	bool CanApplySkill(const CSkillProto* proto, const CSkillLevelProto* levelproto);


	////////////////////
	// Function name	: CanMove
	// Description	    : 이동 가능 검사
	virtual bool CanMove();

	////////////////////
	// Function name	: CanAttack
	// Description	    : 공격 가능 검사
	virtual bool CanAttack();

	////////////////////
	// Function name	: CanSpell
	// Description	    : 시전 가능 검사
	virtual bool CanSpell();


	////////////////////
	// Function name	: GetAttackLevel
	// Description	    : 무기레벨 얻기
	virtual int GetAttackLevel() const = 0;

	////////////////////
	// Function name	: GetDefenseLevel
	// Description	    : 방어구 레벨 얻기
	virtual int GetDefenseLevel() const = 0;

	////////////////////
	// Function name	: GetAttackType
	// Description	    : 공격 타입 얻기, 스킬 프로토가 있으면 스킬의 타입, NULL이면 기본 타입
	virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const = 0;


	////////////////////
	// Function name	: ApplyAssistData
	// Description	    : assist value를 적용
	void ApplyAssistData(ASSISTVALUE* add, ASSISTVALUE* rate);


	////////////////////
	// Function name	: IsInPeaceZone
	// Description	    : 피스존 안인지 검사, bBlockAsPeace이면 MAPATT_BLOCK을 피스존으로 간주
	bool IsInPeaceZone(bool bBlockAsPeace);

	////////////////////
	// Function name	: GetSize
	// Description	    : 캐릭터 크기 반환
	virtual float GetSize() = 0;


	////////////////////
	// Function name	: GetUsingStat
	// Description	    : 공격시 사용하는 스탯
	virtual int GetUsingStat() const = 0;

	bool IsInvisible() { return (m_assist.m_state & AST_INVISIBLE) ? true : false; }
	bool IsEnemy(CCharacter* tch);
	bool CanVisible(CCharacter* tch);

	void CancelInvisible();
	void CancelDamageLink();
#ifdef ENABLE_CHANGEJOB
	int GetDefenseLevelBonus() const { return (m_type == MSG_CHAR_PC) ? (m_level * 3) : 0; }
	virtual int GetResistLevelBonus() const { return (m_type == MSG_CHAR_PC) ? (m_level / 2) : 0; }
	int GetMeleeLevelBonus() const;
	int GetRangeLevelBonus() const;
	int GetMagicLevelBonus() const;
#endif

	bool IsBlind() { return (m_assist.m_state & AST_BLIND) ? true : false; }

	unsigned char GetMapAttr();

	virtual CSkill* FindSkill(int skillindex) = 0;

	// 패시브 스킬 적용
	virtual void ApplyPassive() = 0;
	void ApplyPassiveSkill(CSkill* skill, int param);

	// 스탯 재계산
	virtual void CalcStatus(bool bSend) = 0;

	// 공속 검사 : PC, 펫, Elemental
	// 핵으로 판단시 true
	bool CheckHackAttack(CPC* pPC);

#ifdef ADULT_SERVER_NEW_BALANCE
	virtual float GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type) = 0;
	virtual float GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type) = 0;
#endif // ADULT_SERVER_NEW_BALANCE

#ifdef ADULT_SERVER_NEW_BALANCE
	// 아래 함수는 회피 계산 후 사용 해야 합니다.  다른곳에 사용 금지...
	float	GetFinalHitRate() { return m_nFinalHitrate; }
	float	GetFinalAvoidRate() { return m_nFinalAvoidrate; }
	void	SetFinalHitRate( float FinalHitRate ) { m_nFinalHitrate = (FinalHitRate<0 ? 0:FinalHitRate); }
	void	SetFinalAvoidRate( float FinalAvoidRate ) { m_nFinalAvoidrate = (FinalAvoidRate<0 ? 0:FinalAvoidRate); }
#endif // ADULT_SERVER_NEW_BALANCE

	// disable 설정, 현재 설정이 더 오래가면 무시
	void SetDisableTime(int sec);
	// disable 상태 반환
	bool IsDisable();
};

///////////
// PC class
class CPC : public CCharacter
{
	int				m_state;			// pc 상태

#ifdef ENABLE_WAR_CASTLE
	int				m_joinMerac;		// 메라크 공성 참여여부
#endif // ENABLE_WAR_CASTLE

#ifdef DRATAN_CASTLE
	int				m_joinDratan;
#endif // DRATAN_CASTLE

	int		m_pulseDisable;				// 캐릭터 행동 불능 시간 설정 : 설정된 시간 이후로 행동 가능

public:
	CPC();
	virtual ~CPC();

	////////////
	// 기본 정보
	int		m_listIdx;			// 리스트에서 인덱스
	bool	m_bPlaying;			// 플레이 여부
	bool	m_bChangeStatus;	// SendStatus()를 할지 안할지 결정
	bool	m_bImmortal;		// 몹에게 공격 당하는가? 운영자 명령어로 셋팅
	bool	m_bChatMonitor;		// 채팅 모니터링 여부
	bool	m_bLoadChar;		// 캐릭터 로딩 여부
	CLCString m_nick;			// 별명(이름 변경카드)
	int		m_billReqTime;		// 빌링 아이템 지급 타임

#ifdef NO_CHATTING
	int		m_nflag;			// 캐릭터 flag - 채팅금지
#endif

#ifdef TRADE_AGENT
	bool			m_bTradeAgentRegIng;	//현재 등록 처리중인가(거래대행)
	bool			m_bTradeAgentCalcIng;	//현재 정산 처리중인가(거래대행)
	int				m_nCalcPageNo;			//정산(리스트)현재 페이지 번호
#endif	// TRADE_AGENT

	int		m_speedup;			// 운영자 명령어 : speedup
#ifdef RANKER_NOTICE
	bool	m_bRanker;			// 랭커 인가.  
#endif //RANKER_NOTICE
#ifdef ENABLE_SINGLEDUNGEON_DATA
	int		m_pd3Count;			// 퍼스널던전3 입장 수
	int		m_pd3Time;			// 퍼스널던전3 마지막 입장 시간
	int		m_pd4Count;			// 퍼스널던전4 입장 수
	int		m_pd4Time;			// 퍼스널던전4 마지막 입장 시간
#endif
	unsigned char m_recentAtt;	// 최근 위치 속성맵 값

	int		m_autoSkillTime;
	int		m_SkillTime_511;
#ifdef FEEITEM
	int		m_cashBalance;		// 보유 캐쉬
#endif // FEEITEM

#ifdef ETC_EVENT
	int m_etcEvent;
#endif
#ifdef MARGADUM_PVP
	LONGLONG	m_magadumDamage;	// 마르가둠 pvp시 등수를 가리기 위한 점수
#endif
#ifdef CASH_MOONSTONE
	int m_cashmoonStoneItemIndex;
	int m_cashmoonStoneRemoveCtId;
	char m_cashMonnStoneGrade;
	int m_cashMoonStoneGiveCtId;
#endif

#ifdef GMTOOL
	bool	m_bGmMonitor;			// 지엠툴 채팅 모니터링 여부
#endif // GMTOOL

//0707
	int		m_secretkey;
	/////////////////////////////////////////////
	// BANGWALL : 2005-06-27 오후 7:10:37
	// Comment : 
		// pd4 시작 타임
	int m_pd4StartTime;

	LONGLONG m_loseexp;			// 사망 경험치 저장
	LONGLONG m_losesp;			// 사망 sp 저장

#ifdef ENABLE_MESSENGER	
	CFriend* m_Friend;
	int m_nRegFriend;
	int m_nCondition;
#ifdef MESSENGER_NEW
	CLCList<CBlockPC*>	m_listBlockPC;
#endif
#endif

	////////////
	// 외양 정보
	char	m_job;				// 직업
#ifdef ENABLE_CHANGEJOB
	char	m_job2;				// 2차 직업
#endif
	char	m_hairstyle;		// 머리모양
								// 1의 자리 : 원래 헤러
								// 10의 자리 : 산타 모자
	char	m_facestyle;		// 얼굴모양

	////////////
	// 스킬 정보
	CSkillList	m_activeSkillList;
	CSkillList	m_passiveSkillList;
	CSkillList	m_etcSkillList;

#ifdef FACTORY_SYSTEM
	CSkillList			m_sealSkillList;							// 제작 스킬
	SEAL_SKILL_DATA 	m_sealSkillExp[MAX_SEAL_TYPE_SKILL];		// 제작 스킬 숙련도
#endif

	//////////////////
	// Special Skill
	CSSkillList	m_sSkillList;	// Special Skill List

	//////////////
	// 적용되는 값
	int		m_weight;			// 가방 무게
	int		m_maxWeight;		// 최대 가방 무게
	int		m_admin;			// 관리자 등급


// 050401 : bs : plus에 따른 확률 변수 추가 : PC만 사용
	int		m_plusStrong;
	int		m_plusHard;
	int		m_plusWeak;
// --- 050401 : bs : plus에 따른 확률 변수 추가



	///////////////////
	// 스탯 포인트 관련
	int		m_statpt_remain;	// 남은 수
	int		m_statpt_str;		// 힘
	int		m_statpt_dex;		// 민첩
	int		m_statpt_int;		// 지혜
	int		m_statpt_con;		// 체질

	////////////////////
	// Blood Point
	int		m_bloodPoint;		// Blood Point

	////////////////////
	// PC 전용 옵션 관련
#ifdef ENABLE_WAR_CASTLE
	int			m_opSturnLevel;		// 레벨 : 스턴
	int			m_opBloodLevel;		// 레벨 : 블러드
	int			m_opPoisonLevel;	// 레벨 : 포이즌
	int			m_opSlowLevel;		// 레벨 : 슬로우
	int			m_opMoveLevel;		// 레벨 : 무브

	int			m_opSturnIndex;		// 스킬 번호 : 스턴
	int			m_opBloodIndex;		// 스킬 번호 : 블러드
	int			m_opPoisonIndex;	// 스킬 번호 : 포이즌
	int			m_opSlowIndex;		// 스킬 번호 : 슬로우
	int			m_opMoveIndex;		// 스킬 번호 : 무브

	bool		m_bCreateMixItem;	// 공성조합 아이템 생성 여부
#endif

	int			m_opIncreaseInven;		// 최대 무게 증가 : 단위 %
	int			m_opMPSteal;			// 마나 흡수 : 단위 %
	int			m_opHPSteal;			// 생명력 흡수 : 단위 %
	int			m_opAttackBlind;		// 암흑 공격 415 : 단위 스킬 레벨
	int			m_opAttackPoison;		// 독 공격 414 : 단위 스킬 레벨
	int			m_opAttackCritical;		// 크리티컬 확률 증가 : 단위 %
	int			m_opRecoverHP;			// 체력회복 증가 : 단위 %
	int			m_opRecoverMP;			// 체력회복 증가 : 단위 %
	int			m_opDecreaseSkillDelay;	// 스킬 쿨타임 감소 : 단위 %
	int			m_opDecreaseSkillMP;	// MP 소모량 감소 : 단위 %
	int			m_opResistStone;		// 스톤 내성 증가 : 단위 %
	int			m_opResistSturn;		// 스톤 내성 증가 : 단위 %
	int			m_opResistSilent;		// 침묵 내성 증가 : 단위 %
	int			m_opBlocking;			// 데미지 절반 감소 (스킬 제외) : 단위 %

	//////////////
	// 저장되는 값
	int		m_dbStr;					// 힘
	int		m_dbDex;					// 민첩
	int		m_dbInt;					// 지혜
	int		m_dbCon;					// 체질

	int		m_dbHP;						// 체력
	int		m_dbMP;						// 마나

	////////////
	// 소켓 관련
	CDescriptor* m_desc;				// 디스크립터

	////////////
	// 시간 관련
	int		m_lastupdate;				// 최근 저장 시간
	int		m_autochknum[2];			// auto check에서 사용한 수치 저장
	int		m_silencePulse;				// 채팅 금지 시간 셋팅 변수
	int		m_lastProducePulse;			// 최근 생산 메시지 온 시간
	int		m_hackProduceCount;			// 샌상메세지 온 수 저장
	int		m_ProduceClientTime;		// 클라이언트가 보낸 생산시간
	int		m_perSecondPulse;			// 1초 검사용 : 다용도

#ifdef CHAT_BLOCK_USA
	int		m_shoutPulse;
#endif // CHAT_BLOCK_USA

	bool	m_bProcDisconnect;			// 접속 종료시 ProcDisconnect()를 호출 했는지 여부

	//////////////
	// 아이템 관련
	CInventory	m_invenNormal;			// 일반 아이템 탭
	CInventory	m_invenQuest;			// 퀘스트 탭
	CInventory	m_invenEvent;			// 이벤트 탭
	CStash		m_stash;				// 창고
	bool		m_bChangeStash;			// 창고 변경 사항 있는지 검사
	CNetMsg		m_stashMsg;				// 창고 요청 메시지

	CItem*		m_moneyItem;			// 돈

	CItem*		m_wearing[MAX_WEARING];	// 착용장비 : 투구 상의 특수 무기 하의 방패 장갑 신발 액서서리1,2,3

	CLCList<AUTO_GIVE_DATA>		m_listAutoGive;		// 자동 지급한 아이템 정보
	CLCList<BLOOD_BUG_DATA>		m_listBloodBug;		// 업그레이드 불가 아이템에 옵션 붙는 버그 수정에 의해 옵션 제거된 아이템 정보

	////////////
	// 교환 관련
	CExchangeItems*		m_exchange;

	// 상점 관련
	CPersonalShop*		m_personalshop;


	/////////////////
	// 장소 기억 관련
	CMemPos				m_mempos;				// 기억된 장소 리스트
	int					m_reqWarpTime;			// 순간이동 남은 시간
	int					m_reqWarpType;			// 이동 방법(귀환/장소기억)
	int					m_reqWarpData;			// 귀환시 존번호/장소기억시 슬롯번호

#ifdef	PRIMIUM_MEMORYBOOK
	CMemPosPuls			m_memposplus;			// 기억된 장소 리스트	
#endif	// PRIMIUM_MEMORYBOOK

	////////////
	// 파티 정보
	CParty*				m_party;

#ifdef	EXPEDITION_RAID
	// 원정대 정보
	CExpedition*		m_Exped;	
//	int					m_nExpedQuitMode;
//	time_t				m_nLastCmdRunTime;
#endif	// EXPEDITION_RAID
	

	//////////////
	// Quick Slot
	CQuickSlot		m_quickSlot[QUICKSLOT_PAGE_NUM];	// 퀵슬록 3페이지 소유

	//////////////
	// pc 상테관련
	int				m_pkPenalty;				// PK 성향
	int				m_pkCount;					// PK 수
	int				m_pkRecoverPulse;			// 성향 수치 회복 남은 시간
	int				m_pkmodedelay;				// PKMODE 전환 딜레이
	int				m_pkPenaltyHP;				// HP 감소
	int				m_pkPenaltyMP;				// MP 감소
	int				m_pkRecoverNPCCount;		// 사냥 수
#ifdef RESTRICT_PK
	int				m_lastPKTime;				// 접속한 시간
#endif // RESTRICT_PK

#ifdef RESTRICT_PVP_SKILL
	int				m_nRestrictPvPSkillDelaySec;	// 몹에 의한 타게팅 유지 유효 시간
#endif // RESTRICT_PVP_SKILL

	// 정당방위 관련
	CRaChar*		m_raList;

	///////////////////
	// 아이템 사용 관련
	int				m_recoverHPItemTime;		// HP 회복 남은 시간
	int				m_recoverHPItemValue;		// HP 초당 회복량
	int				m_recoverMPItemTime;		// HP 회복 남은 시간
	int				m_recoverMPItemValue;		// MP 초당 회복량

	///////////////////
	// 퀘스트 관련
	CQuestList		m_questList;				// 퀘스트 리스트
	int				m_nLastCollectRequest;		// 마지막 MSG_QUEST_COLLECT 받은 시간
	int				m_nCollectRequestNPCIndex;	// 마지막 MSG_QUEST_COLLECT 받은 NPC 인덱스
	int				m_nCollectRequestCount;		// 수집퀘스트 누적 수

	////////////
	// 길드 관련
	CGuildMember*	m_guildInfo;				// 길드 정보, 상위길드원이면서 길드장인 경우 자신이 길드 장인 길드 정보
	int				m_regGuild;
	int				m_guildoutdate;				// 길드 탈퇴 일

	////////////////
	// 균등파티시 아이템을 받은적이 있는가?
	bool			m_bGiveItem;

	////////////////////
	// 변신 관련
	int				m_changePulse;			// 변신 시간
	int				m_changeIndex;			// 변신 몹 인덱스

	////////////////////
	// 아이템 이펙트 옵션
	char			m_plusEffect;			// 아이템 플러스 이펙트 옵션

	/////////////////////
	// 사제 시스템

	// 사제 필요 정보
	int		m_teachIdx[TEACH_MAX_STUDENTS];		// 사제 인덱스
	int		m_teachLevel[TEACH_MAX_STUDENTS];	// 사제 레벨
	char	m_teachJob[TEACH_MAX_STUDENTS];		// 사제 직업
#ifdef ENABLE_CHANGEJOB
	char	m_teachJob2[TEACH_MAX_STUDENTS];	// 사제 직업
#endif
	char	m_teachName[TEACH_MAX_STUDENTS][MAX_CHAR_NAME_LENGTH + 1];// 사제 이름
	int		m_teachTime[TEACH_MAX_STUDENTS];	// 사제 성립된 시간 : sec

	// 공통정보
	int		m_teachWait;				// 신청중인 캐릭터의 인덱스
	char	m_teachType;				// none -1 teacher 0 student 1
	bool	m_bTeacher;					// 서버의 선생리스트에 등록 여부

	int		m_fame;						// 명성치
	int		m_cntTeachingStudent;		//0627
	int		m_cntCompleteStudent;
	int		m_cntFailStudent;
	int		m_superstone;				//받은 초고제 갯수
	int		m_guardian;					//후견인 시스템으로 성장한 캐릭터인지 체크.
	bool	m_displayCanSstone;			// 초고제를 받을수 있는지를 display했는가?

//0502 kwon
	int m_nMoonStoneDigit;				//가장 많은 갯수의 문스톤 숫자
	int m_nMoonStoneSum;				//문스톤 숫자 갯수

#ifdef EVENT_NEW_MOONSTONE
	int m_nMSIndex;
	int	m_nMSGiftIndex;
#endif // EVENT_NEW_MOONSTONE

#ifdef ENABLE_PET
	///////////
	// 애완동물
	CPet*	m_petList;
	int		m_pulseLastAttackSkill;
#ifdef NEW_PET_EXP_SYSTEM
#else // NEW_PET_EXP_SYSTEM
	// pc가 공격 했을때 미스가 아닐때만 경험치 추가 
	bool	m_bNotMiss;
#endif // NEW_PET_EXP_SYSTEM
#endif // ENABLE_PET

	CElemental*		m_elementalList;		// 소환수 리스트
	int				m_pulseElemental[4];	// 각 소환수별 소환 시간

	int				m_pulseEvocation[2];	// 강신한 시간 : 헬운드, 엘레넨
	char			m_evocationType;		// 강신 타입

	int				m_memposTime;			// 메모리 스크롤 확장 사용시간
#ifdef CHARSLOTEXT_CARD
	int				m_charslotTime[MAX_EXT_CHAR_COUNT];	// 케릭터 슬롯 확장 사용시간
#endif
	int				m_stashextTime;			// 창과 확장 사용시간

	// 2005 크리스마스 이벤트 변수
	int		m_pulseTakeEventXMas2005;

	// 2006 신년 이벤트
	int		m_nTimeEventNewYear2006;		// 사냥시간
	int		m_pulseEventNewYear2006;		// 최근사냥시간

#ifdef NEW_DIVISION_EXPSP
	// 경험치 분배관련 추가 변수 : 051228 : BS
	int				m_nExpDamage;			// 누적 대미지
#endif // #ifdef NEW_DIVISION_EXPSP

#ifdef EVENT_SEARCHFRIEND
	int			m_serverOld;				// 통합시 서버당시 아이디에 더하는 부분
#endif // #ifdef LC_KOR

#ifdef EVENT_SEARCHFRIEND
	// 휴면 케릭 이벤트
	int		m_nTimeEventSearchFriend;			// 사냥시간
	int		m_pulseEventSearchFriend;			// 최근사냥시간
	int     m_nGoodEventSearchFriend;			// 휴면 케릭 아템 개수

	// 휴면 케릭 리스트에 들어갈 부분들
	bool	m_bEventSearchFriendSelect;			//등록 여부
	int     m_nEventSearchFriendListCount;		//리스트 개수

	//휴면 케릭을 등록한 케릭 한명을 선택
	int		m_nEventSearchFriendIndex[50];		//등록한 케릭 인덱스.
	char	m_nEventSearchFriendNick[50][MAX_CHAR_NAME_LENGTH + 1];	//등록한 케릭 닉네임.

	//휴면 케릭들로 받을수 있는 아이템 (NPC)
	int		m_nGoodEventSearchFriendIndex[50];			// 휴면 케릭이 등록된...인덱스
	char	m_nGoodEventSearchFriendNick[50][MAX_CHAR_NAME_LENGTH + 1];	// 휴면 케릭이 등록된...이름.
	int		m_nGoodEventSearchFriendGood[50];			// 휴면 케릭을 등록한 개수 아템
	bool	m_bGoodEventSearchFriendListImprove ;		// 리스트 갱신. 가능한지. 
	int		m_nGoodEventSearchFriendListGoodCount;		// 보상여부 리스트 개수
	int		m_pulseEventSearchFriendList;				// 최근 리스트 보상여부 본시간.
	int		m_nGoodEventSearchFriendGoodNumber;			// 선택된 번호.
#endif // #ifdef EVENT_SEARCHFRIEND


	int		m_nGoldenBallNoticeStatus;					// 골든볼 이벤트 공지 받은 상태

#ifdef EVENT_JPN_2007_NEWSERVER
	int		m_nEventJPN2007NewServerGift;				// 신섭 이벤트
#endif // EVENT_JPN_2007_NEWSERVER

#ifdef JPN_GPARA_PROMOTION
	int		m_nGparaPromotionData;						// 일본 GPARA 이벤트
#endif // JPN_GPARA_PROMOTION

#ifdef JPN_OCN_GOO_PROMOTION
	int		m_nOCN_Goo_PromotionData;					// 일본 OCN, Goo 이벤트
#endif // JPN_OCN_GOO_PROMOTION
	
#ifdef JPN_MSN_PROMOTION
	int		m_nMSNPromotionData;						// 일본 MSN 이벤트
#endif // JPN_MSN_PROMOTION
	
#ifdef CREATE_EVENT
	int		m_nCreateEventData;							// 신규 유저 이벤트
#endif // CREATE_EVENT

#ifdef EVENT_GOMDORI_2007
	char	m_nEventGomdori2007Win;				// 누적 승수
	char	m_nEventGomdori2007Game;			// 현재 게임 가위바위보 시도 수
	char	m_nEventGomdori2007FirstLose;		// 처음 패 보상 가능 여부
	char	m_nEventGomdori2007FirstWin;		// 처음 1승 보상 가능 여부
	bool	m_bEventGomdori2007Start;			// 게임 시작 여부
	int		m_nEventGomdori2007Total;			// 게임 누적 수
	bool	m_bEventGomdori2007CanSelect;		// 선택 가능 여부
	int		m_bEventGomdori2007Drop;			// 드롭 여부
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_INDEPENDENCE_DAY_2007_USA
	int		m_bEventIndependenceDay2007drop;
#endif // EVENT_INDEPENDENCE_DAY_2007_USA

#ifdef NEW_GUILD
	int		m_GuildExp;			// 길드포인트로 전환될 경험치
	int		m_GuildFame;		// 길드 포인트로 전환될 명성치
#endif // NEW_GUILD

#ifdef DRATAN_CASTLE		
	long		m_nRespondTime;		// 교감 시간 저장
#endif // DRATAN_CASTLE

#ifdef ALTERNATE_MERCHANT			// 개인 상점 대행 NPC
	CAlternateMerchant*		m_pAlternateMerchant;
#endif // ALTERNATE_MERCHANT

#ifdef ATTACK_PET
	CAPet*		m_pApetlist;
#endif //ATTACK_PET Value

#ifdef NEW_UI
	float m_runSpeed_org;
	char m_attackSpeed_org;
	int m_maxHP_org;
	int m_maxMP_org;
#endif // NEW_UI

//#ifdef IRIS_POINT
//	int	m_connect_min;
//#endif //IRIS_POINT

#ifdef EVENT_PHOENIX// yhj   081105  피닉스 
	int m_bPhoenix_Char;
#endif // EVENT_PHOENIX

#ifdef GM_GO_ZONE
	GM_GOZONE_TYPE m_gmGoZoneType;
#endif // GM_GO_ZONE

#ifdef EX_GO_ZONE
	CLCList<int> m_listRegisterNpc;
	int	m_lastsavepos;
	int		GetSaveNpcTeleporter();
	int		GetLastSaveZone(int* zone);
#endif // EX_GO_ZONE

#ifdef FACTORY_SYSTEM
	CFactoryList	m_listFactory;		// 제작 가능 아이템 list
#endif

#ifdef NIGHTSHADOW_SKILL
	bool		m_bTaming_npc;			// 테이밍 스킬을 사용하고 있는지 여부
	int			m_nSoul_Count;			// 나이트쉐도우 흡수한 영혼
	CCharacter* m_Owners_target;
	CCharacter* GetOwners_target()						{ return m_Owners_target; }
	void		SetOwners_target(CCharacter* target)	{ m_Owners_target = target; }
	int			GetSoul_Count()							{ return m_nSoul_Count; }
	void		SetSoul_Count(int count)				{ m_nSoul_Count = count; }
#endif // NIGHTSHADOW_SKILL
	///////////////////////////////////////////////////////////////////
	// 함수 정의

	///////////////////
	// 캐릭터 정보 전달
	void SendStatus();				// 상태 보내기
	void SendInventory(char resultArrange);	// 인벤토리 전달
	void Send(CNetMsg& msg);		// 메시지 보내기
	void SendQuickSlot();			// 퀵슬롯 전달

	void Restore();					// 상태 회복
	bool AddMoney(LONGLONG money);	// + money
	bool AddExpSP(LONGLONG exp, int sp, bool bUseEvent);	// + sp, + exp

#ifdef JP_OTAKU_SYSTEM
	void OtakuAddExpSP( LONGLONG exp, int sp );
#endif //JP_OTAKU_SYSTEM

	void LevelUp(bool bSendEffect);	// 레벨업 효과
	void InitStat();				// 스탯을 레벨에서 부터 초기화
	void CalcStatus(bool bSend);	// 아이템, 스킬에 의한 스탯 변화

	bool InitSkill(CSkillList* skillList, bool isPassive = false); // 스킬 초기화
	bool InitSSkill();						// 특수 스킬 초기화

	void RecoverPC();				// 자동회복, HP MP
	void ApplyItemValue(bool bSend);

	int CanChange();

	const char* GetName()
	{
		if( IsNick() )
			return m_nick;
		return m_name;
	}

	void ChangeName(const char* name);

	bool IsNick()
	{
		if (m_nick != NULL && strcmp(m_nick, "") != 0)
			return true;
		else
			return false;
	}

	////////////////////
	// Function name	: SettingItemOption
	// Description	    : 착용 아이템 옵션 셋팅
	// Return type		: int 
	//                  : 옵션 세팅된 파트 번호
	int SettingItemOption();


	//////////////////////////////
	// 퀵슬롯에서 특정 아이템 제거
	void RemoveItemFromQuickSlot(CItem* item);

	//////////////////////////////
	// 퀵슬롯에서 특정 스킬 제거
	void RemoveSkillFromQuickSlot(CSkill* pSkill);	


	/////////////////////
	// Function name	: CheckWeight
	// Description	    : 무게 검사
	// Return type		: bool 
	//            		: weight만큼 추가 가능하면 true 아니면 false
	//            		: 100~150%까지는 경고 메시지 보냄
	//                  : 150% 초과시에는 오류 메시지 보냄
	// Argument         : int weight
	//                  : 추가하려고 하는 무게
	// Argument         : bool bSendWarning
	//                  : 무게 초과시 경고 문구 날릴지 여부
	// Argument         : bool bCheckWeight
	//                  : false이면 무게 초과에 관계없이 아이템을 소유하도록 허용
	// Argument         : bool bCalcStatus
	//                  : true이면 CalcStatus()를 사용
	bool CheckWeight(LONGLONG weight, bool bSendWarning, bool bCheckWeight, bool bCalcStatus);


	/////////////////////
	// Function name	: SetMaxWeight
	// Description	    : 최대 무게 구하기
	void SetMaxWeight(bool bReset);


	/////////////////////
	// Function name	: GoMemPos
	// Description	    : 기억된 장소로 이동
	// Argument         : int slot
	//                  : 이동할 장소 기억 위치
	void GoMemPos(int slot);

#ifdef PRIMIUM_MEMORYBOOK
	void GoMemPosPlus(int slot);
#endif	// PRIMIUM_MEMORYBOOK

	/////////////////////
	// Function name	: UseStatPoint
	// Description	    : 스탯 포인트 사용
	// Return type		: bool 
	//            		: 성공 여부
	// Argument         : MSG_STATPOINT_USE_TYPE type
	//                  : 변경할 스탯
	// Argument         : int* val
	//                  : 변화된 스탯 수치
	bool UseStatPoint(MSG_STATPOINT_USE_TYPE type, int* val);


	/////////////////////
	// Function name	: CanWarp
	// Description	    : 워프 아이템을 사용할 수 있는지 검사
	// Return type		: int
	//            		: 사용할 수 있으면 0, 아니면 시스템 메시지
	int CanWarp();


	/////////////////////
	// Function name	: FindSkill
	// Description	    : 스킬 찾기
	CSkill* FindSkill(int index);


	////////////////////
	// Function name	: GetAttackLevel
	// Description	    : 무기 레벨 구하기
	virtual int GetAttackLevel() const;


	////////////////////
	// Function name	: GetDefenseLevel
	// Description	    : 방어구 평균 레벨 구하기
	virtual int GetDefenseLevel() const;


	////////////////////
	// Function name	: GetAttackType
	// Description	    : 기본 공격의 근접 원거리 마법 종류를 가려냄
	virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const;


	////////////////////
	// Function name	: GetPKName
	// Description	    : 성향에 따른 칭호를 반환
	// Return type		: int 
	//                  : -5 부터 +5까지 순서대로 카오스리전,네크로나이트,아웃로,피스캐스터,머더러,일반,어벤저,라이트포터,가디언,세인트나이트,디바인프로텍터
	int GetPKName();


	////////////////////
	// Function name	: IsSetPlayerState
	// Description	    : 상태 필드가 셋 되어 있는지 검사
	bool IsSetPlayerState(int checkstate) { return ((m_state & checkstate) ? true : false); }

	////////////////////
	// Function name	: SetPlayerState
	// Description	    : 상태 필드 세팅
	void SetPlayerState(int checkstate) { m_state |= checkstate; }

	////////////////////
	// Function name	: ResetPlayerState
	// Description	    : 상태 필드 리셋
	void ResetPlayerState(int checkstate) { m_state &= ~checkstate; }

	////////////////////
	// Function name	: TogglePlayerState
	// Description	    : 상태 필드 토글
	void TogglePlayerState(int checkstate) { m_state ^= checkstate; }

	////////////////////
	// Function name	: GetPlayerState
	// Description	    : 상태 반환
	int GetPlayerState() { return m_state; }

	////////////////////
	// Function name	: IsCombatMode
	// Description	    : 전투 상태 검사, 최근 공격후 일정 시간(PULSE_ATTACKMODE_DELAY)이 지났는가
	bool IsCombatMode();


	////////////////////
	// Function name	: CanMove
	// Description	    : 이동 가능 검사
	virtual bool CanMove();

	////////////////////
	// Function name	: CanAttack
	// Description	    : 공격 가능 검사
	virtual bool CanAttack();

	////////////////////
	// Function name	: CanSpell
	// Description	    : 시전 가능 검사
	virtual bool CanSpell();

	////////////////////
	// FN : GiveItem
	// 아이템 생성 / 지급
	bool GiveItem(int itemIndex, int plus, int flag, LONGLONG itemNum, bool bdrop = false , bool bOption = false );

	////////////////////
	// Function name	: GetSize
	// Description	    : 캐릭터 크기 반환
	virtual float GetSize() { return 0.0f; }

	bool IsParty() { return (m_party && m_party->GetRequestIndex() != m_index && m_party->GetMemberCount() > 1) ? true : false; }
	void SaveNow(bool bStash) { m_bChangeStash = bStash; m_lastupdate = 0; }

	virtual int GetUsingStat() const { if (m_wearing[WEARING_WEAPON]) return m_wearing[WEARING_WEAPON]->GetUsingStat(); else return USING_STR; }

#ifdef ENABLE_MESSENGER
	void SetMessengerCondition(int condition) {m_nCondition = condition;}
#ifdef MESSENGER_NEW
	bool AddBlockPC(int charIndex, const char* name);
	void ReleaseBlockPC(int charIndex, CLCString& name);
	// 리스트의 이름을 최신으로 유지할수 없으므로 이즈 블럭을 호출할때 최신으로 유지한다
	bool IsBlockPC(int charIndex, const char* name );
	void GetBlockListString(CLCString& blockIndexList, CLCString& blockNameList);
#endif
#endif // ENABLE_MESSENGER

	void do_QuestGiveUp(CPC* ch, CQuest* quest);//레벨업에 의한 퀘스트 강제 포기.

	bool CheckDungeonData(int zone);

#ifdef ENABLE_CHANGEJOB
	virtual int GetResistLevelBonus()
	{
		switch (m_job)
		{
		case JOB_MAGE:
		case JOB_HEALER:
		case JOB_SORCERER:
#ifdef NIGHT_SHADOW
		case JOB_NIGHTSHADOW:
#endif //NIGHT_SAHDOW
			return m_level;
		default:
			return m_level / 2;
		}
	}
#endif

	// 공성 참여 플래그 구하기
	int GetJoinFlag(int zoneindex);

	// 공성 참여 플래그 설정
	void SetJoinFlag(int zoneindex, int value);

	// 카오틱 캐릭터 판별
	bool IsChaotic() { return (m_pkPenalty < -9) ? true : false; }

#ifdef ENABLE_PET
	// 펫의 배고픔/교감도/경험치 값을 변경한다
	void UpdatePetValue();
	// 펫 리스트에서 특정 INDEX의 펫을 찾는다
	CPet* GetPet(int index = 0);
	// 펫 삭제
	void DelPet(int petIndex);
#endif

#ifdef ATTACK_PET
	// 펫의 배고픔/교감도/경험치 값을 변경한다
	void UpdateAPetValue();
	// 펫 리스트에서 특정 INDEX의 펫을 찾는다
	CAPet* GetAPet(int index = 0);			// 0 이면 현재 착용한 펫
	// 펫 삭제
	void DelAPet(int petIndex);
#endif // ATTACK_PET Function

	// target에게 pvp 가능 여부 검사, 가능하하면 정당방위 설정
	bool CanPvP(CCharacter* target, bool bIgnoreInvisible);

	// 변신 해제
	void CancelChange();

	// 소환
	CElemental* SummonElemental(char elementalType);
	// 소환 해제
	void UnsummonElemental(CElemental* elemental);
	// 소환 가능 검사 : 중복, 동시소환, 소환시간
	bool CanSummonElemental(char newElementalType, char* duplicationType, int* nRemainTimeSummon, bool* errElementalType);
	// 소환수 얻기
	CElemental* GetElemental(int elementalindex);
	// 소환수 상태정보 보내기 : 변경된 것만
	void SendElementalStatus();

	// 강신
	void Evocation(int type);
	// 강신 해제
	void Unevocation();

	// 강신 남은 시간
	int GetRemainEvocation(bool bSec);
	// 패시브 스킬 적용
	void ApplyPassive();

//#ifdef ADULT_SERVER_NEWITEM
//	bool IsSameAttrItem(int itemflag);
//	void WearSameAttrWeapon();

// 	int m_lastpulseCurse;
// 	int m_lastpulseDespair;
//#endif // ADULT_SERVER_NEWITEM


	////////////////////
	// Function name	: ProcDisconnect
	// Description	    : 존이동, 접속 종료 등에서 파티, 교환, 사제 등 처리
	// Argument         : bool bDisconnect
	//                  : 접속 종료 여부, 종료시 레이어 존에 있으면 시작 존으로 변경
	// Argument         : bool bGoto
	//                  : 같은 존내 이동에서 호출 된 경우
	void ProcDisconnect(bool bDisconnect, bool bGoto);

	// 자동 지급, 블러드옵션 버그 로그 남기기
	void OutputDBItemLog();

#ifdef ADULT_SERVER_NEW_BALANCE
	virtual float GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type);
	virtual float GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type);
#endif // ADULT_SERVER_NEW_BALANCE

#ifdef DRATAN_CASTLE	
	void SetRespondTime(long time) { m_nRespondTime = time;}
	long GetRespondTime() { return m_nRespondTime; }
#endif // DRATAN_CASTLE

#ifdef MONSTER_COMBO
	CLCList<MONSTER_COMBO_DATA> m_listSaveComboData;
#endif // MONSTER_COMBO

#ifdef LAKA_PRESSCORPS
	char m_bPressCorps;
#endif // LAKA_PRESSCORPS
	
#ifdef SET_ITEM
	void ApplySetItemValue(CItem * pItem);
	void ApplyOriginOption(CItem * pItem);
	void RemoveSetItemValue(CItem * pItem);
	void CountSetItem(CItem* pItem, bool bPlus = true);
#endif

#ifdef FACTORY_SYSTEM
	int GetSealExpPos(int nSkillIdx)	
	{
		for (int i = 0; i < MAX_SEAL_TYPE_SKILL; i++)
		{
			if (m_sealSkillExp[i].nSkillIdx == nSkillIdx)
				return i;
		}
		return -1;
	}

	void SetSealExp(int nSkillIdx, LONGLONG llExp)	
	{
		int nPos = GetSealExpPos(nSkillIdx);
		if (nPos > -1 && nPos < MAX_SEAL_TYPE_SKILL)
			m_sealSkillExp[nPos].llExp = llExp;
	}
	
	void AddSealSkill(int nSkillIdx)
	{
		int nPos = -1;
		for (int i = 0; i < MAX_SEAL_TYPE_SKILL; i++)
		{
			if (m_sealSkillExp[i].nSkillIdx < 0)
			{
				nPos = i;
				break;
			}
		}

		m_sealSkillExp[nPos].nSkillIdx	= nSkillIdx;
		m_sealSkillExp[nPos].llExp		= 1;
	}

#endif
};

//////////
// PC List

class CPCList  
{
public:
	CPC*	m_pcList[MAX_PLAYER_LIST];	// 플레이어 리스트 : 현재 배열로 사용
	int		m_max;						// 리스트의 최대 사용 인덱스
	int		m_cur;						// 현재 카운터


	CPCList()
	{
		int i;

		for (i = 0; i < MAX_PLAYER_LIST; i++)
			m_pcList[i] = NULL;
		m_max = 0;
		m_cur = 0;
	}

	virtual ~CPCList()
	{
		m_max = m_cur = 0;
	}


	// 오퍼레이터
	int Add(CPC *ch)
	{
		int i;

		if (m_max >= MAX_PLAYER_LIST)	return -1;
		if (ch->m_listIdx != -1 && m_pcList[ch->m_listIdx] == ch) return ch->m_listIdx;

		for (i = 0; i < m_max && m_pcList[i]; i++)
			;

		m_pcList[i] = ch;
		ch->m_listIdx = i;
		if (i == m_max) m_max++;
		m_cur++;

		return i;
	}

	void Remove(CPC *ch)
	{
		int i;

		if (!Playing(ch))	return ;
		
		DelAttackList(ch);
		DelRaList(ch, true);

		i = ch->m_listIdx;
		if (i < 0 || i >= MAX_PLAYER_LIST)	return ;

		m_pcList[i] = NULL;
		ch->m_listIdx = -1;
		if (i == m_max) m_max--;
		m_cur--;
	}

	bool Playing(CPC *ch)
	{
		if (ch == NULL)				return false;
		if (ch->m_name[0] == '\0')	return false;
		if (ch->m_listIdx < 0 || ch->m_listIdx >= MAX_PLAYER_LIST)	return false;
		if (m_pcList[ch->m_listIdx] == ch)	return true;
		return false;
	}

	CPC* Find(int nCharIndex)
	{
		int i;

		for (i = 0; i < m_max; i++)
			if (m_pcList[i] && m_pcList[i]->m_index == nCharIndex)
				return m_pcList[i];

		return NULL;
	}

	CPC* Find(const char* strCharName, char NickUseFlag )
	{
		NickUseFlag = NickUseFlag;

		int i;
		
		for (i = 0; i < m_max;i++)
		{
#ifdef LC_JPN
			if (m_pcList[i] && ( ( !NickUseFlag && strcmp(m_pcList[i]->m_name, strCharName) == 0 ) || strcmp(m_pcList[i]->m_nick, strCharName) == 0 ) )
#else
			if (m_pcList[i] && ( strcmp(m_pcList[i]->m_name, strCharName) == 0 || strcmp(m_pcList[i]->m_nick, strCharName) == 0 ) )
#endif
				return m_pcList[i];
		}

		return NULL;
	}
	
	CPC* Get(int nListIndex)
	{
		if(nListIndex < 0 || nListIndex >= MAX_PLAYER_LIST)
			return NULL;

		return m_pcList[nListIndex];
	}

	void SendToAll(CNetMsg& msg);
	void SendGuildWarPointMsg(CWarCastle* castle, int zoneindex);
	void SendToWarJoinUser(CNetMsg& msg, int zoneindex, bool bInZone);
	//0627
	void SendToAllTeacher(CNetMsg& msg);
};

////////////
// NPC class
class CNPC : public CCharacter
{
public:
	CNPC();
	virtual ~CNPC();

	CNPCProto*	m_proto;		// DB 설정치

	int			m_idNum;		// 몹 아이디 번호(DB index)
	int			m_flag;			// Flag
	int			m_flag1;		// Flag1(확장 플래그)	
	float		m_regenX;
	int			m_regenY;
	float		m_regenZ;
	int			m_disableTime;	// 리젠 이후 안 보이는 시간
	int			m_delay;		// 딜레이 공통

// 050322 : bs : 공격 전용 딜레이 변수 추가
	int			m_pulseAttack;	// 최근 공격 시간 : gserver.m_pulse와 직접 비교
// --- 050322 : bs : 공격 전용 딜레이 변수 추가

	char		m_attackType;	// 기본공격방법
	int			m_postregendelay;	// 리젠 이후 선공 하기 전까지의 시간 딜레이

	// 이동관련
	bool		m_bMoveRun;		// 뛰는가 true 걷는가 false
	int			m_nBlockRegen;	// 리젠위치로 가고 있는데 블럭당해서 못 간 경우 카운트
								// BLESS_WARRIOR : 전사의 축복에서 소유자 PC 인덱스
	bool		m_bStop;		// 서있는가
	int			m_moveArea;		// 이동 범위

	// AI 관련
	int			m_aipulse;		// NPC AI Pulse
	int			m_moveDir;		// 이동 방향 오른쪽 : 1 왼쪽 : -1 정지 : 0
	int			m_nBlockAI;		// AI 적용시 이동경로에서 블럭당한 수
	bool		m_bChangeDir;	// 이동향이 바뀌어야 하는가
	bool		m_bFail45;		// 45도 꺾기가 실패인가

	int			m_quest[QUEST_MAX_NPC];			// 퀘스트 NPC 퀘스트 번호 저장
	int			m_nQuestCount;					// 퀘스트 개수

	// 050221 : bs : NPC 스킬
	CSkill*		m_skills[MAX_NPC_SKILL];
	// --- 050221 : bs : NPC 스킬

#ifndef NEW_DIVISION_EXPSP
	// npc target 관련
	int			m_totalDamage;	// npc가 맞은 총 누적 damage
#endif // #ifndef NEW_DIVISION_EXPSP

#ifdef	EVENT_HALLOWEEN_2006
	int			m_regenTime;
#endif	// EVENT_HALLOWEEN_2006
	
#ifdef EVENT_XMAS_2007
	int			m_regenTimeXmas2007;
#endif //EVENT_XMAS_2007

	CNPC*		m_pNPCPrev;		// NPC 리스트 링크
	CNPC*		m_pNPCNext;		//

	CNPCRegenInfo* m_regenInfo;

#ifdef MONSTER_RAID_SYSTEM
	int			m_nRaidMoveTime;			// 마지막 전투 시간
	int			m_bRaidNPCSummon;			// 레이드 몹이 소환했는지 여부
#endif // MONSTER_RAID_SYSTEM

#ifdef EVENT_XMAS_2006
	int			m_nEventXMasMoveTime;		// 마지막 전투 시간
#endif // EVENT_XMAS_2006

#ifdef MOB_SCROLL
	int m_MobScrollType;
	int m_NextMobIndex;
	int m_UseCharIndex;
#endif // MOB_SCROLL

#ifdef ALTERNATE_MERCHANT			// 개인 상점 대행 NPC
	int		m_nOWnerIndex;
	CLCString	m_pstrOwnerName;
	CLCString	m_pstrShopName;	
#endif // ALTERNATE_MERCHANT

#ifdef MONSTER_COMBO
	int		m_coinidx;
	int		m_coincount;
#endif // MONSTER_COMBO

#ifdef MONSTER_AI
	float	m_nOldDist;						// 몬스터의 이동 전 위치
	bool	m_bMoveLock;					// 몬스터 무적 상태
	int		m_pulseMoveLock;				// 무적 시간
	int		m_bMoveToRegen;					// 리젠 위치로 이동 여부
#endif
	
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
	int		m_Mob_State;					// 몬스터가 테이밍이 되었는가? 아니면 혼란인가?
	CPC*	m_owner;						// 몬스터를 테이밍한 캐릭터
	CPC*	GetOwner()				{ return m_owner; }
	void	SetOwner(CPC* pc)		{ m_owner = pc; }	// 소유자 설정

	int		Check_MobFlag(int mask) const	{ return m_Mob_State & mask; }
	void	Set_MobFlag(int state)			{ m_Mob_State |= state; }
	void	reSet_MobFlag(int state)		{ m_Mob_State &= ~state; }
#endif  // NIGHTSHADOW_SKILL	
	
#ifdef RAID_MONSTER_SKILL  // 레이드 몬스터 스킬
	CNPC*	m_calling_npc;					// npc를 호출 한 npc;
#endif //RAID_MONSTER_SKILL
	
	void		DeleteNPC();	// npc의 타켓리스트의 어택리스트에서 스스로를 삭제하고 메모리 해제
	void		InitAIVar();	// AI 관련 변수 초기화
	void		ResetStat();	// 프로토의 값으로 수치를 초기화

	////////////////////
	// Function name	: CanMove
	// Description	    : 이동 가능 검사
	virtual bool CanMove();

	////////////////////
	// Function name	: CanAttack
	// Description	    : 공격 가능 검사
	virtual bool CanAttack();


	////////////////////
	// Function name	: GetAttackLevel
	// Description	    : 무기 레벨 구하기
	virtual int GetAttackLevel() const;


	////////////////////
	// Function name	: GetDefenseLevel
	// Description	    : 방어구 평균 레벨 구하기
	virtual int GetDefenseLevel() const;


	////////////////////
	// Function name	: GetAttackType
	// Description	    : 기본 공격의 근접 원거리 마법 종류를 가려냄
	virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const;

	////////////////////
	// Function name	: GetSize
	// Description	    : 캐릭터 크기 반환
	virtual float GetSize() { return m_proto->m_size; }

	virtual int GetUsingStat() const { if (GetAttackType(NULL) == MSG_DAMAGE_RANGE) return USING_DEX; else return USING_STR; }

	CSkill* FindSkill(int idx);

	// 패시브 스킬 적용 : NPC는 없음
	void ApplyPassive() {}

	// 스탯 재계산
	void CalcStatus(bool bSend);

#ifdef NEW_DIVISION_EXPSP
	// 자신을 공격한 대상들이 준 데미지 총합을 반환
	LONGLONG GetTotalDamage();
#endif // #ifdef NEW_DIVISION_EXPSP

#ifdef ADULT_SERVER_NEW_BALANCE
	virtual float GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type);
	virtual float GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type);
#endif // ADULT_SERVER_NEW_BALANCE

#ifdef EXTREME_CUBE
	bool m_bCubeRegen;
#endif // EXTREME_CUBE

#ifdef MONSTER_AI
	float GetDistToRegen();					// 리젠위치와 거리
#endif
};

#ifdef ENABLE_PET
class CPet : public CCharacter
{
	friend class CDBThread;
	friend void do_GM(CPC* ch, CNetMsg& msg);

	CPC*	m_owner;			// 펫 주인
	char	m_petTypeGrade;		// 펫 종류 + 등급


	int		m_hungry;			// 배고픔
	int		m_pulseHungry;		// 배고픔 펄스
	int		m_sympathy;			// 교감도
	int		m_pulseSympathy;	// 교감도 펄스
	int		m_abilityPoint;		// 기술 포인트
#ifdef NEW_PET_EXP_SYSTEM
#else // NEW_PET_EXP_SYSTEM
	int		m_pulseExpUp;		// 경험치 획득 펄스
#endif // NEW_PET_EXP_SYSTEM
	
#ifdef PET_DIFFERENTIATION_ITEM
	char	m_petColorType;
#endif //PET_DIFFERENTIATION_ITEM
	
#ifdef PET_TURNTO_NPC
	int		m_nTurnToNpcIndex;
	int		m_nTurnToNpcSize;
#endif //PET_TURNTO_NPC
	bool	m_bSummon;			// 소환되었는지 여부
	bool	m_bMount;			// 타고 있는지 여부
	bool	m_wasnear;			// 050309: bw 20미터 내에 있는지 밖에 있는지의 변화
	
	// 060221 : bs : 펫 사망시 일정시간 착용불능으로
	int		m_nRemainRebirth;	// 부활 남은 시간

	CSkillList m_skillList;		// 스킬 리스트

public:

	CPet*	m_prevPet;
	CPet*	m_nextPet;

	CPet(CPC* owner, int index, char petTypeGrade, int level);
	~CPet();


	////////////
	// 기본 정보

	// 소유자 반환
	CPC* GetOwner()				{ return m_owner; }

	// 소유자 설정
	void SetOwner(CPC* pc)		{ m_owner = pc; }

	// 펫 종류 구하기
	char GetPetType()			{ return (m_petTypeGrade & PET_TYPE_MASK); }

	// 펫 등급 구하기
	char GetPetGrade()			{ return (m_petTypeGrade & PET_GRADE_MASK); }

	// 펫을 탈수 있는지 검사
	bool IsMountType()			{ return (GetPetGrade() == PET_GRADE_MOUNT); }

	// 펫 종류 + 등급 구하기
	char GetPetTypeGrade()		{ return m_petTypeGrade; }

	// 스탯을 레벨에 맞게 초기화 : HP/MP 등 수치 최대치 설정
	void InitStat();

	// 현재 착용되어 있는지 검사
	bool IsWearing()			{ return (m_owner && m_owner->m_wearing[WEARING_PET] != NULL && m_owner->m_wearing[WEARING_PET]->m_plus == m_index); }

	// 현재 소환 되어 있는지 검사
	bool IsSummon()				{ return m_bSummon; }

	// 현재 타고 있는지 검사
	bool IsMount()				{ return m_bMount; }

	// 소환/타기 상태 리셋
	void ResetSummonMountFlag()	{ m_bSummon = false; m_bMount = false; }

	// 애완동물을 나타나게 함
	void Appear(bool bIncludeOwner);

	// 애완동물을 사라지게 함
	void Disappear();

	// 타고 내기리
	void Mount(bool bMount);

	// 060221 : bs : 펫 부활 남은 시간 설정
	void SetRemainRebirthTime(int nRemainRebirth = -1);

	// 060221 : bs : 펫 부활 남은 시간 반환
	int GetRemainRebirthTime()	{ return m_nRemainRebirth; }

	// 060309 : bw : 펫의 오너의 거리 구함
	void SetWasNear(bool bnear)	{ m_wasnear = bnear; }	
	bool WasNear()				{ return m_wasnear; }
	float GetDistanceFromOwner();
	void ChangePetType();

	//////////////
	// 배고픔 관련

	// 행동 불능 검사
	bool IsActive()				{ return (m_hungry > 0); }

	// 배고픔 수치 반환
	int GetHungryPoint()		{ return m_hungry; }

	// 배고픔 수치 변경, 수치 변경시 true 반환
	bool DecreaseHungryPoint();

	// 1회에 배고픔 수치 떨어지는 양 구하기
	int GetUnitForHungry()		{ return (m_owner->m_level < m_level) ? (m_level - m_owner->m_level) / 2 + 1 : 1; }

	// 배고픔 수치 증가, 수치 변경시 true 반환
	bool IncreaseHungryPoint(int val);


	//////////////
	// 교감도 관련

	// 교감도 수치 반환
	int GetSympathyPoint()		{ return m_sympathy; }

	// 교감도 수치 변경, 수치 변경시 true 반환, 050309: bw 근처에 있지 않으면 감소만함
	bool UpdateSympathyPoint(bool bnear = false);

	// 교감도 수치 올라가는 단위 시간 구하기
	int GetUnitForSympathy()	{ return (m_owner->m_level < m_level) ? (m_level - m_owner->m_level) / 2 + PULSE_PET_SYMPATHY_INCREASE : PULSE_PET_SYMPATHY_INCREASE; }

	// 교감도 상승
	bool IncreaseSympathyPoint(int val);


	////////////
	// 성장 관련

	// 레벨업 필요 경험치 반환
	LONGLONG GetNeedExp()		{ return (LONGLONG)(600 * pow(1.09, m_level)); }

	// 경험치 증가, 수치 변경/레벨업 시에 true
#ifdef NEW_PET_EXP_SYSTEM
	bool AddExp(int nOwnerLevel, int nNPCLevel, int nNPCFlag);
#else // NEW_PET_EXP_SYSTEM
	bool AddExp();
#endif // NEW_PET_EXP_SYSTEM

#ifdef PET_DIFFERENTIATION_ITEM
	void SetPetColor( char colorType = 0 )	{ m_petColorType = colorType; }
	char GetPetColor( void )				{ return m_petColorType; }
#endif // PET_DIFFERENTIATION_ITEM

#ifdef PET_TURNTO_NPC
	void SetPetTurnToNpc( int npcIndex =0 ) { m_nTurnToNpcIndex = npcIndex; }
	int	GetPetTurnToNpc( void )				{ return m_nTurnToNpcIndex; }
	void SetPetTurnToNpcSize( int size = 10 ) { m_nTurnToNpcSize = size; }
	int GetPetTurnToNpcSize( void )			{ return m_nTurnToNpcSize; }
#endif //PET_TURNTO_NPC

	// 레벨업 처리 : 레벨 변경이후 처리, 레벨/경험치/AP 등은 변경후 호출해야 함
	void LevelUp();

	// 기술 포인트 반환
	int GetAbilityPoint()		{ return m_abilityPoint; }

	// 기술 포인트 감소
	int DecreaseAbilityPoint(int val) { m_abilityPoint -= val; if (m_abilityPoint < 0) m_abilityPoint = 0; return m_abilityPoint; }

	// 기술 포인트 감소
	int IncreaseAbilityPoint(int val) { m_abilityPoint += val; return m_abilityPoint; }

	// 마운트 타입으로 변경
	void ChangeToMount()		{ m_petTypeGrade = GetPetType() | PET_GRADE_MOUNT; InitStat(); }

	// 펫 마운트 타입 취소
	void ResetMountType()		{ m_petTypeGrade = GetPetType() | PET_GRADE_ADULT; InitStat(); }


	////////////
	// 스킬 관련

	// 특정 인덱스 스킬 구하기
	CSkill* FindSkill(int skillindex);

	// 스킬 추가
	void AddSkill(CSkill*& skill);

	// 스킬 리스트 반환
	CSkillList* GetSkillList()	{ return &m_skillList; }

	// 스킬 초기화
	bool ResetSkill();


	/////////////////////////////
	// virtual function implement
	virtual int GetAttackLevel() const		{ return 1; }
	virtual int GetDefenseLevel() const		{ return 1; }
	virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const { return MSG_DAMAGE_MELEE; }
	virtual float GetSize()				{ return 0.5f; }
	virtual int GetUsingStat() const			{ return USING_STR; }

	// 패시브 스킬 적용 : Pet은 없음
	void ApplyPassive() {}

	// 스탯 재계산 : Pet은 없음
	void CalcStatus(bool bSend) {}

#ifdef ADULT_SERVER_NEW_BALANCE 
	virtual float GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type);
	virtual float GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type);
#endif // ADULT_SERVER_NEW_BALANCE
};
#endif // #ifdef ENABLE_PET


//// 대리 상인
//#ifdef ALTERNATE_MERCHANT
//class CAlternateMerchant : public CCharacter
//{
//private:	
//	// 상점 타입
//	char			m_cShopType;
//	// 상점 이름
//	CLCString		m_strShopName;
////	char			m_strShopName[PS_MAX_SHOPNAME];
//
//	// 주인 
//	CPC*			m_pShopOwner;
//
//
//	// 일반 아이템 인덱스 arr
//	int				m_nNormalItemIndex[PS_MAX_NORMAL_ITEM];
//	// 일반 아이템 카운트 arr
//	LONGLONG		m_lNormalItemCount[PS_MAX_NORMAL_ITEM];
//	// 일반 아이템 가격 arr
//	LONGLONG		m_lNormalItemPrice[PS_MAX_NORMAL_ITEM];
//
//
//	// 패키지 아이템 인덱스 arr
//	int				m_nPackageItemIndex[PS_MAX_PACKAGE_ITEM];
//	// 패키지 아이템 카운트 arr
//	LONGLONG		m_lPackageItemCount[PS_MAX_PACKAGE_ITEM];
//	// 패키지 아이템 가격
//	LONGLONG		m_lPackagePrice;
//
//
//	//물건을 팔고 가지고 있는 나스 
//	LONGLONG		m_lNasCount;
//
//	CSkillList	m_skillList;
//public:
//	// 리스트에서 인덱스
//	int				m_nlistIdx;		
//
//	CItem*				m_NormalItems[PS_MAX_PACKAGE_ITEM];
//	CItem*				m_PackItems[PS_MAX_PACKAGE_ITEM];
//
//
//public:
//	// 생성자
//	CAlternateMerchant( void );
//	CAlternateMerchant( CPC* owner, char cShopType, const char* strShopName );
//
//	// 아이템 추가
//	/*
//	* FunName		: AddItem
//	* Description	: 개인 상점 대행 NPC에게
//	*
//	*
//	*/
//	bool	AddItem( bool bNormal, int nItemIndex, LONGLONG lItemCount, LONGLONG lItemdPrice );
//
//	// 아이템 검색
//	CItem*		FindItem( bool bNormal, int nItemIndex );
//
//	// 아이템 삭제
//	void	RemoveNormalItem( int nItemIndex, LONGLONG lItemCount );
//
//	// 패키지 아이템 삭제
//	void	RemovePackage( void );
//
//	// 상점 타입 반환
//	char	GetShopType( void )					{ return m_cShopType; }
//
//	// 상점 이름 반환
//	const char* GetShopName( void )				{ return (const char*)m_strShopName; }
//
//	// 전체 판매 중인 일반 아이템 개수
//	char	 GetNormalCount( void );
//
//	// 일반 아이템 위치
//	int		GetNextNormalItem( int nPos );
//	int		GetItemPos( bool bNormal, int nItemIndex );
//
//	// 현재 클릭된 일반 아이템 인덱스  
//	int		GetNormalItemIndex( int nPos )			{ return (nPos < 0 || nPos >= PS_MAX_NORMAL_ITEM) ? -1 : m_nNormalItemIndex[nPos]; }
//
//	// 현재 클릭된 일반 아이템 갯수
//	LONGLONG GetNormalItemCount( int nPos )		{ return (nPos < 0 || nPos >= PS_MAX_NORMAL_ITEM) ? -1 : m_lNormalItemCount[nPos]; }
//
//	// 현재 클릭된 일반 아이템 가격
//	LONGLONG GetNormalItemPrice( int nPos )		{ return (nPos < 0 || nPos >= PS_MAX_NORMAL_ITEM) ? -1 : m_lNormalItemPrice[nPos]; }
//
//	// 패키지 아이템 가격
//	LONGLONG GetPackagePrice( void )			{ return m_lPackagePrice; }
//
//	// 전체 패키지 아이템 갯수
//	char	GetPackageCount( void );
//
//	// 패키지 아이템의 다음 아이템 가져 오기
//	int		GetNextPackageItem( int nPos );
//
//	// 패키지 아이템 인덱스 가져오기
//	int		GetPackageItemIndex( int nPos )			{ return (nPos < 0 || nPos >= PS_MAX_PACKAGE_ITEM) ? -1 : m_nPackageItemIndex[nPos]; }
//
//	// 패키지 아이템 갯수 가져오기
//	LONGLONG GetPackageItemCount( int nPos )	{ return (nPos < 0 || nPos >= PS_MAX_PACKAGE_ITEM) ? 0 : m_lPackageItemCount[nPos]; }
//
//	// 나스 추가
//	void	AddNas( LONGLONG lCount )			{ 	m_lNasCount = m_lNasCount + lCount; }
//
//	// 나스 반환
//	LONGLONG GetNas( void )						{ return m_lNasCount;	};
//
//	// 소유자 반환
//	CPC*	GetOwnerPC( void )					{ return m_pShopOwner;	};
//
//	// 아이템 사용시 소유자 설정
//	void	SetOwnerPC( CPC* pOwner )			{ m_pShopOwner = pOwner;	};
//
//	// 대리 상인 초기화
//	void	InitAlternateMerchant( void );
//
//	// 스탯 초기화
//	void	InitStat( void );
//
//	// 대리 상점 나타나게 함
//	void	AppearAlternateMerchant( bool bIncludeOwner );
//
//	// 대리 상점 사라지게 함
//	void	DisappearAlternateMerchant( void );
//
//
//	////////////
//	// 스킬 관련
//
//	// 특정 인덱스 스킬 구하기
//	CSkill* FindSkill(int skillindex)	{return NULL;}
//
//	// 스킬 추가
//	void AddSkill(CSkill*& skill);
//
//	// 스킬 리스트 반환
//	CSkillList* GetSkillList()	{ return &m_skillList; }
//
//	// 스킬 초기화
//	bool ResetSkill();
//
//
//	/////////////////////////////
//	// virtual function implement
//	virtual int GetAttackLevel() const		{ return 1; }
//	virtual int GetDefenseLevel() const		{ return 1; }
//	virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const { return MSG_DAMAGE_MELEE; }
//	virtual float GetSize()				{ return 0.5f; }
//	virtual int GetUsingStat() const			{ return USING_STR; }
//
//	// 패시브 스킬 적용 : Pet은 없음
//	void ApplyPassive() {}
//
//	// 스탯 재계산 : Pet은 없음
//	void CalcStatus(bool bSend) {}
//
//};
//
//
//class CAlternateMerchantList
//{
//public: 
//	CAlternateMerchant* m_pAlternateMerchantList[MAX_PLAYER_LIST];		//  배열로 사용
//	int	m_nMax;
//	int m_nCur;
//
//	CAlternateMerchantList()
//	{
//		int i;
//		for ( i = 0; i < MAX_PLAYER_LIST; i++ )
//			m_pAlternateMerchantList[i] = NULL;
//		m_nMax = 0;
//		m_nCur = 0;
//	}
//
//	~CAlternateMerchantList()
//	{
//		m_nMax = m_nCur = 0;
//	}
//
//	int AddAlternateMerchant( CAlternateMerchant* pAlternateMerchant )
//	{
//		int i;
//		if (m_nMax >= MAX_PLAYER_LIST )
//			return -1;
//
//		if (pAlternateMerchant->m_nlistIdx != -1 && m_pAlternateMerchantList[pAlternateMerchant->m_nlistIdx] == pAlternateMerchant) 
//			return pAlternateMerchant->m_nlistIdx;
//
//		for (i = 0; i < m_nMax && m_pAlternateMerchantList[i]; i++)
//			;
//
//		m_pAlternateMerchantList[i] = pAlternateMerchant;
//		pAlternateMerchant->m_nlistIdx = i;
//		if (i == m_nMax)
//			m_nMax++;
//
//		m_nMax++;
//
//		return i;
//	}
//
//	void RemoveAlternateMerchant( CAlternateMerchant* pAlternateMerchant )
//	{
//		int i;
////		DelAttackList(pAlternateMerchant);
//
//		i = pAlternateMerchant->m_nlistIdx;
//		if (i < 0 || i >= MAX_PLAYER_LIST)	return ;
//
//		m_pAlternateMerchantList[i] = NULL;
//		pAlternateMerchant->m_nlistIdx = -1;
//		if (i == m_nMax) m_nMax--;
//		m_nCur--;
//	}
//
//	CAlternateMerchant* Find(int nAlternateMerchantIndex)
//	{
//		int i;
//
//		for (i = 0; i < m_nMax; i++)
//			if (m_pAlternateMerchantList[i] && m_pAlternateMerchantList[i]->m_nlistIdx == nAlternateMerchantIndex )
//				return m_pAlternateMerchantList[i];
//
//		return NULL;
//	}
//
//};
//#endif // ALTERNATE_MERCHANT

class CElemental : public CCharacter
{
	static int	m_nVirtualIndex;			// 가상 인덱스

	CPC*		m_owner;					// 소유자

	char		m_elementalType;			// 소환수 종류

	int			m_defStr;					// 기본 스탯
	int			m_defDex;
	int			m_defInt;
	int			m_defCon;

	int			m_defHP;					// 기본 HP

	int			m_defAttackLevel;			// 기본 공방
	int			m_defDefenseLevel;

	int			m_pulseRemain;				// 소환 남은 시간

	CSkillList	m_skillList;				// 정령 스킬 리스트 (액티브/패시브)

public:
	bool		m_bChangeStatus;			// 상태변화여부

	CElemental*	m_prevElemental;
	CElemental*	m_nextElemental;

public:
	CElemental(CPC* owner, char elementalType);

	// 소유자 반환
	CPC* GetOwner() { return m_owner; }

	// 소환수 타입 반환
	char GetElementalType() { return m_elementalType; }

	// 소환수 남은 시간 반환
	int GetRemainTime() { return m_pulseRemain; }

	// 소환수 남은 시간 감소 : 소환 다되면 true 반환
	bool DecreaseRemainTime() { m_pulseRemain--; if (m_pulseRemain < 0) m_pulseRemain = 0; return (m_pulseRemain <= 0) ? true : false; }

	// 소환수 스킬 추가
	void AddSkill(int skillindex, int level);

	// 소환수 능력치 초기화 : 현재 hp 등은 유지
	void ResetStat();

	// 능력치 재계산
	void CalcStatus(bool bSend);

private:
	int GetVirtualIndex() { if (m_nVirtualIndex < 1) m_nVirtualIndex = 1; return CElemental::m_nVirtualIndex++; }

	// virtual functions
public:
	int GetAttackLevel() const { return m_defAttackLevel; }
	int GetDefenseLevel() const { return m_defDefenseLevel; }
	MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const;
	float GetSize() { return 0.0f; }
	int GetUsingStat() const { return USING_DEX; }
	CSkill* FindSkill(int skillindex);
	void ApplyPassive();

#ifdef ADULT_SERVER_NEW_BALANCE
	virtual float GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type);
	virtual float GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type);
#endif // ADULT_SERVER_NEW_BALANCE
};

#ifdef ATTACK_PET

#ifdef APET_AI
typedef struct __tagApetAIData
{
	int		nItemNum;		// DB index
//	int		nItemIdx;		// Unique index
	CItem * pItem;
	int		nSkillIdx;
	int		nSkillType;		// skill - 0, action - 1
	char	cUse;
} APET_AI_DATA;
#endif

class CAPet : public CCharacter
{
protected:
	CPC* m_pOwner;

	int		m_nHpMpPulse;
	int		m_nStmPulse;
	int		m_nDStmPluse;
	int		m_nFaithPulse;
	int		m_nDFaithPulse;

	int m_nFaith;
	int m_nStm;

public:

	CAPet();
	~CAPet();

	// List
	CAPet*		m_pPrevPet;
	CAPet*		m_pNextPet;

	bool m_bSummon;			// 소환되었는지 여부

	int m_nSP;
	int m_nRemainStat;

	int m_nSeal;

	int m_nPlusStr;
	int m_nPlusCon;
	int m_nPlusDex;
	int m_nPlusInt;

	int m_nAttack;
	int	m_nMagicAttack;
	int m_nDefence;
	int m_nMagicDefence;

	int m_nHitPoint;
	int m_nMagicHitPoint;
	int m_nAvoid;
	int m_nMagicAvoid;
	
	int	m_nOpHP, m_nOpMP;

	CAPetProto*	m_pProto;
	CSkillList	m_skillList;			// Skill only  Active
	CItem*		m_wearing[APET_WEARPOINT];

#ifdef APET_AI
	char m_cAI;
	int  m_nAICount;
	int  m_nAISlot;
	APET_AI_DATA m_tAIData[APET_MAX_AI];
#endif

	// Function
	void	SetOwner(CPC* pc)	{ m_pOwner = pc; }			// 소유자 설정
	void	SetFaith(int faith) { m_nFaith = faith; }		// 충성도 설정
	void	SetStamina(int stm) { m_nStm = stm; }			// 스테미너 설정
	void	AddFaith(int faith)	{ m_nFaith+=faith; if(m_nFaith<0)m_nFaith=0; if(m_nFaith>m_pProto->m_nMaxFaith) m_nFaith=m_pProto->m_nMaxFaith; } // 충성도 증가 ( 예외처리 )
	void	AddStamina(int stm)	{ m_nStm+=stm; if(m_nStm<0)m_nStm=0; if(m_nStm>m_pProto->m_nMaxStm) m_nStm=m_pProto->m_nMaxStm;} // 스테미너 증가 ( 예외처리 )

	CPC*	GetOwner()			{ return m_pOwner; }		// 소유자 반환
	int		GetFaith()			{ return m_nFaith; }		// 충성도
	int		GetStamina()		{ return m_nStm; }			// 스테미너 
	int		GetStmLevel();									// 기아상태
	int		GetFaithLevel();								// 충성도 상태

	bool	IsSummon()			{ return m_bSummon; }		// 작용 소환 여부

	CItem*	AddToWearItem( CItem* item );					// Pet 방어구등 장착 
	void	Appear(bool bIncludeOwner);
	void	Disappear();

	bool	IsWearing()			{ return ( m_pOwner && m_pOwner->m_wearing[WEARING_PET] != NULL && m_pOwner->m_wearing[WEARING_PET]->m_plus == m_index); }
	bool	IsPetEquip()		{ return ( m_wearing[APET_WEAR_HEAD] || m_wearing[APET_WEAR_BODY] || m_wearing[APET_WEAR_WEAPON] || m_wearing[APET_WEAR_ACCE] )? true : false; }

	bool	IncreaseStat();			// 시간에 따라 증가되는 수치 
	bool	DecreaseStat();			// 시간에 따라 감소되는 수치
	void	CheckItemTime();		// 펫장비 아이템 시간 확인

	void	UpdateStatPlus( int nStr, int nDex, int nInt , int nCon );

	LONGLONG	GetNeedExp()	{ return (LONGLONG)(600.0 * pow(1.09,m_level)); }

	bool		AddExpSP(LONGLONG exp, int sp);
	void		LevelUp(bool bSendEffect);	// 레벨업 효과
	
	void		ApplyItemValue();			// 아이템 능력치 적용

	// Virtual Function
	////////////////////
	// Function name	: GetAttackLevel
	// Description	    : 무기레벨 얻기.
	virtual int GetAttackLevel() const;

	////////////////////
	// Function name	: GetDefenseLevel
	// Description	    : 방어구 레벨 얻기
	virtual int GetDefenseLevel() const;

	////////////////////
	// Function name	: GetAttackType
	// Description	    : 공격 타입 얻기, 스킬 프로토가 있으면 스킬의 타입, NULL이면 기본 타입
	virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const ;

	////////////////////
	// Function name	: GetSize
	// Description	    : 캐릭터 크기 반환
	virtual float GetSize() { return 0; }

	////////////////////
	// Function name	: GetUsingStat
	// Description	    : 공격시 사용하는 스탯
	virtual int GetUsingStat() const { if( m_wearing[WEARING_WEAPON]) return m_wearing[WEARING_WEAPON]->GetUsingStat(); else return USING_STR; }

	virtual CSkill* FindSkill(int skillindex);
	// 패시브 스킬 적용
	virtual void ApplyPassive() { return; }
	// 스탯 재계산
	virtual void CalcStatus(bool bSend);

#ifdef ADULT_SERVER_NEW_BALANCE
	virtual float GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type);
	virtual float GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type);
#endif // ADULT_SERVER_NEW_BALANCE

#ifdef APET_AI
	bool IsAI() { return (m_cAI == '1') ? true:false; }
	bool IsEmptyAILine(int nIndex) { return (!m_tAIData[nIndex].pItem || m_tAIData[nIndex].nSkillIdx < 0) ? true:false; }

	bool DelAIList(CItem * pItem);
	void CheckAIList();
#endif
};
#endif //ATTACK_PET

bool AddToInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int row = -1, int col = -1);
bool DecreaseFromInventory(CPC* ch, CItem* item, LONGLONG count);
bool RemoveFromInventory(CPC* ch, int tab, int row, int col, bool bFreeMem, bool removeQuickSlot);
bool RemoveFromInventory(CPC* ch, CItem* item, bool bFreeMem, bool removeQuickSlot);
bool AddToPreInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int tab, int row = -1, int col = -1);
void CheckQuestCondition(CPC* ch, CItem* item, LONGLONG itemSum);


#endif
