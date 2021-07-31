#ifndef __ACMDMSG_H__
#define __ACMDMSG_H__

#include "Descriptor.h"
#include "Character.h"

#ifdef DRATAN_CASTLE
#include "DratanCastle.h"
#endif // DRATAN_CASTLE

#ifdef NEW_GUILD
#include "Guild.h"
#endif // NEW_GUILD

#ifdef NEW_SERIAL_PACKAGE_EVENT
#include "PromotionItem.h"
#endif //NEW_SERIAL_PACKAGE_EVENT

class CWarCastle;
class CDratanCastle;

// 캐릭터 관련
void AppearMsg(CNetMsg& msg, CCharacter* ch, bool bNew = false);
void DisappearMsg(CNetMsg& msg, CCharacter* ch);
void StatusMsg(CNetMsg& msg, CPC* ch);
void AtMsg(CNetMsg& msg, CPC* ch);
void ExpSPMsg(CNetMsg& msg, LONGLONG exp, int sp);

#ifdef FEEITEM
void NameChangeRepMsg(CNetMsg& msg, MSG_EX_NAMECHANGE_ERROR_TYPE type, const char* name, char bguild);
void CashItemPurchaseRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalance);
void CashItemBringRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, bool bPresent);
void CashItemPurchaseListRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int count, int idx[], int ctid[]);
void CashItemBalanceRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalace);
#ifdef LIMIT_CATALOG
void CashItemLimitCatalAddMsg(CNetMsg& msg);
#endif // LIMIT_CATALOG
#ifdef CASH_ITEM_COUPON
void CashItemCouponListRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int nCouponCount, CASH_COUPON* pCoupon );
void CashItemPurchaseByCouponRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalance);
#endif //CASH_ITEM_COUPON

void NameChangeFriendMsg(CNetMsg& msg, int index, const char* name, char job );
void NameChangeGuildMemberMsg(CNetMsg& msg, int index, const char* name, char job );

void CashItemMoonstoneStartRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_MOONSTONE_ERROR_TYPE errorcode, char grade);
void CashItemGiftRecvNoticeRepMsg(CNetMsg& msg, char bGiftExist);
void CashItemGiftSendRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errCode);
void CashItemGiftHistoryRepMsg(CNetMsg& msg, bool bSend, MSG_EX_CASHITEM_ERROR_TYPE errCode );
void CashItemGiftRecvListRepMsg(CNetMsg& msg, unsigned char listflag, CNetMsg recvMsg);
#endif // FEEITEM
// 변신 관련
void ChangeErrMsg(CNetMsg& msg, MSG_CHANGE_ERR_TYPE type);
void ChangeStartMsg(CNetMsg& msg, CPC* ch);
void ChangeStopMsg(CNetMsg& msg, CPC* ch);


// 공격 관련
void DamageMsg(CNetMsg& msg, CCharacter* ch, CCharacter* tch, MSG_DAMAGE_TYPE damageType, int skillidx, int damage, char flag);


// Inventory
bool InventoryMsg(CNetMsg& msg, CPC* ch, char resultArrange, int tab, int row);


// Wearing
// 050223 : bs : plus 효과를 위한 정보 추가
void WearingMsg(CNetMsg& msg, CPC* ch, char wearpos, int item_db_index, int item_plus);
// --- 050223 : bs : plus 효과를 위한 정보 추가


// Item 관련
void ItemNotUseMsg(CNetMsg& msg, MSG_ITEM_USE_ERROR_TYPE error);
void ItemProlongMsg(CNetMsg& msg, int itemindex, int useTime, bool bprolong);
void ItemUseMsg(CNetMsg& msg, CPC* ch, char tab, char row, char col, int itemindex, int extra);
void ItemTakeMsg(CNetMsg& msg, CCharacter* ch, CItem* item);
void ItemDeleteMsg(CNetMsg& msg, CItem* item);
void ItemWearMsg(CNetMsg& msg, char wear_pos, CItem* item1, CItem* item2);
void ItemSwapMsg(CNetMsg& msg, char tab_idx, char row_idx1, char col_idx1, int item_idx1, char row_idx2, char col_idx2, int item_idx2);
void ItemAddMsg(CNetMsg& msg, CItem* item);
void ItemUpdateMsg(CNetMsg& msg, CItem* item, LONGLONG changeCount);
void ItemDropMsg(CNetMsg& msg, CCharacter* ch, CItem* item);
void ItemAppearMsg(CNetMsg& msg, CItem* item);
void ItemDisappearMsg(CNetMsg& msg, CItem* item);
void ItemUpgradeRepMsg(CNetMsg& msg, char result);
void ItemRefineRepMsg(CNetMsg& msg, char result);
void ItemOptionAddRepMsg(CNetMsg& msg, CItem* item, MSG_ITEM_OPTION_ADD_RESULT result);
void ItemOptionDelRepMsg(CNetMsg& msg, CItem* item, MSG_ITEM_OPTION_DEL_RESULT result);
void ItemProcessRepMsg(CNetMsg& msg, MSG_ITEM_PROCESS_RESULT result);
void ItemMakeRepMsg(CNetMsg& msg, MSG_ITEM_MAKE_RESULT result);
void ItemMixRepMsg(CNetMsg& msg, MSG_ITEM_MIX_RESULT result);
void ItemArcaneRepMsg(CNetMsg& msg, MSG_ITEM_ARCANE_RESULT result);
void ItemGetMsg(CNetMsg& msg, CPC* ch, CItem* item);
#ifdef ENABLE_CHANGEJOB
void ItemChangeWeaponRepMsg(CNetMsg& msg, int olddbindex, int newdbindex);
#endif
void ItemProcessNPCMsg(CNetMsg& msg, int itemdbindex, int count, MSG_ITEM_PROCESS_NPC_ERROR_TYPE errcode);
void ItemAddBoosterMsg(CNetMsg& msg, MSG_ITEM_ADD_BOOSTER_ERROR_TYPE errcode);
#ifdef ENABLE_WAR_CASTLE
void ItemMixWarItemMsg(CNetMsg& msg, MSG_ITEM_MIX_WARITEM_ERROR_TYPE errcode);
#endif

#ifdef LEVELDOWN_SCROLL
void ItemLevelDownErrMsg(CNetMsg& msg, MSG_ITEM_LEVELDOWN_ERROR_TYPE errcode);
#endif	// LEVELDOWN_SCROLL

#ifdef ATTACK_PET // 공격형 펫 관련
void ExAPetStatusMsg(CNetMsg& msg, CAPet* pet);
void ExAPetFuntionMsg( CNetMsg& msg, MSG_EX_ATTACK_PET_ERRORTYPE subtype, CAPet* apet, unsigned char error );
void HelperAttackPetMsg( CNetMsg& msg , unsigned char subtype, int ownerIndex );
void ItemAPetWearMsg( CNetMsg& msg, char wear_pos, CItem* item1, CItem* item2 );
void ItemAPetWearingMsg( CNetMsg& msg, CAPet* apet );
void AddItemExAPetMsg( CNetMsg& msg, CItem* addItem );
void ExApetSellInfo( CNetMsg& msg, CAPet* apet );

#ifdef APET_AI
void APetAIListMsg(CNetMsg& msg, CAPet* apet);
void APetAIOnOffMsg(CNetMsg& msg, CAPet* apet, unsigned char ucErrMsg);
#endif //APET_AI
#endif //ATTACK_PET




void BillItemListReqMsg(CNetMsg& msg, int userindex, int charindex);
// 채팅관련
void ChatWhisperMsg(CNetMsg& msg, int sindex, const char* sname, const char* rname, const char* chat);
void SayMsg(CNetMsg& msg, MSG_CHAT_TYPE chatType, int sender_index, const char* sender_name, const char* receiver_name, const char* chat,int nGroup=-1);


// 이동관련
void GoZoneMsg(CNetMsg& msg, int zone, int extra, const char* ip, int port);
void MoveMsg(CNetMsg& msg, MSG_MOVE_TYPE movetype, CCharacter* ch);
void GotoMsg(CNetMsg& msg, CPC* ch, char bHack = 0);


// DB 관련
#ifdef FEEITEM
void DBCharEndMsg(CNetMsg & msg, int remain0, int remain1, unsigned int nSeed);
#else
void DBCharEndMsg(CNetMsg & msg);
#endif // FEEITEM

// 050223 : bs : plus 효과를 위한 정보 추가
void DBCharExistMsg(CNetMsg& msg, int index, const char* name, const char* nick, char job, char job2, char hairstyle, char facestyle, int level, LONGLONG ex, int sp, int hp, int maxhp, int mp, int maxmp, int wear[MAX_WEARING], int plus[MAX_WEARING], int nDeleteDelayRemain);
// --- 050223 : bs : plus 효과를 위한 정보 추가
void DBSuccessMsg(CNetMsg& msg);
void DBOKMsg(CNetMsg& msg, int zone);
void DBOtherServerMsg(CNetMsg& msg, int zone, const char* ip, int port);


// 메신저 관련
void MsgrConnectMsg(CNetMsg& msg, int version, int server, int subno, int count, int* zones);
void MsgrRepWhisperNotFoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex, const char* sname);
void MsgrRepWhisperRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrWhisperReqMsg(CNetMsg& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrGuildChatMsg(CNetMsg& msg, int seq, int server, int subno, int sindex, const char* sname, int guildindex, const char* chat);
void MsgrNoticeMsg(CNetMsg& msg, int seq, int server, int subno, int zone, const char* chat);
void MsgrLogoutReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, const char* id);
void MsgrPlayerCountReq(CNetMsg& msg, int server, int subno, int charidx);
void MsgrPlayerCountRep(CNetMsg& msg, int seq, int server, int subno, int zone, int charidx, int countZone, int* countPC, int* countProduce, int* countShop, int* countPlay);
void MsgrLogoutRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, const char* id);
void MsgrEndGameReqMsg(CNetMsg& msg);
void MsgrRebootReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone);
void MsgrObserverMsg(CNetMsg& msg, int seq, int server, int subno, int zone, const char* chat);

#ifdef NEW_DOUBLE_GM
void MsgrDoubleEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int percent[]);
void MsgrDoubleEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd);
#else
void MsgrDoubleEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, bool bStart);
#endif

void MsgrDoubleExpEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int expPercent);

void MsgrLattoEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex);
void MsgrNewYearEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex);
void MsgrValentineEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex);
void MsgrWhiteDayEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex);
void MsgrLetterEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex);
void MsgrDoubleExpEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int expPercent);
void MsgrEventReqMsg(CNetMsg& msg, int server, int subno, int zone, int thisServer, int thisSubno, int charIndex, const char* eventcmd, int flag);

void MsgrEventReqMsg(CNetMsg& msg, MSG_MSGR_TYPE msgType, int seq, int server, int subno, int zone, int charindex, int cmd, int drop);
void MsgrEventRepMsg(CNetMsg& msg, MSG_MSGR_TYPE msgType, int seq, int server, int subno, int zone, int charindex, int cmd, int drop);

#ifdef NEW_DOUBLE_EVENT_AUTO
void MsgrDoubleEventAutoRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char cmd, char state);
void MsgrDoubleEventAutoReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char cmd);
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef UPGRADE_EVENT
void MsgrUpgradeEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char cmd, int prob);
void MsgrUpgradeEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char cmd);
#endif // UPGRADE_EVENT

#ifdef MESSENGER_NEW
// 서브 타입은 MSG_MSGR_MESSENER 관련된것만
void MsgrMessengerChatMsg(CNetMsg& msg, MSG_MSGR_TYPE subtype, int makeCharIndex, int chatIndex, int chatColor, int charIndex, CLCString chat = "" );
void MsgrMessengerChatMsg(CNetMsg& msg, MSG_MSGR_TYPE subtype, int makeCharIndex, int chatIndex, int chatColor, const char* charName, CLCString chat = "" );
#endif // MESSENGER_NEW

#ifdef CHANCE_EVENT
void MsgrChanceEventReqMsg(CNetMsg & msg, int seq, int server, int subno, int zone, int charindex, int cmd, int slevel, int elevel, int percent[]);
void MsgrChanceEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd);
#endif // CHANCE_EVENT

