#ifndef __DO_FUNCTION_H__
#define __DO_FUNCTION_H__

void do_Request(CNetMsg& msg, CDescriptor* dest);
void do_Reply(CNetMsg& msg, CDescriptor* dest);

#ifdef RECOMMEND_SERVER_SYSTEM
void do_Request_Recommend(CNetMsg& msg);
#endif // RECOMMEND_SERVER_SYSTEM

void do_Request_EventGoldenball(CNetMsg& msg);

#endif