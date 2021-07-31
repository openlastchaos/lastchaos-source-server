#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"

void do_PK(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype;

	msg.MoveFirst();

	msg >> subtype;

	switch (subtype)
	{
//	case MSG_PK_RECOVER_HPMP:
//		do_PKRecoverHPMP(ch, msg);
//		break;
	case MSG_PK_RECOVER_ITEMSEAL:
		do_PKRecoverItemSealed(ch, msg);
		break;
	}
}

// TODO : DELETE
//void do_PKRecoverHPMP(CPC* ch, CNetMsg& msg)
//{
//	CNetMsg rmsg;
//	char hp, mp;
//
//	msg >> hp >> mp;
//
//	if (hp)
//	{
//		if (ch->m_pkPenaltyHP)
//		{
//			// µ· °Ë»ç
//			LONGLONG needmoney = (LONGLONG)100 * ch->m_pkPenaltyHP * ch->m_level;
//			if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < needmoney)
//			{
//				PKErrorMsg(rmsg, 0);
//				SEND_Q(rmsg, ch->m_desc);
//				return ;
//			}
//			if (ch->m_moneyItem->Count() == needmoney)
//			{
//				ItemDeleteMsg(rmsg, ch->m_moneyItem);
//				SEND_Q(rmsg, ch->m_desc);
//				ch->AddMoney(-needmoney);
//			}
//			else
//			{
//				ch->AddMoney(-needmoney);
//				ItemUpdateMsg(rmsg, ch->m_moneyItem, -needmoney);
//				SEND_Q(rmsg, ch->m_desc);
//			}
//
//			ch->m_pkPenaltyHP = 0;
//			PKRecoverHPMPMsg(rmsg, true, false);
//			SEND_Q(rmsg, ch->m_desc);
//			PKPenaltyHPMPMsg(rmsg, ch);
//			SEND_Q(rmsg, ch->m_desc);
//			ch->CalcStatus(true);
//		}
//		else
//		{
//			PKErrorMsg(rmsg, 1);
//			SEND_Q(rmsg, ch->m_desc);
//		}
//	}
//	else if (mp)
//	{
//		if (ch->m_pkPenaltyMP)
//		{
//			// µ· °Ë»ç
//			LONGLONG needmoney = (LONGLONG)100 * ch->m_pkPenaltyHP * ch->m_level;
//			if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < needmoney)
//			{
//				PKErrorMsg(rmsg, 0);
//				SEND_Q(rmsg, ch->m_desc);
//				return ;
//			}
//			if (ch->m_moneyItem->Count() == needmoney)
//			{
//				ItemDeleteMsg(rmsg, ch->m_moneyItem);
//				SEND_Q(rmsg, ch->m_desc);
//				ch->AddMoney(-needmoney);
//			}
//			else
//			{
//				ch->AddMoney(-needmoney);
//				ItemUpdateMsg(rmsg, ch->m_moneyItem, -needmoney);
//				SEND_Q(rmsg, ch->m_desc);
//			}
//
//			ch->m_pkPenaltyMP = 0;
//			PKRecoverHPMPMsg(rmsg, false, true);
//			SEND_Q(rmsg, ch->m_desc);
//			PKPenaltyHPMPMsg(rmsg, ch);
//			SEND_Q(rmsg, ch->m_desc);
//			ch->CalcStatus(true);
//		}
//		else
//		{
//			PKErrorMsg(rmsg, 2);
//			SEND_Q(rmsg, ch->m_desc);
//		}
//	}
//}

void do_PKRecoverItemSealed(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	char tab, row, col;
	int itemindex;
	msg >> tab >> row >> col >> itemindex;

	CInventory* inven = GET_INVENTORY(ch, tab);
	if (inven != NULL)
	{
		CItem* item = inven->GetItem(row, col);
		if (item != NULL)
		{
			if (item->m_index == itemindex) // && item->m_wearPos != WEARING_NONE)
			{
				if (item->m_flag & FLAG_ITEM_SEALED)
				{
					// µ· °Ë»ç
#ifdef NEW_PK
					LONGLONG needmoney  = (LONGLONG)100
										* item->GetItemLevel(true)
										* item->GetItemLevel(true);
#else
					LONGLONG needmoney  = (LONGLONG)10
										* item->GetItemLevel(true)
										* item->GetItemLevel(true);
#endif
					switch (item->m_itemProto->m_typeIdx)
					{
					case ITYPE_WEAPON:
						needmoney *= 2;
						break;
					case ITYPE_WEAR:
						switch (item->m_itemProto->m_subtypeIdx)
						{
						case IWEAR_HELMET:
						case IWEAR_GLOVE:
						case IWEAR_SHOES:
							break;
						case IWEAR_ARMOR:
						case IWEAR_PANTS:
							needmoney = needmoney * 3 / 2;
							break;
						case IWEAR_SHIELD:
							needmoney *= 2;
						}
						break;
					}
					if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < needmoney)
					{
						PKErrorMsg(rmsg, 0);
						SEND_Q(rmsg, ch->m_desc);
						return ;
					}
					if (ch->m_moneyItem->Count() == needmoney)
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

					item->m_flag &= ~FLAG_ITEM_SEALED;
					ItemUpdateMsg(rmsg, item, 0);
					SEND_Q(rmsg, ch->m_desc);
					PKRecoverItemSealMsg(rmsg, item);
					SEND_Q(rmsg, ch->m_desc);
					ch->CalcStatus(true);

					return ;
				}
			}
		}
	}

	PKErrorMsg(rmsg, 3);
	SEND_Q(rmsg, ch->m_desc);
}
