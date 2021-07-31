#include "stdhdrs.h"
#include "Server.h"
#include "DBCmd.h"
#include "CmdMsg.h"


void LoginRepMsg(CNetMsg& msg, MSG_CONN_ERRORCODE errcode, const char* id, CUser* user)
{
	msg.Init(MSG_CONN_REP);
	msg	<< (unsigned char)MSG_CONN_LOGIN_REP
		<< id
		<< (unsigned char)errcode;
	if (errcode != MSG_CONN_ERROR_SUCCESS)
		return ;
	msg	<< user->m_index
		<< (unsigned char)user->m_usertype
		<< user->m_timestamp
		<< (unsigned char)user->m_paytype
		<< (unsigned char)user->m_location
		<< user->m_timeremain;

	msg << user->m_userFlag;//0627

#ifdef CHARDEL_CHECKID
	msg << user->m_identification;
#endif
#ifdef EVENT_PROMOTION_SITE
	msg << user->m_proSite;
#endif//EVENT_PROMOTION_SITE
}

void ConnCashItemBalanceRep(CNetMsg& msg, int userindex, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalance)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_CASHITEM_BALANCE_REP
		<< userindex
		<< (unsigned char) errorcode
		<< cashBalance;
}

void ConnCashItemPurchaseRep(CNetMsg& msg, int userindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, int cash)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_CASHITEM_PURCHASE_REP
		<< userindex
		<< (unsigned char) errorCode
		<< cash;
}

void ConnCashItemBringRep(CNetMsg& msg, int charindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, bool bPresent, int count, int ctid[])
{
	msg.Init(MSG_CONN_REP);
	if( bPresent )
		msg << (unsigned char) MSG_CONN_CASHITEM_GIFT_RECV;
	else
		msg << (unsigned char) MSG_CONN_CASHITEM_BRING_REP;

    msg	<< charindex
		<< (unsigned char) errorCode
		<< count;

	for(int i = 0; i < count; i++)
	{
		msg << ctid[i];
	}
}

void ConnCashItemPurchaselistRep(CNetMsg& msg, int charindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, int count, int idx[], int ctid[])
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_CASHITEM_PURCHASELIST_REP
		<< charindex
		<< (unsigned char) errorCode
		<< count;

	for(int i = 0; i < count; i++)
	{
		msg << idx[i]
			<< ctid[i];
	}
}

void ConnCashItemPurchaseHistoryRep(CNetMsg& msg, int userindex, int charindex, int y, int m, int d)
{
	CLCString sql(4096);
	int ctid = -1, preCtid = -1, i;

	sql.Format("select * from t_purchase0%d where a_user_idx = %d and a_server = %d "
		"and year(a_pdate) = %d and month(a_pdate) = %d and dayofmonth(a_pdate) = %d order by a_ctid ", userindex % 10, userindex, gserver.m_serverno, y, m, d);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	cmd.SetQuery(sql);

	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_CASHITEM_PURCHASEHISTORY_REP;

	i = 0;
	if( !cmd.Open() )
	{
		msg << charindex 
			 << (unsigned char) MSG_EX_CASHITEM_ERROR_CONN << i;
		return;
	}

	if( !cmd.MoveFirst() )
	{
		msg << charindex 
			 << (unsigned char) MSG_EX_CASHITEM_ERROR_SUCCESS << i;
		return;
	}

	CNetMsg ctmsg;
	ctmsg.Init();
	int j = 1;

	do
	{
		cmd.GetRec("a_ctid", ctid);

		if( preCtid != -1 && preCtid != ctid )
		{
			ctmsg << i
				  << preCtid;
			i = 0;
			j++;
		}

		preCtid = ctid;
		i++;
	}while(cmd.MoveNext() );

	ctmsg << i << ctid;

	msg << charindex 
		 << (unsigned char) MSG_EX_CASHITEM_ERROR_SUCCESS << j << ctmsg;

}

