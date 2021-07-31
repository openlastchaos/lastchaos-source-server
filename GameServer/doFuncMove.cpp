#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"
#include "DratanCastle.h"

//////////////////
// 이동관련 함수
//N1InPersonalDungeon : BW

void do_pd_Move(CPC* ch, CNetMsg& msg)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(ch);
	}
#endif // DRATAN_CASTLE

	//MSG_PD_MOVE: movetype(uc) speed(f) multicount(c) multiindex(n:multicount) x(f) z(f) h(f) r(f) y(n)
	unsigned char movetype;
	float speed;
	float x;
	float z;
	float h;
	float r;
	char ylayer;
	char multicount;
	int multiIndex;

//	msg.MoveFirst();

	msg >> movetype
		>> speed
		>> multicount;

	if(multicount > 20) multicount = 20;

	if (ch->m_pZone == NULL || ch->m_pArea == NULL)
		return ;

	switch (movetype)
	{
	case MSG_MOVE_WALK:
	case MSG_MOVE_RUN:
#ifdef NIGHT_SHADOW
	case MSG_MOVE_FLY:
#endif //NIGHT_SHADOW
		
		ch->SetPlayerState(PLAYER_STATE_MOVING);
		break;
	case MSG_MOVE_STOP:
		ch->ResetPlayerState(PLAYER_STATE_MOVING);
		break;

	case MSG_MOVE_PLACE:
		break;
	default:
		return ;
	}

	CCharacter* tch;
	do
	{
		if(multicount)
		{
			multicount--;
			if(multicount < 0)
			{
				return;
			}
			msg >> multiIndex
				>> x
				>> z
				>> h
				>> r
				>> ylayer;

			
			if (x < 0 || z < 0 || x >= ch->m_pZone->m_attrMap[0].m_size[0] || z >= ch->m_pZone->m_attrMap[0].m_size[1] || ylayer < 0 || ylayer >= ch->m_pZone->m_countY * 2)
			{
#ifdef _DEBUG
#ifdef _CONSOLE
				sprintf(g_buf2, "x: %.2f, z: %.2f, h: %.2f, y: %d -> x: %.2f, z: %.2f, h: %.2f, y: %d",
						GET_X(ch), GET_Z(ch), GET_H(ch), GET_YLAYER(ch),
						x, z, h, ylayer);
				SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", g_buf2);
				SEND_Q(msg, ch->m_desc);
#endif
#endif
				continue ;
			}

			CArea* m_area = ch->m_pArea;
			if (!m_area) return ;
			if(!m_area->m_zone->IsPersonalDungeon()) return ;

			tch = m_area->FindCharInCell(ch, multiIndex, (MSG_CHAR_TYPE) MSG_CHAR_NPC);
			if (!tch) return ;

			CNPC* npc = IS_NPC(tch) ? TO_NPC(tch) : NULL;
			if (!npc) continue;

			GET_X(npc) = x;
			GET_Z(npc) = z;
			GET_R(npc) = r;
			npc->m_pos.m_h = h;
			GET_YLAYER(npc) = ylayer;
		} 
		else tch = NULL;
	}while(tch);
}


void do_Move(CPC* ch, CNetMsg& msg)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(ch);
	}
#endif // DRATAN_CASTLE


	unsigned char chartype;
	unsigned char movetype;
	int index;
	float speed;
	float x;
	float z;
	float h;
	float r;
	char ylayer;

	msg.MoveFirst();

	msg >> chartype
		>> movetype
		>> index
		>> speed
		>> x
		>> z
		>> h
		>> r
		>> ylayer;

	if (ch->m_pZone == NULL || ch->m_pArea == NULL)
		return ;

#if defined(KYD_TEST) && defined ( ENABLE_BACKWING )
	if( movetype == MSG_MOVE_RUN )
	{
		movetype = MSG_MOVE_FLY;
		ch->m_runSpeed = ch->m_flySpeed;
	}
