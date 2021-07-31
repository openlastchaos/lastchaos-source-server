#include "stdhdrs.h"
#include "Server.h"
#include "Log.h"
#include "CmdMsg.h"
#include "WarCastle.h"

#ifdef ENABLE_WAR_CASTLE

CMeracCastle::CMeracCastle() : CWarCastle()
{
	memset(m_gateNPC, 0, sizeof(CNPC*) * COUNT_MERAC_GATE_NPC);
}

void CMeracCastle::SetNextWarTime(int nextWarTime)
{
	time_t curtime;
	time(&curtime);

	// 공성 시작 시간이 현재 시간이후이면 그대로 적용
	if (GetState() == WCSF_NORMAL && nextWarTime != 0)
	{
		if (nextWarTime > curtime)
		{
			m_nextWarTime = nextWarTime;
			return ;
		}
		else
		{
			GAMELOG << init("WAR CASTLE : WARN SET TIME")
					<< "CUR TIME : " << (int)curtime << delim
					<< "NEXT TIME : " << nextWarTime
					<< end;
		}
	}

	// 공성 시간을 기본 값으로 설정
	// 현재 요일을 구함
	struct tm curtm = NOW();

	// 다음 토요일을 구함
	int next = 6 - curtm.tm_wday;
	if (next <= 0)
		next += 7;

	// 다음 토요일 20시에 해당하는 time_t 값을 구함
	struct tm nexttm = curtm;
#ifdef LC_TLD
	nexttm.tm_hour = 19;
#else
	nexttm.tm_hour = 20;
#endif
	nexttm.tm_min = 0;
	nexttm.tm_sec = 0;
	nexttm.tm_mday += next;
	time_t nexttime = mktime(&nexttm);

	// 현재 시간보다 작으면 다음주로
	if (nexttime < curtime)
	{
		nexttm.tm_mday += 7;
		nexttime = mktime(&nexttm);
	}

#ifdef TLD_WAR_TEST
	m_nextWarTime = 1293721200;
	int timetest;
	time((time_t*)&timetest);
	if (timetest >= m_nextWarTime)
	{
		GAMELOG << init("WARCASTLE TLD WAR TEST ERROR")
				<< end;
		gserver.m_bShutdown = true;
		gserver.m_bReboot = true;
		
		GAMELOG << init("shutdown by CMeracCastle::SetNextWarTime") << delim << "timetest >= m_nextWarTime" << end;
	}
	return;
#endif


	m_nextWarTime = nexttime;

	if (nextWarTime != 0)
	{
		GAMELOG << init("WAR CASTLE : SET NEXT TIME FOR LOAD")
				<< "IN : " << nextWarTime << delim
				<< "SET : " << m_nextWarTime
				<< end;
	}
}

bool CMeracCastle::IsJoinTime()
{
	if (m_bMoveToRegen)
		return false;

	struct tm tmJoinTime;
	GetJoinTime(&tmJoinTime, false);
	time_t jointime = mktime(&tmJoinTime);

	// 마지막 공성시간과 비교
	time_t curtime;
	time(&curtime);
	if (m_lastWarTime < curtime && curtime < jointime)
		return false;

	return true;
}

void CMeracCastle::GetJoinTime(struct tm* joinTime, bool bHumanable)
{
	// 다음 공성을 구하고
	struct tm tmNext;
	GetNextWarTime(&tmNext, false);

	// 월요일 구하기
	int nDay = tmNext.tm_wday;
	if (nDay == 0)
		nDay = 7;
	nDay -= 1;

	// 다음 공성에서 nDay일을 빼고
	// 그 날 20시로 설정 : 7시 
#ifdef LC_TLD
	tmNext.tm_hour = 19;
#else
	tmNext.tm_hour = 20;
#endif
	tmNext.tm_min = 0;
	tmNext.tm_sec = 0;
	tmNext.tm_mday -= nDay;
	time_t tJoin = mktime(&tmNext);

#ifdef TLD_WAR_TEST
	tJoin = 0;
#endif
	*joinTime = *localtime(&tJoin);

	if (bHumanable)
	{
		joinTime->tm_year += 1900;
		joinTime->tm_mon += 1;
	}
}

int CMeracCastle::GetRegenPoint(int joinflag, CPC* ch)
{
	switch (joinflag)
	{
	case WCJF_OWNER:
	case WCJF_DEFENSE_GUILD:
	case WCJF_DEFENSE_CHAR:
		return 2;

	case WCJF_ATTACK_GUILD:
		if (ch && ch->m_guildInfo && ch->m_guildInfo->guild())
		{
			int i;
			for (i = 0; i < m_nRegenPoint; i++)
			{
				if (m_posRegenPoint[i] == ch->m_guildInfo->guild()->index())
				{
					switch (i)
					{
					case 0:		return 8;
					case 1:		return 9;
					case 2:		return 10;
					case 3:		return 11;
					case 4:		return 12;
					case 5:		return 13;
					case 6:		return 14;
					case 7:		return 15;
					}
				}
			}
		}
		return 3;

	case WCJF_ATTACK_CHAR:
		return 3;

	default:
		return 0;
	}
}

