#include "stdhdrs.h"
#include "Server.h"
#include "CmdMsg.h"
#include "Exp.h"
#include "Log.h"
#include "Battle.h"

#ifdef NEW_DIVISION_EXPSP

// 경험치 지급 받을 대상 리스트용
typedef struct __tagExpList
{
	CPC*					pc;		// 지급받을 대상
	struct __tagExpList*	next;	// 다음 대상 포인터
} EXP_LIST;

// 경험치 패널티 테이블 [%] : 1레벨 부터 최대 6레벨
static const int levelPenalty[6] = {95, 90, 75, 60, 45, 10};

// 파티 경험치 배분
void DivisionExpSPParty(CParty* party, CNPC* npc, CPC* pPreferencePC, LONGLONG nTotalDamage);

// 경험치 레벨 페널티 비율 구함
int GetExpLevelPenalty(int nLevelDiff);
// SP 레벨 패널티 비율 구함
int GetSPLevelPenalty(int nLevelDiff);

#ifdef EVENT_TEACH_2007
bool IsEventTeach2007(CParty * pParty);
#endif // EVENT_TEACH_2007

#ifdef CHANCE_EVENT
int CountChanceEventParty(CParty * pParty);
#endif // CHANCE_EVENT

// npc 사망시 경험치 배분
// 퍼스널 던전에서 레벨업에 의한 존 이동 발생시에는 true를 리턴
bool DivisionExpSP(CNPC* npc, CPC* pPreferencePC, LONGLONG nTotalDamage)
{
	LONGLONG		nExpNPC = npc->m_proto->m_exp;			// 지급될 경험치
#ifdef LC_MAL			/// kj
	nExpNPC = nExpNPC * ( 1 + ( ( 80 - npc->m_level ) * 3 / 160 ) );
#endif	// LC_MAL

	LONGLONG		nSPNPC = npc->m_proto->m_skillPoint;	// 지급될 SP
	EXP_LIST*		expList = NULL;							// 경험치 받을 대상 리스트
	EXP_LIST*		expNode = NULL;							// 대상 리스트 노드
	CAttackChar*	pAttackChar = NULL;						// 공격자 리스트 루프용
	CAttackChar*	pAttackCharNext = npc->m_attackList;	// 공격자 리스트 루프용
	CPC*			pExpPC = NULL;							// 지급받을 대상

	if (nTotalDamage < 1)
		return false;

	// 공격자 루프에서 각 PC의 대미지를 구함
	while ((pAttackChar = pAttackCharNext))
	{
		pAttackCharNext = pAttackCharNext->m_next;

		if (!pAttackChar->ch)
			continue ;

		// 근처에 있어야 대상에 포함
		if (!CheckInNearCellExt(npc, pAttackChar->ch))
			continue ;

		// 대미지가 있어야 함
		if (pAttackChar->m_damage < 1)
			continue ;

		// 받을 대상 PC를 선정
		switch (pAttackChar->ch->m_type)
		{
		case MSG_CHAR_PC:
			pExpPC = TO_PC(pAttackChar->ch);
			break;
		case MSG_CHAR_PET:
			pExpPC = TO_PET(pAttackChar->ch)->GetOwner();
			break;
		case MSG_CHAR_ELEMENTAL:
			pExpPC = TO_ELEMENTAL(pAttackChar->ch)->GetOwner();
			break;
#ifdef ATTACK_PET
		case MSG_CHAR_APET:
			pExpPC = TO_APET(pAttackChar->ch)->GetOwner();
			break;
#endif // ATTACK_PET
#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
		case MSG_CHAR_NPC:
			pExpPC = TO_NPC(pAttackChar->ch)->GetOwner();
			break;
#endif  // NIGHTSHADOW_SKILL	
		default:
			pExpPC = NULL;
			break;
		}

		// 받을 대상이 있어야 함
		if (pExpPC == NULL)
			continue ;

		// 누적 대미지 적용 : 펫/소환수등에 의한 대미지도 PC에 누적
		if (pExpPC->m_nExpDamage + pAttackChar->m_damage > 1)
			pExpPC->m_nExpDamage += pAttackChar->m_damage;

		// 대상 리스트에 추가
		expNode = new EXP_LIST;
		expNode->pc = pExpPC;
		expNode->next = expList;
		expList = expNode;
	}
	// -- 공격자 루프에서 대상 리스트 작성 끝

	// 대상 리스트를 돌면서 해당 대상자의 경험치 지급
	expNode = expList;
	while (expNode)
	{
		// 지급 대상의 누적 대미지 검사
		if (expNode->pc->m_nExpDamage > 0)
		{
			// 파티 여부에 따른 처리 구분
			if (expNode->pc->IsParty())
			{
				// 파티의 경우 따로 처리 : expNode 이후 노드만 처리
				DivisionExpSPParty(expNode->pc->m_party, npc, pPreferencePC, nTotalDamage);
			}
			else
			{
				// 파티가 아니면 대미지 비율에 맞게 지급

				// 몹과의 레벨 차이 구함
				int nLevelDiff = expNode->pc->m_level - npc->m_level;

				// 경험치/SP 패널티 구함
				LONGLONG nExpPenalty = GetExpLevelPenalty(nLevelDiff);
				LONGLONG nSPPenalty  = GetSPLevelPenalty(nLevelDiff);

				// 얻을 경험치 구함 : (NPC값 * 패널티 * 대미지) / (100 * 전체 대미지)
				LONGLONG nGiveExp	= (nExpNPC	* nExpPenalty	* expNode->pc->m_nExpDamage) / (100 * nTotalDamage);
				LONGLONG nGiveSP	= (nSPNPC	* nSPPenalty	* expNode->pc->m_nExpDamage) / (100 * nTotalDamage);

				// 랜덤 +- 10% 적용
				nGiveExp = nGiveExp + (nGiveExp * GetRandom(-10, 10) / 100);
				nGiveSP  = nGiveSP  + (nGiveSP  * GetRandom(-10, 10) / 100);

				
#ifdef NEW_GUILD
				int GuildExp = 0;
				if( expNode->pc->m_guildInfo && expNode->pc->m_guildInfo->guild() )
				{
					int guildindex = expNode->pc->m_guildInfo->guild()->index();
					CGuild* guild = gserver.m_guildlist.findguild( guildindex );

					if( expNode->pc->m_guildInfo->GetcontributeExp() != 0 )
					{
						GuildExp = nGiveExp * expNode->pc->m_guildInfo->GetcontributeExp() / 100;
						expNode->pc->m_GuildExp += GuildExp;
						nGiveExp = nGiveExp - GuildExp;
						int GP = 0;
						if( expNode->pc->m_GuildExp >= 10000 )
						{
							GP = expNode->pc->m_GuildExp / 10000;
							expNode->pc->m_GuildExp = expNode->pc->m_GuildExp % 10000;
							guild->AddGuildPoint( GP );
							expNode->pc->m_guildInfo->AddCumulatePoint( GP );


							if (IS_RUNNING_HELPER)
							{
								CNetMsg guildPointMsg;
								CNetMsg guildMemberPointMsg;
								HelperNewGuildPointUpdate( guildPointMsg, expNode->pc->m_index, guild->index(), GP );
								SEND_Q(guildPointMsg, gserver.m_helper);

								HelperSaveGuildMemberPointMsg( guildMemberPointMsg, guild->index(), expNode->pc->m_index, GP );
								SEND_Q( guildMemberPointMsg, gserver.m_helper );
							}

						}

					}
				}
#endif // NEW_GUILD

				// 보정
				if (nGiveExp < 1)	nGiveExp = 1;
				if (nGiveSP  < 1)	nGiveSP  = 1;

				// SP는 MAX_SKILLPOINT를 넘을 수 없다
				if (nGiveSP > MAX_SKILLPOINT) nGiveSP = MAX_SKILLPOINT;

				// 9월 이벤트 : 대상자가 우선권자이면
				if (expNode->pc == pPreferencePC)
				{
					// 경험치 4배
					if (expNode->pc->m_assist.m_avAddition.hcSepExp)
					{
						expNode->pc->m_assist.CureByItemIndex(882);	// 경험치
						nGiveExp = nGiveExp * 4;
						CNetMsg rmsg;
						EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_EXP);
						SEND_Q(rmsg, expNode->pc->m_desc);
					}

					// 9월 이벤트 : SP 4배
					if (expNode->pc->m_assist.m_avAddition.hcSepSP)
					{
						expNode->pc->m_assist.CureByItemIndex(883);	// 숙련도
						nGiveSP = nGiveSP * 4;
						CNetMsg rmsg;
						EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_SP);
						SEND_Q(rmsg, expNode->pc->m_desc);
					}
				}
#ifdef NEW_ACCERY_ADD
				if( expNode->pc->m_AddProb )
				{
					nGiveExp = nGiveExp + ( nGiveExp * expNode->pc->m_AddProb / 100 );
					nGiveSP = nGiveSP + ( nGiveSP * expNode->pc->m_AddProb / 100 );
				}
#endif //NEW_ACCERY_ADD

#ifdef ATTACK_PET
				CAPet* apet = expNode->pc->GetAPet();
				if( apet )
				{
					LONGLONG petexp = (npc->m_level) / 5 * ( npc->m_proto->CheckFlag(NPC_MBOSS|NPC_BOSS)? 4:1 );
					apet->AddExpSP( petexp , 0 );
				}
#endif // ATTACK_PET

				// 경험치 지급 : 레벨업에 의한 존이동시에 true가 반환된다
				if (expNode->pc->AddExpSP(nGiveExp, (int)nGiveSP, true))
				{
					// 대상 리스트 메모리 해제
					while (expList)
					{
						expNode = expList;
						expList = expList->next;
						expNode->pc->m_nExpDamage = 0;
						delete expNode;
					}
					return true;
				}

				// 지급 받은 PC 초기화
				expNode->pc->m_nExpDamage = 0;
			} // --- 파티 아닌 경우 처리 끝
		} // --- 지급 대상 처리 끝

		expNode = expNode->next;
	}

	// 대상 리스트 메모리 해제
	while (expList)
	{
		expNode = expList;
		expList = expList->next;
		expNode->pc->m_nExpDamage = 0;
		delete expNode;
	}

	return false;
}


