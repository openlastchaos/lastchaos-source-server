#include "stdhdrs.h"
#include "Log.h"
#include "Exp.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"

void ProcessMessengerMessage_CharLord(CNetMsg& msg);
void ProcessMessengerMessage_Observer(CNetMsg& msg);
void ProcessMessengerMessage_EndGameReq();
void ProcessMessengerMessage_RebootReq(CNetMsg& msg);
void ProcessMessengerMessage_WhisperReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_NoticeChat(CNetMsg& msg, int nZone);
void ProcessMessengerMessage_LogoutReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_PlayerCountReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_DoubleEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_DoubleExpEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);

void ProcessMessengerMessage_ChanceEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);

void ProcessMessengerMessage_LattoEventReq(CNetMsg& msg);
void ProcessMessengerMessage_NewyearEventReq(CNetMsg& msg);
void ProcessMessengerMessage_ValentineEventReq(CNetMsg& msg);
void ProcessMessengerMessage_WhitedayEventReq(CNetMsg& msg);
void ProcessMessengerMessage_LetterEventReq(CNetMsg& msg);
void ProcessMessengerMessage_MessengerChatReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);

void ProcessMessengerMessage_WhisperRep(CNetMsg& msg);
void ProcessMessengerMessage_WhisperNotfound(CNetMsg& msg);
void ProcessMessengerMessage_PlayerCountRep(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_LogoutRep(CNetMsg& msg);
void ProcessMessengerMessage_DoubleExpEventRep(CNetMsg& msg);
void ProcessMessengerMessage_MessengerChatRep(CNetMsg& msg);
void ProcessMessengerMessage_MessengerchatNotfound(CNetMsg& msg);

#ifdef MESSENGER_NEW
void ProcessMessengerMessage_MessengerReq( unsigned char subtype, CNetMsg& msg);

#ifdef MSG_VER2
void ProcessMessengerMessage_MessengerChatOneReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
#endif // MSG_VER2

#endif // MESSENGER_NEW

#ifdef EVENT_OPEN_BETA_TLD
void ProcessMessengerMessage_OpenBetaEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_OpenBetaEventRep(CNetMsg& msg);
#endif // EVENT_OPEN_BETA_TLD

#ifdef EVENT_FLOWER
void ProcessMessengerMessage_FlowerEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_FlowerEventRep(CNetMsg& msg);
#endif // EVENT_FLOWER

#ifdef MARGADUM_PVP
void ProcessMessengerMessage_MargadumPvpReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_MargadumPvpRep(CNetMsg& msg);
#endif // MARGADUM_PVP

#ifdef ENABLE_OXQUIZ
void ProcessEventOXMessage(int server, int subno, CNetMsg& msg);
#endif // ENABLE_OXQUIZ

#ifdef NEW_DOUBLE_GM
#ifdef NEW_DOUBLE_GM_ZONE
void ProcessMessengerMessage_DoubleEventRep(CNetMsg& msg, int zone);
#else
void ProcessMessengerMessage_DoubleEventRep(CNetMsg& msg);
#endif // NEW_DOUBLE_GM_ZONE
#endif // NEW_DOUBLE_GM

#ifdef RECOMMEND_SERVER_SYSTEM
void ProcessMessengerMessage_Recommend(CNetMsg& msg);
void ProcessMessengerMessage_RecommendMoonstone(CNetMsg& msg);
#endif // RECOMMEND_SERVER_SYSTEM

void ProcessMessengerMessage_EventGoldenball(CNetMsg& msg);

#ifdef EVENTSETTING
void ProcessMessengerMessage_EventSetting(CNetMsg& msg);
#endif //EVENTSETTING

#ifdef CHANCE_EVENT
void ProcessMessengerMessage_ChanceEventRep(CNetMsg& msg);
#endif // CHANCE_EVENT

#ifdef GMTOOL
void ProcessMessengerMessage_GmToolKickIDReq(CNetMsg& msg, int seq, int serverno, int subno, int zone);
void ProcessMessengerMessage_GmToolKickIDRep(CNetMsg& msg);
void ProcessMessengerMessage_GmToolChatMonitorReq(CNetMsg& msg, int seq, int serverno, int subno, int zone);
void ProcessMessengerMessage_GmToolChatMonitorRep(CNetMsg& msg);
void ProcessMessengerMessage_GmToolChatMonitorNotice(CNetMsg& msg);
void ProcessMessengerMessage_GmToolChatMonitorOff(CNetMsg& msg);
void ProcessMessengerMessage_GmToolChatMonitorNoticeParty(CNetMsg& msg);
void ProcessMessengerMessage_GmToolChatMonitorPartyOff(CNetMsg& msg);
void ProcessMessengerMessage_GmToolChatMonitorNoticeGuild(CNetMsg& msg);
void ProcessMessengerMessage_GmToolChatMonitorGuildOff(CNetMsg& msg);
void ProcessMessengerMessage_GmToolWhisperRep(CNetMsg& msg);
void ProcessMessengerMessage_GmWhisperReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_GmToolWhisperReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_GmToolChatMonitorNoticeWhisper(CNetMsg& msg);
void ProcessMessengerMessage_GMLoopCountReq(CNetMsg& msg);
void ProcessMessengerMessage_GMLoopCountRep(CNetMsg& msg);
void ProcessMessengerMessage_GMReloadgmip(CNetMsg& msg);
void ProcessMessengerMessage_GmToolSilenceReq(CNetMsg& msg, int seq, int server, int subno, int zone);
void ProcessMessengerMessage_GmToolSilenceRep(CNetMsg& msg);
void ProcessMessengerMessage_GmToolWhisperNotfound(CNetMsg& msg);
void ProcessMessengerMessage_GMWhisperNotfound(CNetMsg& msg);
void ProcessMessengerMessage_GMToolUserWhisperRep(CNetMsg& msg);
void ProcessMessengerMessage_GmToolKickReq(CNetMsg& msg, int seq, int serverno, int subno, int zone);
void ProcessMessengerMessage_GmToolKickRep(CNetMsg& msg);
void ProcessMessengerMessage_GMToolShutdown(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
#endif // GMTOOL

#ifdef DOUBLE_ITEM_DROP
void ProcessMessengerMessage_DoubleItemEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_DoubleItemEventRep(CNetMsg& msg);
#endif // DOUBLE_ITEM_DROP

#ifdef DOUBLE_PET_EXP
void ProcessMessengerMessage_DoublePetExpEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_DoublePetExpEventRep(CNetMsg& msg);
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
void ProcessMessengerMessage_DoubleAttackEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_DoubleAttackEventRep(CNetMsg& msg);
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
void ProcessMessengerMessage_EventDropItemReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_EventDropItemRep(CNetMsg& msg);
#endif // EVENT_DROPITEM

void ProcessMessengerMessage_SetExpLimit( CNetMsg& msg);
void ProcessMessengerMessage_DoubleEventAutoReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_DoubleEventAutoRep( CNetMsg& msg );
void ProcessMessengerMessage_UpgradeEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_UpgradeEventRep( CNetMsg& msg );

void CServer::ProcessMessengerMessage(CNetMsg& msg)
{
	msg.MoveFirst();

	switch (msg.m_mtype)
	{
	case MSG_MSGR_REQ:
		{
			int seq, server, subno, zone;
			unsigned char subtype;
			int i;

			msg >> seq
				>> server >> subno >> zone
				>> subtype;

			i = gserver.FindZone(zone);

			GAMELOG << init("MSG_MSGR_REQ:")
					<< " seq"  << delim <<  seq
					<< " server"  << delim <<  server
					<< " subno"  << delim <<  subno
					<< " zone"  << delim <<  zone
					<< " subtype"  << delim <<  subtype
					<< end;
			
			//성주 메시지 차단됨(검토)
 			if (server != -1 && (server != gserver.m_serverno || (subno != -1 && subno != gserver.m_subno)))
 				return ;			
 			if (zone != -1 && (i == -1 || gserver.m_zones[i].m_index != zone))
 				return ;

			switch (subtype)
			{
			case MSG_MSGR_EVENT_GOLDENBALL:
				ProcessMessengerMessage_EventGoldenball(msg);
				break;

#ifdef RECOMMEND_SERVER_SYSTEM
			case MSG_MSGR_RECOMMEND:
				ProcessMessengerMessage_Recommend(msg);
				break;

			case MSG_MSGR_RECOMMEND_MOONSTONE:
				ProcessMessengerMessage_RecommendMoonstone(msg);
				break;
#endif // RECOMMEND_SERVER_SYSTEM

#ifdef ENABLE_OXQUIZ
			case MSG_MSGR_EVENT_OX:
				ProcessEventOXMessage(server, subno, msg);
				break;
#endif // ENABLE_OXQUIZ

			case MSG_MSGR_CHAT_LORD:
				ProcessMessengerMessage_CharLord(msg);
				break;

			case MSG_MSGR_EVENT_REQ:
				ProcessEventMessage(msg);
				break;

			case MSG_MSGR_OBSERVER:
				ProcessMessengerMessage_Observer(msg);
				break;

			case MSG_MSGR_ENDGAME_REQ:
				ProcessMessengerMessage_EndGameReq();
				break;

			case MSG_MSGR_REBOOT_REQ:
				ProcessMessengerMessage_RebootReq(msg);
				break;

			case MSG_MSGR_WHISPER_REQ:
				ProcessMessengerMessage_WhisperReq(msg, seq, server, subno, zone);
				break;

			case MSG_MSGR_NOTICECHAT:
				ProcessMessengerMessage_NoticeChat(msg, zone);
				break;

			case MSG_MSGR_LOGOUT_REQ:
				ProcessMessengerMessage_LogoutReq(msg, seq, server, subno, zone);
				break;

			case MSG_MSGR_PLAYER_COUNT_REQ:
				ProcessMessengerMessage_PlayerCountReq(msg, seq, server, subno, zone);
				break;

			case MSG_MSGR_DOUBLE_EVENT_REQ:
				ProcessMessengerMessage_DoubleEventReq(msg, seq, server, subno, zone);
				break;

			case MSG_MSGR_DOUBLE_EXP_EVENT_REQ:
				ProcessMessengerMessage_DoubleExpEventReq(msg, seq, server, subno, zone);
				break;
				
			case MSG_MSGR_CHANCE_EVENT_REQ:
				ProcessMessengerMessage_ChanceEventReq(msg, seq, server, subno, zone);
				break;

#ifdef EVENT_OPEN_BETA_TLD
			case MSG_MSGR_OPEN_BETA_EVENT_REQ:
				ProcessMessengerMessage_OpenBetaEventReq(msg, seq, server, subno, zone);
				break;
#endif // EVENT_OPEN_BETA_TLD

#ifdef EVENT_FLOWER
			case MSG_MSGR_FLOWER_EVENT_REQ:
				ProcessMessengerMessage_FlowerEventReq(msg, seq, server, subno, zone);
				break;
#endif // EVENT_FLOWER

#ifdef MARGADUM_PVP
			case MSG_MSGR_MARGADUM_PVP_REQ:
				ProcessMessengerMessage_MargadumPvpReq(msg, seq, server, subno, zone);
				break;
#endif // MARGADUM_PVP

			case MSG_MSGR_LATTO_EVENT_REQ:
				ProcessMessengerMessage_LattoEventReq(msg);
				break;

			case MSG_MSGR_NEWYEAR_EVENT_REQ:
				ProcessMessengerMessage_NewyearEventReq(msg);
				break;

			case MSG_MSGR_VALENTINE_EVENT_REQ:
				ProcessMessengerMessage_ValentineEventReq(msg);
				break;

			case MSG_MSGR_WHITEDAY_EVENT_REQ:
				ProcessMessengerMessage_WhitedayEventReq(msg);
				break;

			case MSG_MSGR_LETTER_EVENT_REQ:
				ProcessMessengerMessage_LetterEventReq(msg);
				break;

			case MSG_MSGR_MESSENGERCHAT_REQ:
				ProcessMessengerMessage_MessengerChatReq(msg, seq, server, subno, zone);
				break;
#ifdef MESSENGER_NEW
			case MSG_MSGR_MESSENGER_ADD_CONFIRM:
			case MSG_MSGR_MESSENGER_ADD:
			case MSG_MSGR_MESSENGER_DEL:
			case MSG_MSGR_MESSENGER_CHAT:			
				ProcessMessengerMessage_MessengerReq(subtype, msg);
				break;
#ifdef MSG_VER2
			case MSG_MSGR_MESSENGER_ONE_VS_ONE:
				ProcessMessengerMessage_MessengerChatOneReq(msg, seq, server, subno, zone);
				break;
#endif // MSG_VER2
#endif // MESSENGER_NEW
#ifdef EVENTSETTING
			case MSG_MSGR_EVENT_SETTING_REQ:
				ProcessMessengerMessage_EventSetting(msg);
				break;
#endif // EVENTSETTING

#ifdef GMTOOL
			case MSG_MSGR_GMTOOL_KICKID_REQ:
				ProcessMessengerMessage_GmToolKickIDReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_GMTOOL_KICK_REQ:
				ProcessMessengerMessage_GmToolKickReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_REQ:
				ProcessMessengerMessage_GmToolChatMonitorReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE:
				ProcessMessengerMessage_GmToolChatMonitorNotice(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_PARTY:
				ProcessMessengerMessage_GmToolChatMonitorNoticeParty(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_OFF:
				ProcessMessengerMessage_GmToolChatMonitorOff(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_PARTY_OFF:
				ProcessMessengerMessage_GmToolChatMonitorPartyOff(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_GUILD:
				ProcessMessengerMessage_GmToolChatMonitorNoticeGuild(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_GUILD_OFF:
				ProcessMessengerMessage_GmToolChatMonitorGuildOff(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_WHISPER:
				ProcessMessengerMessage_GmToolChatMonitorNoticeWhisper(msg);
				break;
			case MSG_MSGR_GMTOOL_SILENCE_REQ:
				ProcessMessengerMessage_GmToolSilenceReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_GM_LOOPCOUNT_REQ:
				ProcessMessengerMessage_GMLoopCountReq(msg);
				break;
			case MSG_MSGR_GM_LOOPCOUNT_REP:
				ProcessMessengerMessage_GMLoopCountRep(msg);
				break;
			case MSG_MSGR_GM_WHISPER_REQ:
				ProcessMessengerMessage_GmWhisperReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_GMTOOL_WHIPER_REQ:
				ProcessMessengerMessage_GmToolWhisperReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_GMTOOL_SHUTDOWN:
				ProcessMessengerMessage_GMToolShutdown(msg, seq, server, subno, zone);
				break;
#endif // GMTOOL

#ifdef DOUBLE_ITEM_DROP
			case MSG_MSGR_DOUBLE_ITEM_EVENT_REQ:
				ProcessMessengerMessage_DoubleItemEventReq(msg, seq, server, subno, zone);
				break;
				
#endif // DOUBLE_ITEM_DROP

#ifdef DOUBLE_PET_EXP
			case MSG_MSGR_DOUBLE_PET_EXP_EVENT_REQ:
				ProcessMessengerMessage_DoublePetExpEventReq(msg, seq, server, subno, zone);
				break;
#endif // DOUBLE_PET_EXP
#ifdef DOUBLE_ATTACK
			case MSG_MSGR_DOUBLE_ATTACK_EVENT_REQ:
				ProcessMessengerMessage_DoubleAttackEventReq(msg, seq, server, subno, zone);
				break;
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
			case MSG_MSGR_EVENT_DROPITEM_REQ:
				ProcessMessengerMessage_EventDropItemReq(msg, seq, server, subno, zone);
				break;
#endif // EVENT_DROPITEM
				
			case MSG_MSGR_SET_EXPLIMIT :
				ProcessMessengerMessage_SetExpLimit( msg );
				break;
#ifdef NEW_DOUBLE_EVENT_AUTO
			case MSG_MSGR_DOUBLE_EVENT_AUTO_REQ:
				ProcessMessengerMessage_DoubleEventAutoReq(msg, seq, server, subno, zone);
				break;
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef UPGRADE_EVENT
			case MSG_MSGR_UPGRADE_EVENT_REQ:
				ProcessMessengerMessage_UpgradeEventReq(msg, seq, server, subno, zone);
				break;
#endif // UPGRADE_EVENT
			}
		}
		break;

	case MSG_MSGR_REP:
		{
			int seq, server, subno, zone;
			unsigned char subtype;
			int i;

			msg >> seq
				>> server >> subno >> zone
				>> subtype;

			i = gserver.FindZone(zone);
			if (server != -1 && (server != gserver.m_serverno || (subno != -1 && subno != gserver.m_subno)))
				return ;
			if (zone != -1 && (i == -1 || gserver.m_zones[i].m_index != zone))
				return ;

			switch (subtype)
			{
			case MSG_MSGR_WHISPER_REP:
				ProcessMessengerMessage_WhisperRep(msg);
				break;


			case MSG_MSGR_WHISPER_NOTFOUND:
				ProcessMessengerMessage_WhisperNotfound(msg);
				break;

			case MSG_MSGR_PLAYER_COUNT_REP:
				ProcessMessengerMessage_PlayerCountRep(msg, seq, server, subno, zone);
				break;

			case MSG_MSGR_LOGOUT_REP:
				ProcessMessengerMessage_LogoutRep(msg);
				break;


#ifdef NEW_DOUBLE_GM
			case MSG_MSGR_DOUBLE_EVENT_REP:
#ifdef NEW_DOUBLE_GM_ZONE
				ProcessMessengerMessage_DoubleEventRep(msg, zone);
#else
				ProcessMessengerMessage_DoubleEventRep(msg);
#endif // NEW_DOUBLE_GM_ZONE
				break;
#endif

#ifdef CHANCE_EVENT
			case MSG_MSGR_CHANCE_EVENT_REP:
				ProcessMessengerMessage_ChanceEventRep(msg);
				break;
#endif // CHANCE_EVENT

			case MSG_MSGR_DOUBLE_EXP_EVENT_REP:
				ProcessMessengerMessage_DoubleExpEventRep(msg);
				break;

#ifdef EVENT_OPEN_BETA_TLD
			case MSG_MSGR_OPEN_BETA_EVENT_REP:
				ProcessMessengerMessage_OpenBetaEventRep(msg);
				break;
#endif

#ifdef	EVENT_FLOWER
			case MSG_MSGR_FLOWER_EVENT_REP:
				ProcessMessengerMessage_FlowerEventRep(msg);
				break;
#endif

#ifdef MARGADUM_PVP
			case MSG_MSGR_MARGADUM_PVP_REP:
				ProcessMessengerMessage_MargadumPvpRep(msg);
				break;
#endif

			case MSG_MSGR_MESSENGERCHAT_REP:
				ProcessMessengerMessage_MessengerChatRep(msg);
				break;
				
			case MSG_MSGR_MESSENGERCHAT_NOTFOUND:
				ProcessMessengerMessage_MessengerchatNotfound(msg);
				break;
#ifdef GMTOOL
			case MSG_MSGR_GMTOOL_KICKID_REP:
				ProcessMessengerMessage_GmToolKickIDRep(msg);
				break;
			case MSG_MSGR_GMTOOL_KICK_REP:
				ProcessMessengerMessage_GmToolKickRep(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_REP:
				ProcessMessengerMessage_GmToolChatMonitorRep(msg);
				break;
			case MSG_MSGR_GM_WHISPER_REP:
				ProcessMessengerMessage_GmToolWhisperRep(msg);
				break;
			case MSG_MSGR_GMTOOL_SILENCE_REP:
				ProcessMessengerMessage_GmToolSilenceRep(msg);
				break;
			case MSG_MSGR_GMTOOL_WHISPER_NOTFOUND:
				ProcessMessengerMessage_GmToolWhisperNotfound(msg);
				break;
			case MSG_MSGR_GM_WHISPER_NOTFOUND:
				ProcessMessengerMessage_GMWhisperNotfound(msg);
				break;
			case MSG_MSGR_GM_RELOADGMIP_REP:
				ProcessMessengerMessage_GMReloadgmip(msg);
				break;
			case MSG_MSGR_GMTOOL_WHISPER_REP:
				ProcessMessengerMessage_GMToolUserWhisperRep(msg);
				break;
#endif // GMTOOL

#ifdef DOUBLE_ITEM_DROP
			case MSG_MSGR_DOUBLE_ITEM_EVENT_REP:
				ProcessMessengerMessage_DoubleItemEventRep(msg);
				break;
#endif	// DOUBLE_ITEM_DROP

#ifdef DOUBLE_PET_EXP
			case MSG_MSGR_DOUBLE_PET_EXP_EVENT_REP:
				ProcessMessengerMessage_DoublePetExpEventRep(msg);
				break;
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
			case MSG_MSGR_DOUBLE_ATTACK_EVENT_REP:
				ProcessMessengerMessage_DoubleAttackEventRep(msg);
				break;
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
			case MSG_MSGR_EVENT_DROPITEM_REP:
				ProcessMessengerMessage_EventDropItemRep(msg);
				break;
#endif // EVENT_DROPITEM

#ifdef NEW_DOUBLE_EVENT_AUTO
			case MSG_MSGR_DOUBLE_EVENT_AUTO_REP:
				ProcessMessengerMessage_DoubleEventAutoRep(msg);
				break;
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef UPGRADE_EVENT
			case MSG_MSGR_UPGRADE_EVENT_REP:
				ProcessMessengerMessage_UpgradeEventRep(msg);
				break;
#endif
			}
		}
		break;

	default:
		break;
	}
}

#ifdef ENABLE_OXQUIZ
void ProcessEventOXMessage(int server, int subno, CNetMsg& msg)
{
	int			nSubType;

	int			nGMCharIndex;
	CPC*		pGMPC;
	int			nYear;
	int			nMonth;
	int			nDay;
	int			nHour;
	int			nMin;

	int			nQuizNo;
	int			nSec;
	void*		pos;

	COXQuizData*	qdFind;

	msg >> nSubType;

	switch (nSubType)
	{
	case MSG_MSGR_EVENT_OX_QUIZ:
		msg >> nGMCharIndex
			>> nQuizNo
			>> nSec;
		qdFind = new COXQuizData(nQuizNo, "", false);
		pos = gserver.m_listOXQuiz.FindData(qdFind);
		delete qdFind;
		if (pos)
		{
			gserver.m_nOXQuizRealNo = nQuizNo;
			time(&gserver.m_timeOXQuizLimit);
			gserver.m_timeOXQuizLimit += nSec;
			gserver.m_nOXQuizGMCharIndex = nGMCharIndex;

			CNetMsg rmsg;
			qdFind = gserver.m_listOXQuiz.GetData(pos);

			GAMELOG << init("EVENT OX QUIZ QUESTION")
					<< "BY" << delim
					<< nGMCharIndex << delim
					<< "QUIZ" << delim
					<< nQuizNo << delim
					<< qdFind->GetQuizString()
					<< end;

			pGMPC = gserver.m_playerList.Find(nGMCharIndex);
			if (pGMPC)
			{
				CLCString chat(1000);
				chat.Format("QUIZ: [%d]%s(%d sec)", nQuizNo, qdFind->GetQuizString(), nSec);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				SEND_Q(rmsg, pGMPC->m_desc);
			}

			int idxZone = gserver.FindZone(ZONE_OXQUIZROOM);
			if (idxZone != -1)
			{
				CZone* pZone = gserver.m_zones + idxZone;
				if (!pZone->m_bRemote)
				{
					EventOXQuizQuizMsg(rmsg, gserver.m_nOXQuizSeqNo, nSec, qdFind->GetQuizString());
					pZone->m_area[0].SendToAllClient(rmsg);
				}
			}
		}
		break;

	case MSG_MSGR_EVENT_OX_END:
		msg >> nGMCharIndex;
		gserver.m_bEventOX = false;
		GAMELOG << init("EVENT OX QUIZ SET END")
				<< " BY " << nGMCharIndex
				<< end;
		pGMPC = gserver.m_playerList.Find(nGMCharIndex);
		if (pGMPC)
		{
			CNetMsg rmsg;
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "OX Quiz End");
			SEND_Q(rmsg, pGMPC->m_desc);
		}
		break;

	case MSG_MSGR_EVENT_OX_SET_START:
		msg >> nGMCharIndex
			>> nYear
			>> nMonth
			>> nDay
			>> nHour
			>> nMin;

		gserver.m_bEventOX = true;
		gserver.m_bOXQuizNoticeStart = false;
		gserver.m_nOXQuizSeqNo = 1;
		gserver.m_nOXQuizRealNo = -1;

		
// 브라질은 시간이 안맞으므로 다른 방법을 사용함...   yhj
#if defined(LC_BRZ)
		struct tm	tmStart = NOW();
		tmStart.tm_year = nYear - 1900;
		tmStart.tm_mon = nMonth - 1;
		tmStart.tm_mday = nDay;
		tmStart.tm_hour = nHour;
		tmStart.tm_min = nMin;
		gserver.m_timeEventOXStart = mktime(&tmStart);
#else
		struct tm	tmStart;
		memset(&tmStart, 0, sizeof(tmStart));
		tmStart.tm_year = nYear - 1900;
		tmStart.tm_mon = nMonth - 1;
		tmStart.tm_mday = nDay;
		tmStart.tm_hour = nHour;
		tmStart.tm_min = nMin;
		gserver.m_timeEventOXStart = mktime(&tmStart);
#endif

		GAMELOG << init("EVENT OX QUIZ SET START")
				<< "START TIME" << delim
				<< nYear << delim
				<< nMonth << delim
				<< nDay << delim
				<< nHour << delim
				<< nMin << delim
				<< " BY " << nGMCharIndex
				<< end;

		pGMPC = gserver.m_playerList.Find(nGMCharIndex);
		if (pGMPC)
		{
			CNetMsg rmsg;
			CLCString chat(1000);
			chat.Format("OX Quiz set start time: %04d-%02d-%02d %02d:%02d", nYear, nMonth, nDay, nHour, nMin);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, pGMPC->m_desc);
		}
		break;
	}
}
#endif // ENABLE_OXQUIZ

void ProcessMessengerMessage_CharLord(CNetMsg& msg)
{
	int sindex;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	msg >> sindex
		>> sname
		>> chat;

	GAMELOG << init("CHATLOAD CHATMSG SEND TO CLIENT:")
			<< " sindex"  << delim <<  sindex
			<< " sname"  << delim <<  sname
			<< " chat"  << delim <<  chat
			<< end;

	CNetMsg rmsg;
	SayMsg(rmsg, MSG_CHAT_LORD, sindex, sname, "", chat);
	gserver.m_playerList.SendToAll(rmsg);
}

void ProcessMessengerMessage_Observer(CNetMsg& msg)
{
	static CLCString chat(1000);
	if (gserver.m_observer)
	{
		msg >> chat;
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		SEND_Q(rmsg, gserver.m_observer);
	}
}

void ProcessMessengerMessage_EndGameReq()
{
	CNetMsg rmsg;
	ConnRebootReqMsg(rmsg);
	if (IS_RUNNING_CONN) SEND_Q(rmsg, gserver.m_connector);
	HelperShutdownMsg(rmsg);
	if (IS_RUNNING_HELPER) SEND_Q(rmsg, gserver.m_helper);
}

void ProcessMessengerMessage_RebootReq(CNetMsg& msg)
{
	int t;
	static CLCString chat(1000);

	msg >> t
		>> chat;
	
	gserver.m_pulseReboot = t * PULSE_REAL_SEC;
	if (chat.Length() > 0)
	{
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		gserver.m_playerList.SendToAll(rmsg);
	}
}

void ProcessMessengerMessage_WhisperReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int sidx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	msg >> sidx >> sname
		>> rname
		>> chat;

	CPC* receiver = gserver.m_playerList.Find(rname, true );
	if (receiver == NULL)
	{
		// 수신자가 없을 때
		CNetMsg mmsg;
		MsgrRepWhisperNotFoundMsg(mmsg, nSeq, nServer, nSubNo, nZone, sidx, sname);
		if (IS_RUNNING_MSGR) SEND_Q(mmsg, gserver.m_messenger);
	}
	else
	{
		// 수신자가 있을 때
		CNetMsg cmsg;

		ChatWhisperMsg(cmsg, sidx, sname, receiver->GetName(), chat);
#ifdef MESSENGER_NEW
		if( !receiver->IsBlockPC(sidx, sname) )
#endif
			SEND_Q(cmsg, receiver->m_desc);

		// 메신저에 응답 보내기
		CNetMsg mmsg;
		MsgrRepWhisperRepMsg(mmsg, nSeq, nServer, nSubNo, nZone, sidx, sname, receiver->GetName(), chat);
		if (IS_RUNNING_MSGR) SEND_Q(mmsg, gserver.m_messenger);

#ifdef GMTOOL
		if(receiver->m_bGmMonitor)
		{
			// 메신저를 통해서 받은 내용을 툴로 보낸다.
			MsgrNoticeGmChatMonitorWhisperMsg(mmsg, -1, 1, 1, 0, gserver.m_serverno, gserver.m_subno, -1, chat, receiver->m_index, receiver->m_name, (unsigned char)MSG_CHAT_WHISPER, sname);
			if (IS_RUNNING_MSGR) SEND_Q(mmsg, gserver.m_messenger);
		}
#endif // GMTOOL
	}
}

void ProcessMessengerMessage_NoticeChat(CNetMsg& msg, int nZone)
{
	static CLCString chat(1000);

	msg >> chat;

	CNetMsg cmsg;
	SayMsg(cmsg, MSG_CHAT_NOTICE, 0, "", "", chat);

	int i;
	for (i = 0; i < gserver.m_playerList.m_max; i++)
	{
		if (gserver.m_playerList.m_pcList[i]
				&& gserver.m_playerList.m_pcList[i]->m_pZone
				&& (gserver.m_playerList.m_pcList[i]->m_pZone->m_index == nZone || nZone == -1))
		{
			SEND_Q(cmsg, gserver.m_playerList.m_pcList[i]->m_desc);
		}
	}
}

void ProcessMessengerMessage_LogoutReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	CNetMsg rmsg;

	static CLCString id(MAX_ID_NAME_LENGTH + 1);

	msg >> id;

	char success = 1;
	CDescriptor* desc = gserver.FindConnectIdName(id, NULL);
	if (desc)
	{
		gserver.CloseSocket(desc, false);
	}
	else if (IS_RUNNING_MSGR)
	{
		success = 0;
		if (nSeq != -1)
		{
			MsgrLogoutRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, (char)success, id);
			SEND_Q(rmsg, gserver.m_messenger);
		}
	}
}

void ProcessMessengerMessage_PlayerCountReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int reqChar;

	msg >> reqChar;

	int i;
	int countZone = 0;
	int* countPC = new int[gserver.m_numZone];
	int* countProduce = new int[gserver.m_numZone];
	int* countShop = new int[gserver.m_numZone];
	int* countPlay = new int[gserver.m_numZone];

	memset(countPC, 0, sizeof(int) * gserver.m_numZone);
	memset(countProduce, 0, sizeof(int) * gserver.m_numZone);
	memset(countShop, 0, sizeof(int) * gserver.m_numZone);
	memset(countPlay, 0, sizeof(int) * gserver.m_numZone);

	// 플레이어수를 존별로 구함
	for (i = 0; i < gserver.m_playerList.m_max; i++)
	{
		CPC* pc = gserver.m_playerList.m_pcList[i];
		if (pc)
		{
			countPC[pc->m_pZone - gserver.m_zones]++;

			// 생산중인가 : pc의 AttackList 중 생산 npc가 있는가
			bool bProduce = false;
			CAttackChar* attackCh = pc->m_attackList;
			while (attackCh)
			{
				if (IS_NPC(attackCh->ch))
				{
					if (attackCh->npc->m_proto->CheckFlag(NPC_MINERAL | NPC_CROPS | NPC_ENERGY))
					{
						bProduce = true;
						break;
					}
				}
				attackCh = attackCh->m_next;
			}

			// 생산중인가
			if (bProduce)
				countProduce[pc->m_pZone - gserver.m_zones]++;
			// 개인상점을 열었는가
			else if (pc->m_personalshop)
				countShop[pc->m_pZone - gserver.m_zones]++;
			// 일반 플레이 유저
			else
				countPlay[pc->m_pZone - gserver.m_zones]++;
		}
	}

	// 리모트를 제외한 존의 개수를 구함
	for (i = 0; i < gserver.m_numZone; i++)
	{
		if (countPC[i] != 0)
			countZone++;
	}

	CNetMsg mmsg;
	MsgrPlayerCountRep(mmsg, nSeq, nServer, nSubNo, nZone, reqChar, countZone, countPC, countProduce, countShop, countPlay);
	if (IS_RUNNING_MSGR) SEND_Q(mmsg, gserver.m_messenger);

	delete [] countPC;
	countPC = NULL;
	delete [] countProduce;
	countProduce = NULL;
	delete [] countShop;
	countShop = NULL;
	delete [] countPlay;
	countPlay = NULL;
}

void ProcessMessengerMessage_DoubleEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
#ifdef NEW_DOUBLE_GM
	int charindex, cmd;
	CNetMsg rmsg;

	msg >> charindex
		>> cmd;

	switch (cmd)
	{
	case 0:
		{
#ifdef NEW_DOUBLE_GM_ZONE
			gserver.m_bDoubleEventZone = nZone;
#endif // NEW_DOUBLE_GM_ZONE
			
			gserver.m_bDoubleEvent = true;

			msg >> gserver.m_bDoubleNasPercent
				>> gserver.m_bDoubleNasGetPercent
				>> gserver.m_bDoubleExpPercent
				>> gserver.m_bDoubleSpPercent
				>> gserver.m_bDoubleProducePercent
				>> gserver.m_bDoubleProduceNum;

#ifdef	TLD_DOUBLE_EVENT
			
			if ( gserver.m_bDoubleNasPercent < 0
				|| gserver.m_bDoubleNasGetPercent  < 0
				|| gserver.m_bDoubleExpPercent  < 0
				|| gserver.m_bDoubleSpPercent  < 0
				|| gserver.m_bDoubleProducePercent  < 0
				|| gserver.m_bDoubleProduceNum  < 0 
				|| gserver.m_bDoubleNasPercent > TLD_DOUBLE_EVENT
				|| gserver.m_bDoubleNasGetPercent > TLD_DOUBLE_EVENT 
				|| gserver.m_bDoubleExpPercent  > TLD_DOUBLE_EVENT
				|| gserver.m_bDoubleSpPercent  > TLD_DOUBLE_EVENT
				|| gserver.m_bDoubleProducePercent  > TLD_DOUBLE_EVENT
				|| gserver.m_bDoubleProduceNum  > 10 )
			{
				gserver.m_bDoubleNasPercent = DEFAULT_NAS_PERCENT;
				gserver.m_bDoubleNasGetPercent = DEFAULT_NAS_GET_PERCENT;
				gserver.m_bDoubleExpPercent = DEFAULT_EXP_PERCENT;
				gserver.m_bDoubleSpPercent = DEFAULT_SP_PERCENT;
				gserver.m_bDoubleProducePercent = DEFAULT_PRO_PERCENT;
				gserver.m_bDoubleProduceNum = DEFAULT_PRO_GET_NUMBER;
			}
#else

			if ( gserver.m_bDoubleNasPercent < 0
				|| gserver.m_bDoubleNasGetPercent  < 0
				|| gserver.m_bDoubleExpPercent  < 0
				|| gserver.m_bDoubleSpPercent  < 0
				|| gserver.m_bDoubleProducePercent  < 0
				|| gserver.m_bDoubleProduceNum  < 0 
				|| gserver.m_bDoubleNasPercent > 200
				|| gserver.m_bDoubleNasGetPercent > 200 
				|| gserver.m_bDoubleExpPercent  > 200
				|| gserver.m_bDoubleSpPercent  > 200
				|| gserver.m_bDoubleProducePercent  > 200
				|| gserver.m_bDoubleProduceNum  > 10 )
			{
				gserver.m_bDoubleNasPercent = DEFAULT_NAS_PERCENT;
				gserver.m_bDoubleNasGetPercent = DEFAULT_NAS_GET_PERCENT;
				gserver.m_bDoubleExpPercent = DEFAULT_EXP_PERCENT;
				gserver.m_bDoubleSpPercent = DEFAULT_SP_PERCENT;
				gserver.m_bDoubleProducePercent = DEFAULT_PRO_PERCENT;
				gserver.m_bDoubleProduceNum = DEFAULT_PRO_GET_NUMBER;
			}
#endif	// TLD_DOUBLE_EVENT

			MsgrDoubleEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0);

#ifdef NEW_DOUBLE_GM_SAVEFILE
			FILE* fpDoubleEvent = fopen(NEW_DOUBLE_GM_SAVEFILE, "wt");
			if (fpDoubleEvent)
			{
				fprintf(fpDoubleEvent, "%d\n", gserver.m_bDoubleNasPercent);
				fprintf(fpDoubleEvent, "%d\n", gserver.m_bDoubleNasGetPercent);
				fprintf(fpDoubleEvent, "%d\n", gserver.m_bDoubleExpPercent);
				fprintf(fpDoubleEvent, "%d\n", gserver.m_bDoubleSpPercent);
				fprintf(fpDoubleEvent, "%d\n", gserver.m_bDoubleProducePercent);
				fprintf(fpDoubleEvent, "%d\n", gserver.m_bDoubleProduceNum);
				fclose(fpDoubleEvent);
				fpDoubleEvent = NULL;
			}
#endif // NEW_DOUBLE_GM_SAVEFILE
		}
		break;
	case 1:
		{
#ifdef NEW_DOUBLE_GM_ZONE
			if (gserver.m_bDoubleEvent)
				MsgrDoubleEventRepMsg(rmsg, nSeq, nServer, nSubNo, gserver.m_bDoubleEventZone, charindex, 1);
			else
				MsgrDoubleEventRepMsg(rmsg, nSeq, nServer, nSubNo, gserver.m_bDoubleEventZone, charindex, 2);
#else
			if (gserver.m_bDoubleEvent)
				MsgrDoubleEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1);
			else
				MsgrDoubleEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2);
#endif // NEW_DOUBLE_GM_ZONE
		}
		break;
	case 2:
		{
			gserver.m_bDoubleEvent = false;

#ifdef NEW_DOUBLE_GM_ZONE

			gserver.m_bDoubleEventZone = -1;
#endif // NEW_DOUBLE_GM_ZONE

			gserver.m_bDoubleNasPercent = DEFAULT_NAS_PERCENT;
			gserver.m_bDoubleNasGetPercent = DEFAULT_NAS_GET_PERCENT;
			gserver.m_bDoubleExpPercent = DEFAULT_EXP_PERCENT;
			gserver.m_bDoubleSpPercent = DEFAULT_SP_PERCENT;
			gserver.m_bDoubleProducePercent = DEFAULT_PRO_PERCENT;
			gserver.m_bDoubleProduceNum = DEFAULT_PRO_GET_NUMBER;
			MsgrDoubleEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2);

#ifdef NEW_DOUBLE_GM_SAVEFILE
			remove(NEW_DOUBLE_GM_SAVEFILE);
#endif // NEW_DOUBLE_GM_SAVEFILE
		}
		break;
	default:
		return;
	}

	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);

