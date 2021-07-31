#include "stdhdrs.h"
#include "Server.h"
#include "Character.h"
#include "CmdMsg.h"
#include "Log.h"
#include "doFunc.h"

#ifdef ATTACK_PET

CAPet::CAPet()
{
	m_pOwner = NULL;

	m_nPlusStr = m_nPlusCon = m_nPlusDex = m_nPlusInt = 0;
	m_nFaith = m_nStm = 0;

	m_nSeal = 1; 
	m_nSP	= m_nRemainStat = 0;

	m_bSummon = false;

	memset( m_wearing, 0 , sizeof(CItem*)*APET_WEARPOINT );
	m_pProto = NULL;
	m_pPrevPet = m_pNextPet = NULL;

	m_nHpMpPulse = m_nStmPulse = m_nFaithPulse = 0;
	m_nDStmPluse = m_nDFaithPulse = 0;
	
	m_nOpHP = m_nOpMP = 0;
	m_type = MSG_CHAR_APET;

#ifdef APET_AI	
	m_cAI = 0;
	m_nAICount = 0;
	m_nAISlot = 0;
	memset(m_tAIData, -1, sizeof(APET_AI_DATA) * APET_MAX_AI);
	for (int i = 0; i < APET_MAX_AI; i++)
	{
		m_tAIData[i].cUse = 0;
		m_tAIData[i].pItem = NULL;
	}
#endif
}

CAPet::~CAPet()
{
	for( int idx=0 ; idx<APET_WEARPOINT; idx++ )
	{
		if( m_wearing[idx] != NULL )
		{
			delete m_wearing[idx];
			m_wearing[idx] = NULL;
		}
	}
}

void CAPet::Appear(bool bIncludeOwner)
{
	CNetMsg rmsg;

	if (m_pOwner == NULL)
		return ;

	CArea* area = m_pOwner->m_pArea;
	if (area == NULL)
		return ;

	if (m_bSummon)
		return ;

	// 셀에 넣기
	m_pZone = area->m_zone;
	m_pArea = area;
	m_pos = m_pOwner->m_pos;
	int cx, cz;
	area->PointToCellNum(GET_X(this), GET_Z(this), &cx, &cz);
	area->CharToCell(this, GET_YLAYER(this), cx, cz);

	// 나타남을 알림
	AppearMsg(rmsg, this, true);
	area->SendToCell(rmsg, this, bIncludeOwner);
	
	ApplyItemValue();

	ExAPetStatusMsg( rmsg, this );
	m_pArea->SendToCell(rmsg, GetOwner(), true);


	for( int i=0 ; i < APET_WEARPOINT; i++ )
	{
		if( m_wearing[i] )
		{
			AddItemExAPetMsg(rmsg, m_wearing[i]);
			SEND_Q(rmsg, GetOwner()->m_desc);
		}
	}

	m_bSummon = true;
}

void CAPet::Disappear()
{
	CNetMsg rmsg;
	CArea* area = m_pArea;
	if (area == NULL)
		return ;

	if (!IS_IN_CELL(this))
		return ;

	if (!m_bSummon)
		return ;

	// 어택 리스트 지우고
	DelAttackList(this);
	// 사라짐을 알리고
	DisappearMsg(rmsg, this);
	area->SendToCell(rmsg, this, true);

	// 셀에서 제거
	area->CharFromCell(this, true);
	m_pZone = NULL;
	m_pArea = NULL;

	m_bSummon = false;
}

int CAPet::GetStmLevel()
{
	if( m_nStm == 0 )
		return 2;
	else if ( m_nStm>0 && m_nStm<=20 )
		return 1;
	else
		return 0;
}

int	CAPet::GetFaithLevel()
{
	if( m_nFaith == 0 )
		return 2;
	else if ( m_nFaith>0 && m_nFaith<=20 )
		return 1;
	else
		return 0;
}

int CAPet::GetAttackLevel() const
{
	int attacklevel = 0;

	if (m_wearing[APET_WEAR_WEAPON])
	{
		attacklevel = m_wearing[APET_WEAR_WEAPON]->GetItemLevel(false);
	}

	return attacklevel;
}

int CAPet::GetDefenseLevel() const
{
	static int armors[APET_WEARPOINT] = { APET_WEAR_HEAD, APET_WEAR_BODY, APET_WEAR_WEAPON, APET_WEAR_ACCE };

	int defenselevel = 0;
	bool bWear = false;

	int i;

	for (i = 0; i < APET_WEARPOINT; i++)
	{
		if (m_wearing[armors[i]])
		{
			CItem* pItem = m_wearing[armors[i]];
			int nThisItemLevel = pItem->GetItemLevel(false);
			if (defenselevel < nThisItemLevel)
				defenselevel = nThisItemLevel;
			bWear = true;
		}
	}

	if (!bWear)
	{
		defenselevel = m_level * 2;
	}

	return defenselevel;
}

