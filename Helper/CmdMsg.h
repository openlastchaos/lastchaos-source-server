#ifndef __CMDMSG_H__
#define __CMDMSG_H__

#include "Descriptor.h"

void FailMsg(CNetMsg& msg, MSG_FAIL_TYPE failtype);

void HelperWhisperNotfoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sidx, const char* sname);
void HelperWhisperRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sidx, const char* sname, const char* rname, const char* chat);
void HelperLogoutRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, const char* id);

void HelperGuildCreateRepMsg(CNetMsg& msg, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildCreateNotifyMsg(CNetMsg& msg, CGuild* guild);
void HelperGuildOnlineNotifyMsg(CNetMsg& msg, CGuildMember* member);
void HelperGuildMemberListMsg(CNetMsg& msg, CGuild* guild, int startidx, int cutCount );
void HelperGuildLoadRepMsg(CNetMsg& msg, const char* idname, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildLevelUpRepMsg(CNetMsg& msg, int guildindex, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildLevelUpNotifyMsg(CNetMsg& msg, int guildindex, int level);
void HelperGuildBreakUpRepMsg(CNetMsg& msg, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildBreakUpNotifyMsg(CNetMsg& msg, int guildindex);
void HelperGuildMemberAddRepMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildMemberAddNotifyMsg(CNetMsg& msg, int guildindex, int charindex, const char* name, MSG_GUILD_POSITION_TYPE pos);
void HelperGuildMemberOutRepMsg(CNetMsg& msg, int guildindex, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildMemberOutNotifyMsg(CNetMsg& msg, int guildindex, int charindex, const char* charname);
void HelperGuildMemberKickRepMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildMemberKickNotifyMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex);
void HelperGuildChangeBossRepMsg(CNetMsg& msg, int guildindex, int current, int change, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildChangeBossNotifyMsg(CNetMsg& msg, int guildindex, int current, int change);
void HelperGuildAppointOfficerRepMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildAppointOfficerNotifyMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex);
void HelperGuildFireOfficerRepMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildFireOfficerNotifyMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex);
void HelperGuildLoadNotifyMsg(CNetMsg& msg, CGuild* guild);

void HelperGuildBattleRepMsg(CNetMsg& msg, CGuild* g1, CGuild* g2, int prize);
void HelperGuildBattleStartMsg(CNetMsg& msg, CGuild* g1, CGuild* g2);
void HelperGuildBattleStopRepMsg(CNetMsg& msg, int winner_index, CGuild* g1, CGuild* g2);
void HelperGuildBattleStatusMsg(CNetMsg& msg, CGuild* g1, CGuild* g2);
void HelperGuildBattlePeaceRepMsg(CNetMsg& msg, CGuild* g);

void HelperEventMoonStoneLoadMsg(CNetMsg& mmsg, int moonstone);
void HelperEventMoonStoneUpdateRepMsg(CNetMsg& msg, int moonstone);
void HelperEventMoonStoneJackPotRepMsg(CNetMsg& msg, int moonstone, int chaindex);

void HelperFriendMemberAddRepMsg(CNetMsg& msg, int approvalindex, const char* appname, int appjob, int requestindex, const char* reqname, int reqjob, MSG_FRIEND_ERROR_TYPE errcode);
void HelperFriendMemberDelRepMsg(CNetMsg& msg, int removerindex, int deleteindex, MSG_FRIEND_ERROR_TYPE errcode);
void HelperFriendSetConditionNotifyMsg(CNetMsg& msg, int chaindex, int condition, int reply, int sum, const int* index);
//MSG_HELPER_NAME_CHANGE_REP,	// 이름 변경카드 사용 : bguild(c) charindex(n) nickname(str) errorcode(c)										
void HelperNameChangeRepMsg(CNetMsg& msg, char bguild, int charindex, const char* name, MSG_EX_NAMECHANGE_ERROR_TYPE error);
#ifdef MESSENGER_NEW
void HelperBlockCharRepMsg(CNetMsg& msg, int reqindex, int blockIndex, const char* blockName );
#endif

#ifdef CASH_ITEM_GIFT
//MSG_HELPER_GIFT_RECVCHARNAME_REP,	//: sendCharIndex(n) recvUserIdx(n) recvCharIndex(n) revCharName(str) msg(str) count(n) idx(n) ctid(n)
void HelperGiftCharRepMsg(CNetMsg& msg, int sendUserIdx, int sendCharIdx, int recvUserIdx, int recvCharIdx, const char* recvCharName, const char* sendMsg, int count, int idx[], int ctid[] );
#endif

// 애완동물
void HelperPetCreateRepMsg(CNetMsg& msg, int index, int owner, char typegrade);
// TODO : DELETE void HelperPetDeleteRepMsg(CNetMsg& msg, int index, int owner);

#ifdef NEW_UI
void HelperPetDeleteRepMsg(CNetMsg& msg, int index, int owner);
#endif // NEW_UI

// 길드 나스 보관함
void HelperGuildStashHistoryRepMsg(CNetMsg& msg, int charindex, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, int month[7], int day[7], LONGLONG money[7]);
void HelperGuildStashViewRepMsg(CNetMsg& msg, int charindex, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, LONGLONG money);
void HelperGuildStashTakeRepMsg(CNetMsg& msg, int guildindex, int charindex, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, LONGLONG money, LONGLONG balance);
void HelperGuildStashSaveTaxRepMsg(CNetMsg& msg, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, int guildindex, int zoneindex, LONGLONG taxItem, LONGLONG taxProduce);

void HelperFameupRepMsg(CNetMsg& msg, int teachidx, int studentidx, const char* studentname, int fameup);
void HelperTeacherInfo(CNetMsg& rmsg, const char* idname, int charindex, char m_teachType, int m_fame, char bteacher, int m_cntCompleteStudent, 
		int m_cntFailStudent, int m_superstone, int m_guardian, const char* strTeachIdx, const char* strTeachSec);

//등록한 케릭을 상태를 GameServer로 보냄.
void HelperEventSearchFriendMemberAddRepMsg(CNetMsg& msg, int approvalindex, const char reqnick[MAX_CHAR_NAME_LENGTH + 1],  MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode);
//휴면케릭이 리스트에서 선택한 결과 상태를 GameServer로 보냄.
void HelperEventSearchFriendMemberSelectAddRepMsg(CNetMsg& msg, int approvalindex,MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode);
//휴면케릭이 한시간 단위로 사냥한 시간을 저장하고 상태를 GameServer로 보냄.
void HelperEventSearchFriendMemberOneTimeCheckRepMsg(CNetMsg& msg, int timesec, int appDormantindex, MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode);
//휴면케릭 이벤트 등록된 친구 보상가능 여부 데이터를 GameServer로 보냄.
void HelperEventSearchFriendMemberListGoodRepMsg(CNetMsg& msg, int approvalindex, int* requestindex, const char requestnick[][MAX_CHAR_NAME_LENGTH + 1], int* itemgood, int request_listmember, int startindex, int nTotal);
//휴면케릭 보상지급 후 결과를 GameServer로 보냄.
void HelperEventSearchFriendMemberGoodRepMsg(CNetMsg& msg, int approvalindex, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TYPE errcode);

void HelperPartyInviteReqMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nTargetIndex, char nPartyType);
void HelperPartyInviteRepMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nTargetIndex, const char* strTargetName, char nPartyType, MSG_HELPER_PARTY_ERROR_TYPE errcode);
void HelperPartyAllowRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, const char* strTargetName, MSG_HELPER_PARTY_ERROR_TYPE errcode);
void HelperPartyRejectRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, MSG_HELPER_PARTY_ERROR_TYPE errcode);
void HelperPartyQuitRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, MSG_HELPER_PARTY_ERROR_TYPE errcode, int nNewBossIndex, const char* strNewBossName);
void HelperPartyKickRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, MSG_HELPER_PARTY_ERROR_TYPE errcode);
void HelperPartyChangeBossRepMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nNewBossIndex, const char* strNewBossName);

