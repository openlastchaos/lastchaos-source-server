#ifndef __EXPEDITION_H__
#define __EXPEDITION_H__

#ifdef EXPEDITION_RAID

#define	EXPED_TYPE_RANDOM			0		// 분배방식(균등) 
#define	EXPED_TYPE_FIRSTGET			1		// 분배방식(입수우선)
#define	EXPED_TYPE_BATTLE			2		// 분배방식(전투형)

#define	EXPED_DIVITYPE_EXP			0		// 원정대 분배 방법(경험치)
#define	EXPED_DIVITYPE_ITEM			1		// 원정대 분배 방법(아이템)
#define	EXPED_DIVITYPE_SPECIAL		2		// 원정대 분배 방법(스페셜 아이템)

#define MAX_EXPED_MEMBER			32		// 원정대 최대 인원수
#define MAX_EXPED_GMEMBER			8		// 원정대 1개 그룹 최대 인원수
#define MAX_EXPED_GROUP				4		// 원정대내 그룹 수	
#define MAX_EXPED_DIVITYPE			3		// 분배방법 수


class CNetMsg;

// 원정대 멤버 클래스
class CExpedMember
{
	int				m_nCharIndex;			// 캐릭터 인덱스 
	CLCString		m_strCharName;			// 캐릭터 이름		
	int				m_nGroupType;			// 그룹 타입 
	int				m_nMemberType;			// 멤버 타입
	int				m_nLabelType;			// 표식
	int				m_nListIndex;			// 그룹내 배열 인덱스
	int				m_nQuitType;			// 정상,비정상 구분
#ifdef __GAME_SERVER__
	CPC*			m_pChar;
#endif // __GAME_SERVER__

public:
#ifdef __GAME_SERVER__
	CExpedMember(int nCharIndex, const char* strCharName, int nGroupType, int nMemberType, int nListIndex, CPC* pc);
#else
	CExpedMember(int nCharIndex, const char* strCharName, int nGroupType, int nMemberType, int nListIndex);
#endif // __GAME_SERVER__
	~CExpedMember();

	int GetCharIndex() const			{ return m_nCharIndex; }
	const char* GetCharName() const		{ return (const char*)m_strCharName; }
	int GetGroupType() const			{ return m_nGroupType; }
	int GetMemberType() const			{ return m_nMemberType; }
	int GetLabelType() const			{ return m_nLabelType; }
	int GetQuitType() const				{ return m_nQuitType; }
	int GetListIndex() const			{ return m_nListIndex; }	

	void SetCharIndex(int nCharIndex);
	void SetCharName(const char* strCharName);
	void SetGroupType(int nGroupType);
	void SetMemberType(int nMemberType);	
	void SetLabelType(int nLabelType);
	void SetQuitType(int nQuitType);
	
#ifdef __GAME_SERVER__
	CPC* GetMemberPCPtr() const			{ return m_pChar; }
	void SetMemberPCPtr(CPC* pc);
#endif // __GAME_SERVER__
};


//원정대 클래스
class CExpedition
{
	char			m_nExpedType[MAX_EXPED_DIVITYPE];					// 분배 방법(경험치,일반 아이템,스폐셜 아이템)
	int				m_nTotCount;										// 원정대 인원수
	int				m_nGTotCount[MAX_EXPED_GROUP];						// 그룹 인원수
	CExpedMember*	m_listMember[MAX_EXPED_GROUP][MAX_EXPED_GMEMBER];	// 4개 그룹, 8명 
	int				m_nRequestIndex;									// 요청자 캐릭터 인덱스
	CLCString		m_strRequestName;									// 현재 요청자 캐릭터 이름

#ifndef __GAME_SERVER__
	int				m_nSubNo;
#endif // #ifndef __GAME_SERVER__

public:

#ifdef __GAME_SERVER__
	CExpedition(char nExpeditionType, int nBossIndex, const char* strBossName, int nGroupType, int nMemberType, CPC* pBoss);
#else
	CExpedition(int nSubNo, char nExpeditionType, int nBossIndex, const char* strBossName, int nGroupType, int nMemberType);
#endif // __GAME_SERVER__

