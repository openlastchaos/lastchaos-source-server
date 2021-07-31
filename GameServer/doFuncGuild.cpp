#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "DBCmd.h"
#include "WarCastle.h"

void do_Guild(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype;
	msg.MoveFirst();
	msg >> subtype;

#ifdef ENABLE_WAR_CASTLE
	if (!IsValidGuildCommandWithWarCastle(ch, subtype))
	{
		CNetMsg rmsg;

		GuildWarInvalidCommandMsg(rmsg, (MSG_GUILD_TYPE)subtype);
		SEND_Q(rmsg, ch->m_desc);

		return ;
	}
#endif

	switch (subtype)
	{
	case MSG_GUILD_CREATE:
		do_GuildCreate(ch, msg);
		break;

	case MSG_GUILD_LEVELUP:
		do_GuildLevelUp(ch, msg);
		break;

	case MSG_GUILD_BREAKUP:
		do_GuildBreakUp(ch, msg);
		break;

	case MSG_GUILD_REGIST_REQ:
		do_GuildRegistReq(ch, msg);
		break;

	case MSG_GUILD_REGIST_ALLOW:
		do_GuildRegistAllow(ch, msg);
		break;

	case MSG_GUILD_REGIST_CANCEL:
		do_GuildRegistCancel(ch, msg);
		break;

	case MSG_GUILD_OUT_REQ:
		do_GuildOutReq(ch, msg);
		break;

	case MSG_GUILD_KICK:
		do_GuildKick(ch, msg);
		break;

	case MSG_GUILD_CHANGE_BOSS:
		do_GuildChangeBoss(ch, msg);
		break;

	case MSG_GUILD_APPOINT_OFFICER:
		do_GuildAppointOfficer(ch, msg);
		break;

	case MSG_GUILD_FIRE_OFFICER:
		do_GuildFireOfficer(ch, msg);
		break;

		// 길드전
	case MSG_GUILD_BATTLE_REQ_REQ:
		do_GuildBattleReqReq(ch, msg);
		break;

	case MSG_GUILD_BATTLE_REQ_REJECT:
		do_GuildBattleReqReject(ch, msg);
		break;

	case MSG_GUILD_BATTLE_REQ_ACCEPT:
		do_GuildBattleReqAccept(ch, msg);
		break;

	case MSG_GUILD_BATTLE_STOP_REQ:
		do_GuildBattleStopReq(ch, msg);
		break;

	case MSG_GUILD_BATTLE_STOP_REJECT:
		do_GuildBattleStopReject(ch, msg);
		break;

	case MSG_GUILD_BATTLE_STOP_ACCEPT:
		do_GuildBattleStopAccept(ch, msg);
		break;

// 공성전
#ifdef ENABLE_WAR_CASTLE

	case MSG_GUILD_WAR_SET_TIME_REQ:
		do_GuildWarSetTimeReq(ch, msg);
		break;

	case MSG_GUILD_WAR_GET_TIME:
		do_GuildWarGetTime(ch, msg);
		break;

	case MSG_GUILD_WAR_JOIN_ATTACK_GUILD:
		do_GuildWarJoinAttackGuild(ch, msg);
		break;

	case MSG_GUILD_WAR_JOIN_DEFENSE_GUILD:
		do_GuildWarJoinDefenseGuild(ch, msg);
		break;

	case MSG_GUILD_WAR_JOIN_ATTACK_CHAR:
		do_GuildWarJoinAttackChar(ch, msg);
		break;

	case MSG_GUILD_WAR_JOIN_DEFENSE_CHAR:
		do_GuildWarJoinDefenseChar(ch, msg);
		break;

	case MSG_GUILD_WAR_ATTACK_SYMBOL:
		do_GuildWarAttackSymbol(ch, msg);
		break;

	case MSG_GUILD_STASH_HISTORY_REQ:
		do_GuildStashHistoryReq(ch, msg);
		break;
	case MSG_GUILD_STASH_VIEW_REQ:
		do_GuildStashViewReq(ch, msg);
		break;
	case MSG_GUILD_STASH_TAKE_REQ:
		do_GuildStashTakeReq(ch, msg);
		break;

#endif // #ifdef ENABLE_WAR_CASTLE

#ifdef NEW_GUILD
	case MSG_NEW_GUILD_INCLINE_ESTABLISH:
		do_GuildInclineEstablist( ch, msg );
		break;
	case MSG_NEW_GUILD_MEMBER_ADJUST:
		do_GuildMemberAdjust( ch, msg );
		break;

	case MSG_NEW_GUILD_INFO:
		do_NewGuildInfo( ch, msg );
		break;
	case MSG_NEW_GUILD_MEMBERLIST:
		do_NewGuildMemberList( ch, msg );
		break;
	case MSG_NEW_GUILD_MANAGE:
		do_NewGuildManage( ch, msg );
		break;
	case MSG_NEW_GUILD_NOTICE:
		do_NewGuildNotice( ch, msg );
		break;
	case MSG_NEW_GUILD_NOTICE_UPDATE:
		do_NewGuildNoticeUpdate( ch, msg );
		break;
	case MSG_NEW_GUILD_NOTICE_REQUEST:
		do_NewGuildNoticeTrans( ch, msg );
		break;
	case MSG_NEW_GUILD_SKILL:
		do_NewGuildSkillList( ch, msg );
		break;

	case MSG_NEW_GUILD_SKILL_LEARN:
		do_NewGuildSkillLearn( ch, msg );
		break;
#endif // NEW_GUILD

	default:
		break;
	}
}

