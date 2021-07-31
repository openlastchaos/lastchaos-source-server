#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"
#include "Guild.h"
#include "CmdMsg.h"
#include "doFunc.h"

CGuildMember::CGuildMember()
: m_name(MAX_CHAR_NAME_LENGTH + 1)
//, m_nick(MAX_CHAR_NAME_LENGTH + 1)
#ifdef NEW_GUILD
, m_positionName( GUILD_POSITION_NAME +1)
#endif // NEW_GUILD
{
	m_charindex = -1;
	m_pos = MSG_GUILD_POSITION_MEMBER;
	m_guild = NULL;
	m_listindex = -1;
	m_online = 0;
#ifdef __GAME_SERVER__
	m_pc = NULL;
#endif

#ifdef NEW_GUILD
	m_contributeExp		= 0;
	m_contributeFame	= 0;
	m_cumulatePoint		= 0;
	// kj add
	m_channel			= 1;
	m_zoneindex			= 0;
#endif // NEW_GUILD

}

CGuildMember::~CGuildMember()
{
#ifdef __GAME_SERVER__
	if (m_pc)
		m_pc->m_guildInfo = NULL;
	m_pc = NULL;
#endif
}

#ifdef EVENT_2007_PARENTSDAY_RESULT
char CGuildMember::Event2007ParentsdayResult()
{
	char Ranking = 0;
	if( guild() !=NULL )
	{
		if( gserver.m_serverno == 1 )
		{
			if( guild()->index() == 2604 )
			{
				Ranking = 1;
			}
			else if( guild()->index() == 38 )
			{
				Ranking = 2;
			}
			else if( guild()->index() == 3225 )
			{
				Ranking = 3;
			}
		}
		else if( gserver.m_serverno == 2 )
		{
			if( guild()->index() == 2063 )
			{
				Ranking = 1;
			}
			else if( guild()->index() == 3623 )
			{
				Ranking = 2;
			}
			else if( guild()->index() == 5138 )
			{
				Ranking = 3;
			}
		}
		else if( gserver.m_serverno == 3 )
		{
			if( guild()->index() == 431 )
			{
				Ranking = 1;
			}
			else if( guild()->index() == 1079 )
			{
				Ranking = 2;
			}
			else if( guild()->index() == 1107 )
			{
				Ranking = 3;
			}
		}
		else if( gserver.m_serverno == 4 )
		{
			if( guild()->index() == 478 )
			{
				Ranking = 1;
			}
			else if( guild()->index() == 235 )
			{
				Ranking = 2;
			}
			else if( guild()->index() == 660 )
			{
				Ranking = 3;
			}
		}
	}
	return Ranking;
}
#endif // EVENT_2007_PARENTSDAY_RESULT

#ifdef GUILD_MARK_TABLE
char CGuildMember::GetGuildMark()
{
	if (guild() != NULL)
	{
		for(int i = 0; i < 3;i++)
		{
			if(guild()->index() == gserver.m_nGuildMarkTable[i])
			{
				return i+1;
			}
		}
	}

	return 0;
}
#endif // GUILD_MARK_TABLE

CGuild::CGuild(int guildindex, const char* name, int level, int battleIndex, int battlePrize, int battleTime, int battleZone, int battleKillCount, int battleState)
: m_name(MAX_GUILD_NAME_LENGTH + 1)
{
	if (name && level > 0)
	{
		m_name = name;
		m_level = level;
		m_boss = NULL;
		memset(m_officer, 0, sizeof(CGuildMember*) * GUILD_MAX_OFFICER);
		memset(m_member, 0, sizeof(CGuildMember*) * GUILD_MAX_MEMBER);
		m_membercount = 0;
		m_next = NULL;
		m_prev = NULL;
		m_index = guildindex;

		m_battleIndex = battleIndex;
		m_battlePrize = battlePrize;
		m_battleTime = battleTime;
		m_battleZone = battleZone;
		m_killCount = battleKillCount;
		m_battleState = battleState;
		m_battlePulse = 0;
	}
	else
		memset(this, 0, sizeof(*this));

#ifdef __GAME_SERVER__

	memset(&m_guildSignal, 0, sizeof(GUILD_SIGNAL));

#ifdef GUILD_RANKING
	m_dExpGuild = 0.0;
#endif // GUILD_RANKING

#endif // #ifdef __GAME_SERVER__

#ifdef GMTOOL
	m_bGmtarget = false;
#endif // GMTOOL

#ifdef NEW_GUILD
//	m_avelevel			= 0;
//	m_entireMemberCount	= 0;
	m_landcount = 0;
	memset(m_land, -1, sizeof(m_land) );
	m_GuildPoint		= 0;
	m_incline			= 0;
	m_maxmember = 0;
#ifdef NEW_GUILD_POINT_RANKING_NOTICE	
	m_GuildPointRanking = 0;
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD
#ifdef DRATAN_CASTLE
	m_bRebirthPos = false;
#endif // DRATAN_CASTLE

#ifdef __GAME_SERVER__
#ifdef EXTREME_CUBE
	m_cubeUniqueIdx = -1;
#endif // EXTREME_CUBE
#endif // __GAME_SERVER__
}

