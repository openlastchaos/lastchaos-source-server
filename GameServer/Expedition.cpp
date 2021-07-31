#include "stdhdrs.h"
#include "Server.h"
#include "Expedition.h"

#ifdef EXPEDITION_RAID			//원정대 시스템

#ifdef __GAME_SERVER__
#include "Battle.h"
#endif // __GAME_SERVER__

////////////////////////////
// implement of CExpeditionMember
#ifdef __GAME_SERVER__
CExpedMember::CExpedMember(int nCharIndex, const char* strCharName, int nGroupType, int nMemberType,int nListIndex, CPC* pc)
: m_strCharName(MAX_CHAR_NAME_LENGTH + 1)
#else
CExpedMember::CExpedMember(int nCharIndex, const char* strCharName, int nGroupType, int nMemberType,int nListIndex)
: m_strCharName(MAX_CHAR_NAME_LENGTH + 1)
#endif // __GAME_SERVER__
{
	m_nCharIndex	= nCharIndex;
	m_strCharName	= strCharName;
	m_nGroupType	= nGroupType;
	m_nMemberType	= nMemberType; 
	m_nListIndex	= nListIndex;	
	m_nQuitType		= MSG_EXPED_QUITMODE_NORMAL;

#ifdef __GAME_SERVER__
	m_pChar = pc;
#endif // __GAME_SERVER__
}

CExpedMember::~CExpedMember()
{
	m_nCharIndex	= 0;
	m_strCharName	= "";
	m_nGroupType	= -1;
	m_nMemberType	= -1;
	m_nLabelType	= -1;

#ifdef __GAME_SERVER__
	m_pChar = NULL;
#endif // __GAME_SERVER__
}

#ifdef __GAME_SERVER__
void CExpedMember::SetMemberPCPtr(CPC* pc)
{
	if (m_pChar != NULL && pc == NULL)
		m_pChar->m_Exped = NULL;						//CPC 클래스에 원정대 추가 필요
	m_pChar = pc;
}
#endif // __GAME_SERVER__

void CExpedMember::SetCharIndex(int nCharIndex)
{
	m_nCharIndex	= nCharIndex;
}

void CExpedMember::SetCharName(const char* strCharName)
{
	m_strCharName	= strCharName;
}

void CExpedMember::SetGroupType(int nGroupType)
{
	m_nGroupType	= nGroupType;
}

void CExpedMember::SetMemberType(int nMemberType)
{
	m_nMemberType	= nMemberType;
}

void CExpedMember::SetLabelType(int nLabelType)
{
	m_nLabelType	= nLabelType;
}

void CExpedMember::SetQuitType(int nQuitType)
{
	m_nQuitType	= nQuitType;
}

//////////////////////
// implement of CParty
#ifdef __GAME_SERVER__
CExpedition::CExpedition(char nExpedType, int nBossIndex, const char* strBossName, int nGroupType, int nMemberType, CPC* pBoss)
: m_strRequestName(MAX_CHAR_NAME_LENGTH + 1)
#else
CExpedition::CExpedition(int nSubNo, char nExpedType, int nBossIndex, const char* strBossName, int nGroupType, int nMemberType)
: m_strRequestName(MAX_CHAR_NAME_LENGTH + 1)
#endif // __GAME_SERVER__
{
	m_nExpedType[MSG_DIVITYPE_EXP]	 = nExpedType;
	m_nExpedType[MSG_DIVITYPE_ITEM]	 = nExpedType;
	m_nExpedType[MSG_DIVITYPE_SPECIAL] = nExpedType;

	memset(m_listMember, 0, sizeof(CExpedMember*) * MAX_EXPED_MEMBER);
	
	for(int i=0; i < MAX_EXPED_GROUP; i++)
	{
		m_nGTotCount[i]	= 0;
	}

#ifdef __GAME_SERVER__
	m_listMember[0][0] = new CExpedMember(nBossIndex, strBossName, nGroupType, nMemberType, 0,pBoss);
#else
	m_listMember[0][0] = new CExpedMember(nBossIndex, strBossName, nGroupType, nMemberType, 0);
	m_nSubNo = nSubNo;
#endif // __GAME_SERVER__

	m_nRequestIndex = -1;
	m_strRequestName = "";

	m_nTotCount = 1;		// 전체 인원수 
	m_nGTotCount[0] = 1;	// 그룹1 인원수
}

