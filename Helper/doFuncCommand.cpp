#include "stdhdrs.h"
#include "Server.h"
#include "doFunc.h"
#include "Guild.h"
#include "CmdMsg.h"
#include "DBCmd.h"
#include "Log.h"
//#include "Friend.h"

void do_Command(CNetMsg& msg, CDescriptor* dest)
{
	int subtype = -1;
	msg.MoveFirst();
	msg >> subtype;

	switch (subtype)
	{
#ifdef GUILD_RANKING
	case MSG_HELPER_SAVE_EXP_GUILD:
		do_CommandSaveExpGuild(msg, dest);
		break;
#endif // GUILD_RANKING


#ifdef LC_HBK
	case MSG_HELPER_LEVELUP_LOG:
		do_LevelUpLog(msg, dest);
		break;
#endif // LC_HBK

#ifdef PARTY_MATCHING
	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO:
		do_CommandPartyMatchMemberChangeInfoName(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_REG_MEMBER_REQ:
		do_CommandPartyMatchRegMemberReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_REG_PARTY_REQ:
		do_CommandPartyMatchRegPartyReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_DEL_REQ:
		do_CommandPartyMatchDelReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_INVITE_REQ:
		do_CommandPartyMatchInviteReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_JOIN_REQ:
		do_CommandPartyMatchJoinReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_JOIN_ALLOW_REQ:
		do_CommandPartyMatchJoinAllowReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_JOIN_REJECT_REQ:
		do_CommandPartyMatchJoinRejectReq(msg, dest);
		break;
#endif // PARTY_MATCHING

	case MSG_HELPER_PARTY_CHANGE_BOSS_REQ:
		do_CommandPartyChangeBossReq(msg, dest);
		break;

#ifdef NEW_UI
	case MSG_HELPER_PARTY_CHANGE_TYPE_REQ:
		do_CommandPartyChangeTypeReq(msg, dest);
#endif // NEW_UI

#ifdef PARTY_RAID
	case MSG_HELPER_PARTY_END_PARTY_REQ:
		do_CommandPartyEndPartyReq(msg, dest);
#endif //PARTY_RAID

	case MSG_HELPER_PARTY_KICK_REQ:
		do_CommandPartyKickReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_QUIT_REQ:
		do_CommandPartyQuitReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_REJECT_REQ:
		do_CommandPartyRejectReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_ALLOW_REQ:
		do_CommandPartyAllowReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_INVITE_REP:
		do_CommandPartyInviteRep(msg, dest);
		break;

	case MSG_HELPER_PARTY_INVITE_REQ:
		do_CommandPartyInviteReq(msg, dest);
		break;

	case MSG_HELPER_SHUTDOWN:
		do_CommandShutdown(msg, dest);
		break;

	case MSG_HELPER_GUILD_CREATE_REQ:
		do_CommandGuildCreateReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_ONLINE:
		do_CommandGuildOnline(msg, dest);
		break;

	case MSG_HELPER_GUILD_LOAD_REQ:
		do_CommandGuildLoadReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_LEVELUP_REQ:
		do_CommandGuildLevelUpReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_BREAKUP_REQ:
		do_CommandGuildBreakUpReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_MEMBER_ADD_REQ:
		do_CommandGuildMemberAddReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_MEMBER_OUT_REQ:
		do_CommandGuildMemberOutReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_MEMBER_KICK_REQ:
		do_CommandGuildMemberKickReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_CHANGE_BOSS_REQ:
		do_CommandGuildChangeBossReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_APPOINT_OFFICER_REQ:
		do_CommandGuildAppointOfficerReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_CHAT:
		do_CommandGuildChat(msg, dest);
		break;

	case MSG_HELPER_GUILD_FIRE_OFFICER_REQ:
		do_CommandGuildFireOfficerReq(msg, dest);
		break;

	case MSG_HELPER_CHAR_DEL:
		do_CommandCharacterDelete(msg, dest);
		break;

	case MSG_HELPER_GUILD_BATTLE_REQ:
		do_CommandGuildBattleReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_BATTLE_STOP_REQ:
		do_CommandGuildBattleStopReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_BATTLE_PEACE_REQ:
		do_CommandGuildBattlePeaceReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_BATTLE_KILL_REQ:
		do_CommandGuildBattleKillReq(msg, dest);
		break;
//0503 kwon
	case MSG_HELPER_EVENT_MOONSTONE_UPDATE_REQ:
		do_CommandEventMoonStoneUpdateReq(msg, dest);
		break;
	case MSG_HELPER_EVENT_MOONSTONE_JACKPOT_REQ:
		do_CommandEventMoonStoneJackPotReq(msg, dest);
		break;

	case MSG_HELPER_FRIEND_MEMBER_ADD_REQ:
		do_CommandFriendMemberAddReq(msg, dest);
		break;
	case MSG_HELPER_FRIEND_MEMBER_DEL_REQ:
		do_CommandFriendMemberDelReq(msg, dest);
		break;
	case MSG_HELPER_FRIEND_SET_CONDITION_REQ:
		do_CommandFriendSetConditionReq(msg,dest);
		break;

//0616 bw
	case MSG_HELPER_PD4_RANK_INIT:
		do_CommandPD4RankInit(msg, dest);
		break;
	case MSG_HELPER_PD4_RANK_ADD:
		do_CommandPD4RankAdd(msg, dest);
		break;
	case MSG_HELPER_PD4_RANK_END:
		do_CommandPD4RankEnd(msg, dest);
		break;
	case MSG_HELPER_PD4_RANK_VIEW_REQ:
		do_CommandPD4RankView(msg, dest);
		break;
	case MSG_HELPER_PD4_REWARD_VIEW_REQ:
		do_CommandPD4RewardView(msg, dest);
		break;
	case MSG_HELPER_PD4_REWARD_REQ:
		do_CommandPD4Reward(msg, dest);
		break;
	case MSG_HELPER_PD4_REWARD_INIT:
		do_CommandPD4RewardInit(msg, dest);
		break;
	case MSG_HELPER_PD4_REWARD_END:
		do_CommandPD4RewardEnd(msg, dest);
		break;

	case MSG_HELPER_NAME_CHANGE_REQ:
		do_CommandNameChange(msg, dest);
	

// 애완동물
	case MSG_HELPER_PET_CREATE_REQ:
		do_CommandPetCreateReq(msg, dest);
		break;
//TODO : DELETE

#ifdef NEW_UI
	case MSG_HELPER_PET_DELETE_REQ:
		do_CommandPetDeleteReq(msg, dest);
		break;
#endif // NEW_UI

// 길드 나스 보관함
	case MSG_HELPER_GUILD_STASH_HISTORY_REQ:
		do_CommandGuildStashHistoryReq(msg, dest);
		break;
	case MSG_HELPER_GUILD_STASH_VIEW_REQ:
		do_CommandGuildStashViewReq(msg, dest);
		break;
	case MSG_HELPER_GUILD_STASH_TAKE_REQ:
		do_CommandGuildStashTakeReq(msg, dest);
		break;
	case MSG_HELPER_GUILD_STASH_ROLLBACK:
		do_CommandGuildStashRollback(msg, dest);
		break;
	case MSG_HELPER_GUILD_STASH_SAVE_TAX_REQ:
		do_CommandGuildStashSaveTaxReq(msg, dest);
		break;

	case MSG_HELPER_TEACHER_FAMEUP_REQ:
		do_CommandFameUp(msg, dest);
		break;
	case MSG_HELPER_TEACHER_REGISTER_REQ:
		do_CommandTeacherRegister(msg, dest);
		break;
	case MSG_HELPER_TEACHER_GIVEUP_REQ:
		do_CommandTeacherGiveup(msg, dest);
		break;
	case MSG_HELPER_TEACHER_REQ:
		do_CommandTeacher(msg, dest);
		break;
	case MSG_HELPER_TEACHER_TIMEOVER_REQ:
		do_CommandTeacherTimeover(msg, dest);
		break;
	case MSG_HELPER_TEACHER_SUPERSTONERECIEVE:
		do_CommandTeacherSuperstoneRecieve(msg, dest);
		break;
	case MSG_HELPER_TEACHER_LOAD_REQ:
		do_CommandTeacherLoad(msg, dest);
		break;
	case MSG_HELPER_MARGADUM_PVP_RANK:
		do_CommandMargadumPvpRank(msg, dest);
		break;
		//휴면케릭 이벤트
	case MSG_HELPER_SEARCHFRIEND_ADD_REQ:			//휴면케릭 등록하는 부분
		do_CommandEventSearchFriendMemberAddReq(msg, dest);
		break;
	case MSG_HELPER_SEARCHFRIEND_SELECT_ADD_REQ:			// 케릭 등록 선택
		do_CommandEventSearchFriendMemberSelectAddReq(msg, dest);
		break;
	case MSG_HELPER_SEARCHFRIEND_ONETIME_ADD_REQ:			// 한시간 단위 체크
		do_CommandEventSearchFriendMemberOneTimeCheckReq(msg, dest);
		break;
	case MSG_HELPER_SEARCHFRIEND_LISTGOOD_REQ:			// 등록된 친구 보상가능여부
		do_CommandEventSearchFriendMemberListGoodReq(msg, dest);
		break;
	case MSG_HELPER_SEARCHFRIEND_GOOD_REQ:
		do_CommandEventSearchFriendMemberGoodReq(msg, dest);
		break;
#ifdef MESSENGER_NEW
	case MSG_HELPER_BLOCKPC_REQ:
		do_CommandBlockCharReq(msg, dest);
		break;
#endif
#ifdef CASH_ITEM_GIFT
	case MSG_HELPER_GIFT_RECVCHARNAME_REQ:
		do_CommandGiftCharReq(msg, dest);
		break;
#endif

#ifdef EVENT_2007_PARENTSDAY
	case MSG_HELPER_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST:
		do_CommandParentsdayAccumulatepointList( msg, dest );
		break;
	case MSG_HELPER_PARENTSDAY_2007_ADD_POINT:
		do_CommandParentsdayAddPoint( msg, dest );
		break;
	case MSG_HELPER_PARENTSDAY_2007_EXCHANGE_TICKET:
		do_CommandParentsdayExchangeTicket( msg, dest );
		break;

	case MSG_HELPER_PARENTSDAY_2007_EXCHANGE_ITEM:
		do_CommandParentsdayExchangeItem( msg, dest );
		break;
	
	case MSG_HELPER_PARENTSDAY_2007_EXCHANGE_ITEM_INSERT:
		do_CommandParentsdayItemInsert( msg, dest );
		break;
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_CHILDRENSDAY_2007
	case MSG_HELPER_CHILDRENSDAY_2007 :
		do_CommandChildrensDay2007( msg, dest );
		break;
#endif	// EVENT_CHILDRENSDAY_2007


#ifdef EVENT_TEACH_2007
	case MSG_HELPER_TEACH_2007:
		do_CommandTeach2007( msg, dest );
		break;
	case MSG_HELPER_TEACH_2007_ADDFLOWER:
		do_CommandTeach2007AddFlower(msg, dest);
		break;
	case MSG_HELPER_TEACH_2007_RECEIVE_FLOWER:
		do_CommandTeach2007ReciveFlower(msg, dest);
		break;
#endif // EVENT_TEACH_2007

#ifdef EVENT_FLOWERTREE_2007
	case MSG_HELPER_FLOWERTREE_2007:
		do_CommandFlowerTree2007(msg, dest);
		break;
#endif //EVENT_FLOWERTREE_2007

#ifdef NEW_GUILD
	case MSG_HELPER_GUILD_INCLINE_ESTABLISH_REQ:
		do_CommandGuildInclineEstablish( msg, dest );
		break;
	case MSG_HELPER_GUILD_MEMBER_ADJUST_REQ:
		do_CommandGuildMemberAdjust( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_INFO_REQ:
		do_CommandNewGuildInfo( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_MEMBERLIST_REQ:
		do_CommandNewGuildMemberList( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_MANAGE_REQ:
		do_CommandNewGuildManage( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_NOTICE_REQ:
		do_CommandNewGuildNotice( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_NOTICE_UPDATE_REQ:
		do_CommandNewGuildNoticeUpdate( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_REQ:
		do_CommandNewGuildNoticeTrans( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_NOTICE_SKILLLIST_REQ:
		do_CommandNewGuildSkillList( msg, dest );
		break;
//	case MSG_HELPER_SAVE_GUILD_SKILL:
//		do_CommandNewGuildSaveSkill( msg, dest );
		break;
	case MSG_HELPER_UPDATE_GUILD_POINT:
		do_CommandNewGuildUpdatePoint( msg, dest );
		break;
	case MSG_HELPER_SAVE_GUILD_POINT:
		do_CommandSaveGuildPoint( msg, dest );
		break;
	case MSG_HELPER_SAVE_GUILD_MEMBER_POINT:
		do_CommandSaveGuildMemberPoint( msg, dest );
		break;
	case MSG_HELPER_GUILD_NOTICE :
		do_CommandGuildNotice( msg, dest );
		break;
	case MSG_HELPER_GUILD_SKILL_LEARN:
		do_CommandGuildSkillLearn( msg, dest );
		break;
#endif // NEW_GUILD
		
	case MSG_HELPER_INSERT_GUARD:
		do_InsertGuard(msg, dest);
		break;

	case MSG_HELPER_CASTLE_TOWER_ADD:
		do_CastleTowerAdd(msg, dest);
		break;

	case MSG_HELPER_CASTLE_TOWER_REINFORCE:
		do_CastleTowerReinforce(msg, dest);
		break;

	case MSG_HELPER_CASTLE_QUARTERS_INSTALL:
		do_CastleQuartersInstall(msg, dest);
		break;

#ifdef EVENT_OPEN_ADULT_SERVER
	case MSG_HELPER_OPEN_ADULT_SERVER:
		do_CommandOpenAdultServer(msg, dest);
		break;
#endif // EVENT_OPEN_ADULT_SERVER
#ifdef PET_NAME_CHANGE
	case MSG_HELPER_PET_NAME_CHANGE:
		do_CommandPetChangeName( msg, dest );
		break;
#endif // PET_NAME_CHANGE

#ifdef PET_DIFFERENTIATION_ITEM
	case MSG_HELPER_PET_COLOR_CHANGE:
			do_CommandPetColorChange( msg, dest );
			break;
#endif // PET_DIFFERENTIATION_ITEM

#ifdef REWARD_IDC2007
	case MSG_HELPER_REWARD_IDC2007_REQ:
		do_CommandRewardIDC2007( msg, dest );
		break;
#endif // REWARD_IDC2007

#ifdef EVENT_HALLOWEEN_2007
	case MSG_HELPER_HALLOWEEN_2007:
		do_CommandHalloween2007( msg, dest );
		break;	
#endif //EVENT_HALLOWEEN_2007
		
#ifdef DYNAMIC_DUNGEON
	case MSG_HELPER_DVD_RATE_CHANGE:
		do_CommandDVDRateChange( msg, dest );
		break;
#endif //DYNAMIC_DUNGEON

#ifdef SAVE_MOONSTONE_MIX

	case MSG_HELPER_SAVE_MOONSTONE_MIX:
		do_CommandSaveMoonStoneMix( msg, dest );
		break;
#endif // SAVE_MOONSTONE_MIX
		
#ifdef PET_TURNTO_NPC
	case MSG_HELPER_PET_TURNTO_NPC:
		do_CommandPetTurnToNpc( msg, dest );
		break;
#endif //PET_TURNTO_NPC

#ifdef ALTERNATE_MERCHANT
	case MSG_HELPER_ALTERNATEMERCHANT_START :
		do_CommandMerchantStart( msg, dest );
		break;
	case MSG_HELPER_ALTERNATEMERCHANT_BUY :
		do_CommandMerchantBuy( msg, dest );
		break;
	case MSG_HELPER_ALTERNATEMERCHANT_PRODUCT_RECOVERY :
		do_CommandMerchantProductRecover( msg, dest );
		break;
	case MSG_HELPER_ALTERNATEMERCHANT_NAS_RECOVERY :
		do_CommandMerchantNasRecovery( msg, dest );
		break;
	case MSG_HELPER_ALTERNATEMERCHANT_END :
		do_CommandMerchantEnd( msg, dest );
		break;
#endif // ALTERNATE_MERCHANT

#ifdef MONSTER_COMBO
	case MSG_HELPER_CREATE_COMBO:
		do_CommandCreateCombo( msg, dest );
		break;
#endif 
		
#ifdef ATTACK_PET
	case MSG_HELPER_APET:
		do_CommandAttackPet( msg, dest );
#endif //ATTACK_PET

#ifdef EXTREME_CUBE
	case MSG_HELPER_CUBESTATE_REQ:
		do_CommandCubeStateReq( msg, dest );
		break;
	case MSG_HELPER_CUBESTATE_PERSONAL_REQ:
		do_CommandCubeStatePersonalReq( msg, dest );
		break;
	case MSG_HELPER_ADDCUBEPOINT:
		do_CommandAddCubePoint( msg, dest );
		break;
	case MSG_HELPER_ADDCUBEPOINT_PERSONAL:
		do_CommandAddCubePointPersonal( msg, dest );
		break;
#endif // EXTREME_CUBE

#ifdef EVENT_PHOENIX	// //피닉스 이벤트 yhj	
	case MSG_HELPER_EVENT_PHOENIX:
		do_CommandEventPhoenix( msg, dest );
		break;
#endif // EVENT_PHOENIX		

#ifdef TRADE_AGENT
	case MSG_HELPER_TRADEAGENT_REG_LIST_REQ:
		do_Command_TradeAgent_RegListReq(msg, dest );
		break;			
	case MSG_HELPER_TRADEAGENT_REG_REQ:
		do_Command_TradeAgent_RegReq(msg, dest );
		break;	
	case MSG_HELPER_TRADEAGENT_REGRESULT:
		do_Command_TradeAgent_RegResult(msg, dest );
		break;	
	case MSG_HELPER_TRADEAGENT_REG_CANCEL_REQ:
		do_Command_TradeAgent_RegCancelReq(msg, dest );
		break;	
		
	case MSG_HELPER_TRADEAGENT_SEARCH_REQ:
		do_Command_TradeAgent_SearchReq( msg, dest );
		break;	

	case MSG_HELPER_TRADEAGENT_BUY_REQ:
		do_Command_TradeAgent_BuyReq( msg, dest );
		break;	

	case MSG_HELPER_TRADEAGENT_CALCLIST_REQ:
		do_Command_TradeAgent_CalcListReq( msg, dest );
		break;	
	case MSG_HELPER_TRADEAGENT_CALCULATE_REQ:
		do_Command_TradeAgent_CalculateReq( msg, dest );
		break;	
	case MSG_HELPER_TRADEAGENT_CALCRESULT:
		do_Command_TradeAgent_CalcResult( msg, dest );
		break;		
	case MSG_HELPER_TRADEAGENT_CHECKCALC_REQ:
		do_Command_TradeAgent_CheckCalcReq( msg, dest );
		break;				

#endif //TRADE_AGENT

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
	case MSG_HELPER_DISCONNECT_HACK_CHARACTER:
		do_Command_DisconnectHackCharacter( msg, dest );
		break;
#endif // DISCONNECT_HACK_CHARACTER 

#ifdef EXPEDITION_RAID
	case MSG_HELPER_EXPED_CREATE_REQ:
		do_Command_Exped_CreateReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_INVITE_REQ:
		do_Command_Exped_InviteReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_ALLOW_REQ:
		do_Command_Exped_AllowReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_REJECT_REQ:
		do_Command_Exped_RejectReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_QUIT_REQ:
		do_Command_Exped_QuitReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_KICK_REQ:
		do_Command_Exped_KickReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_CHANGETYPE_REQ:
		do_Command_Exped_ChangeTypeReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_CHANGEBOSS_REQ:
		do_Command_Exped_ChangeBossReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_SETMBOSS_REQ:
		do_Command_Exped_SetMBossReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_RESETMBOSS_REQ:
		do_Command_Exped_ResetMBossReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_ENDEXPED_REQ:
		do_Command_Exped_EndExpedReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_MOVEGROUP_REQ:
		do_Command_Exped_MoveGroupReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_SET_LABEL_REQ:
		do_Command_Exped_SetLabelReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_REJOIN_REQ:
		do_Command_Exped_RejoinReq( msg, dest );
		break;

#endif //EXPEDITION_RAID
		
	default:
		break;
	}
}

void do_CommandMargadumPvpRank(CNetMsg& msg, CDescriptor* dest)
{
	// 케릭터 데이터베이스에 쓰기

	int count, i;
	int rank;
	int subno;
	LONGLONG damage;

	msg >> subno
		>> count;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	
	cmd.BeginTrans();
	for(i = 0; i < count; i++)
	{
		msg >> rank
			>> damage;

#ifdef CIRCLE_WINDOW
		sprintf(g_buf, "insert into t_margadum_pvp ( a_date, subno, a_rank, a_char_idx, a_damage ) values ( now(), %d, %d, %d, %I64d ) ", subno, i+1, rank, damage);
		cmd.SetQuery(g_buf);
#else
		sprintf(g_buf, "insert into t_margadum_pvp ( a_date, subno, a_rank, a_char_idx, a_damage ) values ( now(), %d, %d, %d, %lld ) ", subno, i+1, rank, damage);
		cmd.SetQuery(g_buf);
#endif

		if( !cmd.Update() )
		{
			cmd.Rollback();
			GAMELOG << init("MARGADUM_PVP_RANK_UPDATE_ERROR")
					<< mysql_error(&gserver.m_dbchar) << end;
		}

	}
	cmd.Commit();
}

void do_CommandTeacherLoad(CNetMsg& msg, CDescriptor* dest)
{
	CLCString idname(MAX_ID_NAME_LENGTH + 1);
	int charindex;
	
	int		m_guardian;
	int		m_superstone;
	int		m_cntFailStudent;
	int		m_cntCompleteStudent;
	char		m_teachType;
	int		m_fame;
	char	bteacher;
	CLCString strTeachIdx(256);
	CLCString strTeachSec(256);
	int subtype;

	msg.MoveFirst();
	msg >> subtype;

	msg >> idname
		>> charindex;

	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);

	sprintf(g_buf, "select a_teach_idx, a_teach_sec, a_teach_type, a_fame, a_teach_list, a_teach_complete, a_teach_fail, a_superstone, a_guardian "
		" from t_characters where a_index=%d ", charindex);
	dbChar.SetQuery(g_buf);

	if( !dbChar.Open() || !dbChar.MoveFirst() )
	{
		CNetMsg rmsg;
		HelperGuildLoadRepMsg(rmsg, idname, charindex, MSG_GUILD_ERROR_OK);
		SEND_Q(rmsg, dest);

		return;
	}

	dbChar.GetRec("a_teach_type",	m_teachType);
	dbChar.GetRec("a_fame",			m_fame);
	dbChar.GetRec("a_teach_list",	bteacher);
	dbChar.GetRec("a_teach_complete", m_cntCompleteStudent);
	dbChar.GetRec("a_teach_fail",	  m_cntFailStudent);
	dbChar.GetRec("a_superstone",	  m_superstone);
	dbChar.GetRec("a_guardian",		  m_guardian);
	dbChar.GetRec("a_teach_idx", strTeachIdx);
	dbChar.GetRec("a_teach_sec", strTeachSec);

	CNetMsg rmsg;
	
	if( m_teachType == MSG_TEACH_TEACHER_TYPE ||  m_teachType == MSG_TEACH_STUDENT_TYPE)
	{
		HelperTeacherInfo(rmsg, idname, charindex, m_teachType, m_fame, bteacher, m_cntCompleteStudent, 
			m_cntFailStudent, m_superstone, m_guardian, strTeachIdx, strTeachSec);
	}
	else
	{
		rmsg.Init(MSG_HELPER_COMMAND);
		rmsg << MSG_HELPER_TEACHER_LOAD_REP
			 << idname
			 << charindex
			 << 0
			 << m_guardian
			 << m_superstone
			 << m_cntFailStudent
			 << m_cntCompleteStudent
			 << m_teachType
			 << m_fame
			 << bteacher;
	}
	SEND_Q(rmsg, dest);

	HelperGuildLoadRepMsg(rmsg, idname, charindex, MSG_GUILD_ERROR_OK);
	SEND_Q(rmsg, dest);
}

void do_CommandTeacherSuperstoneRecieve(CNetMsg& msg, CDescriptor* dest)
{
	int charindex;

	msg >> charindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "update t_characters set a_superstone = a_superstone+1 where a_index = %d ", charindex);
	cmd.SetQuery(g_buf);

	if( !cmd.Update() )
	{
		GAMELOG << init("TEACH_SUPERSTONE_ERROR_NOTACCESSDB")
				<< charindex
				<< end;
		return;
	}

}

void do_CommandTeacherTimeover(CNetMsg& msg, CDescriptor* dest)
{
	int flag=0;
	int teachidx=0;
	int studentidx=0;

	msg >> flag
		>> teachidx
		>> studentidx;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.BeginTrans();
	if( !do_TeachCancel(cmd, teachidx, studentidx, flag) )
	{
		cmd.Rollback();
		GAMELOG << init("TEACH_TIMEOVER_ERROR_NOTACCESSDB")
				<< flag << delim
				<< teachidx << delim
				<< studentidx << delim
				<< end;
		return;
	}
	cmd.Commit();

}

void do_CommandTeacher(CNetMsg& msg, CDescriptor* dest)
{
	int teacheridx;
	char bteacher;

	msg >> teacheridx
		>> bteacher;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "update t_characters set a_teach_list=%d where a_index =%d ", bteacher, teacheridx);
	cmd.SetQuery(g_buf);

	if( !cmd.Update() )
	{
		GAMELOG << init("TEACH_LIST_ERROR_NOTACCESSDB")
				<< teacheridx << delim
				<< bteacher << delim
				<< end;
		return;
	}
}

void do_CommandTeacherGiveup(CNetMsg& msg, CDescriptor* dest)
{
	int teachidx = 0;
	int studentidx = 0;

	msg >> teachidx
		>> studentidx;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	cmd.BeginTrans();
	if( !do_TeachCancel(cmd, teachidx, studentidx) )
	{
		cmd.Rollback();
		return;
	}

	// 여기서는 학생의 teach_type을 MSG_TEACG_NO_STUDENT_TYPE로 수정하고 do_TeachCancel를 호출한다.
	sprintf(g_buf, "update t_characters set a_teach_type = %d where a_index = %d", MSG_TEACG_NO_STUDENT_TYPE, studentidx);
	cmd.SetQuery(g_buf);
	if( !cmd.Update() )
	{
		cmd.Rollback();
		return;
	}

	sprintf(g_buf, "update t_characters set a_teach_fail = a_teach_fail + 1 where a_index = %d ", teachidx);
	cmd.SetQuery(g_buf);
	if( !cmd.Update() )
	{
		cmd.Rollback();
	}
	cmd.Commit();
	/*
	// 선생의 학생 목록

	cmd.BeginTrans();
	// 학생 업데이트
	sprintf(g_buf, "update t_characters set a_teach_idx = ' -1 -1 -1 -1 -1 -1 -1 -1', a_teach_sec = ' 0 0 0 0 0 0 0 0', a_teach_type = %d "
		"where a_index = %d", MSG_TEACG_NO_STUDENT_TYPE, studentidx);
	cmd.SetQuery(g_buf);
	if(!cmd.Update())
	{
		cmd.Rollback();
		return;
	}

	//선생 업데이트
	// 
	//select concat(substring_index(a_teach_idx, '1069', 1), substring(a_teach_idx, locate(a_teach_idx, '1069', 1) + length('1069') ) )
	//from t_characters where a_index = 96
	sprintf(g_buf, "update t_characters set "
		"a_teach_idx = concat(substring_index(a_teach_idx, '%d', 1), substring(a_teach_idx, locate(a_teach_idx, '%d', 1) + length('%d') ) ) "
		"from t_characters where a_index = %d ",
		studentidx, studentidx, studentidx, teachidx);
		

	cmd.SetQuery(g_buf);
	if(!cmd.Update())
	{
		cmd.Rollback();
		return;
	}*/


}

void do_CommandTeacherRegister(CNetMsg& msg, CDescriptor* dest)
{
	int teachidx = 0;
	int studentidx = 0;
	char bTeacher = 0;
	CLCString strteach(256);
	int m_teachIdx[TEACH_MAX_STUDENTS];
	int m_teachTime[TEACH_MAX_STUDENTS];
	int i;
	time_t tCurTime;
	time(&tCurTime);

	msg >> teachidx
		>> bTeacher
		>> studentidx;

	// 학생 데이터베이스
	sprintf(g_buf, "update t_characters set a_teach_idx = CONCAT(' %d', ' -1', ' -1',' -1',' -1',' -1',' -1',' -1'), "
		"a_teach_sec = CONCAT(' %d',' 0',' 0',' 0',' 0',' 0',' 0',' 0'), a_teach_type = %d "
		"where a_index = %d ", teachidx, (int)tCurTime, MSG_TEACH_STUDENT_TYPE, studentidx);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	cmd.BeginTrans();

	cmd.SetQuery(g_buf);
	if( !cmd.Update() )
	{
		//등록 실패
		cmd.Rollback();
		return;
	}

	
	// 선생 데이터베이스
	sprintf(g_buf, "select a_teach_idx, a_teach_sec from t_characters where a_index=%d ", teachidx);
	cmd.SetQuery(g_buf);
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		cmd.Rollback();
		return;
	}

	memset(m_teachIdx, -1, sizeof(m_teachIdx) );
	memset(m_teachTime, 0, sizeof(m_teachTime) );

	cmd.GetRec("a_teach_idx", strteach);

	int cnt =0;
	int count = 0;
	bool isFirst = true;

	const char* pTeach = strteach;
	while (*pTeach)
	{
		int idx;
		pTeach = AnyOneArg(pTeach, g_buf);
		idx = atoi(g_buf);

		if (idx == -1 && isFirst)
		{
			m_teachIdx[cnt] = studentidx;
			count = cnt;
			isFirst = false;
		}
		else
		{
			m_teachIdx[cnt] = idx;
		}
		cnt++;
	}

	cmd.GetRec("a_teach_sec", strteach);

	pTeach = strteach;
	cnt = 0;
	while (*pTeach)
	{
		int sec;
		pTeach = AnyOneArg(pTeach, g_buf);
		sec = atoi(g_buf);
		m_teachTime[cnt++] = sec;
	}
	m_teachTime[count] = time(0);
	
	sprintf(g_buf, "update t_characters set a_teach_idx = '");
	for(i = 0; i < TEACH_MAX_STUDENTS; i++)
	{
		IntCat(g_buf, m_teachIdx[i], true);
	}
	StrCat(g_buf, "', a_teach_sec = '");
	for(i = 0; i < TEACH_MAX_STUDENTS; i++)
	{
		IntCat(g_buf, m_teachTime[i], true);
	}
	StrCat(g_buf, "', a_teach_type = ");
	IntCat(g_buf, MSG_TEACH_TEACHER_TYPE);
	StrCat(g_buf, ", a_teach_list = ");
	IntCat(g_buf, bTeacher);
	StrCat(g_buf, " where a_index=");
	IntCat(g_buf, teachidx);

	cmd.SetQuery(g_buf);
	if( !cmd.Update() )
	{
		//등록 실패
		cmd.Rollback();
		return;
	}
	cmd.Commit();
	
}

bool do_TeachCancel(CDBCmd cmd, int teacheridx, int studentidx, int cancelflag)
{
	int studentList[TEACH_MAX_STUDENTS];
	int studentTime[TEACH_MAX_STUDENTS];
	CLCString str_studentList(256);
	CLCString str_studentTime(256);
	char temp[1024];
	bool bTeacher = false;
	int i;

	memset(studentList, -1, sizeof(studentList) );
	memset(studentTime, 0, sizeof(studentTime) );
	memset(temp, '\0', sizeof(temp) );

	if(cancelflag & CANCELTEACHER)
	{
		// 선생부분업데이트-시작
		sprintf(g_buf, "select a_teach_idx, a_teach_sec from t_characters where a_index = %d", teacheridx);
		cmd.SetQuery(g_buf);
		if( !cmd.Open() || !cmd.MoveFirst() )
		{
			GAMELOG << init("TEACH_SUCCESS_FAIL-do_TeachCancel")
					<< teacheridx << delim
					<< studentidx
					<< end;
			return false;
		}

		cmd.GetRec("a_teach_idx", str_studentList);
		cmd.GetRec("a_teach_sec", str_studentTime);
		const char* p_list = str_studentList;
		const char* p_time = str_studentTime;

		for(i = 0; i < TEACH_MAX_STUDENTS; i++)
		{
			p_list = AnyOneArg(p_list, temp);
			studentList[i] = atoi(temp);
			p_time = AnyOneArg(p_time, temp);
			studentTime[i] = atoi(temp);

			if(studentList[i] == studentidx)
			{
				studentList[i] = -1;
				studentTime[i] = 0;
			}

			if( studentList[i] != -1)
				bTeacher = true;
		}

		*g_buf2 = '\0';
		*g_buf3 = '\0';

		for(i = 0; i < TEACH_MAX_STUDENTS; i++)
		{
			IntCat(g_buf2, studentList[i]);
			IntCat(g_buf3, studentTime[i]);
		}

		sprintf(g_buf, "update t_characters set "
			"a_teach_idx = '%s', a_teach_sec = '%s', a_teach_list = %d where a_index = %d"
			,g_buf2, g_buf3, bTeacher, teacheridx);

		cmd.SetQuery(g_buf);
		if( !cmd.Update() )
		{
			GAMELOG << init("TEACH_SUCCESS_FAIL-do_TeachCancel")
					<< teacheridx << delim
					<< studentidx
					<< end;
			return false;
		}
	}

	if(cancelflag & CANCELSTUDENT)
	{
		// 학생일때는 제자가 없으므로 그냥 업데이트
		sprintf(g_buf, "update t_characters set a_teach_idx=' -1 -1 -1 -1 -1 -1 -1 -1', a_teach_sec=' 0 0 0 0 0 0 0 0', a_teach_type=%d "
			"where a_index=%d ", MSG_TEACH_NO_TYPE, studentidx);
		cmd.SetQuery(g_buf);

		if(!cmd.Update())
		{
			GAMELOG << init("TEACH_SUCCESS_FAIL-do_TeachCancel")
					<< teacheridx << delim
					<< studentidx
					<< end;
			return false;
		}
	}

	return true;
}

void do_CommandFameUp(CNetMsg& msg, CDescriptor* dest)
{
	int teacheridx = 0;
	int studentidx = 0;
	CLCString studentname(MAX_CHAR_NAME_LENGTH + 1);
	int fame = 0;

	msg >> teacheridx
		>> studentidx
		>> studentname
		>> fame;

	CDBCmd cmd;
	
	cmd.Init(&gserver.m_dbchar);

	cmd.BeginTrans();
	// 여기서는 선생의 명성, complete 추가하고 do_TeachCancel을 호출한다
	sprintf(g_buf, "update t_characters set a_fame = a_fame + %d, a_teach_complete = a_teach_complete + 1 where a_index = %d ", fame, teacheridx);
	cmd.SetQuery(g_buf);

	if(!cmd.Update() )
	{
		cmd.Rollback();
		return;
	}

	if( !do_TeachCancel(cmd, teacheridx, studentidx) )
	{
		cmd.Rollback();
		return;
	}
	cmd.Commit();

	// 전 서브서버에 거시기가 명성 업 했다는 것을 보냄
	CNetMsg rmsg;
	HelperFameupRepMsg(rmsg, teacheridx, studentidx, studentname, fame);
	gserver.SendToAllGameServer(rmsg);
	
	GAMELOG << init("TEACH_SUCCESS")
				<< teacheridx << delim
				<< studentidx
				<< fame
				<< end;
	
	// 명성과 함께 아이리스 포인트 적립
#ifdef IRIS_POINT
	sprintf(g_buf, "select a_user_index from t_characters where a_index = %d ", teacheridx);
	cmd.SetQuery(g_buf);
	if( cmd.Open() && cmd.MoveFirst() )
	{
		int teacher_userIdx;
		cmd.GetRec("a_user_index", teacher_userIdx );		
		HelperToConnAddIpointMsg(rmsg, teacher_userIdx, MSG_IPOINT_TYPE_TEACHER, 1 );	// Limit 없음 
		SEND_Q( rmsg, dest);
	}
#endif // IRIS_POINT
	
#ifdef TEACH_LOG
	sprintf(g_buf, "INSERT INTO t_teach_log ( a_char_index, a_teacher_index, a_update_date ) VALUES ( %d, %d, NOW() )", studentidx, teacheridx );
	cmd.SetQuery(g_buf);
	cmd.Update();
#endif // TEACH_LOG

#ifdef EVENT_TEACH_LIGHT_USER
	// 이벤트 기간동안의 랭킹
	sprintf(g_buf, "select * from t_event_teach_lightuser where a_index=%d ", teacheridx);
	cmd.SetQuery(g_buf);

	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		sprintf(g_buf, "insert into t_event_teach_lightuser values(%d, %d, now(), now())", teacheridx, 1);
		cmd.SetQuery(g_buf);

		if(!cmd.Update() )
		{
			return;
		}
	}
	else
	{
		sprintf(g_buf, "update t_event_teach_lightuser set a_result=a_result+1, a_update=now() where a_index=%d", teacheridx);
		cmd.SetQuery(g_buf);
		if(!cmd.Update() )
		{
			return;
		}
	}
	
	// 동점일 경우를 위한 기록
	sprintf(g_buf, "insert into t_event_teach_lightuser_result(%d, %d, now())", teacheridx, studentidx);
	cmd.SetQuery(g_buf);
	cmd.Update();

	GAMELOG << init("EVENT TEACH LIGHT USER ADD")
		<< teacheridx << delim << studentidx << end;
#endif // EVENT_TEACH_LIGHT_USER
}

void do_CommandNameChange(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	char bguild = 0;
	int index = 0;
	CLCString name(256);

	msg >> bguild
		>> index
		>> name;

	CDBCmd cmd;

	cmd.Init(&gserver.m_dbchar);

	if(bguild == 1)
	{
		sprintf(g_buf, "update t_guild set a_name = '%s' where a_index = %d", (const char*)name, index);
		cmd.SetQuery(g_buf);
		if(!cmd.Update())
		{
			HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_DUPLICATE);
			SEND_Q(rmsg, dest);
			return;
		}

		CGuild *guild = gserver.m_guildlist.findguild(index);
		guild->changeName(name);
		// 변경되었다는 메세지 보냄
	}
	// 3, 4는 롤백
	else if(bguild == 3)
	{
		sprintf(g_buf, "update t_guild set a_name = '%s' where a_index = %d", (const char*)name, index);
		cmd.SetQuery(g_buf);
		cmd.Update();
		return;
	}
	else if(bguild == 4)
	{
		cmd.BeginTrans();

		sprintf(g_buf, "update t_characters set a_nick = '%s' where a_index = %d", (const char*)name, index);
		cmd.SetQuery(g_buf);
		cmd.Update();

		sprintf(g_buf, "update t_guildmember set a_char_name = '%s' where a_char_index = %d", (const char*)name, index);
		cmd.SetQuery(g_buf);
		if(!cmd.Update())
		{
			cmd.Rollback();
			return;
		}

		for(int j = 0; j < 10; j++)
		{
			CDBCmd cmd1;
			cmd1.Init(&gserver.m_dbchar);

			sprintf(g_buf, "select * from t_friend0%d where a_friend_index = %d", j, index);
			cmd1.SetQuery(g_buf);
			cmd1.Open();

			if( cmd1.m_nrecords < 1 )
				continue;

			sprintf(g_buf, "update t_friend0%d set a_friend_name = '%s' where a_friend_index = %d", j, (const char*)name, index);
			cmd.SetQuery(g_buf);
			if( !cmd.Update() )
			{
				cmd.Rollback();
				return;
			}
		}

		cmd.Commit();

		return;
	}
	else if(bguild == 5)
	{
		// 모든 서버에게 길드 이름이 바뀌었으므로 바꾸고 전 길드원에게 통보
		HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_SUCCESS);
		gserver.SendToAllGameServer(rmsg);
		return;
		//SEND_Q(rmsg, dest);
	}
	else
	{
		sprintf(g_buf, "select a_name from t_characters where a_name = '%s' and a_index != %d ", (const char*)name, index);
		cmd.SetQuery(g_buf);
		if( !cmd.Open() || (cmd.m_nrecords > 0))
		{
			HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_DUPLICATE);
			SEND_Q(rmsg, dest);
			return;
		}

		cmd.BeginTrans();

		sprintf(g_buf, "update t_characters set a_nick = '%s' where a_index = %d", (const char*)name, index);
		cmd.SetQuery(g_buf);
		if(!cmd.Update())
		{
			cmd.Rollback();
			HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_DUPLICATE);
			SEND_Q(rmsg, dest);
			return;
		}

		sprintf(g_buf, "update t_guildmember set a_char_name = '%s' where a_char_index = %d", (const char*)name, index);
		cmd.SetQuery(g_buf);
		if(!cmd.Update())
		{
			cmd.Rollback();
			HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_DUPLICATE);
			SEND_Q(rmsg, dest);
			return;
		}

		for(int j = 0; j < 10; j++)
		{
			CDBCmd cmd1;
			cmd1.Init(&gserver.m_dbchar);

			sprintf(g_buf, "select * from t_friend0%d where a_friend_index = %d", j, index);
			cmd1.SetQuery(g_buf);
			cmd1.Open();

			if( cmd1.m_nrecords < 1 )
				continue;

			sprintf(g_buf, "update t_friend0%d set a_friend_name = '%s' where a_friend_index = %d", j, (const char*)name, index);
			cmd.SetQuery(g_buf);
			if( !cmd.Update() )
			{
				cmd.Rollback();
				HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_HELPER);
				SEND_Q(rmsg, dest);
				return;
			}
		}
		cmd.Commit();

		CGuildMember* gMember = gserver.m_guildlist.findmember(index);

		if( gMember )
			gMember->name(name);
			//member nick부분 삭제 : bw 060403
			//gMember->nick(name);

#ifdef PARTY_MATCHING
		// 파티 매칭 업데이트
		CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(dest->m_subNo, index);
		if (pMatchMember)		pMatchMember->SetName(name);
		CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, index);
		if (pMatchParty)		pMatchParty->SetBossName(name);
		HelperPartyMatchMemberChangeInfoMsg(rmsg, index, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME, name, 0, 0);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
#endif // PARTY_MATCHING

		// 변경되었다는 메세지 보냄
	}

	HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_SUCCESS);
	SEND_Q(rmsg, dest);
}


///////////////////////////////////////////
// Rank DB control in personal dungeon 4(차원의 문) : bw
//
void do_CommandPD4RewardEnd(CNetMsg& msg, CDescriptor* dest)
{
	CDBCmd cmd;

	// 보상 DB의 일주일 전 레코드를 삭제한다.
	cmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "delete from t_pd4reward where date_add(a_adddate, interval 6 day) < curdate();");
	cmd.SetQuery(g_buf);

	if( !cmd.Update() )
	{
		GAMELOG << init("QUEST REWARD DB END FAIL");
		return;
	}

}
void do_CommandPD4RewardInit(CNetMsg& msg, CDescriptor* dest)
{
	CDBCmd cmd;

	cmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "update t_pd4reward set a_breward = 0 where a_breward = 2 and date_add(a_adddate, interval 6 day) > curdate();");
	cmd.SetQuery(g_buf);

	if( !cmd.Update() )
	{
		GAMELOG << init("QUEST REWARD DB INIT FAIL") << end ;
		return;
	}

}

void do_CommandPD4Reward(CNetMsg& msg, CDescriptor* dest)
{
	//MSG_HELPER_PD4_REWARD_REQ : charindex(n)
	//MSG_HELPER_PD4_REWARD_REP : charindex(n) charrank(c) breward(c)
	char charrank = 0;
	char breward = 1;
	int charindex;

	msg >> charindex;

	CDBCmd rewardDBcmd;
	rewardDBcmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "select a_rank, a_breward from t_pd4reward where a_index =%d and date_add(a_adddate, interval 6 day) > curdate();", charindex);

	rewardDBcmd.SetQuery(g_buf);

	if( !rewardDBcmd.Open() )
	{
		msg.Init(MSG_HELPER_COMMAND);
		msg << MSG_HELPER_PD4_REWARD_REP;
		SEND_Q(msg, dest);
		return;
	}
	if( !rewardDBcmd.MoveFirst() )
	{
		msg.Init(MSG_HELPER_COMMAND);
		msg << MSG_HELPER_PD4_REWARD_REP
			<< charindex
			<< (char) 0
			<< (char) 1;
		SEND_Q(msg, dest);
		return;
	}

	if( !rewardDBcmd.GetRec("a_rank", charrank)
		|| !rewardDBcmd.GetRec("a_breward", breward))
	{
		msg.Init(MSG_HELPER_COMMAND);
		msg << MSG_HELPER_PD4_REWARD_REP
			<< charindex
			<< charrank
			<< (char) 1;
		SEND_Q(msg, dest);
		return;
	}
	
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PD4_REWARD_REP
		<< charindex
		<< charrank
		<< breward;

	rewardDBcmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "update t_pd4reward set a_breward = 1 where a_index= %d and date_add(a_adddate, interval 6 day) > curdate();", charindex);

	rewardDBcmd.SetQuery(g_buf);
	if(!rewardDBcmd.Update())
	{
		msg.Init(MSG_HELPER_COMMAND);
		msg << MSG_HELPER_PD4_REWARD_REP;
		return;
	}

	SEND_Q(msg, dest);
}
void do_CommandPD4RankView(CNetMsg& msg, CDescriptor* dest)
{
	//MSG_HELPER_PD4_RANK_VIEW_REQ : charindex(n) charjob(c)
	//MSG_HELPER_PD4_RANK_VIEW_REP : charindex(n) multicount(c), charname(str), bclear(c), deadmon(n), ctime(ll);
	int charindex;
	char charjob;
	char multicount;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	char bclear;
	int deadmon;
	LONGLONG ctime;

	msg >> charindex
		>> charjob;

	CDBCmd cmd;

	cmd.Init(&gserver.m_dbchar);
	
	sprintf(g_buf, "select a_name, a_bclear, a_deadmon, a_ctime from t_pd4rank where a_etime != '' and a_job = %d and a_etime != '' order by a_bclear desc, a_deadmon desc, a_ctime asc, a_level asc, a_exp asc, a_index asc LIMIT 5", charjob);

	cmd.SetQuery(g_buf);
	if( !cmd.Open() )
	{
		msg.Init(MSG_HELPER_COMMAND);
		msg << MSG_HELPER_PD4_RANK_VIEW_REP
			<< charindex
			<< (char) 0;

		SEND_Q(msg, dest);
		return;
	}
	if( !cmd.MoveFirst() )
	{
		msg.Init(MSG_HELPER_COMMAND);
		msg << MSG_HELPER_PD4_RANK_VIEW_REP
			<< charindex
			<< (char) 0;
		SEND_Q(msg, dest);
		return;
	}

	multicount = 1;
	while(cmd.MoveNext()) multicount++;

	if( !cmd.MoveFirst() )
	{
		msg.Init(MSG_HELPER_COMMAND);
		msg << MSG_HELPER_PD4_RANK_VIEW_REP
			<< charindex
			<< (char) 0;
		SEND_Q(msg, dest);
		return;
	}

	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PD4_RANK_VIEW_REP
		<< charindex
		<< multicount;

	for(int i = 0; i < multicount; i++)
	{
		if( !cmd.GetRec("a_name", charname)
			|| !cmd.GetRec("a_bclear", bclear)
			|| !cmd.GetRec("a_deadmon", deadmon)
			|| !cmd.GetRec("a_ctime", ctime) )
		{
			msg.Init(MSG_HELPER_COMMAND);
			msg << MSG_HELPER_PD4_RANK_VIEW_REP
				<< charindex
				<< (char) 0;
			SEND_Q(msg, dest);
			return;
		}

		msg << charname
			<< bclear
			<< deadmon
			<< ctime;

		cmd.MoveNext();
	}

	SEND_Q(msg, dest);
}
void do_CommandPD4RewardView(CNetMsg& msg, CDescriptor* dest)
{
	
	//MSG_HELPER_PD4_REWARD_VIEW_REQ : charindex(n) charjob(c)
	//MSG_HELPER_PD4_REWARD_VIEW_REP : charindex(n) multicount(c) charname(str) breward(c)
	char multicount;
	int charindex;
	char charjob;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	char breward;

	msg >> charindex
		>> charjob;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "select a_name, a_breward from t_pd4reward where a_job = %d and date_add(a_adddate, interval 6 day) > curdate() order by a_rank;", charjob);
	cmd.SetQuery(g_buf);

	if( !cmd.Open() )
	{
		msg.Init(MSG_HELPER_COMMAND);
		msg << MSG_HELPER_PD4_REWARD_VIEW_REP;
		SEND_Q(msg, dest);
		return;
	}
	if( !cmd.MoveFirst() )
	{
		msg.Init(MSG_HELPER_COMMAND);
		msg << MSG_HELPER_PD4_REWARD_VIEW_REP
			<< charindex
			<< (char) 0;
		SEND_Q(msg, dest);
		return;
	}

	multicount = 1;
	while(cmd.MoveNext()) multicount++;

	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PD4_REWARD_VIEW_REP
		<< charindex
		<< multicount;

	cmd.MoveFirst();

	for(int i = 0; i < multicount; i++)
	{
		if( !cmd.GetRec("a_name", charname) 
			|| !cmd.GetRec("a_breward", breward) )
		{
			msg.Init(MSG_HELPER_COMMAND);
			msg << MSG_HELPER_PD4_REWARD_VIEW_REP
				<< charindex
				<< (char) 0;
			SEND_Q(msg, dest);
			return;
		}

		msg << charname;

		if(!breward)
			msg << breward;
		else
			msg << (char) 1;

		cmd.MoveNext();
	}

	SEND_Q(msg, dest);
}

void do_CommandPD4RankInit(CNetMsg& msg, CDescriptor* dest)
{
	//MSG_HELPER_PD4_RANK_INIT,			// pd4 rank DB init 

	////////////////////////////////////////
	// 보상 DB로 백업 부분 추가
	//
	int charindex;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	char charjob = 0;

	CDBCmd rankCmd, rewardCmd;
	
	while(charjob < JOBCOUNT) 
	{
		rankCmd.Init(&gserver.m_dbchar);

		//select a_name, a_bclear, a_deadmon, a_ctime from t_pd4rank where a_job = %d order by a_bclear desc, a_deadmon desc, a_ctime desc, a_level desc, a_exp desc, a_index desc LIMIT 5
		sprintf(g_buf, "select a_index, a_name from t_pd4rank where a_etime != '' and a_job = %d order by a_bclear desc, a_deadmon desc, a_ctime asc, a_level asc, a_exp asc, a_index asc LIMIT 5", charjob);

		rankCmd.SetQuery(g_buf);
		if( !rankCmd.Open() )
		{
			charjob++;
			continue;
		}

		if( !rankCmd.MoveFirst() )
		{

			charjob++;
			continue;
		}

		char rank = 0;

		do
		{
			rank++;

			if(!rankCmd.GetRec("a_index", charindex) 
				|| !rankCmd.GetRec("a_name", charname))
			{
				continue;
			}
			rewardCmd.Init(&gserver.m_dbchar);

			sprintf(g_buf, "insert into t_pd4reward (a_index, a_rank, a_name, a_job, a_breward, a_adddate ) values ( %d, %d, '%s', %d, 2, now())", charindex, rank, (const char*)charname, charjob);
			
			rewardCmd.SetQuery(g_buf);
			rewardCmd.Update();
			GAMELOG << init("PD4_RANK")
					<< rank << delim
					<< charindex << delim
					<< charname << delim
					<< charjob << end;

		} while(rankCmd.MoveNext());

		charjob++;
	}

	rankCmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "drop table t_pd4rank; ");
	rankCmd.SetQuery(g_buf);
	if(!rankCmd.Update())
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB INIT FAILED") << end;
		
		return;
	}

#ifdef LC_CHN
	sprintf(g_buf, "CREATE TABLE t_pd4rank ( a_index int(10) unsigned NOT NULL default '0',  a_name varchar(20) default '0',  a_exp bigint(20) unsigned default '0',  a_level tinyint(3) unsigned default '0', a_job int(10) unsigned default '0', a_stime datetime default '0000-00-00 00:00:00', a_etime datetime default '0000-00-00 00:00:00', a_ctime bigint(20) unsigned default '0', a_bclear tinyint(3) unsigned default '0', a_deadmon int(10) unsigned default '0', PRIMARY KEY  (a_index), UNIQUE KEY a_index (a_index), KEY a_index_2 (a_index))");
#else
	sprintf(g_buf, "CREATE TABLE t_pd4rank ( a_index int(10) unsigned NOT NULL default '0',  a_name varchar(20) default '0',  a_exp bigint(20) unsigned default '0',  a_level tinyint(3) unsigned default '0', a_job int(10) unsigned default '0', a_stime datetime default '0000-00-00 00:00:00', a_etime datetime default '0000-00-00 00:00:00', a_ctime bigint(20) unsigned default '0', a_bclear tinyint(3) unsigned default '0', a_deadmon int(10) unsigned default '0', PRIMARY KEY  (a_index), UNIQUE KEY a_index (a_index), KEY a_index_2 (a_index)) TYPE=innoDB");
#endif
	rankCmd.SetQuery(g_buf);
	if(!rankCmd.Update())
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB INIT FAILED") << end;
		
		return;
	}

}

void do_CommandPD4RankAdd(CNetMsg& msg, CDescriptor* dest)
{

	/////////////////////////////////////////////
	// BANGWALL : 2005-06-23 오전 11:41:59
	// Comment : Quest Start time


	//MSG_HELPER_PD4_RANK_ADD,			// pd4 rank 시작	 : charIndex(n) 
	// index, name, exp, stime, job, level
	int charindex;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString charnick(MAX_CHAR_NAME_LENGTH + 1);
	LONGLONG charexp;
	char charjob;
	int charlevel;

	msg >> charindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "select a_name, a_nick, a_exp, a_job, a_level from t_characters where a_index = %d and a_admin = 0", charindex);

	cmd.SetQuery(g_buf);
	if( !cmd.Open() )
	{
		GAMELOG << init("PD4 RANK ADD ERROR")
				<< charindex;
		return;
	}
	if( !cmd.MoveFirst() )
	{
		GAMELOG << init("PD4 RANK ADD ERROR")
				<< charindex;
		return;
	}
	

	if( !cmd.GetRec("a_name", charname)
		||!cmd.GetRec("a_nick", charnick)
		||!cmd.GetRec("a_exp", charexp)
		||!cmd.GetRec("a_job", charjob)
		||!cmd.GetRec("a_level", charlevel) ) 
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB ADD FAILED")
				<< charname << end;
		return;
	}
	
	cmd.Init(&gserver.m_dbchar);

	if( strcmp(charnick, "") != 0 )
	{
#ifdef CIRCLE_WINDOWS
	sprintf(g_buf, "INSERT INTO t_pd4rank (a_index, a_name, a_exp, a_level, a_job, a_stime, a_ctime ) VALUES (%d, '%s', %I64d, %d, %d, now(), 0 )"
		,charindex, (const char*)charnick, charexp, charlevel, charjob );
#else
	sprintf(g_buf, "INSERT INTO t_pd4rank (a_index, a_name, a_exp, a_level, a_job, a_stime, a_ctime ) VALUES (%d, '%s', %lld, %d, %d, now(), 0 )"
		,charindex, (const char*)charnick, charexp, charlevel, charjob );
#endif
	}
	else
	{
#ifdef CIRCLE_WINDOWS
	sprintf(g_buf, "INSERT INTO t_pd4rank (a_index, a_name, a_exp, a_level, a_job, a_stime, a_ctime ) VALUES (%d, '%s', %I64d, %d, %d, now(), 0 )"
		,charindex, (const char*)charname, charexp, charlevel, charjob );
#else
	sprintf(g_buf, "INSERT INTO t_pd4rank (a_index, a_name, a_exp, a_level, a_job, a_stime, a_ctime ) VALUES (%d, '%s', %lld, %d, %d, now(), 0 )"
		,charindex, (const char*)charname, charexp, charlevel, charjob );
#endif
	}

	cmd.SetQuery(g_buf);
	cmd.Update();

	if( cmd.m_nrecords < 1)
	{
		/////////////////
		// 초기화 후에 이미 디비에 있으면 update
		//
		if( strcmp(charnick, "") != 0)
		{
#ifdef CIRCLE_WINDOWS
		sprintf(g_buf, "update t_pd4rank set a_name='%s', a_exp=%I64d, a_level=%d, a_job=%d, a_stime=now() where a_index=%d", (const char*)charnick, charexp, charlevel, charjob, charindex);
#else
		sprintf(g_buf, "update t_pd4rank set a_name='%s', a_exp=%lld, a_level=%d, a_job=%d, a_stime=now() where a_index=%d", (const char*)charnick, charexp, charlevel, charjob, charindex);
#endif
		}
		else
		{
#ifdef CIRCLE_WINDOWS
		sprintf(g_buf, "update t_pd4rank set a_name='%s', a_exp=%I64d, a_level=%d, a_job=%d, a_stime=now() where a_index=%d", (const char*)charname, charexp, charlevel, charjob, charindex);
#else
		sprintf(g_buf, "update t_pd4rank set a_name='%s', a_exp=%lld, a_level=%d, a_job=%d, a_stime=now() where a_index=%d", (const char*)charname, charexp, charlevel, charjob, charindex);
#endif
		}

		cmd.SetQuery(g_buf);
		if(!cmd.Update())
		{
			GAMELOG << init("PERSONAL DUNGEON 4 RANK DB ADD FAILED")
					<< charname << end;

			return;
		}

	}

}

void do_CommandPD4RankEnd(CNetMsg& msg, CDescriptor* dest)
{
	//MSG_HELPER_PD4_RANK_END,			// pd4 종료			 : charIndex(n) deadmonNum(n) bclear(c)
	//index, etime, deadmonNum, bclear

	int charindex;
	int deadmonNum=0, predeadmonNum=0;
	char bclear=0, prebclear=0;
	LONGLONG ctime=0, prectime=0;

	// 이전과 비교하여 최대 기록이면 수정 그렇지 않으면 return;
	// bclear, ctime(stime), deadmon
	msg >> charindex
		>> deadmonNum
		>> bclear;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "update t_pd4rank set a_etime = now() where a_index = %d", charindex);
	cmd.SetQuery(g_buf);

	if(!cmd.Update())
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB END FAILED1")
				<< charindex << end;
		return;
	}

	cmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "select a_deadmon, a_bclear, a_ctime, (UNIX_TIMESTAMP(a_etime) - UNIX_TIMESTAMP(a_stime)) as a_nctime  from t_pd4rank where a_index = %d", charindex);
	cmd.SetQuery(g_buf);
	
	if(!cmd.Open())
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB END FAILED2")
				<< charindex << end;
		return;
	}
	if(!cmd.MoveFirst())
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB END FAILED3")
				<< charindex << end;
		return;
	}

	if(!cmd.GetRec("a_deadmon", predeadmonNum)
		||!cmd.GetRec("a_bclear", prebclear)
		||!cmd.GetRec("a_ctime", prectime) 
		||!cmd.GetRec("a_nctime", ctime))
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB END FAILED4")
				<< charindex << end;
		return;
	}

	GAMELOG << init("PD4 END")
			<< charindex << delim
			<< deadmonNum << delim
			<< bclear << delim
			<< ctime << end;

	if( (prebclear > bclear) 
		|| ( (prebclear == bclear) && (predeadmonNum > deadmonNum) )  
		|| ( (prebclear == bclear) && (predeadmonNum == deadmonNum) && (prectime != 0 && prectime <= ctime) ) )
	{
		return;
	}
		
	cmd.Close();

	cmd.Init(&gserver.m_dbchar);

	//UPDATE t_pd4rank SET a_etime= '2005-06-13 18:50:11', a_bclear= 1, a_deadmon= 210 WHERE a_index=23
#ifdef CIRCLE_WINDOWS
	sprintf(g_buf,"UPDATE t_pd4rank SET a_bclear=%d, a_deadmon=%d, a_ctime=%I64d WHERE a_index=%d ", bclear, deadmonNum, ctime, charindex);
#else
	sprintf(g_buf,"UPDATE t_pd4rank SET a_bclear=%d, a_deadmon=%d, a_ctime=%lld WHERE a_index=%d ", bclear, deadmonNum, ctime, charindex);
#endif
	//update t_pd4rank set a_ctime = period_diff(a_etime, a_stime) where a_index = %d; , charindex

	cmd.SetQuery(g_buf);
	if( !cmd.Update())
	{
		
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB END FAILED5")
				<< charindex << end;
		return;

	}

}

void do_CommandGuildCreateReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int charindex;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString name(MAX_GUILD_NAME_LENGTH + 1);
	msg >> charindex
		>> charname
		>> name;

#ifdef NEW_GUILD
	int guildmaxmember;
	int zoneindex;
	msg >> guildmaxmember
		>> zoneindex;
#endif // NEW_GUILD
	// 기존 구성에서 검사
	CGuildMember* prev = gserver.m_guildlist.findmember(charindex);
	if (prev == NULL)
	{
		// 이전것 없으면 새로 독립길드 창설

		// DB 에 저장
		sprintf(g_buf, "INSERT INTO t_guild (a_name,a_createdate,a_recentdate) VALUES ('%s',NOW(),NOW())", (const char*)name);
		CDBCmd cmd;
		cmd.Init(&gserver.m_dbchar);
		cmd.BeginTrans();
		cmd.SetQuery(g_buf);
		if (!cmd.Update())
		{
			cmd.Rollback();
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_DUPNAME);
			SEND_Q(rmsg, dest);
			return ;
		}
		int guildindex = cmd.insertid();
		sprintf(g_buf, "INSERT INTO t_guildmember (a_guild_index,a_char_index,a_char_name,a_pos,a_regdate) VALUES (%d,%d,'%s',%d,NOW())",
			guildindex, charindex, (const char*)charname, MSG_GUILD_POSITION_BOSS);
		cmd.SetQuery(g_buf);
		if (!cmd.Update())
		{
			cmd.Rollback();
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);
			return ;
		}
#ifdef NEW_GUILD
		sprintf(g_buf, "INSERT INTO t_extend_guild (a_guild_index, a_guild_point, a_guild_maxmember, a_guild_incline ) VALUES ( %d, 0, %d, 0 )", guildindex, guildmaxmember );
		cmd.SetQuery(g_buf);
		if (!cmd.Update())
		{
			cmd.Rollback();
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_DUPNAME);
			SEND_Q(rmsg, dest);
			return ;
		}
		
		sprintf(g_buf, "INSERT INTO t_extend_guildmember (a_guild_index, a_char_index ) VALUES (%d, %d )", guildindex, charindex );
		cmd.SetQuery(g_buf);
		if (!cmd.Update())
		{
			cmd.Rollback();
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);
			return ;
		}
#endif // NEW_GUILD

		// 메모리에 만들기
		CGuild* nguild = gserver.m_guildlist.create(guildindex, name, charindex, charname);

		if (!nguild)
		{
			cmd.Rollback();
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);
			return ;
		}
#ifdef NEW_GUILD
		nguild->incline( 0 );
		nguild->setMaxmember(guildmaxmember);
		nguild->boss()->zoneindex( zoneindex );
		
#endif //NEW_GUILD

		cmd.Commit();

		GAMELOG << init("GUILD CREATE")
				<< guildindex << delim
				<< name << delim
				<< "BOSS" << delim
				<< charindex << delim
				<< charname << delim
				<< end;

		// 리스트에 추가
		gserver.m_guildlist.add(nguild);
		// 멤버 온라인
		nguild->boss()->online(1);

		// 리스트 추가 결과 알리기
		HelperGuildCreateNotifyMsg(rmsg, nguild);
		gserver.SendToAllGameServer(rmsg);

		// 온라인 알리기
		HelperGuildOnlineNotifyMsg(rmsg, nguild->boss());
#ifdef NEW_GUILD
		rmsg << zoneindex;
#endif // NEW_GUILD
		gserver.SendToAllGameServer(rmsg);

		// 성공 응답
		HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_OK);
		SEND_Q(rmsg, dest);
	}
	else
	{
		// TODO : GUILD : 트리구성시
		HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_PACKET);
		SEND_Q(rmsg, dest);
	}
}

void do_CommandShutdown(CNetMsg& msg, CDescriptor* dest)
{
	gserver.m_bshutdown = true;
}

void do_CommandGuildOnline(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;
	char online;
	msg >> guildindex
		>> charindex
		>> online;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (guild == NULL)
		return ;

	CGuildMember* member = guild->findmember(charindex);
	if (member)
	{

		member->online(online);
		HelperGuildOnlineNotifyMsg(rmsg, member);
#ifdef NEW_GUILD
		int zoneNum ;
		msg >> zoneNum;

		GAMELOG << init("do_CommandGuildOnline") << delim
				<< guild->index() << delim
			<< charindex << delim
			<< zoneNum 	<< end;

			
		member->zoneindex( zoneNum );

		rmsg << member->GetZoneNum();
#endif //NEW_GUILD

		gserver.SendToAllGameServer(rmsg);
	}
}

void do_CommandGuildLoadReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int charindex;
	CLCString idname(MAX_ID_NAME_LENGTH + 1);
	msg >> idname >> charindex;

#ifdef NEW_GUILD
	int channel;
	int zoneindex;
	msg >> channel >> zoneindex;
#endif // NEW_GUILD

	CGuildMember* member = gserver.m_guildlist.findmember(charindex);
	if (member)
	{
		CGuild* guild = member->guild();

		if (!guild)
			return;
		
		HelperGuildLoadNotifyMsg(rmsg, guild);
		SEND_Q(rmsg, dest);

#ifdef TEST_SERVER
		gserver.SendGuildMemberList(guild, dest);
#else
		for( int idx=0; idx<guild->membercount(); idx+=20 )
		{
			HelperGuildMemberListMsg(rmsg, guild, idx, 20 );
			SEND_Q(rmsg, dest);
		}
#endif // TEST_SERVER

#ifdef NEW_GUILD
		CDBCmd cmd;
		CLCString sql( 1204 );
		cmd.Init( &gserver.m_dbchar );
		sql.Format( "SELECT a_skill_index, a_skill_level FROM t_extend_guild WHERE a_guild_index = %d", guild->index() );
		cmd.SetQuery( sql );
		if( !cmd.Open() || !cmd.MoveFirst() )
		{
//			HelperNewGuildSkillListRepMsg( rmsg, charindex, guildindex, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST, 0, NULL, NULL );
//			SEND_Q( rmsg, dest );
			return;
		}
		CLCString skillIndex(256);
		CLCString skillLevel(256);
		const char* pIndex = skillIndex;
		const char* pLevel = skillLevel;
		char tbuf[256];

		int sindex[256];
		int sLevel[256];
	
		cmd.GetRec( "a_skill_index", skillIndex );
		cmd.GetRec( "a_skill_level", skillLevel );

		int roopcount = 0;
		while (*pIndex && *pLevel)
		{
			int i, l;
			pIndex = AnyOneArg(pIndex, tbuf);
			i = atoi(tbuf);
			pLevel = AnyOneArg(pLevel, tbuf);
			l = atoi(tbuf);

			sindex[roopcount] = i;
			sLevel[roopcount] = l;
			roopcount++;
		}		
		
		member->channel( channel );
		member->zoneindex( zoneindex );

		GAMELOG << init("do_CommandGuildLoadReq") << delim
				<< charindex << delim
				<< zoneindex 	<< end;

		HelperExtendGuildLoadNotifyMsg( rmsg, guild, roopcount, sindex, sLevel);
		SEND_Q( rmsg, dest );

#ifdef TEST_SERVER
		gserver.SendExtendGuildMemberList(guild, dest);
#else
		int membercount = guild->membercount();
		int loopcount = guild->membercount() / 20;
		int endcount = 0;
		int guildstart = 0;
		int i;
		for( i = 0; i < loopcount; i++ )
		{
			endcount = (i + 1) * 20;
			HelperExtendGuildMemberListMsg( rmsg, endcount, guildstart, guild );			
			gserver.SendToAllGameServer(rmsg);
		}
		int remaincount = membercount - guildstart;
		if( remaincount > 0 )
		{
			HelperExtendGuildMemberListMsg( rmsg, membercount, guildstart, guild );			
			gserver.SendToAllGameServer(rmsg);

		}
#endif // WJKTEST

#endif // NEW_GUILD

		// 길드전 중인 길드이면
		if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
		{
			CGuild* g = gserver.m_guildlist.findguild(guild->battleIndex());
			if (g)
			{
				HelperGuildLoadNotifyMsg(rmsg, g);
				SEND_Q(rmsg, dest);
#ifdef WJKTEST
				gserver.SendGuildMemberList(g, dest);
#else
				for( int idx=0; idx<guild->membercount(); idx+=20 )
				{
					HelperGuildMemberListMsg(rmsg, g, idx, 20 );
					SEND_Q(rmsg, dest);
				}
#endif // WJKTEST
			}
		}
	}
	
	do_CommandTeacherLoad(msg, dest);

#ifdef GUILD_MARK_TABLE
	HelperGuildMarkTableMsg(rmsg);
	SEND_Q(rmsg, dest);
#endif // GUILD_MARK_TABLE
}

void do_CommandGuildLevelUpReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;
	msg >> guildindex
		>> charindex;

#ifdef NEW_GUILD
	int guildmaxmember;
	msg >> guildmaxmember;
#endif // NEW_GUILD

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (guild->level() >= GUILD_MAX_GUILD_LEVEL)
	{
		HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_MAXLEVEL);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (guild->boss()->charindex() != charindex)
	{
		HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	sprintf(g_buf, "UPDATE t_guild SET a_level=a_level+1, a_recentdate=NOW() WHERE a_index=%d", guildindex);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.BeginTrans();
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return ;
	}

#ifdef NEW_GUILD
	int gap = 0;
	int needGP = 0;
	if( guild->level() + 1 >= GUILD_EXTEND_BASE_LEVEL )
	{
		gap = guild->level() + 1 - GUILD_EXTEND_BASE_LEVEL;
		if( gap >= 0 )
		{
			needGP		= (int)(GUILD_LEVEL7_NEED_GP * pow(1.09, gap));
		}
		else
		{
			cmd.Rollback();
			HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);				
			return ;
		}

		sprintf( g_buf, "UPDATE t_extend_guild SET a_guild_point = a_guild_point - %d, a_guild_maxmember = %d WHERE a_guild_index = %d", needGP, guildmaxmember, guildindex  );
		cmd.SetQuery( g_buf );
		if( !cmd.Update() )
		{
			cmd.Rollback();
			HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);				
			return ;
		}
	}
	else
	{
		sprintf( g_buf, "UPDATE t_extend_guild SET a_guild_maxmember = %d WHERE a_guild_index = %d", guildmaxmember, guildindex  );
		cmd.SetQuery( g_buf );
		if( !cmd.Update() )
		{
			cmd.Rollback();
			HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);				
			return ;
		}
	}
	guild->setMaxmember( guildmaxmember );
	guild->AddGuildPoint(-needGP);
#endif // NEW_GUILD
	cmd.Commit();

	GAMELOG << init("GUILD LEVELUP")
			<< guild->index() << delim
			<< guild->name() << delim
			<< guild->level()
			<< end;

	guild->LevelUp();
	

	HelperGuildLevelUpNotifyMsg(rmsg, guildindex, guild->level());
	gserver.SendToAllGameServer(rmsg);

	HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_OK);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildBreakUpReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int bossindex;
	int guildindex;

	msg >> bossindex
		>> guildindex;

	do_CommandGuildBreakUpReq_real(dest, bossindex, guildindex);
}

void do_CommandGuildMemberAddReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int guildindex;
	int bossindex;
	int charindex;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);

	msg >> guildindex
		>> bossindex
		>> charindex
		>> name;
#ifdef NEW_GUILD
	int zoneindex;
	msg >> zoneindex;
#endif // NEW_GUILD

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (guild->maxmember() <= guild->membercount())
	{
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_FULLMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* boss = guild->findmember(bossindex);
	if (!boss)
	{
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_TARGET_NOBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (boss->pos() == MSG_GUILD_POSITION_MEMBER)
	{
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_TARGET_NOBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	// DB
	sprintf(g_buf, "INSERT INTO t_guildmember (a_guild_index,a_char_index,a_char_name,a_pos,a_regdate) VALUES (%d,%d,'%s',%d,NOW())", guildindex, charindex, (const char*)name, MSG_GUILD_POSITION_MEMBER);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.BeginTrans();
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return ;
	}
#ifdef NEW_GUILD
	sprintf(g_buf, "INSERT INTO t_extend_guildmember (a_guild_index, a_char_index ) VALUES (%d, %d )", guildindex, charindex );
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		cmd.Rollback();
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_FULLMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
#endif // NEW_GUILD
	cmd.Commit();

	int listindex = guild->addmember(charindex, name);
	if (listindex == -1)
	{
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_FULLMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	sprintf(g_buf, "UPDATE t_guild SET a_recentdate=NOW() WHERE a_index=%d", guildindex);
	cmd.SetQuery(g_buf);
	cmd.Update();

	GAMELOG << init("GUILD MEMBER ADD")
			<< guild->index() << delim
			<< guild->name() << delim
			<< "AGREE" << delim
			<< boss->charindex() << delim
			<< boss->GetName() << delim
			<< "CHARACTER" << delim
			<< charindex << delim
			<< name << delim
			<< end;

	CGuildMember* member = guild->member(listindex);
	HelperGuildMemberAddNotifyMsg(rmsg, guildindex, charindex, name, member->pos());
#ifdef NEW_GUILD
	member->zoneindex( zoneindex );

	GAMELOG << init("do_CommandGuildMemberAddReq") << delim
			<< guild->index() << delim			
			<< zoneindex 	<< end;
		
	rmsg << zoneindex;
#endif // NEW_GUILD
	gserver.SendToAllGameServer(rmsg);
	HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_OK);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildMemberOutReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;
	msg >> guildindex
		>> charindex;

	do_CommandGuildMemberOutReq_real(dest, guildindex, charindex, true);
}

void do_CommandGuildMemberKickReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int guildindex;
	int bossindex;
	int charindex;

	msg >> guildindex
		>> bossindex
		>> charindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* boss = guild->findmember(bossindex);
	if (!boss)
	{
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (boss->pos() == MSG_GUILD_POSITION_MEMBER)
	{
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* kick = guild->findmember(charindex);
	if (!kick)
	{
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (kick->pos() == MSG_GUILD_POSITION_BOSS)
	{
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_CANTKICKBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (boss->pos() == MSG_GUILD_POSITION_OFFICER && kick->pos() == MSG_GUILD_POSITION_OFFICER)
	{
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_CANTKICKBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (!gserver.DeleteGuildMember(guildindex, guild->name(), charindex, kick->GetName(), true))
	{
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}

	HelperGuildMemberKickNotifyMsg(rmsg, guildindex, bossindex, charindex);
	gserver.SendToAllGameServer(rmsg);
	guild->removemember(kick);
	HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_OK);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildChangeBossReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int guildindex;
	int current;
	int change;

	msg >> guildindex
		>> current
		>> change;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* boss = guild->findmember(current);
	if (!boss)
	{
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (boss->pos() == MSG_GUILD_POSITION_MEMBER)
	{
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* member = guild->findmember(change);
	if (!member)
	{
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (member->pos() == MSG_GUILD_POSITION_BOSS)
	{
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_CANTKICKBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.BeginTrans();

	sprintf(g_buf, "UPDATE t_guildmember SET a_pos=%d WHERE a_guild_index=%d AND a_char_index=%d", MSG_GUILD_POSITION_MEMBER, guildindex, current);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		cmd.Rollback();
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}

	sprintf(g_buf, "UPDATE t_guildmember SET a_pos=%d WHERE a_guild_index=%d AND a_char_index=%d", MSG_GUILD_POSITION_BOSS, guildindex, change);
	cmd.SetQuery(g_buf);
	if (!cmd.Update() || cmd.m_nrecords < 1)
	{
		cmd.Rollback();
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	cmd.Commit();

	sprintf(g_buf, "UPDATE t_guild SET a_recentdate=NOW() WHERE a_index=%d", guildindex);
	cmd.SetQuery(g_buf);
	cmd.Update();

	GAMELOG << init("GUILD CHANGE BOSS")
			<< guild->index() << delim
			<< guild->name() << delim
			<< boss->charindex() << delim
			<< boss->GetName()
			<< " ==> "
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

	guild->changeboss(member->listindex());

	HelperGuildChangeBossNotifyMsg(rmsg, guildindex, current, change);
	gserver.SendToAllGameServer(rmsg);
	HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_OK);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildAppointOfficerReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int guildindex;
	int bossindex;
	int charindex;

	msg >> guildindex
		>> bossindex
		>> charindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* boss = guild->findmember(bossindex);
	if (!boss)
	{
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (boss->pos() != MSG_GUILD_POSITION_BOSS)
	{
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
	{
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (member->pos() != MSG_GUILD_POSITION_MEMBER)
	{
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_CANTOFFICER);
		SEND_Q(rmsg, dest);
		return ;
	}

	int i;
	for (i = 0; i < GUILD_MAX_OFFICER; i++)
	{
		if (guild->officer(i) == NULL)
			break;
	}
	if (i == GUILD_MAX_OFFICER)
	{
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_FULLOFFICER);
		SEND_Q(rmsg, dest);
		return ;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "UPDATE t_guildmember SET a_pos=%d WHERE a_guild_index=%d AND a_char_index=%d", MSG_GUILD_POSITION_OFFICER, guildindex, charindex);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	sprintf(g_buf, "UPDATE t_guild SET a_recentdate=NOW() WHERE a_index=%d", guildindex);
	cmd.SetQuery(g_buf);
	cmd.Update();

	GAMELOG << init("GUILD APPOINT OFFICER")
			<< guild->index() << delim
			<< guild->name() << delim
			<< "OFFICER" << delim
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

	guild->appoint(member->listindex());

	HelperGuildAppointOfficerNotifyMsg(rmsg, guildindex, bossindex, charindex);
	gserver.SendToAllGameServer(rmsg);
	HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_OK);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildChat(CNetMsg& msg, CDescriptor* dest)
{
	int guildindex;
	int charindex;

	msg >> guildindex
		>> charindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (guild)
	{
		if (guild->findmember(charindex))
		{
			gserver.SendToAllGameServer(msg);
		}
	}
}

void do_CommandGuildFireOfficerReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int guildindex;
	int bossindex;
	int charindex;

	msg >> guildindex
		>> bossindex
		>> charindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* boss = guild->findmember(bossindex);
	if (!boss)
	{
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (boss->pos() != MSG_GUILD_POSITION_BOSS)
	{
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
	{
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (member->pos() != MSG_GUILD_POSITION_OFFICER)
	{
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTOFFICER);
		SEND_Q(rmsg, dest);
		return ;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "UPDATE t_guildmember SET a_pos=%d WHERE a_guild_index=%d AND a_char_index=%d", MSG_GUILD_POSITION_MEMBER, guildindex, charindex);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	sprintf(g_buf, "UPDATE t_guild SET a_recentdate=NOW() WHERE a_index=%d", guildindex);
	cmd.SetQuery(g_buf);
	cmd.Update();

	GAMELOG << init("GUILD FIRE OFFICER")
			<< guild->index() << delim
			<< guild->name() << delim
			<< "OFFICER" << delim
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

	guild->fire(member->listindex());

	HelperGuildFireOfficerNotifyMsg(rmsg, guildindex, bossindex, charindex);
	gserver.SendToAllGameServer(rmsg);
	HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_OK);
	SEND_Q(rmsg, dest);
}

void do_CommandCharacterDelete(CNetMsg& msg, CDescriptor* dest)
{
	int charindex;
	msg >> charindex;

	CNetMsg rmsg;

	CGuildMember* member = gserver.m_guildlist.findmember(charindex);
	if (!member)
		return ;

	CGuild* guild = member->guild();
	if (guild->membercount() < 2)
	{
		// 1인 길드 삭제 : 멤버를 DB에서만 삭제
		gserver.DeleteGuildMember(guild->index(), guild->name(), charindex, member->GetName(), false);
		HelperGuildMemberOutNotifyMsg(rmsg, guild->index(), charindex, member->GetName());
		gserver.SendToAllGameServer(rmsg);
		guild->removemember(member);
		return;
	}
	else if (member->pos() == MSG_GUILD_POSITION_BOSS)
	{
		// 길드장 이임
		CGuildMember* newboss = NULL;

		// 부관1 검사
		if (guild->officer(0) != NULL)
			newboss = guild->officer(0);

		// 부관2 검사
		else if (guild->officer(1) != NULL)
			newboss = guild->officer(1);

		// 일반 길원 검사
		else
		{
			int i;
			for (i = 0; i < guild->maxmember(); i++)
			{
				newboss = guild->member(i);
				if (newboss && newboss != member)
					break;
			}
		}

		// 아무도 안되면 그냥 삭제
		if (newboss == NULL)
		{
			gserver.DeleteGuildMember(guild->index(), guild->name(), charindex, member->GetName(), false);
			HelperGuildMemberOutNotifyMsg(rmsg, guild->index(), charindex, member->GetName());
			gserver.SendToAllGameServer(rmsg);
			guild->removemember(member);
			return ;
		}

		// 선택된 길원을 보스로 : 오류 발생시 길드멤버에서 삭제
		
		
		CDBCmd cmd;
		cmd.Init(&gserver.m_dbchar);
		cmd.BeginTrans();
		
		sprintf(g_buf, "UPDATE t_guildmember SET a_pos=%d WHERE a_guild_index=%d AND a_char_index=%d", MSG_GUILD_POSITION_MEMBER, guild->index(), member->charindex());
		cmd.SetQuery(g_buf);
		if (!cmd.Update())
		{
			cmd.Rollback();
			gserver.DeleteGuildMember(guild->index(), guild->name(), charindex, member->GetName(), false);
			HelperGuildMemberOutNotifyMsg(rmsg, guild->index(), charindex, member->GetName());
			gserver.SendToAllGameServer(rmsg);
			guild->removemember(member);
			return ;
		}
		
		sprintf(g_buf, "UPDATE t_guildmember SET a_pos=%d WHERE a_guild_index=%d AND a_char_index=%d", MSG_GUILD_POSITION_BOSS, guild->index(), newboss->charindex());
		cmd.SetQuery(g_buf);
		if (!cmd.Update() || cmd.m_nrecords < 1)
		{
			cmd.Rollback();
			gserver.DeleteGuildMember(guild->index(), guild->name(), charindex, member->GetName(), false);
			HelperGuildMemberOutNotifyMsg(rmsg, guild->index(), charindex, member->GetName());
			gserver.SendToAllGameServer(rmsg);
			guild->removemember(member);
			return ;
		}
		cmd.Commit();
		
		sprintf(g_buf, "UPDATE t_guild SET a_recentdate=NOW() WHERE a_index=%d", guild->index());
		cmd.SetQuery(g_buf);
		cmd.Update();
		
		GAMELOG << init("GUILD CHANGE BOSS")
				<< guild->index() << delim
				<< guild->name() << delim
				<< member->charindex() << delim
				<< member->GetName()
				<< " ==> "
				<< newboss->charindex() << delim
				<< newboss->GetName()
				<< end;
		
		guild->changeboss(newboss->listindex());
		
		HelperGuildChangeBossNotifyMsg(rmsg, guild->index(), member->charindex(), newboss->charindex());
		gserver.SendToAllGameServer(rmsg);
	}
	// 길드 탈퇴
	do_CommandGuildMemberOutReq_real(dest, member->guild()->index(), member->charindex(), false);
}

void do_CommandGuildBreakUpReq_real(CDescriptor* dest, int bossindex, int guildindex)
{
	CNetMsg rmsg;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if( !guild )
	{
		HelperGuildBreakUpRepMsg(rmsg, bossindex, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	
	if ( guild->boss()->charindex() != bossindex)
	{
		HelperGuildBreakUpRepMsg(rmsg, bossindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 길드전 중
	if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
		return;

	// DB
	if (!gserver.DeleteGuild(guildindex))
	{
		HelperGuildBreakUpRepMsg(rmsg, bossindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	GAMELOG << init("GUILD BREAK")
			<< guild->index() << delim
			<< guild->name()
			<< end;

	// TODO : GUILD : 하위길드 독립

	gserver.m_guildlist.Remove(guild);

	HelperGuildBreakUpNotifyMsg(rmsg, guildindex);
	gserver.SendToAllGameServer(rmsg);

	HelperGuildBreakUpRepMsg(rmsg, bossindex, MSG_GUILD_ERROR_OK);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildMemberOutReq_real(CDescriptor* dest, int guildindex, int charindex, bool bSendError)
{
	CNetMsg rmsg;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		if (bSendError)
		{
			HelperGuildMemberOutRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_NOGUILD);
			SEND_Q(rmsg, dest);
		}
		return ;
	}

	if (guild->boss()->charindex() == charindex)
	{
		if (bSendError)
		{
			HelperGuildMemberOutRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_OUTBOSS);
			SEND_Q(rmsg, dest);
		}
		return ;
	}

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
	{
		if (bSendError)
		{
			HelperGuildMemberOutRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
			SEND_Q(rmsg, dest);
		}
		return ;
	}
	if (member->pos() == MSG_GUILD_POSITION_BOSS)
	{
		if (bSendError)
		{
			HelperGuildMemberOutRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_OUTBOSS);
			SEND_Q(rmsg, dest);
		}
		return ;
	}

	if (!gserver.DeleteGuildMember(guildindex, guild->name(), charindex, member->GetName(), false))
	{
		if (bSendError)
		{
			HelperGuildMemberOutRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
			SEND_Q(rmsg, dest);
		}
		return ;
	}

	HelperGuildMemberOutNotifyMsg(rmsg, guildindex, charindex, member->GetName());
	gserver.SendToAllGameServer(rmsg);
	guild->removemember(member);
	if (bSendError)
	{
		HelperGuildMemberOutRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_OK);
		SEND_Q(rmsg, dest);
	}
}

void do_CommandGuildBattleReq(CNetMsg& msg, CDescriptor* dest)
{
	int guildindex1;
	int guildindex2;
	int prize;
	int zone;
	int time;


	msg >> guildindex1
		>> guildindex2
		>> prize
		>> zone
		>> time;

	// 헬퍼 2배
	time *= 2;

	CGuild* g1 = gserver.m_guildlist.findguild(guildindex1);
	CGuild* g2 = gserver.m_guildlist.findguild(guildindex2);

	if (!g1 || !g2)
		return;

	// DB UPDATE
	sprintf (g_buf, "UPDATE t_guild SET a_battle_index=%d, a_battle_prize=%d, a_battle_zone=%d, a_battle_time=%d, a_battle_state=%d WHERE a_index=%d",
		guildindex2, prize * 2 * 95 / 100, zone, time, GUILD_BATTLE_STATE_WAIT, guildindex1);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		return ;
	}

	sprintf (g_buf, "UPDATE t_guild SET a_battle_index=%d, a_battle_prize=%d, a_battle_zone=%d, a_battle_time=%d, a_battle_state=%d WHERE a_index=%d",
		guildindex1, prize * 2 * 95 / 100, zone, time, GUILD_BATTLE_STATE_WAIT, guildindex2);
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		return ;
	}

	// 세부 셋팅
	g1->BattleSet(guildindex2, prize * 2 * 95 / 100, zone);
	g1->BattleTime(time);
	g1->BattleState(GUILD_BATTLE_STATE_WAIT);

	g2->BattleSet(guildindex1, prize * 2 * 95 / 100, zone);
	g2->BattleTime(time);
	g2->BattleState(GUILD_BATTLE_STATE_WAIT);

	// 시작 시간 셋팅
	g1->BattlePulse(gserver.m_pulse);
	g2->BattlePulse(gserver.m_pulse);

	GAMELOG << init("GUILD BATTLE INIT")
			<< g1->index() << delim
			<< g1->name() << delim
			<< "<==>" << delim
			<< g2->index() << delim
			<< g2->name() << delim
			<< g1->battlePrize() << delim
			<< g1->battleZone() << delim
			<< g1->battleTime()
			<< end;

	CNetMsg rmsg;
	HelperGuildBattleRepMsg(rmsg, g1, g2, prize);
	gserver.SendToAllGameServer(rmsg);
}

void do_CommandGuildBattleStopReq(CNetMsg& msg, CDescriptor* dest)
{
	int guildindex1;
	int guildindex2;

	msg >> guildindex1
		>> guildindex2;

	CGuild* g1 = gserver.m_guildlist.findguild(guildindex1);
	CGuild* g2 = gserver.m_guildlist.findguild(guildindex2);

	if (!g1 || !g2)
		return;

	if (g1->battleState() != GUILD_BATTLE_STATE_WAIT &&
			g1->battleState() != GUILD_BATTLE_STATE_ING &&
			g1->battleState() != GUILD_BATTLE_STATE_STOP_WAIT)
	{
		return ;
	}

	if (g2->battleState() != GUILD_BATTLE_STATE_WAIT &&
			g2->battleState() != GUILD_BATTLE_STATE_ING &&
			g2->battleState() != GUILD_BATTLE_STATE_STOP_WAIT)
	{
		return ;
	}

	// 승리길드
	CGuild* winner;

#if defined (WJKTEST) || defined (LC_MAL)
	CGuild* loser;
#endif //#if defined (WJKTEST) || defined (LC_MAL)

	int prize = g1->battlePrize();
	int zone = g1->battleZone();

	// killcount 비교
	if (g1->killCount() > g2->killCount())
	{
		winner = g1;

#if defined (WJKTEST) || defined (LC_MAL)
		loser  = g2;
#endif //#if defined (WJKTEST) || defined (LC_MAL)

	}
	else if (g1->killCount() < g2->killCount())
	{
		winner = g2;

#if defined (WJKTEST) || defined (LC_MAL)
		loser  = g1;
#endif //#if defined (WJKTEST) || defined (LC_MAL)

	}
	else
	{
		winner = NULL;

#if defined (WJKTEST) || defined (LC_MAL)
		loser  = NULL;
#endif //#if defined (WJKTEST) || defined (LC_MAL)

	}
	// msg 부터 만든다
	CNetMsg rmsg;
	if (winner)
		HelperGuildBattleStopRepMsg(rmsg, winner->index(), g1, g2);
	else
		HelperGuildBattleStopRepMsg(rmsg, -1, g1, g2);

	if (winner)
	{
		if (winner == g1)
		{
			g2->BattleSet(-1, 0, -1);
			g2->BattleTime(0);
			g2->KillCount(0);
			g2->BattleState(GUILD_BATTLE_STATE_PEACE);
		}
		else if (winner == g2)
		{
			g1->BattleSet(-1, 0, -1);
			g1->BattleTime(0);
			g1->KillCount(0);
			g1->BattleState(GUILD_BATTLE_STATE_PEACE);
		}

		winner->BattleState(GUILD_BATTLE_STATE_PRIZE);
	}
	else
	{
		g1->BattleSet(-1, prize / 2, -1);
		g1->BattleState(GUILD_BATTLE_STATE_PRIZE);
		g2->BattleSet(-1, prize / 2, -1);
		g2->BattleState(GUILD_BATTLE_STATE_PRIZE);
	}

	// DB UPDATE
	sprintf (g_buf, "UPDATE t_guild SET a_battle_index=%d, a_battle_prize=%d, a_battle_zone=%d, a_battle_time=%d, a_battle_killcount=%d, a_battle_state=%d WHERE a_index=%d",
		g1->battleIndex(), g1->battlePrize(), g1->battleZone(), g1->battleTime(), g1->killCount(), (int)g1->battleState(), g1->index());
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		return ;
	}

	sprintf (g_buf, "UPDATE t_guild SET a_battle_index=%d, a_battle_prize=%d, a_battle_zone=%d, a_battle_time=%d, a_battle_killcount=%d, a_battle_state=%d WHERE a_index=%d",
		g2->battleIndex(), g2->battlePrize(), g2->battleZone(), g2->battleTime(), g2->killCount(), (int)g2->battleState(), g2->index());
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		return ;
	}


#if defined (WJKTEST) || defined (LC_MAL)
	sprintf(g_buf, "INSERT INTO t_guildwar (a_index, a_guildindex1, a_killcount1, a_guildindex2, a_killcount2, a_winnerindex, a_prize, a_battledate) VALUES (NULL, %d, %d, %d, %d, %d, %d, NOW())", 
		g1->index(), g1->killCount(), g2->index(), g2->killCount(), (winner ? winner->index() : 0 ), prize);

	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		return ;
	}
#endif //#if defined (WJKTEST) || defined (LC_MAL)


	GAMELOG << init("GUILD BATTLE END")
			<< g1->index() << delim
			<< g1->name() << delim
			<< "<==>" << delim
			<< g2->index() << delim
			<< g2->name() << delim
			<< prize << delim
			<< zone << delim
			<< "WINNER" << delim;

	if (winner)
		GAMELOG << winner->index();
	else
		GAMELOG << "-1";

	GAMELOG	<< end;
	
	gserver.SendToAllGameServer(rmsg);
}

void do_CommandGuildBattlePeaceReq(CNetMsg& msg, CDescriptor* dest)
{
	int guildindex;

	msg >> guildindex;

	CGuild* g = gserver.m_guildlist.findguild(guildindex);

	if (!g)
		return;

	if (g->battleState() != GUILD_BATTLE_STATE_PRIZE)
		return;
	
	g->BattleSet(-1, 0, -1);
	g->BattleTime(0);
	g->KillCount(0);
	g->BattleState(GUILD_BATTLE_STATE_PEACE);

	sprintf (g_buf, "UPDATE t_guild SET a_battle_index=%d, a_battle_prize=%d, a_battle_zone=%d, a_battle_time=%d, a_battle_killcount=%d, a_battle_state=%d WHERE a_index=%d",
		-1, 0, -1, 0, 0, -1, g->index());

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		return;
	}

	GAMELOG << init("GUILD BATTLE GET PRIZE")
			<< g->index() << delim
			<< g->name()
			<< end;

	CNetMsg rmsg;
	HelperGuildBattlePeaceRepMsg(rmsg, g);
	gserver.SendToAllGameServer(rmsg);
}


void do_CommandGuildBattleKillReq(CNetMsg& msg, CDescriptor* dest)
{
	int of_guidlindex;
	int df_guildindex;

	msg >> of_guidlindex
		>> df_guildindex;

	CGuild* g1 = gserver.m_guildlist.findguild(of_guidlindex);
	CGuild* g2 = gserver.m_guildlist.findguild(df_guildindex);

	if (!g1 || !g2)
		return;

	if (g1->battleIndex() != g2->index())
		return;

	if (g2->battleIndex() != g1->index())
		return;

	int killcount = g1->killCount();
	killcount++;

	sprintf (g_buf, "UPDATE t_guild SET a_battle_killcount=%d WHERE a_index=%d",
		killcount, g1->index());

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		return;
	}

	g1->KillCount(killcount);

	GAMELOG << init("GUILD BATTLE KILLCOUNT")
			<< g1->index() << delim
			<< g1->name() << delim
			<< g1->killCount()
			<< end;

	CNetMsg rmsg;
	HelperGuildBattleStatusMsg(rmsg, g1, g2);
	gserver.SendToAllGameServer(rmsg);

}
//0503 kown 
void do_CommandEventMoonStoneUpdateReq(CNetMsg& msg, CDescriptor* dest)
{
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	
	sprintf(g_buf2,
		" WHERE a_group_index=%d"
		" AND a_subgroup_index=%d"
		" AND a_event_type=%d"
		
		,gserver.m_serverno,
		0,
		0);

	gserver.m_nMoonStoneNas += 200;

	const static int maxNas = 10000000;
	if(gserver.m_nMoonStoneNas > maxNas)//1000만 이상 안모이도록 한다.
		gserver.m_nMoonStoneNas = maxNas;

	sprintf(g_buf, "UPDATE t_event_dev SET a_moonstone=%d WHERE a_group_index=%d AND a_subgroup_index=0 AND a_event_type=0", gserver.m_nMoonStoneNas, gserver.m_serverno);	cmd.SetQuery(g_buf);
	if (!cmd.Update() || cmd.m_nrecords < 1)
	{
		sprintf(g_buf, "INSERT INTO t_event_dev (a_group_index, a_subgroup_index, a_event_type, a_moonstone) VALUES (%d, 0, 0, %d)", gserver.m_serverno, gserver.m_nMoonStoneNas);
		cmd.SetQuery(g_buf);
		cmd.Update();
	}

	CNetMsg rmsg;
	HelperEventMoonStoneUpdateRepMsg(rmsg, gserver.m_nMoonStoneNas);
	gserver.SendToAllGameServer(rmsg);
}

void do_CommandEventMoonStoneJackPotReq(CNetMsg& msg, CDescriptor* dest)
{
	int chaindex;
	msg >> chaindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sprintf(g_buf2,
		" WHERE a_group_index=%d"
		" AND a_subgroup_index=%d"
		" AND a_event_type=%d"
		
		,gserver.m_serverno,
		0,
		0);

	sprintf(g_buf, "UPDATE t_event_dev SET a_moonstone=0 WHERE a_group_index=%d AND a_subgroup_index=0 AND a_event_type=0", gserver.m_serverno);
	cmd.SetQuery(g_buf);
	if (!cmd.Update() || cmd.m_nrecords < 1)
	{
		sprintf(g_buf, "INSERT INTO t_event_dev (a_group_index, a_subgroup_index, a_event_type, a_moonstone) VALUES (%d, 0, 0, 0)", gserver.m_serverno);
	cmd.SetQuery(g_buf);
	cmd.Update();
	}
	
	CNetMsg rmsg;
	HelperEventMoonStoneJackPotRepMsg(rmsg, gserver.m_nMoonStoneNas, chaindex);
	SEND_Q(rmsg, dest);

	gserver.m_nMoonStoneNas = 0; //초기화.

	CNetMsg hmsg;
	HelperEventMoonStoneUpdateRepMsg(hmsg, gserver.m_nMoonStoneNas);
	gserver.SendToAllGameServer(hmsg);
}

void do_CommandFriendMemberAddReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int approval_index, approval_job,
		request_index, request_job;

	CLCString approval_name(MAX_CHAR_NAME_LENGTH + 1);
	CLCString request_name(MAX_CHAR_NAME_LENGTH + 1);

	msg >> approval_index
		>> approval_name
		>> approval_job
		>> request_index
		>> request_name
		>> request_job;



	//DB에 접속해서 처리.. 

	//먼저 친구등록 요청한 캐릭에 친구등록
	CLCString m_buf(1024);		

	m_buf.Format("INSERT INTO t_friend%02d (a_char_index,a_friend_index,a_friend_name,a_friend_job) VALUES ( %d,%d,'%s',%d)",
		request_index % 10, request_index, approval_index, (const char*)approval_name, approval_job);



	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(m_buf);
	if (!cmd.Update())
	{
		HelperFriendMemberAddRepMsg(rmsg, approval_index, approval_name, approval_job, request_index, request_name, request_job, MSG_FRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return ;
	}
	
	//승인한 캐릭에도 친구등록	

	m_buf.Format("INSERT INTO t_friend%02d (a_char_index,a_friend_index,a_friend_name,a_friend_job) VALUES ( %d,%d,'%s',%d)",
		approval_index % 10, approval_index, request_index, (const char*)request_name, request_job);	



	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(m_buf);
	if (!cmd.Update())
	{
		HelperFriendMemberAddRepMsg(rmsg, approval_index, approval_name, approval_job, request_index, request_name, request_job, MSG_FRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return ;
	}
	
	HelperFriendMemberAddRepMsg(rmsg, approval_index, approval_name, approval_job, request_index, request_name, request_job, MSG_FRIEND_ERROR_OK);
	SEND_Q(rmsg, dest);
}

void do_CommandFriendMemberDelReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;
	
	
	int removerindex;//삭제요청자.
	int deleteindex; //삭제하려는 아이디.
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	
	msg >> removerindex
		>> deleteindex
		>> name;			
	
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	
	CLCString table(20);
	CLCString table2(20);
	table.Format("t_friend%02d", removerindex % 10);
	table2.Format("t_friend%02d", deleteindex % 10);
	
	//삭제요청자 DB지우기.
	sprintf(g_buf, "DELETE FROM %s WHERE a_char_index=%d AND a_friend_index=%d", (const char*)table, removerindex, deleteindex);
	
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		HelperFriendMemberDelRepMsg(rmsg, removerindex, deleteindex, MSG_FRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return;
	}
	
	cmd.Init(&gserver.m_dbchar);
	//삭제 당한자 DB지우기.	
	sprintf(g_buf, "DELETE FROM %s WHERE a_char_index=%d AND a_friend_index=%d", (const char*)table2, deleteindex, removerindex);		
	
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		HelperFriendMemberDelRepMsg(rmsg, removerindex, deleteindex, MSG_FRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return;
	}
	
	HelperFriendMemberDelRepMsg(rmsg, removerindex, deleteindex, MSG_FRIEND_ERROR_OK);
//0521 전체에 보내줘야 할듯..
//	SEND_Q(rmsg, dest);
	gserver.SendToAllGameServer(rmsg);
}

#ifdef MESSENGER_NEW
void do_CommandBlockCharReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;
	
	int reqIndex;
	int blockIndex = -1;
	CLCString blockName(MAX_CHAR_NAME_LENGTH + 1);
	
	msg >> reqIndex
		>> blockName;
	
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	
	sprintf(g_buf, " select a_index from t_characters where a_nick = '%s' ", (const char*) blockName);
	cmd.SetQuery(g_buf);
	if( !cmd.Open() || cmd.m_nrecords < 1 || !cmd.MoveFirst())
	{
		HelperBlockCharRepMsg(rmsg, reqIndex, -1, "" );
		SEND_Q(rmsg, dest);
		return;
	}

	cmd.GetRec("a_index", blockIndex);

	HelperBlockCharRepMsg(rmsg, reqIndex, blockIndex, blockName );
	gserver.SendToAllGameServer(rmsg);
}
#endif

#ifdef CASH_ITEM_GIFT
void do_CommandGiftCharReq(CNetMsg& msg, CDescriptor* dest)
{
	//MSG_HELPER_GIFT_RECVCHARNAME_REQ,			// 핼퍼에 해당 케릭터가 있는 : sendCharIndex(n) recvCharName(str) msg(str) count(n) idx(n) ctid(n)
	//MSG_HELPER_GIFT_RECVCHARNAME_REP,			//							: sendCharIndex(n) recvCharIndex(n) revCharName(str) msg(str) count(n) idx(n) ctid(n)
	int sendCharIdx, sendUserIdx;
	CLCString recvCharName(MAX_CHAR_NAME_LENGTH + 1);
	CLCString sendMsg(MAX_GIFT_MESSAGE_LENGTH + 1);
	int count;
	int idx[MAX_PURCHASEITEM];
	int ctid[MAX_PURCHASEITEM];
	int i;
	int recvuserIndex, recvcharIndex;

	msg >> sendUserIdx
		>> sendCharIdx
		>> recvCharName
		>> sendMsg
		>> count;

	for(i = 0; i < count; i++)
	{
		msg >> idx[i]
			>> ctid[i];
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sprintf(g_buf, "select a_index, a_user_index from t_characters where a_enable = 1 and a_nick = '%s' and a_user_index != %d ", (const char*) recvCharName, sendUserIdx );
	cmd.SetQuery(g_buf);

	CNetMsg rmsg;
	if( !cmd.Open() || cmd.m_nrecords < 1 )
	{
		HelperGiftCharRepMsg(rmsg, sendUserIdx, sendCharIdx, -1, -1, recvCharName, sendMsg, count, idx, ctid);
		SEND_Q(rmsg, dest);
		return;
	}

	if( !cmd.MoveFirst() )
	{

		HelperGiftCharRepMsg(rmsg, sendUserIdx, sendCharIdx, -1, -1, recvCharName, sendMsg, count, idx, ctid);
		SEND_Q(rmsg, dest);
		return;
	}

	if( !cmd.GetRec("a_user_index", recvuserIndex ) || !cmd.GetRec("a_index", recvcharIndex) )
	{
		
		HelperGiftCharRepMsg(rmsg, sendUserIdx, sendCharIdx, -1, -1, recvCharName, sendMsg, count, idx, ctid);
		SEND_Q(rmsg, dest);
		return;
	}

	HelperGiftCharRepMsg(rmsg, sendUserIdx, sendCharIdx, recvuserIndex, recvcharIndex, recvCharName, sendMsg, count, idx, ctid);
	SEND_Q(rmsg, dest);
}
#endif

void do_CommandFriendSetConditionReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;
	
	int chaindex;
	int condition;
	int sum;
	int index[FRIEND_MAX_MEMBER];
	int reply;
	
	msg >> chaindex
		>> condition
		>> reply;

	if(reply == -1)
	{
	msg >> sum;

		for(int i=0 ; i<sum ; i++)
		{
			msg >> index[i];
		}
		/*
		CPC* pCh = gserver.m_playerList.Find(chaindex);
		pCh->SetMessengerCondition(condition);//우선 자신의 상태를 셋팅한다..나중에 접속하는 사람에게 상태를 알리려면..
	*/
	//접속하고 있는 친구들에게 알려야한다..
	HelperFriendSetConditionNotifyMsg(rmsg, chaindex, condition, -1, sum, index);
//0521
//	SEND_Q(rmsg, dest);
	gserver.SendToAllGameServer(rmsg);
	}
	else
	{
		CNetMsg tmsg;
		HelperFriendSetConditionNotifyMsg(tmsg, chaindex, condition, reply, 0, NULL);
		gserver.SendToAllGameServer(tmsg);
	}
	
}

void do_CommandPetCreateReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int owner;
	char typegrade;

	msg >> owner
		>> typegrade;


	// 이미 펫을 가지고 있으면 생성하지 않고 페일 메세지 보낸닷
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	sprintf(g_buf, " select a_index from t_pet where a_owner = %d and a_enable = 1", owner);
	cmd.SetQuery(g_buf);
	cmd.Open();

	if( cmd.m_nrecords >= MAX_OWN_PET )
	{
		HelperPetCreateRepMsg(rmsg, -1, owner, typegrade);
		SEND_Q(rmsg, dest);
		return;
	}

	sprintf(g_buf,
		"INSERT INTO t_pet (a_owner, a_enable, a_lastupdate, a_type) VALUES (%d, 2, NOW(), %d)",
		owner, typegrade);

	cmd.SetQuery(g_buf);
	if (cmd.Update())
	{
		int index = cmd.insertid();

		HelperPetCreateRepMsg(rmsg, index, owner, typegrade);
		SEND_Q(rmsg, dest);
	}
	else
	{
		HelperPetCreateRepMsg(rmsg, -1, owner, typegrade);
		SEND_Q(rmsg, dest);
	}
}

// TODO : DELETE
#ifdef NEW_UI
void do_CommandPetDeleteReq(CNetMsg& msg, CDescriptor* dest)
{
	int index;
	int owner;

	msg >> index
		>> owner;

	sprintf(g_buf,

		"DELETE FROM t_pet WHERE a_index=%d",

		index);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);
	if (cmd.Update())
	{
		CNetMsg rmsg;
		HelperPetDeleteRepMsg(rmsg, index, owner);
		SEND_Q(rmsg, dest);
	}
}
#endif // NEW_UI

void do_CommandGuildStashHistoryReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int guildindex = 0;
	int charindex = 0;
	int month[7] = {0, 0, 0, 0, 0, 0, 0};
	int day[7] = {0, 0, 0, 0, 0, 0, 0};
	LONGLONG money[7] = {0, 0, 0, 0, 0, 0, 0};

	msg >> guildindex
		>> charindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	// 7일보다 오래된 기록 삭제
	sprintf(g_buf, "DELETE FROM t_guild_tax_history WHERE a_guild_index = %d AND a_indate <= CAST(DATE_ADD(NOW(), INTERVAL -7 DAY) AS DATE)", guildindex);
	cmd.SetQuery(g_buf);
	cmd.Update();

	// 시간순으로 나스 입금액 얻기
	sprintf(g_buf, "SELECT MONTH(a_indate) AS mm, DAYOFMONTH(a_indate) AS dd, a_money FROM t_guild_tax_history WHERE a_guild_index = %d ORDER BY a_indate", guildindex);
	cmd.SetQuery(g_buf);
	if (cmd.Open() && cmd.m_nrecords)
	{
		int i = 0;
		while (cmd.MoveNext() && i < 7)
		{
			cmd.GetRec("mm",		month[i]);
			cmd.GetRec("dd",		day[i]);
			cmd.GetRec("a_money",	money[i]);
			i++;
		}
		HelperGuildStashHistoryRepMsg(rmsg, charindex, MSG_HELPER_GUILD_STASH_ERROR_OK, month, day, money);
	}
	else
	{
		// 없으면 없다고 오류
		HelperGuildStashHistoryRepMsg(rmsg, charindex, MSG_HELPER_GUILD_STASH_ERROR_NOHISTORY, month, day, money);
	}
	SEND_Q(rmsg, dest);
}

void do_CommandGuildStashViewReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int guildindex = 0;
	int charindex = 0;
	LONGLONG balance = 0;

	msg >> guildindex
		>> charindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	// 잔액 가져오기
	sprintf(g_buf, "SELECT a_balance FROM t_guild WHERE a_index = %d", guildindex);
	cmd.SetQuery(g_buf);
	if (cmd.Open() && cmd.MoveFirst())
	{
		cmd.GetRec("a_balance", balance);
	}

	HelperGuildStashViewRepMsg(rmsg, charindex, MSG_HELPER_GUILD_STASH_ERROR_OK, balance);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildStashTakeReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int guildindex = 0;
	int charindex = 0;
	LONGLONG money = 0;
	LONGLONG balance = 0;

	msg >> guildindex
		>> charindex
		>> money;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	if (money > 0)
	{
		// 찾기
#ifdef CIRCLE_WINDOWS
		sprintf(g_buf, "UPDATE t_guild SET a_balance = a_balance - %I64d WHERE a_index = %d AND a_balance >= %I64d", money, guildindex, money);
#else
		sprintf(g_buf, "UPDATE t_guild SET a_balance = a_balance - %lld WHERE a_index = %d AND a_balance >= %lld", money, guildindex, money);
#endif
		cmd.SetQuery(g_buf);
		if (!cmd.Update() || cmd.m_nrecords < 1)
		{
			HelperGuildStashTakeRepMsg(rmsg, guildindex, charindex, MSG_HELPER_GUILD_STASH_ERROR_NOTENOUGH, 0, 0);
			SEND_Q(rmsg, dest);
			return ;
		}
	}

	// 잔액 가져오기
	sprintf(g_buf, "SELECT a_balance FROM t_guild WHERE a_index = %d", guildindex);
	cmd.SetQuery(g_buf);
	if (cmd.Open() && cmd.MoveFirst())
	{
		cmd.GetRec("a_balance", balance);
	}

	HelperGuildStashTakeRepMsg(rmsg, guildindex, charindex, MSG_HELPER_GUILD_STASH_ERROR_OK, money, balance);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildStashRollback(CNetMsg& msg, CDescriptor* dest)
{
	int guildindex = 0;
	LONGLONG money = 0;

	msg >> guildindex
		>> money;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	if (money > 0)
	{
#ifdef CIRCLE_WINDOWS
		sprintf(g_buf, "UPDATE t_guild SET a_balance = a_balance + %I64d WHERE a_index = %d", money, guildindex);
#else
		sprintf(g_buf, "UPDATE t_guild SET a_balance = a_balance + %lld  WHERE a_index = %d", money, guildindex);
#endif
	}
	cmd.SetQuery(g_buf);
	cmd.Update();
}

void do_CommandGuildStashSaveTaxReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int guildindex = 0;
	int zoneindex = 0;
	LONGLONG taxItem = 0;
	LONGLONG taxProduce = 0;

	msg >> guildindex
		>> zoneindex
		>> taxItem
		>> taxProduce;

	if (guildindex < 1)
	{
		// 세금 받을 길드가 없으면 무조건 성공
		HelperGuildStashSaveTaxRepMsg(rmsg, MSG_HELPER_GUILD_STASH_ERROR_OK, guildindex, zoneindex, taxItem, taxProduce);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 최소 금액 설정
	LONGLONG realTaxItem = 1000000;
	LONGLONG realTaxProduce = 500000;

	if (realTaxItem < taxItem)
		realTaxItem = taxItem;
	if (realTaxProduce < taxProduce)
		realTaxProduce = taxProduce;

	LONGLONG totalTax = realTaxItem + realTaxProduce;

	if (totalTax < 1)
	{
		taxItem = 0;
		taxProduce = 0;
		HelperGuildStashSaveTaxRepMsg(rmsg, MSG_HELPER_GUILD_STASH_ERROR_OK, guildindex, zoneindex, taxItem, taxProduce);
		SEND_Q(rmsg, dest);
		return ;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	// 길드 테이블의 잔액 갱신
#ifdef CIRCLE_WINDOWS
	sprintf(g_buf, "UPDATE t_guild SET a_balance = a_balance + %I64d WHERE a_index = %d", totalTax, guildindex);
#else
	sprintf(g_buf, "UPDATE t_guild SET a_balance = a_balance + %lld  WHERE a_index = %d", totalTax, guildindex);
#endif
	cmd.SetQuery(g_buf);
	if (!cmd.Update() || cmd.m_nrecords < 1)
	{
		HelperGuildStashSaveTaxRepMsg(rmsg, MSG_HELPER_GUILD_STASH_ERROR_FAIL_DB, guildindex, zoneindex, taxItem, taxProduce);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 입금 내역 갱신
#ifdef KJTEST
	sprintf(g_buf, "select * from t_guild_tax_history where a_guild_index=%d and a_indate=CAST(NOW() AS DATE)", guildindex);
	cmd.SetQuery(g_buf);
	cmd.Open();

	if(cmd.m_nrecords < 1)
	{
#ifdef CIRCLE_WINDOWS
		sprintf(g_buf, "INSERT INTO t_guild_tax_history (a_guild_index, a_indate, a_money) VALUES (%d, CAST(NOW() AS DATE), %I64d)", guildindex, totalTax);
#else
		sprintf(g_buf, "INSERT INTO t_guild_tax_history (a_guild_index, a_indate, a_money) VALUES (%d, CAST(NOW() AS DATE), %lld )", guildindex, totalTax);
#endif
	}
	else
	{
#ifdef CIRCLE_WINDOWS
		sprintf(g_buf, "UPDATE t_guild_tax_history SET a_money = a_money + %I64d WHERE a_guild_index = %d AND a_indate = CAST(NOW() AS DATE)", totalTax, guildindex);
#else
		sprintf(g_buf, "UPDATE t_guild_tax_history SET a_money = a_money + %lld  WHERE a_guild_index = %d AND a_indate = CAST(NOW() AS DATE)", totalTax, guildindex);
#endif
	}

	cmd.SetQuery(g_buf);
	cmd.Update();
#else
#ifdef CIRCLE_WINDOWS
	sprintf(g_buf, "UPDATE t_guild_tax_history SET a_money = a_money + %I64d WHERE a_guild_index = %d AND a_indate = CAST(NOW() AS DATE)", totalTax, guildindex);
#else
	sprintf(g_buf, "UPDATE t_guild_tax_history SET a_money = a_money + %lld  WHERE a_guild_index = %d AND a_indate = CAST(NOW() AS DATE)", totalTax, guildindex);
#endif
	cmd.SetQuery(g_buf);
	if (!cmd.Update() || cmd.m_nrecords < 1)
	{
		// 오늘 날짜 입금 내역 갱신이 안되면 추가
#ifdef CIRCLE_WINDOWS
		sprintf(g_buf, "INSERT INTO t_guild_tax_history (a_guild_index, a_indate, a_money) VALUES (%d, CAST(NOW() AS DATE), %I64d)", guildindex, totalTax);
#else
		sprintf(g_buf, "INSERT INTO t_guild_tax_history (a_guild_index, a_indate, a_money) VALUES (%d, CAST(NOW() AS DATE), %lld )", guildindex, totalTax);
#endif
		cmd.SetQuery(g_buf);
		cmd.Update();
	}
#endif // KJTEST

	// 결과
	HelperGuildStashSaveTaxRepMsg(rmsg, MSG_HELPER_GUILD_STASH_ERROR_OK, guildindex, zoneindex, taxItem, taxProduce);
	SEND_Q(rmsg, dest);
}

//휴면케릭 이벤트 친구 등록부분 : GameServer에서 넘어온 부분.
void do_CommandEventSearchFriendMemberAddReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;
	
	int approval_index, approval_job, approval_user_index, approval_server_old;
	CLCString approval_nick(MAX_CHAR_NAME_LENGTH + 1);

	int request_index , request_user_index, request_datestamp;
	CLCString request_nick(MAX_CHAR_NAME_LENGTH + 1);

	msg >> approval_index
		>> approval_nick
		>> approval_job
		>> approval_user_index
		>> approval_server_old
		>> request_nick;
	
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	strcpy(g_buf, "SELECT a_index, a_user_index, a_nick, (UNIX_TIMESTAMP('2006-02-14 10:00:00') - UNIX_TIMESTAMP(a_datestamp)) as a_ctime FROM t_characters ");
	strcat(g_buf, "WHERE a_name =");	strcat(g_buf, "'"); IntCat(g_buf, approval_server_old, false);strcat(g_buf, request_nick); strcat(g_buf, "'");
	cmd.SetQuery(g_buf);

	//휴면 케릭 대상 서버군 케릭만 등록 할 수 있다. 
	//등록한 케릭명 검색 우선 서버군 케릭 확인.
	//만약 동일 케릭명일 경우 (서버군 명과 비교)
	if (!(cmd.Open() && cmd.MoveFirst()))
	{
		//기존 서버군이 아닌 휴면케릭일 경우
		strcpy(g_buf, "SELECT a_index, a_user_index, a_nick, (UNIX_TIMESTAMP('2006-02-14 10:00:00') - UNIX_TIMESTAMP(a_datestamp)) as a_ctime FROM t_characters ");
		strcat(g_buf, "WHERE a_nick =");	strcat(g_buf, "'"); strcat(g_buf, request_nick); strcat(g_buf, "'");
		cmd.SetQuery(g_buf);

		if (!(cmd.Open() && cmd.MoveFirst()))
		{
			// 등록 신청한 친구가 없습니다.
			HelperEventSearchFriendMemberAddRepMsg(rmsg
				, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_NOT_EXIST);
			SEND_Q(rmsg, dest);

			GAMELOG << init("EVENT SEARCH FRIEND _ERROR_NOT_EXIST")
					<< approval_nick << delim
					<< request_nick << delim
					<< end;
			return;
		}
	}
	//등록한 케릭명 검색 휴면케릭 자료 
	cmd.GetRec("a_user_index", request_user_index);
	cmd.GetRec("a_index", request_index);
	cmd.GetRec("a_nick", request_nick);
	cmd.GetRec("a_ctime", request_datestamp);

	sprintf(g_buf, "SELECT (UNIX_TIMESTAMP('2006-02-14 10:00:00') - UNIX_TIMESTAMP(a_datestamp)) as a_ctime "
				   "FROM t_event_searchfriend_date where a_index = %d", request_index);
	cmd.SetQuery(g_buf);
	if (!(cmd.Open() && cmd.MoveFirst()))
	{
		//금요일 10일과 14일 사이에 생서된 케릭들일 경우....
		if ( request_datestamp < 2678400)
		{
			HelperEventSearchFriendMemberAddRepMsg(rmsg
				, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);

			GAMELOG << init("EVENT SEARCH FRIEND ERROR_GAMESERVER ")
					<< approval_nick << delim
					<< request_nick << delim
					<< request_datestamp << delim
					<< end;
			return ;
		}
		else
		{   //혹시라도 t_event_searchfriend_date에서 전 날짜를 불러오지 못했을 경우..예외.
			GAMELOG << init("EVENT SEARCH FRIEND ERROR_GAMESERVER ")
					<< approval_nick << delim
					<< request_nick << delim
					<< request_datestamp << delim
					<< end;
			return ;
		}
	}
	else
		cmd.GetRec("a_ctime", request_datestamp);

	//휴면 케릭이 1개월 미만(31일 미만일 경우) 등록 불가.
	if ( request_datestamp < 2678400)
	{
		HelperEventSearchFriendMemberAddRepMsg(rmsg
			, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND ERROR_GAMESERVER ")
				<< approval_nick << delim
				<< request_nick << delim
				<< request_datestamp << delim
				<< end;
		return ;
	}

	//접속자가 같은 ID이면 불가능 함.
	if(request_user_index == approval_user_index)
	{
		HelperEventSearchFriendMemberAddRepMsg(rmsg
			, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_NOT_MATCH_INDEX);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND ERROR NOT MATCH INDEX")
				<< approval_nick << delim
				<< request_nick << delim
				<< approval_user_index << delim
				<< end;
		return;
	}
	//이미 등록이 되어 있는지..
	sprintf(g_buf
		, "SELECT count(a_char_index) AS nCount FROM t_event_searchfriend where a_dormant_index = %d and (a_char_index = %d or a_dormant_select = 1)"
		, request_index, approval_index);
	cmd.SetQuery(g_buf);

	int nCount = 0;
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		GAMELOG << init("EVENT SEARCH FRIEND ERROR ALREADY EXIST FAIL")
				<< approval_nick << delim
				<< request_nick << delim
				<< end;
		return;
	}

	cmd.GetRec("nCount", nCount);

	if(nCount >= 1 )  
	{
		HelperEventSearchFriendMemberAddRepMsg(rmsg
			, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_ALREADY_EXIST);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND ERROR ALREADY EXIST")
				<< approval_nick << delim
				<< request_nick << delim				
				<< end;
		return;
	}

	//등록 케릭 50명 제한
	sprintf(g_buf
		, "SELECT count(a_char_index) AS nCount FROM t_event_searchfriend where a_char_index = %d", approval_index);
	cmd.SetQuery(g_buf);

	nCount = 0;
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		GAMELOG << init("EVENT SEARCH FRIEND ERROR FULLMEMBER FAIL")
				<< approval_nick << delim
				<< request_nick << delim
				<< end;
		return;
	}

	if( !cmd.GetRec("nCount", nCount) || nCount >= 50 )  
	{
		HelperEventSearchFriendMemberAddRepMsg(rmsg
			, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_FULLMEMBER);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND ERROR FULLMEMBER")
				<< approval_nick << delim
				<< request_nick << delim
				<< nCount << delim
				<< end;
		return;
	}

	//DB에 접속해서 처리.. 
	//먼저 친구등록 요청한 캐릭에 친구등록
	sprintf(g_buf, "INSERT INTO t_event_searchfriend (a_char_index, a_char_nick, a_char_good"
				",a_dormant_index,a_dormant_name,a_dormant_nick"
				",a_dormant_total_time, a_dormant_good) VALUES (%d,'%s',%d, %d,'%s','%s',%d, %d)"
				,approval_index, (const char*)approval_nick, -1, request_index, (const char*)request_nick, (const char*)request_nick, 0, -1);
	
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		HelperEventSearchFriendMemberAddRepMsg(rmsg
			, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_PACKET);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND ERROR PACKET")
				<< approval_nick << delim
				<< request_nick << delim
				<< end;
		return ;
	}

	HelperEventSearchFriendMemberAddRepMsg(rmsg
		, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_OK);
	SEND_Q(rmsg, dest);

	GAMELOG << init("EVENT SEARCH FRIEND OK")
			<< approval_nick << delim
			<< request_nick << delim
			<< end;

}

//휴면케릭 이벤트 등록 설정
void do_CommandEventSearchFriendMemberSelectAddReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;
	
	int approval_index;
	int request_index;

	msg >> approval_index
		>> request_index;
	
	//DB에 접속해서 처리.. 
	//먼저 친구등록 요청한 캐릭에 친구등록
	sprintf(g_buf, "UPDATE t_event_searchfriend SET a_dormant_select = 1, a_char_good = 0, a_dormant_good = 0 "
				   "WHERE a_dormant_index= %d and a_char_index = %d"
				   ,approval_index, request_index);
	
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.BeginTrans();
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		HelperEventSearchFriendMemberSelectAddRepMsg(rmsg
			, approval_index, MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND")
				<< "ERROR HELPER DB " << delim 
				<< "SELECT ADD CHAR" << delim
				<< approval_index << delim << request_index
				<< end;
		return ;
	}

	//휴면 이벤트 시간
	char	te_buf[1024];	// for temp string
	struct tm ti = NOW();
	te_buf[0] = '\0';
	IntCat0(te_buf, ti.tm_year + 1900, 4, false);
	strcat(te_buf, "/");
	IntCat0(te_buf, ti.tm_mon + 1, 2, false);
	strcat(te_buf, "/");
	IntCat0(te_buf, ti.tm_mday, 2, false);
	strcat(te_buf, " ");
	IntCat0(te_buf, ti.tm_hour, 2, false);
	strcat(te_buf, ":");
	IntCat0(te_buf, ti.tm_min, 2, false);
	strcat(te_buf, ":");
	IntCat0(te_buf, ti.tm_sec, 2, false);

	strcpy(g_buf, "UPDATE t_event_searchfriend_date SET a_datestamp='");
	strcat(g_buf, te_buf);
	strcat(g_buf, "' WHERE a_index=");
	IntCat(g_buf, approval_index, false);

	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		HelperEventSearchFriendMemberSelectAddRepMsg(rmsg
			, approval_index, MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND")
				<< "ERROR HELPER DB Time " << delim 
				<< "SELECT ADD CHAR" << delim
				<< approval_index << delim << request_index
				<< end;
		return;
	}

	sprintf(g_buf, "DELETE FROM t_event_searchfriend "
				   "WHERE a_dormant_index= %d and a_dormant_select = 0",approval_index);
	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		cmd.Rollback();
		HelperEventSearchFriendMemberSelectAddRepMsg(rmsg
			, approval_index, MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND")
				<< "ERROR HELPER DB_delete " << delim 
				<< "SELECT ADD CHAR" << delim
				<< approval_index << delim << request_index
				<< end;
		return;
	}
	else
	{
		cmd.Commit();
	}


	HelperEventSearchFriendMemberSelectAddRepMsg(rmsg
		, approval_index, MSG_EVENT_SEARCHFRIEND_ERROR_OK);
	SEND_Q(rmsg, dest);
}

//휴면케릭 사냥시간 한시간 단위로 업데이터 하는 부분
void do_CommandEventSearchFriendMemberOneTimeCheckReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;
	
	int	timesec, appDormantindex;

	msg >> timesec
		>> appDormantindex;

	//DB에 접속해서 처리..
	
	if(timesec == (60 * 60 * 30)) //30시간일때만 
	{
		//30시간 채울 경우 아이템 보상 1점씩 올려줌.
		sprintf(g_buf, "UPDATE t_event_searchfriend SET a_dormant_total_time=%d"
					   ", a_char_good = 1, a_dormant_good = 2 WHERE a_dormant_index=%d and a_dormant_select = 1"
					   , timesec, appDormantindex);
	}
	else
	{			
		//1시간당 체크 업데이터
		sprintf(g_buf, "UPDATE t_event_searchfriend SET a_dormant_total_time=%d "
					   "WHERE a_dormant_index=%d and a_dormant_select = 1", timesec, appDormantindex);
	}
	
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(g_buf);

	if(!cmd.Update())
	{		
		GAMELOG << init("EVENT SEARCH FRIEND") 
				<< "ERROR HELPER DB " << appDormantindex << delim 
				<< "ONE TIME CHECK" << timesec 
				<< end;
		return;
	}
	
	GAMELOG << init("EVENT SEARCH FRIEND")
			<< "ONE TIME CHECK " << delim
			<< timesec<< delim
			<< appDormantindex
			<< end;

	HelperEventSearchFriendMemberOneTimeCheckRepMsg(rmsg,timesec, appDormantindex, MSG_EVENT_SEARCHFRIEND_ERROR_OK);
	SEND_Q(rmsg, dest);
}

//휴면케릭 이벤트 등록된 친구 보상가능 여부
void do_CommandEventSearchFriendMemberListGoodReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;	
	int approval_index;
	msg >> approval_index;
	
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	sprintf(g_buf
		, "SELECT * FROM t_event_searchfriend WHERE (a_dormant_index = %d and a_dormant_select = 1) or a_char_index = %d", approval_index, approval_index);
	cmd.SetQuery(g_buf);

	int ntemp = 0;
	int request_index[50];
	int item_good[50];
	char request_nick[50][MAX_CHAR_NAME_LENGTH + 1];

	if( cmd.Open() && cmd.MoveFirst())
	{
		CLCString rnick(MAX_CHAR_NAME_LENGTH + 1);
		do
		{
			cmd.GetRec("a_dormant_index", request_index[ntemp]);
			cmd.GetRec("a_dormant_nick", rnick);
			strcpy(request_nick[ntemp], rnick);

			if(request_index[ntemp] == approval_index)
				cmd.GetRec("a_dormant_good", item_good[ntemp]);
			else
				//이벤트 사냥에서 얻은 아이템 개수, 본인케릭과 등록해준 케릭의 아이템 얻은 양을 알아야된다.
				cmd.GetRec("a_char_good", item_good[ntemp]);
			ntemp++;
		}while (cmd.MoveNext());
	}

	if (ntemp <= 20)
	{
		HelperEventSearchFriendMemberListGoodRepMsg(rmsg, approval_index, request_index, request_nick, item_good, ntemp, 0, ntemp);
		SEND_Q(rmsg, dest);
	}
	else
	{
		HelperEventSearchFriendMemberListGoodRepMsg(rmsg, approval_index, request_index, request_nick, item_good, 20, 0, ntemp);
		SEND_Q(rmsg, dest);
		if (ntemp <= 40)
		{
			HelperEventSearchFriendMemberListGoodRepMsg(rmsg, approval_index, request_index + 20, request_nick + 20, item_good + 20, ntemp - 20, 20, ntemp);
			SEND_Q(rmsg, dest);
		}
		else
		{
			HelperEventSearchFriendMemberListGoodRepMsg(rmsg, approval_index, request_index + 20, request_nick + 20, item_good + 20, 20, 20, ntemp);
			SEND_Q(rmsg, dest);
			HelperEventSearchFriendMemberListGoodRepMsg(rmsg, approval_index, request_index + 40, request_nick + 40, item_good + 40, ntemp - 40, 40, ntemp);
			SEND_Q(rmsg, dest);
		}
	}
}


//휴면케릭 이벤트 등록된 친구 보상 결과 
void do_CommandEventSearchFriendMemberGoodReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;	
	int approval_index, ndormantindex, item_good;
	msg >> approval_index
		>> ndormantindex
		>> item_good;
	
	//DB에 접속해서 처리.. 
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.BeginTrans();
	if(ndormantindex == approval_index)
		sprintf(g_buf, "UPDATE t_event_searchfriend SET a_dormant_good = 3 WHERE a_dormant_index=%d and a_dormant_select = 1"
					   , approval_index);
	else
		sprintf(g_buf, "UPDATE t_event_searchfriend SET a_char_good = 3 WHERE a_dormant_index=%d and a_dormant_select = 1"
					   , ndormantindex);	

	cmd.SetQuery(g_buf);
	if (!cmd.Update())
	{
		cmd.Rollback();
		GAMELOG << init("EVENT SEARCH FRIEND")
				<< "ERROR HELPER DB " << delim 
				<< "ITEM" << item_good << delim
				<< approval_index << delim << ndormantindex
				<< end;
		HelperEventSearchFriendMemberGoodRepMsg(rmsg, approval_index, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_HELPER);
		SEND_Q(rmsg, dest);
		return;
	}
	else
		cmd.Commit();

	HelperEventSearchFriendMemberGoodRepMsg(rmsg, approval_index, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_OK);
	SEND_Q(rmsg, dest);
}

void do_CommandPartyInviteReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nBossLevel;
	int			nTargetIndex;
	char		nPartyType;

	msg >> nBossIndex
		>> strBossName
		>> nBossLevel
		>> nTargetIndex
		>> nPartyType;

	HelperPartyInviteReqMsg(rmsg, nBossIndex, strBossName, nBossLevel, nTargetIndex, nPartyType);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}

void do_CommandPartyInviteRep(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);
	char		nPartyType;
	int			nErrorCode;

	msg >> nBossIndex
		>> strBossName
		>> nTargetIndex
		>> strTargetName
		>> nPartyType
		>> nErrorCode;

	if (nErrorCode == MSG_HELPER_PARTY_ERROR_INVITE_OK)
	{
		CParty* pPartyBoss = gserver.FindPartyByMemberIndex(dest->m_subNo, nBossIndex, true);
		CParty* pPartyTarget = gserver.FindPartyByMemberIndex(dest->m_subNo, nTargetIndex, true);

		// nBossIndex가 파티 초대가 가능한지 : 파티가 없거나 보스
		if (pPartyBoss && pPartyBoss->GetBossIndex() != nBossIndex)
		{
			HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_ME);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
			return ;
		}

		// 파티 타입 검사
		if (pPartyBoss && pPartyBoss->GetPartyType(MSG_DIVITYPE_EXP) != nPartyType)
		{
			HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_DIFF_TYPE);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
			return ;
		}

		// 대상 파티 검사
		if (pPartyTarget)
		{
			HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_TARGET);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
			return ;
		}

		// 파티 인원 검사
		if (pPartyBoss && pPartyBoss->GetMemberCount() >= MAX_PARTY_MEMBER)
		{
			HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_FULL);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
			return ;
		}

		// 현재 요청자 유무 검사
		if (pPartyBoss && pPartyBoss->GetRequestIndex() > 0)
		{
			HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_REQUEST);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
			return ;
		}

		if (pPartyBoss)
		{
			// 기존 파티에 요청자로 설정
			pPartyBoss->SetRequest(nTargetIndex, strTargetName);
		}
		else
		{
			// 신규 파티 생성
			pPartyBoss = new CParty(dest->m_subNo, nPartyType, nBossIndex, strBossName, nTargetIndex, strTargetName);
			gserver.m_listParty.AddToTail(pPartyBoss);
		}

		// 결과 전달
		HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_OK);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
	}
	else
	{
		// 오류는 에코
		HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, (MSG_HELPER_PARTY_ERROR_TYPE)nErrorCode);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
	}
}

void do_CommandPartyAllowReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int			nBossIndex;
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> nBossIndex
		>> nTargetIndex
		>> strTargetName;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);

	if (!pParty)
	{
		HelperPartyAllowRepMsg(rmsg, nBossIndex, nTargetIndex, strTargetName, MSG_HELPER_PARTY_ERROR_ALLOW_NOT_PARTY);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	if (pParty->GetRequestIndex() != nTargetIndex)
	{
		HelperPartyAllowRepMsg(rmsg, nBossIndex, nTargetIndex, strTargetName, MSG_HELPER_PARTY_ERROR_ALLOW_NOT_PARTY);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	// 파티원으로 등록
	pParty->JoinRequest(strTargetName);

	HelperPartyAllowRepMsg(rmsg, nBossIndex, nTargetIndex, strTargetName, MSG_HELPER_PARTY_ERROR_ALLOW_OK);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);

#ifdef PARTY_MATCHING
	// 파티에 참가하는 유저는 매칭 테이블에서 제거
	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(dest->m_subNo, nTargetIndex);
	if (pMatchMember)
		do_CommandPartyMatchDelReq_real(dest->m_subNo, nTargetIndex);

	// 풀 파티는 매칭 테이블에서 제거
	if (pParty->GetMemberCount() >= MAX_PARTY_MEMBER)
		do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
#endif // PARTY_MATCHING
}

void do_CommandPartyRejectReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int			nBossIndex;
	int			nTargetIndex;

	msg >> nBossIndex
		>> nTargetIndex;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
	{
		HelperPartyRejectRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_REJECT_FAIL);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	if (pParty->GetRequestIndex() < 1)
	{
		HelperPartyRejectRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_REJECT_FAIL);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	// 요청자가 보스인지 일반인지 검사
	if (pParty->GetBossIndex() == nTargetIndex || pParty->GetRequestIndex() == nTargetIndex)
	{
		HelperPartyRejectRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_REJECT_OK);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);

		pParty->SetRequest(-1, "");
		if (pParty->GetMemberCount() < 2)
		{
			// 파티 종료
			gserver.m_listParty.RemoveData(pParty);
			delete pParty;
		}
	}
	else
	{
		HelperPartyRejectRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_REJECT_FAIL);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}
}

void do_CommandPartyQuitReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int			nBossIndex;
	int			nTargetIndex;

	msg >> nBossIndex
		>> nTargetIndex;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
		return ;

	pParty->DeleteMember(nTargetIndex);

	if (pParty->GetMemberCount() < 2)
	{
		// 파티 해체
		gserver.m_listParty.RemoveData(pParty);
		delete pParty;
		HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_END, -1, "");
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);

#ifdef PARTY_MATCHING
		// 제거 파티는 매칭 테이블에서 제거
		do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
#endif // PARTY_MATCHING
	}
	else
	{
		// 보스 변경 검사
		if (pParty->GetBossIndex() != nBossIndex)
		{
#ifdef PARTY_MATCHING
			CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
			if (pMatchParty)
			{
				pMatchParty->SetBossIndex(pParty->GetBossIndex());
				pMatchParty->SetBossName(pParty->GetMemberByListIndex(0)->GetCharName());
			}
#endif // PARTY_MATCHING
			HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_OK, pParty->GetBossIndex(), pParty->GetMemberByListIndex(0)->GetCharName());
		}
		else
		{
			HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_OK, nBossIndex, pParty->GetMemberByListIndex(0)->GetCharName());
		}
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
	}
}

void do_CommandPartyKickReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int			nBossIndex;
	int			nTargetIndex;

	msg >> nBossIndex
		>> nTargetIndex;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
		return ;

	pParty->DeleteMember(nTargetIndex);

	if (pParty->GetMemberCount() < 2)
	{
		// 파티 해체
		gserver.m_listParty.RemoveData(pParty);
		delete pParty;
		HelperPartyKickRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_KICK_END);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);

#ifdef PARTY_MATCHING
		// 제거 파티는 매칭 테이블에서 제거
		do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
#endif // PARTY_MATCHING
	}
	else
	{
		HelperPartyKickRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_KICK_OK);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
	}
}

void do_CommandPartyChangeBossReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> nBossIndex
		>> strTargetName;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
		return ;

	strBossName = pParty->GetMemberByListIndex(0)->GetCharName();

	if (pParty->ChangeBoss(strTargetName))
	{
		HelperPartyChangeBossRepMsg(rmsg, nBossIndex, strBossName, pParty->GetBossIndex(), strTargetName);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);

#ifdef PARTY_MATCHING
		CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
		if (pMatchParty)
		{
			pMatchParty->SetBossIndex(pParty->GetBossIndex());
			pMatchParty->SetBossName(strTargetName);
		}
#endif // PARTY_MATCHING
	}
}

#ifdef NEW_UI
void do_CommandPartyChangeTypeReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;

	int nBossIndex;
	char cPartyType,cDiviType=MSG_DIVITYPE_EXP;

	msg >> nBossIndex
		>> cPartyType
		>> cDiviType;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
		return ;

	pParty->SetPartyType(cDiviType,cPartyType);

	HelperPartyChangeTypeRepMsg(rmsg, nBossIndex, cPartyType, cDiviType);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}
#endif // NEW_UI

#ifdef PARTY_RAID
void do_CommandPartyEndPartyReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;
	int nBossIndex;

	msg >> nBossIndex;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
		return ;

	CPartyMember* pMember=NULL;
	for(int i=0; i < MAX_PARTY_MEMBER; i++)
	{
		pMember = (CPartyMember*)pParty->GetMemberByListIndex(i);

		if(pMember)
		{
			pParty->DeleteMember(pMember->GetCharIndex());
		}
	}

	if(pParty->GetMemberCount() <= 0)
	{
		// 파티 해체
		gserver.m_listParty.RemoveData(pParty);
		delete pParty;


		HelperPartyEndPartyRepMsg(rmsg, nBossIndex);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
	}
}
#endif // PARTY_RAID

#ifdef PARTY_MATCHING
void do_CommandPartyMatchRegMemberReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg			rmsg;

	int				nCharIndex;
	CLCString		strCharName(MAX_CHAR_NAME_LENGTH + 1);
	int				nCharLevel;
	int				nZone;
	char			cJob;
	char			cPartyType;

	msg >> nCharIndex
		>> strCharName
		>> nCharLevel
		>> nZone
		>> cJob
		>> cPartyType;

	CParty* pParty = gserver.FindPartyByMemberIndex(dest->m_subNo, nCharIndex, true);
	if (pParty)
	{
		HelperPartyMatchRegMemberRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_PARTY, nCharIndex, strCharName, nCharLevel, nZone, cJob, cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(dest->m_subNo, nCharIndex);
	if (pMatchMember)
	{
		HelperPartyMatchRegMemberRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_REG, nCharIndex, strCharName, nCharLevel, nZone, cJob, cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	pMatchMember = new CPartyMatchMember(dest->m_subNo, nCharIndex, strCharName, nCharLevel, nZone, cJob, cPartyType);
	gserver.m_listPartyMatchMember.AddToHead(pMatchMember);

	HelperPartyMatchRegMemberRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_OK, nCharIndex, strCharName, nCharLevel, nZone, cJob, cPartyType);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}

void do_CommandPartyMatchRegPartyReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg			rmsg;

	int				nBossIndex;
	CLCString		strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int				nBossLevel;
	int				nZone;
	int				nJobFlag;
	char			cLimitLevel;
	CLCString		strComment(PARTY_MATCHING_COMMENT + 1);

	msg >> nBossIndex
		>> strBossName
		>> nBossLevel
		>> nZone
		>> nJobFlag
		>> cLimitLevel
		>> strComment;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty || pParty->GetMemberCount() < 2)
	{
		HelperPartyMatchRegPartyRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_NO_BOSS, nBossIndex, strBossName, nBossLevel, nZone, nJobFlag, cLimitLevel, 0, strComment);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (pParty->GetMemberCount() == MAX_PARTY_MEMBER)
	{
		HelperPartyMatchRegPartyRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_FULL, nBossIndex, strBossName, nBossLevel, nZone, nJobFlag, cLimitLevel, 0, strComment);
		SEND_Q(rmsg, dest);
		return ;
	}

	CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (pMatchParty)
	{
		HelperPartyMatchRegPartyRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_ALREADY_REG, nBossIndex, strBossName, nBossLevel, nZone, nJobFlag, cLimitLevel, 0, strComment);
		SEND_Q(rmsg, dest);
		return ;
	}

	pMatchParty = new CPartyMatchParty(dest->m_subNo, nBossIndex, strBossName, nBossLevel, nZone, nJobFlag, pParty->GetPartyType(MSG_DIVITYPE_EXP), (cLimitLevel == 1) ? true : false, strComment);
	gserver.m_listPartyMatchParty.AddToHead(pMatchParty);

	HelperPartyMatchRegPartyRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_OK, nBossIndex, strBossName, nBossLevel, nZone, nJobFlag, cLimitLevel, pParty->GetPartyType(MSG_DIVITYPE_EXP), strComment);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}

void do_CommandPartyMatchDelReq(CNetMsg& msg, CDescriptor* dest)
{
	int				nCharIndex;
	msg >> nCharIndex;
	do_CommandPartyMatchDelReq_real(dest->m_subNo, nCharIndex);
}

void do_CommandPartyMatchDelReq_real(int nSubNo, int nCharIndex)
{
	CNetMsg			rmsg;

	// 멤버 리스트에서 찾기
	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(nSubNo, nCharIndex);
	if (pMatchMember)
	{
		gserver.m_listPartyMatchMember.RemoveData(pMatchMember);
		delete pMatchMember;
		HelperPartyMatchDelRepMsg(rmsg, nCharIndex, MSG_HELPER_PARTY_MATCH_ERROR_DEL_MEMBER);
		gserver.SendToAllSubServer(rmsg, nSubNo);
		return ;
	}

	// 파티 리스트에서 찾기
	CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(nSubNo, nCharIndex);
	if (pMatchParty)
	{
		gserver.m_listPartyMatchParty.RemoveData(pMatchParty);
		delete pMatchParty;
		HelperPartyMatchDelRepMsg(rmsg, nCharIndex, MSG_HELPER_PARTY_MATCH_ERROR_DEL_PARTY);
		gserver.SendToAllSubServer(rmsg, nSubNo);
		return ;
	}
}

void do_CommandPartyMatchInviteReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg		rmsg;

	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nBossLevel;
	int			nCharIndex;

	msg >> nBossIndex
		>> strBossName
		>> nBossLevel
		>> nCharIndex;

	// 대상의 파티 매칭 등록을 찾는다
	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(dest->m_subNo, nCharIndex);
	if (!pMatchMember)
	{
		HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_NOT_FOUND, nBossIndex, strBossName, nCharIndex, "", 0);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 대상자의 희망 파티 타입을 구한다
	char		cPartyType = pMatchMember->GetPartyType();

	// 파티 타입이 PARTY_TYPE_BATTLE이면 tch와 ch의 레벨 차이를 검사
	// 차이가 +- 10이상이면 ERROR;
	if (cPartyType == PARTY_TYPE_BATTLE)
	{
		if (ABS(pMatchMember->GetLevel() - nBossLevel) > 10)
		{
			HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_LEVEL, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
			SEND_Q(rmsg, dest);
			return ;
		}
	}

	// 기존 파티를 찾는다
	CParty* pPartyBoss = gserver.FindPartyByMemberIndex(dest->m_subNo, nBossIndex, true);
	CParty* pPartyTarget = gserver.FindPartyByMemberIndex(dest->m_subNo, nCharIndex, true);

	// nBossIndex가 파티 초대가 가능한지 : 파티가 없거나 보스
	if (pPartyBoss && pPartyBoss->GetBossIndex() != nBossIndex)
	{
		HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_NOBOSS, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 파티 타입 검사
	if (pPartyBoss && pPartyBoss->GetPartyType(MSG_DIVITYPE_EXP) != cPartyType)
	{
		HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_DIFF_TYPE, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 대상 파티 검사
	if (pPartyTarget)
	{
		HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_ALREADY_TARGET, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 파티 인원 검사
	if (pPartyBoss && pPartyBoss->GetMemberCount() >= MAX_PARTY_MEMBER)
	{
		HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_FULL, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 현재 요청자 유무 검사
	if (pPartyBoss && pPartyBoss->GetRequestIndex() > 0)
	{
		HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_ALREADY_REQUEST, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (pPartyBoss)
	{
		// 기존 파티에 요청자로 설정
		pPartyBoss->SetRequest(nCharIndex, pMatchMember->GetName());
	}
	else
	{
		// 신규 파티 생성
		pPartyBoss = new CParty(dest->m_subNo, cPartyType, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName());
		gserver.m_listParty.AddToTail(pPartyBoss);
	}

	// 결과 전달
	HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_OK, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}

void do_CommandPartyMatchJoinReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg		rmsg;

	int			nBossIndex;
	int			nCharIndex;
	CLCString	strCharName(MAX_CHAR_NAME_LENGTH + 1);
	int			nCharLevel;
	char		cCharJob;

	msg >> nBossIndex
		>> nCharIndex
		>> strCharName
		>> nCharLevel
		>> cCharJob;

	// 파티 매칭에 등록된 것을 찾고
	CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (pMatchParty == NULL)
	{
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_NOT_FOUND, 0, nBossIndex, "", nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 파티를 찾고
	CParty*		pPartyBoss = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (pPartyBoss == NULL)
	{
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_NOT_FOUND, 0, nBossIndex, "", nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 파티 타입을 구하고
	char cPartyType = pPartyBoss->GetPartyType(MSG_DIVITYPE_EXP);

	// 파티 타입이 PARTY_TYPE_BATTLE이면 레벨 검사
	if (cPartyType == PARTY_TYPE_BATTLE)
	{
		if (ABS(pMatchParty->GetBossLevel() - nCharLevel) > 10)
		{
			HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_LEVEL, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
			SEND_Q(rmsg, dest);
			return ;
		}
	}

	// 파티 매칭에 레벨 제한이 있으면 레벨 검사
	if (pMatchParty->IsLimitLevel() && ABS(pMatchParty->GetBossLevel() - nCharLevel) > PARTY_MATCHING_LIMIT_LEVEL)
	{
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_LIMIT_LEVEL, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 파티 매칭에 직업 제한이 있으면 직업 검사
	if ((pMatchParty->GetJobFlag() & (1 << cCharJob)) == 0)
	{
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_JOB, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 파티 인원 검사
	if (pPartyBoss->GetMemberCount() >= MAX_PARTY_MEMBER)
	{
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_FULL, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 현재 요청자 검사
	if (pPartyBoss->GetRequestIndex() > 0)
	{
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALREADY_REQUEST, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 요청한 사람의 파티 검사
	CParty*		pPartyTarget = gserver.FindPartyByMemberIndex(dest->m_subNo, nCharIndex, true);
	if (pPartyTarget)
	{
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALREADY_TARGET, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 기존 파티에 요청자로 설정
	pPartyBoss->SetRequest(nCharIndex, strCharName);

	// 결과 전달
	HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_OK, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}

void do_CommandPartyMatchJoinAllowReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg		rmsg;

	int			nBossIndex;
	int			nReqCharIndex;

	msg >> nBossIndex
		>> nReqCharIndex;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);

	if (!pParty)
	{
		HelperPartyMatchJoinAllowRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALLOW_NOT_PARTY, nBossIndex, nReqCharIndex, "");
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	if (pParty->GetRequestIndex() != nReqCharIndex)
	{
		HelperPartyMatchJoinAllowRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALLOW_NOT_PARTY, nBossIndex, nReqCharIndex, "");
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	// 파티원으로 등록
	CLCString strReqName(MAX_CHAR_NAME_LENGTH + 1);
	strReqName = pParty->GetRequestName();
	pParty->JoinRequest(strReqName);

	HelperPartyMatchJoinAllowRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALLOW_OK, nBossIndex, nReqCharIndex, strReqName);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);

	// 파티에 참가하는 유저는 매칭 테이블에서 제거
	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(dest->m_subNo, nReqCharIndex);
	if (pMatchMember)
		do_CommandPartyMatchDelReq_real(dest->m_subNo, nReqCharIndex);

	// 풀 파티는 매칭 테이블에서 제거
	if (pParty->GetMemberCount() >= MAX_PARTY_MEMBER)
		do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
}

void do_CommandPartyMatchJoinRejectReq(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg		rmsg;

	int			nJoinCharIndex;
	int			nRejectCharIndex;

	msg >> nJoinCharIndex
		>> nRejectCharIndex;

	// nJoinCharIndex로 파티 검색
	CParty* pParty = gserver.FindPartyByMemberIndex(dest->m_subNo, nJoinCharIndex, true);
	if (!pParty)
	{
		HelperPartyMatchJoinRejectRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_REJECT_FAIL, nJoinCharIndex, nRejectCharIndex);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	// 요청자가 보스인지 일반인지 검사
	if (pParty->GetBossIndex() == nRejectCharIndex || pParty->GetRequestIndex() == nRejectCharIndex)
	{
		HelperPartyMatchJoinRejectRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_REJECT_OK, nJoinCharIndex, nRejectCharIndex);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);

		pParty->SetRequest(-1, "");
		if (pParty->GetMemberCount() < 2)
		{
			// 제거 파티는 매칭 테이블에서 제거
			do_CommandPartyMatchDelReq_real(dest->m_subNo, pParty->GetBossIndex());

			// 파티 종료
			gserver.m_listParty.RemoveData(pParty);
			delete pParty;
		}
	}
	else
	{
		HelperPartyMatchJoinRejectRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_REJECT_FAIL, nJoinCharIndex, nRejectCharIndex);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}
}

void do_CommandPartyMatchMemberChangeInfoName(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg		rmsg;

	int			nCharIndex;
	int			nType;
	CLCString	strCharName(MAX_CHAR_NAME_LENGTH + 1);
	int			nLevel;
	int			nZone;

	msg >> nCharIndex
		>> nType;

	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(dest->m_subNo, nCharIndex);
	CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nCharIndex);

	switch (nType)
	{
	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME:
		msg >> strCharName;
		if (pMatchMember)		pMatchMember->SetName(strCharName);
		if (pMatchParty)		pMatchParty->SetBossName(strCharName);
		if (pMatchMember || pMatchParty)
		{
			HelperPartyMatchMemberChangeInfoMsg(rmsg, nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME, strCharName, 0, 0);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		}
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL:
		msg >> nLevel;
		if (pMatchMember)		pMatchMember->SetLevel(nLevel);
		if (pMatchParty)		pMatchParty->SetBossLevel(nLevel);
		if (pMatchMember || pMatchParty)
		{
			HelperPartyMatchMemberChangeInfoMsg(rmsg, nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL, "", nLevel, 0);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		}
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE:
		msg >> nLevel
			>> nZone;
		if (pMatchMember)
		{
			pMatchMember->SetLevel(nLevel);
			pMatchMember->SetZone(nZone);
		}
		if (pMatchParty)
		{
			pMatchParty->SetBossLevel(nLevel);
			pMatchParty->SetZone(nZone);
		}
		if (pMatchMember || pMatchParty)
		{
			HelperPartyMatchMemberChangeInfoMsg(rmsg, nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE, "", nLevel, nZone);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		}
		break;
	}
}

#endif // PARTY_MATCHING

#ifdef LC_HBK
void do_LevelUpLog(CNetMsg & msg, CDescriptor * dest)
{
	CLCString chrname(MAX_CHAR_NAME_LENGTH + 1);
	int level;
	msg >> chrname
		>> level;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	CLCString sql(256);
	sql.Format("insert into t_levelup_log values('%s', %d, NOW())", (const char * )chrname, level);
	cmd.SetQuery(sql);
	cmd.Update();
}
#endif // LC_HBK_ENG

#ifdef GUILD_RANKING
void do_CommandSaveExpGuild(CNetMsg& msg, CDescriptor* dest)
{
	int			nGuildIndex;
	double		dGuildExp;

	msg >> nGuildIndex
		>> dGuildExp;

	CGuild* pGuild = gserver.m_guildlist.findguild(nGuildIndex);
	if (pGuild)
	{
		GAMELOG << init("GUILD RANKING SAVE EXP")
				<< "GUILD"			<< delim
				<< nGuildIndex		<< delim
				<< pGuild->name()	<< delim
				<< "EXP"			<< delim
				<< (float)dGuildExp
				<< end;

		bool bNew = true;		// 새로 INSERT를 할 것인지 결정
		CDBCmd cmd;
		cmd.Init(&gserver.m_dbchar);
		CLCString sql(256);
		sql.Format("SELECT IF(YEARWEEK(NOW())=YEARWEEK(a_updatetime),1,0) AS a_sameweek FROM t_guild_exp WHERE a_guild_index=%d", nGuildIndex);
		cmd.SetQuery(sql);
		if (cmd.Open() && cmd.MoveFirst())
		{
			int nSameWeek;
			cmd.GetRec("a_sameweek", nSameWeek);
			if (nSameWeek == 1)
			{
				bNew = false;
				sql.Format("UPDATE t_guild_exp SET a_exp=a_exp+%f WHERE a_guild_index=%d", dGuildExp, nGuildIndex);
				cmd.SetQuery(sql);
				cmd.Update();
			}
		}
		if (bNew)
		{
			sql.Format("REPLACE INTO t_guild_exp (a_guild_index, a_exp, a_updatetime) VALUES (%d, %f, NOW())", nGuildIndex, dGuildExp);
			cmd.SetQuery(sql);
			cmd.Update();
		}
	}
}
#endif // GUILD_RANKING

#ifdef EVENT_2007_PARENTSDAY
void do_CommandParentsdayAccumulatepointList( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg		rmsg;
	int	CharIndex;

	msg >> CharIndex;

	static const int ACCUMULATE_SIZE = 3;
	
	int	Ranking[ACCUMULATE_SIZE];
	int Accumulatepoint[ACCUMULATE_SIZE];
	//int GuildIndex[ACCUMULATE_SIZE];
	char GuildName[ACCUMULATE_SIZE][MAX_CHAR_NAME_LENGTH+1];

	int count = 0;


	// 누적 포인트를 가져오는 쿼리문 생성
	// SELECT * FROM t_event_2007_parentsday ORDER BY a_point desc;
	CDBCmd	parentsdaypointCmd;
	parentsdaypointCmd.Init( &gserver.m_dbchar );
	CLCString sql( 256 );

	sql.Format( "SELECT * FROM t_event_2007_parentsday ORDER BY a_point DESC, a_guild_idx ASC LIMIT 0, 3" );
	parentsdaypointCmd.SetQuery( sql );

	if( parentsdaypointCmd.Open() && parentsdaypointCmd.MoveFirst() )
	{
		CLCString strGuildName(MAX_CHAR_NAME_LENGTH + 1 );
		do
		{
			Ranking[count] = count + 1;
			parentsdaypointCmd.GetRec( "a_point", Accumulatepoint[count] );			
			parentsdaypointCmd.GetRec( "a_guild_name", strGuildName );
			strcpy( GuildName[count], strGuildName );
			count++;
		}while( parentsdaypointCmd.MoveNext() );
	}

	HelperEventAccmulatepointList( rmsg, MSG_PARENTSDAY_2007_SUCCESS, CharIndex, count,  Ranking, Accumulatepoint, GuildName );
	SEND_Q( rmsg, dest );


}
void do_CommandParentsdayAddPoint( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg		rmsg;
	int CharIndex;
	int GuildIndex;
	int CarnationCount;

	msg >> CharIndex
		>> GuildIndex
		>> CarnationCount;

	CDBCmd parentsdayCmd;
	parentsdayCmd.Init( &gserver.m_dbchar );
	CLCString sql( 1024 );

	//SELECT * FROM t_event_2007_parentsday WHERE a_guild_idx = CharIndex;

	//sprintf( g_buf, "SELECT * FROM t_event_2007_parentsday WHERE a_guild_idx = %d", GuildIndex );
	sql.Format( "SELECT * FROM t_event_2007_parentsday WHERE a_guild_idx = %d", GuildIndex );
	parentsdayCmd.SetQuery( sql );

	int AccumulatePoint = 0;

	if( parentsdayCmd.Open() && parentsdayCmd.MoveFirst() )
	{
		parentsdayCmd.GetRec( "a_point", AccumulatePoint );

		// update query 작성
		// UPDATE t_event_2007_parentsday SET a_point = a_point + CarnationCount, a_char_idx = CharIndex  WEHRE a_guild_idx = GuildIndex;
		CDBCmd updateCmd;
		updateCmd.Init( &gserver.m_dbchar );
//		sprintf( g_buf, "UPDATE t_event_2007_parentsday SET a_point = a_point + %d, a_char_idx = %d WHERE a_guild_idx = %d", CarnationCount, CharIndex, GuildIndex );
		sql.Format( "UPDATE t_event_2007_parentsday SET a_point = a_point + %d WHERE a_guild_idx = %d", CarnationCount, GuildIndex );
		updateCmd.SetQuery( sql );
		if( !updateCmd.Update() )
		{
			// 업데이이트 실패 전송
			HelperEventParentsdayAddPoint( rmsg, MSG_ERROR_ADD_POINT, CharIndex, CarnationCount, 0  );
			SEND_Q( rmsg, dest );
			return;
		}
	}
	else
	{
		// insert query 작성
		//INSERT INTO t_event_2007_parentsday ( a_guild_idx, a_guild_name, a_char_idx, a_accumulate_point )  
		//SELECT g.a_index, g.a_name, gm.a_char_index, 'CarnationCount' FROM t_guild as g, t_guildmember as gm
		//WHERE g.a_enable = 1 AND g.a_index = gm.a_guild_index AND gm.a_pos = 0 AND gm.a_char_index = 'CharIndex';

		CDBCmd insertCmd;
		insertCmd.Init( &gserver.m_dbchar );
//		sprintf( g_buf, "INSERT INTO t_event_2007_parentsday ( a_guild_idx, a_guild_name, a_point ) " 
//			"SELECT g.a_index, g.a_name, %d FROM t_guild as g, t_guildmember as gm"
//			"WHERE g.a_enable = 1 AND g.a_index = gm.a_guild_index AND gm.a_pos = 0 AND gm.a_guild_index = %d", CarnationCount, GuildIndex );
		sql.Format( "INSERT INTO t_event_2007_parentsday ( a_guild_idx, a_guild_name, a_point ) " 
			"SELECT g.a_index, g.a_name, %d FROM t_guild as g, t_guildmember as gm "
			"WHERE g.a_enable = 1 AND g.a_index = gm.a_guild_index AND gm.a_pos = 0 AND gm.a_guild_index = %d", CarnationCount, GuildIndex );
		insertCmd.SetQuery( sql );

		if( !insertCmd.Update() )
		{
			// insert 실패 전송
			HelperEventParentsdayAddPoint( rmsg, MSG_ERROR_ADD_POINT, CharIndex , CarnationCount, 0);
			SEND_Q( rmsg, dest );
			return;
		}
	}
	//gs로 메세지 성공 메세지 전송
	AccumulatePoint = AccumulatePoint + CarnationCount;
	HelperEventParentsdayAddPoint( rmsg, MSG_PARENTSDAY_2007_SUCCESS, CharIndex, CarnationCount, AccumulatePoint );
	SEND_Q( rmsg, dest );

}

void do_CommandParentsdayExchangeTicket( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;
	int GuildIndex;
	int CharIndex;
	msg >> CharIndex
		>> GuildIndex;


	CDBCmd parentsdayCmd;
	parentsdayCmd.Init( &gserver.m_dbchar );
	CLCString sql( 512 );

	int point = 0; // 전체 누적 포인트
	int usedPoint = 0; // 사용된 누적 포인트
	int usePoint = 0; // 남아 있는 누적 포인트
	int GiftTicket = 0;

	// SELECT a_point, a_used_point FROM t_event_2007_parentsday WHERE a_char_idx = CharIndex;
//	sprintf ( g_buf, "SELECT a_point, a_used_point FROM t_event_2007_parentsday WHERE a_guild_idx = %d", GuildIndex );
	sql.Format( "SELECT a_point, a_used_point FROM t_event_2007_parentsday WHERE a_guild_idx = %d", GuildIndex );

	parentsdayCmd.SetQuery( sql );
	if( parentsdayCmd.Open() && parentsdayCmd.MoveFirst() )
	{
		//1. Data 가져오기
		//parentsdaypointCmd.GetRec( "a_point", Accumulatepoint[count] );
		parentsdayCmd.GetRec( "a_point", point );
		parentsdayCmd.GetRec( "a_used_point", usedPoint );

		//2. 가용 point 정보 
		usePoint = point - usedPoint;
		//3.if( 가용 point >=  100  )
		//	update query 작성
		if( usePoint >= 100 )
		{
			GiftTicket = usePoint / 100;
			CDBCmd UpdateCmd;
			UpdateCmd.Init( &gserver.m_dbchar );
			CLCString sql( 256 );

//			sprintf( g_buf, "UPDATE t_event_2007_parentsday SET a_used_point = a_used_point +( GiftTicket * 100 ) WHERE a_guild_idx = %d", GuildIndex );
			sql.Format( "UPDATE t_event_2007_parentsday SET a_used_point = a_used_point +( %d * 100 ) WHERE a_guild_idx = %d", GiftTicket, GuildIndex );
			UpdateCmd.SetQuery( sql );
			UpdateCmd.Update();

			// 성공 메세지 전송
			HelperEventParentsdayExchangeTicket( rmsg, MSG_PARENTSDAY_2007_SUCCESS, CharIndex, GiftTicket );
			SEND_Q( rmsg, dest );
			return;
		}
		else
		{
			//4.else
			// 포인트 부족 에러 전송
			HelperEventParentsdayExchangeTicket( rmsg, MSG_ERROR_NOT_ENOUGH_POINT, CharIndex, 0  );
			SEND_Q( rmsg, dest );
			return;
			
		}
		

	}
	else
	{
		// 등록된 정보 없음 error 전송
			HelperEventParentsdayExchangeTicket( rmsg, MSG_ERROR_NOT_FOUND_ACCUMULATEPOINT_LIST, CharIndex, 0 );
			SEND_Q( rmsg, dest );
	}
}
void do_CommandParentsdayExchangeItem( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;
	int GuildIndex;
	int CharIndex;
	msg >> CharIndex
		>> GuildIndex;
	

	const int ITEM_COUNT = 10;
	
	int GiftItemTable[ITEM_COUNT][3]	 = { // itemIndex, prob, count
		{85, 5, 10},			// 고급제련석, 0.5%, 10
		{2348, 15, 30},			// 메모리스크롤, 1.5%, 30
		{2347, 30, 30},			// 개인창고 확장카드, 3%, 30
		{85, 50, 3},			// 고급제련석, 5%, 3
		{860, 70, 2},			// 데스나이트 소환서, 7%, 2
		{859, 80, 2},			// 바알 소환서, 8%, 2
		{723, 100, 10},			// 문스톤, 10%, 10
		{786, 150, 10},			// 부스터, 15%, 10
		{1075, 200, 15},			// 매우 빨간 아비 꽃 , 20%, 15
		{1076, 300, 15},			// 매우 노란 아비 꽃, 20%, 15		
	};
	
	int giftItemIdx = 0;
	int nProbGift = GetRandom( 0, 1000 );

	int tmpProb = nProbGift;
	int count;

	CDBCmd	parentsdayItemCmd;
	parentsdayItemCmd.Init( &gserver.m_dbchar );
	CLCString sql(1024);
	int ItemIndex[6];
	int ItemCount[6];
	//SELECT a_itemIndex FROM t_event_may2007 WHERE a_date = now() AND a_eventIndex = EVENT_2007_PARENTSDAY  GROUP BY a_itemIndex );
	sql.Format( "SELECT a_itemIndex, count( a_itemIndex ) AS a_itemCount FROM t_event_may2007 WHERE a_date = now() AND a_eventIndex = %d GROUP BY a_itemIndex", EVENT_2007_PARENTSDAY );
	parentsdayItemCmd.SetQuery( sql );

	int i = 0;
	if( parentsdayItemCmd.Open() && parentsdayItemCmd.MoveFirst() )
	{
		do
		{
			parentsdayItemCmd.GetRec( "a_itemIndex", ItemIndex[i] );
			parentsdayItemCmd.GetRec( "a_itemCount", ItemCount[i] );
			i++;
		}while( parentsdayItemCmd.MoveNext() );
	}

	for( i = 0; i < parentsdayItemCmd.m_nrecords; i++ )
	{
		if( ItemIndex[i] == 85  && ItemCount[i] == 1)
		{
			nProbGift = 900;
		}
		else if( ItemIndex[i] == 2348  && ItemCount[i] == 2)
		{
			nProbGift = 900;
		}
		else if( ItemIndex[i] == 2347  && ItemCount[i] == 3)
		{
			nProbGift = 900;
		}
		else
		{
			nProbGift = tmpProb;
		}
	}
	

	for( count = 0; count < ITEM_COUNT; count++ )
	{
		if( nProbGift <= GiftItemTable[count][1] )
		{
			giftItemIdx = GiftItemTable[count][0];
			break;
		}
		else
		{
			nProbGift -= GiftItemTable[count][1];
		}
	}
	HelperEventParentsdayExchangeItem( rmsg, MSG_PARENTSDAY_2007_SUCCESS, CharIndex, giftItemIdx, GiftItemTable[count][2] );
	SEND_Q( rmsg, dest );
}

void do_CommandParentsdayItemInsert( CNetMsg& msg, CDescriptor* dest )
{
	int CharIndex = 0;
	int GiftItemIndex = 0;
	int GiftItemCount = 0;
	CLCString strGuildName(50);

	msg >> CharIndex
		>> GiftItemIndex
		>> GiftItemCount;

	if( CharIndex  == 0 )
	{
		msg >> strGuildName;
		CNetMsg rmsg;
		HelperEventParentsdayGet( rmsg, CharIndex, GiftItemIndex, GiftItemCount );
		rmsg << strGuildName;
		gserver.SendToAllGameServer(rmsg);
		return;
	}
	CDBCmd cmd;
	CLCString sql( 512 );
	cmd.Init( &gserver.m_dbchar );
	
	sql.Format( "INSERT INTO t_event_may2007 ( a_eventIndex, a_itemIndex, a_date, a_charIndex ) VALUES ( %d, %d, now(), %d ) ", EVENT_2007_PARENTSDAY, GiftItemIndex, CharIndex );

	cmd.SetQuery( sql );
	cmd.Update();

//	if( !cmd.Update() )
//	{
//		// insert 실패 
//	}
	CNetMsg rmsg;
	HelperEventParentsdayGet( rmsg, CharIndex, GiftItemIndex, GiftItemCount );
	gserver.SendToAllGameServer(rmsg);
}

#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_TEACH_2007
void do_CommandTeach2007( CNetMsg& msg, CDescriptor* dest )
{
	int charidx = 0;	
	int fame = 0;

	msg >> charidx
		>> fame;

	CDBCmd cmd;
	
	cmd.Init(&gserver.m_dbchar);

	//cmd.BeginTrans();
	// 노란 카네이션에 의한 명성 추가 저장
	sprintf(g_buf, "UPDATE t_characters SET a_fame = a_fame + %d WHERE a_index = %d ", fame, charidx);
	cmd.SetQuery(g_buf);
	cmd.Update();
	cmd.Close();
}

void do_CommandTeach2007AddFlower(CNetMsg & msg, CDescriptor * dest)
{	// 노란 카네이션 직급
	int charidx = 0, maked = 0;
	msg >> charidx;

	char buf[1024];
	CDBCmd dbchar;
	dbchar.Init(&gserver.m_dbchar);
	
	// t_event_teach2007에 a_index가 charidx인 레코드 체크
	sprintf(buf, "SELECT count(a_index) as Make FROM t_event_teach2007 WHERE a_index=%d", charidx);
	dbchar.SetQuery(buf);
	if (dbchar.Open() && dbchar.MoveFirst())
	{
		dbchar.GetRec("Make", maked);
		dbchar.Close();
	}
	else
	{		
		dbchar.Close();
		return;
	}	

	// 지급
	dbchar.Init(&gserver.m_dbchar);
	if (maked == 0)
	{
		// insert
		sprintf(buf, "INSERT t_event_teach2007(a_index, a_addflower) VALUES(%d,1)", charidx);
		dbchar.SetQuery(buf);
		dbchar.Update();		
	}
	else	
	{
		// update
		sprintf(buf, "UPDATE t_event_teach2007 set a_addflower=a_addflower+1 WHERE a_index=%d", charidx);
		dbchar.SetQuery(buf);
		dbchar.Update();		
	}	

	dbchar.Close();

	// send all server
	CNetMsg rmsg;
	HelperTeach2007Addflower(rmsg, charidx);		///===
	gserver.SendToAllGameServer(rmsg);
}

void do_CommandTeach2007ReciveFlower(CNetMsg & msg, CDescriptor * dest)
{	// 노란 카네이션 지급 확인 
	int charidx = 0;
	msg >> charidx;

	char buf[1024];
	CDBCmd dbchar;
	dbchar.Init(&gserver.m_dbchar);

	sprintf(buf, "UPDATE t_event_teach2007 set a_addflower=a_addflower-1 WHERE a_index=%d", charidx);
	dbchar.SetQuery(buf);
	dbchar.Update();	
}
#endif // EVENT_TEACH_2007

#ifdef EVENT_CHILDRENSDAY_2007
void do_CommandChildrensDay2007( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	unsigned char subtype;

	int nCharIndex;
	int nItemIndex;
	int nNeedFlower;
	msg >> subtype;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	CLCString sql(256);

	switch ( subtype )
	{
	case MSG_HELPER_CHILDRENSDAY_2007_CHECK :
		{
			msg	>> nCharIndex
				>> nItemIndex
				>> nNeedFlower;

			char cResult = 1;
			
			sql.Format("SELECT * FROM t_event_childrensday2007 WHERE a_charIndex=%d AND a_itemIndex=%d ", nCharIndex , nItemIndex );
			cmd.SetQuery(sql);
			if ( cmd.Open() )
			{
				if( cmd.m_nrecords > 0 )
					cResult = 0;	// 중복
				else
					cResult = 1;
			}
			else	
			{
				cResult = 1;		
			}

			HelperChildrensDay2007Msg( rmsg, nCharIndex, nItemIndex, nNeedFlower , cResult );
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		}break;
	case MSG_HELPER_CHILDRENSDAY_2007_UPDATE :
		{
			msg	>> nCharIndex
				>> nItemIndex;

			sql.Format("INSERT INTO t_event_childrensday2007 ( a_charIndex, a_itemIndex ) VALUES ( %d, %d ) ", nCharIndex , nItemIndex );
			cmd.SetQuery(sql);
			cmd.Update();
		}break;
	}
	
}
#endif //EVENT_CHILDRENSDAY_2007

#ifdef EVENT_FLOWERTREE_2007
void do_CommandFlowerTree2007( CNetMsg& msg, CDescriptor* dest )
{
	const int nPriceItem[10][4] = { // 아이템, 갯수, 확률, 하루에 받을수 있는 갯수
		{85,3,5,1},				// 고급제련석 , 3, 0.5% , 1
		{845,5,15,2},			// 숙련도 복구주문서, 5, 1.5%, 2
		{857,5,30,3},			// 순간이동 마법서, 5, 3% , 3
		{884,10,50,0},			// 드롭율 증폭제 , 10 , 5% 
		{860,1,70,0},			// 데스나이트 소환서, 1, 7%
		{859,1,80,0},			// 바알소환서, 1, 8%
		{723,7,100,0},			// 문스톤 , 7, 10%
		{786,7,150,0},			// 부스터 , 7, 15% 
		{1310,5,200,0},			// 분홍 벗꽃, 5, 20%
		{1311,5,300,0},			// 흰 벗꽃, 5, 30%
	};

	CNetMsg	rmsg;
	unsigned char	subtype;
	unsigned int	point = 0;
	int				nCharIndex;

	msg >> subtype
		>> nCharIndex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	CLCString sql(1024);

	switch( subtype )
	{
	case MSG_HELPER_FLOWERTREE_2007_MYPOINT:
		{			
			CDBCmd	selectCMD;
			unsigned int ticket = 0;
			int			 recvTicketCount = 0;

			selectCMD.Init( &gserver.m_dbchar );
			sql.Format("SELECT a_point, a_recvTicket FROM t_event_flowertree2007 WHERE a_charIndex=%d ", nCharIndex );
			selectCMD.SetQuery(sql);
			if( selectCMD.Open() && selectCMD.MoveFirst() )
			{
				selectCMD.GetRec("a_point", point );
				selectCMD.GetRec("a_recvTicket", ticket );
				recvTicketCount = point/100 - ticket;
			}
			HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_MYPOINT, nCharIndex );
			rmsg << point;
			rmsg << recvTicketCount;
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);		
		} break;

	case MSG_HELPER_FLOWERTREE_2007_ADDMYPOINT:
		{
			msg >> point;		//추가할 포인트	

			// 유저 업데이트
			CDBCmd selectCMD;
			selectCMD.Init( &gserver.m_dbchar );

			sql.Format( "SELECT * FROM t_event_flowertree2007 WHERE a_charIndex=%d", nCharIndex );
			selectCMD.SetQuery( sql );
		
			if( selectCMD.Open() && selectCMD.MoveFirst() )
			{
				sql.Format("UPDATE t_event_flowertree2007 SET a_point=a_point+%d where a_charIndex=%d", point , nCharIndex );
				cmd.SetQuery(sql);
				cmd.Update();
			}
			else
			{
				sql.Format("INSERT INTO t_event_flowertree2007 (a_charIndex, a_point, a_recvTicket) VALUES (%d, %d, 0) ", nCharIndex,  point );
				cmd.SetQuery(sql);
				cmd.Update();
			}

			HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_ADDMYPOINT, nCharIndex );
			rmsg << point;
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		} break;

	case MSG_HELPER_FLOWERTREE_2007_RECV_TICKET :
		{
			unsigned int ticket = 0;
			int			 recvTicketCount = 0;
			sql.Format("SELECT a_point, a_recvTicket FROM t_event_flowertree2007 WHERE a_charIndex=%d", nCharIndex );
			cmd.SetQuery(sql);
			if( cmd.Open() && cmd.MoveFirst() )
			{
				cmd.GetRec("a_point", point );
				cmd.GetRec("a_recvTicket", ticket );
				recvTicketCount = point/100 - ticket;
			}
			HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_RECV_TICKET, nCharIndex );
			rmsg << recvTicketCount;
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		} break;

	case MSG_HELPER_FLOWERTREE_2007_TICKET_UPDATE :
		{
			int		 recvTicketCount = 0;
			// 유저 업데이트
			msg >> recvTicketCount;
			sql.Format("UPDATE t_event_flowertree2007 SET a_recvTicket=a_recvTicket+%d where a_charIndex=%d", recvTicketCount , nCharIndex );
			cmd.SetQuery(sql);
			cmd.Update();
		}break;

	case MSG_HELPER_FLOWERTREE_2007_PRICEITEM :
		{
			int nRand = GetRandom(0, 1000);
			int nIndex=0;
			for( nIndex=0; nIndex<9 ; nIndex++ )		//9까지 안돌면 자동으로 마지막 아이템
			{
				if( nRand <= nPriceItem[nIndex][2] )
				{
					if( nPriceItem[nIndex][3] > 0 )
					{
						sql.Format("SELECT * FROM t_event_may2007 WHERE a_eventIndex=%d AND a_itemIndex=%d AND a_date=NOW()", EVENT_FLOWERTREE_2007, nPriceItem[nIndex][0] );
						cmd.SetQuery(sql);
						if( cmd.Open() && cmd.MoveFirst() )
						{
							if( cmd.m_nrecords < nPriceItem[nIndex][3] )
							{
								break;		// 갯수 보다 작으면 당첨 
							}
						}
						else
						{
							break;  // 하나도 없으면 당첨
						}
					}
					else  // 한도 개수가 없으면 이면 당첨 
					{
						break;
					}
				}
				nRand -= nPriceItem[nIndex][2];		
			}

			HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_PRICEITEM, nCharIndex );
			rmsg << nPriceItem[nIndex][0];		//ItemIndex
			rmsg << nPriceItem[nIndex][1];		//Count
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		} break;
	case MSG_HELPER_FLOWERTREE_2007_PRICEITEM_INSERT:
		{
			int itemIndex;
			unsigned char isGm;
			CLCString strName(256);
			msg >> isGm;
			msg >> strName;
			msg >> itemIndex;		//아이템 인덱스
			if( isGm == 1 )			//GM이 아니면
			{
				sql.Format("INSERT INTO t_event_may2007( a_eventIndex, a_itemIndex, a_date, a_charIndex ) VALUES ( %d, %d, NOW(), %d ) ", EVENT_FLOWERTREE_2007, itemIndex , nCharIndex );
				cmd.SetQuery(sql);
				cmd.Update();
			}
			
			// 각 서버에 메세지 전달
			if( itemIndex == 85 || itemIndex == 845 || itemIndex == 857 )
			{
				GAMELOG << init("FLOWER TREE EVENT PRIZE WINNING MSG ")
					<< isGm	<< delim  
					<< strName << delim 
					<< itemIndex << end;
				
				HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_PRICEMSG, nCharIndex );
				rmsg << strName;
				rmsg << itemIndex;		//아이템 인덱스
				gserver.SendToAllGameServer( rmsg );
			}
		} break;
	}
}
#endif //EVENT_FLOWERTREE_2007

#ifdef PET_DIFFERENTIATION_ITEM
void do_CommandPetColorChange( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int nCharIndex;
	int	nPetIndex;
	char colorType;

	msg >> nCharIndex
		>> nPetIndex
		>> colorType;

	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	
	sql.Format( "UPDATE t_pet SET a_color = '%c' WHERE a_index = %d AND a_owner = %d", colorType, nPetIndex, nCharIndex );
	cmd.SetQuery( sql );
	cmd.Update();
//	if( !cmd.Update() )
//	{
//		HelperPetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_FAIL, nCharIndex, nPetIndex, strPetName );
//		SEND_Q( rmsg, dest );
//		return;
// 	}

//	HelperPetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_OK, nCharIndex, nPetIndex, strPetName );
//	SEND_Q( rmsg, dest );
//	return;
}
#endif // PET_DIFFERENTIATION_ITEM


#ifdef NEW_GUILD
void do_CommandGuildInclineEstablish( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;
	int guildindex;
	int charindex;
	char guildincline;
	LONGLONG needMoney;
	int needGP;

	msg >> guildindex
		>> charindex
		>> guildincline;

	msg >> needMoney
		>> needGP;



	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );
	sprintf(g_buf, "update t_extend_guild set a_guild_incline= %d, a_guild_point = a_guild_point - %d where a_guild_index = %d", guildincline, needGP, guildindex);
	cmd.SetQuery(g_buf);
	if(!cmd.Update())
	{
		HelperGuildInclineEstablishRepMsg(rmsg, guildindex, charindex, guildincline, MSG_NEW_GUILD_ERROR_CHANG_INCLINE );
		SEND_Q(rmsg, dest);
		return;
	}

	CGuild *guild = gserver.m_guildlist.findguild(guildindex);
	if( guild )
	{
		guild->incline(guildincline);
		guild->AddGuildPoint( -needGP );

		HelperGuildInclineEstablishRepMsg( rmsg, guildindex, charindex, guildincline, MSG_NEW_GUILD_ERROR_INCLINE_OK );
		rmsg << needMoney << needGP;
		//SEND_Q(rmsg, dest);
		gserver.SendToAllGameServer(rmsg);
	}
}

void do_CommandGuildMemberAdjust( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int guildindex;
	int ownerindex;
	int charindex;
	CLCString strPositionName(GUILD_POSITION_NAME+1);
	int contributeExp;
	int contributeFame;

	msg >> guildindex
		>> ownerindex
		>> charindex
		>> strPositionName
		>> contributeExp
		>> contributeFame;
	

	
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );
	
	cmd.BeginTrans();
	sprintf( g_buf, "UPDATE t_extend_guildmember SET a_position_name = '%s', a_contribute_exp = %d, a_contribute_fame = %d "
		" WHERE a_guild_index = %d AND a_char_index = %d ", (const char*)strPositionName, contributeExp, contributeFame, guildindex, charindex );
		cmd.SetQuery( g_buf );

	if ( !cmd.Update() )
	{
		cmd.Rollback();
		GAMELOG << init( "GUILD_MEMBER_ADJUST_UPDATE_ERROR")
			<< mysql_error( &gserver.m_dbchar ) << end;
		
		HelperGuildMemberAdjustRepMsg( rmsg, guildindex, ownerindex, charindex, strPositionName, contributeExp, contributeFame, MSG_NEW_GUILD_ERROR_ADJUST_FAIL );
		SEND_Q( rmsg, dest );
		return;
	}


	cmd.Commit();


	CGuildMember* gMember = gserver.m_guildlist.findmember( charindex );
	if( gMember )
	{
		gMember->positionName( strPositionName );
		gMember->contributeExp( contributeExp );
		gMember->contributeFame( contributeFame );
	}

	HelperGuildMemberAdjustRepMsg( rmsg, guildindex, ownerindex, charindex, strPositionName, contributeExp, contributeFame, MSG_NEW_GUILD_ERROR_ADJUST_OK );
//	SEND_Q( rmsg, dest );
	gserver.SendToAllGameServer(rmsg);

}

void do_CommandNewGuildInfo( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;
	int guildindex;
	int charindex;

	msg >> guildindex
		>> charindex;

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

//	SELECT g.a_index, g.a_name, g.a_level, eg.a_guild_point FROM t_guild as g, t_extend_guild as eg
//	WHERE g.a_index = eg.a_guild_index AND g.a_enable != 0
//	AND g.a_index = 1;


	sprintf( g_buf, "SELECT g.a_index, g.a_name, g.a_level, eg.a_guild_point FROM t_guild as g, t_extend_guild as eg "
		" WHERE g.a_index = eg.a_guild_index AND g.a_enable != 0 AND g.a_index = %d ", guildindex );

	cmd.SetQuery( g_buf );

	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		HelperNewGuildInfoRepMsg( rmsg, charindex, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, dest );
		return;
	}

//	int guildpoint;
//	cmd.GetRec( "a_guild_point", guildpoint );


	sprintf( g_buf, "SELECT g.a_guild_index, g.a_char_index, g.a_char_name, g.a_pos, eg.a_point, eg.a_position_name FROM t_guildmember as g, t_extend_guildmember as eg "
		" WHERE g.a_guild_index = eg.a_guild_index AND g.a_char_index = eg.a_char_index "
		" AND g.a_guild_index = %d AND g.a_char_index = %d ", guildindex, charindex );

	cmd.SetQuery( g_buf );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		HelperNewGuildInfoRepMsg( rmsg, charindex, MSG_GUILD_ERROR_NOTBOSS );
		SEND_Q( rmsg, dest );
		return;
	}
	int usepoint;
	CLCString positionname( GUILD_POSITION_NAME + 1 );
	cmd.GetRec( "a_point", usepoint );
	cmd.GetRec( "a_position_name", positionname );


	sprintf( g_buf, "SELECT avg( c.a_level ) as ave_level FROM t_guildmember as gm, t_characters as c "
		" WHERE gm.a_char_index = c.a_index AND gm.a_guild_index = %d ", guildindex );

	cmd.SetQuery( g_buf );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		HelperNewGuildInfoRepMsg( rmsg, charindex, MSG_GUILD_ERROR_NOTBOSS );
		SEND_Q( rmsg, dest );
		return;
	}

	int avelevel;
	cmd.GetRec( "ave_level", avelevel );
	
	HelperNewGuildInfoNotifyMsg( rmsg, charindex, guildindex, avelevel, /* guildpoint,  */ usepoint,  MSG_NEW_GUILD_INFO_ERROR_OK );
	SEND_Q( rmsg, dest );	
}

void do_CommandNewGuildMemberList( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;

	msg >> guildindex
		>> charindex;
	
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	sprintf( g_buf, "SELECT g.a_guild_index, g.a_char_index, g.a_char_name, g.a_pos, eg.a_position_name, eg.a_point, c.a_job, c.a_job2, c.a_level "
					" FROM t_guildmember as g LEFT JOIN t_extend_guildmember as eg ON g.a_guild_index = eg.a_guild_index "
					" LEFT JOIN t_characters as c ON g.a_char_index = c.a_index "
					" WHERE g.a_char_index = eg.a_char_index AND g.a_guild_index = %d AND c.a_enable = 1", guildindex );

	cmd.SetQuery( g_buf );

	
	int endcount = 0;
	int guildstart = 0;	

	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		HelperNewGuildMemberListRepMsg( rmsg, endcount, guildstart, charindex, guildindex, MSG_GUILD_ERROR_NOTBOSS, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		SEND_Q( rmsg, dest );
		return;
	}
	int membercount		= 0;
	int membercharindex[GUILD_MAX_MEMBER];
	int cumulatePoint[GUILD_MAX_MEMBER];
	char charName[GUILD_MAX_MEMBER][MAX_CHAR_NAME_LENGTH];
	char positionName[GUILD_MAX_MEMBER][GUILD_POSITION_NAME+1];
	char job[GUILD_MAX_MEMBER];
	char job2[GUILD_MAX_MEMBER];
	int level[GUILD_MAX_MEMBER];
	int position[GUILD_MAX_MEMBER];
	CLCString strCharName( MAX_CHAR_NAME_LENGTH );
	CLCString strPositionName( GUILD_POSITION_NAME+1 );


	membercount = cmd.m_nrecords;
	int i = 0;

	do
	{
		cmd.GetRec( "a_char_index", membercharindex[i] );
		cmd.GetRec( "a_point", cumulatePoint[i] );
		cmd.GetRec( "a_char_name", strCharName );
		if( !cmd.GetRec( "a_position_name",strPositionName ) )
		{
			strPositionName = '\0';
		}
		cmd.GetRec( "a_job", job[i] );
		cmd.GetRec( "a_job2", job2[i] );
		cmd.GetRec( "a_level", level[i] );
		cmd.GetRec( "a_pos", position[i] );
		strcpy( charName[i], strCharName );
		strcpy( positionName[i], strPositionName );
		i++;
	}while( cmd.MoveNext());

	int loopcount = membercount / 5;

	for( i = 0; i < loopcount; i++ )
	{
		endcount = (i + 1) * 5;
		HelperNewGuildMemberListRepMsg( rmsg, endcount, guildstart, charindex, guildindex, MSG_NEW_GUILD_MEMBERLIST_ERROR_OK,
				 membercharindex, cumulatePoint, charName, positionName, job, job2, level, position );
		SEND_Q( rmsg, dest );
	}

	int remaincount = membercount - guildstart;
	if( remaincount > 0 )
	{
		HelperNewGuildMemberListRepMsg( rmsg, membercount, guildstart, charindex, guildindex, MSG_NEW_GUILD_MEMBERLIST_ERROR_OK,
				 membercharindex, cumulatePoint, charName, positionName, job, job2, level, position );	
		SEND_Q( rmsg, dest );
	}

	return;
}

void do_CommandNewGuildManage( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;

	msg >> guildindex
		>> charindex;
	
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	
	sprintf( g_buf, "SELECT g.a_guild_index, g.a_char_index, g.a_char_name, g.a_pos, eg.a_position_name, eg.a_contribute_exp, eg.a_contribute_fame, c.a_level "
			" FROM t_guildmember as g LEFT JOIN  t_extend_guildmember as eg ON g.a_guild_index = eg.a_guild_index "
			" LEFT JOIN t_characters as c ON g.a_char_index = c.a_index "
			" WHERE g.a_char_index = eg.a_char_index AND g.a_guild_index = %d AND c.a_enable = 1", guildindex );

	cmd.SetQuery( g_buf );


	int endcount = 0;
	int guildstart = 0;	

	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		HelperNewGuildManageRepMsg( rmsg, endcount, guildstart, charindex, guildindex, MSG_GUILD_ERROR_NOTBOSS, NULL, NULL, NULL, NULL , NULL, NULL, NULL, 1);
		SEND_Q( rmsg, dest );
		return;
	}
	int membercount		= 0;
	int membercharindex[GUILD_MAX_MEMBER];
	int contributeExp[GUILD_MAX_MEMBER];
	int contributeFame[GUILD_MAX_MEMBER];
	int level[GUILD_MAX_MEMBER];
	char charName[GUILD_MAX_MEMBER][MAX_CHAR_NAME_LENGTH];
	char positionName[GUILD_MAX_MEMBER][GUILD_POSITION_NAME+1];
	CLCString strCharName( MAX_CHAR_NAME_LENGTH );
	CLCString strPositionName( GUILD_POSITION_NAME+1 );
	int position[GUILD_MAX_MEMBER];



	membercount = cmd.m_nrecords;
	int i = 0;

	do
	{
		cmd.GetRec( "a_char_index", membercharindex[i] );
		cmd.GetRec( "a_contribute_exp", contributeExp[i] );
		cmd.GetRec( "a_contribute_fame", contributeFame[i] );
		cmd.GetRec( "a_char_name", strCharName );
		if( !cmd.GetRec( "a_position_name",strPositionName ) )
		{
			strPositionName = '\0';
		}
		cmd.GetRec( "a_level", level[i] );
		cmd.GetRec( "a_pos", position[i] );
		strcpy( charName[i], strCharName );
		strcpy( positionName[i], strPositionName );
		i++;
	}while( cmd.MoveNext());

	int loopcount = membercount / 5;
	char first = 1;			// 1: 처음보냄, 2: 나중에 보냄

	for( i = 0; i < loopcount; i++ )
	{
		endcount = (i + 1) * 5;
		HelperNewGuildManageRepMsg( rmsg, endcount, guildstart, charindex, guildindex, MSG_NEW_GUILD_MANAGE_ERROR_OK,  membercharindex, contributeExp, contributeFame, charName, positionName, level, position, first );
		SEND_Q( rmsg, dest );

		first = 0;
	}

	int remaincount = membercount - guildstart;
	if( remaincount > 0 )
	{
		HelperNewGuildManageRepMsg( rmsg, membercount, guildstart, charindex, guildindex, MSG_NEW_GUILD_MANAGE_ERROR_OK,  membercharindex, contributeExp, contributeFame, charName, positionName, level, position, first );
		SEND_Q( rmsg, dest );
	}

	return;
}

void do_CommandNewGuildNotice( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;

	msg >> guildindex
		>> charindex;
	
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	// SELECT * FROM t_guild_notice WHERE a_guild_index = %d

	sprintf( g_buf, "SELECT * FROM t_guild_notice WHERE a_guild_index = %d", guildindex );

	cmd.SetQuery( g_buf );

	if( !cmd.Open() || !cmd.MoveFirst()  )
	{
		HelperNewGuildNoticeRepMsg( rmsg, charindex, guildindex, NULL, NULL );
		SEND_Q( rmsg, dest );
		return;
	}
	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );
	
	cmd.GetRec( "a_title", title );
	cmd.GetRec( "a_text", text );	


	HelperNewGuildNoticeRepMsg( rmsg, charindex, guildindex, title, text );
	SEND_Q( rmsg, dest );
	return;
}

void do_CommandNewGuildNoticeUpdate( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;
	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );

	msg >> guildindex
		>> charindex
		>> title
		>> text;
	
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	// REPLACE INTO t_guild_notice ( a_guild_index, a_title, a_text, a_date ) VALUES ( %d, '%s', '%s', NOW() )", guildindex, title, text );
	sprintf( g_buf, "REPLACE INTO t_guild_notice ( a_guild_index, a_title, a_text, a_date ) VALUES ( %d, '%s', '%s', NOW() )", guildindex, (const char*)title, (const char*)text );

	cmd.SetQuery( g_buf );

	if( !cmd.Update() )
	{
		HelperNewGuildNoticeUpdateRepMsg( rmsg, charindex, guildindex, MSG_NEW_GUILD_NOTICE_UPDATE_ERROR_FAIL );
		SEND_Q( rmsg, dest );
		return;
	}
	HelperNewGuildNoticeUpdateRepMsg( rmsg, charindex, guildindex, MSG_NEW_GUILD_NOTICE_UPDATE_ERROR_OK );
	SEND_Q( rmsg, dest );
	return;
}
void do_CommandNewGuildNoticeTrans( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;

	msg >> guildindex
		>> charindex;
//		>> title
//		>> text;
//	

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		HelperNewGuildNoticeTransRep( rmsg, guildindex, charindex, MSS_NEW_GUILD_NOTICE_TRANSMISSION_ERROR_FAIL );
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* boss = guild->findmember(charindex);
	if (!boss)
	{
		HelperNewGuildNoticeTransRep( rmsg, guildindex, charindex, MSS_NEW_GUILD_NOTICE_TRANSMISSION_ERROR_FAIL );
		SEND_Q(rmsg, dest);
		return ;
	}

	sprintf( g_buf, "SELECT * FROM t_guild_notice WHERE a_guild_index = %d", guildindex );

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );
	cmd.SetQuery( g_buf );

	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		HelperNewGuildNoticeTransRep( rmsg, guildindex, charindex, MSG_NEW_GUILD_NOTICE_ERROR_NOT_MESSAGE );
		SEND_Q( rmsg, dest );
		return;
	}
	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );
	
	cmd.GetRec( "a_title", title );
	cmd.GetRec( "a_text", text );	

	HelperNewGuildNoticeTransMsg( rmsg, guildindex, title, text );
	gserver.SendToAllGameServer(rmsg);
	HelperNewGuildNoticeTransRep( rmsg, guildindex, charindex, MSS_NEW_GUILD_NOTICE_TRANSMISSION_ERROR_OK );
	SEND_Q(rmsg, dest);
}

void do_CommandNewGuildSkillList( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;

	msg >> guildindex
		>> charindex;

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	// SELECT * FROM t_extend_guild WHERE a_guild_index = %d

	sprintf( g_buf, "SELECT a_skill_index, a_skill_level FROM t_extend_guild WHERE a_guild_index = %d", guildindex );

	cmd.SetQuery( g_buf );

	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		HelperNewGuildSkillListRepMsg( rmsg, charindex, guildindex, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST, 0, NULL, NULL );
		SEND_Q( rmsg, dest );
		return;
	}
	
	CLCString skillIndex(256);
	CLCString skillLevel(256);
	const char* pIndex = skillIndex;
	const char* pLevel = skillLevel;
	char tbuf[256];

	int sindex[256];
	int sLevel[256];
	memset( sindex, 0, sizeof( sindex ) );
	memset( sindex, 0, sizeof( sLevel ) );

	
	
	if(!cmd.GetRec( "a_skill_index", skillIndex ) )
	{
		skillIndex = '\0';
	}
	
	if( !cmd.GetRec( "a_skill_level", skillLevel ) )
	{
		skillLevel = '\0';
	}


	int roopcount = 0;
	while (*pIndex && *pLevel)
	{
		int i, l;
		pIndex = AnyOneArg(pIndex, tbuf);
		i = atoi(tbuf);
		pLevel = AnyOneArg(pLevel, tbuf);
		l = atoi(tbuf);

		sindex[roopcount] = i;
		sLevel[roopcount] = l;
		roopcount++;
	}
		
	HelperNewGuildSkillListRepMsg( rmsg, charindex, guildindex, MSG_NEW_GUILD_SKILL_ERROR_OK, roopcount , sindex, sLevel );
	SEND_Q( rmsg, dest );
	return;

}

void do_CommandSaveGuildPoint(CNetMsg& msg, CDescriptor* dest)
{
	int nGuildIndex;
	int nGuildPoint;

	msg >> nGuildIndex
		>> nGuildPoint;

	CGuild* pGuild = gserver.m_guildlist.findguild( nGuildIndex );

	if( pGuild )
	{
		GAMELOG << init( "GUILD POINT SAVE" )
				<< "GUILD" << delim
				<< nGuildIndex << delim
				<< pGuild->name() << delim
				<< "POINT" << delim
				<< nGuildPoint
				<< end;

		CDBCmd cmd;
		cmd.Init( &gserver.m_dbchar );
		CLCString sql(256);
		sql.Format( "UPDATE t_extend_guild SET a_guild_point = %d WHERE a_guild_index = %d", nGuildPoint, nGuildIndex );
		cmd.SetQuery( sql );
		cmd.Update();
	}
}

void do_CommandSaveGuildMemberPoint(CNetMsg& msg, CDescriptor* dest)
{
	int nGuildIndex;
	int nGuildMemberPoint;
	int nCharIndex;

	msg >> nGuildIndex
		>> nCharIndex
		>> nGuildMemberPoint;

	CGuild* pGuild = gserver.m_guildlist.findguild( nGuildIndex );

	if( pGuild )
	{
//		GAMELOG << init( "GUILD MEMBER POINT SAVE" )
//				<< "GUILD" << delim
//				<< nGuildIndex << delim
//				<< pGuild->name() << delim
//				<< "CHAR" << delim
//				<< nCharIndex << delim
//				<< "POINT" << delim
//				<< nGuildMemberPoint
//				<< end;

		CDBCmd cmd;
		cmd.Init( &gserver.m_dbchar );
		CLCString sql(256);
		
		sql.Format( "UPDATE t_extend_guildmember SET a_point = a_point + %d WHERE a_guild_index = %d AND a_char_index = %d", nGuildMemberPoint, nGuildIndex, nCharIndex );
		cmd.SetQuery( sql );
		if (!cmd.Update())
		{
			GAMELOG << init( "GUILD MEMBER POINT SAVE" )
			<< "GUILD" << delim
			<< nGuildIndex << delim
			<< pGuild->name()
			<< end;
		}

		CGuildMember* pMember = gserver.m_guildlist.findmember(nCharIndex);
		if (!pMember) return;

		pMember->AddCumulatePoint(nGuildMemberPoint);

		CNetMsg rmsg;
		HelperNewGuildMemberPointSaveMsg(rmsg, nCharIndex, nGuildIndex, pMember->GetcumulatePoint());
		SEND_Q(rmsg, dest);
		gserver.SendToAllGameServer(rmsg);
	}
}

void do_CommandNewGuildUpdatePoint( CNetMsg& msg, CDescriptor* dest)
{
	int guildindex;
	int charindex;
	int point;

	msg >> charindex
		>> guildindex
		>> point;

	CGuild* pGuild = gserver.m_guildlist.findguild( guildindex );
	if( pGuild )
	{
		CDBCmd cmd;
		cmd.Init( &gserver.m_dbchar );
		CLCString sql( 1024);
		sql.Format( "UPDATE t_extend_guild SET a_guild_point = a_guild_point + %d WHERE a_guild_index = %d ", point, guildindex );
		cmd.SetQuery( sql );
		if( !cmd.Update() )
		{
			GAMELOG << init( "GUILD POINT UPDATE" )
			<< "GUILD" << delim
			<< guildindex << delim
			<< pGuild->name()
			<< end;
		}
		pGuild->AddGuildPoint( point );

		CNetMsg rmsg;
		HelperNewGuildPointUpdateMsg( rmsg, charindex, guildindex, pGuild->GetGuildPoint() );
		SEND_Q( rmsg, dest );
		gserver.SendToAllGameServer(rmsg);
	}

}

void do_CommandGuildNotice( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int charindex;	
	msg >> charindex;

//#ifdef NEW_GUILD
//	int channel;
//	int zoneindex;
//	msg >> channel >> zoneindex;
//#endif // NEW_GUILD


	CGuildMember* member = gserver.m_guildlist.findmember(charindex);
	if (member)
	{
		CGuild* guild = member->guild();

		if (!guild)
			return;
		CDBCmd cmd;
		cmd.Init( &gserver.m_dbchar );

		// SELECT * FROM t_guild_notice WHERE a_guild_index = %d

		sprintf( g_buf, "SELECT * FROM t_guild_notice WHERE a_guild_index = %d", guild->index() );

		cmd.SetQuery( g_buf );

		if( !cmd.Open() || !cmd.MoveFirst() )
		{
			HelperNewGuildNotice( rmsg, charindex, guild->index(), NULL, NULL );
			SEND_Q( rmsg, dest );
			return;
		}
		CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
		CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );
		
		cmd.GetRec( "a_title", title );
		cmd.GetRec( "a_text", text );	

		HelperNewGuildNotice( rmsg, charindex, guild->index(), title, text );
		SEND_Q( rmsg, dest );
	}
}

void do_CommandGuildSkillLearn( CNetMsg& msg, CDescriptor* dest )
{
	int nGuildIndex;
	int nGuildPoint;
	CLCString  guildSkillIndex(1024);
	CLCString  guildSkillLevel(1024);

	const char* pskillIndex = guildSkillIndex;
	const char* pskillLevel = guildSkillLevel;
	char tbuf[256];
	int sindex[256];
	int sLevel[256];

	msg >> nGuildIndex
		>> nGuildPoint
		>> guildSkillIndex
		>> guildSkillLevel;


	CGuild* pGuild = gserver.m_guildlist.findguild( nGuildIndex );

	if( pGuild )
	{
		CDBCmd cmd;
		cmd.Init( &gserver.m_dbchar );
		CLCString sql( 1024);
		sql.Format( "UPDATE t_extend_guild SET a_skill_index ='%s', a_skill_level='%s', a_guild_point = a_guild_point +( %d ) WHERE a_guild_index = %d ", (const char*)guildSkillIndex, (const char*)guildSkillLevel, nGuildPoint, nGuildIndex  );
		cmd.SetQuery( sql );
		if( !cmd.Update() )
		{
			GAMELOG << init( "GUILD SKILL SAVE" )
					<< "GUILD" << delim
					<< nGuildIndex << delim
					<< pGuild->name()
					<< end;
		}	

		pGuild->AddGuildPoint( nGuildPoint );
		
		CNetMsg rmsg;
		HelperNewGuildPointUpdateMsg( rmsg, pGuild->boss()->charindex(), nGuildIndex, pGuild->GetGuildPoint() );
		SEND_Q( rmsg, dest );
		gserver.SendToAllGameServer(rmsg);

		int roopcount = 0;
		while( *pskillIndex && pskillLevel )
		{
			int i, l;
			pskillIndex = AnyOneArg( pskillIndex, tbuf );
			i = atoi( tbuf );
			pskillLevel = AnyOneArg( pskillLevel, tbuf );
			l = atoi( tbuf );

			sindex[roopcount] = i;
			sLevel[roopcount] = l;
			if( sindex[roopcount] == 444 )
			{
			//	int maxmember = pGuild->maxmember() + ( 5 * sLevel[roopcount] );

#ifdef NEW_GUILD_BUG_FIX
				int maxmember;
				if(sLevel[roopcount] <= 5)
					maxmember = GUILD_LEVEL6_MAX_MEMBER + (5 * sLevel[roopcount] );
				else
					maxmember = 55 + 9 * (sLevel[roopcount] - 5);
#else
				int maxmember = GUILD_LEVEL6_MAX_MEMBER + ( 5 * sLevel[roopcount] );
#endif // NEW_GUILD_BUG_FIX
				pGuild->setMaxmember( maxmember );

				sql.Format( "UPDATE t_extend_guild SET a_guild_maxmember = %d WHERE a_guild_index = %d", maxmember, nGuildIndex  );
				cmd.SetQuery( sql );
				if( !cmd.Update() )
				{					
					GAMELOG << init( "GUILD MEMBER UPDATE FAIL" )
							<< "GUILD" << delim
							<< nGuildIndex << end;
				}
			}
			roopcount++;
		}
		HelperExtendGuildLoadNotifyMsg( rmsg, pGuild, roopcount, sindex, sLevel );
		gserver.SendToAllGameServer( rmsg );



	}
}
#endif // NEW_GUILD

void do_InsertGuard(CNetMsg & msg, CDescriptor * dest)
{
	int zoneindex, itemindex;
	float x, z, h;
	msg >> zoneindex >> x >> z >> h >> itemindex;

	int mobindex;
	
	switch(itemindex)
	{
	case 2380:
		mobindex = 405;
		break;
	case 2381:
		mobindex = 406;
		break;

	default:
		mobindex = 0;
		break;
	}

	if(mobindex == 0)
	{
		return;
	}

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );
	CLCString sql( 1024);
	sql.Format( "INSERT INTO t_castle_guard VALUES(%d, %d, %f, %f, %f)", zoneindex, mobindex, x, z, h);
	cmd.SetQuery( sql );
	if( !cmd.Update() )
	{
		GAMELOG << init( "CASTLE GUARD INSERT FAILED" )
			<< zoneindex << delim
			<< mobindex << delim
			<< x << delim
			<< z << delim
			<< h << delim
			<< end;
	}		
}

void do_CastleTowerAdd(CNetMsg & msg, CDescriptor * dest)
{
	char status[7];
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );
	CLCString sql(1024);
	int zoneidx = 0;

	msg >> zoneidx;
	sql.Format( "DELETE FROM t_castle_tower WHERE a_zone_num=%d", zoneidx);
	cmd.SetQuery( sql );
	cmd.Update();

	for(int i=0; i<7;i++)
	{
		msg >> status[i];
		sql.Format( "INSERT INTO t_castle_tower VALUES(%d, %d, %d)", zoneidx, i, (int)status[i]);
		cmd.SetQuery( sql );
		if( !cmd.Update() )
		{
			GAMELOG << init( "CASTLE TOWER ADD" )
				<< zoneidx << delim
				<< i << delim
				<< status[i] << delim
				<< end;
		}		
	}
}

void do_CastleTowerReinforce(CNetMsg & msg, CDescriptor * dest)
{
	int zoneidx = 0;
	char type = 0, step = 0;
	msg >> zoneidx >> type >> step;
	
	// type : 0. 공격형 타워 1. 가드형 타워 2. 성문
	// step 1 - 3
	if (type <=0 || type > 3 || step <= 0 || step > 3)
	{
		return;
	}

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );
	CLCString sql(1024);

	sql.Format( "DELETE FROM t_castle_reinforce WHERE a_zone_num=%d and a_type=%c", zoneidx, type);
	cmd.SetQuery( sql );
	cmd.Update();

	sql.Format( "INSERT INTO t_castle_reinforce VALUES(%d, %c, %c)", zoneidx, type, step);
	cmd.SetQuery( sql );
	cmd.Update();
}

void do_CastleQuartersInstall(CNetMsg & msg, CDescriptor * dest)
{
	int qindex = 0, gindex = 0;
	CLCString gname(MAX_CHAR_NAME_LENGTH + 1);

	msg >> qindex >> gindex >> gname;

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );
	CLCString sql(1024);

	sql.Format( "DELETE FROM t_castle_rebrith WHERE a_zone_num=%d and a_npc_idx=%c", ZONE_DRATAN, qindex);
	cmd.SetQuery( sql );
	cmd.Update();

	sql.Format( "INSERT INTO t_castle_rebrith VALUES(%d, %d, %d, '%s')", ZONE_DRATAN, qindex, gindex, (const char *) gname);
	cmd.SetQuery( sql );
	cmd.Update();
}

#ifdef EVENT_OPEN_ADULT_SERVER
void do_CommandOpenAdultServer(CNetMsg& msg, CDescriptor* dest)
{
	int subtype;
	msg >> subtype;
	
	switch(subtype)
	{
	case MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB:
		{
			CNetMsg rmsg;
			int charindex;
			msg >> charindex;
			
			CDBCmd cmd;
			cmd.Init( &gserver.m_dbchar );
			CLCString sql(1024);
			sql.Format("SELECT * FROM t_event_adultserver WHERE a_charindex = %d", charindex);
			cmd.SetQuery(sql);
			if( !cmd.Open() )
				return ;
			
			if(cmd.MoveFirst())
			{
				// 이미 있음
				HelperEventAdultServerErrorMsg(rmsg, charindex, MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_ALREADY);
				SEND_Q(rmsg, dest);
			}
			else
			{
				// 테이블에 추가(보상)
				sql.Format("SELECT * FROM t_event_adultserver ORDER BY a_ranking DESC LIMIT 1");
				cmd.SetQuery(sql);
				if(!cmd.Open())
					return ;
				if(cmd.MoveFirst())
				{
					int rank;
					cmd.GetRec("a_ranking", rank);
					sql.Format("INSERT INTO t_event_adultserver (a_charindex, a_ranking) VALUES (%d, %d)", charindex, rank + 1);
					cmd.SetQuery(sql);
					if(cmd.Update())
					{
						HelperEventAdultServerErrorMsg(rmsg, charindex, MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_OK);
						rmsg << (int)(rank + 1);
						SEND_Q(rmsg, dest);
					}
				}
				else
				{
					// 1등
					sql.Format("INSERT INTO t_event_adultserver (a_charindex, a_ranking) VALUES (%d, 1)", charindex);
					cmd.SetQuery(sql);
					if(cmd.Update())
					{
						HelperEventAdultServerErrorMsg(rmsg, charindex, MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_OK);
						rmsg << (int)1;
						SEND_Q(rmsg, dest);
					}
				}
			}
		}
		break;

	case MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_FAILED:
		{
			int charindex;
			CLCString sql(1024);
			CDBCmd cmd;

			msg >> charindex;

			cmd.Init(&gserver.m_dbchar);
			sql.Format("DELETE FROM t_event_adultserver WHERE a_charindex = %d", charindex);
			cmd.SetQuery(sql);
			cmd.Update();
		}
		break;
	}
}
#endif // EVENT_OPEN_ADULT_SERVER

#ifdef PET_NAME_CHANGE
void do_CommandPetChangeName( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int nCharIndex;
	int	nPetIndex;
	CLCString strPetName( 30 );

	msg >> nCharIndex
		>> nPetIndex
		>> strPetName;

	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	
	sql.Format( "SELECT * FROM t_pet WHERE a_index = %d AND a_owner = %d ", nPetIndex, nCharIndex );
	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		HelperPetNameChange(rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_NOPET, nCharIndex, nPetIndex, strPetName );
		SEND_Q(rmsg, dest);
		return;
	}


	sql.Format( "SELECT * FROM t_pet_name WHERE a_pet_name = '%s' ", (const char*)strPetName );
	cmd.SetQuery( sql );
	cmd.Open();
	if( cmd.m_nrecords > 0 )
	{
		HelperPetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_DUPLICATE, nCharIndex, nPetIndex, strPetName );
		SEND_Q( rmsg, dest );
		return;

	}

	sql.Format( "REPLACE  INTO t_pet_name ( a_pet_index, a_pet_owner, a_pet_name ) VALUES ( %d, %d, '%s' )", nPetIndex, nCharIndex, (const char* )strPetName );
	cmd.SetQuery( sql );
	if( !cmd.Update() )
	{
		HelperPetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_FAIL, nCharIndex, nPetIndex, strPetName );
		SEND_Q( rmsg, dest );
		return;
	}

	HelperPetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_OK, nCharIndex, nPetIndex, strPetName );
	SEND_Q( rmsg, dest );
	return;


}
#endif // PET_NAME_CHANGE

#ifdef REWARD_IDC2007
void do_CommandRewardIDC2007( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int userindex;

	msg >> userindex;

	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	sql.Format("INSERT INTO t_reward_idc2007 ( a_user_idx, a_date ) VALUES (%d, NOW())", userindex);
	cmd.SetQuery(sql);
	if(cmd.Update())
	{
		HelperRewardIDC2007Msg(rmsg, userindex);
		SEND_Q(rmsg, dest);
	}
}
#endif // REWARD_IDC2007

#ifdef EVENT_HALLOWEEN_2007
void do_CommandHalloween2007( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int charIndex;

	msg >> charIndex;

	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	sql.Format("INSERT INTO t_event_halloween2007 ( a_char_index , a_update_date ) VALUES (%d, NOW())", charIndex );
	cmd.SetQuery(sql);
	if(cmd.Update())
	{
		HelperHalloween2007Msg( rmsg, charIndex , 0 );
	}
	else
	{
		HelperHalloween2007Msg( rmsg, charIndex , 1 );
	}

	SEND_Q(rmsg, dest);
}
#endif // EVENT_HALLOWEEN_2007

#ifdef DYNAMIC_DUNGEON
void do_CommandDVDRateChange( CNetMsg& msg, CDescriptor* dest )
{
	// 모든 서버에 변경 subtype과 nRate 전달
	CNetMsg	rmsg;
	CDBCmd	cmd;
	unsigned char subtype;
	int		nRate;
	msg >> subtype >> nRate;
	
	// DB 에 저장
	int	nZoneIndex=0;
	CLCString sql(1024), strRateName(1024);

	switch( subtype )
	{
	case MSG_MANAGEMENT_ENV_CHANGE :	strRateName.Format("a_env_rate");			break;
	case MSG_MANAGEMENT_STATE_CHANGE:	strRateName.Format("a_mop_rate");			break;
	case MSG_MANAGEMENT_TAX_CHANGE :	strRateName.Format("a_tax_rate");			break;
	case MSG_MANAGEMENT_HUNTER_TAX_CHANGE :		strRateName.Format("a_hunt_rate");	break;		
	}

	sql.Format("SELECT a_zone_index FROM t_castle_dungeon WHERE a_zone_index=%d" , ZONE_DRATAN_CASTLE_DUNGEON );
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if( cmd.Open() && cmd.MoveFirst() )
	{
		cmd.GetRec("a_zone_index", nZoneIndex );
	}	

	if(  nZoneIndex == ZONE_DRATAN_CASTLE_DUNGEON )
	{	
		sql.Format("UPDATE t_castle_dungeon SET %s = %d WHERE a_zone_index=%d" , (const char*)strRateName, nRate, ZONE_DRATAN_CASTLE_DUNGEON );
	}
	else
	{
		sql.Format("INSERT INTO t_castle_dungeon (  a_zone_index , %s )  VALUES ( %d, %d  ) " , (const char*)strRateName , ZONE_DRATAN_CASTLE_DUNGEON , nRate) ;
	}
	cmd.SetQuery(sql);
	cmd.Update();

	HelperDVDRateChangeMsg( rmsg, subtype, nRate );	
	gserver.SendToAllGameServer(rmsg);
}
#endif //DYNAMIC_DUNGEON

#ifdef SAVE_MOONSTONE_MIX
void do_CommandSaveMoonStoneMix(CNetMsg& msg, CDescriptor* dest)
{
	CNetMsg rmsg;
	int charindex;
	msg >> charindex;
	
	CLCString sql(1024);
	sql.Format("SELECT * FROM t_event_moonstone WHERE a_charindex = %d", charindex);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(cmd.Open())
	{
		if(cmd.MoveFirst())
		{
			// 성공 횟수 증가
			int mixcount;
			cmd.GetRec("a_mixcount", mixcount);
			mixcount += 1;
			
			sql.Format("UPDATE t_event_moonstone SET a_mixcount = %d WHERE a_charindex = %d", mixcount, charindex);
			cmd.SetQuery(sql);
			if(cmd.Update())
			{
				GAMELOG << init("SUCCESS SAVE MOONSTONE MIX : update")
						<< "CharIndex" << delim << charindex
						<< "Count" << delim << mixcount
						<< end;
			}
			else
			{
				GAMELOG << init("FAILED SAVE MOONSTONE MIX : update")
						<< "CharIndex" << delim << charindex
						<< "Count" << mixcount
						<< end;
			}
		}
		else
		{
			// 처음 저장
			sql.Format("INSERT INTO `t_event_moonstone` (`a_charindex`, `a_mixcount`) VALUES (%d, %d)", charindex, 1);
			cmd.SetQuery(sql);
			if(!cmd.Update())
			{
				GAMELOG << init("FAILED SAVE MOONSTONE MIX : insert")
						<< "CharIndex" << delim << charindex
						<< end;
			}
		}
	}
	else
	{
		GAMELOG << init("FAILED SAVE MOONSTONE MIX : open")
				<< "CharIndex" << delim << charindex
				<< end;
	}
}
#endif // SAVE_MOONSTONE_MIX

#ifdef PET_TURNTO_NPC
void do_CommandPetTurnToNpc( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int nCharIndex;
	int	nPetIndex;
	int nToNpc;
	int size;

	msg >> nCharIndex
		>> nPetIndex
		>> nToNpc
		>> size;

	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
#ifdef PET_TURNTO_NPC_ITEM
	sql.Format( "UPDATE t_pet SET a_pet_turnto_npc=%d, a_pet_size=%d WHERE a_index = %d AND a_owner = %d", nToNpc, size, nPetIndex, nCharIndex );
#else
	sql.Format( "UPDATE t_pet SET a_pet_turnto_npc = %d WHERE a_index = %d AND a_owner = %d", nToNpc, nPetIndex, nCharIndex );
#endif // PET_TURNTO_NPC_ITEM
	cmd.SetQuery( sql );
	if( !cmd.Update() )
	{
		GAMELOG << init("FAILED SAVE PET CHANGE NPC")
				<< "CharIndex" << delim << nCharIndex << delim
				<< "PetIndex" << delim << nPetIndex << delim
				<< "NpcIndex" << delim << nToNpc << end;
	}
}
#endif //PET_TURNTO_NPC

#ifdef ALTERNATE_MERCHANT
void do_CommandMerchantStart( CNetMsg& msg, CDescriptor* dest )
{
	// 캐릭터 인덱스를 통해서 보관중인 아이템이 있는지 검사
	// 아이템 등록
	// 결과 전송
	CNetMsg rmsg;
	int nCharIndex;
	char normalCount;
		
	msg >> nCharIndex
		>> normalCount;

	CLCString sql( 1024 );
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	sql.Format( "SELECT * FROM t_parcel_room%.2d WHERE a_user_idx = %d AND a_count != 0", nCharIndex % 10, nCharIndex );
	cmd.SetQuery( sql );
	if( cmd.Open() || cmd.MoveFirst() )
	{
		if( cmd.m_nrecords > 0 )
		{
			HelperMerchantErrorMsg( rmsg, MSG_ALTERNATEMERCHANT_ERROR_NOCOLLECT_ITEM, nCharIndex );
			SEND_Q( rmsg, dest );
			return;
		}
	}

	int i;
	int* pNormalItemDBIndex		= new int[normalCount];
	LONGLONG* pNormalItemCount	= new LONGLONG[normalCount];
	char* strSerial				= new char[normalCount];
	int* pNoramalitemWearPos	= new int[normalCount];		int* pNormalItemFlag		= new int[normalCount];
	int* pNormalItemPlus		= new int[normalCount];		int* pNormalItemOption0		= new int[normalCount];
	int* pNormalItemOption1		= new int[normalCount];		int* pNormalItemOption2		= new int[normalCount];
	int* pNormalItemOption3		= new int[normalCount];		int* pNormalItemOption4		= new int[normalCount];

	for( i = 0; i < normalCount; i++ )
	{
		msg >> pNormalItemDBIndex[i]
			>> pNormalItemCount[i]
			>> strSerial[i]
			>> pNoramalitemWearPos[i]
			>> pNormalItemFlag[i]
			>> pNormalItemPlus[i]
			>> pNormalItemOption0[i]
			>> pNormalItemOption1[i]
			>> pNormalItemOption2[i]
			>> pNormalItemOption3[i]
			>> pNormalItemOption4[i];
	}

	for( i = 0; i < normalCount; i++ )
	{
		
		sql.Format( "REPLACE INTO ( a_user_idx, a_item_idx, a_plus, a_wear_pos, a_flag, a_serial, a_count, a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4 ) "
				" t_parcelroom%.2d VALUES ( %d, %d, %d, %d, %d, %s, %d, %d, %d, %d, %d, %d )", nCharIndex % 10, nCharIndex, pNormalItemDBIndex[i], 
				pNormalItemPlus[i], pNoramalitemWearPos[i], pNormalItemFlag[i], strSerial[i], pNormalItemCount[i], pNormalItemOption0[i], pNormalItemOption1[i], pNormalItemOption2[i], pNormalItemOption3[i], pNormalItemOption4[i] );

		cmd.SetQuery(sql);
		cmd.Update();
	}



	char cPackCount;
	msg >> cPackCount;

	if( cPackCount > 0 )
	{
		int* pPackItemDBIndex		= new int[cPackCount];
		LONGLONG* pPackItemCount	= new LONGLONG[cPackCount];
		char* strSerial				= new char[cPackCount];
		int* pNoramalitemWearPos	= new int[cPackCount];		int* pPackItemFlag		= new int[cPackCount];
		int* pPackItemPlus		= new int[cPackCount];		int* pPackItemOption0		= new int[cPackCount];
		int* pPackItemOption1		= new int[cPackCount];		int* pPackItemOption2		= new int[cPackCount];
		int* pPackItemOption3		= new int[cPackCount];		int* pPackItemOption4		= new int[cPackCount];

		for( i = 0; i < cPackCount; i++ )
		{
			msg >> pPackItemDBIndex[i]
				>> pPackItemCount[i]
				>> strSerial[i]
				>> pNoramalitemWearPos[i]
				>> pPackItemFlag[i]
				>> pPackItemPlus[i]
				>> pPackItemOption0[i]
				>> pPackItemOption1[i]
				>> pPackItemOption2[i]
				>> pPackItemOption3[i]
				>> pPackItemOption4[i];
		}

		for( i = 0; i < normalCount; i++ )
		{
			
			sql.Format( "REPLACE INTO ( a_user_idx, a_item_idx, a_plus, a_wear_pos, a_flag, a_serial, a_count, a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4 ) "
					" t_parcelroom%.2d VALUES ( %d, %d, %d, %d, %d, %s, %d, %d, %d, %d, %d, %d )", nCharIndex % 10, nCharIndex, pPackItemDBIndex[i],
					pPackItemPlus[i], pNoramalitemWearPos[i], pPackItemFlag[i], strSerial[i], pPackItemCount[i], pPackItemOption0[i], pPackItemOption1[i], 
					pPackItemOption2[i], pPackItemOption3[i], pPackItemOption4[i] );

			cmd.SetQuery(sql);
			cmd.Update();
		}
		delete [] pPackItemDBIndex;		delete [] pPackItemCount;		delete [] strSerial;
		delete [] pNoramalitemWearPos;	delete [] pPackItemFlag;		delete [] pPackItemPlus;		delete [] pPackItemOption0;
		delete [] pPackItemOption1;		delete []  pPackItemOption2;	delete [] pPackItemOption3;
	}

	HelperMerchantErrorMsg( rmsg, MSG_ALTERNATEMERCHANT_ERROR_OK, nCharIndex );
	SEND_Q( rmsg, dest );

	HelperMerchantStartMsg( rmsg, nCharIndex );
	return;
}

void do_CommandMerchantBuy( CNetMsg& msg, CDescriptor* dest )
{
	// 캐릭터가 가지고 있는 아이템 변경
	CNetMsg rmsg;
	int nCharIndex;
	int	nItemIndex;
	int nItemCount;
		
	msg >> nCharIndex
		>> nItemIndex
		>> nItemCount;

	CLCString sql( 1024 );
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	sql.Format( "SELECT * FROM t_parcel_room%.2d WHERE a_user_idx = %d AND a_count != 0 AND a_item_idx = %d ", nCharIndex % 10, nCharIndex, nItemIndex );
	cmd.SetQuery( sql );
	if( !cmd.Open() )
	{
		if( cmd.m_nrecords > 0 )
		{
			HelperMerchantErrorMsg( rmsg, MSG_ALTERNATEMERCHANT_ERROR_NOTITEM, nCharIndex );
			SEND_Q( rmsg, dest );
			return;
		}
	}

	sql.Format( "UPDATE t_parcel_room%.2d SET a_count = a_count - %d WHERE a_user_idx = %d AND a_item_idx = %d", nCharIndex, nItemCount, nCharIndex, nItemIndex );
	cmd.SetQuery( sql );
	cmd.Update();



}

void do_CommandMerchantProductRecover( CNetMsg& msg, CDescriptor* dest )
{
	// 캐릭터가 가지고 있는 물품 회수
	CNetMsg	rmsg;
	int nCharIndex;

	msg >> nCharIndex;

	CLCString	sql( 1024 );
	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );

	sql.Format( "SELECT * FROM t_parcel_room%.2d WHERE a_user_idx = %d AND a_item_idx != 19 AND a_count != 0" , nCharIndex, nCharIndex );
	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
			HelperMerchantErrorMsg( rmsg, MSG_ALTERNATEMERCHANT_ERROR_NOTITEM, nCharIndex );
			SEND_Q( rmsg, dest );
			return;
	}

	int nItemCount = cmd.m_nrecords;

	int* pIndex = new int[nItemCount];
	int* pItemDBIndex		= new int[nItemCount];
	char pstrSerial[20][50];
	int* pItemCount = new int[nItemCount];
	int* pItemWearPos	= new int[nItemCount];		int* pItemFlag		= new int[nItemCount];
	int* pItemPlus			= new int[nItemCount];		short* pItemOption0	= new short[nItemCount];
	short* pItemOption1		= new short[nItemCount];		short* pItemOption2	= new short[nItemCount];
	short* pItemOption3		= new short[nItemCount];		short* pItemOption4	= new short[nItemCount];

	int i =0;

	do
	{
		CLCString strSerial( 50 );

		cmd.GetRec( "a_index", pIndex[i] );
		cmd.GetRec( "a_item_idx", pItemDBIndex[i] );
		cmd.GetRec( "a_plus", pItemPlus[i] );
		cmd.GetRec( "a_wear_pos", pItemWearPos[i] );
		cmd.GetRec( "a_flag", pItemFlag[i]);
		if( !cmd.GetRec( "a_serial",strSerial ) )
		{
			strSerial = '\0';
		}
		cmd.GetRec( "a_count", pItemCount[i] );
		cmd.GetRec( "a_item_option0", pItemOption0[i] );
		cmd.GetRec( "a_item_option1", pItemOption1[i] );
		cmd.GetRec( "a_item_option2", pItemOption2[i] );
		cmd.GetRec( "a_item_option3", pItemOption3[i] );
		cmd.GetRec( "a_item_option4", pItemOption4[i] );

		strcpy( pstrSerial[i], strSerial );
		i++;
	}while( cmd.MoveNext());


	HelperMerchantErrorMsg( rmsg, MSG_ALTERNATEMERCHANT_ERROR_OK, nCharIndex );
	SEND_Q( rmsg, dest );
	HelperMerchantProductRecoverMsg( rmsg, nCharIndex, nItemCount, pItemDBIndex, pstrSerial, pItemCount, pItemWearPos, 
		pItemFlag, pItemPlus, pItemOption0, pItemOption1, pItemOption2, pItemOption3, pItemOption4 );
	SEND_Q( rmsg, dest );
	
	for( i = 0 ; i < nItemCount; i++ )
	{
		sql.Format( "UPDATE t_parcel_room%.2d SET a_count = 0 WHERE a_index = %d" , nCharIndex, pIndex[i] );
		cmd.SetQuery( sql );
		cmd.Update();		
	}
	


	delete [] pIndex;	delete [] pItemDBIndex;	delete []  pItemCount;
	delete []  pItemWearPos;	delete []  pItemFlag;	delete []  pItemPlus;
	delete []  pItemOption0;	delete []  pItemOption1;	delete []  pItemOption2;
	delete []  pItemOption3;	delete []  pItemOption4;

	return;
}

void do_CommandMerchantNasRecovery( CNetMsg& msg, CDescriptor* dest )
{
	// 캐릭터가 가지고 있는 나스 회수
	CNetMsg	rmsg;
	int nCharIndex;

	msg >> nCharIndex;

	CLCString	sql( 1024 );
	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );

	sql.Format( "SELECT * FROM t_parcel_room%.2d WHERE a_user_idx = %d AND a_item_idx = 19 AND a_count != 0" , nCharIndex, nCharIndex );
	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
			HelperMerchantErrorMsg( rmsg, MSG_ALTERNATEMERCHANT_ERROR_NOCOLLECT_ITEM, nCharIndex );
			SEND_Q( rmsg, dest );
			return;
	}

//	int nItemCount = cmd.m_nrecords;

	int nIndex;
	int nItemDBIndex;
	CLCString strSerial( 50 );
	int nItemCount;
	int nItemWearPos;	int nItemFlag;
	int nItemPlus;			short nItemOption0;
	short nItemOption1;		short nItemOption2;
	short nItemOption3;		short nItemOption4;


	cmd.GetRec( "a_index", nIndex );
	cmd.GetRec( "a_item_idx", nItemDBIndex );
	cmd.GetRec( "a_plus", nItemPlus );
	cmd.GetRec( "a_wear_pos", nItemWearPos );
	cmd.GetRec( "a_flag", nItemFlag);
	if( !cmd.GetRec( "a_serial",strSerial ) )
	{
		strSerial = '\0';
	}
	cmd.GetRec( "a_count", nItemCount );
	cmd.GetRec( "a_item_option0", nItemOption0 );
	cmd.GetRec( "a_item_option1", nItemOption1 );
	cmd.GetRec( "a_item_option2", nItemOption2 );
	cmd.GetRec( "a_item_option3", nItemOption3 );
	cmd.GetRec( "a_item_option4", nItemOption4 );



	HelperMerchantErrorMsg( rmsg, MSG_ALTERNATEMERCHANT_ERROR_OK, nCharIndex );
	SEND_Q( rmsg, dest );
	HelperMerchantNasMsg( rmsg, nCharIndex, nItemDBIndex, nItemPlus, nItemWearPos, nItemFlag, 
		strSerial,	nItemCount, nItemOption0, nItemOption1, nItemOption2, nItemOption3, nItemOption4 );
	SEND_Q( rmsg, dest );
	

	sql.Format( "UPDATE t_parcel_room%.2d SET a_count = 0 WHERE a_index = %d" , nCharIndex, nIndex );
	cmd.SetQuery( sql );
	cmd.Update();
	return;
}

void do_CommandMerchantEnd( CNetMsg& msg, CDescriptor* dest )
{
	// 캐릭터 인덱스를 통해서 보관중인 아이템이 있는지 검사
	// 아이템 등록
	// 결과 전송
	CNetMsg rmsg;
	int nCharIndex;
	char normalCount;
		
	msg >> nCharIndex
		>> normalCount;

	CLCString sql( 1024 );
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	int i;
	int* pNormalItemDBIndex		= new int[normalCount];
	LONGLONG* pNormalItemCount	= new LONGLONG[normalCount];
	char* strSerial				= new char[normalCount];
	int* pNoramalitemWearPos	= new int[normalCount];		int* pNormalItemFlag		= new int[normalCount];
	int* pNormalItemPlus		= new int[normalCount];		short* pNormalItemOption0		= new short[normalCount];
	short* pNormalItemOption1		= new short[normalCount];		short* pNormalItemOption2		= new short[normalCount];
	short* pNormalItemOption3		= new short[normalCount];		short* pNormalItemOption4		= new short[normalCount];

	for( i = 0; i < normalCount; i++ )
	{
		msg >> pNormalItemDBIndex[i]
			>> pNormalItemCount[i]
			>> strSerial[i]
			>> pNoramalitemWearPos[i]
			>> pNormalItemFlag[i]
			>> pNormalItemPlus[i]
			>> pNormalItemOption0[i]
			>> pNormalItemOption1[i]
			>> pNormalItemOption2[i]
			>> pNormalItemOption3[i]
			>> pNormalItemOption4[i];
	}

	for( i = 0; i < normalCount; i++ )
	{
		
		sql.Format( "REPLACE INTO ( a_user_idx, a_item_idx, a_plus, a_wear_pos, a_flag, a_serial, a_count, a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4 ) "
				" t_parcelroom%.2d VALUES ( %d, %d, %d, %d, %d, %s, %d, %d, %d, %d, %d, %d )", nCharIndex % 10, nCharIndex, pNormalItemDBIndex[i], 
				pNormalItemPlus[i], pNoramalitemWearPos[i], pNormalItemFlag[i], strSerial[i], pNormalItemCount[i], pNormalItemOption0[i], pNormalItemOption1[i], pNormalItemOption2[i], pNormalItemOption3[i], pNormalItemOption4[i] );

		cmd.SetQuery(sql);
		cmd.Update();
	}



	char cPackCount;
	msg >> cPackCount;

	if( cPackCount > 0 )
	{
		int* pPackItemDBIndex		= new int[cPackCount];
		LONGLONG* pPackItemCount	= new LONGLONG[cPackCount];
		char* strSerial				= new char[cPackCount];
		int* pNoramalitemWearPos	= new int[cPackCount];		int* pPackItemFlag		= new int[cPackCount];
		int* pPackItemPlus		= new int[cPackCount];		short* pPackItemOption0		= new short[cPackCount];
		short* pPackItemOption1		= new short[cPackCount];		short* pPackItemOption2		= new short[cPackCount];
		short* pPackItemOption3		= new short[cPackCount];		short* pPackItemOption4		= new short[cPackCount];

		for( i = 0; i < cPackCount; i++ )
		{
			msg >> pPackItemDBIndex[i]
				>> pPackItemCount[i]
				>> strSerial[i]
				>> pNoramalitemWearPos[i]
				>> pPackItemFlag[i]
				>> pPackItemPlus[i]
				>> pPackItemOption0[i]
				>> pPackItemOption1[i]
				>> pPackItemOption2[i]
				>> pPackItemOption3[i]
				>> pPackItemOption4[i];
		}

		for( i = 0; i < normalCount; i++ )
		{
			
			sql.Format( "REPLACE INTO ( a_user_idx, a_item_idx, a_plus, a_wear_pos, a_flag, a_serial, a_count, a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4 ) "
					" t_parcelroom%.2d VALUES ( %d, %d, %d, %d, %d, %s, %d, %d, %d, %d, %d, %d )", nCharIndex % 10, nCharIndex, pPackItemDBIndex[i],
					pPackItemPlus[i], pNoramalitemWearPos[i], pPackItemFlag[i], strSerial[i], pPackItemCount[i], pPackItemOption0[i], pPackItemOption1[i], 
					pPackItemOption2[i], pPackItemOption3[i], pPackItemOption4[i] );

			cmd.SetQuery(sql);
			cmd.Update();
		}
		delete [] pPackItemDBIndex;		delete [] pPackItemCount;		delete [] strSerial;
		delete [] pNoramalitemWearPos;	delete [] pPackItemFlag;		delete [] pPackItemPlus;		delete [] pPackItemOption0;
		delete [] pPackItemOption1;		delete []  pPackItemOption2;	delete [] pPackItemOption3;


	}
}
#endif // ALTERNATE_MERCHANT

#ifdef MONSTER_COMBO
void do_CommandCreateCombo( CNetMsg& msg, CDescriptor* dest )
{
	int subserver, charindex, areaindex, comboindex;
	CLCString sql(200);
	CDBCmd cmd;

	msg >> subserver
		>> charindex
		>> areaindex
		>> comboindex;

	sql.Format("INSERT INTO t_create_combo (`a_date`, `a_server`, `a_charindex`, `a_areaindex`, `a_comboindex`) VALUES ( NOW() , %d, %d, %d, %d )", 
		subserver, charindex, areaindex, comboindex);

	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	cmd.Update();
}
#endif // MONSTER_COMBO

#ifdef ATTACK_PET
void do_CommandAttackPet( CNetMsg& msg, CDescriptor* dest )
{
	char	subtype;
	int		ownerIndex;
	msg >> subtype
		>> ownerIndex;

	CLCString sql( 1024 );
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	CNetMsg rmsg;

	switch( subtype )
	{
	case MSG_HELPER_APET_CREATE_REQ :
		{
			int protoIndex;
			int index = 0;

			msg >> protoIndex;

			sql.Format( "SELECT a_index FROM t_apets ORDER BY a_index DESC");
			cmd.SetQuery( sql );
			if( cmd.Open() )
			{
				if( cmd.MoveFirst() )
				{
					cmd.GetRec("a_index",index);
					index++;
				}
				else
				{
					index =1;
				}
			}
			if( index > 0 )
			{
				sql.Format( "INSERT INTO t_apets ( a_index, a_owner, a_enable, a_proto_index, a_create_date, a_lastupdate_date, a_level )"
							"VALUES ( %d, %d, 3, %d, NOW(), NOW(), 1 )" , index , ownerIndex, protoIndex );
				cmd.SetQuery( sql );
				if( cmd.Update() )
				{
					// 성공 (n)dbindex, (n)pet_proto_index
					HelperAttackPet( rmsg, MSG_HELPER_APET_CREATE_OK, ownerIndex );
					rmsg << index << protoIndex;
					SEND_Q( rmsg, dest);
				}
				else
				{
					HelperAttackPet( rmsg, MSG_HELPER_APET_CREATE_FAILED, ownerIndex );
					rmsg << protoIndex;
					SEND_Q( rmsg, dest);
				}
			}
		}break;
	case MSG_HELPER_APET_ACCENPT_REQ :
		{
			int petIndex;
			msg >> petIndex;

			sql.Format( "UPDATE t_apets SET a_enable=1 WHERE a_enable=3 AND a_index=%d AND a_owner=%d", petIndex, ownerIndex );
			cmd.SetQuery( sql );
			if( cmd.Update() )
			{
				HelperAttackPet( rmsg, MSG_HELPER_APET_ACCENPT_OK, ownerIndex );
				SEND_Q( rmsg, dest);
			}
			else
			{
				HelperAttackPet( rmsg, MSG_HELPER_APET_ACCENPT_FAILED, ownerIndex );
				SEND_Q( rmsg, dest);
			}
		}break;
	case MSG_HELPER_APET_DELETE_REQ :
		{
			int		petIndex;
			char	enable;
			msg >> petIndex
				>> enable;
			
			sql.Format( "DELETE FROM t_apets WHERE a_enable=%d AND a_index=%d AND a_owner=%d", enable, petIndex, ownerIndex );
			cmd.SetQuery( sql );
			if( cmd.Update() )
			{

			}
		}break;
	}


}
#endif //ATTACK_PET

#ifdef EXTREME_CUBE
void do_CommandCubeStateReq( CNetMsg& msg, CDescriptor* dest )
{
	CDBCmd cmd;
	int charindex;
	char nRemainSendCount;
	const char nMaxSendCount = 10;
	int selfpoint = 0;
	CLCString sql(1000);
	CNetMsg rmsg;

	char SendCount;
	char TotalSendCount;
	int guildidx, cubepoint;
	char guildname[MAX_GUILD_NAME_LENGTH + 1], guildboss[MAX_CHAR_NAME_LENGTH + 1];
	CGuild* guild;
	CGuildMember* guildmember;
	char rank;

	msg >> charindex;

	cmd.Init(&gserver.m_dbchar);

	guildmember = gserver.m_guildlist.findmember(charindex);
	if(guildmember)
	{
		sql.Format("SELECT cp.* FROM t_cubepoint cp , t_guild g WHERE cp.a_guild_index = %d and cp.a_guild_index = g.a_index and g.a_enable = 1", guildmember->guild()->index());
		cmd.SetQuery(sql);
		if(cmd.Open() && cmd.MoveFirst())
		{
			cmd.GetRec("a_cubepoint", selfpoint);
		}
	}
	cmd.SetQuery("SELECT cp.* FROM t_cubepoint cp, t_guild g WHERE cp.a_guild_index = g.a_index and g.a_enable = 1 ORDER BY cp.a_cubepoint DESC, cp.a_update_date ASC LIMIT 10");
	if(!cmd.Open())
	{
		GAMELOG << init("CUBESTATE OPEN FAILED") << end;
		return ;
	}

	if(!cmd.MoveFirst())
	{
		// count 0을 보낸다
		rmsg.Init(MSG_HELPER_COMMAND);
		rmsg << MSG_HELPER_CUBESTATE_REP
			 << charindex
			 << selfpoint
			 << (char)0;

		SEND_Q(rmsg, dest);
		return;
	}

	rank = 0;
	nRemainSendCount = (char)cmd.m_nrecords;

	do
	{
		if(nMaxSendCount < nRemainSendCount)
			TotalSendCount = nMaxSendCount;

		TotalSendCount = nRemainSendCount;
		SendCount = 0;

		rmsg.Init(MSG_HELPER_COMMAND);
		rmsg << MSG_HELPER_CUBESTATE_REP
			 << charindex
			 << selfpoint
			 << TotalSendCount;
		do
		{
			cmd.GetRec("a_guild_index", guildidx);
			cmd.GetRec("a_cubepoint", cubepoint);

			memset(guildname, 0, sizeof(guildname));
			memset(guildboss, 0, sizeof(guildboss));

			guild = gserver.m_guildlist.findguild(guildidx);
			if(guild)
			{
				strcpy(guildname, guild->name());
				if(guild->boss())
					strcpy(guildboss, guild->boss()->GetName());
			}

			rank++;
			rmsg << rank
				 << guildname
				 << guildboss
				 << cubepoint;

			SendCount++;
		}while(cmd.MoveNext() && SendCount < TotalSendCount);

		if(TotalSendCount != SendCount)
			return ;

		SEND_Q(rmsg, dest);
		nRemainSendCount -= TotalSendCount;
	}while(nRemainSendCount > 0);
}

void do_CommandCubeStatePersonalReq( CNetMsg& msg, CDescriptor* dest )
{
	CDBCmd cmd;
	int charindex;
	char nRemainSendCount;
	const char nMaxSendCount = 10;
	int selfpoint = 0;
	CLCString sql(1000);

	CNetMsg rmsg;

	msg >> charindex;

	sql.Format("SELECT cp.* FROM t_cubepoint_personal cp, t_characters c WHERE c.a_index = %d and cp.a_char_idx = c.a_index", charindex);
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(!cmd.Open())
	{
		GAMELOG << init("CUBESTATE_PERSONAL OPEN FAILED") << "character index" << delim << charindex << end;
		return ;
	}

	if(cmd.MoveFirst())
	{
		cmd.GetRec("a_cubepoint", selfpoint);
	}

	sql.Format("SELECT cp.*, c.a_nick as a_nick FROM t_cubepoint_personal cp, t_characters c WHERE cp.a_char_idx = c.a_index and c.a_enable = 1 ORDER BY cp.a_cubepoint DESC, cp.a_update_date ASC LIMIT 10");
	cmd.SetQuery(sql);
	if(!cmd.Open())
	{
		GAMELOG << init("CUBESTATE_PERSONAL OPEN FAILED") << end;
		return ;
	}

	if(!cmd.MoveFirst())
	{
		rmsg.Init(MSG_HELPER_COMMAND);
		rmsg << MSG_HELPER_CUBESTATE_PERSONAL_REP
			 << charindex
			 << selfpoint
			 << (char)0;

		SEND_Q(rmsg, dest);
		return ;
	}

	nRemainSendCount = (char)cmd.m_nrecords;

	char SendCount;
	char TotalSendCount;
	int character, cubepoint;
	char guildname[MAX_GUILD_NAME_LENGTH + 1], charname[MAX_CHAR_NAME_LENGTH + 1];
	CLCString str(1000);
	CGuildMember* guildmember;
	char rank = 0;
	
	do
	{
		if(nMaxSendCount < nRemainSendCount)
			TotalSendCount = nMaxSendCount;

		TotalSendCount = nRemainSendCount;
		SendCount = 0;

		rmsg.Init(MSG_HELPER_COMMAND);
		rmsg << MSG_HELPER_CUBESTATE_PERSONAL_REP
			 << charindex
			 << selfpoint
			 << TotalSendCount;

		do
		{
			cmd.GetRec("a_char_idx", character);
			cmd.GetRec("a_cubepoint", cubepoint);
			cmd.GetRec("a_nick", str);

			memset(guildname, 0, sizeof(guildname));
			memset(charname, 0, sizeof(charname));

			guildmember = gserver.m_guildlist.findmember(character);
			if(guildmember && guildmember->guild())
			{
				strcpy(guildname, guildmember->guild()->name());
			}

			strcpy(charname, (const char* )str);

			rank++;
			rmsg << rank
				 << guildname
				 << charname
				 << cubepoint;

			SendCount++;
		}while(cmd.MoveNext() && SendCount < TotalSendCount);

		if(TotalSendCount != SendCount)
			return ;

		SEND_Q(rmsg, dest);
		nRemainSendCount -= TotalSendCount;
	}while(nRemainSendCount > 0);
}

void do_CommandAddCubePoint( CNetMsg& msg, CDescriptor* dest )
{
	CLCString sql(1000);
	int ofguildindex, ofcubepoint;
	long time;
	msg >> ofguildindex 
		>> ofcubepoint 
		>> time;

	sql.Format("INSERT INTO t_cubepoint ( `a_guild_index`, `a_cubepoint`, `a_update_date` ) VALUES (%d, %d, NOW())", 
			ofguildindex, ofcubepoint);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(cmd.Update())
	{
		GAMELOG << init("Increase CubePoint")
				<< "Guild Index" << delim
				<< ofguildindex << delim
				<< "CubePoint" << delim
				<< ofcubepoint << end;
	}
	else
	{
		sql.Format("UPDATE t_cubepoint SET a_cubepoint = a_cubepoint + %d, a_update_date = NOW() WHERE a_guild_index = %d", ofcubepoint, ofguildindex);
		cmd.SetQuery(sql);
		if(cmd.Update())
		{
			GAMELOG << init("Increase CubePoint")
					<< "Guild Index" << delim
					<< ofguildindex << delim
					<< "CubePoint" << delim
					<< ofcubepoint << end;
		}
		else
		{
			// 실패하면 로그 찍기
			GAMELOG << init("Failed Increase CubePoint")
					<< "Guild Index" << delim
					<< ofguildindex << delim
					<< "CubePoint" << delim
					<< ofcubepoint << end;
		}
	}
}

void do_CommandAddCubePointPersonal( CNetMsg& msg, CDescriptor* dest )
{
	CLCString sql(1000);
	int ofcharidx, ofcubepoint;
	long time;
	msg >> ofcharidx 
		>> ofcubepoint 
		>> time;

	sql.Format("INSERT INTO t_cubepoint_personal ( `a_char_idx`, `a_cubepoint`, `a_update_date` ) VALUES (%d, %d, NOW())", 
			ofcharidx, ofcubepoint);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(cmd.Update())
	{
		GAMELOG << init("Increase CubePoint Personal")
				<< "CharIdx" << delim
				<< ofcharidx << delim
				<< "CubePoint" << delim
				<< ofcubepoint << end;
	}
	else
	{
		sql.Format("UPDATE t_cubepoint_personal SET a_cubepoint = a_cubepoint + %d, a_update_date = NOW() WHERE a_char_idx = %d", ofcubepoint, ofcharidx);
		cmd.SetQuery(sql);
		if(cmd.Update())
		{
			GAMELOG << init("Increase CubePoint Personal")
					<< "CharIdx" << delim
					<< ofcharidx << delim
					<< "CubePoint" << delim
					<< ofcubepoint << end;
		}
		else
		{
			// 실패하면 로그 찍기
			GAMELOG << init("Failed Increase CubePoint Personal")
					<< "CharIdx" << delim
					<< ofcharidx << delim
					<< "CubePoint" << delim
					<< ofcubepoint << end;
		}
	}
}
#endif // EXTREME_CUBE 

#ifdef EVENT_PHOENIX	// //피닉스 이벤트 yhj	
void do_CommandEventPhoenix( CNetMsg& msg, CDescriptor* dest )
{
	int nCharIdx = 0, nUserIdx = 0;
	msg >> nUserIdx
		>> nCharIdx;

	CNetMsg rmsg;

	CDBCmd DB_cmd1;
	CDBCmd DB_cmd2;	
	CDBCmd DB_cmd3;	
	int query_count1 = 0;
	DB_cmd1.Init(&gserver.m_dbchar);
	DB_cmd2.Init(&gserver.m_dbchar);
	DB_cmd3.Init(&gserver.m_dbchar);
	
	// 100 레벨이상인 캐릭터가 있다는 것을 표시
	bool level_100_up = false;

	// 자신의 계정 내에 모든 케릭터의 인덱스를 가져온다.
	sprintf(g_buf, "SELECT a_index FROM t_characters WHERE a_user_index=%d ", nUserIdx );
	DB_cmd1.SetQuery(g_buf);
	if (!DB_cmd1.Open())
		return;
	
	query_count1 = DB_cmd1.m_nrecords;
	if (query_count1)
	{
		while (DB_cmd1.MoveNext())
		{
			int index;
			int level;
		
			DB_cmd1.GetRec("a_index", index);
			
			// 각 캐릭터의 레벨을 하나씩 가져온다.
			sprintf(g_buf, "SELECT a_level FROM t_characters WHERE a_index=%d ", index );
			
			DB_cmd2.SetQuery(g_buf);
			if (!DB_cmd2.Open())
				continue;

			DB_cmd2.MoveFirst();
			DB_cmd2.GetRec("a_level", level);

			// 피닉스 이벤트 승락, 레벨이 100 이상인 캐릭터가 하나라도 있으면
			if( level >= 100 ) 
			{
				level_100_up = true;
				break;
			}

		}
	}
	else
	{
		// 계정의 캐릭터를 불러오지 못해서 자격 미달임
		HelperEventPhoenixMsg(rmsg, MSG_HELPER_EVENT_PHOENIX_ERROR_REQUIREMENT, nCharIdx);
		SEND_Q(rmsg, dest);
		return;
	}

	// 100 레벨 캐릭터가 하나라도 있다면
	if ( level_100_up )
	{
		int phoenix;

		// 캐릭터가 피닉스 이벤트 이후 생성된 캐릭터 이라면 피닉스가 가능
		sprintf(g_buf, "SELECT a_phoenix FROM t_characters WHERE a_index=%d ", nCharIdx );
		DB_cmd3.SetQuery(g_buf);
		if (!DB_cmd3.Open())
		{
			// 데이터를 가져오지 못해서 자격 미달임
			HelperEventPhoenixMsg(rmsg, MSG_HELPER_EVENT_PHOENIX_ERROR_REQUIREMENT, nCharIdx);
			SEND_Q(rmsg, dest);
			return;
		}

		DB_cmd3.MoveFirst();
		DB_cmd3.GetRec("a_phoenix", phoenix);

		// 피닉스 이벤트 이후 생성된 캐릭터 이라면
		if ( phoenix == 0 )
		{
			// 자격이 있음
			HelperEventPhoenixMsg(rmsg, MSG_HELPER_EVENT_PHOENIX_OK, nCharIdx);
			SEND_Q(rmsg, dest);
			return;
		}
		else
		{
			// 기존의 캐릭터라서 피닉스 자격이 없음
			HelperEventPhoenixMsg(rmsg, MSG_HELPER_EVENT_PHOENIX_ERROR_REQUIREMENT, nCharIdx);
			SEND_Q(rmsg, dest);
			return;
		}

	}
	else
	{
		// 레벨이 100 이상인 캐릭터가 하나도 없어서 자격 미달임
		HelperEventPhoenixMsg(rmsg, MSG_HELPER_EVENT_PHOENIX_ERROR_REQUIREMENT, nCharIdx);
		SEND_Q(rmsg, dest);
		return;
	}
	
}

#endif // EVENT_PHOENIX		


#ifdef TRADE_AGENT	// 거래 대행

#define  MAX_ONEPAGE_LISTNUM	10
#define  MAX_CHAR_REG_COUNT		10

// 등록 리스트 요청
void do_Command_TradeAgent_RegListReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	char sql[1024];
	int nCharIndex;
	int nPageNo, nAlignType;

	msg >> nCharIndex >> nPageNo >> nAlignType;

	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );

	memset(sql,0,sizeof(sql));
	sprintf(sql,"SELECT * , TO_DAYS( DATE_ADD(a_regist_date, INTERVAL 5 DAY)) - TO_DAYS(now()) as a_FinishDay, "
				" HOUR(DATE_ADD(a_regist_date, INTERVAL 5 DAY)) - HOUR(now()) as a_FinishHour, " 
				" MINUTE(DATE_ADD(a_regist_date, INTERVAL 5 DAY)) - MINUTE(now()) as a_FinishMin "
				" FROM t_tradeagent WHERE a_sell_charindex = %d AND a_state = %d AND (NOW() >= a_regist_date "
				" AND NOW() < DATE_ADD(a_regist_date, INTERVAL 5 DAY)) " , 
				nCharIndex, MSG_TRADEAGENT_STATE_SELL_BEFORE );  

	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, nCharIndex,MSG_TRADEAGENT_ERROR_PART_REGIST );
		SEND_Q( rmsg, dest );
		return;
	}

	int nTotItemCount  = 0;		
	nTotItemCount  =	cmd.m_nrecords;		// PC가 등록한 총 아이템수 

	//페이지 계산 
	//-->
	int nMaxPageNo = nTotItemCount / MAX_ONEPAGE_LISTNUM;
	if(nTotItemCount % MAX_ONEPAGE_LISTNUM != 0) nMaxPageNo++;

	if(nPageNo > nMaxPageNo)
	{
		//에러:페이지 번호 초과
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, nCharIndex,MSG_TRADEAGENT_ERROR_PART_REGIST );
		SEND_Q( rmsg, dest );
	}
	
	int nOffset = (MAX_ONEPAGE_LISTNUM * (nPageNo - 1)); 
 	//<--

	if(!cmd.Seek(nOffset))
	{
		//에러:해당 데이타가 존재 하지 않음
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, nCharIndex,MSG_TRADEAGENT_ERROR_PART_REGIST );
		SEND_Q( rmsg, dest );
		return;
	}

	//실제 처리 수 계산
	int nRealItemCount = 0;
	int iMok,iMod;
	iMok = iMod = 0;

	iMok = (int)(nTotItemCount / MAX_ONEPAGE_LISTNUM);
	iMod = (int)(nTotItemCount % MAX_ONEPAGE_LISTNUM);

	if(iMok > 0 && iMod > 0)
	{
		if(nPageNo >= 1 && nPageNo <= iMok) 
		{
			nRealItemCount = MAX_ONEPAGE_LISTNUM;
		}else
		{
			nRealItemCount = iMod;
		}		
	}else if(iMok > 0 && iMod == 0)
	{
		nRealItemCount = MAX_ONEPAGE_LISTNUM;
	}else if(iMok == 0 && iMod > 0)
	{
		nRealItemCount = iMod;
	}

	int* pListindex				= new int[nRealItemCount];			// 리스트 인덱스
	int* pItemSerial			= new int[nRealItemCount];			// 아이템 시리얼 
	int* pItemPlus				= new int[nRealItemCount];			// 아이템 플러스
	int* pItemFlag				= new int[nRealItemCount];			// 아이템 플래그
	S_ITEMOPTION *pItemOption	= new S_ITEMOPTION[nRealItemCount];	// 아이템 옵션
	int* pQuantity				= new int[nRealItemCount];			// 수량 
	LONGLONG* ptotalmoney		= new LONGLONG[nRealItemCount];		// 총액 

	int* pFinishDay				= new int[nRealItemCount];			// 마감 일
	int* pFinishHour			= new int[nRealItemCount];			// 마감 시간
	int* pFinisMin				= new int[nRealItemCount];			// 마감 분
	int* pFinishDayUnit			= new int[nRealItemCount];			// 마감 단위 
	
	int i = 0;
	do
	{
		cmd.GetRec( "a_index"		, pListindex[i] );
		cmd.GetRec( "a_item_serial"	, pItemSerial[i] );
		cmd.GetRec( "a_item_plus"	, pItemPlus[i] );
		cmd.GetRec( "a_item_flag"	, pItemFlag[i] );

		cmd.GetRec( "a_option_count"	, pItemOption[i].m_nOptionCnt);
		cmd.GetRec( "a_option_0_type"	, pItemOption[i].m_Option_type[0]);
		cmd.GetRec( "a_option_0_level"	, pItemOption[i].m_Option_level[0]);
		cmd.GetRec( "a_option_1_type"	, pItemOption[i].m_Option_type[1]);
		cmd.GetRec( "a_option_1_level"	, pItemOption[i].m_Option_level[1]);
		cmd.GetRec( "a_option_2_type"	, pItemOption[i].m_Option_type[2]);
		cmd.GetRec( "a_option_2_level"	, pItemOption[i].m_Option_level[2]);
		cmd.GetRec( "a_option_3_type"	, pItemOption[i].m_Option_type[3]);
		cmd.GetRec( "a_option_3_level"	, pItemOption[i].m_Option_level[3]);
		cmd.GetRec( "a_option_4_type"	, pItemOption[i].m_Option_type[4]);
		cmd.GetRec( "a_option_4_level"	, pItemOption[i].m_Option_level[4]);

		cmd.GetRec( "a_quantity"	, pQuantity[i] );
		cmd.GetRec( "a_totalmoney"	, ptotalmoney[i] );

		cmd.GetRec( "a_FinishDay"	, pFinishDay[i] );
		cmd.GetRec( "a_FinishHour"	, pFinishHour[i] );
		cmd.GetRec( "a_FinishMin"	, pFinisMin[i] );

		//마감일, 단위 결정
		if(pFinishDay[i] > 0) 
		{
			pFinishDayUnit[i]	=	MSG_TRADEAGENT_FINISH_DAY;
		}else if(pFinishDay[i] == 0 && pFinishHour[i] > 0) 
		{
			pFinishDay[i]		=	pFinishHour[i]; 	  				
			pFinishDayUnit[i]	=	MSG_TRADEAGENT_FINISH_HOUR;
		}else if(pFinishDay[i] == 0 && pFinishHour[i] == 0 && pFinisMin[i] > 0 ) 
		{
			pFinishDay[i]		=	pFinisMin[i]; 	  				
			pFinishDayUnit[i]	=	MSG_TRADEAGENT_FINISH_MIN;
		}

		i++;

		if(i >= nRealItemCount) break;

	}while( cmd.MoveNext());


	HelperTradeAgentRegListRepMsg(rmsg, nCharIndex, nTotItemCount, nMaxPageNo, nPageNo, nRealItemCount, pListindex, pItemSerial,pItemPlus, pItemFlag,pItemOption,pQuantity,ptotalmoney,pFinishDay,pFinishDayUnit);
	SEND_Q( rmsg, dest );

	delete[] pListindex;
	delete[] pItemSerial;
	delete[] pItemPlus;
	delete[] pItemFlag;
	delete[] pItemOption;
	delete[] pQuantity;
	delete[] ptotalmoney;
	delete[] pFinishDay;  
	delete[] pFinishHour; 
	delete[] pFinisMin; 
	delete[] pFinishDayUnit;
}

// 등록 
void do_Command_TradeAgent_RegReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	char sql[1024];
	char  tab, row, col;
	int ncharindex = -1; 
	int nitem_serial, nitem_plus, nitem_flag, nitem_level,nitem_type,nitem_subtype,nitem_class,nQuantity;
	CLCString item_name(MAX_ITEM_NAME_LENGTH + 1),sellchar_name(MAX_CHAR_NAME_LENGTH + 1);
	CLCString item_serial2(MAX_SERIAL_LENGTH + 1);
	LONGLONG llTotalMoney, llGuaranty;
	LONGLONG llCurCharHaveNas=0;

	S_ITEMOPTION sItemOption;

	msg >> ncharindex 
		>> sellchar_name
		>> tab >> row >> col
		>> nitem_serial 
		>> item_serial2
		>> nitem_plus
		>> nitem_flag
		>> sItemOption.m_nOptionCnt;

	for (int i = 0; i < sItemOption.m_nOptionCnt; i++)
	{
		msg >> sItemOption.m_Option_type[i]
			>> sItemOption.m_Option_level[i];
	}
		
	msg	>> item_name  >> nitem_level >> nitem_type 	>> nitem_subtype >> nitem_class >> nQuantity   
		>> llTotalMoney >> llGuaranty
		>> llCurCharHaveNas;

	CDBCmd cmdReg;
	cmdReg.Init(&gserver.m_dbchar);

	//캐릭터별 최대 등록 수 체크 
	//-->
	memset(sql,0,sizeof(sql));
	sprintf(sql,"SELECT count(*) AS CharMaxRegCount FROM t_tradeagent "
		        "WHERE a_sell_charindex = %d AND a_state = %d "
				" AND (NOW() >= a_regist_date AND NOW() < DATE_ADD(a_regist_date, INTERVAL 5 DAY)) " 
				,ncharindex
				,MSG_TRADEAGENT_STATE_SELL_BEFORE);  

	cmdReg.SetQuery( sql );
	if( !cmdReg.Open() || !cmdReg.MoveFirst() )
	{
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, ncharindex,MSG_TRADEAGENT_ERROR_PART_REGIST );
		SEND_Q( rmsg, dest );
		return;
	}

	int	nCharMaxRegCount = 0;

	cmdReg.GetRec( "CharMaxRegCount"	, nCharMaxRegCount );

	if(nCharMaxRegCount >= MAX_CHAR_REG_COUNT)
	{
		//오류: 캐릭터 최대 등록 수 초과
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_CHAR_MAXREGCOUNT_FULL, ncharindex,MSG_TRADEAGENT_ERROR_PART_REGIST );
		SEND_Q( rmsg, dest );
		return;
	}	
	//<--

	//서버 최대 등록 수 체크 
	//-->
	if(gserver.m_TradeAgentList.GetCurList() >= MAX_TRADEAGENT_LIST)
	{
		//오류: 최대 등록 수 초과
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_MAXREGCOUNT_FULL, ncharindex,MSG_TRADEAGENT_ERROR_PART_REGIST );
		SEND_Q( rmsg, dest );
		return;
	}
	//<--
	
	//기존 보증금 무시 재산출 (추가)
	//-->
	llGuaranty = (nCharMaxRegCount + 1) * 1000;

	//보증금 체크 
	if(llCurCharHaveNas < llGuaranty)
	{
		//오류:보증금 부족 
		HelperTradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NOT_ENUGH_GUARANTY, ncharindex, MSG_TRADEAGENT_ERROR_PART_REGIST);
		SEND_Q( rmsg, dest );
		return;
	}
	//<--

	//메모리에서 등록 기간 체크를 위한
	//-->
	CTRADEAGENT* pTradeAgent = new CTRADEAGENT();
	if(pTradeAgent)
	{
		pTradeAgent->SetItemReg(nitem_serial,ncharindex);
		pTradeAgent->SetItemRegFinishDay();		//등록 마감일
	}
	//<--
	
	//아이템명 (')작은 따옴표 처리
	item_name.Replace("'","''");

	memset(sql,0,sizeof(sql));
#ifdef CIRCLE_WINDOWS
	sprintf(sql,"INSERT DELAYED INTO t_tradeagent(a_item_serial, a_item_serial2, a_item_plus, a_item_flag, a_item_name, "
			   " a_item_level, a_item_type, a_item_subtype, a_item_class, a_quantity, a_totalmoney, "
			   " a_regist_date, a_sell_charindex, a_sell_charname, a_state, a_guaranty , a_option_count, a_option_0_type, "
			   " a_option_0_level,a_option_1_type, a_option_1_level,a_option_2_type, a_option_2_level, "
			   " a_option_3_type, a_option_3_level,a_option_4_type, a_option_4_level) "
			   "VALUES(%d, '%s', %d, %d, '%s', %d, %d, %d, %d, %d, %I64d, NOW() ,%d, '%s', %d, %I64d, %d, %d, %d, %d,"
			   " %d, %d, %d, %d, %d , %d, %d)", 
				nitem_serial, (const char*)item_serial2, nitem_plus, nitem_flag, (const char*)item_name, nitem_level, nitem_type, nitem_subtype, nitem_class, nQuantity, llTotalMoney, ncharindex, (const char*)sellchar_name,MSG_TRADEAGENT_STATE_SELL_BEFORE, llGuaranty, sItemOption.m_nOptionCnt,sItemOption.m_Option_type[0], sItemOption.m_Option_level[0], sItemOption.m_Option_type[1], sItemOption.m_Option_level[1], sItemOption.m_Option_type[2], sItemOption.m_Option_level[2],sItemOption.m_Option_type[3], sItemOption.m_Option_level[3],sItemOption.m_Option_type[4], sItemOption.m_Option_level[4]);

#else
	sprintf(sql,"INSERT DELAYED INTO t_tradeagent(a_item_serial, a_item_serial2, a_item_plus, a_item_flag, a_item_name, "
			   " a_item_level, a_item_type, a_item_subtype, a_item_class, a_quantity, a_totalmoney, "
			   " a_regist_date, a_sell_charindex, a_sell_charname, a_state, a_guaranty , a_option_count, a_option_0_type, "
			   " a_option_0_level,a_option_1_type, a_option_1_level,a_option_2_type, a_option_2_level, "
			   " a_option_3_type, a_option_3_level,a_option_4_type, a_option_4_level) "
			   "VALUES(%d, '%s', %d, %d, '%s', %d, %d, %d, %d, %d, %lld, NOW() ,%d, '%s', %d, %lld, %d, %d, %d, %d,"
			   " %d, %d, %d, %d, %d , %d, %d)", 
				nitem_serial, (const char*)item_serial2, nitem_plus, nitem_flag, (const char*)item_name, nitem_level, nitem_type, nitem_subtype, nitem_class, nQuantity, llTotalMoney, ncharindex, (const char*)sellchar_name,MSG_TRADEAGENT_STATE_SELL_BEFORE, llGuaranty, sItemOption.m_nOptionCnt,sItemOption.m_Option_type[0], sItemOption.m_Option_level[0], sItemOption.m_Option_type[1], sItemOption.m_Option_level[1], sItemOption.m_Option_type[2], sItemOption.m_Option_level[2],sItemOption.m_Option_type[3], sItemOption.m_Option_level[3],sItemOption.m_Option_type[4], sItemOption.m_Option_level[4]);
#endif



	cmdReg.SetQuery(sql);
	if(cmdReg.Update())
	{
		//-->
		if(pTradeAgent)
		{
			//db index 설정
			int nDbIndex = -1;
			pTradeAgent->SetDbIndex(nDbIndex); 

			//리스트 추가 
			gserver.m_TradeAgentList.Add(pTradeAgent);

			// 상품명, 수량, 총금액, 보증금
			HelperTradeAgentRegRepMsg(rmsg, ncharindex, tab, row, col, nitem_serial,(const char*)item_serial2, (const char*)item_name, nQuantity, llTotalMoney, llGuaranty,nDbIndex);
			SEND_Q( rmsg, dest );
		}
		//<--
	}
	else
	{
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NORMAL, ncharindex,MSG_TRADEAGENT_ERROR_PART_REGIST );
		SEND_Q( rmsg, dest );

		// 실패하면 로그 찍기
		GAMELOG << init("Fail Regist TradeAgent")
				<< "charindex" << delim
				<< ncharindex << delim
				<< "item_serial" << delim
				<< nitem_serial << end;

		//쿼리 실패 시 
		//-->
		if(pTradeAgent)  delete pTradeAgent;
		//<--
	}
	
}

//등록 결과 처리
void do_Command_TradeAgent_RegResult( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int ncharindex, nDbIndex,nResult; 
	CTRADEAGENT* pTradeAgent = NULL;

	ncharindex =  nDbIndex = nResult = -1;

	msg >> ncharindex 
		>> nDbIndex
		>> nResult;

	CLCString	sql( 1024 );
	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );

	if(nDbIndex > 0 && nResult == 0)		//Game 서버에서 정산 처리 된 것만
	{
			// 갱신: 상태를 정산완료,정산일시 처리
			//-->
			sql.Format("UPDATE t_tradeagent SET a_state = -1 WHERE a_index = %d "
						,nDbIndex);

			cmd.SetQuery(sql);
			if(!cmd.Update())
			{
				// 실패하면 로그 찍기
				GAMELOG << init("Fail  RegResult TradeAgent DB Update")
						<< "ncharindex" << delim
						<< ncharindex << delim
						<< "nDbIndex" << delim
						<< nDbIndex << end;
			}else
			{
				// 성공 하면 로그 찍기
				GAMELOG << init("Sucess RegResult DB Update")
						<< "ncharindex" << delim
						<< ncharindex << delim
						<< "nDbIndex" << delim
						<< nDbIndex << end;

			}

			//메모리 제거 
			pTradeAgent = gserver.m_TradeAgentList.FindDb(nDbIndex);
			if(pTradeAgent)
			{
				gserver.m_TradeAgentList.Remove(pTradeAgent);	
			}
	}

}

// 등록 취소
void do_Command_TradeAgent_RegCancelReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int ncharindex, nCanelindex;
	CLCString sql(1024);

	msg >> ncharindex >> nCanelindex; 

	// 조회 
	//-->
	sql.Format("SELECT * FROM t_tradeagent WHERE a_index = %d and a_sell_charindex = %d and a_state = %d"
		        ,nCanelindex, ncharindex,MSG_TRADEAGENT_STATE_SELL_BEFORE);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		//에러:해당 데이타가 존재 하지 않음
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, ncharindex,MSG_TRADEAGENT_ERROR_PART_REGIST );
		SEND_Q( rmsg, dest );
		return;
	}
	//<--

	int nQuantity = 0;
	CLCString stItemName(MAX_ITEM_NAME_LENGTH + 1);

	cmd.GetRec( "a_item_name"		, stItemName );
	cmd.GetRec( "a_quantity"		, nQuantity );

	// 갱신: 상태를 반송(2),처리 일시 처리
	//-->
	sql.Format("UPDATE t_tradeagent SET a_state = %d , a_process_date = now() WHERE a_index = %d"
		        ,MSG_TRADEAGENT_STATE_SELL_RETURNED,nCanelindex);

	CDBCmd cmd2;
	cmd2.Init(&gserver.m_dbchar);
	cmd2.SetQuery(sql);
	if(cmd2.Update())
	{
		// 상품명 
		HelperTradeAgentRegCancelRepMsg(rmsg, ncharindex,stItemName,nQuantity);
		SEND_Q( rmsg, dest );

		//쿼리 성공시 
		//-->
		gserver.m_TradeAgentList.SetState(nCanelindex,MSG_TRADEAGENT_STATE_SELL_RETURNED);
		//<--

	}else
	{
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NORMAL, ncharindex,MSG_TRADEAGENT_ERROR_PART_REGIST );
		SEND_Q( rmsg, dest );

		// 실패하면 로그 찍기
		GAMELOG << init("Fail  Cancel Regist TradeAgent")
				<< "charindex" << delim
				<< ncharindex << delim
				<< "nCanelindex" << delim
				<< nCanelindex << end;
	}
	//<--
}

// 조회(검색)
void do_Command_TradeAgent_SearchReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	char sql[1024],sqltemp[1024];
	int	ncharindex,nitemType,nitemSubType,nJobClass;
	CLCString stSearchWord(20 + 1);
	int nPageNo,nAlignType;

	msg >> ncharindex 
		>> nitemType >> nitemSubType >> nJobClass
		>> stSearchWord 
		>> nPageNo
		>> nAlignType; 

	//nitemType:전체(-1) nitemSubType:전체(-1)  nJobClass:전체(-1) nAlignType:선택되지 않음(-1)

	memset(sql,0,sizeof(sql));
	sprintf(sql, "SELECT * FROM t_tradeagent WHERE a_state = %d AND a_sell_charindex != %d "
		         "AND (NOW() >= a_regist_date AND NOW() < DATE_ADD(a_regist_date, INTERVAL 5 DAY))", 
		         MSG_TRADEAGENT_STATE_SELL_BEFORE, ncharindex);

	if(nitemType >= 0)	  
	{
		memset(sqltemp,0,sizeof(sqltemp));
		sprintf(sqltemp, "AND a_item_type = %d " ,nitemType);
		strcat(sql , sqltemp);
	}

	if(nitemSubType >= 0)
	{
		memset(sqltemp,0,sizeof(sqltemp));
		sprintf(sqltemp, "AND a_item_subtype = %d " ,nitemSubType);
		strcat(sql , sqltemp);
	}

	if(nJobClass >= 0)
	{
		memset(sqltemp,0,sizeof(sqltemp));
		sprintf(sqltemp, "AND (a_item_class & (1 << %d)) " ,nJobClass);
		strcat(sql , sqltemp);
	}

	if(stSearchWord.Length() > 0)
	{
		memset(sqltemp,0,sizeof(sqltemp));
		sprintf(sqltemp, "AND a_item_name LIKE '%%%s%%' " , (const char *)stSearchWord);
		strcat(sql , sqltemp);
	}

	switch(nAlignType)
	{
		case -1:  
				strcat(sql ," ORDER BY a_item_level, a_item_serial, a_regist_date ASC");  break;

		case MSG_TRADEAGENT_ALIGN_NAME_ASC:  
			    strcat(sql ," ORDER BY  a_item_name ASC");  break;
		case MSG_TRADEAGENT_ALIGN_NAME_DESC:  
			    strcat(sql ," ORDER BY  a_item_name DESC");  break;

		case MSG_TRADEAGENT_ALIGN_QUANTITY_ASC:  
			    strcat(sql ," ORDER BY a_quantity ASC ");  break;
		case MSG_TRADEAGENT_ALIGN_QUANTITY_DESC:  
			    strcat(sql ," ORDER BY a_quantity DESC ");  break;
		
		case MSG_TRADEAGENT_ALIGN_TOTALMONEY_ASC:  
			    strcat(sql ," ORDER BY a_totalmoney ASC ");  break;
		case MSG_TRADEAGENT_ALIGN_TOTALMONEY_DESC:  
			    strcat(sql ," ORDER BY a_totalmoney DESC ");  break;

		case MSG_TRADEAGENT_ALIGN_LEVEL_ASC:  
			    strcat(sql ," ORDER BY a_item_level ASC ");  break;
		case MSG_TRADEAGENT_ALIGN_LEVEL_DESC:  
			    strcat(sql ," ORDER BY a_item_level DESC ");  break;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		//에러:해당 데이타가 존재 하지 않음
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, ncharindex,MSG_TRADEAGENT_ERROR_PART_SEARCH );
		SEND_Q( rmsg, dest );
		return;
	}

	int nTotItemCount = 0;
	nTotItemCount =	cmd.m_nrecords;		// 등록된 총 아이템수 

	//페이지 계산 
	//-->
	int nMaxPageNo = nTotItemCount / MAX_ONEPAGE_LISTNUM;
	if(nTotItemCount % MAX_ONEPAGE_LISTNUM != 0) nMaxPageNo++;

	if(nPageNo > nMaxPageNo)
	{
		//에러: 페이지 번호 초과
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, ncharindex,MSG_TRADEAGENT_ERROR_PART_SEARCH );
		SEND_Q( rmsg, dest );
	}
	
	int nOffset = (MAX_ONEPAGE_LISTNUM * (nPageNo - 1)); 
 	//<--

	if(!cmd.Seek(nOffset))
	{
		//에러: 해당 데이타가 존재 하지 않음
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, ncharindex,MSG_TRADEAGENT_ERROR_PART_SEARCH );
		SEND_Q( rmsg, dest );
		return;
	}

	//실제 처리 수 계산
	int nRealItemCount = 0;
	int iMok,iMod;
	iMok = iMod = 0;

	iMok = (int)(nTotItemCount / MAX_ONEPAGE_LISTNUM);
	iMod = (int)(nTotItemCount % MAX_ONEPAGE_LISTNUM);

	if(iMok > 0 && iMod > 0)
	{
		if(nPageNo >= 1 && nPageNo <= iMok) 
		{
			nRealItemCount = MAX_ONEPAGE_LISTNUM;
		}else
		{
			nRealItemCount = iMod;
		}		
	}else if(iMok > 0 && iMod == 0)
	{
		nRealItemCount = MAX_ONEPAGE_LISTNUM;
	}else if(iMok == 0 && iMod > 0)
	{
		nRealItemCount = iMod;
	}

	int* pListindex				= new int[nRealItemCount];			// 리스트 인덱스
	int* pItemSerial			= new int[nRealItemCount];			// 아이템 시리얼 
	int* pItemPlus				= new int[nRealItemCount];			// 아이템 플러스
	int* pItemFlag				= new int[nRealItemCount];			// 아이템 플래그
	S_ITEMOPTION *pItemOption	= new S_ITEMOPTION[nRealItemCount];	// 아이템 옵션
	int* pQuantity				= new int[nRealItemCount];			// 수량 
	LONGLONG* ptotalmoney		= new LONGLONG[nRealItemCount];		// 총액 
	int* pLevel					= new int[nRealItemCount];			// 레벨
	CLCString *pSellCharname	= new CLCString[nRealItemCount];	// 판매자 아이디

	int i = 0;
	do
	{
		cmd.GetRec( "a_index"		, pListindex[i] );
		cmd.GetRec( "a_item_serial"	, pItemSerial[i] );
		cmd.GetRec( "a_item_plus"	, pItemPlus[i]);
		cmd.GetRec( "a_item_flag"	, pItemFlag[i]);

		cmd.GetRec( "a_option_count"	, pItemOption[i].m_nOptionCnt);
		cmd.GetRec( "a_option_0_type"	, pItemOption[i].m_Option_type[0]);
		cmd.GetRec( "a_option_0_level"	, pItemOption[i].m_Option_level[0]);
		cmd.GetRec( "a_option_1_type"	, pItemOption[i].m_Option_type[1]);
		cmd.GetRec( "a_option_1_level"	, pItemOption[i].m_Option_level[1]);
		cmd.GetRec( "a_option_2_type"	, pItemOption[i].m_Option_type[2]);
		cmd.GetRec( "a_option_2_level"	, pItemOption[i].m_Option_level[2]);
		cmd.GetRec( "a_option_3_type"	, pItemOption[i].m_Option_type[3]);
		cmd.GetRec( "a_option_3_level"	, pItemOption[i].m_Option_level[3]);
		cmd.GetRec( "a_option_4_type"	, pItemOption[i].m_Option_type[4]);
		cmd.GetRec( "a_option_4_level"	, pItemOption[i].m_Option_level[4]);

		cmd.GetRec( "a_quantity"		, pQuantity[i]);
		cmd.GetRec( "a_totalmoney"		, ptotalmoney[i]);
		cmd.GetRec( "a_item_level"		, pLevel[i] );
		cmd.GetRec( "a_sell_charname"	, pSellCharname[i]);

		i++;

		if(i >= nRealItemCount) break;

	}while( cmd.MoveNext());

	                                                                                                                               
	HelperTradeAgentSearchRepMsg(rmsg, ncharindex, nMaxPageNo, nPageNo, nRealItemCount, pListindex, pItemSerial,pItemPlus,pItemFlag,pItemOption,pQuantity,ptotalmoney,pLevel,pSellCharname);
	SEND_Q( rmsg, dest );

	delete[] pListindex;
	delete[] pItemSerial;
	delete[] pItemPlus;
	delete[] pItemFlag;
    delete[] pItemOption;
 	delete[] pQuantity;
	delete[] ptotalmoney;
	delete[] pLevel;
	delete[] pSellCharname;
}

// 구매
void do_Command_TradeAgent_BuyReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	CLCString sql(1024),buychar_name(MAX_CHAR_NAME_LENGTH + 1);
	int ncharindex, nList_idx;
	LONGLONG llCurCharHaveNas =0;

	msg >> ncharindex
		>> buychar_name
		>> nList_idx
		>> llCurCharHaveNas;

	// 조회 
	//-->
	sql.Format("SELECT * FROM t_tradeagent WHERE a_index = %d" ,nList_idx);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		//에러: 해당 데이타가 존재 하지 않음
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, ncharindex,MSG_TRADEAGENT_ERROR_PART_SEARCH );
		SEND_Q( rmsg, dest );
		return;
	}
	//<--

	//판매한 캐릭터 정보 획득
	int nSellcharindex  = -1;
	LONGLONG TotalMoney = 0;
	int	nState			= -1;		
	int nQuantity		= 0;
	CLCString stItemName(MAX_ITEM_NAME_LENGTH + 1);

	cmd.GetRec( "a_item_name"			, stItemName );
	cmd.GetRec( "a_quantity"			, nQuantity );
	cmd.GetRec( "a_sell_charindex"		, nSellcharindex );
	cmd.GetRec( "a_totalmoney"			, TotalMoney );
	cmd.GetRec( "a_state"				, nState );

	//보유머니가 금액 부족 한지를 체크 (추가)
	if(TotalMoney > llCurCharHaveNas)
	{
		//보유 나스 부족 
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_ENUGH_MONEY, ncharindex,MSG_TRADEAGENT_ERROR_PART_SEARCH );
		SEND_Q( rmsg, dest );
		return;		
	}	

	if(nState != MSG_TRADEAGENT_STATE_SELL_BEFORE || nState == MSG_TRADEAGENT_STATE_SELL_COMPLETE)
	{
		//에러:이미 판매된 아이템 
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_ALREADY_SELLITEM, ncharindex,MSG_TRADEAGENT_ERROR_PART_SEARCH );
		SEND_Q( rmsg, dest );
		return;
	}

	if( nSellcharindex == ncharindex )
	{
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NORMAL, ncharindex, MSG_TRADEAGENT_ERROR_PART_SEARCH);
		SEND_Q( rmsg, dest );

		// 실패하면 로그 찍기
		GAMELOG << init("TradeAgent TRY ITEMCOPY")
				<< "charindex" << delim
				<< ncharindex << delim
				<< "nList_idx" << delim
				<< nList_idx << end;
		return;
	}


	// 갱신: 상태를 판매완료(1),처리 일시 처리
	//-->
	sql.Format("UPDATE t_tradeagent SET a_state = %d , a_process_date = now() , a_buy_charindex = %d , a_buy_charname = '%s' "
		       "WHERE a_index = %d "
		        ,MSG_TRADEAGENT_STATE_SELL_COMPLETE, ncharindex,(const char *)buychar_name, nList_idx);

	CDBCmd cmd2;
	cmd2.Init(&gserver.m_dbchar);
	cmd2.SetQuery(sql);
	if(cmd2.Update())
	{
		HelperTradeAgentBuyRepMsg(rmsg, ncharindex,nSellcharindex,TotalMoney,stItemName,nQuantity);
		SEND_Q( rmsg, dest );

		//쿼리 성공시 
		//-->
		gserver.m_TradeAgentList.SetState(nList_idx,MSG_TRADEAGENT_STATE_SELL_COMPLETE);
		//<--
	}else
	{
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NORMAL, ncharindex, MSG_TRADEAGENT_ERROR_PART_SEARCH );
		SEND_Q( rmsg, dest );

		// 실패하면 로그 찍기
		GAMELOG << init("Fail  Buy TradeAgent")
				<< "charindex" << delim
				<< ncharindex << delim
				<< "nList_idx" << delim
				<< nList_idx << end;
	}
	//<--
}

// 정산 리스트
void do_Command_TradeAgent_CalcListReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	char sql[1024];
	int nCharIndex;
	int nPageNo, nAlignType;

	msg >> nCharIndex >> nPageNo >> nAlignType;

	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );

	memset(sql,0,sizeof(sql));
	sprintf(sql,"SELECT *, TO_DAYS(DATE_ADD(a_process_date, INTERVAL 15 DAY)) - TO_DAYS(now()) as a_passday "
		        "FROM t_tradeagent "
				"WHERE"
				"("
				" ( a_state = 1 OR a_state = 2 ) "
				"  AND"
				"  (( a_buy_charindex = %d AND a_buycalc_state  = 0 ) OR ( a_sell_charindex = %d AND  a_sellcalc_state = 0 ))"
				")"
				"HAVING " 
				"NOW() >= a_process_date AND NOW() <= DATE_ADD(a_process_date, INTERVAL 15 DAY)"
				,nCharIndex
				,nCharIndex);


	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, nCharIndex,MSG_TRADEAGENT_ERROR_PART_CALCULATE );
		SEND_Q( rmsg, dest );
		return;
	}

	int nTotItemCount	=	0;
	nTotItemCount	=	cmd.m_nrecords;		// PC가 등록한 총 아이템수 

	//페이지 계산 
	//-->
	int nMaxPageNo = nTotItemCount / MAX_ONEPAGE_LISTNUM;
	if(nTotItemCount % MAX_ONEPAGE_LISTNUM != 0) nMaxPageNo++;

	if(nPageNo > nMaxPageNo)
	{
		//에러:페이지 번호 초과
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, nCharIndex,MSG_TRADEAGENT_ERROR_PART_CALCULATE );
		SEND_Q( rmsg, dest );
	}
	
	int nOffset = (MAX_ONEPAGE_LISTNUM * (nPageNo - 1)); 
 	//<--

	if(!cmd.Seek(nOffset))
	{
		//에러:해당 데이타가 존재 하지 않음
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, nCharIndex,MSG_TRADEAGENT_ERROR_PART_CALCULATE );
		SEND_Q( rmsg, dest );
		return;
	}

	//실제 처리 수 계산
	int nRealItemCount = 0;
	int iMok,iMod;
	iMok = iMod = 0;

	iMok = (int)(nTotItemCount / MAX_ONEPAGE_LISTNUM);
	iMod = (int)(nTotItemCount % MAX_ONEPAGE_LISTNUM);

	if(iMok > 0 && iMod > 0)
	{
		if(nPageNo >= 1 && nPageNo <= iMok) 
		{
			nRealItemCount = MAX_ONEPAGE_LISTNUM;
		}else
		{
			nRealItemCount = iMod;
		}		
	}else if(iMok > 0 && iMod == 0)
	{
		nRealItemCount = MAX_ONEPAGE_LISTNUM;
	}else if(iMok == 0 && iMod > 0)
	{
		nRealItemCount = iMod;
	}

	int* pItemSerial			= new int[nRealItemCount];			// 아이템 시리얼 
	int* pItemPlus				= new int[nRealItemCount];			// 아이템 플러스
	int* pItemFlag				= new int[nRealItemCount];			// 아이템 플래그
	S_ITEMOPTION *pItemOption	= new S_ITEMOPTION[nRealItemCount];	// 아이템 옵션
	int* pQuantity				= new int[nRealItemCount];			// 수량 
	LONGLONG* ptotalmoney		= new LONGLONG[nRealItemCount];		// 총액 
	LONGLONG* pGuaranty			= new LONGLONG[nTotItemCount];		// 보증금 
	int* pState					= new int[nRealItemCount];			// 상태(판매/반품)	
	int* pPassDay				= new int[nRealItemCount];			// 처리후 지난일

	int* pSellCalcState			= new int[nRealItemCount];			// 판매자 정산 상태 		
	int* pBuyCalcState			= new int[nRealItemCount];			// 구매자 정산 상태

	CLCString *pSellCharname	= new CLCString[nRealItemCount];	// 판매자 캐릭터 이름
	CLCString *pBuyCharname		= new CLCString[nRealItemCount];	// 구매자 캐릭터 이름 
	CLCString *pCharname		= new CLCString[nRealItemCount];	// 구매자 캐릭터 이름 
	
	int nBuyCharIndex,nSellCharIndex;

	nBuyCharIndex = nSellCharIndex = 0;

	int i = 0;
	do
	{
		cmd.GetRec( "a_item_serial"		, pItemSerial[i] );
		cmd.GetRec( "a_item_plus"		, pItemPlus[i] );
		cmd.GetRec( "a_item_flag"		, pItemFlag[i] );
		
		cmd.GetRec( "a_option_count"	, pItemOption[i].m_nOptionCnt);
		cmd.GetRec( "a_option_0_type"	, pItemOption[i].m_Option_type[0]);
		cmd.GetRec( "a_option_0_level"	, pItemOption[i].m_Option_level[0]);
		cmd.GetRec( "a_option_1_type"	, pItemOption[i].m_Option_type[1]);
		cmd.GetRec( "a_option_1_level"	, pItemOption[i].m_Option_level[1]);
		cmd.GetRec( "a_option_2_type"	, pItemOption[i].m_Option_type[2]);
		cmd.GetRec( "a_option_2_level"	, pItemOption[i].m_Option_level[2]);
		cmd.GetRec( "a_option_3_type"	, pItemOption[i].m_Option_type[3]);
		cmd.GetRec( "a_option_3_level"	, pItemOption[i].m_Option_level[3]);
		cmd.GetRec( "a_option_4_type"	, pItemOption[i].m_Option_type[4]);
		cmd.GetRec( "a_option_4_level"	, pItemOption[i].m_Option_level[4]);

		cmd.GetRec( "a_quantity"		, pQuantity[i] );
		cmd.GetRec( "a_totalmoney"		, ptotalmoney[i] );
		cmd.GetRec( "a_state"			, pState[i] );
		cmd.GetRec( "a_passday"			, pPassDay[i] );

		cmd.GetRec( "a_sell_charindex"	, nSellCharIndex);
		cmd.GetRec( "a_buy_charindex"	, nBuyCharIndex);
		cmd.GetRec( "a_guaranty"		, pGuaranty[i] );

		cmd.GetRec( "a_sellcalc_state"	, pSellCalcState[i] );
		cmd.GetRec( "a_buycalc_state"	, pBuyCalcState[i] );

		cmd.GetRec( "a_sell_charname"	, pSellCharname[i] );
		cmd.GetRec( "a_buy_charname"	, pBuyCharname[i] );


		//판매 완료 
		if(nSellCharIndex == nCharIndex && pState[i] == MSG_TRADEAGENT_STATE_SELL_COMPLETE)
		{
			//판매 수수료 및 보증금 처리(소숫점 버림)
			ptotalmoney[i] = (LONGLONG)(ptotalmoney[i] - (LONGLONG)(ptotalmoney[i] * 0.05f)  + pGuaranty[i]);

			//구매자 캐릭터 이름 
			pCharname[i] = pBuyCharname[i];
		}

		//구매,반송  총금액 0 표시
		if(nBuyCharIndex == nCharIndex || pState[i] == MSG_TRADEAGENT_STATE_SELL_RETURNED)
		{
			ptotalmoney[i] = 0;
		}

		//반송 캐릭터 이름 
		if(pState[i] == MSG_TRADEAGENT_STATE_SELL_RETURNED)
		{
			//캐릭터 이름 
			pCharname[i] = "";
		}

		//구입은 상태를 바꿔 표시
		if(nBuyCharIndex == nCharIndex && pState[i] == MSG_TRADEAGENT_STATE_SELL_COMPLETE)
		{
			pState[i] = MSG_TRADEAGENT_STATE_BUY;

			//판매자 캐릭터 이름 
			pCharname[i] = pSellCharname[i];
		}
		i++;

		if(i >= nRealItemCount) break;

	}while( cmd.MoveNext());


	HelperTradeAgentCalcListRepMsg(rmsg, nCharIndex, nMaxPageNo, nPageNo, nRealItemCount, pItemSerial,pItemPlus,pItemFlag,pItemOption,pQuantity,ptotalmoney,pState,pPassDay,pCharname);
	SEND_Q( rmsg, dest );

	delete[] pItemSerial;
	delete[] pItemPlus;
	delete[] pItemFlag;
	delete[] pItemOption; 
	delete[] pQuantity;
	delete[] ptotalmoney;
	delete[] pState;
	delete[] pPassDay;
	delete[] pGuaranty;

	delete[] pSellCalcState;
	delete[] pBuyCalcState;	
	delete[] pSellCharname;	
	delete[] pBuyCharname;	
	delete[] pCharname;	
}

// 정산 하기(페이지 단위 순서대로 정산) 
void do_Command_TradeAgent_CalculateReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int nCharIndex;

	msg >> nCharIndex;

	char sql[1024];
	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );

	memset(sql,0,sizeof(sql));
	sprintf(sql,"SELECT *, TO_DAYS(DATE_ADD(a_process_date, INTERVAL 15 DAY)) - TO_DAYS(now()) as a_passday "
		        "FROM t_tradeagent "
				"WHERE"
				"("
				" ( a_state = 1 OR a_state = 2 ) "
				"  AND"
				"  (( a_buy_charindex = %d AND a_buycalc_state  = 0 ) OR ( a_sell_charindex = %d AND  a_sellcalc_state = 0 ))"
				")"
				"HAVING " 
				"NOW() >= a_process_date AND NOW() <= DATE_ADD(a_process_date, INTERVAL 15 DAY)"
				,nCharIndex
				,nCharIndex);


	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, nCharIndex, MSG_TRADEAGENT_ERROR_PART_CALCULATE );
		SEND_Q( rmsg, dest );
		return;
	}


	int nTotItemCount	=	0;
	nTotItemCount	=	cmd.m_nrecords;		// PC가 등록한 총 아이템수 

	//페이지 계산 
	//-->
	int nMaxPageNo = nTotItemCount / MAX_ONEPAGE_LISTNUM;
	if(nTotItemCount % MAX_ONEPAGE_LISTNUM != 0) nMaxPageNo++;

	for(int nPageNo=1; nPageNo <= nMaxPageNo; nPageNo++)
	{
		if(nPageNo > nMaxPageNo)
		{
			//에러:페이지 번호 초과
			HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, nCharIndex,MSG_TRADEAGENT_ERROR_PART_CALCULATE );
			SEND_Q( rmsg, dest );
		}
		
		int nOffset = (MAX_ONEPAGE_LISTNUM * (nPageNo - 1)); 
 		//<--

		if(!cmd.Seek(nOffset))
		{
			//에러:해당 데이타가 존재 하지 않음
			HelperTradeAgentErrorMsg( rmsg, MSG_TRADEAGENT_ERROR_NOT_BE_ITEM, nCharIndex,MSG_TRADEAGENT_ERROR_PART_CALCULATE );
			SEND_Q( rmsg, dest );
			return;
		}

		//실제 처리 수 계산
		int nRealItemCount = 0;
		int iMok,iMod;
		iMok = iMod = 0;

		iMok = (int)(nTotItemCount / MAX_ONEPAGE_LISTNUM);
		iMod = (int)(nTotItemCount % MAX_ONEPAGE_LISTNUM);

		if(iMok > 0 && iMod > 0)
		{
			if(nPageNo >= 1 && nPageNo <= iMok) 
			{
				nRealItemCount = MAX_ONEPAGE_LISTNUM;
			}else
			{
				nRealItemCount = iMod;
			}		
		}else if(iMok > 0 && iMod == 0)
		{
			nRealItemCount = MAX_ONEPAGE_LISTNUM;
		}else if(iMok == 0 && iMod > 0)
		{
			nRealItemCount = iMod;
		}

		int* pListindex				= new int[nRealItemCount];			// db 인덱스
		int* pItemSerial			= new int[nRealItemCount];			// 아이템 시리얼 
		CLCString* pItemSerial2		= new CLCString[nRealItemCount];	// 아이템 시리얼2 
		int* pItemPlus				= new int[nRealItemCount];			// 아이템 플러스
		int* pItemFlag				= new int[nRealItemCount];			// 아이템 플래그
		S_ITEMOPTION *pItemOption	= new S_ITEMOPTION[nRealItemCount];	// 아이템 옵션
		int* pQuantity				= new int[nRealItemCount];			// 수량 
		LONGLONG* ptotalmoney		= new LONGLONG[nRealItemCount];		// 총액 
		LONGLONG* pGuaranty			= new LONGLONG[nRealItemCount];		// 보증금 
		int* pState					= new int[nRealItemCount];			// 상태(판매/반품)	

		int* pSellCalcState			= new int[nRealItemCount];			
		int* pBuyCalcState			= new int[nRealItemCount];			

		int nBuyCharIndex,nSellCharIndex;

		nBuyCharIndex = nSellCharIndex = 0;

		int i = 0;
		do
		{
			cmd.GetRec( "a_index"			, pListindex[i] );
			cmd.GetRec( "a_item_serial"		, pItemSerial[i] );
			cmd.GetRec( "a_item_serial2"	, pItemSerial2[i] );
			cmd.GetRec( "a_item_plus"		, pItemPlus[i] );
			cmd.GetRec( "a_item_flag"		, pItemFlag[i] );

			cmd.GetRec( "a_option_count"	, pItemOption[i].m_nOptionCnt);
			cmd.GetRec( "a_option_0_type"	, pItemOption[i].m_Option_type[0]);
			cmd.GetRec( "a_option_0_level"	, pItemOption[i].m_Option_level[0]);
			cmd.GetRec( "a_option_1_type"	, pItemOption[i].m_Option_type[1]);
			cmd.GetRec( "a_option_1_level"	, pItemOption[i].m_Option_level[1]);
			cmd.GetRec( "a_option_2_type"	, pItemOption[i].m_Option_type[2]);
			cmd.GetRec( "a_option_2_level"	, pItemOption[i].m_Option_level[2]);
			cmd.GetRec( "a_option_3_type"	, pItemOption[i].m_Option_type[3]);
			cmd.GetRec( "a_option_3_level"	, pItemOption[i].m_Option_level[3]);
			cmd.GetRec( "a_option_4_type"	, pItemOption[i].m_Option_type[4]);
			cmd.GetRec( "a_option_4_level"	, pItemOption[i].m_Option_level[4]);

			cmd.GetRec( "a_quantity"		, pQuantity[i] );
			cmd.GetRec( "a_totalmoney"		, ptotalmoney[i] );
						
			cmd.GetRec( "a_sell_charindex"	, nSellCharIndex);
			cmd.GetRec( "a_buy_charindex"	, nBuyCharIndex);

			cmd.GetRec( "a_state"			, pState[i] );
			cmd.GetRec( "a_guaranty"		, pGuaranty[i] );

			cmd.GetRec( "a_sellcalc_state"	, pSellCalcState[i] );
			cmd.GetRec( "a_buycalc_state"	, pBuyCalcState[i] );


			//판매 완료 
			if( nSellCharIndex == nCharIndex && pState[i] == MSG_TRADEAGENT_STATE_SELL_COMPLETE )
			{
				//판매 수수료 및 보증금 처리(소숫점 버림)
				ptotalmoney[i] = (LONGLONG)(ptotalmoney[i] - (LONGLONG)(ptotalmoney[i] * 0.05f)  + pGuaranty[i]);
			}

			//구매,반송  총금액 0 표시
			if(nBuyCharIndex == nCharIndex || pState[i] == MSG_TRADEAGENT_STATE_SELL_RETURNED)
			{
				ptotalmoney[i] = 0;
			}

			//구입은 상태를 바꿔 표시
			if( nBuyCharIndex == nCharIndex && pState[i] == MSG_TRADEAGENT_STATE_SELL_COMPLETE)
			{
				pState[i] = MSG_TRADEAGENT_STATE_BUY;
			}
			i++;

			if(i >= nRealItemCount) break;

		}while( cmd.MoveNext());


		HelperTradeAgentCalculateRepMsg(rmsg, nCharIndex, nMaxPageNo, nPageNo, nRealItemCount,pListindex, pItemSerial,pItemSerial2,pItemPlus,pItemFlag,pItemOption,pQuantity,ptotalmoney,pState);
		SEND_Q( rmsg, dest );

		delete[] pListindex;
		delete[] pItemSerial;
		delete[] pItemSerial2;
		delete[] pItemPlus;
		delete[] pItemFlag;
		delete[] pItemOption; 
		delete[] pQuantity;
		delete[] ptotalmoney;
		delete[] pState;
		delete[] pGuaranty;

		delete[] pSellCalcState;
		delete[] pBuyCalcState;	
	}
}

// 정산 결과 처리
void do_Command_TradeAgent_CalcResult( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int nCharIndex = -1;
	int nItemCount=0;
	int i;

	msg >> nCharIndex
		>> nItemCount;

	int* pResultindex	=   new int[nItemCount];
	int* pProcResult	=   new int[nItemCount];

	for (i = 0; i < nItemCount; i++)
	{
		msg >> pResultindex[i]
			>> pProcResult[i];
	}

	//정산 결과 처리 
	GAMELOG << init("TradeAgent_CalcResult")
		<< "nCharIndex" << delim << nCharIndex 
		<< "nItemCount" << delim << nItemCount << end;; 

	for (i = 0; i < nItemCount; i++)
	{
		GAMELOG << init(" TradeAgent_CalcResult :")
			<< "pResultindex"   << delim
			<<  pResultindex[i] << delim << i << delim 
			<< "pProcResult"    << delim << i << delim 
			<<  pProcResult[i]  << end;
	}
		 

	CLCString	sql( 1024 );
	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );

	for (i = 0; i < nItemCount; i++)
	{
		if(pProcResult[i] == 1)		//Game 서버에서 정산 처리 된 것만
		{
			// 정보 획득
			//-->
			sql.Format("SELECT * FROM t_tradeagent WHERE a_index = %d",pResultindex[i]);

			cmd.SetQuery( sql );
			if( !cmd.Open() || !cmd.MoveFirst() )
			{
				continue;
			}

			int nBuyCharIndex,nSellCharIndex;

			nBuyCharIndex = nSellCharIndex = 0;

			cmd.GetRec( "a_sell_charindex"	, nSellCharIndex);
			cmd.GetRec( "a_buy_charindex"	, nBuyCharIndex);
			//<--	
		
			if(nSellCharIndex == nCharIndex)
			{
				// 갱신: 상태를 정산완료,정산일시 처리
				//-->
				sql.Format("UPDATE t_tradeagent SET a_sellcalc_state = %d , a_account_date = now() WHERE a_index = %d "
							,MSG_TRADEAGENT_STATE_CALCULATE_COMPLETE, pResultindex[i]);

				cmd.SetQuery(sql);
				if(!cmd.Update())
				{
					// 실패하면 로그 찍기
					GAMELOG << init("Fail  CalcResult TradeAgent DB Update")
							<< "charindex" << delim
							<< nCharIndex << delim
							<< "Resultindex" << delim
							<< pResultindex[i] << end;
				}else
				{
					// 성공 하면 로그 찍기
					GAMELOG << init("Sucess TradeAgent_CalcResult DB Update")
						    << "t_tradeagent Set a_sellcalc_state:CALCULATE_COMPLETE" << delim
							<< "charindex" << delim
							<< nCharIndex << delim
							<< "Resultindex" << delim
							<< pResultindex[i] << end;

				}
				//<--
			}else if(nBuyCharIndex == nCharIndex)
			{
				// 갱신: 상태를 정산완료,정산일시 처리
				//-->
				sql.Format("UPDATE t_tradeagent SET a_buycalc_state = %d , a_account_date = now() WHERE a_index = %d "
							,MSG_TRADEAGENT_STATE_CALCULATE_COMPLETE, pResultindex[i]);

				cmd.SetQuery(sql);
				if(!cmd.Update())
				{
					// 실패하면 로그 찍기
					GAMELOG << init("Fail  CalcResult TradeAgent DB Update")
							<< "charindex" << delim
							<< nCharIndex << delim
							<< "Resultindex" << delim
							<< pResultindex[i] << end;
				}else
				{
					// 성공 하면 로그 찍기
					GAMELOG << init("Sucess TradeAgent_CalcResult DB Update")
						    << "t_tradeagent Set a_buycalc_state:CALCULATE_COMPLETE" << delim
							<< "charindex" << delim
							<< nCharIndex << delim
							<< "Resultindex" << delim
							<< pResultindex[i] << end;
				}
				//<--
			}
		}
	}

	delete[] pResultindex;
	delete[] pProcResult;
}

// 체크 정산
void do_Command_TradeAgent_CheckCalcReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int nCharIndex;

	msg >> nCharIndex;

	char sql[1024];
	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );

	int nIsBeCalcdata = 0;					// 존재 여부

	sprintf(sql,"SELECT count(*) as CALCCOUNT FROM t_tradeagent "
				"WHERE"
				"("
				" ( a_state = 1 OR a_state = 2 ) "
				"  AND"
				"  (( a_buy_charindex = %d AND a_buycalc_state  = 0 ) OR ( a_sell_charindex = %d AND  a_sellcalc_state = 0 ))"
				")"
				"AND " 
				"NOW() >= a_process_date AND NOW() <= DATE_ADD(a_process_date, INTERVAL 15 DAY)"
				,nCharIndex
				,nCharIndex);


	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		HelperTradeAgentCheckCalcRepMsg(rmsg, nCharIndex,nIsBeCalcdata);
		SEND_Q( rmsg, dest );
		return;		
	}

	int nTotItemCount = 0;
	nTotItemCount =	cmd.m_nrecords;		// 레코드 수
	int nCalcCount = 0;

	cmd.GetRec( "CALCCOUNT"	, nCalcCount );

	if(nCalcCount > 0)
	{
		nIsBeCalcdata = 1;
	}else 
	{
		nIsBeCalcdata = 0;
	}

	HelperTradeAgentCheckCalcRepMsg(rmsg, nCharIndex,nIsBeCalcdata);
	SEND_Q( rmsg, dest );

}

#endif	// TRADE_AGENT

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
void do_Command_DisconnectHackCharacter( CNetMsg& msg, CDescriptor* dest )
{
	int nCharIdx = 0, nUserIdx = 0;
	int nHack_type=0;
	float delay;
	CLCString user_id(20 + 1);
	CLCString char_id(20 + 1);
	msg >> nUserIdx
		>> user_id
		>> nCharIdx
		>> char_id
		>> nHack_type
		>> delay;

	CLCString strType_hack(20 + 1);
	if ( nHack_type == HACK_TYPE_PULSE )
	{
		strType_hack.Format("Pulse hack");
	}
	else if( nHack_type == HACK_TYPE_CLIENT )
	{	
		strType_hack.Format("client hack check");
	}
	else
		return;

	CNetMsg rmsg;

	CDBCmd DB_cmd1;

	DB_cmd1.Init(&gserver.m_dbchar);

	sprintf(g_buf, "insert into t_hack_characters (a_user_idx, a_user_id, a_char_idx, a_char_id, a_time, a_hacktype, a_delay) values (%d, '%s', %d, '%s', NOW(), '%s', %f)",
		nUserIdx, (const char*)user_id, nCharIdx, (const char*)char_id, (const char*)strType_hack, delay);
	DB_cmd1.SetQuery(g_buf);
	if (!DB_cmd1.Update())
		return;
}
#endif // DISCONNECT_HACK_CHARACTER 

#ifdef EXPEDITION_RAID			// 원정대 

//생성(파티 전환)
void do_Command_Exped_CreateReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int nBossIndex;
	CParty* pPartyBoss = NULL;
	CLCString BossName(MAX_CHAR_NAME_LENGTH + 1);
	CPartyMember* pPartyMember = NULL;
	CExpedMember* pExpedMember = NULL;

	msg >> nBossIndex
		>> BossName;

	CParty partyFind(dest->m_subNo, 0, nBossIndex, "", 0, "");
	pPartyBoss =  gserver.m_listParty.GetData(gserver.m_listParty.FindData(&partyFind));

	if(pPartyBoss)
	{
		if(pPartyBoss->GetBossIndex() != nBossIndex)
		{
			// 에러 
			HelperExpedErrorMsg( rmsg, MSG_EXPED_ERROR_NORMAL, nBossIndex );
			SEND_Q( rmsg, dest );
		}

		if (pPartyBoss->GetMemberCount() >= MAX_PARTY_MEMBER)
		{
			// 에러
			HelperExpedErrorMsg( rmsg, MSG_EXPED_ERROR_NORMAL, nBossIndex );
			SEND_Q( rmsg, dest );
		}

		// 신규 원정대 생성
		CExpedition* pExped = NULL;
		pExped = new CExpedition(dest->m_subNo, pPartyBoss->GetPartyType(MSG_DIVITYPE_EXP), nBossIndex,(const char*)BossName,MSG_EXPED_GROUP_1,MSG_EXPED_MEMBERTYPE_BOSS);
		
		if(pExped)
		{
			int nJoinCnt=0;
			for(int i=0; i < MAX_PARTY_MEMBER ; i++)
			{
				pPartyMember = (CPartyMember*)pPartyBoss->GetMemberByListIndex(i);
				if(pPartyMember)
				{
					//대원 추가
					pExpedMember = (CExpedMember*) pExped->Join(pPartyMember->GetCharIndex(),pPartyMember->GetCharName(),MSG_EXPED_MEMBERTYPE_NORMAL);

					if(pExpedMember)
					{
						nJoinCnt++;	
					}

				}
				pPartyMember = NULL;
			}

			if(pPartyBoss->GetMemberCount() == nJoinCnt )
			{
				gserver.m_listExped.AddToTail(pExped);		// 성공	

				int nRealCount = pExped->GetMemberCount() - 1;	//원정대장 제외

				int* pCharIndex		 = new int[nRealCount];			// 캐릭터 인덱스 
				CLCString* pCharName = new CLCString[nRealCount];	// 캐릭터 이름 
				int*  pGroupType	 = new int[nRealCount];			// 그룹 타입
				int*  pMemberType	 = new int[nRealCount];			// 멤버 타입
				int*  pSetLabelType	 = new int[nRealCount];			// 표식
				int*  pQuitType		 = new int[nRealCount];			// 정상, 비정상

				int nCnt=0;
				pExpedMember = NULL;
				for(int i=0; i < MAX_EXPED_GMEMBER ; i++)
				{
					pExpedMember = (CExpedMember*)pExped->GetMemberByListIndex(MSG_EXPED_GROUP_1,i,MSG_EXPED_MEMBERTYPE_NORMAL);
					if(pExpedMember)
					{
						pCharIndex[nCnt]	= pExpedMember->GetCharIndex();
						pCharName[nCnt]		= pExpedMember->GetCharName();	
						pGroupType[nCnt]	= pExpedMember->GetGroupType();	
						pMemberType[nCnt]	= pExpedMember->GetMemberType();
						pSetLabelType[nCnt]	= pExpedMember->GetLabelType();
						pQuitType[nCnt]		= pExpedMember->GetQuitType();
						nCnt++;	

						if(nCnt >= nRealCount) break;
					}
					pExpedMember = NULL;
				}


				// 결과 전달
				HelperExpedCreateRepMsg(rmsg,pExped->GetExpeditionType(EXPED_DIVITYPE_EXP),pExped->GetExpeditionType(EXPED_DIVITYPE_EXP),pExped->GetExpeditionType(EXPED_DIVITYPE_SPECIAL),pExped->GetBossIndex(),pExped->GetBossName(),nRealCount,pCharIndex,pCharName,pGroupType,pMemberType,pSetLabelType,pQuitType);
				gserver.SendToAllSubServer(rmsg, dest->m_subNo);

				delete[] pCharIndex;
				delete[] pCharName;
				delete[] pGroupType;
				delete[] pMemberType;
				delete[] pSetLabelType;
				delete[] pQuitType;


			}else 
			{
				// 에러
				HelperExpedErrorMsg( rmsg, MSG_EXPED_ERROR_NORMAL, nBossIndex );
				SEND_Q( rmsg, dest );
			}

		}else
		{
			// 에러
			HelperExpedErrorMsg( rmsg, MSG_EXPED_ERROR_NORMAL, nBossIndex );
			SEND_Q( rmsg, dest );
		}
	}
}

//초대 요청 
void do_Command_Exped_InviteReq( CNetMsg& msg, CDescriptor* dest )
{
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> nBossIndex
		>> strBossName
		>> nTargetIndex
		>> strTargetName;

	CExpedition* pExpedBoss = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	CExpedition* pExpedTarget = gserver.FindExpedByBossIndex(dest->m_subNo, nTargetIndex);

	CNetMsg	rmsg;
	// nBossIndex가 원정대 초대가 가능한지 : 원정대가 없거나 보스
	if (pExpedBoss && pExpedBoss->GetBossIndex() != nBossIndex)
	{
		HelperExpedErrorMsg( rmsg, MSG_EXPED_ERROR_NORMAL, nBossIndex );
 		SEND_Q( rmsg, dest );
		return ;
	}

	// 대상 원정대 검사
	if (pExpedTarget)
	{
		HelperExpedErrorMsg( rmsg, MSG_EXPED_ERROR_NORMAL, nBossIndex );
 		SEND_Q( rmsg, dest );
		return ;
	}

	// 원정대 인원 검사
	if (pExpedBoss && pExpedBoss->GetMemberCount() >= MAX_EXPED_MEMBER)
	{
		HelperExpedErrorMsg( rmsg, MSG_EXPED_ERROR_NORMAL, nBossIndex );
 		SEND_Q( rmsg, dest );
		return ;
	}

	// 현재 요청자 유무 검사
	if (pExpedBoss && pExpedBoss->GetRequestIndex() > 0)
	{
		HelperExpedErrorMsg( rmsg, MSG_EXPED_ERROR_NORMAL, nBossIndex );
 		SEND_Q( rmsg, dest );
		return ;
	}

	if (pExpedBoss)
	{
		// 기존 원정대에 요청자로 설정
		pExpedBoss->SetRequest(nTargetIndex, strTargetName);
	}

	// 결과 전달
	HelperExpedInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName,pExpedBoss->GetExpeditionType(EXPED_DIVITYPE_EXP),pExpedBoss->GetExpeditionType(EXPED_DIVITYPE_ITEM),pExpedBoss->GetExpeditionType(EXPED_DIVITYPE_SPECIAL));
	SEND_Q( rmsg, dest );

}

//초대 수락 
void do_Command_Exped_AllowReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int	nBossIndex;
	int	nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> nBossIndex
		>> nTargetIndex
		>> strTargetName;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);

	if (!pExped)
	{
		HelperExpedErrorMsg(rmsg, MSG_EXPED_ERROR_ALLOW_NOT_EXPED,nBossIndex);
		SEND_Q( rmsg, dest );
		return ;
	}

	if (pExped->GetRequestIndex() != nTargetIndex)
	{
		HelperExpedErrorMsg(rmsg, MSG_EXPED_ERROR_ALLOW_NOT_EXPED,nBossIndex);
		SEND_Q( rmsg, dest );
		return ;
	}

	//빈슬룻 찾기(그룹 결정)
	int nNewGroup = pExped->GetGroupBeEmptySlot();

	if(nNewGroup >= 0)
	{
		CExpedMember* pMember=NULL;
		// 파티원으로 등록(추후 수정)		
		pMember = (CExpedMember*)pExped->JoinRequest(strTargetName, MSG_EXPED_MEMBERTYPE_NORMAL);

		if(pMember)
		{
			HelperExpedAllowRepMsg(rmsg, nBossIndex, nTargetIndex, strTargetName, nNewGroup, MSG_EXPED_MEMBERTYPE_NORMAL,pMember->GetListIndex());
			SEND_Q( rmsg, dest );
		}
	}else
	{
		//원정대 가득참
		//HelperExpedErrorMsg(rmsg, MSG_EXPED_ERROR_ALLOW_NOT_PARTY,nBossIndex);
		//SEND_Q( rmsg, dest );
	}

}

//초대 거절 
void do_Command_Exped_RejectReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;
	int	nBossIndex;
	int	nTargetIndex;

	msg >> nBossIndex
		>> nTargetIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
	{
		HelperExpedErrorMsg(rmsg, MSG_HELPER_PARTY_ERROR_REJECT_FAIL,nBossIndex);
		SEND_Q( rmsg, dest );
		return ;
	}

	if (pExped->GetRequestIndex() < 1)
	{
		HelperExpedErrorMsg(rmsg, MSG_HELPER_PARTY_ERROR_REJECT_FAIL,nBossIndex);
		SEND_Q( rmsg, dest );
		return ;
	}

	// 요청자가 보스인지 일반인지 검사
	if (pExped->GetBossIndex() == nTargetIndex || pExped->GetRequestIndex() == nTargetIndex)
	{
		HelperExpedRejectRepMsg(rmsg, nBossIndex, nTargetIndex);
		SEND_Q( rmsg, dest );

		pExped->SetRequest(-1, "");
		if (pExped->GetMemberCount() < 2)
		{
			// 파티 종료
			gserver.m_listExped.RemoveData(pExped);
			delete pExped;
		}
	}
	else
	{
		HelperExpedErrorMsg(rmsg, MSG_HELPER_PARTY_ERROR_REJECT_FAIL,nBossIndex);
		SEND_Q( rmsg, dest );
		return ;
	}
}

//탈퇴 
void do_Command_Exped_QuitReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int	nBossIndex,	nTargetIndex;
	int nQuitMode;						// 정상,비정성 구분

	msg >> nBossIndex
		>> nTargetIndex					// 나가는 캐릭터인덱스
	    >> nQuitMode;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;

	if(nQuitMode == MSG_EXPED_QUITMODE_NORMAL)	
	{
		pExped->DeleteMember(nTargetIndex);
	}	

	if (pExped->GetMemberCount() < 2)
	{
		// 원정대  해체
		gserver.m_listExped.RemoveData(pExped);
		delete pExped;

		HelperExpedQuitRepMsg(rmsg, nBossIndex, nTargetIndex, nQuitMode, MSG_EXPED_ERROR_QUIT_END);
		SEND_Q( rmsg, dest );
	}
	else
	{
		//나가는 캐릭터가 보스이면 위임할 보스를 획득 함
		if(nBossIndex == nTargetIndex)
		{
			CExpedMember* pNewBossMember = (CExpedMember*) pExped->FindNextBoss();
			if(pNewBossMember)
			{
				pExped->ChangeBoss(pNewBossMember->GetCharIndex(),nBossIndex,false);
			}
		}

		HelperExpedQuitRepMsg(rmsg, nBossIndex, nTargetIndex, nQuitMode, MSG_EXPED_ERROR_QUIT_OK);
		SEND_Q( rmsg, dest );
	}	

}

//강퇴
void do_Command_Exped_KickReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int	nBossIndex;
	int	nTargetIndex;

	msg >> nBossIndex
		>> nTargetIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;

	pExped->DeleteMember(nTargetIndex);

	if (pExped->GetMemberCount() < 2)
	{
		// 원정대 해체
		gserver.m_listExped.RemoveData(pExped);
		delete pExped;

		HelperExpedKickRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_EXPED_ERROR_KICK_END);
		SEND_Q( rmsg, dest );
	}
	else
	{
		HelperExpedKickRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_EXPED_ERROR_KICK_OK);
		SEND_Q( rmsg, dest );
	}
}

//대장 위임 
void do_Command_Exped_ChangeBossReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int	nBossIndex, nChangeMode, nNewBossIndex;

	msg >> nBossIndex
		>> nChangeMode				// 수동,자동
		>> nNewBossIndex;           // 자동(-1)

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;
	
	// 자동위임 이면 
	if(nChangeMode == MSG_EXPED_CHANGEBOSS_AUTO)
	{
		CExpedMember* pNewBossMember = (CExpedMember*) pExped->FindNextBoss();
		if(pNewBossMember)
		{
			if(pExped->ChangeBoss(pNewBossMember->GetCharIndex(),nBossIndex,true))
			{
				HelperExpedChangeBossRepMsg(rmsg, nBossIndex, pNewBossMember->GetCharIndex(), nChangeMode);
				SEND_Q( rmsg, dest );
			}
		}
	}else if(nChangeMode == MSG_EXPED_CHANGEBOSS_MANUAL)
	{
		if(pExped->ChangeBoss(nNewBossIndex,nBossIndex,true))
		{
			HelperExpedChangeBossRepMsg(rmsg, nBossIndex, nNewBossIndex, nChangeMode);
			SEND_Q( rmsg, dest );
		}
	}

}

//타입 변경
void do_Command_Exped_ChangeTypeReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int nBossIndex;
	char cDiviType,cExpedType;

	msg >> nBossIndex
		>> cDiviType
		>> cExpedType;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;

	pExped->SetExpedType(cDiviType,cExpedType);

	HelperExpedChangeTypeRepMsg(rmsg, nBossIndex, cDiviType, cExpedType);
	SEND_Q( rmsg, dest );
}

//부대장 임명
void do_Command_Exped_SetMBossReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int nBossIndex, nNewMBossIndex;

	msg >> nBossIndex
		>> nNewMBossIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if(!pExped)
		return ;

	CExpedMember* pMember = NULL;
	pMember =  (CExpedMember*)pExped->GetMemberCharIndex(pExped->FindMemberGroupIndex(nNewMBossIndex), pExped->FindMemberListIndex(nNewMBossIndex) );

	if(pMember)
	{
		//부대장 수 체크
		if(pExped->GetGroupMembertypeCount(pMember->GetGroupType(),MSG_EXPED_MEMBERTYPE_MBOSS) >= 1)
		{
			//에러:한 그룹에 부대장 수 초과
			return;
		}

		if (pExped->SetMBoss(nNewMBossIndex))
		{
 			HelperExpedSetMBossRepMsg(rmsg, nBossIndex, nNewMBossIndex);
 			SEND_Q( rmsg, dest );
		}
	}
}

//부대장 임명 해제
void do_Command_Exped_ResetMBossReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int nBossIndex, nNewMBossIndex;

	msg >> nBossIndex
		>> nNewMBossIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;

	CExpedMember* pMember = NULL;
	pMember =  (CExpedMember*)pExped->GetMemberCharIndex(pExped->FindMemberGroupIndex(nNewMBossIndex), pExped->FindMemberListIndex(nNewMBossIndex) );

	if(pMember)
	{
		if(pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_MBOSS)
		{
			//에러: 부대장아 아님
			return;
		}

		if (pExped->ResetMBoss(nNewMBossIndex))
		{
 			HelperExpedResetMBossRepMsg(rmsg, nBossIndex, nNewMBossIndex);
 			SEND_Q( rmsg, dest );
		}
	}
}

//원정대 해체
void do_Command_Exped_EndExpedReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;

	int	nBossIndex;

	msg >> nBossIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;

	//원정 대원 정보 삭제
	CExpedMember*  pMember = NULL;
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			 pMember =  (CExpedMember*) pExped->GetMemberByListIndex(i,j); 
			 if(pMember)
			 {
				 pExped->DeleteMember(pMember->GetCharIndex());
			 }

			 pMember = NULL;
		}
	}

	// 원정대 삭제
	gserver.m_listExped.RemoveData(pExped);
	delete pExped;

	HelperExpedEndExpedRepMsg(rmsg, nBossIndex);
	SEND_Q( rmsg, dest );
}

//그룹이동
void do_Command_Exped_MoveGroupReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;
	int nBossIndex, nSourceGroup, nMoveCharIndex, nTargetGroup, nTargetListindex; 

	msg >> nBossIndex
		>> nSourceGroup
		>> nMoveCharIndex
		>> nTargetGroup
		>> nTargetListindex;	

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;

	if(pExped->MoveGroup(nSourceGroup,nMoveCharIndex,nTargetGroup,nTargetListindex))
	{
		//성공
		HelperExpedMoveGroupRepMsg(rmsg, nBossIndex,nSourceGroup, nMoveCharIndex, nTargetGroup, nTargetListindex);
		SEND_Q( rmsg, dest );
	}else
	{
		//에러 
	}
}

//표식 지정 (미정)
void do_Command_Exped_SetLabelReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;
	int nBossIndex, nType, nMode, nLabel, nDestIndex;

	msg >> nBossIndex
		>> nType
		>> nMode
		>> nLabel
		>> nDestIndex;
	
	if(nType == MSG_EXPED_SETLABEL_TYPE_PC)
	{
		CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
		if (!pExped)
			return ;

		CExpedMember* pMember = NULL;
		pMember = (CExpedMember*) pExped->GetMemberByListIndex(pExped->FindMemberGroupIndex(nDestIndex),pExped->FindMemberListIndex(nDestIndex));
		if(pMember)
		{

			if(nMode == MSG_EXPED_SETLABEL_MODE_SET)
			{
				pMember->SetLabelType(nLabel);
			}else
			{
				pMember->SetLabelType(-1);
			}

			//성공
			HelperExpedSetLabelRepMsg(rmsg, nBossIndex,nType,nMode,nLabel,nDestIndex);
			SEND_Q( rmsg, dest );
		}

	}
}

//로그아웃 후 다시 접속
void do_Command_Exped_RejoinReq( CNetMsg& msg, CDescriptor* dest )
{
	CNetMsg rmsg;
	int nBossIndex, nJoinIndex;

	msg >> nBossIndex
		>> nJoinIndex;
	
	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return;

	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) pExped->GetMemberByCharIndex(nJoinIndex);
	if(pMember)
	{
		//정상 설정
		pMember->SetQuitType(MSG_EXPED_QUITMODE_NORMAL);

		//성공
		HelperExpedRejoinRepMsg(rmsg, nBossIndex, nJoinIndex);
		SEND_Q( rmsg, dest );
	}
}

void do_Command_Exped_ChatReq( CNetMsg& msg, CDescriptor* dest )
{
	int nBossIndex;
	int nCharIndex;
	int nGroupTyp;
	CLCString strName(MAX_CHAR_NAME_LENGTH + 1);
	CLCString strChat(1000);

	msg >> nBossIndex
		>> nCharIndex
		>> strName
		>> nGroupTyp
		>> strChat;

	CNetMsg rmsg;	

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return;

	HelperExpedChatRepMsg(rmsg, nBossIndex, nCharIndex,strName,nGroupTyp,strChat);
	SEND_Q( rmsg, dest );
}
#endif //EXPEDITION_RAID