#include "stdhdrs.h"
#include "Server.h"
#include "Battle.h"
#include "WarCastle.h"
#include "CmdMsg.h"
#include "Exp.h"
#include "Log.h"
#include "doFunc.h"

void ProcDead(CPC* df, CCharacter* of)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL && df != NULL)
	{
		pCastle->CheckRespond(df);
	}
#endif // DRATAN_CASTLE

	const char* strOFType = "UNKNOWN";
	const char* strOFName = "UNKNOWN";
	int strOFIndex = 0;

	CPC*		opc				= NULL;
	CNPC*		onpc			= NULL;
#ifdef ENABLE_PET
	CPet*		opet			= NULL;
#endif // #ifdef ENABLE_PET
	CElemental*	oelemental		= NULL;
#ifdef ATTACK_PET
	CAPet*		oapet			= NULL;
#endif //ATTACK_PET

	switch (of->m_type)
	{
	case MSG_CHAR_PC:
		opc = TO_PC(of);
		strOFType = "PC";
		strOFName = opc->GetName();
		strOFIndex = opc->m_index;
		break;

	case MSG_CHAR_NPC:
		onpc = TO_NPC(of);
		strOFType = "NPC";
		strOFName = onpc->m_name;
		strOFIndex = onpc->m_idNum;
		break;

	case MSG_CHAR_PET:
		opet = TO_PET(of);
		opc = opet->GetOwner();
		if (opc == NULL)
			return ;
		strOFType = "PET";
		strOFName = opc->GetName();
		strOFIndex = opc->m_index;
		break;

	case MSG_CHAR_ELEMENTAL:
		oelemental = TO_ELEMENTAL(of);
		opc = oelemental->GetOwner();
		if (opc == NULL)
			return ;
		strOFType = "ELEMENTAL";
		strOFName = opc->GetName();
		strOFIndex = opc->m_index;
		break;
#ifdef ATTACK_PET
	case MSG_CHAR_APET:
		oapet = TO_APET(of);
		opc = oapet->GetOwner();
		if (opc == NULL)
			return ;
		strOFType = "APET";
		strOFName = opc->GetName();
		strOFIndex = opc->m_index;
		break;
#endif //ATTACK_PET

	default:
		return ;
	}

	// NPC에 의한 사망시 사망 패널티는 기본으로 true, PC에게 사망시 사망 패널티는 기본으로 false
	// * bPKPenalty변수는 pk 패널티를 주는것 뿐만 아니라 성향회복에도 관계되므로 성향 회복이나 패널티등 어느것에라도 걸리면 true
	bool bPvP = (opc) ? true : false;
	bool bPKPenalty = (opc) ? IsPK(opc, df) : false;
	bool bDeadPenalty = (bPvP) ? false : true;

#ifdef NEW_PK
	bool bRestorePKOfDefensePC = true;

#ifdef LC_KOR
	// 060318 : bs : 공방 모두 카오PC PvP 시에는 죽은 사람 회복 없음
	//             : 죽은카오가 선공이면 회복 없음
	if (   opc											// PC -> PC 이고
		&& opc->IsChaotic()								// 둘다 카오 캐릭이고
		&& df->IsChaotic() )
	{
		bRestorePKOfDefensePC = false;
	}
	if (   opc											// PC -> PC 이고
		&& !opc->IsChaotic()							// 일반 -> 카오이고
		&& df->IsChaotic()
		&& IsFirstAttackInAttackList(df, opc) )			// 죽은 카오가 먼저 때렸을 때
	{
		bRestorePKOfDefensePC = false;
	}
