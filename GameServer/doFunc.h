#ifndef __DOFUNCTION_H__
#define __DOFUNCTION_H__

// 채팅 관련
void do_Chat(CPC* ch, CNetMsg& msg);

// 이동 관련
void do_Move(CPC* ch, CNetMsg& msg);
void do_GoZone(CPC* ch, CNetMsg& msg);
void do_Warp(CPC* ch, CNetMsg& msg);

// 공격 관련
void do_Attack(CPC* pc, CNetMsg& msg);

// N:1 Attack  & move in Personal dungeon
//N1InPersonalDungeon BW

void do_pd_Attack(CPC* pc, CNetMsg& msg);
void do_pd_Move(CPC* ch, CNetMsg& msg);


// 아이템 관련
void do_Item(CPC* ch, CNetMsg& msg);
void do_ItemUse(CPC* ch, CNetMsg& msg, bool bprolong = false);
void do_ItemTake(CPC* ch, CNetMsg& msg);
void do_ItemThrow(CPC* ch, CNetMsg& msg);
void do_ItemArrange(CPC* ch, CNetMsg& msg);
void do_ItemDelete(CPC* ch, CNetMsg& msg);
void do_ItemWear(CPC* ch, CNetMsg& msg);
void do_ItemSwap(CPC* ch, CNetMsg& msg);
void do_ItemBuy(CPC* ch, CNetMsg& msg);
void do_ItemSell(CPC* ch, CNetMsg& msg);
void do_ItemUpgradeReq(CPC* ch, CNetMsg& msg);
void do_ItemRefinReq(CPC* ch, CNetMsg& msg);
void do_ItemOptionAddReq(CPC* ch, CNetMsg& msg);
void do_ItemOptionDelReq(CPC* ch, CNetMsg& msg);
void do_ItemProcessReq(CPC* ch, CNetMsg& msg);
void do_ItemMakeReq(CPC* ch, CNetMsg& msg);
void do_ItemMixReq(CPC* ch, CNetMsg& msg);
void do_ItemArcaneReq(CPC* ch, CNetMsg& msg);
#ifdef ENABLE_CHANGEJOB
void do_ItemChangeWeaponReq(CPC* ch, CNetMsg& msg);
#endif
void do_ItemProcessNPC(CPC* ch, CNetMsg& msg);
void do_ItemAddBooster(CPC* ch, CNetMsg& msg);
#ifdef ENABLE_WAR_CASTLE
void do_ItemMixWarItem(CPC* ch, CNetMsg& msg);
#endif

#ifdef LAKA_PRESSCORPS
void do_itemUse_PressCorps(CPC* ch, CNetMsg& msg);
#endif // LAKA_PRESSCORPS

#ifdef ADULT_SERVER_NEWITEM
void do_ItemTarget(CPC* ch, CNetMsg& msg);
#endif // ADULT_SERVER_NEWITEM

#ifdef FEEITEM
bool do_useCashItem(CPC* ch, CItem* item, CNetMsg& msg, bool bprolong = false, int extra1 = 0);
void do_Ex_CashItem(CPC* ch, CNetMsg& msg);
void doCashItemPuchase(CPC* ch, CNetMsg& msg);
void doCashItemBalanceReq(CPC* ch, CNetMsg& msg);
void doCashItemBring(CPC* ch, CNetMsg& msg);
void doCashItemPurchaseList(CPC* ch, CNetMsg& msg);
void doCashItemPurchaseHistory(CPC* ch, CNetMsg& msg);
int usePeriodItem(int* useTime, bool* bprolong);
int usePeriodItem_7(int* useTime, bool* bprolong);
void doCashItemMoonStoneStart(CPC* ch, CNetMsg& msg);
void doCashItemMoonStoneStop(CPC* ch, CNetMsg& msg);
// 상품 가져오기와, 선물 가져오기, 같은 프로세스를 사용함,
// bPresent가 트루이면 선물 
void CashItemBringProcess(CPC* ch, CNetMsg& msg, bool bPresent);
#ifdef CASH_ITEM_GIFT
void doCashItemGift(CPC* ch, CNetMsg& msg);
void doCashItemGiftSendHistory(CPC* ch, CNetMsg& msg);
void doCashItemGiftRecvHistory(CPC* ch, CNetMsg& msg);
void doCashItemGiftRecvList(CPC* ch, CNetMsg& msg);
void doCashItemGiftRecv(CPC* ch, CNetMsg& msg);
#endif // CASH_ITEM_GIFT
#ifdef CASH_ITEM_COUPON
void doCashItemCouponListReq( CPC* ch, CNetMsg& msg );
void doCashItemPuchaseByCoupon( CPC* ch, CNetMsg& msg );
#endif //CASH_ITEM_COUPON
#endif // FEEITEM

