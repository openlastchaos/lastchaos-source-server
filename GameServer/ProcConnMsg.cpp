#include "stdhdrs.h"
#include "Log.h"
#include "Exp.h"
#include "Server.h"
#include "Cmd.h"
#include "CmdMsg.h"
#include "DBCmd.h"
#include "doFunc.h"

#ifdef EVENT_FLOWERTREE_2007
void OnConnEventFlowerTree2007(CNetMsg& msg);
#endif //EVENT_FLOWERTREE_2007

#ifdef EVENT_GOMDORI_2007
void OnConnEventGomdori2007(CNetMsg& msg);
#endif // EVENT_GOMDORI_2007

#ifdef NEW_SERIAL_PACKAGE_EVENT
void OnCouponConfirm_new( CNetMsg& msg );
void OnCouponUse_new( CNetMsg& msg );
void RollBack_new( CPC* pc, int loopCount, int ItemIndex, int ItemCount );
#endif // NEW_SERIAL_PACKAGE_EVENT

#ifdef EVENT_PACKAGE_ITEM
void OnCouponConfirm(CNetMsg& msg);
void OnCouponUse(CNetMsg& msg);
void RollBack( CPC* pc, int loopCount, int ItemIndex , int ItemCount);
#endif

#ifdef EVENT_PCBANG
void OnPcbangItemGive(CNetMsg& msg);
#endif

#ifdef LIMIT_CATALOG
void OnLimitCatalog(CNetMsg& msg);
#endif

#ifdef FEEITEM
void OnCashItemPurchase(CNetMsg& msg);
void OnCashBalance(CNetMsg& msg);
void OnCashItemPurchaselist(CNetMsg& msg);
void OnCashItemPurchaseHistory(CNetMsg& msg);
void OnCashItemBring(CNetMsg& msg);
void OnCashItemBringProcess(CNetMsg& msg, bool bPresent);
#endif  // FEEITEM
void OnStashSetNewPassword(CNetMsg& msg);
void OnStashDeletePassword(CNetMsg& msg);

void OnStashSeal(CNetMsg& msg);
void OnStashChangePassword(CNetMsg& msg);
void OnStashCheckPassword(CNetMsg& msg);
void OnStashState(CNetMsg& msg);
void OnPreopenGift(CNetMsg& msg);
void OnLoginRep(CNetMsg& msg);

void OnEvent2pan4panLetterRep(CNetMsg& msg);
void OnEvent2pan4panLetterRollbackRep(CNetMsg& msg);

void OnEvent2pan4panBoxRep(CNetMsg& msg);
void OnEvent2pan4panBoxCommit(CNetMsg& msg);

void OnEvent2pan4panGoodsRep(CNetMsg& msg);
void OnEvent2pan4panGoodsCommit(CNetMsg& msg);

void OnEvent2pan4panMoneyRep(CNetMsg& msg);
#ifdef RANKER_NOTICE
void OnRankerRep(CNetMsg& msg);
#endif

#ifdef CASH_ITEM_GIFT
void OnCashItemGift(CNetMsg& msg);
void OnCashItemGiftSendHistory(CNetMsg& msg);
void OnCashItemGiftRecvHistory(CNetMsg& msg);
void OnCashItemGiftRecvList(CNetMsg& msg);
void OnCashItemGiftRecv(CNetMsg& msg);
void OnCashItemGiftPresentHistory(CNetMsg& msg, bool bSend);
#endif

#ifdef CASH_ITEM_COUPON
void OnCashItemCouponList(CNetMsg& msg);
void OnCashItemBuyByCoupon(CNetMsg& msg);
#endif //CASH_ITEM_COUPON

void OnMoveServerOK(CNetMsg& msg);

#ifdef EVENT_OPEN_BETA_GIFT
void OnOpenBetaGift(CNetMsg& msg);
#endif

#ifdef JPN_GPARA_PROMOTION
void OngparaPromotion(CNetMsg& msg);
#endif // JPN_GPARA_PROMOTION

#ifdef JPN_OCN_GOO_PROMOTION
void OnOCN_GooPromotion(CNetMsg& msg);
#endif // JPN_OCN_GOO_PROMOTION

#ifdef JPN_MSN_PROMOTION
void  OnMSNPromotion(CNetMsg& msg);
#endif // JPN_MSN_PROMOTION

#ifdef CREATE_EVENT
void  OnCreateEvent(CNetMsg& msg);
#endif // CREATE_EVENT

#ifdef GIFT_EVENT_2007
void OnEventGift2007(CNetMsg & msg);
#endif // GIFT_EVENT_2007

#ifdef EVENT_BJMONO_2007
void OnConnEventBjMono2007(CNetMsg & msg);
#endif // EVENT_BJMONO_2007

#ifdef NEW_GUILD_POINT_RANKING_NOTICE
void OnGuildPointRankingRep( CNetMsg& msg );
#endif // NEW_GUILD_POINT_RANKING_NOTICE

#ifdef EVENT_SSHOT_2007
void OnConnEventSshot2007(CNetMsg& msg);
#endif // EVENT_SSHOT_2007

#ifdef EVENT_XMAS_2007
void OnConnEventXmas2007( CNetMsg& msg );
#endif //EVENT_XMAS_2007

#ifdef ATTENDANCE_EVENT_REWARD
void OnConnEventAttendance( CNetMsg& msg );
#endif //ATTENDANCE_EVENT_REWARD

#ifdef EVENT_NOM_MOVIE
void OnEventNom(CNetMsg& msg);
#endif

#ifdef REQUITAL_EVENT	// 보상 이벤트 
void OnEventRequital(CNetMsg& msg);
#endif // REQUITAL_EVENT

#ifdef IRIS_POINT
void OnConnMessageIPoint( CNetMsg& msg );
#endif //IRIS_POINT

void CServer::ProcessBilling(CNetMsg& msg)
{
	msg.MoveFirst();
	
	unsigned char subtype;
	
	msg >> subtype;
	
	if (msg.m_mtype != MSG_CONN_REP)
		return ;

	switch (subtype)
	{
#ifdef ATTENDANCE_EVENT_REWARD
	case MSG_CONN_ATTENDANCE_COUNT:
		OnConnEventAttendance(msg);
		break;
#endif //ATTENDANCE_EVENT_REWARD

#ifdef EVENT_XMAS_2007
	case MSG_CONN_EVENT_XMAS_2007:
		OnConnEventXmas2007(msg);
		break;
#endif //EVENT_XMAS_2007

#ifdef EVENT_SSHOT_2007
	case MSG_CONN_EVENT_SSHOT_2007 :
		OnConnEventSshot2007( msg );
		break;
#endif //EVENT_SSHOT_2007

#ifdef NEW_GUILD_POINT_RANKING_NOTICE
	case MSG_CONN_GUILD_POINT_RANKER_REP :
		OnGuildPointRankingRep( msg );
		break;
#endif // NEW_GUILD_POINT_RANKING_NOTICE

#ifdef EVENT_BJMONO_2007
	case MSG_CONN_EVENT_BJMONO_2007 :
		OnConnEventBjMono2007(msg);
		break;
#endif // EVENT_BJMONO_2007

#ifdef EVENT_FLOWERTREE_2007
	case MSG_CONN_EVENT_FLOWERTREE_2007 :
		OnConnEventFlowerTree2007(msg);
		break;
#endif //EVENT_FLOWERTREE_2007

#ifdef EVENT_GOMDORI_2007
	case MSG_CONN_EVENT_GOMDORI_2007:
		OnConnEventGomdori2007(msg);
		break;
#endif // EVENT_GOMDORI_2007

	case MSG_CONN_GPARA_PROMOTION:
#ifdef JPN_GPARA_PROMOTION
		OngparaPromotion(msg);
#endif // JPN_GPARA_PROMOTION
		break;

	case MSG_CONN_OCN_GOO_PROMOTION:
#ifdef JPN_OCN_GOO_PROMOTION
		OnOCN_GooPromotion(msg);
#endif // JPN_OCN_GOO_PROMOTION
		break;

	case MSG_CONN_MSN_PROMOTION:
#ifdef JPN_MSN_PROMOTION
		OnMSNPromotion(msg);
#endif // JPN_MSN_PROMOTION
		break;

	case MSG_CONN_CREATEEVENT:
#ifdef CREATE_EVENT
		OnCreateEvent(msg);
#endif // CREATE_EVENT

	case MSG_CONN_MOVESERVER_OK:
		OnMoveServerOK(msg);
		break;

#ifdef NEW_SERIAL_PACKAGE_EVENT
	case MSG_CONN_COUPON_CONFIRM_NEW:
		OnCouponConfirm_new( msg );
		break;
	case MSG_CONN_COUPON_USE_NEW:
		OnCouponUse_new( msg );
		break;
#endif // NEW_SERIAL_PACKAGE_EVENT

#ifdef EVENT_PACKAGE_ITEM
	case MSG_CONN_COUPON_CONFIRM:
		OnCouponConfirm(msg);
		break;
	case MSG_CONN_COUPON_USE:
		OnCouponUse(msg);
		break;
#endif
#ifdef EVENT_PCBANG
	case MSG_CONN_PCBANG_ITEM_GIVE:
		OnPcbangItemGive(msg);
		break;
#endif //#ifdef EVENT_PCBANG
#ifdef RANKER_NOTICE
	case MSG_CONN_RANKER_REP:
		OnRankerRep(msg);
		break;
#endif
	case MSG_CONN_EVENT_2PAN4PAN_MONEY_REP:
		OnEvent2pan4panMoneyRep(msg);
		break;

	case MSG_CONN_EVENT_2PAN4PAN_BOX_REP:
		OnEvent2pan4panBoxRep(msg);
		break;
	case MSG_CONN_EVENT_2PAN4PAN_BOX_COMMIT:
		OnEvent2pan4panBoxCommit(msg);
		break;

	case MSG_CONN_EVENT_2PAN4PAN_GOODS_REP:
		OnEvent2pan4panGoodsRep(msg);
		break;
	case MSG_CONN_EVENT_2PAN4PAN_GOODS_COMMIT:
		OnEvent2pan4panGoodsCommit(msg);
		break;

	case MSG_CONN_EVENT_2PAN4PAN_LETTER_REP:
		OnEvent2pan4panLetterRep(msg);
		break;
	case MSG_CONN_EVENT_2PAN4PAN_LETTER_ROLLBACK_REP:
		OnEvent2pan4panLetterRollbackRep(msg);
		break;

#ifdef FEEITEM
	case MSG_CONN_CASHITEM_BRING_REP:
		OnCashItemBring(msg);
		break;
	case MSG_CONN_CASHITEM_PURCHASE_REP:
		OnCashItemPurchase(msg);
		break;
	case MSG_CONN_CASHITEM_BALANCE_REP:
		OnCashBalance(msg);
		break;
	case MSG_CONN_CASHITEM_PURCHASELIST_REP:
		OnCashItemPurchaselist(msg);
		break;
	case MSG_CONN_CASHITEM_PURCHASEHISTORY_REP:
		OnCashItemPurchaseHistory(msg);
		break;
#endif // FEEITEM
#ifdef LIMIT_CATALOG
	case MSG_CONN_LIMIT_CATALOG:
		OnLimitCatalog(msg);
		break;
#endif // LIMIT_CATALOG
#ifdef CASH_ITEM_COUPON
	case MSG_CONN_CASHITEM_COUPON:
		OnCashItemCouponList(msg);
		break;
	case MSG_CONN_CASHITEM_PURCHASE_WITH_COUPON :
		OnCashItemBuyByCoupon(msg);
		break;
#endif //CASH_ITEM_COUPON
/////////////////////////////////////////////
// BANGWALL : 2005-07-05 오후 2:37:03
// Comment : 테섭적용

	case MSG_CONN_STASH_DELETE_PASSWORD:
		OnStashDeletePassword(msg);
		break;
	case MSG_CONN_STASH_SET_NEW_PASSWORD:
		OnStashSetNewPassword(msg);
		break;

	case MSG_CONN_STASH_SEAL:
		OnStashSeal(msg);
		break;

	case MSG_CONN_STASH_CHANGE_PASSWORD:
		OnStashChangePassword(msg);
		break;

	case MSG_CONN_STASH_CHECK_PASSWORD:
		OnStashCheckPassword(msg);
		break;

	case MSG_CONN_STASH_STATE:
		OnStashState(msg);
		break;

	case MSG_CONN_PREOPEN_GIFT:
		OnPreopenGift(msg);
		break;

	case MSG_CONN_LOGIN_REP:
		OnLoginRep(msg);
		break;
#ifdef LC_TIME
	case MSG_CONN_LCTIME:
		msg >> gserver.m_gameTime
			>> gserver.m_lcStartTime;
		break;
#endif
#ifdef CASH_ITEM_GIFT
	case MSG_CONN_CASHITEM_GIFT:
		OnCashItemGift(msg);
		break;
	case MSG_CONN_CASHITEM_GIFT_SENDHISTORY:  
		OnCashItemGiftSendHistory(msg);
		break;
	case MSG_CONN_CASHITEM_GIFT_RECVHISTORY:  
		OnCashItemGiftRecvHistory(msg);
		break;
	case MSG_CONN_CASHITEM_GIFT_RECVLIST:     
		OnCashItemGiftRecvList(msg);
		break;
	case MSG_CONN_CASHITEM_GIFT_RECV:		
		OnCashItemGiftRecv(msg);
		break;
#endif
#ifdef EVENT_OPEN_BETA_GIFT
	case MSG_CONN_OPEN_BETA_GIFT:
		OnOpenBetaGift(msg);
		break;
#endif // EVENT_OPEN_BETA_GIFT

#ifdef GIFT_EVENT_2007
	case MSG_CONN_EVENT_GIFT_2007:
		OnEventGift2007(msg);
		break;
#endif // GIFT_EVENT_2007
		
#ifdef EVENT_NOM_MOVIE
	case MSG_CONN_EVENT_NOM:
		OnEventNom(msg);
		break;
#endif

#ifdef REQUITAL_EVENT	// 보상 이벤트 
	case MSG_CONN_EVENT_REQUITAL:
		OnEventRequital(msg);
		break;
#endif // REQUITAL_EVENT
		
#ifdef IRIS_POINT
	case MSG_CONN_ADD_IPOINT:
		OnConnMessageIPoint(msg);
		break;
#endif //IRIS_POINT
	}
}

#ifdef LIMIT_CATALOG
// 물품이 팔렸을때 커넥터가 한정판매 물품 갱신 메세지를 보냄
void OnLimitCatalog(CNetMsg& msg)
{
	int ctid, sell, count, i;
	

	msg >> count;
	CCatalog* catal = NULL;
	for(i = 0; i < count; i++)
	{
		msg >> ctid
			>> sell;
		catal = gserver.m_catalogList.Find(ctid);
		if( catal )
			catal->SetToSell(catal->GetLimit() - sell);
	}
}
#endif

void OnLoginRep(CNetMsg& msg)
{
	CLCString id(MAX_ID_NAME_LENGTH + 1);
	unsigned char errcode, usertype, paytype, location;
	int index, timestamp, timeremain;
			
	msg >> id
		>> errcode;
			
	CDescriptor* desc = gserver.FindConnectIdName(id, NULL);
	
	if (desc == NULL)
	{
		// 빌링 메시지에 해당하는 사용자 없음
		GAMELOG << init("SYS_ERR") << "Connector descriptor not fount :" << id << ": " << errcode << end;
		
		// TODO : 로그아웃 시켜야 하는 오류 코드 검사
		if (errcode == MSG_CONN_ERROR_SUCCESS)
		{
			// 로그아웃 보내기
			CNetMsg cmsg;
			ConnLogoutMsg(cmsg, id);
			if (IS_RUNNING_CONN)
				SEND_Q(cmsg, gserver.m_connector);
		}
		return;
	}
	
	if (errcode == MSG_CONN_ERROR_SUCCESS)
	{
		desc->m_logined = LOGIN_STATE_SUCCESS;
		
		msg >> index
			>> usertype
			>> timestamp
			>> paytype
			>> location
			>> timeremain;

#if defined(EVENT_PCBANG) || defined(EVENT_PCBANG_2ND)
		desc->m_location = location;
#endif
		//0627
		msg >> desc->m_userFlag;
#ifdef CHARDEL_CHECKID
		msg >> desc->m_ident;
#endif
#ifdef EVENT_PROMOTION_SITE
		CLCString proSite(4);
		msg >> proSite;
		desc->m_proSite = proSite;
#endif//#ifdef EVENT_PROMOTION_SITE
		
#ifdef HANARO_EVENT
		// 코드 조작을 로그인 한 유저를 방지 하기 위해.
		if (strcmp(desc->m_proSite, "HF") != 0 && strcmp(desc->m_proSite, "hf") != 0)
		{
			desc->m_hanaroCode = "";
		}
		GAMELOG << init("HANARO_EVENT", id) 
			    << "PROSITE : " << desc->m_proSite << delim
				<< "HAHARO_CODE : " << desc->m_hanaroCode << end;
#endif
		
		desc->m_index = index;
	}
	else if ( errcode != MSG_CONN_ERROR_ALREADY)
		desc->m_logined = LOGIN_STATE_FAIL;
	
	// 각 경우에 해당하는 로그 남기기
	CNetMsg rmsg;
	switch (errcode)
	{
	case MSG_CONN_ERROR_SUCCESS:
		if (STATE(desc) != CON_PLAYING && STATE(desc) != CON_CLOSE && STATE(desc) != CON_DISCONNECT)
			STATE(desc) = CON_GET_IDEN_MSG;
		break;
		
	case MSG_CONN_ERROR_ALREADY:
		GAMELOG << init("CONN_ERR", id) << "ALREADY CONNECT" << end;
		MsgrLogoutReqMsg(rmsg, 0, -1, -1, -1, id);
		if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
		FailMsg(rmsg, MSG_FAIL_BILLING_ALREADY_CONNECT);
		SEND_Q(rmsg, desc);
		break;
		
	case MSG_CONN_ERROR_FULLUSER:
	case MSG_CONN_ERROR_SYSTEM:
		GAMELOG << init("CONN_ERR", id) << "CONNECTOR SYSTEM ERROR" << end;
		FailMsg(rmsg, MSG_FAIL_BILLING_OTHER);
		SEND_Q(rmsg, desc);
		break;
		
	case MSG_CONN_ERROR_NOMATCH:
		GAMELOG << init("CONN_ERR", id) << "NO MATCH" << end;
		FailMsg(rmsg, MSG_FAIL_BILLING_WRONGPW);
		SEND_Q(rmsg, desc);
		break;
		
	case MSG_CONN_ERROR_BLOCKEDUSER:
		GAMELOG << init("CONN_ERR", id) << "BLOCKED USER" << end;
		FailMsg(rmsg, MSG_FAIL_BILLING_BLOCKED);
		SEND_Q(rmsg, desc);
		break;
		
	default:
		FailMsg(rmsg, MSG_FAIL_BILLING_OTHER);
		SEND_Q(rmsg, desc);
		break;
	}
}

void OnMoveServerOK(CNetMsg& msg)
{
	int userindex = -1;
	msg >> userindex;

	CDescriptor* d;
	CDescriptor* dFound = NULL;
	for (d = gserver.m_desclist; d && dFound == NULL; d = d->m_pNext)
	{
		switch (STATE(d))
		{
		case CON_MOVESERVER_WAIT:
			if (d->m_index == userindex)
			{
				dFound = d;
				break;
			}
		}
	}
	if (dFound)
	{
		SEND_Q(dFound->m_msgGoZone, dFound);
		dFound->m_bGoZoneSent = true;
	}
}

/////////////////////////////////////////////
// BANGWALL : 2005-07-05 오후 2:36:52
// Comment : 테섭적용

void OnStashSetNewPassword(CNetMsg& msg)
{
	int userindex = -1;
	int charindex = -1;
	char valid = 0;

	msg >> userindex
		>> charindex
		>> valid;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		if(valid == 0)
		{
			CNetMsg rmsg;
			StashSetNewPasswordRepMsg(rmsg, 0);
			SEND_Q(rmsg, pc->m_desc);

			GAMELOG << init("STASH SET PASSWORD", pc)
					<< end;
		}
		else
		{
			CNetMsg rmsg;
			StashSetNewPasswordRepMsg(rmsg, valid);
			SEND_Q(rmsg, pc->m_desc);
		}
		
		pc->m_stashMsg.Init();
	}
}

#ifdef FEEITEM
void OnCashItemPurchaseHistory(CNetMsg& msg)
{
	//MSG_CONN_CASHITEM_PURCHASEHISTORY_REP,//응답		: charindex(n) errorcode(uc) count(n) ctid(n)
	int charindex, count, ctid, i, ctcount;
	unsigned char errorcode;
	//MSG_EX_CASHITEM_PURCHASEHISTORY_REP,// 응답				: errorcode(uc) count(n) ctid(n)

	msg >> charindex
		>> errorcode
		>> count;

	CPC* pc = gserver.m_playerList.Find(charindex);

	if( !pc )
	{
		return;
	}

	CNetMsg rmsg;
	rmsg.Init(MSG_EXTEND);
	rmsg << MSG_EX_CASHITEM
		 << (unsigned char) MSG_EX_CASHITEM_PURCHASEHISTORY_REP
		 << errorcode
		 << count;

	for(i = 0; i < count; i++)
	{
		msg >> ctcount
			>> ctid;
		rmsg << ctcount
			 << ctid;
	}

	SEND_Q(rmsg, pc->m_desc);
}
void OnCashItemBring(CNetMsg& msg)
{
	OnCashItemBringProcess(msg, false);
}

