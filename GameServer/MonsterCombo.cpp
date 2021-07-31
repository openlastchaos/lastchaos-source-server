// MonsterCombo.cpp: implementation of the CMonsterCombo class.
//
//////////////////////////////////////////////////////////////////////
#include "stdhdrs.h"
#include "DBCmd.h"
#include "Server.h"
#include "Log.h"
#include "MonsterCombo.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Battle.h"

#ifdef MONSTER_COMBO

CMissionCaseProto::CMissionCaseProto() : m_name(MAX_ITEM_NAME_LENGTH + 1), m_listPanalty(NULL) 
{
	m_nIndex		= 0;
	m_nNas			= 0;
	m_nRegenCount	= 0;
	m_npcRegen		= NULL;
	m_nPoint		= 0;
}

CMissionCaseProto::~CMissionCaseProto()
{
	if(m_npcRegen)
	{
		delete[] m_npcRegen;
		m_npcRegen = NULL;
	}

	m_listPanalty.RemoveAll();
}

bool CMissionCaseProto::LoadProto()
{
	CLCString sql(1024);
	sql.Format("SELECT distinct a_step FROM t_npc_regen_combo WHERE a_enable = 1 AND a_missioncase_idx = %d ORDER BY a_step", m_nIndex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbdata);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;

	if(cmd.m_nrecords == 0)
		return false;

	
	m_nRegenCount = cmd.m_nrecords;
	m_npcRegen = new MOBREGEN[cmd.m_nrecords];
	memset(m_npcRegen, 0, sizeof(m_npcRegen));

	int i = 0;
	int step;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_step", step);

		CLCString sql2(1024);
		sql2.Format("SELECT * FROM t_npc_regen_combo WHERE a_step = %d AND a_missioncase_idx = %d AND a_enable = 1", step, m_nIndex);

		CDBCmd cmd2;
		cmd2.Init(&gserver.m_dbdata);
		cmd2.SetQuery(sql2);
		if(!cmd2.Open())
			return false;

		if(cmd2.m_nrecords == 0)
			return false;

		m_npcRegen[i].step = step;
		m_npcRegen[i].typecount = cmd2.m_nrecords;
		m_npcRegen[i].mobtype = new MOBTYPE[cmd2.m_nrecords];
		memset(m_npcRegen[i].mobtype, 0, sizeof(m_npcRegen[i].mobtype));

		// 몹종류
		int k = 0;
		while(cmd2.MoveNext())
		{
			cmd2.GetRec("a_npcidx", m_npcRegen[i].mobtype[k].npcidx);
			cmd2.GetRec("a_count", m_npcRegen[i].mobtype[k].count);
			k++;
		}

		// 패널티
		PENALTY penalty;

		sql2.Format("SELECT * FROM t_penalty WHERE a_missioncase_idx = %d", m_nIndex);
		cmd2.SetQuery(sql2);
		if(!cmd2.Open())
			return false;

		while(cmd2.MoveNext())
		{
			cmd2.GetRec("a_skillindex", penalty.index);
			cmd2.GetRec("a_skillLevel", penalty.level);	

			m_listPanalty.AddToTail(penalty);
		}

		i++;
	}
	return true;
}

const MOBREGEN* CMissionCaseProto::FindMobRegen(int step)
{
	int i;
	for(i = 0; i < m_nRegenCount; ++i)
	{
		if(m_npcRegen[i].step == step)
			return &m_npcRegen[i];
	}
	return NULL;
}


CMIssionCaseList::CMIssionCaseList()
{
	m_nCount	= 0;
	m_proto		= NULL;
}

CMIssionCaseList::~CMIssionCaseList()
{
	if(m_proto)
	{
		delete[] m_proto;
		m_proto = NULL;
	}
}

bool CMIssionCaseList::LoadList()
{
	CLCString sql(1024);
	sql.Format("SELECT * FROM t_missioncase WHERE a_enable = 1 ORDER BY a_index");

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbdata);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;
	
	if(cmd.m_nrecords == 0)
		return false;

	m_nCount = cmd.m_nrecords;
	m_proto = new CMissionCaseProto[cmd.m_nrecords];
	memset(m_proto, 0, sizeof(m_proto));

	int i = 0;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_index", m_proto[i].m_nIndex);
		cmd.GetRec("a_nas", m_proto[i].m_nNas);
		cmd.GetRec("a_name", m_proto[i].m_name);
		cmd.GetRec("a_point", m_proto[i].m_nPoint);

		if(!m_proto[i].LoadProto())
			return false;

		i++;
	}
	return true;
}

CMissionCaseProto* CMIssionCaseList::FindProto(int index)
{
	CMissionCaseProto proto;
	proto.m_nIndex = index;

	return (CMissionCaseProto*)bsearch(&proto, m_proto, m_nCount, sizeof(CMissionCaseProto), CompIndex);
}


/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonsterCombo::CMonsterCombo() : m_listEffect(NULL)
{
	m_bIsBattle		= false;
	m_nExtra		= 0;
	m_nStage		= 1;
	m_nRegenStep	= 1;
	m_pulseFirst	= 0;
	m_pulseSecond	= 0;
	m_nTotalMission = 0;

	m_case = NULL;
	m_area = NULL;

	m_cPlayerCount = 1;
	m_cPlayerNum   = 1;

	m_nIndex	   = -1;

	memset(m_effectNPC, 0, sizeof(CNPC*) * MAXEFFECT);
}

CMonsterCombo::~CMonsterCombo()
{
	if(m_case)
	{
		delete[] m_case;
		m_case = NULL;
	}
	memset(m_effectNPC, 0, sizeof(CNPC*) * MAXEFFECT);
	m_listEffect.RemoveAll();
}