bool do_QuestStart(CPC* ch, int questIndex);
bool do_QuestGiveUp(CPC* ch, int questIndex);

// 교환 관련
void do_Exchange(CPC* ch, CNetMsg& msg);
void do_ExchangeReq(CPC* ch, CNetMsg& msg);
void do_ExchangeItem(CPC* ch, CNetMsg& msg);
void do_ExchangeReqReq(CPC* ch, CNetMsg& msg);
void do_ExchangeReqRep(CPC* ch, CNetMsg& msg);
void do_ExchangeReqReject(CPC* ch, CNetMsg& msg);
void do_ExchangeReqReady(CPC* ch, CNetMsg& msg);
void do_ExchangeReqOK(CPC* ch, CNetMsg& msg);
void do_ExchangeItemAdd(CPC* ch, CNetMsg& msg);
void do_ExchangeItemDel(CPC* ch, CNetMsg& msg);

// 장소 기억 관련
void do_MemPos(CPC* ch, CNetMsg& msg);
#ifdef PRIMIUM_MEMORYBOOK
void do_MemPosPlus(CPC* ch, CNetMsg& msg);
#endif	// PRIMIUM_MEMORYBOOK

// 펄스 관련
void do_Pulse(CPC* ch, CNetMsg& msg);

#ifdef DISCONNECT_HACK_CHARACTER
void do_Disconnect_Hack_Character(CPC* ch, CNetMsg& msg);
#endif // DISCONNECT_HACK_CHARACTER

// 운영자 명령 관련
void do_GM(CPC* ch, CNetMsg& msg);
void do_GMEcho(CPC* ch, const char* arg);
void do_GMGoZone(CPC* ch, const char* arg);
void do_GMGoto(CPC* ch, const char* arg);
void do_GMItemDrop(CPC* ch, const char* arg);
void do_GMCountPC(CPC* ch, const char* arg);
void do_GMLoopCount(CPC* ch, const char* arg);
void do_GMImmortal(CPC* ch, const char* arg);
void do_GMShutdown(CPC* ch, const char* arg);
void do_GMReboot(CPC* ch, const char* arg);
void do_GMCountNPC(CPC* ch, const char* arg);
void do_GMVisible(CPC* ch, const char* arg);
void do_GMGoPC(CPC* ch, const char* arg);
void do_GMGoNPC(CPC* ch, const char* arg);
void do_GMLevelUP(CPC* ch, const char* arg);
void do_GMKICK(CPC* ch, const char* arg);
void do_GMComon(CPC* ch, const char* arg);
void do_GMSummon(CPC* ch, const char* arg);
void do_GMSilence(CPC* ch, const char* arg);
void do_GMWhereAmI(CPC* ch, const char* arg);
void do_GMQuestComplete(CPC* ch, const char* arg);
void do_GMStatPoint(CPC* ch, const char* arg);
void do_GMSkillPoint(CPC* ch, const char* arg);
void do_GMSetRegenSec(CPC* ch, const char* arg);
void do_GMNPCRegen(CPC* ch, const char* arg);
void do_GMKillNPC(CPC* ch, const char* arg);
void do_GMExpUp(CPC* ch, const char* arg);
void do_GMBloodPoint(CPC* ch, const char* arg);
void do_GMEndGame(CPC* ch, const char* arg);
void do_GMObserve(CPC* ch, const char* /*arg*/);
void do_GMChatMonitor(CPC* ch, const char* arg);
void do_GMDoubleEvent(CPC* ch, const char* arg);
void do_GMDoubleExpEvent(CPC* ch, const char* arg);
void do_GMLattoEvent(CPC* ch, const char* arg);
void do_GMNewYearEvent(CPC* ch, const char* arg);
void do_GMValentineEvent(CPC* ch, const char* arg);
void do_GMWhiteDayEvent(CPC* ch, const char* arg);
void do_GMLetterEvent(CPC* ch, const char* arg);
void do_GMReloadGMIP(CPC* ch, const char* arg);
void do_GMSpeedUp(CPC* ch, const char* arg);
void do_GMFameUp(CPC* ch, const char* arg);
void do_GMPD4(CPC* ch, const char* arg);
void do_GMKickID(CPC* ch, const char* arg);
void do_GMResurrection(CPC *ch, const char* arg);
void do_GMRecommend(CPC *ch, const char* arg);
void do_GMSetLimit(CPC* ch, const char* arg);
void do_GMDisguise(CPC* ch, const char* /* arg */);
void do_GMLoginServer(CPC* ch, const char* arg);
void do_OTAKUSpeedUp(CPC* ch, const char* arg);
void do_OTAKUImmortal(CPC* ch, const char* arg);
void do_GMCalTime(CPC* ch, const char* arg);
void do_GMKickCharIndex(CPC* ch, const char* arg );
void do_GMEventShow(CPC* ch, const char* arg);
void do_GMEventSetting(CPC* ch, const char* arg );
void do_GMChanceEvent(CPC* ch, const char* arg);
void do_GMToolChatMonitor(CPC* ch, const char* arg );
void do_GMDoubleItemEvent(CPC* ch, const char* arg);
void do_GMDoublePetExpEvent(CPC* ch, const char* arg);
void do_GMDoubleAttackEvent(CPC* ch, const char* arg);
void do_GMDropItemEvent(CPC* ch, const char* arg);
void do_GMSilenceMulti(CPC* ch, const char* arg);
void do_GMKickMulti(CPC* ch, const char* arg);
void do_GMExpLimit(CPC* ch, const char* arg);
void do_GMDoubleEventAuto(CPC* ch, const char* arg);
void do_GMUpgradeEvent(CPC* ch, const char* arg);
void do_GMKickGuildMember(CPC* ch, const char* arg);
void do_GMApplySkill(CPC* ch, const char* arg);
// 파티 관련
void do_Party(CPC* ch, CNetMsg& msg);
void do_PartyInvite(CPC* ch, CNetMsg& msg);
void do_PartyAllow(CPC* ch, CNetMsg& msg);
void do_PartyReject(CPC* ch, CNetMsg& msg);
void do_PartyQuit(CPC* ch, CNetMsg& msg);
void do_PartyKick(CPC* ch, CNetMsg& msg);
void do_PartyAssistInfo(CPC* ch, CNetMsg& msg);
void do_PartyChangeBoss(CPC* ch, CNetMsg& msg);