void OnCashItemBringProcess(CNetMsg& msg, bool bPresent)
{
	//MSG_CONN_CASHITEM_BRING_REP,	// 응답				: charindex(n) errorCode(uc) count(n) ctid(n)
	int charindex, count, i;
	unsigned char errorcode;
	int ctid[10];

	msg >> charindex
		>> errorcode
		>> count;

	for(i = 0; i < count; i++)
	{
		msg >> ctid[i];
	}

	CPC* pc = gserver.m_playerList.Find(charindex);

	if( !pc )
	{
		GAMELOG << init("CASH_ITEM_BRING_ERROR-NotChar")
				<< (bPresent ? "PRESENT" : "NOT_PRESENT") << delim
				<< charindex << delim
				<< errorcode << delim
				<< count << delim;

		for(i = 0; i < count; i++)
		{
			GAMELOG << ctid[i] << delim;
		}
		GAMELOG << end;

		return;
	}

	CNetMsg rmsg;
	if((!bPresent && errorcode == MSG_EX_CASHITEM_ERROR_SUCCESS) || 
		(bPresent && errorcode == MSG_EX_CASHITEM_ERROR_GIFTRECV_SUCCESS))
	{
		// 성공 메세지를 보냄
		// 케릭터 락해제
		pc->ResetPlayerState(PLAYER_STATE_CASHITEM);
		pc->m_billReqTime = 0;
		CashItemBringRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errorcode, bPresent);
		SEND_Q(rmsg, pc->m_desc);

		GAMELOG << init("CASH_ITEM_BRING_SUCCESS", pc)
				<< (bPresent ? "PRESENT" : "NOT_PRESENT") << delim
				<< count << delim;

		CCatalog* catal = NULL;
		for(i = 0; i < count; i++)
		{
			catal = gserver.m_catalogList.Find(ctid[i]);
			if( catal )
			{
				GAMELOG << catal->GetIndex() << delim
						<< catal->GetName() << delim
						<< catal->GetCash() << delim
						<< catal->GetMileage() << delim;
			}
		}
		GAMELOG << end;

		return;
	}


	CCtItemList* ctItemList = NULL;
	// 롤백
	for(i = 0; i < count; i++)
	{

		int j = 0;

		CCatalog* m_catalog = gserver.m_catalogList.Find(ctid[i]);

		if(!m_catalog)
		{
			// 상품 목록이 없어서 롤백 몬한다
			return;
		}

		ctItemList = m_catalog->GetCtItemList();

		if( !ctItemList )
		{
			return;
		}

		int count = ctItemList->GetCount();

		CItem* ctitem;
		for(j = 0; j < count; j++)
		{
			ctitem = ctItemList->GetItem(j);

			int r, c;
			if (!pc->m_invenNormal.FindItem(&r, &c, ctitem->m_itemProto->m_index, ctitem->m_plus, ctitem->m_flag) )
				continue;
			
			CItem* item = pc->m_invenNormal.GetItem(r, c);
			if (!item)
				continue;
			
			// item 수량 변경
			CNetMsg itemmsg;
			DecreaseFromInventory(pc, item, ctitem->Count());
			
			if (item->Count() > 0)
			{
				ItemUpdateMsg(itemmsg, item, -ctitem->Count());
				SEND_Q(itemmsg, pc->m_desc);
			}
			else
			{
				ItemDeleteMsg(itemmsg, item);
				SEND_Q(itemmsg, pc->m_desc);
				pc->RemoveItemFromQuickSlot(item);
				RemoveFromInventory(pc, item, true, true);
			}

		}
	}

	pc->ResetPlayerState(PLAYER_STATE_CASHITEM);
	pc->m_billReqTime = 0;
	CashItemBringRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errorcode, bPresent);
	SEND_Q(rmsg, pc->m_desc);
}

void OnCashItemPurchaselist(CNetMsg& msg)
{
	int charindex, count, i;
	unsigned char errorcode;
	int idx[25];
	int ctid[25];

	msg >> charindex
		>> errorcode
		>> count;

	CPC* pc = gserver.m_playerList.Find(charindex);

	if( !pc )
	{
		GAMELOG << init("CASH_ITEM_PURCHASELIST_ERROR-NotChar")
				<< charindex << end;
		return;
	}

	for(i = 0; i < count; i++)
	{
		msg >> idx[i]
			>> ctid[i];
	}

	CNetMsg rmsg;
	CashItemPurchaseListRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errorcode, count, idx, ctid);
	SEND_Q(rmsg, pc->m_desc);
}

void OnCashItemPurchase(CNetMsg& msg)
{
	//MSG_CONN_CASHITEM_PURCHASE_REP,	// 응답				: userid(str) errorcode(c) cashBalance(n)
	unsigned char errorcode;
	int cashBalance;
	int userindex;

	msg >> userindex
		>> errorcode
		>> cashBalance;

	CPC *pc = NULL;
	
	int i;
	for (i = 0; i < gserver.m_playerList.m_max; i++)
	{
		if (gserver.m_playerList.m_pcList[i] && userindex == gserver.m_playerList.m_pcList[i]->m_desc->m_index )
		{
			pc = gserver.m_playerList.m_pcList[i];
			break;
		}
		pc = NULL;
	}

	if( pc )
	{

		GAMELOG << init("CASH_ITEM_PURCHASE", pc);

		if( !errorcode )
			GAMELOG	<< "SUCCESS";
		else
			GAMELOG << "ERROR";

		GAMELOG	<< errorcode << delim 
				<< pc->m_cashBalance << delim
				<< cashBalance << end;

		pc->m_cashBalance = cashBalance;
		// 캐쉬 메세지 보냄
		CNetMsg rmsg;
		pc->ResetPlayerState(PLAYER_STATE_CASHITEM);
		pc->m_billReqTime = 0;
		CashItemPurchaseRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errorcode, cashBalance);
		SEND_Q(rmsg, pc->m_desc);
		return;
	}

	GAMELOG << init("CASH_ITEM_PURCHASE_ERROR")
			<< userindex;

}

void OnCashBalance(CNetMsg& msg)
{
	unsigned char errorcode;
	int cashBalance;
	int userindex;
	int i;

	msg >> userindex
		>> errorcode
		>> cashBalance;

	CPC *pc = NULL;
	
	for (i = 0; i < gserver.m_playerList.m_max; i++)
	{
		if (gserver.m_playerList.m_pcList[i] && userindex == gserver.m_playerList.m_pcList[i]->m_desc->m_index )
		{
			pc = gserver.m_playerList.m_pcList[i];
			break;
		}
		pc = NULL;
	}

	if( pc )
	{
		pc->m_cashBalance = cashBalance;
		// 캐쉬 메세지 보냄
		CNetMsg rmsg;
		CashItemBalanceRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errorcode, cashBalance);
		SEND_Q(rmsg, pc->m_desc);
		return;
	}

	GAMELOG << init("CASH_BALANCE_ERROR")
			<< userindex;
}
#endif // LIMIT_CATALOG

#ifdef CASH_ITEM_COUPON
void OnCashItemCouponList( CNetMsg& msg )
{
	unsigned char errorcode;
	int userindex;
	int i;
	int nCouponCount;
	CASH_COUPON *pCoupon;

	msg >> userindex
		>> errorcode
		>> nCouponCount;

	CPC *pc = NULL;
	
	for (i = 0; i < gserver.m_playerList.m_max; i++)
	{
		if (gserver.m_playerList.m_pcList[i] && userindex == gserver.m_playerList.m_pcList[i]->m_desc->m_index )
		{
			pc = gserver.m_playerList.m_pcList[i];
			break;
		}
		pc = NULL;
	}

	if( pc )
	{
		pCoupon = new CASH_COUPON[nCouponCount];
		for( i=0; i<nCouponCount; i++ )
		{
			msg >> pCoupon[i].name
				>> pCoupon[i].id
				>> pCoupon[i].serial		// 클라이언트 필요 없음
				>> pCoupon[i].amount
				>> pCoupon[i].limitAmount;
		}

		// 캐쉬 메세지 보냄
		CNetMsg rmsg;
		CashItemCouponListRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errorcode, nCouponCount, pCoupon );
		SEND_Q(rmsg, pc->m_desc);

		delete[] pCoupon;
		return;
	}

	GAMELOG << init("CASH_BALANCE_ERROR")
			<< userindex;
}

void OnCashItemBuyByCoupon(CNetMsg& msg)
{
	unsigned char errorcode;
	int cashBalance;
	int userindex;

	msg >> userindex
		>> errorcode
		>> cashBalance;

	CPC *pc = NULL;
	
	int i;
	for (i = 0; i < gserver.m_playerList.m_max; i++)
	{
		if (gserver.m_playerList.m_pcList[i] && userindex == gserver.m_playerList.m_pcList[i]->m_desc->m_index )
		{
			pc = gserver.m_playerList.m_pcList[i];
			break;
		}
		pc = NULL;
	}

	if( pc )
	{

		GAMELOG << init("CASH_ITEM_PURCHASE_BY_COUPON", pc);

		if( !errorcode )
			GAMELOG	<< "SUCCESS";
		else
			GAMELOG << "ERROR";

		GAMELOG	<< errorcode << delim 
				<< pc->m_cashBalance << delim
				<< cashBalance << end;

		pc->m_cashBalance = cashBalance;
		// 캐쉬 메세지 보냄
		CNetMsg rmsg;
		pc->ResetPlayerState(PLAYER_STATE_CASHITEM);
		pc->m_billReqTime = 0;
		CashItemPurchaseByCouponRepMsg( rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errorcode, cashBalance);
		SEND_Q(rmsg, pc->m_desc);
		return;
	}

	GAMELOG << init("CASH_ITEM_PURCHASE_ERROR")
			<< userindex;
}

#endif //CASH_ITEM_COUPON

void OnStashDeletePassword(CNetMsg& msg)
{
	// 창고 패스워드 삭제 결과를 받음
	int userindex = -1;
	int charindex = -1;
	char valid = 0;

	msg >> userindex
		>> charindex
		>> valid;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		if(valid == 0)
		{
			// 삭제 성공
			CNetMsg rmsg;
			StashDeletePasswordRepMsg(rmsg, 0);
			SEND_Q(rmsg, pc->m_desc);

			GAMELOG << init("STASH PASSWORD DELETE", pc)
					<< end;
		}
		else if(valid == 1)
		{
			// 민증번호 틀림
			CNetMsg rmsg;
			StashDeletePasswordRepMsg(rmsg, 1);
			SEND_Q(rmsg, pc->m_desc);
		}
		else
		{
			//오류
			CNetMsg rmsg;
			StashDeletePasswordRepMsg(rmsg, 2);
			SEND_Q(rmsg, pc->m_desc);
		}
		
		pc->m_stashMsg.Init();
	}
}

void OnStashSeal(CNetMsg& msg)
{
	// 잠금 결과를 받음

	int userindex = -1;
	int charindex = -1;
	char valid = 0;

	msg >> userindex
		>> charindex
		>> valid;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		if (valid == 0)
		{
			// 잘 잠김

			CNetMsg rmsg;
			StashSealMsg(rmsg);
			SEND_Q(rmsg, pc->m_desc);

			GAMELOG << init("STASH SEALED", pc)
					<< end;
		}
		else if (valid == 1)
		{
			// 이미 잠김

			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_ALREADY_SEAL);
			SEND_Q(rmsg, pc->m_desc);
		}
		else
		{
			// 오류

			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_SEAL);
			SEND_Q(rmsg, pc->m_desc);
		}

		// 처리 중인 메시지는 초기화
		pc->m_stashMsg.Init();
	}
}

void OnStashChangePassword(CNetMsg& msg)
{
	// 암호 변경 결과를 받음
	
	int userindex = -1;
	int charindex = -1;
	CLCString pw(MAX_STASH_PASSWORD_LENGTH + 1);
	CLCString pw2(MAX_STASH_PASSWORD_LENGTH + 1);
	char valid = 0;
	
	msg >> userindex
		>> charindex
		>> pw
		>> pw2
		>> valid;
	
	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		if (valid == 0)
		{
			// 올바른 암호임
			
			CNetMsg rmsg;
			StashChangePasswordRepMsg(rmsg, true);
			SEND_Q(rmsg, pc->m_desc);
			
			GAMELOG << init("STASH CHANGE PASSWORD", pc)
					<< pw << delim
					<< pw2
					<< end;
		}
		else if (valid == 1)
		{
			// 잘못된 암호임
			
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_WRONG_PASSWORD);
			SEND_Q(rmsg, pc->m_desc);
		}
		else
		{
			// 잠김
			
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_SEALED);
			SEND_Q(rmsg, pc->m_desc);
		}
		
		// 처리 중인 메시지는 초기화
		pc->m_stashMsg.Init();
	}
}

void OnStashCheckPassword(CNetMsg& msg)
{
	// 암호 검사 결과를 받음
	
	int userindex = -1;
	int charindex = -1;
	CLCString pw(MAX_STASH_PASSWORD_LENGTH + 1);
	char valid = 0;
	
	msg >> userindex
		>> charindex
		>> pw
		>> valid;
	
	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		unsigned char subtype = (unsigned char)-1;
		pc->m_stashMsg.MoveFirst();
		pc->m_stashMsg >> subtype;

		/////////////////////////////////////////////
		// BANGWALL : 2005-07-02 오전 11:05:10
		// Comment : list를 요구할때만 패스워드 확인 
		if( subtype == MSG_STASH_KEEP_REQ || subtype == MSG_STASH_TAKE_REQ) valid = 0;

		if (valid == 0)
		{
			// 올바른 암호임
			/*
			unsigned char subtype = (unsigned char)-1;
			pc->m_stashMsg.MoveFirst();
			pc->m_stashMsg >> subtype;*/
			
			// 각 메시지에 따른 처리
			switch (subtype)
			{
			case MSG_STASH_LIST_REQ:
				do_StashListReq_real(pc);
				break;
				
			case MSG_STASH_KEEP_REQ:
				do_StashKeep_real(pc);
				break;
				
			case MSG_STASH_TAKE_REQ:
				do_StashTake_real(pc);
				break;
			case MSG_STASH_CHECK_PASSWORD_REQ:
				CNetMsg rmsg;
				rmsg.Init(MSG_STASH);
				rmsg << (unsigned char) MSG_STASH_CHECK_PASSWORD_REP;
				SEND_Q(rmsg, pc->m_desc);
				break;
			}
		}
		else if (valid == 1)
		{
			// 잘못된 암호임
			
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_WRONG_PASSWORD);
			SEND_Q(rmsg, pc->m_desc);
		}
		else
		{
			// 잠김
			
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_SEALED);
			SEND_Q(rmsg, pc->m_desc);
		}
		
		// 처리 중인 메시지는 초기화
		pc->m_stashMsg.Init();
	}
}

void OnStashState(CNetMsg& msg)
{
	// 창고 상태 조사 결과를 받음
	
	int userindex = -1;
	int charindex = -1;
	char pw = -1;
	char sealed = -1;
	
	msg >> userindex
		>> charindex
		>> pw
		>> sealed;
	
	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		if (pw < 0 || sealed < 0)
		{
			// 오류난 경우
			CNetMsg rmsg;
			StashErrorMsg(rmsg, MSG_STASH_ERROR_CANNOT_STATE);
			SEND_Q(rmsg, pc->m_desc);
		}
		else
		{
			if (sealed)
			{
				// 잠긴 경우
				CNetMsg rmsg;
				StashErrorMsg(rmsg, MSG_STASH_ERROR_SEALED);
				SEND_Q(rmsg, pc->m_desc);
			}
			else
			{
				// 암호 설정 여부 보내기
				CNetMsg rmsg;
				StashIsSetPassword(rmsg, (pw) ? true : false);
				SEND_Q(rmsg, pc->m_desc);
			}
		}
	}
}

void OnPreopenGift(CNetMsg& msg)
{
	CNetMsg rmsg;
	
	int userindex = 0;
	int charindex = 0;
	char errorcode = -1;
	
	msg >> userindex
		>> charindex
		>> errorcode;
	
	if (errorcode == MSG_CONN_ERROR_SUCCESS)
	{
		CPC* pc = gserver.m_playerList.Find(charindex);
		if (pc && pc->m_desc->m_index == userindex)
		{
			int giftindex = -1;
			switch (pc->m_job)
			{
			case JOB_TITAN:
				giftindex = 460;
				break;
			case JOB_KNIGHT:
				giftindex = 427;
				break;
			case JOB_HEALER:
				giftindex = 462;
				break;
			case JOB_MAGE:
				giftindex = 461;
				break;
			case JOB_ROGUE:
				giftindex = 951;
				break;
			case JOB_SORCERER:
				giftindex = 1069;
				break;
			}
			
			CItem* item = gserver.m_itemProtoList.CreateItem(giftindex, -1, 0, 0, 1);
			if (item)
			{
				if (AddToInventory(pc, item, false, true))
				{
					ItemAddMsg(rmsg, item);
					SEND_Q(rmsg, pc->m_desc);
					EventPreopenGiftMsg(rmsg, item->m_idNum);
					SEND_Q(rmsg, pc->m_desc);
					
					GAMELOG << init("EVENT PREOPEN", pc)
						<< item->m_idNum
						<< end;
					
					return ;
				}
				else
				{
					EventErrorMsg(rmsg, MSG_EVENT_ERROR_FULLINVENTORY);
					SEND_Q(rmsg, pc->m_desc);
				}
				
				delete item;
				item = NULL;
			}
			else
			{
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_INSUFF_CONDITION);
				SEND_Q(rmsg, pc->m_desc);
			}
		}
		
		if (IS_RUNNING_CONN)
		{
			ConnPreopenGiftMsg(rmsg, userindex, charindex, true);
			SEND_Q(rmsg, gserver.m_connector);
		}
	}
	else
	{
		CPC* pc = gserver.m_playerList.Find(charindex);
		if (pc == NULL)
			return ;
		
		CNetMsg rmsg;
		switch (errorcode)
		{
		case MSG_CONN_ERROR_PREOPEN_GIFT_INSUFF_CONDITION:
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_INSUFF_CONDITION);
			break;
		case MSG_CONN_ERROR_PREOPEN_GIFT_ALREADY_GIVE:
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_ALREADY_GIVE);
			break;
		default:
			return ;
		}
		SEND_Q(rmsg, pc->m_desc);
	}
}

