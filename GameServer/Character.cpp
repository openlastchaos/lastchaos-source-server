#include "stdhdrs.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "Exp.h"
#include "NetMsg.h"
#include "doFunc.h"
#include "Battle.h"
#include "WarCastle.h"
#include "Log.h"

extern void DelAttackList(CCharacter* ch);

////////////////////
// CCharacter member
CCharacter::CCharacter()
: m_name(MAX_CHAR_NAME_LENGTH + 1)
{
	m_type = MSG_CHAR_UNKNOWN;

	m_index = -1;
	m_level = 0;
	m_exp = 0;
	m_skillPoint = 0;


	m_maxHP = 0;
	m_hp = 0;
	m_maxMP = 0;
	m_mp = 0;

	m_recoverHP = 0;
	m_recoverMP = 0;
	m_attackRange = 0.0f;

	m_str = 0;
	m_dex = 0;
	m_int = 0;
	m_con = 0;

	m_eqMelee = 0;
	m_eqRange = 0;
	m_eqMagic = 0;
	m_eqDefense = 0;
	m_eqResist = 0;

	m_attackSpeed = 0;
	m_magicSpeed = 0;
	m_skillSpeed = 0;
	m_statPall = 0;
	// << 071226 kjban add
	m_statPall_per = 0;
	// >>

	m_walkSpeed = 0.0f;
	m_runSpeed = 0.0f;
	m_recoverPulse = 0;

	m_pos = CPos(0, 0, 0, 0, 0);

	m_cellX = NO_CELL;
	m_cellZ = NO_CELL;

	m_assist.Init(this);

	m_attackList = NULL;

	m_pCellPrev = NULL;
	m_pCellNext = NULL;
	m_pZone = NULL;
	m_pArea = NULL;

	m_bVisible = true;

	m_currentSkill = NULL;
	m_linkSource = NULL;
	m_linkTarget = NULL;

	m_opStr = 0;
	m_opDex = 0;
	m_opInt = 0;
	m_opCon = 0;

	m_opMelee = 0;
	m_opRange = 0;
	m_opMeleeHitRate = 0;
	m_opRangeHitRate =0;

	m_opDMelee = 0;
	m_opDRange = 0;
	m_opMeleeAvoid = 0;
	m_opRangeAvoid =0;

	m_opMagic = 0;
	m_opMagicHitRate = 0;
	m_opResist = 0;
	m_opResistAvoid = 0;
	
	memset(&m_avPassiveAddition, 0, sizeof(m_avPassiveAddition));
	memset(&m_avPassiveRate, 0, sizeof(m_avPassiveRate));

	m_hackAttackCount = PULSE_HACK_ATTACK_COUNT;	// 어택메세지 온 수 저장
	m_AttackServerTime = 0;	// 공속에 따른 최대 어택 시간 
	m_AttackClientTime = 0; // 클라이언트가 보낸 어택시간
	m_lastAttackPulse = 0;
	
	m_pulseDisable = 0;
#ifdef NEW_ACCERY_ADD
	m_AddProb	= 0;
#endif // NEW_ACCERY_ADD
	cooltime_2142 = 0;			// 건강의 물약
	cooltime_2143 = 0;			// 지력의 물약
#ifdef MOB_SCROLL
	MobScroll = 0;
#endif // MOB_SCROLL

	
#ifdef COMPETITION_ITEM
	m_cooltime_Competition = 0;	// 대전용 아이템 

#endif // COMPETITION_ITEM
	
#ifdef ADULT_SERVER_NEW_BALANCE
	m_nFinalHitrate = 0;
	m_nFinalAvoidrate = 0;
#endif //ADULT_SERVER_NEW_BALANCE

#ifdef NIGHT_SHADOW
	m_flySpeed = 15.0f;		// 날기 속도
	m_attacktype = ATTACK_TYPE_NORMAL;
	m_attackcount = 0;
#endif //NIGHT_SHADOW
}

CCharacter::~CCharacter()
{
	m_type = MSG_CHAR_UNKNOWN;
	m_index = -1;
	DelAttackList(this);
}

