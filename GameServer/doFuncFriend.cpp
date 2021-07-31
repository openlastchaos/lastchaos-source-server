#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "DBCmd.h"
//#include "Friend.h"

//클라이언트로부터 메시지 받는 부분...
void do_Friend(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_MESSENGER
	unsigned char subtype;
	msg.MoveFirst();
	msg >> subtype;

	switch (subtype)
	{
	case MSG_FRIEND_REGIST_REQUEST://친구 등록 요청
		do_FriendRegistRequest(ch, msg);
		break;

	case MSG_FRIEND_REGIST_ALLOW: // 등록 승인
		do_FriendRegistAllow(ch, msg);
		break;

	case MSG_FRIEND_REGIST_CANCEL: // 등록 거절
		do_FriendRegistRefuse(ch, msg);
		break;

	case MSG_FRIEND_DELETE_MEMBER:
		do_FriendDeleteMember(ch, msg);
		break;

	case MSG_FRIEND_SET_CONDITION: //상태변경
		do_FriendSetCondition(ch, msg);
		break;

	case MSG_FRIEND_CHATTING: //채팅
		do_FriendChatting(ch, msg);
		break;
	//case MSG_FRIEND_BLOCK_MEMBER:
	//	do_FriendBlockMember(ch, msg);
		break;
	default:
		break;
	}
#endif
}

//클라어언트로부터 친구등록 요청 메시지가 왔을때...
void do_FriendRegistRequest(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_MESSENGER
	CNetMsg rmsg;

	int requesterindex; //친구 등록 요청자.
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
//	int targetindex; //친구 등록 당하는 자.	

	msg >> requesterindex
		>> name;

	if( strcmp(ch->m_name, name) == 0 || strcmp(ch->m_nick, name) == 0)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 이름 검사
	int lenname = strlen(name);
	if (lenname < 2 || lenname > 16)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (strchr(name, '\'') || strchr(name, ' ') /* || strchr(name, '\\') || strchr(name, '\"') || strchr(name, '%') || strchr(name, '_')*/)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CPC* pPc;
	pPc= gserver.m_playerList.Find(name, true );

//해당 캐릭이 존재하지 않는다면...
	if(!pPc)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_NOT_EXIST);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	else
	{
		if( pPc->IsSetPlayerState(PLAYER_STATE_PKMODE) )
		{
			FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_REGIST_REFUSE_PVP);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

#ifdef MESSENGER_NEW
	if( pPc->IsBlockPC(ch->m_index, ch->GetName() ) )
		return;
#endif // MESSENGER_NEW

//정원 초과했는지 체크...
	int friendcnt;
	friendcnt = pPc->m_Friend->GetFriendSum();

	if(friendcnt >= FRIEND_MAX_MEMBER)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FULLMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	friendcnt = ch->m_Friend->GetFriendSum();

	if(friendcnt >= FRIEND_MAX_MEMBER)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FULLMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
//요청자가 이미 등록되어 있는지 체크..
	CFriendMember* pFriendMem = pPc->m_Friend->FindFriendMember(requesterindex);
	if(pFriendMem != NULL)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_ALREADY_EXIST);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
//이미 다른 사람의 친구요청의 답변을 기다라는 상태라면,,
	if (pPc->m_nRegFriend != 0 && pPc->m_nRegFriend  != ch->m_index)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_WAIT_OTHER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	pPc->m_nRegFriend = ch->m_index;
	ch->m_nRegFriend = pPc->m_index;

//	SEND_Q(msg, ch->m_desc); //자기자신에게 다시 메시지를 보낼 필요가 있을까...
	// 자기 자신에게 메시지를 보낸다.
#ifdef LC_KOR
	rmsg.Init(MSG_FRIEND);
	rmsg << (unsigned char)MSG_FRIEND_REGIST_REQUEST
	 	 << requesterindex
		 << (const char*)pPc->GetName();
	SEND_Q(rmsg, ch->m_desc); //등록 당하는자에게 보내기.
#endif

	//메시지 그대로 보낸다.
	CNetMsg tmsg;
	tmsg.Init(MSG_FRIEND);
	tmsg << (unsigned char)MSG_FRIEND_REGIST_REQUEST
	 	 << requesterindex
		 << (const char*)ch->GetName();
	SEND_Q(tmsg, pPc->m_desc); //등록 당하는자에게 보내기.
#endif
}

//친구등록을 허락했다...
void do_FriendRegistAllow(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_MESSENGER
	CNetMsg rmsg;

	int approvalindex;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
//	int requesterindex;

	msg >> approvalindex
		>> name;

	CPC* pPc;
	pPc= gserver.m_playerList.Find(name, true );
//요청자가 접속을 안하고 있네..
	//CPC* pPc = gserver.m_playerList.Find(requesterindex);
	if (!pPc)
	{
		FriendRegistCancelMsg(rmsg);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_nRegFriend = 0;
		return ;
	}
//서로의 인덱스가 맞지 않는다면..
	if (pPc->m_nRegFriend != ch->m_index || ch->m_nRegFriend != pPc->m_index)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_NOT_MATCH_INDEX);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pPc->m_desc);
		ch->m_nRegFriend = 0;
		pPc->m_nRegFriend = 0;
		return ;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperFriendAddReqMsg(rmsg, ch->m_index, ch->GetName(), ch->m_job, pPc->m_index, pPc->GetName(), pPc->m_job);
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true, 6);
		pPc->m_desc->WaitHelperReply(true, 7);
	}
	else
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_HELPER_SERVER);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pPc->m_desc);
		ch->m_nRegFriend = 0;
		pPc->m_nRegFriend = 0;
		return ;
	}
