#include "stdhdrs.h"
#include "Server.h"
#include "CmdMsg.h"
#include "Log.h"
#include "hardcoding.h"

// npc - 사망 npc, pc - 공격한 캐릭터, tpc - 우선권 캐릭터, level - npc를 공격한 캐릭터의 평균레벨
typedef void (*NPC_DROP_FUNCTION) (CNPC* npc, CPC* pc, CPC* tpc, int level); 
void ProcDropItemAfterBattle(CNPC* df, CPC* opc, CPC* tpc, int level)
{
#ifdef MONSTER_COMBO
	if(df->m_pZone->IsComboZone())
	{
		DropComboGiftMob(df, opc, tpc, level);
		return ;
	}
#endif // MONSTER_COMBO

	// 아이템 드롭율 증폭제 검사
	bool hcSepDrop = false;
	bool hcDropPlus_S360 = false;

#ifdef NEW_CASHITEM_USA_2008
	bool hcPlatinumDrop = false;
#endif // NEW_CASHITEM_USA_2008

	if (opc && tpc == opc)
	{
		if (opc->m_assist.m_avAddition.hcSepDrop)
		{
			opc->m_assist.CureByItemIndex(884);	// 아이템
			hcSepDrop = true;
			CNetMsg rmsg;
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_DROP);
			SEND_Q(rmsg, opc->m_desc);
		}

		// 추천서버포션 드롭율 상승
		else if (opc->m_assist.m_avAddition.hcDropPlus_S360)
		{
			opc->m_assist.CureBySkillIndex(360);
			hcDropPlus_S360 = true;
		}
#ifdef NEW_CASHITEM_USA_2008
		else if(opc->m_assist.m_avAddition.hcPlatinumDrop)
		{
			opc->m_assist.CureByItemIndex(2855);	// 아이템
			hcPlatinumDrop = true;
		}
#endif // NEW_CASHITEM_USA_2008
	} // 아이템 드롭율 증폭제 검사
	
	// 아이템 드롭
	int loopcount;
	bool IsNotDropInCube = true;
#ifdef EXTREME_CUBE
	if(df->m_bCubeRegen)
		IsNotDropInCube = false;
