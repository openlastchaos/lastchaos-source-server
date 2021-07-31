/************************************
  수정사항 
  수정한 사람: bs
  수정일: 2004-11-18 오후 3:37:20
  설명: 스탯 변화 적용
 ************************************/

#include "stdhdrs.h"
#include "Server.h"
#include "Skill.h"
#include "Assist.h"
#include "CmdMsg.h"
#include "Log.h"

#define APPVAL(v)		{ \
							switch (mp->m_damagetype) \
							{ \
							case MDT_ADDITION: \
								addition->v += mlp->m_nPowerValue * CalcSkillParam(ch, ch, SPARAM_NONE, mp->m_ptp) / 100; \
								break; \
							case MDT_RATE: \
								rate->v += mlp->m_nPowerValue * CalcSkillParam(ch, ch, SPARAM_NONE, mp->m_ptp) / 100; \
								if( rate->v < -90 ) rate->v = -90; \
								break; \
							} \
						}
////////////////////
// class CAssistData

CAssistData::CAssistData(MSG_CHAR_TYPE spellertype, int spellerindex, int itemidx, const CSkillProto* proto, int level, int remain, bool bHit[MAX_SKILL_MAGIC])
{
	if (itemidx < 0)
		m_index = -1;
	else
		m_index = itemidx;

	m_proto = proto;
	m_level = level;
	m_remain = remain;
	memcpy(m_bHit, bHit, sizeof(bool) * MAX_SKILL_MAGIC);

	m_prev = NULL;
	m_next = NULL;

	m_spellerType = spellertype;
	m_spellerIndex = spellerindex;
}


////////////////////
// class CAssistList

CAssistList::CAssistList()
{
	m_head = NULL;
	m_tail = NULL;
	m_max = 0;
	m_count = 0;
	m_abscount = 0;
}

CAssistList::~CAssistList()
{
	while (m_head)
	{
		CAssistData* p = m_head->m_next;
		delete m_head;
		m_head = p;
	}

	m_head = NULL;
	m_tail = NULL;
}

void CAssistList::Max(int n)
{
	m_max = n;
}

bool CAssistList::Add(CCharacter* spellchar, int itemidx, const CSkillProto* proto, int level, bool bHit[MAX_SKILL_MAGIC], int& remain, int param, int nBlessAdd, int nBlessRate)
{
	if (proto && (proto->m_flag & SF_ABSTIME) && m_abscount >= MAX_ASSIST_ABS )
		return false;

	if (m_count >= m_max && !(proto->m_flag & SF_ABSTIME))
		return false;

	const CSkillLevelProto* levelproto = proto->Level(level);

	if (levelproto == NULL)
		return false;

	// 복구주문서
	if (levelproto->m_durtime < 1)
	{
		if(
			proto->m_index == 245 || proto->m_index == 246 || proto->m_index == 244
#ifdef AUTO_POTION
			|| proto->m_index == 466
#endif // AUTO_POTION
#ifdef EVENT_PCBANG_2ND
			|| proto->m_index == 493
#endif // EVENT_PCBANG_2ND
#ifdef NEW_CASHITEM_USA_2008
			|| proto->m_index == 553
#endif // NEW_CASHITEM_USA_2008
#ifdef EVENT_PHOENIX   // 피닉스 이벤트  yhj
			|| proto->m_index == 516    // 피닉스 버프
			|| proto->m_index == 589    // 피닉스 부활
#endif // EVENT_PHOENIX
			);
		else
			return false;
	}

	if (remain == -1)
	{
		remain = levelproto->m_durtime;
		remain = remain * (param / 100);
		remain += nBlessAdd
				+ remain * nBlessRate / SKILL_RATE_UNIT;
	}
#ifdef ENABLE_ROGUE_SKILL125_BRZ
	else
	{
		if(proto->m_index == 125)
		{
			remain = remain * (param / 100);
			remain += nBlessAdd
					+ remain * nBlessRate / SKILL_RATE_UNIT;
		}
	}
#endif // ENABLE_ROGUE_SKILL125_BRZ

	MSG_CHAR_TYPE spellertype = (spellchar) ? spellchar->m_type : MSG_CHAR_UNKNOWN;
	int spellerindex = (spellchar) ? spellchar->m_index : 0;
	CAssistData* p = new CAssistData(spellertype, spellerindex, itemidx, proto, level, remain, bHit);

	if (m_head == NULL)
	{
		// 하나도 없으면
		m_head = m_tail = p;
	}
	else
	{
		m_tail->m_next = p;
		p->m_prev = m_tail;
		m_tail = p;
	}

	if (proto && (proto->m_flag & SF_ABSTIME) )
		m_abscount++;
	else
		m_count++;

	return true;
}

bool CAssistList::CanApply(const CSkillProto* proto, int level)
{
	const CSkillLevelProto* inlevelproto = proto->Level(level);
	if (inlevelproto == NULL)
		return false;

	CAssistData* data;
	const CSkillLevelProto* listlevelproto;
	const CMagicProto* inmagic;
	const CMagicProto* listmagic;
	int i, j;

	// 리스트 루프
	for (data = m_head; data; data = data->m_next)
	{
		listlevelproto = data->m_proto->Level(data->m_level);
		if (listlevelproto == NULL)
			continue ;

		// 리스트의 스킬 루프
		for (i = 0; i < MAX_SKILL_MAGIC; i++)
		{
			listmagic = listlevelproto->m_magic[i];
			if (listmagic == NULL)
				continue ;

			// 입력 스킬 루프
			for (j = 0; j < MAX_SKILL_MAGIC; j++)
			{
				inmagic = inlevelproto->m_magic[j];
				if (inmagic == NULL)
					continue ;

				// 비교
				if (inmagic->m_index != listmagic->m_index)
					continue ;
#ifdef EVENT_HALLOWEEN_2007
				if( inmagic->m_index == 518 || inmagic->m_index == 519 )
					continue;
#endif // EVENT_HALLOWEEN_2007
				if (inlevelproto->m_magicLevel[j] < listlevelproto->m_magicLevel[i])
					return false;
			}
		}
	}

	return true;
}

// 060227 : bs : 절대시간 버프 추가
void CAssistList::DelDuplicate(const CSkillProto* proto, int level, bool bSend, CCharacter* ch, bool bNoCancelType)
{
	const CSkillLevelProto* inlevelproto = proto->Level(level);
	if (inlevelproto == NULL)
		return ;

	CAssistData* data;
	CAssistData* dataNext;
	const CSkillLevelProto* listlevelproto;
	const CMagicProto* inmagic;
	const CMagicProto* listmagic;
	int i, j;
	bool bDelete;

	// 리스트 루프
	dataNext = m_head;
	while ((data = dataNext))
	{
		dataNext = data->m_next;

		listlevelproto = data->m_proto->Level(data->m_level);
		if (listlevelproto == NULL)
			continue ;

		bDelete = false;

		// 리스트의 스킬 루프
		for (i = 0; i < MAX_SKILL_MAGIC && !bDelete; i++)
		{
			listmagic = listlevelproto->m_magic[i];
			if (listmagic == NULL)
				continue ;

			// 입력 스킬 루프
			for (j = 0; j < MAX_SKILL_MAGIC; j++)
			{
				inmagic = inlevelproto->m_magic[j];
				if (inmagic == NULL)
					continue ;

				// 비교
				if (inmagic->m_index != listmagic->m_index)
					continue ;
				if (listlevelproto->m_magicLevel[i] <= inlevelproto->m_magicLevel[j])
				{
					bDelete = true;
					break;
				}
			}
		}

		if (!bDelete)
			continue ;

		DelAssist(data, bSend, ch, bNoCancelType);
	}
}

