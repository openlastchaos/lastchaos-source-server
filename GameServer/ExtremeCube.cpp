#include "stdhdrs.h"
#include "Server.h"
#include "Character.h"
#include "CmdMsg.h"
#include "ExtremeCube.h"
#include "DBCmd.h"
#include "doFunc.h"

#ifdef EXTREME_CUBE

//////////  CCubeMemList  ///////////////
CCubeMemList::CCubeMemList() : m_pcList(NULL)
{
	m_index = -1;
	m_type = -1;
	m_cubenum = -1;
	m_clearCount = 0;
	m_bEnable = false;
	m_pcList.RemoveAll();
}


CCubeMemList::~CCubeMemList()
{

}


bool CCubeMemList::JoinCube(CPC* pc, int extra)
{
	CNetMsg rmsg;
	int i,itemidx;
	CZone* pZone;
	i = gserver.FindZone(ZONE_EXTREME_CUBE);
	if(i == -1)
		return false;
	pZone = gserver.m_zones + i;

	if(extra < 0 || extra > BOSSCUBE_START - 1)
		return false;

	switch(m_type)
	{
	case CUBEMEM_PARTY:	itemidx = 2986; break;
	case CUBEMEM_GUILD:	itemidx = 2987; break;
	default:
		return false;
	}

	int row, col;
	if(pc->m_invenNormal.FindItem(&row, &col, itemidx, 0, 0))
	{
		GoZone(pc, ZONE_EXTREME_CUBE, pZone->m_zonePos[extra][0],
			GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
			GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z

		if(pc->m_pZone->m_index != ZONE_EXTREME_CUBE)
			return false;
	
		CItem* item;
		item = pc->m_invenNormal.GetItem( row, col );
		DecreaseFromInventory( pc, item, 1 );
		if( item->Count() > 0 )
		{
			ItemUpdateMsg( rmsg, item, -1 );
			SEND_Q( rmsg, pc->m_desc );
		}
		else
		{
			ItemDeleteMsg( rmsg, item );
			SEND_Q( rmsg, pc->m_desc );
			RemoveFromInventory( pc, item, true, true );
		}
		ExtremeCubeErrorMsg( rmsg, (m_type == CUBEMEM_PARTY) ? MSG_EX_EXTREME_CUBE_ERROR_PARTYCUBE : MSG_EX_EXTREME_CUBE_ERROR_GUILDCUBE);
		SEND_Q( rmsg, pc->m_desc);
	}
	else	
	{
		// 아이템 없다한다.
		ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_NOITEM);
		SEND_Q(rmsg, pc->m_desc);
		return false;
	}	
	return true;
}

void CCubeMemList::DelPC(CPC* pc)
{
	CNetMsg msg;
	int i;
	m_pcList.RemoveData(pc);
	if(m_pcList.GetCount() == 0)
	{
		if(IsGuildCubeMemList())
		{
			CGuild* guild = ((CGuildCubeMemList*)this)->GetGuild();
			if(guild)
			{
				for(i = 0; i < GUILD_MAX_MEMBER; i++)
				{
					CGuildMember* guildmember = guild->member(i);
					if(guildmember && guildmember->GetPC())
					{
						CPC* pc = guildmember->GetPC();
						if(DEAD(pc))
						{
#if defined(LC_USA) 							
							//부활은 클라이언트에 부활 요청 패킷을 전송 하여 서버에서 처리 하므로 
							//서버에서 부활을 처리 하면 오류(아이템 844,845,846 사용 안됨) 
							//미국만 아이템 844,845,846 사용.
#else
							msg.Init(MSG_PC_REBIRTH);
							do_Rebirth(pc, msg);
#endif

						}
					}
				}
			}
		}
		else if(IsPartyCubeMemList())
		{
			CParty* party = ((CPartyCubeMemList*)this)->GetParty();
			if(party)
			{
				for(i = 0; i < MAX_PARTY_MEMBER; i++)
				{
					const CPartyMember* partymember = party->GetMemberByListIndex(i);
					if(partymember && partymember->GetMemberPCPtr())
					{
						CPC* pc = partymember->GetMemberPCPtr();
						if(pc->m_pZone != NULL && DEAD(pc) && pc->m_pZone->IsExtremeCube())
						{
#if defined(LC_USA) 							
							//부활은 클라이언트에 부활 요청 패킷을 전송 하여 서버에서 처리 하므로 
							//서버에서 부활을 처리 하면 오류(아이템 844,845,846 사용 안됨) 
							//미국만 아이템 844,845,846 사용.
#else
						    msg.Init(MSG_PC_REBIRTH);
							do_Rebirth(pc, msg);
#endif
						}
					}
				}
			}
		}
	}
}