void OnEvent2pan4panLetterRep(CNetMsg& msg)
{
#ifdef EVENT_2PAN4PAN_LETTER
#ifndef TEST_SERVER
	CNetMsg rmsg;

	int userindex = -1;
	int charindex = -1;
	int wordtype = -1;

	msg >> userindex
		>> charindex
		>> wordtype;

	GAMELOG << init("EVENT 2PAN4PAN LETTER REP")
			<< "USERINDEX" << delim
			<< userindex << delim
			<< "CHARINDEX" << delim
			<< charindex << delim
			<< "WORD TYPE" << delim
			<< wordtype
			<< end;

	// 해당 유저를 찾아서
	CPC* ch = gserver.m_playerList.Find(charindex);
	if (!ch)
	{
		// PC가 없으면 롤백 요청
		if (IS_RUNNING_CONN && wordtype >= 0 && wordtype < 5)
		{
			GAMELOG << init("EVENT 2PAN4PAN LETTER ROLLBACK REQ NOT FOUND USER")
					<< "USERINDEX" << delim
					<< userindex << delim
					<< "CHARINDEX" << delim
					<< charindex << delim
					<< "WORD TYPE" << delim
					<< wordtype
					<< end;
			ConnEvent2pan4panLetterRollbackReqMsg(rmsg, userindex, charindex, (MSG_EVENT_2PAN4PAN_WORD_TYPE)wordtype);
			SEND_Q(rmsg, gserver.m_connector);
		}
		return ;
	}

	if (wordtype < 0 || wordtype >= 5)
	{
		if (wordtype == MSG_EVENT_2PAN4PAN_WORD_ALREADY)
		{
			Event2pan4panLetterMsg(rmsg, MSG_EVENT_2PAN4PAN_WORD_ALREADY);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}

	// 아이템 검사
	extern int event2pan4panWords[5][7][2];
	int i;
	CItem* item;
	CItem* letterItems[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	for (i = 0; i < 7 && event2pan4panWords[wordtype][i][1] > 0; i++)
	{
		if (event2pan4panWords[wordtype][i][0] != -1)
		{
			bool bNotfound = false;
			int r, c;
			if (!ch->m_invenNormal.FindItem(&r, &c, event2pan4panWords[wordtype][i][0], 0, 0))
				bNotfound = true;
			else
			{
				item = ch->m_invenNormal.GetItem(r, c);
				if (!item || item->Count() < event2pan4panWords[wordtype][i][1])
					bNotfound = true;
			}

			if (bNotfound)
			{
				// 아이템 부족시
				Event2pan4panLetterMsg(rmsg, MSG_EVENT_2PAN4PAN_WORD_NOITEM);
				SEND_Q(rmsg, ch->m_desc);

				if (IS_RUNNING_CONN)
				{
					GAMELOG << init("EVENT 2PAN4PAN LETTER ROLLBACK REQ NOT FOUND USER")
							<< "USERINDEX" << delim
							<< userindex << delim
							<< "CHARINDEX" << delim
							<< charindex << delim
							<< "WORD TYPE" << delim
							<< wordtype
							<< end;
					ConnEvent2pan4panLetterRollbackReqMsg(rmsg, userindex, charindex, (MSG_EVENT_2PAN4PAN_WORD_TYPE)wordtype);
					SEND_Q(rmsg, gserver.m_connector);
				}
				return ;
			}

			letterItems[i] = item;
		}
	}

	// 아이템 감소
	for (i = 0; i < 7 && letterItems[i]; i++)
	{
		if (letterItems[i]->Count() > event2pan4panWords[wordtype][i][1])
		{
			// 수량 변경
			DecreaseFromInventory(ch, letterItems[i], event2pan4panWords[wordtype][i][1]);
			ItemUpdateMsg(rmsg, letterItems[i], -event2pan4panWords[wordtype][i][1]);
		}
		else
		{
			// 제거
			ItemDeleteMsg(rmsg, letterItems[i]);
			RemoveFromInventory(ch, letterItems[i], true, true);
		}
		SEND_Q(rmsg, ch->m_desc);
	}

	// 지급하고
	CItem* moonstone = gserver.m_itemProtoList.CreateItem(723, -1, 0, 0, 50);
	if (moonstone)
	{
		GAMELOG << init("EVENT 2PAN4PAN LETTER GIFT", ch)
				<< "WORD TYPE" << delim
				<< wordtype << delim;

		if (AddToInventory(ch, moonstone, true, true))
		{
			if (moonstone->tab() < 0)
			{
				int r, c;
				if (ch->m_invenNormal.FindItem(&r, &c, 723, 0, 0))
				{
					delete moonstone;
					moonstone = ch->m_invenNormal.GetItem(r, c);
					ItemUpdateMsg(rmsg, moonstone, 50);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
			else
			{
				ItemAddMsg(rmsg, moonstone);
				SEND_Q(rmsg, ch->m_desc);
			}

			GAMELOG << "ADD INVEN" << end;
		}
		else
		{
			ch->m_pArea->DropItem(moonstone, ch);
			moonstone->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, moonstone);
			ch->m_pArea->SendToCell(rmsg, GET_YLAYER(moonstone), moonstone->m_cellX, moonstone->m_cellZ);

			GAMELOG << "DROP" << end;
		}
	}

	// 알리기
	Event2pan4panLetterMsg(rmsg, MSG_EVENT_2PAN4PAN_WORD_OK);
	SEND_Q(rmsg, ch->m_desc);
#endif // #ifndef TEST_SERVER
#endif // #ifdef EVENT_2PAN4PAN_LETTER
}

void OnEvent2pan4panLetterRollbackRep(CNetMsg& msg)
{
#ifdef EVENT_2PAN4PAN_LETTER
#ifndef TEST_SERVER
	int userindex = -1;
	int charindex = -1;
	int wordtype = -1;

	msg >> userindex
		>> charindex
		>> wordtype;

	GAMELOG << init("EVENT 2PAN4PAN LETTER ROLLBACK REP")
			<< "USERINDEX" << delim
			<< userindex << delim
			<< "CHARINDEX" << delim
			<< charindex << delim
			<< "WORD TYPE" << delim
			<< wordtype
			<< end;
#endif // #ifndef TEST_SERVER
#endif // #ifdef EVENT_2PAN4PAN_LETTER
}

void OnEvent2pan4panGoodsRep(CNetMsg& msg)
{
#ifdef EVENT_2PAN4PAN_GOODS
	CNetMsg rmsg;

	int userindex;
	int charindex;
	int count;
	int goodsindex[20];
	int itemindex[20];
	LONGLONG itemcount[20];

	msg >> userindex
		>> charindex
		>> count;

	int i;
	for (i = 0; i < count; i++)
		msg >> goodsindex[i]
			>> itemindex[i]
			>> itemcount[i];

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (!pc)
		return ;

	if (pc->m_desc->m_event2pan4panGoods)
	{
		delete [] pc->m_desc->m_event2pan4panGoods;
		pc->m_desc->m_nEvent2pan4panGoods = 0;
	}

	pc->m_desc->m_nEvent2pan4panGoods = count;
	if (count > 0)
	{
		GAMELOG << init("EVENT 2PAN4PAN GOODS", pc)
				<< "READ GOODS" << delim
				<< "COUNT" << delim
				<< count << delim;
		pc->m_desc->m_event2pan4panGoods = new EVENT_2PAN4PAN_GOODS_DATA[count];
		for (i = 0; i < count; i++)
		{
			pc->m_desc->m_event2pan4panGoods[i].goodsindex = goodsindex[i];
			pc->m_desc->m_event2pan4panGoods[i].itemindex = itemindex[i];
			pc->m_desc->m_event2pan4panGoods[i].itemcount = itemcount[i];
			GAMELOG << "GOODS INDEX" << delim
					<< goodsindex[i] << delim
					<< "ITEM INDEX" << delim
					<< itemindex[i] << delim
					<< "ITEM COUNT" << delim
					<< itemcount[i] << delim;
		}
		GAMELOG << end;
		Event2pan4panGoodsCheckMsg(rmsg, count);
		SEND_Q(rmsg, pc->m_desc);
	}
	else
	{
		// 지급할 거 없음
		GAMELOG << init("EVENT 2PAN4PAN GOODS", pc)
				<< "NO ITEMS"
				<< end;
		Event2pan4panGoodsCheckMsg(rmsg, 0);
		SEND_Q(rmsg, pc->m_desc);
	}
#endif // #ifdef EVENT_2PAN4PAN_GOODS
}

void OnEvent2pan4panGoodsCommit(CNetMsg& msg)
{
#ifdef EVENT_2PAN4PAN_GOODS
	int userindex;
	int charindex;
	int count;
	int goodsindex[20];

	msg >> userindex
		>> charindex
		>> count;

	int i;
	for (i = 0; i < count; i++)
		msg >> goodsindex[i];

	GAMELOG << init("EVENT 2PAN4PAN GOODS")
			<< "USER INDEX" << delim
			<< userindex << delim
			<< "CHAR INDEX" << delim
			<< charindex << delim
			<< "COUNT" << delim
			<< count << delim
			<< "GOODS";

	for (i = 0; i < count; i++)
		GAMELOG << delim << goodsindex[i];

	GAMELOG << end;
#endif // #ifdef EVENT_2PAN4PAN_GOODS
}

void OnEvent2pan4panBoxRep(CNetMsg& msg)
{
#ifdef EVENT_2PAN4PAN_BOX
	CNetMsg rmsg;

	int userindex;
	int charindex;
	int count;
	int goodsindex[20];
	int itemindex[20];
	LONGLONG itemcount[20];

	msg >> userindex
		>> charindex
		>> count;

	int i;
	for (i = 0; i < count; i++)
		msg >> goodsindex[i]
			>> itemindex[i]
			>> itemcount[i];

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (!pc)
		return ;

	if (pc->m_desc->m_event2pan4panBox)
	{
		delete [] pc->m_desc->m_event2pan4panBox;
		pc->m_desc->m_nEvent2pan4panBox = 0;
	}

	pc->m_desc->m_nEvent2pan4panBox = count;
	if (count > 0)
	{
		GAMELOG << init("EVENT 2PAN4PAN BOX", pc)
				<< "READ BOX" << delim
				<< "COUNT" << delim
				<< count << delim;
		pc->m_desc->m_event2pan4panBox = new EVENT_2PAN4PAN_GOODS_DATA[count];
		for (i = 0; i < count; i++)
		{
			pc->m_desc->m_event2pan4panBox[i].goodsindex = goodsindex[i];
			pc->m_desc->m_event2pan4panBox[i].itemindex = itemindex[i];
			pc->m_desc->m_event2pan4panBox[i].itemcount = itemcount[i];
			GAMELOG << "BOX INDEX" << delim
					<< goodsindex[i] << delim
					<< "ITEM INDEX" << delim
					<< itemindex[i] << delim
					<< "ITEM COUNT" << delim
					<< itemcount[i] << delim;
		}
		GAMELOG << end;
		Event2pan4panBoxCheckMsg(rmsg, count);
		SEND_Q(rmsg, pc->m_desc);
	}
	else
	{
		// 지급할 거 없음
		GAMELOG << init("EVENT 2PAN4PAN BOX", pc)
				<< "NO ITEMS"
				<< end;
		Event2pan4panBoxCheckMsg(rmsg, 0);
		SEND_Q(rmsg, pc->m_desc);
	}
#endif // #ifdef EVENT_2PAN4PAN_BOX
}

void OnEvent2pan4panBoxCommit(CNetMsg& msg)
{
#ifdef EVENT_2PAN4PAN_BOX
	int userindex;
	int charindex;
	int count;
	int goodsindex[20];

	msg >> userindex
		>> charindex
		>> count;

	int i;
	for (i = 0; i < count; i++)
		msg >> goodsindex[i];

	GAMELOG << init("EVENT 2PAN4PAN BOX")
			<< "USER INDEX" << delim
			<< userindex << delim
			<< "CHAR INDEX" << delim
			<< charindex << delim
			<< "COUNT" << delim
			<< count << delim
			<< "BOX";

	for (i = 0; i < count; i++)
		GAMELOG << delim << goodsindex[i];

	GAMELOG << end;
#endif // #ifdef EVENT_2PAN4PAN_BOX
}

void OnEvent2pan4panMoneyRep(CNetMsg& msg)
{
#ifdef EVENT_2PAN4PAN_MONEY
	int userindex;
	int charindex;
	int itemindex;

	msg >> userindex
		>> charindex
		>> itemindex;

	GAMELOG << init("EVENT 2PAN4PAN MONEY")
			<< "USER INDEX" << delim
			<< userindex << delim
			<< "CHAR INDEX" << delim
			<< charindex << delim
			<< "ITEMINDEX" << delim
			<< itemindex
			<< end;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		CNetMsg rmsg;
		int type = 0;
		switch (itemindex)
		{
		case 1051:		type = 1;		break;
		case 1052:		type = 2;		break;
		case 1055:		type = 1;		break;
		case 1054:		type = 2;		break;
		}
		Event2pan4panMoneyRepMsg(rmsg, type);
		SEND_Q(rmsg, pc->m_desc);
	}
#endif // #ifdef EVENT_2PAN4PAN_MONEY
}

#ifdef RANKER_NOTICE
void OnRankerRep(CNetMsg& msg)
{
	int charindex, i;
	msg >> charindex;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if( pc)
	{
		pc->m_bRanker = true;

		CNetMsg rmsg;
		sprintf(g_buf, "%d", pc->m_job);
		//SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", g_buf2);
		SayMsg(rmsg, MSG_CHAT_RANKER_CONN, pc->m_index, pc->GetName(), "", g_buf);
		
		for (i = 0; i < gserver.m_playerList.m_max; i++)
		{
			if (gserver.m_playerList.m_pcList[i] )
			{
				SEND_Q(rmsg, gserver.m_playerList.m_pcList[i]->m_desc);
			}
		}

		GAMELOG << init("RANKER_CONN", pc) << end;
	}
}
#endif

#ifdef EVENT_PCBANG
void OnPcbangItemGive(CNetMsg& msg)
{
	CLCString idname(MAX_ID_NAME_LENGTH+1);
	char type = 0;

	msg >> idname
		>> type;

	CDescriptor* desc = gserver.FindConnectIdName(idname, NULL);

	if( desc && desc->m_pChar )
	{
		if ( type < 0 || type >= 4 )
		{
			GAMELOG << init("PCBANG_GIVE_ERROR_TYPE", desc->m_pChar )
					<< type << end;
			return;
		}

		// pc방 등록 아이템으로 변경해야 함
		if( !desc->m_pChar->GiveItem(1394 + type, 0, 0, 1) )
		{
			GAMELOG << init("PCBANG_ITEM_GIVE_ERROR", desc->m_pChar) << type << end;
		}
		GAMELOG << init("PCBANG_ITEM_GIVE_SUCCESS", desc->m_pChar) << type << end;

		CNetMsg rmsg;
		EventPCbangRepMsg(rmsg, 1394 + type);

		SEND_Q(rmsg, desc);
	}
	else
	{
		GAMELOG << init("PCBANG_ITEM_GIVE_ERROR_not_character") << idname << delim << type << end;
	}
}
#endif


#ifdef NEW_SERIAL_PACKAGE_EVENT
void RollBack_new( CPC* pc, int loopCount, int ItemIndex, int ItemCount )
{	
	int row, col;
	if( pc->m_invenNormal.FindItem( &row, &col, ItemIndex, 0, 0 ) )
	{
		CItem* item = pc->m_invenNormal.GetItem( row, col );
		if( item )
		{
			CNetMsg	itemMsg;

			DecreaseFromInventory( pc, item, ItemCount );

			if( item->Count() > 0 )
			{
				ItemUpdateMsg( itemMsg, item, -ItemCount );
				SEND_Q( itemMsg, pc->m_desc );
			}
			else
			{
				ItemDeleteMsg( itemMsg, item );
				SEND_Q( itemMsg, pc->m_desc );
				pc->RemoveItemFromQuickSlot( item );
				RemoveFromInventory( pc, item, true, true );
			}
		}
	}
}

void OnCouponConfirm_new( CNetMsg& msg )
{
	int charindex;
	unsigned char errorCode;
	int promotionIndex, promotionType;
	CNetMsg rmsg;

	msg >> charindex
		>> errorCode
		>> promotionIndex
		>> promotionType;

	CPC* pc = gserver.m_playerList.Find( charindex );
	
	if( pc )
	{
		if( pc->m_job > JOB_SORCERER )
		{
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_INSUFF_CONDITION );
			SEND_Q( msg, pc->m_desc );
			return;
		}

		if( errorCode == MSG_EVENT_ERROR_COUPON_SUCCESS )
		{
			int GiftItemCount;
			msg >> GiftItemCount;

			PromotionItem* pPromoItem = new PromotionItem[ GiftItemCount ];
			int RoopCount;
			for( RoopCount = 0 ; RoopCount < GiftItemCount; RoopCount++ )
			{
				msg >> pPromoItem[RoopCount].m_itemIndex
					>> pPromoItem[RoopCount].m_itemCount
					>> pPromoItem[RoopCount].m_itemPlus
					>> pPromoItem[RoopCount].m_itemFlag;
//					>> pPromoItem[RoopCount].m_itemOption0
//					>> pPromoItem[RoopCount].m_itemOption1
//					>> pPromoItem[RoopCount].m_itemOption2
//					>> pPromoItem[RoopCount].m_itemOption3
//					>> pPromoItem[RoopCount].m_itemOption4;
			}

			if( IS_RUNNING_CONN )
			{
				// inven 저장
				for( RoopCount = 0; RoopCount < GiftItemCount; RoopCount++ )
				{
					if( !pc->GiveItem( pPromoItem[RoopCount].m_itemIndex, pPromoItem[RoopCount].m_itemPlus, pPromoItem[RoopCount].m_itemFlag, pPromoItem[RoopCount].m_itemCount, false, true ) )
					{
						GAMELOG << init("SERIAL PACKAGE ITEM GIVE FAIL", pc->m_name, pc->m_desc->m_idname )
								<< pPromoItem[RoopCount].m_itemIndex;
						for( RoopCount = RoopCount - 1; RoopCount >= 0; RoopCount-- )
						{
							RollBack_new( pc, RoopCount, pPromoItem[RoopCount].m_itemIndex, pPromoItem[RoopCount].m_itemCount );
						}
						SEND_Q( msg, pc->m_desc );
						delete [] pPromoItem;
						return;
					}
				}
				// 커넥터에 성공 메세지 전송	
				ConnCouponUse_new( msg, pc->m_index, promotionIndex, promotionType, GiftItemCount, pPromoItem );
				SEND_Q( msg, gserver.m_connector );
			}
			else 
			{
				//없는  쿠폰 번호라고 클라이언트에게 보냄
				EventErrorMsg( rmsg, MSG_EVENT_ERROR_NOT_COUPON );
				SEND_Q(rmsg, pc->m_desc );
			}
			delete [] pPromoItem;
			return;
		}
		EventErrorMsg( rmsg, (MSG_EVENT_ERROR_TYPE)errorCode);
		SEND_Q( rmsg, pc->m_desc );
	}
}

void OnCouponUse_new(CNetMsg& msg)
{
	int charindex;
//	int promotionIndex;
	int promotionType;
	unsigned char errorCode;


	msg >> charindex
		>> errorCode
		>> promotionType;
		

	CPC* pc = gserver.m_playerList.Find( charindex );

	if( pc )
	{
		if( errorCode != MSG_EVENT_ERROR_COUPON_SUCCESS )
		{
			int GiftItemCount;
			msg >> GiftItemCount;
			
			PromotionItem* pPromoItem = new PromotionItem[ GiftItemCount ];
			int RoopCount;
			for( RoopCount = 0; RoopCount < GiftItemCount; RoopCount++ )
			{
				msg >> pPromoItem[RoopCount].m_itemIndex
					>> pPromoItem[RoopCount].m_itemCount
					>> pPromoItem[RoopCount].m_itemPlus
					>> pPromoItem[RoopCount].m_itemFlag;
//					>> pPromoItem[RoopCount].m_itemOption0
//					>> pPromoItem[RoopCount].m_itemOption1
//					>> pPromoItem[RoopCount].m_itemOption2
//					>> pPromoItem[RoopCount].m_itemOption3
//					>> pPromoItem[RoopCount].m_itemOption4;
			}
			// 롤백
			RollBack_new( pc, RoopCount, pPromoItem[RoopCount].m_itemIndex, pPromoItem[RoopCount].m_itemCount );

		}
		CNetMsg rmsg;
		EventErrorMsg( rmsg, (MSG_EVENT_ERROR_TYPE)errorCode );
		SEND_Q( rmsg, pc->m_desc );
	}
}
#endif //NEW_SERIAL_PACKAGE_EVENT

#ifdef EVENT_PACKAGE_ITEM
void RollBack( CPC* pc, int loopCount, int ItemIndex , int ItemCount )
{
	int row, col;
	if( pc->m_invenNormal.FindItem( &row, &col, ItemIndex, 0, 0 ) )
	{
		CItem* item = pc->m_invenNormal.GetItem( row, col );
		if( item )
		{
			CNetMsg	itemMsg;

			DecreaseFromInventory( pc, item, ItemCount );

			if( item->Count() > 0 )
			{
				ItemUpdateMsg( itemMsg, item, -ItemCount );
				SEND_Q( itemMsg, pc->m_desc );
			}
			else
			{
				ItemDeleteMsg( itemMsg, item );
				SEND_Q( itemMsg, pc->m_desc );
				pc->RemoveItemFromQuickSlot( item );
				RemoveFromInventory( pc, item, true, true );
			}
		}
	}
}

#ifdef LC_MAL
void OnCouponConfirm( CNetMsg& msg )
{
	int charindex, i;
	unsigned char errorCode;
	int cIndex, type;

	msg >> charindex
		>> errorCode
		>> cIndex	
		>> type;

	CPC* pc = gserver.m_playerList.Find(charindex);

	if( pc )
	{
		// 아이템 인덱스를 일본 의상 아이템으로
		if( pc->m_job > JOB_SORCERER)
		{
			EventErrorMsg(msg, MSG_EVENT_ERROR_INSUFF_CONDITION );
			SEND_Q(msg, pc->m_desc);
			return;
		}

		CNetMsg msg;
		if( errorCode == MSG_EVENT_ERROR_COUPON_SUCCESS )
		{
			// 2차 아이템으로
			const int ROWSIZE1 = 6;
			const int COLUMNSIZE1 = 3;
			const int ROWSIZE2 = 3;
			const int COLUMNSIZE2 = 15;

			// 10차 아이템
			const int ROWSIZE3 = 2;
			const int COLUMNSIZE3 = 3;

			
			const static int Item1_table[ROWSIZE1][COLUMNSIZE1] = {
				{ 1291, 1294, 1823 },		// 인증키 범위 1 ~ 1200
				{ 1291, 1294, 1824 },		// 인증키 범위 1201 ~ 2100
				{ 1291, 1294, 1825 },		// 인증키 범위 2101 ~ 2550
				{ 1291, 1294, 1826 },		// 인증키 범위 2551 ~ 2850
				{ 1291, 1294, 1827 },		// 인증키 범위 2851 ~ 3000 
				{ 5, 5, 1 } };				// 아이템 개수

			const static int Item2_table[ROWSIZE2][COLUMNSIZE2] = {
				{ 1288, 284, 498, 498, 498, 499, 499, 499, 500, 500, 500, 501, 501, 501, 948 },	// 아이템
				{ 1, 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1 },			// 아이템 개수
				{ 0, 0, 3, 6, 12, 3, 6, 12, 3, 6, 12, 3, 6, 12, 0 }			// 아이템 플래그
			};
			const static int Item3_table[ROWSIZE2][COLUMNSIZE2] = {
				{ 1288, 284, 498, 498, 498, 499, 499, 499, 500, 500, 500, 501, 501, 501, 949 },	// 아이템
				{ 1, 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1 },			// 아이템 개수
				{ 0, 0, 3, 6, 12, 3, 6, 12, 3, 6, 12, 3, 6, 12, 0 }			// 아이템 플래그
			};

			const static int Item4_table[ROWSIZE3][COLUMNSIZE3] = {
				{ 2039, 1291, 1294 },
				{ 1, 5, 5 }
			};
			const static int Item5_table[ROWSIZE3][COLUMNSIZE3] = {
				{ 2040, 1291, 1294 },
				{ 1, 5, 5 }
			};

			const static int TitanItem[4] = { 1446, 1447, 1448, 1449 };
			const static int KnightItem[4] = { 1450, 1451, 1452, 1453 };
			const static int HealerItem[4] = { 1454, 1455, 1456, 1457 };
			const static int MageItem[4] = { 1458, 1459, 1460, 1461 };
			const static int RougeItem[4] = { 1462, 1463, 1464, 1465 };
			const static int Sorcerer[4] = { 1466, 1467, 1468, 1469 };


			if ( type == 1 || type == 2 || type == 3 || type == 4 || type == 5 )
			{
				for ( i = 0; i < COLUMNSIZE1; i++ )
				{
					if( !pc->GiveItem(Item1_table[type-1][i], 0, 0, Item1_table[5][i], false, true ) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
								<< Item1_table[type-1][i] << end;
						//RollBack
						for( i = i-1; i >= 0; i-- )
						{
							RollBack( pc, i, Item1_table[type-1][i] , Item1_table[5][i] );
						}
						SEND_Q( msg, pc->m_desc );
						return;
					}
				}	
			}
			else if ( type == 6 )
			{
				for ( i = 0; i < COLUMNSIZE2; i++ )
				{
					if( !pc->GiveItem(Item2_table[0][i], 0, Item2_table[2][i], Item2_table[1][i], false, true ) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
								<< Item2_table[1][i] << end;
						//RollBack
						for( i = i-1; i >= 0; i-- )
						{
							RollBack( pc, i, Item2_table[0][i] , Item2_table[1][i] );
						}
						SEND_Q( msg, pc->m_desc );
						return;
					}

				}
			}
			else if ( type == 7 )
			{
				for ( i = 0; i < COLUMNSIZE2; i++ )
				{
					if( !pc->GiveItem(Item3_table[0][i], 0, Item3_table[2][i], Item3_table[1][i], false, true ) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
								<< Item3_table[1][i] << end;
						//RollBack
						for( i = i-1; i >= 0; i-- )
						{
							RollBack( pc, i, Item3_table[0][i], Item3_table[1][i] );
						}
						SEND_Q( msg, pc->m_desc );
						return;
					}
				}
			}
			else if ( type == 8 )
			{
				if( !pc->GiveItem( 852, 0 ,0, 1 , false, true ) )
				{
					GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
							<< 852 << end;
					RollBack( pc, 1, 852 , 1);
					SEND_Q(msg, pc->m_desc);
					return;
				}
			}
			else if ( type == 9 )
			{
				if( !pc->GiveItem( 85, 0 ,0, 1 , false, true ) )
				{
					GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
							<< 85 << end;
					RollBack( pc, 1, 85, 1 );
					SEND_Q(msg, pc->m_desc);
					return;
				}

			}
			else if ( type == 10 )
			{
				for ( i = 0; i < COLUMNSIZE3; i++ )
				{
					if( !pc->GiveItem(Item4_table[0][i], 0, 0, Item4_table[1][i], false, false ) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
								<< Item4_table[1][i] << end;
						//RollBack
						for( i = i-1; i >= 0; i-- )
						{
							RollBack( pc, i, Item4_table[0][i], Item4_table[1][i]);
						}
						SEND_Q( msg, pc->m_desc );
						return;
					}
				}
			}
			else if ( type == 11 )
			{
				for ( i = 0; i < COLUMNSIZE3; i++ )
				{
					if( !pc->GiveItem(Item5_table[0][i], 0, 0, Item5_table[1][i], false, false) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
								<< Item5_table[1][i] << end;
						//RollBack
						for( i = i-1; i >= 0; i-- )
						{
							RollBack( pc, i, Item5_table[0][i], Item5_table[1][i]);
						}
						SEND_Q( msg, pc->m_desc );
						return;
					}
				}
			}
			else if ( type == 12 )
			{
				for ( i = 0; i < 4; i++ )
				{
					if( !pc->GiveItem(TitanItem[i], 0, 0, 1, false, true ) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
								<< TitanItem[i] << end;
						//RollBack
						for( i = i-1; i >= 0; i-- )
						{
							RollBack( pc, i, TitanItem[i], 1);
						}
						SEND_Q( msg, pc->m_desc );
						return;
					}
				}
			}
			else if ( type == 13 )
			{
				for ( i = 0; i < 4; i++ )
				{
					if( !pc->GiveItem(KnightItem[i], 0, 0, 1, false, true ) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
								<< KnightItem[i] << end;
						//RollBack
						for( i = i-1; i >= 0; i-- )
						{
							RollBack( pc, i, KnightItem[i] ,1);
						}
						SEND_Q( msg, pc->m_desc );
						return;
					}
				}
			}
			else if ( type == 14 )
			{
				for ( i = 0; i < 4; i++ )
				{
					if( !pc->GiveItem(HealerItem[i], 0, 0, 1, false, true ) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
								<< HealerItem[i] << end;
						//RollBack
						for( i = i-1; i >= 0; i-- )
						{
							RollBack( pc, i, HealerItem[i], 1);
						}
						SEND_Q( msg, pc->m_desc );
						return;
					}
				}
			}
			else if ( type == 15 )
			{
				for ( i = 0; i < 4; i++ )
				{
					if( !pc->GiveItem(MageItem[i], 0, 0, 1, false, true ) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
								<< MageItem[i] << end;
						//RollBack
						for( i = i-1; i >= 0; i-- )
						{
							RollBack( pc, i, MageItem[i], 1);
						}
						SEND_Q( msg, pc->m_desc );
						return;
					}
				}
			}
			else if ( type == 16 )
			{
				for ( i = 0; i < 4; i++ )
				{
					if( !pc->GiveItem(RougeItem[i], 0, 0, 1, false, true ) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
								<< RougeItem[i] << end;
						//RollBack
						for( i = i-1; i >= 0; i-- )
						{
							RollBack( pc, i, RougeItem[i], 1);
						}
						SEND_Q( msg, pc->m_desc );
						return;
					}
				}
			}
			else if ( type == 17 )
			{
				for ( i = 0; i < 4; i++ )
				{
					if( !pc->GiveItem(Sorcerer[i], 0, 0, 1, false, true ) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname )
								<< Sorcerer[i] << end;
						//RollBack
						for( i = i-1; i >= 0; i-- )
						{
							RollBack( pc, i, Sorcerer[i], 1);
						}
						SEND_Q( msg, pc->m_desc );
						return;
					}
				}
			}
			else if ( type == 18 )
			{
				if( !pc->GiveItem(85, 0, 0, 5, false, true ) )
				{
					GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL type-18", pc->m_name, pc->m_desc->m_idname )
							<< 85 << end;

					RollBack( pc, 1, 85, 5);
					SEND_Q( msg, pc->m_desc );
					return;
				}				
			}
			else if ( type == 19 )
			{
				if( !pc->GiveItem( 2858, 0 ,0, 1 , false, true ) )
				{
					GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL type-19", pc->m_name, pc->m_desc->m_idname )
							<< 2858 << end;
					RollBack( pc, 1, 2858, 1);
					SEND_Q(msg, pc->m_desc);
					return;
				}

				if( !pc->GiveItem( 2859, 0 ,0, 1 , false, true ) )
				{
					GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL type-19", pc->m_name, pc->m_desc->m_idname )
							<< 2859 << end;
					RollBack( pc, 1, 2859, 1);
					SEND_Q(msg, pc->m_desc);
					return;
				}
			}


			if( IS_RUNNING_CONN)
			{
#ifdef EVENT_PACKAGE_ITEM_DBLOG
				ConnCouponUse(msg, pc->m_index, cIndex, type, pc->m_desc->m_idname, pc->GetName());
#else
				ConnCouponUse(msg, pc->m_index, cIndex, type);
#endif

				SEND_Q(msg, gserver.m_connector);
			}
			else
			{
		
				if ( type == 1 || type == 2 || type == 3 || type == 4 || type == 5 )
				{				
					for( i = i-1; i >= 0; i-- )
					{
						RollBack( pc, i, Item1_table[type-1][i], Item1_table[5][i] );						
					}
				}
				else if ( type == 6 )
				{
					for( i = i-1; i >= 0; i-- )
					{
						RollBack( pc, i, Item2_table[0][i], Item2_table[1][i] );
					}
				}
				else if ( type == 7 )
				{
					for( i = i-1; i >= 0; i-- )
					{
						RollBack( pc, i, Item3_table[0][i], Item3_table[1][i] );						
					}
				}
				else if ( type == 8 )
				{
					RollBack( pc, 1, 852, 1);
				
				}
				else if ( type == 9 )
				{
					RollBack( pc, 1, 85, 1);
				}
				else if ( type == 10 )
				{
					for( i = i-1; i >= 0; i-- )
					{
						RollBack( pc, i, Item4_table[0][i], Item4_table[1][i] );						
					}
				}
				else if ( type == 11 )
				{
					for( i = i-1; i >= 0; i-- )
					{
						RollBack( pc, i, Item5_table[0][i], Item5_table[1][i]);						
					}
				}
				else if ( type == 18 )
				{
					RollBack( pc, 1, 85, 5);						
				}


				// 지급한 아이템을 롤백 하고 없는 쿠폰번호라고 클라이언트에게 보냄
				EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
				SEND_Q(msg, pc->m_desc);
			}
			return;
		}

		// 실패이면 에러코드를 그대로 클라이언트에게 전달
		EventErrorMsg(msg, (MSG_EVENT_ERROR_TYPE) errorCode );
		SEND_Q(msg, pc->m_desc);
	}
}

