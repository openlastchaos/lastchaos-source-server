#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"

#ifdef FACTORY_SYSTEM

void do_Factory(CPC* ch, CNetMsg& msg)
{
//	if (DEAD(ch) || ch->IsSetPlayerState(PLAYER_STATE_SITDOWN))
//		return ;
	
	msg.MoveFirst();
	
	unsigned char subtype;
	msg >> subtype;
	
	switch (subtype)
	{
	case MSG_FACTORY_LEARN:
		do_FactoryLearn(ch, msg);
		break;
	case MSG_FACTORY_ITEM_LIST:
		do_FactoryList(ch, msg);
		break;
	case MSG_FACTORY_ITEM_MAKE:
		do_FactoryMake(ch, msg);
	}
}

void do_FactoryLearn(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	try
	{
		unsigned char subtype;
		int nIndex;
		
		msg.MoveFirst();
		
		msg >> subtype
			>> nIndex;
		
		CFactoryProto * pFactory = gserver.m_factoryItemProtoList.Find(nIndex);
		if (!pFactory)
			throw MSG_FACTORY_ERROR_SYSTEM;

		// 연합 등록 여부
		if (!ch->m_sealSkillList.Find(FACTORY_UNION_REG_SKILL))
			throw MSG_FACTORY_ERROR_SYSTEM;

		// 이미 배웠으면 더 배울 필요없다.
		if (ch->m_listFactory.Find(pFactory))
			throw MSG_FACTORY_ERROR_ALREADY_REG;

		// 나스
		if (ch->m_moneyItem->Count() < pFactory->GetNas())
			throw MSG_FACTORY_ERROR_NAS;

		// 숙련도
		if (ch->GetSealExp(pFactory->GetSealType()) < pFactory->GetNeedExp())
			throw MSG_FACTORY_ERROR_EXP;

		// 나스 소모
		int r, c;
		CItem * pMoney = ch->m_moneyItem;
		if (ch->m_invenNormal.FindItem(&r, &c, pMoney->m_idNum, pMoney->m_plus, pMoney->m_flag))
		{
			CItem* p = ch->m_invenNormal.GetItem(r, c);
			if (p)
			{
				DecreaseFromInventory(ch, pMoney, pFactory->GetNas());
				if (pMoney->Count() < 1)
				{
					ItemDeleteMsg(rmsg, pMoney);
					SEND_Q(rmsg, ch->m_desc);
					RemoveFromInventory(ch, pMoney, true, true);
				}
				else
				{
					ItemUpdateMsg(rmsg, pMoney, -pFactory->GetNas());
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}

		// 추가
		ch->m_listFactory.Add(pFactory);
		GAMELOG << init("FACTORY ITEM LEARN", ch) << pFactory->GetIndex() << end;
	
		FactoryLearnMsg(rmsg, pFactory);
		SEND_Q(rmsg, ch->m_desc);
	}
	catch (MSG_FACTORY_ERROR_TYPE learnerr)
	{
		FactoryErrorMsg(rmsg, learnerr);
		SEND_Q(rmsg, ch->m_desc);
	}
}


void do_FactoryList(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype;
	int nSealType;

	msg.MoveFirst();
		
	msg >> subtype
		>> nSealType;

	CNetMsg rmsg;
	FactoryListMsg(rmsg, ch, nSealType);
	SEND_Q(rmsg, ch->m_desc);
}

void do_FactoryMake(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	unsigned char subtype;
	int nIndex;

	msg.MoveFirst();
		
	msg >> subtype
		>> nIndex;

	try
	{
		CFactoryProto * pFactory = gserver.m_factoryItemProtoList.Find(nIndex);
		if (!pFactory)
			throw MSG_FACTORY_ERROR_SYSTEM;

		// 스킬 있냐?
		//pFactory->GetSealType()
		//ch->FindSkill()

		// 재료 체크
		int i;
		CItem* item[MAX_FACTORY_ITEM_STUFF];
		for (i = 0; i < MAX_FACTORY_ITEM_STUFF; i++)
		{
			item[i] = NULL;
			if (!pFactory->GetStuff(i))
				continue ;
			int r, c;
			if (!ch->m_invenNormal.FindItem(&r, &c, pFactory->GetStuff(i)->nItemIdx, 0, 0))
				throw MSG_FACTORY_ERROR_ITEM;
			item[i] = ch->m_invenNormal.GetItem(r, c);
			if (!item[i] || item[i]->Count() < pFactory->GetStuff(i)->nCount)
				throw MSG_FACTORY_ERROR_ITEM;
		}

		// 인벤토리에 빈칸이 있는지
		int r, c;
		if(!ch->m_invenNormal.FindEmpty(&r, &c))
			throw MSG_FACTORY_ERROR_INVEN;

		// 제작 아이템 추가
		bool bCountable = false;
		CNetMsg itemMsg;

		CItem * pMakeItem = gserver.m_itemProtoList.CreateItem(pFactory->GetItemIdx(), WEARING_NONE, 0, 0, 1);
		if (AddToInventory(ch, pMakeItem, false, true, r, c))
		{
			// 겹쳐졌는지 검사
			if (pMakeItem->tab() == -1)
			{
				// 수량 변경 알림
				int r, c;
				if (ch->m_invenNormal.FindItem(&r, &c, pMakeItem->m_idNum, pMakeItem->m_plus, pMakeItem->m_flag))
				{
					CItem* p = ch->m_invenNormal.GetItem(r, c);
					if (p)
						ItemUpdateMsg(itemMsg, p, pMakeItem->Count());
				}
			}
			else
			{
				ItemAddMsg(itemMsg, pMakeItem);
			}
			SEND_Q(itemMsg, ch->m_desc);
		}
		else
		{
			delete pMakeItem;
			throw MSG_FACTORY_ERROR_SYSTEM;
		}

		if (bCountable)
			delete pMakeItem;


		// 나스 소모
		CItem * pMoney = ch->m_moneyItem;
		if (ch->m_invenNormal.FindItem(&r, &c, pMoney->m_idNum, pMoney->m_plus, pMoney->m_flag))
		{
			CItem* p = ch->m_invenNormal.GetItem(r, c);
			if (p)
			{
				DecreaseFromInventory(ch, pMoney, pFactory->GetNas());
				if (pMoney->Count() < 1)
				{
					ItemDeleteMsg(rmsg, pMoney);
					SEND_Q(rmsg, ch->m_desc);
					RemoveFromInventory(ch, pMoney, true, true);
				}
				else
				{
					ItemUpdateMsg(rmsg, pMoney, -pFactory->GetNas());
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}

		// 아이템 소모
		for (i = 0; i < MAX_FACTORY_ITEM_STUFF; i++)
		{
			if (!pFactory->GetStuff(i))
				continue;

			if (item[i] && pFactory->GetStuff(i)->nItemIdx > 0)
			{
				DecreaseFromInventory(ch, item[i], pFactory->GetStuff(i)->nCount);
				if (item[i]->Count() < 1)
				{
					ItemDeleteMsg(rmsg, item[i]);
					SEND_Q(rmsg, ch->m_desc);
					RemoveFromInventory(ch, item[i], true, true);
				}
				else
				{
					ItemUpdateMsg(rmsg, item[i], -pFactory->GetStuff(i)->nCount);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}

		// 숙련도 증가
		LONGLONG llCharExp = ch->GetSealExp(pFactory->GetSealType());
		LONGLONG llMakeExp = pFactory->GetMakeExp();
		LONGLONG llPlusExp = 0;
		 	 if (llCharExp		 == llMakeExp || (llCharExp -  9) < llMakeExp)	llPlusExp = llMakeExp;
		else if ((llCharExp - 10) > llMakeExp || (llCharExp - 19) < llMakeExp)	llPlusExp = llMakeExp * 0.8;
		else if ((llCharExp - 20) > llMakeExp || (llCharExp - 29) < llMakeExp)	llPlusExp = llMakeExp * 0.5;
		else if ((llCharExp - 30) > llMakeExp) llPlusExp = 0;

		llCharExp += llPlusExp;
		ch->SetSealExp(pFactory->GetSealType(), llCharExp);

		FactoryMakeMsg(rmsg, ch, pFactory);
		SEND_Q(rmsg, ch->m_desc);

		SysMsg(rmsg, MSG_SYS_FACTORY_EXP);
		rmsg << llPlusExp;
		SEND_Q(rmsg, ch->m_desc);

	}
	catch (MSG_FACTORY_ERROR_TYPE learnerr)
	{
		FactoryErrorMsg(rmsg, learnerr);
		SEND_Q(rmsg, ch->m_desc);
	}
}

#endif // FACTORY_SYSTEM