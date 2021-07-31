#include "stdhdrs.h"
#include "Server.h"
#include "Log.h"
#include "CmdMsg.h"
#include "WarCastle.h"
#include "DratanCastle.h"

#ifdef ENABLE_WAR_CASTLE

void CheckWarCastle()
{
	meracCastle.CheckWarCastle();
#ifdef DRATAN_CASTLE 
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if(pCastle != NULL)
	{
		pCastle->CheckWarCastle();
	}
#endif // DRATAN_CASTLE
}

bool LoadCastleData()
{
	// 메라크
	if (!meracCastle.LoadCastleData())
		return false;

#ifdef DRATAN_CASTLE
	// 드라탄(성인 서버만 드라탄 공성이 있다)
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL && pCastle->LoadCastleData() == false)
	{
		return false;
	}
#endif // DRATAN_CASTLE

	return true;
}

bool IsValidGuildCommandWithWarCastle(CPC* ch, unsigned char subtype)
{
	// 공성 참여 유저는 길드 관련 커맨드 중 일부를 할 수 없음
#ifndef DRATAN_CASTLE
	int warZoneCount = 1;
	int warZoneIndex[] = { ZONE_MERAC };
#else
	int warZoneCount = 2;
	int warZoneIndex[] = { ZONE_MERAC, ZONE_DRATAN };
#endif // DRATAN_CASTLE

	int i;
	for (i = 0; i < warZoneCount; i++)
	{
		CWarCastle* castle = CWarCastle::GetCastleObject(warZoneIndex[i]);
		if (castle)
		{
			if (castle->GetState() != WCSF_NORMAL && ch->GetJoinFlag(warZoneIndex[i]) != WCJF_NONE)
			{
				switch (subtype)
				{
				case MSG_GUILD_CREATE:
				case MSG_GUILD_LEVELUP:
				case MSG_GUILD_BREAKUP:
				case MSG_GUILD_REGIST_REQ:
				case MSG_GUILD_REGIST_ALLOW:
				case MSG_GUILD_REGIST_CANCEL:
				case MSG_GUILD_OUT_REQ:
				case MSG_GUILD_KICK:
				case MSG_GUILD_CHANGE_BOSS:
				case MSG_GUILD_APPOINT_OFFICER:
				case MSG_GUILD_FIRE_OFFICER:
				case MSG_GUILD_BATTLE_REQ_REQ:
				case MSG_GUILD_BATTLE_REQ_REJECT:
				case MSG_GUILD_BATTLE_REQ_ACCEPT:
				case MSG_GUILD_BATTLE_STOP_REQ:
				case MSG_GUILD_BATTLE_STOP_REJECT:
				case MSG_GUILD_BATTLE_STOP_ACCEPT:
					return false;
				}
			}
		}
	}

	return true;
}

#endif // ENABLE_WAR_CASTLE