MSG_DAMAGE_TYPE CAPet::GetAttackType(const CSkillProto* proto) const
{
	if (!proto)
		return MSG_DAMAGE_MELEE;
	else
	{
		switch (proto->m_type)
		{
		case ST_MELEE:
			return MSG_DAMAGE_MELEE;
		case ST_RANGE:
			return MSG_DAMAGE_RANGE;
		case ST_MAGIC:
		default:
			return MSG_DAMAGE_MAGIC;
		}
	}
}

bool CAPet::IncreaseStat()
{
	bool bChanged = false;

	if( m_nHpMpPulse == 0  )	m_nHpMpPulse = gserver.m_pulse;
	if( m_nFaithPulse == 0 )	m_nFaithPulse = gserver.m_pulse;

	if( IsWearing() )
	{
		if ( m_nHpMpPulse + PULSE_REAL_SEC*20 < gserver.m_pulse)
		{
			m_nHpMpPulse = gserver.m_pulse;

			m_hp += m_recoverHP;
			m_mp += m_recoverMP;

			if ( m_hp > m_maxHP )
				m_hp = m_maxHP;
			if ( m_mp > m_maxMP )
				m_mp = m_maxMP;

			bChanged = true;
		}

		// 기아가 아닐때만 충성도 증가
		if ( GetStmLevel() == 0 && m_nFaithPulse + (PULSE_REAL_MIN*5) < gserver.m_pulse)
		{
			m_nFaithPulse = gserver.m_pulse;
			int leveldef = ( m_pOwner->m_level - m_level ) / 2 ;
			int nAddFaith = 1 + (leveldef>0? leveldef:0);
			AddFaith( nAddFaith );
			bChanged = true;
		}
	}
	else
	{

	}

	return bChanged;
}

bool CAPet::DecreaseStat()
{
	bool bChanged = false;

	if( m_nStmPulse == 0 )		m_nStmPulse = gserver.m_pulse;
	if( m_nDFaithPulse == 0 )	m_nDFaithPulse = gserver.m_pulse;
	if( m_nDStmPluse == 0 )		m_nDStmPluse = gserver.m_pulse;
	
	// stm 착용 5분에 2포인트, 미착용 30분에 1포인트
	// fath 미착용 20분에 1포인트 
	if( IsWearing() )
	{
		if ( m_nStmPulse + PULSE_REAL_MIN*5 < gserver.m_pulse)
		{
			m_nStmPulse = gserver.m_pulse;
			int leveldef = ( m_level - m_pOwner->m_level ) / 2 ;
			int nAddStm = 2 + (leveldef>0? leveldef:0) + ( GetStmLevel()?1:0 ) ;	// 기아상태일때 스테미너 1 추가 하락

			AddStamina( -nAddStm );
			bChanged = true;
		}
	}
	else
	{
		if ( m_nDStmPluse + PULSE_REAL_MIN*30 < gserver.m_pulse)
		{
			m_nDStmPluse = gserver.m_pulse;
			AddStamina(-1);
			bChanged = true;
		}

		if ( m_nDFaithPulse + PULSE_REAL_MIN*20 < gserver.m_pulse)
		{
			m_nDFaithPulse = gserver.m_pulse;
			int leveldef = ( m_level - m_pOwner->m_level ) / 2 ;
			int nAddFaith = 1 + (leveldef>0? leveldef:0) + (GetStmLevel()? 0:1);
			AddFaith(-nAddFaith);
			bChanged = true;
		}
	}

	return bChanged;
}

void CAPet::CheckItemTime()
{
	int i;
	CItem* pItem = NULL ;

	for( i=0; i< APET_WEARPOINT; i++ )
	{
		pItem = m_wearing[i];
		if( pItem )
		{
			if (pItem->m_used > 0 && pItem->m_used < gserver.m_gameTime)
			{
				GAMELOG << init("ITEM USE TIMEOUT IN PET", GetOwner() )
						<< "ITEM" << delim
						<< itemlog(pItem) << delim
						<< "APET" << delim << m_index
						<< end;
				
				CNetMsg rmsg;
				ExAPetFuntionMsg( rmsg, MSG_SUB_DELETE_EQUIP, this, 0 );
				rmsg << pItem->m_idNum
					 << (char) i;
				SEND_Q( rmsg, GetOwner()->m_desc );

				delete pItem;
				m_wearing[i] = NULL;
			}
		}
	}
}

void CAPet::UpdateStatPlus( int nStr, int nDex, int nInt , int nCon )
{
	// STAT 에 따른 수치 상승 
	if( nStr > 0 )
	{
		m_nPlusStr = nStr;
	}

	if( nDex > 0 )
	{
		m_nPlusDex	= nDex;
	}

	if( nInt > 0 )
	{
		m_nPlusInt	= nInt;
	}

	if( nCon > 0 )
	{
		m_nPlusCon	= nCon;
	}

	CalcStatus(false);
}

