#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "DBCmd.h"

#ifdef FEEITEM
void do_Ex_CashItem(CPC* ch, CNetMsg& msg)
{
	//msg.MoveFirst();

	unsigned char subtype;

	msg >> subtype;
	switch(subtype)
	{
	case MSG_EX_CASHITEM_BALANCE_REQ:
		doCashItemBalanceReq(ch, msg);
		break;
	case MSG_EX_CASHITEM_PURCHASE_REQ:
		doCashItemPuchase(ch, msg);
		break;
	case MSG_EX_CASHITEM_BRING_REQ:
		doCashItemBring(ch, msg);
		break;
	case MSG_EX_CASHITEM_PURCHASELIST_REQ:
		doCashItemPurchaseList(ch, msg);
		break;
	case MSG_EX_CASHITEM_PURCHASEHISTORY_REQ:
		doCashItemPurchaseHistory(ch, msg);
		break;
#ifdef CASH_MOONSTONE
	case MSG_EX_CASHITEM_MOONSTONE_START:
		doCashItemMoonStoneStart(ch, msg);
		break;
	case MSG_EX_CASHITEM_MOONSTONE_STOP:
		doCashItemMoonStoneStop(ch, msg);
		break;
#endif
#ifdef CASH_ITEM_GIFT
	case MSG_EX_CASHITEM_GIFT_REQ:			// 선물 보내기 요청	: charName(str) Msg(str) count(n) idx(n) ctid(n)
		doCashItemGift(ch, msg);
		break;
	case MSG_EX_CASHITEM_GIFT_SENDHISTORY_REQ:  // 선물 내역 리스트 요청 : y(n) m(c) d(c)
		doCashItemGiftSendHistory(ch, msg);
		break;
	case MSG_EX_CASHITEM_GIFT_RECVHISTORY_REQ:  // 선물 내역 리스트 요청 : y(n) m(c) d(c)
		doCashItemGiftRecvHistory(ch, msg);
		break;
	case MSG_EX_CASHITEM_GIFT_RECVLIST_REQ:	   // 선물 받은 리스트 요청
		doCashItemGiftRecvList(ch, msg);
		break;
	case MSG_EX_CASHITEM_GIFT_RECV_REQ:			// 선물 받기		: count(n) idx(n) ctid(n)
		doCashItemGiftRecv(ch, msg);
		break;
#endif
#ifdef CASH_ITEM_COUPON
	case MSG_EX_CASHITEM_CUPON_REQ :
		doCashItemCouponListReq(ch, msg);
		break;

	case MSG_EX_CASHITEM_PURCHASE_WITH_COUPON_REQ :
		doCashItemPuchaseByCoupon(ch, msg);
		break;
#endif //CASH_ITEM_COUPON
	}
}

void doCashItemPurchaseHistory(CPC* ch, CNetMsg& msg)
{
	int y;
	char m;
	char d;

	msg >> y
		>> m
		>> d;

	CNetMsg rmsg;
	if(IS_RUNNING_CONN)
	{
		ConnCashItemPurchaseHistoryReq(rmsg, ch->m_desc->m_index, ch->m_index, y, m, d);
		SEND_Q(rmsg, gserver.m_connector);
	}
	else
	{
		// 커넥터 에러 메세지
	}
}