void CCharacter::SetVisible()
{
	CNetMsg msg;
#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
	CNetMsg GuildPointMsg;
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD
#ifdef DYNAMIC_DUNGEON
	CNetMsg NameColorMsg;
#endif //DYNAMIC_DUNGEON
	if (m_bVisible)
	{
		// 보이는 상태이면
		// 공격 받지 않게 하고
		DelAttackList(this);
		// 주변에 유저에게 사라진다고 알리고
		DisappearMsg(msg, this);
		// 상태 토글
		m_bVisible = false;
		if (IS_PC(this))
			TO_PC(this)->m_bImmortal = true;
	}
	else
	{
		// 안보이는 상태이면
		// 상태 토글
		m_bVisible = !m_bVisible;
		// 주변 유저에게 나타남을 알리고
		AppearMsg(msg, this, true);

#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
		if( TO_PC(this)->m_guildInfo && TO_PC(this)->m_guildInfo->guild() )
			GuildPointRankingMsg( GuildPointMsg, TO_PC(this)->m_index, TO_PC(this)->m_guildInfo->guild()->index(), TO_PC(this)->m_guildInfo->guild()->GetGuildPointRanking() );
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD
#ifdef DYNAMIC_DUNGEON
		if( TO_PC(this) )
		{
			GuildNameColorStateMsg(NameColorMsg, TO_PC(this) ); 
		}
#endif //DYNAMIC_DUNGEON
	}

	// 주변에 알리고
	m_pArea->SendToCell(msg, this);

#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
		if( TO_PC(this)->m_guildInfo && TO_PC(this)->m_guildInfo->guild() )
			m_pArea->SendToCell( GuildPointMsg, this );
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD

#ifdef DYNAMIC_DUNGEON
		if( TO_PC(this) )
		{
			m_pArea->SendToCell( NameColorMsg, this );
		}
#endif //DYNAMIC_DUNGEON
}

void CCharacter::SendDisappearAllInCell(bool bSendOtherDisappear)
{
	CNetMsg rmsg;

	// 타캐릭터에게 사라짐 알림
	DisappearMsg(rmsg, this);
	if (IS_PC(this))
		m_pArea->SendToCell(rmsg, this);
	else
		m_pArea->SendToCell(rmsg, this, true);

	if (!IS_PC(this))
		return ;

	if (!bSendOtherDisappear)
		return ;

	CPC* pc = TO_PC(this);

	// 사용자에게 다른 캐릭터 삭제
	int ix, iz;
	int cx = m_cellX;
	int cz = m_cellZ;
	for (ix = cx - CELL_EXT; ix <= cx + CELL_EXT; ix++)
	{
		if (ix < 0 || ix >= m_pArea->m_size[0]) continue ;

		for (iz = cz - CELL_EXT; iz <= cz + CELL_EXT; iz++)
		{
			if (iz < 0 || iz >= m_pArea->m_size[1]) continue ;

			CCharacter* pChar;
			CCharacter* pNext = m_pArea->m_cell[ix][iz].m_listChar;
			while ((pChar = pNext))
			{
				pNext = pChar->m_pCellNext;
				if (pChar == this)
					continue ;
				DisappearMsg(rmsg, pChar);
				SEND_Q(rmsg, pc->m_desc);
			}

			CItem* pItem;
			CItem* pItemNext = m_pArea->m_cell[ix][iz].m_listItem;
			while ((pItem = pItemNext))
			{
				pItemNext = pItem->m_pCellNext;
				ItemDisappearMsg(rmsg, pItem);
				SEND_Q(rmsg, pc->m_desc);
			}
		}
	}
}