bool CCubeMemList::MoveToOtherCube(int cubeidx)
{
	void* pos;
	CPC* pc;
	int k, zone, extra, oldcubeidx;
	CCubeSpace* space;
	CCubeSpace* old_space;
	CZone* pZone;
	zone = ZONE_EXTREME_CUBE;

	oldcubeidx = m_cubenum;

	old_space = gserver.m_extremeCube.GetExtremeCube(oldcubeidx);
	if(!old_space)
	{
		GAMELOG << init("FAILED MOVE CUBE 1") << "prev cubeidx" << delim << oldcubeidx << end;
		return false;
	}

	if(cubeidx < 0 
	|| cubeidx >= MAX_CUBESPACE
	|| oldcubeidx < 0
	|| oldcubeidx >= MAX_CUBESPACE)
	{
		GAMELOG << init("FAILED MOVE CUBE 2") << "prev cubeidx" << delim << oldcubeidx << delim
				<< "new cubeidx" << delim << cubeidx << end;
		return false;
	}

	k = gserver.FindZone(zone);
	if(k == -1)
		return false;

	pZone = gserver.m_zones + k;
	
	space = gserver.m_extremeCube.GetExtremeCube(cubeidx);

	if(!space)
	{
		GAMELOG << init("FAILED MOVE CUBE 3") << end;
		return false;
	}

	if(space->m_extra < 0 || space->m_extra >= pZone->m_countZonePos)
	{
		GAMELOG << init("FAILED MOVE CUBE 4") << "extra" << delim << space->m_extra << end;
		return false;
	}

	if(!space->SetMemList(this))
	{
		GAMELOG << init("FAILED MOVE CUBE 5") << "prev cubeidx" << delim << oldcubeidx << delim
				<< "new cubeidx" << delim << cubeidx << end;

		m_cubenum = oldcubeidx;
		return false;
	}

	if(!old_space->DelMemList(this))
	{
		space->DelMemList(this);
		GAMELOG << init("FAILED MOVE CUBE 6") << end;
		return false;
	}

	extra = space->m_extra;

	pos = m_pcList.GetHead();
	while(pos)
	{
		pc = (CPC*)(m_pcList.GetData(pos));
		pos = m_pcList.GetNext(pos);

		GoZone(pc, zone, 
			pZone->m_zonePos[extra][0],
			GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
			GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
	}

	return true;
}

void CCubeMemList::GetOutAll()
{
	void* pos;
	CPC* pc;
	CZone* pZone;
	int zone = ZONE_MERAC;
	int extra = 0;
	int i = gserver.FindZone(zone);
	if(i == -1)
		return ;

	pZone = gserver.m_zones + i;
	pos = m_pcList.GetHead();
	while(pos)
	{
		pc = (CPC*)(m_pcList.GetData(pos));
		pos = m_pcList.GetNext(pos);

		GoZone(pc, zone,
			pZone->m_zonePos[extra][0],
			GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
			GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
	}

	m_pcList.RemoveAll();
}

bool CCubeMemList::FindPC(CPC* pc)
{
	if( !pc )
		return false;
	
	void* pos = NULL;
	pos = m_pcList.FindData(pc);
	if(!pos)
		return false;

	return true;
}


void CCubeMemList::SendCompleteCount()
{
	void* pos;
	CPC* pc;

	CNetMsg msg;
	ExtremeCubeMsg(msg, MSG_EX_EXTREME_CUBE_COMPLETE);
	msg << m_clearCount;

	pos = m_pcList.GetHead();
	while(pos)
	{
		pc = (CPC*)(m_pcList.GetData(pos));
		pos = m_pcList.GetNext(pos);
		SEND_Q(msg, pc->m_desc);
	}
}

int CCubeMemList::GetPersonalCubePoint(CPC* pc, int level)
{
	
	if(!pc)
	{
		GAMELOG << init("PERSONAL CUBEPOINT ERROR") << "NULL" << delim << level << end;
		return 0;
	}

    if(!FindPC(pc))
	{
		GAMELOG << init("PERSONAL CUBEPOINT ERROR", pc) << "NOT FOUND PC" << delim 
				<< "CUBEIDX" << delim << GetUniqueIdx() 
				<< "TargetLEVEL" << delim << level << end;
		return 0;
	}

	int TotalLevel = 0;
	CPC* cubemember;
	void* pos;
	int point;

	pos = m_pcList.GetHead();
	while(pos)
	{
		cubemember = (CPC*)(m_pcList.GetData(pos));
		pos = m_pcList.GetNext(pos);

		TotalLevel += cubemember->m_level;
	}

	if(TotalLevel < 1)
	{
		GAMELOG << init("PERSONAL CUBEPOINT ERROR", pc) << "TOTALLEVEL" << delim << TotalLevel << delim
				<< "CUBEIDX" << delim << GetUniqueIdx() 
				<< "TargetLEVEL" << delim << level << end;
		return 0;
	}

	point = level * 10 * pc->m_level / TotalLevel;

	if(point == 0)
	{
		GAMELOG << init("PERSONAL CUBEPOINT ZERO", pc) 
				<< "LEVEL" << delim << pc->m_level << delim
				<< "TOTALLEVEL" << delim << TotalLevel << delim
				<< "CUBEIDX" << delim << GetUniqueIdx() 
				<< "TargetLEVEL" << delim << level << end;
	}

	return point;
}

int CCubeMemList::m_uniqueidx = 0;

CPartyCubeMemList::CPartyCubeMemList()
{
	m_type = CUBEMEM_PARTY;
	m_party = NULL;
}

CPartyCubeMemList::~CPartyCubeMemList()
{
}

void CPartyCubeMemList::Enable(CParty* party, int cubenum)
{
	m_party = party;
	m_uniqueidx++;
	m_index = m_uniqueidx;
	party->m_cubeUniqueIdx = m_uniqueidx;
	m_cubenum = cubenum;
	m_clearCount = 0;
	m_bEnable = true;
	m_pcList.RemoveAll();
}

void CPartyCubeMemList::Disable()
{
	if(m_party)
	{
		m_party->m_cubeUniqueIdx = -1;
		m_party = NULL;
	}
	m_index = -1;
	m_cubenum = -1;
	m_clearCount = 0;
	m_bEnable = false;
	m_pcList.RemoveAll();
}

bool CPartyCubeMemList::IsBreak()
{
	return (m_party != NULL) ? true : false;
}

CGuildCubeMemList::CGuildCubeMemList()
{
	m_type = CUBEMEM_GUILD;
	m_guild = NULL;
}

CGuildCubeMemList::~CGuildCubeMemList()
{
}

void CGuildCubeMemList::Enable(CGuild* guild, int cubenum)
{
	m_guild = guild;
	m_uniqueidx++;
	m_index = m_uniqueidx;
	m_guild->m_cubeUniqueIdx = m_uniqueidx;
	m_cubenum = cubenum;
	m_clearCount = 0;
	m_bEnable = true;
	m_pcList.RemoveAll();
}

void CGuildCubeMemList::Disable()
{
	if(m_guild)
	{
		m_guild->m_cubeUniqueIdx = -1;
		m_guild = NULL;
	}
	m_index = -1;
	m_cubenum = -1;
	m_clearCount = 0;
	m_bEnable = false;
	m_pcList.RemoveAll();
}


bool CGuildCubeMemList::IsBreak()
{
	return (m_guild != NULL) ? true : false;
}



/////////// CCubeSpace //////////////////
CCubeSpace::CCubeSpace()
{
	m_type		= -1;
	m_crystal	= NULL;
	m_waitTime	= 0;

	m_crystalkind[CUBESTATE_RED] = gserver.m_npcProtoList.Create(CUBE_RED, NULL);
	m_crystalkind[CUBESTATE_ORANGE] = gserver.m_npcProtoList.Create(CUBE_ORANGE, NULL);
	m_crystalkind[CUBESTATE_BLUE] = gserver.m_npcProtoList.Create(CUBE_BLUE, NULL);
	m_crystalkind[CUBESTATE_PURPLE] = gserver.m_npcProtoList.Create(CUBE_PURPLE, NULL);
	m_crystalkind[CUBESTATE_YELLOW] = gserver.m_npcProtoList.Create(CUBE_YELLOW, NULL);
}

CCubeSpace::~CCubeSpace()
{
	int i;
	for(i = 0; i < CRYSTALCOUNT; ++i)
	{
		if(m_crystalkind[i] && (m_crystalkind[i] != m_crystal) )
		{
			delete m_crystalkind[i];
			m_crystalkind[i] = NULL;
		}
	}

	m_crystal = NULL;
}

void CCubeSpace::ChageState(CUBESTATE state)
{
	switch(state)
	{
	case CUBESTATE_RED:		
	case CUBESTATE_ORANGE:	
	case CUBESTATE_BLUE:
	case CUBESTATE_PURPLE:
	case CUBESTATE_YELLOW:	
		break;
	default:
		return ;
	}

	m_state = state;

	if(m_crystal)	
	{
		DelCrystal(true);
	}

	m_crystal = m_crystalkind[state];  // 만들어 놓은 것을 가져다 씀
	m_crystal->InitPointsToMax();
	if(!m_crystal)
		return ;
	
	GET_X(m_crystal) = (m_area->m_zone->m_zonePos[m_extra][1] + m_area->m_zone->m_zonePos[m_extra][3])/4;
	GET_Z(m_crystal) = (m_area->m_zone->m_zonePos[m_extra][2] + m_area->m_zone->m_zonePos[m_extra][4])/4;
	GET_YLAYER(m_crystal) = m_area->m_zone->m_zonePos[m_extra][0];
	GET_R(m_crystal) = 0;

	m_crystal->m_regenX = GET_X(m_crystal);
	m_crystal->m_regenY = GET_YLAYER(m_crystal);
	m_crystal->m_regenZ = GET_Z(m_crystal);

	int cx, cz;
	m_area->AddNPC(m_crystal);
	m_area->PointToCellNum(GET_X(m_crystal), GET_Z(m_crystal), &cx, &cz);
	m_area->CharToCell(m_crystal, GET_YLAYER(m_crystal), cx, cz);
	
	CNetMsg appearNPCMsg;
	AppearMsg(appearNPCMsg, m_crystal, true);
	m_area->SendToCell(appearNPCMsg, GET_YLAYER(m_crystal), cx, cz);
}

void CCubeSpace::DelCrystal(bool bDisappear)
{
	if(!m_crystal)
		return ;

	if(bDisappear)
		m_crystal->SendDisappearAllInCell(true);

	DelAttackList(m_crystal);
	m_crystal->m_pArea->CharFromCell(m_crystal, true);

	REMOVE_FROM_BILIST(m_crystal, m_crystal->m_pArea->m_npcList, m_pNPCPrev, m_pNPCNext);
	if(m_crystal->m_regenInfo)
	{
		m_crystal->m_regenInfo->m_lastDieTime = gserver.m_pulse;
		m_crystal->m_regenInfo->m_bAlive = false;
	}

	m_crystal->m_pArea->m_nCountNPC--;

	m_crystal->m_pZone = NULL;
	m_crystal->m_pArea = NULL;
	m_crystal = NULL;
}

CBattleSpace::CBattleSpace() : m_mobList(NULL)
{
	m_state = CUBESTATE_RED;
	m_memList1 = NULL;
	m_memList2 = NULL;
	m_type = BATTLE_SPACE;
}


CBattleSpace::~CBattleSpace()
{
	if(m_memList1)
	{
	//	delete m_memList1;
		m_memList1->Disable();
		m_memList1 = NULL;
	}

	if(m_memList2)
	{
	//	delete m_memList2;
		m_memList2->Disable();
		m_memList2 = NULL;
	}

	m_mobList.RemoveAll();
}

void CBattleSpace::CheckCubeState()
{
	CCubeMemList* WinList;
	bool IsMemList1, IsMemList2, IsMob;

	if(m_memList1)
	{
		if(m_memList1->GetPCCount() == 0 || m_memList1->IsBreak() == false)
		{
			GAMELOG << init("CUBE MEMLIST ALL DEAD")
					<< "UNIQUEIDX" << delim
					<< m_memList1->GetUniqueIdx() << delim
					<< "CLEARCNT" << delim
					<< m_memList1->GetWinCount() << delim
					<< "AREAIDX" << delim
					<< m_area->m_index
					<< end;

			m_memList1->GetOutAll();
			m_memList1->Disable();
			m_memList1 = NULL;
			IsMemList1 = false;
		}
		else
		{
			IsMemList1 = true;
		}
	}
	else
		IsMemList1 = false;


	if(m_memList2)
	{
		if(m_memList2->GetPCCount() == 0 || m_memList2->IsBreak() == false)
		{
			GAMELOG << init("CUBE MEMLIST ALL DEAD")
					<< "UNIQUEIDX" << delim
					<< m_memList2->GetUniqueIdx() << delim
					<< "CLEARCNT" << delim
					<< m_memList2->GetWinCount() << delim
					<< "AREAIDX" << delim
					<< m_area->m_index
					<< end;

			m_memList2->GetOutAll();
			m_memList2->Disable();
			m_memList2 = NULL;
			IsMemList2 = false;
		}
		else
		{
			IsMemList2 = true;
		}
	}
	else
		IsMemList2 = false;

	if(m_mobList.GetCount() > 0)
		IsMob = true;
	else
		IsMob = false;


	if(m_state == CUBESTATE_RED)
	{
		if(IsMemList1 && IsMemList2)
		{
			ChageState(CUBESTATE_ORANGE);

			GAMELOG << init("CHANGE CUBE STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "RED => ORANGE"
					<< end;
		}
		else if(WinMemList() && !IsMob)			
		{
			WinList = WinMemList();
			WinList->IncreaseWinCount();

			GAMELOG << init("INCREASE CUBE CREARCNT")
					<< "UNIQUEIDX" << delim
					<< WinList->GetUniqueIdx() << delim
					<< "CLEARCNT" << delim
					<< WinList->GetWinCount() << delim
					<< "AREAIDX" << delim
					<< m_area->m_index << end;

			if(WinList->GetWinCount() >= 25)		// 수정
			{
				ChageState(CUBESTATE_PURPLE);

				GAMELOG << init("CHANGE CUBE STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "RED => PURPLE"
					<< end;
			}
			else
			{
				ChageState(CUBESTATE_BLUE);	
				m_waitTime = gserver.m_pulse + PULSE_REAL_MIN;

				GAMELOG << init("CHANGE CUBE STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "RED => BLUE"
					<< end;
			}

//			WinMemList()->SendCompleteCount();
		}
		else if(!IsMemList1 && !IsMemList2 && IsMob)
		{
			Clean();

			GAMELOG << init("CHANGE CUBE STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "RED => CLEAN"
					<< end;
		}
	}
	else if(m_state == CUBESTATE_ORANGE)
	{
		if( WinMemList() && !IsMob)
		{
			WinList = WinMemList();
			WinList->IncreaseWinCount();

			GAMELOG << init("INCREASE CUBE CREARCNT")
					<< "UNIQUEIDX" << delim
					<< WinList->GetUniqueIdx() << delim
					<< "CLEARCNT" << delim
					<< WinList->GetWinCount() << delim
					<< "AREAIDX" << delim
					<< m_area->m_index << end;

			if(WinList->GetWinCount() >= 25)		// 수정
			{
				ChageState(CUBESTATE_PURPLE);

				GAMELOG << init("CHANGE CUBE STATE")
						<< "AREAIDX" << delim
						<< m_area->m_index << delim
						<< "ORANGE => PURPLE"
						<< end;
			}
			else
			{
				ChageState(CUBESTATE_BLUE);
				m_waitTime = gserver.m_pulse + PULSE_REAL_MIN;

				GAMELOG << init("CHANGE CUBE STATE")
						<< "AREAIDX" << delim
						<< m_area->m_index << delim
						<< "ORANGE => BLUE"
						<< end;
			}

//			WinMemList()->SendCompleteCount();
		}
		else if(!IsMemList1 && !IsMemList2 && !IsMob)
		{
			Clean();

			GAMELOG << init("CHANGE CUBE STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "ORANGE => CLEAN 1"
					<< end;
		}
		else if(!IsMemList1 && !IsMemList2 && IsMob)
		{
			Clean();

			GAMELOG << init("CHANGE CUBE STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "ORANGE => CLEAN 2"
					<< end;
		}
	}
	else if(m_state == CUBESTATE_BLUE)
	{
		if( WinMemList() && !IsMob)
		{
			if(m_crystal)
			{
				if(gserver.m_pulse > m_waitTime)
				{	
					int cubeidx;
					bool bFirst;
					CCubeSpace* cube;
					cubeidx = gserver.m_extremeCube.GetAvailableBattleCube(bFirst);
					cube = gserver.m_extremeCube.GetExtremeCube(cubeidx);
					
					if(cube)
					{
						if(bFirst)
						{
							cube->RegenMob();
						}
						WinMemList()->MoveToOtherCube(cubeidx);
						Clean();
					}
					else
					{
					
						ChageState(CUBESTATE_RED);
						RegenMob();
					}
					m_waitTime = 0;

					GAMELOG << init("CHANGE CUBE STATE")
							<< "AREAIDX" << delim
							<< m_area->m_index << delim
							<< "BLUE => RED 1"
							<< end;
				}
			}
			else
			{
				if(gserver.m_pulse > m_waitTime)
				{
					int cubeidx;
					bool bFirst;
					cubeidx = gserver.m_extremeCube.GetAvailableBattleCube(bFirst);
					CCubeSpace* cube = gserver.m_extremeCube.GetExtremeCube(cubeidx);
					if(cube)
					{
						if(bFirst)
						{
							cube->RegenMob();
						}
						WinMemList()->MoveToOtherCube(cubeidx);
						Clean();
					}
					else
					{
						ChageState(CUBESTATE_RED);
						RegenMob();
					}
					m_waitTime = 0;
					
					GAMELOG << init("CHANGE CUBE STATE")
						<< "AREAIDX" << delim
						<< m_area->m_index << delim
						<< "BLUE => RED 2"
						<< end;
				}
			}
		}
		else if(!IsMemList1 && !IsMemList2 && !IsMob)
		{
			Clean();

			GAMELOG << init("CHANGE CUBE STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "BLUE => CLEAN 1"
					<< end;
		}
		else if(!IsMemList1 && !IsMemList2 && IsMob)
		{
			Clean();

			GAMELOG << init("CHANGE CUBE STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "BLUE => CLEAN 2"
					<< end;
		}
		else
		{
			Clean();

			GAMELOG << init("CHANGE CUBE STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "BLUE => CLEAN 3"
					<< end;
		}
	}
	else if(m_state == CUBESTATE_PURPLE)
	{
		if( WinMemList() && !IsMob)
		{
			if(!m_crystal)
			{
				if(gserver.m_pulse > m_waitTime)
				{
					int cubeidx = gserver.m_extremeCube.GetAvailableBossCube();
					CCubeSpace* cube = gserver.m_extremeCube.GetExtremeCube(cubeidx);
					if(cube)
					{
						cube->RegenMob();
						cube->ChageState(CUBESTATE_ORANGE);
						WinMemList()->MoveToOtherCube(cubeidx);
						Clean();
						
						GAMELOG << init("CHANGE CUBE STATE")
							<< "AREAIDX" << delim
							<< m_area->m_index << delim
							<< "PURPLE => CLEAN"
							<< end;
					}
				}
			}
		}
		else
		{
			Clean();

			GAMELOG << init("CHANGE CUBE STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "PURPLE => CLEAN 2"
					<< end;
		}
	}
	else if(m_state == CUBESTATE_YELLOW)		
	{
		Clean();

		GAMELOG << init("CHANGE CUBE STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "YELLOW => CLEAN"
					<< end;
	}
}


bool CBattleSpace::IsAvailableJoin(bool& bfirst)
{
	bfirst = false;

	if((m_state != CUBESTATE_RED)/* && (m_state != CUBESTATE_BLUE)*/)
		return false;

	if(m_memList1 && m_memList2)
		return false;

	if(!m_memList1 && !m_memList2 && m_mobList.GetCount() == 0)
		bfirst = true;

	return true;
}

bool CBattleSpace::SetMemList(CCubeMemList* memlist)
{
	bool bSet = false;

	if(!memlist)
		return false;

	if(!m_memList1 && !m_memList2)
	{
		if(!memlist->SetCubeNum(m_area->m_index))
		{
			GAMELOG << init("FAILED SET CUBENUM 1") << "AREAIDX" << m_area->m_index << end;
			return false;
		}
		m_memList1 = memlist;
		bSet = true;
	}
	else if(!m_memList1)
	{
		if(!memlist->SetCubeNum(m_area->m_index))
		{
			GAMELOG << init("FAILED SET CUBENUM 2") << "AREAIDX" << m_area->m_index << end;
			return false;
		}
		m_memList1 = memlist;
		bSet = true;
	}
	else if(!m_memList2)
	{
		if(!memlist->SetCubeNum(m_area->m_index))
		{
			GAMELOG << init("FAILED SET CUBENUM 3") << "AREAIDX" << m_area->m_index << end;
			return false;
		}
		m_memList2 = memlist;
		bSet = true;
	}
	else
	{
		GAMELOG << init("FAILED SET MEMLIST")
				<< end;
	}

	return bSet;
}

bool CBattleSpace::DelMemList(CCubeMemList* memlist)
{
	if(!memlist)
		return false;

	if(m_memList1 == memlist)
	{
		if(m_memList1)
		{
			m_memList1 = NULL;
		}
		
		return true;
	}
	else if(m_memList2 == memlist)
	{
		if(m_memList2)
		{
			m_memList2 = NULL;
		}
		return true;
	}
	
	GAMELOG << init("FAILED DEL MEMLIST IN CUBE") << end;
	return false;
}


void CBattleSpace::DelMob(CNPC* npc)
{
	m_mobList.RemoveData(npc);
}

void CBattleSpace::Clean()
{
	ChageState(CUBESTATE_RED);

	if(m_memList1)
	{
		m_memList1->GetOutAll();
	//	delete m_memList1;
		m_memList1->Disable();
		m_memList1 = NULL;
	}

	if(m_memList2)
	{
		m_memList2->GetOutAll();
	//	delete m_memList2;
		m_memList2->Disable();
		m_memList2 = NULL;
	}

	CNPC* npc;
	void* pos = m_mobList.GetHead();
	while(pos)
	{
		npc = (CNPC*)(m_mobList.GetData(pos));
		pos = m_mobList.GetNext(pos);

//		npc->SendDisappearAllInCell(true);
		DelAttackList(npc);
		m_area->CharFromCell(npc, true);
		m_area->DelNPC(npc);
	}

	m_mobList.RemoveAll();
}

bool CBattleSpace::IsAvailableJoinPersonal()
{
	if(/*m_state == CUBESTATE_RED || */ m_state == CUBESTATE_BLUE)
		return true;

	return false;
}

void CBattleSpace::RegenMob()
{
	int i;
	int rand;
	CNPC* npc;
	int cx, cz;
	for(i = 0; i < m_extremeCube->m_regencount; ++i)
	{
		if(m_extremeCube->m_regenPoint[i].regenX >= m_area->m_zone->m_zonePos[this->m_extra][1]/2
			&& m_extremeCube->m_regenPoint[i].regenX <= m_area->m_zone->m_zonePos[this->m_extra][3]/2
			&& m_extremeCube->m_regenPoint[i].regenZ >= m_area->m_zone->m_zonePos[this->m_extra][2]/2
			&& m_extremeCube->m_regenPoint[i].regenZ <= m_area->m_zone->m_zonePos[this->m_extra][4]/2)
		{
			rand = GetRandom(0, m_extremeCube->m_mobCount - 1);
			npc =  gserver.m_npcProtoList.Create(m_extremeCube->m_mobidx[rand], NULL);
			if(!npc)
				continue;

			GET_X(npc) = m_extremeCube->m_regenPoint[i].regenX;
			GET_Z(npc) = m_extremeCube->m_regenPoint[i].regenZ;
			GET_YLAYER(npc) = m_extremeCube->m_regenPoint[i].ylayer;
			GET_R(npc) = 0;

			npc->m_regenX = GET_X(npc);
			npc->m_regenZ = GET_Z(npc);
			npc->m_regenY = GET_YLAYER(npc);

			m_area->AddNPC(npc);
			m_area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			m_area->CharToCell(npc, GET_YLAYER(npc), cx, cz);

			npc->m_bCubeRegen = true;

			m_mobList.AddToTail(npc);
		}
	}
}


// 보스방
CBossSpace::CBossSpace()
{
	m_state = CUBESTATE_YELLOW;
	m_memList = NULL;
	m_boss = NULL;
	m_type = BOSS_SPACE;
}


CBossSpace::~CBossSpace()
{
	m_boss = NULL;

	if(m_memList)
	{
	//	delete m_memList;
		m_memList->Disable();
		m_memList = NULL;
	}
}


void CBossSpace::CheckCubeState()
{
	bool IsBoss = false;
	bool IsMemList = false;

	if(m_boss)
		IsBoss = true;

	if(m_memList)
	{
		if(m_memList->GetPCCount() == 0 || m_memList->IsBreak() == false)
		{
			GAMELOG << init("BOSS MEMLIST ALL DEAD")
					<< "UNIQUEIDX" << delim
					<< m_memList->GetUniqueIdx() << delim
					<< "CLEARCNT" << delim
					<< m_memList->GetWinCount() << delim
					<< "AREAIDX" << delim
					<< m_area->m_index
					<< end;

			IsMemList = false;
			m_memList->GetOutAll();
			m_memList->Disable();
			m_memList = NULL;
		}
		else
		{
			IsMemList = true;
		}
	}
	else
	{
		IsMemList = false;
	}

	if(m_state == CUBESTATE_YELLOW)
	{
		if(IsMemList || IsBoss)
		{
			Clean();

			GAMELOG << init("CHANGE BOSS STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "YELLOW => CLEAN"
					<< end;
		}
	}
	else if(m_state == CUBESTATE_ORANGE)
	{
		if(!IsBoss && !IsMemList)
		{
			Clean();

			GAMELOG << init("CHANGE BOSS STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "ORANGE => CLEAN"
					<< end;
		}
		else if(IsBoss && !IsMemList)
		{
			Clean();

			GAMELOG << init("CHANGE BOSS STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "ORANGE => CLEAN"
					<< end;
		}
		else if(!IsBoss && IsMemList)
		{
			ChageState(CUBESTATE_BLUE);
			m_waitTime = gserver.m_pulse + PULSE_REAL_MIN;

			GAMELOG << init("CHANGE BOSS STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "ORANGE => BLUE"
					<< end;
		}
	}
	else if(m_state == CUBESTATE_BLUE)
	{
		if(IsBoss && IsMemList)
		{
			Clean();

			GAMELOG << init("CHANGE BOSS STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "BLUE => CLEAN 1"
					<< end;
		}
		else if(!IsBoss && !IsMemList)
		{
			Clean();

			GAMELOG << init("CHANGE BOSS STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "BLUE => CLEAN 2"
					<< end;
		}
		else if(IsBoss && !IsMemList)
		{
			Clean();

			GAMELOG << init("CHANGE BOSS STATE")
					<< "AREAIDX" << delim
					<< m_area->m_index << delim
					<< "BLUE => CLEAN 3"
					<< end;
		}
		else if(!IsBoss && IsMemList)
		{
			if(m_crystal == NULL)
			{
				if(m_waitTime < gserver.m_pulse)
				{
					Clean();
					
					GAMELOG << init("CHANGE BOSS STATE")
						<< "AREAIDX" << delim
						<< m_area->m_index << delim
						<< "BLUE => CLEAN 5" 
						<< end;
				}
			}
			else
			{
				if(m_waitTime < gserver.m_pulse)
				{
					Clean();

					GAMELOG << init("CHANGE BOSS STATE")
							<< "AREAIDX" << delim
							<< m_area->m_index << delim
							<< "BLUE => CLEAN 6" 
							<< end;
				}
			}
		}
	}
}


bool CBossSpace::IsAvailableJoin()
{
	if(m_state != CUBESTATE_YELLOW)
		return false;

	if(m_boss)
		return false;

	if(m_memList)
		return false;
	
	return true;
}


bool CBossSpace::SetMemList(CCubeMemList* memlist)
{
	if(!memlist)
		return false;

	if(m_memList)
	{
		GAMELOG << init("FAILED SET MEMLIST IN BOSSCUBE") << end;
		return false;
	}
	m_memList = memlist;
	if(!m_memList->SetCubeNum(m_area->m_index))
	{
		GAMELOG << init("FAILED SET MEMLIST IN BOSSCUBE 1") << "AREAIDX" << m_area->m_index << end;
		m_memList = NULL;
		return false;
	}
	return true;
}


void CBossSpace::DelMob(CNPC* npc)
{
	if(m_boss == npc)
	{
		m_boss = NULL;
	}
}

bool CBossSpace::DelMemList(CCubeMemList* memlist)
{
	if(!memlist)
		return false;

	if(m_memList == memlist)
	{
		m_memList = NULL;
		return true;
	}
	
	GAMELOG << init("FAILED DEL MEMLIST IN BOSSCUBE") << end;
	return false;
}

void CBossSpace::Clean()
{
	ChageState(CUBESTATE_YELLOW);

	if(m_boss)
	{
		DelAttackList(m_boss);
		m_boss->m_pArea->CharFromCell(m_boss, true);
		m_boss->m_pArea->DelNPC(m_boss);
		m_boss = NULL;
	}
	
	if(m_memList)
	{
		m_memList->GetOutAll();
		m_memList->Disable();
		m_memList = NULL;
	}
}

void CBossSpace::RegenMob()
{
	int rand, cx, cz;
	CNPC* npc;
	int nTry = 0;
	int centerX, centerZ;

	rand = GetRandom(0, m_extremeCube->m_bossCount - 1);
	npc =  gserver.m_npcProtoList.Create(m_extremeCube->m_bossidx[rand], NULL);
	if(!npc)
		return ;

	centerX = (m_area->m_zone->m_zonePos[m_extra][1] + m_area->m_zone->m_zonePos[m_extra][3])/2/2;
	centerZ = (m_area->m_zone->m_zonePos[m_extra][2] + m_area->m_zone->m_zonePos[m_extra][4])/2/2;

	do{
		GET_X(npc) = centerX + GetRandom(-5, 5);
		GET_Z(npc) = centerZ + GetRandom(-5, 5);
		GET_YLAYER(npc) = m_area->m_zone->m_zonePos[m_extra][0];
		GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		if (m_area->GetAttr(GET_YLAYER(npc), GET_X(npc), GET_Z(npc)) == MAPATT_FIELD)
			break;
		nTry++;
	}while(nTry <= 10);
				
	npc->m_regenX = GET_X(npc);
	npc->m_regenY = GET_YLAYER(npc);
	npc->m_regenZ = GET_Z(npc);
				
	m_area->AddNPC(npc);
	m_area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
	m_area->CharToCell(npc, GET_YLAYER(npc), cx, cz);

	npc->m_bCubeRegen = true;
	m_boss = npc;
}



///////////////  CExtremeCube  ///////////////


CExtremeCube::CExtremeCube()
{
	m_nextGuildCubeTime = 0;
	m_bGuildCubeStart = false;
	memset(m_cube, sizeof(CCubeSpace*) * MAX_CUBESPACE, 0);

	m_mobidx = NULL;
	m_mobCount = 0;

	m_bossidx = NULL;
	m_bossCount = 0;
	
	m_regenPoint = NULL;
	m_regencount = 0;

	m_bGuildCubeRemainStartNotice = false;
	m_bGuildCubeRemainEndNotice = false;
}

CExtremeCube::~CExtremeCube()
{
	int i;

	for(i = 0; i < MAX_CUBESPACE; ++i)
	{
		if(m_cube[i])
		{
			delete m_cube[i];
			m_cube[i] = NULL;
		}
	}

	if(m_regenPoint)
	{
		delete[] m_regenPoint;
		m_regenPoint = NULL;
		m_regencount = 0;
	}

	if(m_mobidx)
	{
		delete[] m_mobidx;
		m_mobidx = NULL;
		m_mobCount = 0;
	}

	if(m_bossidx)
	{
		delete m_bossidx;
		m_bossidx = NULL;
		m_bossCount = 0;
	}
}

bool CExtremeCube::Init()
{
	int i;
	int offset;
	CZone* pZone;

	offset = gserver.FindZone(ZONE_EXTREME_CUBE);
	if(offset == -1)
		return false;

	pZone = gserver.m_zones + offset;

	if(pZone->m_countArea < MAX_CUBESPACE)
		return false;

	for(i = 0; i < BOSSCUBE_START; ++i)
	{
		m_cube[i] = new CBattleSpace();
		m_cube[i]->m_area = pZone->m_area + i;
		m_cube[i]->m_extremeCube = this;
		m_cube[i]->m_extra = i%2;
		m_cube[i]->ChageState(CUBESTATE_RED);
	}
	for(i = BOSSCUBE_START; i < MAX_CUBESPACE; ++i)
	{
		m_cube[i] = new CBossSpace();
		m_cube[i]->m_area = pZone->m_area + i;
		m_cube[i]->m_extremeCube = this;
		m_cube[i]->m_extra = 2;
		m_cube[i]->ChageState(CUBESTATE_YELLOW);
	}

	return true;
}



void CExtremeCube::CheckCubeSpace()
{
	if(!IsExtremeCubeServer())
		return ;

	int i;

	time_t curtime;
	time(&curtime);

#ifdef LC_KOR
	if(!m_bGuildCubeRemainStartNotice && (curtime + 60) > m_nextGuildCubeTime)
	{
		GAMELOG << init("STARTCUBE_REMAIN") << "CURETIME" << delim << (int)curtime << delim
			    << "STARTIME" << delim << (int)m_nextGuildCubeTime << end;
		StartGuildCubeRemainTime();
	}

	if(!m_bGuildCubeRemainEndNotice && (curtime + 60) > (m_nextGuildCubeTime + GUILDCUBE_TIME) )
	{
		GAMELOG << init("ENDCUBE_REMAIN") << "CURETIME" << delim << (int)curtime << delim
			    << "STARTIME" << delim << (int)m_nextGuildCubeTime << end;
		EndGuildCubeRemainTime();
	}
#endif // LC_KOR

	if(m_bGuildCubeStart == false && (curtime > m_nextGuildCubeTime))
	{
		GAMELOG << init("STARTCUBETIME") << "CURETIME" << delim << (int)curtime << delim
			    << "STARTIME" << delim << (int)m_nextGuildCubeTime << end;
		StartGuildCube();
	}
	else if(m_bGuildCubeStart && (curtime > m_nextGuildCubeTime + GUILDCUBE_TIME))
	{
		GAMELOG << init("ENDCUBETIME") << (int)curtime << delim << (int)m_nextGuildCubeTime << end;
		EndGuildCube();
	}
	
	for(i = 0; i < MAX_CUBESPACE; ++i)
	{
		if(m_cube[i])
			m_cube[i]->CheckCubeState();
	}
}


int CExtremeCube::GetAvailableBattleCube(bool& bfirst)
{
	int i;
	int cubeindex[BOSSCUBE_START];
	int cubecount = 0;
	memset(cubeindex, -1, sizeof(int) * BOSSCUBE_START);
	for(i = 0; i < BOSSCUBE_START; ++i)
	{
		if(m_cube[i]->IsBattleCubeSpace())
		{
			if(((CBattleSpace*)m_cube[i])->IsAvailableJoin(bfirst))
			{
				cubeindex[cubecount] = i;
				cubecount++;
			}
		}
	}

	if(cubecount > 0 && cubecount <= BOSSCUBE_START)
	{
		int index;
		index = GetRandom(0, cubecount - 1);
		if(cubeindex[index] >= 0 && cubeindex[index] < BOSSCUBE_START)
		{
			if(((CBattleSpace*)m_cube[cubeindex[index]])->IsAvailableJoin(bfirst))
			{
				return cubeindex[index];
			}
		}
	}

	return -1;
}




int CExtremeCube::GetAvailableBossCube()
{
	int i;
	for(i = BOSSCUBE_START; i < MAX_CUBESPACE; ++i)
	{
		if(m_cube[i]->IsBossCubeSpace())
		{
			if( ((CBossSpace*)m_cube[i])->IsAvailableJoin())
				return i;
		}
	}

	return -1;
}

CPartyCubeMemList* CExtremeCube::GetAvailablePartyCubeMemList()
{
	int i;
	for(i = 0 ; i < MAX_CUBESPACE * 2; ++i)
	{
		if( (m_partyCubeMemList[i].IsEnable() == false) && (m_partyCubeMemList[i].GetPCCount() == 0) )
			return &(m_partyCubeMemList[i]);
	}
	GAMELOG << "CANNOT FIND PARTYCUBE MEMLIST" << end;
	return NULL;
}

CGuildCubeMemList* CExtremeCube::GetAvailableGuildCubeMemList()
{
	int i;
	for(i = 0 ; i < MAX_CUBESPACE * 2; ++i)
	{
		if( (m_guildCubeMemList[i].IsEnable() == false) && (m_guildCubeMemList[i].GetPCCount() == 0) )
			return &(m_guildCubeMemList[i]);
	}

	GAMELOG << "CANNOT FIND GUILDCUBE MEMLIST" << end;
	return NULL;
}

bool CExtremeCube::Load()
{	
	if(CheckCubeMasterNPC())
	{
		return true;
	}

	if(!LoadCubeData())
	{
		GAMELOG << init("LoadCubeData Failed")
				<< end;
		return false;
	}

	long time;
	SetGuildCubeTime(time);

	if(!CreateCubeSpace())
	{
		GAMELOG << init("CreateCubeSpace Failed")
				<< end;
		return false;
	}

	return true;
}

CCubeSpace* CExtremeCube::GetExtremeCube(int i)
{
	if(i < 0 || i > MAX_CUBESPACE - 1)
		return NULL;

	return m_cube[i];
}

CCubeMemList* CExtremeCube::FindMemList(CParty* party)
{
	if(!party)
		return NULL;

	int i;
	CBattleSpace* space;
	CBossSpace* bosscube;
	for(i = 0 ; i < BOSSCUBE_START; ++i)
	{
		if(m_cube[i] && m_cube[i]->IsBattleCubeSpace())
		{
			space = (CBattleSpace*)m_cube[i];
			if(space->GetMemList1()
			&& space->GetMemList1()->IsPartyCubeMemList()
			&& (space->GetMemList1()->GetUniqueIdx() == party->m_cubeUniqueIdx) )
			{
				return space->GetMemList1();
			}

			else if(space->GetMemList2()
			&& space->GetMemList2()->IsPartyCubeMemList()
			&& (space->GetMemList2()->GetUniqueIdx() == party->m_cubeUniqueIdx) )
			{
				return space->GetMemList2();
			}
		}
	}

	for(i = BOSSCUBE_START; i < MAX_CUBESPACE;++i)
	{
		if(m_cube[i] && m_cube[i]->IsBossCubeSpace())
		{
			bosscube = (CBossSpace*)m_cube[i];
			if(bosscube->GetMemList()
			&& (bosscube->GetMemList()->GetUniqueIdx() == party->m_cubeUniqueIdx) )
			{
				return bosscube->GetMemList();
			}
		}
	}

	return NULL;
}


CCubeMemList* CExtremeCube::FindMemList(CGuild* guild)
{
	if(!guild)
		return NULL;

	int i;
	CBattleSpace* space;
	CBossSpace* bosscube;
	
	for(i = 0 ; i < BOSSCUBE_START; ++i)
	{
		if(m_cube[i] && m_cube[i]->IsBattleCubeSpace())
		{
			space = (CBattleSpace*)m_cube[i];
			if(space->GetMemList1()
			&& space->GetMemList1()->IsGuildCubeMemList()
			&& (space->GetMemList1()->GetUniqueIdx() == guild->m_cubeUniqueIdx) )
			{
				return space->GetMemList1();
			}

			else if(space->GetMemList2()
			&& space->GetMemList2()->IsGuildCubeMemList()
			&& (space->GetMemList2()->GetUniqueIdx() == guild->m_cubeUniqueIdx) )
			{
				return space->GetMemList2();
			}
		}
	}

	for(i = BOSSCUBE_START; i < MAX_CUBESPACE;++i)
	{
		if(m_cube[i] && m_cube[i]->IsBossCubeSpace())
		{
			bosscube = (CBossSpace*)m_cube[i];
			if(bosscube->GetMemList()
			&& (bosscube->GetMemList()->GetUniqueIdx() == guild->m_cubeUniqueIdx) )
			{
				return bosscube->GetMemList();
			}
		}
	}

	return NULL;
}


CCubeMemList* CExtremeCube::FindMemList(int uniqueidx)
{
	int i;
	CBattleSpace* battlecube;
	CBossSpace* bosscube;
	for(i = 0 ; i < BOSSCUBE_START; ++i)
	{
		if(m_cube[i] && m_cube[i]->IsBattleCubeSpace())
		{
			battlecube = (CBattleSpace*)m_cube[i];
			if(battlecube->GetMemList1()
			&& (battlecube->GetMemList1()->GetUniqueIdx() == uniqueidx) )
			{
				return battlecube->GetMemList1();
			}

			else if(battlecube->GetMemList2()
			&& (battlecube->GetMemList2()->GetUniqueIdx() == uniqueidx) )
			{
				return battlecube->GetMemList2();
			}
		}
	}

	for(i = BOSSCUBE_START; i < MAX_CUBESPACE;++i)
	{
		if(m_cube[i] && m_cube[i]->IsBossCubeSpace())
		{
			bosscube = (CBossSpace*)m_cube[i];
			if(bosscube->GetMemList()
			&& (bosscube->GetMemList()->GetUniqueIdx() == uniqueidx) )
			{
				return bosscube->GetMemList();
			}
		}
	}

	return NULL;
}

void CExtremeCube::StartGuildCube()
{
	m_bGuildCubeStart = true;

	GetOutAllMemList();
#ifdef LC_KOR
	if(IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		HelperGuildCubeNoticeMsg(rmsg, 0);
		SEND_Q(rmsg, gserver.m_helper);
	}
#endif // LC_KOR

	GAMELOG << init("ExCube Start") << end;
}

void CExtremeCube::EndGuildCube()
{
	long time;

	GetOutAllMemList();

	SetGuildCubeTime(time);

	m_bGuildCubeStart = false;

#ifdef LC_KOR
	m_bGuildCubeRemainStartNotice = false;
	m_bGuildCubeRemainEndNotice = false;

	if (IS_RUNNING_HELPER)
	{
		CNetMsg rmsg;
		HelperGuildCubeNoticeMsg(rmsg, 1);
		SEND_Q(rmsg, gserver.m_helper);
	}
#endif // LC_KOR

	GAMELOG << init("ExCube End") << end;
}

void CExtremeCube::StartGuildCubeRemainTime()
{
	m_bGuildCubeRemainStartNotice = true;

	CNetMsg rmsg;
	HelperGuildCubeNoticeMsg(rmsg, 2);
	SEND_Q(rmsg, gserver.m_helper);
}

void CExtremeCube::EndGuildCubeRemainTime()
{
	m_bGuildCubeRemainEndNotice = true;

	CNetMsg rmsg;
	HelperGuildCubeNoticeMsg(rmsg, 3);
	SEND_Q(rmsg, gserver.m_helper);
}

void CExtremeCube::GetOutAllMemList()
{
	CBattleSpace* battlecube;
	CBossSpace* bosscube;

	int i;
	for(i = 0 ; i < BOSSCUBE_START; ++i)
	{
		if(m_cube[i] && m_cube[i]->IsBattleCubeSpace())
		{
			battlecube = (CBattleSpace*)m_cube[i];

			if(battlecube->GetMemList1())
				battlecube->GetMemList1()->GetOutAll();

			if(battlecube->GetMemList2())
				battlecube->GetMemList2()->GetOutAll();
		}
	}

	for(i = BOSSCUBE_START; i < MAX_CUBESPACE;++i)
	{
		if(m_cube[i] && m_cube[i]->IsBossCubeSpace())
		{
			bosscube = (CBossSpace*)m_cube[i];
			if(bosscube->GetMemList())
				bosscube->GetMemList()->GetOutAll();
		}
	}
}

void CExtremeCube::SetGuildCubeTime(long& time)
{
	int addmday;
	struct tm nextcubetime = NOW();


	if(2 > nextcubetime.tm_wday)
	{
		addmday = 2 - nextcubetime.tm_wday;
	}
	else if(2 == nextcubetime.tm_wday)
	{
		if(nextcubetime.tm_hour > 21)
		{
			addmday = 2;
		}
		else
		{
			addmday = 0;
		}
	}
	else if(4 > nextcubetime.tm_wday)
	{
		 addmday = 4 - nextcubetime.tm_wday;
	}
	else if(4 == nextcubetime.tm_wday)
	{
		if(nextcubetime.tm_hour > 21)
		{
			addmday = 5;  // 다음주 화요일로
		}
		else
		{
			addmday = 0;  // 목요일 8시로 
		}
	}
	else 
	{
		addmday = 9 - nextcubetime.tm_wday;
	}
	
	nextcubetime.tm_hour = 21;
	nextcubetime.tm_min = 0;
	nextcubetime.tm_sec = 0;
	nextcubetime.tm_mday += addmday;

	time = mktime(&nextcubetime);
	m_nextGuildCubeTime = time;


}


bool CExtremeCube::IsExtremeCubeServer()
{
	return (gserver.m_subno == EXTREME_CUBE_SUBSERVER);
} 

bool CExtremeCube::LoadCubeData()
{
	int npcidx, i;
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbdata);
	cmd.SetQuery("SELECT * FROM t_npc_cube WHERE a_type = 0");
	if(!cmd.Open())
		return false;

	if(cmd.m_nrecords == 0)
		return false;

	m_mobCount = cmd.m_nrecords;
	m_mobidx = new int[m_mobCount];
	memset(m_mobidx, 0, sizeof(int) * cmd.m_nrecords);

	i = 0;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_npcidx", npcidx);
		m_mobidx[i] = npcidx;
		i++;
	}

	cmd.SetQuery("SELECT * FROM t_npc_cube WHERE a_type = 1");
	if(!cmd.Open())
		return false;

	if(cmd.m_nrecords == 0)
		return false;

	m_bossCount = cmd.m_nrecords;
	m_bossidx = new int[m_bossCount];
	memset(m_bossidx, 0, sizeof(int) * cmd.m_nrecords);

	i = 0;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_npcidx", npcidx);
		m_bossidx[i] = npcidx;
		i++;
	}

	cmd.SetQuery("SELECT * FROM t_npc_regen_cube");
	if(!cmd.Open())
		return false;

	if(cmd.m_nrecords == 0)
		return false;

	m_regencount = cmd.m_nrecords;

	m_regenPoint = new CUBEREGENPOINT[cmd.m_nrecords];

	i = 0;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_pos_x", m_regenPoint[i].regenX);
		cmd.GetRec("a_pos_z", m_regenPoint[i].regenZ);
		cmd.GetRec("a_pos_h", m_regenPoint[i].regenY);
		cmd.GetRec("a_y_layer", m_regenPoint[i].ylayer);

		i++;
	}
	return true;
}

bool CExtremeCube::CreateCubeSpace()
{
	int offset, i;
	CZone* pZone;

	// 큐브생성
	offset = gserver.FindZone(ZONE_EXTREME_CUBE);
	if(offset == -1)
		return false;

	pZone = gserver.m_zones + offset;

	if(pZone->m_countArea < MAX_CUBESPACE)
		return false;

	for(i = 0; i < BOSSCUBE_START; ++i)
	{
		m_cube[i] = new CBattleSpace();
		m_cube[i]->m_area = pZone->m_area + i;
		m_cube[i]->m_extremeCube = this;
		m_cube[i]->m_extra = i%2;
		m_cube[i]->ChageState(CUBESTATE_RED);
	}
	for(i = BOSSCUBE_START; i < MAX_CUBESPACE; ++i)
	{
		m_cube[i] = new CBossSpace();
		m_cube[i]->m_area = pZone->m_area + i;
		m_cube[i]->m_extremeCube = this;
		m_cube[i]->m_extra = 2;
		m_cube[i]->ChageState(CUBESTATE_YELLOW);
	}

	return true;
}


bool CExtremeCube::CheckCubeMasterNPC()
{
	CZone* pZone;
	CArea* pArea;
	CNPC *npc;	
	if(!IsExtremeCubeServer())
	{
		int k;
		for(k = 0; k < gserver.m_numZone; ++k)
		{
			if(gserver.m_zones[k].m_countArea > 1)
				continue;
		
			pZone = gserver.m_zones + k;
			pArea = pZone->m_area;

			if( !pZone || !pArea )
				continue;
			
			CNPC* next;
			next = pArea->m_npcList;
			while((npc = next))
			{
				next = npc->m_pNPCNext;

				if(npc->m_proto->m_index == CUBEMASTER_RED
					|| npc->m_proto->m_index == CUBEMASTER_BLUE
					|| npc->m_proto->m_index == CUBEMASTER_YELLOW
					|| npc->m_proto->m_index == CUBEMASTER_PURPLE
					|| npc->m_proto->m_index == CUBEMASTER_ORANGE)
				{
					GAMELOG << init("CubeMasterNPC Delete")
							<< "subserver" << delim
							<< gserver.m_subno
							<< "npc index" << delim
							<< npc->m_proto->m_index << end;

					pArea->CharFromCell(npc, true);
					pArea->DelNPC(npc);
				}
			}				
		}

		return true;
	}

	return false;
}

#endif EXTREME_CUBE