void CAPet::CalcStatus(bool bSend)
{
	int nStr = m_str + m_nPlusStr + m_opStr;
	int nDex = m_dex + m_nPlusDex + m_opDex;
	int nInt = m_int + m_nPlusInt + m_opInt;
	int nCon = m_con + m_nPlusCon + m_opCon;

	m_nAttack		= (int)(m_pProto->m_nAttack + ( nStr * 1.5 ) );
	m_nDefence		= (int)(m_pProto->m_nDefence + (nCon*5) );
	m_nHitPoint		= (int)(m_pProto->m_nHitPoint + nDex );
	m_nAvoid		= (int)(m_pProto->m_nAvoidPoint + (nDex*0.25) );
	m_nMagicDefence	= (int)(m_pProto->m_nMagicDefence + nInt );
	m_nMagicAvoid	= (int)(m_pProto->m_nMagicAvoidPoint + (nInt*0.25) );
	m_maxHP			= (int)(m_pProto->m_nMaxHP + m_nOpHP + (nCon*9) );
	m_maxMP			= (int)(m_pProto->m_nMaxMP + m_nOpMP + (nInt*8) );
}

void CAPet::ApplyItemValue( )
{
	CItem* item;
	int i,j;

	// 옵션값 초기화 
	m_eqDefense = m_eqResist = m_eqMelee = m_eqRange = m_eqMagic = 0;
	m_opStr = m_opCon = m_opDex = m_opInt = m_opMelee = m_opMagic = m_opDMelee = m_opResist = 0;
	m_opMeleeHitRate = m_opMagicHitRate = m_opMeleeAvoid = m_opRangeAvoid = m_opResistAvoid = 0;
	m_nOpHP = m_nOpMP = 0;
	m_recoverMP = m_pProto->m_nRecoverMP;
	m_recoverMP = m_pProto->m_nRecoverMP;
	m_runSpeed	= m_pProto->m_nSpeed;

	// 방어구, 무기, 악세
	for( i=0; i< APET_WEARPOINT; i++ )
	{
		item = m_wearing[i];
		if( item )
		{
			// 아이템 타입 검사 해서 넣어주기
			if( i==APET_WEAR_HEAD || i== APET_WEAR_BODY )
			{
				m_eqDefense += item->m_itemProto->m_num0;
				m_eqResist	+= item->m_itemProto->m_num1;
			}
			else if ( i==APET_WEAR_WEAPON )		
			{
				if( item->m_itemProto->m_subtypeIdx)
				m_attackSpeed = item->m_itemProto->m_num2;
			
				if( item->IsMagicWeapon() )
				{
					m_eqMagic += item->m_itemProto->m_num1;
				}
				else if ( item->IsRangeWeapon() )
				{
					m_eqRange += item->m_itemProto->m_num0;
				}
				else
				{
					m_eqMelee += item->m_itemProto->m_num0;
				}				
			}
			
#ifdef MONSTER_RAID_SYSTEM
			if ( item->IsRareItem() )
			{
				if (item->m_pRareOptionProto)
				{
					for (j = 0; j < MAX_RARE_OPTION_SETTING; j++)
					{
						const RAREOPTIONDATA* rod = item->m_pRareOptionProto->GetData(j);
						if (item->m_nRareOptionBit & rod->rarebit)
						{
							COption::ApplyOptionValue(this, rod->optiontype, rod->optionval, item);
						}
					}
					// 스탯 상승 적용
					COption::ApplyOptionValue(this, OPTION_DAMAGE_UP,  item->m_pRareOptionProto->GetDamageUp(), NULL);
					COption::ApplyOptionValue(this, OPTION_DEFENSE_UP, item->m_pRareOptionProto->GetDefenseUp(), NULL);
					COption::ApplyOptionValue(this, OPTION_MAGIC_UP,   item->m_pRareOptionProto->GetMagicUp(), NULL);
					COption::ApplyOptionValue(this, OPTION_RESIST_UP,  item->m_pRareOptionProto->GetResistUp(), NULL);
				}
			}
			else
#endif
			{
				for (j=0; j < MAX_WEAPON_OPTION; j++)
				{
					if (item->m_option[j].m_level <= 0)
						continue;
					item->m_option[i].ApplyOptionValue( this, item );
				}
			}
		}
	}

	CalcStatus(false);
}

float CAPet::GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type)
{
	float hitrate =  m_pProto->m_nHitPoint + (m_level - df->m_level);

	if( GetStmLevel() > 0 )
	{
		hitrate = hitrate / 2;		
	}

	return hitrate;
}