#ifdef DOUBLE_PET_EXP
void MsgrDoublePetExpEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int PetExpPercent);
void MsgrDoublePetExpEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int PetExpPercent);
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
void MsgrDoubleAttackEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int AttackPercent);
void MsgrDoubleAttackEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int AttackPercent);
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
void MsgrEventDropItemReqMsg( CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char type, int npcidx, int itemidx, int prob, int thisServer, int thisSub);
void MsgrEventDropItemRepMsg( CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char type, int npcidx, int itemidx, int prob, int thisServer, int thisSub);
#endif // EVENT_DROPITEM

// 시스템 메시지
void FailMsg(CNetMsg& msg, MSG_FAIL_TYPE failtype);
void SysWhisperNotFoundMsg(CNetMsg& msg);
void SysFullInventoryMsg(CNetMsg& msg, char tab);
void SysMsg(CNetMsg& msg, MSG_SYS_TYPE systype);
void SysBloodItemMsg(CNetMsg& msg, int itemdbindex);
void SysEnableSuperStoneMsg(CNetMsg& msg, int fame);//0627
void NoviceNotifyMsg(CNetMsg& msg, const char* name);
void NoticeInfoMsg(CNetMsg& msg, int notice);//0704
void SysCannotDuplcationMsg(CNetMsg& msg, int newIndex, int curIndex);

// 환경관련
void EnvWeatherMsg(CNetMsg& msg, int zoneNo);
void EnvTimeMsg(CNetMsg& msg);
void EnvTaxChangeMsg(CNetMsg& msg);

// 교환 관련
void ExchangeReqMsg(CNetMsg& msg, MSG_EXCHANGE_REQ_TYPE reqType, CPC* src, CPC* dest);
void ExchangeItemMsg(CNetMsg& msg, MSG_EXCHANGE_ITEM_ACTION action, CItem* item, LONGLONG count);

// 장소 기억 관련
void MemPosListMsg(CNetMsg& msg, CPC* ch);
void MemPosWriteMsg(CNetMsg& msg, CPC* ch, int slot);

#ifdef PRIMIUM_MEMORYBOOK
void MemPosPlusListMsg(CNetMsg& msg, CPC* ch);
void MemPosPlusWriteMsg(CNetMsg& msg, CPC* ch, int slot);
#endif	// PRIMIUM_MEMORYBOOK

// 운영자 명령 관련
void GMWhoAmIMsg(CNetMsg& msg, CPC* ch);
void GMCommandMsg(CNetMsg& msg, const char* cmd);

// 파티 관련
void PartyInviteMsg(CNetMsg& msg, char nPartyType, int nBossIndex, const char* strBossName);
void PartyMsg(CNetMsg& msg, MSG_PARTY_TYPE subtype);
void PartyAddMsg(CNetMsg& msg, int nCharIndex, const char* strCharName, CPC* tch, char isboss);
void PartyDelMsg(CNetMsg& msg, bool bKick, int nTargetIndex);
void PartyInfoMsg(CNetMsg& msg, int nIndex, int nLevel, int nHP, int nMaxHP, int nMP, int nMaxMP, float x, float z, char nYlayer, int nZone);
void PartyErrorMsg(CNetMsg& msg, MSG_PARTY_ERROR_TYPE partyerror);
void PartyAssistInfoMsg(CNetMsg& msg, CPC* tch);
void PartyChangeBoss(CNetMsg& msg, const char* bossname, int nNewBossIndex, const char* newname, bool bMandate = false);

#ifdef NEW_UI
void HelperPartyChangeTypeReqMsg(CNetMsg& msg, int nBossIndex, char partytype, char cDiviType);
void PartyChangeType(CNetMsg& msg, char cPartyType,char cDiviType);
#endif // NEW_UI

#ifdef PARTY_RAID
void HelperPartyEndPartyReqMsg(CNetMsg& msg, int nBossIndex);
#endif //PARTY_RAID

// QuickSlot 관련
void QuickSlotAddMsg(CNetMsg& msg, CPC* ch, char page, char slot);
void QuickSlotListMsg(CNetMsg& msg, CPC* ch, char page);

// 싱글던전 - NPC 리젠관련
void NPCRegenMsg(CNetMsg& msg, CNPC* npc, int entityIndex);

// 컨넥터 관련
void ConnConnectMsg(CNetMsg& msg, int version, int server, int subno, int count, int* zones);
void ConnLoginMsg(CNetMsg& msg, CDescriptor* d);
void ConnLogoutMsg(CNetMsg& msg, const char* id);
void ConnPlayingMsg(CNetMsg& msg, CDescriptor* d, MSG_LOGIN_TYPE mode);
void ConnRebootReqMsg(CNetMsg& msg);
void ConnPreopenGiftMsg(CNetMsg& msg, int userindex, int charindex, bool bCancel);
void ConnLevelupPointMsg(CNetMsg& msg, int userindex, int point);
void ConnStashState(CNetMsg& msg, int userindex, int charindex);
void ConnStashCheckPassword(CNetMsg& msg, int userindex, int charindex, const char* pw);
void ConnStashChangePassword(CNetMsg& msg, int userindex, int charindex, const char* oldpw, const char* newpw);
void ConnStashSeal(CNetMsg& msg, int userindex, int charindex);

void ConnEvent2pan4panLetterReqMsg(CNetMsg& msg, int userindex, int charindex, MSG_EVENT_2PAN4PAN_WORD_TYPE wordtype);
void ConnEvent2pan4panLetterRollbackReqMsg(CNetMsg& msg, int userindex, int charindex, MSG_EVENT_2PAN4PAN_WORD_TYPE wordtype);

void ConnEvent2pan4panGoodsReqMsg(CNetMsg& msg, int userindex, int charindex);
void ConnEvent2pan4panGoodsCommitMsg(CNetMsg& msg, int userindex, int charindex, int count, int* goodindex);

void ConnEvent2pan4panBoxReqMsg(CNetMsg& msg, int userindex, int charindex);
void ConnEvent2pan4panBoxCommitMsg(CNetMsg& msg, int userindex, int charindex, int count, int* goodindex);

void ConnEvent2pan4panMoneyReqMsg(CNetMsg& msg, int userindex, int charindex, int itemindex);

#ifdef EVENT_PARTNER_CODENEO
void ConnEventPartnerCodeNEOReqMsg( CNetMsg& msg, int userindex, int createcharacter );
#endif

#ifdef RANKER_NOTICE
void ConnRankerReqMsg(CNetMsg& msg, int userindex, int charindex);
#endif

// 유료아이템 관련
void ConnCashItemPurchaseHistoryReq(CNetMsg& msg, int userindex, int charindex, int y, char m, char d);
void ConnCashItemPurchaselistReq(CNetMsg& msg, int userindex, int charindex);
// 역시 bPresent 이면 기프트 테이블 참조, 아니면 보통
void ConnCashItemBringReq(CNetMsg& msg, bool bPresent, int userindex, int charindex, int count, int idx[], int ctid[]);
void ConnCashItemPurchaseReqMsg(CNetMsg& msg, int userindex, const char* idname, int charindex, int cash, int count, int ctid[], int requestCash = -1);
void ConnCashItemBalanceReq(CNetMsg& msg, int userindex, const char* idname);
#ifdef CASH_ITEM_GIFT
void ConnCashItemPresentHistoryReq(CNetMsg& msg, bool bSend, int userindex, int charindex, int y, char m, char d);
void ConnCashItemGiftReqMsg(CNetMsg& msg, int sendUserIndex, int sendCharIndex, const char* sendCharName, const char* sendMsg, int recvuserIndex, int recvcharIndex, const char* recvcharName, int count, int idx[], int ctid[]);
void ConnCashItemGiftRecvListReqMsg(CNetMsg& msg, int userIndex, int charIndex );
#endif
#ifdef CASH_ITEM_COUPON
void ConnCashItemCouponListReq( CNetMsg& msg, int userindex, const char* idname );
void ConnCashItemPurchaseByCouponReqMsg(CNetMsg& msg, int userindex, const char* idname, int charindex, int couponID, int cash, int count, int ctid[] );
#endif // CASH_ITEM_COUPON



/////////////////////////////////////////////
// BANGWALL : 2005-07-05 오후 2:36:09
// Comment : 태섭적용
void ConnStashDeletePassword(CNetMsg& msg, int userindex, int charindex, const char* idnum);
void ConnStashSetNewPassword(CNetMsg& msg, int userindex, int charindex, const char* newpw, const char* solpw = NULL);

///////////////////////////////////////////////
// WHS25 : 2006-11-15 오후 9 : 35
// Comment : 미국 오픈 베타 무기 지급 이벤트
#ifdef EVENT_OPEN_BETA_GIFT
void ConnOpenBetaGiftMsg(CNetMsg& msg, int userindex, int charindex/*, bool bCancel*/);
void EventOpenBetaGiftMsg(CNetMsg& msg );
#endif // EVENT_OPEN_BETA_GIFT


// 스킬 관련
void SkillListMsg(CNetMsg& msg, CPC* ch);
void SkillLearnErrorMsg(CNetMsg& msg, MSG_SKILL_LEARN_ERROR_TYPE errorcode);
void SkillLearnMsg(CNetMsg& msg, CSkill* skill);
void SkillReadyMsg(CNetMsg& msg, CCharacter* ch, CSkill* skill, CCharacter* tch);
void SkillCancelMsg(CNetMsg& msg, CCharacter* ch);
void SkillFireMsg(CNetMsg& msg, CCharacter* ch, CSkill* skill, CCharacter* tch, char count, char* mtargettype, int* mtargetindex, char cMoveChar, float x, float z, float h, float r, char yLayer);
void SkillErrorMsg(CNetMsg& msg, MSG_SKILL_ERROR_TYPE errcode, int skillindex, int extraInfo);
void SkillAutoUseMsg(CNetMsg& msg, int skillindex);

// 캐릭터 보조/저주 상태 관련
void AssistAddMsg(CNetMsg& msg, CCharacter* ch, int itemidx, int index, char level, int remain);
void AssistDelMsg(CNetMsg& msg, CCharacter* ch, int itemidx, int index);
void AssistListMsg(CNetMsg& msg, CPC* ch);

// 몹 상태 메시지
void CharStatusMsg(CNetMsg& msg, CCharacter* ch, int state2);

// 부활 msg
void RebirthMsg(CNetMsg& msg, CCharacter* ch);

// 기타 이펙트 메시지
void EffectEtcMsg(CNetMsg& msg, CCharacter* ch, MSG_EFFECT_ETC_TYPE effectType);
void EffectSkillMsg(CNetMsg& msg, CCharacter* ch, const CSkillProto* proto);
void EffectProduceMsg(CNetMsg& msg, CCharacter* ch, CCharacter* tch, MSG_PRODUCE_KIND kind);
void EffectItemMsg(CNetMsg& msg, CCharacter* ch, CItem* item);

// 퀘스트 msg
void QuestPCListMsg(CNetMsg& msg, CPC* pc);
void QuestNPCListMsg(CNetMsg& msg, int total, int* flag, int* index);
void QuestCreateListMsg(CNetMsg& msg, int count, int* index);
void QuestStartMsg(CNetMsg& msg, CQuest* quest);
void QuestCompleteMsg(CNetMsg& msg, CQuest* quest);
void QuestStatusMsg(CNetMsg& msg, CQuest* quest);
void QuestPrizeMsg(CNetMsg& msg, CQuest* quest);
void QuestGiveUpMsg(CNetMsg& msg, CQuest* quest);
void QuestFailMsg(CNetMsg& msg, CQuest* quest);
void QuestErrorMsg(CNetMsg& msg, MSG_QUEST_ERROR_TYPE type);

void QuestCompleteListMsg(CNetMsg& msg, CPC* pc);
void QuestAbandonListMsg(CNetMsg& msg, CPC* pc);
void QuestGiveUpMsg(CNetMsg& msg, int questIndex);
void QuestForceGiveUpMsg(CNetMsg& msg, CQuest* quest);
void QuestUnCompleteMsg(CNetMsg& msg, CQuest* quest);