#endif // LC_KOR
#endif

	// 변신 해제
	if (df->IsSetPlayerState(PLAYER_STATE_CHANGE))
		df->CancelChange();

	if (opc)
	{
#ifndef DISABLE_PKPENALTY
#ifdef FREE_PK_SYSTEM
		if( !gserver.m_bFreePk )
		{
#endif // FREE_PK_SYSTEM

#ifdef MAL_DISABLE_PKPENALTY
		if( gserver.m_bDisablePKPaenalty )
			bDeadPenalty = true;
		 else if( !gserver.m_bDisablePKPaenalty )
		{
#endif // MAL_DISABLE_PKPENALTY
		// df가 pk모드 이거나 카오면 둘다 트루
		if (df->IsSetPlayerState(PLAYER_STATE_PKMODE) || df->IsChaotic())
			bDeadPenalty = true;
#ifdef MAL_DISABLE_PKPENALTY
		}
#endif // MAL_DISABLE_PKPENALTY

#ifdef FREE_PK_SYSTEM
		}
#endif // FREE_PK_SYSTEM
#endif

		// 길드전
		if (opc->m_guildInfo && (opc->m_guildInfo->guild()->battleState() == GUILD_BATTLE_STATE_ING) &&
			df->m_guildInfo && (df->m_guildInfo->guild()->battleState() == GUILD_BATTLE_STATE_ING))
		{
			if (opc->m_guildInfo->guild()->battleIndex() == df->m_guildInfo->guild()->index() && 
				df->m_guildInfo->guild()->battleIndex() == opc->m_guildInfo->guild()->index())
			{
				bDeadPenalty = false;
				
				int killCount = opc->m_guildInfo->guild()->killCount();
				killCount++;
				
				CNetMsg rmsg;

				if (IS_RUNNING_HELPER)
				{
					HelperGuildBattleKillReqMsg(rmsg, opc->m_guildInfo->guild()->index(), df->m_guildInfo->guild()->index());
					SEND_Q(rmsg, gserver.m_helper);
				}
				else
				{
					GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
					SEND_Q(rmsg, opc->m_desc);
				}
			}
		}
	} // 공격자가 PC 또는 PC의 소유물일때

#ifdef NEW_PK
#else
	// 정당방위 해제
	DelRaList(df);
#endif


#ifdef ENABLE_WAR_CASTLE
	// 공성 포인트 계산
	if (opc)
		CalcWarPoint(opc, df);
	else
		CalcWarPoint(of, df);

	// 공성 도중 사망은 패널티 없음
	CWarCastle* castle = CWarCastle::GetCastleObject(df->m_pZone->m_index);
	if (castle && castle->GetState() != WCSF_NORMAL && df->GetMapAttr() == MAPATT_WARZONE)
		bDeadPenalty = false;
#endif

	/////////////////////////////////////////////
	// BANGWALL : 2005-07-18 오전 11:27:24
	// Comment : freepkzone 패널티 없음
	// 공격자와 방어자가 모두 freepkzone에 있으면 pkpenalty 없음
	if( of->GetMapAttr() == df->GetMapAttr() && of->GetMapAttr() == MAPATT_FREEPKZONE)
		bDeadPenalty = false;

	// PvP에서 PK 적용
	if (opc && bPvP && bPKPenalty)
		CalcPKPoint(opc, df, false);

#ifdef NEW_PK
	bool bDeadExpPenalty = true;
#endif

#ifdef FREE_PK_SYSTEM
	if( gserver.m_bFreePk )
	{
		if(!bDeadPenalty)
			bDeadExpPenalty = false;
	}
#endif // FREE_PK_SYSTEM

#ifdef LC_KOR
	// 국내에서는 절대 PvP시에 경험치 다운이 없다
	if (bPvP)
#ifdef NEW_PK
#ifdef MAL_DISABLE_PKPENALTY
		if( gserver.m_bDisablePKPaenalty )
		{
		bDeadExpPenalty = true;
		}
#else // MAL_DISABLE_PKPENALTY
		bDeadExpPenalty = false;
#endif // // MAL_DISABLE_PKPENALTY

#else
		bDeadPenalty = false;
#endif
#endif

// 브라질은 m_bDisablePKPaenalty true일 경우 경험치 다운 없다.
#ifdef LC_BRZ
		if (bPvP)