bool CMonsterCombo::Init(int count)
{
	if(count < 1 || count > 20)
		return false;

	m_nExtra = GetRandom(1, 5);
	m_nTotalMission = count;
	m_case = new MISSION[count];
	return true;
}

void CMonsterCombo::SetArea(CArea *area)
{
	m_area = area;
}


char CMonsterCombo::Run()
{
	CNetMsg rmsg;
	char ret;
	int error;
	int extra;

	switch(m_nExtra)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		break;
	default:
		return 2;
	}

	extra = m_nExtra + 6;
	ret = 1;

	if(!m_area)
		return 2;

	if(m_bIsBattle)
	{
		// 모두 죽었을 경우
		if(m_area->CountPCInExtra(extra, true) < 1)
		{
			// 변수 원래대로 변경
			InitStageVar();

			// npc 모두 제거
			DeleteAllMob();

			// 미션실패했음을 알림
			MonsterComboMsg(rmsg, MSG_EX_MONSTERCOMBO_MISSION_COMPLETE);
			rmsg << 0;
			rmsg << m_nStage;
			m_area->SendToAllClient(rmsg); 

			// 로그찍기
			GAMELOG << init("COMBO MISSION FAILED")
					<< "AREA INDEX: "
					<< m_area->m_index
					<< " COMBO INDEX: "
					<< m_area->m_monsterCombo->m_nIndex;

			PrintCharIndex();
			
			GAMELOG	<< end;

		}
		else
		{
			if(m_area->CountMobInCombo() < 1)
			{
				// return 2:오류, 1: 리젠 0: 리젠 없음
				error = NextNPCRegen();

				switch(error)
				{
				case 0:
					{
						char Success = 1;		// 미션케이스 완료 여부 : 1 다음스테이지로, 2 모두 완료
						int  Stage;				// 다음스테이지 넘버, 모두 완료했을 경우 0을 넘겨줌

						// 이펙트 npc 모두 제거
						DeleteAllMob();

						// 선물상자 npc 리젠
						GiftMobRegen();

						// 패널티 상태 초기화
						ClearPenalty();

						// 다음 미션이 있는가?
						if(!FindMission(m_nStage + 1))
						{
							ret = 0;
							Success = 2;			// 완료 여부
							Stage = 0;				// 다음스테이지
						}
						Stage = m_nStage + 1;

						ComboNPCRegen();

						// 미션 성공했음을 알림
						MonsterComboMsg(rmsg, MSG_EX_MONSTERCOMBO_MISSION_COMPLETE);
						rmsg << Success
							 << Stage;

						m_area->SendToAllClient(rmsg);

						// 로그
						if(Success == 1)
						{
							GAMELOG << init("COMBO MISSION COMPLETE");
						}
						else
						{
							GAMELOG << init("COMBO MISSION ALL COMPLETE");
						}

						GAMELOG << "AREA INDEX: "
								<< m_area->m_index
								<< " COMBO INDEX: "
								<< m_area->m_monsterCombo->m_nIndex
								<< " STAGE: " << m_nStage;
						PrintCharIndex();
						GAMELOG	<< end;

						SetNextVar();
					}
					break;
				case 1:
					{
						const MISSION* mission = NULL;
						mission = FindMission(m_nStage);

						GAMELOG << init("COMBO NPCREGEN")
								<< "AREA INDEX: "
								<< m_area->m_index
								<< " COMBO INDEX: "
								<< m_area->m_monsterCombo->m_nIndex
								<< " STAGE: " << m_nStage
								<< " NPC REGEN STEP: " << m_nRegenStep		// 몇번째 리젠인지
								<< " TOTAL REGEN COUNT: " << m_cPlayerNum		// 리젠 총 반복 횟수
								<< " REGEN REPEAT: " << m_cPlayerCount;		// 리젠반복 횟수
								
						if(mission)
						{
							GAMELOG << " CASE INDEX: "
									<< mission->proto->m_nIndex;
						}

						PrintCharIndex();
						GAMELOG << end;
					}
					break;
				case 2:
					{
						ret = 2;
					}
					break;
				}
			}
			else
			{
				// 불기둥 발동
				if(gserver.m_pulse > m_pulseFirst)
				{
					m_pulseFirst = gserver.m_pulse + PULSE_REAL_SEC * 10;

					FireEffectReady2();
				}

				if(gserver.m_pulse > m_pulseSecond)
				{
					m_pulseSecond = m_pulseFirst + PULSE_REAL_SEC * 2;

					FireEffectAct();
				}
			}
		}
	}

	return ret;
}


void CMonsterCombo::DeleteAllMob()
{
	CNPC* pNext = m_area->m_npcList;
	CNPC* p;
	while((p = pNext))
	{
		pNext = p->m_pNPCNext;
		if(p->m_proto->CheckFlag(NPC_SHOPPER) == 0)
		{
			p->SendDisappearAllInCell(true);

			DelAttackList(p);
			m_area->CharFromCell(p, true);
			m_area->DelNPC(p);
		}
	}

	memset(m_effectNPC, 0, sizeof(CNPC*) * MAXEFFECT);
	m_listEffect.RemoveAll();
}