void do_GuildCreate(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	CLCString name(MAX_GUILD_NAME_LENGTH + 1);

	msg >> name;

	// 이름 검사
	int lenname = strlen(name);
	if (lenname < 2 || lenname > 16)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GUILDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

#if defined(LC_JPN) && defined(_DEBUG) 
	if (strchr(name, '\'') || strchr(name, ' '))
#elif defined(LC_HBK)
	if (strchr(name, '\'') || strchr(name, ' '))
#else
	if (strchr(name, '\'') || strchr(name, '\\') || strchr(name, '\"') || strchr(name, '%') || strchr(name, '_') || strchr(name, ' '))
#endif // LC_JPN
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GUILDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 생성 조건 : 레벨 검사
	if (ch->m_level < GUILD_LEVEL1_NEED_LEVEL)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CHARLEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 생성 조건 : SP 검사
	if (ch->m_skillPoint < GUILD_LEVEL1_NEED_SP * 10000)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERRRO_SKILLPOINT);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 생성 조건 : 돈 검사
	if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < GUILD_LEVEL1_NEED_MONEY)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 이전 길드 검사
	if (ch->m_guildInfo)
	{
		if (ch->m_guildInfo->pos() == MSG_GUILD_POSITION_BOSS)
		{
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_ALREADYBOSS);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	if (IS_RUNNING_HELPER)
	{
		HelperGuildCreateReqMsg(rmsg, ch, name);
#ifdef NEW_GUILD
		rmsg << GUILD_LEVEL1_MAX_MEMBER
			 << ch->m_pZone->m_index;
#endif // NEW_GUILD

		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true,9);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildLevelUp(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (ch->m_guildInfo == NULL)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuild* guild = ch->m_guildInfo->guild();
	if (guild->boss()->charindex() != ch->m_index)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int needlevel		= 0x7fffffff;
	LONGLONG needmoney	= 0x7fffffff;
	int needsp			= 0x7fffffff;
#ifdef NEW_GUILD
	int	needgp			= 0x7fffffff;
	int gap				= 0x7fffffff;
#endif// NEW_GUILD

#ifdef NEW_GUILD
	if( guild->level() < GUILD_EXTEND_BASE_LEVEL  )
	{
#endif // NEW_GUILD
		switch (guild->level())
		{
		case 1:
			needlevel = GUILD_LEVEL2_NEED_LEVEL;
			needmoney = GUILD_LEVEL2_NEED_MONEY;
			needsp = GUILD_LEVEL2_NEED_SP;
			break;

		case 2:
			needlevel = GUILD_LEVEL3_NEED_LEVEL;
			needmoney = GUILD_LEVEL3_NEED_MONEY;
			needsp = GUILD_LEVEL3_NEED_SP;
			break;

		case 3:
			needlevel = GUILD_LEVEL4_NEED_LEVEL;
			needmoney = GUILD_LEVEL4_NEED_MONEY;
			needsp = GUILD_LEVEL4_NEED_SP;
			break;

		case 4:
			needlevel = GUILD_LEVEL5_NEED_LEVEL;
			needmoney = GUILD_LEVEL5_NEED_MONEY;
			needsp = GUILD_LEVEL5_NEED_SP;
			break;
#ifdef NEW_GUILD
		case 5:
			needlevel = GUILD_LEVEL6_NEED_LEVEL;
			needmoney = GUILD_LEVEL6_NEED_MONEY;
			needsp = GUILD_LEVEL6_NEED_SP;
			break;
		case 6:
			needlevel = GUILD_LEVEL7_NEED_LEVEL;
			needmoney = GUILD_LEVEL7_NEED_MONEY;
			needsp = GUILD_LEVEL7_NEED_SP;
			needgp	= GUILD_LEVEL7_NEED_GP;
			break;
#endif // NEW_GUILD
		}
#ifdef NEW_GUILD
	}
	else if( guild->level() >= GUILD_EXTEND_BASE_LEVEL )
	{
		if( guild->level() > 4 && guild->level() < 11 )
			needlevel = GUILD_LEVEL7_NEED_LEVEL;
		else if( guild->level() > 10 && guild->level() < 20 )
			needlevel = GUILD_LEVEL7_NEED_LEVEL + 10;
		else if( guild->level() > 19 && guild->level() < 30 )
			needlevel = GUILD_LEVEL7_NEED_LEVEL + 20;
		else if( guild->level() > 29 && guild->level() < 40 )
			needlevel = GUILD_LEVEL7_NEED_LEVEL + 30;
		else if( guild->level() > 39 && guild->level() < 50 )
			needlevel = GUILD_LEVEL7_NEED_LEVEL + 40;
		else
			needlevel = GUILD_LEVEL7_NEED_LEVEL + 50;

		gap = guild->level() + 1 - GUILD_EXTEND_BASE_LEVEL;
		if( gap >= 0 )
		{
			needmoney	= (LONGLONG)(GUILD_LEVEL7_NEED_MONEY * pow(1.09, gap));
			needsp		= (int)(GUILD_LEVEL7_NEED_SP * pow(1.09, gap));
			needgp		= (int)(GUILD_LEVEL7_NEED_GP * pow(1.09, gap));
		}
//		else
//		{
//			needmoney	= GUILD_LEVEL7_NEED_MONEY;
//			needsp		= GUILD_LEVEL7_NEED_SP;
//			needgp		= GUILD_LEVEL7_NEED_GP;
//		}
	}

	// 생성 조건
	if( guild->level() >= GUILD_EXTEND_BASE_LEVEL  - 1 )
	{
		if( guild->GetGuildPoint() < needgp )
		{
			GuildErrorMsg( rmsg, MSG_NEW_GUILD_ERRROR_GUILDPOINT );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
	}
#endif // NEW_GUILD


	// 레벨 검사
	if (ch->m_level < needlevel)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CHARLEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 생성 조건 : SP 검사
	if (ch->m_skillPoint < needsp * 10000)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERRRO_SKILLPOINT);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 생성 조건 : 돈 검사
	if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < needmoney)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// TODO : GUILD : 트리구조시 상위 길드와의 관계 고려
	if (IS_RUNNING_HELPER)
	{
#ifdef NEW_GUILD
		int guildmaxmember = 0;
		switch( guild->level() +1 )
		{
			case 1: guildmaxmember = GUILD_LEVEL1_MAX_MEMBER; break;
			case 2: guildmaxmember = GUILD_LEVEL2_MAX_MEMBER; break;
			case 3: guildmaxmember = GUILD_LEVEL3_MAX_MEMBER; break;
			case 4: guildmaxmember = GUILD_LEVEL4_MAX_MEMBER; break;
			case 5: guildmaxmember = GUILD_LEVEL5_MAX_MEMBER; break;
			case 6: guildmaxmember = GUILD_LEVEL6_MAX_MEMBER; break;
			default :
				{
					gap = guild->level() + 1 - GUILD_EXTEND_BASE_LEVEL;
				//	gap = guild->level() - GUILD_EXTEND_BASE_LEVEL;
					if( gap >= 0 )
					{
						if( guild->m_passiveSkillList.Find( 444 ) )
						{
							CSkill* skill = guild->m_passiveSkillList.Find( 444 );

#ifdef NEW_GUILD_BUG_FIX
							if(skill->m_level <= 5)
								guildmaxmember = GUILD_LEVEL6_MAX_MEMBER + (5 * skill->m_level );
							else
								guildmaxmember = 55 + 9 * (skill->m_level - 5);
#else
							guildmaxmember = GUILD_LEVEL6_MAX_MEMBER + (5 * skill->m_level );
#endif // NEW_GUILD_BUG_FIX
						}
						else 
							guildmaxmember = GUILD_LEVEL6_MAX_MEMBER;

					}
				}
				break;
		}
	
		HelperGuildLevelUpReqMsg(rmsg, guild->index(), ch->m_index);
		rmsg << guildmaxmember;
#else
		HelperGuildLevelUpReqMsg(rmsg, guild->index(), ch->m_index);
#endif // NEW_GUILD
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true, 10);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildBreakUp(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (ch->m_guildInfo == NULL)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_BREAK_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

#ifdef ENABLE_WAR_CASTLE
	int ownZoneCount = 0;
	int* ownZone = CWarCastle::GetOwnCastle(ch->m_guildInfo->guild()->index(), &ownZoneCount);
	if (ownZone)
	{
		int i;
		for (i = 0; i < ownZoneCount; i++)
		{
			CWarCastle* castle = CWarCastle::GetCastleObject(ownZone[i]);
			if (castle)
			{
				if(castle->GetState() != WCSF_NORMAL || castle->IsJoinTime())
				{
					GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_BREAK_WAR);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}
		}
		delete [] ownZone;
	}
#endif

	if (IS_RUNNING_HELPER)
	{
		HelperGuildBreakUpReqMsg(rmsg, ch);
		SEND_Q(rmsg, gserver.m_helper);
//		ch->m_desc->WaitHelperReply(true);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildRegistReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int bossindex;
	int requesterindex;

	msg >> guildindex
		>> bossindex
		>> requesterindex;

	if (ch->m_guildoutdate != 0)
	{
		time_t t;
		if (time(&t) / 60 / 60 / 24 < ch->m_guildoutdate + GUILD_REG_DELAY)
		{
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_REGDELAY);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	if (bossindex == ch->m_index || requesterindex != ch->m_index)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_TARGET_NOBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* member = guild->findmember(bossindex);
	if (member == NULL)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_TARGET_NOBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (member->pos() == MSG_GUILD_POSITION_MEMBER)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_TARGET_NOBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->GetPC() == NULL)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_TARGET_NOBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->GetPC()->m_regGuild && member->GetPC()->m_regGuild != ch->m_index)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_TARGET_BUSY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_regGuild && ch->m_regGuild != member->charindex())
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_SOURCE_BUSY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_REGIST_REQ_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	member->GetPC()->m_regGuild = ch->m_index;
	ch->m_regGuild = member->charindex();

	SEND_Q(msg, ch->m_desc);
	SEND_Q(msg, member->GetPC()->m_desc);
}