bool CAssistList::DecreaseTime(CCharacter* ch, int* changestate)
{
	bool ret = false;

	CAssistData* data;
	CAssistData* dataNext = m_head;

	const CSkillProto* sp;
	const CSkillLevelProto* slp;
	const CMagicProto* mp;
	const CMagicLevelProto* mlp;
	bool bCancel;
	int i;

	while ((data = dataNext))
	{
		dataNext = data->m_next;

		bool bSkip = false;
		// 증폭제는 시간 감소 없음
		switch (data->m_index)
		{
		case 882:
		case 883:
		case 884:
		case 885:
#ifdef NEW_CASHITEM_USA_2008
		case 2875:
		case 2855:
#endif // NEW_CASHITEM_USA_2008
			bSkip = true;
			break;
		}

#ifdef MONSTER_COMBO
		if(data->m_proto->m_flag & SF_COMBO)		// 시간감소없는스킬
			continue;
#endif // MONSTER_COMBO

#ifdef EVENT_PHOENIX   // 피닉스 이벤트    yhj
		// 피닉스 버프일 경우
		if ( data->m_proto->m_index == 516 )
		{
			// 레벨 100 까지만 사용
			if( ch->m_level < 100 )		
				bSkip = true;
		}
#endif // EVENT_PHOENIX

		if (bSkip)
			continue ;

		// 매번 적용되는 상태 검사
		sp = data->m_proto;
		if (!sp)
			continue;
		slp = sp->Level(data->m_level);
		if (!slp)
			continue;
		i = 0;
		bCancel = false;
		while (i < MAX_SKILL_MAGIC && !bCancel)
		{
			mp = slp->m_magic[i];
			if (mp)
			{
				mlp = mp->Level(slp->m_magicLevel[i]);
				if (mlp)
				{
					int nPowerValue = mlp->m_nPowerValue * CalcSkillParam(ch, ch, SPARAM_NONE, mp->m_ptp) / 100;

					switch (slp->m_magic[i]->m_type)
					{
					case MT_ASSIST:
						{
							
							switch (slp->m_magic[i]->m_subtype)				
							{
							// Dot Damage 죽지는 않는 시간당 데미지
							case MST_ASSIST_POISON: 
							case MST_ASSIST_BLOOD:
							case MST_ASSIST_DISEASE:
							case MST_ASSIST_CURSE:
#ifdef NIGHTSHADOW_SKILL
							case MST_ASSIST_HP_DOT:
#endif // NIGHTSHADOW_SKILL
								{
									if (ch->m_hp > 1)
									{
										switch (mp->m_damagetype)
										{
										case MDT_RATE:
											{
												// yhj  090305   유저가 디버프를 걸었을 경우 시간당 데미지를 1/5 로..
												switch( data->m_spellerType )
												{
												case MSG_CHAR_PC:
													ch->m_hp -= (int)ch->m_maxHP * ( (nPowerValue / SKILL_RATE_UNIT / 5) ) ;
													break;

												// yhj  090305   유저를 제외한 것은 디버프를 걸었을 경우 초당 데미지 그대로임..
												case MSG_CHAR_NPC:
												case MSG_CHAR_PET:
												case MSG_CHAR_ELEMENTAL:
												case MSG_CHAR_APET:
												default:
													ch->m_hp -= (int)ch->m_maxHP * nPowerValue / SKILL_RATE_UNIT;
													break;
												}

											}break;
											
										default:
											{
												switch( data->m_spellerType )
												{
												case MSG_CHAR_PC:
													ch->m_hp -=  (int)( nPowerValue/5 );
													break;

												// yhj  090305   유저를 제외한 것은 디버프를 걸었을 경우 초당 데미지 그대로임..
												case MSG_CHAR_NPC:
												case MSG_CHAR_PET:
												case MSG_CHAR_ELEMENTAL:
												case MSG_CHAR_APET:
												default:
													ch->m_hp -=  nPowerValue;
													break;
												}
											}											
											break;
										}

										if (ch->m_hp <= 0)
											ch->m_hp = 1;

										switch (slp->m_magic[i]->m_subtype)	
										{
										case MST_ASSIST_POISON:		*changestate |= AST_POISON;		break;
										case MST_ASSIST_BLOOD:		*changestate |= AST_BLOOD;		break;
										case MST_ASSIST_DISEASE:	*changestate |= AST_DISEASE;	break;
										case MST_ASSIST_CURSE:		*changestate |= AST_CURSE;		break;
#ifdef NIGHTSHADOW_SKILL
										case MST_ASSIST_HP_DOT:		*changestate |= AST_HP_DOT;		break;
#endif // NIGHTSHADOW_SKILL
										default: break;
										}										
									}
								}break;
/*
							case MST_ASSIST_POISON:
								if (ch->m_hp > 1)
								{
									switch (mp->m_damagetype)
									{
									case MDT_RATE:
										ch->m_hp -= (int)ch->m_maxHP * nPowerValue / SKILL_RATE_UNIT;
										break;
									default:
										ch->m_hp -=  nPowerValue;
										break;
									}
									if (ch->m_hp <= 0)
										ch->m_hp = 1;
									*changestate |= AST_POISON;
								}
								break;

							case MST_ASSIST_BLOOD:
								if (ch->m_hp > 1)
								{
									switch (mp->m_damagetype)
									{
									case MDT_RATE:
										ch->m_hp -= (int)ch->m_maxHP * nPowerValue / SKILL_RATE_UNIT;
										break;
									default:
										ch->m_hp -=  nPowerValue;
										break;
									}
									if (ch->m_hp <= 0)
										ch->m_hp = 1;
									*changestate |= AST_BLOOD;
								}
								break;
*/
							case MST_ASSIST_HP:
								switch (mp->m_damagetype)
								{
								case MDT_RATE:
									ch->m_hp += (int)ch->m_maxHP * nPowerValue / SKILL_RATE_UNIT;
									break;
								default:
									ch->m_hp +=  nPowerValue;
									break;
								}
								if (ch->m_hp > ch->m_maxHP)
									ch->m_hp = ch->m_maxHP;
								if (ch->m_hp <= 0)
									ch->m_hp = 1;
								*changestate |= AST_HP;
								break;

							case MST_ASSIST_MP:
								switch (mp->m_damagetype)
								{
								case MDT_RATE:
									ch->m_mp += (int)ch->m_maxMP * nPowerValue / SKILL_RATE_UNIT;
									break;
								default:
									ch->m_mp +=  nPowerValue;
									break;
								}
								if (ch->m_mp > ch->m_maxMP)
									ch->m_mp = ch->m_maxMP;
								if (ch->m_mp < 0)
									ch->m_mp = 0;
								*changestate |= AST_MP;
								break;

							case MST_ASSIST_HP_CANCEL:
								switch (mp->m_damagetype)
								{
								case MDT_RATE:
									ch->m_hp += (int)ch->m_maxHP * nPowerValue / SKILL_RATE_UNIT;
									break;
								default:
									ch->m_hp +=  nPowerValue;
									break;
								}
								if (ch->m_hp > ch->m_maxHP)
								{
									ch->m_hp = ch->m_maxHP;
									bCancel = true;
								}
								if (ch->m_hp <= 0)
								{
									ch->m_hp = 1;
									bCancel = true;
								}
								*changestate |= AST_HP_CANCEL;
								break;

							case MST_ASSIST_MP_CANCEL:
								switch (mp->m_damagetype)
								{
								case MDT_RATE:
									ch->m_mp += (int)ch->m_maxMP * nPowerValue / SKILL_RATE_UNIT;
									break;
								default:
									ch->m_mp +=  nPowerValue;
									break;
								}
								if (ch->m_mp > ch->m_maxMP)
								{
									ch->m_mp = ch->m_maxMP;
									bCancel = true;
								}
								if (ch->m_mp < 0)
								{
									ch->m_mp = 0;
									bCancel = true;
								}
								*changestate |= AST_MP_CANCEL;
								break;
							}
							break;
						}
					default:
						break;

					}
				}
			}
			i++;
		}

		data->m_remain -= PULSE_ASSIST_CHECK;
		if (data->m_remain <= 0 || bCancel)
		{
			// 경험치 부활 숙련도 주문서일 경우 버프를 삭제하지 않는다.
#ifdef ADULT_SERVER_NEWITEM
			if( data->m_index == 844 
				|| data->m_index == 845 
				|| data->m_index == 846		// 부활 주문서
#ifdef LIMIT_ITEM_M35				
				|| data->m_index == 2667	// 초보자용 부활 주문서
#endif // LIMIT_ITEM_M35
				|| data->m_index == 2371
#ifdef AUTO_POTION
				|| data->m_index == ONE_PERIOD_ITEM
				|| data->m_index == SEVEN_PERIOD_ITEM
				|| data->m_index == THIRTY_PERIOD_ITEM
				|| data->m_index == 2610
#endif // AUTO_POTION			
#ifdef EVENT_PHOENIX // 피닉스 부활 아이템
				|| data->m_index == 3218
#endif //EVENT_PHOENIX
				)
#else
			if( data->m_index == 844 
				|| data->m_index == 845 
				|| data->m_index == 846		// 부활 주문서
#ifdef LIMIT_ITEM_M35				
				|| data->m_index == 2667	// 초보자용 부활 주문서
#endif // LIMIT_ITEM_M35
#ifdef AUTO_POTION
				|| data->m_index == ONE_PERIOD_ITEM
				|| data->m_index == SEVEN_PERIOD_ITEM
				|| data->m_index == THIRTY_PERIOD_ITEM
				|| data->m_index == 2610
#endif // AUTO_POTION				
#ifdef EVENT_PHOENIX // 피닉스 부활 아이템
				|| data->m_index == 3218
#endif //EVENT_PHOENIX
				)
#endif // ADULT_SERVER_NEWITEM
			{
				data->m_remain = 0;
			}
			else
			{
#ifdef EVENT_XMAS_2007
				// 시간에 의한 크리스마스 스킬 제거 ( SkillIndex : 490 )
				// NPC를 소환 한다
				int old_skillIndex = data->m_proto->m_index;
				if( old_skillIndex == 490 )
				{
					CNPC* pBoxNpc = gserver.m_npcProtoList.Create( 483/*크리스마스 선물 상자*/, NULL );
					if ( pBoxNpc )
					{
						GET_X(pBoxNpc) = GET_X(ch) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
						GET_Z(pBoxNpc) = GET_Z(ch) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
						GET_YLAYER(pBoxNpc) = GET_YLAYER(ch);
						GET_R(pBoxNpc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;

						if (ch->m_pArea->GetAttr(GET_YLAYER(pBoxNpc), GET_X(pBoxNpc), GET_Z(pBoxNpc)) == MAPATT_FIELD)
						{
							pBoxNpc->m_regenX = GET_X(pBoxNpc);
							pBoxNpc->m_regenY = GET_YLAYER(pBoxNpc);
							pBoxNpc->m_regenZ = GET_Z(pBoxNpc);

							pBoxNpc->m_regenTimeXmas2007 = gserver.m_pulse;

							int cx, cz;
							ch->m_pArea->AddNPC(pBoxNpc);
							ch->m_pArea->PointToCellNum(GET_X(pBoxNpc), GET_Z(pBoxNpc), &cx, &cz);
							ch->m_pArea->CharToCell(pBoxNpc, GET_YLAYER(pBoxNpc), cx, cz);
						
							CNetMsg appearNPCMsg;
							AppearMsg(appearNPCMsg, pBoxNpc, true);
							ch->m_pArea->SendToCell(appearNPCMsg, GET_YLAYER(pBoxNpc), cx, cz);

							GAMELOG << init(" EVENT XMAS XMAS BOX REGEN " ) << ch->m_name << end;	// 스킬 적용 실패
						}
					}
				}
#endif //EVENT_XMAS_2007

#ifdef NIGHTSHADOW_SKILL
				// 혼란이나 테이밍 중인 몬스터이면 시간이 지나면 풀어준다.
				if (IS_NPC(ch))
				{
					if ( mp && mp->m_type == MT_ASSIST)
					{
						if ( mp->m_subtype == MST_ASSIST_CONFUSED )
						{
							CNPC* pNPC = TO_NPC(ch);
							if (pNPC)
							{
								pNPC->reSet_MobFlag( STATE_MONSTER_CONFUSION );
								DelAttackList(pNPC);		
							}
						}

						if ( mp->m_subtype == MST_ASSIST_TAMING )
						{
							CNPC* pNPC = TO_NPC(ch);
							if (pNPC)
							{
								pNPC->reSet_MobFlag( STATE_MONSTER_TAMING );
								DelAttackList(pNPC);		
								
								// 주인을 찾는다.
								CPC* owner = pNPC->GetOwner();
								if(owner != NULL) 
								{
									// 주인이 있는지 확인, 있으면 테이밍 중을 해제
									CPC* pOwnerCheck = gserver.m_playerList.Find(owner->m_index);
									if (pOwnerCheck != NULL)
									{
										pOwnerCheck->m_bTaming_npc = false;
									}
									// npc의 주인도 지워 줌
									pNPC->SetOwner(NULL);
								}								
							}
							
						}
					}
				}
#endif // NIGHTSHADOW_SKILL				
				DelAssist(data, true, ch, true);
				ret = true;
#ifdef EVENT_XMAS_2007
				if( old_skillIndex == 490 )
				{
					CSkill *pSkill = gserver.m_skillProtoList.Create( old_skillIndex , 1 );
					if( pSkill )
					{
						bool bApply = false ;
						ApplySkill( ch, ch, pSkill, -1 , bApply );
						if( !bApply )
						{
							GAMELOG << init(" EVENT XMAS SKILL FAILED " ) << ch->m_name << end;	// 스킬 적용 실패
						}
					}
				}
#endif //EVENT_XMAS_2007
#ifdef RAID_MONSTER_SKILL
				// 자폭 버프가 사라지면 자폭을 실시한다.
				if ( IS_NPC(ch) )
				{
					if ( mp && mp->m_type == MT_ASSIST)
					{
						if ( mp->m_subtype == MST_ASSIST_EXPLOSION )
						{
							CNPC* pNPC = TO_NPC(ch);
							if (pNPC)
							{
								pNPC->m_hp = 0;
								if (DEAD(pNPC))
								{
									pNPC->SendDisappearAllInCell(true);

									// 어택리스트에서 삭제
									DelAttackList(pNPC);

									// npc 삭제
									ch->m_pArea->CharFromCell(pNPC, true);
									ch->m_pArea->DelNPC(pNPC);
								}
							}
						}
					}
				}
#endif // RAID_MONSTER_SKILL	
			}
		}
	}

	return ret;
}

void CAssistList::Apply(CCharacter* ch, ASSISTVALUE* addition, ASSISTVALUE* rate, int* state)
{
	CAssistData* p;
	const CSkillProto* sp;
	const CSkillLevelProto* slp;
	const CMagicProto* mp;
	const CMagicLevelProto* mlp;
	int i;

	for (p = m_head; p; p = p->m_next)
	{
		sp = p->m_proto;
		if (sp == NULL)
			continue;
		slp = sp->Level(p->m_level);
		if (slp == NULL)
			continue;

		// TODO : 하드코딩용

		bool bSkip = false;

		// 아이템 인덱스 하드 코딩
		switch (p->m_index)
		{
		case 2388:		addition->hcExpPlus_2388 += 100;	bSkip = true;	break;	// Poem book
		case 2389:		addition->hcExpPlus_2389 += 50;		bSkip = true;	break;	// Blessing of Mother( Blue )
		case 2390:		addition->hcSPPlus_2390 += 100;		bSkip = true;	break;	// Blessing of Mother( Yellow )
		case 2391:		addition->hcDropPlus_2391 = true;	bSkip = true;	break;	// Blessing of Mother( Red )
		case 671:		addition->hcDeathExpPlus = 1;		bSkip = true;	break ;		// 하급 경험의 결정
		case 672:		addition->hcDeathExpPlus = 2;		bSkip = true;	break ;		// 중급 경험의 결정
		case 673:		addition->hcDeathExpPlus = 3;		bSkip = true;	break ;		// 상급 경험의 결정
		case 674:		addition->hcDeathSPPlus = true;		bSkip = true;	break ;		// 노력의 결정
		case 508:		addition->hcExpPlus = true;			bSkip = true;	break ;		// 떡국
		case 509:		addition->hcSPPlus = true;			bSkip = true;	break ;		// 떡만두국
		case 836:		addition->hcExpPlus_836 = true;		bSkip = true;	break ;		// 수박
		case 837:		addition->hcSPPlus_837 = true;		bSkip = true;	break ;		// 참외
		case 838:		addition->hcDropPlus_838 = true;	bSkip = true;	break ;		// 자두
		case 884:		addition->hcSepDrop = true;			bSkip = true;	break ;
		case 882:		addition->hcSepExp = true;			bSkip = true;	break ;
		case 883:		addition->hcSepSP = true;			bSkip = true;	break ;
		case 885:		addition->hcSepNas = true;			bSkip = true;	break ;
		case 971:		// 경험의스크롤
		case 2499:		// 일본 경험의스크롤 복사본
						addition->hcScrollExp = true;		bSkip = true;	break ;		
		case 972:		addition->hcScrollDrop = true;		bSkip = true;	break ;		// 행운의스크롤
		case 973:		addition->hcScrollSP = true;		bSkip = true;	break ;		// 노력의스크롤

#ifdef ENABLE_WAR_CASTLE
		case 792:		addition->hcAttackTower = true;		bSkip = true;	break ;		// 석상 공격시 대미지 2배
#endif

		case 1628:		addition->hcSPPlusPer100 += 50;		bSkip = true;	break ;		// 달콤한 참외
		case 1629:		addition->hcDropPlusPer100 += 100;	bSkip = true;	break ;		// 새콤한 자두
		case 1627:		
		case 2494:		// 일본 잘익은 수박 복사본
						addition->hcExpPlusPer100 += 20;	bSkip = true;	break ;		// 잘익은 수박
		case 1630:		addition->hcExpPlusPer100 += 20;	bSkip = true;	break ;		// 축복의 물병
		case 1975:		// 케이크
		case 2495:		// 일본 케이크 복사본
						addition->hcExpPlus_1975 = 30;		bSkip = true;	break ;		// 케이크 : 2006 크리스마스
		case 1976:		// 눈사람
		case 2498:		// 일본 눈사람 복사본
						addition->hcExpPlus_1976 = 100;		bSkip = true;	break ;		// 눈사람 : 2006 크리스마스
		case 2139:		addition->hcSPPlusPer100 += 15;		bSkip = true;	break ;		// 훈련주문서
		case 2140:		addition->hcSPPlusPer100 += 50;		bSkip = true;	break ;		// 플래티넘 훈련주문서
		case 2141:		addition->hcDropPlus_2141 = true;	bSkip = true;	break ;		// 행운 주문서	

#ifdef PLATINUM_SKILL_POTION_ITEM
		case 2453:		addition->hcSPPlusPer100 += 200;	bSkip = true; break;		// 플래티늄 숙련 묘약
#endif // PLATINUM_SKILL_POTION_ITEM
#ifdef LIMIT_ITEM_M35
		case 2659:		addition->hcSPPlusPer100 += 200;	bSkip = true; break;	// 초보자용 플래티늄 숙련의 묘약			
#endif // LIMIT_ITEM_M35
#ifdef SKILL_POTION_ITEM
		case 2452:		addition->hcSPPlusPer100 += 100;	bSkip = true; break;		// 숙련의 묘약
#endif // SKILL_POTION_ITEM

#ifdef PET_EXP_UP
		case 2358:		addition->hcCashPetExpUp_2358 = true; bSkip = true; break;		// 펫 경험의 묘약
		case 2359:		addition->hcCashPetExpUp_2359 = true; bSkip = true; break;		// 플래티늄 펫 경험의 묘약
#endif // PEt_EXP_UP
#ifdef MP_STEEL_POTION
		case 2356:		
		case 2841:
						addition->hcMPSteelPotion = true; bSkip = true; break;			// 일반 공격시 30%의 Max MP 중 10% 감소 되고 나의 MP 증가
#endif // MP_STEEL_POTION
		
#ifdef HP_STEEL_POTION
		case 2357:		
		case 2842:
						addition->hcHPSteelPotion = true; bSkip = true; break;			// 일반 공격시 30%의 Max HP 중 5% 감소 되고 나의 HP 증가
#endif // HP_STEEL_POTION

//#ifdef EVENT_INDEPENDENCE_DAY_2007_USA
		case 2410:
		case 2354:		addition->hcExpSPPlus_2354 = true; bSkip = true; break;			// 줄무늬 성조기(브라질 국기): 50% EXP + 50% SP
//#endif // EVENT_INDEPENDENCE_DAY_2007_USA

#ifdef NEW_CASHITEM_USA_2008
		case 2853:		addition->hcAttackBerserker = true; bSkip = true; break;		// 10% 확률로 2배 데미지
		case 2870:		addition->hcExplimitPlus = 50; bSkip = true; break;			// 50% 경험치 제한 50% 해제
		case 2855:		addition->hcPlatinumDrop = true; bSkip = true; break;		// 드롭율 20배 증가
		case 2856:		addition->hcPlatinumScroll = true; bSkip = true; break;
#endif // NEW_CASHITEM_USA_2008
		}

		// 060227 : bs : 스킬 인덱스 하드 코딩
		switch (p->m_proto->m_index)
		{
		case 348:
		case 349:		mp = slp->m_magic[0];											// 060227 : bs : 유료아이템 경험치 상승
						if (mp)
						{
							mlp = mp->Level(slp->m_magicLevel[0]);
							if (mlp)
							{
								if (rate->hcCashExpUp < mlp->m_nPowerValue)
									rate->hcCashExpUp = mlp->m_nPowerValue;
							}
						}
						bSkip = true;
						break;

		case 350:		mp = slp->m_magic[0];											// 060227 : bs : 유료아이템 HP 확장
						if (mp)
						{
							mlp = mp->Level(slp->m_magicLevel[0]);
							if (mlp)
							{
								if (rate->hcCashMaxHPUp < mlp->m_nPowerValue)
									rate->hcCashMaxHPUp = mlp->m_nPowerValue;
							}
						}
						bSkip = true;
						break;

		case 351:		mp = slp->m_magic[0];											// 060227 : bs : 유료아이템 MP 확장
						if (mp)
						{
							mlp = mp->Level(slp->m_magicLevel[0]);
							if (mlp)
							{
								if (rate->hcCashMaxMPUp < mlp->m_nPowerValue)
									rate->hcCashMaxMPUp = mlp->m_nPowerValue;
							}
						}
						bSkip = true;
						break;

		case 352:		for (i = 0; i < 2; i++)											// 060227 : bs : 유료아이템 HP/MP 확장
						{
							mp = slp->m_magic[i];
							if (mp)
							{
								mlp = mp->Level(slp->m_magicLevel[i]);
								if (mlp)
								{
									switch (mp->m_subtype)
									{
									case MST_STAT_MAXHP:
										if (rate->hcCashMaxHPUp < mlp->m_nPowerValue)
											rate->hcCashMaxHPUp = mlp->m_nPowerValue;
										break;
									case MST_STAT_MAXMP:
										if (rate->hcCashMaxMPUp < mlp->m_nPowerValue)
											rate->hcCashMaxMPUp = mlp->m_nPowerValue;
										break;
									default:
										break;
									}
								}
							}
						}
						bSkip = true;
						break;

		case 354:		addition->hcExpPlus_S354 = true;								// 경험치 포션 : 스킬 354, 1.5배 상승
						bSkip = true;
						break;

		case 355:		addition->hcExpPlus_S355 = true;								// 추천 서버 포션 경험치 상승
						bSkip = true;
						break;

		case 356:		addition->hcSPPlus_S356 = true;									// 추천 서버 포션 SP 상승
						bSkip = true;
						break;

		case 360:		addition->hcDropPlus_S360 = true;								// 추천 서버 포션 드롭율 상승
						bSkip = true;
						break;

		case 379:		mp = slp->m_magic[0];											// 태국 경험치 상승 포션
						if (mp)
						{
							mlp = mp->Level(slp->m_magicLevel[0]);
							if (mlp)
							{
								addition->hcExpPlusPer100 += mlp->m_nPowerValue;				// 태국 경험치 상승 포션
							}
						}
						bSkip = true;
						break;

		case 398:		mp = slp->m_magic[0];											// (유료)경험치 포션 : 1840, 1841, 1842
						if (mp)
						{
							mlp = mp->Level(slp->m_magicLevel[0]);
							if (mlp)
							{
								addition->hcExpPlus_398 = mlp->m_nPowerValue;
							}
						}
						bSkip = true;
						break;

		case 418:		mp = slp->m_magic[0];							// 럭키 소울 닉스 
						if( mp )
						{
							mlp = mp->Level(slp->m_magicLevel[0]);
							if( mlp )
							{
								addition->HitRate_2033 = mlp->m_nPowerValue;
							}
						}
						bSkip = true;
						break;
		case 419:		mp = slp->m_magic[0];		//럭키 다지 피어
						if( mp )
						{
							mlp = mp->Level(slp->m_magicLevel[0]);
							if( mlp )
							{
								addition->Avoid_2034 = mlp->m_nPowerValue;
							}
						}
						bSkip = true;
						break;
#if defined (EVENT_WHITEDAY_2007) || defined (EVENT_OPEN_ADULT_SERVER)	|| defined (EVENT_MAGIC_CARD)	//STAT 부분 하드코딩 적용 
		case 424 :		//힘의 러브매직 
			COption::ApplyOptionValue( (CPC*)ch, OPTION_STR_UP, 20, NULL);	bSkip = true;	break;
		case 425 :		//민첩의 러브매직
			COption::ApplyOptionValue( (CPC*)ch, OPTION_DEX_UP, 20, NULL);	bSkip = true; break;
		case 426 :		//지해의 러브매직
			COption::ApplyOptionValue( (CPC*)ch, OPTION_INT_UP, 20, NULL);	bSkip = true; break;
		case 427 :		//체질의 러브매직
			COption::ApplyOptionValue( (CPC*)ch, OPTION_CON_UP, 50, NULL);	bSkip = true; break;
		case 428 :
			addition->bRorainOfLoveMagic = true; bSkip = true; break;
#endif // defined (EVENT_WHITEDAY_2007) || defined (EVENT_OPEN_ADULT_SERVER) || defined (EVENT_MAGIC_CARD)

		case 433:	// 건강의 물약
			ch->cooltime_2142 = gserver.m_gameTime + 2*60*60;		
			break;
		case 434:	// 지력의 물약
			ch->cooltime_2143 = gserver.m_gameTime + 2*60*60; 			
			break;
		case 465:
#ifdef COMPETITION_ITEM			
			ch->m_cooltime_Competition = gserver.m_gameTime + 30 * 60;
#endif // COMPETITION_ITEM
			break;
#ifdef EVENT_HALLOWEEN_2007  // 감소되는 부분 따로 처리 
		case 470:	// 할로윈 이벤트 최대 HP 변화
			{
				mp = slp->m_magic[0]; // 최대 HP 감소
				if (mp)
				{
					mlp = mp->Level(slp->m_magicLevel[0]);
					if (mlp)
					{
						rate->hcEventHalloweenMaxHP = mlp->m_nPowerValue;
					}
				}

				mp = slp->m_magic[1]; // 이동속도 감소
				if (mp)
				{
					mlp = mp->Level(slp->m_magicLevel[1]);
					if (mlp)
					{
						rate->hcEventHalloweenSpeed = mlp->m_nPowerValue;
					}
				}
				bSkip = true;
			}
			break;
		case 471:
			{
				mp = slp->m_magic[0]; // 이동속도 감소
				if (mp)
				{
					mlp = mp->Level(slp->m_magicLevel[0]);
					if (mlp)
					{
						rate->hcEventHalloweenSpeed = mlp->m_nPowerValue;
					}
				}
				bSkip = true;
			}break;
#endif // EVENT_HALLOWEEN_2007
#ifdef EVENT_XMAS_2007
		case 485: case 486: case 487: case 488:
			{
				mp = slp->m_magic[0]; 
				if (mp)
				{
					mlp = mp->Level(slp->m_magicLevel[0]);
					if (mlp)
					{
						// 드랍률 증가
						rate->hcDropPlus_Xmas2007 = mlp->m_nPowerValue;
					}
				}
				bSkip = true;
			}
			break;
#endif //EVENT_XMAS_2007
		case 550:
			{
				mp = slp->m_magic[0];											// 060227 : bs : 유료아이템 경험치 상승
				if (mp)
				{
					mlp = mp->Level(slp->m_magicLevel[0]);
					if (mlp)
					{
						if (addition->hcCashExpUp < mlp->m_nPowerValue)
							addition->hcCashExpUp = mlp->m_nPowerValue;
					}
				}
				bSkip = true;
				break;
			}
			break;
		}

		if (bSkip == true)
			continue ;

		// --- TODO : 하드코딩용

		for (i = 0; i < MAX_SKILL_MAGIC; i++)
		{
			if (!p->m_bHit[i])
				continue ;

			mp = slp->m_magic[i];
			if (mp == NULL)
				continue ;
			mlp = mp->Level(slp->m_magicLevel[i]);
			if (mlp == NULL)
				continue ;

			switch (mp->m_type)
			{
			case MT_STAT:
				switch (mp->m_subtype)
				{
				case MST_STAT_ATTACK:			APPVAL(attack);					break;
				case MST_STAT_DEFENSE:			APPVAL(defense);				break;
				case MST_STAT_MAGIC:			APPVAL(magic);					break;
				case MST_STAT_RESIST:			APPVAL(resist);					break;
				case MST_STAT_HITRATE:			APPVAL(hitrate);				break;
				case MST_STAT_AVOID:			APPVAL(avoid);					break;
				case MST_STAT_CRITICAL:			APPVAL(critical);				break;
				case MST_STAT_ATTACKSPD:		APPVAL(attackspeed);			break;
				case MST_STAT_MAGICSPD:			APPVAL(magicspeed);				break;
				case MST_STAT_MOVESPD:			APPVAL(movespeed);				break;
				case MST_STAT_RECOVERHP:		APPVAL(recoverhp);				break;
				case MST_STAT_RECOVERMP:		APPVAL(recovermp);				break;
				case MST_STAT_MAXHP:			APPVAL(maxhp);					break;
				case MST_STAT_MAXMP:			APPVAL(maxmp);					break;
				case MST_STAT_DEADLY:			APPVAL(deadly);					break;
				case MST_STAT_MAGICHITRATE:		APPVAL(magichitrate);			break;
				case MST_STAT_MAGICAVOID:		APPVAL(magicavoid);				break;
				case MST_STAT_ATTACKDIST:		APPVAL(attackdist);				break;
				case MST_STAT_ATTACK_MELEE:		APPVAL(attack_dam_melee);		break;
				case MST_STAT_ATTACK_RANGE:		APPVAL(attack_dam_range);		break;
				case MST_STAT_HITRATE_SKILL:	APPVAL(hitrate_skill);			break;
				case MST_STAT_ATTACK_80:		APPVAL(attack80);				break;
				case MST_STAT_MAXHP_450:		APPVAL(maxhp450);				break;

					/////////////////////////////////////////////
					// BANGWALL : 2005-07-18 오후 2:22:33
					// Comment : skillspd
				case MST_STAT_SKILLSPD:			APPVAL(skillspd);				break;	
				case MST_STAT_VALOR:			
					{
						if( IS_PC(ch) )
						{
							CPC *pc = TO_PC(ch);
							if( pc->IsParty() && pc->m_party->GetNearPartyMemberCount(pc) > 1)
							{
								addition->attack += (mlp->m_nPowerValue * CalcSkillParam(ch, ch, SPARAM_NONE, mp->m_ptp) / 100) * pc->m_party->GetNearPartyMemberCount(pc) / 10;
							}
						}
					}
					break;
				case MST_STAT_STATPALL:		
					APPVAL(statpall);				break;
				// << 071211 kjban add
				case MST_STAT_ATTACK_PER:
					{
						switch (mp->m_damagetype)
						{ 
						case MDT_ADDITION:
							addition->attack += ch->m_eqMelee * mlp->m_nPowerValue / 100; 
							addition->magic += ch->m_eqMagic * mlp->m_nPowerValue / 100; 
							break;
						case MDT_RATE:
							rate->attack += ch->m_eqMelee * mlp->m_nPowerValue / 100; 
							rate->magic += ch->m_eqMagic * mlp->m_nPowerValue / 100; 
							break; 
						}
					}
					break;

				case MST_STAT_DEFENSE_PER:		
					{
						switch (mp->m_damagetype)
						{ 
						case MDT_ADDITION:
							addition->defense += ch->m_eqDefense * mlp->m_nPowerValue / 100; 
							break;
						case MDT_RATE:
							rate->defense += ch->m_eqDefense * mlp->m_nPowerValue / 100; 
							break; 
						}
					}				
					break;

				case MST_STAT_STATPALL_PER:	
					{
					switch (mp->m_damagetype)
						{ 
						case MDT_ADDITION:
							addition->statpall_per += mlp->m_nPowerValue; 
							break;
						case MDT_RATE:
							rate->statpall_per += mlp->m_nPowerValue; 
							break; 
						}
					}
				//  [5/15/2008 KwonYongDae]
				case MST_STAT_STR:
					{
						COption::ApplyOptionValue( (CPC*)ch, OPTION_STR_UP, mlp->m_nPowerValue, NULL);	
					}break;
				case MST_STAT_DEX:
					{
						COption::ApplyOptionValue( (CPC*)ch, OPTION_DEX_UP, mlp->m_nPowerValue, NULL);	
					}break;
				case MST_STAT_INT:
					{
						COption::ApplyOptionValue( (CPC*)ch, OPTION_INT_UP, mlp->m_nPowerValue, NULL);	
					}break;
				case MST_STAT_CON:
					{
						COption::ApplyOptionValue( (CPC*)ch, OPTION_CON_UP, mlp->m_nPowerValue, NULL);	
					}break;
				}
				break;
				// >>
			case MT_ATTRIBUTE:
				switch (mp->m_subtype)
				{
				case AT_FIRE:		APPVAL(attfire);	break;
				case AT_WATER:		APPVAL(attwater);	break;
				case AT_EARTH:		APPVAL(attearth);	break;
				case AT_WIND:		APPVAL(attwind);	break;
				case AT_DARK:		APPVAL(attdark);	break;
				case AT_LIGHT:		APPVAL(attlight);	break;

				case AT_ALL:		APPVAL(attfire);
									APPVAL(attwater);
									APPVAL(attearth);
									APPVAL(attwind);
									APPVAL(attdark);
									APPVAL(attlight);	break;
				}
				break;

			case MT_ASSIST:
				switch (mp->m_subtype)
				{
				case MST_ASSIST_POISON:			*state |= AST_POISON;			break;
				case MST_ASSIST_HOLD:			*state |= AST_HOLD;				break;
				case MST_ASSIST_CONFUSION:		*state |= AST_CONFUSION;		break;
				case MST_ASSIST_STONE:			
#ifdef RAID_MONSTER_SKIP_STONE		// 레이드 몹 스톤 제외	
					if( IS_NPC(ch) )
					{
						CNPC * pNpc = TO_NPC(ch);
						if( pNpc != NULL )
						{
							if( pNpc->m_proto->CheckFlag(NPC_RAID) )
							{
								break;
							}
						}
					}
#endif	// RAID_MONSTER_SKIP_STONE
					*state |= AST_STONE;			break;

				case MST_ASSIST_SILENT:			*state |= AST_SILENT;			break;
				case MST_ASSIST_BLOOD:			*state |= AST_BLOOD;			break;
				case MST_ASSIST_BLIND:			*state |= AST_BLIND;			break;
				case MST_ASSIST_STURN:			
#ifdef RAID_MONSTER_SKIP_STURN		// 레이드 몹 스턴 제외
					if( IS_NPC(ch) )
					{
						CNPC * pNpc = TO_NPC(ch);
						if( pNpc != NULL )
						{
							if( pNpc->m_proto->CheckFlag(NPC_RAID) )
							{
								break;
							}
						}
					}
#endif	// RAID_MONSTER_SKIP_STURN
					*state |= AST_STURN;			break;
				case MST_ASSIST_SLEEP:			*state |= AST_SLEEP;			break;
				case MST_ASSIST_HP:				*state |= AST_HP;				break;
				case MST_ASSIST_MP:				*state |= AST_MP;				break;
				case MST_ASSIST_MOVESPD:		*state |= AST_MOVESPD;
												APPVAL(movespeed);
												break;
				case MST_ASSIST_DIZZY:			*state |= AST_DIZZY;
												APPVAL(movespeed);
												break;
				case MST_ASSIST_INVISIBLE:		*state |= AST_INVISIBLE;		break;
				case MST_ASSIST_SLOTH:			*state |= AST_SLOTH;
												APPVAL(attackspeed);
												break;
				case MST_ASSIST_FEAR:			*state |= AST_FEAR;
												addition->hcFearType = p->m_spellerType;
												addition->hcFearIndex = p->m_spellerIndex;
												break;
				case MST_ASSIST_FAKEDEATH:		*state |= AST_FAKEDEATH;		break;
				case MST_ASSIST_PERFECTBODY:	*state |= AST_PERFECTBODY;
												APPVAL(defense);
												break;
				case MST_ASSIST_FRENZY:			*state |= AST_FRENZY;
												APPVAL(attack);
												break;
				case MST_ASSIST_DAMAGELINK:		*state |= AST_DAMAGELINK;		break;
				case MST_ASSIST_BERSERK: 
												*state |= AST_BERSERK;
												APPVAL(attackspeed);
												break;
				case MST_ASSIST_DESPAIR:		*state |= AST_DESPAIR;
												APPVAL(despair);
												break;
				case MST_ASSIST_MANASCREEN:		*state |= AST_MANASCREEN;
												APPVAL(manascreen);
												break;
				case MST_ASSIST_BLESS:			*state |= AST_BLESS;
												APPVAL(bless);
												break;
				case MST_ASSIST_SAFEGUARD:			
												*state |= AST_SAFEGUARD;		break;
#ifdef ADULT_SERVER_NEWITEM
				case MST_ASSIST_MANTLE:			
												*state |= AST_MANTLE;			break;
				case MST_ASSIST_GUARD:
												*state |= AST_GUARD;			break;
#endif // ADULT_SERVER_NEWITEM

#ifdef NEW_CASHITEM_USA_2008
				case MST_ASSIST_CHARGEATC:		APPVAL(charge_attack);			break;
				case MST_ASSIST_CHARGEMGC:		APPVAL(charge_magic);			break;
#endif // NEW_CASHITEM_USA_2008

				case MST_ASSIST_DISEASE:		*state |= AST_DISEASE;			break;
				case MST_ASSIST_CURSE:			*state |= AST_CURSE;			break;
#ifdef RAID_MONSTER_SKILL
				case MST_ASSIST_REBIRTH:		*state |= AST_REBIRTH;			break;
				case MST_ASSIST_EXPLOSION:		*state |= AST_EXPLOSION;		break;
#endif // RAID_MONSTER_SKILL
#ifdef NIGHTSHADOW_SKILL
				case MST_ASSIST_HP_DOT:			*state |= AST_HP_DOT;			break;
				case MST_ASSIST_INVERSE_DAMAGE:	*state |= AST_INVERSE_DAMAGE;
												APPVAL(inverse_damage);
												break;
				case MST_ASSIST_FREEZE:			*state |= AST_FREEZE;			break;
				case MST_ASSIST_CONFUSED:		*state |= AST_CONFUSED;	
					if (IS_NPC(ch))
					{
						CNPC* pNPC = TO_NPC(ch);
						if (pNPC)
						{
							pNPC->Set_MobFlag( STATE_MONSTER_CONFUSION );
						}
					}
												break;
				case MST_ASSIST_TAMING:			*state |= AST_TAMING;			
					if (IS_NPC(ch))
					{
						CNPC* pNPC = TO_NPC(ch);
						if (pNPC)
						{
							pNPC->Set_MobFlag( STATE_MONSTER_TAMING );

							// 주인을 찾는다.
							CPC* owner = pNPC->GetOwner();
							if(owner == NULL) 
								break;

							// 주인이 있는지 확인, 있으면 테이밍 중이라고 체크해 줌
							CPC* pOwnerCheck = gserver.m_playerList.Find(owner->m_index);
							if (pOwnerCheck != NULL)
							{
								pOwnerCheck->m_bTaming_npc = true;
							}
						}
					}
												break;
#endif // NIGHTSHADOW_SKILL
				}
				break;

			case MT_OTHER:
				switch (mp->m_subtype)
				{
				case MST_OTHER_REFLEX:			APPVAL(hcReflex);
												addition->hcReflexProb += mlp->m_nHitrateValue;
												break;
				}
				break;

			case MT_REDUCE:
				switch (mp->m_subtype)
				{
				case MST_REDUCE_MELEE:			APPVAL(reduceMelee);
												addition->reduceMeleeProb += mlp->m_nHitrateValue;
												break;
				case MST_REDUCE_RANGE:			APPVAL(reduceRange);
												addition->reduceRangeProb += mlp->m_nHitrateValue;
												break;
				case MST_REDUCE_MAGIC:			APPVAL(reduceMagic);
												addition->reduceMagicProb += mlp->m_nHitrateValue;
												break;
				}
				break;

			case MT_IMMUNE:
				switch (mp->m_subtype)
				{
				case MST_IMMUNE_BLIND:		addition->immune_blind = true;		break;
				}
				break;
			}
		}
	}
}

// 060227 : bs : 절대시간 버프 추가
void CAssistList::DelAssist(CAssistData* d, bool bSend, CCharacter* ch, bool bNoCancelType)
{
	// 060317 : bs : SF_NOCANCEL 검사
	// bNoCancelType 이 false
	if (!bNoCancelType && (d->m_proto->m_flag & SF_NOCANCEL))
		return ;

	// data 제거
	// 헤더포인터 설정
	if (m_head == d)		m_head = m_head->m_next;
	// 테일포인터 설정
	if (m_tail == d)		m_tail = m_tail->m_prev;
	// prev 링크 연결
	if (d->m_prev)			d->m_prev->m_next = d->m_next;
	// next 링크 연결
	if (d->m_next)			d->m_next->m_prev = d->m_prev;
	// 링크 제거
	d->m_prev = NULL;
	d->m_next = NULL;

	bool bIsDamageLink = false;
	const CSkillLevelProto* pSkillLevelProto = d->m_proto->Level(d->m_level);
	if (pSkillLevelProto)
	{
		int i;
		for (i = 0; i < MAX_SKILL_MAGIC; i++)
		{
			if (pSkillLevelProto->m_magic[i])
			{
				if (pSkillLevelProto->m_magic[i]->m_type == MT_ASSIST && pSkillLevelProto->m_magic[i]->m_subtype == MST_ASSIST_DAMAGELINK)
				{
					bIsDamageLink = true;
					break;
				}
#ifdef SAFEGUARD				
				if (pSkillLevelProto->m_magic[i]->m_type == MT_ASSIST
					&& pSkillLevelProto->m_magic[i]->m_subtype == MST_ASSIST_SAFEGUARD)
				{
					CPC * pc = TO_PC(ch);
					if (pc != NULL)
					{
						pc->m_bImmortal = false;
						pc->m_assist.m_state &= ~AST_SAFEGUARD;
					}					
				}
#endif	// SAFEGUARD
#ifdef ADULT_SERVER_NEWITEM
				if (pSkillLevelProto->m_magic[i]->m_type == MT_ASSIST
					&& pSkillLevelProto->m_magic[i]->m_subtype == MST_ASSIST_GUARD)
				{
					CPC * pc = TO_PC(ch);
					if (pc != NULL)
					{
						CElemental* pElemental = NULL;
						CElemental* pElementalNext = pc->m_elementalList;
						while ((pElemental = pElementalNext))
						{
							pElementalNext = pElemental->m_nextElemental;
							if(pElemental->GetElementalType() == ELEMENTAL_GUARD)
								pc->UnsummonElemental(pElemental);
						}
					}					
				}
#endif // ADULT_SERVER_NEWITEM
			}
		}
	}

	if (bIsDamageLink/*(ch->m_assist.m_state & AST_DAMAGELINK) != 0*/)
	{
		CCharacter* plinkSource = ch->m_linkSource;
		CCharacter* plinkTarget = ch->m_linkTarget;
		ch->m_linkSource = NULL;
		ch->m_linkTarget = NULL;
		if (plinkSource)
		{
			plinkSource->m_linkTarget = NULL;
			plinkSource->CancelDamageLink();
		}
		if (plinkTarget)
		{
			plinkTarget->m_linkSource = NULL;
			plinkTarget->CancelDamageLink();
		}
	}

	if( d->m_proto->m_flag & SF_ABSTIME )
		m_abscount--;
	else
		m_count--;

	ch->CalcStatus(bSend);

	if( d->m_index != -1 )
	{
		if( IS_PC(ch) )
		{
			CItemProto* proto = gserver.m_itemProtoList.FindIndex(d->m_index);
			if( proto && proto->m_flag & ITEM_FLAG_CASH )
			{
				GAMELOG << init("CASH_ASSIST_DEL", TO_PC(ch))
						<< proto->m_index << delim
						<< proto->m_name << delim
						<< d->m_index << delim
						<< d->m_remain << delim
						<< d->m_proto->m_index << delim
						<< (d->m_proto->m_flag & SF_ABSTIME) << end;
			}
		}
	}

	if (bSend)
	{
		CNetMsg rmsg;
		AssistDelMsg(rmsg, ch, d->m_index, d->m_proto->m_index);
		ch->m_pArea->SendToCell(rmsg, ch, true);
		CharStatusMsg(rmsg, ch, 0);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	delete d;
}


////////////////
// class CAssist

CAssist::CAssist()
{
	m_ch = NULL;

	m_help.Max(MAX_ASSIST_HELP);
	m_curse.Max(MAX_ASSIST_CURSE);

	m_delaycheck = gserver.m_gameTime;

	memset(&m_avAddition, 0, sizeof(m_avAddition));
	memset(&m_avRate, 0, sizeof(m_avRate));
	m_state = 0;
}

void CAssist::Init(CCharacter* ch)
{
	m_ch = ch;
}

bool CAssist::Add(CCharacter* spellchar, int itemidx, const CSkillProto* proto, int level, bool bHit[MAX_SKILL_MAGIC], bool bSend, int remain, int param, int nBlessAdd, int nBlessRate)
{
	if (!CanApply(proto, level))
		return false;

	CAssistList* list = NULL;

	if (proto->m_flag & SF_HELP)
		list = &m_help;
	else
		list = &m_curse;

	bool bCancelBlind = false;
	bool isStone = false;
	bool isDamageLink = false;

	int i;
	int j = 0;
	for (i = 0; i < MAX_SKILL_MAGIC; i++)
	{
		if (bHit[i] && proto->Level(level)->m_magic[i])
		{
			j++;

			switch (proto->Level(level)->m_magic[i]->m_type)
			{
			case MT_IMMUNE:
				switch (proto->Level(level)->m_magic[i]->m_subtype)
				{
				case MST_IMMUNE_BLIND:
					bCancelBlind = true;
					break;
				}
				break;

			case MT_ASSIST:
				switch (proto->Level(level)->m_magic[i]->m_subtype)
				{
#ifdef RAID_MONSTER_SKIP_HOLD
				case MST_ASSIST_HOLD:
					if( IS_NPC(m_ch) )
					{
						CNPC * pNpc = TO_NPC(m_ch);
						if( pNpc != NULL )
						{
							if( pNpc->m_proto->CheckFlag(NPC_RAID) )
							{
								return false;
							}
						}
					}
					break;
#endif // RAID_MONSTER_SKIP_HOLD

#ifdef NIGHTSHADOW_SKILL
#ifdef RAID_MONSTER_SKIP_FREEZE		// 레이드 몹 결계 제외	
				case MST_ASSIST_FREEZE:
					if( IS_NPC(m_ch) )
					{
						CNPC * pNpc = TO_NPC(m_ch);
						if( pNpc != NULL )
						{
							if( pNpc->m_proto->CheckFlag(NPC_RAID) )
							{
								return false;
							}
						}
					}
					break;
#endif // RAID_MONSTER_SKIP_FREEZE
#endif // NIGHTSHADOW_SKILL

				case MST_ASSIST_STONE:
#ifdef RAID_MONSTER_SKIP_STONE
					if( IS_NPC(m_ch) )
					{
						CNPC * pNpc = TO_NPC(m_ch);
						if( pNpc != NULL )
						{
							if( pNpc->m_proto->CheckFlag(NPC_RAID) )
							{
								return false;
							}
						}
					}
#endif // RAID_MONSTER_SKIP_STONE
					isStone = true;
					break;
#ifdef RAID_MONSTER_SKIP_STURN
				case MST_ASSIST_STURN:
					if( IS_NPC(m_ch) )
					{
						CNPC * pNpc = TO_NPC(m_ch);
						if( pNpc != NULL )
						{
							if( pNpc->m_proto->CheckFlag(NPC_RAID) )
							{
								return false;
							}
						}
					}
					break;
#endif // RAID_MONSTER_SKIP_STURN
				case MST_ASSIST_FEAR:
					if (spellchar == NULL)
						return false;
					break;

				case MST_ASSIST_DAMAGELINK:
					if (spellchar == NULL)
						return false;
					else
						isDamageLink = true;
					break;
				}
				break;
			}
		}
	}
	if (j == 0)
		return false;

	if (isStone)
		ClearAssist(bSend, true, true, true, false);
//	else if (m_state & AST_STONE)
//		return false;
	
	// 소서러의 소노바 브레이크 이면서 석화가 실패했다면 디버프 메세지를 주지 않는다.   yhj
	if ( proto->m_index == 311 && !isStone )
	{
		return false;
	}

	if (isDamageLink)
	{
		if (m_ch->m_linkSource || m_ch->m_linkTarget)
			m_ch->CancelDamageLink();

		if (spellchar != m_ch)
		{
			int nTemp = remain;
			if (!spellchar->m_assist.Add(spellchar, itemidx, proto, level, bHit, bSend, nTemp, param, nBlessAdd, nBlessRate))
				return false;
		}
	}

//	DelDuplicate(proto, level, bSend, false);
	DelDuplicate(proto, level, bSend, true);
	CheckApplyConditions();

	if (bCancelBlind)
		CureAssist(MST_ASSIST_BLIND, 99);

	if (!list->Add(spellchar, itemidx, proto, level, bHit, remain, param, nBlessAdd, nBlessRate))
		return false;

	if (isDamageLink)
	{
		if (spellchar != m_ch)
		{
			m_ch->m_linkSource = spellchar;
			spellchar->m_linkTarget = m_ch;
		}
	}

	m_ch->CalcStatus(true);

	if (bSend)
	{
		CNetMsg rmsg;

		AssistAddMsg(rmsg, m_ch, itemidx, proto->m_index, level, remain);
		m_ch->m_pArea->SendToCell(rmsg, m_ch, true);

		CharStatusMsg(rmsg, m_ch, 0);
		m_ch->m_pArea->SendToCell(rmsg, m_ch, true);
	}

	return true;
}

bool CAssist::CanApply(const CSkillProto* proto, int level)
{
	if (proto == NULL)
		return false;
	const CSkillLevelProto* levelproto = proto->Level(level);
	if (levelproto == NULL)
		return false;

	// 복구주문서
	if (levelproto->m_durtime <	PULSE_ASSIST_CHECK)
	{
#if defined ADULT_SERVER_NEWITEM
		if(proto->m_index == 245 || proto->m_index == 246 || proto->m_index == 244 || proto->m_index == 464 
#ifdef AUTO_POTION
			|| proto->m_index == 466
#endif // AUTO_POTION
#ifdef EVENT_PCBANG_2ND
			|| proto->m_index == 493
#endif // EVENT_PCBANG_2ND
#ifdef NEW_CASHITEM_USA_2008
			|| proto->m_index == 553
#endif //NEW_CASHITEM_USA_2008
#ifdef EVENT_PHOENIX   // 피닉스 이벤트  yhj
			|| proto->m_index == 516    // 피닉스 버프
			|| proto->m_index == 589    // 피닉스 부활
#endif // EVENT_PHOENIX
		);
#else
		if(proto->m_index == 245 || proto->m_index == 246 || proto->m_index == 244
#ifdef AUTO_POTION
			|| proto->m_index == 466
#endif // AUTO_POTION
#ifdef EVENT_PCBANG_2ND
			|| proto->m_index == 493
#endif // EVENT_PCBANG_2ND

#ifdef NEW_CASHITEM_USA_2008
			|| proto->m_index == 553
#endif //NEW_CASHITEM_USA_2008

#ifdef EVENT_PHOENIX   // 피닉스 이벤트  yhj
			|| proto->m_index == 516    // 피닉스 버프
			|| proto->m_index == 589    // 피닉스 부활
#endif // EVENT_PHOENIX
		);
#endif // ADULT_SERVER_NEWITEM
		else
			return false;
	}

	int i;
	for (i = 0; i < MAX_SKILL_MAGIC; i++)
	{
		if (levelproto->m_magic[i])
		{
			switch (levelproto->m_magic[i]->m_type)
			{
			case MT_ASSIST:
				switch (levelproto->m_magic[i]->m_subtype)
				{
				case MST_ASSIST_BLIND:
					if (FindByType(MT_IMMUNE, MST_IMMUNE_BLIND))
						return false;
					if (m_ch->m_avPassiveAddition.immune_blind)
							return false;
					break;
				}
				break;
			}
		}
	}

	return (m_help.CanApply(proto, level) && m_curse.CanApply(proto, level));
}

// 060227 : bs : 절대시간 버프 추가
void CAssist::DelDuplicate(const CSkillProto* proto, int level, bool bSend, bool bNoCancelType)
{
	m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);
	m_curse.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

	// 이속물약과 차크라실드는 더블무브먼트와 동시 적용 안된다
	switch (proto->m_index)
	{
	case 155:		// 더블무브먼트
		proto = gserver.m_skillProtoList.Find(70);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);
		m_curse.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(62);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);
		m_curse.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);
		break;
	case 70:		// 이속물약
	case 62:		// 챠크라
		proto = gserver.m_skillProtoList.Find(155);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);
		m_curse.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);
		break;