#endif // EXTREME_CUBE
	for (loopcount = 0; loopcount < MAX_NPC_DROPITEM_LOOP; loopcount++)
	{
		// 5레벨보다 크면 아이템 드롭 없음
		// 1. 드롭할 수 있는 수 범위에서 아이템 선정
		// 2. 그 아이팀의 드롭확률로 드롭여부 결정
		// 3. 드롭
		if (df->m_proto->m_itemCount > 0 &&  ((level != -1 && level - df->m_level <= 5) ||  !IsNotDropInCube)  )
		{
			CItem* dropItem = NULL;
			int tableindex = GetRandom(0, MAX_NPC_DROPITEM - 1);
			int dropprob = df->m_proto->m_itemPercent[tableindex];

			dropprob = dropprob * gserver.m_itemDropProb / 100;

			if (tpc)
			{
#ifdef EVENT_TLD_MOTHERDAY_2007
				// Blessing of Mother
				if( tpc->m_assist.m_avAddition.hcDropPlus_2391 )
					dropprob *= 2;
#endif // EVENT_TLD_MOTHERDAY_2007
				
				// 자두
				if (tpc->m_assist.m_avAddition.hcDropPlus_838)
					dropprob *= 2;

				// 행운의스크롤
				if (tpc->m_assist.m_avAddition.hcScrollDrop)
					dropprob *= 2;
#ifdef EVENT_XMAS_2007
				if ( tpc->m_assist.m_avAddition.hcDropPlus_Xmas2007 > 0)
					dropprob *= tpc->m_assist.m_avAddition.hcDropPlus_Xmas2007;
#endif //EVENT_XMAS_2007
				
				// 행운 주문서
				if (tpc->m_assist.m_avAddition.hcDropPlus_2141)
				{
					if (GetRandom(1, 10000) <= 2000 ) // 20 %
					{
						dropprob *= 2;
					}
				}

#ifdef NEW_CASHITEM_USA_2008
				// 플래티늄 행운의 스크롤
				if(tpc->m_assist.m_avAddition.hcPlatinumScroll)
				{
					dropprob *= 4;
				}
#endif // NEW_CASHITEM_USA_2008
			}

			// BS 수정 : 아이템 드롭 이벤트
			dropprob = dropprob * gserver.m_nItemDropEventRate / 100;

			// 9월 이벤트 드롭율 10배
			if (hcSepDrop)
				dropprob = dropprob * 10;

			// 추천서버포션 드롭율 상승
			else if (hcDropPlus_S360)
				dropprob = dropprob * 10;

#ifdef NEW_CASHITEM_USA_2008
			else if (hcPlatinumDrop)
				dropprob = dropprob * 20;
#endif // NEW_CASHITEM_USA_2008

			// 드롭율 상승 %단위 누적
			if (opc && tpc == opc && opc->m_assist.m_avAddition.hcDropPlusPer100 > 0)
				dropprob += dropprob * opc->m_assist.m_avAddition.hcDropPlusPer100 / 100;
#ifdef DOUBLE_ITEM_DROP			
			if ( gserver.m_bDoubleItemEvent )
				dropprob += dropprob * gserver.m_bDoubleItemPercent / 100;

#endif // DOUBLE_ITEM_DROP

#ifdef ADULT_SERVER
			if( !hcSepDrop && !hcDropPlus_S360 )
			{
				int nPKLevel = 0;
				if( tpc )
					nPKLevel = tpc->GetPKLevel();
				else if ( opc )
					nPKLevel = opc->GetPKLevel();

				switch( nPKLevel )
				{
				case PK_NAME_CHAOS_KING :	dropprob = dropprob * 200 / 100; break;
				case PK_NAME_DARK_KNIGHT:	dropprob = dropprob * 150 / 100; break;
				case PK_NAME_ASSESIN :		dropprob = dropprob * 125 / 100; break;
				}
			}
#endif // ADULT_SERVER

			if (df->m_proto->m_item[tableindex] != -1 && GetRandom(1, 10000) <= dropprob)
			{
				if (df->m_proto->m_item[tableindex] == 84)
				{
					dropItem = df->m_pArea->DropItem(df->m_proto->m_item[tableindex], df, 0, df->m_level, 1);
				}
#if defined (LC_TWN2) || defined (LC_MAL) || defined (LC_USA) || defined(LC_BRZ)
				// 대만과 말레이시아는 중소형 테력약이 소형으로 떨어진다
				else if( df->m_proto->m_item[tableindex] == 44
						|| df->m_proto->m_item[tableindex] == 45 )
				{
					dropItem = df->m_pArea->DropItem(43, df, 0, 0, 1);

				}
				// 대만과 말레이시아는 중형 마나 회복 물약이 드롭되지 않는다.
				else if ( df->m_proto->m_item[tableindex] == 485 )
				{
					dropItem = NULL;
				}
#endif // #if defined (LC_MAL) || defined (LC_USA) || defined(LC_BRZ)
#if defined (LC_MAL) || defined (LC_USA) || defined (LC_BRZ)
				// 대형 힐링포션 제작서, 대형 마나, 중형 마나 메뉴얼 드롭 금지 
				else if ( df->m_proto->m_item[tableindex] == 1066 
					|| df->m_proto->m_item[tableindex] == 1067
					|| df->m_proto->m_item[tableindex] == 1068
					|| df->m_proto->m_item[tableindex] == 489)
				{
					dropItem = NULL;
				}
#endif // #if defined (LC_MAL) || defined (LC_USA) || defined (LC_BRZ)
				else
				{
					// 61레벨 무기류 및 65레벨 방어구 드롭 금지 제작서 드롭 금지
					switch (df->m_proto->m_item[tableindex])
					{
					case -1:	// 지우지 말것
						dropItem = NULL;
						break;

					default:
						{
							bool bAvailableDrop = true;
#ifdef LC_TWN2
							// 대만은 완성된 아이템 드롭은 없다
							const CItemProto* pItemProto = gserver.m_itemProtoList.FindIndex(df->m_proto->m_item[tableindex]);
							if (pItemProto)
							{
								if (pItemProto->m_typeIdx == 0 || pItemProto->m_typeIdx == 1)
									bAvailableDrop = false;
							}
#endif // LC_TWN2

							if (bAvailableDrop)
								dropItem = df->m_pArea->DropItem(df->m_proto->m_item[tableindex], df, 0, 0, 1, true);
							else
								dropItem = NULL;
						}
						break;
					} // switch (df->m_proto->m_item[tableindex])
				}
			} // if (df->m_proto->m_item[tableindex] != -1 && GetRandom(1, 10000) <= dropprob)

			if (dropItem)
			{
#ifdef LC_HBK
				dropItem->m_plus = 0;
#else
// 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기
				if (df->m_proto->m_minplus >= 0 && df->m_proto->m_maxplus >= df->m_proto->m_minplus && df->m_proto->m_probplus > 0 && dropItem->CanUpgrade())
				{
					if (GetRandom(1, 10000) <= df->m_proto->m_probplus)
					{
						dropItem->m_plus = GetRandom(df->m_proto->m_minplus, df->m_proto->m_maxplus);
					}
				}
// --- 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기
#endif // LC_HBK
				// Drop Msg 보내기
				CNetMsg dropMsg;
				// 아이템 우선권 셋팅 (같은 데미지 고려, 선공 고려)
				if (tpc)
					dropItem->m_preferenceIndex = tpc->m_index;
				else
					dropItem->m_preferenceIndex = -1;
				ItemDropMsg(dropMsg, df, dropItem);
				df->m_pArea->SendToCell(dropMsg, GET_YLAYER(dropItem), dropItem->m_cellX, dropItem->m_cellZ);

				if (df->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS))
				{
					GAMELOG << init("MOB DROP ITEM")
							<< "NPC INDEX" << delim
							<< df->m_proto->m_index << delim
							<< "NPC NAME" << delim
							<< df->m_name << delim
							<< "ITEM" << delim
							<< itemlog(dropItem)
							<< end;
				}
			}
		}
	} // // 아이템 드롭

	// typedef void (*NPC_DROP_FUNCTION) (CNPC* npc, CPC* pc, CPC* tpc, int level);
	// pc, tpc는 NULL이 될 수 있다
	NPC_DROP_FUNCTION fnNPCDrop[] = {

		DropBloodGem,						// 블러드젬 드롭 : 대만 천하대란
		DropLuckySpecialStone,				// 행운의 제련석 드롭 : 대만 천하대란
		DropSpecialRefineStone,				// 고제 드롭
		DropPersonalDungeon2Ticket,			// 퍼스널던전 2 입장권 드롭
		DropBoosterItem,					// 부스터

#ifdef ENABLE_SINGLEDUNGEON_DATA
		DropPersonalDungeon3Ticket,			// 퍼스널던전 3 입장권 드롭
#ifndef DISABLE_PD4
		DropPersonalDungeon4Ticket,			// 퍼스널던전 4 입장권 드롭
#endif // DISABLE_PD4
#endif // ENABLE_SINGLEDUNGEON_DATA

#ifdef ENABLE_DROP_PETEGG
		DropPetEgg,							// 애완동물 알 드롭
#endif

#ifdef EVENT_MOONSTONE
		DropMoonStoneItem,					// 문스톤 드롭
#endif

#ifdef EVENT_NEW_MOONSTONE
		DropNewMoonStoneItem,
#endif // EVENT_NEW_MOONSTONE

#ifdef EVENT_RICESOUP
		DropRiceSoupItem,					// 떡국 드롭
#endif

#ifdef EVENT_NEWYEAR
		DropNewYearItem,					// 설날이벤트
#endif

#ifdef EVENT_VALENTINE
		DropValentineItem,					// 발렌타인
#endif

#ifdef EVENT_WHITEDAY
		DropWhiteDayItem,					// 화이트데이
#endif

#ifdef EVENT_LETTER
		DropLetterItem,						// 낱말맞추기
#endif

#ifdef EVENT_FRUIT_WATERMELON
		DropEventWaterMelon,				// 수박
#endif

#ifdef EVENT_FRUIT_MELON
		DropEventMelon,						// 참외
#endif

#ifdef EVENT_FRUIT_PLUM
		DropEventPlum,						// 자두
#endif

#ifdef EVENT_CHUSEOK
		DropEventChuseokItem,				// 추석
#endif

#ifdef EVENT_SEPTEMBER
		DropEventSeptemberItem,				// 9월 이벤트
#endif // #ifdef EVENT_SEPTEMBER

#ifdef EVENT_PEPERO
		DropEventPeperoItem,				// 빼빼로
#endif // #ifdef EVENT_PEPERO

#ifdef EVENT_XMAS_2005
		DropEventXMas2005,
#endif // #ifdef EVENT_XMAS_2005

#ifdef EVENT_VALENTINE_2006					// 초콜릿
		DropEventValentine2006,
#endif // #ifdef EVENT_VALENTINE_2006

#ifdef EVENT_WHITEDAY_2006
		DropEventWhiteDay2006,				// 사탕
#endif // #ifdef EVENT_WHITEDAY_2006

#if defined(EVENT_NEW_SERVER_2006_OLDSERVER) || defined(EVENT_NEW_SERVER_2006_NEWSERVER)
		DropEventNewServer2006StatPotion,	// 공방향상
#endif // #if defined(EVENT_NEW_SERVER_2006_OLDSERVER) || defined(EVENT_NEW_SERVER_2006_NEWSERVER)

#ifdef EVENT_NEW_SERVER_2006_NEWSERVER
		DropEventNewServer2006NewServer,	// 경험치 향상
#endif // EVENT_NEW_SERVER_2006_NEWSERVER

#ifdef DROP_MAKE_DOCUMENT
		DropMakeDocument,					// 제작문서 드롭
#endif // DROP_MAKE_DOCUMENT

#ifdef RECOMMEND_SERVER_SYSTEM
		DropRecommendItem,					// 추천 서버 전용 인스턴스 포션 아이템
#endif // RECOMMEND_SERVER_SYSTEM

#ifdef EVENT_WORLDCUP_2006_VOTE
		DropFIFACupItem,					// 월드컵 이벤트 : FIFA CUP 모으기
#endif // EVENT_WORLDCUP_2006_VOTE

		DropGoldenBallItem,					// 골든볼 이벤트

#ifdef EVENT_RAIN_2006
		DropRain2006Item,					// 장마 이벤트
#endif // EVENT_RAIN_2006

#ifdef EVENT_TLD_BUDDHIST
		DropTldBuddhistItem,				// 불교 촛불 이벤트
#endif // EVENT_TLD_BUDDHIST

#ifdef EVENT_COLLECT_BUG_DROP
		DropEventCollectBugItem,			// 곤충 채집 이벤트 : 곤충 드롭
#endif // EVENT_COLLECT_BUG_DROP

#ifdef EVENT_NEWSERVER_BASTARD
		DropNewserverBastardItem,			// 신섭 바스타드 오픈 이벤트
#endif // EVENT_NEWSERVER_BASTARD
#ifdef MAGIC_MONSTER_DROP_ACC
		DropMagicMonsterAccesary,
#endif

#ifdef BLESS_WARRIOR
		RegenBlessWarrior,					// 전사의 축복
#endif // BLESS_WARRIOR

#ifdef EVENT_CHUSEOK_2006
		DropChuseok2006Item,				// 2006 추석 이벤트
#endif // EVENT_CHUSEOK_2006

#ifdef EVENT_HALLOWEEN_2006
		DropHalloween2006Item,				// 2006 할로윈 이벤트
#endif // EVENT_HALLOWEEN_2006

#ifdef MONSTER_RAID_SYSTEM
		DropRaidMonsterItem,
#endif // MONSTER_RAID_SYSTEM

#ifdef EVENT_XMAS_2006
		DropEventXMas2006Item,				// 2006 크리스마스 이벤트 아이템 드롭
#endif // EVENT_XMAS_2006

#ifdef EVENT_VALENTINE_2007
		DropEventValentine2007,
#endif //EVENT_VALENTINE_2007

#ifdef EVENT_WHITEDAY_2007
		DropEventWhiteday2007,
#endif //EVENT_WHITEDAY_2007


#ifdef EVENT_TLD_2007_SONGKRAN
		DropEventSongkran2007,
#endif // EVENT_TLD_2007_SONGKRAN

#ifdef EVENT_EGGS_HUNT_2007
		DropEventEggsHunt2007,
#endif // EVENT_EGGS_HUNT_2007

#ifdef EVENT_EASTER_DAY_BRZ
		DropEventEasterDay,
#endif // EVENT_EASTER_DAY_BRZ
		
#ifdef MOB_SCROLL
		DropMobScrollSpecialStone,
#endif	// MOB_SCROLL

#ifdef EVENT_2007_PARENTSDAY
		DropEventParentsDay2007,
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_GOMDORI_2007
		DropEventGomdori2007,
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_CHILDRENSDAY_2007
		DropEventChildrensDay2007,
#endif //EVENT_CHILDRENSDAY_2007
		
#ifdef EVENT_FLOWERTREE_2007
		DropEventFlowerTree2007,
#endif //EVENT_FLOWERTREE_2007

#ifdef EVENT_INDEPENDENCE_DAY_2007_USA
		DropEventIndependenceDay2007USA,
#endif // EVENT_INDEPENDENCE_DAY_2007_USA

#ifdef EVENT_SUMMER_VACATION_2007
		DropEventSummerVacation2007,
#endif // EVENT_SUMMER_VACATION_2007

#ifdef EVENT_TLD_MOTHERDAY_2007
		DropEventMotherday2007,
#endif //EVENT_TLD_MOTHERDAY_2007

#if defined(GIFT_EVENT_2007) || defined(EVENT_OPEN_ADULT_SERVER) || defined (EVENT_MAGIC_CARD)
		DropEventOpenAdultServer,
#endif // GIFT_EVENT_2007

#ifdef EVENT_DROPITEM
		DropEventNpcDropItem,
#endif // EVENT_DROPITEM

#ifdef EVENT_LC_1000DAY
		DropEventLC1000Day,
#endif // EVENT_LC_1000DAY

#ifdef EVENT_RICHYEAR_2007
		DropEventRichYear2007,
#endif // EVENT_RICHYEAR_2007
		
#ifdef EVENT_HALLOWEEN_2007
		DropEventHalloween2007,
#endif //EVENT_HALLOWEEN_2007
		
#ifdef EVENT_XMAS_2007
		DropEventXmas2007,
#endif // EVENT_XMAS_2007
		
#ifdef SAKURA_EVENT_2008
		DropEventSakura2008,
#endif //SAKURA_EVENT_2008
		
#ifdef ATTACK_PET
		DropAPetLifeBook,
#endif //ATTACK_PET

#ifdef EVENT_PHOENIX	// //피닉스 이벤트 가입권 드랍 확률 yhj	
		DropPhoenix_MembersTicket,
#endif // EVENT_PHOENIX
		
	}; // 드롭 함수 테이블

	// 방어코드 : tpc가 있는데 m_pZone이나 m_pArea가 없으면 NULL로 바꾼다
	if (tpc && (tpc->m_pZone == NULL || tpc->m_pArea == NULL))
		tpc = NULL;

	unsigned int fnDropLoop;
	for (fnDropLoop = 0; fnDropLoop < sizeof(fnNPCDrop) / sizeof(NPC_DROP_FUNCTION); fnDropLoop++)
		(fnNPCDrop[fnDropLoop])(df, opc, tpc, level);
	
	
	// 돈 떨어뜨릴 확률 : default 80 %
	// BS 수정 : 낮은 레벨 몬스터 잡을때 패널티만 존재
	// 잠수함 : 돈 드롭확률
	int moneyDropProb = MONEY_DROP_PROB * gserver.m_moneyDropProb / 100;
	if (level != -1 && df->m_level - level < 0)
		moneyDropProb += (df->m_level - level) * 500;
	