#ifdef NEW_PK
#ifdef MAL_DISABLE_PKPENALTY
			if( gserver.m_bDisablePKPaenalty )
			{
				bDeadExpPenalty = false;
			}
#endif // MAL_DISABLE_PKPENALTY
#else
			bDeadPenalty = false;
#endif
#endif // LC_BRZ

	// 퍼스널 던전은 무조건 패널티 없음
	if (df->m_pZone->IsPersonalDungeon())
		bDeadPenalty = false;

	// 경험치 하락
	LONGLONG nLoseExp = 0;
	int nLoseSP = 0;
// TODO : DELETE	bool bSaveLose = true;

	// 하락된 경험치는 최근 것만 기억
	df->m_loseexp = 0;
	df->m_losesp = 0;

	if (bDeadPenalty)
	{
#ifdef NEW_PK
		if (bDeadExpPenalty)
#endif
		{
			// 사망시 패널티
			if (df->m_level < 11)
			{
				nLoseExp = (LONGLONG)(GetLevelupExp(df->m_level) * DEATH_PENALTY_EXP_1);
				nLoseSP = (int)(df->m_skillPoint * DEATH_PENALTY_SP_1);
			}
			else if (df->m_level < 21)
			{
				nLoseExp = (LONGLONG)(GetLevelupExp(df->m_level) * DEATH_PENALTY_EXP_2);
				nLoseSP = (int)(df->m_skillPoint * DEATH_PENALTY_SP_2);
			}
			else if (df->m_level < 36)
			{
				nLoseExp = (LONGLONG)(GetLevelupExp(df->m_level) * DEATH_PENALTY_EXP_3);
				nLoseSP = (int)(df->m_skillPoint * DEATH_PENALTY_SP_3);
			}
			else
			{
				nLoseExp = (LONGLONG)(GetLevelupExp(df->m_level) * DEATH_PENALTY_EXP_4);
				nLoseSP = (int)(df->m_skillPoint * DEATH_PENALTY_SP_4);
			}

#ifdef ADULT_SERVER  
			// 경험치 감소
			switch( df->GetPKLevel() )
			{
			case PK_NAME_CHAOS_KING :	nLoseExp = nLoseExp * 200 / 100; break;
			case PK_NAME_DARK_KNIGHT :	nLoseExp = nLoseExp * 175 / 100; break;
			case PK_NAME_ASSESIN :		nLoseExp = nLoseExp * 150 / 100; break;
			case PK_NAME_MURDERER :		nLoseExp = nLoseExp * 125 / 100; break;
			case PK_NAME_OUTLAW :		nLoseExp = nLoseExp * 110 / 100; break;
			case PK_NAME_HUNTER :		nLoseExp = nLoseExp * 95 / 100; break;
			case PK_NAME_HUNTER_MASTER :nLoseExp = nLoseExp * 90 / 100; break;
			case PK_NAME_KNIGHT :		nLoseExp = nLoseExp * 85 / 100; break;
			case PK_NAME_SAINT_KNIGHT :	nLoseExp = nLoseExp * 80 / 100; break;
			case PK_NAME_GUARDIAN :		nLoseExp = nLoseExp * 70 / 100; break;
			}

			// 페널티 수치 변경 알리기
			if( df->IsChaotic() )
			{
				df->m_pkPenalty += 10;
				if( df->m_pkPenalty > 0 )		//최대 회복수치 0
					df->m_pkPenalty = 0;
			}
			else // 헌터
			{
				df->m_pkPenalty -= 10;
				if( df->m_pkPenalty < 0 )		//최대 회복수치 0
					df->m_pkPenalty = 0;
			}
		
			CNetMsg rmsg;
			CharStatusMsg(rmsg, df, 0);
			df->m_pArea->SendToCell(rmsg, df, false);
			df->m_bChangeStatus = true;
			df->CalcStatus(true);
			
			char cChangeKing = df->IsUpdateKing();
			if( cChangeKing )
			{
				CNetMsg KingChangeMsg;		// MSG_HELPER_UPDATE_CHAOKING  bChao : 1(Guardian),2(ChaoKing) , index , penalty
				HelperUpdateKing( KingChangeMsg, cChangeKing, df->m_index, df->m_pkPenalty );
				SEND_Q( KingChangeMsg, gserver.m_helper );
			}
#endif // ADULT_SERVER

			// 경험의 결정 시리즈 적용
			switch (df->m_assist.m_avAddition.hcDeathExpPlus)
			{
			case 1:		nLoseExp -= 50000;		break;
			case 2:		nLoseExp -= 600000;		break;
			case 3:		nLoseExp /= 2;			break;
			default:							break;
			}
			
			// 노력의 결정 적용
			if (df->m_assist.m_avAddition.hcDeathSPPlus)
				nLoseSP /= 2;
		}

#ifndef ADULT_SERVER

#ifdef FREE_PK_SYSTEM
		if( !gserver.m_bFreePk )
		{
#endif // FREE_PK_SYSTEM

#ifdef MAL_DISABLE_PKPENALTY
		if( !gserver.m_bDisablePKPaenalty )
		{
#endif // MAL_DISABLE_PKPENALTY
		if (df->IsChaotic())
		{
// TODO : DELETE			bSaveLose = false;
			bool bseal = false;
			CNetMsg rmsg;

			if (df->m_pkPenalty <= -130)			{ nLoseExp = nLoseExp * 225 / 100; bseal = (GetRandom(1, 100) <= 13) ? true : false; }
			else if (df->m_pkPenalty <= -100)		{ nLoseExp = nLoseExp * 200 / 100; bseal = (GetRandom(1, 100) <= 11) ? true : false; }
			else if (df->m_pkPenalty <=  -70)		{ nLoseExp = nLoseExp * 175 / 100; bseal = (GetRandom(1, 100) <=  9) ? true : false; }
			else if (df->m_pkPenalty <=  -40)		{ nLoseExp = nLoseExp * 150 / 100; bseal = (GetRandom(1, 100) <=  7) ? true : false; }
			else if (df->m_pkPenalty <=  -10)		{ nLoseExp = nLoseExp * 125 / 100; bseal = (GetRandom(1, 100) <=  5) ? true : false; }

			if (bseal)
			{
				CItem* table[MAX_WEARING];
				memset(table, 0, sizeof(CItem*) * MAX_WEARING);
				int i = 0, j = 0;
				while (i < MAX_WEARING)
				{
					if (df->m_wearing[i] && !(df->m_wearing[i]->m_flag & FLAG_ITEM_SEALED))
					{
						// 장비에 따라 봉인 되는지 결정
						switch (i)
						{
						case WEARING_HELMET:
						case WEARING_ARMOR_UP:
						case WEARING_WEAPON:
						case WEARING_ARMOR_DOWN:
						case WEARING_SHIELD:
						case WEARING_GLOVE:
						case WEARING_BOOTS:
							table[j] = df->m_wearing[i];
							j++;
							break;

						default:
							break;
						}
					}
					i++;
				}
				if (j)
				{
					i = GetRandom(0, j - 1);
					if (table[i])
					{
						table[i]->m_flag |= FLAG_ITEM_SEALED;
						ItemUpdateMsg(rmsg, table[i], 0);
						SEND_Q(rmsg, df->m_desc);
						PKItemSealMsg(rmsg, table[i]);
						SEND_Q(rmsg, df->m_desc);
						GAMELOG << init("ITEM SEAL" , df)
								<< "ITEM" << delim
								<< itemlog(table[i])
								<< end;
					}
				}
			}

			if (nLoseExp < 0)
				nLoseExp = 0;
			if (nLoseSP < 0)
				nLoseSP = 0;

			// 060318 : bs : 공방 모두 카오PC PvP 시에는 죽은 사람 회복 없음
			//             : 죽은카오가 선공이면 회복 없음
			// 성향 회복
#ifdef NEW_PK
			if (bRestorePKOfDefensePC)
#endif
			{
#ifdef RESTRICT_PK
				if (df->m_lastPKTime == -1 || df->m_lastPKTime + RESTRICT_PK <= gserver.m_gameTime)
				{
#endif // RESTRICT_PK
#if defined (NON_PK_SYSTEM)
					if( !gserver.m_bNonPK )
						df->m_pkPenalty += 5;
#else
					df->m_pkPenalty += 5;
#endif
					if (df->m_pkPenalty > 0)
						df->m_pkPenalty = 0;
#ifdef RESTRICT_PK
				}
#endif // RESTRICT_PK
			}

			// 페널티 수치 변경 알리기
			CharStatusMsg(rmsg, df, 0);
			df->m_pArea->SendToCell(rmsg, df, false);
			df->m_bChangeStatus = true;
			df->CalcStatus(true);
		}
		else
		{
			if (df->m_exp < nLoseExp)
				nLoseExp = df->m_exp;
		}
#ifdef MAL_DISABLE_PKPENALTY
		}
		else 
		{
			if( df->m_exp < nLoseExp )
				nLoseExp = df->m_exp;
		}
#endif 
#ifdef FREE_PK_SYSTEM
		}
		else 
		{
			if( df->m_exp < nLoseExp )
				nLoseExp = df->m_exp;
		}
#endif // FREE_PK_SYSTEM

#else // #ifndef DISABLE_PKPENALTY
		if (df->m_exp < nLoseExp)
			nLoseExp = df->m_exp;
#endif // #ifndef DISABLE_PKPENALTY

#ifdef NEW_PK
		if (bDeadExpPenalty)
#endif
		{
			if (nLoseExp < 0)
				nLoseExp = 0;
			if (nLoseSP < 0)
				nLoseSP = 0;
			if (df->m_skillPoint < nLoseSP)
				nLoseSP = df->m_skillPoint;
// 경험치 - 방지
			if ( df->m_exp < nLoseExp )
				nLoseExp = df->m_exp;

// 수정
#ifdef MONSTER_COMBO
			if(df->m_pZone->IsComboZone())
			{
				nLoseExp = nLoseExp / 10;
				nLoseSP = nLoseSP / 10;
			}
#endif // MONSTER_COMBO
			df->m_exp -= nLoseExp;
			df->m_skillPoint -= nLoseSP;
			if (df->m_skillPoint < 0)
				df->m_skillPoint = 0;
			df->m_bChangeStatus = true;

// TODO : DELETE			if (bSaveLose)
// TODO : DELETE			{
				df->m_loseexp = nLoseExp;
				df->m_losesp = nLoseSP;
// TODO : DELETE			}
		}
	} // 사망 패널티 적용