#ifdef CASH_ITEM_GIFT
void ConnCashItemPresentHistoryRep(CNetMsg& msg, int userindex, int charindex, int y, int m, int d, bool bSend)
{
	// 응답				: errorcode(uc) count(n) ctid(n) sendcharName(str)
	CLCString sql(10240);
	int ctid = -1;
	CLCString charName( MAX_CHAR_NAME_LENGTH + 1);

	if( bSend )
	{
		int i;
		CLCString temp(4096);
		for(i = 0; i < 9; i++)
		{
			sprintf(g_buf, "t_gift0%d", i);
			temp.Format("select a_ctid, a_recv_char_name as a_charName from %s where a_send_user_idx = %d and a_server = %d "
				"and year(a_send_date) = %d and month(a_send_date) = %d and dayofmonth(a_send_date) = %d union all ", 
				g_buf, userindex, gserver.m_serverno, y, m, d);

			sql += temp;
		}
		sprintf(g_buf, "t_gift0%d", i);
		temp.Format("select a_ctid, a_recv_char_name as a_charName from %s where a_send_user_idx = %d and a_server = %d "
				"and year(a_send_date) = %d and month(a_send_date) = %d and dayofmonth(a_send_date) = %d ", 
				g_buf, userindex, gserver.m_serverno, y, m, d);
		sql += temp;
	}
	else
	{
		sql.Format("select a_ctid, a_send_char_name as a_charName from t_gift0%d where a_recv_user_idx = %d and a_server = %d "
			"and year(a_send_date) = %d and month(a_send_date) = %d and dayofmonth(a_send_date) = %d order by a_send_date ", 
			userindex % 10, userindex, gserver.m_serverno, y, m, d);
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	msg.Init(MSG_CONN_REP);
	
	if( bSend )
		msg << (unsigned char) MSG_CONN_CASHITEM_GIFT_SENDHISTORY;
	else
		msg << (unsigned char) MSG_CONN_CASHITEM_GIFT_RECVHISTORY;

	msg << charindex;

	cmd.SetQuery(sql);

	int count = 0;
	if( cmd.Open() && cmd.MoveFirst() )
	{
		msg << (unsigned char) MSG_EX_CASHITEM_ERROR_SUCCESS;

		count = cmd.m_nrecords;

		msg << count;
		do
		{
			cmd.GetRec("a_ctid", ctid);
			cmd.GetRec("a_charName", charName);

			msg << ctid
				<< charName;

		} while( cmd.MoveNext() );
	}
	else
	{
		msg << (unsigned char) MSG_EX_CASHITEM_ERROR_SUCCESS;
		msg << (int) 0;
	}
}
#endif

#ifdef LIMIT_CATALOG
void LimitCatalogMsg(CNetMsg& msg, CLimitSell* limit)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_LIMIT_CATALOG;

	if( !limit )
	{
		int count = gserver.m_limitSell.GetCount();
		int i;
		CLimitSell* sellList;
		void* pos;
		pos = gserver.m_limitSell.GetHead();

		msg << count;
		for( i = 0; i < count; i++)
		{
			sellList = gserver.m_limitSell.GetData(pos);
			msg << sellList->GetIndex()
				<< sellList->GetSell();
			pos = gserver.m_limitSell.GetNext(pos);
		}
	}
	else
	{
		msg << 1
			<< limit->GetIndex()
			<< limit->GetSell();
	}
}
#endif

#ifdef CASH_ITEM_COUPON
void ConnCashItemCouponListRep( CNetMsg& msg, int userindex, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int nCouponCount, CASH_COUPON* pCoupon )
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_CASHITEM_COUPON
		<< userindex
		<< (unsigned char) errorcode
		<< nCouponCount;

	int count=0;
	for( count=0; count<nCouponCount; count++ )
	{
		msg << pCoupon[count].name
			<< pCoupon[count].id
			<< pCoupon[count].serial
			<< pCoupon[count].amount
			<< pCoupon[count].limitAmount;		
	}

}

void ConnCashItemPurchaseByCouponRep( CNetMsg& msg, int userindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, int cash)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_CASHITEM_PURCHASE_WITH_COUPON
		<< userindex
		<< (unsigned char) errorCode
		<< cash;
}

#endif //CASH_ITEM_COUPON

void PlayerRepMsg(CNetMsg& msg)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_PLAYER_REP
		<< gserver.m_serverno
		<< gserver.m_maxSubServer;

	// zone 0번을 가지고 있는 넘을 찾는다
	CDescriptor* d = gserver.m_desclist;
	CDescriptor** result = new CDescriptor*[gserver.m_maxSubServer];
	memset(result, 0, sizeof(CDescriptor*) * gserver.m_maxSubServer);

	int i = 0;

	while (d)
	{
		if (d->m_bStartServer)
		{
			result[d->m_subno - 1] = d;
		}
		d = d->m_pNext;
	}

	for (i = 1; i <= gserver.m_maxSubServer; i++)
	{
		msg << i;
		if (result[i - 1])
		{
			msg << gserver.m_userList[result[i - 1]->m_subno - 1].m_count
				<< result[i - 1]->m_ipAddr
				<< result[i - 1]->m_portNumber;
		}
		else
		{
			msg << (int)-1
				<< ""
				<< (int)0;
		}
	}
}