void QuestPD4ErrorMsg(CNetMsg& smg, MSG_QUEST_PD4_ERROR_TYPE type);
// 스탯 포인트 관련
void StatPointRemainMsg(CNetMsg& msg, CPC* ch);
void StatPointUseMsg(CNetMsg& msg, CPC* ch, MSG_STATPOINT_USE_TYPE type, int value);
void StatPointErrorMsg(CNetMsg& msg, MSG_STATPOINT_ERROR_TYPE errcode);

// Action 관련
void ActionMsg(CNetMsg& msg, CPC* ch, char type, char action);

// 이동 장치 관련
void WarpStartMsg(CNetMsg& msg, CPC* ch);
void WarpCancelMsg(CNetMsg& msg, CPC* ch);
void WarpEndMsg(CNetMsg& msg, CPC* ch);
void WarpTeleportMsg(CNetMsg& msg, int pos, CPC* ch);
#ifdef ENABLE_WAR_CASTLE
void WarpPromptMsg(CNetMsg& msg, int zoneindex, int pos);
#endif

void WarpErrorMsg(CNetMsg& msg, MSG_WARP_ERROR_TYPE subtype, const char* name);
void WarpReqIngMsg(CNetMsg& msg, MSG_WARP_TYPE subtype);
void WarpReqMsg(CNetMsg& msg, CPC* ch, MSG_WARP_TYPE subtype);

// 특수 스킬 관련
void SSkillListMsg(CNetMsg& msg, CPC* ch);
void SSkillLearnErrorMsg(CNetMsg& msg, MSG_SSKILL_LEARN_ERROR_TYPE errorcode);
void SSkillLearnMsg(CNetMsg& msg, CSSkill* sskill);
void SSkillRemoveMsg(CNetMsg& msg, CSSkill* sskill);

// PK 관련 메시지
void PKItemSealMsg(CNetMsg& msg, CItem* item);
void PKRecoverItemSealMsg(CNetMsg& msg, CItem* item);
void PKErrorMsg(CNetMsg& msg, char errcode);
#ifdef FREE_PK_SYSTEM
void FreePKMsg(CNetMsg& msg);
#endif //FREE_PK_SYSTEM

// 이벤트 관련
void EventErrorMsg(CNetMsg& msg, MSG_EVENT_ERROR_TYPE errcode);
void EventPreopenGiftMsg(CNetMsg& msg, int itemdbindex);
void EventLattoMsg(CNetMsg& msg, MSG_EVENT_LATTO_TYPE type, int arg1 = -1, int arg2 = -1);
void EventLetterMsg(CNetMsg& msg, MSG_EVENT_LETTER_TYPE type);
void EventChangeWeaponMsg(CNetMsg& msg, int olddbindex, int newdbindex);
void EventMoonStoneMsg(CNetMsg& msg, MSG_EVENT_MOONSTONE_TYPE type, MSG_EVENT_MOONSTONE_CHANGE_TYPE subtype, int arg1 = -1, int arg2 = -1, int accumulate = 0);
void EventTreasureBoxMsg(CNetMsg& msg, MSG_EVENT_TREASUREBOX_TYPE type, int arg);
void EventSuperStoneMsg(CNetMsg& msg, MSG_EVENT_SUPERSTONE_TYPE type);//0627
void EventChangeWithoutOptionMsg(CNetMsg& msg, int olditem, int newsubtype);
void Event2pan4panLetterMsg(CNetMsg& msg, MSG_EVENT_2PAN4PAN_WORD_TYPE wordtype);
void Event2pan4panGoodsCheckMsg(CNetMsg& msg, int count);
void Event2pan4panGoodsRepMsg(CNetMsg& msg, MSG_EVENT_2PAN4PAN_GOODS_ERROR_TYPE errcode, int count, int* items, LONGLONG* itemcounts);
void Event2pan4panBoxCheckMsg(CNetMsg& msg, int count);
void Event2pan4panBoxRepMsg(CNetMsg& msg, MSG_EVENT_2PAN4PAN_BOX_ERROR_TYPE errcode, int count, int* items, LONGLONG* itemcounts);
void Event2pan4panMoneyRepMsg(CNetMsg& msg, int success);
#ifdef EVENT_PCBANG
void EventPCbangRepMsg(CNetMsg& msg, int index);
#endif

#ifdef EVENT_TREASUREBOX_RED
void EventTreasureBoxRedMsg(CNetMsg& msg, MSG_EVENT_TREASUREBOX_TYPE type, int arg);
#endif // EVENT_TREASUREBOX_RED

// 개인상점
void PersonalShopErrorMsg(CNetMsg& msg, MSG_PERSONALSHOP_ERROR_TYPE errcode);
void PersonalShopSellStartMsg(CNetMsg& msg, CPC* ch);
void PersonalShopSellListMsg(CNetMsg& msg, CPC* ch);
void PersonalShopChangeMsg(CNetMsg& msg, CPC* ch);
void PersonalShopBuyMsg(CNetMsg& msg, CNetMsg& buymsg, CPC* tch);

// 정당방위 관련
void RightAttackMsg(CNetMsg& msg, CPC* target, MSG_RIGHT_ATTACK_TYPE type);

// 창고 관련
void StashIsSetPassword(CNetMsg& msg, bool bisset);
void StashErrorMsg(CNetMsg& msg, MSG_STASH_ERROR_TYPE err);
void StashListRepMsg(CNetMsg& msg, const CItem* item, bool isstart, bool isend, bool isempty, int remain = 0);
void StashKeepRepMsg(CNetMsg& msg, int keepcount, int* itemdbindex, LONGLONG* count);
void StashTakeRepMsg(CNetMsg& msg, int takecount, int* itemdbindex, LONGLONG* count);
void StashChangePasswordRepMsg(CNetMsg& msg, bool bisset);
void StashSealMsg(CNetMsg& msg);

/////////////////////////////////////////////
// BANGWALL : 2005-07-05 오후 2:36:31
// Comment : 테섭적용

void StashSetNewPasswordRepMsg(CNetMsg& msg, char bsuccess);
void StashDeletePasswordRepMsg(CNetMsg& msg, char bsuccess);


// 길드 관련
void GuildErrorMsg(CNetMsg& msg, MSG_GUILD_ERROR_TYPE errcode);
void GuildInfoMsg(CNetMsg& msg, CPC* pc);
void GuildListMsg(CNetMsg& msg, CPC* pc);
void GuildOnlineMsg(CNetMsg& msg, CGuildMember* member);
void GuildLevelInfoMsg(CNetMsg& msg, int guildindex, int guildlevel);
void GuildBreakUpNotifyMsg(CNetMsg& msg, int guildindex, const char* guildname);
void GuildRegistCancelMsg(CNetMsg& msg, bool bTarget);
void GuildMemberAddMsg(CNetMsg& msg, int guildindex, int charindex, const char* name);
void GuildMemberOutMsg(CNetMsg& msg, int guildindex, int charindex, const char* name);
void GuildMemberKickMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex, const char* name);
void GuildInfoChangeMsg(CNetMsg& msg, int charindex, int guildindex, const char* name, MSG_GUILD_POSITION_TYPE pos);
void GuildChangeBossMsg(CNetMsg& msg, int guildindex, int current, int change);
void GuildAppointOfficerMsg(CNetMsg& msg, int guildindex, int charindex);
void GuildFireOfficerMsg(CNetMsg& msg, int guildindex, int charindex);

void GuildBattleReqReqMsg(CNetMsg& msg, int guildindex, const char* name, int prize, int time);
void GuildBattleReqRejectMsg(CNetMsg& msg, int reject_charindex);
void GuildBattleReqAccpetMsg(CNetMsg& msg, int guidindex1, const char* name1, int guildindex2, const char* name2, int prize, int zone, int time);
void GuildBattleStopReqMsg(CNetMsg& msg, int guildindex, const char* name);;
void GuildBattleStopRejectMsg(CNetMsg& msg, int reject_charindex);
void GuildBattleEndMsg(CNetMsg& msg, int winner_guildindex, int guidindex1, const char* name1, int guildindex2, const char* name2, int prize);
void GuildBattleStartMsg(CNetMsg& msg, int guidindex1, const char* name1, int guildindex2, const char* name2, int prize, int zone, int time);
void GuildBattleStatusMsg(CNetMsg& msg, int guildindex1, const char* name1, int killcount1, int guildindex2, const char* name2, int killcount2, int battletime, int battleZone);
void GuildBattleErrMsg(CNetMsg& msg, MSG_GUILD_EROOR_BATTLE_TYPE type);
#ifdef ENABLE_WAR_CASTLE
void GuildWarGetTimeMsg(CNetMsg& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle);
void GuildWarNoticeTimeMsg(CNetMsg& msg, int zoneindex, char month, char day, char hour, char min);
void GuildWarNoticeTimeRemainMsg(CNetMsg& msg, int zoneindex, char min);
void GuildWarNoticeStartMsg(CNetMsg& msg, int zoneindex, int remainSec);
void GuildWarJoinAttackGuildMsg(CNetMsg& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle);
void GuildWarJoinDefenseGuildMsg(CNetMsg& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle);
void GuildWarJoinAttackCharMsg(CNetMsg& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle);
void GuildWarJoinDefenseCharMsg(CNetMsg& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle);
void GuildWarPointMsg(CNetMsg& msg, CPC* pc, CWarCastle* castle);
void GuildWarNoticeStartCastleMsg(CNetMsg& msg, int zoneindex, int remainSec, int guildindex1, const char* guildname1, int guildindex2, const char* guildname2, int guildindex3, const char* guildname3);
void GuildWarNoticeRemainFieldTimeMsg(CNetMsg& msg, int zoneindex, int remainSec);
void GuildWarCastleStateMsg(CNetMsg& msg, int zoneindex, CPC* pc, CWarCastle* castle);
void GuildWarGateStateMsg(CNetMsg& msg, int oldstate, int newstate);
void GuildWarEndMsg(CNetMsg& msg, int zoneindex, char bWinDefense, int ownerindex, const char* ownername, int charindex, const char* charname, int nextMonth, int nextDay, int nextHour, int wDay);
void GuildWarSetTimeRepMsg(CNetMsg& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, char month, char day, char hour, char min);
void GuildWarInvalidCommandMsg(CNetMsg& msg, MSG_GUILD_TYPE reqtype);
void GuildStashHistoryRepMsg(CNetMsg& msg, MSG_GUILD_STASH_ERROR_TYPE errcode, int month[7], int day[7], LONGLONG money[7]);
void GuildStashViewRepMsg(CNetMsg& msg, MSG_GUILD_STASH_ERROR_TYPE errcode, LONGLONG money);
void GuildStashTakeRepMsg(CNetMsg& msg, MSG_GUILD_STASH_ERROR_TYPE errcode);
#endif // #ifdef ENABLE_WAR_CASTLE

#ifdef NEW_GUILD
void GuildInclineEstablishMsg( CNetMsg& msg, char guildincline ); 
void GuildNewInfo( CNetMsg& msg, CPC* ch, int avelevel, int guildpoint, int usepoint );
void GuildMemberListRep( CNetMsg& msg, int membercount, int* membercharindex, int* cumulatePoint, const char CharName[][MAX_CHAR_NAME_LENGTH], const char PositionName[][GUILD_POSITION_NAME+1], char* job, char* job2, int* level, int* position, CGuild* guild );
void GuildNewManageRep( CNetMsg& msg, int membercount, int* membercharindex, int* contributeExp, int* contributeFame, const char CharName[][MAX_CHAR_NAME_LENGTH], const char PositionName[][GUILD_POSITION_NAME+1], int* level, int* position,  CGuild* guild, char first );
void GuildNewNotify( CNetMsg& msg,const char* title, const char* text );
void GuildNewNotifyTrans( CNetMsg& msg, const char* guildname, const char* title, const char* text );
void GuildSkillListRepMsg( CNetMsg& msg, int skillcount, int* skillIndex, int* skillLearnLevel );
void GuildSkillLearnMsg(CNetMsg& msg, CSkill* skill);


