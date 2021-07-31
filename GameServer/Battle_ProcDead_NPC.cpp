#include "stdhdrs.h"
#include "Server.h"
#include "Battle.h"
#include "WarCastle.h"
#include "CmdMsg.h"
#include "Log.h"

void ProcDead_PD2(CNPC* df);

void ProcDead(CNPC* df, CCharacter* of)
{
	CPC*		opc				= NULL;
	CNPC*		onpc			= NULL;
#ifdef ENABLE_PET
	CPet*		opet			= NULL;
#endif // #ifdef ENABLE_PET
	CElemental*	oelemental		= NULL;
#ifdef ATTACK_PET
	CAPet*		oapet			= NULL;
#endif // ATTACK_PET

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
			goto END_PROC;
		break;

	case MSG_CHAR_ELEMENTAL:
		oelemental = TO_ELEMENTAL(of);
		opc = oelemental->GetOwner();
		if (opc == NULL)
			goto END_PROC;
		break;
#ifdef ATTACK_PET
	case MSG_CHAR_APET:
		oapet	= TO_APET(of);
		opc		= oapet->GetOwner();
		if( opc == NULL )
			goto END_PROC;
#endif //ATTACK_PET 
		break;

	default:
		goto END_PROC;
	}

#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
	// 죽인 것이 테이밍 몬스터일 경우
	if (onpc && onpc->Check_MobFlag( STATE_MONSTER_TAMING ) ) 
	{
		CPC* pOwnerCheck = NULL;
		CPC* owner = NULL;				// 몬스터를 테이밍한 캐릭터 
		owner = onpc->GetOwner();		// 몬스터가 테이밍 되었는지 확인

		// 주인을 찾는다.
		if ( owner )
		{
			pOwnerCheck = gserver.m_playerList.Find(owner->m_index);
		}

		// 주인이 공격하고 있는 타겟을 지워준다.
		if ( pOwnerCheck )
		{
			pOwnerCheck->SetOwners_target(NULL);
		}
		goto SKIP_DROP;
	}

	// 이곳으로 넘어오면 테이밍이 아니므로, 모든 몬스터는 몬스터에게 죽으면 패스
	else if (onpc)
	{
		goto SKIP_DROP;
	}

	// pc가 npc를 죽이면 테이밍 몬스터의 타겟을 지워준다.
	if (opc)
	{
		opc->SetOwners_target(NULL);
	}
#else
	// 경비에게 죽으면 그냥 패수
	if (onpc && ( onpc->m_proto->CheckFlag(NPC_GUARD) ) )
		goto SKIP_DROP;
#endif  // NIGHTSHADOW_SKILL

	// 공주구출 퀘스트 (퍼스널 던전 2) 실패
	ProcDead_PD2(df);

	// 리더 사망시 처리
	if (!df->m_proto->CheckFlag(NPC_RAID))
		ProcFollowNPC(df);

#ifdef ENABLE_WAR_CASTLE
	// 공성 포인트 계산
	if (opc)
		CalcWarPoint(opc, df);
