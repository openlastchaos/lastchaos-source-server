#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"
#include "DratanCastle.h"

#ifdef ENABLE_WAR_CASTLE
void do_ExCastleMapRecent(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	CWarCastle* castle = CWarCastle::GetCastleObject(CWarCastle::GetCurSubServerCastleZoneIndex());
	if (!castle)
		return ;
	if (ch->m_pZone->m_index != castle->GetZoneIndex())
		return ;

	switch (ch->GetJoinFlag(castle->GetZoneIndex()))
	{
	case WCJF_DEFENSE_GUILD:
	case WCJF_ATTACK_GUILD:
		break;

	default:
		return ;
	}

	ExCastleMapRecentMsg(rmsg, castle, ch);
	SEND_Q(rmsg, ch->m_desc);
}

void do_ExCastleMapSignal(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	float x;
	float z;
	msg >> x
		>> z;

	if (ch->m_guildInfo == NULL)
		return ;

	switch (ch->m_guildInfo->pos())
	{
	case MSG_GUILD_POSITION_BOSS:
	case MSG_GUILD_POSITION_OFFICER:
		break;

	default:
		return ;
	}

	CWarCastle* castle = CWarCastle::GetCastleObject(CWarCastle::GetCurSubServerCastleZoneIndex());
	if (!castle)
		return ;
	if (ch->m_pZone->m_index != castle->GetZoneIndex())
		return ;

	switch (ch->GetJoinFlag(castle->GetZoneIndex()))
	{
	case WCJF_DEFENSE_GUILD:
	case WCJF_ATTACK_GUILD:
		break;

	default:
		return ;
	}

	if (ch->m_guildInfo->guild() == NULL)
		return ;

	ch->m_guildInfo->guild()->guildSignal(ch->m_guildInfo->pos(), ch->m_index, x, z);

	ExCastleMapSignalMsg(rmsg, ch->m_guildInfo->pos(), ch, x, z);
	ch->m_guildInfo->guild()->SendToAllInSameZone(rmsg, ch->m_pZone->m_index);
}
#endif // #ifdef ENABLE_WAR_CASTLE