#else
	char bEventStart;

	msg >> bEventStart;

	if (bEventStart)
		gserver.m_bDoubleEvent = true;
	else
		gserver.m_bDoubleEvent = false;

#endif
}

void ProcessMessengerMessage_ChanceEventReq(CNetMsg & msg, int nSeq, int nServer, int nSubNo, int nZone)
{
#ifdef CHANCE_EVENT
	int charindex, cmd;
	CNetMsg rmsg;

	msg >> charindex
		>> cmd;

	switch (cmd)
	{
	case 0:		// start
		{
			gserver.m_bChanceEvent = true;

			msg >> gserver.m_bChanceSlevel
				>> gserver.m_bChanceElevel
				>> gserver.m_bChanceNasPercent
				>> gserver.m_bChanceNasGetPercent
				>> gserver.m_bChanceExpPercent
				>> gserver.m_bChanceSpPercent
				>> gserver.m_bChanceProducePercent
				>> gserver.m_bChanceProduceNum;

			if (gserver.m_bChanceSlevel < 1
				|| gserver.m_bChanceSlevel > MAX_LEVEL
				|| gserver.m_bChanceElevel < 1
				|| gserver.m_bChanceElevel > MAX_LEVEL
				|| (gserver.m_bChanceSlevel > gserver.m_bChanceElevel))
			{
				gserver.m_bChanceSlevel = 1;
				gserver.m_bChanceElevel = MAX_LEVEL;
			}

			if (gserver.m_bChanceNasPercent < 0
				|| gserver.m_bChanceNasGetPercent  < 0
				|| gserver.m_bChanceExpPercent  < 0
				|| gserver.m_bChanceSpPercent  < 0
				|| gserver.m_bChanceProducePercent  < 0
				|| gserver.m_bChanceProduceNum  < 0 
				|| gserver.m_bChanceNasPercent > 200
				|| gserver.m_bChanceNasGetPercent > 200 
				|| gserver.m_bChanceExpPercent  > 200
				|| gserver.m_bChanceSpPercent  > 200
				|| gserver.m_bChanceProducePercent  > 200
				|| gserver.m_bChanceProduceNum  > 10)
			{
				gserver.m_bChanceNasPercent = DEFAULT_NAS_PERCENT;
				gserver.m_bChanceNasGetPercent = DEFAULT_NAS_GET_PERCENT;
				gserver.m_bChanceExpPercent = DEFAULT_EXP_PERCENT;
				gserver.m_bChanceSpPercent = DEFAULT_SP_PERCENT;
				gserver.m_bChanceProducePercent = DEFAULT_PRO_PERCENT;
				gserver.m_bChanceProduceNum = DEFAULT_PRO_GET_NUMBER;
			}

			MsgrChanceEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0);

			FILE * fpChanceEvent = fopen(".chanceevent", "wt");
			if (fpChanceEvent)
			{
				fprintf(fpChanceEvent, "%d\n", gserver.m_bChanceSlevel);
				fprintf(fpChanceEvent, "%d\n", gserver.m_bChanceElevel);
				fprintf(fpChanceEvent, "%d\n", gserver.m_bChanceNasPercent);
				fprintf(fpChanceEvent, "%d\n", gserver.m_bChanceNasGetPercent);
				fprintf(fpChanceEvent, "%d\n", gserver.m_bChanceExpPercent);
				fprintf(fpChanceEvent, "%d\n", gserver.m_bChanceSpPercent);
				fprintf(fpChanceEvent, "%d\n", gserver.m_bChanceProducePercent);
				fprintf(fpChanceEvent, "%d\n", gserver.m_bChanceProduceNum);
				fclose(fpChanceEvent);
				fpChanceEvent = NULL;
			}
		}
		break;
	case 1:	// ing
		{
			if (gserver.m_bChanceEvent)
			{
				MsgrChanceEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1);
			}
			else
			{
				MsgrChanceEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2);
			}
		}
		break;
	case 2:	// stop
		{
			gserver.m_bChanceEvent = false;

			gserver.m_bChanceNasPercent = DEFAULT_NAS_PERCENT;
			gserver.m_bChanceNasGetPercent = DEFAULT_NAS_GET_PERCENT;
			gserver.m_bChanceExpPercent = DEFAULT_EXP_PERCENT;
			gserver.m_bChanceSpPercent = DEFAULT_SP_PERCENT;
			gserver.m_bChanceProducePercent = DEFAULT_PRO_PERCENT;
			gserver.m_bChanceProduceNum = DEFAULT_PRO_GET_NUMBER;
			MsgrChanceEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2);

			remove(".chanceevent");
		}
		break;
	default:
		return;
	}

	if (IS_RUNNING_MSGR) 
	{
		SEND_Q(rmsg, gserver.m_messenger);
	}
