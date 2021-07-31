#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"



#ifdef ALTERNATE_MERCHANT
void do_AlternateMerchant(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype = (unsigned char)-1;
//	unsigned int subtype = (unsigned int )-1;
	msg >> subtype;
	switch (subtype)
	{

	case	MSG_ALTERNATEMERCHANT_SELL_START :
		do_AlternateMerchantSellStart(ch, msg);
		break;
	case	MSG_ALTERNATEMERCHANT_SELL_LIST :
		do_AlternateMerchantSellList(ch, msg);
		break;
	case	MSG_ALTERNATEMERCHANT_CHANGE :
		do_AlternateMerchantChange(ch, msg);
		break;
	case	MSG_ALTERNATEMERCHANT_BUY :
		do_AlternateMerchantBuy(ch, msg);
		break;

	case	MSG_ALTERNATEMERCHANT_END:
		do_AlternateMerchantEnd( ch, msg );
		break;
	case	MSG_ALTERNATEMERCHANT_PRODUCT_RECOVERY:
		do_AlternateMerchantProductRecovery( ch, msg );
		break;
	case MSG_ALTERNATEMERCHANT_PRODUCT_NAS:
		do_AlternateMerchantNasRecovery( ch, msg );
		break;

	}
}

void do_AlternateMerchantSellStart(CPC* ch, CNetMsg& msg)
{
	if (ch->m_pArea == NULL)
		return ;

	if (ch->m_moneyItem == NULL && ch->m_invenNormal.GetSpace() < 1)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_NOSPACE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->m_pArea->GetAttr(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)) == MAPATT_WARZONE)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_AREA);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 브라질 요청 업데이트, OX 퀴즈룸에선 개인상점 금지...yhj 090227
	if ( ch->m_pZone->IsOXQuizRoom() )
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_AREA);
		SEND_Q(rmsg, ch->m_desc);
		return;	
	}

	if (ch->m_exchange)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->IsSetPlayerState(PLAYER_STATE_WARP))
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->m_currentSkill)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_SKILL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->m_pAlternateMerchant)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ALREADY);
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
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}


	char normalCount = -1;
	msg >> normalCount;
	if (normalCount < 0 || normalCount > PS_MAX_NORMAL_ITEM)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (normalCount == 0 && (shoptype & PST_PACKAGE) == 0)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CAlternateMerchant* ps = new CAlternateMerchant( shoptype, shopname );
	ps->SetOwnerPC( ch );

	int i;
	char* pNormalRow			= new char[normalCount];
	char* pNormalCol			= new char[normalCount];
	int* pNormalItemIndex		= new int[normalCount];
	int* pNormalItemDBIndex		= new int[normalCount];
	LONGLONG* pNormalItemCount	= new LONGLONG[normalCount];
	LONGLONG* pNormalItemPrice	= new LONGLONG[normalCount];

	memset( pNormalRow , -1, normalCount );			memset( pNormalCol , -1, normalCount );
	memset( pNormalItemIndex , -1, normalCount );	memset( pNormalItemDBIndex , -1, normalCount );
	memset( pNormalItemCount , -1, normalCount );	memset( pNormalItemPrice , -1, normalCount );

	for (i = 0; i < normalCount; i++)
	{
		msg >> pNormalRow[i]
			>> pNormalCol[i]
			>> pNormalItemIndex[i]
			>> pNormalItemDBIndex[i]
			>> pNormalItemCount[i]
			>> pNormalItemPrice[i];
		if( pNormalItemDBIndex[i] == gserver.m_itemProtoList.m_moneyItem->m_index )
		{
			delete ps;
			delete [] pNormalRow;
			delete [] pNormalCol;
			delete [] pNormalItemIndex;
			delete [] pNormalItemDBIndex;
			delete [] pNormalItemCount;
			delete [] pNormalItemPrice;

			CNetMsg rmsg;
			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		// 060406 : bs : 봉인된 아이템 교환 불가
		CItem* item = ch->m_invenNormal.GetItem(pNormalRow[i], pNormalCol[i]);
		if ( 
			item == NULL || item->m_index != pNormalItemIndex[i]	|| item->m_idNum != pNormalItemDBIndex[i]	|| item->Count() < pNormalItemCount[i]
				|| pNormalItemPrice[i] < 1 || !item->CanExchange()
			)
		{
			delete ps;
			delete [] pNormalRow;
			delete [] pNormalCol;
			delete [] pNormalItemIndex;
			delete [] pNormalItemDBIndex;
			delete [] pNormalItemCount;
			delete [] pNormalItemPrice;

			CNetMsg rmsg;
			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
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
				delete [] pNormalRow; delete [] pNormalCol; delete [] pNormalItemIndex;
				delete [] pNormalItemDBIndex; delete [] pNormalItemCount; delete [] pNormalItemPrice;


				CNetMsg rmsg;
				AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
#endif
		if (ps->FindItemPos(true, pNormalItemIndex[i]) != -1)
		{
			delete ps; delete [] pNormalRow; delete [] pNormalCol; delete [] pNormalItemIndex;
			delete [] pNormalItemDBIndex; delete [] pNormalItemCount; delete [] pNormalItemPrice;

			CNetMsg rmsg;
			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_DUPLICATION);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (!ps->AddItem(true, pNormalItemIndex[i], pNormalItemCount[i], pNormalItemPrice[i]))
		{
			delete ps; delete [] pNormalRow; delete [] pNormalCol; delete [] pNormalItemIndex;
			delete [] pNormalItemDBIndex; delete [] pNormalItemCount; delete [] pNormalItemPrice;

			CNetMsg rmsg;
			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		ps->m_NormalItems[i] = item;
	}

	LONGLONG lPackItemPrice = -1;
	char cPackItemCount		= -1; 
	char* cPackIemRow		= NULL;
	char* cPackItemCol		= NULL;
	int* pPackItemIndex		= NULL;
	int* pPackItemDBIndex	= NULL;
	LONGLONG* pPackItemCount	= NULL;

	if (shoptype & PST_PACKAGE)
	{
		msg >> lPackItemPrice
			>> cPackItemCount;
		if (lPackItemPrice < 1 || cPackItemCount < 1 || cPackItemCount > PS_MAX_PACKAGE_ITEM)
		{
			delete ps;			delete [] pNormalRow;			delete [] pNormalCol;
			delete [] pNormalItemIndex;			delete [] pNormalItemDBIndex;			delete [] pNormalItemCount;
			delete [] pNormalItemPrice;

			CNetMsg rmsg;
			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		cPackIemRow			= new char[cPackItemCount];
		cPackItemCol		= new char[cPackItemCount];
		pPackItemIndex		= new int[cPackItemCount];
		pPackItemDBIndex	= new int[cPackItemCount];
		pPackItemCount	= new LONGLONG[cPackItemCount];

		
		memset( cPackIemRow , -1, normalCount );			memset( cPackItemCol , -1, normalCount );
		memset( pPackItemIndex , -1, normalCount );	memset( pPackItemDBIndex , -1, normalCount );
		memset( pPackItemCount , -1, normalCount );

		for (i = 0; i < cPackItemCount; i++)
		{			
			msg >> cPackIemRow[i]
				>> cPackItemCol[i]
				>> pPackItemIndex[i]
				>> pPackItemDBIndex[i]
				>> pPackItemCount[i];
			if (pPackItemDBIndex[i] == gserver.m_itemProtoList.m_moneyItem->m_index)
			{
				delete ps;
				delete [] pNormalRow;		delete [] pNormalCol;		delete [] pNormalItemIndex;
				delete [] pNormalItemDBIndex;		delete [] pNormalItemCount;		delete [] pNormalItemPrice;
				delete [] cPackIemRow;		delete [] cPackItemCol;		delete [] pPackItemIndex;
				delete [] pPackItemDBIndex;		delete [] pPackItemCount;	


				CNetMsg rmsg;
				AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_MONEY);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			// 060406 : bs : 봉인된 아이템 교환 불가
			CItem* item = ch->m_invenNormal.GetItem( cPackIemRow[i], cPackItemCol[i] );
			if ( 
				item == NULL || item->m_index != pPackItemIndex[i] || item->m_idNum != pPackItemDBIndex[i]
					|| item->Count() < pPackItemCount[i] || !item->CanExchange()
				)
			{
				delete ps;
				delete [] pNormalRow;		delete [] pNormalCol;		delete [] pNormalItemIndex;
				delete [] pNormalItemDBIndex;		delete [] pNormalItemCount;		delete [] pNormalItemPrice;
				delete [] cPackIemRow;		delete [] cPackItemCol;		delete [] pPackItemIndex;
				delete [] pPackItemDBIndex;		delete [] pPackItemCount;	

				CNetMsg rmsg;
				AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			if (ps->FindItemPos(false, pNormalItemIndex[i]) != -1)
			{
				delete ps;
				delete [] pNormalRow;		delete [] pNormalCol;		delete [] pNormalItemIndex;
				delete [] pNormalItemDBIndex;		delete [] pNormalItemCount;		delete [] pNormalItemPrice;
				delete [] cPackIemRow;		delete [] cPackItemCol;		delete [] pPackItemIndex;
				delete [] pPackItemDBIndex;		delete [] pPackItemCount;	

				CNetMsg rmsg;
				AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_DUPLICATION);
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
					delete [] pNormalRow;		delete [] pNormalCol;		delete [] pNormalItemIndex;
					delete [] pNormalItemDBIndex;		delete [] pNormalItemCount;		delete [] pNormalItemPrice;
					delete [] cPackIemRow;		delete [] cPackItemCol;		delete [] pPackItemIndex;
					delete [] pPackItemDBIndex;		delete [] pPackItemCount;	

					CNetMsg rmsg;
					AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}
#endif
			if (!ps->AddItem(false, pPackItemIndex[i], pPackItemCount[i], lPackItemPrice))
			{
				delete ps;
				delete [] pNormalRow;		delete [] pNormalCol;		delete [] pNormalItemIndex;
				delete [] pNormalItemDBIndex;		delete [] pNormalItemCount;		delete [] pNormalItemPrice;
				delete [] cPackIemRow;		delete [] cPackItemCol;		delete [] pPackItemIndex;
				delete [] pPackItemDBIndex;		delete [] pPackItemCount;

				CNetMsg rmsg;
				AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			ps->m_PackItems[i] = item;
		}
	}

	ch->m_pAlternateMerchant = ps;

	gserver.m_AlternateMerchantList.AddAlternateMerchant( ps );

	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		HelperAlternateMerchantStartMsg( rmsg, charindex, shoptype, ps );
		SEND_Q(rmsg, gserver.m_helper);
		return;

	}
	else
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_HELPER_SERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CNetMsg rmsg;
//	PersonalShopSellStartMsg(rmsg, ch);
	ch->m_pArea->SendToCell(rmsg, ch, true);
//
//	GAMELOG << init("PERSONAL SHOP START", ch)
//			<< "TYPE" << delim << ps->GetType() << delim << "ITEMS";
//	int pos = -1;
//	for (i = 0; i < ps->GetNormalCount(); i++)
//	{
//		pos = ps->GetNextNormalItem(pos);
//		int itemindex = ps->GetNormalItemIndex(pos);
//		CItem* item = ch->m_invenNormal.GetItem(itemindex);
//		GAMELOG << delim << itemlog(item)
//				<< delim << ps->GetNormalItemCount(pos)
//				<< delim << ps->GetNormalItemPrice(pos);
//	}
//	if (ps->GetType() & PST_PACKAGE)
//	{
//		GAMELOG << delim << "PACKAGE"
//				<< delim << ps->GetPackagePrice();
//
//		pos = -1;
//		for (i = 0; i < ps->GetPackageCount(); i++)
//		{
//			pos = ps->GetNextPackageItem(pos);
//			int itemindex = ps->GetPackageItemIndex(pos);
//			CItem* item = ch->m_invenNormal.GetItem(itemindex);
//			GAMELOG << delim << itemlog(item)
//					<< delim << ps->GetPackageItemCount(pos);
//		}
//	}
//	GAMELOG << end;
}

void do_AlternateMerchantSellList(CPC* ch, CNetMsg& msg)
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

	CNPC* pAlternateNPC = ch->m_pArea->FindProtoNPCInCell( ch, charindex, false, 2 );
	if( pAlternateNPC == NULL )
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;		
	}

	CAlternateMerchant* tch = gserver.m_AlternateMerchantList.Find( pAlternateNPC->m_index );
	if (tch == NULL)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (tch == NULL || (tch->GetShopType() & PST_SELL) == 0)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_NOTSELL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}


	CNetMsg rmsg;
#ifdef ENABLE_PET
	// 일반 아이템에서 펫이면 펫 정보 보내기
		if (tch->GetNormalCount())
	{
		int i;
		int pos = -1;
		for (i = 0; i < tch->GetNormalCount(); i++)
		{
			pos = tch->GetNextNormalItem(pos);
			if (pos != -1)
			{
				int itemindex = tch->GetNormalItemIndex(pos);

				CItem* pitem = tch->FindItem( true, itemindex );
				if (pitem)
				{
					if (pitem->IsPet())
					{
						CPet* pet = tch->GetOwnerPC()->GetPet(pitem->m_plus);
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
	// 패키지 아이템에서 펫이면 펫 정보 보내기
	if (tch->GetShopType() & PST_PACKAGE)
	{
		int i;
		int pos = -1;
		for (i = 0; i < tch->GetPackageCount(); i++)
		{
			pos = tch->GetNextPackageItem(pos);
			if (pos != -1)
			{
				int itemindex = tch->GetPackageItemIndex(pos);
				CItem* pitem = tch->FindItem( false, itemindex );
				if (pitem)
				{
					if (pitem->IsPet())
					{
						CPet* pet = tch->GetOwnerPC()->GetPet(pitem->m_plus);
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
#endif
	//PersonalShopSellListMsg(rmsg, tch);
	SEND_Q(rmsg, ch->m_desc);
}

void do_AlternateMerchantChange(CPC* ch, CNetMsg& msg)
{
	if (ch->m_pArea == NULL)
		return ;
	
	if (ch->m_exchange)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->IsSetPlayerState(PLAYER_STATE_WARP))
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->m_currentSkill)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_SKILL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int charindex = -1;
	char shoptype = -1;
	msg >> charindex
		>> shoptype;

	if (shoptype != PST_NOSHOP)
		return ;
	if (ch->m_pAlternateMerchant == NULL)
		return ;
	if (ch->m_pAlternateMerchant->m_nlistIdx != charindex)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CNPC* pAlternateNPC = ch->m_pArea->FindProtoNPCInCell( ch, charindex, false, 2 );

	CAlternateMerchant* pMerchant = gserver.m_AlternateMerchantList.Find( charindex );
	gserver.m_AlternateMerchantList.RemoveAlternateMerchant( pMerchant );
	delete ch->m_pAlternateMerchant;
	ch->m_pAlternateMerchant = NULL;


	CNetMsg rmsg;
	AlternateMerchantChangeMsg(rmsg, ch);
	pAlternateNPC->m_pArea->SendToCell(rmsg, ch, true);

	delete ch->m_personalshop;
	ch->m_personalshop = NULL;
}

void do_AlternateMerchantBuy(CPC* ch, CNetMsg& msg)
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
	
	CNPC* pAlternateNPC = ch->m_pArea->FindProtoNPCInCell( ch, charindex, false, 2 );
	if( pAlternateNPC == NULL )
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;		
	}

	CAlternateMerchant* ps = gserver.m_AlternateMerchantList.Find( pAlternateNPC->m_index );
	if (ps == NULL)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ps == NULL || (ps->GetShopType() & PST_SELL) == 0)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_NOTSELL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	char shoptype = ps->GetShopType();

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
	CAlternateMerchant list(shoptype, "");

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
			CItem* item = ps->FindItem( false, ps->GetPackageItemIndex(j) );
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
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (normalcount < 0 || normalcount > ps->GetNormalCount())
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
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

		int pos = ps->FindItemPos(true, itemindex);
		if (pos == -1)
		{
			CNetMsg rmsg;
			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (count < 1)
		{
			CNetMsg rmsg;
						AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_COUNT);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (ps->GetNormalItemCount(pos) < count)
		{
			CNetMsg rmsg;
			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_COUNT);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
#ifdef ENABLE_PET 
		CItem* item = ps->FindItem(true, itemindex);
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
		// 같은 아이템 여러번 사려고 하면 ?		if (list.FindItemPos(true, itemindex) != -1 || list.FindItemPos(false, itemindex) != -1)
		{
			CNetMsg rmsg;
			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			GAMELOG << init("HACK PERSONAL SHOP BUY", ch) << end;
			return ;
		}
		list.AddItem(true, ps->GetNormalItemIndex(pos), count, ps->GetNormalItemPrice(pos));
	}

	if (nCountOfPet > MAX_OWN_PET)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_ANYMORE_PET);
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
		CItem* pitem = ps->FindItem( true, itemindex );
		if (!pitem)
		{
			CNetMsg rmsg;
			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (pitem->Count() < list.GetNormalItemCount(pos))
		{
			CNetMsg rmsg;
			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
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
		CItem* pitem = ps->FindItem( true, itemindex );
		if (!pitem)
		{
			CNetMsg rmsg;
			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (pitem->Count() < list.GetPackageItemCount(pos))
		{
			CNetMsg rmsg;
						AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
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
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_BUY_FULLINVENTORY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("ALTERNATE MERCHANT BUY", ch);
	GAMELOG << "ALTERNATE_MERCHANT" << delim << ps->GetOwnerPC()->m_name << delim;			
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
		CItem* pitem = ps->FindItem( true, itemindex );
		pitem->m_wearPos = WEARING_NONE;
		int r, c;
		CNetMsg rmsg;
		CItem* nitem;

#ifdef ENABLE_PET
		if (pitem->IsPet())
		{
			CPet* pet = ps->GetOwnerPC()->GetPet(pitem->m_plus);
			if (pet)
			{
				REMOVE_FROM_BILIST(pet, ps->GetOwnerPC()->m_petList, m_prevPet, m_nextPet);
				pet->SetOwner(ch);
				ADD_TO_BILIST(pet, ch->m_petList, m_prevPet, m_nextPet);
				ExPetStatusMsg(rmsg, pet);
				SEND_Q(rmsg, ch->m_desc);
				ExPetSkillListMsg(rmsg, pet);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
#endif

		// 구매 리스트의 카운트보다 크면
		if (pitem->Count() > count)
		{
			// 겹치는 아이템이 판매 후에도 남는 경우
			// 만들고
			nitem = gserver.m_itemProtoList.CreateItem(pitem->m_idNum, -1, pitem->m_plus, pitem->m_flag, count);
//			// 빼고
//			DecreaseFromInventory(tch, pitem, count);
//			// 일리고
//			ItemUpdateMsg(rmsg, pitem, -count);
//			SEND_Q(rmsg, tch->m_desc);
//			// 넣고
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
//			// 알리고
//			ItemDeleteMsg(rmsg, pitem);
//			SEND_Q(rmsg, tch->m_desc);
//			// 빼고
//			RemoveFromInventory(tch, pitem, false, true);
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
		CItem* pitem = ps->FindItem(false, itemindex);
		int r, c;
		CNetMsg rmsg;
		CItem* nitem;

#ifdef ENABLE_PET
		if (pitem->IsPet())
		{
			CPet* pet = ps->GetOwnerPC()->GetPet(pitem->m_plus);
			if (pet)
			{
				REMOVE_FROM_BILIST(pet, ps->GetOwnerPC()->m_petList, m_prevPet, m_nextPet);
				pet->SetOwner(ch);
				ADD_TO_BILIST(pet, ch->m_petList, m_prevPet, m_nextPet);
				ExPetStatusMsg(rmsg, pet);
				SEND_Q(rmsg, ch->m_desc);
				ExPetSkillListMsg(rmsg, pet);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
#endif

		// 구매 리스트의 카운트보다 크면
		if (pitem->Count() > count)
		{
			// 겹치는 아이템이 판매 후에도 남는 경우
			// 만들고
			nitem = gserver.m_itemProtoList.CreateItem(pitem->m_idNum, -1, pitem->m_plus, pitem->m_flag, count);
//			// 빼고
//			DecreaseFromInventory(tch, pitem, count);
//			// 일리고
//			ItemUpdateMsg(rmsg, pitem, -count);
//			SEND_Q(rmsg, tch->m_desc);
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
//			// 알리고
//			ItemDeleteMsg(rmsg, pitem);
//			SEND_Q(rmsg, tch->m_desc);
//			// 빼고
//			RemoveFromInventory(tch, pitem, false, true);
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
	if (ps->GetNas() )
	{
//		CNetMsg rmsg;
		ps->AddNas( needmoney - ( needmoney * rate / 10000 ) );
//		tch->AddMoney(needmoney - (needmoney * rate / 10000));
//		ItemUpdateMsg(rmsg, tch->m_moneyItem, needmoney - (needmoney * rate / 10000));
//		SEND_Q(rmsg, tch->m_desc);
	}
	else
	{
//		CNetMsg rmsg;
		ps->AddNas( needmoney - ( needmoney * rate / 10000 ) );
//		if (tch->AddMoney(needmoney - (needmoney * rate / 10000)))
//		{
//			ItemAddMsg(rmsg, tch->m_moneyItem);
//			SEND_Q(rmsg, tch->m_desc);
//		}
//		else
//		{
//			GAMELOG << init("PERSONAL SHOP ERROR : CANT GIVE MONEY TO SHOP", tch)
//					<< "MONEY" << delim
//					<< (needmoney - (needmoney * rate / 10000))
//					<< end;
//		}
	}

	// 판매자에게 판매됨 알림
	CNetMsg rmsg;
	PersonalShopBuyMsg(rmsg, msg, ch);
	SEND_Q(rmsg, ps->GetOwnerPC()->m_desc);

	// 상점 종료 및 상태 변화 검사
	// 패키지 상태 검사
	if (ps->GetShopType() != shoptype)
	{
		CNetMsg rmsg;
//		PersonalShopChangeMsg(rmsg, tch);
//		ps->m_pArea->SendToCell(rmsg, ps, true);
		if (ps->GetShopType() == PST_NOSHOP)
		{
			delete ps;
			ps = NULL;
		}
	}

	ch->SaveNow(false);
//	tch->SaveNow(false);

	// 아이템의 변화를 HELPER에 알림 
	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
//		HelperAlternateMerchantBuytMsg( rmsg, charindex, shoptype, ps );
		SEND_Q(rmsg, gserver.m_helper);
		return;

	}
	else
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_HELPER_SERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_AlternateMerchantEnd(CPC* ch, CNetMsg& msg)
{
	if (ch->m_pArea == NULL)
		return ;
	if (ch->m_exchange)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->IsSetPlayerState(PLAYER_STATE_WARP))
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_EXCHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (ch->m_currentSkill)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_SKILL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int charindex = -1;
	char shoptype = -1;
	msg >> charindex
		>> shoptype;

	if (shoptype != PST_NOSHOP)
		return ;
	if (ch->m_pAlternateMerchant == NULL)
		return ;
	if (ch->m_pAlternateMerchant->m_nlistIdx != charindex)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CNPC* pAlternateNPC = ch->m_pArea->FindProtoNPCInCell( ch, charindex, false, 2 );

	CAlternateMerchant* pMerchant = gserver.m_AlternateMerchantList.Find( charindex );
	gserver.m_AlternateMerchantList.RemoveAlternateMerchant( pMerchant );
	delete ch->m_pAlternateMerchant;
	ch->m_pAlternateMerchant = NULL;

	
	// 아이템의 변화를 HELPER에 알림 
	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		HelperAlternateMerchantEndMsg( rmsg, charindex, shoptype, ch->m_pAlternateMerchant );
		SEND_Q(rmsg, gserver.m_helper);
		return;

	}
	else
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_HELPER_SERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CNetMsg rmsg;
	AlternateMerchantChangeMsg(rmsg, ch);
	pAlternateNPC->m_pArea->SendToCell(rmsg, ch, true);

	delete ch->m_personalshop;
	ch->m_personalshop = NULL;


//	CAlternateMerchant* pMerchant = gserver.m_AlternateMerchantList.Find( charindex );
//	gserver.m_AlternateMerchantList.RemoveAlternateMerchant( pMerchant );
//	delete ch->m_pAlternateMerchant;
//	ch->m_pAlternateMerchant = NULL;
//	CNetMsg rmsg;
//	AlternateMerchantChangeMsg(rmsg, ch);
//	ch->m_pArea->SendToCell(rmsg, ch, true);
}
void do_AlternateMerchantProductRecovery(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	// 인벤토리 검사
//	if (ch->m_invenNormal.GetSpace() < needspace || ch->m_weight + needweight >= ch->m_maxWeight * 15 / 10)
//	{
//		CNetMsg rmsg;
//		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_BUY_FULLINVENTORY);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}

	// 회수할 ITEM검사를 위해서 HELPER로 메세지 전송 
	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
//		HelperAlternateMerchantProductRecoverytMsg( rmsg, charindex, shoptype, ch->m_pAlternateMerchant );
		SEND_Q(rmsg, gserver.m_helper);
		return;

	}
	else
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_HELPER_SERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

//
//	int needweight = 0;
//	int needspace = 0;
//	int pos;
//	pos = list.GetNextNormalItem(-1);
//	while (pos != -1)
//	{
//		// 파는 아이템 검사
//		int itemindex = list.GetNormalItemIndex(pos);
//		CItem* pitem = ps->FindItem( true, itemindex );
//		if (!pitem)
//		{
//			CNetMsg rmsg;
//			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
//			SEND_Q(rmsg, ch->m_desc);
//			return ;
//		}
//		if (pitem->Count() < list.GetNormalItemCount(pos))
//		{
//			CNetMsg rmsg;
//			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
//			SEND_Q(rmsg, ch->m_desc);
//			return ;
//		}
//
//		needweight += pitem->m_itemProto->m_weight * list.GetNormalItemCount(pos);
//
//		// 인벤토리 가능 검사
//		int r, c;
//		if (!(pitem->m_itemProto->m_flag & ITEM_FLAG_COUNT) || !ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag))
//			needspace++;
//
//		pos = list.GetNextNormalItem(pos);
//	}
//	pos = list.GetNextPackageItem(-1);
//	while (pos != -1)
//	{
//		// 파는 아이템 검사
//		int itemindex = list.GetPackageItemIndex(pos);
//		CItem* pitem = ps->FindItem( true, itemindex );
//		if (!pitem)
//		{
//			CNetMsg rmsg;
//			AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
//			SEND_Q(rmsg, ch->m_desc);
//			return ;
//		}
//		if (pitem->Count() < list.GetPackageItemCount(pos))
//		{
//			CNetMsg rmsg;
//						AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC);
//			SEND_Q(rmsg, ch->m_desc);
//			return ;
//		}
//
//		needweight += pitem->m_itemProto->m_weight * list.GetPackageItemCount(pos);
//
//		// 인벤토리 가능 검사
//		int r, c;
//		if (!(pitem->m_itemProto->m_flag & ITEM_FLAG_COUNT) || !ch->m_invenNormal.FindItem(&r, &c, pitem->m_idNum, pitem->m_plus, pitem->m_flag))
//			needspace++;
//
//		pos = list.GetNextPackageItem(pos);
//	}
	// 회수할 ITEM검사를 위해서 HELPER로 메세지 전송
}

void do_AlternateMerchantNasRecovery(CPC* ch, CNetMsg& msg)
{
	// 인벤의 빈공간 검사
	if (ch->m_moneyItem == NULL && ch->m_invenNormal.GetSpace() < 1)
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_NOSPACE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}	
	// 회수할 ITEM검사를 위해서 HELPER로 메세지 전송 
	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
//		HelperAlternateMerchantNasRecoveryMsg( rmsg, charindex, shoptype, ch->m_pAlternateMerchant );
		SEND_Q(rmsg, gserver.m_helper);
		return;

	}
	else
	{
		CNetMsg rmsg;
		AlternateMerchantErrorMsg(rmsg, MSG_ALTERNATEMERCHANT_ERROR_HELPER_SERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

#endif // ALTERNATE_MERCHANT
