#include "stdhdrs.h"
#include "Server.h"
#include "Battle.h"
#include "WarCastle.h"
#ifdef DRATAN_CASTLE
#include "DratanCastle.h"
#endif // DRATAN_CASTLE

// of가 df를 공격할 때, 공격 못하는 예외 처리
bool IsAvailableAttack(CCharacter* of, CCharacter* df)
{
	CPC*		opc				= NULL;
	CPC*		dpc				= NULL;
	CNPC*		onpc			= NULL;
	CNPC*		dnpc			= NULL;
#ifdef ENABLE_PET
	CPet*		opet			= NULL;
	CPet*		dpet			= NULL;
#endif // #ifdef ENABLE_PET
	CElemental*	oelemental		= NULL;
	CElemental*	delemental		= NULL;
#ifdef ATTACK_PET
	CAPet*		oapet			= NULL;
	CAPet*		dapet			= NULL;
#endif //ATTACK_PET

	// 죽은 사람은 더이상 공격 불가
	if (DEAD(df))
		return false;

	// 피스존에서는 공격 불가
#ifndef ADULT_SERVER
	if (of->IsInPeaceZone(true) || df->IsInPeaceZone(true))
		return false;
#endif // ADULT_SERVER

	// 방어자 안보이면 공격 불가
	if (!of->CanVisible(df))
		return false;

	switch (of->m_type)
	{
	case MSG_CHAR_PC:
		opc = TO_PC(of);
		break;

	case MSG_CHAR_NPC:
		onpc = TO_NPC(of);
		break;

	case MSG_CHAR_PET:
		opet = TO_PET(of);
		opc = opet->GetOwner();
		if (opc == NULL)
			return false;
		break;

	case MSG_CHAR_ELEMENTAL:
		oelemental = TO_ELEMENTAL(of);
		opc = oelemental->GetOwner();
		if (opc == NULL)
			return false;
		break;
#ifdef ATTACK_PET
	case MSG_CHAR_APET:
		oapet	= TO_APET(of);
		opc		= oapet->GetOwner();
		if( opc == NULL )
			return false;
		break;
#endif // ATTACK_PET

	default:
		return false;
	}

	switch (df->m_type)
	{
	case MSG_CHAR_PC:
		dpc = TO_PC(df);
		break;

	case MSG_CHAR_NPC:
		dnpc = TO_NPC(df);
		break;

	case MSG_CHAR_PET:
		dpet = TO_PET(df);
		dpc = dpet->GetOwner();
		if (dpc == NULL)
			return false;
		break;

	case MSG_CHAR_ELEMENTAL:
		delemental = TO_ELEMENTAL(df);
		dpc = delemental->GetOwner();
		if (dpc == NULL)
			return false;
		break;
#ifdef ATTACK_PET
	case MSG_CHAR_APET:
		dapet = TO_APET(df);
		dpc = dapet->GetOwner();
		if (dapet == NULL)
			return false;
		break;
#endif // ATTACK_PET

	default:
		return false;
	}

	// opc의 immortal 검사
	if (opc && opc->m_bImmortal)
	{
		DelAttackList(of);
		return false;
	}

	// dpc의 immortal 검사
	if (dpc && dpc->m_bImmortal)
	{
		DelAttackList(df);
		return false;
	}

	// PC의 disable 검사
	if ((opc && opc->IsDisable()) || (dpc && dpc->IsDisable()))
		return false;

#ifdef ADULT_SERVER_NEWITEM
	if(opc && opc->m_assist.FindByType(MT_OTHER, MST_OTHER_PEACE) && dpc)
		return false;
#endif // ADULT_SERVER_NEWITEM

	// 방어자 타입에 따른 처리
	switch (df->m_type)
	{

	case MSG_CHAR_NPC:

		// 공격 안당하는 NPC 들 검사
		if (!(IS_NPC(of) && of->m_pZone->IsPersonalDungeon()) && dnpc->m_proto->CheckFlag(NPC_SHOPPER | NPC_ZONEMOVER | NPC_REFINER | NPC_QUEST | NPC_PEACEFUL | NPC_MINERAL | NPC_CROPS | NPC_ENERGY | NPC_REMISSION))
		{
#if defined (DRATAN_CASTLE)
			if (of->m_pZone->m_index == ZONE_DRATAN)
			{
				CDratanCastle * pCastle = CDratanCastle::CreateInstance();
				if( pCastle != NULL && (pCastle->GetState() == WCSF_NORMAL) )
				{
					return false;
				}
			}
			if( dnpc->m_proto->CheckFlag(NPC_ZONEMOVER) && 
				!( opc && opc->GetJoinFlag(opc->m_pZone->m_index) & (WCJF_ATTACK_GUILD | WCJF_ATTACK_CHAR) ) )
			{
				return false;
			}
#else
			return false;
#endif // DRATAN_CASTLE
		}

#ifdef EVENT_HALLOWEEN_2007
		if( IS_PC(of) && dnpc->m_proto->m_index == 454  ) // 호박머리(2470~2475)를 착용하지 않았으면
		{
			CItem * pHelmet = TO_PC(of)->m_wearing[WEARING_HELMET] ;
			if( pHelmet == NULL ) 
				return false;

			if( pHelmet->m_itemProto->m_index < 2470 || pHelmet->m_itemProto->m_index > 2475 )
				return false;
		}
#endif // EVENT_HALLOWEEN_2007

#ifdef ENABLE_WAR_CASTLE

		// 권좌는 공격당하지 않음
		if (dnpc->m_proto->CheckFlag(NPC_LORD_SYMBOL))
			return false;

		// 수호병은 공성측만 공성지역에서 공격 가능
		if (dnpc->m_proto->CheckFlag(NPC_CASTLE_GUARD | NPC_CASTLE_TOWER))
		{
			// 공성 지역 검사
			if (of->GetMapAttr() != MAPATT_WARZONE)
				return false;
			
			// 공성중인지 검사
			
			if(of->m_pZone->m_index == ZONE_MERAC)
			{
				CWarCastle * pCastle = CWarCastle::GetCastleObject(ZONE_MERAC);
				if (pCastle == NULL)
				{
					return false;
				}

				if ( pCastle->GetState() == WCSF_NORMAL)
				{
					return false;
				}
			}
#ifdef DRATAN_CASTLE
			else if (of->m_pZone->m_index == ZONE_DRATAN)
			{
				CDratanCastle * pCastle = CDratanCastle::CreateInstance();
				if( pCastle == NULL)
				{
					return false;
				}

				if (pCastle->GetState() == WCSF_NORMAL)
				{
					return false;
				}
			}
#endif // DRATAN_CASTLE

			// 공성 참여 정보 검사
			if (dpc)
			{
				switch (dpc->GetJoinFlag(df->m_pZone->m_index))
				{
				case WCJF_NONE:
				case WCJF_OWNER:
				case WCJF_DEFENSE_GUILD:
				case WCJF_DEFENSE_CHAR:
					return false;
				default:
					break;
				}
			}
		}

#endif // #ifdef ENABLE_WAR_CASTLE

		
#ifdef EXTREME_CUBE
		// 보라색 수정
		if(dnpc->m_proto->m_index == 529)
		{
			if(opc)
			{
				if(gserver.m_extremeCube.IsGuildCubeTime())		// 길드 큐브타임일 때 
				{
					if(opc->m_guildInfo 
						&& opc->m_guildInfo->guild()
						&& opc->m_guildInfo->guild()->m_cubeUniqueIdx != -1)
					{
						int cubeidx;
						CBattleSpace* battlespace;
						CCubeMemList* memlist = gserver.m_extremeCube.FindMemList(opc->m_guildInfo->guild());
						if(!memlist)
							return false;
						
						memlist->GetCubeNum(cubeidx);
						CCubeSpace* space = gserver.m_extremeCube.GetExtremeCube(cubeidx);
						if(!space)
						{
							return false;
						}
						
						if(!space->IsBattleCubeSpace())
						{
							return false;
						}
						
						battlespace = (CBattleSpace*)space;
						
						if(!battlespace->WinMemList()
							|| battlespace->WinMemList() != memlist
							|| battlespace->m_crystal !=  dnpc)
							return false;
					}
				}
				else
				{
					if(opc->m_party && opc->m_party->m_cubeUniqueIdx != -1)
					{
						int cubeidx;
						CBattleSpace* battlespace;
						CCubeMemList* memlist = gserver.m_extremeCube.FindMemList(opc->m_party);
						if(!memlist)
							return false;
						
						memlist->GetCubeNum(cubeidx);
						CCubeSpace* space = gserver.m_extremeCube.GetExtremeCube(cubeidx);
						if(!space)
						{
							return false;
						}
						
						if(!space->IsBattleCubeSpace())
						{
							return false;
						}
						
						battlespace = (CBattleSpace*)space;
						
						if(!battlespace->WinMemList()
							|| battlespace->WinMemList() != memlist
							|| battlespace->m_crystal != dnpc)
							return false;
					}
				}
				
				if(gserver.m_extremeCube.GetAvailableBossCube() == -1)
					return false;
			}
		}
#endif // EXTREME_CUBE
		
		break;	// case MSG_CHAR_NPC:

	case MSG_CHAR_PC:
	case MSG_CHAR_PET:
	case MSG_CHAR_ELEMENTAL:
	case MSG_CHAR_APET:

		// 공격자에 따른 처리
		switch (of->m_type)
		{
		case MSG_CHAR_PC:
		case MSG_CHAR_ELEMENTAL:		// kj : 소환수가 공격시에도 PVP 가능 검사
#ifdef ATTACK_PET
		case MSG_CHAR_APET:
#endif //ATTACK_PET
			// PvP 가능 검사
			if (!opc->CanPvP(dpc, false))
				return false;

			break;
			
		default:
			break;
		} // switch (of->m_type)

		break;	// case MSG_CHAR_PC:

	default:
		return false;

	} // switch (df->m_type)

	return true;
}