#endif
}

void ProcessMessengerMessage_DoubleExpEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, cmd;
	CNetMsg rmsg;

	msg >> charindex
		>> cmd;

	switch (cmd)
	{
	case 0:			// start
		if (gserver.m_bDoubleExpEvent)
			MsgrDoubleExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_expPercent);
		else
		{
			gserver.m_bDoubleExpEvent = true;
			msg >> gserver.m_expPercent;
			if( gserver.m_expPercent < 0 || gserver.m_expPercent > 200 )
			{
				gserver.m_expPercent = 100;
			}

			MsgrDoubleExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, gserver.m_expPercent);
		}
		break;

	case 1:			// ing
		if (gserver.m_bDoubleExpEvent)
			MsgrDoubleExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_expPercent);
		else
			MsgrDoubleExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_expPercent);
		break;

	case 2:			// stop
		gserver.m_bDoubleExpEvent = false;
		MsgrDoubleExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_expPercent);
		break;

	default:
		return ;
	}

	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
}

#ifdef EVENT_OPEN_BETA_TLD
void ProcessMessengerMessage_OpenBetaEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, cmd;
	CNetMsg rmsg;

	msg >> charindex
		>> cmd;

	switch (cmd)
	{
	case 0:			// start
		if (gserver.m_bOpenEvent)
			MsgrEventRepMsg(rmsg, MSG_MSGR_OPEN_BETA_EVENT_REP, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_bSoilDrop);
		else
		{
			gserver.m_bOpenEvent = true;
			msg >> gserver.m_bSoilDrop;

			MsgrEventRepMsg(rmsg, MSG_MSGR_OPEN_BETA_EVENT_REP, nSeq, nServer, nSubNo, nZone, charindex, 0, gserver.m_bSoilDrop);
		}
		break;

	case 1:			// ing
		if (gserver.m_bOpenEvent)
			MsgrEventRepMsg(rmsg, MSG_MSGR_OPEN_BETA_EVENT_REP, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_bSoilDrop);
		else
			MsgrEventRepMsg(rmsg, MSG_MSGR_OPEN_BETA_EVENT_REP, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_bSoilDrop);
		break;

	case 2:			// stop
		gserver.m_bOpenEvent = false;
		MsgrEventRepMsg(rmsg, MSG_MSGR_OPEN_BETA_EVENT_REP, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_bSoilDrop);
		break;

	default:
		return ;
	}

	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
}

void ProcessMessengerMessage_OpenBetaEventRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int charindex, tserver, tsub, cmd;
	msg >> charindex
		>> tserver
		>> tsub
		>> cmd;
	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		sprintf(g_buf, "Open Beta Event: Server %2d-%2d: ", tserver, tsub);
		switch (cmd)
		{
		case 0:
			strcat(g_buf, "start");
			IntCat(g_buf, gserver.m_bSoilDrop, true);
			break;
		case 1:
			strcat(g_buf, "ing");
			IntCat(g_buf, gserver.m_bSoilDrop, true);
			break;
		case 2:
			strcat(g_buf, "stop");
			IntCat(g_buf, gserver.m_bSoilDrop, true);
			break;
		}

		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, pc->m_desc);
	}
}
#endif // EVENT_OPEN_BETA_TLD