#ifdef NEW_UI
void do_PartyChangeType(CPC* ch, CNetMsg& msg);
#endif // NEW_UI

#ifdef PARTY_RAID
void do_PartyEndPartyReq(CPC* ch, CNetMsg& msg);
void do_PartyInzoneClearReq(CPC* ch, CNetMsg& msg);
#endif //PARTY_RAID

// 퀵슬롯 관련
void do_QuickSlot(CPC* ch, CNetMsg& msg);

// 싱글던전 - NPC 리젠 관련
void do_NPCRegen(CPC* ch, CNetMsg& msg);

// 스킬 관련
void do_Skill(CPC* ch, CNetMsg& msg);
void do_SkillLearn(CPC* ch, CNetMsg& msg);
void do_SkillReady(CPC* ch, CNetMsg& msg);
void do_SkillFire(CPC* ch, CNetMsg& msg);
void do_SkillCancel(CPC* ch, CNetMsg& msg);
#ifdef NEWYEAR_EVENT_2008
void do_SkillNewYear2008(CPC * ch, CNetMsg & msg);
#endif // NEWYEAR_EVENT_2008
#ifdef HANARO_EVENT
void do_SkillHanaroEvent(CPC * ch, CNetMsg & msg);
#endif
#ifdef FACTORY_SYSTEM
void do_SealSkillLearn(CPC* ch, CNetMsg& msg);
#endif


// 부활 메시지
void do_Rebirth(CPC* ch, CNetMsg& msg);

// 액션 메시지
void do_Action(CPC* ch, CNetMsg& msg);

// 퀘스트 관련
void do_Quest(CPC* ch, CNetMsg& msg);
void do_QuestRequest(CPC* ch, CNetMsg& msg);
void do_QuestStart(CPC* ch, CNetMsg& msg);
void do_QuestGiveUp(CPC* ch, CNetMsg& msg);
void do_QuestPrize(CPC* ch, CNetMsg& msg);
void do_QuestComplete(CPC* ch, CNetMsg& msg);
void do_QuestCollect(CPC* ch, CNetMsg& msg);
CItem* OptionSettingItem(CPC* ch, CItem* item);


