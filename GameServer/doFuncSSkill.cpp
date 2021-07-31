#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "DratanCastle.h"

void do_SSkill(CPC* ch, CNetMsg& msg)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(ch);
	}
#endif // DRATAN_CASTLE

	if (DEAD(ch))
		return;

	msg.MoveFirst();

	unsigned char subtype;

	msg >> subtype;

	if (subtype != MSG_SSKILL_LEARN)
		return;

	int index;
	msg >> index;

	CNetMsg errMsg;

	// index가 0이하면 리턴
	if (index <= 0)
	{
		SSkillLearnErrorMsg(errMsg, MSG_SSKILL_LEARN_ERROR_SYSTEM);
		SEND_Q(errMsg, ch->m_desc);
		return;
	}

	CSSkillProto* proto = gserver.m_sSkillProtoList.FindProto(index);

	if (!proto)
	{
		SSkillLearnErrorMsg(errMsg, MSG_SSKILL_LEARN_ERROR_SYSTEM);
		SEND_Q(errMsg, ch->m_desc);
		return;
	}

	// 우선순위 Validation 검사
	CSSkillNode* node = ch->m_sSkillList.m_head;
	while (node)
	{
		// 타입이 같은 스킬이 있고
		if (node->m_sskill->m_proto->m_type == proto->m_type)
		{
			// 있는 스킬보다 낮은 우선순위의 스킬을 배우려 하면 Error
			if (node->m_sskill->m_proto->m_preference != -1 && node->m_sskill->m_proto->m_preference > proto->m_preference)
			{
				SSkillLearnErrorMsg(errMsg, MSG_SSKILL_LEARN_ERROR_SYSTEM);
				SEND_Q(errMsg, ch->m_desc);
				return;
			}
		}
		node = node->m_next;
	}

	CSSkill* sskill = ch->m_sSkillList.Find(index);
	int level = 1;

	if (sskill)
	{
		if (sskill->m_level >= sskill->m_proto->m_maxLevel)
			return;

		level = sskill->m_level + 1;
	}

	// 필요 Level 검사
	if (proto->m_needLevel[level - 1] > ch->m_level)
	{
		SSkillLearnErrorMsg(errMsg, MSG_SSKILL_LEARN_ERROR_LEVEL);
		SEND_Q(errMsg, ch->m_desc);
		return;
	}

	// 필요 SP 검사
	if (proto->m_needSP[level - 1] * 10000 > ch->m_skillPoint)
	{
		SSkillLearnErrorMsg(errMsg, MSG_SSKILL_LEARN_ERROR_SP);
		SEND_Q(errMsg, ch->m_desc);
		return;
	}

	// 필요 SSkill 검사
	if (proto->m_needSSkill != -1 && !sskill)
	{
		CSSkill* check = ch->m_sSkillList.Find(proto->m_needSSkill);

		// 필요 sskill이 아예 없다
		if (!check)
		{
			SSkillLearnErrorMsg(errMsg, MSG_SSKILL_LEARN_ERROR_SSKILL);
			SEND_Q(errMsg, ch->m_desc);
			return;
		}
		//  필요 sskill level이 안된다
		else if (proto->m_needSSkillLevel > check->m_level)
		{
			SSkillLearnErrorMsg(errMsg, MSG_SSKILL_LEARN_ERROR_SSKILL_LEVEL);
			SEND_Q(errMsg, ch->m_desc);
			return;
		}
	}

	// 기존에 같은 타입에 우선순위가 낮은 SSkill 삭제 루틴
	//  [11/6/2008 KwonYongDae] 기존 스킬 삭제 없음 
/*
	CSSkillNode* nodeNext = ch->m_sSkillList.m_head;
	while ((node = nodeNext))
	{
		nodeNext = node->m_next;

		// 우선순위가 없으면 상관없음
		if (node->m_sskill->m_proto->m_preference == -1)
			continue;

		// 타입이 같고 배우려는 스킬보다 우선순위가 낮으면 삭제
		if (node->m_sskill->m_proto->m_type == proto->m_type && node->m_sskill->m_proto->m_preference < proto->m_preference)
		{
			// 삭제 메시지 전송
			CNetMsg removeMsg;
			SSkillRemoveMsg(removeMsg, node->m_sskill);
			SEND_Q(removeMsg, ch->m_desc);

			// 조건 만족 조건이 되었던 스킬 삭제
			if (!ch->m_sSkillList.Remove(node->m_sskill))
			{
				SSkillLearnErrorMsg(errMsg, MSG_SSKILL_LEARN_ERROR_SYSTEM);
				SEND_Q(errMsg, ch->m_desc);
				return;
			}

		}
	}
*/

	// 기존에 스킬이 있었다면 Level Up
	if (sskill)
	{
		// 최고렙 초과
		if (sskill->m_level >= sskill->m_proto->m_maxLevel)
		{
			SSkillLearnErrorMsg(errMsg, MSG_SSKILL_LEARN_ERROR_SYSTEM);
			SEND_Q(errMsg, ch->m_desc);
			return;
		}

		sskill->m_level++;
	}
	else
	{
		sskill = gserver.m_sSkillProtoList.Create(index, 1);
		if (!sskill)
		{
			SSkillLearnErrorMsg(errMsg, MSG_SSKILL_LEARN_ERROR_SYSTEM);
			SEND_Q(errMsg, ch->m_desc);
			return;
		}

		if (!ch->m_sSkillList.Add(sskill))
		{
			SSkillLearnErrorMsg(errMsg, MSG_SSKILL_LEARN_ERROR_SYSTEM);
			SEND_Q(errMsg, ch->m_desc);
			return;
		}
	}

	// SP 소모
	ch->m_skillPoint -= proto->m_needSP[level - 1] * 10000;
	ch->m_bChangeStatus = true;

	// Learn Msg 전송
	CNetMsg learnMsg;
	SSkillLearnMsg(learnMsg, sskill);
	SEND_Q(learnMsg, ch->m_desc);
}