#endif //KYD_TEST

	if (x < 0 || z < 0 || x >= ch->m_pZone->m_attrMap[0].m_size[0] || z >= ch->m_pZone->m_attrMap[0].m_size[1] || ylayer < 0 || ylayer >= ch->m_pZone->m_countY * 2)
	{
#ifdef _DEBUG
#ifdef _CONSOLE
		sprintf(g_buf2, "x: %.2f, z: %.2f, h: %.2f, y: %d -> x: %.2f, z: %.2f, h: %.2f, y: %d",
				GET_X(ch), GET_Z(ch), GET_H(ch), GET_YLAYER(ch),
				x, z, h, ylayer);
		SayMsg(msg, MSG_CHAT_NOTICE, 0, "", "", g_buf2);
		SEND_Q(msg, ch->m_desc);
#endif
#endif
		return ;
	}

	switch (movetype)
	{
	case MSG_MOVE_WALK:
	case MSG_MOVE_RUN:
	case MSG_MOVE_PLACE:
	case MSG_MOVE_STOP:
#ifdef NIGHT_SHADOW
	case MSG_MOVE_FLY:
#endif //NIGHT_SHADOW
		break;

	default:
		return ;
	}

	CCharacter* chMove = NULL;

	switch (chartype)
	{
	case MSG_CHAR_PC:
#ifdef FORBUG
	GAMELOG << init("RECV : CharMove", ch)
			<< speed << delim
			<< x << delim
			<< z << delim
			<< h << delim
			<< r << delim
			<< ylayer
			<< end;
#endif //FORBUG

		if (ch->m_index != index)
			return;

		chMove = ch;


		// 이동속도 검사
		if (ch->m_admin < 1)
		{
			// 퍼스널 던전은 제외
			//if (!ch->m_pZone->IsPersonalDungeon() && !ch->m_pZone->IsGuildRoom()) : 왜 pd는 검사하지 않는가? X, Z의 거리만 검사하도록 수정 bw
			//{
				// 060109 : BS : BEGIN : 200미터 이상 이동 불가
				float walkSpeed = ch->m_walkSpeed;
				float runSpeed = ch->m_runSpeed;
#ifdef NIGHT_SHADOW
				float flySpeed = ch->m_flySpeed;
#endif //NIGHT_SHADOW

#ifdef DYNAMIC_DUNGEON
				if(ch->m_pZone->m_index == ZONE_DRATAN_CASTLE_DUNGEON && IS_PC(ch))
				{
					CDratanCastle * pCastle = CDratanCastle::CreateInstance();
					if(pCastle && pCastle->m_dvd.GetZone() && pCastle->m_dvd.GetEnvRate() >= 90)
					{
						walkSpeed = walkSpeed + walkSpeed*0.1;
						runSpeed = runSpeed + runSpeed*0.1;
#ifdef NIGHT_SHADOW
						flySpeed = flySpeed + flySpeed*0.1;
#endif //NIGHT_SHADOW
					}
				}
#endif // DYNAMIC_DUNGEON

#ifdef NIGHT_SHADOW
				float spd = 0.0f; 
				switch( movetype )
				{
				case MSG_MOVE_WALK :	spd = walkSpeed;	break;
				case MSG_MOVE_RUN :		spd = runSpeed;		break;
				case MSG_MOVE_FLY :		spd = flySpeed;		break;
				default :				spd = walkSpeed;	break;
				}
#else
				float spd = (movetype == MSG_MOVE_RUN) ? runSpeed : walkSpeed;
#endif 				
				float dist = GetDistance(x, z, ch->m_pArea->GetHeight(GET_YLAYER(ch), x, z), ch);
				bool bCurrectX = false;
				bool bCurrectZ = false;
				//float chx = GET_X(ch);
				//float chz = GET_Z(ch);
				float errorFloat =0.0f;
				float tempFloat = ch->m_pArea->GetHeight(GET_YLAYER(ch), GET_X(ch), GET_Z(ch));
				float checkSpeed = 0.0f;

				//if( ABS(tempFloat-h) > 1 )
				{
					errorFloat = ABS(tempFloat-h);
				}

#ifdef NIGHT_SHADOW
				checkSpeed = spd;
#else
				checkSpeed = ch->m_runSpeed;
#endif // NIGHT_SHADOW

				CPet* pet = ch->GetPet();
				if( pet )
				{
					if( pet->IsMount() )
						checkSpeed = pet->m_runSpeed;
				}

				checkSpeed = checkSpeed * 3;

				if( GET_X(ch) > x-checkSpeed - errorFloat && GET_X(ch) < x+checkSpeed + errorFloat)
					bCurrectX = true;
				if( GET_Z(ch) > z-checkSpeed - errorFloat && GET_Z(ch) < z+checkSpeed + errorFloat)
					bCurrectZ = true;

				if( !bCurrectX || !bCurrectZ )
				//if (dist > 200.0f)
				{
					GAMELOG << init("HACK MOVE", ch)
							<< ch->m_pArea->m_zone->m_index << delim
							<< GET_X(ch) << delim
							<< GET_Z(ch) << delim
							<< GET_H(ch) << delim
							<< GET_YLAYER(ch) << delim
							<< x << delim
							<< z << delim
							<< ch->m_pArea->GetHeight(GET_YLAYER(ch), x, z) << delim
							<< ylayer << delim
							<< dist << delim
							<< spd << delim
							<< bCurrectX << delim
							<< bCurrectZ << end;

					ch->SetDisableTime(3);

					GoTo(ch, GET_YLAYER(ch), GET_X(ch), GET_Z(ch), GET_H(ch), GET_R(ch), 1);

					ch->m_desc->IncreaseHackCount(1);
					return ;
				}

		}

		// 상태 설정
		switch (movetype)
		{
		case MSG_MOVE_WALK:
		case MSG_MOVE_RUN:
#ifdef NIGHT_SHADOW
		case MSG_MOVE_FLY:
#endif //NIGHT_SHADOW
			ch->SetPlayerState(PLAYER_STATE_MOVING);
			break;
		case MSG_MOVE_STOP:
			ch->ResetPlayerState(PLAYER_STATE_MOVING);
			break;

		case MSG_MOVE_PLACE:
			break;
		default:
			return ;
		}
		break;

	case MSG_CHAR_NPC:
		chMove = ch->m_pArea->FindCharInCell(ch, index, MSG_CHAR_NPC);
		break;

#ifdef ENABLE_PET
	case MSG_CHAR_PET:
		if (ch->GetPet())
		{
			CPet* pet = ch->GetPet();
			if (pet->m_index != index)
				return ;
			if (pet->m_pArea == NULL)
				return ;
			if (pet->m_pZone == NULL)
				return ;
			if (!pet->IsSummon())
				return ;
			if (!pet->IsActive())
				return ;
			if (pet->IsMountType())
				return ;
			chMove = pet;
		}
		else
			return ;
		break;
#endif // #ifdef ENABLE_PET
		
	case MSG_CHAR_ELEMENTAL:
		chMove = ch->m_pArea->FindCharInCell(ch, index, MSG_CHAR_ELEMENTAL);
		if (chMove)
		{
			CElemental* elemental = TO_ELEMENTAL(chMove);
			if (elemental)
			{
				// 주인이 명령하고
				if (elemental->GetOwner() != ch)
			return ;
			}
		}
		break;

#ifdef ATTACK_PET
	case MSG_CHAR_APET:
		{
			CAPet* apet = ch->GetAPet();
			if( apet )
			{
				if ( !apet->IsSummon() )
					return ;
				chMove = apet;
			}
		}
		break;
#endif // ATTACK_PET

	default:
			return ;
	}

	if (!chMove || DEAD(chMove) || !chMove->CanMove())
			return ;

	if (!IS_NPC(chMove))
		ch->m_pArea->MoveChar(chMove, ylayer, x, z, h, r, (MSG_MOVE_TYPE)movetype);
	else
	{
		GAMELOG << init("DEBUG NPC MOVE")
				<< "ZONE : " << ch->m_pZone->m_index << " : NPC : " << chMove->m_name << end;
	}
}

void do_GoZone(CPC* ch, CNetMsg& msg)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(ch);
	}