CExpedition::~CExpedition()
{
	m_nExpedType[MSG_DIVITYPE_EXP]	 = -1;
	m_nExpedType[MSG_DIVITYPE_ITEM]	 = -1;
	m_nExpedType[MSG_DIVITYPE_SPECIAL] = -1;

	m_nTotCount = 0;

	for(int k=0; k < MAX_EXPED_GROUP; k++)
	{
		m_nGTotCount[k]	= 0;
	}

	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j] != NULL)
			{
#ifdef __GAME_SERVER__
				if (m_listMember[i][j]->GetMemberPCPtr())
				{
					m_listMember[i][j]->SetMemberPCPtr(NULL);
				}
#endif // __GAME_SERVER__
				delete m_listMember[i][j];
				m_listMember[i][j] = NULL;
			}
		}
	}
	m_nRequestIndex = -1;
	m_strRequestName = "";
}

//////////////////////////////////////////////////////////////////////
// 게임 서버 전용 함수들
#ifdef __GAME_SERVER__
int CExpedition::GetNearExpeditionMemberCount(CCharacter* pCenter)
{
	int i;
	int ret = 0;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if ( m_listMember[i][j] != NULL
				 && m_listMember[i][j]->GetMemberPCPtr()
				 && CheckInNearCellExt(pCenter, m_listMember[i][j]->GetMemberPCPtr())
				)
			{
				ret++;
			}
		}
	}

	return ret;
}

void CExpedition::SendToAllPC(CNetMsg& msg, int nExcludeCharIndex)
{
	int i;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j] != NULL
				&& m_listMember[i][j]->GetMemberPCPtr()
				&& m_listMember[i][j]->GetCharIndex() != nExcludeCharIndex
				)
			{
				SEND_Q(msg, m_listMember[i][j]->GetMemberPCPtr()->m_desc);
			}
		}
	}
}

void CExpedition::SendToPCInSameZone(int nZone, int nArea, CNetMsg& msg)
{
	int i;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j] != NULL
				&& m_listMember[i][j]->GetMemberPCPtr()
				&& m_listMember[i][j]->GetMemberPCPtr()->m_pZone->m_index == nZone
				&& m_listMember[i][j]->GetMemberPCPtr()->m_pArea->m_index == nArea
				)
			{
				SEND_Q(msg, m_listMember[i][j]->GetMemberPCPtr()->m_desc);
			}
		}
	}
}

void CExpedition::SendToPCInSameGroup(int nGroup, CNetMsg& msg)
{
	if(nGroup < 0 ||  nGroup >= MAX_EXPED_GROUP) return;

	for(int j=0; j < MAX_EXPED_GMEMBER; j++)
	{
		if (m_listMember[nGroup][j] != NULL)
		{
			SEND_Q(msg, m_listMember[nGroup][j]->GetMemberPCPtr()->m_desc);
		}
	}

}


CPC* CExpedition::GetNearMember(CCharacter* pPC, int nGroup,int nListIndex)
{
	if (nListIndex < 0 || nListIndex >= MAX_EXPED_GMEMBER)
		return NULL;

	if (m_listMember[nGroup][nListIndex] == NULL)
		return NULL;

	if (m_listMember[nGroup][nListIndex]->GetMemberPCPtr() == NULL)
		return NULL;

	if (!CheckInNearCellExt(pPC, m_listMember[nGroup][nListIndex]->GetMemberPCPtr()))
		return NULL;

	return m_listMember[nGroup][nListIndex]->GetMemberPCPtr();
}

void CExpedition::SetMemberPCPtr(int nCharIndex, CPC* pPC)
{
	int i;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j] != NULL
				&& m_listMember[i][j]->GetCharIndex() == nCharIndex
				)
			{
				m_listMember[i][j]->SetMemberPCPtr(pPC);
				return ;
			}
		}
	}
	return ;
}

#endif // __GAME_SERVER__
// 게임 서버 전용 함수들
//////////////////////////////////////////////////////////////////////

const CExpedMember* CExpedition::GetMemberByCharIndex(int nCharIndex) const
{
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j])
			{
				if (m_listMember[i][j]->GetCharIndex() == nCharIndex)
				{
					return m_listMember[i][j];
				}
			}
		}
	}

	return NULL;
}

const CExpedMember* CExpedition::GetMemberByListIndex(int nGroup, int nListIndex) const
{
	if (nListIndex < 0 || nListIndex >= MAX_EXPED_GMEMBER)
		return NULL;

	if (m_listMember[nGroup][nListIndex] == NULL)
		return NULL;

	return m_listMember[nGroup][nListIndex];
}

const CExpedMember* CExpedition::GetMemberByListIndex(int nGroup, int nListIndex, int nMemberType) const
{
	if (nListIndex < 0 || nListIndex >= MAX_EXPED_GMEMBER)
		return NULL;

	if (m_listMember[nGroup][nListIndex] == NULL)
		return NULL;

	if(m_listMember[nGroup][nListIndex]->GetMemberType() == nMemberType)
		return m_listMember[nGroup][nListIndex];

	return NULL;
}