void OnCouponUse(CNetMsg& msg)
{
	int charindex, i, type;
	unsigned char errorCode;

	msg >> charindex
		>> errorCode
		>> type;

	CPC* pc = gserver.m_playerList.Find(charindex);

	if( pc )
	{
		const int ROWSIZE1 = 6;
		const int COLUMNSIZE1 = 3;
		const int ROWSIZE2 = 3;
		const int COLUMNSIZE2 = 15;

		
		// 10차 아이템
		const int ROWSIZE3 = 2;
		const int COLUMNSIZE3 = 3;

			
		const static int Item1_table[ROWSIZE1][COLUMNSIZE1] = {
				{ 1291, 1294, 1823 },		// 인증키 범위 1 ~ 1200
				{ 1291, 1294, 1824 },		// 인증키 범위 1201 ~ 2100
				{ 1291, 1294, 1825 },		// 인증키 범위 2101 ~ 2550
				{ 1291, 1294, 1826 },		// 인증키 범위 2551 ~ 2850
				{ 1291, 1294, 1827 },		// 인증키 범위 2851 ~ 3000 
				{ 5, 5, 1 } };				// 아이템 개수

			const static int Item2_table[ROWSIZE2][COLUMNSIZE2] = {
				{ 1288, 284, 498, 498, 498, 499, 499, 499, 500, 500, 500, 501, 501, 501, 948 },	// 아이템
				{ 1, 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1 },			// 아이템 개수
				{ 0, 0, 3, 6, 12, 3, 6, 12, 3, 6, 12, 3, 6, 12, 0 }			// 아이템 플래그
			};
			const static int Item3_table[ROWSIZE2][COLUMNSIZE2] = {
				{ 1288, 284, 498, 498, 498, 499, 499, 499, 500, 500, 500, 501, 501, 501, 949 },	// 아이템
				{ 1, 10, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1 },			// 아이템 개수
				{ 0, 0, 3, 6, 12, 3, 6, 12, 3, 6, 12, 3, 6, 12, 0 }			// 아이템 플래그
			};

			const static int Item4_table[ROWSIZE3][COLUMNSIZE3] = {
				{ 2039, 1291, 1294 },
				{ 1, 5, 5 },
			};
			const static int Item5_table[ROWSIZE3][COLUMNSIZE3] = {
				{ 2040, 1291, 1294 },
				{ 1, 5, 5 },
			};

			const static int TitanItem[4] = { 1446, 1447, 1448, 1449 };
			const static int KnightItem[4] = { 1450, 1451, 1452, 1453 };
			const static int HealerItem[4] = { 1454, 1455, 145, 1457 };
			const static int MageItem[4] = { 1458, 1459, 1460, 1461 };
			const static int RougeItem[4] = { 1462, 1463, 1464, 1465 };
			const static int Sorcerer[4] = { 1466, 1467, 1468, 1469 };


		if( errorCode != MSG_EVENT_ERROR_COUPON_SUCCESS )
		{
			// 롤백

				if ( type == 1 || type == 2 || type == 3 || type == 4 || type == 5 )
				{				
					for( i = 0; i < COLUMNSIZE1; i++ )
					{
						RollBack( pc, i, Item1_table[type-1][i], Item1_table[5][i]);						
					}
				}
				else if ( type == 6 )
				{
					for( i = 0; i < COLUMNSIZE2 ; i++ )
					{
						RollBack( pc, i, Item2_table[0][i], Item2_table[1][i]);
					}
				}
				else if ( type == 7 )
				{
					for( i = 0; i < COLUMNSIZE2 ; i++ )
					{
						RollBack( pc, i, Item3_table[0][i], Item3_table[1][i]);						
					}
				}
				else if ( type == 8 )
				{
					RollBack( pc, 1, 852, 1);
				
				}
				else if ( type == 9 )
				{
					RollBack( pc, 1, 85, 1);
				}
				else if ( type == 10 )
				{
					for( i = 0; i < COLUMNSIZE3; i++ )
					{
						RollBack( pc, i, Item4_table[0][i], Item4_table[1][i]);						
					}
				}
				else if ( type == 11 )
				{
					for( i = 0; i < COLUMNSIZE3; i++ )
					{
						RollBack( pc, i, Item5_table[0][i], Item5_table[1][i]);						
					}
				}
				else if ( type == 12 )
				{
					for ( i = 0; i < 4; i++ )
					{
						RollBack( pc, i, TitanItem[i], 1);						
					}
				}
				else if ( type == 13 )
				{
					for ( i = 0; i < 4; i++ )
					{			
						RollBack( pc, i, KnightItem[i], 1);
					}
				}
				else if ( type == 14 )
				{
					for ( i = 0; i < 4; i++ )
					{
						RollBack( pc, i, HealerItem[i], 1);					
					}
				}
				else if ( type == 15 )
				{
					for ( i = 0; i < 4; i++ )
					{
						RollBack( pc, i, MageItem[i], 1);						
					}
				}
				else if ( type == 16 )
				{
					for ( i = 0; i < 4; i++ )
					{
						RollBack( pc, i, RougeItem[i], 1);
					}
				}
				else if ( type == 17 )
				{
					for ( i = 0; i < 4; i++ )
					{
						RollBack( pc, i, Sorcerer[i], 1);							
					}
				}
				else if ( type == 18 )
				{									
					RollBack( pc, 1, 85, 5);					
				}
		}
/*
		GAMELOG << init("COUPON_ITEM_GIVE", pc->m_name, pc->m_desc->m_idname)
				<< "TYPE" << delim << type
				<< "ERROR_CODE" << delim << errorCode << end;	*/
		CNetMsg msg;
		EventErrorMsg(msg, (MSG_EVENT_ERROR_TYPE) errorCode);

		SEND_Q(msg, pc->m_desc);
	}
}
#elif defined( LC_HBK )
void OnCouponConfirm( CNetMsg& msg )
{
	int charindex, i, k;
	unsigned char errorCode;
	int cIndex, type;

	msg >> charindex
		>> errorCode
		>> cIndex
		>> type;

	CPC* pc = gserver.m_playerList.Find(charindex);

	if( pc )
	{
		// 아이템 인덱스를 일본 의상 아이템으로
		if( pc->m_job > JOB_SORCERER)
		{
			EventErrorMsg(msg, MSG_EVENT_ERROR_INSUFF_CONDITION );
			SEND_Q(msg, pc->m_desc);
			return;
		}

		CNetMsg msg;
		if( errorCode == MSG_EVENT_ERROR_COUPON_SUCCESS )
		{
			const static int Item_Table[2][4]  = {
				{ 1282, 19, 47, 1313 },			// item index
				{ 5, 5000, 10, 1} };			// item count

			const static int Item2_Table[2][6] = {
				{ 841, 85, 44, 485, 391, 786 },			// item index
				{ 1, 10, 30, 30, 30, 10 } };			// item count
				
			const static int Item3_Table[2][4] = {
				{ 46, 19, 1057, 972	},		// item index
				{ 50, 5000, 10, 6 } };		// item count
					
			const static int Item4_Table[3][3] = {
				{ 498, 499, 779 },			// item index
				{ 6, 6, 0 },				// item flag(level)
				{ 20, 20, 5 } };			// item count

			const static int Item5_Table[2][11] = {
				{ 1706, 1707, 1416, 85, 1295, 2356, 2357, 2348, 2358, 779 },		// item index
				{ 1, 1, 1, 1, 1, 10, 10, 2, 2, 1}	};									// item count				
// << kjban 080121
			const static int Item6_Table[2][12] = { 
				{ 2356, 2357, 2141, 2394, 2395, 2396, 2397, 2398, 2399, 2400, 2401, 2402 },	// item index 
				{ 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };									// item count
// >>

			const static int Item5_RandomTable1[3][3] = {
				{882, 883, 884},
				{5, 5, 5},
				{40, 40, 20}
			};

			const static int Item5_RandomTable2[3][5] = {
				{1830, 1833, 1834, 1835, 1836},
				{1, 1, 1, 1, 1},
				{30, 30, 12, 8, 20}
			};

			int randomtable[2][2];
			memset(randomtable, -1, sizeof(randomtable));

			if( type == 1)
			{
				for( i = 0; i < 4; i++ )
				{
					// 성공이면 아이템을 지급하고 커넥터에게 컨펌을 보냄
					if( !pc->GiveItem(Item_Table[0][i], 0, 0, Item_Table[1][i], false) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
								<< Item_Table[0][i] << delim 
								<< Item_Table[1][i] << end;
						// 롤백 
						int r, c;
						for(i = i-1; i >= 0; i--)
						{
							if( pc->m_invenNormal.FindItem(&r, &c, Item_Table[0][i], 0 , 0) )
							{
								CItem* item = pc->m_invenNormal.GetItem(r, c);
								if( item )
								{
									CNetMsg itemMsg;

									// Item 수량 변경
									DecreaseFromInventory(pc, item, Item_Table[1][i]);
									
									if (item->Count() > 0)
									{
										ItemUpdateMsg(itemMsg, item, -Item_Table[1][i]);
										SEND_Q(itemMsg, pc->m_desc);
									}
									else
									{
										ItemDeleteMsg(itemMsg, item);
										SEND_Q(itemMsg, pc->m_desc);
										pc->RemoveItemFromQuickSlot(item);
										RemoveFromInventory(pc, item, true, true);
									}
								}
							}
						}

						//EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
						SEND_Q(msg, pc->m_desc);
						return;
					}
					
					GAMELOG << init("PACKAGE_ITEM_GIVE_SUCCESS", pc->m_name, pc->m_desc->m_idname)
							<< Item_Table[0][i] << delim 
							<< Item_Table[1][i] << end;
				}
			}
			else if(type == 2)
			{
				for( i = 0; i < 6; i++ )
				{
					// 성공이면 아이템을 지급하고 커넥터에게 컨펌을 보냄
					if( !pc->GiveItem(Item2_Table[0][i], 0, 0, Item2_Table[1][i], false) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
								<< Item2_Table[0][i] << delim 
								<< Item2_Table[1][i] << end;
						// 롤백 
						int r, c;
						for(i = i-1; i >= 0; i--)
						{
							if( pc->m_invenNormal.FindItem(&r, &c, Item2_Table[0][i], 0 , 0) )
							{
								CItem* item = pc->m_invenNormal.GetItem(r, c);
								if( item )
								{
									CNetMsg itemMsg;

									// Item 수량 변경
									DecreaseFromInventory(pc, item, Item2_Table[1][i]);
									
									if (item->Count() > 0)
									{
										ItemUpdateMsg(itemMsg, item, -Item2_Table[1][i]);
										SEND_Q(itemMsg, pc->m_desc);
									}
									else
									{
										ItemDeleteMsg(itemMsg, item);
										SEND_Q(itemMsg, pc->m_desc);
										pc->RemoveItemFromQuickSlot(item);
										RemoveFromInventory(pc, item, true, true);
									}
								}
							}
						}

						//EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
						SEND_Q(msg, pc->m_desc);
						return;
					}
					
					GAMELOG << init("PACKAGE_ITEM_GIVE_SUCCESS", pc->m_name, pc->m_desc->m_idname)
							<< Item2_Table[0][i] << delim 
							<< Item2_Table[1][i] << end;
				}
			}
			else if (type == 3)
			{
				for( i = 0; i < 4; i++ )
				{
					// 성공이면 아이템을 지급하고 커넥터에게 컨펌을 보냄
					if( !pc->GiveItem(Item3_Table[0][i], 0, 0, Item3_Table[1][i], false) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
								<< Item3_Table[0][i] << delim 
								<< Item3_Table[1][i] << end;
						// 롤백 
						int r, c;
						for(i = i-1; i >= 0; i--)
						{
							if( pc->m_invenNormal.FindItem(&r, &c, Item3_Table[0][i], 0 , 0) )
							{
								CItem* item = pc->m_invenNormal.GetItem(r, c);
								if( item )
								{
									CNetMsg itemMsg;

									// Item 수량 변경
									DecreaseFromInventory(pc, item, Item3_Table[1][i]);
									
									if (item->Count() > 0)
									{
										ItemUpdateMsg(itemMsg, item, -Item3_Table[1][i]);
										SEND_Q(itemMsg, pc->m_desc);
									}
									else
									{
										ItemDeleteMsg(itemMsg, item);
										SEND_Q(itemMsg, pc->m_desc);
										pc->RemoveItemFromQuickSlot(item);
										RemoveFromInventory(pc, item, true, true);
									}
								}
							}
						}

						//EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
						SEND_Q(msg, pc->m_desc);
						return;
					}
					
					GAMELOG << init("PACKAGE_ITEM_GIVE_SUCCESS", pc->m_name, pc->m_desc->m_idname)
							<< Item3_Table[0][i] << delim 
							<< Item3_Table[1][i] << end;
				}
			}
			else if (type == 4)
			{
				for( i = 0; i < 3; i++ )
				{
					// 성공이면 아이템을 지급하고 커넥터에게 컨펌을 보냄
					if( !pc->GiveItem(Item4_Table[0][i], 0, Item4_Table[1][i], Item4_Table[2][i], false) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
								<< Item4_Table[0][i] << delim 
								<< Item4_Table[1][i] << delim
								<< Item4_Table[2][i] << end;
						// 롤백 
						int r, c;
						for(i = i-1; i >= 0; i--)
						{
							if( pc->m_invenNormal.FindItem(&r, &c, Item4_Table[0][i], 0 , Item4_Table[1][i]) )
							{
								CItem* item = pc->m_invenNormal.GetItem(r, c);
								if( item )
								{
									CNetMsg itemMsg;

									// Item 수량 변경
									DecreaseFromInventory(pc, item, Item4_Table[2][i]);
									
									if (item->Count() > 0)
									{
										ItemUpdateMsg(itemMsg, item, -Item4_Table[2][i]);
										SEND_Q(itemMsg, pc->m_desc);
									}
									else
									{
										ItemDeleteMsg(itemMsg, item);
										SEND_Q(itemMsg, pc->m_desc);
										pc->RemoveItemFromQuickSlot(item);
										RemoveFromInventory(pc, item, true, true);
									}
								}
							}
						}

						//EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
						SEND_Q(msg, pc->m_desc);
						return;
					}
					
					GAMELOG << init("PACKAGE_ITEM_GIVE_SUCCESS", pc->m_name, pc->m_desc->m_idname)
							<< Item4_Table[0][i] << delim 
							<< Item4_Table[1][i] << delim
							<< Item4_Table[2][i] << end;
				}
			}
			else if (type == 5)
			{
				int rand, idx, total;
				rand = GetRandom(1, 100);

				total = 0;
				for(idx = 0; idx < 3; idx++)
				{
					total += Item5_RandomTable1[2][idx];
					if(rand <= total)
					{
						randomtable[0][0] = Item5_RandomTable1[0][idx];
						randomtable[0][1] = Item5_RandomTable1[1][idx];
						break;
					}
				}

				total = 0;
				for(idx = 0; idx < 5; idx++)
				{
					total += Item5_RandomTable2[2][idx];
					if(rand <= total)
					{
						randomtable[1][0] = Item5_RandomTable2[0][idx];
						randomtable[1][1] = Item5_RandomTable2[1][idx];
						break;
					}
				}

				for( k = 0; k < 2 ; k++)
				{
					if( !pc->GiveItem(randomtable[k][0], 0, 0, randomtable[k][1], false) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
								<< randomtable[k][0] << delim 
								<< randomtable[k][1] << end;
						// 롤백 
						int r, c;
						for(k = k-1; k >= 0; k--)
						{
							if( pc->m_invenNormal.FindItem(&r, &c, randomtable[k][0], 0 , 0) )
							{
								CItem* item = pc->m_invenNormal.GetItem(r, c);
								if( item )
								{
									CNetMsg itemMsg;

									// Item 수량 변경
									DecreaseFromInventory(pc, item, randomtable[k][1]);
									
									if (item->Count() > 0)
									{
										ItemUpdateMsg(itemMsg, item, -randomtable[k][1]);
										SEND_Q(itemMsg, pc->m_desc);
									}
									else
									{
										ItemDeleteMsg(itemMsg, item);
										SEND_Q(itemMsg, pc->m_desc);
										pc->RemoveItemFromQuickSlot(item);
										RemoveFromInventory(pc, item, true, true);
									}
								}
							}
						}

						//EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
						SEND_Q(msg, pc->m_desc);
						return;
					}
					
					GAMELOG << init("PACKAGE_ITEM_GIVE_SUCCESS", pc->m_name, pc->m_desc->m_idname)
							<< randomtable[k][0] << delim 
							<< randomtable[k][1] << end;
				}

				for( i = 0; i < 10; i++ )
				{

					// 성공이면 아이템을 지급하고 커넥터에게 컨펌을 보냄
					if( !pc->GiveItem(Item5_Table[0][i], 0, 0, Item5_Table[1][i], false) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
								<< Item5_Table[0][i] << delim 
								<< Item5_Table[1][i] << end;
						// 롤백 
						int r, c;
						for(i = i-1; i >= 0; i--)
						{
							if( pc->m_invenNormal.FindItem(&r, &c, Item5_Table[0][i], 0 , 0) )
							{
								CItem* item = pc->m_invenNormal.GetItem(r, c);
								if( item )
								{
									CNetMsg itemMsg;

									// Item 수량 변경
									DecreaseFromInventory(pc, item, Item5_Table[1][i]);
									
									if (item->Count() > 0)
									{
										ItemUpdateMsg(itemMsg, item, -Item5_Table[1][i]);
										SEND_Q(itemMsg, pc->m_desc);
									}
									else
									{
										ItemDeleteMsg(itemMsg, item);
										SEND_Q(itemMsg, pc->m_desc);
										pc->RemoveItemFromQuickSlot(item);
										RemoveFromInventory(pc, item, true, true);
									}
								}
							}
						}

						for(k = k-1; k >= 0; k--)
						{
							if( pc->m_invenNormal.FindItem(&r, &c, randomtable[k][0], 0 , 0) )
							{
								CItem* item = pc->m_invenNormal.GetItem(r, c);
								if( item )
								{
									CNetMsg itemMsg;

									// Item 수량 변경
									DecreaseFromInventory(pc, item, randomtable[k][1]);
									
									if (item->Count() > 0)
									{
										ItemUpdateMsg(itemMsg, item, -randomtable[k][1]);
										SEND_Q(itemMsg, pc->m_desc);
									}
									else
									{
										ItemDeleteMsg(itemMsg, item);
										SEND_Q(itemMsg, pc->m_desc);
										pc->RemoveItemFromQuickSlot(item);
										RemoveFromInventory(pc, item, true, true);
									}
								}
							}
						}

						//EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
						SEND_Q(msg, pc->m_desc);
						return;
					}
					
					GAMELOG << init("PACKAGE_ITEM_GIVE_SUCCESS", pc->m_name, pc->m_desc->m_idname)
							<< Item5_Table[0][i] << delim 
							<< Item5_Table[1][i] << end;
				}
			}
// << kjban 080121
			else if(type == 6)
			{
				for( i = 0; i < 12; i++ )
				{
					// 성공이면 아이템을 지급하고 커넥터에게 컨펌을 보냄
					if( !pc->GiveItem(Item6_Table[0][i], 0, 0, Item6_Table[1][i], false) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
								<< Item6_Table[0][i] << delim 
								<< Item6_Table[1][i] << end;
						// 롤백 
						int r, c;
						for(i = i-1; i >= 0; i--)
						{
							if( pc->m_invenNormal.FindItem(&r, &c, Item6_Table[0][i], 0 , 0) )
							{
								CItem* item = pc->m_invenNormal.GetItem(r, c);
								if( item )
								{
									CNetMsg itemMsg;

									// Item 수량 변경
									DecreaseFromInventory(pc, item, Item6_Table[1][i]);
									
									if (item->Count() > 0)
									{
										ItemUpdateMsg(itemMsg, item, -Item6_Table[1][i]);
										SEND_Q(itemMsg, pc->m_desc);
									}
									else
									{
										ItemDeleteMsg(itemMsg, item);
										SEND_Q(itemMsg, pc->m_desc);
										pc->RemoveItemFromQuickSlot(item);
										RemoveFromInventory(pc, item, true, true);
									}
								}
							}
						}

						SEND_Q(msg, pc->m_desc);
						return;
					}
					
					GAMELOG << init("PACKAGE_ITEM_GIVE_SUCCESS", pc->m_name, pc->m_desc->m_idname)
							<< Item6_Table[0][i] << delim 
							<< Item6_Table[1][i] << end;
				}
			}
// >>

			if( IS_RUNNING_CONN)
			{
#ifdef EVENT_PACKAGE_ITEM_DBLOG
				ConnCouponUse(msg, pc->m_index, cIndex, type, pc->m_desc->m_idname, pc->GetName(), randomtable[0][0], randomtable[0][1], randomtable[1][0], randomtable[1][1]);
#else
				ConnCouponUse(msg, pc->m_index, cIndex, type, randomtable[0][0], randomtable[0][1], randomtable[1][0], randomtable[1][1]);
#endif
				SEND_Q(msg, gserver.m_connector);
			}
			else
			{
				// 롤백 
				int r, c;
				if(type == 1)
				{
					for(i = i-1; i >= 0; i--)
					{
						if( pc->m_invenNormal.FindItem(&r, &c, Item_Table[0][i], 0 , 0) )
						{
							CItem* item = pc->m_invenNormal.GetItem(r, c);
							if( item )
							{
								CNetMsg itemMsg;

								// Item 수량 변경
								DecreaseFromInventory(pc, item, Item_Table[1][i]);
								
								if (item->Count() > 0)
								{
									ItemUpdateMsg(itemMsg, item, -Item_Table[1][i]);
									SEND_Q(itemMsg, pc->m_desc);
								}
								else
								{
									ItemDeleteMsg(itemMsg, item);
									SEND_Q(itemMsg, pc->m_desc);
									pc->RemoveItemFromQuickSlot(item);
									RemoveFromInventory(pc, item, true, true);
								}
							}
						}
					}
				}
				else if(type == 2)
				{
					for(i = i-1; i >= 0; i--)
					{
						if( pc->m_invenNormal.FindItem(&r, &c, Item2_Table[0][i], 0 , 0) )
						{
							CItem* item = pc->m_invenNormal.GetItem(r, c);
							if( item )
							{
								CNetMsg itemMsg;

								// Item 수량 변경
								DecreaseFromInventory(pc, item, Item2_Table[1][i]);
								
								if (item->Count() > 0)
								{
									ItemUpdateMsg(itemMsg, item, -Item2_Table[1][i]);
									SEND_Q(itemMsg, pc->m_desc);
								}
								else
								{
									ItemDeleteMsg(itemMsg, item);
									SEND_Q(itemMsg, pc->m_desc);
									pc->RemoveItemFromQuickSlot(item);
									RemoveFromInventory(pc, item, true, true);
								}
							}
						}
					}
				}
				else if(type == 3)
				{
					for(i = i-1; i >= 0; i--)
					{
						if( pc->m_invenNormal.FindItem(&r, &c, Item3_Table[0][i], 0 , 0) )
						{
							CItem* item = pc->m_invenNormal.GetItem(r, c);
							if( item )
							{
								CNetMsg itemMsg;

								// Item 수량 변경
								DecreaseFromInventory(pc, item, Item3_Table[1][i]);
								
								if (item->Count() > 0)
								{
									ItemUpdateMsg(itemMsg, item, -Item3_Table[1][i]);
									SEND_Q(itemMsg, pc->m_desc);
								}
								else
								{
									ItemDeleteMsg(itemMsg, item);
									SEND_Q(itemMsg, pc->m_desc);
									pc->RemoveItemFromQuickSlot(item);
									RemoveFromInventory(pc, item, true, true);
								}
							}
						}
					}
				}
				else if(type == 4)
				{
					for(i = i-1; i >= 0; i--)
					{
						if( pc->m_invenNormal.FindItem(&r, &c, Item4_Table[0][i], 0 , Item4_Table[1][i]) )
						{
							CItem* item = pc->m_invenNormal.GetItem(r, c);
							if( item )
							{
								CNetMsg itemMsg;

								// Item 수량 변경
								DecreaseFromInventory(pc, item, Item4_Table[2][i]);
								
								if (item->Count() > 0)
								{
									ItemUpdateMsg(itemMsg, item, -Item4_Table[2][i]);
									SEND_Q(itemMsg, pc->m_desc);
								}
								else
								{
									ItemDeleteMsg(itemMsg, item);
									SEND_Q(itemMsg, pc->m_desc);
									pc->RemoveItemFromQuickSlot(item);
									RemoveFromInventory(pc, item, true, true);
								}
							}
						}
					}
				}
				else if(type == 5)
				{
					for(i = i-1; i >= 0; i--)
					{
						if( pc->m_invenNormal.FindItem(&r, &c, Item5_Table[0][i], 0 , 0) )
						{
							CItem* item = pc->m_invenNormal.GetItem(r, c);
							if( item )
							{
								CNetMsg itemMsg;

								// Item 수량 변경
								DecreaseFromInventory(pc, item, Item5_Table[1][i]);
								
								if (item->Count() > 0)
								{
									ItemUpdateMsg(itemMsg, item, -Item5_Table[1][i]);
									SEND_Q(itemMsg, pc->m_desc);
								}
								else
								{
									ItemDeleteMsg(itemMsg, item);
									SEND_Q(itemMsg, pc->m_desc);
									pc->RemoveItemFromQuickSlot(item);
									RemoveFromInventory(pc, item, true, true);
								}
							}
						}
					}
					
					for(k = k-1; k >= 0; k--)
					{
						if( pc->m_invenNormal.FindItem(&r, &c, randomtable[k][0], 0 , 0) )
						{
							CItem* item = pc->m_invenNormal.GetItem(r, c);
							if( item )
							{
								CNetMsg itemMsg;
								
								// Item 수량 변경
								DecreaseFromInventory(pc, item, randomtable[k][1]);
								
								if (item->Count() > 0)
								{
									ItemUpdateMsg(itemMsg, item, -randomtable[k][1]);
									SEND_Q(itemMsg, pc->m_desc);
								}
								else
								{
									ItemDeleteMsg(itemMsg, item);
									SEND_Q(itemMsg, pc->m_desc);
									pc->RemoveItemFromQuickSlot(item);
									RemoveFromInventory(pc, item, true, true);
								}
							}
						}
					}

				}
// << kjban 080121
				else if(type == 6)
				{
					for(i = i-1; i >= 0; i--)
					{
						if( pc->m_invenNormal.FindItem(&r, &c, Item6_Table[0][i], 0 , 0) )
						{
							CItem* item = pc->m_invenNormal.GetItem(r, c);
							if( item )
							{
								CNetMsg itemMsg;

								// Item 수량 변경
								DecreaseFromInventory(pc, item, Item6_Table[1][i]);
								
								if (item->Count() > 0)
								{
									ItemUpdateMsg(itemMsg, item, -Item6_Table[1][i]);
									SEND_Q(itemMsg, pc->m_desc);
								}
								else
								{
									ItemDeleteMsg(itemMsg, item);
									SEND_Q(itemMsg, pc->m_desc);
									pc->RemoveItemFromQuickSlot(item);
									RemoveFromInventory(pc, item, true, true);
								}
							}
						}
					}
				}
// >>

				// 지급한 아이템을 롤백 하고 없는 쿠폰번호라고 클라이언트에게 보냄
				EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
				SEND_Q(msg, pc->m_desc);
			}

			return;
		}

		// 실패이면 에러코드를 그대로 클라이언트에게 전달
		EventErrorMsg(msg, (MSG_EVENT_ERROR_TYPE) errorCode );
		SEND_Q(msg, pc->m_desc);
	}
}

void OnCouponUse(CNetMsg& msg)
{
	int charindex, i, r,c, type;
	unsigned char errorCode;
	int randomtable[2][2];

	memset(randomtable, -1, sizeof(randomtable));

	msg >> charindex
		>> errorCode
		>> type
		>> randomtable[0][0]
		>> randomtable[0][1]
		>> randomtable[1][0]
		>> randomtable[1][1];

	CPC* pc = gserver.m_playerList.Find(charindex);

	if( pc )
	{
		const static int Item_Table[2][4]  = {
				{ 1282, 19, 47, 1313 },			// item index
				{ 5, 5000, 1, 10} };			// item count

		const static int Item2_Table[2][6] = {
				{ 841, 85, 44, 485, 391, 786 },			// item index
				{ 1, 10, 30, 30, 30, 10 } };			// item count

		const static int Item3_Table[2][4] = {
			{ 46, 19, 1057, 972	},		// item index
			{ 50, 5000, 10, 6 } };		// item count
				
		const static int Item4_Table[3][3] = {
			{ 498, 499, 779 },			// item index
			{ 6, 6, 0 },				// item flag(level)
			{ 20, 20, 5 } };			// item count
			
		const static int Item5_Table[2][11] = {
			{ 1706, 1707, 1416, 85, 1295, 2356, 2357, 2358, 85, 1416, 2361 },		// item index
			{ 1, 1, 1, 1, 1, 5, 5, 5, 1, 1, 1 }	};									// item count
// << kjban 080121
		const static int Item6_Table[2][12] = { 
			{ 2356, 2357, 2141, 2394, 2395, 2396, 2397, 2398, 2399, 2400, 2401, 2402 },	// item index 
			{ 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };									// item count
// >>

		if( errorCode != MSG_EVENT_ERROR_COUPON_SUCCESS )
		{
			// 롤백
			if( type == 1 )
			{
				for(i = 0; i < 4; i++)
				{
					if( pc->m_invenNormal.FindItem(&r, &c, Item_Table[0][i], 0 , 0) )
					{
						CItem* item = pc->m_invenNormal.GetItem(r, c);
						if( item )
						{
							CNetMsg itemMsg;

							// Item 수량 변경
							DecreaseFromInventory(pc, item, Item_Table[1][i]);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -Item_Table[1][i]);
								SEND_Q(itemMsg, pc->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, pc->m_desc);
								pc->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(pc, item, true, true);
							}
						}
					}
				}
			}
			else if( type == 2 )
			{
				for(i = 0; i < 6; i++)
				{
					if( pc->m_invenNormal.FindItem(&r, &c, Item2_Table[0][i], 0 , 0) )
					{
						CItem* item = pc->m_invenNormal.GetItem(r, c);
						if( item )
						{
							CNetMsg itemMsg;

							// Item 수량 변경
							DecreaseFromInventory(pc, item, Item2_Table[1][i]);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -Item2_Table[1][i]);
								SEND_Q(itemMsg, pc->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, pc->m_desc);
								pc->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(pc, item, true, true);
							}
						}
					}
				}
			}
			else if( type == 3 )
			{
				for(i = 0; i < 4; i++)
				{
					if( pc->m_invenNormal.FindItem(&r, &c, Item3_Table[0][i], 0 , 0) )
					{
						CItem* item = pc->m_invenNormal.GetItem(r, c);
						if( item )
						{
							CNetMsg itemMsg;

							// Item 수량 변경
							DecreaseFromInventory(pc, item, Item3_Table[1][i]);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -Item3_Table[1][i]);
								SEND_Q(itemMsg, pc->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, pc->m_desc);
								pc->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(pc, item, true, true);
							}
						}
					}
				}
			}
			else if( type == 4 )
			{
				for(i = 0; i < 3; i++)
				{
					if( pc->m_invenNormal.FindItem(&r, &c, Item4_Table[0][i], 0 , Item4_Table[1][i]) )
					{
						CItem* item = pc->m_invenNormal.GetItem(r, c);
						if( item )
						{
							CNetMsg itemMsg;

							// Item 수량 변경
							DecreaseFromInventory(pc, item, Item4_Table[2][i]);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -Item4_Table[2][i]);
								SEND_Q(itemMsg, pc->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, pc->m_desc);
								pc->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(pc, item, true, true);
							}
						}
					}
				}
			}
			else if( type == 5 )
			{
				for(i = 0; i < 11; i++)
				{
					if( pc->m_invenNormal.FindItem(&r, &c, Item5_Table[0][i], 0 , 0) )
					{
						CItem* item = pc->m_invenNormal.GetItem(r, c);
						if( item )
						{
							CNetMsg itemMsg;

							// Item 수량 변경
							DecreaseFromInventory(pc, item, Item5_Table[1][i]);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -Item5_Table[1][i]);
								SEND_Q(itemMsg, pc->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, pc->m_desc);
								pc->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(pc, item, true, true);
							}
						}
					}
				}
				
				for(i = 0; i < 2; i++)
				{
					if( pc->m_invenNormal.FindItem(&r, &c, randomtable[i][0], 0 , 0) )
					{
						CItem* item = pc->m_invenNormal.GetItem(r, c);
						if( item )
						{
							CNetMsg itemMsg;
							
							// Item 수량 변경
							DecreaseFromInventory(pc, item, randomtable[i][1]);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -randomtable[i][1]);
								SEND_Q(itemMsg, pc->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, pc->m_desc);
								pc->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(pc, item, true, true);
							}
						}
					}
				}
			}