void doCashItemPurchaseList(CPC* ch, CNetMsg& msg)
{
	//MSG_CONN_CASHITEM_PURCHASELIST_REQ, // 구입목록     : userindex(n) charindex(n)

	CNetMsg rmsg;
	if(IS_RUNNING_CONN)
	{
		ConnCashItemPurchaselistReq(rmsg, ch->m_desc->m_index, ch->m_index);
		SEND_Q(rmsg, gserver.m_connector);
		return;
	}
	else
	{
		CashItemPurchaseListRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_CONN, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

}

void CashItemBringProcess(CPC* ch, CNetMsg& msg, bool bPresent)
{
	// 케릭터 ?걸고 현재 인벤칸/ 소지량 정보를 확인후 결과에따라 에러 메세지를 보내거나 아이템을 지급후 커넥터에 세팅 요청을 한다
	// 커넥터가 제대로 세팅 못했을 경우 롤백한다.
	//MSG_EX_CASHITEM_BRING_REQ,			// 가져오기			: count(n) idx(n) ctid(n)
	int count = 0;
	int ctid[10], idx[10], i;
	int invenSpace = 0;
	int remainWeight = 0;
	int sumWeight = 0;
	int sumItem = 0;
	memset(idx, -1, sizeof(idx) );
	memset(ctid, -1, sizeof(ctid) );

	msg >> count;

	CNetMsg rmsg;
	if(count == 0)
	{
		// 옮길 물품이 없어요
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;
		CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, bPresent);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	

	ch->SetPlayerState(PLAYER_STATE_CASHITEM);
	ch->m_billReqTime = PULSE_BILLITEMREQ;
	invenSpace = ch->m_invenNormal.GetSpace();
	remainWeight = ch->m_maxWeight * 3 / 2 - ch->m_weight;

	CCtItemList* ctItemList = NULL;
	for(i = 0; i < count; i++)
	{
		msg >> idx[i]
			>> ctid[i];		
		
		int j = 0;
		CCatalog* m_catalog = gserver.m_catalogList.Find(ctid[i]);

		if( !m_catalog )
		{
			ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
			ch->m_billReqTime = 0;
			CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, bPresent);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		ctItemList = m_catalog->GetCtItemList();
		while(ctItemList && j < ctItemList->GetCount())
		{
			CItem* item = ctItemList->GetItem(j);
			if( !item )
			{
				ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
				CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, bPresent);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			sumWeight += item->m_itemProto->m_weight;
			sumItem++;
			j++;
		}
	}

	if( sumWeight > remainWeight || sumItem > invenSpace)
	{
		// 케릭터 락해제
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;
		CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_LACKINVEN, bPresent);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(IS_RUNNING_CONN)
	{
		GAMELOG << init("CASH_ITEM_BRING_REQ", ch )
				<< (bPresent ? "PRESENT" : "NOT_PRESENT") << delim
				<< count << delim;
		CCtItemList* ctItemList = NULL;
		for(i = 0; i < count; i++)
		{

			int j = 0;
			CCatalog* m_catalog = gserver.m_catalogList.Find(ctid[i]);

			if( !m_catalog )
			{
				// 케릭터 락해제
				ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
				ch->m_billReqTime = 0;
				CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, bPresent);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			GAMELOG << m_catalog->GetIndex() << delim
					<< m_catalog->GetName()  << delim;

			ctItemList = m_catalog->GetCtItemList();

			if( !ctItemList )
			{
				// 케릭터 락해제
				ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
				ch->m_billReqTime = 0;
				CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, bPresent);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			int itemcount = ctItemList->GetCount();


			GAMELOG << itemcount << delim;

			CItem* item;
			for(j = 0; j < itemcount; j++)
			{
				item = ctItemList->GetItem(j);

				if( !item )
				{
					// 케릭터 락해제
					ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
					ch->m_billReqTime = 0;
					CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, bPresent);
					SEND_Q(rmsg, ch->m_desc);
					return;
				}

				ch->GiveItem(item->m_itemProto->m_index, 
					item->m_plus, 
					item->m_flag, 
					item->Count() );

//				GAMELOG << itemlog(item);		생성되는 아이템 로그로 대체
			}
		}

		ConnCashItemBringReq(rmsg, bPresent, ch->m_desc->m_index, ch->m_index, count, idx, ctid);
		SEND_Q(rmsg, gserver.m_connector);

		GAMELOG << end;

	}
	else
	{
		// 콘넥터 에라 메세지 
		// 케릭터 락해제
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		CashItemBringRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_CONN, bPresent);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void doCashItemBring(CPC* ch, CNetMsg& msg)
{	
	CashItemBringProcess(ch, msg, false);
}

void doCashItemBalanceReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg cmsg;
	ConnCashItemBalanceReq(cmsg, ch->m_desc->m_index, ch->m_desc->m_idname);

	if(IS_RUNNING_CONN)
	{
		SEND_Q(cmsg, gserver.m_connector);
	}
}
void doCashItemPuchase(CPC* ch, CNetMsg& msg)
{
	int cash, count, i, requestcash;
	int ctid[10];
	int sumCash = 0;
	memset(ctid, -1, sizeof(ctid) );

	msg >> cash
		>> count;

	// 케릭터 락
	ch->SetPlayerState(PLAYER_STATE_CASHITEM);
	ch->m_billReqTime = PULSE_BILLITEMREQ;

	CNetMsg cmsg;
	CCatalog* catal = NULL;

	GAMELOG << init("CASH_ITEM_PURCHASE_REQ", ch)
			<< ch->m_cashBalance << delim
			<< count << delim;

	int limitCount;
	limitCount = 0;
	for(i = 0; i < count; i++)
	{
		msg >> ctid[i];

		catal = gserver.m_catalogList.Find(ctid[i]);
		
		if( !catal || !catal->GetEnable())
		{
			ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
			ch->m_billReqTime = 0;
			CashItemPurchaseRepMsg(cmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, cash);
			SEND_Q(cmsg, ch->m_desc);
			return;
		}

		sumCash +=catal->GetCash();

		GAMELOG << catal->GetIndex() << delim
				<< catal->GetName() << delim
				<< catal->GetCash() << delim
				<< catal->GetMileage() << delim;
#ifdef LIMIT_CATALOG
		// 구입요청한 물품이 한정 판매 물품일 경우 리미트가 남아있지 않으면 에러 메세지를 보냄
		void* pos;
		pos = gserver.m_limitCT.FindData(ctid[i]);
		if( pos )
		{
			limitCount++;

			if(  catal->GetToSell() < limitCount )
			{
				ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
				ch->m_billReqTime = 0;
				CashItemPurchaseRepMsg(cmsg, MSG_EX_CASHITEM_ERROR_NOTHAVECT, cash);
				SEND_Q(cmsg, ch->m_desc);
				return;
			}
		}
		GAMELOG << catal->GetToSell() << delim;
#endif
	}
	
	msg >> requestcash;

	if(sumCash != requestcash || sumCash > cash)
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;
		CashItemPurchaseRepMsg(cmsg, MSG_EX_CASHITEM_ERROR_LACKCASH, cash);
		SEND_Q(cmsg, ch->m_desc);
		return;
	}

