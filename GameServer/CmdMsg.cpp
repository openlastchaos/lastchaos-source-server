#include "stdhdrs.h"
#include "CmdMsg.h"
#include "Server.h"
#include "DBCmd.h"
#include "DBThread.h"
#include "Exp.h"
#include "WarCastle.h"
#include "Log.h"


void AppearMsg(CNetMsg& msg, CCharacter* _ch, bool bNew)
{
	// 안 보이면 무시
	if (!_ch->m_bVisible)
	{
		msg.Init();
		return ;
	}

	int i;

	msg.Init(MSG_APPEAR);

	if (_ch)
	{
		if (_ch->m_maxHP <= 0) _ch->m_maxHP = 1;

		if (IS_NPC(_ch))
		{
			CNPC* ch = TO_NPC(_ch);

			if (ch->m_idNum <= 0 || ch->m_disableTime > 0)
			{
				msg.Init();
				return;
			}

			msg << (char)((bNew) ? 1 : 0)
				<< GET_CHAR_TYPE(ch)
				<< ch->m_index
				<< ch->m_idNum
				<< GET_X(ch)
				<< GET_Z(ch)
				<< GET_H(ch)
				<< GET_R(ch)
				<< GET_YLAYER(ch)
				<< ch->m_hp
				<< ch->m_maxHP;
			ch->m_assist.AppendAssistToNetMsg(msg);
			msg << ch->GetMapAttr();
#ifdef ALTERNATE_MERCHANT
			msg << ch->m_nOWnerIndex
				<< ch->m_pstrOwnerName
				<< ch->m_pstrShopName;			
#endif // ALTERNATE_MERCHANT
		} // NPC


		else if (IS_PC(_ch))
		{
			CPC* ch = TO_PC(_ch);
			
			msg << (char)((bNew) ? 1 : 0)
				<< GET_CHAR_TYPE(ch)
				<< ch->m_index
				<< ch->GetName()
				<< ch->m_job
#ifdef ENABLE_CHANGEJOB
				<< ch->m_job2
#endif
				<< ch->m_hairstyle
				<< ch->m_facestyle
				<< GET_X(ch)
				<< GET_Z(ch)
				<< GET_H(ch)
				<< GET_R(ch)
				<< GET_YLAYER(ch)
				<< ch->m_hp
				<< ch->m_maxHP
				<< (unsigned short)ch->GetPlayerState()
				<< ch->m_pkPenalty
				<< (char)ch->GetPKName()
				<< ch->m_plusEffect;
			
			for (i = WEARING_SHOW_START; i <= WEARING_SHOW_END; i++)
			{
				if (ch->m_wearing[i])
				{
					msg << (int)ch->m_wearing[i]->m_idNum;
					// 050223 : bs : plus 효과를 위한 정보 추가
					msg << ch->m_wearing[i]->m_plus;
				}
				else
				{
					msg << (int)-1;
					// 050223 : bs : plus 효과를 위한 정보 추가
					msg << (int)0;
				}
			}
#if defined(LC_MAL) || defined(KYD_TEST) // [2/17/2009 KwonYongDae] 악세서리 정보 추가
			for (i = WEARING_ACCESSORY1; i <= WEARING_ACCESSORY3; i++)
			{
				if (ch->m_wearing[i])
				{
					// 악세서리 정보
					msg << (int)ch->m_wearing[i]->m_idNum ;
				}
				else
				{
					msg << (int)-1;
				}
			}
#endif
			ch->m_assist.AppendAssistToNetMsg(msg);
			
			// 상점관련 추가 - 050107 bs
			if (ch->m_personalshop)
			{
				msg << ch->m_personalshop->GetType()
					<< ch->m_personalshop->GetName();
			}
			else
			{
				msg << (char)PST_NOSHOP << "";
			}
			
			if (ch->m_guildInfo && ch->m_guildInfo->guild())
			{
				msg << ch->m_guildInfo->guild()->index()
					<< ch->m_guildInfo->guild()->name()
#if defined(GUILD_MARK_TABLE)
					<< ch->m_guildInfo->pos()
					<< ch->m_guildInfo->GetGuildMark();
#else
				<< ch->m_guildInfo->pos();
#endif // ( GUILD_MARK_TABLE)
			}
			else
			{
				msg << (int)-1
					<< ""
#if defined(GUILD_MARK_TABLE)
					<< -1
					<< (char)0;
#else
				<< -1;
#endif // (GUILD_MARK_TABLE)
			}
			
			msg << ch->m_changeIndex
				<< ch->m_fame  //0627
				<< ch->GetMapAttr()
				<< (char)ch->GetJoinFlag(ZONE_MERAC);
#ifdef DRATAN_CASTLE
			msg << (char)ch->GetJoinFlag(ZONE_DRATAN);
#endif // DRATAN_CASTLE		
#ifdef ENABLE_PET
			CPet* pet = ch->GetPet();
			if (pet && ch->m_wearing[WEARING_PET])
			{	
				if (pet->IsMount())
				{
					msg << pet->GetPetTypeGrade();
				}
				else
				{
					msg << (char)0;
				}
#ifdef PET_DIFFERENTIATION_ITEM			
				msg << pet->GetPetColor();
#endif // PET_DIFFERENTIATION_ITEM
			}
			else
			{
				msg << (char)0;
#ifdef PET_DIFFERENTIATION_ITEM			
				msg << (char)0;
#endif // PET_DIFFERENTIATION_ITEM
			}
			
#endif
			msg << ch->m_evocationType;
			if (ch->IsSetPlayerState(PLAYER_STATE_SUPPORTER))
				msg << (char)1;
			else
				msg << (char)0;
#ifdef LAKA_PRESSCORPS
			msg << ch->m_bPressCorps;
#endif // LAKA_PRESSCORPS
		} // PC

#ifdef ENABLE_PET
		else if (IS_PET(_ch))
		{
			CPet* ch = TO_PET(_ch);

			msg << (char)((bNew) ? 1 : 0)
				<< GET_CHAR_TYPE(ch)
				<< ch->m_index;
			if (ch->GetOwner())
			{
				msg << ch->GetOwner()->m_index
					<< ch->GetOwner()->GetName();
			}
			else
			{
				msg << 0
					<< "";
			}
			msg << ch->GetPetTypeGrade()
				<< GET_X(ch)
				<< GET_Z(ch)
				<< GET_H(ch)
				<< GET_R(ch)
				<< GET_YLAYER(ch)
				<< ch->m_hp
				<< ch->m_maxHP
				<< ch->GetMapAttr();
#ifdef PET_NAME_CHANGE
			msg << ch->m_name;
#endif // PET_NAME_CHANGE
#ifdef PET_DIFFERENTIATION_ITEM		
			msg << ch->GetPetColor();
#endif //PET_DIFFERENTIATION_ITEM
#ifdef PET_TURNTO_NPC
			msg << ch->GetPetTurnToNpc();	
			msg << ch->GetPetTurnToNpcSize();
#endif //PET_TURNTO_NPC
		} // pet
#endif

		else if (IS_ELEMENTAL(_ch))
		{
			CElemental* ch = TO_ELEMENTAL(_ch);
			if (ch->GetOwner())
			{
				// 소환수 크기를 크게 할 것인가? : 엘리멘탈 리스트로 전직하면 커짐
				char cIsBigSize = (ch->GetOwner()->m_job2 == JOB_2ND_ELEMENTALLIST) ? 1 : 0;
				msg << (char)((bNew) ? 1 : 0)
					<< GET_CHAR_TYPE(ch)
					<< ch->m_index
					<< ch->GetElementalType()
					<< ch->GetOwner()->m_index
					<< ch->GetOwner()->GetName()
					<< GET_X(ch)
					<< GET_Z(ch)
					<< GET_H(ch)
					<< GET_R(ch)
					<< GET_YLAYER(ch)
					<< ch->m_hp
					<< ch->m_maxHP;
				ch->m_assist.AppendAssistToNetMsg(msg);
				msg	<< ch->GetMapAttr()
					<< cIsBigSize;
			}
			else
			{
				msg.Init();
			}
		} // elemental

#ifdef ATTACK_PET
		else if( IS_APET(_ch) )
		{
			CAPet* ch = TO_APET(_ch);
			msg << (char)((bNew)? 1: 0 )
				<< GET_CHAR_TYPE(ch)
				<< ch->m_index
				<< ch->m_pProto->Index()
				<< ch->m_level
				<< ch->m_name;

			if( ch->GetOwner() )
			{
				msg << ch->GetOwner()->m_index
					<< ch->GetOwner()->m_name;
			}
			else
			{
				msg << 0
					<< "";
			}

			msg << ch->m_pProto->Index()
				<< GET_X(ch)
				<< GET_Z(ch)
				<< GET_H(ch)
				<< GET_R(ch)
				<< GET_YLAYER(ch);

			for( int i=0; i<APET_WEARPOINT; i++)
			{
				if (ch->m_wearing[i])
				{
					msg << (int)ch->m_wearing[i]->m_idNum;
					msg << ch->m_wearing[i]->m_plus;
				}
				else
				{
					msg << (int)-1;
					msg << (int)0;
				}
			}

			msg << ch->m_hp
				<< ch->m_maxHP
				<< ch->GetMapAttr();
		}
#endif // ATTACK_PET
	}
	else
		msg.Init();
}

#ifdef FEEITEM
void NameChangeGuildMemberMsg(CNetMsg& msg, int index, const char* name, char job )
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_GUILDMEMBERNAMECHANGE
		<< index
		<< name
		<< job;
}

void NameChangeFriendMsg(CNetMsg& msg, int index, const char* name, char job )
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_FRIENDNAMECHANGE
		<< index
		<< name
		<< job;
}
void NameChangeRepMsg(CNetMsg& msg, MSG_EX_NAMECHANGE_ERROR_TYPE type, const char* name, char bguild)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_NAMECHANGE
		<< (unsigned char) type
		<< name
		<< bguild;
}

void CashItemBalanceRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalance)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASHITEM
		 << (unsigned char) MSG_EX_CASHITEM_BALANCE_REP
		 << (unsigned char) errorcode
		 << cashBalance;
#ifdef LIMIT_CATALOG
	CashItemLimitCatalAddMsg(msg);
#endif // LIMIT_CATALOG
}

#ifdef LIMIT_CATALOG
// 해당 메세지 뒤에 한정판매 물품 카운터, 인덱스 갯수를 붙임
void CashItemLimitCatalAddMsg(CNetMsg& msg)
{
	int count, i;
	count = gserver.m_limitCT.GetCount();
	void* pos = NULL;
	CCatalog* catal = NULL;
	pos = gserver.m_limitCT.GetHead();

	msg << count;
	for(i = 0; i < count; i++)
	{
		catal = gserver.m_catalogList.Find(gserver.m_limitCT.GetData(pos));
		if( catal )
		{
			msg << catal->GetIndex()
				<< catal->GetToSell();
		}
		pos = gserver.m_limitCT.GetNext(pos);
	}
}
#endif // LIMIT_CATALOG

void CashItemPurchaseRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalance)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASHITEM
		<< (unsigned char) MSG_EX_CASHITEM_PURCHASE_REP
		<< (unsigned char) errorcode
		<< cashBalance;
#ifdef LIMIT_CATALOG
	CashItemLimitCatalAddMsg(msg);
#endif // LIMIT_CATALOG
}

void CashItemBringRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, bool bPresent)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASHITEM;
	if( bPresent )
		msg << (unsigned char) MSG_EX_CASHITEM_GIFT_RECV_REP;
	else
		msg << (unsigned char) MSG_EX_CASHITEM_BRING_REP;

	msg	<< (unsigned char) errorcode;
}

void CashItemPurchaseListRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int count, int idx[], int ctid[])
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASHITEM
		<< (unsigned char) MSG_EX_CASHITEM_PURCHASELIST_REP
		<< (unsigned char) errorcode
		<< count;

	if(count == 0) return;

	for(int i = 0; i < count; i++)
	{
		msg << idx[i] 
			<< ctid[i];
	}
}

#endif // FEEITEM

#ifdef CASH_ITEM_COUPON
void CashItemCouponListRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int nCouponCount, CASH_COUPON* pCoupon )
{
	int i=0;
	
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASHITEM
		 << (unsigned char) MSG_EX_CASHITEM_CUPON_REP
		 << (unsigned char) errorcode
		 << nCouponCount;

	for( i=0; i<nCouponCount; i++ )
	{
		msg << pCoupon[i].name
			<< pCoupon[i].id
			<< pCoupon[i].amount
			<< pCoupon[i].limitAmount;
	}
}

void CashItemPurchaseByCouponRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalance)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASHITEM
		<< (unsigned char) MSG_EX_CASHITEM_PURCHASE_WITH_COUPON_REP
		<< (unsigned char) errorcode
		<< cashBalance;
#ifdef LIMIT_CATALOG
	CashItemLimitCatalAddMsg(msg);
#endif // LIMIT_CATALOG
}

#endif //CASH_ITEM_COUPON

void DisappearMsg(CNetMsg& msg, CCharacter* ch)
{
	if (ch)
	{
		// 안 보이면 무시
		if (!ch->m_bVisible)
		{
			msg.Init();
			return ;
		}

		msg.Init(MSG_DISAPPEAR);
		msg << GET_CHAR_TYPE(ch)
			<< ch->m_index;
	}
}

void MoveMsg(CNetMsg& msg, MSG_MOVE_TYPE movetype, CCharacter* ch)
{
	// 안 보이면 무시
	if (!ch->m_bVisible)
	{
		msg.Init();
		return ;
	}

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

#ifdef ENABLE_PET
	if (IS_PC(ch) && TO_PC(ch)->m_wearing[WEARING_PET])
	{
		CPet* pet = TO_PC(ch)->GetPet();
		if (pet)
		{
			if (pet->IsMount())
			{
				walkSpeed = pet->m_walkSpeed;
				runSpeed = pet->m_runSpeed;
			}
		}
	}
#endif

	msg.Init(MSG_MOVE);
	msg << GET_CHAR_TYPE(ch)
		<< (char)movetype
		<< ch->m_index;

#ifdef NIGHT_SHADOW  // TESTING
	switch ( movetype )
	{
	case MSG_MOVE_WALK:		msg << flySpeed;	break;
	case MSG_MOVE_RUN:		msg << runSpeed;	break;
	case MSG_MOVE_FLY:		msg << flySpeed;	break;
	default :				msg << (float)0.0f;	break;
	}
#else
	if (movetype == MSG_MOVE_WALK)
		msg << walkSpeed;
	else if (movetype == MSG_MOVE_RUN)
		msg << runSpeed;
	else
		msg << (float)0.0f;
#endif //NIGHT_SHADOW	

	msg	<< GET_X(ch)
		<< GET_Z(ch)
		<< GET_H(ch)
		<< GET_R(ch)
		<< GET_YLAYER(ch)
		<< ch->GetMapAttr();
}

void StatusMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_STATUS);

	// 옵션에 의한 스탯 상승 수치 적용
	int str = ch->m_statpt_str + ch->m_opStr;
	int dex = ch->m_statpt_dex + ch->m_opDex;
	int intl = ch->m_statpt_int + ch->m_opInt;
	int con = ch->m_statpt_con + ch->m_opCon;

	int attack = 0;
	int magic = 0;
	int defense = 0;
	int resist = 0;

#ifdef NEW_UI
	int attack_diff = 0;
	int magic_diff = 0;
	int defense_diff = 0;
	int resist_diff = 0;

	extern int base_stat[JOBCOUNT][4];
	extern int nBaseHitTypeProb[2][6];
#endif // NEW_UI

	if (ch->m_wearing[WEARING_WEAPON] && ch->m_wearing[WEARING_WEAPON]->m_itemProto == NULL)
	{
		// TODO : DEBUGING용
		strncpy(g_buf3, "ERR:ITEM:PROTO:", 16);
		strncat(g_buf3, ch->m_name, ch->m_name.Length());
		strcat(g_buf3, ":");
		strcat(g_buf3, ch->m_nick);
		strcat(g_buf3, ":");
		IntCat(g_buf3, ch->m_wearing[WEARING_WEAPON]->m_idNum);
		puts(g_buf3);
	}
	else
	{
		if (ch->m_wearing[WEARING_WEAPON] && ch->m_wearing[WEARING_WEAPON]->GetUsingStat() == USING_DEX)
			attack = ch->m_dex * 10 / 15;
		else
			attack = ch->m_str * 10 / 15;

		if (ch->m_wearing[WEARING_WEAPON] && ch->m_wearing[WEARING_WEAPON]->IsRangeWeapon())
			attack += ch->m_eqRange + ch->m_opRange;
		else
			attack += ch->m_eqMelee + ch->m_opMelee;
	}

	magic = ch->m_int * 10 / 15 + ch->m_eqMagic + ch->m_opMagic;

	defense = ch->m_eqDefense + ch->m_opDMelee + ch->m_opDRange;
	resist = ch->m_eqResist + ch->m_opResist;

	attack += ch->m_avPassiveAddition.attack + attack * ch->m_avPassiveRate.attack / SKILL_RATE_UNIT;
	magic += ch->m_avPassiveAddition.magic + magic * ch->m_avPassiveRate.magic / SKILL_RATE_UNIT;
	defense += ch->m_avPassiveAddition.defense + defense * ch->m_avPassiveRate.defense / SKILL_RATE_UNIT;
	resist += ch->m_avPassiveAddition.resist + resist * ch->m_avPassiveRate.resist / SKILL_RATE_UNIT;
	int addattack80 = ch->m_avPassiveAddition.attack80;
	int rateattack80 = attack * ch->m_avPassiveRate.attack80 / SKILL_RATE_UNIT;
	if (addattack80 > 80)
		addattack80 = 80;
	if (rateattack80 > 80)
		rateattack80 = 80;
	attack += addattack80 + rateattack80;

#ifdef NEW_UI
	attack_diff = attack;
	magic_diff = magic;
	defense_diff = defense;
	resist_diff = resist;
#endif // NEW_UI

#ifdef NEW_BALANCE
	addattack80 = ch->m_assist.m_avAddition.attack80;
#else
	addattack80 = ch->m_assist.m_avRate.attack80;
#endif // #ifdef NEW_BALANCE
	rateattack80 = attack * ch->m_assist.m_avRate.attack80 / SKILL_RATE_UNIT;
	if (addattack80 > 80)
		addattack80 = 80;
	if (rateattack80 > 80)
		rateattack80 = 80;
	attack += addattack80 + rateattack80;

	// 버프효과 표시
	if (ch->m_wearing[WEARING_WEAPON] && ch->m_wearing[WEARING_WEAPON]->IsRangeWeapon())
	{
		attack += ch->m_assist.m_avAddition.attack + attack * ch->m_assist.m_avRate.attack / SKILL_RATE_UNIT
					+ ch->m_assist.m_avAddition.attack_dam_range
					+  attack * ch->m_assist.m_avRate.attack_dam_range / SKILL_RATE_UNIT;
	}
	else
	{
		attack += ch->m_assist.m_avAddition.attack + attack * ch->m_assist.m_avRate.attack / SKILL_RATE_UNIT
					+ ch->m_assist.m_avAddition.attack_dam_melee
					+ attack * ch->m_assist.m_avRate.attack_dam_melee / SKILL_RATE_UNIT;
	}

	magic += ch->m_assist.m_avAddition.magic + magic * ch->m_assist.m_avRate.magic / SKILL_RATE_UNIT;
	defense += ch->m_assist.m_avAddition.defense + defense * ch->m_assist.m_avRate.defense / SKILL_RATE_UNIT;
	resist += ch->m_assist.m_avAddition.resist + resist * ch->m_assist.m_avRate.resist / SKILL_RATE_UNIT;

#ifdef NEW_UI
	attack_diff = attack - attack_diff;
	magic_diff = magic - magic_diff;
	defense_diff = defense - defense_diff;
	resist_diff = resist - resist_diff;
#endif // NEW_UI

#ifdef ENABLE_CHANGEJOB
	if (ch->m_wearing[WEARING_WEAPON] && ch->m_wearing[WEARING_WEAPON]->IsRangeWeapon())
		attack += ch->GetRangeLevelBonus();
	else
		attack += ch->GetMeleeLevelBonus();
	magic += ch->GetMagicLevelBonus();
	defense += ch->GetDefenseLevelBonus();
	resist += ch->GetResistLevelBonus();
#endif

	// 강신 적용 : 물리방어
	switch (ch->m_evocationType)
	{
	case EVOCATION_HELLOUND:
		defense = defense * 10 / 11;
		break;
	case EVOCATION_ELENEN:
		defense = defense * 10 / 7;
		break;
	}

	// 강신 적용 : 마공
	switch (ch->m_evocationType)
	{
	case EVOCATION_HELLOUND:
		magic = magic;
		break;

	case EVOCATION_ELENEN:
		magic = magic * 10 / 13;
		break;
	}

	// 강신 적용 : 마법방어
	switch (ch->m_evocationType)
	{
	case EVOCATION_HELLOUND:
		resist = resist;
		break;
	case EVOCATION_ELENEN:
		resist = resist * 13 / 10;
		break;
	}

	float walkSpeed = ch->m_walkSpeed;
	float runSpeed = ch->m_runSpeed;
#ifdef NIGHT_SHADOW
	float flySpeed = ch->m_flySpeed;
#endif // NIGHT_SHADOW

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
#endif // NIGHT_SHADOW
		}
	}
#endif // DYNAMIC_DUNGEON

#ifdef ENABLE_PET
	char mountPet = 0;
#ifdef PET_DIFFERENTIATION_ITEM
	char colorType = 0;
#endif // PET_DIFFERENTIATION_ITEM
	if (ch->m_wearing[WEARING_PET])
	{
		CPet* pet = ch->GetPet();
		if (pet)
		{
			if (pet->IsMount())
			{
				mountPet = pet->GetPetTypeGrade();
				walkSpeed = pet->m_walkSpeed;
				runSpeed = pet->m_runSpeed;
#ifdef PET_DIFFERENTIATION_ITEM
				colorType = pet->GetPetColor();
#endif // PET_DIFFERENTIATION_ITEM
			}
		}
	}
#endif

#ifdef NEW_UI
	///////////////////////////////////////////////////////
	float avoid_magic_org = 0.0, avoid_magic = 0.0;			// 마법회피
	float avoid_melee_org = 0.0, avoid_melee = 0.0;			// 물리회피
	float fhitrate_magic_org = 0.0, fhitrate_magic = 0.0;	// 마법명중
	float fhitrate_melee_org = 0.0, fhitrate_melee = 0.0;	// 물리명중

	MSG_DAMAGE_TYPE meleetype;

	// 물리회피계산, 물리명중계산
	if(ch->m_wearing[WEARING_WEAPON])
	{	
		if(ch->m_wearing[WEARING_WEAPON]->IsRangeWeapon())
			meleetype = MSG_DAMAGE_RANGE;
		else
			meleetype = MSG_DAMAGE_MELEE;
	}
	else
			meleetype = MSG_DAMAGE_MELEE;

	avoid_melee = ch->GetAvoid(ch, meleetype);
	fhitrate_melee = ch->GetHitrate(ch, meleetype);

	switch (meleetype)
	{
	case MSG_DAMAGE_MELEE:
		avoid_melee += ch->m_opMeleeAvoid + ch->m_avPassiveAddition.Avoid_2034;
		avoid_melee += ch->m_avPassiveAddition.avoid + (avoid_melee * ch->m_avPassiveRate.avoid / SKILL_RATE_UNIT);
		fhitrate_melee += ch->m_opMeleeHitRate + ch->m_avPassiveAddition.HitRate_2033;
		fhitrate_melee += ch->m_avPassiveAddition.hitrate + (fhitrate_melee * ch->m_avPassiveRate.hitrate / SKILL_RATE_UNIT);
		break;
	case MSG_DAMAGE_RANGE:
		avoid_melee += ch->m_opRangeAvoid + ch->m_avPassiveAddition.Avoid_2034;
		avoid_melee += ch->m_avPassiveAddition.avoid + (avoid_melee * ch->m_avPassiveRate.avoid / SKILL_RATE_UNIT);
		fhitrate_melee += ch->m_opRangeHitRate + ch->m_avPassiveAddition.HitRate_2033;
		fhitrate_melee += ch->m_avPassiveAddition.hitrate + (fhitrate_melee * ch->m_avPassiveRate.hitrate / SKILL_RATE_UNIT);
		break;
	default:
		break;
	}

	avoid_melee_org = avoid_melee;			// 물리회피계산
	avoid_melee += ch->m_assist.m_avAddition.avoid + (avoid_melee * ch->m_assist.m_avRate.avoid / SKILL_RATE_UNIT);

	fhitrate_melee_org = fhitrate_melee;	// 물리명중계산
	fhitrate_melee += ch->m_assist.m_avAddition.hitrate + (fhitrate_melee * ch->m_assist.m_avRate.hitrate / SKILL_RATE_UNIT);

	
	// 마법회피계산
	avoid_magic = ch->GetAvoid(ch, MSG_DAMAGE_MAGIC);
	avoid_magic += ch->m_opResistAvoid + ch->m_avPassiveAddition.Avoid_2034;
	avoid_magic += ch->m_avPassiveAddition.magicavoid + (avoid_magic * ch->m_avPassiveRate.magicavoid / SKILL_RATE_UNIT);

	avoid_magic_org = avoid_magic;
	avoid_magic += ch->m_assist.m_avAddition.magicavoid + (avoid_magic * ch->m_assist.m_avRate.magicavoid / SKILL_RATE_UNIT);

	// 마법명중계산
	fhitrate_magic = ch->GetHitrate(ch, MSG_DAMAGE_MAGIC);
	fhitrate_magic += ch->m_opMagicHitRate + ch->m_avPassiveAddition.HitRate_2033;
	fhitrate_magic += ch->m_avPassiveAddition.magichitrate + (fhitrate_magic * ch->m_avPassiveRate.magichitrate / SKILL_RATE_UNIT);

	fhitrate_magic_org = fhitrate_magic;
	fhitrate_magic += ch->m_assist.m_avAddition.magichitrate + (fhitrate_magic * ch->m_assist.m_avRate.magichitrate / SKILL_RATE_UNIT);

	// 크리티컬, 데들리
	int critical = 0, deadly = 0;
	int critical_org = 0, deadly_org = 0;
	int nOffenseDexBase = 0;
	nOffenseDexBase  = base_stat[ch->m_job][1];
	critical = nBaseHitTypeProb[0][4];
	deadly = nBaseHitTypeProb[0][5];

	critical += (ch->m_dex - nOffenseDexBase) * 9 / 10;
	deadly += (ch->m_dex - nOffenseDexBase) * 6 / 10;

	critical += critical * ch->m_opAttackCritical / 100;

	critical += ch->m_avPassiveAddition.critical + (critical * ch->m_avPassiveRate.critical);
	critical_org = critical;
	critical += ch->m_assist.m_avAddition.critical + (critical * ch->m_assist.m_avRate.critical);

	deadly += ch->m_avPassiveAddition.deadly + (deadly * ch->m_avPassiveRate.deadly);
	deadly_org = deadly;
	deadly += ch->m_assist.m_avAddition.deadly + (deadly * ch->m_assist.m_avRate.deadly);

	////////////////////////////////////////////////////////////////////////////////////

#endif // NEW_UI

	msg << ch->m_level
		<< ch->m_exp
		<< GetLevelupExp(ch->m_level) 
		<< ch->m_hp
		<< ch->m_maxHP
#ifdef NEW_UI
		<< ch->m_maxHP_org
#endif // NEW_UI

		<< ch->m_mp
		<< ch->m_maxMP
#ifdef NEW_UI
		<< ch->m_maxMP_org
#endif // NEW_UI

		<< str
		<< dex
		<< intl
		<< con

		<< ch->m_opStr
		<< ch->m_opDex
		<< ch->m_opInt
		<< ch->m_opCon

		<< attack
#ifdef NEW_UI
		<< attack_diff // (증감값)
#endif // NEW_UI

		<< magic
#ifdef NEW_UI
		<< magic_diff // (증감값)
#endif // NEW_UI

		<< defense
#ifdef NEW_UI
		<< defense_diff // (증감값)
#endif // NEW_UI

		<< resist
#ifdef NEW_UI
		<< resist_diff // (증감값)
#endif // NEW_UI

#ifdef NEW_UI
		<< (int)avoid_melee			// 물리회피(최종)
		<< (int)avoid_melee_org		// 물리회피(추가되는거 뺀 값)
		<< (int)avoid_magic			// 마법회피(최종)
		<< (int)avoid_magic_org		// 마법회피(추가되는거 뺀 값)
		<< (int)fhitrate_melee
		<< (int)fhitrate_melee_org
		<< (int)fhitrate_magic
		<< (int)fhitrate_magic_org

		<< critical
		<< critical_org
		<< deadly
		<< deadly_org
#endif // NEW_UI

		<< ch->m_skillPoint
		<< ch->m_weight
		<< ch->m_maxWeight
		<< walkSpeed
		<< runSpeed
#ifdef NEW_UI
		<< ch->m_runSpeed_org  // (추가되는거 뺀 값)
#endif // NEW_UI
		<< ch->m_attackSpeed
#ifdef NEW_UI
		<< ch->m_attackSpeed_org  // (추가되는거 뺀 값)
#endif // NEW_UI
		<< ch->m_magicSpeed
		<< (char)ch->GetPKName()
		<< ch->m_pkPenalty
		<< ch->m_pkCount
		<< ch->m_fame
		<< ch->m_attackRange
		<< (char)ch->GetJoinFlag(ZONE_MERAC)
#ifdef DRATAN_CASTLE
		<< (char)ch->GetJoinFlag(ZONE_DRATAN)
#endif // DRATAN_CASTLE	
		<< ch->m_skillSpeed
		<< ch->GetMapAttr();
#ifdef ENABLE_PET
	msg << mountPet;
#ifdef PET_DIFFERENTIATION_ITEM
	msg << colorType;
#endif //PET_DIFFERENTIATION_ITEM

#endif

	msg << ch->m_evocationType
		<< ch->GetRemainEvocation(true);

#ifdef LAKA_PRESSCORPS
	msg << ch->m_bPressCorps;
#endif // LAKA_PRESSCORPS
	msg << ch->m_opDecreaseSkillDelay			//쿨타임
		<< ch->m_opDecreaseSkillMP ;
}

void AtMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_AT);

	msg << ch->m_index
		<< ch->GetName()
		<< ch->m_job
#ifdef ENABLE_CHANGEJOB
		<< ch->m_job2
#endif
		<< ch->m_hairstyle
		<< ch->m_facestyle
		<< ch->m_pZone->m_index
		<< ch->m_pArea->m_index
		<< GET_X(ch)
		<< GET_Z(ch)
		<< GET_H(ch)
		<< GET_R(ch)
		<< GET_YLAYER(ch)
		<< ch->m_desc->m_index
		<< ch->m_guildoutdate
		<< ch->m_plusEffect
		<< ch->GetMapAttr();
//0707
#ifdef SECRET_KEY		 
		ch->m_secretkey = GetRandom(10,100);		
		int key = ((ch->m_index + ch->m_secretkey) << 1);
		msg << key;
#endif
#ifdef EVENT_PCBANG
		msg << ch->m_desc->m_location;
#endif

#ifdef NO_CHATTING
			msg << ch->m_nflag;
#endif
}

void SayMsg(CNetMsg& msg, MSG_CHAT_TYPE chatType, int sender_index, const char* sender_name, const char* receiver_name, const char* chat,int nGroup)
{
	msg.Init(MSG_CHAT);
	msg << (unsigned char)chatType
		<< sender_index
		<< sender_name
		<< receiver_name
		<< chat
		<< nGroup;
}

void GotoMsg(CNetMsg& msg, CPC* ch, char bHack)
{
	msg.Init(MSG_GOTO);
	msg << ((ch->m_pZone) ? ch->m_pZone->m_index : (int)-1)
		<< GET_X(ch)
		<< GET_Z(ch)
		<< GET_H(ch)
		<< GET_R(ch)
		<< GET_YLAYER(ch)
		<< bHack;
}

void FailMsg(CNetMsg& msg, MSG_FAIL_TYPE failtype)
{
	msg.Init(MSG_FAIL);
	msg << (unsigned char)failtype;
}

void DamageMsg(CNetMsg& msg, CCharacter* ch, CCharacter* tch, MSG_DAMAGE_TYPE damageType, int skillidx, int damage, char flag)
{
	msg.Init(MSG_DAMAGE);

	msg << GET_CHAR_TYPE(ch)
		<< ch->m_index
		<< (char)damageType
		<< (int)skillidx
		<< GET_CHAR_TYPE(tch)
		<< tch->m_index
		<< tch->m_hp
		<< tch->m_mp
		<< damage;					// damage

	// 공격속도
#ifdef ENABLE_WAR_CASTLE
	if (IS_NPC(tch) && TO_NPC(tch)->m_proto->CheckFlag(NPC_LORD_SYMBOL))
	{
		msg << (char)12;
	}
	else
	{
		switch (damageType)
		{
		case MSG_DAMAGE_MELEE:
		case MSG_DAMAGE_RANGE:
		case MSG_DAMAGE_MAGIC:
			msg << ch->m_attackSpeed;
			break;
		default:
			msg << (char)0;
			break;
		}
	}
#else
	switch (damageType)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
	case MSG_DAMAGE_MAGIC:
		msg << ch->m_attackSpeed;
		break;
	default:
		msg << (char)0;
		break;
	}
#endif
	msg << flag;					// 회피(0), 명중(1), 크리티컬(2) 같은 것들 flag
}

bool InventoryMsg(CNetMsg& msg, CPC* ch, char resultArrange, int tab, int row)
{
	CInventory* inven = GET_INVENTORY(ch, tab);
	if (!inven || !inven->m_rows[row])
	{
		msg.Init();
		return false;
	}

	msg.Init(MSG_INVENTORY);
	msg << resultArrange
		<< (char)tab
		<< (char)row;

	int i, j;
	CItem* item;
	for (i = 0; i < ITEMS_PER_ROW; i++)
	{
		item = inven->GetItem(row, i);
		if (!item)
		{
			// 아이템 없으면 아이템 인덱스만 -1로 셋팅한 후 나머지 데이타 Skip
			msg << (int)-1;							// item_idx
			continue;
		}
		else
		{
			msg << (int)item->m_index				// item_idx
				<< (int)item->m_idNum				// item_type
				<< item->m_wearPos					// wear_pos
				<< (int)item->m_plus				// plus
				<< (int)item->m_flag				// flag
				<< (int)item->m_used				// used
#ifdef COMPOSITE_TIME				
				<< (int)item->m_used_2				// used2
#endif // COMPOSITE_TIME
				<< (LONGLONG)item->Count();			// count
#ifdef SET_ITEM
			if (item->IsOriginItem())
			{
				// 옵션
				msg << (char)item->GetOriginOptionCnt();
				for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
				{
					if (item->GetOriginIndex(i) > -1)
					{
						msg << (char)item->GetOriginIndex(i)
							<< (char)item->GetOriginLevel(i);
					}
				}
				
				// 스킬
				msg << (char)item->GetBelongType()					// 귀속 설정
					<< (char)item->GetOriginSkillCnt();
				for (i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
				{
					if (item->GetOriginIndex(i) > -1)
					{
						msg << item->GetOriginIndex(i)
							<< (char)item->GetOriginLevel(i);
					}
				}
			}
			else // IsOriginItem
#endif // SET_ITEM
			{
				msg << (char)item->m_nOption;
				
				for (j=0; j < item->m_nOption; j++)
				{
					msg << (char)item->m_option[j].m_type
						<< (char)item->m_option[j].m_level;
				}
			}
		}
	}

	return true;
}

void WearingMsg(CNetMsg& msg, CPC* ch, char wearpos, int item_db_index, int item_plus)
{
	// 안 보이면 무시
	if (!ch->m_bVisible)
	{
		msg.Init();
		return ;
	}

	msg.Init(MSG_WEARING);

	msg << ch->m_index		// char_index
		<< wearpos
		<< item_db_index;
// 050223 : bs : plus 효과를 위한 정보 추가
	msg << item_plus;
// --- 050223 : bs : plus 효과를 위한 정보 추가
}

void ItemProlongMsg(CNetMsg& msg, int itemindex, int useTime, bool bprolong)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_USE_PROLONG
		<< itemindex
		<< useTime
		<< (unsigned char) bprolong;
}

void ItemNotUseMsg(CNetMsg& msg, MSG_ITEM_USE_ERROR_TYPE error)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_USE_ERROR
		<< error;
}

void ItemUseMsg(CNetMsg& msg, CPC* ch, char tab, char row, char col, int itemindex, int extra)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_USE
		<< tab
		<< row
		<< col
		<< itemindex
		<< extra;
}

void ItemTakeMsg(CNetMsg& msg, CCharacter* ch, CItem* item)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_TAKE
		<< (char)ch->m_type
		<< ch->m_index
		<< item->m_index;
}

void ItemDeleteMsg(CNetMsg& msg, CItem* item)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_DELETE
		<< (char)item->tab()
		<< (char)item->row()
		<< (char)item->col();
		//<< item->m_index;
}

void ItemWearMsg(CNetMsg& msg, char wear_pos, CItem* item1, CItem* item2)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_WEAR
		<< (char)wear_pos;

	if (item1)
	{
		msg << (char)item1->tab()
			<< (char)item1->row()
			<< (char)item1->col()
			<< (int)item1->m_index;
#ifdef SET_ITEM
		msg << item1->m_flag;
#endif

	}
	else
	{
		msg << (char)0
			<< (char)0
			<< (char)0
			<< (int)-1;
#ifdef SET_ITEM
		msg << -1;
#endif
	}

	if (item2)
	{
		msg << (char)item2->tab()
			<< (char)item2->row()
			<< (char)item2->col()
			<< (int)item2->m_index;
#ifdef SET_ITEM
		msg << item2->m_flag;
#endif
	}
	else
	{
		msg << (char)0
			<< (char)0
			<< (char)0
			<< (int)-1;
#ifdef SET_ITEM
		msg << -1;
#endif
	}
}

void ItemSwapMsg(CNetMsg& msg, char tab_idx, char row_idx1, char col_idx1, int item_idx1, char row_idx2, char col_idx2, int item_idx2)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_SWAP
		<< (char)tab_idx
		<< (char)row_idx1 << (char)col_idx1// << item_idx1
		<< (char)row_idx2 << (char)col_idx2;// << item_idx2;
}