int CExpedition::FindMemberListIndex(int nCharIndex)
{
	int i;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j] != NULL 	&& m_listMember[i][j]->GetCharIndex() == nCharIndex)
			{
				return j;
			}
		}
	}
	return -1;
}

int CExpedition::FindMemberGroupIndex(int nCharIndex)
{
	int i;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j] != NULL && m_listMember[i][j]->GetCharIndex() == nCharIndex)
			{
				return i;
			}
		}
	}
	return -1;
}

int CExpedition::GetMemberCharIndex(int nGroup, int nListIndex)
{
	if (nListIndex < 0 || nListIndex >= MAX_EXPED_GMEMBER)
		return -1;

	if (m_listMember[nGroup][nListIndex] == NULL)
		return -1;

	return m_listMember[nGroup][nListIndex]->GetCharIndex();
}

int CExpedition::CompExpedition(CExpedition* p1, CExpedition* p2)
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

void CExpedition::SetRequest(int nRequest, const char* strRequest)
{
#ifdef __GAME_SERVER__
	// 게임 서버에서는 요청자를 찾아 파티 정보를 리셋한다.
	if (m_nRequestIndex != -1)
	{
		CPC* pPC = gserver.m_playerList.Find(m_nRequestIndex);
		if (pPC)
			pPC->m_Exped = NULL;
	}
#endif // __GAME_SERVER__
	m_nRequestIndex = nRequest;
	m_strRequestName = strRequest;
}

#ifdef __GAME_SERVER__
const CExpedMember* CExpedition::JoinRequest(const char* strRequestName, int nMemberType, CPC* pRequest)
#else
const CExpedMember* CExpedition::JoinRequest(const char* strRequestName, int nMemberType)
#endif // __GAME_SERVER__
{
	if (GetRequestIndex() < 1)
		return NULL;

	// 빈자리를 찾고
	int i,j;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j] == NULL)
				break;
		}
	}

	if (i == MAX_EXPED_GROUP || j == MAX_EXPED_GMEMBER)
	{
		m_nRequestIndex = -1;
		return NULL;
	}

	// 빈자리에 설정하고
#ifdef __GAME_SERVER__
	m_listMember[i][j] = new CExpedMember(m_nRequestIndex, strRequestName, i, nMemberType, j, pRequest);
#else
	m_listMember[i][j] = new CExpedMember(m_nRequestIndex, strRequestName, i, nMemberType, j);
#endif // __GAME_SERVER__

	m_nTotCount++;
	m_nGTotCount[i]++;

	// request 초기화 하고
	m_nRequestIndex = -1;

	return m_listMember[i][j];
}

// 멤버 참여
#ifdef __GAME_SERVER__
const CExpedMember* CExpedition::Join(int nCharIndex, const char* strCharName, int nMemberType, CPC* pRequest)
#else
const CExpedMember* CExpedition::Join(int nCharIndex, const char* strCharName, int nMemberType)
#endif // __GAME_SERVER__
{
	// 빈자리를 찾고
	int i,j;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j] == NULL)
				break;
		}
	}
	
	if (i == MAX_EXPED_GROUP || j == MAX_EXPED_GMEMBER)
	{
		return NULL;
	}

	// 빈자리에 설정하고
#ifdef __GAME_SERVER__
	m_listMember[i][j] = new CExpedMember(nCharIndex, strCharName, i, nMemberType, j, pRequest);
#else
	m_listMember[i][j] = new CExpedMember(nCharIndex, strCharName, i, nMemberType, j);
#endif // __GAME_SERVER__

	m_nTotCount++;
	m_nGTotCount[i]++;

	return m_listMember[i][j];
}


void CExpedition::DeleteMember(int nCharIndex)
{
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j])
			{
				if (m_listMember[i][j]->GetCharIndex() == nCharIndex)
				{
					delete m_listMember[i][j];
					m_listMember[i][j] = NULL;

					m_nTotCount--;
					m_nGTotCount[i]--;
				}
			}
		}
	}
	return ;
}

bool CExpedition::ChangeBoss(int nTargetIndex, int nOldBossIndex,bool bOldReset)
{
	if(bOldReset)
	{
		CExpedMember* pMemberOld = (CExpedMember*)GetMemberByCharIndex(nOldBossIndex);
		if(pMemberOld)
		{
			pMemberOld->SetMemberType(MSG_EXPED_MEMBERTYPE_NORMAL);
		}
	}

	CExpedMember* pMemberTarget = (CExpedMember*)GetMemberByCharIndex(nTargetIndex);
	if(pMemberTarget)
	{
		pMemberTarget->SetMemberType(MSG_EXPED_MEMBERTYPE_BOSS);

		return true;
	}

	return false;
}