#ifdef EVENT_FLOWER
void ProcessMessengerMessage_FlowerEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, cmd;
	CNetMsg rmsg;

	msg >> charindex
		>> cmd;

	switch (cmd)
	{
	case 0:			// start
		if (gserver.m_bFlowerEvent)
			MsgrEventRepMsg(rmsg, MSG_MSGR_FLOWER_EVENT_REP, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_bFlower);
		else
		{
			gserver.m_bFlowerEvent = true;
			msg >> gserver.m_bFlower;

			MsgrEventRepMsg(rmsg, MSG_MSGR_FLOWER_EVENT_REP, nSeq, nServer, nSubNo, nZone, charindex, 0, gserver.m_bFlower);
		}
		break;

	case 1:			// ing
		if (gserver.m_bFlowerEvent)
			MsgrEventRepMsg(rmsg, MSG_MSGR_FLOWER_EVENT_REP, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_bFlower);
		else
			MsgrEventRepMsg(rmsg, MSG_MSGR_FLOWER_EVENT_REP, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_bFlower);
		break;

	case 2:			// stop
		gserver.m_bFlowerEvent = false;
		MsgrEventRepMsg(rmsg, MSG_MSGR_FLOWER_EVENT_REP, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_bFlower);
		break;

	default:
		return ;
	}
	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
}

void ProcessMessengerMessage_FlowerEventRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int charindex, tserver, tsub, cmd;
	msg >> charindex
		>> tserver
		>> tsub
		>> cmd;
	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		sprintf(g_buf, "Flower Event: Server %2d-%2d: ", tserver, tsub);
		switch (cmd)
		{
		case 0:
			strcat(g_buf, "start");
			IntCat(g_buf, gserver.m_bFlower, true);
			break;
		case 1:
			strcat(g_buf, "ing");
			IntCat(g_buf, gserver.m_bFlower, true);
			break;
		case 2:
			strcat(g_buf, "stop");
			IntCat(g_buf, gserver.m_bFlower, true);
			break;
		}

		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, pc->m_desc);
	}
}
#endif // EVENT_FLOWER

#ifdef MARGADUM_PVP
void ProcessMessengerMessage_MargadumPvpReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, cmd;
	CNetMsg rmsg;

	msg >> charindex
		>> cmd;

	switch (cmd)
	{
	case 0:			// start
		if (gserver.m_bMargadumPvp)
			MsgrEventRepMsg(rmsg, MSG_MSGR_MARGADUM_PVP_REP, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_bMargadumPvpLevel);
		else
		{
			gserver.m_bMargadumPvp = true;
			msg >> gserver.m_bMargadumPvpLevel;

			MsgrEventRepMsg(rmsg, MSG_MSGR_MARGADUM_PVP_REP, nSeq, nServer, nSubNo, nZone, charindex, 0, gserver.m_bMargadumPvpLevel);
		}
		break;

	case 1:			// ing
		if (gserver.m_bMargadumPvp)
			MsgrEventRepMsg(rmsg, MSG_MSGR_MARGADUM_PVP_REP, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_bMargadumPvpLevel);
		else
			MsgrEventRepMsg(rmsg, MSG_MSGR_MARGADUM_PVP_REP, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_bMargadumPvpLevel);
		break;

	case 2:			// stop
		{
			gserver.m_bMargadumPvp = false;
			MsgrEventRepMsg(rmsg, MSG_MSGR_MARGADUM_PVP_REP, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_bMargadumPvpLevel);

			// 순위를 매겨주세여..

			int rank[5];
			LONGLONG rankDamage[5];
			memset(rank, -1, sizeof(rank) );
			memset(rankDamage, 0, sizeof(rankDamage) );

			int minIndex = 0;
			int i, j;

			// 해당 존에 있는 플레이어만 가져오기
			// 마을로 보낸당
			//2층으로 이동할때 of->m_magadumdamage 는 참
			// 동일 싱글 던전으로는 이동 불가
			i = gserver.FindZone(0);
			if (i == -1)
				return ;

			CZone* pZone = gserver.m_zones + i;
			
			for(i = 0; i < gserver.m_playerList.m_cur; i++)
			{
				if( gserver.m_playerList.m_pcList[i] && gserver.m_playerList.m_pcList[i]->m_magadumDamage )
				{
					if ( gserver.m_playerList.m_pcList[i] && gserver.m_playerList.m_pcList[i]->m_magadumDamage > rankDamage[minIndex]) 
					{
						rank[minIndex] = gserver.m_playerList.m_pcList[i]->m_index;
						rankDamage[minIndex] = gserver.m_playerList.m_pcList[i]->m_magadumDamage;

						for(j = 0; j < 5; j++)
						{
							
							if( rankDamage[j] < rankDamage[minIndex] )
							{
								minIndex = j;
							}
							else if ( rank[j] == -1 )
							{
								minIndex = j;
								break;
							}
						}
					}

					GoZone(gserver.m_playerList.m_pcList[i], 0,
							pZone->m_zonePos[0][0],														// ylayer
							GetRandom(pZone->m_zonePos[0][1], pZone->m_zonePos[0][3]) / 2.0f,		// x
							GetRandom(pZone->m_zonePos[0][2], pZone->m_zonePos[0][4]) / 2.0f);		// z
					//do_GoZone(gserver.m_playerList.m_pcList[i], rankMsg);
				}
			}

			// rank를 소트하고 헬퍼에 쓰기
	
			int temp;
			LONGLONG temp1;
			CNetMsg rankMsg;

			for(i = 0; i < 5; i ++)
			{
				for( j = 0; j < 5; j++)
				{
					if( rankDamage[i] > rankDamage[j] )
					{
						temp = rank[j];
						temp1 = rankDamage[j];

						rank[j] = rank[i];
						rankDamage[j] = rankDamage[i];

						rank[i] = temp;
						rankDamage[i] = temp1;
					}
				}
			}

			HelperMargadumPvpRankReqMsg(rankMsg, gserver.m_subno, rank, rankDamage);
			if( IS_RUNNING_HELPER )
			{
				SEND_Q(rankMsg, gserver.m_helper);
			}
			else
			{
				GAMELOG << init("MARGADUM_PVP_RANK_ERROR : Helper")
						<< end;
			}
			
			GAMELOG << init("MARGADUM_PVP_RANK");
			for(i = 0; i < 5; i++)
			{
				GAMELOG << rank[i] << delim
						<< rankDamage[i] << delim;
			}
			GAMELOG << end;
			
		}
		break;

	default:
		return ;
	}
	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
}

void ProcessMessengerMessage_MargadumPvpRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int charindex, tserver, tsub, cmd;
	msg >> charindex
		>> tserver
		>> tsub
		>> cmd;
	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		sprintf(g_buf, "Magrgadum Pvp : Server %2d-%2d: ", tserver, tsub);
		switch (cmd)
		{
		case 0:
			strcat(g_buf, "start");
			IntCat(g_buf, gserver.m_bMargadumPvpLevel, true);
			break;
		case 1:
			strcat(g_buf, "ing");
			IntCat(g_buf, gserver.m_bMargadumPvpLevel, true);
			break;
		case 2:
			strcat(g_buf, "stop");
			IntCat(g_buf, gserver.m_bMargadumPvpLevel, true);
			break;
		}

		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, pc->m_desc);
	}
}
#endif // MARGADUM_PVP

void ProcessMessengerMessage_LattoEventReq(CNetMsg& msg)
{
	char bEventStart;
	int server, subno, charIndex;

	msg >> bEventStart
		>> server
		>> subno
		>> charIndex;

	if (bEventStart)
		gserver.m_bLattoEvent= true;
	else
		gserver.m_bLattoEvent = false;

	if (gserver.m_serverno == server && gserver.m_subno == subno)
	{
		CPC* pPC = gserver.m_playerList.Find(charIndex);
		if (pPC)
		{
			CNetMsg rmsg;
			if (gserver.m_bLattoEvent)
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Latto Event Start!!");
			else
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Latto Event Stop!!");
			SEND_Q(rmsg, pPC->m_desc);
		}
	}
}

void ProcessMessengerMessage_NewyearEventReq(CNetMsg& msg)
{
	char bEventStart;
	int server, subno, charIndex;

	msg >> bEventStart
		>> server
		>> subno
		>> charIndex;

	if (bEventStart)
		gserver.m_bNewYearEvent= true;
	else
		gserver.m_bNewYearEvent = false;

	if (gserver.m_serverno == server && gserver.m_subno == subno)
	{
		CPC* pPC = gserver.m_playerList.Find(charIndex);
		if (pPC)
		{
			CNetMsg rmsg;
			if (gserver.m_bNewYearEvent)
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "NewYear Event Start!!");
			else
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "NewYear Event Stop!!");
			SEND_Q(rmsg, pPC->m_desc);
		}
	}
}

void ProcessMessengerMessage_ValentineEventReq(CNetMsg& msg)
{
	char bEventStart;
	int server, subno, charIndex;

	msg >> bEventStart
		>> server
		>> subno
		>> charIndex;

	if (bEventStart)
		gserver.m_bValentineEvent= true;
	else
		gserver.m_bValentineEvent = false;

	if (gserver.m_serverno == server && gserver.m_subno == subno)
	{
		CPC* pPC = gserver.m_playerList.Find(charIndex);
		if (pPC)
		{
			CNetMsg rmsg;
			if (gserver.m_bValentineEvent)
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Valentine Event Start!!");
			else
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Valentine Event Stop!!");
			SEND_Q(rmsg, pPC->m_desc);
		}
	}
}

void ProcessMessengerMessage_WhitedayEventReq(CNetMsg& msg)
{
	char bEventStart;
	int server, subno, charIndex;

	msg >> bEventStart
		>> server
		>> subno
		>> charIndex;

	if (bEventStart)
		gserver.m_bWhiteDayEvent= true;
	else
		gserver.m_bWhiteDayEvent = false;

	if (gserver.m_serverno == server && gserver.m_subno == subno)
	{
		CPC* pPC = gserver.m_playerList.Find(charIndex);
		if (pPC)
		{
			CNetMsg rmsg;
			if (gserver.m_bWhiteDayEvent)
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "WhiteDay Event Start!!");
			else
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "WhiteDay Event Stop!!");
			SEND_Q(rmsg, pPC->m_desc);
		}
	}
}

void ProcessMessengerMessage_LetterEventReq(CNetMsg& msg)
{
#ifdef EVENT_LETTER
	char bEventStart;
	int server, subno, charIndex;

	msg >> bEventStart
		>> server
		>> subno
		>> charIndex;

	if (bEventStart)
		gserver.m_bLetterEvent= true;
	else
		gserver.m_bLetterEvent = false;

	if (gserver.m_serverno == server && gserver.m_subno == subno)
	{
		CPC* pPC = gserver.m_playerList.Find(charIndex);
		if (pPC)
		{
			CNetMsg rmsg;
			if (m_bLetterEvent)
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Letter Event Start!!");
			else
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Letter Event Stop!!");
			SEND_Q(rmsg, pPC->m_desc);
		}
	}
#endif // EVENT_LETTER
}

#ifdef MESSENGER_NEW
void ProcessMessengerMessage_MessengerReq(unsigned char subtype, CNetMsg& msg)
{
	int makeCharIndex, chatIndex, chatColor, result, charIndex;
	CLCString chat(1000);
	CLCString charName(MAX_CHAR_NAME_LENGTH + 1 );

	msg >> makeCharIndex
		>> chatIndex
		>> chatColor;

	if( subtype == MSG_MSGR_MESSENGER_ADD_CONFIRM )
		msg >> charIndex;
	else
		msg >> charName;			

	msg	>> chat;

	switch(subtype)
	{
	case MSG_MSGR_MESSENGER_ADD_CONFIRM:
		{
			CPC* pc = gserver.m_playerList.Find(charIndex);
			if( pc )
			{
				CNetMsg rmsg;
				MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_ADD, makeCharIndex, chatIndex, chatColor, pc->GetName() );
				SEND_Q(rmsg, gserver.m_messenger);
			}
		}
		break;
	case MSG_MSGR_MESSENGER_ADD:
		result = gserver.addChatGroup(makeCharIndex, chatIndex, charName);
		if( result == 1 )
			gserver.SendChatGroup( (MSG_EX_MESSENGER_TYPE) MSG_EX_MESSENGER_INVITE , makeCharIndex, chatIndex, chatColor, charName, "" );
		break;
	case MSG_MSGR_MESSENGER_DEL:	
		gserver.SendChatGroup( (MSG_EX_MESSENGER_TYPE) MSG_EX_MESSENGER_OUT , makeCharIndex, chatIndex, chatColor, charName, "" );
		gserver.delChatGroup(makeCharIndex, chatIndex, charName);
		break;
	case MSG_MSGR_MESSENGER_CHAT:
		gserver.SendChatGroup( (MSG_EX_MESSENGER_TYPE) MSG_EX_MESSENGER_CHAT , makeCharIndex, chatIndex, chatColor, charName, chat);
		break;
	default:
		return;
	}
}

#ifdef MSG_VER2
void ProcessMessengerMessage_MessengerChatOneReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int sCharIndex, tCharIndex, ChatColor;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	msg >> sCharIndex
		>> tCharIndex
		>> ChatColor
		>> sname
		>> chat;

	CPC * receiver = gserver.m_playerList.Find( tCharIndex );
	if (receiver != NULL)
	{	// 수신자가 있을 때
		CNetMsg cmsg;

		ChatOneMessengerMsg( cmsg, sCharIndex, sname, tCharIndex, receiver->GetName(), ChatColor, chat );
		SEND_Q( cmsg, receiver->m_desc );
	}
}

#endif // MSG_VER2

#endif	// MESSENGER_NEW

void ProcessMessengerMessage_MessengerChatReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
#ifdef ENABLE_MESSENGER
	int sidx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	msg >> sidx >> sname
		>> rname
		>> chat;
	
	CPC* receiver = gserver.m_playerList.Find(rname, true );
	if (receiver == NULL)
	{
		// 수신자가 없을 때
		CNetMsg mmsg;
		MsgrRepMessengerNotFoundMsg(mmsg, nSeq, nServer, nSubNo, nZone, sidx, sname);
		if (IS_RUNNING_MSGR) SEND_Q(mmsg, gserver.m_messenger);
	}
	else
	{
		// 수신자가 있을 때
		CNetMsg cmsg;
		
		ChatMessengerMsg(cmsg, sidx, sname, receiver->GetName(), chat);
		SEND_Q(cmsg, receiver->m_desc);
		
		// 메신저에 응답 보내기
		CNetMsg mmsg;
		MsgrRepMessengerChatRepMsg(mmsg, nSeq, nServer, nSubNo, nZone, sidx, sname, receiver->GetName() , chat);
		if (IS_RUNNING_MSGR) SEND_Q(mmsg, gserver.m_messenger);
	}
#endif // ENABLE_MESSENGER
}

void ProcessMessengerMessage_WhisperRep(CNetMsg& msg)
{
	int sidx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	msg >> sidx >> sname
		>> rname
		>> chat;

	CPC* sender = gserver.m_playerList.Find(sidx);
	if (sender != NULL)
	{
		// 송신자가 있으면
		CNetMsg cmsg;
		ChatWhisperMsg(cmsg, sidx, sname, rname, chat);
		SEND_Q(cmsg, sender->m_desc);
	}
}

void ProcessMessengerMessage_WhisperNotfound(CNetMsg& msg)
{
	int sidx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);

	msg >> sidx >> sname;

	CPC* sender = gserver.m_playerList.Find(sidx);
	if (sender != NULL)
	{
		// 송신자가 있으면 오류 알림
		CNetMsg smsg;
		SysWhisperNotFoundMsg(smsg);
		SEND_Q(smsg, sender->m_desc);
	}
}