#ifdef EVENT_HALLOWEEN_2007
	case 470:
		proto = gserver.m_skillProtoList.Find(471);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(472);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(473);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(474);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);
		break;
	case 471:
		proto = gserver.m_skillProtoList.Find(470);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(472);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(473);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(474);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);
		break;
	case 472:
		proto = gserver.m_skillProtoList.Find(470);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(471);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(473);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(474);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);
		break;
	case 473:
		proto = gserver.m_skillProtoList.Find(470);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(471);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(472);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(474);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);
		break;
	case 474:
		proto = gserver.m_skillProtoList.Find(470);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(471);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(472);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);

		proto = gserver.m_skillProtoList.Find(473);
		m_help.DelDuplicate(proto, level, bSend, m_ch, bNoCancelType);
		break;
#endif //EVENT_HALLOWEEN_2007
	}
}

bool CAssist::DecreaseTime()
{
	if ( m_delaycheck == gserver.m_gameTime)
		return false;

	m_delaycheck = gserver.m_gameTime;

	int changestate = 0;

	bool bret = m_help.DecreaseTime(m_ch, &changestate);
	if (m_curse.DecreaseTime(m_ch, &changestate))
		bret = true;

	if (bret || changestate)
	{
		m_ch->CalcStatus(true);

		CNetMsg rmsg;
		CharStatusMsg(rmsg, m_ch, changestate);
		m_ch->m_pArea->SendToCell(rmsg, m_ch, true);
	}

	return bret;
}

