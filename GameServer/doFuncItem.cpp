#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "hardcoding.h"
#include "Battle.h"
#include "Exp.h"
#include "WarCastle.h"
#include "DratanCastle.h"

//////////////
// 아이템 관련
#if defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)
bool do_ItemUse_SakuraBox(CPC * pc, const CItemProto * pItemProto, int nExtra1);
#endif // defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)

#ifdef ALTERNATE_MERCHANT
bool do_useCashItem_AlternateMerchant( CPC* ch, CItem* pItem );
#endif // ALTERNATE_MERCHANT

#ifdef USE_GROCERY
void do_GroceryItemBuy(CPC* ch, CNetMsg& msg);
void do_GroceryItemSell(CPC* ch, CNetMsg& msg);
#endif // USE_GROCERY


#if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 
bool do_ItemUse_2046(CPC* ch, const CItemProto* itemproto, int nExtra1);
#endif

#ifdef MONSTER_COMBO_COIN_CHANGE
bool do_ItemUse_Coin(CPC * pc, const CItemProto * pItemProto, int nExtra1);
#endif // MONSTER_COMBO_COIN_CHANGE

#ifdef EVENT_EGGS_HUNT_2007
bool do_ItemUse_2148(CPC* ch, const CItemProto* itemproto);
#endif // EVENT_EGGS_HUNT_2007

#ifdef EVENT_NEW_MOONSTONE
bool do_itemUse_MoonStoneBox(CPC* ch, const CItemProto* itemproto);
#endif // EVENT_NEW_MOONSTONE

bool do_ItemUse_2037(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2597(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_1838_1839(CPC* ch, const CItemProto* itemproto, int nExtar1);
bool do_ItemUse_IONCE_ETC(CPC* ch, const CItemProto* itemproto, int nExtar1);
bool do_ItemUse_IETC_PRODUCT(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_IONCE_SUMMON(CPC* ch, CItem* item);
bool do_ItemUse_IONCE_QUEST_SCROLL(CPC* ch, const CItemProto* itemproto, int extra1);
bool do_ItemUse_IONCE_CHANGE_DOC(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_IONCE_WARP_RETURN(CPC* ch, const CItemProto* itemproto);

// 060227 : bs : 파티리콜
void do_ItemUsePartyRecall(CPC* ch, CItem* pItem);

#ifdef RECOMMEND_SERVER_SYSTEM
void do_ItemTake_RecommendPotion(CPC* ch, CItem* pItem);
#endif // RECOMMEND_SERVER_SYSTEM

#ifdef EVENT_COLLECT_BUG
bool do_ItemTake_EventCollectBug(CPC* ch, CItem* pItem);
#endif // EVENT_COLLECT_BUG

#ifdef ITEM_COMPOSITE
bool do_useCashItem_1420(CPC* ch, CItem* item, CNetMsg& msg, bool bprolong, int extra1);  // 미감정 아이템
bool do_useCashItem_1421(CPC* ch, CItem* item, CNetMsg& msg, bool bprolong, int nIndexClothes);		// 감정 아이템
// << kjban 080129
#ifdef LIMIT_ITEM_M35
bool do_useCashItem_2664(CPC* ch, CItem* item, CNetMsg& msg, bool bprolong, int extra1);		// 미감정 초보자용 아이템
bool do_useCashItem_2665(CPC* ch, CItem* item, CNetMsg& msg, bool bprolong, int nIndexClothes);		// 감정 초보자용 아이템
#endif // LIMIT_ITEM_M35
// >>
bool do_useCashItem_1540(CPC* ch, CItem* item, CNetMsg& msg, int nIndexClothes);	// 분리주문서
#endif // ITEM_COMPOSITE

#ifdef ENABLE_PET
bool do_useCashItem_1519(CPC* ch, CItem* item, CNetMsg& msg, int nPetIndex);
#endif // ENABLE_PET

void do_ItemLendWeapon(CPC* ch, CNetMsg& msg);

#ifdef LEVELDOWN_SCROLL
void do_ItemUseLevelDown(CPC* ch, CNetMsg& msg);
#endif	// LEVELDOWN_SCROLL

#ifdef EVENT_TLD_2007_SONGKRAN
bool do_ItemUse_2146_2147( CPC* ch, const CItemProto* itemproto );
#endif //EVENT_TLD_2007_SONGKRAN

#ifdef DRATAN_CASTLE
bool do_ItemUse_Castle_Guard(CPC* ch, CItem* item);
#endif // DRATAN_CASTLE

#ifdef CASH_EXCHANGE_EQUIP
void do_ItemUseExchagneEquip( CPC* ch, CNetMsg& msg );
#endif // CASH_EXCHANGE_EQUIP

//펫 색깔 변경
#ifdef PET_DIFFERENTIATION_ITEM
bool do_useCashItem_PetColorChange( CPC* ch, const CItemProto* itemproto );  // pet 색깔 변경하기
#endif //PET_DIFFERENTIATION_ITEM

#ifdef PET_TURNTO_NPC
bool do_useCashItem_PetTurnToNPC( CPC* ch, const CItemProto* itemproto );  // pet NPC로 변신 하기
#endif //PET_TURNTO_NPC

// 대전용 아이템
#ifdef COMPETITION_ITEM  
bool do_useCashItem_CompetitionItem( CPC* ch, CItem* pItem );
#endif // COMPETITION_ITEM


#ifdef PET_HPRECOVER_ITEM		// 펫 HP 회복 
bool do_useCashItem_PetHPRecover( CPC* ch, const CItemProto* itemproto );
#endif // PET_HPRECOVER_ITEM
//
//#ifdef USE_WAREHOUSE		// 창고 이용 주문서
//bool do_useCashItem_UseWarhouse( CPC* ch, const CItemProto* itemproto );
//#endif // USE_WAREHOUSE
//
//#ifdef USE_GROCERY		// 잡화상 이용 주문서
//bool do_useCashItem_UseGrocery( CPC* ch, const CItemProto* itemproto );
//#endif // USE_GROCERY
//
//	
//#ifdef SHOW_HP		// 써치 라이프 - 캐릭터의 HP를 볼수 있음
//bool do_useCashItem_ShowHP( CPC* ch, const CItemProto* itemproto );
//#endif // SHOW_HP

#if defined ( USE_WAREHOUSE ) || defined ( USE_GROCERY ) || defined ( SHOW_HP )
bool do_useCashItem_WareHouse_Grocery_ShowHP( CPC* ch, CItem* pItem );
#endif // #if defined ( USE_WAREHOUSE ) || defined ( USE_GROCERY ) || defined ( SHOW_HP )

bool do_useEventItem_HalloweenCandy( CPC* ch, CItem* pItem );

#ifdef NEW_CASHITEM_USA_2008
void do_itemUse_WarpDoc(CPC* ch, CNetMsg& msg);
#endif // NEW_CASHITEM_USA_2008



bool do_ItemUse_2887(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2888(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2889(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2890(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2891(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2892(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2893(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_2968(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_GoldBox(CPC* ch, const CItemProto* itemproto);
bool do_ItemUse_3769(CPC* ch, const CItemProto* itemproto);


#ifdef EVENT_NOM_MOVIE
bool do_useEventItem_Nom(CPC* ch, CItem* pItem);
#endif

void do_Item(CPC* ch, CNetMsg& msg)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(ch);
	}
#endif // DRATAN_CASTLE

	// 죽거나 교환중이면 무시
	if (DEAD(ch) || ch->m_exchange != NULL || ch->m_currentSkill != NULL || ch->m_personalshop != NULL)
		return ;
	
	msg.MoveFirst();
	
	unsigned char subtype;
	msg >> subtype;
	
	switch (subtype)
	{
	case MSG_ITEM_USE:
		do_ItemUse(ch, msg);
		break;
		
	case MSG_ITEM_TAKE:
		do_ItemTake(ch, msg);
		break;
		
	case MSG_ITEM_THROW:
		do_ItemThrow(ch, msg);
		break;
		
	case MSG_ITEM_ARRANGE:
		do_ItemArrange(ch, msg);
		break;
		
	case MSG_ITEM_DELETE:
		do_ItemDelete(ch, msg);
		break;
		
	case MSG_ITEM_WEAR:
		do_ItemWear(ch, msg);
		break;
		
	case MSG_ITEM_SWAP:
		do_ItemSwap(ch, msg);
		break;
		
	case MSG_ITEM_BUY:
		do_ItemBuy(ch, msg);
		break;
		
	case MSG_ITEM_SELL:
		do_ItemSell(ch, msg);
		break;
		
	case MSG_ITEM_UPGRADE_REQ:
		do_ItemUpgradeReq(ch, msg);
		break;
		
	case MSG_ITEM_REFINE_REQ:
		do_ItemRefinReq(ch, msg);
		break;

	case MSG_ITEM_OPTION_ADD_REQ:
		do_ItemOptionAddReq(ch, msg);
		break;

	case MSG_ITEM_OPTION_DEL_REQ:
		do_ItemOptionDelReq(ch, msg);
		break;

	case MSG_ITEM_PROCESS_REQ:
		do_ItemProcessReq(ch, msg);
		break;

	case MSG_ITEM_MAKE_REQ:
		do_ItemMakeReq(ch, msg);
		break;

	case MSG_ITEM_MIX_REQ:
		do_ItemMixReq(ch, msg);
		break;

	case MSG_ITEM_ARCANE_REQ:
		do_ItemArcaneReq(ch, msg);
		break;

	case MSG_ITEM_CHANGEWEAPON_REQ:
#ifdef ENABLE_CHANGEJOB
		do_ItemChangeWeaponReq(ch, msg);
#endif
		break;

	case MSG_ITEM_PROCESS_NPC:
		do_ItemProcessNPC(ch, msg);
		break;

	case MSG_ITEM_ADD_BOOSTER:
		do_ItemAddBooster(ch, msg);
		break;

#ifdef ENABLE_WAR_CASTLE
	case MSG_ITEM_MIX_WARITEM:
		do_ItemMixWarItem(ch, msg);
		break;
#endif

#ifdef FEEITEM
	case MSG_ITEM_USE_PROLONG:
		do_ItemUse(ch, msg, true);
		break;
#endif

#ifdef LEND_ITEM_SYSTEM
	case MSG_ITEM_LEND_WEAPON:
		do_ItemLendWeapon(ch, msg);
		break;
#endif // LEND_ITEM_SYSTEM

#ifdef LEVELDOWN_SCROLL
	case MSG_ITEM_LEVELDOWN:
		do_ItemUseLevelDown(ch, msg);
		break;
#endif	// LEVELDOWN_SCROLL

#ifdef ADULT_SERVER_NEWITEM
	case MSG_ITEM_TARGET:
		do_ItemTarget(ch, msg);
		break;
#endif // ADULT_SERVER_NEWITEM

#ifdef CASH_EXCHANGE_EQUIP
	case MSG_ITEM_EXCHANGE_EQUIP_REQ:
		do_ItemUseExchagneEquip( ch, msg );
		break;
#endif // CASH_EXCHANGE_EQUIP

#ifdef USE_GROCERY
	case MSG_ITEM_USE_GROCERY_BUY:
		do_GroceryItemBuy(ch, msg);
		break;
	case MSG_ITEM_USE_GROCERY_SELL:
		do_GroceryItemSell(ch, msg);
		break;
#endif // USE_GROCERY

#ifdef NEW_CASHITEM_USA_2008
	case MSG_ITEM_USE_WARPDOC:
		do_itemUse_WarpDoc(ch, msg);
		break;
#endif // NEW_CASHITEM_USA_2008

#ifdef LAKA_PRESSCORPS
	case MSG_ITEM_USE_PRESSCORPS:
		do_itemUse_PressCorps(ch, msg);
		break;
#endif // LAKA_PRESSCORPS
		
	default:
		break;
	}

	// 아이템 관련 메시지 처리후 착용 정보에 따라 소환/강신 취소
	CItem* pItemWeapon = ch->m_wearing[WEARING_WEAPON];
	bool bCancelSummonEvocation = false;
	if (!pItemWeapon || !pItemWeapon->IsWeaponType())
	{
		// 무기 미착용
		bCancelSummonEvocation = true;
	}
	else
	{
		// 무기 착용
		// 직업에 따라
		switch (ch->m_job2)
		{
		case JOB_2ND_ELEMENTALLIST:
			// 엘리멘탈리스트는 폴암 아니면 소환 취소
			if (pItemWeapon->m_itemProto->m_subtypeIdx != IWEAPON_POLEARM)
				bCancelSummonEvocation = true;
			break;

		case JOB_2ND_SPECIALLIST:
			// 스페셜리스트는 사이드 아니면 강신 취소
			if (pItemWeapon->m_itemProto->m_subtypeIdx != IWEAPON_SCYTHE)
				bCancelSummonEvocation = true;
			break;

		default:
			// 전직 전에는 폴암 사이드 둘다 됨
			if (pItemWeapon->m_itemProto->m_subtypeIdx != IWEAPON_POLEARM && pItemWeapon->m_itemProto->m_subtypeIdx != IWEAPON_SCYTHE)
				bCancelSummonEvocation = true;
			break;
		}
	}
	if (bCancelSummonEvocation)
	{
		// 소환 취소
		while (ch->m_elementalList)
			ch->UnsummonElemental(ch->m_elementalList);
		// 강신 취소
		if (ch->m_evocationType != EVOCATION_NONE)
			ch->Unevocation();
	}
}

void do_ItemUse(CPC* ch, CNetMsg& msg, bool bprolong)
{
	msg.MoveFirst();
	
	unsigned char subtype;
	char tab, row, col;
	int item_idx;
	int extra1;
	
	msg >> subtype
		>> tab >> row >> col >> item_idx
		>> extra1;
	
	CNetMsg rmsg;
	CInventory* inven;
	CItem* item;
	const CItemProto* itemproto;

	// 아이템 찾기
	inven = GET_INVENTORY(ch, tab);
	if (inven == NULL)
		goto SKIP_USE;
	item = inven->GetItem(row, col);
	if (item == NULL || item->m_index != item_idx || item->m_wearPos != -1)
		goto SKIP_USE;
	
	itemproto = item->m_itemProto;
	
#ifdef LIMIT_ITEM_M35	// 35레벨 초과시 사용금지
	switch(itemproto->m_index)
	{
	case 2666:		// 초보자용 돋보기
	case 2667:		// 초보자용 부활 주문서
	case 2668:		// 초보자용 플래티늄 아드레날린
	case 2658:		// 초보자용 플래티늄 아이리스의 축복
	case 2659:		// 초보자용 플래티늄 숙련의 묘약
		if(ch->m_level > 35)
		{
			goto SKIP_USE;
		}
		break;

	default:
		break;
	}
#endif // LIMIT_ITEM_M35
	
#ifdef EVENT_PHOENIX	// 피닉스 이벤트 
	// 피닉스 부활 아이템 100레벨을 초과하면 사용 못함
	switch(itemproto->m_index)
	{
	case 3218:
		if(ch->m_level > 100)
		{
			goto SKIP_USE;
		}
		break;

	default:
		break;
	}
#endif // EVENT_PHOENIX
	
#ifndef FEEITEM
	if(itemproto->m_flag & ITEM_FLAG_CASH) goto SKIP_USE;
#endif
	// 스톤 걸려도 스톤 치료물약은 되게
	if (ch->m_assist.m_state & AST_STONE && itemproto->m_index != 874 && itemproto->m_index != 3582 )
		goto SKIP_USE;
	
	// 수량 확인
	if (item->Count() < 1)
	{
		ItemDeleteMsg(rmsg, item);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, item, true, true);
		goto SKIP_USE;
	}
	
#ifdef PRIMIUM_MEMORYBOOK
	// 개봉 확인
	if (itemproto->m_index == PRIMIUM_MEMORYBOOK)
	{
		if (item->m_used == -1)
		{	// 처음 사용시
			item->m_used = gserver.m_gameTime + item->m_itemProto->m_num3 * 60 * 60;

			// 개봉 알림
			SysMsg(rmsg, MSG_SYS_MEMPOSPLUS_OPEN);
			SEND_Q(rmsg, ch->m_desc);

			ItemUpdateMsg(rmsg, item, 0);
			SEND_Q(rmsg, ch->m_desc);
		}
		goto SKIP_USE;
	}
#endif	// PRIMIUM_MEMORYBOOK

	// Item 효과 적용
	switch (itemproto->m_typeIdx)
	{
	case ITYPE_ONCE:				// 일회용 아이템
		
		switch (itemproto->m_subtypeIdx)
		{
		case IONCE_WARP:
			switch (itemproto->m_num0)
			{
			case IONCE_WARP_RETURN:			// 귀환 스크롤
				if (!do_ItemUse_IONCE_WARP_RETURN(ch, itemproto))
					goto SKIP_USE;
				break;

			// 060227 : bs : 파티리콜 : 사용후에도 수량 감소 없음
			case IONCE_WARP_PARTYRECALL:
				do_ItemUsePartyRecall(ch, item);
				goto SKIP_USE;
				
			default:
				goto SKIP_USE;
			}
			break;

		case IONCE_CHANGE_DOC:
			if (!do_ItemUse_IONCE_CHANGE_DOC(ch, itemproto))
				goto SKIP_USE;
			break;

		case IONCE_QUEST_SCROLL:
			if (!do_ItemUse_IONCE_QUEST_SCROLL(ch, itemproto, extra1))
				goto SKIP_USE;
			break;

#ifdef FEEITEM
		case IONCE_CASH:

#if defined ( USE_WAREHOUSE ) || defined ( USE_GROCERY ) || defined ( SHOW_HP )
			switch( itemproto->m_index )
			{
			case 2455: case 2456: case 2457:
			case 2458: case 2459: case 2460:
			case 2461: case 2462: case 2463:
			case 2606: case 2607: case 2608:
#ifdef NEW_CASHITEM_USA_2008
			case 2867: case 2868:
#endif // NEW_CASHITEM_USA_2008
				if( do_useCashItem_WareHouse_Grocery_ShowHP( ch, item ) )
				{
					ItemUseMsg( rmsg, ch, tab, row, col, itemproto->m_index, extra1 );
					SEND_Q( rmsg, ch->m_desc );

					GAMELOG << init("CASH_ITEM_WAREHOUSE_GROCERY_SHOWHP", ch )
							<< itemlog(item) << delim
							<< end;
					goto SKIP_USE;
				}				
				break;
			}

#endif // defined ( USE_WAREHOUSE ) || defined ( USE_GROCERY ) || defined ( SHOW_HP )

#ifdef	ALTERNATE_MERCHANT   // 아이템 대행 상인 사용
			switch( itemproto->m_index )
			{
			case 2565: case 2566: case 2567:
				if( do_useCashItem_AlternateMerchant( ch, item ) )
				{
					ItemUseMsg( rmsg, ch, tab, row, col,itemproto->m_index, extra1 );
					SEND_Q( rmsg, ch->m_desc );

					GAMELOG << init( "CASH_ITEM_USE_ALTERNATE_MERCHANT", ch )
							<< itemlog( item ) << delim
							<< end;

					goto SKIP_USE;
				}
				break;
			}
#endif // ALTERNATE_MERCHANT
			
			if( !do_useCashItem(ch, item, msg, bprolong, extra1) )
				return;
			break;
		case IONCE_SUMMON:
			if (!do_ItemUse_IONCE_SUMMON(ch, item))
				goto SKIP_USE;
			break;
#endif


		case IONCE_BOX:
			switch(itemproto->m_index)
			{
			case 2046: case 2632:
#if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 
				if(!do_ItemUse_2046(ch, itemproto, extra1))
					return ;
#endif	// #if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 
				break;
#ifdef MONSTER_COMBO_COIN_CHANGE
			case 2740:
			case 2741:
			case 2742:
			case 2743:
			case 2744:
				if(do_ItemUse_Coin(ch, itemproto, extra1) == false)
				{
					return;
				}
				break;
#endif // MONSTER_COMBO_COIN_CHANGE
#if defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)
			case 2747:
				if(do_ItemUse_SakuraBox(ch, itemproto, extra1) == false)
				{
					return;
				}
				break;
#endif //defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)
#ifdef MONSTER_COMBO_GOLDBOX
			case 3576:
			case 3713:
			case 3714:
			case 3715:
			case 3716:
			case 3717:
			case 3718:
			case 3762:
				if(!do_ItemUse_GoldBox(ch, itemproto))
					return ;
				break;
#endif // MONSTER_COMBO_GOLDBOX
			default:
				goto SKIP_USE;
			}
			break;
		case IONCE_ETC:
#ifdef COMPETITION_ITEM
			if( itemproto->m_index == EXPERIENCE_COMPETITION_ITEM 
				|| itemproto->m_index == PERIOD_COMPETITION_ITEM 
#ifdef LC_JPN
				|| itemproto->m_index == 2500   // 일본은 2500번 아이템도 광속임
#endif // LC_JPN
				|| itemproto->m_index == 2609
				)
			{
				if(do_useCashItem_CompetitionItem( ch, item ))
				{
					ItemUseMsg(rmsg, ch, tab, row, col, itemproto->m_index, extra1);
					SEND_Q(rmsg, ch->m_desc);

					GAMELOG << init("CASH_ITEM_USE", ch)
							<< itemlog(item) << delim
							<< end;	
				}
				goto SKIP_USE;
			}
#endif // COMPETITION_ITEM
#ifdef IRIS_POINT
			if ( itemproto->m_index == 3764 )
			{
				if( item->Count() > 0 )
				{
					CNetMsg ipointMsg;
					ConnIPointAddMsg( ipointMsg, ch->m_desc->m_index, ch->m_index,  MSG_IPOINT_TYPE_USEITEM, item->Count() );
					SEND_Q(ipointMsg,gserver.m_connector );

					ItemDeleteMsg(rmsg, item);
					SEND_Q(rmsg, ch->m_desc);
					RemoveFromInventory(ch, item, true, true);
					goto SKIP_USE;

				}
				break;			
			} 			
#endif //IRIS_POINT
			
			if( itemproto->m_index == 3769 )			// 독일 보너스 패키지
			{
				if( !do_ItemUse_3769( ch , itemproto ) )
					goto SKIP_USE;
				break;
			}

			if (!do_ItemUse_IONCE_ETC(ch, itemproto, extra1))
				goto SKIP_USE;
			break;

		default:
			goto SKIP_USE;

		}

		break;

	case ITYPE_POTION:
		{
			// 눈물
			if (itemproto->m_subtypeIdx == IPOTION_TEARS)
			{	
				switch (itemproto->m_num0)
				{
					// 구원의 눈물
				case IPOTION_TEARS_TYPE_SAFE:
					
					if (ch->m_pkPenalty < -15)
					{
						ch->m_pkPenalty++;
						ch->m_bChangeStatus = true;
					}
					else
						goto SKIP_USE;

					break;

					// 용서의 눈물
				case IPOTION_TEARS_TYPE_FORGIVE:

					if (ch->m_exp < 0)
					{
						LONGLONG plusexp = GetLevelupExp(ch->m_level) * 5 / 100;
						ch->m_exp += plusexp;

						if (ch->m_exp > 0)
							ch->m_exp = 0;

						ch->m_bChangeStatus = true;
					}
					else
						goto SKIP_USE;

					break;

				default:
					goto SKIP_USE;
				}
			}
			else
			{	
#ifdef FEEITEM
				switch (itemproto->m_index)
				{
				case 844:		// 경험치복구 주문서
				case 845:		// 숙련도복구 주문서
				case 2035:		// 럭키 경험치복구 주문서
				case 2036:		// 럭키 숙련도복구 주문서
					// 부활주문서의 이펙트 인덱스 / 각레벨
					if( ch->m_assist.Find(184, 2) )
					{
						CNetMsg rmsg;
						ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_EXP_SP);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;

					}
					else if( ch->m_assist.Find(185, 2) )
					{
						CNetMsg rmsg;
						ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_SP_EXP);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
// TODO : DELETE
//					else if( ch->m_assist.Find(184, 2) && ch->m_assist.Find(185, 2) )
//					{
//						// 부활 주문서가 사용중이므로 거시기 사용 못함
//						CNetMsg rmsg;
//						if(itemproto->m_index == 844)
//						{
//							ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_REVIVAL_EXP);
//							SEND_Q(rmsg, ch->m_desc);
//							goto SKIP_USE;
//						}
//						else
//						{
//							ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_REVIVAL_SP);
//							SEND_Q(rmsg, ch->m_desc);
//							goto SKIP_USE;
//						}
//					}
					break;


				}
#endif // #ifdef FEEITEM

				switch (itemproto->m_index)
				{	
#ifdef NEW_CASHITEM_USA_2008
				case 2855:											// 플래티늄 드랍 증폭제
					if(ch->m_assist.FindByItemIndex(884)
					|| ch->m_assist.FindByItemIndex(838)
					|| ch->m_assist.FindByItemIndex(972)
					|| ch->m_assist.FindByItemIndex(1629)
					|| ch->m_assist.FindByItemIndex(2856))
						goto SKIP_USE;
					break;

				case 2856:											// 플래티늄 행운의 스크롤
					if(ch->m_assist.FindByItemIndex(884)
					|| ch->m_assist.FindByItemIndex(838)
					|| ch->m_assist.FindByItemIndex(972)
					|| ch->m_assist.FindByItemIndex(1629)
					|| ch->m_assist.FindByItemIndex(2855))	
						goto SKIP_USE; 
					 break;
#endif // NEW_CASHITEM_USA_2008

#if defined (PLATINUM_SKILL_POTION_ITEM ) || defined (SKILL_POTION_ITEM)
				case 2452:		// 숙련의 묘약
					if (ch->m_assist.FindByItemIndex(2453))
					{	// 플래티늄 훈련 주문서와 중복 불가
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 2453);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					break;
				case 2453:		// 플래티늄 숙련의 묘약
#ifdef LIMIT_ITEM_M35
				case 2659:		// 초보자용 플래티늄 숙련의 묘약
#endif // LIMIT_ITEM_M35
					if (ch->m_assist.FindByItemIndex(2452))
					{	// 훈련 주문서와 중복 불가
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 2452);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					break;
#endif // defined (PLATINUM_SKILL_POTION_ITEM ) || defined (SKILL_POTION_ITEM)
				case 2139:		// 훈련 주문서
					if (ch->m_assist.FindByItemIndex(2140))
					{	// 플래티늄 훈련 주문서와 중복 불가
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 2140);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					break;
				case 2140:		// 플래티늄 훈련 주문서
					if (ch->m_assist.FindByItemIndex(2139))
					{	// 훈련 주문서와 중복 불가
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 2139);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					break;
				case 836:		// 수박
					// 경험치 증폭제는 수박과 불가
					if (ch->m_assist.FindByItemIndex(882))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 882);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					break;

				case 837:		// 참외
					// SP 증폭제는 참외와 불가
					if (ch->m_assist.FindByItemIndex(883))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 883);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					break;

				case 838:		// 자두
					// 아이템 증폭제는 자두와 불가
					if (ch->m_assist.FindByItemIndex(884))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 884);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}

#ifdef NEW_CASHITEM_USA_2008
					// 플래티늄 드롭 증폭제와 불가
					if (ch->m_assist.FindByItemIndex(2855))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 2855);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}

					// 플래티늄 행운의 스크롤과 불가
					if (ch->m_assist.FindByItemIndex(2856))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 2856);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
#endif // NEW_CASHITEM_USA_2008
					break;

				case 882:		// 경험치
					// 증폭제는 중복 복용 불가
					if (ch->m_assist.FindByItemIndex(itemproto->m_index))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, itemproto->m_index);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					// 경험치 증폭제는 수박과 불가
					if (ch->m_assist.FindByItemIndex(836))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 836);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					break;

				case 883:		// SP
					// 증폭제는 중복 복용 불가
					if (ch->m_assist.FindByItemIndex(itemproto->m_index))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, itemproto->m_index);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					// SP 증폭제는 참외와 불가
					if (ch->m_assist.FindByItemIndex(837))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 837);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					break;

				case 884:		// 아이템
					// 증폭제는 중복 복용 불가
					if (ch->m_assist.FindByItemIndex(itemproto->m_index))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, itemproto->m_index);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					// 아이템 증폭제는 자두와 불가
					if (ch->m_assist.FindByItemIndex(838))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 838);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}

#ifdef NEW_CASHITEM_USA_2008
					// 플래티늄 드랍 증폭제와 불가
					if (ch->m_assist.FindByItemIndex(2855))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 2855);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}

					// 플래티늄 행운의 스크롤과 불가
					if (ch->m_assist.FindByItemIndex(2856))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 2856);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
#endif // NEW_CASHITEM_USA_2008
					break;

				case 885:		// 나스
					// 증폭제는 중복 복용 불가
					if (ch->m_assist.FindByItemIndex(itemproto->m_index))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, itemproto->m_index);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					break;

				case 972:
				case 1629:
#ifdef NEW_CASHITEM_USA_2008
					// 플래티늄 드랍 증폭제와 불가
					if (ch->m_assist.FindByItemIndex(2855))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 2855);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}

					// 플래티늄 행운의 스크롤과 불가
					if (ch->m_assist.FindByItemIndex(2856))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 2856);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
#endif // NEW_CASHITEM_USA_2008
					break;

				case 1288:		// 아이리스의 축복
				case 1416:		// 플래티늄 아이리스의 축복
#ifdef LIMIT_ITEM_M35
				case 2658:		// 초보자용 플래티늄 아이리스의 축복
#endif // LIMIT_ITEM_M35
				case 2032:		// 럭키 아이리스의 축복
					if( ch->m_assist.FindBySkillIndex(348) )
					{
						if( !extra1 )
						{
							ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_EXPUP);
							//>> tab >> row >> col >> item_idx
							rmsg << tab
								 << row
								 << col
								 << item_idx;
							SEND_Q(rmsg, ch->m_desc);
							goto SKIP_USE;
						}
					}
					break;

#ifdef EVENT_PHOENIX	// 피닉스 부활 아이템
				case 3218:
					if( ch->m_assist.FindByItemIndex(3218) )
					{
						goto SKIP_USE;
					}
					break;
#endif // EVENT_PHOENIX					

				}

				// 경험의 묘약은 아이리스를 사용 중일때 불가능
				if (itemproto->m_num0 == 348)
				{
					if (ch->m_assist.FindBySkillIndex(349))
					{
						SysCannotDuplcationMsg(rmsg, itemproto->m_index, 1288);
						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
				}

				if( itemproto->m_index >= 2482 && itemproto->m_index <= 2487 )
				{
					if( do_useEventItem_HalloweenCandy( ch, item ) )
					{
						goto SKIP_SKILL;
					}
					else 
						goto SKIP_USE;
				}

#ifdef EVENT_NOM_MOVIE
				// 2885 : 복권, 2886 : 영화 티켓
				if (itemproto->m_index == 2885 || itemproto->m_index == 2886)
				{
					do_useEventItem_Nom(ch, item);
				}
#endif
				CSkill* skill = NULL;
				
				// 포션중에 IPOTION_UP 은 아이템 플래그에서 스킬레벨을 가져온다
				if (itemproto->m_subtypeIdx == IPOTION_UP)
					skill = gserver.m_skillProtoList.Create(itemproto->m_num0, item->m_flag);
				else
					skill = gserver.m_skillProtoList.Create(itemproto->m_num0, itemproto->m_num1);
				
				if (skill == NULL)
					goto SKIP_USE;

				// 트루시잉 코덱스와 상급 트루시잉 코덱스
				if (itemproto->m_index == 677 || itemproto->m_index == 3579)
				{
					if (ch->IsInPeaceZone(false) )
					{
						CNetMsg rmsg;
						delete skill;
						rmsg.Init(MSG_EXTEND);
						rmsg << MSG_EX_STRING
							 << (unsigned char) MSG_EX_STRING_OUTPUT_SYS
							 << 393
							 << 0;

						SEND_Q(rmsg, ch->m_desc);
						goto SKIP_USE;
					}
					ApplyItem677_or_3579(ch, skill, itemproto->m_index);
				}
				else
				{
					// 060227 : bs : 중복안되는 스킬인지 검사
					if ((skill->m_proto->m_flag & SF_NOTDUPLICATE) && ch->m_assist.FindBySkillIndex(skill->m_proto->m_index))
					{
						delete skill;
						goto SKIP_USE;
					}
					// 확장 포션 : 듀얼 사용중일때 따로 따로 수행 못함 
					if (( skill->m_proto->m_index == 350 || skill->m_proto->m_index == 351 ) && ch->m_assist.FindBySkillIndex(352) )
					{
						delete skill;
						goto SKIP_USE;
					}
					bool bApply;
#ifdef ATTACK_PET
					if( skill->m_proto->CheckSorcererFlag(SSF_APP_APET) )
					{
						if( ch->GetAPet() == NULL )
						{
							delete skill;
							goto SKIP_USE;							
						}
						ApplySkill(ch, ch->GetAPet() , skill, itemproto->m_index, bApply);
					}
					else
#endif //ATTACK_PET
					{
						ApplySkill(ch, ch, skill, itemproto->m_index, bApply);
					}

					if (!bApply)
					{
						delete skill;
						goto SKIP_USE;
					}

					if( itemproto->m_index == 846	// 부활 주문서
#ifdef LIMIT_ITEM_M35
						|| itemproto->m_index == 2667 // 초보자용 부활 주문서
#endif // LIMIT_ITEM_M35
						)
					{
						// 숙련도 / 경험치 주문서 버프 삭제 
						ch->m_assist.CureByItemIndex(844);
						ch->m_assist.CureByItemIndex(845);
						ch->m_assist.CureByItemIndex(2035);
						ch->m_assist.CureByItemIndex(2036);
					}
				}
				delete skill;
			}

			// 포션 사용 이펙트 메시지
			CNetMsg effectMsg;
			EffectItemMsg(effectMsg, ch, item);
			ch->m_pArea->SendToCell(effectMsg, ch, true);
		}
		break;

	case ITYPE_ETC:
#ifdef ENABLE_PET
		if (itemproto->m_subtypeIdx == IETC_PRODUCT)
		{
			if (!do_ItemUse_IETC_PRODUCT(ch, itemproto))
				goto SKIP_USE;
		}
		else
			goto SKIP_USE;
#endif
		break;

	default:
		goto SKIP_USE;
	}

SKIP_SKILL:
	if( (item->m_itemProto->m_flag & ITEM_FLAG_CASH) )
	{
		GAMELOG << init("CASH_ITEM_USE", ch)
				<< itemlog(item) << delim
				<< end;
	}

	// 아이템 사용하고
	ItemUseMsg(rmsg, ch, tab, row, col, item_idx, extra1);
	SEND_Q(rmsg, ch->m_desc);

#ifdef EVENT_PHOENIX	// 피닉스 부활 아이템 
	// 피닉스 부활 아이템은 삭제하지 않는다.
	if ( itemproto->m_index == 3218 )
	{
		return;
	}
#endif // EVENT_PHOENIX					

	// Item 수량 변경
	DecreaseFromInventory(ch, item, 1);
	if (item->Count() > 0)
	{
		ItemUpdateMsg(rmsg, item, -1);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		ItemDeleteMsg(rmsg, item);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, item, true, true);
	}

SKIP_USE:
	return ;
}

#ifdef FEEITEM
bool do_useCashItem(CPC* ch, CItem* item, CNetMsg& msg, bool bprolong, int extra1)
{
	// 스텟 초기화
	CNetMsg rmsg;

	if(item->m_itemProto->m_index == 847)
	{
		ch->m_statpt_remain += ch->m_statpt_str + ch->m_statpt_dex + ch->m_statpt_int + ch->m_statpt_con;

		ch->m_statpt_str = ch->m_statpt_dex = ch->m_statpt_int = ch->m_statpt_con = 0;
		ch->InitStat();

		StatPointRemainMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);

		ch->CalcStatus(true);
		ch->m_hp = ch->m_maxHP;
		ch->m_mp = ch->m_maxMP;

		ch->SaveNow(false);

		StatPointRemainMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);

		return true;
	}

	// 전투스킬 초기화
	if(item->m_itemProto->m_index == 848)
	{
		ch->InitSkill(&ch->m_activeSkillList);
		ch->InitSkill(&ch->m_passiveSkillList, true);
		ch->m_job2 = 0;
		ch->InitStat();

		ch->CalcStatus(true);

		ch->SaveNow(false);

		SkillListMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);

		ChangeJobResetRepMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
		

		return true;
	}

	// 특수 스킬 초기화
	if(item->m_itemProto->m_index == 849)
	{
		ch->InitSSkill();
		ch->CalcStatus(true);
		ch->SaveNow(false);

		SSkillListMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);

		return true;
	}

	// 스킬 초기화
	if(item->m_itemProto->m_index == 850)
	{
		ch->InitSkill(&ch->m_activeSkillList);
		ch->InitSkill(&ch->m_passiveSkillList, true);
		ch->InitSSkill();
		ch->m_job2 = 0;
		ch->InitStat();

		ch->CalcStatus(true);
		ch->SaveNow(false);

		SkillListMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
		SSkillListMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);

		ChangeJobResetRepMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
		return true;
	}

	// 능력치 초기화카드
	if(item->m_itemProto->m_index == 851)
	{
		ch->m_statpt_remain += ch->m_statpt_str + ch->m_statpt_dex + ch->m_statpt_int + ch->m_statpt_con;

		ch->m_statpt_str = ch->m_statpt_dex = ch->m_statpt_int = ch->m_statpt_con = 0;
		ch->InitStat();

		StatPointRemainMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);

		ch->CalcStatus(true);
		ch->m_hp = ch->m_maxHP;
		ch->m_mp = ch->m_maxMP;

		ch->InitSkill(&ch->m_activeSkillList);
		ch->InitSkill(&ch->m_passiveSkillList, true);
		ch->InitSSkill();
		ch->m_job2 = 0;

		ch->InitStat();

		ch->CalcStatus(true);
		ch->SaveNow(false);

		SkillListMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
		SSkillListMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
		ChangeJobResetRepMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
		return true;
	}

	//12. 메모리확장 카드
	if( item->m_itemProto->m_index == 839 || item->m_itemProto->m_index == 840 || item->m_itemProto->m_index == 841 
		|| item->m_itemProto->m_index == 2347 || item->m_itemProto->m_index == 2348 )
	{
		int itemUseTime = 0;

		switch(item->m_itemProto->m_index)
		{
		case 839:
#ifdef CHARSLOTEXT_CARD
			itemUseTime = usePeriodItem(&ch->m_charslotTime[extra1], &bprolong);
#endif//#ifdef CHARSLOTEXT_CARD
			break;
		case 840:	// 메모리 스크롤 확장
			itemUseTime = usePeriodItem(&ch->m_memposTime, &bprolong);
			break;
		case 841:	// 개인창고 확장
			itemUseTime = usePeriodItem(&ch->m_stashextTime, &bprolong);
			break;
//#ifdef EVENT_2007_PARENTSDAY
		case 2348:
			itemUseTime = usePeriodItem_7(&ch->m_memposTime, &bprolong );
			break;
		case 2347:
			itemUseTime = usePeriodItem_7( &ch->m_stashextTime, &bprolong );
			break;
//#endif // EVENT_2007_PARENTSDAY
		default:
			return false;
		}

		ItemProlongMsg(rmsg, item->m_itemProto->m_index, itemUseTime, bprolong);
		SEND_Q(rmsg, ch->m_desc);
		return !bprolong;
	}
	// 무기 교환 아이템 : 할일 인덱스
	if( item->m_itemProto->m_index == 975 )
	{
		int extra2;
		msg >> extra2;

		CItem* item = ch->m_invenNormal.GetItem(extra1);
		if (!item)
			return false;

		if (!item->IsWeaponType())
			return false;

		if (item->m_itemProto->m_subtypeIdx == extra2)
			return false;

		if (extra2 < 0 || extra2 >= 16)
			return false;

		int itemlevel = item->GetItemLevel(true);

		// 레벨, 한손검, 석궁, 스태프, 대검, 도끼, 숏스테프, 활, 단검, -1, -1, -1, 이도류, 완드, 사이드, 폴암 : 할일 : 소서러
		const static int matchTable[12][16] = {
			{1,48,530,600,12,558,356,50,528,-1,-1,-1,459,599,-1,-1},
			{5,53,666,628,51,601,357,56,529,-1,-1,-1,610,619,-1,-1},
			{9,55,637,629,52,602,358,57,532,-1,-1,-1,611,620,-1,-1},
			{13,107,638,630,105,603,359,106,533,-1,-1,-1,612,621,-1,-1},
			{17,180,639,631,306,604,360,185,534,-1,-1,-1,613,622,-1,-1},
			{21,322,640,632,307,605,361,341,535,-1,-1,-1,614,623,-1,-1},
			{25,323,641,633,308,606,362,342,536,-1,-1,-1,615,624,-1,-1},
			{29,324,642,634,309,607,363,343,537,-1,-1,-1,616,625,983,995},
			{33,325,643,635,310,608,364,344,538,-1,-1,-1,617,626,984,-996},
			{37,326,644,636,311,609,365,345,539,-1,-1,-1,618,627,985,997},
			{41,681,721,719,715,645,720,718,722,-1,-1,-1,716,717,986,998},
			{45,800,823,816,793,794,815,808,822,-1,-1,-1,801,809,987,999}
		};

		CItemProto* proto = NULL;
		int i;
		for (i = 0; i < 12; i++)
		{
			if (matchTable[i][0] == itemlevel)
			{
				proto = gserver.m_itemProtoList.FindIndex(matchTable[i][extra2 + 1]);
				break;
			}
		}

		if (proto == NULL)
			return false;

		CItem* newitem = gserver.m_itemProtoList.CreateItem(proto->m_index, -1, item->m_plus, item->m_flag, 1);
		if (!newitem)
			return false;

		GAMELOG << init("CASHITEM_CHANGE_WEAPON_WITHOUT OPTION", ch)
				<< "OLD" << delim
				<< itemlog(item) << delim;

		GAMELOG << "NEW" << delim
				<< itemlog(newitem) << delim;

		ItemDeleteMsg(rmsg, item);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, item, true, true);

		if (AddToInventory(ch, newitem, true, true))
		{
			ItemAddMsg(rmsg, newitem);
			SEND_Q(rmsg, ch->m_desc);
			GAMELOG << "ADD INVEN"
					<< end;
		}
		else
		{
			ch->m_pArea->DropItem(newitem, ch);
			newitem->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, newitem);
			ch->m_pArea->SendToCell(rmsg, GET_YLAYER(newitem), newitem->m_cellX, newitem->m_cellZ);
			GAMELOG << "DROP"
					<< end;
		}

		return true;
	}
	// 플래티늄 제련석 
#ifdef PLATINUM
	if ( item->m_itemProto->m_index == 1418 || item->m_itemProto->m_index == 2894 || item->m_itemProto->m_index == 3269)
	{
		// 인벤토리에 최소 1개의 빈공간이 있어야 한다
		if (ch->m_invenNormal.GetSpace() < 1)
		{
			SysFullInventoryMsg(rmsg, INVENTORY_NORMAL);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}

		int giveItemIndex=0;
		if( item->m_itemProto->m_index == 1418 )
			giveItemIndex=1419;
		else if( item->m_itemProto->m_index == 2894 )
			giveItemIndex=2895;
		else if( item->m_itemProto->m_index == 3269 )
			giveItemIndex=3270;

		// 인벤토리에 최소 무게가 보장되어야 한다.
		const CItemProto* pItemProto = gserver.m_itemProtoList.FindIndex(giveItemIndex);
		if (pItemProto == NULL)
			return false;
		if (ch->m_weight + pItemProto->m_weight >= (ch->m_maxWeight*3/2))
		{
			SysFullInventoryMsg(rmsg, INVENTORY_NORMAL);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}

		// 플러스를 붙인 플래티늄 제련석을 지급한다
		int rand = GetRandom(1, 100);

		// 5프로 +3, 15프로 +2, 나머지 +1
		//+1 60%, +2 25%, +3 15% 로 변경 /bw : 060613
		if( rand <= 15 )
		{
			rand = 3;
		}
		else if( rand <= 40 )
		{
			rand = 2;
		}
		else
		{
			rand = 1;
		}

		if( giveItemIndex == 3270 )
			rand = 3;			// +3 전용 제련 아이템

		if( !ch->GiveItem(giveItemIndex, rand, 0, 1) )
		{
			return false;
		}
		
		ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_PLATINUM_SPECIAL);
		SEND_Q(rmsg, ch->m_desc);
		return true;
	}
#endif // PLATINUM

#ifdef ITEM_COMPOSITE
	// 결합 주문서 미감정
	if (item->m_idNum == 1420)
	{
		return do_useCashItem_1420(ch, item, msg, bprolong, extra1);
	}

// << kjban 080129
#ifdef LIMIT_ITEM_M35
	if(item->m_idNum == 2664)
	{	// 미감정 초보자용 결합 주문서
		return do_useCashItem_2664(ch, item, msg, bprolong, extra1);
	}
	
	if (item->m_idNum == 2665)
	{	// 감정된 초보자용 결합 주문서
		return do_useCashItem_2665(ch, item, msg, bprolong, extra1);
	}
#endif // LIMIT_ITEM_M35
// >>

	// 감정된 결합 주문서 사용
	if (item->m_idNum == 1421)
		return do_useCashItem_1421(ch, item, msg, bprolong, extra1);

	if (item->m_idNum == 1540)
		return do_useCashItem_1540(ch, item, msg, extra1);
#endif // ITEM_COMPOSITE

#ifdef ENABLE_PET
	// 펫 마운트 취소 카드
	if (item->m_idNum == 1519)
		return do_useCashItem_1519(ch, item, msg, extra1);
	// 펫의 차별화 아이템( 펫의 색깔 변경 ) 
#ifdef PET_DIFFERENTIATION_ITEM
	bool bresult = false;
	switch( item->m_idNum )
	{
	case PET_RED_COLOR_ITEM:
	case PET_SCARLET_COLOR_ITEM:
	case PET_YELLOW_COLOR_ITEM:
	case PET_GREEN_COLOR_ITEM:
	case PET_BLUE_COLOR_ITEM:
	case PET_DEEP_BLUE_COLOR_ITEM:
	case PET_VIOLET_COLOR_ITEM:
		
//#if defined (LC_BRZ) || defined (LC_USA) || defined (LC_MAL) || defined(LC_GER) || defined(LC_HBK)
	case PET_BLACK_COLOR_ITEM:
	case PET_WHITE_COLOR_ITEM:
#ifdef NEW_CASHITEM_USA_2008
	case PET_NON_COLOR_ITEM:
#endif // NEW_CASHITEM_USA_2008
		

#ifdef EVENT_PHOENIX   // 피닉스 이벤트 
	// 피닉스의 결정
	case 3262:
#endif
	case PET_BLACK_HEART_COLOR_ITEM :
	case PET_WHITE_HEART_COLOR_ITEM :
	case PET_YELLOW_HEART_COLOR_ITEM :
		bresult = do_useCashItem_PetColorChange( ch, item->m_itemProto );
		return bresult;
	}
#endif //PET_DIFFERENTIATION_ITEM
		
#ifdef PET_TURNTO_NPC
	switch( item->m_idNum )	// 펫변신 관련
	{
	case 2604 :			// 루돌프 
	case 2763 : case 2764: case 2765 :		// 1차 npc 변신 주문서 size 포함
	case 2605 :			// 복구 아이템
		return do_useCashItem_PetTurnToNPC( ch, item->m_itemProto );
	}
#endif // PET_TURNTO_NPC

#ifdef PET_HPRECOVER_ITEM		// 펫 HP 회복 
	if (item->m_idNum == PET_HPRECOVER_ITEM )
		return do_useCashItem_PetHPRecover( ch, item->m_itemProto );
#endif //PET_HPRECOVER_ITEM
#endif // ENABLE_PET
//
//#ifdef USE_WAREHOUSE		// 창고 이용 주문서
//	if (item->m_idNum == USE_WAREHOUSE_ITEM1 || item->m_idNum == USE_WAREHOUSE_ITEM2 || item->m_idNum == USE_WAREHOUSE_ITEM3 )
//		return do_useCashItem_UseWarhouse( ch, item->m_itemProto );
//#endif // USE_WAREHOUSE
//
//#ifdef USE_GROCERY		// 잡화상 이용 주문서
//	if (item->m_idNum == USE_GROCERY_ITEM1 || item->m_idNum == USE_GROCERY_ITEM2 || item->m_idNum == USE_GROCERY_ITEM3 )
//		return do_useCashItem_UseGrocery( ch, item->m_itemProto );
//#endif // USE_GROCERY
//
//	
//#ifdef SHOW_HP		// 써치 라이프 - 캐릭터의 HP를 볼수 있음
//	if (item->m_idNum == SHOW_HP_ITEM1 || item->m_idNum == SHOW_HP_ITEM2 || item->m_idNum == SHOW_HP_ITEM3 )
//		return do_useCashItem_ShowHP( ch, item->m_itemProto );
//#endif // SHOW_HP

#ifdef FIRECRACKER_ITEM
	if( item->m_idNum == FIRECRACKER_ITEM )
	{
		EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_FIRECRACKER);
		SEND_Q( rmsg, ch->m_desc );
		ch->m_pArea->SendToCell(rmsg, ch, true);
		return true;
	}
#endif // FIRECRACKER_ITEM

	return false;
}

// bprolong은 연기신청 창을 띄울것인가로 바꿈
// return 값은 OUTPUTDAY
int usePeriodItem(int* useTime, bool* bprolong)
{
	bool reqProlong = false;

	if(*bprolong)
	{
		if( *useTime >= gserver.m_gameTime )
		{
			*useTime += LCYEAR * 2;
		}
		else
		{
			*useTime = gserver.m_gameTime + LCYEAR * 2;
		}
	}
	else
	{
		if( *useTime <= gserver.m_gameTime )
		{
			*useTime = gserver.m_gameTime + LCYEAR * 2;
		}
		else
		{
			reqProlong = true;
		}
	}

	*bprolong = reqProlong;
	int remain = *useTime - gserver.m_gameTime;
	if(  remain <= 0)
	{
		return 0;
	}
	else
	{
		remain = remain / OUTPUTDAY;
		if (remain == 0)
		{
			remain = 1;
		}
	}

	return remain;
}
int usePeriodItem_7(int* useTime, bool* bprolong)
{
	bool reqProlong = false;

	if(*bprolong)
	{
		if( *useTime >= gserver.m_gameTime )
		{
			*useTime += LCYEAR / 2;
		}
		else
		{
			*useTime = gserver.m_gameTime + LCYEAR / 2;
		}
	}
	else
	{
		if( *useTime <= gserver.m_gameTime )
		{
			*useTime = gserver.m_gameTime + LCYEAR / 2;
		}
		else
		{
			reqProlong = true;
		}
	}

	*bprolong = reqProlong;
	int remain = *useTime - gserver.m_gameTime;
	if(  remain <= 0)
	{
		return 0;
	}
	else
	{
		remain = remain / OUTPUTDAY;
		if (remain == 0)
		{
			remain = 1;
		}
	}

	return remain;
}
#endif // FEEITEM
bool do_QuestStart(CPC* ch, int questIndex)
{

	CNetMsg rMsg;

	// 10개 초과
#ifdef QUEST_DATA_EXTEND
	if (ch->m_questList.GetCountRun() >= QUEST_MAX_PC)
	{
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_MAX);
		SEND_Q(rMsg, ch->m_desc);
		return FALSE;
	}
#else // QUEST_DATA_EXTEND
	if (ch->m_questList.m_nCount >= QUEST_MAX_PC)
	{
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_MAX);
		SEND_Q(rMsg, ch->m_desc);
		return FALSE;
	}
#endif // QUEST_DATA_EXTEND

	CQuestProto* proto = gserver.m_questProtoList.FindProto(questIndex);

	if (!proto)
		return FALSE;

	// 시작 조건
	if (proto->m_startType == QSTART_LEVEL)
	{
		if (proto->m_startData != ch->m_level)
			return FALSE;
	}
/*
	int i;
	// 1회용 퀘스트 이면서 실행했는지
	if (proto->m_type[1] == QTYPE_REPEAT_ONCE)
	{
		for (i=0; i < QUEST_MAX_PC_COMPLETE; i++)
		{
			if (ch->m_questList.m_doneQuest[i] == -1)
				return FALSE;

			if (ch->m_questList.m_doneQuest[i] == proto->m_index)
			{
				QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_ONCE);
				SEND_Q(rMsg, ch->m_desc);
				return FALSE;
			}
		}
	}
*/
	// 수행중인가
#ifdef QUEST_DATA_EXTEND
	if (ch->m_questList.FindQuest(proto->m_index))
	{
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_ALREADY);
		SEND_Q(rMsg, ch->m_desc);
		return FALSE;
	}
#else // QUEST_DATA_EXTEND
	int idx = ch->m_questList.FindQuest(proto->m_index);
	if (idx != -1)
	{
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_ALREADY);
		SEND_Q(rMsg, ch->m_desc);
		return FALSE;
	}
#endif // QUEST_DATA_EXTEND
	
	// 필요 레벨 조건
	if (proto->m_needMinLevel > ch->m_level || proto->m_needMaxLevel < ch->m_level)
	{
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_LEVEL);
		SEND_Q(rMsg, ch->m_desc);
		return FALSE;
	}
	
	// 필요 직업 조건
	if (proto->m_needJob != -1 && proto->m_needJob != ch->m_job)
	{
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_START_JOB);
		SEND_Q(rMsg, ch->m_desc);
		return FALSE;
	}
		
	
#ifdef QUEST_DATA_EXTEND

	CQuest* pQuest = ch->m_questList.AddQuest(proto->m_index);
	if (pQuest == NULL)
		return false;
	ch->m_questList.SetQuestState(pQuest, QUEST_STATE_RUN);
	pQuest->SetComplete1(false);
	pQuest->SetComplete2(false);

	// Quest Start Log
	GAMELOG << init("QUEST START", ch)
			<< pQuest->GetQuestIndex()
			<< end;
	
	// Quest Start Msg
	QuestStartMsg(rMsg, pQuest);
	SEND_Q(rMsg, ch->m_desc);

#else // QUEST_DATA_EXTEND

	CQuest* quest = gserver.m_questProtoList.Create(proto->m_index);
	
	if (!quest)
		return FALSE;
	
	// 캐릭터 퀘스트 리스트에 Add
	idx = ch->m_questList.AddQuest(quest);
	if (idx < 0)
	{
		delete quest;
		return FALSE;
	}
	
	ch->m_questList.m_bQuest[idx] = true;
	ch->m_questList.SetComplete(idx, false);
	ch->m_questList.SetComplete2(idx, false);

	// Quest Start Log
	GAMELOG << init("QUEST START", ch)
			<< quest->m_proto->m_index
			<< end;
	
	// Quest Start Msg
	QuestStartMsg(rMsg, ch->m_questList.m_list[idx]);
	SEND_Q(rMsg, ch->m_desc);

#endif // QUEST_DATA_EXTEND
	
	return TRUE;	
}

bool do_QuestGiveUp(CPC* ch, int questIndex)
{
	
	CNetMsg rMsg;

	if(questIndex < 0)
		return FALSE;
	
	// Quest GiveUp Log
	GAMELOG << init("QUEST GIVEUP", ch)
		<< questIndex
		<< end;

#ifdef QUEST_DATA_EXTEND
	CQuest* pQuest = ch->m_questList.FindQuest(questIndex);
	if (!pQuest)
		pQuest = ch->m_questList.AddQuest(questIndex);
	if (pQuest)
		ch->m_questList.SetQuestState(pQuest, QUEST_STATE_ABANDON);
#else // QUEST_DATA_EXTEND
	for (int i=0; i < QUEST_MAX_PC_COMPLETE; i++)
	{
		// 비어 있는 곳에 순서대로..
		if (ch->m_questList.m_abandonQuest[i] == -1)
		{
			ch->m_questList.m_abandonQuest[i] = questIndex;
			break;
		}
	}				
#endif // QUEST_DATA_EXTEND
	
	QuestGiveUpMsg(rMsg, questIndex);
	
	SEND_Q(rMsg, ch->m_desc);
	
	return TRUE;
	
}

void do_ItemTake(CPC* ch, CNetMsg& msg)
{
	// 안보이면 무시
	if (!ch->m_bVisible)
		return ;
	
	char char_type;
	int char_idx, item_idx;
	
	msg >> char_type
		>> char_idx
		>> item_idx;

	CCharacter* takeChar;

	switch (char_type)
	{
	case MSG_CHAR_PC:
		if (ch->m_index != char_idx)
			return ;
		takeChar = ch;
		break;

#ifdef ENABLE_PET
	case MSG_CHAR_PET:
		if (!ch->GetPet() || !ch->GetPet()->IsSummon() || ch->GetPet()->m_index != char_idx || ch->GetPet()->GetOwner() != ch)
			return ;
		takeChar = ch->GetPet();
		break;
#endif // #ifdef ENABLE_PET
		
#ifdef ATTACK_PET
	case MSG_CHAR_APET:
		if(!ch->GetAPet() || !ch->GetAPet()->IsSummon() || ch->GetAPet()->m_index != char_idx || ch->GetAPet()->GetOwner() != ch)
			return;
		takeChar = ch->GetAPet();
		break;
#endif //ATTACK_PET

	default:
		return ;
	}
	
	int sx = takeChar->m_cellX - 1;
	int ex = takeChar->m_cellX + 1;
	int sz = takeChar->m_cellZ - 1;
	int ez = takeChar->m_cellZ + 1;
	
	int x, z;
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= takeChar->m_pArea->m_size[0])
			continue ;
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= takeChar->m_pArea->m_size[1])
				continue ;
			CItem* item;
			CItem* itemNext = takeChar->m_pArea->m_cell[x][z].m_listItem;
			while ((item = itemNext))
			{
				itemNext = item->m_pCellNext;

				if (ABS(GET_YLAYER(takeChar) - GET_YLAYER(item)) > 1)
					continue ;
				
				if (item->m_index == item_idx)
				{
					// 돈이고 파티이고 균등파티이거나 새파티이면 나눠먹기
					if (   item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index
						&& ch->IsParty()
						&& ( ch->m_party->GetPartyType(MSG_DIVITYPE_ITEM) == PARTY_TYPE_RANDOM || ch->m_party->GetPartyType(MSG_DIVITYPE_ITEM) == PARTY_TYPE_BATTLE) )
					{
						DivisionPartyMoney(ch, item);
						return;
					}

#ifdef RECOMMEND_SERVER_SYSTEM
					if (item->m_idNum == RECOMMEND_SERVER_POTION_INDEX)
					{
						do_ItemTake_RecommendPotion(ch, item);
						return ;
					}
#endif // RECOMMEND_SERVER_SYSTEM

#ifdef EVENT_WHITEDAY_2007
					bool bCantWDItem = false;
					if( item->m_idNum == 2133 ) //하늘색 사탕 타이탄 나이트 소서러만 획득 가능
					{
						if( ch->m_job == JOB_TITAN || ch->m_job == JOB_KNIGHT || ch->m_job == JOB_SORCERER )
						{ 
							bCantWDItem = true;	
						}
					}
					
					if( item->m_idNum == 2134 ) //분홍색 사탕 힐러 메이지 로그 획득 가능
					{
						if( ch->m_job == JOB_HEALER || ch->m_job == JOB_MAGE || ch->m_job == JOB_ROGUE )
						{
							bCantWDItem = true;	
						}
					}

					if( item->m_idNum == 2133 || item->m_idNum == 2134 )
					{
						if( !bCantWDItem )
						{
							CNetMsg smsg;
							SysMsg(smsg, MSG_SYS_MATCHSEX );
							SEND_Q(smsg, ch->m_desc);
							return;
						}
					}					
#endif

#ifdef EVENT_COLLECT_BUG
					switch (item->m_idNum)
					{
					case 1579:
					case 1580:
					case 1581:
					case 1582:
					case 1583:
					case 1584:
					case 1585:
					case 1586:
					case 1587:
					case 1588:
						do_ItemTake_EventCollectBug(ch, item);
						return ;

					default:
						break;
					}
#endif // EVENT_COLLECT_BUG

#ifdef EVENT_XMAS_2007
					switch (item->m_idNum)
					{
						// 크리스 마스 선물 중복 습득 불가
					case 2590: case 2591: case 2592: case 2593: case 2594: case 2595: case 2596:
						{
							int nRow, nCol;
							if( ch->m_invenNormal.FindItem( &nRow, &nCol, item->m_idNum, -1, -1 ) )
							{
								CNetMsg smsg;
								SysMsg(smsg, MSG_SYS_CANNOT_HAVE_DUPLICATE);	// 아이템을 중복해서 가질 수 없습니다.
								SEND_Q(smsg, ch->m_desc);
								return;
							}
						}break;
					}
#endif //EVENT_XMAS_2007
					
#ifdef TRIGER_SYSTEM
					// 트리거 전용 아이템이면
					if( (item->m_itemProto->m_flag & ITEM_FLAG_TRIGER) )
					{
						/*해당 레이드의 인벤토리에 넣어준다.*/
					}
					else
#endif // TRIGER_SYSTEM

					// 균등파티이거나 새파티이면 아이템 랜덤 먹기
					if (ch->IsParty() && ( ch->m_party->GetPartyType(MSG_DIVITYPE_ITEM) == PARTY_TYPE_RANDOM || ch->m_party->GetPartyType(MSG_DIVITYPE_ITEM) == PARTY_TYPE_BATTLE) )
					{
						if (GetItemRandomParty(ch, item))
							return;
					}
					
					// 아이템 소유권 변경: 050413
					if (item->m_preferenceIndex != -1 && item->m_preferenceIndex != ch->m_index)
					{
						// 입수우선 파티
						if (ch->IsParty() && ch->m_party->GetPartyType(MSG_DIVITYPE_ITEM) == PARTY_TYPE_FIRSTGET)
						{
							CParty* party = ch->m_party;
							if (!party)
								return;

							int i;
							// 소유권자가 파티에 속해있다
							bool bParty = false;
							for (i=0; i < MAX_PARTY_MEMBER; i++)
							{
								int nMemberIndex = party->GetMemberCharIndex(i);
								if (nMemberIndex != -1 && nMemberIndex == item->m_preferenceIndex)
								{
									bParty = true;
									break;
								}
							}

							if (!bParty)
							{
								// 우선권이 없습니다 메시지 전송
								CNetMsg smsg;
								SysMsg(smsg, MSG_SYS_NOT_OWNER_ITEM);
								SEND_Q(smsg, ch->m_desc);
								return;
							}
						}
						else
						{
							// 우선권이 없습니다 메시지 전송
							CNetMsg smsg;
							SysMsg(smsg, MSG_SYS_NOT_OWNER_ITEM);
							SEND_Q(smsg, ch->m_desc);
							return;
						}
					}

					CNetMsg disappmsg;
					CNetMsg takemsg;
					CNetMsg itemmsg;
					
					int cx, cz;
					
					cx = item->m_cellX;
					cz = item->m_cellZ;
					
					ItemTakeMsg(takemsg, takeChar, item);
					ItemDisappearMsg(disappmsg, item);
					
					// 들어갈 인벤토리 결정
					CInventory* inven = GET_INVENTORY(ch, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
					if (!inven)
						return ;

#ifdef EVENT_EGGS_HUNT_2007
					if(item->m_idNum == 2148)
					{
						int r, c;
						if(inven->FindItem(&r, &c, item->m_idNum, -1, -1))
						{
							// EGG 아이템 이미 소유하고 있음
							CNetMsg rmsg;
							EventEggsHunt2007ErrorMsg(rmsg, MSG_EVENT_EGGS_HUNT_2007_ERROR_ALREADY_EXIST);
							SEND_Q(rmsg, ch->m_desc);
							return ;
						}
					}
#endif // EVENT_EGGS_HUNT_2007

#ifdef EVENT_RICHYEAR_2007
					if(item->m_idNum == 876 
						|| item->m_idNum == 877
						|| item->m_idNum == 878
						|| item->m_idNum == 879
						|| item->m_idNum == 2422)
					{
						int r, c;
						if(inven->FindItem(&r, &c, 876, -1, -1)
							|| inven->FindItem(&r, &c, 877, -1, -1)
							|| inven->FindItem(&r, &c, 878, -1, -1)
							|| inven->FindItem(&r, &c, 879, -1, -1)
							|| inven->FindItem(&r, &c, 2422, -1, -1))
						{
							return ;
						}
					}
#endif // EVENT_RICHYEAR_2007
					
					bool bCountable = false;
					// 인벤에 넣기
					if (AddToInventory(ch, item, true, true))
					{
						// 겹쳐졌는지 검사
						if (item->tab() == -1)
						{
							bCountable = true;
							
							// 수량 변경 알림
							int r, c;
							if (inven->FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
							{
								CItem* p = inven->GetItem(r, c);
								if (p)
									ItemUpdateMsg(itemmsg, p, item->Count());
							}
						}
						else
						{
							ItemAddMsg(itemmsg, item);
							// 돈 검사
							if (item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && ch->m_moneyItem == NULL)
								ch->m_moneyItem = item;
						}
					}
					else
					{
						CNetMsg smsg;
						SysFullInventoryMsg(smsg, (char)inven->m_tab);
						SEND_Q(smsg, ch->m_desc);
						return ;
					}
					
					// 우선권 제거
					item->m_preferenceIndex = -1;
					
					// 땅에서 제거
					takeChar->m_pArea->ItemFromCell(item);
					
					// 메시지 보내고
					takeChar->m_pArea->SendToCell(takemsg, takeChar, true);
					SEND_Q(itemmsg, ch->m_desc);
					takeChar->m_pArea->SendToCell(disappmsg, GET_YLAYER(takeChar), cx, cz);
					
					// LOG
					GAMELOG << init("ITEM_PICK", ch)
						<< itemlog(item);

					if (char_type == MSG_CHAR_PET)
						GAMELOG << delim << "BY PET";

					GAMELOG	<< end;
					
					if (bCountable)
						delete item;
					
					return ;
				}
			}
		}
	}
}

void do_ItemWear(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	msg.MoveFirst();
	
	unsigned char subtype;
	char wear_pos, tab1, row1, col1;
	int itemidx1;
	
	msg >> subtype
		>> wear_pos
		>> tab1 >> row1 >> col1 >> itemidx1;

	if (itemidx1 < 0)
	{
		row1 = -1;
		col1 = -1;
	}
	
	if (wear_pos < 0 || wear_pos > MAX_WEARING)
	{
		SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CInventory* inven1 = GET_INVENTORY(ch, tab1);
	
	CItem * item1 = (inven1) ? inven1->GetItem(row1, col1) : NULL;	// 입는 아이템

	// 착용 위치가 올바른지 검사
	if (item1 && item1->m_itemProto->m_wearing != wear_pos)
	{
		// 올바르지 않은 착용 위치이면 착용 아이템이 악세인지 검사
		// 악세사리는 올바르지 않은 착용 위치가 오면 서버에서 조정함
		if (item1->IsAccessary())
		{
			int i;
			for (i = WEARING_ACCESSORY1; i <= WEARING_ACCESSORY3; i++)
			{
				if (ch->m_wearing[i] == NULL)
					break;
			}
			
			if (i > WEARING_ACCESSORY3)
				wear_pos = WEARING_ACCESSORY1;
			else
				wear_pos = i;
		}
		else
		{
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	// 입는 아이템이 결합된 아이템이면 불가능
	if (item1 && !item1->CanWear())
	{
		SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 060221 : bs : 펫 착용시 최근 죽은 시간을 검사
	if (item1 && item1->IsPet())
	{
		CPet* pet = ch->GetPet(item1->m_plus);
		if (!pet)
		{
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

#ifdef IMSO
		if (!(ch->m_pZone->m_bCanSummonPet))
		{
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, ch->m_desc);
			return ;			
		}	
#endif

		if (pet->GetRemainRebirthTime() > 0)
		{
			SysCannotWearDeadPetMsg(rmsg, pet->m_index, pet->GetRemainRebirthTime());
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

#ifdef MONSTER_RAID_SYSTEM
#ifdef COMPOSITE_RARE_ITEM
#ifndef NO_CHECK_RARE_LEVEL_AND_INDET
	// 레어 아이템은 착용레벨만 제한을 검사한다
	if (item1)
	{
		CItem* pEquip = item1;
		if (item1->m_flag & FLAG_ITEM_COMPOSITION)
		{
			if (item1->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE)
			{
				pEquip = ch->m_invenNormal.GetItem(item1->m_nCompositeItem);
			}
		}
		if (pEquip != NULL && pEquip->IsRareItem())
		{
			if (pEquip->GetItemLevel(false) > ch->m_level)
			{
				SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
	}
#endif // #ifndef NO_CHECK_RARE_LEVEL_AND_INDET
#endif // COMPOSITE_RARE_ITEM
#endif // MONSTER_RAID_SYSTEM

#ifdef SET_ITEM
	if (item1 && item1->IsOriginItem())
	{
		if (item1->GetItemLevel(false) > ch->m_level)
		{
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}
#endif


#ifdef LIMIT_ITEM_M35 // 35레벨 초과시 사용금지
	if(item1)
	{	// 착용 레벨 검사
		switch(item1->m_itemProto->m_index)
		{
		case 2669:	// 타이탄 목걸이
		case 2670:	// 나이트 목걸이
		case 2671:	// 힐러 목걸이
		case 2672:	// 메이지 목걸이
		case 2673:	// 로그 목걸이
		case 2674:	// 소서러 목걸이
			if(ch->m_level > 35)
			{
				SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		default:
			break;
		}
	}
#endif // LIMIT_ITEM_M35

	CItem * item2 = ch->m_wearing[(int)wear_pos];		// 벗는 아이템

#ifdef ENABLE_PET
	// 펫은 앉은 상태로 입거나 벗기 불가
	if (ch->IsSetPlayerState(PLAYER_STATE_SITDOWN) && ((item1 && item1->IsPet()) || (item2 && item2->IsPet())))
	{
		SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif // #ifdef ENABLE_PET
	
#ifdef NIGHT_SHADOW
	// 나이트 쉐도우는 마운트 타입 착용 불가
	if ( ch->m_job == JOB_NIGHTSHADOW && item1 && item1->IsPet() )
	{
		CPet *pet = ch->GetPet( item1->m_plus );
		if( pet && pet->IsMountType() )
		{
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}
#endif // NIGHT_SHADOW

	// 둘다 NULL 이면 무시
	if (!item1 && !item2)
	{
		SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (!item1)
	{
		// 벗기이면 무조건 위치가 올바르게 전달되야 함
		if (item2 != NULL && item2->m_itemProto->m_wearing != wear_pos)
		{
			// 악세는 위치 비교시 2, 3 자리를 검사
			if (item2->IsAccessary())
			{
				if (wear_pos < WEARING_ACCESSORY1 || wear_pos > WEARING_ACCESSORY3)
				{
					SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}
			else
			{
				SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
	}
	
	if (itemidx1 == -1)
	{
		// 벗기
		if (!item2)
		{
			// 빈자리에 빈자리 입히기는 무시
			return ;
		}

		// 봉인 아이템 불가능
		if (
#ifdef ATTACK_PET
			!item2->IsAPet() && 
#endif //ATTACK_PET
			item2->m_flag & FLAG_ITEM_SEALED)
			return;

		// 방어코드 : 원래 벗었었는데 또 벗을라 하믄 리턴
		if (item2->m_wearPos == WEARING_NONE)
			return;

		ch->m_wearing[(int)wear_pos] = NULL;
		item2->m_wearPos = WEARING_NONE;

#ifdef SET_ITEM 
		if (item2->IsOriginItem())
		{
			// 오리지날 옵션 및 세트 아이템의 스킬 삭제
			for (int i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
			{
				int nIndex = item2->GetOriginIndex(i);
				if (nIndex <= 0)
					continue;

				CSkill * pSkill = gserver.m_skillProtoList.Create(nIndex, item2->GetOriginLevel(i));
				if(pSkill)
				{
					if (pSkill->m_proto->IsActive())
					{
						pSkill->Cancel(ch);
						SkillCancelMsg(rmsg, ch);
						ch->m_pArea->SendToCell(rmsg, ch, true);

						ch->RemoveSkillFromQuickSlot(pSkill);
						ch->m_activeSkillList.Remove(pSkill);
					}
					else if (pSkill->m_proto->IsPassive())	
					{
						ch->m_passiveSkillList.Remove(pSkill);
					}
					else
					{
						ch->m_etcSkillList.Remove(pSkill);
					}
				}
			}

			// 세트 아이템 착용 개수 감소 및 세트 효과 스킬 삭제
			if (item2->m_itemProto->GetSetIndex())
			{
				ch->CountSetItem(item2, false);
				ch->RemoveSetItemValue(item2);

				item2->m_nSetWearCnt = 0;
				item2->m_nSetOption = 0;
			}

			SkillListMsg(rmsg, ch);
			SEND_Q(rmsg, ch->m_desc);
		}
#endif // SET_ITEM 

#ifdef ENABLE_PET
		// 애완동물 벗을 때에는 애완동물을 disappear 시킨다
		if (item2->IsPet())
		{
			// 착용 아이템에 해당하는 펫을 찾는다
			CPet* pet = ch->GetPet(item2->m_plus);
			if (pet)
			{
				if (pet->IsMount())
				{
					pet->Mount(false);
				}
				else
				{
					pet->Disappear();
				}
			}
		}
#endif
		
#ifdef ATTACK_PET
		if( item2->IsAPet() )
		{
			CAPet* pet = ch->GetAPet(item2->m_plus);
			if( pet )
			{
				pet->Disappear();
			}
		}
#endif //ATTACK_PET
		
#ifdef AUTO_POTION		
		if( item2->m_itemProto->m_index == ONE_PERIOD_ITEM 
			|| item2->m_itemProto->m_index == SEVEN_PERIOD_ITEM 
			|| item2->m_itemProto->m_index == THIRTY_PERIOD_ITEM 
			|| item2->m_itemProto->m_index == 2610 )
		{
			ch->m_assist.CureByItemIndex(item2->m_itemProto->m_index);				
		}			
#endif // AUTO_POTION

		ItemWearMsg(rmsg, wear_pos, NULL, item2);
		SEND_Q(rmsg, ch->m_desc);
		
		if (wear_pos >= WEARING_SHOW_START && wear_pos <= WEARING_SHOW_END)
		{
			WearingMsg(rmsg, ch, wear_pos, -1, 0);
			ch->m_pArea->SendToCell(rmsg, ch, false);
		}

#ifdef KYD_TEST
		if (wear_pos >= WEARING_ACCESSORY1 && wear_pos <= WEARING_ACCESSORY3)
		{
			WearingMsg(rmsg, ch, wear_pos, -1, -1);
			ch->m_pArea->SendToCell(rmsg, ch, false);
		}
#endif
		ch->CalcStatus(true);
	}
	else
	{
		// 입기
		if (!item1)
		{
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		if (!(item1->m_itemProto->m_jobFlag & (1 << ch->m_job)))
		{
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 방어코드 : 원래 벗겨져 있던게 아니면 리턴
		if (item1->m_wearPos != WEARING_NONE)
			return;

		// 명성치 검사
		if (item1->m_itemProto->m_fame != -1)
		{
			if (ch->m_fame < item1->m_itemProto->m_fame)
			{
				SysMsg(rmsg, MSG_SYS_SHORT_FAME);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		}

#ifdef ENABLE_PET
		char mountPetType = 0;
		bool bChangeMountPet = false;
		CPet* pet1 = NULL;
		CPet* pet2 = NULL;
#endif

		if (item2)
		{
			if (
#ifdef ATTACK_PET
				!item2->IsAPet() && 
#endif //ATTACK_PET
				item2->m_flag & FLAG_ITEM_SEALED)
				return;

			// 현재꺼 벗기기
			ch->m_wearing[(int)wear_pos] = NULL;
			item2->m_wearPos = WEARING_NONE;


#ifdef AUTO_POTION		
			if( item2->m_itemProto->m_index == ONE_PERIOD_ITEM 
				|| item2->m_itemProto->m_index == SEVEN_PERIOD_ITEM 
				|| item2->m_itemProto->m_index == THIRTY_PERIOD_ITEM 
				|| item2->m_itemProto->m_index == 2610 )
			{
				ch->m_assist.CureByItemIndex(item2->m_itemProto->m_index);	
				
				GAMELOG << init("ITEM PUT OFF AUTO POTION") << delim << item2->m_itemProto->m_index << end;

			}			
#endif // AUTO_POTION

#ifdef ENABLE_PET
			if (item2->IsPet())
			{
				// 착용 아이템에 해당하는 펫을 찾는다
				CPet* pet = ch->GetPet(item2->m_plus);
				if (pet)
				{
					if (pet->IsMount())
					{
						bChangeMountPet = true;
						mountPetType = 0;
						pet2 = pet;
					}
					else
					{
						pet->Disappear();
					}
				}
			}
#endif
#ifdef ATTACK_PET
			if( item2->IsAPet() )
			{
				CAPet* pet = ch->GetAPet(item2->m_plus);
				if( pet )
				{
					pet->Disappear();
				}
			}
#endif //ATTACK_PET
		}

		// 양손무기와 방패를 같이 착용하지 못하게
		switch (item1->m_itemProto->m_typeIdx)
		{
		case ITYPE_WEAPON:
			switch (item1->m_itemProto->m_subtypeIdx)
			{
			case IWEAPON_CROSSBOW:
			case IWEAPON_STAFF:
			case IWEAPON_BIGSWORD:
			case IWEAPON_AXE:
			case IWEAPON_BOW:
			case IWEAPON_MINING:
			case IWEAPON_GATHERING:
			case IWEAPON_CHARGE:
			case IWEAPON_TWOSWORD:
			case IWEAPON_SCYTHE:
			case IWEAPON_POLEARM:
			case IWEAPON_SOUL:
				if (ch->m_wearing[WEARING_SHIELD])
				{
					CItem* pitem = ch->m_wearing[WEARING_SHIELD];
					if (pitem->m_flag & FLAG_ITEM_SEALED)
						return;

					if (pitem->m_itemProto->m_typeIdx == ITYPE_WEAR && pitem->m_itemProto->m_subtypeIdx == IWEAR_SHIELD)
					{
						ch->m_wearing[WEARING_SHIELD] = NULL;
						pitem->m_wearPos = WEARING_NONE;
						
						ItemWearMsg(rmsg, WEARING_SHIELD, NULL, pitem);
						SEND_Q(rmsg, ch->m_desc);
						WearingMsg(rmsg, ch, WEARING_SHIELD, -1, 0);
						ch->m_pArea->SendToCell(rmsg, ch, false);
						break;
					}
				}
				break;
			}
			break;
		case ITYPE_WEAR:
			switch (item1->m_itemProto->m_subtypeIdx)
			{
			case IWEAR_SHIELD:
				if (ch->m_wearing[WEARING_WEAPON])
				{
					CItem* pitem = ch->m_wearing[WEARING_WEAPON];
					if (pitem->m_flag & FLAG_ITEM_SEALED)
						return;

					switch (pitem->m_itemProto->m_typeIdx)
					{
					case ITYPE_WEAPON:
						switch (pitem->m_itemProto->m_subtypeIdx)
						{
						case IWEAPON_CROSSBOW:
						case IWEAPON_STAFF:
						case IWEAPON_BIGSWORD:
						case IWEAPON_AXE:
						case IWEAPON_BOW:
						case IWEAPON_MINING:
						case IWEAPON_GATHERING:
						case IWEAPON_CHARGE:
						case IWEAPON_TWOSWORD:
						case IWEAPON_SCYTHE:
						case IWEAPON_POLEARM:
						case IWEAPON_SOUL:
							ch->m_wearing[WEARING_WEAPON] = NULL;
							pitem->m_wearPos = WEARING_NONE;
							
							ItemWearMsg(rmsg, WEARING_WEAPON, NULL, pitem);
							SEND_Q(rmsg, ch->m_desc);
							WearingMsg(rmsg, ch, WEARING_WEAPON, -1, 0);
							ch->m_pArea->SendToCell(rmsg, ch, false);
							break;
						}
						break;
					}
				}
				break;
			}
			break;

		case ITYPE_ACCESSORY:
			{
				if(item1->m_itemProto->m_index == 2371
#ifdef AUTO_POTION					
					|| item1->m_itemProto->m_index == ONE_PERIOD_ITEM 
					|| item1->m_itemProto->m_index == SEVEN_PERIOD_ITEM 
					|| item1->m_itemProto->m_index == THIRTY_PERIOD_ITEM 
					|| item1->m_itemProto->m_index == 2610
#endif //AUTO_POTION
					)
				{
					CSkill* pSkill;
					bool bApply;
					pSkill = gserver.m_skillProtoList.Create(item1->m_itemProto->m_num0, item1->m_itemProto->m_num1);
					if(pSkill)
					{
						ApplySkill(ch, ch, pSkill, item1->m_itemProto->m_index, bApply);
						delete pSkill;
						pSkill = NULL;
					}
					
					 GAMELOG << init("ITEM WEAR AUTO POTION") << delim << item1->m_itemProto->m_index << end;
				}
			}
			break;
		}
		// --- 양손무기와 방패를 같이 착용하지 못하게

		ch->m_wearing[(int)wear_pos] = item1;
		item1->m_wearPos = wear_pos;

#ifdef SET_ITEM
		bool bAddSkill = false;
		if (item1->IsOriginItem())
		{
			// 귀속 설정
			if (item1->GetBelongType() != ORIGIN_OPTION_BELONG_NONE && !item1->IsBelong())
			{
				item1->m_flag |= FLAG_ITEM_BELONG;
				GAMELOG << init("ITEM WEAR BELONG", ch) << itemlog(item1) << end;
			}

			// 착용 개수, 옵션 적용
			if (item1->m_itemProto->GetSetIndex())
			{
				ch->CountSetItem(item1);
			}

			// 아이템의 옵션이나 세트 옵션으로 스킬이 적용된 경우
			if (item1->GetOriginSkillCnt() > 0 || item1->m_nSetOption == 2)
				bAddSkill = true;
		}
#endif

		ItemWearMsg(rmsg, wear_pos, item1, item2);
		SEND_Q(rmsg, ch->m_desc);
		if (wear_pos >= WEARING_SHOW_START && wear_pos <= WEARING_SHOW_END)
		{
			WearingMsg(rmsg, ch, wear_pos, item1->m_idNum, item1->m_plus);
			ch->m_pArea->SendToCell(rmsg, ch, false);
		}

#ifdef KYD_TEST
		if (wear_pos >= WEARING_ACCESSORY1 && wear_pos <= WEARING_ACCESSORY3)
		{
			WearingMsg(rmsg, ch, wear_pos, item1->m_idNum, item1->m_plus);
			ch->m_pArea->SendToCell(rmsg, ch, false);
		}
#endif

#ifdef ENABLE_PET
		// 애완동물 입을 때에는 애완동물을 Appear
		if (item1->IsPet())
		{
			// 착용 아이템에 해당하는 펫을 찾는다
			CPet* pet = ch->GetPet(item1->m_plus);
			if (pet)
			{
				if (pet->IsMountType())
				{
					bChangeMountPet = true;
					mountPetType = pet->GetPetTypeGrade();
					pet1 = pet;
				}
				else
				{
					pet->Appear(true);
				}
			}
		}

		if (bChangeMountPet)
		{
			if (pet2)
			{
				pet2->Mount(false);
			}
			if (pet1)
			{
				pet1->Mount(true);
			}
		}
#endif

#ifdef ATTACK_PET
		if( item1->IsAPet() )	// 공격형 애완동물 Appear
		{
			CAPet* apet = ch->GetAPet(item1->m_plus);
			if( apet )
			{
				apet->Appear(true);

				ExAPetFuntionMsg(msg, MSG_SUB_SKILLLIST, apet, 0);
				SEND_Q(msg, apet->GetOwner()->m_desc);
#ifdef APET_AI
				APetAIOnOffMsg(rmsg, apet, MSG_APET_ERROR_OK);
				SEND_Q(rmsg, apet->GetOwner()->m_desc);
				
				APetAIListMsg(rmsg, apet);
				SEND_Q(rmsg, apet->GetOwner()->m_desc);
#endif
			}
		}
#endif // ATTACK_PET

		ch->m_assist.CheckApplyConditions();
		ch->CalcStatus(true);
#ifdef SET_ITEM
		if (bAddSkill)
		{
			CNetMsg rmsg;
			SkillListMsg(rmsg, ch);
			SEND_Q(rmsg, ch->m_desc)
		}
#endif
	}
	switch (wear_pos)
	{
	case WEARING_HELMET:
		{
			char cOldHairStyle = ch->m_hairstyle;
			ch->m_hairstyle = ch->m_hairstyle % 10;
			if (ch->m_wearing[WEARING_HELMET])
			{
				switch (ch->m_wearing[WEARING_HELMET]->m_itemProto->m_index)
				{
				case 1843:			// 빨강 산타 모자 : HAIR_RED_CAP + n
					ch->m_hairstyle += HAIR_RED_CAP;
					break;

				case 1844:			// 녹색 산타 모자 : HAIR_GREEN_CAP + n
					ch->m_hairstyle += HAIR_GREEN_CAP;
					break;
				}
			}
			if (cOldHairStyle != ch->m_hairstyle)
			{
				ExHairChangeMsg( rmsg, ch );
				ch->m_pArea->SendToCell(rmsg, ch, true);
			}
		}
		break;

	case WEARING_ACCESSORY1:
	case WEARING_ACCESSORY2:
	case WEARING_ACCESSORY3:
		{
			int nOldState = ch->GetPlayerState();
			ch->ResetPlayerState(PLAYER_STATE_SUPPORTER);
			int i;
			for (i = WEARING_ACCESSORY1; i <= WEARING_ACCESSORY3; i++)
			{
				if (ch->m_wearing[i])
				{
					// 서포터 아이템
					if (ch->m_wearing[i]->m_itemProto->m_index == 1912)
						ch->SetPlayerState(PLAYER_STATE_SUPPORTER);
				}
			}
			if (nOldState != ch->GetPlayerState())
			{
				ExPlayerStateChangeMsg(rmsg, ch);
				ch->m_pArea->SendToCell(rmsg, ch, true);
			}
		}
		break;
	}
}

void do_ItemSwap(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
	
	unsigned char subtype;
	char tab, row1, col1, row2, col2;
	int itemidx1, itemidx2;
	
	CInventory* inven;
	
	msg >> subtype
		>> tab
		>> row1 >> col1 >> itemidx1
		>> row2 >> col2 >> itemidx2;

	if (row1 == row2 && col1 == col2 && itemidx1 == itemidx2)
	{
		ch->m_bChatMonitor = true;
		GAMELOG << init("DEBUG SWAP", ch)
				<< ch->m_desc->m_host
				<< end;
		return ;
	}
	
	inven = GET_INVENTORY(ch, tab);
	if (!inven)
		return ;
	
	if (!inven->Swap(row1, col1, itemidx1, row2, col2, itemidx2))
		return ;
	
	CNetMsg rmsg;
	ItemSwapMsg(rmsg, tab, row1, col1, itemidx1, row2, col2, itemidx2);
	SEND_Q(rmsg, ch->m_desc);
#ifdef BWTEST
	CLCString chat(1000);
	chat.Format("item1 : idx_%d, r_%d, c_%d, t_%d  item2 : idx_%d, r_%d, c_%d, t_%d ", itemidx1, row1, col1, tab, itemidx2, row2, col2 );
	SayMsg(rmsg, MSG_CHAT_WHISPER, 0, "", "", chat);
	SEND_Q(rmsg, ch->m_desc);
#endif
}

void do_ItemArrange(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
	
	unsigned char subtype;
	char tab;
	
	msg >> subtype
		>> tab;
	
	CInventory* inven = GET_INVENTORY(ch, tab);
	if (!inven)
		return ;
	
	inven->Arrange();
	
	int i;
	int lastrow = -1;
	for (i = 0; i < MAX_INVENTORY_ROWS; i++)
	{
		if (inven->m_rows[i])
			lastrow = i;
	}

	CNetMsg retmsg;
	char resultArrange = 1;
	for (i = 0; i < MAX_INVENTORY_ROWS; i++)
	{
		if (lastrow == i)
			resultArrange |= 2;
		if (InventoryMsg(retmsg, ch, resultArrange, tab, i))
		{
			SEND_Q(retmsg, ch->m_desc);
			resultArrange = 0;
		}
	}
	
	for (i = 0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		QuickSlotListMsg(retmsg, ch, i);
		SEND_Q(retmsg, ch->m_desc);
	}
}

void do_ItemDelete(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
	
	unsigned char subtype;
	char tab, row, col;
	int itemidx;
	
	msg >> subtype
		>> tab >> row >> col
		>> itemidx;
	

	if (tab < 0 || tab >= 3 || row < 0 || row >= MAX_INVENTORY_ROWS || col < 0 || col >= ITEMS_PER_ROW)
		return ;
	
	CInventory* inven = GET_INVENTORY(ch, tab);
	if (!inven)
		return ;
	
	CItem* item = inven->GetItem(row, col);
	if (!item || item->m_index != itemidx || item->m_wearPos != WEARING_NONE)
		return ;
#ifdef NEW_UI
#ifdef ENABLE_PET

	if( item->IsPet() ) 
	{
		ch->DelPet(item->m_plus);
		return ;
	}
#endif // ENABLE_PET

#ifdef ATTACK_PET
	if( item->IsAPet())
	{
		ch->DelAPet(item->m_plus);
		return ;
	}
#endif // ATTACK_PET
#endif // NEW_UI	

	// Log
	GAMELOG << init("ITEM_DELETE", ch)
			<< itemlog(item)
			<< end;

	CNetMsg retmsg;
	ItemDeleteMsg(retmsg, item);

	// 돈이면 ...
	if (item == ch->m_moneyItem)
		ch->m_moneyItem = NULL;
	
	// 인벤에서 제거
	RemoveFromInventory(ch, tab, row, col, true, true);
	SEND_Q(retmsg, ch->m_desc);
}

void do_ItemThrow(CPC* ch, CNetMsg& msg)
{
	// 안보이면 무시
	if (!ch->m_bVisible)
		return ;
	
	msg.MoveFirst();
	
	unsigned char subtype;
	char tab, row, col;
	int itemidx;
	LONGLONG count;
	
	msg >> subtype
		>> tab >> row >> col
		>> itemidx
		>> count;
	
	if (tab < 0 || tab >= 3 || row < 0 || row >= MAX_INVENTORY_ROWS || col < 0 || col >= ITEMS_PER_ROW || count < 0)
		return ;
	
	CInventory* inven = GET_INVENTORY(ch, tab);
	if (!inven)
		return ;
	
	CItem* item = inven->GetItem(row, col);
	if (!item || item->m_index != itemidx || item->Count() < count || !item->CanDrop())
		return ;

	CItem* dropitem = NULL;
	CNetMsg updatemsg;

	// 버리는 개수와 소지 개수가 다르면
	if (item->Count() != count)
	{
		// 복사해서 count 설정
		dropitem = ch->m_pArea->DropItem(item->m_idNum, ch, item->m_plus, item->m_flag, count);
		if (!dropitem)
			return ;
		// 옵션 복사
		if (!(item->m_itemProto->m_flag & ITEM_FLAG_COUNT))
		{
			int i;
			for (i=0; i < MAX_ITEM_OPTION; i++)
			{
				dropitem->m_option[i] = item->m_option[i];
				dropitem->m_nOption = item->m_nOption;
			}
		}
		
		DecreaseFromInventory(ch, item, count);
		
		// 없어지거나 갱신 메시지
		if (item->Count() > 0)
		{
			ItemUpdateMsg(updatemsg, item, -count);
		}
		else
		{
			// 없어질 때
			ItemDeleteMsg(updatemsg, item);
			
			// 돈이면 ...
			if (item == ch->m_moneyItem)
				ch->m_moneyItem = NULL;
			
			// 인벤에서 제거
			RemoveFromInventory(ch, tab, row, col, true, true);
		}
	}
	else
	{
		// 없어질 때
		ItemDeleteMsg(updatemsg, item);
		
		// 돈이면 ...
		if (item == ch->m_moneyItem)
			ch->m_moneyItem = NULL;
		
		// 인벤에서 제거
		RemoveFromInventory(ch, tab, row, col, false, true);
		dropitem = item;
		ch->m_pArea->DropItem(dropitem, ch);
	}
	
	// 드롭 메시지
	CNetMsg dropmsg;
	ItemDropMsg(dropmsg, ch, dropitem);
	
	// 보내기
	// 삭제 보내기
	SEND_Q(updatemsg, ch->m_desc);
	// 드롭 보내기
	ch->m_pArea->SendToCell(dropmsg, ch, true);
	
	// Log
	GAMELOG << init("ITEM_DROP", ch)
			<< itemlog(dropitem)
			<< end;
}

// Buy & Sell 모두 PC위주로 구성
// Player가 살때 Item Buy Msg, Buy Rate 적용
void do_ItemBuy(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
	
	unsigned char subtype;
	int			i;
	int			npcIdx,
				itemCount = 0;
	
	LONGLONG	clientPrice;
	LONGLONG	serverPrice;
	
	CInventory* inven = NULL;
	
	int*		itemDBIdx = NULL;
	LONGLONG*	count = NULL;
	CItem**		item = NULL;
	
	try
	{
		msg >> subtype
			>> npcIdx
			>> clientPrice
			>> itemCount;
		
		// 최대 살수 있는 아이템 종류의 수
		if (itemCount > MAX_ITEM_BUY || itemCount <= 0)
			throw 0;		// 메시지 내의 아이템 종류 수 오류
		
		// validation shop
		CShop* shop;
		if (!(shop = ch->m_pZone->FindShop(npcIdx)))
			throw 1;		// 상점이 없음

		itemDBIdx = new int[itemCount];
		count = new LONGLONG[itemCount];
		
#if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 
		bool	bBingoBoxDup = false;
#endif // #if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 
		for(i=0; i < itemCount; i++)
		{
			msg >> itemDBIdx[i]
				>> count[i];

			if( count[i] < 1)
				throw 0;

#if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 
			// 마법상점이고, 빙고박스를 사려고 할 때 
			if(itemDBIdx[i] == 2047			// 초코 빙고
				|| itemDBIdx[i] == 2633 )	// 복주머니 빙고
			{
				int row, col;
				if(ch->m_invenNormal.FindItem(&row, &col, itemDBIdx[i], -1, -1))	// 이미 빙고 박스를 가지고 있으면 
					throw 10;
				if(bBingoBoxDup)
					throw 10;
				bBingoBoxDup = true;
			}
#endif // #if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 


			// 상점에 아이템이 있는지 검사 throw 6
			if (!shop->ThersIsItem(itemDBIdx[i]))
				throw 6;		// 상점에 아이템 없음
		}

		// 누적합계
		serverPrice = 0;
		item = new CItem*[itemCount];
		memset(item, 0, sizeof(CItem*) * itemCount);

		for (i=0; i < itemCount; i++)
		{
#ifdef TEST_SERVER
			// 테스트 상인은 인첸된 무기를 판매
			if (npcIdx == 170 || npcIdx == 171)
			{
				int prePlus = 0;
				CItemProto* itemproto = gserver.m_itemProtoList.FindIndex(itemDBIdx[i]);
				if (itemproto)
				{
					// 무기: 41, 49레벨
					if (itemproto->m_typeIdx == ITYPE_WEAPON && (itemproto->GetItemProtoLevel() == 41 || itemproto->GetItemProtoLevel() == 49))
						prePlus = 14;
					// 방어구: 40-42, 50-52
					else if (itemproto->m_typeIdx == ITYPE_WEAR && ((itemproto->GetItemProtoLevel() >= 40 && itemproto->GetItemProtoLevel() <= 42) || (itemproto->GetItemProtoLevel() >= 50 && itemproto->GetItemProtoLevel() <= 52)))
						prePlus = 10;
				}
				item[i] = gserver.m_itemProtoList.CreateItem(itemDBIdx[i], WEARING_NONE, prePlus, 0, count[i]);
				if (!item[i]->IsWeaponType() && !item[i]->IsArmorType())
					item[i]->m_plus = 0;
			}
			else
				item[i] = gserver.m_itemProtoList.CreateItem(itemDBIdx[i], WEARING_NONE, 0, 0, count[i]);
#else


			item[i] = gserver.m_itemProtoList.CreateItem(itemDBIdx[i], WEARING_NONE, 0, 0, count[i]);
#endif
			
			if (!item[i])
				throw 2;		// DB에 없는 아이템

			// 아이템 명성치 검사
			if (item[i]->m_itemProto->m_fame != -1)
			{
				// 명성치 딸린다
				if (ch->m_fame < item[i]->m_itemProto->m_fame)
					throw 9;
			}
			// 들어갈 인벤토리 결정
			inven = GET_INVENTORY(ch, GET_TAB(item[i]->m_itemProto->m_typeIdx, item[i]->m_itemProto->m_subtypeIdx));
			if (!inven)
				throw 3;		// DB에 잘못된 값
			
			// 무조건 인벤에 일단 추가
			if (!AddToInventory(ch, item[i], false, true))
			{
				delete item[i];
				item[i] = NULL;
				throw 4;		// 인벤토리 가득 참
			}
			
			serverPrice += (item[i]->m_itemProto->m_price * shop->m_buyRate / 100) * count[i];

		}
		
		// 합계가 다르다! --> 
		if (serverPrice != clientPrice)
			throw 5;		// 클라이언트 데이터 오류 : 가격 틀림
		
		// 돈 메세지
		CNetMsg moneyMsg;
		
		// 돈검사
		if (ch->m_moneyItem)
		{
			if ((ch->m_moneyItem->Count() > serverPrice))
			{
				ch->AddMoney(-serverPrice);
				ItemUpdateMsg(moneyMsg, ch->m_moneyItem, -serverPrice);
			}
			else if ((ch->m_moneyItem->Count() == serverPrice))
			{
				ItemDeleteMsg(moneyMsg, ch->m_moneyItem);
				ch->AddMoney(-serverPrice);
			}
			else
				throw 8;	// 돈 모질람
		}
		else
			throw 8;		// 돈 모질람
		
		for (i=0; i < itemCount; i++)
		{
			if (item[i])
			{
				// Log
				CNetMsg itemmsg;
				GAMELOG << init("ITEM_BUY", ch)
					<< npcIdx << delim
					<< itemlog(item[i]) << delim
					<< serverPrice << delim
					<< ((ch->m_moneyItem) ? ch->m_moneyItem->Count() : 0)
					<< end;
				
				// 인벤안에 셋팅되어 있으면 Add
				if (item[i]->tab() != -1)
				{
					ItemAddMsg(itemmsg, item[i]);
					item[i] = NULL;
				}

				// 인벤에 셋팅되어있지 않으면 Update
				else
				{
					int row, col;
					if (inven->FindItem(&row, &col, item[i]->m_idNum, item[i]->m_plus, item[i]->m_flag))
					{
						CItem* tItem = inven->GetItem(row, col);
						
						if (tItem)
							ItemUpdateMsg(itemmsg, tItem, count[i]);
						delete item[i];
						item[i] = NULL;
					}
				}
				
				SEND_Q(itemmsg, ch->m_desc);
			}
		}

#ifdef ENABLE_STATISTICS
		STATISTICS(ItemBuy, serverPrice);
#endif

#ifdef ENABLE_WAR_CASTLE
		// 세금 징수 : 구입금액의 10%
		if(ch->m_pZone->m_index == ZONE_MERAC || ch->m_pZone->m_index == ZONE_EGEHA)
			gserver.AddTaxItem(serverPrice / 10);
#endif
#ifdef DRATAN_CASTLE
		if(ch->m_pZone->m_index == ZONE_START || ch->m_pZone->m_index == ZONE_DRATAN)
			gserver.AddTaxItemDratan(serverPrice / 10);
#endif // DRATAN_CASTLE

		SEND_Q(moneyMsg, ch->m_desc);
	}
	catch (const int errCode)
	{
		// 오류 발생시 만들었던 아이템 제거
		if (item)
		{
			for (i=0; i < itemCount; i++)
			{
				if (item[i])
				{
					inven = GET_INVENTORY(ch, GET_TAB(item[i]->m_itemProto->m_typeIdx, item[i]->m_itemProto->m_subtypeIdx));
					
					// 인벤에 제대로 못들어간 넘이면 delete
					// 					if (item[i]->tab() == -1)
					// RemoveFromInventory(ch, item[i], true, false);
					// 					else
					// 						delete item[i];

					if(item[i]->tab() == -1)		// 기존에 추가했던 것
					{
						int row, col;
						if(ch->m_invenNormal.FindItem(&row, &col, item[i]->m_idNum, item[i]->m_plus, item[i]->m_flag))
						{
							CItem* pSelectItem = ch->m_invenNormal.GetItem(row, col);
							if(pSelectItem)
							{
								DecreaseFromInventory(ch, pSelectItem, count[i]);
							}
						}
					}
					else							// 새롭게 추가했던 것
					{
						RemoveFromInventory(ch, item[i], true, false);
					}
				}
			}
		}
		
		// 오류 메시지 보내기
		CNetMsg errmsg;
		
		switch (errCode)
		{
		case 0:			// 메시지 내의 아이템 종류 수 오류
		case 1:			// 상점이 없음
		case 2:			// DB에 없는 아이템
		case 3:			// DB에 잘못된 값
			SysMsg(errmsg, MSG_SYS_SHOP_CANCEL);
			break;
		case 4:			// 인벤토리 가득 참
			SysFullInventoryMsg(errmsg, (char)inven->m_tab);
			break;
		case 5:			// 클라이언트 데이터 오류 : 가격 틀림
		case 6:			// 상점에 아이템 없음
		case 7:			// 돈 빼기 실패
			SysMsg(errmsg, MSG_SYS_SHOP_CANCEL);
			break;
		case 8:			// 돈 모질람
			SysMsg(errmsg, MSG_SYS_SHORT_MONEY);
			break;

		case 9:			// 명성 부족
			SysMsg(errmsg, MSG_SYS_SHORT_FAME);
			break;
#if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 
		case 10:		// 빙고박스가 이미 있음
			SysMsg(errmsg, MSG_SYS_CANNOT_HAVE_DUPLICATE);
			break;
#endif	// #if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 
		}
		
		SEND_Q(errmsg, ch->m_desc);
	}
	
	if (itemDBIdx)
		delete [] itemDBIdx;
	if (count)
		delete [] count;
	if (item)
		delete [] item;
}

// Buy & Sell 모두 PC위주로 구성
// Player가 팔때 Item Sell Msg, Sell Rate 적용
void do_ItemSell(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
	
	unsigned char subtype;
	int			i;
	int			npcIdx;
	int			itemCount;
	
	LONGLONG	clientPrice;
	
	char*		tab = NULL;
	char*		row = NULL;
	char*		col = NULL;
	LONGLONG*	count = NULL;
	
	CItem**		item = NULL;
	LONGLONG	serverPrice;
	CInventory*	inven = NULL;
	CNetMsg		itemmsg;
	
	try
	{
		msg >> subtype
			>> npcIdx
			>> clientPrice
			>> itemCount;
		
		// 최대 팔수 있는 아이템 종류의 수
		if (itemCount > MAX_ITEM_SELL || itemCount <= 0)
			throw 0;		// 메시지 내의 아이템 종류 수 오류
		
		tab = new char[itemCount];
		row = new char[itemCount];
		col = new char[itemCount];
		count = new LONGLONG[itemCount];
		
		for(i=0; i < itemCount; i++)
		{
			tab[i] = -1;
			row[i] = -1;
			col[i] = -1;
			count[i] = 0;

			msg >> tab[i]
				>> row[i]
				>> col[i]
				>> count[i];

			if( count[i] < 1 )
				throw 0;

			for (int j = 0; j < i; j++)
			{
				if (tab[i] == tab[j] && row[i] == row[j] && col[i] == col[j])
					throw 2;
			}
		}
		
		// validation shop
		CShop* shop;
		if (!(shop = ch->m_pZone->FindShop(npcIdx)))
			throw 1;		// 상점이 없음
		
		serverPrice = 0;
		item = new CItem*[itemCount];
		memset(item, 0, sizeof(CItem*) * itemCount);
		
		// 판매 아이템 만들기
		for (i=0; i < itemCount; i++)
		{
			inven = GET_INVENTORY(ch, tab[i]);

			if (!inven)
				throw 2;
			
			if (!(item[i] = inven->GetItem(row[i], col[i])))
				throw 2;	// 인벤에 없는 아이템

			// 실제 아이템의 갯수가 클라이언트에서 팔려던 갯수보다 작다.
			if (item[i]->Count() < count[i])
				throw 2;
			
			// 입고 있거나 상점 불가 아이템인 경우
			if (!item[i]->CanSellToShop())
				throw 6;
			
			serverPrice += (item[i]->m_itemProto->m_price * shop->m_sellRate / 100) * count[i];
		}
		
		// 합계가 다르다! --> 
		if (serverPrice != clientPrice)
			throw 3;		// 클라이언트 데이터 오류 : 가격 틀림
		
		bool bHaveMoney = false;	// 돈이 아예 없었을 경우 false
		if (ch->m_moneyItem)
			bHaveMoney = true;
		
		// 돈 더하기
		if (!ch->AddMoney(serverPrice))
			throw 4;		// 돈 넣기 실패
		
		// 인벤에서 파는 아이템 완전 제거
		for (i=0; i < itemCount; i++)
		{
			// Log
			GAMELOG << init("ITEM_SELL", ch)
					<< npcIdx << delim
					<< itemlog(item[i]) << delim
					<< serverPrice << delim
					<< ((ch->m_moneyItem) ? ch->m_moneyItem->Count() : 0)
					<< end;
			
			bool bCountable = false;
			// 겹치는 아이템이면 수량 변화 안 겹치면 제거
			if (item[i]->m_itemProto->m_flag & ITEM_FLAG_COUNT)
			{
				bCountable = true;
				CInventory* tinven = GET_INVENTORY(ch, item[i]->tab());
				if (tinven)
				{
					DecreaseFromInventory(ch, item[i], count[i]);
					if (item[i]->Count() < 1)
					{
						ItemDeleteMsg(itemmsg, item[i]);
						if (!RemoveFromInventory(ch, item[i], true, true))
							throw 5;	// 인벤토리 제거 실패
					}
					else
						ItemUpdateMsg(itemmsg, item[i], -count[i]);
				}
			}
			else
			{
				ItemDeleteMsg(itemmsg, item[i]);
				if (!RemoveFromInventory(ch, item[i], true, true))
					throw 5;	// 인벤토리 제거 실패
			}
			
			SEND_Q(itemmsg, ch->m_desc);
		}

#ifdef ENABLE_STATISTICS
		STATISTICS(ItemSell, serverPrice * 3 / 2);
#endif

#ifdef ENABLE_WAR_CASTLE
		// 세금 징수 : 판매 금액의 40% 의 10% => 전체 금액의 4%
		if(ch->m_pZone->m_index == ZONE_MERAC || ch->m_pZone->m_index == ZONE_EGEHA)
			gserver.AddTaxItem(serverPrice / 25);
#endif
#ifdef DRATAN_CASTLE
		if(ch->m_pZone->m_index == ZONE_START || ch->m_pZone->m_index == ZONE_DRATAN)
			gserver.AddTaxItemDratan(serverPrice / 25);
#endif // DRATAN_CASTLE
		
		// 돈 늘어난거 보내기
		CNetMsg moneyMsg;
		if (bHaveMoney && ch->m_moneyItem)
			ItemUpdateMsg(moneyMsg, ch->m_moneyItem, serverPrice);	// 원래 돈 있으면 update
		else if (ch->m_moneyItem)
			ItemAddMsg(moneyMsg, ch->m_moneyItem);		// 원래 돈 없으면 add
		
		SEND_Q(moneyMsg, ch->m_desc);
	}
	catch (const int errCode)
	{
		// 오류 메시지 보내기
		CNetMsg errmsg;
		
		switch (errCode)
		{
		case 0:			// 메시지 내의 아이템 종류 수 오류
		case 1:			// 상점이 없음
		case 2:			// 인벤에 없는 물건
		case 3:			// 클라이언트 데이터 오류 : 가격 틀림
		case 4:			// 돈 넣기 실패
		case 5:			// 인벤에서 제거 실패
		case 6:			// 팔수 없는 아이템
			SysMsg(errmsg, MSG_SYS_SHOP_CANCEL);
			break;
		}
		
		SEND_Q(errmsg, ch->m_desc);
	}
	
	if (tab)
		delete [] tab;
	if (row)
		delete [] row;
	if (col)
		delete [] col;
	if (count)
		delete [] count;
	if (item)
		delete [] item;
	
}


void do_ItemUpgradeReq(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
	
	unsigned char subtype;
	char row1, col1, row2, col2;
	int plus, level;
	
	msg >> subtype
		>> row1 >> col1 >> plus
		>> row2 >> col2 >> level;
	//제련보호아이템
#ifdef RUNE_PROTECTION_ITEM
	char row3, col3;
	int runeItemIndex;
	msg >> runeItemIndex
		>> row3
		>> col3;
#endif // RUNE_PROTECTION_ITEM
	
	// inven row, col validation
	if (row1 < 0 || row1 >= MAX_INVENTORY_ROWS || col1 < 0 || col1 >= ITEMS_PER_ROW ||
		row2 < 0 || row2 >= MAX_INVENTORY_ROWS || col2 < 0 || col2 >= ITEMS_PER_ROW)
		return ;
	
	CInventory* inven = GET_INVENTORY(ch, INVENTORY_NORMAL);
	
	if (!inven)
		return ;
	
	// 업그레이드 아이템
	CItem* upItem = inven->GetItem(row1, col1);
	if(!upItem)
		return ;
	char wearPos = upItem->m_wearPos;
	CItem* wearItem = inven->GetItem(row1, col1);
	// 제련석 아이템
	CItem* reItem = inven->GetItem(row2, col2);

	// 아이템 검사
	// 1. NULL 검사
	// 2. 제련석 검사
	// 3. 업그레이드 가능 검사
	// 4. 결합 검사
	// 4.1. 결합용 아이템은 결합 이후에 가능
	// 4.2. 결합용이 아닌 아이템은 결합 플래그 OFF에 가능

	// 1. NULL 검사
	if (!upItem || !reItem)
		return ;

	// 2. 제련석 검사
	if (	reItem->m_itemProto->m_typeIdx != ITYPE_ETC
		|| reItem->m_itemProto->m_subtypeIdx != IETC_UPGRADE
//		|| (reItem->m_flag & PLATINUM_MAX_PLUS) != level	// 아이템 레벨 검사 제거 [3/11/2009 KwonYongDae]
		)
		return ;

	CItem* pItemClothes = NULL;		// 실패시 함께 사라질 아이템

	if (upItem->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE)
	{
		if (!(upItem->m_flag & FLAG_ITEM_COMPOSITION))
			return ;
		CItem* pItemEquip = ch->m_invenNormal.GetItem(upItem->m_nCompositeItem);
		if (!pItemEquip)
			return ;
		pItemClothes = upItem;
		upItem = pItemEquip;
	}
	else
	{
		if (upItem->m_flag & FLAG_ITEM_COMPOSITION)
			return ;

		if (upItem->m_plus != plus)
			return ;
	}

	// 3. 업그레이드 가능 검사
	if (!upItem->CanUpgrade())
		return ;

	// 부스터는 업그레이드 아님 : a_num0가 3이면 부스터
	if (reItem->m_itemProto->m_num0 == IETC_UPGRADE_BOOSTER)
		return ;
	
	// 일반 제련석의 경우 1 - 60 레벨이 맞는가
	if (reItem->m_flag != 0 && (reItem->m_flag < ITEM_UPGRADE_GENERAL_MINLEVEL || reItem->m_flag > ITEM_UPGRADE_GENERAL_MAXLEVEL))
		return ;
	
	// 일반제련석의 경우 제련석의 레벨 이하만 업그레이드 가능
	if (reItem->m_flag != 0 && reItem->m_flag < upItem->GetItemLevel(true))
	{
		CNetMsg errMsg;
		SysMsg(errMsg, MSG_SYS_UPGRADE_NOCONDITION);
		SEND_Q(errMsg, ch->m_desc);
		return;
	}
   //슈퍼고제를 이미 슈퍼고제 바른 아이템에 업그레이드 하려고 할경우.//0627	
	if(reItem->m_itemProto->m_num0 == IETC_UPGRADE_SPECIAL_SUPER && upItem->m_flag & FLAG_ITEM_SUPER_STONE_USED)
	{
		CNetMsg errMsg;
		SysMsg(errMsg, MSG_SYS_UPGRADE_CANT_SUPERSTONE);
		SEND_Q(errMsg, ch->m_desc);
		return;
	}
	
	int result = ITEM_UPGRADE_RESULT_NOCHANGE;
#ifdef PLATINUM
	if (
		   ( upItem->m_plus >= MAX_UPGRADE_PLUS && reItem->m_itemProto->m_num0 != IETC_UPGRADE_PLATINUM )
		|| ( upItem->IsUsedPlatinumSpecial() &&  reItem->m_itemProto->m_num0 == IETC_UPGRADE_PLATINUM )
		|| ( (upItem->m_flag & FLAG_ITEM_COMPOSITION) && reItem->m_itemProto->m_num0 == IETC_UPGRADE_PLATINUM)
		|| ( (upItem->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE) && reItem->m_itemProto->m_num0 == IETC_UPGRADE_PLATINUM)
		)
#else
	if ( upItem->m_plus >= MAX_UPGRADE_PLUS )
#endif
	{
		CNetMsg errMsg;
		SysMsg(errMsg, MSG_SYS_UPGRADE_NOCONDITION);
		SEND_Q(errMsg, ch->m_desc);
		return;
	}
	else
	{
		if (reItem->m_itemProto->m_num0 == IETC_UPGRADE_GENERAL)
		{
			// 일반 제련석
			int upprob = 50 - upItem->m_plus * 2;
			
#ifdef LC_JPN
			int nLevel = upItem->m_itemProto->GetItemProtoLevel();
			if (nLevel > 70)
			{
				upprob = upprob - 10;			
				if (upItem->m_plus >= 11)
				{
					upprob = upprob - 20;
				}				
			} 
			else if (nLevel > 60)
			{
				upprob = upprob - 5;
			}
		
			// 2007.04.03 bkj
			// 73Lv 이상은 +9부터 업그레이드 성공 확률 5% 하향
			// 50~60까지는 +9부터 업그레이드 성공 2% 하향
			// 61~71까지는 +9부터 3% 하향 
			// (파괴확률은 현상태 유지, 단 실패확률 상향조정) 
			if (nLevel >= 73)
			{
				upprob -= 5;
			}
			else if (nLevel > 60)
			{
				upprob -= 3;
			}
			else if (nLevel > 50)
			{
				upprob -= 2;
			}
#endif	// LC_JPN

			if (upprob < 1)
				upprob = 1;

			// 성공/변화 없음(+3미만)/파괴
			if (GetRandom(1, 100) <= upprob)
				result = ITEM_UPGRADE_RESULT_PLUS;	
			else
			{
				result = ITEM_UPGRADE_RESULT_BROKEN;
#ifdef	LC_MAL		/// kj
				if (upItem->m_plus < 7)	
					result = ITEM_UPGRADE_RESULT_NOCHANGE;
#else
				if (upItem->m_plus < 3)
					result = ITEM_UPGRADE_RESULT_NOCHANGE;
#endif	// LC_MAL				
			}
				

#ifdef RUNE_PROTECTION_ITEM	// 제련 보호 아이템
			if( runeItemIndex != -1 )
			{
				if( upItem->m_plus > 2 && upItem->m_plus < 10 )
				{
					if( runeItemIndex == LOW_RUNE_PROTECTION_ITEM && result == ITEM_UPGRADE_RESULT_BROKEN )
					{

						result = ITEM_UPGRADE_RESULT_NOCHANGE;
					}

				}
				else if( upItem->m_plus > 9 && upItem->m_plus < 15 )
				{
					if( runeItemIndex == HIGH_RUNE_PROTECTION_ITEM && result == ITEM_UPGRADE_RESULT_BROKEN )
					{
						result = ITEM_UPGRADE_RESULT_MINUS;
					}
				}
			}			
#endif // RUNE_PROTECTION_ITEM
		}
#ifdef FEEITEM
		else if( reItem->m_itemProto->m_num0 == IETC_UPGRADE_LUCKY)
		{
#if defined (LC_BRZ)
			if( upItem->m_plus > 6)
#else
			if( upItem->m_plus > 5)
#endif // LC_BRZ
			{
				CNetMsg errMsg;
				SysMsg(errMsg, MSG_SYS_UPGRADE_CANT_6LEVEL_LUCKY);
				SEND_Q(errMsg, ch->m_desc);
				return;
			}
			result = ITEM_UPGRADE_RESULT_PLUS;
		}
#endif// FEEITEM
		else if (reItem->m_itemProto->m_num0 == IETC_UPGRADE_SPECIAL)//고급제련석//0627
		{
		// 잠수함 : 고제 확률 조정
			// +14에 고제 사용시에는 확률 조정, 그외는 기존과 동일
			if (upItem->m_plus == 14)
			{
				// 고급 제련석
				int prob = GetRandom(1, 100);
				int upprob = 18;
#ifdef UPGRADE_EVENT
				if(gserver.m_bUpgradeEvent)
					upprob = (upprob * gserver.m_nUpgradeProb) / 100;
#endif // UPGRADE_EVENT
				// 성공/+3미만 무변/실패
#ifdef SPECIALREFINE_UPGRAGE_DOWN
				upprob -= 7;
#endif
				if (prob <= upprob)
					result = ITEM_UPGRADE_RESULT_PLUS;
				else
				{
					prob -= upprob;
#ifdef LC_MAL
#else
					int downprob = 62;
					// 하락 62%/그외는 무변

					if (prob <= downprob)
						result = ITEM_UPGRADE_RESULT_MINUS;					
					else
						result = ITEM_UPGRADE_RESULT_NOCHANGE;
#endif // LC_MAL

				}
			}
			else
			{
				// 고급 제련석
				int prob = GetRandom(1, 100);
				int upprob = 60 - upItem->m_plus * 3;

#ifdef UPGRADE_EVENT
				if(gserver.m_bUpgradeEvent)
					upprob = (upprob * gserver.m_nUpgradeProb) / 100;
#endif // UPGRADE_EVENT


#ifdef SPECIALREFINE_UPGRAGE_DOWN
				upprob -= 6;
#endif
				if (upprob < 1)
					upprob = 1;
				// 성공/+3미만 무변/실패
				if (prob <= upprob)
					result = ITEM_UPGRADE_RESULT_PLUS;
#ifdef LC_MAL			/// kj
				else if (upItem->m_plus < 7 )// 변경 없음
					result = ITEM_UPGRADE_RESULT_NOCHANGE;		
#elif defined (LC_JPN)
				else if (upItem->m_plus < 4)
					result = ITEM_UPGRADE_RESULT_NOCHANGE;
#else
				else if (upItem->m_plus < 3)
					result = ITEM_UPGRADE_RESULT_NOCHANGE;
#endif		// LC_MAL
				else
				{
					prob -= upprob;
					int destruct = 5;
					
					// 파괴 5%
#ifdef RUNE_PROTECTION_ITEM	// 제련 보호 아이템
					if (prob <= destruct)
					{
						if( runeItemIndex != -1 )
						{
							if( upItem->m_plus > 2 && upItem->m_plus < 10 )
							{
								if( runeItemIndex == LOW_RUNE_PROTECTION_ITEM )
								{
									result = ITEM_UPGRADE_RESULT_NOCHANGE;
								}
								else if(runeItemIndex == HIGH_RUNE_PROTECTION_ITEM)
								{
									CNetMsg errMsg;
									SysMsg(errMsg, MSG_SYS_UPGRADE_NOCONDITION);
									SEND_Q(errMsg, ch->m_desc);
									return;									
								}
							}
							else if( upItem->m_plus > 9 && upItem->m_plus < 15 )
							{
								if( runeItemIndex == HIGH_RUNE_PROTECTION_ITEM )
								{
									result = ITEM_UPGRADE_RESULT_MINUS;
								}
								else if(runeItemIndex == LOW_RUNE_PROTECTION_ITEM)
								{
									CNetMsg errMsg;
									SysMsg(errMsg, MSG_SYS_UPGRADE_NOCONDITION);
									SEND_Q(errMsg, ch->m_desc);
									return;									
								}

							}
						}
						else
						{
							result = ITEM_UPGRADE_RESULT_BROKEN;
						}
					}
#else
					if (prob <= destruct)
						result = ITEM_UPGRADE_RESULT_BROKEN;
#endif // RUNE_PROTECTION_ITEM
					else
					{
						prob -= destruct;
#ifdef LC_MAL
						// 말레이시아는 고제로 되지 않고 유지만 된다.
#else
						// 하락 13%/그외는 무변
						int downprob = 13;
						if (prob <= downprob)
							result = ITEM_UPGRADE_RESULT_MINUS;
						else
#endif // LC_MAL
							result = ITEM_UPGRADE_RESULT_NOCHANGE;
					}
				}
			}
		}
#ifdef PLATINUM
		else if( reItem->m_itemProto->m_num0 == IETC_UPGRADE_PLATINUM )
		{
			if( upItem->m_plus > MAX_UPGRADE_PLUS || reItem->m_plus >= PLATINUM_MAX_PLUS)
			{
				CNetMsg errMsg;
				SysMsg(errMsg, MSG_SYS_UPGRADE_NOCONDITION);
				SEND_Q(errMsg, ch->m_desc);
				return;
			}

			result = ITEM_UPGRADE_PLATINUM_RESULT_PLUS;

		}
#endif
		else if (reItem->m_itemProto->m_num0 == IETC_UPGRADE_CHAOS)//특급 제련석
		{
			if (upItem->m_plus < 12)
			{
				result = ITEM_UPGRADE_RESULT_PLUS;
			}
			else
			{
				result = ITEM_UPGRADE_RESULT_NOCHANGE;
			}
		}
		else if(reItem->m_itemProto->m_num0 == IETC_UPGRADE_SPECIAL_SUPER)//슈퍼고제.//0627
		{

			if (upItem->m_plus > 13)//슈퍼고제는 +14 아이템에는 사용할수 없다.
			{
				CNetMsg errMsg;
				SysMsg(errMsg, MSG_SYS_UPGRADE_CANT_14LEVEL);
				SEND_Q(errMsg, ch->m_desc);
				return;
			}

			//100프로 성공
			result = ITEM_UPGRADE_RESULT_PLUS;

			upItem->m_flag |= FLAG_ITEM_SUPER_STONE_USED; //한번밖에 사용 못함.

		}
	}
	
	// 업글할 아이템 update 여부
	bool bUpdate = false;
	
	CNetMsg upItemMsg;
	CNetMsg reItemMsg;
	CNetMsg repMsg;
	
	switch (result)
	{
		// + 1
	case ITEM_UPGRADE_RESULT_PLUS:
		upItem->m_plus++;
		if (pItemClothes)
			pItemClothes->m_plus++;
		bUpdate = true;

#ifdef EVENT_LATTO
		// Latto Event
		if (gserver.m_bLattoEvent && upItem->m_plus <= 4)
		{
			 if (GetRandom(1, 10000) <= LATTO_EVENT_UPGRADE_PROB)
			 {
				 upItem->m_plus++;
				if (pItemClothes)
					pItemClothes->m_plus++;
				 EventLattoMsg(repMsg, MSG_EVENT_LATTO_UPGRADE_SUCCESS);
			 }
			 else
				 ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_PLUS);
		}
		else
			ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_PLUS);
#else
		ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_PLUS);
#endif

		break;
		
		// -1
	case ITEM_UPGRADE_RESULT_MINUS:
#ifdef RUNE_PROTECTION_ITEM	// 제련 보호 아이템
		if( runeItemIndex != -1 )
		{	
			if( upItem->m_plus > 3 && upItem->m_plus < 10 )
			{
				if( runeItemIndex == LOW_RUNE_PROTECTION_ITEM  )
				{
					upItem->m_plus--;
					if (pItemClothes)
						pItemClothes->m_plus--;
					bUpdate = true;
					ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_MINUS);
				}
			}
			else if( upItem->m_plus > 9 && upItem->m_plus < 15 )
			{
				if( runeItemIndex == HIGH_RUNE_PROTECTION_ITEM )
				{
					upItem->m_plus = 10;
					if( pItemClothes )
						pItemClothes->m_plus = 10;
					bUpdate = true;
					ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RUNE_PROTECTION_ITEM);
				}
			}
		}
		else
		{
			upItem->m_plus--;
			if (pItemClothes)
				pItemClothes->m_plus--;
			bUpdate = true;
			ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_MINUS);
		}
#else
		upItem->m_plus--;
		if (pItemClothes)
			pItemClothes->m_plus--;
		bUpdate = true;
		ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_MINUS);
#endif //RUNE_PROTECTION_ITEM
		
	
		break;
		
		// no change
	case ITEM_UPGRADE_RESULT_NOCHANGE:
		GAMELOG << init("ITEM_UPGRADE", ch)
				<< "No change" << delim
				<< itemlog(upItem);

		if (reItem->m_itemProto->m_num0 == IETC_UPGRADE_GENERAL)
			GAMELOG << delim << "USE GENERAL" << end;
		else
			GAMELOG << delim << "USE SPECIAL" << end;
		
		ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_NOCHANGE);
		break;
		
	case ITEM_UPGRADE_RESULT_BROKEN:
		
		// 입고 있는 아이템 이면 벗기
		if (wearPos != WEARING_NONE)
		{
			ch->m_wearing[(int)wearPos] = NULL;
			ItemWearMsg(upItemMsg, wearPos, NULL, wearItem);
			wearItem->m_wearPos = WEARING_NONE;
			SEND_Q(upItemMsg, ch->m_desc);
		}
		
		// LOG
		GAMELOG << init("ITEM_UPGRADE", ch)
				<< "Broken" << delim
				<< itemlog(upItem);

		if (reItem->m_itemProto->m_num0 == IETC_UPGRADE_GENERAL)
			GAMELOG << delim << "USE GENERAL" << end;
		else
			GAMELOG << delim << "USE SPECIAL" << end;
		
		// Item Delete Msg 전송
		ItemDeleteMsg(upItemMsg, upItem);
		SEND_Q(upItemMsg, ch->m_desc);
		
		// 인벤에서 제거
		RemoveFromInventory(ch, upItem->tab(), upItem->row(), upItem->col(), true, true);
		
		if (wearPos >= WEARING_SHOW_START && wearPos <= WEARING_SHOW_END)
		{
			CNetMsg wearmsg;
// 050223 : bs : plus 효과를 위한 정보 추가
			WearingMsg(wearmsg, ch, wearPos, -1, 0);
// --- 050223 : bs : plus 효과를 위한 정보 추가
			ch->m_pArea->SendToCell(wearmsg, ch, false);
		}

		// 의상 아이템의 경우 실패시 결합 아이템 함께 제거
		if (pItemClothes)
		{
			GAMELOG << init("ITEM_UPGRADE", ch)
					<< "BROKEN COMPOSITED ITEM" << delim
					<< itemlog(pItemClothes)
					<< end;
			ItemDeleteMsg(upItemMsg, pItemClothes);
			SEND_Q(upItemMsg, ch->m_desc);
			RemoveFromInventory(ch, pItemClothes, true, true);
		}
		
		ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_RESULT_BROKEN);
		
		break;
#ifdef PLATINUM
	case ITEM_UPGRADE_PLATINUM_RESULT_PLUS:
		{
			int platinumPlused;
			FLAG_ITEM_PLATINUM_GET(upItem->m_flag, platinumPlused);

			if( upItem->m_plus + reItem->m_plus > 16 )
			{
				int differ = 16 - upItem->m_plus;
				upItem->m_plus = 16;
				platinumPlused += differ;
			}
			else
			{
				platinumPlused += reItem->m_plus;
				upItem->m_plus += reItem->m_plus;
			}

			FLAG_ITEM_PLATINUM_SET(upItem->m_flag, platinumPlused);
			// 시간 세팅
			
			upItem->m_used = gserver.m_gameTime + reItem->m_itemProto->m_num3 * 60 * 60;
			bUpdate = true;
			ItemUpgradeRepMsg(repMsg, ITEM_UPGRADE_PLATINUM_RESULT_PLUS);
		}
		break;
#endif // PLATINUM
		
	default:
		break;
	}
	
	// upgrade 된 결과 알리기
	if (bUpdate)
	{
		// LOG
		GAMELOG << init("ITEM_UPGRADE", ch);
		switch (result)
		{
		case ITEM_UPGRADE_RESULT_PLUS:
			GAMELOG << "+1";
			break;
		case ITEM_UPGRADE_RESULT_MINUS:
#ifdef RUNE_PROTECTION_ITEM
		if( runeItemIndex != -1 )
		{			
			if( upItem->m_plus > 9 && upItem->m_plus < 15 )
			{
				if( runeItemIndex == HIGH_RUNE_PROTECTION_ITEM )
				{
					GAMELOG << "10";		// 10으로 고정
				}
			}
		}
		else
		{
			GAMELOG << "-1";
		}	
#else
			GAMELOG << "-1";
#endif
			break;
#ifdef PLATINUM
		case ITEM_UPGRADE_PLATINUM_RESULT_PLUS:
			GAMELOG << "PLATINUM" << delim << upItem->m_used << delim;
			break;
#endif
		}
		GAMELOG << itemlog(upItem);

		if (reItem->m_itemProto->m_num0 == IETC_UPGRADE_GENERAL)
			GAMELOG << delim << "USE GENERAL" << end;
		else if (reItem->m_itemProto->m_num0 == IETC_UPGRADE_SPECIAL_SUPER)
			GAMELOG << delim << "USE SUPER-SPECIAL" << end;
#ifdef PLATINUM
		else if (reItem->m_itemProto->m_num0 == IETC_UPGRADE_PLATINUM)
			GAMELOG << delim << "USE PLATINUM-SPECIAL" << end;
#endif
		else
			GAMELOG << delim << "USE SPECIAL" << end;
		
		ItemUpdateMsg(upItemMsg, upItem, 0);
		SEND_Q(upItemMsg, ch->m_desc);

		// 의상 아이템의 경우 FLAG_ITEM_PLATINUM_SET
		if (pItemClothes)
		{
			FLAG_ITEM_PLATINUM_SET(pItemClothes->m_flag, upItem->m_plus);
			ItemUpdateMsg(upItemMsg, pItemClothes, 0);
			SEND_Q(upItemMsg, ch->m_desc);
		}

		CNetMsg wearMsg;
		WearingMsg(wearMsg, ch, upItem->m_wearPos, upItem->m_idNum, upItem->m_plus);
		ch->m_pArea->SendToCell(wearMsg, ch, false);
	}
	
	// 제련석 하나 감소
	if (reItem->Count() > 1)
	{
		DecreaseFromInventory(ch, reItem, 1);
		ItemUpdateMsg(reItemMsg, reItem, -1);
	}
	else
	{
		ItemDeleteMsg(reItemMsg, reItem);
		RemoveFromInventory(ch, reItem->tab(), reItem->row(), reItem->col(), true, true);
	}



	
	SEND_Q(reItemMsg, ch->m_desc);
	SEND_Q(repMsg, ch->m_desc);

	// 제련 보호 아이템 하나 감소
#ifdef RUNE_PROTECTION_ITEM
	if( runeItemIndex != -1 )
	{		
		CNetMsg RuneMsg;
	 	CItem* runeItem = inven->GetItem(row3, col3);
		if( runeItem )
		{			
			if( runeItem->Count() > 1 )
			{
				DecreaseFromInventory( ch, runeItem, 1 );
				ItemUpdateMsg( RuneMsg, runeItem, -1 );
			}
			else
			{
				ItemDeleteMsg( RuneMsg, runeItem );
				RemoveFromInventory( ch, runeItem->tab(), runeItem->row(), runeItem->col(), true, true );
			}

			SEND_Q( RuneMsg, ch->m_desc );
			
			GAMELOG << "USE RUNE_PROTECTION_ITEM" << delim << runeItemIndex << end;
		}
	}
#endif // RUNE_PROTECTION_ITEM

	ch->m_assist.CheckApplyConditions();
	ch->CalcStatus(true);

	ch->SaveNow(false);

}

#ifdef LEVELDOWN_SCROLL
void do_ItemUseLevelDown(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
		
	unsigned char subtype;
	char row1, col1, row2, col2;
	CNetMsg rmsg;
		
	msg >> subtype
		>> row1 >> col1
		>> row2 >> col2;
	
	// inven row, col validation
	if (row1 < 0 || row1 >= MAX_INVENTORY_ROWS || col1 < 0 || col1 >= ITEMS_PER_ROW ||
		row2 < 0 || row2 >= MAX_INVENTORY_ROWS || col2 < 0 || col2 >= ITEMS_PER_ROW)
	{
		ItemLevelDownErrMsg(rmsg, MSG_ITEM_LEVELDOWN_ERROR_NOEQUIP);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	CInventory* inven = GET_INVENTORY(ch, INVENTORY_NORMAL);
	
	if (!inven)
		return ;
	
	// 업그레이드 아이템
	CItem * pEquipItem = inven->GetItem(row1, col1);
	// 주문서
	CItem * pUsedItem = inven->GetItem(row2, col2);

	// 아이템 검사
	// 1. NULL 검사
	// 2. 주문서 검사
	// 3. 레벨 다운 가능 검사
	// 4. 레벨 다운 적용

	// 1. NULL 검사
	if (!pEquipItem || !pUsedItem)
		return ;

	//2. 주문서 검사
	if (pUsedItem->m_itemProto->m_index !=  LEVELDOWN6_15 
		|| pUsedItem->Count() < 1 )
	{
		ItemLevelDownErrMsg(rmsg, MSG_ITEM_LEVELDOWN_ERROR_SCROLL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}		

	// 3. 레벨 다운 가능 검사
	if (pEquipItem->m_flag & FLAG_ITEM_LEVELDOWN)
	{	// 이미 레벨 다운된 아이템
		ItemLevelDownErrMsg(rmsg, MSG_ITEM_LEVELDOWN_ERROR_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (pEquipItem->m_itemProto->m_typeIdx != ITYPE_WEAPON 
		&& pEquipItem->m_itemProto->m_typeIdx != ITYPE_WEAR)
	{	// 무기나 장비가 아닌 경우
		ItemLevelDownErrMsg(rmsg, MSG_ITEM_LEVELDOWN_ERROR_NOEQUIP);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (pEquipItem->m_itemProto->GetItemProtoLevel() < 2)
	{
		ItemLevelDownErrMsg(rmsg, MSG_ITEM_LEVELDOWN_ERROR_NOEQUIP);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//  4. 레벨 다운 적용
	pEquipItem->m_flag |= FLAG_ITEM_LEVELDOWN;

	switch (pUsedItem->m_itemProto->m_index)
	{
	case LEVELDOWN6_15:	// 6레벨 15일
		pEquipItem->m_nLevel = -6;
		if (pEquipItem->m_itemProto->m_num0 == 0 )
		{
			pEquipItem->m_used_2 = gserver.m_gameTime + 15 * 24 * 60 * 60;
		}
		else
		{
			pEquipItem->m_used_2 = gserver.m_gameTime + pEquipItem->m_itemProto->m_num0;
		}
		break;
	default:	// 레벨 다운 주문서가 아닌 경우
		ItemLevelDownErrMsg(rmsg, MSG_ITEM_LEVELDOWN_ERROR_SCROLL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (pUsedItem->Count() > 1)
	{
		DecreaseFromInventory(ch, pUsedItem, 1);
		ItemUpdateMsg(rmsg, pUsedItem, -1);
	}
	else
	{
		ItemDeleteMsg(rmsg, pUsedItem);
		RemoveFromInventory(ch, pUsedItem->tab(), pUsedItem->row(), pUsedItem->col(), true, true);
	}

	SEND_Q(rmsg, ch->m_desc);

	// 레벨 다운된 아이템 업데이트
	ItemUpdateMsg(rmsg, pEquipItem, 0);
	SEND_Q(rmsg, ch->m_desc);
	
	ItemLevelDownErrMsg(rmsg, MSG_ITEM_LEVELDOWN_ERROR_OK);
	SEND_Q(rmsg, ch->m_desc);	
}
#endif	// LEVELDOWN_SCROLL

void do_ItemRefinReq(CPC* ch, CNetMsg& msg)
{
#ifdef LC_KOR
	static const int nProbFactorWeaponLow10				= 10;
	static const int nProbFactorWeaponHigh10			= 5;
	static const int nProbFactorArmorLow10				= 30;
	static const int nProbFactorArmorHigh10				= 15;
#else
	static const int nProbFactorWeaponLow10				= 5;
	static const int nProbFactorWeaponHigh10			= 5;
	static const int nProbFactorArmorLow10				= 15;
	static const int nProbFactorArmorHigh10				= 15;
#endif // LC_KOR
	CNetMsg rmsg;

	msg.MoveFirst();
	
	unsigned char subtype;
	char row, col;
	
	msg >> subtype
		>> row >> col;
	
	// inven row, col validation
	if (row < 0 || row >= MAX_INVENTORY_ROWS || col < 0 || col >= ITEMS_PER_ROW)
		return ;
	
	CInventory* inven = GET_INVENTORY(ch, INVENTORY_NORMAL);
	
	if (!inven)
		return ;
	
	// 전환 아이템
	CItem* cItem = inven->GetItem(row, col);

	if (cItem == NULL || cItem->m_wearPos != -1)
		return ;
	
	// 전환 아이템이 무기 or 방어구 인가
	if (!cItem || !(cItem->IsWeaponType() || cItem->IsArmorType()))
	{
		// 무기나 방어구만 가능하다
		SysMsg(rmsg, MSG_SYS_MAKE_REFINE_KIND);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (cItem->GetItemLevel(true) < 1)
	{
		// 레벨이 1보다 작으면 불가능
		SysMsg(rmsg, MSG_SYS_MAKE_REFINE_CANNOT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	// 전환아이템의 plus가 0이 아니면 제련석 변환 불가
	if (cItem->m_plus > 0)
	{
		SysMsg(rmsg, MSG_SYS_MAKE_REFINE_PLUS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	if (cItem->m_wearPos != WEARING_NONE)
	{
		// 착용중인 아이템은 못올린다 벗고해라
		SysMsg(rmsg, MSG_SYS_MAKE_REFINE_WEARING);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int price =0;

	// 아이템 레벨에 따른 소요 비용
	price = ((cItem->GetItemLevel(true) + 1) * (cItem->GetItemLevel(true) + 3) * (cItem->GetItemLevel(true) - 1) + 100) / 4;
	
	// 돈검사
	bool bDelMoney = false;
	if (!ch->m_moneyItem || ch->m_moneyItem->Count() < price)
	{
		// 돈이 모질라
		SysMsg(rmsg, MSG_SYS_MAKE_REFINE_SHORT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	else if (ch->m_moneyItem->Count() == price)
		bDelMoney = true;

	int result = ITEM_REFINE_RESULT_ONE;

	int spb = 0;
	if (cItem->IsWeaponType())
	{
		if (cItem->GetItemLevel(true) == 128)
			spb = 10000;
		else if (cItem->GetItemLevel(true) > 10)
			spb = ABS(100 * (cItem->GetItemLevel(true) + 128) / (cItem->GetItemLevel(true) - 128) / nProbFactorWeaponHigh10);
		else
			spb = ABS(100 * (cItem->GetItemLevel(true) + 128) / (cItem->GetItemLevel(true) - 128) / nProbFactorWeaponLow10);
		result = ITEM_REFINE_RESULT_TWO;
	}
	else if (cItem->IsArmorType())
	{
		if (cItem->GetItemLevel(true) == 256)
			spb = 10000;
		else if (cItem->GetItemLevel(true) > 10)
			spb = ABS(100 * (cItem->GetItemLevel(true) + 256) / (cItem->GetItemLevel(true) - 256) / nProbFactorArmorHigh10);
		else
			spb = ABS(100 * (cItem->GetItemLevel(true) + 256) / (cItem->GetItemLevel(true) - 256) / nProbFactorArmorLow10);
		result = ITEM_REFINE_RESULT_ONE;
	}

	// 말레이시아에서는 고제가 나오지 않는다
#ifdef DISABLE_REFINE_SPECIAL_STONE
#elif defined( LIMIT_REFINE_SPECIAL_STONE ) // 일본의 고급제련석 레벨제한
	if ( cItem->m_itemProto->GetItemProtoLevel() >= LIMIT_REFINE_SPECIAL_STONE
		&& GetRandom(1, 10000) <= (spb - 1000) )	// 일본 고급 제련석 확률 기존 보다 10% 적게
		result = ITEM_REFINE_RESULT_SPECIAL;
#else
	if (GetRandom(1, 10000) <= spb)
		result = ITEM_REFINE_RESULT_SPECIAL;
#endif // LC_MAL
	
	CItem* reItem = NULL;
	int needweight = 9999;
	
	if (result == ITEM_REFINE_RESULT_TWO)
	{
		reItem = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_normalRefineItem->m_index, WEARING_NONE, 0, cItem->GetItemLevel(true), 2);
		needweight = gserver.m_itemProtoList.m_normalRefineItem->m_weight;
	}
	else if (result == ITEM_REFINE_RESULT_ONE)
	{
		reItem = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_normalRefineItem->m_index, WEARING_NONE, 0, cItem->GetItemLevel(true), 1);
		needweight = gserver.m_itemProtoList.m_normalRefineItem->m_weight;
	}
	else if (result == ITEM_REFINE_RESULT_SPECIAL)
	{
		reItem = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_specialRefineItem->m_index, WEARING_NONE, 0, 0, 1);
		needweight = gserver.m_itemProtoList.m_specialRefineItem->m_weight;
	}
	
	if (!reItem)
		return;

	if (ch->m_weight + needweight >= ch->m_maxWeight || ch->m_invenNormal.GetSpace() < 1)
	{
		SysFullInventoryMsg(rmsg, inven->m_tab);
		SEND_Q(rmsg, ch->m_desc);
		delete reItem;
		return;
	}

	GAMELOG << init("ITEM_REFINE", ch)
			<< itemlog(cItem) << delim
			<< itemlog(reItem);

	// 전환된 아이템 삭제
	ItemDeleteMsg(rmsg, cItem);
	SEND_Q(rmsg, ch->m_desc);
	// 전환된 아이템 인벤에서 제거
	RemoveFromInventory(ch, cItem->tab(), cItem->row(), cItem->col(), true, true);
	
	bool bDrop = false;
	if (!AddToInventory(ch, reItem, true, true))
	{
		bDrop = true;
		ch->m_pArea->DropItem(reItem, ch);
		reItem->m_preferenceIndex = ch->m_index;
		ItemDropMsg(rmsg, ch, reItem);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	if (bDrop)
		GAMELOG << delim << "DROP" << end;
	else
		GAMELOG << delim << "INVEN" << end;

	if (bDelMoney)
	{
		ItemDeleteMsg(rmsg, ch->m_moneyItem);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, ch->m_moneyItem, true, true);
	}
	else
	{
		DecreaseFromInventory(ch, ch->m_moneyItem, price);
		ItemUpdateMsg(rmsg, ch->m_moneyItem, -price);
		SEND_Q(rmsg, ch->m_desc);
	}
	
	// 겹쳐져서 인벤에서 tab이 셋팅 되지 않으면
	if (reItem->tab() == -1)
	{
		int r, c;
		if (inven->FindItem(&r, &c, reItem->m_idNum, reItem->m_plus, reItem->m_flag))
		{
			CItem* p = inven->GetItem(r, c);
			
			if (p)
			{
				ItemUpdateMsg(rmsg, p, reItem->Count());
				SEND_Q(rmsg, ch->m_desc);
				delete reItem;
			}
		}
	}
	// 처음 인벤에 들어가서 tab이 셋팅 되면
	else
	{
		ItemAddMsg(rmsg, reItem);
		SEND_Q(rmsg, ch->m_desc);
	}
	
	ItemRefineRepMsg(rmsg, (char)result);
	SEND_Q(rmsg, ch->m_desc);
	
	ch->CalcStatus(true);

	ch->SaveNow(false);
}

void do_ItemOptionAddReq(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
	
	unsigned char subtype;
	char row1, col1, wearpos, row2, col2;
	
	msg >> subtype
		>> row1 >> col1 >> wearpos
		>> row2 >> col2;

	CNetMsg rMsg;
	
	// inven row, col validation
	if (row1 < 0 || row1 >= MAX_INVENTORY_ROWS || col1 < 0 || col1 >= ITEMS_PER_ROW || row2 < 0 || row2 >= MAX_INVENTORY_ROWS || col2 < 0 || col2 >= ITEMS_PER_ROW)
	{
		ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	// 착용한 아이템 이어야지
	if (wearpos < WEARING_HELMET || wearpos > WEARING_BOOTS)
	{
		ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_WEAR);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
	
	CInventory* inven = GET_INVENTORY(ch, INVENTORY_NORMAL);
	
	if (!inven)
	{
		ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return ;
	}
	
	// 옵션 붙일 아이템
	CItem* item = inven->GetItem(row1, col1);

	// 아이템이 없거나 wearpos가 다르거나
	if (item == NULL || item->m_wearPos != wearpos)
	{
		ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
	
	// 옵션 붙일 아이템이 무기 or 방어구 인가
	if (!item || !(item->IsWeaponType() || item->IsArmorType()))
	{
		// 무기나 방어구만 가능하다
		ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_KIND);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	if (item->IsWeaponType() && item->m_nOption >= MAX_WEAPON_OPTION)
	{
		// 최대 옵션 수 초과
		ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_MAX);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
	if (item->IsArmorType() && item->m_nOption >= MAX_ARMOR_OPTION)
	{
		// 최대 옵션 수 초과
		ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_MAX);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	CItem* use = inven->GetItem(row2, col2);
	if (!use)
	{
		ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return ;
	}

	if( use->m_itemProto->m_index !=  2361 )
	{

		if (!(item->m_flag & FLAG_ITEM_OPTION_ENABLE))
		{
			// enable 셋팅 되어 있지 않다
			ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_ENABLE);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
	}
	else
	{
		if( (item->m_itemProto->m_flag & ITEM_FLAG_RARE ) || (item->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE) )
		{
			ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_ENABLE);
			SEND_Q(rMsg, ch->m_desc);
			return ;
		}
		// 유니크 아이템 피와땀 적용 안되게 처리
		if( item->m_idNum == 887 || item->m_idNum == 907 || item->m_idNum == 908 || item->m_idNum == 909 ||
			item->m_idNum == 910 || item->m_idNum == 911 || item->m_idNum == 912 || item->m_idNum == 913 ||
			item->m_idNum == 914 || item->m_idNum == 915 || item->m_idNum == 916 || item->m_idNum == 917 ||
			item->m_idNum == 918 || item->m_idNum == 919 || item->m_idNum == 920 || item->m_idNum == 921 || 
			item->m_idNum == 922 || item->m_idNum == 923 || item->m_idNum == 924 || item->m_idNum == 925 ||
			item->m_idNum == 926 || item->m_idNum == 927 || item->m_idNum == 928 || item->m_idNum == 929 ||
			item->m_idNum == 930 || item->m_idNum == 931 || item->m_idNum == 932 || item->m_idNum == 933 ||
			item->m_idNum == 934 || item->m_idNum == 935 || item->m_idNum == 936 || item->m_idNum == 937 ||
			item->m_idNum == 938 || item->m_idNum == 939 || item->m_idNum == 940 || item->m_idNum == 941 ||
			item->m_idNum == 942 || item->m_idNum == 943 || item->m_idNum == 944 || item->m_idNum == 944 ||
			item->m_idNum == 945 || item->m_idNum == 946 || item->m_idNum == 947 ) 
		{
			ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_ENABLE);
			SEND_Q(rMsg, ch->m_desc);
			return ;
		}

		// 산타 모자 피와담 적용 안되게 처리
		if( item->m_idNum == 1843 || item->m_idNum == 1844 ) 
		{
			ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_ENABLE);
			SEND_Q(rMsg, ch->m_desc);
			return ;
		}

	}

	// 블러드 젬 확인

	if( use->m_itemProto->m_index != 2361 )
	{
		if (use->m_itemProto->m_typeIdx != ITYPE_ETC || use->m_itemProto->m_subtypeIdx != IETC_OPTION || use->m_itemProto->m_num0 != IETC_OPTION_TYPE_BLOOD)
		{
			ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_SYSTEM);
			SEND_Q(rMsg, ch->m_desc);
			return ;
		}
 	}

	const int maxnumoption = 24;	// TODO : 나중에 MAX_NUM_OPTION으로 대체
	// 기존의 옵션 비트 필드에 저장
	LONGLONG bitfield = 0;
	int i;
	for (i=0; i < item->m_nOption; i++)
		bitfield |= ((LONGLONG)1 << item->m_option[i].m_type);

	int type;
	COptionProto* proto;
	while (true)
	{
		// type 결정
	  	type = GetRandom(0, maxnumoption - 1);
		for (i = 0; i < maxnumoption; i++)
		{
			if ((bitfield & ((LONGLONG)1 << ((type + i) % maxnumoption))) == 0)
				break ;
		}

		// 더이상 옵션을 붙일 수 없음
		if (i == maxnumoption)
		{
			ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_MAX);
			SEND_Q(rMsg, ch->m_desc);
			return ;
		}
		else
			type = (type + i) % maxnumoption;

		bitfield |= ((LONGLONG)1 << type);

		// type 가능 검사
		proto = gserver.m_optionProtoList.FindProto(type);

		if (!proto)
		{
			ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_SYSTEM);
			SEND_Q(rMsg, ch->m_desc);
			return ;
		}

		// 붙일 수 있는 옵션인지 검사

		// 무기이면
		if (item->IsWeaponType())
		{
			if (!((1 << item->m_itemProto->m_subtypeIdx) & proto->m_weaponType))
				continue ;
		}

		// 방어구이면
		if (item->IsArmorType())
		{
			if (!((1 << item->m_itemProto->m_subtypeIdx) & proto->m_wearType))
				continue ;
		}

		break;
	}

	item->m_option[item->m_nOption].MakeOptionValue(proto, item->GetItemLevel(true), item->m_nOption);
	item->m_nOption++;

	// 옵션을 붙일수 없는 상태로
	item->m_flag &= ~(FLAG_ITEM_OPTION_ENABLE);


	if( use->m_itemProto->m_index != 2361 )
	{
		GAMELOG << init("ITEM_OPTION_ADD", ch)
				<< itemlog(item)
				<< end;
	}
	else
	{
		GAMELOG << init( "CASH_ITEM_USE", ch )
				<< itemlog( use ) << delim 
				<< end;
	}

#ifdef EVENT_LATTO
	if (gserver.m_bLattoEvent)
	{
		if (GetRandom(1, 10000) <= LATTO_EVENT_BLOOD_PROB)
		{
			if (ch->SettingItemOption() != -1)
				EventLattoMsg(rMsg, MSG_EVENT_LATTO_BLOOD_SUCCESS);
			else
				ItemOptionAddRepMsg(rMsg, item, MSG_OPTION_ADD_SUCCESS);
		}
		else
			ItemOptionAddRepMsg(rMsg, item, MSG_OPTION_ADD_SUCCESS);
	}
	else
	{
		if( use->m_idNum != 2361 )
			ItemOptionAddRepMsg(rMsg, item, MSG_OPTION_ADD_SUCCESS);
		else if( use->m_idNum == 2361 )
			ItemOptionAddRepMsg(rMsg, item, MSG_OPTION_BLOOD_SWEAT_FRUIT_ADD_SUCCESS);
	}
#else
	if( use->m_idNum != 2361 )
		ItemOptionAddRepMsg(rMsg, item, MSG_OPTION_ADD_SUCCESS);
	else if( use->m_idNum == 2361 )
		ItemOptionAddRepMsg(rMsg, item, MSG_OPTION_BLOOD_SWEAT_FRUIT_ADD_SUCCESS);
#endif

	// 결과 메시지
	SEND_Q(rMsg, ch->m_desc);

	// 블러드 젬 없애기
	CNetMsg itemmsg;
	if (use->Count() > 1)
	{
		DecreaseFromInventory(ch, use, 1);
		ItemUpdateMsg(itemmsg, use, -1);
	}
	else
	{
		ItemDeleteMsg(itemmsg, use);
		RemoveFromInventory(ch, use->tab(), use->row(), use->col(), true, true);
	}
	SEND_Q(itemmsg, ch->m_desc);

	// 옵션 업데이트 메시지
	ItemUpdateMsg(itemmsg, item, 0);
	SEND_Q(itemmsg, ch->m_desc);

	ch->CalcStatus(true);

	ch->SaveNow(false);
}

void do_ItemOptionDelReq(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
	
	unsigned char subtype;
	char row1, col1, wearpos, row2, col2;
	
	msg >> subtype
		>> row1 >> col1 >> wearpos
		>> row2 >> col2;

	CNetMsg rMsg;
	
	// inven row, col validation
	if (row1 < 0 || row1 >= MAX_INVENTORY_ROWS || col1 < 0 || col1 >= ITEMS_PER_ROW || row2 < 0 || row2 >= MAX_INVENTORY_ROWS || col2 < 0 || col2 >= ITEMS_PER_ROW)
	{
		ItemOptionDelRepMsg(rMsg, NULL, MSG_OPTION_DEL_ERROR_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	// 착용한 아이템이어야지
	if (wearpos < WEARING_HELMET || wearpos > WEARING_BOOTS)
	{
		ItemOptionDelRepMsg(rMsg, NULL, MSG_OPTION_DEL_ERROR_WEAR);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
	
	CInventory* inven = GET_INVENTORY(ch, INVENTORY_NORMAL);
	
	if (!inven)
	{
		ItemOptionDelRepMsg(rMsg, NULL, MSG_OPTION_DEL_ERROR_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return ;
	}
	
	// 옵션 지울 아이템
	CItem* item = inven->GetItem(row1, col1);

	// 아이템이 없거나 wearpos가 다르거나
	if (item == NULL || item->m_wearPos != wearpos)
	{
		ItemOptionDelRepMsg(rMsg, NULL, MSG_OPTION_DEL_ERROR_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

#ifdef MONSTER_RAID_SYSTEM
	if (item->IsRareItem())
	{
		ItemOptionDelRepMsg(rMsg, NULL, MSG_OPTION_DEL_ERROR_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
#endif // MONSTER_RAID_SYSTEM

#ifdef SET_ITEM
	if (item->IsOriginItem())
	{
		ItemOptionDelRepMsg(rMsg, NULL, MSG_OPTION_DEL_ERROR_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
#endif // MONSTER_RAID_SYSTEM

	// 옵션 지울 아이템이 무기 or 방어구 인가
	if (!item || !(item->IsWeaponType() || item->IsArmorType()))
	{
		// 무기나 방어구만 가능하다
		ItemOptionDelRepMsg(rMsg, NULL, MSG_OPTION_DEL_ERROR_KIND);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	if (item->m_nOption < MAX_ITEM_OPTION_DEL)
	{
		// 최대 옵션 수 초과
		ItemOptionDelRepMsg(rMsg, NULL, MSG_OPTION_DEL_ERROR_NUM);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	// 정화석 확인
	CItem* use = inven->GetItem(row2, col2);
	if (!use)
	{
		ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return ;
	}

	if (use->m_itemProto->m_typeIdx != ITYPE_ETC || use->m_itemProto->m_subtypeIdx != IETC_OPTION || use->m_itemProto->m_num0 != IETC_OPTION_TYPE_CLEAR)
	{
		ItemOptionAddRepMsg(rMsg, NULL, MSG_OPTION_ADD_ERROR_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return ;
	}

	// 정화석 없애기
	CNetMsg itemmsg;
	if (use->Count() > 1)
	{
		DecreaseFromInventory(ch, use, 1);
		ItemUpdateMsg(itemmsg, use, -1);
	}
	else
	{
		ItemDeleteMsg(itemmsg, use);
		RemoveFromInventory(ch, use->tab(), use->row(), use->col(), true, true);
	}
	SEND_Q(itemmsg, ch->m_desc);

	int i;
	for (i=0; i < item->m_nOption; i++)
	{
		item->m_option[i].m_proto = NULL;
		item->m_option[i].m_type = -1;
		item->m_option[i].m_level = 0;
		item->m_option[i].m_value = 0;
		item->m_option[i].m_dbValue = 0;
	}

	item->m_nOption = 0;

	// 옵션을 붙일수 없는 상태로
	item->m_flag &= ~(FLAG_ITEM_OPTION_ENABLE);

	GAMELOG << init("ITEM_OPTION_DEL", ch)
			<< itemlog(item)
			<< end;

	// 옵션 업데이트 메시지
	ItemUpdateMsg(itemmsg, item, 0);
	SEND_Q(itemmsg, ch->m_desc);

	// 결과 메시지
	ItemOptionDelRepMsg(rMsg, item, MSG_OPTION_DEL_SUCCESS);
	SEND_Q(rMsg, ch->m_desc);

	ch->CalcStatus(true);

	ch->SaveNow(false);
}

void do_ItemProcessReq(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();

	unsigned char subtype;

	char useRow, useCol;	// 사용한 가공 문서
	int makeDBIdx;			// 만들려는 가공품 DB Index
	int	num;				// 필요한 재료의 수

	// 재료하나당 정보
	char* tab = NULL;
	char* row = NULL;
	char* col = NULL;
	LONGLONG* count = NULL;
	int i, j;

	msg >> subtype
		>> useRow
		>> useCol
		>> makeDBIdx
		>> num;

	try
	{
		// 사용한 가공문서 확인
		CItem* doc = ch->m_invenNormal.GetItem((int)useRow, (int)useCol);
		
		if (!doc || doc->m_itemProto->m_typeIdx != ITYPE_ONCE || doc->m_itemProto->m_subtypeIdx != IONCE_PROCESS_DOC)
			throw 0;
		
		// 만드려는 가공품 확인
		CItemProto* proto = gserver.m_itemProtoList.FindIndex(makeDBIdx);
		
		if (!proto)
			throw 0;
		
		// 가공품이 아니면 return
		if (proto->m_typeIdx != ITYPE_ETC || proto->m_subtypeIdx != IETC_PROCESS)
			throw 0;
		
		// 만들수 있지 않으면
		if (!(proto->m_flag & ITEM_FLAG_MADE))
			throw 0;
		
		// 가공문서로 만들 수 있는 가공품 인지 확인
		if (doc->m_itemProto->m_num0 != proto->m_num0 || doc->m_itemProto->m_num1 != proto->m_num1)
			throw 0;

		// 여기에서 NUM하고 아이템 프로토의 10개 세팅 상태하고 검사
		for (i = 0, j = 0; i < MAX_MAKE_ITEM_MATERIAL; i++)
		{
			if (proto->m_materialIndex[i] != -1)
				j++;
		}
		if (j != num)
			throw 1;
		
		tab = new char [num];
		row = new char [num];
		col = new char [num];
		count = new LONGLONG [num];
		
		int weight = 0;

		// 재료 하나하나 프로토의 재료와 비교 (DB Index, Count)
		for (i=0; i < num; i++)
		{
			msg >> tab[i]
				>> row[i]
				>> col[i]
				>> count[i];

			// inven row, col validation
			if (tab[i] < 0 || tab[i] > INVENTORY_EVENT|| row[i] < 0 || row[i] >= MAX_INVENTORY_ROWS || col[i] < 0 || col[i] >= ITEMS_PER_ROW)
				throw 0;
			
			CInventory* inven = GET_INVENTORY(ch, tab[i]);
			CItem* material = NULL;
			
			if (!inven)
			{
				throw 0;
			}
			
			material = inven->GetItem(row[i], col[i]);
			
			if (!material)
			{
				// 해당 위치에 재료 없음
				throw 1;
			}
			
			bool bCondition = false;
			
			for (j=0; j < MAX_MAKE_ITEM_MATERIAL; j++)
			{
				if (proto->m_materialIndex[j] == -1)
					continue;
				
				// 재료의 인덱스와 가공품의 재료 인덱스가 같으면
				if (material->m_idNum == proto->m_materialIndex[j])
				{
					// 원하는 재료의수가 같은지
					if (count[i] != proto->m_materialCount[j])
						throw 1;
					// 수량 확인
					if (material->Count() >= proto->m_materialCount[j])
						bCondition = true;

					weight += material->m_itemProto->m_weight * proto->m_materialCount[j];
				}
			}
			
			if (!bCondition)
			{
				// 프로트의 재료와 다르다
				throw 1;
			}
		}

		CNetMsg itemmsg;

		// 시료 생성 : 6% 
		if (GetRandom(1, 10000) <= MAKE_SAMPLE_ON_PROCESS_PROB)
		{
			// 하드코딩 : 시료를 만들어 인벤으로
			CItem* item = NULL;
			int makeIdx;

			switch (proto->m_num0)
			{
				// 채광
			case IETC_PROCESS_STONE:
				makeIdx = GetRandom(0, 1) ? 503 : 504;
				break;

				// 채집
			case IETC_PROCESS_PLANT:
				makeIdx = GetRandom(0, 1) ? 502 : 504;
				break;

				// 차지
			case IETC_PROCESS_ELEMENT:
				makeIdx = GetRandom(0, 1) ? 496 : 502;
				break;

			default:
				break;
			}

			item = gserver.m_itemProtoList.CreateItem(makeIdx, -1, 0, 0, 1);
			
			if (!item)
				goto SAMPLE_SKIP;
			
			bool bCountable = false;
			
			// 인벤에 넣기
			if (AddToInventory(ch, item, true, true))
			{
				// 겹쳐졌는지 검사
				if (item->tab() == -1)
				{
					bCountable = true;
					
					// 수량 변경 알림
					int r, c;
					if (ch->m_invenNormal.FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
					{
						CItem* p = ch->m_invenNormal.GetItem(r, c);
						if (p)
							ItemUpdateMsg(itemmsg, p, item->Count());
					}
				}
				else
					ItemAddMsg(itemmsg, item);
			}
			else
			{				
				// 인젠토리 꽉차서 못 받을 때 Drop
				item = ch->m_pArea->DropItem(item, ch);
				if (!item)
					return;

				CNetMsg dropMsg;
				item->m_preferenceIndex = ch->m_index;
				ItemDropMsg(dropMsg, ch, item);
				ch->m_pArea->SendToCell(dropMsg, ch, true);
			}
			
			// 메시지 보내고
			SEND_Q(itemmsg, ch->m_desc);
			
			// Item LOG
			GAMELOG << init("ITEM_PROCESS_SAMPLE", ch)
					<< itemlog(item)
					<< end;
			
			if (bCountable)
				delete item;
		}

SAMPLE_SKIP:

		// 무게 최대치 확인! 재료무게 총합이 만드려는 아이템 무게보다 작고 무게 오바시
		// bs 수정	: weight(재료), proto(생성 아이템)
		//			: ch->m_weight - weight + proto->m_weight => 최종 무게
		//			: ch->m_maxWeight * 15 / 10 => 최대 소지 무게
		if (ch->m_weight - weight + proto->m_weight > ch->m_maxWeight * 15 / 10)
			throw 4;
//		if (weight < proto->m_weight && !(ch->CheckWeight(proto->m_weight, false, true)))
//			throw 4;

		// 가공문서 없애기
		// 수량 변경
		DecreaseFromInventory(ch, doc, 1);
		
		if (doc->Count() > 0)
		{
			ItemUpdateMsg(itemmsg, doc, -1);
			SEND_Q(itemmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(itemmsg, doc);
			SEND_Q(itemmsg, ch->m_desc);
			
			ch->RemoveItemFromQuickSlot(doc);
			RemoveFromInventory(ch, doc, true, true);
		}

		// 아이템의 필요 스킬 확인 && SSkill 로 확률 굴리기
		CSSkillProto* sskillProto  = gserver.m_sSkillProtoList.FindProto(proto->m_needSSkillIndex);
		if (!sskillProto)
			throw 0;

		CSSkillNode* node = ch->m_sSkillList.m_head;
		while (node)
		{
			// 필요한 스킬에 우선순위가 있고
			if (sskillProto->m_preference != -1)
			{
				// 타입이 같고 필요 스킬보다 우선순위가 같거나 높고 필요레벨보다 높으면 선택
				if (node->m_sskill->m_proto->m_type == sskillProto->m_type && 
					node->m_sskill->m_proto->m_preference >= sskillProto->m_preference &&
					node->m_sskill->m_level >= proto->m_needSSkillLevel)
						break;
			}
			// 운선순위가 없으면 
			else
			{
				// 인덱스가 같고 필요 레벨보다 높으면 선택
				if (node->m_sskill->m_proto->m_index == proto->m_needSSkillIndex &&
					node->m_sskill->m_level >= proto->m_needSSkillLevel)
						break;
			}

			node = node->m_next;
		}
		
		// 필요 가공 스킬이 없음
		if (!node)
			throw 2;

		// 재료 없애기
		for (i=0; i < num; i++)
		{
			CInventory* inven = GET_INVENTORY(ch, tab[i]);
			if (!inven)
				throw 0;
			
			CItem* material = inven->GetItem(row[i], col[i]);
			if (!material)
				throw 0;
			
			CNetMsg itemMsg;
			// 수량 변경
			DecreaseFromInventory(ch, material, count[i]);

			if (material->Count() > 0)
			{
				ItemUpdateMsg(itemMsg, material, -count[i]);
				SEND_Q(itemMsg, ch->m_desc);
			}
			else
			{
				ItemDeleteMsg(itemMsg, material);
				SEND_Q(itemMsg, ch->m_desc);
		
				ch->RemoveItemFromQuickSlot(material);
				RemoveFromInventory(ch, material, true, true);
			}
		}
		
		int prob = GetRandom(1, 10000);
		//  [2/23/2007 KwonYongDae] 확률 100%
		// 실행중인 퀘스트가 가공 퀘스트인경우 
#ifdef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae]
		CQuest *pQuest = NULL;
		if( ch->m_questList.IsQuestType0( QTYPE_KIND_PROCESS_EXPERIENCE ) )
		{
			int	iQuestIndex = 0;

			switch( makeDBIdx )
			{
			case 214 :	iQuestIndex = 150;	break;		// 구리 - 스톤정련 체험
			case 239 :	iQuestIndex = 151;	break;		// E등급 불의 결정 - 원소 정제 체험
			case 251 :	iQuestIndex = 152;	break;		// 크락 추출액 E - 식물 가공 체험	
			}

			pQuest = ch->m_questList.FindQuest(iQuestIndex);
			if( pQuest != NULL && !pQuest->IsCompleted() )
			{
				prob = 0;	//100% 성공률 
			}					
		}
#endif // EXPRIENCE_QUEST	
		if (prob > node->m_sskill->m_proto->m_num0[node->m_sskill->m_level - 1])
		{
			// 확룰 실패
			throw 3;
		}

		CNetMsg rMsg;	// 결과 메세지

		// 가공품 만들어 인벤으로
		CItem* item = NULL;

#ifdef EVENT_LATTO
		if (gserver.m_bLattoEvent)
		{
			CNetMsg rmsg;
			if (GetRandom(1, 10000) <= LATTO_EVENT_PROCESS_PROB)
			{
				item = gserver.m_itemProtoList.CreateItem(makeDBIdx, -1, 0, 0, 2);
				EventLattoMsg(rMsg, MSG_EVENT_LATTO_PROCESS_SUCCESS);
			}
			else
			{
				item = gserver.m_itemProtoList.CreateItem(makeDBIdx, -1, 0, 0, 1);
				ItemProcessRepMsg(rMsg, MSG_PROCESS_SUCCESS);
			}
		}
		else
		{
			item = gserver.m_itemProtoList.CreateItem(makeDBIdx, -1, 0, 0, 1);
			ItemProcessRepMsg(rMsg, MSG_PROCESS_SUCCESS);
		}
#else
		item = gserver.m_itemProtoList.CreateItem(makeDBIdx, -1, 0, 0, 1);
		ItemProcessRepMsg(rMsg, MSG_PROCESS_SUCCESS);
#endif

		
		if (!item)
			throw 0;
		
		bool bCountable = false;
		
		// 인벤에 넣기
		if (AddToInventory(ch, item, true, true))
		{
			// 겹쳐졌는지 검사
			if (item->tab() == -1)
			{
				bCountable = true;
				
				// 수량 변경 알림
				int r, c;
				if (ch->m_invenNormal.FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
				{
					CItem* p = ch->m_invenNormal.GetItem(r, c);
					if (p)
						ItemUpdateMsg(itemmsg, p, item->Count());
				}
			}
			else
				ItemAddMsg(itemmsg, item);
		}
		else
		{				
			// 인젠토리 꽉차서 못 받을 때 Drop
			item = ch->m_pArea->DropItem(item, ch);
			if (!item)
				return;

			CNetMsg dropMsg;
			item->m_preferenceIndex = ch->m_index;
			ItemDropMsg(dropMsg, ch, item);
			ch->m_pArea->SendToCell(dropMsg, ch, true);
		}
		
		// 메시지 보내고
		SEND_Q(itemmsg, ch->m_desc);
		
		// Item LOG
		GAMELOG << init("ITEM_PROCESS", ch)
				<< itemlog(item)
				<< end;
		
		if (bCountable)
			delete item;

		// 성공 메시지 보내기
		SEND_Q(rMsg, ch->m_desc);

		// 가공 성공 이펙트 메시지
		EffectEtcMsg(rMsg, ch, MSG_EFFECT_ETC_ITEM_PROCESS);
		ch->m_pArea->SendToCell(rMsg, ch, true);
#ifdef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae]
		ch->m_questList.CheckComplete( ch );
#endif // EXPRIENCE_QUEST
	}

	catch (const int err)
	{
		CNetMsg errMsg;
		
		// 오류 메시지 보내기			
		switch (err)
		{
		case 0:			// 있어서는 안될 오류
			ItemProcessRepMsg(errMsg, MSG_PROCESS_FAIL_SYSTEM);
			break;
		case 1:			// 재료 정보 잘못됨
			ItemProcessRepMsg(errMsg, MSG_PROCESS_FAIL_MATERIAL);
			break;
		case 2:			// 가공 스킬이 없음
			ItemProcessRepMsg(errMsg, MSG_PROCESS_FAIL_NO_SSKILL);
			break;
		case 3:			// 확률 실패
			ItemProcessRepMsg(errMsg, MSG_PROCESS_FAIL_PROB);
			break;
		case 4:			// 무게초과로 인한 실패
			ItemProcessRepMsg(errMsg, MSG_PROCESS_FAIL_OVER_WEIGHT);

		default:
			ItemProcessRepMsg(errMsg, MSG_PROCESS_FAIL_SYSTEM);
			break;
		}
		
		SEND_Q(errMsg, ch->m_desc);
	}

	if (tab)
		delete [] tab;
	if (row)
		delete [] row;
	if (col)
		delete [] col;
	if (count)
		delete [] count;		
}

void do_ItemMakeReq(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();

	unsigned char subtype;

	char useRow, useCol;	// 사용한 제작 문서
	int makeDBIdx;			// 만들려는 아이템 DB Index
	int	num;				// 필요한 재료의 수

	// 재료하나당 정보
	char* tab = NULL;
	char* row = NULL;
	char* col = NULL;
	LONGLONG* count = NULL;
	int i, j;

	msg >> subtype
		>> useRow
		>> useCol
		>> makeDBIdx
		>> num;

	try
	{
		// 재료가 없음 
		if (num < 1)
			throw 0;

		// 사용한 제작문서 확인
		CItem* doc = ch->m_invenNormal.GetItem((int)useRow, (int)useCol);
		
		if (!doc || doc->m_itemProto->m_typeIdx != ITYPE_ONCE)
			throw 0;

		// 무기방어구 제작문서도 포션제작문서도 아니면
		if (doc->m_itemProto->m_subtypeIdx != IONCE_MAKE_TYPE_DOC && doc->m_itemProto->m_subtypeIdx != IONCE_MAKE_POTION_DOC)
			throw 0;
		
		// 만드려는 아이템 확인
		CItemProto* proto = gserver.m_itemProtoList.FindIndex(makeDBIdx);
		
		if (!proto)
			throw 0;
		
		// 무기나 방어구 포션 아니면 return
		if (proto->m_typeIdx != ITYPE_WEAPON && proto->m_typeIdx != ITYPE_WEAR && proto->m_typeIdx != ITYPE_POTION)
			throw 0;

#ifndef CREATE_SORCERER
		// 소서러 생성 불가이면 소서러 아이템도 제작 불가
		if (proto->m_jobFlag & ( 1 << JOB_SORCERER ) && proto->m_typeIdx != ITYPE_POTION )
			throw 0;
#endif // #ifndef CREATE_SORCERER
		
		// 만들수 있지 않으면
		if (!(proto->m_flag & ITEM_FLAG_MADE))
			throw 0;
		
		// 제작문서로 만들 수 있는 아이템 타입 인지 확인
		if (doc->m_itemProto->m_num0 != proto->m_typeIdx || !(doc->m_itemProto->m_num1 & (1 << proto->m_subtypeIdx)))
			throw 0;

		// 포션인경우 만들 수 있는 등급인지 확인
		if (proto->m_typeIdx == ITYPE_POTION)
		{
			if (doc->m_itemProto->m_num2 != proto->m_num2)
				throw 0;
		}
		// 무기, 방어구 제작문서로 만들 수 있는 아이템 레벨 인지 확인
		else
		{
			if (doc->GetItemLevel(true) != proto->GetItemProtoLevel())
				throw 0;
		}

		// 제작 아이템 재료수와 비교
		for (i = 0, j = 0; i < MAX_MAKE_ITEM_MATERIAL; i++)
		{
			if (proto->m_materialIndex[i] != -1)
				j++;
		}
		if (j != num)
			throw 1;
		
		tab = new char [num];
		row = new char [num];
		col = new char [num];
		count = new LONGLONG [num];

		int weight = 0;
		
		// 재료 하나하나 프로토의 재료와 비교 (DB Index, Count)
		for (i=0; i < num; i++)
		{
			msg >> tab[i]
				>> row[i]
				>> col[i]
				>> count[i];

			// inven row, col validation
			if (tab[i] < 0 || tab[i] > INVENTORY_EVENT|| row[i] < 0 || row[i] >= MAX_INVENTORY_ROWS || col[i] < 0 || col[i] >= ITEMS_PER_ROW)
				throw 0;
			
			CInventory* inven = GET_INVENTORY(ch, tab[i]);
			CItem* material = NULL;
			
			if (!inven)
			{
				throw 0;
			}
			
			material = inven->GetItem(row[i], col[i]);
			
			if (!material)
			{
				// 해당 위치에 재료 없음
				throw 1;
			}
			
			bool bCondition = false;
			
			for (j=0; j < MAX_MAKE_ITEM_MATERIAL; j++)
			{
				if (proto->m_materialIndex[j] == -1)
					continue;
				
				// 재료의 인덱스와 아이템 재료의 인덱스가 같으면
				if (material->m_idNum == proto->m_materialIndex[j])
				{
					// 원하는 재료의수가 같은지
					if (count[i] != proto->m_materialCount[j])
						throw 1;
					// 수량 확인
					if (material->Count() >= proto->m_materialCount[j])
						bCondition = true;

					weight += material->m_itemProto->m_weight * proto->m_materialCount[j];
					break;
				}
			}
			
			if (!bCondition)
			{
				// 프로트의 재료와 다르다
				throw 1;
			}
		}

		// 무게 최대치 확인! 재료무게 총합이 만드려는 아이템 무게보다 작고 무게 오바시
		// bs 수정	: weight(재료), proto(생성 아이템)
		//			: ch->m_weight - weight + proto->m_weight => 최종 무게
		//			: ch->m_maxWeight * 15 / 10 => 최대 소지 무게
		if (doc->m_itemProto->m_subtypeIdx == IONCE_MAKE_POTION_DOC)
		{
			if (ch->m_weight - weight + proto->m_weight * doc->m_itemProto->m_num3 > ch->m_maxWeight * 15 / 10)
				throw 4;
		}
		else
		{
			if (ch->m_weight - weight + proto->m_weight > ch->m_maxWeight * 15 / 10)
				throw 4;
		}
//		if (weight < proto->m_weight && !(ch->CheckWeight(proto->m_weight, false, true)))
//			throw 5;

		// 아이템의 필요 스킬 확인 && SSkill 로 확률 굴리기
		CSSkillProto* sskillProto  = gserver.m_sSkillProtoList.FindProto(proto->m_needSSkillIndex);

		// 없으면
		if (!sskillProto)
			throw 0;

		// sskillProto
		CSSkillNode* node = ch->m_sSkillList.m_head;
		node = ch->m_sSkillList.m_head;
		
		while (node)
		{
			// 필요한 스킬에 우선순위가 있고
			if (sskillProto->m_preference != -1)
			{
				// 타입이 같고 필요 스킬보다 우선순위가 같거나 높으면 선택
				if (node->m_sskill->m_proto->m_type == sskillProto->m_type && 
					node->m_sskill->m_proto->m_preference >= sskillProto->m_preference)
					break;
			}
			// 운선순위가 없으면 
			else
			{
				// 인덱스가 같고 필요 레벨보다 높으면 선택
				if (node->m_sskill->m_proto->m_index == proto->m_needSSkillIndex &&
					node->m_sskill->m_level >= proto->m_needSSkillLevel)
					break;
			}
			
			node = node->m_next;
		}
		
		// 필요 제작 스킬이 없음
		if (!node)
			throw 2;

		// 재료 없애기
		for (i=0; i < num; i++)
		{
			CInventory* inven = GET_INVENTORY(ch, tab[i]);
			if (!inven)
				throw 0;
			
			CItem* material = inven->GetItem(row[i], col[i]);
			if (!material)
				throw 0;
			
			CNetMsg itemMsg;
			// 수량 변경
			DecreaseFromInventory(ch, material, count[i]);

			if (material->Count() > 0)
			{
				ItemUpdateMsg(itemMsg, material, -count[i]);
				SEND_Q(itemMsg, ch->m_desc);
			}
			else
			{
				ItemDeleteMsg(itemMsg, material);
				SEND_Q(itemMsg, ch->m_desc);
		
				ch->RemoveItemFromQuickSlot(material);
				RemoveFromInventory(ch, material, true, true);
			}
		}

		int	successProb;
		
		// 포션
		if (doc->m_itemProto->m_subtypeIdx == IONCE_MAKE_POTION_DOC)
			successProb= GetMakeProb(node->m_sskill->m_proto->m_num0[node->m_sskill->m_level-1], proto->m_num2) + node->m_sskill->m_proto->m_num1[node->m_sskill->m_level-1];
		else
			successProb= GetMakeProb(node->m_sskill->m_proto->m_num0[node->m_sskill->m_level-1], proto->GetItemProtoLevel()) + node->m_sskill->m_proto->m_num1[node->m_sskill->m_level-1];
#ifdef MAL_TEST_SERVER
		successProb *= 3;
#endif

		// 최대 성공률 80%
		if (successProb > MAX_ITEM_MAKE_SUCCESS_PROB)
			successProb = MAX_ITEM_MAKE_SUCCESS_PROB;

		int prob = GetRandom(1, 10000);

		//  [2/23/2007 KwonYongDae] 확률 100% 
		// 실행중인 퀘스트가 제작 퀘스트인경우 
#ifdef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae]
		CQuest *pQuest = NULL;
		if( ch->m_questList.IsQuestType0( QTYPE_KIND_MAKE_EXPERIENCE ) )
		{
			int	iQuestIndex = 0;

			switch( makeDBIdx )
			{
			case 308 :	iQuestIndex = 154;	break;		// 타바르 - 무기제작 체험
			}

			pQuest = ch->m_questList.FindQuest(iQuestIndex);
			if( pQuest != NULL && !pQuest->IsCompleted() )
			{
				prob = 0;	//100% 성공률 
			}
		}
#endif // EXPRIENCE_QUEST
		// 확룰 실패
		if ( prob > successProb)
			throw 3;

		// 아이템 만들어 인벤으로
		CItem* item = NULL;

		// 포션이면 갯수만큼
		if (doc->m_itemProto->m_subtypeIdx == IONCE_MAKE_POTION_DOC)
			item = gserver.m_itemProtoList.CreateItem(makeDBIdx, -1, 0, 0, doc->m_itemProto->m_num3);
		else
			item = gserver.m_itemProtoList.CreateItem(makeDBIdx, -1, 0, 0, 1);
		
		if (!item)
			throw 0;

		// 방어구 무기 제작문서의 경우 확률 실패해도 재료만 사라진다 : 제작문서 없애기
		CNetMsg itemmsg;
		// 수량 변경
		DecreaseFromInventory(ch, doc, 1);
		
		if (doc->Count() > 0)
		{
			ItemUpdateMsg(itemmsg, doc, -1);
			SEND_Q(itemmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(itemmsg, doc);
			SEND_Q(itemmsg, ch->m_desc);
			
			ch->RemoveItemFromQuickSlot(doc);
			RemoveFromInventory(ch, doc, true, true);
		}
		
		bool bCountable = false;
		
		// 인벤에 넣기
		if (AddToInventory(ch, item, true, true))
		{
			// 겹쳐졌는지 검사
			if (item->tab() == -1)
			{
				bCountable = true;
				
				// 수량 변경 알림
				int r, c;
				if (ch->m_invenNormal.FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
				{
					CItem* p = ch->m_invenNormal.GetItem(r, c);
					if (p)
						ItemUpdateMsg(itemmsg, p, item->Count());
				}
			}
			else
				ItemAddMsg(itemmsg, item);
		}
		else
		{				
			// 인젠토리 꽉차서 못 받을 때 Drop
			item = ch->m_pArea->DropItem(item, ch);
			if (!item)
				return;

			CNetMsg dropMsg;
			item->m_preferenceIndex = ch->m_index;
			ItemDropMsg(dropMsg, ch, item);
			ch->m_pArea->SendToCell(dropMsg, ch, true);

			// 성공 메시지
			CNetMsg rMsg;
			ItemMakeRepMsg(rMsg, MSG_MAKE_SUCCESS);
			SEND_Q(rMsg, ch->m_desc);
			goto EXIT_FUNC;
		}
		
		// 메시지 보내고
		SEND_Q(itemmsg, ch->m_desc);
		
		// Item LOG
		GAMELOG << init("ITEM_MAKE", ch)
				<< itemlog(item)
				<< end;
		
		if (bCountable)
			delete item;

		// 성공 메시지
		CNetMsg rMsg;
		ItemMakeRepMsg(rMsg, MSG_MAKE_SUCCESS);
		SEND_Q(rMsg, ch->m_desc);

		// 제작 성공 이펙트 메시지
		EffectEtcMsg(rMsg, ch, MSG_EFFECT_ETC_ITEM_MAKE);
		ch->m_pArea->SendToCell(rMsg, ch, true);
#ifdef EXPRIENCE_QUEST	//  [2/26/2007 KwonYongDae]
		ch->m_questList.CheckComplete( ch );		// 퀘스트 완료 체크 
#endif // EXPRIENCE_QUEST
		ch->SaveNow(false);
	}

	catch (const int err)
	{
		CNetMsg errMsg;
		
		// 오류 메시지 보내기			
		switch (err)
		{
		case 0:			// 있어서는 안될 오류
			ItemMakeRepMsg(errMsg, MSG_MAKE_FAIL_SYSTEM);
			break;
		case 1:			// 재료 정보 잘못됨
			ItemMakeRepMsg(errMsg, MSG_MAKE_FAIL_MATERIAL);
			break;
		case 2:			// 제작 스킬이 없음
			ItemMakeRepMsg(errMsg, MSG_MAKE_FAIL_NO_SSKILL);
			break;
		case 3:			// 확률 실패
			ItemMakeRepMsg(errMsg, MSG_MAKE_FAIL_PROB);
			break;
		case 4:			// 현재 스킬로 제작 불가능 아이템
			ItemMakeRepMsg(errMsg, MSG_MAKE_FAIL_ITEM_LEVEL);
			break;
		case 5:			// 무게초과
			ItemMakeRepMsg(errMsg, MSG_MAKE_FAIL_OVER_WEIGHT);
			break;

		default:
			ItemMakeRepMsg(errMsg, MSG_MAKE_FAIL_SYSTEM);
			break;
		}
		
		SEND_Q(errMsg, ch->m_desc);
	}
	
EXIT_FUNC:
	if (tab)
		delete [] tab;
	if (row)
		delete [] row;
	if (col)
		delete [] col;
	if (count)
		delete [] count;		
}

void do_ItemMixReq(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();

	unsigned char subtype;

	char useRow, useCol;		// 재활의 상자

	char row[MAX_ACCESSORY_MIX], col[MAX_ACCESSORY_MIX];
	CNetMsg rMsg;

	msg >> subtype
		>> useRow
		>> useCol;

	CItem* use = ch->m_invenNormal.GetItem((int)useRow, (int)useCol);

	// 재활의 상자인가
	if (!use || use->m_itemProto->m_typeIdx != ITYPE_ONCE || use->m_itemProto->m_subtypeIdx != IONCE_BOX || use->m_itemProto->m_num0 != IONCE_BOX_REMAKE)
	{
		ItemMixRepMsg(rMsg, MSG_MIX_FAIL_REMAKEBOX);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	int i;
	for (i=0; i < MAX_ACCESSORY_MIX; i++)
	{
		msg >> row[i]
			>> col[i];

		// row, col validation
		if (row[i] < 0 || row[i] >= MAX_INVENTORY_ROWS || col[i] < 0 || col[i] >= ITEMS_PER_ROW)
		{
			ItemMixRepMsg(rMsg, MSG_MIX_FAIL_SYSTEM);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
	}

	// 악세사리 확인 작업
	CItem* oItem[MAX_ACCESSORY_MIX];

	// 옵션 한개 붙은 악세사리가 있다
	bool bOption = false;

	for (i=0; i < MAX_ACCESSORY_MIX; i++)
	{
		oItem[i] = ch->m_invenNormal.GetItem((int)row[i], (int)col[i]);

		// 아이템이 없거나 악세사리가 아니거나 착용하고 있거나
		if (!oItem[i] || !(oItem[i]->IsAccessary()) || oItem[i]->m_wearPos != WEARING_NONE || oItem[i]->m_itemProto->m_flag & ITEM_FLAG_CASH)
		{
			ItemMixRepMsg(rMsg, MSG_MIX_FAIL_ACCESSORY);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}

#ifdef MONSTER_RAID_SYSTEM
		if (oItem[i]->IsRareItem())
		{
			ItemMixRepMsg(rMsg, MSG_MIX_FAIL_ACCESSORY);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
#endif // MONSTER_RAID_SYSTEM

#ifdef SET_ITEM
		if (oItem[i]->IsOriginItem())
		{
			ItemMixRepMsg(rMsg, MSG_MIX_FAIL_ACCESSORY);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
#endif // MONSTER_RAID_SYSTEM


		// 옵션이 1개인 아이템이 있다
		if (oItem[i]->m_nOption == 1)
			bOption = true;
	}

	// 재활의 상자 없애기
	CNetMsg itemmsg;
	// 수량 변경
	DecreaseFromInventory(ch, use, 1);
	
	if (use->Count() > 0)
	{
		ItemUpdateMsg(itemmsg, use, -1);
		SEND_Q(itemmsg, ch->m_desc);
	}
	else
	{
		ItemDeleteMsg(itemmsg, use);
		SEND_Q(itemmsg, ch->m_desc);
		
		ch->RemoveItemFromQuickSlot(use);
		RemoveFromInventory(ch, use, true, true);
	}

	// MAX_ACCESSORY_MIX 중 하나 선택
	int idx = GetRandom(0, MAX_ACCESSORY_MIX - 1);

	// 새로 생기는 아이템
	CItem* item = gserver.m_itemProtoList.CreateItem(oItem[idx]->m_itemProto->m_index, -1, 0, 0, 1);

	if (!item)
	{
		ItemMixRepMsg(rMsg, MSG_MIX_FAIL_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	int num;	// 옵션 붙는 갯수

	if (bOption)
		num = GetRandom(2, 4);
	else
		num = GetRandom(3, 5);

	int j;
	int* type = new int[num];

	LONGLONG bitfield = 0;		// TODO : 옵션이 64개 넘으면 고쳐야 함!
	const int maxnumoption = 24;	// TODO : 나중에 MAX_NUM_OPTION으로 대체

	// 옵션 타입 결정
	i = 0;
	while (i < num)
	{
		// type[i] 결정
		type[i] = GetRandom(0, maxnumoption - 1);

		// type[i]가 중복인 검사
		// 중복이면 type[i]를 1씩 증가하면서 중복 검사
		for (j = 0; j < maxnumoption; j++)
		{
			if ((bitfield & ((LONGLONG)1 << ((type[i] + j) % maxnumoption))) == 0)
				break;
		}

		// 더 이상 옵션 붙일 수 없음
		if (j == maxnumoption)
			num = i;
		else
			type[i] = (type[i] + j) % maxnumoption;

		bitfield |= ((LONGLONG)1 << type[i]);
		
		// 옵션번호로 붙을 수 있는 옵션인지 검사
		COptionProto* proto = gserver.m_optionProtoList.FindProto(type[i]);
		if (!proto)
			continue;
		
		// 악세사리중에서 붙을 수 없으면 다시
		if (!((1 << item->m_itemProto->m_subtypeIdx) & proto->m_accessoryType))
			continue;

		i++;
	}

	// 옵션 부여
	for (i=0; i < num; i++)
	{
		COptionProto* proto = gserver.m_optionProtoList.FindProto(type[i]);
		
		if (!proto)
		{
			ItemMixRepMsg(rMsg, MSG_MIX_FAIL_SYSTEM);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
		
		item->m_nOption++;
		// Level, Value 셋팅
		item->m_option[i].MakeOptionValue(proto, ch->m_level, num);
	}

	delete [] type;

	// 악세사리 삭제하기
	for (i=0; i < MAX_ACCESSORY_MIX; i++)
	{
		ItemDeleteMsg(itemmsg, oItem[i]);
		SEND_Q(itemmsg, ch->m_desc);

		ch->RemoveItemFromQuickSlot(oItem[i]);
		RemoveFromInventory(ch, oItem[i], true, true);
	}
	
	// 새 악세사리 인벤에 넣기
	if (AddToInventory(ch, item, true, true))
	{
		// 겹쳐졌는지 검사
		if (item->tab() == -1)
		{
			delete item;
			item = NULL;
			return;
		}
		else
			ItemAddMsg(itemmsg, item);
	}
	else
	{				
		// 인젠토리 꽉차서 못 받을 때 Drop
		item = ch->m_pArea->DropItem(item, ch);
		if (!item)
			return;

		CNetMsg dropMsg;
		item->m_preferenceIndex = ch->m_index;
		ItemDropMsg(dropMsg, ch, item);
		ch->m_pArea->SendToCell(dropMsg, ch, true);

		// 성공 메시지
		ItemMixRepMsg(rMsg, MSG_MIX_SUCCESS);
		SEND_Q(rMsg, ch->m_desc);
	}
	
	// 메시지 보내고
	SEND_Q(itemmsg, ch->m_desc);
	
	// Item LOG
	GAMELOG << init("ITEM_MIX", ch)
			<< itemlog(item)
			<< end;

	// 성공 메시지
	ItemMixRepMsg(rMsg, MSG_MIX_SUCCESS);
	SEND_Q(rMsg, ch->m_desc);

	ch->SaveNow(false);
}


void do_ItemArcaneReq(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();

	unsigned char subtype;

	char useRow, useCol;		// 비밀의 상자

	char row[MAX_ARCANE_MIX], col[MAX_ARCANE_MIX];
	CNetMsg rMsg;

	msg >> subtype
		>> useRow
		>> useCol;

	CItem* use = ch->m_invenNormal.GetItem((int)useRow, (int)useCol);

	// 비밀의 상자인가
	if (!use || use->m_itemProto->m_typeIdx != ITYPE_ONCE || use->m_itemProto->m_subtypeIdx != IONCE_BOX || use->m_itemProto->m_num0 != IONCE_BOX_ARCANE)
	{
		ItemArcaneRepMsg(rMsg, MSG_ARCANE_FAIL_ARCANEBOX);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	int i;
	for (i=0; i < MAX_ARCANE_MIX; i++)
	{
		msg >> row[i]
			>> col[i];

		// row, col validation : 엄한값이 오다.
		if (row[i] < 0 || row[i] >= MAX_INVENTORY_ROWS || col[i] < 0 || col[i] >= ITEMS_PER_ROW)
		{
			ItemArcaneRepMsg(rMsg, MSG_ARCANE_FAIL_SYSTEM);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}
	}

	// 재료 확인 작업 : 클라이언트가 보낸 그대로의 순서 
	CItem* oItem[MAX_ARCANE_MIX];

	bool bMaterial[MAX_ARCANE_MIX];
	memset(bMaterial, false, sizeof(bool) * MAX_ARCANE_MIX);

	for (i=0; i < MAX_ARCANE_MIX; i++)
	{
		oItem[i] = ch->m_invenNormal.GetItem((int)row[i], (int)col[i]);

		// 아이템이 없거나 착용하고 있거나
		if (!oItem[i] || oItem[i]->m_wearPos != WEARING_NONE)
		{
			ItemArcaneRepMsg(rMsg, MSG_ARCANE_FAIL_MATERIAL);
			SEND_Q(rMsg, ch->m_desc);
			return;
		}

		switch (i)
		{
		case ITEM_ARCANE_MATERIAL_UPGRADE:
			// 제련석이 아니다
			if (oItem[i]->m_itemProto->m_typeIdx != ITYPE_ETC || oItem[i]->m_itemProto->m_subtypeIdx != IETC_UPGRADE || oItem[i]->m_itemProto->m_num0 != IETC_UPGRADE_GENERAL || oItem[i]->m_flag == 0)
			{
				ItemArcaneRepMsg(rMsg, MSG_ARCANE_FAIL_UPGRADE);
				SEND_Q(rMsg, ch->m_desc);
				return;
			}
			break;

		case ITEM_ARCANE_MATERIAL_ACCESSORY:
			// 악세사리가 아니다
			if (!oItem[i]->IsAccessary())
			{
				ItemArcaneRepMsg(rMsg, MSG_ARCANE_FAIL_ACCESSORY);
				SEND_Q(rMsg, ch->m_desc);
				return;
			}			
			break;

		case ITEM_ARCANE_MATERIAL_SAMPLE:
			// 시료가 아니다
			if (oItem[i]->m_itemProto->m_typeIdx != ITYPE_ETC || oItem[i]->m_itemProto->m_subtypeIdx != IETC_SAMPLE)
			{
				ItemArcaneRepMsg(rMsg, MSG_ARCANE_FAIL_SAMPLE);
				SEND_Q(rMsg, ch->m_desc);
				return;
			}
			break;
		}
	}

	// 비밀의 상자 없애기
	CNetMsg itemmsg;
	// 수량 변경
	DecreaseFromInventory(ch, use, 1);
	
	if (use->Count() > 0)
	{
		ItemUpdateMsg(itemmsg, use, -1);
		SEND_Q(itemmsg, ch->m_desc);
	}
	else
	{
		ItemDeleteMsg(itemmsg, use);
		SEND_Q(itemmsg, ch->m_desc);
		
		ch->RemoveItemFromQuickSlot(use);
		RemoveFromInventory(ch, use, true, true);
	}

	// 새로 생기는 아이템 : 포션
	CItem* item = gserver.m_itemProtoList.CreateItem(oItem[ITEM_ARCANE_MATERIAL_SAMPLE]->m_itemProto->m_num0, -1, 0, 0, 1);

	if (!item)
	{
		ItemArcaneRepMsg(rMsg, MSG_ARCANE_FAIL_SYSTEM);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	// 포션의 스킬레벨 셋팅
	if (oItem[ITEM_ARCANE_MATERIAL_UPGRADE]->m_flag % 5 == 0)
		item->m_flag = oItem[ITEM_ARCANE_MATERIAL_UPGRADE]->m_flag / 5;
	else
		item->m_flag = oItem[ITEM_ARCANE_MATERIAL_UPGRADE]->m_flag / 5 + 1;

	// 060111 : BS : 아이템 60레벨 이상은 12레벨로 고정
	if (item->m_flag > 12)
		item->m_flag = 12;

	// 재료 삭제하기
	for (i=0; i < MAX_ARCANE_MIX; i++)
	{
		DecreaseFromInventory(ch, oItem[i], 1);
		if (oItem[i]->Count() > 0)
		{
			ItemUpdateMsg(itemmsg, oItem[i], -1);
			SEND_Q(itemmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(itemmsg, oItem[i]);
			SEND_Q(itemmsg, ch->m_desc);
			
			ch->RemoveItemFromQuickSlot(oItem[i]);
			RemoveFromInventory(ch, oItem[i], true, true);
		}
	}
	
	bool bCountable = false;
	// 만들어진 포션 인벤에 넣기
	if (AddToInventory(ch, item, true, true))
	{
		// 겹쳐졌는지 검사
		if (item->tab() == -1)
		{
			bCountable = true;
			
			// 수량 변경 알림
			int r, c;
			if (ch->m_invenNormal.FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
			{
				CItem* p = ch->m_invenNormal.GetItem(r, c);
				if (p)
					ItemUpdateMsg(itemmsg, p, item->Count());
			}
		}
		else
			ItemAddMsg(itemmsg, item);
	}
	else
	{				
		// 인젠토리 꽉차서 못 받을 때 Drop
		item = ch->m_pArea->DropItem(item, ch);
		if (!item)
			return;

		CNetMsg dropMsg;
		item->m_preferenceIndex = ch->m_index;
		ItemDropMsg(dropMsg, ch, item);
		ch->m_pArea->SendToCell(dropMsg, ch, true);
	}
	
	// 메시지 보내고
	SEND_Q(itemmsg, ch->m_desc);
	
	// Item LOG
	GAMELOG << init("ITEM_ARCANE", ch)
			<< itemlog(item)
			<< end;
	
	if (bCountable)
		delete item;

	// 성공 메시지
	ItemArcaneRepMsg(rMsg, MSG_ARCANE_SUCCESS);
	SEND_Q(rMsg, ch->m_desc);
}

#ifdef ENABLE_CHANGEJOB
void do_ItemChangeWeaponReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	char row = -1;
	char col = -1;
	int itemindex = -1;
	int changetype = -1;		// 무기는 서브타입, 방어구는 직업

	msg >> row
		>> col
		>> itemindex
		>> changetype;

	CItem* olditem = ch->m_invenNormal.GetItem(row, col);
	if (olditem == NULL)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (olditem->m_index != itemindex)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (olditem->GetItemLevel(true) < 29)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (olditem->m_wearPos != WEARING_NONE)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 무기 방어구 모두 교환 가능하게 수정 : 051219 : bs
	if (!olditem->IsWeaponType() && (!olditem->IsArmorType() || olditem->m_itemProto->m_subtypeIdx == IWEAR_SHIELD))
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if ((olditem->IsWeaponType() && olditem->m_itemProto->m_subtypeIdx == changetype)
		|| (olditem->IsArmorType() && (olditem->m_itemProto->m_jobFlag & (1 << changetype))))
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 교환 가능 아이템 검사
	if (!(olditem->m_itemProto->m_flag & ITEM_FLAG_CHANGE) 
#ifdef PLATINUM
		|| olditem->IsUsedPlatinumSpecial()
#endif
		)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 대여 무기 검사
	if (olditem->IsLent())
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 교환시 plus 수치 보존 및 옵션 제거 : 051219 : bs
	int nNewPlus = olditem->m_plus;
	int nNewFlag = olditem->m_flag & ~FLAG_ITEM_OPTION_ENABLE;
//
//	if (olditem->m_plus > 0 || olditem->m_nOption > 0 || (olditem->m_flag & FLAG_ITEM_SEALED))
//	{
//		ItemChangeWeaponRepMsg(rmsg, 0, -1);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}

	// 교환시 필요 나스 수정 : 051219 : bs
	LONGLONG needmoney = (olditem->IsWeaponType()) ? 5000 : 3000;
	needmoney = needmoney * (nNewPlus * 2 + 10) * (olditem->GetItemLevel(true) - 28) / 10;

#ifdef EVENT_CHANGE_ARMOR_2005
	needmoney = 0;
#endif

	if (needmoney > 0 && (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < needmoney))
	{
		ItemChangeWeaponRepMsg(rmsg, -1, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int newindex = -1;
	int i;
	for (i = 0; newindex == -1 && i < gserver.m_itemProtoList.m_nCount; i++)
	{
		// 메인 타입이 같고, 교환 타입이고, 레벨이 같고 교환 가능 아이템일때
		if (gserver.m_itemProtoList.m_protoItems[i].m_typeIdx == olditem->m_itemProto->m_typeIdx
			&& gserver.m_itemProtoList.m_protoItems[i].GetItemProtoLevel() == olditem->GetItemLevel(true)
			&& (gserver.m_itemProtoList.m_protoItems[i].m_flag & ITEM_FLAG_CHANGE))
		{
			if ((olditem->IsWeaponType() && gserver.m_itemProtoList.m_protoItems[i].m_subtypeIdx == changetype)
				|| (olditem->IsArmorType() && gserver.m_itemProtoList.m_protoItems[i].m_subtypeIdx == olditem->m_itemProto->m_subtypeIdx && (gserver.m_itemProtoList.m_protoItems[i].m_jobFlag & (1 << changetype))))
			newindex = gserver.m_itemProtoList.m_protoItems[i].m_index;
		}
	}

	if (newindex == -1)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CItem* newitem = gserver.m_itemProtoList.CreateItem(newindex, -1, nNewPlus, nNewFlag, 1);
	if (newitem == NULL)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("ITEM CHANGE WEAPON", ch)
			<< "NEED MONEY" << delim
			<< needmoney << delim
			<< "OLD ITEM" << delim
			<< itemlog(olditem) << delim
			<< "NEW ITEM" << delim
			<< itemlog(newitem)
			<< end;

	int olddb = olditem->m_itemProto->m_index;
	int newdb = newitem->m_itemProto->m_index;

	ItemDeleteMsg(rmsg, olditem);
	SEND_Q(rmsg, ch->m_desc);
	RemoveFromInventory(ch, olditem, true, true);

	AddToInventory(ch, newitem, false, false);
	ItemAddMsg(rmsg, newitem);
	SEND_Q(rmsg, ch->m_desc);

#ifndef EVENT_CHANGE_ARMOR_2005
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
#endif

	ItemChangeWeaponRepMsg(rmsg, olddb, newdb);
	SEND_Q(rmsg, ch->m_desc);
}
#endif

void do_ItemProcessNPC(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int itemDBIndex;
	int count;

	msg >> itemDBIndex
		>> count;

	CItemProto* proto = gserver.m_itemProtoList.FindIndex(itemDBIndex);
	if (proto == NULL)
		return ;

	if (proto->m_typeIdx != ITYPE_ETC || proto->m_subtypeIdx != IETC_PROCESS)
		return ;

	// 확률 구하기 : a_level1_num0를 확률로 사용 (m_num0[1])
	int sskillindex = proto->m_needSSkillIndex;
	CSSkillProto* skillproto = gserver.m_sSkillProtoList.FindProto(sskillindex);
	if (skillproto == NULL)
		return ;
	int skillprob = skillproto->m_num0[1];

	if (count < 1 || count > 10)
	{
		ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_COUNT);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 필요한 재료 구하기 : 가공품은 필요 재료를 무조건 0번 인덱스에 저장
	int needItemIndex = proto->m_materialIndex[0];
	int needItemCount = proto->m_materialCount[0];
	if (needItemIndex < 1 || needItemCount < 1)
		return ;

	LONGLONG totalCount = count * needItemCount;
	bool bDeleteItem = false;

	// 재료를 가지고 있는지 검사
	int r, c;
	if (!ch->m_invenNormal.FindItem(&r, &c, needItemIndex, 0, 0))
	{
		ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_NOHAVE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	CItem* item = ch->m_invenNormal.GetItem(r, c);
	if (!item || item->Count() < totalCount)
	{
		ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_NOHAVE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	else if (item->Count() == totalCount)
		bDeleteItem = true;

	// 종류 구하기 : 가공품의 종류는 a_num0에 저장되고 스톤, 크락, 원소 순으로
	int type = proto->m_num0;
	// 등급 구하기 : 가공품의 등급은 a_num1에 저장되고 E 등급이 0으로 시작해서 A 등급이 4로 끝남
	int grade = proto->m_num1;

	if (type < 0 || type >= 3 || grade < 0 || grade >=5)
		return ;

	// type과 grade에 해당하는 수수료 테이블
	int pricetable[3][5] = {
		{10, 21, 40, 63, 94},		// 스톤
		{10, 21, 40, 63, 94},		// 크락
		{10, 21, 40, 63, 94}		// 원소
	};

	// 소유금액 검사
	LONGLONG needmoney = pricetable[type][grade] * totalCount;
	bool bDeleteMoney = false;

	if (!ch->m_moneyItem || ch->m_moneyItem->Count() < needmoney)
	{
		ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	else if (ch->m_moneyItem->Count() == needmoney)
		bDeleteMoney = true;

	// 아이템 감소
	if (bDeleteItem)
	{
		ItemDeleteMsg(rmsg, item);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, item, true, true);
	}
	else
	{
		DecreaseFromInventory(ch, item, totalCount);
		ItemUpdateMsg(rmsg, item, -totalCount);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 확률로 성공 판단
	if (GetRandom(1, 10000) <= skillprob)
	{
		// item 생성
		item = gserver.m_itemProtoList.CreateItem(itemDBIndex, -1, 0, 0, count);
		if (!item)
		{
			GAMELOG << init("ITEM PROCESS NPC ERROR", ch)
					<< "ITEM" << delim
					<< itemDBIndex << delim
					<< "COUNT" << delim
					<< count
					<< end;
			ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_FAIL);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// 돈 감소
		if (bDeleteMoney)
		{
			ItemDeleteMsg(rmsg, ch->m_moneyItem);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, ch->m_moneyItem, true, true);
		}
		else
		{
			DecreaseFromInventory(ch, ch->m_moneyItem, needmoney);
			ItemUpdateMsg(rmsg, ch->m_moneyItem, -needmoney);
			SEND_Q(rmsg, ch->m_desc);
		}

		// 인벤에 넣기
		if (AddToInventory(ch, item, true, true))
		{
			// 겹쳐졌는지 검사
			if (item->tab() < 0)
			{
				// 겹쳤다
				if (ch->m_invenNormal.FindItem(&r, &c, itemDBIndex, 0, 0))
				{
					delete item;
					item = ch->m_invenNormal.GetItem(r, c);
					if (item)
					{
						ItemUpdateMsg(rmsg, item, count);
						SEND_Q(rmsg, ch->m_desc);
					}
				}
			}
			else
			{
				// 추가되었다
				ItemAddMsg(rmsg, item);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		else
		{
			// 땅에 버리기
			ch->m_pArea->DropItem(item, ch);
			item->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, item);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}

		ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_OK);
		SEND_Q(rmsg, ch->m_desc);

		GAMELOG << init("ITEM PROCESS NPC", ch)
				<< "ITEM" << delim
				<< proto->m_index << delim
				<< proto->m_name << delim
				<< "SERIAL" << delim
				<< item->m_serial << delim
				<< "COUNT" << delim
				<< count << delim
				<< "TOTAL" << delim
				<< item->Count()
				<< end;
	}
	else
	{
		ItemProcessNPCMsg(rmsg, 0, 0, MSG_ITEM_PROCESS_NPC_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);

		GAMELOG << init("ITEM PROCESS NPC FAIL", ch)
				<< "ITEM" << delim
				<< proto->m_index << delim
				<< proto->m_name << delim
				<< "COUNT" << delim
				<< count
				<< end;
	}
}

void do_ItemAddBooster(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int boostitemindex;
	CItem* boost = NULL;
	CItem* tool = NULL;

	msg >> boostitemindex;

	// 부스터 아이템 찾기
	boost = ch->m_invenNormal.GetItem(boostitemindex);
	if (boost == NULL)
		return ;
	// 부스터 아이템이 맞나? : a_num0가 IETC_UPGRADE_BOOSTER(3)이어야 부스터
	if (boost->m_itemProto->m_typeIdx != ITYPE_ETC || boost->m_itemProto->m_subtypeIdx != IETC_UPGRADE || boost->m_itemProto->m_num0 != IETC_UPGRADE_BOOSTER)
		return ;

	// 착용중인 아이템 검사하기
	tool = ch->m_wearing[WEARING_WEAPON];
	// 착용중인가?
	if (tool == NULL)
		return ;
	// 무기이고?
	if (tool->m_itemProto->m_typeIdx != ITYPE_WEAPON)
		return ;
	// 생산도구이고?
	if (tool->m_itemProto->m_subtypeIdx != IWEAPON_MINING && tool->m_itemProto->m_subtypeIdx != IWEAPON_GATHERING && tool->m_itemProto->m_subtypeIdx != IWEAPON_CHARGE)
		return ;
	// 부스터 안 붙였나?
	if (tool->m_flag & FLAG_ITEM_BOOSTER_ADDED
		|| tool->m_flag & FLAG_ITEM_SILVERBOOSTER_ADDED 
		|| tool->m_flag & FLAG_ITEM_GOLDBOOSTER_ADDED 
#ifdef PLATINUM
		|| tool->m_flag & FLAG_ITEM_PLATINUMBOOSTER_ADDED
#endif
		)
	{
		ItemAddBoosterMsg(rmsg, MSG_ITEM_ADD_BOOSTER_ERROR_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	// 내구도 검사
	if (tool->m_used < 5)
	{
		ItemAddBoosterMsg(rmsg, MSG_ITEM_ADD_BOOSTER_ERROR_USED);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 플래그 변경 : 할일 부스터 나오면 거기에 따라 코딩
	switch(boost->m_itemProto->m_index)
	{
	case 786:
		tool->m_flag |= FLAG_ITEM_BOOSTER_ADDED;
		break;
	case 1283:
		tool->m_flag |= FLAG_ITEM_SILVERBOOSTER_ADDED;
		break;
	case 1284:
		tool->m_flag |= FLAG_ITEM_GOLDBOOSTER_ADDED;
		break;
#ifdef PLATINUM
	case 1417:
		tool->m_flag |= FLAG_ITEM_PLATINUMBOOSTER_ADDED;
			break;
#endif
	default:
		ItemAddBoosterMsg(rmsg, MSG_ITEM_ADD_BOOSTER_ERROR_USED);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	ItemUpdateMsg(rmsg, tool, 0);
	SEND_Q(rmsg, ch->m_desc);

	// 부스터 감소
	if (boost->Count() < 2)
	{
		ItemDeleteMsg(rmsg, boost);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, boost, true, true);
	}
	else
	{
		DecreaseFromInventory(ch, boost, 1);
		ItemUpdateMsg(rmsg, boost, -1);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 결과 알림
	ItemAddBoosterMsg(rmsg, MSG_ITEM_ADD_BOOSTER_ERROR_OK);
	SEND_Q(rmsg, ch->m_desc);
}

#ifdef ENABLE_WAR_CASTLE
void do_ItemMixWarItem(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	char rowType1, colType1;
	char rowType2, colType2;
	char rowType3, colType3;

	msg >> rowType1 >> colType1
		>> rowType2 >> colType2
		>> rowType3 >> colType3;

	CItem* itemType1 = ch->m_invenNormal.GetItem(rowType1, colType1);
	CItem* itemType2 = ch->m_invenNormal.GetItem(rowType2, colType2);
	CItem* itemType3 = ch->m_invenNormal.GetItem(rowType3, colType3);

	if (!itemType1 || !itemType2 || !itemType3)
	{
		ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 각 아이템 타입 검사
	// 대분류 기타
	if (itemType1->m_itemProto->m_typeIdx != ITYPE_ETC
			|| itemType1->m_itemProto->m_typeIdx != itemType2->m_itemProto->m_typeIdx
			|| itemType1->m_itemProto->m_typeIdx != itemType3->m_itemProto->m_typeIdx)
	{
		ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	// type1, type2, type3
	if (itemType1->m_itemProto->m_subtypeIdx != IETC_MIX_TYPE1
			|| itemType2->m_itemProto->m_subtypeIdx != IETC_MIX_TYPE2
			|| itemType3->m_itemProto->m_subtypeIdx != IETC_MIX_TYPE3)
	{
		ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 생성될 아이템 결정 : type3의 a_num1가 생성될 아이템 인덱스
	int itemindex = itemType3->m_itemProto->m_num1;
	// 생성될 옵션 결정 : 각 재료의 a_num0가 옵션 index
	int option1 = itemType1->m_itemProto->m_num0;
	int option2 = itemType2->m_itemProto->m_num0;
	int option3 = itemType3->m_itemProto->m_num0;
	COptionProto* optionProto1 = gserver.m_optionProtoList.FindProto(option1);
	COptionProto* optionProto2 = gserver.m_optionProtoList.FindProto(option2);
	COptionProto* optionProto3 = gserver.m_optionProtoList.FindProto(option3);

	// 생성될 아이템이 조합 아이템인지 검사
	CItemProto* itemproto = gserver.m_itemProtoList.FindIndex(itemindex);
	if (!itemproto || itemproto->m_typeIdx != ITYPE_ACCESSORY || itemproto->m_subtypeIdx == IACCESSORY_PET || !(itemproto->m_flag & ITEM_FLAG_MIX))
	{
		ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	// 생성될 옵션이 합당한지 검사
	if (!optionProto1 || !optionProto2 || !optionProto3)
	{
		ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 아이템 생성
	CItem* mixItem = gserver.m_itemProtoList.CreateItem(itemindex, -1, 0, 0, 1);
	if (!mixItem)
	{
		ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// Type1
	mixItem->m_option[0].m_type = optionProto1->m_type;
	mixItem->m_option[0].m_level = 1;
	mixItem->m_option[0].m_proto = optionProto1;
	mixItem->m_option[0].m_value = optionProto1->m_levelValue[0];

	// Type2
	mixItem->m_option[1].m_type = optionProto2->m_type;
	mixItem->m_option[1].m_level = 1;
	mixItem->m_option[1].m_proto = optionProto2;
	mixItem->m_option[1].m_value = optionProto2->m_levelValue[0];

	// Type3
	mixItem->m_option[2].m_type = optionProto3->m_type;
	mixItem->m_option[2].m_level = 1;
	mixItem->m_option[2].m_proto = optionProto3;
	mixItem->m_option[2].m_value = optionProto3->m_levelValue[0];

	mixItem->m_nOption = 3;
	mixItem->m_option[0].SetDBValue();
	mixItem->m_option[1].SetDBValue();
	mixItem->m_option[2].SetDBValue();

	// GAMELOG 남기기
	GAMELOG << init("ITEM MIX WAR", ch)
			<< "TYPE1" << delim
			<< itemType1->m_itemProto->m_name << delim
			<< "TYPE2" << delim
			<< itemType2->m_itemProto->m_name << delim
			<< "TYPE3" << delim
			<< itemType3->m_itemProto->m_name << delim
			<< "MIX ITEM" << delim
			<< itemlog(mixItem);

	// 아이템 추가 및 제거
	if (itemType1->Count() < 2)
	{
		ItemDeleteMsg(rmsg, itemType1);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, itemType1, true, true);
	}
	else
	{
		DecreaseFromInventory(ch, itemType1, 1);
		ItemUpdateMsg(rmsg, itemType1, -1);
		SEND_Q(rmsg, ch->m_desc);
	}
	if (itemType2->Count() < 2)
	{
		ItemDeleteMsg(rmsg, itemType2);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, itemType2, true, true);
	}
	else
	{
		DecreaseFromInventory(ch, itemType2, 1);
		ItemUpdateMsg(rmsg, itemType2, -1);
		SEND_Q(rmsg, ch->m_desc);
	}
	if (itemType3->Count() < 2)
	{
		ItemDeleteMsg(rmsg, itemType3);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, itemType3, true, true);
	}
	else
	{
		DecreaseFromInventory(ch, itemType3, 1);
		ItemUpdateMsg(rmsg, itemType3, -1);
		SEND_Q(rmsg, ch->m_desc);
	}

	// Inven에 넣기
	if (AddToInventory(ch, mixItem, true, true))
	{
		// 장비이므로 겹치지 않는다는 가정하에
		GAMELOG << delim << "INVEN" << end;

		ItemAddMsg(rmsg, mixItem);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		// 못 넣었으면 바닥으로
		GAMELOG << delim << "DROP" << end;

		ch->m_pArea->DropItem(mixItem, ch);
		mixItem->m_preferenceIndex = ch->m_index;
		ItemDropMsg(rmsg, ch, mixItem);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	ItemMixWarItemMsg(rmsg, MSG_ITEM_MIX_WARITEM_ERROR_OK);
	SEND_Q(rmsg, ch->m_desc);

	ch->m_bCreateMixItem = true;
}
#endif

// 060227 : bs : 파티리콜
void do_ItemUsePartyRecall(CPC* ch, CItem* pItem)
{
	CNetMsg rmsg;

#ifdef PARTY_RECALL_NEW
	bool bUsed = false;
	if (pItem->m_used == -1)
	{	// 처음 사용
		pItem->m_used = gserver.m_gameTime + pItem->m_itemProto->m_num3 * 60 * 60;
		bUsed = true;
	}
#endif	// PARTY_RECALL_NEW

	// 리콜 사용 가능 검사
	if (pItem->m_used < gserver.m_gameTime)
		return ;
	
	// 파티 중인지 검사
	if (!ch->IsParty())
		return ;

	if (ch->m_pZone == NULL)
		return ;
	
	if( ch->m_pZone->IsSummonZone() )
		return;

	// 파티원에게 알림 메시지 보내기
	if (IS_RUNNING_HELPER)
	{
		int nGuildIndex = 0;
		char cIsInCastle = 0;

#ifdef ENABLE_WAR_CASTLE
		CWarCastle* pCastle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
		if (pCastle && pCastle->GetState() != WCSF_NORMAL && pCastle->CheckSubServer())
		{
			// 공성 진행중인 서브 서버이면
			// 성안 영역인가 확인하고
			if (pCastle->IsInInnerCastle(ch))
			{
#if defined (LC_JPN) || defined (LC_USA) 
				return;	//  공성 진행중인 성안에 파티 리콜 금지
#endif // #if defined (LC_JPN) || defined (LC_USA)
				
#ifdef DRATAN_CASTLE
				if(ch->m_pZone->m_index == ZONE_DRATAN)
				{	// 드라탄 공성은 파티 리콜 사용 금지
					return;
				}
#endif // DRATAN_CASTLE

				// 경고 메시지 보내고
				ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_WARN_PARTYRECALL);
				SEND_Q(rmsg, ch->m_desc);

				//공성시간에 PARTY_RECALL 이동 로그
					GAMELOG << init(" PARTY_RECALL IN WARTIME " , ch ) << end;
				return;  

				// 길드 정보를 구해서
				if (ch->m_guildInfo && ch->m_guildInfo->guild())
					nGuildIndex = ch->m_guildInfo->guild()->index();
				cIsInCastle = 1;
			}
		}
#endif // ENABLE_WAR_CASTLE

		HelperPartyRecallPromptMsg(rmsg, ch->m_party->GetBossIndex(), ch->m_index, ch->GetName(), cIsInCastle, nGuildIndex);
		SEND_Q(rmsg, gserver.m_helper);
	}

	// 결과 보내기
	ExPartyRecallNoticeMsg(rmsg);
	SEND_Q(rmsg, ch->m_desc);
	
#ifdef PARTY_RECALL_NEW
	if (bUsed)
	{	// 처음 사용시 만료 시간을 보낸다
		ItemUpdateMsg(rmsg, pItem, 0);
		SEND_Q(rmsg, ch->m_desc);
	}	
#endif	// PARTY_RECALL_NEW
}

#ifdef RECOMMEND_SERVER_SYSTEM
void do_ItemTake_RecommendPotion(CPC* ch, CItem* pItem)
{
	bool bCanTake = false;
	// 30레벨 이하
	if (ch->m_level <= RECOMMEND_SERVER_LEVEL)
	{
		// 우선권 검사
		if (pItem->m_preferenceIndex != -1)
		{
			// 우선권이 있을 때
			if (pItem->m_preferenceIndex == ch->m_index)
			{
				bCanTake = true;
			}
			else if (ch->IsParty())
			{
				// 파티원 우선권 검사
				if (ch->m_party->FindMember(pItem->m_preferenceIndex) != -1)
				{
					bCanTake = true;
				}
			}
		}
		else
		{
			// 우선권 없으면 집을 수 있음
			bCanTake = true;
		}
	}

	if (!bCanTake && ch->m_admin < 2)
	{
		// 우선권이 없습니다 메시지 전송
		CNetMsg smsg;
		SysMsg(smsg, MSG_SYS_NOT_OWNER_ITEM);
		SEND_Q(smsg, ch->m_desc);
		return;
	}
	else
	{
		// 효과 랜덤 결정
		const int nCountSkill = 6;
		int nSkillIndex[nCountSkill] = {
			355,
			356,
			357,
			358,
			359,
			360
		};
		int nIndex = GetRandom(0, nCountSkill - 1);

		CSkill* skill = gserver.m_skillProtoList.Create(nSkillIndex[nIndex], 1);
		bool bSkipApply = false;
		// 060227 : bs : 중복안되는 스킬인지 검사
		if ((skill->m_proto->m_flag & SF_NOTDUPLICATE) && ch->m_assist.FindBySkillIndex(skill->m_proto->m_index))
			bSkipApply = true;
		// 확장 포션 : 듀얼 사용중일때 따로 따로 수행 못함 
		if (( skill->m_proto->m_index == 350 || skill->m_proto->m_index == 351 ) && ch->m_assist.FindBySkillIndex(352) )
			bSkipApply = true;

		if (!bSkipApply)
		{
			bool bApply;
			ApplySkill(ch, ch, skill, -1, bApply);
		}

		delete skill;

		CNetMsg disappmsg;
		CNetMsg takemsg;

		int cx = pItem->m_cellX;
		int cz = pItem->m_cellZ;
		char yLayer = GET_YLAYER(pItem);
		CArea* pArea = pItem->m_pArea;

		// 포션 사용 이펙트 메시지
		CNetMsg effectMsg;
		EffectItemMsg(effectMsg, ch, pItem);
		pArea->SendToCell(effectMsg, ch, true);
		
		ItemTakeMsg(takemsg, ch, pItem);
		ItemDisappearMsg(disappmsg, pItem);
		
		// 우선권 제거
		pItem->m_preferenceIndex = -1;
		
		// 땅에서 제거
		pArea->ItemFromCell(pItem);
		
		// 메시지 보내고
		pArea->SendToCell(takemsg, ch, true);
		pArea->SendToCell(disappmsg, yLayer, cx, cz);

		GAMELOG << init("RECOMMEND SERVER ITEM PICK", ch)
				<< "SKILL" << delim
				<< nSkillIndex[nIndex]
				<< end;

		delete pItem;
	}
}
#endif // RECOMMEND_SERVER_SYSTEM

#ifdef ITEM_COMPOSITE
// 미감정 결합 아이템 감정
bool do_useCashItem_1420(CPC* ch, CItem* item, CNetMsg& msg, bool bprolong, int extra1)
{
	CNetMsg rmsg;

	int nProb = GetRandom(1, 100);
	const int nNewItemIndex = 1421;
	int nNewItemPlus = 0;
	if (nProb <= 80)					nNewItemPlus = 1;
	else if (nProb <= 80 + 15)			nNewItemPlus = 2;
	else if (nProb <= 80 + 15 + 5)		nNewItemPlus = 3;
	if (nNewItemPlus == 0)
		return false;

	CItem* pNewItem = gserver.m_itemProtoList.CreateItem(nNewItemIndex, -1, nNewItemPlus, 0, 1);
	if (!pNewItem)
	{
		GAMELOG << init("USE CASHITEM 1420 ERROR CANT CREATE ITEM 1421", ch) << end;
		return false;
	}

	// 인벤토리에 최소 1개의 빈공간이 있어야 한다
	if (ch->m_invenNormal.GetSpace() < 1)
	{
		delete pNewItem;
		pNewItem = NULL;
		SysFullInventoryMsg(rmsg, INVENTORY_NORMAL);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 인벤토리에 최소 무게가 보장되어야 한다.
	if (ch->m_weight + pNewItem->m_itemProto->m_weight >= (ch->m_maxWeight * 3 / 2))
	{
		delete pNewItem;
		pNewItem = NULL;
		SysFullInventoryMsg(rmsg, INVENTORY_NORMAL);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	if (AddToInventory(ch, pNewItem, true, true))
	{
		GAMELOG << init("ITEM IDENTIFY", ch)
				<< itemlog(item, false) << delim
				<< itemlog(pNewItem, false)
				<< end;

		if (pNewItem->tab() < 0)
		{
			int r, c;
			delete pNewItem;
			ch->m_invenNormal.FindItem(&r, &c, nNewItemIndex, nNewItemPlus, 0);
			pNewItem = ch->m_invenNormal.GetItem(r, c);
			ItemUpdateMsg(rmsg, pNewItem, 1);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemAddMsg(rmsg, pNewItem);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else
	{
		delete pNewItem;
		pNewItem = NULL;
		GAMELOG << init("USE CASHITEM 1420 ERROR FULL INVENTORY", ch) << end;

		SysFullInventoryMsg(rmsg, INVENTORY_NORMAL);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
	return true;
}

// << kjban 080129
#ifdef LIMIT_ITEM_M35
// 미감정 초보자용 결합 아이템 감정
bool do_useCashItem_2664(CPC* ch, CItem* item, CNetMsg& msg, bool bprolong, int extra1)
{
	CNetMsg rmsg;

	int nProb = GetRandom(1, 100);
	const int nNewItemIndex = 2665;
	int nNewItemPlus = 0;
	if (nProb <= 80)					nNewItemPlus = 1;
	else if (nProb <= 80 + 15)			nNewItemPlus = 2;
	else if (nProb <= 80 + 15 + 5)		nNewItemPlus = 3;
	if (nNewItemPlus == 0)
		return false;

	CItem* pNewItem = gserver.m_itemProtoList.CreateItem(nNewItemIndex, -1, nNewItemPlus, 0, 1);
	if (!pNewItem)
	{
		GAMELOG << init("USE CASHITEM 2664 ERROR CANT CREATE ITEM 2665", ch) << end;
		return false;
	}

	// 인벤토리에 최소 1개의 빈공간이 있어야 한다
	if (ch->m_invenNormal.GetSpace() < 1)
	{
		delete pNewItem;
		pNewItem = NULL;
		SysFullInventoryMsg(rmsg, INVENTORY_NORMAL);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 인벤토리에 최소 무게가 보장되어야 한다.
	if (ch->m_weight + pNewItem->m_itemProto->m_weight >= (ch->m_maxWeight * 3 / 2))
	{
		delete pNewItem;
		pNewItem = NULL;
		SysFullInventoryMsg(rmsg, INVENTORY_NORMAL);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	if (AddToInventory(ch, pNewItem, true, true))
	{
		GAMELOG << init("ITEM IDENTIFY", ch)
				<< itemlog(item, false) << delim
				<< itemlog(pNewItem, false)
				<< end;

		if (pNewItem->tab() < 0)
		{
			int r, c;
			delete pNewItem;
			ch->m_invenNormal.FindItem(&r, &c, nNewItemIndex, nNewItemPlus, 0);
			pNewItem = ch->m_invenNormal.GetItem(r, c);
			ItemUpdateMsg(rmsg, pNewItem, 1);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemAddMsg(rmsg, pNewItem);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else
	{
		delete pNewItem;
		pNewItem = NULL;
		GAMELOG << init("USE CASHITEM 2664 ERROR FULL INVENTORY", ch) << end;

		SysFullInventoryMsg(rmsg, INVENTORY_NORMAL);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
	return true;
}

// 감정된 결합 주문서 사용
bool do_useCashItem_2665(CPC* ch, CItem* item, CNetMsg& msg, bool bprolong, int nIndexClothes)
{
	int nIndexEquip = 0;
	msg >> nIndexEquip;

	CItem* pItemClothes = ch->m_invenNormal.GetItem(nIndexClothes);
	CItem* pItemEquip   = ch->m_invenNormal.GetItem(nIndexEquip);

	if (pItemClothes == NULL || pItemEquip == NULL || pItemClothes == pItemEquip || pItemClothes == item || pItemEquip == item)
		return false;
	
	// pItemClothes 검사
	// 1. ITEM_FLAG_COMPOSITE		ON
	// 2. FLAG_ITEM_COMPOSITION		OFF
	// 3. USED						< NOW
	// 4. PLUS						NOT SET
	// 5. COMPOSITEITEM				NOT SET
	// 6. WEARPOS					OFF
	if (!(pItemClothes->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE))
		return false;
	if (pItemClothes->m_flag & FLAG_ITEM_COMPOSITION)
		return false;
	if (pItemClothes->m_used < gserver.m_gameTime)
	{
		// pItemClothes->m_used == -1 일 경우 기간 무제한 yhj
		// 피닉스 이벤트 아이템이 아닐경우
		if( pItemClothes->m_used == -1 )  
		{
			/* 무제한 이므로 넘어감*/
		}
		else
		{
			return false;
		}
	}
	if (pItemClothes->m_plus != 0)
		return false;
	if (pItemClothes->m_nCompositeItem >= 0)
		return false;
	if (pItemClothes->m_wearPos != WEARING_NONE)
		return false;

	// pItemEquip 검사
	// 1. FLAG_ITEM_COMPOSITION		OFF
	// 2. TYPE						== pItemClothes.TYPE
	// 3. SUBTYPE					== pItemClothes.SUBTYPE
	// 4. WEARPOS					OFF
	// 5. UPGRADE FLAG				ON
	// 6. ITEM_FLAG_COMPOSITE		OFF
	// 7. JOB						== pItemClothes.JOB
	// 8. PLATINUM					OFF
	if (pItemEquip->m_flag & FLAG_ITEM_COMPOSITION)
		return false;
	if (pItemEquip->m_itemProto->m_typeIdx != pItemClothes->m_itemProto->m_typeIdx)
		return false;
	if (pItemEquip->m_itemProto->m_subtypeIdx != pItemClothes->m_itemProto->m_subtypeIdx)
		return false;
	if (pItemEquip->m_wearPos != WEARING_NONE)
		return false;
	if (!pItemEquip->CanCompositeEquipItem())
		return false;
	if (pItemEquip->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE)
		return false;
	if (pItemEquip->m_itemProto->m_jobFlag != pItemClothes->m_itemProto->m_jobFlag)
		return false;
#ifdef PLATINUM
	if (pItemEquip->IsUsedPlatinumSpecial())
		return false;
#endif // PLATINUM

	// 처리
	// 1. FLAG						FLAG_ITEM_COMPOSITION
	// 2. COMPOSITEITEM				pItemEquip.INDEX
	// 3. pItemClothes.PLUS			item.PLUS + pItemEquip.PLUS
	// 4. PLATINUM SET				FLAG_ITEM_PLATINUM_SET

	pItemClothes->m_flag |= FLAG_ITEM_COMPOSITION;
	pItemEquip->m_flag |= FLAG_ITEM_COMPOSITION;
	pItemClothes->m_nCompositeItem = pItemEquip->m_index;
	pItemClothes->m_plus = item->m_plus + pItemEquip->m_plus;
	FLAG_ITEM_PLATINUM_SET(pItemClothes->m_flag, pItemEquip->m_plus);

#ifdef COMPOSITE_TIME
	// 결합 주문서 만료 시간 저장
	if (pItemClothes->m_itemProto->m_flag & ITEM_FLAG_ABS)
	{	// 시간제 아이템
		pItemClothes->m_used_2 = pItemClothes->m_used;
	}
	else
	{	// 기간제 아이템
		static const int one_month = 60*60*24*30;		

#if defined (LC_JPN)
		// 일본은 만료 시간이 없는 아이템도 한달이다.
		if ( pItemClothes->m_used < 0 )
		{
			pItemClothes->m_used_2 = gserver.m_gameTime + one_month;
		}
		else
#endif // (LC_JPN)

		if (pItemClothes->m_used - gserver.m_gameTime < one_month)
		{	// 1개월 미만인 경우 결합시간은 유료 아이템 만료 시간과 같다
			pItemClothes->m_used_2 = pItemClothes->m_used;
		}		
		else
		{	// 1개월 이상인 경우 결합시간은 1개월이다.
			pItemClothes->m_used_2 = gserver.m_gameTime + one_month;
		}
	}
#endif // COMPOSITE_TIME

	// 로그 남김
	GAMELOG << init("ITEM COMPOSITION", ch)
			<< "COMPOSITION DOC" << delim
			<< itemlog(item, false) << delim
			<< "CLOTHES" << delim
			<< itemlog(pItemClothes) << delim
			<< "EQUIP" << delim
			<< itemlog(pItemEquip)
			<< end;

	// 결과 전달
	// ITEM UPDATE			pItemClothes, pItemEquip
	CNetMsg rmsg;
	ItemUpdateMsg(rmsg, pItemClothes, 0);
	SEND_Q(rmsg, ch->m_desc);
	ItemUpdateMsg(rmsg, pItemEquip, 0);
	SEND_Q(rmsg, ch->m_desc);
	SysMsg(rmsg, MSG_SYS_ITEMCOMPOSITED);
	SEND_Q(rmsg, ch->m_desc);

	ch->CalcStatus(true);

	ch->SaveNow(false);

	return true;
}
#endif // LIMIT_ITEM_M35
// >>

// 감정된 결합 주문서 사용
bool do_useCashItem_1421(CPC* ch, CItem* item, CNetMsg& msg, bool bprolong, int nIndexClothes)
{
	int nIndexEquip = 0;
	msg >> nIndexEquip;

	CItem* pItemClothes = ch->m_invenNormal.GetItem(nIndexClothes);
	CItem* pItemEquip   = ch->m_invenNormal.GetItem(nIndexEquip);

	if (pItemClothes == NULL || pItemEquip == NULL || pItemClothes == pItemEquip || pItemClothes == item || pItemEquip == item)
		return false;

	// pItemClothes 검사
	// 1. ITEM_FLAG_COMPOSITE		ON
	// 2. FLAG_ITEM_COMPOSITION		OFF
	// 3. USED						< NOW
	// 4. PLUS						NOT SET
	// 5. COMPOSITEITEM				NOT SET
	// 6. WEARPOS					OFF
	if (!(pItemClothes->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE))
		return false;
	if (pItemClothes->m_flag & FLAG_ITEM_COMPOSITION)
		return false;
	if (pItemClothes->m_used < gserver.m_gameTime)
	{
		// pItemClothes->m_used == -1 일 경우 기간 무제한 yhj
		// 피닉스 이벤트 아이템이 아닐경우
		if( pItemClothes->m_used == -1 )  
		{
			/* 무제한 이므로 넘어감*/
		}
		else
		{
			return false;
		}
	}
	if (pItemClothes->m_plus != 0)
		return false;
	if (pItemClothes->m_nCompositeItem >= 0)
		return false;
	if (pItemClothes->m_wearPos != WEARING_NONE)
		return false;

	// pItemEquip 검사
	// 1. FLAG_ITEM_COMPOSITION		OFF
	// 2. TYPE						== pItemClothes.TYPE
	// 3. SUBTYPE					== pItemClothes.SUBTYPE
	// 4. WEARPOS					OFF
	// 5. UPGRADE FLAG				ON
	// 6. ITEM_FLAG_COMPOSITE		OFF
	// 7. JOB						== pItemClothes.JOB
	// 8. PLATINUM					OFF
	if (pItemEquip->m_flag & FLAG_ITEM_COMPOSITION)
		return false;
	if (pItemEquip->m_itemProto->m_typeIdx != pItemClothes->m_itemProto->m_typeIdx)
		return false;
	if (pItemEquip->m_itemProto->m_subtypeIdx != pItemClothes->m_itemProto->m_subtypeIdx)
		return false;
	if (pItemEquip->m_wearPos != WEARING_NONE)
		return false;
	if (!pItemEquip->CanCompositeEquipItem())
		return false;
	if (pItemEquip->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE)
		return false;
	if (pItemEquip->m_itemProto->m_jobFlag != pItemClothes->m_itemProto->m_jobFlag)
		return false;
#ifdef PLATINUM
	if (pItemEquip->IsUsedPlatinumSpecial())
		return false;
#endif // PLATINUM

	// 처리
	// 1. FLAG						FLAG_ITEM_COMPOSITION
	// 2. COMPOSITEITEM				pItemEquip.INDEX
	// 3. pItemClothes.PLUS			item.PLUS + pItemEquip.PLUS
	// 4. PLATINUM SET				FLAG_ITEM_PLATINUM_SET

	pItemClothes->m_flag |= FLAG_ITEM_COMPOSITION;
	pItemEquip->m_flag |= FLAG_ITEM_COMPOSITION;
	pItemClothes->m_nCompositeItem = pItemEquip->m_index;
	pItemClothes->m_plus = item->m_plus + pItemEquip->m_plus;
	FLAG_ITEM_PLATINUM_SET(pItemClothes->m_flag, pItemEquip->m_plus);

#ifdef COMPOSITE_TIME
	// 결합 주문서 만료 시간 저장
	if (pItemClothes->m_itemProto->m_flag & ITEM_FLAG_ABS)
	{	// 시간제 아이템
		pItemClothes->m_used_2 = pItemClothes->m_used;
	}
	else
	{	// 기간제 아이템
		static const int one_month = 60*60*24*30;		

#if defined (LC_JPN)
		// 일본은 만료 시간이 없는 아이템도 한달이다.
		if ( pItemClothes->m_used < 0 )
		{
			pItemClothes->m_used_2 = gserver.m_gameTime + one_month;
		}
		else
#endif // (LC_JPN)

		if (pItemClothes->m_used - gserver.m_gameTime < one_month)
		{	// 1개월 미만인 경우 결합시간은 유료 아이템 만료 시간과 같다
			pItemClothes->m_used_2 = pItemClothes->m_used;
		}		
		else
		{	// 1개월 이상인 경우 결합시간은 1개월이다.
			pItemClothes->m_used_2 = gserver.m_gameTime + one_month;
		}
	}
#endif // COMPOSITE_TIME

	// 로그 남김
	GAMELOG << init("ITEM COMPOSITION", ch)
			<< "COMPOSITION DOC" << delim
			<< itemlog(item, false) << delim
			<< "CLOTHES" << delim
			<< itemlog(pItemClothes) << delim
			<< "EQUIP" << delim
			<< itemlog(pItemEquip)
			<< end;

	// 결과 전달
	// ITEM UPDATE			pItemClothes, pItemEquip
	CNetMsg rmsg;
	ItemUpdateMsg(rmsg, pItemClothes, 0);
	SEND_Q(rmsg, ch->m_desc);
	ItemUpdateMsg(rmsg, pItemEquip, 0);
	SEND_Q(rmsg, ch->m_desc);
	SysMsg(rmsg, MSG_SYS_ITEMCOMPOSITED);
	SEND_Q(rmsg, ch->m_desc);

	ch->CalcStatus(true);

	ch->SaveNow(false);

	return true;
}

bool do_useCashItem_1540(CPC* ch, CItem* item, CNetMsg& msg, int nIndexClothes)
{
	CNetMsg		rmsg;
	CItem*		pItemClothes = NULL;
	CItem*		pItemEquip = NULL;

	// 1. nIndexClothes로 아이템 찾기
	// 2. 아이템 합성 여부 검사
	// 3. 합성된 아이템 찾기
	// 4. 분리 처리
	// 5. 게임 로그
	// 6. 결과 알림

	// 1. nIndexClothes로 아이템 찾기
	pItemClothes = ch->m_invenNormal.GetItem(nIndexClothes);
	if (pItemClothes == NULL)
	{
		SysMsg(rmsg, MSG_SYS_CANNOT_SEPARATE_ITEM);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

// EVENT_PHOENIX 관련 수정... m_used == -1 이면 무한인데 여기서 분리가 안되게 되어있음 
// 고로 m_used == -1 이면 건너 뛰게 수정
	// 2. 아이템 합성 여부 검사
	if (	   (pItemClothes->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE)
			&& ( pItemClothes->m_used == -1 || pItemClothes->m_used > gserver.m_gameTime )
			&& pItemClothes->m_flag & FLAG_ITEM_COMPOSITION
		)
	{
		// 3. 합성된 아이템 찾기
		pItemEquip = ch->m_invenNormal.GetItem(pItemClothes->m_nCompositeItem);
		if (!pItemEquip)
		{
			SysMsg(rmsg, MSG_SYS_CANNOT_SEPARATE_ITEM);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}

		// 4. 분리 처리
		pItemClothes->m_flag &= ~FLAG_ITEM_COMPOSITION;
		pItemEquip->m_flag &= ~FLAG_ITEM_COMPOSITION;
		pItemClothes->m_nCompositeItem = -1;
		pItemClothes->m_plus = 0;
		FLAG_ITEM_PLATINUM_SET(pItemClothes->m_flag, 0);

#ifdef COMPOSITE_TIME
		pItemClothes->m_used_2 = -1;		// 결합 주문서 만료 시간 제거
#endif // COMPOSITE_TIME

		// 5. 게임 로그
		GAMELOG << init("ITEM COMPOSITE SEPARATE", ch)
				<< "CLOTHES" << delim
				<< itemlog(pItemClothes) << delim
				<< "EQUIP" << delim
				<< itemlog(pItemEquip)
				<< end;

		// 6. 결과 알림
		SysSeparateItemMsg(rmsg, pItemClothes->m_idNum);
		SEND_Q(rmsg, ch->m_desc);
		ItemUpdateMsg(rmsg, pItemClothes, 0);
		SEND_Q(rmsg, ch->m_desc);
		ItemUpdateMsg(rmsg, pItemEquip, 0);
		SEND_Q(rmsg, ch->m_desc);

		ch->CalcStatus(true);

		return true;
	}

	return false;
}

#endif // ITEM_COMPOSITE

void do_ItemLendWeapon(CPC* ch, CNetMsg& msg)
{
	const static int	nMinPrice			= 1;				// 대여 가능 아이템 최소 금액
	const static int	nMoneyFactorMul		= 3;				// 대여 금액 산출시 아이템 가격에 곱해지는 수
	const static int	nMoneyFactorDiv		= 2;				// 대여 금액 산출시 아이템 가격에 나누는 수
	const static int	nMoneyFactorAdd		= 50000;			// 대여 금액 산출시 기본으로 더해지는 수
	const static int	nPlusLendItem		= 6;				// 대여 아이템 기본 Plus 수치
	const static int	nFlagLendItem		= FLAG_ITEM_LENT;	// 대여 아이템 기본 Flag

	CNetMsg			rmsg;
	int				nItemDBIndex	= 0;		// 생성하고자 하는 아이템 DB 인덱스
	void*			posLendItem		= NULL;		// 대여 가능 리스트 검색용
	CItemProto*		pItemProto		= NULL;		// 생성할 아이템 프로토
	LONGLONG		nNeedMoney		= 0;		// 필요 금액
	CItem*			pItem			= NULL;		// 지급할 아이템

	msg >> nItemDBIndex;

	// 1. 대여가능 리스트에서 찾는다
	// 2. 소지금을 계산한다
	// 3. 소지금 검사
	// 4. 아이템 생성
	// 5. 아이템 지급
	// 6. 소지금 감소
	// 7. 게임 로그
	// 8. 결과 알림

	// 1. 대여가능 리스트에서 찾는다
	if (   (posLendItem = gserver.m_itemProtoList.m_listLendItem.FindData(nItemDBIndex)) == NULL
		|| (pItemProto = gserver.m_itemProtoList.FindIndex(nItemDBIndex)) == NULL
		|| pItemProto->m_price < nMinPrice)
	{
		ItemLendWeaponMsg(rmsg, nItemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 2. 소지금을 계산한다
	nNeedMoney = pItemProto->m_price * nMoneyFactorMul / nMoneyFactorDiv + nMoneyFactorAdd;
#ifdef EVENT_SUMMER_2008
	nNeedMoney = 100;		// 이벤트 기간 대여 일괄 100나스
#endif // EVENT_SUMMER_2008

	// 3. 소지금 검사
	if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < nNeedMoney)
	{
		ItemLendWeaponMsg(rmsg, nItemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_NOMONEY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 4. 아이템 생성
	pItem = gserver.m_itemProtoList.CreateItem(nItemDBIndex, -1, nPlusLendItem, nFlagLendItem, 1);
	if (!pItem)
	{
		ItemLendWeaponMsg(rmsg, nItemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 5. 아이템 지급
	if (AddToInventory(ch, pItem, true, true))
	{
		ItemAddMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);

		// 6. 소지금 감소
		if (ch->m_moneyItem->Count() > nNeedMoney)
		{
			DecreaseFromInventory(ch, ch->m_moneyItem, nNeedMoney);
			ItemUpdateMsg(rmsg, ch->m_moneyItem, -nNeedMoney);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, ch->m_moneyItem);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, ch->m_moneyItem, true, true);
			ch->m_moneyItem = NULL;
		}

		// 7. 게임 로그
		GAMELOG << init("LEND ITEM", ch)
				<< "MONEY" << delim
				<< nNeedMoney << delim
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< end;

		// 8. 결과 알림
		ItemLendWeaponMsg(rmsg, nItemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_OK);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		delete pItem;
		pItem = NULL;

		ItemLendWeaponMsg(rmsg, nItemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_FULLINVEN);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

#ifdef ENABLE_PET
bool do_useCashItem_1519(CPC* ch, CItem* item, CNetMsg& msg, int nPetIndex)
{
	// 1. nPetIndex 아이템 찾고
	// 2. 펫 착용 해제 상태인지 보고
	// 3. 해당 아이템의 펫을 찾고
	// 4. 펫의 타입과 스킬 검사하고
	// 5. 펫 마운트 취소후
	// 6. 펫 상태 알리고
	// 7. 결과 알림

	CNetMsg rmsg;
	bool bFail = false;

	// 1. extra1으로 아이템 찾고
	CItem* pItemPet = ch->m_invenNormal.GetItem(nPetIndex);
	if (!pItemPet)
		bFail = true;

	// 2. 펫 착용 해제 상태인지 보고
	else if (pItemPet->m_wearPos != WEARING_NONE)
		bFail = true;
	else
	{
		// 3. 해당 아이템의 펫을 찾고
		CPet* pPet = ch->GetPet(pItemPet->m_plus);
		if (!pPet)
			bFail = true;

		// 4. 펫의 타입과 스킬 검사하고
		else if (!pPet->IsMountType() || pPet->GetSkillList()->count() > 1)
			bFail = true;

		else
		{
			// TODO : petlog
			GAMELOG << init("PET RESET MOUNT", ch)
					<< "PET ITEM" << delim
					<< itemlog(pItemPet) << delim
					<< "PET" << delim
					<< pPet->GetPetTypeGrade() << delim
					<< "INDEX" << delim
					<< pPet->m_index << delim
					<< "LEVEL" << delim
					<< pPet->m_level
					<< end;

			// 5. 펫 마운트 취소후
			pPet->ResetMountType();

			// 6. 펫 상태 알리고
			ExPetStatusMsg(rmsg, pPet);
			SEND_Q(rmsg, ch->m_desc);
		}
	}

	// 7. 결과 알림
	if (bFail)
	{
		SysMsg(rmsg, MSG_SYS_CANNOT_RESET_MOUNT);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
	else
	{
		SysMsg(rmsg, MSG_SYS_RESET_MOUNT);
		SEND_Q(rmsg, ch->m_desc);
		return true;
	}
}
#endif // ENABLE_PET

#ifdef EVENT_COLLECT_BUG
void TO_BUGDATA(CItem* pItem, unsigned char ucBug[16]);
void FROM_BUGDATA(unsigned char ucBug[16], CItem* pItem);

bool do_ItemTake_EventCollectBug(CPC* ch, CItem* pItem)
{
	// 곤충 테이블 : 인덱스, 포인트(번호)
	static const int tableBug[10][2] = {
		{1579,  1},		// 벌
		{1580,  2},		// 흰나비
		{1581,  3},		// 호랑나비
		{1582,  4},		// 잠자리
		{1583,  5},		// 고추잠자리
		{1584,  6},		// 매미
		{1585,  7},		// 무당벌레
		{1586,  8},		// 개똥벌레
		{1587,  9},		// 사슴벌레
		{1588, 10},		// 풍뎅이
	};
	static const int	nNormalBoxIndex = 1577;		// 일반 상자 인덱스
	static const int	nGoldenBoxIndex = 1578;		// 골든 상자 인덱스

	CNetMsg			rmsg;
	bool			bCanTake = false;		// 집을 수 있는지 검사
	CItem*			pItemCollectBox = NULL;	// 채집 상자 아이템
	unsigned char	ucBug[16];				// 곤충 데이터
	int				nSpace = 0;				// 채집 상자 빈칸
	int				nRow, nCol;
	int				i;

	memset(ucBug, 0, sizeof(ucBug));

	// 1. 우선권 검사
	// 2. 빈칸있는 채집 상자 찾기
	// 3. 채집 상자에 넣기 : flag에 포인트 증가, plus, used 설정
	// 4. 바닥에서 제거
	// 5. 게임 로그

	// 1. 우선권 검사
	if (pItem->m_preferenceIndex != -1)
	{
		// 우선권이 있을 때
		if (pItem->m_preferenceIndex == ch->m_index)
		{
			bCanTake = true;
		}
		else if (ch->IsParty())
		{
			// 파티원 우선권 검사
			if (ch->m_party->FindMember(pItem->m_preferenceIndex) != -1)
			{
				bCanTake = true;
			}
		}
	}
	else
	{
		// 우선권 없으면 집을 수 있음
		bCanTake = true;
	}
	if (!bCanTake && ch->m_admin < 2)
	{
		// 우선권이 없습니다 메시지 전송
		SysMsg(rmsg, MSG_SYS_NOT_OWNER_ITEM);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 2. 빈칸있는 채집 상자 찾기
	if (ch->m_invenNormal.FindItem(&nRow, &nCol, nGoldenBoxIndex, -1, -1))
	{
		pItemCollectBox = ch->m_invenNormal.GetItem(nRow, nCol);
		TO_BUGDATA(pItemCollectBox, ucBug);
		for (nSpace = 0; nSpace < 16; nSpace++)
		{
			if (ucBug[nSpace] == 0)
				break;
		}
		if (nSpace == 16)
			pItemCollectBox = NULL;
	}
	if (!pItemCollectBox && ch->m_invenNormal.FindItem(&nRow, &nCol, nNormalBoxIndex, -1, -1))
	{
		pItemCollectBox = ch->m_invenNormal.GetItem(nRow, nCol);
		TO_BUGDATA(pItemCollectBox, ucBug);
		for (nSpace = 0; nSpace < 16; nSpace++)
		{
			if (ucBug[nSpace] == 0)
				break;
		}
		if (nSpace == 16)
			pItemCollectBox = NULL;
	}
	if (!pItemCollectBox)
	{
		// 채집 상자 없음
		EventCollectBugMsg(rmsg, (nSpace == 16) ? MSG_EVENT_COLLECT_BUG_PICK_FULL : MSG_EVENT_COLLECT_BUG_PICK_NOITEM, 0);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 3. 채집 상자에 넣기
	for (i = 0; i < 10; i++)
	{
		if (tableBug[i][0] == pItem->m_idNum)
		{
			ucBug[nSpace] = tableBug[i][1];
			pItemCollectBox->m_flag += tableBug[i][1];
			FROM_BUGDATA(ucBug, pItemCollectBox);
			ItemUpdateMsg(rmsg, pItemCollectBox, 0);
			SEND_Q(rmsg, ch->m_desc);

			// 4. 바닥에서 제거
			CNetMsg		disappmsg;
			CNetMsg		takemsg;
			int			cx = pItem->m_cellX;
			int			cz = pItem->m_cellZ;
			char		yLayer = GET_YLAYER(pItem);
			CArea*		pArea = pItem->m_pArea;

			ItemTakeMsg(takemsg, ch, pItem);
			ItemDisappearMsg(disappmsg, pItem);
			pItem->m_preferenceIndex = -1;
			pArea->ItemFromCell(pItem);
			pArea->SendToCell(takemsg, ch, true);
			pArea->SendToCell(disappmsg, yLayer, cx, cz);
			EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_PICK_OK, pItem->m_idNum);
			SEND_Q(rmsg, ch->m_desc);

			// 5. 게임 로그
			GAMELOG << init("EVENT COLLECT BUG PICK", ch)
					<< "BUG" << delim
					<< itemlog(pItem) << delim
					<< "BOX" << delim
					<< itemlog(pItemCollectBox)
					<< end;

			delete pItem;

			return true;
		}
	}

	return false;
}
#endif // EVENT_COLLECT_BUG

bool do_ItemUse_IONCE_WARP_RETURN(CPC* ch, const CItemProto* itemproto)
{
#ifdef MARGADUM_PVP
	if( gserver.m_bMargadumPvp && itemproto->m_num1 == 13 )
	{	
		//0: free 1:16~25, 2: 26~35, 3: 36~ 
		switch(gserver.m_bMargadumPvpLevel)
		{
		case 0:
			{
				if( ch->m_level < 16 )
				{
					CNetMsg rmsg;
					SysMsg(rmsg, MSG_SYS_NOTTIME_GOZONE);

					SEND_Q(rmsg, ch->m_desc);
					ch->m_magadumDamage = 0;

					return false;
				}
			}
			break;
		case 1:
			if( ch->m_level < 16 || ch->m_level > 25 )
			{
				CNetMsg rmsg;
				SysMsg(rmsg, MSG_SYS_NOTTIME_GOZONE);

				SEND_Q(rmsg, ch->m_desc);
				ch->m_magadumDamage = 0;

				return false;
			}
			break;
		case 2:
			if( ch->m_level < 26 || ch->m_level > 35 )
			{
				CNetMsg rmsg;
				SysMsg(rmsg, MSG_SYS_NOTTIME_GOZONE);

				SEND_Q(rmsg, ch->m_desc);
				ch->m_magadumDamage = 0;

				return false;
			}
			break;
		case 3:
			if( ch->m_level < 36 )
			{
				CNetMsg rmsg;
				SysMsg(rmsg, MSG_SYS_NOTTIME_GOZONE);

				SEND_Q(rmsg, ch->m_desc);
				ch->m_magadumDamage = 0;

				return false;
			}
			break;
		default:
			return false;
		}

		ch->m_magadumDamage = 1;
	}
	else if( itemproto->m_num1 == 13 )
	{
		CNetMsg rmsg;
		SysMsg(rmsg, MSG_SYS_NOTTIME_GOZONE);

		SEND_Q(rmsg, ch->m_desc);

		ch->m_magadumDamage = 0;

		return false;
	}
#endif

	// 이동 가능 검사
	int canWarp = ch->CanWarp();
	if (canWarp != 0)
	{
		CNetMsg rmsg;
		SysMsg(rmsg, (MSG_SYS_TYPE)canWarp);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
	
	// 10초후 이동하게 하기
	ch->m_reqWarpType = IONCE_WARP_RETURN;
	ch->m_reqWarpData = itemproto->m_num1;
	ch->m_reqWarpTime = PULSE_WARPDELAY;
	ch->SetPlayerState(PLAYER_STATE_WARP);
	
	// 사용했음을 알리기
	CNetMsg warpMsg;
	WarpStartMsg(warpMsg, ch);
	ch->m_pArea->SendToCell(warpMsg, ch, true);

	return true;
}

bool do_ItemUse_IONCE_CHANGE_DOC(CPC* ch, const CItemProto* itemproto)
{
	int canChange = ch->CanChange();
	CNetMsg rmsg;

	if (canChange != -1)
	{
		ChangeErrMsg(rmsg, (MSG_CHANGE_ERR_TYPE) canChange);
		SEND_Q(rmsg, ch->m_desc);
		return false;

	}
	// 레벨 검사
	if (ch->m_level < itemproto->m_num2)
	{
		ChangeErrMsg(rmsg, MSG_CHANGE_ERR_LEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	// 존 검사
	if (ch->m_pZone->m_index != itemproto->m_num3)
	{
		ChangeErrMsg(rmsg, MSG_CHANGE_ERR_ZONE);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

	DelAttackList(ch);

	ch->m_changePulse = itemproto->m_num1;
	ch->m_changeIndex = itemproto->m_num0;
	ch->SetPlayerState(PLAYER_STATE_CHANGE);

	ChangeStartMsg(rmsg, ch);
	ch->m_pArea->SendToCell(rmsg, ch, true);

	return true;
}

bool do_ItemUse_IONCE_QUEST_SCROLL(CPC* ch, const CItemProto* itemproto, int extra1)
{
	if(itemproto->m_num0 == 0 || itemproto->m_num0 == -1)
	{
		return false;
	}
	
	bool ret;

	if(extra1 != -1)
	{				
		ret = do_QuestStart(ch, itemproto->m_num0);
		
		if(ret)
		{
			
		}
		else
		{
			return false;
		}
	}
	else
	{
		ret = do_QuestGiveUp(ch, itemproto->m_num0);

		if(ret)
		{
			
		}
		else
		{
			return false;
		}
	}

	return true;
}

#ifdef DRATAN_CASTLE
bool do_ItemUse_Castle_Guard(CPC* ch, CItem* item)
{
	// 공성 지역인지 확인
	if(ch->GetMapAttr() != MAPATT_WARZONE
		|| ch->m_pZone->m_index != ZONE_DRATAN)
	{
		return false;
	}

	// 피라미드 상층이면 불가
	if(ch->m_pos.m_yLayer != 0)
	{
		return false;
	}

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if(pCastle == NULL)
	{
		return false;
	}

#ifndef KJTEST	
	// 해당 성의 성주인지 확인
	if(pCastle->GetOwnerCharIndex() != ch->m_index)
	{
		return false;
	}
#endif // KJTEST

	// 종류별 개수 확인	
	int mobindex = 0;
	switch(item->m_itemProto->m_index)
	{
	case 2380:
		mobindex = 405;
		break;
	case 2381:
		mobindex = 406;
		break;

	default:
		mobindex = 0;
		break;
	}

	if(mobindex == 0)
	{
		return false;
	}

	if (pCastle->m_GuardList.GetCount(mobindex) >= pCastle->m_GuardList.GetMaxKindCount())
	{
		return false;
	}

	// 사용 가능 시간 확인(공성전중에는  사용불가)
	if(pCastle->GetState() != WCSF_NORMAL)
	{
		return false;
	}

	CNetMsg msg;
	if(IS_RUNNING_HELPER)
	{
		// 가디언 추가
		if( pCastle->m_GuardList.InsertGuard(mobindex, ch->m_pos.m_x, ch->m_pos.m_z, 0) == false)
		{
			return false;
		}

		int regensec = 20*60;

		float r = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		int idx = ch->m_pZone->m_area->m_npcRegenList.m_wPos;
		ch->m_pZone->m_area->m_npcRegenList.AddRegenInfo(
			idx, mobindex, regensec, ch->m_pos.m_x, 0, ch->m_pos.m_z, r, 1, ZONE_DRATAN);
	
		HelperInsertGuardMsg(msg, ch, item);
		SEND_Q(msg, gserver.m_helper);

		CNPCProto* proto = gserver.m_npcProtoList.FindProto(mobindex);
		if (proto && proto->CheckFlag(NPC_CASTLE_GUARD))
		{
			CNPCRegenInfo * p = ch->m_pZone->m_area->m_npcRegenList.GetNpcRegenInfo(idx);
			CNPC* npc = gserver.m_npcProtoList.Create(mobindex, p);
			CArea * area = ch->m_pZone->m_area;
			if (npc == NULL || p == NULL)
			{
				return false;
			}

			GET_X(npc) = p->m_regenX;
			GET_Z(npc) = p->m_regenZ;
			GET_R(npc) = p->m_regenR;
			GET_YLAYER(npc) = p->m_regenY;

			npc->m_regenX = GET_X(npc);
			npc->m_regenZ = GET_Z(npc);
			npc->m_regenY = GET_YLAYER(npc);

			npc->m_recoverPulse = gserver.m_pulse;
#ifndef NEW_DIVISION_EXPSP
			npc->m_totalDamage = 0;
#endif // #ifndef NEW_DIVISION_EXPSP

			p->m_bAlive = true;
			p->m_lastDieTime = 0;
			npc->InitPointsToMax();
			npc->m_disableTime = 0;

			npc->InitPointsToMax();
			area->AddNPC(npc);
			npc->m_postregendelay = 0;

			int cx, cz;
			area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			area->CharToCell(npc, GET_YLAYER(npc), cx, cz);

			CNetMsg rmsg;
			AppearMsg(rmsg, npc, true);
			area->SendToCell(rmsg, npc);
		}

//		InsertGuardMsg(msg, ch, item);
//		SEND_Q(msg, ch->m_desc);

		return true;
	}
	
	return false;
}
#endif // DRATAN_CASTLE

bool do_ItemUse_IONCE_SUMMON(CPC* ch, CItem* item)
{
	if(item->m_itemProto->m_index == 2381		// [용병]스핑크스 창병
		|| item->m_itemProto->m_index == 2380)	// [용병]스핑크스 파이터
	{
#ifdef DRATAN_CASTLE
		return do_ItemUse_Castle_Guard(ch, item);
#else
		return false;
#endif // DRATAN_CASTLE
	}

	// 보스 몬스터 소환서
#ifdef ENABLE_WAR_CASTLE
	// 공성중에 공성 지역(MAPATT_WARZONE) 으로 이동 불가
	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle)
	{
		if (castle->GetState() != WCSF_NORMAL)
		{
			if ( ch->GetMapAttr() == MAPATT_WARZONE)
			{
				CNetMsg rmsg;
				ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_SUMMON_NOTALLOWZONE);
				SEND_Q(rmsg, ch->m_desc);
				return false;
			}
		}
	}
#endif
	// ox 퀴즈룸 길드룸 쪽도 사용하지 못하도록 변경 
	if( ch->IsInPeaceZone(true)
		|| ch->m_pZone->IsPersonalDungeon() ||  ch->GetMapAttr() == MAPATT_FREEPKZONE
		|| ch->m_pZone->IsGuildRoom() || ch->m_pZone->IsOXQuizRoom() )
	{
		CNetMsg rmsg;
		ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_SUMMON_NOTALLOWZONE);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}


	int npc_index = item->m_itemProto->m_num0;

#ifdef MOB_SCROLL
	static const int patterns[2][13] =
	{ 
		{1, 297, 0, 9, 133, 132, 264, 125, 126, 158, 68, 69, 250,},	// 2144
		{2, 190, 267, 8, 250, 251, 252, 189, 188, 236, 190, 189, 0,},	// 2145
	};

	int pattern_index;

	if (item->m_itemProto->m_index == 2144 
		|| item->m_itemProto->m_index == 2145)
	{
		pattern_index = item->m_itemProto->m_index - 2144;
		int temp = GetRandom(4, 4 + patterns[pattern_index][3] - 1);
		npc_index = patterns[pattern_index][temp];
	}
#endif // MOB_SCROLL

	CNPCProto* proto = gserver.m_npcProtoList.FindProto(npc_index);
	if (!proto)
	{
		return false;
	}

	CNPC* npc = gserver.m_npcProtoList.Create(npc_index, NULL);
	if (!npc)
		return false;

#ifdef MOB_SCROLL
	if (item->m_itemProto->m_index == 2144 
		|| item->m_itemProto->m_index == 2145)
	{	// 몹에 스크롤 정보 저장
		npc->m_MobScrollType = pattern_index;
		npc->m_NextMobIndex = 0;
		npc->m_UseCharIndex = ch->m_index;
	}
#endif // MOB_SCROLL
	
	int nTry = 0;
	do
	{
		GET_X(npc) = GET_X(ch) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
		GET_YLAYER(npc) = GET_YLAYER(ch);
		GET_Z(npc) = GET_Z(ch) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
		GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		if (ch->m_pArea->GetAttr(GET_YLAYER(npc), GET_X(npc), GET_Z(npc)) == MAPATT_FIELD)
			break;
		nTry++;
	} while (nTry <= 10);

	if (nTry > 10)
	{
		GET_X(npc) = GET_X(ch);
		GET_Z(npc) = GET_Z(ch);
		GET_YLAYER(npc) = GET_YLAYER(ch);
		GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
	}

	npc->m_regenX = GET_X(npc);
	npc->m_regenY = GET_YLAYER(npc);
	npc->m_regenZ = GET_Z(npc);

	int cx, cz;
	ch->m_pArea->AddNPC(npc);
	ch->m_pArea->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
	ch->m_pArea->CharToCell(npc, GET_YLAYER(npc), cx, cz);
	
	CNetMsg appearNPCMsg;
	AppearMsg(appearNPCMsg, npc, true);
	ch->m_pArea->SendToCell(appearNPCMsg, GET_YLAYER(npc), cx, cz);

	npc->m_postregendelay += 2 * PULSE_REAL_SEC;

	return true;
}

bool do_ItemUse_IETC_PRODUCT(CPC* ch, const CItemProto* itemproto)
{
	// 펫 사료 먹이기
	if (ch->m_wearing[WEARING_PET])
	{
		CPet* pet = ch->GetPet();
		if (!pet)
			return false;

		// 등급에 따라 수치
		int upVal = 0;
		switch (itemproto->m_index)
		{
		case 157:		// A등급 원소
		case 152:		// 스톤 웰스던
		case 162:		// 크락의 꽃
			upVal = 9;
			break;

		case 158:		// B등급 원소
		case 153:		// 스톤 비스트
		case 163:		// 크락의 가시
			upVal = 7;
			break;

		case 159:		// C등급 원소
		case 154:		// 스톤 웨버
		case 197:		// 크락의 파란잎
			upVal = 5;
			break;

		case 160:		// D등급 원소
		case 155:		// 스톤 리스
		case 198:		// 크락의 줄기
			upVal = 3;
			break;

		case 161:		// E등급 원소
		case 156:		// 스톤 온
		case 199:		// 크락의 노란잎
			upVal = 1;
			break;

		default:
			return false;
		}

		bool bUpdate = false;
		// 종류에 따라 회복 되는 파리미터 다름
		switch (itemproto->m_index)
		{
		case 157:		// A등급 원소
		case 158:		// B등급 원소
		case 159:		// C등급 원소
		case 160:		// D등급 원소
		case 161:		// E등급 원소
			// 교감도 회복
			bUpdate = pet->IncreaseSympathyPoint(upVal);
			break;

		case 152:		// 스톤 웰스던
		case 153:		// 스톤 비스트
		case 154:		// 스톤 웨버
		case 155:		// 스톤 리스
		case 156:		// 스톤 온
			// 배고픔 회복
			bUpdate = pet->IncreaseHungryPoint(upVal);
			break;

		case 162:		// 크락의 꽃
		case 163:		// 크락의 가시
		case 197:		// 크락의 파란잎
		case 198:		// 크락의 줄기
		case 199:		// 크락의 노란잎
			// HP 회복
			if (pet->m_hp < pet->m_maxHP)
			{
				pet->m_hp += upVal;
				if (pet->m_hp > pet->m_maxHP)
					pet->m_hp = pet->m_maxHP;
				bUpdate = true;
			}
			break;

		default:
			return false;
		}

		if (bUpdate)
		{
			CNetMsg rmsg;
			ExPetStatusMsg(rmsg, pet);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else
		return false;

	return true;
}

bool do_ItemUse_IONCE_ETC(CPC* ch, const CItemProto* itemproto, int nExtar1)
{
	switch (itemproto->m_index)
	{
	case 1838:		// 돋보기
	case 1839:		// 황금 돋보기
#ifdef LIMIT_ITEM_M35
	case 2666:		// 초보자용 돋보기
#endif // LIMIT_ITEM_M35
		return do_ItemUse_1838_1839(ch, itemproto, nExtar1);
		break;

	case 2037:		// 럭키 드로우 상자
		return do_ItemUse_2037(ch, itemproto);
		break;

	case 2597:
		return do_ItemUse_2597(ch, itemproto);
		break;

	case 2887:
		return do_ItemUse_2887(ch, itemproto);
		break;
	case 2888:
		return do_ItemUse_2888(ch, itemproto);
		break;
	case 2889:
		return do_ItemUse_2889(ch, itemproto);
		break;
	case 2890:
		return do_ItemUse_2890(ch, itemproto);
		break;
	case 2891:
		return do_ItemUse_2891(ch, itemproto);
		break;
	case 2892:
		return do_ItemUse_2892(ch, itemproto);
		break;
	case 2893:
		return do_ItemUse_2893(ch, itemproto);
		break;
	case 2968:
		return do_ItemUse_2968(ch, itemproto);
		break;
#ifdef EVENT_TLD_2007_SONGKRAN		
	case 2146:
	case 2147:
		return do_ItemUse_2146_2147( ch, itemproto );
		break;
#endif // EVENT_TLD_2007_SONGKRAN

#ifdef EVENT_EGGS_HUNT_2007
	case 2148:
		return do_ItemUse_2148( ch, itemproto );
#endif // EVENT_EGGS_HUNT_2007

#ifdef EVENT_NEW_MOONSTONE
	case 2544:
		return do_itemUse_MoonStoneBox( ch, itemproto );
#endif // 
	case 2859:
	case 2982:
		{
			// 스트레이아나 백신 물약
			CSkill* pSkill = gserver.m_skillProtoList.Create(itemproto->m_num0, itemproto->m_num1);
			if(pSkill)
			{
				bool bApply;
				ApplySkill(ch, ch, pSkill, itemproto->m_index, bApply);
				if(!bApply)
				{
					delete pSkill;
					return false;
				}
			}
			
			ch->m_assist.CureBySkillIndex(511);
			ch->m_SkillTime_511 = 0;
			return true;
		}
	default:
		return false;
		break;
	}
	return true;
}

bool do_ItemUse_1838_1839(CPC* ch, const CItemProto* itemproto, int nExtar1)
{
#ifdef MONSTER_RAID_SYSTEM
	int nProb = 0;
	switch (itemproto->m_index)
	{
	case 1838:	// 돋보기
#ifdef HOLY_WATER
//		nProb = 40;
		nProb = 30;		//  2007.05.02
#else
		nProb = 50;
#endif	// HOLY_WATER
		break;
	case 1839:	// 황금 돋보기
		nProb = 100;
		break;
#ifdef LIMIT_ITEM_M35
	case 2666:	// 초보자용 돋보기
		nProb = 100;
		break;
#endif // LIMIT_ITEM_M35
	default:
		return false;
	}

	CItem* pItemRare = ch->m_invenNormal.GetItem(nExtar1);
	if (!pItemRare)
		return false;
	if (!pItemRare->IsRareItem())
		return false;
	if (pItemRare->m_pRareOptionProto)
		return false;

#ifdef SET_ITEM
	if (pItemRare->IsOriginItem())
		return false;
#endif

	CNetMsg rmsg;

	if (GetRandom(1, 100) <= nProb)
	{
		GAMELOG << init("ITEM IDENTIFY RARE", ch)
				<< "BEFORE" << delim
				<< itemlog(pItemRare) << delim;
		pItemRare->IdentifyRareOption();
		GAMELOG << "AFTER" << delim
				<< itemlog(pItemRare)
				<< end;

		ItemUpdateMsg(rmsg, pItemRare, 0);
		SEND_Q(rmsg, ch->m_desc);

		ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_IDENTIFY_RARE_OK);
		SEND_Q(rmsg, ch->m_desc);

		ch->CalcStatus(true);
	}
	else
	{
		ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_IDENTIFY_RARE_FAIL);
		SEND_Q(rmsg, ch->m_desc);
	}

	return true;
#else // MONSTER_RAID_SYSTEM
	return false;
#endif // MONSTER_RAID_SYSTEM
}

#ifdef EVENT_TLD_2007_SONGKRAN
bool do_ItemUse_2146_2147( CPC* ch, const CItemProto* itemproto )
{
	const int ItemTable[2][9][4] ={
		//은그릇 보상 목록
		{ {974, 0, 1, 1},				// 행운의 고급제련석 1개 확률 0.1%
		{85, 0, 1, 3},				// 고급제련석 1개 확률 0.3%
		{498, 6, 5, 100},			// 6Lv 붉은 케르 5개 확률 10%
		{499, 6, 5, 100},			// 6Lv 붉은 넨 5개 확률 10%
		{500, 6, 5, 100},			// 6Lv 푸른 케르 5개 확률 10%
		{501, 6, 5, 100},			// 6Lv 푸른 넨 5개 확률 10%
		{723, 0, 3, 96},			// 문스톤 3개 확률 9.6%
		{45, 0, 5, 250},			// 대형 체력 회복 물약 5개 확률 25%
		{724, 0, 5, 250},			// 대형 마나 회복 물약 5개 확률 25%
		}
	,
	{// 금그릇 보상 목록
		{779, 0, 1, 1},				// 초 고급제련석 1개 확률 0.1%
		{85, 0, 1, 9},				// 고급제련석 1개 확률 0.9%
		{498, 12, 5, 100},			// 12Lv 붉은 케르 5개 확률 10%
		{499, 12, 5, 100},			// 12Lv 붉은 넨 5개 확률 10%
		{500, 12, 5, 100},			// 12Lv 푸른 케르 5개 확률 10%
		{501, 12, 5, 100},			// 12Lv 푸른 넨 5개 확률 10%
		{723, 0, 3, 90},			// 문스톤 3개 확률 9%
		{45, 0, 10, 250},			// 대형 체력 회복 물약 10개 확률 25%
		{724, 0, 10, 250},			// 대형 마나 회복 물약 10개 확률 25%
	},	
	};

	CNetMsg	rmsg;
	int		nRow, nCol;	
	int		exchangeType = 0;
	CItem*	pItemMade		= NULL;
	static const int	nIndexGoldeBowl		= 2146;	// 금그릇
	static const int	nIndexSilverBowl	= 2147;	// 은그릇

	if( itemproto->m_index == nIndexSilverBowl ) // 은그룻
	{
		exchangeType = 0;
	
	}
	else if ( itemproto->m_index == nIndexGoldeBowl ) // 금그릇
	{
		exchangeType = 1;
	}

	// 인벤토리에 최소 1개의 빈공간이 있어야 한다
	if (ch->m_invenNormal.GetSpace() < 1)
	{
		SysFullInventoryMsg(rmsg, INVENTORY_NORMAL);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}




	int i;
	int nProbGift = GetRandom(0, 1000);

	for	( i = 0; i < 9; i++ )
	{
		if (nProbGift <= ItemTable[exchangeType][i][3] )
		{			
			pItemMade = gserver.m_itemProtoList.CreateItem( ItemTable[exchangeType][i][0], -1, 0, ItemTable[exchangeType][i][1], ItemTable[exchangeType][i][2] );
			
			if( !pItemMade )
			{
				EventSongkran2007Msg( rmsg, MSG_EVENT_SONGKRAN_2007_EXCHANGE_NOITEM, exchangeType );
				SEND_Q( rmsg, ch->m_desc );
				return false;
			} // if( !pItemMade )

				// 인벤토리에 최소 무게가 보장되어야 한다.
			if (ch->m_weight + pItemMade->m_itemProto->m_weight >= (ch->m_maxWeight * 3 / 2))
			{
				SysFullInventoryMsg(rmsg, INVENTORY_NORMAL);
				SEND_Q(rmsg, ch->m_desc);
				return false;
			}

			if (!AddToInventory( ch, pItemMade, true, true ) )
			{
				delete pItemMade;
				pItemMade = NULL;
				EventSongkran2007Msg( rmsg, MSG_EVENT_SONGKRAN_2007_EXCHANGE_FULLINVEN, exchangeType );
				SEND_Q( rmsg, ch->m_desc );
				return false;

			}
			break;

		}// if (nProbGift <= ItemTable[exchangeType][i][3] )
		else
		{
			nProbGift -= ItemTable[exchangeType][i][3];
		}
	} // 	for	( i = 0; i < 9; i++ )		

		// 결과 전송
		if (pItemMade->tab() < 0)
		{
			CItem*			pItemPrev = NULL;
			if( ch->m_invenNormal.FindItem( &nRow, &nCol, pItemMade->m_itemProto->m_index, pItemMade->m_plus, pItemMade->m_flag ) )
			{
				delete pItemMade;
				pItemMade = NULL;
				pItemPrev = ch->m_invenNormal.GetItem(nRow, nCol);
				if( pItemPrev )
				{
					GAMELOG << init("EVENT_TLD_2007_SONGKRAN", ch)
						<< "PACKAGED ITEM INDEX" << delim
						<< ItemTable[exchangeType][i][0] << delim
						<< "PACKAGED ITEM COUNT" << delim
						<< ItemTable[exchangeType][i][2] << delim
						<< itemlog(pItemMade)
						<< end;

					ItemUpdateMsg(rmsg, pItemPrev, ItemTable[exchangeType][i][2] );
					SEND_Q(rmsg, ch->m_desc);
				} // if( pItem )
			}// if( pInven->FindItem(&nRow, &nCol, pItemMade->m_itemProto->m_index, pItemMade->m_plus, pItemMade->m_flag )

		}
		else
		{
			ItemAddMsg(rmsg, pItemMade);
			SEND_Q(rmsg, ch->m_desc);
			pItemMade = NULL;
		}
		//EventSongkran2007Msg(rmsg, MSG_EVENT_SONGKRAN_2007_EXCHANGE_OK, exchangeType );
		//SEND_Q(rmsg, ch->m_desc);

	return true;
		
}
#endif //EVENT_TLD_2007_SONGKRAN

bool do_ItemUse_2597(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_invenNormal.GetSpace() < 1)
	{	// 인벤에 공간이 없으면 실패 
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	CNetMsg rmsg;

	const int nTableSize	= 10;

	int nTable[nTableSize][2]	= {
		{ 853, 2209, },		// 대형 퀵 체력 회복 물약
		{ 854, 2200, },		// 대형 퀵 마나 회복 물약
		{ 1315, 2000, },		// 공격력 향상제 (대)
		{ 1314, 2000, },		// 방어력 향상제 (대)
		{ 1418, 500, },		// 플래티늄 제련석
		{ 974, 500, },		// 행운의 제련석
		{ 1706, 290, },		// 플래티늄 제련석
		{ 2394, 200, },		// 플래티늄 하급 제련 보호석
		{ 2395, 100, },		// 플래티늄 상급 제련 보호석
		{ 1835, 1, },		// 그림자의 돌
	};

	int nRate = GetRandom(0, 10000);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate < nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver.m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, 1);
	if (pItem == NULL)
		return false;
	if (!AddToInventory(ch, pItem, true, true))
	{
		delete pItem;
		pItem = NULL;
		return false;
	}
	if (pItem->tab() < 0)
	{
		int nRow = -1, nCol = -1;
		ch->m_invenNormal.FindItem(&nRow, &nCol, pItem->m_itemProto->m_index, 0, 0);
		CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pPrevItem)
		{
			GAMELOG << init("USE SUPER LUCKY THROW BOX", ch)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< "TOTAL" << delim
					<< itemlog(pPrevItem)
					<< end;
			ItemUpdateMsg(rmsg, pPrevItem, 1);
			SEND_Q(rmsg, ch->m_desc);
			delete pItem;
			pItem = NULL;
		}
	}
	else
	{
		GAMELOG << init("USE SUPER LUCKY THROW BOX", ch)
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< "TOTAL" << delim
				<< itemlog(pItem)
				<< end;
		ItemAddMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);
		pItem = NULL;
	}

	SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->m_index);
	SEND_Q(rmsg, ch->m_desc);

	return true;
}

bool do_ItemUse_2037(CPC* ch, const CItemProto* itemproto)
{
	CNetMsg rmsg;
#ifdef LC_MAL
	const int nTableSize	= 500;
	int nTable[nTableSize]	= {
		110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,
		130,131,132,133,134,135,136,137,138,139,140,154,155,156,159,160,161,197,198,199,
		200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,
		220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
		240,241,242,243,244,245,246,247,248,249,250,251,293,294,295,296,297,298,299,300,
		301,302,303,304,305,400,416,424,425,426,486,487,510,511,556,671,672,673,674,675,
		676,677,708,709,710,711,712,713,714,759,829,830,831,832,833,834,835,840,841,842,
		843,844,845,846,847,848,849,853,854,855,856,857,858,886,887,888,889,890,891,892,
		893,894,895,896,897,898,899,900,901,902,903,904,905,906,907,908,909,910,911,912,
		913,914,915,916,917,918,919,920,921,922,923,924,925,926,927,928,929,930,931,932,
		933,934,935,936,937,938,939,940,941,942,943,944,945,946,947,948,949,1073,1074,1075,
		1076,1077,1078,1079,1080,1081,1082,1083,1084,1085,1086,1087,1088,1089,1090,1091,1092,1093,1094,1095,
		1096,1097,1098,1099,1100,1101,1102,1103,1104,1105,1106,1107,1108,1109,1110,1111,1112,1113,1114,1115,
		1116,1117,1118,1119,1126,1127,1128,1129,1130,1131,1132,1136,1137,1138,1139,1140,1141,1142,1143,1144,
		1145,1146,1147,1148,1149,1150,1151,1152,1153,1154,1155,1156,1157,1158,1159,1160,1161,1162,1163,1164,
		1165,1166,1167,1168,1169,1170,1171,1172,1173,1174,1175,1176,1177,1178,1179,1180,1181,1182,1183,1184,
		1185,1186,1187,1188,1189,1190,1191,1192,1193,1194,1195,1196,1197,1198,1199,1200,1201,1202,1203,1204,
		1205,1206,1207,1208,1208,1209,1209,1210,1210,1211,1211,1212,1212,1213,1214,1215,1216,1217,1218,1219,
		1220,1221,1222,1223,1224,1281,1283,1284,1285,1319,1320,1321,1322,1323,1324,1325,1326,1327,1328,1329,
		1330,1331,1332,1333,1334,1335,1336,1337,1338,1339,1340,1341,1342,1343,1344,1345,1346,1347,1348,1349,
		1350,1351,1352,1353,1354,1355,1356,1357,1358,1359,1360,1361,1362,1363,1364,1365,1366,1367,1368,1369,
		1370,1371,1372,1373,1374,1375,1376,1377,1378,1379,1380,1381,1382,1383,1384,1385,1386,1387,1388,1389,
		1390,1391,1392,1407,1410,1541,1542,1543,1544,1545,1546,1547,1548,1549,1550,1551,1552,1553,1554,1555,
		1556,1557,1558,1559,1560,1561,1562,1563,1564,1565,1566,1567,1568,1569,1570,1614,1615,1616,1617,1618,
		1619,1620,1621,1622,1623,1624,1625,1631,1819,1820,1821,1822,1840,1975,1976,1977,2033,2034,2135,2136};
#else
	const int nTableSize	= 6;
	int nTable[nTableSize]	= {
		2032,		// 럭키 아이리스의 축복
		1840,		// 소형 경험치 포션
		2033,		// 럭키 소울 닉스
		2034,		// 럭키 다지 피어
		2035,		// 럭키 경험치복구 주문서
		2036,		// 럭키 숙련도복구 주문서
	};
#endif// LC_MAL
	int nIndex = GetRandom(0, nTableSize - 1);

	CItem* pItem = gserver.m_itemProtoList.CreateItem(nTable[nIndex], -1, 0, 0, 1);
	if (pItem == NULL)
		return false;
	if (!AddToInventory(ch, pItem, true, true))
	{
		delete pItem;
		pItem = NULL;
		return false;
	}
	if (pItem->tab() < 0)
	{
		int nRow = -1, nCol = -1;
		ch->m_invenNormal.FindItem(&nRow, &nCol, pItem->m_itemProto->m_index, 0, 0);
		CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pPrevItem)
		{
			GAMELOG << init("USE LUCKY THROW BOX", ch)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< "TOTAL" << delim
					<< itemlog(pPrevItem)
					<< end;
			ItemUpdateMsg(rmsg, pPrevItem, 1);
			SEND_Q(rmsg, ch->m_desc);
			delete pItem;
			pItem = NULL;
		}
	}
	else
	{
		GAMELOG << init("USE LUCKY THROW BOX", ch)
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< "TOTAL" << delim
				<< itemlog(pItem)
				<< end;
		ItemAddMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);
		pItem = NULL;
	}

	SysChangeLuckyBoxMsg(rmsg, nTable[nIndex], itemproto->m_index);
	SEND_Q(rmsg, ch->m_desc);

	return true;
}

#ifdef MONSTER_COMBO_COIN_CHANGE
bool do_ItemUse_Coin(CPC * pc, const CItemProto * pItemProto, int nExtra1)
{
	if(pc == NULL
		|| pItemProto == NULL)
	{
		return false;
	}

	// 지급 아이템 테이블
	const int nItems[5][5] = {
		{ 2715, 2716, 2717, 2718, 2719, },
		{ 2720, 2721, 2722, 2723, 2724, },
		{ 2725, 2726, 2727, 2728, 2729, },
		{ 2730, 2731, 2732, 2733, 2734, },
		{ 2735, 2736, 2737, 2738, 2739, },
	};


	int item_index = -1, coin_index = -1, coin_count = -1, op_level = -1;
	switch(pItemProto->m_index)
	{
	case 2740:
		item_index = 0;
		coin_index = 2710;
		coin_count = 10;
		op_level = 1;
		break;

	case 2741:
		item_index = 1;
		coin_index = 2711;
		coin_count = 30;
		op_level = 2;
		break;

	case 2742:
		item_index = 2;
		coin_index = 2712;
		coin_count = 50;
		op_level = 3;
		break;

	case 2743:
		item_index = 3;
		coin_index = 2713;
		coin_count = 70;
//		coin_count = 100;
		op_level = 4;
		break;

	case 2744:
		item_index = 4;
		coin_index = 2714;
		coin_count = 100;
//		coin_count = 200;
		op_level = 5;
		break;

	default:
		return false;
	}

	// 인벤에 해당 코인 확인
	int r = -1, c = -1;
	if(pc->m_invenNormal.FindItem(&r, &c, coin_index, 0, 0) == false)
	{
		return false;
	}

	CItem * pItem = pc->m_invenNormal.GetItem(r, c);
	if(pItem == NULL
		|| pItem->Count() < coin_count)
	{
		return false;
	}

	// 아이템 생성
	int random_item = GetRandom(0, 4);
	CItem * pChange = gserver.m_itemProtoList.CreateItem(nItems[item_index][random_item], -1, 0, 0, 1);
	if(pChange == NULL)
	{
		return false;
	}

		// 	옵션 붙이기
// 		if(pChange->IsRareItem() == false)
// 		{
// 			return false;
// 		}
// 	
// 		pChange->IdentifyRareOption();

		// 	옵션 확인
// 		if(pChange->m_pRareOptionProto == NULL)
// 		{
// 			return false;
// 		}
	
	// 옵션 붙는 갯수
	int num = GetRandom(1, 5);
	
	// 옵션 테이블
	int nOptions[5][7] = { 
		{3, 6, 12, 19, 23, 52, 56,}, 
		{1, 5, 12, 19, 23, 53, 56,}, 
		{2, 5, 12, 19, 23, 53, 56,}, 
		{4, 7, 17, 21, 24, 22, 56,}, 
		{4, 5, 53, 52, 23, 22, 56} 
	};

	GAMELOG << init("MONSTER_COMBO_COIN_CHANGE") 
		<< nItems[item_index][random_item] << delim
		<< "Option count: " << num;

	// 옵션 붙이기
	for(int i = 0; i < num; i++)
	{
		int tmpOp = nOptions[random_item][GetRandom(0, 6)];

		// 옵션번호로 붙을 수 있는 옵션인지 검사
		COptionProto * proto = gserver.m_optionProtoList.FindProto(tmpOp - 1);
		if(proto == NULL)
		{
			continue;
		}

		// Level, Value 셋팅
//		pChange->m_option[i].MakeOptionValue(proto, op_level, num);
		pChange->m_option[i].m_proto = proto;
		pChange->m_option[i].m_type = proto->m_type;
		pChange->m_option[i].m_level = op_level;
		pChange->m_option[i].m_value = proto->m_levelValue[op_level - 1];
		pChange->m_option[i].SetDBValue();
		pChange->m_nOption++;

		GAMELOG << delim << tmpOp;
	}

	GAMELOG << end;

	// 아이템 지급
	if(AddToInventory(pc, pChange, true, true) == false)
	{
		return false;
	}

	// 코인 제거
	if(DecreaseFromInventory(pc, pItem, coin_count) == false)
	{
		// 코인 제거 실패시 지급된 아이템 제거
		DecreaseFromInventory(pc, pChange, 1);
		delete pChange;
		pChange = NULL;
		return false;
	}

	// 아이템 지급 메세지
	CNetMsg itemmsg;
	ItemAddMsg(itemmsg, pChange);	// 겹치지 않는 아이템 추가 메세지만 전송
	SEND_Q(itemmsg, pc->m_desc);

	// 코인 제거 메세지
	if(pItem->Count() > 0)
	{
		ItemUpdateMsg(itemmsg, pItem, -coin_count);
		SEND_Q(itemmsg, pc->m_desc);
	}
	else
	{
		ItemDeleteMsg(itemmsg, pItem);
		SEND_Q(itemmsg, pc->m_desc);
		RemoveFromInventory(pc, pItem, true, true);
	}

	return true;
}

#endif // MONSTER_COMBO_COIN_CHANGE

#if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 
bool do_ItemUse_2046(CPC* ch, const CItemProto* itemproto, int nExtar1)
{
	CNetMsg rmsg;
	CItem* pCacaoItem;

	pCacaoItem = ch->m_invenNormal.GetItem(nExtar1);
	if(!pCacaoItem)
	{
		// 인벤에서 아이템 찾지 못했음
		return false;
	}

	CItem* pPackagedItem;
	switch(pCacaoItem->m_itemProto->m_index)
	{
	case 2043:
		pPackagedItem = gserver.m_itemProtoList.CreateItem(2048, -1, 0, 0, 1);
		break;
	case 2044:
		pPackagedItem = gserver.m_itemProtoList.CreateItem(2049, -1, 0, 0, 1);
		break;
	case 2045:
		pPackagedItem = gserver.m_itemProtoList.CreateItem(2050, -1, 0, 0, 1);
		break;

	case 2629:
		pPackagedItem = gserver.m_itemProtoList.CreateItem(2634, -1, 0, 0, 1);
		break;
	case 2630:
		pPackagedItem = gserver.m_itemProtoList.CreateItem(2635, -1, 0, 0, 1);
		break;
	case 2631:
		pPackagedItem = gserver.m_itemProtoList.CreateItem(2636, -1, 0, 0, 1);
		break;

	default:				// 카카오 30, 60, 90 인 것만 포장할 수 있다	// 복주머니 셋트 추가
		return false;
	}
	
	if(!pPackagedItem) return false;

	// 포장된 카카오 아이템 추가
	if (!AddToInventory(ch, pPackagedItem, true, true))
	{
		delete pPackagedItem;
		pPackagedItem = NULL;
		EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_PACKAGE_FULLINVEN);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}

//#error "TODO gamelog"
	
	CNetMsg itemmsg;
	if (pPackagedItem->tab() == -1)
	{	
		// 수량 변경 알림
		// 겹쳤는지 검사
		int r, c;
		if (ch->m_invenNormal.FindItem(&r, &c, pPackagedItem->m_idNum, pPackagedItem->m_plus, pPackagedItem->m_flag))
		{
			delete pPackagedItem;
			pPackagedItem = NULL;
			CItem* p = ch->m_invenNormal.GetItem(r, c);
			if (p)
			{
				GAMELOG << init("EVENT VALENTINE 2007 ITEM PACKAGE", ch)
						<< "PACKAGED ITEM INDEX" << delim
						<< p->m_itemProto->m_index << delim
						<< "PACKAGED ITEM COUNT" << delim
						<< p->Count() << delim
						<< itemlog(pPackagedItem)
						<< end;

				ItemUpdateMsg(itemmsg, p, 1);
				SEND_Q(itemmsg, ch->m_desc);
			}
		}
	}
	else
	{
		GAMELOG << init("EVENT VALENTINE 2007 ITEM PACKAGE", ch)
				<< itemlog(pPackagedItem)
				<< end;

		ItemAddMsg(itemmsg, pPackagedItem);
		SEND_Q(itemmsg, ch->m_desc);
		pPackagedItem = NULL;
	}

	// 포장한 카카오 아이템 제거
	DecreaseFromInventory(ch, pCacaoItem, 1);
	if (pCacaoItem->Count() > 0)
	{
		ItemUpdateMsg(rmsg, pCacaoItem, -1);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		ItemDeleteMsg(rmsg, pCacaoItem);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, pCacaoItem, true, true);
	}

	EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_PACKAGE_OK);
	SEND_Q(rmsg, ch->m_desc);
	return true;
}
#endif // #if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 

#ifdef EVENT_EGGS_HUNT_2007
bool do_ItemUse_2148(CPC* ch, const CItemProto* itemproto)
{
	CNetMsg rmsg, itemmsg;
// * 확률 체크 부분을 수정해야 함!!!!!!!!
// 아이템들의 확률이 총 100%로 되게끔... 

#ifdef EVENT_EGGS_HUNT_2008_ITEM	
	static const int giftcount = 5;
#ifdef LC_HBK
	static const int gifttable[giftcount][3] = {		// 확률, 아이템인덱스, 개수
		{1, 884, 3},			// 1.	3개
		{4, 971, 5},			// 2.	5개
		{36, 972, 10},			// 3.	10개
		{68, 1057, 10},			// 4.	10개
		{100, 723, 10},			// 4.	10개
	};
#else
	static const int gifttable[giftcount][3] = {		// 확률, 아이템인덱스, 개수
		{1, 85, 3},				// 1.	고급재련석				3개
		{4, 723, 5},			// 2.	문스톤					5개
		{36, 45, 10},			// 3.	대형 체력회복 물약		10개
		{68, 883, 10},			// 4.	숙련도 증폭제			10개
		{100, 885, 10},			// 4.	나스 증폭제				10개
	};
#endif // LC_HBK
#else
#if defined (LC_JPN)
	static const int giftcount = 12;
	static const int gifttable[giftcount][3] = {		// 확률, 아이템인덱스, 개수
		{20, 2403,	 1},			// 1.	폭죽
		{27, 159,	 3},			// 2.	C등급 원소
		{34, 154,	 3},			// 3.	스톤 웨버		
		{41, 198,	 3},			// 4.	크락의 줄기
		{56, 1309,	 1},			// 5.	작은 흰 벚꽃
		{71, 1308,	 1},			// 6.	작은 분홍 벚꽃
		{74, 786,	 1},			// 7.	부스터
		{84, 1311,	 1},			// 8.	흰 벚꽃
		{94, 1310,	 1},			// 9.	분홍 벚꽃
		{96, 85,	 1},			// 10.	고급제련석
		{98, 2499,	 1},			// 11.	경험의 스크롤
		{100, 2498,	 1},			// 12.	눈사람 인형
	};
#elif defined (LC_GER) || defined (LC_EUR)
	static const int giftcount = 13;
	static const int gifttable[giftcount][3] = {		// 확률, 아이템인덱스, 개수
		{  2,  85,	 1},
		{  5, 501,	 2},
		{  8,  499,	 2},
		{ 11,  500,	 2},
		{ 14,  498,	 2},
		{ 21, 1310,	 1},
		{ 28, 1311,	 1},
		{ 35, 1309,	 1},
		{ 42, 1308,	 1},
		{ 50,  723,	 2},
		{ 60,  786,	 2},
		{ 80,  485,	 2},
		{100,   44,	 2},
	};
#else
	static const int giftcount = 17;
	static const int gifttable[giftcount][3] = {		// 확률, 아이템인덱스, 개수
		{40, 19, 50},			// 1.	50 나스
		{49, 19, 500},			// 2.	500 나스
		{56, 154, 10},			// 3.	스톤 웨버		10개
		{63, 159, 10},			// 4.	C등급 원소		10개
		{70, 198, 10},			// 5.	크락의 줄기		10개
		{77, 391, 10},			// 6.	속도향상 물약	5개
		{83, 786, 1},			// 7.
		{86, 172, 1},			// 8.
		{89, 173, 1},			// 9.
		{91, 85, 1},			// 10.
		{93, 971, 1},			// 11.
		{95, 1417, 1},			// 12.
		{96, 1975, 1},			// 13.
		{97, 2045, 1},			// 14.
		{98, 1976, 1},			// 15.
		{99, 1287, 1},			// 16.
		{100, 19, 25000},		// 17.
	};
#endif // #ifdef LC_JPN
#endif // EVENT_EGGS_HUNT_2008_ITEM

	CItem* pEggItem;
	int row, col;
	if(ch->m_invenNormal.FindItem(&row, &col, 2148, -1, -1))
	{
		pEggItem = ch->m_invenNormal.GetItem(row, col);
	}
	else
	{
		// 인벤에서 찾지 못했다.
		EventEggsHunt2007ErrorMsg(rmsg, MSG_EVENT_EGGS_HUNT_2007_ERROR_NOITEM);
		SEND_Q(rmsg, ch->m_desc);
		return false;
	}
	
	if( ch->m_invenNormal.GetSpace() < 1  )
	{
		SysMsg( rmsg , MSG_SYS_FULL_INVENTORY );
		SEND_Q( rmsg, ch->m_desc);
		return false;
	}

	int nProb = GetRandom(1, 100);
	if(nProb < 1 || nProb > 100)
		return false;

	int i;
	for(i = 0; i < giftcount; ++i)
	{
		if(nProb <= gifttable[i][0])
		{
			// 498, 499, 500, 501 (붉은/푸른 넨/케르는 falg 하드코딩)
			int flag = 0;
			if (gifttable[i][1] >= 498 && gifttable[i][1] <= 501)
				flag = 12;

			CItem* pGiftItem = gserver.m_itemProtoList.CreateItem(gifttable[i][1], -1, 0, flag, gifttable[i][2]);
			if(!pGiftItem)
				return false;
			if(AddToInventory(ch, pGiftItem, false, true))
			{
				GAMELOG << init("EVENT EGGSHUNT GIFT ITEM", ch)
					<< itemlog(pGiftItem) << delim
					<< end;
				
				if(pGiftItem->tab() != -1)
				{
					ItemAddMsg(itemmsg, pGiftItem);
				}
				else
				{
					int r, c;
					if(ch->m_invenNormal.FindItem(&r, &c, pGiftItem->m_idNum, pGiftItem->m_plus, pGiftItem->m_flag))
					{
						CItem* pItem = ch->m_invenNormal.GetItem(r, c);
						ItemUpdateMsg(itemmsg, pItem, gifttable[i][2]);
						delete pGiftItem;
						pGiftItem = NULL;
					}
				}
				SEND_Q(itemmsg, ch->m_desc);
			}
			else
			{
				EventEggsHunt2007ErrorMsg(rmsg, MSG_EVENT_EGGS_HUNT_2007_ERROR_GIFT_FULLINVEN);
				SEND_Q(rmsg, ch->m_desc);

				delete pGiftItem;
				pGiftItem = NULL;
				return false;
			}

			EventEggsHunt2007GiftMsg(rmsg, gifttable[i][1], gifttable[i][2]);
			SEND_Q(rmsg, ch->m_desc);
			break;
		}
	}

	return true;
}
#endif // EVENT_EGGS_HUNT_2007

#ifdef ADULT_SERVER_NEWITEM
void do_ItemTarget(CPC* ch, CNetMsg& msg)
{
	CNetMsg effectMsg, rmsg;
	unsigned char subtype;
	char tab, row, col;
	int item_idx;
	char chartype;
	int charindex;

	msg.MoveFirst();

	msg >> subtype
		>> tab >> row >> col >> item_idx
		>> chartype
		>> charindex;

	CInventory* inven;
	CItem* item;
	const CItemProto* itemproto;

	inven = GET_INVENTORY(ch, tab);
	if (inven == NULL)
		return ;
	item = inven->GetItem(row, col);
	if (item == NULL || item->m_index != item_idx || item->m_wearPos != -1)
		return ;

	itemproto = item->m_itemProto;

	if (!itemproto || itemproto->m_typeIdx != ITYPE_ONCE || itemproto->m_subtypeIdx != IONCE_TARGET)
	{
		if( itemproto )
			GAMELOG << init("ERROR") << itemproto->m_typeIdx << itemproto->m_subtypeIdx << end;
		return ;
	}
		

	CCharacter* tch;
	CSkill* pSkill;
	bool bApply;
	int nRetApplySkill;

	tch = ch->m_pArea->FindCharInCell(ch, charindex, (MSG_CHAR_TYPE)chartype, false);
	if(!tch)
		return ;

	pSkill = gserver.m_skillProtoList.Create(itemproto->m_num0, itemproto->m_num1);
	if(!pSkill)
		return ;
	
	switch(itemproto->m_index)
	{
	case 2374:
		{
			if (tch->m_assist.FindByItemIndex(2374))
			{	
				SysCannotDuplcationMsg(rmsg, itemproto->m_index, 2374);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

//			if(ch->m_lastpulseCurse > gserver.m_pulse)
//				return ;
		}
		break;
//	case 2382:		// 절망의 부적
//		{
//			if(ch->m_lastpulseDespair > gserver.m_pulse)
//				return ;
//		}
//		break;
		
	}
	
	nRetApplySkill = ApplySkill(ch, tch, pSkill, itemproto->m_index, bApply);
	delete pSkill;
	pSkill = NULL;

	//if(nRetApplySkill != 0)
	//	return ;

	if(!bApply)
		return ;

	EffectItemMsg(effectMsg, ch, item);
	ch->m_pArea->SendToCell(effectMsg, ch, true);

	ItemUseMsg(rmsg, ch, tab, row, col, item_idx, 0);
	SEND_Q(rmsg, ch->m_desc);

// 	switch(itemproto->m_index)
// 	{
// 	case 2374:
// 		ch->m_lastpulseCurse = gserver.m_pulse + PULSE_REAL_SEC * 90;
// 		break;
// 	case 2382:
// 		ch->m_lastpulseDespair = gserver.m_pulse + PULSE_REAL_SEC * 10;
// 		break;
//	}

	DecreaseFromInventory(ch, item, 1);
	if (item->Count() > 0)
	{
		ItemUpdateMsg(rmsg, item, -1);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		ItemDeleteMsg(rmsg, item);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, item, true, true);
	}
}
#endif // ADULT_SERVER_NEWITEM

#ifdef EVENT_NEW_MOONSTONE
bool do_itemUse_MoonStoneBox(CPC* ch, const CItemProto* itemproto)
{
	CNetMsg rmsg, itemmsg;

	const int nMoonStoneGradeCount = 4;
	static const int MoonStoneGradeProb[nMoonStoneGradeCount][2] = {   // 아이템인덱스, 확률
		{2545, 520},						// 최하급문스톤		52
		{2546, 300},						// 하급문스톤		30
		{723, 150},						// 문스톤			15
		{2547, 30},							// 상급문스톤		3
	};

	CItem* MoonStoneBox;
	int row, col;
	if(!ch->m_invenNormal.FindItem(&row, &col, 2544, -1, -1))
		return false;

	MoonStoneBox = ch->m_invenNormal.GetItem(row, col);
	if(!MoonStoneBox)
		return false;

	int rand = GetRandom(1, 1000);
	int prob = 0;
	int i;
	for(i = 0; i < nMoonStoneGradeCount; ++i)
	{
		prob += MoonStoneGradeProb[i][1];
		if(rand <= prob)
		{
			CItem* MoonStone = gserver.m_itemProtoList.CreateItem(MoonStoneGradeProb[i][0], -1, 0, 0, 1);
			if(!MoonStone)
				return false;

			GAMELOG << init("MOONSTONE BOX OPEN", ch)
						<< "MOONSTONE" << itemlog(MoonStone) << end;

			EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTON_OK);
			rmsg << MoonStone->m_idNum;
			SEND_Q(rmsg, ch->m_desc);

			if(AddToInventory(ch, MoonStone, false, true))
			{
				if(MoonStone->tab() != -1)
				{
					ItemAddMsg(itemmsg, MoonStone);
				}
				else
				{
					int r,c;
					if(ch->m_invenNormal.FindItem(&r, &c, MoonStone->m_idNum, MoonStone->m_plus, MoonStone->m_flag))
					{
						CItem* item = ch->m_invenNormal.GetItem(r, c);
						ItemUpdateMsg(itemmsg, item, 1);
						delete MoonStone;
						MoonStone = NULL;
					}
				}

				SEND_Q(itemmsg, ch->m_desc);
			}
			else
			{
				ch->m_pArea->DropItem(MoonStone, ch);
				MoonStone->m_preferenceIndex = ch->m_index;
				ItemDropMsg(rmsg, ch, MoonStone);
				SEND_Q(rmsg, ch->m_desc);
			}
			break;
		}
	}

	if(i == nMoonStoneGradeCount)
		return false;
	return true;
}
#endif 

#ifdef CASH_EXCHANGE_EQUIP
void do_ItemUseExchagneEquip( CPC* ch, CNetMsg& msg )
{

	CNetMsg rmsg;

	char row = -1;
	char col = -1;
	int itemindex = -1;
	int changetype = -1;		// 무기는 서브타입, 방어구는 직업
	char row2 = -1;
	char col2 = -1;
	int useitemindex = -1;

	msg >> row
		>> col
		>> itemindex
		>> changetype
		>> row2
		>> col2
		>> useitemindex;


	CItem* item = ch->m_invenNormal.GetItem( row2, col2  );
	if( item == NULL )
	{
		ItemChangeWeaponRepMsg( rmsg, 0, -1 );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	CItem* olditem = ch->m_invenNormal.GetItem(row, col);
	if (olditem == NULL)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (olditem->m_index != itemindex)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (olditem->GetItemLevel(true) < 29)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (olditem->m_wearPos != WEARING_NONE)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 무기 방어구 모두 교환 가능하게 수정 : 051219 : bs
	if (!olditem->IsWeaponType() && (!olditem->IsArmorType() || olditem->m_itemProto->m_subtypeIdx == IWEAR_SHIELD))
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if ((olditem->IsWeaponType() && olditem->m_itemProto->m_subtypeIdx == changetype)
		|| (olditem->IsArmorType() && (olditem->m_itemProto->m_jobFlag & (1 << changetype))))
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 교환 가능 아이템 검사
	if (!(olditem->m_itemProto->m_flag & ITEM_FLAG_CHANGE) 
#ifdef PLATINUM
		 || olditem->IsUsedPlatinumSpecial()
#endif
		)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 대여 무기 검사
	if (olditem->IsLent())
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 교환시 plus 수치 보존 및 옵션 제거 : 051219 : bs
	int nNewPlus = olditem->m_plus;
	int nNewFlag = olditem->m_flag & ~FLAG_ITEM_OPTION_ENABLE;

	int newindex = -1;
	int i;
	for (i = 0; newindex == -1 && i < gserver.m_itemProtoList.m_nCount; i++)
	{
		// 메인 타입이 같고, 교환 타입이고, 레벨이 같고 교환 가능 아이템일때
		if (gserver.m_itemProtoList.m_protoItems[i].m_typeIdx == olditem->m_itemProto->m_typeIdx
			&& gserver.m_itemProtoList.m_protoItems[i].GetItemProtoLevel() == olditem->GetItemLevel(true)
			&& (gserver.m_itemProtoList.m_protoItems[i].m_flag & ITEM_FLAG_CHANGE))
		{
			if ((olditem->IsWeaponType() && gserver.m_itemProtoList.m_protoItems[i].m_subtypeIdx == changetype)
				|| (olditem->IsArmorType() && gserver.m_itemProtoList.m_protoItems[i].m_subtypeIdx == olditem->m_itemProto->m_subtypeIdx && (gserver.m_itemProtoList.m_protoItems[i].m_jobFlag & (1 << changetype))))
			newindex = gserver.m_itemProtoList.m_protoItems[i].m_index;
		}
	}

	if (newindex == -1)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CItem* newitem = gserver.m_itemProtoList.CreateItem(newindex, -1, nNewPlus, nNewFlag, 1);
	if (newitem == NULL)
	{
		ItemChangeWeaponRepMsg(rmsg, 0, -1);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("CASH_EXCHANGE_EQUIP", ch)
			<< "OLD ITEM" << delim
			<< itemlog(olditem) << delim
			<< "NEW ITEM" << delim
			<< itemlog(newitem)
			<< end;

	int olddb = olditem->m_itemProto->m_index;
	int newdb = newitem->m_itemProto->m_index;

	ItemDeleteMsg(rmsg, olditem);
	SEND_Q(rmsg, ch->m_desc);
	RemoveFromInventory(ch, olditem, true, true);

	AddToInventory(ch, newitem, false, false);
	ItemAddMsg(rmsg, newitem);
	SEND_Q(rmsg, ch->m_desc);
	
	DecreaseFromInventory(ch, item, 1);
	if (item->Count() != 0)
	{
		ItemUpdateMsg(rmsg, item, -1);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		ItemDeleteMsg(rmsg, item);
		SEND_Q(rmsg, ch->m_desc);
		RemoveFromInventory(ch, item, true, true);
	}

//	ItemDeleteMsg(rmsg, item);
//	SEND_Q(rmsg, ch->m_desc);
//	RemoveFromInventory(ch, item, true, true);

	ItemChangeWeaponRepMsg(rmsg, olddb, newdb);
	SEND_Q(rmsg, ch->m_desc);
}
#endif // CASH_EXCHANGE_EQUIP

// pet 색깔 변경하기
#ifdef PET_DIFFERENTIATION_ITEM
bool do_useCashItem_PetColorChange( CPC* ch, const CItemProto* itemproto )
{
	if( ch->m_wearing[WEARING_PET])
	{
		CPet* pet = ch->GetPet();
		if( !pet )
			return false;
		char PetColorType = 0 ;

		
		// 말만 가능한 것들
		if( (itemproto->m_index == PET_BLACK_HEART_COLOR_ITEM) ||
			(itemproto->m_index == PET_WHITE_HEART_COLOR_ITEM) ||
			(itemproto->m_index == PET_YELLOW_HEART_COLOR_ITEM) )
		{
			
			if( (pet->GetPetType() != PET_TYPE_HORSE) &&
				(pet->GetPetType() != PET_TYPE_BLUE_HORSE) &&
				(pet->GetPetType() != PET_TYPE_UNKOWN_HORSE) )
			{
				CNetMsg rmsg;
				CashItemMoonstoneStartRepMsg(rmsg, MSG_EX_CASHITEM_MOONSTONE_ERROR_CANTUSE_CASHMOON, -1);
				SEND_Q(rmsg, ch->m_desc);
				return false;
			}
		}

		switch( itemproto->m_index )
		{
		case PET_RED_COLOR_ITEM:
			PetColorType = (char)RED_COLOR;
			break;
		case PET_SCARLET_COLOR_ITEM:
			PetColorType = (char)SCARLET_COLOR;
			break;
		case PET_YELLOW_COLOR_ITEM:
			PetColorType = (char)YELLOW_COLOR;
			break;
		case PET_GREEN_COLOR_ITEM:
			PetColorType = (char)GREEN_COLOR;
			break;
		case PET_BLUE_COLOR_ITEM:
			PetColorType = (char)BLUE_COLOR;
			break;
		case PET_DEEP_BLUE_COLOR_ITEM:
			PetColorType = (char)DEEP_BLUE_COLOR;
			break;
		case PET_VIOLET_COLOR_ITEM:
			PetColorType = (char)VIOLET_COLOR;
			break;
//#if defined (LC_BRZ) || defined (LC_USA)  || defined (LC_MAL) || defined(LC_GER)
		case PET_BLACK_COLOR_ITEM:
			PetColorType = (char)BLACK_COLOR;
			break;
		case PET_WHITE_COLOR_ITEM:
			PetColorType = (char)WHITE_COLOR;
			break;
#ifdef NEW_CASHITEM_USA_2008
		case PET_NON_COLOR_ITEM:
			PetColorType = (char)NON_COLOR;
			break;
#endif //WJKTEST
//#endif // defined (LC_BRZ) || defined (LC_USA)  || defined (LC_MAL)
#ifdef EVENT_PHOENIX   // 피닉스 이벤트 
		// 피닉스의 결정
		case 3262:
			PetColorType = (char)PHOENIX_COLOR;
			break;
#endif
		case PET_BLACK_HEART_COLOR_ITEM : 
			PetColorType = (char) BLACK_HEART_COLOR;
			break;

		case PET_WHITE_HEART_COLOR_ITEM : 
			PetColorType = (char) WHITE_HEART_COLOR;
			break;			

		case PET_YELLOW_HEART_COLOR_ITEM : 
			PetColorType = (char) YELLOW_HEART_COLOR;
			break;
		}

		pet->SetPetColor( PetColorType );
	
		CNetMsg rmsg;
		HelperPetColorChange( rmsg, ch->m_index, pet->m_index, PetColorType );
		SEND_Q( rmsg, gserver.m_helper );

		CNetMsg ColorMsg;
		PetChangeColor( ColorMsg,  pet->m_index, PetColorType, ch->m_index, pet->GetPetTypeGrade()  );
		ch->m_pArea->SendToCell( ColorMsg, ch, true );
		return true;
	}
	return false;
}
#endif //PET_DIFFERENTIATION_ITEM

// 대전용 아이템
#ifdef COMPETITION_ITEM  
bool do_useCashItem_CompetitionItem( CPC* ch, CItem* pItem )
{
	CNetMsg rmsg;

	int r, c;
	bool bUsed = false;
	CInventory* inven = GET_INVENTORY( ch, INVENTORY_NORMAL );

// 광속 디버프
#if defined (LC_USA) || defined (LC_TLD)
	if (ch->m_cooltime_Competition > gserver.m_gameTime || ch->m_assist.FindBySkillIndex(590))
	{
		SkillErrorMsg(rmsg, MSG_SKILL_ERROR_COMPETITION, 590, -1);
		SEND_Q(rmsg, ch->m_desc);

		return false;
	}
#else
	if( ch->m_cooltime_Competition > gserver.m_gameTime )
	{			
		return false;
	}
#endif 

#if defined (LC_TLD)
	if(ch->m_assist.GetAssistHelpCount() >= (ch->m_assist.GetAssistHelpMax() - 1) )
		return false;
#endif // LC_TLD

	if( inven->FindItem( &r, &c, 19, -1 , -1 ) )		
	{

		CItem* pNas = inven->GetItem( r, c );

#if defined(LC_USA)
		if( pNas && pNas->Count() > (ch->m_level * (5 * USE_COMPETITION_ITEM_NAS)) )
		{
			DecreaseFromInventory( ch, pNas, ch->m_level * (5 * USE_COMPETITION_ITEM_NAS) );
			ItemUpdateMsg( rmsg, pNas, -ch->m_level * (5 * USE_COMPETITION_ITEM_NAS) );
			SEND_Q( rmsg, ch->m_desc );
			EffectEtcMsg(rmsg, ch, MGS_EFFECT_ETC_COMPETITION);
			ch->m_pArea->SendToCell(rmsg, ch, true);	 	

		}
		else
		{
			SysMsg(rmsg, MSG_SYS_SHORT_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return false;			
		}
#else		
		if( pNas && pNas->Count() > (ch->m_level * USE_COMPETITION_ITEM_NAS) )
		{
			DecreaseFromInventory( ch, pNas, ch->m_level * USE_COMPETITION_ITEM_NAS );
			ItemUpdateMsg( rmsg, pNas, -ch->m_level * USE_COMPETITION_ITEM_NAS );
			SEND_Q( rmsg, ch->m_desc );
			EffectEtcMsg(rmsg, ch, MGS_EFFECT_ETC_COMPETITION);
			ch->m_pArea->SendToCell(rmsg, ch, true);	 	

		}
		else
		{
			SysMsg(rmsg, MSG_SYS_SHORT_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return false;
			
		}
#endif
	}
	else 
	{
			SysMsg(rmsg, MSG_SYS_SHORT_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return false;
	}

	if( pItem->m_used == -1 )
	{		
		pItem->m_used = gserver.m_gameTime + pItem->m_itemProto->m_num3 * 60 * 60;
		bUsed = true;
	}

	if( pItem->m_used < gserver.m_gameTime )
		return false;

	if ( bUsed )
	{
		ItemUpdateMsg( rmsg, pItem, 0 );
		SEND_Q( rmsg, ch->m_desc );
	}

	CSkill* skill = NULL;
	skill = gserver.m_skillProtoList.Create( pItem->m_itemProto->m_num0, pItem->m_itemProto->m_num1 );
	if( skill == NULL )
		return false;

	bool bApply;
	ApplySkill( ch, ch, skill, pItem->m_itemProto->m_index, bApply );
	if( !bApply )
	{
		delete skill;
		return false;
	}

// 광속 디버프
#if defined (LC_USA) || defined (LC_TLD)
	CSkill* pDebuff = NULL;
	pDebuff = gserver.m_skillProtoList.Create(590);
	if( pDebuff == NULL )
		return false;

	bool bDebuffApply = false;
	ApplySkill( ch, ch, pDebuff, -1, bDebuffApply);

	if (!bDebuffApply)
	{
		delete pDebuff;
		return false;
	}
	delete pDebuff;
#endif

	delete skill;

	// 포션 사용 이펙트 메시지

	CNetMsg effectMsg;
	EffectItemMsg(effectMsg, ch, pItem);
	ch->m_pArea->SendToCell(effectMsg, ch, true);

	return true;
}

#endif // COMPETITION_ITEM


#ifdef PET_HPRECOVER_ITEM		// 펫 HP 회복 
bool do_useCashItem_PetHPRecover( CPC* ch, const CItemProto* itemproto )
{
	if( ch->m_wearing[WEARING_PET])
	{
		CPet* pet = ch->GetPet();
		if( !pet )
			return false;
		if( itemproto->m_index != PET_HPRECOVER_ITEM )
			return false;

		int upVal = 30;
		bool bUpdate = false;
		if( pet->m_hp < pet->m_maxHP )
		{
			pet->m_hp += upVal;
			if( pet->m_hp > pet->m_maxHP )
				pet->m_hp = pet->m_maxHP;
			bUpdate = true;
		}
		
		if( bUpdate )
		{
			CNetMsg rmsg;
			ExPetStatusMsg( rmsg, pet );
			SEND_Q( rmsg, ch->m_desc );
		}
	}
	else
	{
		return false;
	}
	return true;
}
#endif // PET_HPRECOVER_ITEM
//
//#ifdef USE_WAREHOUSE		// 창고 이용 주문서
//bool do_useCashItem_UseWarhouse( CPC* ch, const CItemProto* itemproto )
//{
//	if(  itemproto->m_index != USE_WAREHOUSE_ITEM1 && itemproto->m_index != USE_WAREHOUSE_ITEM2 && itemproto->m_index != USE_WAREHOUSE_ITEM3 )
//		return false;
//
//	return true;
//}
//#endif // USE_WAREHOUSE
//
//#ifdef USE_GROCERY		// 잡화상 이용 주문서
//bool do_useCashItem_UseGrocery( CPC* ch, const CItemProto* itemproto )
//{
//	if(  itemproto->m_index != USE_GROCERY_ITEM1 && itemproto->m_index != USE_GROCERY_ITEM2 && itemproto->m_index != USE_GROCERY_ITEM3 )
//		return false;
//
//	return true;
//}
//#endif // USE_GROCERY
	
//#ifdef SHOW_HP		// 써치 라이프 - 캐릭터의 HP를 볼수 있음
//bool do_useCashItem_ShowHP( CPC* ch, const CItemProto* itemproto )
//{
//	if( itemproto->m_index != SHOW_HP_ITEM1 && itemproto->m_index != SHOW_HP_ITEM2 && itemproto->m_index != SHOW_HP_ITEM3 )
//		return false;
//
//	CNetMsg rmsg;
///* 	char	cOn = 0; */
//	rmsg.Init(MSG_PK);
//	rmsg << (unsigned char)MSG_PK_TARGET_SHOWHP
//		 << (char)1 ;
//	SEND_Q( rmsg, ch->m_desc );
//
//	return true;
//}
//#endif // SHOW_HP

#if defined ( USE_WAREHOUSE ) || defined ( USE_GROCERY ) || defined ( SHOW_HP )
bool do_useCashItem_WareHouse_Grocery_ShowHP( CPC* ch, CItem* pItem )
{
	CNetMsg rmsg;

//	bool bUsed = false;
//
//	if( pItem->m_used == -1 )
//	{		
//		pItem->m_used = gserver.m_gameTime + 4 * 60;
////		pItem->m_used = gserver.m_gameTime + pItem->m_itemProto->m_num3 * 60 * 60;
//		bUsed = true;
//	}

	if( pItem->m_used < gserver.m_gameTime )
		return false;

//	if ( bUsed )
//	{
		ItemUpdateMsg( rmsg, pItem, 0 );
		SEND_Q( rmsg, ch->m_desc );

		if( pItem->m_itemProto->m_index == SHOW_HP_ITEM1 
			|| pItem->m_itemProto->m_index == SHOW_HP_ITEM2 
			|| pItem->m_itemProto->m_index == SHOW_HP_ITEM3 
			|| pItem->m_itemProto->m_index == 2606 )
		{
			rmsg.Init(MSG_PK);
			rmsg << (unsigned char)MSG_PK_TARGET_SHOWHP
				 << (char)1 ;
			SEND_Q( rmsg, ch->m_desc );

			CSkill* skill = NULL;
			skill = gserver.m_skillProtoList.Create( pItem->m_itemProto->m_num0, pItem->m_itemProto->m_num1 );
			if( skill == NULL )
				return false;

			bool bApply;
			ApplySkill( ch, ch, skill, pItem->m_itemProto->m_index, bApply );
			if( !bApply )
			{
				delete skill;
				return false;
			}
			delete skill;
		}
/* 	} */// 

	// 포션 사용 이펙트 메시지

	CNetMsg effectMsg;
	EffectItemMsg(effectMsg, ch, pItem);
	ch->m_pArea->SendToCell(effectMsg, ch, true);

	return true;

}
#endif // #if defined ( USE_WAREHOUSE ) || defined ( USE_GROCERY ) || defined ( SHOW_HP )

bool do_useEventItem_HalloweenCandy( CPC* ch, CItem* pItem )
{
	int nTasteSkill[5][2] = { {470, 25},{471,350},{472,300},{473,300},{474,25} }; 
	int nTasteIndex = 0 , idx = 0;

	int prob = GetRandom(0,1000) ;
	for(idx=0; idx<5; idx++)
	{
		prob -= nTasteSkill[idx][1];
		if( prob <= 0 )
		{
			nTasteIndex = idx;
				break;
		}					
	}

	// 스킬 적용
	CSkill* pSkill = NULL;
	pSkill = gserver.m_skillProtoList.Create( nTasteSkill[nTasteIndex][0] , 1 );
	if( pSkill == NULL )
		return false;

	bool bApply;
	ApplySkill( ch, ch, pSkill, -1 , bApply );
	if( !bApply )
	{
		delete pSkill;
		return false;
	}
	delete pSkill;

	CNetMsg rmsg;
	EffectItemMsg(rmsg, ch, pItem);
	ch->m_pArea->SendToCell(rmsg, ch, true);

	EventHalloween2007Msg( rmsg, MSG_EVENT_TASTE_CANDY,  nTasteIndex );
	SEND_Q( rmsg, ch->m_desc);

	int nPriceItemIndex = 0;
	if ( nTasteSkill[nTasteIndex][0] == 470 )
	{
		// 퀘스트 아이템 제공 2489(최악의맛 증표)
		nPriceItemIndex=2489;
	}
	else if ( nTasteSkill[nTasteIndex][0] == 474 ) 
	{
		// 퀘스트 아이템 제공 2488(최고의맛증표)
		nPriceItemIndex=2488;
	}

	if ( nPriceItemIndex > 0 )
	{
		CItem* pPriceItem = gserver.m_itemProtoList.CreateItem( nPriceItemIndex , -1, 0, 0, 1 );
		if( pPriceItem )
		{
			// 아이템 지급
			if( AddToInventory( ch, pPriceItem, true, true ) )
			{
				int nRow, nCol;
				if( pPriceItem->tab() < 0 )
				{
					delete pPriceItem;
					ch->m_invenEvent.FindItem( &nRow, &nCol, nPriceItemIndex, 0, 0 );
					pPriceItem =  ch->m_invenEvent.GetItem( nRow, nCol );
					ItemUpdateMsg( rmsg, pPriceItem, 1 );
					SEND_Q( rmsg, ch->m_desc );
				}
				else
				{
					ItemAddMsg( rmsg, pPriceItem );
					SEND_Q( rmsg, ch->m_desc );
				}
			}
		}
	}

	return true;
}





#ifdef USE_GROCERY
void do_GroceryItemBuy(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
	
	const int npcIdx  = 130;
	unsigned char subtype;
	int			i;
	int			itemCount = 0;
		
	LONGLONG	clientPrice;
	LONGLONG	serverPrice;
	
	CInventory* inven = NULL;
	
	int*		itemDBIdx = NULL;
	LONGLONG*	count = NULL;
	CItem**		item = NULL;
	
	try
	{
		msg >> subtype
			>> clientPrice
			>> itemCount;
		
		// 최대 살수 있는 아이템 종류의 수
		if (itemCount > MAX_ITEM_BUY || itemCount <= 0)
			throw 0;		// 메시지 내의 아이템 종류 수 오류
		
		// validation shop
		int zonenum = gserver.FindZone( ZONE_DRATAN );
		if( zonenum == -1  )
			throw 1;
		CZone* pZone = gserver.m_zones + zonenum;
		CShop* shop;
		if (!(shop = pZone->FindShop(npcIdx)))
			throw 1;		// 상점이 없음			

		itemDBIdx = new int[itemCount];
		count = new LONGLONG[itemCount];
		

		for(i=0; i < itemCount; i++)
		{
			msg >> itemDBIdx[i]
				>> count[i];

			// 상점에 아이템이 있는지 검사 throw 6
			if (!shop->ThersIsItem(itemDBIdx[i]))
				throw 6;		// 상점에 아이템 없음
		}

		// 누적합계
		serverPrice = 0;
		item = new CItem*[itemCount];
		memset(item, 0, sizeof(CItem*) * itemCount);

		for (i=0; i < itemCount; i++)
		{
			item[i] = gserver.m_itemProtoList.CreateItem(itemDBIdx[i], WEARING_NONE, 0, 0, count[i]);
			
			if (!item[i])
				throw 2;		// DB에 없는 아이템

			// 아이템 명성치 검사
			if (item[i]->m_itemProto->m_fame != -1)
			{
				// 명성치 딸린다
				if (ch->m_fame < item[i]->m_itemProto->m_fame)
					throw 9;
			}
			// 들어갈 인벤토리 결정
			inven = GET_INVENTORY(ch, GET_TAB(item[i]->m_itemProto->m_typeIdx, item[i]->m_itemProto->m_subtypeIdx));
			if (!inven)
				throw 3;		// DB에 잘못된 값
			
			// 무조건 인벤에 일단 추가
			if (!AddToInventory(ch, item[i], false, true))
			{
				delete item[i];
				item[i] = NULL;
				throw 4;		// 인벤토리 가득 참
			}
			serverPrice += (item[i]->m_itemProto->m_price * shop->m_buyRate / 100) * count[i];
		}
		
		// 합계가 다르다! --> 
		if (serverPrice != clientPrice)
			throw 5;		// 클라이언트 데이터 오류 : 가격 틀림
		
		// 돈 메세지
		CNetMsg moneyMsg;
		
		// 돈검사
		if (ch->m_moneyItem)
		{
			if ((ch->m_moneyItem->Count() > serverPrice))
			{
				ch->AddMoney(-serverPrice);
				ItemUpdateMsg(moneyMsg, ch->m_moneyItem, -serverPrice);
			}
			else if ((ch->m_moneyItem->Count() == serverPrice))
			{
				ItemDeleteMsg(moneyMsg, ch->m_moneyItem);
				ch->AddMoney(-serverPrice);
			}
			else
				throw 8;	// 돈 모질람
		}
		else
			throw 8;		// 돈 모질람
		
		for (i=0; i < itemCount; i++)
		{
			if (item[i])
			{
				// Log
				CNetMsg itemmsg;
				GAMELOG << init("ITEM_BUY", ch)
					<< npcIdx << delim
					<< itemlog(item[i]) << delim
					<< serverPrice << delim
					<< ((ch->m_moneyItem) ? ch->m_moneyItem->Count() : 0)
					<< end;
				
				// 인벤안에 셋팅되어 있으면 Add
				if (item[i]->tab() != -1)
				{
					ItemAddMsg(itemmsg, item[i]);
					item[i] = NULL;
				}

				// 인벤에 셋팅되어있지 않으면 Update
				else
				{
					int row, col;
					if (inven->FindItem(&row, &col, item[i]->m_idNum, item[i]->m_plus, item[i]->m_flag))
					{
						CItem* tItem = inven->GetItem(row, col);
						
						if (tItem)
							ItemUpdateMsg(itemmsg, tItem, count[i]);
						delete item[i];
						item[i] = NULL;
					}
				}
				
				SEND_Q(itemmsg, ch->m_desc);
			}
		}

#ifdef ENABLE_STATISTICS
		STATISTICS(ItemBuy, serverPrice);
#endif
		SEND_Q(moneyMsg, ch->m_desc);
	}
	catch (const int errCode)
	{
		// 오류 발생시 만들었던 아이템 제거
		if (item)
		{
			for (i=0; i < itemCount; i++)
			{
				if (item[i])
				{
					inven = GET_INVENTORY(ch, GET_TAB(item[i]->m_itemProto->m_typeIdx, item[i]->m_itemProto->m_subtypeIdx));
					
					// 인벤에 제대로 못들어간 넘이면 delete
					// 					if (item[i]->tab() == -1)
					// RemoveFromInventory(ch, item[i], true, false);
					// 					else
					// 						delete item[i];

					if(item[i]->tab() == -1)		// 기존에 추가했던 것
					{
						int row, col;
						if(ch->m_invenNormal.FindItem(&row, &col, item[i]->m_idNum, item[i]->m_plus, item[i]->m_flag))
						{
							CItem* pSelectItem = ch->m_invenNormal.GetItem(row, col);
							if(pSelectItem)
							{
								DecreaseFromInventory(ch, pSelectItem, count[i]);
							}
						}
					}
					else							// 새롭게 추가했던 것
					{
						RemoveFromInventory(ch, item[i], true, false);
					}
				}
			}
		}
		
		// 오류 메시지 보내기
		CNetMsg errmsg;
		
		switch (errCode)
		{
		case 0:			// 메시지 내의 아이템 종류 수 오류
		case 1:			// 상점이 없음
		case 2:			// DB에 없는 아이템
		case 3:			// DB에 잘못된 값
			SysMsg(errmsg, MSG_SYS_SHOP_CANCEL);
			break;
		case 4:			// 인벤토리 가득 참
			SysFullInventoryMsg(errmsg, (char)inven->m_tab);
			break;
		case 5:			// 클라이언트 데이터 오류 : 가격 틀림
		case 6:			// 상점에 아이템 없음
		case 7:			// 돈 빼기 실패
			SysMsg(errmsg, MSG_SYS_SHOP_CANCEL);
			break;
		case 8:			// 돈 모질람
			SysMsg(errmsg, MSG_SYS_SHORT_MONEY);
			break;

		case 9:			// 명성 부족
			SysMsg(errmsg, MSG_SYS_SHORT_FAME);
			break;
		}
		
		SEND_Q(errmsg, ch->m_desc);
	}
	
	if (itemDBIdx)
		delete [] itemDBIdx;
	if (count)
		delete [] count;
	if (item)
		delete [] item;
}

// Buy & Sell 모두 PC위주로 구성
// Player가 팔때 Item Sell Msg, Sell Rate 적용
void do_GroceryItemSell(CPC* ch, CNetMsg& msg)
{
	msg.MoveFirst();
	
	const int npcIdx  = 130;
	unsigned char subtype;
	int			i;
	int			itemCount;
	
	LONGLONG	clientPrice;
	
	char*		tab = NULL;
	char*		row = NULL;
	char*		col = NULL;
	LONGLONG*	count = NULL;
	
	CItem**		item = NULL;
	LONGLONG	serverPrice;
	CInventory*	inven = NULL;
	CNetMsg		itemmsg;
	
	try
	{
		msg >> subtype
			>> clientPrice
			>> itemCount;
		
		// 최대 팔수 있는 아이템 종류의 수
		if (itemCount > MAX_ITEM_SELL || itemCount <= 0)
			throw 0;		// 메시지 내의 아이템 종류 수 오류
		
		tab = new char[itemCount];
		row = new char[itemCount];
		col = new char[itemCount];
		count = new LONGLONG[itemCount];
		
		for(i=0; i < itemCount; i++)
		{
			tab[i] = -1;
			row[i] = -1;
			col[i] = -1;
			count[i] = 0;

			msg >> tab[i]
				>> row[i]
				>> col[i]
				>> count[i];

			for (int j = 0; j < i; j++)
			{
				if (tab[i] == tab[j] && row[i] == row[j] && col[i] == col[j])
					throw 2;
			}
		}
		
		// validation shop
		// validation shop
		int zonenum = gserver.FindZone( ZONE_DRATAN );
		if( zonenum == -1  )
			throw 1;
		CZone* pZone = gserver.m_zones + zonenum;
		CShop* shop;
		if (!(shop = pZone->FindShop(npcIdx)))
			throw 1;		// 상점이 없음
		
		serverPrice = 0;
		item = new CItem*[itemCount];
		memset(item, 0, sizeof(CItem*) * itemCount);
		
		// 판매 아이템 만들기
		for (i=0; i < itemCount; i++)
		{
			inven = GET_INVENTORY(ch, tab[i]);

			if (!inven)
				throw 2;
			
			if (!(item[i] = inven->GetItem(row[i], col[i])))
				throw 2;	// 인벤에 없는 아이템

			// 실제 아이템의 갯수가 클라이언트에서 팔려던 갯수보다 작다.
			if (item[i]->Count() < count[i])
				throw 2;
			
			// 입고 있거나 상점 불가 아이템인 경우
			if (!item[i]->CanSellToShop())
				throw 6;
			
			serverPrice += (item[i]->m_itemProto->m_price * shop->m_sellRate / 100) * count[i];
		}
		
		// 합계가 다르다! --> 
		if (serverPrice != clientPrice)
			throw 3;		// 클라이언트 데이터 오류 : 가격 틀림
		
		bool bHaveMoney = false;	// 돈이 아예 없었을 경우 false
		if (ch->m_moneyItem)
			bHaveMoney = true;
		
		// 돈 더하기
		if (!ch->AddMoney(serverPrice))
			throw 4;		// 돈 넣기 실패
		
		// 인벤에서 파는 아이템 완전 제거
		for (i=0; i < itemCount; i++)
		{
			// Log
			GAMELOG << init("ITEM_SELL", ch)
					<< npcIdx << delim
					<< itemlog(item[i]) << delim
					<< serverPrice << delim
					<< ((ch->m_moneyItem) ? ch->m_moneyItem->Count() : 0)
					<< end;
			
			bool bCountable = false;
			// 겹치는 아이템이면 수량 변화 안 겹치면 제거
			if (item[i]->m_itemProto->m_flag & ITEM_FLAG_COUNT)
			{
				bCountable = true;
				CInventory* tinven = GET_INVENTORY(ch, item[i]->tab());
				if (tinven)
				{
					DecreaseFromInventory(ch, item[i], count[i]);
					if (item[i]->Count() < 1)
					{
						ItemDeleteMsg(itemmsg, item[i]);
						if (!RemoveFromInventory(ch, item[i], true, true))
							throw 5;	// 인벤토리 제거 실패
					}
					else
						ItemUpdateMsg(itemmsg, item[i], -count[i]);
				}
			}
			else
			{
				ItemDeleteMsg(itemmsg, item[i]);
				if (!RemoveFromInventory(ch, item[i], true, true))
					throw 5;	// 인벤토리 제거 실패
			}
			
			SEND_Q(itemmsg, ch->m_desc);
		}

#ifdef ENABLE_STATISTICS
		STATISTICS(ItemSell, serverPrice * 3 / 2);
#endif
		
		// 돈 늘어난거 보내기
		CNetMsg moneyMsg;
		if (bHaveMoney && ch->m_moneyItem)
			ItemUpdateMsg(moneyMsg, ch->m_moneyItem, serverPrice);	// 원래 돈 있으면 update
		else if (ch->m_moneyItem)
			ItemAddMsg(moneyMsg, ch->m_moneyItem);		// 원래 돈 없으면 add
		
		SEND_Q(moneyMsg, ch->m_desc);
	}
	catch (const int errCode)
	{
		// 오류 메시지 보내기
		CNetMsg errmsg;
		
		switch (errCode)
		{
		case 0:			// 메시지 내의 아이템 종류 수 오류
		case 1:			// 상점이 없음
		case 2:			// 인벤에 없는 물건
		case 3:			// 클라이언트 데이터 오류 : 가격 틀림
		case 4:			// 돈 넣기 실패
		case 5:			// 인벤에서 제거 실패
		case 6:			// 팔수 없는 아이템
			SysMsg(errmsg, MSG_SYS_SHOP_CANCEL);
			break;
		}
		
		SEND_Q(errmsg, ch->m_desc);
	}
	
	if (tab)
		delete [] tab;
	if (row)
		delete [] row;
	if (col)
		delete [] col;
	if (count)
		delete [] count;
	if (item)
		delete [] item;
	
}
#endif USE_GROCERY


#ifdef ALTERNATE_MERCHANT
bool do_useCashItem_AlternateMerchant( CPC* ch, CItem* pItem )
{
/*	
	// 아이템 사용시 체크 사항
	// 아이템 중복 사용 안됨
	// 아이템 사용 시간 체크	
*/
	CNetMsg		rmsg;
	if( pItem->m_used < gserver.m_gameTime )
	{
		// 사용 시간 만료
		return false;

	}

	switch( pItem->m_itemProto->m_index )
	{
	case 2565:
		if( ch->m_assist.FindByItemIndex( 2566 ) || ch->m_assist.FindByItemIndex( 2567 ) )
		{
			SysCannotDuplcationMsg(rmsg, pItem->m_itemProto->m_index, 2565);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}
		break;
	case 2566:
		if( ch->m_assist.FindByItemIndex( 2565 ) || ch->m_assist.FindByItemIndex( 2567 ) )
		{
			SysCannotDuplcationMsg(rmsg, pItem->m_itemProto->m_index, 2566);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}
		break;
	case 2567:
		if( ch->m_assist.FindByItemIndex( 2565 ) || ch->m_assist.FindByItemIndex( 2566 ) )
		{
			SysCannotDuplcationMsg(rmsg, pItem->m_itemProto->m_index, 2567);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}
		break;
	}
	return true;
}
#endif // ALTERNATE_MERCHANT

#ifdef PET_TURNTO_NPC
bool do_useCashItem_PetTurnToNPC( CPC* ch, const CItemProto* itemproto )
{
	// 사용조건 검사
	// 착용하고 있는 펫에 적용
	// 펫 레벨이 16 이상의 말이고, 탈수 없을때
	if( ch->m_wearing[WEARING_PET])
	{
		CNetMsg rmsg;
		CPet* pet = ch->GetPet();
		if( !pet )
			return false;

		int toNpc = -1 ;
		int	toNpcSize = 10;

		if( !pet->IsMountType() )
		{
			if( pet->GetPetTurnToNpc() == 0 ) // 변신중이 아니다
			{
				// 아이템이 말타입(1)이고 펫이 말이거나 아이템이 용타입(1)이고 펫이 용이거나 ,아이템 타입이 모두(0)일때 
				if( pet->m_level > 15 && 
					( (itemproto->m_num4 == 1 && ( pet->GetPetType() & (0x10)) > 0 )
					  || (itemproto->m_num4 == 2 && ( pet->GetPetType() & (0x00)) > 0 )
					  || itemproto->m_num4 == 0 
					)
				  )
				{
					toNpc = itemproto->m_num0;	// 변신 NPC 인덱스
					toNpcSize = itemproto->m_num1; // 변신 npc size
				}
			}
			else	// 변신중이면 변신 해제만 가능
			{
				if( itemproto->m_index == 2605 )	// 변신 해제 아이템
					toNpc = 0;
			}

			if( toNpc > -1 )
			{
#if defined (EVENT_XMAS_2007) && defined (LC_KOR)
				if( toNpc > 0 )		// 변신 성공
				{
					int	nEventItemIndex = 2360;		// 펫의 명찰
					CItem* pEventItem =  gserver.m_itemProtoList.CreateItem( nEventItemIndex, -1, 0, 0, 1 );
					if( !pEventItem )
					{	
						SysMsg(rmsg, MSG_SYS_OVER_WEIGHT);
						SEND_Q(rmsg, ch->m_desc);
						return false;
					}

					// 아이템 지급
					if( AddToInventory( ch, pEventItem, true, true ) )
					{
						if( pEventItem->tab() < 0 )
						{
							delete pEventItem;
							int nRow, nCol;
							ch->m_invenNormal.FindItem( &nRow, &nCol, nEventItemIndex, 0, 0 );
							pEventItem =  ch->m_invenNormal.GetItem( nRow, nCol );
							ItemUpdateMsg( rmsg, pEventItem, 1 );
							SEND_Q( rmsg, ch->m_desc );
						}
						else
						{
							ItemAddMsg( rmsg, pEventItem );
							SEND_Q( rmsg, ch->m_desc );
						}
					}
					else // 실패
					{
						SysMsg(rmsg, MSG_SYS_OVER_WEIGHT);
						SEND_Q(rmsg, ch->m_desc);
						return false;
					}
				}
#endif //EVENT_XMAS_2007
				pet->SetPetTurnToNpc( toNpc );
#ifdef PET_TURNTO_NPC_ITEM
				pet->SetPetTurnToNpcSize(toNpcSize);
#endif //PET_TURNTO_NPC_ITEM

				HelperPetTurnToNPCMsg( rmsg, ch->m_index, pet->m_index, toNpc , toNpcSize );
				SEND_Q( rmsg, gserver.m_helper );
				
				PetTurnToNPCMsg( rmsg,  pet->m_index, toNpc, ch->m_index , toNpcSize );
				//SEND_Q( rmsg, ch->m_desc );
				ch->m_pArea->SendToCell( rmsg, ch, true );
				return true;
			}
		}
		//else
		//{
			// 조건에 맞지 않음
		//}
	}
	return false;
}
#endif //PET_TURNTO_NPC

#if defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)
bool do_ItemUse_SakuraBox( CPC * pc, const CItemProto * pItemProto, int nExtra1 )
{
	// 보상아이템 제공
	CNetMsg rmsg;

#ifdef LC_HBK
	int nEventItem[10][3] = { // ItemIndex, Count, Percent
		{ 85, 1, 5}, { 971, 2, 15	}, { 973, 5, 30}, { 884, 2, 50 }, { 786, 1, 50 }, { 1310, 1, 100 }, 
		{ 1311, 1, 100 }, { 1308, 1, 200 }, { 1309, 1, 200 }, { 391, 1, 250 }
	};

#elif defined (EVENT_NEW_MOONSTONE)
	int nEventItem[10][3] = { // ItemIndex, Count, Percent
		{ 85, 1, 5}, { 2548, 2, 15	}, { 2547, 5, 30}, { 884, 2, 50 }, { 786, 1, 50 }, { 1310, 1, 100 }, 
		{ 1311, 1, 100 }, { 1308, 1, 200 }, { 1309, 1, 200 }, { 391, 1, 250 }
	};
#else
	int nEventItem[10][3] = { // ItemIndex, Count, Percent
		{ 85, 1, 5}, { 673, 2, 15	}, { 672, 5, 30}, { 884, 2, 50 }, { 786, 1, 50 }, { 1310, 1, 100 }, 
		{ 1311, 1, 100 }, { 1308, 1, 200 }, { 1309, 1, 200 }, { 391, 1, 250 }
	};
#endif // EVENT_NEW_MOONSTONE

	int prob = GetRandom(0, 1000 );
	int nProbIndex = 0;
	do
	{
		prob -= nEventItem[nProbIndex][2];
		nProbIndex++;
	} while( prob > 0 );
	nProbIndex--;

	if( nProbIndex < 0 || nProbIndex > 9 )
		return false;

	CItem* pEventItem =  gserver.m_itemProtoList.CreateItem( nEventItem[nProbIndex][0], -1, 0, 0, nEventItem[nProbIndex][1] );
	if( !pEventItem )
	{	
		SysMsg(rmsg, MSG_SYS_OVER_WEIGHT);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}
	
	int nRow, nCol;
	if( pc->m_invenNormal.GetSpace() < 1 )
	{
		SysMsg(rmsg, MSG_SYS_FULL_INVENTORY);
		SEND_Q(rmsg, pc->m_desc);
		return false;		
	}

	// 아이템 지급
	if( AddToInventory( pc, pEventItem, true, true ) )
	{
		if( pEventItem->tab() < 0 )
		{
			delete pEventItem;
			nRow = nCol = 0;
			pc->m_invenNormal.FindItem( &nRow, &nCol, nEventItem[nProbIndex][0], 0, 0 );
			pEventItem =  pc->m_invenNormal.GetItem( nRow, nCol );
			ItemUpdateMsg( rmsg, pEventItem, nEventItem[nProbIndex][1] );
			SEND_Q( rmsg, pc->m_desc );
		}
		else
		{
			ItemAddMsg( rmsg, pEventItem );
			SEND_Q( rmsg, pc->m_desc );
		}

		GAMELOG << init( "USE SAKURA EVENT ITEM BOX", pc ) << itemlog(pEventItem) << end;
#ifdef EVENT_NEW_MOONSTONE
		if( nEventItem[nProbIndex][0] == 85 || nEventItem[nProbIndex][0] == 2548 )	// 고제거나 , 최상급 문스톤 이면 서버에 알림
#else
		if( nEventItem[nProbIndex][0] == 85 || nEventItem[nProbIndex][0] == 673 )	// 고제거나 , 상급경험의결정 이면 서버에 알림
#endif // EVENT_NEW_MOONSTONE
		{
			EventSakuraEvent2008Msg( rmsg, MSG_EVNET_SAKURAEVENT_2008_ITEMCHANGE );
			rmsg << pc->m_nick << nEventItem[nProbIndex][0];
			gserver.m_playerList.SendToAll( rmsg );
		}
		return true;
		
	}
	else // 실패
	{
		SysMsg(rmsg, MSG_SYS_OVER_WEIGHT);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}

	return true;
}
#endif //defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)


#ifdef NEW_CASHITEM_USA_2008
void do_itemUse_WarpDoc(CPC* ch, CNetMsg& msg)
{
	int item_idx, zone, extra;
	int row, col, i;
	CNetMsg rmsg;
	CItem* item;
	CZone* pZone;
	unsigned char subtype;

	msg.MoveFirst();

	msg >> subtype >> item_idx >> zone >> extra;

	if(!ch->m_invenNormal.FindItem(&row, &col, item_idx))
		return;

	item = ch->m_invenNormal.GetItem(row, col);
	
	i = gserver.FindZone(zone);
	if(i == -1)
		return;

	pZone = gserver.m_zones + i;
	if (extra < 0 || extra >= pZone->m_countZonePos)
		return;

	GoZone(ch, zone, pZone->m_zonePos[extra][0], 
		GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		
		GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		


	if(ch->m_pZone->m_index == zone)
	{
		GAMELOG << init("CASH_ITEM_USE", ch)
				<< itemlog(item) << delim
				<< end;

		ItemUseMsg(rmsg, ch, 0, row, col, item_idx, 0);
		SEND_Q(rmsg, ch->m_desc);
		
		DecreaseFromInventory(ch, item, 1);
		if (item->Count() > 0)
		{
			ItemUpdateMsg(rmsg, item, -1);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, item);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, item, true, true);
		}
	}
}
#endif // NEW_CASHITEM_USA_2008


#ifdef LAKA_PRESSCORPS
void do_itemUse_PressCorps(CPC* ch, CNetMsg& msg)
{
	int item_idx;
	int row, col;
	CNetMsg rmsg;
	unsigned char subtype;
	CLCString str(1000);

	msg.MoveFirst();

	msg >> subtype >> item_idx >> str;
	if(!ch->m_invenNormal.FindItem(&row, &col, item_idx))
		return ;

	if(ch->m_bPressCorps)
	{
		SayMsg(rmsg, MSG_CHAT_NOTICE_PRESSCORPS, 0, "", "", str);
		gserver.m_playerList.SendToAll(rmsg);
	}
}
#endif // LAKA_PRESSCORPS

bool do_ItemUse_2887(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_invenNormal.GetSpace() < 1)
	{	// 인벤에 공간이 없으면 실패 
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	CNetMsg rmsg;

	const int nTableSize	= 7;

	int nTable[nTableSize][3]	= {
		{ 884, 20, 1},		
		{ 2141, 15, 1},		
		{ 1315, 15, 1},	
		{ 882, 15, 1},	
		{ 2356, 15, 1},		
		{ 972, 15, 1},		
		{ 1839, 5, 1},		
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver.m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;
	if (!AddToInventory(ch, pItem, true, true))
	{
		delete pItem;
		pItem = NULL;
		return false;
	}
	if (pItem->tab() < 0)
	{
		int nRow = -1, nCol = -1;
		ch->m_invenNormal.FindItem(&nRow, &nCol, pItem->m_itemProto->m_index, 0, 0);
		CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pPrevItem)
		{
			GAMELOG << init("USE TOWER BOX", ch)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< "TOTAL" << delim
					<< itemlog(pPrevItem)
					<< end;
			ItemUpdateMsg(rmsg, pPrevItem, nTable[nIndex][2]);
			SEND_Q(rmsg, ch->m_desc);
			delete pItem;
			pItem = NULL;
		}
	}
	else
	{
		GAMELOG << init("USE TOWER BOX", ch)
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< "TOTAL" << delim
				<< itemlog(pItem)
				<< end;
		ItemAddMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);
		pItem = NULL;
	}

	SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->m_index);
	SEND_Q(rmsg, ch->m_desc);

	return true;
}

bool do_ItemUse_2888(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_invenNormal.GetSpace() < 1)
	{	// 인벤에 공간이 없으면 실패 
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	CNetMsg rmsg;

	const int nTableSize	= 8;

	int nTable[nTableSize][3]	= {
		{ 671, 15, 1},		
		{ 672, 20, 1},		
		{ 673, 5, 1},	
		{ 676, 15, 5},	
		{ 844, 10, 1},		
		{ 846, 10, 1},		
		{ 674, 10, 1},		
		{ 845, 15, 1},			
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver.m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;
	if (!AddToInventory(ch, pItem, true, true))
	{
		delete pItem;
		pItem = NULL;
		return false;
	}
	if (pItem->tab() < 0)
	{
		int nRow = -1, nCol = -1;
		ch->m_invenNormal.FindItem(&nRow, &nCol, pItem->m_itemProto->m_index, 0, 0);
		CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pPrevItem)
		{
			GAMELOG << init("USE RESTORE BOX", ch)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< "TOTAL" << delim
					<< itemlog(pPrevItem)
					<< end;
			ItemUpdateMsg(rmsg, pPrevItem, nTable[nIndex][2]);
			SEND_Q(rmsg, ch->m_desc);
			delete pItem;
			pItem = NULL;
		}
	}
	else
	{
		GAMELOG << init("USE RESTORE BOX", ch)
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< "TOTAL" << delim
				<< itemlog(pItem)
				<< end;
		ItemAddMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);
		pItem = NULL;
	}

	SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->m_index);
	SEND_Q(rmsg, ch->m_desc);

	return true;
}

bool do_ItemUse_2889(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_invenNormal.GetSpace() < 1)
	{	// 인벤에 공간이 없으면 실패 
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	CNetMsg rmsg;

	const int nTableSize	= 10;

	int nTable[nTableSize][3]	= {
		{ 1840, 10, 1},		
		{ 1841, 10, 1},		
		{ 1842, 10, 1},		
		{ 1416, 10, 1},		
		{ 1288, 10, 1},		
		{ 1285, 15, 1},		
		{ 1286, 14, 1},		
		{ 1287, 1, 1},		
		{ 971, 10, 1},		
		{ 882, 10, 5},		
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver.m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;
	if (!AddToInventory(ch, pItem, true, true))
	{
		delete pItem;
		pItem = NULL;
		return false;
	}
	if (pItem->tab() < 0)
	{
		int nRow = -1, nCol = -1;
		ch->m_invenNormal.FindItem(&nRow, &nCol, pItem->m_itemProto->m_index, 0, 0);
		CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pPrevItem)
		{
			GAMELOG << init("USE EXP BOX", ch)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< "TOTAL" << delim
					<< itemlog(pPrevItem)
					<< end;
			ItemUpdateMsg(rmsg, pPrevItem, nTable[nIndex][2]);
			SEND_Q(rmsg, ch->m_desc);
			delete pItem;
			pItem = NULL;
		}
	}
	else
	{
		GAMELOG << init("USE EXP BOX", ch)
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< "TOTAL" << delim
				<< itemlog(pItem)
				<< end;
		ItemAddMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);
		pItem = NULL;
	}

	SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->m_index);
	SEND_Q(rmsg, ch->m_desc);

	return true;
}

bool do_ItemUse_2890(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_invenNormal.GetSpace() < 1)
	{	// 인벤에 공간이 없으면 실패 
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	CNetMsg rmsg;

	const int nTableSize	= 5;

	int nTable[nTableSize][3]	= {
		{ 883, 20, 5},	
		{ 2452, 20, 1},		
		{ 2453, 20, 1},	
		{ 973, 20, 1},	
		{ 2139, 20, 1},		
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver.m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;
	if (!AddToInventory(ch, pItem, true, true))
	{
		delete pItem;
		pItem = NULL;
		return false;
	}
	if (pItem->tab() < 0)
	{
		int nRow = -1, nCol = -1;
		ch->m_invenNormal.FindItem(&nRow, &nCol, pItem->m_itemProto->m_index, 0, 0);
		CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pPrevItem)
		{
			GAMELOG << init("USE SKILL BOX", ch)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< "TOTAL" << delim
					<< itemlog(pPrevItem)
					<< end;
			ItemUpdateMsg(rmsg, pPrevItem, nTable[nIndex][2]);
			SEND_Q(rmsg, ch->m_desc);
			delete pItem;
			pItem = NULL;
		}
	}
	else
	{
		GAMELOG << init("USE SKILL BOX", ch)
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< "TOTAL" << delim
				<< itemlog(pItem)
				<< end;
		ItemAddMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);
		pItem = NULL;
	}

	SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->m_index);
	SEND_Q(rmsg, ch->m_desc);

	return true;
}	

bool do_ItemUse_2891(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_invenNormal.GetSpace() < 1)
	{	// 인벤에 공간이 없으면 실패 
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	CNetMsg rmsg;

	const int nTableSize	= 6;

	int nTable[nTableSize][3]	= {
		{ 840, 17, 1},		
		{ 841, 17, 1},		
		{ 1300, 15, 1},
		{ 2460, 17, 1},	
		{ 2463, 17, 1},		
		{ 2457, 17, 1},		
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver.m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;
	if (!AddToInventory(ch, pItem, true, true))
	{
		delete pItem;
		pItem = NULL;
		return false;
	}
	if (pItem->tab() < 0)
	{
		int nRow = -1, nCol = -1;
		ch->m_invenNormal.FindItem(&nRow, &nCol, pItem->m_itemProto->m_index, 0, 0);
		CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pPrevItem)
		{
			GAMELOG << init("USE 30DAYS BOX", ch)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< "TOTAL" << delim
					<< itemlog(pPrevItem)
					<< end;
			ItemUpdateMsg(rmsg, pPrevItem, nTable[nIndex][2]);
			SEND_Q(rmsg, ch->m_desc);
			delete pItem;
			pItem = NULL;
		}
	}
	else
	{
		GAMELOG << init("USE 30DAYS BOX", ch)
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< "TOTAL" << delim
				<< itemlog(pItem)
				<< end;
		ItemAddMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);
		pItem = NULL;
	}

	SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->m_index);
	SEND_Q(rmsg, ch->m_desc);

	return true;
}

bool do_ItemUse_2892(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_invenNormal.GetSpace() < 1)
	{	// 인벤에 공간이 없으면 실패 
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	CNetMsg rmsg;

	const int nTableSize	= 8;

	int nTable[nTableSize][3]	= {
		{ 1420, 20, 5},		
		{ 1418, 20, 5},		
		{ 2361, 20, 2},	
		{ 85, 8, 1},	
		{ 779, 8, 1},		
		{ 974, 8, 1},		
		{ 2394, 8, 1},		
		{ 2395, 8, 1},				
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver.m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;
	if (!AddToInventory(ch, pItem, true, true))
	{
		delete pItem;
		pItem = NULL;
		return false;
	}
	if (pItem->tab() < 0)
	{
		int nRow = -1, nCol = -1;
		ch->m_invenNormal.FindItem(&nRow, &nCol, pItem->m_itemProto->m_index, 0, 0);
		CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pPrevItem)
		{
			GAMELOG << init("USE UPGRADE BOX", ch)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< "TOTAL" << delim
					<< itemlog(pPrevItem)
					<< end;
			ItemUpdateMsg(rmsg, pPrevItem, nTable[nIndex][2]);
			SEND_Q(rmsg, ch->m_desc);
			delete pItem;
			pItem = NULL;
		}
	}
	else
	{
		GAMELOG << init("USE UPGRADE BOX", ch)
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< "TOTAL" << delim
				<< itemlog(pItem)
				<< end;
		ItemAddMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);
		pItem = NULL;
	}

	SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->m_index);
	SEND_Q(rmsg, ch->m_desc);

	return true;
}

bool do_ItemUse_2893(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_invenNormal.GetSpace() < 1)
	{	// 인벤에 공간이 없으면 실패 
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	CNetMsg rmsg;
	const int nTableSize	= 12;

	int nTable[nTableSize][3]	= {
		{ 1706, 8, 1},	
		{ 1707, 8, 1},		
		{ 2568, 8, 1},	
		{ 2569, 8, 1},	
		{ 2399, 8, 1},		
		{ 2396, 8, 1},		
		{ 2401, 8, 1},		
		{ 2398, 8, 1},		
		{ 2397, 8, 1},		
		{ 2402, 8, 1},
		{ 2359, 10, 5},
		{ 2454, 10, 100},
	};

	int nRate = GetRandom(1, 100);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver.m_itemProtoList.CreateItem(nTable[nIndex][0], -1, 0, 0, nTable[nIndex][2]);
	if (pItem == NULL)
		return false;
	if (!AddToInventory(ch, pItem, true, true))
	{
		delete pItem;
		pItem = NULL;
		return false;
	}
	if (pItem->tab() < 0)
	{
		int nRow = -1, nCol = -1;
		ch->m_invenNormal.FindItem(&nRow, &nCol, pItem->m_itemProto->m_index, 0, 0);
		CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pPrevItem)
		{
			GAMELOG << init("USE PET BOX", ch)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< "TOTAL" << delim
					<< itemlog(pPrevItem)
					<< end;
			ItemUpdateMsg(rmsg, pPrevItem, nTable[nIndex][2]);
			SEND_Q(rmsg, ch->m_desc);
			delete pItem;
			pItem = NULL;
		}
	}
	else
	{
		GAMELOG << init("USE PET BOX", ch)
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< "TOTAL" << delim
				<< itemlog(pItem)
				<< end;
		ItemAddMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);
		pItem = NULL;
	}

	SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->m_index);
	SEND_Q(rmsg, ch->m_desc);

	return true;
}

bool do_ItemUse_2968(CPC* ch, const CItemProto* itemproto)
{
	if(ch->m_invenNormal.GetSpace() < 1)
	{	// 인벤에 공간이 없으면 실패 
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	CNetMsg rmsg;
#ifdef IRIS_POINT //오픈시 사용되는 테이블
	const int nTableSize	= 33;
	int nTable[nTableSize][5]	= {		// 아이템인덱스, 확률, 개수, plus, flag	
		{2844, 10, 1, 0, 0},
		{85, 15, 1, 0, 0},
		{84, 35, 1, 0, 121},
		{499, 80, 1, 0, 12},
		{498, 80, 1, 0, 12},
		{884, 10, 1, 0, 0},
		{883, 10, 1, 0, 0},
		{465, 35, 1, 10, 0},
		{467, 35, 1, 10, 0},
		{476, 35, 1, 10, 0},
		{479, 35, 1, 10, 0},
		{482, 35, 1, 10, 0},
		{609, 35, 1, 10, 0},
		{795, 35, 1, 10, 0},
		{813, 35, 1, 10, 0},
		{826, 35, 1, 10, 0},
		{828, 35, 1, 10, 0},
		{1007, 35, 1, 10, 0},
		{1113, 35, 1, 10, 0},
		{882, 10, 1, 0, 0},
		{1283, 10, 1, 0, 0},
		{674, 35, 1, 0, 0},
		{723, 30, 1, 0, 0},
		{1288, 10, 1, 0, 0},
		{1404, 25, 1, 0, 0},
		{2361, 20, 1, 0, 0},
		{2358, 25, 1, 0, 0},
		{2404, 10, 1, 0, 0},
		{2454, 30, 1, 0, 0},
		{2141, 15, 1, 0, 0},
		{2360, 25, 1, 0, 0},
		{2652, 80, 5, 0, 0},
		{2455, 15, 1, 0, 0},
	};
#else
	const int nTableSize	= 34;
	int nTable[nTableSize][5]	= {		// 아이템인덱스, 확률, 개수, plus, flag
		{2844, 2, 1, 0, 0},
		{19, 2, 1000000000, 0, 0},
		{85, 10, 1, 0, 0},
		{2356, 10, 1, 0, 0},
		{2357, 10, 1, 0, 0},
		{884, 6, 1, 0, 0},
		{1288, 5, 1, 0, 0},
		{2569, 15, 1, 0, 0},
		{2568, 15, 1, 0, 0},
		{2404, 15, 1, 0, 0},
		{2894, 5, 1, 0, 0},
		{860, 30, 1, 0, 0},
		{1404, 15, 1, 0, 0},
		{859, 30, 1, 0, 0},
		{1283, 30, 1, 0, 0},
		{972, 30, 1, 0, 0},
		{840, 30, 1, 0, 0},
		{609, 30, 1, 10, 0},
		{1113, 30, 1, 10, 0},
		{479, 30, 1, 10, 0},
		{828, 35, 1, 10, 0},
		{1007, 35, 1, 10, 0},
		{795, 35, 1, 10, 0},
		{476, 35, 1, 10, 0},
		{465, 35, 1, 10, 0},
		{482, 35, 1, 10, 0},
		{826, 35, 1, 10, 0},
		{467, 35, 1, 10, 0},
		{813, 35, 1, 10, 0},
		{499, 80, 1, 0, 12},
		{501, 80, 1, 0, 12},
		{500, 80, 1, 0, 12},
		{498, 80, 1, 0, 12},
		{19, 15, 10000, 0, 0},		// IRIS_POINT 누적
	};
#endif

	int nRate = GetRandom(1, 1000);

	int nCnt = 0, nIndex = -1;
	for(int i = 0; i < nTableSize; i++)
	{
		nCnt += nTable[i][1];
		if(nRate <= nCnt)
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1 || nIndex >= nTableSize)
	{
		return false;
	}

	CItem* pItem = gserver.m_itemProtoList.CreateItem(nTable[nIndex][0], -1, nTable[nIndex][3], nTable[nIndex][4], nTable[nIndex][2]);
	if (pItem == NULL)
		return false;
	if (!AddToInventory(ch, pItem, true, true))
	{
		delete pItem;
		pItem = NULL;
		return false;
	}
	if (pItem->tab() < 0)
	{
		int nRow = -1, nCol = -1;
		ch->m_invenNormal.FindItem(&nRow, &nCol, pItem->m_itemProto->m_index, nTable[nIndex][3], nTable[nIndex][4]);
		CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pPrevItem)
		{
			GAMELOG << init("USE IRISBALL", ch)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< "TOTAL" << delim
					<< itemlog(pPrevItem)
					<< end;
			ItemUpdateMsg(rmsg, pPrevItem, nTable[nIndex][2]);
			SEND_Q(rmsg, ch->m_desc);
			delete pItem;
			pItem = NULL;
		}
	}
	else
	{
		GAMELOG << init("USE IRISBALL", ch)
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< "TOTAL" << delim
				<< itemlog(pItem)
				<< end;
		ItemAddMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);
		pItem = NULL;
	}

	SysChangeLuckyBoxMsg(rmsg, nTable[nIndex][0], itemproto->m_index);
	SEND_Q(rmsg, ch->m_desc);
	return true;
}

#ifdef EVENT_NOM_MOVIE
bool do_useEventItem_Nom(CPC* ch, CItem* pItem)
{
	CNetMsg rmsg;
	
	// 놈놈놈 복권
	if (pItem->m_itemProto->m_index == 2885)
	{
		// 보상 테이블
		int lottery_item[3][3] = {
			{ 836, 5, 0 },		// 수박
			{ 838, 5, 0 },		// 자두
			{ 837, 5, 0 },		// 참외
		};
		
		int nItemIdx = GetRandom(0, 2);
		if (nItemIdx < 0)
		{
			EventNomMsg(rmsg, MSG_EVENT_NOM_ERROR_ITEM_FAIL);
			SEND_Q(rmsg, ch->m_desc);
			return false;
		}
		
		// 아이템 지급
		if (!ch->GiveItem(lottery_item[nItemIdx][0], 0, 0, lottery_item[nItemIdx][1], false, false))
		{
			EventNomMsg(rmsg, MSG_EVENT_NOM_ERROR_ITEM_FAIL);
			SEND_Q(rmsg, ch->m_desc);
			
			// 롤백 
			int r, c;
			if (ch->m_invenNormal.FindItem(&r, &c, lottery_item[nItemIdx][0], 0, 0))
			{
				CItem* item = ch->m_invenNormal.GetItem(r, c);
				if(item)
				{
					CNetMsg itemMsg;
					
					// Item 수량 변경
					DecreaseFromInventory(ch, item, lottery_item[nItemIdx][1]);
					
					if (item->Count() > 0)
					{
						ItemUpdateMsg(itemMsg, item, -lottery_item[nItemIdx][1]);
						SEND_Q(itemMsg, ch->m_desc);
					}
					else
					{
						ItemDeleteMsg(itemMsg, item);
						SEND_Q(itemMsg, ch->m_desc);
						ch->RemoveItemFromQuickSlot(item);
						RemoveFromInventory(ch, item, true, true);
					}
				}
			} // 롤백 end
			return false;
		}
		
		GAMELOG << init("EVENT NOM LOTTERY GIVE", ch) 
			<< "ITEM" << delim << lottery_item[nItemIdx][0] << delim << lottery_item[nItemIdx][1] << end;
		
		// 복권 제거
		if(pItem->Count() > 1)
		{	// 하나 만 제거
			DecreaseFromInventory(ch, pItem, 1);
			
			ItemUpdateMsg(rmsg, pItem, -1);
			SEND_Q(rmsg, ch->m_desc);
		}
		else if(pItem->Count() == 1)
		{
			// 완전히 빼기
			ItemDeleteMsg(rmsg, pItem);
			SEND_Q(rmsg, ch->m_desc);
			
			RemoveFromInventory(ch, pItem, false, true);
		}
		
		// 복권 사용 성공
		EventNomMsg(rmsg, MSG_EVENT_NOM_LOTTERY_USE_OK);
		SEND_Q(rmsg, ch->m_desc);
		
		return true;
	}

	// 놈놈놈 영화 티켓
	if (pItem->m_itemProto->m_index == 2886)
	{
		if (IS_RUNNING_CONN)
		{
			CNetMsg rmsg;
			ConnEventNomMsg(rmsg, MSG_CONN_EVENT_NOM_MOVIE_TICKET_REQ, ch->m_desc->m_index, ch->m_index);
			SEND_Q(rmsg, gserver.m_connector);
		}
		return true;
	}

	return false;
}
#endif


bool do_ItemUse_GoldBox(CPC* ch, const CItemProto* itemproto)
{

	if(ch->m_invenNormal.GetSpace() < 1)
	{	// 인벤에 공간이 없으면 실패 
		// 만약 겹치는 아이템인 경우 원하는 아이템을 인벤에 넣어 놓고 인벤을 꽉 채우면
		// 우너하는 아이템을 받을 수 있기 때문에  이를 위한 방지 코드
		return false;
	}

	int row, col;
	if(!ch->m_invenNormal.FindItem(&row, &col, 3575, 0, 0))
	{
		return false;
	}

	int needitemcount = 100;
	CItem* pComboAccessory = ch->m_invenNormal.GetItem(row, col);
	if( !pComboAccessory || pComboAccessory->Count() < needitemcount )
	{
		return false;
	}

	int nMaxItemCount, nRandom, nRow, nCol;

	switch(itemproto->m_index)
	{
	case 3576:
		{
			nRow = 0;
			nMaxItemCount = 12;
		}
		break;
	case 3713:
		{
			nRow = 2;
			nMaxItemCount = 6;
		}
		break;
	case 3714:
		{
			nRow = 3;
			nMaxItemCount = 6;
		}
		break;
	case 3715:
		{
			nRow = 4;
			nMaxItemCount = 6;
		}
		break;
	case 3716:
		{
			nRow = 5;
			nMaxItemCount = 6;
		}
		break;
	case 3717:
		{
			nRow = 6;
			nMaxItemCount = 6;
		}
		break;
	case 3762:
		{
			nRow = 7;
			nMaxItemCount = 22;
		}
		break;
	case 3718:
		{
			nRow = 11;
			nMaxItemCount = 1;
		}
		break;
	
	default:
		return false;
		
	}

	static const int nTable[][6] = {
		{3719, 3720, 3721, 3722, 3723, 3724},		// 3576
		{3725, 3726, 3727, 3728, 3729, 3730},		// 3576
		{3731, 3736, 3742, 3747, 3752, 3757},		// 3713
		{3732, 3737, 3743, 3748, 3753, 3758},		// 3714
		{3733, 3738, 3744, 3749, 3754, 3759},		// 3715
		{3734, 3739, 3745, 3750, 3755, 3760},		// 3716
		{3735, 3740, 3746, 3751, 3756, 3761},		// 3717
		{3553, 3554, 3555, 3556, 3557, 3558},		// 3762
		{3559, 3560, 3561, 3562, 3563, 3564},		// 3762
		{3565, 3566, 3567, 3568, 3569, 3570},		// 3762
		{3571, 3572, 3573, 3574, 0, 0},				// 3762
		{3741,0,},									// 3718
	};

	nRandom = GetRandom(0, nMaxItemCount - 1);

	nRow = nRow + nRandom / 6;
	nCol = nRandom % 6;

	CNetMsg rmsg;
	CItem* pItem = gserver.m_itemProtoList.CreateItem(nTable[nRow][nCol], -1, 0, 0, 1);
	if (pItem == NULL)
		return false;
	if (!AddToInventory(ch, pItem, true, true))
	{
		delete pItem;
		pItem = NULL;
		return false;
	}
	if (pItem->tab() < 0)
	{
		int nRow = -1, nCol = -1;
		ch->m_invenNormal.FindItem(&nRow, &nCol, pItem->m_itemProto->m_index, 0, 0);
		CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
		if (pPrevItem)
		{
			GAMELOG << init("USE MCOMBO GOLD BOX", ch)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< end;
			ItemUpdateMsg(rmsg, pPrevItem, 1);
			SEND_Q(rmsg, ch->m_desc);
			delete pItem;
			pItem = NULL;
		}
	}
	else
	{
		GAMELOG << init("USE MCOMBO GOLD BOX", ch)
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< end;
		ItemAddMsg(rmsg, pItem);
		SEND_Q(rmsg, ch->m_desc);
		pItem = NULL;
	}

	if( pComboAccessory != NULL )
	{
		if( needitemcount < pComboAccessory->Count() )
		{
			DecreaseFromInventory(ch, pComboAccessory, needitemcount);
			ItemUpdateMsg(rmsg, pComboAccessory, -needitemcount);
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, pComboAccessory);
			SEND_Q(rmsg, ch->m_desc);
			ch->RemoveItemFromQuickSlot(pComboAccessory);
			RemoveFromInventory(ch, pComboAccessory, true, true);
		}
	}

	return true;
}

bool do_ItemUse_3769 ( CPC* ch, const CItemProto* itemproto )
{
	CNetMsg rmsg;

	if( ch->m_level > 2 )							// 사용 레벨 제한
		return false;

	if( ch->m_job < 0 || ch->m_job > 5)				// 직업제한
		return false;

	if( ch->m_invenNormal.GetSpace() < 7 )			// 장비 6 돈 1
	{
		SysMsg(rmsg, MSG_SYS_FULL_INVENTORY);
		SEND_Q( rmsg , ch->m_desc );
		return false;
	}

	int wepon[6][6] = { { 609, 469, 470, 471, 472, 473 },
						{ 618, 464, 465, 466, 467, 468 },
						{ 345, 474, 475, 476, 477, 478 },
						{ 636, 479, 480, 481, 482, 483 },
						{ 644, 570, 572, 574, 576, 578 },
						{ 997, 1047, 1007, 1017, 1027, 1037 },
	};

	CItem* item[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
	int nNeedWeight = 0;
	int i=0;
	for( i=0 ; i<6; i++ )
	{
		
		item[i] = gserver.m_itemProtoList.CreateItem(wepon[ch->m_job][i], -1, 4, 0, 1);
		if (!item)
		{
			nNeedWeight = -1;
			break;
		}
		nNeedWeight += item[i]->m_itemProto->m_weight;
	}

	if ( ch->m_weight + nNeedWeight >= ch->m_maxWeight * 15 / 10 || nNeedWeight<0 )		// 무개검사
	{
		for( i=0 ; i<6 ; i++ )
		{
			if( item[i] )
				delete item[i];
			item[i] = NULL;
		}		
		return false;
	}

	for( i=0 ; i<6; i++ )
	{
		AddToInventory(ch, item[i], true, true);
		ItemAddMsg(rmsg, item[i]);
		SEND_Q( rmsg, ch->m_desc );
	}

	if ( ch->m_moneyItem )
	{
		ch->AddMoney(2000000);
		ItemUpdateMsg(rmsg, ch->m_moneyItem, 2000000 );
	}
	else 
	{
		ch->AddMoney(2000000);
		ItemAddMsg(rmsg,  ch->m_moneyItem );
	}
	SEND_Q( rmsg, ch->m_desc );

	ch->m_statpt_remain += 44;
	StatPointRemainMsg(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);
	
	ch->m_exp = 0;
	ch->m_level = 35;
	ch->m_skillPoint += 40000000;
	ch->m_bChangeStatus = true;
	ch->LevelUp(true);
	ch->CalcStatus(true);

	ch->m_hp = ch->m_maxHP;
	ch->m_mp = ch->m_maxMP;
	
	return true;
}