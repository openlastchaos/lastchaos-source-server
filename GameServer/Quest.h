// Quest.h: interface for the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEST_H__0F547455_B4DB_4214_8DA3_2F5F2BE49117__INCLUDED_)
#define AFX_QUEST_H__0F547455_B4DB_4214_8DA3_2F5F2BE49117__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CQuest;

// 퀘스트 프로토 타입
class CQuestProto
{
public:

	// DB
	int			m_index;							// 퀘스트 DB 번호
	int			m_type[2];							// 퀘스트 종류 : 방식/횟수에 따른 분류

	int			m_startType;						// 시작 방법
	int			m_startData;						// 시작 방법에 따른 데이타
	int			m_prizeNPC;							// 퀘스트를 완료시키는 NPC

	// 필요조건 : 퀘스트 성립 조건
	int			m_needMinLevel;							// 최소레벨
	int			m_needMaxLevel;							// 최대레벨
	int			m_needJob;								// 직업
	int			m_needItemIndex[QUEST_MAX_NEED_ITEM];	// 필요아이템 인덱스 : 없으면 -1
	int			m_needItemCount[QUEST_MAX_NEED_ITEM];	// 필요아이템 수량
	
	int			m_prequestNum;							//선행퀘스트 번호
	int			m_startNpcZoneNum;						//시작 NPC 존번호
	int			m_prizeNpcZoneNum;						//보상 NPC 존번호
	int			m_needExp;								//필요 경험치

	// 수행 조건 : 퀘스트 수행 조건
	int			m_conditionType[QUEST_MAX_CONDITION];	// 수행 종류
	int			m_conditionIndex[QUEST_MAX_CONDITION];	// 수행 종류 인덱스
	int			m_conditionNum[QUEST_MAX_CONDITION];	// 수행 수
	int			m_conditionData[QUEST_MAX_CONDITION][QUEST_MAX_CONDITION_DATA];	// 수행 조건 부가 데이타

	// 보상 조건
	int			m_prizeType[QUEST_MAX_PRIZE];		// 보상 종류
	int			m_prizeIndex[QUEST_MAX_PRIZE];		// 보상 인덱스
	int			m_prizeData[QUEST_MAX_PRIZE];		// 보상 데이타

	int			m_optPrizeType[QUEST_MAX_PRIZE];		// 선택 보상 종류
	int			m_optPrizeIndex[QUEST_MAX_PRIZE];		// 선택 보상 인덱스
	int			m_optPrizeData[QUEST_MAX_PRIZE];		// 선택 보상 데이타


	CQuestProto();
};

class CQuestProtoList
{
public:
	CQuestProto*	m_proto;		// 퀘스트 프로토 리스트
	int				m_nCount;		// 퀘스트 수

	CQuestProtoList();
	~CQuestProtoList();

	bool Load();							// DB 읽기
	CQuestProto* FindProto(int index);		// 퀘스트 proto 찾기

#ifdef QUEST_DATA_EXTEND
#else
	CQuest* Create(int index);	// Quest 생성
#endif // QUEST_DATA_EXTEND

	protected:
	static int CompIndex(const void* p1, const void* p2)
	{
		CQuestProto* i1 = (CQuestProto*)p1;
		CQuestProto* i2 = (CQuestProto*)p2;

		return i1->m_index - i2->m_index;
	}
};






#ifdef QUEST_DATA_EXTEND

class CQuest
{
	friend class CQuestList;

	const CQuestProto*	m_pQuestProto;
	char				m_cQuestState;
	int					m_nQuestValue[QUEST_MAX_CONDITION];
	bool				m_bComplete1;
	bool				m_bComplete2;

	CQuest*				m_pPrev;
	CQuest*				m_pNext;

public:
	CQuest(const CQuestProto* pQuestProto);
	~CQuest();

	void SetPrevQuest(CQuest* pPrev)		{ m_pPrev = pPrev; }
	void SetNextQuest(CQuest* pNext)		{ m_pNext = pNext; }
	CQuest* GetPrevQuest()					{ return m_pPrev; }
	CQuest* GetNextQuest()					{ return m_pNext; }

	char GetQuestState()					{ return m_cQuestState; }