void ProcessMessengerMessage_PlayerCountRep(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int reqChar;
	int countZone;

	msg >> reqChar
		>> countZone;

	CPC* pc = gserver.m_playerList.Find(reqChar);
	if (pc)
	{
		int zone, count;
		int nProduce, nShop, nPlay;

		CNetMsg rmsg;

		int i;
		for (i = 0; i < countZone; i++)
		{
			msg >> zone >> count
				>> nProduce >> nShop >> nPlay;

			// Server - SubNum - Zone - Player: n - m - l - count
			strcpy(g_buf, "Server-SubNum-Zone-Player: ");
			if (nServer < 0)
				strcat(g_buf, "all");
			else
				IntCat(g_buf, nServer, false);
			strcat(g_buf, "-");
			if (nSubNo < 0)
				strcat(g_buf, "all");
			else
				IntCat(g_buf, nSubNo, false);
			strcat(g_buf, "-");
			IntCat(g_buf, zone, false);
			strcat(g_buf, "-");
			IntCat(g_buf, count, false);
			strcat(g_buf, "-");
			IntCat(g_buf, nProduce, false);
			strcat(g_buf, "-");
			IntCat(g_buf, nShop, false);
			strcat(g_buf, "-");
			IntCat(g_buf, nPlay, false);


			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}

void ProcessMessengerMessage_LogoutRep(CNetMsg& msg)
{
	if (IS_RUNNING_CONN)
	{
		char success;
		CLCString id(MAX_ID_NAME_LENGTH + 1);
		msg >> success >> id;
// 050224 : bs : 중복 로그아웃 요청 수정
//		if (success == 0)
//		{
//			CNetMsg rmsg;
//			ConnLogoutMsg(rmsg, id);
//			SEND_Q(rmsg, gserver.m_connector);
//		}
// --- 050224 : bs : 중복 로그아웃 요청 수정
	}
}

#ifdef NEW_DOUBLE_GM
#ifdef NEW_DOUBLE_GM_ZONE
void ProcessMessengerMessage_DoubleEventRep(CNetMsg& msg, int zone)
#else
void ProcessMessengerMessage_DoubleEventRep(CNetMsg& msg)
#endif // NEW_DOUBLE_GM_ZONE
{
	CNetMsg rmsg;
	int charindex, tserver, tsub, cmd, nas, nasget, exp, sp, produce, pronum;
	msg >> charindex
		>> tserver
		>> tsub
		>> cmd
		>> nas
		>> nasget
		>> exp
		>> sp
		>> produce
		>> pronum;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
#ifdef NEW_DOUBLE_GM_ZONE
		sprintf(g_buf, "Double Event: Server %2d-%2d Zone %2d: ", tserver, tsub, zone);
#else
		sprintf(g_buf, "Double Event: Server %2d-%2d: ", tserver, tsub);
#endif // NEW_DOUBLE_GM_ZONE
		switch (cmd)
		{
		case 0:
			strcat(g_buf, "start");
			break;
		case 1:
			strcat(g_buf, "ing");
			break;
		case 2:
			strcat(g_buf, "stop");
			break;
		}

		StrCat(g_buf, "Nas_Drop:");
		IntCat(g_buf, nas, false);
		StrCat(g_buf, "%");
		StrCat(g_buf, "Nas_Get:");
		IntCat(g_buf, nasget, false);
		StrCat(g_buf, "%");
		StrCat(g_buf, "Exp:");
		IntCat(g_buf, exp, false);
		StrCat(g_buf, "%");
		StrCat(g_buf, "Sp:");
		IntCat(g_buf, sp, false);
		StrCat(g_buf, "%");
		StrCat(g_buf, "Produce:");
		IntCat(g_buf, produce, false);
		StrCat(g_buf, "%");
		StrCat(g_buf, "ProNum:");
		IntCat(g_buf, pronum, false);
		StrCat(g_buf, " ");

		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, pc->m_desc);
	}
}
#endif // NEW_DOUBLE_GM

#ifdef CHANCE_EVENT
void ProcessMessengerMessage_ChanceEventRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int charindex, tserver, tsub, cmd, slevel, elevel, nas, nasget, exp, sp, produce, pronum;
	msg >> charindex
		>> tserver
		>> tsub
		>> cmd
		>> slevel
		>> elevel
		>> nas
		>> nasget
		>> exp
		>> sp
		>> produce
		>> pronum;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		sprintf(g_buf, "Chance Event: Server %2d-%2d: ", tserver, tsub);
		switch (cmd)
		{
		case 0:
			strcat(g_buf, "start");
			break;
		case 1:
			strcat(g_buf, "ing");
			break;
		case 2:
			strcat(g_buf, "stop");
			break;
		}

		StrCat(g_buf, "level:");
		IntCat(g_buf, slevel, false);

		StrCat(g_buf, " - ");
		IntCat(g_buf, elevel, false);

		StrCat(g_buf, "Nas_Drop:");
		IntCat(g_buf, nas, false);
		StrCat(g_buf, "%");
		StrCat(g_buf, "Nas_Get:");
		IntCat(g_buf, nasget, false);
		StrCat(g_buf, "%");
		StrCat(g_buf, "Exp:");
		IntCat(g_buf, exp, false);
		StrCat(g_buf, "%");
		StrCat(g_buf, "Sp:");
		IntCat(g_buf, sp, false);
		StrCat(g_buf, "%");
		StrCat(g_buf, "Produce:");
		IntCat(g_buf, produce, false);
		StrCat(g_buf, "%");
		StrCat(g_buf, "ProNum:");
		IntCat(g_buf, pronum, false);
		StrCat(g_buf, " ");

		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, pc->m_desc);
	}
}
#endif // CHANCE_EVENT

void ProcessMessengerMessage_DoubleExpEventRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int charindex, tserver, tsub, cmd;
	msg >> charindex
		>> tserver
		>> tsub
		>> cmd;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		sprintf(g_buf, "Double Exp Event: Server %2d-%2d: ", tserver, tsub);
		switch (cmd)
		{
		case 0:
			strcat(g_buf, "start");
			break;
		case 1:
			strcat(g_buf, "ing");
			break;
		case 2:
			strcat(g_buf, "stop");
			break;
		}

		IntCat(g_buf, gserver.m_expPercent, true);
		StrCat(g_buf, "%");

		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcessMessengerMessage_MessengerChatRep(CNetMsg& msg)
{
#ifdef ENABLE_MESSENGER
	int sidx;//, ridx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	msg >> sidx >> sname
		>> rname
		>> chat;
	
	CPC* sender = gserver.m_playerList.Find(sidx);
	if (sender != NULL)
	{
		// 송신자가 있으면
		CNetMsg cmsg;
		ChatMessengerMsg(cmsg, sidx, sname, rname, chat);
		SEND_Q(cmsg, sender->m_desc);
	}
#endif
}

void ProcessMessengerMessage_MessengerchatNotfound(CNetMsg& msg)
{
#ifdef ENABLE_MESSENGER
	int sidx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);

	msg >> sidx >> sname;
	
	CPC* sender = gserver.m_playerList.Find(sidx);
	if (sender != NULL)
	{
		// 송신자가 있으면 오류 알림
		CNetMsg smsg;
		SysFriendNotFoundMsg(smsg);
		SEND_Q(smsg, sender->m_desc);
	}
#endif
}

#ifdef RECOMMEND_SERVER_SYSTEM
void ProcessMessengerMessage_Recommend(CNetMsg& msg)
{
	CNetMsg rmsg;

	int nGMCharIndex;
	int nRecommendServer;

	msg >> nGMCharIndex
		>> nRecommendServer;

	GAMELOG << init("RECOMMEND SERVER")
			<< nRecommendServer << delim
			<< "BY" << delim
			<< nGMCharIndex
			<< end;

	if (nRecommendServer == gserver.m_serverno)
	{
		if (!gserver.m_bRecommend)
		{
			gserver.m_bRecommend = true;
			EventRecommendSetMsg(rmsg);
			gserver.m_playerList.SendToAll(rmsg);
		}
	}
	else if (gserver.m_bRecommend)
	{
		gserver.m_bRecommend = false;
		EventRecommendSetMsg(rmsg);
		gserver.m_playerList.SendToAll(rmsg);
	}

	CPC* pGMPC = gserver.m_playerList.Find(nGMCharIndex);
	if (pGMPC)
	{
		if (nRecommendServer > 0)
		{
			CLCString chat(100);
			chat.Format("Set Recommend Server: %d", nRecommendServer);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		}
		else
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Reset Recommend Server");
		SEND_Q(rmsg, pGMPC->m_desc);
	}
}

void ProcessMessengerMessage_RecommendMoonstone(CNetMsg& msg)
{
	int nCharIndex;
	CLCString strCharName(MAX_CHAR_NAME_LENGTH + 1);
	msg >> nCharIndex
		>> strCharName;
	CNetMsg rmsg;
	EventRecommendMoonstoneMsg(rmsg, nCharIndex, strCharName);
	gserver.m_playerList.SendToAll(rmsg);
}
#endif // RECOMMEND_SERVER_SYSTEM

void ProcessMessengerMessage_EventGoldenball(CNetMsg& msg)
{
	CNetMsg rmsg;

	int nSubType;
	msg >> nSubType;

	int			nGMCharIndex;
	CLCString	strTeam1(GOLDENBALL_TEAM_NAME_LENGTH + 1);
	CLCString	strTeam2(GOLDENBALL_TEAM_NAME_LENGTH + 1);
	int			nTeam1Score;
	int			nTeam2Score;
	int			nEndYear;
	int			nEndMonth;
	int			nEndDay;
	int			nEndHour;
	int			nEndMin;
	int			nEndVoteTime;

	CPC*		pGMChar;
	CLCString	strGMMessage(1000);

	switch (nSubType)
	{
	case MSG_MSGR_EVENT_GOLDENBALL_VOTE:
		msg >> nGMCharIndex
			>> strTeam1
			>> strTeam2
			>> nEndYear
			>> nEndMonth
			>> nEndDay
			>> nEndHour
			>> nEndMin;
		EventGoldenballVoteStartMsg(rmsg, strTeam1, strTeam2, nEndYear, nEndMonth, nEndDay, nEndHour, nEndMin);
		gserver.m_playerList.SendToAll(rmsg);
		gserver.m_clGoldenBall.Vote(strTeam1, strTeam2, nEndYear, nEndMonth, nEndDay, nEndHour, nEndMin);
		pGMChar = gserver.m_playerList.Find(nGMCharIndex);
		if (pGMChar)
		{
			strGMMessage.Format("GOLDEN BALL VOTE : [%s] vs [%s] : %04d-%02d-%02d %02d:%02d"
				, (const char*)strTeam1
				, (const char*)strTeam2
				, nEndYear
				, nEndMonth
				, nEndDay
				, nEndHour
				, nEndMin
				);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", strGMMessage);
			SEND_Q(rmsg, pGMChar->m_desc);
		}
		GAMELOG << init("EVENT GOLDEN BALL VOTE SET")
				<< "BY" << delim
				<< nGMCharIndex << delim
				<< "TEAM1" << delim
				<< strTeam1 << delim
				<< "TEAM2" << delim
				<< strTeam2 << delim
				<< "END" << delim
				<< nEndYear << delim
				<< nEndMonth << delim
				<< nEndDay << delim
				<< nEndHour << delim
				<< nEndMin
				<< end;
		break;

	case MSG_MSGR_EVENT_GOLDENBALL_GIFT:
		msg >> nGMCharIndex
			>> strTeam1
			>> nTeam1Score
			>> strTeam2
			>> nTeam2Score
			>> nEndYear
			>> nEndMonth
			>> nEndDay
			>> nEndHour
			>> nEndMin
			>> nEndVoteTime;
		EventGoldenballGiftStartMsg(rmsg, strTeam1, nTeam1Score, strTeam2, nTeam2Score, nEndYear, nEndMonth, nEndDay, nEndHour, nEndMin);
		gserver.m_playerList.SendToAll(rmsg);
		gserver.m_clGoldenBall.Gift(strTeam1, nTeam1Score, strTeam2, nTeam2Score, nEndYear, nEndMonth, nEndDay, nEndHour, nEndMin, (time_t)nEndVoteTime);
		pGMChar = gserver.m_playerList.Find(nGMCharIndex);
		if (pGMChar)
		{
			strGMMessage.Format("GOLDEN BALL GIFT : [%s][%d] vs [%s][%d] : %04d-%02d-%02d %02d:%02d"
				, gserver.m_clGoldenBall.GetTeam1Name()
				, nTeam1Score
				, gserver.m_clGoldenBall.GetTeam2Name()
				, nTeam2Score
				, nEndYear
				, nEndMonth
				, nEndDay
				, nEndHour
				, nEndMin
				);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", strGMMessage);
			SEND_Q(rmsg, pGMChar->m_desc);
		}
		GAMELOG << init("EVENT GOLDEN BALL GIFT SET")
				<< "BY" << delim
				<< nGMCharIndex << delim
				<< "TEAM1" << delim
				<< strTeam1 << delim
				<< nTeam1Score << delim
				<< "TEAM2" << delim
				<< strTeam2 << delim
				<< nTeam2Score << delim
				<< "END" << delim
				<< nEndYear << delim
				<< nEndMonth << delim
				<< nEndDay << delim
				<< nEndHour << delim
				<< nEndMin << delim
				<< "END VOTE" << delim
				<< nEndVoteTime
				<< end;
		break;

	case MSG_MSGR_EVENT_GOLDENBALL_END:
		msg >> nGMCharIndex;
		gserver.m_clGoldenBall.End();
		pGMChar = gserver.m_playerList.Find(nGMCharIndex);
		if (pGMChar)
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "GOLDEN BALL END");
			SEND_Q(rmsg, pGMChar->m_desc);
		}
		GAMELOG << init("EVENT GOLDEN BALL END")
				<< "BY" << delim
				<< nGMCharIndex
				<< end;
		break;

	default:
		break;
	}
}
void ProcessEventMessage_Valentine2006(int server, int subno, int charindex, int flag)
{
#ifdef EVENT_VALENTINE_2006
	CNetMsg rmsg;

	bool	bSendReply = false;		// 명령을 보낸 서버에 응답을 보낼지 여부
	int		nReplyFlag = -1;		// 응답 플래그
	const char* strState = "";		// 알림용

	switch (flag)
	{
	case 0:		// stop
		gserver.m_bEventValentine2006 = false;
		bSendReply = true;
		nReplyFlag = 3;
		break;

	case 1:		// start
		gserver.m_bEventValentine2006 = true;
		bSendReply = true;
		nReplyFlag = 4;
		break;

	case 2:		// ing
		bSendReply = true;
		nReplyFlag = (gserver.m_bEventValentine2006) ? 4 : 3;
		break;

	case 3:		// reply : stop
		strState = "stop";
		break;

	case 4:		// reply : ing
		strState = "ing";
		break;

	default:
		return ;
	}

	if (bSendReply)
	{
		if (IS_RUNNING_MSGR)
		{
			MsgrEventReqMsg(rmsg, server, subno, -1, gserver.m_serverno, gserver.m_subno, charindex, "valentine2006", nReplyFlag);
			SEND_Q(rmsg, gserver.m_messenger);
		}
	}
	else
	{
		CPC* pc = gserver.m_playerList.Find(charindex);
		if (pc)
		{
			sprintf(g_buf, "Valentine 2006: %d - %d %s", server, subno, strState);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
			SEND_Q(rmsg, pc->m_desc);
		}
	}
#endif // #ifdef EVENT_VALENTINE_2006
}