void ItemAddMsg(CNetMsg& msg, CItem* item)
{
	msg.Init(MSG_ITEM);
	
	msg << (unsigned char)MSG_ITEM_ADD
		<< (char)item->tab()
		<< (char)item->row()
		<< (char)item->col()
		<< item->m_index
		<< item->m_idNum
		<< item->m_wearPos
		<< item->m_plus
		<< item->m_flag
		<< item->m_used
#ifdef COMPOSITE_TIME		
		<< item->m_used_2
#endif	//COMPOSITE_TIME
		<< item->Count();
#ifdef SET_ITEM
		if (item->IsOriginItem())
		{
			msg << (char)item->GetOriginOptionCnt();
			for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
			{
				if (item->GetOriginIndex(i) > -1)
				{
					msg << (char)item->GetOriginIndex(i)
						<< (char)item->GetOriginLevel(i);
				}
			}

			msg << (char)item->GetBelongType()
			    << (char)item->GetOriginSkillCnt();
			for (i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
			{
				if (item->GetOriginIndex(i) > -1)
				{
					msg << item->GetOriginIndex(i)
						<< (char)item->GetOriginLevel(i);
				}
			}
		}
		else // IsOriginItem
#endif // SET_ITEM
		{
			msg << (char)item->m_nOption;
			
			int i;
			for (i=0; i < item->m_nOption; i++)
			{
				msg << (char)item->m_option[i].m_type
					<< (char)item->m_option[i].m_level;
			}
		}
}

void ItemUpdateMsg(CNetMsg& msg, CItem* item, LONGLONG changeCount)
{
	if (item == NULL)
	{
		msg.Init();
		return ;
	}

	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_UPDATE
		<< (char)item->tab()
		<< (char)item->row()
		<< (char)item->col()
		<< item->m_index
		<< item->m_plus
		<< item->m_flag
		<< item->m_used
#ifdef COMPOSITE_TIME		
		<< item->m_used_2
#endif	//COMPOSITE_TIME
		<< item->Count()
		<< changeCount;
#ifdef SET_ITEM
	if (item->IsOriginItem())
	{
		msg << (char)item->GetOriginOptionCnt();
		for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
		{
			if (item->GetOriginIndex(i) > -1)
			{
				msg << (char)item->GetOriginIndex(i)
					<< (char)item->GetOriginLevel(i);
			}
		}
		
		msg << (char)item->GetBelongType()
			<< (char)item->GetOriginSkillCnt();
		for (i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
		{
			if (item->GetOriginIndex(i) > -1)
			{
				msg << item->GetOriginIndex(i)
					<< (char)item->GetOriginLevel(i);
			}
		}
	}
	else // IsOriginItem
#endif // SET_ITEM
	{
		msg << (char)item->m_nOption;
		
		int i;
		for (i=0; i < item->m_nOption; i++)
		{
			msg << (char)item->m_option[i].m_type
				<< (char)item->m_option[i].m_level;
		}
	}
}

void ItemDropMsg(CNetMsg& msg, CCharacter* ch, CItem* item)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_DROP
		<< item->m_index
		<< item->m_idNum
		<< item->Count()
		<< GET_X(item)
		<< GET_Z(item);

	// 싱글던전 itemdrop
	if (ch->m_pZone->IsGuildRoom() || ch->m_pZone->IsPersonalDungeon())
		msg << item->m_pos.m_h + 1.0f;
	else
		msg << (GET_H(item) + 1.0f);

	msg	<< GET_R(item)
		<< GET_YLAYER(item)
		<< GET_CHAR_TYPE(ch)
		<< ch->m_index;
// #if defined (LC_KOR) || defined ( EVENT_XMAS_2007 ) // 모두 적용
	if( IS_NPC(ch) )
	{
		if( DEAD(ch) )	
			msg << (char)0;
		else			
			msg << (char)1;
	}
// #endif //  defined (LC_KOR) || defined ( EVENT_XMAS_2007 )
}

void ItemAppearMsg(CNetMsg& msg, CItem* item)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_APPEAR
		<< item->m_index
		<< item->m_idNum
		<< item->Count()
		<< GET_X(item)
		<< GET_Z(item)
		<< (GET_H(item) + 1.0f)
		<< GET_R(item)
		<< GET_YLAYER(item);
}

void ItemDisappearMsg(CNetMsg& msg, CItem* item)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_DISAPPEAR
		<< item->m_index;
}

void ItemUpgradeRepMsg(CNetMsg& msg, char result)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_UPGRADE_REP
		<< result;
}

void ItemRefineRepMsg(CNetMsg& msg, char result)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_REFINE_REP
		<< result;
}

void ItemOptionAddRepMsg(CNetMsg& msg, CItem* item, MSG_ITEM_OPTION_ADD_RESULT result)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_OPTION_ADD_REP
		<< (char)result;
}

void ItemOptionDelRepMsg(CNetMsg& msg, CItem* item, MSG_ITEM_OPTION_DEL_RESULT result)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_OPTION_DEL_REP
		<< (char)result;
}

void ItemProcessRepMsg(CNetMsg& msg, MSG_ITEM_PROCESS_RESULT result)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char) MSG_ITEM_PROCESS_REP
		<< (char)result;
}

void ItemMakeRepMsg(CNetMsg& msg, MSG_ITEM_MAKE_RESULT result)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char) MSG_ITEM_MAKE_REP
		<< (char)result;
}

void ItemMixRepMsg(CNetMsg& msg, MSG_ITEM_MIX_RESULT result)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char) MSG_ITEM_MIX_REP
		<< (char) result;
}

void ItemArcaneRepMsg(CNetMsg& msg, MSG_ITEM_ARCANE_RESULT result)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char) MSG_ITEM_ARCANE_REP
		<< (char) result;
}

void ItemGetMsg(CNetMsg& msg, CPC* ch, CItem* item)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char) MSG_ITEM_GET
		<< ch->GetName()
		<< item->m_itemProto->m_index
		<< item->Count();
}

#ifdef FEEITEM
void DBCharEndMsg(CNetMsg& msg, int remain0, int remain1, unsigned int nSeed)
{
	msg.Init(MSG_DB);
	msg << (unsigned char)MSG_DB_CHAR_END
		<< remain0
		<< remain1
		<< nSeed;
}
#else
void DBCharEndMsg(CNetMsg& msg)
{
	msg.Init(MSG_DB);
	msg << (unsigned char)MSG_DB_CHAR_END;
}
#endif // FEEITEM

void DBCharExistMsg(CNetMsg& msg, int index, const char* name, const char* nick, char job, char job2, char hairstyle, char facestyle, int level, LONGLONG ex, int sp, int hp, int maxhp, int mp, int maxmp, int wear[MAX_WEARING], int plus[MAX_WEARING], int nDeleteDelayRemain)
{
	int i;

	msg.Init(MSG_DB);

	if (hp > maxhp)
		hp = maxhp;
	if (mp > maxmp)
		mp = maxmp;

	// 캐릭터 정보

	msg	<< (unsigned char)MSG_DB_CHAR_EXIST
		<< index;
	if( (nick != NULL) && (strcmp(nick, "") != 0) )
		msg << nick;
	else
		msg	<< name;
	msg << job
#ifdef ENABLE_CHANGEJOB
		<< job2
#endif
		<< hairstyle
		<< facestyle
		<< level
		<< ex
		<< GetLevelupExp(level)
		<< sp
		<< hp
		<< maxhp
		<< mp
		<< maxmp;
	
	for (i = WEARING_SHOW_START; i <= WEARING_SHOW_END; i++)
	{
		msg << wear[i - WEARING_SHOW_START];
// 050223 : bs : plus 효과를 위한 정보 추가
		msg << plus[i - WEARING_SHOW_START];
// --- 050223 : bs : plus 효과를 위한 정보 추가
	}

#ifdef ENABLE_CHARACTER_DELETE_DELAY
	msg << nDeleteDelayRemain;
#endif // ENABLE_CHARACTER_DELETE_DELAY
}

void DBSuccessMsg(CNetMsg& msg)
{
	msg.Init(MSG_DB);
	msg << (unsigned char)MSG_DB_SUCCESS;
}

void DBOtherServerMsg(CNetMsg& msg, int zone, const char* ip, int port)
{
	msg.Init(MSG_DB);
	msg	<< (unsigned char)MSG_DB_OTHER_SERVER
		<< zone
		<< ip
		<< port;
}

void DBOKMsg(CNetMsg& msg, int zone)
{
	msg.Init(MSG_DB);
	msg << (unsigned char)MSG_DB_OK
		<< zone;
}

void MsgrRepWhisperNotFoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex, const char* sname)
{
	msg.Init(MSG_MSGR_REP);
	msg	<< seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_WHISPER_NOTFOUND
		<< sindex << sname;
}

void ChatWhisperMsg(CNetMsg& msg, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_CHAT);
	msg	<< (unsigned char)MSG_CHAT_WHISPER
		<< sindex << sname
		<< rname
		<< chat;
}

void MsgrRepWhisperRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_WHISPER_REP
		<< sindex << sname
		<< rname
		<< chat;
}

void SysWhisperNotFoundMsg(CNetMsg& msg)
{
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_WHISPER_NOTFOUND;
}

#ifdef ENABLE_MESSENGER
void MsgrRepMessengerChatRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_MESSENGERCHAT_REP
		<< sindex << sname
		<< rname
		<< chat;
}

void MsgrRepMessengerNotFoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex, const char* sname)
{
	msg.Init(MSG_MSGR_REP);
	msg	<< seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_MESSENGERCHAT_NOTFOUND
		<< sindex << sname;
}

void ChatMessengerMsg(CNetMsg& msg, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_CHAT);
	msg	<< (unsigned char)MSG_CHAT_MESSENGER
		<< sindex << sname
		<< rname
		<< chat;
}

void SysFriendNotFoundMsg(CNetMsg& msg)
{
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_FRIEND_NOTFOUND;
}

void MsgrFriendChatReqMsg(CNetMsg& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< (int)-1
		<< (unsigned char)MSG_MSGR_MESSENGERCHAT_REQ
		<< sindex << sname
		<< rname
		<< chat;
}

void FriendErrorMsg(CNetMsg& msg, MSG_FRIEND_ERROR_TYPE errcode)
{
	msg.Init(MSG_FRIEND);
	msg << (unsigned char)MSG_FRIEND_ERROR
		<< (char)errcode;
}

void FriendRegistCancelMsg(CNetMsg& msg)
{	
	msg.Init(MSG_FRIEND);
	msg << (unsigned char)MSG_FRIEND_REGIST_CANCEL;
}

void HelperNameChangeReqMsg(CNetMsg& msg, char bguild, int index, const char* reqname)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_NAME_CHANGE_REQ
		<< bguild
		<< index
		<< reqname;
}

void HelperFriendAddReqMsg(CNetMsg& msg, int approvalindex, const char* appname, int appjob, int requesterindex, const char* reqname, int reqjob)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_FRIEND_MEMBER_ADD_REQ		
		<< approvalindex
		<< appname
		<< appjob
		<< requesterindex
		<< reqname
		<< reqjob;
}

void HelperFriendDelReqMsg(CNetMsg& msg, int approvalindex, int requester, const char* name)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_FRIEND_MEMBER_DEL_REQ		
		<< approvalindex
		<< requester
		<< name;
}

void HelperBlockPCReqMsg(CNetMsg& msg, int reqIndex, const char* blockName)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_BLOCKPC_REQ		
		<< reqIndex
		<< blockName;
}

void BlockPCRepMsg(CNetMsg& msg, MSG_EX_MESSENGER_ERROR_TYPE errcode, int charIndex, const char* name)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_MESSENGER
		<< (unsigned char) MSG_EX_MESSENGER_BLOCK
		 << (unsigned char) errcode
		 << charIndex
		 << name;
}


void HelperFriendSetConditionMsg(CNetMsg& msg, int chaindex, int condition, int bReply, CPC* pc)
{
	CFriend* pfriend = NULL;
	if( pc )
		pfriend = pc->m_Friend;

	if(bReply==-1 && !pfriend)
	{
		msg.Init();
		return;
	}
	if(pfriend && pfriend->GetFriendSum() < 1)
	{
		msg.Init();
		return;
	}

	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_FRIEND_SET_CONDITION_REQ		
		<< chaindex
		<< condition;	
	

	msg << bReply; 

	if(bReply == -1)
	{
		
		int index;
		int sum;
		
		sum = pfriend->GetFriendSum();	
		
#ifdef MESSENGER_NEW
		CNetMsg IndexMsg;
		IndexMsg.Init();
		int blockSum = 0;

		for(int j=0; j< FRIEND_MAX_MEMBER; j++)
		{
			if(pfriend->GetFriendMember(j))
			{
				index = pfriend->GetFriendMember(j)->GetChaIndex();
				if( index != -1 )
				{
					if( !pc->IsBlockPC(index, pfriend->GetFriendMember(j)->GetName() ) )
					{
						IndexMsg << index;
					}
					else 
						blockSum++;
				}
			}
		}

		msg << sum - blockSum
			<< IndexMsg;
#else
		msg << sum;

		for(int j=0; j< FRIEND_MAX_MEMBER; j++)
		{
			if(pfriend->GetFriendMember(j))
			{
				index = pfriend->GetFriendMember(j)->GetChaIndex();
				if( index != -1 )
					msg << index;
			}
		}
#endif	

	}

}


void FriendSetConditionMsg(CNetMsg& msg, int requester, int condition)
{

	msg.Init(MSG_FRIEND);
	msg << (unsigned char)MSG_FRIEND_SET_CONDITION		
		<< requester		
		<< condition;
		
}

void MsgrMessengerChatReqMsg(CNetMsg& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< (int)-1
		<< (unsigned char)MSG_MSGR_MESSENGERCHAT_REQ
		<< sindex << sname
		<< rname
		<< chat;


}

void FriendAddNotify(CNetMsg& msg, CPC* pc)
{
	if (pc == NULL)
	{
		msg.Init(MSG_UNKNOWN);
		return;
	}

	msg.Init(MSG_FRIEND);
	msg << (unsigned char)MSG_FRIEND_REGIST_MEMBER_NOTIFY;
	msg << pc->m_index
		<< pc->GetName()
		<< (int)pc->m_job
		<< pc->m_nCondition;	
}

void FriendDelNotify(CNetMsg& msg, int delindex)
{
	msg.Init(MSG_FRIEND);
	msg << (unsigned char)MSG_FRIEND_DELETE_MEMBER_NOTIFY;
	msg << delindex;

}

#ifdef MESSENGER_NEW
void BlockPCListMsg(CNetMsg& msg, CDescriptor* d)
{
	CPC* pc = d->m_pChar;

	if( pc )
	{
		msg.Init(MSG_EXTEND);
		msg << MSG_EX_MESSENGER
			<< (unsigned char) MSG_EX_MESSENGER_BLOCK_LIST;

		int count = pc->m_listBlockPC.GetCount();

		msg << count;

		void* pos = pc->m_listBlockPC.GetHead();
		CBlockPC* blockPC = NULL;
		for(int i = 0;pos && i < count; i++ )
		{
			blockPC = pc->m_listBlockPC.GetData(pos);

			msg << blockPC->GetIndex()
				<< blockPC->GetName();

			pos = pc->m_listBlockPC.GetNext(pos);
		}

		SEND_Q(msg, d);
	}
}
#endif

#ifdef MSG_VER2
void ChatOneMessengerMsg(CNetMsg& msg, int sindex, const char* sname, int rindex, const char* rname, int chatcolor, const char* chat)
{	// sindex(n) sname(str) rindex(n) rname(str) chatcolor(n) chat(str)
	// sindex(n) rindex(n) chatcolor(n) char(str)
	msg.Init(MSG_EXTEND);
	msg	<< MSG_EX_MESSENGER
		<< (unsigned char)MSG_EX_MESSENGER_ONE_VS_ONE
		<< sindex 
		<< sname
		<< rindex
//		<< rname
		<< chatcolor
		<< chat;
}

void ChatOneOffLineMessengerMsg(CNetMsg& msg, int sindex, int rindex )
{
	msg.Init(MSG_EXTEND);
	msg	<< MSG_EX_MESSENGER
		<< (unsigned char)MSG_EX_MESSENGER_ONE_OFF_LINE
		<< sindex 
		<< rindex;
}

#endif // MSG_VER2

void FriendListMsg(CNetMsg& msg, CDescriptor* d)
{		
	CPC* pc = d->m_pChar;

	CFriendMember* pfriendmember;

	if(pc)
	{
		CFriend* pFriend = pc->m_Friend;
		
		if(pFriend)
		{
			int cnt = pFriend->GetFriendSum();	
			
			for (int i = 0; i < cnt; i++)
			{
				pfriendmember = pFriend->GetFriendMember(i);
				if (pfriendmember)
				{
					msg.Init(MSG_FRIEND);
					msg << (unsigned char)MSG_FRIEND_MEMBERLIST;
					msg << pfriendmember->GetChaIndex()
						<< pfriendmember->GetName()				
						<< pfriendmember->GetJob()		
						<< pfriendmember->GetCondition()
#ifdef MESSENGER_NEW
						<< pfriendmember->GetGroup()
#endif
						;
					SEND_Q(msg, d);
				}
			}
		}
	}
	
}

#endif

void MsgrConnectMsg(CNetMsg& msg, int version, int server, int subno, int count, int* zones)
{
	msg.Init(MSG_MSGR_CONNECT);
	msg << version
		<< server
		<< subno
		<< count;
	int i;
	for (i = 0; i < count; i++)
		msg << zones[i];
}

void MsgrWhisperReqMsg(CNetMsg& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< (int)-1
		<< (unsigned char)MSG_MSGR_WHISPER_REQ
		<< sindex << sname
		<< rname
		<< chat;
}

void MsgrGuildChatMsg(CNetMsg& msg, int seq, int server, int subno, int sindex, const char* sname, int guildindex, const char* chat)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< (int)-1
		<< (unsigned char)MSG_MSGR_GUILDCHAT
		<< sindex << sname
		<< guildindex
		<< chat;
}

#ifdef GMTOOL
void MsgGmKickIDCommandReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int thisServer, int thisSubno, int charindex, const char* id)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_GMTOOL_KICKID_REQ
		<< thisServer
		<< thisSubno
		<< charindex		// 지엠툴 캐릭터 인덱스
		<< id;
}

void MsgGmKickIDCommandRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* id)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< -1
		<< (unsigned char)MSG_MSGR_GMTOOL_KICKID_REP
		<< success
		<< charindex
		<< id;
}

void MsgGmKickCommandReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int thisServer, int thisSubno, int charindex, const char* name)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_GMTOOL_KICK_REQ
		<< thisServer
		<< thisSubno
		<< charindex		// 지엠툴 캐릭터 인덱스
		<< name;
}

void MsgGmKickCommandRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* name)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< -1
		<< (unsigned char)MSG_MSGR_GMTOOL_KICK_REP
		<< success
		<< charindex
		<< name;
}


void MsgrGmToolChatMonitorReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int gmcharindex, const char* name, const char* onoff)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_REQ
		<< gmcharindex
		<< name
		<< onoff;
}

void MsgrGmToolChatMonitorRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, int gmcharindex, const char* id)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_REP
		<< success
		<< gmcharindex
		<< id;
}

void MsgrNoticeGmChatMonitorMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int charindex, const char* name, unsigned char chattype)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE
		<< thisServer
		<< thisSub
		<< thisZone
		<< chat
		<< charindex
		<< name
		<< chattype;
}

void MsgrNoticeGmChatMonitorPartyMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int bossindex, const char* name, int charindex, const char* charname)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_PARTY
		<< thisServer
		<< thisSub
		<< thisZone
		<< chat
		<< bossindex
		<< name
		<< charindex
		<< charname;
}

void MsgrNoticeGmChatMonitorGuildMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int guildindex, const char* name, int charindex, const char* charname)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_GUILD
		<< thisServer
		<< thisSub
		<< thisZone
		<< chat
		<< guildindex
		<< name
		<< charindex
		<< charname;
}

void MsgrGmToolChatMonitorOffMsg(CNetMsg& rmsg, int seq, int server, int subno, int zone, int charindex)
{
	rmsg.Init(MSG_MSGR_REQ);
	rmsg << seq
		 << server
		 << subno
		 << zone
		 << (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_OFF
		 << charindex;
}

void MsgrGmToolChatMonitorPartyOffMsg(CNetMsg& rmsg, int seq, int server, int subno, int zone, int bossindex)
{
	rmsg.Init(MSG_MSGR_REQ);
	rmsg << seq
		 << server
		 << subno
		 << zone
		 << (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_PARTY_OFF
		 << bossindex;
}

void MsgrGmToolChatMonitorGuildOffMsg(CNetMsg& rmsg, int seq, int server, int subno, int zone, int guildindex)
{
	rmsg.Init(MSG_MSGR_REQ);
	rmsg << seq
		 << server
		 << subno
		 << zone
		 << (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_GUILD_OFF
		 << guildindex;
}


void MsgrNoticeGmChatMonitorWhisperMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int charindex, const char* name, unsigned char chattype, const char* sender)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_WHISPER
		<< thisServer
		<< thisSub
		<< thisZone
		<< chat
		<< charindex
		<< name
		<< chattype
		<< sender;
}

void MsgGMSilenceReq(CNetMsg& msg, int seq, int serverno, int subno, int gmcharindex, const char* charname, int sec)
{
	msg.Init(MSG_MSGR_REQ);
	
	msg << seq
		<< serverno
		<< subno
		<< -1
		<< (unsigned char)MSG_MSGR_GMTOOL_SILENCE_REQ
		<< gmcharindex
		<< charname
		<< sec;
}

void MsgGMSilenceRep(CNetMsg& msg, int seq, int thisServer, int thisSub, int success, int gmcharindex, const char* name)
{
	msg.Init(MSG_MSGR_REP);

	msg << seq
		<< 1
		<< 1
		<< -1
		<< (unsigned char)MSG_MSGR_GMTOOL_SILENCE_REP
		<< thisServer
		<< thisSub
		<< success
		<< gmcharindex
		<< name;
}

void MsgrRepGMToolWhisperNotFoundMsg(CNetMsg& msg, int nSeq, int nServer, int nSubno, int nZone, int charindex, const char* name)
{
	msg.Init(MSG_MSGR_REP);

	msg << nSeq
		<< nServer
		<< nSubno
		<< nZone
		<< (unsigned char)MSG_MSGR_GMTOOL_WHISPER_NOTFOUND
		<< charindex
		<< name;
}

void GMSayMsg(CNetMsg& msg, int server, int sub, int charindex, const char* name, const char* chat, unsigned char chattype)
{
	msg.Init(MSG_GM_CHAT);
	msg << server
		<< sub
		<< charindex
		<< name
		<< chat
		<< chattype;
}

void MsgrRepGMWhisperNotFoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex)
{
	msg.Init(MSG_MSGR_REP);
	msg	<< seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_GM_WHISPER_NOTFOUND
		<< sindex;
}

void MsgExtraGMCommandReq(CNetMsg& msg, int serverno, int subno, MSG_MSGR_TYPE msgtype)
{
	msg.Init(MSG_MSGR_REQ);

	msg << -1
		<< serverno
		<< subno
		<< 0
		<< (unsigned char)msgtype;
}

void MsgLoopCountRep(CNetMsg& msg, int serverno, int subno, int thisServer, int thisSub, const char* loopbuf)
{
	msg.Init(MSG_MSGR_REQ);
	msg << -1
		<< serverno
		<< subno
		<< 0
		<< (unsigned char)MSG_MSGR_GM_LOOPCOUNT_REP
		<< thisServer
		<< thisSub
		<< loopbuf;
}

void ChatGMWhisperMsg(CNetMsg& msg, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_CHAT);
	msg	<< (unsigned char)MSG_CHAT_GMTOOL
		<< sindex << sname
		<< rname
		<< chat;
}
void MsgrRepGMWhisperRepMsg(CNetMsg& msg, int nSeq, int server, int subno, int zone, int index, const char*  name, const char* chat, int thisServer, int thisSub, int sindex)
{
	msg.Init(MSG_MSGR_REP);
	msg << nSeq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_GM_WHISPER_REP
		<< index
		<< name
		<< chat
		<< thisServer
		<< thisSub
		<< sindex;
}

void MsgrGMWhisperReqMsg(CNetMsg& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< (int)-1
		<< (unsigned char)MSG_MSGR_GM_WHISPER_REQ
		<< sindex << sname
		<< rname
		<< chat;
}

void MsgrGMWhisperReqMsg(CNetMsg& msg, int seq, int server, int sub, const char* chat, int charindex, const char* name, int serverno, int subno, const char* receiver)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< sub
		<< -1
		<< (unsigned char)MSG_MSGR_GMTOOL_WHIPER_REQ
		<< chat
		<< charindex
		<< name
		<< serverno
		<< subno
		<< receiver;
}


void MsgrRepGmToolWhisperRepMsg(CNetMsg& msg, int seq, int server, int sub, int zone, int sindex, const char* sname, const char* rname, const char *chat)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< sub
		<< zone
		<< (unsigned char)MSG_MSGR_GMTOOL_WHISPER_REP
		<< sindex << sname
		<< rname
		<< chat;
}

void MsgrGMShutdownReqMsg(CNetMsg& msg, int server, int sub, int remain, const char* chat)
{
	msg.Init(MSG_MSGR_REQ);
	msg << -1
		<< server
		<< sub
		<< -1
		<< (unsigned char)MSG_MSGR_GMTOOL_SHUTDOWN
		<< remain
		<< chat;
}

#endif // GMTOOL

void SysFullInventoryMsg(CNetMsg& msg, char tab)
{
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_FULL_INVENTORY
		<< tab;
}

void SysMsg(CNetMsg& msg, MSG_SYS_TYPE systype)
{
	msg.Init(MSG_SYS);
	msg	<< (unsigned char)systype;
}

void SysCannotDuplcationMsg(CNetMsg& msg, int newIndex, int curIndex)
{
	msg.Init(MSG_SYS);
	msg	<< (unsigned char)MSG_SYS_CANNOT_DUPLICATION
		<< newIndex
		<< curIndex;
}

void SysBloodItemMsg(CNetMsg& msg, int itemdbindex)
{
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_BLOODITEM
		<< itemdbindex;
}

void GoZoneMsg(CNetMsg& msg, int zone, int extra, const char* ip, int port)
{
	msg.Init(MSG_GO_ZONE);
	msg << zone
		<< extra
		<< ip
		<< port;
}

void EnvWeatherMsg(CNetMsg& msg, int zoneNo)
{
	msg.Init(MSG_ENV);
	msg << (unsigned char)MSG_ENV_WEATHER
		<< gserver.m_zones[zoneNo].m_weather;
}

void EnvTimeMsg(CNetMsg& msg)
{
	msg.Init(MSG_ENV);

#ifdef LC_TIME

	// 현실시간 2분30초 == 1시간 // 현실시간 1시간 == 하루	   gameHour = gameTime / 150;
	// 하루 24시간				  gameDay = gameHour * 24; 
	// 한달 30일				gameMon = gameDay * 30;
	// 일년 360일				gameYear = gameMon * 12;
	int gameTime = gserver.m_gameTime - gserver.m_lcStartTime;
	int gameYear = gameTime / (150 * 24 * 360); // 하루 * 360일
	gameTime %= 150 * 24 * 360;
	char gameMon = gameTime / (150 * 24 * 30); // 하루 * 30일
	gameTime %= 150 * 24 * 30;
	char gameDay = gameTime / (150 * 24); // 하루
	gameTime %= 150 * 24;
	char gameHour = gameTime / 150;	

	msg << (unsigned char)MSG_ENV_TIME
		<< gameYear << gameMon << gameDay << gameHour << gserver.m_lcStartTime;

#else

	static const int nSecPerMin = 60;
	static const int nSecPerHour = nSecPerMin * 60;
	static const int nSecPerDay = nSecPerHour * 30;
	static const int nSecPerMon = nSecPerDay * 30;
	static const int nSecPerYear = nSecPerMon * 12;

	LONGLONG gameTime = (LONGLONG)(gserver.m_gameTime) * 60;

	LONGLONG year, mon, day, hour, min, sec;
	// 하루:30시간 한달:30일 일년:360일
	year = gameTime / nSecPerYear;
	gameTime %= nSecPerYear;
	mon = gameTime / nSecPerMon;
	gameTime %= nSecPerMon;
	day = gameTime / nSecPerDay;
	gameTime %= nSecPerDay;
	hour = gameTime / nSecPerHour;
	gameTime %= nSecPerHour;
	min = gameTime / nSecPerMin;
	gameTime %= nSecPerMin;
	sec = gameTime;

	msg << (unsigned char)MSG_ENV_TIME
		<< (int)year << (char)mon << (char)day << (char)hour << (char)min << (char)sec;

#endif
}

void EnvTaxChangeMsg(CNetMsg& msg, int zoneNo)
{
	msg.Init(MSG_ENV);
	msg << (unsigned char)MSG_ENV_TAX_CHANGE
		<< zoneNo
		<< gserver.m_zones[zoneNo].m_sellRate
		<< gserver.m_zones[zoneNo].m_buyRate;
}

void ExchangeReqMsg(CNetMsg& msg, MSG_EXCHANGE_REQ_TYPE reqType, CPC* src, CPC* dest)
{
	msg.Init(MSG_EXCHANGE);

	msg << (unsigned char)MSG_EXCHANGE_REQ
		<< (unsigned char)reqType;
	if (reqType == MSG_EXCHANGE_REQ_REQ)
	{
		msg << src->m_index
			<< src->GetName()
			<< dest->m_index
			<< dest->GetName();
	}
}

void ExchangeItemMsg(CNetMsg& msg, MSG_EXCHANGE_ITEM_ACTION action, CItem* item, LONGLONG count)
{
	msg.Init(MSG_EXCHANGE);

	msg << (unsigned char)MSG_EXCHANGE_ITEM
		<< (unsigned char)action
		<< item->m_index;
	switch (action)
	{
	case MSG_EXCHANGE_ITEM_DEL_SRC:
	case MSG_EXCHANGE_ITEM_DEL_DEST:
		return ;
	default:
		msg << count;
		break;
	}

	switch (action)
	{
	case MSG_EXCHANGE_ITEM_ADD:
	case MSG_EXCHANGE_ITEM_ADD_SRC:
	case MSG_EXCHANGE_ITEM_ADD_DEST:
		msg << item->m_idNum << item->m_plus << item->m_flag << item->m_used;
#ifdef COMPOSITE_TIME		
		msg << item->m_used_2;
#endif	//COMPOSITE_TIME
#ifdef SET_ITEM
			if (item->IsOriginItem())
			{
				msg << (char)item->GetOriginOptionCnt();
				for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
				{
					if (item->GetOriginIndex(i) > -1)
					{
						msg << (char)item->GetOriginIndex(i)
							<< (char)item->GetOriginLevel(i);
					}
				}

				msg << (char)item->GetBelongType()
					<< (char)item->GetOriginSkillCnt();
				for (i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
				{
					if (item->GetOriginIndex(i) > -1)
					{
						msg << item->GetOriginIndex(i)
							<< (char)item->GetOriginLevel(i);
					}
				}
			}
			else // IsOriginItem
#endif // SET_ITEM
			{
				msg << (char)item->m_nOption;
				int i;
				for (i=0; i < item->m_nOption; i++)
				{
					msg << (char)item->m_option[i].m_type
						<< (char)item->m_option[i].m_level;
				}
			}
			break;
	default:
		break;
	}
}

void MemPosListMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_MEMPOS);
	msg << (unsigned char)MSG_MEMPOS_LIST
		<< (unsigned char)ch->m_mempos.m_count;
	for (int i = 0; i < MAX_MEMPOS; i++)
	{
		if (ch->m_mempos.m_data[i])
		{
			msg << (unsigned char)i
				<< ch->m_mempos.m_data[i]->m_zone
				<< ch->m_mempos.m_data[i]->m_x
				<< ch->m_mempos.m_data[i]->m_z
				<< ch->m_mempos.m_data[i]->m_comment;
		}
	}

	int remain = 0;
	remain = ( ch->m_memposTime - gserver.m_gameTime );
	if(remain <= 0 ) 
	{
		remain = 0;
	}
	else
	{
		remain = remain / OUTPUTDAY;
		if(remain == 0)
		{
			remain = 1;
		}
	}

	msg << remain;
}

void MemPosWriteMsg(CNetMsg& msg, CPC* ch, int slot)
{
	msg.Init(MSG_MEMPOS);
	msg << (unsigned char)MSG_MEMPOS_WRITE
		<< (unsigned char)slot
		<< ch->m_mempos.m_data[slot]->m_comment
		<< ch->m_mempos.m_data[slot]->m_zone
		<< ch->m_mempos.m_data[slot]->m_x
		<< ch->m_mempos.m_data[slot]->m_z;
}

#ifdef PRIMIUM_MEMORYBOOK
void MemPosPlusListMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_EXTEND);
	msg << (unsigned long)MSG_EX_MEMPOSPLUS
		<< (unsigned char)MSG_EX_MEMPOSPLUS_LIST
		<< (unsigned char)ch->m_memposplus.m_count;
	for (int i = 0; i < MAX_MEMPOS; i++)
	{
		if (ch->m_memposplus.m_data[i])
		{
			msg << (unsigned char)i
				<< ch->m_memposplus.m_data[i]->m_zone
				<< ch->m_memposplus.m_data[i]->m_x
				<< ch->m_memposplus.m_data[i]->m_z
				<< ch->m_memposplus.m_data[i]->m_comment;
		}
	}

	int remain = -1;
	msg << remain;
}

void MemPosPlusWriteMsg(CNetMsg& msg, CPC* ch, int slot)
{
	msg.Init(MSG_EXTEND);
	msg << (unsigned long)MSG_EX_MEMPOSPLUS
		<< (unsigned char)MSG_EX_MEMPOSPLUS_WRITE
		<< (unsigned char)slot
		<< ch->m_memposplus.m_data[slot]->m_comment
		<< ch->m_memposplus.m_data[slot]->m_zone
		<< ch->m_memposplus.m_data[slot]->m_x
		<< ch->m_memposplus.m_data[slot]->m_z;
}
#endif // PRIMIUM_MEMORYBOOK

void GMWhoAmIMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_GM);
	msg << (unsigned char)MSG_GM_WHOAMI
		<< (unsigned char)ch->m_admin;
}

void GMCommandMsg(CNetMsg& msg, const char* cmd)
{
	msg.Init(MSG_GM);
	msg << (unsigned char)MSG_GM_COMMAND
		<< cmd;
}

void MsgrNoticeMsg(CNetMsg& msg, int seq, int server, int subno, int zone, const char* chat)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_NOTICECHAT
		<< chat;
}

void MsgrLogoutReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, const char* id)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_LOGOUT_REQ
		<< id;
}

void MsgrLogoutRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, char success, const char* id)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_LOGOUT_REP
		<< success
		<< id;
}

void MsgrEndGameReqMsg(CNetMsg& msg)
{
	msg.Init(MSG_MSGR_REQ);
	msg << (int)-1
		<< (int)-1
		<< (int)-1
		<< (int)-1
		<< (unsigned char)MSG_MSGR_ENDGAME_REQ;
}

void MsgrRebootReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_REBOOT_REQ;
}

void MsgrObserverMsg(CNetMsg& msg, int seq, int server, int subno, int zone, const char* chat)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_OBSERVER
		<< chat;
}

#ifdef NEW_DOUBLE_GM
void MsgrDoubleEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int percent[])
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_EVENT_REQ
		<< charindex
		<< cmd;

	for(int i = 0; i < 6; i++)
	{
		msg << percent[i];
	}
}

void MsgrDoubleEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_EVENT_REP
		<< charindex
		<< gserver.m_serverno
		<< gserver.m_subno
		<< cmd
		<< gserver.m_bDoubleNasPercent
		<< gserver.m_bDoubleNasGetPercent
		<< gserver.m_bDoubleExpPercent
		<< gserver.m_bDoubleSpPercent
		<< gserver.m_bDoubleProducePercent
		<< gserver.m_bDoubleProduceNum;
}

#ifdef NEW_DOUBLE_EVENT_AUTO
void MsgrDoubleEventAutoReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char cmd)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_EVENT_AUTO_REQ
		<< charindex
		<< cmd;
}

void MsgrDoubleEventAutoRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char cmd, char state)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_EVENT_AUTO_REP
		<< charindex
		<< gserver.m_serverno
		<< gserver.m_subno
		<< cmd
		<< state;
}
#endif // NEW_DOUBLE_EVENT_AUTO

#else
void MsgrDoubleEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, bool bStart)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_EVENT_REQ
		<< (char) bStart;
}
#endif

#ifdef CHANCE_EVENT
void MsgrChanceEventReqMsg(CNetMsg & msg, int seq, int server, int subno, int zone, int charindex, int cmd, int slevel, int elevel, int percent[])
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_CHANCE_EVENT_REQ
		<< charindex	
		<< cmd
		<< slevel
		<< elevel;

	for(int i = 0; i < 6; i++)
	{
		msg << percent[i];
	}
}

void MsgrChanceEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_CHANCE_EVENT_REP
		<< charindex
		<< gserver.m_serverno
		<< gserver.m_subno
		<< cmd
		<< gserver.m_bChanceSlevel
		<< gserver.m_bChanceElevel
		<< gserver.m_bChanceNasPercent
		<< gserver.m_bChanceNasGetPercent
		<< gserver.m_bChanceExpPercent
		<< gserver.m_bChanceSpPercent
		<< gserver.m_bChanceProducePercent
		<< gserver.m_bChanceProduceNum;
}
#endif // CHANCE_EVENT

void MsgrDoubleExpEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int expPercent)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_EXP_EVENT_REQ
		<< charindex
		<< cmd
		<< expPercent;
}

void MsgrDoubleExpEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int expPercent)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_EXP_EVENT_REP
		<< charindex
		<< gserver.m_serverno
		<< gserver.m_subno
		<< cmd
		<< expPercent;
}

void MsgrLattoEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_LATTO_EVENT_REQ
		<< (char) bStart
		<< thisServer
		<< thisSubno
		<< charIndex;
}

void MsgrNewYearEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_NEWYEAR_EVENT_REQ
		<< (char) bStart
		<< thisServer
		<< thisSubno
		<< charIndex;
}

void MsgrValentineEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_VALENTINE_EVENT_REQ
		<< (char) bStart
		<< thisServer
		<< thisSubno
		<< charIndex;
}

void MsgrWhiteDayEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_WHITEDAY_EVENT_REQ
		<< (char) bStart
		<< thisServer
		<< thisSubno
		<< charIndex;
}

void MsgrLetterEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_LETTER_EVENT_REQ
		<< (char) bStart
		<< thisServer
		<< thisSubno
		<< charIndex;
}



void QuickSlotAddMsg(CNetMsg& msg, CPC* ch, char page, char slot)
{
	msg.Init(MSG_QUICKSLOT);

	msg << (unsigned char)MSG_QUICKSLOT_ADD
		<< page
		<< slot;

	switch (ch->m_quickSlot[(int)page].m_slotType[(int)slot])
	{	
	case QUICKSLOT_TYPE_SKILL:
		msg << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
			<< ch->m_quickSlot[(int)page].m_skillType[(int)slot];
		break;
		
	case QUICKSLOT_TYPE_ACTION:
		msg << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
			<< ch->m_quickSlot[(int)page].m_actionType[(int)slot];
		break;
		
	case QUICKSLOT_TYPE_ITEM:
		if (ch->m_quickSlot[(int)page].m_item[(int)slot])
		{
			msg << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
				<< (char)ch->m_quickSlot[(int)page].m_item[(int)slot]->row()
				<< (char)ch->m_quickSlot[(int)page].m_item[(int)slot]->col();
		}
		else
			msg << (char)QUICKSLOT_TYPE_EMPTY;
		break;

	case QUICKSLOT_TYPE_EMPTY:
	default:
		msg << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot];
		break;
	}

}