#ifdef USE_TENTER_BILLING
	ConnCashItemPurchaseReqMsg(cmsg, ch->m_desc->m_index, ch->m_desc->m_idname, ch->m_index, cash, count, ctid, requestcash);
#else
	ConnCashItemPurchaseReqMsg(cmsg, ch->m_desc->m_index, ch->m_desc->m_idname, ch->m_index, cash, count, ctid);
#endif

	if(IS_RUNNING_CONN)
	{
		GAMELOG << requestcash << end;
		SEND_Q(cmsg, gserver.m_connector);
	}
	else
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;
		CashItemPurchaseRepMsg(cmsg, MSG_EX_CASHITEM_ERROR_CONN, cash);
		SEND_Q(cmsg, ch->m_desc);
	}
}

void doCashItemMoonStoneStop(CPC* ch, CNetMsg& msg)
{
#ifdef CASH_MOONSTONE

	if( ch->m_cashMonnStoneGrade == -1 || ch->m_cashmoonStoneItemIndex == -1 || ch->m_cashmoonStoneRemoveCtId == -1 )
		return;

//2차 직업이 있는 경우
//타이탄 - 909 드래곤 액스
//하이랜더 - 909 드래곤 액스
//워마스터 - 908 드래곤 블레이드

//나이트 - 887 드래곤 소드
//로열나이드 - 907 - 드래곤 블레이드
//템플나이트 - 887 드래곤 소드

//힐러 - 911 드래곤 보우
//아처 - 911 드래곤 보우
//클레릭 - 910 드래곤 완드

//메이지 - 912 드래곤 스틱
//위자드 - 913 드래곤 스테프
//위치 - 912 드래곤 스틱

//로그 - 914 드래곤 대거
//어쌔신 - 914 드래곤 대거
//레인저 - 915 드래곤 암릿보우

//소서러 - 917 드래곤 로드
//엘리멘탈 리스트 - 917 드래곤 로드
//스페셜 리스트 - 916 드래곤 사이드
	static int petUniWeaponTable[6][3] =
	{
		{909, 909, 908},
		{887, 907, 887},
		{911, 911, 910},
		{912, 913, 912},
		{914, 914, 915},
		{917, 917, 916},
	};

	static int petUniDefenTable[6][5] = 
	{
		{918, 919, 920, 921, 922},
		{923, 924, 925, 926, 927},
		{938, 939, 940, 941, 942},
		{928, 929, 930, 931, 932},
		{933, 934, 935, 936, 937},
		{943, 944, 945, 946, 947},
	};

	CNetMsg rmsg;
	try
	{
		CInventory* inven = GET_INVENTORY(ch, 0);
		if (inven == NULL)
		{
			throw MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM;
		}

		CItem* item = inven->GetItem(ch->m_cashmoonStoneItemIndex);
		if(!item)
		{
			throw MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM;
		}
		
		//#define		ITEM_FLAG_CASHMOON	(1 << 13)	// 캐쉬 문스톤 룰렛 가능 여부 
		// 기간제 아이템을 사용 못함 
		if(item->m_itemProto && item->m_used != -1 && !(item->m_itemProto->m_flag & ITEM_FLAG_CASHMOON) && item->m_wearPos != -1 )
		{
			throw MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM;
		}

		// 삭제할 상품, 지급할 상품 가져오기
		CCatalog* giveCatalog = gserver.m_catalogList.Find(ch->m_cashMoonStoneGiveCtId);
		CCatalog* removeCatalog = gserver.m_catalogList.Find(ch->m_cashmoonStoneRemoveCtId);

		// 삭제할 상품이 목록에 없으면 에러, 
		if( !removeCatalog )
			throw MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM;

		int removeCount = removeCatalog->GetCtItemList()->GetItem(0)->Count();
		int count = 0;
		int itemIndex = -1;

		// 지급할 상품이 있으면 그대로 지급하고,
		if( giveCatalog )
		{
			CItem* pItem = giveCatalog->GetCtItemList()->GetItem(0);

			if( !ch->GiveItem(pItem->m_itemProto->m_index, pItem->m_plus, pItem->m_flag, pItem->Count() ) )
			{
				throw MSG_EX_CASHITEM_MOONSTONE_ERROR_INVEN;
			}
		}
		else
		{
			// 없으면 등급에 따라, 동일한 상품 지급 혹은 퀵대형 물약 지급
			switch( ch->m_cashMonnStoneGrade )
			{
			case 0:
				count = 3 * removeCount;
				break;
			case 1:
				count = 2 * removeCount;
				break;
			case 2:
			case 3:
				count = 1 * removeCount;
				break;
			}

			if( ch->m_cashMonnStoneGrade != 4 )
			{
				itemIndex = item->m_itemProto->m_index;

				if( !ch->GiveItem(item->m_itemProto->m_index, item->m_plus, item->m_flag, count  ) )
				{
					throw MSG_EX_CASHITEM_MOONSTONE_ERROR_INVEN;
				}
			}
			else
			{
				itemIndex = 855;

				int recoveryId = gserver.m_catalogList.m_recoveryCtid;
				int cash;
				if( recoveryId != -1 )
					cash = gserver.m_catalogList.Find(recoveryId)->GetCash();
				else
					cash = 50;

				count = (int) ((removeCatalog->GetCash() / cash) * 0.6);

				if( !ch->GiveItem(itemIndex, 0, 0,  count ) )
				{
					throw MSG_EX_CASHITEM_MOONSTONE_ERROR_INVEN;
				}
			}
		}

		GAMELOG << init("CASH_ITEM_USE_MOONSTONE_EXCHANGE", ch)
				<< itemlog(item) << delim
				<< "GRADE" << delim
				<< ch->m_cashMonnStoneGrade << delim
				<< "REMOVE_CT" << delim 
				<< removeCatalog->GetIndex() << delim 
				<< removeCatalog->GetName() << delim 
				<< removeCatalog->GetCash() << delim;

		if( giveCatalog )
		{
			GAMELOG << "GIVE_CT" << delim 
					<< giveCatalog->GetIndex() << delim 
					<< giveCatalog->GetName() << delim 
					<< giveCatalog->GetCash() << end;
		}
		else
		{
			GAMELOG << "GIVE_ID" << delim 
					<< ch->m_cashMoonStoneGiveCtId << delim 
					<< "ITEMINDEX" << delim 
					<< itemIndex << delim 
					<< "COUNT" << delim 
					<< count << end;
		}

		// 아이템 없애고
		CNetMsg itemmsg;
		DecreaseFromInventory(ch, item, removeCount);
		
		if (item->Count() > 0)
		{
			ItemUpdateMsg(itemmsg, item, -removeCount);
			SEND_Q(itemmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(itemmsg, item);
			SEND_Q(itemmsg, ch->m_desc);
			
			ch->RemoveItemFromQuickSlot(item);
			RemoveFromInventory(ch, item, true, true);
		}

		// 추가 보상품 지급
		if( ch->m_cashMonnStoneGrade == 0 )
		{
			if( removeCatalog->GetCash() < 400 )
			{
				if( rand() % 2 )
				{
					// 고제 5개
					ch->GiveItem(85, 0, 0, 5, true);
				}
				else
				{
					// 펫 유니크 무기 드래곤
					ch->GiveItem(petUniWeaponTable[(int)ch->m_job][(int)ch->m_job2], 0, 0, 1, true);
				}
			}
			else
			{
				if( rand() % 2 )
				{
					ch->GiveItem(1706, 0, 0, 1, true);
				}
				else
				{
					ch->GiveItem(1707, 0, 0, 1, true);
				}
			}
		}
		else if ( ch->m_cashMonnStoneGrade == 1 )
		{
			if( removeCatalog->GetCash() < 400 )
			{
				if( rand() % 2 )
				{
					ch->GiveItem(1709, 0, 0, 1, true);
				}
				else
				{
					ch->GiveItem(1708, 0, 0, 1, true);
				}
			}
			else
			{
				if( rand() % 2 )
				{
					// 고제 5개
					ch->GiveItem(85, 0, 0, 1, true);
				}
				else
				{
					// 펫 유니크 방어구중 하나, 
					int rand = GetRandom(0, 5);
					ch->GiveItem(petUniDefenTable[(int)ch->m_job][rand], 0, 0, 1, true);
				}
			}
		}

		ch->m_cashmoonStoneItemIndex = -1;
		ch->m_cashMoonStoneGiveCtId = -1;
		ch->m_cashmoonStoneRemoveCtId = -1;
		ch->m_cashMonnStoneGrade = -1;
	}
	catch( MSG_EX_CASHITEM_MOONSTONE_ERROR_TYPE ex )
	{
		CashItemMoonstoneStartRepMsg(rmsg, ex, -1);
		SEND_Q(rmsg, ch->m_desc);

		ch->m_cashmoonStoneItemIndex = -1;
		ch->m_cashMoonStoneGiveCtId = -1;
		ch->m_cashmoonStoneRemoveCtId = -1;
		ch->m_cashMonnStoneGrade = -1;
	}
#endif
}

void doCashItemMoonStoneStart(CPC* ch, CNetMsg& msg)
{
#ifdef CASH_MOONSTONE

#if defined( LC_JPN ) || defined( BWTEST )
	const int S_GRADE_PROB	= 10;
	const int A_GRADE_PROB	= 40;
	const int B_GRADE_PROB	= 140;
	const int C_GRADE_PROB	= 320;
//	const int D_GRADE_PROB	= 10;
	static double D_PRICE_SCOPE  = 0.1;
	static double D_PRICE_SCOPE1	= 0.5;
#else
	const int S_GRADE_PROB	= 10;
	const int A_GRADE_PROB	= 40;
	const int B_GRADE_PROB	= 100;
	const int C_GRADE_PROB	= 650;
	static double D_PRICE_SCOPE  = 0.3;
	static double D_PRICE_SCOPE1	= 0.9;
#endif

	ch->m_cashmoonStoneItemIndex = -1;
	ch->m_cashmoonStoneRemoveCtId = -1;
	ch->m_cashMonnStoneGrade = -1;
	ch->m_cashMoonStoneGiveCtId = -1;

	char tab, row, col;
	int itemidx, ctid;
	
	msg >> tab >> row >> col
		>> itemidx >> ctid;

	CNetMsg rmsg;
	try
	{
		CInventory* inven = GET_INVENTORY(ch, tab);
		if (inven == NULL)
		{
			throw MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM;
		}

		CItem* item = inven->GetItem(itemidx);
		if(!item)
		{
			throw MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM;
		}
		
		//#define		ITEM_FLAG_CASHMOON	(1 << 13)	// 캐쉬 문스톤 룰렛 가능 여부 
		// 기간제 아이템을 사용 못함 
		if(item->m_itemProto && item->m_used != -1 && !(item->m_itemProto->m_flag & ITEM_FLAG_CASHMOON) && item->m_wearPos != -1 )
		{
			throw MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM;
		}

		char grade = -1;
		// 등급확률 1, 3, 6, 55, 35
		int rand = GetRandom(1, 1000);
		if( rand < S_GRADE_PROB )
			grade = 0;
		else if( rand < A_GRADE_PROB )
			grade = 1;
		else if( rand < B_GRADE_PROB )
			grade = 2;
		else if( rand < C_GRADE_PROB )
			grade = 3;
		else
			grade = 4;
	
		//int rand = GetRandom(1, 1000);
		//if( rand < 10 )
		//	grade = 0;
		//else if( rand < 40 )
		//	grade = 1;
		//else if( rand < 100 )
		//	grade = 2;
		//else if( rand < 650 )
	//		grade = 3;2
	//	else
	//		grade = 4;

		// 보상 등급을 정하고, 등급에 따라 보상물품 결정
		// 5등급, min, max
			static double gradeTable[5][2] = {
			{ 2.5 , 3.5 },
			{ 2.0 , 2.5 },
			{ 1.5 , 2.0 },
			{ 0.8 , 1.4 },
			{ D_PRICE_SCOPE , D_PRICE_SCOPE1 } };

//		static double gradeTable[5][2] = {
//			{ 2.5 , 3.5 },
//			{ 2.0 , 2.5 },
//			{ 1.5 , 2.0 },
//			{ 0.8 , 1.4 },
//			{ 0.3 , 0.9 } };

		// 보내온 아이템의 상품 얻어오기
		// 1421 : 결합주문서
		// 1418 : 제련석 
		CCatalog* removeCatalog = NULL;
	
		// 삭제할 상품의 카탈로그 얻어오기
		removeCatalog = gserver.m_catalogList.Find(ctid);
		void* pos = NULL;
		CLCList<int> cashMoonCatal(NULL);
		if( !removeCatalog )
			throw MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM;

		// 세트 상품인지, 삭제할 상품과 보내온 아이템이 같은 넘인지, 
		if( removeCatalog->GetType() == CTYPE_SET )
		{
			throw MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM;
		}

		if( removeCatalog->GetCtItemList()->GetItem(0)->m_itemProto->m_index != item->m_itemProto->m_index  )
		{
			if( item->m_itemProto->m_index == 1419 || item->m_itemProto->m_index == 1421 || item->m_itemProto->m_index == 2895 || item->m_itemProto->m_index == 3270 )
			{
				if( item->m_plus > 0 )
				{
					CCatalog* checkCatalog = gserver.m_catalogList.FindItemToCatalog(item->m_itemProto->m_index - 1);
					if( !checkCatalog )
						throw MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM;
					
					if( removeCatalog->GetIndex() != checkCatalog->GetIndex() )
						throw MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM;
				}
			}
			else
			{
				throw MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM;
			}
		}

		// 해당 등급에 맞는 상품 리스트를 만들고
		gserver.m_catalogList.GetCashMoonCatalogList(cashMoonCatal, removeCatalog->GetCash() * gradeTable[grade][0], removeCatalog->GetCash() * gradeTable[grade][1]);
		
		// 해당 등급에 맞는 상품을 얻어 옴
		pos = cashMoonCatal.GetPosition(GetRandom(0, cashMoonCatal.GetCount() - 1));

		int catalogIndex = -1;

		// 해당 등급에 맞는 상품이 있으면 
		if( pos )
			catalogIndex = cashMoonCatal.GetData(pos);

		// 인벤토리에 빈칸이 있는지, 
		int r, c;
		if( !ch->m_invenNormal.FindEmpty(&r, &c) )
		{
			throw MSG_EX_CASHITEM_MOONSTONE_ERROR_INVEN;
		}

		CashItemMoonstoneStartRepMsg(rmsg, MSG_EX_CASHITEM_MOONSTONE_ERROR_SUCCESS, grade);
		SEND_Q(rmsg, ch->m_desc);

		// 지급할 상품 인덱스, 삭제할 아이템 정보, 삭제할 상품 인덱스를 저장,
		ch->m_cashMoonStoneGiveCtId = catalogIndex;
		ch->m_cashmoonStoneItemIndex = item->m_index;
		ch->m_cashmoonStoneRemoveCtId = removeCatalog->GetIndex();
		ch->m_cashMonnStoneGrade = grade;
	}
	catch( MSG_EX_CASHITEM_MOONSTONE_ERROR_TYPE ex )
	{
		CashItemMoonstoneStartRepMsg(rmsg, ex, -1);
		SEND_Q(rmsg, ch->m_desc);

		ch->m_cashmoonStoneItemIndex = -1;
		ch->m_cashMoonStoneGiveCtId = -1;
		ch->m_cashmoonStoneRemoveCtId = -1;
		ch->m_cashMonnStoneGrade = -1;
	}
#endif
}

#ifdef CASH_ITEM_GIFT
void doCashItemGift(CPC* ch, CNetMsg& msg)
{
	// 보내는 케릭터 lock 걸고,
	// t_character에서 케릭터가 있는지 확인,
	// 가져오기 프로세스를 이용하여 선물 디비에 세팅 
	// 세팅 성공여부에 따라 rep 메세지 보내기

	int count = 0;
	int idx[10], ctid[10], i;

	CLCString recvcharName( MAX_CHAR_NAME_LENGTH + 1);
	CLCString strMsg( MAX_GIFT_MESSAGE_LENGTH + 1);

	// 락걸기 
	ch->SetPlayerState(PLAYER_STATE_CASHITEM);
	ch->m_billReqTime = PULSE_BILLITEMREQ;

	msg >> recvcharName
		>> strMsg
		>> count;

	CNetMsg rmsg;
	if( count < 1 )
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;

		CashItemGiftSendRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_GIFT_NOCTID);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	for( i = 0; i < count; i++ )
	{
		msg >> idx[i]
			>> ctid[i];
	}

	// <<< kjban 2008.04.21 
	// 선물 받을 대상의 캐릭명 길이 체크
	if(recvcharName.Length() == 0)
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;

		CashItemGiftSendRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_GIFT_WRONGCHAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	// >>>

	if( strcmp(ch->GetName(), recvcharName) == 0 )
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;

		CashItemGiftSendRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_GIFT_WRONGCHAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	HelperGiftCharReq(rmsg, ch->m_desc->m_index, ch->m_index, recvcharName, strMsg, count, idx, ctid);
	if( IS_RUNNING_HELPER )
	{
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true, 4);
	}
	else
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;
	}
}