void do_QuestPD4RankView(CPC* ch, CNetMsg& msg);
void do_QuestPD4RankRewardView(CPC* ch, CNetMsg& msg);
void do_QuestPD4RankReward(CPC* ch, CNetMsg& msg);


// 특수스킬 관련
void do_SSkill(CPC* ch, CNetMsg& msg);

// PK 관련
void do_PK(CPC* ch, CNetMsg& msg);
//void do_PKRecoverHPMP(CPC* ch, CNetMsg& msg);
void do_PKRecoverItemSealed(CPC* ch, CNetMsg& msg);

// 이벤트 관련
void do_Event(CPC* ch, CNetMsg& msg);

// 개인상점 관련
void do_PersonalShop(CPC* ch, CNetMsg& msg);
void do_PersonalShopSellStart(CPC* ch, CNetMsg& msg);
void do_PersonalShopSellList(CPC* ch, CNetMsg& msg);
void do_PersonalShopChange(CPC* ch, CNetMsg& msg);
void do_PersonalShopBuy(CPC* ch, CNetMsg& msg);

// 창고 관련
void do_Stash(CPC* pc, CNetMsg& msg);
void do_StashIsSetPassword(CPC* ch, CNetMsg& msg);
void do_StashListReq(CPC* ch, CNetMsg& msg);
void do_StashListReq_real(CPC* ch);
void do_StashKeep(CPC* ch, CNetMsg& msg);
void do_StashKeep_real(CPC* ch);
void do_StashTake(CPC* ch, CNetMsg& msg);
void do_StashTake_real(CPC* ch);
void do_StashChangePassword(CPC* ch, CNetMsg& msg);
void do_StashSeal(CPC* ch, CNetMsg& msg);

/////////////////////////////////////////////
// BANGWALL : 2005-07-05 오후 2:37:20
// Comment : 테섭적용
void do_StashDeletePassword(CPC* ch, CNetMsg& msg);
void do_StashCheckPassword(CPC* ch, CNetMsg& msg);
void do_StashSetNewPassword(CPC* ch, CNetMsg& msg);


// 길드 관련
void do_Guild(CPC* ch, CNetMsg& msg);
void do_GuildCreate(CPC* ch, CNetMsg& msg);
void do_GuildLevelUp(CPC* ch, CNetMsg& msg);
void do_GuildBreakUp(CPC* ch, CNetMsg& msg);
void do_GuildRegistReq(CPC* ch, CNetMsg& msg);
void do_GuildRegistAllow(CPC* ch, CNetMsg& msg);
void do_GuildRegistCancel(CPC* ch, CNetMsg& msg);
void do_GuildOutReq(CPC* ch, CNetMsg& msg);
void do_GuildKick(CPC* ch, CNetMsg& msg);
void do_GuildChangeBoss(CPC* ch, CNetMsg& msg);
void do_GuildAppointOfficer(CPC* ch, CNetMsg& msg);
void do_GuildFireOfficer(CPC* ch, CNetMsg& msg);
void do_GuildBattleReqReq(CPC* ch, CNetMsg& msg);
void do_GuildBattleReqReject(CPC* ch, CNetMsg& msg);
void do_GuildBattleReqAccept(CPC* ch, CNetMsg& msg);
void do_GuildBattleStopReq(CPC* ch, CNetMsg& msg);
void do_GuildBattleStopReject(CPC* ch, CNetMsg& msg);
void do_GuildBattleStopAccept(CPC* ch, CNetMsg& msg);
void do_GuildWarSetTimeReq(CPC* ch, CNetMsg& msg);
void do_GuildWarGetTime(CPC* ch, CNetMsg& msg);
void do_GuildWarJoinAttackGuild(CPC* ch, CNetMsg& msg);
void do_GuildWarJoinDefenseGuild(CPC* ch, CNetMsg& msg);
void do_GuildWarJoinAttackChar(CPC* ch, CNetMsg& msg);
void do_GuildWarJoinDefenseChar(CPC* ch, CNetMsg& msg);
void do_GuildWarAttackSymbol(CPC* ch, CNetMsg& msg);
void do_GuildStashHistoryReq(CPC* ch, CNetMsg& msg);
void do_GuildStashViewReq(CPC* ch, CNetMsg& msg);
void do_GuildStashTakeReq(CPC* ch, CNetMsg& msg);


