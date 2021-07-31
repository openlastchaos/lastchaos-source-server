#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "DratanCastle.h"

void do_Skill(CPC* ch, CNetMsg& msg)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(ch);
	}
#endif // DRATAN_CASTLE

	if (DEAD(ch) || ch->IsSetPlayerState(PLAYER_STATE_SITDOWN))
		return ;
	
	msg.MoveFirst();
	
	unsigned char subtype;
	
	msg >> subtype;
	
	switch (subtype)
	{
	case MSG_SKILL_LEARN:
		do_SkillLearn(ch, msg);
		break;
		
	case MSG_SKILL_READY:
		do_SkillReady(ch, msg);
		break;
		
	case MSG_SKILL_FIRE:
		do_SkillFire(ch, msg);
		break;
		
	case MSG_SKILL_CANCEL:
		do_SkillCancel(ch, msg);
		break;
#ifdef NEWYEAR_EVENT_2008
	case MSG_SKILL_NEWYEAR_2008:
		do_SkillNewYear2008(ch, msg);
		break;
#endif // NEWYEAR_EVENT_2008

#ifdef HANARO_EVENT
	case MSG_SKILL_HANARO_EVENT:
		do_SkillHanaroEvent(ch, msg);
		break;
#endif
#ifdef FACTORY_SYSTEM
	case MSG_SKILL_LEARN_SEAL:
		do_SealSkillLearn(ch, msg);
		break;
#endif
	}
}