CGuild::~CGuild()
{
	int i;
	for (i = 0; i < GUILD_MAX_OFFICER; i++)
	{
		if (m_member[i])
			delete m_member[i];
	}
	memset(m_officer, 0, sizeof(CGuildMember*) * GUILD_MAX_OFFICER);
	memset(m_member, 0, sizeof(CGuildMember*) * GUILD_MAX_MEMBER);
	m_membercount = 0;
	m_next = NULL;
	m_prev = NULL;

#ifdef __GAME_SERVER__
#ifdef EXTREME_CUBE
	m_cubeUniqueIdx = -1;
#endif // EXTREME_CUBE
#endif // __GAME_SERVER__
}
#ifdef NEW_GUILD
int CGuild::maxmember()		
{
	if( m_level == 1 )
		return GUILD_LEVEL1_MAX_MEMBER;
	else if( m_level == 2 )
		return GUILD_LEVEL2_MAX_MEMBER;
	else if( m_level == 3 )
		return GUILD_LEVEL3_MAX_MEMBER;
	else if( m_level == 4 )
		return GUILD_LEVEL4_MAX_MEMBER;
	else if( m_level == 5 )
		return GUILD_LEVEL5_MAX_MEMBER;
#ifdef NEW_GUILD
		return m_maxmember;	
#endif // NEW_GUILD
	
}
#else
int CGuild::maxmember()
{
	switch (m_level)
	{
	case 1:			return GUILD_LEVEL1_MAX_MEMBER;
	case 2:			return GUILD_LEVEL2_MAX_MEMBER;
	case 3:			return GUILD_LEVEL3_MAX_MEMBER;
	case 4:			return GUILD_LEVEL4_MAX_MEMBER;
	case 5:			return GUILD_LEVEL5_MAX_MEMBER;
	default:		return 0;
	}

}
#endif // NEW_GUILD

int CGuild::addmember(int charindex, const char* name)
{
	CGuildMember* pmember = findmember(charindex);
	if (pmember != NULL)
		return pmember->listindex();

	int max = maxmember();
	if (m_membercount >= max)
	{
		return -1;
	}
	else
	{
		int i;
		for (i = 0; i < GUILD_MAX_MEMBER; i++)
		{
			if (m_member[i] == NULL)
			{
				pmember = new CGuildMember();
				pmember->charindex(charindex);
				pmember->name(name);
				//member nick부분 삭제 : bw 060403
//				if( (nick != NULL) && (strcmp(nick, "") != 0) )
//					pmember->nick(nick);
				pmember->pos(MSG_GUILD_POSITION_MEMBER);
				pmember->guild(this);
				pmember->listindex(i);

				m_member[i] = pmember;
				m_membercount++;

				return i;
			}
		}
		return -1;
	}
}