// 파티원 들에게 경험치 SP 분배	(파티, NPC, 우선권, 전체 데미지)
void DivisionExpSPParty(CParty* party, CNPC* npc, CPC* pPreferencePC, LONGLONG nTotalDamage)
{
	// 파티 보너스 테이블 [%] : 1인부터 시작
#if defined (LC_USA) || defined (LC_BRZ) || defined (LC_JPN) || defined(LC_HBK) || defined(BSTEST)
	static const int countBonus[MAX_PARTY_MEMBER] = { 0, 30, 40, 50, 60, 70, 75, 80};
#else
	static const int countBonus[MAX_PARTY_MEMBER] = { 15, 30, 40, 50, 60, 70, 70, 70};
#endif // #if defined (LC_USA) || defined (LC_BRZ)

	EXP_LIST*			expParty = NULL;		// 파티원중 경험치 받을 리스트
	EXP_LIST*			expPartyNode = NULL;	// 파티원 노드
	EXP_LIST*			expNode = NULL;			// 대상 리스트 루프용
	EXP_LIST*			expNodeNext = NULL;		// 대상 리스트 루프용

#ifdef CHANCE_EVENT
	EXP_LIST *			ChanceMemberList = NULL;	// 찬스 이벤트 대상 파티원 리스트
	EXP_LIST *			ChanceMember = NULL;		// 찬스 이벤트 대상 파티원 노드
	int nChanceMember = 0;
#endif // CHANCE_EVENT

	LONGLONG			nPartyDamage = 0;		// 파티 총 대미지
	int					nParty = 0;				// 대상 파티원 수
	int					nPartyLevelTotal = 0;	// 파티원 레벨 합
	int					nPartyLevelMax = 0;		// 파티원 최대 레벨
	int					nPartyLevel = 0;		// 파티 평균 레벨
	LONGLONG			nExpParty = 0;			// 파티 경험치
#ifdef PARTY_EXP_DIVISION_CHANGE_2006
	LONGLONG			nExpParty_CA = 0;		// 파티 경험치 : 참여자용
	int					nSharedParty = 0;		// 파티 사냥 참여자 수
#endif // PARTY_EXP_DIVISION_CHANGE_2006
	LONGLONG			nSPParty = 0;			// 파티 SP
	LONGLONG			nExpPenalty;			// 경험치 패널티
	LONGLONG			nSPPenalty;				// SP 패널티
	int					i;						// 파티 루프용
#ifdef ATTACK_PET
	int					nHavePetCount=0;			// 파티원이 가지고 있는 공격형 펫 수
#endif //ATTACK_PET

	if (nTotalDamage < 1)
		return ;

	if (party == NULL)
		return ;

	if (npc == NULL)
		return ;

#ifdef EVENT_TEACH_2007
	bool bEventTeach2007 = IsEventTeach2007(party);
#endif // EVENT_TEACH_2007

#ifdef CHANCE_EVENT
	bool bChanceParty = false;
	int nChancePartyCnt = CountChanceEventParty(party);
	if (nChancePartyCnt > 0)
	{
		bChanceParty = true;
	}	 
#endif

	// 파티 총 대미지 구하기
	// 파티원 수 구하기
	// 파티원 중 해당자 리스트 만들기
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		CPC* pMember = party->GetNearMember(npc, i);
		if (pMember == NULL)
			continue;
		if ( DEAD(pMember) )
			continue;
		// 전체 대미지에 추가
		nPartyDamage += pMember->m_nExpDamage;
#ifdef PARTY_EXP_DIVISION_CHANGE_2006
		if (pMember->m_nExpDamage > 0)
			nSharedParty++;
#endif // PARTY_EXP_DIVISION_CHANGE_2006
		// 대상 리스트에 추가
		expPartyNode = new EXP_LIST;
		expPartyNode->pc = pMember;
		expPartyNode->next = expParty;
		expParty = expPartyNode;
		nParty++;
		// 파티원 레벨합
		nPartyLevelTotal += pMember->m_level;
		// 파티원 최대 레벨
		if (nPartyLevelMax < pMember->m_level)
			nPartyLevelMax = pMember->m_level;

#ifdef CHANCE_EVENT
		if (gserver.m_bChanceEvent == true
			&& bChanceParty == true
			&& gserver.CheckChanceEventLevel(pMember->m_level) == true)
		{
			ChanceMember = new EXP_LIST;
			ChanceMember->pc = pMember;
			ChanceMember->next = ChanceMemberList;
			ChanceMemberList = ChanceMember;
			nChanceMember++;
		}
#endif // CHANCE_EVENT
#ifdef ATTACK_PET
		// 현재 착용한 펫이 있으면 카운트 증가
		if( pMember->GetAPet() )
			nHavePetCount++;
#endif //ATTACK_PET
	}

	// 파티 경험치 분배 조건
	// 파티 대미지 존재
	// 대상 파티원이 1인 이상
	// 파티 레벨합이 1 이상
	if (nPartyDamage > 0 && nParty > 0 && nPartyLevelTotal > 0)
	{
		int basic_party_plus_exp;
		int basic_party_plus_sp;


		// bw : 060817 : 파티 시스템 버그 부분, 인원수 보너스도 한명일때 0이도록 수정요함
		/*
		if( nParty == 1 )
		{
			basic_party_plus_exp = 0;
			basic_party_plus_sp = 0;
		}
		else
		*/
		{
			basic_party_plus_exp = PARTY_PLUS_EXP;
			basic_party_plus_sp = PARTY_PLUS_SP;
		}

		// 파티 평균 레벨
		nPartyLevel = (nPartyLevelMax - (nPartyLevelTotal / nParty) > 5) ? (nPartyLevelMax - 5) : (nPartyLevelTotal / nParty);

		// 레벨차이 구하기
		int nLevelDiff = nPartyLevel - npc->m_level;

		// 파티 레벨 패널티 구하기
		nExpPenalty = GetExpLevelPenalty(nLevelDiff);
		nSPPenalty  = GetSPLevelPenalty(nLevelDiff);

		// 파티	보너스 구하기
		LONGLONG nExpBonus	= 100 + basic_party_plus_exp + countBonus[nParty - 1];
		LONGLONG nSPBonus	= 100 + basic_party_plus_sp + 5 * (nParty - 1);

		// 파티가 받을 경험치/SP 구하기
		LONGLONG nExpNPC = npc-> m_proto->m_exp;
#ifdef LC_MAL
		nExpNPC 		= nExpNPC * (1 + ( ( 80 - npc->m_level ) *3 /160 ) );
#endif // LC_MAL
		nExpParty		= nExpNPC * nExpBonus * nExpPenalty * nPartyDamage / (100 * 100 * nTotalDamage);
		if (nExpNPC >= 0 && nExpParty <= 0)
			nExpParty	= nExpNPC * nExpBonus / 100 * nExpPenalty / 100 * nPartyDamage / nTotalDamage;

#ifdef PARTY_EXP_DIVISION_CHANGE_2006
#if !defined (TEST_SERVER) && !defined (LC_JPN)
		nExpParty_CA	= nExpNPC * nExpBonus * nExpPenalty * nPartyDamage * (100 + nSharedParty * 9 - 8) / (100 * 100 * 100 * nTotalDamage);
		if (nExpNPC >= 0 && nExpParty_CA <= 0)
#endif
			nExpParty_CA= nExpNPC * nExpBonus / 100 * nExpPenalty / 100 * nPartyDamage / nTotalDamage * (100 + nSharedParty * 9 - 8) / 100;
#endif // PARTY_EXP_DIVISION_CHANGE_2006
		nSPParty		= npc->m_proto->m_skillPoint  * nSPBonus  * nSPPenalty  * nPartyDamage / (100 * 100 * nTotalDamage);
		if (npc->m_proto->m_skillPoint > 0 && nSPParty <= 0)
			nSPParty	= npc->m_proto->m_skillPoint  * nSPBonus  / 100 * nSPPenalty  / 100 * nPartyDamage / nTotalDamage;

		// 랜덤 적용
#if defined (LC_USA) || defined (LC_BRZ)
		if (nParty >= 2 )
		{
			nExpParty = nExpParty + (nExpParty * GetRandom(-10, 10) / 100);
#ifdef PARTY_EXP_DIVISION_CHANGE_2006
			nExpParty_CA = nExpParty_CA + (nExpParty_CA * GetRandom(-10, 10) / 100);
#endif // PARTY_EXP_DIVISION_CHANGE_2006
			nSPParty  = nSPParty  + (nSPParty *  GetRandom(-10, 10) / 100);
		}
#else
		nExpParty = nExpParty + (nExpParty * GetRandom(-10, 10) / 100);
#ifdef PARTY_EXP_DIVISION_CHANGE_2006
		nExpParty_CA = nExpParty_CA + (nExpParty_CA * GetRandom(-10, 10) / 100);
#endif // PARTY_EXP_DIVISION_CHANGE_2006
		nSPParty  = nSPParty  + (nSPParty *  GetRandom(-10, 10) / 100);
#endif // #if defined (LC_USA) || defined (LC_BRZ)
		
	}
	else
	{
		// 대상자 리스트 초기화
		while (expParty)
		{
			expPartyNode = expParty;
			expParty = expParty->next;
			expPartyNode->pc->m_nExpDamage = 0;
			delete expPartyNode;
		}
		return;
	}

	// 파티원에게 경험치/SP 지급
	LONGLONG nGiveExp = 0;		// 지급될 경험치
	LONGLONG nGiveSP  = 0;		// 지급될 SP
	LONGLONG nExpPartyForGive = 0;			// 지급에 사용할 파티 경험치 : nExpParty or nExpParty_CA
	expNodeNext = expParty;
	while ((expNode = expNodeNext))
	{
		expNodeNext = expNodeNext->next;

#ifdef PARTY_EXP_DIVISION_CHANGE_2006
		if (expNode->pc->m_nExpDamage > 0)
		{
			nExpPartyForGive = nExpParty_CA;
		}
		else
		{
			nExpPartyForGive = nExpParty;
		}
#else // PARTY_EXP_DIVISION_CHANGE_2006
		nExpPartyForGive = nExpParty;
#endif // PARTY_EXP_DIVISION_CHANGE_2006

		// 파티 타입에 따라 경험치
		switch (party->GetPartyType(MSG_DIVITYPE_EXP))
		{
		case PARTY_TYPE_RANDOM:			// 균등
		case PARTY_TYPE_FIRSTGET:		// 입수 우선
			//   ( (파티경험치 75%) * (대상자 레벨) / (파티총레벨) )
			// + ( (파티경험치 25%) / (파티원수) )
			nGiveExp = (nExpPartyForGive * 75 * expNode->pc->m_level / nPartyLevelTotal)
					 + (nExpPartyForGive * 25 / nParty);
			// 비율 단위 조정 : 분배%
			nGiveExp /= 100;
			if (nExpPartyForGive > 0 && nGiveExp <= 0)
			{
				nGiveExp = (nExpPartyForGive * 75 / 100 * expNode->pc->m_level / nPartyLevelTotal)
						 + (nExpPartyForGive * 25 / 100 / nParty);
			}			
			break;

		default:
		case PARTY_TYPE_BATTLE:			// 전투형
			//   ( (파티 경험치 70%) * (대상자 대미지) / (파티 대미지) )
			// + ( (파티 경험치 30%) * (대상자 레벨) / (파티 전체레벨) )
			nGiveExp = (nExpPartyForGive * 70 * expNode->pc->m_nExpDamage / nPartyDamage)
					 + (nExpPartyForGive * 30 * expNode->pc->m_level / nPartyLevelTotal);
			// 비율 단위 조정 : 분배%
			nGiveExp /= 100;
			if (nExpPartyForGive > 0 && nGiveExp <= 0)
			{
				nGiveExp = (nExpPartyForGive * 70 / 100 * expNode->pc->m_nExpDamage / nPartyDamage)
						 + (nExpPartyForGive * 30 / 100 * expNode->pc->m_level / nPartyLevelTotal);
			}
			break;
		}

		// 파티 타입에 따라 SP
		switch (party->GetPartyType(MSG_DIVITYPE_EXP))
		{
		case PARTY_TYPE_RANDOM:			// 균등
		case PARTY_TYPE_BATTLE:			// 전투형
			// (파티 SP) / (파티원수)
			nGiveSP = nSPParty / nParty;
			break;

		default:
		case PARTY_TYPE_FIRSTGET:		// 입수우선
			// 캐릭터 레벨 패널티
			nSPPenalty = GetSPLevelPenalty(expNode->pc->m_level - npc->m_level);

			//   ( (파티SP 20%) / (파티원수) )
			// + ( (파티SP 60%) * (대상자대미지) / (파티대미지) )
			// + ( (파티SP 20%) * (대상자레벨) / (파티총레벨) )
			nGiveSP = (nSPParty * 20 * nSPPenalty / nParty)
					+ (nSPParty * 60 * nSPPenalty * expNode->pc->m_nExpDamage / nPartyDamage)
					+ (nSPParty * 20 * nSPPenalty * expNode->pc->m_level / nPartyLevelTotal);
			// 비율 단위 조정 : 분배%, 패널티
			nGiveSP /= 10000;
			if (nSPParty > 0 && nGiveSP <= 0)
			{
				nGiveSP = (nSPParty * 20 / 100 * nSPPenalty / 100 / nParty)
						+ (nSPParty * 60 / 100 * nSPPenalty / 100 * expNode->pc->m_nExpDamage / nPartyDamage)
						+ (nSPParty * 20 / 100 * nSPPenalty / 100 * expNode->pc->m_level / nPartyLevelTotal);
			}
			break;
		}

#ifdef EVENT_TEACH_2007
		if (bEventTeach2007 == false)
		{
			// 파티 레벨 제한 패널티 : 최고레벨보다 15렙 더 작으면 5%만 얻음
			if (nPartyLevelMax - 15 > expNode->pc->m_level)
			{
				nGiveExp /= 20;
				nGiveSP  /= 20;
			}
		}
#elif defined(CHANCE_EVENT)
		if (gserver.m_bChanceEvent == false
			|| bChanceParty == false 
			|| gserver.CheckChanceEventLevel(expNode->pc->m_level) == false)
		{
			// 파티 레벨 제한 패널티 : 최고레벨보다 15렙 더 작으면 5%만 얻음
			if (nPartyLevelMax - 15 > expNode->pc->m_level)
			{
				nGiveExp /= 20;
				nGiveSP  /= 20;
			}
		}
#else
		// 파티 레벨 제한 패널티 : 최고레벨보다 15렙 더 작으면 5%만 얻음
		if (nPartyLevelMax - 15 > expNode->pc->m_level)
		{
			nGiveExp /= 20;
			nGiveSP  /= 20;
		}
#endif	// EVENT_TEACH_2007


		// 보정
		if (nGiveExp < 1)	nGiveExp = 1;
		if (nGiveSP  < 1)	nGiveSP  = 1;

		// SP는 MAX_SKILLPOINT를 넘을 수 없다
		if (nGiveSP > MAX_SKILLPOINT) nGiveSP = MAX_SKILLPOINT;

		// 9월 이벤트 : 대상자가 우선권자이면
		if (expNode->pc == pPreferencePC)
		{
			// 경험치 4배
			if (expNode->pc->m_assist.m_avAddition.hcSepExp)
			{
				expNode->pc->m_assist.CureByItemIndex(882);	// 경험치
				nGiveExp = nGiveExp * 4;
				CNetMsg rmsg;
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_EXP);
				SEND_Q(rmsg, expNode->pc->m_desc);
			}

			// 9월 이벤트 : SP 4배
			if (expNode->pc->m_assist.m_avAddition.hcSepSP)
			{
				expNode->pc->m_assist.CureByItemIndex(883);	// 숙련도
				nGiveSP = nGiveSP * 4;
				CNetMsg rmsg;
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_SEPTEMBER_SP);
				SEND_Q(rmsg, expNode->pc->m_desc);
			}
		}