float CAPet::GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type)
{
	float avoidrate = 0.0f;

	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
		avoidrate = m_pProto->m_nAvoidPoint + (m_level - of->m_level);
		break;
	case MSG_DAMAGE_MAGIC:
		avoidrate = m_pProto->m_nMagicAvoidPoint + (m_level - of->m_level);
		break;
	default:
		break;
	}

	if( GetStmLevel() > 0 )
	{
		avoidrate = avoidrate/2 ;
	}

	return avoidrate;
}

bool CAPet::AddExpSP( LONGLONG exp, int sp )
{
	const int MAX_APET_LEVEL = 100 ;
	
	if( GetStmLevel() > 1 || GetFaithLevel() > 1 )		// 충성도나 기아 레벨 2 이상이면 경험치 획득 없음
		return false;
	
	if( m_level >= MAX_APET_LEVEL )
		return false;

	LONGLONG add_exp =0;

	if (m_level <= MAX_APET_LEVEL)
		add_exp = exp;
	if (m_level >= MAX_APET_LEVEL && m_exp > GetNeedExp())
		add_exp = GetNeedExp();

	if ( sp > 0 )
		m_nSP += sp;

	//각종 아이템,스킬 버프 확인
#ifdef PET_EXP_UP
	if( m_pOwner->m_assist.m_avAddition.hcCashPetExpUp_2358 == true )
		add_exp = 2 * add_exp;
	else if( m_pOwner->m_assist.m_avAddition.hcCashPetExpUp_2359 == true )
		add_exp = 3 * add_exp;
#endif // PET_EXP_UP

	m_exp += add_exp;

	if ( m_exp < 0 )
		m_exp = 0;

	if (m_nSP > MAX_SKILLPOINT)
		m_nSP = MAX_SKILLPOINT;
	
	if ( GetNeedExp() <= m_exp && m_level < MAX_APET_LEVEL)
	{
		do {
			GAMELOG << init("APET LEVEL UP", GetOwner() )
					<< m_level
					<< " ==> "
					<< m_level + 1
					<< end;
			m_exp -= GetNeedExp();
			m_level++;
			
			// 레벨업 효과
			LevelUp(true);
			
			if( m_level >= MAX_APET_LEVEL )
				m_exp = 0;

		} while ( GetNeedExp() <= m_exp );
	}

	CNetMsg rmsg;
	ExAPetStatusMsg(rmsg, this);
	SEND_Q(rmsg, GetOwner()->m_desc);
	return true;
}

void CAPet::LevelUp( bool bSendEffect )
{
	CNetMsg rmsg;

	// stat point 증가
	m_nRemainStat += 9;
	m_hp = m_maxHP;
	m_mp = m_maxMP;


	if( bSendEffect )
	{
		// LevelUp Effect Msg
		EffectEtcMsg(rmsg, this, MSG_EFFECT_ETC_LEVELUP);
		this->m_pArea->SendToCell(rmsg, this, true);
	}

	// LevelUp Msg
	ExAPetFuntionMsg( rmsg, MSG_SUB_LEVELUP, NULL, 0 );
	SEND_Q( rmsg, GetOwner()->m_desc );
}


CSkill* CAPet::FindSkill(int skillindex)
{
	CSkill* ret;
	ret = m_skillList.Find( skillindex );

	return ret;
}

#ifdef APET_AI
void CAPet::CheckAIList()
{
	int nCount = 0;
	CPC * pOwner = GetOwner();

	for (int i = 0; i < m_nAISlot; i++)
	{
		CItem * pItem = NULL;
		int r, c;

		if (pOwner->m_invenNormal.FindItem(&r, &c, m_tAIData[i].nItemNum, -1, -1))
			pItem = pOwner->m_invenNormal.GetItem(r, c);

		if (!pItem || (m_tAIData[i].nSkillType == 0  && !FindSkill(m_tAIData[i].nSkillIdx)))
		{
			m_tAIData[i].pItem		= NULL;
			m_tAIData[i].nItemNum	= -1;
			m_tAIData[i].nSkillIdx	= -1;
			m_tAIData[i].nSkillType = -1;
			m_tAIData[i].cUse		= 0;
		}
		else
		{
			m_tAIData[i].pItem = pItem;
			nCount++;
		}
	}

	m_nAICount = nCount;

}

bool CAPet::DelAIList(CItem * pItem)
{
	int i = 0;
	for (i = 0; i < m_nAISlot; i++)
	{
		if (m_tAIData[i].pItem == NULL)
			continue;

		if (m_tAIData[i].pItem == pItem)
		{
			m_tAIData[i].pItem		= NULL;
			m_tAIData[i].cUse		= 0;
			m_tAIData[i].nItemNum	= -1;
			m_tAIData[i].nSkillIdx	= -1;
			m_tAIData[i].nSkillType = -1;
		}
	}

	return true;
}

#endif	// APET_AI
#endif  // ATTACK_PET