#ifndef ADULT_SERVER
	// Accessory 내구도
	int i;
	for (i = WEARING_ACCESSORY1; i <= WEARING_ACCESSORY3; i++)
	{
		if (!df->m_wearing[i] || df->m_wearing[i]->m_itemProto->m_maxUse == -1)
			continue;

		df->m_wearing[i]->m_used -= ACCESSORY_USED_DEATH;

		// 악세사리 소멸
		if (df->m_wearing[i]->m_used <= 0)
		{
			df->m_wearing[i]->m_used = 0;
			df->CalcStatus(true);
		}
	}
#endif // ADULT_SERVER
		
	// 보조효과 리셋
	df->m_assist.ClearAssist(true, false, true, true, false);

	// 워프중이면 취소
	if (df->IsSetPlayerState(PLAYER_STATE_WARP))
	{
		df->m_reqWarpTime = 0;
		df->m_reqWarpType = -1;
		df->m_reqWarpData = -1;
	}

	df->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING | PLAYER_STATE_WARP | PLAYER_STATE_PKMODE);

#ifdef ENABLE_PET
	CPet* pet = df->GetPet();
	if (pet)
	{
		// 펫 착용 중 사망시 펫 소환 해제
		CNetMsg rmsg;
		ItemWearMsg(rmsg, WEARING_PET, NULL, NULL);
		do_ItemWear(df, rmsg);
		if (pet->IsMountType())
		{
			// 사망 설정
			pet->SetRemainRebirthTime();
		}
		// 펫 상태 보냄
		ExPetStatusMsg(rmsg, pet);
		SEND_Q(rmsg, df->m_desc);
	}
	// 펫 마운트 중이면