void CAssist::Apply()
{
	memset(&m_avAddition, 0, sizeof(m_avAddition));
	memset(&m_avRate, 0, sizeof(m_avRate));
	m_state = 0;
	m_help.Apply(m_ch, &m_avAddition, &m_avRate, &m_state);
	m_curse.Apply(m_ch, &m_avAddition, &m_avRate, &m_state);

	m_ch->ApplyAssistData(&m_avAddition, &m_avRate);
}

// 060317 : bs : SF_NOCANCEL 검사 추가
void CAssist::ClearAssist(bool bSend, bool bByStone, bool bHelp, bool bCurse, bool bNoCancelType)
{
	CAssistData* pAssist;
	CAssistData* pAssistNext;

	if(bHelp)
	{
		pAssistNext = m_help.m_head;
		while ((pAssist = pAssistNext))
		{
			pAssistNext = pAssistNext->m_next;
			// 060317 : bs : SF_NOCANCEL 검사
			if (!bNoCancelType && (pAssist->m_proto->m_flag & SF_NOCANCEL))
				continue ;

			if (bByStone)
			{
				switch (pAssist->m_index)
				{
				case 671:		// 하급 경험의 결정
				case 672:		// 중급 경험의 결정
				case 673:		// 상급 경험의 결정
				case 674:		// 노력의 결정
					break;
				default:
					m_help.DelAssist(pAssist, bSend, m_ch, bNoCancelType);
					break;
				}
			}
			else
				m_help.DelAssist(pAssist, bSend, m_ch, bNoCancelType);
		}
	}
	if(bCurse)
	{
		pAssistNext = m_curse.m_head;
		while ((pAssist = pAssistNext))
		{
			pAssistNext = pAssistNext->m_next;
			if (bByStone)
			{
				switch (pAssist->m_index)
				{
				case 671:		// 하급 경험의 결정
				case 672:		// 중급 경험의 결정
				case 673:		// 상급 경험의 결정
				case 674:		// 노력의 결정
					break;
				default:
					m_curse.DelAssist(pAssist, bSend, m_ch, bNoCancelType);
					break;
				}
			}
			else
				m_curse.DelAssist(pAssist, bSend, m_ch, bNoCancelType);
		}
	}

	m_ch->CalcStatus(bSend);

	if (bSend)
	{
		CNetMsg rmsg;
		CharStatusMsg(rmsg, m_ch, 0);
		m_ch->m_pArea->SendToCell(rmsg, m_ch, true);
	}
}