// << kjban 080121
			else if(type == 6)
			{
				for(i = 0; i < 12; i++)
				{
					if( pc->m_invenNormal.FindItem(&r, &c, Item6_Table[0][i], 0 , 0) )
					{
						CItem* item = pc->m_invenNormal.GetItem(r, c);
						if( item )
						{
							CNetMsg itemMsg;

							// Item 수량 변경
							DecreaseFromInventory(pc, item, Item6_Table[1][i]);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -Item6_Table[1][i]);
								SEND_Q(itemMsg, pc->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, pc->m_desc);
								pc->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(pc, item, true, true);
							}
						}
					}
				}
			}
// >>
		}

/*		GAMELOG << init("COUPON_ITEM_GIVE", pc->m_name, pc->m_desc->m_idname)
				<< "TYPE" << delim << type
				<< "ERROR_CODE" << delim << errorCode << end;	*/
		CNetMsg msg;
		EventErrorMsg(msg, (MSG_EVENT_ERROR_TYPE) errorCode);

		SEND_Q(msg, pc->m_desc);
	}
}
#elif defined (LC_GER)
void OnCouponConfirm(CNetMsg& msg)
{
	int charindex;
	unsigned char errorCode;
	int cIndex, type;

	msg >> charindex
		>> errorCode
		>> cIndex
		>> type;

	int nPromoItem[2] = {1706, 2360};

	CPC* pc = gserver.m_playerList.Find(charindex);

	if (pc)
	{
		CNetMsg msg;
		if (errorCode == MSG_EVENT_ERROR_COUPON_SUCCESS)
		{
			for (int i = 0; i < 2; i++)
			{
				if(!pc->GiveItem(nPromoItem[i], 0, 0, 1, false))
				{
					GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
						<< "1706, 2360" << end;

					// 롤백 
					int r, c;
					for(int j = i - 1; j >= 0; j--)
					{
						CItemProto* pItemProto = gserver.m_itemProtoList.FindIndex(nPromoItem[j]);
						if (!pItemProto) continue;
						
						CInventory* pInven = GET_INVENTORY(pc, GET_TAB(pItemProto->m_typeIdx, pItemProto->m_subtypeIdx));
						if (!pInven) continue;

						if (pInven->FindItem(&r, &c, nPromoItem[j], 0, 0))
						{
							CItem* item = pInven->GetItem(r, c);
							if (item)
							{
								CNetMsg itemMsg;
								
								// Item 수량 변경
								DecreaseFromInventory(pc, item, 1);
								
								if (item->Count() > 0)
								{
									ItemUpdateMsg(itemMsg, item, -1);
									SEND_Q(itemMsg, pc->m_desc);
								}
								else
								{
									ItemDeleteMsg(itemMsg, item);
									SEND_Q(itemMsg, pc->m_desc);
									pc->RemoveItemFromQuickSlot(item);
									RemoveFromInventory(pc, item, true, true);
								}
							}
						} // FindItem end
					} // 롤백 for end
					
					//EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
					SEND_Q(msg, pc->m_desc);
					//return;
				} // GiveItem end
			}

			if(IS_RUNNING_CONN)
			{
#ifdef EVENT_PACKAGE_ITEM_DBLOG
				ConnCouponUse(msg, pc->m_index, cIndex, type, pc->m_desc->m_idname, pc->GetName());
#else
				ConnCouponUse(msg, pc->m_index, cIndex, type);
#endif
				SEND_Q(msg, gserver.m_connector);
			}
			else
			{
				// 롤백 
				int r, c;
				for(int i = 0; i < 2; i++)
				{
					CItemProto* pItemProto = gserver.m_itemProtoList.FindIndex(nPromoItem[i]);
					if (!pItemProto) continue;
					
					CInventory* pInven = GET_INVENTORY(pc, GET_TAB(pItemProto->m_typeIdx, pItemProto->m_subtypeIdx));
					if (!pInven) continue;
					
					if (pInven->FindItem(&r, &c, nPromoItem[i], 0, 0))
					{
						CItem* item = pInven->GetItem(r, c);
						if (item)
						{
							CNetMsg itemMsg;
							
							// Item 수량 변경
							DecreaseFromInventory(pc, item, 1);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -1);
								SEND_Q(itemMsg, pc->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, pc->m_desc);
								pc->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(pc, item, true, true);
							}
						}
					} // FindItem end
				} // 롤백 for end
				
				//EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
				SEND_Q(msg, pc->m_desc);
				return;
			} // end if( IS_RUNNING_CONN)
			
			return;
		} 
		
		// 실패이면 에러코드를 그대로 클라이언트에게 전달
		EventErrorMsg(msg, (MSG_EVENT_ERROR_TYPE) errorCode );
		SEND_Q(msg, pc->m_desc);
	}
}

