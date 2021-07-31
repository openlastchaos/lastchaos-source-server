#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"

/////////////////
// 채팅 관련 함수
void do_Chat(CPC* ch, CNetMsg& msg)
{
	// ch가 셀에 속해 있을 때, 보일때
	if (!IS_IN_CELL(ch))
		return ;

	msg.MoveFirst();

	*g_buf = '\0';

	unsigned char chatType;
	int sender_index;
	CLCString sender_name(MAX_CHAR_NAME_LENGTH + 1);
	CLCString receiver_name(MAX_CHAR_NAME_LENGTH + 1);
	CLCString chat(1000);
	int	nGroupTyp=-1;  	//원정대 그룹1(0), 그룹 2(1),  그룹 3(2), 그룹 4(3) , 전체(-1)

	msg >> chatType
		>> sender_index
		>> sender_name
		>> receiver_name
		>> chat
		>> nGroupTyp;

	sender_name.Trim();
	receiver_name.Trim();
	chat.Trim();

	CNetMsg gmmsg;
	CLCString tmp(1000);

	// 내용 없는 채팅은 무시
	if (chat.Length() < 1)
		return ;

	// 채팅 금지 Check
	if (gserver.m_pulse - ch->m_silencePulse <= 0)
		return;
	
#ifdef CHAT_BLOCK_USA
	if (chatType == MSG_CHAT_TRADE || chatType == MSG_CHAT_SHOUT)
	{
		if (gserver.m_pulse - ch->m_shoutPulse <= 0)
			return;

#if defined (LC_GER) || defined (LC_EUR)	// 레벨 15이상 외치기 가능
		if (ch->m_level < 15)
			return;
#endif
	}
#endif // CHAT_BLOCK_USA

#ifdef NO_CHATTING

	switch (chatType)
	{
	case MSG_CHAT_SAY:		// 일반
		{	
			if (ch->m_nflag & CHAT_FLAG_NO_SAY) return;
			tmp.Format("SAY: %s: %s", ch->GetName(), (const char*)chat);
		} break;
	case MSG_CHAT_SHOUT:	// 외치기
		{	
			if (ch->m_nflag & CHAT_FLAG_NO_SHOUT) return;
			tmp.Format("SHOUT: %s: %s", ch->GetName(), (const char*)chat);
		} break;
	case MSG_CHAT_TRADE:	// 매매
		{
			if (ch->m_nflag & CHAT_FLAG_NO_TRADE) return;
			tmp.Format("TRADE: %s: %s", ch->GetName(), (const char*)chat);
		} break;
	case MSG_CHAT_WHISPER:	// 귓말
		{
			if (ch->m_nflag & CHAT_FLAG_NO_WHISPER) return;
			tmp.Format("WHISPER: %s ==> %s: %s", ch->GetName(), (const char*)receiver_name, (const char*)chat);
		} break;
	case MSG_CHAT_PARTY:	// 파티
		{
			if (ch->m_nflag & CHAT_FLAG_NO_PARTY) return;
			tmp.Format("PARTYCHAT: %s: %s", ch->GetName(), (const char*)chat);
		} break;
	case MSG_CHAT_EXPEDITION:	// 원정대 
		{
			if (ch->m_nflag & CHAT_FLAG_NO_EXPED) return;
			tmp.Format("EXPEDCHAT: %s: %s", ch->GetName(), (const char*)chat);
		} break;	
	case MSG_CHAT_GUILD:	// 길드
		{
			if (ch->m_nflag & CHAT_FLAG_NO_GUILD) return;
			tmp.Format("GUILDCHAT: %s: %s", ch->GetName(), (const char*)chat);
		} break;
	case MSG_CHAT_GM:		// GM 일반채팅
		{
			tmp.Format("GM SAY: %s: %s", ch->GetName(), (const char*)chat);
		} break;
	case MSG_CHAT_LORD:		// 성주 공지
		{
			tmp.Format("LORD SAY: %s: %s", ch->GetName(), (const char*)chat);
		} break;
	default:
		return;
	}

	if (strlen(tmp) > 0 && ch->m_bChatMonitor)
	{
		SayMsg(gmmsg, MSG_CHAT_NOTICE, 0, "", "", tmp);
		
		int i;
		for (i = 0; i < 20; i++)
		{
			if (gserver.m_chatmonitor[i])
				SEND_Q(gmmsg, gserver.m_chatmonitor[i]);
		}
		GAMELOG << init("CHAT MONITOR", ch)
			<< tmp
			<< end;
	}

#else
	if (ch->m_bChatMonitor)
	{
		switch (chatType)
		{
		case MSG_CHAT_SAY:			// 일반
			tmp.Format("SAY: %s: %s", ch->GetName(), (const char*)chat);
			break;

		case MSG_CHAT_SHOUT:		// 외치기
			tmp.Format("SHOUT: %s: %s", ch->GetName(), (const char*)chat);
			break;

		case MSG_CHAT_TRADE:		// 매매
			tmp.Format("TRADE: %s: %s", ch->GetName(), (const char*)chat);
			break;

		case MSG_CHAT_WHISPER:		// 귓말
			tmp.Format("WHISPER: %s ==> %s: %s", ch->GetName(), (const char*)receiver_name, (const char*)chat);
			break;

		case MSG_CHAT_PARTY:		// 파티
			tmp.Format("PARTYCHAT: %s: %s", ch->GetName(), (const char*)chat);
			break;
#ifdef EXPEDITION_RAID		
		case MSG_CHAT_EXPEDITION:	// 원정대
			tmp.Format("EXPEDCHAT: %s: %s", ch->GetName(), (const char*)chat);
			break;
#endif //#ifdef EXPEDITION_RAID		
		case MSG_CHAT_GUILD:		// 길드
			tmp.Format("GUILDCHAT: %s: %s", ch->GetName(), (const char*)chat);
			break;

		case MSG_CHAT_GM:			// GM 일반채팅
			tmp.Format("GM SAY: %s: %s", ch->GetName(), (const char*)chat);
			break;

		case MSG_CHAT_LORD:			// 성주 공지
			tmp.Format("LORD SAY: %s: %s", ch->GetName(), (const char*)chat);
			break;

		default:
			return ;
		}

		if (strlen(tmp) > 0)
		{
			SayMsg(gmmsg, MSG_CHAT_NOTICE, 0, "", "", tmp);

			int i;
			for (i = 0; i < 20; i++)
			{
				if (gserver.m_chatmonitor[i])
					SEND_Q(gmmsg, gserver.m_chatmonitor[i]);
			}

			GAMELOG << init("CHAT MONITOR", ch)
					<< tmp
					<< end;
		}
	}
#endif // NO_CHATTING

#ifdef GMTOOL
	if(strlen(tmp) > 0 && ch->m_bGmMonitor)
	{
		switch (chatType)
		{
		case MSG_CHAT_SAY:			// 일반
		case MSG_CHAT_SHOUT:		// 외치기
		case MSG_CHAT_TRADE:		// 매매
		case MSG_CHAT_WHISPER:		// 귓말
		case MSG_CHAT_GM:			// GM 일반채팅
		case MSG_CHAT_LORD:			// 성주 공지
		case MSG_CHAT_GMTOOL:
			{
				MsgrNoticeGmChatMonitorMsg(gmmsg, -1, 1, 1, 0, gserver.m_serverno, gserver.m_subno, -1, tmp, ch->m_index, ch->m_name, chatType);
				if (IS_RUNNING_MSGR) SEND_Q(gmmsg, gserver.m_messenger);
			} break;
		case MSG_CHAT_PARTY:
		case MSG_CHAT_EXPEDITION:	
		case MSG_CHAT_GUILD:
			break;
		default:return;
		}
	}
#endif // GMTOOL

	CNetMsg rmsg;

#ifdef DRATAN_CASTLE
		CDratanCastle  * pCastle = CDratanCastle::CreateInstance();
#endif // DRATAN_CASTLE

	switch (chatType)
	{
	case MSG_CHAT_SAY:			// 일반
		if (ch->IsInvisible())
		{
			SysMsg(rmsg, MSG_SYS_CANTCHAT_INVISIBLE);
			SEND_Q(rmsg, ch->m_desc);
		}
#ifdef RANKER_NOTICE
		else if( ch->m_bRanker )
		{
			SayMsg(rmsg, MSG_CHAT_RANKER, ch->m_index, ch->GetName(), "", chat);
			ch->m_pArea->SendToCell(rmsg, GET_YLAYER(ch), ch->m_cellX, ch->m_cellZ);
		}
#endif
		else
		{
			SayMsg(rmsg, MSG_CHAT_SAY, ch->m_index, ch->GetName(), "", chat);
			ch->m_pArea->SendToCell(rmsg, GET_YLAYER(ch), ch->m_cellX, ch->m_cellZ);
		}
		break;

	case MSG_CHAT_SHOUT:		// 외치기
	case MSG_CHAT_TRADE:		// 매매
#ifdef CHAT_BLOCK_USA 

#if defined (LC_USA) // 미국 요청 사항 외치기는 5초마다
		ch->m_shoutPulse = gserver.m_pulse + 5 * PULSE_REAL_SEC;
#else	
		//독일은 30초
		ch->m_shoutPulse = gserver.m_pulse + 30 * PULSE_REAL_SEC;
#endif

#endif // CHAT_BLOCK_USA
		SayMsg(rmsg, (MSG_CHAT_TYPE)chatType, ch->m_index, ch->GetName(), "", chat);
		ch->m_pArea->SendToCell(rmsg, GET_YLAYER(ch), ch->m_cellX, ch->m_cellZ, CELL_EXT * 2);
		break;

	case MSG_CHAT_WHISPER:		// 귓말
			MsgrWhisperReqMsg(rmsg,
			0,
			gserver.m_serverno,
			gserver.m_subno,
			ch->m_index,
			ch->GetName(),
			receiver_name,
			chat);

			if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
		break;

	case MSG_CHAT_MESSENGER:		// 메신저.
#ifdef ENABLE_MESSENGER
			MsgrFriendChatReqMsg(rmsg,
				0,
				gserver.m_serverno,
				-1,
				ch->m_index,
				ch->GetName(),
				receiver_name,
				chat);
			if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
#endif
		break;

	case MSG_CHAT_PARTY:		// 파티
		{
			if (ch->IsParty())
			{
				if (IS_RUNNING_HELPER)
				{
					CNetMsg rmsg;
					HelperPartyChatMsg(rmsg, ch->m_party->GetBossIndex(), ch->m_index, ch->GetName(), chat);
					SEND_Q(rmsg, gserver.m_helper);
				}
			}
		}
		break;

#ifdef EXPEDITION_RAID		
	case MSG_CHAT_EXPEDITION:		// 원정대
		{
			if (ch->m_Exped)
			{
				if (IS_RUNNING_HELPER)
				{
					CNetMsg rmsg;
					HelperExpedChatReqMsg(rmsg, ch->m_Exped->GetBossIndex(), ch->m_index, ch->GetName(), nGroupTyp,chat);
					SEND_Q(rmsg, gserver.m_helper);
				}
			}
		}
		break;
#endif //EXPEDITION_RAID

	case MSG_CHAT_GUILD:		// 길드
		{
			if (ch->m_guildInfo)
			{
				if (IS_RUNNING_HELPER)
				{
					CNetMsg rmsg;
					HelperGuildChat(rmsg,
						ch->m_guildInfo->guild()->index(),
						ch->m_index,
						ch->GetName(),
						chat);

					SEND_Q(rmsg, gserver.m_helper);
				}
			}
		}
		break;

	case MSG_CHAT_GM:			// GM 일반 채팅
		SayMsg(rmsg, (MSG_CHAT_TYPE)chatType, ch->m_index, ch->GetName(), "", chat);
		ch->m_pArea->SendToCell(rmsg, GET_YLAYER(ch), ch->m_cellX, ch->m_cellZ);
		break;
		
	case MSG_CHAT_LORD:			// 성주 공지
#ifdef ENABLE_WAR_CASTLE
		if (CWarCastle::CanLordChat(ch) && IS_RUNNING_MSGR)
		{
			MsgrChatLordMsg(rmsg,
				ch->m_index,
				ch->GetName(),
				chat);

			SEND_Q(rmsg, gserver.m_messenger);
		}
#endif
		
#ifdef DRATAN_CASTLE
		if(pCastle != NULL && pCastle->CanLordChat(ch) == true && IS_RUNNING_MSGR)
		{
			MsgrChatLordMsg(rmsg,
				ch->m_index,
				ch->GetName(),
				chat);

			GAMELOG << init("CHATLOAD CHATMSG SEND TO MESSENGER:",ch)
					<< " chat"  << delim <<  chat
					<< end;

			SEND_Q(rmsg, gserver.m_messenger);
		}
#endif // DRATAN_CASTLE	
		break;

#ifdef GMTOOL
	case MSG_CHAT_GM_WHISPER:	// 툴에서 날린 귓말
		{
			int server, sub;
			msg >> server
				>> sub;
				
			MsgrGMWhisperReqMsg(rmsg,
			0,
			server,
			sub,
			ch->m_index,
			ch->GetName(),
			receiver_name,
			chat);

			if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
		}
		break;
	case MSG_CHAT_GMTOOL:		// 유저가 지엠에게 날린 귓말
		{
			MsgrGMWhisperReqMsg(rmsg, 0, 1, 1, chat, ch->m_index, ch->m_name, gserver.m_serverno, gserver.m_subno, receiver_name);
			if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);
		}
		break;
#endif // GMTOOL
	default:
		return ;
	}

	// TODO : DEBUG
#ifdef _DEBUG
#ifdef _CONSOLE
	if (strcmp(chat, "admin") == 0)
	{
		if (ch->m_admin == 0)
		{
			ch->m_admin = 10;
			GMWhoAmIMsg(rmsg, ch);
			SEND_Q(rmsg, ch->m_desc);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "SET ADMIN");
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ch->m_admin = 0;
			GMWhoAmIMsg(rmsg, ch);
			SEND_Q(rmsg, ch->m_desc);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "SET NORMAL");
			SEND_Q(rmsg, ch->m_desc);
		}
	}

	else if (strcmp(chat, "pos") == 0)
	{
		chat.Format("x: %.2f, z: %.2f, h: %.2f, y: %d, r:%.2f, att:%d", GET_X(ch), GET_Z(ch), GET_H(ch), GET_YLAYER(ch), GET_R(ch), ch->GetMapAttr());
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		SEND_Q(rmsg, ch->m_desc);
	}

	else if (strcmp(chat, "goto") == 0)
	{
		GoTo(ch, GET_YLAYER(ch), GET_X(ch), GET_Z(ch), GET_H(ch), GET_R(ch));
	}

#endif
#endif
}
