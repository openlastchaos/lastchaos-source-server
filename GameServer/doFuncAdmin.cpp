#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Exp.h"
#include "WarCastle.h"
#include "DBCmd.h"

#ifdef DRATAN_CASTLE
#include "DratanCastle.h"
#endif // DRATAN_CASTLE

///////////////////
// 운영자 명령 관련

typedef void (*GMFunction) (CPC* pc, const char* arg);

void do_GM(CPC* ch, CNetMsg& msg)
{
	static const int countGMCmd = 64;

	static const char* gmCmd[countGMCmd] = {
		GM_CMD_ECHO,
		GM_CMD_GO_ZONE,
		GM_CMD_GOTO,
		GM_CMD_ITEMDROP,
		GM_CMD_COUNT_PC,
		GM_CMD_LOOPCOUNT,
		GM_CMD_IMMORTAL,
		GM_CMD_SHUTDOWN,
		GM_CMD_REBOOT,
		GM_CMD_COUNT_NPC,
		GM_CMD_VISIBLE,
		GM_CMD_GO_PC,
		GM_CMD_GO_NPC,
		GM_CMD_LEVELUP,
		GM_CMD_KICK,
		GM_CMD_COMON,
		GM_CMD_SUMMON,
		GM_CMD_SILENCE,
		GM_CMD_WHERE_AM_I,
		GM_CMD_QUEST_COMPLETE,
		GM_CMD_STATPOINT,
		GM_CMD_SKILLPOINT,
		GM_CMD_SET_REGENSEC,
		GM_CMD_NPC_REGEN,
		GM_CMD_KILL_NPC,
		GM_CMD_EXPUP,
		GM_CMD_BLOODPOINT,
		GM_CMD_ENDGAME,
		GM_CMD_OBSERVE,
		GM_CMD_CHATMONITOR,
		GM_CMD_DOUBLE_EVENT,
		GM_CMD_LATTO_EVENT,
		GM_CMD_DOUBLE_EXP_EVENT,
		GM_CMD_NEWYEAR_EVENT,
		GM_CMD_VALENTINE_EVENT,
		GM_CMD_WHITEDAY_EVENT,
		GM_CMD_LETTER_EVENT,
		GM_CMD_RELOAD_GMIP,
		GM_CMD_SPEEDUP,
		GM_CMD_FAMEUP,
		GM_CMD_EVENT,
		GM_CMD_PD4,
		GM_CMD_KICK_ID,
		GM_CMD_RESURRECTION,
		GM_CMD_RECOMMEND,
		GM_CMD_SETLIMIT,
		GM_CMD_DISGUISE,
		GM_CMD_LOGINSERVER,
		GM_CMD_KICK_CHAR_INDEX,
		GM_GMD_EVENTSHOW,
		GM_CMD_EVENTSETTING,
		GM_CMD_CHANCE_EVENT,
		GM_CMD_GMCHATMONITOR,
		GM_CMD_DOUBLE_ITEM_DROP,
		GM_CMD_DOUBLE_PET_EXP,
		GM_CMD_DOUBLE_ATTACK,
		GM_CMD_EVENT_DROPITEM,
		GM_CMD_KICK_PC_MULTI,
		GM_CMD_SILENCE_PC_MULTI,
		GM_CMD_SET_EXPLIMIIT,
		GM_CMD_DOUBLE_EVENT_AUTO,
		GM_CMD_UPGRADE_EVENT,
		GM_CMD_KICK_GUILDMEMBER,
		GM_CMD_APPLY_SKILL,
	};

	static const int gmLevel[countGMCmd] = {
		GM_LEVEL_ECHO,
		GM_LEVEL_GO_ZONE,
		GM_LEVEL_GOTO,
		GM_LEVEL_ITEMDROP,
		GM_LEVEL_COUNT_PC,
		GM_LEVEL_LOOPCOUNT,
		GM_LEVEL_IMMORTAL,
		GM_LEVEL_SHUTDOWN,
		GM_LEVEL_REBOOT,
		GM_LEVEL_COUNT_NPC,
		GM_LEVEL_VISIBLE,
		GM_LEVEL_GO_PC,
		GM_LEVEL_GO_NPC,
		GM_LEVEL_LEVELUP,
		GM_LEVEL_KICK,
		GM_LEVEL_COMON,
		GM_LEVEL_SUMMON,
		GM_LEVEL_SILENCE,
		GM_LEVEL_WHERE_AM_I,
		GM_LEVEL_QUEST_COMPLETE,
		GM_LEVEL_STATPOINT,
		GM_LEVEL_SKILLPOINT,
		GM_LEVEL_SET_REGENSEC,
		GM_LEVEL_NPC_REGEN,
		GM_LEVEL_KILL_NPC,
		GM_LEVEL_EXPUP,
		GM_LEVEL_BLOODPOINT,
		GM_LEVEL_ENDGAME,
		GM_LEVEL_OBSERVE,
		GM_LEVEL_CHATMONITOR,
		GM_LEVEL_DOUBLE_EVENT,
		GM_LEVEL_LATTO_EVENT,
		GM_LEVEL_DOUBLE_EXP_EVENT,
		GM_LEVEL_NEWYEAR_EVENT,
		GM_LEVEL_VALENTINE_EVENT,
		GM_LEVEL_WHITEDAY_EVENT,
		GM_LEVEL_LETTER_EVENT,
		GM_LEVEL_RELOAD_GMIP,
		GM_LEVEL_SPEEDUP,
		GM_LEVEL_FAMEUP,
		GM_LEVEL_EVENT,
		GM_LEVEL_PD4,
		GM_LEVEL_KICK_ID,
		GM_LEVEL_RESURRECTION,
		GM_LEVEL_RECOMMEND,
		GM_LEVEL_SETLIMIT,
		GM_LEVEL_DISGUISE,
		GM_LEVEL_LOGINSERVER,
		GM_LEVEL_KICK_CHAR_INDEX,
		GM_LEVEL_EVENTSHOW,
		GM_LEVEL_EVENTSETTING,
		GM_LEVEL_CHANCE_EVENT,
		GM_LEVEL_GMCHATMONITOR,
		GM_LEVEL_DOUBLE_ITEM_EVENT,
		GM_LEVEL_DOUBLE_PET_EXP_EVENT,
		GM_LEVEL_DOUBLE_ATTACK_EVENT,
		GM_LEVEL_EVENT_DROPITEM,
		GM_LEVEL_KICK_PC_MULRI,
		GM_LEVEL_SILENCE_PC_MULTI,
		GM_LEVEL_SET_EXPLIMIT,
		GM_LEVEL_DOUBLE_EVENT_AUTO,
		GM_LEVEL_UPGRADE_EVENT,
		GM_LEVEL_KICK_GUILDMEMBER,
		GM_LEVEL_APPLY_SKILL,
	};

	static GMFunction gmFunc[countGMCmd] = {
		do_GMEcho,
		do_GMGoZone,
		do_GMGoto,
		do_GMItemDrop,
		do_GMCountPC,
		do_GMLoopCount,
		do_GMImmortal,
		do_GMShutdown,
		do_GMReboot,
		do_GMCountNPC,
		do_GMVisible,
		do_GMGoPC,
		do_GMGoNPC,
		do_GMLevelUP,
		do_GMKICK,
		do_GMComon,
		do_GMSummon,
		do_GMSilence,
		do_GMWhereAmI,
		do_GMQuestComplete,
		do_GMStatPoint,
		do_GMSkillPoint,
		do_GMSetRegenSec,
		do_GMNPCRegen,
		do_GMKillNPC,
		do_GMExpUp,
		do_GMBloodPoint,
		do_GMEndGame,
		do_GMObserve,
		do_GMChatMonitor,
		do_GMDoubleEvent,
		do_GMLattoEvent,
		do_GMDoubleExpEvent,
		do_GMNewYearEvent,
		do_GMValentineEvent,
		do_GMWhiteDayEvent,
		do_GMLetterEvent,
		do_GMReloadGMIP,
		do_GMSpeedUp,
		do_GMFameUp,
		do_GMEvent,
		do_GMPD4,
		do_GMKickID,
		do_GMResurrection,
		do_GMRecommend,
		do_GMSetLimit,
		do_GMDisguise,
		do_GMLoginServer,
		do_GMKickCharIndex,
		do_GMEventShow,
		do_GMEventSetting,
		do_GMChanceEvent,
		do_GMToolChatMonitor,
		do_GMDoubleItemEvent,
		do_GMDoublePetExpEvent,
		do_GMDoubleAttackEvent,
		do_GMDropItemEvent,
		do_GMKickMulti,
		do_GMSilenceMulti,
		do_GMExpLimit,
		do_GMDoubleEventAuto,
		do_GMUpgradeEvent,
		do_GMKickGuildMember,
		do_GMApplySkill,
	};

	msg.MoveFirst();

	int i;
#ifdef USING_GMIP_TABLE
	bool bGMIP = false;
	int nip[4];
	IPtoi(ch->m_desc->m_host, nip, nip + 1, nip + 2, nip + 3);
	for (i = 0; i < gserver.m_nGMIP; i++)
	{
		if (strncmp(gserver.m_GMIP[i].prefix, ch->m_desc->m_host, strlen(gserver.m_GMIP[i].prefix)) == 0)
		{
			if (gserver.m_GMIP[i].from <= nip[3] && nip[3] <= gserver.m_GMIP[i].to)
			{
				bGMIP = true;
				break;
			}
		}
	}
	if (!bGMIP)
		return ;
#endif

	unsigned char subtype;
	static CLCString buf(MAX_MESSAGE_SIZE);

	msg >> subtype;

	switch (subtype)
	{
	case MSG_GM_WHOAMI:
		{
			CNetMsg gmmsg;
			GMWhoAmIMsg(gmmsg, ch);
			SEND_Q(gmmsg, ch->m_desc);

			GAMELOG << init("GM COMMAND")
					<< "whoami" << delim
					<< ch->m_index << delim
					<< ch->m_name << delim
					<< ch->m_nick << delim
					<< ch->m_pZone->m_index
					<< end;
		}
		break;

	case MSG_GM_COMMAND:
		{
			buf = "";
			msg >> buf;
			
			bool bSkipDebugCommand = false;
#ifdef OTAKU_SYSTEM
#ifdef JP_OTAKU_SYSTEM
			if ((ch->m_etcEvent & OTAKU_SYSTEM) || ch->m_admin >= 2)
			{
				if (strcmp(buf, (const char*)gserver.m_strGMCommandOTAKUSpeedUp) == 0)
				{
					do_OTAKUSpeedUp(ch, NULL);
				}
				else if (strcmp(buf, (const char*)gserver.m_strGMCommandOTAKUImmortal) == 0)
				{
					do_OTAKUImmortal(ch, NULL);
				}
				else if ((ch->m_etcEvent & OTAKU_SYSTEM) && ch->m_admin < 2)
				{
					bSkipDebugCommand = true;
				}
			}
#endif // JP_OTAKU_SYSTEM
#endif // OTAKU_SYSTEM

			const char* p = buf;
			*g_buf2 = '\0';		// argument for command
			p = AnyOneArg(p, g_buf2, true);

			for (i = 0; i < countGMCmd; i++)
			{
				if (strcmp(g_buf2, gmCmd[i]) == 0 && ch->m_admin >= gmLevel[i])
				{
					if (i != 4)
					{
						GAMELOG << init("GM COMMAND")
								<< buf << delim
								<< ch->m_index << delim
								<< ch->m_name << delim
								<< ch->m_nick << delim
								<< end;
					}

					gmFunc[i](ch, p);

					break;
				}
			}

			if(bSkipDebugCommand)
			{
			}
			else if (strcmp(g_buf2, "hpmp") == 0)
			{
				p = AnyOneArg(p, g_buf2);
				int hp = atoi(g_buf2);
				p = AnyOneArg(p, g_buf2);
				int mp = atoi(g_buf2);
				if (hp < 0)
				{
					ch->m_hp -= hp;
					if (ch->m_hp > ch->m_maxHP)
						ch->m_hp = ch->m_maxHP;
				}
				else
				{
					ch->m_hp = ch->m_maxHP * hp / 100;
				}
				if (mp < 0)
				{
					ch->m_mp -= mp;
					if (ch->m_mp > ch->m_maxMP)
						ch->m_mp = ch->m_maxMP;
				}
				else
				{
					ch->m_mp = ch->m_maxMP * mp / 100;
				}
				ch->CalcStatus(true);
			}
#ifdef EVENT_2007_PARENTSDAY
			if( strcmp( g_buf2, "event_parent") == 0 )
			{
				CNetMsg rmsg;
				p = AnyOneArg( p, g_buf2);
				if( strcmp( g_buf2, "add" ) == 0 )
				{
						rmsg.Init( MSG_HELPER_COMMAND );
						rmsg << MSG_HELPER_PARENTSDAY_2007_ADD_POINT
							<< ch->m_index
							<< ch->m_guildInfo->guild()->index()
							<< 1;
						SEND_Q( rmsg, gserver.m_helper );
				}
			}
#endif //EVENT_2007_PARENTSDAY

#ifdef NEW_GUILD
#ifdef LC_JPN
#else
			if( strcmp( g_buf2, "guildpoint" ) == 0 )
			{
				CNetMsg rmsg;
				
				p = AnyOneArg( p, g_buf2 );
				int charindex = atoi( g_buf2 );
				p = AnyOneArg( p, g_buf2 );
				
				int guildpoint = atoi(g_buf2);				
				CPC* tcp = gserver.m_playerList.Find( charindex );
				if( tcp && tcp->m_admin == 0 )
				{
					if( tcp->m_guildInfo )
					{
						int guildindex = ch->m_guildInfo->guild()->index();
						gserver.m_guildlist.findguild( guildindex )->guildPoint( guildpoint );

						if( IS_RUNNING_HELPER )
						{
							HelperSaveGuildPointMsg( rmsg, guildindex, guildpoint );
							SEND_Q(rmsg, gserver.m_helper);
						}
						
					}
				}
//				else
//				{
//					int guildindex = ch->m_guildInfo->guild()->index();
//					gserver.m_guildlist.findguild( guildindex )->guildPoint( charindex );
//				}
			}
#endif // LC_JPN
#endif // NEW_GUILS

#ifdef EVENT_FLOWERTREE_2007
			if( strcmp( g_buf2, "event_ftree") == 0 )
			{
				CNetMsg rmsg;
				p = AnyOneArg( p, g_buf2);
				if( strcmp( g_buf2, "add" ) == 0 )
				{
					p = AnyOneArg( p, g_buf2);
					int point = atoi(g_buf2);
					HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_ADDMYPOINT, ch->m_index );
					rmsg << (unsigned int)point;
					SEND_Q( rmsg, gserver.m_helper );

					ConnEventFlowerTree2007Msg( rmsg, MSG_CONN_EVENT_FLOWERTREE_2007_ADDPOINT, ch->m_index );
					rmsg << (unsigned int)point;
					SEND_Q( rmsg, gserver.m_connector );
				}
				else if( strcmp( g_buf2, "rank" ) == 0 )
				{
					ConnEventFlowerTree2007Msg( rmsg, MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT_RANK, ch->m_index );
					SEND_Q( rmsg, gserver.m_connector );
				}
				else if( strcmp( g_buf2, "ticket" ) == 0 )
				{
					HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_RECV_TICKET, ch->m_index );
					SEND_Q( rmsg, gserver.m_helper );
				}
				else if( strcmp( g_buf2, "give" ) == 0 )
				{
					HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_PRICEITEM, ch->m_index );
					SEND_Q( rmsg, gserver.m_helper );					
				}
				else
				{
					ConnEventFlowerTree2007Msg( rmsg, MSG_CONN_EVENT_FLOWERTREE_2007_TREE_POINT, ch->m_index );
					SEND_Q( rmsg, gserver.m_connector );
				}
			}
#endif //EVENT_FLOWERTREE_2007


#ifdef EVENT_COLLECT_BUG_DROP
			else if( strcmp(g_buf2, "event_collect") == 0 )
			{
				CNetMsg rmsg;
				p = AnyOneArg(p, g_buf2);

				if( strcmp( g_buf2, "set" ) == 0 )
				{
					p = AnyOneArg(p, g_buf2);

					int pct = atoi(g_buf2);

					if( pct < 0 || pct > 50 )
						return;

					gserver.m_CollectBugEventPercent = pct * 10;

					sprintf(g_buf2, "Bug Drop Percent set %d Percent", pct);
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf2);
					SEND_Q(rmsg, ch->m_desc);
				}
				else if( strcmp( g_buf2, "show" ) == 0 )
				{
					sprintf(g_buf2, "Now, Bug Drop Percent is %d Percent", gserver.m_CollectBugEventPercent / 10);
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf2);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
#endif // EVENT_COLLECT_BUG_DROP
#ifdef CASH_ITEM_GIFT
			else if( strcmp(g_buf2, "gift") == 0 )
			{
				p = AnyOneArg(p, g_buf2);
				
				if( strcmp( g_buf2, "send" ) == 0)
				{
					CLCString recvName(MAX_CHAR_NAME_LENGTH+1);
					CLCString strMsg( 50 + 1 );
					int count;
					int idx;
					int ctid;
					p = AnyOneArg(p, g_buf2);
					recvName = g_buf2;
					p = AnyOneArg(p, g_buf2);
					strMsg = g_buf2;
					p = AnyOneArg(p, g_buf2);
					count = atoi(g_buf2);
					
					CNetMsg rmsg;
					rmsg.Init(MSG_EXTEND);
					rmsg << MSG_EX_CASHITEM
						 << (unsigned char) MSG_EX_CASHITEM_GIFT_REQ
						 << recvName
						 << strMsg
						 << count;

					for(int i = 0; i < count; i++ )
					{
						p = AnyOneArg(p, g_buf2);
						idx = atoi(g_buf2);
						rmsg << idx;
						p = AnyOneArg(p, g_buf2);
						ctid = atoi(g_buf2);
						rmsg << ctid;
					}

					do_Extend(ch, rmsg);
				}

				if( strcmp( g_buf2, "recvlist" ) == 0)
				{	
					CNetMsg rmsg;
					rmsg.Init(MSG_EXTEND);
					rmsg << MSG_EX_CASHITEM
						 << (unsigned char) MSG_EX_CASHITEM_GIFT_RECVLIST_REQ;
				
					do_Extend(ch, rmsg);
				}

				if( strcmp( g_buf2, "recv" ) == 0)
				{	
					p = AnyOneArg(p, g_buf2);
					int idx = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					int ctid = atoi(g_buf2);
					CNetMsg rmsg;
					rmsg.Init(MSG_EXTEND);
					rmsg << MSG_EX_CASHITEM
						 << (unsigned char) MSG_EX_CASHITEM_GIFT_RECV_REQ
						 << 1
						 << idx
						 << ctid;
				
					do_Extend(ch, rmsg);
				}

				if( strcmp( g_buf2, "recvtest" ) == 0)
				{	
					p = AnyOneArg(p, g_buf2);
					int cnt = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					int idx = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					int ctid = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					int repeat = atoi(g_buf2);

					CNetMsg rmsg;
					rmsg.Init(MSG_EXTEND);
					rmsg << MSG_EX_CASHITEM
						 << (unsigned char) MSG_EX_CASHITEM_GIFT_RECV_REQ
						 << cnt
						 << idx
						 << ctid;

					while( repeat-- != 0)
					{
						do_Extend(ch, rmsg);
					}
				}
				

				if( strcmp( g_buf2, "sendhistory" ) == 0)
				{	
					p = AnyOneArg(p, g_buf2);
					int y = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					char m = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					char d = atoi(g_buf2);

					CNetMsg rmsg;
					rmsg.Init(MSG_EXTEND);
					rmsg << MSG_EX_CASHITEM
						 << (unsigned char) MSG_EX_CASHITEM_GIFT_SENDHISTORY_REQ
						 << y
						 << m
						 << d;
				
					do_Extend(ch, rmsg);
				}

				if( strcmp( g_buf2, "recvhistory" ) == 0)
				{	
					p = AnyOneArg(p, g_buf2);
					int y = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					char m = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					char d = atoi(g_buf2);

					CNetMsg rmsg;
					rmsg.Init(MSG_EXTEND);
					rmsg << MSG_EX_CASHITEM
						 << (unsigned char) MSG_EX_CASHITEM_GIFT_RECVHISTORY_REQ
						 << y
						 << m
						 << d;
				
					do_Extend(ch, rmsg);
				}
			}
#endif
#ifdef CREATE_EVENT
			else if(strcmp(g_buf2, "create_event_reload") == 0)
			{
				gserver.LoadCreateEvent();
			}