void do_GuildRegistAllow(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int requesterindex;

	msg >> guildindex
		>> requesterindex;

	CPC* pc = gserver.m_playerList.Find(requesterindex);
	if (!pc)
	{
		GuildRegistCancelMsg(rmsg, false);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_regGuild = 0;
		return ;
	}

	if (pc->m_regGuild != ch->m_index || ch->m_regGuild != pc->m_index)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		GuildRegistCancelMsg(rmsg, true);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}

	if (guild->maxmember() <= guild->membercount())
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_FULLMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}

	// TODO : GUILD : 대상이 이미 다른 길드에 소속되어 있다면, 트리 구성시 길드 간의 레벨을 검사

	CGuildMember* member = guild->findmember(ch->m_index);
	if (!member)
	{
		GuildRegistCancelMsg(rmsg, true);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}
	if (member->pos() == MSG_GUILD_POSITION_MEMBER)
	{
		GuildRegistCancelMsg(rmsg, true);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}
	if (member->GetPC() == NULL)
	{
		GuildRegistCancelMsg(rmsg, true);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperGuildMemberAddReqMsg(rmsg, guildindex, member->charindex(), pc->m_index, pc->GetName());
#ifdef NEW_GUILD
		rmsg << pc->m_pZone->m_index;
#endif // NEW_GUILD
		SEND_Q(rmsg, gserver.m_helper);
		member->GetPC()->m_desc->WaitHelperReply(true, 11);
		pc->m_desc->WaitHelperReply(true, 12);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}
}

void do_GuildRegistCancel(CPC* ch, CNetMsg& msg)
{
	if (ch->m_regGuild == 0)
		return ;

	CPC* pc = gserver.m_playerList.Find(ch->m_regGuild);
	if (pc)
	{
		if (pc->m_regGuild == ch->m_index)
		{
			pc->m_regGuild = 0;
			SEND_Q(msg, pc->m_desc);
		}
	}
	ch->m_regGuild = 0;
	SEND_Q(msg, ch->m_desc);
}

void do_GuildOutReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	msg >> guildindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_OUT_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* member = guild->findmember(ch->m_index);
	if (!member)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->pos() == MSG_GUILD_POSITION_BOSS)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_OUTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperGuildOutReqMsg(rmsg, guildindex, ch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildKick(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;

	msg >> guildindex
		>> charindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_guildInfo == NULL)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_guildInfo->guild() != guild)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_guildInfo->pos() == MSG_GUILD_POSITION_MEMBER)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_KICK_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->pos() == MSG_GUILD_POSITION_BOSS)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANTKICKBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperGuildKickReqMsg(rmsg, guildindex, ch->m_index, charindex);
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true, 13);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildChangeBoss(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int current;
	int change;

	msg >> guildindex
		>> current
		>> change;

	if (ch->m_index != current)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* boss = guild->findmember(current);
	if (!boss)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (boss->pos() != MSG_GUILD_POSITION_BOSS)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* member = guild->findmember(change);
	if (!member)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->pos() == MSG_GUILD_POSITION_BOSS)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CPC * pc = member->GetPC();
	if ( !pc || pc->m_level < GUILD_LEVEL1_NEED_LEVEL)
	{
		GuildErrorMsg(rmsg,MSG_GUILD_ERROR_CHARLEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperGuildChangeBossReqMsg(rmsg, guildindex, current, change);
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true, 14);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildAppointOfficer(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;

	msg >> guildindex
		>> charindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

//	int i;
//	for (i = 0; i < GUILD_MAX_OFFICER; i++)
//	{
//		if (guild->officer(i) == NULL)
//			break;
//	}
//	if (i == GUILD_MAX_OFFICER)
//	{
//		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_FULLOFFICER);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}

	CGuildMember* boss = guild->findmember(ch->m_index);
	if (!boss)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (boss->pos() != MSG_GUILD_POSITION_BOSS)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->pos() != MSG_GUILD_POSITION_MEMBER)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANTOFFICER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperGuildAppointOfficerReqMsg(rmsg, guildindex, ch->m_index, charindex);
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildFireOfficer(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;

	msg >> guildindex
		>> charindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* boss = guild->findmember(ch->m_index);
	if (!boss)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (boss->pos() != MSG_GUILD_POSITION_BOSS)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->pos() != MSG_GUILD_POSITION_OFFICER)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTOFFICER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperGuildFireOfficerReqMsg(rmsg, guildindex, ch->m_index, charindex);
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true, 15);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildBattleReqReq(CPC* ch, CNetMsg& msg)
{
	int charindex;
	int prize, time;

	msg >> charindex
		>> prize
		>> time;

	CNetMsg rmsg;

	// 길드 없음
	if (!ch->m_guildInfo)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드장이 아님
	if (ch->m_guildInfo->guild()->boss() && ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
#ifdef KJTEST
#else
	// 10명이상 길드
	if (ch->m_guildInfo->guild()->membercount() < GUILD_BATTLE_MIN_MEMBER)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_MEMBERCOUNT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
#endif	// KJTEST

	// 길드전이 가능한 존에 있어야 
#ifndef LC_KOR
	if (ch->m_pZone->m_index != ZONE_START && ch->m_pZone->m_index != ZONE_DRATAN && ch->m_pZone->m_index != ZONE_MERAC && ch->m_pZone->m_index != ZONE_EGEHA)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_ZONE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
#endif // LC_KOR

	// 이미 길드전 중
	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_ALREADY_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 배팅금액 오류
	if (prize * 10000 < GUILD_BATTLE_MIN_PRIZE || prize * 10000 > GUILD_BATTLE_MAX_PRIZE)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_PRIZE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 배팅액 모질람
	if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < (LONGLONG)prize)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_PRIZE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 시간 초과 검사
	if (time * 10 < GUILD_BATTLE_MIN_TIME || time * 10 > GUILD_BATTLE_MAX_TIME)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_TIME);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대가 이상함
	CPC* tch = gserver.m_playerList.Find(charindex);
	if (!tch || !tch->m_guildInfo)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대가 길드장이 아님
	if (tch->m_guildInfo->guild()->boss() && tch->m_guildInfo->guild()->boss()->charindex() != tch->m_index)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

#ifdef KJTEST
#else
	// 상대가 10명 미만 길드
	if (tch->m_guildInfo->guild()->membercount() < GUILD_BATTLE_MIN_MEMBER)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_MEMBERCOUNT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
#endif	// KJTESt

	// 이미 길드전 중
	if (tch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_ALREADY_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드전이 가능한 존에 있어야 
#ifndef LC_KOR
	if (tch->m_pZone->m_index != ZONE_START && tch->m_pZone->m_index != ZONE_DRATAN && tch->m_pZone->m_index != ZONE_MERAC && tch->m_pZone->m_index != ZONE_EGEHA)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_ZONE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
#endif // LC_KOR

	// 셋팅
	ch->m_guildInfo->guild()->BattleSet(tch->m_guildInfo->guild()->index(), prize * 10000, ch->m_pZone->m_index);
	ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_START_WAIT);
	ch->m_guildInfo->guild()->BattleTime(GUILD_BATTLE_WAIT_TIME + time * 10);

	tch->m_guildInfo->guild()->BattleSet(ch->m_guildInfo->guild()->index(), prize * 10000, tch->m_pZone->m_index);
	tch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_START_WAIT);
	tch->m_guildInfo->guild()->BattleTime(GUILD_BATTLE_WAIT_TIME + time * 10);

	// 요청 msg
	GuildBattleReqReqMsg(rmsg, ch->m_guildInfo->guild()->index(), ch->m_guildInfo->guild()->name(), prize, time);
	SEND_Q(rmsg, tch->m_desc);
}

