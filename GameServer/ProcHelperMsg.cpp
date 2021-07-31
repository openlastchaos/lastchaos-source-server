#include "stdhdrs.h"
#include "Log.h"
#include "Exp.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"
#include "DBCmd.h"
#include "DratanCastle.h"

void ProcHelperWhisperReq(CNetMsg& msg, int seq, int server, int subno, int zone);
void ProcHelperWhisperRep(CNetMsg& msg, int seq, int server, int subno, int zone);
void ProcHelperWhisperNotfound(CNetMsg& msg, int seq, int server, int subno, int zone);

void ProcHelperWarNoticeTime(CNetMsg& msg);
void ProcHelperWarNoticeTimeRemain(CNetMsg& msg);
void ProcHelperWarNoticeStart(CNetMsg& msg);
void ProcHelperWarJoinAttackGuild(CNetMsg& msg);
void ProcHelperWarJoinDefenseGuild(CNetMsg& msg);
void ProcHelperWarNoticeStartAttackCastle(CNetMsg& msg);
void ProcHelperWarNoticeRemainFieldTime(CNetMsg& msg);
void ProcHelperWarNoticeEnd(CNetMsg& msg);

void ProcHelperGuildStashHistoryRep(CNetMsg& msg);
void ProcHelperGuildStashViewRep(CNetMsg& msg);
void ProcHelperGuildStashTakeRep(CNetMsg& msg);
void ProcHelperGuildStashSaveTaxRep(CNetMsg& msg);

void ProcHelperPartyMemberInfo(CNetMsg& msg);
void ProcHelperPartyMemberChangeJob(CNetMsg& msg);
void ProcHelperPartyChat(CNetMsg& msg);
void ProcHelperPartyRecallPrompt(CNetMsg& msg);
void ProcHelperPartyRecallConfirm(CNetMsg& msg);
void ProcHelperPartyRecallProc(CNetMsg& msg);

void ProcHelperFameupRep(CNetMsg& msg);
void ProcHelperTeacherLoadRep(CNetMsg& msg);

void ProcHelperGuildCreateRep(CNetMsg& msg);
void ProcHelperGuildCreateNotify(CNetMsg& msg);
void ProcHelperGuildOnlineNotify(CNetMsg& msg);
void ProcHelperGuildMemberList(CNetMsg& msg);
void ProcHelperGuildLoadRep(CNetMsg& msg);
void ProcHelperGuildLevelUpRep(CNetMsg& msg);
void ProcHelperGuildLevelUpNotify(CNetMsg& msg);
void ProcHelperGuildBreakUpRep(CNetMsg& msg);
void ProcHelperGuildBreakUpNotify(CNetMsg& msg);
void ProcHelperGuildMemberAddNotify(CNetMsg& msg);
void ProcHelperGuildMemberAddRep(CNetMsg& msg);
void ProcHelperGuildMemberOutNotify(CNetMsg& msg);
void ProcHelperGuildMemberOutRep(CNetMsg& msg);
void ProcHelperGuildMemberKickRep(CNetMsg& msg);
void ProcHelperGuildMemberKickNotify(CNetMsg& msg);
void ProcHelperGuildChangeBossNotify(CNetMsg& msg);
void ProcHelperGuildChangeBossRep(CNetMsg& msg);
void ProcHelperGuildAppointOfficerRep(CNetMsg& msg);
void ProcHelperGuildAppointOfficerNotify(CNetMsg& msg);
void ProcHelperGuildChat(CNetMsg& msg);
void ProcHelperGuildFireOfficerRep(CNetMsg& msg);
void ProcHelperGuildFireOfficerNotify(CNetMsg& msg);
void ProcHelperGuildLoadNotify(CNetMsg& msg);

void ProcHelperGuildBattleRep(CNetMsg& msg);
void ProcHelperGuildBattleStart(CNetMsg& msg);
void ProcHelperGuildBattleStopRep(CNetMsg& msg);
void ProcHelperGuildBattleStatus(CNetMsg& msg);
void ProcHelperGuildBattlePeaceRep(CNetMsg& msg);
void ProcHelperEventMoonStoneLoad(CNetMsg& msg);
void ProcHelperEventMoonStoneUpdate(CNetMsg& msg);
void ProcHelperEventMoonStoneJackpot(CNetMsg& msg);

#ifdef GUILD_MARK_TABLE
void ProcHelperGuildMarkTable(CNetMsg & msg);
#endif // GUILD_MARK_TABLE

#ifdef ENABLE_MESSENGER
void ProcHelper_FriendAddRep(CNetMsg& msg);
void ProcHelper_FriendDelRep(CNetMsg& msg);
void ProcHelper_FriendSetConditionRep(CNetMsg& msg);
#ifdef MESSENGER_NEW
void ProcHelper_BlockPCRep(CNetMsg& msg);
#endif
#endif // #ifdef ENABLE_MESSENGER

#ifdef CASH_ITEM_GIFT
void ProcHelper_GiftRecvChar(CNetMsg& msg);
#endif

#ifdef ENABLE_PET
void ProcHelperPetCreateRep(CNetMsg& msg);
// TODO : DELETE : 
#ifdef NEW_UI
void ProcHelperPetDeleteRep(CNetMsg& msg);
#endif // NEW_UI

#ifdef PET_NAME_CHANGE
void ProcHelperPetChangeName( CNetMsg& msg );
#endif // PET_NAME_CHANGE
#endif // #ifdef ENABLE_PET

#ifdef EVENT_SEARCHFRIEND
void ProcHelper_EventSearchFriendAddRep(CNetMsg& msg);
void ProcHelper_EventSearchFriendSelectAddRep(CNetMsg& msg);
void ProcHelper_EventSearchFriendOneTimeAddRep(CNetMsg& msg);
void ProcHelper_EventSearchFriendListGoodRep(CNetMsg& msg);
void ProcHelper_EventSearchFriendGoodRep(CNetMsg& msg);
#endif // #ifdef EVENT_SEARCHFRIEND
///////////////////////
// pd4 helper rep : bw

void ProcHelperPD4RankViewRep(CNetMsg& msg);
void ProcHelperPD4RewardViewRep(CNetMsg& msg);
void ProcHelperPD4RewardRep(CNetMsg& msg);

void ProcHelperNameChange(CNetMsg& msg);

void ProcHelperPartyInviteReq(CNetMsg& msg);
void ProcHelperPartyInviteRep(CNetMsg& msg);
void ProcHelperPartyAllowRep(CNetMsg& msg);
void ProcHelperPartyRejectRep(CNetMsg& msg);
void ProcHelperPartyQuitRep(CNetMsg& msg);
void ProcHelperPartyKickRep(CNetMsg& msg);
void ProcHelperPartyChangeBossRep(CNetMsg& msg);

#ifdef NEW_UI
void ProcHelperPartyChangeTypeRep(CNetMsg& msg);
#endif // NEW_UI

#ifdef PARTY_RAID
void ProcHelperPartyEndPartyRep(CNetMsg& msg);
#endif //PARTY_RAID

#ifdef PARTY_MATCHING
void ProcHelperPartyMatchRegMemberRep(CNetMsg& msg);
void ProcHelperPartyMatchRegPartyRep(CNetMsg& msg);
void ProcHelperPartyMatchDelRep(CNetMsg& msg);
void ProcHelperPartyMatchInviteRep(CNetMsg& msg);
void ProcHelperPartyMatchJoinRep(CNetMsg& msg);
void ProcHelperPartyMatchJoinAllowRep(CNetMsg& msg);
void ProcHelperPartyMatchJoinRejectRep(CNetMsg& msg);
void ProcHelperPartyMatchMemberChangeInfo(CNetMsg& msg);
#endif // PARTY_MATCHING

void ProcHelperPartyInfoParty(CNetMsg& msg);
#ifdef PARTY_MATCHING
void ProchelperPartyInfoPartyMatchMember(CNetMsg& msg);
void ProchelperPartyInfoPartyMatchParty(CNetMsg& msg);
#endif // PARTY_MATCHING

void ProcHelperPartyInfoEnd(CNetMsg& msg);

#ifdef EVENT_CHILDRENSDAY_2007
void ProcHelperChildrenEvent(CNetMsg& msg);
#endif //EVENT_CHILDRENSDAY_2007

#ifdef EVENT_FLOWERTREE_2007
void ProcHelperFlowerTreeEvent(CNetMsg& msg);
#endif //EVENT_FLOWERTREE_2007

#ifdef EVENT_2007_PARENTSDAY
void ProcHelperAccumulatePointList( CNetMsg& msg );
void ProHelperAddPoint( CNetMsg& msg );
void ProHelperExchangeTicket( CNetMsg& msg );
void ProHelperExchangeItem( CNetMsg& msg );
void ProHelperParentsGetItemNotice( CNetMsg& msg );
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_TEACH_2007
void ProcHelperTeach2007Addflower(CNetMsg & msg);
#endif // EVENT_TEACH_2007

#ifdef NEW_GUILD
void ProcHelperGuildInclineEstablish( CNetMsg& msg );
void ProcHelperGuildMemberAdjust( CNetMsg& msg );
void ProcHelperGuildInfoNotice( CNetMsg& msg );
void ProcHelperNewGuildMemberListRep( CNetMsg& msg );
void ProcHelperNewGuilManageRep( CNetMsg& msg );
void ProcHelperNewGuildNotifyRep( CNetMsg& msg );
void ProcHelperNewGuildNotifyUpdateReP( CNetMsg& msg );
void ProcHelperNewGuildNotifyTrans( CNetMsg& msg );
void ProcHelperNewGuildNotifyTransRepMsg( CNetMsg& msg );
void ProcHelperNewGuildSkillListRepMsg( CNetMsg& msg );
void ProcHelperNewGuildLoadNTF( CNetMsg& msg );
void ProcHelperNewGuildMemberList( CNetMsg& msg );
void ProcHelperNewGuildInfoRepMsg( CNetMsg& msg );
void ProcHelperNewGuildPointUpdateMsg( CNetMsg& msg );
void ProcHelperNewGuildNotice( CNetMsg& msg );
void ProcHelperNewGuildMemberPointSaveMsg( CNetMsg& msg );
#endif // NEW_GUILD

#ifdef EVENT_OPEN_ADULT_SERVER
void ProcHelperOpenAdultServerRep( CNetMsg& msg);
#endif // EVENT_OPEN_ADULT_SERVER

#ifdef REWARD_IDC2007
void ProcHelperRewardIDC2007( CNetMsg& msg );
#endif // REWARD_IDC2007

#ifdef EVENT_HALLOWEEN_2007
void ProcHelperHalloween2007( CNetMsg& msg );
#endif //EVENT_HALLOWEEN_2007

#ifdef DRATAN_CASTLE
#ifdef DYNAMIC_DUNGEON
void ProcHelperDVDRateChange(CNetMsg& msg);
#endif //DYNAMIC_DUNGEON
#endif // DRATAN_CASTLE

#ifdef ALTERNATE_MERCHANT
void ProcHelperAlternateMerchant( CNetMsg& msg );
void ProcHelperAlternateMerchantProductRecovery( CNetMsg& msg );
void ProcHelperAlternateMerchantNasRecovery( CNetMsg& msg );
void ProcHelperAlternateMerchantStart( CNetMsg& msg );
#endif // ALTERNATE_MERCHANT

#ifdef MONSTER_COMBO
void ProcHelperComboGotoWaitRoomPrompt( CNetMsg& msg );
void ProcHelperComboRecallToWaitRoomPrompt(CNetMsg& msg);
#endif // MONSTER_COMBO

#ifdef IRIS_POINT
void ProcHelperToConnAddIpoint( CNetMsg& msg );
#endif //IRIS_POINT

#ifdef ATTACK_PET
void ProcHelperAttackPet( CNetMsg& msg );
#endif // ATTACK_PET

#ifdef EXTREME_CUBE
void ProcHelperCubeStateRep(CNetMsg& msg);
void ProcHelperCubeStatePersonalRep(CNetMsg& msg);
void ProcHelperGuildCubeNotice(CNetMsg& msg);
#endif // EXTREME_CUBE

#ifdef EVENT_PHOENIX	// //피닉스 이벤트 yhj	
void ProcHelperEventPhoenix( CNetMsg& msg);
#endif  // EVENT_PHOENIX

#ifdef TRADE_AGENT
void ProcHelperTradeAgentErrorRep( CNetMsg& msg);
void ProcHelperTradeAgentRegListRep( CNetMsg& msg);
void ProcHelperTradeAgentRegRep( CNetMsg& msg);
void ProcHelperTradeAgentRegCancelRep( CNetMsg& msg);
void ProcHelperTradeAgentSearchRep( CNetMsg& msg);
void ProcHelperTradeAgentReturned( CNetMsg& msg);
void ProcHelperTradeAgentBuyRep( CNetMsg& msg);
void ProcHelperTradeAgentCalcListRep( CNetMsg& msg);
void ProcHelperTradeAgentCalculateRep( CNetMsg& msg);
void ProcHelperTradeAgentCheckCalcRep( CNetMsg& msg);
#endif // TRADE_AGENT

#ifdef EXPEDITION_RAID
void ProcHelperExpedErrorRep( CNetMsg& msg);

void ProcHelperExped_CreateRep( CNetMsg& msg);
void ProcHelperExped_InviteRep( CNetMsg& msg);
void ProcHelperExped_AllowRep( CNetMsg& msg);
void ProcHelperExped_RejectRep( CNetMsg& msg);
void ProcHelperExped_QuitRep( CNetMsg& msg);
void ProcHelperExped_KickRep( CNetMsg& msg);
void ProcHelperExped_ChangeTypeRep( CNetMsg& msg);
void ProcHelperExped_ChangeBossRep( CNetMsg& msg);
void ProcHelperExped_SetMBossRep( CNetMsg& msg);
void ProcHelperExped_ResetMBossRep( CNetMsg& msg);
void ProcHelperExped_EndExpedRep( CNetMsg& msg);
void ProcHelperExped_MoveGroupRep( CNetMsg& msg);
void ProcHelperExped_SetLabelRep( CNetMsg& msg);
void ProcHelperExped_RejoinRep( CNetMsg& msg);
void ProcHelperExped_ChatRep( CNetMsg& msg);
#endif	//EXPEDITION_RAID

