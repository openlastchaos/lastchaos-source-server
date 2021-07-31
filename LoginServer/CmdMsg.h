#ifndef __ACMDMSG_H__
#define __ACMDMSG_H__

#include "Descriptor.h"

void FailMsg(CNetMsg& msg, MSG_FAIL_TYPE failtype);
void LogoutReqMsg(CNetMsg& msg, int server, int subnum, int zone, CDescriptor* d);
void ConLogoutReqMsg(CNetMsg& msg, char* d);
void PlayerReqMsg(CNetMsg& msg);
void PlayerNumMsg(CNetMsg& msg, int recentServer, int recentSubNum, int idx);
void PlayerRealIP( CNetMsg& msg, const char* ip );

#ifdef CHECK_SECURE_CARD
void CheckSecureCardMsg(CNetMsg & msg, CDescriptor * desc);
#endif // CHECK_SECURE_CARD

#endif