// return 2:오류, 1: 리젠 0: 리젠 없음
char CMonsterCombo::NextNPCRegen()
{
	const MISSION* mission = FindMission(m_nStage);

	if(!mission)
		return 2;

	CMissionCaseProto* proto = mission->proto;

	m_cPlayerCount++;

	m_listEffect.RemoveAll();

	int centerX, centerZ;
	centerX = (m_area->m_zone->m_zonePos[m_nExtra][1] + m_area->m_zone->m_zonePos[m_nExtra][3])/2/2;
	centerZ = (m_area->m_zone->m_zonePos[m_nExtra][2] + m_area->m_zone->m_zonePos[m_nExtra][4])/2/2;

	if(m_cPlayerCount <= m_cPlayerNum)
	{
		const MOBREGEN* mobregen = proto->FindMobRegen(m_nRegenStep);
		if(!mobregen)
			return 2;
		int i, k;
		CNPC* npc;
		for(i = 0 ; i < mobregen->typecount; ++i)
		{
			for(k = 0; k < mobregen->mobtype[i].count; ++k)
			{
				npc = gserver.m_npcProtoList.Create(mobregen->mobtype[i].npcidx, NULL);
				if(!npc)
					break;
				
				int nTry = 0;
				
				do{
					GET_X(npc) = centerX + GetRandom(-20, 20);
					GET_Z(npc) = centerZ + GetRandom(-20, 20);
					GET_YLAYER(npc) = m_area->m_zone->m_zonePos[m_nExtra][0];
					GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
					if (m_area->GetAttr(GET_YLAYER(npc), GET_X(npc), GET_Z(npc)) == MAPATT_FIELD)
						break;
					nTry++;
				}while(nTry <= 10);

				if(nTry > 10)
				{
					GET_X(npc) = centerX;
					GET_Z(npc) = centerZ;
					GET_YLAYER(npc) = m_area->m_zone->m_zonePos[m_nExtra][0];
					GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
				}
				
				npc->m_regenX = GET_X(npc);
				npc->m_regenY = GET_YLAYER(npc);
				npc->m_regenZ = GET_Z(npc);
				
				int cx, cz;
				m_area->AddNPC(npc);
				m_area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
				m_area->CharToCell(npc, GET_YLAYER(npc), cx, cz);
				
				CNetMsg appearNPCMsg;
				AppearMsg(appearNPCMsg, npc, true);
				m_area->SendToCell(appearNPCMsg, GET_YLAYER(npc), cx, cz);
			}
		}
		return 1;

	}
	else
	{
		m_cPlayerCount = 1;

		m_nRegenStep++;
		if(m_nRegenStep <= proto->m_nRegenCount)
		{
			// npc 리젠
		//	m_nRegenStep = 1;
			const MOBREGEN* mobregen = proto->FindMobRegen(m_nRegenStep);
			if(!mobregen)
				return 2;
			int i, k;
			CNPC* npc;
			for(i = 0 ; i < mobregen->typecount; ++i)
			{
				for(k = 0; k < mobregen->mobtype[i].count; ++k)
				{
					npc = gserver.m_npcProtoList.Create(mobregen->mobtype[i].npcidx, NULL);
					if(!npc)
						break;
					
					int nTry = 0;
					
					do{
						GET_X(npc) = centerX + GetRandom(-20, 20);
						GET_Z(npc) = centerZ + GetRandom(-20, 20);
						GET_YLAYER(npc) = m_area->m_zone->m_zonePos[m_nExtra][0];
						GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
						if (m_area->GetAttr(GET_YLAYER(npc), GET_X(npc), GET_Z(npc)) == MAPATT_FIELD)
							break;
						nTry++;
					}while(nTry <= 10);

					if(nTry > 10)
					{
						GET_X(npc) = centerX;
						GET_Z(npc) = centerZ;
						GET_YLAYER(npc) = m_area->m_zone->m_zonePos[m_nExtra][0];
						GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
					}
					
					npc->m_regenX = GET_X(npc);
					npc->m_regenY = GET_YLAYER(npc);
					npc->m_regenZ = GET_Z(npc);
					
					int cx, cz;
					m_area->AddNPC(npc);
					m_area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
					m_area->CharToCell(npc, GET_YLAYER(npc), cx, cz);
					
					CNetMsg appearNPCMsg;
					AppearMsg(appearNPCMsg, npc, true);
					m_area->SendToCell(appearNPCMsg, GET_YLAYER(npc), cx, cz);
				}
			}
			return 1;
		}
	}

	return 0;
}


const MISSION* CMonsterCombo::FindMission(int step)
{
	int i;
	for(i = 0 ; i < m_nTotalMission; ++i)
	{
		if(m_case[i].step == step)
			return &m_case[i];
	}
	return NULL;
}

bool CMonsterCombo::SetMission(int step, int index)
{
	if(step > m_nTotalMission)
		return false;

	CMissionCaseProto* proto = NULL;
    proto = gserver.m_missionCaseList.FindProto(index);
	if(!proto)
		return false;

	int i;
	for(i = 0; i < m_nTotalMission; ++i)
	{
		if(m_case[i].step == 0)
		{
			m_case[i].step = step;
			m_case[i].proto = proto;
			break;
		}
	}
	if(i == m_nTotalMission)
		return false;

	return true;
}