bool CCharacter::CanApplySkill(const CSkillProto* proto, const CSkillLevelProto* levelproto)
{
	if (proto == NULL)
		return false;
	if (levelproto == NULL)
		return false;

	// 적용조건 검사 : state
	if ((proto->m_applyState & SCT_DEATH) && !DEAD(this))
		return false;
	if (!(proto->m_applyState & SCT_DEATH) && DEAD(this))
		return false;

	if (IS_PC(this))
	{
		CPC* pc = TO_PC(this);
		if (proto->m_applyState & SCT_WEAPON)
		{
			if (pc->m_wearing[WEARING_WEAPON] == NULL)
				return false;
			if (!pc->m_wearing[WEARING_WEAPON]->IsWeaponType())
				return false;
			int sub = pc->m_wearing[WEARING_WEAPON]->m_itemProto->m_subtypeIdx;
			if ((proto->m_applyWeaponType0 >= 0 || proto->m_applyWeaponType1 >= 0) && (sub != proto->m_applyWeaponType0 && sub != proto->m_applyWeaponType1))
				return false;
		}
		if (proto->m_applyState & SCT_WEAPON && pc->m_wearing[WEARING_WEAPON] == NULL)
			return false;
		if (!(proto->m_applyState & SCT_STAND) || !(proto->m_applyState & SCT_SITDOWN))
		{
			// 둘다 꺼져 있는 경우는 둘다 켜져 있는 경우와 동일
			if ((proto->m_applyState & SCT_SITDOWN) || (proto->m_applyState & SCT_STAND))
			{
				if (proto->m_applyState & SCT_SITDOWN && !pc->IsSetPlayerState(PLAYER_STATE_SITDOWN))
					return false;
				if (proto->m_applyState & SCT_STAND && pc->IsSetPlayerState(PLAYER_STATE_SITDOWN))
					return false;
			}
		}
		if (proto->m_applyState & SCT_SHIELD && pc->m_wearing[WEARING_SHIELD] == NULL)
			return false;
		
		if (proto->m_index == 433	// 건강의 물약
			&& cooltime_2142 > gserver.m_gameTime)
		{
			return false;			
		}

		if (proto->m_index == 434	// 지력의 물약
			&& cooltime_2143 > gserver.m_gameTime)
		{
			return false;
		}

#ifdef COMPETITION_ITEM
		if( proto->m_index == 465 // 광속 
			&& m_cooltime_Competition > gserver.m_gameTime )
		{
			return false;
		}
#endif // COMPETITION_ITEM

	}

	if ((proto->m_applyState & SCT_PEACEZONE) && IsInPeaceZone(false))
		return false;
	
	// 적용조건 검사 : 효과
	int i;
	for (i = 0; i < MAX_SKILL_USE_MAGIC; i++)
	{
		if (levelproto->m_appMagicIndex[i] < 0)
			continue ;
		if (!m_assist.Find(levelproto->m_appMagicIndex[i], levelproto->m_appMagicLevel[i]))
			return false;
	}

	return true;
}

bool CCharacter::CanMove()
{
	if (m_assist.m_state & AST_STONE)
		return false;
	if (m_assist.m_state & AST_HOLD)
		return false;
	if (m_assist.m_state & AST_STURN)
		return false;
	if (m_assist.m_state & AST_SLEEP)
		return false;
#ifdef NIGHTSHADOW_SKILL
	if (m_assist.m_state & AST_FREEZE)
		return false;
#endif // NIGHTSHADOW_SKILL
	return true;
}

bool CCharacter::CanAttack()
{
	if (m_assist.m_state & AST_STONE)
		return false;
	if (m_assist.m_state & AST_STURN)
		return false;
	if (m_assist.m_state & AST_SLEEP)
		return false;
	if (m_assist.m_state & AST_FAKEDEATH)
		return false;
#ifdef NIGHTSHADOW_SKILL
	if (m_assist.m_state & AST_FREEZE)
		return false;
#endif // NIGHTSHADOW_SKILL
	return true;
}

bool CCharacter::CanSpell()
{
	if (m_assist.m_state & AST_STONE)
		return false;
	if (m_assist.m_state & AST_SILENT)
		return false;
	if (m_assist.m_state & AST_STURN)
		return false;
	if (m_assist.m_state & AST_SLEEP)
		return false;
	if (m_assist.m_state & AST_FAKEDEATH)
		return false;
#ifdef NIGHTSHADOW_SKILL
	if (m_assist.m_state & AST_FREEZE)
		return false;
#endif // NIGHTSHADOW_SKILL
	return true;
}