void HelperGuildInclineEstablishReqMsg( CNetMsg& msg, CPC* ch, char guildincline );
void HelperGuildMemberAdjust( CNetMsg& msg, CPC* ch, int charindex, const char* strPositionName, int contributeExp, int contributFame );
void HelperNewGuildInfo( CNetMsg& msg, CPC* ch );
void HelperNewGuildMemberList( CNetMsg& msg, CPC* ch );
void HelperNewGuildManage( CNetMsg& msg, CPC* ch );
void HelperNewGuildNotice( CNetMsg& msg, CPC* ch );
void HelperNewGuildNoticeUpdate( CNetMsg& msg, CPC* ch, const char* title, const char* text );
void HelperNewGuildNoticeTrans( CNetMsg& msg, CPC* ch);
void HelperNewGuildSkillList( CNetMsg& msg, CPC* ch );
void HelperSaveGuildPointMsg(CNetMsg& msg, int guildindex, int guildpoint );
void HelperSaveGuildMemberPointMsg( CNetMsg& msg, int guildindex, int charindex, int GuildMemberPoint );
//void HelperSaveGuildSkillMsg( CNetMsg& msg, CGuild* guild );
void HelperNewGuildPointUpdate( CNetMsg& msg, int charindex, int guildindex, int point );
void HelperGuildLoadReq(CNetMsg& msg, int charindex );

void HelperGuildSkillLearnMsg( CNetMsg& msg, int gp, CGuild* guild  );

#ifdef NEW_GUILD_POINT_RANKING_NOTICE
void ConnGuildPointRankerReqMsg( CNetMsg& msg, int guildindex, int charindex );
void GuildPointRankingMsg( CNetMsg& msg, int nCharindex, int nGuildindex, int nRanking );
#endif //NEW_GUILD_POINT_RANKING_NOTICE

#endif // NEW_GUILD

// 헬퍼 관련
void HelperConnectMsg(CNetMsg& msg, int version, int server, int subno, int count, int* zones);
void HelperShutdownMsg(CNetMsg& msg);
void HelperRepWhisperNotFoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex, const char* sname);
void HelperRepWhisperRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex, const char* sname, const char* rname, const char* chat);

void HelperGuildCreateReqMsg(CNetMsg& msg, CPC* ch, const char* name);
void HelperGuildOnline(CNetMsg& msg, CGuildMember* member);
#ifdef NEW_GUILD
void HelperGuildLoadReq(CNetMsg& msg, const char* idname, int charindex, int channel, int zoneindex );
#else
void HelperGuildLoadReq(CNetMsg& msg, const char* idname, int charindex);
#endif // NEW_GUILD
void HelperGuildLevelUpReqMsg(CNetMsg& msg, int guildindex, int charindex);
void HelperGuildBreakUpReqMsg(CNetMsg& msg, CPC* boss);
void HelperGuildMemberAddReqMsg(CNetMsg& msg, int guildindex, int targetindex, int requester, const char* name);
void HelperGuildOutReqMsg(CNetMsg& msg, int guildindex, int charindex);
void HelperGuildKickReqMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex);
void HelperGuildChangeBossReqMsg(CNetMsg& msg, int guildindex, int current, int change);
void HelperGuildAppointOfficerReqMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex);
void HelperGuildChat(CNetMsg& msg, int guildindex, int charindex, const char* charname, const char* chat);
void HelperGuildFireOfficerReqMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex);
void HelperCharDelMsg(CNetMsg& msg, int charindex);

void HelperGuildBattleReqMsg(CNetMsg& msg, int guildindex1, int guildindex2, int prize, int zone, int time);
void HelperGuildBattleStopReqMsg(CNetMsg& msg, int guildindex1, int guildindex2);
void HelperGuildBattlePeaceReqMsg(CNetMsg& msg, CGuild* g);
void HelperGuildBattleKillReqMsg(CNetMsg& msg, int of_guildindex, int df_guildindex);
void HelperEventMoonStoneUpdateReqMsg(CNetMsg& msg);
void HelperEventMoonStoneJackPotReqMsg(CNetMsg& msg, int chaindex);

#ifdef ENABLE_WAR_CASTLE
void HelperWarNoticeTimeMsg(CNetMsg& msg, int zoneindex, char month, char day, char hour, char min);
void HelperWarNoticeRemainMsg(CNetMsg& msg, int zoneindex, char remain);
void HelperWarNoticeStartMsg(CNetMsg& msg, int zoneindex, int remainSec);
void HelperWarJoinAttackGuildMsg(CNetMsg& msg, int zoneindex, int guildindex);
void HelperWarJoinDefenseGuildMsg(CNetMsg& msg, int zoneindex, int guildindex);
void HelperWarNoticeStartAttackCastleMsg(CNetMsg& msg, int zoneindex, int remainSec, CGuild* g1, CGuild* g2, CGuild* g3);
void HelperWarNoticeRemainFieldTimeMsg(CNetMsg& msg, int zoneindex, int remainSec);
void HelperWarNoticeEndMsg(CNetMsg& msg, int zoneindex, char bWinDefense, int ownerindex, const char* ownername, int charindex, const char* charname, int nextMonth, int nextDay, int nextHour, int wDay);
void HelperGuildStashHistoryReqMsg(CNetMsg& msg, int guildindex, int charindex);
void HelperGuildStashViewReqMsg(CNetMsg& msg, int guildindex, int charindex);
void HelperGuildStashTakeReqMsg(CNetMsg& msg, int guildindex, int charindex, LONGLONG money);
void HelperGuildStashRollbackMsg(CNetMsg& msg, int guildindex, LONGLONG money);
void HelperGuildStashSaveTaxReqMsg(CNetMsg& msg, int guildIndex, int zoneindex, LONGLONG taxItem, LONGLONG taxProduct);
#endif // ENABLE_WAR_CASTLE

#ifdef ENABLE_PET
void HelperPetCreateReqMsg(CNetMsg& msg, int owner, char typeGrade);
// TODO : DELETE : 
#ifdef NEW_UI
void HelperPetDeleteReqMsg(CNetMsg& msg, int index, int owner);
#endif // NEW_UI
#endif // #ifdef ENABLE_PET

#ifdef FEEITEM
void HelperNameChangeReqMsg(CNetMsg& msg, char bguild, int index, const char* reqname);
#endif // FEEITEM
void HelperMargadumPvpRankReqMsg(CNetMsg& msg, int subno, int rank[], LONGLONG rankDamage[]);
void HelperTeachFameUpReqMsg(CNetMsg& msg, int teachidx, int studentidx, const char* studentname, int fame);
void HelperTeachRegisterMsg(CNetMsg& msg, int teachidx, bool bteacher, int studentidx);
void HelperTeacherGiveup(CNetMsg& msg, int teachidx, int studentidx);
void HelperTeachMsg(CNetMsg& msg, int teachidx, char bTeacher);
void HelperTeachTimeover(CNetMsg& msg, int flag, int teachidx, int studentidx);
void HelperTeacherSuperstoneRecieve(CNetMsg& msg, int charindex);
void HelperTeachLoadReq(CNetMsg& msg, const char* idname, int charindex);

#ifdef CASH_ITEM_GIFT
void HelperGiftCharReq(CNetMsg& msg, int sendUserIdx, int sendCharIdx, const char* recvCharName, const char* sendMsg, int count, int idx[], int ctid[] );
#endif

// 클라이언트 인터페이스 UI 관련 
void UIPlusEffectRep(CNetMsg& msg, CPC* ch);


// 사제 시스템 관련
void TeachTeacherListMsg(CNetMsg& msg, CPC* ch, MSG_TEACH_TEACHER_LIST_TYPE type, CPC** list);
void TeachTeacherReqReqMsg(CNetMsg& msg, CPC* ch);
void TeachTeacherReqRejectMsg(CNetMsg& msg, CPC* ch);
void TeachTeacherReqAcceptMsg(CNetMsg& msg, CPC* teacher, CPC* student);
void TeachEndMsg(CNetMsg& msg, int teacher_index, const char* teacher_name, int studetn_index, const char* student_name, MSG_TEACH_END_TYPE type);
void TeachInfoMsg(CNetMsg& msg, CPC* ch);
void TeachTeacherGiveUPMsg(CNetMsg& msg, CPC* teacher, CPC* student);
void TeachStudentLevelUPMsg(CNetMsg& msg, CPC* ch);
void TeachErrMsg(CNetMsg& msg, MSG_TEACH_ERR_TYPE type);

// 전직 관련
#ifdef ENABLE_CHANGEJOB
void ChangeJobErrorMsg(CNetMsg& msg, MSG_CHANGEJOB_ERROR_TYPE type);
void ChangeJobRepMsg(CNetMsg& msg, int nIndex, char job1, char job2);
void ChangeJobResetRepMsg(CNetMsg& msg, CPC* pc);
#endif // ENABLE_CHANGEJOB

#ifdef ENABLE_MESSENGER
void ChatMessengerMsg(CNetMsg& msg, int sindex, const char* sname, const char* rname, const char* chat);
void SysFriendNotFoundMsg(CNetMsg& msg);
void MsgrFriendChatReqMsg(CNetMsg& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrRepMessengerChatRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrRepMessengerNotFoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex, const char* sname);
void FriendErrorMsg(CNetMsg& msg, MSG_FRIEND_ERROR_TYPE errcode);
void FriendRegistCancelMsg(CNetMsg& msg);
void HelperFriendAddReqMsg(CNetMsg& msg, int approvalindex, const char* appname, int appjob, int requesterindex, const char* reqname, int reqjob);
void HelperFriendDelReqMsg(CNetMsg& msg, int approvalindex, int requester, const char* name);
void FriendSetConditionMsg(CNetMsg& msg, int requester, int condition);
void HelperFriendSetConditionMsg(CNetMsg& msg, int chaindex, int condition, int bReply, CPC* pc);
void MsgrMessengerChatReqMsg(CNetMsg& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat);
void FriendListMsg(CNetMsg& msg, CDescriptor* d);

#ifdef MESSENGER_NEW
void BlockPCListMsg(CNetMsg& msg, CDescriptor* d);

#ifdef MSG_VER2
void ChatOneMessengerMsg(CNetMsg& msg, int sindex, const char* sname, int rindex, const char* rname, int chatcolor, const char* chat);
void ChatOneOffLineMessengerMsg(CNetMsg & msg, int sindex, int rindex);

#endif // MSG_VER2

#endif // MESSENGER_NEW

void FriendAddNotify(CNetMsg& msg, CPC* pc);
void FriendDelNotify(CNetMsg& msg, int delindex);
void HelperBlockPCReqMsg(CNetMsg& msg, int approvalindex, int requester, const char* name, char bBlock);
void BlockPCRepMsg(CNetMsg& msg, MSG_EX_MESSENGER_ERROR_TYPE errcode, int charIndex, const char* name);
void do_ExMessenger(CPC* ch, CNetMsg& msg);

#endif // #ifdef ENABLE_MESSENGER

#ifdef ENABLE_WAR_CASTLE
void MsgrChatLordMsg(CNetMsg& msg, int index, const char* name, const char* chatmsg);
#endif // #ifdef ENABLE_WAR_CASTLE

#ifdef ENABLE_PET
// 펫관련
void ExPetStatusMsg(CNetMsg& msg, CPet* pet);
void ExPetMountMsg(CNetMsg& msg, int charindex, char pettype);
void ExPetLearnMsg(CNetMsg& msg, int skillindex, char level, MSG_EX_PET_LEARN_ERROR_TYPE errcode);
void ExPetSkillListMsg(CNetMsg& msg, CPet* pet);
void ExPetResetSkillMsg(CNetMsg& msg);
void ExPetSellInfoMsg(CNetMsg& msg, CPet* pet);
void ExPetChangeMountMsg(CNetMsg& msg, MSG_EX_PET_CHANGE_MOUNT_ERROR_TYPE errcode);
void ExPetCommandMsg(CNetMsg& msg, int petindex, int commandindex, char targettype, int targetindex);
void ExPetMixItemMsg(CNetMsg& msg, MSG_EX_PET_MIX_ITEM_ERROR_TYPE errcode);
void ExPetLevelupMsg(CNetMsg& msg, CPet* pet);
void ExPetRebirthMsg(CNetMsg& msg, int nPetIndex, MSG_EX_PET_REBIRTH_ERROR_TYPE errcode);
#endif // #ifdef ENABLE_PET

#ifdef ENABLE_WAR_CASTLE
void ExCastleMapRecentMsg(CNetMsg& msg, CWarCastle* castle, CPC* pc);
void ExCastleMapSignalMsg(CNetMsg& msg, MSG_GUILD_POSITION_TYPE flag, CPC* sender, float x, float z);
#endif // #ifdef ENABLE_WAR_CASTLE

