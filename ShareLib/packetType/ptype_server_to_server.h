#ifndef __PTYPE_SERVER_TO_SERVE_H__
#define __PTYPE_SERVER_TO_SERVE_H__

#include "ptype_base.h"

enum
{
	MSG_SUB_SERVERTOSERVER_USER_LOGIN,
	MSG_SUB_SERVERTOSERVER_USER_LOGOUT,
	MSG_SUB_SERVERTOSERVER_TRADEAGENT_FEE,

	MSG_SUB_SERVERTOSERVER_CHANGE_NICK_NAME,

	MSG_SUB_SERVERTOSERVER_KICK_REQ,
	MSG_SUB_SERVERTOSERVER_KICK_ANSER,
	MSG_SUB_SERVERTOSERVER_KICK_BY_CHAR_INDEX_REQ,
	MSG_SUB_SERVERTOSERVER_KICK_BY_CHAR_INDEX_ANSWER,
	MSG_SUB_SERVERTOSERVER_KICK_BY_USER_INDEX_REQ,
	MSG_SUB_SERVERTOSERVER_KICK_BY_USER_INDEX_ANSWER,
	MSG_SUB_SERVERTOSERVER_KICK_BY_USER_ID_REQ,
	MSG_SUB_SERVERTOSERVER_KICK_BY_USER_ID_ANSER,

	MSG_SUB_GUILD_BATTLE_CHALLENGE_TO_HELPER,
	MSG_SUB_GUILD_BATTLE_REG_TO_HELPER,
	MSG_SUB_GUILD_BATTLE_STASH_LOCK_OFF,
	MSG_SUB_GUILD_BATTLE_STASH_LOCK_OFF_ALL,
};

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////

namespace ServerToServerPacket
{
struct LoginUserInfo : public pTypeBase
{
	int userIndex;
	int charIndex;
	int serverNo;
	int zoneNo;
	char userId[MAX_ID_NAME_LENGTH + 1];
	char charName[MAX_CHAR_NAME_LENGTH + 1];
};

struct LogoutUserInfo : public pTypeBase
{
	int userIndex;
};

struct changeNickName : public pTypeBase
{
	char	oldNickName[MAX_CHAR_NAME_LENGTH + 1];
	char	newNickName[MAX_CHAR_NAME_LENGTH + 1];
};

struct GuildBattleChallengeToHelper : public pTypeBase
{
	int guild_index;
	int char_index;
	int target_char_index;
	int target_guild_index;
	LONGLONG stake_nas;
	int error_code;
};

struct GuildBattleRegToHelper : public pTypeBase
{
	int guild_index;
	int char_index;
	LONGLONG stake_nas;
	int stake_gp;
	int guild_battle_time;
	int error_code;
	int ave_level;
	int zone_index;
};

struct GuildBattleStashLockOff : public pTypeBase
{
	int guild_index;
};

struct GuildBattleStashLockOffAll : public pTypeBase
{

};

//////////////////////////////////////////////////////////////////////////
inline void makeLogoutUserInfo(CNetMsg::SP& msg, int userIndex)
{
	LogoutUserInfo* packet = reinterpret_cast<LogoutUserInfo*>(msg->m_buf);

	packet->type = MSG_SERVER_TO_SERVER;
	packet->subType = MSG_SUB_SERVERTOSERVER_USER_LOGOUT;
	packet->userIndex = userIndex;

	msg->setSize(sizeof(ServerToServerPacket::LogoutUserInfo));
}

inline void makeChangeNickName(CNetMsg::SP& msg, const char* oldnickname, const char* newnickname)
{
	changeNickName* packet = reinterpret_cast<changeNickName*>(msg->m_buf);
	packet->type = MSG_SERVER_TO_SERVER;
	packet->subType = MSG_SUB_SERVERTOSERVER_CHANGE_NICK_NAME;
	memcpy(packet->oldNickName, oldnickname, sizeof(packet->oldNickName));
	packet->oldNickName[MAX_CHAR_NAME_LENGTH] = '\0';
	memcpy(packet->newNickName, newnickname, sizeof(packet->newNickName));
	packet->newNickName[MAX_CHAR_NAME_LENGTH] = '\0';
	msg->setSize(sizeof(changeNickName));
}

//ÇďĆŰ¸Ţ˝ĂÁö
inline void makeGuildBattleChallengeToHelperMsg(CNetMsg::SP& rmsg, int guild_index, int target_guild_index, int target_index, int char_index, int stake_nas)
{
	GuildBattleChallengeToHelper* p = reinterpret_cast<GuildBattleChallengeToHelper*>(rmsg->m_buf);
	p->type = MSG_SERVER_TO_SERVER;
	p->subType = MSG_SUB_GUILD_BATTLE_CHALLENGE_TO_HELPER;
	p->guild_index = guild_index;
	p->target_guild_index = target_guild_index;
	p->target_char_index = target_index;
	p->char_index = char_index;
	p->stake_nas = stake_nas;
	rmsg->setSize(sizeof(GuildBattleChallengeToHelper));
}

inline void makeGuildBattleRegToHelperMsg(CNetMsg::SP& rmsg, int guild_index, int char_index, LONGLONG nas, int guild_point, int battle_time, int zone_index)
{
	GuildBattleRegToHelper* p = reinterpret_cast<GuildBattleRegToHelper*>(rmsg->m_buf);
	p->type = MSG_SERVER_TO_SERVER;
	p->subType = MSG_SUB_GUILD_BATTLE_REG_TO_HELPER;
	p->guild_index = guild_index;
	p->stake_nas = nas;
	p->stake_gp = guild_point;
	p->guild_battle_time = battle_time;
	p->char_index = char_index;
	p->zone_index = zone_index;
	rmsg->setSize(sizeof(GuildBattleRegToHelper));
}

inline void makeGuildBattleStashLockOff(CNetMsg::SP& rmsg, int guild_index)
{
	GuildBattleStashLockOff* p = reinterpret_cast<GuildBattleStashLockOff*>(rmsg->m_buf);
	p->type = MSG_SERVER_TO_SERVER;
	p->subType = MSG_SUB_GUILD_BATTLE_STASH_LOCK_OFF;
	p->guild_index = guild_index;
	rmsg->setSize(sizeof(GuildBattleStashLockOff));
}

inline void makeGuildBatlleStashLockOffALL(CNetMsg::SP& rmsg)
{
	GuildBattleStashLockOffAll* p = reinterpret_cast<GuildBattleStashLockOffAll*>(rmsg->m_buf);
	p->type = MSG_SERVER_TO_SERVER;
	p->subType = MSG_SUB_GUILD_BATTLE_STASH_LOCK_OFF_ALL;
	rmsg->setSize(sizeof(GuildBattleStashLockOffAll));
}
//////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