#endif // CREATE_EVENT
#ifdef MESSENGER_NEW
			else if( strcmp(g_buf2, "messenger") == 0 )
			{
				p = AnyOneArg(p, g_buf2);
				
				if( strcmp( g_buf2, "invite" ) == 0)
				{
					p = AnyOneArg(p, g_buf2);
					int makeCharIndex = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					int chatIndex = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					CLCString charName = g_buf2;

					CNetMsg rmsg;
					rmsg.Init(MSG_EXTEND);
					rmsg << MSG_EX_MESSENGER
						 << (unsigned char) MSG_EX_MESSENGER_INVITE
						 << makeCharIndex
						 << chatIndex
						 << charName;

					do_Extend(ch, rmsg);
				}
				if( strcmp( g_buf2, "out" ) == 0)
				{
					p = AnyOneArg(p, g_buf2);
					int makeCharIndex = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					int chatIndex = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					CLCString charName = g_buf2;

					CNetMsg rmsg;
					rmsg.Init(MSG_EXTEND);
					rmsg << MSG_EX_MESSENGER
						 << (unsigned char) MSG_EX_MESSENGER_OUT
						 << makeCharIndex
						 << chatIndex
						 << charName;

					do_Extend(ch, rmsg);
				}
				else if (strcmp( g_buf2, "chat" ) == 0 )
				{
					p = AnyOneArg(p, g_buf2);
					int makeCharIndex = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					int chatIndex = atoi(g_buf2);
					p = AnyOneArg(p, g_buf2);
					CLCString charName = g_buf2;

					CNetMsg rmsg;
					rmsg.Init(MSG_EXTEND);
					rmsg << MSG_EX_MESSENGER
						 << (unsigned char) MSG_EX_MESSENGER_CHAT
						 << makeCharIndex
						 << chatIndex
						 //<< ch->GetName()
						 << charName;

					do_Extend(ch, rmsg);
				}
			}
			else if( strcmp(g_buf2, "group") == 0 )
			{
				p = AnyOneArg(p, g_buf2);
				
				if( strcmp( g_buf2, "add" ) == 0)
				{
					/*
					MSG_EX_MESSENGER_GROUP_ADD,	// 친구 그룹 추가	: groupName(str)
								// GS->C			: gIndex(n) groupName(str)  : gIndex 가 -1이면 중복된 이름 
					MSG_EX_MESSENGER_GROUP_DEL,	// 친구 그룹 삭제	: gIndex(n)
								// GS->C			: gIndex(n) : gIndex 가 -1이면 존재하지 않는 그룹*/
					p = AnyOneArg(p, g_buf2);

					CNetMsg rmsg;
					rmsg.Init(MSG_EXTEND);
					rmsg << MSG_EX_MESSENGER
						 << (unsigned char) MSG_EX_MESSENGER_GROUP_ADD
						 << g_buf2;

					do_Extend(ch, rmsg);
				}
				else if( strcmp( g_buf2, "del" ) == 0)
				{
					p = AnyOneArg(p, g_buf2);
					int gIndex = atoi(g_buf2);
					CNetMsg rmsg;
					rmsg.Init(MSG_EXTEND);
					rmsg << MSG_EX_MESSENGER
						 << (unsigned char) MSG_EX_MESSENGER_GROUP_DEL
						 << gIndex;


					do_Extend(ch, rmsg);
				}
			}
			else if( strcmp(g_buf2, "block") == 0 )
			{
				p = AnyOneArg(p, g_buf2);
				CNetMsg rmsg;
				rmsg.Init(MSG_EXTEND);
				rmsg << MSG_EX_MESSENGER
					 << (unsigned char) MSG_EX_MESSENGER_BLOCK
					 << g_buf2;

				do_Extend(ch, rmsg);
			}
			else if( strcmp(g_buf2, "release") == 0 )
			{
				CNetMsg rmsg;
				rmsg.Init(MSG_EXTEND);
				rmsg << MSG_EX_MESSENGER
					 << (unsigned char) MSG_EX_MESSENGER_RELEASE_BLOCK
					 << atoi(g_buf2);

				do_Extend(ch, rmsg);
			}
#endif
#ifdef CASH_MOONSTONE
			else if( strcmp(g_buf2, "cashmoon") == 0 )
			{
				//MSG_ITEM_REMOVE_ADD_REQ,	// 아이템을 삭제하고 지급요청: tab_idx(c) row_idx(c) col_idx(c) item_idx(n)
				//MSG_ITEM_REMOVE_ADD_REP,	//							   result(uc) grade(c)
				char row;
				char col;
				int itemindex;

				p = AnyOneArg(p, g_buf2);
				row = atoi(g_buf2);
				p = AnyOneArg(p, g_buf2);
				col = atoi(g_buf2);
				p = AnyOneArg(p, g_buf2);
				itemindex = atoi(g_buf2);

				CNetMsg rmsg;
				rmsg.Init(MSG_EXTEND);
				rmsg << MSG_EX_CASHITEM
				 	 << (unsigned char) MSG_EX_CASHITEM_MOONSTONE_START
					 << (char) 0
					 << row
					 << col
					 << itemindex;

				do_Extend(ch, rmsg);
			}
#endif
#ifdef EVENT_PCBANG
			else if( strcmp(g_buf2, "pcbangtime") == 0)
			{
				p = AnyOneArg(p, g_buf, true);
				TrimString(g_buf);

				CNetMsg msg;
				msg.Init(MSG_CONN_REQ);
				msg << (unsigned char) MSG_CONN_PCBANG_ITEM_GIVE
					<< ch->m_desc->m_index
					<< atoi(g_buf);

				if(IS_RUNNING_CONN)
				{
					SEND_Q(msg, gserver.m_connector);
				}
			}
			else if( strcmp(g_buf2, "pcbangexchange") == 0)
			{
				CNetMsg msg;
				msg.Init(MSG_EVENT);
				msg << (unsigned char) MSG_EVENT_PCBANG;

				do_Event(ch, msg);
			}
#endif

#ifdef NEW_SERIAL_PACKAGE_EVENT
			else if( strcmp( g_buf2, "new_coupon") == 0 )
			{
				p = AnyOneArg( p, g_buf, true );
				TrimString( g_buf );

				CNetMsg msg;
				msg.Init( MSG_EVENT );
				msg << (unsigned char) MSG_EVENT_PACKAGE_ITEM_GIVE
					<< g_buf;
				do_Event( ch, msg );
			}
#endif // NEW_SERIAL_PACKAGE_EVENT

#ifdef EVENT_PACKAGE_ITEM
			else if( strcmp(g_buf2, "coupon") == 0 )
			{
				p = AnyOneArg(p, g_buf, true);
				TrimString(g_buf);

				CNetMsg msg;
				msg.Init(MSG_EVENT);
				msg << (unsigned char) MSG_EVENT_PACKAGE_ITEM_GIVE
					<< g_buf;

				do_Event(ch, msg);
			}
#endif//#ifdef EVENT_PACKAGE_ITEM
#ifdef EVENT_PROMOTION_SITE
			else if( strcmp(g_buf2, "promotion") == 0)
			{
				CDBCmd cmd;
				cmd.Init(&gserver.m_dbchar);

				sprintf(g_buf, "delete from t_proSite where a_user_idx = %d ", ch->m_desc->m_index );
				cmd.SetQuery(g_buf);

				cmd.Update();
			}
#endif//#ifdef EVENT_PROMOTION_SITE
#ifdef EVENT_SAKURA
			else if( strcmp(g_buf2, "sakura") == 0)
			{
				msg.Init(MSG_EVENT);
				msg << (unsigned char) MSG_EVENT_SAKURABUD
					<< (unsigned char) 2;

				do_Event(ch, msg);
			}
#endif
#ifdef ENABLE_WAR_CASTLE
			else if (strcmp(g_buf2, "setwartime") == 0)
			{
				struct tm t;
				time_t t2;
				time(&t2);
				t = *localtime(&t2);
				p = AnyOneArg(p, g_buf);		t.tm_year = atoi(g_buf) - 1900;

				if( t.tm_year < 0 )
				{
#ifdef TLD_WAR_TEST
					meracCastle.RemoveCastleNPC();
					meracCastle.m_nextWarTime = 1165849200;
					meracCastle.m_bNoticeWarTime = true;
					meracCastle.m_noticeRemain = 9999;
					gserver.m_warnotice = false;
#endif // #ifdef TLD_WAR_TEST
					return;
				}

				p = AnyOneArg(p, g_buf);		t.tm_mon = atoi(g_buf) -1;
				p = AnyOneArg(p, g_buf);		t.tm_mday = atoi(g_buf);
				p = AnyOneArg(p, g_buf);		t.tm_hour = atoi(g_buf);
				p = AnyOneArg(p, g_buf);		t.tm_min = atoi(g_buf);
				p = AnyOneArg(p, g_buf);		t.tm_sec = 0;
				time_t t3 = mktime(&t);
				if (t3 < t2)
					return ;
				meracCastle.m_nextWarTime = mktime(&t);
//				meracCastle.m_bNoticeWarTime = false;
				meracCastle.m_bNoticeWarTime = true;
				meracCastle.m_noticeRemain = 9999;
#ifdef TLD_WAR_TEST
				meracCastle.RegenCastleNPC();
				gserver.m_warnotice = true;
#endif // #ifdef TLD_WAR_TEST				
			}
#ifdef DRATAN_CASTLE
			else if (strcmp(g_buf2, "setwartime2") == 0)
			{
				struct tm t;
				time_t t2;
				time(&t2);
				t = *localtime(&t2);
				p = AnyOneArg(p, g_buf);		t.tm_year = atoi(g_buf) - 1900;

				if( t.tm_year < 0 )
				{
					return;
				}

				p = AnyOneArg(p, g_buf);		t.tm_mon = atoi(g_buf) -1;
				p = AnyOneArg(p, g_buf);		t.tm_mday = atoi(g_buf);
				p = AnyOneArg(p, g_buf);		t.tm_hour = atoi(g_buf);
				p = AnyOneArg(p, g_buf);		t.tm_min = atoi(g_buf);
				p = AnyOneArg(p, g_buf);		t.tm_sec = 0;
				time_t t3 = mktime(&t);
				if (t3 < t2)
					return ;


				CDratanCastle * pCastle = CDratanCastle::CreateInstance();

				if (pCastle)
				{
					pCastle->m_nextWarTime = mktime(&t);
					pCastle->m_bNoticeWarTime = true;
					pCastle->m_noticeRemain = 9999;
				}
			}
			else if (strcmp(g_buf2, "setwartime2end") == 0)
			{
				CDratanCastle * pCastle = CDratanCastle::CreateInstance();

				if (pCastle)
				{
					pCastle->m_bLoop = false;
				}
			}
#endif // DRATAN_CASTLE
#ifdef EXTREME_CUBE
			else if(strcmp(g_buf2, "guildcubestart") == 0)
			{
				if(gserver.m_extremeCube.IsGuildCubeTime())
				{
					GAMELOG << init( "GM COMMAND", ch) << g_buf2 << delim << "ERROR" << delim 
						    << "GUILDCUBE-ING" << end;
					return ;
				}

				struct tm t;
				const char* p2;
				p2 = p;
				time_t t2;
				time(&t2);
				t = *localtime(&t2);
				p = AnyOneArg(p, g_buf);		t.tm_year = atoi(g_buf) - 1900;

				if( t.tm_year < 0 )
				{
					GAMELOG << init( "GM COMMAND", ch) << g_buf2 << delim << "ERROR" << delim 
						    << "TIME" << delim << t.tm_year << end;
					return;
				}

				p = AnyOneArg(p, g_buf);		t.tm_mon = atoi(g_buf) -1;
				p = AnyOneArg(p, g_buf);		t.tm_mday = atoi(g_buf);
				p = AnyOneArg(p, g_buf);		t.tm_hour = atoi(g_buf);
				p = AnyOneArg(p, g_buf);		t.tm_min = atoi(g_buf);
				p = AnyOneArg(p, g_buf);		t.tm_sec = atoi(g_buf);
				time_t t3 = mktime(&t);
				if (t3 < t2)
				{
					GAMELOG << init( "GM COMMAND", ch) << g_buf2 << delim << "ERROR" << delim 
						    << (int)t3 << delim << (int)t2 << end;
					return ;
				}
				gserver.m_extremeCube.m_nextGuildCubeTime = t3;

				GAMELOG << init( "GM COMMAND", ch) << g_buf2 << delim << p2 << end;


//				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "TIMESET: ");
//				SEND_Q(rmsg, ch->m_desc);
			}
			else if(strcmp(g_buf2, "guildcubeend") == 0)
			{
				if(gserver.m_extremeCube.IsGuildCubeTime())
				{
					gserver.m_extremeCube.EndGuildCube();
					GAMELOG << init( "GM COMMAND", ch) << g_buf2 << end;

				}
				else
				{
					GAMELOG << init( "GM COMMAND", ch) << g_buf2 << "ERROR" << end;
				}
			}
			else if(strcmp(g_buf2, "guildcubestate") == 0)
			{
				if(gserver.m_extremeCube.IsExtremeCubeServer())
				{
					FILE* fpCube = fopen(".CubeState", "w");
					if(fpCube)
					{
						int i;
						CCubeSpace* cube;
						char buf[1024] = "";
						struct tm ti = NOW();
						
						IntCat0(buf, ti.tm_year + 1900, 4, false);
						strcat(buf, "/");
						IntCat0(buf, ti.tm_mon + 1, 2, false);
						strcat(buf, "/");
						IntCat0(buf, ti.tm_mday, 2, false);
						strcat(buf, " ");
						IntCat0(buf, ti.tm_hour, 2, false);
						strcat(buf, ":");
						IntCat0(buf, ti.tm_min, 2, false);
						strcat(buf, ":");
						IntCat0(buf, ti.tm_sec, 2, false);
						
						fprintf(fpCube, "%s\n", buf );
						
						for(i = 0; i < MAX_CUBESPACE; ++i)
						{
							cube = gserver.m_extremeCube.GetExtremeCube(i);
							if(cube)
								fprintf(fpCube, "%d: %d\n", i, (int)cube->GetState());
						}
						
						fclose(fpCube);
						fpCube = NULL;
					}
				}
			}
			else if(strcmp(g_buf2, "guildcubetimecheck") == 0)
			{
				tm* ti;
				char buf[1024] = "";
				ti = localtime(&(gserver.m_extremeCube.m_nextGuildCubeTime));
				
				strcat(buf, "guildcubetime: ");
				IntCat0(buf, (*ti).tm_year + 1900, 4, false);
				strcat(buf, "/");
				IntCat0(buf, (*ti).tm_mon + 1, 2, false);
				strcat(buf, "/");
				IntCat0(buf, (*ti).tm_mday, 2, false);
				strcat(buf, " ");
				IntCat0(buf, (*ti).tm_hour, 2, false);
				strcat(buf, ":");
				IntCat0(buf, (*ti).tm_min, 2, false);
				strcat(buf, ":");
				IntCat0(buf, (*ti).tm_sec, 2, false);

				CNetMsg rmsg;
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", buf);
				SEND_Q(rmsg, ch->m_desc);
			}
#endif // EXTREME_CUBE
			else if (strcmp(g_buf2, "setowner") == 0)
			{
				g_buf[0] = '\0';
				p = AnyOneArg(p, g_buf);
				CPC* pc = gserver.m_playerList.Find(g_buf, false );
				if (pc)
				{
					if (pc->m_guildInfo)
					{
						if (pc->m_guildInfo->guild())
						{
							CWarCastle* castle = CWarCastle::GetCastleObject(pc->m_pZone->m_index);
							if (castle)
							{
								castle->SetOwner(pc->m_guildInfo->guild());
								CWarCastle::CheckJoinAll(pc);
							}
						}
					}
				}
			}
			else if (strcmp(g_buf2, "lordchat") == 0)
			{
				CNetMsg rmsg;
				SayMsg(rmsg, MSG_CHAT_LORD, ch->m_index, ch->GetName() , "", p);
				do_Chat(ch, rmsg);
			}
#endif // #ifdef ENABLE_WAR_CASTLE
			
#ifdef ATTACK_PET
			else if (strcmp(g_buf2, "create_pet") == 0)
			{
				p = AnyOneArg( p, g_buf2);
				int nPetProtoIndex = atoi(g_buf2);
				if( nPetProtoIndex == 3 )
				{
					p = AnyOneArg( p, g_buf2);
					int key = atoi(g_buf2);
					if( key != 79 )
						return;
				}
				CNetMsg rmsg;
				HelperAttackPetMsg( rmsg, MSG_HELPER_APET_CREATE_REQ, ch->m_index );
				rmsg << nPetProtoIndex ;
				SEND_Q( rmsg, gserver.m_helper );
			}
			else if (strcmp(g_buf2, "apet_stat") == 0)
			{
				CNetMsg rmsg;

				int remain, str, con, dex, Int;
				p = AnyOneArg( p, g_buf2);
				remain = atoi(g_buf2);
				p = AnyOneArg( p, g_buf2);
				str = atoi(g_buf2);
				p = AnyOneArg( p, g_buf2);
				con = atoi(g_buf2);
				p = AnyOneArg( p, g_buf2);
				dex = atoi(g_buf2);
				p = AnyOneArg( p, g_buf2);
				Int = atoi(g_buf2);

				CAPet* apet = ch->GetAPet();
				if( apet )
				{
					apet->m_nRemainStat = remain;
					apet->UpdateStatPlus( str, dex, Int, con );
					ExAPetStatusMsg( rmsg, apet );
					SEND_Q( rmsg, ch->m_desc );
				}
			}
			else if (strcmp(g_buf2, "apet_hpmp") == 0)
			{
				CNetMsg rmsg;

				int hp, mp;
				p = AnyOneArg( p, g_buf2);
				hp = atoi(g_buf2);
				p = AnyOneArg( p, g_buf2);
				mp = atoi(g_buf2);

				CAPet* apet = ch->GetAPet();
				if( apet )
				{
					apet->m_hp = apet->m_maxHP * hp / 100;
					apet->m_mp = apet->m_maxMP * mp / 100;
					ExAPetStatusMsg( rmsg, apet );
					SEND_Q( rmsg, ch->m_desc );
				}
			}
			else if (strcmp(g_buf2, "apet_exp") == 0)
			{
				CNetMsg rmsg;

				int exp;
				p = AnyOneArg( p, g_buf2);
				exp = atoi(g_buf2);
				
				if( exp <0 || exp>100 )
					return;

				CAPet* apet = ch->GetAPet();
				if( apet )
				{
					apet->m_exp = apet->GetNeedExp() * exp / 100;
					ExAPetStatusMsg( rmsg, apet );
					SEND_Q( rmsg, ch->m_desc );
				}				
			}
#endif //ATTACK_PET