void CMonsterCombo::RecallToStage()
{
	CNetMsg rmsg;

	CCharacter* pChar;
	CCharacter* pCharNext;

//#ifdef WJKTEST
	float x, z;
	char ylayer;
//#endif // WJKTEST

	int i, j;
	for(i = 0 ; i < m_area->m_size[0]; ++i)
	{
		for(j = 0; j < m_area->m_size[1]; ++j)
		{
			pCharNext = m_area->m_cell[i][j].m_listChar;
			while((pChar = pCharNext))
			{
				pCharNext = pChar->m_pCellNext;
				
				if (IS_PC(pChar) && !DEAD(pChar))
				{
#ifdef GM_GO_ZONE
					if(TO_PC(pChar)->m_gmGoZoneType == GM_GOZONE_COMBO)
						continue;
#endif // GM_GO_ZONE

					if( (int)(pChar->m_pos.m_x) >= gserver.m_comboZone->m_zonePos[m_nExtra + 6][1]/2 &&
						(int)(pChar->m_pos.m_x) <= gserver.m_comboZone->m_zonePos[m_nExtra + 6][3]/2 &&
						(int)(pChar->m_pos.m_z) >= gserver.m_comboZone->m_zonePos[m_nExtra + 6][2]/2 &&
						(int)(pChar->m_pos.m_z) <= gserver.m_comboZone->m_zonePos[m_nExtra + 6][4]/2 &&
						pChar->m_pos.m_yLayer == gserver.m_comboZone->m_zonePos[m_nExtra + 6][0] )
					{
						continue;
					}
//#ifdef WJKTEST
					x = GetRandom(gserver.m_comboZone->m_zonePos[m_nExtra][1], gserver.m_comboZone->m_zonePos[m_nExtra][3]) / 2.0f;
					z = GetRandom(gserver.m_comboZone->m_zonePos[m_nExtra][2], gserver.m_comboZone->m_zonePos[m_nExtra][4]) / 2.0f;
					ylayer = gserver.m_comboZone->m_zonePos[m_nExtra][0];
					GET_R(pChar) = 0.0f;
					GoTo(pChar, ylayer, x, z, pChar->m_pArea->GetHeight(ylayer, x, z), GET_R(pChar));
/*
					GoZone((CPC*)pChar, gserver.m_comboZone->m_index,
						gserver.m_comboZone->m_zonePos[m_nExtra][0],														// ylayer
						GetRandom(gserver.m_comboZone->m_zonePos[m_nExtra][1], gserver.m_comboZone->m_zonePos[m_nExtra][3]) / 2.0f,		// x
						GetRandom(gserver.m_comboZone->m_zonePos[m_nExtra][2], gserver.m_comboZone->m_zonePos[m_nExtra][4]) / 2.0f);		// z
#endif // WJKTEST
*/
				}
			}
		}
	}
}


bool CMonsterCombo::StartStage()
{
	int extra;
	PENALTY penalty;
	void* pos;

	CNPC* pNext;
	CNPC* p;

	int i, k;
	CNPC* npc;

	int centerX, centerZ;
	const MOBREGEN* mobregen;

	const MISSION* mission = FindMission(m_nStage);
	if(!mission)
		goto SKIP_START;

	if(m_nStage > m_nTotalMission)
		goto SKIP_START;

	if(m_bIsBattle)
		goto SKIP_START;

	if(mission->proto->m_nRegenCount == 0)
		goto SKIP_START;

	mobregen = mission->proto->FindMobRegen(m_nRegenStep);
	if(!mobregen)
		goto SKIP_START;

	extra = m_nExtra + 6;

	m_listEffect.RemoveAll();

	// npc 초기화
	pNext = m_area->m_npcList;
	while((p = pNext))
	{
		pNext = p->m_pNPCNext;
		
		if(p->m_proto->m_index != 489)
		{
			p->SendDisappearAllInCell(true);
			
			DelAttackList(p);
			m_area->CharFromCell(p, true);
			m_area->DelNPC(p);	
		}
	}

	// 몹 리젠

	centerX = (m_area->m_zone->m_zonePos[m_nExtra][1] + m_area->m_zone->m_zonePos[m_nExtra][3])/2/2;
	centerZ = (m_area->m_zone->m_zonePos[m_nExtra][2] + m_area->m_zone->m_zonePos[m_nExtra][4])/2/2;
	for(i = 0 ; i < mobregen->typecount; ++i)
	{
		for(k = 0; k < mobregen->mobtype[i].count; ++k)
		{
			npc = gserver.m_npcProtoList.Create(mobregen->mobtype[i].npcidx, NULL);
			if(!npc)
				continue;
			
			int nTry = 0;
			
			do{
				GET_X(npc) = centerX + GetRandom(-20, 20);
				GET_Z(npc) = centerZ + GetRandom(-20, 20);
				GET_YLAYER(npc) = m_area->m_zone->m_zonePos[m_nExtra][0];
				GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
				if (m_area->GetAttr(GET_YLAYER(npc), GET_X(npc), GET_Z(npc)) == MAPATT_FIELD)
					break;
				nTry++;
			}while(nTry <= 10);

			if(nTry > 10)
			{
				GET_X(npc) = centerX;
				GET_Z(npc) = centerZ;
				GET_YLAYER(npc) = m_area->m_zone->m_zonePos[m_nExtra][0];
				GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			}
			
			npc->m_regenX = GET_X(npc);
			npc->m_regenY = GET_YLAYER(npc);
			npc->m_regenZ = GET_Z(npc);
			
			int cx, cz;
			m_area->AddNPC(npc);
			m_area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			m_area->CharToCell(npc, GET_YLAYER(npc), cx, cz);
			
			CNetMsg appearNPCMsg;
			AppearMsg(appearNPCMsg, npc, true);
			m_area->SendToCell(appearNPCMsg, GET_YLAYER(npc), cx, cz);
		}
	}

	// 이펙트 npc
	for(i = 0 ; i < MAXEFFECT; ++i)
	{
		npc = gserver.m_npcProtoList.Create(491, NULL);
		if(!npc)
			goto SKIP_START;
		
		GET_X(npc) = (m_area->m_zone->m_zonePos[m_nExtra][1] + m_area->m_zone->m_zonePos[m_nExtra][3])/2/2;
		GET_Z(npc) = (m_area->m_zone->m_zonePos[m_nExtra][2] + m_area->m_zone->m_zonePos[m_nExtra][4])/2/2;
		GET_YLAYER(npc) = m_area->m_zone->m_zonePos[m_nExtra][0];
		GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		
		npc->m_regenX = GET_X(npc);
		npc->m_regenY = GET_YLAYER(npc);
		npc->m_regenZ = GET_Z(npc);
		
		int cx, cz;
		m_area->AddNPC(npc);
		m_area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
		m_area->CharToCell(npc, GET_YLAYER(npc), cx, cz);
		
		CNetMsg appearNPCMsg;
		AppearMsg(appearNPCMsg, npc, true);
		m_area->SendToCell(appearNPCMsg, GET_YLAYER(npc), cx, cz);
		
		m_effectNPC[i] = npc;
	}

	// 패널티 적용
	
	pos = mission->proto->m_listPanalty.GetHead();
	while(pos)
	{
		penalty = mission->proto->m_listPanalty.GetData(pos);
		ApplyPenalty(penalty.index, penalty.level);

		pos = mission->proto->m_listPanalty.GetNext(pos);
	}

	m_nRegenStep = 1;

	// 플레이어 강제소환
	RecallToStage();

	m_cPlayerCount = 1;

	m_cPlayerNum = m_area->CountPCInExtra(m_nExtra + 6, false);
	if(m_cPlayerNum < 1)
		goto SKIP_START;

	m_pulseFirst	= gserver.m_pulse + PULSE_REAL_SEC * 10;
	m_pulseSecond	= m_pulseFirst + PULSE_REAL_SEC * 2;

	NoticeStage();	

	GAMELOG << init("COMBO START ")
			<< "AREA INDEX: " << m_area->m_index
			<< " COMBO INDEX: " << m_area->m_monsterCombo->m_nIndex;

	PrintCharIndex();

	GAMELOG << end;

	m_bIsBattle = true;

	return true;
	
SKIP_START:
	{
		CNPC* pNext = m_area->m_npcList;
		CNPC* p;
		while((p = pNext))
		{
			pNext = p->m_pNPCNext;
			
			if(p->m_proto->m_index != 489)
			{
				p->SendDisappearAllInCell(true);
				
				DelAttackList(p);
				m_area->CharFromCell(p, true);
				m_area->DelNPC(p);	
			}
		}
	}
	return false;
	
}

