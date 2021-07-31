#include "stdhdrs.h"
#include "CmdMsg.h"
#include "Server.h"
#include "DBCmd.h"

void FailMsg(CNetMsg& msg, MSG_FAIL_TYPE failtype)
{
	msg.Init(MSG_FAIL);
	msg << (unsigned char)failtype;
}

void MsgrWhisperNotfoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sidx, const char* sname)
{
	msg.Init(MSG_MSGR_REP);
	msg	<< seq
		<< server << subno << zone
		<< (unsigned char)MSG_MSGR_WHISPER_NOTFOUND
		<< sidx << sname;
}

void MsgrWhisperRep(CNetMsg& msg, int seq, int server, int subno, int zone, int sidx, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_MSGR_REP);
	msg	<< seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_WHISPER_REP
		<< sidx << sname
		<< rname
		<< chat;
}

void MsgrLogoutRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, const char* id)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_LOGOUT_REP
		<< success
		<< id;
}

#ifdef NEW_DOUBLE_GM
void MsgrDoubleEventRepMsg(CNetMsg& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, int cmd, int nas, int nasget, int exp, int sp, int produce, int pronum)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< serverno
		<< subno
		<< zoneno
		<< (unsigned char)MSG_MSGR_DOUBLE_EVENT_REP
		<< charindex
		<< tserver
		<< tsub
		<< cmd
		<< nas
		<< nasget
		<< exp
		<< sp
		<< produce
		<< pronum;
}
#endif

#ifdef NEW_DOUBLE_EVENT_AUTO
void MsgrDoubleEventAutoRepMsg(CNetMsg& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, char cmd, char state)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< serverno
		<< subno
		<< zoneno
		<< (unsigned char)MSG_MSGR_DOUBLE_EVENT_AUTO_REP
		<< charindex
		<< tserver
		<< tsub
		<< cmd
		<< state;
}
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef UPGRADE_EVENT
void MsgrUpgradeEventRepMsg(CNetMsg& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, char cmd, int prob)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< serverno
		<< subno
		<< zoneno
		<< (unsigned char)MSG_MSGR_UPGRADE_EVENT_REP
		<< charindex
		<< tserver
		<< tsub
		<< cmd
		<< prob;
}
#endif // UPGRADE_EVENT

#ifdef CHANCE_EVENT
void MsgrChanceEventRepMsg(CNetMsg& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, int cmd, int slevel, int elevel, int nas, int nasget, int exp, int sp, int produce, int pronum)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< serverno
		<< subno
		<< zoneno
		<< (unsigned char)MSG_MSGR_CHANCE_EVENT_REP
		<< charindex
		<< tserver
		<< tsub
		<< cmd
		<< slevel
		<< elevel
		<< nas
		<< nasget
		<< exp
		<< sp
		<< produce
		<< pronum;
}
#endif // CHANCE_EVENT

void MsgrDoubleExpEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int expPercent)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_DOUBLE_EXP_EVENT_REP
		<< charindex
		<< tserver
		<< tsub
		<< cmd
		<< expPercent;
}

void MsgrFriendChatNotfoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sidx, const char* sname)
{
	msg.Init(MSG_MSGR_REP);
	msg	<< seq
		<< server << subno << zone
		<< (unsigned char)MSG_MSGR_MESSENGERCHAT_NOTFOUND
		<< sidx << sname;
}

void MsgrFriendChatRep(CNetMsg& msg, int seq, int server, int subno, int zone, int sidx, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_MSGR_REP);
	msg	<< seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_MESSENGERCHAT_REP
		<< sidx << sname
		<< rname
		<< chat;
}

void MsgrEventRepMsg(CNetMsg& msg, unsigned char type, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int drop)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< type
		<< charindex
		<< tserver
		<< tsub
		<< cmd
		<< drop;
}

#ifdef RECOMMEND_SERVER_SYSTEM
void MsgrRecommendMsg(CNetMsg& msg)
{
	msg.Init(MSG_MSGR_REQ);
	msg << -1
		<< -1
		<< -1
		<< -1
		<< (unsigned char)MSG_MSGR_RECOMMEND
		<< 0
		<< gserver.m_nRecommendServer;
}
#endif // RECOMMEND_SERVER_SYSTEM