#ifdef	NEW_ACCERY_ADD
		if( expNode->pc->m_AddProb )
		{
			nGiveExp = nGiveExp + ( nGiveExp * expNode->pc->m_AddProb / 100 );
			nGiveSP = nGiveSP + ( nGiveSP * expNode->pc->m_AddProb / 100 );
		}
#endif // NEW_ACCERY_ADD
		
#ifdef EVENT_TEACH_2007
		if (bEventTeach2007 == true
			&& party->GetPartyType() == PARTY_TYPE_RANDOM)
		{
			nGiveExp = nExpPartyForGive / nParty;
			
#ifdef LC_JPN
			if(expNode->pc->m_teachType == MSG_TEACH_STUDENT_TYPE)
			{	// 제가 최대 경험치 1만으로 제한
				if( nGiveExp > 10000)
				{
					nGiveExp = 10000;
				}
			}
#endif // LC_JPN
		}
#endif	// EVENT_TEACH_2007

#ifdef CHANCE_EVENT
		if ((gserver.m_bChanceEvent == true)
			&& (bChanceParty == true))
		{	// 찬스 이벤트 중이고 이벤트 파티일 경우
			if (gserver.CheckChanceEventLevel(expNode->pc->m_level) == false)
			{	// 이벤트 대상이 아님
				// 이벤트 대상 파티원과 거리 체크
				bool bTarget = false;
				while (ChanceMemberList)
				{
					ChanceMember = ChanceMemberList;
					ChanceMemberList = ChanceMember->next;
					if (ChanceMember->pc 
						&& expNode->pc
						&& CheckInNearCellExt(ChanceMember->pc, expNode->pc))
					{
						bTarget = true;
						break;
					}
					
				}
				
				if (bTarget == true)
				{	// 이벤트 대상 파티원과 가까이 있을 경우				
					if (nChancePartyCnt == 1)
					{	// 찬스 이벤트 파티원이 1명일때 해당 레벨대가 아닌 파티원
						// 이파티원은 AddExpSp 함수에서 찬스 이벤트 적용이 안되므로 
						// 여기서 증가분의 50% 적용
						int nProb = gserver.m_bChanceExpPercent - 100;
						if (nProb > 2)
						{
							nProb = 100 + nProb/2;
						}
						else
						{
							nProb = 100;
						}

						nGiveExp = nGiveExp * nProb / 100;
						nGiveSP = nGiveSP * nProb / 100;
					}
					else
					{	// 찬스 이벤트 파티원 2명이상, 해당 레벨대가 아닌 파티원
						nGiveExp = nGiveExp * gserver.m_bChanceExpPercent / 100;
						nGiveSP = nGiveSP * gserver.m_bChanceSpPercent / 100;
					}
				}
			}
			else	// 이벤트 대상
			{	// 레벨 차이 * 레벨 차이 / 35  최대 -95%
				int diff_level = npc->m_level - expNode->pc->m_level;
				int level_penalty = 0;
				if(diff_level > 0)
				{
//					level_penalty = diff_level*diff_level/35;
					level_penalty = diff_level*diff_level/25;
//					if(level_penalty > 95)
					if(level_penalty > 98)
					{
//						level_penalty = 95;
						level_penalty = 98;
					}
				}
				
				nGiveExp = nExpPartyForGive / nParty;
				nGiveSP = nSPParty / nParty;

				if(level_penalty > 0)
				{
//					nGiveExp = nGiveExp - nGiveExp * level_penalty / gserver.m_bChanceExpPercent;
					nGiveExp = nGiveExp * (100 - level_penalty) / 100;
//					nGiveSP = nGiveSP - nGiveSP * level_penalty / gserver.m_bChanceSpPercent;
					nGiveSP = nGiveSP * (100 - level_penalty) / 100;
				}							
			}
		}