#ifdef ENABLE_PET
			else if (strcmp(g_buf2, "petlevelup") == 0)
			{
#ifdef LC_TLD
				if( ch->m_admin < 9  )
					return;
#endif // LC_TLD
				g_buf[0] = '\0';
				p = AnyOneArg(p, g_buf);
				int level = atoi(g_buf);
				if (level < 1 || level > 60)
					return ;
				CPet* pet = ch->GetPet();
				if (pet)
				{
					pet->m_exp = 0;
					pet->m_abilityPoint += level - pet->m_level;
					pet->m_level = level;

					pet->LevelUp();

					CNetMsg rmsg;
					ExPetStatusMsg(rmsg, pet);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
			else if (strcmp(g_buf2, "petdeadtime") == 0)
			{
				g_buf[0] = '\0';
				p = AnyOneArg(p, g_buf);
				int remain = atoi(g_buf);
				if (remain < 1)
					return ;
				CItem* item = ch->m_invenNormal.GetItem(0, 0);
				if (item && item->IsPet())
				{
					CPet* pet = ch->GetPet(item->m_plus);
					if (pet)
					{
						int cur = pet->GetRemainRebirthTime();
						if (cur < remain)
							return ;
						pet->SetRemainRebirthTime(remain);
						CNetMsg rmsg;
						ExPetStatusMsg(rmsg, pet);
						SEND_Q(rmsg, ch->m_desc);
					}
				}
			}
			else if (strcmp(g_buf2, "petreset") == 0)
			{
				CNetMsg rmsg;
				rmsg.Init(MSG_EXTEND);
				rmsg << MSG_EX_PET_RESET_SKILL;
				do_Extend(ch, rmsg);
			}
			else if (strcmp(g_buf2, "petexpup") == 0)
			{
				double exppercent = atof(p);
				if (exppercent < 0 || exppercent > 100)
					return ;
				CPet* pPet = ch->GetPet();
				if (pPet)
				{
					LONGLONG exp = pPet->GetNeedExp();
					exp = (LONGLONG)(exp * exppercent / 100);
					pPet->m_exp = exp;
					CNetMsg rmsg;
					ExPetStatusMsg(rmsg, pPet);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
			else if (strcmp(g_buf2, "petmountreset") == 0)
			{
				CNetMsg rmsg;
				CItem* pItem = ch->m_invenNormal.GetItem(0, 1);
				if (pItem)
				{
					ItemUseMsg(rmsg, ch, INVENTORY_NORMAL, 0, 0, 1519, pItem->m_index);
					do_Item(ch, rmsg);
				}
			}
#ifdef PET_NAME_CHANGE
			else if (strcmp( g_buf2, "petchangename") == 0 )
			{				
				p = AnyOneArg( p, g_buf, true );
				TrimString( g_buf );

				CNetMsg msg;

				msg.Init( MSG_EXTEND );
				msg << MSG_EX_PET_CHANGE_NAME
					<< g_buf;
				do_Extend( ch, msg );
			}
#endif // PET_NAME_CHANGE
#endif // #ifdef ENABLE_PET
#ifdef EVENT_OPEN_BETA_TLD
			// TODO : TEST
			else if( strcmp(g_buf2, "openbeta_event") == 0 )
			{
				CNetMsg rmsg;
				int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group

				p = AnyOneArg(p, g_buf, true);
				TrimString(g_buf);
				if (strcmp(g_buf, "all") == 0)
					target = -1;
				else if (strcmp(g_buf, "cur") == 0)
					target = gserver.m_serverno;
				else
				{
					// all, cur 모두 아니면 숫자여야함
					char* p1 = g_buf;
					while (*p1)
					{
						if (!isdigit(*p1))
							goto INVALID_FORMAT;
						p1++;
					}
					target = atoi(g_buf);
					if (target < 1)
						goto INVALID_FORMAT;
				}

				// 명령 정하기
				if (!p || !(*p))
					goto INVALID_FORMAT;
				p = AnyOneArg(p, g_buf, true);
				TrimString(g_buf);
				// 0 : 모두 드롭 1: 배양토 2: 레드 3: 푸른 4: 황금 5: 배양,레드 6:레드,푸른 7:푸른,골드 8: 모두 안 드롭
				if (strcmp(g_buf, "start") == 0)
				{
					p = AnyOneArg(p, g_buf, true);
					TrimString(g_buf);

					if( atoi(g_buf) < 0 || atoi(g_buf) > 8  )
						goto INVALID_FORMAT;

					MsgrEventReqMsg(rmsg, MSG_MSGR_OPEN_BETA_EVENT_REQ, 0, target, -1, -1, ch->m_index, 0, atoi(g_buf) );
				}
				else if (strcmp(g_buf, "ing") == 0)
				{
					MsgrEventReqMsg(rmsg, MSG_MSGR_OPEN_BETA_EVENT_REQ, 0, target, -1, -1, ch->m_index, 1, -1 );
				}
				else if (strcmp(g_buf, "stop") == 0)
				{
					MsgrEventReqMsg(rmsg, MSG_MSGR_OPEN_BETA_EVENT_REQ, 0, target, -1, -1, ch->m_index, 2, -1 );
				}
				else
					goto INVALID_FORMAT;

				if (IS_RUNNING_MSGR)
				{
					SEND_Q(rmsg, gserver.m_messenger);
				}
				else
				{
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
					SEND_Q(rmsg, ch->m_desc);
				}

				return ;

			INVALID_FORMAT:
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
				SEND_Q(rmsg, ch->m_desc);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: openbeta_event <server-group no | all | cur> <start | ing | stop> < 0|1|2|3|4|5|6");
				SEND_Q(rmsg, ch->m_desc);
			}
#endif
#ifdef EVENT_FLOWER
			else if( strcmp(g_buf2, "flower_event") == 0 )
			{
				CNetMsg rmsg;
				int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group

				p = AnyOneArg(p, g_buf, true);
				TrimString(g_buf);
				if (strcmp(g_buf, "all") == 0)
					target = -1;
				else if (strcmp(g_buf, "cur") == 0)
					target = gserver.m_serverno;
				else
				{
					// all, cur 모두 아니면 숫자여야함
					char* p1 = g_buf;
					while (*p1)
					{
						if (!isdigit(*p1))
							goto INVALID_FORMAT_FLOWER;
						p1++;
					}
					target = atoi(g_buf);
					if (target < 1)
						goto INVALID_FORMAT_FLOWER;
				}

				// 명령 정하기
				if (!p || !(*p))
					goto INVALID_FORMAT_FLOWER;
				p = AnyOneArg(p, g_buf, true);
				TrimString(g_buf);
				// 0 : 모두 드롭 1: 배양토 2: 레드 3: 푸른 4: 황금 5: 배양,레드 6:레드,푸른 7:푸른,골드 8: 모두 안 드롭
				if (strcmp(g_buf, "start") == 0)
				{
					p = AnyOneArg(p, g_buf, true);
					TrimString(g_buf);

					if( atoi(g_buf) < 0 || atoi(g_buf) > 8  )
						goto INVALID_FORMAT_FLOWER;

					MsgrEventReqMsg(rmsg, MSG_MSGR_FLOWER_EVENT_REQ, 0, target, -1, -1, ch->m_index, 0, atoi(g_buf) );
				}
				else if (strcmp(g_buf, "ing") == 0)
				{
					MsgrEventReqMsg(rmsg, MSG_MSGR_FLOWER_EVENT_REQ, 0, target, -1, -1, ch->m_index, 1, -1 );
				}
				else if (strcmp(g_buf, "stop") == 0)
				{
					MsgrEventReqMsg(rmsg, MSG_MSGR_FLOWER_EVENT_REQ, 0, target, -1, -1, ch->m_index, 2, -1 );
				}
				else
					goto INVALID_FORMAT_FLOWER;

				if (IS_RUNNING_MSGR)
				{
					SEND_Q(rmsg, gserver.m_messenger);
				}
				else
				{
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
					SEND_Q(rmsg, ch->m_desc);
				}

				return ;

			INVALID_FORMAT_FLOWER:
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
				SEND_Q(rmsg, ch->m_desc);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: flower_event <server-group no | all | cur> <start | ing | stop> < 0|1|2|3|4|5|6");
				SEND_Q(rmsg, ch->m_desc);
			}
#endif
#ifdef MARGADUM_PVP
			else if( strcmp(g_buf2, "margadum_pvp") == 0 )
			{
				CNetMsg rmsg;
				int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group

				p = AnyOneArg(p, g_buf, true);
				TrimString(g_buf);
				if (strcmp(g_buf, "all") == 0)
					target = -1;
				else if (strcmp(g_buf, "cur") == 0)
					target = gserver.m_serverno;
				else
				{
					// all, cur 모두 아니면 숫자여야함
					char* p1 = g_buf;
					while (*p1)
					{
						if (!isdigit(*p1))
							goto INVALID_FORMAT_MARGADUM_PVP;
						p1++;
					}
					target = atoi(g_buf);
					if (target < 1)
						goto INVALID_FORMAT_MARGADUM_PVP;
				}

				// 명령 정하기
				if (!p || !(*p))
					goto INVALID_FORMAT_MARGADUM_PVP;
				p = AnyOneArg(p, g_buf, true);
				TrimString(g_buf);
				
				//0: free 1:16~25, 2: 26~35, 3: 36~
				if (strcmp(g_buf, "start") == 0)
				{
					p = AnyOneArg(p, g_buf, true);
					TrimString(g_buf);

					if( atoi(g_buf) < 0 || atoi(g_buf) > 3  )
						goto INVALID_FORMAT_MARGADUM_PVP;

					MsgrEventReqMsg(rmsg, MSG_MSGR_MARGADUM_PVP_REQ, 0, target, -1, -1, ch->m_index, 0, atoi(g_buf) );
				}
				else if (strcmp(g_buf, "ing") == 0)
				{
					MsgrEventReqMsg(rmsg, MSG_MSGR_MARGADUM_PVP_REQ, 0, target, -1, -1, ch->m_index, 1, -1 );
				}
				else if (strcmp(g_buf, "stop") == 0)
				{
					MsgrEventReqMsg(rmsg, MSG_MSGR_MARGADUM_PVP_REQ, 0, target, -1, -1, ch->m_index, 2, -1 );
				}
				else
					goto INVALID_FORMAT_MARGADUM_PVP;

				if (IS_RUNNING_MSGR)
				{
					SEND_Q(rmsg, gserver.m_messenger);
				}
				else
				{
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
					SEND_Q(rmsg, ch->m_desc);
				}

				return ;

			INVALID_FORMAT_MARGADUM_PVP:
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
				SEND_Q(rmsg, ch->m_desc);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: margadum_pvp <server-group no | all | cur> <start | ing | stop> < 0|1|2|3");
				SEND_Q(rmsg, ch->m_desc);
			}
#endif

#ifdef _DEBUG
			else if (strcmp(g_buf2, "ap") == 0)
			{
				if (ch->m_wearing[WEARING_WEAPON])
				{
					CNetMsg rmsg;
					ch->m_wearing[WEARING_WEAPON]->m_plus = atoi(p);
					ch->CalcStatus(true);
					ItemUpdateMsg(rmsg, ch->m_wearing[WEARING_WEAPON], 0);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
			else if (strcmp(g_buf2, "dp") == 0)
			{
				if (ch->m_wearing[WEARING_ARMOR_UP])
				{
					CNetMsg rmsg;
					ch->m_wearing[WEARING_ARMOR_UP]->m_plus = atoi(p);
					ch->CalcStatus(true);
					ItemUpdateMsg(rmsg, ch->m_wearing[WEARING_ARMOR_UP], 0);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
#ifdef EVENT_CHUSEOK_2006
			else if (strcmp(g_buf2, "eventtest1") == 0)
			{
				CNetMsg rmsg;
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE);
				do_Event(ch, rmsg);
			}
			else if (strcmp(g_buf2, "eventtest2") == 0)
			{
				CNetMsg rmsg;
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE);
				do_Event(ch, rmsg);
			}
#endif // EVENT_CHUSEOK_2006
			else if (strcmp(g_buf2, "itemuse") == 0)
			{
				CItem* pItem = ch->m_invenNormal.GetItem(0, 0);
				CItem* pItem2 = ch->m_invenNormal.GetItem(0, 1);
				if (pItem && pItem2)
				{
					CNetMsg rmsg;
					ItemUseMsg(rmsg, ch, INVENTORY_NORMAL, 0, 0, pItem->m_index, pItem2->m_index);
					do_Item(ch, rmsg);
				}
			}
#endif // __DEBUG

#ifdef LC_KOR
			// TODO : TEST
			else if (strcmp(g_buf2, "pkcount") == 0)
			{
				p = AnyOneArg(p, g_buf2, true);
				if (strlen(g_buf2) < 1)
					return ;
				int pkcount = atoi(g_buf2);
				if (pkcount < -110)
					pkcount = -110;
				if (pkcount > 110)
					pkcount = 110;
				ch->m_pkCount = pkcount;
				CNetMsg rmsg;
				CharStatusMsg(rmsg, ch, 0);
				ch->m_pArea->SendToCell(rmsg, ch, false);
				ch->m_bChangeStatus = true;
			}
			else if (strcmp(g_buf2, "pkpenalty") == 0)
			{
				p = AnyOneArg(p, g_buf2, true);
				if (strlen(g_buf2) < 1)
					return ;
				int pkpenalty = atoi(g_buf2);
				if (pkpenalty < -155)
					pkpenalty = -155;
				if (pkpenalty > 155)
					pkpenalty = 155;
				ch->m_pkPenalty = pkpenalty;
				CNetMsg rmsg;
				CharStatusMsg(rmsg, ch, 0);
				ch->m_pArea->SendToCell(rmsg, ch, false);
				ch->m_bChangeStatus = true;
			}
			else if (strcmp(g_buf2, "showhp") == 0)
			{
				CNetMsg rmsg;
				char	cOn = 0;
				p = AnyOneArg(p, g_buf, true);
				TrimString(g_buf);

				if ( strcmp(g_buf, "on") == 0 )
					cOn = 1;
				else if ( strcmp(g_buf, "off") == 0 )
					cOn = 0;
				rmsg.Init(MSG_PK);
				rmsg << (unsigned char)MSG_PK_TARGET_SHOWHP
					 << (char)cOn ;
				SEND_Q( rmsg, ch->m_desc );
			}
			else if (strcmp(g_buf2, "gopos") == 0)
			{
				p = AnyOneArg(p, g_buf2);
				int pos = atoi(g_buf2);
				CNetMsg rmsg;
				rmsg.Init(MSG_WARP);
				rmsg << (unsigned char)MSG_WARP_TELEPORT
					 << pos;
				do_Warp(ch, rmsg);
			}
			else if( strcmp(g_buf2, "pd_npc") == 0)
			{
				//MSG_NPC_REGEN,				// NPC 리젠 관련	: index(n) dbIndex(n) x(f) z(f) h(f) r(f) yLayer(int) entityindex(n)
				msg.Init(MSG_NPC_REGEN);
				msg << -1
					<< 190
					<< (float) 178
					<< (float) 253
					<< (float) 153
					<< 0
					<< 1095;

				do_NPCRegen(ch, msg);
			}

#endif // #ifdef LC_KOR
#ifdef FEEITEM			
			else if ( strcmp( g_buf2, "cashbalance") == 0 )
			{
				CNetMsg msg;
				msg.Init(MSG_EXTEND);
				msg << MSG_EX_CASHITEM
					<< (unsigned char) MSG_EX_CASHITEM_BALANCE_REQ;

				do_Extend(ch, msg);
			}
			else if ( strcmp( g_buf2, "cashpurchase") == 0 )
			{
				g_buf[0] = '\0';
				p = AnyOneArg(p, g_buf);
				int count = atoi(g_buf);
				g_buf[0] = '\0';

				int* ctIndex = new int[count];

				int i;
				for(i = 0; i < count; i++)
				{
					p = AnyOneArg(p, g_buf);

					if(g_buf == NULL) 
						return;

					ctIndex[i] = atoi(g_buf);
					g_buf[0] = '\0';
				}

				CNetMsg msg;
				msg.Init(MSG_EXTEND);
				msg << MSG_EX_CASHITEM
					<< (unsigned char) MSG_EX_CASHITEM_PURCHASE_REQ
					<< ch->m_cashBalance
					<< count;

				for(i = 0; i < count; i++)
				{
					msg << ctIndex[i];
				}
	
				do_Extend(ch, msg);
			}
			else if ( strcmp( g_buf2, "cashpurchaselist") == 0 )
			{
				CNetMsg msg;
				msg.Init(MSG_EXTEND);
				msg << MSG_EX_CASHITEM
					<< (unsigned char) MSG_EX_CASHITEM_PURCHASELIST_REQ;

				do_Extend(ch, msg);
			}
			else if ( strcmp( g_buf2, "cashbring") == 0 )
			{
				g_buf[0] = '\0';
				p = AnyOneArg(p, g_buf);
				int count = atoi(g_buf);
				g_buf[0] = '\0';

				int i;
				int *pIndex = new int[count];
				int *ctIndex = new int[count];

				for(i = 0; i < count; i++)
				{
					p = AnyOneArg(p, g_buf);
					if(g_buf == NULL) 
						return;
					pIndex[i] = atoi(g_buf);
					g_buf[0] = '\0';

					p = AnyOneArg(p, g_buf);
					if(g_buf == NULL) 
						return;
					ctIndex[i] = atoi(g_buf);
					g_buf[0] = '\0';
				}

				CNetMsg msg;
				msg.Init(MSG_EXTEND);
				msg << MSG_EX_CASHITEM
					<< (unsigned char) MSG_EX_CASHITEM_BRING_REQ
					<< count;

				for(i = 0; i < count; i++)
				{
					msg << pIndex[i]
						<< ctIndex[i];
				}
				
				do_Extend(ch, msg);
			}
			else if ( strcmp( g_buf2, "cashpurchasehistory") == 0 )
			{
				g_buf[0] = '\0';
				p = AnyOneArg(p, g_buf);
				if(g_buf == NULL) 
						return;
				int year = atoi(g_buf);
				g_buf[0] = '\0';
				p = AnyOneArg(p, g_buf);
				if(g_buf == NULL) 
						return;
				char month = atoi(g_buf);
				p = AnyOneArg(p, g_buf);
				if(g_buf == NULL) 
						return;
				char day = atoi(g_buf);

				CNetMsg msg;
				msg.Init(MSG_EXTEND);
				msg << MSG_EX_CASHITEM
					<< (unsigned char) MSG_EX_CASHITEM_PURCHASEHISTORY_REQ
					<< year
					<< month
					<< day;

				do_Extend(ch, msg);
			}
#endif // FEEITEM

#ifdef GMTOOL
			if(ch->m_desc->m_userFlag & IS_GM_CHAR)
			{
				if( IS_RUNNING_CONN )
				{
					CNetMsg rmsg;
					ConnGMToolCommand(rmsg, ch->m_index, ch->m_name, buf);
					SEND_Q(rmsg, gserver.m_connector);
				}
			}
#endif // GMTOOL
		}
		break;
	}
}

void do_GMEcho(CPC* ch, const char* arg)
{
#ifdef GMTOOL
	if(ch->m_desc->m_userFlag & IS_GM_CHAR)
	{
		*g_buf = '\0';		// argument for arg
		if (!arg || !(*arg))		return ;

		int serverno, subno;
		char* p1;
		arg = AnyOneArg(arg, g_buf);
		p1 = g_buf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		serverno = atoi(g_buf);

		if (!arg || !(*arg))
			return;

		arg = AnyOneArg(arg, g_buf);
		p1 = g_buf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		subno = atoi(g_buf);

		arg = AnyOneArg(arg, g_buf, true);
		// 0: server, 1: all, 2: subserver
		int type;
		if (strcmp(g_buf, GM_CMD_ECHO_SERVER) == 0)		type = 0;
		else if (strcmp(g_buf, GM_CMD_ECHO_ALL) == 0)	type = 1;
		else if (strcmp(g_buf, GM_CMD_ECHO_SUBSERVER) == 0)		type = 2;
		else return ;

		arg = SkipSpaces(arg);
		if (!arg || !(*arg))		return ;

		CNetMsg msg;
		switch (type)
		{
		case 0:
			MsgrNoticeMsg(msg, -1, serverno, -1, -1, arg);
			break;
		case 1:
			MsgrNoticeMsg(msg, -1, -1, -1, -1, arg);
			break;
		case 2:
			MsgrNoticeMsg(msg, -1, serverno, subno, -1, arg);
			break;
		}
		if (IS_RUNNING_MSGR) SEND_Q(msg, gserver.m_messenger);
		return ;
	}
#endif // GMTOOL
	*g_buf = '\0';		// argument for arg
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, g_buf, true);
	// 0: zone, 1: server, 2: all, 3: subserver
	int type;
		 if (strcmp(g_buf, GM_CMD_ECHO_ZONE) == 0)		type = 0;
	else if (strcmp(g_buf, GM_CMD_ECHO_SERVER) == 0)	type = 1;
	else if (strcmp(g_buf, GM_CMD_ECHO_ALL) == 0)		type = 2;
	else if (strcmp(g_buf, GM_CMD_ECHO_SUBSERVER) == 0)	type = 3;
	else if (strcmp(g_buf, GM_CMD_ECHO_ALLZONE) == 0)	type = 4;
	else return ;

	arg = SkipSpaces(arg);
	if (!arg || !(*arg))		return ;

	CNetMsg msg;
	switch (type)
	{
	case 0:
		MsgrNoticeMsg(msg, -1, gserver.m_serverno, gserver.m_subno, ch->m_pZone->m_index, arg);
		break;
	case 1:
		MsgrNoticeMsg(msg, -1, gserver.m_serverno, -1, -1, arg);
		break;
	case 2:
		MsgrNoticeMsg(msg, -1, -1, -1, -1, arg);
		break;
	case 3:
		MsgrNoticeMsg(msg, -1, gserver.m_serverno, gserver.m_subno, -1, arg);
		break;
	case 4:
		MsgrNoticeMsg(msg, -1, -1, -1, ch->m_pZone->m_index, arg);
		break;
	}
	if (IS_RUNNING_MSGR) SEND_Q(msg, gserver.m_messenger);

}

void do_GMGoZone(CPC* ch, const char* arg)
{
	*g_buf = '\0';		// argument for command
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, g_buf);
	int zone = atoi(g_buf);
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, g_buf);
	int extra = atoi(g_buf);

	if (zone == ZONE_SINGLE_DUNGEON_TUTORIAL)
		return;

#ifdef MONSTER_COMBO
	if (zone == ZONE_COMBO_DUNGEON)
		return;
#endif

	if (zone == ZONE_SINGLE_DUNGEON1 || zone == ZONE_SINGLE_DUNGEON2)
	{
#ifdef QUEST_DATA_EXTEND
		CQuest* pQuest = NULL;
#else
		CQuest* quest = NULL;
#endif // QUEST_DATA_EXTEND

		int i;
		for (i=0; i < gserver.m_questProtoList.m_nCount; i++)
		{
			if (zone == ZONE_SINGLE_DUNGEON1)
			{
				if (gserver.m_questProtoList.m_proto[i].m_type[0] == QTYPE_KIND_DEFEAT)
				{
#ifdef QUEST_DATA_EXTEND
					pQuest = ch->m_questList.AddQuest(gserver.m_questProtoList.m_proto[i].m_index);
#else
					quest = gserver.m_questProtoList.Create(gserver.m_questProtoList.m_proto[i].m_index);
#endif // QUEST_DATA_EXTEND
					break;
				}
			}

			if (zone == ZONE_SINGLE_DUNGEON2)
			{
				if (gserver.m_questProtoList.m_proto[i].m_type[0] == QTYPE_KIND_SAVE)
				{
#ifdef QUEST_DATA_EXTEND
					pQuest = ch->m_questList.AddQuest(gserver.m_questProtoList.m_proto[i].m_index);
#else
					quest = gserver.m_questProtoList.Create(gserver.m_questProtoList.m_proto[i].m_index);
#endif // QUEST_DATA_EXTEND
					break;
				}
			}
		}

#ifdef QUEST_DATA_EXTEND
		if (pQuest == NULL)
			return ;
		ch->m_questList.SetQuestState(pQuest, QUEST_STATE_RUN);
		pQuest->SetComplete1(false);
		pQuest->SetComplete2(false);
		
		// Quest Start Log
		GAMELOG << init("QUEST START", ch)
				<< pQuest->GetQuestIndex()
				<< end;
		
		// Quest Start Msg
		CNetMsg rMsg;
		QuestStartMsg(rMsg, pQuest);
		SEND_Q(rMsg, ch->m_desc);
#else // QUEST_DATA_EXTEND
		if (!quest)
			return;
		
		// 캐릭터 퀘스트 리스트에 Add
		int idx = ch->m_questList.AddQuest(quest);
		if (idx < 0)
		{
			delete quest;
			return;
		}
		
		ch->m_questList.m_bQuest[idx] = true;
		ch->m_questList.SetComplete(idx, false);
		ch->m_questList.SetComplete2(idx, false);
		
		// Quest Start Log
		GAMELOG << init("QUEST START", ch)
				<< quest->m_proto->m_index
				<< end;
		
		// Quest Start Msg
		CNetMsg rMsg;
		QuestStartMsg(rMsg, ch->m_questList.m_list[idx]);
		SEND_Q(rMsg, ch->m_desc);
#endif // QUEST_DATA_EXTEND
	}

	// 동일 싱글 던전으로는 이동 불가
	if ((ch->m_pZone->IsPersonalDungeon() || ch->m_pZone->IsGuildRoom()) && ch->m_pZone->m_index == zone)
		return ;

	// 존을 찾아서
	int i = gserver.FindZone(zone);
	if (i == -1)
		return ;

	CZone* pZone = gserver.m_zones + i;

	if (extra < 0 || extra >= pZone->m_countZonePos)
		return ;

#ifdef LC_KOR
	if (ch->m_pZone->m_index == ZONE_SINGLE_DUNGEON_TUTORIAL && zone == ZONE_START)
		extra = 2;
#endif

	GoZone(ch, zone,
			pZone->m_zonePos[extra][0],														// ylayer
			GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
			GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z

}

void do_GMGoto(CPC* ch, const char* arg)
{
	*g_buf = '\0';		// argument for command
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, g_buf);
	float x = atof(g_buf);
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, g_buf);
	float z = atof(g_buf);
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, g_buf);
	char ylayer = (char)atoi(g_buf);

	//if (ylayer < 0 || ylayer >= ch->m_pZone->m_countY)
	// yLayer 수정
	if (ylayer < 0 || ylayer > ((ch->m_pZone->m_countY - 1) * 2))
		return ;

	if (x < 0 || z < 0 || x >= ch->m_pZone->m_attrMap[ylayer/2].m_size[0] || z >= ch->m_pZone->m_attrMap[ylayer/2].m_size[1])
		return ;

	GoTo(ch, ylayer, x, z, ch->m_pArea->GetHeight(ylayer, x, z), GET_R(ch));

}