#ifdef CHANCE_EVENT
	if ( opc != NULL && tpc != NULL)
	{
		if ((gserver.m_bChanceEvent == true)
			&& ((gserver.CheckChanceEventLevel(tpc->m_level) == true) || (gserver.CheckChanceEventLevel(opc->m_level) == true)))
		{
			moneyDropProb = (moneyDropProb * gserver.m_bChanceNasPercent) / 100;
		}
	}
#endif // CHANCE_EVENT
	
	bool hcSepNas = false;
	if (opc && tpc == opc)
	{
		if (opc->m_assist.m_avAddition.hcSepNas)
		{
			opc->m_assist.CureByItemIndex(885);	// 나스
			hcSepNas = true;
			CNetMsg rmsg;
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_NAS);
			SEND_Q(rmsg, opc->m_desc);
		}
	}

	// 돈 드롭
	CItem* money = NULL;
	if (GetRandom(1, 10000) <= moneyDropProb)	// 80%
	{
		// 돈 액수 : +- 50%
		LONGLONG count = df->m_proto->m_price * GetRandom(50, 150) / 100;

		// 더블이벤트
#ifdef NEW_DOUBLE_GM
		if (gserver.m_bDoubleEvent)
		{
#ifdef NEW_DOUBLE_GM_ZONE
			if( gserver.m_bDoubleEventZone == -1 || gserver.m_bDoubleEventZone == df->m_pZone->m_index )
#endif // NEW_DOUBLE_GM_ZONE
				count = count * gserver.m_bDoubleNasPercent / 100 ;
		}
#else // NEW_DOUBLE_GM
		if (gserver.m_bDoubleEvent)
			count *= 2;
#endif // NEW_DOUBLE_GM				

		// 9월 이벤트 나스 10배
		if (hcSepNas)
			count = count * 10;

		// 행운 주문서
		if (tpc && tpc->m_assist.m_avAddition.hcDropPlus_2141)
		{
			if (GetRandom(1, 10000) <= 8000 ) // 80 %
			{
				count *= 2;
			}
		}

#ifdef CHANCE_EVENT
	if ( opc != NULL && tpc != NULL)
	{
		if ((gserver.m_bChanceEvent == true)
			&& ((gserver.CheckChanceEventLevel(tpc->m_level) == true) || (gserver.CheckChanceEventLevel(opc->m_level) == true)))
		{
			count = (count * gserver.m_bChanceNasGetPercent) / 100;
		}
	}
#endif // CAHNCE_EVENT
	
#ifdef DRATAN_CASTLE
#ifdef DYNAMIC_DUNGEON
 		if( opc && opc->m_pZone->m_index == ZONE_DRATAN_CASTLE_DUNGEON )
 		{
 			CDratanCastle * pCastle = CDratanCastle::CreateInstance();
 			if( pCastle )
			{
				if( opc->m_guildInfo && opc->m_guildInfo->guild()->index() == pCastle->GetOwnerGuildIndex()  )
				{
					// 세금 없음
				}
				else
				{
					LONGLONG tax=0;
					tax = count * pCastle->m_dvd.GetHuntRate() / 100;
					count = count - tax;
					gserver.AddTaxItemDratan( tax );
				}
			}
 		}
#endif //DYNAMIC_DUNGEON
#endif //DRATAN_CASTLE

		money = df->m_pArea->DropItem(gserver.m_itemProtoList.m_moneyItem->m_index, df, 0, 0, (LONGLONG)count);
	}
	if (money)
	{
		CNetMsg dropMsg;
		// 돈 우선권 셋팅
		if (tpc)
			money->m_preferenceIndex = tpc->m_index;
		else
			money->m_preferenceIndex = -1;
		ItemDropMsg(dropMsg, df, money);
		df->m_pArea->SendToCell(dropMsg, GET_YLAYER(money), money->m_cellX, money->m_cellZ);

		if (df->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS))
		{
			GAMELOG << init("MOB DROP MONEY")
					<< "NPC INDEX" << delim
					<< df->m_proto->m_index << delim
					<< "NPC NAME" << delim
					<< df->m_name << delim
					<< "ITEM" << delim
					<< itemlog(money, true)
					<< end;
		}
	}
}