#endif // DRATAN_CASTLE

	CNetMsg rmsg;
	int zone, extra;
	int npcIdx;

	msg.MoveFirst();
	msg	>> zone
		>> extra;

	msg >> npcIdx;

	// npc를 찾는다.
	if( !ch->m_pZone->IsPersonalDungeon() )
	{
		// 존이동 유효성 검사
		bool bHackGoZone = false;
		bool bSkipFindNPC = false;

		// 던전의 워프 유효성 검사
		const int nSizeOfTable = 6;
		// Current Zone, Request Zone, Request Position, Valid Position, Valid Cell Size
		int nCheckValidWarp[nSizeOfTable][5] = {
			{ ZONE_EGEHA_DUNGEON_1, ZONE_EGEHA_DUNGEON_8, 0, 1, 2},
			{ ZONE_EGEHA_DUNGEON_8, ZONE_EGEHA_DUNGEON_1, 1, 0, 2},
			{ ZONE_EGEHA_DUNGEON_8, ZONE_EGEHA_DUNGEON_9, 0, 1, 2},
			{ ZONE_EGEHA_DUNGEON_9, ZONE_EGEHA_DUNGEON_8, 1, 0, 2},
			{ ZONE_EGEHA_DUNGEON_9, ZONE_EGEHA_DUNGEON_10, 0, 1, 2},
			{ ZONE_EGEHA_DUNGEON_10, ZONE_EGEHA_DUNGEON_9, 1, 0, 2},
		};
		int nCheck;
		for (nCheck = 0; nCheck < nSizeOfTable; nCheck++)
		{
			if (ch->m_pZone->m_index == nCheckValidWarp[nCheck][0])
			{
				if (zone == nCheckValidWarp[nCheck][1])
				{
					if (extra == nCheckValidWarp[nCheck][2])
					{
						int nTestCellX, nTestCellZ;
						ch->m_pArea->PointToCellNum(ch->m_pZone->m_zonePos[nCheckValidWarp[nCheck][3]][1] / 2.0f, ch->m_pZone->m_zonePos[nCheckValidWarp[nCheck][3]][2] / 2.0f, &nTestCellX, &nTestCellZ);
						if (ABS(ch->m_cellX - nTestCellX) > nCheckValidWarp[nCheck][4] || ABS(ch->m_cellZ - nTestCellZ) > nCheckValidWarp[nCheck][4])
							bHackGoZone = true;
					}
					else
					{
						bHackGoZone = true;
					}
					bSkipFindNPC = true;
					break;
				}
			}
		}

		if ( !bSkipFindNPC && !ch->m_pArea->FindProtoNPCInCell(ch, npcIdx, false, 2) )
			bHackGoZone = true;

		if (bHackGoZone)
		{
			GAMELOG << init("HACK_GO_ZONE", ch)
					<< end;
			return ;
		}
	}

	CZone* pZone;