void CServer::ProcessHelperMessage(CNetMsg& msg)
{
	msg.MoveFirst();

	switch (msg.m_mtype)
	{
	case MSG_HELPER_COMMAND:
		{
			int subtype;
			msg >> subtype;
			switch (subtype)
			{
			case MSG_HELPER_PARTY_INFO_PARTY:
				ProcHelperPartyInfoParty(msg);
				break;

#ifdef PARTY_MATCHING
			case MSG_HELPER_PARTY_INFO_PARTY_MATCH_MEMBER:
				ProchelperPartyInfoPartyMatchMember(msg);
				break;

			case MSG_HELPER_PARTY_INFO_PARTY_MATCH_PARTY:
				ProchelperPartyInfoPartyMatchParty(msg);
				break;
#endif // PARTY_MATCHING

			case MSG_HELPER_PARTY_INFO_END:
				ProcHelperPartyInfoEnd(msg);
				break;

#ifdef PARTY_MATCHING
			case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO:
				ProcHelperPartyMatchMemberChangeInfo(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_REG_MEMBER_REP:
				ProcHelperPartyMatchRegMemberRep(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_REG_PARTY_REP:
				ProcHelperPartyMatchRegPartyRep(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_DEL_REP:
				ProcHelperPartyMatchDelRep(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_INVITE_REP:
				ProcHelperPartyMatchInviteRep(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_JOIN_REP:
				ProcHelperPartyMatchJoinRep(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_JOIN_ALLOW_REP:
				ProcHelperPartyMatchJoinAllowRep(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_JOIN_REJECT_REP:
				ProcHelperPartyMatchJoinRejectRep(msg);
				break;
#endif // PARTY_MATCHING

			case MSG_HELPER_PARTY_CHANGE_BOSS_REP:
				ProcHelperPartyChangeBossRep(msg);
				break;

#ifdef NEW_UI
			case MSG_HELPER_PARTY_CHANGE_TYPE_REP:
				ProcHelperPartyChangeTypeRep(msg);
				break;
#endif // NEW_UI

#ifdef PARTY_RAID
			case MSG_HELPER_PARTY_END_PARTY_REP:
				ProcHelperPartyEndPartyRep(msg);
#endif //PARTY_RAID

			case MSG_HELPER_PARTY_KICK_REP:
				ProcHelperPartyKickRep(msg);
				break;

			case MSG_HELPER_PARTY_QUIT_REP:
				ProcHelperPartyQuitRep(msg);
				break;

			case MSG_HELPER_PARTY_REJECT_REP:
				ProcHelperPartyRejectRep(msg);
				break;

			case MSG_HELPER_PARTY_ALLOW_REP:
				ProcHelperPartyAllowRep(msg);
				break;

			case MSG_HELPER_PARTY_INVITE_REP:
				ProcHelperPartyInviteRep(msg);
				break;

			case MSG_HELPER_PARTY_INVITE_REQ:
				ProcHelperPartyInviteReq(msg);
				break;

			case MSG_HELPER_GUILD_CREATE_REP:
				ProcHelperGuildCreateRep(msg);
				break;

			case MSG_HELPER_GUILD_CREATE_NTF:
				ProcHelperGuildCreateNotify(msg);
				break;

			case MSG_HELPER_GUILD_ONLINE_NTF:
				ProcHelperGuildOnlineNotify(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_LIST:
				ProcHelperGuildMemberList(msg);
				break;

			case MSG_HELPER_GUILD_LOAD_REP:
				ProcHelperGuildLoadRep(msg);
				break;

			case MSG_HELPER_GUILD_LEVELUP_REP:
				ProcHelperGuildLevelUpRep(msg);
				break;

			case MSG_HELPER_GUILD_LEVELUP_NTF:
				ProcHelperGuildLevelUpNotify(msg);
				break;

			case MSG_HELPER_GUILD_BREAKUP_REP:
				ProcHelperGuildBreakUpRep(msg);
				break;

			case MSG_HELPER_GUILD_BREAKUP_NTF:
				ProcHelperGuildBreakUpNotify(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_ADD_NTF:		// 길드 멤버 추가 알림
				ProcHelperGuildMemberAddNotify(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_ADD_REP:
				ProcHelperGuildMemberAddRep(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_OUT_NTF:
				ProcHelperGuildMemberOutNotify(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_OUT_REP:
				ProcHelperGuildMemberOutRep(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_KICK_REP:
				ProcHelperGuildMemberKickRep(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_KICK_NTF:
				ProcHelperGuildMemberKickNotify(msg);
				break;

			case MSG_HELPER_GUILD_CHANGE_BOSS_REP:
				ProcHelperGuildChangeBossRep(msg);
				break;

			case MSG_HELPER_GUILD_CHANGE_BOSS_NTF:
				ProcHelperGuildChangeBossNotify(msg);
				break;

			case MSG_HELPER_GUILD_APPOINT_OFFICER_REP:
				ProcHelperGuildAppointOfficerRep(msg);
				break;

			case MSG_HELPER_GUILD_APPOINT_OFFICER_NTF:
				ProcHelperGuildAppointOfficerNotify(msg);
				break;

			case MSG_HELPER_GUILD_CHAT:
				ProcHelperGuildChat(msg);
				break;

			case MSG_HELPER_GUILD_FIRE_OFFICER_REP:
				ProcHelperGuildFireOfficerRep(msg);
				break;

			case MSG_HELPER_GUILD_FIRE_OFFICER_NTF:
				ProcHelperGuildFireOfficerNotify(msg);
				break;

			case MSG_HELPER_GUILD_LOAD_NTF:
				ProcHelperGuildLoadNotify(msg);
				break;

			case MSG_HELPER_GUILD_BATTLE_REP:
				ProcHelperGuildBattleRep(msg);
				break;

			case MSG_HELPER_GUILD_BATTLE_START:
				ProcHelperGuildBattleStart(msg);
				break;

			case MSG_HELPER_GUILD_BATTLE_STOP_REP:
				ProcHelperGuildBattleStopRep(msg);
				break;

			case MSG_HELPER_GUILD_BATTLE_STATUS:
				ProcHelperGuildBattleStatus(msg);
				break;

			case MSG_HELPER_GUILD_BATTLE_PEACE_REP:
				ProcHelperGuildBattlePeaceRep(msg);
				break;
			case MSG_HELPER_GUILD_MARK_TABLE:
#ifdef GUILD_MARK_TABLE
				ProcHelperGuildMarkTable(msg);
#endif // GUILD_MARK_TABLE
				break;
//0503 kwon			
			case MSG_HELPER_EVENT_MOONSTONE_LOAD:
			case MSG_HELPER_EVENT_MOONSTONE_UPDATE_REP:
				ProcHelperEventMoonStoneLoad(msg);
				break;		
			case MSG_HELPER_EVENT_MOONSTONE_JACKPOT_REP:
				ProcHelperEventMoonStoneJackpot(msg);
				break;
#ifdef ENABLE_MESSENGER
			case MSG_HELPER_FRIEND_MEMBER_ADD_REP:
				ProcHelper_FriendAddRep(msg);
				break;
			case MSG_HELPER_FRIEND_MEMBER_DEL_REP:
				ProcHelper_FriendDelRep(msg);
				break;
			case MSG_HELPER_FRIEND_SET_CONDITION_NOTIFY:
				ProcHelper_FriendSetConditionRep(msg);
				break;	
#endif // #ifdef ENABLE_MESSENGER

////////////////////////
// pd4 헬퍼 rep 처리 : bw

			case MSG_HELPER_PD4_RANK_VIEW_REP:
				ProcHelperPD4RankViewRep(msg);
				break;
			case MSG_HELPER_PD4_REWARD_VIEW_REP:
				ProcHelperPD4RewardViewRep(msg);
				break;
			case MSG_HELPER_PD4_REWARD_REP:
				ProcHelperPD4RewardRep(msg);
				break;
			case MSG_HELPER_NAME_CHANGE_REP:
				ProcHelperNameChange(msg);
				break;

// 애완동물
#ifdef ENABLE_PET
			case MSG_HELPER_PET_CREATE_REP:
				ProcHelperPetCreateRep(msg);
				break;
// TODO : DELETE

#ifdef NEW_UI
			case MSG_HELPER_PET_DELETE_REP:
				ProcHelperPetDeleteRep(msg);
				break;
#endif // NEW_UI

#ifdef PET_NAME_CHANGE
			case MSG_HELPER_PET_NAME_CHANGE:
				ProcHelperPetChangeName( msg );
				break;
#endif // PET_NAME_CHAGE
#endif // #ifdef ENABLE_PET

#ifdef ENABLE_WAR_CASTLE
			case MSG_HELPER_GUILD_STASH_HISTORY_REP:
				ProcHelperGuildStashHistoryRep(msg);
				break;
			case MSG_HELPER_GUILD_STASH_VIEW_REP:
				ProcHelperGuildStashViewRep(msg);
				break;
			case MSG_HELPER_GUILD_STASH_TAKE_REP:
				ProcHelperGuildStashTakeRep(msg);
				break;
			case MSG_HELPER_GUILD_STASH_SAVE_TAX_REP:
				ProcHelperGuildStashSaveTaxRep(msg);
				break;
#endif // #ifdef ENABLE_WAR_CASTLE
			case MSG_HELPER_TEACHER_FAMEUP_REP:
				ProcHelperFameupRep(msg);
				break;
			case MSG_HELPER_TEACHER_LOAD_REP:
				ProcHelperTeacherLoadRep(msg);
				break;
#ifdef EVENT_SEARCHFRIEND
			case MSG_HELPER_SEARCHFRIEND_ADD_REP:
				ProcHelper_EventSearchFriendAddRep(msg);
				break;
			case MSG_HELPER_SEARCHFRIEND_SELECT_ADD_REP:
				ProcHelper_EventSearchFriendSelectAddRep(msg);
				break;
			case MSG_HELPER_SEARCHFRIEND_ONETIME_ADD_REP:
				ProcHelper_EventSearchFriendOneTimeAddRep(msg);
				break;
			case MSG_HELPER_SEARCHFRIEND_LISTGOOD_REP:
				ProcHelper_EventSearchFriendListGoodRep(msg);
				break;
			case MSG_HELPER_SEARCHFRIEND_GOOD_REP:
				ProcHelper_EventSearchFriendGoodRep(msg);
				break;
#endif // #ifdef EVENT_SEARCHFRIEND
#ifdef MESSENGER_NEW
			case MSG_HELPER_BLOCKPC_REP:
				ProcHelper_BlockPCRep(msg);
				break;
#endif
#ifdef CASH_ITEM_GIFT
			case MSG_HELPER_GIFT_RECVCHARNAME_REP:
				ProcHelper_GiftRecvChar(msg);
				break;
#endif
#ifdef EVENT_CHILDRENSDAY_2007
			case MSG_HELPER_CHILDRENSDAY_2007:
				ProcHelperChildrenEvent(msg);
				break;
#endif //EVENT_CHILDRENSDAY_2007

#ifdef EVENT_FLOWERTREE_2007
			case MSG_HELPER_FLOWERTREE_2007:
				ProcHelperFlowerTreeEvent(msg);
				break;
#endif //EVENT_FLOWERTREE_2007

#ifdef EVENT_2007_PARENTSDAY
			case MSG_HELPER_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST:
				ProcHelperAccumulatePointList( msg );
				break;
			case MSG_HELPER_PARENTSDAY_2007_ADD_POINT:
				ProHelperAddPoint( msg );
				break;
			case MSG_HELPER_PARENTSDAY_2007_EXCHANGE_TICKET:
				ProHelperExchangeTicket( msg );
				break;
			case MSG_HELPER_PARENTSDAY_2007_EXCHANGE_ITEM:
				ProHelperExchangeItem( msg );
				break;

			case MSG_HELPER_PARENTSDAY_2007_NOTICE:
				ProHelperParentsGetItemNotice( msg );
				break;
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_TEACH_2007
			case MSG_HELPER_TEACH_2007_ADDFLOWER:
				ProcHelperTeach2007Addflower(msg);
				break;
#endif // EVENT_TEACH_2007

#ifdef NEW_GUILD
			case MSG_HELPER_GUILD_INCLINE_ESTABLISH_REP:
				ProcHelperGuildInclineEstablish( msg );
				break;
			case MSG_HELPER_GUILD_MEMBER_ADJUST_REP:
				ProcHelperGuildMemberAdjust( msg );
				break;
			case MSG_HELPER_NEW_GUILD_INFO_NTF:
				ProcHelperGuildInfoNotice( msg );
				break;
			case MSG_HELPER_NEW_GUILD_MEMBERLIST_REP:
				ProcHelperNewGuildMemberListRep( msg );
				break;
			case MSG_HELPER_NEW_GUILD_MANAGE_REP:
				ProcHelperNewGuilManageRep( msg );
				break;
			case MSG_HELPER_NEW_GUILD_NOTICE_REP:
				ProcHelperNewGuildNotifyRep( msg );
				break;
			case MSG_HELPER_NEW_GUILD_NOTICE_UPDATE_REP:
				ProcHelperNewGuildNotifyUpdateReP( msg );
				break;
			case MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_NTF:
				ProcHelperNewGuildNotifyTrans( msg );
				break;
			case MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_REP:
				ProcHelperNewGuildNotifyTransRepMsg( msg );
				break;
			case MSG_HELPER_NEW_GUILD_NOTICE_SKILLLIST_REP:
				ProcHelperNewGuildSkillListRepMsg( msg );
				break;
			case MSG_HELPER_NEW_GUILD_LOAD_NTF:
				ProcHelperNewGuildLoadNTF( msg );
				break;
			case MSG_HELPER_NEW_GUILD_MEMBER_LIST:
				ProcHelperNewGuildMemberList( msg );
				break;

			case MSG_HELPER_NEW_GUILD_INFO_REP:
				ProcHelperNewGuildInfoRepMsg( msg );
				break;
			case MSG_HELPER_UPDATE_GUILD_POINT:
				ProcHelperNewGuildPointUpdateMsg( msg );
				break;
			case MSG_HELPER_GUILD_NOTICE:
				ProcHelperNewGuildNotice( msg );
				break;
			case MSG_HELPER_SAVE_GUILD_MEMBER_POINT:
				ProcHelperNewGuildMemberPointSaveMsg( msg );
				break;	
#endif // NEW_GUILD

#ifdef EVENT_OPEN_ADULT_SERVER
			case MSG_HELPER_OPEN_ADULT_SERVER_REP:
				ProcHelperOpenAdultServerRep( msg );
				break;
#endif // EVENT_OPEN_ADULT_SERVER

#ifdef REWARD_IDC2007
			case MSG_HELPER_REWARD_IDC2007_REP:
				ProcHelperRewardIDC2007( msg);
				break;
#endif // REWARD_IDC2007

#ifdef EVENT_HALLOWEEN_2007
			case MSG_HELPER_HALLOWEEN_2007:
				ProcHelperHalloween2007( msg );
				break;
#endif //EVENT_HALLOWEEN_2007
				
#ifdef DYNAMIC_DUNGEON
			case MSG_HELPER_DVD_RATE_CHANGE:
				ProcHelperDVDRateChange( msg );
				break;
#endif //DYNAMIC_DUNGEON

#ifdef ALTERNATE_MERCHANT
			case MSG_HELPER_ALTERNATEMERCHANT:
				ProcHelperAlternateMerchant( msg );
				break;
			case MSG_HELPER_ALTERNATEMERCHANT_PRODUCT_RECOVERY:
				ProcHelperAlternateMerchantProductRecovery( msg );
				break;
			case MSG_HELPER_ALTERNATEMERCHANT_NAS_RECOVERY:
				ProcHelperAlternateMerchantNasRecovery( msg );
				break;
			case MSG_HELPER_ALTERNATEMERCHANT_START:
				ProcHelperAlternateMerchantStart( msg );
				break;
#endif // ALTERNATE_MERCHANT

#ifdef IRIS_POINT
			case MSG_HELPER_TO_CONN_ADD_IPOINT:
				ProcHelperToConnAddIpoint( msg );
				break;
#endif //IRIS_POINT
#ifdef ATTACK_PET
			case MSG_HELPER_APET :
				ProcHelperAttackPet(msg);
				break;
#endif //ATTACK_PET
#ifdef EXTREME_CUBE
			case MSG_HELPER_CUBESTATE_REP:
				ProcHelperCubeStateRep(msg);
				break;
			case MSG_HELPER_CUBESTATE_PERSONAL_REP:
				ProcHelperCubeStatePersonalRep(msg);
				break;
#endif // EXTREME_CUBE
				
#ifdef EVENT_PHOENIX	// //피닉스 이벤트 yhj	
			case MSG_HELPER_EVENT_PHOENIX:
				ProcHelperEventPhoenix(msg);
				break;
#endif  // EVENT_PHOENIX
				
#ifdef TRADE_AGENT
			case MSG_HELPER_TRADEAGENT_ERROR:
				ProcHelperTradeAgentErrorRep(msg);
				break;
			case MSG_HELPER_TRADEAGENT_REG_LIST_REP:
				ProcHelperTradeAgentRegListRep(msg);
				break;
			case MSG_HELPER_TRADEAGENT_REG_REP:
				ProcHelperTradeAgentRegRep(msg);
				break;
			case MSG_HELPER_TRADEAGENT_REG_CANCEL_REP:
				ProcHelperTradeAgentRegCancelRep(msg);
				break;
			case MSG_HELPER_TRADEAGENT_SEARCH_REP:
				ProcHelperTradeAgentSearchRep(msg);
				break;
			case MSG_HELPER_TRADEAGENT_RETURNED :
				ProcHelperTradeAgentReturned(msg);
				break;
			case MSG_HELPER_TRADEAGENT_BUY_REP:
				ProcHelperTradeAgentBuyRep(msg);
				break;	
			case MSG_HELPER_TRADEAGENT_CALCLIST_REP:
				ProcHelperTradeAgentCalcListRep(msg);
				break;	
			case MSG_HELPER_TRADEAGENT_CALCULATE_REP:
				ProcHelperTradeAgentCalculateRep(msg);
				break;	
			case MSG_HELPER_TRADEAGENT_CHECKCALC_REP:
				ProcHelperTradeAgentCheckCalcRep(msg);
				break;	
				
#endif	//TRADE_AGENT

#ifdef EXPEDITION_RAID
			case MSG_HELPER_EXPED_ERROR:
				ProcHelperExpedErrorRep(msg);
				break;
			case MSG_HELPER_EXPED_CREATE_REP:
				ProcHelperExped_CreateRep(msg);
				break;
			case MSG_HELPER_EXPED_INVITE_REP:
				ProcHelperExped_InviteRep(msg);				
				break;
			case MSG_HELPER_EXPED_ALLOW_REP:
				ProcHelperExped_AllowRep(msg);
				break;
			case MSG_HELPER_EXPED_REJECT_REP:
				ProcHelperExped_RejectRep(msg);
				break;
			case MSG_HELPER_EXPED_QUIT_REP:
				ProcHelperExped_QuitRep(msg);
				break;
			case MSG_HELPER_EXPED_KICK_REP:
				ProcHelperExped_KickRep(msg);
				break;
// 			case MSG_HELPER_EXPED_MEMBERINFO_REP:
// 				ProcHelperExped_MemberInfoRep(msg);
// 				break;
// 			case MSG_HELPER_EXPED_ADMIN_REP:
//				ProcHelperExped_AdminRep(msg);
// 				break;
			case MSG_HELPER_EXPED_CHANGETYPE_REP:
				ProcHelperExped_ChangeTypeRep(msg);
				break;
			case MSG_HELPER_EXPED_CHANGEBOSS_REP:
				ProcHelperExped_ChangeBossRep(msg);
				break;
			case MSG_HELPER_EXPED_SETMBOSS_REP:
				ProcHelperExped_SetMBossRep(msg);
				break;
			case MSG_HELPER_EXPED_RESETMBOSS_REP:
				ProcHelperExped_ResetMBossRep(msg);
				break;
// 			case MSG_HELPER_EXPED_VIEWDETAIL_REP:	 	
// 				ProcHelperExped_ViewDetailRep(msg);
// 				break;
			case MSG_HELPER_EXPED_ENDEXPED_REP:
				ProcHelperExped_EndExpedRep(msg);
				break;
			case MSG_HELPER_EXPED_MOVEGROUP_REP:
				ProcHelperExped_MoveGroupRep(msg);
				break;
// 			case MSG_HELPER_EXPED_ADDMEMBER_REP:
// 				ProcHelperExped_AddMemberRep(msg);
// 				break;
			case MSG_HELPER_EXPED_SET_LABEL_REP:
				ProcHelperExped_SetLabelRep(msg);
				break;			
#endif //EXPEDITION_RAID

			}
		}
		break;

	case MSG_HELPER_REQ:
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
			case MSG_HELPER_PARTY_MEMBER_CHANGE_JOB:
				ProcHelperPartyMemberChangeJob(msg);
				break;

			case MSG_HELPER_PARTY_CHAT:
				ProcHelperPartyChat(msg);
				break;

			case MSG_HELPER_PARTY_RECALL_PROMPT:
				ProcHelperPartyRecallPrompt(msg);
				break;

			case MSG_HELPER_PARTY_RECALL_CONFIRM:
				ProcHelperPartyRecallConfirm(msg);
				break;

			case MSG_HELPER_PARTY_RECALL_PROC:
				ProcHelperPartyRecallProc(msg);
				break;

			case MSG_HELPER_WAR_NOTICE_END:
				ProcHelperWarNoticeEnd(msg);
				break;

			case MSG_HELPER_WAR_NOTICE_REMAIN_FIELD_TIME:
				ProcHelperWarNoticeRemainFieldTime(msg);
				break;

			case MSG_HELPER_WAR_NOTICE_START_ATTACK_CASTLE:
				ProcHelperWarNoticeStartAttackCastle(msg);
				break;

			case MSG_HELPER_WAR_JOIN_ATTACK_GUILD:
				ProcHelperWarJoinAttackGuild(msg);
				break;

			case MSG_HELPER_WAR_JOIN_DEFENSE_GUILD:
				ProcHelperWarJoinDefenseGuild(msg);
				break;

			case MSG_HELPER_WAR_NOTICE_START:
				ProcHelperWarNoticeStart(msg);
				break;

			case MSG_HELPER_WAR_NOTICE_TIME_REMAIN:
				ProcHelperWarNoticeTimeRemain(msg);
				break;

			case MSG_HELPER_WAR_NOTICE_TIME:
				ProcHelperWarNoticeTime(msg);
				break;

			case MSG_HELPER_WHISPER_REQ:
				ProcHelperWhisperReq(msg, seq, server, subno, zone);
				break;
#ifdef MONSTER_COMBO
			case MSG_HELPER_COMBO_GOTO_COMBO_PROMPT:
				ProcHelperComboGotoWaitRoomPrompt(msg);
				break;
			case MSG_HELPER_COMBO_RECALL_TO_COMBO_PROMPT:
				ProcHelperComboRecallToWaitRoomPrompt(msg);
				break;
#endif // MONSTER_COMBO
#ifdef EXTREME_CUBE
			case MSG_HELPER_GUILDCUBETIME_NOTICE:
				ProcHelperGuildCubeNotice(msg);
				break;
#endif // EXTREME_CUBE
			}
		}
		break;

	case MSG_HELPER_REP:
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
			case MSG_HELPER_WHISPER_REP:
				ProcHelperWhisperRep(msg, seq, server, subno, zone);
				break;


			case MSG_HELPER_WHISPER_NOTFOUND:
				ProcHelperWhisperNotfound(msg, seq, server, subno, zone);
				break;
			}
		}
		break;

	default:
		break;
	}
}

void ProcHelperNameChange(CNetMsg& msg)
{
#ifdef FEEITEM
	// TODO : BS : 길드와 캐릭터명의 길이가 달라지면 바꿔야함
	char bguild;
	char error;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	int index;
	CNetMsg rmsg;

	msg >> bguild
		>> index
		>> name
		>> error;
	CPC* pc = NULL;

	if(bguild == 1)
	{
		 pc = gserver.m_guildlist.findguild(index)->boss()->GetPC();

		 if( !pc ) 
		{
			GAMELOG << init("CASH_ITEM_USE_ERROR-notPC")
					<< bguild << delim
					<< index << delim
					<< 843 << end;
			return;
		}

		if(error == MSG_EX_NAMECHANGE_ERROR_SUCCESS)
		{
			// 아이템 삭제 / 서버 변수 변경 // 길드 개명카드 삭제
			
			// 아이템 검사
			int r, c;
			if (!pc->m_invenNormal.FindItem(&r, &c, 843, 0, 0))
					return ;

			// 카운트 줄이기
			CItem* item = pc->m_invenNormal.GetItem(r, c);

			if( !item )
			{
				NameChangeRepMsg(rmsg, MSG_EX_NAMECHANGE_ERROR_HELPER, name, bguild);
				SEND_Q(rmsg, pc->m_desc);

				//롤백
				HelperNameChangeReqMsg(rmsg, 3, pc->m_guildInfo->guild()->index(), pc->m_guildInfo->guild()->name());

				if(IS_RUNNING_HELPER)
				{
					SEND_Q(msg, gserver.m_helper);
				}

				GAMELOG << init("CASH_ITEM_USE_ERROR-notItem")
						<< bguild << delim
						<< index << delim
						<< 843 << end;
				return;
			}

			if( (item->m_itemProto->m_flag & ITEM_FLAG_CASH) )
			{
				GAMELOG << init("CASH_ITEM_USE", pc)
						<< itemlog(item) << delim
						<< pc->m_guildInfo->guild()->name() << delim
						<< name << delim
						<< end;
			}

			pc->m_guildInfo->guild()->changeName(name);

			// Item 수량 변경
			ItemUseMsg(rmsg, pc, item->tab(), item->row(), item->col(), item->m_itemProto->m_index, 0);
			SEND_Q(rmsg, pc->m_desc);
			DecreaseFromInventory(pc, item, 1);
			if (item->Count() > 0)
			{
				ItemUpdateMsg(rmsg, item, -1);
				SEND_Q(rmsg, pc->m_desc);
			}
			else
			{
				ItemDeleteMsg(rmsg, item);
				SEND_Q(rmsg, pc->m_desc);
				RemoveFromInventory(pc, item, true, true);
			}


			HelperNameChangeReqMsg(rmsg, 5, pc->m_guildInfo->guild()->index(), name);

			if(IS_RUNNING_HELPER)
			{
				SEND_Q(rmsg, gserver.m_helper);
			}

			return;
		}
		NameChangeRepMsg(rmsg, (MSG_EX_NAMECHANGE_ERROR_TYPE) error, name, bguild); 
		SEND_Q(rmsg, pc->m_desc);

	}
	else if( bguild == 5)
	{
		CGuild* guild = gserver.m_guildlist.findguild(index);
		if( guild == NULL) return;

		guild->changeName(name);
		NameChangeRepMsg(rmsg, (MSG_EX_NAMECHANGE_ERROR_TYPE) error, name, bguild); 
		guild->SendToAll(rmsg, true);
		return;
	}
	else
	{
		pc = gserver.m_playerList.Find(index);

		if( !pc ) 
		{
			GAMELOG << init("CASH_ITEM_USE_ERROR-notPC")
					<< bguild << delim
					<< index << delim
					<< 842 << end;
			return;
		}
		// 에러코드에 따른 메세지 작성
		
		if(error == MSG_EX_NAMECHANGE_ERROR_SUCCESS)
		{
			// 아이템 삭제/ 서버 변수 변경 // 개명카드 삭제
			// 아이템 검사
			int r, c;
			if (!pc->m_invenNormal.FindItem(&r, &c, 842, 0, 0))
			{
				if (!pc->m_invenNormal.FindItem(&r, &c, 1120, 0, 0))
					return ;
			}

			// 카운트 줄이기
			CItem* item = pc->m_invenNormal.GetItem(r, c);

			if( !item )
			{
				NameChangeRepMsg(rmsg, MSG_EX_NAMECHANGE_ERROR_HELPER, name, bguild);
				SEND_Q(rmsg, pc->m_desc);

				//롤백
				HelperNameChangeReqMsg(rmsg, 4, pc->m_index, pc->GetName());
				if(IS_RUNNING_HELPER)
				{
					SEND_Q(msg, gserver.m_helper);
				}

				GAMELOG << init("CASH_ITEM_USE_ERROR-notItem")
						<< bguild << delim
						<< index << delim
						<< 842 << end;
				return;
			}

			if( (item->m_itemProto->m_flag & ITEM_FLAG_CASH) )
			{
				GAMELOG << init("CASH_ITEM_USE", pc)
						<< itemlog(item) << delim
						<< pc->GetName() << delim
						<< name << delim
						<< end;
			}

			// Item 수량 변경
			ItemUseMsg(rmsg, pc, item->tab(), item->row(), item->col(), item->m_itemProto->m_index, 0);
			SEND_Q(rmsg, pc->m_desc);
			DecreaseFromInventory(pc, item, 1);
			if (item->Count() > 0)
			{
				ItemUpdateMsg(rmsg, item, -1);
				SEND_Q(rmsg, pc->m_desc);
			}
			else
			{
				ItemDeleteMsg(rmsg, item);
				SEND_Q(rmsg, pc->m_desc);
				RemoveFromInventory(pc, item, true, true);
			}

			pc->ChangeName(name);

			return;
		}
		NameChangeRepMsg(rmsg, (MSG_EX_NAMECHANGE_ERROR_TYPE) error, name, bguild);
		SEND_Q(rmsg, pc->m_desc);
	}
#endif // FEEITEM
}

void ProcHelperPD4RankViewRep(CNetMsg& msg)
{
	//////////////////////////////////
	// 받은 데이터를 클라이언트에게 보냄 : BW
	//MSG_HELPER_PD4_RANK_VIEW_REQ,		// : charindex(n) charjob(c)
	//MSG_HELPER_PD4_RANK_VIEW_REP : charindex(n) multicount(c), charname(str), bclear(c), deadmon(n), ctime(ll);
	int charindex;
	char multicount;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	char bclear;
	int deadmon;
	LONGLONG ctime;

	
	if( !msg.CanRead() )
	{
		return;
	}


	msg >> charindex
		>> multicount;

	CPC* ch = gserver.m_playerList.Find(charindex);

	if( !ch )
		return;

	CNetMsg rmsg;
	rmsg.Init(MSG_QUEST);
	rmsg << (unsigned char) MSG_QUEST_PD4_RANK_VIEW_REP
		 << multicount;

	if(multicount == 0)
	{
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	for(int i = 0; i < multicount; i++)
	{
		msg >> charname
			>> bclear
			>> deadmon
			>> ctime;

		rmsg << charname
			 << bclear
			 << deadmon
			 << ctime;
	}

	SEND_Q(rmsg, ch->m_desc);
		 
	//MSG_QUEST_PD4_RANK_VIEW_REP,			// 랭크 뷰응답	: multicount(c) charname(str) bclear(c) deadmon(n) time(ll)   	
}
void ProcHelperPD4RewardViewRep(CNetMsg& msg)
{
	//////////////////////////////////
	// 받은 데이터를 클라이언트에게 보냄
	//MSG_HELPER_PD4_REWARD_VIEW_REQ,		// : charindex(n) charjob(c)
	//MSG_HELPER_PD4_REWARD_VIEW_REP : charindex(n) multicount(c), charname(str) breward(c)
	int charindex;
	char multicount;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	int itemindex[5] = { 85, 85, 85, 673, 673 };
	char itemnum[5] = { 3, 2, 1, 3, 1 };
	char breward;

	if( !msg.CanRead() )
	{
		return;
	}

	msg >> charindex
		>> multicount;

	CPC* ch = gserver.m_playerList.Find(charindex);

	if( !ch ) 
		return;

	CNetMsg rmsg;
	if(multicount == 0)
	{
		rmsg.Init(MSG_QUEST);
		rmsg << (unsigned char) MSG_QUEST_PD4_RANK_REWARD_RANK_REP
			 << (char) 0;

		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	rmsg.Init(MSG_QUEST);
	rmsg << (unsigned char) MSG_QUEST_PD4_RANK_REWARD_RANK_REP
		 << multicount;

	for(int i = 0; i < multicount; i++)
	{
		msg >> charname
			>> breward;

		rmsg << charname
			 << itemindex[i]
			 << itemnum[i]
			 << breward;
	}

	SEND_Q(rmsg, ch->m_desc);

	//MSG_QUEST_PD4_RANK_REWARD_RANK_REP,	// 보상 뷰응답	: multicount(c) charname(str) itemindex(n) itemnum(c) breward(c)
}
void ProcHelperPD4RewardRep(CNetMsg& msg)
{
	//////////////////////////////////
	// 받은 데이터를 클라이언트에게 보냄
	//MSG_HELPER_PD4_REWARD_REQ : charindex(n)
	//MSG_HELPER_PD4_REWARD_REP : charindex(n) charrank(c) breward(c)
	int itemindex[5] = { 85, 85, 85, 673, 673 };
	char itemnum[5] = { 3, 2, 1, 3, 1 };

	int charindex;
	char charrank;
	char breward;

	if(!msg.CanRead())
	{
		GAMELOG << init("Quest reward Error");
		return;
	}

	msg >> charindex
		>> charrank
		>> breward;

	CNetMsg rMsg;
	CPC * ch = gserver.m_playerList.Find(charindex);

	if( !ch)
	{
		GAMELOG << init("PD4_REWARD_ERROR")
				<< charindex << delim
				<< charrank << delim
				<< breward << end;
		return;
	}

	if(!charrank)
	{
		//////////////////////////
		// 랭커가 아니다
		QuestPD4ErrorMsg(rMsg, MSG_QUEST_ERR_REWARD_NOT_RANK);
		GAMELOG << init("QuestPD4Error Reward Not Rank", ch)
				<< end;

		SEND_Q(rMsg, ch->m_desc);
		return;
	}

	if(breward)
	{
		////////////////////////////////////////////
		// 이미 보상 받았으
		QuestPD4ErrorMsg(rMsg, MSG_QUEST_ERR_BREWARD);
		GAMELOG << init("QuestPD4Error Reward Not Rank", ch)
				<< end;

		SEND_Q(rMsg, ch->m_desc);
		return;

	}

	CItem* item = NULL;
	item = gserver.m_itemProtoList.CreateItem(itemindex[charrank - 1], -1, 0, 0, itemnum[charrank - 1]);

	if (!item)
	{
		//////////////////////////////
		// item 생성 실패
		return;
	}
				
	if(item->IsAccessary() && item->m_nOption < 1)
	{
		OptionSettingItem(ch, item);
	}

	CNetMsg itemmsg;

	// 들어갈 인벤토리 결정
	CInventory* inven = GET_INVENTORY(ch, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
	if (!inven)
	{
		//////////////////
		// inven 결정 실패
		return;
	}
		
	bool bCountable = false;
	// 인벤에 넣기
	if (AddToInventory(ch, item, true, true))
	{
		// 겹쳐졌는지 검사
		if (item->tab() == -1)
		{
			bCountable = true;
				
			// 수량 변경 알림
			int r, c;
			if (inven->FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
			{
				CItem* p = inven->GetItem(r, c);
				if (p)
					ItemUpdateMsg(itemmsg, p, item->Count());
			}
		}
		else
		{
			ItemAddMsg(itemmsg, item);
			// 돈 검사
			if (item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && ch->m_moneyItem == NULL)
				ch->m_moneyItem = item;
		}
	}
	else
	{
		// 인젠토리 꽉차서 못 받을 때
		// Quest Error Log
		GAMELOG << init("QUEST ERROR", ch)
				<< 105 << delim
				<< (int)MSG_QUEST_ERR_PRIZE_FULL
				<< end;

		// 인벤토리 가득참
		item = ch->m_pArea->DropItem(item, ch);
		if (!item)
		{
			/////////////////
			// item drop 실패
			return;
		}
		
			
		CNetMsg dropMsg;
		item->m_preferenceIndex = ch->m_index;
		ItemDropMsg(dropMsg, ch, item);
		ch->m_pArea->SendToCell(dropMsg, ch, true);
	
		QuestErrorMsg(rMsg, MSG_QUEST_ERR_PRIZE_FULL);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
		
	// 메시지 보내고
	SEND_Q(itemmsg, ch->m_desc);

	rMsg.Init(MSG_QUEST);
	rMsg << (unsigned char) MSG_QUEST_PD4_REWARD;
	SEND_Q(rMsg, ch->m_desc);
		
	// Item LOG
	GAMELOG << init("ITEM_PICK_REWARD_PD4", ch)
			<< itemlog(item)
			<< end;

	// Quest Prize Log
	GAMELOG << init("QUEST PRIZE", ch)
			<< 105 << delim
			<< QPRIZE_ITEM << delim
			<< itemindex[charrank-1] << delim
			<< itemnum[charrank-1]
			<< end;

	//MSG_QUEST_PD4_REWARD_REP,					// 보상 요청 : charindex(n) 

#ifdef IRIS_POINT
	if( IS_RUNNING_CONN )
	{
		int nIpoint = 0;
		switch( charrank )	
		{
		case 1 :  nIpoint=200;	break;
		case 2 :  nIpoint=150;	break;
		case 3 :  nIpoint=80;	break;
		case 4 :  nIpoint=50;	break;
		case 5 :  nIpoint=20;	break;
		}

		CNetMsg ipoint_msg;
		ConnIPointAddMsg( ipoint_msg, ch->m_desc->m_index, ch->m_index, MSG_IPOINT_TYPE_PD4 ,nIpoint );
		SEND_Q( ipoint_msg, gserver.m_connector );
	}
#endif //IRIS_POINT
}

void ProcHelperWhisperReq(CNetMsg& msg, int seq, int server, int subno, int zone)
{
	int sidx;
	CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString chat(1000);
	msg >> sidx >> sname
		>> rname
		>> chat;

	CPC* receiver = gserver.m_playerList.Find(rname, true );
	if (receiver == NULL)
	{
		// 수신자가 없을 때
		CNetMsg mmsg;
		HelperRepWhisperNotFoundMsg(mmsg, seq, server, subno, zone, sidx, sname);
		if (IS_RUNNING_HELPER) SEND_Q(mmsg, gserver.m_helper);
	}
	else
	{
		// 수신자가 있을 때
		CNetMsg cmsg;

		ChatWhisperMsg(cmsg, sidx, sname, receiver->GetName(), chat);
		SEND_Q(cmsg, receiver->m_desc);

		// 메신저에 응답 보내기
		CNetMsg mmsg;
		HelperRepWhisperRepMsg(mmsg, seq, server, subno, zone, sidx, sname, receiver->GetName() , chat);
		if (IS_RUNNING_HELPER) SEND_Q(mmsg, gserver.m_helper);
	}
}

void ProcHelperWhisperRep(CNetMsg& msg, int seq, int server, int subno, int zone)
{
	int sidx, ridx;
	CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString chat(1000);
	msg >> sidx >> sname
		>> ridx >> rname
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

void ProcHelperWhisperNotfound(CNetMsg& msg, int seq, int server, int subno, int zone)
{
	int sidx;
	CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
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

void ProcHelperGuildCreateRep(CNetMsg& msg)
{
	int charindex;
	unsigned char errcode;
	msg >> charindex >> errcode;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (!pc)
	{
		return ;
	}

	CNetMsg rmsg;

	if (errcode == MSG_GUILD_ERROR_OK)
	{
		if (pc->m_moneyItem && pc->m_moneyItem->Count() > GUILD_LEVEL1_NEED_MONEY)
		{
			pc->AddMoney(-GUILD_LEVEL1_NEED_MONEY);
			ItemUpdateMsg(rmsg, pc->m_moneyItem, -GUILD_LEVEL1_NEED_MONEY);
			SEND_Q(rmsg, pc->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, pc->m_moneyItem);
			SEND_Q(rmsg, pc->m_desc);
			pc->AddMoney(-GUILD_LEVEL1_NEED_MONEY);
		}
		pc->m_skillPoint -= GUILD_LEVEL1_NEED_SP * 10000;
		
		pc->CalcStatus(true);
		pc->SaveNow(false);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CREATE_OK);
	}
	else
	{
		GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);
	}
	SEND_Q(rmsg, pc->m_desc);
	pc->m_desc->WaitHelperReply(false);
}

void ProcHelperGuildCreateNotify(CNetMsg& msg)
{
	int guildindex;
	int guildlevel;
	CLCString guildname(MAX_GUILD_NAME_LENGTH + 1);
	int bossindex;
	CLCString bossname(MAX_CHAR_NAME_LENGTH + 1);

	msg >> guildindex
		>> guildlevel
		>> guildname
		>> bossindex
		>> bossname;

	CGuild* guild = gserver.m_guildlist.create(guildindex, guildname, bossindex, bossname);
	if (!guild)
	{
		return ;
	}

	guild->level(guildlevel);
#ifdef NEW_GUILD
	guild->setMaxmember( GUILD_LEVEL1_MAX_MEMBER );
	guild->incline( 0 );
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
	guild->guildPointRanking( 0 );
#endif NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD
	gserver.m_guildlist.add(guild);

	// 보스가 있으면 알리기
	CPC* pc = gserver.m_playerList.Find(bossindex);
	if (pc)
	{
		guild->findmember(bossindex)->online(1);
		guild->findmember(bossindex)->SetPC(pc);
		CNetMsg rmsg;
		GuildInfoMsg(rmsg, pc);
		SEND_Q(rmsg, pc->m_desc);
		GuildListMsg(rmsg, pc);
		SEND_Q(rmsg, pc->m_desc);

		GuildInfoChangeMsg(rmsg, pc->m_index, guildindex, guildname, MSG_GUILD_POSITION_BOSS);
		pc->m_pArea->SendToCell(rmsg, pc, false);
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
		GuildPointRankingMsg( rmsg, pc->m_index, guildindex, guild->GetGuildPointRanking() );
		SEND_Q(rmsg, pc->m_desc);
		pc->m_pArea->SendToCell( rmsg,pc, false );
#endif //NEW_GUILD_POINT_RANKING_NOTICE
	}

	GAMELOG << init("GUILD CREATE")
			<< "INDEX" << delim
			<< guild->index() << delim
			<< "NAME" << delim
			<< guild->name() << delim
			<< "BOSS" << delim
			<< guild->boss()->charindex() << delim
			<< guild->boss()->GetName()
			<< end;
}

void ProcHelperGuildOnlineNotify(CNetMsg& msg)
{
	int guildindex;
	int charindex;
	char online;
	msg >> guildindex
		>> charindex
		>> online;

#ifdef NEW_GUILD
	int zoneindex;
	msg >> zoneindex;
#endif // NEW_GUILD

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
		return ;
	CGuildMember* member = guild->findmember(charindex);
	if (!member)
		return ;
	member->online(online);

#ifdef NEW_GUILD
//	GAMELOG << "ProcHelperGuildOnlineNotify()" << delim
//		<< "ZoneIndex :" << zoneindex
//		<< end;
	member->zoneindex( zoneindex );
#endif // NEW_GUILD

	member->SetPC(NULL);
	if (online)
	{
		CPC* pc = gserver.m_playerList.Find(charindex);
		if (pc)
		{
			member->SetPC(pc);
#ifdef NEW_GUILD			
			CNetMsg guildNoticeMsg;										
			HelperGuildLoadReq( guildNoticeMsg, charindex );
			if (IS_RUNNING_HELPER)
			{
				SEND_Q(guildNoticeMsg, gserver.m_helper);
//				pc->m_desc->WaitHelperReply(true);
			}
#endif //NEW_GUILD
		}
	}

	// 길드원에게 알려주기
	// TODO : GUILD : 트리 구조에 전달하게 될 경우 변경
	CNetMsg rmsg;
	GuildOnlineMsg(rmsg, member);
	guild->SendToAll(rmsg);
}

void ProcHelperGuildMemberList(CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int count;
	int charindex;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	int pos;
	char online;

	msg >> guildindex
		>> count;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	int i;
	for (i = 0; i < count; i++)
	{
		msg >> charindex
			>> charname
			>> pos
			>> online;

		int listindex = guild->addmember(charindex, charname);
		if (listindex == -1)
			continue ;

		switch (pos)
		{
		case MSG_GUILD_POSITION_BOSS:
			guild->changeboss(listindex);
			break;
		case MSG_GUILD_POSITION_OFFICER:
			guild->appoint(listindex);
			break;
		}
		CGuildMember* member = guild->member(listindex);
		if (!member)
			continue ;
		member->online(online);

		CPC* pc = gserver.m_playerList.Find(member->charindex());
		if (pc)
		{
			member->online(1);
			member->SetPC(pc);
			HelperGuildOnline(rmsg, member);
#ifdef NEW_GUILD		
			rmsg << member->GetZoneNum();
#endif // NEW_GUILD
			if (IS_RUNNING_HELPER) SEND_Q(rmsg, gserver.m_helper);
		}
		else
			member->SetPC(NULL);
	}
}

void ProcHelperGuildLoadRep(CNetMsg& msg)
{
	CLCString idname(MAX_ID_NAME_LENGTH + 1);
	int charindex;
	unsigned char errcode;
	msg >> idname >> charindex >> errcode;

	CDescriptor* d = gserver.FindConnectIdName(idname, NULL);
	if (d && d->m_pChar && d->m_pChar->m_index == charindex)
	{
		d->WaitHelperReply(false);
	}
}

void ProcHelperGuildLevelUpRep(CNetMsg& msg)
{
	int guildindex;
	int charindex;
	unsigned char errcode;

	msg >> guildindex >> charindex >> errcode;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	CPC* pc = gserver.m_playerList.Find(charindex);
	if (!pc || !guild)
	{
		return ;
	}

	CNetMsg rmsg;


	if (errcode == MSG_GUILD_ERROR_OK)
	{
		int needlevel = 0x7fffffff;
		LONGLONG needmoney = 0x7fffffff;
		int needsp = 0x7fffffff;
#ifdef NEW_GUILD		
		int needgp = 0x7fffffff;
		if( guild->level() < GUILD_EXTEND_BASE_LEVEL )
		{
#endif // NEW_GUILD
			switch (guild->level())
			{
			case 2:
				needlevel = GUILD_LEVEL2_NEED_LEVEL;
				needmoney = GUILD_LEVEL2_NEED_MONEY;
				needsp = GUILD_LEVEL2_NEED_SP;
				break;

			case 3:
				needlevel = GUILD_LEVEL3_NEED_LEVEL;
				needmoney = GUILD_LEVEL3_NEED_MONEY;
				needsp = GUILD_LEVEL3_NEED_SP;
				break;

			case 4:
				needlevel = GUILD_LEVEL4_NEED_LEVEL;
				needmoney = GUILD_LEVEL4_NEED_MONEY;
				needsp = GUILD_LEVEL4_NEED_SP;
				break;

			case 5:
				needlevel = GUILD_LEVEL5_NEED_LEVEL;
				needmoney = GUILD_LEVEL5_NEED_MONEY;
				needsp = GUILD_LEVEL5_NEED_SP;
				break;
			
#ifdef NEW_GUILD
			case 6:
				needlevel = GUILD_LEVEL6_NEED_LEVEL;
				needmoney = GUILD_LEVEL6_NEED_MONEY;
				needsp = GUILD_LEVEL6_NEED_SP;
				break;
#endif // NEW_GUILD
			}
#ifdef NEW_GUILD
		}
		else if( guild->level() >= GUILD_EXTEND_BASE_LEVEL )
		{
			if( guild->level() > 6 && guild->level() < 11 )
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
			
			int	gap = guild->level() - GUILD_EXTEND_BASE_LEVEL;  // 이미렙업한 상태
			if( gap >= 0 )
			{
				needmoney	= (LONGLONG)( GUILD_LEVEL7_NEED_MONEY * pow(1.09, gap) );
				needsp		= (int)( GUILD_LEVEL7_NEED_SP * pow(1.09, gap) );
				needgp		= (int)( GUILD_LEVEL7_NEED_GP * pow(1.09, gap) );
			}
			else
			{
				needmoney	= GUILD_LEVEL7_NEED_MONEY;
				needsp		= GUILD_LEVEL7_NEED_SP;
				needgp		= GUILD_LEVEL7_NEED_GP;
			}
		}

				

		int guildmaxmember = 0;
		switch( guild->level())
			{
			case 2: guildmaxmember = GUILD_LEVEL2_MAX_MEMBER; break;
			case 3: guildmaxmember = GUILD_LEVEL3_MAX_MEMBER; break;
			case 4: guildmaxmember = GUILD_LEVEL4_MAX_MEMBER; break;
			case 5: guildmaxmember = GUILD_LEVEL5_MAX_MEMBER; break;
			case 6: guildmaxmember = GUILD_LEVEL6_MAX_MEMBER; break;
			default :
				{
					int gap = guild->level() - GUILD_EXTEND_BASE_LEVEL;
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
						{
							guildmaxmember = GUILD_LEVEL6_MAX_MEMBER;
						} 
					}
				}
				break;

			}
		
		
		guild->setMaxmember( guildmaxmember );


		if( guild->GetGuildPoint() > needgp )
		{
			guild->AddGuildPoint( -needgp );			
		}
#endif //NEW_GUILD


		if (pc->m_moneyItem && pc->m_moneyItem->Count() > needmoney)
		{
			pc->AddMoney(-needmoney);
			ItemUpdateMsg(rmsg, pc->m_moneyItem, -needmoney);
			SEND_Q(rmsg, pc->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, pc->m_moneyItem);
			SEND_Q(rmsg, pc->m_desc);
			pc->AddMoney(-needmoney);
		}
		pc->m_skillPoint -= needsp * 10000;
		
		pc->CalcStatus(true);
		pc->SaveNow(false);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_LEVELUP_OK);

		GAMELOG << init("GUILD LEVEL UP ", pc )
				<< "Guild Index" << delim << guild->index() << delim
				<< "NAS" << delim << needmoney  << delim
				<< "SP" << delim << pc->m_skillPoint << delim
				<< "Need SP" << delim << needsp * 10000 << delim
#ifdef NEW_GUILD				
				<< "Guild Point" << delim << guild->GetGuildPoint() << delim
				<< "Need GP" << delim << needgp << delim
#endif // NEW_GUILD
				<< end;
	}
	else
		GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);

	SEND_Q(rmsg, pc->m_desc);
	pc->m_desc->WaitHelperReply(false);
}

void ProcHelperGuildLevelUpNotify(CNetMsg& msg)
{
	int guildindex;
	int guildlevel;

	msg >> guildindex >> guildlevel;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (guild)
	{
		guild->level(guildlevel);

		// TODO : GUILD : 트리 구성시 전체에 알리기
		CNetMsg rmsg;
		GuildLevelInfoMsg(rmsg, guildindex, guildlevel);
		guild->SendToAll(rmsg);

		GAMELOG << init("GUILD LEVELUP")
				<< "INDEX" << delim
				<< guild->index() << delim
				<< "NAME" << delim
				<< guild->name() << delim
				<< "LEVEL" << delim
				<< guild->level()
				<< end;
	}
}

void ProcHelperGuildBreakUpRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int charindex;
	unsigned char errcode;
	msg >> charindex >> errcode;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		if (errcode == MSG_GUILD_ERROR_OK)
		{
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_BREAKUP_OK);
		}
		else
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);

		SEND_Q(rmsg, pc->m_desc);
		pc->m_desc->WaitHelperReply(false);
	}
}

void ProcHelperGuildBreakUpNotify(CNetMsg& msg)
{
	int guildindex;
	msg >> guildindex;

#ifdef ENABLE_WAR_CASTLE
	// 길드 삭제시 소유 성 정보 초기화 및 용병신청 초기화
	CWarCastle::DeleteGuild(guildindex);
#endif // #ifdef ENABLE_WAR_CASTLE

	// TODO : GUILD : 트리 안에 모두 알리기

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
		return ;
#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
	guild->guildPointRanking( 0 );
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif //NEW_GUILD

	GAMELOG << init("GUILD BREAKUP")
			<< "INDEX" << delim
			<< guild->index() << delim
			<< "NAME" << delim
			<< guild->name()
			<< end;

	CNetMsg rmsg;

	GuildBreakUpNotifyMsg(rmsg, guild->index(), guild->name());
	gserver.m_playerList.SendToAll(rmsg);


#ifdef EXTREME_CUBE
	if(guild->m_cubeUniqueIdx != -1)
	{
		CCubeMemList* memlist;
		memlist = gserver.m_extremeCube.FindMemList(guild);
		if(memlist)
		{
			((CGuildCubeMemList*)memlist)->SetGuild(NULL);
		}
	}
#endif // EXTREME_CUBE

	gserver.m_guildlist.Remove(guild);
}

void ProcHelperGuildMemberAddNotify(CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	int pos;

	msg >> guildindex
		>> charindex
		>> charname
		>> pos;

#ifdef NEW_GUILD
	int zoneindex;
	msg >> zoneindex;
#endif NEW_GUILD

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if(!guild)
	{
		return;
	}
	
	int listindex = guild->addmember(charindex, charname);
	if(listindex == -1)
	{
		return;
	}

	CGuildMember * member = guild->member(listindex);
	if(!member)
	{
		return;
	}
	member->online(0);

#ifdef NEW_GUILD
	member->zoneindex(zoneindex);
#endif // NEW_GUILD

	// 길드원 전부에게 추가 메세지   // 071210 kjban edit
	GuildMemberAddMsg(rmsg, guildindex, charindex, charname);
	guild->SendToAll(rmsg);

	CPC * pc = gserver.m_playerList.Find(charindex);
	if(!pc)
	{	// 혀해당 게임 채널에 캐릭이 없는 경우 아래 과정 생략
		return;
	}

	member->online(1);
	member->SetPC(pc);

	pc->m_guildoutdate = 0;

	GuildInfoMsg(rmsg, pc);
	SEND_Q(rmsg, pc->m_desc);

	GuildListMsg(rmsg, pc);
	SEND_Q(rmsg, pc->m_desc);

	HelperGuildOnline(rmsg, member);
#ifdef NEW_GUILD
	rmsg << zoneindex;
#endif // NEW_GUILD
	if(IS_RUNNING_HELPER) 
	{
		SEND_Q(rmsg, gserver.m_helper);
	}

	GuildInfoChangeMsg(rmsg, charindex, guildindex, guild->name(), member->pos());
#ifdef EVENT_2007_PARENTSDAY_RESULT
	rmsg << pc->m_guildInfo->Event2007ParentsdayResult();
#endif // EVENT_2007_PARENTSDAY_RESULT
#ifdef GUILD_MARK_TABLE
	rmsg << pc->m_guildInfo->GetGuildMark();
#endif // GUILD_MARK_TABLE
	pc->m_pArea->SendToCell(rmsg, pc, false);

#ifdef GMTOOL
	if(pc->m_bGmMonitor)
	{
		guild->m_bGmtarget = true;
	}
#endif // GMTOOL

	GAMELOG << init("GUILD MEMBER ADD")
		<< "GUILD" << delim
		<< guild->index() << delim
		<< guild->name() << delim
		<< "MEMBER" << delim
		<< member->charindex() << delim
		<< member->GetName() << delim
		<< end;
}

void ProcHelperGuildMemberAddRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int bossindex;
	int charindex;
	unsigned char errcode;

	msg >> guildindex
		>> bossindex
		>> charindex
		>> errcode;

	CPC* boss = gserver.m_playerList.Find(bossindex);
	CPC* pc = gserver.m_playerList.Find(charindex);

	if (errcode == MSG_GUILD_ERROR_OK)
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_REGIST_OK);
	else
		GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);
	if (pc)
	{
		SEND_Q(rmsg, pc->m_desc);
		pc->m_regGuild = 0;
		pc->m_desc->WaitHelperReply(false);
		
#ifdef DRATAN_CASTLE
		// 가입한 길드에 공성전 정보가 있을경우 캐릭터에 공성정보 저장
		CDratanCastle * pCastle = CDratanCastle::CreateInstance();
		if( pCastle )
		{
			if( pCastle->IsAttackGuild(guildindex) )
			{
				pc->SetJoinFlag(ZONE_DRATAN, WCJF_ATTACK_GUILD);
			}
			else if( pCastle->IsDefenseGuild(guildindex) )
			{
				pc->SetJoinFlag(ZONE_DRATAN, WCJF_DEFENSE_GUILD);
			}
			else if( pCastle->GetOwnerGuildIndex() == guildindex )
			{
				pc->SetJoinFlag(ZONE_DRATAN, WCJF_OWNER);
			}
		}
#endif // DRATAN_CASTLE
	}
	if (boss)
	{
		boss->m_regGuild = 0;
		boss->m_desc->WaitHelperReply(false);
	}
}

void ProcHelperGuildMemberOutNotify(CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);

	msg >> guildindex
		>> charindex
		>> name;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
		return ;

	GuildMemberOutMsg(rmsg, guildindex, charindex, member->GetName());
	guild->SendToAll(rmsg);

	if (member->GetPC())
	{
		GuildInfoChangeMsg(rmsg, member->charindex(), -1, "", MSG_GUILD_POSITION_UNKNOWN);
		member->GetPC()->m_pArea->SendToCell(rmsg, member->GetPC(), false);
		time_t t;
		member->GetPC()->m_guildoutdate = time(&t) / 60 / 60 / 24;
		member->GetPC()->SaveNow(false);
	}

#ifdef GMTOOL
	if(member->GetPC() && member->GetPC()->m_bGmMonitor)
	{
		guild->m_bGmtarget = false;
	}
#endif // GMTOOL


#ifdef EXTREME_CUBE
	if(member->GetPC() && member->GetPC()->m_pZone != NULL  && member->GetPC()->m_pZone->IsExtremeCube())
	{
		CCubeMemList* memlist;
		memlist = gserver.m_extremeCube.FindMemList(guild);
		if(memlist)
		{
			int zone = ZONE_MERAC;
			int extra = 0;
			
			int i;
			CZone* pZone;
			
			i = gserver.FindZone(zone);
			pZone = gserver.m_zones + i;
			
			if(member->GetPC())
			{
				CPC* pc = member->GetPC();
				GoZone(pc, pZone->m_index,
					pZone->m_zonePos[extra][0],														// ylayer
					GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
					GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
			}
		}
	}
#endif // EXTREME_CUBE

	GAMELOG << init("GUILD MEMBER OUT")
			<< "GUILD" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< "MEMBER" << delim
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

	guild->removemember(member);
}

void ProcHelperGuildMemberOutRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;
	unsigned char errcode;

	msg >> guildindex
		>> charindex
		>> errcode;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		if (errcode == MSG_GUILD_ERROR_OK)
		{
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_OUT_OK);
		}
		else
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);
		SEND_Q(rmsg, pc->m_desc);
		pc->m_desc->WaitHelperReply(false);

#ifdef DRATAN_CASTLE
		// 캐릭터에 공성정보 삭제
		pc->SetJoinFlag(ZONE_DRATAN, WCJF_NONE);
		///=== 메라크?
#endif // DRATAN_CASTLE
	}
}

void ProcHelperGuildMemberKickRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int bossindex;
	int charindex;
	unsigned char errcode;

	msg >> guildindex
		>> bossindex
		>> charindex
		>> errcode;

	CPC* pc = gserver.m_playerList.Find(bossindex);
	if (pc)
	{
		if (errcode == MSG_GUILD_ERROR_OK)
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_KICK_OK);
		else
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);

		SEND_Q(rmsg, pc->m_desc);
		pc->m_desc->WaitHelperReply(false);
	}
}

void ProcHelperGuildMemberKickNotify(CNetMsg& msg)
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
		return ;

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
		return ;

	GuildMemberKickMsg(rmsg, guildindex, bossindex, charindex, member->GetName());
	guild->SendToAll(rmsg);

	GAMELOG << init("GUILD MEMBER KICK")
			<< "GUILD" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< "MEMBER" << delim
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		GuildInfoChangeMsg(rmsg, charindex, -1, "", MSG_GUILD_POSITION_UNKNOWN);
		pc->m_pArea->SendToCell(rmsg, pc, false);

#if defined (LC_GER)
		time_t t;
		pc->m_guildoutdate = time(&t) / 60 / 60 / 24;
		pc->SaveNow(false);
#endif

#ifdef GMTOOL
		if(pc->m_bGmMonitor)
		{
			guild->m_bGmtarget = false;
		}
#endif // GMTOOL

#ifdef NEW_GUILD	
		if( guild->level() > MANAGE_NEED_LEVEL )
		{
			if( IS_RUNNING_HELPER )
			{
				HelperNewGuildManage( rmsg, pc );
				SEND_Q( rmsg, gserver.m_helper );
//				pc->m_desc->WaitHelperReply( true );
			}
			else
			{
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
				SEND_Q(rmsg, pc->m_desc);
			}
		}
#endif // NEW_GUILD

#ifdef DRATAN_CASTLE
		// 캐릭터에 공성정보 삭제
		pc->SetJoinFlag(ZONE_DRATAN, WCJF_NONE);
		///=== 메라크?
#endif // DRATAN_CASTLE

#ifdef EXTREME_CUBE
		if(pc->m_pZone != NULL && pc->m_pZone->IsExtremeCube())
		{
			CCubeMemList* memlist;
			memlist = gserver.m_extremeCube.FindMemList(guild);
			if(memlist)
			{
				int zone = ZONE_MERAC;
				int extra = 0;
				
				int i;
				CZone* pZone;
				
				i = gserver.FindZone(zone);
				pZone = gserver.m_zones + i;
				
				GoZone(pc, pZone->m_index,
					pZone->m_zonePos[extra][0],														// ylayer
					GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
					GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
				
			}
		}
#endif // EXTREME_CUBE
	}

	guild->removemember(member);
}

void ProcHelperGuildChangeBossNotify(CNetMsg& msg)
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
		return ;

	CGuildMember* member = guild->findmember(change);
	if (!member)
		return ;
// << kjban
// 	GAMELOG << init("GUILD CHANGE BOSS")
// 			<< "GUILD" << delim
// 			<< guild->index() << delim
// 			<< guild->name() << delim
// 			<< "PRE BOSS" << delim;
// 	if( guild->boss() )
// 	{
// 			GAMELOG << guild->boss()->charindex() << delim
// 					<< guild->boss()->GetName();
// 	}
// 	GAMELOG	<< "NEW BOSS" << delim
// 			<< member->charindex() << delim
// 			<< member->GetName()
// 			<< end;
	if(guild->boss() == NULL)
	{
		GAMELOG << init("GUILD CHANGE BOSS")
			<< "GUILD" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< "NEW BOSS" << delim
			<< member->charindex() << delim
			<< member->GetName() << end;
	}
	else
	{		
		GAMELOG << init("GUILD CHANGE BOSS")
			<< "GUILD" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< "PRE BOSS" << delim
			<< guild->boss()->charindex() << delim
			<< guild->boss()->GetName() << delim
			<< "NEW BOSS" << delim
			<< member->charindex() << delim
			<< member->GetName() << end;
	}
// >>

	guild->changeboss(member->listindex());

	GuildChangeBossMsg(rmsg, guildindex, current, change);
	guild->SendToAll(rmsg);


	if (member->GetPC())
	{
		GuildInfoChangeMsg(rmsg, change, guildindex, guild->name(), MSG_GUILD_POSITION_BOSS);
		member->GetPC()->m_pArea->SendToCell(rmsg, member->GetPC(), false);
	}
	
