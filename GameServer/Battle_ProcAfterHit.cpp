#include "stdhdrs.h"
#include "Server.h"
#include "Battle.h"
#include "WarCastle.h"
#include "CmdMsg.h"

// ProcAttack에서 미스를 제외한 공격 데미지 적용 이후 처리되는 내용
void ProcAfterHit(CCharacter* of, CCharacter* df)
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

	CPC* tpc = NULL;

	switch (of->m_type)
	{
	case MSG_CHAR_PC:
		opc = TO_PC(of);
#ifdef NEW_PET_EXP_SYSTEM
#else // NEW_PET_EXP_SYSTEM
		opc->m_bNotMiss = true;
#endif // NEW_PET_EXP_SYSTEM
		break;

	case MSG_CHAR_NPC:
		onpc = TO_NPC(of);
		break;

	case MSG_CHAR_PET:
		opet = TO_PET(of);
		opc = opet->GetOwner();
		if (opc == NULL)
			return ;
		break;

	case MSG_CHAR_ELEMENTAL:
		oelemental = TO_ELEMENTAL(of);
		opc = oelemental->GetOwner();
		if (opc == NULL)
			return ;
		break;
#ifdef ATTACK_PET
	case MSG_CHAR_APET:
		oapet = TO_APET(of);
		opc = oapet->GetOwner();
		if (opc == NULL)
			return ;
		break; 
#endif //ATTACK_PET

	default:
		return ;
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
			return ;
		break;

	case MSG_CHAR_ELEMENTAL:
		delemental = TO_ELEMENTAL(df);
		dpc = delemental->GetOwner();
		if (dpc == NULL)
			return ;
		break;
#ifdef ATTACK_PET
	case MSG_CHAR_APET:
		dapet = TO_APET(df);
		dpc = dapet->GetOwner();
		if (dpc == NULL)
			return ;
		break; 
#endif // ATTACK_PET

	default:
		return ;
	}

#ifdef RESTRICT_PK
	if (opc && dpc)
	{
		if (opc->IsChaotic())
			opc->m_lastPKTime = gserver.m_gameTime;
	}
#endif // RESTRICT_PK

#ifdef MONSTER_RAID_SYSTEM
	// NPC가 공격을 해도 전투 상태를 저장
	if (onpc)
#ifdef LC_JPN	// 일본은 3~4시간 마다 텔레포트
		onpc->m_nRaidMoveTime = gserver.m_pulse + GetRandom(PULSE_REAL_HOUR * 3, PULSE_REAL_HOUR * 4);
#else			
		onpc->m_nRaidMoveTime = gserver.m_pulse + GetRandom(PULSE_MONSTER_RAID_MOVE * 9 / 10, PULSE_MONSTER_RAID_MOVE * 11 / 10);
#endif // LC_JPN

	// NPC가 맞아도 저장
	if (dnpc)
	{
#ifdef LC_JPN	// 일본은 3~4시간 마다 텔레포트
		dnpc->m_nRaidMoveTime = gserver.m_pulse + GetRandom(PULSE_REAL_HOUR * 3, PULSE_REAL_HOUR * 4);
#else	
		dnpc->m_nRaidMoveTime = gserver.m_pulse + GetRandom(PULSE_MONSTER_RAID_MOVE * 9 / 10, PULSE_MONSTER_RAID_MOVE * 11 / 10);
#endif // LC_JPN

		// 체력이 20% 이하로 떨어지면 소환
		if (dnpc->m_proto->CheckFlag(NPC_RAID)			// 레이드 몹이 맞았고
			&& dnpc->m_proto->m_aileader_idx > 0		// 소환 설정이 입력 되어 있고
			&& dnpc->m_proto->m_aileader_count > 0
			&& dnpc->m_hp <= dnpc->m_maxHP / 5			// 체력이 20% 이하
			&& !dnpc->m_bRaidNPCSummon)					// 현재 소환하지 않은 상태
		{
			for(int i = 0; i < dnpc->m_proto->m_aileader_count; i++)
			{
				float x = 0.0f;
				float z = 0.0f;
				int nTry = 0;
				do
				{
					x = GET_X(dnpc) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 30) / 10.0f;
					z = GET_Z(dnpc) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 30) / 10.0f;
					switch (dnpc->m_pArea->GetAttr(GET_YLAYER(dnpc), x, z))
					{
					case MAPATT_FIELD:
					case MAPATT_STAIR_UP:
					case MAPATT_STAIR_DOWN:
						break;
					default:
						x = 0.0f;
						z = 0.0f;
						break;
					}
					nTry++;
				} while (nTry <= 10 && x < 0.1f && z < 0.1f);
				if (nTry > 10)
				{
					x = GET_X(dnpc);
					z = GET_Z(dnpc);
				}

				CNPC* rnpc = gserver.m_npcProtoList.Create(dnpc->m_proto->m_aileader_idx, NULL);
				if (!rnpc)
					continue ;

				GET_X(rnpc) = x;
				GET_YLAYER(rnpc) = GET_YLAYER(dnpc);
				GET_Z(rnpc) = z;
				GET_R(rnpc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
				rnpc->m_disableTime = 0;
				rnpc->m_postregendelay = 0;
				
				rnpc->m_regenX = GET_X(rnpc);
				rnpc->m_regenY = GET_YLAYER(rnpc);
				rnpc->m_regenZ = GET_Z(rnpc);

				int cx, cz;
				dnpc->m_pArea->AddNPC(rnpc);
				dnpc->m_pArea->PointToCellNum(GET_X(rnpc), GET_Z(rnpc), &cx, &cz);
				dnpc->m_pArea->CharToCell(rnpc, GET_YLAYER(rnpc), cx, cz);
				
				CNetMsg appearNPCMsg;
				AppearMsg(appearNPCMsg, rnpc, true);
				dnpc->m_pArea->SendToCell(appearNPCMsg, GET_YLAYER(rnpc), cx, cz);

				dnpc->m_bRaidNPCSummon = true;
			}
		}

