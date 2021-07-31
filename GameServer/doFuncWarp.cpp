#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"
#include "DratanCastle.h"

void do_WarpCancel(CPC* ch, CNetMsg& msg);
void do_WarpTeleport(CPC* ch, CNetMsg& msg);
void do_WarpPrompt(CPC* ch, CNetMsg& msg);

#ifdef FEEITEM
void do_WarpToReq(CPC* ch, CNetMsg& msg);
void do_WarpTo(CPC* ch, CNetMsg& msg);
void do_WarpTakeReq(CPC* ch, CNetMsg& msg);
void do_WarpTake(CPC* ch, CNetMsg& msg);
bool do_WarpFromTo(CPC* fromChar, CPC* toChar, bool recvMsg = false);
void do_TelePortCancel(CPC* ch, CNetMsg& msg);
#endif

void do_Warp(CPC* ch, CNetMsg& msg)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(ch);
	}
#endif // DRATAN_CASTLE

	unsigned char subtype = (unsigned char)-1;
	msg.MoveFirst();
	msg >> subtype;

	switch (subtype)
	{
	case MSG_WARP_CANCEL:
		do_WarpCancel(ch, msg);
		break;
	case MSG_WARP_TELEPORT:
		do_WarpTeleport(ch, msg);
		break;
	case MSG_WARP_PROMPT:
		do_WarpPrompt(ch, msg);
		break;
#ifdef FEEITEM
	case MSG_WARP_TO_REQ:
		do_WarpToReq(ch, msg);
		break;
	case MSG_WARP_TO:
		do_WarpTo(ch, msg);
		break;
	case MSG_WARP_TAKE_REQ:
		do_WarpTakeReq(ch, msg);
		break;
	case MSG_WARP_TAKE:
		do_WarpTake(ch, msg);
		break;
	case MSG_WARP_TO_REQING:
		do_TelePortCancel(ch, msg);
		break;
	case MSG_WARP_TAKE_REQING:
		do_TelePortCancel(ch, msg);
		break;
#endif
	default:
		break;
	}
}

#ifdef FEEITEM
void do_TelePortCancel(CPC* ch, CNetMsg& msg)
{
	CLCString tcharname(MAX_CHAR_NAME_LENGTH + 1);

	msg >> tcharname;

	CPC* tpc = gserver.m_playerList.Find(tcharname, true );

	if( !tpc )
		return;

	CNetMsg rmsg;
	WarpErrorMsg(rmsg, MSG_WARP_ERROR_CANCEL, ch->GetName() );

	SEND_Q(rmsg, tpc->m_desc);
}

bool do_WarpFromTo(CPC* fromChar, CPC* toChar, bool recvMsg)
{
	if( toChar->m_pZone->IsPersonalDungeon() )
	{
		CNetMsg rmsg;
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTALLOWZONE, toChar->GetName());
		if( !recvMsg )
		{
			SEND_Q(rmsg, fromChar->m_desc);
		}
		else
		{
			SEND_Q(rmsg, toChar->m_desc);
		}
		return false;
	}
#ifdef MONSTER_COMBO
	if( toChar->m_pZone->IsComboZone() )
	{
		CNetMsg rmsg;
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTALLOWZONE, toChar->GetName());
		if( !recvMsg )
		{
			SEND_Q(rmsg, fromChar->m_desc);
		}
		else
		{
			SEND_Q(rmsg, toChar->m_desc);
		}
		return false;
	}
#endif // MONSTER_COMBO

	int canWarp = fromChar->CanWarp();
	if (canWarp != 0)
	{
		CNetMsg rmsg;
		SysMsg(rmsg, (MSG_SYS_TYPE)canWarp);
		if( !recvMsg )
		{
			SEND_Q(rmsg, fromChar->m_desc);
		}
		else
		{
			SEND_Q(rmsg, toChar->m_desc);
		}
		return false;
	}