#ifdef NEW_GUILD
void do_GuildInclineEstablist( CPC* ch, CNetMsg& msg );
void do_GuildMemberAdjust( CPC* ch, CNetMsg& msg );
void do_NewGuildInfo( CPC* ch, CNetMsg& msg );
void do_NewGuildMemberList( CPC* ch, CNetMsg& msg );
void do_NewGuildManage( CPC* ch, CNetMsg& msg );
void do_NewGuildNotice( CPC* ch, CNetMsg& msg );
void do_NewGuildNoticeUpdate( CPC* ch, CNetMsg& msg );
void do_NewGuildNoticeTrans( CPC* ch, CNetMsg& msg );
void do_NewGuildSkillList( CPC* ch, CNetMsg& msg );
void do_NewGuildSkillLearn( CPC* ch, CNetMsg& msg );
#endif // NEW_GUILD

// 변신 관련
void do_Change(CPC* ch, CNetMsg& msg);

// 클라이언트 UI 관련
void do_UI(CPC* ch, CNetMsg& msg);

// 사제 시스템 관련
void do_Teach(CPC* ch, CNetMsg& msg);
void do_TeachTeacherList(CPC* ch, CNetMsg& msg);
void do_TeachTeacherReqReq(CPC* ch, CNetMsg& msg);
void do_TeachTeacherReqReject(CPC* ch, CNetMsg& msg);
void do_TeachTeacherReqAccept(CPC* ch, CNetMsg& msg);
void do_TeachInfo(CPC* ch, CNetMsg& msg);
void do_TeachTeacherGiveUP(CPC* ch, CNetMsg& msg);
void do_TeachSetCancel(CPC* teacher, CPC* student);
void do_TeachSetCancel(CPC* teacher, int studentidx);

// 전직 관련
#ifdef ENABLE_CHANGEJOB
void do_ChangeJob(CPC* ch, CNetMsg& msg);
bool do_ChangeJobReq(CPC* pc, CNetMsg& msg);
bool do_ChangeJobResetReq(CPC* pc, CNetMsg& msg);
#endif // #ifdef ENABLE_CHANGEJOB

// 성장포인트 관련
void do_StatPoint(CPC* ch, CNetMsg& msg);
void do_StatPointUse(CPC* ch, CNetMsg& msg);
#ifdef ENABLE_CHANGEJOB
void do_StatPointReset(CPC* ch, CNetMsg& msg);
#endif // #ifdef ENABLE_CHANGEJOB

// 이벤트
void do_GMEvent(CPC* ch, const char* arg);

#ifdef ENABLE_MESSENGER
void do_Friend(CPC* ch, CNetMsg& msg);
void do_FriendChatting(CPC* ch, CNetMsg& msg);
void do_FriendSetCondition(CPC* ch, CNetMsg& msg);
void do_FriendRegistRequest(CPC* ch, CNetMsg& msg);
void do_FriendRegistAllow(CPC* ch, CNetMsg& msg);
void do_FriendDeleteMember(CPC* ch, CNetMsg& msg);
void do_FriendRegistRefuse(CPC* ch, CNetMsg& msg);
void do_FriendBlockMember(CPC* ch, CNetMsg& msg);
#endif // #ifdef ENABLE_MESSENGER

// 선택 가공
void do_SelectProduce(CPC* ch, CNetMsg& msg);

// 확장 메시지 처리
void do_Extend(CPC* ch, CNetMsg& msg);

void do_Ex_NameChange(CPC* ch, CNetMsg& msg);

#ifdef ENABLE_PET
void do_ExPetCall(CPC* ch, CNetMsg& msg);
void do_ExPetLearn(CPC* ch, CNetMsg& msg);
void do_ExPetResetSkill(CPC* ch, CNetMsg& msg);
void do_ExPetChangeMount(CPC* ch, CNetMsg& msg);
void do_ExPetCommand(CPC* ch, CNetMsg& msg);
void do_ExPetMixItem(CPC* ch, CNetMsg& msg);
void do_ExPetWarpTown(CPC* ch, CNetMsg& msg);
void do_ExPetChangeItem(CPC* ch, CNetMsg& msg);
void do_ExPetRebirth(CPC* ch, CNetMsg& msg);
#ifdef PET_NAME_CHANGE
void do_PetNameChange( CPC* ch, CNetMsg& msg );
#endif //PET_NAME_CHANGE
#endif // #ifdef ENABLE_PET