#ifdef ENABLE_WAR_CASTLE
	// 성주 길드라면 성주변경
	CWarCastle* cast = CWarCastle::GetCastleObject( CWarCastle::GetCurSubServerCastleZoneIndex() );
	if( cast && cast->GetOwnerGuildIndex() == guild->index() )
	{
		// 이전 보스에 무기 뺏고,
		CPC* preboss = gserver.m_playerList.Find(current);
		if( preboss )
		{
			int i;
			for (i = 0; i < gserver.m_itemProtoList.m_nCount; i++)
			{
				if (gserver.m_itemProtoList.m_protoItems[i].m_flag & ITEM_FLAG_LORD)
				{
					int r, c;
					while (preboss->m_invenNormal.FindItem(&r, &c, gserver.m_itemProtoList.m_protoItems[i].m_index, 0, 0))
					{
						CItem* item = preboss->m_invenNormal.GetItem(r, c);
						if (item->m_wearPos != WEARING_NONE)
						{
							ItemWearMsg(msg, item->m_wearPos, NULL, item);
							SEND_Q(msg, preboss->m_desc);
							if (item->m_wearPos >= WEARING_SHOW_START && item->m_wearPos <= WEARING_SHOW_END && preboss->m_pArea)
							{
								WearingMsg(msg, preboss, item->m_wearPos, -1, 0);
								preboss->m_pArea->SendToCell(msg, preboss, true);
							}
							preboss->m_wearing[(int)item->m_wearPos] = NULL;
							item->m_wearPos = WEARING_NONE;
						}

						ItemDeleteMsg(msg, item);
						SEND_Q(msg, preboss->m_desc);
						RemoveFromInventory(preboss, item, true, true);
					}
				}
			}
		}

		cast->SetOwner(guild);
		CDBCmd cmd;
		cmd.Init(&gserver.m_dbcastle);

		sprintf(g_buf, "update t_castle set a_owner_charindex = %d, a_owner_char_name = '%s' where a_zone_index = %d ", member->charindex(), member->GetName(), cast->GetZoneIndex() );
		cmd.SetQuery(g_buf);
		cmd.Update();
		GAMELOG << init("WARCASTLE BOSS CHANGE")
				<< member->charindex() << delim
				<< member->GetName() << end;

		// 현재 보스에 무기 지급
		CPC* boss = gserver.m_playerList.Find(change);
		if (boss && boss->m_guildInfo && boss->m_guildInfo->guild())
		{
			int ownZoneCount;
			int* ownZoneIndex = CWarCastle::GetOwnCastle(boss->m_guildInfo->guild()->index(), &ownZoneCount);
			if (ownZoneIndex)
			{
				time_t curtime;
				time(&curtime);

				int i;
				for (i = 0; i < ownZoneCount; i++)
				{
					CWarCastle* castle = CWarCastle::GetCastleObject(ownZoneIndex[i]);
					if (castle)
					{
						// 060116 : BS : BEGIN : 공성 시작해도 칼 회수 안하게
	//					int nexttime = castle->GetNextWarTime();

						if (castle->GetOwnerCharIndex() == boss->m_index)
						{
							// 060116 : BS : BEGIN : 공성 시작해도 칼 회수 안하게
	//						// 공성 시작 5분전에서 공성 진행중이면 회수
	//						if (castle->GetState() != WCSF_NORMAL || curtime + 5 * 60 >= nexttime)
	//						{
	//							castle->TakeLordItem(d->m_pChar);
	//						}
	//
	//						// 일반 상태에서 없으면 지급
	//						else
	//						{
								castle->GiveLordItem(boss);
	//						}
							// 060116 : BS : END : 공성 시작해도 칼 회수 안하게
						}
					}
				}
				delete [] ownZoneIndex;
			}
		}
	}
#endif // ENABLE_WAR_CASTLE

	CGuildMember* boss = guild->findmember(current);
	if (!boss)
		return ;

	if (boss->GetPC())
	{
		GuildInfoChangeMsg(rmsg, current, guildindex, guild->name(), MSG_GUILD_POSITION_MEMBER);
		boss->GetPC()->m_pArea->SendToCell(rmsg, boss->GetPC(), false);
	}

#ifdef NEW_GUILD
	if( guild->level() > MANAGE_NEED_LEVEL )
	{
		CPC* ch = gserver.m_playerList.Find(current);
		CPC* ch1 = gserver.m_playerList.Find(change);
		if( ch && ch1 )
		{
			if( IS_RUNNING_HELPER )
			{
				HelperNewGuildManage( rmsg, ch );
				SEND_Q( rmsg, gserver.m_helper );
//				ch->m_desc->WaitHelperReply( true );

				HelperNewGuildManage( rmsg, ch1 );
				SEND_Q( rmsg, gserver.m_helper );
//				ch1->m_desc->WaitHelperReply( true );
			}
			else
			{
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
				SEND_Q(rmsg, ch->m_desc);
				SEND_Q(rmsg, ch1->m_desc);
			}
		}
	}
#endif // NEW_GUILD


}

void ProcHelperGuildChangeBossRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int current;
	int change;
	unsigned char errcode;

	msg >> guildindex
		>> current
		>> change
		>> errcode;

	CPC* pc = gserver.m_playerList.Find(current);
	if (pc)
	{
		if (errcode == MSG_GUILD_ERROR_OK)
		{
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CHANGE_BOSS_OK);
		}
		else
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);

		SEND_Q(rmsg, pc->m_desc);
		pc->m_desc->WaitHelperReply(false);
	}
}

void ProcHelperGuildAppointOfficerRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int bossindex;
	int charindex;
	unsigned char errcode;

	msg >> guildindex
		>> bossindex
		>> charindex
		>> errcode;

	CPC* pc = gserver.m_playerList.Find(bossindex);
	if (pc)
	{
		if (errcode == MSG_GUILD_ERROR_OK)
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_APPOINT_OFFICER_OK);
		else
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);

		SEND_Q(rmsg, pc->m_desc);
		pc->m_desc->WaitHelperReply(false);
	}
}

void ProcHelperGuildAppointOfficerNotify(CNetMsg& msg)
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
		return ;

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
		return ;

	guild->appoint(member->listindex());

	GuildAppointOfficerMsg(rmsg, guildindex, charindex);
	guild->SendToAll(rmsg);

	if (member->GetPC())
	{
		GuildInfoChangeMsg(rmsg, charindex, guildindex, guild->name(), MSG_GUILD_POSITION_OFFICER);
		member->GetPC()->m_pArea->SendToCell(rmsg, member->GetPC(), false);
	}

	GAMELOG << init("GUILD OFFICER APPOINT")
			<< "GUILD" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< "NEW OFFICER" << delim
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

#ifdef NEW_GUILD
	if( guild->level() > MANAGE_NEED_LEVEL )
	{
		CPC* ch = gserver.m_playerList.Find(bossindex);
		if( ch )
		{
			if( IS_RUNNING_HELPER )
			{
				HelperNewGuildManage( rmsg, ch );
				SEND_Q( rmsg, gserver.m_helper );
//				ch->m_desc->WaitHelperReply( true );
			}
			else
			{
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}
#endif // NEW_GUILD
}

void ProcHelperGuildChat(CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int charindex;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString chat(1000);

	msg >> guildindex
		>> charindex
		>> charname
		>> chat;

	SayMsg(rmsg, MSG_CHAT_GUILD, charindex, charname, "", chat);

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (guild)
	{
		guild->SendToAll(rmsg);
#ifdef GMTOOL
		if(guild->m_bGmtarget)
		{
			if( IS_RUNNING_MSGR )
			{
				CNetMsg rmsg;
				int i;
				bool bSend = false;
				for(i = 0; i < GUILD_MAX_MEMBER; ++i)
				{
					CGuildMember* member = guild->member(i);
					if(member && member->GetPC() && member->GetPC()->m_bGmMonitor)
					{
						MsgrNoticeGmChatMonitorGuildMsg(rmsg, -1, 1, 1, 0, gserver.m_serverno, gserver.m_subno, -1, chat, guildindex, charname, member->GetPC()->m_index, member->GetPC()->m_name);
						SEND_Q(rmsg, gserver.m_messenger);
						
						bSend = true;
					}
				}

				if(!bSend)
				{
					guild->m_bGmtarget = false;
				}
			}
		}
#endif // GMTOOL
	}
}

void ProcHelperGuildFireOfficerRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int guildindex;
	int bossindex;
	int charindex;
	unsigned char errcode;

	msg >> guildindex
		>> bossindex
		>> charindex
		>> errcode;

	CPC* pc = gserver.m_playerList.Find(bossindex);
	if (pc)
	{
		if (errcode == MSG_GUILD_ERROR_OK)
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_FIRE_OFFICER_OK);
		else
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);

		SEND_Q(rmsg, pc->m_desc);
		pc->m_desc->WaitHelperReply(false);
	}
}

void ProcHelperGuildFireOfficerNotify(CNetMsg& msg)
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
		return ;

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
		return ;

	guild->fire(member->listindex());

	GuildFireOfficerMsg(rmsg, guildindex, charindex);
	guild->SendToAll(rmsg);

	if (member->GetPC())
	{
		GuildInfoChangeMsg(rmsg, charindex, guildindex, guild->name(), MSG_GUILD_POSITION_MEMBER);
		member->GetPC()->m_pArea->SendToCell(rmsg, member->GetPC(), false);
	}

	GAMELOG << init("GUILD OFFICER FIRE")
			<< "GUILD" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< "OFFICER" << delim
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

#ifdef NEW_GUILD
	if( guild->level() > MANAGE_NEED_LEVEL )
	{
		CPC* ch = gserver.m_playerList.Find(bossindex);
		if( ch )
		{
			if( IS_RUNNING_HELPER )
			{
				HelperNewGuildManage( rmsg, ch );
				SEND_Q( rmsg, gserver.m_helper );
//				ch->m_desc->WaitHelperReply( true );
			}
			else
			{
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}
#endif // NEW_GUILD
}

void ProcHelperGuildLoadNotify(CNetMsg& msg)
{
	int guildindex;
	int guildlevel;
	CLCString guildname(MAX_GUILD_NAME_LENGTH + 1);
	int bossindex;
	CLCString bossname(MAX_CHAR_NAME_LENGTH + 1);
	int battleIndex;

	msg >> guildindex
		>> guildlevel
		>> guildname
		>> bossindex
		>> bossname
		>> battleIndex;



	CGuild* guild = gserver.m_guildlist.findguild(guildindex);

	if (!guild)
	{
		guild = gserver.m_guildlist.create(guildindex, guildname, bossindex, bossname);
		if (!guild)
			return;

		guild->level(guildlevel);

		gserver.m_guildlist.add(guild);
	}
#ifdef NEW_GUILD
	
	int battlePrize;
	int battleTime;
	int battleZone;
	int killCount;
	char battleState;
	int maxmember;

	msg >> battlePrize
		>> battleTime
		>> battleZone
		>> killCount
		>> battleState
		>> maxmember;
	guild->setMaxmember( maxmember );

	if (battleIndex != -1)
	{
		guild->BattleSet(battleIndex, battlePrize, battleZone);
		guild->BattleTime(battleTime);
		guild->KillCount(killCount);
		guild->BattleState(battleState);
	}	
#else

	if (battleIndex != -1)
	{
		int battlePrize;
		int battleTime;
		int battleZone;
		int killCount;
		char battleState;

		msg >> battlePrize
			>> battleTime
			>> battleZone
			>> killCount
			>> battleState;
//#ifdef NEW_GUILD
//		int maxmember;
//		msg >> maxmember;		
//		guild->setMaxmember( maxmember );
//#endif // NEW_GUILD

		guild->BattleSet(battleIndex, battlePrize, battleZone);
		guild->BattleTime(battleTime);
		guild->KillCount(killCount);
		guild->BattleState(battleState);
	}

#endif // NEW_GUILD

	/*
	CGuild* guild = gserver.m_guildlist.create(guildindex, guildname, bossindex, bossname);
	if (!guild)
	{
		return ;
	}

	guild->level(guildlevel);
	gserver.m_guildlist.add(guild);
	*/
}

void ProcHelperGuildBattleRep(CNetMsg& msg)
{
	int guildindex1;
	CLCString guildname1(MAX_GUILD_NAME_LENGTH + 1);
	int guildindex2;
	CLCString guildname2(MAX_GUILD_NAME_LENGTH + 1);
	int prize;
	int zone;
	int time;
	
	msg >> guildindex1
		>> guildname1
		>> guildindex2
		>> guildname2
		>> prize
		>> zone
		>> time;
	
	CGuild* g1 = gserver.m_guildlist.findguild(guildindex1);
	CGuild* g2 = gserver.m_guildlist.findguild(guildindex2);

	CPC* boss1 = NULL;
	CPC* boss2 = NULL;

	if (g1)
		boss1 = g1->boss()->GetPC();

	if (g2)
		boss2 = g2->boss()->GetPC();

	if (!g1 || !g2 || !boss1 || !boss2)
		return ;

	CNetMsg itemmsg;

	if (boss1)
	{
		if (boss1->m_moneyItem)
		{
			if (boss1->m_moneyItem->Count() > (LONGLONG)prize)
			{
				boss1->AddMoney(-prize);
				ItemUpdateMsg(itemmsg, boss1->m_moneyItem, -prize);
			}
			else if (boss1->m_moneyItem->Count() == (LONGLONG)prize)
			{
				ItemDeleteMsg(itemmsg, boss1->m_moneyItem);
				boss1->AddMoney(-prize);
			}

			SEND_Q(itemmsg, boss1->m_desc);
		}
	}

	if (boss2)
	{	
		if (boss2->m_moneyItem)
		{
			if (boss2->m_moneyItem->Count() > (LONGLONG)prize)
			{
				boss2->AddMoney(-prize);
				ItemUpdateMsg(itemmsg, boss2->m_moneyItem, -prize);
			}
			else if (boss2->m_moneyItem->Count() == (LONGLONG)prize)
			{
				ItemDeleteMsg(itemmsg, boss2->m_moneyItem);
				boss2->AddMoney(-prize);
			}

			SEND_Q(itemmsg, boss2->m_desc);
		}
	}
	
	// 세부 셋팅
	if (g1)
	{
		g1->BattleSet(g2->index(), prize * 2 * 95 / 100, g1->battleZone());
		g1->BattleTime(time);
		g1->BattleState(GUILD_BATTLE_STATE_WAIT);
	}
	if (g2)
	{
		g2->BattleSet(g1->index(), prize * 2 * 95 / 100, g1->battleZone());
		g2->BattleTime(time);
		g2->BattleState(GUILD_BATTLE_STATE_WAIT);
	}

	// 길전이 성립되다!
	CNetMsg rmsg;
	GuildBattleReqAccpetMsg(rmsg, guildindex1, guildname1, guildindex2, guildname2, prize * 2 * 95 / 100 / 10000, zone, time);

	if (g1)
		g1->SendToAll(rmsg);
	if (g2)
		g2->SendToAll(rmsg);
}

void ProcHelperGuildBattleStart(CNetMsg& msg)
{
	int guildindex1;
	CLCString guildname1(MAX_GUILD_NAME_LENGTH + 1);
	int guildindex2;
	CLCString guildname2(MAX_GUILD_NAME_LENGTH + 1);
	int prize;
	int zone;
	int time;
	
	msg >> guildindex1
		>> guildname1
		>> guildindex2
		>> guildname2
		>> prize
		>> zone
		>> time;
	
	CGuild* g1 = gserver.m_guildlist.findguild(guildindex1);
	CGuild* g2 = gserver.m_guildlist.findguild(guildindex2);

	if (g1)
	{
		g1->BattleState(GUILD_BATTLE_STATE_ING);
		g1->BattleTime(time);
	}

	if (g2)
	{
		g2->BattleState(GUILD_BATTLE_STATE_ING);
		g2->BattleTime(time);
	}

	CNetMsg rmsg;
	GuildBattleStartMsg(rmsg, guildindex1, guildname1, guildindex2, guildname2, prize / 10000, zone, time);

	if (g1)
		g1->SendToAll(rmsg);

	if (g2)
		g2->SendToAll(rmsg);
}


void ProcHelperGuildBattleStopRep(CNetMsg& msg)
{
	int winner_index;
	int guildindex1;
	CLCString guildname1(MAX_GUILD_NAME_LENGTH + 1);
	int guildindex2;
	CLCString guildname2(MAX_GUILD_NAME_LENGTH + 1);
	int prize;
	int zone;

	msg >> winner_index
		>> guildindex1
		>> guildname1
		>> guildindex2
		>> guildname2
		>> prize
		>> zone;
	
	CGuild* g1 = gserver.m_guildlist.findguild(guildindex1);
	CGuild* g2 = gserver.m_guildlist.findguild(guildindex2);

	CPC* boss = NULL;
	bool bHaveMoney = false;
	CNetMsg itemmsg;
	CNetMsg rmsg;

	// 동점이면 상금이 반
	if (winner_index == -1)
	{
		prize /= 2;

		if (g1)
			g1->BattleSet(guildindex2, prize, zone);
		if (g2)
			g2->BattleSet(guildindex1, prize, zone);
	}

	// 있는 길드 우선 셋팅 해제
	if (g1)
	{
		g1->BattleState(GUILD_BATTLE_STATE_PRIZE);

		// 동점이거나 승리길드일때 길드장 찾기
		if (winner_index == -1 || winner_index == g1->index())
		{
			boss = g1->boss()->GetPC();

			// 길드장이 있으면 상금 주고 길드 상태 해제 요청
			if (boss)
			{
				if (boss->m_moneyItem)
					bHaveMoney = true;
				
				boss->AddMoney(prize);
				
				if (bHaveMoney)
					ItemUpdateMsg(itemmsg, boss->m_moneyItem, prize);
				else
					ItemAddMsg(itemmsg, boss->m_moneyItem);
				
				SEND_Q(itemmsg, boss->m_desc);

				// 길드전 해제 요청
				if (IS_RUNNING_HELPER)
				{
					HelperGuildBattlePeaceReqMsg(rmsg, g1);
					SEND_Q(rmsg, gserver.m_helper);
				}
				else
				{
					return ;
				}

				g1->BattleSet(-1, 0, -1);
				g1->BattleTime(0);
				g1->KillCount(0);
				g1->BattleState(GUILD_BATTLE_STATE_PEACE);
			}
		}
		else
		{
			g1->BattleSet(-1, 0, -1);
			g1->BattleTime(0);
			g1->KillCount(0);
			g1->BattleState(GUILD_BATTLE_STATE_PEACE);
		}
	}

	boss = NULL;
	bHaveMoney = false;

	if (g2)
	{
		g2->BattleState(GUILD_BATTLE_STATE_PRIZE);

		// 동점이거나 승리길드일때 길드장 찾기
		if (winner_index == -1 || winner_index == g2->index())
		{
			boss = g2->boss()->GetPC();

			// 길드장이 있으면 상금 주고 길드 상태 해제 요청
			if (boss)
			{
				if (boss->m_moneyItem)
					bHaveMoney = true;

				boss->AddMoney(prize);
								
				if (bHaveMoney)
					ItemUpdateMsg(itemmsg, boss->m_moneyItem, prize);
				else
					ItemAddMsg(itemmsg, boss->m_moneyItem);
				
				SEND_Q(itemmsg, boss->m_desc);

				// 길드전 해제 요청
				if (IS_RUNNING_HELPER)
				{
					HelperGuildBattlePeaceReqMsg(rmsg, g2);
					SEND_Q(rmsg, gserver.m_helper);
				}
				else
				{
					return ;
				}

				g2->BattleSet(-1, 0, -1);
				g2->BattleTime(0);
				g2->KillCount(0);
				g2->BattleState(GUILD_BATTLE_STATE_PEACE);
			}
		}
		else
		{
			g2->BattleSet(-1, 0, -1);
			g2->BattleTime(0);
			g2->KillCount(0);
			g2->BattleState(GUILD_BATTLE_STATE_PEACE);
		}
	}

	// 길드전 종료 메시지
	GuildBattleEndMsg(rmsg, winner_index, guildindex1, guildname1, guildindex2, guildname2, prize);

	if (g1)
		g1->SendToAll(rmsg);

	if (g2)
		g2->SendToAll(rmsg);
}

void ProcHelperGuildBattleStatus(CNetMsg& msg)
{
	int guildindex1;
	CLCString guildname1(MAX_GUILD_NAME_LENGTH + 1);
	int killCount1;
	int guildindex2;
	CLCString guildname2(MAX_GUILD_NAME_LENGTH + 1);
	int killCount2;
	int battleTime;
	int battleZone;

	msg >> guildindex1
		>> guildname1
		>> killCount1
		>> guildindex2
		>> guildname2
		>> killCount2
		>> battleTime
		>> battleZone;

	CGuild* g1 = gserver.m_guildlist.findguild(guildindex1);
	CGuild* g2 = gserver.m_guildlist.findguild(guildindex2);

	if (g1)
	{
		g1->KillCount(killCount1);
		g1->BattleTime(battleTime);
	}
	if (g2)
	{
		g2->KillCount(killCount2);
		g2->BattleTime(battleTime);
	}

	CNetMsg rmsg;
	GuildBattleStatusMsg(rmsg, guildindex1, guildname1, killCount1, guildindex2, guildname2, killCount2, battleTime, battleZone);

	if (g1)
		g1->SendToAll(rmsg);
	if (g2)
		g2->SendToAll(rmsg);
}

void ProcHelperGuildBattlePeaceRep(CNetMsg& msg)
{
	int guildindex;

	msg >> guildindex;

	CGuild* g = gserver.m_guildlist.findguild(guildindex);

	if (!g)
		return;

	g->BattleSet(-1, 0, -1);
	g->BattleTime(0);
	g->KillCount(0);
	g->BattleState(GUILD_BATTLE_STATE_PEACE);
}

#ifdef GUILD_MARK_TABLE
void ProcHelperGuildMarkTable(CNetMsg & msg)
{
	for(int i=0; i < 3; i++)
	{
		msg >> gserver.m_nGuildMarkTable[i];
	}	
}
#endif // GUILD_MARK_TABLE

void ProcHelperEventMoonStoneLoad(CNetMsg& msg)
{
	int moonstone_nas;

	msg >> moonstone_nas;
			
	if(moonstone_nas >= 0)
	{
		gserver.m_nMoonStoneNas = moonstone_nas;
	}
}

void ProcHelperEventMoonStoneJackpot(CNetMsg& msg)
{
	int moonstone_nas;
	int chaindex;
	CNetMsg itemmsg, rMsg;

	msg >> moonstone_nas
		>> chaindex;

	CPC* ch = gserver.m_playerList.Find(chaindex);

	if(!ch)
		return;

	CItem* gift = NULL;	
	
	gift = gserver.m_itemProtoList.CreateItem(19, -1, 0, 0, moonstone_nas);
	if (!gift)
		return;

	// 들어갈 인벤토리 결정
	CInventory* inven = GET_INVENTORY(ch, GET_TAB(gift->m_itemProto->m_typeIdx, gift->m_itemProto->m_subtypeIdx));
	if (!inven)
		return ;
				
	bool bCountable = false;
	// 인벤에 넣기
	if (AddToInventory(ch, gift, true, true))
	{
		// 겹쳐졌는지 검사
		if (gift->tab() == -1)
		{
			bCountable = true;
			
			// 수량 변경 알림
			int r, c;
			if (inven->FindItem(&r, &c, gift->m_idNum, gift->m_plus, gift->m_flag))
			{
				CItem* p = inven->GetItem(r, c);
				if (p)
					ItemUpdateMsg(itemmsg, p, gift->Count());
			}
		}
		else
		{
			ItemAddMsg(itemmsg, gift);
			// 돈 검사
			if (gift->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index && ch->m_moneyItem == NULL)
				ch->m_moneyItem = gift;
		}
	}
	else
	{				
		// 인벤토리 가득참
		gift = ch->m_pArea->DropItem(gift, ch);
		if (!gift)
			return;
		
		CNetMsg dropMsg;
		gift->m_preferenceIndex = ch->m_index;
		ItemDropMsg(dropMsg, ch, gift);
		ch->m_pArea->SendToCell(dropMsg, ch, true);
		SEND_Q(rMsg, ch->m_desc);
		return;
	}
				
	// 메시지 보내고
	SEND_Q(itemmsg, ch->m_desc);
				
	// Item LOG
	GAMELOG << init("CHANGE_MOONSTONE", ch)
		<< itemlog(gift)
		<< end;
				
	if (bCountable)
		delete gift;

}

#ifdef ENABLE_MESSENGER
void ProcHelper_FriendAddRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	
	int approvalindex;	
	int requestindex;	
	CLCString appname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString reqname(MAX_CHAR_NAME_LENGTH + 1);
	int appjob, reqjob;
	unsigned char errcode;
	
	msg >> approvalindex
		>> appname
		>> appjob
		>> requestindex
		>> reqname
		>> reqjob
		>> errcode;		
	
	
	CPC* appPc = gserver.m_playerList.Find(approvalindex);
	CPC* reqPc = gserver.m_playerList.Find(requestindex);
	
	if (!appPc || !reqPc)
	{
		return;
	}
	
	if((MSG_FRIEND_ERROR_TYPE)errcode == MSG_FRIEND_ERROR_GAMESERVER)
	{
		FriendErrorMsg(rmsg, (MSG_FRIEND_ERROR_TYPE)errcode);//요청자에게 성공여부를 알린다.
		SEND_Q(rmsg, reqPc->m_desc);
		reqPc->m_nRegFriend = 0;
		appPc->m_nRegFriend = 0;
	}
	else
	{
		if (reqPc)//동시에 접속해 있어야 한다.
		{
			FriendErrorMsg(rmsg, (MSG_FRIEND_ERROR_TYPE)errcode);//요청자에게 성공여부를 알린다.
			SEND_Q(rmsg, reqPc->m_desc);
			reqPc->m_nRegFriend = 0;
			
			reqPc->m_Friend->AddFriend(approvalindex, appname, appjob, MSG_FRIEND_CONDITION_NORMAL, 0);
			
			FriendAddNotify(rmsg, appPc); //승낙자에게 멤버가 추가됨을 알림
			SEND_Q(rmsg, reqPc->m_desc);
		}

		if(appPc)
		{
			appPc->m_Friend->AddFriend(requestindex, reqname, reqjob, MSG_FRIEND_CONDITION_NORMAL, 0);
			appPc->m_nRegFriend = 0;
			
			FriendAddNotify(rmsg, reqPc); //요청자에게 멤버가 추가됨을 알림.
			SEND_Q(rmsg, appPc->m_desc);
		}
	}

	if (appPc)
		appPc->m_desc->WaitHelperReply(false);
	if (reqPc)
		reqPc->m_desc->WaitHelperReply(false);
}

void ProcHelper_FriendDelRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int removerindex;
	int deleteindex;
	unsigned char errcode;

	msg >> removerindex
		>> deleteindex
		>> errcode;		


	//삭제 요청자...
	CPC* pPc = gserver.m_playerList.Find(removerindex);

	if(pPc)
	{		
		pPc->m_Friend->RemoveFriend(deleteindex);

		FriendErrorMsg(rmsg, (MSG_FRIEND_ERROR_TYPE)errcode);
		SEND_Q(rmsg, pPc->m_desc);

		FriendDelNotify(rmsg, deleteindex);
		SEND_Q(rmsg, pPc->m_desc);

		pPc->m_desc->WaitHelperReply(false);
	}


	//삭제 당한자...
	CPC* pPc2 = gserver.m_playerList.Find(deleteindex);

	if(pPc2)//접속해있다면...
	{
		pPc2->m_Friend->RemoveFriend(removerindex);

		FriendDelNotify(rmsg, removerindex);
		SEND_Q(rmsg, pPc2->m_desc);
	}		
}

#ifdef MESSENGER_NEW
void ProcHelper_BlockPCRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int reqindex;
	int blockIndex;
	CLCString blockName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> reqindex
		>> blockIndex
		>> blockName;

	//삭제 요청자...
	CPC* pPc = gserver.m_playerList.Find(reqindex);

	if(pPc)
	{

		if( blockIndex == -1 )
		{
			BlockPCRepMsg(rmsg, MSG_EX_MESSENGER_BLOCK_NOTCHAR, blockIndex, blockName);
			SEND_Q(rmsg, pPc->m_desc);
			pPc->m_desc->WaitHelperReply(false);
			return;
		}

		if( pPc->AddBlockPC(blockIndex, blockName) )
		{
			// 블럭 요청이 성공하였다는 메세지 : 할일
			// GS->C			: errcode(c) charIndex(n) name(str)
			BlockPCRepMsg(rmsg, MSG_EX_MESSENGER_SUCCESS, blockIndex, blockName);
		}
		else
			BlockPCRepMsg(rmsg, MSG_EX_MESSENGER_ALREADY_BLOCK, blockIndex, blockName);
		
		SEND_Q(rmsg, pPc->m_desc);
		pPc->m_desc->WaitHelperReply(false);
	}

	CPC* pPc2 = gserver.m_playerList.Find(blockIndex);

	if(pPc2)//접속해있다면...
	{
		CFriendMember* fMember = pPc2->m_Friend->FindFriendMember(reqindex);
		if( fMember )
		{
			fMember->SetCondition(MSG_FRIEND_CONDITION_OFFLINE);
		}

		FriendSetConditionMsg(rmsg, reqindex, MSG_FRIEND_CONDITION_OFFLINE);
		SEND_Q(rmsg, pPc2->m_desc);

		pPc2->m_desc->WaitHelperReply(false);
	}		
}
#endif

#ifdef CASH_ITEM_GIFT
void ProcHelper_GiftRecvChar(CNetMsg& msg)
{
	
	int count = 0;
	int idx[10], ctid[10], i;
	int sendUserIdx, sendCharIdx;
	int recvuserIndex, recvcharIndex;

	CLCString recvcharName( MAX_CHAR_NAME_LENGTH + 1);
	CLCString strMsg( MAX_GIFT_MESSAGE_LENGTH + 1);

	//int sendUserIdx, int sendCharIdx, int recvUserIdx, int recvCharIdx, const char* recvCharName, const char* sendMsg, int count, int idx[], int ctid[] )
	msg >> sendUserIdx
		>> sendCharIdx
		>> recvuserIndex
		>> recvcharIndex
		>> recvcharName
		>> strMsg
		>> count;

	CPC* ch = gserver.m_playerList.Find(sendCharIdx);

	if( !ch )
		return;

	CNetMsg rmsg;

	ch->m_desc->WaitHelperReply(false);

	if( recvcharIndex == -1 )
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;

		CashItemGiftSendRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_GIFT_WRONGCHAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( count < 1 )
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;

		CashItemGiftSendRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_GIFT_NOCTID);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	for( i = 0; i < count; i++ )
	{
		msg >> idx[i]
			>> ctid[i];
	}

	CNetMsg connMsg;
	ConnCashItemGiftReqMsg(connMsg, ch->m_desc->m_index, ch->m_index, ch->GetName(), strMsg, recvuserIndex, recvcharIndex, recvcharName, count, idx, ctid);

	if( IS_RUNNING_CONN )
	{
		SEND_Q(connMsg, gserver.m_connector);

		GAMELOG << init("CASH_ITEM_BRING_GIFT_SEND_REQ", ch)
				<< "RECEIVER" << delim
				<< "USRIDX"  << delim
				<< recvuserIndex << delim
				<< "CHARIDX" << delim
				<< recvcharIndex << delim
				<< "CHARNAME" << delim
				<< recvcharName << delim
				<< count << delim;

		CCatalog* catalog = NULL;
		for(i = 0; i < count; i++ )
		{
			GAMELOG << idx[i] << delim
					<< ctid[i] << delim;
			catalog = gserver.m_catalogList.Find( ctid[i] );

			if( catalog )
				GAMELOG << catalog->GetName() << delim;
		}

		GAMELOG << end;
	}
	else
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;
	}
}
#endif

//상태가 바뀐걸 모든 친구에게 보낸다.
void ProcHelper_FriendSetConditionRep(CNetMsg& msg)
{
	int chaindex;	
	int condition;		
	int reply;

	CFriendMember * pMember = NULL;

	msg >> chaindex
		>> condition
		>> reply;
	if(reply == -1)
	{
		int sum, index;
		msg >> sum;

		for(int i =0; i < sum; i++)
		{
			CNetMsg rmsg;

			msg >> index;
		
			CPC* pPc   = gserver.m_playerList.Find(index);
			if(pPc)
			{
				FriendSetConditionMsg(rmsg, chaindex, condition);
				SEND_Q(rmsg, pPc->m_desc);

				// 친구 리스트 업데이트
				if (pPc->m_Friend)
				{
					pMember = pPc->m_Friend->FindFriendMember( chaindex );
					if( pMember != NULL )
					{
						pMember->SetCondition( condition );
					}

					//접속한 사람에게 내 자신의 상태도 보내줘야지..
					CNetMsg tmsg;
					HelperFriendSetConditionMsg(tmsg, index, pPc->m_nCondition, chaindex, NULL);
					if (IS_RUNNING_HELPER) 
					{
						SEND_Q(tmsg, gserver.m_helper);
					}
				}	
			}
		}
	}
	else
	{
		CNetMsg rmsg;

		CPC* pPc   = gserver.m_playerList.Find(reply);
		if(pPc)
		{
			FriendSetConditionMsg(rmsg, chaindex, condition);
			SEND_Q(rmsg, pPc->m_desc);

			// 친구 리스트 업데이트
			pMember = pPc->m_Friend->FindFriendMember( chaindex );
			if( pMember != NULL )
			{
				pMember->SetCondition( condition );
			}
		}
	}
}
#endif // #ifdef ENABLE_MESSENGER

void ProcHelperWarNoticeTime(CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	int zoneindex;
	char month;
	char day;
	char hour;
	char min;
	msg >> zoneindex
		>> month
		>> day
		>> hour
		>> min;
#if defined (LC_JPN)
	if (gserver.m_subno == WAR_CASTLE_SUBNUMBER_MERAC)
#endif
	{
		CNetMsg rmsg;
		GuildWarNoticeTimeMsg(rmsg, zoneindex, month, day, hour, min);
		gserver.m_playerList.SendToAll(rmsg);
	}
#endif // #ifdef ENABLE_WAR_CASTLE
}

void ProcHelperWarNoticeTimeRemain(CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	int zoneindex;
	char remain;
	msg >> zoneindex
		>> remain;

	CNetMsg rmsg;
	GuildWarNoticeTimeRemainMsg(rmsg, zoneindex, remain);
	gserver.m_playerList.SendToAll(rmsg);

	// 060116 : BS : BEGIN : 공성 시작해도 칼 회수 안하게
//	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
//	if (castle)
//	{
//		// 성주의 칼 회수 : 시작 5분전
//		if (remain <= 5)
//		{
//			CPC* ownerPC = gserver.m_playerList.Find(castle->GetOwnerCharIndex());
//			if (ownerPC)
//			{
//				castle->TakeLordItem(ownerPC);
//			}
//		}
//	}
	// 060116 : BS : END : 공성 시작해도 칼 회수 안하게

#endif // #ifdef ENABLE_WAR_CASTLE
}

void ProcHelperWarNoticeStart(CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	int zoneindex;
	int remainSec;
	msg >> zoneindex
		>> remainSec;

	CNetMsg rmsg;
	GuildWarNoticeStartMsg(rmsg, zoneindex, remainSec);
	gserver.m_playerList.SendToAll(rmsg);

	// 공성 상태 갱신 : 진행중인 서브 서버는 직접 바꾸므로 스킵
	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
	if (castle && !castle->CheckSubServer())
	{
		castle->SetState(WCSF_WAR_FIELD);
	}
#endif // #ifdef ENABLE_WAR_CASTLE
}

void ProcHelperWarJoinAttackGuild(CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	int zoneindex;
	int guildindex;

	msg >> zoneindex
		>> guildindex;

	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
	if (!castle)
		return ;

	if (castle->IsAttackGuild(guildindex))
		return ;

	if (castle->GetOwnerGuildIndex() == guildindex)
		return ;

	if (castle->IsDefenseGuild(guildindex))
		castle->RemoveDefenseGuild(guildindex);

	castle->AddAttackGuild(guildindex);

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	int i;
	for (i = 0; i < guild->maxmember(); i++)
	{
		if (guild->member(i))
		{
			if (castle->IsAttackChar(guild->member(i)->charindex()))
				castle->RemoveAttackChar(guild->member(i)->charindex());
			if (castle->IsDefenseChar(guild->member(i)->charindex()))
				castle->RemoveDefenseChar(guild->member(i)->charindex());

			if (guild->member(i)->GetPC())
			{
				guild->member(i)->GetPC()->SetJoinFlag(zoneindex, WCJF_ATTACK_GUILD);
				guild->member(i)->GetPC()->m_bChangeStatus = true;
			}
		}
	}
#endif // #ifdef ENABLE_WAR_CASTLE
}

void ProcHelperWarJoinDefenseGuild(CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	int zoneindex;
	int guildindex;

	msg >> zoneindex
		>> guildindex;

	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
	if (!castle)
		return ;

	if (castle->IsDefenseGuild(guildindex))
		return ;

	if (castle->GetOwnerGuildIndex() == guildindex)
		return ;

	if (castle->IsAttackGuild(guildindex))
		return ;

	castle->AddDefenseGuild(guildindex);

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	int i;
	for (i = 0; i < guild->maxmember(); i++)
	{
		if (guild->member(i))
		{
			if (castle->IsAttackChar(guild->member(i)->charindex()))
				castle->RemoveAttackChar(guild->member(i)->charindex());
			if (castle->IsDefenseChar(guild->member(i)->charindex()))
				castle->RemoveDefenseChar(guild->member(i)->charindex());

			if (guild->member(i)->GetPC())
			{
				guild->member(i)->GetPC()->SetJoinFlag(zoneindex, WCJF_DEFENSE_GUILD);
				guild->member(i)->GetPC()->m_bChangeStatus = true;
			}
		}
	}
#endif // #ifdef ENABLE_WAR_CASTLE
}

void ProcHelperWarNoticeStartAttackCastle(CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	int zoneindex;
	int remainSec;
	int guildindex1 = -1;
	int guildindex2 = -1;
	int guildindex3 = -1;
	CLCString guildname1(MAX_GUILD_NAME_LENGTH + 1);
	CLCString guildname2(MAX_GUILD_NAME_LENGTH + 1);
	CLCString guildname3(MAX_GUILD_NAME_LENGTH + 1);

	msg >> zoneindex
		>> remainSec
		>> guildindex1
		>> guildname1
		>> guildindex2
		>> guildname2
		>> guildindex3
		>> guildname3;

	CNetMsg rmsg;
	GuildWarNoticeStartCastleMsg(rmsg, zoneindex, remainSec, guildindex1, guildname1, guildindex2, guildname2, guildindex3, guildname3);
	gserver.m_playerList.SendToAll(rmsg);

	// 공성 상태 갱신 : 진행중인 서브 서버는 직접 바꾸므로 스킵
	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
	if (castle && !castle->CheckSubServer())
	{
		castle->SetState(WCSF_WAR_CASTLE);
	}
	
	// 포인트 변경 알림
	if  (castle != NULL)
	{
		gserver.m_playerList.SendGuildWarPointMsg(castle, zoneindex);
	}	

#endif // #ifdef ENABLE_WAR_CASTLE
}