#endif

	// 죽은 NPC가 공성탑이나 수호병이 아닐 경우 처리
	if (!df->m_proto->CheckFlag(NPC_CASTLE_TOWER | NPC_CASTLE_GUARD))
	{
		int level = -1;
#ifdef NEW_DIVISION_EXPSP
		LONGLONG nTotalDamage = 0;
		// 우선권 PC, 평균 레벨 구하기
		CPC* tpc = FindPreferencePC(df, &level, &nTotalDamage);
#else // #ifdef NEW_DIVISION_EXPSP
		// 우선권 PC, 평균 레벨 구하기
		CPC* tpc = FindPreferencePC(df, &level);
#endif // #ifdef NEW_DIVISION_EXPSP

		// 보스몹
		if (df->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS | NPC_RAID))
		{
			GAMELOG << init("MOB DEAD")
					<< "INDEX" << delim
					<< df->m_proto->m_index << delim
					<< "NAME" << delim
					<< df->m_name << delim
					<< "ZONE" << delim
					<< df->m_pZone->m_index << delim
					<< "POSITION" << delim
					<< GET_X(df) << delim
					<< GET_Z(df) << delim
					<< GET_YLAYER(df) << delim
					<< "KILL BY" << delim;
			if (opc)
			{
				GAMELOG << opc->m_index << delim
						<< opc->m_name << delim
						<< opc->m_nick << delim
						<< opc->m_job << delim
						<< opc->m_job2 << delim
						<< opc->m_level;
			}
			else
			{
				GAMELOG << of->m_type << delim
						<< of->m_index << delim
						<< of->m_name << delim
						<< of->m_level;
			}
			GAMELOG << end;
#ifndef ADULT_SERVER			
			if (df->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS))
			{
				// 카오 성향 회복 : 보스몹을 잡으면 회복 보너스
				if (opc && opc->IsChaotic() && tpc == opc)
				{
#ifdef RESTRICT_PK
					if (opc->m_lastPKTime == -1 || opc->m_lastPKTime + RESTRICT_PK <= gserver.m_gameTime)
					{
#endif // RESTRICT_PK
#if defined ( NON_PK_SYSTEM )
						if( !gserver.m_bNonPK )
							opc->m_pkPenalty += df->m_level / 10;				
#else
						opc->m_pkPenalty += df->m_level / 10;
#endif // NON_PK_SYSTEM
						if (opc->m_pkPenalty > 0)
							opc->m_pkPenalty = 0;
#ifdef RESTRICT_PK
					}
#endif // RESTRICT_PK
	
					CNetMsg rmsg;
					CharStatusMsg(rmsg, opc, 0);
					opc->m_pArea->SendToCell(rmsg, opc, false);
					opc->m_bChangeStatus = true;
				}
			}
#endif //ifndef ADULT_SERVER
		} // 보스몹

#ifndef ADULT_SERVER
#ifdef RESTRICT_PK
#else // RESTRICT_PK
		// 카오 성향 회복
		if (opc && opc->IsChaotic() && df->m_level >= opc->m_level - 5)
		{
			opc->m_pkRecoverNPCCount++;
			if (opc->m_pkRecoverNPCCount >= 25)
			{
				opc->m_pkRecoverNPCCount = 0;
#if defined ( NON_PK_SYSTEM )
				if( !gserver.m_bNonPK )
					opc->m_pkPenalty++;
#else
				opc->m_pkPenalty++;
#endif

				CNetMsg rmsg;
				CharStatusMsg(rmsg, opc, 0);
				opc->m_pArea->SendToCell(rmsg, opc, false);
				opc->m_bChangeStatus = true;
			}
		} // 카오 성향 회복
#endif // RESTRICT_PK
#endif // ifndef ADULT_SERVER	
		// Exp, SP 분배
		// 이루틴중 레벨업을 하여 존이동을 하였을 경우 나머지 실행하지 않는다.
#ifdef NEW_DIVISION_EXPSP
		if( DivisionExpSP(df, tpc, nTotalDamage) )
#else // #ifdef NEW_DIVISION_EXPSP
		if( DivisionExpSP(df, tpc) )
#endif // #ifdef NEW_DIVISION_EXPSP
			goto END_PROC;

		// 아이템 드롭
		ProcDropItemAfterBattle(df, opc, tpc, level);
#ifdef NEW_PET_EXP_SYSTEM
		if (tpc && tpc->m_wearing[WEARING_PET] && tpc->GetPet())
		{
			CPet* pPet = tpc->GetPet();
			pPet->AddExp(tpc->m_level, df->m_level, df->m_proto->GetFlag() );
		}
#endif // NEW_PET_EXP_SYSTEM