#ifdef ENABLE_WAR_CASTLE
	// 공성중에 공성 지역(MAPATT_WARZONE) 으로 이동 불가
	CWarCastle* castle = CWarCastle::GetCastleObject(toChar->m_pZone->m_index);
	if (castle)
	{
		if (castle->GetState() != WCSF_NORMAL)
		{
			if ( toChar->GetMapAttr() == MAPATT_WARZONE)
			{
				CNetMsg rmsg;
				SysMsg(rmsg, MSG_SYS_MEMPOS_CASTLE);
				if( !recvMsg )
				{
					SEND_Q(rmsg, fromChar->m_desc);
				}
				else
				{
					SEND_Q(rmsg, toChar->m_desc);
				}
				return false;
			}
		}
	}
#endif
	
	
//	if(!recvMsg )
//	{
//		CNetMsg rmsg;
//		WarpTeleportMsg(rmsg, toChar->m_pZone->m_index, fromChar);
//		SEND_Q(rmsg, fromChar->m_desc);
//	}
//	else
//	{
//		CNetMsg rmsg;
//		WarpTeleportMsg(rmsg, fromChar->m_pZone->m_index, toChar);
//		SEND_Q(rmsg, toChar->m_desc);
//	}
	
	GoZone(fromChar, toChar->m_pZone->m_index, toChar->m_pos.m_yLayer, toChar->m_pos.m_x, toChar->m_pos.m_z);

	if(!recvMsg )
	{
		CNetMsg wmsg;
		WarpEndMsg(wmsg, fromChar);
		fromChar->m_pArea->SendToCell(wmsg, fromChar, true);
	}
	else
	{
		CNetMsg wmsg;
		WarpEndMsg(wmsg, toChar);
		toChar->m_pArea->SendToCell(wmsg, toChar, true);
	}

	return true;
}

void do_WarpTake(CPC* ch, CNetMsg& msg)
{
	// ch가 소환될 케릭
	CLCString scharName(MAX_CHAR_NAME_LENGTH + 1);
	char ballow = false;

	msg >> scharName
		>> ballow;

	CPC* sPC = gserver.m_playerList.Find(scharName, true );

	if( !sPC )
	{
	//	CNetMsg rmsg;
	//	WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTCHAR, scharName);
	//	SEND_Q( rmsg, sPC->m_desc);
		return;
	}

	if( !ballow )
	{
		CNetMsg rmsg;
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTALLOW, ch->GetName() );
		SEND_Q(rmsg, sPC->m_desc);
		return;
	}

	if( !do_WarpFromTo(ch, sPC, true) )
	{
		return;
	}

	// 아이템 검사
	int r, c;
	if (!sPC->m_invenNormal.FindItem(&r, &c, 858, 0, 0))
		return ;

	// 카운트 줄이기
	CItem* item = sPC->m_invenNormal.GetItem(r, c);
	CNetMsg rmsg;

	ItemUseMsg(rmsg, sPC, item->tab(), item->row(), item->col(), item->m_index, 0);
	SEND_Q(rmsg, sPC->m_desc);

	DecreaseFromInventory(sPC, item, 1);

	if( (item->m_itemProto->m_flag & ITEM_FLAG_CASH) )
	{
		GAMELOG << init("CASH_ITEM_USE", sPC)
				<< itemlog(item)
				<< end;
	}

	if (item->Count() == 0)
	{
		ItemDeleteMsg(rmsg, item);
		SEND_Q(rmsg, sPC->m_desc);
		RemoveFromInventory(sPC, item, true, true);
	}
	else
	{
		ItemUpdateMsg(rmsg, item, -1);
		SEND_Q(rmsg, sPC->m_desc);
	}

}
void do_WarpTakeReq(CPC* ch, CNetMsg& msg)
{
	// ch가 사용한 케릭
	CLCString tcharName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> tcharName;

	CPC * tPC = gserver.m_playerList.Find(tcharName, true );

	if( !tPC )
	{
		CNetMsg rmsg;
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTCHAR, tcharName);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( tPC->m_plusEffect & CLIENT_OPTION_WARP )
	{
		CNetMsg rmsg;
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTALLOW, tcharName);
		SEND_Q( rmsg, ch->m_desc);
		return;
	}

	if( ch->m_pZone->IsPersonalDungeon() )
	{
		CNetMsg rmsg;
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTALLOWZONE, ch->GetName());

		SEND_Q( rmsg, ch->m_desc);
		return;
	}
	if( ch->m_pZone->IsSummonZone() )
	{
		CNetMsg rmsg;
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTALLOWZONE, ch->GetName() );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	// 각 메세지 보냄
	CNetMsg smsg;
	CNetMsg dmsg;

	WarpReqIngMsg(smsg, MSG_WARP_TAKE_REQING);
	WarpReqMsg(dmsg, ch, MSG_WARP_TAKE_REQ);

	SEND_Q(smsg, ch->m_desc);
	SEND_Q(dmsg, tPC->m_desc);
}