	~CExpedition();

	int GetRequestIndex() const				{ return m_nRequestIndex; }
	const char* GetRequestName() const		{ return (const char*)m_strRequestName; }
	int GetMemberCount() const				{ return m_nTotCount; }
	char GetExpeditionType(int nType);

	// boss 인덱스 반환
	int GetBossIndex();
	const char* GetBossName();
	int GetBossGroupType();

	// 해당 캐릭터 인덱스를 갖는 파티원의 리스트 인덱스를 구한다
	int FindMemberListIndex(int nCharIndex);
	int FindMemberGroupIndex(int nCharIndex);
	
	// 해당 리스트 인덱스의 멤버 Char index를 구한다.
	int GetMemberCharIndex(int nGroup, int nListIndex);

	static int CompExpedition(CExpedition* d1, CExpedition* d2);

	void SetRequest(int nRequest, const char* strRequest);

	// request를 멤버로 참여
#ifdef __GAME_SERVER__
	const CExpedMember* JoinRequest(const char* strRequestName, int nMemberType, CPC* pRequest);
#else
	const CExpedMember* JoinRequest(const char* strRequestName, int nMemberType);
#endif // __GAME_SERVER__

	// 멤버 참여
#ifdef __GAME_SERVER__
	 const CExpedMember* Join(int nCharIndex, const char* strCharName, int nMemberType, CPC* pRequest);
#else
	const CExpedMember* Join(int nCharIndex, const char* strCharName, int nMemberType);
#endif // __GAME_SERVER__


	// 해당 캐릭터를 제거, 제거하거나 없으면 true, 제거에 실패하면 false
	void DeleteMember(int nCharIndex);

	// char index로 CExpeditionMember 얻기
	const CExpedMember* GetMemberByCharIndex(int nCharIndex) const;

	// list index로 CExpeditionMember 얻기
	const CExpedMember* GetMemberByListIndex(int nGroup, int nListIndex) const;

	const CExpedMember* CExpedition::GetMemberByListIndex(int nGroup, int nListIndex, int nMemberType) const;	

	bool ChangeBoss(int nTargetIndex, int nOldBossIndex,bool bOldReset);

	void SetExpedType(int nType,char ExpedType)		{ if(nType < 0  && nType >= MAX_EXPED_DIVITYPE) return; m_nExpedType[nType] = ExpedType; }

	int GetGroupMemberCount(int nGroupType);
	int GetGroupBeEmptySlot(void);

	int GetGroupMembertypeCount(int nGroupType, int nMemberType);
	bool SetMBoss(int nNewMBossIndex);
	bool ResetMBoss(int nNewMBossIndex);
	
	bool MoveGroup(int nSourceGroup, int nMoveCharIndex, int nTargetGroup, int nTargetListindex);
	CExpedMember* FindNextBoss(void);

#ifdef __GAME_SERVER__

	int GetNearExpeditionMemberCount(CCharacter* pCenter);

	// pPC 가까운 곳에 있는 nListIndex번째 파티원을 구한다
	CPC* GetNearMember(CCharacter* pPC,int nGroup, int nListIndex);

	// 해당 캐릭터인덱스의 멤버의 PC 포인터를 설정한다
	void SetMemberPCPtr(int nCharIndex, CPC* pPC);

	// 현재 PC가 설정되어 있는 모든 원정대원에게 메시지를 전달, nExcludeCharIndex 제외
	void SendToAllPC(CNetMsg& msg, int nExcludeCharIndex = -1);

	// 같은 존에 있는 PC에게만 메시지 전달
	void SendToPCInSameZone(int nZone, int nArea, CNetMsg& msg);
	
	// 같은 그룹 PC에게만 메시지 전달
	void SendToPCInSameGroup(int nGroup, CNetMsg& msg);

#else

	int GetSubNo() const		{ return m_nSubNo; }

#endif // __GAME_SERVER__

};

#endif

#endif // EXPEDITION_RAID