#ifdef NEW_UI
void HelperPartyChangeTypeRepMsg(CNetMsg& msg, int nBossIndex, char cPartyType,char cDiviType);
#endif // NEW_UI

#ifdef PARTY_RAID
void HelperPartyEndPartyRepMsg(CNetMsg& msg, int nBossIndex);
#endif //PARTY_RAID

#ifdef PARTY_MATCHING
void HelperPartyMatchRegMemberRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nCharIndex, const char* strCharName, int nCharLevel, int nZone, char cJob, char cPartyType);
void HelperPartyMatchRegPartyRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, const char* strBossName, int nBossLevel, int nZone, int nJobFlag, char cLimitLevel, char cPartyType, const char* strComment);
void HelperPartyMatchDelRepMsg(CNetMsg& msg, int nCharIndex, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode);
void HelperPartyMatchInviteRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName, char cPartyType);
void HelperPartyMatchInviteReqMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nCharIndex, char cPartyType);
void HelperPartyMatchJoinRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, char cPartyType, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName, char cCharJob);
void HelperPartyMatchJoinAllowRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, int nCharIndex, const char* strCharName);
void HelperPartyMatchJoinRejectRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nJoinCharIndex, int nRejectCharIndex);
void HelperPartyMatchMemberChangeInfoMsg(CNetMsg& msg, int nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_TYPE nType, const char* strCharName, int nLevel, int nZone);
#endif // PARTY_MATCHING