#ifdef ADULT_SERVER
		if( opc && opc->IsChaotic() && df->m_proto->m_level >= opc->m_level )	// 카오가 동렙이상몹을 잡으면 
		{
			if( opc->AddKillNpcCount() )
			{
				opc->m_pkPenalty += 1;		// 카오라면 기본 0보다 작다.

				CNetMsg rmsg;
				CharStatusMsg(rmsg, opc, 0);
				opc->m_pArea->SendToCell(rmsg, opc, false);
				opc->m_bChangeStatus = true;
				
				char cChangeKing = opc->IsUpdateKing();
				if( cChangeKing )
				{
					CNetMsg KingChangeMsg;		// MSG_HELPER_UPDATE_CHAOKING  bChao : 1(Guardian),2(ChaoKing) , index , penalty
					HelperUpdateKing( KingChangeMsg, cChangeKing, opc->m_index, opc->m_pkPenalty );
					SEND_Q( KingChangeMsg, gserver.m_helper );
				}
			}
		}
#endif //ADULT_SERVER
		
		// 진행중인 Quest 중 죽은 npc로 진행중이면 UpdateData
		if (opc && opc == tpc)
		{
			if( opc->m_pZone->IsPersonalDungeon() )
			{
				opc->m_pArea->m_nMakeNPC++;

				if( (df->m_proto->m_index == 5 || df->m_proto->m_index == 201 ) && opc->m_pArea->m_nMakeNPC < 50 )
				{
					goto SKIP_DROP;
				}
			}

#ifdef QUEST_DATA_EXTEND
			CQuest* pQuest;
			CQuest* pQuestNext = opc->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
			while ((pQuest = pQuestNext))
			{
				pQuestNext = opc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
				// 퀘스트 있고 수행중이고 반복, 수집, 격파, 구출 퀘스트이면
				switch (pQuest->GetQuestType0())
				{
				case QTYPE_KIND_REPEAT:
				case QTYPE_KIND_COLLECTION:
				case QTYPE_KIND_DEFEAT:
				case QTYPE_KIND_SAVE:
					pQuest->QuestUpdateData(opc, df);
					break;

				default:
					break;
				}
			}
#else // QUEST_DATA_EXTEND
			int i;
			for (i=0; i < QUEST_MAX_PC; i++)
			{
				// 퀘스트 있고 수행중이고 반복, 수집, 격파, 구출 퀘스트이면
				if (opc->m_questList.m_list[i] && opc->m_questList.m_bQuest[i] &&
					(opc->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_REPEAT ||
					opc->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_COLLECTION ||
					opc->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_DEFEAT ||
					opc->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_SAVE))
				{
					opc->m_questList.m_list[i]->QuestUpdateData(opc, df);
				}
			}
#endif // QUEST_DATA_EXTEND
		}
	} // 죽은 NPC가 공성탑이나 수호병이 아닐 경우 처리

SKIP_DROP:

#ifdef ENABLE_WAR_CASTLE
	// 수호탑은 DelNPC() 안하고 UpdateGateState() 후에 메시지로 알린다.
	if (df->m_proto->CheckFlag(NPC_CASTLE_TOWER) != 0)
	{
		int gstate_old = 0, gstate_new = 0;
		CNetMsg rmsg;

		CWarCastle * castle = CWarCastle::GetCastleObject(ZONE_MERAC);
#ifdef DRATAN_CASTLE
		CDratanCastle * pCastle = CDratanCastle::CreateInstance();
#endif // DRATAN_CASTLE

		if (of->m_pZone->m_index == ZONE_MERAC)
		{		
			if (castle != NULL)
			{
				gstate_old = castle->GetGateState();
#ifdef DRATAN_CASTLE
				if (pCastle != NULL)
				{
					gstate_old |= pCastle->GetGateState();
				}
#endif // DRATAN_CASTLE
				castle->UpdateGateState(df);
				gstate_new = castle->GetGateState();
#ifdef DRATAN_CASTLE
				if (pCastle != NULL)
				{
					gstate_new |= pCastle->GetGateState();
				}
#endif // DRATAN_CASTLE
			}
		}
#ifdef DRATAN_CASTLE
		else if (of->m_pZone->m_index == ZONE_DRATAN)
		{	
			if (pCastle != NULL)
			{
				gstate_old = pCastle->GetGateState();

				if (castle != NULL)
				{
					gstate_old |= castle->GetGateState();
				}

				pCastle->UpdateGateState(df);
				gstate_new = pCastle->GetGateState();

				if (castle != NULL)
				{
					gstate_new |= castle->GetGateState();
				}
			}

			if (df->m_proto->CheckFlag(NPC_WARCASTLE) != 0)
			{	// NPC_CASTLE_TOWER 에 NPC_WARCASTLE 면 
				// 마스터 타워와 부활진지
				int qindex = df->m_proto->m_index;
				if (qindex >= 390 && qindex <= 396)
				{	// 부활진지 파괴 알림
					CastleTowerQuartersCrushMsg(rmsg, qindex);
					of->m_pArea->SendToAllClient(rmsg);
					
					// 부활진지 파괴 처리
					pCastle->m_nRebrithGuild[df->m_proto->m_index - 390] =  -1;
					memset((void *)pCastle->m_strRebrithGuild[df->m_proto->m_index - 390], 0, 51);
				}
			}
		}
#endif // DRATAN_CASTLE

		if (gstate_old != gstate_new)
		{
			GuildWarGateStateMsg(rmsg, gstate_old, gstate_new);		
			of->m_pArea->SendToAllClient(rmsg);
		}
			
		DelAttackList(df);
		
#ifdef DRATAN_CASTLE
		if (of->m_pZone->m_index == ZONE_DRATAN)
		{
			if( df->m_proto->m_index == 351)
			{	// 마스터 타워
				// 모든 타워 기능 정지
				pCastle->StopCastleTower();
			}
			else if (df->m_proto->CheckFlag(NPC_CASTLE_TOWER) != 0)
			{
				// 공격 수호상 (결계의 눈 등..) 로그 
				GAMELOG << init("DRATAN CASTLE NPC DEAD : ") << df->m_proto->m_name
						<< " BROKEN BY : " << of->m_name << end;
				// 마스터 타워가 아닌 모든 타워
				int  i;
				// 부활 진지 삭제
				for (i=0; i<7; i++)
				{
					if (pCastle->m_pRebrithNPC[i] == df)
					{
						pCastle->m_pRebrithNPC[i] = NULL;
						pCastle->m_nRebrithGuild[i] = -1;
						memset((void *)pCastle->m_strRebrithGuild[i], 0, 51);

					}
				}

				// 워프 타워 삭제
				for (i=0; i<5; i++)
				{
					if (pCastle->m_pWarpNPC[i] == df)
					{
						pCastle->m_pWarpNPC[i] = NULL;
					}
				}

				// 결계의 눈 삭제
				for (i=0; i<3; i++)
				{
					if (pCastle->m_gateNPC[i] == df)
					{
						pCastle->m_gateNPC[i] = NULL;
					}
				}
				
				// 알림
				of->m_pArea->CharFromCell(df, true);
				of->m_pArea->DelNPC(df);
			}
		}
#endif // DRATAN_CASTLE
		return ;
	} // 수호탑은 DelNPC() 안하고 UpdateGateState() 후에 메시지로 알린다.
#endif