	const CQuestProto* GetQuestProto()		{ return m_pQuestProto; }
	int GetQuestIndex()						{ return (m_pQuestProto) ? m_pQuestProto->m_index : -1; }
	int GetQuestType0()						{ return (m_pQuestProto) ? m_pQuestProto->m_type[0] : -1; }
	int GetQuestType1()						{ return (m_pQuestProto) ? m_pQuestProto->m_type[1] : -1; }
	int GetPrizeNPCIndex()					{ return (m_pQuestProto) ? m_pQuestProto->m_prizeNPC : -1; }
	int GetPrizeNPCZone()					{ return (m_pQuestProto) ? m_pQuestProto->m_prizeNpcZoneNum : -1; }

	bool IsTutorialQuest()					{ return (m_pQuestProto && m_pQuestProto->m_type[0] == QTYPE_KIND_TUTORIAL) ? true : false; }
	bool IsPetQuest()						{
												return (	m_pQuestProto->m_index ==  PET_HORSE_QUEST_INDEX
														||	m_pQuestProto->m_index ==  PET_DRAGON_QUEST_INDEX
														||	m_pQuestProto->m_index ==  PET_BLUE_HORSE_QUEST_INDEX
														||	m_pQuestProto->m_index ==  PET_PINK_DRAGON_QUEST_INDEX
														||	m_pQuestProto->m_index ==  PET_UNKOWN_HORSE_QUEST_INDEX
														||	m_pQuestProto->m_index ==  PET_UNKOWN_DRAGON_QUEST_INDEX);
											}
	void SetComplete1(bool bComplete1)		{ m_bComplete1 = bComplete1; }
	void SetComplete2(bool bComplete2)		{ m_bComplete2 = bComplete2; }
	bool GetComplete1()						{ return m_bComplete1; }
	bool GetComplete2()						{ return m_bComplete2; }
	bool IsCompleted()						{ return (m_bComplete1 && m_bComplete2); }

	void IncreaseQuestValue(int idx, int val = 1)	{ if (idx >= 0 && idx < QUEST_MAX_CONDITION) m_nQuestValue[idx] += val; }
	void SetQuestValue(int idx, int val)				{ if (idx >= 0 && idx < QUEST_MAX_CONDITION) m_nQuestValue[idx] = val; }
	int GetQuestValue(int idx)						{ return (idx >= 0 && idx < QUEST_MAX_CONDITION) ? m_nQuestValue[idx] : 0; }

	void QuestUpdateData(CPC* pPC, CNPC* pNPC);

	void RemoeQuestItem(CPC* pc);

private:
	void SetQuestState(char cQuestState)	{ m_cQuestState = cQuestState; }
};

class CQuestList
{
	CQuest*			m_head;
	int				m_nCountRun;
	int				m_nCountDone;
	int				m_nCountAbandon;
	int				m_nCountComplete;

public:
	CQuestList();
	~CQuestList();

	int GetCountRun()						{ return m_nCountRun; }
	int GetCountDone()						{ return m_nCountDone; }
	int GetCountAbandon()					{ return m_nCountAbandon; }

	void DecreaseQuestComepleteCount()		{ if (m_nCountComplete > 0) m_nCountComplete--; }
	void IncreaseQuestComepleteCount()		{ m_nCountComplete++; }
	int GetQuestComepleteCount()			{ return m_nCountComplete; }
	void ResetQuestComepleteCount()			{ m_nCountComplete = 0; }

	CQuest* AddQuest(int nQuestIndex);
	bool DelQuest(CPC* pPC, CQuest* pQuest);
	CQuest* FindQuest(int nQuestIndex);
	bool	IsQuestType0( int qType0 );		//  [2/26/2007 KwonYongDae] 현재 퀘스트 타입이 진행 중인지 확인

	void SetQuestState(CQuest* pQuest, char cQuestState);

	// pCurrentQuest 다음의 cQuestState 상태의 퀘스트를 반환, pCurrentQuest가 NULL이면 처음부터 검색
	CQuest* GetNextQuest(CQuest* pCurrentQuest, char cQuestState = QUEST_STATE_ALL);

	void CheckComplete(CPC* pc);				// 퀘스트 중 Complete 되었는데 Msg 안보냈다면 보낸다 체크!
};

#else // QUEST_DATA_EXTEND

class CQuest
{
public:
	CQuestProto*	m_proto;						// 퀘스트 프로토