CGuildMember* CGuild::changeboss(int newbosslistindex)
{
	if (newbosslistindex < 0 || newbosslistindex >= GUILD_MAX_MEMBER)
		return NULL;

	CGuildMember* ret = NULL;

	if (m_member[newbosslistindex])
	{
		if (m_boss)
		{
			ret = m_boss;
			m_boss->pos(MSG_GUILD_POSITION_MEMBER);
			m_boss = NULL;
		}
		if (m_member[newbosslistindex]->pos() == MSG_GUILD_POSITION_OFFICER)
		{
			fire(newbosslistindex);
		}
		m_boss = m_member[newbosslistindex];
		m_boss->pos(MSG_GUILD_POSITION_BOSS);
	}

	return ret;
}

CGuildMember* CGuild::findmember(int charindex)
{
	int i;
	for (i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		if (m_member[i])
		{
			if (m_member[i]->charindex() == charindex)
				return m_member[i];
		}
	}
	return NULL;
}

#ifdef __GAME_SERVER__
//#ifdef ADMIN_KICKGUILDMEM
CGuildMember* CGuild::findmember(const char* charname)
{
	int i;
	for (i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		if (m_member[i])
		{
			if (strcmp(m_member[i]->GetName(), charname) == 0)
				return m_member[i];
		}
	}
	return NULL;
}
//#endif // ADMIN_KICKGUILDMEM
#endif // __GAME_SERVER__

CGuildList::CGuildList()
{
	m_head = NULL;
	m_count = 0;
}

CGuildList::~CGuildList()
{
	while (m_head)
	{
		CGuild* p = m_head;
		m_head = m_head->nextguild();
		delete p;
	}
}

CGuild* CGuildList::create(int guildindex, const char* guildname, int charindex, const char* bossname)
{
	if (guildname == NULL || guildindex < 1 || charindex < 1 || bossname == NULL)
		return NULL;
	if (strlen(guildname) < 1 || strlen(bossname) < 1)
		return NULL;

	// 길드 만들고
	CGuild* guild = new CGuild(guildindex, guildname, 1);
	// 길드에 추가
	int listindex = guild->addmember(charindex, bossname);
	// 보스 지정
	guild->changeboss(listindex);

	return guild;
}

CGuild* CGuildList::create(int guildindex, const char* guildname, int guildlevel, int battleIndex, int battlePrize, int battleTime, int battleZone, int battleKillCount, int battleState)
{
	if (guildname == NULL || guildindex < 1 || guildlevel < 1 || guildlevel > GUILD_MAX_GUILD_LEVEL)
		return NULL;
	if (strlen(guildname) < 1)
		return NULL;

	if (battlePrize > GUILD_BATTLE_MAX_PRIZE * 2)
		return NULL;

	if (battleIndex != -1)
	{
		if (battleZone != ZONE_START && battleZone != ZONE_DRATAN && battleZone != ZONE_MERAC && battleZone != ZONE_EGEHA)
			return NULL;
		if (battleKillCount < 0 || battleState < 0)
			return NULL;
	}

	CGuild* guild = new CGuild(guildindex, guildname, guildlevel, battleIndex, battlePrize, battleTime, battleZone, battleKillCount, battleState);

	return guild;
}

void CGuildList::add(CGuild* guild)
{
	if (guild == NULL)
		return ;

	if (m_head)
	{
		m_head->prevguild(guild);
	}
	guild->prevguild(NULL);
	guild->nextguild(m_head);
	m_head = guild;
	m_count++;
}

CGuild* CGuildList::findguild(int guildindex)
{
	CGuild* ret = m_head;
	while (ret)
	{
		if (ret->index() == guildindex)
			return ret;

		ret = ret->nextguild();
	}
	return NULL;
}

#ifdef __GAME_SERVER__
//#ifdef ADMIN_KICKGUILDMEM
CGuild* CGuildList::findguild(const char* guildname)
{
	CGuild* ret = m_head;
	while (ret)
	{
		if (strcmp(ret->name(), guildname) == 0)
			return ret;

		ret = ret->nextguild();
	}
	return NULL;
}
//#endif // ADMIN_KICKGUILDMEM
#endif // __GAME_SERVER__

CGuildMember* CGuildList::findmember(int charindex)
{
	// TODO : GUILD : 길드 트리시 2개 길드에 모두 있는 경우 검사
	CGuildMember* ret = NULL;
	CGuild* p = m_head;
	while (p)
	{
		ret = p->findmember(charindex);
		if (ret)
			return ret;
		else
			p = p->nextguild();
	}
	return ret;
}