void ProcessEventMessage_NewYear2006(int server, int subno, int charindex, int flag)
{
#ifdef EVENT_NEWYEAR_2006_TIME
	CNetMsg rmsg;

	bool	bSendReply = false;		// 명령을 보낸 서버에 응답을 보낼지 여부
	int		nReplyFlag = -1;		// 응답 플래그
	const char* strState = "";		// 알림용

	switch (flag)
	{
	case 0:		// stop
		gserver.m_bNewYear2006Event = false;
		bSendReply = true;
		nReplyFlag = 3;
		break;

	case 1:		// start
		gserver.m_bNewYear2006Event = true;
		bSendReply = true;
		nReplyFlag = 4;
		break;

	case 2:		// ing
		bSendReply = true;
		nReplyFlag = (gserver.m_bNewYear2006Event) ? 4 : 3;
		break;

	case 3:		// reply : stop
		strState = "stop";
		break;

	case 4:		// reply : ing
		strState = "ing";
		break;

	default:
		return ;
	}

	if (bSendReply)
	{
		if (IS_RUNNING_MSGR)
		{
			MsgrEventReqMsg(rmsg, server, subno, -1, gserver.m_serverno, gserver.m_subno, charindex, "newyear2006", nReplyFlag);
			SEND_Q(rmsg, gserver.m_messenger);
		}
	}
	else
	{
		CPC* pc = gserver.m_playerList.Find(charindex);
		if (pc)
		{
			sprintf(g_buf, "NewYear 2006: %d - %d %s", server, subno, strState);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
			SEND_Q(rmsg, pc->m_desc);
		}
	}
#endif // #ifdef EVENT_NEWYEAR_2006_TIME
}

void ProcessEventMessage_XMas2005(int server, int subno, int charindex, int flag)
{
#ifdef EVENT_XMAS_2005
	CNetMsg rmsg;

	bool	bSendReply = false;		// 명령을 보낸 서버에 응답을 보낼지 여부
	int		nReplyFlag = -1;		// 응답 플래그
	const char* strState = "";		// 알림용

	switch (flag)
	{
	case 0:		// stop
		gserver.m_bXMas2005Event = false;
		bSendReply = true;
		nReplyFlag = 3;
		break;

	case 1:		// start
		gserver.m_bXMas2005Event = true;
		bSendReply = true;
		nReplyFlag = 4;
		break;

	case 2:		// ing
		bSendReply = true;
		nReplyFlag = (gserver.m_bXMas2005Event) ? 4 : 3;
		break;

	case 3:		// reply : stop
		strState = "stop";
		break;

	case 4:		// reply : ing
		strState = "ing";
		break;

	default:
		return ;
	}

	if (bSendReply)
	{
		if (IS_RUNNING_MSGR)
		{
			MsgrEventReqMsg(rmsg, server, subno, -1, gserver.m_serverno, gserver.m_subno, charindex, "xmas2005", nReplyFlag);
			SEND_Q(rmsg, gserver.m_messenger);
		}
	}
	else
	{
		CPC* pc = gserver.m_playerList.Find(charindex);
		if (pc)
		{
			sprintf(g_buf, "XMas 2005: %d - %d %s", server, subno, strState);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
			SEND_Q(rmsg, pc->m_desc);
		}
	}
#endif // #ifdef EVENT_XMAS_2005
}

void ProcessEventMessage_Itemdrop(int server, int subno, int charindex, int rate)
{
	switch (rate)
	{
	case 0:		// stop
		gserver.m_nItemDropEventRate = 100;
		break;
	case -1:	// ing
		break;
	default:	// start
		gserver.m_nItemDropEventRate = rate;
		break;
	}

	if (gserver.m_serverno == server && gserver.m_subno == subno)
	{
		CPC* pc = gserver.m_playerList.Find(charindex);
		if (pc)
		{
			CNetMsg rmsg;
			if (gserver.m_nItemDropEventRate == 100)
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Event Rice Stop!!");
			else
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Event Itemdrop Start!!");
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}

void CServer::ProcessEventMessage(CNetMsg& msg)
{
	int server, subno, charIndex, flag;
	CLCString cmdstring(256);

	msg >> server
		>> subno
		>> charIndex
		>> cmdstring
		>> flag;

	if (strcmp(cmdstring, "itemdrop") == 0)
		ProcessEventMessage_Itemdrop(server, subno, charIndex, flag);

	else if (strcmp(cmdstring, "xmas2005") == 0)
		ProcessEventMessage_XMas2005(server, subno, charIndex, flag);

	else if (strcmp(cmdstring, "newyear2006") == 0)
		ProcessEventMessage_NewYear2006(server, subno, charIndex, flag);

	else if (strcmp(cmdstring, "valentine2006") == 0)
		ProcessEventMessage_Valentine2006(server, subno, charIndex, flag);

}


#ifdef EVENTSETTING
void ProcessMessengerMessage_EventSetting(CNetMsg& msg)
{
	int server, subno, charindex, flag, success;

	msg >> server
		>> subno
		>> charindex
		>> flag
		>> success;

	bool bSendReply = false;
	CNetMsg rmsg;
	switch(flag)
	{
	case 0:			// 이벤트 세팅
		{
			bSendReply = true;

			msg.Init(MSG_MSGR_REQ);
			msg << -1
				<< -1
				<< -1
				<< -1
				<< (unsigned char) MSG_MSGR_EVENT_SETTING_REQ
				<< server
				<< subno
				<< charindex
				<< 1
				<< 0;
			if(!gserver.m_EventSetting.Load(rmsg))
			{
				rmsg.Init(MSG_MSGR_REQ);
				rmsg << -1
					 << server
					 << subno
					 << -1
					 << (unsigned char) MSG_MSGR_EVENT_SETTING_REQ
					 << gserver.m_serverno
					 << gserver.m_subno
					 << charindex
					 << -1
					 << 0;
			}
		}
		break;
	case 1:		// 이벤트 새롭게 설정함
		{
			bSendReply = true;

			int nTotalCount, nEventIdx, nEventEnable, nEventExtra, nEventNotice;
			msg >> nTotalCount;

			if(nTotalCount < 0 || nTotalCount >= gserver.m_EventSetting.GetEventCount())
			{
				MsgEventSetReqMsg(rmsg, server, subno, -1, gserver.m_serverno, gserver.m_subno, charindex, -1, 1);
				break;
			}

			struct EVENT_TEMP_TABLE
			{
				int  nEventIdx;
				int  nEventEnable;
				int  nEventExtra;
				bool bNotice;
			};

			EVENT_TEMP_TABLE* pTemTable;
			pTemTable = new EVENT_TEMP_TABLE[nTotalCount];

			int i;
			bool bFailed = false;
			for(i = 0 ; i < nTotalCount; ++i)
			{
				msg >> nEventIdx
					>> nEventEnable
					>> nEventExtra
					>> nEventNotice;
				
				if(nEventIdx <= 0 
				|| nEventIdx >= gserver.m_EventSetting.GetEventCount() 
				|| nEventEnable <= 0 
				|| nEventEnable > 9999)
				{
					bFailed = true;
					break;
				}
				
				pTemTable[i].nEventIdx		= nEventIdx;
				pTemTable[i].nEventEnable	= nEventEnable;
				pTemTable[i].nEventExtra	= nEventExtra;
				
				int min, max;
				min = (nEventNotice >> 16) & 0xffff;
				max = nEventNotice & 0xffff;
				if(min <= nEventEnable || nEventEnable <= max)
				{
					pTemTable[i].bNotice = true;
				}
				else
				{
					pTemTable[i].bNotice = false;
				}
			}
			
			if(!bFailed)
			{
				gserver.m_EventSetting.Backup();		// 백업
				gserver.m_EventSetting.Reset();			// 초기화
				for(i = 0; i < nTotalCount; ++i)
				{
					if(gserver.m_EventSetting.SetEventData(pTemTable[i].nEventIdx, pTemTable[i].nEventEnable, pTemTable[i].nEventExtra, pTemTable[i].bNotice) == 0)
					{
						bFailed = true;
						gserver.m_EventSetting.RollBack();	// 롤백
						break;
					}
				}
				if(i == nTotalCount)
					gserver.m_EventSetting.DeleteBackupData();	// 성공시 백업데이터 삭제
			}

			if(pTemTable)
			{
				delete[] pTemTable;
				pTemTable = NULL;
			}

			if(bFailed)
			{
				MsgEventSetReqMsg(rmsg, server, subno, -1, gserver.m_serverno, gserver.m_subno, charindex, -1, 1);
			}
			else
			{
				MsgEventSetReqMsg(rmsg, server, subno, -1, gserver.m_serverno, gserver.m_subno, charindex, -1, 2);
			}
		}
		break;
	case 2:			// 현재 세팅 되어있는 것의 개수를 구한다.
		{
			bSendReply = true;
			MsgEventCurrentRepMsg(rmsg, server, subno, -1, gserver.m_serverno, gserver.m_subno, charindex);
		}
		break;
	}
	
	if(bSendReply)
	{
		if(IS_RUNNING_MSGR)
		{
			SEND_Q(rmsg, gserver.m_messenger);
		}
	}
	else
	{
		switch(success)
		{
		case 0:			// 디비에서 읽어드릴 때 잘못됐다.
			{
				CPC* pc = gserver.m_playerList.Find(charindex);
				if (pc)
				{
					sprintf(g_buf, "EventTable Loading DB Error");
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
					SEND_Q(rmsg, pc->m_desc);
				}
			}
			break;
		case 1:			// 디비에서 읽은 것은 잘 됐는데 이상한 값이 전송됐다.
			{
				CPC* pc = gserver.m_playerList.Find(charindex);
				if (pc)
				{
					sprintf(g_buf, "[%d-%d] Event Setting Error", server, subno);
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
					SEND_Q(rmsg, pc->m_desc);
				}
			}
			break;
		case 2:			// 성공
			{
				CPC* pc = gserver.m_playerList.Find(charindex);
				if (pc)
				{
					sprintf(g_buf, "[%d-%d] Event Setting OK", server, subno);
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
					SEND_Q(rmsg, pc->m_desc);
				}
			}
			break;
		case 3:
			{
				CPC* pc = gserver.m_playerList.Find(charindex);
				if (pc)
				{
					sprintf(g_buf, "[%d-%d] CurrentEvent:", server, subno);

					int tmp;
					int i;
					for(i = 0; i < MAX_NOTICE - 1; ++i)
					{
						msg >> tmp;
						IntCat(g_buf, tmp, false);
						StrCat(g_buf, "-");
					}
					msg >> tmp;
					IntCat(g_buf, tmp, false);
					
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
					SEND_Q(rmsg, pc->m_desc);
				}
			}
		}
	}
}
#endif // EVENTSETTING


#ifdef GMTOOL
void ProcessMessengerMessage_GmToolKickIDReq(CNetMsg& msg, int seq, int serverno, int subno, int zone)
{
	CNetMsg rmsg;

	static CLCString id(MAX_ID_NAME_LENGTH + 1);

	int server, sub, charindex;
	msg >> server
		>> sub
		>> charindex
		>> id;

	char success = 0;
	CDescriptor* desc = gserver.FindConnectIdName(id, NULL);
	if (desc)
	{
		gserver.CloseSocket(desc, false);
		success = 1;
	}

	if (IS_RUNNING_MSGR)
	{
		CNetMsg rmsg;
		MsgGmKickIDCommandRepMsg(rmsg, seq, server, sub, zone, (char)success, charindex, id);
		SEND_Q(rmsg, gserver.m_messenger);
	}
}

void ProcessMessengerMessage_GmToolKickReq(CNetMsg& msg, int seq, int serverno, int subno, int zone)
{
	CNetMsg rmsg;

	static CLCString name(MAX_CHAR_NAME_LENGTH + 1);

	int server, sub, charindex;
	msg >> server
		>> sub
		>> charindex
		>> name;

	char success = 0;
	CPC* pc = gserver.m_playerList.Find(name, true);
	if (pc)
	{
		gserver.CloseSocket(pc->m_desc, false);
		success = 1;
	}

	if (IS_RUNNING_MSGR)
	{
		CNetMsg rmsg;
		MsgGmKickCommandRepMsg(rmsg, seq, server, sub, zone, (char)success, charindex, name);
		SEND_Q(rmsg, gserver.m_messenger);
	}
}



void ProcessMessengerMessage_GmToolChatMonitorReq(CNetMsg& msg, int seq, int serverno, int subno, int zone)
{
	CNetMsg rmsg;

	static CLCString name(MAX_ID_NAME_LENGTH + 1);
	static CLCString onoff(100);
	int gmcharindex;
	msg >> gmcharindex
		>> name
		>> onoff;

	char success = 0;
	CPC* pc = gserver.m_playerList.Find(name, false);
	if(pc)
	{
		if(strcmp((const char*)onoff, "on") == 0)
		{
			pc->m_bGmMonitor = true;

			CParty* party = pc->m_party;
			if(party)
			{
				party->m_bGmtarget = true;
			}
			CGuildMember* guildmember = gserver.m_guildlist.findmember(pc->m_index);
			if(guildmember)
			{
				CGuild* guild = guildmember->guild();
				if(guild)
				{
					guild->m_bGmtarget = true;
				}
			}
			success = 1;
		}
		else if(strcmp((const char*)onoff, "off") == 0)
		{
			pc->m_bGmMonitor = false;
			success = 2;
		}
	}
	else
	{
		success = 0;
	}

	if (IS_RUNNING_MSGR)
	{
		MsgrGmToolChatMonitorRepMsg(rmsg, seq, 1, 1, -1, success, gmcharindex, name);
		SEND_Q(rmsg, gserver.m_messenger);
	}
}

void ProcessMessengerMessage_GmToolKickIDRep(CNetMsg& msg)
{
	if (IS_RUNNING_CONN)
	{
		char success;
		CLCString id(MAX_ID_NAME_LENGTH + 1);
		int charindex;
		msg >> success >> charindex >> id;

		CPC* gmtool = gserver.m_playerList.Find(charindex);
		if(gmtool)
		{
			char chat[100];
			sprintf(chat, "Kick %s ID: %s", (success ? "success" : "not found"), (const char*)id);

			CNetMsg rmsg;
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, gmtool->m_desc);
		}
// 050224 : bs : 중복 로그아웃 요청 수정
//		if (success == 0)
//		{
//			CNetMsg rmsg;
//			ConnLogoutMsg(rmsg, id);
//			SEND_Q(rmsg, gserver.m_connector);
//		}
// --- 050224 : bs : 중복 로그아웃 요청 수정
	}
}

void ProcessMessengerMessage_GmToolKickRep(CNetMsg& msg)
{
	if (IS_RUNNING_CONN)
	{
		char success;
		CLCString name(MAX_CHAR_NAME_LENGTH + 1);
		int charindex;
		msg >> success >> charindex >> name;

		CPC* gmtool = gserver.m_playerList.Find(charindex);
		if(gmtool)
		{
			char chat[100];
			sprintf(chat, "Kick %s NAME: %s", (success ? "success" : "not found"), (const char*)name);

			CNetMsg rmsg;
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, gmtool->m_desc);
		}
// 050224 : bs : 중복 로그아웃 요청 수정
//		if (success == 0)
//		{
//			CNetMsg rmsg;
//			ConnLogoutMsg(rmsg, id);
//			SEND_Q(rmsg, gserver.m_connector);
//		}
// --- 050224 : bs : 중복 로그아웃 요청 수정
	}
}


void ProcessMessengerMessage_GmToolChatMonitorRep(CNetMsg& msg)
{
	char success;
	CLCString id(MAX_ID_NAME_LENGTH + 1);
	int gmcharindex;
	msg >> success >> gmcharindex >> id;

	CPC* gmtool;
	gmtool = gserver.m_playerList.Find(gmcharindex);

	if(gmtool)
	{
		char chat[100];

		switch(success)
		{
		case 0:
			sprintf(chat, "GMTOOL CHATMONITOR NOT FOUND ID: %s", (const char*)id);
			break;
		case 1:
			sprintf(chat, "GMTOOL CHATMONITOR ID: %s ON", (const char*)id);
			break;
		case 2:
			sprintf(chat, "GMTOOL CHATMONITOR OFF ID: %s OFF", (const char*)id);
			break;
		}
		
		CNetMsg rmsg;
		GMSayMsg(rmsg, 0, 0, 0, "", chat, MSG_CHAT_GM_SAY);
		SEND_Q(rmsg, gmtool->m_desc);
	}
}