void HelperPartyInfoEndMsg(CNetMsg& msg);
void HelperPartyInfoPartyMsg(CNetMsg& msg, const CParty* pParty);
#ifdef PARTY_MATCHING
void HelperPartyInfoPartyMatchMemberMsg(CNetMsg& msg, const CPartyMatchMember* pMatchMember);
void HelperPartyInfoPartyMatchPartyMsg(CNetMsg& msg, const CPartyMatchParty* pMatchParty);
#endif // PARTY_MATCHING

#ifdef EVENT_CHILDRENSDAY_2007
void HelperChildrensDay2007Msg( CNetMsg& msg, int nCharIndex, int nItemIndex, int nNeedFlower , char cResult );
#endif //EVENT_CHILDRENSDAY_2007

#ifdef EVENT_FLOWERTREE_2007
void HelperFlowerTree2007Msg( CNetMsg& msg, MSG_HELPER_FLOWERTREE_2007_TYPE subtype ,int nCharIndex );
#endif //EVENT_FLOWERTREE_2007

#ifdef EVENT_2007_PARENTSDAY
void HelperEventAccmulatepointList( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE errorType, int CharIndex, int count,  int* Ranking, int* Accumulatepoint, const char GuildName[][MAX_CHAR_NAME_LENGTH+1] );
void HelperEventParentsdayAddPoint( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE errorType, int CharIndex, int CarnationCount,  int AccumulatePoint );
void HelperEventParentsdayExchangeTicket( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE errorType, int CharIndex, int GiftTicket );
void HelperEventParentsdayExchangeItem( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE errorType, int CharIndex, int GiftItemIndex , int GigtItemCount );
void HelperEventParentsdayGet( CNetMsg& msg, int CharIndex , int GiftItemIndex, int GiftItemCount );
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_TEACH_2007
void HelperTeach2007Addflower(CNetMsg & msg, int charidx);
#endif // EVENT_TEACH_2007

#ifdef NEW_GUILD
void HelperGuildInclineEstablishRepMsg( CNetMsg& msg, int guildindex, int charindex, char guildincline, MSG_GUILD_ERROR_TYPE errcode );
void HelperGuildMemberAdjustRepMsg( CNetMsg& msg, int guildindex, int ownerindex, int charindex, const char* strPositionName, int contributeExp, int contributeFame,  MSG_GUILD_ERROR_TYPE errcode );
void HelperNewGuildInfoRepMsg( CNetMsg& msg, int charindex, MSG_GUILD_ERROR_TYPE errorcode );
void HelperNewGuildInfoNotifyMsg( CNetMsg& msg, int charindex, int guildindex, int avelevel, int usepoint, MSG_GUILD_ERROR_TYPE errorcode  );
void HelperNewGuildMemberListRepMsg( CNetMsg& msg, int endcount, int & guildstart, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode,  int* membercharindex, int* cumulatePoint, const char CharName[][MAX_CHAR_NAME_LENGTH], const char positionName[][GUILD_POSITION_NAME+1], char* job, char* job2, int* level, int* position );
void HelperNewGuildManageRepMsg( CNetMsg& msg, int endcount, int & guildstart, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode,  int* membercharindex, int* contributeExp, int* contributeFame, const char CharName[][MAX_CHAR_NAME_LENGTH], const char positionName[][GUILD_POSITION_NAME+1], int* level, int* position, char first );

