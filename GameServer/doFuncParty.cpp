#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"

/////////////////
// 파티 관련 함수
void do_Party(CPC* ch, CNetMsg& msg)
{
	// 안보이면 무시
	if (!ch->m_bVisible)
		return ;

	unsigned char subtype;

	msg.MoveFirst();

	msg >> subtype;

	switch (subtype)
	{
	case MSG_PARTY_INVITE:
		do_PartyInvite(ch, msg);
		break;
	case MSG_PARTY_ALLOW:
		do_PartyAllow(ch, msg);
		break;
	case MSG_PARTY_REJECT:
		do_PartyReject(ch, msg);
		break;
	case MSG_PARTY_QUIT:
		do_PartyQuit(ch, msg);
		break;
	case MSG_PARTY_KICK:
		do_PartyKick(ch, msg);
		break;
	case MSG_PARTY_ASSISTINFO:
		do_PartyAssistInfo(ch, msg);
		break;
	case MSG_PARTY_CHANGEBOSS:
		do_PartyChangeBoss(ch, msg);
		break;
#ifdef NEW_UI
	case MSG_PARTY_CHANGETYPE:
		do_PartyChangeType(ch, msg);
		break;
#endif // NEW_UI

#ifdef PARTY_RAID
	case MSG_PARTY_ENDPARTY_REQ:
		do_PartyEndPartyReq(ch, msg);
		break;		
	case MSG_PARTY_INZONE_CLEAR_REQ:
		do_PartyInzoneClearReq(ch, msg);
		break;
#endif //PARTY_RAID
	}
}