void do_GMItemDrop(CPC* ch, const char* arg)
{
	int dbindex, plus = 0, flag = 0;
	LONGLONG count = 1;

	int		optionCount = 0;
	int*	optionType = NULL;
	int*	optionLevel = NULL;

	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, g_buf);
	dbindex = atoi(g_buf);
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, g_buf);
		plus = atoi(g_buf);
	}
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, g_buf);
		flag = atoi(g_buf);
	}
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, g_buf);
		count = ATOLL(g_buf);
	}

	if (count == 0)
		count = 1;


	CItem* item = gserver.m_itemProtoList.CreateItem(dbindex, -1, plus, flag, count);

	if (item == NULL)
		return ;

#ifdef EVENT_EGGS_HUNT_2007
	if(item->m_idNum == 2148)
		item->m_used = 1;		// 1: 운영자가 떨어뜨린 것, -1: 몬스터가 떨어뜨린 것
#endif // EVENT_EGGS_HUNT_2007

	// itemdrop <index> <plus> <flag> <count> <option_count> <option_type> <option_level>
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, g_buf);
		optionCount = atoi(g_buf);

		bool bIsRare = false;

#ifdef MONSTER_RAID_SYSTEM
		if (item->IsRareItem())
			bIsRare = true;
#endif // MONSTER_RAID_SYSTEM

		if (bIsRare)
		{
#ifdef MONSTER_RAID_SYSTEM
			if (optionCount == 1)
			{
				if (!arg || !(*arg))
				{
					delete item;
					return ;
				}
				arg = AnyOneArg(arg, g_buf);
				int nRareIndex = atoi(g_buf);
				
				if (!arg || !(*arg))
				{
					delete item;
					return ;
				}
				arg = AnyOneArg(arg, g_buf);
				int nRareBit = atoi(g_buf);

				const CRareOptionProto* pProto = gserver.m_rareOptionList.Find(nRareIndex);
				if (pProto)
				{
					int i;
					for (i = 0; i < MAX_RARE_OPTION_SETTING; i++)
					{
						const RAREOPTIONDATA* pROD = pProto->GetData(i);
						if (pROD->optionlevel == 0)
						{
							nRareBit &= ~pROD->rarebit;
						}
					}
					if (nRareBit != 0)
					{
						item->m_nOption = 2;
						item->m_option[0].GetDBValue((short)nRareIndex);
						item->m_option[1].GetDBValue((short)nRareBit);
						item->m_pRareOptionProto = pProto;
						item->m_nRareOptionIndex = nRareIndex;
						item->m_nRareOptionBit = nRareBit;
					}
				}
			}
			else
			{
				delete item;
				return ;
			}
#endif // MONSTER_RAID_SYSTEM
		}
		else
		{
			// 옵션 부분 추가
			if (optionCount > 0 && optionCount <= MAX_ITEM_OPTION)
			{
				optionType = new int[optionCount];
				optionLevel = new int[optionCount];
				
				int i;
				for (i=0; i < optionCount; i++)
				{
					if (!arg || !(*arg))
					{
						delete item;
						return ;
					}
					arg = AnyOneArg(arg, g_buf);
					optionType[i] = atoi(g_buf);
					
					if (!arg || !(*arg))
					{
						delete item;
						return ;
					}
					arg = AnyOneArg(arg, g_buf);
					optionLevel[i] = atoi(g_buf);
					
					if (optionType[i] < 0 || optionType[i] >= MAX_NUM_OPTION || optionLevel[i] < 1 || optionLevel[i] > OPTION_MAX_LEVEL)
						continue;
					
					// Option 붙이기
					COptionProto* proto = gserver.m_optionProtoList.FindProto(optionType[i]);
					
					if (!proto)
						continue;
					
					item->m_option[i].m_proto = proto;
					item->m_option[i].m_type = optionType[i];
					item->m_option[i].m_level = optionLevel[i];
					item->m_option[i].m_value = proto->m_levelValue[optionLevel[i] - 1];
					item->m_option[i].SetDBValue();
					item->m_nOption++;
				}
			}
		}

#ifdef SET_ITEM
		if (item->IsOriginItem())
		{
			item->m_pSetItemProto = gserver.m_setItemProtoList.Find(item->m_itemProto->GetSetIndex());
		}
#endif
	}

	ch->m_pArea->DropItem(item, ch);

	CNetMsg rmsg;
	ItemDropMsg(rmsg, ch, item);
	ch->m_pArea->SendToCell(rmsg, ch, true);

	GAMELOG << init("ITEM_LOAD", ch)
			<< itemlog(item)
			<< end;

	if (optionType)
		delete [] optionType;

	if (optionLevel)
		delete [] optionLevel;

}

void do_GMCountPC(CPC* ch, const char* arg)
{
	int mode = 0;
	int server = -1;

	if (!arg || !*arg)
		mode = 0;

	else
	{
		arg = AnyOneArg(arg, g_buf, true);
		TrimString(g_buf);

		if (strlen(g_buf) == 0)
			mode = 0;
		else if (strcmp(g_buf, "all") == 0)
			mode = 1;
		else
		{
			mode = 2;
			server = atoi(g_buf);
		}
	}

	// 메신저에 요청
	CNetMsg rmsg;
	switch (mode)
	{
	case 0:		// 현재 서버 - 서브
		MsgrPlayerCountReq(rmsg, gserver.m_serverno, gserver.m_subno, ch->m_index);
		break;
	case 1:		// 전체 서버
		MsgrPlayerCountReq(rmsg, -1, -1, ch->m_index);
		break;
	case 2:		// 특정 서버군
		MsgrPlayerCountReq(rmsg, server, -1, ch->m_index);
		break;
	default:
		return ;
	}
	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);

}

void do_GMLoopCount(CPC* ch, const char*  arg)
{
#ifdef GMTOOL
	if(ch->m_desc->m_userFlag & IS_GM_CHAR)
	{
		if (!arg || !(*arg))
			return;

		int serverno, subno;
		char* p1;
		arg = AnyOneArg(arg, g_buf);
		p1 = g_buf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		serverno = atoi(g_buf);

		if (!arg || !(*arg))
			return;

		arg = AnyOneArg(arg, g_buf);
		p1 = g_buf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		subno = atoi(g_buf);

		gserver.m_descLoop = ch->m_desc;

		if(IS_RUNNING_MSGR)
		{
			CNetMsg rmsg;
			MsgExtraGMCommandReq(rmsg, serverno, subno, MSG_MSGR_GM_LOOPCOUNT_REQ);
			SEND_Q(rmsg, gserver.m_messenger);

			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Sending LoopCount");
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}
#endif // GMTOOL

	if (gserver.m_bLoop)
		return ;

	gserver.m_bLoop = 1;
	gserver.m_nLoop = 0;
	GetTimeofDay(&gserver.m_tvLoop, NULL);
	gserver.m_descLoop = ch->m_desc;

}

void do_GMImmortal(CPC* ch, const char* /* arg */)
{
	CLCString chat(100);
	
	if (ch->m_bImmortal)
	{
		ch->m_bImmortal = false;
		chat = "Immortal Set Off";
	}
	else
	{
		ch->m_bImmortal = true;
		DelAttackList(ch);
		chat = "Immortal Set On";
	}
	
	CNetMsg msg;
	SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", chat);
	SEND_Q(msg, ch->m_desc);

}

void do_GMShutdown(CPC*  ch, const char* arg)
{
#ifdef GMTOOL
	if(ch->m_desc->m_userFlag & IS_GM_CHAR)
	{
		if (!arg || !(*arg))
			return;

		int serverno, subno;
		char* p1;
		arg = AnyOneArg(arg, g_buf);
		p1 = g_buf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		serverno = atoi(g_buf);

		if (!arg || !(*arg))
			return;

		arg = AnyOneArg(arg, g_buf);
		p1 = g_buf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		subno = atoi(g_buf);

		arg = AnyOneArg(arg, g_buf);
		int t = atoi(g_buf);
		CLCString chat = arg;
		chat.Trim();
		
		if(IS_RUNNING_MSGR)
		{
			CNetMsg rmsg;
			MsgrGMShutdownReqMsg(rmsg, serverno, subno, t, (const char*)chat);
			SEND_Q(rmsg, gserver.m_messenger);
		}
		return ;
	}
#endif // GMTOOL
	if (!arg || !(*arg))
		return;

	arg = AnyOneArg(arg, g_buf);
	int t = atoi(g_buf);
	CLCString chat = arg;
	chat.Trim();

	if (gserver.m_pulseShutdown != -1)
		return ;

	gserver.m_pulseShutdown = t * PULSE_REAL_SEC;

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

void do_GMReboot(CPC* /* ch */, const char* arg)
{
	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);

	CNetMsg rmsg;
	if( strcmp(g_buf, GM_CMD_ECHO_ALL) == 0 )
	{
		MsgrRebootReqMsg(rmsg, -1, -1, -1, -1);
	}
	else if( strcmp(g_buf, GM_CMD_ECHO_SERVER) == 0 )
	{
		MsgrRebootReqMsg(rmsg, -1, gserver.m_serverno, -1, -1);
	}
	else if( strcmp(g_buf, GM_CMD_ECHO_SUBSERVER) == 0 )
	{
		MsgrRebootReqMsg(rmsg, -1, gserver.m_serverno, gserver.m_subno, -1);
	}
	else
		return;

	arg = AnyOneArg(arg, g_buf);
	int t = atoi(g_buf);
	CLCString chat = arg;
	chat.Trim();

	if (gserver.m_pulseReboot != -1)
		return ;

	

	rmsg << t
		 << chat;
	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);

	/*
	gserver.m_pulseReboot = t * PULSE_REAL_SEC;
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
	*/

}

void do_GMCountNPC(CPC* ch, const char* /* arg */)
{
	CLCString chat(100);
	int i;

	for (i=0; i < gserver.m_numZone; i++)
	{
		if (gserver.m_zones[i].m_countArea > 1)
			continue;

		chat.Format("Zone :%d First :%d Now :%d ShopCount :%d"
			, gserver.m_zones[i].m_index, gserver.m_zones[i].m_area[0].m_npcRegenList.m_nCount, gserver.m_zones[i].m_area[0].m_nCountNPC, gserver.m_zones[i].m_nShopCount);

		CNetMsg msg;
		SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", chat);
		SEND_Q(msg, ch->m_desc);
	}

}

void do_GMVisible(CPC* ch, const char* /* arg */)
{
	ch->SetVisible();

	CNetMsg msg;

	if (ch->m_bVisible)
	{
		SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "Visible: ON");
	}
	else
	{
		SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "Visible: OFF");
	}
	SEND_Q(msg, ch->m_desc);

}

void do_GMGoPC(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))		return;

	// 이름 가져 오기
	CLCString name = arg;
	name.Trim();

	// 나랑 비교
	if (strcmp(name, ch->m_name) == 0 || strcmp(name, ch->m_nick) == 0)
		return ;

	// 전체 유저 중에
	int i;
	for (i = 0; i < gserver.m_playerList.m_max; i++)
	{
		CPC* pc = gserver.m_playerList.m_pcList[i];

		// 유저 검사
		if (pc && ( strcmp(pc->m_name, name) == 0 || strcmp(pc->m_nick, name) == 0))
		{
#ifdef GM_GO_ZONE
			if (pc->m_pZone->IsPersonalDungeon())
			{
				CNetMsg rmsg;
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "This PC in SINGLE_DUNGEON");
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			GMGoZone(ch, pc);
#else
			// 같은 존
			if (pc->m_pZone == ch->m_pZone)
			{
				// 이동 시키기
				GoTo(ch, GET_YLAYER(pc), GET_X(pc), GET_Z(pc), ch->m_pArea->GetHeight(GET_YLAYER(pc), GET_X(pc), GET_Z(pc)), GET_R(ch));
				return ;
			}
			// 같은 존에 없으면 해당존으로 pc의 좌표로 이동
			else
			{
				if (pc->m_pZone->IsPersonalDungeon() || pc->m_pZone->IsGuildRoom())
				{
					CNetMsg rmsg;
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "This PC in SINGLE_DUNGEON");
					SEND_Q(rmsg, ch->m_desc);
					return;
				}

				GoZone(ch, pc->m_pZone->m_index, pc->m_pos.m_yLayer, pc->m_pos.m_x, pc->m_pos.m_z);
				return;
			}
#endif
		}
	}
	// TODO : 아예 없으면 Use 메신저
	// 없으면 메시지
	CNetMsg rmsg;
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not Found the PC");
	SEND_Q(rmsg, ch->m_desc);

}

void do_GMGoNPC(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))		return;

	arg = AnyOneArg(arg, g_buf);

	CLCString name(128);
	name = g_buf;
	name.Trim();
	const char* str = name;

	if(str == NULL) return;

	if (strlen(str) < 1)
		return ;

	bool bName = false;
	while (*str)
	{
		if (!isdigit(*str))
		{
			bName = true;
			break;
		}
		str++;
	}
	
	bool bToMe = false;
	if( bName && strncmp( str, "tome", 4 ) == 0 )
	{
		arg = AnyOneArg(arg, g_buf);
		name = g_buf;
		name += arg;
		name.Trim();
		
		if ( name.Length() < 1)
			return ;

		const char* str2 = name;
		bName = false;
		while (*str2)
		{
			if (!isdigit(*str2))
			{
				bName = true;
				break;
			}
			str2++;
		}

		bToMe = true;
	}
	else
	{
		name += arg;
		name.Trim();
	}
	
	int index = -1;
	if (!bName)
		index = atoi(name);

	CArea* area = ch->m_pArea;

	int i, j;

	int sx = 0;
	int ex = area->m_size[0] - 1;
	int sz = 0;
	int ez = area->m_size[1] - 1;
	float retdist = ((area->m_size[0] > area->m_size[1]) ? area->m_size[0] : area->m_size[1]) * CELL_SIZE;

	CCharacter* p;
	CNPC* npc;
	CNPC* ret = NULL;
	float dist;

	CNetMsg rmsg;

	// 자신 주변에서
	for (i = sx; i <= ex; i++)
	{
		if (i < 0 || i >= area->m_size[0])
			continue ;

		for (j = sz; j <= ez; j++)
		{
			if (j < 0 || j >= area->m_size[1])
				continue ;

			// NPC를 찾기
			p = area->m_cell[i][j].m_listChar;

			while (p)
			{
				if (IS_NPC(p))
				{
					npc = TO_NPC(p);
					if ((index == -1 && strcmp(npc->m_name, name) == 0) || (index != -1 && npc->m_idNum == index))
					{
						// 거리를 비교해서 가까운 넘으로
						dist = GetDistance(ch, npc);
						if (dist < retdist)
						{
							retdist = dist;
							ret = npc;
						}
					}
				}
				p = p->m_pCellNext;
			}
		}
	}

	if (ret == NULL)
	{
		// 없으면 메시지
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not Found the NPC");
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if( bToMe )
	{
		GoTo(ret, GET_YLAYER(ch), GET_X(ch), GET_Z(ch), ret->m_pArea->GetHeight(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)), GET_R(ret));
	}
	else
	// 이동 시키기
		GoTo(ch, GET_YLAYER(ret), GET_X(ret), GET_Z(ret), ch->m_pArea->GetHeight(GET_YLAYER(ret), GET_X(ret), GET_Z(ret)), GET_R(ch));

}


void do_GMLevelUP(CPC* ch, const char* arg)
{
	if  (!arg || !(*arg))		return;

	CLCString var = arg;
	var.Trim();

	if (var.Length() < 1)
		return ;

	int level = atoi(var);

	if (level < 1 || level > MAX_LEVEL)
		return;

	ch->m_exp = 0;
	ch->m_level = level;

	ch->LevelUp(true);

	ch->CalcStatus(true);

}


