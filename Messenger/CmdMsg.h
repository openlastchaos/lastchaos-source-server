#ifndef __CMDMSG_H__
#define __CMDMSG_H__

#include "Descriptor.h"

void FailMsg(CNetMsg& msg, MSG_FAIL_TYPE failtype);
void MsgrWhisperNotfoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sidx, const char* sname);
void MsgrWhisperRep(CNetMsg& msg, int seq, int server, int subno, int zone, int sidx, const char* sname, const char* rname, const char* chat);
void MsgrLogoutRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, const char* id);
#ifdef NEW_DOUBLE_GM
void MsgrDoubleEventRepMsg(CNetMsg& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, int cmd, int nas, int nasget, int exp, int sp, int produce, int pronum);
#endif
#ifdef CHANCE_EVENT
void MsgrChanceEventRepMsg(CNetMsg& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, int cmd, int slevel, int elevel, int nas, int nasget, int exp, int sp, int produce, int pronum);
#endif // CHANCE_EVENT
void MsgrDoubleExpEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int expPercent);

void MsgrFriendChatNotfoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sidx, const char* sname);
void MsgrFriendChatRep(CNetMsg& msg, int seq, int server, int subno, int zone, int sidx, const char* sname, const char* rname, const char* chat);

void MsgrEventRepMsg(CNetMsg& msg, unsigned char type, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int drop);

#ifdef RECOMMEND_SERVER_SYSTEM
void MsgrRecommendMsg(CNetMsg& msg);
#endif // RECOMMEND_SERVER_SYSTEM

void MsgrEventGoldenballVoteMsg(CNetMsg& msg, int nGMCharIndex, const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMin);
void MsgrEventGoldenballGiftMsg(CNetMsg& msg, int nGMCharIndex, const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMin, time_t timeEndVote);

#ifdef GMTOOL
void MsgrGmToolKickIDRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* id);
void MsgrGmToolChatMonitorRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, int gmcharindex, const char* id);
void MsgrGMWhisperRep(CNetMsg& msg, int seq, int server, int sub, int zone, int charindex, const char* rname, const char* chat, int serverno, int subno, int sindex);
void MsgrGMWhisperNotfoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex);
void MsgrGMSilenceRep(CNetMsg& msg, int seq, int serverno, int subno, int zoneno, int server, int sub, int success, int gmcharindex, const char* name);
void MsgrGMToolWhisperRep(CNetMsg& msg, int seq, int serverno, int subno, int zoneno, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrGmToolKickRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* name);
#endif // GMTOOL

#ifdef DOUBLE_ITEM_DROP
void MsgrDoubleItemEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int itemPercent);
#endif // DOUBLE_ITEM_DROP

#ifdef DOUBLE_PET_EXP
void MsgrDoublePetExpEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int PetExpPercent);
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
void MsgrDoubleAttackEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int AttackPercent);
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
void MsgrEventDropItemRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char type, int npcidx, int itemidx, int prob, int thisServer, int thisSub);
#endif // EVENT_DROPITEM

#ifdef NEW_DOUBLE_EVENT_AUTO
void MsgrDoubleEventAutoRepMsg(CNetMsg& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, char cmd, char state);
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef UPGRADE_EVENT
void MsgrUpgradeEventRepMsg(CNetMsg& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, char cmd, int prob);
#endif // UPGRADE_EVENT

#endif	// __CMDMSG_H__