void EventXMas2005Msg(CNetMsg& msg, MSG_EVENT_XMAS_2005_ERROR_TYPE errcode);
void EventNewYear2006CheckMsg(CNetMsg& msg, int timesec);
void EventNewYear2006GoodsMsg(CNetMsg& msg, MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_TYPE errcode);

#ifdef EVENT_SEARCHFRIEND
//휴면케릭 이벤트
//휴면케릭을 npc를 통해 등록하는 부분(Helper이동)
void EventHelperSearchFriendAddReqMsg(CNetMsg& msg, int approvalindex, const char* appnick, int appjob, int appuserindex, int appserverold, const char* reqnick);
//휴면케릭을 등록한 케릭의 리스트 상태를 보내는 부분   /등록한 케릭의 리스트 수 / 자신 인덱스, /상대 인덱스
void EventDormantSearchFriendSelectMsg(CNetMsg& msg, int listCount, int* requestindex, const char reqnick[][MAX_CHAR_NAME_LENGTH + 1], int startindex, int nTotal);
//휴면케릭이 리스트에서 한명을 선택한 결과를 등록하는 부분(Helper이동)
void EventHelperSearchFriendSelectAddReqMsg(CNetMsg& msg, int approvalindex, int requestindex);
//휴면 케릭 사냥시간 체크
void EventSearchFriendTimeCheckMsg(CNetMsg& msg, int timesec);
//휴면 케릭 사냥시간 한시간 단위로 저장(Helper이동)
void EventHelperSearchFriendOneTimeCheckReqMsg(CNetMsg& msg, int timesec, int appDormantindex);
//휴면 케릭들에게 받을 수 있는 아이템 보상여부 리스트를 갱신하기 위해 (Helper이동)
void EventHelperSearchFriendListGoodReqMsg(CNetMsg& msg, int approvalindex);
//휴면 케릭 아이템 보상여부 리스트 상태를 보내는 부분 
void EventSearchFriendListGoodsMsg(CNetMsg& msg, int request_listmember, int* request_index, const char reqnick[][MAX_CHAR_NAME_LENGTH + 1], int* item_good, int startindex, int nTotal);
//휴면 케릭 아이템 선택시 상태를 보내는 부분 
void EventSearchFriendGoodsMsg(CNetMsg& msg, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TYPE errcode);
//휴면 케릭 아이템 보상후 결과 (Helper이동)
void EventHelperSearchFriendGoodReqMsg(CNetMsg& msg, int approvalindex, int ndormantindex, int itemgood);
//휴면케릭 등록 진행 후 성공 여부를 클라이언트로 보내는 부분
void EventSearchFriendErrorMsg(CNetMsg& msg, MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode);
//휴면케릭이 리스트에서 한명을 선택한 결과 클라이언트로 보내는 부분
void EventSearchFriendSelectAddErrorMsg(CNetMsg& msg, MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode);
//휴면 케릭 사냥시간 한시간 단위로 저장 결과를 클라이언트로 보내는 부분
void EventSearchFriendOneTimeAddErrorMsg(CNetMsg& msg, int timesec, MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode);
#endif // #ifdef EVENT_SEARCHFRIEND

void ExElementalStatusMsg(CNetMsg& msg, CElemental* elemental);
void ExElementalDeleteMsg(CNetMsg& msg, CElemental* elemental);

void ExEvocationStartMsg(CNetMsg& msg, CPC* ch);
void ExEvocationStopMsg(CNetMsg& msg, CPC* ch);

void ExKeyChangeMsg(CNetMsg& msg, unsigned int nKey);

void ExPetChangeItemMsg(CNetMsg& msg, MSG_EX_PET_CHANGE_ITEM_ERROR_TYPE errcode);
void SysCannotWearDeadPetMsg(CNetMsg& msg, int nPetIndex, int nRemainRebirth);

void ExPartyRecallNoticeMsg(CNetMsg& msg);
void ExPartyRecallPromptMsg(CNetMsg& msg, int charindex, const char* charname);
void ExPartyRecallCancelMsg(CNetMsg& msg, int requestcharindex, const char* requestcharname, int targetcharindex, const char* targetcharname);

#ifdef NEW_SERIAL_PACKAGE_EVENT
void ConnCouponConfirm_new( CNetMsg& msg, int charindex, const char* serialKey );
void ConnCouponUse_new( CNetMsg& msg, int charindex, int promotionindex, int promotionType, int GiftItemCount, PromotionItem* pPromotionItem );
#endif // NEW_SERIAL_PACKAGE_EVENT

#ifdef EVENT_PACKAGE_ITEM
void ConnCouponConfirm(CNetMsg& msg, int charindex, const char* coupon);
#ifdef EVENT_PACKAGE_ITEM_DBLOG

#ifdef LC_HBK
void ConnCouponUse(CNetMsg& msg, int charindex, int cIndex, int type, CLCString idname, CLCString nick, int randidx1, int randcnt1, int randidx2, int randcnt2);
#else // LC_HBK
void ConnCouponUse(CNetMsg& msg, int charindex, int cIndex, int type, CLCString idname, CLCString nick);
#endif // LC_HBK

#else // EVENT_PACKAGE_ITEM_DBLOG

#ifdef LC_HBK
void ConnCouponUse(CNetMsg& msg, int charindex, int cIndex, int type, int randidx1, int randcnt1, int randidx2, int randcnt2);
#else
void ConnCouponUse(CNetMsg& msg, int charindex, int cIndex, int type);
#endif // LC_HBK
#endif // EVENT_PACKAGE_ITEM_DBLOG
#endif // EVENT_PACKAGE_ITEM

void HelperPartyMemberChangeJobMsg(CNetMsg& msg, int nBossIndex, int nCharIndex, char job1, char job2);
void HelperPartyChatMsg(CNetMsg& msg, int nBossIndex, int nCharIndex, const char* strName, const char* strChat);
void HelperPartyRecallPromptMsg(CNetMsg& msg, int nBossIndex, int nReqIndex, const char* strReqName, char cIsInCastle, int nGuildIndex);
void HelperPartyRecallConfirmMsg(CNetMsg& msg, int nBossIndex, int nReqIndex, const char* strReqName, int nRepIndex, const char* strRepName, char yesno);
void HelperPartyRecallProcMsg(CNetMsg& msg, int nBossIndex, int nCharIndex, int nZone, float x, float z, char nYlayer, char cIsInCastle, int nGuildIndex);

void HelperPartyInviteReqMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nTargetIndex, char partyType);
void HelperPartyInviteRepMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nTargetIndex, const char* strTargetName, char partyType, MSG_HELPER_PARTY_ERROR_TYPE errcode);
void HelperPartyAllowReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, const char* strTargetName);
void HelperPartyRejectReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex);
void HelperPartyQuitReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex);
void HelperPartyKickReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex);
void HelperPartyChangeBossReqMsg(CNetMsg& msg, int nBossIndex, const char* strTargetName);

void MsgrEventOXSetStartMsg(CNetMsg& msg, int nGMCharIndex, int yy, int mm, int dd, int hh, int min);
void MsgrEventOXEndMsg(CNetMsg& msg, int nGMCharIndex);
void MsgrEventOXQuizMsg(CNetMsg& msg, int nGMCharIndex, int nQuizNo, int nSec);

void EventOXQuizMsg(CNetMsg& msg, MSG_EVENT_OXQUIZ_TYPE subtype);
void EventOXQuizQuizMsg(CNetMsg& msg, int nQuizNo, int nSec, const char* strQuiz);
void EventOXQuizAnswerMsg(CNetMsg& msg, int nQuizNo, bool bAnswer);

#ifdef RECOMMEND_SERVER_SYSTEM
void MsgrRecommendMsg(CNetMsg& msg, int nGMCharIndex, int nRecommendServer);
void EventRecommendSetMsg(CNetMsg& msg);
void EventRecommendPotionMsg(CNetMsg& msg);
void MsgrRecommendMoonstoneMsg(CNetMsg& msg, int nCharIndex, const char* strCharName);
void EventRecommendMoonstoneMsg(CNetMsg& msg, int nCharIndex, const char* strCharName);
#endif // RECOMMEND_SERVER_SYSTEM

#ifdef PARTY_MATCHING
void ExPartyMatchRegMemberRepMsg(CNetMsg& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE errcode);
void ExPartyMatchRegPartyRepMsg(CNetMsg& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE errcode);
void ExPartyMatchMemberListRepMsg(CNetMsg& msg, int nPageNo, int nCharLevel, CLCList<CPartyMatchMember*>& list);
void ExPartyMatchPartyListRepMsg(CNetMsg& msg, int nPageNo, int nCharLevel, CLCList<CPartyMatchParty*>& list);
void HelperPartyMatchRegMemberReqMsg(CNetMsg& msg, int nCharIndex, const char* strCharName, int nLevel, int nZone, char nJob, char nPartyType);
void HelperPartyMatchRegPartyReqMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nZone, int nJobFlag, char cLimitLevel, const char* strComment);
void HelperPartyMatchDelReqMsg(CNetMsg& msg, int nCharIndex);
void ExPartyMatchDelRepMsg(CNetMsg& msg);
void HelperPartyMatchInviteReqMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nCharIndex);
void HelperPartyMatchInviteRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName, char cPartyType);
void ExPartyMatchInviteRepMsg(CNetMsg& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE nErrorCode, char cPartyType, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName);
void HelperPartyMatchJoinReqMsg(CNetMsg& msg, int nBossIndex, int nCharIndex, const char* strCharName, int nCharLevel, char cCharJob);
void ExPartyMatchJoinRepMsg(CNetMsg& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE nErrorCode, char cPartyType, int nBossIndex, const char* strBossName, int nReqCharIndex, const char* strReqCharName, char cReqCharJob);
void HelperPartyMatchJoinAllowReqMsg(CNetMsg& msg, int nBossIndex, int nCharIndex);
void HelperPartyMatchJoinRejectReqMsg(CNetMsg& msg, int nJoinCharIndex, int nRejectCharIndex);
void HelperPartyMatchMemberChangeInfoMsg(CNetMsg& msg, int nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_TYPE nType, const char* strCharName, int nLevel, int nZone);
#endif // PARTY_MATCHING
#ifdef LC_HBK
void HelperLevelUpLogMsg(CNetMsg & msg, CPC * pPc);
#endif // LC_HBK

void SysExpireItemcompositeMsg(CNetMsg& msg, int nItemDBIndex);

void EventWorldcupVoteMsg(CNetMsg& msg, int nCountry, MSG_EVENT_WORLDCUP_ERROR_TYPE nErrorCode);
void EventWorldcupGiftMsg(CNetMsg& msg, int nRank, MSG_EVENT_WORLDCUP_ERROR_TYPE nErrorCode);

void MsgrEventGoldenballVoteMsg(CNetMsg& msg, int nGMCharIndex, const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMinute);
void MsgrEventGoldenballGiftMsg(CNetMsg& msg, int nGMCharIndex, const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMinute, time_t timeEndVote);
void MsgrEventGoldenballEndMsg(CNetMsg& msg, int nGMCharIndex);
void EventGoldenballVoteStartMsg(CNetMsg& msg, const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMin);
void EventGoldenballVoteEndMsg(CNetMsg& msg, const char* strTeam1, const char* strTeam2);
void EventGoldenballGiftStartMsg(CNetMsg& msg, const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMin);
void EventGoldenballGiftEndMsg(CNetMsg& msg, const char* strTeam1, const char* strTeam2);
void EventGoldenballVoteMsg(CNetMsg& msg, int nTeam1Score, int nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_TYPE nErrorCode);
void EventGoldenballGiftMsg(CNetMsg& msg, MSG_EVENT_GOLDENBALL_ERROR_TYPE nErrorCode);

#ifdef EVENT_RAIN_2006
void EventRain2006Msg(CNetMsg& msg, MSG_EVENT_RAIN_2006_ERROR_TYPE nErrorCode);
#endif // EVENT_RAIN_2006