void do_WarpTo(CPC* ch, CNetMsg& msg)
{
	// ch가 가만히 있는 케릭
	CLCString scharName(MAX_CHAR_NAME_LENGTH + 1);
	char ballow = false;

	msg >> scharName
		>> ballow;

	CPC* sPC = gserver.m_playerList.Find(scharName, true );

	if( !sPC )
	{
		return;
	}

	if( !ballow )
	{
		CNetMsg rmsg;
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTALLOW, scharName);
		SEND_Q(rmsg, sPC->m_desc);
		return;
	}

	if( !do_WarpFromTo(sPC, ch, false) )
	{
		return;
	}
	
	// 아이템 검사
	int r, c;
	if (!sPC->m_invenNormal.FindItem(&r, &c, 857, 0, 0))
		return ;

	// 카운트 줄이기
	CItem* item = sPC->m_invenNormal.GetItem(r, c);
	CNetMsg rmsg;

	ItemUseMsg(rmsg, sPC, item->tab(), item->row(), item->col(), item->m_index, 0);
	SEND_Q(rmsg, sPC->m_desc);

	DecreaseFromInventory(sPC, item, 1);

	if( (item->m_itemProto->m_flag & ITEM_FLAG_CASH) )
	{
		GAMELOG << init("CASH_ITEM_USE", sPC)
				<< itemlog(item)
				<< end;
	}

	if (item->Count() == 0)
	{
		ItemDeleteMsg(rmsg, item);
		SEND_Q(rmsg, sPC->m_desc);
		RemoveFromInventory(sPC, item, true, true);
	}
	else
	{
		ItemUpdateMsg(rmsg, item, -1);
		SEND_Q(rmsg, sPC->m_desc);
	}

}

void do_WarpToReq(CPC* ch, CNetMsg& msg)
{
	// ch가 사용한 케릭
	CLCString tcharName(MAX_CHAR_NAME_LENGTH + 1);

	msg >> tcharName;

	CPC * tPC = gserver.m_playerList.Find(tcharName, true );

	if( !tPC )
	{
		CNetMsg rmsg;
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTCHAR, tcharName);
		SEND_Q( rmsg, ch->m_desc);
		return;
	}

	if( tPC->m_plusEffect & CLIENT_OPTION_WARP )
	{
		CNetMsg rmsg;
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTALLOW, tcharName);
		SEND_Q( rmsg, ch->m_desc);
		return;
	}

	if( tPC->m_pZone->IsPersonalDungeon() )
	{
		CNetMsg rmsg;
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTALLOWZONE, tPC->GetName());

		SEND_Q( rmsg, ch->m_desc);
		return;
	}