void CAssist::GetListString(bool bHelp, char* item, char* index, char* level, char* remain, char* hit0, char* hit1, char* hit2)
{
	*item = *index = *level = *remain = *hit0 = *hit1 = *hit2 = '\0';

	CAssistData* p;

	if (bHelp)
		p = m_help.m_head;
	else
		p = m_curse.m_head;

	while (p)
	{
		bool bSkip = false;

		// 증폭제 저장 안함
		switch (p->m_index)
		{
		case 882:
		case 883:
		case 884:
		case 885:
			bSkip = true;
			break;
		}

		// 060227 : bs : 절대시간 사용 스킬은 t_assist에 저장 안함
		if (p->m_proto->m_flag & SF_ABSTIME)
			bSkip = true;
		// 060227 : bs : 절대시간 사용 스킬은 t_assist에 저장 안함

#ifdef MONSTER_COMBO
		if(p->m_proto->m_flag & SF_COMBO)
			bSkip = true;
#endif // MONSTER_COMBO

#ifdef EVENT_PCBANG_2ND
		if(p->m_proto->m_index == 493 )
		{	// PC방 버프 저장 안함
			bSkip = true;
		}
#endif // EVENT_PCBAG_2ND

#ifdef EVENT_PHOENIX   // 피닉스 이벤트  yhj
		if( p->m_proto->m_index == 516 )
		{	// 피닉스 버프 저장 안함
			bSkip = true;
		}
#endif // EVENT_PHOENIX

		if (!bSkip)
		{
			IntCat(item, p->m_index, true);
			IntCat(index, p->m_proto->m_index, true);
			IntCat(level, p->m_level, true);
			IntCat(remain, p->m_remain, true);
			IntCat(hit0, (p->m_bHit[0]) ? 1 : 0, true);
			IntCat(hit1, (p->m_bHit[1]) ? 1 : 0, true);
			IntCat(hit2, (p->m_bHit[2]) ? 1 : 0, true);
		}

		p = p->m_next;
	}
}