void ItemLendWeaponMsg(CNetMsg& msg, int nItemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_TYPE nErrorCode);
void SysSeparateItemMsg(CNetMsg& msg, int nItemDBIndex);

#ifdef EVENT_TLD_BUDDHIST
void EventTldBuddhistMsg(CNetMsg& msg, char cRequestType, MSG_EVENT_TLD_BUDDHIST_ERROR_TYPE nErrorCode);
#endif // EVENT_TLD_BUDDHIST

#ifdef EVENT_COLLECT_BUG
void EventCollectBugMsg(CNetMsg& msg, MSG_EVENT_COLLECT_BUG_TYPE nType, int nData);
#endif // EVENT_COLLECT_BUG

void QuestCollectMsg(CNetMsg& msg, int nNPCIndex, int nCharIndex, int nItemIndex);

#ifdef GUILD_RANKING
void HelperSaveExpGuildMsg(CNetMsg& msg, int nGuildIndex, double dExpGuild);
#endif // GUILD_RANKING

#ifdef EVENT_CHUSEOK_2006_GIFT
void EventChuseok2006Msg(CNetMsg& msg, MSG_EVENT_CHUSEOK_2006_TYPE nErrorcode);
#endif // EVENT_CHUSEOK_2006_GIFT

void MsgrLoginServerMsg(CNetMsg& msg, int nCode);

#ifdef MONSTER_RAID_SYSTEM
void SysRaidmobRegenMsg(CNetMsg& msg, int nNPCIndex, float x, float z);
#endif // MONSTER_RAID_SYSTEM

void ExHairChangeMsg(CNetMsg& msg, CPC* pPC );

void ExPlayerStateChangeMsg(CNetMsg& msg, CPC* pPC);

#ifdef EVENT_XMAS_2006
void EventXmas2006Msg(CNetMsg& msg, int nCakeCount, MSG_EVENT_XMAS_2006_ERROR_TYPE nErrorcode);
#endif // EVENT_XMAS_2006

#ifdef EVENT_WHITEDAY_2007
void EventWhiteday2007Msg(CNetMsg& msg, MSG_EVENT_WHITEDAY_2007_TYPE nMsg);
#endif

#ifdef EVENT_TLD_2007_SONGKRAN
void EventSongkran2007Msg( CNetMsg& msg, MSG_EVENT_SONGKRAN_2007_TYPE nMsg, int type );
#endif //EVENT_TLD_2007_SONGKRAN

#ifdef EVENT_CHILDRENSDAY_2007
void EventChildrensDay2007Msg(CNetMsg& msg, unsigned char nErrorCode);
void HelperChildrensDay2007Msg( CNetMsg& msg, MSG_HELPER_CHILDRENSDAY_2007_TYPE subtype , int nCharIndex, int nItemIndex );
#endif //EVENT_CHILDRENSDAY_2007

#ifdef EVENT_FLOWERTREE_2007
void EventFlowerTree2007Msg(CNetMsg& msg, MSG_EVENT_FLOWERTREE_2007_TYPE subtype );
void HelperFlowerTree2007Msg( CNetMsg& msg, MSG_HELPER_FLOWERTREE_2007_TYPE subtype, int nCharIndex );
#endif //EVENT_FLOWERTREE_2007

void SysTimeoutLuckyAccessoryMsg(CNetMsg& msg, int nItemDBIndex);
void SysChangeLuckyBoxMsg(CNetMsg& msg, int nItemDBIndex, int nUseDBIndex);



#if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 
void EventValentine2007ErrorMsg(CNetMsg& msg, MSG_EVENT_VALENTINE_TYPE nErrorcode);
void EventValentine2007Msg(CNetMsg& msg, MSG_EVENT_VALENTINE_TYPE nErrorcode, int extra);
#endif	// #if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 

void ConnGparaPromotionMsg(CNetMsg& msg, MSG_CONN_GPARA_PROMOTION_TYPE subtype, int nUserIndex, int nCharIndex);
void ConnOCN_GooPromotionMsg(CNetMsg& msg, MSG_CONN_OCN_GOO_PROMOTION_TYPE subtype, int nUserIndex, int nCharIndex);
void ConnMSNPromotionMsg(CNetMsg& msg, MSG_CONN_MSN_PROMOTION_TYPE subtype, int nUserIndex, int nCharIndex);

#ifdef CREATE_EVENT
void ConnCreateEventMsg(CNetMsg & msg, MSG_CONN_CREATEEVENT_TYPE subtype, int nUserIndex, int nCharIndex);
#endif // CREATE_EVENT

#ifdef EVENT_EGGS_HUNT_2007
void EventEggsHunt2007ErrorMsg(CNetMsg& msg, MSG_EVENT_EGGS_HUNT_2007_TYPE nMsg);
void EventEggsHunt2007GiftMsg(CNetMsg& msg, int nGiftIndex, int nGiftCount);
#endif // EVENT_EGGS_HUNT_2007

#ifdef EVENT_TEACH_2007
void EventTeach2007Msg(CNetMsg & msg, int value);
void HelperTeachFameUp2007ReqMsg( CNetMsg & msg, int charidx, int fame );
void HelperTeachAddFlowerMsg(CNetMsg & msg, int charidx);
void HelperReciveFlowerMsg(CNetMsg & msg, int charidx);
#endif // EVENT_TEACH_2007

#ifdef EVENT_GOMDORI_2007
void EventGomdori2007Msg(CNetMsg& msg, MSG_EVENT_GOMDORI_2007_TYPE nType);
void EventGomdori2007ResultMsg(CNetMsg& msg, MSG_EVENT_GOMDORI_2007_TYPE nResult, char cWinCount, char cUserSel, char cNPCSel);
void EventGomdori2007EndMsg(CNetMsg& msg, char cWinCount, char cPromptType, int nExtra1, int nExtra2);
void EventGomdori2007SelectGiftMsg(CNetMsg& msg, int nItemDBIndex);
void EventGomdori2007ViewStatusMsg(CNetMsg& msg, int nCount, int* nStatus);
void ConnEventGomdori2007CountMsg(CNetMsg& msg);
void ConnEventGomdori2007StatusMsg(CNetMsg& msg, int nCharIndex);
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_FLOWERTREE_2007
void ConnEventFlowerTree2007Msg(CNetMsg& msg, MSG_CONN_EVENT_FLOWERTREE_2007_TYPE subtype, int nCharIndex );
#endif //EVENT_FLOWERTREE_2007


#ifdef EVENTSETTING
void MsgEventSetReqMsg(CNetMsg& msg, int server, int subno, int zone, int thisServer, int thisSubno, int charIndex, int flag, int success);
void MsgEventCurrentRepMsg(CNetMsg& msg, int server, int subno, int zone, int thisServer, int thisSubno, int charIndex);
#endif // EVENTSETTING

#ifdef EVENT_2007_PARENTSDAY
void EventParentsdayAccumulateList( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE err, int count, int* ranking, int* point, const char guildname[][MAX_CHAR_NAME_LENGTH+1] );
void EventParentsdayAddPoint( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE err, int CarnationCaount, int entirePoint );
void EventParentsdayExchangeTicket( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE err, int TicketCount);
void EventParentsdayExchangeItem( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE err );
void EventParentsdayItemNotice( CNetMsg& msg, const char* guildName, CLCString& itemName );

void HelperEventParentsdayAccumulateList( CNetMsg& msg, int charIndex);
void HelperEventParentsdayAddPoint( CNetMsg& msg, int charIndex, int guildIndex, int CarnationCount);
void HelperEventParentsdayExchangeTicket( CNetMsg& msg, int charIndex , int guildIndex );
void HelperEventParentsdayExchangeItem( CNetMsg& msg, int charIndex , int guildIndex  );
void HelperEventParentsdayExchangeItemInsert( CNetMsg& msg, int charindex, int GiftItemIndex, int GiftItemCount );
#endif // EVENT_2007_PARENTSDAY


#ifdef GMTOOL
void MsgGmKickIDCommandReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int thisServer, int thisSubno, int charindex, const char* id);
void MsgGmKickIDCommandRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* id);
void MsgrGmToolChatMonitorReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int gmcharindex, const char* name, const char* onoff);
void MsgrGmToolChatMonitorRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, int gmcharindex, const char* id);
void MsgrNoticeGmChatMonitorMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int charindex, const char* name, unsigned char chattype);
void MsgrGmToolChatMonitorOffMsg(CNetMsg& rmsg, int seq, int server, int subno, int zone, int charindex);
void MsgrGmToolChatMonitorPartyOffMsg(CNetMsg& rmsg, int seq, int server, int subno, int zone, int bossindex);
void MsgrGmToolChatMonitorGuildOffMsg(CNetMsg& rmsg, int seq, int server, int subno, int zone, int guildindex);
void MsgrNoticeGmChatMonitorGuildMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int guildindex, const char* name, int charindex, const char* charname);
void MsgrNoticeGmChatMonitorPartyMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int bossindex, const char* name, int charindex, const char* charname);
void MsgrRepGMWhisperNotFoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex);
void MsgrGMWhisperReqMsg(CNetMsg& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrGMWhisperReqMsg(CNetMsg& msg, int seq, int server, int sub, const char* chat, int charindex, const char* name, int serverno, int subno, const char* receiver);
void MsgrNoticeGmChatMonitorWhisperMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int charindex, const char* name, unsigned char chattype, const char* sender);
void MsgLoopCountRep(CNetMsg& msg, int serverno, int subno, int thisServer, int thisSub, const char* loopbuf);
void MsgExtraGMCommandReq(CNetMsg& msg, int serverno, int subno, MSG_MSGR_TYPE msgtype);
void MsgGMSilenceReq(CNetMsg& msg, int seq, int serverno, int subno, int gmcharindex, const char* charname, int sec);
void MsgGMSilenceRep(CNetMsg& msg, int seq, int thisServer, int thisSub, int success, int gmcharindex, const char* name);
void MsgrRepGMToolWhisperNotFoundMsg(CNetMsg& msg, int nSeq, int nServer, int nSubno, int nZone, int charindex, const char* name);
void ChatGMWhisperMsg(CNetMsg& msg, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrRepGMWhisperRepMsg(CNetMsg& msg, int nSeq, int server, int subno, int zone, int index, const char*  name, const char* chat,  int thisServer, int thisSub, int sindex);
void GMSayMsg(CNetMsg& msg, int server, int sub, int charindex, const char* name, const char* chat, unsigned char chattype);
void MsgrRepGmToolWhisperRepMsg(CNetMsg& rmsg, int seq, int server, int sub, int zone, int sindex, const char* sname, const char* rname, const char *chat);
void ConnGMToolCommand(CNetMsg& msg, int gmindex, const char* gmname, const char* gmcommand);
void ConnGMToolChatMonitor(CNetMsg& msg, int charindex, const char* name, unsigned char chattype, int server, int sub, const char* chat);
void ConnGMToolChatting(CNetMsg& msg, int server, int sub, int gmindex, const char* gmname, const char* name, const char* chat);
void MsgGmKickCommandReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int thisServer, int thisSubno, int charindex, const char* name);
void MsgGmKickCommandRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* name);
void MsgrGMShutdownReqMsg(CNetMsg& msg, int server, int sub, int remain, const char* chat);
#endif // GMTOOL