//	CPet* pet = df->GetPet();
//	if (pet && pet->IsMount())
//	{
//		ProcDeathPet(pet, "OWNER DEATH", df->m_index, df->m_name);
//	}
#endif

#ifdef ATTACK_PET
#ifdef LC_USA
	CAPet* apet = df->GetAPet();
	if(apet)
	{
		CNetMsg rmsg;
		ItemWearMsg(rmsg, WEARING_PET, NULL, NULL);
		do_ItemWear(df, rmsg);

		// 펫 상태 보냄
		ExAPetStatusMsg(rmsg, apet);
		SEND_Q(rmsg, df->m_desc);
	}
#endif // LC_USA
#endif // ATTACK_PET

	// 소환 취소
	while (df->m_elementalList)
		df->UnsummonElemental(df->m_elementalList);
	// 강신 취소
	if (df->m_evocationType != EVOCATION_NONE)
		df->Unevocation();
	// 강신 시간 초기화
	df->m_pulseEvocation[0] = 0;
	df->m_pulseEvocation[1] = 0;

	// 로그
	GAMELOG << init("CHAR_DEATH", df)
			<< "BY" << delim
			<< strOFType << delim
			<< strOFName << delim
			<< strOFIndex << delim
			<< "LOSE EXP" << delim
			<< nLoseExp << delim
			<< "CUR EXP" << delim
			<< df->m_exp << delim
			<< "LOSE SP" << delim
			<< nLoseSP << delim
			<< "CUR SP" << delim
			<< df->m_skillPoint
			<< end;

	df->SaveNow(false);

	DelAttackList(df);

