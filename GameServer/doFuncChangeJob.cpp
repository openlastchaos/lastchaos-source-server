#include "stdhdrs.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Log.h"

#ifdef ENABLE_CHANGEJOB
void do_ChangeJob(CPC* pc, CNetMsg& msg)
{
	msg.MoveFirst();

	unsigned char subtype = (unsigned char)-1;

	msg >> subtype;

	bool bRet = false;

	switch (subtype)
	{
	case MSG_CHANGEJOB_REQ:
		bRet = do_ChangeJobReq(pc, msg);
		break;

	case MSG_CHANGEJOB_RESET_REQ:
		bRet = do_ChangeJobResetReq(pc, msg);
		break;

	default:
		return ;
	}

	// 전직 관련 처리를 완료하면 소환 강신 취소
	if (bRet)
	{
		// 소환 취소
		while (pc->m_elementalList)
			pc->UnsummonElemental(pc->m_elementalList);
		// 강신 취소
		if (pc->m_evocationType != EVOCATION_NONE)
			pc->Unevocation();
	}
}

bool do_ChangeJobReq(CPC* pc, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (pc->m_job2 != 0)
	{
		ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_ALREADY);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	if (pc->m_level < JOB_2ND_LEVEL)
	{
		ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_NOCONDITION_LEVEL);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	char job2 = 0;

	msg >> job2;

	CSkill* skill = NULL;
	int needskilllevel = 5;

	// 스킬 구하기 : 필요 스킬 레벨이 5가 아니면 수정 필요
	switch (pc->m_job)
	{
	case JOB_TITAN:
		switch (job2)
		{
		case JOB_2ND_HIGHLANDER:
		case JOB_2ND_WARMASTER:
			skill = pc->m_activeSkillList.Find(43);
			break;

		default:
			ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
			SEND_Q(rmsg, pc->m_desc);
			return false;
		}
		break;

	case JOB_KNIGHT:
		switch (job2)
		{
		case JOB_2ND_ROYALKNIGHT:
		case JOB_2ND_TEMPLEKNIGHT:
			skill = pc->m_activeSkillList.Find(48);
			break;

		default:
			ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
			SEND_Q(rmsg, pc->m_desc);
			return false;
		}
		break;

	case JOB_HEALER:
		switch (job2)
		{
		case JOB_2ND_ARCHER:
		case JOB_2ND_CLERIC:
			skill = pc->m_activeSkillList.Find(52);
			break;

		default:
			ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
			SEND_Q(rmsg, pc->m_desc);
			return false;
		}
		break;

	case JOB_MAGE:
		switch (job2)
		{
		case JOB_2ND_WIZARD:
		case JOB_2ND_WITCH:
			skill = pc->m_activeSkillList.Find(65);
			break;

		default:
			ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
			SEND_Q(rmsg, pc->m_desc);
			return false;
		}
		break;

	case JOB_ROGUE:
		switch (job2)
		{
		case JOB_2ND_ASSASSIN:
		case JOB_2ND_RANGER:
			skill = pc->m_activeSkillList.Find(131);
			break;

		default:
			ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
			SEND_Q(rmsg, pc->m_desc);
			return false;
		}
		break;

	case JOB_SORCERER:
		switch (job2)
		{
		case JOB_2ND_ELEMENTALLIST:
		case JOB_2ND_SPECIALLIST:
			skill = pc->m_activeSkillList.Find(308);
			break;

		default:
			ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
			SEND_Q(rmsg, pc->m_desc);
			return false;
		}
		break;

	default:
		return false;
	}
	// --- 스킬 구하기

	if (skill == NULL)
	{
		ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_NOCONDITION_SKILL);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}
	if (skill->m_level < needskilllevel)
	{
		ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_NOCONDITION_SKILL);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	int needmoney = 50000;

	if (pc->m_moneyItem == NULL || pc->m_moneyItem->Count() < needmoney)
	{
		ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_NOMONEY);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}
	else if (pc->m_moneyItem->Count() == needmoney)
	{
		ItemDeleteMsg(rmsg, pc->m_moneyItem);
		SEND_Q(rmsg, pc->m_desc);
		pc->AddMoney(-needmoney);
	}
	else
	{
		pc->AddMoney(-needmoney);
		ItemUpdateMsg(rmsg, pc->m_moneyItem, -needmoney);
		SEND_Q(rmsg, pc->m_desc);
	}

	GAMELOG << init("CHANGE JOB", pc)
			<< "2ND JOB" << delim
			<< job2 << delim
			<< "ORIGINAL STATPOINT" << delim
			<< pc->m_statpt_str << delim
			<< pc->m_statpt_dex << delim
			<< pc->m_statpt_int << delim
			<< pc->m_statpt_con << delim
			<< pc->m_statpt_remain << delim;

	pc->m_job2 = job2;

	// 스탯 재계산
	// 스탯 포인트 초기화
	pc->m_statpt_str = 0;
	pc->m_statpt_dex = 0;
	pc->m_statpt_int = 0;
	pc->m_statpt_con = 0;
	pc->m_statpt_remain = (pc->m_level - JOB_2ND_LEVEL) * 3 + (pc->m_level - JOB_2ND_LEVEL) / 2 + (JOB_2ND_LEVEL - 1);