CNetMsg& CAssist::AppendAssistToNetMsg(CNetMsg& msg)
{
	CAssistData* p;
	
	msg << m_state
		<< (char)GetAssistCount();

	p = m_help.m_head;
	while (p)
	{
		msg << p->m_index
			<< p->m_proto->m_index
			<< (char)p->m_level
			<< p->m_remain;
		p = p->m_next;
	}

	p = m_curse.m_head;
	while (p)
	{
		msg << p->m_index
			<< p->m_proto->m_index
			<< (char)p->m_level
			<< p->m_remain;
		p = p->m_next;
	}

	return msg;
}

bool CAssist::Find(int magicindex, int magiclevel)
{
	CAssistData* p;
	const CSkillLevelProto* slp;
	int i;

	p = m_help.m_head;
	while (p)
	{
		slp = p->m_proto->Level(p->m_level);
		if (slp)
		{
			i = 0;
			while (i < MAX_SKILL_MAGIC)
			{
				if (slp->m_magic[i] && slp->m_magic[i]->m_index == magicindex && slp->m_magicLevel[i] >= magiclevel)
					return true;
				i++;
			}
		}
		p = p->m_next;
	}

	p = m_curse.m_head;
	while (p)
	{
		slp = p->m_proto->Level(p->m_level);
		if (slp)
		{
			i = 0;
			while (i < MAX_SKILL_MAGIC)
			{
				if (slp->m_magic[i] && slp->m_magic[i]->m_index == magicindex && slp->m_magicLevel[i] >= magiclevel)
					return true;
				i++;
			}
		}
		p = p->m_next;
	}

	return false;
}