void doCashItemGiftSendHistory(CPC* ch, CNetMsg& msg)
{
	int y;
	char m;
	char d;

	msg >> y
		>> m
		>> d;

	CNetMsg rmsg;
	if(IS_RUNNING_CONN)
	{
		ConnCashItemPresentHistoryReq(rmsg, true, ch->m_desc->m_index, ch->m_index, y, m, d);
		SEND_Q(rmsg, gserver.m_connector);
	}
}
void doCashItemGiftRecvHistory(CPC* ch, CNetMsg& msg)
{
	int y;
	char m;
	char d;

	msg >> y
		>> m
		>> d;

	CNetMsg rmsg;
	if(IS_RUNNING_CONN)
	{
		ConnCashItemPresentHistoryReq(rmsg, false, ch->m_desc->m_index, ch->m_index, y, m, d);
		SEND_Q(rmsg, gserver.m_connector);
	}
}

void doCashItemGiftRecvList(CPC* ch, CNetMsg& msg)
{
	//// listflag					: (1 << 0): start, (1 << 1): end, (1 << 2): empty
	// 받는 케릭터 락걸고, 선물 받은 리스트를 커넥터에 요청
	// 리스트를 받아 보냄
	CNetMsg rmsg;
	if(IS_RUNNING_CONN)
	{
		ConnCashItemGiftRecvListReqMsg(rmsg, ch->m_desc->m_index, ch->m_index);
		SEND_Q(rmsg, gserver.m_connector);

		return;
	}
}

