#ifndef __CMD_MSG_H__
#define __CMD_MSG_H__

#ifdef NEW_SERIAL_PACKAGE_EVENT
#include "PromotionItem.h"
#endif //NEW_SERIAL_PACKAGE_EVENT

void LoginRepMsg(CNetMsg& msg, MSG_CONN_ERRORCODE errcode, const char* id, CUser* user);

void ConnCashItemBalanceRep(CNetMsg& msg, int userindex, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalance);
void ConnCashItemPurchaseRep(CNetMsg& msg, int userindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, int cash);
void ConnCashItemBringRep(CNetMsg& msg, int charindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, bool bPresent, int count, int ctid[]);
void ConnCashItemPurchaselistRep(CNetMsg& msg, int charindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, int count, int idx[], int ctid[]);
void ConnCashItemPurchaseHistoryRep(CNetMsg& msg, int userindex, int charindex, int y, int m, int d);

void ConnCashItemPresentHistoryRep(CNetMsg& msg, int userindex, int charindex, int y, int m, int d, bool bSend);

#ifdef CASH_ITEM_COUPON
void ConnCashItemCouponListRep( CNetMsg& msg, int userindex, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int nCouponCount, CASH_COUPON* pCoupon );
void ConnCashItemPurchaseByCouponRep( CNetMsg& msg, int userindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, int cash);
#endif //CASH_ITEM_COUPON

void PlayerRepMsg(CNetMsg& msg);
void PreopenGiftErrorMsg(CNetMsg& msg, int userindex, int charindex, MSG_CONN_ERRORCODE errcode);
void StashStateMsg(CNetMsg& msg, int userindex, int charindex, char issetpw, char sealed);
void StashCheckPasswordMsg(CNetMsg& msg, int userindex, int charindex, char valid);
void StashChangePasswordMsg(CNetMsg& msg, int userindex, int charindex, const char* newpw, char valid);
void StashSealMsg(CNetMsg& msg, int userindex, int charindex, char valid);
void StashDeletePasswordMsg(CNetMsg& msg, int userindex, int charindex, char valid);
void StashSetNewPasswordMsg(CNetMsg& msg, int userindex, int charindex, char valid);

void Event2pan4panLetterRepMsg(CNetMsg& msg, int userindex, int charindex, MSG_EVENT_2PAN4PAN_WORD_TYPE wordtype);
void Event2pan4panLetterRollbackRepMsg(CNetMsg& msg, int userindex, int charindex, MSG_EVENT_2PAN4PAN_WORD_TYPE wordtype);

void Event2pan4panGoodsRepMsg(CNetMsg& msg, int userindex, int charindex, int count, int* goodsindex, int* itemindex, LONGLONG* itemcount);
void Event2pan4panGoodsCommitMsg(CNetMsg& msg, int userindex, int charindex, int count, int* goodsindex);

void Event2pan4panBoxRepMsg(CNetMsg& msg, int userindex, int charindex, int count, int* goodsindex, int* itemindex, LONGLONG* itemcount);
void Event2pan4panBoxCommitMsg(CNetMsg& msg, int userindex, int charindex, int count, int* goodsindex);

void Event2pan4panMoneyRepMsg(CNetMsg& msg, int userindex, int charindex, int itemindex);
void RankerRepMsg(CNetMsg& msg, int charindex);

#ifdef EVENT_PCBANG
void PcbangItemGiveMsg(CNetMsg& msg, const char* idname, char type);
#endif

#ifdef EVENT_PACKAGE_ITEM
void CouponConfirmMsg(CNetMsg& msg, int charindex, MSG_EVENT_ERROR_TYPE subtype, int cIndex = -1, int type = -1);

#ifdef LC_HBK
void CouponUseMsg(CNetMsg& msg, int charindex, MSG_EVENT_ERROR_TYPE subtype, int randidx1, int randcnt1, int randidx2, int randcnt2, int type = -1);
#else
void CouponUseMsg(CNetMsg& msg, int charindex, MSG_EVENT_ERROR_TYPE subtype, int type = -1);
#endif // LC_HBK

#endif

#ifdef NEW_SERIAL_PACKAGE_EVENT
void CouponConfirmMsg_new(CNetMsg& msg, int charindex, MSG_EVENT_ERROR_TYPE errorCode, int promotionIndex = -1, int promotionType = -1, int GiftItemCount = 0, PromotionItem* pPromotionItem = NULL );
void CouponUseMsg_new(CNetMsg& msg, int charindex, MSG_EVENT_ERROR_TYPE errorCode, int promotionType = -1, int GiftItemCount = 0, PromotionItem* pPromotionItem = NULL );
#endif  // NEW_SERIAL_PACKAGE_EVENT