void ProcessMessengerMessage_GmToolChatMonitorNotice(CNetMsg& msg)
{
	int server, sub, zone;
	int charindex;
	unsigned char chattype;
	CLCString chat(100);
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
//	CLCString recever(MAX_ID_NAME_LENGTH + 1)
	msg >> server
		>> sub
		>> zone
		>> chat
		>> charindex
		>> name
		>> chattype;

	if(gserver.m_gmtool && gserver.m_nGMToolCount > 0)
	{
		int i;
		CNetMsg rmsg;
		bool bSend = false;
		for(i = 0; i < gserver.m_nGMToolCount; ++i)
		{
			if(gserver.m_gmtool[i])
			{
				GMSayMsg(rmsg, server, sub, charindex, name, chat, chattype);
				SEND_Q(rmsg, gserver.m_gmtool[i]);

				bSend = true;
			}
		}
		
		if(bSend)
		{
			// 디비에 저장
			if( IS_RUNNING_CONN )
			{
				CNetMsg rmsg;
				ConnGMToolChatMonitor(rmsg, charindex, name, chattype, server, sub, chat);
				SEND_Q(rmsg, gserver.m_connector);
			}
		}
		else
		{
			// 툴이 없으면 감시 해제한다.
			CNetMsg rmsg;
			if (IS_RUNNING_MSGR)
			{
				MsgrGmToolChatMonitorOffMsg(rmsg, -1, server, sub, zone, charindex);
				SEND_Q(rmsg, gserver.m_messenger);
			}
		}
	}
}

void ProcessMessengerMessage_GmToolChatMonitorNoticeWhisper(CNetMsg& msg)
{
	int server, sub, zone, charindex;
	unsigned char chattype;
	CLCString chat(1000), sender(MAX_CHAR_NAME_LENGTH + 1), name(MAX_CHAR_NAME_LENGTH + 1);
	msg >> server
		>> sub
		>> zone
		>> chat
		>> charindex
		>> name
		>> chattype
		>> sender;

	char gmchat[1000];
	memset(gmchat, 0, 1000);
	sprintf(gmchat, "WHISPER: %s ==> %s: %s", (const char*)sender,(const char*) name, (const char*)chat);

	CNetMsg rmsg;
	if(gserver.m_gmtool && gserver.m_nGMToolCount > 0)
	{
		int i;
		bool bSend = false;
		for(i = 0 ; i < gserver.m_nGMToolCount; ++i)
		{
			if(gserver.m_gmtool[i])
			{
				GMSayMsg(rmsg, server, sub, charindex, name, gmchat, chattype);
				SEND_Q(rmsg, gserver.m_gmtool[i]);

				bSend = true;
			}
		}
		
		if(bSend)
		{
			if( IS_RUNNING_CONN )
			{
				CNetMsg rmsg;
				ConnGMToolChatMonitor(rmsg, charindex, name, chattype, server, sub, chat);
				SEND_Q(rmsg, gserver.m_connector);
			}
			
		}
		else
		{
			if (IS_RUNNING_MSGR)
			{
				MsgrGmToolChatMonitorOffMsg(rmsg, -1, server, sub, zone, charindex);
				SEND_Q(rmsg, gserver.m_messenger);
			}
		}
	}
}


void ProcessMessengerMessage_GmToolChatMonitorNoticeParty(CNetMsg& msg)
{
	int server, sub, zone;
	int bossindex;
	int charindex;
	CLCString chat(1000);
	CLCString name(MAX_CHAR_NAME_LENGTH + 1), charname(MAX_CHAR_NAME_LENGTH + 1);
	msg >> server
		>> sub
		>> zone
		>> chat
		>> bossindex
		>> name
		>> charindex
		>> charname;

	char gmchat[1000];
	memset(gmchat, 0, 1000);
	sprintf(gmchat, "PARTYCHAT: %s: %s", (const char*)name, (const char*)chat);
	CNetMsg rmsg;
	if(gserver.m_gmtool && gserver.m_nGMToolCount > 0)
	{	
		int i;
		bool bSend = false;
		for(i = 0 ; i < gserver.m_nGMToolCount; ++i)
		{
			if(gserver.m_gmtool[i])
			{
				GMSayMsg(rmsg, server, sub, charindex, name, gmchat, (unsigned char)MSG_CHAT_PARTY);
				SEND_Q(rmsg, gserver.m_gmtool[i]);
				
				bSend = true;
			}
		}

		if(bSend)
		{
			if( IS_RUNNING_CONN )
			{
				CNetMsg rmsg;
				ConnGMToolChatMonitor(rmsg, charindex, name, (unsigned char)MSG_CHAT_PARTY, server, sub, gmchat);
				SEND_Q(rmsg, gserver.m_connector);
			}
		}
		else
		{
			if(IS_RUNNING_MSGR)
			{
				MsgrGmToolChatMonitorPartyOffMsg(rmsg, -1, server, sub, zone, bossindex);
				SEND_Q(rmsg, gserver.m_messenger);
			}
		}
	}
}

void ProcessMessengerMessage_GmToolChatMonitorNoticeGuild(CNetMsg& msg)
{
	int server, sub, zone;
	int guildindex;
	int charindex;
	CLCString chat(1000);
	CLCString name(MAX_CHAR_NAME_LENGTH + 1), charname(MAX_CHAR_NAME_LENGTH + 1);

	msg >> server
		>> sub
		>> zone
		>> chat
		>> guildindex
		>> name
		>> charindex
		>> charname;

	char gmchat[1000];
	memset(gmchat, 0, 1000);
	sprintf(gmchat, "GUILDCHAT: %s: %s", (const char*)name, (const char*)chat);
	CNetMsg rmsg;
	GMSayMsg(rmsg, server, sub, charindex, charname, gmchat, (unsigned char)MSG_CHAT_GUILD);

	if(gserver.m_gmtool && gserver.m_nGMToolCount > 0)
	{
		int i;
		bool bSend = false;
		for(i = 0; i < gserver.m_nGMToolCount; ++i)
		{
			if(gserver.m_gmtool[i])
			{
				SEND_Q(rmsg, gserver.m_gmtool[i]);
				bSend = true;
			}
		}

		if(bSend)
		{
			if( IS_RUNNING_CONN )
			{
				CNetMsg rmsg;
				ConnGMToolChatMonitor(rmsg, charindex, name, (unsigned char)MSG_CHAT_GUILD, server, sub, gmchat);
				SEND_Q(rmsg, gserver.m_connector);
			}
		}
		else
		{
			if (IS_RUNNING_MSGR)
			{
				MsgrGmToolChatMonitorGuildOffMsg(rmsg, -1, server, sub, zone, guildindex);
				SEND_Q(rmsg, gserver.m_messenger);
			}
		}
	}
}

void ProcessMessengerMessage_GmToolChatMonitorOff(CNetMsg& msg)
{
	int charindex;
	msg >> charindex;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if(pc)
	{
		pc->m_bGmMonitor = false;

		CParty* party = pc->m_party;
		if(party)
		{
			party->m_bGmtarget = false;
		}
		CGuildMember* guildmember = gserver.m_guildlist.findmember(pc->m_index);
		if(guildmember)
		{
			CGuild* guild = guildmember->guild();
			if(guild)
			{
				guild->m_bGmtarget = false;
			}
		}
	}
}

void ProcessMessengerMessage_GmToolChatMonitorPartyOff(CNetMsg& msg)
{
	int i;
	int bossindex;
	msg >> bossindex;

	CPC* pc = gserver.m_playerList.Find(bossindex);
	if(pc)
	{
		if(pc->IsParty() && pc->m_party)
		{
			CParty* party = pc->m_party;
			party->m_bGmtarget = false;
			for(i = 0; i < MAX_PARTY_MEMBER; ++i)
			{
				if(party->GetMemberByListIndex(i) && party->GetMemberByListIndex(i)->GetMemberPCPtr())
				{
					party->GetMemberByListIndex(i)->GetMemberPCPtr()->m_bGmMonitor = false;
				}
			}
		}
	}
}

void ProcessMessengerMessage_GmToolChatMonitorGuildOff(CNetMsg& msg)
{
	int i;
	int guildindex;
	msg >> guildindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if(guild)
	{
		guild->m_bGmtarget = false;
		for(i = 0 ; i < GUILD_MAX_MEMBER; ++i)
		{
			if(guild->member(i) && guild->member(i)->GetPC())
			{
				guild->member(i)->GetPC()->m_bGmMonitor = false;
			}
		}
	}
}

void ProcessMessengerMessage_GmToolWhisperRep(CNetMsg& msg)
{
	CLCString rname(MAX_ID_NAME_LENGTH + 1), sname(MAX_ID_NAME_LENGTH + 1);
	CLCString chat(1000);

	int rindex;
	int sindex;
	int server, sub;
	msg >> rindex
		>> rname
		>> chat
		>> server
		>> sub
		>> sindex;

	CPC* gmtool = gserver.m_playerList.Find(sindex);

	if(gmtool)
	{
		char message[1000];
		memset(message, 0, 1000);
		sprintf(message, "WHISPER: [%d-%d] %s ==> %s: %s", server, sub, (const char*)gmtool->m_name, (const char*)rname, (const char*)chat);

		CNetMsg rmsg;
		GMSayMsg(rmsg,  server, sub, rindex, rname, message, MSG_CHAT_WHISPER);
		SEND_Q(rmsg, gmtool->m_desc);


	}
}

void ProcessMessengerMessage_GMLoopCountReq(CNetMsg& msg)
{
	if(gserver.m_bLoop)
		return;

	gserver.m_bLoop = 2;
	gserver.m_nLoop = 0;
	GetTimeofDay(&gserver.m_tvLoop, NULL);
}


void ProcessMessengerMessage_GMLoopCountRep(CNetMsg& msg)
{
	int serverno, subno;
	CLCString loopcount(100);
	msg >> serverno
		>> subno
		>> loopcount;

	if(gserver.m_descLoop)
	{
		sprintf(g_buf, "[%d-%d] %s", serverno, subno, (const char*)loopcount);
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, gserver.m_descLoop);
		gserver.m_descLoop = NULL;
	}
}

void ProcessMessengerMessage_GMReloadgmip(CNetMsg& msg)
{
#ifdef USING_GMIP_TABLE
	gserver.LoadGMIPTable();
#endif
}


void ProcessMessengerMessage_GmToolSilenceReq(CNetMsg& msg, int seq, int server, int subno, int zone)
{
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	int sec;
	int gmcharindex;

	msg >> gmcharindex
		>> name
		>> sec;

	if(name.Length() < 4 || sec < 1)
	{
		return;
	}

	int	success = 0;
	int i;
	for (i = 0; i < gserver.m_playerList.m_max; i++)
	{
		CPC* pc = gserver.m_playerList.m_pcList[i];

		// 유저 검사
		if (pc && ( strcmp(pc->m_name, name) == 0 || strcmp(pc->m_nick, name) == 0 ))
		{
			success = 1;
			pc->m_silencePulse = gserver.m_pulse + (sec * PULSE_REAL_SEC);
		}
	}

	if(IS_RUNNING_MSGR)
	{
		CNetMsg rmsg;
		MsgGMSilenceRep(rmsg, seq, gserver.m_serverno, gserver.m_subno, success, gmcharindex, (const char* )name);
		SEND_Q(rmsg, gserver.m_messenger);
	}
}

void ProcessMessengerMessage_GmToolSilenceRep(CNetMsg& msg)
{
	int server, sub;
	int success;
	int gmcharindex;
	CLCString name(100);
	msg >> server
		>> sub
		>> success
		>> gmcharindex
		>> name;

	char tmp[256];
	memset(tmp, 0, 256);
	sprintf(tmp, "[%d-%d] Silence Name:%s %s", server, sub, (const char*)name, success ? "Success" : "Not Found");

	CPC* gmtool = gserver.m_playerList.Find(gmcharindex);
	
	if(gmtool)
	{
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmp);
		SEND_Q(rmsg, gmtool->m_desc);
	}
//	sprintf(g_buf, "[%d-%d] Name:%s Silence: %s", server, sub, name,  g_buf2);
}


void ProcessMessengerMessage_GmToolWhisperNotfound(CNetMsg& msg)
{
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	int charindex;
	msg >> charindex
		>> name;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if(pc)
	{
		CNetMsg smsg;
		SysWhisperNotFoundMsg(smsg);
		SEND_Q(smsg, pc->m_desc);
	}
}

void ProcessMessengerMessage_GmWhisperReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int sidx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	msg >> sidx >> sname
		>> rname
		>> chat;

	CPC* receiver = gserver.m_playerList.Find(rname, true );

	if (receiver == NULL)
	{
		// 수신자가 없을 때
		CNetMsg mmsg;
		MsgrRepGMWhisperNotFoundMsg(mmsg, nSeq, 1, 1, 0, sidx);
		if (IS_RUNNING_MSGR) SEND_Q(mmsg, gserver.m_messenger);
	}
	else
	{
		// 수신자가 있을 때
		CNetMsg cmsg;
		ChatGMWhisperMsg(cmsg, sidx, /*sname*/ "GM", receiver->GetName(), chat);		// 툴이 유저에게 보내는 메시지 클라이언트와 협의
		/*
#ifdef MESSENGER_NEW
		if( !receiver->IsBlockPC(sidx, sname) )
#endif
		*/
		
		SEND_Q(cmsg, receiver->m_desc);

		// 메신저에 응답 보내기
		CNetMsg mmsg;
		MsgrRepGMWhisperRepMsg(mmsg, nSeq, 1, 1, 0, receiver->m_index, receiver->GetName(), chat, gserver.m_serverno, gserver.m_subno, sidx);
		if (IS_RUNNING_MSGR) 
			SEND_Q(mmsg, gserver.m_messenger);
	}
}

void ProcessMessengerMessage_GmToolWhisperReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	CLCString chat(1000);
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	CLCString rname(MAX_CHAR_NAME_LENGTH + 1);

	int charindex, serverno, subno;
	msg >> chat
		>> charindex
		>> name
		>> serverno
		>> subno
		>> rname;

	CPC* gmtool = gserver.m_playerList.Find(rname, false);

	if(gmtool)
	{
		char buf[1000];
		memset(&buf, 0, 1000);
		sprintf(buf, "WHISPER: %s ==> %s: %s", (const char*)name, (const char*)gmtool->m_name, (const char*)chat);

		CNetMsg rmsg;
		GMSayMsg(rmsg, serverno, subno, charindex, name, buf, MSG_CHAT_GM_WHISPER);
		SEND_Q(rmsg, gmtool->m_desc);

		MsgrRepGmToolWhisperRepMsg(rmsg, nSeq, serverno, subno, -1, charindex, name, gmtool->m_name, chat);
		if (IS_RUNNING_MSGR) 
			SEND_Q(rmsg, gserver.m_messenger);

		if( IS_RUNNING_CONN )
		{
			ConnGMToolChatting(rmsg, serverno, subno, gmtool->m_index, gmtool->m_name, name, buf);
			SEND_Q(rmsg, gserver.m_connector);
		}
	}
	else
	{
		CNetMsg mmsg;
		MsgrRepGMToolWhisperNotFoundMsg(mmsg, nSeq, nServer, nSubNo, -1, charindex, name);
		if (IS_RUNNING_MSGR) SEND_Q(mmsg, gserver.m_messenger);
	}
}

void ProcessMessengerMessage_GMWhisperNotfound(CNetMsg& msg)
{
	int sindex;
	msg >> sindex;
	CPC* gmtool = gserver.m_playerList.Find(sindex);
	if(gmtool)
	{
		CNetMsg smsg;
		SysWhisperNotFoundMsg(smsg);
		SEND_Q(smsg, gmtool->m_desc);
	}
}