void CCharacter::ApplyAssistData(ASSISTVALUE* add, ASSISTVALUE* rate)
{
	m_attackSpeed -= (char)add->attackspeed;
	m_attackSpeed -= (char)((int)m_attackSpeed * rate->attackspeed / SKILL_RATE_UNIT);
	m_magicSpeed += (char)add->magicspeed;
	m_magicSpeed += (char)((int)m_magicSpeed * rate->magicspeed / SKILL_RATE_UNIT);
	m_skillSpeed += rate->skillspd;

	m_statPall += add->statpall;

	this->m_str += add->statpall;
	if( this->m_str < 1) this->m_str = 1;
	this->m_dex += add->statpall;
	if( this->m_dex < 1) this->m_dex = 1;
	this->m_int += add->statpall;
	if( this->m_int < 1) this->m_int = 1;
	this->m_con += add->statpall;
	if( this->m_con < 1) this->m_con = 1;
	// < 071226 kjban add
	m_statPall_per += add->statpall_per;
	this->m_str += this->m_str*add->statpall_per/100;
	if( this->m_str < 1) this->m_str = 1;
	this->m_dex += this->m_dex*add->statpall_per/100;
	if( this->m_dex < 1) this->m_dex = 1;
	this->m_int += this->m_int*add->statpall_per/100;
	if( this->m_int < 1) this->m_int = 1;
	this->m_con += this->m_con*add->statpall_per/100;
	if( this->m_con < 1) this->m_con = 1;
	// >

	m_runSpeed += add->movespeed / 10.0f;
	m_runSpeed += m_runSpeed * rate->movespeed / SKILL_RATE_UNIT;

	m_maxHP += add->maxhp;
	m_maxHP += m_maxHP * rate->maxhp / SKILL_RATE_UNIT;

	int addmaxhp450 = add->maxhp450;
	int ratemaxhp450 = m_maxHP * rate->maxhp450 / SKILL_RATE_UNIT;;
	if (addmaxhp450 > 450)
		addmaxhp450 = 450;
	if (ratemaxhp450 > 450)
		ratemaxhp450 = 450;
	m_maxHP += addmaxhp450 + ratemaxhp450;

	m_maxMP += add->maxmp;
	m_maxMP += m_maxMP * rate->maxmp / SKILL_RATE_UNIT;

	// 060227 : bs : 유료 HP,MP 확장
	if (rate->hcCashMaxHPUp > 0)
	{
		m_maxHP += m_maxHP * rate->hcCashMaxHPUp / SKILL_RATE_UNIT;
	}
	if (rate->hcCashMaxMPUp > 0)
	{
		m_maxMP += m_maxMP * rate->hcCashMaxMPUp / SKILL_RATE_UNIT;
	}

#ifdef EVENT_HALLOWEEN_2007
	m_maxHP += m_maxHP * rate->hcEventHalloweenMaxHP / SKILL_RATE_UNIT;
	m_runSpeed += m_runSpeed * rate->hcEventHalloweenSpeed / SKILL_RATE_UNIT;
#endif //EVENT_HALLOWEEN_2007

	m_attackRange += add->attackdist;
	m_attackRange += m_attackRange * rate->attackdist / SKILL_RATE_UNIT;

	// 속성 지속
	m_attribute.Add(AT_FIRE, add->attfire, false);
	m_attribute.Add(AT_WATER, add->attwater, false);
	m_attribute.Add(AT_EARTH, add->attearth, false);
	m_attribute.Add(AT_WIND, add->attwind, false);
	m_attribute.Add(AT_DARK, add->attdark, false);
	m_attribute.Add(AT_LIGHT, add->attlight, false);

	// TODO : 하드코딩용
	// --- TODO : 하드코딩용
}

bool CCharacter::IsInPeaceZone(bool bBlockAsPeace)
{
	if (!m_pArea)
		return true;
	switch (m_pArea->GetAttr(GET_YLAYER(this), GET_X(this), GET_Z(this)))
	{
	case MAPATT_PEACEZONE:
	case MAPATT_PRODUCT_PUBLIC:
	case MAPATT_PRODUCT_PRIVATE:
		return true;

	case MAPATT_BLOCK:
		return bBlockAsPeace;
	}
	return false;
}