#ifdef EXTREME_CUBE
	if(tPC->m_pZone != NULL && tPC->m_pZone->IsExtremeCube() )
	{
		CNetMsg rmsg;
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTALLOWZONE, tPC->GetName());

		SEND_Q( rmsg, ch->m_desc);
		return;
	}

	if(ch->m_pZone !=NULL && ch->m_pZone->IsExtremeCube() )
		return ;

#endif // EXTREME_CUBE

	// 각 메세지 보냄
	CNetMsg smsg;
	CNetMsg dmsg;

	WarpReqIngMsg(smsg, MSG_WARP_TO_REQING);
	WarpReqMsg(dmsg, ch, MSG_WARP_TO_REQ);

	SEND_Q(smsg, ch->m_desc);
	SEND_Q(dmsg, tPC->m_desc);
}
#endif

void do_WarpCancel(CPC* ch, CNetMsg& msg)
{
	ch->m_reqWarpTime = 0;
	ch->m_reqWarpType = -1;
	ch->m_reqWarpData = -1;
	ch->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING | PLAYER_STATE_WARP);

	CNetMsg wmsg;
	WarpCancelMsg(wmsg, ch);
	ch->m_pArea->SendToCell(wmsg, ch, true);
}

void do_WarpTeleport(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int pos = -1;
	msg >> pos;

	// 싱글던전 터치패드 인덱스(999) : 발록 bw
	if ( (pos != 999 && (pos < 0 || pos >= ch->m_pZone->m_countZonePos)) 
		&& (pos != 998 && (pos < 0 || pos >= ch->m_pZone->m_countZonePos)) )
		return ;

#ifdef ENABLE_WAR_CASTLE
	bool bSendGoZoneEffect = false;
	// 메라크 4, 5, 6, 7은 공성시 수성측만 가능
	if (ch->m_admin < 2 && ch->m_pZone->m_index == CWarCastle::GetCurSubServerCastleZoneIndex())
	{
		CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
		if (castle && castle->IsDefenseWarpPoint(pos))
		{
			if (castle->GetState() == WCSF_NORMAL)
				return ;
			if (!IS_DEFENSE_TEAM(ch->GetJoinFlag(ch->m_pZone->m_index)))
				return ;

			bSendGoZoneEffect = true;
		}
	}
#endif
	
#ifdef DRATAN_CASTLE
	// 드라탄 워프 타워 13 - 17 은 공성시 성주 길드만 가능
	if (ch->m_admin < 2 && ch->m_pZone->m_index == ZONE_DRATAN)
	{
		CDratanCastle * pCastle = CDratanCastle::CreateInstance();
		if (pCastle && pCastle->IsDefenseWarpPoint(pos))
		{
			if (pCastle->GetState() == WCSF_NORMAL)
			{
				return;
			}

			if (!IS_DEFENSE_TEAM(ch->GetJoinFlag(ch->m_pZone->m_index)))
			{
				return;
			}

			bSendGoZoneEffect = true;
		}
	}
#endif // DRATAN_CASTLE

	// 셀 안 사람에게 나 사라지고, 다른 사람들 사라짐 알리고
	ch->SendDisappearAllInCell(true);

	// 셀에서 제거
	ch->m_pArea->CharFromCell(ch, true);

	if( ch->m_pZone->m_index == ZONE_SINGLE_DUNGEON1)
	{
		// 발록 방으로 강제 이동
		if( pos == 999 )
		{
			GET_YLAYER(ch)	= 0;
			GET_X(ch)		= 80;
			GET_Z(ch)		= 240;
		}
		else
		{
			GET_YLAYER(ch)	= 0;
			GET_X(ch)		= 122;
			GET_Z(ch)		= 100;
			ch->m_pArea->m_bBoss = true;
		}
	}
	else
	{
	// 캐릭터의 위치 설정
		GET_YLAYER(ch)	= ch->m_pZone->m_zonePos[pos][0];
		GET_X(ch)		= GetRandom(ch->m_pZone->m_zonePos[pos][1], ch->m_pZone->m_zonePos[pos][3]) / 2.0f;
		GET_Z(ch)		= GetRandom(ch->m_pZone->m_zonePos[pos][2], ch->m_pZone->m_zonePos[pos][4]) / 2.0f;
	}

	ch->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING);

	WarpTeleportMsg(rmsg, pos, ch);
	SEND_Q(rmsg, ch->m_desc);

	int cx, cz;
	ch->m_pArea->PointToCellNum(GET_X(ch), GET_Z(ch), &cx, &cz);
	ch->m_pArea->CharToCell(ch, GET_YLAYER(ch), cx, cz);

	AppearMsg(rmsg, ch, true);

	ch->m_pArea->SendToCell(rmsg, ch);
	ch->m_pArea->SendCellInfo(ch, true);