void CMonsterCombo::NoticeStage()
{
	CCharacter* pChar;
	CCharacter* pCharNext;
	
	CNetMsg rmsg;
	MonsterComboMsg(rmsg, MSG_EX_MONSTERCOMBO_NOTICE_STAGE);
	rmsg << m_nStage;

	int i, j;
	for(i = 0 ; i < m_area->m_size[0]; ++i)
	{
		for(j = 0; j < m_area->m_size[1]; ++j)
		{
			pCharNext = m_area->m_cell[i][j].m_listChar;
			while((pChar = pCharNext))
			{
				pCharNext = pChar->m_pCellNext;
				
				if (IS_PC(pChar) && !DEAD(pChar))
				{
					
					int extra = m_nExtra + 6;
					
					if( (int)(pChar->m_pos.m_x) >= gserver.m_comboZone->m_zonePos[extra][1]/2 &&
						(int)(pChar->m_pos.m_x) <= gserver.m_comboZone->m_zonePos[extra][3]/2 &&
						(int)(pChar->m_pos.m_z) >= gserver.m_comboZone->m_zonePos[extra][2]/2 &&
						(int)(pChar->m_pos.m_z) <= gserver.m_comboZone->m_zonePos[extra][4]/2 &&
						pChar->m_pos.m_yLayer == gserver.m_comboZone->m_zonePos[extra][0] )
					{
						SEND_Q(rmsg, TO_PC(pChar)->m_desc);
					}
				}
			}
		}
	}
	
}

void CMonsterCombo::ApplyPenalty(int skillindex, int skilllevel)
{
	CSkill* skill;
	skill = gserver.m_skillProtoList.Create(skillindex, skilllevel);

	bool bApply;
	CCharacter* pChar;
	CCharacter* pCharNext;

	int i, j;
	for(i = 0 ; i < m_area->m_size[0]; ++i)
	{
		for(j = 0; j < m_area->m_size[1]; ++j)
		{
			pCharNext = m_area->m_cell[i][j].m_listChar;
			while((pChar = pCharNext))
			{
				pCharNext = pChar->m_pCellNext;
				
				if (IS_PC(pChar) && !DEAD(pChar))
				{	
					int extra = m_nExtra + 6;
					
					if( (int)(pChar->m_pos.m_x) >= gserver.m_comboZone->m_zonePos[extra][1]/2 &&
						(int)(pChar->m_pos.m_x) <= gserver.m_comboZone->m_zonePos[extra][3]/2 &&
						(int)(pChar->m_pos.m_z) >= gserver.m_comboZone->m_zonePos[extra][2]/2 &&
						(int)(pChar->m_pos.m_z) <= gserver.m_comboZone->m_zonePos[extra][4]/2 &&
						pChar->m_pos.m_yLayer == gserver.m_comboZone->m_zonePos[extra][0] )
					{
						ApplySkill(pChar, pChar, skill, -1, bApply);
					}
				}
			}
		}
	}
	
	delete skill;
	skill = NULL;
}


void CMonsterCombo::ClearPenalty()
{
	CCharacter* pChar;
	CCharacter* pCharNext;

	int i, j;
	for(i = 0 ; i < m_area->m_size[0]; ++i)
	{
		for(j = 0; j < m_area->m_size[1]; ++j)
		{
			pCharNext = m_area->m_cell[i][j].m_listChar;
			while((pChar = pCharNext))
			{
				pCharNext = pChar->m_pCellNext;

				if(IS_PC(pChar))
					pChar->m_assist.CureAssist(SF_COMBO); // 콤보존과 관련된 assist 모두 삭제
			}
		}
	}
}