#ifdef	EVENT_PHOENIX 
	if ( pc->m_bPhoenix_Char == 1 )
	{
		// 피닉스로 전직하면서 추가된 30 스텟 포인트 재적립
		pc->m_statpt_remain += 30;
	}
#endif // EVENT_PHOENIX

	GAMELOG << "RESET STATPOINT" << delim
			<< pc->m_statpt_str << delim
			<< pc->m_statpt_dex << delim
			<< pc->m_statpt_int << delim
			<< pc->m_statpt_con << delim
			<< pc->m_statpt_remain << end;

	pc->InitStat();
	pc->CalcStatus(true);

	pc->m_hp = pc->m_maxHP;
	pc->m_mp = pc->m_maxMP;

	pc->SaveNow(false);

	StatPointRemainMsg(rmsg, pc);
	SEND_Q(rmsg, pc->m_desc);

	ChangeJobRepMsg(rmsg, pc->m_index, pc->m_job, pc->m_job2);
	pc->m_pArea->SendToCell(rmsg, pc, true);
	if (pc->IsParty())
	{
		HelperPartyMemberChangeJobMsg(rmsg, pc->m_party->GetBossIndex(), pc->m_index, pc->m_job, pc->m_job2);
		if (IS_RUNNING_HELPER)
			SEND_Q(rmsg, gserver.m_helper);
	}

	return true;
}

bool do_ChangeJobResetReq(CPC* pc, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (pc->m_job2 == 0)
	{
		ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_WRONGMSG);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	LONGLONG need = 0;

//#if !defined(LC_JPN)
	int statptall = pc->m_statpt_str + pc->m_statpt_dex + pc->m_statpt_int + pc->m_statpt_con;

	need = 25000;			// 필요 금액 단위
	need *= statptall;
	need *= pc->m_level;
	need /= JOB_2ND_LEVEL;
	need += 100000;


	if (pc->m_moneyItem == NULL || pc->m_moneyItem->Count() < need)
	{
		ChangeJobErrorMsg(rmsg, MSG_CHANGEJOB_ERROR_NOMONEY);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}
	else if (pc->m_moneyItem->Count() == need)
	{
		ItemDeleteMsg(rmsg, pc->m_moneyItem);
		SEND_Q(rmsg, pc->m_desc);
		pc->AddMoney(-need);
	}
	else
	{
		pc->AddMoney(-need);
		ItemUpdateMsg(rmsg, pc->m_moneyItem, -need);
		SEND_Q(rmsg, pc->m_desc);
	}
//#endif

	GAMELOG << init("CHANGE JOB", pc)
			<< "2ND JOB" << delim
			<< pc->m_job2 << delim
			<< "NEED MONEY" << delim
			<< need << delim
			<< "ORIGINAL STATPOINT" << delim
			<< pc->m_statpt_str << delim
			<< pc->m_statpt_dex << delim
			<< pc->m_statpt_int << delim
			<< pc->m_statpt_con << delim
			<< pc->m_statpt_remain << end;

	// 초기화
	// 직업
	pc->m_job2 = 0;

	// 스킬
	int skillpoint = 0;
	skillpoint += pc->m_activeSkillList.RemoveSkillFor2ndJob();
	skillpoint += pc->m_passiveSkillList.RemoveSkillFor2ndJob();
	skillpoint += pc->m_etcSkillList.RemoveSkillFor2ndJob();
//#ifdef LC_JPN	// 일본 스킬 포인트 환원
//	pc->m_skillPoint += skillpoint;
//#endif //LC_JPN

	// 스탯 포인트
	pc->m_statpt_str = 0;
	pc->m_statpt_dex = 0;
	pc->m_statpt_int = 0;
	pc->m_statpt_con = 0;
	pc->m_statpt_remain = pc->m_level - 1;

	pc->InitStat();

	pc->CalcStatus(true);

	pc->m_hp = pc->m_maxHP;
	pc->m_mp = pc->m_maxMP;

	pc->SaveNow(false);

	StatPointRemainMsg(rmsg, pc);
	SEND_Q(rmsg, pc->m_desc);

	ChangeJobResetRepMsg(rmsg, pc);
	pc->m_pArea->SendToCell(rmsg, pc, true);

	SkillListMsg(rmsg, pc);
	SEND_Q(rmsg, pc->m_desc);

	if (pc->IsParty())
	{
		HelperPartyMemberChangeJobMsg(rmsg, pc->m_party->GetBossIndex(), pc->m_index, pc->m_job, pc->m_job2);
		if (IS_RUNNING_HELPER)
			SEND_Q(rmsg, gserver.m_helper);
	}

	return true;
}

#endif