#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
		if( ch->m_guildInfo )
		{		
			GuildPointRankingMsg( rmsg, ch->m_index, ch->m_guildInfo->guild()->index(), ch->m_guildInfo->guild()->GetGuildPointRanking() );
			ch->m_pArea->SendToCell( rmsg, ch );
		}
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILDif( m_guildInfo && m_guildInfo->guild() ) 

#ifdef DYNAMIC_DUNGEON
	GuildNameColorStateMsg(rmsg, ch );
	ch->m_pArea->SendToCell( rmsg, ch );
#endif //DYNAMIC_DUNGEON

#ifdef ENABLE_PET
	if (ch->GetPet())
	{
		CPet* pet = ch->GetPet();
		if (pet->IsSummon())
			GoTo(pet, GET_YLAYER(ch), GET_X(ch), GET_Z(ch), GET_H(ch), GET_R(pet));
	}
#endif // #ifdef ENABLE_PET

	if (ch->m_elementalList)
	{
		CElemental* pElemental = ch->m_elementalList;
		while (pElemental)
		{
			GoTo(pElemental, GET_YLAYER(ch), GET_X(ch), GET_Z(ch), GET_H(ch), GET_R(pElemental));
			pElemental = pElemental->m_nextElemental;
		}
	}
	
#ifdef ENABLE_WAR_CASTLE
	if (bSendGoZoneEffect)
	{
		// GO_ZONE 효과 메시지 전송
		EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_GOZONE);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}
#endif
}

void do_WarpPrompt(CPC* ch, CNetMsg& msg)
{
	int zoneindex;
	int pos;

	msg >> zoneindex
		>> pos;

	zoneindex = gserver.FindZone(zoneindex);
	if (zoneindex == -1)
		return ;

	CZone* zone = gserver.m_zones + zoneindex;

	if (pos < 0 || pos >= zone->m_countZonePos)
		return ;

	if( zone->m_index == ZONE_SINGLE_DUNGEON1)
	{
		// 발록 방으로 강제 이동
		GoZone(ch, zone->m_index,
				GET_YLAYER(ch),
				80,
				240);
	}
	else
	{
		// 공성중 성주 수성 길드가 아니 캐릭터는 성안 이동 금지
#ifdef DRATAN_CASTLE
		CDratanCastle * pCastle = CDratanCastle::CreateInstance();
		if (pCastle != NULL)
		{
			if(zone->m_index == ZONE_DRATAN 
				&& pCastle->GetState() != WCSF_NORMAL 
				&& ch->GetJoinFlag(zone->m_index) == WCJF_ATTACK_GUILD
				&& pos == 25) // 수성시작 위치 25
//			&& ch->m_admin < 2)
			{
				return;		
			}
		}		
#endif // DRATAN_CASTLE

		GoZone(ch, zone->m_index,
				zone->m_zonePos[pos][0],
				GetRandom(zone->m_zonePos[pos][1], zone->m_zonePos[pos][3]) / 2.0f,
				GetRandom(zone->m_zonePos[pos][2], zone->m_zonePos[pos][4]) / 2.0f);
	}

	// 이동후 3초간 무적
	ch->SetDisableTime(3);
}