void do_GuildBattleReqReject(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (!ch->m_guildInfo)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 신청받은적 없음
	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_START_WAIT)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOT_REQ);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드장이 아님
	if (ch->m_guildInfo->guild()->boss() && ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방 길드
	CGuild* guild = gserver.m_guildlist.findguild(ch->m_guildInfo->guild()->battleIndex());

	// 없음
	if (!guild)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방이 신청한적 없음
	if (guild->battleState() != GUILD_BATTLE_STATE_START_WAIT)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOT_REQ);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방 길드장
	CPC* tch = guild->boss()->GetPC();

	// 상대방 길드장이 없음
	if (!tch)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 셋팅 해제
	ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
	ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
	ch->m_guildInfo->guild()->BattleTime(0);

	guild->BattleSet(-1, 0, -1);
	guild->BattleState(GUILD_BATTLE_STATE_PEACE);
	guild->BattleTime(0);

	GuildBattleReqRejectMsg(rmsg, ch->m_index);
	SEND_Q(rmsg, ch->m_desc);
	SEND_Q(rmsg, tch->m_desc);
}

void do_GuildBattleReqAccept(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (!ch->m_guildInfo)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 신청받은적 없음
	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_START_WAIT)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOT_REQ);
		SEND_Q(rmsg, ch->m_desc);

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		return;
	}

	// 길드장이 아님
	if (ch->m_guildInfo->guild()->boss() && ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		return;
	}

	// 상대방 길드
	CGuild* guild = gserver.m_guildlist.findguild(ch->m_guildInfo->guild()->battleIndex());

	// 상대방 길드가 없음
	if (!guild)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		return;
	}

	// 상대방 길드장 
	CPC* tch = guild->boss()->GetPC();

	// 상대방 길드장이 없음
	if (!tch)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		guild->BattleSet(-1, 0, -1);
		guild->BattleState(GUILD_BATTLE_STATE_PEACE);
		guild->BattleTime(0);

		return;
	}

	// 배팅액이 모질라
	if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < ch->m_guildInfo->guild()->battlePrize())
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_SHORT_PRIZE);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, tch->m_desc);

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		guild->BattleSet(-1, 0, -1);
		guild->BattleState(GUILD_BATTLE_STATE_PEACE);
		guild->BattleTime(0);

		return;
	}

	// 배팅금액이 없음
	if (tch->m_moneyItem == NULL || tch->m_moneyItem->Count() < guild->battlePrize())
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_SHORT_PRIZE);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, tch->m_desc);

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		guild->BattleSet(-1, 0, -1);
		guild->BattleState(GUILD_BATTLE_STATE_PEACE);
		guild->BattleTime(0);
		return;
	}

	// 셋팅된 금액이 틀림
	if (ch->m_guildInfo->guild()->battlePrize() != guild->battlePrize())
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_PRIZE);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, tch->m_desc);

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		guild->BattleSet(-1, 0, -1);
		guild->BattleState(GUILD_BATTLE_STATE_PEACE);
		guild->BattleTime(0);

		return;
	}

	// 셋팅된 존이 틀림
	if (ch->m_guildInfo->guild()->battleZone() != guild->battleZone())
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_ZONE);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, tch->m_desc);

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		guild->BattleSet(-1, 0, -1);
		guild->BattleState(GUILD_BATTLE_STATE_PEACE);
		guild->BattleTime(0);

		return;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperGuildBattleReqMsg(rmsg, ch->m_guildInfo->guild()->index(), guild->index(), guild->battlePrize(), guild->battleZone(), guild->battleTime());
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildBattleStopReq(CPC* ch, CNetMsg& msg)
{
	int charindex;

	msg >> charindex;

	CNetMsg rmsg;

	if (!ch->m_guildInfo)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드전 중 아님 
	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_ING)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOT_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드장이 아님
	if (ch->m_guildInfo->guild()->boss() && ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방 길드
	CGuild* guild = gserver.m_guildlist.findguild(ch->m_guildInfo->guild()->battleIndex());

	if (!guild)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CPC* tch = guild->boss()->GetPC();

	// 상대방 길드장이 없음
	if (!tch)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방이 길장이 아님
	if (charindex != tch->m_index)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 중단 신청 상태 셋팅
	ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_STOP_WAIT);
	guild->BattleState(GUILD_BATTLE_STATE_STOP_WAIT);

	// 상대방에게 요청
	GuildBattleStopReqMsg(rmsg, ch->m_guildInfo->guild()->index(), ch->m_guildInfo->guild()->name());
	SEND_Q(rmsg, tch->m_desc);
}

void do_GuildBattleStopReject(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (!ch->m_guildInfo)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드전 중이 아님
	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_STOP_WAIT)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOT_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드장이 아님
	if (ch->m_guildInfo->guild()->boss() && ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방 길드
	CGuild* guild = gserver.m_guildlist.findguild(ch->m_guildInfo->guild()->battleIndex());

	if (!guild)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CPC* tch = guild->boss()->GetPC();

	// 상대방 길드장이 없음
	if (!tch)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 전투 상태 셋팅
	ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_ING);
	guild->BattleState(GUILD_BATTLE_STATE_ING);

	GuildBattleStopRejectMsg(rmsg, ch->m_index);
	SEND_Q(rmsg, ch->m_desc);
	SEND_Q(rmsg, tch->m_desc);
}