void QuickSlotListMsg(CNetMsg& msg, CPC* ch, char page)
{
	int slot;

	msg.Init(MSG_QUICKSLOT);

	msg << (unsigned char)MSG_QUICKSLOT_LIST
		<< page;

	for (slot=0; slot < QUICKSLOT_MAXSLOT; slot++)
	{
		switch (ch->m_quickSlot[(int)page].m_slotType[(int)slot])
		{	
		case QUICKSLOT_TYPE_SKILL:
			msg << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
				<< ch->m_quickSlot[(int)page].m_skillType[(int)slot];
			break;
			
		case QUICKSLOT_TYPE_ACTION:
			msg << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
				<< ch->m_quickSlot[(int)page].m_actionType[(int)slot];
			break;
			
		case QUICKSLOT_TYPE_ITEM:
			if (ch->m_quickSlot[(int)page].m_item[(int)slot])
			{
				msg << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot]
					<< (char)ch->m_quickSlot[(int)page].m_item[(int)slot]->row()
					<< (char)ch->m_quickSlot[(int)page].m_item[(int)slot]->col();
			}
			else
				msg << (char)QUICKSLOT_TYPE_EMPTY;
			break;

		case QUICKSLOT_TYPE_EMPTY:
		default:
			msg << (char)ch->m_quickSlot[(int)page].m_slotType[(int)slot];
			break;
		}
	}
}

void PartyInviteMsg(CNetMsg& msg, char nPartyType, int nBossIndex, const char* strBossName)
{
	msg.Init(MSG_PARTY);
	msg << (unsigned char)MSG_PARTY_INVITE
		<< nPartyType
		<< nBossIndex
		<< strBossName;
}

void PartyMsg(CNetMsg& msg, MSG_PARTY_TYPE subtype)
{
	msg.Init(MSG_PARTY);
	msg << (unsigned char)subtype;
}

void PartyAddMsg(CNetMsg& msg, int nCharIndex, const char* strCharName, CPC* tch, char isboss)
{
	msg.Init(MSG_PARTY);
	msg << (unsigned char)MSG_PARTY_ADD
		<< isboss
		<< nCharIndex
		<< strCharName;
	if (tch)
	{
		msg << tch->m_job
#ifdef ENABLE_CHANGEJOB
		<< tch->m_job2
#endif
		<< tch->m_level
		<< tch->m_hp << tch->m_maxHP
		<< tch->m_mp << tch->m_maxMP
		<< GET_X(tch)
		<< GET_Z(tch)
			<< GET_YLAYER(tch)
			<< tch->m_pZone->m_index;
	}
	else
	{
		msg << (char)0
#ifdef ENABLE_CHANGEJOB
			<< (char)0
#endif
			<< (int)1
			<< (int)1 << (int)1
			<< (int)1 << (int)1
			<< (int)0
			<< (int)0
			<< (int)0
			<< (int)0;
	}
}

void PartyDelMsg(CNetMsg& msg, bool bKick, int nTargetIndex)
{
	msg.Init(MSG_PARTY);
	if (bKick)
		msg << (unsigned char)MSG_PARTY_KICK;
	else
		msg << (unsigned char)MSG_PARTY_QUIT;
	msg << nTargetIndex;
}

void PartyInfoMsg(CNetMsg& msg, int nIndex, int nLevel, int nHP, int nMaxHP, int nMP, int nMaxMP, float x, float z, char nYlayer, int nZone)
{
	msg.Init(MSG_PARTY);
	msg << (unsigned char)MSG_PARTY_MEMBER_INFO
		<< nIndex
		<< nLevel
		<< nHP
		<< nMaxHP
		<< nMP
		<< nMaxMP
		<< x
		<< z
		<< nYlayer
		<< nZone;		// TODO : PARTY : 메시지 수정
}

void PartyErrorMsg(CNetMsg& msg, MSG_PARTY_ERROR_TYPE partyerror)
{
	msg.Init(MSG_PARTY);
	msg << (unsigned char)MSG_PARTY_ERROR
		<< (unsigned char)partyerror;
}

void PartyAssistInfoMsg(CNetMsg& msg, CPC* tch)
{
	msg.Init(MSG_PARTY);
	msg << (unsigned char)MSG_PARTY_ASSISTINFO;
	if (tch)
		tch->m_assist.AppendAssistToNetMsg(msg);
	else
	{
		msg << (int)0
			<< (char)0;
	}
}

void PartyChangeBoss(CNetMsg& msg, const char* bossname, int nNewBossIndex, const char* newname, bool bMandate)
{
	msg.Init(MSG_PARTY);
	msg << (unsigned char) MSG_PARTY_CHANGEBOSS
		<< bossname
		<< nNewBossIndex
		<< newname
		<< (char)bMandate;
}

#ifdef NEW_UI
void PartyChangeType(CNetMsg& msg, char cPartyType, char cDiviType)
{
	msg.Init(MSG_PARTY);
	msg << (unsigned char) MSG_PARTY_CHANGETYPE
		<< cPartyType
		<< cDiviType;
}
#endif // NEW_UI


void ConnConnectMsg(CNetMsg& msg, int version, int server, int subno, int count, int* zones)
{
	msg.Init(MSG_CONN_CONNECT);
	msg << version
		<< server
		<< subno
		<< gserver.m_serverAddr
		<< gserver.m_serverPort
		<< count;
	int i;
	for (i = 0; i < count; i++)
		msg << zones[i];
}

void ConnLoginMsg(CNetMsg& msg, CDescriptor* d)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_LOGIN_REQ
		<< d->m_idname
		<< d->m_passwd
		<< gserver.m_serverno
		<< gserver.m_subno
		<< (unsigned char)d->m_playmode
		<< d->m_host;
}

void ConnLogoutMsg(CNetMsg& msg, const char* id)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_LOGOUT_REQ
		<< id;
}

void ConnPlayingMsg(CNetMsg& msg, CDescriptor* d, MSG_LOGIN_TYPE mode)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_PLAYING
		<< d->m_idname
		<< (unsigned char)mode
		<< d->m_pChar->m_pZone->m_index;
}

void NPCRegenMsg(CNetMsg& msg, CNPC* npc, int entityIndex)
{
	msg.Init(MSG_NPC_REGEN);

	msg << npc->m_index
		<< npc->m_idNum
		<< GET_X(npc)
		<< GET_Z(npc)
		// TODO : 높이맵에 따른 height 값 조정
		<< npc->m_pos.m_h //GET_H(npc)
		<< GET_R(npc)
		<< GET_YLAYER(npc)
		<< entityIndex;
}

void SkillListMsg(CNetMsg& msg, CPC* ch)
{
	char count = ch->m_activeSkillList.count();
	count += ch->m_passiveSkillList.count();
	count += ch->m_etcSkillList.count();

	msg.Init(MSG_SKILL);

	msg << (unsigned char)MSG_SKILL_LIST
		<< count;

	void* pos;

	pos = ch->m_activeSkillList.GetHeadPosition();
	while (pos)
	{
		CSkill* p = ch->m_activeSkillList.GetNext(pos);
		if (p)
		{
			msg << p->m_proto->m_index
				<< p->m_level;
		}
	}

	pos = ch->m_passiveSkillList.GetHeadPosition();
	while (pos)
	{
		CSkill* p = ch->m_passiveSkillList.GetNext(pos);
		if (p)
		{
			msg << p->m_proto->m_index
				<< p->m_level;
		}
	}

	pos = ch->m_etcSkillList.GetHeadPosition();
	while (pos)
	{
		CSkill* p = ch->m_etcSkillList.GetNext(pos);
		if (p)
		{
			msg << p->m_proto->m_index
				<< p->m_level;
		}
	}
	
#ifdef FACTORY_SYSTEM
	pos = ch->m_sealSkillList.GetHeadPosition();
	while (pos)
	{
		CSkill* p = ch->m_sealSkillList.GetNext(pos);
		if (p)
		{
			msg << p->m_proto->m_index
				<< ch->GetSealExp(p->m_proto->m_index);
		}
	}
#endif
}

void SkillLearnErrorMsg(CNetMsg& msg, MSG_SKILL_LEARN_ERROR_TYPE errorcode)
{
	msg.Init(MSG_SKILL);
	msg << (unsigned char)MSG_SKILL_LEARN_ERROR
		<< (unsigned char)errorcode;
}

void SkillLearnMsg(CNetMsg& msg, CSkill* skill)
{
	msg.Init(MSG_SKILL);

	msg << (unsigned char)MSG_SKILL_LEARN
		<< skill->m_proto->m_index
		<< (char)((skill->m_level == 1) ? 1 : 0)
		<< skill->m_level;
}

void SkillReadyMsg(CNetMsg& msg, CCharacter* ch, CSkill* skill, CCharacter* tch)
{
	// 스킬이 셀프, 파티 타입이면 tch를 ch로 사용
	switch (skill->m_proto->m_targetType)
	{
	case STT_SELF_ONE:
	case STT_SELF_RANGE:
	case STT_PARTY_ALL:
		tch = ch;
		break;
	}

	msg.Init(MSG_SKILL);

	msg << (unsigned char)MSG_SKILL_READY
		<< (char)(ch->m_type)
		<< ch->m_index
		<< skill->m_proto->m_index;
	if (tch == NULL)
		msg << (char)MSG_CHAR_UNKNOWN
			<< (int)-1;
	else
		msg << (char)(tch->m_type)
			<< tch->m_index;

	msg << ch->m_skillSpeed;
}

void SkillCancelMsg(CNetMsg& msg, CCharacter* ch)
{
	msg.Init(MSG_SKILL);

	msg << (unsigned char)MSG_SKILL_CANCEL
		<< (char)(ch->m_type)
		<< ch->m_index;
}

void SkillFireMsg(CNetMsg& msg, CCharacter* ch, CSkill* skill, CCharacter* tch, char count, char* mtargettype, int* mtargetindex, char cMoveChar, float x, float z, float h, float r, char yLayer)
{
	// 스킬이 셀프, 파티 타입이면 tch를 ch로 사용
	switch (skill->m_proto->m_targetType)
	{
	case STT_SELF_ONE:
	case STT_SELF_RANGE:
	case STT_PARTY_ALL:
		tch = ch;
		break;
	}

	msg.Init(MSG_SKILL);

	msg << (unsigned char)MSG_SKILL_FIRE
		<< (char)(ch->m_type)
		<< ch->m_index
		<< skill->m_proto->m_index;
	if (tch == NULL)
		msg << (char)MSG_CHAR_UNKNOWN
			<< (int)-1;
	else
		msg << (char)(tch->m_type)
			<< tch->m_index;

	msg << count;

	int i;
	for (i = 0; i < count; i++)
	{
		msg << mtargettype[i]
			<< mtargetindex[i];
	}

	msg << ch->m_skillSpeed
		<< cMoveChar
		<< x
		<< z
		<< h
		<< r
		<< yLayer;
}

void SkillErrorMsg(CNetMsg& msg, MSG_SKILL_ERROR_TYPE errcode, int skillindex, int extraInfo)
{
	msg.Init(MSG_SKILL);
	msg << (unsigned char)MSG_SKILL_ERROR
		<< (char)errcode
		<< skillindex
		<< extraInfo;
}

void SkillAutoUseMsg(CNetMsg& msg, int skillindex)
{
	msg.Init(MSG_SKILL);
	msg << (unsigned char) MSG_SKILL_AUTO_USE
		<< skillindex;
}
void MsgrPlayerCountReq(CNetMsg& msg, int server, int subno, int charidx)
{
	msg.Init(MSG_MSGR_REQ);

	msg << 0
		<< server
		<< subno
		<< (int)-1
		<< (unsigned char)MSG_MSGR_PLAYER_COUNT_REQ
		<< charidx;
}

void MsgrPlayerCountRep(CNetMsg& msg, int seq, int server, int subno, int zone, int charidx, int countZone, int* countPC, int* countProduce, int* countShop, int* countPlay)
{
	msg.Init(MSG_MSGR_REP);

	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_MSGR_PLAYER_COUNT_REP
		<< charidx
		<< countZone;

	int i;
	for (i = 0; i < gserver.m_numZone; i++)
	{
		if (countPC[i] == 0)
			continue ;

		msg << gserver.m_zones[i].m_index
			<< countPC[i]
			<< countProduce[i]
			<< countShop[i]
			<< countPlay[i];
	}
}

void AssistAddMsg(CNetMsg& msg, CCharacter* ch, int itemidx, int index, char level, int remain)
{
	msg.Init(MSG_ASSIST);

	msg << (unsigned char)MSG_ASSIST_ADD
		<< GET_CHAR_TYPE(ch)
		<< ch->m_index
		<< itemidx
		<< index
		<< level
		<< remain;
}

void AssistDelMsg(CNetMsg& msg, CCharacter* ch, int itemidx, int index)
{
	msg.Init(MSG_ASSIST);

	msg << (unsigned char)MSG_ASSIST_DEL
		<< GET_CHAR_TYPE(ch)
		<< ch->m_index
		<< itemidx
		<< index;
}

void AssistListMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_ASSIST);

	msg << (unsigned char)MSG_ASSIST_LIST;
	ch->m_assist.AppendAssistToNetMsg(msg);
}

void CharStatusMsg(CNetMsg& msg, CCharacter* ch, int state2)
{
	msg.Init(MSG_CHAR_STATUS);

	msg << GET_CHAR_TYPE(ch)
		<< ch->m_index
		<< ch->m_hp
		<< ch->m_maxHP
		<< ch->m_mp
		<< ch->m_maxMP;
	if (IS_PC(ch))
	{
		CPC* pc = TO_PC(ch);
		msg << pc->m_pkPenalty;
		msg << (char)pc->GetPKName();
	}
	else if (IS_NPC(ch))
	{
		msg << (int)0;
		msg << (char)0;
	}
	else
	{
		msg << (int)0;
		msg << (char)0;
	}
	msg << ch->m_assist.m_state
		<< state2;
}

void RebirthMsg(CNetMsg& msg, CCharacter* ch)
{
	msg.Init(MSG_PC_REBIRTH);

	msg << GET_CHAR_TYPE(ch)
		<< ch->m_index
		<< ch->m_hp << ch->m_maxHP
		<< ch->m_mp << ch->m_maxMP;
}

void EffectEtcMsg(CNetMsg& msg, CCharacter* ch, MSG_EFFECT_ETC_TYPE effectType)
{
	msg.Init(MSG_EFFECT);

	msg << (unsigned char) MSG_EFFECT_ETC
		<< GET_CHAR_TYPE(ch)
		<< ch->m_index
		<< (unsigned char)effectType;
}

void EffectSkillMsg(CNetMsg& msg, CCharacter* ch, const CSkillProto* proto)
{
	msg.Init(MSG_EFFECT);

	msg << (unsigned char) MSG_EFFECT_SKILL
		<< proto->m_index
		<< GET_CHAR_TYPE(ch)
		<< ch->m_index;
}

void EffectProduceMsg(CNetMsg& msg, CCharacter* ch, CCharacter* tch, MSG_PRODUCE_KIND kind)
{
	msg.Init(MSG_EFFECT);

	msg << (unsigned char) MSG_EFFECT_PRODUCE
		<< (char)kind
		<< GET_CHAR_TYPE(ch)
		<< ch->m_index
		<< GET_CHAR_TYPE(tch)
		<< tch->m_index
		<< tch->m_hp;
}

void EffectItemMsg(CNetMsg& msg, CCharacter* ch, CItem* item)
{
	msg.Init(MSG_EFFECT);

	msg << (unsigned char) MSG_EFFECT_ITEM
		<< GET_CHAR_TYPE(ch)
		<< ch->m_index
		<< item->m_itemProto->m_index;
}

#ifdef MONSTER_COMBO
void EffectFireReadyMsg(CNetMsg& msg, int extra, int count, int* effectNPC, float* x, float* z, float* h)  
{
	msg.Init(MSG_EFFECT);
	msg << (unsigned char) MSG_EFFECT_FIRE
		<< extra
		<< count;

	int i;
	for(i = 0 ; i < count; ++i)
	{
		msg << effectNPC[i] << x[i] << z[i] << h[i];
	}
}
#endif 

void ConnRebootReqMsg(CNetMsg& msg)
{
	msg.Init(MSG_CONN_REQ);

	msg << (unsigned char)MSG_CONN_REBOOT_REQ;
}


void ConnPreopenGiftMsg(CNetMsg& msg, int userindex, int charindex, bool bCancel)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_PREOPEN_GIFT
		<< userindex
		<< charindex;
	if (bCancel)
		msg << (char)1;
	else
		msg << (char)0;
}


void ConnLevelupPointMsg(CNetMsg& msg, int userindex, int point)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_LEVELUP_POINT
		<< userindex
		<< point;
}

/////////////////////////////////////////////
// BANGWALL : 2005-07-05 오후 2:35:40
// Comment : 태섭적용 

void ConnStashDeletePassword(CNetMsg& msg, int userindex, int charindex, const char* idnum)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_STASH_DELETE_PASSWORD
		<< userindex
		<< charindex
		<< idnum;
}


void ConnStashState(CNetMsg& msg, int userindex, int charindex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_STASH_STATE
		<< userindex
		<< charindex;
}


void ConnStashCheckPassword(CNetMsg& msg, int userindex, int charindex, const char* pw)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_STASH_CHECK_PASSWORD
		<< userindex
		<< charindex
		<< pw;
}


void ConnStashChangePassword(CNetMsg& msg, int userindex, int charindex, const char* oldpw, const char* newpw)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_STASH_CHANGE_PASSWORD
		<< userindex
		<< charindex
		<< oldpw
		<< newpw;
}

void ConnStashSetNewPassword(CNetMsg& msg, int userindex, int charindex, const char* newpw, const char* solpw)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_STASH_SET_NEW_PASSWORD
		<< userindex
		<< charindex
		<< newpw;
	if( solpw )
		msg << solpw;
}


void ConnStashSeal(CNetMsg& msg, int userindex, int charindex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_STASH_SEAL
		<< userindex
		<< charindex;
}


#ifdef FEEITEM
#ifdef CASH_ITEM_GIFT
void ConnCashItemPresentHistoryReq(CNetMsg& msg, bool bSend, int userindex, int charindex, int y, char m, char d)
{
	msg.Init(MSG_CONN_REQ);
	if( bSend )
		msg << (unsigned char) MSG_CONN_CASHITEM_GIFT_SENDHISTORY;
	else
		msg << (unsigned char) MSG_CONN_CASHITEM_GIFT_RECVHISTORY;

	msg	<< userindex
		<< charindex
		<< y
		<< m 
		<< d;
}

void ConnCashItemGiftReqMsg(CNetMsg& msg, int sendUserIndex, int sendCharIndex, const char* sendCharName, const char* sendMsg, int recvuserIndex, int recvcharIndex, const char* recvcharName, int count, int idx[], int ctid[])
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_CASHITEM_GIFT
			<< sendUserIndex
			<< sendCharIndex
		    << sendCharName
			<< sendMsg
			<< recvuserIndex
			<< recvcharIndex
			<< recvcharName
			<< count;

	for(int  i = 0; i < count; i++ )
	{
		msg << idx[i]
			<< ctid[i];
	}
}

void ConnCashItemGiftRecvListReqMsg(CNetMsg& msg, int userIndex, int charIndex )
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_CASHITEM_GIFT_RECVLIST
		<< userIndex
		<< charIndex;
}

#endif //  CASH_ITEM_GIFT

void ConnCashItemPurchaseHistoryReq(CNetMsg& msg, int userindex, int charindex, int y, char m, char d)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_CASHITEM_PURCHASEHISTORY_REQ
		<< userindex
		<< charindex
		<< y
		<< m 
		<< d;
}

void ConnCashItemPurchaselistReq(CNetMsg& msg, int userindex, int charindex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_CASHITEM_PURCHASELIST_REQ
		<< userindex
		<< charindex;
}
void ConnCashItemBringReq(CNetMsg& msg, bool bPresent, int userindex, int charindex, int count, int idx[], int ctid[])
{
	msg.Init(MSG_CONN_REQ);
	if( bPresent )
		msg << (unsigned char) MSG_CONN_CASHITEM_GIFT_RECV;
	else
		msg << (unsigned char) MSG_CONN_CASHITEM_BRING_REQ;

	msg	<< userindex
		<< charindex
		<< count;

	for(int i = 0; i < count; i++)
	{
		msg << idx[i]
			<< ctid[i];
	}

}

void ConnCashItemBalanceReq(CNetMsg& msg, int userindex, const char* idname)
{
	// userindex, idname
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_CASHITEM_BALANCE_REQ
		<< userindex
		<< idname;
}

void ConnCashItemPurchaseReqMsg(CNetMsg& msg, int userindex, const char* idname, int charindex, int cash, int count, int ctid[], int requestCash)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_CASHITEM_PURCHASE_REQ
		<< userindex
		<< idname
		<< charindex
		<< cash;

	int i;
#ifdef LIMIT_CATALOG
	// 물품 구매 요청이 오면
	// 한정 판매 물품인지 구분하여 한정 판매 물품이면 분류하여 뒤에 붙인다.
	CLCList<int> limitList(NULL);
	void* pos;
	CNetMsg ctlist;
	ctlist.Init();

	for(i = 0; i < count; i++)
	{
		pos = gserver.m_limitCT.FindData(ctid[i]);
		if( pos )
		{
			limitList.AddToTail(ctid[i]);
		}
		else
		{
			ctlist << ctid[i];
		}
	}
	int lcount = limitList.GetCount();
	count -= lcount;
	msg << count;
	msg << ctlist;

	msg << lcount;
	pos = limitList.GetHead();
	for(i = 0; i < lcount; i++)
	{
		msg << limitList.GetData(pos);
		pos = limitList.GetNext(pos);
	}
#else
	msg << count;
	for(i = 0; i < count; i++)
	{
		msg << ctid[i];
	}
#endif // LIMIT_CATALOG
#ifdef USE_TENTER_BILLING
	msg << requestCash;
#endif // USE_TENTER_BILLING
}

#ifdef CASH_ITEM_COUPON
void ConnCashItemCouponListReq( CNetMsg& msg, int userindex, const char* idname)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_CASHITEM_COUPON
		<< userindex
		<< idname;
}

void ConnCashItemPurchaseByCouponReqMsg(CNetMsg& msg, int userindex, const char* idname, int charindex, int couponID, int cash, int count, int ctid[] )
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_CASHITEM_PURCHASE_WITH_COUPON
		<< userindex
		<< idname
		<< charindex
		<< couponID
		<< cash;

	int i;
#ifdef LIMIT_CATALOG
	// 물품 구매 요청이 오면
	// 한정 판매 물품인지 구분하여 한정 판매 물품이면 분류하여 뒤에 붙인다.
	CLCList<int> limitList(NULL);
	void* pos;
	CNetMsg ctlist;
	ctlist.Init();

	for(i = 0; i < count; i++)
	{
		pos = gserver.m_limitCT.FindData(ctid[i]);
		if( pos )
		{
			limitList.AddToTail(ctid[i]);
		}
		else
		{
			ctlist << ctid[i];
		}
	}
	int lcount = limitList.GetCount();
	count -= lcount;
	msg << count;
	msg << ctlist;

	msg << lcount;
	pos = limitList.GetHead();
	for(i = 0; i < lcount; i++)
	{
		msg << limitList.GetData(pos);
		pos = limitList.GetNext(pos);
	}
#else
	msg << count;
	for(i = 0; i < count; i++)
	{
		msg << ctid[i];
	}
#endif // LIMIT_CATALOG
}

#endif //CASH_ITEM_COUPON

#endif // FEEITEM


void ExpSPMsg(CNetMsg& msg, LONGLONG exp, int sp)
{
	msg.Init(MSG_EXP_SP);

	msg << exp
		<< sp;
}

void QuestPCListMsg(CNetMsg& msg, CPC* pc)
{
	msg.Init(MSG_QUEST);

#ifdef QUEST_DATA_EXTEND
	int i;

	msg << (unsigned char) MSG_QUEST_PC_LIST
		<< pc->m_questList.GetCountRun();

	CQuest* pQuest;
	CQuest* pQuestNext = pc->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = pc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		msg << pQuest->GetQuestIndex();
		for (i = 0; i <	QUEST_MAX_CONDITION; i++)
			msg << pQuest->GetQuestValue(i);
	}
#else // QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_PC_LIST
		<< pc->m_questList.m_nCount;

	int i,j;

	for (i=0; i < QUEST_MAX_PC; i++)
	{
		if (pc->m_questList.m_list[i] && pc->m_questList.m_bQuest[i])
		{
			msg << pc->m_questList.m_list[i]->m_proto->m_index;

			for (j=0; j < QUEST_MAX_CONDITION; j++)
				msg << pc->m_questList.m_list[i]->m_currentData[j];
		}
	}
#endif // QUEST_DATA_EXTEND
}


void QuestCompleteListMsg(CNetMsg& msg, CPC* pc)
{
	msg.Init(MSG_QUEST);

#ifdef QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_DONE_LIST
		<< pc->m_questList.GetCountDone();

	CQuest* pQuest;
	CQuest* pQuestNext = pc->m_questList.GetNextQuest(NULL, QUEST_STATE_DONE);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = pc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_DONE);
		msg << pQuest->GetQuestIndex();
	}
#else // QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_DONE_LIST
		<< pc->m_questList.m_doneCnt;

	int j;

	if(pc->m_questList.m_doneCnt != 0)
	{
		for (j=0; j < QUEST_MAX_PC_COMPLETE; j++)
		{
			if (pc->m_questList.m_doneQuest[j] == -1)
			{
				break;
			}
			else
			{
				msg << pc->m_questList.m_doneQuest[j];
			}				
		}
	}
#endif // QUEST_DATA_EXTEND
}

void QuestAbandonListMsg(CNetMsg& msg, CPC* pc)
{
	msg.Init(MSG_QUEST);

#ifdef QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_ABANDON_LIST
		<< pc->m_questList.GetCountAbandon();

	CQuest* pQuest;
	CQuest* pQuestNext = pc->m_questList.GetNextQuest(NULL, QUEST_STATE_ABANDON);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = pc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_ABANDON);
		msg << pQuest->GetQuestIndex();
	}
#else // QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_ABANDON_LIST

		<< pc->m_questList.m_abandonCnt;
	
	if(pc->m_questList.m_abandonCnt != 0)
	{
		for (int j=0; j < QUEST_MAX_PC_COMPLETE; j++)
		{
			if (pc->m_questList.m_abandonQuest[j] == -1)
			{
				break;
			}
			else
			{
				msg << pc->m_questList.m_abandonQuest[j];
			}				
		}
	}
#endif // QUEST_DATA_EXTEND
}

void QuestNPCListMsg(CNetMsg& msg, int total, int* flag, int* index)
{
	msg.Init(MSG_QUEST);

	msg << (unsigned char) MSG_QUEST_NPC_LIST
		<< total;

	int i;
	for (i=0; i < total; i++)
	{
		msg << index[i]
			<< (char)flag[i];
	}
}

void QuestCreateListMsg(CNetMsg& msg, int count, int* index)
{
	msg.Init(MSG_QUEST);

	msg << (unsigned char) MSG_QUEST_CREATE_LIST
		<< count;

	int i;
	for (i=0; i < count; i++)
	{
		msg << index[i];
	}
}

void QuestStartMsg(CNetMsg& msg, CQuest* quest)
{
	msg.Init(MSG_QUEST);

#ifdef QUEST_DATA_EXTEND
	msg << (unsigned char)MSG_QUEST_START
		<< quest->GetQuestIndex();
#else // QUEST_DATA_EXTEND
	msg << (unsigned char)MSG_QUEST_START
		<< quest->m_proto->m_index;
#endif // QUEST_DATA_EXTEND
}

void QuestCompleteMsg(CNetMsg& msg, CQuest* quest)
{
	msg.Init(MSG_QUEST);

#ifdef QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_COMPLETE
		<< quest->GetQuestIndex();
#else // QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_COMPLETE
		<< quest->m_proto->m_index;
#endif // QUEST_DATA_EXTEND
}

void QuestUnCompleteMsg(CNetMsg& msg, CQuest* quest)
{
	msg.Init(MSG_QUEST);

#ifdef QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_UNCOMPLETE
		<< quest->GetQuestIndex();
#else // QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_UNCOMPLETE
		<< quest->m_proto->m_index;
#endif // QUEST_DATA_EXTEND
}

void QuestStatusMsg(CNetMsg& msg, CQuest* quest)
{
	msg.Init(MSG_QUEST);

#ifdef QUEST_DATA_EXTEND
	msg << (unsigned char)MSG_QUEST_STATUS
		<< quest->GetQuestIndex();

	int i;
	for (i=0; i < QUEST_MAX_CONDITION; i++)
		msg << quest->GetQuestValue(i);
#else // QUEST_DATA_EXTEND
	msg << (unsigned char)MSG_QUEST_STATUS
		<< quest->m_proto->m_index;

	int i;
	for (i=0; i < QUEST_MAX_CONDITION; i++)
		msg << quest->m_currentData[i];
#endif // QUEST_DATA_EXTEND
}

void QuestPrizeMsg(CNetMsg& msg, CQuest* quest)
{
	msg.Init(MSG_QUEST);

#ifdef QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_PRIZE
		<< quest->GetQuestIndex();
#else // QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_PRIZE
		<< quest->m_proto->m_index;
#endif // QUEST_DATA_EXTEND
}

void QuestGiveUpMsg(CNetMsg& msg, CQuest* quest)
{
	msg.Init(MSG_QUEST);

#ifdef QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_GIVEUP
		<< quest->GetQuestIndex();
#else // QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_GIVEUP
		<< quest->m_proto->m_index;
#endif // QUEST_DATA_EXTEND
}

void QuestGiveUpMsg(CNetMsg& msg, int questIndex)
{
	msg.Init(MSG_QUEST);

	msg << (unsigned char) MSG_QUEST_GIVEUP
		<< questIndex;
}

void QuestForceGiveUpMsg(CNetMsg& msg, CQuest* quest)
{
	msg.Init(MSG_QUEST);

#ifdef QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_FORCE_GIVEUP
		<< quest->GetQuestIndex();
#else // QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_FORCE_GIVEUP
		<< quest->m_proto->m_index;
#endif // QUEST_DATA_EXTEND
}

void QuestFailMsg(CNetMsg& msg, CQuest* quest)
{
	msg.Init(MSG_QUEST);

#ifdef QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_FAIL
		<< quest->GetQuestIndex();
#else // QUEST_DATA_EXTEND
	msg << (unsigned char) MSG_QUEST_FAIL
		<< quest->m_proto->m_index;
#endif // QUEST_DATA_EXTEND
}

void QuestErrorMsg(CNetMsg& msg, MSG_QUEST_ERROR_TYPE type)
{
	msg.Init(MSG_QUEST);

	msg << (unsigned char) MSG_QUEST_ERROR
		<< (unsigned char) type;
}

void QuestPD4ErrorMsg(CNetMsg& msg, MSG_QUEST_PD4_ERROR_TYPE type)
{
	msg.Init(MSG_QUEST);

	msg << (unsigned char) MSG_QUEST_PD4_ERROR
		<< (unsigned char) type;
}

void StatPointRemainMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_STATPOINT);
	msg << (unsigned char)MSG_STATPOINT_REMAIN
		<< ch->m_statpt_remain;
}

void StatPointUseMsg(CNetMsg& msg, CPC* ch, MSG_STATPOINT_USE_TYPE type, int value)
{
	msg.Init(MSG_STATPOINT);
	msg << (unsigned char)MSG_STATPOINT_USE
		<< (unsigned char)type
		<< value
		<< ch->m_statpt_remain;
}

void StatPointErrorMsg(CNetMsg& msg, MSG_STATPOINT_ERROR_TYPE errcode)
{
	msg.Init(MSG_STATPOINT);
	msg << (unsigned char)MSG_STATPOINT_ERROR
		<< (char)errcode;
}

void ActionMsg(CNetMsg& msg, CPC* ch, char type, char action)
{
	msg.Init(MSG_ACTION);
	msg << ch->m_index
		<< type
		<< action
		<< (char)ch->GetPlayerState();
}

void WarpStartMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_WARP);
	msg << (unsigned char)MSG_WARP_START
		<< ch->m_index;
}

void WarpCancelMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_WARP);
	msg << (unsigned char)MSG_WARP_CANCEL
		<< ch->m_index;
}

void WarpEndMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_WARP);
	msg << (unsigned char)MSG_WARP_END
		<< ch->m_index;
}

void WarpErrorMsg(CNetMsg& msg, MSG_WARP_ERROR_TYPE subtype, const char *name)
{
	msg.Init(MSG_WARP);
	msg << (unsigned char) MSG_WARP_ERROR
		<< (unsigned char) subtype
		<< name;
}

void WarpReqIngMsg(CNetMsg& msg, MSG_WARP_TYPE subtype)
{
	msg.Init(MSG_WARP);
	msg << (unsigned char) subtype;
}

void WarpReqMsg(CNetMsg& msg, CPC* ch, MSG_WARP_TYPE subtype)
{
	msg.Init(MSG_WARP);
	msg << (unsigned char) subtype
		<< ch->GetName();
}

void SSkillListMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_SSKILL);

	msg << (unsigned char)MSG_SSKILL_LIST
		<< (char)ch->m_sSkillList.m_nCount;

	CSSkillNode* node = ch->m_sSkillList.m_head;

	while (node)
	{
		msg << node->m_sskill->m_proto->m_index
			<< node->m_sskill->m_level;

		node = node->m_next;
	}
}

void SSkillLearnErrorMsg(CNetMsg& msg, MSG_SSKILL_LEARN_ERROR_TYPE errorcode)
{
	msg.Init(MSG_SSKILL);
	msg << (unsigned char)MSG_SSKILL_LEARN_ERROR
		<< (unsigned char)errorcode;
}

void SSkillLearnMsg(CNetMsg& msg, CSSkill* sskill)
{
	msg.Init(MSG_SSKILL);

	msg << (unsigned char)MSG_SSKILL_LEARN
		<< sskill->m_proto->m_index
		<< (char)((sskill->m_level == 1) ? 1 : 0)
		<< sskill->m_level;
}

void SSkillRemoveMsg(CNetMsg& msg, CSSkill* sskill)
{
	msg.Init(MSG_SSKILL);

	msg << (unsigned char)MSG_SSKILL_REMOVE
		<< sskill->m_proto->m_index;
}

void PKItemSealMsg(CNetMsg& msg, CItem* item)
{
	msg.Init(MSG_PK);
	msg << (unsigned char)MSG_PK_ITEMSEAL
		<< (char)item->tab()
		<< (char)item->row()
		<< (char)item->col()
		<< item->m_index
		<< item->m_flag;
}

void PKRecoverItemSealMsg(CNetMsg& msg, CItem* item)
{
	msg.Init(MSG_PK);
	msg << (unsigned char)MSG_PK_RECOVER_ITEMSEAL
		<< (char)item->tab()
		<< (char)item->row()
		<< (char)item->col()
		<< item->m_index
		<< item->m_flag;
}

void PKErrorMsg(CNetMsg& msg, char errcode)
{
	msg.Init(MSG_PK);
	msg << (unsigned char)MSG_PK_ERROR
		<< errcode;
}


void EventErrorMsg(CNetMsg& msg, MSG_EVENT_ERROR_TYPE errcode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_ERROR
		<< (unsigned char)errcode;
}

void EventPreopenGiftMsg(CNetMsg& msg, int itemdbindex)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_PREOPEN_GIFT
		<< itemdbindex;
}

void EventLattoMsg(CNetMsg& msg, MSG_EVENT_LATTO_TYPE type, int arg1, int arg2)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char) MSG_EVENT_LATTO
		<< (unsigned char) type;

	if ((unsigned char) type == (unsigned char) MSG_EVENT_LATTO_EXPSP_SUCCESS)
	{
		msg << arg1
			<< arg2;
	}

	if ((unsigned char) type == (unsigned char) MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP)
		msg << (char) arg1;
}

void PersonalShopErrorMsg(CNetMsg& msg, MSG_PERSONALSHOP_ERROR_TYPE errcode)
{
	msg.Init(MSG_PERSONALSHOP);
	msg << (unsigned char)MSG_PERSONALSHOP_ERROR
		<< (unsigned char)errcode;
}

void PersonalShopSellStartMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_PERSONALSHOP);
	msg << (unsigned char)MSG_PERSONALSHOP_SELL_START
		<< ch->m_index
		<< ch->m_personalshop->GetType()
		<< ch->m_personalshop->GetName();
}