void do_GMKICK(CPC* ch, const char* arg)
{
#ifdef GMTOOL
#if !defined (LC_GER)
	if(ch->m_desc->m_userFlag & IS_GM_CHAR)
#endif
	{
		if (!arg || !(*arg))		
			return;

		char name[100];
		arg = AnyOneArg(arg, g_buf);
		TrimString(g_buf);
		strcpy(name, g_buf);

		if(strcmp(name, ch->m_name) == 0 || strcmp(name, ch->m_nick) == 0)
		{
			strcpy(g_buf, "Not Found PC: ");
			strcat(g_buf, name);
			CNetMsg rmsg;
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		if(IS_RUNNING_MSGR)
		{
			int server, sub;
			char* p1;

			if (!arg || !(*arg))		
				return;

			arg = AnyOneArg(arg, g_buf);
			TrimString(g_buf);
			p1 = g_buf;
			while (*p1)
			{
				if (!isdigit(*p1))
					return ;		//goto INVALID_FORMAT;
				p1++;
			}
			server = atoi(g_buf);

			if (!arg || !(*arg))		
				return;

			arg = AnyOneArg(arg, g_buf);
			TrimString(g_buf);
			p1 = g_buf;
			while (*p1)
			{
				if (!isdigit(*p1))
					return ;		//goto INVALID_FORMAT;
				p1++;
			}
			sub = atoi(g_buf);

			CNetMsg rmsg;
			MsgGmKickCommandReqMsg(rmsg, 0, server, sub, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, name);
			SEND_Q(rmsg, gserver.m_messenger);

			strcpy(g_buf, "Sending Kick: ");
			strcat(g_buf, name);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
			SEND_Q(rmsg, ch->m_desc);

		}

		return ;
	}
#endif // GMTOOL

	if (!arg || !(*arg))		return;

	CLCString name = arg;
	name.Trim();

	if (strcmp(name, ch->m_name) == 0 || strcmp(name, ch->m_nick) == 0)
		return;

	// 캐릭터 찾기
	CPC* tch = gserver.m_playerList.Find(name, false );
	if (tch)
	{
		CNetMsg msg;
		MsgrLogoutReqMsg(msg, -1, gserver.m_serverno, gserver.m_subno, -1, tch->m_desc->m_idname);

		if (IS_RUNNING_MSGR)
			SEND_Q(msg, gserver.m_messenger);

		// 보냈다는 메시지
		strcpy(g_buf, "Sending Kick: ");
		strcat(g_buf, name);
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		// 없으면 메시지
		strcpy(g_buf, "Not Found PC: ");
		strcat(g_buf, name);
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, ch->m_desc);
	}

}

void do_GMComon(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))		return;

	// 이름 가져 오기
	CLCString name = arg;
	name.Trim();

	//////yhj/////////////////////081014
	int user_index = atoi(arg);
	//if (user_index <= 0)
	//	return ;
	/////////////////////////////////////

	// 나랑 비교
	if (strcmp(name, ch->m_name) == 0 || strcmp(name, ch->m_nick) == 0 || ch->m_index == user_index ) //
		return ;

	int i;
	// 전체 유저 중에
	for (i = 0; i < gserver.m_playerList.m_max; i++)
	{
		CPC* pc = gserver.m_playerList.m_pcList[i];

		// 유저 검사
		if (pc && ( strcmp(pc->m_name, name) == 0 || strcmp(pc->m_nick, name) == 0 || pc->m_index == user_index ) ) //
		{
			// 같은 존
			if (pc->m_pZone == ch->m_pZone)
			{
				// 이동 시키기
				GoTo(pc, GET_YLAYER(ch), GET_X(ch), GET_Z(ch), pc->m_pArea->GetHeight(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)), GET_R(pc));
				return ;
			}
			// 같은 존에 없으면 해당존으로 ch의 좌표로 이동
			else
			{
				if (ch->m_pZone->IsPersonalDungeon() || ch->m_pZone->IsGuildRoom())
					return;

				GoZone(pc, ch->m_pZone->m_index, ch->m_pos.m_yLayer, ch->m_pos.m_x, ch->m_pos.m_z);
				return;
			}
		}
	}

	// TODO : 아예 없으면 Use 메신저
	// 없으면 메시지
	CNetMsg rmsg;
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not Found the PC");
	SEND_Q(rmsg, ch->m_desc);

}

void do_GMSummon(CPC* ch, const char* arg)
{
	int npcindex = -1;
	int count = 1;
	int i;
	bool bName = false;
	const char* str = g_buf;
	const char* name = g_buf;

	g_buf[0] = '\0';
	g_buf2[0] = '\0';
	arg = AnyOneArg(arg, g_buf);		// npc
	TrimString(g_buf);
	CLCString var = arg;
	var.Trim();

	if (g_buf[0] == '\0')
		return ;
	if (var[0] != '\0')
	{
		count = atoi(g_buf);
		str = var;
		name = var;
	}

	if (ch->IsInPeaceZone(true))
		return ;

	while (!bName && *str)
	{
		if (!isdigit(*str))
			bName = true;
		str++;
	}
	if (bName)
	{
		for (i = 0; i < gserver.m_npcProtoList.m_nCount; i++)
		{
			if (strcmp(gserver.m_npcProtoList.m_npcProtoList[i].m_name, name) == 0)
			{
				npcindex = gserver.m_npcProtoList.m_npcProtoList[i].m_index;
				break;
			}
		}
		if (npcindex == -1)
			return ;
	}
	else
	{
		npcindex = atoi(name);
		CNPCProto* proto = gserver.m_npcProtoList.FindProto(npcindex);
		if (!proto)
			return ;
	}

#ifdef BLOCK_SUMMON_OTHERZONE_MOB
	// 해당 Area의 리젠리스트에 있는지 : 있는 NPC만 소환할 수 있다 
	for (i = 0; i < ch->m_pArea->m_npcRegenList.m_nCount; i++)
	{
		if (npcindex == ch->m_pArea->m_npcRegenList.m_infoList[i].m_npcIdx)
			break;
	}

	if (i == ch->m_pArea->m_npcRegenList.m_nCount)
		return;
#endif

	for (i = 0; i < 10 && i < count; i++)
	{
		CNPC* npc = gserver.m_npcProtoList.Create(npcindex, NULL);
		if (!npc)
			return ;

		int nTry = 0;
		do
		{
			GET_X(npc) = GET_X(ch) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
			GET_Z(npc) = GET_Z(ch) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
			GET_YLAYER(npc) = GET_YLAYER(ch);
			GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			if (ch->m_pArea->GetAttr(GET_YLAYER(npc), GET_X(npc), GET_Z(npc)) == MAPATT_FIELD)
				break;
			nTry++;
		} while (nTry <= 10);
		if (nTry > 10)
		{
			GET_X(npc) = GET_X(ch);
			GET_Z(npc) = GET_Z(ch);
			GET_YLAYER(npc) = GET_YLAYER(ch);
			GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		}
		
		npc->m_regenX = GET_X(npc);
		npc->m_regenY = GET_YLAYER(npc);
		npc->m_regenZ = GET_Z(npc);

		int cx, cz;
		ch->m_pArea->AddNPC(npc);
		ch->m_pArea->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
		ch->m_pArea->CharToCell(npc, GET_YLAYER(npc), cx, cz);
		
		CNetMsg appearNPCMsg;
		AppearMsg(appearNPCMsg, npc, true);
		ch->m_pArea->SendToCell(appearNPCMsg, GET_YLAYER(npc), cx, cz);
	}
}

void do_GMSilence(CPC*  ch , const char* arg)
{
#ifdef GMTOOL
	if(ch->m_desc->m_userFlag & IS_GM_CHAR)
	{
		if (!arg || !(*arg))		return ;

		char charname[100];
		char* p1;
		arg =  AnyOneArg(arg, g_buf);
		TrimString(g_buf);
		strcpy(charname, g_buf);

		if (strlen(g_buf) < 4)
			return;

		if (!arg || !(*arg))		
			return ;

		int sec;
		arg =  AnyOneArg(arg, g_buf);
		TrimString(g_buf);
		p1 = g_buf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		sec = atoi(g_buf);
		if(sec < 1)
			return;

		if (!arg || !(*arg))		
			return ;

		int serverno, subno;
		arg =  AnyOneArg(arg, g_buf);
		TrimString(g_buf);
		p1 = g_buf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		serverno = atoi(g_buf);

		if (!arg || !(*arg))		
			return ;
		arg =  AnyOneArg(arg, g_buf);
		TrimString(g_buf);
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		subno = atoi(g_buf);

		if( IS_RUNNING_MSGR )
		{
			CNetMsg rmsg;
			MsgGMSilenceReq(rmsg, 0, serverno, subno, ch->m_index, charname, sec);
			SEND_Q(rmsg, gserver.m_messenger);
		}

		return ;
	}
#endif // GMTOOL

	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);

	if (strlen(g_buf) < 4)
		return;

	int sec = 0;

	// 전체 유저 중에
	int i;
	for (i = 0; i < gserver.m_playerList.m_max; i++)
	{
		CPC* pc = gserver.m_playerList.m_pcList[i];

		// 유저 검사
		if (pc && ( strcmp(pc->m_name, g_buf) == 0 || strcmp(pc->m_nick, g_buf) == 0 ))
		{
			arg = AnyOneArg(arg, g_buf);
			sec = atoi(g_buf);

			if (sec < 1)
				return;

			pc->m_silencePulse = gserver.m_pulse + (sec * PULSE_REAL_SEC);
		}
	}

}

void do_GMWhereAmI(CPC* ch, const char* /* arg */)
{
	CLCString chat(100);

	chat.Format("Server: %d, Subserver: %d, Zone: %d, Area: %d", gserver.m_serverno, gserver.m_subno, ch->m_pZone->m_index, ch->m_pArea->m_index);

	CNetMsg msg;
	SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", chat);
	SEND_Q(msg, ch->m_desc);

}

void do_GMQuestComplete(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))	return;

	CLCString var = arg;
	var.Trim();

	if (var.Length() < 1)
		return ;

	int index = atoi(var);

#ifdef QUEST_DATA_EXTEND
	CQuest* pQuest;
	CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		// 진행중인 퀘스트가 있고 완료되지 않았고 index가 같으면
		if (!pQuest->IsCompleted() && pQuest->GetQuestIndex() == index)
		{
			// Quest Complete Log
			GAMELOG << init("QUEST COMPLETE (GMQuestComplete)", ch)
					<< pQuest->GetQuestIndex()
					<< end;

			CNetMsg completeMsg;
			QuestCompleteMsg(completeMsg, pQuest);
			SEND_Q(completeMsg, ch->m_desc);
			
			pQuest->SetComplete1(true);
			pQuest->SetComplete2(true);
			return;
		}
	}
#else // QUEST_DATA_EXTEND
	int i;
	for (i=0; i < QUEST_MAX_PC; i++)
	{
		// 진행중인 퀘스트가 있고 완료되지 않았고 index가 같으면
		if (ch->m_questList.m_list[i] && ch->m_questList.m_bQuest[i] && !ch->m_questList.IsCompleted(i) && ch->m_questList.m_list[i]->m_proto->m_index == index)
		{
			// Quest Complete Log
			GAMELOG << init("QUEST COMPLETE (GMQuestComplete)", ch)
					<< ch->m_questList.m_list[i]->m_proto->m_index
					<< end;

			CNetMsg completeMsg;
			QuestCompleteMsg(completeMsg, ch->m_questList.m_list[i]);
			SEND_Q(completeMsg, ch->m_desc);
			
			ch->m_questList.SetComplete(i, true);
			ch->m_questList.SetComplete2(i, true);
			return;
		}
	}
#endif // QUEST_DATA_EXTEND
}

void do_GMStatPoint(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))		return;
	arg = AnyOneArg(arg, g_buf);
	int statpt = atoi(g_buf);
	if (statpt < 0)
		return ;
	ch->m_statpt_remain = statpt;
	CNetMsg rmsg;
	StatPointRemainMsg(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);

	if (!arg || !(*arg))		return;
	arg = AnyOneArg(arg, g_buf);
	int strpt = atoi(g_buf);
	if (!arg || !(*arg))		return;
	arg = AnyOneArg(arg, g_buf);
	int dexpt = atoi(g_buf);
	if (!arg || !(*arg))		return;
	arg = AnyOneArg(arg, g_buf);
	int intpt = atoi(g_buf);
	if (!arg || !(*arg))		return;
	arg = AnyOneArg(arg, g_buf);
	int conpt = atoi(g_buf);
	if (strpt < 0 || dexpt < 0 || intpt < 0 || conpt < 0)
		return ;
	ch->m_statpt_str = strpt;
	ch->m_statpt_dex = dexpt;
	ch->m_statpt_int = intpt;
	ch->m_statpt_con = conpt;
	ch->InitStat();
	ch->CalcStatus(true);
}

void do_GMSkillPoint(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))		return;

	int skillpt = atoi(arg);
	if (skillpt <= 0)
		return ;
	if (skillpt > MAX_SKILLPOINT)
		skillpt = MAX_SKILLPOINT;

	ch->m_skillPoint = skillpt * 10000;
	ch->m_bChangeStatus = true;

}

void do_GMSetRegenSec(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);
	int index = atoi(g_buf);

	arg = AnyOneArg(arg, g_buf);
	int regenSec = atoi(g_buf);

	if (index <= 0 || regenSec <= 0)
		return;

	int i;
	for (i=0; i < ch->m_pArea->m_npcRegenList.m_nCount; i++)
	{
		if (ch->m_pArea->m_npcRegenList.m_infoList[i].m_npcIdx == index)
			ch->m_pArea->m_npcRegenList.m_infoList[i].m_regenSec = regenSec;
	}

}


void do_GMNPCRegen(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))
		return;

	arg = AnyOneArg(arg, g_buf);
	int index = atoi(g_buf);

	if (index <= 0)
		return;

	int i;
	for (i=0; i < ch->m_pArea->m_npcRegenList.m_nCount; i++)
	{
		// 해당 npc이고 bAlive가 false이면
		if (ch->m_pArea->m_npcRegenList.m_infoList[i].m_npcIdx == index &&
			!ch->m_pArea->m_npcRegenList.m_infoList[i].m_bAlive)
			// 바로 리젠
			ch->m_pArea->m_npcRegenList.m_infoList[i].m_regenSec = 0;
	}

}


void do_GMKillNPC(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))
		return;

	arg = AnyOneArg(arg, g_buf);
	int distance = atoi(g_buf);
	
	if (distance <= 0)
		return;
	
	// Cell 2개 정도에서
	int sx = ch->m_cellX - 2;
	int ex = ch->m_cellX + 2;
	int sz = ch->m_cellZ - 2;
	int ez = ch->m_cellZ + 2;
	
	int x, z;
	CNetMsg damageMsg;
	
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= ch->m_pArea->m_size[0])
			continue;
		
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= ch->m_pArea->m_size[1])
				continue;
			
			CCharacter* tch;;
			CCharacter* tchNext = ch->m_pArea->m_cell[x][z].m_listChar;
			while ((tch = tchNext))
			{
				tchNext = tchNext->m_pCellNext;

				// npc이고
				if (IS_NPC(tch))
				{
					CNPC* tnpc = TO_NPC(tch);

					if( tnpc->m_proto->m_index == 491 ||				// 바닥데미지 삭제 불가
						( tnpc->m_flag & NPC_WARCASTLE ) )				// 공성 NPC 삭제 불가 
						continue;
					
#ifdef EXTREME_CUBE
					switch(tnpc->m_proto->m_index)
					{
					case 526:
					case 527:
					case 528:
					case 529:
					case 530:
						continue;
						break;
					}
#endif // EXTREME_CUBE

					// npc와 해당거리내에 있으면
					if (GetDistance(ch, tnpc) < distance)
					{
						// 결과 메시지 보냄
						DamageMsg(damageMsg, ch, tnpc, MSG_DAMAGE_MELEE, -1, 1, HITTYPE_NORMAL);
						tnpc->m_pArea->SendToCell(damageMsg, tnpc, false);

						tnpc->m_hp = 0;
						
						if (DEAD(tnpc))
						{

#ifdef EXTREME_CUBE
							if(tnpc->m_bCubeRegen)
							{
								CCubeSpace* cube = gserver.m_extremeCube.GetExtremeCube(tnpc->m_pArea->m_index);
								if(cube)
								{
									cube->DelMob(tnpc);
								}
							}
#endif // EXTREME_CUBE
							tnpc->SendDisappearAllInCell(true);

							// 어택리스트에서 삭제
							DelAttackList(tnpc);

							// npc 삭제
							ch->m_pArea->CharFromCell(tnpc, true);
							ch->m_pArea->DelNPC(tnpc);
						}
					}
				}
			}
		}
	}

}

void do_GMExpUp(CPC* ch, const char* arg)
{
	if (!arg || !*arg)
		return ;
	
	double exppercent = atof(arg);
	if (exppercent < 0 || exppercent > 100)
		return ;
	LONGLONG exp = GetLevelupExp(ch->m_level);
	exp = (LONGLONG)(exp * exppercent / 100);
	ch->m_exp = exp;
	ch->m_bChangeStatus = true;

}


void do_GMBloodPoint(CPC* ch, const char* arg)
{
    if (arg && *arg)
    {
        int bloodPoint = atoi(arg);

		if (bloodPoint < 0 || bloodPoint > PC_BLOOD_POINT)
			return;

        ch->m_bloodPoint = bloodPoint;

		strcpy(g_buf, "Blood points: ");
		IntCat(g_buf, ch->m_bloodPoint);
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf); 
		SEND_Q(rmsg, ch->m_desc);

		if (ch->m_bloodPoint <= 0)
		{
			ch->m_bloodPoint = PC_BLOOD_POINT;
			ch->SettingItemOption();
		}

		return;
    }

	strcpy(g_buf, "Blood points: ");
	IntCat(g_buf, ch->m_bloodPoint);
	CNetMsg rmsg;
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf); 
	SEND_Q(rmsg, ch->m_desc);

}


void do_GMEndGame(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);
	int t = atoi(g_buf);
	CLCString var = arg;
	var.Trim();

	if (gserver.m_pulseEndGame != -1)
		return ;

	gserver.m_pulseEndGame = t * PULSE_REAL_SEC;

	if (var.Length() > 0)
	{
		CNetMsg msg;
		MsgrNoticeMsg(msg, -1, -1, -1, -1, arg);
		if (IS_RUNNING_MSGR) SEND_Q(msg, gserver.m_messenger);
	}

}


void do_GMObserve(CPC* ch, const char* /*arg*/)
{                                                                                              
	CNetMsg rmsg;

	if (gserver.m_observer)
	{
		if (gserver.m_observer == ch->m_desc)
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Observe Reset");
			SEND_Q(rmsg, ch->m_desc);
			gserver.m_observer = NULL;
			return ;
		}

		strcpy(g_buf, "Change Observer: ");

		if (gserver.m_observer->m_pChar)
		{
			strcat(g_buf, gserver.m_observer->m_pChar->GetName());
		}
		else
		{
			strcat(g_buf, gserver.m_observer->m_idname);
		}

		strcat(g_buf, " -> ");
		strcat(g_buf, ch->GetName());

		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q(rmsg, gserver.m_observer);
		SEND_Q(rmsg, ch->m_desc);
	}

	gserver.m_observer = ch->m_desc;
}