void CMeracCastle::SetGateNPC(CNPC* npc)
{
	if (npc->m_proto->CheckFlag(NPC_CASTLE_TOWER))
	{
		int i = 0;
		switch (npc->m_proto->m_index)
		{
		case 222:	i = 0;	break;		// 좌측성문1
		case 223:	i = 1;	break;		// 좌측성문2
		case 227:	i = 2;	break;		// 우측성문1
		case 228:	i = 3;	break;		// 우측성문2
		case 229:	i = 4;	break;		// 중앙성문1
		case 230:	i = 5;	break;		// 중앙성문2
		default:			return ;
		}

		m_gateNPC[i] = npc;
	}
}

void CMeracCastle::UpdateGateState(CNPC* npc)
{
	int i;
	for (i = 0; i < COUNT_MERAC_GATE_NPC; i++)
	{
		if (m_gateNPC[i] == npc)
		{
			m_gateState |= (1 << i);
		}
	}
}

bool CMeracCastle::IsOpenGate()
{
	// 왼쪽
	int left = (1 << 0) | (1 << 1);

	// 오른쪽
	int right = (1 << 2) | (1 << 3);

	// 중앙
	int center = (1 << 4) | (1 << 5);

	if (GetGateState(center) != center)
		return false;

	if (GetGateState(left) == left)
		return true;

	if (GetGateState(right) == right)
		return true;

	return false;
}

bool CMeracCastle::SetNextWarTime(int wday, int hour)
{
	int minHour = 9999;
	int maxHour = -1;
	switch (wday)
	{
	case 0:		// 일요일 : 13 ~ 23
		minHour = 13;
		maxHour = 23;
		break;

	case 6:		// 토요일 : 18 ~ 23
#ifdef LC_USA	// 미국 08 ~ 16
		minHour = 8;
		maxHour = 16;
		break;
#else
		minHour = 18;
		maxHour = 23;
		break;
#endif // LC_USA
	default:
		return false;
	}
	if (hour < minHour || hour > maxHour)
		return false;

	time_t curtime;
	time(&curtime);
	struct tm curtm = NOW();

	// 다음 해당 요일까지 차이를 구함
	int next = wday - curtm.tm_wday;
	if (next <= 0)
		next += 7;
	if (next <= 4)
		next += 7;

	// 그날 해당 시간의 time_t 값을 구함
	struct tm nexttm = curtm;
	nexttm.tm_hour = hour;
	nexttm.tm_min = 0;
	nexttm.tm_sec = 0;
	nexttm.tm_mday += next;
	time_t nexttime = mktime(&nexttm);

	// 현재 시간보다 작으면 ?	if (nexttime < curtime)
		return false;

	m_nextWarTime = nexttime;

	return true;
}

bool CMeracCastle::GiveLordItem(CPC* pc)
{
	CNetMsg rmsg;
	CItem* lordItem = NULL;

	int lordItemIndex = GetLordItemIndex(pc->m_job, 1);
	int r, c;
	if (pc->m_invenNormal.FindItem(&r, &c, lordItemIndex, 0, 0))
	{
		m_bGiveLordWeapon = true;
		return true;
	}
	lordItemIndex = GetLordItemIndex(pc->m_job, 2);
	if (pc->m_invenNormal.FindItem(&r, &c, lordItemIndex, 0, 0))
	{
		m_bGiveLordWeapon = true;
		return true;
	}
	lordItemIndex = GetLordItemIndex(pc->m_job, pc->m_job2);

	if (lordItemIndex > 0)
	{
		lordItem = gserver.m_itemProtoList.CreateItem(lordItemIndex, -1, 0, 0, 1);
		if (lordItem)
		{
			if (AddToInventory(pc, lordItem, true, true))
			{
				ItemAddMsg(rmsg, lordItem);
				SEND_Q(rmsg, pc->m_desc);
			}
			else
			{
				delete lordItem;
				lordItem = NULL;
			}
		}
	}

	if (lordItem == NULL)
	{
		GAMELOG << init("WAR CASTLE : CANT GIVE WEAPON", pc)
				<< "ZONE" << delim
				<< GetZoneIndex() << delim
				<< "JOB" << delim
				<< pc->m_job << delim
				<< pc->m_job2
				<< end;

		return false;
	}
	else
	{
		GAMELOG << init("WAR CASTLE : GIVE WEAPON", pc)
				<< "ZONE" << delim
				<< GetZoneIndex() << delim
				<< "JOB" << delim
				<< pc->m_job << delim
				<< pc->m_job2 << delim
				<< "ITEM" << delim
				<< itemlog(lordItem)
				<< end;

		m_bGiveLordWeapon = true;

		return true;
	}
}