#ifdef ENABLE_OXQUIZ
	if (zone == ZONE_OXQUIZROOM)
	{
		time_t timeCurrent;
		time(&timeCurrent);
		if ((!gserver.m_bEventOX || gserver.m_timeEventOXStart < timeCurrent) && ch->m_admin < 2)
		{
			EventOXQuizMsg(rmsg, MSG_EVENT_OXQUIZ_INVALID_TIME);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}
#endif // ENABLE_OXQUIZ

#ifdef MONSTER_COMBO
	// 동일 싱글 던전으로는 이동 불가
	if ((ch->m_pZone->IsPersonalDungeon() || ch->m_pZone->IsGuildRoom() || ch->m_pZone->IsComboZone()) && ch->m_pZone->m_index == zone)
		return ;
#else
	if ((ch->m_pZone->IsPersonalDungeon() || ch->m_pZone->IsGuildRoom()) && ch->m_pZone->m_index == zone)
		return ;
#endif 
	// 존을 찾아서
	int i = gserver.FindZone(zone);
	
	if (i == -1)
		return ;

#ifdef EGEHA_ZONE_PAYMENT
	if ( zone == ZONE_EGEHA )
	{
		if( ch->m_pZone->m_index != ZONE_EGEHA_DUNGEON_1 && ch->m_pZone->m_index != ZONE_EGEHA_DUNGEON_8 
			&& ch->m_pZone->m_index != ZONE_EGEHA_DUNGEON_9 && ch->m_pZone->m_index != ZONE_EGEHA_DUNGEON_10
			&& ch->m_pZone->m_index != ZONE_FLORAIM_CAVE)
		{
			if( ch->m_moneyItem)
			{
				if( ch->m_moneyItem->Count() < EGEHA_ZONE_PAYMENT )
				{
					// 현재 가지고 있는 나스 부족 에러메세지 전송
					SysMsg( rmsg, MSG_SYS_SHORT_MONEY);
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
				else if ( ch->m_moneyItem->Count() == EGEHA_ZONE_PAYMENT )
				{
					ItemDeleteMsg(rmsg, ch->m_moneyItem);
					ch->AddMoney( -EGEHA_ZONE_PAYMENT );
					SEND_Q(rmsg, ch->m_desc);
				}
				else
				{
					ch->AddMoney( -EGEHA_ZONE_PAYMENT );
					ItemUpdateMsg(rmsg, ch->m_moneyItem, -EGEHA_ZONE_PAYMENT);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
			else
			{
				SysMsg( rmsg, MSG_SYS_SHORT_MONEY);
				SEND_Q( rmsg, ch->m_desc );
				return;
			}
		}
	}
#endif // EGEHA_ZONE_PAYMENT

#ifdef EGEHA_ZONE_MINLEVEL
	if( zone == ZONE_EGEHA )
	{
		if( ch->m_level < EGEHA_ZONE_MINLEVEL )
		{
			 SysMsg( rmsg, MSG_SYS_EGEHA_MINLEVEL);
			 SEND_Q( rmsg, ch->m_desc );
			return ;
		}
	}
#endif // EGEHA_ZONE_MINLEVEL

	pZone = gserver.m_zones + i;

	if (extra < 0 || extra >= pZone->m_countZonePos)
		return ;

#ifdef LC_KOR
	if (ch->m_pZone->m_index == ZONE_SINGLE_DUNGEON_TUTORIAL && zone == ZONE_START)
		extra = 2;
#endif // LC_KOR

	GoZone(ch, zone,
			pZone->m_zonePos[extra][0],														// ylayer
			GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
			GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
}

void GoZone(CPC* ch, int zone, char ylayer, float x, float z)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(ch);
	}
#endif // DRATAN_CASTLE

	switch(zone)
	{
		case ZONE_SINGLE_DUNGEON3:
		case ZONE_SINGLE_DUNGEON4:
			if( ch->m_admin < 2 && !ch->CheckDungeonData(zone) )
			{
				GAMELOG << init("PD_BUG_GO_ZONE", ch)
						<< end;
				return;
			}
		default:
			break;
	}

	CNetMsg rmsg;
	bool bGoto = false;
	bool bValid = false;
	CZone* pZone = NULL;

#ifdef MONSTER_COMBO
	CParty* pParty = NULL;
#endif // MONSTER_COMBO

	int i = 0;
	if (ch->m_pZone->m_index == zone)
	{
		// 같은 존안에서의 이동
		bGoto = true;
		bValid = true;
		pZone = ch->m_pZone;
	}
	else
	{
		i = gserver.FindZone(zone);
		if (i != -1)
		{
			pZone = gserver.m_zones + i;
			if (pZone->m_bRemote)
			{
				// 외부 서버는 인스턴스 던전은 불가능
				if (!pZone->IsGuildRoom() && !pZone->IsPersonalDungeon())
				{
					bGoto = false;
					bValid = true;
				}
			}
			else
			{
				bGoto = true;
				bValid = true;
			}
		}
	}

	if (!bValid)
	{
		// 오류
		return ;
	}

	CZone* pCurZone = ch->m_pZone;
	CArea* pCurArea = ch->m_pArea;

	// 존이 바뀌면 
	if (pCurZone != pZone)
	{
		// 파티 처리
		if (ch->m_party != NULL)
		{
			// 초대하거나, 초대받은 사람일 경우 거절로 처리
			if (ch->m_party->GetRequestIndex() > 0 && (ch->m_party->GetBossIndex() == ch->m_index || ch->m_party->GetRequestIndex() == ch->m_index))
			{
				CNetMsg tmpmsg;
				PartyMsg(tmpmsg, MSG_PARTY_REJECT);
				do_Party(ch, tmpmsg);
			}
			// 갈곳이 퍼던이면 파티탈퇴
			else if (pZone->IsPersonalDungeon())
			{
				CNetMsg tmpmsg;
				PartyMsg(tmpmsg, MSG_PARTY_QUIT);
				do_Party(ch, tmpmsg);
			}
#if defined (MONSTER_COMBO) || defined (EXTREME_CUBE)
			if(ch->IsParty())
			{
				pParty = ch->m_party;
			}
#endif // MONSTER_COMBO

			ch->m_party->SetMemberPCPtr(ch->m_index, NULL);
			ch->m_party = NULL;
		}
	} // 존이 바뀌면

	bool bSetPrePlay = false;
	if (bGoto)
	{
		ch->ProcDisconnect(false, (pCurZone == pZone));

		// 서버내의 이동

		i = pCurArea - pCurZone->m_area;

		// 존이 바뀌면 
		if (pCurZone != pZone)
		{
			// 가야할 존이 레이어 존 이면
			if (pZone->m_countArea > 1)
			{
				if (pZone->IsGuildRoom())
				{
					if (!ch->m_guildInfo)
						return;

					int idx;
					for (idx=0; idx < pZone->m_countArea; idx++)
					{
						if (!pZone->m_area[idx].m_bEnable)
							continue;

						if (pZone->m_area[idx].m_guildIndex == ch->m_guildInfo->guild()->index())
						{
							i = idx;
							break;
						}
					}

					if (idx == pZone->m_countArea)
					{
						i = pZone->SetEnableArea();
						// 빈 영억 없음
						if (i == -1)
						{
							CNetMsg errMsg;
							SysMsg(errMsg, MSG_SYS_SINGLEDUNGEON_FULL);
							SEND_Q(errMsg, ch->m_desc);
							return;
						}
						pZone->m_area[i].m_guildIndex = ch->m_guildInfo->guild()->index();
					}
				}
#ifdef MONSTER_COMBO
				// 콤보던전
				else if(pZone->IsComboZone())
				{
					if(pParty)
					{
						int idx;
						for(idx = 0 ; idx < pZone->m_countArea; ++idx)
						{
							if(!pZone->m_area[idx].m_bEnable)
								continue;

							if((pZone->m_area[idx].m_monsterCombo->m_nIndex != -1) && 
							   (pZone->m_area[idx].m_monsterCombo->m_nIndex == pParty->m_comboAreaIndex))
							{
								i = idx;
								break;
							}
						}

						// area 없음, 새로 area 만들어서 입장
						if(idx == pZone->m_countArea)
						{
							// 파장만 area 생성할 수 있음
							if(pParty->GetBossIndex() != ch->m_index)
								return ;

							i = pZone->SetEnableArea();
							if(i == -1)
							{
								MonsterComboErrorMsg(rmsg, MSG_EX_MONSTERCOMBO_ERROR_FULL);
								SEND_Q(rmsg, ch->m_desc);
								return ;
							}

							// 콤보존 미션케이스 설정
							if(!pZone->m_area[i].m_monsterCombo->SetMonsterCombo(ch))
								return;

							// 파티 콤보인덱스 설정
							pParty->m_comboAreaIndex = pZone->m_area[i].m_monsterCombo->m_nIndex;

							// 편집데이터 삭제
							ch->m_listSaveComboData.RemoveAll();
							// 다른 파티원 소환
							if(IS_RUNNING_HELPER)
							{
								CNetMsg rmsg;
								HelperComboGotoWaitRoomPrompt(rmsg, ch->m_index, (const char*)ch->m_name);
								SEND_Q(rmsg, gserver.m_helper);
							}

							GAMELOG << init("CREATE COMBO AREA", ch)
									<< "AREA INDEX: " << i
									<< "COMBO INDEX: " << pZone->m_area[i].m_monsterCombo->m_nIndex
									<< end;

							if(IS_RUNNING_HELPER)
							{
								CNetMsg rmsg;
								HelperCreateComboMsg(rmsg, gserver.m_subno, ch->m_index, i, pZone->m_area[i].m_monsterCombo->m_nIndex);
								SEND_Q(rmsg, gserver.m_helper);
							}
						}
					}
					else
					{
						// 편집한 데이터가 있는지 확인
						if(ch->m_listSaveComboData.GetCount() < 1)
							return ;

						i = pZone->SetEnableArea();
						// 빈 영역 없음
						if(i == -1)
						{
							MonsterComboErrorMsg(rmsg, MSG_EX_MONSTERCOMBO_ERROR_FULL);
							SEND_Q(rmsg, ch->m_desc);
							return ;
						}

						// 미션케이스 세팅
						if(!pZone->m_area[i].m_monsterCombo->SetMonsterCombo(ch))
							return ;

						// 미션케이스 편집 정보 삭제
						ch->m_listSaveComboData.RemoveAll();

						GAMELOG << init("CREATE COMBO AREA", ch)
								<< "AREA INDEX: " << i
								<< "COMBO INDEX: " << pZone->m_area[i].m_monsterCombo->m_nIndex
								<< end;

						if(IS_RUNNING_HELPER)
						{
							CNetMsg rmsg;
							HelperCreateComboMsg(rmsg, gserver.m_subno, ch->m_index, i, pZone->m_area[i].m_monsterCombo->m_nIndex);
							SEND_Q(rmsg, gserver.m_helper);
						}
					}
				}
#endif // MONSTER_COMBO
#ifdef EXTREME_CUBE
				else if(pZone !=NULL && pZone->IsExtremeCube())
				{
					// 가야할 곳이 cube 이면
					// memlist 에 추가시킨다.
					// 1. memlist 찾기
					CCubeMemList* memlist = NULL;
					if(gserver.m_extremeCube.IsGuildCubeTime())
					{
						if(ch->m_guildInfo && ch->m_guildInfo->guild())
						{
							memlist = gserver.m_extremeCube.FindMemList(ch->m_guildInfo->guild());
						}
					}
					else
					{
						if(pParty)
						{
							memlist = gserver.m_extremeCube.FindMemList(pParty);
						}
					}

					if(!memlist)
					{
						GAMELOG << init("EXTREME CUBE MOVE ERROR 1", ch);
						return ;
					}
					if(!memlist->GetCubeNum(i))
					{
						// 에러
						GAMELOG << init("EXTREME CUBE MOVE ERROR 1", ch) << "CUBEIDX" << delim << memlist->GetIndex() << end;
						return ;
					}

					memlist->AddPC(ch);
					
				}
#endif // EXTREME_CUBE
				else
				{
					i = pZone->SetEnableArea();
					// 빈 영억 없음
					if (i == -1)
					{
						CNetMsg errMsg;
						SysMsg(errMsg, MSG_SYS_SINGLEDUNGEON_FULL);
						SEND_Q(errMsg, ch->m_desc);
						return;
					}
				}
			}
			else
				i= 0;

			// 다른 존으로 이동
			// 캐릭터를 프리플레이 상태로
			gserver.m_playerList.Remove(ch);
			ch->m_bPlaying = false;
			STATE(ch->m_desc) = CON_PREPARE_PLAY;
			bSetPrePlay = true;

#ifdef ENABLE_PET
			// 060310 : bs : 펫 착용해제 추가
//			CPet* pet = ch->GetPet();
//			if (pet)
//			{
//				// 착용중인 펫이 이동할 존에서 소환/마운트 불가능 할 경우 착용 해제
//				if ((!pZone->m_bCanSummonPet && !pet->IsMountType()) || (!pZone->m_bCanMountPet && pet->IsMountType()))
//				{
//					ItemWearMsg(rmsg, WEARING_PET, NULL, NULL);
//					do_ItemWear(ch, rmsg);
//					ExPetStatusMsg(rmsg, pet);
//					SEND_Q(rmsg, ch->m_desc);
//				}
//			}
			// 060310 : bs : 펫 착용해제 추가
#endif

			// DB에 저장
			ch->m_lastupdate = 0;
		} // 다른 존으로 이동
		else
		{
			
#ifdef EXTREME_CUBE
			// 같은 존으로 이동
			if(pCurZone !=NULL && pCurZone->IsExtremeCube())
			{
				// 이동만 한다.
				CCubeMemList* memlist = NULL;
				if(gserver.m_extremeCube.IsGuildCubeTime())
				{
					if(ch->m_guildInfo && ch->m_guildInfo->guild())
					{
						memlist = gserver.m_extremeCube.FindMemList(ch->m_guildInfo->guild());
					}
				}
				else
				{
					if(ch->m_party)
					{
						memlist = gserver.m_extremeCube.FindMemList(ch->m_party);
					}
				}
				
				if(!memlist)
				{
					GAMELOG << init("EXTREME CUBE MOVE ERROR 2", ch);
					return ;
				}
				if(!memlist->GetCubeNum(i))
				{
					// 에러
					GAMELOG << init("EXTREME CUBE MOVE ERROR 2", ch) << "CUBEIDX" << delim << memlist->GetIndex() << end;
					return ;
				}

				if( i == ch->m_pArea->m_index )
				{
					if( !memlist->FindPC(ch) )
						memlist->AddPC(ch);
				}
			}
#endif // EXTREME_CUBE
		}

		// 캐릭터의 위치 설정
		ch->m_pZone = pZone;
		ch->m_pArea = ch->m_pZone->m_area + i;

		GET_YLAYER(ch)	= ylayer;
		GET_R(ch)		= 0.0f;
		GET_X(ch)		= x;
		GET_Z(ch)		= z;

		
		GotoMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);

#ifdef NEW_GUILD
		if( ch->m_guildInfo && ch->m_guildInfo->guild() )
		{
			ch->m_guildInfo->zoneindex( pZone->m_index );
		}
#endif // NEW_GUILD

		if (!bSetPrePlay)
		{
			int cx, cz;

			while (ch->m_elementalList)
				ch->UnsummonElemental(ch->m_elementalList);

			ch->ResetPlayerState(PLAYER_STATE_SITDOWN);

			ch->m_pArea->PointToCellNum(GET_X(ch), GET_Z(ch), &cx, &cz);
			ch->m_pArea->CharToCell(ch, GET_YLAYER(ch), cx, cz);

			AppearMsg(rmsg, ch, true);
			ch->m_pArea->SendToCell(rmsg, ch);

			ch->m_pArea->SendCellInfo(ch, true);
#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
			if( ch->m_guildInfo && ch->m_guildInfo->guild() )
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
			if (IS_PC(ch))
			{
				CPC* pc = TO_PC(ch);
				CPet* pet = pc->GetPet();
				if (pet && pet->IsSummon())
				{
					GoTo(pet, GET_YLAYER(ch), x, z, ch->m_pArea->GetHeight(GET_YLAYER(ch), x, z), GET_R(pet));
				}
			}
#endif

			if (IS_PC(ch))
			{
				CPC* pc = TO_PC(ch);
				CElemental* elemental = pc->m_elementalList;
				while (elemental)
				{
					GoTo(elemental, GET_YLAYER(ch), x, z, ch->m_pArea->GetHeight(GET_YLAYER(ch), x, z), GET_R(elemental));
					elemental = elemental->m_nextElemental;
				}
			}
			
#ifdef ATTACK_PET
			if (IS_PC(ch))
			{
				CPC* pc = TO_PC(ch);
				CAPet* apet = pc->GetAPet();
				if (apet && apet->IsSummon())
				{
					GoTo(apet, GET_YLAYER(ch), x, z, ch->m_pArea->GetHeight(GET_YLAYER(ch), x, z), GET_R(apet));
				}
			}
#endif //ATTACK_PET
		}

		if (!DEAD(ch))
		{
			// GO_ZONE 효과 메시지 전송
			EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_GOZONE);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}

#ifdef ENABLE_SINGLEDUNGEON_DATA
		if (ch->m_admin < 2)
		{
			switch (zone)
			{
			case ZONE_SINGLE_DUNGEON3:
				ch->m_pd3Count++;

				if(ch->m_pd3Count == 1 || ch->m_pd3Count > 6)
				{
					time_t now;
					time(&now);
					ch->m_pd3Time = now;
				}
				break;
			case ZONE_SINGLE_DUNGEON4:
				ch->m_pd4Count++;
				if(ch->m_pd4Count == 1 || ch->m_pd4Count > 4)
				{
					time_t now;
					time(&now);
					ch->m_pd4Time = now;
				}
				break;
			}
		}
#endif
	
		ch->m_bChangeStatus = true;
	
#ifdef NON_PK_SYSTEM
		// 존이동시 PK모드 체크
		if (ch->IsSetPlayerState(PLAYER_STATE_PKMODE) == false)			
		{	// PK모드가 아닐 경우	
			return;
		}
		
		bool bPKState = false;

		// 공성 체크
#ifdef ENABLE_WAR_CASTLE
		CWarCastle * castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
		if (castle)
		{
			if (castle->GetState() != WCSF_NORMAL)
			{
				if ( ch->GetMapAttr() == MAPATT_WARZONE)
				{
					bPKState = true;
				}
			}
		}
#endif // ENABLE_WAR_CASTLE
	
		if (ch->IsInPeaceZone(false) == false)		
		{	// PK존
			if (ch->GetMapAttr() == MAPATT_FREEPKZONE)
			{
				bPKState = true;
			}
		}

		if (bPKState == false)
		{
			CNetMsg msg;
			ch->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
			ActionMsg(msg, ch, 0, AGT_PKMODE);
			SEND_Q(msg, ch->m_desc);
		}
#endif // NON_PK_SYSTEM		
	}
	else
	{

		// 서버 외부로 이동 처리

		// 존 이동 메시지 생성
		GoZoneMsg(ch->m_desc->m_msgGoZone, pZone->m_index, 0, pZone->m_remoteIP, pZone->m_remotePort);
		// 좌표 설정
		ch->m_pZone = pZone;
		ch->m_pArea = NULL;
		GET_YLAYER(ch)	= ylayer;
		GET_X(ch)		= x;
		GET_Z(ch)		= z;
		ch->m_desc->m_bGoZone = true;
		ch->m_desc->m_bGoZoneSent = false;
		gserver.CloseSocket(ch->m_desc, true);
		return ;
	}
}