void HelperNewGuildNoticeRepMsg( CNetMsg& msg, int charindex, int guildindex, const char* title, const char* text);
void HelperNewGuildNoticeUpdateRepMsg( CNetMsg& msg, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode );
void HelperNewGuildNoticeTransMsg( CNetMsg& msg, int guildindex, const char* title, const char* text );
void HelperNewGuildNotice( CNetMsg& msg, int charindex, int guildindex, const char* title, const char* text );
void HelperNewGuildNoticeTransRep( CNetMsg& msg, int guildindex, int charindex, MSG_GUILD_ERROR_TYPE errcode );
void HelperNewGuildSkillListRepMsg( CNetMsg& msg, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode, int skillcount, int* skillindex, int* skillLevel );
void HelperExtendGuildLoadNotifyMsg( CNetMsg& msg, CGuild* guild, int skillcount, int* skillindex, int* skillLevel  );
void HelperExtendGuildMemberListMsg( CNetMsg& msg, int endcount, int & guildstart, CGuild* guild );
void HelperNewGuildPointUpdateMsg( CNetMsg& msg, int charindex, int guildindex, int guildpoint );
void HelperNewGuildMemberPointSaveMsg(CNetMsg& msg, int charindex, int guildindex, int memberpoint);
#endif // NEW_GUILD

#ifdef PET_NAME_CHANGE
void HelperPetNameChange( CNetMsg& msg, MSG_EX_PET_CHANGE_NAME_ERROR_TYPE err,int charidx, int petidx = 0, const char* strPetName = NULL );
#endif // PET_NAME_CHANGE

#ifdef GUILD_MARK_TABLE
void HelperGuildMarkTableMsg(CNetMsg & msg);
#endif // GUILD_MARK_TABLE

#ifdef REWARD_IDC2007
void HelperRewardIDC2007Msg( CNetMsg& msg, int userindex);
#endif // REWARD_IDC2007

#ifdef EVENT_HALLOWEEN_2007
void HelperHalloween2007Msg( CNetMsg& msg, int charIndex, unsigned char error );
#endif // EVENT_HALLOWEEN_2007

#ifdef DYNAMIC_DUNGEON
void HelperDVDRateChangeMsg( CNetMsg& msg, unsigned char subtype, int nRate );
#endif //DYNAMIC_DUNGEON

#ifdef ALTERNATE_MERCHANT
void HelperMerchantErrorMsg( CNetMsg& msg, MSG_EX_ALTERNATE_MERCHANT_ERROR_TYPE errorType, int CharIndex );
void HelperMerchantProductRecoverMsg( CNetMsg& msg, int nCharIndex, int nItemCount, int* pItemDBIndex, char strSerial[][50], int* pItemCount, int* pItemWearPos, 
		int* pItemFlag, int*  pItemPlus, short* pItemOption0, short* pItemOption1, short* pItemOption2, short* pItemOption3, short* pItemOption4 );

void HelperMerchantNasMsg( CNetMsg& msg, int nCharIndex, int nItemDBIndex, 
						  int nItemPlus, int nItemWearPos, int nItemFlag, const char* pSerial, int nItemCount, 
						  short nItemOption0, short nItemOption1, short  nItemOption2, short  nItemOption3, short nItemOption4 );

void HelperMerchantStartMsg( CNetMsg& msg, int nCharIndex );
#endif //ALTERNATE_MERCHANT

#ifdef IRIS_POINT
void HelperToConnAddIpointMsg( CNetMsg& msg, int user_index, MSG_CONN_IPOINT_TYPE type , int nIpoint );
#endif //IRIS_POINT

#ifdef ATTACK_PET
void HelperAttackPet( CNetMsg& msg, MSG_HELPER_APET_TYPE type, int charIndex );
#endif // ATTACK_PET

#ifdef EVENT_PHOENIX	// //피닉스 이벤트 yhj	
void HelperEventPhoenixMsg(CNetMsg& msg, MSG_HELPER_EVENT_PHOENIX_ERROR_TYPE nError, int nCharIdx );
#endif  // EVENT_PHOENIX

#ifdef TRADE_AGENT

typedef struct ItemOption
{
	int	m_nOptionCnt;

	int m_Option_type[MAX_ITEM_OPTION  + 1];
	int m_Option_level[MAX_ITEM_OPTION + 1];

	ItemOption(void)
	{
		m_nOptionCnt = 0;

		for(int i=0; i < MAX_ITEM_OPTION + 1; i++)
		{
			m_Option_type[i] = 0;
			m_Option_level[i] = 0;
		}
	}

}S_ITEMOPTION;