bool CCharacter::IsEnemy(CCharacter* tch)
{
	if (this == tch)
		return false;

	switch (m_type)
	{
	case MSG_CHAR_PC:
		switch (tch->m_type)
		{
		case MSG_CHAR_NPC:
#ifdef ENABLE_WAR_CASTLE
			// PC가 수호병을 공격할 때에는 공성 지역내에 있어야 적으로 인식한다
			if (TO_NPC(tch)->m_proto->CheckFlag(NPC_CASTLE_GUARD))
			{
				if (GetMapAttr() == MAPATT_WARZONE && IS_ATTACK_TEAM(TO_PC(this)->GetJoinFlag(m_pZone->m_index)))
					return true;
				else
					return false;
			}
#endif
			return true;

		case MSG_CHAR_PC:
			{
				CPC* ch = TO_PC(this);
				CPC* target = TO_PC(tch);

				// 같은 파티는 적이 될 수 없음 : 파티가 가장 높은 우선순위를 가짐
				if (ch->IsParty() && target->IsParty() && ch->m_party == target->m_party)
					return false;

				// 같은 길드는 적이 될 수 없음
				if (ch->m_guildInfo && target->m_guildInfo && ch->m_guildInfo->guild() == target->m_guildInfo->guild())
				{

#ifdef EXTREME_CUBE
					if(!gserver.m_extremeCube.IsGuildCubeTime())
					{
						// 파티큐브일 때
						// 큐브 안에서는 같은 길드라도 공격가능
						if(ch->m_pZone != NULL && !ch->m_pZone->IsExtremeCube())
							return false;
						
						if(target->m_pZone !=NULL && !target->m_pZone->IsExtremeCube())
							return false;

						return true;
					}
#endif // EXTREME_CUBE					
					return false;
				}



//#ifdef EXTREME_CUBE
//#if defined (TEST_SERVER) || defined (LC_USA)
					
//#endif // TEST_SERVER
//#endif // EXTREME_CUBE
					
//					return false;

				// 길드전
				if (ch->m_guildInfo && (ch->m_guildInfo->guild()->battleState() == GUILD_BATTLE_STATE_ING)
					&& target->m_guildInfo && (target->m_guildInfo->guild()->battleState() == GUILD_BATTLE_STATE_ING))
				{
					if (ch->m_guildInfo->guild()->battleIndex() == target->m_guildInfo->guild()->index() &&
						target->m_guildInfo->guild()->battleIndex() == ch->m_guildInfo->guild()->index())
					{
						return true;
					}
				}

#ifdef ENABLE_WAR_CASTLE
				// 둘다 공성 참여자 이면
				// 서로의 참여 플래그가 적대적이면 적이다
				CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
				if (castle && castle->GetState() != WCSF_NORMAL)
				{
					if (ch->GetMapAttr() == tch->GetMapAttr() && ch->GetMapAttr() == MAPATT_WARZONE)
					{
						if ( (IS_DEFENSE_TEAM(ch->GetJoinFlag(ch->m_pZone->m_index)) && IS_ATTACK_TEAM(target->GetJoinFlag(ch->m_pZone->m_index))) ||
							 ( IS_ATTACK_TEAM(ch->GetJoinFlag(ch->m_pZone->m_index)) && target->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE)  )
							 return true;
					}
				}
#endif
				/////////////////////////////////////////////
				// BANGWALL : 2005-07-18 오전 11:38:17
				// Comment : freepkzone
				if ( ch->GetMapAttr() == tch->GetMapAttr() && ch->GetMapAttr() == MAPATT_FREEPKZONE )
					return true;

				if (ch->IsSetPlayerState(PLAYER_STATE_PKMODE))
				{
					bool bSkipLevel = false;

#ifdef ENABLE_WAR_CASTLE
					// 공성전 지역에서 공성 진행시에는 레벨 검사 안함
					if (ch->m_pZone->m_index == tch->m_pZone->m_index && ch->m_pZone->m_index == CWarCastle::GetCurSubServerCastleZoneIndex() && ch->GetMapAttr() == tch->GetMapAttr() && ch->GetMapAttr() == MAPATT_WARZONE)
					{
						CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
						if (castle && castle->GetState() != WCSF_NORMAL)
						{
							bSkipLevel = true;
						}
					}
#endif // #ifdef ENABLE_WAR_CASTLE
					if(!bSkipLevel && tch->m_level <= PKMODE_LIMIT_LEVEL )
					{
#ifdef FREE_PK_SYSTEM
						if( gserver.m_bFreePk)
							return true;
#endif // FREE_PK_SYSTEM
						return false;
					}
					else
						return true;
				}

				if (ch->IsSetPlayerState(PLAYER_STATE_RAMODE))
				{
					if (IsRaList(ch, target))
						return true;
					else
						return false;
				}
					
				return false;
			}
			break;

#ifdef ENABLE_PET
		case MSG_CHAR_PET:
			{
				CPet* tpet = TO_PET(tch);
				if (!tpet->GetOwner())
					return false;
				else
					return IsEnemy(tpet->GetOwner());
			}
			break;
#endif // #ifdef ENABLE_PET

		case MSG_CHAR_ELEMENTAL:
			{
				CElemental* tElemental = TO_ELEMENTAL(tch);
				if (!tElemental->GetOwner())
					return false;
				else
					return IsEnemy(tElemental->GetOwner());
			}
			break;
			
#ifdef ATTACK_PET
		case MSG_CHAR_APET :
			{
				CAPet* apet = TO_APET(tch);
				if (!apet->GetOwner())
					return false;
				else
					return IsEnemy(apet->GetOwner());
			}
			break;
#endif //ATTACK_PET

		default:
			return false;
		}
		break;

	case MSG_CHAR_NPC:
		{
			CPC* tpc = NULL;
			switch (tch->m_type)
			{
			case MSG_CHAR_PC:
				tpc = TO_PC(tch);
				break;
			case MSG_CHAR_PET:
				tpc = TO_PET(tch)->GetOwner();
				break;
			case MSG_CHAR_ELEMENTAL:
				tpc = TO_ELEMENTAL(tch)->GetOwner();
				break;
#ifdef ATTACK_PET
			case MSG_CHAR_APET :
				tpc = TO_APET(tch)->GetOwner();
				break;
#endif // ATTACK_PET
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
				// 일반 몬스터가 테이밍된 몬스터를 적으로 인식
			case MSG_CHAR_NPC :
				tpc = TO_NPC(tch)->GetOwner();
				break;
#endif  // NIGHTSHADOW_SKILL	
			default:
				tpc = NULL;
				break;
			}
			
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
			// 여기로 넘어오면 테이밍 npc가 일반 npc를 타겟으로 상대하는 중이다.
			// tpc가 없고 npc 이면 일반 몬스터 이므로 적이다.
			if ( tpc == NULL && IS_NPC(tch) && TO_NPC(this)->Check_MobFlag( STATE_MONSTER_TAMING ) )
			{
				// 자기 자신이면 적이 아님
				if ( this == tch )
				{
					return false;
				}

				// 다른 몬스터는 적
				else
				{
					return true;
				}
			}
#endif  // NIGHTSHADOW_SKILL

			if (tpc)
			{
#ifdef ENABLE_WAR_CASTLE
				// 수호병은 공성측만 적으로 인식
				if (TO_NPC(this)->m_proto->CheckFlag(NPC_CASTLE_GUARD | NPC_CASTLE_TOWER))
				{
						if (IS_ATTACK_TEAM(tpc->GetJoinFlag(tpc->m_pZone->m_index)))
						return true;
					else
						return false;
				}
#endif
				
#ifdef NIGHTSHADOW_SKILL
				// 테이밍된 npc는 pc를 아군으로 인식, 힐 스킬을 사용해준다.
				if ( TO_NPC(this)->Check_MobFlag( STATE_MONSTER_TAMING ) )
				{
					return false;
				}
#endif  // NIGHTSHADOW_SKILL

				return true;
			}
			else
				return false;
			}
			break;
#ifdef ENABLE_PET
	case MSG_CHAR_PET:
		{
			CPet* pet = TO_PET(this);
			if (!pet->GetOwner())
				return false;
			else
				return pet->GetOwner()->IsEnemy(tch);
		}
		break;
#endif // #ifdef ENABLE_PET

	case MSG_CHAR_ELEMENTAL:
		{
			CElemental* pElemental = TO_ELEMENTAL(this);
			if (!pElemental->GetOwner())
				return false;
			else
				return pElemental->GetOwner()->IsEnemy(tch);
		}
		break;
		
#ifdef ATTACK_PET
	case MSG_CHAR_APET :
		{
			CAPet* apet = TO_APET(this);
			if( apet )
			{
				return apet->GetOwner()->IsEnemy(tch);
			}
		}
#endif // ATTACK_PET

	default:
		return false;
	}
}