void ProcHelperWarNoticeRemainFieldTime(CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	int zoneindex;
	int remainSec;

	msg >> zoneindex
		>> remainSec;

	CNetMsg rmsg;
	GuildWarNoticeRemainFieldTimeMsg(rmsg, zoneindex, remainSec);
	gserver.m_playerList.SendToWarJoinUser(rmsg, zoneindex, false);
#endif // #ifdef ENABLE_WAR_CASTLE
}

void ProcHelperWarNoticeEnd(CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	int zoneindex;
	char bWinDefense;
	int ownerguildindex;
	CLCString ownerguildname(MAX_GUILD_NAME_LENGTH + 1);
	int ownercharindex;
	CLCString ownercharname(MAX_CHAR_NAME_LENGTH + 1);
	int nextmonth;
	int nextday;
	int nexthour;
	int nextwday;

	msg >> zoneindex
		>> bWinDefense
		>> ownerguildindex
		>> ownerguildname
		>> ownercharindex
		>> ownercharname
		>> nextmonth
		>> nextday
		>> nexthour
		>> nextwday;

	CNetMsg rmsg;
	GuildWarEndMsg(rmsg, zoneindex, bWinDefense, ownerguildindex, ownerguildname, ownercharindex, ownercharname, nextmonth, nextday, nexthour, nextwday);
	gserver.m_playerList.SendToAll(rmsg);

	for( int i = 0; i < gserver.m_playerList.m_max; i++ )
	{
		CPC* pc = gserver.m_playerList.m_pcList[i];
		if (pc)
		{
			// 바로 보내기
			CNetMsg rmsg;
			StatusMsg(rmsg, pc);
			SEND_Q(rmsg, pc->m_desc );
		}
	}

	// 공성 상태 갱신 : 진행중인 서브 서버는 직접 바꾸므로 스킵
	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);

#ifdef DRATAN_CASTLE
	if( castle && zoneindex == ZONE_DRATAN )
	{
		CastletowerQuartersListMsg( rmsg, (CDratanCastle*)castle );
		castle->GetArea()->SendToAllClient(rmsg);

		GuildWarGateStateMsg(rmsg, 0xffffffff, 0xffffffff);		
		castle->GetArea()->SendToAllClient(rmsg);	
	}

#endif // DRATAN_CASTLE

	if (castle 
		&& !castle->CheckSubServer()
		&& zoneindex == ZONE_MERAC)
	{
		// 상태 변경
		castle->SetState(WCSF_NORMAL);

		// 시간 설정
		castle->m_lastWarTime = castle->GetNextWarTime() + WCT_WAR;
		castle->SetNextWarTime(0);

		// 성주 설정
		if (!bWinDefense)
		{
			if (ownerguildindex > 0)
			{
				castle->m_ownerGuildIndex = ownerguildindex;
				castle->m_ownerGuildName = ownerguildname;
				castle->m_ownerCharIndex = ownercharindex;
				castle->m_ownerCharName = ownercharname;
			}
			else
				castle->ResetOwner();
		}

		// 기타
		castle->m_bNoticeWarTime = false;
		castle->m_noticeRemain = 9999;
		castle->m_defensePoint = 0;
		castle->m_gateState = 0;

		// 참여정보 초기화
		castle->ResetJoinFlag();

		// 참여 리스트 초기화
		castle->RemoveAllJoinList();
	}
#endif // #ifdef ENABLE_WAR_CASTLE
	
#ifdef DRATAN_CASTLE
	// 공성 상태 갱신 : 진행중인 서브 서버는 직접 바꾸므로 스킵
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle && zoneindex == ZONE_DRATAN )
	{
		//공성 종료 후 강제로 존 이동 ( 수성 측 제외 )
		pCastle->EndWarRegenPoint();
	}

	if (pCastle 
		&& !pCastle->CheckSubServer()
		&& zoneindex == ZONE_DRATAN)
	{

		// 상태 변경
		pCastle->SetState(WCSF_NORMAL);

		// 시간 설정
		pCastle->m_lastWarTime = pCastle->GetNextWarTime() + WCT_WAR;
		pCastle->SetNextWarTime(0);

		// 성주 설정
		if (!bWinDefense)
		{
			if (ownerguildindex > 0)
			{
				pCastle->m_ownerGuildIndex = ownerguildindex;
				pCastle->m_ownerGuildName = ownerguildname;
				pCastle->m_ownerCharIndex = ownercharindex;
				pCastle->m_ownerCharName = ownercharname;
			}
			else
				pCastle->ResetOwner();
		}

		// 기타
		pCastle->m_bNoticeWarTime = false;
		pCastle->m_noticeRemain = 9999;
		pCastle->m_gateState = 0;

		// 참여정보 초기화
		pCastle->ResetJoinFlag();

		// 참여 리스트 초기화
		pCastle->RemoveAllJoinList();
	}
#endif // DRATAN_CASTLE
}

#ifdef ENABLE_PET
void ProcHelperPetCreateRep(CNetMsg& msg)
{
	int index;
	int owner;
	char typeGrade;

	msg >> index
		>> owner
		>> typeGrade;

	bool bSuccess = false;
	CPC* pc = gserver.m_playerList.Find(owner);
	if (pc)
	{
		int itemdbindex = 0;

		
		pc->m_desc->WaitHelperReply(false);

		const char* petTypeName;
		switch (typeGrade & PET_TYPE_MASK)
		{
		case PET_TYPE_HORSE:
			itemdbindex = PET_HORSE_ITEM_INDEX;
			petTypeName = "HORSE";
			break;
		case PET_TYPE_BLUE_HORSE:
			itemdbindex = PET_BLUE_HORSE_ITEM_INDEX;
			petTypeName = "BLUE_HORSE";
			break;
		case PET_TYPE_UNKOWN_HORSE:
			itemdbindex = PET_UNKOWN_HORSE_ITEM_INDEX;
			petTypeName = "UNKOWN_HORSE";
			break;
		case PET_TYPE_DRAGON:
			itemdbindex = PET_DRAGON_ITEM_INDEX;
			petTypeName = "DRAGON";
			break;
		case PET_TYPE_PINK_DRAGON:
			itemdbindex = PET_PINK_DRAGON_ITEM_INDEX;
			petTypeName = "PINK_DRAGON";
			break;
		case PET_TYPE_UNKOWN_DRAGON:
			itemdbindex = PET_UNKOWN_DRAGON_ITEM_INDEX;
			petTypeName = "UNKOWN_DRAGON";
			break;
		default:
			return ;
		}

		// Helper의 펫 지급 실패시 펫 아이템을 삭제하고 피리지급
		if( index == -1 )
		{
			int r, c;

			CItem* item = NULL;
			if( pc->m_invenNormal.FindItem(&r, &c, itemdbindex, 0, 0) )
			{
				item = pc->m_invenNormal.GetItem(r, c);
			}
			
			if( item )
			{
				CNetMsg rmsg;
				DecreaseFromInventory(pc, item, 1);
			
				if (item->Count() > 0)
				{
					ItemUpdateMsg(rmsg, item, -1);
					SEND_Q(rmsg, pc->m_desc);
				}
				else
				{
					ItemDeleteMsg(rmsg, item);
					SEND_Q(rmsg, pc->m_desc);
					pc->RemoveItemFromQuickSlot(item);
					RemoveFromInventory(pc, item, true, true);
				}

				int egg_index = 0;
				switch(itemdbindex)
				{
				case PET_HORSE_ITEM_INDEX:
					egg_index = PET_HORSE_EGG_INDEX;
					break;
				case PET_BLUE_HORSE_ITEM_INDEX:
					egg_index = PET_BLUE_HORSE_EGG_INDEX;
					break;
				case PET_UNKOWN_HORSE_ITEM_INDEX:
					egg_index = PET_UNKOWN_HORSE_EGG_INDEX;
					break;
				case PET_DRAGON_ITEM_INDEX:
					egg_index = PET_DRAGON_EGG_INDEX;
					break;
				case PET_PINK_DRAGON_ITEM_INDEX:
					egg_index = PET_PINK_DRAGON_EGG_INDEX;
					break;
				case PET_UNKOWN_DRAGON_ITEM_INDEX:
					egg_index = PET_UNKOWN_DRAGON_EGG_INDEX;
					break;
				}
				pc->GiveItem(egg_index, 0, 0, 1, true);
			}

			return;
		}

		CPet* pet = new CPet(pc, index, typeGrade, 1);
		if (pet)
		{
			// 펫의 이미 지급한 아이템의 옵션을 변경한닷

			//CItem* CItemList::CreateItem(int idx, char wearPos, int plus, int flag, LONGLONG count)
			int r, c;
			if( !pc->m_invenNormal.FindItem(&r, &c, itemdbindex, 0, 0) )
			{
				return;
			}
			CItem* item = pc->m_invenNormal.GetItem(r, c);
			if( item )
			{
				item->m_wearPos = -1;
				item->m_plus = index;
				item->m_flag = 0;

				CNetMsg rmsg;
				ExPetStatusMsg(rmsg, pet);
				SEND_Q(rmsg, pc->m_desc);
				ExPetSkillListMsg(rmsg, pet);
				SEND_Q(rmsg, pc->m_desc);
				ItemUpdateMsg(rmsg, item, 0);
				SEND_Q(rmsg, pc->m_desc);
				ADD_TO_BILIST(pet, pc->m_petList, m_prevPet, m_nextPet);
				bSuccess = true;

				// TODO : petlog
				GAMELOG << init("PET CREATE", pc)
						<< "TYPE" << delim
						<< petTypeName
						<< end;

				pc->SaveNow(false);
			}

			if (!bSuccess)
			{
				if (item)
					delete item;
				delete pet;
			}
		}
	}
}

// TODO : DELETE
#ifdef NEW_UI
void ProcHelperPetDeleteRep(CNetMsg& msg)
{
	int index;
	int owner;

	msg >> index
		>> owner;

	CPC* pc = gserver.m_playerList.Find(owner);
	if (pc)
	{
		pc->m_desc->WaitHelperReply(false);
		pc->DelPet(index);
	}
}
#endif // NEW_UI
#endif // #ifdef ENABLE_PET

void ProcHelperGuildStashHistoryRep(CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	int charindex;
	int errcode;
	int month[7] = {0, 0, 0, 0, 0, 0, 0};
	int day[7] = {0, 0, 0, 0, 0, 0, 0};
	LONGLONG money[7] = {0, 0, 0, 0, 0, 0, 0};

	msg >> charindex
		>> errcode;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (!pc)
		return ;

	if (errcode == MSG_HELPER_GUILD_STASH_ERROR_OK)
	{
		int i;
		for (i = 0; i < 7; i++)
		{
			msg >> month[i]
				>> day[i]
				>> money[i];
		}
	}

	GuildStashHistoryRepMsg(rmsg, (MSG_GUILD_STASH_ERROR_TYPE)errcode, month, day, money);
	SEND_Q(rmsg, pc->m_desc);

	pc->m_desc->WaitHelperReply(false);
#endif // #ifdef ENABLE_WAR_CASTLE
}

void ProcHelperGuildStashViewRep(CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	int charindex;
	int errcode;
	LONGLONG money = 0;

	msg >> charindex
		>> errcode
		>> money;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (!pc)
		return ;

	GuildStashViewRepMsg(rmsg, (MSG_GUILD_STASH_ERROR_TYPE)errcode, money);
	SEND_Q(rmsg, pc->m_desc);

	pc->m_desc->WaitHelperReply(false);
#endif // #ifdef ENABLE_WAR_CASTLE
}

void ProcHelperGuildStashTakeRep(CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	CNetMsg rmsg;

	int guildindex;
	int charindex;
	int errcode;
	LONGLONG money = 0;
	LONGLONG balance = 0;

	msg >> guildindex
		>> charindex
		>> errcode
		>> money
		>> balance;

	bool bRollback = false;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (!pc)
	{
		// 찾으려던 사람이 없어서 롤백
		bRollback = true;
	}
	else
	{
		if (errcode == MSG_HELPER_GUILD_STASH_ERROR_OK)
		{
			// 돈을 생성 지급
			CItem* item = gserver.m_itemProtoList.CreateItem(gserver.m_itemProtoList.m_moneyItem->m_index, -1, 0, 0, money);
			if (!item)
			{
				// 생성 실패시 롤백
				bRollback = true;
			}
			else
			{
				if (!AddToInventory(pc, item, true, true))
				{
					// 지급 실패시 롤백
					bRollback = true;
				}
				else
				{
					// 지급 성공
					// 아이템 지급됨을 알리고
					if (item->tab() < 0)
					{
						ItemUpdateMsg(rmsg, pc->m_moneyItem, money);
						SEND_Q(rmsg, pc->m_desc);
						delete item;
					}
					else
					{
						ItemAddMsg(rmsg, item);
						SEND_Q(rmsg, pc->m_desc);
					}
					// 성공 메시지 전달
					GuildStashTakeRepMsg(rmsg, MSG_GUILD_STASH_ERROR_OK);
					SEND_Q(rmsg, pc->m_desc);

					GAMELOG << init("GUILD STASH TAKE MONEY", pc)
							<< "GUILD" << delim
							<< guildindex << delim
							<< "MONEY" << delim
							<< money << delim
							<< "BALANCE" << delim
							<< balance
							<< end;
				}
			}
		}
		else
		{
			// 실패 알림
			GAMELOG << init("GUILD STASH TAKE MONEY FAIL", pc)
					<< "GUILD" << delim
					<< guildindex << delim
					<< "ERROR" << delim
					<< errcode
					<< end;
			GuildStashTakeRepMsg(rmsg, (MSG_GUILD_STASH_ERROR_TYPE)errcode);
			SEND_Q(rmsg, pc->m_desc);
		}

		pc->m_desc->WaitHelperReply(false);
	}

	if (bRollback && errcode == MSG_HELPER_GUILD_STASH_ERROR_OK)
	{
		if (IS_RUNNING_HELPER)
		{
			if (pc)
				GAMELOG << init("GUILD STASH ROLLBACK TAKE MONEY", pc);
			else
				GAMELOG << init("GUILD STASH ROLLBACK TAKE MONEY") << "CHARINDEX" << delim << charindex << delim;
			GAMELOG << "GUILD" << delim
					<< guildindex << delim
					<< "MONEY" << delim
					<< money << delim
					<< "BALANCE" << delim
					<< balance
					<< end;

			HelperGuildStashRollbackMsg(rmsg, guildindex, money);
			SEND_Q(rmsg, gserver.m_helper);
		}
		else
		{
			if (pc)
				GAMELOG << init("GUILD STASH FAIL ROLLBACK TAKE MONEY", pc);
			else
				GAMELOG << init("GUILD STASH FAIL ROLLBACK TAKE MONEY") << "CHARINDEX" << delim << charindex << delim;
			GAMELOG << "GUILD" << delim
					<< guildindex << delim
					<< "MONEY" << delim
					<< money << delim
					<< "BALANCE" << delim
					<< balance
					<< end;
		}
	}
#endif // #ifdef ENABLE_WAR_CASTLE
}

void ProcHelperGuildStashSaveTaxRep(CNetMsg& msg)
{
#ifdef ENABLE_WAR_CASTLE
	int errcode;
	int guildindex = 0;
	int zoneindex = 0;
	LONGLONG taxItem;
	LONGLONG taxProduce;

	msg >> errcode
		>> guildindex
		>> zoneindex
		>> taxItem
		>> taxProduce;

	switch (errcode)
	{
	case MSG_HELPER_GUILD_STASH_ERROR_OK:
		{
			CDBCmd cmd;
			cmd.Init(&gserver.m_dbcastle);
			struct tm tmCur = NOW();
#ifdef CIRCLE_WINDOWS
			sprintf(g_buf, "UPDATE t_castle SET a_tax_wday=%d, a_tax_item = a_tax_item - %I64d, a_tax_produce = a_tax_produce - %I64d WHERE a_zone_index = %d", tmCur.tm_wday, taxItem, taxProduce, zoneindex);
#else
			sprintf(g_buf, "UPDATE t_castle SET a_tax_wday=%d, a_tax_item = a_tax_item - %lld,  a_tax_produce = a_tax_produce - %lld  WHERE a_zone_index = %d", tmCur.tm_wday, taxItem, taxProduce, zoneindex);
#endif
			cmd.SetQuery(g_buf);
			if (!cmd.Update())
			{
				GAMELOG << init("TAX RESET FAIL")
						<< "ZONE" << delim
						<< zoneindex << delim
						<< "GUILD" << delim
						<< guildindex << delim
						<< "TAX ITEM" << delim
						<< taxItem << delim
						<< "TAX PRODUCT" << delim
						<< taxProduce
						<< end;
			}
			else
			{
				GAMELOG << init("TAX RESET SUCCESS")
						<< "ZONE" << delim
						<< zoneindex << delim
						<< "GUILD" << delim
						<< guildindex << delim
						<< "TAX ITEM" << delim
						<< taxItem << delim
						<< "TAX PRODUCT" << delim
						<< taxProduce
						<< end;
			}

// TODO : DELETE
//			// 일요일 지급이면 a_tax_guild_index를 갱신
//			if (tmCur.tm_wday == 0)
//			{
//				sprintf(g_buf, "UPDATE t_castle SET a_tax_item = 0, a_tax_produce = 0, a_tax_guild_index = a_owner_guild_index WHERE a_zone_index = %d", CWarCastle::GetCurSubServerCastleZoneIndex());
//				cmd.SetQuery(g_buf);
//				if (!cmd.Update())
//				{
//					GAMELOG << init("TAX GUILD RESET FAIL")
//							<< "ZONE" << delim
//							<< CWarCastle::GetCurSubServerCastleZoneIndex()
//							<< end;
//				}
//				else
//				{
//					GAMELOG << init("TAX GUILD RESET SUCCESS")
//							<< "ZONE" << delim
//							<< CWarCastle::GetCurSubServerCastleZoneIndex()
//							<< end;
//				}
//			}
		}
		break;

	default:
		GAMELOG << init("TAX SAVE FAIL")
				<< "ZONE" << delim
				<< zoneindex << delim
				<< "GUILD" << delim
				<< guildindex << delim
				<< "TAX ITEM" << delim
				<< taxItem << delim
				<< "TAX PRODUCT" << delim
				<< taxProduce
				<< end;
		break;
	}
#endif // #ifdef ENABLE_WAR_CASTLE
}

void ProcHelperTeacherLoadRep(CNetMsg& msg)
{
	
	CLCString idname(MAX_ID_NAME_LENGTH + 1);
	int charindex;

	msg >> idname
		>> charindex;

	CDescriptor* d = gserver.FindConnectIdName(idname, NULL);
	if( d && d->m_pChar && d->m_pChar->m_index )
	{
		char bteacher;
		msg	 >> d->m_pChar->m_cntTeachingStudent
			 >> d->m_pChar->m_guardian
			 >> d->m_pChar->m_superstone
			 >> d->m_pChar->m_cntFailStudent
			 >> d->m_pChar->m_cntCompleteStudent
			 >> d->m_pChar->m_teachType
			 >> d->m_pChar->m_fame
			 >> bteacher;

		if(bteacher)
			d->m_pChar->m_bTeacher = true;
		else
			d->m_pChar->m_bTeacher = false;

		if( d->m_pChar->m_teachType != MSG_TEACH_TEACHER_TYPE && d->m_pChar->m_teachType != MSG_TEACH_STUDENT_TYPE)
			return;

		if( d->m_pChar->m_teachType == MSG_TEACH_TEACHER_TYPE )
		{
			CLCString teachName(MAX_CHAR_NAME_LENGTH + 1);
			for(int i = 0; i < TEACH_MAX_STUDENTS; i++)
			{
				msg	>> d->m_pChar->m_teachIdx[i]
					>> d->m_pChar->m_teachJob[i]
#ifdef ENABLE_CHANGEJOB
					>> d->m_pChar->m_teachJob2[i]
#endif
					>> d->m_pChar->m_teachLevel[i]
					>> teachName
					>> d->m_pChar->m_teachTime[i];
				strcpy(d->m_pChar->m_teachName[i], teachName);

				if( d->m_pChar->m_teachIdx[i] != -1 )
				{
					CPC* student = gserver.m_playerList.Find(d->m_pChar->m_teachIdx[i]);
					if( student )
					{
						d->m_pChar->m_teachIdx[i] = student->m_index;
						d->m_pChar->m_teachJob[i] = student->m_job;
#ifdef ENABLE_CHANGEJOB
						d->m_pChar->m_teachJob2[i] = student->m_job2;
#endif
						d->m_pChar->m_teachLevel[i] = student->m_level;
						strcpy(d->m_pChar->m_teachName[i], student->GetName());
					}
				}
			}
		}
		else // 학생
		{
			CLCString teachName(MAX_CHAR_NAME_LENGTH + 1);
			msg	>> d->m_pChar->m_teachIdx[0]
				>> d->m_pChar->m_teachJob[0]
#ifdef ENABLE_CHANGEJOB
				>> d->m_pChar->m_teachJob2[0]
#endif
				>> d->m_pChar->m_teachLevel[0]
				>> teachName
				>> d->m_pChar->m_teachTime[0];
			strcpy(d->m_pChar->m_teachName[0], teachName);

			// 자신이 학생이면 idx[1]을 선생의 명성수치로, idx[2]를 선생의 양성중이 초보로, time[1]을 완료인원, time[2]을 실패인원으로 쓴다.
			msg >> d->m_pChar->m_teachIdx[1]
				>> d->m_pChar->m_teachIdx[2]
				>> d->m_pChar->m_teachTime[1]
				>> d->m_pChar->m_teachTime[2];


			if( d->m_pChar->m_teachIdx[0] != -1 )
			{
				CPC* student = gserver.m_playerList.Find(d->m_pChar->m_teachIdx[0]);
				if( student )
				{
					d->m_pChar->m_teachIdx[0] = student->m_index;
					d->m_pChar->m_teachJob[0] = student->m_job;
#ifdef ENABLE_CHANGEJOB
					d->m_pChar->m_teachJob2[0] = student->m_job2;
#endif
					d->m_pChar->m_teachLevel[0] = student->m_level;
					strcpy(d->m_pChar->m_teachName[0], student->GetName());
				}
			}
		}
	}
}

void ProcHelperFameupRep(CNetMsg& msg)
{
	int teachidx;
	int studentidx;
	CLCString studentname(MAX_CHAR_NAME_LENGTH + 1);
	int fameup;

	msg >> teachidx
		>> studentidx
		>> studentname
		>> fameup;

	CPC* tch = gserver.m_playerList.Find(teachidx);
	if(tch)
	{
		
		
		GAMELOG << init("TEACH_SUCCESS_HELPER_REP")
				<< studentidx << delim
				<< studentname << delim
				<< tch->m_name << delim
				<< tch->m_nick << delim
				<< tch->m_desc->m_idname << delim
				<< tch->m_fame << end;
				
#if defined( EVENT_TEACH ) || defined ( EVENT_CHILDERN_DAY )
		tch->m_fame = tch->m_fame + TEACH_FAME_PRIZE * 2;
#else
		tch->m_fame = tch->m_fame + TEACH_FAME_PRIZE;
#endif

		CNetMsg rmsg;
		TeachEndMsg(rmsg, tch->m_index, tch->GetName(), studentidx, studentname, MSG_TEACH_END_SUCCESS);
		SEND_Q(rmsg, tch->m_desc);

		//0627
		if( tch->m_displayCanSstone 
			&& ((tch->m_fame >= 200  && tch->m_superstone < 1 )
			|| (tch->m_fame >= 300 && tch->m_superstone < 2 )
			|| (tch->m_fame >= 500 && tch->m_superstone < 3)
			|| (tch->m_fame >= 800 && tch->m_superstone < 4)
			|| (tch->m_fame >= 1200 && tch->m_superstone < 5)
			|| (tch->m_fame >= 1700 && tch->m_superstone < 6)
			|| (tch->m_fame >= 2300 && tch->m_superstone < 7)) )
		{
			SysEnableSuperStoneMsg(rmsg, tch->m_fame);
			SEND_Q(rmsg, tch->m_desc);
			tch->m_displayCanSstone = false;
		}				
		tch->m_cntCompleteStudent++;

		tch->m_bChangeStatus = true;
		// 셋팅해제
		do_TeachSetCancel(tch, studentidx);
	}
	else
	{
		GAMELOG << init("TEACH_SUCCESS_HELPER_REP-notTeacher")
				<< studentidx << delim
				<< studentname << delim
				<< teachidx << delim
				<< fameup << end;
	}
}

#ifdef EVENT_SEARCHFRIEND
void ProcHelper_EventSearchFriendAddRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	
	int approvalindex;	
	CLCString reqnick(MAX_CHAR_NAME_LENGTH + 1);
	unsigned char errcode;
	
	msg >> approvalindex
		>> reqnick
		>> errcode;		

	CPC* appPc = gserver.m_playerList.Find(approvalindex);

	if ( appPc )
	{
		appPc->m_bGoodEventSearchFriendListImprove = true;
		

		EventSearchFriendErrorMsg(rmsg, (MSG_EVENT_SEARCHFRIEND_ERROR_TYPE)errcode);//요청자에게 성공여부를 알린다.
		SEND_Q(rmsg, appPc->m_desc);

		GAMELOG << init("EVENT_SEARCHFRIEND_ERROR_TYPE-Add Chartacter", appPc)
				<< reqnick << delim
				<< (MSG_EVENT_SEARCHFRIEND_ERROR_TYPE)errcode << end;
	}
}

//휴면케릭 선택한것을 알리는 부분.
void ProcHelper_EventSearchFriendSelectAddRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	
	int approvalindex;	
	unsigned char errcode;
	
	msg >> approvalindex
		>> errcode;		

	CPC* appPc = gserver.m_playerList.Find(approvalindex);
	if( appPc )
	{

		appPc->m_bEventSearchFriendSelect = true;
		
		EventSearchFriendSelectAddErrorMsg(rmsg, (MSG_EVENT_SEARCHFRIEND_ERROR_TYPE)errcode);//요청자에게 성공여부를 알린다.
		SEND_Q(rmsg, appPc->m_desc);

		GAMELOG << init("EVENT_SEARCHFRIEND_ERROR_TYPE-List Select")
				<< approvalindex << delim
				<< (MSG_EVENT_SEARCHFRIEND_ERROR_TYPE)errcode << end;
	}
}

//휴면케릭 한시간 단위저장응답
void ProcHelper_EventSearchFriendOneTimeAddRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	
	int timesec;
	int appDormantindex;	
	unsigned char errcode;
	
	msg >> timesec
		>> appDormantindex		
		>> errcode;

	CPC* appPc = gserver.m_playerList.Find(appDormantindex);
	if( appPc )
	{
	
		EventSearchFriendOneTimeAddErrorMsg(rmsg, timesec, (MSG_EVENT_SEARCHFRIEND_ERROR_TYPE)errcode);//요청자에게 성공여부를 알린다.
		SEND_Q(rmsg, appPc->m_desc);

		GAMELOG << init("EVENT_SEARCHFRIEND_ERROR_TYPE-One Time")
				<< timesec << delim
				<< appDormantindex << delim
				<< (MSG_EVENT_SEARCHFRIEND_ERROR_TYPE)errcode << end;
	}	
}

//휴면 케릭 이벤트 응답 받고 화면으로 보내는 부분 (등록된 친구 보상가능 여부 중 선택해서 받는 부분)
void ProcHelper_EventSearchFriendListGoodRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	
	int approvalindex, request_listmember, startIndex, nTotal;
	int request_index[50], item_good[50];
	char request_nick[50][MAX_CHAR_NAME_LENGTH + 1];
	CLCString rnick(MAX_CHAR_NAME_LENGTH + 1);

	memset(request_nick, 0, sizeof(request_nick));
	
	msg >> approvalindex
		>> request_listmember
		>> startIndex
		>> nTotal;

	int i;
	for (i = 0; i < request_listmember; i++)
	{
		msg >> request_index[i]
			>> rnick
			>> item_good[i];
		strcpy(request_nick[i], rnick);
	}
	
	CPC* appPc = gserver.m_playerList.Find(approvalindex);	
	if( appPc )
	{
		for(i = 0; i < request_listmember; i++)
		{
			appPc->m_nGoodEventSearchFriendIndex[startIndex + i] = request_index[i];
			strncpy(appPc->m_nGoodEventSearchFriendNick[startIndex + i], request_nick[i], MAX_CHAR_NAME_LENGTH);
			appPc->m_nGoodEventSearchFriendNick[startIndex + i][MAX_CHAR_NAME_LENGTH] = '\0';
			appPc->m_nGoodEventSearchFriendGood[startIndex + i] = item_good[i];
		}
		appPc->m_nRegFriend = 0;
		appPc->m_nGoodEventSearchFriendListGoodCount = nTotal;
		appPc->m_bGoodEventSearchFriendListImprove = false;
		appPc->m_pulseEventSearchFriendList = gserver.m_pulse;

		//아이템 리스트 개수 ,/리스트에 올라온 케릭 인덱스,/ 케릭 이름,
		// 보상 여부( 값이 0일경우 불가능, 1 이상일 경우 개수만큼 받을 수 있는 개수.		
		EventSearchFriendListGoodsMsg(rmsg,request_listmember, request_index
										  ,request_nick, item_good, startIndex, nTotal);
		SEND_Q(rmsg, appPc->m_desc);

		GAMELOG << init("EVENT_SEARCHFRIEND_GOOD_LIST_READ")
				<< approvalindex << delim
				<< "List num" << request_listmember << delim
				<< end;
	}
}

//휴면케릭 보상 결과 응답
void ProcHelper_EventSearchFriendGoodRep(CNetMsg& msg)
{
	CNetMsg rmsg;	
	int appindex;
	unsigned char errcode;	
	msg >> appindex
		>> errcode; 

	CPC* appPc = gserver.m_playerList.Find(appindex);
	if ( appPc )
	{
		appPc->m_bGoodEventSearchFriendListImprove = true;

		int nGoodsIndex = 85;
		int nGoodsCount = appPc->m_nGoodEventSearchFriendGood[appPc->m_nGoodEventSearchFriendGoodNumber];

		if(errcode == MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_HELPER)
		{
			GAMELOG << init("EVENT SEARCH FRIEND ERROR_HELPER", appPc)
					<< "absconder with Item "<< delim
					<< nGoodsCount << delim << end;
			// 카운트 줄이기
			int r, c;
			if( !appPc->m_invenNormal.FindItem(&r, &c, nGoodsIndex, 0, 0) )
			{
				return;
			}

			CItem* item = appPc->m_invenNormal.GetItem(r, c);
			
			if (!item)
			{
				GAMELOG << init("EVENT SEARCH FRIEND ERROR_HELPER", appPc)
						<< "absconder with Item "<< delim
						<< nGoodsCount << delim << end;
				return;
			}
			
			// Item 수량 변경
			DecreaseFromInventory(appPc, item, nGoodsCount);
			if (item->Count() > 0)
			{
				ItemUpdateMsg(rmsg, item, -(nGoodsCount));
				SEND_Q(rmsg, appPc->m_desc);
			}
			else
			{
				ItemDeleteMsg(rmsg, item);
				SEND_Q(rmsg, appPc->m_desc);
				RemoveFromInventory(appPc, item, true, true);
			}
		}
		else if(errcode == MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_OK)
		{
			GAMELOG << init("EVENT_SEARCHFRIEND_GOOD_LIST_READ", appPc)
					<< nGoodsCount << delim
					<< "GOOD OK"<< end;
			EventSearchFriendGoodsMsg(rmsg, (MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TYPE)errcode);
			SEND_Q(rmsg, appPc->m_desc);
		}
	}
	else
	{
		GAMELOG << init("EVENT SEARCH FRIEND ERROR_HELPER", appPc)
		<< "absconder with Item "<< end;
		return;
	}
}
#endif // #ifdef EVENT_SEARCHFRIEND

void ProcHelperPartyMemberInfo(CNetMsg& msg)
{
	int nBossIndex;
	int nCharIndex;
	int nCharLevel;
	int nHP;
	int nMaxHP;
	int nMP;
	int nMaxMP;
	float x;
	float z;
	char nYlayer;
	int nZone;

	msg >> nBossIndex
		>> nCharIndex
		>> nCharLevel
		>> nHP
		>> nMaxHP
		>> nMP
		>> nMaxMP
		>> x
		>> z
		>> nYlayer
		>> nZone;

	CParty* pParty = gserver.FindPartyByBossIndex(nBossIndex);
	if (pParty)
	{
		CNetMsg rmsg;
		PartyInfoMsg(rmsg, nCharIndex, nCharLevel, nHP, nMaxHP, nMP, nMaxMP, x, z, nYlayer, nZone);
		pParty->SendToAllPC(rmsg, nCharIndex);
	}
}

void ProcHelperPartyMemberChangeJob(CNetMsg& msg)
{
	int nBossIndex;
	int nCharIndex;
	char job1;
	char job2;

	msg >> nBossIndex
		>> nCharIndex
		>> job1
		>> job2;

	CParty* pParty = gserver.FindPartyByBossIndex(nBossIndex);
	if (pParty)
	{
		CNetMsg rmsg;
		ChangeJobRepMsg(rmsg, nCharIndex, job1, job2);
		pParty->SendToAllPC(rmsg, nCharIndex);
	}
}

void ProcHelperPartyChat(CNetMsg& msg)
{
	int nBossIndex;
	int nCharIndex;
	CLCString strName(MAX_CHAR_NAME_LENGTH + 1);
	CLCString strChat(1000);

	msg >> nBossIndex
		>> nCharIndex
		>> strName
		>> strChat;

	CParty* pParty = gserver.FindPartyByBossIndex(nBossIndex);
	if (pParty)
	{
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_PARTY, nCharIndex, strName, "", strChat);
		pParty->SendToAllPC(rmsg);
#ifdef GMTOOL
		if(pParty->m_bGmtarget)
		{
			// 메신저 서버를 통해서 툴로 채팅내용을 보낸다.
			if( IS_RUNNING_MSGR )
			{
				int i;
				bool bSend = false;
				for(i = 0; i < MAX_PARTY_MEMBER; ++i)
				{
					const CPartyMember* member = pParty->GetMemberByListIndex(i);
					if(member && member->GetMemberPCPtr() && member->GetMemberPCPtr()->m_bGmMonitor)
					{
						CNetMsg rmsg;
						MsgrNoticeGmChatMonitorPartyMsg(rmsg, -1, 1, 1, 0, gserver.m_serverno, gserver.m_subno, -1, strChat, nBossIndex, strName, member->GetMemberPCPtr()->m_index, member->GetMemberPCPtr()->m_name);
						SEND_Q(rmsg, gserver.m_messenger);

						bSend = true;
					}
				}

				if(!bSend)
				{
					pParty->m_bGmtarget = false;
				}
			}
		}
#endif // GMTOOL
	}
}

void ProcHelperPartyRecallPrompt(CNetMsg& msg)
{
	int nBossIndex;
	int nReqIndex;
	CLCString strReqName(MAX_CHAR_NAME_LENGTH + 1);
	char cIsInCastle = 0;
	int nGuildIndex = 0;

	msg >> nBossIndex
		>> nReqIndex
		>> strReqName
		>> cIsInCastle
		>> nGuildIndex;

	CParty* pParty = gserver.FindPartyByBossIndex(nBossIndex);
	if (pParty)
	{
		CNetMsg rmsg;
		
	///yhj  081023  파티리콜 공성전 중에 금지  // 공성중이거나 시작 1분 전에는 공성지역으로 이동 불가

		//요청한 유저를 찾고
		CPC* ch = gserver.m_playerList.Find(nReqIndex);
		if( !ch )
			return ;

#ifdef ENABLE_WAR_CASTLE
			
		// 요청한 유저를 통해서 공성전클래스 초기화
		CWarCastle* pCastle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);

		// 요청한 유저가 공성전에 있는지 검사
		if (pCastle)
		{
			if (pCastle->GetState() != WCSF_NORMAL || pCastle->IsWarCastleReady() )
			{
				// 공성 진행중인 서브 서버이면
				// 성안 영역인가 확인하고
				//if (pCastle->IsInInnerCastle(ch) )
				if ( pCastle->IsInInnerCastle(ch) || ch->m_pArea->GetAttr( GET_YLAYER(ch), GET_X(ch), GET_Z(ch) ) == MAPATT_WARZONE )
				{		
					if( ch->m_pZone->m_index == ZONE_DRATAN || ch->m_pZone->m_index == ZONE_MERAC )
					{	// 드라탄 공성이나 메라크 공성은 파티 리콜 사용 금지
						// 경고 메시지 보내고
						ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_WARN_PARTYRECALL);
						SEND_Q(rmsg, ch->m_desc);
						
						//공성시간에 PARTY_RECALL 이동 로그
						GAMELOG << init(" PARTY_RECALL IN WARTIME " , ch ) << end;
						return; 
					}
				}
			}
		}
#endif // ENABLE_WAR_CASTLE/////////////////////////////////////////////////////////////////////////

		///yhj  081023  파티리콜 pvp아레나 존에서는 금지
		if ( ch->m_pZone->m_index == ZONE_PK_TOURNAMENT )
		{
			// 경고 메시지 보내고
			ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_WARN_PARTYRECALL);
			SEND_Q(rmsg, ch->m_desc);
			
			//pvp아레나에서 PARTY_RECALL 이동 로그
			GAMELOG << init(" PARTY_RECALL IN PK_TOURNAMENT " , ch ) << end;
			return; 
		}/////////////////////////////////////////////////////////////////////////////////////////

			
				/*	///yhj  081023  파티리콜 공성전 중에 금지 
			
				int i;
				for (i = 0; i < MAX_PARTY_MEMBER; i++)
				{
					// 파티원 중에
					if (	   pParty->GetMemberByListIndex(i)
							&& pParty->GetMemberByListIndex(i)->GetMemberPCPtr()
							&& pParty->GetMemberByListIndex(i)->GetCharIndex() != nReqIndex
						)
					{
						// 요청한 사람과 같은 길드 사람이 있으면
						if (	   pParty->GetMemberByListIndex(i)->GetMemberPCPtr()->m_guildInfo
								&& pParty->GetMemberByListIndex(i)->GetMemberPCPtr()->m_guildInfo->guild()
								&& pParty->GetMemberByListIndex(i)->GetMemberPCPtr()->m_guildInfo->guild()->index() == nGuildIndex
							)
						{
							// 대려온다.
							SEND_Q(rmsg, pParty->GetMemberByListIndex(i)->GetMemberPCPtr()->m_desc);
						}
					}
				}*///////////////////////////////////

		ExPartyRecallPromptMsg(rmsg, nReqIndex, strReqName);
		pParty->SendToAllPC(rmsg, nReqIndex);
	}
}