void do_GMChatMonitor(CPC* ch, const char* arg)
{
	int i;
	// 있으면 해제
	for (i = 0; i < 20; i++)
	{
		if (gserver.m_chatmonitor[i] == ch->m_desc)
		{
			if (!arg || !*arg)
			{
				gserver.m_chatmonitor[i] = NULL;

				CNetMsg rmsg;
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Monitoring Finished!!!");
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			break;
		}
	}

	// 없으면 추가
	if (i == 20)
	{
		for (i = 0; i < 20; i++)
		{
			if (gserver.m_chatmonitor[i] == NULL)
			{
				gserver.m_chatmonitor[i] = ch->m_desc;
				break;
			}
		}
	}

	// 20명까지 가능
	if (i == 20)
	{
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Cannot More Monitor!!!");
		SEND_Q(rmsg, ch->m_desc);
	}

	// 대상 캐릭터 찾기
	if (!arg || !*arg)
		return ;
	*g_buf = '\0';
	arg = AnyOneArg(arg, g_buf);
	TrimString(g_buf);
	if (strlen(g_buf) < 1)
		return ;

	CPC* pc = gserver.m_playerList.Find(g_buf, false );
	if (pc == NULL)
	{
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not Found the PC");
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	*g_buf = '\0';
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	pc->m_bChatMonitor = true;
	if (strcmp(g_buf, "off") == 0)
		pc->m_bChatMonitor = false;
	else
		pc->m_bChatMonitor = true;

	CNetMsg rmsg;
	strcpy(g_buf, "SET CHAT MONITOR: ");
	strcat(g_buf, pc->m_nick);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
	SEND_Q(rmsg, ch->m_desc);
}

void do_GMDoubleEvent(CPC* ch, const char* arg)
{
#ifdef EVENT_DOUBLE
#ifdef NEW_DOUBLE_GM
	CNetMsg rmsg;
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group
	int	zonenum;			// 대상 존 번호: -1: all, 0 ~ : zonenum
	zonenum = -1;

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "all") == 0)
		target = -1;
	else if (strcmp(g_buf, "cur") == 0)
		target = gserver.m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(g_buf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);

	int percent[6];
	memset(percent, -1, sizeof(percent) );

	if (strcmp(g_buf, "start") == 0)
	{
#ifdef NEW_DOUBLE_GM_ZONE
		if (!arg || !(*arg))
			goto INVALID_FORMAT;
		arg = AnyOneArg(arg, g_buf, true);
		TrimString(g_buf);
		if (strcmp(g_buf, "all") == 0)
			zonenum = -1;
		else
		{
			// all 아니면 숫자여야함
			char* p = g_buf;
			while (*p)
			{
				if (!isdigit(*p))
					goto INVALID_FORMAT;
				p++;
			}
			zonenum = atoi(g_buf);
			if ( zonenum < 0 )
				goto INVALID_FORMAT;
		}
#endif // NEW_DOUBLE_GM_ZONE
		for( int i = 0; i < 6; i ++)
		{
			arg = AnyOneArg(arg, g_buf, true);
			TrimString(g_buf);

			percent[i] = atoi(g_buf);

			if( i == 5 && (percent[i] < 0 || percent[i] > 10) )
				goto INVALID_FORMAT;
#ifdef TLD_DOUBLE_EVENT
			if( percent[i] < 0 || percent[i] > TLD_DOUBLE_EVENT )
				goto INVALID_FORMAT;
#else
			if( percent[i] < 0 || percent[i] > 200)
				goto INVALID_FORMAT;
#endif //TLD_DOUBLE_EVENT
		}

		MsgrDoubleEventReqMsg(rmsg, 0, target, -1, zonenum, ch->m_index, 0, percent);
	}
	else if (strcmp(g_buf, "ing") == 0)
		MsgrDoubleEventReqMsg(rmsg, 0, target, -1, zonenum, ch->m_index, 1, percent);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrDoubleEventReqMsg(rmsg, 0, target, -1, zonenum, ch->m_index, 2, percent);
	else
		goto INVALID_FORMAT;

	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);

	return;

INVALID_FORMAT:
#ifdef NEW_DOUBLE_GM_ZONE
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: double_event <server-group no | all | cur> <start | ing | stop> <zonenum no | all> <nasdrop> <nasget> <exp> <sp> <produce> <producenum> ");
	SEND_Q(rmsg, ch->m_desc);
#else
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: double_event <server-group no | all | cur> <start | ing | stop> <nasdrop> <nasget> <exp> <sp> <produce> <producenum> ");
	SEND_Q(rmsg, ch->m_desc);
#endif // NEW_DOUBLE_GM_ZONE
#else // NEW_DOUBLE_GM

	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);

	TrimString(g_buf);

	CNetMsg msg;

	if (strcmp(g_buf, "start") == 0)
		MsgrDoubleEventReqMsg(msg, -1, gserver.m_serverno, -1, -1, true);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrDoubleEventReqMsg(msg, -1, gserver.m_serverno, -1, -1, false);
	else if (strcmp(g_buf, "ing") == 0)
	{
		if (gserver.m_bDoubleEvent)
			SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "This Server Double Event Ing..");
		else
			SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "This Server Double Event Stop..");

		SEND_Q(msg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_MSGR) SEND_Q(msg, gserver.m_messenger);
#endif // NEW_DOUBLE_GM
#endif // EVENT_DOUBLE
}

void do_GMDoubleExpEvent(CPC* ch, const char* arg)
{
	CNetMsg rmsg;
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "all") == 0)
		target = -1;
	else if (strcmp(g_buf, "cur") == 0)
		target = gserver.m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(g_buf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "start") == 0)
	{
		arg = AnyOneArg(arg, g_buf, true);
		TrimString(g_buf);

		if( atoi(g_buf) < 0 || atoi(g_buf) > 200)
			goto INVALID_FORMAT;

		MsgrDoubleExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, atoi(g_buf));
	}
	else if (strcmp(g_buf, "ing") == 0)
		MsgrDoubleExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, -1);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrDoubleExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, -1);
	else
		goto INVALID_FORMAT;

	if (IS_RUNNING_MSGR)
	{
		SEND_Q(rmsg, gserver.m_messenger);
	}
	else
	{
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
		SEND_Q(rmsg, ch->m_desc);
	}

	return ;

INVALID_FORMAT:
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: doubleexp_event <server-group no | all | cur> <start | ing | stop>");
	SEND_Q(rmsg, ch->m_desc);
}

void do_GMLattoEvent(CPC* ch, const char* arg)
{
#ifdef EVENT_LATTO

	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);

	TrimString(g_buf);

	CNetMsg msg;

	if (strcmp(g_buf, "start") == 0)
		MsgrLattoEventReqMsg(msg, -1, -1, -1, -1, true, gserver.m_serverno, gserver.m_subno, ch->m_index);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrLattoEventReqMsg(msg, -1, -1, -1, -1, false, gserver.m_serverno, gserver.m_subno, ch->m_index);
	else if (strcmp(g_buf, "ing") == 0)
	{
		if (gserver.m_bLattoEvent)
			SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "This Server Latto Event Ing..");
		else
			SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "This Server Latto Event Stop..");

		SEND_Q(msg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_MSGR) SEND_Q(msg, gserver.m_messenger);

#endif 
}

void do_GMNewYearEvent(CPC* ch, const char* arg)
{
#ifdef EVENT_NEWYEAR

	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);

	TrimString(g_buf);

	CNetMsg msg;

	if (strcmp(g_buf, "start") == 0)
		MsgrNewYearEventReqMsg(msg, -1, -1, -1, -1, true, gserver.m_serverno, gserver.m_subno, ch->m_index);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrNewYearEventReqMsg(msg, -1, -1, -1, -1, false, gserver.m_serverno, gserver.m_subno, ch->m_index);
	else if (strcmp(g_buf, "ing") == 0)
	{
		if (gserver.m_bNewYearEvent)
			SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "This Server NewYear Event Ing..");
		else
			SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "This Server NewYear Event Stop..");

		SEND_Q(msg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_MSGR) SEND_Q(msg, gserver.m_messenger);

#endif 
}

void do_GMValentineEvent(CPC* ch, const char* arg)
{
#ifdef EVENT_VALENTINE

	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);

	TrimString(g_buf);

	CNetMsg msg;

	if (strcmp(g_buf, "start") == 0)
		MsgrValentineEventReqMsg(msg, -1, -1, -1, -1, true, gserver.m_serverno, gserver.m_subno, ch->m_index);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrValentineEventReqMsg(msg, -1, -1, -1, -1, false, gserver.m_serverno, gserver.m_subno, ch->m_index);
	else if (strcmp(g_buf, "ing") == 0)
	{
		if (gserver.m_bValentineEvent)
			SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "This Server Valentine Event Ing..");
		else
			SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "This Server Valentine Event Stop..");

		SEND_Q(msg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_MSGR) SEND_Q(msg, gserver.m_messenger);

#endif 
}

void do_GMWhiteDayEvent(CPC* ch, const char* arg)
{
#ifdef EVENT_WHITEDAY

	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);

	TrimString(g_buf);

	CNetMsg msg;

	if (strcmp(g_buf, "start") == 0)
		MsgrWhiteDayEventReqMsg(msg, -1, -1, -1, -1, true, gserver.m_serverno, gserver.m_subno, ch->m_index);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrWhiteDayEventReqMsg(msg, -1, -1, -1, -1, false, gserver.m_serverno, gserver.m_subno, ch->m_index);
	else if (strcmp(g_buf, "ing") == 0)
	{
		if (gserver.m_bWhiteDayEvent)
			SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "This Server WhiteDay Event Ing..");
		else
			SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "This Server WhiteDay Event Stop..");

		SEND_Q(msg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_MSGR) SEND_Q(msg, gserver.m_messenger);

#endif 
}

void do_GMLetterEvent(CPC* ch, const char* arg)
{
#ifdef EVENT_LETTER

	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);

	TrimString(g_buf);

	CNetMsg msg;

	if (strcmp(g_buf, "start") == 0)
		MsgrLetterEventReqMsg(msg, -1, -1, -1, -1, true, gserver.m_serverno, gserver.m_subno, ch->m_index);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrLetterEventReqMsg(msg, -1, -1, -1, -1, false, gserver.m_serverno, gserver.m_subno, ch->m_index);
	else if (strcmp(g_buf, "ing") == 0)
	{
		if (gserver.m_bLetterEvent)
			SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "This Server Letter Event Ing..");
		else
			SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", "This Server Letter Event Stop..");

		SEND_Q(msg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_MSGR) SEND_Q(msg, gserver.m_messenger);

#endif 
}

void do_GMReloadGMIP(CPC* ch, const char* arg)
{
#ifdef USING_GMIP_TABLE
#ifdef GMTOOL
	if(ch->m_desc->m_userFlag & IS_GM_CHAR)
	{
		if (!arg || !(*arg))
			return;

		int serverno, subno;
		char* p1;
		arg = AnyOneArg(arg, g_buf);
		p1 = g_buf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		serverno = atoi(g_buf);

		if (!arg || !(*arg))
			return;

		arg = AnyOneArg(arg, g_buf);
		p1 = g_buf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return false;		//goto INVALID_FORMAT;
			p1++;
		}
		subno = atoi(g_buf);

		if(IS_RUNNING_MSGR)
		{
			CNetMsg rmsg;
			MsgExtraGMCommandReq(rmsg, serverno, subno, MSG_MSGR_GM_RELOADGMIP_REP);
			SEND_Q(rmsg, gserver.m_messenger);
		}
		return ;
	}
#endif // GMTOOL
	gserver.LoadGMIPTable();
#endif
}

void do_GMSpeedUp(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, g_buf);
	int spd = atoi(g_buf);
	if (spd < 1)
	{
		// 원래 속도로
		ch->m_speedup = 0;
	}
	else
	{
		// 입력 속도로
		ch->m_speedup = spd;
	}
	ch->CalcStatus(true);
}

void do_GMFameUp(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, g_buf);
	int fame = atoi(g_buf);

	if (fame < 0)
		fame = 0;
	
	ch->m_fame = fame;
	ch->CalcStatus(true);
}

void do_GMPD4(CPC* ch, const char* arg)
{
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, g_buf);

	// BW
	if (strcmp(g_buf, "rankinit") == 0)
	{
		CNetMsg rmsg;

		rmsg.Init(MSG_HELPER_COMMAND);
		rmsg << MSG_HELPER_PD4_RANK_INIT;

		SEND_Q(rmsg, gserver.m_helper);
	}
	else if (strcmp(g_buf, "rewardinit") == 0)
	{
		CNetMsg rmsg;

		rmsg.Init(MSG_HELPER_COMMAND);
		rmsg << MSG_HELPER_PD4_REWARD_INIT;

		SEND_Q(rmsg, gserver.m_helper);
	}
	else if (strcmp(g_buf, "rewardend") == 0)
	{
		CNetMsg rmsg;

		rmsg.Init(MSG_HELPER_COMMAND);
		rmsg << MSG_HELPER_PD4_REWARD_END;

		SEND_Q(rmsg, gserver.m_helper);
	}
}

void do_GMKickID(CPC* ch, const char* arg)
{
#ifdef GMTOOL
	if(ch->m_desc->m_userFlag & IS_GM_CHAR)
	{
		if (!arg || !(*arg))
			return ;
		
		int serverno, subno;
		char id[100];
		char* p1;

		arg = AnyOneArg(arg, g_buf);
		TrimString(g_buf);

		if (strlen(g_buf) < 1)
			return ;

		if (strcmp(g_buf, ch->m_desc->m_idname) == 0)
			return ;

		strcpy(id, g_buf);

		if (!arg || !(*arg))
			return ;

		arg = AnyOneArg(arg, g_buf);
		TrimString(g_buf);
		p1 = g_buf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		serverno = atoi(g_buf);

		if (!arg || !(*arg))
			return ;

		arg = AnyOneArg(arg, g_buf);
		TrimString(g_buf);
		p1 = g_buf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		subno = atoi(g_buf);

		if (IS_RUNNING_MSGR)
		{	
			CNetMsg rmsg;	

			MsgGmKickIDCommandReqMsg(rmsg, 0, serverno, subno, -1, 1, 1, ch->m_index, id);
			SEND_Q(rmsg, gserver.m_messenger);

			// 보냈다는 메시지
			sprintf(g_buf2, "Sending Kick ID: %s", id);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf2);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}
#endif // GMTOOL

	if (!arg || !(*arg))
		return ;

	arg = AnyOneArg(arg, g_buf);
	TrimString(g_buf);

	if (strlen(g_buf) < 1)
		return ;

	if (strcmp(g_buf, ch->m_desc->m_idname) == 0)
		return ;

	if (IS_RUNNING_MSGR)
	{
		CNetMsg rmsg;

		MsgrLogoutReqMsg(rmsg, -1, -1, -1, -1, g_buf);
		SEND_Q(rmsg, gserver.m_messenger);

		// 보냈다는 메시지
		sprintf(g_buf2, "Sending Kick ID: %s", g_buf);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf2);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMResurrection(CPC *ch, const char* arg)
{
	if(!arg || !(*arg) )
		return;
	arg = AnyOneArg(arg, g_buf);

	CPC* tpc = gserver.m_playerList.Find(g_buf, false );

	if( !tpc )
		return;
	
	CSkill* skill = new CSkill(gserver.m_skillProtoList.Find(50), 5);
	
	bool bApply;
	ApplySkill(ch, tpc, skill, -1, bApply);

	delete skill;
}

void do_GMRecommend(CPC *ch, const char* arg)
{
#ifdef RECOMMEND_SERVER_SYSTEM
	g_buf[0] = '\0';
	arg = AnyOneArg(arg, g_buf);
	int nServerNo = atoi(g_buf);
	if (nServerNo < 1 && nServerNo != -1)
		return ;
	if (IS_RUNNING_MSGR)
	{
		CNetMsg rmsg;
		MsgrRecommendMsg(rmsg, ch->m_index, nServerNo);
		SEND_Q(rmsg, gserver.m_messenger);
	}
#endif // RECOMMEND_SERVER_SYSTEM
}

void do_GMSetLimit(CPC* ch, const char* arg)
{
#ifdef LIMIT_CATALOG
	g_buf[0] = '\0';
	arg = AnyOneArg(arg, g_buf);
	int ctid = atoi(g_buf);
	arg = AnyOneArg(arg, g_buf);
	int toSell = atoi(g_buf);

	CCatalog* catal;
	catal = gserver.m_catalogList.Find(ctid);
	CNetMsg rmsg;
	if( !catal )
	{
		sprintf(g_buf2, "No Catalog Index: %d", ctid);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf2);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	if( catal->GetToSell() + toSell < 0 || catal->GetLimit() < toSell || catal->GetToSell() + toSell > catal->GetLimit() )
	{
		sprintf(g_buf2, "Not Currect to sell, ctid : %d, number : %d, remain : %d, limit : %d", ctid, toSell, catal->GetToSell(), catal->GetLimit() );
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf2);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	// 메세지 보낸다. 

	if( IS_RUNNING_CONN)
	{
		rmsg.Init(MSG_CONN_REQ);
		rmsg << (unsigned char) MSG_CONN_LIMIT_CATALOG
			 << ctid
			 << toSell;

		SEND_Q(rmsg, gserver.m_connector);
		sprintf(g_buf2, "Now Catalog %s, Additional %d products will be sold from now on.(remain : %d, limit : %d) ", catal->GetName(), toSell, catal->GetToSell(), catal->GetLimit() );
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf2);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		sprintf(g_buf2, "Connector Server Error");
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf2);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
#endif
}

