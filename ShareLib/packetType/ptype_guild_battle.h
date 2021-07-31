#ifndef __PTYPE_GUILD_BATTLE_H__
#define __PTYPE_GUILD_BATTLE_H__

#include "ptype_base.h"
#ifndef _CLIENT_
#include "ptype_server_to_server.h"
#endif

//subtype
enum
{
	MSG_SUB_GUILD_BATTLE_REGIST,					//길드전투 두번째 등록버튼
	MSG_SUB_GUILD_BATTLE_CHALLENGE,					//길드전투 두번째 도전버튼
	MSG_SUB_GUILD_BATTLE_CANCEL,					//길드전투 취소버튼
	MSG_SUB_GUILD_BATTLE_LIST,						//길드전투 리스트
	MSG_SUB_GUILD_BATTLE_CHALLENGE_AGREE_UPDATE,	//도전 수락여부
	MSG_SUB_GUILD_BATTLE_CHALLENGE_AGREE,	
	MSG_SUB_GUILD_BATTLE_ERROR,				
};

//error_code
enum
{
	//GuildBattleErrorcode
	GUILD_BATTLE_ERROR_AREADY,					//이미 성사되었거나 취소된 길드입니다.
	GUILD_BATTLE_ERROR_CONDITION_GUILD_MASTER,	// [1086]길드장만 길드전투를 신청 할 수 있습니다.
	GUILD_BATTLE_ERROR_CONDITION,				//길드전투 챌린지에 등록할 수 없는 상태입니다.
	GUILD_BATTLE_SUCCESS_REGIST,				//길드가 길드전투 챌린지에 등록되었습니다.
	GUILD_BATTLE_SUCCESS_CANCEL,				//길드의 길드전투 챌린지의 등록이 취소되었습니다.
	GUILD_BATTLE_FAIL_CHALLENGE,				//길드전투 도전을 할 수 없는 상태입니다.

	//GuildBattleChallenge
	GUILD_BATTLE_SUCCESS_CHALLENGE,				//길드전투를 신청하였습니다.
	
	//GuildBattleChallengeAgree
	GUILD_BATTLE_SUCCESS_AGREE,					//길드전투 신청을 받아드렸습니다. 즉시 길드전투가 시작됩니다.
	GUILD_BATTLE_SUCCESS_REFUSE,				//길드전투 신청을 거부하였습니다.
};

#pragma pack(push, 1)
namespace RequestClient
{
	//팝업창 등록하기 버튼
	struct GuildBattleRegist : public pTypeBase
	{
		LONGLONG stake_nas;
		int stake_gp;
		int guild_battle_time; //min
		int zone_index;
	};

	struct GuildBattleChallenge : public pTypeBase
	{
		int guild_index;
	};

	struct GuildBattleRegCancel : public pTypeBase
	{

	};

	struct GuildBattleList : public pTypeBase
	{
		int guild_battle_time;		//0 : 전체
		int page_num;
	};

	struct GuildBattleChallengeAgree : public pTypeBase
	{
		int char_index;		//신청한 캐릭터 인덱스
		bool isAgree;		//true : yes, false : no
	};
}

namespace ResponseClient
{
	struct GuildBattleData
	{
		int guild_index;
		char guild_name[MAX_GUILD_NAME_LENGTH]; //MAX_GUILD_NAME_LENGTH = 50Byte
		char guild_master_name[MAX_CHAR_NAME_LENGTH]; //MAX_CHAR_NAME_LENGTH = 50Byte
		int guild_member_count;
		int guild_member_level_aver;
		int guild_win_count;
		int guild_battle_count;
		LONGLONG stake_nas;
		int stake_gp;
		int guild_battle_time;	//min
		int zone_index;
	};
	
	//이부분 클라이언트에서 받을 필요 없는 데이터인듯 하나 일단 넣어놨습니다. 확인 후 필요 없으시면 말해주세요
	struct GuildBattleChallenge : public pTypeBase
	{
		char guild_name[MAX_GUILD_NAME_LENGTH]; //MAX_GUILD_NAME_LENGTH = 50Byte
		int error_code;
	};
	//

	struct GuildBattleList : public pTypeBase
	{
		int count;
		bool isNext;			//다음페이지 존재 여부
		bool isReg;				//길드전투 등록여부
		GuildBattleData data[0];
	};
	