void PreopenGiftErrorMsg(CNetMsg& msg, int userindex, int charindex, MSG_CONN_ERRORCODE errcode)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_PREOPEN_GIFT
		<< userindex
		<< charindex
		<< (char)errcode;
}

void StashStateMsg(CNetMsg& msg, int userindex, int charindex, char issetpw, char sealed)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_STASH_STATE
		<< userindex
		<< charindex
		<< issetpw
		<< sealed;
}

void StashCheckPasswordMsg(CNetMsg& msg, int userindex, int charindex, char valid)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_STASH_CHECK_PASSWORD
		<< userindex
		<< charindex
		<< ""
		<< valid;
}

void StashChangePasswordMsg(CNetMsg& msg, int userindex, int charindex, const char* newpw, char valid)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_STASH_CHANGE_PASSWORD
		<< userindex
		<< charindex
		<< ""
		<< newpw
		<< valid;
}

void StashSealMsg(CNetMsg& msg, int userindex, int charindex, char valid)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_STASH_SEAL
		<< userindex
		<< charindex
		<< valid;
}

void StashDeletePasswordMsg(CNetMsg& msg, int userindex, int charindex, char valid)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_STASH_DELETE_PASSWORD
		<< userindex
		<< charindex
		<< valid;
}

void StashSetNewPasswordMsg(CNetMsg& msg, int userindex, int charindex, char valid)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_STASH_SET_NEW_PASSWORD
		<< userindex
		<< charindex
		<< valid;
}

void Event2pan4panLetterRepMsg(CNetMsg& msg, int userindex, int charindex, MSG_EVENT_2PAN4PAN_WORD_TYPE wordtype)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_LETTER_REP
		<< userindex
		<< charindex
		<< wordtype;
}

void Event2pan4panLetterRollbackRepMsg(CNetMsg& msg, int userindex, int charindex, MSG_EVENT_2PAN4PAN_WORD_TYPE wordtype)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_LETTER_ROLLBACK_REP
		<< userindex
		<< charindex
		<< wordtype;
}

void Event2pan4panBoxRepMsg(CNetMsg& msg, int userindex, int charindex, int count, int* goodsindex, int* itemindex, LONGLONG* itemcount)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_BOX_REP
		<< userindex
		<< charindex
		<< count;
	int i;
	for (i = 0; i < count; i++)
	{
		msg << goodsindex[i]
			<< itemindex[i]
			<< itemcount[i];
	}
}

void Event2pan4panBoxCommitMsg(CNetMsg& msg, int userindex, int charindex, int count, int* goodsindex)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_BOX_COMMIT
		<< userindex
		<< charindex
		<< count;
	int i;
	for (i = 0; i < count; i++)
		msg << goodsindex[i];
}

void Event2pan4panGoodsRepMsg(CNetMsg& msg, int userindex, int charindex, int count, int* goodsindex, int* itemindex, LONGLONG* itemcount)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_GOODS_REP
		<< userindex
		<< charindex
		<< count;
	int i;
	for (i = 0; i < count; i++)
	{
		msg << goodsindex[i]
			<< itemindex[i]
			<< itemcount[i];
	}
}

void Event2pan4panGoodsCommitMsg(CNetMsg& msg, int userindex, int charindex, int count, int* goodsindex)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_GOODS_COMMIT
		<< userindex
		<< charindex
		<< count;
	int i;
	for (i = 0; i < count; i++)
		msg << goodsindex[i];
}

void Event2pan4panMoneyRepMsg(CNetMsg& msg, int userindex, int charindex, int itemindex)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_MONEY_REP
		<< userindex
		<< charindex
		<< itemindex;
}

void RankerRepMsg(CNetMsg& msg, int charindex)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_RANKER_REP
		<< charindex;
}
#ifdef EVENT_PCBANG
void PcbangItemGiveMsg(CNetMsg& msg, const char* idname, char type)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_PCBANG_ITEM_GIVE
		<< idname
		<< type;
}