void CAssist::CancelSleep()
{
	CureAssist(MST_ASSIST_SLEEP, 99);
}
#ifdef ADULT_SERVER_NEWITEM
void CAssist::CancelMantle()
{
	CureAssist(MST_ASSIST_MANTLE, 99);
}
#endif // ADULT_SERVER_NEWITEM

void CAssist::CureAssist(int flag)
{
	CAssistData* p;
	CAssistData* pNext;

	pNext = m_help.m_head;
	while((p = pNext))
	{
		pNext = pNext->m_next;
		if(p->m_proto->m_flag & flag)
			m_help.DelAssist(p, true, m_ch, true);
	}

	pNext = m_curse.m_head;
	while((p = pNext))
	{
		pNext = pNext->m_next;
		if(p->m_proto->m_flag & flag)
			m_curse.DelAssist(p, true, m_ch, true);
	}
}

void CAssist::CureAssist(int type, int level)
{
	CAssistData* p;
	CAssistData* pNext;
	const CSkillLevelProto* slp;
	const CMagicProto* mp;
	int i;

	pNext = m_help.m_head;
	while ((p = pNext))
	{
		pNext = pNext->m_next;
		slp = p->m_proto->Level(p->m_level);
		if (slp)
		{
			i = 0;
			while (i < MAX_SKILL_MAGIC)
			{
				mp = slp->m_magic[i];
				if (mp && mp->m_type == MT_ASSIST && mp->m_subtype == type && slp->m_magicLevel[i] <= level)
				{
					m_help.DelAssist(p, true, m_ch, true);
					break;
				}
				i++;
			}
		}
	}

	pNext = m_curse.m_head;
	while ((p = pNext))
	{
		pNext = pNext->m_next;
		slp = p->m_proto->Level(p->m_level);
		if (slp)
		{
			i = 0;
			while (i < MAX_SKILL_MAGIC)
			{
				mp = slp->m_magic[i];
				if (mp && mp->m_type == MT_ASSIST && mp->m_subtype == type && slp->m_magicLevel[i] <= level)
				{
					m_curse.DelAssist(p, true, m_ch, true);
					break ;
				}
				i++;
			}
		}
	}
}

