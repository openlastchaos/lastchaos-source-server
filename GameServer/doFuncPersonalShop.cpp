#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"



void do_PersonalShop(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
	unsigned char subtype = (unsigned char)-1;
	msg >> subtype;
	switch (subtype)
	{
	case MSG_PERSONALSHOP_SELL_START:
		do_PersonalShopSellStart(ch, msg);
		break;

	case MSG_PERSONALSHOP_SELL_LIST:
		do_PersonalShopSellList(ch, msg);
		break;

	case MSG_PERSONALSHOP_CHANGE:
		do_PersonalShopChange(ch, msg);
		break;

	case MSG_PERSONALSHOP_BUY:
		do_PersonalShopBuy(ch, msg);
		break;
	}
}

void do_PersonalShopSellStart(CPC* ch, CNetMsg& msg)
{
	if (ch->m_pArea == NULL)
		return ;
	if (ch->m_moneyItem == NULL && ch->m_invenNormal.GetSpace() < 1)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOSPACE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->m_pArea->GetAttr(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)) != MAPATT_PEACEZONE)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_AREA);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	
	// 브라질 요청 업데이트, OX 퀴즈룸에선 개인상점 금지...yhj 090227
	if ( ch->m_pZone->IsOXQuizRoom() )
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_AREA);
		SEND_Q(rmsg, ch->m_desc);
		return;	
	}

#ifdef LC_TWN2
	// 대만은 레벨 4 이하는 개인상점 불가
	if ( ch->m_level <= 4 )
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif

#ifdef LC_USA
	if ( ch->m_level < 10 )
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif // LC_USA
	
#ifdef LC_GER
	if ( ch->m_level < 15 )
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif

	if (ch->m_exchange)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->IsSetPlayerState(PLAYER_STATE_WARP))
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->m_currentSkill)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_SKILL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->m_personalshop)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int charindex = -1;
	char shoptype = -1;
	CLCString shopname(PS_MAX_SHOPNAME + 1);
	msg >> charindex
		>> shoptype
		>> shopname;
	shopname.Trim();
	if (charindex < 1 || shoptype < 0 || (shoptype & PST_SELL) == 0 || shopname.Length() > PS_MAX_SHOPNAME)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
