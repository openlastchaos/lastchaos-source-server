#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"

#ifdef EXPEDITION_RAID

/////////////////
// 원정대 관련 함수
void do_Expedition(CPC* ch, CNetMsg& msg)
{
	// 안보이면 무시
	if (!ch->m_bVisible)
		return ;

	unsigned char subtype;

	msg.MoveFirst();

	msg >> subtype;

	switch (subtype)
	{
	case MSG_CREATE_REQ:						// 파티 => 원정대 전환
		do_ExpedCreateReq(ch, msg);
		break;

	case MSG_INVITE_REQ:						// 초대 요청
		do_ExpedInviteReq(ch, msg);
		break;

	case MSG_ALLOW_REQ:							// 초대 수락
		do_ExpedAllowReq(ch, msg);
		break;

	case MSG_REJECT_REQ:						// 초대 거부 요청
		do_ExpedRejectReq(ch, msg);
		break;

	case MSG_QUIT_REQ:							// 탈퇴 요청
		do_ExpedQuitReq(ch, msg);
		break;

	case MSG_ENDEXPED_REQ:						// 원정대 해체
		do_ExpedEndExpedReq(ch, msg);
		break;

	case MSG_KICK_REQ:							// 추방 요청
		do_ExpedKickReq(ch, msg);
		break;

	case MSG_CHANGETYPE_REQ:					// 원정대 타입 변경
		do_ExpedChangeTypeReq(ch, msg);
		break;

	case MSG_CHANGEBOSS_REQ:					// 원정 대장 위임(변경)
		do_ExpedChangeBossReq(ch, msg);
		break;

	case MSG_SETMBOSS_REQ:						// 부대장 임명
		do_ExpedSetMBossReq(ch, msg);
		break;

	case MSG_RESETMBOSS_REQ:					// 부대장 임명 해제
		do_ExpedResetMBossReq(ch, msg);
		break;

	case MSG_MOVEGROUP_REQ:						// 그룹 이동 
		do_ExpedMoveGroupReq(ch, msg);
		break;

	case MSG_ADDMEMBER_REQ:						// 대원 추가
		do_ExpedAddMemberReq(ch, msg);
		break;

	case MSG_VIEWDETAIL_REQ:					// 살펴보기
		do_ExpedViewDetailReq(ch, msg);
		break;

	case MSG_SET_LABEL_REQ:						// 표식 지정
		do_ExpedSetLabelReq(ch, msg);
		break;

// 	case MSG_CHAT_REQ:							// 채팅
// 		do_ExpedChatReq(ch, msg);
// 		break;
	}
}

//원정대 생성(파티 전환)(대장) 
void do_ExpedCreateReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	//파티에 소속되어 있는가?
	if (ch->m_party == NULL)
	{
		// 파티에 소속되지 않음(파티 전환 불가)
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_PARTY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//파티장인가? 
	if(ch->m_party->GetBossIndex() != ch->m_index)
	{
		// 파티장이 아니다.	(파티전환 불가) 
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_PARTYBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;		
	}

	if (IS_RUNNING_HELPER)
	{
		HelperExpedCreateReqMsg(rmsg, ch->m_Exped->GetBossIndex(),ch->GetName());
		SEND_Q(rmsg, gserver.m_helper);
	}
}