	// 수행 조건
	int				m_currentData[QUEST_MAX_CONDITION];	// 현재 상태

	CQuest(CQuestProto* proto);

	void QuestUpdateData(CPC* pc, CNPC* npc);		// 진행중인 퀘스트 데이타 갱신
	bool IsPetQuest() 
	{ if( m_proto->m_index ==  PET_HORSE_QUEST_INDEX || m_proto->m_index ==  PET_DRAGON_QUEST_INDEX || m_proto->m_index ==  PET_BLUE_HORSE_QUEST_INDEX || m_proto->m_index ==  PET_PINK_DRAGON_QUEST_INDEX || m_proto->m_index ==  PET_UNKOWN_HORSE_QUEST_INDEX || m_proto->m_index ==  PET_UNKOWN_DRAGON_QUEST_INDEX ) return true; return false; }
};

/////////////////////
// 캐릭터 Quest List
class CQuestList
{

public:
	CQuest*			m_list[QUEST_MAX_PC];				// 퀘스트 리스트
	bool			m_bQuest[QUEST_MAX_PC];				// 퀘스트 수행중인가 (보상받아야 수행끝)
	int				m_nCount;							// 수행중인 퀘스트 수

	int				m_nComplete;						// 수행완료해서 보상 받은 퀘스트 수 : DB저장 없음 (for Single Dungeon)

	bool			m_bComplete[QUEST_MAX_PC];			// 퀘스트 완료 했는가 (보상전)
	bool			m_bComplete2[QUEST_MAX_PC];			// 퀘스트 완료 했는가 (보상전)

	int				m_doneQuest[QUEST_MAX_PC_COMPLETE];	// 완료한 1회용 퀘스트 번호 모음

	int				m_abandonQuest[QUEST_MAX_PC_COMPLETE];  // 포기한 퀘스트 번호 모음
	int				m_doneCnt;								//포기 퀘스트 갯수
	int				m_abandonCnt;							//완료 퀘스트 갯수


	CQuestList();
	~CQuestList();

	int		AddQuest(CQuest* quest);			// Quest List에 새로운 quest insert
	bool	DelQuest(CPC* pc, CQuest* quest);	// Quest List에 새로운 quest delete
	int		FindQuest(int index);				// 퀘스트 리스트중 해당 퀘스트 찾아 리스트 인덱스 반환

	char*	GetIndexString(char* buf);			// 수행중인 퀘스트 DB 저장 Index String 만들기
	char*	GetValueString(char* buf);			// 수행중인 퀘스트 DB 저장 Value String 만들기
	char*	GetCompleteIndexString(char* buf);	// 완료한 퀘스트 DB 저장 Index String 만들기

	void	CheckComplete(CPC* pc);				// 퀘스트 중 Complete 되었는데 Msg 안보냈다면 보낸다 체크!
	
	char*	GetAbandonIndexString(char* buf); //포기한 퀘스트 DB저장할 index string 만들기.

	bool SetDoneQuest(int val)
	{
		int i;
		for (i = 0; i < QUEST_MAX_PC_COMPLETE; i++)
		{
			if (m_doneQuest[i] == -1)
			{
				m_doneQuest[i] = val;
				return true;
			}
		}
		return false;
	}

	int GetDoneQuest(int idx)
	{
		if (idx >= 0 && idx < QUEST_MAX_PC_COMPLETE)
			return m_doneQuest[idx];
		else
			return -1;
	}

	bool IsCompleted(int idx)
	{
		if (idx >= 0 && idx < QUEST_MAX_PC)
			return (m_bComplete[idx] && m_bComplete2[idx]);//0608
		else
			return false;
	}

	bool SetComplete(int idx, bool val)
	{
		if (idx >= 0 && idx < QUEST_MAX_PC)
		{
			m_bComplete[idx] = val;
			return true;
		}
		else
			return false;
	}

	bool SetComplete2(int idx, bool val)//일반 아이템 condition체크.
	{
		if (idx >= 0 && idx < QUEST_MAX_PC)
		{
			m_bComplete2[idx] = val;
			return true;
		}
		else
			return false;
	}
};
#endif // QUEST_DATA_EXTEND

#endif // !defined(AFX_QUEST_H__0F547455_B4DB_4214_8DA3_2F5F2BE49117__INCLUDED_)
