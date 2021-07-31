#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"
#include "Friend.h"
#include "CmdMsg.h"
#include "doFunc.h"


CFriendMember::CFriendMember()
: m_strFriendName(MAX_CHAR_NAME_LENGTH + 1)
{
	m_nChaIndex = -1;
//	m_nLastConnectDate = 0; //최근접속일자
	m_nJobType = -1; //종족
	m_nCondition = 	MSG_FRIEND_CONDITION_OFFLINE; //상태
#ifdef MESSENGER_NEW
	m_groupIndex = 0;
#endif
#ifdef __GAME_SERVER__
	m_pPc = NULL;
#endif
}


CFriendMember::~CFriendMember()
{
	/*
#ifdef __GAME_SERVER__
	if (m_pPc)
		m_pPc->m_Friend = NULL;
	m_pPc = NULL;
#endif
	*/
}
/*
int CFriendMember::GetChaIndex()
{
	return m_nChaIndex;
}
*/
void CFriendMember::SetChaIndex(int chaindex)
{
	m_nChaIndex = chaindex;
}

void CFriendMember::SetName(const char* name)
{
	if (name) 
	{
		m_strFriendName = name;
	}
	else 
	{
		m_strFriendName = "";
	}
}

/*
void CFriendMember::SetFriendListIndex(int listindex)
{
	m_nListIndex = listindex;
}
*/

void CFriendMember::SetCondition(int condition)
{
	m_nCondition = condition;
}

void CFriendMember::SetJob(int job)
{
	m_nJobType = job;
}

CFriend::CFriend()
#ifdef MESSENGER_NEW
:m_groupList(CFriendGroup::CompByIndex)
#endif
{
	memset(m_pFriendMember, 0, sizeof(CFriendMember*) * FRIEND_MAX_MEMBER);
//	memset(m_aFriendMeChaIndex, -1, sizeof(int) * FRIEND_MAX_MEMBER);
	m_nFriendCnt = 0;
}

CFriend::~CFriend()
{
	for (int i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i])
			delete m_pFriendMember[i];
	}
	memset(m_pFriendMember, 0, sizeof(CFriendMember*) * FRIEND_MAX_MEMBER);
	m_nFriendCnt=0;
}


CFriendMember* CFriend::FindFriendMember(int chaindex)
{
	
	for (int i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i])
		{
			if (m_pFriendMember[i]->GetChaIndex() == chaindex)
				return m_pFriendMember[i];
		}
	}
	return NULL;
}

CFriendMember* CFriend::FindFriendMember(const char* name)
{
	for (int i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i])
		{
			if ( strcmp(m_pFriendMember[i]->GetName(), name) == 0 )
				return m_pFriendMember[i];
		}
	}
	return NULL;
}

//친구의 상태 변환
void CFriend::ChangeCondition(int chaindex, int condition)
{
	if (chaindex < 0 || chaindex >= FRIEND_MAX_MEMBER)
		return ;
	
	CFriendMember* pFriendMember;	
	pFriendMember = FindFriendMember(chaindex);
	
	if(pFriendMember == NULL)
	{
		return;
	}
	
	for (int i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i] == pFriendMember)
		{		
			m_pFriendMember[i]->m_nCondition = condition;
			//오프라인이라면 마지막 접속종료일자 저장 
//			if(condition == MSG_FRIEND_CONDITION_OFFLINE)
//			{
//				time_t t;
//				m_pFriendMember[i]->m_nLastConnectDate = time(&t) / 60 / 60 / 24;
//			}
		}
	}
}

//친구등록
int CFriend::AddFriend(int chaindex, const char* name, int job, int nCon, int groupIndex)
{
	if (m_nFriendCnt >= FRIEND_MAX_MEMBER)
	{
		return -1;
	}
		
	
	CFriendMember* pFriendMember;	
	for (int i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i] == NULL)
		{
			pFriendMember = new CFriendMember();
			pFriendMember->SetChaIndex(chaindex);
			pFriendMember->SetName(name);
			pFriendMember->SetCondition(nCon);
			pFriendMember->SetJob(job);
#ifdef MESSENGER_NEW
			CFriendGroup* fgroup = new CFriendGroup(groupIndex, "");
			void * pos = this->m_groupList.FindData(fgroup);
			delete fgroup;
			fgroup = NULL;

			if (pos)
			{
				pFriendMember->SetGroup(groupIndex);
			}
			else
			{
				pFriendMember->SetGroup(0);
			}
#endif

			m_pFriendMember[i] = pFriendMember;
			m_nFriendCnt++;
			return i;	//리스트의 인덱스가 되겠지..
		}
	}

	return -1;
}