//원정대 초대(대장) 
void do_ExpedInviteReq(CPC* ch, CNetMsg& msg)
{
	int destindex;
	
	msg >>  destindex;

	CNetMsg rmsg;

	if (ch->m_index == destindex)
		return ;

	CPC * pTargetPC = gserver.m_playerList.Find(destindex);
	if(pTargetPC == NULL)
		return;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(ch->IsSetPlayerState(PLAYER_STATE_PKMODE) || pTargetPC->IsSetPlayerState(PLAYER_STATE_PKMODE))
	{
		return ;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	if (IS_RUNNING_HELPER)
	{
		HelperExpedInviteReqMsg(rmsg, ch->m_index, ch->GetName(), destindex,pTargetPC->GetName());
		SEND_Q(rmsg, gserver.m_helper);
	}
}

//초대 수락(모두) 
void do_ExpedAllowReq(CPC* ch, CNetMsg& msg)
{
	// 원정대 정보가 있고
	CNetMsg rmsg;

	if(ch->m_Exped != NULL)
	{
		//에러: 원정대에 이미 소속 되어 있음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_ALREADY_JOIN_OTHER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperExpedAllowReqMsg(rmsg, ch->m_Exped->GetBossIndex(), ch->m_index, ch->GetName());
		SEND_Q(rmsg, gserver.m_helper);
	}
}

//초대 거부(모두)
void do_ExpedRejectReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	if(ch->m_Exped != NULL)
	{
		//에러: 원정대에 이미 소속 되어 있음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_ALREADY_JOIN_OTHER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperExpedRejectReqMsg(rmsg, ch->m_Exped->GetBossIndex(), ch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

//탈퇴(모두)
void do_ExpedQuitReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	int nQuitMode;			// 정상,비정상 구분

	msg >> nQuitMode;

	if(ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperExpedQuitReqMsg(rmsg, ch->m_Exped->GetBossIndex(), ch->m_index, nQuitMode);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

//원정대 해체(대장) 
void do_ExpedEndExpedReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	if (IS_RUNNING_HELPER)
	{
		HelperExpedEndExpedMsg(rmsg, ch->m_Exped->GetBossIndex());
		SEND_Q(rmsg, gserver.m_helper);
	}

}

//추방(대장,부대장)
void do_ExpedKickReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int nTargetIndex;
	msg >> nTargetIndex;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_index == nTargetIndex)
		return ;

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS && pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_MBOSS ))
	{
		//에러: 권한 실패
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		HelperExpedKickReqMsg(rmsg, ch->m_Exped->GetBossIndex(), nTargetIndex);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

//원정 대장 위임(대장) 
void do_ExpedChangeBossReq(CPC* ch, CNetMsg& msg)
{
	int nChangeMode, nNewBossIndex;

	msg >> nChangeMode						// 수동,자동
		>> nNewBossIndex;					// 자동(-1)

	CNetMsg rmsg;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	if (IS_RUNNING_HELPER)
	{
		HelperExpedChangeBossReqMsg(rmsg, ch->m_index, nChangeMode, nNewBossIndex);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

//타입 변경(대장) 
void do_ExpedChangeTypeReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	char cExpedType, cDiviType;
	msg >> cExpedType >> cDiviType;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	if(ch->m_Exped->GetExpeditionType(cDiviType) == cExpedType)
		return ;

	if (IS_RUNNING_HELPER)
	{
		HelperExpedChangeTypeReqMsg(rmsg, ch->m_index, cExpedType, cDiviType);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

//부대장 임명(대장) 
void do_ExpedSetMBossReq(CPC* ch, CNetMsg& msg)
{
	int nNewMBossIndex;

	msg >> nNewMBossIndex;

	CNetMsg rmsg;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	if (IS_RUNNING_HELPER)
	{
		HelperExpedSetMBossReqMsg(rmsg, ch->m_index, nNewMBossIndex);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

//부대장 해임(대장) 
void do_ExpedResetMBossReq(CPC* ch, CNetMsg& msg)
{
	int nNewMBossIndex;

	msg >> nNewMBossIndex;

	CNetMsg rmsg;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	if (IS_RUNNING_HELPER)
	{
		HelperExpedResetMBossReqMsg(rmsg, ch->m_index, nNewMBossIndex);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

//그룹 이동(대장)  
void do_ExpedMoveGroupReq(CPC* ch, CNetMsg& msg)
{
	int nSourceGroup, nMoveCharIndex, nTargetGroup, nTargetListindex; 

	msg >> nSourceGroup
		>> nMoveCharIndex
		>> nTargetGroup
		>> nTargetListindex;

	CNetMsg rmsg;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	if (IS_RUNNING_HELPER)
	{
		HelperExpedMoveGroupReqMsg(rmsg, ch->m_index, nSourceGroup, nMoveCharIndex, nTargetGroup, nTargetListindex);
		SEND_Q(rmsg, gserver.m_helper);
	}

}

//대원 추가(대장) 
void do_ExpedAddMemberReq(CPC* ch, CNetMsg& msg)
{
	CLCString addCharName(MAX_CHAR_NAME_LENGTH + 1);

	msg >>	addCharName;
	
	CNetMsg rmsg;

	CPC * pTargetPC = gserver.m_playerList.Find(addCharName,false);
	if(pTargetPC == NULL)
		return;

	if(ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	// 타입이 EXPED_TYPE_BATTLE 이면 tch와 ch의 레벨 차이를 검사
	// 차이가 +- 10이상이면 ERROR;
	if (ch->m_Exped->GetExpeditionType(MSG_DIVITYPE_EXP) == EXPED_TYPE_BATTLE)
	{
		if (ABS(pTargetPC->m_level - ch->m_level) > 10)
		{
			return ;
		}
	}

	if (IS_RUNNING_HELPER)
	{
		//초대 요청
		HelperExpedInviteReqMsg(rmsg, ch->m_index, ch->GetName(), pTargetPC->m_index,pTargetPC->GetName());
		SEND_Q(rmsg, gserver.m_helper);
	}
}

//살펴 보기(대장,부대장)
void do_ExpedViewDetailReq(CPC* ch, CNetMsg& msg)
{
	int nGroup;
	int nDestIndex;

	msg >> nGroup
		>> nDestIndex;

	CNetMsg rmsg;

	if(ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS && pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_MBOSS ))
	{
		//에러: 권한 실패
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	CPC * pPC = gserver.m_playerList.Find(nDestIndex);
	if(pPC == NULL)
		return;

	// 펫정보
	//-->
#ifdef ENABLE_PET
	CPet* pet = pPC->m_petList;
	while (pet)
	{
		ExpedViewDail_ExPetMsg(msg, pet);
		SEND_Q(msg, ch->m_desc);

		pet = pet->m_nextPet;
	}
#endif // #ifdef ENABLE_PET

#ifdef ATTACK_PET
	CAPet* apet = pPC->m_pApetlist;
	while ( apet )
	{
		// Pet 상태 MSG
		if( apet->IsWearing() )
		{
			CNetMsg rmsg;
			ExpedViewDail_ExAPetMsg(msg, apet);
			SEND_Q(msg, ch->m_desc);			
		}
		apet = apet->m_pNextPet;
	}
#endif //ATTACK_PET

	//<--
	
	//인벤토리 정보
	//-->	
	ExpedViewDail_InvenMsg(msg, pPC);
	SEND_Q(msg, ch->m_desc);

	//<--

}

//표식 설정
void do_ExpedSetLabelReq(CPC* ch, CNetMsg& msg)
{
	int nType,nMode,nLabel,nDestIndex;

	msg >> nType				// pc,npc 구분
		>> nMode				// 설정,해제 구분
		>> nLabel				// 라벨 타입 
		>> nDestIndex;			// 설정 대상 인덱스

	CNetMsg rmsg;

	if(ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperExpedSetLabelReqMsg(rmsg, ch->m_index, nType, nMode, nLabel, nDestIndex);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

//채팅
// void do_ExpedChatReq(CPC* ch, CNetMsg& msg)
// {
// 	CLCString strChat(1000);
// 	int		  nGroupTyp;
// 
// 	msg >> nGroupTyp	 //그룹1(0), 그룹 2(1),  그룹 3(2), 그룹 4(3) , 전체(-1)
// 		>> strChat;	    
// 
// 	CNetMsg rmsg;
// 
// 	if(ch->m_Exped == NULL)
// 	{
// 		//에러: 원정대에 소속 되어 있지 않음
// 		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
// 		SEND_Q(rmsg, ch->m_desc);
// 		return;
// 	}
// 
// 	if (IS_RUNNING_HELPER)
// 	{
// 		HelperExpedChatReqMsg(rmsg, ch->m_Exped->GetBossIndex(), ch->m_index, ch->GetName(), nGroupTyp,strChat);
// 		SEND_Q(rmsg, gserver.m_helper);
// 	}
// }


#endif  //EXPEDITION_RAID