void MsgrEventGoldenballVoteMsg(CNetMsg& msg, int nGMCharIndex, const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMin)
{
	msg.Init(MSG_MSGR_REQ);
	msg << -1
		<< -1
		<< -1
		<< -1
		<< (unsigned char)MSG_MSGR_EVENT_GOLDENBALL
		<< MSG_MSGR_EVENT_GOLDENBALL_VOTE
		<< nGMCharIndex
		<< strTeam1
		<< strTeam2
		<< nYear
		<< nMonth
		<< nDay
		<< nHour
		<< nMin;
}

void MsgrEventGoldenballGiftMsg(CNetMsg& msg, int nGMCharIndex, const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMin, time_t timeEndVote)
{
	msg.Init(MSG_MSGR_REQ);
	msg << -1
		<< -1
		<< -1
		<< -1
		<< (unsigned char)MSG_MSGR_EVENT_GOLDENBALL
		<< MSG_MSGR_EVENT_GOLDENBALL_GIFT
		<< nGMCharIndex
		<< strTeam1
		<< nTeam1Score
		<< strTeam2
		<< nTeam2Score
		<< nYear
		<< nMonth
		<< nDay
		<< nHour
		<< nMin
		<< (int)timeEndVote;
}

#ifdef GMTOOL
void MsgrGmToolKickIDRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* id)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_GMTOOL_KICKID_REP
		<< success
		<< charindex
		<< id;
}

void MsgrGmToolKickRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* name)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_GMTOOL_KICK_REP
		<< success
		<< charindex
		<< name;
}

void MsgrGmToolChatMonitorRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, int gmcharindex, const char* id)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_REP
		<< success
		<< gmcharindex
		<< id;
}

void MsgrGMWhisperNotfoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex)
{
	msg.Init(MSG_MSGR_REP);
	msg	<< seq
		<< server << subno << zone
		<< (unsigned char)MSG_MSGR_GM_WHISPER_NOTFOUND
		<< sindex;
}

void MsgrGMSilenceRep(CNetMsg& msg, int seq, int serverno, int subno, int zoneno, int server, int sub, int success, int gmcharindex, const char* name)
{
	msg.Init(MSG_MSGR_REP);
	msg	<< seq << serverno << subno << zoneno
		<< (unsigned char)MSG_MSGR_GMTOOL_SILENCE_REP
		<< server 
		<< sub 
		<< success 
		<< gmcharindex
		<< name;
}

void MsgrGMWhisperRep(CNetMsg& msg, int seq, int server, int sub, int zone, int charindex, const char* rname, const char* chat, int serverno, int subno, int sindex)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< sub
		<< zone
		<< (unsigned char)MSG_MSGR_GM_WHISPER_REP
		<< charindex
		<< rname
		<< chat
		<< serverno
		<< subno
		<< sindex;
}


void MsgrGMToolWhisperRep(CNetMsg& msg, int seq, int serverno, int subno, int zoneno, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< serverno
		<< subno
		<< zoneno
		<< (unsigned char)MSG_MSGR_GMTOOL_WHISPER_REP
		<< sindex << sname
		<< rname
		<< chat;
}

#endif // GMTOOL

#ifdef DOUBLE_ITEM_DROP
void MsgrDoubleItemEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int itemPercent)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_DOUBLE_ITEM_EVENT_REP
		<< charindex
		<< tserver
		<< tsub
		<< cmd
		<< itemPercent;
}
#endif // DOUBLE_ITEM_DROP


#ifdef DOUBLE_PET_EXP
void MsgrDoublePetExpEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int PetExpPercent)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_DOUBLE_PET_EXP_EVENT_REP
		<< charindex
		<< tserver
		<< tsub
		<< cmd
		<< PetExpPercent;
}
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
void MsgrDoubleAttackEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int AttackPercent)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_DOUBLE_ATTACK_EVENT_REP
		<< charindex
		<< tserver
		<< tsub
		<< cmd
		<< AttackPercent;
}
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
void MsgrEventDropItemRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char type, int npcidx, int itemidx, int prob, int thisServer, int thisSub)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_EVENT_DROPITEM_REP
		<< charindex
		<< type
		<< npcidx
		<< itemidx
		<< prob
		<< thisServer
		<< thisSub;
}
#endif // EVENT_DROPITEM