void PersonalShopSellListMsg(CNetMsg& msg, CPC* ch)
{
	CPersonalShop* ps = ch->m_personalshop;
	CInventory& inven = ch->m_invenNormal;

	if (ps == NULL)
	{
		msg.Init();
		return ;
	}

	msg.Init(MSG_PERSONALSHOP);
	msg << (unsigned char)MSG_PERSONALSHOP_SELL_LIST
		<< ch->m_index
		<< ps->GetType();
	char normalcount = ps->GetNormalCount();
	msg << normalcount;
	if (normalcount)
	{
		int i;
		int pos = -1;
		for (i = 0; i < normalcount; i++)
		{
			pos = ps->GetNextNormalItem(pos);
			if (pos == -1)
			{
				msg.Init();
				return ;
			}
			int itemindex = ps->GetNormalItemIndex(pos);
			int r, c;
			if (!inven.FindItem(&r, &c, itemindex))
			{
				msg.Init();
				return ;
			}
			CItem* pitem = inven.GetItem(r, c);
			if (!pitem)
			{
				msg.Init();
				return ;
			}
			LONGLONG count = ps->GetNormalItemCount(pos);
			LONGLONG price = ps->GetNormalItemPrice(pos);
			msg << itemindex
				<< pitem->m_idNum
				<< pitem->m_plus
				<< pitem->m_flag;
#ifdef SET_ITEM
			if (pitem->IsOriginItem())
			{
				msg << (char)pitem->GetOriginOptionCnt();
				for (int j = 0; j < MAX_ORIGIN_OPTION; j++)
				{
					if (pitem->GetOriginIndex(j) > -1)
					{
						msg << (char)pitem->GetOriginIndex(j)
							<< (char)pitem->GetOriginLevel(j);
					}
				}
			}
			else
#endif // SET_ITEM
			{
				msg	<< (char)pitem->m_nOption;
				int j;
				for (j = 0; j < pitem->m_nOption; j++)
				{
					msg << (char)pitem->m_option[j].m_type
						<< (char)pitem->m_option[j].m_level;
				}
			}
			msg << count
				<< price
				<< pitem->m_used;
#ifdef COMPOSITE_TIME
			msg << pitem->m_used_2;
#endif	// COMPOSITE_TIME	
#ifdef SET_ITEM
			if (pitem->IsOriginItem())
			{
				msg << (char)pitem->GetBelongType()					// 귀속 설정
					<< (char)pitem->GetOriginSkillCnt();
				for (int j = MAX_ORIGIN_OPTION; j < MAX_ORIGIN; j++)
				{
					if (pitem->GetOriginIndex(j) > -1)
					{
						msg << pitem->GetOriginIndex(j)
							<< (char)pitem->GetOriginLevel(j);
					}
				}
			}
#endif // SET_ITEM
		}
	}

	if (ps->GetType() & PST_PACKAGE)
	{
		char itemcount = ps->GetPackageCount();
		msg << ps->GetPackagePrice()
			<< itemcount;
		int i;
		int pos = -1;
		for (i = 0; i < itemcount; i++)
		{
			pos = ps->GetNextPackageItem(pos);
			if (pos == -1)
			{
				msg.Init();
				return ;
			}
			int itemindex = ps->GetPackageItemIndex(pos);
			int r, c;
			if (!inven.FindItem(&r, &c, itemindex))
			{
				msg.Init();
				return ;
			}
			CItem* pitem = inven.GetItem(r, c);
			if (!pitem)
			{
				msg.Init();
				return ;
			}
			LONGLONG count = ps->GetPackageItemCount(pos);
			msg << itemindex
				<< pitem->m_idNum
				<< pitem->m_plus
				<< pitem->m_flag;
#ifdef SET_ITEM
			if (pitem->IsOriginItem())
			{
				msg << (char)pitem->GetOriginOptionCnt();
				for (int j = 0; j < MAX_ORIGIN_OPTION; j++)
				{
					if (pitem->GetOriginIndex(j) > -1)
					{
						msg << (char)pitem->GetOriginIndex(j)
							<< (char)pitem->GetOriginLevel(j);
					}
				}
			}
			else
#endif // SET_ITEM
			{
				msg << (char)pitem->m_nOption;
				int j;
				for (j = 0; j < pitem->m_nOption; j++)
				{
					msg << (char)pitem->m_option[j].m_type
						<< (char)pitem->m_option[j].m_level;
				}
			}
			msg << count
				<< pitem->m_used;
#ifdef COMPOSITE_TIME
			msg << pitem->m_used_2;
#endif	// COMPOSITE_TIME
#ifdef SET_ITEM
			if (pitem->IsOriginItem())
			{
				msg << (char)pitem->GetBelongType()
					<< (char)pitem->GetOriginSkillCnt();
				for (int j = MAX_ORIGIN_OPTION; j < MAX_ORIGIN; j++)
				{
					if (pitem->GetOriginIndex(j) > -1)
					{
						msg << pitem->GetOriginIndex(j)
							<< (char)pitem->GetOriginLevel(j);
					}
				}
			}
#endif // SET_ITEM
		}
	}
}

void PersonalShopChangeMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_PERSONALSHOP);
	msg << (unsigned char)MSG_PERSONALSHOP_CHANGE;
	msg << ch->m_index;
	if (ch->m_personalshop)
		msg << ch->m_personalshop->GetType();
	else
		msg << (char)PST_NOSHOP;
}

void PersonalShopBuyMsg(CNetMsg& msg, CNetMsg& buymsg, CPC* tch)
{
	msg.Init(buymsg);
	msg.MoveFirst();
	msg << (unsigned char)MSG_PERSONALSHOP_BUY;
	msg << tch->m_index;
}

void RightAttackMsg(CNetMsg& msg, CPC* target, MSG_RIGHT_ATTACK_TYPE type)
{
	msg.Init(MSG_RIGHT_ATTACK);

	msg << (unsigned char) type
		<< GET_CHAR_TYPE(target)
		<< target->m_index;
}

void StashSetNewPasswordRepMsg(CNetMsg& msg, char bsuccess)
{
	msg.Init(MSG_STASH);
	msg << (unsigned char) MSG_STASH_SETTING_NEWPASSWORD_REP
		<< bsuccess;
}
void StashDeletePasswordRepMsg(CNetMsg& msg, char bsuccess)
{
	msg.Init(MSG_STASH);
	msg << (unsigned char) MSG_STASH_DELETE_PASSWORD_REP;
	
	if(bsuccess == 0)
		msg << (char) 0;
	else if(bsuccess == 1)
		msg << (char) 1;
	else
		msg << (char) 2;
}

void StashIsSetPassword(CNetMsg& msg, bool bisset)
{
	msg.Init(MSG_STASH);
	msg << (unsigned char)MSG_STASH_ISSETPASSWORD;
	if (bisset)
		msg << (char)1;
	else
		msg << (char)0;
}

void StashErrorMsg(CNetMsg& msg, MSG_STASH_ERROR_TYPE err)
{
	msg.Init(MSG_STASH);
	msg << (unsigned char)MSG_STASH_ERROR
		<< (unsigned char)err;
}

void StashListRepMsg(CNetMsg& msg, const CItem* item, bool isstart, bool isend, bool isempty, int remain)
{
	char listflag = 0;
	if (isstart)
		listflag |= (1 << 0);
	if (isend)
		listflag |= (1 << 1);
	if (isempty)
		listflag |= (1 << 2);
	msg.Init(MSG_STASH);
	msg << (unsigned char)MSG_STASH_LIST_REP
		<< listflag;
	if (isempty)
		return ;
	else
	{
		msg << item->m_index
			<< item->m_idNum
			<< item->m_plus
			<< item->m_flag
			<< item->m_used;
#ifdef COMPOSITE_TIME		
		msg << item->m_used_2;
#endif	//COMPOSITE_TIME
#ifdef SET_ITEM
			if (item->IsOriginItem())
			{
				msg << (char)item->GetOriginOptionCnt();
				for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
				{
					if (item->GetOriginIndex(i) > -1)
					{
						msg << (char)item->GetOriginIndex(i)
							<< (char)item->GetOriginLevel(i);
					}
				}
			}
			else // IsOriginItem
#endif // SET_ITEM
			{
				msg << (char)item->m_nOption;
				int i;
				for (i = 0; i < item->m_nOption; i++)
				{
					msg << (char)item->m_option[i].m_type
						<< (char)item->m_option[i].m_level;
				}
			}
			msg << item->Count();
	}
	msg << remain;

#ifdef SET_ITEM
	if (item->IsOriginItem())
	{
		msg << (char)item->GetBelongType()
			<< (char)item->GetOriginSkillCnt();
		for (int i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
		{
			if (item->GetOriginIndex(i) > -1)
			{
				msg << item->GetOriginIndex(i)
					<< (char)item->GetOriginLevel(i);
			}
		}
	}
#endif
	
}

void StashKeepRepMsg(CNetMsg& msg, int keepcount, int* itemdbindex, LONGLONG* count)
{
	msg.Init(MSG_STASH);
	msg << (unsigned char)MSG_STASH_KEEP_REP
		<< keepcount;
	int i;
	for (i = 0; i < keepcount; i++)
	{
		msg << itemdbindex[i]
			<< count[i];
	}
}

void StashTakeRepMsg(CNetMsg& msg, int takecount, int* itemdbindex, LONGLONG* count)
{
	msg.Init(MSG_STASH);
	msg << (unsigned char)MSG_STASH_TAKE_REP
		<< takecount;
	int i;
	for (i = 0; i < takecount; i++)
	{
		msg << itemdbindex[i]
			<< count[i];
	}
}

void StashChangePasswordRepMsg(CNetMsg& msg, bool bisset)
{
	msg.Init(MSG_STASH);
	msg << (unsigned char)MSG_STASH_CHANGE_PASSWORD_REP;
	if (bisset)
		msg << (char)1;
	else
		msg << (char)0;
}

void StashSealMsg(CNetMsg& msg)
{
	msg.Init(MSG_STASH);
	msg << (unsigned char)MSG_STASH_SEAL;
}

void GuildErrorMsg(CNetMsg& msg, MSG_GUILD_ERROR_TYPE errcode)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_ERROR
		<< (char)errcode;
}

void GuildInfoMsg(CNetMsg& msg, CPC* pc)
{
	// TODO : GUILD : 트리 구성시 트리 정보 보내기
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_INFO;
	if (pc->m_guildInfo)
	{
		CGuild* guild = pc->m_guildInfo->guild();
		msg << guild->index()
			<< guild->name()
			<< guild->level()			
#if defined(GUILD_MARK_TABLE)
			<< pc->m_guildInfo->pos()
			<< pc->m_guildInfo->GetGuildMark();
#else
			<< pc->m_guildInfo->pos();

#endif // (GUILD_MARK_TABLE)


#ifdef NEW_GUILD
#ifdef NEW_UI
		msg << pc->m_guildInfo->GetPositionName();
#endif // NEW_UI

		if( guild->m_passiveSkillList.Find( 444 ) )
		{
			msg << guild->m_passiveSkillList.Find( 444 )->m_level;
		}
		else
		{
			msg << 0;
		}
#endif // NEW_GUILD
		
	}
	else
	{
		msg << (int)-1;
	}
}

void GuildListMsg(CNetMsg& msg, CPC* pc)
{
	if (pc->m_guildInfo == NULL)
	{
		msg.Init();
		return ;
	}

	// 길드 있을때
	CGuild* guild = pc->m_guildInfo->guild();

	int i;
	CGuildMember* member;

	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_MEMBERLIST;
	msg << guild->index()
		<< guild->membercount();
	for (i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		member = guild->member(i);
		if (member)
		{
			msg << member->charindex()
				<< member->GetName()
				<< member->pos()
				<< member->online();
		}
	}
}

void HelperShutdownMsg(CNetMsg& msg)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_SHUTDOWN;
}

void HelperRepWhisperNotFoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex, const char* sname)
{
	msg.Init(MSG_HELPER_REP);
	msg	<< seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_HELPER_WHISPER_NOTFOUND
		<< sindex << sname;
}

void HelperRepWhisperRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_HELPER_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_HELPER_WHISPER_REP
		<< sindex << sname
		<< rname
		<< chat;
}

void HelperGuildCreateReqMsg(CNetMsg& msg, CPC* ch, const char* name)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_CREATE_REQ
		<< ch->m_index
		<< ch->GetName()
		<< name;
}

void GuildOnlineMsg(CNetMsg& msg, CGuildMember* member)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_ONLINE
		<< member->guild()->index()
		<< member->charindex()
		<< member->GetName()
		<< member->online();
}

void HelperConnectMsg(CNetMsg& msg, int version, int server, int subno, int count, int* zones)
{
	msg.Init(MSG_HELPER_CONNECT);
	msg << version
		<< server
		<< subno
		<< count;
	int i;
	for (i = 0; i < count; i++)
		msg << zones[i];
}

void HelperGuildOnline(CNetMsg& msg, CGuildMember* member)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_ONLINE
		<< member->guild()->index()
		<< member->charindex()
		<< member->online();
}

#ifdef NEW_GUILD
void HelperGuildLoadReq(CNetMsg& msg, const char* idname, int charindex, int channel, int zoneindex )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_GUILD_LOAD_REQ
		<< idname
		<< charindex
		<< channel
		<< zoneindex;

}
#else

void HelperGuildLoadReq(CNetMsg& msg, const char* idname, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_LOAD_REQ
		<< idname
		<< charindex;
}
#endif // NEW_GUILD

void HelperGuildLevelUpReqMsg(CNetMsg& msg, int guildindex, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_LEVELUP_REQ
		<< guildindex
		<< charindex;
}

void GuildLevelInfoMsg(CNetMsg& msg, int guildindex, int guildlevel)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_LEVELINFO
		<< guildindex
		<< guildlevel;
}

void HelperGuildBreakUpReqMsg(CNetMsg& msg, CPC* boss)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_BREAKUP_REQ
		<< boss->m_index
		<< boss->m_guildInfo->guild()->index();
}

void GuildBreakUpNotifyMsg(CNetMsg& msg, int guildindex, const char* guildname)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_BREAKUP_NOTIFY
		<< guildindex
		<< guildname;
}

void GuildRegistCancelMsg(CNetMsg& msg, bool bTarget)
{
	char target = (bTarget) ? 1 : 0;
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_REGIST_CANCEL
		<< (char)target;
}

void HelperGuildMemberAddReqMsg(CNetMsg& msg, int guildindex, int targetindex, int requester, const char* name)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_MEMBER_ADD_REQ
		<< guildindex
		<< targetindex
		<< requester
		<< name;
}

void GuildMemberAddMsg(CNetMsg& msg, int guildindex, int charindex, const char* name)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_MEMBER_ADD
		<< guildindex
		<< charindex
		<< name;
}

void HelperGuildOutReqMsg(CNetMsg& msg, int guildindex, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_MEMBER_OUT_REQ
		<< guildindex
		<< charindex;
}

void GuildMemberOutMsg(CNetMsg& msg, int guildindex, int charindex, const char* name)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_MEMBER_OUT
		<< guildindex
		<< charindex
		<< name;
}

void HelperGuildKickReqMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_MEMBER_KICK_REQ
		<< guildindex
		<< bossindex
		<< charindex;
}

void GuildMemberKickMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex, const char* name)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_MEMBER_KICK
		<< guildindex
		<< bossindex
		<< charindex
		<< name;
}

void GuildInfoChangeMsg(CNetMsg& msg, int charindex, int guildindex, const char* name, MSG_GUILD_POSITION_TYPE pos)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_INFO_CHANGE
		<< charindex
		<< guildindex
		<< name
		<< pos;
}

void HelperGuildChangeBossReqMsg(CNetMsg& msg, int guildindex, int current, int change)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_CHANGE_BOSS_REQ
		<< guildindex
		<< current
		<< change;
}

void GuildChangeBossMsg(CNetMsg& msg, int guildindex, int current, int change)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_CHANGE_BOSS
		<< guildindex
		<< current
		<< change;
}

void HelperGuildAppointOfficerReqMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_APPOINT_OFFICER_REQ
		<< guildindex
		<< bossindex
		<< charindex;
}

void GuildAppointOfficerMsg(CNetMsg& msg, int guildindex, int charindex)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_APPOINT_OFFICER
		<< guildindex
		<< charindex;
}

void HelperGuildChat(CNetMsg& msg, int guildindex, int charindex, const char* charname, const char* chat)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_CHAT
		<< guildindex
		<< charindex
		<< charname
		<< chat;
}

void HelperGuildFireOfficerReqMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_FIRE_OFFICER_REQ
		<< guildindex
		<< bossindex
		<< charindex;
}

void HelperGuildBattleReqMsg(CNetMsg& msg, int guildindex1, int guildindex2, int prize, int zone, int time)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_GUILD_BATTLE_REQ
		<< guildindex1
		<< guildindex2
		<< prize
		<< zone
		<< time;
}

void HelperGuildBattleStopReqMsg(CNetMsg& msg, int guildindex1, int guildindex2)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_GUILD_BATTLE_STOP_REQ
		<< guildindex1
		<< guildindex2;
}

void HelperGuildBattlePeaceReqMsg(CNetMsg& msg, CGuild* g)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_GUILD_BATTLE_PEACE_REQ
		<< g->index();
}

void HelperGuildBattleKillReqMsg(CNetMsg& msg, int of_guildindex, int df_guildindex)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_GUILD_BATTLE_KILL_REQ
		<< of_guildindex
		<< df_guildindex;
}
//0503 kwon
void HelperEventMoonStoneUpdateReqMsg(CNetMsg& msg)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_EVENT_MOONSTONE_UPDATE_REQ;
}

void HelperEventMoonStoneJackPotReqMsg(CNetMsg& msg, int chaindex)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_EVENT_MOONSTONE_JACKPOT_REQ
		<< chaindex;
}

void GuildFireOfficerMsg(CNetMsg& msg, int guildindex, int charindex)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_FIRE_OFFICER
		<< guildindex
		<< charindex;
}

void GuildBattleReqReqMsg(CNetMsg& msg, int guildindex, const char* name, int prize, int time)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char) MSG_GUILD_BATTLE_REQ_REQ
		<< guildindex
		<< name
		<< prize
		<< time;
}

void GuildBattleReqRejectMsg(CNetMsg& msg, int reject_charindex)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char) MSG_GUILD_BATTLE_REQ_REJECT
		<< reject_charindex;
}

void GuildBattleReqAccpetMsg(CNetMsg& msg, int guildindex1, const char* name1, int guildindex2, const char* name2, int prize, int zone, int time)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char) MSG_GUILD_BATTLE_REQ_ACCEPT
		<< guildindex1
		<< name1
		<< guildindex2
		<< name2
		<< prize
		<< zone
		<< time / 10;
}

void GuildBattleStopReqMsg(CNetMsg& msg, int guildindex, const char* name)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char) MSG_GUILD_BATTLE_STOP_REQ
		<< guildindex
		<< name;
}

void GuildBattleStopRejectMsg(CNetMsg& msg, int reject_charindex)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char) MSG_GUILD_BATTLE_STOP_REJECT
		<< reject_charindex;
}

void GuildBattleEndMsg(CNetMsg& msg, int winner_guildindex, int guildindex1, const char* name1, int guildindex2, const char* name2, int prize)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char) MSG_GUILD_BATTLE_END
		<< winner_guildindex
		<< guildindex1
		<< name1
		<< guildindex2
		<< name2
		<< prize;
}

void GuildBattleStartMsg(CNetMsg& msg, int guildindex1, const char* name1, int guildindex2, const char* name2, int prize, int zone, int time)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char) MSG_GUILD_BATTLE_START
		<< guildindex1
		<< name1
		<< guildindex2
		<< name2
		<< prize
		<< zone
		<< time / 10;
}

void GuildBattleStatusMsg(CNetMsg& msg, int guildindex1, const char* name1, int killcount1, int guildindex2, const char* name2, int killcount2, int battletime, int battleZone)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char) MSG_GUILD_BATTLE_STATUS
		<< guildindex1
		<< name1
		<< killcount1
		<< guildindex2
		<< name2
		<< killcount2
		<< battletime / 10
		<< battleZone;
}

void GuildBattleErrMsg(CNetMsg& msg, MSG_GUILD_EROOR_BATTLE_TYPE type)
{
	msg.Init(MSG_GUILD);

	msg << (unsigned char) MSG_GUILD_ERROR
		<< (unsigned char) MSG_GUILD_ERROR_BATTLE
		<< (char) type;
}

void HelperCharDelMsg(CNetMsg& msg, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_CHAR_DEL
		<< charindex;
}

void ChangeErrMsg(CNetMsg& msg, MSG_CHANGE_ERR_TYPE type)
{
	msg.Init (MSG_CHANGE);

	msg << (unsigned char) MSG_CHANGE_ERR
		<< (unsigned char) type;
}

void ChangeStartMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init (MSG_CHANGE);

	msg << (unsigned char) MSG_CHANGE_START
		<< ch->m_index
		<< ch->m_changeIndex;
}

void ChangeStopMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init (MSG_CHANGE);

	msg <<(unsigned char) MSG_CHANGE_STOP
		<< ch->m_index;
}

void UIPlusEffectRep(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_UI);

	msg << (unsigned char) MSG_UI_PLUS_EFFECT_REP
		<< ch->m_plusEffect
		<< ch->m_index;
}

void TeachTeacherListMsg(CNetMsg& msg, CPC* ch, MSG_TEACH_TEACHER_LIST_TYPE type, CPC** list)
{
	msg.Init(MSG_TEACH);

	msg << (unsigned char) MSG_TEACH_TEACHER_LIST
		<< (unsigned char) type;

	if (type == MSG_TEACH_TEACHER_LIST_SHOW)
	{
		int i;
		for (i=0; i < 20; i++)
		{
			if (list[i])
			{
				msg << list[i]->m_index
					<< list[i]->GetName()
					<< list[i]->m_job;
#ifdef ENABLE_CHANGEJOB
				msg << list[i]->m_job2;
#endif
				//0627
				msg << list[i]->m_fame
					<< list[i]->m_cntTeachingStudent
					<< list[i]->m_cntCompleteStudent
					<< list[i]->m_cntFailStudent;
			}
			else
				msg << -1;
		}
	}
}

void HelperTeachMsg(CNetMsg& msg, int teachidx, char bTeacher)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TEACHER_REQ
		<< teachidx
		<< bTeacher;
}

void HelperTeacherGiveup(CNetMsg& msg, int teachidx, int studentidx)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TEACHER_GIVEUP_REQ
		<< teachidx
		<< studentidx;
}
void HelperTeachRegisterMsg(CNetMsg& msg, int teachidx, bool bteacher, int studentidx)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TEACHER_REGISTER_REQ
		<< teachidx
		<< (char) bteacher
		<< studentidx;
}

void HelperTeacherSuperstoneRecieve(CNetMsg& msg, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TEACHER_SUPERSTONERECIEVE
		<< charindex;
}

void HelperTeachTimeover(CNetMsg& msg, int flag, int teachidx, int studentidx)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TEACHER_TIMEOVER_REQ
		<< flag
		<< teachidx
		<< studentidx;
}

void HelperTeachLoadReq(CNetMsg& msg, const char* idname, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TEACHER_LOAD_REQ
		<< idname
		<< charindex;
}

void HelperTeachFameUpReqMsg(CNetMsg& msg, int teachidx, int studentidx, const char* studentname, int fame)
{
	msg.Init(MSG_HELPER_COMMAND);
	
	msg << MSG_HELPER_TEACHER_FAMEUP_REQ
		<< teachidx
		<< studentidx
		<< studentname
		<< fame;
}
#ifdef CASH_ITEM_GIFT
void HelperGiftCharReq(CNetMsg& msg, int sendUserIdx, int sendCharIdx, const char* recvCharName, const char* sendMsg, int count, int idx[], int ctid[] )
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_GIFT_RECVCHARNAME_REQ
		<< sendUserIdx
		<< sendCharIdx
		<< recvCharName
		<< sendMsg
		<< count;

	for(int i = 0; i < count; i++)
	{
		msg << idx[i]
			<< ctid[i];
	}
}
#endif

void HelperMargadumPvpRankReqMsg(CNetMsg& msg, int subno, int rank[], LONGLONG rankDamage[])
{
	int i = 0;
	msg.Init(MSG_HELPER_COMMAND);
	
	msg << MSG_HELPER_MARGADUM_PVP_RANK
		<< subno;

	int count = 0;
	for(i = 0; i < 5 && rank[i] != -1; i ++) count++;

	msg << count;

	for(i = 0; i < count; i ++)
	{
		msg << rank[i]
			<< rankDamage[i];
	}
}

void TeachTeacherReqReqMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_TEACH);

	msg << (unsigned char) MSG_TEACH_TEACHER_REQ
		<< (unsigned char) MSG_TEACH_TEACHER_REQ_REQ
		<< ch->m_index
		<< ch->GetName();
}

void TeachTeacherReqRejectMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_TEACH);

	msg << (unsigned char) MSG_TEACH_TEACHER_REQ
		<< (unsigned char) MSG_TEACH_TEACHER_REQ_REJECT
		<< ch->m_index
		<< ch->GetName();
}

void TeachTeacherReqAcceptMsg(CNetMsg& msg, CPC* teacher, CPC* student)
{
	msg.Init(MSG_TEACH);

	msg << (unsigned char) MSG_TEACH_TEACHER_REQ
		<< (unsigned char) MSG_TEACH_TEACHER_REQ_ACCEPT
		<< teacher->m_index
		<< teacher->GetName()
		<< student->m_index
		<< student->GetName();
}

void TeachEndMsg(CNetMsg& msg, int teacher_index, const char* teacher_name, int studetn_index, const char* student_name, MSG_TEACH_END_TYPE type)
{
	msg.Init(MSG_TEACH);

	msg << (unsigned char) MSG_TEACH_END
		<< teacher_index
		<< teacher_name
		<< studetn_index
		<< student_name
		<< (unsigned char) type;
#if defined( EVENT_TEACH ) || defined ( EVENT_CHILDERN_DAY )
	msg << (char) 1;
#else
	msg << (char) 0;
#endif
}

void TeachInfoMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_TEACH);

	msg << (unsigned char) MSG_TEACH_INFO
		<< (char)((ch->m_bTeacher) ? 1 : 0)
		<< ch->m_teachType;

	if (ch->m_teachType == MSG_TEACH_TEACHER_TYPE)
	{
		int i;
		for (i=0; i < TEACH_MAX_STUDENTS; i++)
		{
			if (ch->m_teachIdx[i] != -1)
			{
				msg << ch->m_teachIdx[i]
					<< ch->m_teachLevel[i]
					<< ch->m_teachName[i]
					<< ch->m_teachJob[i];
#ifdef ENABLE_CHANGEJOB
				msg << ch->m_teachJob2[i];
#endif
				//0627								
				char buff[50], buff2[50];			
				TransDateStr(ch->m_teachTime[i], buff);
				msg << buff;
				TransDateStr(ch->m_teachTime[i]+TEACH_LIMIT_SEC, buff2);
				msg << buff2;
			}
			else
				msg << -1;
		}

	}
	else if (ch->m_teachType == MSG_TEACH_STUDENT_TYPE)
	{
		if (ch->m_teachIdx[0] != -1)
		{
			msg << ch->m_teachIdx[0]
				<< ch->m_teachLevel[0]
				<< ch->m_teachName[0]
				<< ch->m_teachJob[0];
#ifdef ENABLE_CHANGEJOB
			msg << ch->m_teachJob2[0];
#endif
			//0627
			CPC* tpc = gserver.m_playerList.Find(ch->m_teachIdx[0]);
			if(tpc)
			{
				msg << tpc->m_fame
					<< tpc->m_cntTeachingStudent
					<< tpc->m_cntCompleteStudent
					<< tpc->m_cntFailStudent;

				// 자신이 학생이면 idx[1]을 선생의 명성수치로, idx[2]를 선생의 양성중이 초보로, time[1]을 완료인원, time[2]을 실패인원으로 쓴다.
				ch->m_teachIdx[1] = tpc->m_fame;
				ch->m_teachIdx[2] = tpc->m_cntTeachingStudent;
				ch->m_teachTime[1] = tpc->m_cntCompleteStudent;
				ch->m_teachTime[2] = tpc->m_cntFailStudent;
			}
			else //접속해 있지 않은 스승의 정보를 어찌 알리오..
			{
				// 자신이 학생이면 idx[1]을 선생의 명성수치로, idx[2]를 선생의 양성중이 초보로, time[1]을 완료인원, time[2]을 실패인원으로 쓴다.
				msg << ch->m_teachIdx[1]
					<< ch->m_teachIdx[2]
					<< ch->m_teachTime[1]
					<< ch->m_teachTime[2];
			}
		}
		else
			msg << -1;
	}

}

void TeachTeacherGiveUPMsg(CNetMsg& msg, CPC* teacher, CPC* student)
{
	msg.Init(MSG_TEACH);

	msg << (unsigned char) MSG_TEACH_TEACHER_REQ
		<< (unsigned char) MSG_TEACH_TEACHER_GIVEUP
		<< teacher->m_index
		<< teacher->GetName()
		<< student->m_index
		<< student->GetName();
}

void TeachStudentLevelUPMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_TEACH);

	msg << (unsigned char) MSG_TEACH_STUDENT_LEVELUP
		<< ch->m_index
		<< ch->m_level
		<< ch->GetName()
		<< ch->m_job
#ifdef ENABLE_CHANGEJOB
		<< ch->m_job2;
#endif
}


void TeachErrMsg(CNetMsg& msg, MSG_TEACH_ERR_TYPE type)
{
	msg.Init(MSG_TEACH);

	msg << (unsigned char) MSG_TEACH_ERR
		<< (unsigned char) type;
}

#ifdef ENABLE_CHANGEJOB
void ChangeJobErrorMsg(CNetMsg& msg, MSG_CHANGEJOB_ERROR_TYPE type)
{
	msg.Init(MSG_CHANGEJOB);
	msg << (unsigned char)MSG_CHANGEJOB_ERROR
		<< (char)type;
}

void ChangeJobRepMsg(CNetMsg& msg, int nIndex, char job1, char job2)
{
	msg.Init(MSG_CHANGEJOB);
	msg << (unsigned char)MSG_CHANGEJOB_REP
		<< nIndex
		<< job1
		<< job2;
}

void ChangeJobResetRepMsg(CNetMsg& msg, CPC* pc)
{
	msg.Init(MSG_CHANGEJOB);
	msg << (unsigned char)MSG_CHANGEJOB_RESET_REP
		<< pc->m_index
		<< pc->m_job;
}

void ItemChangeWeaponRepMsg(CNetMsg& msg, int olddbindex, int newdbindex)
{
	msg.Init(MSG_ITEM);
	msg << (unsigned char)MSG_ITEM_CHANGEWEAPON_REP
		<< olddbindex
		<< newdbindex;
}
#endif

void ItemProcessNPCMsg(CNetMsg& msg, int itemdbindex, int count, MSG_ITEM_PROCESS_NPC_ERROR_TYPE errcode)
{
	msg.Init(MSG_ITEM);
	msg << (unsigned char)MSG_ITEM_PROCESS_NPC
		<< itemdbindex
		<< count
		<< errcode;
}

void ItemAddBoosterMsg(CNetMsg& msg, MSG_ITEM_ADD_BOOSTER_ERROR_TYPE errcode)
{
	msg.Init(MSG_ITEM);
	msg << (unsigned char)MSG_ITEM_ADD_BOOSTER
		<< errcode;
}

#ifdef ENABLE_WAR_CASTLE
void ItemMixWarItemMsg(CNetMsg& msg, MSG_ITEM_MIX_WARITEM_ERROR_TYPE errcode)
{
	msg.Init(MSG_ITEM);
	msg << (unsigned char)MSG_ITEM_MIX_WARITEM
		<< errcode;
}
#endif

#ifdef LEVELDOWN_SCROLL
void ItemLevelDownErrMsg(CNetMsg& msg, MSG_ITEM_LEVELDOWN_ERROR_TYPE errcode)
{
	msg.Init(MSG_ITEM);
	msg << (unsigned char)MSG_ITEM_LEVELDOWN
		<< errcode;
}
#endif	// LEVELDOWN_SCROLL

void CashItemMoonstoneStartRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_MOONSTONE_ERROR_TYPE errorcode, char grade)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASHITEM
		<< (unsigned char) MSG_EX_CASHITEM_MOONSTONE_START
		<< (unsigned char) errorcode
		<< grade;
}

void CashItemGiftRecvNoticeRepMsg(CNetMsg& msg, char bGiftExist)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASHITEM
		<< (unsigned char) MSG_EX_CASHITEM_GIFT_NOTICE
		<< bGiftExist;
}

void CashItemGiftSendRepMsg(CNetMsg& msg, MSG_EX_CASHITEM_ERROR_TYPE errCode)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASHITEM
		 << (unsigned char) MSG_EX_CASHITEM_GIFT_REP
		 << (unsigned char) errCode;
}

void CashItemGiftHistoryRepMsg(CNetMsg& msg, bool bSend, MSG_EX_CASHITEM_ERROR_TYPE errCode )
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASHITEM;
		
	if( bSend )
		msg << (unsigned char) MSG_EX_CASHITEM_GIFT_SENDHISTORY_REP;
	else
		msg << (unsigned char) MSG_EX_CASHITEM_GIFT_RECVHISTORY_REP;

	msg << (unsigned char) errCode;
}

void CashItemGiftRecvListRepMsg(CNetMsg& msg, unsigned char listflag, CNetMsg recvMsg)
{
	int idx, ctid, count, i;
	long sendDate;
	CLCString strMsg(MAX_GIFT_MESSAGE_LENGTH + 1);
	CLCString sendName( MAX_CHAR_NAME_LENGTH + 1);

	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASHITEM
		<< (unsigned char) MSG_EX_CASHITEM_GIFT_RECVLIST_REP
		<< listflag;

	if( listflag & ( 1 << 2 ) )
	{
		return;
	}
	else
	{
		recvMsg >> count;
		msg << count;

		for(i = 0; i < count; i++)
		{
			recvMsg >> idx
				>> ctid
				>> sendDate
				>> sendName
				>> strMsg;

			msg << idx
				 << ctid
				<< sendDate
				<< sendName
				<< strMsg;
		}

	}
}


void EventLetterMsg(CNetMsg& msg, MSG_EVENT_LETTER_TYPE type)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_LETTER
		<< (unsigned char)type;
}

void EventChangeWeaponMsg(CNetMsg& msg, int olddbindex, int newdbindex)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_CHANGEWEAPON
		<< olddbindex
		<< newdbindex;
}

void WarpTeleportMsg(CNetMsg& msg, int pos, CPC* ch)
{
	msg.Init(MSG_WARP);
	msg << (unsigned char)MSG_WARP_TELEPORT
		<< pos
		<< GET_X(ch)
		<< GET_Z(ch)
		<< GET_H(ch)
		<< GET_R(ch)
		<< GET_YLAYER(ch);
}

//0502 kwon
void EventMoonStoneMsg(CNetMsg& msg, MSG_EVENT_MOONSTONE_TYPE type, MSG_EVENT_MOONSTONE_CHANGE_TYPE subtype, int arg1, int arg2, int MoonStoneNas)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char) MSG_EVENT_MOONSTONE
		<< (unsigned char) type
		<< (unsigned char) subtype;

	if ((unsigned char) type == (unsigned char) MSG_EVENT_MOONSTONE_TRY_REP)
	{		
		if((unsigned char) subtype == (unsigned char) MSG_CHANGE_MOONSTONE_RULLET)
		{
			msg << arg1 //숫자
				<< arg2	//갯수	
				<< MoonStoneNas;
		}
	}		
	else if ((unsigned char) type == (unsigned char) MSG_EVENT_MOONSTONE_START_REP)
	{		
		if((unsigned char) subtype == (unsigned char) MSG_CHANGE_MOONSTONE_ACC_NAS)
		{
			msg << arg1;
		}
	}
}

void MsgrEventReqMsg(CNetMsg& msg, MSG_MSGR_TYPE msgType, int seq, int server, int subno, int zone, int charindex, int cmd, int drop)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) msgType
		<< charindex
		<< cmd
		<< drop;
}

void MsgrEventRepMsg(CNetMsg& msg, MSG_MSGR_TYPE msgType, int seq, int server, int subno, int zone, int charindex, int cmd, int drop)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) msgType
		<< charindex
		<< gserver.m_serverno
		<< gserver.m_subno
		<< cmd
		<< drop;
}


void MsgrEventReqMsg(CNetMsg& msg, int server, int subno, int zone, int thisServer, int thisSubno, int charIndex, const char* eventcmd, int flag)
{
	msg.Init(MSG_MSGR_REQ);
	msg << (int)-1
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_EVENT_REQ
		<< thisServer
		<< thisSubno
		<< charIndex
		<< eventcmd
		<< flag;
}

#ifdef MESSENGER_NEW
void MsgrMessengerChatMsg( CNetMsg& msg, MSG_MSGR_TYPE subtype, 
						  int makeCharIndex, int chatIndex, int chatColor, int charIndex, CLCString chat )
{
	msg.Init(MSG_MSGR_REQ);

	msg  << -1
		 << gserver.m_serverno
		 << -1
		 << -1
		 << (unsigned char) subtype
		 << makeCharIndex
		 << chatIndex
		 << chatColor
		 << charIndex
		 << chat;
}

void MsgrMessengerChatMsg( CNetMsg& msg, MSG_MSGR_TYPE subtype, 
						  int makeCharIndex, int chatIndex, int chatColor, const char* charName, CLCString chat )
{
	msg.Init(MSG_MSGR_REQ);

	msg  << -1
		 << gserver.m_serverno
		 << -1
		 << -1
		 << (unsigned char) subtype
		 << makeCharIndex
		 << chatIndex
		 << chatColor
		 << charName
		 << chat;
}
#endif

#ifdef ENABLE_WAR_CASTLE

void WarpPromptMsg(CNetMsg& msg, int zoneindex, int pos)
{
	msg.Init(MSG_WARP);
	msg << (unsigned char)MSG_WARP_PROMPT
		<< zoneindex
		<< pos;
}

void MsgrChatLordMsg(CNetMsg& msg, int index, const char* name, const char* chatmsg)
{
	msg.Init(MSG_MSGR_REQ);
	msg	<< -1
		<< gserver.m_serverno
		<< gserver.m_subno
		<< -1
		<< (unsigned char)MSG_MSGR_CHAT_LORD
		<< index
		<< name
		<< chatmsg;
}


void GuildWarGetTimeMsg(CNetMsg& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_GET_TIME
		<< errcode;

	if (castle)
	{
		struct tm tmNext;
		castle->GetNextWarTime(&tmNext, true);

		msg << castle->GetOwnerGuildIndex()
			<< castle->GetOwnerGuildName()
			<< (char)tmNext.tm_mon
			<< (char)tmNext.tm_mday
			<< (char)tmNext.tm_hour
			<< (char)tmNext.tm_min;
	}
	else
	{
		msg << 0
			<< ""
			<< 0
			<< 0
			<< 0
			<< 0;
	}
}

void HelperWarNoticeTimeMsg(CNetMsg& msg, int zoneindex, char month, char day, char hour, char min)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< -1
		<< -1
		<< (unsigned char)MSG_HELPER_WAR_NOTICE_TIME
		<< zoneindex
		<< month
		<< day
		<< hour
		<< min;
}

void GuildWarNoticeTimeMsg(CNetMsg& msg, int zoneindex, char month, char day, char hour, char min)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_NOTICE_TIME
		<< zoneindex
		<< month
		<< day
		<< hour
		<< min;
}

void GuildWarNoticeTimeRemainMsg(CNetMsg& msg, int zoneindex, char min)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_NOTICE_TIME_REMAIN
		<< zoneindex
		<< min;
}

void HelperWarNoticeRemainMsg(CNetMsg& msg, int zoneindex, char remain)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< -1
		<< -1
		<< (unsigned char)MSG_HELPER_WAR_NOTICE_TIME_REMAIN
		<< zoneindex
		<< remain;
}

void HelperWarNoticeStartMsg(CNetMsg& msg, int zoneindex, int remainSec)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< -1
		<< -1
		<< (unsigned char)MSG_HELPER_WAR_NOTICE_START
		<< zoneindex
		<< remainSec;
}

void GuildWarNoticeStartMsg(CNetMsg& msg, int zoneindex, int remainSec)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_START
		<< zoneindex
		<< remainSec;
}

void GuildWarJoinAttackGuildMsg(CNetMsg& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_JOIN_ATTACK_GUILD
		<< errcode;

	if (castle)
	{
		struct tm tmNext;
		castle->GetNextWarTime(&tmNext, true);

		msg << (char)tmNext.tm_mon
			<< (char)tmNext.tm_mday
			<< (char)tmNext.tm_hour
			<< (char)tmNext.tm_min;
	}
	else
	{
		msg << 0
			<< 0
			<< 0
			<< 0;
	}
}

void GuildWarJoinDefenseGuildMsg(CNetMsg& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_JOIN_DEFENSE_GUILD
		<< errcode;

	if (castle)
	{
		struct tm tmNext;
		castle->GetNextWarTime(&tmNext, true);

		msg << (char)tmNext.tm_mon
			<< (char)tmNext.tm_mday
			<< (char)tmNext.tm_hour
			<< (char)tmNext.tm_min;
	}
	else
	{
		msg << 0
			<< 0
			<< 0
			<< 0;
	}
}