#endif // CHANCE_EVENT

#ifdef NEW_GUILD
			int GuildExp = 0;
			if( expNode->pc->m_guildInfo && expNode->pc->m_guildInfo->guild() )
			{
				CGuild* guild = expNode->pc->m_guildInfo->guild();
				if( expNode->pc->m_guildInfo->GetcontributeExp() != 0 )
				{
					GuildExp = nGiveExp * expNode->pc->m_guildInfo->GetcontributeExp() / 100;
					expNode->pc->m_GuildExp += GuildExp;
					nGiveExp = nGiveExp - GuildExp;
					int GP = 0;
					if( expNode->pc->m_GuildExp >= 10000 )
					{
						GP = expNode->pc->m_GuildExp / 10000;
						expNode->pc->m_GuildExp = expNode->pc->m_GuildExp % 10000;
						guild->AddGuildPoint( GP );
						expNode->pc->m_guildInfo->AddCumulatePoint( GP );
						if (IS_RUNNING_HELPER)
						{
							CNetMsg guildPointMsg;
							CNetMsg guildMemberPointMsg;
							HelperNewGuildPointUpdate( guildPointMsg, expNode->pc->m_index, guild->index(), GP );
							SEND_Q(guildPointMsg, gserver.m_helper);

							HelperSaveGuildMemberPointMsg( guildMemberPointMsg, guild->index(), expNode->pc->m_index, GP );
							SEND_Q( guildMemberPointMsg, gserver.m_helper );
						}
					}



				}
			}