void do_PartyInvite(CPC* ch, CNetMsg& msg)
{
	char partyType;
	int destindex;
	
	msg >> partyType >> destindex;

	GAMELOG << init("PARTY DEBUG INVITE", ch)
			<< partyType << delim
			<< destindex
			<< end;

	CNetMsg rmsg;

	if (ch->m_index == destindex)
		return ;

// << kjban 080110 공성에 참가한 경우
	CPC * pPC = gserver.m_playerList.Find(destindex);
	if(pPC == NULL)
	{
		return;
	}

#ifdef ENABLE_WAR_CASTLE
	// 같은 편인 경우 만 파티 가능
	switch(ch->GetJoinFlag(ZONE_MERAC))
	{
	case WCJF_ATTACK_GUILD:
	case WCJF_ATTACK_CHAR:
		if(pPC->GetJoinFlag(ZONE_MERAC) == WCJF_DEFENSE_GUILD
			|| pPC->GetJoinFlag(ZONE_MERAC) == WCJF_OWNER
			|| pPC->GetJoinFlag(ZONE_MERAC) == WCJF_DEFENSE_CHAR)
		{
			return;
		}
		break;

	case WCJF_DEFENSE_GUILD:
	case WCJF_OWNER:
	case WCJF_DEFENSE_CHAR:
		if(pPC->GetJoinFlag(ZONE_MERAC) == WCJF_ATTACK_GUILD
			|| pPC->GetJoinFlag(ZONE_MERAC) == WCJF_ATTACK_CHAR)
		{
			return;
		}
		break;
	}
#endif // ENABLE_WAR_CASTLE

#ifdef DRATAN_CASTLE
// 같은 편인 경우 만 파티 가능
	switch(ch->GetJoinFlag(ZONE_DRATAN))
	{
	case WCJF_ATTACK_GUILD:
	case WCJF_ATTACK_CHAR:
		if(pPC->GetJoinFlag(ZONE_DRATAN) == WCJF_DEFENSE_GUILD
			|| pPC->GetJoinFlag(ZONE_DRATAN) == WCJF_OWNER
			|| pPC->GetJoinFlag(ZONE_DRATAN) == WCJF_DEFENSE_CHAR)
		{
			return;
		}
		break;

	case WCJF_DEFENSE_GUILD:
	case WCJF_OWNER:
	case WCJF_DEFENSE_CHAR:
		if(pPC->GetJoinFlag(ZONE_DRATAN) == WCJF_ATTACK_GUILD
			|| pPC->GetJoinFlag(ZONE_DRATAN) == WCJF_ATTACK_CHAR)
		{
			return;
		}
		break;
	}
#endif // DRATAN_CASTLE
// >>

	if (IS_RUNNING_HELPER)
	{
		HelperPartyInviteReqMsg(rmsg, ch->m_index, ch->GetName(), ch->m_level, destindex, partyType);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

void do_PartyAllow(CPC* ch, CNetMsg& /* msg */)
{
	// 파티 정보가 있고
	CNetMsg rmsg;

	GAMELOG << init("PARTY DEBUG ALLOW", ch)
			<< end;

	if (ch->m_party == NULL)
	{
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_PARTY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	// 내가 요청 받은 사람이면
	if (ch->m_party->GetRequestIndex() != ch->m_index)
		return ;

	if (IS_RUNNING_HELPER)
	{
		HelperPartyAllowReqMsg(rmsg, ch->m_party->GetBossIndex(), ch->m_index, ch->GetName());
		SEND_Q(rmsg, gserver.m_helper);
//		ch->m_desc->WaitHelperReply(true);
	}
}

void do_PartyReject(CPC* ch, CNetMsg& /* msg */)
{
	CNetMsg rmsg;

	GAMELOG << init("PARTY DEBUG REJECT", ch)
			<< end;

	// 파티 정보가 있고
	if (ch->m_party == NULL)
	{
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_PARTY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	if( ch->m_party->GetRequestIndex() < 1 )
		return;

	if (IS_RUNNING_HELPER)
	{
		HelperPartyRejectReqMsg(rmsg, ch->m_party->GetBossIndex(), ch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
//		ch->m_desc->WaitHelperReply(true);
	}
}

void do_PartyQuit(CPC* ch, CNetMsg& /* msg */)
{
	GAMELOG << init("PARTY DEBUG QUIT", ch)
			<< end;

	// 파티 중이고
	if (ch->m_party == NULL)
		return ;
	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		HelperPartyQuitReqMsg(rmsg, ch->m_party->GetBossIndex(), ch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

void do_PartyKick(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int index;
	msg >> index;

	GAMELOG << init("PARTY DEBUG KICK", ch)
			<< index
			<< end;

	// 파티 중이고
	if (ch->m_party == NULL)
	{
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_PARTY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 보스이고
	if (ch->m_party->GetBossIndex() != ch->m_index)
	{
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_BOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 자신이 아니고
	if (ch->m_index == index)
		return ;

	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		HelperPartyKickReqMsg(rmsg, ch->m_party->GetBossIndex(), index);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

void do_PartyAssistInfo(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	if (ch->m_party == NULL)
	{
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_PARTY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int tindex;
	msg >> tindex;

	int nListIndex = ch->m_party->FindMember(tindex);
	if (nListIndex == -1)
		return ;

	PartyAssistInfoMsg(rmsg, ch->m_party->GetMemberByListIndex(nListIndex)->GetMemberPCPtr());
	SEND_Q(rmsg, ch->m_desc);
}

void do_PartyChangeBoss(CPC* ch, CNetMsg& msg)
{
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	msg >> name;

	GAMELOG << init("PARTY DEBUG CHANGEBOSS", ch)
			<< name
			<< end;

	CNetMsg rmsg;

	if (ch->m_party == NULL)
	{
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_PARTY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_party->GetBossIndex() != ch->m_index)
	{
		// 파티장이 아님 메세지 보내고
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_BOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperPartyChangeBossReqMsg(rmsg, ch->m_index, name);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

#ifdef NEW_UI
void do_PartyChangeType(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	GAMELOG << init("PARTY DEBUG CHNAGETYPE", ch)
			<< end;

	char cPartyType,cDiviType=MSG_DIVITYPE_EXP;

	msg >> cPartyType
		>> cDiviType;

	if (ch->m_party == NULL)
	{
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_PARTY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_party->GetBossIndex() != ch->m_index)
	{
		// 파티장이 아님 메세지 보내고
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_BOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(ch->m_party->GetPartyType(cDiviType) == cPartyType)
		return ;

	// 전투형 파티
	if(cPartyType == PARTY_TYPE_BATTLE)
	{
		int bosslevel;
		bosslevel = ch->m_level;

		int i;
		for(i = 0; i < MAX_PARTY_MEMBER; ++i)
		{
			const CPartyMember* member = ch->m_party->GetMemberByListIndex(i);
			if(member && member->GetMemberPCPtr() && (member->GetMemberPCPtr()->m_index != ch->m_index) )
			{
				if(ABS(member->GetMemberPCPtr()->m_level - bosslevel) > 10)
				{
					PartyErrorMsg(rmsg, MSG_PARTY_ERROR_INVALID_LEVEL);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}
		}
	}

	if (IS_RUNNING_HELPER)
	{
		HelperPartyChangeTypeReqMsg(rmsg, ch->m_index, cPartyType,cDiviType);
		SEND_Q(rmsg, gserver.m_helper);
	}
}
#endif // NEW_UI

#ifdef PARTY_RAID
void do_PartyEndPartyReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (ch->m_party == NULL)
	{
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_PARTY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_party->GetBossIndex() != ch->m_index)
	{
		// 파티장이 아님 메세지 보내고
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_BOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperPartyEndPartyReqMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

void do_PartyInzoneClearReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (ch->m_party == NULL)
	{
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_PARTY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_party->GetBossIndex() != ch->m_index)
	{
		// 파티장이 아님 메세지 보내고
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_NOT_BOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}


}
#endif //PARTY_RAID