#ifdef NEW_PK
	// 정당방위 해제
	DelRaList(df);
#endif

#ifdef ADULT_SERVER
	// 명성 페널티
	DelRaList(df);
	if( IS_PC(of) )  // PC에 의해 죽으면 공격자 정당방위 해제 
		DelRaList(TO_PC(of));

	CNetMsg heperFameDown;
	switch( df->GetPKLevel() )
	{
	case PK_NAME_KNIGHT :		HelperLevelUpFameUpReqMsg(heperFameDown, df->m_index, -50 );	df->m_fame += -50;		SEND_Q(heperFameDown, gserver.m_helper); break;
	case PK_NAME_SAINT_KNIGHT :	HelperLevelUpFameUpReqMsg(heperFameDown, df->m_index, -75 );	df->m_fame += -70;		SEND_Q(heperFameDown, gserver.m_helper); break;
	case PK_NAME_GUARDIAN :		HelperLevelUpFameUpReqMsg(heperFameDown, df->m_index, -100 );	df->m_fame += -100;		SEND_Q(heperFameDown, gserver.m_helper); break;
	}
	if( df->m_fame < 0 )
		df->m_fame = 0;

	// 자신의 아이템 드랍
	int nprob = 0;
	switch( df->GetPKLevel() )
	{
	case PK_NAME_KNIGHT :		case PK_NAME_ASSESIN :		nprob = 50;		break;
	case PK_NAME_SAINT_KNIGHT :	case PK_NAME_DARK_KNIGHT : 	nprob = 100;	break;
	case PK_NAME_GUARDIAN :		case PK_NAME_CHAOS_KING :	nprob = 150;	break;
	}

	if( nprob > GetRandom( 0, 1000 ) ) // 인벤 아이템 드랍 시키기
	{
		CNetMsg ItemDown;
		bool	bDrop = false;
		// 인벤에서 찾아서

		CItem* dropItem = NULL ;
		for( int i=0; i< MAX_INVENTORY_ROWS ; i++ )
		{
			int nRowProb = 0;
			if( i%ITEMS_PER_ROW == 0 )
				nRowProb = GetRandom( 0, ITEMS_PER_ROW );
			else
				nRowProb = GetRandom( 0, MAX_INVENTORY_ROWS-1 );

			dropItem = df->m_invenNormal.GetItem( GetRandom( 0, ITEMS_PER_ROW-1 ), nRowProb );
			if(  dropItem != NULL )
			{
				// 입고 있는 아이템 이면 벗기
				if ( ( dropItem->CanDrop() ) 										// 드랍 가능아이템 이고
					&& !( dropItem->m_flag & FLAG_ITEM_COMPOSITION )				// 결합아이템이 아니고
					&& !( dropItem->IsLent() )										// 대여가 아니고 
#ifdef PLATINUM
					&& !( dropItem->IsUsedPlatinumSpecial() )
#endif // PLATINUM
#ifdef ENABLE_PET
					&& !( dropItem->IsPet() ) // 애완동물이 아니면 
#endif // ENABLE_PET 					
					)
				{
					if( dropItem->m_wearPos != WEARING_NONE )						// 입은 아이템 이면 벗자	
					{
						df->m_wearing[dropItem->m_wearPos] = NULL;
						ItemWearMsg(ItemDown, dropItem->m_wearPos, NULL, dropItem);
						dropItem->m_wearPos = WEARING_NONE;
						SEND_Q(ItemDown, df->m_desc);
						bDrop = true;
						break;
					}
				}				
			}
		}
		

		if( bDrop && dropItem != NULL )
		{
			CItem* pNewDropItem = dropItem;
			if( dropItem->Count() > 1 )
			{
				int dropCount = 1;
				int dropMaxCount = 1; 
				
				// 떨어뜨릴 아이템 갯수
				if( pNewDropItem->m_itemProto->m_subtypeIdx == IETC_MONEY  ) // 돈인경우 최대치 결정
				{
					dropMaxCount = dropItem->Count() * 10 / 100 ;  // 최대 돈의 10%
				}
				else
				{
					dropMaxCount = dropItem->Count() * 75 / 100 ;  // 최대 전체 개수의 75%
				}
				dropCount = GetRandom( 0, dropMaxCount );

				if( dropCount < 1 )
					dropCount = 1;	//최소 1개 

				// 인벤에 아이템 감소
				DecreaseFromInventory( df , dropItem, dropCount);
				ItemUpdateMsg( ItemDown , dropItem, -dropCount);
				SEND_Q(ItemDown, df->m_desc);
				
				// 감소된 아이템 생성
				pNewDropItem =  gserver.m_itemProtoList.CreateItem( dropItem->m_idNum , dropItem->m_wearPos , dropItem->m_plus , dropItem->m_flag , dropCount );
			}
			else // 아이템 인벤에서만 제거 메모리 해제하지 않음
			{
				ItemDeleteMsg(ItemDown, pNewDropItem);
				SEND_Q(ItemDown, df->m_desc);
				RemoveFromInventory(df, pNewDropItem, false, true);
			}

			of->m_pArea->DropItem( pNewDropItem, of );	// 공격자 포지션에 아이템 드랍
			pNewDropItem->m_preferenceIndex = IS_PC(of)? of->m_index:-1 ;
			ItemDropMsg(ItemDown, df, pNewDropItem );
			of->m_pArea->SendToCell( ItemDown, of , true);
			
			GAMELOG << "PK ITEMDROP" << itemlog(pNewDropItem)
				<< "ATTACK :" << of->m_name << delim << "DIE : " << df->m_name;
		}
	}		