//	if (shoptype & PST_PREMIUM)
//	{
//		if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < PS_NEED_MONEY_PREMIUM)
//		{
//			CNetMsg rmsg;
//			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_CANTPREMIUM);
//			SEND_Q(rmsg, ch->m_desc);
//			return ;
//		}
//	}

	char normalCount = -1;
	msg >> normalCount;
	if (normalCount < 0 || normalCount > PS_MAX_NORMAL_ITEM)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (normalCount == 0 && (shoptype & PST_PACKAGE) == 0)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CPersonalShop* ps = new CPersonalShop(shoptype, shopname);

	int i;
	for (i = 0; i < normalCount; i++)
	{
		char row = -1, col = -1;
		int itemindex = -1;
		int itemdbindex = -1;
		LONGLONG count = -1;
		LONGLONG price = -1;
		msg >> row
			>> col
			>> itemindex
			>> itemdbindex
			>> count
			>> price;
		if (itemdbindex == gserver.m_itemProtoList.m_moneyItem->m_index)
		{
			delete ps;

			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		// 060406 : bs : 봉인된 아이템 교환 불가
		CItem* item = ch->m_invenNormal.GetItem(row, col);
		if (
				   item == NULL
				|| item->m_index != itemindex
				|| item->m_idNum != itemdbindex
				|| item->Count() < count
				|| price < 1
				|| !item->CanExchange()
			)
		{
			delete ps;

			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
#ifdef ENABLE_PET
		// 펫일 경우 스킬을 하나라도 배운 것은 교환 불가
		if (item->IsPet())
		{
			CPet* pet = ch->GetPet(item->m_plus);
			if (!pet || pet->GetSkillList()->count() != 0 )
			{
				delete ps;

				CNetMsg rmsg;
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
#endif
#ifdef ATTACK_PET
		if( item->IsAPet() )
		{
			CAPet* apet = ch->GetAPet(item->m_plus);
			if( apet && apet->IsPetEquip() )
			{
				delete ps;

				CNetMsg rmsg;
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
#endif //ATTACK_PET
		if (ps->FindItem(true, itemindex) != -1)
		{
			delete ps;

			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_DUPLICATION);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (!ps->AddItem(true, itemindex, count, price))
		{
			delete ps;

			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	if (shoptype & PST_PACKAGE)
	{
		LONGLONG packprice = -1;
		char packcount = -1;
		msg >> packprice
			>> packcount;
		if (packprice < 1 || packcount < 1 || packcount > PS_MAX_PACKAGE_ITEM)
		{
			delete ps;

			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		int i;
		for (i = 0; i < packcount; i++)
		{
			char row = -1, col = -1;
			int itemindex = -1;
			int itemdbindex = -1;
			LONGLONG count = -1;
			msg >> row
				>> col
				>> itemindex
				>> itemdbindex
				>> count;
			if (itemdbindex == gserver.m_itemProtoList.m_moneyItem->m_index)
			{
				delete ps;

				CNetMsg rmsg;
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_MONEY);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			// 060406 : bs : 봉인된 아이템 교환 불가
			CItem* item = ch->m_invenNormal.GetItem(row, col);
			if (
					   item == NULL
					|| item->m_index != itemindex
					|| item->m_idNum != itemdbindex
					|| item->Count() < count
					|| !item->CanExchange()
				)
			{
				delete ps;

				CNetMsg rmsg;
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			if (ps->FindItem(true, itemindex) != -1)
			{
				delete ps;

				CNetMsg rmsg;
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_DUPLICATION);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			if (ps->FindItem(false, itemindex) != -1)
			{
				delete ps;

				CNetMsg rmsg;
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_DUPLICATION);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
#ifdef ENABLE_PET
			// 펫일 경우 스킬을 하나라도 배운 것은 교환 불가
			if (item->IsPet())
			{
				CPet* pet = ch->GetPet(item->m_plus);
				if (!pet || pet->GetSkillList()->count() != 0 )
				{
					delete ps;

					CNetMsg rmsg;
					PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}
#endif
#ifdef ATTACK_PET
			if( item->IsAPet() )
			{
				CAPet* apet = ch->GetAPet(item->m_plus);
				if( apet && apet->IsPetEquip() )
				{
					delete ps;

					CNetMsg rmsg;
					PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}
#endif //ATTACK_PET

			if (!ps->AddItem(false, itemindex, count, packprice))
			{
				delete ps;

				CNetMsg rmsg;
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
	}

//	if (ps->type() & PST_PREMIUM)
//	{
//		CNetMsg rmsg;
//		if (ch->m_moneyItem->Count() > PS_NEED_MONEY_PREMIUM)
//		{
//			ch->AddMoney(-PS_NEED_MONEY_PREMIUM);
//			ItemUpdateMsg(rmsg, ch->m_moneyItem, -PS_NEED_MONEY_PREMIUM);
//			SEND_Q(rmsg, ch->m_desc);
//		}
//		else
//		{
//			ItemDeleteMsg(rmsg, ch->m_moneyItem);
//			SEND_Q(rmsg, ch->m_desc);
//			ch->AddMoney(-PS_NEED_MONEY_PREMIUM);
//		}
//	}

	ch->m_personalshop = ps;

	CNetMsg rmsg;
	PersonalShopSellStartMsg(rmsg, ch);
	ch->m_pArea->SendToCell(rmsg, ch, true);

	GAMELOG << init("PERSONAL SHOP START", ch)
			<< "TYPE" << delim << ps->GetType() << delim << "ITEMS";
	int pos = -1;
	for (i = 0; i < ps->GetNormalCount(); i++)
	{
		pos = ps->GetNextNormalItem(pos);
		int itemindex = ps->GetNormalItemIndex(pos);
		CItem* item = ch->m_invenNormal.GetItem(itemindex);
		GAMELOG << delim << itemlog(item)
				<< delim << ps->GetNormalItemCount(pos)
				<< delim << ps->GetNormalItemPrice(pos);
	}
	if (ps->GetType() & PST_PACKAGE)
	{
		GAMELOG << delim << "PACKAGE"
				<< delim << ps->GetPackagePrice();

		pos = -1;
		for (i = 0; i < ps->GetPackageCount(); i++)
		{
			pos = ps->GetNextPackageItem(pos);
			int itemindex = ps->GetPackageItemIndex(pos);
			CItem* item = ch->m_invenNormal.GetItem(itemindex);
			GAMELOG << delim << itemlog(item)
					<< delim << ps->GetPackageItemCount(pos);
		}
	}
	GAMELOG << end;
}

void do_PersonalShopSellList(CPC* ch, CNetMsg& msg)
{
	if (ch->m_pArea == NULL)
		return ;
	if (ch->m_personalshop)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int charindex = -1;
	msg >> charindex;
	CPC* tch = TO_PC(ch->m_pArea->FindCharInCell(ch, charindex, MSG_CHAR_PC, false));
	if (tch == NULL)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (tch->m_personalshop == NULL || (tch->m_personalshop->GetType() & PST_SELL) == 0)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTSELL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CNetMsg rmsg;
#ifdef ENABLE_PET
	// 일반 아이템에서 펫이면 펫 정보 보내기
	if (tch->m_personalshop->GetNormalCount())
	{
		int i;
		int pos = -1;
		for (i = 0; i < tch->m_personalshop->GetNormalCount(); i++)
		{
			pos = tch->m_personalshop->GetNextNormalItem(pos);
			if (pos != -1)
			{
				int itemindex = tch->m_personalshop->GetNormalItemIndex(pos);
				int r, c;
				if (tch->m_invenNormal.FindItem(&r, &c, itemindex))
				{
					CItem* pitem = tch->m_invenNormal.GetItem(r, c);
					if (pitem)
					{
						if (pitem->IsPet())
						{
							CPet* pet = tch->GetPet(pitem->m_plus);
							if (pet)
							{
								ExPetSellInfoMsg(rmsg, pet);
								SEND_Q(rmsg, ch->m_desc);
							}
						}
					}
				}
			}
		}
	}
	// 패키지 아이템에서 펫이면 펫 정보 보내기
	if (tch->m_personalshop->GetType() & PST_PACKAGE)
	{
		int i;
		int pos = -1;
		for (i = 0; i < tch->m_personalshop->GetPackageCount(); i++)
		{
			pos = tch->m_personalshop->GetNextPackageItem(pos);
			if (pos != -1)
			{
				int itemindex = tch->m_personalshop->GetPackageItemIndex(pos);
				int r, c;
				if (tch->m_invenNormal.FindItem(&r, &c, itemindex))
				{
					CItem* pitem = tch->m_invenNormal.GetItem(r, c);
					if (pitem)
					{
						if (pitem->IsPet())
						{
							CPet* pet = tch->GetPet(pitem->m_plus);
							if (pet)
							{
								ExPetSellInfoMsg(rmsg, pet);
								SEND_Q(rmsg, ch->m_desc);
							}
						}
					}
				}
			}
		}
	}
#endif
	PersonalShopSellListMsg(rmsg, tch);
	SEND_Q(rmsg, ch->m_desc);
}

void do_PersonalShopChange(CPC* ch, CNetMsg& msg)
{
	if (ch->m_pArea == NULL)
		return ;
	if (ch->m_exchange)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->IsSetPlayerState(PLAYER_STATE_WARP))
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->m_currentSkill)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_SKILL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int charindex = -1;
	char shoptype = -1;
	msg >> charindex
		>> shoptype;

	if (shoptype != PST_NOSHOP)
		return ;
	if (ch->m_personalshop == NULL)
		return ;
	if (ch->m_index != charindex)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	delete ch->m_personalshop;
	ch->m_personalshop = NULL;
	CNetMsg rmsg;
	PersonalShopChangeMsg(rmsg, ch);
	ch->m_pArea->SendToCell(rmsg, ch, true);
}

void do_PersonalShopBuy(CPC* ch, CNetMsg& msg)
{
	if (ch->m_pArea == NULL)
		return ;
	if (ch->m_personalshop)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 구매시 필요한 나스
	LONGLONG needmoney = 0;

	// 상점 찾기
	int charindex = -1;
	char packbuy = -1;
	msg >> charindex
		>> packbuy;
	CPC* tch = TO_PC(ch->m_pArea->FindCharInCell(ch, charindex, MSG_CHAR_PC, false));
	if (tch == NULL)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (tch->m_personalshop == NULL || (tch->m_personalshop->GetType() & PST_SELL) == 0)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTSELL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CPersonalShop* ps = tch->m_personalshop;
	char shoptype = ps->GetType();

	int nCountOfPet = 0;
#ifdef ENABLE_PET 
	if (ch->m_petList)
	{
		CPet* pPetOfBuyer = ch->m_petList;
		while (pPetOfBuyer)
		{
			nCountOfPet++;
			pPetOfBuyer = pPetOfBuyer->m_nextPet;
		}
	}
#endif // ENABLE_PET

	// 구매 리스트
	CPersonalShop list(shoptype, "");

	// 패키지 검사
	if (packbuy)
	{
		if ((shoptype & PST_PACKAGE) == 0)
		{
			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTPACKAGE);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		needmoney = ps->GetPackagePrice();
		if (needmoney < 0 || ch->m_moneyItem == NULL || needmoney > ch->m_moneyItem->Count())
		{
			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 패키지 구매 리스트에 추가
		int i, j = -1;
		for (i = 0; i < ps->GetPackageCount(); i++)
		{
			j = ps->GetNextPackageItem(j);
			if (j == -1)
			{
				CNetMsg rmsg;
				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
#ifdef ENABLE_PET 
			CItem* item = tch->m_invenNormal.GetItem(ps->GetPackageItemIndex(j));
			if(item == NULL)
				return ;
			if (item->IsPet())
				nCountOfPet++;
#endif
			list.AddItem(false, ps->GetPackageItemIndex(j), ps->GetPackageItemCount(j), ps->GetPackagePrice());
		}
	}

	// 일반 상품 검사
	char normalcount = -1;
	msg >> normalcount;
	if (normalcount == 0 && !packbuy)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (normalcount < 0 || normalcount > ps->GetNormalCount())
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int i;
	for (i = 0; i < normalcount; i++)
	{
		int itemindex = -1;
		LONGLONG count = -1;
		msg >> itemindex
			>> count;

		int pos = ps->FindItem(true, itemindex);
		if (pos == -1)
		{
			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (count < 1)
		{
			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_COUNT);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (ps->GetNormalItemCount(pos) < count)
		{
			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_COUNT);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
#ifdef ENABLE_PET 
		CItem* item = tch->m_invenNormal.GetItem(itemindex);
		if(item == NULL)
			return ;

		if (item->IsPet())
			nCountOfPet++;
#endif
		needmoney += ps->GetNormalItemPrice(pos) * count;
		if (needmoney < 0 || ch->m_moneyItem == NULL || needmoney > ch->m_moneyItem->Count())
		{
			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		// 같은 아이템 여러번 사려고 하면 ?		if (list.FindItem(true, itemindex) != -1 || list.FindItem(false, itemindex) != -1)
		{
			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			GAMELOG << init("HACK PERSONAL SHOP BUY", ch) << end;
			return ;
		}
		list.AddItem(true, ps->GetNormalItemIndex(pos), count, ps->GetNormalItemPrice(pos));
	}

	if (nCountOfPet > MAX_OWN_PET)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_ANYMORE_PET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int needweight = 0;
	int needspace = 0;
	int pos;
	pos = list.GetNextNormalItem(-1);
	while (pos != -1)
	{
		// 파는 아이템 검사
		int itemindex = list.GetNormalItemIndex(pos);
		CItem* pitem = tch->m_invenNormal.GetItem(itemindex);
		if (!pitem)
		{
			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (pitem->Count() < list.GetNormalItemCount(pos))
		{
			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		needweight += pitem->m_itemProto->m_weight * list.GetNormalItemCount(pos);

		// 인벤토리 가능 검사
		int r, c;
		if (!(pitem->m_itemProto->m_flag & ITEM_FLAG_COUNT) || !ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag))
			needspace++;

		pos = list.GetNextNormalItem(pos);
	}
	pos = list.GetNextPackageItem(-1);
	while (pos != -1)
	{
		// 파는 아이템 검사
		int itemindex = list.GetPackageItemIndex(pos);
		CItem* pitem = tch->m_invenNormal.GetItem(itemindex);
		if (!pitem)
		{
			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (pitem->Count() < list.GetPackageItemCount(pos))
		{
			CNetMsg rmsg;
			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		needweight += pitem->m_itemProto->m_weight * list.GetPackageItemCount(pos);

		// 인벤토리 가능 검사
		int r, c;
		if (!(pitem->m_itemProto->m_flag & ITEM_FLAG_COUNT) || !ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag))
			needspace++;

		pos = list.GetNextPackageItem(pos);
	}

	// 인벤토리 검사
	if (ch->m_invenNormal.GetSpace() < needspace || ch->m_weight + needweight >= ch->m_maxWeight * 15 / 10)
	{
		CNetMsg rmsg;
		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_BUY_FULLINVENTORY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("PERSONAL SHOP BUY", ch);
	GAMELOG << "SHOP" << delim << tch->m_name << delim
			<< tch->m_nick << delim
			<< tch->m_desc->m_idname << delim;
	if (packbuy)
		GAMELOG << "PACKAGE" << delim;
	else
		GAMELOG << "NO PACKAGE" << delim;
	GAMELOG << "NEEDMONEY" << delim << needmoney << delim;
	GAMELOG << "ITEMS";

	// 일반 아이템 구매/판매
	pos = list.GetNextNormalItem(-1);
	while (pos != -1)
	{
		int itemindex = list.GetNormalItemIndex(pos);
		LONGLONG count = list.GetNormalItemCount(pos);
		CItem* pitem = tch->m_invenNormal.GetItem(itemindex);
		pitem->m_wearPos = WEARING_NONE;
		int r, c;
		CNetMsg rmsg;
		CItem* nitem;

#ifdef ENABLE_PET
		if (pitem->IsPet())
		{
			CPet* pet = tch->GetPet(pitem->m_plus);
			if (pet)
			{
				REMOVE_FROM_BILIST(pet, tch->m_petList, m_prevPet, m_nextPet);
				pet->SetOwner(ch);
				ADD_TO_BILIST(pet, ch->m_petList, m_prevPet, m_nextPet);
				ExPetStatusMsg(rmsg, pet);
				SEND_Q(rmsg, ch->m_desc);
				ExPetSkillListMsg(rmsg, pet);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
#endif
#ifdef ATTACK_PET
		if( pitem->IsAPet() )
		{
			CAPet* apet = tch->GetAPet(pitem->m_plus);
			if( apet )
			{
				REMOVE_FROM_BILIST(apet, tch->m_pApetlist, m_pPrevPet, m_pNextPet);
				apet->SetOwner(ch);
				apet->AddFaith(-10);
				ADD_TO_BILIST(apet, ch->m_pApetlist, m_pPrevPet, m_pNextPet);
			}

		}
#endif //ATTACK_PET

		// 구매 리스트의 카운트보다 크면
		if (pitem->Count() > count)
		{
			// 겹치는 아이템이 판매 후에도 남는 경우
			// 만들고
			nitem = gserver.m_itemProtoList.CreateItem(pitem->m_idNum, -1, pitem->m_plus, pitem->m_flag, count);
			// 빼고
			DecreaseFromInventory(tch, pitem, count);
			// 일리고
			ItemUpdateMsg(rmsg, pitem, -count);
			SEND_Q(rmsg, tch->m_desc);
			// 넣고
			AddToInventory(ch, nitem, false, true);
			// 알리고
			if (nitem->tab() < 0)
			{
				// 겹쳤다
				delete nitem;
				ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag);
				nitem = ch->m_invenNormal.GetItem(r, c);
				ItemUpdateMsg(rmsg, nitem, count);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				// 안겹쳤다
				ItemAddMsg(rmsg, nitem);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		else
		{
			// 겹치든 안 겹치든 모두 옮기는 경우
			nitem = pitem;
			// 알리고
			ItemDeleteMsg(rmsg, pitem);
			SEND_Q(rmsg, tch->m_desc);
			// 빼고
			RemoveFromInventory(tch, pitem, false, true);
			// 넣고
			AddToInventory(ch, nitem, false, true);
			// 알리고
			if (nitem->tab() < 0)
			{
				// 겹쳤다
				delete nitem;
				ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag);
				nitem = ch->m_invenNormal.GetItem(r, c);
				ItemUpdateMsg(rmsg, nitem, count);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				// 안겹쳤다
				ItemAddMsg(rmsg, nitem);
				SEND_Q(rmsg, ch->m_desc);
			}
		}

		GAMELOG << delim << itemlog(nitem)
				<< delim << count;

		// 판매 리스트 갱신
		ps->RemoveNormalItem(itemindex, count);

		pos = list.GetNextNormalItem(pos);
	}

	// 패키지 아이템 구매 판매
	pos = list.GetNextPackageItem(-1);
	while (pos != -1)
	{
		int itemindex = list.GetPackageItemIndex(pos);
		LONGLONG count = list.GetPackageItemCount(pos);
		CItem* pitem = tch->m_invenNormal.GetItem(itemindex);
		int r, c;
		CNetMsg rmsg;
		CItem* nitem;

#ifdef ENABLE_PET
		if (pitem->IsPet())
		{
			CPet* pet = tch->GetPet(pitem->m_plus);
			if (pet)
			{
				REMOVE_FROM_BILIST(pet, tch->m_petList, m_prevPet, m_nextPet);
				pet->SetOwner(ch);
				ADD_TO_BILIST(pet, ch->m_petList, m_prevPet, m_nextPet);
				ExPetStatusMsg(rmsg, pet);
				SEND_Q(rmsg, ch->m_desc);
				ExPetSkillListMsg(rmsg, pet);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
#endif
#ifdef ATTACK_PET
		if( pitem->IsAPet() )
		{
			CAPet* apet = tch->GetAPet(pitem->m_plus);
			if( apet )
			{
				REMOVE_FROM_BILIST(apet, tch->m_pApetlist, m_pPrevPet, m_pNextPet);
				apet->SetOwner(ch);
				apet->AddFaith(-10);
				ADD_TO_BILIST(apet, ch->m_pApetlist, m_pPrevPet, m_pNextPet);
			}

		}
#endif //ATTACK_PET
		// 구매 리스트의 카운트보다 크면
		if (pitem->Count() > count)
		{
			// 겹치는 아이템이 판매 후에도 남는 경우
			// 만들고
			nitem = gserver.m_itemProtoList.CreateItem(pitem->m_idNum, -1, pitem->m_plus, pitem->m_flag, count);
			// 빼고
			DecreaseFromInventory(tch, pitem, count);
			// 일리고
			ItemUpdateMsg(rmsg, pitem, -count);
			SEND_Q(rmsg, tch->m_desc);
			// 넣고
			AddToInventory(ch, nitem, false, true);
			// 알리고
			if (nitem->tab() < 0)
			{
				// 겹쳤다
				delete nitem;
				ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag);
				nitem = ch->m_invenNormal.GetItem(r, c);
				ItemUpdateMsg(rmsg, nitem, count);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				// 안겹쳤다
				ItemAddMsg(rmsg, nitem);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		else
		{
			// 겹치든 안 겹치든 모두 옮기는 경우
			nitem = pitem;
			// 알리고
			ItemDeleteMsg(rmsg, pitem);
			SEND_Q(rmsg, tch->m_desc);
			// 빼고
			RemoveFromInventory(tch, pitem, false, true);
			// 넣고
			AddToInventory(ch, nitem, false, true);
			// 알리고
			if (nitem->tab() < 0)
			{
				// 겹쳤다
				delete nitem;
				ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag);
				nitem = ch->m_invenNormal.GetItem(r, c);
				ItemUpdateMsg(rmsg, nitem, count);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				// 안겹쳤다
				ItemAddMsg(rmsg, nitem);
				SEND_Q(rmsg, ch->m_desc);
			}
		}

		GAMELOG << delim << itemlog(nitem)
				<< delim << count;

		// 판매 리스트 갱신
		ps->RemovePackage();

		pos = list.GetNextPackageItem(pos);
	}

	GAMELOG << end;

	// 돈 넣고 빼기
	if (ch->m_moneyItem->Count() > needmoney)
	{
		CNetMsg rmsg;
		ch->AddMoney(-needmoney);
		ItemUpdateMsg(rmsg, ch->m_moneyItem, -needmoney);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		CNetMsg rmsg;
		ItemDeleteMsg(rmsg, ch->m_moneyItem);
		ch->AddMoney(-needmoney);
		SEND_Q(rmsg, ch->m_desc);
	}
	int rate = (shoptype & (PST_PACKAGE | PST_PREMIUM)) ? PS_TEX_PREMIUM : PS_TEX_NORMAL;
	if (tch->m_moneyItem)
	{
		CNetMsg rmsg;
		tch->AddMoney(needmoney - (needmoney * rate / 10000));
		ItemUpdateMsg(rmsg, tch->m_moneyItem, needmoney - (needmoney * rate / 10000));
		SEND_Q(rmsg, tch->m_desc);
	}
	else
	{
		CNetMsg rmsg;
		if (tch->AddMoney(needmoney - (needmoney * rate / 10000)))
		{
			ItemAddMsg(rmsg, tch->m_moneyItem);
			SEND_Q(rmsg, tch->m_desc);
		}
		else
		{
			GAMELOG << init("PERSONAL SHOP ERROR : CANT GIVE MONEY TO SHOP", tch)
					<< "MONEY" << delim
					<< (needmoney - (needmoney * rate / 10000))
					<< end;
		}
	}

	// 판매자에게 판매됨 알림
	CNetMsg rmsg;
	PersonalShopBuyMsg(rmsg, msg, ch);
	SEND_Q(rmsg, tch->m_desc);

	// 상점 종료 및 상태 변화 검사
	// 패키지 상태 검사
	if (ps->GetType() != shoptype)
	{
		CNetMsg rmsg;
		PersonalShopChangeMsg(rmsg, tch);
		tch->m_pArea->SendToCell(rmsg, tch, true);
		if (ps->GetType() == PST_NOSHOP)
		{
			delete ps;
			tch->m_personalshop = NULL;
		}
	}
	ch->SaveNow(false);
	tch->SaveNow(false);
}


//
//#ifdef ALTERNATE_MERCHANT
//void do_AlternateMerchantSellStart(CPC* ch, CNetMsg& msg)
//{
//	/*
//	처리 사항 
//	 1. user의 지역 검사
//	 2. 현재 지역이 공성지역 인지 검사
//	 3. 현재 대리 상점이 존재하는지 검사
//	 4.  회수할 아이템이 있는지 검사
//	   - helper 로 전송( charindex, shoptype, shopname )
//	*/
//
//	// user의 지역 검사
//	if( ch->m_pArea == NULL )
//		return;
//
//	// 현재 지역이 공성 지역인지 검사
//	if (ch->m_pArea->GetAttr(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)) != MAPATT_WARZONE )
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_AREA); // 전송 메세지 정의
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//	
//	if (ch->m_exchange)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//	if (ch->IsSetPlayerState(PLAYER_STATE_WARP))
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//	if (ch->m_currentSkill)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_SKILL);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//
//	int charindex = -1;
//	char shoptype = -1;
//	CLCString shopname(PS_MAX_SHOPNAME + 1);
//	msg >> charindex
//		>> shoptype
//		>> shopname;
//	shopname.Trim();
//	if (charindex < 1 || shoptype < 0 || (shoptype & PST_SELL) == 0 || shopname.Length() > PS_MAX_SHOPNAME)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//
//
//	char normalCount = -1;
//	msg >> normalCount;
//	if (normalCount < 0 || normalCount > PS_MAX_NORMAL_ITEM)
//	{
//		CNetMsg rmsg;
//		//PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//
//	if (normalCount == 0 && (shoptype & PST_PACKAGE) == 0)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//
//	//CPersonalShop* ps = new CPersonalShop(shoptype, shopname);
//
//	int i;
//	for (i = 0; i < normalCount; i++)
//	{
//		char row = -1, col = -1;
//		int itemindex = -1;
//		int itemdbindex = -1;
//		LONGLONG count = -1;
//		LONGLONG price = -1;
//		msg >> row
//			>> col
//			>> itemindex
//			>> itemdbindex
//			>> count
//			>> price;
//		if (itemdbindex == gserver.m_itemProtoList.m_moneyItem->m_index)
//		{
//			//delete ps;
//
//			CNetMsg rmsg;
//			//PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_MONEY);
//			SEND_Q(rmsg, ch->m_desc);
//			return ;
//		}
//		// 060406 : bs : 봉인된 아이템 교환 불가
//		CItem* item = ch->m_invenNormal.GetItem(row, col);
//		if (
//				   item == NULL
//				|| item->m_index != itemindex
//				|| item->m_idNum != itemdbindex
//				|| item->Count() < count
//				|| price < 1
//				|| !item->CanExchange()
//			)
//		{
////			delete ps;
//
//			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
//			SEND_Q(rmsg, ch->m_desc);
//			return ;
//		}
//#ifdef ENABLE_PET
//		// 펫일 경우 스킬을 하나라도 배운 것은 교환 불가
//		if (item->IsPet())
//		{
//			CPet* pet = ch->GetPet(item->m_plus);
//			if (!pet || pet->GetSkillList()->count() != 0 )
//			{
////				delete ps;
////
//				CNetMsg rmsg;
////				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
//				SEND_Q(rmsg, ch->m_desc);
//				return ;
//			}
//		}
//#endif
////		if (ps->FindItem(true, itemindex) != -1)
////		{
////			delete ps;
//
////			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_DUPLICATION);
////			SEND_Q(rmsg, ch->m_desc);
////			return ;
////		}
////		if (!ps->AddItem(true, itemindex, count, price))
////		{
////			delete ps;
////
////			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
////			SEND_Q(rmsg, ch->m_desc);
////			return ;
////		}
//	}
//
//	if (shoptype & PST_PACKAGE)
//	{
//		LONGLONG packprice = -1;
//		char packcount = -1;
//		msg >> packprice
//			>> packcount;
//		if (packprice < 1 || packcount < 1 || packcount > PS_MAX_PACKAGE_ITEM)
//		{
////			delete ps;
////
//			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
//			SEND_Q(rmsg, ch->m_desc);
//			return ;
//		}
//		int i;
//		for (i = 0; i < packcount; i++)
//		{
//			char row = -1, col = -1;
//			int itemindex = -1;
//			int itemdbindex = -1;
//			LONGLONG count = -1;
//			msg >> row
//				>> col
//				>> itemindex
//				>> itemdbindex
//				>> count;
//			if (itemdbindex == gserver.m_itemProtoList.m_moneyItem->m_index)
//			{
////				delete ps;
//
//				CNetMsg rmsg;
////				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_MONEY);
//				SEND_Q(rmsg, ch->m_desc);
//				return ;
//			}
//			// 060406 : bs : 봉인된 아이템 교환 불가
//			CItem* item = ch->m_invenNormal.GetItem(row, col);
//			if (
//					   item == NULL
//					|| item->m_index != itemindex
//					|| item->m_idNum != itemdbindex
//					|| item->Count() < count
//					|| !item->CanExchange()
//				)
//			{
////				delete ps;
////
//				CNetMsg rmsg;
////				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
//				SEND_Q(rmsg, ch->m_desc);
//				return ;
//			}
////			if (ps->FindItem(true, itemindex) != -1)
////			{
////				delete ps;
////
////				CNetMsg rmsg;
////				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_DUPLICATION);
////				SEND_Q(rmsg, ch->m_desc);
////				return ;
////			}
////			if (ps->FindItem(false, itemindex) != -1)
////			{
////				delete ps;
////
////				CNetMsg rmsg;
////				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_DUPLICATION);
////				SEND_Q(rmsg, ch->m_desc);
////				return ;
////			}
//#ifdef ENABLE_PET
//			// 펫일 경우 스킬을 하나라도 배운 것은 교환 불가
//			if (item->IsPet())
//			{
//				CPet* pet = ch->GetPet(item->m_plus);
//				if (!pet || pet->GetSkillList()->count() != 0 )
//				{
////					delete ps;
//
//					CNetMsg rmsg;
////					PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
//					SEND_Q(rmsg, ch->m_desc);
//					return ;
//				}
//			}
//#endif
////			if (!ps->AddItem(false, itemindex, count, packprice))
////			{
////				delete ps;
////
////				CNetMsg rmsg;
////				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
////				SEND_Q(rmsg, ch->m_desc);
////				return ;
////			}
//		}
//	}
//
////	ch->m_personalshop = ps;
//
//	CNetMsg rmsg;
////	PersonalShopSellStartMsg(rmsg, ch);
//	ch->m_pArea->SendToCell(rmsg, ch, true);
//
////	GAMELOG << init("PERSONAL SHOP START", ch)
////			<< "TYPE" << delim << ps->GetType() << delim << "ITEMS";
//	int pos = -1;
////	for (i = 0; i < ps->GetNormalCount(); i++)
////	{
////		pos = ps->GetNextNormalItem(pos);
////		int itemindex = ps->GetNormalItemIndex(pos);
////		CItem* item = ch->m_invenNormal.GetItem(itemindex);
////		GAMELOG << delim << itemlog(item)
////				<< delim << ps->GetNormalItemCount(pos)
////				<< delim << ps->GetNormalItemPrice(pos);
////	}
////	if (ps->GetType() & PST_PACKAGE)
////	{
////		GAMELOG << delim << "PACKAGE"
////				<< delim << ps->GetPackagePrice();
//
//		pos = -1;
////		for (i = 0; i < ps->GetPackageCount(); i++)
////		{
////			pos = ps->GetNextPackageItem(pos);
////			int itemindex = ps->GetPackageItemIndex(pos);
////			CItem* item = ch->m_invenNormal.GetItem(itemindex);
////			GAMELOG << delim << itemlog(item)
////					<< delim << ps->GetPackageItemCount(pos);
////		}
////	}
////	GAMELOG << end;
//}
//
//void do_AlternateMerchantSellList(CPC* ch, CNetMsg& msg)
//{
//	/*
//	1. 현재 대리 상점이 존재 하는지 검사
//	2. 상품 리스트 전송
//	*/
//	// 현재 대리 상점이 존재 하는지 검사
////	CAlternateMerchant* pAlternateMerchant = gserver.m_playerList.Find( charindex );
////	if( pAlternateMerchant == NULL )
////	{
////		CNetMsg rmsg;
//////		PersonalShopErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_AREA); // 전송 메세지 정의
////		SEND_Q(rmsg, ch->m_desc);
////		return ;
////	}
//
//	if (ch->m_pArea == NULL)
//		return ;
//
//
//	int charindex = -1;
//	msg >> charindex;
//	CPC* tch = TO_PC(ch->m_pArea->FindCharInCell(ch, charindex, MSG_CHAR_PC, false));
//	if (tch == NULL)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTFOUND);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//	if (tch->m_personalshop == NULL || (tch->m_personalshop->GetType() & PST_SELL) == 0)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTSELL);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//
//	CNetMsg rmsg;
//#ifdef ENABLE_PET
//	// 일반 아이템에서 펫이면 펫 정보 보내기
//	if (tch->m_personalshop->GetNormalCount())
//	{
//		int i;
//		int pos = -1;
//		for (i = 0; i < tch->m_personalshop->GetNormalCount(); i++)
//		{
//			pos = tch->m_personalshop->GetNextNormalItem(pos);
//			if (pos != -1)
//			{
//				int itemindex = tch->m_personalshop->GetNormalItemIndex(pos);
//				int r, c;
//				if (tch->m_invenNormal.FindItem(&r, &c, itemindex))
//				{
//					CItem* pitem = tch->m_invenNormal.GetItem(r, c);
//					if (pitem)
//					{
//						if (pitem->IsPet())
//						{
//							CPet* pet = tch->GetPet(pitem->m_plus);
//							if (pet)
//							{
//								ExPetSellInfoMsg(rmsg, pet);
//								SEND_Q(rmsg, ch->m_desc);
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//	// 패키지 아이템에서 펫이면 펫 정보 보내기
//	if (tch->m_personalshop->GetType() & PST_PACKAGE)
//	{
//		int i;
//		int pos = -1;
//		for (i = 0; i < tch->m_personalshop->GetPackageCount(); i++)
//		{
//			pos = tch->m_personalshop->GetNextPackageItem(pos);
//			if (pos != -1)
//			{
//				int itemindex = tch->m_personalshop->GetPackageItemIndex(pos);
//				int r, c;
//				if (tch->m_invenNormal.FindItem(&r, &c, itemindex))
//				{
//					CItem* pitem = tch->m_invenNormal.GetItem(r, c);
//					if (pitem)
//					{
//						if (pitem->IsPet())
//						{
//							CPet* pet = tch->GetPet(pitem->m_plus);
//							if (pet)
//							{
//								ExPetSellInfoMsg(rmsg, pet);
//								SEND_Q(rmsg, ch->m_desc);
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//#endif
////	PersonalShopSellListMsg(rmsg, tch);
//	SEND_Q(rmsg, ch->m_desc);
//}
//
//void do_AlternateMerchantChange(CPC* ch, CNetMsg& msg)
//{	
//	/*
//	*  1. user의 지역 검사
//	*  2. 메세지 처리
//	*      - 판매 대행 NPC Index , shop type
//	*	   -  shoptype검사	
//	*	   - 상인 검사
//	*	   - char 검사
//	*/
//
//	if (ch->m_pArea == NULL)
//		return ;
//	if (ch->m_exchange)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//	if (ch->IsSetPlayerState(PLAYER_STATE_WARP))
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//	if (ch->m_currentSkill)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_SKILL);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//
//	int charindex = -1;
//	char shoptype = -1;
//	msg >> charindex
//		>> shoptype;
//
//	if (shoptype != PST_NOSHOP)
//		return ;
//	if (ch->m_personalshop == NULL)
//		return ;
//	if (ch->m_index != charindex)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
////	delete ch->m_personalshop;
////	ch->m_personalshop = NULL;
//	CNetMsg rmsg;
////	PersonalShopChangeMsg(rmsg, ch);
//	ch->m_pArea->SendToCell(rmsg, ch, true);
//
//
//}
//
//void do_AlternateMerchantBuy(CPC* ch, CNetMsg& msg)
//{
//	/*
//	* 1. user의 지역 검사
//	* 2. 대리 상점 NPC 검사
//	* 3. 
//	*/
//
//	if (ch->m_pArea == NULL)
//		return ;
////	if (ch->m_personalshop)
////	{
////		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ALREADY);
////		SEND_Q(rmsg, ch->m_desc);
////		return ;
////	}
//
//	// 구매시 필요한 나스
//	LONGLONG needmoney = 0;
//
//	// 상점 찾기
//	int charindex = -1;
//	char packbuy = -1;
//	msg >> charindex
//		>> packbuy;
//	CPC* tch = TO_PC(ch->m_pArea->FindCharInCell(ch, charindex, MSG_CHAR_PC, false));
//	if (tch == NULL)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTFOUND);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//	if (tch->m_personalshop == NULL || (tch->m_personalshop->GetType() & PST_SELL) == 0)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTSELL);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//
////	CPersonalShop* ps = tch->m_personalshop;
////	char shoptype = ps->GetType();
//
//	int nCountOfPet = 0;
//#ifdef ENABLE_PET 
//	if (ch->m_petList)
//	{
//		CPet* pPetOfBuyer = ch->m_petList;
//		while (pPetOfBuyer)
//		{
//			nCountOfPet++;
//			pPetOfBuyer = pPetOfBuyer->m_nextPet;
//		}
//	}
//#endif // ENABLE_PET
//
//	// 구매 리스트
////	CPersonalShop list(shoptype, "");
//
//	// 패키지 검사
//	if (packbuy)
//	{
////		if ((shoptype & PST_PACKAGE) == 0)
////		{
////			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_NOTPACKAGE);
////			SEND_Q(rmsg, ch->m_desc);
////			return ;
////		}
//
////		needmoney = ps->GetPackagePrice();
////		if (needmoney < 0 || ch->m_moneyItem == NULL || needmoney > ch->m_moneyItem->Count())
////		{
////			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_MONEY);
////			SEND_Q(rmsg, ch->m_desc);
////			return ;
////		}
//
//		// 패키지 구매 리스트에 추가
////		int i, j = -1;
////		for (i = 0; i < ps->GetPackageCount(); i++)
////		{
////			j = ps->GetNextPackageItem(j);
////			if (j == -1)
////			{
////				CNetMsg rmsg;
////				PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
////				SEND_Q(rmsg, ch->m_desc);
////				return ;
////			}
////#ifdef ENABLE_PET 
////			CItem* item = tch->m_invenNormal.GetItem(ps->GetPackageItemIndex(j));
////			if(item == NULL)
////				return ;
////			if (item->IsPet())
////				nCountOfPet++;
////#endif
////			list.AddItem(false, ps->GetPackageItemIndex(j), ps->GetPackageItemCount(j), ps->GetPackagePrice());
////		}
//	}
//
//	// 일반 상품 검사
//	char normalcount = -1;
//	msg >> normalcount;
//	if (normalcount == 0 && !packbuy)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
////	if (normalcount < 0 || normalcount > ps->GetNormalCount())
////	{
////		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
////		SEND_Q(rmsg, ch->m_desc);
////		return ;
////	}
//
//	int i;
//	for (i = 0; i < normalcount; i++)
//	{
//		int itemindex = -1;
//		LONGLONG count = -1;
//		msg >> itemindex
//			>> count;
//
////		int pos = ps->FindItem(true, itemindex);
////		if (pos == -1)
////		{
////			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
////			SEND_Q(rmsg, ch->m_desc);
////			return ;
////		}
//		if (count < 1)
//		{
//			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_COUNT);
//			SEND_Q(rmsg, ch->m_desc);
//			return ;
//		}
////		if (ps->GetNormalItemCount(pos) < count)
////		{
////			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_COUNT);
////			SEND_Q(rmsg, ch->m_desc);
////			return ;
////		}
//#ifdef ENABLE_PET 
//		CItem* item = tch->m_invenNormal.GetItem(itemindex);
//		if(item == NULL)
//			return ;
//
//		if (item->IsPet())
//			nCountOfPet++;
//#endif
////		needmoney += ps->GetNormalItemPrice(pos) * count;
////		if (needmoney < 0 || ch->m_moneyItem == NULL || needmoney > ch->m_moneyItem->Count())
////		{
////			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_MONEY);
////			SEND_Q(rmsg, ch->m_desc);
////			return ;
////		}
//		// 같은 아이템 여러번 사려고 하면 ?////		if (list.FindItem(true, itemindex) != -1 || list.FindItem(false, itemindex) != -1)
////		{
////			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_MONEY);
////			SEND_Q(rmsg, ch->m_desc);
////			GAMELOG << init("HACK PERSONAL SHOP BUY", ch) << end;
////			return ;
////		}
////		list.AddItem(true, ps->GetNormalItemIndex(pos), count, ps->GetNormalItemPrice(pos));
//	}
//
//	if (nCountOfPet > MAX_OWN_PET)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_ANYMORE_PET);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//
//	int needweight = 0;
//	int needspace = 0;
////	int pos;
////	pos = list.GetNextNormalItem(-1);
////	while (pos != -1)
////	{
//		// 파는 아이템 검사
////		int itemindex = list.GetNormalItemIndex(pos);
////		CItem* pitem = tch->m_invenNormal.GetItem(itemindex);
////		if (!pitem)
////		{
////			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
////			SEND_Q(rmsg, ch->m_desc);
////			return ;
////		}
////		if (pitem->Count() < list.GetNormalItemCount(pos))
////		{
////			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
////			SEND_Q(rmsg, ch->m_desc);
////			return ;
////		}
//
////		needweight += pitem->m_itemProto->m_weight * list.GetNormalItemCount(pos);
//
//		// 인벤토리 가능 검사
////		int r, c;
////		if (!(pitem->m_itemProto->m_flag & ITEM_FLAG_COUNT) || !ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag))
////			needspace++;
////
////		pos = list.GetNextNormalItem(pos);
////	}
////	pos = list.GetNextPackageItem(-1);
////	while (pos != -1)
////	{
////		// 파는 아이템 검사
////		int itemindex = list.GetPackageItemIndex(pos);
////		CItem* pitem = tch->m_invenNormal.GetItem(itemindex);
////		if (!pitem)
////		{
////			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
////			SEND_Q(rmsg, ch->m_desc);
////			return ;
////		}
////		if (pitem->Count() < list.GetPackageItemCount(pos))
////		{
////			CNetMsg rmsg;
////			PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_INSUFF_ETC);
////			SEND_Q(rmsg, ch->m_desc);
////			return ;
////		}
////
////		needweight += pitem->m_itemProto->m_weight * list.GetPackageItemCount(pos);
////
////		// 인벤토리 가능 검사
////		int r, c;
////		if (!(pitem->m_itemProto->m_flag & ITEM_FLAG_COUNT) || !ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag))
////			needspace++;
////
////		pos = list.GetNextPackageItem(pos);
////	}
//
//	// 인벤토리 검사
//	if (ch->m_invenNormal.GetSpace() < needspace || ch->m_weight + needweight >= ch->m_maxWeight * 15 / 10)
//	{
//		CNetMsg rmsg;
////		PersonalShopErrorMsg(rmsg, MSG_PERSONALSHOP_ERROR_BUY_FULLINVENTORY);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//
////	GAMELOG << init("PERSONAL SHOP BUY", ch);
////	GAMELOG << "SHOP" << delim << tch->m_name << delim
////			<< tch->m_nick << delim
////			<< tch->m_desc->m_idname << delim;
//	if (packbuy)
//		GAMELOG << "PACKAGE" << delim;
//	else
//		GAMELOG << "NO PACKAGE" << delim;
//	GAMELOG << "NEEDMONEY" << delim << needmoney << delim;
//	GAMELOG << "ITEMS";
//
//	// 일반 아이템 구매/판매
////	pos = list.GetNextNormalItem(-1);
////	while (pos != -1)
////	{
////		int itemindex = list.GetNormalItemIndex(pos);
////		LONGLONG count = list.GetNormalItemCount(pos);
////		CItem* pitem = tch->m_invenNormal.GetItem(itemindex);
////		pitem->m_wearPos = WEARING_NONE;
////		int r, c;
////		CNetMsg rmsg;
////		CItem* nitem;
//
////#ifdef ENABLE_PET
////		if (pitem->IsPet())
////		{
////			CPet* pet = tch->GetPet(pitem->m_plus);
////			if (pet)
////			{
////				REMOVE_FROM_BILIST(pet, tch->m_petList, m_prevPet, m_nextPet);
////				pet->SetOwner(ch);
////				ADD_TO_BILIST(pet, ch->m_petList, m_prevPet, m_nextPet);
////				ExPetStatusMsg(rmsg, pet);
////				SEND_Q(rmsg, ch->m_desc);
////				ExPetSkillListMsg(rmsg, pet);
////				SEND_Q(rmsg, ch->m_desc);
////			}
////		}
////#endif
////
////		// 구매 리스트의 카운트보다 크면
////		if (pitem->Count() > count)
////		{
////			// 겹치는 아이템이 판매 후에도 남는 경우
////			// 만들고
////			nitem = gserver.m_itemProtoList.CreateItem(pitem->m_idNum, -1, pitem->m_plus, pitem->m_flag, count);
////			// 빼고
////			DecreaseFromInventory(tch, pitem, count);
////			// 일리고
////			ItemUpdateMsg(rmsg, pitem, -count);
////			SEND_Q(rmsg, tch->m_desc);
////			// 넣고
////			AddToInventory(ch, nitem, false, true);
////			// 알리고
////			if (nitem->tab() < 0)
////			{
////				// 겹쳤다
////				delete nitem;
////				ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag);
////				nitem = ch->m_invenNormal.GetItem(r, c);
////				ItemUpdateMsg(rmsg, nitem, count);
////				SEND_Q(rmsg, ch->m_desc);
////			}
////			else
////			{
////				// 안겹쳤다
////				ItemAddMsg(rmsg, nitem);
////				SEND_Q(rmsg, ch->m_desc);
////			}
////		}
////		else
////		{
////			// 겹치든 안 겹치든 모두 옮기는 경우
////			nitem = pitem;
////			// 알리고
////			ItemDeleteMsg(rmsg, pitem);
////			SEND_Q(rmsg, tch->m_desc);
////			// 빼고
////			RemoveFromInventory(tch, pitem, false, true);
////			// 넣고
////			AddToInventory(ch, nitem, false, true);
////			// 알리고
////			if (nitem->tab() < 0)
////			{
////				// 겹쳤다
////				delete nitem;
////				ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag);
////				nitem = ch->m_invenNormal.GetItem(r, c);
////				ItemUpdateMsg(rmsg, nitem, count);
////				SEND_Q(rmsg, ch->m_desc);
////			}
////			else
////			{
////				// 안겹쳤다
////				ItemAddMsg(rmsg, nitem);
////				SEND_Q(rmsg, ch->m_desc);
////			}
////		}
////
////		GAMELOG << delim << itemlog(nitem)
////				<< delim << count;
////
////		// 판매 리스트 갱신
////		ps->RemoveNormalItem(itemindex, count);
////
////		pos = list.GetNextNormalItem(pos);
////	}
//
////	// 패키지 아이템 구매 판매
////	pos = list.GetNextPackageItem(-1);
////	while (pos != -1)
////	{
////		int itemindex = list.GetPackageItemIndex(pos);
////		LONGLONG count = list.GetPackageItemCount(pos);
////		CItem* pitem = tch->m_invenNormal.GetItem(itemindex);
////		int r, c;
////		CNetMsg rmsg;
////		CItem* nitem;
////
////#ifdef ENABLE_PET
////		if (pitem->IsPet())
////		{
////			CPet* pet = tch->GetPet(pitem->m_plus);
////			if (pet)
////			{
////				REMOVE_FROM_BILIST(pet, tch->m_petList, m_prevPet, m_nextPet);
////				pet->SetOwner(ch);
////				ADD_TO_BILIST(pet, ch->m_petList, m_prevPet, m_nextPet);
////				ExPetStatusMsg(rmsg, pet);
////				SEND_Q(rmsg, ch->m_desc);
////				ExPetSkillListMsg(rmsg, pet);
////				SEND_Q(rmsg, ch->m_desc);
////			}
////		}
////#endif
////
////		// 구매 리스트의 카운트보다 크면
////		if (pitem->Count() > count)
////		{
////			// 겹치는 아이템이 판매 후에도 남는 경우
////			// 만들고
////			nitem = gserver.m_itemProtoList.CreateItem(pitem->m_idNum, -1, pitem->m_plus, pitem->m_flag, count);
////			// 빼고
////			DecreaseFromInventory(tch, pitem, count);
////			// 일리고
////			ItemUpdateMsg(rmsg, pitem, -count);
////			SEND_Q(rmsg, tch->m_desc);
////			// 넣고
////			AddToInventory(ch, nitem, false, true);
////			// 알리고
////			if (nitem->tab() < 0)
////			{
////				// 겹쳤다
////				delete nitem;
////				ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag);
////				nitem = ch->m_invenNormal.GetItem(r, c);
////				ItemUpdateMsg(rmsg, nitem, count);
////				SEND_Q(rmsg, ch->m_desc);
////			}
////			else
////			{
////				// 안겹쳤다
////				ItemAddMsg(rmsg, nitem);
////				SEND_Q(rmsg, ch->m_desc);
////			}
////		}
////		else
////		{
////			// 겹치든 안 겹치든 모두 옮기는 경우
////			nitem = pitem;
////			// 알리고
////			ItemDeleteMsg(rmsg, pitem);
////			SEND_Q(rmsg, tch->m_desc);
////			// 빼고
////			RemoveFromInventory(tch, pitem, false, true);
////			// 넣고
////			AddToInventory(ch, nitem, false, true);
////			// 알리고
////			if (nitem->tab() < 0)
////			{
////				// 겹쳤다
////				delete nitem;
////				ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag);
////				nitem = ch->m_invenNormal.GetItem(r, c);
////				ItemUpdateMsg(rmsg, nitem, count);
////				SEND_Q(rmsg, ch->m_desc);
////			}
////			else
////			{
////				// 안겹쳤다
////				ItemAddMsg(rmsg, nitem);
////				SEND_Q(rmsg, ch->m_desc);
////			}
////		}
////
////		GAMELOG << delim << itemlog(nitem)
////				<< delim << count;
////
////		// 판매 리스트 갱신
////		ps->RemovePackage();
////
////		pos = list.GetNextPackageItem(pos);
////	}
////
////	GAMELOG << end;
////
////	// 돈 넣고 빼기
////	if (ch->m_moneyItem->Count() > needmoney)
////	{
////		CNetMsg rmsg;
////		ch->AddMoney(-needmoney);
////		ItemUpdateMsg(rmsg, ch->m_moneyItem, -needmoney);
////		SEND_Q(rmsg, ch->m_desc);
////	}
////	else
////	{
////		CNetMsg rmsg;
////		ItemDeleteMsg(rmsg, ch->m_moneyItem);
////		ch->AddMoney(-needmoney);
////		SEND_Q(rmsg, ch->m_desc);
////	}
////	int rate = (shoptype & (PST_PACKAGE | PST_PREMIUM)) ? PS_TEX_PREMIUM : PS_TEX_NORMAL;
////	if (tch->m_moneyItem)
////	{
////		CNetMsg rmsg;
////		tch->AddMoney(needmoney - (needmoney * rate / 10000));
////		ItemUpdateMsg(rmsg, tch->m_moneyItem, needmoney - (needmoney * rate / 10000));
////		SEND_Q(rmsg, tch->m_desc);
////	}
////	else
////	{
////		CNetMsg rmsg;
////		if (tch->AddMoney(needmoney - (needmoney * rate / 10000)))
////		{
////			ItemAddMsg(rmsg, tch->m_moneyItem);
////			SEND_Q(rmsg, tch->m_desc);
////		}
////		else
////		{
////			GAMELOG << init("PERSONAL SHOP ERROR : CANT GIVE MONEY TO SHOP", tch)
////					<< "MONEY" << delim
////					<< (needmoney - (needmoney * rate / 10000))
////					<< end;
////		}
////	}
////
////	// 판매자에게 판매됨 알림
////	CNetMsg rmsg;
////	PersonalShopBuyMsg(rmsg, msg, ch);
////	SEND_Q(rmsg, tch->m_desc);
////
////	// 상점 종료 및 상태 변화 검사
////	// 패키지 상태 검사
////	if (ps->GetType() != shoptype)
////	{
////		CNetMsg rmsg;
////		PersonalShopChangeMsg(rmsg, tch);
////		tch->m_pArea->SendToCell(rmsg, tch, true);
////		if (ps->GetType() == PST_NOSHOP)
////		{
////			delete ps;
////			tch->m_personalshop = NULL;
////		}
////	}
//	ch->SaveNow(false);
//	tch->SaveNow(false);
//}
//
//#endif // ALTERNATE_MERCHANT
