#ifndef __HARD_CODING_H__
#define __HARD_CODING_H__

#ifdef EVENT_XMAS_2006
void DropEventXMas2006Item(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_XMAS_2006

#ifdef MONSTER_RAID_SYSTEM
void DropRaidMonsterItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // MONSTER_RAID_SYSTEM

#ifdef EVENT_CHUSEOK_2006
void DropChuseok2006Item(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_CHUSEOK_2006

#ifdef EVENT_TLD_BUDDHIST
void DropTldBuddhistItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_TLD_BUDDHIST

void DropGoldenBallItem(CNPC* npc, CPC* pc, CPC* tpc, int level);

#ifdef EVENT_WORLDCUP_2006_VOTE
void DropFIFACupItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_WORLDCUP_2006_VOTE

#ifdef RECOMMEND_SERVER_SYSTEM
void DropRecommendItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // RECOMMEND_SERVER_SYSTEM

#ifdef DROP_MAKE_DOCUMENT
void DropMakeDocument(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // DROP_MAKE_DOCUMENT

#ifdef EVENT_NEW_SERVER_2006_NEWSERVER
void DropEventNewServer2006NewServer(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_NEW_SERVER_2006_NEWSERVER

#if defined(EVENT_NEW_SERVER_2006_OLDSERVER) || defined(EVENT_NEW_SERVER_2006_NEWSERVER)
void DropEventNewServer2006StatPotion(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

#ifdef EVENT_WHITEDAY_2006
void DropEventWhiteDay2006(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // #ifdef EVENT_WHITEDAY_2006

#ifdef EVENT_VALENTINE_2006
void DropEventValentine2006(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // #ifdef EVENT_VALENTINE_2006

#ifdef EVENT_XMAS_2005
void DropEventXMas2005(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // #ifdef EVENT_XMAS_2005

#ifdef EVENT_PEPERO
void DropEventPeperoItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // #ifdef EVENT_PEPERO

#ifdef EVENT_2PAN4PAN_MONEY
void DropEvent2pan4panMoneyCard(CNPC* npc, CPC* tpc, int level);
#endif // #ifdef EVENT_2PAN4PAN_MONEY

#ifdef EVENT_SEPTEMBER
void DropEventSeptemberItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // #ifdef EVENT_SEPTEMBER

#ifdef EVENT_CHUSEOK
void DropEventChuseokItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // #ifdef EVENT_CHUSEOK

#ifdef EVENT_OPEN_BETA_TLD
void DropEventChuseokItem(CNPC* npc, CPC* tpc, CPC* pc);
#endif // #ifdef EVENT_OPEN_BETA_TLD

#ifdef EVENT_FLOWER
void DropFlowerItem(int level, CNPC* npc, CPC* pc, CPC* tpc);
#endif

void DropLuckySpecialStone(CNPC* npc, CPC* pc, CPC* tpc, int level);

void DropBloodGem(CNPC* npc, CPC* pc, CPC* tpc, int level);

#ifdef EVENT_FRUIT_WATERMELON
void DropEventWaterMelon(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

#ifdef EVENT_FRUIT_MELON
void DropEventMelon(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

#ifdef EVENT_FRUIT_PLUM
void DropEventPlum(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

void DropBoosterItem(CNPC* npc, CPC* pc, CPC* tpc, int level);

#ifdef EVENT_RICESOUP
void DropRiceSoupItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif


int ApplyItem677_or_3579(CCharacter* ch, CSkill* skill, int itemidx);


#if defined (ENABLE_SINGLEDUNGEON_DATA)
void DropPersonalDungeon3Ticket(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

#ifdef ENABLE_SINGLEDUNGEON_DATA
void DropPersonalDungeon4Ticket(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

void DropSpecialRefineStone(CNPC* npc, CPC* pc, CPC* tpc, int level);
void DropPersonalDungeon2Ticket(CNPC* npc, CPC* pc, CPC* tpc, int level);;

#ifdef EVENT_WHITEDAY
void DropWhiteDayItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

#ifdef EVENT_VALENTINE
void DropValentineItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

#ifdef EVENT_NEWYEAR
void DropNewYearItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

#ifdef EVENT_LETTER
void DropLetterItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

#ifdef EVENT_LETTER_CHANGE
int GetLetterBit(int dbindex);
MSG_EVENT_LETTER_TYPE CheckLetterBit(int bitfield);
#endif

#ifdef EVENT_MOONSTONE_DROP
void DropMoonStoneItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

#ifdef EVENT_NEW_MOONSTONE
void DropNewMoonStoneItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_NEW_MOONSTONE

#ifdef ENABLE_DROP_PETEGG
void DropPetEgg(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

#ifdef EVENT_SAKURA
void DropSakuraItem(int level, CNPC* npc, CPC* pc, CPC* tpc);
#endif

#ifdef EVENT_RAIN_2006
void DropRain2006Item(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_RAIN_2006

#ifdef EVENT_COLLECT_BUG_DROP
void DropEventCollectBugItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_COLLECT_BUG_DROP

#ifdef EVENT_NEWSERVER_BASTARD
void DropNewserverBastardItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_NEWSERVER_BASTARD


#ifdef MAGIC_MONSTER_DROP_ACC
void DropMagicMonsterAccesary(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif//#ifdef MAGIC_MONSTER_DROP_ACC

#ifdef BLESS_WARRIOR
void RegenBlessWarrior(CNPC* npc, CPC* pc, CPC* tpc, int level);
void DropBlessWarrior(CNPC* pNPC);
#endif // BLESS_WARRIOR

#ifdef	EVENT_HALLOWEEN_2006
 void DropHalloween2006Item( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif	// EVENT_HALLOWEEN_2006

#ifdef EVENT_VALENTINE_2007
void DropEventValentine2007(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif	// EVENT_VALENTINE_2007

#ifdef EVENT_WHITEDAY_2007
void DropEventWhiteday2007(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif

#ifdef EVENT_TLD_2007_SONGKRAN
void DropEventSongkran2007( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif	// EVENT_TLD_2007_SONGKRAN

#ifdef MOB_SCROLL
void DropMobScrollSpecialStone(CNPC * npc, CPC* pc, CPC* tpc, int level);
#endif // MOB_SCROLL

#ifdef EVENT_EGGS_HUNT_2007
void DropEventEggsHunt2007(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_EGGS_HUNT_2007

#ifdef EVENT_EASTER_DAY_BRZ
void DropEventEasterDay(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_EASTER_DAY_BRZ

#ifdef EVENT_2007_PARENTSDAY
void DropEventParentsDay2007( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_GOMDORI_2007
void DropEventGomdori2007(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_CHILDRENSDAY_2007
void DropEventChildrensDay2007( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif // EVENT_CHILDRENSDAY_2007

#ifdef EVENT_FLOWERTREE_2007
void DropEventFlowerTree2007( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif // EVENT_FLOWERTREE_2007

#ifdef EVENT_INDEPENDENCE_DAY_2007_USA
void DropEventIndependenceDay2007USA(CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif // EVENT_INDEPENDENCE_DAY_2007_USA

#ifdef EVENT_SUMMER_VACATION_2007
void DropEventSummerVacation2007( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif // EVENT_SUMMER_VACATION_2007


#ifdef EVENT_TLD_MOTHERDAY_2007
void DropEventMotherday2007(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_TLD_MOTHERDAY_2007

#if defined(GIFT_EVENT_2007) || defined(EVENT_OPEN_ADULT_SERVER) || defined (EVENT_MAGIC_CARD)
void DropEventOpenAdultServer(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // defined(GIFT_EVENT_2007) || defined(EVENT_OPEN_ADULT_SERVER) || defined (EVENT_MAGIC_CARD)

#ifdef EVENT_DROPITEM
void DropEventNpcDropItem(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_DROPITEM

#ifdef EVENT_RICHYEAR_2007
void DropEventRichYear2007(CNPC* npc, CPC* pc, CPC* tpc, int level);
#endif // EVENT_RICHYEAR_2007



#ifdef EVENT_LC_1000DAY
void DropEventLC1000Day( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif // EVENT_LC_1000DAY

#ifdef EVENT_HALLOWEEN_2007
void DropEventHalloween2007( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif // EVENT_HALLOWEEN_2007

#ifdef EVENT_XMAS_2007
void DropEventXmas2007( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif //EVENT_XMAS_2007

#ifdef MONSTER_COMBO
void DropComboGiftMob( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif // MONSTER_COMBO

#ifdef SAKURA_EVENT_2008
void DropEventSakura2008( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif //SAKURA_EVENT_2008

#ifdef ATTACK_PET
void DropAPetLifeBook( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif // ATTACK_PET

#ifdef EVENT_PHOENIX	// //피닉스 이벤트 가입권 드랍 확률 yhj	
void DropPhoenix_MembersTicket( CNPC* npc, CPC* pc, CPC* tpc, int level );
#endif // EVENT_PHOENIX

#endif // __HARD_CODING_H__