void ProcessMessengerMessage_GMToolUserWhisperRep(CNetMsg& msg)
{
	int sindex;
	CLCString sname(MAX_CHAR_NAME_LENGTH + 1), rname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString chat(1000);

	msg >> sindex
		>> sname >> rname
		>> chat;

	CPC* sender = gserver.m_playerList.Find(sname, true );
	if(sender)
	{
		CNetMsg cmsg;
		ChatGMWhisperMsg(cmsg, sindex, sname, rname, chat);
		SEND_Q(cmsg, sender->m_desc);
	}
}

void ProcessMessengerMessage_GMToolShutdown(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int remain;
	CLCString chat(1000);

	msg >> remain
		>> chat;

	if (gserver.m_pulseShutdown != -1)
		return ;

	gserver.m_pulseShutdown = remain * PULSE_REAL_SEC;

	if (chat.Length() > 0)
	{
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		for (int i = 0; i < gserver.m_playerList.m_max; i++)
		{
			if (gserver.m_playerList.m_pcList[i])
			{
				SEND_Q(rmsg, gserver.m_playerList.m_pcList[i]->m_desc);
			}
		}
	}
}
#endif // GMTOOL

#ifdef DOUBLE_ITEM_DROP
void ProcessMessengerMessage_DoubleItemEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, cmd;
	CNetMsg rmsg;

	msg >> charindex
		>> cmd;

	switch (cmd)
	{
	case 0:
		{
			// start
			if (gserver.m_bDoubleItemEvent)
				MsgrDoubleItemEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_bDoubleItemPercent );
			else
			{
				gserver.m_bDoubleItemEvent = true;
				msg >> gserver.m_bDoubleItemPercent;
				/*        /////// yhj    081014 ///////////////// 확률 제한해제
				if( gserver.m_bDoubleItemPercent < 0 || gserver.m_bDoubleItemPercent > 200 )
				{
					gserver.m_bDoubleItemPercent = 100;
				}
				*///////////////////////////////////////////////////

				MsgrDoubleItemEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, gserver.m_bDoubleItemPercent);
			}
#ifdef DOUBLE_ITEM_DROP_SAVEFILE
				FILE* fpDoubleItemEvent = fopen(DOUBLE_ITEM_DROP_SAVEFILE, "wt");
				if (fpDoubleItemEvent)
				{
					fprintf(fpDoubleItemEvent, "%d\n", gserver.m_bDoubleItemPercent);
					fclose(fpDoubleItemEvent);
					fpDoubleItemEvent = NULL;
				}
#endif // DOUBLE_ITEM_DROP_SAVEFILE
		}
		break;

	case 1:			// ing
		if (gserver.m_bDoubleItemEvent)
			MsgrDoubleItemEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_bDoubleItemPercent);
		else
			MsgrDoubleItemEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_bDoubleItemPercent);
		break;

	case 2:			// stop
		{
		gserver.m_bDoubleItemEvent = false;
		MsgrDoubleItemEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_bDoubleItemPercent);
#ifdef DOUBLE_ITEM_DROP_SAVEFILE
			remove(DOUBLE_ITEM_DROP_SAVEFILE);
#endif // DOUBLE_ITEM_DROP_SAVEFILE
		}
		break;

	default:
		return ;
	}

	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
}

void ProcessMessengerMessage_DoubleItemEventRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int charindex, tserver, tsub, cmd;
	msg >> charindex
		>> tserver
		>> tsub
		>> cmd;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		sprintf(g_buf, "Double Exp Event: Server %2d-%2d: ", tserver, tsub);
		switch (cmd)
		{
		case 0:
			strcat(g_buf, "start");
			break;
		case 1:
			strcat(g_buf, "ing");
			break;
		case 2:
			strcat(g_buf, "stop");
			break;
		}

		IntCat(g_buf, gserver.m_bDoubleItemPercent, true);
		StrCat(g_buf, "%");

		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, pc->m_desc);
	}
}

#endif // DOUBLE_ITEM_DROP



#ifdef DOUBLE_PET_EXP
void ProcessMessengerMessage_DoublePetExpEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, cmd;
	CNetMsg rmsg;

	msg >> charindex
		>> cmd;

	switch (cmd)
	{
	case 0:			// start
		if (gserver.m_bDoublePetExpEvent)
			MsgrDoublePetExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_PetExpPercent);
		else
		{
			gserver.m_bDoublePetExpEvent = true;
			msg >> gserver.m_PetExpPercent;
			if( gserver.m_PetExpPercent < 0 || gserver.m_PetExpPercent > 200 )
			{
				gserver.m_PetExpPercent = 100;
			}

			MsgrDoublePetExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, gserver.m_PetExpPercent);
		}
		break;

	case 1:			// ing
		if (gserver.m_bDoublePetExpEvent)
			MsgrDoublePetExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_PetExpPercent);
		else
			MsgrDoublePetExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_PetExpPercent);
		break;

	case 2:			// stop
		gserver.m_bDoublePetExpEvent = false;
		MsgrDoublePetExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_PetExpPercent);
		break;

	default:
		return ;
	}

	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
}
void ProcessMessengerMessage_DoublePetExpEventRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int charindex, tserver, tsub, cmd;
	msg >> charindex
		>> tserver
		>> tsub
		>> cmd;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		sprintf(g_buf, "Double Pet Exp Event: Server %2d-%2d: ", tserver, tsub);
		switch (cmd)
		{
		case 0:
			strcat(g_buf, "start");
			break;
		case 1:
			strcat(g_buf, "ing");
			break;
		case 2:
			strcat(g_buf, "stop");
			break;
		}

		IntCat(g_buf, gserver.m_PetExpPercent, true);
		StrCat(g_buf, "%");

		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, pc->m_desc);
	}
}
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
void ProcessMessengerMessage_DoubleAttackEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, cmd;
	CNetMsg rmsg;

	msg >> charindex
		>> cmd;

	switch (cmd)
	{
	case 0:			// start
		if (gserver.m_bDoubleAttackEvent)
			MsgrDoubleAttackEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_AttackPercent);
		else
		{
			gserver.m_bDoubleAttackEvent = true;
			msg >> gserver.m_AttackPercent;
			if( gserver.m_AttackPercent < 0 || gserver.m_AttackPercent > 200 )
			{
				gserver.m_AttackPercent = 100;
			}

			MsgrDoubleAttackEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, gserver.m_AttackPercent);
		}
		break;

	case 1:			// ing
		if (gserver.m_bDoubleAttackEvent)
			MsgrDoubleAttackEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver.m_AttackPercent);
		else
			MsgrDoubleAttackEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_AttackPercent);
		break;

	case 2:			// stop
		gserver.m_bDoubleAttackEvent = false;
		MsgrDoubleAttackEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver.m_AttackPercent);
		break;

	default:
		return ;
	}

	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
}
void ProcessMessengerMessage_DoubleAttackEventRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int charindex, tserver, tsub, cmd;
	msg >> charindex
		>> tserver
		>> tsub
		>> cmd;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		sprintf(g_buf, "Double Attack Event: Server %2d-%2d: ", tserver, tsub);
		switch (cmd)
		{
		case 0:
			strcat(g_buf, "start");
			break;
		case 1:
			strcat(g_buf, "ing");
			break;
		case 2:
			strcat(g_buf, "stop");
			break;
		}

		IntCat(g_buf, gserver.m_AttackPercent, true);
		StrCat(g_buf, "%");

		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, pc->m_desc);
	}
}
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
void ProcessMessengerMessage_EventDropItemReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	CNetMsg rmsg;
	int charindex, npcidx, itemidx, prob, fromServer, fromSub;
	char type;

	msg >> charindex
		>> type
		>> npcidx
		>> itemidx
		>> prob
		>> fromServer
		>> fromSub;

	switch(type)
	{
	case 0:		// start
		{
			if(gserver.m_bDropItem)
				MsgrEventDropItemRepMsg(rmsg, nSeq, fromServer, fromSub, -1, charindex, 1, gserver.m_bDropItemNpcIdx, gserver.m_bDropItemIdx, 
				gserver.m_bDropProb, gserver.m_serverno, gserver.m_subno);
			else
			{
				gserver.m_bDropItem = true;
				
				CNPCProto* npcproto = NULL;
				CItem* item = NULL;
				npcproto = gserver.m_npcProtoList.FindProto(npcidx);
				item = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);
				if(npcproto && item)
				{
					if(prob < 1 || prob > 100)
						prob = 1;

					gserver.m_bDropItemNpcIdx	= npcidx;
					gserver.m_bDropItemIdx		= itemidx;
					gserver.m_bDropProb			= prob;
					MsgrEventDropItemRepMsg(rmsg, nSeq, fromServer, fromSub, -1, charindex, 0, npcidx, itemidx, prob, gserver.m_serverno, gserver.m_subno);
				}
				else
				{
					MsgrEventDropItemRepMsg(rmsg, nSeq, fromServer, fromSub, -1, charindex, 4, -1, -1, 0, gserver.m_serverno, gserver.m_subno);
				}

				if(item)
				{
					delete item;
					item = NULL;
				}
			}
		}
		break;
	case 1:		// ing
		{
			if(gserver.m_bDropItem)
				MsgrEventDropItemRepMsg(rmsg, nSeq, fromServer, fromSub, -1, charindex, 1, gserver.m_bDropItemNpcIdx, gserver.m_bDropItemIdx, 
				gserver.m_bDropProb, gserver.m_serverno, gserver.m_subno);
			else
				MsgrEventDropItemRepMsg(rmsg, nSeq, fromServer, fromSub, -1, charindex, 2, gserver.m_bDropItemNpcIdx, gserver.m_bDropItemIdx,
				gserver.m_bDropProb, gserver.m_serverno, gserver.m_subno);
		}
		break;
	case 2:		// stop
		{
			gserver.m_bDropItem = false;
			MsgrEventDropItemRepMsg(rmsg, nSeq, fromServer, fromSub, -1, charindex, 2, gserver.m_bDropItemNpcIdx, gserver.m_bDropItemIdx,
				gserver.m_bDropProb, gserver.m_serverno, gserver.m_subno);
		}
		break;
	}

	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
}

void ProcessMessengerMessage_EventDropItemRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int charindex, npcidx, itemidx, prob, tServer, tSub;
	char type;

	msg >> charindex
		>> type
		>> npcidx
		>> itemidx
		>> prob
		>> tServer
		>> tSub;


	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		sprintf(g_buf, "Event NPC DropItem: Server %2d-%2d: ", tServer, tSub);
		switch(type)
		{
		case 0:
			{
				strcat(g_buf, "start ");
			}
			break;
		case 1:
			{
				strcat(g_buf, "ing ");
			}
			break;
		case 2:
			{
				strcat(g_buf, "stop ");
			}
			break;
		case 3:		// 운영자명령어 실행실패
			{
				strcat(g_buf, "Not Abailable");
			}
			break;
		}

		strcat(g_buf, "NPC:");
		IntCat(g_buf, npcidx);
		strcat(g_buf, " ");
		strcat(g_buf, "Item:");
		IntCat(g_buf, itemidx);
		strcat(g_buf, " ");
		strcat(g_buf, "Prob:");
		IntCat(g_buf, prob);
		
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, pc->m_desc);
	}
}
#endif // EVENT_DROPITEM

void ProcessMessengerMessage_SetExpLimit( CNetMsg& msg)
{
#ifndef LIMIT_EXP_INCREASE
	CNetMsg rmsg;
	int charindex, nLimit;

	msg >> charindex
		>> nLimit;

	if( nLimit > 0 && nLimit <= 3000 )
	{
		gserver.m_nExpLimit = nLimit;
	}

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		char strMsg[64];
		sprintf( strMsg, " Set Limit Sucesses ( %d ) " , gserver.m_nExpLimit );
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", strMsg );
		SEND_Q(rmsg, pc->m_desc);
	}
#endif // LIMIT_EXP_INCREASE
}


void ProcessMessengerMessage_DoubleEventAutoReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
#ifdef NEW_DOUBLE_EVENT_AUTO
	int charindex;
	char cmd;
	CNetMsg rmsg;
	msg >> charindex
		>> cmd;

	switch(cmd)
	{
	case 0:
		gserver.m_bDoubleEventAuto = true;
		gserver.NewDoubleEventAuto();
		if(gserver.m_bDoubleEventAutoOn)
			MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, 1);
		else
			MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, 0);
		break;
	case 1:
		if(gserver.m_bDoubleEventAuto)
		{
			if(gserver.m_bDoubleEventAutoOn)
				MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, 1);
			else
				MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, 0);
		}
		else
			MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, 0);
		break;
	case 2:
		gserver.m_bDoubleEventAuto = false;
		gserver.m_bDoubleEventAutoOn = false;
		MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, 0);
		break;
	default:
		return ;
	}
	
	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
#endif // NEW_DOUBLE_EVENT_AUTO
}



void ProcessMessengerMessage_DoubleEventAutoRep( CNetMsg& msg )
{
#ifdef NEW_DOUBLE_EVENT_AUTO
	CNetMsg rmsg;
	int charindex, server, sub;
	char state, cmd;
	msg >> charindex
		>> server
		>> sub
		>> cmd
		>> state;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if(pc)
	{
		sprintf(g_buf, "Double Event Auto: Server %d-%d: ", server, sub);
		switch(cmd)
		{
		case 0:
			strcat(g_buf, "Start ");
			
			if(state)
				strcat(g_buf, "On");
			else
				strcat(g_buf, "OFF");
			break;
		case 1:
			strcat(g_buf, "ing ");

			if(state)
				strcat(g_buf, "On");
			else
				strcat(g_buf, "OFF");
			break;
		case 2:
			strcat(g_buf, "stop");
			break;
		}

		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, pc->m_desc);
	}
#endif // NEW_DOUBLE_EVENT_AUTO
}

void ProcessMessengerMessage_UpgradeEventReq(CNetMsg& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
#ifdef UPGRADE_EVENT
	int charindex;
	char cmd;
	int prob;
	CNetMsg rmsg;
	msg >> charindex
		>> cmd
		>> prob;

	switch(cmd)
	{
	case 0:
		{
			gserver.m_bUpgradeEvent = true;

			switch(prob)
			{
			case 100:
			case 125:
			case 150:
			case 175:
			case 200:
				break;
			default:
				prob = 100;
				return ;
			}
			gserver.m_nUpgradeProb = prob;
			MsgrUpgradeEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0);
		}
		break;
	case 1:
		{
			if(gserver.m_bUpgradeEvent)
				MsgrUpgradeEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1);
			else
				MsgrUpgradeEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2);
		}
		break;
	case 2:
		{
			gserver.m_bUpgradeEvent = false;
			gserver.m_nUpgradeProb = 100;
			MsgrUpgradeEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2);
		}
		break;
	default:
		return ;
	}
	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);

#endif // UPGRADE_EVENT
}

void ProcessMessengerMessage_UpgradeEventRep( CNetMsg& msg )
{
#ifdef UPGRADE_EVENT
	CNetMsg rmsg;
	int charindex, tserver, tsub, prob;
	char cmd;

	msg >> charindex
		>> tserver
		>> tsub
		>> cmd
		>> prob;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if(pc)
	{
		sprintf(g_buf, "Upgrade Event: Server %2d-%2d: ", tserver, tsub);
		switch (cmd)
		{
		case 0:
			strcat(g_buf, "start");
			break;
		case 1:
			strcat(g_buf, "ing");
			break;
		case 2:
			strcat(g_buf, "stop");
			break;
		}
		StrCat(g_buf, "Prob:");
		IntCat(g_buf, prob, false);
		StrCat(g_buf, "%");

		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, pc->m_desc);
	}
#endif // UPGRADE_EVENT
}