#ifdef DRATAN_CASTLE
void HelperInsertGuardMsg(CNetMsg & msg, CPC * ch, CItem * item);
void InsertGuardMsg(CNetMsg & msg, CPC * ch, CItem * item);
void CastleCristalRespondStartMsg(CNetMsg & msg, CPC * ch);
void CastleCristalRespondEndMsg(CNetMsg & msg, CPC * ch);
void CastleCristalRespondFailMsg(CNetMsg & msg, CPC * ch);
void CastleTowerListMsg(CNetMsg & msg, CDratanCastle * pCastle);
void CastleTowerAddMsg(CNetMsg & msg, CDratanCastle * pCastle);
void HelperCastleTowerAddMsg(CNetMsg & msg, int zoneidx, CDratanCastle * pCastle);
void CastleTowerReinforceListMsg(CNetMsg & msg, char type, CDratanCastle * pCastle);
void CastleTowerReinforceMsg(CNetMsg & msg, char type, CDratanCastle * pCastle);
void HelperCastleReinforceMsg(CNetMsg & msg, char type, char step, int zoneidx);
void CastleTowerWarpListMsg(CNetMsg & rmsg, int * aliveindex, char total);
void CastleTowerRepaireListMsg(CNetMsg & msg, int idx, LONGLONG money);
void CastleTowerRepaireMsg(CNetMsg & msg, int idx, LONGLONG money);
void CastleTowerRebrithInstallMsg(CNetMsg & msg, int qindex, int gindex, const char * gname, int list_index);
void HeplerCastleRebrithInstallMsg(CNetMsg & msg, int qindex, int gindex, const char * ganme);
void CastleTowerQuartersCrushMsg(CNetMsg & msg, int index);
void CastletowerQuartersListMsg(CNetMsg & msg, CDratanCastle * pCastle);
void WaitTimeMsg(CNetMsg & msg, int wait_time);
void CastleErrorMsg(CNetMsg & msg, MSG_EX_CASTLE_ERROR_TYPE error);
void CastleErrorMsg(CNetMsg & msg, unsigned char error);
#endif // DRATAN_CASTLE

#ifdef EVENT_OPEN_ADULT_SERVER
void HelerOpenAdultServerMsg(CNetMsg& msg, int charindex, MSG_HELPER_OPEN_ADULT_SERVER_ERROR_TYPE error);
#endif // EVENT_OPEN_ADULT_SERVER

#ifdef GIFT_EVENT_2007
void ConnEventGift2007Msg(CNetMsg & msg, MSG_CONN_EVENT_GIFT_2007_TYPE subtype, CPC * pc);
void GiftEvent2007Msg(CNetMsg & rmsg, MSG_CONN_EVENT_GIFT_2007_TYPE nError);
#endif // GIFT_EVENT_2007

#if defined(GIFT_EVENT_2007) || defined(EVENT_OPEN_ADULT_SERVER) || defined( EVENT_MAGIC_CARD )
void EventOpenAdultServerMsg(CNetMsg& msg, MSG_EVENT_OPEN_ADULT_SERVER_TYPE type);
#endif //  GIFT_EVENT_2007 || EVENT_OPEN_ADULT_SERVER || EVENT_MAGIC_CARD

#ifdef GIFT_EVENT_2007
void ConnEventGift2007Msg(CNetMsg & msg, MSG_CONN_EVENT_GIFT_2007_TYPE subtype, CPC * pc);
void GiftEvent2007Msg(CNetMsg & rmsg, MSG_CONN_EVENT_GIFT_2007_TYPE nError);
#endif // GIFT_EVENT_2007

#ifdef PET_NAME_CHANGE
void PetNameChange( CNetMsg& msg, MSG_EX_PET_CHANGE_NAME_ERROR_TYPE err, int petidx , const char* strPetName );
void HelperPetNameChange( CNetMsg& msg, int charindex, int petindex, const char* strPetName );
#endif // PET_NAME_CHANGE

#ifdef PET_DIFFERENTIATION_ITEM
void HelperPetColorChange( CNetMsg& msg, int charindex, int petindex, char petCorlorType );
void PetChangeColor( CNetMsg& msg, int petindex, char petColorType, int ownerindex, char petMount );
#endif // PET_DIFFERENTIATION_ITEM

#ifdef CASH_EXCHANGE_EQUIP
void ItemExchangeEquipRepMsg(CNetMsg& msg, int olddbindex, int newdbindex);
#endif // CASH_EXCHANGE_EQUIP

#ifdef BLOOD_SWEAT_FRUIT
void ItemBloodSweatFruitAddRepMsg(CNetMsg& msg, CItem* item, MSG_ITEM_OPTION_ADD_RESULT result);
#endif

#ifdef DOUBLE_ITEM_DROP
void MsgrDoubleItemEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int itemPercent);
void MsgrDoubleItemEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int itemPercent);
#endif // DOUBLE_ITEM_DROP

#ifdef CREATE_EVENT
void ConnCreateEventCreateTableMsg(CNetMsg & msg, int idx);
#endif // CREATE_EVENT

#if defined( EVENT_SUMMER_VACATION_2007_RESULT ) || defined (EVENT_SUMMER_VACATION_2007 )
void EventSummerVacationInchenMsg( CNetMsg & msg, MSG_EVENT_SUMMER_VACATION_2007_TYPE vacationType, int itemindex );
void EventSummerVacation2007FigureMsg( CNetMsg & msg, MSG_EVENT_SUMMER_VACATION_2007_TYPE vacationType, int figuretype, int itemindex, int count );
#endif// defined( EVENT_SUMMER_VACATION_2007_RESULT ) || defined (EVENT_SUMMER_VACATION_2007 )

#ifdef EVENT_TLD_MOTHERDAY_2007
void EventTldMotherday2007tMsg(CNetMsg& msg, MSG_EVENT_TLD_MOTHERDAY_2007_ERROR_TYPE nErrorCode);
#endif // EVENT_TLD_MOTHERDAY_2007

#ifdef EVENT_BJMONO_2007
void EventBjMono2007Msg(CNetMsg & msg, MSG_EVENT_BJMONO_2007_TYPE subtype);
void ConnEventBjMono2007Msg(CNetMsg& msg, MSG_CONN_EVENT_BJMONO_2007_TYPE subtype,  CPC* pc );
#endif // EVENT_BJMONO_2007

#ifdef EVENT_SSHOT_2007
void EventSshot2007Msg(CNetMsg & msg, MSG_EVENT_SSHOT_2007_TYPE subtype);
void ConnEventSshot2007Msg(CNetMsg& msg, MSG_CONN_EVENT_SSHOT_2007_TYPE subtype, CPC* pc );
#endif //EVENT_SSHOT_2007

#ifdef EVENT_LC_1000DAY
void EventLC1000DayMsg( CNetMsg & msg, MSG_EVENT_LC_1000DAY_TYPE type );
#endif // EVENT_LC_1000DAY

#ifdef EVENT_RICHYEAR_2007
void EventRichYearMsg(CNetMsg& msg, MSG_EVENT_RICHYEAR_TYPE type);
#endif // EVENT_RICHYEAR_2007

#ifdef REWARD_IDC2007
void HelerRewardIDC2007Msg(CNetMsg& msg, int userindex);
#endif // REWARD_IDC2007

//#ifdef EVENT_HALLOWEEN_2007
void EventHalloween2007Msg( CNetMsg & msg, MSG_EVENT_HALLOWEEN_2007_TYPE type, unsigned char error );
void EventHalloween2007Msg( CNetMsg & msg, MSG_EVENT_HALLOWEEN_2007_TYPE type );
void HelperHalloween2007Msg( CNetMsg& msg, int char_index );
//#endif // EVENT_HALLOWEEN_2007

#ifdef EVENT_NEW_MOONSTONE
void EventNewMoonStoneMsg( CNetMsg& msg, MSG_EVENT_NEW_MOONSTONE_TYPE type);
#endif // EVENT_NEW_MOONSTONE

#ifdef ENABLE_WAR_CASTLE
void CastleOwnerInfoMsg(CNetMsg & msg);
#endif // ENABLE_WAR_CASTLE

#ifdef DRATAN_CASTLE
#ifdef DYNAMIC_DUNGEON
void DVDInfoMsg(CPC * pPC, CNetMsg & msg, int nMobRate, int nEnvRate );
void GuildNameColorStateMsg( CNetMsg & msg, CPC* ch );
void DVDManagementMsg( CNetMsg& msg, MSG_DVD_INFO_SUBTYPE subtype );
void DVDDungeonEnterMsg( CNetMsg& msg, MSG_DVD_INFO_SUBTYPE subtype );
void HelperDVDRateChangeMsg(CNetMsg & msg, MSG_DVD_INFO_SUBTYPE subtype, int nRate);
#endif //DYNAMIC_DUNGEON
#endif // DRATAN_CASTLE


#ifdef SAVE_MOONSTONE_MIX
void HelperSaveMoonStoneMix(CNetMsg& msg, int charindex);
#endif 

#ifdef EVENT_XMAS_2007
void ConnEventXmas2007Msg( CNetMsg& msg, MSG_CONN_EVENT_XMAS_2007_TYPE subtype );
void EventXmas2007Msg( CNetMsg& msg, MSG_EVENT_XMAS_2007_TYPE subtype );
#endif //EVENT_XMAS_2007


#ifdef ALTERNATE_MERCHANT
void AlternateMerchantErrorMsg(CNetMsg& msg, MSG_EX_ALTERNATE_MERCHANT_ERROR_TYPE errcode);
void AlternateMerchantSellStartMsg(CNetMsg& msg, CPC* ch);
void AlternateMerchantSellListMsg(CNetMsg& msg, CPC* ch);
void AlternateMerchantChangeMsg(CNetMsg& msg, CPC* ch);
void AlternateMerchantBuyMsg(CNetMsg& msg, CNetMsg& buymsg, CPC* tch);

void HelperAlternateMerchantStartMsg( CNetMsg& msg, int charindex, char shoptype, CAlternateMerchant* pAlternateMerchant );
void HelperAlternateMerchantBuyMsg( CNetMsg& msg, int charindex, char shoptype, CAlternateMerchant* pAlternateMerchant );
void HelperAlternateMerchantProductRecoveryMsg( CNetMsg& msg, int charindex, char shoptype, CAlternateMerchant* pAlternateMerchant );
void HelperAlternateMerchantNasRecoveryMsg( CNetMsg& msg, int charindex, char shoptype, CAlternateMerchant* pAlternateMerchant );
void HelperAlternateMerchantEndMsg( CNetMsg& msg, int charindex, char shoptype, CAlternateMerchant* pAlternateMerchant );

#endif // ALTERNATE_MERCHANT

#ifdef PET_TURNTO_NPC
void HelperPetTurnToNPCMsg( CNetMsg& msg, int charindex, int petindex, int toNpc, int size );
void PetTurnToNPCMsg( CNetMsg& msg, int petindex, int toNpc, int ownerindex , int size);
#endif //PET_TURNTO_NPC

#ifdef NEWYEAR_EVENT_2008
void NewYearSkillReqMsg(CNetMsg & msg, CPC * ch);
#endif // NEWYEAR_EVENT_2008

void MsgrSetExpLimitMsg( CNetMsg& msg, int server,  int charindex, int nLimit );

#ifdef MONSTER_COMBO
void MonsterComboErrorMsg(CNetMsg& msg, MSG_EX_MONSTERCOMBO_ERROR_TYPE subtype);
void MonsterComboMsg(CNetMsg & msg, MSG_EX_MONSTERCOMBO_TYPE subtype);
void HelperComboGotoWaitRoomPrompt(CNetMsg& msg, int bossindex, const char* bossname);
void ComboGotoWaitRoomPrompt(CNetMsg& msg, int bossindex, int nas);
//void ComboRecallToWaitRoomPrompt(CNetMsg& msg, int bossindex, int nas);
void HelperRecallToWaitRoomPrompt(CNetMsg& msg, int bossindex);
void EffectFireReadyMsg(CNetMsg& msg, int extra, int count, int* effectNPC, float* x, float* z, float* h);
void HelperCreateComboMsg(CNetMsg& msg, int subserver, int charindex, int areaindex, int comboindex);
#endif // MONSTER_COMBO

#if defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)
void EventSakuraEvent2008Msg( CNetMsg & msg, MSG_EVNET_SAKURA_2008_TYPE type );
#endif // defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)

#ifdef CLIENT_RESTART
void ConnClientRestartMsg( CNetMsg& msg , const char* userid );
#endif //CLIENT_RESTART

#ifdef CHAOSBALL
void ChaosBallMsg(CNetMsg & msg, MSG_EX_CHAOSBALL_ERRORTYPE error);
#endif

#ifdef NEW_CASHITEM_USA_2008
void ItemWarpDoc(CNetMsg& msg, int zone, int npcidx);
#endif // NEW_CASHITEM_USA_2008

#ifdef ATTENDANCE_EVENT_REWARD
void ConnEventAttendanceRewardMsg( CNetMsg& msg, MSG_CONN_EVENT_ATTENDANCE_EVENT_TYPE subtype, int userIndex, int charIndex );
#endif //ATTENDANCE_EVENT_REWARD