void GuildWarJoinAttackCharMsg(CNetMsg& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_JOIN_ATTACK_CHAR
		<< errcode;

	if (castle)
	{
		struct tm tmNext;
		castle->GetNextWarTime(&tmNext, true);

		msg << (char)tmNext.tm_mon
			<< (char)tmNext.tm_mday
			<< (char)tmNext.tm_hour
			<< (char)tmNext.tm_min;
	}
	else
	{
		msg << 0
			<< 0
			<< 0
			<< 0;
	}
}

void GuildWarJoinDefenseCharMsg(CNetMsg& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_JOIN_DEFENSE_CHAR
		<< errcode;

	if (castle)
	{
		struct tm tmNext;
		castle->GetNextWarTime(&tmNext, true);

		msg << (char)tmNext.tm_mon
			<< (char)tmNext.tm_mday
			<< (char)tmNext.tm_hour
			<< (char)tmNext.tm_min;
	}
	else
	{
		msg << 0
			<< 0
			<< 0
			<< 0;
	}
}

void HelperWarJoinAttackGuildMsg(CNetMsg& msg, int zoneindex, int guildindex)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< -1
		<< -1
		<< (unsigned char)MSG_HELPER_WAR_JOIN_ATTACK_GUILD
		<< zoneindex
		<< guildindex;
}

void HelperWarJoinDefenseGuildMsg(CNetMsg& msg, int zoneindex, int guildindex)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< -1
		<< -1
		<< (unsigned char)MSG_HELPER_WAR_JOIN_DEFENSE_GUILD
		<< zoneindex
		<< guildindex;
}

void HelperWarNoticeStartAttackCastleMsg(CNetMsg& msg, int zoneindex, int remainSec, CGuild* g1, CGuild* g2, CGuild* g3)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< -1
		<< -1
		<< (unsigned char)MSG_HELPER_WAR_NOTICE_START_ATTACK_CASTLE
		<< zoneindex
		<< remainSec;

	if (g1)		msg << g1->index() << g1->name();
	else		msg << 0 << "";
	if (g2)		msg << g2->index() << g2->name();
	else		msg << 0 << "";
	if (g3)		msg << g3->index() << g3->name();
	else		msg << 0 << "";
}

void GuildWarPointMsg(CNetMsg& msg, CPC* pc, CWarCastle* castle)
{
	int remainsec = castle->GetRemainWarTime();
	int guildindex[3] = {-1, -1, -1};
	char guildname[3][MAX_GUILD_NAME_LENGTH + 1] = {"", "", ""};
	int guildpoint[3] = {0, 0, 0};
	castle->GetTop3AttackGuild(guildindex, guildname, guildpoint);
	int myguildpoint = -1;
	int defensepoint = -1;
	if (pc->m_guildInfo && pc->m_guildInfo->guild() && castle->GetState() == WCSF_WAR_FIELD)
	{
		myguildpoint = castle->GetGuildPoint(pc->m_guildInfo->guild()->index());
		defensepoint = castle->GetGuildPoint(castle->GetOwnerGuildIndex());
	}
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_POINT
		<< remainsec;
	int i;
	for (i = 0; i < 3; i++)
	{
		msg << guildindex[i]
			<< guildname[i]
			<< guildpoint[i];
	}
	msg << myguildpoint
		<< defensepoint;
}

void GuildWarCastleStateMsg(CNetMsg& msg, int zoneindex, CPC* pc, CWarCastle* castle)
{
	int state = castle->GetState();
	int remainsec = castle->GetRemainWarTime();

	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_CASTLE_STATE
		<< zoneindex
		<< state
		<< castle->GetGateState()
		<< remainsec;

#ifdef DRATAN_CASTLE
	if (zoneindex != ZONE_DRATAN)
#endif // DRATAN_CASTLE
	{
		int guildindex[3] = {-1, -1, -1};
		char guildname[3][MAX_GUILD_NAME_LENGTH + 1] = {"", "", ""};
		int guildpoint[3] = {0, 0, 0};

		castle->GetTop3AttackGuild(guildindex, guildname, guildpoint);


		int myguildpoint = -1;
		int defensepoint = -1;
		if (pc->m_guildInfo && pc->m_guildInfo->guild() && castle->GetState() == WCSF_WAR_FIELD)
		{
			myguildpoint = castle->GetGuildPoint(pc->m_guildInfo->guild()->index());
			defensepoint = castle->GetGuildPoint(castle->GetOwnerGuildIndex());
		}

		int i;
		for (i = 0; i < 3; i++)
		{
			msg << guildindex[i]
				<< guildname[i]
				<< guildpoint[i];
		}
		msg << myguildpoint
			<< defensepoint;
	}
}

void HelperWarNoticeRemainFieldTimeMsg(CNetMsg& msg, int zoneindex, int remainSec)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< -1
		<< -1
		<< (unsigned char)MSG_HELPER_WAR_NOTICE_REMAIN_FIELD_TIME
		<< zoneindex
		<< remainSec;
}

void GuildWarNoticeStartCastleMsg(CNetMsg& msg, int zoneindex, int remainSec, int guildindex1, const char* guildname1, int guildindex2, const char* guildname2, int guildindex3, const char* guildname3)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_NOTICE_START_CASTLE
		<< zoneindex
		<< remainSec
		<< guildindex1
		<< guildname1
		<< guildindex2
		<< guildname2
		<< guildindex3
		<< guildname3;
}

void GuildWarNoticeRemainFieldTimeMsg(CNetMsg& msg, int zoneindex, int remainSec)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_NOTICE_REMAIN_FIELD_TIME
		<< zoneindex
		<< remainSec;
}

void GuildWarGateStateMsg(CNetMsg& msg, int oldstate, int newstate)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_GATE_STATE
		<< oldstate
		<< newstate;
}

void HelperWarNoticeEndMsg(CNetMsg& msg, int zoneindex, char bWinDefense, int ownerindex, const char* ownername, int charindex, const char* charname, int nextMonth, int nextDay, int nextHour, int wDay)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< -1
		<< -1
		<< (unsigned char)MSG_HELPER_WAR_NOTICE_END
		<< zoneindex;
	if (bWinDefense)
		msg << (char)1;
	else
		msg << (char)0;
	msg << ownerindex
		<< ownername
		<< charindex
		<< charname
		<< nextMonth
		<< nextDay
		<< nextHour
		<< wDay;
}

void GuildWarEndMsg(CNetMsg& msg, int zoneindex, char bWinDefense, int ownerindex, const char* ownername, int charindex, const char* charname, int nextMonth, int nextDay, int nextHour, int wDay)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_END
		<< zoneindex
		<< bWinDefense
		<< ownerindex
		<< ownername
		<< charindex
		<< charname
		<< nextMonth
		<< nextDay
		<< nextHour
		<< wDay;
}

void GuildWarSetTimeRepMsg(CNetMsg& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, char month, char day, char hour, char min)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_SET_TIME_REP
		<< errcode
		<< month
		<< day
		<< hour
		<< min;
}

void GuildWarInvalidCommandMsg(CNetMsg& msg, MSG_GUILD_TYPE reqtype)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_WAR_INVALID_COMMAND
		<< (unsigned char)reqtype;
}

void GuildStashHistoryRepMsg(CNetMsg& msg, MSG_GUILD_STASH_ERROR_TYPE errcode, int month[7], int day[7], LONGLONG money[7])
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_STASH_HISTORY_REP
		<< errcode;
	int i;
	for (i = 0; i < 7; i++)
	{
		msg << month[i]
			<< day[i]
			<< money[i];
	}
}

void GuildStashViewRepMsg(CNetMsg& msg, MSG_GUILD_STASH_ERROR_TYPE errcode, LONGLONG money)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_STASH_VIEW_REP
		<< errcode
		<< money;
}

void GuildStashTakeRepMsg(CNetMsg& msg, MSG_GUILD_STASH_ERROR_TYPE errcode)
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_GUILD_STASH_TAKE_REP
		<< errcode;
}

void HelperGuildStashHistoryReqMsg(CNetMsg& msg, int guildindex, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_STASH_HISTORY_REQ
		<< guildindex
		<< charindex;
}

void HelperGuildStashViewReqMsg(CNetMsg& msg, int guildindex, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_STASH_VIEW_REQ
		<< guildindex
		<< charindex;
}

void HelperGuildStashTakeReqMsg(CNetMsg& msg, int guildindex, int charindex, LONGLONG money)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_STASH_TAKE_REQ
		<< guildindex
		<< charindex
		<< money;
}

void HelperGuildStashRollbackMsg(CNetMsg& msg, int guildindex, LONGLONG money)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_STASH_ROLLBACK
		<< guildindex
		<< money;
}

void HelperGuildStashSaveTaxReqMsg(CNetMsg& msg, int guildIndex, int zoneindex, LONGLONG taxItem, LONGLONG taxProduct)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_STASH_SAVE_TAX_REQ
		<< guildIndex
		<< zoneindex
		<< taxItem
		<< taxProduct;
}

#endif	// ENABLE_WAR_CASTLE

#ifdef EVENT_TREASUREBOX_RED
void EventTreasureBoxRedMsg(CNetMsg& msg, MSG_EVENT_TREASUREBOX_TYPE type, int arg)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char) MSG_EVENT_TREASUREBOX_RED
		<< (unsigned char) type
		<< arg;
}
#endif // EVENT_TREASUREBOX_RED

void EventTreasureBoxMsg(CNetMsg& msg, MSG_EVENT_TREASUREBOX_TYPE type, int arg)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char) MSG_EVENT_TREASUREBOX
		<< (unsigned char) type
		<< arg;

	if(type ==MSG_EVENT_TREASUREBOX_TRY_REP)
	{
		if(arg == (int)MSG_EVENT_TREASUREBOX_10LEVEL)
		{
			msg << (unsigned char) MSG_EVENT_CHANGE_13LEVEL_WEAPON
				<< (int) 1
				<< (unsigned char) MSG_EVENT_CHANGE_NAS
				<< (int) 30000
				<< (unsigned char) MSG_EVENT_CHANGE_CANDY
				<< (int) 5
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
				<< (unsigned char) MSG_EVENT_CHANGE_HP_POTION
				<< (int) 1;
#else
			;
#endif  // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM

		}
		else if(arg == (int)MSG_EVENT_TREASUREBOX_14LEVEL)
		{
			msg << (unsigned char) MSG_EVENT_CHANGE_ATTACK_POTION
				<< (int) 10
				<< (unsigned char) MSG_EVENT_CHANGE_NAS
				<< (int) 50000
				<< (unsigned char) MSG_EVENT_CHANGE_CANDY
				<< (int) 10
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
				<< (unsigned char) MSG_EVENT_CHANGE_MP_POTION
				<< (int) 1;
#else
			;
#endif  // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
		}
		else if(arg == (int)MSG_EVENT_TREASUREBOX_18LEVEL)
		{
			msg << (unsigned char) MSG_EVENT_CHANGE_21LEVEL_WEAPON
				<< (int) 1
				<< (unsigned char) MSG_EVENT_CHANGE_NAS
				<< (int) 80000
				<< (unsigned char) MSG_EVENT_CHANGE_CANDY
				<< (int) 15
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
				<< (unsigned char) MSG_EVENT_CHANGE_EX_MEMORY
				<< (int) 1;
#else
			;
#endif  // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
		}
		else if(arg == (int)MSG_EVENT_TREASUREBOX_22LEVEL)
		{
			msg << (unsigned char) MSG_EVENT_CHANGE_DEFENSE_POTION
				<< (int) 15
				<< (unsigned char) MSG_EVENT_CHANGE_NAS
				<< (int) 100000
				<< (unsigned char) MSG_EVENT_CHANGE_CANDY
				<< (int) 20
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
				<< (unsigned char) MSG_EVENT_CHANGE_HPMP_POTION
				<< (int) 1;
#else
			;
#endif  // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
		}
		else if(arg == (int)MSG_EVENT_TREASUREBOX_26LEVEL)
		{
			msg << (unsigned char) MSG_EVENT_CHANGE_29LEVEL_WEAPON
				<< (int) 1
				<< (unsigned char) MSG_EVENT_CHANGE_NAS
				<< (int) 150000
				<< (unsigned char) MSG_EVENT_CHANGE_CANDY
				<< (int) 30
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
				<< (unsigned char) MSG_EVEVNT_CHAGE_EX_STAT
				<< (int) 1;
#else
			;
#endif  // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
		}
		else if(arg == (int)MSG_EVENT_TREASUREBOX_30LEVEL)
		{
#ifdef LC_TLD
			msg << (unsigned char) MSG_EVENT_CHANGE_BLUE_HORSE
				<< (int) 1
#else
			msg << (unsigned char) MSG_EVENT_CHANGE_ATTACK_POTION
				<< (int) 20
#endif // LC_TLD
				<< (unsigned char) MSG_EVENT_CHANGE_NAS
				<< (int) 200000
#ifdef LC_TLD
				<< (unsigned char) MSG_EVENT_CHANGE_PINK_DRAGON
				<< (int) 1
#else
				<< (unsigned char) MSG_EVENT_CHANGE_CANDY
				<< (int) 40
#endif // LC_TLD
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
				<< (unsigned char) MSG_EVENT_CHANGE_HPMP_POTION
				<< (int) 1;
#else
			;
#endif  // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
		}
		
	}
}

//0627
void EventSuperStoneMsg(CNetMsg& msg, MSG_EVENT_SUPERSTONE_TYPE type)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char) MSG_EVENT_SUPERSTONE
		<< (unsigned char) type;
}

void SysEnableSuperStoneMsg(CNetMsg& msg, int fame)
{
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_CAN_RECEIVE_SUPERSTONE
		<< fame;
}

void NoviceNotifyMsg(CNetMsg& msg, const char* name)
{
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_APPEAR_NOVICE
		<< name;
}

//0704
void NoticeInfoMsg(CNetMsg& msg, int notice)
{
 
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_NOTICE
		<< notice;
}

#ifdef ENABLE_PET
void HelperPetCreateReqMsg(CNetMsg& msg, int owner, char typeGrade)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PET_CREATE_REQ
		<< owner
		<< typeGrade;
}

// TODO : DELETE
void HelperPetDeleteReqMsg(CNetMsg& msg, int index, int owner)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PET_DELETE_REQ
		<< index
		<< owner;
}

void ExPetStatusMsg(CNetMsg& msg, CPet* pet)
{
	int nRemainGetRebirth = pet->GetRemainRebirthTime();
	float x = 0.0f;
	float z = 0.0f;
	float h = 0.0f;
	float r = 0.0f;
	char y = 0;
	unsigned char mapAttr = MAPATT_BLOCK;

	if (IS_IN_CELL(pet))
	{
		x = GET_X(pet);
		z = GET_Z(pet);
		h = GET_H(pet);
		r = GET_R(pet);
		y = GET_YLAYER(pet);
		mapAttr = pet->GetMapAttr();
	}

	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PET_STATUS
		<< pet->m_index
		<< pet->GetPetTypeGrade()
		<< pet->m_level
		<< pet->m_exp
		<< pet->GetNeedExp()
		<< pet->m_hp
		<< pet->m_maxHP
		<< pet->GetAbilityPoint()
		<< pet->GetHungryPoint()
		<< (int)PET_MAX_HUNGRY
		<< pet->GetSympathyPoint()
		<< (int)PET_MAX_SYMPATHY
		<< x
		<< z
		<< h
		<< r
		<< y
		<< mapAttr
		<< nRemainGetRebirth;
#ifdef PET_NAME_CHANGE
	msg << pet->m_name;
#endif // PET_NAME_CHAGNE
}

void ExPetMountMsg(CNetMsg& msg, int charindex, char pettype)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PET_MOUNT
		<< charindex
		<< pettype;
}

void ExPetLearnMsg(CNetMsg& msg, int skillindex, char level, MSG_EX_PET_LEARN_ERROR_TYPE errcode)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PET_LEARN
		<< skillindex
		<< level
		<< errcode;
}

void ExPetSkillListMsg(CNetMsg& msg, CPet* pet)
{
	if (pet == NULL)
	{
		msg.Init();
		return ;
	}
	CSkillList& list = *(pet->GetSkillList());

	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PET_SKILLLIST
		<< pet->m_index
		<< list.count();

	void* p = list.GetHeadPosition();
	while (p)
	{
		CSkill* skill = list.GetNext(p);
		if (skill)
		{
			msg << skill->m_proto->m_index
				<< skill->m_level;
		}
	}
}

void ExPetResetSkillMsg(CNetMsg& msg)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PET_RESET_SKILL;
}

void ExPetSellInfoMsg(CNetMsg& msg, CPet* pet)
{
	if (pet == NULL || pet->GetOwner() == NULL)
	{
		msg.Init();
		return ;
	}

	int nRemainGetRebirth = pet->GetRemainRebirthTime();

	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PET_SELL_INFO
		<< pet->GetOwner()->m_index
		<< pet->m_index
		<< pet->GetPetTypeGrade()
		<< pet->m_level
		<< pet->m_exp
		<< pet->GetNeedExp()
		<< pet->m_hp
		<< pet->m_maxHP
		<< pet->GetAbilityPoint()
		<< pet->GetHungryPoint()
		<< (int)PET_MAX_HUNGRY
		<< pet->GetSympathyPoint()
		<< (int)PET_MAX_SYMPATHY
		<< nRemainGetRebirth;
#ifdef PET_NAME_CHANGE
	msg << pet->m_name;
#endif // PET_NAME_CHANGE
}

void ExPetChangeMountMsg(CNetMsg& msg, MSG_EX_PET_CHANGE_MOUNT_ERROR_TYPE errcode)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PET_CHANGE_MOUNT
		<< errcode;
}

void ExPetCommandMsg(CNetMsg& msg, int petindex, int commandindex, char targettype, int targetindex)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PET_COMMAND
		<< petindex
		<< commandindex
		<< targettype
		<< targetindex;
}

void ExPetMixItemMsg(CNetMsg& msg, MSG_EX_PET_MIX_ITEM_ERROR_TYPE errcode)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PET_MIX_ITEM
		<< errcode;
}

void ExPetLevelupMsg(CNetMsg& msg, CPet* pet)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PET_LEVELUP
		<< pet->m_index
		<< pet->GetPetTypeGrade()
		<< pet->m_level;
#ifdef PET_DIFFERENTIATION_ITEM
	msg << pet->GetPetColor();
#endif // PET_DIFFERENTIATION_ITEM
}

#endif // #ifdef ENABLE_PET

#ifdef ATTACK_PET // 공격형 펫 관련
void ExAPetStatusMsg(CNetMsg& msg, CAPet* pet)
{
	float x = 0.0f;
	float z = 0.0f;
	float h = 0.0f;
	float r = 0.0f;
	char y = 0;
	unsigned char mapAttr = MAPATT_BLOCK;

	if (IS_IN_CELL(pet))
	{
		x = GET_X(pet);
		z = GET_Z(pet);
		h = GET_H(pet);
		r = GET_R(pet);
		y = GET_YLAYER(pet);
		mapAttr = pet->GetMapAttr();
	}

	int	nEqAttack = 0;
	if( pet->m_wearing[APET_WEAR_WEAPON] )
	{
		if( pet->m_wearing[APET_WEAR_WEAPON]->IsRangeWeapon() )
		{
			nEqAttack = pet->m_eqRange + pet->m_opRange;
		}
		else
		{
			nEqAttack = pet->m_eqMelee + pet->m_opMelee;
		}
	}

	msg.Init(MSG_EXTEND);
	msg << MSG_EX_ATTACK_PET
		<< (unsigned char) MSG_SUB_STAT
		<< pet->m_index
		<< pet->m_name
		<< pet->m_level
		<< pet->m_pProto->m_Type
		<< pet->m_exp
		<< pet->GetNeedExp()
		<< pet->m_hp
		<< pet->m_maxHP
		<< pet->m_mp
		<< pet->m_maxMP
		<< pet->GetFaith()
		<< pet->m_pProto->m_nMaxFaith
		<< pet->GetStamina()
		<< pet->m_pProto->m_nMaxStm
		<< pet->m_str + pet->m_nPlusStr + pet->m_opStr << pet->m_nPlusStr + pet->m_opStr
		<< pet->m_con + pet->m_nPlusCon + pet->m_opCon << pet->m_nPlusCon + pet->m_opCon
		<< pet->m_dex + pet->m_nPlusDex + pet->m_opDex << pet->m_nPlusDex + pet->m_opDex
		<< pet->m_int + pet->m_nPlusInt + pet->m_opInt << pet->m_nPlusInt + pet->m_opInt
		<< pet->m_nRemainStat
		<< pet->m_nAttack + nEqAttack
		<< pet->m_nMagicAttack + pet->m_eqMagic + pet->m_opMagic
		<< pet->m_nDefence + pet->m_eqDefense + pet->m_opDMelee
		<< pet->m_nMagicDefence + pet->m_eqResist + pet->m_opResist
		<< (int)( pet->m_nHitPoint + pet->m_dex + pet->m_opMeleeHitRate )		//(int)pet->GetFinalHitRate()
		<< (int)( pet->m_nAvoid + ( pet->m_dex * 0.25 ) + pet->m_opMeleeAvoid )	//(int)pet->GetFinalAvoidRate()
		<< x
		<< z
		<< h
		<< r
		<< y
		<< mapAttr;
}

void ExAPetFuntionMsg( CNetMsg& msg, MSG_EX_ATTACK_PET_ERRORTYPE subtype, CAPet* apet, unsigned char error )
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_ATTACK_PET
		<< (unsigned char)subtype;

	switch( subtype )
	{
	case MSG_SUB_SKILLLIST :
		{
			if( apet )
			{
				msg << apet->m_skillList.count();
				void* pos = apet->m_skillList.GetHeadPosition();
				while ( pos )
				{
					CSkill* pSkill = apet->m_skillList.GetNext(pos);
					msg << pSkill->m_proto->m_index
						<< pSkill->m_level;
				}
			}
		}
		break;
	case MSG_SUB_DELETE_EQUIP:
		{
			if( apet )
			{
				msg << apet->m_name;
			}
			return;
		}
		break;
	case MSG_SUB_SKILLLEAN:
	case MSG_SUB_REBIRTH:
	case MSG_SUB_LEVELUP:
	default:
		break;
	}
	msg << error;
}

void ItemAPetWearMsg( CNetMsg& msg, char wear_pos, CItem* item1, CItem* item2 )
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_ATTACK_PET
		<< (unsigned char)MSG_SUB_ITEM_WEAR
		<< (char)wear_pos;

	if (item1)
	{
		msg << (int)item1->m_idNum;
	}
	else
	{
		msg << (int)-1;
	}

	if (item2)
	{
		msg << (int)item2->m_idNum;
	}
	else
	{
		msg << (int)-1;
	}
}

void ItemAPetWearingMsg( CNetMsg& msg, CAPet* apet )
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_ATTACK_PET
		<< (unsigned char)MSG_SUB_ITEM_WEARING
		<< apet->GetOwner()->m_index
		<< apet->m_index;

	for( int i=0; i<APET_WEARPOINT; i++)
	{
		if (apet->m_wearing[i])
		{
			msg << (int)apet->m_wearing[i]->m_idNum;
			msg << apet->m_wearing[i]->m_plus;
		}
		else
		{
			msg << (int)-1;
			msg << (int)0;
		}
	}
}

void AddItemExAPetMsg( CNetMsg& msg, CItem* addItem )
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_ATTACK_PET
		<< (unsigned char) MSG_SUB_ADDITEM_MSG
		<< addItem->m_wearPos
		<< addItem->m_idNum
		<< addItem->m_plus
		<< addItem->m_flag
		<< addItem->m_used;
#ifdef COMPOSITE_TIME		
	msg << addItem->m_used_2;
#endif	//COMPOSITE_TIME
#ifdef SET_ITEM
	if (addItem->IsOriginItem())
	{
		msg << (char)addItem->GetOriginOptionCnt();
		for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
		{
			if (addItem->GetOriginIndex(i) > -1)
			{
				msg << (char)addItem->GetOriginIndex(i)
					<< (char)addItem->GetOriginLevel(i);
			}
		}

		msg << (char)addItem->GetBelongType()
			<< (char)addItem->GetOriginSkillCnt();
		for (i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
		{
			if (addItem->GetOriginIndex(i) > -1)
			{
				msg << addItem->GetOriginIndex(i)
					<< (char)addItem->GetOriginLevel(i);
			}
		}
	}
	else
#endif // SET_ITEM
	{
		msg << (char)addItem->m_nOption;
		
		int i;
		for (i=0; i < addItem->m_nOption; i++)
		{
			msg << (char)addItem->m_option[i].m_type
				<< (char)addItem->m_option[i].m_level;
		}
	}
}

void ExApetSellInfo( CNetMsg& msg, CAPet* apet )
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_ATTACK_PET
		<< (unsigned char)MSG_SUB_APET_INFO
		<< apet->m_index
		<< apet->m_name
		<< apet->m_level
		<< apet->m_str + apet->m_nPlusStr + apet->m_opStr
		<< apet->m_con + apet->m_nPlusCon + apet->m_opCon
		<< apet->m_dex + apet->m_nPlusDex + apet->m_opDex
		<< apet->m_int + apet->m_nPlusInt + apet->m_opInt;
//		<< apet->m_skillList.count();
//		void* pos = apet->m_skillList.GetHeadPosition();
//		while( pos )
//		{
//			CSkill* pSkill = apet->m_skillList.GetNext(pos);
//			msg << pSkill->m_proto->m_index << pSkill->m_level;
//		}
}
#endif //ATTACK_PET

#ifdef APET_AI
void APetAIListMsg(CNetMsg& msg, CAPet* apet)
{
	if (!apet) return;

	msg.Init(MSG_EXTEND);
	msg << MSG_EX_ATTACK_PET
		<< (unsigned char)MSG_SUB_AI_LIST_NTF
		<< apet->m_nAISlot
		<< apet->m_nAICount;

	for (int i = 0; i < apet->m_nAISlot; i++)
	{
		if (apet->IsEmptyAILine(i) == false)
		{
			msg << i
				<< apet->m_tAIData[i].nItemNum
				<< apet->m_tAIData[i].nSkillIdx
				<< apet->m_tAIData[i].nSkillType
				<< apet->m_tAIData[i].cUse;
		}
	}
}

void APetAIOnOffMsg(CNetMsg& msg, CAPet* apet, unsigned char ucError)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_ATTACK_PET
		<< (unsigned char)MSG_SUB_AI_ONOFF_REP;

	if( apet )
		msg << apet->m_cAI;
	else
		msg << 0;

	msg	<< ucError;
}
#endif // APET_AI

void EventChangeWithoutOptionMsg(CNetMsg& msg, int olditem, int newsubtype)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_CHANGE_WITHOUT_OPTION
		<< olditem
		<< newsubtype;
}

void Event2pan4panLetterMsg(CNetMsg& msg, MSG_EVENT_2PAN4PAN_WORD_TYPE wordtype)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_2PAN4PAN_LETTER
		<< wordtype;
}

void ConnEvent2pan4panLetterReqMsg(CNetMsg& msg, int userindex, int charindex, MSG_EVENT_2PAN4PAN_WORD_TYPE wordtype)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_LETTER_REQ
		<< userindex
		<< charindex
		<< wordtype;
}

void ConnEvent2pan4panLetterRollbackReqMsg(CNetMsg& msg, int userindex, int charindex, MSG_EVENT_2PAN4PAN_WORD_TYPE wordtype)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_LETTER_ROLLBACK_REQ
		<< userindex
		<< charindex
		<< wordtype;
}

void Event2pan4panBoxCheckMsg(CNetMsg& msg, int count)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_2PAN4PAN_BOX_CHECK
		<< count;
}

void Event2pan4panBoxRepMsg(CNetMsg& msg, MSG_EVENT_2PAN4PAN_BOX_ERROR_TYPE errcode, int count, int* items, LONGLONG* itemcounts)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_2PAN4PAN_BOX_REP
		<< (int)errcode
		<< count;
	int i;
	for (i = 0; i < count; i++)
		msg << items[i]
			<< itemcounts[i];
}

void Event2pan4panGoodsCheckMsg(CNetMsg& msg, int count)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_2PAN4PAN_GOODS_CHECK
		<< count;
}

void Event2pan4panGoodsRepMsg(CNetMsg& msg, MSG_EVENT_2PAN4PAN_GOODS_ERROR_TYPE errcode, int count, int* items, LONGLONG* itemcounts)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_2PAN4PAN_GOODS_REP
		<< (int)errcode
		<< count;
	int i;
	for (i = 0; i < count; i++)
		msg << items[i]
			<< itemcounts[i];
}

void Event2pan4panMoneyRepMsg(CNetMsg& msg, int success)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_2PAN4PAN_MONEY_REP
		<< success;
}

#ifdef EVENT_PCBANG
void EventPCbangRepMsg(CNetMsg& msg, int index)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_PCBANG
		<< index;
}
#endif

void ConnEvent2pan4panGoodsReqMsg(CNetMsg& msg, int userindex, int charindex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_GOODS_REQ
		<< userindex
		<< charindex;
}

void ConnEvent2pan4panGoodsCommitMsg(CNetMsg& msg, int userindex, int charindex, int count, int* goodindex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_GOODS_COMMIT
		<< userindex
		<< charindex
		<< count;
	int i;
	for (i = 0; i < count; i++)
	{
		msg << goodindex[i];
	}
}

void ConnEvent2pan4panBoxReqMsg(CNetMsg& msg, int userindex, int charindex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_BOX_REQ
		<< userindex
		<< charindex;
}

void ConnEvent2pan4panBoxCommitMsg(CNetMsg& msg, int userindex, int charindex, int count, int* goodindex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_BOX_COMMIT
		<< userindex
		<< charindex
		<< count;
	int i;
	for (i = 0; i < count; i++)
	{
		msg << goodindex[i];
	}
}

void ConnEvent2pan4panMoneyReqMsg(CNetMsg& msg, int userindex, int charindex, int itemindex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_2PAN4PAN_MONEY_REQ
		<< userindex
		<< charindex
		<< itemindex;
}
#ifdef EVENT_PARTNER_CODENEO
void ConnEventPartnerCodeNEOReqMsg( CNetMsg& msg, int userindex, int createcharacter )
{
	msg.Init( MSG_CONN_REQ );
	msg << (unsigned char) MSG_CONN_EVENT_PARTNER_CODENEO_REQ
		<< userindex
		<< createcharacter;
}
#endif

#ifdef RANKER_NOTICE
void ConnRankerReqMsg(CNetMsg& msg, int userindex, int charindex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_RANKER_REQ
		<< userindex
		<< charindex;
}
#endif// RANKER_NOTICE

#ifdef ENABLE_WAR_CASTLE
void ExCastleMapRecentMsg(CNetMsg& msg, CWarCastle* castle, CPC* pc)
{
	msg.Init(MSG_UNKNOWN);
	int zoneindex = castle->GetZoneIndex();
	zoneindex = gserver.FindZone(zoneindex);
	if (zoneindex < 0)
		return ;
	CZone* zone = gserver.m_zones + zoneindex;

	float lordx = -1.0f;
	float lordz = -1.0f;
	int lordhp = 0;
	int lordmaxhp = 0;

	int towercount = 0;

	// 권좌
	CNPC* lord = castle->GetLordSymbol();
	if (lord)
	{
		lordx = GET_X(lord);
		lordz = GET_Z(lord);
		lordhp = lord->m_hp;
		lordmaxhp = lord->m_maxHP;
	}

	// 수호탑
	towercount = castle->GetGateNPCCount();

	// 리젠 위치
	int joinflag = pc->GetJoinFlag(castle->GetZoneIndex());
	int pos = castle->GetRegenPoint(joinflag, pc);
	float regenx = (zone->m_zonePos[pos][1] + zone->m_zonePos[pos][3]) / 2 / 2.0f;
	float regenz = (zone->m_zonePos[pos][2] + zone->m_zonePos[pos][4]) / 2 / 2.0f;

	// 시그널
	int flag = -1;
	int senderindex = -1;
	float signalx = 0.0f;
	float signalz = 0.0f;
	GUILD_SIGNAL* guildsignal = NULL;
	if (pc->m_guildInfo && pc->m_guildInfo->guild())
		guildsignal = pc->m_guildInfo->guild()->guildSignal();

	if (guildsignal)
	{
		flag = guildsignal->flag;
		senderindex = guildsignal->senderIndex;
		signalx = guildsignal->x;
		signalz = guildsignal->z;
	}

	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_MAP_RECENT
		<< lordx
		<< lordz
		<< lordhp
		<< lordmaxhp
		<< towercount;

	int i;
	for (i = 0; i < towercount; i++)
	{
		CNPC* tower = castle->GetGateNPC(i);
		msg << tower->m_proto->m_index
			<< GET_X(tower)
			<< GET_Z(tower)
			<< tower->m_hp
			<< tower->m_maxHP;
	}

	msg << regenx
		<< regenz
		<< flag
		<< senderindex
		<< signalx
		<< signalz;
}

void ExCastleMapSignalMsg(CNetMsg& msg, MSG_GUILD_POSITION_TYPE flag, CPC* sender, float x, float z)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_MAP_SIGNAL
		<< x
		<< z
		<< flag
		<< sender->m_index;
}
#endif // #ifdef ENABLE_WAR_CASTLE

void EventXMas2005Msg(CNetMsg& msg, MSG_EVENT_XMAS_2005_ERROR_TYPE errcode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_XMAS_2005
		<< errcode;
}

void EventNewYear2006CheckMsg(CNetMsg& msg, int timesec)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_NEWYEAR_2006_CHECK
		<< timesec;
}

void EventNewYear2006GoodsMsg(CNetMsg& msg, MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_TYPE errcode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_NEWYEAR_2006_GOODS
		<< errcode;
}

#ifdef EVENT_SEARCHFRIEND
//휴면케릭
//1. 휴면케릭 등록하는 부분
void EventHelperSearchFriendAddReqMsg(CNetMsg& msg
			, int approvalindex, const char* appnick, int appjob
			, int appuserindex, int appserverold, const char* reqnick)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_SEARCHFRIEND_ADD_REQ		
		<< approvalindex
		<< appnick
		<< appjob		
		<< appuserindex
		<< appserverold		
		<< reqnick;
}
//1. 휴면케릭 등록 진행 후 성공 여부를 클라이언트로 보내는 부분
void EventSearchFriendErrorMsg(CNetMsg& msg, MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode)
{
	//클라이언트로 날리는 부분. 에러상태. 판정
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_SEARCHFRIEND_ADD_CHECK
		<< errcode;
}
//2. 휴면케릭이 자신을 등록한 사람들의 리스트를 클라이언트로 출력하는 부분 
void EventDormantSearchFriendSelectMsg(CNetMsg& msg, int listCount, int* requestindex, const char reqnick[][MAX_CHAR_NAME_LENGTH + 1], int startindex, int nTotal)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_SEARCHFRIEND_ADDSELECT_LIST
		<< listCount
		<< startindex
		<< nTotal;

	for(int i = 0; i < listCount; i++)
	{
		msg << requestindex[i]
			<< reqnick[i];
	}
}
//3. 휴면케릭이 리스트에서 한명을 선택한 결과를 등록하는 부분(Helper이동)
void EventHelperSearchFriendSelectAddReqMsg(CNetMsg& msg, int approvalindex, int requestindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_SEARCHFRIEND_SELECT_ADD_REQ		
		<< approvalindex
		<< requestindex;
} 
//3. 휴면케릭 리스트 선택 결과.
void EventSearchFriendSelectAddErrorMsg(CNetMsg& msg, MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode)
{		
	//클라이언트로 날리는 부분. 에러상태. 판정
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_SEARCHFRIEND_ADDSELECT_CHECK		
		<< errcode;
}
//4. 휴면 케릭 사냥시간 체크
void EventSearchFriendTimeCheckMsg(CNetMsg& msg, int timesec)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_SEARCHFRIEND_TIME_CHECK
		<< timesec;
}
//5. 휴면 케릭 사냥시간 한시간 단위로 저장 (Helper)
void EventHelperSearchFriendOneTimeCheckReqMsg(CNetMsg& msg, int timesec, int appDormantindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_SEARCHFRIEND_ONETIME_ADD_REQ		
		<< timesec
		<< appDormantindex;
}
//5. 휴면 케릭 사냥시간 한시간 단위 출력
void EventSearchFriendOneTimeAddErrorMsg(CNetMsg& msg, int timesec, MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_SEARCHFRIEND_ONETIME_CHECK
		<< timesec;
}
//6. 등록된 친구 보상가능여부 요청 (Helper)
void EventHelperSearchFriendListGoodReqMsg(CNetMsg& msg, int approvalindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_SEARCHFRIEND_LISTGOOD_REQ		
		<< approvalindex;
}
//6. 등록된 친구 보상가능여부 리스트 클라이언트로...
void EventSearchFriendListGoodsMsg(CNetMsg& msg, int request_listmember, int* request_index, const char reqnick[][MAX_CHAR_NAME_LENGTH + 1], int* item_good, int startindex, int nTotal)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_SEARCHFRIEND_LIST_GOODS
		<< request_listmember
		<< startindex
		<< nTotal;

	for (int i = 0; i < request_listmember; i++)
	{
		msg << request_index[i]			
			<< reqnick[i]			
			<< item_good[i];
	}	
}
//7. 등록된 친구 보상 후 상태 저장.(Helper)
void EventHelperSearchFriendGoodReqMsg(CNetMsg& msg, int approvalindex, int ndormantindex, int itemgood)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_SEARCHFRIEND_GOOD_REQ		
		<< approvalindex
		<< ndormantindex
		<< itemgood;
}
//7. 휴면 케릭 아이템 선택시 상태를 보내는 부분 클라이언트로..
void EventSearchFriendGoodsMsg(CNetMsg& msg, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TYPE errcode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_SEARCHFRIEND_GOODS_CHECK
		<< errcode;
}
#endif // #ifdef EVENT_SEARCHFRIEND

void ExElementalStatusMsg(CNetMsg& msg, CElemental* elemental)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_ELEMENTAL_STATUS
		<< elemental->m_index
		<< elemental->GetElementalType()
		<< elemental->GetRemainTime()
		<< elemental->m_hp
		<< elemental->m_maxHP
		<< elemental->m_attackSpeed
		<< elemental->m_magicSpeed
		<< elemental->m_skillSpeed
		<< elemental->m_walkSpeed
		<< elemental->m_runSpeed
		<< elemental->m_attackRange
		<< GET_X(elemental)
		<< GET_Z(elemental)
		<< GET_H(elemental)
		<< GET_R(elemental)
		<< GET_YLAYER(elemental)
		<< elemental->GetMapAttr();
	elemental->m_assist.AppendAssistToNetMsg(msg);
}

void ExElementalDeleteMsg(CNetMsg& msg, CElemental* elemental)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_ELEMENTAL_DELETE
		<< elemental->m_index;
}

void ExEvocationStartMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_EVOCATION_START
		<< ch->m_index
		<< ch->m_evocationType;
}

void ExEvocationStopMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_EVOCATION_STOP
		<< ch->m_index;
}

void ExKeyChangeMsg(CNetMsg& msg, unsigned int nKey)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_KEYCHANGE
		<< nKey;
}

void ExPetChangeItemMsg(CNetMsg& msg, MSG_EX_PET_CHANGE_ITEM_ERROR_TYPE errcode)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PET_CHANGE_ITEM
		<< (int)errcode;
}

void SysCannotWearDeadPetMsg(CNetMsg& msg, int nPetIndex, int nRemainRebirth)
{
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_CANNOT_WEAR_DEADPET
		<< nPetIndex
		<< nRemainRebirth;
}

void ExPetRebirthMsg(CNetMsg& msg, int nPetIndex, MSG_EX_PET_REBIRTH_ERROR_TYPE errcode)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PET_REBIRTH
		<< nPetIndex
		<< errcode;
}

void ExPartyRecallNoticeMsg(CNetMsg& msg)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PARTY_RECALL
		<< MSG_EX_PARTY_RECALL_NOTICE;
}

void ExPartyRecallPromptMsg(CNetMsg& msg, int charindex, const char* charname)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PARTY_RECALL
		<< MSG_EX_PARTY_RECALL_PROMPT
		<< charindex
		<< charname;
}

void ExPartyRecallCancelMsg(CNetMsg& msg, int requestcharindex, const char* requestcharname, int targetcharindex, const char* targetcharname)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PARTY_RECALL
		<< MSG_EX_PARTY_RECALL_CANCEL
		<< requestcharindex
		<< requestcharname
		<< targetcharindex
		<< targetcharname;
}
#ifdef NEW_SERIAL_PACKAGE_EVENT
void ConnCouponConfirm_new( CNetMsg& msg, int charindex, const char* serialKey )
{
	msg.Init(MSG_CONN_REQ);
	// msg << (unsigned char)MSG_CONN_COUPON_CONFIRM_NEW
	msg << (unsigned char)MSG_CONN_COUPON_CONFIRM
		<< charindex
		<< serialKey;
}

void ConnCouponUse_new( CNetMsg& msg, int charindex, int promotionindex, int promotionType, int GiftItemCount, PromotionItem* pPromotionItem )
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_COUPON_USE_NEW
		<< charindex
		<< promotionindex
		<< promotionType
		<< GiftItemCount;
	for( int i = 0; i < GiftItemCount; i++ )
	{
	msg <<	pPromotionItem[i].m_itemIndex
		<<	pPromotionItem[i].m_itemCount
		<<	pPromotionItem[i].m_itemPlus
		<<	pPromotionItem[i].m_itemFlag;
//		<<	pPromotionItem[i].m_itemOption0;
//		<<	pPromotionItem[i].m_itemOption1;
//		<<	pPromotionItem[i].m_itemOption2;
//		<<	pPromotionItem[i].m_itemOption3;
//		<<	pPromotionItem[i].m_itemOption4;
	}
}
#endif // NEW_SERIAL_PACKAGE_EVENT


#ifdef EVENT_PACKAGE_ITEM
void ConnCouponConfirm(CNetMsg& msg, int charindex, const char* coupon)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_COUPON_CONFIRM
		<< charindex
		<< coupon;
}

#ifdef EVENT_PACKAGE_ITEM_DBLOG

#ifdef LC_HBK
void ConnCouponUse(CNetMsg& msg, int charindex, int cIndex, int type, CLCString idname, CLCString nick, int randidx1, int randcnt1, int randidx2, int randcnt2)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_COUPON_USE
		<< charindex
		<< cIndex
		<< type
		<< idname
		<< nick
		<< gserver.m_serverno
		<< randidx1
		<< randcnt1
		<< randidx2
		<< randcnt2;
}
#else // LC_HBK
void ConnCouponUse(CNetMsg& msg, int charindex, int cIndex, int type, CLCString idname, CLCString nick)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_COUPON_USE
		<< charindex
		<< cIndex
		<< type
		<< idname
		<< nick
		<< gserver.m_serverno;
}
#endif // LC_HBK

#else // EVENT_PACKAGE_ITEM_DBLOG

#ifdef LC_HBK
void ConnCouponUse(CNetMsg& msg, int charindex, int cIndex, int type, int randidx1, int randcnt1, int randidx2, int randcnt2)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_COUPON_USE
		<< charindex
		<< cIndex
		<< type
		<< randidx1
		<< randcnt1
		<< randidx2
		<< randcnt2;
}
#else // LC_HBK
void ConnCouponUse(CNetMsg& msg, int charindex, int cIndex, int type)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_COUPON_USE
		<< charindex
		<< cIndex
		<< type;
}
#endif // LC_HBK
#endif // EVENT_PACKAGE_ITEM_DBLOG

#endif

void HelperPartyMemberChangeJobMsg(CNetMsg& msg, int nBossIndex, int nCharIndex, char job1, char job2)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< gserver.m_subno
		<< (int)-1
		<< (unsigned char)MSG_HELPER_PARTY_MEMBER_CHANGE_JOB
		<< nBossIndex
		<< nCharIndex
		<< job1
		<< job2;
}

void HelperPartyChatMsg(CNetMsg& msg, int nBossIndex, int nCharIndex, const char* strName, const char* strChat)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< gserver.m_subno
		<< (int)-1
		<< (unsigned char)MSG_HELPER_PARTY_CHAT
		<< nBossIndex
		<< nCharIndex
		<< strName
		<< strChat;
}

void HelperPartyRecallPromptMsg(CNetMsg& msg, int nBossIndex, int nReqIndex, const char* strReqName, char cIsInCastle, int nGuildIndex)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< gserver.m_subno
		<< (int)-1
		<< (unsigned char)MSG_HELPER_PARTY_RECALL_PROMPT
		<< nBossIndex
		<< nReqIndex
		<< strReqName
		<< cIsInCastle
		<< nGuildIndex;
}

void HelperPartyRecallConfirmMsg(CNetMsg& msg, int nBossIndex, int nReqIndex, const char* strReqName, int nRepIndex, const char* strRepName, char yesno)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< gserver.m_subno
		<< (int)-1
		<< (unsigned char)MSG_HELPER_PARTY_RECALL_CONFIRM
		<< nBossIndex
		<< nReqIndex
		<< strReqName
		<< nRepIndex
		<< strRepName
		<< yesno;
}

void HelperPartyRecallProcMsg(CNetMsg& msg, int nBossIndex, int nCharIndex, int nZone, float x, float z, char nYlayer, char cIsInCastle, int nGuildIndex)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< gserver.m_subno
		<< (int)-1
		<< (unsigned char)MSG_HELPER_PARTY_RECALL_PROC
		<< nBossIndex
		<< nCharIndex
		<< nZone
		<< x
		<< z
		<< nYlayer
		<< cIsInCastle
		<< nGuildIndex;
}

void HelperPartyInviteReqMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nTargetIndex, char partyType)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_INVITE_REQ
		<< nBossIndex
		<< strBossName
		<< nBossLevel
		<< nTargetIndex
		<< partyType;
}

void HelperPartyInviteRepMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nTargetIndex, const char* strTargetName, char partyType, MSG_HELPER_PARTY_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_INVITE_REP
		<< nBossIndex
		<< strBossName
		<< nTargetIndex
		<< strTargetName
		<< partyType
		<< errcode;
}

void HelperPartyAllowReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, const char* strTargetName)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_ALLOW_REQ
		<< nBossIndex
		<< nTargetIndex
		<< strTargetName;
}

void HelperPartyRejectReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_REJECT_REQ
		<< nBossIndex
		<< nTargetIndex;
}

void HelperPartyQuitReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_QUIT_REQ
		<< nBossIndex
		<< nTargetIndex;
}

void HelperPartyKickReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_KICK_REQ
		<< nBossIndex
		<< nTargetIndex;
}

void HelperPartyChangeBossReqMsg(CNetMsg& msg, int nBossIndex, const char* strTargetName)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_CHANGE_BOSS_REQ
		<< nBossIndex
		<< strTargetName;
}

#ifdef NEW_UI
void HelperPartyChangeTypeReqMsg(CNetMsg& msg, int nBossIndex, char partytype,char cDiviType)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_CHANGE_TYPE_REQ
		<< nBossIndex
		<< partytype
		<< cDiviType;
}
#endif // NEW_UI

#ifdef PARTY_RAID
void HelperPartyEndPartyReqMsg(CNetMsg& msg, int nBossIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_END_PARTY_REQ
		<< nBossIndex;
}
#endif //PARTY_RAID

void MsgrEventOXSetStartMsg(CNetMsg& msg, int nGMCharIndex, int yy, int mm, int dd, int hh, int min)
{
	msg.Init(MSG_MSGR_REQ);
	msg	<< -1
		<< gserver.m_serverno
		<< gserver.m_subno
		<< -1
		<< (unsigned char)MSG_MSGR_EVENT_OX
		<< MSG_MSGR_EVENT_OX_SET_START
		<< nGMCharIndex
		<< yy
		<< mm
		<< dd
		<< hh
		<< min;
}

void MsgrEventOXEndMsg(CNetMsg& msg, int nGMCharIndex)
{
	msg.Init(MSG_MSGR_REQ);
	msg	<< -1
		<< -1
		<< -1
		<< -1
		<< (unsigned char)MSG_MSGR_EVENT_OX
		<< MSG_MSGR_EVENT_OX_END
		<< nGMCharIndex;
}

void MsgrEventOXQuizMsg(CNetMsg& msg, int nGMCharIndex, int nQuizNo, int nSec)
{
	msg.Init(MSG_MSGR_REQ);
	msg	<< -1
		<< -1
		<< -1
		<< -1
		<< (unsigned char)MSG_MSGR_EVENT_OX
		<< MSG_MSGR_EVENT_OX_QUIZ
		<< nGMCharIndex
		<< nQuizNo
		<< nSec;
}

void EventOXQuizMsg(CNetMsg& msg, MSG_EVENT_OXQUIZ_TYPE subtype)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_OXQUIZ
		<< subtype;
}

void EventOXQuizQuizMsg(CNetMsg& msg, int nQuizNo, int nSec, const char* strQuiz)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_OXQUIZ
		<< MSG_EVENT_OXQUIZ_QUIZ
		<< nQuizNo
		<< nSec
		<< strQuiz;
}

void EventOXQuizAnswerMsg(CNetMsg& msg, int nQuizNo, bool bAnswer)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_OXQUIZ
		<< MSG_EVENT_OXQUIZ_ANSWER
		<< nQuizNo;
	if (bAnswer)
		msg << (char)1;
	else
		msg << (char)0;
}

#ifdef RECOMMEND_SERVER_SYSTEM
void MsgrRecommendMsg(CNetMsg& msg, int nGMCharIndex, int nRecommendServer)
{
	msg.Init(MSG_MSGR_REQ);
	msg << (int)-1
		<< (int)-1
		<< (int)-1
		<< (int)-1
		<< (unsigned char)MSG_MSGR_RECOMMEND
		<< nGMCharIndex
		<< nRecommendServer;
}

void EventRecommendSetMsg(CNetMsg& msg)
{
	char cFlag = (gserver.m_bRecommend) ? 1 : 0;

	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_RECOMMEND
		<< MSG_EVENT_RECOMMEND_SET
		<< cFlag;
}

void EventRecommendPotionMsg(CNetMsg& msg)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_RECOMMEND
		<< MSG_EVENT_RECOMMEND_POTION;
}

void MsgrRecommendMoonstoneMsg(CNetMsg& msg, int nCharIndex, const char* strCharName)
{
	msg.Init(MSG_MSGR_REQ);
	msg << (int)-1
		<< gserver.m_serverno
		<< gserver.m_subno
		<< (int)-1
		<< (unsigned char)MSG_MSGR_RECOMMEND_MOONSTONE
		<< nCharIndex
		<< strCharName;
}

void EventRecommendMoonstoneMsg(CNetMsg& msg, int nCharIndex, const char* strCharName)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_RECOMMEND
		<< MSG_EVENT_RECOMMEND_MOONSTONE
		<< nCharIndex
		<< strCharName;
}
#endif // RECOMMEND_SERVER_SYSTEM

#ifdef PARTY_MATCHING
void HelperPartyMatchRegMemberReqMsg(CNetMsg& msg, int nCharIndex, const char* strCharName, int nLevel, int nZone, char nJob, char nPartyType)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_REG_MEMBER_REQ
		<< nCharIndex
		<< strCharName
		<< nLevel
		<< nZone
		<< nJob
		<< nPartyType;
}

void HelperPartyMatchRegPartyReqMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nZone, int nJobFlag, char cLimitLevel, const char* strComment)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_REG_PARTY_REQ
		<< nBossIndex
		<< strBossName
		<< nBossLevel
		<< nZone
		<< nJobFlag
		<< cLimitLevel
		<< strComment;
}

void ExPartyMatchRegMemberRepMsg(CNetMsg& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE errcode)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PARTY_MATCH
		<< MSG_EX_PARTY_MATCH_REG_MEMBER_REP
		<< errcode;
}

void ExPartyMatchRegPartyRepMsg(CNetMsg& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE errcode)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PARTY_MATCH
		<< MSG_EX_PARTY_MATCH_REG_PARTY_REP
		<< errcode;
}

void ExPartyMatchMemberListRepMsg(CNetMsg& msg, int nPageNo, int nCharLevel, CLCList<CPartyMatchMember*>& list)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PARTY_MATCH
		<< MSG_EX_PARTY_MATCH_MEMBER_LIST_REP
		<< nPageNo
		<< list.GetCount();
	void* pos = list.GetHead();
	while (pos)
	{
		CPartyMatchMember* pMatchMember = list.GetData(pos);
		char cLevelDiff = (ABS(nCharLevel - pMatchMember->GetLevel()) > PARTY_MATCHING_LIMIT_LEVEL) ? 1 : 0;
		msg << pMatchMember->GetIndex()
			<< pMatchMember->GetName()
			<< cLevelDiff
			<< pMatchMember->GetZone()
			<< pMatchMember->GetJob()
			<< pMatchMember->GetPartyType();
		pos = list.GetNext(pos);
	}
}

void ExPartyMatchPartyListRepMsg(CNetMsg& msg, int nPageNo, int nCharLevel, CLCList<CPartyMatchParty*>& list)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PARTY_MATCH
		<< MSG_EX_PARTY_MATCH_PARTY_LIST_REP
		<< nPageNo
		<< list.GetCount();
	void* pos = list.GetHead();
	while (pos)
	{
		CPartyMatchParty* pMatchParty = list.GetData(pos);
		char cLevelDiff = (ABS(nCharLevel - pMatchParty->GetBossLevel()) > PARTY_MATCHING_LIMIT_LEVEL) ? 1 : 0;
		char cLimitLevel = (pMatchParty->IsLimitLevel()) ? 1 : 0;
		msg << pMatchParty->GetBossIndex()
			<< cLevelDiff
			<< pMatchParty->GetZone()
			<< pMatchParty->GetJobFlag()
			<< pMatchParty->GetPartyType()
			<< cLimitLevel
			<< pMatchParty->GetComment();
		pos = list.GetNext(pos);
	}
}

void HelperPartyMatchDelReqMsg(CNetMsg& msg, int nCharIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_DEL_REQ
		<< nCharIndex;
}

void ExPartyMatchDelRepMsg(CNetMsg& msg)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PARTY_MATCH
		<< MSG_EX_PARTY_MATCH_DEL_REP;
}

void HelperPartyMatchInviteReqMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nCharIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_INVITE_REQ
		<< nBossIndex
		<< strBossName
		<< nBossLevel
		<< nCharIndex
		<< (char)0;
}

void HelperPartyMatchInviteRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName, char cPartyType)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_INVITE_REP
		<< nErrorCode
		<< nBossIndex
		<< strBossName
		<< nCharIndex
		<< strCharName
		<< cPartyType;
}

void ExPartyMatchInviteRepMsg(CNetMsg& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE nErrorCode, char cPartyType, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PARTY_MATCH
		<< MSG_EX_PARTY_MATCH_INVITE_REP
		<< nErrorCode
		<< cPartyType
		<< nBossIndex
		<< strBossName
		<< nCharIndex
		<< strCharName;
}

void HelperPartyMatchJoinReqMsg(CNetMsg& msg, int nBossIndex, int nCharIndex, const char* strCharName, int nCharLevel, char cCharJob)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_JOIN_REQ
		<< nBossIndex
		<< nCharIndex
		<< strCharName
		<< nCharLevel
		<< cCharJob;
}

void ExPartyMatchJoinRepMsg(CNetMsg& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE nErrorCode, char cPartyType, int nBossIndex, const char* strBossName, int nReqCharIndex, const char* strReqCharName, char cReqCharJob)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PARTY_MATCH
		<< MSG_EX_PARTY_MATCH_JOIN_REP
		<< nErrorCode
		<< cPartyType
		<< nBossIndex
		<< strBossName
		<< nReqCharIndex
		<< strReqCharName
		<< cReqCharJob;
}

void HelperPartyMatchJoinAllowReqMsg(CNetMsg& msg, int nBossIndex, int nCharIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_JOIN_ALLOW_REQ
		<< nBossIndex
		<< nCharIndex;
}

void HelperPartyMatchJoinRejectReqMsg(CNetMsg& msg, int nJoinCharIndex, int nRejectCharIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_JOIN_REJECT_REQ
		<< nJoinCharIndex
		<< nRejectCharIndex;
}

void HelperPartyMatchMemberChangeInfoMsg(CNetMsg& msg, int nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_TYPE nType, const char* strCharName, int nLevel, int nZone)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO
		<< nCharIndex
		<< nType;

	switch (nType)
	{
	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME:
		msg << strCharName;
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL:
		msg << nLevel;
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE:
		msg << nLevel
			<< nZone;
		break;
	}
}

#endif // PARTY_MATCHING

#ifdef LC_HBK
void HelperLevelUpLogMsg(CNetMsg & msg, CPC * pPc)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_LEVELUP_LOG
		<< (const char *)pPc->m_name
		<< pPc->m_level;
}
#endif // LC_HBK

void SysExpireItemcompositeMsg(CNetMsg& msg, int nItemDBIndex)
{
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_EXPIRE_ITEMCOMPOSITE
		<< nItemDBIndex;
}

void EventWorldcupVoteMsg(CNetMsg& msg, int nCountry, MSG_EVENT_WORLDCUP_ERROR_TYPE nErrorCode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_WORLDCUP
		<< MSG_EVENT_WORLDCUP_VOTE
		<< nCountry
		<< nErrorCode;
}

void EventWorldcupGiftMsg(CNetMsg& msg, int nRank, MSG_EVENT_WORLDCUP_ERROR_TYPE nErrorCode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_WORLDCUP
		<< MSG_EVENT_WORLDCUP_GIFT
		<< nRank
		<< nErrorCode;
}

void MsgrEventGoldenballVoteMsg(CNetMsg& msg, int nGMCharIndex, const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMinute)
{
	msg.Init(MSG_MSGR_REQ);
	msg << (int)-1
		<< (int)-1
		<< (int)-1
		<< (int)-1
		<< (unsigned char)MSG_MSGR_EVENT_GOLDENBALL
		<< MSG_MSGR_EVENT_GOLDENBALL_VOTE
		<< nGMCharIndex
		<< strTeam1
		<< strTeam2
		<< nYear
		<< nMonth
		<< nDay
		<< nHour
		<< nMinute;
}

void MsgrEventGoldenballGiftMsg(CNetMsg& msg, int nGMCharIndex, const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMinute, time_t timeEndVote)
{
	msg.Init(MSG_MSGR_REQ);
	msg << (int)-1
		<< (int)-1
		<< (int)-1
		<< (int)-1
		<< (unsigned char)MSG_MSGR_EVENT_GOLDENBALL
		<< MSG_MSGR_EVENT_GOLDENBALL_GIFT
		<< nGMCharIndex
		<< strTeam1
		<< nTeam1Score
		<< strTeam2
		<< nTeam2Score
		<< nYear
		<< nMonth
		<< nDay
		<< nHour
		<< nMinute
		<< (int)timeEndVote;
}

void MsgrEventGoldenballEndMsg(CNetMsg& msg, int nGMCharIndex)
{
	msg.Init(MSG_MSGR_REQ);
	msg << (int)-1
		<< (int)-1
		<< (int)-1
		<< (int)-1
		<< (unsigned char)MSG_MSGR_EVENT_GOLDENBALL
		<< MSG_MSGR_EVENT_GOLDENBALL_END
		<< nGMCharIndex;
}

void EventGoldenballVoteStartMsg(CNetMsg& msg, const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMin)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_GOLDENBALL
		<< MSG_EVENT_GOLDENBALL_VOTE_START
		<< strTeam1
		<< strTeam2
		<< nYear
		<< nMonth
		<< nDay
		<< nHour
		<< nMin;
}

void EventGoldenballVoteEndMsg(CNetMsg& msg, const char* strTeam1, const char* strTeam2)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_GOLDENBALL
		<< MSG_EVENT_GOLDENBALL_VOTE_END
		<< strTeam1
		<< strTeam2;
}

void EventGoldenballGiftStartMsg(CNetMsg& msg, const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMin)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_GOLDENBALL
		<< MSG_EVENT_GOLDENBALL_GIFT_START
		<< strTeam1
		<< nTeam1Score
		<< strTeam2
		<< nTeam2Score
		<< nYear
		<< nMonth
		<< nDay
		<< nHour
		<< nMin;
}

void EventGoldenballGiftEndMsg(CNetMsg& msg, const char* strTeam1, const char* strTeam2)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_GOLDENBALL
		<< MSG_EVENT_GOLDENBALL_GIFT_END
		<< strTeam1
		<< strTeam2;
}

void EventGoldenballVoteMsg(CNetMsg& msg, int nTeam1Score, int nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_TYPE nErrorCode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_GOLDENBALL
		<< MSG_EVENT_GOLDENBALL_VOTE
		<< nTeam1Score
		<< nTeam2Score
		<< nErrorCode;
}

void EventGoldenballGiftMsg(CNetMsg& msg, MSG_EVENT_GOLDENBALL_ERROR_TYPE nErrorCode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_GOLDENBALL
		<< MSG_EVENT_GOLDENBALL_GIFT
		<< nErrorCode;
}

#ifdef EVENT_RAIN_2006
void EventRain2006Msg(CNetMsg& msg, MSG_EVENT_RAIN_2006_ERROR_TYPE nErrorCode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_RAIN_2006
		<< nErrorCode;
}
#endif // EVENT_RAIN_2006

void ItemLendWeaponMsg(CNetMsg& msg, int nItemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_TYPE nErrorCode)
{
	msg.Init(MSG_ITEM);
	msg << (unsigned char)MSG_ITEM_LEND_WEAPON
		<< nItemDBIndex
		<< nErrorCode;
}

void SysSeparateItemMsg(CNetMsg& msg, int nItemDBIndex)
{
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_SEPARATE_ITEM
		<< nItemDBIndex;
}

#ifdef EVENT_TLD_BUDDHIST
void EventTldBuddhistMsg(CNetMsg& msg, char cRequestType, MSG_EVENT_TLD_BUDDHIST_ERROR_TYPE nErrorCode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_TLD_BUDDHIST
		<< cRequestType
		<< nErrorCode;
}
#endif // EVENT_TLD_BUDDHIST

#ifdef EVENT_COLLECT_BUG
void EventCollectBugMsg(CNetMsg& msg, MSG_EVENT_COLLECT_BUG_TYPE nType, int nData)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_COLLECT_BUG
		<< nType
		<< nData;
}
#endif // EVENT_COLLECT_BUG

void QuestCollectMsg(CNetMsg& msg, int nNPCIndex, int nCharIndex, int nItemIndex)
{
	msg.Init(MSG_QUEST);
	msg << (unsigned char)MSG_QUEST_COLLECT
		<< nNPCIndex
		<< nCharIndex
		<< nItemIndex;
}

#ifdef GUILD_RANKING
void HelperSaveExpGuildMsg(CNetMsg& msg, int nGuildIndex, double dExpGuild)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_SAVE_EXP_GUILD
		<< nGuildIndex
		<< dExpGuild;
}
#endif // GUILD_RANKING

#ifdef EVENT_CHUSEOK_2006_GIFT
void EventChuseok2006Msg(CNetMsg& msg, MSG_EVENT_CHUSEOK_2006_TYPE nErrorcode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_CHUSEOK_2006
		<< nErrorcode;
}
#endif // EVENT_CHUSEOK_2006_GIFT

void MsgrLoginServerMsg(CNetMsg& msg, int nCode)
{
	msg.Init(MSG_MSGR_REQ);
	msg << (int)-1
		<< (int)-1
		<< (int)-1
		<< (int)-1
		<< (unsigned char)MSG_MSGR_LOGIN_SERVER
		<< nCode;
}

#ifdef MONSTER_RAID_SYSTEM
void SysRaidmobRegenMsg(CNetMsg& msg, int nNPCIndex, float x, float z)
{
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_RAIDMOB_REGEN
		<< nNPCIndex
		<< x
		<< z;
}
#endif // MONSTER_RAID_SYSTEM



#ifdef EVENT_OPEN_BETA_GIFT
void ConnOpenBetaGiftMsg(CNetMsg& msg, int userindex, int charindex/*, bool bCancel*/)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_OPEN_BETA_GIFT
		<< userindex
		<< charindex;
//	if (bCancel)
//		msg << (char)1;
//	else
//		msg << (char)0;
}

void EventOpenBetaGiftMsg(CNetMsg& msg )
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_OPEN_BETA_GIFT;
}
#endif // EVENT_OPEN_BETA_GIFT

void ExHairChangeMsg(CNetMsg& msg, CPC* pPC )
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_HAIR_CHANGE
		<< pPC->m_index
		<< pPC->m_hairstyle;
#ifdef  NEW_WEARING_HELMET // EVENT_XMAS_2007 과 함께 들어감 // 하이바 시스템 으로 변경
	if( pPC->m_wearing[WEARING_HELMET] )
	{
		msg << pPC->m_wearing[WEARING_HELMET]->m_itemProto->m_index;
	}
#endif //  NEW_WEARING_HELMET
	
}

void ExPlayerStateChangeMsg(CNetMsg& msg, CPC* pPC)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_PLAYER_STATE_CHANGE
		<< pPC->m_index
		<< pPC->GetPlayerState();
}

#ifdef EVENT_XMAS_2006
void EventXmas2006Msg(CNetMsg& msg, int nCakeCount, MSG_EVENT_XMAS_2006_ERROR_TYPE nErrorcode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_XMAS_2006
		<< nCakeCount
		<< nErrorcode;
}
#endif // EVENT_XMAS_2006

void SysTimeoutLuckyAccessoryMsg(CNetMsg& msg, int nItemDBIndex)
{
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_TIMEOUT_LUCKY_ACCESSORY
		<< nItemDBIndex;
}

void SysChangeLuckyBoxMsg(CNetMsg& msg, int nItemDBIndex, int nUseDBIndex)
{
	msg.Init(MSG_SYS);
	msg << (unsigned char)MSG_SYS_CHANGE_LUCKY_BOX
		<< nItemDBIndex
		<< nUseDBIndex;
}


#if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 
void EventValentine2007ErrorMsg(CNetMsg& msg, MSG_EVENT_VALENTINE_TYPE nErrorcode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_VALENTINE_2007
		<< nErrorcode;
}

void EventValentine2007Msg(CNetMsg& msg, MSG_EVENT_VALENTINE_TYPE nErrorcode, int extra)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_VALENTINE_2007
		<< nErrorcode
		<< extra;
}

#endif	// #if defined (EVENT_VALENTINE_2007) || defined (EVENT_VALENTINE_2007_GIFT) 

void ConnGparaPromotionMsg(CNetMsg& msg, MSG_CONN_GPARA_PROMOTION_TYPE subtype, int nUserIndex, int nCharIndex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_GPARA_PROMOTION
		<< subtype
		<< nUserIndex
		<< nCharIndex;
}

void ConnOCN_GooPromotionMsg(CNetMsg& msg, MSG_CONN_OCN_GOO_PROMOTION_TYPE subtype, int nUserIndex, int nCharIndex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_OCN_GOO_PROMOTION
		<< subtype
		<< nUserIndex
		<< nCharIndex;
}

void ConnMSNPromotionMsg(CNetMsg& msg, MSG_CONN_MSN_PROMOTION_TYPE subtype, int nUserIndex, int nCharIndex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_MSN_PROMOTION
		<< subtype
		<< nUserIndex
		<< nCharIndex;
}

#ifdef CREATE_EVENT
void ConnCreateEventMsg(CNetMsg & msg, MSG_CONN_CREATEEVENT_TYPE subtype, int nUserIndex, int nCharIndex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_CREATEEVENT
		<< subtype
		<< nUserIndex
		<< nCharIndex
		<< gserver.m_nCreateEventStartTime
		<< gserver.m_nCreateEventEndTime
		<< gserver.m_nCreateTableIndex;
}
#endif // CREATE_EVENT

#ifdef FREE_PK_SYSTEM
void FreePKMsg(CNetMsg& msg)
{
	msg.Init( MSG_PK );
	msg << (unsigned char)MSG_PK_FREE;
}
#endif //FREE_PK_SYSTEM

#ifdef EVENT_WHITEDAY_2007
void EventWhiteday2007Msg(CNetMsg& msg, MSG_EVENT_WHITEDAY_2007_TYPE nMsg)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_WHITEDAY_2007
		<< nMsg;
}
#endif // EVENT_WHITEDAY_2007

#ifdef EVENT_TLD_2007_SONGKRAN
void EventSongkran2007Msg( CNetMsg& msg, MSG_EVENT_SONGKRAN_2007_TYPE nMsg, int type )
{
	msg.Init( MSG_EVENT );
	msg << (unsigned char)MSG_EVENT_SONGKRAN_2007
		<< nMsg
		<< type;
}
#endif // EVENT_TLD_2007_SONGKRAN

#ifdef EVENT_EGGS_HUNT_2007
void EventEggsHunt2007ErrorMsg(CNetMsg& msg, MSG_EVENT_EGGS_HUNT_2007_TYPE nMsg)
{
	msg.Init( MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_EGGS_HUNT_2007
		<< nMsg;
}

void EventEggsHunt2007GiftMsg(CNetMsg& msg, int nGiftIndex, int nGiftCount)
{
	msg.Init( MSG_EVENT );
	msg << (unsigned char)MSG_EVENT_EGGS_HUNT_2007
		<< MSG_EVENT_EGGS_HUNT_2007_GIFT_REP_OK
		<< nGiftIndex
		<< nGiftCount;
}
#endif // EVENT_EGGS_HUNT_2007


#ifdef EVENT_TEACH_2007
void EventTeach2007Msg(CNetMsg & msg, int value)
{
	msg.Init( MSG_EVENT );
	msg << (unsigned char)MSG_EVENT_TEACH_2007
		<< value;
}

void HelperTeachFameUp2007ReqMsg( CNetMsg & msg, int charidx, int fame )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_TEACH_2007
		<< charidx
		<< fame;		
}

void HelperTeachAddFlowerMsg(CNetMsg & msg, int charidx)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_TEACH_2007_ADDFLOWER
		<< charidx;
};

void HelperReciveFlowerMsg(CNetMsg & msg, int charidx)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_TEACH_2007_RECEIVE_FLOWER
		<< charidx;
}
#endif // EVENT_TEACH_2007

#ifdef EVENT_GOMDORI_2007
void EventGomdori2007Msg(CNetMsg& msg, MSG_EVENT_GOMDORI_2007_TYPE nType)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_GOMDORI_2007
		<< (unsigned char)nType;
}

void EventGomdori2007ResultMsg(CNetMsg& msg, MSG_EVENT_GOMDORI_2007_TYPE nResult, char cWinCount, char cUserSel, char cNPCSel)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_GOMDORI_2007
		<< (unsigned char)nResult
		<< cWinCount
		<< cUserSel
		<< cNPCSel;
}

void EventGomdori2007EndMsg(CNetMsg& msg, char cWinCount, char cPromptType, int nExtra1, int nExtra2)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_GOMDORI_2007
		<< (unsigned char)MSG_EVENT_GOMDORI_2007_END
		<< cWinCount
		<< cPromptType
		<< nExtra1
		<< nExtra2;
}

void EventGomdori2007SelectGiftMsg(CNetMsg& msg, int nItemDBIndex)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_GOMDORI_2007
		<< (unsigned char)MSG_EVENT_GOMDORI_2007_SELECT_GIFT
		<< nItemDBIndex;
}

void EventGomdori2007ViewStatusMsg(CNetMsg& msg, int nCount, int* nStatus)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_GOMDORI_2007
		<< (unsigned char)MSG_EVENT_GOMDORI_2007_VIEW_STATUS
		<< nCount;
	int i;
	for (i = 0; i < nCount; i++)
		msg << nStatus[i];
}

void ConnEventGomdori2007CountMsg(CNetMsg& msg)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_GOMDORI_2007
		<< (unsigned char)MSG_CONN_EVENT_GOMDORI_2007_COUNT;
}

void ConnEventGomdori2007StatusMsg(CNetMsg& msg, int nCharIndex)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_GOMDORI_2007
		<< (unsigned char)MSG_CONN_EVENT_GOMDORI_2007_STATUS
		<< nCharIndex;
}
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_CHILDRENSDAY_2007
void EventChildrensDay2007Msg(CNetMsg& msg, unsigned char nErrorCode)
{
	msg.Init( MSG_EVENT );
	msg << (unsigned char)MSG_EVENT_CHILDRENSDAY_2007
		<< MSG_EVENT_CHILDRENSDAY_2007_REP
		<< nErrorCode;
}

void HelperChildrensDay2007Msg( CNetMsg& msg, MSG_HELPER_CHILDRENSDAY_2007_TYPE subtype , int nCharIndex, int nItemIndex )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_CHILDRENSDAY_2007
		<< (unsigned char) subtype
		<< nCharIndex
		<< nItemIndex;
}
#endif //EVENT_CHILDRENSDAY_2007

#ifdef EVENT_FLOWERTREE_2007
void EventFlowerTree2007Msg(CNetMsg& msg, MSG_EVENT_FLOWERTREE_2007_TYPE subtype )
{
	msg.Init( MSG_EVENT );
	msg << (unsigned char)MSG_EVENT_FLOWERTREE_2007
		<< (unsigned char)subtype;
}

void HelperFlowerTree2007Msg( CNetMsg& msg, MSG_HELPER_FLOWERTREE_2007_TYPE subtype, int nCharIndex )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_FLOWERTREE_2007
		<< (unsigned char)subtype
		<< nCharIndex;
}

void ConnEventFlowerTree2007Msg(CNetMsg& msg, MSG_CONN_EVENT_FLOWERTREE_2007_TYPE subtype, int nCharIndex )
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_FLOWERTREE_2007
		<< (unsigned char)subtype
		<< nCharIndex;
}
#endif //EVENT_FLOWERTREE_2007

#ifdef EVENTSETTING
void MsgEventSetReqMsg(CNetMsg& msg, int server, int subno, int zone, int thisServer, int thisSubno, int charIndex, int flag, int success)
{
	msg.Init(MSG_MSGR_REQ);
	msg << -1
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_EVENT_SETTING_REQ
		<< thisServer
		<< thisSubno
		<< charIndex
		<< flag
		<< success;
}

void MsgEventCurrentRepMsg(CNetMsg& msg, int server, int subno, int zone, int thisServer, int thisSubno, int charIndex)
{
	msg.Init(MSG_MSGR_REQ);
	msg << -1
		<< server
		<< subno
		<< (unsigned char) MSG_MSGR_EVENT_SETTING_REQ
		<< thisServer
		<< thisSubno
		<< charIndex
		<< -1
		<< 3;

	int i;
	for(i = 0; i < MAX_NOTICE; ++i)
	{
		msg << gserver.m_aNotice[i];
	}
}
#endif // EVENTSETTING

#ifdef EVENT_2007_PARENTSDAY
void EventParentsdayAccumulateList( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE err, int count , int* ranking , int* point , const char guildname[][MAX_CHAR_NAME_LENGTH+1] )
{
	msg.Init( MSG_EVENT );
	msg << (unsigned char)MSG_EVENT_PARENTSDAY_2007
		<< (unsigned char)MSG_EVENT_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST
		<< (char)err
		<< count;
	for( int i = 0;  i < count; i++ )
	{
		msg << ranking[i]
			<< point[i]
			<< guildname[i];
	}

}
void EventParentsdayAddPoint( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE err, int CarnationCount, int entirePoint )
{
	msg.Init( MSG_EVENT );
	msg << (unsigned char)MSG_EVENT_PARENTSDAY_2007
		<< (unsigned char)MSG_EVENT_PARENTSDAY_2007_ADD_POINT
		<< (char)err
		<< CarnationCount
		<< entirePoint;

}
void EventParentsdayExchangeTicket( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE err, int TicketCount)
{
	msg.Init( MSG_EVENT );
	msg << (unsigned char)MSG_EVENT_PARENTSDAY_2007
		<< (unsigned char)MSG_EVENT_PARENTSDAY_2007_EXCHANGE_TICKET
		<< (char)err
		<< TicketCount;
}

void EventParentsdayExchangeItem( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE err )
{
	msg.Init( MSG_EVENT );
	msg << (unsigned  char)MSG_EVENT_PARENTSDAY_2007
		<< (unsigned char)MSG_EVENT_PARENTSDAY_2007_EXCHANGE_ITEM
		<< (char)err;
}

void EventParentsdayItemNotice( CNetMsg& msg, const char* guildName, CLCString& itemName )
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_PARENTSDAY_2007
		<< (unsigned char)MSG_EVENT_PARENTSDAY_2007_NOTICE
		<< guildName
		<< itemName;
}

void HelperEventParentsdayAccumulateList( CNetMsg& msg, int charIndex  )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST
		<< charIndex;
}
void HelperEventParentsdayAddPoint( CNetMsg& msg, int charIndex, int guildIndex, int CarnationCount )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_PARENTSDAY_2007_ADD_POINT
		<< charIndex
		<< guildIndex
		<< CarnationCount;
}
void HelperEventParentsdayExchangeTicket( CNetMsg& msg, int charIndex, int guildIndex )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_PARENTSDAY_2007_EXCHANGE_TICKET
		<< charIndex
		<< guildIndex;
}
void HelperEventParentsdayExchangeItem( CNetMsg& msg, int charIndex, int guildIndex )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARENTSDAY_2007_EXCHANGE_ITEM
		<< charIndex
		<< guildIndex;
}
void HelperEventParentsdayExchangeItemInsert( CNetMsg& msg, int charIndex , int GiftItemIndex, int GiftItemCount )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_PARENTSDAY_2007_EXCHANGE_ITEM_INSERT
		<< charIndex
		<< GiftItemIndex
		<< GiftItemCount;
}
#endif // EVENT_2007_PARENTSDAY