#ifdef EXTREME_CUBE
	if(df->m_bCubeRegen)
	{
		CCubeSpace* cube = gserver.m_extremeCube.GetExtremeCube(df->m_pArea->m_index);
		if(cube)
		{
			cube->DelMob(df);  

			if(gserver.m_extremeCube.IsGuildCubeTime() && opc && opc->m_guildInfo && opc->m_guildInfo->guild())
			{
				CCubeMemList* CubeMemList;
				CubeMemList = gserver.m_extremeCube.FindMemList(opc->m_guildInfo->guild());
				if(CubeMemList)
				{
					CNetMsg msg;
					long lastCubePoint;

					time(&lastCubePoint);
					HelperAddCubePointMsg(msg, opc->m_guildInfo->guild()->index(), df->m_level, lastCubePoint);
					SEND_Q(msg, gserver.m_helper);
				}
			}
		}
	}

	if(df->m_pZone != NULL && df->m_proto->m_index == 529 && df->m_pZone->IsExtremeCube())
	{
		CCubeSpace* cube = gserver.m_extremeCube.GetExtremeCube(df->m_pArea->m_index);
		
		if(cube && (cube->m_crystal == df) )
		{
			// cube->m_crystal = NULL;

			cube->DelCrystal(false);
			cube->m_waitTime = gserver.m_pulse + PULSE_REAL_SEC * 10;
			return ;
		}
	}
	else if(df->m_pZone != NULL && df->m_proto->m_index == 527 && df->m_pZone->IsExtremeCube())
	{
		CCubeSpace* cube = gserver.m_extremeCube.GetExtremeCube(df->m_pArea->m_index);
		
		if(cube && (cube->m_crystal == df) )
		{
			// cube->m_crystal = NULL;
			
			cube->DelCrystal(false);
			cube->m_waitTime = gserver.m_pulse + PULSE_REAL_SEC * 10;
			return ;
		}
	}
#endif // EXTREME_CUBE

END_PROC:

#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
	// 모든 몬스터는 몬스터에게 죽으면 패스
	if ( onpc )
	{
		// 해당 에어리어에 죽은 수를 표시한다.  여기서는 바로 안지우고 따로 처리..
		onpc->m_pArea->m_nNPC_Killed_NPC++;
	}

	else
	{
#endif  // NIGHTSHADOW_SKILL

	DelAttackList(df);		
	of->m_pArea->CharFromCell(df, true);
	of->m_pArea->DelNPC(df);

#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
	}
#endif  // NIGHTSHADOW_SKILL

}