#endif // ADULT_SERVER


#ifdef DRATAN_CASTLE
	if (pCastle != NULL)
	{
		if (df->GetJoinFlag(ZONE_DRATAN) != WCJF_NONE
			&& pCastle->GetState() != WCSF_NORMAL
			&& df->GetMapAttr() == MAPATT_WARZONE)
		{	// 공성중에 공성참가
			CNetMsg rmsg;
			int wait_time = -1;
			switch(df->GetJoinFlag(ZONE_DRATAN))
			{
			case WCJF_ATTACK_GUILD:
				// 부활진기가 있으면 부활대기 시간 20초, 없으면 60초
				wait_time = 60;
				if (df->m_guildInfo != NULL 
					&& df->m_guildInfo->guild() != NULL)
				{
					for(int i=0; i<7; i++)
					{
						if (df->m_guildInfo->guild()->index() == pCastle->m_nRebrithGuild[i])
						{
							wait_time = 20;
							break;
						}
					}
				}
				break;
			case WCJF_OWNER:
			case WCJF_DEFENSE_GUILD:
				//  부활대기 시간 60초 - 워프타워개수*10 
				int count = 0;
				for(int i=0; i<5; i++)
				{
					if(pCastle->m_pWarpNPC[i] != NULL 
						&& DEAD(pCastle->m_pWarpNPC[i]) == false)
					{
						count++;
					}
				}
				
				wait_time = 60 - count*10;
				break;
			}

			if (wait_time > 0)
			{
				WaitTimeMsg(rmsg, wait_time);
				SEND_Q(rmsg, df->m_desc);
			}
		}
	}