void ProcHelperPartyRecallConfirm(CNetMsg& msg)
{
	int nBossIndex;
	int reqindex;
	int repindex;
	char yesno;
	CLCString strReqName(MAX_CHAR_NAME_LENGTH + 1);
	CLCString strRepName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> nBossIndex
		>> reqindex
		>> strReqName
		>> repindex
		>> strRepName
		>> yesno;

	CPC* pReqPC = gserver.m_playerList.Find(reqindex);
	if (pReqPC)
	{
		CNetMsg rmsg;

		if (yesno)
		{
			if (!pReqPC->m_pZone->IsPersonalDungeon() && !pReqPC->m_pZone->IsGuildRoom() && !pReqPC->m_pZone->IsComboZone())
			{
				if (IS_RUNNING_HELPER)
				{
					int nGuildIndex = 0;
					char cIsInCastle = 0;

#ifdef ENABLE_WAR_CASTLE
					CWarCastle* pCastle = CWarCastle::GetCastleObject(pReqPC->m_pZone->m_index);
					if (pCastle && pCastle->GetState() != WCSF_NORMAL && pCastle->CheckSubServer())
					{
						// 공성 진행중인 서브 서버이면
						// 성안 영역인가 확인하고
						if (pCastle->IsInInnerCastle(pReqPC))
						{
							// 경고 메시지 보내고
							// 길드 정보를 구해서
							if (pReqPC->m_guildInfo && pReqPC->m_guildInfo->guild())
								nGuildIndex = pReqPC->m_guildInfo->guild()->index();
							cIsInCastle = 1;
						}
					}
#endif // ENABLE_WAR_CASTLE

					HelperPartyRecallProcMsg(rmsg, nBossIndex, repindex, pReqPC->m_pZone->m_index, GET_X(pReqPC), GET_Z(pReqPC), GET_YLAYER(pReqPC), cIsInCastle, nGuildIndex);
					SEND_Q(rmsg, gserver.m_helper);
				}
			}
		}
		else
		{
			// NO 이면 req에게 알리고 마침
			ExPartyRecallCancelMsg(rmsg, reqindex, strReqName, repindex, strRepName);
			SEND_Q(rmsg, pReqPC->m_desc);
		}
	}
}

void ProcHelperPartyRecallProc(CNetMsg& msg)
{
	int nBossIndex;
	int nRepIndex;
	int nZone;
	float x;
	float z;
	char nYlayer;
	char cIsInCastle = 0;
	int nGuildIndex = 0;

	msg >> nBossIndex
		>> nRepIndex
		>> nZone
		>> x
		>> z
		>> nYlayer
		>> cIsInCastle
		>> nGuildIndex;

	CPC* pRepPC = gserver.m_playerList.Find(nRepIndex);
	if (pRepPC)
	{
		if (pRepPC->IsParty() && pRepPC->m_party->GetBossIndex() == nBossIndex)
		{
			if (cIsInCastle)
			{
				if (pRepPC->m_guildInfo && pRepPC->m_guildInfo->guild() && pRepPC->m_guildInfo->guild()->index() == nGuildIndex)
					GoZone(pRepPC, nZone, nYlayer, x, z);
			}
			else
			{
				GoZone(pRepPC, nZone, nYlayer, x, z);
			}
		}
	}
}

void ProcHelperPartyInviteReq(CNetMsg& msg)
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

	GAMELOG << init("PARTY DEBUG HELPER INVITE REQ")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nBossLevel" << delim << nBossLevel << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "nPartyType" << delim << nPartyType
			<< end;

	CPC* pTargetPC = gserver.m_playerList.Find(nTargetIndex);

	if (!pTargetPC)
		return ;

#if defined (LC_USA) || defined (LC_HBK)
	CPC* pBossPC = gserver.m_playerList.Find(nBossIndex);
	if (!pBossPC)
		return ;

	if(pBossPC->IsSetPlayerState(PLAYER_STATE_PKMODE) || pTargetPC->IsSetPlayerState(PLAYER_STATE_PKMODE))
	{
		HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, pTargetPC->GetName(), nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_PVP);
		if (IS_RUNNING_HELPER)
			SEND_Q(rmsg, gserver.m_helper);
		return ;
	}
#endif	// #if defined (LC_USA) || defined (LC_HBK)

	// 파티 타입이 2이면 tch와 ch의 레벨 차이를 검사
	// 차이가 +- 10이상이면 ERROR;
	if (nPartyType == PARTY_TYPE_BATTLE)
	{
		if (ABS(pTargetPC->m_level - nBossLevel) > 10)
		{
			HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, pTargetPC->GetName(), nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_LEVEL);
			if (IS_RUNNING_HELPER)
				SEND_Q(rmsg, gserver.m_helper);
			return ;
		}
	}

	HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, pTargetPC->GetName(), nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_OK);
	if (IS_RUNNING_HELPER)
		SEND_Q(rmsg, gserver.m_helper);
}

void ProcHelperPartyInviteRep(CNetMsg& msg)
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

	GAMELOG << init("PARTY DEBUG HELPER INVITE REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "strTargetName" << delim << strTargetName << delim
			<< "nPartyType" << delim << nPartyType << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CPC* pBossPC = gserver.m_playerList.Find(nBossIndex);
	CPC* pTargetPC = gserver.m_playerList.Find(nTargetIndex);
	CParty* pParty = NULL;

	switch (nErrorCode)
	{
	case MSG_HELPER_PARTY_ERROR_INVITE_OK:
		pParty = gserver.FindPartyByBossIndex(nBossIndex);
		if (pParty)
		{
			// 기존 파티에 초대 상태로
			pParty->SetRequest(nTargetIndex, strTargetName);
			if (pTargetPC)
				pTargetPC->m_party = pParty;
		}
		else
		{
			// 신규 파티
			pParty = new CParty(nPartyType, nBossIndex, strBossName, pBossPC, nTargetIndex, strTargetName);

			if (pBossPC)		pBossPC->m_party = pParty;
			if (pTargetPC)		pTargetPC->m_party = pParty;

			gserver.m_listParty.AddToTail(pParty);
		}

		PartyInviteMsg(rmsg, nPartyType, nBossIndex, strBossName);
		if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
		if (pTargetPC)		SEND_Q(rmsg, pTargetPC->m_desc);
		break;

	case MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_ME:
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_ALREADY_ME);
		if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		break;

	case MSG_HELPER_PARTY_ERROR_INVITE_DIFF_TYPE:
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_DIFF_TYPE);
		if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		break;

	case MSG_HELPER_PARTY_ERROR_INVITE_LEVEL:
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_INVALID_LEVEL);
		if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		break;

	case MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_TARGET:
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_ALREADY_TARGET);
		if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		break;

	case MSG_HELPER_PARTY_ERROR_INVITE_FULL:
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_FULL);
		if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		break;

	case MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_REQUEST:
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_ALREADY_REQUEST);
		if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		break;
#if defined (LC_USA) || defined (LC_HBK)
	case MSG_HELPER_PARTY_ERROR_INVITE_PVP:
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_PVP);
		if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		break;
#endif	// #if defined (LC_USA) || defined (LC_HBK)
	}
}

void ProcHelperPartyAllowRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	CNetMsg addmsg;

	int			nBossIndex;
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);
	int			nErrorCode;

	msg >> nBossIndex
		>> nTargetIndex
		>> strTargetName
		>> nErrorCode;

	GAMELOG << init("PARTY DEBUG HELPER ALLOW REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "strTargetName" << delim << strTargetName << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CPC* pTargetPC = gserver.m_playerList.Find(nTargetIndex);
//	if (pTargetPC)
//		pTargetPC->m_desc->WaitHelperReply(false);

	CParty* pParty = gserver.FindPartyByBossIndex(nBossIndex);
	if (pParty == NULL)
	{
		GAMELOG << init("PARTY DEBUG HELPER ALLOW REP : NOT FOUND PARTY") << end;
		return ;
	}

	int nListIndex = pParty->JoinRequest(strTargetName, pTargetPC);
	if (nListIndex == -1)
	{
		GAMELOG << init("PARTY DEBUG HELPER ALLOW REP : CANNOT JOIN") << end;
		return ;
	}

	// 파티원들에게 결과 알리기
	int i;
	PartyAddMsg(addmsg, nTargetIndex, strTargetName, pTargetPC, ((pParty->GetBossIndex() == nTargetIndex) ? 1 : 0));
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		const CPartyMember* pMember = pParty->GetMemberByListIndex(i);
		if (pMember && i != nListIndex)
		{
			// 타인에게 target을 추가하고
			if (pMember->GetMemberPCPtr())
				SEND_Q(addmsg, pMember->GetMemberPCPtr()->m_desc)

			// target에게는 타인을 추가
			if (pTargetPC)
			{
				PartyAddMsg(rmsg, pMember->GetCharIndex(), pMember->GetCharName(), pMember->GetMemberPCPtr(), ((pParty->GetBossIndex() == pMember->GetCharIndex()) ? 1 : 0));
				SEND_Q(rmsg, pTargetPC->m_desc);
			}
		}
	}
#ifdef GMTOOL
	CPC* boss = gserver.m_playerList.Find(nBossIndex);
	if((pTargetPC && pTargetPC->m_bGmMonitor) || (boss && boss->m_bGmMonitor))
	{
		pParty->m_bGmtarget = true;
	}
#endif // GMTOOL
}

void ProcHelperPartyRejectRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int			nBossIndex;
	int			nTargetIndex;
	int			nErrorCode;

	msg >> nBossIndex
		>> nTargetIndex
		>> nErrorCode;

	GAMELOG << init("PARTY DEBUG HELPER REJECT REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CParty* pParty = gserver.FindPartyByBossIndex(nBossIndex);
	if (!pParty)
	{
		GAMELOG << init("PARTY DEBUG HELPER REJECT REP : NOT FOUND PARTY") << end;
		return ;
	}
	if (pParty->GetRequestIndex() < 1)
	{
		GAMELOG << init("PARTY DEBUG HELPER REJECT REP : NOT FOUND REQUEST") << end;
		return ;
	}

	CPC* pBossPC = pParty->GetMemberByListIndex(0)->GetMemberPCPtr();
//	CPC* pTargetPC = gserver.m_playerList.Find(nTargetIndex);
	CPC* pRequestPC = gserver.m_playerList.Find(pParty->GetRequestIndex());
//	if (pTargetPC)
//		pTargetPC->m_desc->WaitHelperReply(false);


	if (nErrorCode == MSG_HELPER_PARTY_ERROR_REJECT_OK)
	{
		if (pParty->GetRequestIndex() == nTargetIndex)
			PartyMsg(rmsg, MSG_PARTY_REJECT_DEST);
		else if (pParty->GetBossIndex() == nTargetIndex)
			PartyMsg(rmsg, MSG_PARTY_REJECT_SRC);
		else
		{
			GAMELOG << init("PARTY DEBUG HELPER REJECT REP : NO MATCH CHAR") << end;
			return ;
		}

		pParty->SetMemberPCPtr(pParty->GetRequestIndex(), NULL);

		if (pBossPC)
			SEND_Q(rmsg, pBossPC->m_desc);
		if (pRequestPC)
		{
			SEND_Q(rmsg, pRequestPC->m_desc);
			pRequestPC->m_party = NULL;
		}

		pParty->SetRequest(-1, "");
		if (pParty->GetMemberCount() < 2)
		{
			// 파티 종료
#ifdef EXTREME_CUBE
		if(pParty->m_cubeUniqueIdx != -1)
		{
			CCubeMemList* memlist = gserver.m_extremeCube.FindMemList(pParty);
			if(memlist && memlist->IsPartyCubeMemList())
			{
				((CPartyCubeMemList*)memlist)->SetParty(NULL);
			}
		}
#endif // EXTREME_CUBE
			gserver.m_listParty.RemoveData(pParty);
			delete pParty;
		}
	}
}

void ProcHelperPartyQuitRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int			nBossIndex;
	int			nTargetIndex;
	int			nErrorCode;
	int			nNewBossIndex;
	CLCString	strNewBossName(MAX_CHAR_NAME_LENGTH + 1);
	CLCString	strOldBossName(MAX_CHAR_NAME_LENGTH + 1);

#ifdef MONSTER_COMBO
	bool		bEndMonsterCombo = false;
#endif // MONSTER_COMBO
	msg >> nBossIndex
		>> nTargetIndex
		>> nErrorCode
		>> nNewBossIndex
		>> strNewBossName;

	GAMELOG << init("PARTY DEBUG HELPER QUIT REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "nNewBossIndex" << delim << nNewBossIndex << delim
			<< "strNewBossName" << delim << strNewBossName
			<< end;

	CParty* pParty = gserver.FindPartyByBossIndex(nBossIndex);
	if (!pParty)
	{
		GAMELOG << init("PARTY DEBUG HELPER QUIT REP : NOT FOUND PARTY") << end;
		return ;
	}

	strOldBossName = pParty->GetMemberByListIndex(0)->GetCharName();

	CPC* pTargetPC = gserver.m_playerList.Find(nTargetIndex);
	if (pTargetPC)
	{
#ifdef MONSTER_COMBO
		// 콤보 area에 있는 pc 파티 탈퇴하면 콤보존 나감
		if(pTargetPC->m_pZone->IsComboZone())
		{
			int zone = ZONE_DRATAN;
			int extra = 0;
			
			int i;
			CZone* pZone;

			i = gserver.FindZone(zone);
			pZone = gserver.m_zones + i;
			
			GAMELOG << init("GET OUT COMBO AREA BY PARTY QUIT", pTargetPC)
				<< "AREA INDEX: " 
				<< pTargetPC->m_pArea->m_index
				<< "COMBO INDEX: "
				<< pTargetPC->m_pArea->m_monsterCombo->m_nIndex
				<< end;

			GoZone(pTargetPC, pZone->m_index,
				pZone->m_zonePos[extra][0],														// ylayer
				GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
				GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z

			bEndMonsterCombo = true;
		}
#endif // MONSTER_COMBO

#ifdef EXTREME_CUBE
		// 큐브존에 있는 PC 파티 탈퇴하면 큐브존 나감
		if(pTargetPC->m_pZone !=NULL && pTargetPC->m_pZone->IsExtremeCube())
		{
			CCubeMemList* memlist;
			memlist = gserver.m_extremeCube.FindMemList(pParty);
			if(memlist)
			{
				int zone = ZONE_MERAC;
				int extra = 0;
				
				int i;
				CZone* pZone;
				
				i = gserver.FindZone(zone);
				pZone = gserver.m_zones + i;
				
				GoZone(pTargetPC, pZone->m_index,
					pZone->m_zonePos[extra][0],														// ylayer
					GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
					GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z

			//	memlist->DelPC(pTargetPC);
			}
		}
#endif // EXTREME_CUBE
		pTargetPC->CancelDamageLink();
		pTargetPC->m_party = NULL;
	}

	pParty->SetMemberPCPtr(nTargetIndex, NULL);
	pParty->DeleteMember(nTargetIndex);

#ifdef GMTOOL
	if( pTargetPC )
	{
		if(pTargetPC->m_bGmMonitor)
		{
			pParty->m_bGmtarget = false;
		}
	}
#endif // GMTOOL

	if (nErrorCode == MSG_HELPER_PARTY_ERROR_QUIT_END || pParty->GetBossIndex() != nNewBossIndex)
	{
		// 파티 해체 알림
		PartyMsg(rmsg, MSG_PARTY_END);

		if (pTargetPC)
			SEND_Q(rmsg, pTargetPC->m_desc);

		pParty->SendToAllPC(rmsg, nTargetIndex);

		// 초대 거절
		if (pParty->GetRequestIndex() > 0)
		{
			CPC* pRequestPC = gserver.m_playerList.Find(pParty->GetRequestIndex());
			if (pRequestPC)
			{
				PartyMsg(rmsg, MSG_PARTY_REJECT_SRC);
				SEND_Q(rmsg, pRequestPC->m_desc);
			}
			pParty->SetRequest(-1, "");
		}
#ifdef MONSTER_COMBO
		if(pParty->m_comboAreaIndex != -1 && bEndMonsterCombo)
		{
			int i;
			CArea* area;
			i = gserver.m_comboZone->FindComboArea(pParty->m_comboAreaIndex);
			if(i != -1)
			{
				area = gserver.m_comboZone->m_area + i;

				GAMELOG << init("CLOSE COMBO AREA")
						<< "BY Party End"
						<< "AREA INDEX: " << area->m_index
						<< "COMBO INDEX: " << area->m_monsterCombo->m_nIndex
						<< end;

				area->CloseComboZone();
			}
		}
#endif // MONSTER_COMBO

#ifdef EXTREME_CUBE
		if(pParty->m_cubeUniqueIdx != -1)
		{
			CCubeMemList* memlist = gserver.m_extremeCube.FindMemList(pParty);
			if(memlist && memlist->IsPartyCubeMemList())
			{
				((CPartyCubeMemList*)memlist)->SetParty(NULL);
			}
		}
#endif // EXTREME_CUBE
		gserver.m_listParty.RemoveData(pParty);
		delete pParty;
	}
	else
	{
		// 파티원 나감
		PartyDelMsg(rmsg, false, nTargetIndex);

		if (pTargetPC)
			SEND_Q(rmsg, pTargetPC->m_desc);

		pParty->SendToAllPC(rmsg, nTargetIndex);

		// 보스 변경 알림
		if (nBossIndex != nNewBossIndex)
		{
#ifdef PARTY_MATCHING
			CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(nBossIndex);
			if (pMatchParty)
			{
				pMatchParty->SetBossIndex(pParty->GetBossIndex());
				pMatchParty->SetBossName(pParty->GetMemberByListIndex(0)->GetCharName());
			}
#endif // PARTY_MATCHING
			PartyChangeBoss(rmsg, strOldBossName, nNewBossIndex, strNewBossName, false);
			pParty->SendToAllPC(rmsg);
		}
	}
}

void ProcHelperPartyKickRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int			nBossIndex;
	int			nTargetIndex;
	int			nErrorCode;

#ifdef MONSTER_COMBO
	bool		bEndMonsterCombo = false;
#endif // MONSTER_COMBO

	msg >> nBossIndex
		>> nTargetIndex
		>> nErrorCode;

	GAMELOG << init("PARTY DEBUG HELPER KICK REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CParty* pParty = gserver.FindPartyByBossIndex(nBossIndex);
	if (!pParty)
	{
		GAMELOG << init("PARTY DEBUG HELPER KICK REP : NOT FOUND PARTY") << end;
		return ;
	}
	/*
#ifdef MONSTER_COMBO
	CMonsterCombo* combo = NULL;
	if(pParty->m_combo)
	{
		if(pParty->m_combo->m_area)
		{
			// 콤보존에서 게임중 파티 해체 되면 존에서 combo 삭제
			combo = pParty->m_combo;
		}
		else
		{
			// 편집 중임
			delete pParty->m_combo;
			pParty->m_combo = NULL;
			combo = NULL;
		}
	}
#endif // MONSTER_COMBO
	*/

	CPC* pTargetPC = gserver.m_playerList.Find(nTargetIndex);
	if (pTargetPC && pTargetPC->m_party && pTargetPC->m_party == pParty)
	{
#ifdef MONSTER_COMBO
		// 콤보 area에 있는 pc 파티 탈퇴하면 콤보존 나감
		if(pTargetPC->m_pZone->IsComboZone())
		{
			int zone = ZONE_DRATAN;
			int extra = 0;
			
			int i;
			i = gserver.FindZone(zone);
			
			CZone* pZone = gserver.m_zones + i;
			
			GAMELOG << init("GET OUT COMBO AREA BY PARTY KICK", pTargetPC)
				<< "AREA INDEX: " 
				<< pTargetPC->m_pArea->m_index
				<< "COMBO INDEX: "
				<< pTargetPC->m_pArea->m_monsterCombo->m_nIndex
				<< end;
			
			GoZone(pTargetPC, pZone->m_index,
				pZone->m_zonePos[extra][0],														// ylayer
				GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
				GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z

			bEndMonsterCombo = true;
		}
#endif // MONSTER_COMBO

#ifdef EXTREME_CUBE
		// 큐브존에 있는 PC 파티 탈퇴하면 큐브존 나감
		if(pTargetPC->m_pZone !=NULL && pTargetPC->m_pZone->IsExtremeCube())
		{
			CCubeMemList* memlist;
			memlist = gserver.m_extremeCube.FindMemList(pParty);
			if(memlist)
			{
				int zone = ZONE_MERAC;
				int extra = 0;
				
				int i;
				CZone* pZone;
				
				i = gserver.FindZone(zone);
				pZone = gserver.m_zones + i;
				
				GoZone(pTargetPC, pZone->m_index,
					pZone->m_zonePos[extra][0],														// ylayer
					GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
					GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z

			//	memlist->DelPC(pTargetPC);
			}
		}
#endif // EXTREME_CUBE
		
		pTargetPC->CancelDamageLink();
		pTargetPC->m_party = NULL;
	}

	pParty->SetMemberPCPtr(nTargetIndex, NULL);
	pParty->DeleteMember(nTargetIndex);

	if (nErrorCode == MSG_HELPER_PARTY_ERROR_KICK_END)
	{
		// 파티 해체 알림
		PartyMsg(rmsg, MSG_PARTY_END);

		if (pTargetPC)
			SEND_Q(rmsg, pTargetPC->m_desc);

		pParty->SendToAllPC(rmsg, nTargetIndex);

		// 초대 거절
		if (pParty->GetRequestIndex() > 0)
		{
			CPC* pRequestPC = gserver.m_playerList.Find(pParty->GetRequestIndex());
			if (pRequestPC)
			{
				PartyMsg(rmsg, MSG_PARTY_REJECT_SRC);
				SEND_Q(rmsg, pRequestPC->m_desc);
			}
			pParty->SetRequest(-1, "");
		}
#ifdef MONSTER_COMBO
		if(pParty->m_comboAreaIndex != -1 && bEndMonsterCombo)
		{
			int i;
			CArea* area;
			i = gserver.m_comboZone->FindComboArea(pParty->m_comboAreaIndex);
			if(i != -1)
			{
				area = gserver.m_comboZone->m_area + i;

				GAMELOG << init("CLOSE COMBO AREA")
						<< "BY Party End"
						<< "AREA INDEX: " << area->m_index
						<< "COMBO INDEX: " << area->m_monsterCombo->m_nIndex
						<< end;

				area->CloseComboZone();
			}
		}
#endif // MONSTER_COMBO

#ifdef EXTREME_CUBE
		if(pParty->m_cubeUniqueIdx != -1)
		{
			CCubeMemList* memlist = gserver.m_extremeCube.FindMemList(pParty);
			if(memlist && memlist->IsPartyCubeMemList())
			{
				((CPartyCubeMemList*)memlist)->SetParty(NULL);
			}
		}
#endif // EXTREME_CUBE
		gserver.m_listParty.RemoveData(pParty);
		delete pParty;
	}
	else
	{
		// 파티원 나감
		PartyDelMsg(rmsg, false, nTargetIndex);

		if (pTargetPC)
			SEND_Q(rmsg, pTargetPC->m_desc);

		pParty->SendToAllPC(rmsg, nTargetIndex);
	}
}

void ProcHelperPartyChangeBossRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nNewBossIndex;
	CLCString	strNewBossName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> nBossIndex
		>> strBossName
		>> nNewBossIndex
		>> strNewBossName;

	GAMELOG << init("PARTY DEBUG HELPER CHANGE BOSS REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nNewBossIndex" << delim << nNewBossIndex << delim
			<< "strNewBossName" << delim << strNewBossName
			<< end;

	CParty* pParty = gserver.FindPartyByBossIndex(nBossIndex);
	if (!pParty)
	{
		GAMELOG << init("PARTY DEBUG HELPER CHANGE BOSS REP : NOT FOUND PARTY") << end;
		return ;
	}

	pParty->ChangeBoss(strNewBossName);

	PartyChangeBoss(rmsg, strBossName, nNewBossIndex, strNewBossName, true);
	pParty->SendToAllPC(rmsg);

#ifdef PARTY_MATCHING
	CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(nBossIndex);
	if (pMatchParty)
	{
		pMatchParty->SetBossIndex(nNewBossIndex);
		pMatchParty->SetBossName(strNewBossName);

		if (IS_RUNNING_HELPER)
		{
			CPC* pBossPC = gserver.m_playerList.Find(nNewBossIndex);
			if (pBossPC && pBossPC->m_pZone)
			{
				HelperPartyMatchMemberChangeInfoMsg(rmsg, nNewBossIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE, "", pBossPC->m_level, pBossPC->m_pZone->m_index);
				SEND_Q(rmsg, gserver.m_helper);
			}
		}
	}
#endif // PARTY_MATCHING

#ifdef MONSTER_COMBO
	CPC* pc = gserver.m_playerList.Find(nBossIndex);
	if(pc)
		pc->m_listSaveComboData.RemoveAll();
#endif // MONSTER_COMBO
}

#ifdef NEW_UI
void ProcHelperPartyChangeTypeRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int nBossIndex;
	char cPartyType,cDiviType=MSG_DIVITYPE_EXP;
	msg >> nBossIndex
		>> cPartyType
		>> cDiviType;

	GAMELOG << init("PARTY DEBUG HELPER CHANGE TYPE REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "PartyType" << delim << cPartyType
			<< end;

	CParty* pParty = gserver.FindPartyByBossIndex(nBossIndex);
	if (!pParty)
	{
		GAMELOG << init("PARTY DEBUG HELPER CHANGE TYPE REP : NOT FOUND PARTY") << end;
		return ;
	}

	pParty->SetPartyType(cDiviType,cPartyType);

	PartyChangeType(rmsg, cPartyType, cDiviType);
	pParty->SendToAllPC(rmsg);

}
#endif // NEW_UI

#ifdef PARTY_RAID
void ProcHelperPartyEndPartyRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int nBossIndex;

	msg >> nBossIndex;

	CParty* pParty = gserver.FindPartyByBossIndex(nBossIndex);
	if (!pParty)
		return ;

	CPartyMember* pMember=NULL;
	for(int i=0; i < MAX_PARTY_MEMBER; i++)
	{
		pMember = (CPartyMember*)pParty->GetMemberByListIndex(i);

		if(pMember)
		{
			pParty->SetMemberPCPtr(pMember->GetCharIndex(), NULL);
			pParty->DeleteMember(pMember->GetCharIndex());
		}
	}

	if(pParty->GetMemberCount() <= 0)
	{
		// 파티 해체
		gserver.m_listParty.RemoveData(pParty);
		delete pParty;

		// 파티해체 알림 
		PartyMsg(rmsg, MSG_PARTY_END);
		pParty->SendToAllPC(rmsg);
	}

}
#endif // PARTY_RAID

#ifdef PARTY_MATCHING

void ProcHelperPartyMatchRegMemberRep(CNetMsg& msg)
{
	CNetMsg			rmsg;

	int				nErrorCode;
	int				nCharIndex;
	CLCString		strCharName(MAX_CHAR_NAME_LENGTH + 1);
	int				nCharLevel;
	int				nZone;
	char			nCharJob;
	char			nPartyType;
	CPC*			pPC;

	msg >> nErrorCode
		>> nCharIndex
		>> strCharName
		>> nCharLevel
		>> nZone
		>> nCharJob
		>> nPartyType;

	GAMELOG << init("PARTY DEBUG HELPER MATCH REG MEMBER REP")
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "nCharIndex" << delim << nCharIndex << delim
			<< "strCharName" << delim << strCharName << delim
			<< "nCharLevel" << delim << nCharLevel << delim
			<< "nZone" << delim << nZone << delim
			<< "nCharJob" << delim << nCharJob << delim
			<< "nPartyType" << delim << nPartyType
			<< end;

	pPC = gserver.m_playerList.Find(nCharIndex);

	switch (nErrorCode)
	{
	case MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_OK:
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_PARTY:
		if (pPC)
		{
			ExPartyMatchRegMemberRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_PARTY);
			SEND_Q(rmsg, pPC->m_desc);
#ifdef _DEBUG
			CLCString chat(1000);
			chat = "REG MEMBER REPLY: ALREADY PARTY";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, pPC->m_desc);
#endif // _DEBUG
		}
		return ;

	case MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_REG:
		if (pPC)
		{
			ExPartyMatchRegMemberRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_REG);
			SEND_Q(rmsg, pPC->m_desc);
#ifdef _DEBUG
			CLCString chat(1000);
			chat = "REG MEMBER REPLY: ALREADY REGIST";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, pPC->m_desc);
#endif // _DEBUG
		}
		return ;

	default:
		return ;
	}

	CPartyMatchMember* pMatchMember = new CPartyMatchMember(nCharIndex, strCharName, nCharLevel, nZone, nCharJob, nPartyType);
	gserver.m_listPartyMatchMember.AddToHead(pMatchMember);
	if (pPC)
	{
		ExPartyMatchRegMemberRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_OK);
		SEND_Q(rmsg, pPC->m_desc);
#ifdef _DEBUG
		CLCString chat(1000);
		chat = "REG MEMBER REPLY: OK";
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		SEND_Q(rmsg, pPC->m_desc);
#endif // _DEBUG
	}
}

void ProcHelperPartyMatchRegPartyRep(CNetMsg& msg)
{
	CNetMsg			rmsg;

	int				nErrorCode;
	int				nBossIndex;
	CLCString		strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int				nBossLevel;
	int				nZone;
	int				nJobFlag;
	char			cLimitLevel;
	char			cPartyType;
	CLCString		strComment(PARTY_MATCHING_COMMENT + 1);

	msg >> nErrorCode
		>> nBossIndex
		>> strBossName
		>> nBossLevel
		>> nZone
		>> nJobFlag
		>> cLimitLevel
		>> cPartyType
		>> strComment;

	GAMELOG << init("PARTY DEBUG HELPER MATCH REG PARTY REP")
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nBossLevel" << delim << nBossLevel << delim
			<< "nZone" << delim << nZone << delim
			<< "nJobFlag" << delim << nJobFlag << delim
			<< "cLimitLevel" << delim << cLimitLevel << delim
			<< "cPartyType" << delim << cPartyType
			<< end;

	CPC* pBossPC = gserver.m_playerList.Find(nBossIndex);

	switch (nErrorCode)
	{
	case MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_OK:
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_NO_BOSS:
		if (pBossPC)
		{
			ExPartyMatchRegPartyRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_NO_BOSS);
			SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
			CLCString chat(1000);
			chat = "REG PARTY REPLY: NO BOSS";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, pBossPC->m_desc);
#endif // _DEBUG
		}
		return ;

	case MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_ALREADY_REG:
		if (pBossPC)
		{
			ExPartyMatchRegPartyRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_ALREADY_REG);
			SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
			CLCString chat(1000);
			chat = "REG PARTY REPLY: ALREADY REGIST";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, pBossPC->m_desc);
#endif // _DEBUG
		}
		return ;

	case MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_FULL:
		if (pBossPC)
		{
			ExPartyMatchRegPartyRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_FULL);
			SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
			CLCString chat(1000);
			chat = "REG PARTY REPLY: FULL PARTY";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, pBossPC->m_desc);
#endif // _DEBUG
		}
		return ;

	default:
		return ;
	}

	CPartyMatchParty* pMatchParty = new CPartyMatchParty(nBossIndex, strBossName, nBossLevel, nZone, nJobFlag, cPartyType, (cLimitLevel) ? true : false, strComment);
	gserver.m_listPartyMatchParty.AddToHead(pMatchParty);
	if (pBossPC)
	{
		ExPartyMatchRegPartyRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_OK);
		SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
		CLCString chat(1000);
		chat = "REG PARTY REPLY: OK";
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		SEND_Q(rmsg, pBossPC->m_desc);
#endif // _DEBUG
	}
}

void ProcHelperPartyMatchDelRep(CNetMsg& msg)
{
	CNetMsg	rmsg;

	int		nCharIndex;
	int		nErrorCode;

	msg >> nCharIndex
		>> nErrorCode;

	GAMELOG << init("PARTY DEBUG HELPER MATCH DEL REP")
			<< "nCharIndex" << delim << nCharIndex << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CPartyMatchMember*	pMatchMember = NULL;
	CPartyMatchParty*	pMatchParty = NULL;

	CPC* pPC = gserver.m_playerList.Find(nCharIndex);

	switch (nErrorCode)
	{
	case MSG_HELPER_PARTY_MATCH_ERROR_DEL_MEMBER:
		pMatchMember = gserver.FindPartyMatchMemberByCharIndex(nCharIndex);
		if (pMatchMember)
		{
			gserver.m_listPartyMatchMember.RemoveData(pMatchMember);
			delete pMatchMember;
			if (pPC)
			{
				if (!pPC->m_party)
				{
					ExPartyMatchDelRepMsg(rmsg);
					SEND_Q(rmsg, pPC->m_desc);
				}
#ifdef _DEBUG
				CLCString chat(1000);
				chat = "DEL REPLY: DEL MEMBER";
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				SEND_Q(rmsg, pPC->m_desc);
#endif // _DEBUG
			}
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_DEL_PARTY:
		pMatchParty = gserver.FindPartyMatchPartyByBossIndex(nCharIndex);
		if (pMatchParty)
		{
			gserver.m_listPartyMatchParty.RemoveData(pMatchParty);
			delete pMatchParty;
			if (pPC)
			{
				if (pPC->IsParty() && pPC->m_party->GetMemberCount() < MAX_PARTY_MEMBER)
				{
					ExPartyMatchDelRepMsg(rmsg);
					SEND_Q(rmsg, pPC->m_desc);
				}
#ifdef _DEBUG
				CLCString chat(1000);
				chat = "DEL REPLY: DEL PARTY";
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				SEND_Q(rmsg, pPC->m_desc);
#endif // _DEBUG
			}
		}
		break;
	}
}

void ProcHelperPartyMatchInviteRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int			nErrorCode;
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nCharIndex;
	CLCString	strCharName(MAX_CHAR_NAME_LENGTH + 1);
	char		cPartyType;

	msg >> nErrorCode
		>> nBossIndex
		>> strBossName
		>> nCharIndex
		>> strCharName
		>> cPartyType;

	GAMELOG << init("PARTY DEBUG HELPER MATCH INVITE REP")
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nCharIndex" << delim << nCharIndex << delim
			<< "strCharName" << delim << strCharName << delim
			<< "cPartyType" << delim << cPartyType
			<< end;

	CPC* pBossPC = gserver.m_playerList.Find(nBossIndex);
	CPC* pTargetPC = gserver.m_playerList.Find(nCharIndex);
	CParty* pParty = NULL;

	switch (nErrorCode)
	{
	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_OK:
		pParty = gserver.FindPartyByBossIndex(nBossIndex);
		if (pParty)
		{
			// 기존 파티에 초대 상태로
			pParty->SetRequest(nCharIndex, strCharName);
			if (pTargetPC)
				pTargetPC->m_party = pParty;
		}
		else
		{
			// 신규 파티
			pParty = new CParty(cPartyType, nBossIndex, strBossName, pBossPC, nCharIndex, strCharName);

			if (pBossPC)		pBossPC->m_party = pParty;
			if (pTargetPC)		pTargetPC->m_party = pParty;

			gserver.m_listParty.AddToTail(pParty);
		}

		ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_OK, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
		if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
		if (pTargetPC)		SEND_Q(rmsg, pTargetPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat.Format("INVITE REP: TYPE: %d, BOSS: %d, TARGET: %d", cPartyType, nBossIndex, nCharIndex);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			if (pTargetPC)		SEND_Q(rmsg, pTargetPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_NOT_FOUND:
		ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_NOT_FOUND, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
		if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "INVITE REP: NOT FOUND TARGET";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_LEVEL:
		ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_LEVEL, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
		if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "INVITE REP: LEVEL";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_NOBOSS:
		ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_NOBOSS, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
		if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "INVITE REP: NO BOSS";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_DIFF_TYPE:
		ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_DIFF_TYPE, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
		if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "INVITE REP: DIFF TYPE";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_ALREADY_TARGET:
		ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_ALREADY_TARGET, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
		if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "INVITE REP: ALREADY TARGET";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_FULL:
		ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_FULL, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
		if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "INVITE REP: FULL";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_ALREADY_REQUEST:
		ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_ALREADY_REQUEST, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
		if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "INVITE REP: ALREADY REQUEST";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
		}
#endif // _DEBUG
		break;
	}
}

void ProcHelperPartyMatchJoinRep(CNetMsg& msg)
{
	CNetMsg		rmsg;

	int			nErrorCode;
	char		cPartyType;
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nReqCharIndex;
	CLCString	strReqCharName(MAX_CHAR_NAME_LENGTH + 1);
	char		cReqCharJob;

	msg >> nErrorCode
		>> cPartyType
		>> nBossIndex
		>> strBossName
		>> nReqCharIndex
		>> strReqCharName
		>> cReqCharJob;

	GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN REP")
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "cPartyType" << delim << cPartyType << delim
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nReqCharIndex" << delim << nReqCharIndex << delim
			<< "strReqCharName" << delim << strReqCharName << delim
			<< "cReqCharJob" << delim << cReqCharJob
			<< end;

	CPC* pBossPC = gserver.m_playerList.Find(nBossIndex);
	CPC* pReqPC = gserver.m_playerList.Find(nReqCharIndex);
	CParty* pParty = NULL;

	switch (nErrorCode)
	{
	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_OK:
		pParty = gserver.FindPartyByBossIndex(nBossIndex);
		if (pParty)
		{
			// 기존 파티의 초대 상태로
			pParty->SetRequest(nReqCharIndex, strReqCharName);
			if (pReqPC)
				pReqPC->m_party = pParty;
			ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_OK, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
			if (pBossPC)	SEND_Q(rmsg, pBossPC->m_desc);
			if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat.Format("JOIN REPLY: TYPE: %d, BOSS: %d, REQ: %d, JOB: %d", cPartyType, nBossIndex, nReqCharIndex, cReqCharJob);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
				if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
			}
#endif // _DEBUG
		}
		else
		{
			GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN REP : NOT FOUND PARTY") << end;
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALREADY_TARGET:
		ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_ALREADY_TARGET, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
		if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "JOIN REPLY: ALREADY TARGET";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_NOT_FOUND:
		ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_NOT_FOUND, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
		if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "JOIN REPLY: NOT FOUND";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_LEVEL:
		ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_LEVEL, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
		if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "JOIN REPLY: LEVEL";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_LIMIT_LEVEL:
		ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_LIMIT_LEVEL, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
		if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "JOIN REPLY: LIMIT LEVEL";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_JOB:
		ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_JOB, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
		if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "JOIN REPLY: JOB";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_FULL:
		ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_FULL, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
		if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "JOIN REPLY: FULL";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
		}
#endif // _DEBUG
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALREADY_REQUEST:
		ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_ALREADY_REQUEST, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
		if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "JOIN REPLY: ALREADY REQUEST";
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
		}
#endif // _DEBUG
		break;
	}
}

void ProcHelperPartyMatchJoinAllowRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	CNetMsg addmsg;

	int			nErrorCode;
	int			nBossIndex;
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> nErrorCode
		>> nBossIndex
		>> nTargetIndex
		>> strTargetName;

	GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN ALLOW REP")
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "strTargetName" << delim << strTargetName
			<< end;

	CPC* pTargetPC = gserver.m_playerList.Find(nTargetIndex);
	CParty* pParty = gserver.FindPartyByBossIndex(nBossIndex);
	if (pParty == NULL)
	{
		GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN ALLOW REP : NOT FOUND PARTY") << end;
		return ;
	}

	int nListIndex = pParty->JoinRequest(strTargetName, pTargetPC);
	if (nListIndex == -1)
	{
		GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN ALLOW REP : NOT FOUND REQUEST") << end;
		return ;
	}

	// 파티원들에게 결과 알리기
	int i;
	PartyAddMsg(addmsg, nTargetIndex, strTargetName, pTargetPC, ((pParty->GetBossIndex() == nTargetIndex) ? 1 : 0));
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		const CPartyMember* pMember = pParty->GetMemberByListIndex(i);
		if (pMember && i != nListIndex)
		{
			// 타인에게 target을 추가하고
			if (pMember->GetMemberPCPtr())
				SEND_Q(addmsg, pMember->GetMemberPCPtr()->m_desc)

			// target에게는 타인을 추가
			if (pTargetPC)
			{
				PartyAddMsg(rmsg, pMember->GetCharIndex(), pMember->GetCharName(), pMember->GetMemberPCPtr(), ((pParty->GetBossIndex() == pMember->GetCharIndex()) ? 1 : 0));
				SEND_Q(rmsg, pTargetPC->m_desc);
			}
		}
	}
}

void ProcHelperPartyMatchJoinRejectRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int			nErrorCode;
	int			nJoinCharIndex;
	int			nRejectCharIndex;

	msg >> nErrorCode
		>> nJoinCharIndex
		>> nRejectCharIndex;

	GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN REJECT REP")
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "nJoinCharIndex" << delim << nJoinCharIndex << delim
			<< "nRejectCharIndex" << delim << nRejectCharIndex
			<< end;

	// nJoinCharIndex로 파티 검색
	CParty* pParty = gserver.FindPartyByMemberIndex(nJoinCharIndex, true);
	if (!pParty)
	{
		GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN REJECT REP : NOT FOUND PARTY") << end;
		return ;
	}

	// 각각의 CPC 구하기
	CPC* pBossPC = pParty->GetMemberByListIndex(0)->GetMemberPCPtr();
	CPC* pRequestPC = gserver.m_playerList.Find(pParty->GetRequestIndex());

	if (nErrorCode == MSG_HELPER_PARTY_MATCH_ERROR_JOIN_REJECT_OK)
	{
		if (pParty->GetRequestIndex() == nRejectCharIndex)
			PartyMsg(rmsg, MSG_PARTY_REJECT_DEST);
		else if (pParty->GetBossIndex() == nRejectCharIndex)
			PartyMsg(rmsg, MSG_PARTY_REJECT_SRC);
		else
		{
			GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN REJECT REP : NOT FOUND CHAR") << end;
			return ;
		}

		pParty->SetMemberPCPtr(pParty->GetRequestIndex(), NULL);

		if (pBossPC)
			SEND_Q(rmsg, pBossPC->m_desc);
		if (pRequestPC)
		{
			SEND_Q(rmsg, pRequestPC->m_desc);
			pRequestPC->m_party = NULL;
		}

		pParty->SetRequest(-1, "");
		if (pParty->GetMemberCount() < 2)
		{
			// 파티 종료
#ifdef EXTREME_CUBE
		if(pParty->m_cubeUniqueIdx != -1)
		{
			CCubeMemList* memlist = gserver.m_extremeCube.FindMemList(pParty);
			if(memlist && memlist->IsPartyCubeMemList())
			{
				((CPartyCubeMemList*)memlist)->SetParty(NULL);
			}
		}
#endif // EXTREME_CUBE
			gserver.m_listParty.RemoveData(pParty);
			delete pParty;
		}
	}
}

void ProcHelperPartyMatchMemberChangeInfo(CNetMsg& msg)
{
	int			nCharIndex;
	int			nType;
	CLCString	strCharName(MAX_CHAR_NAME_LENGTH + 1);
	int			nLevel;
	int			nZone;

	msg >> nCharIndex
		>> nType;

	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(nCharIndex);
	CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(nCharIndex);

	switch (nType)
	{
	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME:
		msg >> strCharName;
		if (pMatchMember)		pMatchMember->SetName(strCharName);
		if (pMatchParty)		pMatchParty->SetBossName(strCharName);
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL:
		msg >> nLevel;
		if (pMatchMember)		pMatchMember->SetLevel(nLevel);
		if (pMatchParty)		pMatchParty->SetBossLevel(nLevel);
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
		break;
	}
}

#endif // PARTY_MATCHING

void ProcHelperPartyInfoEnd(CNetMsg& msg)
{
	// 헬퍼로부터 파티, 파티 매칭 정보를 모두 읽어 들이면 서버는 클라이언트 처리를 시작한다.
	gserver.m_bLoadPartyInfo = true;
}

void ProcHelperPartyInfoParty(CNetMsg& msg)
{
	char		cPartyType;
	int			nRequestIndex;
	CLCString	strRequestName(MAX_CHAR_NAME_LENGTH + 1);
	int			nMemberCount;
	int			nBossIndex;
	int			nCharIndex;
	CLCString	strCharName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> cPartyType
		>> nRequestIndex
		>> strRequestName
		>> nMemberCount
		>> nBossIndex
		>> strCharName;

	CParty* pParty = new CParty(cPartyType, nBossIndex, strCharName, NULL, nRequestIndex, strRequestName);

	int i;
	for (i = 1; i < nMemberCount; i++)
	{
		msg >> nCharIndex
			>> strCharName;

		pParty->m_listMember[i] = new CPartyMember(nCharIndex, strCharName, NULL);
		pParty->m_nCount++;
	}

	gserver.m_listParty.AddToTail(pParty);

	// 초대 중인 것이 있었으면 자동으로 거절 처리하도록 함
	if (nRequestIndex > 0)
	{
		CNetMsg rmsg;
		HelperPartyRejectReqMsg(rmsg, nBossIndex, nRequestIndex);
		SEND_Q(rmsg, gserver.m_helper);
	}
}

#ifdef PARTY_MATCHING

void ProchelperPartyInfoPartyMatchMember(CNetMsg& msg)
{
	int			nCharIndex;
	CLCString	strCharName(MAX_CHAR_NAME_LENGTH + 1);
	int			nLevel;
	int			nZone;
	char		cJob;
	char		cPartyType;

	msg >> nCharIndex
		>> strCharName
		>> nLevel
		>> nZone
		>> cJob
		>> cPartyType;

	CPartyMatchMember* pMatchMember = new CPartyMatchMember(nCharIndex, strCharName, nLevel, nZone, cJob, cPartyType);
	gserver.m_listPartyMatchMember.AddToHead(pMatchMember);
}

void ProchelperPartyInfoPartyMatchParty(CNetMsg& msg)
{
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nBossLevel;
	int			nZone;
	int			nJobFlag;
	char		cPartyType;
	char		cLimitLevel;
	CLCString	strComment(PARTY_MATCHING_COMMENT + 1);

	msg >> nBossIndex
		>> strBossName
		>> nBossLevel
		>> nZone
		>> nJobFlag
		>> cPartyType
		>> cLimitLevel
		>> strComment;

	CPartyMatchParty* pMatchParty = new CPartyMatchParty(nBossIndex, strBossName, nBossLevel, nZone, nJobFlag, cPartyType, (cLimitLevel) ? true : false, strComment);
	gserver.m_listPartyMatchParty.AddToHead(pMatchParty);
}

#endif // PARTY_MATCHING


#ifdef EVENT_CHILDRENSDAY_2007
void ProcHelperChildrenEvent(CNetMsg& msg)
{
	unsigned char	result = 0;
	int		nCharIndex =0;
	int		nItemIndex =0;
	int		nNeedFlowerCount = 0;
	CNetMsg rmsg;

	msg >> nCharIndex
		>> nItemIndex
		>> nNeedFlowerCount
		>> result;

	CPC* ch = gserver.m_playerList.Find(nCharIndex);
	if( ch == NULL )
		return;
	if( result == 0 )	//인증 실패이거나 캐릭터를 찾을 수 없음
	{
		EventChildrensDay2007Msg( rmsg, 2 );		// 
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 인벤에서 개나리 제거
	int nRow, nCol; 
	CItem	*pFlower = NULL;
	if (  ch->m_invenNormal.FindItem(&nRow, &nCol, 2344, 0 , 0) )		//2344 노란 개나리
	{
		pFlower = ch->m_invenNormal.GetItem(nRow, nCol);
		if( pFlower == NULL )
		{
			EventChildrensDay2007Msg( rmsg, 3 );		// UNKnown
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		// 아이템 지급 
		if ( ch->GiveItem( nItemIndex, 3, 0, 1, false) == true )		//요청한 아이템 +3지급
		{
			GAMELOG << init("EVENT_CHILDRENSDAY_2007", ch)
					<< " GIVE CHILDRENSDAY EVENT ITEM : "
					<< nItemIndex;

			EventChildrensDay2007Msg( rmsg, 0 );		// 성공
			rmsg << nItemIndex;
			SEND_Q(rmsg, ch->m_desc);
			
			// 헬퍼에 업데이트 메세지 보내기
			HelperChildrensDay2007Msg( rmsg , MSG_HELPER_CHILDRENSDAY_2007_UPDATE , ch->m_index, nItemIndex );
			SEND_Q( rmsg, gserver.m_helper );

			// 개나리 제거
			if(  nNeedFlowerCount < pFlower->Count() )
			{
				DecreaseFromInventory(ch, pFlower, nNeedFlowerCount);
				ItemUpdateMsg(rmsg, pFlower, -nNeedFlowerCount);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				ItemDeleteMsg(rmsg, pFlower);
				SEND_Q(rmsg, ch->m_desc);
				ch->RemoveItemFromQuickSlot(pFlower);
				RemoveFromInventory(ch, pFlower, true, true);
			}
		}
		else
		{
			EventChildrensDay2007Msg( rmsg, 3 );		// 아이템 지급 실패
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else
	{
		EventChildrensDay2007Msg( rmsg, 2 );		// 개나리 없음
		SEND_Q(rmsg, ch->m_desc);
	}	
}
#endif


#ifdef EVENT_2007_PARENTSDAY
void ProcHelperAccumulatePointList( CNetMsg& msg )
{
	int err;
	int charIndex;
	int count;
	
	static const int ACCUMULATE_SIZE = 3;
	int	Ranking[ACCUMULATE_SIZE];
	int Accumulatepoint[ACCUMULATE_SIZE];	
	char GuildName[ACCUMULATE_SIZE][MAX_CHAR_NAME_LENGTH+1];
	CLCString strGuildName(MAX_CHAR_NAME_LENGTH + 1);

	CNetMsg rmsg;

	msg >> err
		>> charIndex
		>> count;

	CPC* ch = gserver.m_playerList.Find(charIndex);
	if( ch == NULL )	//캐릭터를 찾을 수 없음
	{
		return;
	}
	
	if( err == MSG_PARENTSDAY_2007_SUCCESS )
	{
		for( int i=0 ; i < count; i++ )
		{
			msg >> Ranking[i]
				>> Accumulatepoint[i]
				>> strGuildName;
			strcpy( GuildName[i], strGuildName );
		}
		
		EventParentsdayAccumulateList( rmsg, (MSG_PARENTSDAY_2007_ERROR_TYPE)err, count, Ranking, Accumulatepoint, GuildName );
		SEND_Q( rmsg, ch->m_desc );
	}
	else
	{
		EventParentsdayAccumulateList( rmsg, (MSG_PARENTSDAY_2007_ERROR_TYPE)err, 0, NULL, NULL, NULL );
		SEND_Q( rmsg, ch->m_desc );
	}
}
void ProHelperAddPoint( CNetMsg& msg )
{
	int err;
	int charIndex;
	int Canationcount;
	int EntirePoint;
	
	CNetMsg rmsg;

	msg >> err
		>> charIndex
		>> Canationcount
		>> EntirePoint;

	int nRow, nCol;
	int RedCarnation = 2331;
	CItem*	pItem;
		
	CPC* ch = gserver.m_playerList.Find(charIndex);
	if( ch == NULL )	//캐릭터를 찾을 수 없음
	{
		return;
	}		
	if( err == MSG_PARENTSDAY_2007_SUCCESS )
	{
		if (!ch->m_invenNormal.FindItem(&nRow, &nCol, RedCarnation, 0, 0) )
		{
			EventParentsdayAddPoint(rmsg, MSG_ERROR_PARENTSDAY_2007_ERROR_NOITEM, 0, 0 );
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		pItem = ch->m_invenNormal.GetItem(nRow, nCol);

		// 1. delete of carnation item 
		if (pItem->Count() > Canationcount )
		{
			DecreaseFromInventory(ch, pItem, pItem->Count() );
			ItemUpdateMsg(rmsg, pItem, pItem->Count() );
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, pItem);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pItem, true, true);
		}		
		
		EventParentsdayAddPoint( rmsg, (MSG_PARENTSDAY_2007_ERROR_TYPE)err, Canationcount, EntirePoint );
		SEND_Q( rmsg, ch->m_desc );

	}
	else
	{
		EventParentsdayAddPoint( rmsg, (MSG_PARENTSDAY_2007_ERROR_TYPE)err, 0, 0 );
		SEND_Q( rmsg, ch->m_desc );
	}

}
void ProHelperExchangeTicket( CNetMsg& msg )
{
	int err;
	int charIndex;
	int GiftTicket;
		
	CNetMsg rmsg;

	msg >> err
		>> charIndex
		>> GiftTicket;

	CPC* ch = gserver.m_playerList.Find(charIndex);
	if( ch == NULL )	//캐릭터를 찾을 수 없음
	{
		return;
	}

	// 인벤토리에 최소 1개의 빈공간이 있어야 한다
	if (ch->m_invenNormal.GetSpace() < 1)
	{
		SysFullInventoryMsg(rmsg, INVENTORY_NORMAL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem* pItem = NULL;
	int ExchangeTicketIndex = 2349;
	if( err == MSG_PARENTSDAY_2007_SUCCESS )
	{
		// 1. create item -> exchange ticket
		pItem = gserver.m_itemProtoList.CreateItem( ExchangeTicketIndex, -1, 0, 0, GiftTicket );
		if( !pItem )
		{
			EventParentsdayExchangeTicket( rmsg, MSG_ERROR_PARENTSDAY_2007_ERROR_NOITEM, 0 );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
		// 2. add to inven
		if( !AddToInventory( ch, pItem, true, true ) )
		{
			delete pItem;
			pItem = NULL;
			EventParentsdayExchangeTicket( rmsg, MSG_ERROR_PARENTSDAY_2007_FULLINVEN, 0 );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}

		EventParentsdayExchangeTicket( rmsg, (MSG_PARENTSDAY_2007_ERROR_TYPE)err, GiftTicket );
		SEND_Q( rmsg, ch->m_desc );

//		// 1. delete of carnation item 
//		if (pItem->Count() > 1)
//		{
//			DecreaseFromInventory(ch, pItem, 1 );
//			ItemUpdateMsg(rmsg, pItem, -1 );
//			SEND_Q(rmsg, ch->m_desc);
///		}
//		else
//		{
//			ItemDeleteMsg(rmsg, pItem);
//			SEND_Q(rmsg, ch->m_desc);
//			RemoveFromInventory(ch, pItem, true, true);
//		}		

		// 결과 전송
		if( pItem->tab() < 0  )
		{
			CItem*	pItemPrev  = NULL;
			int nRow;
			int nCol;
			if( ch->m_invenNormal.FindItem( &nRow, &nCol, pItem->m_itemProto->m_index, pItem->m_plus, pItem->m_flag ) )
			{
				delete pItem;
				pItem = NULL;
				pItemPrev = ch->m_invenNormal.GetItem( nRow, nCol );
				if( pItemPrev )
				{
					GAMELOG << init("EVENT PARENTS DAY 2007", ch)
						<< "ITEM INDEX" << delim
						<< ExchangeTicketIndex << delim
						<< "ITEM COUNT" << delim
						<< GiftTicket << delim
						<< itemlog(pItem)
						<< end;

					ItemUpdateMsg( rmsg, pItemPrev, GiftTicket );
					SEND_Q( rmsg, ch->m_desc );
				} // if( pItemPrev )
			} // if( ch->m_invenNormal.Find( &nRow, &nCol, pItem->m_itemProto->m_index, pItem->m_plus, pItem->m_flag ) )
		}
		else
		{
			ItemAddMsg( rmsg, pItem );
			SEND_Q( rmsg, ch->m_desc );
			pItem = NULL;
		}
		
	}
	else
	{
		EventParentsdayExchangeTicket( rmsg, (MSG_PARENTSDAY_2007_ERROR_TYPE)err, 0 );
		SEND_Q( rmsg, ch->m_desc );
	}
}

void ProHelperExchangeItem( CNetMsg& msg )
{
	CNetMsg rmsg;
	int err;
	int CharIndex;
	int GiftItemIndex;
	int GiftItemCount;

	msg >> err
		>> CharIndex
		>> GiftItemIndex
		>> GiftItemCount;

	CPC* ch = gserver.m_playerList.Find( CharIndex );
	if( ch == NULL )
	{
		return;
	}

	// 인벤토리에 최소 1개의 빈공간이 있어야 한다
	if (ch->m_invenNormal.GetSpace() < 1)
	{
		SysFullInventoryMsg(rmsg, INVENTORY_NORMAL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
		int nRow;
		int nCol;
		int ExchangeTicketIndex = 2349;
		
		if( !ch->m_invenNormal.FindItem( &nRow, &nCol, ExchangeTicketIndex, 0, 0 ) )
		{
			EventParentsdayExchangeItem( rmsg, MSG_ERROR_PARENTSDAY_2007_ERROR_NOITEM );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}


	CItem* pItem = NULL;
	if( err == MSG_PARENTSDAY_2007_SUCCESS )
	{
		// 1 CreateItem
		pItem = gserver.m_itemProtoList.CreateItem( GiftItemIndex, -1, 0, 0, GiftItemCount );
		if( !pItem )
		{
			EventParentsdayExchangeItem( rmsg, MSG_ERROR_PARENTSDAY_2007_ERROR_NOITEM );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
		// 무게 검사
		if( ch->m_weight + pItem->m_itemProto->m_weight >= (ch->m_maxWeight * 3/2 ) )
		{
			SysFullInventoryMsg( rmsg, INVENTORY_NORMAL );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
		// 2 add to Inven
		// 3 add to inven  is success
		if( !AddToInventory( ch, pItem, true, true ) )
		{
			delete pItem;
			pItem = NULL;
			EventParentsdayExchangeItem( rmsg, MSG_ERROR_PARENTSDAY_2007_FULLINVEN );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
		// 4. Helper to insert query
		if( GiftItemIndex == 85 || GiftItemIndex == 2348 || GiftItemIndex == 2347 )
		{
			if( GiftItemIndex == 85 && GiftItemCount != 10 )
			{

			}
			else
			{
				CNetMsg helperMsg;
				HelperEventParentsdayExchangeItemInsert( helperMsg, CharIndex, GiftItemIndex, GiftItemCount );
				SEND_Q( helperMsg, gserver.m_helper );
			}
		}

		// 1. delete of Exchange Ticket item
		CItem* pInven = ch->m_invenNormal.GetItem( nRow, nCol );

		
		if (pInven->Count() > 1)
		{
			DecreaseFromInventory(ch, pInven, 1 );
			ItemUpdateMsg(rmsg, pInven, -1 );
			SEND_Q(rmsg, ch->m_desc);
		}
		else
		{
			ItemDeleteMsg(rmsg, pInven);
			SEND_Q(rmsg, ch->m_desc);
			RemoveFromInventory(ch, pInven, true, true);
		}		

		// 결과 전송
		if( pItem->tab() < 0  )
		{
			CItem*	pItemPrev  = NULL;
			int nRow;
			int nCol;
			if( ch->m_invenNormal.FindItem( &nRow, &nCol, pItem->m_itemProto->m_index, pItem->m_plus, pItem->m_flag ) )
			{
				delete pItem;
				pItem = NULL;
				pItemPrev = ch->m_invenNormal.GetItem( nRow, nCol );
				if( pItemPrev )
				{
					GAMELOG << init("EVENT PARENTS DAY 2007", ch)
						<< "ITEM INDEX" << delim
						<< GiftItemIndex << delim
						<< "ITEM COUNT" << delim
						<< GiftItemCount << delim
						<< itemlog(pItem)
						<< end;

					ItemUpdateMsg( rmsg, pItemPrev, GiftItemCount );
					SEND_Q( rmsg, ch->m_desc );
				} // if( pItemPrev )
			} // if( ch->m_invenNormal.Find( &nRow, &nCol, pItem->m_itemProto->m_index, pItem->m_plus, pItem->m_flag ) )
		}
		else
		{
			ItemAddMsg( rmsg, pItem );
			SEND_Q( rmsg, ch->m_desc );
			pItem = NULL;
		}		
	}
	else 
	{
		// Char Error code send
		EventParentsdayExchangeItem( rmsg, (MSG_PARENTSDAY_2007_ERROR_TYPE)err );
		SEND_Q( rmsg, ch->m_desc );
	}
}


void ProHelperParentsGetItemNotice( CNetMsg& msg )
{
	int CharIndex;
	int GiftItemIndex;
	int GiftItemCount;
	CLCString strGuildName(50);

	msg >> CharIndex
		>> GiftItemIndex
		>> GiftItemCount;

	CItemProto* pItem = NULL;
	if( GiftItemIndex != 85 && GiftItemIndex != 2348 && GiftItemIndex != 2347 )
	{
		return;
	}

	if( GiftItemIndex == 85 && GiftItemCount != 10 )
	{
		return;
	}

	pItem = gserver.m_itemProtoList.FindIndex( GiftItemIndex );
	if( pItem == NULL )
	{
		return;
	}

	CNetMsg rmsg;

	if( CharIndex == 0 )
	{
		msg >> strGuildName;
		
		EventParentsdayItemNotice( rmsg, strGuildName, pItem->m_name );
		gserver.m_playerList.SendToAll(rmsg );
		return;
	}

	CPC* pc = gserver.m_playerList.Find( CharIndex );
	if( !pc )
	{
		return;
	}

	EventParentsdayItemNotice( rmsg, pc->m_guildInfo->GetName(), pItem->m_name );
	gserver.m_playerList.SendToAll(rmsg );

}
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_TEACH_2007
void ProcHelperTeach2007Addflower(CNetMsg & msg)
{
	int charindex = 0;
	msg >> charindex;

	CPC* pc = gserver.m_playerList.Find(charindex);
	if (pc)
	{
		// 노란 카네이션 지급
		if (pc->GiveItem(2330, 0, 0, 1, true) == true)
		{
			GAMELOG << init("TEACH_EVENT_ITEM_GIVE", pc)
					<< " GIVE YELLOW FLOWER";					
			if (IS_RUNNING_HELPER)
			{
				CNetMsg rmsg;
				HelperReciveFlowerMsg(rmsg, charindex);
				SEND_Q(rmsg, gserver.m_helper);
			}
		}		
	}
}
#endif // EVENT_TEACH_2007

#ifdef EVENT_FLOWERTREE_2007
void ProcHelperFlowerTreeEvent( CNetMsg& msg )
{
	unsigned int	point = 0;
	unsigned char	subtype = 0;
	int				recvTicketCount = 0;
	int	nCharIndex = 0;

	CNetMsg rmsg;
	
	msg >> subtype;
	msg >> nCharIndex;

	CPC* ch = gserver.m_playerList.Find(nCharIndex);
	if( subtype != MSG_HELPER_FLOWERTREE_2007_PRICEMSG && ch == NULL )	// 해당유저 찾을 수 없음
		return;

	switch( subtype )
	{
	case MSG_HELPER_FLOWERTREE_2007_MYPOINT :
		{
			msg >> point;
			msg >> recvTicketCount;

			EventFlowerTree2007Msg( rmsg, MSG_EVENT_FLOWERTREE_2007_MY_POINT );
			rmsg << point;
			rmsg << recvTicketCount;
			SEND_Q( rmsg, ch->m_desc );
		} break;

	case MSG_HELPER_FLOWERTREE_2007_ADDMYPOINT :
		{
			msg >> point;

			EventFlowerTree2007Msg( rmsg, MSG_EVENT_FLOWERTREE_2007_GROWUP );
			rmsg << point;
			SEND_Q( rmsg, ch->m_desc );
			break;
		}

	case MSG_HELPER_FLOWERTREE_2007_RECV_TICKET:
		{
			int count;
			msg >> count;

			if ( count && ch->GiveItem( 2351, 0, 0, count, false ) == true )		//교환권 지급
			{
				// HELPER 에 업데이트 메세지
				HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_TICKET_UPDATE, ch->m_index );
				rmsg << count;
				SEND_Q( rmsg, gserver.m_helper );
			}
			else
			{
				count = 0;
			}

			EventFlowerTree2007Msg( rmsg, MSG_EVENT_FLOWERTREE_2007_RECV_TICKET );
			rmsg << count;
			SEND_Q( rmsg, ch->m_desc );
		} break;

	case MSG_HELPER_FLOWERTREE_2007_PRICEITEM :
		{
			int nItemIndex = 0, count = 0;
			int nRow, nCol;
			CItem* pTicket = NULL;
		
			msg >> nItemIndex;			// 아이템 지급 
			msg >> count;

			// 교환권 확인
			if (  ch->m_invenNormal.FindItem(&nRow, &nCol, 2351, 0 , 0) )		// 교환권
			{
				pTicket = ch->m_invenNormal.GetItem(nRow, nCol);			
				if( pTicket == NULL )		//2349 교환권 찾을 수 없다.
				{
					EventFlowerTree2007Msg( rmsg, MSG_EVENT_FLOWERTREE_2007_OPEN_TICKET );
					rmsg << (unsigned char) 0 ;
					SEND_Q( rmsg, ch->m_desc );
				}
			}
			else		// 교환권 없음
			{
				EventFlowerTree2007Msg( rmsg, MSG_EVENT_FLOWERTREE_2007_OPEN_TICKET );
				rmsg << (unsigned char) 0 ;
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			// 아이템 지급 
			if ( ch->GiveItem( nItemIndex, 0, 0, count, false ) == true )		//요청한 아이템 지급
			{
				GAMELOG << init("EVENT_FLOWERTREE_2007", ch)
						<< " GIVE FLOWERTREE EVENT ITEM : "
						<< nItemIndex;
					
				if( pTicket->Count() > 1 )										//교환권 제거
				{
					DecreaseFromInventory(ch, pTicket, 1);
					ItemUpdateMsg(rmsg, pTicket, -1);
					SEND_Q(rmsg, ch->m_desc);
				}
				else
				{
					ItemDeleteMsg(rmsg, pTicket);
					SEND_Q(rmsg, ch->m_desc);
					ch->RemoveItemFromQuickSlot(pTicket);
					RemoveFromInventory(ch, pTicket, true, true);
				}

				// HELPER 에 업데이트 메세지
				if( nItemIndex == 85 || nItemIndex ==845 || nItemIndex == 857 )
				{
					HelperFlowerTree2007Msg( rmsg, MSG_HELPER_FLOWERTREE_2007_PRICEITEM_INSERT, ch->m_index );
					rmsg << (unsigned char) 1;
					rmsg << ch->GetName();
					rmsg << nItemIndex;
					SEND_Q( rmsg, gserver.m_helper );
				}
			}
	

			EventFlowerTree2007Msg( rmsg, MSG_EVENT_FLOWERTREE_2007_OPEN_TICKET );
			rmsg << (unsigned char) 1 ;
			SEND_Q( rmsg, ch->m_desc );
			
		} break;

	case MSG_HELPER_FLOWERTREE_2007_PRICEMSG:
		{
			int nItemIndex = 0;
			CLCString strName(256);
			msg >> strName;
			msg >> nItemIndex;
			// 모든 유저에게 당첨 메세지 뿌리기
			EventFlowerTree2007Msg( rmsg, MSG_EVENT_FLOWERTREE_2007_PRICE_MSG );
			rmsg << strName;
			rmsg << nItemIndex;
			gserver.m_playerList.SendToAll( rmsg );
		
		}break;
	}
}
#endif //EVENT_FLOWERTREE_2007

#ifdef NEW_GUILD
void ProcHelperGuildInclineEstablish( CNetMsg& msg )
{
	CNetMsg	rmsg;

	int		guildindex;
	int		charindex;
	char	guildincline;
	int		errorcode;

	msg >> guildindex
		>> charindex
		>> guildincline
		>> errorcode;

	LONGLONG needMoney;
	int needGP;
	msg >> needMoney
		>> needGP;

	CPC* pc = gserver.m_playerList.Find( charindex );
	CGuild* guild = gserver.m_guildlist.findguild( guildindex );

	if( !pc || !guild )
	{
		return;
	}

	if( guild )
	{
		if( errorcode == MSG_NEW_GUILD_ERROR_INCLINE_OK )
		{
			guild->incline( guildincline );
			guild->AddGuildPoint( -needGP );
		
			if (pc->m_moneyItem && pc->m_moneyItem->Count() > needMoney)
			{
				pc->AddMoney(-needMoney);
				ItemUpdateMsg(rmsg, pc->m_moneyItem, -needMoney);
				SEND_Q(rmsg, pc->m_desc);
			}
			else
			{
				ItemDeleteMsg(rmsg, pc->m_moneyItem);
				SEND_Q(rmsg, pc->m_desc);
				pc->AddMoney(-needMoney);
			}
		
		
			pc->CalcStatus(true);
			pc->SaveNow(false);

			GuildInclineEstablishMsg( rmsg, guildincline );
			guild->SendToAll( rmsg );
		}
	}

//	pc->m_desc->WaitHelperReply(false);

}
void ProcHelperGuildMemberAdjust( CNetMsg& msg )
{
	CNetMsg rmsg;

	int guildindex;
	int ownerindexindex;
	int charindex;
	CLCString strPositionName(GUILD_POSITION_NAME+1);
	int contributeExp;
	int contributeFame;
	int errorcode;

	msg >> guildindex
		>> ownerindexindex
		>> charindex
		>> strPositionName
		>> contributeExp
		>> contributeFame
		>> errorcode;

	CPC* guildOwner = gserver.m_playerList.Find( ownerindexindex );
//	CPC* guildMember = gserver.m_playerList.Find( charindex );
	CGuildMember* guildMember = gserver.m_guildlist.findmember( charindex);
	

	CGuild* guild = gserver.m_guildlist.findguild( guildindex );

	if( !guild )
		return;

	if( errorcode == MSG_NEW_GUILD_ERROR_ADJUST_OK && guildMember )
	{
		
	//	guildMember->m_guildInfo->positionName( strPositionName );
	//	guildMember->m_guildInfo->contributeExp( contributeExp );
	//	guildMember->m_guildInfo->contributeFame( contributeFame );
		guildMember->positionName( strPositionName );
		guildMember->contributeExp( contributeExp );
		guildMember->contributeFame( contributeFame );
	}
	if( guildOwner )
	{
		GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )errorcode );
		SEND_Q( rmsg, guildOwner->m_desc );
//		guildOwner->m_desc->WaitHelperReply(false);
	}
}

void ProcHelperGuildInfoNotice( CNetMsg& msg )
{
	CNetMsg rmsg;

	int charindex;
	int guildindex;
	int avelevel;
	int usepoint;
	int errorcode;

	msg >> charindex
		>> guildindex
		>> avelevel
		>> usepoint
		>> errorcode;

	CPC* pc = gserver.m_playerList.Find( charindex );
	CGuild* guild = gserver.m_guildlist.findguild( guildindex );

	if( !pc || !guild )
		return;

	if( errorcode == MSG_NEW_GUILD_INFO_ERROR_OK )
	{
		if( guild )
		{
			GuildNewInfo( rmsg, pc, avelevel, guild->GetGuildPoint(), usepoint );
			SEND_Q( rmsg, pc->m_desc );
		}
	}
	else
	{
		GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )errorcode );
		SEND_Q( rmsg, pc->m_desc );
	}
//	pc->m_desc->WaitHelperReply(false);

}

void ProcHelperNewGuildMemberListRep( CNetMsg& msg )
{
	
	CNetMsg rmsg;

	int charindex;
	int guildindex;
	int errorcode;
	int membercount;
	int membercharindex[GUILD_MAX_MEMBER];
	int cumulatePoint[GUILD_MAX_MEMBER];
	char CharName[GUILD_MAX_MEMBER][MAX_CHAR_NAME_LENGTH];
	char PositionName[GUILD_MAX_MEMBER][GUILD_POSITION_NAME+1];
	CLCString strCharName( MAX_CHAR_NAME_LENGTH );
	CLCString strPositionName( GUILD_POSITION_NAME +1);
	char job[GUILD_MAX_MEMBER];
	char job2[GUILD_MAX_MEMBER];
	int level[GUILD_MAX_MEMBER];
	int position[GUILD_MAX_MEMBER];

	msg >> charindex
		>> guildindex
		>> errorcode
		>> membercount;
	CPC* pc = gserver.m_playerList.Find( charindex );
	CGuild* guild = gserver.m_guildlist.findguild( guildindex );
	if( !pc || !guild )
		return;

	if( errorcode == MSG_NEW_GUILD_MEMBERLIST_ERROR_OK )
	{
		for( int i = 0; i < membercount; i++ )
		{
			msg >> membercharindex[i]
				>> cumulatePoint[i]
				>> strCharName
				>> strPositionName
				>> job[i]
				>> job2[i]
				>> level[i]
				>> position[i];
			strcpy( CharName[i], strCharName );
			strcpy( PositionName[i], strPositionName );
		}
		
		GuildMemberListRep( rmsg, membercount, membercharindex, cumulatePoint, CharName, PositionName, job, job2, level, position, guild );
		SEND_Q( rmsg, pc->m_desc );
	}
	else
	{
		GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )errorcode );
		SEND_Q( rmsg, pc->m_desc );
	}

}

void ProcHelperNewGuilManageRep( CNetMsg& msg )
{
	CNetMsg rmsg;

	int charindex;
	int guildindex;
	int errorcode;
	int membercount;
	int membercharindex[GUILD_MAX_MEMBER];
	int contributeExp[GUILD_MAX_MEMBER];
	int contributeFame[GUILD_MAX_MEMBER];
	
	int level[GUILD_MAX_MEMBER];
	char charName[GUILD_MAX_MEMBER][MAX_CHAR_NAME_LENGTH];
	char positionName[GUILD_MAX_MEMBER][GUILD_POSITION_NAME+1];
	CLCString strCharName( MAX_CHAR_NAME_LENGTH );
	CLCString strPositionName( GUILD_POSITION_NAME+1 );
	int position[GUILD_MAX_MEMBER];
	char first;

	msg >> first
		>> charindex
		>> guildindex
		>> errorcode
		>> membercount;

	CPC* pc = gserver.m_playerList.Find( charindex );
	CGuild* guild = gserver.m_guildlist.findguild( guildindex );
	if( !pc || !guild )
		return;

	if( errorcode == MSG_NEW_GUILD_MANAGE_ERROR_OK )
	{
		for( int i = 0; i < membercount; i++ )
		{
			msg >> membercharindex[i]
				>> contributeExp[i]
				>> contributeFame[i]
				>> strCharName
				>> strPositionName
				>> level[i]
				>> position[i];
			strcpy( charName[i], strCharName );
			strcpy( positionName[i], strPositionName );
		}
		
		GuildNewManageRep( rmsg, membercount, membercharindex, contributeExp, contributeFame, charName, positionName, level, position, guild, first );
		SEND_Q( rmsg, pc->m_desc );
	}
	else
	{
		GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )errorcode );
		SEND_Q( rmsg, pc->m_desc );
	}

}
void ProcHelperNewGuildNotifyRep( CNetMsg& msg )
{
	CNetMsg rmsg;

	int charindex;
	int guildindex;


	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );
	

	msg >> charindex
		>> guildindex
		>> title
		>> text;
	CPC* pc = gserver.m_playerList.Find( charindex );
	CGuild* guild = gserver.m_guildlist.findguild( guildindex );
	if( !pc || !guild )
		return;

	GuildNewNotify( rmsg, title, text );
	SEND_Q( rmsg, pc->m_desc );

}

void ProcHelperNewGuildNotifyUpdateReP( CNetMsg& msg )
{
	CNetMsg rmsg;

	int charindex;
	int guildindex;
	int errorcode;

	msg >> charindex
		>> guildindex
		>> errorcode;

	CPC* pc = gserver.m_playerList.Find( charindex );
	CGuild* guild = gserver.m_guildlist.findguild( guildindex );

	if( !pc || !guild )
		return;

	GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )errorcode );
	SEND_Q( rmsg, pc->m_desc );	

}

void ProcHelperNewGuildNotifyTrans( CNetMsg& msg )
{
	CNetMsg rmsg;

	int	guildindex;
	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );

	msg >> guildindex
		>> title
		>> text;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	GuildNewNotifyTrans(rmsg, guild->name(), title, text);
	guild->SendToAll(rmsg);


}

void ProcHelperNewGuildNotifyTransRepMsg( CNetMsg& msg )
{
	CNetMsg rmsg;

	int	guildindex;
	int charindex;
	int error;

	msg >> guildindex
		>> charindex
		>> error;


	CPC* pc = gserver.m_playerList.Find( charindex );
	CGuild* guild = gserver.m_guildlist.findguild( guildindex );

	if( !pc || !guild )
		return;

	GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )error );
	SEND_Q( rmsg, pc->m_desc );	

}

