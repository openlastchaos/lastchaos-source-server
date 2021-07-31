#include "stdhdrs.h"
#include "Server.h"
#include "Party.h"

#ifdef __GAME_SERVER__
#include "Battle.h"
#endif // __GAME_SERVER__

////////////////////////////
// implement of CPartyMember
#ifdef __GAME_SERVER__
CPartyMember::CPartyMember(int nCharIndex, const char* strCharName, CPC* pc)
: m_strCharName(MAX_CHAR_NAME_LENGTH + 1)
#else
CPartyMember::CPartyMember(int nCharIndex, const char* strCharName)
: m_strCharName(MAX_CHAR_NAME_LENGTH + 1)
#endif // __GAME_SERVER__
{
	m_nCharIndex = nCharIndex;
	m_strCharName = strCharName;

#ifdef __GAME_SERVER__
	m_pChar = pc;
#endif // __GAME_SERVER__
}

CPartyMember::~CPartyMember()
{
	m_nCharIndex = 0;
	m_strCharName = "";

#ifdef __GAME_SERVER__
	m_pChar = NULL;
#endif // __GAME_SERVER__
}

#ifdef __GAME_SERVER__
void CPartyMember::SetMemberPCPtr(CPC* pc)
{
	if (m_pChar != NULL && pc == NULL)
		m_pChar->m_party = NULL;

	m_pChar = pc;
}
#endif // __GAME_SERVER__


//////////////////////
// implement of CParty
#ifdef __GAME_SERVER__
CParty::CParty(char nPartyType, int nBossIndex, const char* strBossName, CPC* pBoss, int nRequest, const char* strRequest)
: m_strRequest(MAX_CHAR_NAME_LENGTH + 1)
#else
CParty::CParty(int nSubNo, char nPartyType, int nBossIndex, const char* strBossName, int nRequest, const char* strRequest)
: m_strRequest(MAX_CHAR_NAME_LENGTH + 1)
#endif // __GAME_SERVER__
{
	m_nPartyType[MSG_DIVITYPE_EXP]		= nPartyType;
	m_nPartyType[MSG_DIVITYPE_ITEM]		= nPartyType;
	m_nPartyType[MSG_DIVITYPE_SPECIAL]	= nPartyType;

	memset(m_listMember, 0, sizeof(CPartyMember*) * MAX_PARTY_MEMBER);

#ifdef __GAME_SERVER__
	m_listMember[0] = new CPartyMember(nBossIndex, strBossName, pBoss);
#else
	m_listMember[0] = new CPartyMember(nBossIndex, strBossName);
	m_nSubNo = nSubNo;
#endif // __GAME_SERVER__

	m_nRequest = nRequest;
	m_strRequest = strRequest;
	m_nCount = 1;

#ifdef GMTOOL
	m_bGmtarget = false;
#endif // GMTOOL

#ifdef __GAME_SERVER__
#ifdef MONSTER_COMBO
	m_comboAreaIndex = -1;
#endif // MONSTER_COMBO

#ifdef EXTREME_CUBE
	m_cubeUniqueIdx = -1;
#endif // EXTREME_CUBE
#endif // __GAME_SERVER__
}

CParty::~CParty()
{
	m_nPartyType[MSG_DIVITYPE_EXP] = -1;
	m_nPartyType[MSG_DIVITYPE_ITEM] = -1;
	m_nPartyType[MSG_DIVITYPE_SPECIAL] = -1;

	m_nCount = 0;
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if (m_listMember[i] != NULL)
		{
#ifdef __GAME_SERVER__
			if (m_listMember[i]->GetMemberPCPtr())
			{
				m_listMember[i]->SetMemberPCPtr(NULL);
			}
#endif // __GAME_SERVER__
			delete m_listMember[i];
			m_listMember[i] = NULL;
		}
	}
	m_nRequest = -1;

#ifdef __GAME_SERVER__
#ifdef MONSTER_COMBO
	m_comboAreaIndex = -1;
#endif // MONSTER_COMBO

#ifdef EXTREME_CUBE
	m_cubeUniqueIdx = -1;
#endif // EXTREME_CUBE

#endif // __GAME_SERVER__
}

//////////////////////////////////////////////////////////////////////
// 게임 서버 전용 함수들
#ifdef __GAME_SERVER__
int CParty::GetNearPartyMemberCount(CCharacter* pCenter)
{
	int i;
	int ret = 0;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if(m_listMember[i] != NULL
				&& m_listMember[i]->GetMemberPCPtr()
				&& CheckInNearCellExt(pCenter, m_listMember[i]->GetMemberPCPtr())
			)
		{
			ret++;
		}
	}

	return ret;
}

//서버 다운 수정(09.05.11)
void CParty::SendToAllPC(CNetMsg& msg, int nExcludeCharIndex)
{
	int i;
	CPartyMember* pMember = NULL;

	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if(m_listMember[i] != NULL)
		{
			pMember =  (CPartyMember*)m_listMember[i];
			if(pMember != NULL)
			{
				if(pMember->GetCharIndex() != nExcludeCharIndex)	
				{
					CPC *pPC = pMember->GetMemberPCPtr();
					if(pPC != NULL)
					{
						SEND_Q(msg, pPC->m_desc);
					}
				}
			}
		}
	}
}