#endif


#ifdef NEW_SERIAL_PACKAGE_EVENT
void CouponConfirmMsg_new(CNetMsg& msg, int charindex, MSG_EVENT_ERROR_TYPE errorCode, int promotionIndex, int promotionType, int GiftItemCount, PromotionItem* pPromotionItem )
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_COUPON_CONFIRM_NEW
		<< charindex
		<< (unsigned char) errorCode
		<< promotionIndex
		<< promotionType
		<< GiftItemCount;
	for( int i = 0; i < GiftItemCount; i++ )
	{
		msg << pPromotionItem[i].m_itemIndex
			<< pPromotionItem[i].m_itemCount
			<< pPromotionItem[i].m_itemPlus
			<< pPromotionItem[i].m_itemFlag;
//		pPromotionItem[i].m_itemOption0;
//		pPromotionItem[i].m_itemOption1;
//		pPromotionItem[i].m_itemOption2;
//		pPromotionItem[i].m_itemOption3;
//		pPromotionItem[i].m_itemOption4;
	}
}

void CouponUseMsg_new(CNetMsg& msg, int charindex, MSG_EVENT_ERROR_TYPE errorCode, int promotionType, int GiftItemCount, PromotionItem* pPromotionItem )
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_COUPON_USE_NEW
		<< charindex
		<< (unsigned char) errorCode
		<< promotionType
		<< GiftItemCount;
		for( int i = 0; i < GiftItemCount; i++ )
	{
		msg << pPromotionItem[i].m_itemIndex
			<< pPromotionItem[i].m_itemCount
			<< pPromotionItem[i].m_itemPlus
			<< pPromotionItem[i].m_itemFlag;
//		pPromotionItem[i].m_itemOption0;
//		pPromotionItem[i].m_itemOption1;
//		pPromotionItem[i].m_itemOption2;
//		pPromotionItem[i].m_itemOption3;
//		pPromotionItem[i].m_itemOption4;
	}

}
#endif // NEW_SERIAL_PACKAGE_EVENT

#ifdef EVENT_PACKAGE_ITEM 
void CouponConfirmMsg(CNetMsg& msg, int charindex, MSG_EVENT_ERROR_TYPE subtype, int cIndex, int type)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_COUPON_CONFIRM
		<< charindex
		<< (unsigned char) subtype
		<< cIndex
		<< type;
}

#ifdef LC_HBK
void CouponUseMsg(CNetMsg& msg, int charindex, MSG_EVENT_ERROR_TYPE subtype, int randidx1, int randcnt1, int randidx2, int randcnt2, int type)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_COUPON_USE
		<< charindex
		<< (unsigned char) subtype
		<< type
		<< randidx1
		<< randcnt1
		<< randidx2
		<< randcnt2;
}
#else // LC_HBK
void CouponUseMsg(CNetMsg& msg, int charindex, MSG_EVENT_ERROR_TYPE subtype, int type)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char) MSG_CONN_COUPON_USE
		<< charindex
		<< (unsigned char) subtype
		<< type;
}
#endif // LC_HBK

#endif // EVENT_PACKAGE_ITEM


#ifdef EVENT_OPEN_BETA_GIFT
void OpenBetaGiftErrMsg(CNetMsg& msg, int userindex, int charindex, MSG_CONN_ERRORCODE errcode)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_OPEN_BETA_GIFT
		<< userindex
		<< charindex
		<< (char)errcode;
}
#endif // EVENT_OPEN_BETA_GIFT

void MoveServerOKMsg(CNetMsg& msg, int nUserIndex)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_MOVESERVER_OK
		<< nUserIndex;
}

void ConnGparaPromotionMsg(CNetMsg& msg, MSG_CONN_GPARA_PROMOTION_TYPE subtype, int nUserIndex, int nCharIndex)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_GPARA_PROMOTION
		<< subtype
		<< nUserIndex
		<< nCharIndex;
}

void ConnOCN_GooPromotionMsg(CNetMsg& msg, MSG_CONN_OCN_GOO_PROMOTION_TYPE subtype, int nUserIndex, int nCharIndex)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_OCN_GOO_PROMOTION
		<< subtype
		<< nUserIndex
		<< nCharIndex;
}