bool CCharacter::CanVisible(CCharacter* tch)
{
	if (this == tch)
		return true;

	if (!tch->IsInvisible())
		return true;

	switch (m_type)
	{
	case MSG_CHAR_PC:
		switch (tch->m_type)
		{
		case MSG_CHAR_PC:
			{
				CPC* pc = TO_PC(this);
				CPC* tpc = TO_PC(tch);
				if (pc->IsParty() && tpc->IsParty() && pc->m_party == tpc->m_party)
					return true;
				else
					return false;
			}
			break;

		case MSG_CHAR_NPC:
			return false;

		default:
			return false;
		}
		break;

	case MSG_CHAR_NPC:
		switch (tch->m_type)
		{
		case MSG_CHAR_PC:
			if (tch->m_level + 5 - m_level <= 0)
				return true;
			else
				return false;
			break;

		case MSG_CHAR_NPC:
			return true;
			break;

		default:
			return false;
		}
		break;

	default:
		return false;
	}
}

void CCharacter::CancelInvisible()
{
	m_assist.CancelInvisible();
	if (m_currentSkill && m_currentSkill->m_proto->m_index == 131 && m_currentSkill->m_state == SKILL_CON_READY)
	{
		CNetMsg rmsg;
		SkillCancelMsg(rmsg, this);
		m_pArea->SendToCell(rmsg, this, true);
		m_currentSkill->Cancel(this);
		m_currentSkill = NULL;
	}
}

