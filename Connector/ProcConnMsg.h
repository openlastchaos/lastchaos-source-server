#ifndef __PROC_CONNECTOR_MSG_H__
#define __PROC_CONNECTOR_MSG_H__

void ConnLogin(CDescriptor* d, CNetMsg& msg);
void ConnLogout(CDescriptor* d, CNetMsg& msg);
void ConnPlayerReq(CDescriptor* d, CNetMsg& msg);
void ConnPlaying(CDescriptor* d, CNetMsg& msg);

MSG_CONN_ERRORCODE ReadDB(const char* name, const char* pw, int server, int subno, const char* ip, CUser** user, char playmode);

/////////////////////
// Function name	: WriteDB
// Description	    : 유저 테이블에 데이터 저장
//                  : name이나 ip가 NULL 일 경우 t_connect_log는 저장하지 않는다
// Return type		: MSG_CONN_ERRORCODE 
//            		: 에러코드
// Argument         : int index
//                  : 포털 인덱스
// Argument         : int server
//                  : 서버군 번호
// Argument         : int subno
//                  : 서브 번호
// Argument         : int zone
//                  : 존 번호
// Argument         : int timestamp
//                  : 플레이시간
// Argument         : const char* ip
//                  : 접속 아이피
// Argument         : const char* name
//                  : 아이디
// Argument         : bool bLogin
//                  : 로그인시 설정
MSG_CONN_ERRORCODE WriteDB(int index, int server, int subno, int zone, int timestamp, const char* name, const char* ip, bool bLogin, int pcbangTime = 0);


/////////////////////
// Function name	: WriteDB
// Description	    : 유저 테이블에 데이터 저장 (로그아웃처리)
// Return type		: bool 
//            		: 성공 실패 여부
// Argument         : const char* name
//                  : 접속 아이디
bool WriteDB(const char* name);

void ConnPreopenGift(CDescriptor* d, CNetMsg& msg);
void ConnLevelupPoint(CDescriptor* d, CNetMsg& msg);
void ConnStashState(CDescriptor* d, CNetMsg& msg);
void ConnStashCheckPassword(CDescriptor* d, CNetMsg& msg);
void ConnStashChangePassword(CDescriptor* d, CNetMsg& msg);
void ConnStashSeal(CDescriptor* d, CNetMsg& msg);
void ConnStashDeletePassword(CDescriptor* d, CNetMsg& msg);
void ConnStashNewPassword(CDescriptor* d, CNetMsg& msg);

void ConnEvent2pan4panLetterReq(CDescriptor* d, CNetMsg& msg);
void ConnEvent2pan4panLetterRollbackReq(CDescriptor* d, CNetMsg& msg);

void ConnEvent2pan4panBoxReq(CDescriptor* d, CNetMsg& msg);
void ConnEvent2pan4panBoxCommit(CDescriptor* d, CNetMsg& msg);

void ConnEvent2pan4panGoodsReq(CDescriptor* d, CNetMsg& msg);
void ConnEvent2pan4panGoodsCommit(CDescriptor* d, CNetMsg& msg);

void ConnEvent2pan4panMoneyReq(CDescriptor* d, CNetMsg& msg);
void ConnRankerReq(CDescriptor* d, CNetMsg& msg);

#ifdef EVENT_PACKAGE_ITEM
void ConnCouponConfirm(CDescriptor* d, CNetMsg& msg);
void ConnCouponUse(CDescriptor* d, CNetMsg& msg);
#endif

#ifdef NEW_SERIAL_PACKAGE_EVENT
void ConnCouponConfirm_new(CDescriptor* d, CNetMsg& msg);
//void ConnCouponConfirm_new( CDescriptor* d, int charIndex , const char*  serial );
void ConnCouponUse_new(CDescriptor* d, CNetMsg& msg);
#endif // NEW_SERIAL_PACKAGE_EVENT


#ifdef LIMIT_CATALOG
void ConnSetLimit(CDescriptor* d, CNetMsg& msg);
#endif

