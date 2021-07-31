#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"
//#include "Skill.h"
#include "DratanCastle.h"

void do_Action(CPC* ch, CNetMsg& msg)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(ch);
	}
#endif // DRATAN_CASTLE

	int charindex;
	char type;
	char index;

	CNetMsg rmsg;

	msg.MoveFirst();

	msg >> charindex >> type >> index;

	switch (type)
	{
	case ACTION_GENERAL:
		{
			switch (index)
			{
			case AGT_SITDOWN:				// 앉기 서기
			case AGT_PET_SITDOWN:			// 펫 앉기 서기
				// 죽은 상태면 무시
				if (DEAD(ch) || ch->m_personalshop != NULL || ch->IsSetPlayerState(PLAYER_STATE_CHANGE))
					return ; 

#ifdef ENABLE_PET
				// 펫 앉기 서기 등기면 펫을 타고 있어야 함
				if (index == AGT_PET_SITDOWN)
				{
					if (!ch->GetPet() || !ch->GetPet()->IsMount())
						return ;
				}
#endif // #ifdef ENABLE_PET

				if (!ch->IsSetPlayerState(PLAYER_STATE_SITDOWN))
				{
					if (ch->m_currentSkill)
					{
						ch->m_currentSkill->Cancel(ch);
						ch->m_currentSkill = NULL;
					}
					ch->ResetPlayerState(PLAYER_STATE_MOVING);
				}
				ch->TogglePlayerState(PLAYER_STATE_SITDOWN);
				ch->CalcStatus(true);
				break;

			case AGT_PKMODE:			// 평화/대결 모드
#ifdef BLOCK_PVP
				return;
#else
#if defined ( NON_PK_SYSTEM )
				if ( gserver.m_bNonPK )
					return;
#endif

				if (DEAD(ch) || ch->m_personalshop != NULL)
					return ;

				if (ch->m_pZone->m_index == ZONE_GUILDROOM)
					return;

				if (ch->m_level <= PKMODE_LIMIT_LEVEL)
				{
					bool bSkipLevel = false;
#ifdef ENABLE_WAR_CASTLE
					// 공성 진행 중 공성 지역 내에서는 저레벨도 PK 가능
					if (ch->m_pZone->m_index == CWarCastle::GetCurSubServerCastleZoneIndex() && ch->GetMapAttr() == MAPATT_WARZONE)
					{
						CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
						if (castle && castle->GetState() != WCSF_NORMAL)
						{
							bSkipLevel = true;
						}
					}
#endif
					if (!bSkipLevel)
					{
						CNetMsg errMsg;
						SysMsg(errMsg, MSG_SYS_PKMODE_LIMITLEVEL);
						SEND_Q(errMsg, ch->m_desc);
						return;
					}
				}

				if (ch->IsSetPlayerState(PLAYER_STATE_PKMODEDELAY) || ch->IsSetPlayerState(PLAYER_STATE_RAMODE) || ch->IsSetPlayerState(PLAYER_STATE_CHANGE))
					return ;

				if (ch->IsInPeaceZone(true))
					return ;


				if (ch->GetMapAttr() == MAPATT_FREEPKZONE)
					return ;
				
				// PVP 아레나 존이면은 프리피케이 공간 말고는 모두 pk모드가 안되게 막음..   yhj
				if ( ch->GetMapAttr() != MAPATT_FREEPKZONE 
					&& ch->GetMapAttr() != MAPATT_PEACEZONE 
					&& ch->m_pZone->m_index == ZONE_PK_TOURNAMENT )
				{
					return ;
				}

				// 스트레이아나 마을 계단에서 pk모드가 안되게 막음..   yhj
				// 다음 좌표는 스트레이아나 마을의 내부좌표
				if ( (ch->GetMapAttr() == MAPATT_STAIR_UP || ch->GetMapAttr() == MAPATT_STAIR_DOWN ) 
					&& ch->m_pZone->m_index == ZONE_STREIANA
					&& GET_X(ch) >= 940.0 && GET_X(ch) <= 1090.0
					&& GET_Z(ch) >= 515.0 && GET_Z(ch) <= 695.0 ) 
				{
					return ;
				}

				if (ch->IsSetPlayerState(PLAYER_STATE_PKMODE))
				{
					ch->SetPlayerState(PLAYER_STATE_PKMODEDELAY);
					ch->m_pkmodedelay = PULSE_PKMODE_DELAY;
				}
				else
				{
					ch->TogglePlayerState(PLAYER_STATE_PKMODE);
					ch->CancelInvisible();
				}
#endif
				break;
#ifdef EVENT_TLD_2007_SONGKRAN
			case AGT_THROW_WATER:
				int targetindex;
				msg >> targetindex;
				
				CCharacter* tch = NULL;
				CNetMsg rmsg;
				tch = ch->m_pArea->FindCharInCell( ch, targetindex, MSG_CHAR_PC, true );
				if( tch == NULL )
				{
					return;
				}


				CSkill* skill = new CSkill( gserver.m_skillProtoList.Find( 436 ), 1 );
				bool bApply;
				ApplySkill( ch, tch, skill, -1, bApply );
				delete skill;
//				EffectEtcMsg( rmsg, tch, MSG_EFFECT_ETC_HIT_WATER );
//				tch->m_pArea->SendToCell( rmsg, tch, true );

				break;
#endif //EVENT_TLD_2007_SONGKRAN
			}
		}
		break;

	case ACTION_SOCIAL:
		break;

	case ACTION_PARTY:
		break;

	case ACTION_GUILD:
		break;
	}

	ActionMsg(rmsg, ch, type, index);
	ch->m_pArea->SendToCell(rmsg, ch, true);
}