void OnCouponUse(CNetMsg& msg)
{
	
	int charindex, type;
	unsigned char errorCode;
	
	msg >> charindex
		>> errorCode
		>> type;
	
	int nPromoItem[2] = {1706, 2360};
	CPC* pc = gserver.m_playerList.Find(charindex);
	
	if (pc)
	{
		if( errorCode != MSG_EVENT_ERROR_COUPON_SUCCESS )
		{
			// 롤백 
			int r, c;
			for(int i = 0; i < 2; i++)
			{
				CItemProto* pItemProto = gserver.m_itemProtoList.FindIndex(nPromoItem[i]);
				if (!pItemProto) continue;
				
				CInventory* pInven = GET_INVENTORY(pc, GET_TAB(pItemProto->m_typeIdx, pItemProto->m_subtypeIdx));
				if (!pInven) continue;
				
				if (pInven->FindItem(&r, &c, nPromoItem[i], 0, 0))
				{
					CItem* item = pInven->GetItem(r, c);
					if (item)
					{
						CNetMsg itemMsg;
						
						// Item 수량 변경
						DecreaseFromInventory(pc, item, 1);
						
						if (item->Count() > 0)
						{
							ItemUpdateMsg(itemMsg, item, -1);
							SEND_Q(itemMsg, pc->m_desc);
						}
						else
						{
							ItemDeleteMsg(itemMsg, item);
							SEND_Q(itemMsg, pc->m_desc);
							pc->RemoveItemFromQuickSlot(item);
							RemoveFromInventory(pc, item, true, true);
						}
					}
				} // FindItem end
			} // 롤백 for end
			
		}
		
		CNetMsg msg;
		EventErrorMsg(msg, (MSG_EVENT_ERROR_TYPE) errorCode);
		SEND_Q(msg, pc->m_desc);
		
	}// if(pc) end
	
}
#else // 말레이시아, 홍콩을 제외한 나라들
void OnCouponConfirm(CNetMsg& msg)
{
	int charindex, i;
	unsigned char errorCode;
	int cIndex, type;

	msg >> charindex
		>> errorCode
		>> cIndex
		>> type;

	CPC* pc = gserver.m_playerList.Find(charindex);

	if( pc )
	{
		// 아이템 인덱스를 일본 의상 아이템으로
		if( type == 1 && pc->m_job > JOB_SORCERER)
		{
			EventErrorMsg(msg, MSG_EVENT_ERROR_INSUFF_CONDITION );
			SEND_Q(msg, pc->m_desc);
			return;
		}

		CNetMsg msg;
		if( errorCode == MSG_EVENT_ERROR_COUPON_SUCCESS )
		{
			// 2차 아이템으로
			const static int japanItem_table[5][5] = {
				{ 1254, 1255, 1256, 1257, 1258 },		// 타이탄
				{ 1259, 1260, 1261, 1262, 1263 },		// 나이트 
				{ 1264, 1265, 1266, 1267, 1268 },		// 힐러
				{ 1269, 1270, 1271, 1272, 1273 },		// 메이지 
				{ 1274, 1275, 1276, 1277, 1278 } };		// 로그 

			const static int japanEvent2_table[2][4]  = {
				{ 1316, 1317, 1315, 1314 },
				{	7,	15,		7,	15} };

#ifdef	EVENT_CHANGE_JPN_200803	
			const static int japanEvent3_table[2][3] =  {
				{ 1307, 2497, 2498, },
				{ 8, 20, 2, },
			};	
#else
			const static int japanEvent3_table =  2399;
#endif // EVENT_CHANGE_JPN_200803

			if( type == 1 )
			{
				for( i = 0; i < 5; i++ )
				{
					// 성공이면 아이템을 지급하고 커넥터에게 컨펌을 보냄
					if( !pc->GiveItem(japanItem_table[(int)pc->m_job][i], 0, 0, 1, false) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
								<< japanItem_table[(int)pc->m_job][i] << end;
						// 롤백 
						int r, c;
						for(i = i-1; i >= 0; i--)
						{
							if( pc->m_invenNormal.FindItem(&r, &c, japanItem_table[(int)pc->m_job][i], 0 , 0) )
							{
								CItem* item = pc->m_invenNormal.GetItem(r, c);
								if( item )
								{
									CNetMsg itemMsg;

									// Item 수량 변경
									DecreaseFromInventory(pc, item, 1);
									
									if (item->Count() > 0)
									{
										ItemUpdateMsg(itemMsg, item, -1);
										SEND_Q(itemMsg, pc->m_desc);
									}
									else
									{
										ItemDeleteMsg(itemMsg, item);
										SEND_Q(itemMsg, pc->m_desc);
										pc->RemoveItemFromQuickSlot(item);
										RemoveFromInventory(pc, item, true, true);
									} // end if
								} // end if
							} // end if
						} // end for

						//EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
						SEND_Q(msg, pc->m_desc);
						return;
					} // end if
				} // end for
			}
			else if( type == 2)
			{
				for( i = 0; i < 4; i++ )
				{
					// 성공이면 아이템을 지급하고 커넥터에게 컨펌을 보냄
					if( !pc->GiveItem(japanEvent2_table[0][i], 0, 0, japanEvent2_table[1][i], false) )
					{
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
								<< japanEvent2_table[0][i] << delim 
								<< japanEvent2_table[1][i] << end;
						// 롤백 
						int r, c;
						for(i = i-1; i >= 0; i--)
						{
							if( pc->m_invenNormal.FindItem(&r, &c, japanEvent2_table[0][i], 0 , 0) )
							{
								CItem* item = pc->m_invenNormal.GetItem(r, c);
								if( item )
								{
									CNetMsg itemMsg;

									// Item 수량 변경
									DecreaseFromInventory(pc, item, japanEvent2_table[1][i]);
									
									if (item->Count() > 0)
									{
										ItemUpdateMsg(itemMsg, item, -japanEvent2_table[1][i]);
										SEND_Q(itemMsg, pc->m_desc);
									}
									else
									{
										ItemDeleteMsg(itemMsg, item);
										SEND_Q(itemMsg, pc->m_desc);
										pc->RemoveItemFromQuickSlot(item);
										RemoveFromInventory(pc, item, true, true);
									}
								} // end if
							} // end if
						} // end for

						//EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
						SEND_Q(msg, pc->m_desc);
						return;
					} // end if

					GAMELOG << init("PACKAGE_ITEM_GIVE_SUCCESS", pc->m_name, pc->m_desc->m_idname)
							<< japanEvent2_table[0][i] << delim 
							<< japanEvent2_table[1][i] << end;
				}	// end for
			}
			else if( type == 3)
			{
#ifdef EVENT_CHANGE_JPN_200803
				for(i = 0; i < 3; i++ )
				{
					if( !pc->GiveItem(japanEvent3_table[0][i], 0, 0, japanEvent3_table[1][i], false) )
#else
					if( !pc->GiveItem(japanEvent3_table, 0, 0, 1, false) )
#endif // EVENT_CHANGE_JPN_200803			
					{	// 성공이면 아이템을 지급하고 커넥터에게 컨펌을 보냄
#ifdef EVENT_CHANGE_JPN_200803
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
								<< japanEvent3_table[0][i] << delim 
								<< japanEvent3_table[1][i] << end;
#else
						GAMELOG << init("PACKAGE_ITEM_GIVE_FAIL", pc->m_name, pc->m_desc->m_idname)
							<< japanEvent3_table << delim 
							<< 1 << end;
#endif	// EVENT_CHANGE_JPN_200803
					
						// 롤백 
						int r, c;

#ifdef EVENT_CHANGE_JPN_200803
						for(i = i-1; i >= 0; i--)
						{
							if( pc->m_invenNormal.FindItem(&r, &c, japanEvent3_table[0][i], 0 , 0) )
#else
							if( pc->m_invenNormal.FindItem(&r, &c, japanEvent3_table, 0 , 0) )
#endif // EVENT_CHANGE_JPN_200803		
							{
								CItem* item = pc->m_invenNormal.GetItem(r, c);
								if( item )
								{
									CNetMsg itemMsg;

									// Item 수량 변경
#ifdef EVENT_CHANGE_JPN_200803
									DecreaseFromInventory(pc, item, japanEvent3_table[1][i]);
#else
									DecreaseFromInventory(pc, item, 1);
#endif	// EVENT_CHANGE_JPN_200803
														
									if (item->Count() > 0)
									{
#ifdef EVENT_CHANGE_JPN_200803
										ItemUpdateMsg(itemMsg, item, -japanEvent3_table[1][i]);
#else
										ItemUpdateMsg(itemMsg, item, -1);
#endif // EVENT_CHANGE_JPN_200803
										SEND_Q(itemMsg, pc->m_desc);
									}
									else
									{
										ItemDeleteMsg(itemMsg, item);
										SEND_Q(itemMsg, pc->m_desc);
										pc->RemoveItemFromQuickSlot(item);
										RemoveFromInventory(pc, item, true, true);
									}
								}
							} // end if
#ifdef EVENT_CHANGE_JPN_200803
						} // end for
#endif
						SEND_Q(msg, pc->m_desc);
						return;
					}	// end if

#ifdef EVENT_CHANGE_JPN_200803
					GAMELOG << init("PACKAGE_ITEM_GIVE_SUCCESS", pc->m_name, pc->m_desc->m_idname)
							<< japanEvent3_table[0][i] << delim 
							<< japanEvent3_table[1][i] << end;
#else
					GAMELOG << init("PACKAGE_ITEM_GIVE_SUCCESS", pc->m_name, pc->m_desc->m_idname)
						<< japanEvent3_table << delim 
						<< 1 << end;
#endif // EVENT_CHANGE_JPN_200803
#ifdef EVENT_CHANGE_JPN_200803
				}	// end for
#endif // EVENT_CHANGE_JPN_200803
			} // end if( type == 3)

			if( IS_RUNNING_CONN)
			{
#ifdef EVENT_PACKAGE_ITEM_DBLOG
				ConnCouponUse(msg, pc->m_index, cIndex, type, pc->m_desc->m_idname, pc->GetName());
#else
				ConnCouponUse(msg, pc->m_index, cIndex, type);
#endif
				SEND_Q(msg, gserver.m_connector);
			}
			else
			{
				// 롤백 
				int r, c;
				if( type == 1 )
				{
					for(i = i-1; i >= 0; i--)
					{
						if( pc->m_invenNormal.FindItem(&r, &c, japanItem_table[(int)pc->m_job][i], 0, 0) )
						{
							CItem* item = pc->m_invenNormal.GetItem(r, c);
							if( item )
							{
								CNetMsg itemMsg;

								// Item 수량 변경
								DecreaseFromInventory(pc, item, 1);
								
								if (item->Count() > 0)
								{
									ItemUpdateMsg(itemMsg, item, -1);
									SEND_Q(itemMsg, pc->m_desc);
								}
								else
								{
									ItemDeleteMsg(itemMsg, item);
									SEND_Q(itemMsg, pc->m_desc);
									pc->RemoveItemFromQuickSlot(item);
									RemoveFromInventory(pc, item, true, true);
								}
							}
						}
					}
				}
				else if( type == 2)
				{
					for(i = i-1; i >= 0; i--)
					{
						if( pc->m_invenNormal.FindItem(&r, &c, japanEvent2_table[0][i], 0 , 0) )
						{
							CItem* item = pc->m_invenNormal.GetItem(r, c);
							if( item )
							{
								CNetMsg itemMsg;

								// Item 수량 변경
								DecreaseFromInventory(pc, item, japanEvent2_table[1][i]);
								
								if (item->Count() > 0)
								{
									ItemUpdateMsg(itemMsg, item, -japanEvent2_table[1][i]);
									SEND_Q(itemMsg, pc->m_desc);
								}
								else
								{
									ItemDeleteMsg(itemMsg, item);
									SEND_Q(itemMsg, pc->m_desc);
									pc->RemoveItemFromQuickSlot(item);
									RemoveFromInventory(pc, item, true, true);
								}
							}
						}
					}
				}
				else if( type == 3)
				{
#ifdef EVENT_CHANGE_JPN_200803
					for(i = i-1; i >= 0; i--)
					{
						if( pc->m_invenNormal.FindItem(&r, &c, japanEvent3_table[0][i], 0 , 0) )
#else
						if( pc->m_invenNormal.FindItem(&r, &c, japanEvent3_table, 0 , 0) )
#endif	// EVENT_CHANGE_JPN_200803
						{
							CItem* item = pc->m_invenNormal.GetItem(r, c);
							if( item )
							{
								CNetMsg itemMsg;
								// Item 수량 변경
#ifdef EVENT_CHANGE_JPN_200803
								DecreaseFromInventory(pc, item, japanEvent3_table[1][i]);
#else
								DecreaseFromInventory(pc, item, 1);
#endif // EVENT_CHANGE_JPN_200803
							
								if (item->Count() > 0)
								{
#ifdef EVENT_CHANGE_JPN_200803
									ItemUpdateMsg(itemMsg, item, -japanEvent3_table[1][i]);
#else
									ItemUpdateMsg(itemMsg, item, -1);
#endif // EVENT_CHANGE_JPN_200803
									SEND_Q(itemMsg, pc->m_desc);
								}
								else
								{
									ItemDeleteMsg(itemMsg, item);
									SEND_Q(itemMsg, pc->m_desc);
									pc->RemoveItemFromQuickSlot(item);
									RemoveFromInventory(pc, item, true, true);
								} // end if
							} // end if
						} // end if
#ifdef EVENT_CHANGE_JPN_200803
					} // end for
#endif // EVENT_CHANGE_JPN_200803
				} // end if( type == 3)

				// 지급한 아이템을 롤백 하고 없는 쿠폰번호라고 클라이언트에게 보냄
				EventErrorMsg(msg, MSG_EVENT_ERROR_NOT_COUPON );
				SEND_Q(msg, pc->m_desc);
			} // end if( IS_RUNNING_CONN)

			return;
		}  // end if( errorCode == MSG_EVENT_ERROR_COUPON_SUCCESS )

		// 실패이면 에러코드를 그대로 클라이언트에게 전달
		EventErrorMsg(msg, (MSG_EVENT_ERROR_TYPE) errorCode );
		SEND_Q(msg, pc->m_desc);
	} // end if( pc )
}

void OnCouponUse(CNetMsg& msg)
{
	int charindex, i, r,c, type;
	unsigned char errorCode;

	msg >> charindex
		>> errorCode
		>> type;

	CPC* pc = gserver.m_playerList.Find(charindex);

	if( pc )
	{
		// 2차 아이템으로 
		const static int japanItem_table[5][5] = {
				{ 1254, 1255, 1256, 1257, 1258 },		// 타이탄
				{ 1259, 1260, 1261, 1262, 1263 },		// 나이트 
				{ 1264, 1265, 1266, 1267, 1268 },		// 힐러
				{ 1269, 1270, 1271, 1272, 1273 },		// 메이지 
				{ 1274, 1275, 1276, 1277, 1278 } };		// 로그 

		const static int japanEvent2_table[2][4]  = {
				{ 1316, 1317, 1315, 1314 },
				{	7,	15,		7,	15} };

		const static int japanEvent3_table =  2399;

		if( errorCode != MSG_EVENT_ERROR_COUPON_SUCCESS )
		{
			// 롤백
			if( type == 1 )
			{
				for(i = 0; i < 5; i++)
				{
					if( pc->m_invenNormal.FindItem(&r, &c, japanItem_table[(int)pc->m_job][i], 0, 0) )
					{
						CItem* item = pc->m_invenNormal.GetItem(r, c);
						if( item )
						{
							CNetMsg itemMsg;

							// Item 수량 변경
							DecreaseFromInventory(pc, item, 1);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -1);
								SEND_Q(itemMsg, pc->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, pc->m_desc);
								pc->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(pc, item, true, true);
							}
						}
					}
				}
			}
			else if( type == 2 )
			{
				for(i = 0; i < 4; i++)
				{
					if( pc->m_invenNormal.FindItem(&r, &c, japanEvent2_table[0][i], 0 , 0) )
					{
						CItem* item = pc->m_invenNormal.GetItem(r, c);
						if( item )
						{
							CNetMsg itemMsg;

							// Item 수량 변경
							DecreaseFromInventory(pc, item, japanEvent2_table[1][i]);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -japanEvent2_table[1][i]);
								SEND_Q(itemMsg, pc->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, pc->m_desc);
								pc->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(pc, item, true, true);
							}
						}
					}
				}
			}
			else if( type == 3)
			{
				if( pc->m_invenNormal.FindItem(&r, &c, japanEvent3_table, 0 , 0) )
				{
					CItem* item = pc->m_invenNormal.GetItem(r, c);
					if( item )
					{
						CNetMsg itemMsg;

						// Item 수량 변경
						DecreaseFromInventory(pc, item, 1);
						
						if (item->Count() > 0)
						{
							ItemUpdateMsg(itemMsg, item, -1);
							SEND_Q(itemMsg, pc->m_desc);
						}
						else
						{
							ItemDeleteMsg(itemMsg, item);
							SEND_Q(itemMsg, pc->m_desc);
							pc->RemoveItemFromQuickSlot(item);
							RemoveFromInventory(pc, item, true, true);
						}
					}
				}
			}	
		}

/*		GAMELOG << init("COUPON_ITEM_GIVE", pc->m_name, pc->m_desc->m_idname)
				<< "TYPE" << delim << type
				<< "ERROR_CODE" << delim << errorCode << end;	*/
		CNetMsg msg;
		EventErrorMsg(msg, (MSG_EVENT_ERROR_TYPE) errorCode);

		SEND_Q(msg, pc->m_desc);
	}
}
#endif // #else 
#endif //EVENT_PACKAGE_ITEM

#ifdef CASH_ITEM_GIFT
void OnCashItemGift(CNetMsg& msg)
{
	unsigned char errCode;
	int sendCharIndex;
	int recvCharIndex;

	msg >> errCode
		>> sendCharIndex
		>> recvCharIndex;

	CNetMsg rmsg;
	CPC* sendPC = gserver.m_playerList.Find(sendCharIndex);
	if( sendPC )
	{
		sendPC->ResetPlayerState(PLAYER_STATE_CASHITEM);
		sendPC->m_billReqTime = 0;

		CashItemGiftSendRepMsg(rmsg, (MSG_EX_CASHITEM_ERROR_TYPE) errCode);
		SEND_Q(rmsg, sendPC->m_desc);
	}

	if( errCode == MSG_EX_CASHITEM_ERROR_GIFT_SUCCESS )
	{
		CPC* recvPC = gserver.m_playerList.Find( recvCharIndex);
		if( recvPC )
		{
			CashItemGiftRecvNoticeRepMsg( rmsg, 1 );
			SEND_Q(rmsg, recvPC->m_desc);
		}
	}
}

void OnCashItemGiftSendHistory(CNetMsg& msg)
{
	OnCashItemGiftPresentHistory(msg, true);
}
void OnCashItemGiftRecvHistory(CNetMsg& msg)
{
	OnCashItemGiftPresentHistory(msg, false);
}

void OnCashItemGiftPresentHistory(CNetMsg& msg, bool bSend)
{
	// 응답				: errorcode(uc) count(n) ctid(n) sendcharName(str)
	unsigned char errCode;
	int count, ctid, charindex;
	CLCString charName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> charindex
		>> errCode
		>> count;

	CPC* pc = gserver.m_playerList.Find(charindex);

	CNetMsg rmsg;
	if( pc )
	{
		CashItemGiftHistoryRepMsg(rmsg, bSend, (MSG_EX_CASHITEM_ERROR_TYPE) errCode);
		rmsg << count;

		for(int i = 0; i < count; i++ )
		{
			msg >> ctid
				>> charName;

			rmsg << ctid
				 << charName;
		}

		SEND_Q(rmsg, pc->m_desc);

	}
}

void OnCashItemGiftRecvList(CNetMsg& msg)
{
	int charIndex;
	unsigned char listflag;

	msg >> charIndex
		>> listflag;

	CNetMsg rmsg;
	CPC* pc = gserver.m_playerList.Find(charIndex);
	if( !pc )
		return;

	CashItemGiftRecvListRepMsg(rmsg, listflag, msg);
	SEND_Q(rmsg, pc->m_desc);
}
void OnCashItemGiftRecv(CNetMsg& msg)
{
	OnCashItemBringProcess(msg, true);
}
#endif

#ifdef EVENT_OPEN_BETA_GIFT
void OnOpenBetaGift(CNetMsg& msg)
{
	CNetMsg rmsg;
	
	int userindex = 0;
	int charindex = 0;
	char errorcode = -1;
	
	msg >> userindex
		>> charindex
		>> errorcode;
	
	if (errorcode == MSG_CONN_ERROR_SUCCESS)
	{
		CPC* pc = gserver.m_playerList.Find(charindex);
		if (pc && pc->m_desc->m_index == userindex)
		{
			int giftindex = -1;
			switch (pc->m_job)
			{
			case JOB_TITAN:
				giftindex = 460;
				break;
			case JOB_KNIGHT:
				giftindex = 427;
				break;
			case JOB_HEALER:
				giftindex = 462;
				break;
			case JOB_MAGE:
				giftindex = 461;
				break;
			case JOB_ROGUE:
				giftindex = 951;
				break;
			case JOB_SORCERER:
				giftindex = 1069;
				break;
			}
			
			CItem* item = gserver.m_itemProtoList.CreateItem(giftindex, -1, 0, 0, 1);
			if (item)
			{
				if (AddToInventory(pc, item, false, true))
				{
					ItemAddMsg(rmsg, item);
					SEND_Q(rmsg, pc->m_desc);
					EventOpenBetaGiftMsg( rmsg );
					SEND_Q(rmsg, pc->m_desc);
					
					GAMELOG << init("EVENT OPEN BETA", pc)
						<< item->m_idNum
						<< end;
					
					return ;
				}
				else
				{
					EventErrorMsg(rmsg, MSG_EVENT_ERROR_FULLINVENTORY);
					SEND_Q(rmsg, pc->m_desc);
				}
				
				delete item;
				item = NULL;
			}
			else
			{
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_INSUFF_CONDITION);
				SEND_Q(rmsg, pc->m_desc);
			}
		}
		
		if (IS_RUNNING_CONN)
		{
			ConnPreopenGiftMsg(rmsg, userindex, charindex, true);
			SEND_Q(rmsg, gserver.m_connector);
		}
	}
	else
	{
		CPC* pc = gserver.m_playerList.Find(charindex);
		if (pc == NULL)
			return ;
		
		CNetMsg rmsg;
		switch (errorcode)
		{
		case MSG_CONN_ERROR_OPEN_BETA_GIFT_INSUFF_CONDITION:
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_INSUFF_CONDITION);
			break;
		case MSG_CONN_ERROR_OPEN_BETA_GIFT_ALREADY_GIVE:
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_ALREADY_GIVE);
			break;
		default:
			return ;
		}
		SEND_Q(rmsg, pc->m_desc);
	}
}
#endif // EVENT_OPEN_BETA_GIFT