void do_GMDisguise(CPC* ch, const char* /* arg */)
{
	if( ch->m_changeIndex == 301 )
	{
		ch->m_changeIndex = -1;
		ch->m_changePulse = 0;

		ch->ResetPlayerState(PLAYER_STATE_CHANGE);

		CNetMsg rmsg;
		ChangeStopMsg(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}
	else
	{
		ch->m_changeIndex = -1;
		ch->m_changePulse = 0;

		ch->ResetPlayerState(PLAYER_STATE_CHANGE);

		CNetMsg rmsg;
		ChangeStopMsg(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);

		ch->m_changeIndex = 301;
		ch->m_changePulse = INT_MAX;

		ch->SetPlayerState(PLAYER_STATE_CHANGE);

		ChangeStartMsg(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}
}

void do_GMLoginServer(CPC* ch, const char* arg)
{
	if (IS_RUNNING_MSGR)
	{
		CNetMsg rmsg;
		arg = AnyOneArg(arg, g_buf);
		if (strcmp2(g_buf, "open") == 0)			// 1
		{
			MsgrLoginServerMsg(rmsg, 1);
			SEND_Q(rmsg, gserver.m_messenger);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "send open msg to login server");
			SEND_Q(rmsg, ch->m_desc);
		}
		else if (strcmp2(g_buf, "close") == 0)		// 0
		{
			MsgrLoginServerMsg(rmsg, 0);
			SEND_Q(rmsg, gserver.m_messenger);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "send close msg to login server");
			SEND_Q(rmsg, ch->m_desc);
		}
		else if (strcmp2(g_buf, "reboot") == 0)		// 2
		{
			MsgrLoginServerMsg(rmsg, 2);
			SEND_Q(rmsg, gserver.m_messenger);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "send reboot msg to login server");
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

void do_GMEventGoldenBall_End(CPC *ch, const char* arg)
{
	// 이벤트 종료		: /event gball end
	if (IS_RUNNING_MSGR)
	{
		CNetMsg rmsg;
		MsgrEventGoldenballEndMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver.m_messenger);
	}
}

void do_GMEventGoldenBall_Status(CPC *ch, const char* arg)
{
	// 현재 상태 보기	: /event gball status
	//					: 응모중 - A/B팀 이름과 종료일시 알려줌
	//					: 보상중 - A/B팀 이름/점수와 종료일시 알려줌

	CLCString strStatus(1000);
	switch (gserver.m_clGoldenBall.GetStatus())
	{
	case GOLDENBALL_STATUS_VOTE:
		strStatus.Format("GOLDEN BALL VOTE : [%s] vs [%s] : %04d-%02d-%02d %02d:%02d"
			, gserver.m_clGoldenBall.GetTeam1Name()
			, gserver.m_clGoldenBall.GetTeam2Name()
			, gserver.m_clGoldenBall.GetEndYear()
			, gserver.m_clGoldenBall.GetEndMonth()
			, gserver.m_clGoldenBall.GetEndDay()
			, gserver.m_clGoldenBall.GetEndHour()
			, gserver.m_clGoldenBall.GetEndMinute()
			);
		break;

	case GOLDENBALL_STATUS_GIFT:
		strStatus.Format("GOLDEN BALL GIFT : [%s][%d] vs [%s][%d] : %04d-%02d-%02d %02d:%02d"
			, gserver.m_clGoldenBall.GetTeam1Name()
			, gserver.m_clGoldenBall.GetTeam1Score()
			, gserver.m_clGoldenBall.GetTeam2Name()
			, gserver.m_clGoldenBall.GetTeam2Score()
			, gserver.m_clGoldenBall.GetEndYear()
			, gserver.m_clGoldenBall.GetEndMonth()
			, gserver.m_clGoldenBall.GetEndDay()
			, gserver.m_clGoldenBall.GetEndHour()
			, gserver.m_clGoldenBall.GetEndMinute()
			);
		break;

	case GOLDENBALL_STATUS_VOTE_END:
		strStatus.Format("GOLDEN BALL VOTE END : [%s] vs [%s] : %04d-%02d-%02d %02d:%02d"
			, gserver.m_clGoldenBall.GetTeam1Name()
			, gserver.m_clGoldenBall.GetTeam2Name()
			, gserver.m_clGoldenBall.GetEndYear()
			, gserver.m_clGoldenBall.GetEndMonth()
			, gserver.m_clGoldenBall.GetEndDay()
			, gserver.m_clGoldenBall.GetEndHour()
			, gserver.m_clGoldenBall.GetEndMinute()
			);
		break;

	default:
		strStatus = "GOLDEN BALL NOTHING";
		break;
	}

	CNetMsg rmsg;
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", strStatus);
	SEND_Q(rmsg, ch->m_desc);
}

void do_GMEventGoldenBall_Gift(CPC *ch, const char* arg)
{
	// 보상 시작하기	: /event gball gift <team1 score> <team2 score> <year> <month> <day> <hour> <min>
	//					: 현재부터 보상 시작하여 지정된 일시에 보상 종료

	// 1. 현재 상태 검사 : GOLDENBALL_STATUS_VOTE_END
	// 2. 명령어에서 각 데이터 파싱
	// 3. 현재 시간과 입력 시간 비교
	// 4. 메신저로 전달

	// 1. 현재 상태 검사 : GOLDENBALL_STATUS_VOTE_END OR GOLDENBALL_STATUS_GIFT
	if (gserver.m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_VOTE_END && gserver.m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_GIFT)
		return ;

	// 2. 명령어에서 각 데이터 파싱
	int			nTeam1Score;
	int			nTeam2Score;
	int			nYear;
	int			nMonth;
	int			nDay;
	int			nHour;
	int			nMin;

	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	nTeam1Score	= atoi(g_buf);
	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	nTeam2Score	= atoi(g_buf);
	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	nYear		= atoi(g_buf);
	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	nMonth		= atoi(g_buf);
	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	nDay		= atoi(g_buf);
	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	nHour		= atoi(g_buf);
	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	nMin		= atoi(g_buf);

	// 3. 현재 시간과 입력 시간 비교
	time_t tCurrent;
	time(&tCurrent);
	struct tm tmEndGift;
	memset(&tmEndGift, 0, sizeof(tmEndGift));
	tmEndGift.tm_year	= nYear - 1900;
	tmEndGift.tm_mon	= nMonth - 1;
	tmEndGift.tm_mday	= nDay;
	tmEndGift.tm_hour	= nHour;
	tmEndGift.tm_min	= nMin;
	time_t tEndGift = mktime(&tmEndGift);
	if (tEndGift <= tCurrent)
		return ;

	// 4. 메신저로 전달
	if (IS_RUNNING_MSGR)
	{
		CNetMsg rmsg;
		MsgrEventGoldenballGiftMsg(rmsg, ch->m_index, gserver.m_clGoldenBall.GetTeam1Name(), nTeam1Score, gserver.m_clGoldenBall.GetTeam2Name(), nTeam2Score, nYear, nMonth, nDay, nHour, nMin, gserver.m_clGoldenBall.GetVoteEndTime());
		SEND_Q(rmsg, gserver.m_messenger);
	}
}

void do_GMEventGoldenBall_Vote(CPC *ch, const char* arg)
{
	// 응모 시작하기	: /event gball vote <team1> <team2> <year> <month> <day> <hour> <min>
	//					: 현재부터 응모 시작하여 지정된 일시에 응모 종료

	// 1. 현재 상태 검사 : GOLDENBALL_STATUS_NOTHING
	// 2. 명령어에서 각 데이터 파싱
	// 3. 현재 시간과 입력 시간 비교
	// 4. 메신저로 전달

	// 1. 현재 상태 검사 : GOLDENBALL_STATUS_NOTHING
	if (gserver.m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_NOTHING)
		return ;

	// 2. 명령어에서 각 데이터 파싱
	CLCString	strTeam1(GOLDENBALL_TEAM_NAME_LENGTH + 1);
	CLCString	strTeam2(GOLDENBALL_TEAM_NAME_LENGTH + 1);
	int			nYear;
	int			nMonth;
	int			nDay;
	int			nHour;
	int			nMin;

	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	strTeam1	= g_buf;
	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	strTeam2	= g_buf;
	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	nYear		= atoi(g_buf);
	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	nMonth		= atoi(g_buf);
	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	nDay		= atoi(g_buf);
	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	nHour		= atoi(g_buf);
	g_buf[0] = '\0';	arg = AnyOneArg(arg, g_buf);	if (*g_buf == '\0') return ;	nMin		= atoi(g_buf);

	// 3. 현재 시간과 입력 시간 비교
	time_t tCurrent;
	time(&tCurrent);
	struct tm tmEndVote;
	memset(&tmEndVote, 0, sizeof(tmEndVote));
	tmEndVote.tm_year	= nYear - 1900;
	tmEndVote.tm_mon	= nMonth - 1;
	tmEndVote.tm_mday	= nDay;
	tmEndVote.tm_hour	= nHour;
	tmEndVote.tm_min	= nMin;
	time_t tEndVote = mktime(&tmEndVote);
	if (tEndVote <= tCurrent)
		return ;

	// 4. 메신저로 전달
	if (IS_RUNNING_MSGR)
	{
		CNetMsg rmsg;
		MsgrEventGoldenballVoteMsg(rmsg, ch->m_index, strTeam1, strTeam2, nYear, nMonth, nDay, nHour, nMin);
		SEND_Q(rmsg, gserver.m_messenger);
	}
}

void do_GMEventGoldenBall(CPC *ch, const char* arg)
{
	// 응모 시작하기	: /event gball vote <team1> <team2> <year> <month> <day> <hour> <min>
	//					: 현재부터 응모 시작하여 지정된 일시에 응모 종료
	// 보상 시작하기	: /event gball gift <team1 score> <team2 score> <year> <month> <day> <hour> <min>
	//					: 현재부터 보상 시작하여 지정된 일시에 보상 종료
	// 현재 상태 보기	: /event gball status
	//					: 응모중 - A/B팀 이름과 종료일시 알려줌
	//					: 보상중 - A/B팀 이름/점수와 종료일시 알려줌
	// 이벤트 종료		: /event gball end
	g_buf[0] = '\0';
	arg = AnyOneArg(arg, g_buf);
	CLCString strSubCommand = g_buf;

	if (strcmp(strSubCommand, "vote") == 0)
	{
		do_GMEventGoldenBall_Vote(ch, arg);
	}
	else if (strcmp(strSubCommand, "gift") == 0)
	{
		do_GMEventGoldenBall_Gift(ch, arg);
	}
	else if (strcmp(strSubCommand, "status") == 0)
	{
		do_GMEventGoldenBall_Status(ch, arg);
	}
	else if (strcmp(strSubCommand, "end") == 0)
	{
		do_GMEventGoldenBall_End(ch, arg);
	}
	else
		return ;
}

void do_GMEventOXQuiz_Quiz(CPC *ch, const char* arg)
{
	g_buf[0] = '\0';
	arg = AnyOneArg(arg, g_buf);
	if (!g_buf[0])		return ;
	int nIndex = atoi(g_buf);

	g_buf[0] = '\0';
	arg = AnyOneArg(arg, g_buf);
	int nSec = (g_buf[0]) ? atoi(g_buf) : 20;

	// 메신저로 모든 서버군에 전달한다
	if (IS_RUNNING_MSGR)
	{
		CNetMsg rmsg;
		MsgrEventOXQuizMsg(rmsg, ch->m_index, nIndex, nSec);
		SEND_Q(rmsg, gserver.m_messenger);
	}
}

void do_GMEventOXQuiz_End(CPC *ch, const char* arg)
{
	// 메신저로 모든 서버군에 전달한다
	if (IS_RUNNING_MSGR)
	{
		CNetMsg rmsg;
		MsgrEventOXEndMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver.m_messenger);
	}
}

void do_GMEventOXQuiz_Start(CPC *ch, const char* arg)
{
	// 시작 시간 구하기
	struct tm tmStart;
	int	year, mon, day, hour, min;
	memset(&tmStart, 0, sizeof(tmStart));

	g_buf[0] = '\0';
	arg = AnyOneArg(arg, g_buf);
	year = atoi(g_buf) - 1900;

	g_buf[0] = '\0';
	arg = AnyOneArg(arg, g_buf);
	mon = atoi(g_buf) - 1;

	g_buf[0] = '\0';
	arg = AnyOneArg(arg, g_buf);
	day = atoi(g_buf);

	g_buf[0] = '\0';
	arg = AnyOneArg(arg, g_buf);
	hour = atoi(g_buf);

	g_buf[0] = '\0';
	arg = AnyOneArg(arg, g_buf);
	min = atoi(g_buf);

	tmStart.tm_year = year;
	tmStart.tm_mon  = mon;
	tmStart.tm_mday = day;
	tmStart.tm_hour = hour;
	tmStart.tm_min  = min;

	time_t timeStart = mktime(&tmStart);

	// 현재 시간과 비교
	time_t timeCurrent;
	time(&timeCurrent);
	if (timeCurrent >= timeStart)
		return ;

	// 메신저로 현재 서브 채널의 존서버에 모두 전달
	if (IS_RUNNING_MSGR)
	{
		CNetMsg rmsg;
		MsgrEventOXSetStartMsg(rmsg, ch->m_index, year + 1900, mon + 1, day, hour, min);
		SEND_Q(rmsg, gserver.m_messenger);
	}
}

void do_GMEventOXQuiz(CPC *ch, const char* arg)
{
	// 시작하기		: oxevent start <year> <mon> <day> <hour> <min>
	//				: 바로 입장 가능하며 min분후 이벤트 시작, 해당 접속 채널에만 적용
	// 종료하기		: oxevent end
	//				: oxevent 가 바로 종료되며, 존안의 유저들은 모두 주노마을로 이동한다., 전체 서버군 적용
	// 문제 내기	: oxevent quiz <num> <sec>
	//				: num에 해당하는 퀴즈 제시, 제한시간 sec 초, sec이 없으면 기본20초, 전체 서버군 적용
	g_buf[0] = '\0';
	arg = AnyOneArg(arg, g_buf);
	CLCString strSubCommand = g_buf;

	if (strcmp(strSubCommand, "start") == 0)
	{
		do_GMEventOXQuiz_Start(ch, arg);
	}
	else if (strcmp(strSubCommand, "end") == 0)
	{
		do_GMEventOXQuiz_End(ch, arg);
	}
	else if (strcmp(strSubCommand, "quiz") == 0)
	{
		do_GMEventOXQuiz_Quiz(ch, arg);
	}
	else
		return ;
}

void do_GMEventValentine2006(CPC *ch, const char* arg)
{
	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);

	TrimString(g_buf);

	CNetMsg rmsg;

	if (strcmp(g_buf, "start") == 0)
		MsgrEventReqMsg(rmsg, -1, -1, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, "valentine2006", 1);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrEventReqMsg(rmsg, -1, -1, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, "valentine2006", 0);
	else if (strcmp(g_buf, "ing") == 0)
		MsgrEventReqMsg(rmsg, -1, -1, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, "valentine2006", 2);

	if (IS_RUNNING_MSGR)
	{
		SEND_Q(rmsg, gserver.m_messenger);
	}
	else
	{
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not running Messenger Server..");
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMEventXMas2005(CPC *ch, const char* arg)
{
	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);

	TrimString(g_buf);

	CNetMsg rmsg;

	if (strcmp(g_buf, "start") == 0)
		MsgrEventReqMsg(rmsg, -1, -1, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, "xmas2005", 1);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrEventReqMsg(rmsg, -1, -1, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, "xmas2005", 0);
	else if (strcmp(g_buf, "ing") == 0)
		MsgrEventReqMsg(rmsg, -1, -1, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, "xmas2005", 2);

	if (IS_RUNNING_MSGR)
	{
		SEND_Q(rmsg, gserver.m_messenger);
	}
	else
	{
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not running Messenger Server..");
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMEventNewYear2006(CPC *ch, const char* arg)
{
	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);

	TrimString(g_buf);

	CNetMsg rmsg;

	if (strcmp(g_buf, "start") == 0)
		MsgrEventReqMsg(rmsg, -1, -1, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, "newyear2006", 1);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrEventReqMsg(rmsg, -1, -1, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, "newyear2006", 0);
	else if (strcmp(g_buf, "ing") == 0)
		MsgrEventReqMsg(rmsg, -1, -1, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, "newyear2006", 2);

	if (IS_RUNNING_MSGR)
	{
		SEND_Q(rmsg, gserver.m_messenger);
	}
	else
	{
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not running Messenger Server..");
		SEND_Q(rmsg, ch->m_desc);
	}
}

// event itemdrop <serverno | all | cur> <start rate | ing | stop>
// rate : 100 ~ 200 [%]
//		: 0 - stop
//		: -1 - ing
void do_GMEventItemDrop(CPC* ch, const char* arg)
{
	CNetMsg rmsg;

	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group

	// 대상 서버군 정하기
	g_buf[0] = '\0';
	TrimString(g_buf);
	arg = AnyOneArg(arg, g_buf, true);
	if (g_buf[0] == '\0') goto INVALID_FORMAT;
	if (strcmp(g_buf, "all") == 0)
		target = -1;
	else if (strcmp(g_buf, "cur") == 0)
		target = gserver.m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(g_buf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	g_buf[0] = '\0';
	TrimString(g_buf);
	arg = AnyOneArg(arg, g_buf, true);
	if (g_buf[0] == '\0') goto INVALID_FORMAT;

	if (strcmp(g_buf, "start") == 0)
	{
		// rate 정하기
		g_buf[0] = '\0';
		TrimString(g_buf);
		arg = AnyOneArg(arg, g_buf, true);
		if (g_buf[0] == '\0') goto INVALID_FORMAT;
		int rate = atoi(g_buf);
		if (rate < 100 || rate > 200)
			goto INVALID_FORMAT;

		MsgrEventReqMsg(rmsg, target, -1, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, "itemdrop", rate);
	}
	else if (strcmp(g_buf, "ing") == 0)
	{
		MsgrEventReqMsg(rmsg, target, -1, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, "itemdrop", -1);
	}
	else if (strcmp(g_buf, "stop") == 0)
	{
		MsgrEventReqMsg(rmsg, target, -1, -1, gserver.m_serverno, gserver.m_subno, ch->m_index, "itemdrop", 0);
	}
	else
		goto INVALID_FORMAT;

	if (IS_RUNNING_MSGR)
	{
		SEND_Q(rmsg, gserver.m_messenger);
	}
	else
	{
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
		SEND_Q(rmsg, ch->m_desc);
	}

	return ;

INVALID_FORMAT:
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: event itemdrop <server-group no | all | cur> <start <rate> | ing | stop>");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "rate[%]: 100 ~ 200");
	SEND_Q(rmsg, ch->m_desc);
}

void do_GMEvent(CPC* ch, const char* arg)
{
	g_buf[0] = '\0';
	const char* p = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);

	if (strcmp(g_buf, "itemdrop") == 0)
		do_GMEventItemDrop(ch, p);

	else if (strcmp(g_buf, "xmas2005") == 0)
		do_GMEventXMas2005(ch, p);

	else if (strcmp(g_buf, "newyear2006") == 0)
		do_GMEventNewYear2006(ch, p);

	else if (strcmp(g_buf, "valentine2006") == 0)
		do_GMEventValentine2006(ch, p);

	else if (strcmp(g_buf, "oxevent") == 0)
		do_GMEventOXQuiz(ch, p);

	else if (strcmp(g_buf, "gball") == 0)
		do_GMEventGoldenBall(ch, p);
}

#ifdef JP_OTAKU_SYSTEM
void do_OTAKUSpeedUp(CPC* ch, const char* /*arg*/)
{
	if (ch->m_speedup == 0)
		ch->m_speedup = 120;
	else
		ch->m_speedup = 0;
	ch->CalcStatus(true);
}

void do_OTAKUImmortal(CPC* ch, const char* /* arg */)
{
	CLCString chat(100);
	
	if (ch->m_bImmortal)
	{
		ch->m_bImmortal = false;
		chat.Format("%s : Off", (const char*)gserver.m_strGMCommandOTAKUImmortal);
	}
	else
	{
		ch->m_bImmortal = true;
		DelAttackList(ch);
		chat.Format("%s : On", (const char*)gserver.m_strGMCommandOTAKUImmortal);
	}
	
	CNetMsg msg;
	SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", chat);
	SEND_Q(msg, ch->m_desc);

}

#endif //  JP_OTAKU_SYSTEM




void do_GMKickCharIndex(CPC* ch, const char* arg)
{
	if (!arg || !(*arg) ) return;
	arg = AnyOneArg( arg, g_buf );
	int CharIndex = atoi( g_buf );

	if (CharIndex < 1 )
		return;

	CPC* tch = gserver.m_playerList.Find( CharIndex );
	if ( tch )
	{
		CNetMsg msg;
		MsgrLogoutReqMsg( msg, -1, gserver.m_serverno, gserver.m_subno, -1, tch->m_desc->m_idname );

		if( IS_RUNNING_MSGR )
			SEND_Q( msg, gserver.m_messenger );

		// 보냈다는 메세지
		strcpy(g_buf, "Sending KickCharName: ");
		strcat(g_buf, tch->m_nick);


		CNetMsg rmsg;
		SayMsg( rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf);
		SEND_Q( rmsg, ch->m_desc );
	}
	else
	{
		//없으면 메세지
//		sprintf(g_buf2, "Not Fount PC: %s", tch->m_nick);

		strcpy( g_buf, "Not Found PC: " );
		strcat( g_buf, tch->m_nick );
		CNetMsg rmsg;
		SayMsg( rmsg, MSG_CHAT_NOTICE, 0, "", "", g_buf );
		SEND_Q( rmsg, ch->m_desc );
	}
}


void do_GMEventShow(CPC* ch, const char* arg)
{
#if defined ( EVENT_FLOWERTREE_2007 ) || defined (EVENT_2007_PARENTSDAY)
	arg = AnyOneArg(arg, g_buf2);
	int eventid = atoi(g_buf2);
	
	arg = AnyOneArg(arg, g_buf2);
	CLCString strName(g_buf2);

	arg = AnyOneArg(arg, g_buf2);
	int itemIndex = atoi(g_buf2);
	switch ( eventid )
	{
#ifdef EVENT_FLOWERTREE_2007
	case 37 :		// EVENT_FLOWERTREE_2007
		{
			CNetMsg rmsg;
			HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_PRICEITEM_INSERT, 0 );
			rmsg << (unsigned char) 0;			// ISGM = 0;
			rmsg << strName;
			rmsg << itemIndex;
			SEND_Q( rmsg, gserver.m_helper );						
		} break;
#elif defined (  EVENT_2007_PARENTSDAY )// EVENT_FLOWERTREE_2007
	case 34 :		// EVENT_2007_PARENTSDAY
		{
			CNetMsg rmsg;
			HelperEventParentsdayExchangeItemInsert( rmsg, 0, itemIndex, 0 );
			rmsg << strName;			
			SEND_Q( rmsg, gserver.m_helper );
		} break;
#endif // EVENT_2007_PARENTSDAY
	}
#endif // #if defined ( EVENT_FLOWERTREE_2007 ) || defined (EVENT_2007_PARENTSDAY)
}

void do_GMEventSetting(CPC* ch, const char* arg)
{
#ifdef EVENTSETTING
	CNetMsg rmsg;
	CLCString sql(1024);

	g_buf[0] = '\0';
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);

	if(strcmp(g_buf, "reload") == 0)
	{
		// 1-1-0	flag: 0 이벤트 세팅 요청
		MsgEventSetReqMsg(rmsg, 1, 1, 0, gserver.m_serverno, gserver.m_subno, ch->m_index, 0, -1);

		if (IS_RUNNING_MSGR)
		{
			SEND_Q(rmsg, gserver.m_messenger);
		}
		else
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}
	else if(strcmp(g_buf, "current")  == 0)
	{
		MsgEventSetReqMsg(rmsg, 1, 1, 0, gserver.m_serverno, gserver.m_subno, ch->m_index, 2, -1);

		if (IS_RUNNING_MSGR)
		{
			SEND_Q(rmsg, gserver.m_messenger);
		}
		else
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}
	else
	{
		goto INVALID_FORMAT;
	}

	
INVALID_FORMAT:
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: eventsetting <reload>");
	SEND_Q(rmsg, ch->m_desc);
#endif // EVENTSETTING
}

