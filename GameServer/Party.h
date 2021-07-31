#ifndef __PARTY_H__
#define __PARTY_H__

#define		PARTY_TYPE_RANDOM		0		// 균등 파티
#define		PARTY_TYPE_FIRSTGET		1		// 입수 우선
#define		PARTY_TYPE_BATTLE		2		// 전투형
#define		PARTY_TYPE_OPENBOX		3		// 상자열기

#define		MAX_PARTY_DIVITYPE		3		// 분배방법 수

class CNetMsg;
class CMonsterCombo;

#ifdef PARTY_MATCHING

#define PARTY_MATCHING_COMMENT			50		// 파티 매칭 파티 등록시 설명문구 길이
#define PARTY_MATCHING_DATA_PER_PAGE	10		// 파티 매칭 리스트에서 페이지당 표시할 데이터 수
#define PARTY_MATCHING_LIMIT_LEVEL		2		// 파티 매칭에서 레벨 제한 범위

class CPartyMatchMember
{
#ifdef __GAME_SERVER__
#else
	int			m_nSubNo;
#endif // __GAME_SERVER__
	int			m_nIndex;
	CLCString	m_strName;
	int			m_nLevel;
	int			m_nZone;
	char		m_nJob;
	char		m_nPartyType;

public:
#ifdef __GAME_SERVER__
	CPartyMatchMember(int nIndex, const char* strName, int nLevel, int nZone, char nJob, char nPartyType)
#else
	CPartyMatchMember(int nSubNo, int nIndex, const char* strName, int nLevel, int nZone, char nJob, char nPartyType)
#endif // __GAME_SERVER__
	: m_strName(MAX_CHAR_NAME_LENGTH + 1)
	{
#ifdef __GAME_SERVER__
#else
		m_nSubNo = nSubNo;
#endif // __GAME_SERVER__
		m_nIndex		= nIndex;
		m_strName		= strName;
		m_nLevel		= nLevel;
		m_nZone			= nZone;
		m_nJob			= nJob;
		m_nPartyType	= nPartyType;
	}

#ifdef __GAME_SERVER__
#else
	int			GetSubNo() const		{ return m_nSubNo; }
#endif // __GAME_SERVER__
	int			GetIndex() const		{ return m_nIndex; }
	const char*	GetName() const			{ return (const char*)m_strName; }
	int			GetLevel() const		{ return m_nLevel; }
	int			GetZone() const			{ return m_nZone; }
	char		GetJob() const			{ return m_nJob; }
	char		GetPartyType() const	{ return m_nPartyType; }

	void		SetName(const char* strName)		{ m_strName = strName; }
	void		SetLevel(int nLevel)				{ m_nLevel = nLevel; }
	void		SetZone(int nZone)					{ m_nZone = nZone; }

	static int CompByIndex(CPartyMatchMember* p1, CPartyMatchMember* p2)
	{
#ifdef __GAME_SERVER__
		return p1->GetIndex() - p2->GetIndex();
#else
		if (p1->GetSubNo() == p2->GetSubNo())
			return p1->GetIndex() - p2->GetIndex();
		else
			return p1->GetSubNo() - p2->GetSubNo();
#endif // __GAME_SERVER__
	}
};

class CPartyMatchParty
{
#ifdef __GAME_SERVER__
#else
	int			m_nSubNo;
#endif // __GAME_SERVER__
	int			m_nBossIndex;
	CLCString	m_strBossName;
	int			m_nBossLevel;
	int			m_nZone;
	int			m_nJobFlag;
	char		m_nPartyType;
	bool		m_bLimitLevel;
	CLCString	m_strComment;

public:
#ifdef __GAME_SERVER__
	CPartyMatchParty(int nBossIndex, const char* strBossName, int nBossLevel, int nZone, int nJobFlag, char nPartyType, bool bLimitLevel, const char* strComment)
#else
	CPartyMatchParty(int nSubNo, int nBossIndex, const char* strBossName, int nBossLevel, int nZone, int nJobFlag, char nPartyType, bool bLimitLevel, const char* strComment)
#endif // __GAME_SERVER__
	: m_strBossName(MAX_CHAR_NAME_LENGTH + 1)
	, m_strComment(PARTY_MATCHING_COMMENT + 1)
	{
#ifdef __GAME_SERVER__
#else
		m_nSubNo = nSubNo;
#endif // __GAME_SERVER__
		m_nBossIndex = nBossIndex;
		m_strBossName = strBossName;
		m_nBossLevel = nBossLevel;
		m_nZone = nZone;
		m_nJobFlag = nJobFlag;
		m_nPartyType = nPartyType;
		m_bLimitLevel = bLimitLevel;
		m_strComment = strComment;
	}

#ifdef __GAME_SERVER__
#else
	int			GetSubNo() const			{ return m_nSubNo; }
#endif // __GAME_SERVER__
	int			GetBossIndex() const		{ return m_nBossIndex; }
	const char*	GetBossName() const			{ return (const char*)m_strBossName; }
	int			GetBossLevel() const		{ return m_nBossLevel; }
	int			GetZone() const				{ return m_nZone; }
	int			GetJobFlag() const			{ return m_nJobFlag; }
	char		GetPartyType() const		{ return m_nPartyType; }
	bool		IsLimitLevel() const		{ return m_bLimitLevel; }
	const char*	GetComment() const			{ return (const char*)m_strComment; }