#ifdef JPN_GPARA_PROMOTION
void OngparaPromotion(CNetMsg& msg)
{
	CNetMsg		rmsg;
	int			nSubType = 0;
	int			nUserIndex = 0;
	int			nCharIndex = 0;

	CItem*		pItem = NULL;

	msg >> nSubType
		>> nUserIndex
		>> nCharIndex;

	CPC* pPC = gserver.m_playerList.Find(nCharIndex);
	if (pPC == NULL)
		return ;
	if (pPC->m_desc->m_index != nUserIndex)
		return ;
	if (pPC->m_nGparaPromotionData != MSG_CONN_GPARA_PROMOTION_QUERY)
		return ;
	if (!IS_RUNNING_CONN)
		return ;

	switch (nSubType)
	{
	case MSG_CONN_GPARA_PROMOTION_ALLOW:
		// 아이템 생성
		pItem = gserver.m_itemProtoList.CreateItem(2137, -1, 0, 0, 1);
		if (pItem)
		{
			// 아이템 지급
			if (AddToInventory(pPC, pItem, true, true))
			{
				ItemAddMsg(rmsg, pItem);
				SEND_Q(rmsg, pPC->m_desc);

				// 게임로그
				GAMELOG << init("GPARA GIFT", pPC)
						<< itemlog(pItem)
						<< end;

				// 상태 변경
				pPC->m_nGparaPromotionData = MSG_CONN_GPARA_PROMOTION_COMMIT;

				// 컨넥터로 요청
				ConnGparaPromotionMsg(rmsg, MSG_CONN_GPARA_PROMOTION_COMMIT, nUserIndex, nCharIndex);
				SEND_Q(rmsg, gserver.m_connector);
			}
			else
			{
				delete pItem;
				pItem = NULL;
			}
		}
		break;

	case MSG_CONN_GPARA_PROMOTION_DENY:
	default:
		break;
	}
}
#endif // JPN_GPARA_PROMOTION

#ifdef JPN_OCN_GOO_PROMOTION
void OnOCN_GooPromotion(CNetMsg& msg)
{
	CNetMsg		rmsg;
	int			nSubType = 0;
	int			nUserIndex = 0;
	int			nCharIndex = 0;

	CItem*		pItem_waterMelon = NULL;
	CItem*		pItem_cake = NULL;
	CItem*		pItem_candy = NULL;
	CItem*		pItem_iceMan = NULL;
	CItem*		pItem_scroll = NULL;
	CItem*		pItem_velocity_of_light = NULL;

	msg >> nSubType
		>> nUserIndex
		>> nCharIndex;

	CPC* pPC = gserver.m_playerList.Find(nCharIndex);
	if (pPC == NULL)
		return ;
	if (pPC->m_desc->m_index != nUserIndex)
		return ;
	if (pPC->m_nGparaPromotionData != MSG_CONN_OCN_GOO_PROMOTION_QUERY)
		return ;
	if (!IS_RUNNING_CONN)
		return ;

	switch (nSubType)
	{
	case MSG_CONN_OCN_GOO_PROMOTION_ALLOW:
		// 아이템 생성
		{
			bool item_add_flag = false; 

			// 잘익은 수박 // 케이크  // 사탕 // 눈사람 인형 // 경험의 스크롤 // 광속
			pItem_waterMelon = gserver.m_itemProtoList.CreateItem(2494, -1, 0, 0, 30);
			pItem_cake = gserver.m_itemProtoList.CreateItem(2495, -1, 0, 0, 30);
			pItem_candy = gserver.m_itemProtoList.CreateItem(2496, -1, 0, 0, 30);
			pItem_iceMan = gserver.m_itemProtoList.CreateItem(2498, -1, 0, 0, 10);
			pItem_scroll = gserver.m_itemProtoList.CreateItem(2499, -1, 0, 0, 30);
			pItem_velocity_of_light = gserver.m_itemProtoList.CreateItem(2500, -1, 0, 0, 1);

			if ( pItem_waterMelon && pItem_cake && pItem_candy && pItem_iceMan && pItem_scroll && pItem_velocity_of_light )
			{
				// 아이템 지급
				if ( AddToInventory(pPC, pItem_waterMelon, false, false) &&
					 AddToInventory(pPC, pItem_cake, false, false) &&
					 AddToInventory(pPC, pItem_candy, false, false) &&
					 AddToInventory(pPC, pItem_iceMan, false, false) &&
					 AddToInventory(pPC, pItem_scroll, false, false) &&
					 AddToInventory(pPC, pItem_velocity_of_light, false, false)
					)
				{
					ItemAddMsg(rmsg, pItem_waterMelon);
					SEND_Q(rmsg, pPC->m_desc);
					ItemAddMsg(rmsg, pItem_cake);
					SEND_Q(rmsg, pPC->m_desc);
					ItemAddMsg(rmsg, pItem_candy);
					SEND_Q(rmsg, pPC->m_desc);
					ItemAddMsg(rmsg, pItem_iceMan);
					SEND_Q(rmsg, pPC->m_desc);
					ItemAddMsg(rmsg, pItem_scroll);
					SEND_Q(rmsg, pPC->m_desc);
					ItemAddMsg(rmsg, pItem_velocity_of_light);
					SEND_Q(rmsg, pPC->m_desc);

					item_add_flag = true;
				}
				else
				{
					pItem_waterMelon = NULL;
					delete pItem_waterMelon;
					pItem_cake = NULL;
					delete pItem_cake;
					pItem_candy = NULL;
					delete pItem_candy;
					pItem_iceMan = NULL;
					delete pItem_iceMan;
					pItem_scroll = NULL;
					delete pItem_scroll;
					pItem_velocity_of_light = NULL;
					delete pItem_velocity_of_light;
					return;
				}					
			}
			else
			{
				pItem_waterMelon = NULL;
				delete pItem_waterMelon;
				pItem_cake = NULL;
				delete pItem_cake;
				pItem_candy = NULL;
				delete pItem_candy;
				pItem_iceMan = NULL;
				delete pItem_iceMan;
				pItem_scroll = NULL;
				delete pItem_scroll;
				pItem_velocity_of_light = NULL;
				delete pItem_velocity_of_light;
				return;
			}

			if ( item_add_flag )
			{
				// 상태 변경
				pPC->m_nGparaPromotionData = MSG_CONN_OCN_GOO_PROMOTION_COMMIT;

				// 컨넥터로 요청
				ConnOCN_GooPromotionMsg(rmsg, MSG_CONN_OCN_GOO_PROMOTION_COMMIT, nUserIndex, nCharIndex);
				SEND_Q(rmsg, gserver.m_connector);
			}

		}
		break;

	case MSG_CONN_OCN_GOO_PROMOTION_DENY:
	default:
		break;
	}
}
#endif // JPN_OCN_GOO_PROMOTION

#ifdef CREATE_EVENT
void OnCreateEvent(CNetMsg & msg)
{
	CNetMsg		rmsg;
	int			nSubType = 0;
	int			nUserIndex = 0;
	int			nCharIndex = 0;

	msg >> nSubType
		>> nUserIndex
		>> nCharIndex;
	
	if(nSubType == MSG_CONN_CREATEEVENT_CREATETABLE)
	{	// 이벤트 테이블 생성
//		char buf[1024];
//		memset( buf, 0, 1024);
//		sprintf(buf, "UPDATE create_event SET table_created=1 WHERE idx=%d ", gserver.m_nCreateTableIndex);
		
//		CDBCmd cmd;
//		cmd.Init(&gserver.m_dbdata);
//		cmd.SetQuery(buf);
//		if(cmd.Update() == false)
//		{
//			GAMELOG << init("CREATE EVENT UPDATE TABLE ERROR") << end;
//			return;
//		}

//		GAMELOG << init("CREATE EVENT UPDATE TABLE") << end;
		return;
	}

	CPC* pPC = gserver.m_playerList.Find(nCharIndex);
	if (pPC == NULL)
	{
		return;
	}

	if (pPC->m_desc->m_index != nUserIndex)
	{
		return;
	}

	if (pPC->m_nCreateEventData != MSG_CONN_CREATEEVENT_QUERY)
	{
		return;
	}

	if (!IS_RUNNING_CONN)
	{
		return;		
	}

	switch (nSubType)
	{
	case MSG_CONN_CREATEEVENT_ALLOW:
		{
			bool bGive = false;
			int i, j;

			// 아이템 생성		
			for(i=0; i < 10; i++)
			{
				if(gserver.m_nCreateEventItemIdx[i] == 0 
					|| gserver.m_nCreateEventItemCnt[i] == 0)
				{
					continue;
				}
				bGive = pPC->GiveItem(gserver.m_nCreateEventItemIdx[i], 0, 0, gserver.m_nCreateEventItemCnt[i]);
				if (bGive == false)
				{
					break;
				}
			}				
						
			if( bGive == true )
			{	// 아이템 지급 성공
				// 게임로그
				GAMELOG << init("CREATE EVENT GIFT SUCCESS", pPC)					
						<< end;

				// 상태 변경
				pPC->m_nCreateEventData = MSG_CONN_CREATEEVENT_COMMIT;

				// 컨넥터로 요청
				CNetMsg		rmsg;
				ConnCreateEventMsg(rmsg, MSG_CONN_CREATEEVENT_COMMIT, nUserIndex, nCharIndex);
				SEND_Q(rmsg, gserver.m_connector);
			}	
			else
			{	// 지급중 실패 시 지급된 아이템 회수
				GAMELOG << init("CREATE EVENT GIFT FAILED", pPC)					
						<< end;
				int r, c;
				for(j=0; j<i; j++)
				{	
					if (pPC->m_invenNormal.FindItem(&r, &c, gserver.m_nCreateEventItemIdx[j]))
					{
						CItem * item = pPC->m_invenNormal.GetItem(r, c);
						if (item)
						{
							CNetMsg itemMsg;

							// Item 수량 변경
							DecreaseFromInventory(pPC, item, gserver.m_nCreateEventItemCnt[j]);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -gserver.m_nCreateEventItemCnt[j]);
								SEND_Q(itemMsg, pPC->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, pPC->m_desc);
								pPC->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(pPC, item, true, true);
							}
						}
					}
				}	
			}
		}	
		break;

	case MSG_CONN_CREATEEVENT_DENY:
	default:
		break;
	}
}
#endif // CREATE_EVENT

#ifdef JPN_MSN_PROMOTION
void OnMSNPromotion(CNetMsg & msg)
{
	CNetMsg		rmsg;
	int			nSubType = 0;
	int			nUserIndex = 0;
	int			nCharIndex = 0;

	msg >> nSubType
		>> nUserIndex
		>> nCharIndex;

	CPC* pPC = gserver.m_playerList.Find(nCharIndex);
	if (pPC == NULL)
	{
		return;
	}

	if (pPC->m_desc->m_index != nUserIndex)
	{
		return;
	}

	if (pPC->m_nMSNPromotionData != MSG_CONN_MSN_PROMOTION_QUERY)
	{
		return;
	}

	if (!IS_RUNNING_CONN)
	{
		return;		
	}

	switch (nSubType)
	{
	case MSG_CONN_MSN_PROMOTION_ALLOW:
		{
			bool bGive = false;
			int i, j;
			// - 건강의 물약(Index 2142) - 5개(셋트)
			// - 지력의 물약(Index 2143) - 5개(셋트)
			// - 행운 주문서(Index 2141) - 1개
			// - 훈련 주문서(Index 2139) - 1개
			static int nGiveItem[4][2] = 
			{
				{ 2139, 1 },
				{ 2141, 1 }, 
				{ 2142, 5 }, 
				{ 2143,	5 },			
			};

			// 아이템 생성		
			for( i=0; i<4; i++)
			{
				bGive = pPC->GiveItem(nGiveItem[i][0], 0, 0, nGiveItem[i][1]);
				if (bGive == false)
				{
					break;
				}
			}				
						
			if( bGive == true )
			{	// 아이템 지급 성공
				// 게임로그
				GAMELOG << init("MSN GIFT SUCCESS", pPC)					
						<< end;

				// 상태 변경
				pPC->m_nMSNPromotionData = MSG_CONN_MSN_PROMOTION_COMMIT;

				// 컨넥터로 요청
				CNetMsg		rmsg;
				ConnMSNPromotionMsg(rmsg, MSG_CONN_MSN_PROMOTION_COMMIT, nUserIndex, nCharIndex);
				SEND_Q(rmsg, gserver.m_connector);
			}	
			else
			{	// 지급중 실패 시 지급된 아이템 회수
				GAMELOG << init("MSN GIFT FAILED", pPC)					
						<< end;
				int r, c;
				for( j=0; j<i; j++)
				{	
					if (pPC->m_invenNormal.FindItem(&r, &c, nGiveItem[j][0]))
					{
						CItem * item = pPC->m_invenNormal.GetItem(r, c);
						if (item)
						{
							CNetMsg itemMsg;

							// Item 수량 변경
							DecreaseFromInventory(pPC, item, nGiveItem[j][1]);
							
							if (item->Count() > 0)
							{
								ItemUpdateMsg(itemMsg, item, -nGiveItem[j][1]);
								SEND_Q(itemMsg, pPC->m_desc);
							}
							else
							{
								ItemDeleteMsg(itemMsg, item);
								SEND_Q(itemMsg, pPC->m_desc);
								pPC->RemoveItemFromQuickSlot(item);
								RemoveFromInventory(pPC, item, true, true);
							}
						}
					}
				}	
			}
		}	
		break;

	case MSG_CONN_MSN_PROMOTION_DENY:
	default:
		break;
	}
}
#endif // JPN_MSN_PROMOTION

#ifdef EVENT_GOMDORI_2007
void OnConnEventGomdori2007(CNetMsg& msg)
{
	unsigned char		cSubType;
	int					nCharIndex;
	int					nCount;
	int*				nStatus = NULL;

	msg >> cSubType;

	if (cSubType == MSG_CONN_EVENT_GOMDORI_2007_STATUS)
	{
		msg >> nCharIndex
			>> nCount;
		CPC* pPC = gserver.m_playerList.Find(nCharIndex);
		if (!pPC)
			return ;

		if (nCount > 0)
			nStatus = new int[nCount];
		int i;
		for (i = 0; i < nCount; i++)
			msg >> nStatus[i];

		CNetMsg rmsg;
		EventGomdori2007ViewStatusMsg(rmsg, nCount, nStatus);
		SEND_Q(rmsg, pPC->m_desc);

		if (nStatus)
		{
			delete [] nStatus;
			nStatus = NULL;
		}
	}
}
#endif // EVENT_GOMDORI_2007


#ifdef EVENT_FLOWERTREE_2007
void OnConnEventFlowerTree2007(CNetMsg& msg)
{
	unsigned char	cSubtype = 0;
	int				nCharIndex = 0;
	unsigned int	unPoint = 0;
	CNetMsg			rmsg;
	
	msg >> cSubtype;	
	msg >> nCharIndex;
	
	switch( cSubtype )
	{
	case MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT:
		{
			int zoneZuno = gserver.FindZone(0);		//주노
			if ( zoneZuno < 0 )
			{ return; }			// 주노지역 없음

			msg >> unPoint;

			CArea *area = gserver.m_zones[zoneZuno].m_area;		
			if ( area == NULL )
			{ return; }			// 지역정보 없음

 			CNPC* npc = area->m_npcList;
 			while ( npc )
 			{
 				if( npc->m_idNum == 342 )		// 꽃나무 NPC 다수
  				{
  					npc->m_maxHP = npc->m_hp = (int) unPoint;
  				}
  				npc = npc->m_pNPCNext;
 			}
			
			EventFlowerTree2007Msg( rmsg, MSG_EVENT_FLOWERTREE_2007_POINT );
			rmsg << unPoint;
			rmsg << gserver.m_serverno;
			
			if ( nCharIndex == 0 )		// 포인트가 누적되어 나무에 변화가 있는 경우
			{
				gserver.m_playerList.SendToAll( rmsg );			
			}
			else						// 로그인 할때 보내주는 POINT 및 클라이언트가 요청해서 보내주는 부분
			{
				CPC* ch = gserver.m_playerList.Find(nCharIndex);
				if( ch == NULL )
					return;

				SEND_Q( rmsg, ch->m_desc );			
			}
		}break;

	case MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT_RANK :		// 전서버 정보 요청
		{
			int	nServerCount = 0;

			CPC* ch = gserver.m_playerList.Find(nCharIndex);
			if( ch == NULL )
				return;

			msg >> nServerCount;
			
			EventFlowerTree2007Msg( rmsg, MSG_EVENT_FLOWERTREE_2007_POINT_RANK );
			rmsg << nServerCount;
			for( int i=0; i < nServerCount; i++ )
			{
				msg >> unPoint;
				rmsg << unPoint;
				rmsg << (i+1);	//서버 군번호, 클라이언트에서 검색을 위해 요청
			}

			if( nServerCount > 0 )
			{
				SEND_Q( rmsg, ch->m_desc );
			}			
		}break;
	}
}
#endif //EVENT_FLOWERTREE_2007

#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
void OnGuildPointRankingRep( CNetMsg& msg )
{
	CNetMsg rmsg;

	int nCharindex;
	int nGuildindex;
	int nRanking;

	msg >> nCharindex
		>> nGuildindex
		>> nRanking;

	CPC* pc			= gserver.m_playerList.Find(nCharindex);
	CGuild* guild 	= gserver.m_guildlist.findguild( nGuildindex ); 

	if( !pc )
		return;
	if( guild )
	{
		guild->guildPointRanking( nRanking );
		GuildPointRankingMsg( rmsg, nCharindex, nGuildindex, nRanking );
		pc->m_pArea->SendToCell(rmsg, pc, true);
	}
}
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD


#ifdef GIFT_EVENT_2007
void OnEventGift2007(CNetMsg & msg)
{
#ifdef ADULT_SERVER
	int nGift[6][7][2] = {
		{
			{ 1416, 1, },
			{ 1422, 1, },
			{ 1423, 1, },
			{ 1424, 1, },
			{ 1425, 1, },
			{ 1420, 4, },
			{ 1404, 1, },
		},
		{
			{ 1416, 1, },
			{ 1426, 1, },
			{ 1427, 1, },
			{ 1428, 1, },
			{ 1429, 1, },
			{ 1420, 4, },
			{ 1404, 1, },
		},
		{
			{ 1416, 1, },
			{ 1430, 1, },
			{ 1431, 1, },
			{ 1432, 1, },
			{ 1433, 1, },
			{ 1420, 4, },
			{ 1404, 1, },
		},
		{
			{ 1416, 1, },
			{ 1420, 4, },
			{ 1404, 1, },
			{ 1434, 1, },
			{ 1435, 1, },
			{ 1436, 1, },
			{ 1437, 1, },
		},
			{
			{ 1416, 1, },
			{ 1420, 4, },
			{ 1404, 1, },
			{ 1438, 1, },
			{ 1439, 1, },
			{ 1440, 1, },
			{ 1441, 1, },
		},
			{
			{ 1416, 1, },
			{ 1420, 4, },
			{ 1404, 1, },
			{ 1442, 1, },
			{ 1443, 1, },
			{ 1444, 1, },
			{ 1445, 1, },
		},
	};
#else
	int nGift[4][2] = {
		{ 1404, 1, },
		{ 841,  1, },
		{ 1413, 1, },
		{ 1284, 1, },
	};
#endif // ADULT_SERVER

	unsigned char errorcode = 0;
	int userindex = 0, charindex = 0;

	msg >> errorcode >> userindex >> charindex;

	CPC * pPC = gserver.m_playerList.Find(charindex);
	if (pPC == NULL)
	{
		return;
	}

	CNetMsg  rmsg;

	switch(errorcode)
	{
	case MSG_CONN_EVENT_GIFT_2007_CHECK_ALLOW:
		{	// 지급
			int i;
			unsigned char nError;
			bool nGive = true;
			int row, col;

#ifdef ADULT_SERVER
			const int giftitemcount = 7;
#else
			const int giftitemcount = 4;		
#endif // ADULT_SERVER
			CItem * giftitem[giftitemcount];
			memset(giftitem, 0, sizeof(CItem *)*giftitemcount);
			for(i = 0; i < giftitemcount; i++)
			{	// 인벤에 아이템 추가
#ifdef ADULT_SERVER
				giftitem[i] = gserver.m_itemProtoList.CreateItem(nGift[pPC->m_job][i][0], -1, 0, 0, nGift[pPC->m_job][i][1]);
#else				
				giftitem[i] = gserver.m_itemProtoList.CreateItem(nGift[i][0], -1, 0, 0, nGift[i][1]);
#endif // ADULT_SERVER

				if (giftitem[i] == NULL)
				{
					nGive = false;		// 디비에 아이템이 없음
					nError = MSG_CONN_EVENT_GIFT_2007_GIVE_DBERROR;
					break;
				}
				
				if (AddToInventory(pPC, giftitem[i], false, true) == false)
				{
					delete giftitem[i];
					giftitem[i] = NULL;

					nGive = false;		// 인벤에 가득 찼음.
					nError = MSG_CONN_EVENT_GIFT_2007_GIVE_FULL;
					break;
				}
			}

			if (nGive == false)
			{	// 지급 실패 처리
				for(i = 0 ; i < giftitemcount; i++)
				{
					if(giftitem[i]  != NULL)
					{
						if(giftitem[i]->tab() != -1)		// 새롭게 추가한 것
						{
							RemoveFromInventory(pPC, giftitem[i], true, false);
						}
						else								// 기존에 있는 거에 추가한 것
						{
							if(pPC->m_invenNormal.FindItem(&row, &col, giftitem[i]->m_itemProto->m_index, giftitem[i]->m_plus, giftitem[i]->m_flag))
							{
								CItem * pItem = pPC->m_invenNormal.GetItem(row, col);
								if (pItem != NULL)
								{
#ifdef ADULT_SERVER
									DecreaseFromInventory(pPC, pItem, nGift[pPC->m_job][i][1]);
#else
									DecreaseFromInventory(pPC, pItem, nGift[i][1]);
#endif // ADULT_SERVER
								}
							}
						}
					}
					else
					{
						break;
					}
				}

				GiftEvent2007Msg(rmsg, (MSG_CONN_EVENT_GIFT_2007_TYPE)nError);
				SEND_Q(rmsg, pPC->m_desc);
				return;
			}


			// 아이템 추가 전송
			for(i = 0 ; i < giftitemcount; i++)
			{
				if(giftitem[i] != NULL)
				{
					CNetMsg itemmsg;
					
					if(giftitem[i]->tab() != -1)		// 인벤에 아이템 새롭게 추가
					{
						ItemAddMsg(itemmsg, giftitem[i]);
						GAMELOG << init("EVENT GIFT 2007 ITEM", pPC)
								<< itemlog(giftitem[i])
								<< end;

						giftitem[i] = NULL;
					}
					else								// 기존에 있는 아이템에 추가
					{
						if(pPC->m_invenNormal.FindItem(&row, &col, giftitem[i]->m_idNum, giftitem[i]->m_plus, giftitem[i]->m_flag))
						{
							CItem * pItem = pPC->m_invenNormal.GetItem(row, col);
							if(pItem != NULL)
							{
#ifdef ADULT_SERVER
								ItemUpdateMsg(itemmsg, pItem, nGift[pPC->m_job][i][1]);
#else
								ItemUpdateMsg(itemmsg, pItem, nGift[i][1]);
#endif // ADULT_SERVER
							}

							delete giftitem[i];
							giftitem[i] = NULL;

							GAMELOG << init("EVENT GIFT 2007 ITEM", pPC)
#ifdef ADULT_SERVER
									<< "ITEM INDEX" << delim << nGift[pPC->m_job][i][0] << delim
									<< "ADD COUNT" << delim << nGift[pPC->m_job][i][1] << delim
#else
									<< "ITEM INDEX" << delim << nGift[i][0] << delim
									<< "ADD COUNT" << delim << nGift[i][1] << delim
#endif // ADULT_SERVER
									<< itemlog(pItem)
									<< end;
						}
					}

					SEND_Q(itemmsg, pPC->m_desc);
				}
			}

			GiftEvent2007Msg(rmsg, MSG_CONN_EVENT_GIFT_2007_GIVE);
			SEND_Q(rmsg, pPC->m_desc);

			if(IS_RUNNING_CONN)
			{
				ConnEventGift2007Msg(rmsg, MSG_CONN_EVENT_GIFT_2007_GIVE, pPC);
				SEND_Q(rmsg, gserver.m_connector);
			}
		}
		break;

	case MSG_CONN_EVENT_GIFT_2007_CHECK_DENY:
	case MSG_CONN_EVENT_GIFT_2007_CHECK_DBERROR:
		{	// error
			GiftEvent2007Msg(rmsg, (MSG_CONN_EVENT_GIFT_2007_TYPE)errorcode);
			SEND_Q(rmsg, pPC->m_desc);
		}
		break;
	}
}
#endif // GIFT_EVENT_2007