void GoTo(CCharacter* ch, char y, float x, float z, float h, float r, char bHack)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL 
		&& IS_PC(ch))
	{
		pCastle->CheckRespond(TO_PC(ch));
	}
#endif // DRATAN_CASTLE

	CNetMsg rmsg;

	// 셀안에 알리고
	ch->SendDisappearAllInCell(true);

	// 셀에서 빼고
	ch->m_pArea->CharFromCell(ch, true);

#ifdef ATTACK_PET
	if(IS_APET(ch))
	{
		CAPet* apet = TO_APET(ch);
		CPC* ower = apet->GetOwner();
		if(ower)
			apet->m_pArea = ower->m_pArea;
	}
#endif // ATTACK_PET

	// 이동
	ch->Move(x, z, h, r, y);

	// 셀에 넣고
	int cx, cz;
	ch->m_pArea->PointToCellNum(x, z, &cx, &cz);
	ch->m_pArea->CharToCell(ch, y, cx, cz);

	if (IS_PC(ch))
	{
		// 현재 스킬 시전 취소
		if( ch->m_currentSkill != NULL )
		{
			ch->m_currentSkill->Cancel(ch);
			ch->m_currentSkill = NULL;
			SkillCancelMsg(rmsg, ch);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}


		CPC* pc = TO_PC(ch);
		pc->ResetPlayerState(PLAYER_STATE_SITDOWN);

		// 이동 결과 알리고
		GotoMsg(rmsg, pc, bHack);
		SEND_Q(rmsg, pc->m_desc);
		// 셀 정보 보내고
		pc->m_pArea->SendCellInfo(pc, true);

#ifdef SAFEGUARD
		CharStatusMsg(rmsg, ch, 0);
		SEND_Q(rmsg, pc->m_desc);
#endif // SAFEGUARD
	}

	// 나타나는 거 알리고
	AppearMsg(rmsg, ch, false);
	if (IS_PC(ch))
	{
		ch->m_pArea->SendToCell(rmsg, ch);

#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
		CPC* pc = TO_PC(ch);
		if( pc->m_guildInfo )
		{		
			GuildPointRankingMsg( rmsg, pc->m_index, pc->m_guildInfo->guild()->index(), pc->m_guildInfo->guild()->GetGuildPointRanking() );
			ch->m_pArea->SendToCell( rmsg, ch );
		}
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD if( m_guildInfo && m_guildInfo->guild() )
		
#ifdef DYNAMIC_DUNGEON
		GuildNameColorStateMsg(rmsg, TO_PC(ch) );
		ch->m_pArea->SendToCell( rmsg, TO_PC(ch) );
#endif //DYNAMIC_DUNGEON
	}
	else
		ch->m_pArea->SendToCell(rmsg, ch, true);