void doCashItemGiftRecv(CPC* ch, CNetMsg& msg)
{
	// 받는 케릭터 락걸고 가져오기 프로세스 돌리기
	CashItemBringProcess(ch, msg, true);
}
#endif //#ifdef CASH_ITEM_GIFT


#ifdef CASH_ITEM_COUPON
void doCashItemCouponListReq( CPC* ch, CNetMsg& msg )
{
	CNetMsg cmsg;
	ConnCashItemCouponListReq(cmsg, ch->m_desc->m_index,  ch->m_desc->m_idname);

	if(IS_RUNNING_CONN)
	{
		SEND_Q(cmsg, gserver.m_connector);
	}
}

void doCashItemPuchaseByCoupon( CPC* ch, CNetMsg& msg )
{
	int cash, count, i, requestcash;
	int ctid[10];
	int sumCash = 0;
	int	couponID = 0, couponAmount = 0;

	memset(ctid, -1, sizeof(ctid) );

	msg >> cash
		>> couponID
		>> couponAmount
		>> count;

	// 케릭터 락
	ch->SetPlayerState(PLAYER_STATE_CASHITEM);
	ch->m_billReqTime = PULSE_BILLITEMREQ;

	CNetMsg cmsg;
	CCatalog* catal = NULL;

	GAMELOG << init("CASH_ITEM_PURCHASE_BY_COUPON_REQ", ch)
			<< ch->m_cashBalance << delim
			<< "COUPON ID : " << couponID << delim
			<< count << delim;

	int limitCount;
	limitCount = 0;
	for(i = 0; i < count; i++)
	{
		msg >> ctid[i];

		catal = gserver.m_catalogList.Find(ctid[i]);
		
		if( !catal || !catal->GetEnable())
		{
			ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
			ch->m_billReqTime = 0;
			CashItemPurchaseRepMsg(cmsg, MSG_EX_CASHITEM_ERROR_NOTITEM, cash );
			SEND_Q(cmsg, ch->m_desc);
			return;
		}

		sumCash +=catal->GetCash();

		GAMELOG << catal->GetIndex() << delim
				<< catal->GetName() << delim
				<< catal->GetCash() << delim
				<< catal->GetMileage() << delim;
#ifdef LIMIT_CATALOG
		// 구입요청한 물품이 한정 판매 물품일 경우 리미트가 남아있지 않으면 에러 메세지를 보냄
		void* pos;
		pos = gserver.m_limitCT.FindData(ctid[i]);
		if( pos )
		{
			limitCount++;

			if(  catal->GetToSell() < limitCount )
			{
				ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
				ch->m_billReqTime = 0;
				CashItemPurchaseRepMsg(cmsg, MSG_EX_CASHITEM_ERROR_NOTHAVECT, cash);
				SEND_Q(cmsg, ch->m_desc);
				return;
			}
		}
		GAMELOG << catal->GetToSell() << delim;
#endif
	}
	sumCash -= couponAmount;

	msg >> requestcash;

	if(sumCash != requestcash || sumCash > cash)
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;
		CashItemPurchaseRepMsg(cmsg, MSG_EX_CASHITEM_ERROR_LACKCASH, cash);
		SEND_Q(cmsg, ch->m_desc);
		return;
	}

	if(IS_RUNNING_CONN)
	{
		GAMELOG << requestcash << end;

		ConnCashItemPurchaseByCouponReqMsg( cmsg, ch->m_desc->m_index, ch->m_desc->m_idname, ch->m_index, couponID, cash, count, ctid );
		SEND_Q(cmsg, gserver.m_connector);
	}
	else
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;
		CashItemPurchaseRepMsg(cmsg, MSG_EX_CASHITEM_ERROR_CONN, cash);
		SEND_Q(cmsg, ch->m_desc);
	}
}
#endif // CASH_ITEM_COUPON


#endif