#endif
}
//친구 등록을 거절했다...
void do_FriendRegistRefuse(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_MESSENGER
	CNetMsg rmsg;

	int approvalindex;	
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);

	msg >> approvalindex
		>> name;

	CPC* pPc;
	pPc= gserver.m_playerList.Find(name, true );

	if(pPc)
	{
		FriendRegistCancelMsg(rmsg);
		SEND_Q(rmsg, pPc->m_desc);

		pPc->m_nRegFriend = 0;
		pPc->m_desc->WaitHelperReply(false);
	}

	ch->m_nRegFriend = 0;
	ch->m_desc->WaitHelperReply(false);
#endif	
}

void do_FriendDeleteMember(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_MESSENGER
	CNetMsg rmsg;

	int requesterindex; //친구 삭제 요청자.
	int targetindex; //친구 삭제 당하는 자.
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);

	msg >> requesterindex
		>> targetindex
		>> name;

//자신을 지우려고 했거나, 요청자가 자신이 아니라면...
	if (targetindex == ch->m_index || requesterindex != ch->m_index)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (targetindex == -1)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int lenname = strlen(name);
	if (lenname < 2 || lenname > 16)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

#if !defined (LC_HBK) && !defined (LC_JPN)   // 홍콩,일본은 이름 상관 없이 삭제
	if (strchr(name, '\'') || strchr(name, '\\') || strchr(name, '\"') || strchr(name, '%') /*|| strchr(name, '_')*/)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif // LC_HBK
/*
	CPC* pPc;
	pPc= gserver.m_playerList.Find(targetindex);
//해당 캐릭이 존재하지 않는다면...
	if(!pPc)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_NOT_EXIST);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

//이미 삭제하려는 친구가 자신의 리스트를 가지고 있지 않다면,
	CFriendMember* pFriendMem = pPc->m_Friend->FindFriendMember(requesterindex);
	if(pFriendMem == NULL)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_NOT_EXIST);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
*/
	if (IS_RUNNING_HELPER)
	{
		HelperFriendDelReqMsg(rmsg, ch->m_index, targetindex, name);
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true, 8);
	}
#endif
}

/*
void do_FriendBlockMember(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_MESSENGER
	CNetMsg rmsg;

	int requesterindex; //친구 블럭 요청자.
	int targetindex; //친구 블럭 당하는 자.
	char bBlock;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);

	msg >> requesterindex
		>> targetindex
		>> name
		>> bBlock;

//자신을 지우려고 했거나, 요청자가 자신이 아니라면...
	if (targetindex == ch->m_index || requesterindex != ch->m_index)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (targetindex == -1)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int lenname = strlen(name);
	if (lenname < 2 || lenname > 16)
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (strchr(name, '\'') || strchr(name, '\\') || strchr(name, '\"') || strchr(name, '%') || strchr(name, '_'))
	{
		FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CFriendMember* fMember = ch->m_Friend->FindFriendMember(targetindex);

	if( fMember )
	{
		if( bBlock == (char) fMember->isBlock() )
		{
			if( fMember->isBlock() )
				FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_ALREADY_BLOCK);
			else
				FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_NOT_BLOCK);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}
	else
		return;

	if (IS_RUNNING_HELPER)
	{
		HelperFriendBlockReqMsg(rmsg, targetindex, ch->m_index, name, bBlock);
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true);
	}
#endif
}
*/

// 상태 변경하기..
void do_FriendSetCondition(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_MESSENGER
	CNetMsg rmsg;

	int chaindex;
	int condition;

	msg >> chaindex
		>> condition;


	ch->m_nCondition = condition;
//0521
	if(ch->m_Friend)
	{
	if (IS_RUNNING_HELPER) 
	{
		HelperFriendSetConditionMsg(rmsg, ch->m_index, condition, -1, ch);
		SEND_Q(rmsg, gserver.m_helper);			
	}
	}
//임시..릴레이.
	CNetMsg tmsg;
	FriendSetConditionMsg(tmsg, chaindex, condition);
	SEND_Q(tmsg, ch->m_desc);			

/*
	if(ch->m_nCondition != condition)
	{
		ch->m_nCondition = condition;
		
		FriendSetConditionMsg(rmsg,chaindex,condition);

		ch->m_Friend->SendToAll(rmsg);
	}
*/
#endif

}
//메신저 메시지.
void do_FriendChatting(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_MESSENGER
	CNetMsg rmsg;

	int sender_index;
	CLCString sender_name(MAX_CHAR_NAME_LENGTH + 1);
	CLCString receiver_name(MAX_CHAR_NAME_LENGTH + 1);
	CLCString chat(1000);
//	char buf, buf2; 	
		
//	*buf = '\0';
//	*buf2 = '\0';

	msg >> sender_index
		>> sender_name
		>> receiver_name
		>> chat;

	// 내용 없는 채팅은 무시
	if (strlen(chat) < 1)
		return ;

	MsgrFriendChatReqMsg(rmsg,
		0,
		gserver.m_serverno,
		gserver.m_subno,
		ch->m_index,
		ch->GetName(),
		receiver_name,
		chat);
	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
#endif
}