#ifdef EVENT_OPEN_BETA_GIFT
void OpenBetaGiftErrMsg(CNetMsg& msg, int userindex, int charindex, MSG_CONN_ERRORCODE errcode);
#endif // EVENT_OPEN_BETA_GIFT

void MoveServerOKMsg(CNetMsg& msg, int nUserIndex);

#ifdef LIMIT_CATALOG
// 게임서버가 접속할때나 한정판매 아이템이 판매될때 모든 게임서버로 보내는 메세지 
// 게임서버가 접속할때는 모든 한정판매 아이템 리스트와 모든 팔린 갯수를 보내고
// 한정판매 아이템이 팔릴때는 팔린 인덱스와 팔린 갯수(무조건 1이겟죠?)을 보낸다.
void LimitCatalogMsg(CNetMsg& msg, CLimitSell* limit = NULL);
#endif

void ConnGparaPromotionMsg(CNetMsg& msg, MSG_CONN_GPARA_PROMOTION_TYPE subtype, int nUserIndex, int nCharIndex);
void ConnOCN_GooPromotionMsg(CNetMsg& msg, MSG_CONN_OCN_GOO_PROMOTION_TYPE subtype, int nUserIndex, int nCharIndex);
void ConnMSNPromotionMsg(CNetMsg& msg, MSG_CONN_MSN_PROMOTION_TYPE subtype, int nUserIndex, int nCharIndex);

#ifdef CREATE_EVENT
void ConnCreateEventMsg(CNetMsg& msg, MSG_CONN_CREATEEVENT_TYPE subtype, int nUserIndex, int nCharIndex);
#endif // CREATE_EVENT

#ifdef EVENT_GOMDORI_2007
void ConnEventGomdori2007StatusMsg(CNetMsg& msg, int nCharIndex, int nCount, int* nStatus);
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_FLOWERTREE_2007
void ConnEventFlowerTree2007Msg(CNetMsg& msg, MSG_CONN_EVENT_FLOWERTREE_2007_TYPE subtype, int nCharIndex);
#endif //EVENT_FLOWERTREE_2007

#ifdef NEW_GUILD_POINT_RANKING_NOTICE
void GuildPointRankerRepMsg( CNetMsg& msg, int charindex, int guildindex, int ranking );
#endif // NEW_GUILD_POINT_RANKING_NOTICE


#ifdef GIFT_EVENT_2007
void EventGift2007Msg(CNetMsg & msg, MSG_CONN_EVENT_GIFT_2007_TYPE subtype, int userindex, int charindex);
#endif // GIFT_EVENT_2007

#ifdef EVENT_BJMONO_2007
void ConnEventBjMono2007Msg(CNetMsg& msg, int charIndex , int count);
#endif // EVENT_BJMONO_2007

#ifdef EVENT_SSHOT_2007
void ConnEventSshot2007Msg(CNetMsg& msg, int charIndex , int errorcode);
#endif //EVENT_SSHOT_2007

#ifdef EVENT_XMAS_2007
void ConnEventXmas2007Msg( CNetMsg& msg, MSG_CONN_EVENT_XMAS_2007_TYPE subtype );
#endif //EVENT_XMAS_2007

#ifdef ATTENDANCE_EVENT_REWARD
void ConnEventAttendanceRewardMsg( CNetMsg& msg, int charIndex, int count );
#endif //ATTENDANCE_EVENT_REWARD

#ifdef EVENT_NOM_MOVIE
void ConnEventNomMsg(CNetMsg& msg, int nCharIndex, MSG_CONN_EVENT_NOM_TYPE subtype);
#endif

#ifdef REQUITAL_EVENT	// 보상 이벤트 
void ConnEventRequitalMsg(CNetMsg& msg, int nCharIndex, int nCheck_eventType, MSG_CONN_EVENT_REQUITAL_TYPE subtype);
#endif // REQUITAL_EVENT

#ifdef IRIS_POINT
void ConnIpointMessageMsg( CNetMsg& msg, int charIndex, int ipoint );
#endif //IRIS_POINT

#include "BillCmdMsg.h"

#endif