void ConnMSNPromotionMsg(CNetMsg& msg, MSG_CONN_MSN_PROMOTION_TYPE subtype, int nUserIndex, int nCharIndex)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_MSN_PROMOTION
		<< subtype
		<< nUserIndex
		<< nCharIndex;
}

#ifdef CREATE_EVENT
void ConnCreateEventMsg(CNetMsg& msg, MSG_CONN_CREATEEVENT_TYPE subtype, int nUserIndex, int nCharIndex)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_CREATEEVENT
		<< subtype
		<< nUserIndex
		<< nCharIndex;
}
#endif // CREATE_EVENT

#ifdef EVENT_GOMDORI_2007
void ConnEventGomdori2007StatusMsg(CNetMsg& msg, int nCharIndex, int nCount, int* nStatus)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_GOMDORI_2007
		<< (unsigned char)MSG_CONN_EVENT_GOMDORI_2007_STATUS
		<< nCharIndex
		<< nCount;
	int i;
	for (i = 0; i < nCount; i++)
		msg << nStatus[i];
}
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_FLOWERTREE_2007
void ConnEventFlowerTree2007Msg(CNetMsg& msg, MSG_CONN_EVENT_FLOWERTREE_2007_TYPE subtype, int nCharIndex)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_FLOWERTREE_2007
		<< (unsigned char)subtype
		<< nCharIndex;
}
#endif //EVENT_FLOWERTREE_2007

#ifdef NEW_GUILD_POINT_RANKING_NOTICE
void GuildPointRankerRepMsg( CNetMsg& msg, int charindex, int guildindex, int ranking )
{
	msg.Init( MSG_CONN_REP );
	msg << ( unsigned char)MSG_CONN_GUILD_POINT_RANKER_REP
		<< charindex
		<< guildindex
		<< ranking;
}
#endif // NEW_GUILD_POINT_RANKING_NOTICE

#ifdef GIFT_EVENT_2007
void EventGift2007Msg(CNetMsg & msg, MSG_CONN_EVENT_GIFT_2007_TYPE subtype, int userindex, int charindex)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_GIFT_2007
		<< (unsigned char)subtype
		<< userindex
		<< charindex;
}
#endif // GIFT_EVENT_2007

#ifdef EVENT_BJMONO_2007
void ConnEventBjMono2007Msg(CNetMsg& msg, int charIndex , int count)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_BJMONO_2007
		<< charIndex
		<< count;
}
#endif // EVENT_BJMONO_2007

#ifdef EVENT_SSHOT_2007
void ConnEventSshot2007Msg( CNetMsg& msg, int charIndex , int errorcode )
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_SSHOT_2007
		<< charIndex
		<< errorcode;
}
#endif // EVENT_SSHOT_2007

#ifdef EVENT_XMAS_2007
void ConnEventXmas2007Msg( CNetMsg& msg, MSG_CONN_EVENT_XMAS_2007_TYPE subtype )
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_XMAS_2007
		<< (unsigned char)subtype;
}
#endif //EVENT_XMAS_2007

#ifdef ATTENDANCE_EVENT_REWARD
void ConnEventAttendanceRewardMsg( CNetMsg& msg, int charIndex, int count )
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_ATTENDANCE_COUNT
		<< charIndex << count;
}
#endif //ATTENDANCE_EVENT_REWARD

#ifdef IRIS_POINT //IRISPOINT
void ConnIpointMessageMsg( CNetMsg& msg, int charIndex, int ipoint )
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_ADD_IPOINT
		<< charIndex << ipoint;
}
#endif //IRIS_POINT

#ifdef EVENT_NOM_MOVIE
void ConnEventNomMsg(CNetMsg& msg, int nCharIndex, MSG_CONN_EVENT_NOM_TYPE subtype)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_NOM
		<< (unsigned char)subtype
		<< nCharIndex;
}
#endif

#ifdef REQUITAL_EVENT	// 보상 이벤트 
void ConnEventRequitalMsg(CNetMsg& msg, int nCharIndex, int nCheck_eventType, MSG_CONN_EVENT_REQUITAL_TYPE subtype)
{
	msg.Init(MSG_CONN_REP);
	msg << (unsigned char)MSG_CONN_EVENT_REQUITAL
		<< (unsigned char)subtype
		<< nCharIndex
		<< nCheck_eventType;
}
#endif // REQUITAL_EVENT