void CAssist::CancelInvisible()
{
	CureAssist(MST_ASSIST_INVISIBLE, 99);
}

void CAssist::CheckApplyConditions()
{
	m_help.CheckApplyConditions(m_ch);
	m_curse.CheckApplyConditions(m_ch);
}

void CAssistList::CheckApplyConditions(CCharacter* ch)
{
	CAssistData* p;
	CAssistData* pNext;
//	bool bFinish = false;

//	while (!bFinish)
//	{
//		bFinish = true;
		pNext = m_head;
		while ((p = pNext))
		{
			pNext = pNext->m_next;

			if (!ch->CanApplySkill(p->m_proto, p->m_proto->Level(p->m_level)))
			{
				DelAssist(p, true, ch, false);
//				bFinish = false;
			}
		}
//	}
}

int CAssist::FindByType(int type, int subtype, bool* outHelp, CAssistData** outData)
{
	CAssistData* p;
	CAssistData* pNext;
	const CSkillLevelProto* slp;
	const CMagicProto* mp;
	int i;

	pNext = m_help.m_head;
	while ((p = pNext))
	{
		pNext = pNext->m_next;
		slp = p->m_proto->Level(p->m_level);
		if (slp)
		{
			i = 0;
			while (i < MAX_SKILL_MAGIC)
			{
				mp = slp->m_magic[i];
				if (mp && mp->m_type == type && mp->m_subtype == subtype)
				{
					if (outHelp)
						*outHelp = true;
					if (outData)
						*outData = p;
					return slp->m_magicLevel[i];
				}
				i++;
			}
		}
	}

	pNext = m_curse.m_head;
	while ((p = pNext))
	{
		pNext = pNext->m_next;
		slp = p->m_proto->Level(p->m_level);
		if (slp)
		{
			i = 0;
			while (i < MAX_SKILL_MAGIC)
			{
				mp = slp->m_magic[i];
				if (mp && mp->m_type == type && mp->m_subtype == subtype)
				{
					if (outHelp)
						*outHelp = false;
					if (outData)
						*outData = p;
					return slp->m_magicLevel[i];
				}
				i++;
			}
		}
	}

	if (outHelp)
		*outHelp = false;
	if (outData)
		*outData = NULL;
	return 0;
}

void CAssist::CancelFear()
{
	CureAssist(MST_ASSIST_FEAR, 99);
}

void CAssist::CancelFakeDeath()
{
	CureAssist(MST_ASSIST_FAKEDEATH, 99);
}

void CAssist::CureByItemIndex(int itemindex)
{
	CAssistData* p;
	CAssistData* pNext;

	pNext = m_help.m_head;
	while ((p = pNext))
	{
		pNext = pNext->m_next;
		if (p->m_index != -1 && p->m_index == itemindex)
			m_help.DelAssist(p, true, m_ch, true);
	}

	pNext = m_curse.m_head;
	while ((p = pNext))
	{
		pNext = pNext->m_next;
		if (p->m_index != -1 && p->m_index == itemindex)
			m_curse.DelAssist(p, true, m_ch, true);
	}
}

void CAssist::CureBySkillIndex(int nSkillIndex)
{
	CAssistData* p;
	CAssistData* pNext;

	pNext = m_help.m_head;
	while ((p = pNext))
	{
		pNext = pNext->m_next;
		if (p->m_proto && p->m_proto->m_index == nSkillIndex)
			m_help.DelAssist(p, true, m_ch, true);
	}

	pNext = m_curse.m_head;
	while ((p = pNext))
	{
		pNext = pNext->m_next;
		if (p->m_proto && p->m_proto->m_index == nSkillIndex)
			m_curse.DelAssist(p, true, m_ch, true);
	}
}

bool CAssist::FindByItemIndex(int itemindex, CAssistData** outData)
{
	CAssistData* p;
	CAssistData* pNext;

	pNext = m_help.m_head;
	while ((p = pNext))
	{
		pNext = pNext->m_next;
		if (p->m_index != -1 && p->m_index == itemindex)
		{
			if (outData)
				*outData = p;
			return true;
		}
	}

	pNext = m_curse.m_head;
	while ((p = pNext))
	{
		pNext = pNext->m_next;
		if (p->m_index != -1 && p->m_index == itemindex)
		{
			if (outData)
				*outData = p;
			return true;
		}
	}

	return false;
}

int CAssist::FindBySkillIndex(int skillindex)
{
	CAssistData* p;
	CAssistData* pNext;

	pNext = m_help.m_head;
	while ((p = pNext))
	{
		pNext = pNext->m_next;
		if (p->m_proto->m_index == skillindex)
			return p->m_level;
	}

	pNext = m_curse.m_head;
	while ((p = pNext))
	{
		pNext = pNext->m_next;
		if (p->m_proto->m_index == skillindex)
			return p->m_level;
	}

	return 0;
}

// 060227 : bs : 절대시간 사용 보조효과 리스트
int CAssist::GetABSTimeTypeList(int* nAssistABSItemIndex, int* nAssistABSSkillIndex, int* nAssistABSSkillLevel, char* nAssistABSHit0, char* nAssistABSHit1, char* nAssistABSHit2, int* nAssistABSEndTime)
{
	int ret = 0;

	CAssistData* p;

	p = m_help.m_head;
	while (p)
	{
		if (p->m_proto && (p->m_proto->m_flag & SF_ABSTIME))
		{
			if (nAssistABSItemIndex)
			{
#ifdef EVENT_PCBANG_2ND
				if(p->m_proto->m_index != 493)
				{	// PC방 버프 저장 안함
#endif // EVENT_PCBANG_2ND

#ifdef EVENT_PHOENIX   // 피닉스 이벤트  yhj
				if( p->m_proto->m_index != 516 )
				{	// 피닉스 버프 저장 안함
#endif // EVENT_PHOENIX

				nAssistABSItemIndex[ret] = p->m_index;
				nAssistABSSkillIndex[ret] = p->m_proto->m_index;
				nAssistABSSkillLevel[ret] = p->m_level;
				nAssistABSHit0[ret] = p->m_bHit[0];
				nAssistABSHit1[ret] = p->m_bHit[1];
				nAssistABSHit2[ret] = p->m_bHit[2];
				nAssistABSEndTime[ret] = gserver.m_gameTime + (p->m_remain / PULSE_ASSIST_CHECK);

#ifdef EVENT_PHOENIX   // 피닉스 이벤트  yhj
				}
#endif // EVENT_PHOENIX

#ifdef EVENT_PCBANG_2ND
				}
#endif // EVENT_PCBAG_2ND


			}
			ret++;
		}
		p = p->m_next;
	}

	p = m_curse.m_head;
	while (p)
	{
		if (p->m_proto && (p->m_proto->m_flag & SF_ABSTIME))
		{
			if (nAssistABSItemIndex)
			{
#ifdef EVENT_PCBAG_2ND
				if(p->m_proto->m_index != 493)
				{	// PC방 버프 저장 안함
#endif // EVENT_PCBAG_2ND

#ifdef EVENT_PHOENIX   // 피닉스 이벤트  yhj
	//			if( p->m_proto->m_index != 516 )
	//			{	// 피닉스 버프 저장 안함
#endif // EVENT_PHOENIX

				nAssistABSItemIndex[ret] = p->m_index;
				nAssistABSSkillIndex[ret] = p->m_proto->m_index;
				nAssistABSSkillLevel[ret] = p->m_level;
				nAssistABSHit0[ret] = p->m_bHit[0];
				nAssistABSHit1[ret] = p->m_bHit[1];
				nAssistABSHit2[ret] = p->m_bHit[2];
				nAssistABSEndTime[ret] = gserver.m_gameTime + (p->m_remain / PULSE_ASSIST_CHECK);

#ifdef EVENT_PHOENIX   // 피닉스 이벤트  yhj
	//			}
#endif // EVENT_PHOENIX

#ifdef EVENT_PCBAG_2ND
				}
#endif // EVENT_PCBAG_2ND
			}
			ret++;
		}
		p = p->m_next;
	}

	return ret;
}
// 060227 : bs : 절대시간 사용 보조효과 리스트

void CAssist::CancelDespair()
{
	CureAssist(MST_ASSIST_DESPAIR, 99);
}

void CAssist::CancelManaScreen()
{
	CureAssist(MST_ASSIST_MANASCREEN, 99);
}

void CAssist::CancelBless()
{
	CureAssist(MST_ASSIST_BLESS, 99);
}