#ifdef ENABLE_CHANGEJOB
int CCharacter::GetMeleeLevelBonus() const
{
	return (m_type == MSG_CHAR_PC && TO_PC(this)->m_job2) ? MAX((m_level - JOB_2ND_LEVEL), 0) : 0;
}

int CCharacter::GetRangeLevelBonus() const
{
	return (m_type == MSG_CHAR_PC && TO_PC(this)->m_job2) ? MAX((m_level - JOB_2ND_LEVEL), 0) : 0;
}

int CCharacter::GetMagicLevelBonus() const
{
	return (m_type == MSG_CHAR_PC && TO_PC(this)->m_job2) ? MAX((m_level - JOB_2ND_LEVEL), 0) : 0;
}
#endif

void CCharacter::CancelDamageLink()
{
	m_assist.CureAssist(MST_ASSIST_DAMAGELINK, 99);
}

unsigned char CCharacter::GetMapAttr()
{
	return (m_pArea) ? m_pArea->GetAttr(GET_YLAYER(this), GET_X(this), GET_Z(this)) : MAPATT_BLOCK;
}

void CCharacter::ApplyPassiveSkill(CSkill* skill, int param)
{
	// 패시브 없는 캐릭은 스킵
	switch (m_type)
	{
	case MSG_CHAR_PC:
	case MSG_CHAR_ELEMENTAL:
		break;

	case MSG_CHAR_PET:
	case MSG_CHAR_NPC:
	default:
		return ;
	}

	if (skill == NULL)
		return ;

	const CSkillProto* sp;
	const CSkillLevelProto* slp;
	const CMagicProto* mp;
	const CMagicLevelProto* mlp;
	int i;

	sp = skill->m_proto;
	if (sp == NULL)
		return ;
	slp = sp->Level(skill->m_level);
	if (slp == NULL)
		return ;

	// 필요 스탯 검사 : PC
	if (IS_PC(this))
	{
		CPC* pc = TO_PC(this);
		const CSkillLevelProto* level1 = sp->Level(1);
		if (!level1)
			return ;
		if (pc ->m_statpt_str < level1->m_learnstr)
			return ;
		if (pc ->m_statpt_dex < level1->m_learndex)
			return ;
		if (pc ->m_statpt_int < level1->m_learnint)
			return ;
		if (pc ->m_statpt_con < level1->m_learncon)
			return ;
	}

	// 적용 조건 검사
	if (!CanApplySkill(sp, slp))
		return ;

	for (i = 0; i < MAX_SKILL_MAGIC; i++)
	{
		mp = slp->m_magic[i];
		if (mp == NULL)
			return ;
		mlp = mp->Level(slp->m_magicLevel[i]);
		if (mlp == NULL)
			return ;

		int realPowerValue = mlp->m_nPowerValue * param / 100;
		int realHitrateValue = mlp->m_nHitrateValue * param / 100;

	// 효과 적용
#define APPVAL(v)		{ \
						switch (mp->m_damagetype) \
						{ \
						case MDT_ADDITION: \
							m_avPassiveAddition.v += realPowerValue; \
							break; \
						case MDT_RATE: \
							m_avPassiveRate.v += realPowerValue; \
							break; \
						} \
					}

		// 방어 관련은 소환수들도 적용
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
			case MST_STAT_SKILLSPD:			APPVAL(skillspd);				break;
			case MST_STAT_VALOR:			
				{
					if (IS_PC(this))
					{
						CPC* pc = TO_PC(this);
						if (pc->IsParty())
							m_avPassiveAddition.attack += realPowerValue * pc->m_party->GetNearPartyMemberCount(pc) / 10;
					}
				}
				break;
			case MST_STAT_STATPALL:			APPVAL(statpall);				break;
			// << 071211 kjban edit
			case MST_STAT_ATTACK_PER:
				{
					switch (mp->m_damagetype)
					{ 
					case MDT_ADDITION:
						m_avPassiveAddition.attack += this->m_eqMelee * realPowerValue / 100; 
						m_avPassiveAddition.magic += this->m_eqMagic * realPowerValue / 100; 
						break;
					case MDT_RATE:
						m_avPassiveRate.attack += this->m_eqMelee * realPowerValue / 100; 
						m_avPassiveRate.magic += this->m_eqMagic * realPowerValue / 100; 
						break; 
					}
				}
				break;

			case MST_STAT_DEFENSE_PER:		
				{
					switch (mp->m_damagetype)
					{ 
					case MDT_ADDITION:
						m_avPassiveAddition.defense += this->m_eqDefense * realPowerValue / 100; 
						break;
					case MDT_RATE:
						m_avPassiveRate.defense += this->m_eqDefense * realPowerValue / 100; 
						break; 
					}
				}				
				break;

			case MST_STAT_STATPALL_PER:		
				m_avPassiveAddition.statpall_per += realPowerValue;
				break;

			//  [5/15/2008 KwonYongDae]
			case MST_STAT_STR:
				{
					APPVAL(stronger_skill);
				}break;
			case MST_STAT_DEX:
				{
					APPVAL(stronger_skill);
				}break;
			case MST_STAT_INT:
				{
					APPVAL(stronger_skill);
				}break;
			case MST_STAT_CON:
				{
					APPVAL(stronger_skill);
				}break;
			}
			break;
			// >>
		case MT_OTHER:
			switch (mp->m_subtype)
			{
			case MST_OTHER_REFLEX:		APPVAL(hcReflex);
										m_avPassiveAddition.hcReflexProb = realHitrateValue;
										break;
			}
			break;

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

		case MT_REDUCE:
			switch (mp->m_subtype)
			{
			case MST_REDUCE_MELEE:		APPVAL(reduceMelee);
										m_avPassiveAddition.reduceMeleeProb += realHitrateValue;
										break;
			case MST_REDUCE_RANGE:		APPVAL(reduceRange);
										m_avPassiveAddition.reduceRangeProb += realHitrateValue;
										break;
			case MST_REDUCE_MAGIC:		APPVAL(reduceMagic);
										m_avPassiveAddition.reduceMagicProb += realHitrateValue;
										break;
			}
			break;

		case MT_IMMUNE:
			switch (mp->m_subtype)
			{
			case MST_IMMUNE_BLIND:		m_avPassiveAddition.immune_blind = true;		break;
			}
			break;
		}