#ifdef EVENT_BJMONO_2007
void OnConnEventBjMono2007 ( CNetMsg &msg )
{
	// 커넥터에게 갯수를 받아 응모권 증정
	CNetMsg rmsg;
	int nTicketCount = 0;
	int nCharIndex = 0, nRow=0, nCol =0;

	msg >> nCharIndex >> nTicketCount;

	CPC * pPC = gserver.m_playerList.Find(nCharIndex);
	if (pPC == NULL)
	{ return; }

	if( nTicketCount > 0 )
	{
		CItem* pTicketItem = gserver.m_itemProtoList.CreateItem( 2417 , -1, 0, 0, nTicketCount );
		if( !pTicketItem )
		{
			// 아이템 생성 실패
			return;
		}

		// 아이템 지급
		if( AddToInventory( pPC, pTicketItem, true, true ) )
		{
			if( pTicketItem->tab() < 0 )
			{
				delete pTicketItem;
				pPC->m_invenNormal.FindItem( &nRow, &nCol, 2417, 0, 0 );
				pTicketItem =  pPC->m_invenNormal.GetItem( nRow, nCol );
				ItemUpdateMsg( rmsg, pTicketItem, nTicketCount );
				SEND_Q( rmsg, pPC->m_desc );
			}
			else
			{
				ItemAddMsg( rmsg, pTicketItem );
				SEND_Q( rmsg, pPC->m_desc );
			}
		}
		else
		{
			EventBjMono2007Msg( rmsg, MSG_EVENT_BJMONO_2007_PRICE_FULLINVEN);
			SEND_Q( rmsg, pPC->m_desc );
			return;
		}
		
		EventBjMono2007Msg( rmsg, MSG_EVENT_BJMONO_2007_TICKET_REQ );
		rmsg << nTicketCount;
		SEND_Q( rmsg, pPC->m_desc );

		ConnEventBjMono2007Msg( rmsg, MSG_CONN_EVENT_BJMONO_TICKET_UPDATE ,  pPC );
		rmsg << nTicketCount;
		SEND_Q( rmsg, gserver.m_connector );
	}
	else
	{
		// 받을 응모권 없음 
		EventBjMono2007Msg( rmsg, MSG_EVENT_BJMONO_2007_NO_TICKET );
		rmsg << (int) 0;	// 받을 응모권에러 
		SEND_Q(rmsg, pPC->m_desc);
	}
}
#endif // EVENT_BJMONO_2007

#ifdef EVENT_SSHOT_2007
void OnConnEventSshot2007( CNetMsg& msg )
{
	// 커넥터에서 응모 여부 받아 처리
	CNetMsg rmsg;
	int errorcode = 0;
	int nCharIndex = 0, nRow=0, nCol =0;

	msg >> nCharIndex >> errorcode;

	CPC * pPC = gserver.m_playerList.Find(nCharIndex);
	if (pPC == NULL)
	{ return; }

	switch( errorcode )
	{
	case 0 :
		{
			CItem* pSongDduck = gserver.m_itemProtoList.CreateItem( 1817 , -1, 0, 0, 10 );
			if( !pSongDduck )
			{
				// 아이템 생성 실패
				return;
			}

			// 아이템 지급
			if( AddToInventory( pPC, pSongDduck, true, true ) )
			{
				if( pSongDduck->tab() < 0 )
				{
					delete pSongDduck;
					pPC->m_invenNormal.FindItem( &nRow, &nCol, 1817, 0, 0 );	// 송편
					pSongDduck =  pPC->m_invenNormal.GetItem( nRow, nCol );
					ItemUpdateMsg( rmsg, pSongDduck, 10 );
					SEND_Q( rmsg, pPC->m_desc );
				}
				else
				{
					ItemAddMsg( rmsg, pSongDduck );
					SEND_Q( rmsg, pPC->m_desc );
				}
			}
			else
			{
				EventSshot2007Msg( rmsg, MSG_EVENT_SSHOT_2007_FULLINVEN);
				SEND_Q( rmsg, pPC->m_desc );
				return;
			}
			
			EventSshot2007Msg( rmsg, MSG_EVENT_SSHOT_2007_RECIVE );
			SEND_Q( rmsg, pPC->m_desc );

			ConnEventSshot2007Msg( rmsg, MSG_CONN_EVENT_SSHOT_UPDATE, pPC );
			SEND_Q( rmsg, gserver.m_connector );
		}break;

	case 1 :	// 이미 송편을 받은적 이 있다.
		{
			EventSshot2007Msg( rmsg, MSG_EVENT_SSHOT_2007_ALREADY );
			SEND_Q(rmsg, pPC->m_desc);
		}break;
	case 2 :	// 스샷을 올리적이 없다.
		{
			EventSshot2007Msg( rmsg, MSG_EVENT_SSHOT_2007_NOSSHOT );
			SEND_Q(rmsg, pPC->m_desc);
		}break;
	}
}
#endif // EVENT_SSHOT_2007

#ifdef EVENT_XMAS_2007
void OnConnEventXmas2007( CNetMsg& msg )
{
	unsigned char	cSubtype = 0;
	unsigned int	unPoint = 0;
	CNetMsg			rmsg;
	
	msg >> cSubtype;

	int zoneZuno = gserver.FindZone(0);		//주노
	if ( zoneZuno < 0 )
	{ return; }			// 주노지역 없음

	CArea *area = gserver.m_zones[zoneZuno].m_area;		
	if ( area == NULL )
	{ return; }			// 지역정보 없음

	switch( cSubtype )
	{
	case MSG_CONN_EVENT_XMASTREE_POINT :		
		{
			msg >> unPoint;
			
			gserver.m_unEventXmasTreePoint = unPoint;

			CNPC* npc = area->m_npcList;
			while ( npc )
			{
				if( npc->m_idNum == 484 || npc->m_idNum == 486 /*크리스마스 트리 2종 */ )		// 크리스 마스 트리 NPC 다수
				{
					npc->m_maxHP = npc->m_hp = (int) unPoint;
					CharStatusMsg(rmsg, npc , 0);
					npc->m_pArea->SendToCell(rmsg, npc, true);
				}
				npc = npc->m_pNPCNext;
			}
		}break;

	case MSG_CONN_EVENT_XMASTREE_ITEMDROP :
		{
			int nRemainTime = 0;
			msg >> nRemainTime;

			if( nRemainTime == 5 )
			{
				// 모든 클라이언트에 아이템 드랍 까지 남은 시간 표시
				EventXmas2007Msg( rmsg, MSG_EVENT_XMASTREE_DROP_ITEM );
				rmsg << nRemainTime;
				gserver.m_playerList.SendToAll( rmsg );			
			}
			else if ( nRemainTime == 0 )
			{
				const int nXmasEventItem[7][2] = {
					{ 2590, 40 },			// 소형 빨간띠 막대사탕 40개
					{ 2591, 30 },			// 중형 빨간띠 막대사탕 30개
					{ 2592, 20 },			// 대형 빨간띠 막대사탕 20개
					{ 2593, 40 },			// 소형 파란띠 막대사탕 40개
					{ 2594, 30 },			// 중형 파란띠 막대사탕 30개
					{ 2595, 20 },			// 대형 파란띠 막대사탕 20개
					{ 2596, 20 },			// 할록달록 막대사탕 20개
				};
				// KYD TO DO
				int nItemIndex = GetRandom(0,100)%7;
				// 0 이면 NPC 나무 주변에 아이템 50 V트 드랍

				CNPC* npc = area->m_npcList;
				while ( npc )
				{
					if( npc->m_idNum == 484/*메인 크리스마스 트리*/ )		// 크리스 마스 트리 NPC 다수
					{						
						break;
					}
					npc = npc->m_pNPCNext;
				}

				if( npc == NULL )
					return;		// NPC 나무가 없다.

				for( int idx=0; idx<50; idx++ )
				{
					// ITEM DROP
					CItem* pItem = gserver.m_itemProtoList.CreateItem( nXmasEventItem[nItemIndex][0] , -1, 0, 0, nXmasEventItem[nItemIndex][1] );
					if( pItem )
					{
						CNetMsg rmsg;
						npc->m_pArea->DropItem( pItem, npc );
						ItemDropMsg( rmsg, npc, pItem );
						pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );
					}
				}
				
				GAMELOG << init("EVENT XMAS PRESENT ") 
								<< "ITEM" << delim
								<< nXmasEventItem[nItemIndex][0]
								<< end;
			}
		}break;
	}
}
#endif //EVENT_XMAS_2007


#ifdef ATTENDANCE_EVENT_REWARD
void OnConnEventAttendance( CNetMsg& msg )
{
	int charIndex, count = 0;
	CNetMsg rmsg;
	int i=0;
	msg >> charIndex >> count;

	CPC *ch = gserver.m_playerList.Find( charIndex );
	if( ch == NULL )
		return;

	if( count < 5 )		// 출석일 수 모자람
	{
		GAMELOG << init( "ATTENDANCE REWARD FAILED : " ) << count << end ;
		return; 
	}

	int rewardItem[12][2] =
	{
		{786, 5}, {2547, 5},				// 5	<= count < 10	: 2
		{673, 3}, {674, 3},					// 10	<= count < 15	: 4
		{556, 100}, {44,50},{ 485, 50 },	// 15	<= count < 20	: 7
		{973,10}, {971,10}, { 972, 10 },	// 20	<= count < 25	: 10
		{85, 3 }, { 884, 3}					// 25	<= count		: 12
	};
	// 아이템 지급 
	int rewardItemCount = 0;

	if( 5 <= count && count < 10 )
		rewardItemCount = 2;
	else if ( 10 <= count && count < 15 )
		rewardItemCount = 4;
	else if ( 15 <= count && count < 20 )
		rewardItemCount = 7;
	else if ( 20 <= count && count < 25 )
		rewardItemCount = 10;
	else if ( 25 <= count )
		rewardItemCount = 12;

	// 무개 및 빈공간 확인
	int totalWeigth = 0;
	CItem ** ppItem = new CItem* [rewardItemCount];
	for( i=0; i<rewardItemCount; i++ )
	{
		ppItem[i] = gserver.m_itemProtoList.CreateItem(rewardItem[i][0], -1, 0, 0, rewardItem[i][1]);
		if( ppItem[i] == NULL )
			break;
		totalWeigth += ppItem[i]->m_itemProto->m_weight * ppItem[i]->Count();		
	}

	if( ch->m_weight + totalWeigth >= ch->m_maxWeight * 15 / 10  || ch->m_invenNormal.GetSpace() <= rewardItemCount )
	{
		// 보상 불가 
		// 메모리 해제
		for( i=0; i<rewardItemCount; i++)
		{
			delete ppItem[i];
			ppItem[i] = NULL;
		}
		delete[] ppItem;
		ppItem = NULL;

		SysMsg(rmsg, MSG_SYS_FULL_INVENTORY);
		SEND_Q(rmsg, ch->m_desc );

		return;
	}

	// 이상없으니 아이템 지급
	GAMELOG << init( "ATTENDANCE REWARD ITEM", ch ) << delim;
	for( i=0; i< rewardItemCount; i++ )
	{
		if (AddToInventory(ch, ppItem[i], true, true))
		{
			if (ppItem[i]->tab() < 0)
			{
				int nRow, nCol;
				ch->m_invenNormal.FindItem(&nRow, &nCol, rewardItem[i][0] , 0, 0);
				CItem* pPrevItem = ch->m_invenNormal.GetItem(nRow, nCol);
				ItemUpdateMsg(rmsg, pPrevItem, rewardItem[i][1] );
				SEND_Q(rmsg, ch->m_desc);
				
				GAMELOG << itemlog(pPrevItem);

				delete ppItem[i];
				ppItem[i] = NULL;
			}
			else
			{
				ItemAddMsg(rmsg,  ppItem[i]);
				SEND_Q(rmsg, ch->m_desc);
				GAMELOG << itemlog(ppItem[i]);
				ppItem[i] = NULL;
			}
		}
	}
	GAMELOG << end;
// 지급 성공시
	ConnEventAttendanceRewardMsg( rmsg, MSG_ATTENDANCE_UPDATE, ch->m_desc->m_index ,  ch->m_index );
	SEND_Q(rmsg, gserver.m_connector);

	delete[] ppItem;
	ppItem = NULL;
}	
#endif //ATTENDANCE_EVENT_REWARD

#ifdef EVENT_NOM_MOVIE
void OnEventNom(CNetMsg& msg)
{
	CNetMsg rmsg;

	int nCharIdx = 0;
	unsigned char nError = 0;

	msg >> nError >> nCharIdx;

	CPC *ch = gserver.m_playerList.Find(nCharIdx);
	if (ch == NULL)
		return;	

	if (MSG_CONN_EVENT_NOM_OK == nError)
	{
		if(ch->m_invenNormal.GetSpace() < 1)
		{
			GAMELOG << init("EVENT_NOM_FAIL-No Inven : ") 
					<< "User Index" << delim << ch->m_desc->m_index << delim
					<< "CHAR Index" << delim << ch->m_index << end;

			EventNomMsg(rmsg, MSG_EVENT_NOM_ERROR_FULL_INVEN);
			SEND_Q(rmsg, ch->m_desc);

			if (IS_RUNNING_CONN)
			{
				CNetMsg connMsg;
				ConnEventNomMsg(connMsg, MSG_CONN_EVENT_NOM_LOTTERY_NO_GIVE, ch->m_desc->m_index, ch->m_index);
				SEND_Q(connMsg, gserver.m_connector);
			}
			return;
		}
		
		// 놈놈놈 복권  : 2885
		int nLottery = 2885;
		if (!ch->GiveItem(nLottery, 0, 0, 1, false, false))
		{
			GAMELOG << init("EVENT_NOM_FAIL-Item Fail : ") 
					<< "User Index" << delim << ch->m_desc->m_index << delim
					<< "CHAR Index" << delim << ch->m_index << end;

			// 롤백 
			int r, c;
			if( ch->m_invenNormal.FindItem(&r, &c, nLottery, 0, 0))
			{
				CItem* item = ch->m_invenNormal.GetItem(r, c);
				if(item)
				{
					CNetMsg itemMsg;
					
					// Item 수량 변경
					DecreaseFromInventory(ch, item, 1);
					
					if (item->Count() > 0)
					{
						ItemUpdateMsg(itemMsg, item, -1);
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

			if (IS_RUNNING_CONN)
			{
				CNetMsg connMsg;
				ConnEventNomMsg(connMsg, MSG_CONN_EVENT_NOM_LOTTERY_NO_GIVE, ch->m_desc->m_index, ch->m_index);
				SEND_Q(connMsg, gserver.m_connector);
			}
			return;
		}
		
		EventNomMsg(rmsg, MSG_EVENT_NOM_OK);
		SEND_Q(rmsg, ch->m_desc);
		
		GAMELOG << init("EVENT NOM MOVIE") << "Level" << delim << ch->m_level << delim
			    << "char Index" << delim << ch->m_index << end;

	}
	else if (MSG_CONN_EVENT_NOM_MOVIE_TICKET_OK == nError)
	{
		int r, c;
		if (ch->m_invenNormal.FindItem(&r, &c, 2886, 0, 0))
		{
			CItem* item = ch->m_invenNormal.GetItem(r, c);
			if (item)
			{
				CNetMsg itemMsg;
				
				// Item 수량 변경
				DecreaseFromInventory(ch, item, 1);
				
				if (item->Count() > 0)
				{
					ItemUpdateMsg(itemMsg, item, -1);
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
		}

		EventNomMsg(rmsg, MSG_EVENT_NOM_MOVIE_TICKET_OK);
		SEND_Q(rmsg, ch->m_desc);

		GAMELOG << init("EVENT NOM TICKET") << "Level" << delim << ch->m_level << delim
			    << "char Index" << delim << ch->m_index << end;
	}
}
#endif

#ifdef REQUITAL_EVENT	// 보상 이벤트 
void OnEventRequital(CNetMsg& msg)
{
	CNetMsg rmsg;

	int nCharIdx = 0;
	unsigned char nError = 0;
	int nCheck_eventType= -1;
	int item_index;
	int item_count;
	int i;

	msg >> nError >> nCharIdx >> nCheck_eventType;

	CPC *ch = gserver.m_playerList.Find(nCharIdx);
	if (ch == NULL)
		return;	

	switch( nError )
	{
	case MSG_CONN_EVENT_REQUITAL_OK:  	// 성공인 경우
		// 아이템을 주면서 완료
		// 서버 구동시 아이템 정보 저장한것 가져오기
		
		for ( i=0; i < REQUITAL_EVENT_ITEM_MAXCOUNT; i++ )
		{			
			// 서버 부팅때 저장해 놓은 보상아이템 데이터를 가져온다.
			item_index = gserver.m_CRequital_EventList.m_Requital_EventList[nCheck_eventType].m_item_index[i];
			item_count = gserver.m_CRequital_EventList.m_Requital_EventList[nCheck_eventType].m_item_count[i];

			CItem* pItem = gserver.m_itemProtoList.CreateItem(item_index, -1, 0, 0, item_count);

			if ( pItem != NULL )
			{
				if( !AddToInventory(ch, pItem, true, true) )
				{
					delete pItem;
					pItem = NULL;
				}

				if( pItem->tab() < 0 )
				{
					int r, c;
					if ( ch->m_invenNormal.FindItem(&r, &c, pItem->m_itemProto->m_index, pItem->m_plus, pItem->m_flag) )
					{
						delete pItem;
						pItem = ch->m_invenNormal.GetItem(r, c);
						if (pItem)
							ItemUpdateMsg(rmsg, pItem, item_count);
						SEND_Q(rmsg, ch->m_desc);
						pItem = NULL;
					}
				}
				else
				{
					ItemAddMsg(rmsg, pItem);
					SEND_Q(rmsg, ch->m_desc);
					pItem = NULL;
				}
			}						
		}

		EventRequitalMsg(rmsg, MSG_EVENT_REQUITALM_OK);
		break;

	case MSG_CONN_EVENT_REQUITAL_FAIL:	// 실패
		EventRequitalMsg(rmsg, MSG_EVENT_REQUITAL_FAIL);
		break;

	case MSG_CONN_EVENT_REQUITAL_FAIL_TIME:	// 해당 날짜에 게임을 하지 않아서 실패
		EventRequitalMsg(rmsg, MSG_EVENT_REQUITAL_FAIL_TIME_NOPLAY);
		break;

	case MSG_CONN_EVENT_REQUITAL_FAIL_CREATE_TIME:		// 유저가 게임을 시작한 날짜가 너무 이름
		EventRequitalMsg(rmsg, MSG_EVENT_REQUITAL_FAIL_CREATE_TIME);
		break;

	case MSG_CONN_EVENT_REQUITAL_ALREADY_APPLY:			// 이미 보상 받았음..
		EventRequitalMsg(rmsg, MSG_EVENT_REQUITAL_ERROR_ALREADY_APPLY);
		break;
	}

	SEND_Q(rmsg, ch->m_desc);
	return;
}
#endif // REQUITAL_EVENT

#ifdef IRIS_POINT
void OnConnMessageIPoint(CNetMsg& msg)
{
	CNetMsg rmsg;

	int nCharIdx = 0, nIpiont = 0;
	msg >> nCharIdx >> nIpiont;

	// find character
	CPC *ch = gserver.m_playerList.Find(nCharIdx);
	if (ch == NULL)
		return;	

	SysMsg( rmsg, MSG_SYS_IRISPOINT_ALARM );
	rmsg << nIpiont;
	SEND_Q(rmsg, ch->m_desc);
}
#endif //IRIS_POINT