void do_GMChanceEvent(CPC* ch, const char* arg)
{
#ifdef CHANCE_EVENT
	CNetMsg rmsg;
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group
	int slevel = -1, elevel = -1;

	// 대상 서버군 정하기
	if (!arg || !(*arg))
	{
		goto INVALID_FORMAT;
	}

	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "all") == 0)
	{
		target = -1;
	}
	else if (strcmp(g_buf, "cur") == 0)
	{
		target = gserver.m_serverno;
	}
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
			{
				goto INVALID_FORMAT;
			}
			p++;
		}
		target = atoi(g_buf);
		if (target < 1)
		{
			goto INVALID_FORMAT;
		}
	}

	// 명령 정하기
	if (!arg || !(*arg))
	{
		goto INVALID_FORMAT;
	}

	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);

	int percent[6];
	memset(percent, -1, sizeof(percent) );
	
	if (strcmp(g_buf, "start") == 0)
	{
		// 레벨 정하기
		if (!arg || !(*arg))
		{
			goto INVALID_FORMAT;
		}

		arg = AnyOneArg(arg, g_buf, true);
		TrimString(g_buf);

		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
			{
				goto INVALID_FORMAT;
			}
			p++;
		}
		slevel = atoi(g_buf);
		if (slevel < 1)
		{
			goto INVALID_FORMAT;
		}

		if (!arg || !(*arg))
		{
			goto INVALID_FORMAT;
		}

		arg = AnyOneArg(arg, g_buf, true);
		TrimString(g_buf);

		p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
			{
				goto INVALID_FORMAT;
			}
			p++;
		}
		elevel = atoi(g_buf);
		if (elevel < 1)
		{
			goto INVALID_FORMAT;
		}

		if (slevel > elevel)
		{
			goto INVALID_FORMAT;
		}

		for( int i = 0; i < 6; i ++)
		{
			arg = AnyOneArg(arg, g_buf, true);
			TrimString(g_buf);		

			percent[i] = atoi(g_buf);

			if( i == 5 && (percent[i] < 0 || percent[i] > 10) )
			{
				goto INVALID_FORMAT;
			}

			if( percent[i] < 0 || percent[i] > 200)
			{
				goto INVALID_FORMAT;
			}
		}

		MsgrChanceEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, slevel, elevel, percent);
	}
	else if (strcmp(g_buf, "ing") == 0)
	{
		MsgrChanceEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, slevel, elevel, percent);
	}
	else if (strcmp(g_buf, "stop") == 0)
	{
		MsgrChanceEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, slevel, elevel, percent);
	}
	else
	{
		goto INVALID_FORMAT;
	}

	if (IS_RUNNING_MSGR) 
	{
		SEND_Q(rmsg, gserver.m_messenger);
	}

	return;

INVALID_FORMAT:
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: chance_event <server-group no | all | cur> <start | ing | stop> <beginlevel> <endlevel> <nasdrop> <nasget> <exp> <sp> <produce> <producenum> ");
	SEND_Q(rmsg, ch->m_desc);

#endif // CHANCE_EVENT
}

void do_GMToolChatMonitor(CPC* ch, const char* arg )
{
#ifdef GMTOOL
	int serverno, subno;
	char onoff[100], name[100];
	char* p1;
	if(!(ch->m_desc->m_userFlag & IS_GM_CHAR))
	{
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "FAILED(Only GMTOOL)");
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (!arg || !*arg)
		return ;
	*g_buf = '\0';
	// 이름
	arg = AnyOneArg(arg, g_buf);
	TrimString(g_buf);
	if (strlen(g_buf) < 1)
		return ;
	strcpy(name, g_buf);

	if (!arg || !(*arg) ) 
	{
		goto INVALID_FORMAT;
		return;
	}
	// ON or OFF
	arg = AnyOneArg(arg, g_buf);
	TrimString(g_buf);
	if(strcmp(g_buf, "on") != 0 && strcmp(g_buf, "off") != 0)
	{
		goto INVALID_FORMAT;
		return ;
	}
	strcpy(onoff, g_buf);


	if (!arg || !(*arg) ) 
		return;

	// server, sub
	arg = AnyOneArg(arg, g_buf);
	TrimString(g_buf);
	p1 = g_buf;
	while (*p1)
	{
		if (!isdigit(*p1))
			return ;		//goto INVALID_FORMAT;
		p1++;
	}
	serverno = atoi(g_buf);

	if (!arg || !(*arg) ) 
		return;

	arg = AnyOneArg(arg, g_buf);
	TrimString(g_buf);
	p1 = g_buf;
	while (*p1)
	{
		if (!isdigit(*p1))
			return ;		//goto INVALID_FORMAT;
		p1++;
	}
	subno = atoi(g_buf);

	if(IS_RUNNING_MSGR)
	{
		CNetMsg msg;
		MsgrGmToolChatMonitorReqMsg(msg, 0, serverno, subno, -1, ch->m_index, name, onoff);
		SEND_Q(msg, gserver.m_messenger);

		sprintf(g_buf, "Sending Chatmonitor %s %s", name, onoff);
		GMSayMsg(msg, 0, 0, 0, "", g_buf, (unsigned char)MSG_CHAT_GM_SAY);
		SEND_Q(msg, ch->m_desc);
	}
	return ;

INVALID_FORMAT:
	CNetMsg rmsg;
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: gmchatmonitor <name> <on | off>");
	SEND_Q(rmsg, ch->m_desc);
#endif // GMTOOL
}


void do_GMDoubleItemEvent(CPC* ch, const char* arg)
{
#ifdef DOUBLE_ITEM_DROP
	CNetMsg rmsg;
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "all") == 0)
		target = -1;
	else if (strcmp(g_buf, "cur") == 0)
		target = gserver.m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(g_buf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "start") == 0)
	{
		arg = AnyOneArg(arg, g_buf, true);
		TrimString(g_buf);

		///// yhj  081014  ///////////////////// 확률 제한해제
		//if( atoi(g_buf) < 0 || atoi(g_buf) > 200)
		//	goto INVALID_FORMAT;
		///////////////////////////////////////

		MsgrDoubleItemEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, atoi(g_buf));
	}
	else if (strcmp(g_buf, "ing") == 0)
		MsgrDoubleItemEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, -1);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrDoubleItemEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, -1);
	else
		goto INVALID_FORMAT;

	if (IS_RUNNING_MSGR)
	{
		SEND_Q(rmsg, gserver.m_messenger);
	}
	else
	{
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
		SEND_Q(rmsg, ch->m_desc);
	}

	return ;

INVALID_FORMAT:
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: doubleitem_event <server-group no | all | cur> <start | ing | stop>");
	SEND_Q(rmsg, ch->m_desc);
#endif // DOUBLE_ITEM_DROP
}


void do_GMDoublePetExpEvent(CPC* ch, const char* arg)
{
#ifdef DOUBLE_PET_EXP
	CNetMsg rmsg;
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "all") == 0)
		target = -1;
	else if (strcmp(g_buf, "cur") == 0)
		target = gserver.m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(g_buf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "start") == 0)
	{
		arg = AnyOneArg(arg, g_buf, true);
		TrimString(g_buf);

		if( atoi(g_buf) < 0 || atoi(g_buf) > 200)
			goto INVALID_FORMAT;

		MsgrDoublePetExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, atoi(g_buf));
	}
	else if (strcmp(g_buf, "ing") == 0)
		MsgrDoublePetExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, -1);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrDoublePetExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, -1);
	else
		goto INVALID_FORMAT;

	if (IS_RUNNING_MSGR)
	{
		SEND_Q(rmsg, gserver.m_messenger);
	}
	else
	{
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
		SEND_Q(rmsg, ch->m_desc);
	}

	return ;

INVALID_FORMAT:
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: doublepetexp_event <server-group no | all | cur> <start | ing | stop>");
	SEND_Q(rmsg, ch->m_desc);
#endif // DOUBLE_PET_EXP
}


void do_GMDoubleAttackEvent(CPC* ch, const char* arg)
{
#ifdef DOUBLE_ATTACK
	CNetMsg rmsg;
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "all") == 0)
		target = -1;
	else if (strcmp(g_buf, "cur") == 0)
		target = gserver.m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(g_buf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "start") == 0)
	{
		arg = AnyOneArg(arg, g_buf, true);
		TrimString(g_buf);

		if( atoi(g_buf) < 0 || atoi(g_buf) > 200)
			goto INVALID_FORMAT;

		MsgrDoubleAttackEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, atoi(g_buf));
	}
	else if (strcmp(g_buf, "ing") == 0)
		MsgrDoubleAttackEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, -1);
	else if (strcmp(g_buf, "stop") == 0)
		MsgrDoubleAttackEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, -1);
	else
		goto INVALID_FORMAT;

	if (IS_RUNNING_MSGR)
	{
		SEND_Q(rmsg, gserver.m_messenger);
	}
	else
	{
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
		SEND_Q(rmsg, ch->m_desc);
	}

	return ;

INVALID_FORMAT:
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: doubleattack_event <server-group no | all | cur> <start | ing | stop>");
	SEND_Q(rmsg, ch->m_desc);
#endif // DOUBLE_ATTACK
}

void do_GMDropItemEvent(CPC* ch, const char* arg)
{
#ifdef EVENT_DROPITEM
	CNetMsg rmsg;
	int target = -1;
	int npcidx, itemidx, prob;
	npcidx = -1;
	itemidx = -1;
	prob = 0;

	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "all") == 0)
		target = -1;
	else if (strcmp(g_buf, "cur") == 0)
		target = gserver.m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(g_buf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);

	if (strcmp(g_buf, "start") == 0)
	{
		// npcidx
		arg = AnyOneArg(arg, g_buf);
		char* p;
		p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		npcidx = atoi(g_buf);

		CNPCProto* npcproto = NULL;
		npcproto = gserver.m_npcProtoList.FindProto(npcidx);
		if(!npcproto)
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not FOUND NPC");
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// itemidx
		arg = AnyOneArg(arg, g_buf);
		p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		itemidx = atoi(g_buf);

		CItem* item = gserver.m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);
		if(!item)
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not FOUND ITEM");
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		delete item;
		item = NULL;

		// prob
		arg = AnyOneArg(arg, g_buf);
		p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}

		prob = atoi(g_buf);
		if(prob <= 0 || prob > 100)
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "SELECT PROB(1 ~ 100)");
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		MsgrEventDropItemReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, npcidx, itemidx, prob, gserver.m_serverno, gserver.m_subno);  
	}
	else if (strcmp(g_buf, "ing") == 0)
	{
		MsgrEventDropItemReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, npcidx, itemidx, prob, gserver.m_serverno, gserver.m_subno);  
	}
	else if (strcmp(g_buf, "stop") == 0)
	{
		MsgrEventDropItemReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, npcidx, itemidx, prob, gserver.m_serverno, gserver.m_subno);  
	}
	else
		goto INVALID_FORMAT;

	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);

	return;

INVALID_FORMAT:
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: npcdropitem_event <server-group no | all | cur> <start | ing | stop>");
	SEND_Q(rmsg, ch->m_desc);
#endif // EVENT_DROPITEM
}

void do_GMSilenceMulti(CPC* ch, const char* arg)
{
#ifdef SILENCE_PC_MULTI
	int distance, sec;
	char* p1;
	p1 = NULL;

	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);
	TrimString(g_buf);
	p1 = g_buf;
	while (*p1)
	{
		if (!isdigit(*p1))
			return ;		//goto INVALID_FORMAT;
		p1++;
	}
	distance = atoi(g_buf);

	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);
	TrimString(g_buf);
	p1 = g_buf;
	while (*p1)
	{
		if (!isdigit(*p1))
			return ;		//goto INVALID_FORMAT;
		p1++;
	}
	sec = atoi(g_buf);

	if(distance <= 0 || time <= 0)
		return ;

	int sx = ch->m_cellX - 2;
	int ex = ch->m_cellX + 2;
	int sz = ch->m_cellZ - 2;
	int ez = ch->m_cellZ + 2;
	
	int x, z;
	for(x = sx ; x <= ex ; ++x)
	{
		if (x < 0 || x >= ch->m_pArea->m_size[0])
			continue;

		for(z = sz ; z <= ez ; ++z)
		{
			if (z < 0 || z >= ch->m_pArea->m_size[1])
				continue;

			CCharacter* tch;
			CCharacter* tchNext = ch->m_pArea->m_cell[x][z].m_listChar;
			while((tch = tchNext))
			{
				tchNext = tchNext->m_pCellNext;

				if( IS_PC(tch) && (tch != ch) )
				{
					CPC* pc = TO_PC(tch);
					if (GetDistance(ch, pc) < distance)
					{
						pc->m_silencePulse = gserver.m_pulse + (sec * PULSE_REAL_SEC);
					}
				}
			}
		}
	}

#endif // SILENCE_PC_MULTI
}

void do_GMKickMulti(CPC* ch, const char* arg)
{
#ifdef KICK_PC_MULTI
	if (!arg || !(*arg))		return ;

	char* p1;
	int distance;
	arg = AnyOneArg(arg, g_buf);
	TrimString(g_buf);
	p1 = g_buf;
	while (*p1)
	{
		if (!isdigit(*p1))
			return ;		//goto INVALID_FORMAT;
		p1++;
	}
	distance = atoi(g_buf);

	if(distance <= 0)
		return ;

	int sx = ch->m_cellX - 2;
	int ex = ch->m_cellX + 2;
	int sz = ch->m_cellZ - 2;
	int ez = ch->m_cellZ + 2;
	
	int x, z;
	for(x = sx ; x <= ex ; ++x)
	{
		if (x < 0 || x >= ch->m_pArea->m_size[0])
			continue;

		for(z = sz ; z <= ez ; ++z)
		{
			if (z < 0 || z >= ch->m_pArea->m_size[1])
				continue;

			CCharacter* tch;
			CCharacter* tchNext = ch->m_pArea->m_cell[x][z].m_listChar;
			while((tch = tchNext))
			{
				tchNext = tchNext->m_pCellNext;

				if( IS_PC(tch) && (tch != ch) )
				{
					CPC* pc = TO_PC(tch);
					if (GetDistance(ch, pc) < distance)
					{
						CNetMsg rmsg;
						if (IS_RUNNING_MSGR)
						{
							MsgrLogoutReqMsg(rmsg, -1, -1, -1, -1, pc->m_desc->m_idname);
							SEND_Q(rmsg, gserver.m_messenger);
						}
					}
				}
			}
		}
	}
#endif // KICK_PC_MULTI
}

void do_GMExpLimit( CPC* ch, const char* arg )
{
	CNetMsg rmsg;
	int target = -1;
	int nLimit = 250;

	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "all") == 0)
		target = -1;
	else if (strcmp(g_buf, "cur") == 0)
		target = gserver.m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(g_buf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);

	nLimit = atoi( g_buf );
	if ( nLimit <0 || nLimit > 3000 )
		goto INVALID_FORMAT;

	MsgrSetExpLimitMsg( rmsg, target, ch->m_index, nLimit );
	SEND_Q(rmsg, gserver.m_messenger);
	return;

INVALID_FORMAT:
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: exp_limit <server-group no | all | cur> exp_limit_percent");
	SEND_Q(rmsg, ch->m_desc);

}


void do_GMDoubleEventAuto(CPC* ch, const char* arg)
{
#ifdef NEW_DOUBLE_EVENT_AUTO
	CNetMsg rmsg;
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group
	int	zonenum;			// 대상 존 번호: -1: all, 0 ~ : zonenum
	zonenum = -1;

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "all") == 0)
		target = -1;
	else if (strcmp(g_buf, "cur") == 0)
		target = gserver.m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(g_buf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);

	if (strcmp(g_buf, "start") == 0)
	{
		MsgrDoubleEventAutoReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0);
	}
	else if(strcmp(g_buf, "ing") == 0)
	{
		MsgrDoubleEventAutoReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1);
	}
	else if(strcmp(g_buf, "stop") == 0)
	{
		MsgrDoubleEventAutoReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2);
	}
	else
		goto INVALID_FORMAT;

	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);

	return ;
INVALID_FORMAT:
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
	SEND_Q(rmsg, ch->m_desc);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: double_event_auto <server-group no | all | cur> < start | ing | stop >");
	SEND_Q(rmsg, ch->m_desc);
#endif // NEW_DOUBLE_EVENT_AUTO
}

void do_GMUpgradeEvent(CPC* ch, const char* arg)
{
#ifdef UPGRADE_EVENT
	CNetMsg rmsg;
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group
	int prob = 0;

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);
	if (strcmp(g_buf, "all") == 0)
		target = -1;
	else if (strcmp(g_buf, "cur") == 0)
		target = gserver.m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(g_buf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, g_buf, true);
	TrimString(g_buf);

	if (strcmp(g_buf, "start") == 0)
	{
		if (!arg || !(*arg))
			goto INVALID_FORMAT;
		arg = AnyOneArg(arg, g_buf, true);
		TrimString(g_buf);
		
		char* p = g_buf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}

		prob = atoi( g_buf );
		switch(prob)
		{
		case 100:
		case 125:
		case 150:
		case 175:
		case 200:
			break;
		default:
			goto INVALID_FORMAT;
		}
		MsgrUpgradeEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, prob);

	}
	else if(strcmp(g_buf, "ing") == 0)
	{
		MsgrUpgradeEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, prob);
	}
	else if(strcmp(g_buf, "stop") == 0)
	{
		MsgrUpgradeEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, prob);
	}
	else
		goto INVALID_FORMAT;

	if (IS_RUNNING_MSGR) SEND_Q(rmsg, gserver.m_messenger);

	return ;

INVALID_FORMAT:
	;
#endif // UPGRADE_EVENT
}


void do_GMKickGuildMember(CPC* ch, const char* arg)
{
//#ifdef ADMIN_KICKGUILDMEM
	CNetMsg rmsg;
	CGuild* guild;
	CGuildMember* guildmember;
	CGuildMember* boss;
	char guildname[MAX_GUILD_NAME_LENGTH + 1], charname[MAX_CHAR_NAME_LENGTH + 1];

	if (!arg || !(*arg))
		return ;
				
	arg = AnyOneArg(arg, g_buf);
	TrimString(g_buf);
	
	if (strlen(g_buf) < 1)
		return ;
	strcpy(guildname, g_buf);

	arg = AnyOneArg(arg, g_buf);
	TrimString(g_buf);
	
	if (strlen(g_buf) < 1)
		return ;

	strcpy(charname, g_buf);

	guild = gserver.m_guildlist.findguild(guildname);
	if(!guild)
		return ;

	guildmember = guild->findmember(charname);
	if(!guildmember)
		return ;

	boss = guild->boss();
	if(!boss)
		return ;

	if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_KICK_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (guildmember->pos() == MSG_GUILD_POSITION_BOSS)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANTKICKBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	

	if (IS_RUNNING_HELPER)
	{
		CPC* bosspc;
		bosspc = gserver.m_playerList.Find(boss->charindex());
		if(bosspc)
		{
			if(bosspc->m_desc->m_waithelper)
				return ;
			else
				bosspc->m_desc->WaitHelperReply(true, 13);
		}

		HelperGuildKickReqMsg(rmsg, guild->index(), boss->charindex(), guildmember->charindex());
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
//#endif // ADMIN_KICKGUILDMEM
}


void do_GMApplySkill(CPC* ch, const char* arg)
{
	int skillIndex = 0;
	int skillLevel = 1;
	CPC* targetCh = ch;
	bool bcure = false;

	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, g_buf);
	skillIndex = atoi(g_buf);
	if( skillIndex == 0 && strcmp("cure",g_buf)==0 )
	{
		bcure = true;
		arg = AnyOneArg(arg, g_buf);
		skillIndex = atoi(g_buf);
	}

	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, g_buf);
		skillLevel = atoi(g_buf);

		if (arg && (*arg))
		{
			arg = AnyOneArg(arg, g_buf);
			CLCString name = g_buf;
			name.Trim();

			if (g_buf[0] != '\0')
			{
				// 전체 유저 중에
				int i;
				for (i = 0; i < gserver.m_playerList.m_max; i++)
				{
					CPC* tpc = gserver.m_playerList.m_pcList[i];

					// 유저 검사
					if (tpc && ( strcmp(tpc->m_name, name) == 0 || strcmp(tpc->m_nick, name) == 0))
					{
						targetCh = tpc;
						break;
					}
					else
					{
						targetCh = NULL;
					}
				}
			}
		}
	}

	if( bcure && targetCh )
	{
		targetCh->m_assist.CureBySkillIndex( skillIndex );
		return;
	}

	CSkill * pSkill = gserver.m_skillProtoList.Create(skillIndex,skillLevel);
	if( pSkill == NULL )
	{
		return;		
	}

	bool bApply;
	ApplySkill(ch, targetCh, pSkill, -1, bApply);
	if( !bApply && pSkill )
	{
		delete pSkill;
		pSkill = NULL;
	}
}