#include "stdhdrs.h"
#include "Server.h"
#include "Log.h"
#include "doFunc.h"
#include "CmdMsg.h"

#ifdef PARTY_MATCHING

void do_ExPartyMatch_RegMemberReq(CPC* ch, CNetMsg& msg);
void do_ExPartyMatch_RegPartyReq(CPC* ch, CNetMsg& msg);
void do_ExPartyMatchMemberListReq(CPC* ch, CNetMsg& msg);
void do_ExPartyMatchPartyListReq(CPC* ch, CNetMsg& msg);
void do_ExPartyMatchDelReq(CPC* ch, CNetMsg& msg);
void do_ExPartyMatchInviteReq(CPC* ch, CNetMsg& msg);
void do_ExPartyMatchJoinReq(CPC* ch, CNetMsg& msg);
void do_ExPartyMatchJoinAllow(CPC* ch, CNetMsg& msg);
void do_ExPartyMatchJoinReject(CPC* ch, CNetMsg& msg);

void do_ExPartyMatch(CPC* ch, CNetMsg& msg)
{
	int nSubType = -1;

	msg >> nSubType;

	switch (nSubType)
	{
	case MSG_EX_PARTY_MATCH_REG_MEMBER_REQ:
		do_ExPartyMatch_RegMemberReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_REG_PARTY_REQ:
		do_ExPartyMatch_RegPartyReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_MEMBER_LIST_REQ:
		do_ExPartyMatchMemberListReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_PARTY_LIST_REQ:
		do_ExPartyMatchPartyListReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_DEL_REQ:
		do_ExPartyMatchDelReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_INVITE_REQ:
		do_ExPartyMatchInviteReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_JOIN_REQ:
		do_ExPartyMatchJoinReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_JOIN_ALLOW:
		do_ExPartyMatchJoinAllow(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_JOIN_REJECT:
		do_ExPartyMatchJoinReject(ch, msg);
		break;
	}
}

void do_ExPartyMatch_RegMemberReq(CPC* ch, CNetMsg& msg)
{
	if (ch->m_pZone == NULL)
		return ;

	char nPartyType = -1;

	msg >> nPartyType;

	switch (nPartyType)
	{
	case PARTY_TYPE_RANDOM:
	case PARTY_TYPE_FIRSTGET:
	case PARTY_TYPE_BATTLE:
		break;
	default:
		return ;
	}

	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		HelperPartyMatchRegMemberReqMsg(rmsg, ch->m_index, ch->GetName(), ch->m_level, ch->m_pZone->m_index, ch->m_job, nPartyType);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

void do_ExPartyMatch_RegPartyReq(CPC* ch, CNetMsg& msg)
{
	if (ch->m_pZone == NULL)
		return ;

	CNetMsg			rmsg;

	CLCString		strComment(PARTY_MATCHING_COMMENT + 1);
	int				nJobFlag = 0;
	char			cLimitLevel = 0;

	// 파티중이고 파장
	if (ch->m_party == NULL || ch->m_party->GetBossIndex() != ch->m_index)
	{
		ExPartyMatchRegPartyRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_NO_BOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	msg >> strComment
		>> nJobFlag
		>> cLimitLevel;

	if (nJobFlag == 0)
	{
		nJobFlag = (1 << JOB_TITAN)
				 | (1 << JOB_KNIGHT)
				 | (1 << JOB_HEALER)
				 | (1 << JOB_MAGE)
				 | (1 << JOB_ROGUE)
				 | (1 << JOB_SORCERER);
	}

	if (IS_RUNNING_HELPER)
	{
		HelperPartyMatchRegPartyReqMsg(rmsg, ch->m_index, ch->GetName(), ch->m_level, ch->m_pZone->m_index, nJobFlag, cLimitLevel, strComment);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

void do_ExPartyMatchMemberListReq(CPC* ch, CNetMsg& msg)
{
	int		nPageNo;
	char	cJob;
	char	cLimitLevel;
	char	cPartyType;

	msg >> nPageNo
		>> cJob
		>> cLimitLevel
		>> cPartyType;

	int		nSkipCount = nPageNo * PARTY_MATCHING_DATA_PER_PAGE;
	void*	pos = gserver.m_listPartyMatchMember.GetHead();
	CLCList<CPartyMatchMember*> list(NULL);
	while (pos)
	{
		CPartyMatchMember* pMatchMember = gserver.m_listPartyMatchMember.GetData(pos);
		pos = gserver.m_listPartyMatchMember.GetNext(pos);
		bool bValidCondition = true;

		// 직업이 -1이 아니면 특정 직업
		if (cJob != -1 && pMatchMember->GetJob() != cJob)
			bValidCondition = false;

		// 레벨 제한은 2레벨까지
		if (cLimitLevel && ABS(pMatchMember->GetLevel() - ch->m_level) > PARTY_MATCHING_LIMIT_LEVEL)
			bValidCondition = false;

		// 희망 파티 검사, -1이면 전체
		if (cPartyType != -1 && pMatchMember->GetPartyType() != cPartyType)
			bValidCondition = false;

		// 조건에 맞을 때, page만큼 넘기고 전달
		if (bValidCondition)
		{
			if (nSkipCount > 0)
			{
				nSkipCount--;
			}
			else
			{
				list.AddToTail(pMatchMember);
			}
		}

		if (list.GetCount() == PARTY_MATCHING_DATA_PER_PAGE)
			break;
	}

	CNetMsg rmsg;
	ExPartyMatchMemberListRepMsg(rmsg, nPageNo, ch->m_level, list);
	SEND_Q(rmsg, ch->m_desc);
}

void do_ExPartyMatchPartyListReq(CPC* ch, CNetMsg& msg)
{
	int		nPageNo;
	char	cJob;
	char	cLimitLevel;
	char	cPartyType;

	msg >> nPageNo
		>> cJob
		>> cLimitLevel
		>> cPartyType;

	int		nSkipCount = nPageNo * PARTY_MATCHING_DATA_PER_PAGE;
	void*	pos = gserver.m_listPartyMatchParty.GetHead();
	CLCList<CPartyMatchParty*> list(NULL);
	while (pos)
	{
		CPartyMatchParty* pMatchParty = gserver.m_listPartyMatchParty.GetData(pos);
		pos = gserver.m_listPartyMatchParty.GetNext(pos);
		bool bValidCondition = true;

		// 직업이 -1이 아니면 특정 직업
		if (cJob != -1 && (pMatchParty->GetJobFlag() & (1 << cJob)) == 0)
			bValidCondition = false;

		// 레벨 제한은 2레벨까지
		if (cLimitLevel && ABS(pMatchParty->GetBossLevel() - ch->m_level) > PARTY_MATCHING_LIMIT_LEVEL)
			bValidCondition = false;

		// 희망 파티 검사, -1이면 전체
		if (cPartyType != -1 && pMatchParty->GetPartyType() != cPartyType)
			bValidCondition = false;

		// 조건에 맞을 때, page만큼 넘기고 전달
		if (bValidCondition)
		{
			if (nSkipCount > 0)
			{
				nSkipCount--;
			}
			else
			{
				list.AddToTail(pMatchParty);
			}
		}

		if (list.GetCount() == PARTY_MATCHING_DATA_PER_PAGE)
			break;
	}

	CNetMsg rmsg;
	ExPartyMatchPartyListRepMsg(rmsg, nPageNo, ch->m_level, list);
	SEND_Q(rmsg, ch->m_desc);
}

void do_ExPartyMatchDelReq(CPC* ch, CNetMsg& msg)
{
	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		HelperPartyMatchDelReqMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

void do_ExPartyMatchInviteReq(CPC* ch, CNetMsg& msg)
{
	int			nCharIndex;

	msg >> nCharIndex;

	if (nCharIndex == ch->m_index)
		return ;

	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		HelperPartyMatchInviteReqMsg(rmsg, ch->m_index, ch->GetName(), ch->m_level, nCharIndex);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

void do_ExPartyMatchJoinReq(CPC* ch, CNetMsg& msg)
{
	int			nBossIndex;

	msg >> nBossIndex;

	if (ch->m_party)
		return ;

	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		HelperPartyMatchJoinReqMsg(rmsg, nBossIndex, ch->m_index, ch->GetName(), ch->m_level, ch->m_job);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

void do_ExPartyMatchJoinAllow(CPC* ch, CNetMsg& msg)
{
	int			nReqCharIndex;

	msg >> nReqCharIndex;

	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		HelperPartyMatchJoinAllowReqMsg(rmsg, ch->m_index, nReqCharIndex);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

void do_ExPartyMatchJoinReject(CPC* ch, CNetMsg& msg)
{
	int			nReqCharIndex;

	msg >> nReqCharIndex;

	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		if (nReqCharIndex)
		HelperPartyMatchJoinRejectReqMsg(rmsg, nReqCharIndex, ch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

#endif // PARTY_MATCHING