void CMonsterCombo::GiftMobRegen()
{
	if(m_nStage <= 0 || (m_nStage % 5) != 0)
		return ;
		
	int i;
	int count = m_area->CountPCInExtra(m_nExtra + 6, false);
	int point = GetTotalPoint();
	int itemidx;
	int npcidx = 492;
	int centerX, centerZ;
	int maxcoin;
	int giftregencount;

	CNPC* npc;
	if(count <= 0 || point <= 0)
		return ;

	maxcoin = 3 * GetMaxCoinCount(count);

	centerX = (m_area->m_zone->m_zonePos[m_nExtra][1] + m_area->m_zone->m_zonePos[m_nExtra][3])/2/2;
	centerZ = (m_area->m_zone->m_zonePos[m_nExtra][2] + m_area->m_zone->m_zonePos[m_nExtra][4])/2/2;

	if(point <= 70)       itemidx = 2710;
	else if(point <= 120) itemidx = 2711;
	else if(point <= 160) itemidx = 2712;
	else if(point <= 180) itemidx = 2713;
#if defined (MONSTER_COMBO_GOLDBOX)
	else if(point < 280) itemidx = 2714;
	else itemidx = 3575;
#else
	else				  itemidx = 2714;
#endif // MONSTER_COMBO_GOLDBOX

	giftregencount = 0;
	for(i = 0 ; i < count; ++i)
	{	
		npc = gserver.m_npcProtoList.Create(npcidx, NULL);
		if(!npc)
		{
			return ;
		}

		npc->m_coinidx		= itemidx;
		npc->m_coincount	= GetRandom(1, maxcoin);

		int nTry = 0;

		do{
			GET_X(npc) = centerX + (GetRandom(0, 1) ? -1 : 1) * GetRandom(1, 10);
			GET_Z(npc) = centerZ + (GetRandom(0, 1) ? -1 : 1) * GetRandom(1, 10);
			GET_YLAYER(npc) = m_area->m_zone->m_zonePos[m_nExtra][0];
			GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			if (m_area->GetAttr(GET_YLAYER(npc), GET_X(npc), GET_Z(npc)) == MAPATT_FIELD)
				break;
			nTry++;
		}while(nTry <= 10);

		if( nTry > 10 )
		{
			GET_X(npc) = centerX;
			GET_Z(npc) = centerZ;
			GET_YLAYER(npc) = m_area->m_zone->m_zonePos[m_nExtra][0];
			GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		}

		npc->m_regenX = GET_X(npc);
		npc->m_regenY = GET_YLAYER(npc);
		npc->m_regenZ = GET_Z(npc);

		int cx, cz;
		m_area->AddNPC(npc);
		m_area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
		m_area->CharToCell(npc, GET_YLAYER(npc), cx, cz);

		CNetMsg appearNPCMsg;
		AppearMsg(appearNPCMsg, npc, true);
		m_area->SendToCell(appearNPCMsg, GET_YLAYER(npc), cx, cz);

		giftregencount++;
	}

	GAMELOG << init("GIFT NPC REGEN")
			<< "STAGE: " << m_nStage << " "
			<< "PC Count: " << count << " "
			<< "GIFT Count: " << giftregencount << " "
			<< "Area Index: " << m_area->m_index << " "
			<< "Combo Index: " << m_nIndex << " ";
	PrintCharIndex();
	GAMELOG << end;
}

int CMonsterCombo::GetTotalPoint()
{
	int total = 0;

	int i;
	for(i = 0 ; i < m_nTotalMission; ++i)
	{
		total += m_case[i].proto->m_nPoint;
	}

	return total;
}

void CMonsterCombo::FireEffectReady()
{
	int centerX, centerZ;
	int effectCount;
	int extra;

	extra = m_nExtra + 6;
	m_listEffect.RemoveAll();
	centerX = (m_area->m_zone->m_zonePos[m_nExtra][1] + m_area->m_zone->m_zonePos[m_nExtra][3])/2/2;
	centerZ = (m_area->m_zone->m_zonePos[m_nExtra][2] + m_area->m_zone->m_zonePos[m_nExtra][4])/2/2;

	effectCount = GetRandom(1, MAXEFFECT);

	int*   tempNPC = NULL;
	float* tempX = NULL;
	float* tempZ = NULL;
	float* tempH = NULL;


	EFFECTPOS effectpos;

	tempNPC = new int[effectCount];
	tempX = new float[effectCount];
	tempZ = new float[effectCount];
	tempH = new float[effectCount];

	int i;
	for(i = 0; i < effectCount; ++i)
	{
		float x, z, h;
		char ylayer = 0;
		int nTry = 0;

		if(m_effectNPC[i] == NULL)
			continue;

		do{
			x = centerX + GetRandom(-30, 30);
			z = centerZ + GetRandom(-30, 30);
			h = m_area->GetHeight(ylayer, x, z);
		
			if (m_area->GetAttr(ylayer, x, z) == MAPATT_FIELD)
				break;
			nTry++;
			
		}while(nTry <= 10);
		
		if( nTry > 10 )
		{
			x		= centerX;
			z		= centerZ;
			h = m_area->GetHeight(ylayer, x, z);
		}

		effectpos.npc = m_effectNPC[i];
		effectpos.x	  = x;
		effectpos.z	  = z;
		effectpos.h	  = h;

		m_listEffect.AddToTail(effectpos);

		tempNPC[i] = m_effectNPC[i]->m_index;
		tempX[i]  = x;
		tempZ[i]  = z;
		tempH[i]  = h;
	}
	
	if(effectCount > 0 && tempNPC && tempX && tempZ && tempH)
	{
		CNetMsg rmsg;
		EffectFireReadyMsg(rmsg, m_nExtra, effectCount, tempNPC, tempX, tempZ, tempH);
		m_area->SendToAllClient(rmsg);
	}

	if(tempNPC) delete[] tempNPC;
	if(tempX) delete[] tempX;
	if(tempZ) delete[] tempZ;
	if(tempH) delete[] tempH;
}