	struct GuildBattleErrorcode : public pTypeBase
	{
		int error_code;
	};

#ifndef _CLIENT_
	struct GuildBattleCheckNasGp : public pTypeBase
	{
		int char_index;
		LONGLONG guild_nas;
		int guild_point;
	};
#endif

	struct GuildBattleChallengeAgree : public pTypeBase
	{
		char guild_name[MAX_GUILD_NAME_LENGTH];
		int zone_index;		//error_code 거절시 zone_index -1 로 전달 (출력 필요X)
		int error_code;
	};
}

namespace UpdateClient
{
	struct GuildBattleChallengeAgreeUp : public pTypeBase
	{
		char guild_name[MAX_GUILD_NAME_LENGTH];
		int guild_level;
		int guild_member_count;
		int guile_member_level_aver;
		int guild_online_member_count;
		int guile_online_member_level_aver;
		int guild_win_count;
		int guild_battle_count;
		LONGLONG guild_nas;
		int guild_point;
		int char_index;		//신청한 캐릭터 인덱스
	};
}

#ifndef _CLIENT_

inline void makeGuildBattleErrorMsg(CNetMsg::SP& rmsg, int error_code)
{
	ResponseClient::GuildBattleErrorcode* p = reinterpret_cast<ResponseClient::GuildBattleErrorcode*>(rmsg->m_buf);
	p->type = MSG_GUILD_BATTLE;
	p->subType = MSG_SUB_GUILD_BATTLE_ERROR;
	p->error_code = error_code;
	rmsg->setSize(sizeof(ResponseClient::GuildBattleErrorcode));
}

inline void makeGuildBattleChallengeAgreeMsgUpdate(CNetMsg::SP& rmsg, int level, int member_count, int level_aver, int win_count, int battle_count, LONGLONG nas, int guild_point, int char_index, const char* name)
{
	UpdateClient::GuildBattleChallengeAgreeUp* p = reinterpret_cast<UpdateClient::GuildBattleChallengeAgreeUp*>(rmsg->m_buf);
	p->type = MSG_GUILD_BATTLE;
	p->subType = MSG_SUB_GUILD_BATTLE_CHALLENGE_AGREE_UPDATE;
	p->guild_level = level;
	strncpy(p->guild_name, name, MAX_GUILD_NAME_LENGTH);
	p->guild_member_count = member_count;
	p->guile_member_level_aver = level_aver;
	p->guild_win_count = win_count;
	p->guild_battle_count = battle_count;
	p->guild_nas = nas;
	p->guild_point = guild_point;
	p->char_index = char_index;
	rmsg->setSize(sizeof(UpdateClient::GuildBattleChallengeAgreeUp));
}

inline void MakeGuildBattleChallengeAgreeMsgRes(CNetMsg::SP& rmsg, const char* guild_name, int error_code, int zone_index)
{
	ResponseClient::GuildBattleChallengeAgree* p = reinterpret_cast<ResponseClient::GuildBattleChallengeAgree*>(rmsg->m_buf);
	p->type = MSG_GUILD_BATTLE;
	p->subType = MSG_SUB_GUILD_BATTLE_CHALLENGE_AGREE;
	strncpy(p->guild_name, guild_name, MAX_GUILD_NAME_LENGTH);
	p->error_code = error_code;
	p->zone_index = zone_index;
	rmsg->setSize(sizeof(ResponseClient::GuildBattleChallengeAgree));
}

inline void MakeGuildBattleChallengeMsg(CNetMsg::SP& rmsg, const char* guild_name, int error_code)
{
	ResponseClient::GuildBattleChallenge* p = reinterpret_cast<ResponseClient::GuildBattleChallenge*>(rmsg->m_buf);
	p->type = MSG_GUILD_BATTLE;
	p->subType = MSG_SUB_GUILD_BATTLE_CHALLENGE;
	p->error_code = error_code;
	strncpy(p->guild_name, guild_name, MAX_GUILD_NAME_LENGTH);
	rmsg->setSize(sizeof(ResponseClient::GuildBattleChallenge));
}





#endif //_CLIENT_

#pragma pack(pop)
#endif