#ifdef ENABLE_PET
	CPet* pet = NULL;
	if (IS_PC(ch))
	{
		CPC* pc = TO_PC(ch);
		pet = pc->GetPet();
		if (pet && pet->IsSummon())
		{
			GoTo(pet, y, x, z, h, r);
		}
	}
#endif

	if (IS_PC(ch))
	{
		CElemental* elemental = NULL;
		CPC* pc = TO_PC(ch);
		elemental = pc->m_elementalList;
		while (elemental)
		{
			GoTo(elemental, y, x, z, h, r);
			elemental = elemental->m_nextElemental;
		}
	}

#ifdef ATTACK_PET
	CAPet* apet = NULL;
	if (IS_PC(ch))
	{
		CPC* pc = TO_PC(ch);
		apet = pc->GetAPet();
		if (apet && apet->IsSummon())
		{
			GoTo(apet, y, x, z, h, r);
		}
	}

	if ( IS_APET(ch) )
	{
		apet = TO_APET(ch);
		ExAPetStatusMsg( rmsg, apet );
		SEND_Q( rmsg, apet->GetOwner()->m_desc );
	}
#endif //ATTACK_PET
}

#ifdef GM_GO_ZONE
void GMGoZone(CPC* ch, CPC* pc)
{
	int zone = pc->m_pZone->m_index;
	char ylayer = pc->m_pos.m_yLayer;
	float x = pc->m_pos.m_x;
	float z = pc->m_pos.m_z;

#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(ch);
	}