void HelperTradeAgentErrorMsg(CNetMsg& msg, MSG_TRADEAGENT_ERROR_TYPE errorType, int CharIndex,char errorPart);
void HelperTradeAgentRegListRepMsg(CNetMsg& msg, int nCharIndex, int nCharRegTotCount, int nMaxPageNo, int nPageNo, int nItemCount, int* pListindex, int* pItemSerial, int* pItemPlus, int* pItemFlag, S_ITEMOPTION* pItemOption,int* pQuantity,LONGLONG* ptotalmoney,int* pFinishDay,int* pFinishDayUnit);
void HelperTradeAgentRegRepMsg(CNetMsg& msg, int nCharIndex,char tab,char row,char col, int nitem_serial, CLCString item_serial2, CLCString item_name,int Quantity,LONGLONG TotalMoney,LONGLONG Guaranty,int nDbIndex);
void HelperTradeAgentRegCancelRepMsg(CNetMsg& msg, int nCharIndex,CLCString stItemname,int nItemCount);
void HelperTradeAgentSearchRepMsg(CNetMsg& msg, int nCharIndex,int nMaxPageNo, int nPageNo, int nItemCount, int* pListindex, int* pItemSerial,int* pItemPlus,int* pItemFlag,S_ITEMOPTION* pItemOption,int* pQuantity,LONGLONG* ptotalmoney,int* pLevel,CLCString* pSellCharname);
void HelperTradeAgentBuyRepMsg(CNetMsg& msg, int nCharIndex, int nSellcharindex,LONGLONG TotalMoney,CLCString stItemname,int nItemCount);
void HelperTradeAgentCalcListRepMsg(CNetMsg& msg, int nCharIndex, int nMaxPageNo, int nPageNo, int nItemCount, int* pItemSerial, int* pItemPlus, int* pItemFlag,S_ITEMOPTION *pItemOption,int* pQuantity,LONGLONG* ptotalmoney,int* pState,int* pPassDay,CLCString *pCharname);
void HelperTradeAgentCalculateRepMsg(CNetMsg& msg, int nCharIndex, int nMaxPageNo,int nPageNo,int nItemCount, int* pListindex, int* pItemSerial,CLCString* pItemSerial2, int* pItemPlus, int* pItemFlag, S_ITEMOPTION* pItemOption, int* pQuantity,LONGLONG* ptotalmoney,int* pState);
void HelperTradeAgentCheckCalcRepMsg(CNetMsg& msg, int nCharIndex, int nIsBeCalcdata);
void HelperTradeAgentReturnedMsg(CNetMsg& msg, int nSellcharindex,CLCString stItemName,int nItemCount);
#endif  //TRADE_AGENT

#ifdef EXPEDITION_RAID
void HelperExpedErrorMsg(CNetMsg& msg, unsigned char errorType, int CharIndex);
void HelperExpedCreateRepMsg(CNetMsg& msg, char nExpedType1,char nExpedType2,char nExpedType3, int nBossIndex,CLCString BossCharName,int nMemberCount,int *pCharIdex,CLCString* pCharName,int* pGroupType,int* pMemberType,int* pSetLabelType,int* pQuitType);
void HelperExpedInviteRepMsg(CNetMsg& msg, int nBossIndex, CLCString strBossName, int nTargetIndex, CLCString strTargetName,char cExpedType1,char cExpedType2,char cExpedType3);
void HelperExpedAllowRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, CLCString strTargetName,int nTargetGroup, int nTargetMember,int nTargetListIndex);
void HelperExpedRejectRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex);
void HelperExpedQuitRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, int nQuitMode, unsigned char errcode);
void HelperExpedKickRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, unsigned char errcode);
void HelperExpedChangeBossRepMsg(CNetMsg& msg, int nBossIndex,  int nNewBossIndex, int nChangeMode);
void HelperExpedChangeTypeRepMsg(CNetMsg& msg, int nBossIndex, char cDiviType, char cExpedType);
void HelperExpedEndExpedRepMsg(CNetMsg& msg, int nBossIndex);
void HelperExpedSetMBossRepMsg(CNetMsg& msg, int nBossIndex, int nNewMBossIndex);
void HelperExpedResetMBossRepMsg(CNetMsg& msg, int nBossIndex, int nNewMBossIndex);
void HelperExpedMoveGroupRepMsg(CNetMsg& msg,int nBossIndex,int nSourceGroup, int nMoveCharIndex, int nTargetGroup,int nTargetListindex);
void HelperExpedSetLabelRepMsg(CNetMsg& rmsg, int nBossIndex,int nType,int nMode,int nLabel,int nDestIndex);
void HelperExpedRejoinRepMsg(CNetMsg& msg, int nBossIndex, int nJoinIndex);
void HelperExpedChatRepMsg(CNetMsg& msg, int nBossIndex, int nCharIndex, CLCString strName,int nGroupTyp,CLCString strChat);
#endif //EXPEDITION_RAID

#endif	// __CMDMSG_H__