void do_RandomProduce(CPC* ch, CNetMsg& msg);

#ifdef ENABLE_WAR_CASTLE
// 공성맵 관련
void do_ExCastleMapRecent(CPC* ch, CNetMsg& msg);
void do_ExCastleMapSignal(CPC* ch, CNetMsg& msg);
#endif // #ifdef ENABLE_WAR_CASTLE

// 강신 취소
void do_ExEvocationStop(CPC* ch, CNetMsg& msg);

void do_ExPartyRecall(CPC* ch, CNetMsg& msg);

#ifdef PARTY_MATCHING
void do_ExPartyMatch(CPC* ch, CNetMsg& msg);
#endif // PARTY_MATCHING

#ifdef USING_NPROTECT
void do_ExNProtect(CPC* ch, CNetMsg& msg);
#endif // USING_NPROTECT
#ifdef DRATAN_CASTLE
void do_ExCastleWar(CPC * ch, CNetMsg & msg);
void do_ExCastleCristalRespondStart(CPC * ch, CNetMsg & msg);
void do_ExCastleCristalRespondEnd(CPC * ch, CNetMsg & msg);
void do_ExCastleTowerList(CPC * ch, CNetMsg & msg);
void do_ExcastleTowerAdd(CPC * ch, CNetMsg & msg);
void do_ExCastleTowerReinforce(CPC * ch, CNetMsg & msg);
void do_ExCastleTowerReinforceList(CPC * ch, CNetMsg & msg);
void do_ExCastleTowerRepaire(CPC * ch, CNetMsg & msg);
void do_ExCastleTowerRepaireList(CPC * ch, CNetMsg & msg);
void do_ExCastleTowerWarpList(CPC * ch, CNetMsg & msg);
void do_ExCastleQuartersInstall(CPC * ch, CNetMsg & msg);


void do_DVD(CPC * ch, CNetMsg & msg);
#ifdef DYNAMIC_DUNGEON
void do_dvdInfo(CPC * ch, CNetMsg & msg);
void do_dvdManagement( CPC * ch, CNetMsg & msg );
void do_dvdDungenEnter( CPC *ch, CNetMsg& msg );
#endif //DYNAMIC_DUNGEON

#endif // DRATAN_CASTLE




// 대리 상인
#ifdef ALTERNATE_MERCHANT
void do_AlternateMerchant(CPC* ch, CNetMsg& msg);
void do_AlternateMerchantSellStart(CPC* ch, CNetMsg& msg);
void do_AlternateMerchantSellList(CPC* ch, CNetMsg& msg);
void do_AlternateMerchantChange(CPC* ch, CNetMsg& msg);
void do_AlternateMerchantBuy(CPC* ch, CNetMsg& msg);
void do_AlternateMerchantEnd(CPC* ch, CNetMsg& msg);
void do_AlternateMerchantProductRecovery(CPC* ch, CNetMsg& msg);
void do_AlternateMerchantNasRecovery(CPC* ch, CNetMsg& msg);
#endif // ALTERNATE_MERCHANT


#ifdef MONSTER_COMBO
void do_MonsterCombo(CPC* ch, CNetMsg& msg);
void do_MonsterComboEditComplete(CPC* ch, CNetMsg& msg);
void do_MonsterComboEditCancel( CPC* ch );
void do_MonsterComboContextReq( CPC* ch );
void do_MonsterComboGotoCombo(CPC* ch, CNetMsg& msg);
void do_MonsterComboConfirm(CPC* ch, CNetMsg& msg);
void do_MonsterComboRecallToCombo( CPC* ch );
void do_MonsterComboGiveUp(CPC* ch, CNetMsg& msg);
void do_MonsterComboGotoStage( CPC* ch );
void do_MonsterComboGotoWaitRoom( CPC* ch );
#endif // MONSTER_COMBO

#ifdef CLIENT_RESTART
void do_ClientRestart( CPC* ch, CNetMsg& msg);
#endif //CLIENT_RESTART

#ifdef CHAOSBALL
void do_ChaosBall(CPC* ch, CNetMsg& msg );
#endif