#ifdef RAID_MONSTER_SKILL
		// 부활 버프를 가진 캐릭터는 죽으면 버프를 지우면서 mp가 만땅 찬다.
		if ( dnpc->m_hp <= 0 && dnpc->m_assist.FindByType(MT_ASSIST, MST_ASSIST_REBIRTH) )
		{
			dnpc->m_hp = dnpc->m_maxHP;
			dnpc->m_assist.CureAssist(MST_ASSIST_REBIRTH, 99);
		}
#endif // RAID_MONSTER_SKILL
	}
#endif // MONSTER_RAID_SYSTEM

#ifdef EVENT_XMAS_2006
	if (onpc)
		onpc->m_nEventXMasMoveTime = gserver.m_pulse + PULSE_XMAS_2006_MOVE;
	if (dnpc)
		dnpc->m_nEventXMasMoveTime = gserver.m_pulse + PULSE_XMAS_2006_MOVE;
#endif // EVENT_XMAS_2006

	// 블러드 포인트 적용 검사, 액세서리 내구 감소 검사
	int nBloodPoint = 0;
	bool bApplyAccessory = false;
	if (IS_PC(of) && IS_NPC(df))
	{
		tpc = opc;
		bApplyAccessory = true;
		if (opc->m_level - dnpc->m_level < 5)
			nBloodPoint = ATTACK_BLOOD_POINT;
	}
	if (IS_NPC(of) && IS_PC(df))
	{
		tpc = dpc;
		bApplyAccessory = true;
		if (dpc->m_level - onpc->m_level < 5)
			nBloodPoint = DEFENSE_BLOOD_POINT;
	}

	// 블러드 포인트 적용
	if (nBloodPoint)
	{
		tpc->m_bloodPoint -= nBloodPoint;
		if (tpc->m_bloodPoint <= 0)
		{
			tpc->m_bloodPoint = PC_BLOOD_POINT;
			tpc->SettingItemOption();
		}
	}

// <--! kjban 080311
	// Accessory 내구도
	if (NULL != tpc)
	{
		for(int i = WEARING_ACCESSORY1; i <= WEARING_ACCESSORY3; i++)
		{
			if(NULL == tpc->m_wearing[i] 
				|| -1 == tpc->m_wearing[i]->m_itemProto->m_maxUse)
			{
				continue;
			}

			tpc->m_wearing[i]->m_used -= ACCESSORY_USED_DEFENSE;

			// 악세사리 소멸
			if (tpc->m_wearing[i]->m_used <= 0)
			{
				tpc->m_wearing[i]->m_used = 0;
				tpc->CalcStatus(true);
			}
		}
	}
// !-->
/*
	// Accessory 내구도
	if (tpc)
	{
		for (i = WEARING_ACCESSORY1; i <= WEARING_ACCESSORY1; i++)
		{
			if (!tpc->m_wearing[i] || tpc->m_wearing[i]->m_itemProto->m_maxUse != -1)
				continue;

			tpc->m_wearing[i]->m_used -= ACCESSORY_USED_DEFENSE;

			// 악세사리 소멸
			if (tpc->m_wearing[i]->m_used <= 0)
			{
				tpc->m_wearing[i]->m_used = 0;
				tpc->CalcStatus(true);
			}
		}
	}
*/
	// NPC 리젠 이후 경직 상태 해제
	if (dnpc && dnpc->m_postregendelay)
	{
		dnpc->m_postregendelay = 0;
		dnpc->m_delay = 0;
	}
}