void ProcHelperNewGuildSkillListRepMsg( CNetMsg& msg )
{
	CNetMsg rmsg;

	int	charindex;
	int guildindex;
	int error;
	int skillcount;
	int skillIndex[256];
	int skillLevel[256];

	msg >> charindex
		>> guildindex
		>> error
		>> skillcount;
	int i;
	for( i = 0; i < skillcount; i++ )
	{
		msg >> skillIndex[i]
			>> skillLevel[i];
	}


	CPC* pc = gserver.m_playerList.Find( charindex );
	CGuild* guild = gserver.m_guildlist.findguild( guildindex );

	if( !pc || !guild )
		return;
	if( error == MSG_NEW_GUILD_SKILL_ERROR_OK )
	{
		int skillLearnLevel[256] = { 0, };
		for( i = 0 ; i < skillcount; i++ )
		{
			CSkill* skill = guild->m_passiveSkillList.Find( skillIndex[i] );
			if (!skill)
			{
				GAMELOG << init("NEW_GUILD_SKILL_LIST", pc)
						<< "guild " << delim << guildindex << delim
						<< "error " << delim << error << delim
						<< "skill count " << delim << skillcount << delim
					    << " skill index " << delim << skillIndex[i] << end;

				return;
			}

			if( skill->m_proto->m_index == skillIndex[i] )
			{
				skillLearnLevel[i] = skill->m_level;
			}
			
		}
		GuildSkillListRepMsg( rmsg, skillcount, skillIndex, skillLearnLevel );
		SEND_Q( rmsg, pc->m_desc );
	}

}

void ProcHelperNewGuildLoadNTF( CNetMsg& msg )
{
	int guildindex;
	int guildpoint;
	char guildincline;
	int guildmaxmeber;
	int landcount;
	int land[256];
	int skillcount;
	int skillIndex[256];
	int skillLevel[256];
	memset( land, -1, sizeof( land) );
	
	msg >> guildindex
		>> guildpoint
		>> guildincline
		>> guildmaxmeber
		>> landcount;
	if( landcount != 0 )
	{
		for( int i = 0 ; i < landcount; i++ )
		{
			msg >> land[i];
		}
	}
	else
		msg >> land[0];

	msg >> skillcount;
	int i;
	for( i = 0; i < skillcount; i++ )
	{
		msg >> skillIndex[i]
			>> skillLevel[i];
	}

	CGuild* guild = gserver.m_guildlist.findguild( guildindex );

	if( !guild )
		return;

	guild->guildPoint( guildpoint );
	guild->incline( guildincline );
	guild->landcount( landcount );
	guild->setMaxmember( guildmaxmeber );
	guild->land( landcount, land );

	if( skillcount > 0 )
	{
		for( i = 0; i < skillcount; i++ )
		{
			CSkill* skill = gserver.m_skillProtoList.Create( skillIndex[i], skillLevel[i] );
			if( skill )
			{
				guild->m_passiveSkillList.Add( skill );
			}
		}

	}


}

void ProcHelperNewGuildMemberList( CNetMsg& msg )
{
	int guildindex;
	int guildmembercount;
	int charindex;
	int contributeExp;
	int contributeFame;
	int cumulatePoint;
	int channel;
	int zoneNum;

	CLCString positionName( GUILD_POSITION_NAME + 1 );
	msg >> guildindex
		>> guildmembercount;

	CGuild* guild = gserver.m_guildlist.findguild( guildindex );
	if( !guild )
		return;

	int i;
	for( i = 0; i < guildmembercount; i++ )
	{ 
		msg >> charindex
			>> contributeExp
			>> contributeFame
			>> cumulatePoint
			>> positionName
			>> channel
			>> zoneNum;
		CGuildMember* member = guild->findmember( charindex );
		if( !member )
			continue;
//		CPC* pc = gserver.m_playerList.Find( member->charindex() );
//		if( pc )
//		{
			member->contributeExp( contributeExp );
			member->contributeFame( contributeFame );
			member->cumulatePoint( cumulatePoint );
			member->positionName( positionName );
			member->channel( channel );
			member->zoneindex( zoneNum );

//			GAMELOG << "ProcHelperNewGuildMemberList()" << delim
//			<< "CharIndex :"<< delim << charindex << delim
//			<< "ZoneIndex :" << delim << zoneNum << delim
//			<< end;
// 		}
	}
}

void ProcHelperNewGuildInfoRepMsg( CNetMsg& msg )
{
	CNetMsg rmsg;
	int charindex;
	int errorcode;

	msg >> charindex
		>> errorcode;
		
	CPC* pc = gserver.m_playerList.Find( charindex );
	
	if( !pc )
		return;
	GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )errorcode );
	SEND_Q( rmsg, pc->m_desc );	

}

void ProcHelperNewGuildPointUpdateMsg( CNetMsg& msg )
{
	int charindex;
	int guildindex;
	int guildpoint;
	msg >> charindex
		>> guildindex
		>> guildpoint;
	CPC* pc = gserver.m_playerList.Find( charindex );
	CGuild* guild = gserver.m_guildlist.findguild( guildindex );
	if( !pc && !guild )
		return;	

	guild->guildPoint( guildpoint );

}

void ProcHelperNewGuildNotice( CNetMsg& msg )
{
	CNetMsg rmsg;

	int charindex;
	int	guildindex;
	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );

	msg >> charindex
		>> guildindex
		>> title
		>> text;

	CPC* pc = gserver.m_playerList.Find( charindex );
	if( !pc )
		return;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	if( strlen(title) != 0 && strlen(text) != 0 )	
	{
		GuildNewNotifyTrans(rmsg, guild->name(), title, text);
		SEND_Q( rmsg, pc->m_desc );	
	}

}

void ProcHelperNewGuildMemberPointSaveMsg(CNetMsg& msg)
{
	int nCharIdx;
	int nGuildIdx;
	int nMemberPoint;

	msg >> nGuildIdx
		>> nCharIdx
		>> nMemberPoint;

	CPC* pPC = gserver.m_playerList.Find(nCharIdx);
	CGuildMember* pMember = gserver.m_guildlist.findmember(nCharIdx);
	if(!pPC || !pMember)
		return;	

	pMember->cumulatePoint(nMemberPoint);
}
#endif // NEW_GUILD

#ifdef EVENT_OPEN_ADULT_SERVER
void ProcHelperOpenAdultServerRep( CNetMsg& msg)
{
	CNetMsg rmsg;
	int charindex;
	int errcode;
	CPC* pc;

	msg >> charindex
		>> errcode;

	switch(errcode)
	{
	case MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_OK:
		{
			try
			{
				pc = gserver.m_playerList.Find( charindex );
				if(!pc)
					throw MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_FAILED;
				
				int rank, giftcount;
				int giftindex = 2382;			// 절망의 부적
				msg >> rank;
				
				if(rank > 100)		giftcount = 50;
				else if(rank > 10)	giftcount = 100;
				else if(rank > 1)	giftcount = 300;
				else if(rank > 0)	giftcount = 500;
				else				throw MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_FAILED;
				
				CItem* giftitem = gserver.m_itemProtoList.CreateItem(giftindex, -1, 0 , 0, giftcount);
				if(!giftitem)
					throw MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_FAILED;

				if(AddToInventory(pc, giftitem, true, true))
				{
					CNetMsg itemmsg;
					if(giftitem->tab() != -1)
					{
						ItemAddMsg(itemmsg, giftitem);
					}
					else
					{
						int row, col;
						if(pc->m_invenNormal.FindItem(&row, &col, giftindex, -1, -1))
						{
							CItem* pItem = pc->m_invenNormal.GetItem(row, col);
							if(pItem)
							{
								ItemUpdateMsg(itemmsg, pItem, giftcount);
								delete giftitem;
								giftitem = NULL;
							}
						}
					}
					SEND_Q(itemmsg, pc->m_desc);
					
					EventOpenAdultServerMsg(rmsg, MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_EXCHANGE_OK);
					rmsg << rank
						 << giftcount;
					SEND_Q(rmsg, pc->m_desc);

					GAMELOG << init("EVENT OPEN ADULT SERVER", pc)
							<< "CHANGEJOB REWARD" << delim
							<< "RANK" << rank << delim
							<< "GIFT ITEM INDEX" << giftindex << delim
							<< "GIFT ITEM COUNT" << giftcount
							<< end;
				}
				else
				{
					EventOpenAdultServerMsg(rmsg, MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_ERROR_FULLINVEN);
					SEND_Q(rmsg, pc->m_desc);

					throw MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_FAILED;
				}
			}
			catch(MSG_HELPER_OPEN_ADULT_SERVER_ERROR_TYPE err)
			{
				CNetMsg rmsg;
				HelerOpenAdultServerMsg( rmsg, charindex, err);
				SEND_Q(rmsg, gserver.m_helper);
			}
		}
		break;
	case MSG_HELPER_OPEN_ADULT_SERVER_CHANGEJOB_ALREADY:
		{
			pc = gserver.m_playerList.Find( charindex );
			if(pc)
			{
				EventOpenAdultServerMsg(rmsg, MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_ERROR_ALREADY);
				SEND_Q(rmsg, pc->m_desc);
			}
		}
		break;
	}
}
#endif // EVENT_OPEN_ADULT_SERVER

#ifdef PET_NAME_CHANGE
void ProcHelperPetChangeName( CNetMsg& msg )
{
	int err;
	int charidx;
	int petidx;
	
	CLCString PetName( MAX_CHAR_NAME_LENGTH + 1 );

	CNetMsg rmsg;
	CPC* pc = NULL;

	msg >> err
		>> charidx
		>> petidx
		>> PetName;
	pc = gserver.m_playerList.Find( charidx );
	
	if( err == MSG_EX_PET_CHANGE_NAME_ERROR_OK )
	{
		if( pc == NULL )
		{
			GAMELOG << init( "CASH_ITEM_USE_ERROR-notPC" )
					<< charidx << delim
					<< 2360 << end;
			// Helper로 롤백 처리
			PetName = "";
			HelperPetNameChange( rmsg, charidx, petidx, PetName );
			SEND_Q( rmsg, gserver.m_helper );
/*
			PetNameChange( rmsg, (MSG_EX_PET_CHANGE_NAME_ERROR_TYPE)err, petidx, PetName );
			SEND_Q( rmsg, pc->m_desc );
*/
			return;
		}
		else
		{
			int r, c;
			if( !pc->m_invenNormal.FindItem(  &r, &c, 2360, 0, 0 ) )
			{
				GAMELOG << init( "CASH_ITEM_USE_ERROR-notPC" )
						<< charidx << delim
						<< 2360 << end;
				// Helper로 롤백 처리
				PetName = "";
				HelperPetNameChange( rmsg, charidx, petidx, PetName );
				SEND_Q( rmsg, gserver.m_helper );

				PetNameChange( rmsg, (MSG_EX_PET_CHANGE_NAME_ERROR_TYPE)err, petidx, PetName );
				SEND_Q( rmsg, pc->m_desc );
				return;

			}
			CItem* pItem = pc->m_invenNormal.GetItem( r, c );
			CPet* pet = pc->GetPet( petidx );
			
			if( (pItem->m_itemProto->m_flag & ITEM_FLAG_CASH ) )
			{
				GAMELOG << init( "CASH_ITEM_USE", pc )
						<< itemlog( pItem ) << delim 
						<< pet->m_name << delim << end;
			}

			//Pet 이름 변경
			pet->m_name = PetName;
			PetNameChange( rmsg, (MSG_EX_PET_CHANGE_NAME_ERROR_TYPE)err, petidx, PetName );
			SEND_Q( rmsg, pc->m_desc );

			pet->m_pArea->SendToCell( rmsg, pet, true );
			// item 수량 변경
			ItemUseMsg( rmsg, pc, pItem->tab(), pItem->row(), pItem->col(), pItem->m_itemProto->m_index, 0 );
			SEND_Q( rmsg, pc->m_desc );
			DecreaseFromInventory( pc, pItem, 1 );
			if( pItem->Count() > 0 )
			{
				ItemUpdateMsg( rmsg, pItem, -1 );
				SEND_Q( rmsg, pc->m_desc );
			}
			else
			{
				ItemDeleteMsg( rmsg, pItem );
				SEND_Q( rmsg, pc->m_desc );
				RemoveFromInventory( pc, pItem, true, true );
			}
			return;
		}

	}
	else
	{
		if( pc )
		{
			PetNameChange( rmsg, (MSG_EX_PET_CHANGE_NAME_ERROR_TYPE)err, petidx, PetName );
			SEND_Q( rmsg, pc->m_desc );
			return;
		}

	}
	
}
#endif // PET_NAME_CHANGE

#ifdef REWARD_IDC2007
void ProcHelperRewardIDC2007( CNetMsg& msg )
{
	CNetMsg rmsg;
	CPC* pc;
	CDescriptor* desc;
	int userindex;

	static const int reward[3][2] = {
		{837, 5},			// 참외 5개
		{838, 5},			// 자두 5개
		{836, 5},			// 수박 5개
	};

	msg >> userindex;

	desc = gserver.FindUser(userindex);
	if(!desc)
		return ;
	pc = desc->m_pChar;
	if(pc)
	{
		int i;
		CItem* item;
		for(i = 0; i < 3; ++i)
		{
			item = gserver.m_itemProtoList.CreateItem(reward[i][0], -1, 0, 0, reward[i][1]);
			if(item)
			{
				if(AddToInventory(pc, item, false, true))
				{
					GAMELOG << init("REWARD IDC2007 OK", pc);
					if(item->tab() < 0)
					{
						int row, col;
						if(pc->m_invenNormal.FindItem(&row, &col, item->m_itemProto->m_index, 0, 0))
						{
							CItem* prevItem;
							prevItem = pc->m_invenNormal.GetItem(row, col);
							ItemUpdateMsg(rmsg, prevItem, reward[i][1]);
							SEND_Q(rmsg, pc->m_desc);

							delete item;
							item = NULL;

							GAMELOG << "user index" << delim 
									<< userindex << delim
									<< "ITEM INDEX" << delim
									<< reward[i][0] << delim
									<< "ADD COUNT" << delim
									<< reward[i][1] << delim
									<< itemlog(prevItem)
									<< end;
						}
					}
					else
					{
						ItemAddMsg(rmsg, item);
						SEND_Q( rmsg, pc->m_desc );

						GAMELOG << "user index" << delim
								<< userindex << delim
								<< itemlog(item)
								<< end;
					}
				}
				else
				{
					GAMELOG << init("REWARD IDC2007 FAILED", pc) << delim
							<< "user index" << userindex << delim
							<< itemlog(item)
							<< end;
					break;
				}
			}
		}
	}
}
#endif // REWARD_IDC2007


#ifdef EVENT_HALLOWEEN_2007
void ProcHelperHalloween2007( CNetMsg& msg )
{
	CNetMsg rmsg;

	int charIndex;
	unsigned char error;

	int nPriceItem[6] = { 2464, 2465, 2466, 2467, 2468, 2469 };
	// JOB_TITAN 0, JOB_KNIGHT 1, JOB_HEALER 2, JOB_MAGE 3, JOB_ROGUE 4, JOB_SORCERER 5

	msg >> charIndex >> error ;

	CPC* ch = gserver.m_playerList.Find(charIndex);
	if( ch == NULL )
		return; // 해당유저 없음

	if( error == 0 ) // 아이템 지급
	{
		int nRow, nCol;
		CItem* pPriceItem = gserver.m_itemProtoList.CreateItem( nPriceItem[ch->m_job] , -1, 0, 0, 1 );
		if( !pPriceItem )
		{
			EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}

		// 아이템 지급
		if( AddToInventory( ch, pPriceItem, true, true ) )
		{
			if( pPriceItem->tab() < 0 )
			{
				delete pPriceItem;
				ch->m_invenNormal.FindItem( &nRow, &nCol, nPriceItem[ch->m_job], 0, 0 );
				pPriceItem =  ch->m_invenNormal.GetItem( nRow, nCol );
				ItemUpdateMsg( rmsg, pPriceItem, nPriceItem[ch->m_job] );
				SEND_Q( rmsg, ch->m_desc );
			}
			else
			{
				ItemAddMsg( rmsg, pPriceItem );
				SEND_Q( rmsg, ch->m_desc );
			}
		}
		else
		{
			EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN ); // FULL INVEN
			SEND_Q( rmsg, ch->m_desc );
			return;
		}

		EventHalloween2007Msg( rmsg, MSG_EVENT_TAKE_DEVILHEAIR, 0 ); // Sucess
		SEND_Q( rmsg, ch->m_desc );
	}
	else	// 이미 받은적 있음
	{
		EventHalloween2007Msg( rmsg, MSG_EVENT_TAKE_DEVILHEAIR, 1 ); 
		SEND_Q( rmsg, ch->m_desc );
	}

}
#endif // EVENT_HALLOWEEN_2007

#ifdef DRATAN_CASTLE
#ifdef DYNAMIC_DUNGEON
void ProcHelperDVDRateChange( CNetMsg & msg )
{
	unsigned char	subtype;
	int				nRate;
	
	msg >> subtype >> nRate;

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if(pCastle != NULL && pCastle->m_dvd.GetZone() != NULL )
	{
		switch( subtype )
		{
		case MSG_MANAGEMENT_TAX_CHANGE :
			pCastle->m_dvd.SetFeeRate(nRate);
			break;
		case MSG_MANAGEMENT_HUNTER_TAX_CHANGE:
			pCastle->m_dvd.SetHuntRate(nRate);
			break;
		case MSG_MANAGEMENT_ENV_CHANGE:
			pCastle->m_dvd.SetEnvRate(nRate);
			pCastle->m_dvd.SetChangeTime(gserver.m_pulse);
			pCastle->m_dvd.ChangeSetting();
			break;
		case MSG_MANAGEMENT_STATE_CHANGE:
			pCastle->m_dvd.SetMobRate(nRate);
			pCastle->m_dvd.SetChangeTime(gserver.m_pulse);
			pCastle->m_dvd.ChangeSetting();
			break;
		}
	}
}
#endif //DYNAMIC_DUNGEON
#endif //DRATAN_CASTLE

#ifdef ALTERNATE_MERCHANT
void ProcHelperAlternateMerchant( CNetMsg& msg )
{
	CNetMsg rmsg;
	int nErrorCode;
	int nCharIndex;

	msg >> nErrorCode
		>> nCharIndex;

	CPC* pCh = gserver.m_playerList.Find(nCharIndex);

	if( !pCh )
	{
		return;
	}

	AlternateMerchantErrorMsg( rmsg, (MSG_EX_ALTERNATE_MERCHANT_ERROR_TYPE)nErrorCode );
	SEND_Q( rmsg, pCh->m_desc );
}

void ProcHelperAlternateMerchantProductRecovery( CNetMsg& msg )
{
	CNetMsg rmsg;
	int nCharIndex = -1;
	int nCount;		int* pItemDBIndex;	
	char strSerial[20][50];
	int* pItemCount;	int* pItemWearPos;
	int* pItemFlag;		int* pItemPlus;
	short* pItemOption0; short* pItemOption1; short* pItemOption2; short* pItemOption3; short* pItemOption4;
	
	msg >> nCharIndex
		>> nCount;

	CPC* pCh = gserver.m_playerList.Find(nCharIndex);

	if( !pCh )
	{
		return;
	}


	int i ;
	pItemDBIndex = new int[nCount]; pItemCount = new int[nCount]; pItemWearPos = new int[nCount];
	pItemFlag = new int[nCount]; pItemPlus = new int[nCount];
	pItemOption0 = new short[nCount]; pItemOption1 = new short[nCount];  pItemOption2 = new short[nCount]; 
	pItemOption3 = new short[nCount]; pItemOption4 = new short[nCount]; 
	for( i = 0; i < nCount; i++ )
	{
		CLCString Serial(50);
		msg >> pItemDBIndex[i] 
			>> Serial
			>> pItemCount[i]
			>> pItemWearPos[i] 
			>> pItemFlag[i] 
			>> pItemPlus[i]
			>> pItemOption0[i] 
			>> pItemOption1[i] 
			>> pItemOption2[i]
			>> pItemOption3[i]
			>> pItemOption4[i];		

		strcpy( strSerial[i], Serial );
	}


}

void ProcHelperAlternateMerchantNasRecovery( CNetMsg& msg )
{
	CNetMsg rmsg;
	int nCharIndex;
	int nItemCount;

	msg >> nCharIndex
		>> nItemCount;

	CPC* pChar	= gserver.m_playerList.Find( nCharIndex );
	if( !pChar )
	{
		// 헬퍼로 다시 전송
		return;
	}

	int* pItemDBIndex = new int[nItemCount];
	char pSerial[20][50];
	int* pItemCount = new int[nItemCount];
	int* pItemWearPos = new int[nItemCount];
	int* pItemFlag = new int[nItemCount];
	int* pItemPlus = new int[nItemCount];
	int* pItemUsed = new int[nItemCount];
#ifdef COMPOSITE_TIME
	int* pItemUse2 = new int[nItemCount];
#endif // COMPOSITE_TIME
//	short* pItemOption0 = new short[nItemCount];
//	short* pItemOption1 = new short[nItemCount];
//	short* pItemOption2 = new short[nItemCount];
//	short* pItemOption3 = new short[nItemCount];
//	short* pItemOption4 = new short[nItemCount];
	short Option[20][MAX_ITEM_OPTION];


	int i;
	for( i =0; i < nItemCount; i++ )
	{
		CLCString strSerial(50);
		msg >> pItemDBIndex[i]
			>> strSerial
			>> pItemCount[i]
			>> pItemWearPos[i] >> pItemFlag[i] >> pItemPlus[i]
			>> pItemUsed[i];
#ifdef COMPOSITE_TIME
			msg >>  pItemUse2[i];
#endif	// COMPOSITE_TIME
		strcpy( pSerial[i], strSerial );
		int k;
		for( k = 0; k < MAX_ITEM_OPTION; k++ )
		{
			msg >> Option[i][k];
		}
	}

	CItem* pAlternateMerchantItem[30];
	for (i = 0; i < nItemCount; i++)
		{
			CItemProto* pItemProto = gserver.m_itemProtoList.FindIndex(pItemDBIndex[i]);


		// Item 생성
			CItem* item = gserver.m_itemProtoList.CreateDBItem(pItemDBIndex[i], -1, pItemPlus[i], pItemFlag[i], pItemUsed[i], 
#ifdef COMPOSITE_TIME
				pItemUse2[i],
#endif	// COMPOSITE_TIME
				pSerial[i], pItemCount[i], Option[i]);
			if (!item)
			{
				// 헬퍼로 실패 메세지 전송 
				// 클라이언트에게 실패 메세지 전송
				return;
			}
			else
			{
				pAlternateMerchantItem[i] = item;
			}
			

			



		}  


//	for( i = 0; i < nItemCount; i++ )
//	{
//		int nRow, nCol;
//		int weight = 0;
////		CItem* pItem = gserver.m_itemProtoList.CreateItem( pItemDBIndex[i] , -1, 0, 0, 1 );
////		if( !pItem )
////		{
//////			EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
//////			SEND_Q( rmsg, ch->m_desc );
////			return;
////		}
//
//		// 인벤의 공간 검사
//
//
////		if( pCh->m_invenNormal.GetSpace() < 
//		
//		// 인벤의 무게 검사
////		weight += pItem->m_itemProto->m_weight * pItemCount[i];
////
////		if( pCh->m_weight + weight >= pCh->m_maxWeight * 15 / 10 )
////		{
////			AlternateMerchantErrorMsg( rmsg, (MSG_EX_ALTERNATE_MERCHANT_ERROR_TYPE)MSG_STASH_ERROR_WEIGHT_OVER );
////			SEND_Q( rmsg, pCh->m_desc );
////			
////			if( IS_RUNNING_HELPER )
////			{
////				CNetMsg msgHelper;
////				SEND_Q( msgHelper, gserver.m_helper );
////			}
////			return;
////		}
//
//
//
////		// 아이템 지급
////		if( AddToInventory( pCh, pItem, true, true ) )
////		{
////			
////			
////			if( pItem->tab() < 0 )
////			{
////				delete pItem;
////				pCh->m_invenNormal.FindItem( &nRow, &nCol, pItemDBIndex[i], 0, 0 );
////				pItem =  pCh->m_invenNormal.GetItem( nRow, nCol );
////				ItemUpdateMsg( rmsg, pItem, pItemCount[i] );
////				SEND_Q( rmsg, pCh->m_desc );
////			}
////			else
////			{
//////				ItemAddMsg( rmsg, pPriceItem );
//////				SEND_Q( rmsg, ch->m_desc );
////			}
////		}
////		else
////		{ 
//////			EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN ); // FULL INVEN
//////			SEND_Q( rmsg, ch->m_desc );
////		}
			
	
//	}

}

void ProcHelperAlternateMerchantStart( CNetMsg& msg )
{
	CNetMsg rmsg;
	int nCharIndex = -1;

	msg >> nCharIndex;

	CPC* pCh = gserver.m_playerList.Find(nCharIndex);

	if( !pCh )
	{
		return;
	}




}

#endif // ALTERNATE_MERCHANT



#ifdef MONSTER_COMBO
void ProcHelperComboGotoWaitRoomPrompt( CNetMsg& msg )
{
	CLCString bossname(256);
	int		bossindex;
	int		nas;

	msg >> bossindex
		>> bossname;

	CParty* party = gserver.FindPartyByBossIndex(bossindex);
	if(party)
	{
		int i = gserver.m_comboZone->FindComboArea(party->m_comboAreaIndex);
		if(i == -1)
			return;

		CArea* area = gserver.m_comboZone->m_area + i;
		nas = area->m_monsterCombo->GetTotalNas();

		CNetMsg rmsg;
		ComboGotoWaitRoomPrompt(rmsg, bossindex, nas);
		
		for(i = 0 ; i < MAX_PARTY_MEMBER; ++i)
		{
			if( party->GetMemberByListIndex(i) &&
				party->GetMemberByListIndex(i)->GetMemberPCPtr() &&
				party->GetMemberByListIndex(i)->GetCharIndex() != bossindex)
			{
				SEND_Q(rmsg, party->GetMemberByListIndex(i)->GetMemberPCPtr()->m_desc);
			}
		}
	}
}

void ProcHelperComboRecallToWaitRoomPrompt(CNetMsg& msg)
{
	int bossindex;
	int nas;
	CNetMsg rmsg;

	msg >> bossindex;

	CParty* party = gserver.FindPartyByBossIndex(bossindex);
	if(party)
	{
		// 파티가 던전에 있는지
		int ret;
		ret = gserver.m_comboZone->FindComboArea(party->m_comboAreaIndex);
		if(ret == -1)
			return ;

		CArea* area = gserver.m_comboZone->m_area + ret;
		nas = area->m_monsterCombo->GetTotalNas();

		CPC* pc;
		int i;
		for(i = 0 ; i < MAX_PARTY_MEMBER; ++i)
		{
			if( party->GetMemberByListIndex(i) &&
				party->GetMemberByListIndex(i)->GetMemberPCPtr())
			{
				pc = party->GetMemberByListIndex(i)->GetMemberPCPtr();
				if(pc->m_pZone->m_index != ZONE_COMBO_DUNGEON)
				{
					ComboGotoWaitRoomPrompt(rmsg, bossindex, nas);
					SEND_Q(rmsg, party->GetMemberByListIndex(i)->GetMemberPCPtr()->m_desc);
				}
			}
		}
	}
}

#endif // MONSTER_COMBO

#ifdef IRIS_POINT
void ProcHelperToConnAddIpoint( CNetMsg& msg )
{
	int userIndex, type;
	int	charIndex=0;
	int nIpoint;

	msg >> userIndex >> type >> nIpoint;
	if( IS_RUNNING_CONN )
	{
		CNetMsg ipoint_msg;
		ConnIPointAddMsg(ipoint_msg, userIndex, charIndex, (MSG_CONN_IPOINT_TYPE)type, nIpoint );
		SEND_Q( ipoint_msg, gserver.m_connector );
	}
}
#endif // IRIS_POINT

#ifdef ATTACK_PET
void ProcHelperAttackPet( CNetMsg& msg )
{
	unsigned char type;
	CNetMsg rmsg;
	int charIndex;

	msg >> type
		>> charIndex;

	CPC* pCh = gserver.m_playerList.Find(charIndex);
	if( !pCh )
		return;

	switch( type )
	{
	case MSG_HELPER_APET_CREATE_OK:
		{
			int pet_dbIndex, pet_protoIndex;
			msg >> pet_dbIndex
				>> pet_protoIndex;

			if(pCh->m_invenNormal.GetSpace() < 1)
			{
				// 최소 인벤 1칸
				return;
			}

			// 펫 생성
			CAPet* apet = gserver.m_pApetlist.Create( pCh, pet_dbIndex , pet_protoIndex );
			if( !apet )
				return;
			ADD_TO_BILIST(apet,pCh->m_pApetlist,m_pPrevPet,m_pNextPet);
			// 펫 아이템 지급
			CAPetProto* proto = gserver.m_pApetlist.FindProto( pet_protoIndex );
			if( proto )
			{
				CItem*petItem =  gserver.m_itemProtoList.CreateItem( proto->m_nItemIndex, -1, pet_dbIndex, 0, 1 );
				if( AddToInventory( pCh, petItem, true, true ) )
				{
					ItemAddMsg(rmsg, petItem);		// APET은 겹칠 일 없음
					SEND_Q(rmsg, pCh->m_desc);
					
					GAMELOG << init("CREATE APET", pCh) << delim
							<< "APET : " <<  pet_dbIndex << delim << pet_protoIndex
							<< end;

					HelperAttackPetMsg( rmsg , MSG_HELPER_APET_ACCENPT_REQ, charIndex );
					rmsg << pet_dbIndex;
					SEND_Q(rmsg, gserver.m_helper );
				}
				else
				{
					// 실패
					GAMELOG << init("CREATE APET FAIL", pCh) << delim
							<< "APET : " <<  pet_dbIndex << delim << pet_protoIndex
							<< end;
				}
			}		
		}
		break;
	case MSG_HELPER_APET_CREATE_FAILED:
		{
			// 아이템 제거
		}
		break;

	case MSG_HELPER_APET_ACCENPT_OK :
		{
			// 펫 마지막 처리 성공
		}
		break;

	case MSG_HELPER_APET_ACCENPT_FAILED :
		{
			// 메모리에서 펫 찾고 삭제
			// 헬퍼 에 dbindex 삭제 요청
			int pet_dbIndex, item_index ;
			msg >> pet_dbIndex
				>> item_index ;
			
			int r, c;
			if( pCh->m_invenNormal.FindItem(&r, &c,  item_index, pet_dbIndex, 0 ) )
			{
				CItem* pet_item = pCh->m_invenNormal.GetItem(r, c);
				DecreaseFromInventory(pCh, pet_item, 1);
				ItemDeleteMsg(rmsg, pet_item);
				SEND_Q(rmsg, pCh->m_desc);
			}

			HelperAttackPetMsg( rmsg , MSG_HELPER_APET_DELETE_REQ, charIndex );
			rmsg << pet_dbIndex 
				 << (char) 3;
		}break;
	}
}
#endif // ATTACK_PET

#ifdef EXTREME_CUBE
void ProcHelperCubeStateRep(CNetMsg& msg)
{
	int charindex;
	int cubepoint;
	int selfpoint;
	char i, count, rank;
	CPC* ch;
	CLCString guildname(MAX_GUILD_NAME_LENGTH + 1), guildmaster(MAX_CHAR_NAME_LENGTH + 1);

	msg >> charindex >> selfpoint >> count;

	ch = gserver.m_playerList.Find(charindex);
	if(!ch)
		return ;

	if(count == 0)
	{
		// 순위 없음
		CNetMsg rmsg;
		rmsg.Init(MSG_EXTEND);
		rmsg << MSG_EX_EXTREME_CUBE
			 << (char)MSG_EX_EXTREME_CUBE_STATE_REP
			 << selfpoint
			 << (int)0;

		SEND_Q( rmsg, ch->m_desc );
		return ;
	}

	if(count > 0)
	{
		CNetMsg rmsg;
		rmsg.Init(MSG_EXTEND);
		rmsg << MSG_EX_EXTREME_CUBE
			 << (char)MSG_EX_EXTREME_CUBE_STATE_REP
			 << selfpoint
			 << count;
		
		for(i = 0; i < count; ++i)
		{
			msg >> rank >> guildname >> guildmaster >> cubepoint;
			
			rmsg << rank << guildname << guildmaster << cubepoint;
		}
		SEND_Q( rmsg, ch->m_desc );
	}
}

void ProcHelperCubeStatePersonalRep(CNetMsg& msg)
{
	int charindex;
	int cubepoint, selfpoint;
	char i, count, rank;
	CNetMsg rmsg;
	
	CPC* ch;
	CLCString guildname(MAX_GUILD_NAME_LENGTH + 1), charname(MAX_CHAR_NAME_LENGTH + 1);

	msg >> charindex >> selfpoint >>count;

	ch = gserver.m_playerList.Find(charindex);
	if(!ch)
		return ;

	if(count == 0)
	{
		rmsg.Init(MSG_EXTEND);
		rmsg << MSG_EX_EXTREME_CUBE
			 << (char)MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REP
			 << selfpoint
			 << (int)0;

		SEND_Q(rmsg, ch->m_desc );
		return ;
	}
	
	if(count > 0)
	{
		rmsg.Init(MSG_EXTEND);
		rmsg << MSG_EX_EXTREME_CUBE
			 << (char)MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REP
			 << selfpoint
			 << count;

		for(i = 0 ; i < count; ++i)
		{
			msg >> rank
				>> guildname
				>> charname
				>> cubepoint;

			rmsg << rank
				 << guildname
				 << charname
				 << cubepoint;
		}
		SEND_Q(rmsg, ch->m_desc );
	}
}

void ProcHelperGuildCubeNotice(CNetMsg& msg)
{
	int type;
	CNetMsg rmsg;
	msg >> type;

	switch(type)
	{
	case 0:
		ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_START_NOTICE);
		break;
	case 1:
		ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_END_NOTICE);
		break;
	case 2:
		ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_START_REMAINTIME);
		break;
	case 3:
		ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_END_REMAINTIME);
		break;
	default:
		return ;
	}
	gserver.m_playerList.SendToAll(rmsg);
}

#endif // EXTREME_CUBE