int CExpedition::GetGroupMemberCount(int nGroupType)
{
	int nMemberCnt=0;

	for(int i=0; i < MAX_EXPED_GMEMBER; i++)
	{
		if (m_listMember[nGroupType][i])
		{
			nMemberCnt++;
		}
	}
	
	return nMemberCnt;
}

int CExpedition::GetGroupBeEmptySlot(void)
{
	int nCutMemebrCnt=0;
	for(int iGroup=0;  iGroup < MAX_EXPED_GROUP; iGroup++)
	{
		nCutMemebrCnt = GetGroupMemberCount(iGroup);
		if( nCutMemebrCnt >= 0 && nCutMemebrCnt < MAX_EXPED_GMEMBER)
			return iGroup;
	}

	return -1;
}

// 그룹 멤버 타임 수 획득
int CExpedition::GetGroupMembertypeCount(int nGroupType, int nMemberType)
{
	int nMemberCount = 0;

	for(int i=0; i < MAX_EXPED_GMEMBER; i++)
	{
		if (m_listMember[nGroupType][i])
		{
			CExpedMember* pMember = m_listMember[nGroupType][i];
			
			if(pMember->GetMemberType() == nMemberType)
				nMemberCount++;
		}
	}

	return	nMemberCount;
}

bool CExpedition::SetMBoss(int nNewMBossIndex)
{
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j] != NULL && m_listMember[i][j]->GetCharIndex() == nNewMBossIndex)
			{
				CExpedMember* pMember = m_listMember[i][j];
				pMember->SetMemberType(MSG_EXPED_MEMBERTYPE_MBOSS);
				return true;
			}
		}
	}
	return false;	
}

bool CExpedition::ResetMBoss(int nNewMBossIndex)
{
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j] != NULL && m_listMember[i][j]->GetCharIndex() == nNewMBossIndex)
			{
				CExpedMember* pMember = m_listMember[i][j];
				pMember->SetMemberType(MSG_EXPED_MEMBERTYPE_NORMAL);
				return true;
			}
		}
	}
	return false;
}

bool CExpedition::MoveGroup(int nSourceGroup, int nMoveCharIndex, int nTargetGroup, int nTargetListindex)
{
	if(m_listMember[nTargetGroup][nTargetListindex] != NULL)
		return false;

	if(GetGroupMemberCount(nTargetGroup) >= MAX_EXPED_GMEMBER)
		return false;

	int nSourceListindex = FindMemberListIndex(nMoveCharIndex);

	if(nSourceListindex >= 0)
	{
		// 이동 
		m_listMember[nTargetGroup][nTargetListindex] = m_listMember[nSourceGroup][nSourceListindex];
		m_nGTotCount[nTargetGroup]++;

		// 소스 초기화
		m_listMember[nSourceGroup][nSourceListindex] = NULL;
		m_nGTotCount[nSourceGroup]--;

		return true;		
	}

	return false;
}

//위임할 보스 찾기
CExpedMember* CExpedition::FindNextBoss(void)
{
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j])
			{
				CExpedMember* pMember = m_listMember[i][j];
				if( pMember != NULL && 
					pMember->GetQuitType() != MSG_EXPED_QUITMODE_UNUSUAL &&
				   (pMember->GetMemberType() == MSG_EXPED_MEMBERTYPE_MBOSS || 
				    pMember->GetMemberType() == MSG_EXPED_MEMBERTYPE_NORMAL)
				  )	
				{
					return m_listMember[i][j];
				}
			}
		}
	}
	return NULL;
}

char CExpedition::GetExpeditionType(int nType)
{ 
	if(nType < 0  && nType >= MAX_EXPED_DIVITYPE) return -1; 
	
	return m_nExpedType[nType]; 
}

	// boss 인덱스 반환
int CExpedition::GetBossIndex()
{ 
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j])
			{
				CExpedMember* pMember = m_listMember[i][j];
				if( pMember != NULL && pMember->GetMemberType() == MSG_EXPED_MEMBERTYPE_BOSS)
				{
					return pMember->GetCharIndex();						
				}
			}
		}
	}

	return  -1; 
}
const char* CExpedition::GetBossName()
{ 
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j])
			{
				CExpedMember* pMember = m_listMember[i][j];
				if( pMember != NULL && pMember->GetMemberType() == MSG_EXPED_MEMBERTYPE_BOSS)
				{
					return pMember->GetCharName();
				}
			}
		}
	}

	return  NULL; 
}

int CExpedition::GetBossGroupType()
{ 
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j])
			{
				CExpedMember* pMember = m_listMember[i][j];
				if( pMember != NULL && pMember->GetMemberType() == MSG_EXPED_MEMBERTYPE_BOSS)
				{
					return i;
				}
			}
		}
	}

	return  -1; 
}

#endif  //EXPEDITION_RAID