#undef APPVAL
	}
}

bool CCharacter::CheckHackAttack(CPC* pPC)
{
	if (pPC == NULL)
		return false;
	switch (m_type)
	{
	case MSG_CHAR_PC:
	case MSG_CHAR_PET:
	case MSG_CHAR_ELEMENTAL:
		break;

	default:
		return false;
	}

#ifndef NOCHECK_ATTACKSPEED
	m_hackAttackCount--;
	m_AttackServerTime += m_attackSpeed;
	m_AttackClientTime += gserver.m_pulse - m_lastAttackPulse  + PULSE_HACK_ATTACK_THRESHOLD ;

	if( m_hackAttackCount < 1 )
	{
		if( m_AttackClientTime < m_AttackServerTime )
		{
			GAMELOG << init("HACK ATTACK", pPC)
					<< "CHAR TYPE" << delim
					<< m_type << delim
					<< (gserver.m_pulse - m_lastAttackPulse) << delim
					<< m_AttackClientTime << delim
					<< m_AttackServerTime << end;
#if defined( LC_TWN2 ) || defined (LC_USA)
			// 대만은 핵으로 팅기지 않는다
#else
			if (pPC->m_desc->IncreaseHackCount(1))
				return true;
#endif // #if defined( LC_TWN2 ) 
		}
		m_hackAttackCount = PULSE_HACK_ATTACK_COUNT;
		m_AttackClientTime = 0; 
		m_AttackServerTime = 0;
	}

#endif // NOCHECK_ATTACKSPEED
	m_lastAttackPulse = gserver.m_pulse;

	return false;
}

void CCharacter::SetDisableTime(int sec)
{
	if (m_pulseDisable > gserver.m_pulse + (sec * PULSE_REAL_SEC))
		return ;
	m_pulseDisable = gserver.m_pulse + (sec * PULSE_REAL_SEC);
}

bool CCharacter::IsDisable()
{
	return (m_pulseDisable > gserver.m_pulse);
}

#ifdef NIGHT_SHADOW
bool CCharacter::ChekAttackType()
{
	// true 공속검사 , false 검사 안함
	if( m_attacktype == ATTACK_TYPE_NORMAL )
	{
		m_attackcount = 0;
		return true;
	}
	else if ( m_attacktype == ATTACK_TYPE_DOUBLE )
	{
		m_attackcount++;
		if( m_attackcount < ATTACK_TYPE_DOUBLE )
			return false;
		m_attackcount = 0;
		return true;
	}
	else if ( m_attacktype == ATTACK_TYPE_INFINITY )
	{
		return false;
	}

	return true;
}
#endif //NIGHT_SHADOW