void CMonsterCombo::FireEffectReady2()
{
	int i, j;
	int centerX, centerZ;
	int effectCount;
	int extra;

	int randDistance, randDir;
	int rand;

	float x, z, h;
	char ylayer = 0;

	int*   tempNPC = NULL;
	float* tempX = NULL;
	float* tempZ = NULL;
	float* tempH = NULL;

	CPC* pc[MAX_PARTY_MEMBER];
	memset(pc, 0, sizeof(CPC*) * MAX_PARTY_MEMBER);

	for(i = 0 ; i < MAXEFFECT ; ++i)
	{
		if(m_effectNPC[i] == NULL)
		{
			m_listEffect.RemoveAll();
			GAMELOG << init("MONSTER COMBO FIREREADY ERROR: ")
					<< i
					<< end;
			return ;
		}
	}

	extra = m_nExtra + 6;
	m_listEffect.RemoveAll();
	centerX = (m_area->m_zone->m_zonePos[m_nExtra][1] + m_area->m_zone->m_zonePos[m_nExtra][3])/2/2;
	centerZ = (m_area->m_zone->m_zonePos[m_nExtra][2] + m_area->m_zone->m_zonePos[m_nExtra][4])/2/2;

	EFFECTPOS effectpos;

	effectCount = GetRandom(1, MAXEFFECT);

	tempNPC = new int[effectCount];
	tempX = new float[effectCount];
	tempZ = new float[effectCount];
	tempH = new float[effectCount];

	int pccount = 0;
	CCharacter* pChar;
	CCharacter* pCharNext;
	for(i = 0; i < m_area->m_size[0]; ++i)
	{
		for(j = 0 ; j < m_area->m_size[1]; ++j)
		{
			pCharNext = m_area->m_cell[i][j].m_listChar;
			while((pChar = pCharNext))
			{
				pCharNext = pChar->m_pCellNext;
				if (IS_PC(pChar) && !DEAD(pChar))
				{
					// Immortal 혹은 Visible 이면 데미지 안먹음
					if(TO_PC(pChar)->m_bVisible == false || TO_PC(pChar)->m_bImmortal)
						continue;

					if( ((int)(pChar->m_pos.m_x) >= pChar->m_pZone->m_zonePos[extra][1]/2) && ((int)(pChar->m_pos.m_x) <= pChar->m_pZone->m_zonePos[extra][3]/2) &&
						((int)(pChar->m_pos.m_z) >= pChar->m_pZone->m_zonePos[extra][2]/2) && ((int)(pChar->m_pos.m_z) <= pChar->m_pZone->m_zonePos[extra][4]/2) &&
						((int)(pChar->m_pos.m_yLayer) == pChar->m_pZone->m_zonePos[extra][0]))
					{
						pc[pccount] = TO_PC(pChar);
						pccount++;
						
						if( pccount >= MAX_PARTY_MEMBER )
							break;
					}
				}
			}
		}
	}

	if(pccount <= 0)
		goto SKIP;

	for(i = 0; i < effectCount; ++i)
	{
		if(m_effectNPC[i] == NULL)
			goto SKIP;

		rand = GetRandom(0, pccount - 1);
		if(!pc[rand])
			goto SKIP;

		randDistance = GetRandom(0, 10);
		randDir = GetRandom(0, (int) (PI_2 * 10000)) / 10000;

		x = GET_X(pc[rand]) + (float)randDistance * (float)cos(randDir);
		z = GET_Z(pc[rand]) + (float)randDistance * (float)sin(randDir);
		h = m_area->GetHeight(ylayer, x, z);

		effectpos.npc = m_effectNPC[i];
		effectpos.x	  = x;
		effectpos.z	  = z;
		effectpos.h	  = h;

		m_listEffect.AddToTail(effectpos);

		tempNPC[i] = m_effectNPC[i]->m_index;
		tempX[i]  = x;
		tempZ[i]  = z;
		tempH[i]  = h;
	}
 
	if(effectCount > 0 && tempNPC && tempX && tempZ && tempH)
	{
		CNetMsg rmsg;
		EffectFireReadyMsg(rmsg, m_nExtra, effectCount, tempNPC, tempX, tempZ, tempH);
		m_area->SendToAllClient(rmsg);
	}

	if(tempNPC) delete[] tempNPC;
	if(tempX) delete[] tempX;
	if(tempZ) delete[] tempZ;
	if(tempH) delete[] tempH;
	memset(pc, 0, sizeof(CPC*) * MAX_PARTY_MEMBER);
	
	return;

SKIP:
	if(tempNPC) delete[] tempNPC;
	if(tempX) delete[] tempX;
	if(tempZ) delete[] tempZ;
	if(tempH) delete[] tempH;
	memset(pc, 0, sizeof(CPC*) * MAX_PARTY_MEMBER);
	m_listEffect.RemoveAll();
}