#ifdef GMTOOL
void ConnGMToolCommand(CNetMsg& msg, int gmindex, const char* gmname, const char* gmcommand)
{
	msg.Init( MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_GMTOOL_GOMMAND
		<< gmindex
		<< gmname
		<< gmcommand;
}

void ConnGMToolChatMonitor(CNetMsg& msg, int charindex, const char* name, unsigned char chattype, int server, int sub, const char* chat)
{
	msg.Init( MSG_CONN_REQ );
	msg << (unsigned char)MSG_CONN_GMTOOL_CHATMONITOR
		<< charindex
		<< name
		<< chattype
		<< server
		<< sub
		<< chat;
}

void ConnGMToolChatting(CNetMsg& msg, int server, int sub, int gmindex, const char* gmname, const char* name, const char* chat)
{
	msg.Init( MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_GMTOOL_CHATTING
		<< server
		<< sub
		<< gmindex
		<< gmname
		<< name
		<< chat;
}
#endif // GMTOOL




#ifdef PET_NAME_CHANGE
void PetNameChange( CNetMsg& msg, MSG_EX_PET_CHANGE_NAME_ERROR_TYPE err, int petidx, const char* strPetName )
{
	msg.Init(MSG_EXTEND );
	msg << MSG_EX_PET_CHANGE_NAME
		<< (unsigned char)err
		<< petidx
		<< strPetName;
}

void HelperPetNameChange( CNetMsg& msg, int charindex, int petindex, const char* strPetName )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_PET_NAME_CHANGE
		<< charindex
		<< petindex
		<< strPetName;
}

#endif // PET_NAME_CHANGE

#ifdef CASH_EXCHANGE_EQUIP
void ItemExchangeEquipRepMsg(CNetMsg& msg, int olddbindex, int newdbindex)
{
	msg.Init(MSG_ITEM);
	msg << (unsigned char)MSG_ITEM_EXCHANGE_EQUIP_REP
		<< olddbindex
		<< newdbindex;
}
#endif // CASH_EXCHANGE_EQUIP

#ifdef BLOOD_SWEAT_FRUIT
void ItemBloodSweatFruitAddRepMsg(CNetMsg& msg, CItem* item, MSG_ITEM_OPTION_ADD_RESULT result)
{
	msg.Init(MSG_ITEM);

	msg << (unsigned char)MSG_ITEM_OPTION_ADD_REP
		<< (char)result;
}
#endif

#ifdef DOUBLE_ITEM_DROP
void MsgrDoubleItemEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int itemPercent)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_ITEM_EVENT_REP
		<< charindex
		<< gserver.m_serverno
		<< gserver.m_subno
		<< cmd
		<< itemPercent;
}

void MsgrDoubleItemEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int itemPercent)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_ITEM_EVENT_REQ
		<< charindex
		<< cmd
		<< itemPercent;
}
#endif 

#if defined( EVENT_SUMMER_VACATION_2007_RESULT ) || defined (EVENT_SUMMER_VACATION_2007 )
void EventSummerVacationInchenMsg( CNetMsg & msg, MSG_EVENT_SUMMER_VACATION_2007_TYPE vacationType, int itemindex )
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_SUMMER_VACATION_2007
		//<< (unsigned char)MSG_EVENT_SUMMER_VACATION_2007_INCHEN
		<< vacationType
		<< itemindex;
	
}
void EventSummerVacation2007FigureMsg( CNetMsg & msg, MSG_EVENT_SUMMER_VACATION_2007_TYPE vacationType, int figuretype, int itemindex, int count )
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_SUMMER_VACATION_2007
		//<< (unsigned char)MSG_EVENT_SUMMER_VACATION_2007_PAPER_FIGURES
		<< vacationType
		<< figuretype
		<< itemindex
		<< count;
}
#endif// defined( EVENT_SUMMER_VACATION_2007_RESULT ) || defined (EVENT_SUMMER_VACATION_2007 )

#ifdef DOUBLE_PET_EXP
void MsgrDoublePetExpEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int PetExpPercent)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_PET_EXP_EVENT_REQ
		<< charindex
		<< cmd
		<< PetExpPercent;
}
void MsgrDoublePetExpEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int PetExpPercent)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_PET_EXP_EVENT_REP
		<< charindex
		<< gserver.m_serverno
		<< gserver.m_subno
		<< cmd
		<< PetExpPercent;
}
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
void MsgrDoubleAttackEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int AttackPercent)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_ATTACK_EVENT_REQ
		<< charindex
		<< cmd
		<< AttackPercent;
}
void MsgrDoubleAttackEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int AttackPercent)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_DOUBLE_ATTACK_EVENT_REP
		<< charindex
		<< gserver.m_serverno
		<< gserver.m_subno
		<< cmd
		<< AttackPercent;
}
#endif // DOUBLE_ATTACK

#ifdef EVENT_TLD_MOTHERDAY_2007
void EventTldMotherday2007tMsg(CNetMsg& msg, MSG_EVENT_TLD_MOTHERDAY_2007_ERROR_TYPE nErrorCode)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_TLD_MOTHERDAY_2007
		<< nErrorCode;
}
#endif // EVENT_TLD_MOTHERDAY_2007


#ifdef NEW_GUILD
void GuildInclineEstablishMsg( CNetMsg& msg, char guildincline )
{
	msg.Init( MSG_GUILD );
	msg << (unsigned char)MSG_NEW_GUILD_INCLINE_ESTABLISH
		<< guildincline;
}

void GuildNewInfo( CNetMsg& msg, CPC* ch, int avelevel, int guildpoint, int usepoint )
{
#if defined (LC_GER)
	// -- imso 
	if (!ch->m_guildInfo->guild()->boss())
	{
		GAMELOG << init("GuildNewInfo", ch)
				<< "guild_index " << delim << ch->m_guildInfo->guild()->index() << delim
				<< "guild_name " << delim << ch->m_guildInfo->guild()->name() << end;
	}
#endif

	msg.Init( MSG_GUILD );
	msg << (unsigned char)MSG_NEW_GUILD_INFO		
		<< ch->m_guildInfo->guild()->name()
		<< ch->m_guildInfo->guild()->level()
		<< ch->m_guildInfo->guild()->boss()->GetName()
		<< ch->m_guildInfo->guild()->membercount()
		<< ch->m_guildInfo->guild()->maxmember()
		<< avelevel
		<< ch->m_guildInfo->guild()->GetGuildPoint()
		<< ch->m_guildInfo->GetcumulatePoint();
//	int landcount = ch->m_guildInfo->guild()->GetLandCount();
//	if( ch->m_guildInfo->guild()->GetLandCount() > 0 )
//	{
//		msg << landcount;
//		int* land = ch->m_guildInfo->guild()->GetLand();
//		memset(land, -1, sizeof(land) );		
//		for( int i = 0 ; i < landcount; i++ )
//		{
//			msg << land[i];
//		}
//			
//	}
//		
		
}

void GuildMemberListRep( CNetMsg& msg, int membercount, int* membercharindex, int* cumulatePoint,  const char CharName[][MAX_CHAR_NAME_LENGTH], const char PositionName[][GUILD_POSITION_NAME+1],  char* job, char* job2, int* level, int* position, CGuild* guild )
{
	msg.Init( MSG_GUILD );
	msg << (unsigned char)MSG_NEW_GUILD_MEMBERLIST		
		<< membercount	;
	for( int i = 0; i < membercount; i++ )
	{
		
		CGuildMember* guildmember = guild->findmember( membercharindex[i] );
		
		if( guildmember )
		{
//			if( guildmember->GetPC() )
//			{
				msg << CharName[i]
					<< PositionName[i]
					<< level[i]
					<< guildmember->online();

				if( guildmember->online() != 0 )
				{
//					<< guildmember->GetChannel()
					msg << guildmember->GetZoneNum();

				}
				else
				{
//					msg << -1
					msg << -1;
				}
				msg	<< cumulatePoint[i];
#ifdef ENABLE_CHANGEJOB
				msg << job[i]
					<< job2[i]
					//<< position[i];
					<< guildmember->pos();
#else
				msg << m_job[i]					
					//<< position[i];
					<< guildmember->pos();
#endif
//			}
//			else
//			{
//				msg << CharName[i]
//					<< PositionName[i]
//					<< level[i]
//					<< (bool)0
//					<< (int)-1
//					<< cumulatePoint[i];
//#ifdef ENABLE_CHANGEJOB
//				msg << job[i]
//					<< job2[i]
//					<< position[i];
//
//
//#else
//				msg << job[i]
//					<< position[i];	
//
//#endif 
// 			}
		}	
	
	}
}

void GuildNewManageRep( CNetMsg& msg, int membercount, int* membercharindex, int* contributeExp, int* contributeFame, const char CharName[][MAX_CHAR_NAME_LENGTH], const char PositionName[][GUILD_POSITION_NAME+1], int* level, int* position, CGuild* guild, char first )
{
	msg.Init( MSG_GUILD );
	msg << (unsigned char)MSG_NEW_GUILD_MANAGE
		<< first
		<< membercount	;
	for( int i = 0; i < membercount; i++ )
	{
		CGuildMember* guildmember = guild->findmember( membercharindex[i] );
		if( guildmember )
		{
			if( guildmember->GetPC() )
			{
				msg << guildmember->pos()
					<< guildmember->GetName()
					<< guildmember->GetPositionName()
					<< guildmember->GetPC()->m_level
					<< contributeExp[i]
					<< contributeFame[i]
					<< guildmember->charindex();					
			}
			else
			{
				msg << position[i]
					<< CharName[i]
					<< PositionName[i]
					<< level[i]
					<< contributeExp[i]
					<< contributeFame[i]
					<< membercharindex[i];
			}
		}

	}
}

void GuildNewNotify( CNetMsg& msg, const char* title, const char* text )
{
	msg.Init( MSG_GUILD );
	msg << (unsigned char)MSG_NEW_GUILD_NOTICE		
		<< title
		<< text;
}

void GuildNewNotifyTrans( CNetMsg& msg, const char* guildname, const char* title, const char* text )
{
	msg.Init( MSG_GUILD );
	msg << (unsigned char)MSG_NEW_GUILD_NOTICE_TRANSMISSION
		<< guildname
		<< title
		<< text;
}
void GuildSkillListRepMsg( CNetMsg& msg, int skillcount, int* skillIndex, int* skillLearnLevel )
{
	msg.Init( MSG_GUILD );
	msg << (unsigned char)MSG_NEW_GUILD_SKILL
		<< skillcount;
	for( int i = 0; i < skillcount; i++ )
	{
		msg << skillIndex[i]
			<< skillLearnLevel[i];
	}
}

void GuildSkillLearnMsg(CNetMsg& msg, CSkill* skill)
{
	msg.Init(MSG_GUILD);

	msg << (unsigned char)MSG_NEW_GUILD_SKILL_LEARN
		<< skill->m_proto->m_index
		<< (char)((skill->m_level == 1) ? 1 : 0)
		<< skill->m_level;
}


void HelperGuildInclineEstablishReqMsg( CNetMsg& msg, CPC* ch, char guildincline )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_GUILD_INCLINE_ESTABLISH_REQ
		<< ch->m_guildInfo->guild()->index()
		<< ch->m_index
		<< guildincline;
}

void HelperGuildMemberAdjust( CNetMsg& msg, CPC* ch, int charindex, const char* strPositionName, int contributeExp, int contributFame )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_GUILD_MEMBER_ADJUST_REQ
		<< ch->m_guildInfo->guild()->index()
		<< ch->m_index
		<< charindex
		<< strPositionName
		<< contributeExp
		<< contributFame;
	
}

void HelperNewGuildInfo( CNetMsg& msg, CPC* ch )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_INFO_REQ
		<< ch->m_guildInfo->guild()->index()
		<< ch->m_index;
}

void HelperNewGuildMemberList( CNetMsg& msg, CPC* ch )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_MEMBERLIST_REQ
		<< ch->m_guildInfo->guild()->index()
		<< ch->m_index;
}
void HelperNewGuildManage( CNetMsg& msg, CPC* ch )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_MANAGE_REQ
		<< ch->m_guildInfo->guild()->index()
		<< ch->m_index;
}
void HelperNewGuildNotice( CNetMsg& msg, CPC* ch )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_NOTICE_REQ
		<< ch->m_guildInfo->guild()->index()
		<< ch->m_index;
}
void HelperNewGuildNoticeUpdate( CNetMsg& msg, CPC* ch, const char* title, const char* text )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_NOTICE_UPDATE_REQ
		<< ch->m_guildInfo->guild()->index()
		<< ch->m_index
		<< title
		<< text;
}

void HelperNewGuildNoticeTrans( CNetMsg& msg, CPC* ch )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_REQ
		<< ch->m_guildInfo->guild()->index()
		<< ch->m_index;
}

void HelperNewGuildSkillList( CNetMsg& msg, CPC* ch )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_NOTICE_SKILLLIST_REQ
		<< ch->m_guildInfo->guild()->index()
		<< ch->m_index;
}

void HelperSaveGuildPointMsg(CNetMsg& msg, int guildindex, int guildpoint )
{
	msg.Init( MSG_HELPER_COMMAND );

	msg << MSG_HELPER_SAVE_GUILD_POINT
		<< guildindex
		<< guildpoint;
}

void HelperSaveGuildMemberPointMsg( CNetMsg& msg, int guildindex, int charindex, int GuildMemberPoint )
{
	msg.Init( MSG_HELPER_COMMAND );

	msg << MSG_HELPER_SAVE_GUILD_MEMBER_POINT
		<< guildindex
		<< charindex
		<< GuildMemberPoint;
}

//void HelperSaveGuildSkillMsg( CNetMsg& msg, CGuild* guild )
//{
//	char level[1024];	// for temp string
//	char index[1024];	// for temp string
//
//	msg.Init( MSG_HELPER_COMMAND );
//	msg << MSG_HELPER_SAVE_GUILD_SKILL
//		<< guild->index()		
//		<< guild->m_passiveSkillList.GetIndexString(index)
//		<< guild->m_passiveSkillList.GetLevelString(level);
//}

void HelperNewGuildPointUpdate( CNetMsg& msg, int charindex, int guildindex, int point )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_UPDATE_GUILD_POINT
		<< charindex
		<< guildindex
		<< point;
}

void HelperGuildLoadReq(CNetMsg& msg, int charindex )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_GUILD_NOTICE
		<< charindex;
}

void HelperGuildSkillLearnMsg( CNetMsg& msg, int gp, CGuild* guild  )
{
	char level[1024];	// for temp string
	char index[1024];	// for temp string

	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_GUILD_SKILL_LEARN		
		<< guild->index()
		<< gp
		<< guild->m_passiveSkillList.GetIndexString(index)
		<< guild->m_passiveSkillList.GetLevelString(level);
}

#ifdef NEW_GUILD_POINT_RANKING_NOTICE
void ConnGuildPointRankerReqMsg( CNetMsg& msg, int guildindex, int charindex )
{
	msg.Init( MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_GUILD_POINT_RANKER_REQ
		<< guildindex
		<< charindex;
}

void GuildPointRankingMsg( CNetMsg& msg, int nCharindex, int nGuildindex, int nRanking )
{
	msg.Init(MSG_GUILD);
	msg << (unsigned char)MSG_NEW_GUILD_POINT_RANKING
		<< nCharindex
		<< nGuildindex
		<< nRanking;
}
#endif //NEW_GUILD_POINT_RANKING_NOTICE

#endif // NEW_GUILD

#ifdef DRATAN_CASTLE
void HelperInsertGuardMsg(CNetMsg & msg, CPC * ch, CItem * item)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_INSERT_GUARD
		<< ch->m_pZone->m_index
		<< ch->m_pos.m_x
		<< ch->m_pos.m_z
		<< ch->m_pos.m_h
		<< item->m_itemProto->m_index;
}

void CastleCristalRespondStartMsg(CNetMsg & msg, CPC * ch)
{
	msg.Init(MSG_EXTEND);
	msg	<< MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_CRISTAL_RESPOND_START
		<< ch->m_index;
}

void CastleCristalRespondEndMsg(CNetMsg & msg, CPC * ch)
{
	msg.Init(MSG_EXTEND);
	msg	<< MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_CRISTAL_RESPOND_END
		<< ch->m_index
		<< ch->GetName();
}

void CastleCristalRespondFailMsg(CNetMsg & msg, CPC * ch)
{
	msg.Init(MSG_EXTEND);
	msg	<< MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_CRISTAL_RESPOND_FAIL
		<< ch->m_index;
}

void CastleTowerListMsg(CNetMsg & msg, CDratanCastle * pCastle)
{
	
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_TOWER_CONTRAL_LIST;
	for(int i=0; i<7; i++)
	{
		msg << pCastle->GetTowerStatus(i);
	}
	
}

void HelperCastleTowerAddMsg(CNetMsg & msg, int zoneidx, CDratanCastle * pCastle)
{
	
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_CASTLE_TOWER_ADD
		<< zoneidx;
	for(int i=0; i<7; i++)
	{
		msg << pCastle->GetTowerStatus(i);
	}
	
}

void CastleTowerAddMsg(CNetMsg & msg, CDratanCastle * pCastle)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_TOWER_CONTRAL;	
}

void CastleTowerReinforceListMsg(CNetMsg & msg, char type, CDratanCastle * pCastle)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_TOWER_REINFORCE_LIST
		<< type
		<< pCastle->GetReinforceStep((int)type);
}


void CastleTowerReinforceMsg(CNetMsg & msg, char type, CDratanCastle * pCastle)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_TOWER_REINFORCE
		<< type
		<< pCastle->GetReinforceStep((int)type);
}

void HelperCastleReinforceMsg(CNetMsg & msg, char type, char step, int zoneidx)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_CASTLE_TOWER_REINFORCE
		<< zoneidx
		<< type
		<< step;
}

void CastleTowerRepaireListMsg(CNetMsg & msg, int idx, LONGLONG money)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_TOWER_REPAIRE_LIST	
		<< idx
		<< money;
}

void CastleTowerRepaireMsg(CNetMsg & msg, int idx, LONGLONG money)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_TOWER_REPAIRE
		<< idx
		<< money;
}

void CastleTowerWarpListMsg(CNetMsg & msg, int * aliveindex, char total)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_TOWER_WARP_LIST
		<< total;
	for(int i = 0; i < total; i++)
	{
		msg << aliveindex[i];
	}
}

void CastleTowerRebrithInstallMsg(CNetMsg & msg, int qindex, int gindex, const char * gname, int list_index)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_QUARTERS_INSTALL
		<< qindex
		<< gindex
		<< gname
		<< list_index;
}

void HeplerCastleRebrithInstallMsg(CNetMsg & msg, int qindex, int gindex, const char * gname)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_CASTLE_QUARTERS_INSTALL
		<< qindex
		<< gindex
		<< gname;
}

void CastleTowerQuartersCrushMsg(CNetMsg & msg, int index)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_QUARTERS_CRUSH
		<< index;
}

void CastletowerQuartersListMsg(CNetMsg & msg, CDratanCastle * pCastle)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_QUARTERS_LIST;
	for(int i=0; i<7; i++)
	{
		if(pCastle->m_pRebrithNPC[i] == NULL)
		{
			msg << -1 << -1 << "";
		}
		else
		{
			msg << pCastle->m_pRebrithNPC[i]->m_proto->m_index
				<< pCastle->m_nRebrithGuild[i]
				<< pCastle->m_strRebrithGuild[i];
		}
	}
}

void WaitTimeMsg(CNetMsg & msg, int wait_time)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_WAIT_TIME
		<< wait_time;
}


void CastleErrorMsg(CNetMsg & msg, MSG_EX_CASTLE_ERROR_TYPE error)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_EX_CASTLE_ERROR
		<< (unsigned char)error;
}

void CastleErrorMsg(CNetMsg & msg, unsigned char error)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_EX_CASTLE_ERROR
		<< (unsigned char)error;
}

#endif // DRATAN_CASTLE


#ifdef EVENT_OPEN_ADULT_SERVER
void HelerOpenAdultServerMsg(CNetMsg& msg, int charindex, MSG_HELPER_OPEN_ADULT_SERVER_ERROR_TYPE error)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_OPEN_ADULT_SERVER
		<< error
		<< charindex;
}
#endif // EVENT_OPEN_ADULT_SERVER

#ifdef GIFT_EVENT_2007
void ConnEventGift2007Msg(CNetMsg & msg, MSG_CONN_EVENT_GIFT_2007_TYPE subtype, CPC * pc)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_GIFT_2007
		<< (unsigned char)subtype
		<< pc->m_desc->m_index
		<< pc->m_index;
}

void GiftEvent2007Msg(CNetMsg & msg, MSG_CONN_EVENT_GIFT_2007_TYPE nError)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_GIFT_2007
		<< (unsigned char)nError;
}
#endif // GIFT_EVENT_2007

#if defined(GIFT_EVENT_2007) || defined(EVENT_OPEN_ADULT_SERVER) || defined( EVENT_MAGIC_CARD )
void EventOpenAdultServerMsg(CNetMsg& msg, MSG_EVENT_OPEN_ADULT_SERVER_TYPE type)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_OPEN_ADULT_SERVER
		<< type;
}
#endif //  GIFT_EVENT_2007 || EVENT_OPEN_ADULT_SERVER || EVENT_MAGIC_CARD


#ifdef PET_DIFFERENTIATION_ITEM
void HelperPetColorChange( CNetMsg& msg, int charindex, int petindex, char petCorlorType )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_PET_COLOR_CHANGE
		<< charindex
		<< petindex
		<< petCorlorType;
}

void PetChangeColor( CNetMsg& msg, int petindex, char petColorType, int ownerindex, char petMount )
{
	msg.Init( MSG_EXTEND );
	msg << MSG_EX_PET_CHANGE_COLOR		
		<< petindex
		<< petColorType
		<< ownerindex
		<< petMount;
}
#endif // PET_DIFFERENTIATION_ITEM

#ifdef EVENT_DROPITEM
void MsgrEventDropItemReqMsg( CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char type, int npcidx, int itemidx, int prob, int thisServer, int thisSub)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_EVENT_DROPITEM_REQ
		<< charindex
		<< type
		<< npcidx
		<< itemidx
		<< prob
		<< thisServer
		<< thisSub;
}

void MsgrEventDropItemRepMsg( CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char type, int npcidx, int itemidx, int prob, int thisServer, int thisSub)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_EVENT_DROPITEM_REP
		<< charindex
		<< type
		<< npcidx
		<< itemidx
		<< prob
		<< thisServer
		<< thisSub;
}
#endif // EVENT_DROPITEM

#ifdef EVENT_BJMONO_2007
void EventBjMono2007Msg(CNetMsg & msg, MSG_EVENT_BJMONO_2007_TYPE subtype)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_BJMONO_2007
		<< (unsigned char)subtype;
}

void ConnEventBjMono2007Msg(CNetMsg& msg, MSG_CONN_EVENT_BJMONO_2007_TYPE subtype, CPC* pc )
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_BJMONO_2007
		<< (unsigned char) subtype
		<< pc->m_desc->m_index
		<< pc->m_index;
}
#endif // EVENT_BJMONO_2007

#ifdef EVENT_SSHOT_2007
void EventSshot2007Msg(CNetMsg & msg, MSG_EVENT_SSHOT_2007_TYPE subtype)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_SSHOT_2007
		<< (unsigned char)subtype;
}

void ConnEventSshot2007Msg(CNetMsg& msg, MSG_CONN_EVENT_SSHOT_2007_TYPE subtype, CPC* pc )
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_EVENT_SSHOT_2007
		<< (unsigned char) subtype
		<< pc->m_desc->m_index
		<< pc->m_index;
}
#endif // EVENT_SSHOT_2007

#ifdef EVENT_LC_1000DAY
void EventLC1000DayMsg( CNetMsg & msg, MSG_EVENT_LC_1000DAY_TYPE type )
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char) MSG_EVENT_LC_1000DAY
		<< (unsigned char)type;
}
#endif // EVENT_LC_1000DAY


#ifdef EVENT_RICHYEAR_2007
void EventRichYearMsg(CNetMsg& msg, MSG_EVENT_RICHYEAR_TYPE type)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_RICHYEAR
		<< (unsigned char)type;
}
#endif // EVENT_RICHYEAR_2007


#ifdef REWARD_IDC2007
void HelerRewardIDC2007Msg(CNetMsg& msg, int userindex)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_REWARD_IDC2007_REQ
		<< userindex;
}
#endif // REWARD_IDC2007

//#ifdef EVENT_HALLOWEEN_2007
void EventHalloween2007Msg( CNetMsg & msg, MSG_EVENT_HALLOWEEN_2007_TYPE type, unsigned char error )
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_HALLOWEEN_2007
		<< (unsigned char)type
		<< error;
}

void EventHalloween2007Msg( CNetMsg & msg, MSG_EVENT_HALLOWEEN_2007_TYPE type )
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_HALLOWEEN_2007
		<< (unsigned char)type;
}

void HelperHalloween2007Msg( CNetMsg& msg, int char_index )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_HALLOWEEN_2007
		<< char_index;
}
//#endif // EVENT_HALLOWEEN_2007

#ifdef EVENT_NEW_MOONSTONE
void EventNewMoonStoneMsg( CNetMsg& msg, MSG_EVENT_NEW_MOONSTONE_TYPE type)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_NEW_MOONSTONE
		<< (unsigned char)type;
}
#endif // EVENT_NEW_MOONSTONE


#ifdef ENABLE_WAR_CASTLE
void CastleOwnerInfoMsg(CNetMsg & msg)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CASTLE_WAR
		<< (unsigned char)MSG_CASTLE_OWNER_INFO;

	int index = 0;
#ifdef DRATAN_CASTLE
	if( CWarCastle::GetCastleObject(ZONE_DRATAN) != 0 )
	{
		index = CWarCastle::GetCastleObject(ZONE_DRATAN)->GetOwnerGuildIndex();
	}
	msg << index;
#endif // DRATAN_CASTLE

	if( CWarCastle::GetCastleObject(ZONE_MERAC) != 0 )
	{
		index = CWarCastle::GetCastleObject(ZONE_MERAC)->GetOwnerGuildIndex();
	}
	msg << index;
}
#endif // ENABLE_WAR_CASTLE

#ifdef DRATAN_CASTLE
#ifdef DYNAMIC_DUNGEON
void DVDInfoMsg(CPC * pPC, CNetMsg & msg, int nMobRate, int nEnvRate )
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_DVD
		<< (unsigned char)MSG_DUNGEON_INFO
		<< nMobRate
		<< nEnvRate ;
}
void DVDManagementMsg( CNetMsg& msg, MSG_DVD_INFO_SUBTYPE subtype )
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_DVD
		<< (unsigned char) MSG_MANAGEMENT
		<< (unsigned char) subtype;
}

void DVDDungeonEnterMsg( CNetMsg& msg, MSG_DVD_INFO_SUBTYPE subtype )
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_DVD
		<< (unsigned char) MSG_DUNGEON_ENTER
		<< (unsigned char) subtype;
}

void HelperDVDRateChangeMsg(CNetMsg & msg, MSG_DVD_INFO_SUBTYPE subtype, int nRate)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_DVD_RATE_CHANGE
		<< (unsigned char) subtype
		<< nRate;
}

void GuildNameColorStateMsg( CNetMsg & msg , CPC* ch)
{
	unsigned char GuildNameColor = 0;
	if( ch->m_guildInfo != NULL )
	{
		if( CWarCastle::GetCastleObject(ZONE_MERAC) != 0 )
		{
			if( ch->m_guildInfo->guild()->index() == CWarCastle::GetCastleObject(ZONE_MERAC)->GetOwnerGuildIndex() )
			{
				GuildNameColor = 1;
			}
		}
		if( CWarCastle::GetCastleObject(ZONE_DRATAN) != 0 )
		{
			if( ch->m_guildInfo->guild()->index() == CWarCastle::GetCastleObject(ZONE_DRATAN)->GetOwnerGuildIndex() )
			{
				GuildNameColor = 2;
			}
		}
	}

	msg.Init(MSG_EXTEND);
	msg << MSG_EX_DVD
		<< (unsigned char) MSG_GUILD_NAME_COLOR
		<< ch->m_index
		<< GuildNameColor;
}
#endif  //DYNAMIC_DUNGEON
#endif // DRATAN_CASTLE


#ifdef SAVE_MOONSTONE_MIX
void HelperSaveMoonStoneMix(CNetMsg& msg, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_SAVE_MOONSTONE_MIX
		<< charindex;
}
#endif // SAVE_MOONSTONE_MIX

#ifdef EVENT_XMAS_2007
void ConnEventXmas2007Msg( CNetMsg& msg, MSG_CONN_EVENT_XMAS_2007_TYPE subtype )
{
	msg.Init( MSG_CONN_REQ );
	msg << (unsigned char) MSG_CONN_EVENT_XMAS_2007
		<< (unsigned char) subtype;
}

void EventXmas2007Msg(CNetMsg& msg, MSG_EVENT_XMAS_2007_TYPE subtype )
{
	msg.Init( MSG_EVENT );
	msg << (unsigned char)MSG_EVENT_XMAS_2007
		<< (unsigned char)subtype;
}
#endif //EVENT_XMAS_2007


#ifdef ALTERNATE_MERCHANT
void AlternateMerchantErrorMsg(CNetMsg& msg, MSG_EX_ALTERNATE_MERCHANT_ERROR_TYPE errcode)
{
	msg.Init( MSG_EXTEND );
	msg << MSG_EX_ALTERNATE_MERCHANT
		<< (unsigned char)MSG_ALTERNATEMERCHANT_ERROR
		<< (unsigned char)errcode;
}

void AlternateMerchantSellStartMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init( MSG_EXTEND );
	msg << MSG_EX_ALTERNATE_MERCHANT
		<< (unsigned char)MSG_ALTERNATEMERCHANT_SELL_START
		<< ch->m_pAlternateMerchant->m_nlistIdx
		<< ch->m_pAlternateMerchant->GetShopType()
		<< ch->m_pAlternateMerchant->GetShopName();
}

void AlternateMerchantSellListMsg(CNetMsg& msg, CPC* ch)
{
	CAlternateMerchant* ps = ch->m_pAlternateMerchant;
//	CInventory& inven = ch->m_invenNormal;

	if (ps == NULL)
	{
		msg.Init();
		return ;
	}

	msg.Init(MSG_EXTEND);
	msg << MSG_EX_ALTERNATE_MERCHANT
		<< (unsigned char)MSG_ALTERNATEMERCHANT_SELL_LIST
		<< ch->m_index
		<< ps->GetShopType();
	char normalcount = ps->GetNormalCount();
	msg << normalcount;
//	if (normalcount)
//	{
//		int i;
//		int pos = -1;
//		for (i = 0; i < normalcount; i++)
//		{
//			pos = ps->GetNextNormalItem(pos);
//			if (pos == -1)
//			{
//				msg.Init();
//				return ;
//			}
//			int itemindex = ps->GetNormalItemIndex(pos);
//			int r, c;
////			if (!inven.FindItem(&r, &c, itemindex))
//			{
//				msg.Init();
//				return ;
//			}
////			CItem* pitem = inven.GetItem(r, c);
////			if (!pitem)
//			{
//				msg.Init();
//				return ;
//			}
//			LONGLONG count = ps->GetNormalItemCount(pos);
//			LONGLONG price = ps->GetNormalItemPrice(pos);
//			msg << itemindex
//				<< pitem->m_idNum
//				<< pitem->m_plus
//				<< pitem->m_flag
//				<< (char)pitem->m_nOption;
//			int j;
//			for (j = 0; j < pitem->m_nOption; j++)
//			{
//				msg << (char)pitem->m_option[j].m_type
//					<< (char)pitem->m_option[j].m_level;
//			}
//			msg << count
//				<< price;
//		}
//	}
//
//	if (ps->GetType() & PST_PACKAGE)
//	{
//		char itemcount = ps->GetPackageCount();
//		msg << ps->GetPackagePrice()
//			<< itemcount;
//		int i;
//		int pos = -1;
//		for (i = 0; i < itemcount; i++)
//		{
//			pos = ps->GetNextPackageItem(pos);
//			if (pos == -1)
//			{
//				msg.Init();
//				return ;
//			}
//			int itemindex = ps->GetPackageItemIndex(pos);
//			int r, c;
//			if (!inven.FindItem(&r, &c, itemindex))
//			{
//				msg.Init();
//				return ;
//			}
//			CItem* pitem = inven.GetItem(r, c);
//			if (!pitem)
//			{
//				msg.Init();
//				return ;
//			}
//			LONGLONG count = ps->GetPackageItemCount(pos);
//			msg << itemindex
//				<< pitem->m_idNum
//				<< pitem->m_plus
//				<< pitem->m_flag
//				<< (char)pitem->m_nOption;
//			int j;
//			for (j = 0; j < pitem->m_nOption; j++)
//			{
//				msg << (char)pitem->m_option[j].m_type
//					<< (char)pitem->m_option[j].m_level;
//			}
//			msg << count;
//		}
//	}
}

void AlternateMerchantChangeMsg(CNetMsg& msg, CPC* ch)
{
	msg.Init( MSG_EXTEND );
	msg << MSG_EX_ALTERNATE_MERCHANT
		<< (unsigned char)MSG_ALTERNATEMERCHANT_CHANGE
		<< ch->m_pAlternateMerchant->m_nlistIdx;
	if (ch->m_pAlternateMerchant)
		msg << ch->m_pAlternateMerchant->GetShopType();
	else
		msg << (char)PST_NOSHOP;
}

void AlternateMerchantBuyMsg(CNetMsg& msg, CNetMsg& buymsg, CPC* tch)
{
	msg.Init(MSG_EXTEND);	
	msg << MSG_EX_ALTERNATE_MERCHANT
		<< (unsigned char)MSG_ALTERNATEMERCHANT_BUY
		<< tch->m_index;
}


void HelperAlternateMerchantStartMsg( CNetMsg& msg, int charindex, char shoptype, CAlternateMerchant* pAlternateMerchant )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_ALTERNATEMERCHANT_START;

	int normalItemCount = 0; //pAlternateMerchant->GetNormalItemCount();
	if( normalItemCount != 0 )
	{
		msg << normalItemCount;
		int i = 0,j = -1;
		for( i; i < normalItemCount; i++ )
		{
			j = pAlternateMerchant->GetNextNormalItem( i );

			
		}
	}
}

void HelperAlternateMerchantBuyMsg( CNetMsg& msg, int charindex, char shoptype, CAlternateMerchant* pAlternateMerchant )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_ALTERNATEMERCHANT_BUY;
}

void HelperAlternateMerchantProductRecovery( CNetMsg& msg, int charindex, char shoptype, CAlternateMerchant* pAlternateMerchant )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_ALTERNATEMERCHANT_PRODUCT_RECOVERY;
}

void HelperAlternateMerchantProductNas( CNetMsg& msg, int charindex, char shoptype, CAlternateMerchant* pAlternateMerchant )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_ALTERNATEMERCHANT_NAS_RECOVERY;
}

void HelperAlternateMerchantEndMsg( CNetMsg& msg, int charindex, char shoptype, CAlternateMerchant* pAlternateMerchant )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_ALTERNATEMERCHANT_END;
}



#endif // ALTERNATE_MERCHANT

#ifdef PET_TURNTO_NPC
void HelperPetTurnToNPCMsg( CNetMsg& msg, int charindex, int petindex, int toNpc, int size )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_PET_TURNTO_NPC
		<< charindex
		<< petindex
		<< toNpc
		<< size;
}

void PetTurnToNPCMsg( CNetMsg& msg, int petindex, int toNpc, int ownerindex, int size )
{
	msg.Init( MSG_EXTEND );
	msg << MSG_EX_PET_TURNTO_NPC		
		<< petindex
		<< toNpc
		<< ownerindex
		<< size;
}
#endif // PET_TURNTO_NPC

#ifdef NEWYEAR_EVENT_2008
void NewYearSkillReqMsg(CNetMsg & msg, CPC * ch)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_NEWYEAR_2008
		<< ch->m_index
		<< ch->GetName();
}
#endif // NEWYEAR_EVENT_2008

void MsgrSetExpLimitMsg( CNetMsg& msg, int server,  int charindex, int nLimit )
{
	msg.Init(MSG_MSGR_REQ);
	msg << 0
		<< server
		<< -1
		<< -1
		<< (unsigned char) MSG_MSGR_SET_EXPLIMIT
		<< charindex
		<< nLimit;
}


#ifdef MONSTER_COMBO
void MonsterComboErrorMsg(CNetMsg & msg, MSG_EX_MONSTERCOMBO_ERROR_TYPE errtype)
{
	msg.Init(MSG_EXTEND );
	msg << MSG_EX_MONSTERCOMBO
		<< (unsigned char)MSG_EX_MONSTERCOMBO_ERROR
		<< (unsigned char)errtype;
}

void MonsterComboMsg(CNetMsg & msg, MSG_EX_MONSTERCOMBO_TYPE subtype)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_MONSTERCOMBO
		<< (unsigned char)subtype;
}

void HelperComboGotoWaitRoomPrompt(CNetMsg& msg, int bossindex, const char* bossname)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< gserver.m_subno
		<< (int)-1
		<< (unsigned char)MSG_HELPER_COMBO_GOTO_COMBO_PROMPT
		<< bossindex
		<< bossname;
}

void ComboGotoWaitRoomPrompt(CNetMsg& msg, int bossindex, int nas)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_MONSTERCOMBO
		<< (unsigned char)MSG_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT
		<< bossindex
		<< nas;
}

void HelperRecallToWaitRoomPrompt(CNetMsg& msg, int bossindex)
{
	msg.Init(MSG_HELPER_REQ);
	msg << -1
		<< gserver.m_serverno
		<< gserver.m_subno
		<< (int)-1
		<< (unsigned char)MSG_HELPER_COMBO_RECALL_TO_COMBO_PROMPT
		<< bossindex;
}

void HelperCreateComboMsg(CNetMsg& msg, int subserver, int charindex, int areaindex, int comboindex)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_CREATE_COMBO
		<< subserver
		<< charindex
		<< areaindex
		<< comboindex;
}
#endif // MONSTER_COMBO

#if defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)
void EventSakuraEvent2008Msg( CNetMsg & msg, MSG_EVNET_SAKURA_2008_TYPE type )
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_SAKURA_2008
		<< (unsigned char)type;
}
#endif //defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)


#ifdef UPGRADE_EVENT
void MsgrUpgradeEventReqMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char cmd, int prob)
{
	msg.Init(MSG_MSGR_REQ);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_UPGRADE_EVENT_REQ
		<< charindex
		<< cmd
		<< prob;
}

void MsgrUpgradeEventRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int charindex, char cmd)
{
	msg.Init(MSG_MSGR_REP);
	msg << seq
		<< server
		<< subno
		<< zone
		<< (unsigned char) MSG_MSGR_UPGRADE_EVENT_REP
		<< charindex
		<< gserver.m_serverno
		<< gserver.m_subno
		<< cmd
		<< gserver.m_nUpgradeProb;
}

#endif // UPGRADE_EVENT

#ifdef CLIENT_RESTART
void ConnClientRestartMsg( CNetMsg& msg , const char* userid)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_CLIENT_RESTART
		<< userid ;
}
#endif // CLIENT_RESTART

