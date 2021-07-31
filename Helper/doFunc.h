#ifndef __DO_FUNCTION_H__
#define __DO_FUNCTION_H__

#include "DBCmd.h"

void do_Request(CNetMsg& msg, CDescriptor* dest);
void do_Reply(CNetMsg& msg, CDescriptor* dest);
void do_Command(CNetMsg& msg, CDescriptor* dest);

void do_CommandShutdown(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildCreateReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildOnline(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildLoadReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildLevelUpReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildBreakUpReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildBreakUpReq_real(CDescriptor* dest, int bossindex, int guildindex);
void do_CommandGuildMemberAddReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildMemberOutReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildMemberOutReq_real(CDescriptor* dest, int guildindex, int charindex, bool bSendError);
void do_CommandGuildMemberKickReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildChangeBossReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildAppointOfficerReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildChat(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildFireOfficerReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandCharacterDelete(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildBattleReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildBattleStopReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildBattlePeaceReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildBattleKillReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandEventMoonStoneUpdateReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandEventMoonStoneJackPotReq(CNetMsg& msg, CDescriptor* dest);

void do_CommandFriendMemberAddReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandFriendMemberDelReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandFriendSetConditionReq(CNetMsg& msg, CDescriptor* dest);
#ifdef MESSENGER_NEW
void do_CommandBlockCharReq(CNetMsg& msg, CDescriptor* dest);
#endif
#ifdef CASH_ITEM_GIFT
void do_CommandGiftCharReq(CNetMsg& msg, CDescriptor* dest);
#endif

// BW
void do_CommandPD4RankInit(CNetMsg& msg, CDescriptor* dest);
void do_CommandPD4RankAdd(CNetMsg& msg, CDescriptor* dest);
void do_CommandPD4RankEnd(CNetMsg& msg, CDescriptor* dest);
void do_CommandPD4RankView(CNetMsg& msg, CDescriptor* dest);
void do_CommandPD4RewardView(CNetMsg& msg, CDescriptor* dest);
void do_CommandPD4Reward(CNetMsg& msg, CDescriptor* dest);
void do_CommandPD4RewardInit(CNetMsg& msg, CDescriptor* dest);
void do_CommandPD4RewardEnd(CNetMsg& msg, CDescriptor* dest);

void do_CommandNameChange(CNetMsg& msg, CDescriptor* dest);
// 애완동물
void do_CommandPetCreateReq(CNetMsg& msg, CDescriptor* dest);


#ifdef NEW_UI
void do_CommandPetDeleteReq(CNetMsg& msg, CDescriptor* dest);
#endif // NEW_UI

// 길드 나스 보관함
void do_CommandGuildStashHistoryReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildStashViewReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildStashTakeReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildStashRollback(CNetMsg& msg, CDescriptor* dest);
void do_CommandGuildStashSaveTaxReq(CNetMsg& msg, CDescriptor* dest);

// 명성 관련
void do_CommandFameUp(CNetMsg& msg, CDescriptor* dest);
void do_CommandTeacherRegister(CNetMsg& msg, CDescriptor* dest);
void do_CommandTeacherGiveup(CNetMsg& msg, CDescriptor* dest);
bool do_TeachCancel(CDBCmd cmd, int teacheridx, int studentidx, int cancelflag = CANCELTEACHER | CANCELSTUDENT);
//선생 리스트 
void do_CommandTeacher(CNetMsg& msg, CDescriptor* dest);
void do_CommandTeacherTimeover(CNetMsg& msg, CDescriptor* dest);
void do_CommandTeacherSuperstoneRecieve(CNetMsg& msg, CDescriptor* dest);
void do_CommandTeacherLoad(CNetMsg& msg, CDescriptor* dest);

void do_CommandMargadumPvpRank(CNetMsg& msg, CDescriptor* dest);
//휴면케릭
void do_CommandEventSearchFriendMemberAddReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandEventSearchFriendMemberSelectAddReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandEventSearchFriendMemberOneTimeCheckReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandEventSearchFriendMemberListGoodReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandEventSearchFriendMemberGoodReq(CNetMsg& msg, CDescriptor* dest);

void do_CommandPartyInviteReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyInviteRep(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyAllowReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyRejectReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyQuitReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyKickReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyChangeBossReq(CNetMsg& msg, CDescriptor* dest);

#ifdef NEW_UI
void do_CommandPartyChangeTypeReq(CNetMsg& msg, CDescriptor* dest);
#endif // NEW_UI

#ifdef PARTY_RAID
void do_CommandPartyEndPartyReq(CNetMsg& msg, CDescriptor* dest);
#endif //PARTY_RAID


#ifdef PARTY_MATCHING
void do_CommandPartyMatchRegMemberReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyMatchRegPartyReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyMatchDelReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyMatchDelReq_real(int nSubNo, int nCharIndex);
void do_CommandPartyMatchInviteReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyMatchJoinReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyMatchJoinAllowReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyMatchJoinRejectReq(CNetMsg& msg, CDescriptor* dest);
void do_CommandPartyMatchMemberChangeInfoName(CNetMsg& msg, CDescriptor* dest);
#endif // PARTY_MATCHING

#ifdef LC_HBK
void do_LevelUpLog(CNetMsg & msg, CDescriptor * dest);
#endif // LC_HBK

#ifdef GUILD_RANKING
void do_CommandSaveExpGuild(CNetMsg& msg, CDescriptor* dest);
#endif // GUILD_RANKING

#ifdef EVENT_2007_PARENTSDAY
void do_CommandParentsdayAccumulatepointList( CNetMsg& msg, CDescriptor* dest );
void do_CommandParentsdayAddPoint( CNetMsg& msg, CDescriptor* dest );
void do_CommandParentsdayExchangeTicket( CNetMsg& msg, CDescriptor* dest );
void do_CommandParentsdayExchangeItem( CNetMsg& msg, CDescriptor* dest );
void do_CommandParentsdayItemInsert( CNetMsg& msg, CDescriptor* dest );
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_CHILDRENSDAY_2007
void do_CommandChildrensDay2007(CNetMsg& msg, CDescriptor* dest);
#endif //EVENT_CHILDRENSDAY_2007

#ifdef EVENT_TEACH_2007
void do_CommandTeach2007(CNetMsg & msg, CDescriptor * dest);
void do_CommandTeach2007AddFlower(CNetMsg & msg, CDescriptor * dest);
void do_CommandTeach2007ReciveFlower(CNetMsg & msg, CDescriptor * dest);
#endif // EVENT_TEACH_2007

#ifdef EVENT_FLOWERTREE_2007
void do_CommandFlowerTree2007( CNetMsg& msg, CDescriptor* dest );
#endif //EVENT_FLOWERTREE_2007

#ifdef NEW_GUILD
void do_CommandGuildInclineEstablish( CNetMsg& msg, CDescriptor* dest );
void do_CommandGuildMemberAdjust( CNetMsg& msg, CDescriptor* dest );
void do_CommandNewGuildInfo( CNetMsg& msg, CDescriptor* dest );
void do_CommandNewGuildMemberList( CNetMsg& msg, CDescriptor* dest );
void do_CommandNewGuildManage( CNetMsg& msg, CDescriptor* dest );
void do_CommandNewGuildNotice( CNetMsg& msg, CDescriptor* dest );
void do_CommandNewGuildNoticeUpdate( CNetMsg& msg, CDescriptor* dest );
void do_CommandNewGuildNoticeTrans( CNetMsg& msg, CDescriptor* dest );
void do_CommandNewGuildSkillList( CNetMsg& msg, CDescriptor* dest );
void do_CommandSaveGuildPoint(CNetMsg& msg, CDescriptor* dest);
void do_CommandSaveGuildMemberPoint(CNetMsg& msg, CDescriptor* dest);
///void do_CommandNewGuildSaveSkill(CNetMsg& msg, CDescriptor* dest);
void do_CommandNewGuildUpdatePoint( CNetMsg& msg, CDescriptor* dest );
void do_CommandGuildNotice( CNetMsg& msg, CDescriptor* dest );
void do_CommandGuildSkillLearn( CNetMsg& msg, CDescriptor* dest );
#endif // NEW_GUILD


void do_InsertGuard(CNetMsg & msg, CDescriptor * dest);
void do_CastleTowerAdd(CNetMsg & msg, CDescriptor * dest);
void do_CastleTowerReinforce(CNetMsg & msg, CDescriptor * dest);
void do_CastleQuartersInstall(CNetMsg & msg, CDescriptor * dest);

#ifdef EVENT_OPEN_ADULT_SERVER
void do_CommandOpenAdultServer(CNetMsg& msg, CDescriptor* dest);
#endif // EVENT_OPEN_ADULT_SERVER

#ifdef PET_NAME_CHANGE
void do_CommandPetChangeName( CNetMsg& msg, CDescriptor* dest );
#endif // PET_NAME_CHANGE

#ifdef PET_DIFFERENTIATION_ITEM
void do_CommandPetColorChange( CNetMsg& msg, CDescriptor* dest );
#endif // PET_DIFFERENTIATION_ITEM

#ifdef REWARD_IDC2007
void do_CommandRewardIDC2007( CNetMsg& msg, CDescriptor* dest );
#endif // REWARD_IDC2007

#ifdef EVENT_HALLOWEEN_2007
void do_CommandHalloween2007( CNetMsg& msg, CDescriptor* dest );
#endif // EVENT_HALLOWEEN_2007

#ifdef DYNAMIC_DUNGEON
void do_CommandDVDRateChange( CNetMsg& msg, CDescriptor* dest );
#endif //DYNAMIC_DUNGEON

#ifdef SAVE_MOONSTONE_MIX
void do_CommandSaveMoonStoneMix(CNetMsg& msg, CDescriptor* dest);
#endif // SAVE_MOONSTONE_MIX

#ifdef PET_TURNTO_NPC
void do_CommandPetTurnToNpc( CNetMsg& msg, CDescriptor* dest );
#endif //PET_TURNTO_NPC

#ifdef ALTERNATE_MERCHANT
void do_CommandMerchantStart( CNetMsg& msg, CDescriptor* dest );
void do_CommandMerchantBuy( CNetMsg& msg, CDescriptor* dest );
void do_CommandMerchantProductRecover( CNetMsg& msg, CDescriptor* dest );
void do_CommandMerchantNasRecovery( CNetMsg& msg, CDescriptor* dest );
void do_CommandMerchantEnd( CNetMsg& msg, CDescriptor* dest );
#endif // ALTERNATE_MERCHANT

#ifdef MONSTER_COMBO
void do_CommandCreateCombo( CNetMsg& msg, CDescriptor* dest );
#endif // MONSTER_COMBO

#ifdef ATTACK_PET
void do_CommandAttackPet( CNetMsg& msg, CDescriptor* dest );
#endif // ATTACK_PET

#ifdef EXTREME_CUBE
void do_CommandCubeStateReq( CNetMsg& msg, CDescriptor* dest );
void do_CommandCubeStatePersonalReq( CNetMsg& msg, CDescriptor* dest );
void do_CommandAddCubePoint( CNetMsg& msg, CDescriptor* dest );
void do_CommandAddCubePointPersonal( CNetMsg& msg, CDescriptor* dest );
#endif // EXTREME_CUBE

#ifdef EVENT_PHOENIX	// //피닉스 이벤트 yhj	
void do_CommandEventPhoenix( CNetMsg& msg, CDescriptor* dest );
#endif // EVENT_PHOENIX

#ifdef TRADE_AGENT		
void do_Command_TradeAgent_RegListReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_TradeAgent_RegReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_TradeAgent_RegCancelReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_TradeAgent_RegResult( CNetMsg& msg, CDescriptor* dest );
void do_Command_TradeAgent_SearchReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_TradeAgent_BuyReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_TradeAgent_CalcListReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_TradeAgent_CalculateReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_TradeAgent_CalcResult( CNetMsg& msg, CDescriptor* dest );
void do_Command_TradeAgent_CheckCalcReq( CNetMsg& msg, CDescriptor* dest );
#endif // TRADE_AGENT

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
void do_Command_DisconnectHackCharacter( CNetMsg& msg, CDescriptor* dest );
#endif // DISCONNECT_HACK_CHARACTER 

#ifdef EXPEDITION_RAID
void do_Command_Exped_CreateReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_InviteReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_AllowReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_RejectReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_QuitReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_KickReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_MemberInfoReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_ChangeTypeReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_ChangeBossReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_SetMBossReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_ResetMBossReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_EndExpedReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_MoveGroupReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_SetLabelReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_RejoinReq( CNetMsg& msg, CDescriptor* dest );
void do_Command_Exped_ChatReq( CNetMsg& msg, CDescriptor* dest );
#endif //EXPEDITION_RAID

#endif //__DO_FUNCTION_H__