//서버 다운 수정(09.05.11)
void CParty::SendToPCInSameZone(int nZone, int nArea, CNetMsg& msg)
{
	int i;
	CPartyMember* pMember = NULL;

	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if(m_listMember[i] != NULL)
		{
			pMember = (CPartyMember*)m_listMember[i];
			if(pMember != NULL)
			{
				CPC *pPC = pMember->GetMemberPCPtr();
				if(pPC != NULL && pPC->m_pZone->m_index == nZone && pPC->m_pArea->m_index == nArea)
				{
					SEND_Q(msg, pPC->m_desc);
				}
			}
		}
	}
}

CPC* CParty::GetNearMember(CCharacter* pPC, int nListIndex)
{
	if (nListIndex < 0 || nListIndex >= MAX_PARTY_MEMBER)
		return NULL;
	if (m_listMember[nListIndex] == NULL)
		return NULL;
	if (m_listMember[nListIndex]->GetMemberPCPtr() == NULL)
		return NULL;
	if (!CheckInNearCellExt(pPC, m_listMember[nListIndex]->GetMemberPCPtr()))
		return NULL;
	return m_listMember[nListIndex]->GetMemberPCPtr();
}

void CParty::SetMemberPCPtr(int nCharIndex, CPC* pPC)
{
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if(m_listMember[i] != NULL
		   && m_listMember[i]->GetCharIndex() == nCharIndex
		  )
		{
			m_listMember[i]->SetMemberPCPtr(pPC);
			return ;
		}
	}
	return ;
}

#endif // __GAME_SERVER__
// 게임 서버 전용 함수들
//////////////////////////////////////////////////////////////////////

const CPartyMember* CParty::GetMemberByListIndex(int nListIndex) const
{
	if (nListIndex < 0 || nListIndex >= MAX_PARTY_MEMBER)
		return NULL;
	if (m_listMember[nListIndex] == NULL)
		return NULL;
	return m_listMember[nListIndex];
}

int CParty::FindMember(int nCharIndex)
{
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if (	   m_listMember[i] != NULL
				&& m_listMember[i]->GetCharIndex() == nCharIndex
			)
		{
			return i;
		}
	}
	return -1;
}

int CParty::GetMemberCharIndex(int nListIndex)
{
	if (nListIndex < 0 || nListIndex >= MAX_PARTY_MEMBER)
		return -1;
	if (m_listMember[nListIndex] == NULL)
		return -1;
	return m_listMember[nListIndex]->GetCharIndex();
}

int CParty::CompParty(CParty* p1, CParty* p2)
{
#ifdef __GAME_SERVER__
	if (p1->GetBossIndex() == p2->GetBossIndex())
		return 0;
	else
		return p1->GetBossIndex() - p2->GetBossIndex();
#else // __GAME_SERVER__
	if (p1->GetSubNo() == p2->GetSubNo())
	{
		if (p1->GetBossIndex() == p2->GetBossIndex())
			return 0;
		else
			return p1->GetBossIndex() - p2->GetBossIndex();
	}
	else
		return p1->GetSubNo() - p2->GetSubNo();
#endif // __GAME_SERVER__
}

void CParty::SetRequest(int nRequest, const char* strRequest)
{
#ifdef __GAME_SERVER__
	// 게임 서버에서는 요청자를 찾아 파티 정보를 리셋한다.
	if (m_nRequest != -1)
	{
		CPC* pPC = gserver.m_playerList.Find(m_nRequest);
		if (pPC)
			pPC->m_party = NULL;
	}
#endif // __GAME_SERVER__
	m_nRequest = nRequest;
	m_strRequest = strRequest;
}

#ifdef __GAME_SERVER__
int CParty::JoinRequest(const char* strRequestName, CPC* pRequest)
#else
int CParty::JoinRequest(const char* strRequestName)
#endif // __GAME_SERVER__
{
	if (GetRequestIndex() < 1)
		return -1;

	// 빈자리를 찾고
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if (m_listMember[i] == NULL)
			break;
	}
	if (i == MAX_PARTY_MEMBER)
	{
		m_nRequest = -1;
		return -1;
	}

	// 빈자리에 설정하고
#ifdef __GAME_SERVER__
	m_listMember[i] = new CPartyMember(m_nRequest, strRequestName, pRequest);
#else
	m_listMember[i] = new CPartyMember(m_nRequest, strRequestName);
#endif // __GAME_SERVER__
	m_nCount++;

	// request 초기화 하고
	m_nRequest = -1;

	return i;
}

void CParty::DeleteMember(int nCharIndex)
{
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if (m_listMember[i])
		{
			if (m_listMember[i]->GetCharIndex() == nCharIndex)
			{
				delete m_listMember[i];
				m_listMember[i] = NULL;
				m_nCount--;
			}
		}
		if (i < MAX_PARTY_MEMBER - 1 && m_listMember[i] == NULL)
		{
			m_listMember[i] = m_listMember[i + 1];
			m_listMember[i + 1] = NULL;
		}
	}
	return ;
}

bool CParty::ChangeBoss(const char* strNewBossName)
{
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if (m_listMember[i])
		{
			if (strcmp(m_listMember[i]->GetCharName(), strNewBossName) == 0)
			{
				CPartyMember* pMember = m_listMember[0];
				m_listMember[0] = m_listMember[i];
				m_listMember[i] = pMember;
				return true;
			}
		}
	}
	return false;
}