#ifdef __GAME_SERVER__
void CGuild::SendToAll(CNetMsg& msg, bool bCell)
{
	int i;
	for (i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		if (m_member[i] != NULL)
		{
			if (m_member[i]->GetPC() != NULL
// << kjban
				&& m_member[i]->online() == 1				// 온라인 상태인 경우 	 
				&& m_member[i]->GetPC()->m_desc != NULL)	// 디스크립터가 존재 하는 경우
// >>
			{
				SEND_Q(msg, m_member[i]->GetPC()->m_desc);
				if(bCell)
				{
					CNetMsg appearMsg;
					DisappearMsg(appearMsg, m_member[i]->GetPC() );
					m_member[i]->GetPC()->m_pArea->SendToCell(appearMsg, m_member[i]->GetPC() );
					AppearMsg(appearMsg, m_member[i]->GetPC(), true);
					m_member[i]->GetPC()->m_pArea->SendToCell(appearMsg, m_member[i]->GetPC() );
				}
			}
		}
	}
}

void CGuild::SendToAllInSameZone(CNetMsg& msg, int zoneindex)
{
	int i;
	for (i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		if (m_member[i])
		{
			if (m_member[i]->GetPC())
			{
				if (m_member[i]->GetPC()->m_pZone->m_index == zoneindex)
					SEND_Q(msg, m_member[i]->GetPC()->m_desc);
			}
		}
	}
}
#endif

void CGuildList::Reset()
{
	CGuild* p = m_head;
	while (p)
	{
		m_head = p->nextguild();
		delete p;
		p = m_head;
	}
	m_count = 0;
}

void CGuildList::Remove(CGuild* guild)
{
	// TODO : GUILD : 트리구성시 최상위 길드외의 길드 처리

	// 해당 길드원 정보 변경
	int i;
	for (i = 0; i < guild->maxmember(); i++)
		guild->removemember(guild->member(i));

	CGuild* p = m_head;
	while (p)
	{
		if (p == guild)
			break;
		else
			p = p->nextguild();
	}
	if (p == NULL)
		return ;

	if (p->prevguild())
		p->prevguild()->nextguild(p->nextguild());
	if (p->nextguild())
		p->nextguild()->prevguild(p->prevguild());
	if (p == m_head)
		m_head = p->nextguild();
	p->prevguild(NULL);
	p->nextguild(NULL);
	m_count--;
	delete p;
}

void CGuild::removemember(CGuildMember* delmember)
{
	if (delmember == NULL)
		return ;

	int i;
	for (i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		if (m_member[i] == delmember)
		{
#ifdef __GAME_SERVER__
			if (delmember->GetPC())
				delmember->GetPC()->m_guildInfo = NULL;
			delmember->SetPC(NULL);
#endif
//			if (delmember->pos() == MSG_GUILD_POSITION_OFFICER)
//			{
				int j;
				for (j = 0; j < GUILD_MAX_OFFICER; j++)
				{
					if (m_officer[j] == delmember)
						m_officer[j] = NULL;
				}
//			}
			delete delmember;
			m_member[i] = NULL;
			m_membercount--;
			return ;
		}
	}
}

void CGuild::appoint(int listindex)
{
	if (listindex < 0 || listindex >= GUILD_MAX_MEMBER)
		return ;

	if (m_member[listindex])
	{
		int i;
		for (i = 0; i < GUILD_MAX_OFFICER; i++)
		{
			if (m_officer[i] == NULL)
			{
				m_officer[i] = m_member[listindex];
				m_officer[i]->pos(MSG_GUILD_POSITION_OFFICER);
				return ;
			}
		}
	}
}

void CGuild::fire(int listindex)
{
	if (listindex < 0 || listindex >= GUILD_MAX_MEMBER)
		return ;

	if (m_member[listindex])
	{
		int i;
		for (i = 0; i < GUILD_MAX_OFFICER; i++)
		{
			if (m_officer[i] == m_member[listindex])
			{
				m_officer[i]->pos(MSG_GUILD_POSITION_MEMBER);
				m_officer[i] = NULL;
				return ;
			}
		}
	}
}
