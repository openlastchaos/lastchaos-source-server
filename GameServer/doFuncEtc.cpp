#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "DratanCastle.h"

extern CCmdList gexcmdlist;

void do_StatPoint(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype = (unsigned char)-1;
	msg.MoveFirst();
	msg >> subtype;

	switch (subtype)
	{
	case MSG_STATPOINT_USE:
		do_StatPointUse(ch, msg);
		break;

	case MSG_STATPOINT_RESET:
#ifdef ENABLE_CHANGEJOB
		do_StatPointReset(ch, msg);
#endif
		break;
	}
}

void do_StatPointUse(CPC* ch, CNetMsg& msg)
{
	unsigned char type;

	CNetMsg rmsg;

	msg >> type;

	// 남은 포인트 있나 보기
	if (ch->m_statpt_remain < 1)
	{
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_NOTENOUGH);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int val;
	if (ch->UseStatPoint((MSG_STATPOINT_USE_TYPE)type, &val))
	{
		GAMELOG << init("STATPOINT USE", ch)
				<< "TYPE" << delim
				<< type << end;

		StatPointUseMsg(rmsg, ch, (MSG_STATPOINT_USE_TYPE)type, val);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	else
	{
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_CANTUSE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

#ifdef ENABLE_CHANGEJOB
void do_StatPointReset(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

#ifndef LC_KOR
	if (ch->m_job2 == 0)
	{
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_WRONGPACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif

	int reset_str = 0;
	int reset_dex = 0;
	int reset_int = 0;
	int reset_con = 0;
	int reset_all = 0;

	msg >> reset_str
		>> reset_dex
		>> reset_int
		>> reset_con;

	if (reset_str < 0 || reset_dex < 0 || reset_int < 0 || reset_con < 0)
	{
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_WRONGPACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (reset_str > ch->m_statpt_str || reset_dex > ch->m_statpt_dex || reset_int > ch->m_statpt_int || reset_con > ch->m_statpt_con)
	{
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_WRONGPACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	reset_all = reset_str + reset_dex + reset_int + reset_con;

	if (reset_all < 1)
	{
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_WRONGPACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	LONGLONG needmoney = 20000;		// 1 포인트당 필요 나스 
	needmoney *= reset_all;

//#if defined (LC_KOR) // ADULT_SERVER_NEW_BALANCE 이벤트로 20000->0 나스로 변경
//	needmoney = 0;
//#endif // LC_KOR

	if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < needmoney)
	{
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_NOMONEY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	else if (ch->m_moneyItem->Count() == needmoney)
	{
		ItemDeleteMsg(rmsg, ch->m_moneyItem);
		SEND_Q(rmsg, ch->m_desc);
		ch->AddMoney(-needmoney);
	}
	else
	{
		ch->AddMoney(-needmoney);
		ItemUpdateMsg(rmsg, ch->m_moneyItem, -needmoney);
		SEND_Q(rmsg, ch->m_desc);
	}

	GAMELOG << init("STATPOINT RESET", ch)
			<< "MONEY" << delim
			<< needmoney << delim
			<< "RESET POINT" << delim
			<< reset_str << delim
			<< reset_dex << delim
			<< reset_int << delim
			<< reset_con << delim;

	ch->m_statpt_str -= reset_str;
	ch->m_statpt_dex -= reset_dex;
	ch->m_statpt_int -= reset_int;
	ch->m_statpt_con -= reset_con;
	ch->m_statpt_remain += reset_all;

	GAMELOG << "RESULT" << delim
			<< ch->m_statpt_str << delim
			<< ch->m_statpt_dex << delim
			<< ch->m_statpt_int << delim
			<< ch->m_statpt_con << end;

	ch->InitStat();

	StatPointRemainMsg(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);

	SEND_Q(msg, ch->m_desc);

	ch->CalcStatus(true);
}
#endif

void do_Pulse(CPC* ch, CNetMsg& msg)
{
	int pulse;
	unsigned char nation;

	msg.MoveFirst();

	msg >> pulse
		>> nation;

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
	
	float tickDelayTime;
	// 딜레이가 0.06 초과이면 핵 유저로 처리
	msg	>> tickDelayTime;

	if ( tickDelayTime > HACK_LIMIT )
	{
		CNetMsg rmsg;
		HelperDisconnectHackCharacter(rmsg, ch->m_desc->m_index, (const char*)ch->m_desc->m_idname, ch->m_index, (const char*)ch->m_name, HACK_TYPE_PULSE, tickDelayTime);
		if(IS_RUNNING_HELPER)
		{
			SEND_Q(rmsg, gserver.m_helper);
		}

		// 해당 캐릭터 DIS
		//ch->m_desc->m_bclosed = true;
		//return;
	}
	
#endif // DISCONNECT_HACK_CHARACTER 

	if( nation != gserver.m_national )
	{
		ch->m_desc->m_bclosed = true;
		return;
	}
	ch->m_desc->CheckHackPulse(pulse);
}

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
void do_Disconnect_Hack_Character(CPC* ch, CNetMsg& msg)
{
	/*
	CNetMsg rmsg;
	float delay;

	//msg.MoveFirst();
	
	msg >> delay;

	HelperDisconnectHackCharacter(rmsg, ch->m_desc->m_index, (const char*)ch->m_desc->m_idname, ch->m_index, (const char*)ch->m_name, HACK_TYPE_CLIENT, delay);
	if(IS_RUNNING_HELPER)
	{
		SEND_Q(rmsg, gserver.m_helper);
	}

	// 해당 캐릭터 DIS
	ch->m_desc->m_bclosed = true;
	return;
	*/
}
#endif // DISCONNECT_HACK_CHARACTER 

void do_Change(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype;
	
	msg.MoveFirst();

	msg >> subtype;

	if (subtype != MSG_CHANGE_CANCEL)
		return;

	if (ch->IsSetPlayerState(PLAYER_STATE_CHANGE) && ch->m_changeIndex != -1)
		ch->CancelChange();
}

void do_UI(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype;
	char plusEffect;

	msg.MoveFirst();

	msg >> subtype
		>> plusEffect;

	if (subtype != MSG_UI_PLUS_EFFECT_REQ)
		return;

	ch->m_plusEffect = plusEffect;

	CNetMsg rmsg;
	UIPlusEffectRep(rmsg, ch);
	ch->m_pArea->SendToCell(rmsg, ch, true);
}

void do_Extend(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();

	int exmsg;

	msg >> exmsg;

	int cmd_num = gexcmdlist.Find(ch, exmsg);
	if (cmd_num < 0) // invalid command.. 
	{
		GAMELOG << init("SYS_WARN", ch)
			<< "Invalid extend command" << delim
			<< exmsg
			<< end;
		return;
	}

	gexcmdlist.Run(cmd_num, ch, msg);
}

void do_Ex_NameChange(CPC* ch, CNetMsg& msg)
{
#ifdef FEEITEM
	// TODO : BS : 길드와 캐릭명의 길이가 달라질 경우 수정이 필요
	// 이름 변경 아이템	: tab_idx(uc) row_idx(uc) col_idx(uc) item_idx(n) name(str)

	//msg.MoveFirst();

	int itemindex;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);

	msg >> itemindex
		>> name;

	// 이름이 0 보다 작으면 안된다.
	if (strlen(name) < 4 || strlen(name) > 16)
	{
		CNetMsg failmsg;
		FailMsg(failmsg, MSG_FAIL_WRONG_CHAR);
		SEND_Q(failmsg, ch->m_desc);
		return;
	}
	// 이름에 ' 가 들어 있으면 안된다.
	if (strinc(name, "'") || strinc(name, " "))
	{
		CNetMsg failmsg;
		FailMsg(failmsg, MSG_FAIL_WRONG_CHAR);
		SEND_Q(failmsg, ch->m_desc);
		return;
	}

	switch(itemindex)
	{
		// 케릭 이름 변경
	case 842:
		{
			CNetMsg msg;
			HelperNameChangeReqMsg(msg, 0, ch->m_index, name);
			if(IS_RUNNING_HELPER)
			{
				SEND_Q(msg, gserver.m_helper);
			}
			else
			{
				// 이름을 변경하지 못해요 
				NameChangeRepMsg(msg, MSG_EX_NAMECHANGE_ERROR_HELPER, name, 0); 
				SEND_Q(msg, ch->m_desc);
			}
		}
		break;
		// 길드 이름 변경
	case 843:
		{
			CNetMsg msg;
			if(ch->m_guildInfo == NULL || ch->m_guildInfo->guild() == NULL)
			{
				NameChangeRepMsg(msg, MSG_EX_NAMECHANGE_ERROR_NOTGUILDBOSS, name, 1);
				SEND_Q(msg, ch->m_desc);
				return;
			}
			if( ch->m_index != ch->m_guildInfo->guild()->boss()->charindex() )
			{
				NameChangeRepMsg(msg, MSG_EX_NAMECHANGE_ERROR_NOTGUILDBOSS, name, 1);
				SEND_Q(msg, ch->m_desc);
				return;
			}
			HelperNameChangeReqMsg(msg, 1, ch->m_guildInfo->guild()->index(), name);
			if(IS_RUNNING_HELPER)
			{
				SEND_Q(msg, gserver.m_helper);
			}
			else
			{
				// 이름을 변경하지 못해요 
				NameChangeRepMsg(msg, MSG_EX_NAMECHANGE_ERROR_HELPER, name, 1); 
				SEND_Q(msg, ch->m_desc);
			}
		}
		break;
	}
#endif // FEEITEM
}

void do_ExEvocationStop(CPC* ch, CNetMsg& msg)
{
	if (ch->m_evocationType != EVOCATION_NONE)
		ch->Unevocation();
}

void do_ExPartyRecall(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int subtype;
	msg >> subtype;

	switch (subtype)
	{
	case MSG_EX_PARTY_RECALL_CONFIRM:
		{
			int reqcharindex = 0;
			char yesno = 0;
			CLCString reqcharname(MAX_CHAR_NAME_LENGTH + 1);
			msg >> reqcharindex
				>> reqcharname
				>> yesno;

			if (!ch->IsParty())
				return ;

			if (ch->m_index == reqcharindex)
				return ;
				
			if (ch->m_pZone->IsPersonalDungeon() || ch->m_pZone->IsGuildRoom())
				return ;

			if (ch->m_party->FindMember(reqcharindex) == -1)
				return ;

			if (yesno && IS_RUNNING_HELPER)
			{
				HelperPartyRecallConfirmMsg(rmsg, ch->m_party->GetBossIndex(), reqcharindex, reqcharname, ch->m_index, ch->GetName(), yesno);
				SEND_Q(rmsg, gserver.m_helper);
			}
			else
			{
				// 거부
				ExPartyRecallCancelMsg(rmsg, reqcharindex, reqcharname, ch->m_index, ch->GetName());
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;

	default:
		break;
	}
}

void do_ExMessenger(CPC* ch, CNetMsg& msg)
{
#ifdef MESSENGER_NEW
	char subtype;

	msg >> subtype;

	switch(subtype)
	{
#ifdef MSG_VER2
	case MSG_EX_MESSENGER_ONE_VS_ONE:	// 1:1대화 : sCharIndex(n) tCharIndex(x) chat(str)
		{
			int sCharIndex, tCharIndex;
			CLCString chat(1000);
			CNetMsg rmsg, lmsg;

			msg >> sCharIndex
				>> tCharIndex
				>> chat;

			CPC* tpc = NULL;
			tpc	= gserver.m_playerList.Find(tCharIndex);
			// 친구 온라인 여부 확인
			CFriendMember * pMember = ch->m_Friend->FindFriendMember(tCharIndex);
			if( pMember == NULL || 
				pMember->GetCondition() == MSG_FRIEND_CONDITION_OFFLINE )
			{	// 에러전송
				ChatOneOffLineMessengerMsg(rmsg, sCharIndex, tCharIndex);
				SEND_Q(rmsg, ch->m_desc);
			}
			else if( IS_RUNNING_MSGR )
			{ 
				MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_ONE_VS_ONE, sCharIndex, tCharIndex, 
					ch->m_Friend->GetChatColor(), ch->GetName(), chat );					
				SEND_Q(rmsg, gserver.m_messenger);

				ChatOneMessengerMsg(lmsg, sCharIndex, ch->GetName(), tCharIndex, "",
					ch->m_Friend->GetChatColor(), chat );					
				SEND_Q( lmsg, ch->m_desc );
			}
		}
		break;
#endif // MSG_VER2
	case MSG_EX_MESSENGER_INVITE:	// 해당 케릭터 초대 : makeCharIndex(n) chatIndex(n) charindex(n)	// chatIndex 가 -1이면 대화방 추가
		{
			int chatIndex, makeCharIndex, charIndex;
			CNetMsg rmsg;

			msg >> makeCharIndex
				>> chatIndex
				>> charIndex;

			GAMELOG << init( "MESSENGER_INVITE", ch )
					<< makeCharIndex << delim
					<< chatIndex << delim
					<< charIndex					
					<< end;


			if( chatIndex == -1 )
			{	// 대화방 생성일 경우 
				
				// 대화방 생성
				makeCharIndex = ch->m_index;
				chatIndex = gserver.GetMaxChatIndexPerPC(makeCharIndex) + 1;
				//if ( result != -1 )
				//	gserver.SendChatGroup( (MSG_EX_MESSENGER_TYPE) MSG_EX_MESSENGER_INVITE , makeCharIndex, chatIndex, 0, ch->GetName(), "" );
				if( IS_RUNNING_MSGR )
				{
					MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_ADD, makeCharIndex, chatIndex, 0, ch->GetName() );
					SEND_Q(rmsg, gserver.m_messenger);										
				}

			}

			// 초대 케릭터 추가
			if( IS_RUNNING_MSGR )
			{
				// 메신저로 보내면 거시기가 알아서 할꺼여
				MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_ADD_CONFIRM, makeCharIndex, chatIndex, 0, charIndex);
				SEND_Q(rmsg, gserver.m_messenger);
			}
		}
		break;
	case MSG_EX_MESSENGER_OUT:		// 해당 케릭터 대화방에서 나감 : chatIndex(n) charName(str)
		{
			int chatIndex, makeCharIndex;

			msg >> makeCharIndex
				>> chatIndex;

			if( IS_RUNNING_MSGR )
			{
				// 메신저로 보내면 거시기가 알아서 할꺼여
				CNetMsg rmsg;
				MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_DEL, makeCharIndex, chatIndex, 0, ch->GetName() );
				SEND_Q(rmsg, gserver.m_messenger);
			}
		}
		break;
	case MSG_EX_MESSENGER_CHAT:		// 해당 케릭터 대화방에서 나감 : chatIndex(n) chat(str)
		{
			CLCString chat(1000);
			int chatIndex, makeCharIndex;

			msg >> makeCharIndex
				>> chatIndex
				>> chat;

			if( IS_RUNNING_MSGR && ch->m_Friend )
			{
				CNetMsg rmsg;
				MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_CHAT, makeCharIndex, chatIndex, ch->m_Friend->GetChatColor(), ch->GetName(), chat);
				
				SEND_Q(rmsg, gserver.m_messenger);
			}
		}
		break;

	case MSG_EX_MESSENGER_GROUP_ADD:	// 친구 그룹 추가	: groupName(str)
		{
			CLCString groupName(MAX_GROUP_NAME_LENGTH+1);
			msg >> groupName;

			int gIndex = ch->m_Friend->addGroup(groupName);

			CNetMsg rmsg;
			rmsg.Init(MSG_EXTEND);
			rmsg << MSG_EX_MESSENGER
				 << (unsigned char) subtype
				 << gIndex
				 << groupName;

			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_EX_MESSENGER_GROUP_DEL:	// 친구 그룹 삭제	: gIndex(n)
		{
			int gIndex;
			msg >> gIndex;

			if( gIndex == 0 )
			{
				gIndex = -1;
			}
			else
				gIndex = ch->m_Friend->delGroup(gIndex);

			CNetMsg rmsg;
			rmsg.Init(MSG_EXTEND);
			rmsg << MSG_EX_MESSENGER
				 << (unsigned char) subtype
				 << gIndex;

			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_EX_MESSENGER_GROUP_CHANGE:	// 친구 그룹 이름변경 : gIndex(n) newName(str)
		{
			int gIndex;
			CLCString newName(MAX_GROUP_NAME_LENGTH+1);
			msg >> gIndex
				>> newName;
			gIndex = ch->m_Friend->changeGroupName(gIndex, newName);
			CNetMsg rmsg;
			rmsg.Init(MSG_EXTEND);
			rmsg << MSG_EX_MESSENGER
				 << (unsigned char) subtype
				 << gIndex
				 << newName;

			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_EX_MESSENGER_GROUP_MOVE:	// 친구 그룹 이동	  : charName(str) gIndex(n) 
		{
			int gIndex, charIndex;

			msg >> gIndex
				>> charIndex;

			gIndex = ch->m_Friend->moveGroupFriend(gIndex, charIndex);
			CNetMsg rmsg;
			rmsg.Init(MSG_EXTEND);
			rmsg << MSG_EX_MESSENGER
				 << (unsigned char) subtype
				 << gIndex
				 << charIndex;

			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_EX_MESSENGER_CHATCOLOR_CHAGNE:
		{
			int color = 0;
			msg >> color;

			ch->m_Friend->SetChatColor(color);
		}
		break;
	case MSG_EX_MESSENGER_BLOCK:		// 해당유저 차단	: name(str)
								// GS->C			: errcode(c) charIndex(n) name(str)
		{
			CLCString name(MAX_CHAR_NAME_LENGTH + 1);
			msg >> name;


			if( strinc(name, "'") || strinc(name, "%") || strinc(name, " ") )
			{
				CNetMsg rmsg;
				BlockPCRepMsg(rmsg, MSG_EX_MESSENGER_BLOCK_INVALIDNAME, -1, name);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 최대 블럭 인원수 20명 
			if( ch->m_listBlockPC.GetCount() >= BLOCK_MAX_MEMBER )
			{
				CNetMsg rmsg;
				BlockPCRepMsg(rmsg, MSG_EX_MESSENGER_NOMORE_BLOCK, -1, name);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 헬퍼로 블럭 요청하고 헬퍼에서 인덱스와 이름을 받는다
			if( IS_RUNNING_HELPER )
			{
				CNetMsg rmsg;
				rmsg.Init(MSG_HELPER_COMMAND);
				rmsg << MSG_HELPER_BLOCKPC_REQ
					 << ch->m_index
					 << name;

				SEND_Q(rmsg, gserver.m_helper);
				ch->m_desc->WaitHelperReply(true, 5);
			}
		}
		break;
	case MSG_EX_MESSENGER_RELEASE_BLOCK:	// 해당유저 해제: charIndex(n)
								// GS->C			: errcode(c) charIndex(n) name(str)
		{
			int charIndex;

			msg >> charIndex;
			CLCString name(MAX_CHAR_NAME_LENGTH + 1 );
			ch->ReleaseBlockPC(charIndex, name);

			CNetMsg rmsg;
			rmsg.Init(MSG_EXTEND);
			rmsg << MSG_EX_MESSENGER
				 << (unsigned char) MSG_EX_MESSENGER_RELEASE_BLOCK;
			if( strcmp( name, "" ) == 0 )
				 rmsg << (unsigned char) MSG_EX_MESSENGER_BLOCK_NOTCHAR;
			else
				rmsg << (unsigned char) MSG_EX_MESSENGER_SUCCESS;

			rmsg << charIndex
				 << name;

			SEND_Q(rmsg, ch->m_desc);

			if( ch->m_Friend )
			{
				HelperFriendSetConditionMsg(rmsg, ch->m_index, ch->m_nCondition, -1, ch);
				SEND_Q(rmsg, gserver.m_helper);			
			}
		}
		break;
	}
#endif // MESSENGER_NEW
}
#ifdef DRATAN_CASTLE
void do_ExCastleWar(CPC * ch, CNetMsg & msg)
{
	char subtype;

	msg >> subtype;

	switch(subtype)
	{
	case MSG_CASTLE_CRISTAL_RESPOND_START:	// 교감 시작
		do_ExCastleCristalRespondStart(ch, msg);
		break;

	case MSG_CASTLE_CRISTAL_RESPOND_END:	// 교감 완료
		do_ExCastleCristalRespondEnd(ch, msg);
		break;
		
	case MSG_CASTLE_TOWER_CONTRAL_LIST:		// 타워 리스트
		do_ExCastleTowerList(ch, msg);
		break;

	case MSG_CASTLE_TOWER_CONTRAL:			// 타워 리스트 추가
		do_ExcastleTowerAdd(ch, msg);
		break;

	case MSG_CASTLE_TOWER_REINFORCE:			// 마스터 타워 강화 설정		type(c), step(c)
		do_ExCastleTowerReinforce(ch, msg);
		break;

	case MSG_CASTLE_TOWER_REINFORCE_LIST:	// 마스터 타워 강화 리스트		(type(c), step(c))
		do_ExCastleTowerReinforceList(ch, msg);
		break;

	case MSG_CASTLE_TOWER_REPAIRE:			// 공성 타워 수리				cl->gs idx(n) | gs->cl	idx(n)
		do_ExCastleTowerRepaire(ch, msg);
		break;

	case MSG_CASTLE_TOWER_REPAIRE_LIST:		// 공성 타워 수리 상태			cl->gs idx(n) | gs->cl	money(n)
		do_ExCastleTowerRepaireList(ch, msg);
		break;

	case MSG_CASTLE_TOWER_WARP_LIST:		// 워프 타워 리스트 요청
		do_ExCastleTowerWarpList(ch, msg);
		break;

	case MSG_CASTLE_QUARTERS_INSTALL:		// 부활진지 설치
		do_ExCastleQuartersInstall(ch, msg);
		break;
	}
}

void do_ExCastleCristalRespondStart(CPC * ch, CNetMsg & msg)
{
	CNetMsg rmsg;

	// 공성전 참가 확인
	if (ch->GetJoinFlag(ZONE_DRATAN) == WCJF_NONE)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_JOIN);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 공성중 확인
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle == NULL || pCastle->GetState() == WCSF_NORMAL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_WAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 위치 확인
	if(ch->m_pZone->m_index != ZONE_DRATAN)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_POS_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 인원 확인 최대 5명
	if(pCastle->GetRespondCount() > 5)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_MEM_5_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 성주 길드 확인
	if(pCastle->GetOwnerGuildIndex() == ch->m_guildInfo->guild()->index())
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_OWNER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	// NPC(외성문 결계의 눈:388) 파괴 체크
	if( pCastle->m_gateNPC[0] && !DEAD(pCastle->m_gateNPC[0]))
	{
		CastleErrorMsg(rmsg, MSG_CASTLE_CRISTAL_RESPOND_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// NPC(내성문 결계의 눈:389,404) 파괴 체크
	if( (pCastle->m_gateNPC[1] && !DEAD(pCastle->m_gateNPC[1]))
		&& ( pCastle->m_gateNPC[2] && !DEAD(pCastle->m_gateNPC[2])) )
	{
		CastleErrorMsg(rmsg, MSG_CASTLE_CRISTAL_RESPOND_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	//////// yhj 081103   인비저빌리티 상태에서는 크리스탈 교감 불가능//////////
	if ( ch->IsInvisible() )
	{
		// 인비저빌리티 상태를 없애서 다굴당하게 함.. 
		ch->m_assist.CureAssist(MST_ASSIST_INVISIBLE, 99);

		return;
	}//////////////////////////////////////////////////////////////////////////
	
	//////// yhj 081218   레벨이 15보다 낮으면 크리스탈 교감 불가능//////////
	if ( ch->m_level <= PKMODE_LIMIT_LEVEL )
	{
		CastleErrorMsg(rmsg, MSG_CASTLE_CRISTAL_RESPOND_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}//////////////////////////////////////////////////////////////////////////

	// 캐릭터 상태 변화
	ch->SetPlayerState(PLAYER_CRISTAL_RESPOND);

	// 교감 시작 시간 저장
//	ch->SetRespondTime(gserver.m_pulse);
	time_t curTime;
	time(&curTime);
	ch->SetRespondTime(curTime);

	// 공성 교감 인원 추가
	pCastle->SetRespondMember(ch);
	
	// 주위에 전달
	CastleCristalRespondStartMsg(rmsg, ch);
	ch->m_pArea->SendToCell(rmsg, ch, true);
}

void do_ExCastleCristalRespondEnd(CPC * ch, CNetMsg & msg)
{
	CNetMsg rmsg;
	// 공성전 참가 확인
	if (ch->GetJoinFlag(ZONE_DRATAN) == WCJF_NONE)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_JOIN);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 공성중 확인
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle == NULL || pCastle->GetState() == WCSF_NORMAL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_WAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 위치 확인
	if(ch->m_pZone->m_index != ZONE_DRATAN)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_POS_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 캐릭터 상태 확인
	if(ch->IsSetPlayerState(PLAYER_CRISTAL_RESPOND) == false)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_RESPOND);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
///===
	// 유지 시간 확인 3분
//	if(ch->GetRespondTime() == 0  //교감 시작 시간이 없거나 
//		|| gserver.m_pulse	- ch->GetRespondTime() < 3*PULSE_REAL_MIN) // 3분이내면
//	{
//		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_RESPOND_TIME);
//		SEND_Q(rmsg, ch->m_desc);
//		return;
//	}

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
	// 유지 시간 확인 3분
	time_t curTime;
	time(&curTime);
	if(ch->GetRespondTime() == 0  //교감 시작 시간이 없거나 
		|| curTime	- ch->GetRespondTime() < 175 ) // 2분 55초이내면
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_RESPOND_TIME);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
#endif // DISCONNECT_HACK_CHARACTER 

	// 교감 정보 삭제
	pCastle->InitRespond();

	// 주위에 전달
	CastleCristalRespondEndMsg(rmsg, ch);
//	ch->m_pArea->SendToCell(rmsg, ch, true);
	ch->m_pArea->SendToAllClient(rmsg);



	// 성주 바꿈
	CGuildMember * pMember = ch->m_guildInfo;
	if (pMember == NULL)
	{
		return;
	}
	CGuild * pGuild = pMember->guild() ;
	if (pGuild == NULL)
	{
		return;
	}
		
	GAMELOG << init("DRATAN CASTLE CHANGE OWNER")
		<< "before " << delim << pCastle->GetOwnerGuildIndex() << delim
		<< "after " << delim  << pGuild->index() << delim << "char " << delim << ch->m_index << end;

	// 공성 리셋
	// 죽은 부활진지 리젠  -071016 - whs25 추가 작업
	pCastle->RegenCastleControlNPC();

	// 해당 길드 부활 진지 존재 -> 부활 진지 삭제
	for(int i=0; i<7; i++)
	{
		if (pCastle->m_nRebrithGuild[i] == pGuild->index())
		{
			pCastle->m_nRebrithGuild[i] = -1;
			memset((void *)pCastle->m_strRebrithGuild[i], 0, 51);
			break;
		}
	}

	// 부활 진지 정보 전송
	CastletowerQuartersListMsg(rmsg, pCastle);
	ch->m_pArea->SendToAllClient(rmsg);

	// 성주 길드 플래그 <=> 공성 길드 플래그 변환
	pCastle->ChangeSide(pGuild->index());

	// 성주 길드 변환
	pCastle->SetOwner(pGuild);

	//  NPC_CASTLE_TOWER hp 1/2후 리젠
	pCastle->RegenCastleTowerHalf();

	//  해당 길드 이외 캐릭터 부활진지, 간이마을로 이동
	pCastle->GetOutNonDefense();

	//	공성 정보 전송
	GuildWarCastleStateMsg(rmsg, ZONE_DRATAN, ch, pCastle);
	ch->m_pArea->SendToAllClient(rmsg);
}

void do_ExCastleTowerList(CPC * ch, CNetMsg & msg)
{	// 리스트 요청	
	// 캐릭터 확인
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle != NULL										// 드라탄 공성 존재
		&& pCastle->GetState() == WCSF_NORMAL				// 공성중이 아닐때
		&& pCastle->GetOwnerCharIndex() == ch->m_index		// 성주 일 경우
		&& ch->m_pZone->m_index == ZONE_DRATAN)				// 캐릭터가 드라탄에 있을때
	{
		CNetMsg rmsg;
		CastleTowerListMsg(rmsg, pCastle);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExcastleTowerAdd(CPC * ch, CNetMsg & msg)
{
	char status[7];
	memset(status, 0, 7);
	
	for(int i = 0; i < 7; i++)
	{
		msg >> status[i];
	}

	CNetMsg rmsg;

	// 캐릭터 확인
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle != NULL										// 드라탄 공성 존재
		&& pCastle->GetState() == WCSF_NORMAL				// 공성중이 아닐때
		&& pCastle->GetOwnerCharIndex() == ch->m_index		// 성주 일 경우
		&& ch->m_pZone->m_index == ZONE_DRATAN)				// 캐릭터가 드라탄에 있을때
	{
		// 돈확인
		int nMoney = 1000000;
		int total_money = 0;

		int change_status = 0;
		for(int i = 0; i< 7; i++)
		{
			change_status = status[i] - pCastle->GetTowerStatus(i);
			if (change_status > 0)
			{	// 변화 체크
				for(int j = pCastle->GetTowerStatus(i); j < status[i]; j++)
				{
					total_money += (j+1) * nMoney;
				}
			}
		}

		int row = 0, col = 0;
		if( ch->m_invenNormal.FindItem(&row, &col, gserver.m_itemProtoList.m_moneyItem->m_index,  0, 0) == false )
		{
			CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		CItem * pMoney = ch->m_invenNormal.GetItem(row, col);
		if (pMoney != NULL
			&& pMoney->Count() >= total_money)
		{
			DecreaseFromInventory(ch, pMoney, total_money);
			if (pMoney->Count() > 0)
			{
				ItemUpdateMsg(rmsg, pMoney, -total_money);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemDeleteMsg(rmsg, pMoney);
				SEND_Q(rmsg, ch->m_desc);

				RemoveFromInventory(ch, pMoney, true, true);
			}
		}
		else
		{	// 돈부족 메세지 전송
			CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		// 적용
		for(int k = 0; k < 7; k++)
		{
			pCastle->SetTowerStatus(k, status[k]);
		}

		HelperCastleTowerAddMsg(rmsg, ZONE_DRATAN, pCastle);
		SEND_Q(rmsg, gserver.m_helper);

//		CastleTowerListMsg(rmsg, pCastle);
		CastleTowerAddMsg(rmsg, pCastle);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExCastleTowerReinforce(CPC * ch, CNetMsg & msg)
{	// 타워 강화
	CNetMsg rmsg;

	// 수성 길드원인지 확인
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle == NULL
		&& pCastle->GetOwnerGuildIndex() != ch->m_guildInfo->guild()->index())
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_ATTACK);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	char type = 0, step = 0;
	msg >> type >> step;

	// type : 0. 공격형 타워 1. 가드형 타워 2. 성문
	// step 1 - 3
	if (type < 0 || type > 3 || step <= 0 || step > 3)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_SAME_DATA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 이전과 비교
	char before_step = pCastle->GetReinforceStep((int)type);
	char diff = step - before_step;
	if (diff <= 0 || step > 3)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_DIFFER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 가격 비교
	int total_money = 0;
	int price[3][3] = { 
		{ 1000000, 1000000, 2000000 },
		{ 2000000, 2000000, 2000000 },
		{ 3000000, 3000000, 3000000 },
	};

	for(char i = step; i > before_step; i--)
	{
		total_money += price[(int)i-1][(int)type];
	}

	int row = 0, col = 0;
	if( ch->m_invenNormal.FindItem(&row, &col, gserver.m_itemProtoList.m_moneyItem->m_index,  0, 0) == false )
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem * pMoney = ch->m_invenNormal.GetItem(row, col);
	if (pMoney != NULL
		&& pMoney->Count() >= total_money)
	{
		DecreaseFromInventory(ch, pMoney, total_money);
		if (pMoney->Count() > 0)
		{
			ItemUpdateMsg(rmsg, pMoney, -total_money);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, pMoney);
			SEND_Q(rmsg, ch->m_desc);

			RemoveFromInventory(ch, pMoney, true, true);
		}
	}
	else
	{	// 돈부족 메세지 전송
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 적용
	pCastle->SetReinforceStep(type, step);

	// 저장
	HelperCastleReinforceMsg(rmsg, type, step, ZONE_DRATAN);
	SEND_Q(rmsg, gserver.m_helper);

//	CastleTowerReinforceListMsg(rmsg, type, pCastle);
	CastleTowerReinforceMsg(rmsg, type, pCastle);
	SEND_Q(rmsg, ch->m_desc);

	///=== 성공 메세지 필요
}

void do_ExCastleTowerReinforceList(CPC * ch, CNetMsg & msg)
{	// 마스터 타워 강화 리스트
	CNetMsg rmsg;
	char type = 0; 
	msg >> type;
	if( type < 0 || type > 2)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_DATA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL
		&& pCastle->GetOwnerGuildIndex() == ch->m_guildInfo->guild()->index())
	{
		CastleTowerReinforceListMsg(rmsg, type, pCastle);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExCastleTowerRepaire(CPC * ch, CNetMsg & msg)
{	// 수리 요청
	int idx = 0;
	msg >> idx;

	CNetMsg rmsg;

	if (idx < 351 || idx > 381)	// 마스터 타워 포함
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_DATA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// npc리스트에서 검색
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle == NULL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_WAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (pCastle->CheckBuyTower(idx) == false)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_BUY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CNPCProto * pProto = gserver.m_npcProtoList.FindProto(idx);
	if( pProto == NULL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	int zidx = gserver.FindZone(ZONE_DRATAN);
	if (zidx < 0)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_POS_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CArea * pArea = gserver.m_zones[zidx].m_area;
	if( pArea == NULL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_POS_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 살아 있는지 검색
	CNPC * pNpc = pArea->FindProtoNPCInCell( ch, idx, false, 9, true);
	if (pNpc == NULL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(DEAD(pNpc))
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_DEAD_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 수리비용 검색
	LONGLONG total_money = 0;
	if (idx == 351)
	{	// 마스터 타워
		total_money = 10000000;
	}
	else
	{
		total_money = ( ( (idx - 352) % 5) + 1 ) * 1000000;
	}
	
	total_money = total_money - (total_money * pNpc->m_hp) / (pNpc->m_maxHP);

	// 돈 검사
	int row = 0, col = 0;
	if( ch->m_invenNormal.FindItem(&row, &col, gserver.m_itemProtoList.m_moneyItem->m_index, 0, 0) == false )
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem * pMoney = ch->m_invenNormal.GetItem(row, col);
	if (pMoney != NULL
		&& pMoney->Count() >= total_money)
	{
		DecreaseFromInventory(ch, pMoney, total_money);
		if (pMoney->Count() > 0)
		{
			ItemUpdateMsg(rmsg, pMoney, -total_money);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, pMoney);
			SEND_Q(rmsg, ch->m_desc);

			RemoveFromInventory(ch, pMoney, true, true);
		}
	}
	else
	{	// 돈부족 메세지 전송
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 적용
	pNpc->m_hp = pNpc->m_maxHP;
	pNpc->m_mp = pNpc->m_maxMP;
	
	// 주변에 전송
	CharStatusMsg(rmsg, pNpc, 0);
	ch->m_pArea->SendToCell(rmsg, ch, true);

	CastleTowerRepaireMsg(rmsg, idx, total_money);
	SEND_Q(rmsg, ch->m_desc);
}

void do_ExCastleTowerRepaireList(CPC * ch, CNetMsg & msg)
{	// 수리 비용 전송
	int idx = 0;
	msg >> idx;

	CNetMsg  rmsg;

	if (idx < 351 || idx > 381)	// 마스터 타워 포함
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_DATA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// npc리스트에서 검색
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle == NULL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_WAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (pCastle->CheckBuyTower(idx) == false)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_BUY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CNPCProto * pProto = gserver.m_npcProtoList.FindProto(idx);
	if( pProto == NULL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int zidx = gserver.FindZone(ZONE_DRATAN);
	if (zidx < 0)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_POS_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CArea * pArea = gserver.m_zones[zidx].m_area;
	if (pArea == NULL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_POS_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 살아 있는지 검색
	CNPC * pNpc = pArea->FindProtoNPCInCell(ch, idx, false, 9, true);
	if (pNpc == NULL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(DEAD(pNpc))
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_DEAD_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 수리비용 검색
	LONGLONG total_money = 0;
	if (idx == 351)
	{	// 마스터 타워
		total_money = 10000000;
	}
	else
	{
		total_money = ( ( (idx - 352) % 5) + 1 ) * 1000000;
	}
	
	total_money = total_money - (total_money * pNpc->m_hp) / (pNpc->m_maxHP);

	CastleTowerRepaireListMsg(rmsg, idx, total_money);
	SEND_Q(rmsg, ch->m_desc);
}

void do_ExCastleTowerWarpList(CPC * ch, CNetMsg & msg)
{
	CNetMsg rmsg;
	int curindex = 0;
	msg >> curindex;

	// 워프타워 번호 확인
	if (curindex < 382 || curindex > 386)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_DATA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle == NULL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_WAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 가동중인 워프 타워 확인
	char total = 0;
	int aliveindex[5];
	for(char i = 0; i < 5; i++)
	{
		if(pCastle->m_pWarpNPC[i] == NULL)
		{
			continue;
		}

		CNPCProto * pProto = pCastle->m_pWarpNPC[i]->m_proto;
		if (pProto == NULL)
		{
			continue;
		}
		aliveindex[total] = pCastle->m_pWarpNPC[i]->m_proto->m_index;
		total++;
	}

	CastleTowerWarpListMsg(rmsg, aliveindex, total);
	SEND_Q(rmsg, ch->m_desc);
}

void do_ExCastleQuartersInstall(CPC * ch, CNetMsg & msg)
{
	int qindex = 0;
	CNetMsg rmsg;

	msg >> qindex;

	// 공성확인
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle == NULL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_WAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 부활진지 번호 확인
	if(qindex < 390 || qindex > 396)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_DATA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	// 캐릭터의 길드 확인
	CGuildMember * pMember = ch->m_guildInfo;
	if (pMember == NULL)
	{	
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_HAVE_NO_GUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CGuild * pGuild = pMember->guild();
	if (pGuild == NULL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_HAVE_NO_GUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int gindex = pGuild->index();
	if (gindex < 0)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_HAVE_NO_GUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 공성길드 확인
	if (ch->GetJoinFlag(ZONE_DRATAN) != WCJF_ATTACK_GUILD)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_ATTACK);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 선택한 부활진지 살아있는지 확인
	int idx = qindex - 390;
	if (pCastle->m_pRebrithNPC[idx] == NULL)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_NPC2);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (DEAD(pCastle->m_pRebrithNPC[idx]))
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_DEAD_NPC2);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 부활 진지를 한번이라도 사용 했는지 체크  - whs25 추가
	if( pGuild->IsRebrithPos() )
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_BUY_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	// 선택한 부활 진지 사용중 확인
	if (pCastle->m_nRebrithGuild[idx] != -1)
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_USE_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 다른 부활진지 사용중 확인
	for(int i=0; i<7; i++)
	{
		if (pCastle->m_nRebrithGuild[i] == gindex)		
		{
			CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_USE_NPC_OTHER);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	const int nNeedMoney[10] = { 3000000, 2000000, 2000000, 2000000, 2000000, 
							1000000, 1000000, 1000000, 1000000, 1000000 };

	if( ch->m_moneyItem->Count() < nNeedMoney[idx] )
	{
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_MONEY); 	// 돈이 부족합니다. c
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_moneyItem->Count() > nNeedMoney[idx])
	{
		DecreaseFromInventory(ch, ch->m_moneyItem, nNeedMoney[idx]);
		ItemUpdateMsg(rmsg, ch->m_moneyItem, -nNeedMoney[idx]);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		ItemDeleteMsg(rmsg, ch->m_moneyItem);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, ch->m_moneyItem, true, true);
	}

	// 몹의 리스트 인덱스
	int list_index = 0;
	list_index = pCastle->m_pRebrithNPC[idx]->m_index;
	
	// 부활 진지 설치
	pCastle->m_nRebrithGuild[idx] = gindex;
	strncpy(pCastle->m_strRebrithGuild[idx], pGuild->name(), 51);

	HeplerCastleRebrithInstallMsg(rmsg, qindex, gindex, pGuild->name());
	SEND_Q(rmsg, gserver.m_helper);

	// 090106 yhj  함수 수정함..매개변수 한개 추가
	CastleTowerRebrithInstallMsg(rmsg, qindex, gindex, pGuild->name(), list_index);
	ch->m_pArea->SendToAllClient(rmsg);

	pGuild->SetRerithPos( true );
	
	// 부활 진지 사용 로그
	GAMELOG << init("WAR CASTLE : BUY QUARTERS") << delim << "QUARTER index" << delim << qindex 
		<< delim << "GUILD index" << delim << gindex << delim << "GUILD name" << delim << pGuild->name() << end;
}


#ifdef DYNAMIC_DUNGEON
void do_DVD(CPC * ch, CNetMsg & msg)
{
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle == 0)
	{	// 드라탄 공성 없을때
		return;
	}

	if (pCastle->m_dvd.GetZone() == 0)
	{	// 드라탄 공성 던전 지역이 없을때
		return;
	}

	char subtype;
	msg >> subtype;

	switch(subtype)
	{
	case MSG_DUNGEON_ENTER:
		do_dvdDungenEnter( ch, msg );
		break;

	case MSG_DUNGEON_INFO :
		do_dvdInfo(ch, msg);
		break;

	case MSG_MANAGEMENT:
		do_dvdManagement( ch, msg );
		break;
	}
}

void do_dvdManagement(CPC * ch, CNetMsg & msg)
{
	CNetMsg rmsg;

	char	subType;
	int		nRate;

	msg >> subType;

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle == NULL || ch == NULL )
	{
		// ERROR
		DVDManagementMsg( rmsg, MSG_MANAGEMENT_ETC );
		rmsg << (unsigned char) MSG_DVD_ERROR_ZONE;
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if( ch->m_guildInfo == NULL )
	{
		// ERROR 길드 없음
		DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM );
		rmsg << (unsigned char) MSG_DVD_ERROR_CONFIRM;
		SEND_Q( rmsg, ch->m_desc );			
		return;
	}

	switch(subType)
	{
	case MSG_MANAGEMENT_MANAGER_CONFIRM :
		{
			if( gserver.m_subno == WAR_CASTLE_SUBNUMBER_DRATAN 
				&& ch->m_guildInfo->guild()->index() == pCastle->GetOwnerGuildIndex() 
				&& ch->m_guildInfo->pos() < 2 )
			{
				// ERROR OK
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM );
				rmsg << (unsigned char) MSG_DVD_ERROR_OK;
				SEND_Q( rmsg, ch->m_desc );				
			}
			else
			{
				// ERROR CONFIRM FAIL
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM );
				rmsg << (unsigned char) MSG_DVD_ERROR_CONFIRM;
				SEND_Q( rmsg, ch->m_desc );				
			}
		}
		break;
	// INFO REQ
	case MSG_MANAGEMENT_ENV_INFO :
		{
			DVDManagementMsg( rmsg, MSG_MANAGEMENT_ENV_INFO );
			rmsg << pCastle->m_dvd.GetEnvRate();  
			SEND_Q( rmsg, ch->m_desc );
		}
		break;
	case MSG_MANAGEMENT_STATE_INFO :
		{
			DVDManagementMsg( rmsg, MSG_MANAGEMENT_STATE_INFO );
			rmsg << pCastle->m_dvd.GetMobRate();
			SEND_Q( rmsg, ch->m_desc );
		}
		break;
	case MSG_MANAGEMENT_TAX_INFO :
		{
			DVDManagementMsg( rmsg, MSG_MANAGEMENT_TAX_INFO );
			rmsg << pCastle->m_dvd.GetFeeRate();
			SEND_Q( rmsg, ch->m_desc );
		}
		break;
	case MSG_MANAGEMENT_HUNTER_TAX_INFO :
		{
			DVDManagementMsg( rmsg, MSG_MANAGEMENT_HUNTER_TAX_INFO );
			rmsg << pCastle->m_dvd.GetHuntRate();
			SEND_Q( rmsg, ch->m_desc );
		}
		break;

	// CHANGE REQ 
	case MSG_MANAGEMENT_ENV_CHANGE :
		{
			// 돈검사 10%당 500000
			msg >> nRate;
			LONGLONG needMoney = nRate/10 * 500000;
			if(	ch->m_moneyItem == NULL ||
				(ch->m_moneyItem !=NULL && ch->m_moneyItem->Count() < needMoney) )
			{
				// ERROR 돈 없음
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_ENV_CHANGE );
				rmsg << (unsigned char) MSG_DVD_ERROR_MONEY;
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			if( ch->m_moneyItem->Count() == needMoney )
			{
				ItemDeleteMsg(rmsg, ch->m_moneyItem);
				ch->AddMoney( -needMoney );
			}
			else
			{
				ch->AddMoney( -needMoney );
				ItemUpdateMsg(rmsg, ch->m_moneyItem, -needMoney);
			}
			SEND_Q(rmsg, ch->m_desc);

			pCastle->m_dvd.ChangeEnvRate(nRate);

			// 헬퍼에 변경 요청 nRate
			HelperDVDRateChangeMsg(rmsg, MSG_MANAGEMENT_ENV_CHANGE , pCastle->m_dvd.GetEnvRate() );
			SEND_Q(rmsg, gserver.m_helper);
			// 변경 완료 메세지
			DVDManagementMsg( rmsg, MSG_MANAGEMENT_ENV_CHANGE );
			rmsg << (unsigned char) MSG_DVD_ERROR_OK;
			SEND_Q( rmsg, ch->m_desc );
			
			GAMELOG << init(" DRATAN DYNAMIC DUNGEON ENVIRONMENT CHANGE " , ch ) << delim
					<< "CHNAGE RATE : " << nRate << delim 
					<< "CURRENT RATE : " << pCastle->m_dvd.GetEnvRate() << delim
					<< "Spend Money : " << needMoney << end;
		}
		break;
	
	case MSG_MANAGEMENT_STATE_CHANGE :
		{
			// 돈검사 10%당 1000000 
			msg >> nRate;
			LONGLONG needMoney = nRate/10 * 1000000;
			if(	ch->m_moneyItem == NULL ||
				(ch->m_moneyItem !=NULL && ch->m_moneyItem->Count() < needMoney) )
			{
				// ERROR 돈 없음
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_ENV_CHANGE );
				rmsg << (unsigned char) MSG_DVD_ERROR_MONEY;
				SEND_Q( rmsg, ch->m_desc );
				return;
			}
			if( ch->m_moneyItem->Count() == needMoney )
			{
				ItemDeleteMsg(rmsg, ch->m_moneyItem);
				ch->AddMoney( -needMoney );
			}
			else
			{
				ch->AddMoney( -needMoney );
				ItemUpdateMsg(rmsg, ch->m_moneyItem, -needMoney);
			}
			SEND_Q(rmsg, ch->m_desc);

			// 헬퍼에 변경 요청 nRate
			pCastle->m_dvd.ChangeMobRate(nRate);
			HelperDVDRateChangeMsg(rmsg, MSG_MANAGEMENT_STATE_CHANGE , pCastle->m_dvd.GetMobRate() );
			SEND_Q(rmsg, gserver.m_helper);

			DVDManagementMsg( rmsg, MSG_MANAGEMENT_STATE_CHANGE );
			rmsg << (unsigned char) MSG_DVD_ERROR_OK;
			SEND_Q( rmsg, ch->m_desc );

			GAMELOG << init(" DRATAN DYNAMIC DUNGEON MONSTER RATE CHANGE " , ch ) << delim
					<< "CHNAGE RATE : " << nRate << delim 
					<< "CURRENT RATE : " << pCastle->m_dvd.GetMobRate() << delim
					<< "Spend Money : " << needMoney << end;
		}
		break;
	
	case MSG_MANAGEMENT_TAX_CHANGE :
		{
			msg >> nRate;

			pCastle->m_dvd.ChangeFeeRate(nRate);
			// 헬퍼에 변경 요청 nRate
			HelperDVDRateChangeMsg(rmsg, MSG_MANAGEMENT_TAX_CHANGE , pCastle->m_dvd.GetFeeRate() );
			SEND_Q(rmsg, gserver.m_helper);

			DVDManagementMsg( rmsg, MSG_MANAGEMENT_TAX_CHANGE );
			rmsg << (unsigned char) MSG_DVD_ERROR_OK;
			SEND_Q( rmsg, ch->m_desc );

			GAMELOG << init(" DRATAN DYNAMIC DUNGEON TAX CHANGE " , ch ) << delim
					<< "CHNAGE RATE : " << nRate << end;

		}
		break;

	case MSG_MANAGEMENT_HUNTER_TAX_CHANGE :
		{
			msg >> nRate;
			pCastle->m_dvd.ChangeHuntRate(nRate);
			// 헬퍼에 변경 요청 nRate
			HelperDVDRateChangeMsg(rmsg, MSG_MANAGEMENT_HUNTER_TAX_CHANGE , pCastle->m_dvd.GetHuntRate() );
			SEND_Q(rmsg, gserver.m_helper);

			DVDManagementMsg( rmsg, MSG_MANAGEMENT_HUNTER_TAX_CHANGE );
			rmsg << (unsigned char) MSG_DVD_ERROR_OK;
			SEND_Q( rmsg, ch->m_desc );

			GAMELOG << init(" DRATAN DYNAMIC DUNGEON HUNTER TAX CHANGE " , ch ) << delim
					<< "CHNAGE RATE : " << nRate << end;
		}
		break;
	}
}

void do_dvdDungenEnter( CPC *ch, CNetMsg& msg )
{
	CNetMsg rmsg;
	LONGLONG needMoney=0;

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle == NULL || 	pCastle->m_dvd.GetZone() == NULL )
	{
		DVDManagementMsg( rmsg, MSG_MANAGEMENT_ETC );
		rmsg << (unsigned char) MSG_DVD_ERROR_ZONE;
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

#ifdef LC_TLD
	time_t curTime;
	time(&curTime);
	int nextTime = pCastle->GetNextWarTime();
	if(curTime + 60 > nextTime)
	{
		GAMELOG << init("DRATAN DYNAMIC DUNGEON TIME ERROR", ch)
				<< "NOW" << delim << (int)curTime << delim
				<< "NEXT WARTIME" << delim << nextTime
				<< end;
		return ;
	}
#endif // LC_TLD

	unsigned char subType;
	msg >> subType;

	if( ch->m_guildInfo != NULL )
	{
		// 성주 길드이면 0원
		if( ch->m_guildInfo->guild()->index() == pCastle->GetOwnerGuildIndex() )
		{	
			needMoney = 0; 
		}
		else
		{	
			needMoney = ( ch->m_level * 1000 ) * 2 * ( pCastle->m_dvd.GetFeeRate() ) / 100 ; 
		}
	}
	else
	{	
		needMoney = ( ch->m_level * 1000 ) * 2 * ( pCastle->m_dvd.GetFeeRate() ) / 100; 
	}

	switch( subType )
	{
	case MSG_DUNGEON_ENTER_FEE :
		{
			DVDDungeonEnterMsg( rmsg, MSG_DUNGEON_ENTER_FEE );
			rmsg <<  needMoney;
			SEND_Q( rmsg, ch->m_desc );
		}		
		break;

	case MSG_DUNGEON_GO :
		{
			CNetMsg		rmsg; 
			if( needMoney > 0 )
			{
				if(	ch->m_moneyItem == NULL ||
					(ch->m_moneyItem !=NULL && ch->m_moneyItem->Count() < needMoney) )
				{
					// ERROR 돈 없음
					DVDDungeonEnterMsg( rmsg, MSG_DUNGEON_GO );
					rmsg << (unsigned char) MSG_DVD_ERROR_MONEY;
					SEND_Q( rmsg, ch->m_desc );
					return;
				}

				if( ch->m_moneyItem->Count() == needMoney )
				{
					ItemDeleteMsg(rmsg, ch->m_moneyItem);
					ch->AddMoney( -needMoney );
				}
				else
				{
					ch->AddMoney( -needMoney );
					ItemUpdateMsg(rmsg, ch->m_moneyItem, -needMoney);
				}
				SEND_Q(rmsg, ch->m_desc);

				

				//세금 지급
				LONGLONG nTax = needMoney - ( ch->m_level * 1000 ); 
				if( nTax > 0 )
					gserver.AddTaxItemDratan( needMoney - ( nTax ) );
			}

			CZone* pZone = gserver.m_zones + gserver.FindZone( ZONE_DRATAN_CASTLE_DUNGEON );
			int extra = 0;
			if( pZone )
			{				
				GoZone(ch, pZone->m_index , 
					pZone->m_zonePos[extra][0],														// ylayer
					GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
					GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
			}
		}
		break;
	}
}

void do_dvdInfo(CPC * ch, CNetMsg & msg)
{
	CNetMsg rmsg;

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle == NULL || 	pCastle->m_dvd.GetZone() == NULL )
	{
		DVDManagementMsg( rmsg, MSG_MANAGEMENT_ETC );
		rmsg << (unsigned char) MSG_DVD_ERROR_ZONE;
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	DVDInfoMsg(ch, rmsg, pCastle->m_dvd.GetEnvRate(), pCastle->m_dvd.GetMobRate() );
	SEND_Q(rmsg, ch->m_desc);
}
#endif // DYNAMIC_DUNGEON
#endif // DRATAN_CASTLE

#ifdef CLIENT_RESTART
void do_ClientRestart( CPC* ch, CNetMsg& msg )
{
	// 커넥터에 임시 말료시간 갱신요청
	CNetMsg rmsg;
	ConnClientRestartMsg( rmsg, ch->m_desc->m_idname );
	SEND_Q( rmsg, gserver.m_connector );

	GAMELOG << init( "CLIENT_RESTART", ch ) << end; 

	// 클라이언트에 restart MSG 전송
	rmsg.Init(MSG_EXTEND);
	rmsg << MSG_EX_RESTART;
	SEND_Q( rmsg, ch->m_desc );
	
	gserver.CloseSocket(ch->m_desc, true);	
}
#endif //CLIENT_RESTART

//#ifdef INIT_SSKILL
void do_ExInitSSkill( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;
	if(!ch->InitSSkill())
	{
		InitSSkillMsg(rmsg, MSG_EX_INIT_SSkill_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	ch->CalcStatus(true);
	ch->SaveNow(false);
	
	SSkillListMsg(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);

	InitSSkillMsg(rmsg, MSG_EX_INIT_SSkill_ERROR_SUCCESS);
	SEND_Q(rmsg, ch->m_desc);

	GAMELOG << init("INIT SSKILL", ch) << end;
}
//#endif // INIT_SSKILL

#ifdef EX_GO_ZONE
void do_ExGoZone(CPC* ch, CNetMsg& msg)
{
	char subtype;

	msg >> subtype;

	switch(subtype)
	{
	case MSG_EX_GO_ZONE_FIND_REQ:			// 찾은 길을 저장할 때 
		{
			
			int npcidx;
			int zone, extra, x, y, z;
			msg >> npcidx;

			// 데이터에 존데이터가 있는지
			CTeleportZoneData* pTelZone = new CTeleportZoneData(npcidx);
			void* pos = gserver.m_listTeleportZone.FindData(pTelZone);
			delete pTelZone;
			if(pos)
			{
				CTeleportZoneData* npcZoneData = gserver.m_listTeleportZone.GetData(pos);
				zone = npcZoneData->GetZoneIndex();
				extra = npcZoneData->GetExtra();
				x = npcZoneData->GetRegenX();
				z = npcZoneData->GetRegenZ();
				y = npcZoneData->GetRegenY();
			}
			else
			{
				return ;
			}
/*
			// 저장하려는 캐릭터하고 성향이 같은지
			if(!ch->CheckNpcAttr(zone, y, x, z))
			{	
				// 성향이 달라서 경로저장 못함.
				return ;
			}
*/
			pos = ch->m_listRegisterNpc.FindData(npcidx);
			if(pos)
			{
				 // 이미 있다 저장할 수 없다.
				 CNetMsg rmsg;
				 ExGoZoneErrorMsg(rmsg, MSG_EX_GO_ZONE_FIND_ERROR_ALREADY_EXIST, npcidx);
				 SEND_Q(rmsg, ch->m_desc);

				 GAMELOG << init("FIND_TELEPORTER_ERROR_ALREADY", ch)
						 << "NPC" << delim
						 << npcidx
						 << end;
			}
			else
			{
				ch->m_listRegisterNpc.AddToTail(npcidx);
				CNetMsg rmsg;
				ExGoZoneErrorMsg(rmsg, MSG_EX_GO_ZONE_FIND_ERROR_OK, npcidx);
				SEND_Q(rmsg, ch->m_desc);
				// 저장성공
				GAMELOG << init("FIND_TELEPORTER_OK")
						<< "NPC" << delim
						<< npcidx
						<< end;
			}
		}
		break;
	case MSG_EX_GO_ZONE_SAVE_REQ:			// 마지막 위치를 저장할 때
		{
			int npcidx;
			CNetMsg rmsg;

			msg >> npcidx;

			// 1. 존데이터에 데이터가 있는지 확인
			// 2. 캐릭터와 성향이 같은지 확인
			// 3. 저장
			CTeleportZoneData* pTelZone = new CTeleportZoneData(npcidx);
			void* pos = gserver.m_listTeleportZone.FindData(pTelZone);
			delete pTelZone;
			if(!pos)
				return ;

			CTeleportZoneData* npcZoneData = gserver.m_listTeleportZone.GetData(pos);
/*
			if(!ch->CheckNpcAttr(npcZoneData->GetZoneIndex(), npcZoneData->GetRegenY(), npcZoneData->GetRegenX(), npcZoneData->GetRegenZ()))
			{
				ExGoZoneErrorMsg(rmsg, MSG_EX_GO_ZONE_SAVE_ERROR_FAILED, npcidx);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("SAVE_LASTEST_TELEPORTER_ERROR_DIFFATTR", ch)
					    << "NPC" << delim
					    << npcidx
					    << end;
				return ;
			}

			// 카오일 때 
			if(ch->IsChaotic())
			{
				ch->m_last_savepos_cao = npcidx;
			}
			// 일반일 때 
			else
			{
				ch->m_last_savepos_gen = npcidx;
			}
*/
			ch->m_lastsavepos = npcidx;
			// 위치 저장 성공
			ExGoZoneErrorMsg(rmsg, MSG_EX_GO_ZONE_SAVE_ERROR_OK, npcidx);
			SEND_Q(rmsg, ch->m_desc);

			GAMELOG << init("SAVE_LASTEST_TELEPORTER_OK", ch)
					<< "NPC" << delim
					<< npcidx
					<< end;
		}
		break;
	case MSG_EX_GO_ZONE_MAP_REQ:			// 맵요청했을 때
		{
			// 캐릭터의 성향과 같은 것만 보낸다.
			int npcidx;
			int tax;
			CNetMsg rmsg;
			CTeleportTaxData* from;
			CTeleportTaxData* to;

			msg >> npcidx;

			CTeleportZoneData* pTelZone = new CTeleportZoneData(npcidx);
			void* pos = gserver.m_listTeleportZone.FindData(pTelZone);
			delete pTelZone;
			if(!pos)
				return;

			CTeleportZoneData* npcZoneData = gserver.m_listTeleportZone.GetData(pos);
			from = gserver.m_TeleportTaxList.FindTeleportTax(npcZoneData->GetZoneIndex(), npcZoneData->GetExtra());
			if(!from)
				return ;
/*
			// 캐릭터와 성향이 같은 개수
			pos = ch->m_listRegisterNpc.GetHead();
			int nSendCount = 0;
			while(pos)
			{
				bool bRet;
				int teleporter = ch->m_listRegisterNpc.GetData(pos);
				pos = ch->m_listRegisterNpc.GetNext(pos);

				CTeleportZoneData* pTelZone = new CTeleportZoneData(teleporter);
				void* pos = gserver.m_listTeleportZone.FindData(pTelZone);
				delete pTelZone;
				if(!pos)
					continue;
				CTeleportZoneData* RememberZoneData = gserver.m_listTeleportZone.GetData(pos);

				bRet = gserver.CheckAttrWithZone(npcZoneData->GetZoneIndex(), npcZoneData->GetRegenY(), npcZoneData->GetRegenX(), npcZoneData->GetRegenZ(),
					RememberZoneData->GetZoneIndex(), RememberZoneData->GetRegenY(), RememberZoneData->GetRegenX(), RememberZoneData->GetRegenZ());
				if(bRet)
					nSendCount++;
			}
*/
			int nSendCount;
			nSendCount = ch->m_listRegisterNpc.GetCount();
			if(nSendCount == 0)
			{
				// 저장된 것이 없다.
				nSendCount = 0;
				rmsg.Init(MSG_EXTEND);
				rmsg << MSG_EX_GO_ZONE
					 << (unsigned char)MSG_EX_GO_ZONE_MAP_REP
					 << nSendCount;				
			}
			else
			{
				int i;
				rmsg.Init(MSG_EXTEND);
				rmsg << MSG_EX_GO_ZONE
					 << (unsigned char)MSG_EX_GO_ZONE_MAP_REP
					 << nSendCount;
				
				i = 0;
				void* pos = ch->m_listRegisterNpc.GetHead();
				while(pos)
				{
					int teleporter = ch->m_listRegisterNpc.GetData(pos);
					pos = ch->m_listRegisterNpc.GetNext(pos);

					CTeleportZoneData* pTelZone = new CTeleportZoneData(teleporter);
					void* pos = gserver.m_listTeleportZone.FindData(pTelZone);
					delete pTelZone;
					if(!pos)
						return ;

					CTeleportZoneData* RememberZoneData = gserver.m_listTeleportZone.GetData(pos);
					if(!RememberZoneData)
						return ;

					to = gserver.m_TeleportTaxList.FindTeleportTax(RememberZoneData->GetZoneIndex(), RememberZoneData->GetExtra());
					if(!to)
						return ;

					tax = 30 * (ch->m_level * (to->GetTax() + from->GetTax()));

					i++;
					rmsg << RememberZoneData->GetNPCIndex()
						 << tax;
				}

				if(i != nSendCount)
					return ;
			}

			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_EX_GO_ZONE_MOVE_REQ:
		{
			int zone, extra, npcIdx;
			int i;
			CNetMsg moneyMsg;
			CNetMsg rmsg;
			msg >> zone
				>> extra
				>> npcIdx;

			// 요금 계산하고 존 이동시킨다
			CTeleportZoneData* pTelZone = new CTeleportZoneData(npcIdx);
			void* pos = gserver.m_listTeleportZone.FindData(pTelZone);
			delete pTelZone;
			if(!pos)
				return;

			CTeleportZoneData* npcZoneData = gserver.m_listTeleportZone.GetData(pos);
			
			CTeleportTaxData* from = gserver.m_TeleportTaxList.FindTeleportTax(npcZoneData->GetZoneIndex(), npcZoneData->GetExtra());
			CTeleportTaxData* to = gserver.m_TeleportTaxList.FindTeleportTax(zone, extra);

			if(!to || !from)
				return ;

			i = gserver.FindZone(zone);
			if(i == -1)
				return ;
			
			int tax;
			tax = 30 * (ch->m_level * (to->GetTax() + from->GetTax()));

			if(!ch->m_moneyItem)
			{
				ExGoZoneErrorMsg(rmsg, MSG_EX_GO_ZONE_MOVE_ERROR_NOMONEY, npcIdx);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("ZONE_MOVE_ERROR_NOMONEY", ch)
						<< "TAX" << delim
						<< tax
						<< end;
				return ;
			}

			if(ch->m_moneyItem->Count() > tax)
			{
				ch->AddMoney(-tax);
				ItemUpdateMsg(moneyMsg, ch->m_moneyItem, -tax);
			}
			else if(ch->m_moneyItem->Count() == tax)
			{
				ItemDeleteMsg(moneyMsg, ch->m_moneyItem);
				ch->AddMoney(-tax);
			}
			else
			{
				// 돈부족
				ExGoZoneErrorMsg(rmsg, MSG_EX_GO_ZONE_MOVE_ERROR_NOMONEY, npcIdx);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("ZONE_MOVE_ERROR_INSUFFICIENT", ch)
						<< "TAX" << delim
						<< tax << delim
						<< "MONEY" << delim
						<< ch->m_moneyItem->Count()
						<< end;

				return ;
			}

			SEND_Q(moneyMsg, ch->m_desc);

			// 이동 성공메시지
			ExGoZoneMoveOKMsg(rmsg, MSG_EX_GO_ZONE_MOVE_ERROR_OK, zone, extra, npcIdx);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_EX_GO_ZONE_FIELD_DOUNGEON_TAX_REQ:
		{
			int npcIdx, zone, extra;
			CNetMsg rmsg;
			msg >> zone >> extra >> npcIdx;
			
			CTeleportTaxData* taxnpc = gserver.m_TeleportTaxList.FindTeleportTax(zone, extra);
			if(!taxnpc)
				return ;
			
			// 이동할 곳이 던전이면 요금계산, 던전아니면 요금계산안함.
			int tax = 0;
			if(taxnpc->IsDungeon())
				tax = ch->m_level * taxnpc->GetTax();

			ExGoZoneFieldDungeonMsg(rmsg, zone, extra, tax);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_EX_GO_ZONE_FIELD_DOUNGEON_MOVE_REQ:
		{
			CNetMsg moneyMsg, rmsg;
			int npcIdx, zone, extra;
			msg >> zone 
				>> extra 
				>> npcIdx;
			
			CTeleportTaxData* taxnpc = gserver.m_TeleportTaxList.FindTeleportTax(zone, extra);
			if(!taxnpc)
				return ;
			
			int movetax = 0;
			if(taxnpc->IsDungeon())
				movetax = ch->m_level * taxnpc->GetTax();
			
			if(ch->m_moneyItem)
			{
				if(ch->m_moneyItem->Count() > movetax)
				{
					ch->AddMoney(-movetax);
					ItemUpdateMsg(moneyMsg, ch->m_moneyItem, -movetax);
				}
				else if (ch->m_moneyItem->Count() == movetax)
				{
					ItemDeleteMsg(moneyMsg, ch->m_moneyItem);
					ch->AddMoney(-movetax);
				}
				else
				{
					// 돈부족
					ExGoZoneErrorMsg(rmsg, MSG_EX_GO_ZONE_MOVE_ERROR_NOMONEY, npcIdx);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}
			else
			{
				// 돈부족
				ExGoZoneErrorMsg(rmsg, MSG_EX_GO_ZONE_MOVE_ERROR_NOMONEY, npcIdx);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			
			SEND_Q(moneyMsg, ch->m_desc);
			
			ExGoZoneMoveOKMsg(rmsg, MSG_EX_GO_ZONE_MOVE_ERROR_OK, zone, extra, npcIdx);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	}
}
#endif // EX_GO_ZONE