	void		SetBossIndex(int nBossIndex)			{ m_nBossIndex = nBossIndex; }
	void		SetBossName(const char* strName)		{ m_strBossName = strName; }
	void		SetBossLevel(int nLevel)				{ m_nBossLevel = nLevel; }
	void		SetZone(int nZone)						{ m_nZone = nZone; }

	static int CompByBossIndex(CPartyMatchParty* p1, CPartyMatchParty* p2)
	{
#ifdef __GAME_SERVER__
		return p1->GetBossIndex() - p2->GetBossIndex();
#else
		if (p1->GetSubNo() == p2->GetSubNo())
			return p1->GetBossIndex() - p2->GetBossIndex();
		else
			return p1->GetSubNo() - p2->GetSubNo();
#endif // __GAME_SERVER__
	}
};
#endif // PARTY_MATCHING

// 파티 멤버
class CPartyMember
{
	int				m_nCharIndex;
	CLCString		m_strCharName;
#ifdef __GAME_SERVER__
	CPC*			m_pChar;
#endif // __GAME_SERVER__

public:
#ifdef __GAME_SERVER__
	CPartyMember(int nCharIndex, const char* strCharName, CPC* pc);
#else
	CPartyMember(int nCharIndex, const char* strCharName);
#endif // __GAME_SERVER__
	~CPartyMember();

	int GetCharIndex() const			{ return m_nCharIndex; }
	const char* GetCharName() const		{ return (const char*)m_strCharName; }

#ifdef __GAME_SERVER__
	CPC* GetMemberPCPtr() const			{ return m_pChar; }
	void SetMemberPCPtr(CPC* pc);
#endif // __GAME_SERVER__
};

class CParty
{
	char			m_nPartyType[MAX_PARTY_DIVITYPE];			// 경험치 분배 타입	: 0 - 균등, 1 - 입수우선, 2 - new파티
	int				m_nCount;
	CPartyMember*	m_listMember[MAX_PARTY_MEMBER];
	int				m_nRequest;
	CLCString		m_strRequest;

#ifndef __GAME_SERVER__
	int				m_nSubNo;
#endif // #ifndef __GAME_SERVER__


public:

#ifdef __GAME_SERVER__
	CParty(char nPartyType, int nBossIndex, const char* strBossName, CPC* pBoss, int nRequest, const char* strRequest);
#else
	CParty(int nSubNo, char nPartyType, int nBossIndex, const char* strBossName, int nRequest, const char* strRequest);
#endif // __GAME_SERVER__

	~CParty();

	int GetRequestIndex() const			{ return m_nRequest; }
	const char* GetRequestName() const	{ return (const char*)m_strRequest; }
	int GetMemberCount() const			{ return m_nCount; }
	char GetPartyType(int nType) const	{ if(nType < 0  && nType >= MAX_PARTY_DIVITYPE) return -1;  return m_nPartyType[nType]; }

	// boss 인덱스 반환
	int GetBossIndex() const			{ return (m_listMember[0]) ? m_listMember[0]->GetCharIndex() : -1; }

	// 해당 캐릭터 인덱스를 갖는 파티원의 리스트 인덱스를 구한다
	int FindMember(int nCharIndex);

	// 해당 리스트 인덱스의 멤버 Char index를 구한다.
	int GetMemberCharIndex(int nListIndex);

	static int CompParty(CParty* d1, CParty* d2);

	void SetRequest(int nRequest, const char* strRequest);

	// request를 멤버로 참여
#ifdef __GAME_SERVER__
	int JoinRequest(const char* strRequestName, CPC* pRequest);
#else
	int JoinRequest(const char* strRequestName);
#endif // __GAME_SERVER__

	// 해당 캐릭터를 제거, 제거하거나 없으면 true, 제거에 실패하면 false
	void DeleteMember(int nCharIndex);

	// list index로 CPartyMember 얻기
	const CPartyMember* GetMemberByListIndex(int nListIndex) const;

	bool ChangeBoss(const char* strNewBossName);

	void SetPartyType(int nType,char PartyType)		{ if(nType < 0  && nType >= MAX_PARTY_DIVITYPE) return; m_nPartyType[nType] = PartyType; }

#ifdef __GAME_SERVER__

	int GetNearPartyMemberCount(CCharacter* pCenter);

	// pPC 가까운 곳에 있는 nListIndex번째 파티원을 구한다
	CPC* GetNearMember(CCharacter* pPC, int nListIndex);

	// 해당 캐릭터인덱스의 멤버의 PC 포인터를 설정한다
	void SetMemberPCPtr(int nCharIndex, CPC* pPC);

	// 현재 PC가 설정되어 있는 모든 파티원에게 메시지를 전달, nExcludeCharIndex 제외
	void SendToAllPC(CNetMsg& msg, int nExcludeCharIndex = -1);

	// 같은 존에 있는 PC에게만 메시지 전달
	void SendToPCInSameZone(int nZone, int nArea, CNetMsg& msg);

#else

	int GetSubNo() const		{ return m_nSubNo; }

#endif // __GAME_SERVER__

#ifdef __GAME_SERVER__
	// 게임서버 FRIEND 함수
	friend void ProcHelperPartyInfoParty(CNetMsg& msg);
#endif // __GAME_SERVER__

#ifdef GMTOOL
	bool		m_bGmtarget;
#endif // GMTOOL

#ifdef __GAME_SERVER__
#ifdef MONSTER_COMBO
	int m_comboAreaIndex;
#endif // MONSTER_COMBO

#ifdef EXTREME_CUBE
	int	m_cubeUniqueIdx;
#endif // EXTREME_CUBE
#endif // __GAME_SERVER__
};

#endif