void do_GuildBattleStopAccept(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (!ch->m_guildInfo)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드전 중이 아님
	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_STOP_WAIT)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOT_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드장이 아님
	if (ch->m_guildInfo->guild()->boss() && ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방 길드
	CGuild* guild = gserver.m_guildlist.findguild(ch->m_guildInfo->guild()->battleIndex());

	if (!guild)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CPC* tch = guild->boss()->GetPC();

	// 상대방 길드장이 없음
	if (!tch)
	{
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (IS_RUNNING_HELPER)
	{
		HelperGuildBattleStopReqMsg(rmsg, ch->m_guildInfo->guild()->index(), guild->index());
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildWarSetTimeReq(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	int wday;
	int hour;

	msg >> wday
		>> hour;

	CNetMsg rmsg;

	if (ch->m_pZone == NULL) return ;
	int zone = ch->m_pZone->m_index;
	CWarCastle* castle = CWarCastle::GetCastleObject(zone);
	if (!castle)
		return ;

	CHECK_SUBSERVER(castle);

	// 길드가 있고
	if (!ch->m_guildInfo)
	{
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_OWNER, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 성주 길드이고
	CGuild* guild = ch->m_guildInfo->guild();
	if (guild->index() != castle->GetOwnerGuildIndex())
	{
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_OWNER, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드 장이고
	if (guild->boss()->charindex() != ch->m_index)
	{
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_OWNER, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 공성중이 아니고
	if (castle->GetState() != WCSF_NORMAL)
	{
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_PROGRESS_WAR, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 신청시간이 아닐때
	if (castle->IsJoinTime())
	{
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_SETTIME, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 시간 설정
	if (!castle->SetNextWarTime(wday, hour))
	{
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_INVALID_TIME, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// DB 초기화
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbcastle);

	sprintf(g_buf,
			"UPDATE t_castle"
			" SET a_next_war_time = %d"
			" WHERE a_zone_index = %d"
			, castle->GetNextWarTime()
			, castle->GetZoneIndex());
	cmd.SetQuery(g_buf);
	cmd.Update();

	struct tm tmNext;
	castle->GetNextWarTime(&tmNext, true);

	GAMELOG << init("WAR CASTLE : SET WAR TIME", ch)
			<< tmNext.tm_year << delim
			<< tmNext.tm_mon << delim
			<< tmNext.tm_mday << delim
			<< tmNext.tm_hour << delim
			<< wday
			<< end;

	GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_OK, tmNext.tm_mon, tmNext.tm_mday, tmNext.tm_hour, 0);
	SEND_Q(rmsg, ch->m_desc);
#endif
}

void do_GuildWarGetTime(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	// 공성 시간 확인		: zoneindex(n) warerrorcode(n) guildindex(n:s) guildname(n:s) month(n:s) day(n:s) hour(n:s)
	// 해당 존이 공성 존인가?
	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle == NULL)
		return ;

	CHECK_SUBSERVER(castle);

	// 해당 존이 일반 상태인가?
	if (castle->GetState() != WCSF_NORMAL)
	{
		GuildWarGetTimeMsg(rmsg, MSG_GUILD_WAR_ERROR_PROGRESS_WAR, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GuildWarGetTimeMsg(rmsg, MSG_GUILD_WAR_ERROR_OK, castle);
	SEND_Q(rmsg, ch->m_desc);
#endif
}

void do_GuildWarJoinAttackGuild(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle == NULL)
		return ;

	CHECK_SUBSERVER(castle);

	// 길드가 있고
	if (ch->m_guildInfo == NULL)
	{
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_NOT_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 보스 이고
	if (ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS)
	{
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_NOT_GUILD_BOSS, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CGuild* guild = ch->m_guildInfo->guild();

	// 성주 길드가 아니고
	if (guild->index() == castle->GetOwnerGuildIndex())
	{
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 기존 신청 검사하고
	if (castle->IsAttackGuild(guild->index()))
	{
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	
#ifdef DRATAN_CASTLE
	// 드라탄과 메라크 동시 신청 불가
	switch(ch->m_pZone->m_index)
	{
	case ZONE_DRATAN:
		{
			CWarCastle * pCastle = CWarCastle::GetCastleObject(ZONE_MERAC);
			if(pCastle != NULL 
				&& pCastle->IsAttackGuild(guild->index()))
			{	// 메라크 공성 신청한 길드가 드라탄 공성 신청할 경우
				GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if(pCastle != NULL
				&& guild->index() == pCastle->GetOwnerGuildIndex())
			{	// 메라크 성주가 드라탄 공성 신청할 경우
				GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD, NULL);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
		break;

	case ZONE_MERAC:
		{
			CWarCastle * pCastle = CWarCastle::GetCastleObject(ZONE_DRATAN);
			if(pCastle != NULL 
				&& pCastle->IsAttackGuild(guild->index()))
			{	// 드라탄 공성 신청한 길드가 메라크 공성 신청할 경우
				GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if (pCastle != NULL
				&& guild->index() == pCastle->GetOwnerGuildIndex())
			{	// 드라탄 성주가 메라크 공성 신청할 경우
				GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD, NULL);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
		break;
	}
#endif // DRATAN_CASTLE

	// 조건 검사하고 : 신청 가능 시간, 길드 레벨, 길드원 수, 아이템, 나스
	if (!castle->IsJoinTime())
	{
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_TIME, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

#ifdef DRATAN_CASTLE
#ifdef LC_HBK
	if (ch->m_pZone->m_index == ZONE_DRATAN && guild->level() < WCGF_ATTACK_LEVEL)
	{
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_GUILDLEVEL, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif // LC_HBK
#endif // DRATAN_CASTLE

	if (ch->m_pZone->m_index == ZONE_MERAC && guild->level() < WCGF_ATTACK_LEVEL)
	{
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_GUILDLEVEL, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_pZone->m_index == ZONE_MERAC && guild->membercount() < WCGF_MEMBERCOUNT)
	{	// 메라크 공성만 20명 제한
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_GUILDMEMBER, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (!ch->m_moneyItem || ch->m_moneyItem->Count() < WCGF_MONEY)
	{
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_MONEY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 영웅의 증표
//	int row, col;
//	if (!ch->m_invenNormal.FindItem(&row, &col, WCGF_ITEM, 0, 0))
//	{
//		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ITEM, NULL);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}
//
//	// 아이템 줄이고
//	CItem* item = ch->m_invenNormal.GetItem(row, col);
//	ItemDeleteMsg(rmsg, item);
//	SEND_Q(rmsg, ch->m_desc);
//	RemoveFromInventory(ch, INVENTORY_NORMAL, row, col, true, true);
//	if (ch->m_moneyItem->Count() > WCGF_MONEY)
//	{
//		DecreaseFromInventory(ch, ch->m_moneyItem, WCGF_MONEY);
//		ItemUpdateMsg(rmsg, ch->m_moneyItem, -WCGF_MONEY);
//		SEND_Q(rmsg, ch->m_desc);
//	}
//	else
//	{
//		ItemDeleteMsg(rmsg, ch->m_moneyItem);
//		DecreaseFromInventory(ch, ch->m_moneyItem, WCGF_MONEY);
//		SEND_Q(rmsg, ch->m_desc);
//	}

	ch->SaveNow(false);

	// DB에 저장
	// 1. 트랜잭션 시작
	// 2. 현재 캐릭터의 용병 신청 삭제
	// 3. 현재 캐릭터 길드의 신청 삭제
	// 4. 새로 추가
	// 5. 트랜잭션 끝
	int i;
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbcastle);
	cmd.BeginTrans();
	try
	{
		// 개인 용병 신청 삭제
		if (ch->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE)
		{
			switch (ch->GetJoinFlag(ch->m_pZone->m_index))
			{
			case WCJF_DEFENSE_GUILD:
				// 길원 모두 삭제
				g_buf[0] = '\0';
				for (i = 0; i < guild->maxmember(); i++)
				{
					if (guild->member(i))
					{
						if (g_buf[0] == '\0')
							sprintf(g_buf, "%d", guild->member(i)->charindex());
						else
						{
							sprintf(g_buf2, ",%d", guild->member(i)->charindex());
							strcat(g_buf, g_buf2);
						}
					}
				}
				sprintf(g_buf2, "DELETE FROM t_castle_join WHERE a_zone_index=%d AND a_index in (%s) AND a_guild=0", ch->m_pZone->m_index, g_buf);
				break;

			case WCJF_DEFENSE_CHAR:
			case WCJF_ATTACK_CHAR:
				sprintf(g_buf2, "DELETE FROM t_castle_join WHERE a_zone_index=%d AND a_index=%d AND a_guild=0", ch->m_pZone->m_index, ch->m_index);
				break;
			}
			cmd.SetQuery(g_buf2);
			if (!cmd.Update())
				throw 0;

			// 길드 신청 삭제
			sprintf(g_buf, "DELETE FROM t_castle_join WHERE a_zone_index=%d AND a_index=%d AND a_guild=1", ch->m_pZone->m_index, guild->index());
			cmd.SetQuery(g_buf);
			if (!cmd.Update())
				throw 0;
		}

		sprintf(g_buf, "INSERT INTO t_castle_join (a_zone_index, a_index, a_guild, a_attack) VALUES (%d, %d, 1, 1)", ch->m_pZone->m_index, guild->index());
		cmd.SetQuery(g_buf);
		if (!cmd.Update())
			throw 0;

		cmd.Commit();
	}
	catch (...)
	{
		cmd.Rollback();
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("WAR CASTLE : JOIN ATTACK GUILD", ch)
			<< guild->index() << delim
			<< guild->name()
			<< end;

	// Helper로 알리기
	// Helper 없으면 그냥 현재 서브 채널만
	HelperWarJoinAttackGuildMsg(rmsg, ch->m_pZone->m_index, guild->index());
	if (IS_RUNNING_HELPER)
	{
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		gserver.ProcessHelperMessage(rmsg);
	}

	// 결과 알림
	GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_OK, castle);
	SEND_Q(rmsg, ch->m_desc);
#endif
}

void do_GuildWarJoinDefenseGuild(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle == NULL)
		return ;

	CHECK_SUBSERVER(castle);

	// 성주가 있는 성이고
	if (castle->GetOwnerGuildIndex() <= 0)
	{
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_NO_OWNER, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드가 있고
	if (ch->m_guildInfo == NULL)
	{
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_NOT_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 보스 이고
	if (ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS)
		return ;

	CGuild* guild = ch->m_guildInfo->guild();

	// 성주 길드가 아니고
	if (guild->index() == castle->GetOwnerGuildIndex())
	{
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 기존 신청 검사하고
	if (castle->IsAttackGuild(guild->index()) || castle->IsDefenseGuild(guild->index()))
	{
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 조건 검사하고 : 신청 가능 시간, 길드 레벨
	if (!castle->IsJoinTime())
	{
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_TIME, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (guild->level() < WCGF_DEFENSE_LEVEL)
	{
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_GUILDLEVEL, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// DB에 저장
	// 1. 트랜잭션 시작
	// 2. 현재 캐릭터의 용병 신청 삭제
	// 3. 현재 캐릭터 길드의 신청 삭제
	// 4. 새로 추가
	// 5. 트랜잭션 끝
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbcastle);
	cmd.BeginTrans();
	try
	{
		// 개인 용병 신청 삭제
		if (ch->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE)
		{
			switch (ch->GetJoinFlag(ch->m_pZone->m_index))
			{
			case WCJF_DEFENSE_CHAR:
			case WCJF_ATTACK_CHAR:
				sprintf(g_buf2, "DELETE FROM t_castle_join WHERE a_zone_index=%d AND a_index=%d AND a_guild=0", ch->m_pZone->m_index, ch->m_index);
				break;
			}
			cmd.SetQuery(g_buf2);
			if (!cmd.Update())
				throw 0;
		}

		sprintf(g_buf, "INSERT INTO t_castle_join (a_zone_index, a_index, a_guild, a_attack) VALUES (%d, %d, 1, 0)", ch->m_pZone->m_index, guild->index());
		cmd.SetQuery(g_buf);
		if (!cmd.Update())
			throw 0;

		cmd.Commit();
	}
	catch (...)
	{
		cmd.Rollback();
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("WAR CASTLE : JOIN DEFENSE GUILD", ch)
			<< guild->index() << delim
			<< guild->name()
			<< end;

	// Helper로 알리기
	// Helper 없으면 그냥 현재 서브 채널만
	HelperWarJoinDefenseGuildMsg(rmsg, ch->m_pZone->m_index, guild->index());
	if (IS_RUNNING_HELPER)
	{
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		gserver.ProcessHelperMessage(rmsg);
	}

	// 결과 알림
	GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_OK, castle);
	SEND_Q(rmsg, ch->m_desc);
#endif
}

void do_GuildWarJoinAttackChar(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle == NULL)
		return ;

	CHECK_SUBSERVER(castle);

	// 길드 방침을 먼저 따름
	switch (ch->GetJoinFlag(ch->m_pZone->m_index))
	{
	case WCJF_OWNER:
		GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;

	case WCJF_DEFENSE_GUILD:
		GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;

	case WCJF_ATTACK_GUILD:
		GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;

	case WCJF_ATTACK_CHAR:
		GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 조건 검사하고 : 신청 가능 시간, 레벨
	if (!castle->IsJoinTime())
	{
		GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_TIME, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_level < WCGF_CHAR_LEVEL)
	{
		GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_CHARLEVEL, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// DB에 저장
	// 1. 트랜잭션 시작
	// 2. 현재 캐릭터의 용병 신청 삭제
	// 3. 현재 캐릭터 길드의 신청 삭제
	// 4. 새로 추가
	// 5. 트랜잭션 끝
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbcastle);
	cmd.BeginTrans();
	try
	{
		// 개인 용병 신청 삭제
		if (ch->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE)
		{
			switch (ch->GetJoinFlag(ch->m_pZone->m_index))
			{
			case WCJF_DEFENSE_CHAR:
			case WCJF_ATTACK_CHAR:
				sprintf(g_buf2, "DELETE FROM t_castle_join WHERE a_zone_index=%d AND a_index=%d AND a_guild=0", ch->m_pZone->m_index, ch->m_index);
				break;
			}
			cmd.SetQuery(g_buf2);
			if (!cmd.Update())
				throw 0;
		}

		sprintf(g_buf, "INSERT INTO t_castle_join (a_zone_index, a_index, a_guild, a_attack) VALUES (%d, %d, 0, 1)", ch->m_pZone->m_index, ch->m_index);
		cmd.SetQuery(g_buf);
		if (!cmd.Update())
			throw 0;

		cmd.Commit();
	}
	catch (...)
	{
		cmd.Rollback();
		GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("WAR CASTLE : JOIN ATTACK CHAR", ch)
			<< end;

	// 정보 변경
	// 성 정보
	castle->RemoveAttackChar(ch->m_index);
	castle->RemoveDefenseChar(ch->m_index);
	castle->AddAttackChar(ch->m_index);

	// 캐릭정보
	ch->SetJoinFlag(ch->m_pZone->m_index, WCJF_ATTACK_CHAR);
	ch->m_bChangeStatus = true;

	// 결과 알림
	GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_OK, castle);
	SEND_Q(rmsg, ch->m_desc);
#endif
}

void do_GuildWarJoinDefenseChar(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle == NULL)
		return ;

	CHECK_SUBSERVER(castle);

	// 성주가 있는 성이고
	if (castle->GetOwnerGuildIndex() <= 0)
	{
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_NO_OWNER, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드 방침을 먼저 따름
	switch (ch->GetJoinFlag(ch->m_pZone->m_index))
	{
	case WCJF_OWNER:
		GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;

	case WCJF_DEFENSE_GUILD:
		GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;

	case WCJF_ATTACK_GUILD:
		GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 조건 검사하고 : 신청 가능 시간, 레벨
	if (!castle->IsJoinTime())
	{
		GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_TIME, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_level < WCGF_CHAR_LEVEL)
	{
		GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_CHARLEVEL, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// DB에 저장
	// 1. 트랜잭션 시작
	// 2. 현재 캐릭터의 용병 신청 삭제
	// 3. 현재 캐릭터 길드의 신청 삭제
	// 4. 새로 추가
	// 5. 트랜잭션 끝
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbcastle);
	cmd.BeginTrans();
	try
	{
		// 개인 용병 신청 삭제
		if (ch->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE)
		{
			switch (ch->GetJoinFlag(ch->m_pZone->m_index))
			{
			case WCJF_DEFENSE_CHAR:
			case WCJF_ATTACK_CHAR:
				sprintf(g_buf2, "DELETE FROM t_castle_join WHERE a_zone_index=%d AND a_index=%d AND a_guild=0", ch->m_pZone->m_index, ch->m_index);
				break;
			}
			cmd.SetQuery(g_buf2);
			if (!cmd.Update())
				throw 0;
		}

		sprintf(g_buf, "INSERT INTO t_castle_join (a_zone_index, a_index, a_guild, a_attack) VALUES (%d, %d, 0, 0)", ch->m_pZone->m_index, ch->m_index);
		cmd.SetQuery(g_buf);
		if (!cmd.Update())
			throw 0;

		cmd.Commit();
	}
	catch (...)
	{
		cmd.Rollback();
		GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("WAR CASTLE : JOIN DEFENSE CHAR", ch)
			<< end;

	// 정보 변경
	// 성 정보
	castle->RemoveAttackChar(ch->m_index);
	castle->RemoveDefenseChar(ch->m_index);
	castle->AddDefenseChar(ch->m_index);

	// 캐릭정보
	ch->SetJoinFlag(ch->m_pZone->m_index, WCJF_DEFENSE_CHAR);
	ch->m_bChangeStatus = true;

	// 결과 알림
	GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_OK, castle);
	SEND_Q(rmsg, ch->m_desc);
#endif
}

void do_GuildWarAttackSymbol(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle == NULL)
		return ;

	// 성문중 하나라고 열린곳이 있을때
	if (!castle->IsOpenGate())
		return ;

	// 권좌가 아직 안 부셔졌을 때
	if (DEAD(castle->GetLordSymbol()))
		return ;

	// 길드가 있고
	if (ch->m_guildInfo == NULL)
		return ;
	CGuild* guild = ch->m_guildInfo->guild();

	// 공성탑3 길드중 하나이고
	if (!castle->IsTop3Guild(guild->index()))
		return ;

	// 길드장이고
	if (guild->boss()->charindex() != ch->m_index)
		return ;

	// 거리가 3미터 이내이고
	if (GetDistance(ch, castle->GetLordSymbol()) > 3.0f)
		return ;

	// 1.2초 마다 한번
	if (gserver.m_pulse - ch->m_lastAttackPulse + PULSE_HACK_ATTACK_THRESHOLD < 12)
		return ;
	ch->m_lastAttackPulse = gserver.m_pulse;
#ifdef ENABLE_PET
	ch->m_pulseLastAttackSkill = gserver.m_pulse;
#endif // #ifdef ENABLE_PET

	// 공격 메시지 에코
	DamageMsg(rmsg, ch, castle->GetLordSymbol(), MSG_DAMAGE_MELEE, -1, 1, HITTYPE_NORMAL);
	ch->m_pArea->SendToCell(rmsg, ch, true);

	// 권좌 HP 감소
	castle->DecreaseLordSymbolHP();

	// 권좌 HP 감소 알림
	CharStatusMsg(rmsg, castle->GetLordSymbol(), 0);
	ch->m_pArea->SendToCell(rmsg, ch, true);

	// 포인트 증가
	castle->IncreaseTeamPoint(guild->index(), 1);

	// 포인트 증가 알림
	gserver.m_playerList.SendGuildWarPointMsg(castle, ch->m_pZone->m_index);
#endif
}

void do_GuildStashHistoryReq(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	int month[7] = {0, 0, 0, 0, 0, 0, 0};
	int day[7] = {0, 0, 0, 0, 0, 0, 0};
	LONGLONG money[7] = {0, 0, 0, 0, 0, 0, 0};

	// 길드 룸에서만
	if (!ch->m_pZone || !ch->m_pZone->IsGuildRoom())
		return ;

	// 길드가 있는 사람만
	if (!ch->m_guildInfo || !ch->m_guildInfo->guild())
	{
		GuildStashHistoryRepMsg(rmsg, MSG_GUILD_STASH_ERROR_NOTBOSS, month, day, money);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 헬퍼에 요청
	if (IS_RUNNING_HELPER)
	{
		HelperGuildStashHistoryReqMsg(rmsg, ch->m_guildInfo->guild()->index(), ch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true, 16);
	}
#endif // #ifdef ENABLE_WAR_CASTLE
}

void do_GuildStashViewReq(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	// 길드 룸에서만
	if (!ch->m_pZone || !ch->m_pZone->IsGuildRoom())
		return ;

	// 길드가 있는 사람만
	if (!ch->m_guildInfo || !ch->m_guildInfo->guild())
	{
		GuildStashViewRepMsg(rmsg, MSG_GUILD_STASH_ERROR_NOTBOSS, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드장이나 부관만
	if (ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS && ch->m_guildInfo->pos() != MSG_GUILD_POSITION_OFFICER)
	{
		GuildStashViewRepMsg(rmsg, MSG_GUILD_STASH_ERROR_NOTBOSS, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 헬퍼에 요청
	if (IS_RUNNING_HELPER)
	{
		HelperGuildStashViewReqMsg(rmsg, ch->m_guildInfo->guild()->index(), ch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true, 17);
	}
#endif // #ifdef ENABLE_WAR_CASTLE
}

void do_GuildStashTakeReq(CPC* ch, CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	LONGLONG money = 0;
	msg >> money;

	if (money < 1)
		return ;

	// 길드 룸에서만
	if (!ch->m_pZone || !ch->m_pZone->IsGuildRoom())
		return ;

	// 길드가 있는 사람만
	if (!ch->m_guildInfo || !ch->m_guildInfo->guild())
	{
		GuildStashTakeRepMsg(rmsg, MSG_GUILD_STASH_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드장이나 부관만
	if (ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS && ch->m_guildInfo->pos() != MSG_GUILD_POSITION_OFFICER)
	{
		GuildStashTakeRepMsg(rmsg, MSG_GUILD_STASH_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 헬퍼에 요청
	if (IS_RUNNING_HELPER)
	{
		HelperGuildStashTakeReqMsg(rmsg, ch->m_guildInfo->guild()->index(), ch->m_index, money);
		SEND_Q(rmsg, gserver.m_helper);
		ch->m_desc->WaitHelperReply(true, 18);

		GAMELOG << init("GUILD STASH TAKE REQUEST", ch)
				<< "MONEY" << delim
				<< money
				<< end;
	}
#endif // #ifdef ENABLE_WAR_CASTLE
}


#ifdef NEW_GUILD
void do_GuildInclineEstablist( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;	
	char guildincline;

	msg >> guildincline;

	// 조건 검사
	// 길드가 있는 사람
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}
	// 길드장만
	if (ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS )
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드 레벨 검사
	if( ch->m_guildInfo->guild()->level() < 6 )
	{
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_CHANG_INCLINE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}


	//현재 길드 성향
//#define GUILD_INCLINE_GENERAL		0		// 일반 길드
//#define GUILD_INCLINE_CHAO			1		// 카오 길드
//#define GUILD_INCLINE_HUNTER		2		// 헌터 길드
	if( ch->m_guildInfo->guild()->GetIncline() == guildincline || guildincline == GUILD_INCLINE_GENERAL )
	{
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_SAME_INCLNE );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	LONGLONG needMoney = 0;
	int needGP = 0;

	if( guildincline == GUILD_INCLINE_CHAO )
	{
		// 나스 검사		
		if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < 2000000)
		{
			GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_CHANG_INCLINE);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// gp 검사
		if( ch->m_guildInfo->guild()->GetGuildPoint() < 10000 )
		{
			GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_CHANG_INCLINE);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		needMoney = 2000000;
		needGP	= 10000;

	}
	else if ( guildincline == GUILD_INCLINE_HUNTER )
	{
		// 나스 검사		
		if (ch->m_moneyItem == NULL || ch->m_moneyItem->Count() < 5000000 )
		{
			GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_CHANG_INCLINE);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// gp 검사
		if( ch->m_guildInfo->guild()->GetGuildPoint() < 5000 )
		{
			GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_CHANG_INCLINE);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		needMoney = 5000000;
		needGP	= 5000;
	}
	
	if (IS_RUNNING_HELPER)
	{
		HelperGuildInclineEstablishReqMsg( rmsg, ch, guildincline );
		rmsg << needMoney << needGP;

		SEND_Q(rmsg, gserver.m_helper);
//		ch->m_desc->WaitHelperReply(true);
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}

}
void do_GuildMemberAdjust( CPC* ch, CNetMsg& msg )
{
	CNetMsg		rmsg;
	int			charindex;
	CLCString	strPositionName(GUILD_POSITION_NAME+1);
	int			contributeExp;
	int			contributFame;
	
	
	msg >> charindex
		>> strPositionName
		>> contributeExp
		>> contributFame;

	// 길드가 있는 사람
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	//길드장인지 체크
	if( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS )
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	
	if (IS_RUNNING_HELPER)
	{
		HelperGuildMemberAdjust( rmsg, ch, charindex, strPositionName, contributeExp, contributFame );

		SEND_Q(rmsg, gserver.m_helper);
//		ch->m_desc->WaitHelperReply(true);
		
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_NewGuildInfo( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;

	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if( IS_RUNNING_HELPER )
	{
		HelperNewGuildInfo( rmsg, ch );
		SEND_Q( rmsg, gserver.m_helper );
//		ch->m_desc->WaitHelperReply( true );
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}

}
void do_NewGuildMemberList( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;

	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if( IS_RUNNING_HELPER )
	{
		HelperNewGuildMemberList( rmsg, ch );
		SEND_Q( rmsg, gserver.m_helper );
//		ch->m_desc->WaitHelperReply( true );
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_NewGuildManage( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;

	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}
	//길드장인지 체크
	if( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS  && ch->m_guildInfo->pos() != MSG_GUILD_POSITION_OFFICER)
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}


	if( IS_RUNNING_HELPER )
	{
		HelperNewGuildManage( rmsg, ch );
		SEND_Q( rmsg, gserver.m_helper );
//		ch->m_desc->WaitHelperReply( true );
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}
void do_NewGuildNotice( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;

	// 길드가 있는 사람인지 체크
	CGuild* guild;
	if( !ch->m_guildInfo )
	{
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	guild = ch->m_guildInfo->guild();
	if( !guild )
	{
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	// 공지 스킬을 배운 길드인지 체크
	if( !guild->m_passiveSkillList.Find( 443 ) )
	{
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}


	if( IS_RUNNING_HELPER )
	{
		HelperNewGuildNotice( rmsg, ch );
		SEND_Q( rmsg, gserver.m_helper );
//		ch->m_desc->WaitHelperReply( true );
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}
void do_NewGuildNoticeUpdate( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;

	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );

	msg >> title
		>> text;

	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	//길드장인지 체크
	if( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS )
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 공지 스킬을 배운 길드인지 체크
	CGuild* guild = ch->m_guildInfo->guild();
	if( !guild->m_passiveSkillList.Find( 443 ) )
	{
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}



	if( IS_RUNNING_HELPER )
	{
		HelperNewGuildNoticeUpdate( rmsg, ch, title, text );
		SEND_Q( rmsg, gserver.m_helper );
//		ch->m_desc->WaitHelperReply( true );
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}
void do_NewGuildNoticeTrans( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;

//	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
//	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );
//
//	msg >> title
//		>> text;

	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	//길드장인지 체크
	if( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS )
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}


	if( IS_RUNNING_HELPER )
	{
		HelperNewGuildNoticeTrans( rmsg, ch );
		SEND_Q( rmsg, gserver.m_helper );
//		ch->m_desc->WaitHelperReply( true );
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_NewGuildSkillList( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;

	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if( IS_RUNNING_HELPER )
	{
		HelperNewGuildSkillList( rmsg, ch );
		SEND_Q( rmsg, gserver.m_helper );
//		ch->m_desc->WaitHelperReply( true );
	}
	else
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_NewGuildSkillLearn( CPC* ch, CNetMsg& msg )
{
	CNetMsg rmsg;


	int skillindex;
	msg >> skillindex;

	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	//길드장인지 체크
	if( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS )
	{
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int guildindex = ch->m_guildInfo->guild()->index();


	const CSkillProto* proto = gserver.m_skillProtoList.Find( skillindex );

	if( !proto )
	{
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
		SEND_Q(rmsg, ch->m_desc);
		return ;	
	}

	CSkillList* list = NULL;
	if( proto->IsPassive() )
		list = &ch->m_guildInfo->guild()->m_passiveSkillList;
	else
	{
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CSkill* skill = list->Find( skillindex );
	int nextLevel = ( skill ) ? skill->m_level+1 : 1;

	const CSkillLevelProto* levelproto = proto->Level( nextLevel );
	if( !levelproto )
	{
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 스킬 검사
	if( skillindex == 444 )
	{
		CSkill* pCompSkill = list->Find( 443 );
		if( pCompSkill == NULL )
		{
			GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
			SEND_Q(rmsg, ch->m_desc);
			return ;

		}

	}
	
	//레벨 검사
	if( levelproto->m_learnLevel > ch->m_guildInfo->guild()->level() )
	{
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_LEARN_ERROR_LEVEL );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}	
	// gp 검사
	if( levelproto->m_learnGP > ch->m_guildInfo->guild()->GetGuildPoint() )
	{
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_ERRROR_GUILDPOINT );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 아이템 검사
	int i;
	CItem* item[MAX_SKILL_LEARN_ITEM];
	for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
	{
		item[i] = NULL;
		if (levelproto->m_learnItemIndex[i] < 0)
			continue ;
		int r, c;
		if (!ch->m_invenNormal.FindItem(&r, &c, levelproto->m_learnItemIndex[i], 0, 0))
		{
			GuildErrorMsg( rmsg, MSG_GUILD_ERROR_MONEY );
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		item[i] = ch->m_invenNormal.GetItem(r, c);
		if (!item[i] || item[i]->Count() < levelproto->m_learnItemCount[i])
		{
			GuildErrorMsg( rmsg, MSG_GUILD_ERROR_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	// GP 소모
	gserver.m_guildlist.findguild( guildindex )->AddGuildPoint( -levelproto->m_learnGP );
//	if( IS_RUNNING_HELPER )
//	{
//		HelperNewGuildPointUpdate( rmsg, ch->m_index, guildindex, -levelproto->m_learnGP );
//		SEND_Q( rmsg, gserver.m_helper );
//	}
//
	// 아이템 소모
	for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
	{
		if (item[i] && levelproto->m_learnItemCount[i] > 0)
		{
			DecreaseFromInventory(ch, item[i], levelproto->m_learnItemCount[i]);
			if (item[i]->Count() < 1)
			{
				ItemDeleteMsg(rmsg, item[i]);
				SEND_Q(rmsg, ch->m_desc);
				RemoveFromInventory(ch, item[i], true, true);
			}
			else
			{
				ItemUpdateMsg(rmsg, item[i], -levelproto->m_learnItemCount[i]);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}

	// 스킬 추가
	if (skill == NULL)
	{
		// 새로 배움
		skill = gserver.m_skillProtoList.Create(skillindex);
		if (!skill)
		{
			GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (proto->IsPassive())
			gserver.m_guildlist.findguild( guildindex )->m_passiveSkillList.Add(skill);		
	}
	else
	{
		// 있는 스킬 레벨 업
		if (skill->m_level >= proto->m_maxLevel)
		{
			GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ALEADY_LEARN );
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		skill->m_level++;
	}

	GAMELOG << init("GUILD SKILL LEARN", ch)
				<< skill->m_proto->m_index
				<< delim
				<< skill->m_level
				<< end;
	GuildSkillLearnMsg( rmsg, skill );
	SEND_Q(rmsg, ch->m_desc);
	GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_LEARN_ERROR_OK );
	SEND_Q(rmsg, ch->m_desc );


	GAMELOG << init("GUILD LEVEL UP ", ch )
			<< "Guild Index" << delim << ch->m_guildInfo->guild()->index() << delim			
#ifdef NEW_GUILD			
			<< "Guild Point" << delim << ch->m_guildInfo->guild()->GetGuildPoint() << delim
			<< "Need GP" << delim << levelproto->m_learnGP << delim
#endif // NEW_GUILD
			<< end;

	
	if( IS_RUNNING_HELPER )
	{
		HelperGuildSkillLearnMsg( rmsg, -levelproto->m_learnGP, ch->m_guildInfo->guild() );
		SEND_Q( rmsg, gserver.m_helper  );
	}	
}

#endif // NEW_GUILD