#ifdef IRIS_POINT
void do_UpdatePlayTime(CPC* ch, CNetMsg& msg );
#endif //IRIS_POINT

#ifdef ATTACK_PET
void do_AttackPet(CPC* ch, CNetMsg& msg );
#endif //ATTACK_PET

#ifdef EXTREME_CUBE
void do_ExExtremeCube(CPC* ch, CNetMsg& msg);
#endif // EXTREME_CUBE

//#ifdef INIT_SSKILL
void do_ExInitSSkill(CPC* ch, CNetMsg& msg);
//#endif // INIT_SSKILL

#ifdef TRADE_AGENT
void do_TradeAgent(CPC* ch, CNetMsg& msg);

void do_TradeAgent_RegListReq(CPC* ch, CNetMsg& msg);
void do_TradeAgent_RegReq(CPC* ch, CNetMsg& msg);
void do_TradeAgent_RegCancelReq(CPC* ch, CNetMsg& msg);
void do_TradeAgent_SearchReq(CPC* ch, CNetMsg& msg);
void do_TradeAgent_BuyReq(CPC* ch, CNetMsg& msg);
void do_TradeAgent_CalcListReq(CPC* ch, CNetMsg& msg);
void do_TradeAgent_CalculateReq(CPC* ch, CNetMsg& msg);
void do_TradeAgent_CheckCalcReq(CPC* ch, CNetMsg& msg);
#endif //TRADE_AGENT

#ifdef EXPEDITION_RAID
void do_Expedition(CPC* ch, CNetMsg& msg);

void do_ExpedCreateReq(CPC* ch, CNetMsg& msg);
void do_ExpedInviteReq(CPC* ch, CNetMsg& msg);
void do_ExpedAllowReq(CPC* ch, CNetMsg& msg);
void do_ExpedRejectReq(CPC* ch, CNetMsg& msg);
void do_ExpedQuitReq(CPC* ch, CNetMsg& msg);
void do_ExpedEndExpedReq(CPC* ch, CNetMsg& msg);
void do_ExpedKickReq(CPC* ch, CNetMsg& msg);
void do_ExpedChangeTypeReq(CPC* ch, CNetMsg& msg);
void do_ExpedChangeBossReq(CPC* ch, CNetMsg& msg);
void do_ExpedSetMBossReq(CPC* ch, CNetMsg& msg);
void do_ExpedResetMBossReq(CPC* ch, CNetMsg& msg);
void do_ExpedMoveGroupReq(CPC* ch, CNetMsg& msg);
void do_ExpedAddMemberReq(CPC* ch, CNetMsg& msg);
void do_ExpedViewDetailReq(CPC* ch, CNetMsg& msg);
void do_ExpedSetLabelReq(CPC* ch, CNetMsg& msg);
void do_ExpedChatReq(CPC* ch, CNetMsg& msg);

void do_Logout(CPC* ch, CNetMsg& msg);
#endif //EXPEDITION_RAID

#ifdef EX_GO_ZONE
void do_ExGoZone(CPC* ch, CNetMsg& msg);
#endif // EX_GO_ZONE

#ifdef FACTORY_SYSTEM
void do_Factory(CPC* ch, CNetMsg& msg);
void do_FactoryLearn(CPC* ch, CNetMsg& msg);
void do_FactoryList(CPC* ch, CNetMsg& msg);
void do_FactoryMake(CPC* ch, CNetMsg& msg);
#endif

#ifdef TRIGER_SYSTEM	// 트리거 사용
void do_TrigerEvent(CPC* ch, CNetMsg& msg);
void do_TrigerEvent_ItemComposition(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEvent_ItemDrop(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEvent_OpenTheDoor(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEvent_StartRaid(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEvent_MessagePrint(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEvent_SummonNpc(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEvent_UseDebuf(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEvent_ActiveObject(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEvent_RemoveNpc(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEventReady_ItemComposition(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEventReady_ItemDrop(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEventReady_OpenTheDoor(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEventReady_StartRaid(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEventReady_MessagePrint(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEventReady_SummonNpc(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEventReady_UseDebuf(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEventReady_ActiveObject(CPC* ch, CNetMsg& msg, CTriger* triger);
void do_TrigerEventReady_RemoveNpc(CPC* ch, CNetMsg& msg, CTriger* triger);
#endif // TRIGER_SYSTEM

#endif // #ifndef __DOFUNCTION_H__