#endif // DRATAN_CASTLE

	switch(zone)
	{
		case ZONE_SINGLE_DUNGEON3:
		case ZONE_SINGLE_DUNGEON4:
			if( ch->m_admin < 2 && !ch->CheckDungeonData(zone) )
			{
				GAMELOG << init("PD_BUG_GO_ZONE", ch)
						<< end;
				return;
			}
		default:
			break;
	}

	CNetMsg rmsg;
	bool bGoto = false;
	bool bValid = false;
	CZone* pZone = NULL;

#ifdef MONSTER_COMBO
	CParty* pParty = NULL;
#endif // MONSTER_COMBO

	int i = 0;
	if (ch->m_pZone->m_index == zone)
	{
		// 같은 존안에서의 이동
		bGoto = true;
		bValid = true;
		pZone = ch->m_pZone;
	}
	else
	{
		i = gserver.FindZone(zone);
		if (i != -1)
		{
			pZone = gserver.m_zones + i;
			if (pZone->m_bRemote)
			{
				// 외부 서버는 인스턴스 던전은 불가능
				if (!pZone->IsGuildRoom() && !pZone->IsPersonalDungeon())
				{
					bGoto = false;
					bValid = true;
				}
			}
			else
			{
				bGoto = true;
				bValid = true;
			}
		}
	}

	if (!bValid)
	{
		// 오류
		return ;
	}

	CZone* pCurZone = ch->m_pZone;
	CArea* pCurArea = ch->m_pArea;

	// 존이 바뀌면 
	if (pCurZone != pZone)
	{
		// 파티 처리
		if (ch->m_party != NULL)
		{
			// 초대하거나, 초대받은 사람일 경우 거절로 처리
			if (ch->m_party->GetRequestIndex() > 0 && (ch->m_party->GetBossIndex() == ch->m_index || ch->m_party->GetRequestIndex() == ch->m_index))
			{
				CNetMsg tmpmsg;
				PartyMsg(tmpmsg, MSG_PARTY_REJECT);
				do_Party(ch, tmpmsg);
			}
			// 갈곳이 퍼던이면 파티탈퇴
			else if (pZone->IsPersonalDungeon())
			{
				CNetMsg tmpmsg;
				PartyMsg(tmpmsg, MSG_PARTY_QUIT);
				do_Party(ch, tmpmsg);
			}
#if defined (MONSTER_COMBO) || defined (EXTREME_CUBE)
			if(ch->IsParty())
			{
				pParty = ch->m_party;
			}
#endif // MONSTER_COMBO

			ch->m_party->SetMemberPCPtr(ch->m_index, NULL);
			ch->m_party = NULL;
		}
	} // 존이 바뀌면

	bool bSetPrePlay = false;
	if (bGoto)
	{
		ch->ProcDisconnect(false, (pCurZone == pZone));

		// 서버내의 이동

		i = pCurArea - pCurZone->m_area;

		// 가야할 존이 레이어 존 이면
		if (pZone->m_countArea > 1)
		{
			if(pc->m_pArea->m_bEnable)
				i = pc->m_pArea->m_index;
			else
				return ;
		}
		else
			i= 0;

		// 존이 바뀌면 
		if (pCurZone != pZone)
		{
			// 다른 존으로 이동
			// 캐릭터를 프리플레이 상태로
			gserver.m_playerList.Remove(ch);
			ch->m_bPlaying = false;
			STATE(ch->m_desc) = CON_PREPARE_PLAY;
			bSetPrePlay = true;

#ifdef ENABLE_PET
			// 060310 : bs : 펫 착용해제 추가
//			CPet* pet = ch->GetPet();
//			if (pet)
//			{
//				// 착용중인 펫이 이동할 존에서 소환/마운트 불가능 할 경우 착용 해제
//				if ((!pZone->m_bCanSummonPet && !pet->IsMountType()) || (!pZone->m_bCanMountPet && pet->IsMountType()))
//				{
//					ItemWearMsg(rmsg, WEARING_PET, NULL, NULL);
//					do_ItemWear(ch, rmsg);
//					ExPetStatusMsg(rmsg, pet);
//					SEND_Q(rmsg, ch->m_desc);
//				}
//			}
			// 060310 : bs : 펫 착용해제 추가
#endif

			// DB에 저장
			ch->m_lastupdate = 0;
		} // 다른 존으로 이동

		// 캐릭터의 위치 설정
		ch->m_pZone = pZone;
		ch->m_pArea = ch->m_pZone->m_area + i;

		GET_YLAYER(ch)	= ylayer;
		GET_R(ch)		= 0.0f;
		GET_X(ch)		= x;
		GET_Z(ch)		= z;

		GotoMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);

		switch(zone)
		{
		case ZONE_SINGLE_DUNGEON1:
		case ZONE_SINGLE_DUNGEON2:
		case ZONE_SINGLE_DUNGEON_TUTORIAL:
		case ZONE_SINGLE_DUNGEON3:
		case ZONE_SINGLE_DUNGEON4:
			return ;
		case ZONE_COMBO_DUNGEON:
			ch->m_gmGoZoneType = GM_GOZONE_COMBO;
			break;
		default:
			ch->m_gmGoZoneType = GM_GOZONE_NONE;
			break;
		}