#endif // NEW_GUILD

		// 경험치 지급
		expNode->pc->AddExpSP(nGiveExp, (int)nGiveSP, true);

#ifdef ATTACK_PET
		CAPet* apet = expNode->pc->GetAPet();
		if( apet )
		{
			if( nHavePetCount == 0 ) 
				nHavePetCount = 1;

			LONGLONG petexp = (npc->m_level) / 5 / nHavePetCount * ( npc->m_proto->CheckFlag(NPC_MBOSS|NPC_BOSS)? 4:1 );
			apet->AddExpSP( petexp , 0 );
		}
#endif // ATTACK_PET

		// 지급 받은 PC 초기화
		expNode->pc->m_nExpDamage = 0;
	} // -- 파티원 분배 루틴 끝

	// 대상자 리스트 초기화
	while (expParty)
	{
		expPartyNode = expParty;
		expParty = expParty->next;
		expPartyNode->pc->m_nExpDamage = 0;
		delete expPartyNode;
	}
}

int GetExpLevelPenalty(int nLevelDiff)
{
	int nExpPenalty = 100;
	if (nLevelDiff > 0)
	{
		// 경험치 패널티 처리 : 최대 6레벨
		if (nLevelDiff > 6)
			nExpPenalty = levelPenalty[5];
		else
			nExpPenalty = levelPenalty[nLevelDiff - 1];
	}
	return nExpPenalty;
}