//친구삭제
void CFriend::RemoveFriend(int chaindex)
{
	
	for (int i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i])
		{
			if (m_pFriendMember[i]->GetChaIndex() == chaindex)
			{
				m_pFriendMember[i] = NULL;
				m_nFriendCnt--;
				return;
			}
		}
	}

}

/*
void CFriend::SetFriendList(int index, int friendindex, const char* name, int job, int condition)
{
	if(m_pFriendMember)
	{
		SetFriendCount(index+1);
//		m_pFriendMember[index]->SetFriendListIndex(index);
		m_pFriendMember[index]->SetChaIndex(friendindex);
		m_pFriendMember[index]->SetName(name);
		m_pFriendMember[index]->SetJob(job);
		m_pFriendMember[index]->SetCondition(condition);
	}
}
*/

//#ifdef __GAME_SERVER__
void CFriend::SendToAll(CNetMsg& msg)
{
	int i;
	for (i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i])
		{
			if (m_pFriendMember[i]->GetPC())
			{
				SEND_Q(msg, m_pFriendMember[i]->GetPC()->m_desc);
			}
		}
	}
}
//#endif

/*
void CFriend::SetMyCondition(int index, int condition)
{		
	CNetMsg msg;
	
	msg.Init(MSG_FRIEND);
	msg << MSG_FRIEND_SET_CONDITION
		<< index
		<< condition;

//#ifdef __GAME_SERVER__
	SendToAll(msg);
//#endif
}
*/

#ifdef MESSENGER_NEW
int CFriend::addGroup(const char* gName, int gIndex)
{

	int count = this->m_groupList.GetCount();

	// 10개 그룹이 넘으면
	
	if( count >= FRIEND_GROUP_MAX_NUMBER || strinc(gName, "'") || strinc(gName, "%") || strinc(gName, " ") )
	{
		return -1;
	}

	void* pos;
	pos = m_groupList.GetHead();
	CFriendGroup* fgroup = NULL;

	int index = -1;

	for(int i = 0; i < count; i++)
	{
		fgroup = m_groupList.GetData(pos);
		if( strcmp( (const char*) fgroup->GetGroupName(), gName) == 0 )
			return -1;

		if( i != fgroup->GetGroupIndex() )
			index = fgroup->GetGroupIndex();
		pos = m_groupList.GetNext(pos);
	}

	if ( gIndex != -1 )
		fgroup = new CFriendGroup(gIndex, gName);
	else
	{
		if( index == -1 )
			index = fgroup->GetGroupIndex() + 1;

		fgroup = new CFriendGroup(index, gName);
	}

	m_groupList.AddToTail(fgroup);

	return index;
}

//void	delGroup(int gIndex);
//int		changeGroupName(int gIndex, const char* newName);
//int		moveGroupFriend(int gIndex, const char* charName);

int CFriend::delGroup(int gIndex)
{
	CFriendGroup* fgroup = new CFriendGroup(gIndex, "" );

	void* pos = m_groupList.FindData(fgroup);

	delete fgroup;

	if( pos )
	{	
		m_groupList.Remove(pos);

		return gIndex;
	}

	return -1;
}

int CFriend::changeGroupName(int gIndex, const char* newName)
{
	if( strinc(newName, "'") || strinc(newName, "%") || strinc(newName, " ") )
	{
		return -1;
	}

	CFriendGroup* fgroup = new CFriendGroup(gIndex, "");

	void* pos = m_groupList.FindData(fgroup);

	delete fgroup;

	if( pos )
	{
		fgroup = m_groupList.GetData(pos);
		fgroup->SetName(newName);
		return gIndex;
	}

	return -1;
}

int CFriend::moveGroupFriend(int gIndex, int charIndex)
{
	CFriendMember* fmember = this->FindFriendMember(charIndex);

	if( fmember )
	{
		fmember->SetGroup(gIndex);
		return gIndex;
	}

	return -1;
}

void CFriend::GetGroupIndexString(CLCString& gIndexList, CLCString& gNameList)
{
	void* pos = this->m_groupList.GetHead();
	int count = this->m_groupList.GetCount();

	CFriendGroup* fgroup = NULL;
	for(int i = 0; i < count; i++)
	{
		fgroup = this->m_groupList.GetData(pos);

		if( fgroup )
		{
			sprintf(g_buf, "%d", fgroup->GetGroupIndex() );
			gIndexList += g_buf;
			gIndexList += " ";
			gNameList += fgroup->GetGroupName();
			gNameList += " ";
		}

		pos = this->m_groupList.GetNext(pos);
	}
}

CFriendGroup::~CFriendGroup()
{
}
#endif