#ifdef CHAOSBALL
void ChaosBallMsg(CNetMsg & msg, MSG_EX_CHAOSBALL_ERRORTYPE error)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_CHAOSBALL
		<< (unsigned char)error;
}
#endif

#ifdef NEW_CASHITEM_USA_2008
void ItemWarpDoc(CNetMsg& msg, int zone, int npcidx)
{
	msg.Init(MSG_ITEM);
	msg << (unsigned char)MSG_ITEM_USE_WARPDOC
	    << zone << npcidx;
}
#endif // NEW_CASHITEM_USA_2008

#ifdef ATTENDANCE_EVENT_REWARD
void ConnEventAttendanceRewardMsg( CNetMsg& msg, MSG_CONN_EVENT_ATTENDANCE_EVENT_TYPE subtype, int userIndex, int charIndex )
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_ATTENDANCE_COUNT
		<< (unsigned char) subtype
	    << userIndex << charIndex;
}
#endif //ATTENDANCE_EVENT_REWARD


#ifdef REWARD_IDC2007
void HelperPressCorpsMsg(CNetMsg& msg, MSG_HELPER_PRESSCORPS_TYPE subtype int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_PRESSCORPS
		<< (unsigned char)subtype
		<< charindex;
}
#endif // REWARD_IDC2007


#ifdef IRIS_POINT
void ConnIPointAddMsg( CNetMsg& msg, int userIndex, int charIndex, MSG_CONN_IPOINT_TYPE type, int nIpoint )
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_ADD_IPOINT
		<< userIndex
		<< charIndex
		<< (int)type
		<< nIpoint;
}
#endif //IRIS_POINT

#ifdef EVENT_NOM_MOVIE
void ConnEventNomMsg(CNetMsg& msg, MSG_CONN_EVENT_NOM_TYPE eSubType, int nUserIdx, int nCharIdx)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_EVENT_NOM
		<< (unsigned char) eSubType
		<< nUserIdx
		<< nCharIdx;
}

void EventNomMsg(CNetMsg & msg, MSG_EVENT_NOM_ERROR_TYPE eError)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_NOM
		<< (unsigned char)eError;
}
#endif

#ifdef REQUITAL_EVENT	// 보상 이벤트 
void ConnEventRequitalMsg(CNetMsg & msg, int nUserIdx, int nCharIdx, int nCheck_eventType, CLCString User_name, CLCString Char_Name)
{
	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char) MSG_CONN_EVENT_REQUITAL
		<< nUserIdx
		<< nCharIdx
		<< nCheck_eventType
		<< User_name
		<< Char_Name;
}

void EventRequitalMsg(CNetMsg & msg, MSG_EVENT_REQUITAL_ERROR_TYPE eError)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char)MSG_EVENT_REQUITAL
		<< (unsigned char)eError;
}
#endif // REQUITAL_EVENT

#ifdef ATTACK_PET
void HelperAttackPetMsg( CNetMsg& msg , unsigned char subtype, int ownerIndex )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_APET
		<< subtype
		<< ownerIndex;
}
#endif // ATTACK_PET


#ifdef EVENT_PHOENIX  // yhj   081105  피닉스
void HelperEventPhoenixMsg(CNetMsg & msg, int nUserIdx, int nCharIdx  )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EVENT_PHOENIX
		<< nUserIdx
		<< nCharIdx;
}

void EventPhoenixMsg(CNetMsg & msg, MSG_EVENT_PHOENIX_ERROR_TYPE eError)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char) MSG_EVENT_PHOENIX
		<< (unsigned char) eError;
}

void EventPhoenix_SuccessMsg(CNetMsg & msg)
{
	msg.Init(MSG_EVENT);
	msg << (unsigned char) MSG_EVENT_PHOENIX_SUCCESS;
}
#endif // EVENT_PHOENIX /////////////////////////

#ifdef EXTREME_CUBE
void HelperCubeStateReqMsg( CNetMsg& msg, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_CUBESTATE_REQ
		<< charindex;
}

void HelperCubeStatePersonalReqMsg( CNetMsg& msg, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_CUBESTATE_PERSONAL_REQ
		<< charindex;
}


void HelperAddCubePointMsg(CNetMsg& msg, int ofguildindex, int ofcubepoint, long lastcubepoint)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_ADDCUBEPOINT
		<< ofguildindex
		<< ofcubepoint
		<< lastcubepoint;
}

void HelperAddCubePointPersonalMsg(CNetMsg& msg, int ofcharidx ,int ofcubepoint, long lastCubePoint)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_ADDCUBEPOINT_PERSONAL
		<< ofcharidx
		<< ofcubepoint
		<< lastCubePoint;
}

void HelperGuildCubeNoticeMsg(CNetMsg& msg, int type)
{
	msg.Init(MSG_HELPER_REQ);
	msg	<< -1
		<< gserver.m_serverno
		<< -1
		<< -1
		<< (unsigned char) MSG_HELPER_GUILDCUBETIME_NOTICE
		<< type;
}

void ExtremeCubeErrorMsg(CNetMsg& msg, MSG_EX_EXTREME_CUBE_ERROR_TYPE errortype)
{
	msg.Init( MSG_EXTEND );
	msg << MSG_EX_EXTREME_CUBE
		<< (unsigned char)MSG_EX_EXTREME_CUBE_ERROR
		<< (unsigned char)errortype;
}

void ExtremeCubeMsg(CNetMsg& msg, MSG_EX_EXTREME_CUBE_TYPE type)
{
	msg.Init( MSG_EXTEND );
	msg << MSG_EX_EXTREME_CUBE
		<< (unsigned char ) type;
}
#endif // EXTREME_CUBE


//#ifdef INIT_SSKILL
void InitSSkillMsg(CNetMsg& msg, MSG_EX_INIT_SSkill_ERROR_TYPE type)
{
	msg.Init( MSG_EXTEND );
	msg << MSG_EX_INIT_SSKILL
		<< (char)type;
}
//#endif // INIT_SSKILL

#ifdef TRADE_AGENT		// 거래 대행
// 등록 리스트 
void HelperTradeAgentRegListReqMsg(CNetMsg& msg, int charindex,int page_no,int nAlignType)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TRADEAGENT_REG_LIST_REQ
		<< charindex
		<< page_no
		<< nAlignType;
}

// 등록 
void HelperTradeAgentRegReqMsg(CNetMsg& msg, int charindex, CLCString sellchar_name, char tab, char row, char col, int item_serial, CLCString item_serial2, CItem * pitem, CLCString item_name,int item_level,int item_type,int item_subtype,int item_class,int Quantity,LONGLONG TotalMoney,LONGLONG Guaranty,LONGLONG llCurCharHaveNas)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TRADEAGENT_REG_REQ
		<< charindex
		<< sellchar_name
		<< tab << row << col
		<< item_serial
		<< item_serial2
		<< pitem->m_plus
		<< pitem->m_flag
		<< pitem->m_nOption;

		for (int i = 0; i < pitem->m_nOption; i++)
		{
			msg << pitem->m_option[i].m_type
				<< pitem->m_option[i].m_level;
		}

	msg << item_name
		<< item_level
		<< item_type
		<< item_subtype
		<< item_class
		<< Quantity
		<< TotalMoney
		<< Guaranty
		<< llCurCharHaveNas;
}

//등록 처리 결과
void HelperTradeAgentRegResultMsg(CNetMsg& msg, int charindex,int nDbIndex,int nResult)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TRADEAGENT_REGRESULT
		<< charindex	
		<< nDbIndex
		<< nResult;
}

// 등록 취소
void HelperTradeAgentRegCancelReqMsg(CNetMsg& msg, int charindex,int nCancelindex)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TRADEAGENT_REG_CANCEL_REQ
		<< charindex
		<< nCancelindex;

}

// 조회
void HelperTradeAgentSearchReqMsg(CNetMsg& msg, int charindex,int nPageNo, int nitemType,int nitemSubType,int nJobClass,CLCString stSearchWord,int nAlignType)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TRADEAGENT_SEARCH_REQ
		<< charindex
		<< nitemType
		<< nitemSubType
		<< nJobClass
		<< stSearchWord
		<< nPageNo
		<< nAlignType;
}

// 정산 리스트 
void HelperTradeAgentCalcListReqMsg(CNetMsg& msg, int charindex,int page_no,int nAlignType)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TRADEAGENT_CALCLIST_REQ
		<< charindex
		<< page_no
		<< nAlignType;
}

//정산 요청
void HelperTradeAgentCalculateReqMsg(CNetMsg& msg, int charindex,int PageNo)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TRADEAGENT_CALCULATE_REQ
		<< charindex
		<< PageNo;	
}

//정산 처리 결과
void HelperTradeAgentCalcResultMsg(CNetMsg& msg, int charindex,int nItemcount,int* pResultindex,int* pResult)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TRADEAGENT_CALCRESULT
		<< charindex	
		<< nItemcount;

	for(int i=0; i < nItemcount; i++)
	{
		msg << pResultindex[i]		
			<< pResult[i];		
	}
}

// 체크 정산 필요 
void HelperTradeAgentCheckCalcReqMsg(CNetMsg& msg, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TRADEAGENT_CHECKCALC_REQ
		<< charindex;	
}

// 구매
void HelperTradeAgentBuyReqMsg(CNetMsg& msg, int charindex, CLCString buychar_name, int nList_idx,LONGLONG llCharHaveNas)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_TRADEAGENT_BUY_REQ
		<< charindex
		<< buychar_name
		<< nList_idx
		<< llCharHaveNas;
}


// (GameServer => Client) 에러 메세지
void TradeAgentErrorMsg(CNetMsg & msg, unsigned char errorType,char errorPart)
{
	msg.Init(MSG_TRADEAGENT);
	msg << MSG_TRADEAGENT_ERROR
		<< (unsigned char)errorType
 	    << (unsigned char)errorPart;		
}

// 등록 리스트 정보 
void TradeAgentRegListRepMsg(CNetMsg & msg,int nCharIndex, int nCharRegTotCount, int nMaxPageNo, int nPageNo, int nItemCount,int* pListindex, int* pItemSerial,int* pItemPlus,int* pItemFlag,S_ITEMOPTION* pItemOption,int* pQuantity,LONGLONG* ptotalmoney,int* pFinishDay,int* pFinishDayUnit)
{
	msg.Init( MSG_TRADEAGENT );
	msg << MSG_TRADEAGENT_REG_LIST_REP
		<< nCharIndex
		<< nCharRegTotCount
		<< nMaxPageNo
		<< nPageNo
		<< nItemCount;

	for( int i = 0; i < nItemCount; i++ )
	{
		int nItemPlus = 0;
#ifdef SET_ITEM
		int nItemPlus2 = 0;
		nItemPlus = (pItemPlus[i] & 0x0000ffff);
		nItemPlus2 = ((pItemPlus[i] >> 16) & 0x0000ffff);
#else
		nItemPlus = pItemPlus[i];
#endif
		
		msg << pListindex[i]			// List Index
			<< pItemSerial[i]			// 아이템 Index
			<< nItemPlus
			<< pItemFlag[i];
#ifdef SET_ITEM
		CItemProto* pItem = gserver.m_itemProtoList.FindIndex(pItemSerial[i]);
		if (pItem && pItem->IsOriginItem())
		{
			msg << pItem->GetOriginOptionCnt();
			for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
			{
				if (pItem->GetOriginIndex(i) > -1)
				{
					msg << (char)pItem->GetOriginIndex(i)
						<< (char)pItem->GetOriginLevel(i);
				}
			}
		}
		else
#endif
		{
			msg << pItemOption[i].m_nOptionCnt;
			for(int j=0; j < pItemOption[i].m_nOptionCnt; j++)
			{
				msg << (char)pItemOption[i].m_Option_type[j]
					<< (char)pItemOption[i].m_Option_level[j];
			}
		}
		msg << pQuantity[i]
			<< ptotalmoney[i]
			<< pFinishDay[i]
			<< pFinishDayUnit[i];

#ifdef SET_ITEM
		if (pItem && pItem->IsOriginItem())
		{
			msg << (char)pItem->GetBelongType()
				<< (char)pItem->GetOriginSkillCnt();
			for (i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
			{
				if (pItem->GetOriginIndex(i) > -1)
				{
					msg << pItem->GetOriginIndex(i)
						<< (char)pItem->GetOriginLevel(i);
				}
			}
		}
#endif
	}
}

// 검색 정보 
void TradeAgentSearchRepMsg(CNetMsg & msg,int nCharIndex, int nMaxPageNo, int nPageNo, int nItemCount,int* pListindex, int* pItemSerial,int* pItemPlus,int* pItemFlag,S_ITEMOPTION* pItemOption, int* pQuantity,LONGLONG* ptotalmoney,int* pLevel,CLCString *pSellCharname)
{
	msg.Init( MSG_TRADEAGENT );
	msg << MSG_TRADEAGENT_SEARCH_REP
		<< nCharIndex
		<< nMaxPageNo
		<< nPageNo
		<< nItemCount;

	for( int i = 0; i < nItemCount; i++ )
	{
		int nItemPlus = 0;
#ifdef SET_ITEM
		int nItemPlus2 = 0;
		nItemPlus = (pItemPlus[i] & 0x0000ffff);
		nItemPlus2 = ((pItemPlus[i] >> 16) & 0x0000ffff);
#else
		nItemPlus = pItemPlus[i];
#endif

		msg << pListindex[i]
			<< pItemSerial[i]
			<< nItemPlus
			<< pItemFlag[i];
#ifdef SET_ITEM
		CItemProto* pItem = gserver.m_itemProtoList.FindIndex(pItemSerial[i]);
		if (pItem && pItem->IsOriginItem())
		{
			msg << pItem->GetOriginOptionCnt();
			for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
			{
				if (pItem->GetOriginIndex(i) > -1)
				{
					msg << (char)pItem->GetOriginIndex(i)
						<< (char)pItem->GetOriginLevel(i);
				}
			}
		}
		else
#endif
		{
			msg << pItemOption[i].m_nOptionCnt;
			for(int j=0; j < pItemOption[i].m_nOptionCnt; j++)
			{
				msg << (char)pItemOption[i].m_Option_type[j]
					<< (char)pItemOption[i].m_Option_level[j];
			}
			
		}
		
		msg << pQuantity[i]
			<< ptotalmoney[i]
			<< pLevel[i]
			<< pSellCharname[i];
		
#ifdef SET_ITEM
		if (pItem && pItem->IsOriginItem())
		{
			msg << (char)pItem->GetBelongType()
				<< (char)pItem->GetOriginSkillCnt();
			for (i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
			{
				if (pItem->GetOriginIndex(i) > -1)
				{
					msg << pItem->GetOriginIndex(i)
						<< (char)pItem->GetOriginLevel(i);
				}
			}
		}
#endif
	}
}

// 구매 결과 
void TradeAgentBuyRepMsg(CNetMsg & msg,int nCharIndex)
{
	msg.Init( MSG_TRADEAGENT );
	msg << MSG_TRADEAGENT_BUY_REP
		<< nCharIndex;
}

// 등록 결과 정보 
void TradeAgentRegRepMsg(CNetMsg & msg,int nCharIndex, CLCString item_name,int Quantity,LONGLONG TotalMoney,LONGLONG Guaranty)
{
	msg.Init( MSG_TRADEAGENT );
	msg << MSG_TRADEAGENT_REG_REP
		<< nCharIndex
		<< item_name
		<< Quantity
		<< TotalMoney
		<< Guaranty;
}

// 등록 취소 결과 정보 
void TradeAgentRegCancelRepMsg(CNetMsg & msg, int nCharIndex)
{
	msg.Init( MSG_TRADEAGENT );
	msg << MSG_TRADEAGENT_REG_CANCEL_REP
		<< nCharIndex;
}

// 정산 리스트 정보
void TradeAgentCalcListRepMsg(CNetMsg & msg,int nCharIndex, int nMaxPageNo, int nPageNo, int nItemCount,int* pItemSerial,int* pItemPlus,int* pItemFlag,S_ITEMOPTION* pItemOption,int* pQuantity,LONGLONG* ptotalmoney,int* pState, int* pPassDay,CLCString *pCharname)
{
	msg.Init( MSG_TRADEAGENT );
	msg << MSG_TRADEAGENT_CALCLIST_REP
		<< nCharIndex
		<< nMaxPageNo
		<< nPageNo
		<< nItemCount;
	
	for( int i = 0; i < nItemCount; i++ )
	{
		int nItemPlus = 0;
#ifdef SET_ITEM
		int nItemPlus2 = 0;
	///	nItemPlus = (pItemPlus[i] & 0x0000ffff);
	//	nItemPlus2 = ((pItemPlus[i] >> 16) & 0x0000ffff);
#else
		nItemPlus = pItemPlus[i];
#endif

		msg << pItemSerial[i]
			<< nItemPlus
			<< pItemFlag[i];
		
#ifdef SET_ITEM
		CItemProto* pItem = gserver.m_itemProtoList.FindIndex(pItemSerial[i]);
		if (pItem && pItem->IsOriginItem())
		{
			msg << pItem->GetOriginOptionCnt();
			for (int l = 0; l < MAX_ORIGIN_OPTION; l++)
			{
				if (pItem->GetOriginIndex(l) > -1)
				{
					msg << (char)pItem->GetOriginIndex(l)
						<< (char)pItem->GetOriginLevel(l);
				}
			}
		}
		else
#endif
		{
			msg << pItemOption[i].m_nOptionCnt;
			for(int j=0; j < pItemOption[i].m_nOptionCnt; j++)
			{
				msg << (char)pItemOption[i].m_Option_type[j]
					<< (char)pItemOption[i].m_Option_level[j];
			}
		}

		msg << pQuantity[i]
			<< ptotalmoney[i]
			<< pState[i]
			<< pPassDay[i]
			<< pCharname[i];

#ifdef SET_ITEM
		if (pItem && pItem->IsOriginItem())
		{
			msg << (char)pItem->GetBelongType()
				<< (char)pItem->GetOriginSkillCnt();
			for (int l = MAX_ORIGIN_OPTION; l < MAX_ORIGIN; l++)
			{
				if (pItem->GetOriginIndex(l) > -1)
				{
					msg << pItem->GetOriginIndex(l)
						<< (char)pItem->GetOriginLevel(l);
				}
			}
		}
#endif
	}
}

//정산 
void TradeAgentCalculateRepMsg(CNetMsg & msg,int nCharIndex)
{
	msg.Init( MSG_TRADEAGENT );
	msg << MSG_TRADEAGENT_CALCULATE_REP
		<< nCharIndex;
}

//체크 정산
void TradeAgentCheckCalcRepMsg(CNetMsg & msg,int nCharIndex,int nIsBeCalcdata)
{
	msg.Init( MSG_TRADEAGENT );
	msg << MSG_TRADEAGENT_CHECKCALC_REP
		<< nCharIndex
		<< nIsBeCalcdata;
}

//정산 알림
void TradeAgentNeedCalcMotifyMsg(CNetMsg & msg,int nCharIndex,unsigned char ucState,CLCString stItemname,int nItemCount)
{
	msg.Init( MSG_TRADEAGENT );
	msg << MSG_TRADEAGENT_NEEDCALC_NOTIFY
		<< nCharIndex
		<< ucState
		<< stItemname
		<< nItemCount;
}

#endif // TRADE_AGENT

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
void HelperDisconnectHackCharacter(CNetMsg & msg, int nUserIndex, const char* user_id, int nCharIndex, const char* char_id, int nHack_type, float delay)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_DISCONNECT_HACK_CHARACTER
		<< nUserIndex
		<< user_id
		<< nCharIndex
		<< char_id
		<< nHack_type
		<< delay;
}
#endif // DISCONNECT_HACK_CHARACTER 

#ifdef EX_GO_ZONE
void ExGoZoneErrorMsg(CNetMsg& msg, MSG_EX_GO_ZONE_ERROR_TYPE errtype, int npcidx)
{
		msg.Init(MSG_EXTEND);
		msg << MSG_EX_GO_ZONE
			<< (unsigned char)MSG_EX_GO_ZONE_ERROR
			<< errtype
			<< npcidx;
}

void ExGoZoneMoveOKMsg(CNetMsg& msg, MSG_EX_GO_ZONE_ERROR_TYPE errtype, int zone, int extra, int npcIdx)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_GO_ZONE
		<< (unsigned char)MSG_EX_GO_ZONE_ERROR
		<< errtype
		<< zone
		<< extra
		<< npcIdx;
}

void ExGoZoneFieldDungeonMsg(CNetMsg& msg, int zone, int extra, int tax)
{
	msg.Init(MSG_EXTEND);
	msg << MSG_EX_GO_ZONE
		<< (unsigned char)MSG_EX_GO_ZONE_FIELD_DOUNGEON_TAX_REP
		<< zone
		<< extra
		<< tax;
}
#endif // EX_GO_ZONE

#ifdef EXPEDITION_RAID									// 원정대 관련
//에러
void ExpedErrorMsg(CNetMsg & msg, unsigned char errorType)
{
	msg.Init(MSG_EXPEDITION);
	msg << MSG_EXPED_ERROR
		<< (unsigned char)errorType;		
}

void HelperExpedCreateReqMsg(CNetMsg& msg, int nBossIndex,CLCString BossName)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_CREATE_REQ
		<< nBossIndex
		<< BossName;
}

void HelperExpedInviteReqMsg(CNetMsg& msg, int nBossIndex, CLCString strBossName, int nTargetIndex,CLCString strTargetName)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_INVITE_REQ
		<< nBossIndex
		<< strBossName
		<< nTargetIndex
		<< strTargetName;
}

void HelperExpedAllowReqMsg(CNetMsg& msg, int nBossIndex, int nAllowIndex, CLCString strAllowCharName)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_ALLOW_REQ
		<< nBossIndex
		<< nAllowIndex
		<< strAllowCharName;
}

void HelperExpedRejectReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_REJECT_REQ
		<< nBossIndex
		<< nTargetIndex;
}

void HelperExpedQuitReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, int nQuitMode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_REJECT_REQ
		<< nBossIndex
		<< nTargetIndex
		<< nQuitMode;
}

void HelperExpedKickReqMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_KICK_REQ
		<< nBossIndex
		<< nTargetIndex;
}

void HelperExpedChangeBossReqMsg(CNetMsg& msg, int nBossIndex, int nChangeMode, int nNewBossIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_CHANGEBOSS_REQ
		<< nNewBossIndex
		<< nBossIndex;
}

void HelperExpedChangeTypeReqMsg(CNetMsg& msg, int nBossIndex, char ExpedType,char cDiviType)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_CHANGETYPE_REQ
		<< nBossIndex
		<< cDiviType
		<< ExpedType;
}

void HelperExpedEndExpedMsg(CNetMsg& msg, int nBossIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_ENDEXPED_REQ
		<< nBossIndex;
}

void HelperExpedSetMBossReqMsg(CNetMsg& msg, int nBossIndex, int nNewMBossIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_SETMBOSS_REQ
		<< nBossIndex
		<< nNewMBossIndex;
}

void HelperExpedResetMBossReqMsg(CNetMsg& msg, int nBossIndex, int nNewMBossIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_RESETMBOSS_REQ
		<< nBossIndex
		<< nNewMBossIndex;
}

void HelperExpedMoveGroupReqMsg(CNetMsg& msg, int nBossIndex, int nSourceGroup, int nMoveCharIndex, int nTargetGroup, int nTargetListindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_MOVEGROUP_REQ
		<< nBossIndex
		<< nSourceGroup
		<< nMoveCharIndex
		<< nTargetGroup
		<< nTargetListindex;
}

void HelperExpedSetLabelReqMsg(CNetMsg& msg, int nBossIndex, int nType, int nMode, int nLabel,  int nDestIndex)
{
	msg.Init(MSG_EXPEDITION);
	msg << MSG_HELPER_EXPED_SET_LABEL_REQ
		<< nBossIndex
		<< nType
		<< nMode
		<< nLabel
		<< nDestIndex;
}

void HelperExpedRejoinReqMsg(CNetMsg& msg, int nBossIndex, int nJoinIndex)
{
	msg.Init(MSG_EXPEDITION);
	msg << MSG_HELPER_EXPED_REJOIN_REQ
		<< nBossIndex
		<< nJoinIndex;
}

void HelperExpedChatReqMsg(CNetMsg& msg, int nBossIndex, int nCharIdex, CLCString strCharName, int nGroupTyp, CLCString strChat)
{
	msg.Init(MSG_EXPEDITION);
	msg << MSG_HELPER_EXPED_CHAT_REQ
		<< nBossIndex
		<< nCharIdex
		<< strCharName
		<< nGroupTyp
		<< strChat;
}

//파티 전환 응답
void ExpedCreateRepMsg(CNetMsg & msg, char cExpedType1, char cExpedType2, char cExpedType3, int nBossIndex,CLCString BossCharName,int nMemberCount,int *pCharIdex,CLCString* pCharName,int* pGroupType,int* pMemberType,int* pSetLabelType,int* pQuitType)
{
	msg.Init(MSG_EXPEDITION);
	msg << MSG_CREATE_REP
		<< cExpedType1
		<< cExpedType2
		<< cExpedType3
		<< nBossIndex
		<< BossCharName
		<< nMemberCount;

	for(int i=0; i < nMemberCount; i++)
	{
		msg << pCharIdex[i]
			<< pCharName[i]
			<< pGroupType[i]
			<< pMemberType[i]
			<< pSetLabelType[i]
			<< pQuitType[i];
	}		
}

//초대 전달 
void ExpedInviteRepMsg(CNetMsg & msg, char cExpedType1, char cExpedType2, char cExpedType3, int nBossIndex, CLCString strBossName)
{
	msg.Init(MSG_EXPEDITION);
	msg << MSG_INVITE_REP
		<< cExpedType1
		<< cExpedType2
		<< cExpedType3
		<< nBossIndex
		<< strBossName;
}

void ExpedAddMsg(CNetMsg& msg, int nCharIndex, const char* strCharName,int nCharGroup,int nCharMember, int nCharListIndex, CPC* tch, char isboss)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char)MSG_EXPED_ADD
		<< isboss
		<< nCharIndex
		<< strCharName
		<< nCharGroup
		<< nCharMember
		<< nCharListIndex;
	if (tch)
	{
		msg << tch->m_job
#ifdef ENABLE_CHANGEJOB
		<< tch->m_job2
#endif
		<< tch->m_level
		<< tch->m_hp << tch->m_maxHP
		<< tch->m_mp << tch->m_maxMP
		<< GET_X(tch)
		<< GET_Z(tch)
			<< GET_YLAYER(tch)
			<< tch->m_pZone->m_index;
	}
	else
	{
		msg << (char)0
#ifdef ENABLE_CHANGEJOB
			<< (char)0
#endif
			<< (int)1
			<< (int)1 << (int)1
			<< (int)1 << (int)1
			<< (int)0
			<< (int)0
			<< (int)0
			<< (int)0;
	}
}

void ExpedInfoMsg(CNetMsg& msg, int nIndex, int nGroup, int nLevel, int nHP, int nMaxHP, int nMP, int nMaxMP, float x, float z, char nYlayer, int nZone)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char)MSG_EXPED_MEMBER_INFO
		<< nIndex
		<< nGroup
		<< nLevel
		<< nHP
		<< nMaxHP
		<< nMP
		<< nMaxMP
		<< x
		<< z
		<< nYlayer
		<< nZone;		// TODO : PARTY : 메시지 수정
}

void ExpedQuitRepMsg(CNetMsg& msg, int nTargetIndex,int nQuitMode)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char)MSG_QUIT_REP
	    << nTargetIndex
		<< nQuitMode;		// 정상,비정상 구분
}
void ExpedQuitReqMsg(CNetMsg& msg, int nQuitMode)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char)MSG_QUIT_REQ
		<< nQuitMode;		// 정상,비정상 구분
}

void ExpedKickMsg(CNetMsg& msg, int nTargetIndex)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char)MSG_KICK_REP;
	msg << nTargetIndex;	
}

void ExpedMsg(CNetMsg& msg, unsigned char subtype)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char)subtype;
}

void ExpedChangeBossReq(CNetMsg& msg, int nChangeMode, int nNewBossIndex)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char) MSG_CHANGEBOSS_REQ
		<< nChangeMode				// 수동,자동	
		<< nNewBossIndex;			// 자동(-1)
}

void ExpedChangeBoss(CNetMsg& msg, int nBossIndex,  int nNewBossIndex, int nChangeMode)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char) MSG_CHANGEBOSS_REP
		<< nBossIndex
		<< nNewBossIndex
		<< nChangeMode;			// 수동,자동
}

void ExpedChangeType(CNetMsg& msg, char cDiviType, char cExpedType)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char) MSG_CHANGETYPE_REP
		<< cDiviType
		<< cExpedType;
}

void ExpedSetMbossMsg(CNetMsg& msg, int nNewMBossIndex)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char) MSG_SETMBOSS_REP
		<< nNewMBossIndex;

}

void ExpedResetMbossMsg(CNetMsg& msg, int nNewMBossIndex)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char) MSG_RESETMBOSS_REP
		<< nNewMBossIndex;
}

void ExpedMoveGroupRepMsg(CNetMsg& msg,int nSourceGroup, int nMoveCharIndex, int nTargetGroup, int nTargetListindex)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char) MSG_MOVEGROUP_REP
		<< nSourceGroup
		<< nMoveCharIndex
		<< nTargetGroup
		<< nTargetListindex;
}

void ExpedSetLabelRepMsg(CNetMsg& msg,int nType, int nMode, int nLabel,int nDestIndex)
{
	msg.Init(MSG_EXPEDITION);
	msg << (unsigned char) MSG_SET_LABEL_REP
		<< nType
		<< nMode
		<< nLabel
		<< nDestIndex;
}

bool ExpedViewDail_InvenMsg(CNetMsg& msg, CPC* ch)
{
	int nRow,nCol;
	int nItemCount=0;
	CInventory* inven = GET_INVENTORY(ch, INVENTORY_NORMAL);
	if (!inven)
	{
		msg.Init();
		return false;
	}

	for (nRow = 0; nRow < MAX_INVENTORY_ROWS; nRow++)
	{
		if(!inven->m_rows[nRow]) continue;

		CItem* item;
		for (nCol = 0; nCol < ITEMS_PER_ROW; nCol++)
		{
			item = inven->GetItem(nRow, nCol);
			if (item && item->m_wearPos != -1)
			{
				nItemCount++;
			}
		}
	}	

	msg.Init(MSG_EXPEDITION);
	msg << MSG_VIEWDETAIL_INVEN
		<< nItemCount;

	for (nRow = 0; nRow < MAX_INVENTORY_ROWS; nRow++)
	{
		if(!inven->m_rows[nRow]) continue;

		CItem* item;
		for (nCol = 0; nCol < ITEMS_PER_ROW; nCol++)
		{
			item = inven->GetItem(nRow, nCol);
			if (item && item->m_wearPos != -1)
			{
				msg << (int)item->m_index				// item_idx
					<< (int)item->m_idNum				// item_type
					<< item->m_wearPos					// wear_pos
					<< (int)item->m_plus				// plus
					<< (int)item->m_flag				// flag
					<< (int)item->m_used				// used
#ifdef COMPOSITE_TIME				
					<< (int)item->m_used_2				// used2
#endif // COMPOSITE_TIME
					<< (LONGLONG)item->Count();			// count
#ifdef SET_ITEM
				if (item->IsOriginItem())
				{
					msg << (char)item->GetOriginOptionCnt();
					for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
					{
						if (item->GetOriginIndex(i) > -1)
						{
							msg << (char)item->GetOriginIndex(i)
								<< (char)item->GetOriginLevel(i);
						}
					}

					msg << (char)item->GetBelongType()
						<< (char)item->GetOriginSkillCnt();
					for (i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
					{
						if (item->GetOriginIndex(i) > -1)
						{
							msg << item->GetOriginIndex(i)
								<< (char)item->GetOriginLevel(i);
						}
					}
				}
				else
#endif
				{
					msg << (char)item->m_nOption;
					for (int j=0; j < item->m_nOption; j++)
					{
						msg << (char)item->m_option[j].m_type
							<< (char)item->m_option[j].m_level;
					}
				}
			}
		}
	}
	return true;
}	

void ExpedViewDail_ExPetMsg(CNetMsg& msg, CPet* pet)
{
 	int nRemainGetRebirth = pet->GetRemainRebirthTime();

	msg.Init(MSG_EXPEDITION);
	msg << MSG_VIEWDETAIL_PET
		<< pet->m_index
		<< pet->GetPetTypeGrade()
		<< pet->m_level
 		<< pet->m_exp
 		<< pet->GetNeedExp()
 		<< pet->m_hp
 		<< pet->m_maxHP
 		<< pet->GetAbilityPoint()
 		<< pet->GetHungryPoint()
 		<< (int)PET_MAX_HUNGRY
 		<< pet->GetSympathyPoint()
 		<< (int)PET_MAX_SYMPATHY
 		<< nRemainGetRebirth;
#ifdef PET_NAME_CHANGE
	msg << pet->m_name;
#endif // PET_NAME_CHAGNE
}

void ExpedViewDail_ExAPetMsg(CNetMsg& msg, CAPet* pet)
{
	int	nEqAttack = 0;
	if( pet->m_wearing[APET_WEAR_WEAPON] )
	{
		if( pet->m_wearing[APET_WEAR_WEAPON]->IsRangeWeapon() )
		{
			nEqAttack = pet->m_eqRange + pet->m_opRange;
		}
		else
		{
			nEqAttack = pet->m_eqMelee + pet->m_opMelee;
		}
	}

	msg.Init(MSG_EXPEDITION);
	msg << MSG_VIEWDETAIL_APET
		<< (unsigned char) MSG_SUB_STAT
		<< pet->m_index
		<< pet->m_name
		<< pet->m_level
		<< pet->m_str + pet->m_nPlusStr + pet->m_opStr
		<< pet->m_con + pet->m_nPlusCon + pet->m_opCon 
		<< pet->m_dex + pet->m_nPlusDex + pet->m_opDex 
		<< pet->m_int + pet->m_nPlusInt + pet->m_opInt; 
}

#endif //EXPEDITION_RAID


#if defined (CHAR_LOG) && defined (LC_KOR)
void ConnCharLogMsg( CNetMsg& msg, CPC* pc )
{
	if( !pc )
		return;

	msg.Init(MSG_CONN_REQ);
	msg << (unsigned char)MSG_CONN_CHAR_LOG
		<< pc->m_index
		<< pc->m_desc->m_index
		<< pc->m_desc->m_idname
		<< pc->m_name
		<< pc->m_nick;

	if( pc->m_guildInfo && pc->m_guildInfo->guild() )
		msg << pc->m_guildInfo->guild()->name();
	else
		msg << "";

	msg << pc->m_level
		<< pc->m_exp
		<< pc->m_job
		<< pc->m_job2
		<< pc->m_statpt_str
		<< pc->m_statpt_dex
		<< pc->m_statpt_int
		<< pc->m_statpt_con
		<< pc->m_skillPoint;

	if( pc->m_moneyItem )
		msg << pc->m_moneyItem->Count();
	else
		msg << (LONGLONG) 0;

	if( MSG_TEACH_STUDENT_TYPE==pc->m_teachType )
		msg << pc->m_teachIdx[0];
	else
		msg << 0;
}
#endif // #if defined (CHAR_LOG) && defined (LC_KOR)

#ifdef FACTORY_SYSTEM
void FactoryListMsg(CNetMsg& msg, CPC* ch, int nSealType)
{
	int nCount = ch->m_listFactory.GetCount();
	msg.Init(MSG_FACTORY);

	msg << (unsigned char)MSG_FACTORY_ITEM_LIST;
	
	// 정교한
	msg << (char)SEAL_TYPE_DETAIL
		<< ch->m_listFactory.m_listDetail.GetCount();

	void * pos = ch->m_listFactory.m_listDetail.GetHead();
	while (pos)
	{
		CFactoryProto * p = ch->m_listFactory.m_listDetail.GetData(pos);
		pos = ch->m_listFactory.m_listDetail.GetNext(pos);

		msg << p->GetIndex();
	}

	// 화려한
	msg << (char)SEAL_TYPE_COLORFUL
		<< ch->m_listFactory.m_listColorful.GetCount();

	pos = ch->m_listFactory.m_listColorful.GetHead();
	while (pos)
	{
		CFactoryProto * p = ch->m_listFactory.m_listColorful.GetData(pos);
		pos = ch->m_listFactory.m_listColorful.GetNext(pos);

		msg << p->GetIndex();
	}

	// 날카로운
	msg << (char)SEAL_TYPE_COLORFUL
		<< ch->m_listFactory.m_listSharp.GetCount();

	pos = ch->m_listFactory.m_listSharp.GetHead();
	while (pos)
	{
		CFactoryProto * p = ch->m_listFactory.m_listSharp.GetData(pos);
		pos = ch->m_listFactory.m_listSharp.GetNext(pos);

		msg << p->GetIndex();
	}

	// 단단한
	msg << (char)SEAL_TYPE_COLORFUL
		<< ch->m_listFactory.m_listHard.GetCount();

	pos = ch->m_listFactory.m_listHard.GetHead();
	while (pos)
	{
		CFactoryProto * p = ch->m_listFactory.m_listHard.GetData(pos);
		pos = ch->m_listFactory.m_listHard.GetNext(pos);

		msg << p->GetIndex();
	}

	// 소모품
	msg << (char)SEAL_TYPE_COLORFUL
		<< ch->m_listFactory.m_listConsume.GetCount();

	pos = ch->m_listFactory.m_listConsume.GetHead();
	while (pos)
	{
		CFactoryProto * p = ch->m_listFactory.m_listConsume.GetData(pos);
		pos = ch->m_listFactory.m_listConsume.GetNext(pos);

		msg << p->GetIndex();
	}
}

void FactoryLearnMsg(CNetMsg& msg, CFactoryProto * pFactory)
{
	msg.Init(MSG_FACTORY);
	msg << (unsigned char)MSG_FACTORY_LEARN
		<< pFactory->GetIndex();
}

void FactoryErrorMsg(CNetMsg& msg, MSG_FACTORY_ERROR_TYPE errorcode)
{
	msg.Init(MSG_FACTORY);
	msg << (unsigned char)MSG_FACTORY_ERROR
		<< (unsigned char)errorcode;
}

void FactoryMakeMsg(CNetMsg& msg, CPC* ch, CFactoryProto * pFactory)
{
	msg.Init(MSG_FACTORY);
	msg << (unsigned char)MSG_FACTORY_ITEM_MAKE
		<< pFactory->GetIndex()
		<< pFactory->GetSealType()
		<< ch->GetSealExp(pFactory->GetSealType());
}
#endif

#ifdef TRIGER_SYSTEM
void TrigerEvnetMsg(CNetMsg& msg, MSG_EX_TRIGER_EVENT_TYPE type, int touch_ID, int Play_ID)
{
	msg.Init(MSG_EX_TRIGER_EVENT);
	msg << (unsigned char) type
		<< touch_ID
		<< Play_ID;
}
#endif // TRIGER_SYSTEM