int GetSPLevelPenalty(int nLevelDiff)
{
	int nSPPenalty = 100;
	if (nLevelDiff > 0)
	{
		// SP 패널티 처리 : MAX_PENALTY_SP_LEVEL보다 많으면 무조건 1
		if (nLevelDiff > MAX_PENALTY_SP_LEVEL)
			nSPPenalty = 0;
		else
			nSPPenalty = 100 - (nLevelDiff * DOWN_LEVEL_SP);
	}
	else if (nLevelDiff < 0)
	{
		// 상위 몹을 잡을 경우 SP만 증가
		nSPPenalty = 100 + nLevelDiff * UP_LEVEL_SP;
		if( nSPPenalty <= 0 )
			nSPPenalty  = 0;
	}
	return nSPPenalty;
}

#endif // #ifdef NEW_DIVISION_EXPSP

#ifdef EVENT_TEACH_2007
bool IsEventTeach2007(CParty * pParty)
{
	if( pParty->GetPartyType() != PARTY_TYPE_RANDOM )
	{	// 균등 파티 인지 확인
		return false;
	}

	int boss_index = pParty->GetBossIndex();
	if (boss_index == -1)
	{	// 파티장 인덱스가 있는지 확인
		return false;
	}

	CPC * pBoss = gserver.m_playerList.Find(boss_index);
	if (pBoss == NULL)
	{	// 파티장이 있는지 확인
		return false;
	}

	if (pBoss->m_teachType != MSG_TEACH_TEACHER_TYPE)
	{	// 파티장이 선생인지 확인
		return false;
	}

	if (pParty->GetMemberCount() < 2)
	{	// 파티원이 있는지 확인
		return false;
	}

	for (int i = 1; i < MAX_PARTY_MEMBER; i++)
	{
		int idx = pParty->GetMemberCharIndex(i);
		if (idx == -1)
		{
			continue;
		}

		CPC * pMember = gserver.m_playerList.Find(idx);
		if (pMember == NULL)
		{
			continue ;
		}

		if (pMember->m_teachType != MSG_TEACH_STUDENT_TYPE)
		{	// 학생이 아닌 경우
			return false;
		}

		if (pMember->m_teachIdx[0] != boss_index)
		{	// 파티장의 제자가 아니다
			return false;
		}
	}

	return true;
}
#endif // EVENT_TEACH_2007

#ifdef CHANCE_EVENT
int CountChanceEventParty(CParty * pParty)
{
	if (pParty == NULL)
	{
		return 0;
	}

	int cnt = 0;
	for (int i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		int idx = pParty->GetMemberCharIndex(i);
		if (idx == -1)
		{
			continue;
		}

		CPC * pMember = gserver.m_playerList.Find(idx);
		if (pMember == NULL)
		{
			continue ;
		}

		if (gserver.CheckChanceEventLevel(pMember->m_level) == true)
		{
			cnt++;
		}
	}

	return cnt;
}
#endif // CHANCE_EVENT