#ifdef EVENT_PHOENIX	// //피닉스 이벤트 yhj	
void ProcHelperEventPhoenix( CNetMsg& msg )
{
	unsigned char type;
	int charindex = 0;

	msg >> type >> charindex;

	CPC* ch;
	CNetMsg rmsg;

	ch = gserver.m_playerList.Find(charindex);
	if( !ch )
	{
		return;
	}

	// 케릭터 디비에서 검색했을 때 피닉스 이벤트가 가능하다는 응답이 왔을 경우
	if ( (MSG_HELPER_EVENT_PHOENIX_ERROR_TYPE)type == MSG_HELPER_EVENT_PHOENIX_OK )
	{
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_OK);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 케릭터 디비에서 검색했을 때 레벨이 100을 넘는 것이 없어서 피닉스 이벤트 자격이 안된다는 응답
	else if ( (MSG_HELPER_EVENT_PHOENIX_ERROR_TYPE)type == MSG_HELPER_EVENT_PHOENIX_ERROR_REQUIREMENT )
	{
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_REQUIREMENT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

}

#endif  // EVENT_PHOENIX


#ifdef TRADE_AGENT		// 거래 대행

//에러 메세지
void ProcHelperTradeAgentErrorRep( CNetMsg& msg )
{
	CNetMsg rmsg;
	unsigned char errorType;
	char errorPart;
	int nCharIndex = -1;

	msg >> errorType
		>> nCharIndex
		>> errorPart;

	CPC* ch = gserver.m_playerList.Find(nCharIndex);

	if(!ch)	return;

	//변수 초기화
	//-->
	switch(errorPart)
	{
	case MSG_TRADEAGENT_ERROR_PART_REGIST:
		{
			ch->m_bTradeAgentRegIng = false;	
		}
		break;
	case MSG_TRADEAGENT_ERROR_PART_CALCULATE:
		{
			ch->m_bTradeAgentCalcIng = false;
		}
		break;
	}
	//<--

	TradeAgentErrorMsg(rmsg, errorType,errorPart);
	SEND_Q(rmsg, ch->m_desc);
}


//등록 리스트
void ProcHelperTradeAgentRegListRep( CNetMsg& msg )
{
	CNetMsg rmsg;
	int nCharIndex = -1;
	int nItemCount;		
	int nCharRegTotCount;
	int nMaxPageNo,nPageNo;

	msg >> nCharIndex
		>> nCharRegTotCount
		>> nMaxPageNo
		>> nPageNo
		>> nItemCount;

	CPC* ch = gserver.m_playerList.Find(nCharIndex);

	if(!ch)	return;

	int* pListindex				= new int[nItemCount];			// 리스트 인덱스
	int* pItemSerial			= new int[nItemCount];			// 아이템 시리얼 
	int* pItemPlus				= new int[nItemCount];			// 아이템 플러스
	int* pItemFlag				= new int[nItemCount];			// 아이템 플래그	
	S_ITEMOPTION* pItemOption	= new S_ITEMOPTION[nItemCount];	// 아이템 옵션
	int* pQuantity				= new int[nItemCount];			// 수량 
	LONGLONG* ptotalmoney		= new LONGLONG[nItemCount];		// 총액 
	int* pFinishDay				= new int[nItemCount];			// 마감일
	int* pFinishDayUnit			= new int[nItemCount];			// 마감일 단위

	int i ;
	for( i = 0; i < nItemCount; i++ )
	{
		msg >> pListindex[i]
			>> pItemSerial[i] 
			>> pItemPlus[i]
			>> pItemFlag[i]

		    >> pItemOption[i].m_nOptionCnt;

		for(int j=0; j < pItemOption[i].m_nOptionCnt; j++)
		{
			msg >> pItemOption[i].m_Option_type[j]
				>> pItemOption[i].m_Option_level[j];
		}
		
		 msg >> pQuantity[i] 
			 >> ptotalmoney[i]
			 >> pFinishDay[i]		
			 >> pFinishDayUnit[i];
	}	
	
	TradeAgentRegListRepMsg(rmsg, nCharIndex, nCharRegTotCount, nMaxPageNo, nPageNo, nItemCount, pListindex, pItemSerial, pItemPlus, pItemFlag,pItemOption,pQuantity,ptotalmoney,pFinishDay,pFinishDayUnit);
	SEND_Q(rmsg, ch->m_desc);

	delete[] pListindex; 
	delete[] pItemSerial; 
	delete[] pItemPlus;
	delete[] pItemFlag;
	delete[] pItemOption;
	delete[] pQuantity; 
	delete[] ptotalmoney;  
	delete[] pFinishDay; 
	delete[] pFinishDayUnit;

}

//등록
void ProcHelperTradeAgentRegRep( CNetMsg& msg )
{
	CNetMsg rmsg;
	CInventory* inven;
	CItem* item;
	const CItemProto* itemproto;
	char tab,row,col;
	int nCharIndex,nitem_serial, nQuantity;
	int nDbIndex;
	LONGLONG TotalMoney,Guaranty;
	CLCString item_name(MAX_ITEM_NAME_LENGTH + 1);
	CLCString item_serial2(MAX_SERIAL_LENGTH + 1);	

	msg >> nCharIndex
		>> tab >> row >> col
		>> nitem_serial
		>> item_serial2
		>> item_name
		>> nQuantity
		>> TotalMoney
		>> Guaranty
		>> nDbIndex;			

	CPC* ch = gserver.m_playerList.Find(nCharIndex);

	int nResult = 1;

	if(!ch)
	{
		//helper 서버로 다시 처리 결과를 보냄 등록 안되게 함
		if (IS_RUNNING_HELPER)  
		{
			//처리 결과 전달
			nResult =0;
			HelperTradeAgentRegResultMsg(rmsg, nCharIndex, nDbIndex,nResult);
			SEND_Q(rmsg, gserver.m_helper);
			return;
		}
	}

	GAMELOG << init("TRADE AGENT REGIST", ch )
			<< " ITEM SERIAL: "  << nitem_serial << delim
			<< " ITEM SERIAL2: "  << item_serial2 << delim
			<< " ITEM COUNT : "  << delim <<  nQuantity 
			<< " TOTAL MONEY: "  << delim <<  TotalMoney 
			<< " GUARANTY:"  << delim <<  Guaranty << end;

	// 아이템 찾기
	inven = GET_INVENTORY(ch, tab);
	if (inven == NULL)
	{
		if (IS_RUNNING_HELPER)  
		{
			nResult =0;
			HelperTradeAgentRegResultMsg(rmsg, nCharIndex, nDbIndex,nResult);
			SEND_Q(rmsg, gserver.m_helper);
			return;
		}
	}

	item = inven->GetItem(row, col);
	if (item == NULL || item->m_idNum != nitem_serial || item->m_wearPos != -1)
	{
		if (IS_RUNNING_HELPER)  
		{
			nResult =0;
			HelperTradeAgentRegResultMsg(rmsg, nCharIndex, nDbIndex,nResult);
			SEND_Q(rmsg, gserver.m_helper);
			return;
		}	
	}

	itemproto = item->m_itemProto;
	if (itemproto == NULL)
	{
		if (IS_RUNNING_HELPER)  
		{
			nResult =0;
			HelperTradeAgentRegResultMsg(rmsg, nCharIndex, nDbIndex,nResult);
			SEND_Q(rmsg, gserver.m_helper);
			return;
		}	
	}

	// 보증금 차감 
	//-->
	if (ch->m_moneyItem)
	{
		if ((ch->m_moneyItem->Count() > Guaranty))
		{
			ch->AddMoney(-Guaranty);
			ItemUpdateMsg(rmsg, ch->m_moneyItem, -Guaranty);
			SEND_Q(rmsg, ch->m_desc);
		}
		else if ((ch->m_moneyItem->Count() == Guaranty))
		{
			ItemDeleteMsg(rmsg, ch->m_moneyItem);
			ch->AddMoney(-Guaranty);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	//<--

	//아이템 개수를 등록 수량 만큼 차감 
	//-->
	if(item->Count() > nQuantity)
	{	
		// Quantity 수만큼 제거
		DecreaseFromInventory(ch, item, nQuantity);
		
		ItemUpdateMsg(rmsg, item, -1);
		SEND_Q(rmsg, ch->m_desc);
	}
	else if(item->Count() == nQuantity)
	{
		// 완전히 빼기
		ItemDeleteMsg(rmsg, item);
		SEND_Q(rmsg, ch->m_desc);
		
		RemoveFromInventory(ch, item, false, true);
	}
	//<--

	//현재 등록 처리 중 리셋
	//-->
	ch->m_bTradeAgentRegIng = false;
	//<--

	//결과 전송 
	TradeAgentRegRepMsg(rmsg, nCharIndex, item_name,nQuantity,TotalMoney,Guaranty);
	SEND_Q(rmsg, ch->m_desc);	
}

//등록 취소
void ProcHelperTradeAgentRegCancelRep( CNetMsg& msg )
{
	CNetMsg rmsg;
	int nCharIndex;
	int nQuantity = 0;
	CLCString stItemName(MAX_ITEM_NAME_LENGTH + 1);

	msg >> nCharIndex
		>> stItemName
		>> nQuantity;

	CPC* ch = gserver.m_playerList.Find(nCharIndex);

	if(!ch)	return;

	GAMELOG << init("TRADE AGENT REGIST CANCEL", ch )
			<< "ITEM NAME : " << stItemName <<  " COUNT :" << nQuantity << end;

	//취소 결과
	TradeAgentRegCancelRepMsg(rmsg, nCharIndex);
	SEND_Q(rmsg, ch->m_desc);

	//정산 알림 메세지
	TradeAgentNeedCalcMotifyMsg(rmsg,nCharIndex,MSG_TRADEAGENT_STATE_SELL_RETURNED,stItemName,nQuantity);
	SEND_Q(rmsg, ch->m_desc);
}

//조회
void ProcHelperTradeAgentSearchRep( CNetMsg& msg )
{
	CNetMsg rmsg;
	int nCharIndex = -1;
	int nItemCount;		
	int nMaxPageNo, nPageNo;

	msg >> nCharIndex
		>> nMaxPageNo
		>> nPageNo
		>> nItemCount;

	CPC* ch = gserver.m_playerList.Find(nCharIndex);

	if(!ch)	return;

	int* pListindex				= new int[nItemCount];			// 리스트 인덱스
	int* pItemSerial			= new int[nItemCount];			// 아이템 시리얼 
	int* pItemPlus				= new int[nItemCount];			// 아이템 플러스
	int* pItemFlag				= new int[nItemCount];			// 아이템 플래그
	S_ITEMOPTION* pItemOption	= new S_ITEMOPTION[nItemCount];	// 아이템 옵션
	int* pQuantity				= new int[nItemCount];			// 수량 
	LONGLONG* ptotalmoney		= new LONGLONG[nItemCount];		// 총액 
	int* pLevel					= new int[nItemCount];			// 최소 레벨
	CLCString *pSellCharname	= new CLCString[nItemCount];	// 판매자 아이디

	int i ;
	for( i = 0; i < nItemCount; i++ )
	{
		msg >> pListindex[i]
			>> pItemSerial[i] 
			>> pItemPlus[i]
			>> pItemFlag[i]

		    >> pItemOption[i].m_nOptionCnt;

		for(int j=0; j < pItemOption[i].m_nOptionCnt; j++)
		{
			msg >> pItemOption[i].m_Option_type[j]
				>> pItemOption[i].m_Option_level[j];
		}

		msg >> pQuantity[i] 
			>> ptotalmoney[i]
			>> pLevel[i]
			>> pSellCharname[i];		
	}	

	TradeAgentSearchRepMsg(rmsg, nCharIndex, nMaxPageNo, nPageNo, nItemCount, pListindex, pItemSerial,pItemPlus,pItemFlag,pItemOption,pQuantity,ptotalmoney,pLevel,pSellCharname);
	SEND_Q(rmsg, ch->m_desc);

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

//반송 
void ProcHelperTradeAgentReturned( CNetMsg& msg )
{
	CNetMsg rmsg;
	int nSellCharIndex = -1;		//상품 등록 캐릭터 인덱스 
	int nQuantity = 0;
	CLCString stItemName(MAX_ITEM_NAME_LENGTH + 1);

	msg >> nSellCharIndex
		>> stItemName
		>> nQuantity;

	CPC* ch = gserver.m_playerList.Find(nSellCharIndex);

	if(!ch)	return;

	GAMELOG << init("TRADE AGENT ITEM RETURN", ch )
			<< "ITEM NAME : " << stItemName <<  " COUNT :" << nQuantity << end;

	//정산 알림 메세지
	TradeAgentNeedCalcMotifyMsg(rmsg,nSellCharIndex,MSG_TRADEAGENT_STATE_SELL_RETURNED,stItemName,nQuantity);
	SEND_Q(rmsg, ch->m_desc);		

}

//구매
void ProcHelperTradeAgentBuyRep( CNetMsg& msg )
{
	CNetMsg rmsg;
	int nCharIndex = -1;
	int nSellcharindex = -1;
	LONGLONG TotalMoney;
	int nQuantity = 0;
	CLCString stItemName(MAX_ITEM_NAME_LENGTH + 1);

	msg >> nCharIndex
		>> nSellcharindex
		>> TotalMoney
		>> stItemName
		>> nQuantity;

	CPC* ch = gserver.m_playerList.Find(nCharIndex);

	if(!ch)	return;

	// 총 금액 차감
	if (ch->m_moneyItem)
	{
		if ((ch->m_moneyItem->Count() > TotalMoney))
		{
			ch->AddMoney(-TotalMoney);
			ItemUpdateMsg(rmsg, ch->m_moneyItem, -TotalMoney);
			SEND_Q(rmsg, ch->m_desc);
		}
		else if ((ch->m_moneyItem->Count() == TotalMoney))
		{
			ItemDeleteMsg(rmsg, ch->m_moneyItem);
			ch->AddMoney(-TotalMoney);
			SEND_Q(rmsg, ch->m_desc);

		}else
			return;
	}

	GAMELOG << init("TRADE AGENT ITEM BUY", ch )
			<< " SELLER INDEX : " << nSellcharindex 
			<< " ITEM NAME : " << stItemName <<  " COUNT :" << nQuantity << end;

	GAMELOG << init("TRADE AGENT ITEM BUY", ch )
			<< " SELLER INDEX : " << nSellcharindex 
			<< " MONEY : " << TotalMoney << end;

	//클라이언트 결과 전송
	TradeAgentBuyRepMsg(rmsg, nCharIndex);
	SEND_Q(rmsg, ch->m_desc);

	//판매자에게 통보 처리
	CPC* chSeller = gserver.m_playerList.Find(nSellcharindex);
	if(chSeller)
	{
		//정산 알림 메세지
		TradeAgentNeedCalcMotifyMsg(rmsg,nSellcharindex,MSG_TRADEAGENT_STATE_SELL_COMPLETE,stItemName,nQuantity);
		SEND_Q(rmsg, chSeller->m_desc);
	}
}

//정산 리스트
void ProcHelperTradeAgentCalcListRep( CNetMsg& msg )
{
	CNetMsg rmsg;
	int nCharIndex = -1;
	int nItemCount;		
	int nMaxPageNo,nPageNo;

	msg >> nCharIndex
		>> nMaxPageNo
		>> nPageNo
		>> nItemCount;

	CPC* ch = gserver.m_playerList.Find(nCharIndex);

	if(!ch)	return;

	//페이지 번호 저장
	ch->m_nCalcPageNo = nPageNo;

	int* pItemSerial			= new int[nItemCount];			// 아이템 시리얼 
	int* pItemPlus				= new int[nItemCount];			// 아이템 플러스
	int* pItemFlag				= new int[nItemCount];			// 아이템 플래그
	S_ITEMOPTION* pItemOption	= new S_ITEMOPTION[nItemCount];	// 아이템 옵션
	int* pQuantity				= new int[nItemCount];			// 수량 
	LONGLONG* ptotalmoney		= new LONGLONG[nItemCount];		// 총액 
	int* pState					= new int[nItemCount];			// 상태
	int* pPassDay				= new int[nItemCount];			// 처리부터의 지난날
	CLCString *pCharname		= new CLCString[nItemCount];	// 구매,판매 캐릭터 이름

	int i ;
	for( i = 0; i < nItemCount; i++ )
	{
		msg >> pItemSerial[i] 
			>> pItemPlus[i]
			>> pItemFlag[i]
		    >> pItemOption[i].m_nOptionCnt;

		for(int j=0; j < pItemOption[i].m_nOptionCnt; j++)
		{
			msg >> pItemOption[i].m_Option_type[j]
				>> pItemOption[i].m_Option_level[j];
		}
		
		msg	>> pQuantity[i] 
			>> ptotalmoney[i]
			>> pState[i]
			>> pPassDay[i]
			>> pCharname[i];		
	}	
	
	TradeAgentCalcListRepMsg(rmsg, nCharIndex, nMaxPageNo, nPageNo, nItemCount, pItemSerial,pItemPlus,pItemFlag,pItemOption,pQuantity,ptotalmoney,pState,pPassDay,pCharname);
	SEND_Q(rmsg, ch->m_desc);

	delete[] pItemSerial; 
	delete[] pItemPlus;
	delete[] pItemFlag;
	delete[] pItemOption;
	delete[] pQuantity; 
	delete[] ptotalmoney;  
	delete[] pPassDay;	
	delete[] pState;
	delete[] pCharname;
}


//정산
void ProcHelperTradeAgentCalculateRep( CNetMsg& msg )
{
	CNetMsg rmsg;
	int nCharIndex = -1;
	int nItemCount;		
	int i;
	int nMaxPageNo,nPageNo;

	msg >> nCharIndex >> nMaxPageNo >> nPageNo;

	CPC* ch = gserver.m_playerList.Find(nCharIndex);

	if(!ch)	return;

	msg >> nItemCount;

	GAMELOG << init("TRADE AGENT CALCULATE START", ch )
			<< " ITEM COUNT :"  << delim <<  nItemCount << end;

	int* pListindex				= new int[nItemCount];			// db 인덱스	
	int* pItemSerial			= new int[nItemCount];			// 아이템 시리얼 
	CLCString* pItemSerial2		= new CLCString[nItemCount];	// 아이템 시리얼2
	int* pItemPlus				= new int[nItemCount];			// 아이템 플러스
	int* pItemFlag				= new int[nItemCount];			// 아이템 플래그
	S_ITEMOPTION* pItemOption	= new S_ITEMOPTION[nItemCount];	// 아이템 옵션
	int* pQuantity				= new int[nItemCount];			// 수량 
	LONGLONG* ptotalmoney		= new LONGLONG[nItemCount];		// 총액 
	int* pState					= new int[nItemCount];			// 상태(판매/반품)	

	//처리 결과 
	int* pIndexResult			= new int[nItemCount];			// db 인덱스 
	int* pProcResult			= new int[nItemCount];			// 처리결과

	short option[MAX_ITEM_OPTION];
	memset( option, 0 , sizeof(short)*MAX_ITEM_OPTION);

	for( i = 0; i < nItemCount; i++ )
	{
		msg >> pListindex[i]
			>> pItemSerial[i]
			>> pItemSerial2[i]
			>> pItemPlus[i]
			>> pItemFlag[i]

		    >> pItemOption[i].m_nOptionCnt;

		for(int j=0; j < pItemOption[i].m_nOptionCnt; j++)
		{
			msg >> pItemOption[i].m_Option_type[j]
				>> pItemOption[i].m_Option_level[j];
		}

		msg	>> pQuantity[i]
			>> ptotalmoney[i]
			>> pState[i];

		pIndexResult[i] = 0;   pProcResult[i] = 0;		// 초기화 
	}

	//나스 우선 처리
	//-->
	LONGLONG myTotalmoney = 0;
	for( i = 0;  i < nItemCount; i++ )
	{
		if(pState[i] == MSG_TRADEAGENT_STATE_SELL_COMPLETE)
		{
			myTotalmoney += ptotalmoney[i];

			//결과 데이타 저장 
			pIndexResult[i] = pListindex[i];   pProcResult[i] = 1;
		}
	}

	//나스 메세지
	CItem** pItem = NULL;
	bool	bInvenFull = true;

	CNetMsg moneyMsg;
	if (myTotalmoney > 0)
	{
		if(ch->m_moneyItem)
		{
			ch->AddMoney(myTotalmoney);
			ItemUpdateMsg(moneyMsg, ch->m_moneyItem, myTotalmoney);
		}
		else
		{
			ch->AddMoney(myTotalmoney);
			ItemAddMsg( moneyMsg, ch->m_moneyItem );
		}
		SEND_Q(moneyMsg, ch->m_desc);				
			
		GAMELOG << init("TRADE AGENT CALCULATE MONEY ", ch ) 
				<< delim << myTotalmoney   << end;
	}

	//<--
	//아이템 처리 (반송,구입)
	//-->
	pItem = NULL;
	pItem = new CItem*[nItemCount];
	memset(pItem, 0, sizeof(CItem*) * nItemCount);


	GAMELOG << init("TRADE AGENT GET ITEM");
	
	for( i = 0;  i < nItemCount; i++ )
	{
		if(pState[i] == MSG_TRADEAGENT_STATE_SELL_RETURNED || pState[i] == MSG_TRADEAGENT_STATE_BUY)
		{
			//DB INDEX 로 아이템 생성 
			pItem[i] = gserver.m_itemProtoList.CreateItem(pItemSerial[i], WEARING_NONE, pItemPlus[i], pItemFlag[i], pQuantity[i]);

			//새로운 시리얼이 있는 경우만 처리 
			if(pItemSerial2[i].Length() > 0)
			{
				//시리얼2 설정
				pItem[i]->m_serial = pItemSerial2[i];
			}

			bool bIsRare = false;

#ifdef MONSTER_RAID_SYSTEM
			if (pItem[i]->IsRareItem())
					bIsRare = true;
#endif // MONSTER_RAID_SYSTEM
		
			pItem[i]->m_nOption = pItemOption[i].m_nOptionCnt;

			for(int j=0 ; j < pItemOption[i].m_nOptionCnt; j++)
			{
				pItem[i]->m_option[j].m_level = pItemOption[i].m_Option_level[j];
				pItem[i]->m_option[j].m_type =  pItemOption[i].m_Option_type[j];
				pItem[i]->m_option[j].m_proto = gserver.m_optionProtoList.FindProto(pItem[i]->m_option[j].m_type);

				 if( pItem[i]->m_option[j].m_proto )
				 {
					pItem[i]->m_option[j].m_value = pItem[i]->m_option[j].m_proto->m_levelValue[pItem[i]->m_option[j].m_level - 1];
				 }
			}
			
			if(bIsRare)
			{
				if( pItem[i]->m_nOption > 0 )
				{
					pItem[i]->m_option[0].SetDBValue();
					pItem[i]->m_option[1].SetDBValue();

					int nRareIndex = pItem[i]->m_option[0].m_dbValue;
					int nRareBit = pItem[i]->m_option[1].m_dbValue;

					pItem[i]->m_nOption = 2;
					pItem[i]->m_option[0].GetDBValue((short)nRareIndex);
					pItem[i]->m_option[1].GetDBValue((short)nRareBit);
					pItem[i]->m_pRareOptionProto = gserver.m_rareOptionList.Find(nRareIndex);
					pItem[i]->m_nRareOptionIndex = nRareIndex;
					pItem[i]->m_nRareOptionBit = nRareBit;
				}
			}

			if (pItem[i])
			{
				if ( AddToInventory(ch, pItem[i], false, true) )
				{
					GAMELOG << "[" << i << "]" <<delim << itemlog( pItem[i] , true )  << delim ;

					CNetMsg itemmsg;
					// 인벤안에 셋팅되어 있으면 Add
					if (pItem[i]->tab() != -1)
					{
						ItemAddMsg(itemmsg, pItem[i]);
						SEND_Q(itemmsg, ch->m_desc);
					}
					else // 인벤에 셋팅되어있지 않으면 Update
					{
						int row, col;
						ch->m_invenNormal.FindItem(&row, &col, pItem[i]->m_idNum, pItem[i]->m_plus, pItem[i]->m_flag);
						CItem* tItem = ch->m_invenNormal.GetItem(row, col);
							
						if (tItem)
						{
							ItemUpdateMsg(itemmsg, tItem, pQuantity[i]);
							SEND_Q(itemmsg, ch->m_desc);
						}
						else
						{
							delete pItem[i];
							bInvenFull = false;
							break;
						}
						delete pItem[i];   //서버 다운 체크
					}					
					pItem[i] = NULL;

					//결과 데이타 저장 
					pIndexResult[i] = pListindex[i];
					pProcResult[i] = 1;
				}
				else
				{
					delete pItem[i];
					pItem[i] = NULL;

					// 인벤토리 가득 참
					CNetMsg errmsg;
					SysFullInventoryMsg(errmsg, (char)ch->m_invenNormal.m_tab);
					SEND_Q(errmsg, ch->m_desc);	
					
					bInvenFull = false;

					GAMELOG << end;
					GAMELOG << init("TRADE AGENT GET ITEM FAIL FULL") ;
					break;
				}
			}
		}
	}
	GAMELOG << end;
	//<--
	GAMELOG << init("TRADE AGENT CALCULATE END", ch ) << end;

	if(bInvenFull && nMaxPageNo == nPageNo)
	{
		//정산 처리 완료 (인벤 풀이 아닌 경우에만 메세지 보냄)
		TradeAgentCalculateRepMsg(rmsg, nCharIndex);
		SEND_Q(rmsg, ch->m_desc);
	}

	if (IS_RUNNING_HELPER)  
	{
		//처리 결과 전달
		HelperTradeAgentCalcResultMsg(rmsg, ch->m_index,nItemCount,pIndexResult,pProcResult);
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NORMAL,MSG_TRADEAGENT_ERROR_PART_CALCULATE);
		SEND_Q(rmsg, ch->m_desc);
	}
	//<--

	//현재 정산 처리 중 리셋 
	//-->
	ch->m_bTradeAgentCalcIng = false;
	//<--

	delete[] pListindex;
	delete[] pItemSerial;
	delete[] pItemSerial2;
	delete[] pItemPlus;
	delete[] pItemFlag;
	delete[] pItemOption;
	delete[] pQuantity;
	delete[] ptotalmoney;
	delete[] pState;
	delete[] pIndexResult;
	delete[] pProcResult;
}

//체크 정산
void ProcHelperTradeAgentCheckCalcRep( CNetMsg& msg )
{
	CNetMsg rmsg;
	int nCharIndex = -1;
	int nIsBeCalcdata;		

	msg >> nCharIndex;

	CPC* ch = gserver.m_playerList.Find(nCharIndex);

	if(!ch)	return;

	msg >> nIsBeCalcdata;

	//결과 전송 
	TradeAgentCheckCalcRepMsg(rmsg, nCharIndex,nIsBeCalcdata);
	SEND_Q(rmsg, ch->m_desc);
}

#endif // TRADE_AGENT

#ifdef EXPEDITION_RAID
//에러
void ProcHelperExpedErrorRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	unsigned char errorType;
	int nCharIndex = -1;

	msg >> errorType
		>> nCharIndex;

	CPC* ch = gserver.m_playerList.Find(nCharIndex);

	if(!ch)	return;

	ExpedErrorMsg(rmsg, errorType);
	SEND_Q(rmsg, ch->m_desc);
}
//생성 (파티 전환)
void ProcHelperExped_CreateRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	char cExpedType1,cExpedType2,cExpedType3;
	int nMemberCount, nBossIndex;

	CLCString BossName(MAX_CHAR_NAME_LENGTH + 1);

	msg	>> cExpedType1 >> cExpedType2 >> cExpedType3
		>> nBossIndex >> BossName 
		>> nMemberCount;

	int* pCharIndex		 = new int[nMemberCount];			// 캐릭터 인덱스 
	CLCString* pCharName = new CLCString[nMemberCount];		// 캐릭터 이름 
	int* pGroupType		 = new int[nMemberCount];			// 그룹 타입 
	int* pMemberType	 = new int[nMemberCount];			// 멤버 타입 
	int* pSetLabelType	 = new int[nMemberCount];			// 표식
	int* pQuitType		 = new int[nMemberCount];			// 정상 ,비정상


	for(int i=0; i < nMemberCount; i++)
	{
		msg >> pCharIndex[i]
			>> pCharName[i]
			>> pGroupType[i]
			>> pMemberType[i]
			>> pSetLabelType[i]
			>> pQuitType[i];
	}

	CPC* pBoss = gserver.m_playerList.Find(nBossIndex);

	if(!pBoss)	return;

	// 신규 원정대 생성
	CExpedition* pExped = NULL;
	pExped = new CExpedition(cExpedType1, nBossIndex,(const char*)BossName,MSG_EXPED_GROUP_1,MSG_EXPED_MEMBERTYPE_BOSS,pBoss);
	
	if(pExped)
	{
		CExpedMember* pExpMember = NULL;
		for(int i=0; i < nMemberCount; i++)
		{
			CPC* pMember = gserver.m_playerList.Find(pCharIndex[i]);
	
			int nJoinCnt = 0;
			if(pMember)
			{
				//대원 추가
				pExpMember = (CExpedMember*) pExped->Join(pCharIndex[i],pCharName[i],MSG_EXPED_MEMBERTYPE_NORMAL,pMember);
				if(pExpMember)
				{
					pExpMember->SetLabelType(pSetLabelType[i]);
					pExpMember->SetQuitType(pQuitType[i]);

					nJoinCnt++;
				}
			}
		}

		// 파티 해체 알림(삭제)
//		PartyMsg(rmsg, MSG_PARTY_END);
//		pExped->SendToAllPC(rmsg);

		//결과 전송
		ExpedCreateRepMsg(rmsg,cExpedType1,cExpedType2,cExpedType3,nBossIndex,BossName,nMemberCount,pCharIndex,pCharName,pGroupType,pMemberType,pSetLabelType,pQuitType);
		pExped->SendToAllPC(rmsg);
	}


	delete[] pCharIndex;
	delete[] pCharName;
	delete[] pGroupType;
	delete[] pMemberType;
	delete[] pSetLabelType;
	delete[] pQuitType;
}

//초대 
void ProcHelperExped_InviteRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);
	char		cExpedType1;
	char		cExpedType2;
	char		cExpedType3;

	msg >> nBossIndex
		>> strBossName
		>> nTargetIndex
		>> strTargetName
		>> cExpedType1
		>> cExpedType2
		>> cExpedType3;

	CPC* pBossPC = gserver.m_playerList.Find(nBossIndex);
	CPC* pTargetPC = gserver.m_playerList.Find(nTargetIndex);

	CExpedition* pExped = gserver.FindExpedByBossIndex(nBossIndex);
	if (pExped)
	{
		// 기존 원정대 초대 상태로
		pExped->SetRequest(nTargetIndex, strTargetName);
		if (pTargetPC)
			pTargetPC->m_Exped = pExped;
	}

	//초대 메세지
	ExpedInviteRepMsg(rmsg, cExpedType1, cExpedType2, cExpedType3, nBossIndex, strBossName);
	if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
	if (pTargetPC)		SEND_Q(rmsg, pTargetPC->m_desc);
}

//초대 수락 
void ProcHelperExped_AllowRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	CNetMsg addmsg;
	int			nBossIndex;
	int			nTargetIndex,nTargetGroup,nTargetMember,nTargetListIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> nBossIndex
		>> nTargetIndex
		>> strTargetName
		>> nTargetGroup
		>> nTargetMember
	    >> nTargetListIndex;

	CPC* pTargetPC = gserver.m_playerList.Find(nTargetIndex);

	CExpedition* pExped = gserver.FindExpedByBossIndex(nBossIndex);
	if (pExped == NULL)
	{
		return ;
	}

	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*)pExped->JoinRequest(strTargetName, nTargetMember, pTargetPC);

	if (pMember==NULL)
	{
		return;
	}

	// 파티원들에게 결과 알리기
	ExpedAddMsg(rmsg, nTargetIndex, strTargetName, nTargetGroup, nTargetMember,nTargetListIndex, pTargetPC, ((pExped->GetBossIndex() == nTargetIndex) ? 1 : 0));
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			const CExpedMember* pMember = pExped->GetMemberByListIndex(i,j);
			if (pMember && i != pMember->GetListIndex())
			{
				// 타인에게 target을 추가하고
				if (pMember->GetMemberPCPtr())
					SEND_Q(addmsg, pMember->GetMemberPCPtr()->m_desc)

				// target에게는 타인을 추가
				if (pTargetPC)
				{
					ExpedAddMsg(rmsg, pMember->GetCharIndex(), pMember->GetCharName(), nTargetGroup, nTargetMember,nTargetListIndex, pMember->GetMemberPCPtr(), ((pExped->GetBossIndex() == pMember->GetCharIndex()) ? 1 : 0));
					SEND_Q(rmsg, pTargetPC->m_desc);
				}
			}
		}
	}

}

//초대 거절 
void ProcHelperExped_RejectRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int			nBossIndex;
	int			nTargetIndex;
	int			nErrorCode;

	msg >> nBossIndex
		>> nTargetIndex
		>> nErrorCode;

	CExpedition* pExped = gserver.FindExpedByBossIndex(nBossIndex);
	if (!pExped)
	{
		return ;
	}
	if (pExped->GetRequestIndex() < 1)
	{
		return ;
	}

	CPC* pBossPC = pExped->GetMemberByListIndex(0,0)->GetMemberPCPtr();
	CPC* pRequestPC = gserver.m_playerList.Find(pExped->GetRequestIndex());

	if (pExped->GetRequestIndex() == nTargetIndex)
		ExpedMsg(rmsg, MSG_REJECT_DEST);
	else if (pExped->GetBossIndex() == nTargetIndex)
		ExpedMsg(rmsg, MSG_REJECT_SRC);
	else
	{
		return ;
	}

	pExped->SetMemberPCPtr(pExped->GetRequestIndex(), NULL);

	if (pBossPC)
		SEND_Q(rmsg, pBossPC->m_desc);
	if (pRequestPC)
	{
		SEND_Q(rmsg, pRequestPC->m_desc);
		pRequestPC->m_party = NULL;
	}

	pExped->SetRequest(-1, "");
	if (pExped->GetMemberCount() < 2)
	{
		// 파티 종료
		gserver.m_listExped.RemoveData(pExped);
		delete pExped;
	}
}

//탈퇴(나감)
void ProcHelperExped_QuitRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int			nBossIndex;
	int			nTargetIndex;
	int			nErrorCode;
	int			nQuitMode;			// 정상,비정상 구분	
	CLCString	strOldBossName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> nBossIndex
		>> nTargetIndex
		>> nQuitMode
		>> nErrorCode;

	CExpedition* pExped = gserver.FindExpedByBossIndex(nBossIndex);
	if (!pExped)
		return;

	CPC* pTargetPC = gserver.m_playerList.Find(nTargetIndex);
	if (pTargetPC)
	{
		pTargetPC->CancelDamageLink();
		pTargetPC->m_Exped = NULL;
	}

	strOldBossName = pExped->GetBossName();

	if(nQuitMode == MSG_EXPED_QUITMODE_NORMAL)	
	{
		pExped->SetMemberPCPtr(nTargetIndex, NULL);
		pExped->DeleteMember(nTargetIndex);
	}	

	if (nErrorCode == MSG_EXPED_ERROR_QUIT_END)
	{
		// 원정대 해체 알림
		ExpedMsg(rmsg, MSG_ENDEXPED_REP);

		if (pTargetPC)
			SEND_Q(rmsg, pTargetPC->m_desc);

		pExped->SendToAllPC(rmsg, nTargetIndex);

		// 초대 거절
		if (pExped->GetRequestIndex() > 0)
		{
			CPC* pRequestPC = gserver.m_playerList.Find(pExped->GetRequestIndex());
			if (pRequestPC)
			{
				ExpedMsg(rmsg, MSG_REJECT_SRC);
				SEND_Q(rmsg, pRequestPC->m_desc);
			}
			pExped->SetRequest(-1, "");
		}
		gserver.m_listExped.RemoveData(pExped);
		delete pExped;
	}
	else
	{
		// 원정대원 나감
		ExpedQuitRepMsg(rmsg, nTargetIndex, nQuitMode);
		if (pTargetPC)
			SEND_Q(rmsg, pTargetPC->m_desc);

		pExped->SendToAllPC(rmsg, nTargetIndex);

		// 보스 변경 알림(보스 변경 처리:자동 위임 처리 필요)
		if (nBossIndex == nTargetIndex)
		{
			//나가는 캐릭터가 보스이면 위임할 보스를 획득 함
			CExpedMember* pNewBossMember = (CExpedMember*) pExped->FindNextBoss();

			if(pNewBossMember)
			{
				pExped->ChangeBoss(pNewBossMember->GetCharIndex(),nBossIndex,false);	

				ExpedChangeBoss(rmsg, nBossIndex , pNewBossMember->GetCharIndex(),MSG_EXPED_CHANGEBOSS_AUTO);
				pExped->SendToAllPC(rmsg);
			}
		}
	}

}

//강퇴 
void ProcHelperExped_KickRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int	nBossIndex;
	int	nTargetIndex;
	int	nErrorCode;

	msg >> nBossIndex
		>> nTargetIndex
		>> nErrorCode;

	CExpedition* pExped = gserver.FindExpedByBossIndex(nBossIndex);
	if (!pExped)
	{
		return ;
	}

	CPC* pTargetPC = gserver.m_playerList.Find(nTargetIndex);
	if (pTargetPC && pTargetPC->m_Exped && pTargetPC->m_Exped == pExped)
	{
		pTargetPC->CancelDamageLink();
		pTargetPC->m_Exped = NULL;
	}

	pExped->SetMemberPCPtr(nTargetIndex, NULL);
	pExped->DeleteMember(nTargetIndex);

	if (nErrorCode == MSG_EXPED_ERROR_KICK_END)
	{
		// 파티 해체 알림
		ExpedMsg(rmsg, MSG_ENDEXPED_REP);

		if (pTargetPC)
			SEND_Q(rmsg, pTargetPC->m_desc);

		pExped->SendToAllPC(rmsg, nTargetIndex);

		// 초대 거절
		if (pExped->GetRequestIndex() > 0)
		{
			CPC* pRequestPC = gserver.m_playerList.Find(pExped->GetRequestIndex());
			if (pRequestPC)
			{
				ExpedMsg(rmsg, MSG_REJECT_SRC);
				SEND_Q(rmsg, pRequestPC->m_desc);
			}
			pExped->SetRequest(-1, "");
		}


		gserver.m_listExped.RemoveData(pExped);
		delete pExped;
	}
	else
	{
		// 파티원 나감
		ExpedKickMsg(rmsg, nTargetIndex);

		if (pTargetPC)
			SEND_Q(rmsg, pTargetPC->m_desc);

		pExped->SendToAllPC(rmsg, nTargetIndex);
	}
}


//대장 위임 
void ProcHelperExped_ChangeBossRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int	nBossIndex;
	int	nNewBossIndex,nChangeMode;

	msg >> nBossIndex
		>> nNewBossIndex
		>> nChangeMode;

	CExpedition* pExped = gserver.FindExpedByBossIndex(nBossIndex);
	if(!pExped)
	{
		return ;
	}

	pExped->ChangeBoss(nNewBossIndex,nBossIndex,true);

	ExpedChangeBoss(rmsg, nBossIndex, nNewBossIndex, nChangeMode);
	pExped->SendToAllPC(rmsg);
}

//타입변경 
void ProcHelperExped_ChangeTypeRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int nBossIndex;
	char cDiviType,cExpedType;
	msg >> nBossIndex
		>> cDiviType
		>> cExpedType;

	CExpedition* pExped = gserver.FindExpedByBossIndex(nBossIndex);
	if (!pExped)
	{
		return ;
	}

	pExped->SetExpedType(cDiviType,cExpedType);

	ExpedChangeType(rmsg, cDiviType, cExpedType);
	pExped->SendToAllPC(rmsg);
}

//부대장 임명
void ProcHelperExped_SetMBossRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int	nBossIndex, nNewMBossIndex;

	msg >> nBossIndex
		>> nNewMBossIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(nBossIndex);
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
			ExpedSetMbossMsg(rmsg, nNewMBossIndex);
			pExped->SendToAllPC(rmsg);	
		}
	}
}

//부대장 해임
void ProcHelperExped_ResetMBossRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int	nBossIndex, nNewMBossIndex;

	msg >> nBossIndex
		>> nNewMBossIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(nBossIndex);
	if(!pExped)
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
			ExpedResetMbossMsg(rmsg, nNewMBossIndex);
			pExped->SendToAllPC(rmsg);	
		}
	}
}

//원정대 해체
void ProcHelperExped_EndExpedRep(CNetMsg& msg)
{
	CNetMsg rmsg;

	int	nBossIndex;

	msg >> nBossIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(nBossIndex);
	if (!pExped)
	{
		return ;
	}

	//원정 대원 정보 삭제
	CExpedMember*  pMember = NULL;
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			 pMember =  (CExpedMember*) pExped->GetMemberByListIndex(i,j); 
			 if(pMember)
			 {
				 pExped->SetMemberPCPtr(pMember->GetCharIndex(), NULL);
				 pExped->DeleteMember(pMember->GetCharIndex());
			 }

			 pMember = NULL;
		}
	}	

	//삭제 
	gserver.m_listExped.RemoveData(pExped);
	delete pExped;

	// 파티 해체 알림
	ExpedMsg(rmsg, MSG_ENDEXPED_REP);
	pExped->SendToAllPC(rmsg, -1);

}

//그룹 이동
void ProcHelperExped_MoveGroupRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int nBossIndex, nSourceGroup, nMoveCharIndex, nTargetGroup, nTargetListindex;

	msg >> nBossIndex
		>> nSourceGroup
		>> nMoveCharIndex
		>> nTargetGroup
		>> nTargetListindex;

	CExpedition* pExped = gserver.FindExpedByBossIndex( nBossIndex);
	if (!pExped)
		return ;

	
	if(pExped->MoveGroup(nSourceGroup,nMoveCharIndex,nTargetGroup,nTargetListindex))
	{
		//성공
		ExpedMoveGroupRepMsg(rmsg, nSourceGroup, nMoveCharIndex, nTargetGroup, nTargetListindex);
		pExped->SendToAllPC(rmsg, -1);
	}
}


//표식 지정
void ProcHelperExped_SetLabelRep(CNetMsg& msg)
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
		CExpedition* pExped = gserver.FindExpedByBossIndex( nBossIndex);
		if (!pExped)
			return ;

		CExpedMember* pMember = NULL;
		pMember = (CExpedMember*) pExped->GetMemberByListIndex(pExped->FindMemberGroupIndex(nDestIndex), pExped->FindMemberListIndex(nDestIndex));
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
			ExpedSetLabelRepMsg(rmsg, nType,nMode,nLabel,nDestIndex);
			pExped->SendToAllPC(rmsg, -1);
		}
	}
}

//로그아웃 후 다시 접속
void ProcHelperExped_RejoinRep(CNetMsg& msg)
{
	CNetMsg rmsg;
	int nBossIndex, nJoinIndex;

	msg >> nBossIndex
		>> nJoinIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex( nBossIndex);
	if (!pExped)
		return ;

	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) pExped->GetMemberByCharIndex(nJoinIndex);
	if(pMember)
	{
		//정상 설정
		pMember->SetQuitType(MSG_EXPED_QUITMODE_NORMAL);
	}
}

//원정대 채팅
void ProcHelperExped_ChatRep(CNetMsg& msg)
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

	CExpedition* pExped = gserver.FindExpedByBossIndex(nBossIndex);
	if (pExped)
	{
		CNetMsg rmsg;
		SayMsg(rmsg, MSG_CHAT_EXPEDITION, nCharIndex, strName, "", strChat,nGroupTyp);

		if(nGroupTyp == -1)	 //전체 
		{
			pExped->SendToAllPC(rmsg);
		}else				 //그룹
		{
			pExped->SendToPCInSameGroup(nGroupTyp,rmsg);
		}	

	}
}


#endif //EXPEDITION_RAID