bool CMeracCastle::TakeLordItem(CPC* pc)
{
	CNetMsg rmsg;
	int r, c;
	if (pc->m_invenNormal.FindItem(&r, &c, GetLordItemIndex(pc->m_job, 1), 0, 0) ||
		pc->m_invenNormal.FindItem(&r, &c, GetLordItemIndex(pc->m_job, 2), 0, 0))
	{
		CItem* item = pc->m_invenNormal.GetItem(r, c);
		if (item->m_wearPos != WEARING_NONE)
		{
			ItemWearMsg(rmsg, item->m_wearPos, NULL, item);
			SEND_Q(rmsg, pc->m_desc);
			if (item->m_wearPos >= WEARING_SHOW_START && item->m_wearPos <= WEARING_SHOW_END && pc->m_pArea)
			{
				WearingMsg(rmsg, pc, item->m_wearPos, -1, 0);
				pc->m_pArea->SendToCell(rmsg, pc, true);
			}
			pc->m_wearing[item->m_wearPos] = NULL;
			item->m_wearPos = WEARING_NONE;
		}

		ItemDeleteMsg(rmsg, item);
		SEND_Q(rmsg, pc->m_desc);
		RemoveFromInventory(pc, item, true, true);
	}

	if (GetOwnerCharIndex() == pc->m_index)
	{
		m_bGiveLordWeapon = false;
	}
	return true;
}

int CMeracCastle::GetLordItemIndex(char job1, char job2)
{
	if (job2 == 0)
	{
		job2 = GetRandom(1, 2);
	}
	if (job2 < 1 || job2 > 2)
		return -1;

	const int itemindex[JOBCOUNT][2] = {
		{862, 861},
		{864, 863},
		{865, 866},
		{868, 867},
		{869, 870},
		{1071, 1070},
#ifdef NIGHT_SHADOW
		{864, 863},
#endif //NIGHT_SHADOW
	};

	switch (job1)
	{
	case JOB_TITAN:
	case JOB_KNIGHT:
	case JOB_HEALER:
	case JOB_MAGE:
	case JOB_ROGUE:
	case JOB_SORCERER:
		return itemindex[job1][job2 - 1];
	default:
		return -1;
	}
}

bool CMeracCastle::IsDefenseWarpPoint(int pos)
{
	switch (pos)
	{
	case 4:
	case 5:
	case 6:
	case 7:
		return true;
	default:
		return false;
	}
}

void CMeracCastle::InitRegenPosition()
{
	if (m_posRegenPoint)
		delete [] m_posRegenPoint;
	m_nRegenPoint = 0;
	m_posRegenPoint = NULL;

	if (m_countAttackGuild > 0)
	{
		int* tmp = new int[m_countAttackGuild];
		memset(tmp, 0, sizeof(int) * m_countAttackGuild);

		CWarCastleJoin* p = m_attackGuildList;
		int i = 0;
		while (p)
		{
			tmp[i] = p->GetIndex();
			i++;
			p = p->GetNext();
		}

		// 랜덤으로 섞기
		for (i = 0; i < m_countAttackGuild; i++)
		{
			int j = GetRandom(0, m_countAttackGuild - 1);
			int s = tmp[i];
			tmp[i] = tmp[j];
			tmp[j] = s;
		}

		// 리젠 지역 최대 8개
		m_nRegenPoint = (m_countAttackGuild < 8) ? m_countAttackGuild : 8;
		m_posRegenPoint = new int[m_nRegenPoint];

		memcpy(m_posRegenPoint, tmp, sizeof(int) * m_nRegenPoint);

		delete [] tmp;
	}
}

void CMeracCastle::InitGateNPC()
{
	memset(m_gateNPC, 0, sizeof(CNPC*) * COUNT_MERAC_GATE_NPC);
}

void CMeracCastle::GetInnerCastleRect(char* nYlayer, int* nX0x2, int* nZ0x2, int* nX1x2, int* nZ1x2)
{
	int i = gserver.FindZone(GetZoneIndex());
	if (i == -1)
		return ;

	CZone*	pZone	= gserver.m_zones + i;

	*nYlayer		= (char)(pZone->m_zonePos[16][0]);
	*nX0x2			= pZone->m_zonePos[16][1];
	*nZ0x2			= pZone->m_zonePos[16][2];
	*nX1x2			= pZone->m_zonePos[16][3];
	*nZ1x2			= pZone->m_zonePos[16][4];
}

#endif // ENABLE_WAR_CASTLE