void do_SkillLearn(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	
	try
	{
		unsigned char subtype;
		int skillindex;
		
		msg.MoveFirst();
		
		msg >> subtype
			>> skillindex;
		
		const CSkillProto* proto = gserver.m_skillProtoList.Find(skillindex);
		if (!proto)
			throw MSG_SKILL_LEARN_ERROR_SYSTEM;
		
		CSkillList* list = NULL;
		if (proto->IsActive())
			list = &ch->m_activeSkillList;
		else if (proto->IsPassive())
			list = &ch->m_passiveSkillList;
		else
			list = &ch->m_etcSkillList;
		
		CSkill* skill = list->Find(skillindex);
		int nextLevel = (skill) ? skill->m_level + 1 : 1;
		
		const CSkillLevelProto* levelproto = proto->Level(nextLevel);
		if (!levelproto)
			throw MSG_SKILL_LEARN_ERROR_SYSTEM;
		
		// 직업 검사 : TODO : 전직 이후 처리 : 함수로 만들기
		if (!proto->IsSameJob(ch))
			return ;

		// 레벨 검사
		if (levelproto->m_learnLevel > ch->m_level)
			throw MSG_SKILL_LEARN_ERROR_LEVEL;
		
		// SP 검사
		if (levelproto->m_learnSP * 10000 > ch->m_skillPoint)
			throw MSG_SKILL_LEARN_ERROR_SP;
		
		// 아이템 검사
		int i;
		CItem* item[MAX_SKILL_LEARN_ITEM];
		for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
		{
			item[i] = NULL;
			if (levelproto->m_learnItemIndex[i] < 0)
				continue ;
			int r, c;
			if (!ch->m_invenNormal.FindItem(&r, &c, levelproto->m_learnItemIndex[i], 0, 0))
				throw MSG_SKILL_LEARN_ERROR_ITEM;
			item[i] = ch->m_invenNormal.GetItem(r, c);
			if (!item[i] || item[i]->Count() < levelproto->m_learnItemCount[i])
				throw MSG_SKILL_LEARN_ERROR_ITEM;
		}
		
		// 스킬 검사
		for (i = 0; i < MAX_SKILL_LEARN_SKILL; i++)
		{
			if (levelproto->m_learnSkillIndex[i] < 0)
				continue ;
			const CSkillProto* lproto = gserver.m_skillProtoList.Find(levelproto->m_learnSkillIndex[i]);
			CSkill* lskill;
			if (lproto->IsActive())
				lskill = ch->m_activeSkillList.Find(levelproto->m_learnSkillIndex[i]);
			else if (lproto->IsPassive())
				lskill = ch->m_passiveSkillList.Find(levelproto->m_learnSkillIndex[i]);
			else
				lskill = ch->m_etcSkillList.Find(levelproto->m_learnSkillIndex[i]);
			if (lskill == NULL || lskill->m_level < levelproto->m_learnSkillLevel[i])
				throw MSG_SKILL_LEARN_ERROR_SKILL;
		}

// 050401 : bs : 추가 조건 : 스탯 4종
		if (ch->m_statpt_str < levelproto->m_learnstr)
			throw MSG_SKILL_LEARN_ERROR_STR;
		if (ch->m_statpt_dex < levelproto->m_learndex)
			throw MSG_SKILL_LEARN_ERROR_DEX;
		if (ch->m_statpt_int < levelproto->m_learnint)
			throw MSG_SKILL_LEARN_ERROR_INT;
		if (ch->m_statpt_con < levelproto->m_learncon)
			throw MSG_SKILL_LEARN_ERROR_CON;
// --- 050401 : bs : 추가 조건 : 스탯 4종

#ifdef  JP_OTAKU_SYSTEM
		if ( ch->m_etcEvent & OTAKU_SYSTEM )
			throw MSG_SKILL_LEARN_ERROR_SKILL;
#endif // JP_OTAKU_SYSTEM

		// SP 소모
		ch->m_skillPoint -= levelproto->m_learnSP * 10000;
		
		// 아이템 소모
		for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
		{
			if (item[i] && levelproto->m_learnItemCount[i] > 0)
			{
				DecreaseFromInventory(ch, item[i], levelproto->m_learnItemCount[i]);
				if (item[i]->Count() < 1)
				{
					ItemDeleteMsg(rmsg, item[i]);
					SEND_Q(rmsg, ch->m_desc);
					RemoveFromInventory(ch, item[i], true, true);
				}
				else
				{
					ItemUpdateMsg(rmsg, item[i], -levelproto->m_learnItemCount[i]);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}
		
		// 스킬 추가
		if (skill == NULL)
		{
			// 새로 배움
			skill = gserver.m_skillProtoList.Create(skillindex);
			if (!skill)
				throw MSG_SKILL_LEARN_ERROR_SYSTEM;
			if (proto->IsActive())
				ch->m_activeSkillList.Add(skill);
			else if (proto->IsPassive())
				ch->m_passiveSkillList.Add(skill);
			else
				ch->m_etcSkillList.Add(skill);
		}
		else
		{
			// 있는 스킬 레벨 업
			if (skill->m_level >= proto->m_maxLevel)
				return ;
			skill->m_level++;
		}

		// 소환수 스킬은 소환수에게도 추가 : 액티브만
		if ((proto->m_type == ST_MELEE || proto->m_type == ST_RANGE || proto->m_type == ST_MAGIC) && proto->CheckSorcererFlag(SSF_USE_ALL))
		{
			int needFlag = 0;
			CElemental* p = ch->m_elementalList;
			while (p)
			{
				switch (p->GetElementalType())
				{
				case ELEMENTAL_FIRE:
					needFlag = SSF_USE_FIRE;
					break;
				case ELEMENTAL_WIND:
					needFlag = SSF_USE_WIND;
					break;
				case ELEMENTAL_EARTH:
					needFlag = SSF_USE_EARTH;
					break;
				case ELEMENTAL_WATER:
					needFlag = SSF_USE_WATER;
					break;
				default:
					needFlag = -1;
					break;
				}
				if (needFlag != -1)
				{
					if (proto->CheckSorcererFlag(needFlag))
						p->AddSkill(skillindex, skill->m_level);
				}
				p = p->m_nextElemental;
			}
		}
		else
		{
			CElemental* p = ch->m_elementalList;
			while (p)
			{
				p->CalcStatus(true);
				p = p->m_nextElemental;
			}
		}

		GAMELOG << init("SKILL LEARN", ch)
				<< skill->m_proto->m_index
				<< delim
				<< skill->m_level
				<< end;
		
		SkillLearnMsg(rmsg, skill);
		SEND_Q(rmsg, ch->m_desc);

		if (levelproto)
		{
			for (i = 0; i < MAX_SKILL_MAGIC; i++)
			{
				if (levelproto->m_magic[i])
				{
					switch (levelproto->m_magic[i]->m_type)
					{
					case MT_IMMUNE:
						switch (levelproto->m_magic[i]->m_subtype)
						{
						case MST_IMMUNE_BLIND:
							ch->m_assist.CureAssist(MST_ASSIST_BLIND, 99);
							break;
						}
						break;
					}
				}
			}
		}
		
		ch->CalcStatus(true);
	}
	catch (MSG_SKILL_LEARN_ERROR_TYPE learnerr)
	{
		SkillLearnErrorMsg(rmsg, learnerr);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_SkillReady(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	
	char chartype;
	int charindex;
	int skillindex;
	char targettype;
	int targetindex;
	
	CSkill* skill;
	const CSkillLevelProto* levelproto;
	CCharacter* tch = NULL;
	int i;
	int nFailExtraInfo = 0;

	int nNeedMP = 0;

	CNPC* pNPC = NULL;
	CElemental* pElemental = NULL;
#ifdef ENABLE_PET
	CPet* pPet = NULL;
#endif // #ifdef ENABLE_PET

	CCharacter* chMP;
	CCharacter* charSkill = NULL;
	
	msg >> chartype
		>> charindex
		>> skillindex
		>> targettype
		>> targetindex;

	switch (chartype)
	{
	case MSG_CHAR_NPC:
		if (ch->m_pZone == NULL || !ch->m_pZone->IsPersonalDungeon())
			return ;
		pNPC = TO_NPC(ch->m_pArea->FindCharInCell(ch, charindex, (MSG_CHAR_TYPE)chartype));
		charSkill = pNPC;
		break;

#ifdef ENABLE_PET
	case MSG_CHAR_PET:
		if (ch->m_bImmortal || ch->IsDisable())
			return ;
		pPet = ch->GetPet();
		if (!pPet || !pPet->IsMount() || pPet->m_index != charindex)
			return ;
		// 펫의 스킬을 PC가 사용
		charSkill = pPet;
		break;
#endif // #ifdef ENABLE_PET

	case MSG_CHAR_PC:
		if (ch->m_index != charindex || ch->m_bImmortal || ch->IsDisable())
			return ;
		charSkill = ch;
		break;

	case MSG_CHAR_ELEMENTAL:
		if (ch->m_bImmortal || ch->IsDisable())
			return ;
		pElemental = ch->GetElemental(charindex);
		if (pElemental)
		{
			if (pElemental->GetOwner() != ch)
				return ;
		}
		charSkill = pElemental;
		break;
		
#ifdef ATTACK_PET
	case MSG_CHAR_APET:
		{
			CAPet* apet = ch->GetAPet();
			if( !apet || !apet->IsSummon() )
				return;
			charSkill = apet;
		}
		break;
#endif // ATTACK_PET

	default:
		break;
	}

	if (charSkill == NULL)
		return ;

	try
	{
#ifdef  JP_OTAKU_SYSTEM
		if ( ch->m_etcEvent & OTAKU_SYSTEM )
			throw MSG_SKILL_ERROR_NOTREADY;
#endif // JP_OTAKU_SYSTEM

		// 스킬 찾기
		skill = charSkill->FindSkill(skillindex);
		if (skill == NULL || skill->m_proto == NULL)
			throw MSG_SKILL_ERROR_NOTFOUND;
		levelproto = skill->m_proto->Level(skill->m_level);
		if (levelproto == NULL)
			throw MSG_SKILL_ERROR_NOTFOUND;

#ifdef RESTRICT_PVP_SKILL
		if (IS_PC(charSkill) && charSkill == ch)
		{
			// 인비저빌리티
			if (skill->m_proto->m_index == 131)
			{
				if (ch->m_nRestrictPvPSkillDelaySec > 0)
					throw MSG_SKILL_ERROR_CANNOT_INVISIBLE;
			}
			// 데스 모션
			else if (skill->m_proto->m_index == 191)
			{
				if (!IS_IN_CELL(ch))
					throw MSG_SKILL_ERROR_CANNOT_DEATHMOTION_ANOTHER_PC;
				
				int sx = ch->m_cellX - 1;
				int ex = ch->m_cellX + 1;
				int sz = ch->m_cellZ - 1;
				int ez = ch->m_cellZ + 1;
				
				int x, z;
				
				for (x = sx; x <= ex; x++)
				{
					if (x < 0 || x >= ch->m_pArea->m_size[0]) continue;
					for (z = sz; z <= ez; z++)
					{
						if (z < 0 || z >= ch->m_pArea->m_size[1]) continue;
						CCharacter *pChar;
						for (pChar = ch->m_pArea->m_cell[x][z].m_listChar; pChar; pChar = pChar->m_pCellNext)
						{
							if (IS_PC(pChar) && pChar != ch)
							{
								if (GetDistance(pChar, ch) > 15.0f)
									continue ;

								CPC* pTestPC = TO_PC(pChar);
								if (pTestPC->m_admin > 1)
									continue ;
								if (pTestPC->m_party != NULL && ch->m_party == pTestPC->m_party)
									continue ;
								if (pTestPC->m_guildInfo && pTestPC->m_guildInfo->guild() && ch->m_guildInfo && pTestPC->m_guildInfo->guild() == ch->m_guildInfo->guild())
									continue ;
								throw MSG_SKILL_ERROR_CANNOT_DEATHMOTION_ANOTHER_PC;
							}
						}
					}
				}
			}
		}
#endif // RESTRICT_PVP_SKILL

		if ( IS_PC(charSkill) )
		{
			// 존검사 추가 //  [2/20/2009 KwonYongDae]
#ifdef LC_BRZ
			if( charSkill->m_pZone && charSkill->m_pZone->IsOXQuizRoom() )
				throw MSG_SKILL_ERROR_CANNOT_SPELL;
#endif // LC_BRZ
			// 스탯 조건 검사
			const CSkillLevelProto* level1 = skill->m_proto->Level(1);
			if (!level1)
				throw MSG_SKILL_ERROR_NOTFOUND;
			if (ch->m_statpt_str < level1->m_learnstr)
				throw MSG_SKILL_ERROR_STATPOINT_STR;
			if (ch->m_statpt_dex < level1->m_learndex)
				throw MSG_SKILL_ERROR_STATPOINT_DEX;
			if (ch->m_statpt_int < level1->m_learnint)
				throw MSG_SKILL_ERROR_STATPOINT_INT;
			if (ch->m_statpt_con < level1->m_learncon)
				throw MSG_SKILL_ERROR_STATPOINT_CON;

			// 퍼펙트 바디
			if (skillindex == 152 && ch->m_hp > ch->m_maxHP / 2)
				throw MSG_SKILL_ERROR_CANNOT_SPELL;
			// 프렌지
			if (skillindex == 153 && ch->m_hp > ch->m_maxHP / 2)
				throw MSG_SKILL_ERROR_CANNOT_SPELL;
			// 버서크
			if (skillindex == 228 && ch->m_hp > ch->m_maxHP / 2)
				throw MSG_SKILL_ERROR_CANNOT_SPELL;

			// 소환, 강신, 테이밍 검사
			if (levelproto->m_magic[0] && levelproto->m_magic[0]->m_levelproto[0])
			{
#ifdef NIGHTSHADOW_SKILL
				// 테이밍 스킬은 두마리 이상 사용 못한다. 현재 사용중이면 취소
				if ( levelproto->m_magic[0]->m_type == MT_ASSIST )
				{
					if (levelproto->m_magic[0]->m_subtype == MST_ASSIST_TAMING)
					{
						CPC* pPC = TO_PC(charSkill);
						if ( pPC && pPC->m_bTaming_npc )
						{
							throw MSG_SKILL_ERROR_CANNOT_SPELL;
						}
					}
				}
#endif // NIGHTSHADOW_SKILL
				if (levelproto->m_magic[0]->m_type == MT_OTHER)
				{
					if (levelproto->m_magic[0]->m_subtype == MST_OTHER_SUMMON)
					{
						// 스페셜리스트는 소환 불능
						if (ch->m_job == JOB_SORCERER && ch->m_job2 == JOB_2ND_SPECIALLIST)
							throw MSG_SKILL_ERROR_NOTFOUND;

						// 전직 이후에는 폴암 착용시에만 가능
						if (ch->m_job == JOB_SORCERER && ch->m_job2 == JOB_2ND_ELEMENTALLIST)
						{
							if (!ch->m_wearing[WEARING_WEAPON])
								throw MSG_SKILL_ERROR_INVAL_STATE_WEAPON;
							if (!ch->m_wearing[WEARING_WEAPON]->IsWeaponType())
								throw MSG_SKILL_ERROR_INVAL_STATE_WEAPON;
							if (ch->m_wearing[WEARING_WEAPON]->m_itemProto->m_subtypeIdx != IWEAPON_POLEARM)
								throw MSG_SKILL_ERROR_INVAL_STATE_WEAPON;
						}

						// 소환까지 남은 시간이 있으면 오류 정보에 남은 시간을 보냄
						bool bCheckType;
						char nCheckDuplication;
						if (!ch->CanSummonElemental(levelproto->m_magic[0]->m_levelproto[0]->m_nPowerValue, &nCheckDuplication, &nFailExtraInfo, &bCheckType))
						{
							if (nFailExtraInfo > 0)
								throw MSG_SKILL_ERROR_ELEMENTAL_TIME;
							if (bCheckType)
								throw MSG_SKILL_ERROR_NOTFOUND;
							throw (MSG_SKILL_ERROR_TYPE)(MSG_SKILL_ERROR_ELEMENTAL_FIRE + nCheckDuplication);
						}
					}

					else if (levelproto->m_magic[0]->m_subtype == MST_OTHER_EVOCATION)
					{
						// 강신 상태 검사
						if (ch->m_evocationType != EVOCATION_NONE)
							throw MSG_SKILL_ERROR_EVOCATION_ALREADY;

						// 시간 검사
						if (!levelproto->m_magic[0]->Level(1))
							throw MSG_SKILL_ERROR_NOTFOUND;
						switch (levelproto->m_magic[0]->Level(1)->m_nPowerValue)
						{
						case EVOCATION_HELLOUND:
							if (ch->m_pulseEvocation[0] > 0 && ch->m_pulseEvocation[0] + EVOCATION_DURATION <= gserver.m_pulse && ch->m_pulseEvocation[0] + EVOCATION_DURATION + EVOCATION_DELAY > gserver.m_pulse)
							{
								nFailExtraInfo = (ch->m_pulseEvocation[0] + EVOCATION_DURATION + EVOCATION_DELAY - gserver.m_pulse) / PULSE_REAL_SEC;
								throw MSG_SKILL_ERROR_EVOCATION_TIME;
							}
							break;
						case EVOCATION_ELENEN:
							if (ch->m_pulseEvocation[1] > 0 && ch->m_pulseEvocation[1] + EVOCATION_DURATION <= gserver.m_pulse && ch->m_pulseEvocation[1] + EVOCATION_DURATION + EVOCATION_DELAY > gserver.m_pulse)
							{
								nFailExtraInfo = (ch->m_pulseEvocation[1] + EVOCATION_DURATION + EVOCATION_DELAY - gserver.m_pulse) / PULSE_REAL_SEC;
								throw MSG_SKILL_ERROR_EVOCATION_TIME;
							}
							break;
						default:
							throw MSG_SKILL_ERROR_NOTFOUND;
						}
					}
				}
			}

			// 사용 조건 검사 : 아이템
			for (i = 0; i < MAX_SKILL_NEED_ITEM; i++)
			{
				int r, c;
				
				if (levelproto->m_needItemIndex[i] < 0)
					continue ;
				
				if (!ch->m_invenNormal.FindItem(&r, &c, levelproto->m_needItemIndex[i], 0, 0))
					throw MSG_SKILL_ERROR_INSUFF_ITEM;
				
				CItem* item = ch->m_invenNormal.GetItem(r, c);
				
				if (item->Count() < levelproto->m_needItemCount[i])
					throw MSG_SKILL_ERROR_INSUFF_ITEM;
			}
		} // PC의 조건 검사

#ifdef ATTACK_PET
		if( IS_APET(charSkill) )
		{
			// 사용 조건 검사 : 아이템
			for (i = 0; i < MAX_SKILL_NEED_ITEM; i++)
			{
				int r, c;
				
				if (levelproto->m_needItemIndex[i] < 0)
					continue ;
				
				if (!ch->m_invenNormal.FindItem(&r, &c, levelproto->m_needItemIndex[i], 0, 0))
					throw MSG_SKILL_ERROR_INSUFF_ITEM;
				
				CItem* item = ch->m_invenNormal.GetItem(r, c);
				
				if (item->Count() < levelproto->m_needItemCount[i])
					throw MSG_SKILL_ERROR_INSUFF_ITEM;
			}
		}
#endif // ATTACK_PET

		// 펫 스킬은 찾고 나서 스킬 사용자를 PC로 변경
		if (IS_PET(charSkill))
			charSkill = ch;

		if (charSkill == NULL || charSkill->m_pArea == NULL)
			throw MSG_SKILL_ERROR_NOTREADY;

		// 스킬 사용 가능 상태 검사
		if (!charSkill->CanSpell())
			throw MSG_SKILL_ERROR_CANNOT_SPELL;

		// 현재 스킬 사용 중인지 검사
		if (charSkill->m_currentSkill)
		{
			if (gserver.m_pulse - charSkill->m_currentSkill->m_usetime >= charSkill->m_currentSkill->m_proto->m_readyTime + charSkill->m_currentSkill->m_proto->m_stillTime + charSkill->m_currentSkill->m_proto->m_fireTime - 5)
			{
				charSkill->m_currentSkill->Cancel(charSkill);
				charSkill->m_currentSkill = NULL;
			}
			else
				throw MSG_SKILL_ERROR_ALREADY_USE;
		}

		// 스킬 상태 보기 : 재사용 대기 중인지
		if (!skill->IsReady(charSkill))
			throw MSG_SKILL_ERROR_NOTREADY;

		// 소환수는 MP 공유
		chMP = (IS_ELEMENTAL(charSkill)) ? ch : charSkill;

		// 사용 조건 검사 : hp
		if (charSkill->m_hp <= levelproto->m_needHP)
			throw MSG_SKILL_ERROR_INSUFF_HP;
		
		// 사용 조건 검사 : mp
		nNeedMP = levelproto->m_needMP;
		if (charSkill == ch)
		{
			if (ch->m_opDecreaseSkillMP > 0)
				nNeedMP -= nNeedMP * ch->m_opDecreaseSkillMP / 100;
		}
		if (chMP->m_mp <= nNeedMP)
			throw MSG_SKILL_ERROR_INSUFF_MP;

		// 사용 조건 검사 : use state
		switch (skill->m_proto->CheckUseState(charSkill))
		{
		case 0:
			break;
		case SCT_DEATH:
			throw MSG_SKILL_ERROR_INVAL_STATE_DEATH;
		case SCT_WEAPON:
			throw MSG_SKILL_ERROR_INVAL_STATE_WEAPON;
		case SCT_SHIELD:
			throw MSG_SKILL_ERROR_INVAL_STATE_SHIELD;
		case SCT_SITDOWN:
			throw MSG_SKILL_ERROR_INVAL_STATE_STAND;
		case SCT_STAND:
			throw MSG_SKILL_ERROR_INVAL_STATE_SITDOWN;
		case SCT_PEACEZONE:
			throw MSG_SKILL_ERROR_INVAL_STATE_PEACEZONE;
		default:
			throw MSG_SKILL_ERROR_INVAL_STATE_MAGIC;
		}

		if (skill->m_proto->CheckSorcererFlag(SSF_USE_ALL))
		{
			switch (charSkill->m_type)
			{
			case MSG_CHAR_PC:
				switch (ch->m_evocationType)
				{
				case EVOCATION_NONE:
					if (!skill->m_proto->CheckSorcererFlag(SSF_USE_CHARACTER))
						throw MSG_SKILL_ERROR_NOTFOUND;
					break;
				case EVOCATION_HELLOUND:
					if (!skill->m_proto->CheckSorcererFlag(SSF_USE_HELLOUND))
						throw MSG_SKILL_ERROR_NOTFOUND;
					break;
				case EVOCATION_ELENEN:
					if (!skill->m_proto->CheckSorcererFlag(SSF_USE_ELENEN))
						throw MSG_SKILL_ERROR_NOTFOUND;
					break;
				default:
					throw MSG_SKILL_ERROR_NOTFOUND;
				}
				break;

			case MSG_CHAR_PET:
			case MSG_CHAR_NPC:
				if (!skill->m_proto->CheckSorcererFlag(SSF_USE_CHARACTER))
					throw MSG_SKILL_ERROR_NOTFOUND;
				break;

			case MSG_CHAR_ELEMENTAL:
				switch (pElemental->GetElementalType())
				{
				case ELEMENTAL_FIRE:
					if (!skill->m_proto->CheckSorcererFlag(SSF_USE_FIRE))
						throw MSG_SKILL_ERROR_NOTFOUND;
					break;
				case ELEMENTAL_WIND:
					if (!skill->m_proto->CheckSorcererFlag(SSF_USE_WIND))
						throw MSG_SKILL_ERROR_NOTFOUND;
					break;
				case ELEMENTAL_EARTH:
					if (!skill->m_proto->CheckSorcererFlag(SSF_USE_EARTH))
						throw MSG_SKILL_ERROR_NOTFOUND;
					break;
				case ELEMENTAL_WATER:
					if (!skill->m_proto->CheckSorcererFlag(SSF_USE_WATER))
						throw MSG_SKILL_ERROR_NOTFOUND;
					break;
				default:
					throw MSG_SKILL_ERROR_NOTFOUND;
				}
				break;

			default:
				throw MSG_SKILL_ERROR_NOTFOUND;
			}
		}

		// 타겟 검사
		// 거리 검사
		switch (skill->m_proto->m_targetType)
		{
		case STT_TARGET_ONE:
		case STT_TARGET_RANGE:
		case STT_TARGET_D120:
		case STT_TARGET_RECT:
		case STT_PARTY_ONE:
		case STT_ELEMENTAL_ONE:
			tch = charSkill->m_pArea->FindCharInCell(charSkill, targetindex, (MSG_CHAR_TYPE)targettype, true);
			if (tch == NULL)
				throw MSG_SKILL_ERROR_INVAL_TARGET;
			
#ifdef NIGHTSHADOW_SKILL
			// 테이밍이 되었거나 혼란 중인 몬스터에게는 어떤 스킬도 사용할 수 없다.
			if ( IS_NPC(tch) )
			{
				CNPC* pNPC = TO_NPC(tch);
				if ( pNPC && ( pNPC->Check_MobFlag(STATE_MONSTER_TAMING) || pNPC->Check_MobFlag(STATE_MONSTER_CONFUSION) ) )
				{
					throw MSG_SKILL_ERROR_INVAL_TARGET;
				}
			}
#endif // NIGHTSHADOW_SKILL

			switch (charSkill->m_type)
			{
			case MSG_CHAR_PC:
			case MSG_CHAR_PET:
			case MSG_CHAR_ELEMENTAL:
				if( !ch->IsEnemy(tch) && (skill->m_proto->m_flag & SF_NOTHELP) && ch->CanPvP(tch, false))
					throw MSG_SKILL_ERROR_TARGET_PKLIMITLEVEL;

				if (skill->m_proto->m_targetType == STT_PARTY_ONE)
				{
					if (!(IS_PC(tch) && ch->IsParty() && TO_PC(tch)->IsParty() && ch->m_party == TO_PC(tch)->m_party))
						throw MSG_SKILL_ERROR_INVAL_TARGET;
				}

				// 소환수 전용 스킬 타입이면
				else if (skill->m_proto->m_targetType == STT_ELEMENTAL_ONE)
				{
					// 대상이 소환수여야 하고
					if (!IS_ELEMENTAL(tch))
						throw MSG_SKILL_ERROR_INVAL_TARGET;
					CElemental* pElemental = TO_ELEMENTAL(tch);
					// 대상의 주인PC가 있어야 하고
					CPC* pTargetPC = pElemental->GetOwner();
					if (pTargetPC == NULL)
						throw MSG_SKILL_ERROR_INVAL_TARGET;
					// 대상의 주인과 스킬 시전자가 동일
					if (pTargetPC != ch/* && (!pTargetPC->IsParty() || !ch->IsParty() || pTargetPC->m_party != ch->m_party*/)
						throw MSG_SKILL_ERROR_INVAL_TARGET;
				}
				break;

			case MSG_CHAR_NPC:
				if (skill->m_proto->m_targetType == STT_PARTY_ONE)
				{
					if (!(IS_NPC(tch) && pNPC->m_proto->m_family != -1 && pNPC->m_proto->m_family == TO_NPC(tch)->m_proto->m_family))
						return ;
				}
				break;

#ifdef ATTACK_PET
			case MSG_CHAR_APET:
				{

				}
				break;
#endif //ATTACK_PET

			default:
				break;
			}

			if (skill->m_proto->GetFireRange(charSkill) * 2 < GetDistance(charSkill, tch))
				throw MSG_SKILL_ERROR_LONG;

			break;
		}
		
		// 스킬 시전 상태로
		charSkill->CancelInvisible();
		skill->Ready(tch);
		charSkill->m_currentSkill = skill;
		
		SkillReadyMsg(rmsg, charSkill, skill, tch);
		charSkill->m_pArea->SendToCell(rmsg, charSkill, true);
	}
	catch (MSG_SKILL_ERROR_TYPE ex)
	{
		switch (chartype)
		{
		case MSG_CHAR_NPC:
			return ;
		default:
			break;
		}
		SkillErrorMsg(rmsg, ex, skillindex, nFailExtraInfo);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_SkillCancel(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	
	char chartype;
	int charindex;

	CCharacter* charSkill = NULL;
	
	msg >> chartype
		>> charindex;

	switch (chartype)
	{
	case MSG_CHAR_NPC:
		if (ch->m_pArea == NULL)
			return ;
		charSkill = ch->m_pArea->FindCharInCell(ch, charindex, (MSG_CHAR_TYPE)chartype);
		break;

#ifdef ENABLE_PET
	case MSG_CHAR_PET:
		// 펫의 스킬을 PC가 사용
		charSkill = ch;
		break;
#endif // #ifdef ENABLE_PET

	case MSG_CHAR_PC:
		charSkill = ch;
		break;

	case MSG_CHAR_ELEMENTAL:
		charSkill = ch->m_pArea->FindCharInCell(ch, charindex, (MSG_CHAR_TYPE)chartype);
		if (charSkill)
		{
			if (TO_ELEMENTAL(charSkill)->GetOwner() != ch)
				return ;
		}
		break;

#ifdef ATTACK_PET
	case MSG_CHAR_APET :
		charSkill = ch;
		break;
#endif // ATTACK_PET

	default:
		break;
	}
	
	// 스킬 사용 중인가? 사용중이라면 시전상태인가?
	if (charSkill->m_currentSkill == NULL || charSkill->m_currentSkill->m_state != SKILL_CON_READY)
		return ;
	
	// 시전 취소
	charSkill->m_currentSkill->Cancel(charSkill);
	charSkill->m_currentSkill = NULL;
	
	SkillCancelMsg(rmsg, charSkill);
	charSkill->m_pArea->SendToCell(rmsg, charSkill, true);
}

void do_SkillFire(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	
	char chartype;
	int charindex;
	int skillindex;
	char targettype;
	int targetindex;
	char count = 0;
	char* mtargettype = NULL;
	int* mtargetindex = NULL;
	
	CSkill* skill;
	const CSkillProto* proto;
	const CSkillLevelProto* levelproto;
	const CSkillLevelProto* level1;
	CItem* item[MAX_SKILL_NEED_ITEM];
	CCharacter* tch = NULL;

	int nDummySkillSpeed = 0;
	char cMoveChar = 0;
	float fx = GET_X(ch);
	float fz = GET_Z(ch);
	float fh = GET_H(ch);
	float fr = GET_R(ch);
	char cYlayer = GET_YLAYER(ch);

	int nNeedMP = 0;
	
	int i;
	
	msg >> chartype
		>> charindex
		>> skillindex
		>> targettype
		>> targetindex
		>> count;

	CCharacter* chMP;
	CCharacter* charSkill = NULL;

	if (ch->m_pArea == NULL)
		return ;

	switch (chartype)
	{
	case MSG_CHAR_NPC:
		if (!ch->m_pZone->IsPersonalDungeon())
			return ;
		charSkill = ch->m_pArea->FindCharInCell(ch, charindex, (MSG_CHAR_TYPE)chartype);
		break;

#ifdef ENABLE_PET
	case MSG_CHAR_PET:
		if (ch->m_bImmortal || ch->IsDisable())
			return ;
		charSkill = ch;
		break;
#endif // #ifdef ENABLE_PET

	case MSG_CHAR_PC:
		if (ch->m_bImmortal || ch->IsDisable())
			return ;
		charSkill = ch;
		if (charindex != ch->m_index)
			return ;
		break;

	case MSG_CHAR_ELEMENTAL:
		if (ch->m_bImmortal || ch->IsDisable())
			return ;
		charSkill = ch->GetElemental(charindex);
		break;
#ifdef ATTACK_PET
	case MSG_CHAR_APET:
		{
			CAPet* apet = ch->GetAPet();
			if( apet )
			{
				if( apet->IsSummon() )
				{
					charSkill = apet;
				}break;
			}
			return;
		}
		break;
#endif // ATTACK_PET

	default:
		return ;
	}

	if (!charSkill)
		return ;

	// 스킬 시전중 검사
	if (charSkill->m_currentSkill == NULL
		|| charSkill->m_currentSkill->m_proto->m_index != skillindex
		|| charSkill->m_currentSkill->m_state != SKILL_CON_READY)
		goto INSUFF;
	
	if ((skill = charSkill->m_currentSkill) == NULL)
		goto INSUFF;
	if ((proto = skill->m_proto) == NULL)
		goto INSUFF;
	if ((levelproto = proto->Level(skill->m_level)) == NULL)
		goto INSUFF;

	if (proto->m_targetNum < count)
		count = proto->m_targetNum;

	if (count > 0)
	{
		mtargettype = new char[count];
		mtargetindex = new int[count];
		for (i = 0; i < count; i++)
		{
			msg >> mtargettype[i] >> mtargetindex[i];

			int j;
			for (j = 0; j < i; j++)
			{
				if (mtargettype[i] == mtargettype[j] && mtargetindex[i] == mtargetindex[j])
				{
					GAMELOG << init("HACK ATTACK MULTI TARGET", ch)
							<< "ZONE" << delim
							<< ch->m_pZone->m_index << delim
							<< "TARGET" << delim
							<< mtargettype[i] << delim
							<< mtargetindex[i]
							<< end;
					delete [] mtargettype;
					delete [] mtargetindex;
					mtargettype = NULL;
					mtargetindex = NULL;
					count = 0;
					if (ch->m_desc->IncreaseHackCount(1))
						return ;
					break;
				}
			}
//			if (count < 1)
//				break;
		}
	}
	else
		count = 0;

	msg >> nDummySkillSpeed
		>> cMoveChar;
	if (cMoveChar)
	{
		msg >> fx
			>> fz
			>> fh
			>> fr
			>> cYlayer;
	}

	if (IS_PC(charSkill))
	{
		// 사용 조건 : 스탯 : 스킬 레벨 설정에서 1레벨 기준
		level1 = skill->m_proto->Level(1);
		if (!level1)
			goto INSUFF;
		if (ch->m_statpt_str < level1->m_learnstr)
			goto INSUFF;
		if (ch->m_statpt_dex < level1->m_learndex)
			goto INSUFF;
		if (ch->m_statpt_int < level1->m_learnint)
			goto INSUFF;
		if (ch->m_statpt_con < level1->m_learncon)
			goto INSUFF;

		// 사용 조건 검사 : 아이템
		for (i = 0; i < MAX_SKILL_NEED_ITEM; i++)
		{
			int r, c;
			
			item[i] = NULL;
			
			if (levelproto->m_needItemIndex[i] < 0)
				continue ;
			
			if (!ch->m_invenNormal.FindItem(&r, &c, levelproto->m_needItemIndex[i], 0, 0))
				goto INSUFF;
			
			item[i] = ch->m_invenNormal.GetItem(r, c);
			
			if (item[i]->Count() < levelproto->m_needItemCount[i])
				goto INSUFF;
		}
	}

	// 타겟 검사
	// 이전 타겟이 있고
	if (skill->m_targetIndex != -1)
	{
		// 현재 타겟을 구해서
		tch = charSkill->m_pArea->FindCharInCell(charSkill, targetindex, (MSG_CHAR_TYPE)targettype, true);
		// 타겟이 없거나 이전과 다르면
		if (tch == NULL || skill->m_targetIndex != targetindex || skill->m_targetType != targettype)
			goto INSUFF;
	}

	// 소환수는 MP 공유
	chMP = (IS_ELEMENTAL(charSkill)) ? ch : charSkill;

	// 사용 조건 검사 : hp
	if (charSkill->m_hp <= levelproto->m_needHP)
		goto INSUFF;
	
	// 사용 조건 검사 : mp
	nNeedMP = levelproto->m_needMP;
	if (charSkill == ch)
	{
		if (ch->m_opDecreaseSkillMP > 0)
			nNeedMP -= nNeedMP * ch->m_opDecreaseSkillMP / 100;
	}
	if (chMP->m_mp <= nNeedMP)
		goto INSUFF;
	
	// 사용 조건 검사 : use state
	// 하이드는 파이어 행동시 검사하지 않음
	if( skill->m_proto->m_index == 237
		|| skill->m_proto->m_index == 236 ) ;
	else
	{
		switch (skill->m_proto->CheckUseState(charSkill))
		{
		case 0:
			break;
		default:
			goto INSUFF;
		}
	}

#ifdef NIGHTSHADOW_SKILL
	// 사용 조건 검사 : Soul 사용 여부
	if (IS_PC(charSkill))
	{
		// 영혼이 소모되는 스킬이라면
		if ( proto->m_soul_consum > 0 )
		{
			CPC* pPC = TO_PC(charSkill);
			int soul_count = 0;		// 영혼의 소유 개수
			int remain_soul = 0;	// 영혼의 남은 개수

			if ( pPC )
			{
				soul_count = pPC->GetSoul_Count();
				remain_soul = soul_count - proto->m_soul_consum;

				// 영혼의 개수가 마이너스가 되면 스킬 취소
				if ( remain_soul >= 0 )
					pPC->SetSoul_Count(remain_soul);
				else
				{
					GAMELOG << init("SetSoul_Count MINUS ERROR")
						<< "USER_ID" << delim << pPC->m_desc->m_idname << delim
						<< "USER_INDEX" << delim << pPC->m_desc->m_index << delim << end;
					pPC->SetSoul_Count(0);
					goto INSUFF;
				}
			}
		}
	}
#endif // NIGHTSHADOW_SKILL
	
	// 스킬 파이어
	skill->Fire();
	
	if (cMoveChar && tch)
	{
		if (GetDistance(fx, fz, fh, tch) <= CELL_SIZE * 2.0f)
		{
			charSkill->m_pArea->MoveChar(charSkill, cYlayer, fx, fz, fh , fr, MSG_MOVE_STOP, NULL);
		}
	}

	SkillFireMsg(rmsg, charSkill, skill, tch, count, mtargettype, mtargetindex, cMoveChar, fx, fz, fh, fr, cYlayer);
	charSkill->m_pArea->SendToCell(rmsg, charSkill, true);
	
	// HP 소모, MP 소모
	charSkill->m_hp -= levelproto->m_needHP;
	chMP->m_mp -= nNeedMP;
	switch (charSkill->m_type)
	{
	case MSG_CHAR_PC:
	case MSG_CHAR_ELEMENTAL:
		ch->m_bChangeStatus = true;
		break;
#ifdef ATTACK_PET
	case MSG_CHAR_APET:
		ExAPetStatusMsg(rmsg, TO_APET(charSkill) );
		charSkill->m_pArea->SendToCell(rmsg, charSkill, true);
		break;
#endif //ATTACK_PET
	default:
		CharStatusMsg(rmsg, charSkill, 0);
		charSkill->m_pArea->SendToCell(rmsg, charSkill, true);
		break;
	}
	
	// ITEM 소모
	if (IS_PC(charSkill))
	{
		for (i = 0; i < MAX_SKILL_NEED_ITEM; i++)
		{
			if (item[i] && levelproto->m_needItemCount[i] > 0)
			{
				DecreaseFromInventory(ch, item[i], levelproto->m_needItemCount[i]);
				if (item[i]->Count() < 1)
				{
					ItemDeleteMsg(rmsg, item[i]);
					SEND_Q(rmsg, ch->m_desc);
					RemoveFromInventory(ch, item[i], true, true);
				}
				else
				{
					ItemUpdateMsg(rmsg, item[i], -levelproto->m_learnItemCount[i]);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}
	}

#ifdef NEWYEAR_EVENT_2008
	if(skill && skill->m_proto->m_index == 491)
	{
		ApplyNewYearSkill(TO_PC(charSkill), TO_PC(tch));
		return;
	}
#endif // NEWYEAR_EVENT_2008

	// 스킬 적용
	// 타게팅에 따라
	bool bApply;
	switch (proto->m_targetType)
	{
	case STT_SELF_ONE:
		ApplySkill(charSkill, charSkill, skill, -1, bApply);
		break;

	case STT_TARGET_ONE:
		if (tch)
			ApplySkill(charSkill, tch, skill, -1, bApply);
		break;

	case STT_SELF_RANGE:
		if (ApplySkill(charSkill, charSkill, skill, -1, bApply) != -1)
		{
			for (i = 0; i < count; i++)
			{
				tch = charSkill->m_pArea->FindCharInCell(charSkill, mtargetindex[i], (MSG_CHAR_TYPE)mtargettype[i], true);
				if (IS_NPC(charSkill) || (tch && GetDistance(charSkill, tch) <= proto->m_appRange + 1.0f))
				{
#ifdef MARGADUM_PVP
					// 081216 PVP존에서 관중석에 범위마법 데미지 들어가는 것 수정  yhj
					if( tch->m_pZone->m_index == ZONE_PK_TOURNAMENT && tch->GetMapAttr() != MAPATT_FREEPKZONE )
					{
						count++;
						continue;
					}
#endif // MARGADUM_PVP

					if (ApplySkill(charSkill, tch, skill, -1, bApply) == -1)
						break;
				}
			}
		}
		break;

	case STT_TARGET_RANGE:
		if (tch)
		{
			float tx = GET_X(tch);
			float tz = GET_Z(tch);
			float th = GET_H(tch);

			if (ApplySkill(charSkill, tch, skill, -1, bApply) != -1)
			{
				for (i = 0; i < count; i++)
				{
					tch = charSkill->m_pArea->FindCharInCell(charSkill, mtargetindex[i], (MSG_CHAR_TYPE)mtargettype[i], true);
					if (IS_NPC(charSkill) || (tch && GetDistance(tx, tz, th, tch) <= proto->m_appRange + 1.0f))
					{
#ifdef MARGADUM_PVP
						// 081216 PVP존에서 관중석에 범위마법 데미지 들어가는 것 수정  yhj
						if( tch->m_pZone->m_index == ZONE_PK_TOURNAMENT && tch->GetMapAttr() != MAPATT_FREEPKZONE )
						{
							count++;
							continue;
						}
#endif // MARGADUM_PVP

						if (ApplySkill(charSkill, tch, skill, -1, bApply) == -1)
							break;
					}
				}
			}
		}
		break;

	case STT_TARGET_RECT:
	case STT_TARGET_D120:
		if (tch)
		{
			if (ApplySkill(charSkill, tch, skill, -1, bApply) != -1)
			{
				for (i = 0; i < count; i++)
				{
					tch = charSkill->m_pArea->FindCharInCell(charSkill, mtargetindex[i], (MSG_CHAR_TYPE)mtargettype[i], true);
					if (IS_NPC(charSkill) || (tch && GetDistance(charSkill, tch) <= proto->m_appRange + 1.0f))
					{
#ifdef MARGADUM_PVP
						// 081216 PVP존에서 관중석에 범위마법 데미지 들어가는 것 수정  yhj
						if( tch->m_pZone->m_index == ZONE_PK_TOURNAMENT && tch->GetMapAttr() != MAPATT_FREEPKZONE )
						{
							count++;
							continue;
						}
#endif // MARGADUM_PVP

						if (ApplySkill(charSkill, tch, skill, -1, bApply) == -1)
							break;
					}
				}
			}
		}
		break;

	case STT_PARTY_ONE:
	case STT_ELEMENTAL_ONE:
		switch (charSkill->m_type)
		{
		case MSG_CHAR_PC:
		case MSG_CHAR_PET:
		case MSG_CHAR_ELEMENTAL:
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

				default:
					break;
				}
				if (tpc)
				{
					if (ch->IsParty() && tpc->IsParty() && ch->m_party == tpc->m_party)
						ApplySkill(charSkill, tch, skill, -1, bApply);
				}
			}
			break;

		case MSG_CHAR_NPC:
			if (IS_NPC(tch))
			{
				CNPC* tnpc = TO_NPC(tch);
				if (!(TO_NPC(charSkill)->m_proto->m_family != -1 && TO_NPC(charSkill)->m_proto->m_family == tnpc->m_proto->m_family))
					ApplySkill(charSkill, tch, skill, -1, bApply);
			}
			break;

		default:
			break;
		}
		break;

	case STT_PARTY_ALL:
		switch (charSkill->m_type)
		{
		case MSG_CHAR_PC:
			ApplySkillParty(ch, skill, -1, bApply);
			break;

		case MSG_CHAR_NPC:
			if (TO_NPC(charSkill)->m_proto->m_family != -1)
			{
				int sx = charSkill->m_cellX - CELL_EXT;
				int ex = charSkill->m_cellX + CELL_EXT;
				int sz = charSkill->m_cellZ - CELL_EXT;
				int ez = charSkill->m_cellZ + CELL_EXT;

				int x, z;

				for (x = sx; x <= ex; x++)
				{
					if (x < 0 || x >= charSkill->m_pArea->m_size[0]) continue;
					
					for (z = sz; z <= ez; z++)
											{
						if (z < 0 || z >= charSkill->m_pArea->m_size[1]) continue;
						
						CCharacter *pChar;
						CCharacter *pCharNext = charSkill->m_pArea->m_cell[x][z].m_listChar;
						
						while ((pChar = pCharNext))
						{
							pCharNext = pCharNext->m_pCellNext;

							if (IS_NPC(pChar) && TO_NPC(pChar)->m_proto->m_family == TO_NPC(charSkill)->m_proto->m_family)
							{
								if (ApplySkill(charSkill, pChar, skill, -1, bApply) == -1)
									break;
							}
						}
					}
				}
			}
			break;

		default:
			break;
		}
		break;
		
	default:
		goto INSUFF;
	}
	
	goto EXITFUNC;
	
INSUFF:
	// 시전 취소
	if (charSkill->m_currentSkill)
	{
		charSkill->m_currentSkill->Cancel(charSkill);
		charSkill->m_currentSkill = NULL;
	}
	
	SkillCancelMsg(rmsg, charSkill);
	charSkill->m_pArea->SendToCell(rmsg, charSkill, true);
	
EXITFUNC:
	// 불의정령이 Soul of Fire 시전하면 불의 정령 사망
	if (skillindex == 324)
	{
		if (IS_ELEMENTAL(charSkill))
		{
			ch->UnsummonElemental(TO_ELEMENTAL(charSkill));
		}
	}

#ifdef ENABLE_ROGUE_SKILL125_BRZ
	if(skillindex == 125)
	{
		if(!bApply)
		{
			// 로그 와일드 라쏘 스킬 실패
			CNetMsg rmsg;
			SkillErrorMsg(rmsg, MSG_SKILL_ERROR_MISS_ROGUE_SKILL, skillindex, 0);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
#endif // ENABLE_ROGUE_SKILL125_BRZ

	if (mtargettype)
	{
		delete [] mtargettype;
		mtargettype = NULL;
		delete [] mtargetindex;
		mtargetindex = NULL;
	}
}

#ifdef NEWYEAR_EVENT_2008
void do_SkillNewYear2008(CPC * ch, CNetMsg & msg)
{
	int charindex1 = 0, charindex2 = 0;
	msg >> charindex1
		>> charindex2;

	CPC * tch = gserver.m_playerList.Find(charindex2);
	if(tch == NULL)
	{
		return;
	}

	ApplyNewYearSkill(ch, tch);
}
#endif // NEWYEAR_EVENT_2008

#ifdef HANARO_EVENT
void do_SkillHanaroEvent(CPC * ch, CNetMsg & msg)
{
	// 블리스는 3채널, 아프론은 평화채널에서만 지원한다.
	if (gserver.m_subno != 3)
		return;

	if (ch == NULL)
		return;
	
	if (ch->m_level > 60)
		return;

	CSkill * pSkill = gserver.m_skillProtoList.Create(654, 1) ;
	if (pSkill && ch->CanApplySkill(pSkill->m_proto, pSkill->m_proto->Level(pSkill->m_level)))
	{
		bool bApply = false;
		ApplySkill(ch, ch, pSkill, -1 , bApply);
	}
}
#endif

#ifdef FACTORY_SYSTEM
void do_SealSkillLearn(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	try
	{
		unsigned char subtype;
		int skillindex;
		
		msg.MoveFirst();
		
		msg >> subtype
			>> skillindex;
		
		const CSkillProto* proto = gserver.m_skillProtoList.Find(skillindex);
		if (!proto)
			throw MSG_SKILL_LEARN_ERROR_SYSTEM;

		if (!proto->IsSeal())
			throw MSG_SKILL_LEARN_ERROR_SYSTEM;
		
		// 이미 배웠으면 더 배울 필요없다.
		CSkill* skill = ch->m_sealSkillList.Find(skillindex);
		if (skill)
			throw MSG_SKILL_LEARN_ERROR_SYSTEM;

		const CSkillLevelProto* levelproto = proto->Level(skill->m_level);
		if (!levelproto)
			throw MSG_SKILL_LEARN_ERROR_SYSTEM;

		if (!proto->IsSameJob(ch))
			return ;

		// 레벨
		if (levelproto->m_learnLevel > ch->m_level)
			throw MSG_SKILL_LEARN_ERROR_LEVEL;
		
		// 아이템 검사
		int i;
		CItem* item[MAX_SKILL_LEARN_ITEM];
		for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
		{
			item[i] = NULL;
			if (levelproto->m_learnItemIndex[i] < 0)
				continue ;
			int r, c;
			if (!ch->m_invenNormal.FindItem(&r, &c, levelproto->m_learnItemIndex[i], 0, 0))
				throw MSG_SKILL_LEARN_ERROR_ITEM;
			item[i] = ch->m_invenNormal.GetItem(r, c);
			if (!item[i] || item[i]->Count() < levelproto->m_learnItemCount[i])
				throw MSG_SKILL_LEARN_ERROR_ITEM;
		}

		// 스킬 검사
		for (i = 0; i < MAX_SKILL_LEARN_SKILL; i++)
		{
			if (levelproto->m_learnSkillIndex[i] < 0)
				continue ;
			const CSkillProto* lproto = gserver.m_skillProtoList.Find(levelproto->m_learnSkillIndex[i]);
			CSkill* lskill;
			if (lproto->IsSeal())
				lskill = ch->m_sealSkillList.Find(levelproto->m_learnSkillIndex[i]);
			if (lskill == NULL || lskill->m_level < levelproto->m_learnSkillLevel[i])
				throw MSG_SKILL_LEARN_ERROR_SKILL;
		}

		// 아이템 소모
		for (i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
		{
			if (item[i] && levelproto->m_learnItemCount[i] > 0)
			{
				DecreaseFromInventory(ch, item[i], levelproto->m_learnItemCount[i]);
				if (item[i]->Count() < 1)
				{
					ItemDeleteMsg(rmsg, item[i]);
					SEND_Q(rmsg, ch->m_desc);
					RemoveFromInventory(ch, item[i], true, true);
				}
				else
				{
					ItemUpdateMsg(rmsg, item[i], -levelproto->m_learnItemCount[i]);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}

		// 스킬 추가
		if (skill == NULL)
		{
			skill = gserver.m_skillProtoList.Create(skillindex);
			if (!skill)
				throw MSG_SKILL_LEARN_ERROR_SYSTEM;
			if (proto->IsSeal())
			{
				ch->m_sealSkillList.Add(skill);
				if (skill->m_proto->m_index != 655)
					ch->AddSealSkill(skillindex);
			}
		}

		GAMELOG << init("SEAL SKILL LEARN", ch)
				<< skill->m_proto->m_index
				<< delim
				<< skill->m_level
				<< end;
		
		SkillLearnMsg(rmsg, skill);
		SEND_Q(rmsg, ch->m_desc);

		// 연합 등록 증표 이면 Action 추가
		if (skill->m_proto->m_index == FACTORY_UNION_REG_SKILL)
		{
			CNetMsg action;
			ActionMsg(action, ch, ACTION_GENERAL, AGT_FACTORY);
			SEND_Q(action, ch->m_desc);
		}

		ch->CalcStatus(true);
	}
	catch (MSG_SKILL_LEARN_ERROR_TYPE learnerr)
	{
		SkillLearnErrorMsg(rmsg, learnerr);
		SEND_Q(rmsg, ch->m_desc);
	}
}
#endif // FACTORY_SYSTEM