#ifdef NEW_GUILD
		if( ch->m_guildInfo && ch->m_guildInfo->guild() )
		{
			ch->m_guildInfo->zoneindex( pZone->m_index );
		}
#endif // NEW_GUILD

		if (!bSetPrePlay)
		{
			int cx, cz;
			ch->m_pArea->PointToCellNum(GET_X(ch), GET_Z(ch), &cx, &cz);
			ch->m_pArea->CharToCell(ch, GET_YLAYER(ch), cx, cz);

			AppearMsg(rmsg, ch, true);
			ch->m_pArea->SendToCell(rmsg, ch);

			ch->m_pArea->SendCellInfo(ch, true);
#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
			if( ch->m_guildInfo && ch->m_guildInfo->guild() )
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
			if (IS_PC(ch))
			{
				CPC* pc = TO_PC(ch);
				CPet* pet = pc->GetPet();
				if (pet && pet->IsSummon())
				{
					GoTo(pet, GET_YLAYER(ch), x, z, ch->m_pArea->GetHeight(GET_YLAYER(ch), x, z), GET_R(pet));
				}
			}
#endif

			if (IS_PC(ch))
			{
				CPC* pc = TO_PC(ch);
				CElemental* elemental = pc->m_elementalList;
				while (elemental)
				{
					GoTo(elemental, GET_YLAYER(ch), x, z, ch->m_pArea->GetHeight(GET_YLAYER(ch), x, z), GET_R(elemental));
					elemental = elemental->m_nextElemental;
				}
			}
			
#ifdef ATTACK_PET
			if (IS_PC(ch))
			{
				CPC* pc = TO_PC(ch);
				CAPet* apet = pc->GetAPet();
				if (apet && apet->IsSummon())
				{
					GoTo(apet, GET_YLAYER(ch), x, z, ch->m_pArea->GetHeight(GET_YLAYER(ch), x, z), GET_R(apet));
				}
			}
#endif //ATTACK_PET
		}

		if (!DEAD(ch))
		{
			// GO_ZONE 효과 메시지 전송
			EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_GOZONE);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}

#ifdef ENABLE_SINGLEDUNGEON_DATA
		if (ch->m_admin < 2)
		{
			switch (zone)
			{
			case ZONE_SINGLE_DUNGEON3:
				ch->m_pd3Count++;

				if(ch->m_pd3Count == 1 || ch->m_pd3Count > 6)
				{
					time_t now;
					time(&now);
					ch->m_pd3Time = now;
				}
				break;
			case ZONE_SINGLE_DUNGEON4:
				ch->m_pd4Count++;
				if(ch->m_pd4Count == 1 || ch->m_pd4Count > 4)
				{
					time_t now;
					time(&now);
					ch->m_pd4Time = now;
				}
				break;
			}
		}
#endif
	
		ch->m_bChangeStatus = true;
	
#ifdef NON_PK_SYSTEM
		// 존이동시 PK모드 체크
		if (ch->IsSetPlayerState(PLAYER_STATE_PKMODE) == false)			
		{	// PK모드가 아닐 경우	
			return;
		}
		
		bool bPKState = false;

		// 공성 체크
#ifdef ENABLE_WAR_CASTLE
		CWarCastle * castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
		if (castle)
		{
			if (castle->GetState() != WCSF_NORMAL)
			{
				if ( ch->GetMapAttr() == MAPATT_WARZONE)
				{
					bPKState = true;
				}
			}
		}
#endif // ENABLE_WAR_CASTLE
	
		if (ch->IsInPeaceZone(false) == false)		
		{	// PK존
			if (ch->GetMapAttr() == MAPATT_FREEPKZONE)
			{
				bPKState = true;
			}
		}

		if (bPKState == false)
		{
			CNetMsg msg;
			ch->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
			ActionMsg(msg, ch, 0, AGT_PKMODE);
			SEND_Q(msg, ch->m_desc);
		}
#endif // NON_PK_SYSTEM		
	}
	else
	{

		// 서버 외부로 이동 처리

		// 존 이동 메시지 생성
		GoZoneMsg(ch->m_desc->m_msgGoZone, pZone->m_index, 0, pZone->m_remoteIP, pZone->m_remotePort);
		// 좌표 설정
		ch->m_pZone = pZone;
		ch->m_pArea = NULL;
		GET_YLAYER(ch)	= ylayer;
		GET_X(ch)		= x;
		GET_Z(ch)		= z;
		ch->m_desc->m_bGoZone = true;
		ch->m_desc->m_bGoZoneSent = false;
		gserver.CloseSocket(ch->m_desc, true);
		return ;
	}
}
#endif // GM_GO_ZONE