#ifdef EVENT_PARTNER_CODENEO
void ConnEventPartnerCodeNEOReq( CDescriptor* d, CNetMsg& msg );
#endif

#ifdef EVENT_OPEN_BETA_GIFT
void ConnOpenBetaGift(CDescriptor* d, CNetMsg& msg);
#endif

#ifdef JPN_OCN_GOO_PROMOTION
void ConnOCN_GooPromotion(CDescriptor* d, CNetMsg& msg);
#endif // JPN_OCN_GOO_PROMOTION

#ifdef JPN_GPARA_PROMOTION
void ConnGparaPromotion(CDescriptor* d, CNetMsg& msg);
#endif // JPN_GPARA_PROMOTION

#ifdef JPN_MSN_PROMOTION
void ConnMSNPromotion(CDescriptor* d, CNetMsg& msg);
#endif // JPN_MSN_PROMOTION

#ifdef CREATE_EVENT
void ConnCreateEvent(CDescriptor* d, CNetMsg& msg);
#endif // CREATE_EVENT

#ifdef EVENT_GOMDORI_2007
void ConnEventGomdori2007(CDescriptor* d, CNetMsg& msg);
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_FLOWERTREE_2007
void ConnEventFlowerTree2007(CDescriptor* d, CNetMsg& msg);
#endif //EVENT_FLOWERTREE_2007

#ifdef GMTOOL
void ConnGMToolCommand( CDescriptor* d, CNetMsg& msg );
void ConnGMToolChatMonitor( CDescriptor* d, CNetMsg& msg );
void ConnGMToolChatting( CDescriptor* d, CNetMsg& msg );
#endif // GMTOOL

#ifdef NEW_GUILD_POINT_RANKING_NOTICE
void ConnGuildPointRankingReq( CDescriptor* d, CNetMsg& msg );
#endif // NEW_GUILD_POINT_RANKING_NOTICE

#ifdef GIFT_EVENT_2007
void ConnEventGift2007(CDescriptor * d, CNetMsg & msg);
#endif // GIFT_EVENT_2007

#ifdef EVENT_BJMONO_2007
void ConnEvnetBjMono2007(CDescriptor * d, CNetMsg & msg);
#endif // EVENT_BJMONO_2007

#ifdef EVENT_SSHOT_2007
void ConnEvnetSshot2007(CDescriptor * d, CNetMsg & msg);
#endif //EVENT_SSHOT_2007

#ifdef EVENT_XMAS_2007
void ConnEventXmas2007( CDescriptor* d, CNetMsg& msg );
#endif //EVENT_XMAS_2007

#ifdef ATTENDANCE_EVENT
void WriteAttendance(int userindex, int serverno);
#endif // ATTENDANCE_EVENT

#ifdef CLIENT_RESTART
void ConnClientRestart( CDescriptor* d, CNetMsg& msg );
#endif //CLIENT_RESTART

#ifdef ATTENDANCE_EVENT_REWARD
void ConnAttendanceEventReward( CDescriptor* d, CNetMsg& msg );
#endif //ATTENDANCE_EVENT_REWARD


#ifdef IRIS_POINT
bool AddIpoint ( int user_index, int char_index, int type, int nPoint );
void ConnIpointAddMsg( CDescriptor* d, CNetMsg& msg );
#endif //IRIS_POINT
//*/

#ifdef EVENT_NOM_MOVIE
void ConnEventNomMsg(CDescriptor* d, CNetMsg& msg);
#endif // EVENT_NOM_MOVIE

#ifdef REQUITAL_EVENT	// 보상 이벤트 
void ConnEventRequitalMsg(CDescriptor* d, CNetMsg& msg);
#endif // REQUITAL_EVENT

#if defined (CHAR_LOG) && defined (LC_KOR)
void ConnCharLogMsg( CDescriptor* d, CNetMsg& msg);
#endif //#if defined (CHAR_LOG) && defined (LC_KOR)

#if defined (USE_TENTER_BILLING)
#include "ProcConnMsg_TLD.inh"
#else
#include "ProcConnMsg_KOR.inh"
#endif

#endif