#endif // DRATAN_CASTLE

#ifdef EXTREME_CUBE
	if(gserver.m_extremeCube.IsGuildCubeTime())
	{
		if(df->m_guildInfo && df->m_guildInfo->guild())
		{
			if(df->m_pZone != NULL && df->m_guildInfo->guild()->m_cubeUniqueIdx >= 0 && df->m_pZone->IsExtremeCube())
			{
				CCubeMemList* memlist = gserver.m_extremeCube.FindMemList(df->m_guildInfo->guild()->m_cubeUniqueIdx);
				if(memlist)
				{
					memlist->DelPC(df);

					if(opc && opc->m_guildInfo && opc->m_guildInfo->guild())
					{
						CCubeMemList* opcMemList = gserver.m_extremeCube.FindMemList(opc->m_guildInfo->guild());
						if(opcMemList)
						{
							CNetMsg msg;
							long lastCubePoint;
							int point;
							time(&lastCubePoint);

							HelperAddCubePointMsg(msg, opc->m_guildInfo->guild()->index(), df->m_level * 10, lastCubePoint);
							SEND_Q(msg, gserver.m_helper);

							// 개인 큐브포인트 획득
							point = opcMemList->GetPersonalCubePoint(opc, df->m_level);
							HelperAddCubePointPersonalMsg(msg, opc->m_index, point, lastCubePoint);
							SEND_Q(msg, gserver.m_helper);
						}
					}
				}
			}
		}
	}
	else
	{
		if(df->m_party)
		{
			if(df->m_pZone != NULL && df->m_party->m_cubeUniqueIdx >= 0 && df->m_pZone->IsExtremeCube())
			{
				CCubeMemList* memlist = gserver.m_extremeCube.FindMemList(df->m_party->m_cubeUniqueIdx);
				if(memlist)
				{
					memlist->DelPC(df);
					// 개인 큐브포인트 획득
					if(opc && opc->m_party)
					{
						CCubeMemList* opcMemList = gserver.m_extremeCube.FindMemList(opc->m_party);
						if(opcMemList)
						{
							int point;
							long lastCubePoint;
							CNetMsg msg;

							time(&lastCubePoint);
							point = opcMemList->GetPersonalCubePoint(opc, df->m_level);
							HelperAddCubePointPersonalMsg(msg, opc->m_index, point, lastCubePoint);
							SEND_Q(msg, gserver.m_helper);
						}
					}
				}
			}
		}
	}
#endif // EXTREME_CUBE
}