void CMonsterCombo::FireEffectAct()
{
	CNetMsg rmsg;

	int i, j;
	CCharacter* tch;
	CCharacter* tchNext;
	EFFECTPOS effect;
	
	for(i = 0 ; i < MAXEFFECT ; ++i)
	{
		if(m_effectNPC[i] == NULL)
		{
			m_listEffect.RemoveAll();
			GAMELOG << init("MONSTER COMBO FIREACT ERROR: ")
					<< i
					<< end;
			return ;
		}
	}

	void* pos = m_listEffect.GetHead();
	while(pos)
	{
		effect = (EFFECTPOS)(m_listEffect.GetData(pos));

		for(i = 0 ; i < m_area->m_size[0]; ++i)
		{
			for(j = 0; j < m_area->m_size[1]; ++j)
			{
				tchNext = m_area->m_cell[i][j].m_listChar;
				while((tch = tchNext))
				{
					tchNext = tch->m_pCellNext;
					if (IS_PC(tch) && !DEAD(tch))
					{
						// Immortal 혹은 Visible 이면 데미지 안먹음
						if( TO_PC(tch)->m_bVisible == false || TO_PC(tch)->m_bImmortal)
							continue;
						
						// tch 데미지 적용
						if(GetDistance(effect.x, effect.z, effect.h, tch) < 4.0f)
						{
							int damage;
							damage = (int)(tch->m_maxHP * 0.1);
							
							if(effect.npc && damage > 0)
							{
								tch->m_hp -= damage;
								if(tch->m_hp <= 0)
									tch->m_hp = 0;

								if (DEAD(tch))
									ProcDead(TO_PC(tch), effect.npc);

								((CPC*)tch)->m_bChangeStatus = true;
								
								DamageMsg(rmsg, effect.npc, tch, MSG_DAMAGE_COMBO, -1, damage, HITTYPE_NORMAL);							
								m_area->SendToCell(rmsg, tch, true);
							}
						}
					}
				}
			}
		}

		pos = m_listEffect.GetNext(pos);
	}

	m_listEffect.RemoveAll();
}

int CMonsterCombo::GetTotalNas()
{
	int total = 0;

	int i;
	for(i = 0 ; i < m_nTotalMission; ++i)
	{
		total += m_case[i].proto->m_nNas;
	}

	return total;
}

void CMonsterCombo::ComboNPCRegen(/*char bContinue*/)
{
	int extra;
	extra = m_nExtra;

	CNPC* npc;
	npc = gserver.m_npcProtoList.Create(490, NULL);
	if(!npc)
	{
		return ;
	}
	
	int centerX, centerZ;
	centerX = (m_area->m_zone->m_zonePos[extra][1] + m_area->m_zone->m_zonePos[extra][3])/2/2;
	centerZ = (m_area->m_zone->m_zonePos[extra][2] + m_area->m_zone->m_zonePos[extra][4])/2/2;

	GET_X(npc) = centerX;
	GET_Z(npc) = centerZ;
	GET_YLAYER(npc) = m_area->m_zone->m_zonePos[extra][0];
	GET_R(npc) = 0;

	npc->m_regenX = GET_X(npc);
	npc->m_regenY = GET_YLAYER(npc);
	npc->m_regenZ = GET_Z(npc);
	
	int cx, cz;
	m_area->AddNPC(npc);
	m_area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
	m_area->CharToCell(npc, GET_YLAYER(npc), cx, cz);
	
	CNetMsg appearNPCMsg;
	AppearMsg(appearNPCMsg, npc, true);
	m_area->SendToCell(appearNPCMsg, GET_YLAYER(npc), cx, cz);
}

void CMonsterCombo::SetNextVar()
{
	m_bIsBattle = false;
	m_nStage++;
	m_nRegenStep = 1;
	m_nExtra++;
	if(m_nExtra > 5)
		m_nExtra -= 5;

	m_cPlayerCount = 1;
	m_cPlayerNum = 1;
	m_listEffect.RemoveAll();
}

void CMonsterCombo::InitStageVar()
{
	m_bIsBattle = false;
	m_nRegenStep = 1;
	m_cPlayerCount = 1;
	m_cPlayerNum = 1;
	m_listEffect.RemoveAll();
}

void CMonsterCombo::SetVirtualIndex()
{
	++m_virtualIndex;
	m_nIndex = m_virtualIndex;
}


void CMonsterCombo::PrintCharIndex()
{
	int i, j;
	CCharacter* tch;
	CCharacter* tchNext;
	
	GAMELOG << " CharIndex: ";
	for(i = 0 ; i < m_area->m_size[0]; ++i)
	{
		for(j = 0; j < m_area->m_size[1]; ++j)
		{
			tchNext = m_area->m_cell[i][j].m_listChar;
			while((tch = tchNext))
			{
				tchNext = tch->m_pCellNext;
				if(IS_PC(tch))
				{
					GAMELOG << tch->m_index
							<< " ";
				}
			}
		}
	}
}

void CMonsterCombo::InitAllVar()
{
	m_bIsBattle		= false;
	m_nExtra		= 0;
	m_nStage		= 1;
	m_nRegenStep	= 1;
	m_pulseFirst	= 0;
	m_pulseSecond	= 0;
	m_nTotalMission = 0;
	m_cPlayerCount = 1;
	m_cPlayerNum   = 1;
	m_nIndex = -1;
	memset(m_effectNPC, 0, sizeof(CNPC*) * MAXEFFECT);
	m_listEffect.RemoveAll();

	if(m_case)
	{
		delete [] m_case;
		m_case = NULL;
	}
}

bool CMonsterCombo::SetMonsterCombo(CPC* pc)
{
	int count;
	if(!pc)
		return false;

	if(!m_area)
		return false;

	if(!m_area->m_zone->IsComboZone())
		return false;

	// 설정할 미션케이스 개수 검사
	count = pc->m_listSaveComboData.GetCount();
	if(count < 1 || count > 20)
		return false;

	// 모든 변수 초기화
	InitAllVar();

	// 미션케이스 설정
	if(!this->Init(count))
	{
		InitAllVar();
		return false;
	}
	
	void* pos;	
	void* nextpos = pc->m_listSaveComboData.GetHead();
	MONSTER_COMBO_DATA data;
	while((pos = nextpos))
	{
		nextpos = pc->m_listSaveComboData.GetNext(pos);
		data = pc->m_listSaveComboData.GetData(pos);
		if(!SetMission((data.nStep + 1), data.nIndex))
		{
			InitAllVar();
			return false;
		}
	}

	// 콤보 인덱스 설정
	SetVirtualIndex();
	return true;
}

int CMonsterCombo::m_virtualIndex = 0;

#endif // MONSTER_COMBO