#ifdef IRIS_POINT
void ConnIPointAddMsg( CNetMsg& msg, int userIndex, int charIndex, MSG_CONN_IPOINT_TYPE type, int nIpoint );
#endif //IRIS_POINT

#ifdef EVENT_NOM_MOVIE
void ConnEventNomMsg(CNetMsg& msg, MSG_CONN_EVENT_NOM_TYPE eSubType, int nUserIdx, int nCharIdx);
void EventNomMsg(CNetMsg & msg, MSG_EVENT_NOM_ERROR_TYPE eError);
#endif

#ifdef REQUITAL_EVENT	// 보상 이벤트 
void ConnEventRequitalMsg(CNetMsg & msg, int nUserIdx, int nCharIdx, int nCheck_eventType, CLCString User_name, CLCString Char_Name);
void EventRequitalMsg(CNetMsg & msg, MSG_EVENT_REQUITAL_ERROR_TYPE eError);
#endif // REQUITAL_EVENT

#ifdef EVENT_PHOENIX  // yhj   081105  피닉스
void HelperEventPhoenixMsg(CNetMsg & msg, int nUserIdx, int nCharIdx );
void EventPhoenixMsg(CNetMsg & msg, MSG_EVENT_PHOENIX_ERROR_TYPE eError);
void EventPhoenix_SuccessMsg(CNetMsg & msg);
#endif // EVENT_PHOENIX

#ifdef EXTREME_CUBE
void HelperCubeStateReqMsg( CNetMsg& msg, int charindex);
void HelperCubeStatePersonalReqMsg(CNetMsg& msg, int charindex);
void HelperAddCubePointMsg(CNetMsg& msg, int ofguildindex, int ofcubepoint, long lastcubepoint);
void HelperAddCubePointPersonalMsg(CNetMsg& msg, int ofcharidx ,int ofcubepoint, long lastCubePoint);
void HelperGuildCubeNoticeMsg(CNetMsg& msg, int type);
void ExtremeCubeErrorMsg(CNetMsg& msg, MSG_EX_EXTREME_CUBE_ERROR_TYPE errortype);
void ExtremeCubeMsg(CNetMsg& msg, MSG_EX_EXTREME_CUBE_TYPE type);
#endif 

//#ifdef INIT_SSKILL
void InitSSkillMsg(CNetMsg& msg, MSG_EX_INIT_SSkill_ERROR_TYPE type);
//#endif // INIT_SSKILL

#if defined (CHAR_LOG) && defined (LC_KOR)
void ConnCharLogMsg(CNetMsg& msg, CPC* pc );
#endif // #if defined (CHAR_LOG) && defined (LC_KOR)

#ifdef TRADE_AGENT

typedef struct ItemOption
{
	int	m_nOptionCnt;

	int m_Option_type[MAX_ITEM_OPTION];
	int m_Option_level[MAX_ITEM_OPTION];

	ItemOption(void)
	{
		m_nOptionCnt = 0;

		for(int i=0; i < MAX_ITEM_OPTION; i++)
		{
			m_Option_type[i] = 0;
			m_Option_level[i] = 0;
		}
	}

}S_ITEMOPTION;

void HelperTradeAgentRegListReqMsg(CNetMsg& msg, int charindex,int page_no,int nAlignType);
void HelperTradeAgentRegReqMsg(CNetMsg& msg, int charindex, CLCString sellchar_name, char tab, char row, char col, int item_serial,CLCString item_serial2,CItem * pitem, CLCString item_name,int item_level,int item_type,int item_subtype,int item_class,int Quantity,LONGLONG TotalMoney,LONGLONG Guaranty,LONGLONG llCurCharHaveNas);
void HelperTradeAgentRegCancelReqMsg(CNetMsg& msg, int charindex,int nCancelindex);
void HelperTradeAgentRegResultMsg(CNetMsg& msg, int charindex,int nDbIndex,int nResult);
void HelperTradeAgentSearchReqMsg(CNetMsg& msg, int charindex,int nPageNo, int nitemType,int nitemSubType,int nJobClass,CLCString stSearchWord,int nAlignType);
void HelperTradeAgentBuyReqMsg(CNetMsg& msg, int charindex, CLCString buychar_name,int nList_idx,LONGLONG llCharHaveNas);
void HelperTradeAgentCalcListReqMsg(CNetMsg& msg, int charindex,int page_no,int nAlignType);
void HelperTradeAgentCalculateReqMsg(CNetMsg& msg, int charindex,int PageNo);
void HelperTradeAgentCalcResultMsg(CNetMsg& msg, int charindex,int nItemcount,int* pResultindex,int* pResult);
void HelperTradeAgentCheckCalcReqMsg(CNetMsg& msg, int charindex);

void TradeAgentRegListRepMsg(CNetMsg & msg,int nCharIndex, int nCharRegTotCount, int nMaxPageNo, int nPageNo, int nItemCount,int* pListindex, int* pItemSerial,int* pItemPlus,int* pItemFlag,S_ITEMOPTION* pItemOption, int* pQuantity,LONGLONG* ptotalmoney,int* pFinishDay,int* pFinishDayUnit);
void TradeAgentRegRepMsg(CNetMsg & msg,int nCharIndex, CLCString item_name,int Quantity,LONGLONG TotalMoney,LONGLONG Guaranty);
void TradeAgentRegCancelRepMsg(CNetMsg & msg, int nCharIndex);
void TradeAgentSearchRepMsg(CNetMsg & msg,int nCharIndex, int nMaxPageNo, int nPageNo, int nItemCount, int* pListindex, int* pItemSerial,int* pItemPlus,int* pItemFlag,S_ITEMOPTION* pItemOption, int* pQuantity,LONGLONG* ptotalmoney,int* pLevel,CLCString *pSellCharname);
void TradeAgentBuyRepMsg(CNetMsg & msg,int nCharIndex);
void TradeAgentCalcListRepMsg(CNetMsg & msg,int nCharIndex, int nMaxPageNo, int nPageNo, int nItemCount,int* pItemSerial,int* pItemPlus,int* pItemFlag,S_ITEMOPTION* pItemOption,int* pQuantity,LONGLONG* ptotalmoney,int* pState, int* pPassDay,CLCString *pCharname);
void TradeAgentCalculateRepMsg(CNetMsg & msg,int nCharIndex);
void TradeAgentCheckCalcRepMsg(CNetMsg & msg,int nCharIndex,int nIsBeCalcdata);
void TradeAgentNeedCalcMotifyMsg(CNetMsg & msg,int nCharIndex,unsigned char ucState,CLCString stItemname,int nItemCount);

void TradeAgentErrorMsg(CNetMsg & msg, unsigned char errorType,char errorPart);
#endif // TRADE_AGENT

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
void HelperDisconnectHackCharacter(CNetMsg & msg, int nUserIndex, const char* user_id, int nCharIndex, const char* char_id, int nHack_type, float delay);
#endif // DISCONNECT_HACK_CHARACTER 

#ifdef EX_GO_ZONE
void ExGoZoneErrorMsg(CNetMsg& msg, MSG_EX_GO_ZONE_ERROR_TYPE errtype, int npcidx);
void ExGoZoneMoveOKMsg(CNetMsg& msg, MSG_EX_GO_ZONE_ERROR_TYPE errtype, int zone, int extra, int npcIdx);
void ExGoZoneFieldDungeonMsg(CNetMsg& msg, int zone, int extra, int tax);
#endif 

#ifdef EXPEDITION_RAID
void ExpedErrorMsg(CNetMsg & msg, unsigned char errorType);
void HelperExpedCreateReqMsg(CNetMsg& msg,int nBossIndex,CLCString BossName);
void HelperExpedInviteReqMsg(CNetMsg& msg, int nBossIndex, CLCString strBossName, int nTargetIndex,CLCString strTargetName);
void HelperExpedAllowReqMsg(CNetMsg& msg, int nBossIndex, int nAllowIndex, CLCString strAllowCharName);
void HelperExpedRejectReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex);
void HelperExpedKickReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex);
void HelperExpedQuitReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex,int nQuitMode);
void HelperExpedChangeBossReqMsg(CNetMsg& msg, int nBossIndex, int nChangeMode, int nNewBossIndex);
void HelperExpedChangeTypeReqMsg(CNetMsg& msg, int nBossIndex, char cDiviType, char ExpedType);
void HelperExpedEndExpedMsg(CNetMsg& msg, int nBossIndex);
void HelperExpedSetMBossReqMsg(CNetMsg& msg, int nBossIndex, int nNewMBossIndex);
void HelperExpedResetMBossReqMsg(CNetMsg& msg, int nBossIndex, int nNewMBossIndex);
void HelperExpedMoveGroupReqMsg(CNetMsg& msg, int nBossIndex, int nSourceGroup, int nMoveCharIndex, int nTargetGroup, int nTargetListindex);
void HelperExpedSetLabelReqMsg(CNetMsg& msg, int nBossIndex, int nType, int nMode, int nLabel, int nDestIndex);
void HelperExpedRejoinReqMsg(CNetMsg& msg, int nBossIndex, int nJoinIndex);
void HelperExpedChatReqMsg(CNetMsg& msg, int nBossIndex, int nCharIdex, CLCString strCharName,int nGroupTyp, CLCString strChat);

void ExpedCreateRepMsg(CNetMsg & msg, char cExpedType1, char cExpedType2, char cExpedType3, int nBossIndex,CLCString BossCharName,int nMemberCount,int *pCharIdex,CLCString* pCharName,int* pGroupType,int* pMemberType, int* pSetLabelType,int* pQuitType);
void ExpedInviteRepMsg(CNetMsg & msg, char cExpedType1, char cExpedType2, char cExpedType3, int nBossIndex, CLCString strBossName);
void ExpedAddMsg(CNetMsg& msg, int nCharIndex, const char* strCharName,int nCharGroup,int nCharMember, int nCharListIndex, CPC* tch, char isboss);
void ExpedInfoMsg(CNetMsg& msg, int nIndex, int nGroup, int nLevel, int nHP, int nMaxHP, int nMP, int nMaxMP, float x, float z, char nYlayer, int nZone);
void ExpedQuitRepMsg(CNetMsg& msg, int nTargetIndex, int nQuitMode);
void ExpedQuitReqMsg(CNetMsg& msg, int nQuitMode);
void ExpedKickMsg(CNetMsg& msg, int nTargetIndex);
void ExpedMsg(CNetMsg& msg, unsigned char subtype);
void ExpedChangeBossReq(CNetMsg& msg, int nChangeMode, int nNewBossIndex);
void ExpedChangeBoss(CNetMsg& msg, int nBossIndex,  int nNewBossIndex, int nChangeMode);
void ExpedChangeType(CNetMsg& msg, char cDiviType, char cExpedType);
void ExpedSetMbossMsg(CNetMsg& msg, int nNewMBossIndex);
void ExpedResetMbossMsg(CNetMsg& msg, int nNewMBossIndex);
void ExpedMoveGroupRepMsg(CNetMsg& msg,int nSourceGroup, int nMoveCharIndex, int nTargetGroup, int nTargetListindex);
void ExpedSetLabelRepMsg(CNetMsg& msg,int nType, int nMode, int nLabel,int nDestIndex);
bool ExpedViewDail_InvenMsg(CNetMsg& msg, CPC* ch);
void ExpedViewDail_ExPetMsg(CNetMsg& msg, CPet* pet);
void ExpedViewDail_ExAPetMsg(CNetMsg& msg, CAPet* pet);
#endif //EXPEDITION_RAID

#ifdef FACTORY_SYSTEM
void FactoryLearnMsg(CNetMsg& msg, CFactoryProto * pFactory);
void FactoryListMsg(CNetMsg& msg, CPC* ch, int nSealType = -1);
void FactoryErrorMsg(CNetMsg& msg, MSG_FACTORY_ERROR_TYPE errorcode);
void FactoryMakeMsg(CNetMsg& msg, CPC* ch, CFactoryProto * pFactory);
#endif

#ifdef TRIGER_SYSTEM
void TrigerEvnetMsg(CNetMsg& msg, MSG_EX_TRIGER_EVENT_TYPE type, int touch_ID, int Play_ID);
#endif // TRIGER_SYSTEM

#endif // #ifndef __ACMDMSG_H__