void ProcDead_PD2(CNPC* df)
{
	if (df->m_pZone->m_index == ZONE_SINGLE_DUNGEON2)
	{
		CCharacter* ch = NULL;
		CPC* pc = NULL;

		int i, j;
		for (i = 0; i < df->m_pArea->m_size[0]; i++)
		{
			for (j = 0; j < df->m_pArea->m_size[1]; j++)
			{
				ch = df->m_pArea->m_cell[i][j].m_listChar;
				while (ch)
				{
					if (IS_PC(ch) && !DEAD(ch))
					{
						pc = TO_PC(ch);
						break;
					}
					ch = ch->m_pCellNext;
				}
			}
		}

		if (!pc)
			return ;

		// 퀘스트 실패
#ifdef QUEST_DATA_EXTEND

		CQuest* pQuest = NULL;
		CQuest* pQuestNext = NULL;
		const CQuestProto* pQuestProto = NULL;
		pQuestNext = pc->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
		while ((pQuest = pQuestNext))
		{
			pQuestNext = pc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
			// 구출형 퀘스트 이고 
			if (pQuest->GetQuestType0() == QTYPE_KIND_SAVE)
			{
				pQuestProto = pQuest->GetQuestProto();
				int j;
				for (j = 0; j < QUEST_MAX_CONDITION; j++)
				{
					// 죽은 df가 구출할 공주 이면
					if (pQuestProto->m_conditionType[j] == QCONDITION_NPC &&
						pQuestProto->m_conditionIndex[j] == df->m_proto->m_index)
					{
						// 아이템 삭제
						pQuest->RemoeQuestItem(pc);

						// Quest Fail Log
						GAMELOG << init("QUEST FAIL", pc)
								<< pQuestProto->m_index
								<< end;
						
						CNetMsg rMsg;
						QuestFailMsg(rMsg, pQuest);
						pc->m_questList.DelQuest(pc, pQuest);
						SEND_Q(rMsg, pc->m_desc);
						return ;
					}
				}
			}
		} // while (pQuest)

#else // QUEST_DATA_EXTEND

		for (i=0; i < QUEST_MAX_PC; i++)
		{
			if (pc->m_questList.m_list[i] && pc->m_questList.m_bQuest[i])
			{
				// 구출형 퀘스트 이고 
				if (pc->m_questList.m_list[i]->m_proto->m_type[0] == QTYPE_KIND_SAVE)
				{
					int j;
					for (j=0; j < QUEST_MAX_CONDITION; j++)
					{
						// 죽은 df가 구출할 공주 이면
						if (pc->m_questList.m_list[i]->m_proto->m_conditionType[j] == QCONDITION_NPC &&
							pc->m_questList.m_list[i]->m_proto->m_conditionIndex[j] == df->m_proto->m_index)
						{
							// 아이템 삭제
							int k;
							for (k=0; k < QUEST_MAX_CONDITION; k++)
							{
								// 수행조건 아이템을 찾고
								if (pc->m_questList.m_list[i]->m_proto->m_conditionType[k] == QCONDITION_ITEM)
								{
									// 전달 퀘스트 아이템은 퀘스트 인벤에서만 !!!
									int r, c;
									if (!pc->m_invenQuest.FindItem(&r, &c, pc->m_questList.m_list[i]->m_proto->m_conditionIndex[k], 0, 0))
									{
										// Quest Fail Log
										GAMELOG << init("QUEST FAIL", pc)
												<< pc->m_questList.m_list[i]->m_proto->m_index
												<< end;

										// 없으면 바로 실패
										CNetMsg rMsg;
										QuestFailMsg(rMsg, pc->m_questList.m_list[i]);
										pc->m_questList.DelQuest(pc, pc->m_questList.m_list[i]);
										SEND_Q(rMsg, pc->m_desc);
										return ;
									}
									
									// 찾았으면 수량 확인 해서 줄이기
									CItem* item = pc->m_invenQuest.GetItem(r, c);
									
									if (!item)
									{
										// Quest Fail Log
										GAMELOG << init("QUEST FAIL", pc)
												<< pc->m_questList.m_list[i]->m_proto->m_index
												<< end;

										// 없으면 바로 실패
										CNetMsg rMsg;
										QuestFailMsg(rMsg, pc->m_questList.m_list[i]);
										pc->m_questList.DelQuest(pc, pc->m_questList.m_list[i]);
										SEND_Q(rMsg, pc->m_desc);
										return ;
									}
									
									// 필요 숫자 만큼 없음
									if (item->Count() < pc->m_questList.m_list[i]->m_proto->m_conditionNum[k])
									{
										// Quest Fail Log
										GAMELOG << init("QUEST FAIL", pc)
												<< pc->m_questList.m_list[i]->m_proto->m_index
												<< end;

										// 없으면 바로 실패
										CNetMsg rMsg;
										QuestFailMsg(rMsg, pc->m_questList.m_list[i]);
										pc->m_questList.DelQuest(pc, pc->m_questList.m_list[i]);
										SEND_Q(rMsg, pc->m_desc);
										return ;
									}
									CNetMsg itemMsg;
									
									// Item 수량 변경
									DecreaseFromInventory(pc, item, pc->m_questList.m_list[i]->m_proto->m_conditionNum[k]);
									
									if (item->Count() > 0)
									{
										ItemUpdateMsg(itemMsg, item, -pc->m_questList.m_list[i]->m_proto->m_conditionNum[k]);
										SEND_Q(itemMsg, pc->m_desc);
									}
									else
									{
										ItemDeleteMsg(itemMsg, item);
										SEND_Q(itemMsg, pc->m_desc);
										pc->RemoveItemFromQuickSlot(item);
										RemoveFromInventory(pc, item, true, true);
									}

									// Quest Fail Log
									GAMELOG << init("QUEST FAIL", pc)
											<< pc->m_questList.m_list[i]->m_proto->m_index
											<< end;
									
									// 없으면 바로 실패
									CNetMsg rMsg;
									QuestFailMsg(rMsg, pc->m_questList.m_list[i]);
									pc->m_questList.DelQuest(pc, pc->m_questList.m_list[i]);
									SEND_Q(rMsg, pc->m_desc);
									return ;
								}
							}
						}
					}
				}
			}
		}

#endif // QUEST_DATA_EXTEND
	}
}
