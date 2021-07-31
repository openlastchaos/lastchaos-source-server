#include "stdhdrs.h"
#include "Server.h"
#include "Character.h"
#include "CmdMsg.h"
#include "Log.h"
#include "hardcoding.h"

#ifdef EVENT_XMAS_2006
void DropEventXMas2006Item(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
#ifdef EVENT_XMAS_2006_NODROP_HAT
	// 산타 모자 드롭 방지
#else
	if (level > 0 && level  - npc->m_level <= 5 && GetRandom(1, 1000) <= 3)
	{
		CItem* pItem = npc->m_pArea->DropItem(1844, npc, 0, 0, 1, true);
		if (pItem)
		{
			if (tpc)
				pItem->m_preferenceIndex = tpc->m_index;
			else
				pItem->m_preferenceIndex = -1;

			CNetMsg rmsg;
			ItemDropMsg(rmsg, npc, pItem);
			pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

			// 게임 로그
			GAMELOG << init("EVENT XMAS 2006 DROP ITEM")
					<< "NPC" << delim
					<< npc->m_idNum << delim
					<< npc->m_name << delim
					<< "NPC LEVEL" << delim
					<< npc->m_level << delim
					<< "PC" << delim;
			if (tpc)
				GAMELOG << tpc->m_index << delim << tpc->m_name << delim << tpc->m_nick << delim << tpc->m_desc->m_idname << delim;
			else
				GAMELOG << -1 << delim << "(none)" << delim << "(none)" << delim << "(none)" << delim;
			GAMELOG << "PC LEVEL" << delim
					<< level << delim
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< end;
		}
	}

#endif // EVENT_XMAS_2006_NODROP_HAT
	CNetMsg rmsg;
	bool bDrop = false;

#ifdef EVENT_CARNIVAL
	int nTableDropItem[2][10][4] = {
		{
			{ 723,  0, 200,    5},		// 문스톤 5개	
			{2403,  0, 280,    10},		// 폭죽 10개	
			{ 556,  0, 150,    5},		// 사탕 5개		
			{2625,  0,  60,    1},		// 상의(메이지)	
			{2638,  0,  60,    1},		// 상의(로그)	
			{2620,  0,  60,    1},		// 상의(힐러)	
			{2626,  0,  60,    1},		// 하의(메이지)	
			{2639,  0,  60,    1},		// 하의(로그)	
			{2621,  0,  60,    1},		// 하의(힐러)	
			{  85,  0,  10,    1}		// 고급제련석	
		},
		{
			{ 723,  0, 380,    10},		// 문스톤 10개
			{ 556,  0, 300,    10},		// 초코렛 사탕10개
			{2623,  0,  50,    1},		// 신발(힐러)
			{2641,  0,  50,    1},		// 신발(로그)
			{2628,  0,  50,    1},		// 신발(메이지)
			{2622,  0,  50,    1},		// 장갑(힐러)
			{2640,  0,  50,    1},		// 장갑(로그)
			{2627,  0,  50,    1},		// 장갑(메이지)
			{  85,  0,  15,    1},		// 고급제련석
			{ 779,  0,   5,    1}		// 초고급제련석
		}
	};

	int nTableHeadItem[3][3] = {
		{2619, 3, 1},   // 힐러 축제머리장식
		{2624, 3, 1},	// 메이지 축제머리장식
		{2637, 3, 1},   // 로그 축제머리장식
	};

#else
	int nTableDropItem[2][10][4] = {
		{
			{ 723,  0, 250,    1},		// 문스톤
			{1977,  0, 200,    1},		// 초코렛
			{1975,  0, 150,    1},		// 케이크
			{ 498,  6,  50,    1},		// 붉은 케르 Lv6
			{ 499,  6,  50,    1},		// 붉은 넨 Lv6
			{ 500,  6,  50,    1},		// 푸른 케르 Lv6
			{ 501,  6,  49,    1},		// 푸른 넨 Lv6
			{ 391,  0, 100,    1},		// 이동속도물약
			{  45,  0, 100,    1},		// 체력회복물약 대
			{  85,  0,   1,    1}		// 고급제련석
		},
		{
			{ 723,  0, 200,    1},		// 문스톤
			{1977,  0, 200,    1},		// 초코렛
			{1975,  0, 100,    1},		// 케이크
			{ 498, 12,  50,    1},		// 붉은 케르 Lv12
			{ 500, 12,  50,    1},		// 푸른 케르 Lv12
			{  19,  0,  40, 2000},		// 나스 2,000
			{ 391,  0, 190,    1},		// 이동속도물약
			{  45,  0, 160,    1},		// 체력회복물약 대
			{  85,  0,   9,    1},		// 고급제련석
			{ 779,  0,   1,    1}		// 초고급제련석
		}
	};
#endif // EVENT_CARNIVAL

	int nTableIndex = 0;

	switch (npc->m_proto->m_index)
	{
	case 334:
		nTableIndex = 0;
		break;

	case 335:
		nTableIndex = 1;
		break;

	default:
#ifdef EVENT_CARNIVAL
		bDrop = true;
#else
		return ;
#endif // EVENT_CARNIVAL
	}

#ifdef EVENT_CARNIVAL
	if(bDrop)
	{
		int rand = GetRandom(1, 1000);
		int i;
		int prob = 0;
		for(i = 0; i < 3; ++i)
		{
			prob += nTableHeadItem[i][1];
			if(rand <= prob)
			{
				CItem* item = npc->m_pArea->DropItem(nTableHeadItem[i][0], npc, 0, 0, nTableHeadItem[i][2]);
				if(item)
				{
					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;
					
					ItemDropMsg(rmsg, npc, item);
					item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
					
					// 7. 게임 로그
					GAMELOG << init("EVENT XMAS 2006 DROP ITEM")
						<< "NPC" << delim
						<< npc->m_idNum << delim
						<< npc->m_name << delim
						<< "PC" << delim;
					if (tpc)
						GAMELOG << tpc->m_index << delim << tpc->m_name << delim << tpc->m_nick << delim << tpc->m_desc->m_idname << delim;
					else
						GAMELOG << -1 << delim << "(none)" << delim << "(none)" << delim << "(none)" << delim;
					GAMELOG << "ITEM" << delim
						<< itemlog(item)
						<< end;
				}
				break;
			}
		}
		return ;
	}
#endif // EVENT_CARNIVAL

	int nRandomNumber = GetRandom(1, 1000);
	int i;
	for (i = 0; i < 10; i++)
	{
		if (nRandomNumber <= nTableDropItem[nTableIndex][i][2])
		{
			CItem* pItem = npc->m_pArea->DropItem(nTableDropItem[nTableIndex][i][0], npc, 0, nTableDropItem[nTableIndex][i][1], nTableDropItem[nTableIndex][i][3], true);
			if (pItem)
			{
				if (tpc)
					pItem->m_preferenceIndex = tpc->m_index;
				else
					pItem->m_preferenceIndex = -1;

				ItemDropMsg(rmsg, npc, pItem);
				pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

				// 7. 게임 로그
				GAMELOG << init("EVENT XMAS 2006 DROP ITEM")
						<< "NPC" << delim
						<< npc->m_idNum << delim
						<< npc->m_name << delim
						<< "PC" << delim;
				if (tpc)
					GAMELOG << tpc->m_index << delim << tpc->m_name << delim << tpc->m_nick << delim << tpc->m_desc->m_idname << delim;
				else
					GAMELOG << -1 << delim << "(none)" << delim << "(none)" << delim << "(none)" << delim;
				GAMELOG << "ITEM" << delim
						<< itemlog(pItem)
						<< end;
			}
			return ;
		}
		else
		{
			nRandomNumber -= nTableDropItem[nTableIndex][i][2];
		}
	}

}

bool CArea::EventXMas2006NPCRegen(float x, float z, float r, char y)
{
	void* pos = m_eventXMas2006RegenList.GetHead();
	while (pos)
	{
		CNPCRegenInfo* p = m_eventXMas2006RegenList.GetData(pos);
		pos = m_eventXMas2006RegenList.GetNext(pos);

		if (p && (p->m_lastDieTime != 0))
		{
			// totalnum이 -1이면 무한리젠
			// totalnum이 0이면 스킵
			if (p->m_totalNum == 0)
				continue ;
			//  totalNum이 -1 아니고 numRegen이 totalNum 보다 크면 NPC가 있으면 리젠안함
			if ((p->m_totalNum != -1) && (p->m_numRegen >= p->m_totalNum))
				continue;

#ifdef BSTEST
			if (!p->m_bEventXMasMoveRandom && GetRandom(1, 100) <= 50)
				continue;
#else
			if (!p->m_bEventXMasMoveRandom && GetRandom(1, 1000) <= 994)
				continue;
#endif // BSTEST

			CNPC* npc;
			int cx, cz;
			npc = gserver.m_npcProtoList.Create(p->m_npcIdx, p);
			if(!npc)
			{
				p->m_totalNum = 0;
				continue;
			}

			// 속성맵상 피스존이고 피스 NPC가 아니면 리젠안함
			if (!npc->m_proto->CheckFlag(NPC_PEACEFUL))
			{
				switch (GetAttr(y, x, z))
				{
				case MAPATT_BLOCK:
				case MAPATT_PEACEZONE:
					delete npc;
					continue ;
				}
			}

			// 주변에 다른 상자가 있으면 그곳에는 리젠 안함
			if (this->FindProtoNPCInCell(x, z, 334, false, 2, false) != NULL && this->FindProtoNPCInCell(x, z, 335, false, 2, false) != NULL)
			{
				delete npc;
				continue ;
			}

			GET_X(npc) = x;
			GET_YLAYER(npc) = y;
			GET_Z(npc) = z;

 			// 어택 가능한 NPC
			if (npc->m_proto->CheckFlag(NPC_ATTACK))
				GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			else
				GET_R(npc) = r;

			npc->m_regenX = GET_X(npc);
			npc->m_regenY = GET_YLAYER(npc);
			npc->m_regenZ = GET_Z(npc);
			npc->m_recoverPulse = gserver.m_pulse;

#ifndef NEW_DIVISION_EXPSP
			npc->m_totalDamage = 0;
#endif // #ifndef NEW_DIVISION_EXPSP

			p->m_bAlive = true;
			p->m_numRegen++;

			p->m_bEventXMasMoveRandom = false;
			
			p->m_lastDieTime = 0;
			npc->InitPointsToMax();
			npc->m_disableTime = 1;
			AddNPC(npc);
			PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			CharToCell(npc, GET_YLAYER(npc), cx, cz);

			return true;
		}
	}

	return false;
}
#endif // EVENT_XMAS_2006

#ifdef MONSTER_RAID_SYSTEM
void DropRaidMonsterItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	void*			pos = NULL;
	RAIDDROPDATA	rdd;
	int				nProb;
	CItem*			pItem;
	CNetMsg			rmsg;

	if (level == -1)
		return ;
	if (level  - npc->m_level > 5)
		return ;

	// 레이드 드롭 데이터 처리
	pos = npc->m_proto->m_listRaidDrop.GetHead();
	while (pos)
	{
		rdd = npc->m_proto->m_listRaidDrop.GetData(pos);
		pos = npc->m_proto->m_listRaidDrop.GetNext(pos);
		if (rdd.itemindex > 0)
		{
			// 확률 적용
			nProb = GetRandom(1, 10000);
			if (nProb <= rdd.prob)
			{
				pItem = npc->m_pArea->DropItem(rdd.itemindex, npc, 0, rdd.flag, rdd.count, true);
				if (pItem)
				{
					if (tpc)
						pItem->m_preferenceIndex = tpc->m_index;
					else
						pItem->m_preferenceIndex = -1;

					ItemDropMsg(rmsg, npc, pItem);
					pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

					// 7. 게임 로그
					GAMELOG << init("RAID MOB DROP")
							<< "NPC" << delim
							<< npc->m_idNum << delim
							<< npc->m_name << delim
							<< "NPC LEVEL" << delim
							<< npc->m_level << delim
							<< "PC" << delim;
					if (tpc)
						GAMELOG << tpc->m_index << delim << tpc->m_name << delim << tpc->m_nick << delim << tpc->m_desc->m_idname << delim;
					else
						GAMELOG << -1 << delim << "(none)" << delim << "(none)" << delim << "(none)" << delim;
					GAMELOG << "PC LEVEL" << delim
							<< level << delim
							<< "ITEM" << delim
							<< itemlog(pItem)
							<< end;
				}
			}
		}
	}
}
#endif // MONSTER_RAID_SYSTEM

#ifdef EVENT_CHUSEOK_2006
void DropChuseok2006Item(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	static const int		nProbRice	= 4;	// 쌀가루 : 40%
	static const int		nProbHoney	= 4;	// 꿀 : 40%

	static const int		nIndexRice	= 1814;	// 쌀가루
	static const int		nIndexHoney	= 1815;	// 꿀
	static const int		nIndexPine	= 1816;	// 솔잎

	if (!tpc)
		return ;
	if (tpc->m_pZone->IsPersonalDungeon())
		return ;

	int nProb				= 20;		// 추석 이벤트 드롭 확률 ; 2%
	if (npc->m_level < tpc->m_level)
		nProb -= tpc->m_level - npc->m_level;

	if (GetRandom(1, 1000) <= nProb)
	{
		int nIndexItem = -1;
		nProb = GetRandom(1, 10);
		if (nProb <= nProbRice)						nIndexItem = nIndexRice;
		else if (nProb <= nProbRice + nProbHoney)	nIndexItem = nIndexHoney;
		else										nIndexItem = nIndexPine;

		CItem* pItem = gserver.m_itemProtoList.CreateItem(nIndexItem, -1, 0, 0, 1);
		if (pItem)
		{
			CNetMsg rmsg;
			pItem->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(pItem, npc);
			ItemDropMsg(rmsg, npc, pItem);
			pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

			GAMELOG << init("EVENT CHUSEOK 2006 ITEM DROP", tpc)
					<< "NPC" << delim
					<< npc->m_idNum << delim
					<< npc->m_name << delim
					<< "NPC LEVEL" << delim
					<< npc->m_level << delim
					<< "PC LEVEL" << delim
					<< tpc->m_level << delim
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< end;
		}
	}
}
#endif // EVENT_CHUSEOK_2006

#ifdef EVENT_NEWSERVER_BASTARD
void DropNewserverBastardItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
#if defined (BSTEST) || defined(LC_MAL)
	static const int		nNewServerNo = 1;
	static const int		nManoServerNo = 1;
#else // BSTEST
	static const int		nNewServerNo = 5;
	static const int		nManoServerNo = 4;
#endif // BSTEST

	static const int		nGiftIndex = 1630;
	int						nProb;					// 1/1000

	if (!tpc)
		return ;
	if (tpc->m_pZone->IsPersonalDungeon())
		return ;

	if (npc->m_level >= 65)
	{
		// 65레벨 이상 고정
		if( gserver.m_serverno == nNewServerNo || gserver.m_serverno == nManoServerNo )
			nProb = 15;
		else
			nProb = 8;
		//nProb = (gserver.m_serverno == nNewServerNo) ? 15 : 8;
	}
	else
	{
		if( gserver.m_serverno == nNewServerNo || gserver.m_serverno == nManoServerNo )
			nProb = 24;
		else
			nProb = 12;
		//nProb = (gserver.m_serverno == nNewServerNo) ? 24 : 12;
		// -6부터 2씩 감소
		// +4부터 1씩 증가
		if (tpc->m_level - npc->m_level >= 6)
			nProb -= (tpc->m_level - npc->m_level - 5) * 2;
		else if (npc->m_level - tpc->m_level >= 4)
			nProb += (npc->m_level - tpc->m_level - 3) * 1;
	}

	if (GetRandom(1, 1000) <= nProb)
	{
		CItem* pItem = gserver.m_itemProtoList.CreateItem(nGiftIndex, -1, 0, 0, 1);
		if (!pItem)
			return ;
		CNetMsg rmsg;
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem(pItem, npc);
		ItemDropMsg(rmsg, npc, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

		// 7. 게임 로그
		GAMELOG << init("EVENT NEWSERVER BASTARD DROP", tpc)
				<< "NPC" << delim
				<< npc->m_idNum << delim
				<< npc->m_name << delim
				<< "NPC LEVEL" << delim
				<< npc->m_level << delim
				<< "PC LEVEL" << delim
				<< tpc->m_level << delim
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< end;
	}
}
#endif // EVENT_NEWSERVER_BASTARD

#ifdef EVENT_COLLECT_BUG_DROP
void DropEventCollectBugItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// 곤충 인덱스 및 확률(1/100)
#ifdef LC_JPN
	static const int tableBug[10][2] = {
		{1579, 10},
		{1580, 10},
		{1581, 12},
		{1582, 12},
		{1583, 11},
		{1584, 11},
		{1585,  9},
		{1586,  9},
		{1587,  8},
		{1588,  8},
	};
#else
	static const int tableBug[10][2] = {
		{1579, 19},
		{1580, 17},
		{1581, 15},
		{1582, 13},
		{1583, 11},
		{1584,  9},
		{1585,  7},
		{1586,  5},
		{1587,  3},
		{1588,  1},
	};
#endif

	int		nProb = 0;
	int		i;

	// 1. tpc 검사
	// 2. 퍼던 제외
	// 3. 확률 계산 : 1/1000, 기본 10, 1레벨당 1감소
	// 4. 확률 적용
	// 5. 곤충 결정
	// 6. 아이템 드롭
	// 7. 게임 로그

	// 1. tpc 검사
	if (!tpc)
		return ;

	// 2. 퍼던 제외
	if (tpc->m_pZone->IsPersonalDungeon())
		return ;

	// 3. 확률 계산 : 1/1000, 기본 10, 1레벨당 1감소
	nProb = gserver.m_CollectBugEventPercent;
	if (tpc->m_level > npc->m_level)
		nProb -= tpc->m_level - npc->m_level;

	// 4. 확률 적용
	if (GetRandom(1, 1000) <= nProb)
	{
		// 5. 곤충 결정
		nProb = GetRandom(1, 100);
		for (i = 0; i < 10; i++)
		{
			if (nProb <= tableBug[i][1])
			{
				// 6. 아이템 드롭
				CItem* pItem = gserver.m_itemProtoList.CreateItem(tableBug[i][0], -1, 0, 0, 1);
				if (!pItem)
					return ;
				CNetMsg rmsg;
				pItem->m_preferenceIndex = tpc->m_index;
				npc->m_pArea->DropItem(pItem, npc);
				ItemDropMsg(rmsg, npc, pItem);
				pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

				// 7. 게임 로그
				GAMELOG << init("EVENT COLLECT BUG DROP", tpc)
						<< "NPC" << delim
						<< npc->m_idNum << delim
						<< npc->m_name << delim
						<< "NPC LEVEL" << delim
						<< npc->m_level << delim
						<< "PC LEVEL" << delim
						<< tpc->m_level << delim
						<< "ITEM" << delim
						<< itemlog(pItem)
						<< end;
				return ;
			}
			else
			{
				nProb -= tableBug[i][1];
			}
		}
	}
}
#endif // EVENT_COLLECT_BUG_DROP

#ifdef EVENT_TLD_BUDDHIST
void DropTldBuddhistItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// 모래 봉투 인덱스 및 확률
	static const int tableSand[4][2] = {
		{1571, 5},		// 빨강
		{1572, 10},		// 노랑
		{1573, 10},		// 녹색
		{1574, 10},		// 남색
	};

	int		i;

	// 1. tpc 검사
	// 2. 퍼던 제외
	// 3. 확률 계산 : 1/1000, 기본 50, 1레벨당 5감소
	// 4. 확률 적용
	// 5. 모래 봉투 결정
	// 6. 아이템 드롭
	// 7. 게임 로그

	// 1. tpc 검사
	if (!tpc)
		return ;

	// 2. 퍼던 제외
	if (tpc->m_pZone->IsPersonalDungeon())
		return ;

	if ( tpc->m_level - npc->m_level > 5 )	// 5렙 이상 차이 드롭 안함
		return;

	// 5. 모래 봉투 결정
	for (i = 0; i < 4; i++)
	{
		if ( GetRandom(1, 1000) <= tableSand[i][1])
		{
			// 6. 아이템 드롭
			CItem* pItem = gserver.m_itemProtoList.CreateItem(tableSand[i][0], -1, 0, 0, 1);
			if (!pItem)
				return ;
			CNetMsg rmsg;
			pItem->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(pItem, npc);
			ItemDropMsg(rmsg, npc, pItem);
			pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

			// 7. 게임 로그
			GAMELOG << init("EVENT TLD BUDDHIST DROP", tpc)
					<< "NPC" << delim
					<< npc->m_idNum << delim
					<< npc->m_name << delim
					<< "NPC LEVEL" << delim
					<< npc->m_level << delim
					<< "PC LEVEL" << delim
					<< tpc->m_level << delim
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< end;
			//return ;
		}
	}
}
#endif // EVENT_TLD_BUDDHIST

#ifdef EVENT_RAIN_2006
void DropRain2006Item(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// 아이템 인덱스
#ifdef LC_TLD
	static const int nRaindrop = 1705;
	static const int nRainBottle = 1704;
#else
	static const int nRaindrop = 1518;
	static const int nRainBottle = 1517;
#endif

	// 1. tpc 검사
	// 2. 퍼던 제외
	// 3. 확률 계산 : 1/10000, 기본 138, -4부터 0.2%(20) 감소
	// 4. 확률 적용
	// 5. 빗방울, 빗물병 결정 : 빗방울 80%, 빗물병 20%
	// 6. 아이템 드롭
	// 7. 게임로그

	// 1. tpc 검사
	if (tpc == NULL)
		return ;

	// 2. 퍼던 제외
	if (tpc->m_pZone == NULL || tpc->m_pZone->IsPersonalDungeon())
		return ;

	// 3. 확률 계산 : 1/10000, 기본 138, -4부터 0.2%(20) 감소
#ifdef LC_TLD
	int nProb = 170;
#else
	int nProb = 138;
#endif
	if (tpc->m_level - npc->m_level >= 4)
		nProb -= (tpc->m_level - npc->m_level - 3) * 20;

	// 4. 확률 적용
	if (GetRandom(1, 10000) <= nProb)
	{
		// 5. 빗방울, 빗물병 결정 : 빗방울 80%, 빗물병 20%
		int nItemIndex = -1;
		if (GetRandom(1, 10) <= 8)
			nItemIndex = nRaindrop;
		else
			nItemIndex = nRainBottle;

		// 6. 아이템 드롭
		CItem* pItem = gserver.m_itemProtoList.CreateItem(nItemIndex, -1, 0, 0, 1);
		if (!pItem)
			return ;
		CNetMsg rmsg;
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem(pItem, npc);
		ItemDropMsg(rmsg, npc, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

		// 7. 게임로그
		GAMELOG << init("EVENT RAIN 2006 DROP", tpc)
				<< "NPC" << delim
				<< npc->m_idNum << delim
				<< npc->m_name << delim
				<< "NPC LEVEL" << delim
				<< npc->m_level << delim
				<< "PC LEVEL" << delim
				<< tpc->m_level << delim
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< end;
	}
}
#endif // EVENT_RAIN_2006

void DropGoldenBallItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// 1. 이벤트 진행 상태 검사 : GOLDENBALL_STATUS_VOTE
	// 2. tpc 검사
	// 3. 퍼던 제외
	// 4. 확률 계산 : 1/10000, 기본 180, -4부터 0.2%씩 감소
	// 5. 확률 적용
	// 6. 아이템 드롭
	// 7. 게임로그

	// 1. 이벤트 진행 상태 검사 : GOLDENBALL_STATUS_VOTE
	if (gserver.m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_VOTE)
		return ;

	// 2. tpc 검사
	if (tpc == NULL)
		return ;

	// 3. 퍼던 제외
	if (tpc->m_pZone == NULL || tpc->m_pZone->IsPersonalDungeon())
		return ;

	// 4. 확률 계산 : 1/10000, 기본 180, -4부터 0.2%씩 감소
	int nProb = 180;
	if (tpc->m_level - npc->m_level >= 4)
		nProb -= (tpc->m_level - npc->m_level - 3) * 20;

	// 5. 확률 적용
	if (GetRandom(1, 10000) <= nProb)
	{
		// 6. 아이템 드롭
		CItem* pItem = gserver.m_itemProtoList.CreateItem(GOLDENBALL_BALL_INDEX, -1, 0, 0, 1);
		if (!pItem)
			return ;
		CNetMsg rmsg;
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem(pItem, npc);
		ItemDropMsg(rmsg, npc, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

		// 7. 게임로그
		GAMELOG << init("EVENT GOLDEN BALL DROP", tpc)
				<< "NPC" << delim
				<< npc->m_idNum << delim
				<< npc->m_name << delim
				<< "NPC LEVEL" << delim
				<< npc->m_level << delim
				<< "PC LEVEL" << delim
				<< tpc->m_level << delim
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< end;
	}
}

#ifdef EVENT_WORLDCUP_2006_VOTE
void DropFIFACupItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// FIFA 컵 아이템 인덱스
	static const int nFIFACupIndex = 1484;

	// 1. tpc 검사
	// 2. 퍼스널 던전 제외
	// 3. 확률 계산 : 1/10000, 기본 80, 1레벨당 0.1% 감소
	// 4. 확률 적용
	// 5. 아이템 드롭
	// 6. 게임 로그

	// 1. tpc 검사
	if (tpc == NULL)
		return ;

	// 2. 퍼스널 던전 제외
	if (tpc->m_pZone == NULL || tpc->m_pZone->IsPersonalDungeon())
		return ;

	// 3. 확률 계산 : 1/10000, 1레벨당 0.1% 감소
	int nProb = 80;
	if (npc->m_level < tpc->m_level)
		nProb -= (tpc->m_level - npc->m_level) * 10;

	// 4. 확률 적용
	if (GetRandom(1, 10000) <= nProb)
	{
		// 5. 아이템 드롭
		CItem* pItem = gserver.m_itemProtoList.CreateItem(nFIFACupIndex, -1, 0, 0, 1);
		if (!pItem)
			return ;
		CNetMsg rmsg;
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem(pItem, npc);
		ItemDropMsg(rmsg, npc, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

		// 6. 게임 로그
		GAMELOG << init("EVENT WORLDCUP 2006 FIFA CUP DROP", tpc)
				<< "NPC" << delim
				<< npc->m_idNum << delim
				<< npc->m_name << delim
				<< "NPC LEVEL" << delim
				<< npc->m_level << delim
				<< "PC LEVEL" << delim
				<< tpc->m_level << delim
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< end;
	}
}
#endif // EVENT_WORLDCUP_2006_VOTE

#ifdef RECOMMEND_SERVER_SYSTEM
void DropRecommendItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// 추천 서버에서
	// 우선권 갖는 PC가 있고
	// 파란색 몹을 잡은 게 아니고
	// 30레벨 이하일 경우
	// 20%의 확률로
	if (!gserver.m_bRecommend)
		return ;
	if (!tpc)
		return ;
	if (tpc->m_level - npc->m_level > 5)
		return ;
	if (tpc->m_level > RECOMMEND_SERVER_LEVEL)
		return ;
	if (GetRandom(1, 100) > 20)
		return ;

	CItem* pItem = gserver.m_itemProtoList.CreateItem(RECOMMEND_SERVER_POTION_INDEX, -1, 0, 0, 1);
	if (!pItem)
		return ;

	CNetMsg rmsg;
	pItem->m_preferenceIndex = tpc->m_index;
	npc->m_pArea->DropItem(pItem, npc);
	ItemDropMsg(rmsg, npc, pItem);
	pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
	GAMELOG << init("DROP RECOMMEND SERVER POTION", tpc)
			<< "NPC" << delim
			<< npc->m_idNum << delim
			<< npc->m_name << delim
			<< "NPC LEVEL" << delim
			<< npc->m_level << delim
			<< "PC LEVEL" << delim
			<< tpc->m_level << delim
			<< "ITEM" << delim
			<< itemlog(pItem)
			<< end;

	EventRecommendPotionMsg(rmsg);
	SEND_Q(rmsg, tpc->m_desc);
}
#endif // RECOMMEND_SERVER_SYSTEM

#ifdef DROP_MAKE_DOCUMENT
void DropMakeDocument(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	int nNPCLevel = npc->m_level;
	int i;
	void* pos;
	int nSelItem = -1;

	if (!tpc)
		return ;

	// npc.level < 10 이면 드롭 안함
	if (nNPCLevel < 10)
		return ;

	// tpc.level - npc.level > 2 이면 드롭안함
	if (tpc->m_level - nNPCLevel > 2)
		return ;

	// NPC 레벨별 확률 구하기 : {minLevel, maxLevel, prob(1/1000)}
	const int nSizeTable = 4;
	int tableProb[nSizeTable][3] = {
		{10, 20, 50},		// Lv.10 ~ Lv.20, 5%
		{21, 30, 30},		// Lv.21 ~ Lv.30, 5%
		{31, 40, 20},		// Lv.31 ~ Lv.40, 5%
		{41, 50, 10}		// Lv.41 ~ Lv.50, 5%
	};
	for (i = 0; i < nSizeTable; i++)
	{
		if (tableProb[i][0] <= npc->m_level && npc->m_level <= tableProb[i][1])
		{
			if (GetRandom(1, 1000) <= tableProb[i][2])
				break;
			else
				return ;
		}
	}
	if (i == nSizeTable)
		return ;

	CLCList<int> listItemIndex(NULL);

	// 해당 레벨 대의 아이템 구하기
	for (i = 0; i < gserver.m_itemProtoList.m_nCount; i++)
	{
		const CItemProto* pItemProto = gserver.m_itemProtoList.m_protoItems + i;
		if (pItemProto)
		{
			// item.level - 5 <= npc.level <= item.level - 2 일경우만
			if (pItemProto->m_typeIdx == 2 && pItemProto->m_subtypeIdx == 2 && pItemProto->GetItemProtoLevel() - 5 <= npc->m_level && npc->m_level <= pItemProto->GetItemProtoLevel() - 2)
			{
				listItemIndex.AddToTail(pItemProto->m_index);
			}
		}
	}

	// 해당 레벨 대의 아이템이 없으면 드롭 없음
	if (listItemIndex.GetCount() < 1)
		return ;

	// 특정 아이템 선정 : 각 아이템 선정 확률은 동일
	i = GetRandom(0, listItemIndex.GetCount() - 1);
	pos = listItemIndex.GetHead();
	while (i > 0)
	{
		pos = listItemIndex.GetNext(pos);
		i--;
	}
	if (!pos)
		return ;
	nSelItem = listItemIndex.GetData(pos);

	// 해당 아이템 드롭
	CItem* pItem = gserver.m_itemProtoList.CreateItem(nSelItem, -1, 0, 0, 1);
	if (pItem)
	{
		CNetMsg rmsg;
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem(pItem, npc);
		ItemDropMsg(rmsg, npc, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
		GAMELOG << init("DROP MAKE DOCUMENT", tpc)
				<< "NPC" << delim
				<< npc->m_idNum << delim
				<< npc->m_name << delim
				<< "NPC LEVEL" << delim
				<< npc->m_level << delim
				<< "PC LEVEL" << delim
				<< tpc->m_level << delim
				<< "ITEM" << delim
				<< itemlog(pItem)
				<< end;
	}
}
#endif // DROP_MAKE_DOCUMENT

#ifdef EVENT_NEW_SERVER_2006_NEWSERVER
void DropEventNewServer2006NewServer(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
#ifndef BSTEST
	// 신규 서버에서만
	if (gserver.m_serverno < EVENT_NEW_SERVER_2006_SERVERNO)
		return ;
#endif // #ifndef BSTEST

	// 우선권 PC(기준이 되는 PC) 가 존재시에만
	if (!tpc)
		return ;

	// 자신의 레벨 이상의 몬스터 사냥시
	if (npc->m_level < tpc->m_level)
		return ;

	// 매시간 9분에서 0분 사이일 때
	struct tm tmCur = NOW();
	if (tmCur.tm_min % 10 == 9)
	{
		// 1회에 한해
		if (tpc->m_etcEvent & ETC_EVENT_NEW_SERVER_2006_EXPPOTION)
		{
			// Reset
			tpc->m_etcEvent &= ~ETC_EVENT_NEW_SERVER_2006_EXPPOTION;

			// 60% 확률로
			if (GetRandom(1, 10) <= 6)
			{
				// 당첨 되었으면 Drop 함
				CItem* pItem = gserver.m_itemProtoList.CreateItem(1313, -1, 0, 0, 1);
				if (pItem)
				{
					CNetMsg rmsg;

					pItem->m_preferenceIndex = tpc->m_index;
					npc->m_pArea->DropItem(pItem, npc);
					ItemDropMsg(rmsg, npc, pItem);
					pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
					GAMELOG << init("EVENT NEW SERVER 2006", tpc)
							<< "ITEM" << delim
							<< itemlog(pItem)
							<< end;
				}
			}
		}
	}
	else
	{
		// 9분이 아닐때는 다음에 되도록 설정
		tpc->m_etcEvent |= ETC_EVENT_NEW_SERVER_2006_EXPPOTION;
	}
}
#endif // EVENT_NEW_SERVER_2006_NEWSERVER

#if defined(EVENT_NEW_SERVER_2006_OLDSERVER) || defined(EVENT_NEW_SERVER_2006_NEWSERVER)
void DropEventNewServer2006StatPotion(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// 기존 서버는 제외되고 신규 서버만 정의되면 서버군 번호를 검사한다
#if !defined(EVENT_NEW_SERVER_2006_OLDSERVER) && defined(EVENT_NEW_SERVER_2006_NEWSERVER)
	if (gserver.m_serverno < EVENT_NEW_SERVER_2006_SERVERNO)
		return ;
#endif

	// 우선권 PC(기준이 되는 PC) 가 존재시에만
	if (!tpc)
		return ;

	// 자신의 레벨 이상의 몬스터 사냥시
	if (npc->m_level < tpc->m_level)
		return ;

	// 매시간 9분에서 0분 사이일 때
	struct tm tmCur = NOW();
	if (tmCur.tm_min % 10 == 9)
	{
		// 1회에 한해
		if (tpc->m_etcEvent & ETC_EVENT_NEW_SERVER_2006_STATPOTION)
		{
			// Reset
			tpc->m_etcEvent &= ~ETC_EVENT_NEW_SERVER_2006_STATPOTION;

			// 90% 확률로
			if (GetRandom(1, 10) <= 9)
			{
				int prob = GetRandom(1, 10);
				int droptable[4][2] = {
					{1316,	2},			// 공격력 향상제(소), 20%
					{1317,	5},			// 방어력 향상제(소), 50%
					{1315,	1},			// 공격력 향상제(대), 10%
					{1314,	2}			// 방어력 향상제(대), 20%
				};
				int i;
				for (i = 0; i < 4; i++)
				{
					if (prob <= droptable[i][1])
					{
						// 당첨 되었으면 Drop 함
						CItem* pItem = gserver.m_itemProtoList.CreateItem(droptable[i][0], -1, 0, 0, 1);
						if (pItem)
						{
							CNetMsg rmsg;

							pItem->m_preferenceIndex = tpc->m_index;
							npc->m_pArea->DropItem(pItem, npc);
							ItemDropMsg(rmsg, npc, pItem);
							pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
							GAMELOG << init("EVENT NEW SERVER 2006", tpc)
									<< "ITEM" << delim
									<< itemlog(pItem)
									<< end;
							// 루프 종료
							break;
						}
					}
					// 미 당첨시 확률 조정 : 다음 확률 계산에 누적되도록 함
					prob -= droptable[i][1];
				}
			}
		}
	}
	else
	{
		// 9분이 아닐때는 다음에 되도록 설정
		tpc->m_etcEvent |= ETC_EVENT_NEW_SERVER_2006_STATPOTION;
	}
}
#endif // #if defined(EVENT_NEW_SERVER_2006_OLDSERVER) || defined(EVENT_NEW_SERVER_2006_NEWSERVER)

#ifdef EVENT_WHITEDAY_2006
void DropEventWhiteDay2006(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc)
		return ;

	int prob;

	CNetMsg rmsg;

	// 막대사탕 : 3%
	prob = 30;
	if (tpc->m_level > npc->m_level)
		prob -= 5 * (tpc->m_level - npc->m_level);
	if (prob < 0)
		prob = 0;
	if (prob < 10 && tpc->m_level >= 60)
		prob = 10;
	if (GetRandom(1, 1000) <= prob)
	{
		CItem* item = gserver.m_itemProtoList.CreateItem(556, -1, 0, 0, 1);
		if (item)
		{
			item->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(item, npc);
			ItemDropMsg(rmsg, npc, item);
			item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

			GAMELOG << init("EVENT WHITEDAY 2006", tpc)
					<< "ITEM" << delim
					<< itemlog(item)
					<< end;
		}
	}

	// 알사탕 : 1%
	prob = 10;
	if (tpc->m_level > npc->m_level)
		prob -= 5 * (tpc->m_level - npc->m_level);
	if (prob < 0)
		prob = 0;
	if (prob < 10 && tpc->m_level >= 60)
		prob = 10;
	if (GetRandom(1, 1000) <= prob)
	{
		CItem* item = gserver.m_itemProtoList.CreateItem(1307, -1, 0, 0, 1);
		if (item)
		{
			item->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(item, npc);
			ItemDropMsg(rmsg, npc, item);
			item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

			GAMELOG << init("EVENT WHITEDAY 2006", tpc)
					<< "ITEM" << delim
					<< itemlog(item)
					<< end;
		}
	}
}
#endif // #ifdef EVENT_WHITEDAY_2006

#ifdef EVENT_VALENTINE_2006
void DropEventValentine2006(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc)
		return ;
	if (!gserver.m_bEventValentine2006)
		return ;

	int prob = 30;			// 1/1000
	const int minProb = 3;	// 1/1000

	if (tpc->m_level > npc->m_level)
		prob -= 5 * (tpc->m_level - npc->m_level);
	if (prob < minProb)
		prob = minProb;

	int idxItem[2] = {1226, 1227};

	CNetMsg rmsg;
	int i;
	for (i = 0; i < 2; i++)
	{
		if (GetRandom(1, 1000) <= prob)
		{
			CItem* item = gserver.m_itemProtoList.CreateItem(idxItem[i], -1, 0, 0, 1);
			if (item)
			{
				item->m_preferenceIndex = tpc->m_index;
				npc->m_pArea->DropItem(item, npc);
				ItemDropMsg(rmsg, npc, item);
				item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				GAMELOG << init("EVENT VALENTINE 2006", tpc)
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
			}
		}
	}
}
#endif // #ifdef EVENT_VALENTINE_2006

#ifdef EVENT_XMAS_2005
void DropEventXMas2005(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// 이벤트 기간중에
	// tpc를 기준으로
	// tpc의 레벨이 11 이상일때
	// tpc가 최근 10분동안 얻은 이벤트 아이템이 없고
	if (gserver.m_bXMas2005Event
			&& tpc
			&& tpc->m_level > 10
			&& (tpc->m_pulseTakeEventXMas2005 == 0 || tpc->m_pulseTakeEventXMas2005 + (10 * PULSE_REAL_MIN) <= gserver.m_pulse))
	{
		// 1레벨 낮을 때마다 0.2% 확률 감소
		int nPenalty = (tpc->m_level - npc->m_level) * 20;
		if (nPenalty < 0)
			nPenalty = 0;

		// 희망 얻을 확률
		int nProbGoods1 = 500 - nPenalty;
		// 꿈 얻을 확률
		int nProbGoods2 = 2000 - nPenalty;

		// 생성 아이템
		CItem* itemGoods = NULL;

		// 희망 주사위
		if (GetRandom(1, 10000) <= nProbGoods1)
		{
			// 희망 아이템 생성
			itemGoods = gserver.m_itemProtoList.CreateItem(1135, -1, 0, 0, 1);
		}

		// 꿈 주사위 : 희망 드롭 실패시에만
		else if (GetRandom(1, 10000) <= nProbGoods2)
		{
			// 꿈 아이템 생성
			itemGoods = gserver.m_itemProtoList.CreateItem(1134, -1, 0, 0, 1);
		}

		// 아이템 드롭
		if (itemGoods)
		{
			CNetMsg rmsg;

			// 아이템 드롭
			itemGoods->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(itemGoods, npc);
			ItemDropMsg(rmsg, npc, itemGoods);
			itemGoods->m_pArea->SendToCell(rmsg, GET_YLAYER(itemGoods), itemGoods->m_cellX, itemGoods->m_cellZ);

			// 시간 갱신
			tpc->m_pulseTakeEventXMas2005 = gserver.m_pulse;

			// 로그 작성
			GAMELOG << init("EVENT XMAS 2005", tpc)
					<< "ZONE" << delim
					<< npc->m_pZone->m_index << delim
					<< "NPC" << delim
					<< npc->m_idNum << delim
					<< npc->m_name << delim
					<< "ITEM" << delim
					<< itemlog(itemGoods)
					<< end;
		}
	}
}
#endif // #ifdef EVENT_XMAS_2005

#ifdef EVENT_PEPERO
void DropEventPeperoItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// 우선권 소유 PC가 잡을때만
	if (!tpc || tpc != pc)
		return ;

	// 5레벨 이하, 퍼던이 아닐때
	if (tpc->m_level - npc->m_level <= 5)
	{
		// 확률 조정
		int prob = 300;
		if (tpc->m_pZone->IsPersonalDungeon())
			prob /= 2;

		// 랜덤 굴리기
		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(1056, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;
				item->m_preferenceIndex = tpc->m_index;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				GAMELOG << init("EVENT PEPERO DROP", tpc)
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
			}
		}

		// 확률 조정
		prob = 100;
		if (tpc->m_pZone->IsPersonalDungeon())
			prob /= 2;

		// 랜덤 굴리기
		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(1057, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;
				item->m_preferenceIndex = tpc->m_index;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				GAMELOG << init("EVENT PEPERO DROP", tpc)
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
			}
		}
	}
}
#endif // #ifdef EVENT_PEPERO

#ifdef EVENT_2PAN4PAN_MONEY
void DropEvent2pan4panMoneyCard(CNPC* npc, CPC* tpc, int level)
{
	if (level != -1 && level - npc->m_level <= 5)
	{
		int prob = 50;
		if (npc->m_pZone->IsPersonalDungeon())
			prob /= 2;

		if (GetRandom(1, 10000) <= prob)
		{
			// 맞고 10만
			CItem* item = npc->m_pArea->DropItem(1051, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;

				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			}
		}

		if (GetRandom(1, 10000) <= prob)
		{
			// 포커 5천만
			CItem* item = npc->m_pArea->DropItem(1052, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;

				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			}
		}

		prob = 300;
		if (npc->m_pZone->IsPersonalDungeon())
			prob /= 2;

		if (GetRandom(1, 10000) <= prob)
		{
			// 맞고 2만
			CItem* item = npc->m_pArea->DropItem(1055, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;

				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			}
		}

		if (GetRandom(1, 10000) <= prob)
		{
			// 포커 1천만
			CItem* item = npc->m_pArea->DropItem(1054, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;

				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			}
		}
	}
}
#endif // #ifdef EVENT_2PAN4PAN_MONEY

#ifdef EVENT_SEPTEMBER
void DropEventSeptemberItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// 우선권 소유 PC가 잡을때만
	if (!tpc || tpc != pc)
		return ;

	// 5레벨 이하, 퍼던이 아닐때
	if (tpc->m_level - npc->m_level <= 5 && !npc->m_pZone->IsPersonalDungeon() && npc->m_level > 5)
	{
		// 확률 조정
		int prob = 250;
		if (tpc->m_level > npc->m_level)
			prob -= (tpc->m_level - npc->m_level) * 20;

		int dbindex[] = {882, 883, 884, 885};
		int i;
		for (i = 0; i < 4; i++)
		{
			// 랜덤 굴리기
			if (GetRandom(1, 10000) <= prob)
			{
				CItem* item = npc->m_pArea->DropItem(dbindex[i], npc, 0, 0, 1);
				if (item)
				{
					CNetMsg rmsg;
					item->m_preferenceIndex = tpc->m_index;
					ItemDropMsg(rmsg, npc, item);
					npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

					GAMELOG << init("EVENT SEPTEMBER DROP", tpc)
							<< "ITEM" << delim
							<< itemlog(item)
							<< end;
				}
			}
		}
	}
}
#endif // #ifdef EVENT_SEPTEMBER

void DropEventChuseokItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
#if defined( LC_TLD ) && defined (EVENT_OPEN_BETA_TLD)
	// 우선권을 소유하게 되는 PC가 있을 때
	if (!tpc || tpc != pc)
		return ;

	if(!gserver.m_bOpenEvent)
		return;

	// 작물 드롭
	// 이벤트 인벤에 작물이 없을 때
	int r, c;
	if (!tpc->m_invenEvent.FindItem(&r, &c, 876, 0, 0) &&		// 씨앗
		!tpc->m_invenEvent.FindItem(&r, &c, 877, 0, 0) &&		// 묘목
		!tpc->m_invenEvent.FindItem(&r, &c, 878, 0, 0) &&		// 분재
		!tpc->m_invenEvent.FindItem(&r, &c, 879, 0, 0) &&		// 나무
		!tpc->m_invenEvent.FindItem(&r, &c, 880, 0, 0) &&		// 열매
		!(tpc->m_etcEvent & OPEN_EVENT_REWARDED) )				// 이미 보상 받은적이 없으면
	{
		// 확률 조정
		int prob = 200;

		if (GetRandom(1, 10000) <= prob)
		{
			// 랜덤에 성공
			// 아이템 생성
			CItem* item = gserver.m_itemProtoList.CreateItem(876, -1, 0, 0, 1);
			if (item)
			{
				if (AddToInventory(tpc, item, true, true))
				{
					CNetMsg rmsg;
					ItemAddMsg(rmsg, item);
					SEND_Q(rmsg, tpc->m_desc);

					GAMELOG << init("EVENT OPEN BETA GET SEED", tpc->m_name, tpc->m_desc->m_idname)
							<< end;
				}
				else
					delete item;
			}
		}
	}

	// 5레벨 이하 몹이 아니고, 퍼스널 던전이 아니고 몹이 6레벨 이상일때
	if (tpc->m_level - npc->m_level <= 5 && !npc->m_pZone->IsPersonalDungeon() && npc->m_level > 5)
	{

		// 배양토 드롭
		int prob = 150;

		if( gserver.m_bSoilDrop == 0 )
			prob *= 2;

		// 몹이 저렙이면 1렙당 0.05% 감소
		if (tpc->m_level > npc->m_level)
			prob -= (tpc->m_level - npc->m_level) * 5;

		if (GetRandom(1, 10000) <= prob)
		{
			// 랜덤 성공

			int itemIndex = -1;

			// 0 : 모두 드롭 1: 배양토 2: 레드 3: 푸른 4: 황금 5: 배양,레드 6:레드,푸른 7:푸른,골드 8: 모두 안 드롭
			switch(gserver.m_bSoilDrop)
			{
			case 0:
				{
					switch( rand() % 4 )
					{
					case 0:
						itemIndex = 881;
						break;
					case 1:
						itemIndex = 1058;
						break;
					case 2:
						itemIndex = 1059;
						break;
					case 3:
						itemIndex = 1060;
						break;
					default : 
						return;
					}
				}
				break;
			case 1:
				itemIndex = 881;
				break;
			case 2:
				itemIndex = 1058;
				break;
			case 3:
				itemIndex = 1059;
				break;
			case 4:
				itemIndex = 1060;
				break;
			case 5:
				{
					if( rand() % 2)
						itemIndex = 881;
					else
						itemIndex = 1058;
				}
				break;
			case 6:
				{
					if( rand() % 2)
						itemIndex = 1058;
					else
						itemIndex = 1059;
				}
				break;
			case 7:
				{
					if( rand() % 2)
						itemIndex = 1059;
					else
						itemIndex = 1060;
				}
				break;
			default:
				return;
			}
			

			// 배양토 881, 붉은 1058, 푸른 1059, 황금 1060
			CItem* item = npc->m_pArea->DropItem(itemIndex, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;
				item->m_preferenceIndex = tpc->m_index;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				GAMELOG << init("EVENT OPEN BETA SOIL", tpc->m_name, tpc->m_desc->m_idname)
						<< end;
			}
		}
	}
#else
	// 우선권을 소유하게 되는 PC가 있을 때
	if (!tpc || tpc != pc)
		return ;

	// 5레벨 이하 몹이 아니고, 퍼스널 던전이 아니고 몹이 6레벨 이상일때
	if (tpc->m_level - npc->m_level <= 5 && !npc->m_pZone->IsPersonalDungeon() && npc->m_level > 5)
	{
		// 작물 드롭
		// 이벤트 인벤에 작물이 없을 때
		int r, c;
		if (!tpc->m_invenEvent.FindItem(&r, &c, 876, 0, 0) &&		// 씨앗
			!tpc->m_invenEvent.FindItem(&r, &c, 877, 0, 0) &&		// 묘목
			!tpc->m_invenEvent.FindItem(&r, &c, 878, 0, 0) &&		// 분재
			!tpc->m_invenEvent.FindItem(&r, &c, 879, 0, 0) &&		// 나무
			!tpc->m_invenEvent.FindItem(&r, &c, 880, 0, 0))		// 열매
		{
			// 확률 조정
			int prob = 200;

			// 몹이 저렙이면 1렙당 0.1%감소
			if (tpc->m_level > npc->m_level)
				prob -= (tpc->m_level - npc->m_level) * 10;

			if (GetRandom(1, 10000) <= prob)
			{
				// 랜덤에 성공
				// 아이템 생성
				CItem* item = gserver.m_itemProtoList.CreateItem(876, -1, 0, 0, 1);
				if (item)
				{
					if (AddToInventory(tpc, item, true, true))
					{
						CNetMsg rmsg;
						ItemAddMsg(rmsg, item);
						SEND_Q(rmsg, tpc->m_desc);

						GAMELOG << init("EVENT CHUSEOK GET SEED", tpc)
								<< end;
					}
					else
						delete item;
				}
			}
		}

		// 배양토 드롭
		int prob = 100;

		// 몹이 저렙이면 1렙당 0.05% 감소
		if (tpc->m_level > npc->m_level)
			prob -= (tpc->m_level - npc->m_level) * 5;

		if (GetRandom(1, 10000) <= prob)
		{
			// 랜덤 성공
			CItem* item = npc->m_pArea->DropItem(881, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;
				item->m_preferenceIndex = tpc->m_index;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				GAMELOG << init("EVENT CHUSEOK DROP SOIL", tpc)
						<< end;
			}
		}
	}
#endif
}

#ifdef EVENT_FLOWER
void DropFlowerItem(int level, CNPC* npc, CPC* pc, CPC* tpc)
{
	if ( gserver.m_bFlowerEvent )
	{
		struct tm tm_now = NOW();

		if(pc->m_etcEvent & FLOWER_EVENT_DROP && (tm_now.tm_min%10) == 9)
		{
			if (level != -1 && level - npc->m_level <= 5)
			{
				int prob = 2500;
				
				if (pc->m_pZone->IsPersonalDungeon())
					prob /= 2;

				if (GetRandom(1, 10000) <= prob)
				{
					// 꽃 아이템
					// 공격력 10프로, 방어력 20프로, 낮은 공격 20, 낮은 방어력 50 : 할일
					int itemIndex = -1;

					switch(gserver.m_bFlower)
					{
						// 0 : all 1 : 방어력 2 : 공격력 3: 낮은 방어력 4: 낮은 공격력 5: 높은 방어력 6: 높은 공격력
					case 0:
						{
							int result = GetRandom(1, 10000);
							if( result <= 1000 )
								itemIndex = 1075;
							else if( 1000 < result && result <= 3000 ) 
								itemIndex = 1076;
							else if( 3000 < result && result <= 5000 ) 
								itemIndex = 1073;
							else if( 5000 < result && result <= 10000 ) 
								itemIndex = 1074;
						}
						break;
					case 1:
						{
							if( rand() % 2 )
								itemIndex = 1074;
							else
								itemIndex = 1076;
						}
						break;
					case 2:
						{
							if( rand() % 2 )
								itemIndex = 1073;
							else
								itemIndex = 1075;
						}
						break;
					case 3:
						{
							itemIndex = 1074;
						}
						break;
					case 4:
						{
							itemIndex = 1073;
						}
						break;
					case 5:
						{
							itemIndex = 1076;
						}
						break;
					case 6:
						{
							itemIndex = 1075;
						}
						break;
					default :
						return;
					}

					CItem* item = npc->m_pArea->DropItem(itemIndex, npc, 0, 0, 1);
					if (item)
					{
						CNetMsg dropMsg;
						ItemDropMsg(dropMsg, npc, item);
						npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
						
						if (tpc)
							item->m_preferenceIndex = tpc->m_index;
						else
							item->m_preferenceIndex = -1;

						GAMELOG << init("EVENT_FLOWER_DROP")
								<< "NPC INDEX" << delim
								<< npc->m_proto->m_index << delim
								<< "NPC NAME" << delim
								<< npc->m_name << delim
								<< "ITEM" << delim
								<< itemlog(item)
								<< end;
					}
				}
			}
			pc->m_etcEvent = pc->m_etcEvent &~ FLOWER_EVENT_DROP;
		}
		else
		{
			if( (tm_now.tm_min%10) != 9 )
				pc->m_etcEvent = pc->m_etcEvent | FLOWER_EVENT_DROP;
		}
	}
}
#endif

void DropLuckySpecialStone(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
#ifdef LC_TWN2
//	152	바알	3%
//190	데스나이트	5%
//191	와일드 울프	0.5%
//201	데빌로드 카미라	1%
//215	켈베로스	5% --> 0.01% 060314: 인스리아 요청

//이상설정한 몬스터외 lv32급에NPC151( 붉은다리 거미)이상등급의 몬스터는 0.5%확율에  드룹율을 
	if( npc->m_level < 32 && npc->m_proto->m_index != 191 )
		return;

	if (level != -1 && level - npc->m_level <= 5)
	{
		int prob = 0;

		switch(npc->m_proto->m_index)
		{
		case 152:
			prob = 300;
			break;
		case 190:
			prob = 500;
			break;
		case 201:
			prob = 100;
			break;
		case 215:
			prob = 10;
			break;
		case 191:
			prob = 50;
			break;
		default:
			{
				prob = 10;
			}
		}

		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(974, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
			}
		}
	}
#endif
}

void DropBloodGem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
#ifdef LC_TWN2
	if (level != -1 && level - npc->m_level <= 5 && npc->m_level >= 17)
	{
		int prob = 300;

		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(284, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;

				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			}
		}
	}
#endif
}

#ifdef EVENT_FRUIT_WATERMELON
void DropEventWaterMelon(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (level != -1 && level - npc->m_level <= 5)
	{
		int prob = 100;
		if (npc->m_pZone->IsPersonalDungeon())
			prob /= 2;

		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(836, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;

				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			}
		}
	}
}
#endif

#ifdef EVENT_FRUIT_MELON
void DropEventMelon(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (level != -1 && level - npc->m_level <= 5)
	{
		int prob = 300;
		if (npc->m_pZone->IsPersonalDungeon())
			prob /= 2;

		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(837, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;

				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			}
		}
	}
}
#endif

#ifdef EVENT_FRUIT_PLUM
void DropEventPlum(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (level != -1 && level - npc->m_level <= 5)
	{
		int prob = 300;
		if (npc->m_pZone->IsPersonalDungeon())
			prob /= 2;

		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(838, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg rmsg;

				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			}
		}
	}
}
#endif

void DropBoosterItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
#if defined (LC_TLD) || defined (LC_JPN)
#else
	if (level != -1 && level - npc->m_level <= 5 && npc->m_level >= 6 && !npc->m_pZone->IsPersonalDungeon())
	{
		int prob = 54;
		if (npc->m_level >= 31)
			prob -= (level - 30) * 2;

#ifdef EVENT_NEW_SERVER_2006_NEWSERVER
#ifdef BSTEST
		prob *= 2;
#else // BSTEST
		// 신규서버는 2배
		if (gserver.m_serverno >= EVENT_NEW_SERVER_2006_SERVERNO)
		{
			prob *= 2;
		}
#endif // BSTEST
#endif // EVENT_NEW_SERVER_2006_NEWSERVER

		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(786, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg dropMsg;
				
				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
				ItemDropMsg(dropMsg, npc, item);
				npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				GAMELOG << init("MOB DROP BOOSTER")
						<< "NPC INDEX" << delim
						<< npc->m_proto->m_index << delim
						<< "NPC NAME" << delim
						<< npc->m_name << delim
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
			}
		}
	}
#endif // #ifndef LC_TLD
}

#ifdef EVENT_RICESOUP
void DropRiceSoupItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (gserver.m_bRiceShoupEvent)
	{	
		if (level != -1 && level - npc->m_level <= 5)
		{
			// 떡국 드롭
			if (GetRandom(1, 10000) <= NEWYEAR_EVENT_RICESOUP_DROP_PROB)
			{
				CItem* item = npc->m_pArea->DropItem(508, npc, 0, 0, 1);
				if (item)
				{
					CNetMsg dropMsg;
					
					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;
					ItemDropMsg(dropMsg, npc, item);
					npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
				}
			}
			
			// 떡만두국 드롭
			if (GetRandom(1, 10000) <= NEWYEAR_EVENT_RICESOUP_DROP_PROB)
			{
				CItem* item = npc->m_pArea->DropItem(509, npc, 0, 0, 1);
				if (item)
				{
					CNetMsg dropMsg;
					
					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;
					ItemDropMsg(dropMsg, npc, item);
					npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
				}
			}
		}
	}
}
#endif


// 인비지 푸는 아이템 사용시
int ApplyItem677_or_3579(CCharacter* ch, CSkill* skill, int itemidx)
{
	CArea* area = ch->m_pArea;

	int sx = ch->m_cellX - CELL_EXT;
	int ex = ch->m_cellX + CELL_EXT;
	int sz = ch->m_cellZ - CELL_EXT;
	int ez = ch->m_cellZ + CELL_EXT;
	
	int x, z;
	int count = skill->m_proto->m_targetNum;

	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= area->m_size[0]) continue;
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= area->m_size[1]) continue;
			CCharacter *pChar;
			for (pChar = area->m_cell[x][z].m_listChar; pChar; pChar = pChar->m_pCellNext)
			{
				if (GetDistance(ch, pChar) <= skill->m_proto->m_appRange)
				{
					if (pChar->IsInvisible())
					{
						count--;
						bool bApply;
						ApplySkill(ch, pChar, skill, itemidx, bApply);
						if (count < 1)
							return 0;
					}
				}
			}
		}
	}
	
	return 0;
}


// 퍼스널 던전3 입장권 주기
#if defined (ENABLE_SINGLEDUNGEON_DATA)
void DropPersonalDungeon3Ticket(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!pc)
		return ;

	// pc level이 30이상
	if (pc->m_level < gserver.m_questProtoList.FindProto(104)->m_needMinLevel || pc->m_level > gserver.m_questProtoList.FindProto(104)->m_needMaxLevel )
		return ;

	// 확률 : 5%
	if (GetRandom(1, 100) > 5)
		return ;

	// 입장이후 하루 지났는가?
#ifdef ENABLE_SINGLEDUNGEON_DATA
	if (pc->m_pd3Count >= 5)
#else
	time_t pd3time = pc->m_pd3time * 60 * 60 * 24;
	struct tm tm1, tm2;
	tm1 = NOW();
	memcpy(&tm2, localtime(&pd3time), sizeof(struct tm));
	if (tm1.tm_yday == tm2.tm_yday)
#endif
		return ;

	// 입장권을 소지하고 있나?
	int row, col;
	if (pc->m_invenNormal.FindItem(&row, &col, 725, 0, 0))
		return ;

	// 레벨 차이는? : pc->level - npc->level <= 5
	if (pc->m_level - npc->m_level > 5)
		return ;

	// 아이템 생성 후 인벤토리에 넣기
	CItem* item = gserver.m_itemProtoList.CreateItem(725, -1, 0, 0, 1);
	if (!item)
		return ;

	if (!AddToInventory(pc, item, false, true))
	{
		delete item;
		return ;
	}

	CNetMsg rmsg;
	ItemAddMsg(rmsg, item);
	SEND_Q(rmsg, pc->m_desc);
}
#endif

#ifdef ENABLE_SINGLEDUNGEON_DATA
void DropPersonalDungeon4Ticket(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!pc)
		return ;

	if (pc->m_level < gserver.m_questProtoList.FindProto(105)->m_needMinLevel || pc->m_level > gserver.m_questProtoList.FindProto(105)->m_needMaxLevel )
		return ;

	// 확률 : 5%
	if (GetRandom(1, 100) > 5)
		return ;

	// 입장이후 하루 지났는가?
	// 그외 3회
	if (pc->m_pd4Count >= 3)
		return;

	// 입장권을 소지하고 있나?
	int row, col;
	if (pc->m_invenNormal.FindItem(&row, &col, 757, 0, 0))
		return ;

	// 레벨 차이는? : pc->level - npc->level <= 5
	if (pc->m_level - npc->m_level > 5)
		return ;

	// 아이템 생성 후 인벤토리에 넣기
	CItem* item = gserver.m_itemProtoList.CreateItem(757, -1, 0, 0, 1);
	if (!item)
		return ;

	if (!AddToInventory(pc, item, false, true))
	{
		delete item;
		return ;
	}

	CNetMsg rmsg;
	ItemAddMsg(rmsg, item);
	SEND_Q(rmsg, pc->m_desc);
}
#endif

#ifdef ENABLE_DROP_PETEGG
void DropPetEgg(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (level != -1 && level - npc->m_level <= 5)
	{
#ifdef LC_TLD
		int prob = 100;

		if( tpc )
		{
			int r, c;

			// 판의 피리나 드레이크 알을 가지고 있으면 드롭하지 않는다
			// or 펫이 있거나
			if( tpc->m_invenNormal.FindItem(&r, &c, 948, 0, 0) 
				|| tpc->m_invenNormal.FindItem(&r, &c, 949, 0, 0) 
				|| tpc->m_petList )
				return;
		}
#else
		int prob = 30;
#endif
		if (prob < 0)
			return ;

		if (npc->m_pZone->IsPersonalDungeon())
			prob /= 2;

		int ran = rand()%2;

		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(948+ran, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg dropMsg;
				
				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
				ItemDropMsg(dropMsg, npc, item);
				npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				GAMELOG << init("MOB_DROP_PETEGG")
						<< "NPC INDEX" << delim
						<< npc->m_proto->m_index << delim
						<< "NPC NAME" << delim
						<< npc->m_name << delim
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
			}
		}
	}
}
#endif

// 고제드롭
void DropSpecialRefineStone(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (level != -1 && level - npc->m_level <= 5)
	{
		int prob = ((npc->m_level * 2) + 25) / 10;
		if (prob < 0)
			return ;

		if (npc->m_pZone->IsPersonalDungeon())
			prob /= 2;
#ifdef LC_TLD // 태국 고제 드롭율 10프로 증가
		prob += prob * 10 / 100;
#endif
		
#if defined( LC_MAL ) || defined ( LC_BRZ )	// 말레시아 고제 트롭율 50% 감소		/// kj
		prob /=  2;
#endif

		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(85, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg dropMsg;
				
				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
				ItemDropMsg(dropMsg, npc, item);
				npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				GAMELOG << init("MOB DROP SPECIALREFINE")
						<< "NPC INDEX" << delim
						<< npc->m_proto->m_index << delim
						<< "NPC NAME" << delim
						<< npc->m_name << delim
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
			}
		}
	}
}


#ifdef EVENT_WHITEDAY
void DropWhiteDayItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (gserver.m_bWhiteDayEvent)
	{	
		if (level != -1 && level - npc->m_level <= 5)
		{
			int prob;

			if (npc->m_pZone->IsPersonalDungeon())
				prob = 200;
			else
				prob = 600;

			// 사탕 드롭
			if (GetRandom(1, 10000) <= prob)
			{
				CItem* item = npc->m_pArea->DropItem(556, npc, 0, 0, 1);
				if (item)
				{
					CNetMsg dropMsg;
					
					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;
					ItemDropMsg(dropMsg, npc, item);
					npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
				}
			}
		}
	}
}
#endif


#ifdef EVENT_VALENTINE
void DropValentineItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (gserver.m_bValentineEvent)
	{	
		if (level != -1 && level - npc->m_level <= 5)
		{
			int prob;

			if (npc->m_pZone->IsPersonalDungeon())
				prob = 200;
			else
				prob = 600;

			// 초컬릿 드롭
			if (GetRandom(1, 10000) <= prob)
			{
				CItem* item = npc->m_pArea->DropItem(512, npc, 0, 0, 1);
				if (item)
				{
					CNetMsg dropMsg;
					
					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;
					ItemDropMsg(dropMsg, npc, item);
					npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
				}
			}
		}
	}
}
#endif


#ifdef EVENT_NEWYEAR
void DropNewYearItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (gserver.m_bNewYearEvent)
	{	
		if (level != -1 && level - npc->m_level <= 5)
		{
			// 떡국 드롭
			if (GetRandom(1, 10000) <= NEWYEAR_EVENT_RICESOUP_DROP_PROB)
			{
				CItem* item = npc->m_pArea->DropItem(508, npc, 0, 0, 1);
				if (item)
				{
					CNetMsg dropMsg;
					
					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;
					ItemDropMsg(dropMsg, npc, item);
					npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
				}
			}
			
			// 떡만두국 드롭
			if (GetRandom(1, 10000) <= NEWYEAR_EVENT_RICESOUP_DROP_PROB)
			{
				CItem* item = npc->m_pArea->DropItem(509, npc, 0, 0, 1);
				if (item)
				{
					CNetMsg dropMsg;
					
					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;
					ItemDropMsg(dropMsg, npc, item);
					npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
				}
			}
		}
	}
}
#endif


void DropPersonalDungeon2Ticket(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (npc->m_level >= S2_TICKET_MOB_START && npc->m_level <= S2_TICKET_MOB_END)
	{
		if (GetRandom(1, 10000) <= S2_TICKET_DROP_PROB)
		{
			CItem* ticket = npc->m_pArea->DropItem(gserver.m_itemProtoList.m_sTicketItem2->m_index, npc, 0, 0, 1, false);
			
			if (ticket)
			{
				CNetMsg dropMsg;
				
				if (tpc)
					ticket->m_preferenceIndex = tpc->m_index;
				else
					ticket->m_preferenceIndex = -1;
				ItemDropMsg(dropMsg, npc, ticket);
				npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(ticket), ticket->m_cellX, ticket->m_cellZ);
			}
		}
	}
}


#ifdef EVENT_MOONSTONE_DROP
void DropMoonStoneItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (gserver.m_bMoonStoneEvent)
	{
		if (level != -1 && level - npc->m_level <= 5)
		{
			// 잠수함 : 문스톤 드롭
			int prob = 90;
			
			if (npc->m_pZone->IsPersonalDungeon())
				prob /= 2;
			
			// 문스톤 드롭
			if (GetRandom(1, 10000) <= prob)
			{
				CItem* item = npc->m_pArea->DropItem(723, npc, 0, 0, 1);
				if (item)
				{
					CNetMsg dropMsg;
					
					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;
					ItemDropMsg(dropMsg, npc, item);
					npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
				}
			}
		}
	}
}
#endif

#ifdef EVENT_NEW_MOONSTONE
void DropNewMoonStoneItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (level != -1 && level - npc->m_level <= 5)
	{
		// 잠수함 : 문스톤 드롭
		int prob = 90;
		
		if (npc->m_pZone->IsPersonalDungeon())
			prob /= 2;
		
#ifdef TEST_SERVER
		prob = 10000;
#endif // TEST_SERVER
		// 문스톤상자 드롭
		if (GetRandom(1, 10000) <= prob)
		{
			CItem* item = npc->m_pArea->DropItem(2544, npc, 0, 0, 1);
			if (item)
			{
				CNetMsg dropMsg;
				
				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
				ItemDropMsg(dropMsg, npc, item);
				npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			}
		}
	}
}
#endif // EVENT_NEW_MOONSTONE

#ifdef EVENT_LETTER
////////////////////
// Function name	: DropLetterItem
// Description	    : 낱말 맞추기 이벤트
//646,이벤트[라]
//647,이벤트[스]
//648,이벤트[트]
//649,이벤트[카]
//650,이벤트[오]
//651,이벤트[나]
//652,이벤트[코]
//653,이벤트[이]
//654,이벤트[메]
//655,이벤트[크]
//656,이벤트[리]
//657,이벤트[미]
//658,이벤트[안]
//659,이벤트[아]
//660,이벤트[완]
//661,이벤트[드]
void DropLetterItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	static const int item1count = 3;
	static const int item2count = 13;
	static int item1[item1count] = {647, 648, 651};
	static int item2[item2count] = {646, 649, 650, 652, 653, 654, 655, 656, 657, 658, 659, 660, 661};

	if (gserver.m_bLetterEvent)
	{
		if (level != -1 && level - npc->m_level <= 5)
		{
#if defined(WIN32) && defined(_DEBUG) && defined (_CONSOLE)
			int prob1 = 10000;		// 굴자 드롭 확률
			int prob2 = 5000;		// 스, 트, 나 드롭 확률
#else
			int prob1 = 300;		// 굴자 드롭 확률
			int prob2 = 10;			// 스, 트, 나 드롭 확률
#endif

			if (npc->m_pZone->IsPersonalDungeon())
			{
				prob1 /= 2;
				prob2 /= 2;
			}

			int prob = GetRandom(1, 10000);

			if (prob <= prob1)
			{
				CItem* item = NULL;
				if (prob <= prob2)
				{
					// 스, 트, 나
					item = npc->m_pArea->DropItem(item1[GetRandom(0, item1count - 1)], npc, 0, 0, 1);
				}
				else
				{
					// 라, 카, 오, 코, 이, 메, 크, 리, 미, 안, 아, 완, 드
					item = npc->m_pArea->DropItem(item2[GetRandom(0, item2count - 1)], npc, 0, 0, 1);
				}

				if (item)
				{
					CNetMsg rmsg;

					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;
					ItemDropMsg(rmsg, npc, item);
					npc->m_pArea->SendToCell(rmsg, npc, false);
				}
			}
		}
	}
}
#endif

#ifdef EVENT_LETTER_CHANGE
int GetLetterBit(int dbindex)
{
	switch (dbindex)
	{
	case 646:	return (1 <<  0);	// 라
	case 647:	return (1 <<  1);	// 스
	case 648:	return (1 <<  2);	// 트
	case 649:	return (1 <<  3);	// 카
	case 650:	return (1 <<  4);	// 오
	case 651:	return (1 <<  5);	// 나
	case 652:	return (1 <<  6);	// 코
	case 653:	return (1 <<  7);	// 이
	case 654:	return (1 <<  8);	// 메
	case 655:	return (1 <<  9);	// 크
	case 656:	return (1 << 10);	// 리
	case 657:	return (1 << 11);	// 미
	case 658:	return (1 << 12);	// 안
	case 659:	return (1 << 13);	// 아
	case 660:	return (1 << 14);	// 완
	case 661:	return (1 << 15);	// 드
	}
	return 0;
}

#define LETTER_LASTCHAOS	(1 <<  0) | (1 <<  1) | (1 <<  2) | (1 <<  3) | (1 <<  4) | (1 <<  1)
#define LETTER_NAKO			(1 <<  5) | (1 <<  6)
#define LETTER_KNIGHT		(1 <<  5) | (1 <<  7) | (1 <<  2)
#define LETTER_MERAC		(1 <<  8) | (1 <<  0) | (1 <<  9)
#define LETTER_CRIS			(1 <<  9) | (1 << 10) | (1 <<  1)
#define LETTER_LAMIAN		(1 <<  0) | (1 << 11) | (1 << 12)
#define LETTER_ARCHWAND		(1 << 13) | (1 <<  9) | (1 << 14) | (1 << 15)

MSG_EVENT_LETTER_TYPE CheckLetterBit(int bitfield)
{
	switch (bitfield)
	{
	case LETTER_LASTCHAOS:
		return MSG_EVENT_LETTER_LASTCHAOS;
	case LETTER_NAKO:
		return MSG_EVENT_LETTER_NAKO;
	case LETTER_KNIGHT:
		return MSG_EVENT_LETTER_KNIGHT;
	case LETTER_MERAC:
		return MSG_EVENT_LETTER_MERAC;
	case LETTER_CRIS:
		return MSG_EVENT_LETTER_CRIS;
	case LETTER_LAMIAN:
		return MSG_EVENT_LETTER_LAMIAN;
	case LETTER_ARCHWAND:
		return MSG_EVENT_LETTER_ARCHWAND;
	default:
		return MSG_EVENT_LETTER_NOMATCH;
	}
}

#endif

#ifdef EVENT_SAKURA
void DropSakuraItem(int level, CNPC* npc, CPC* pc, CPC* tpc)
{
	struct tm tm_now = NOW();

	if(pc->m_etcEvent & EVENT_SAKURA_DROP && (tm_now.tm_min%10) == 9 )
	{
		if (level != -1 && level <= npc->m_level )
		{
			int prob = 5000;
			
			if (pc->m_pZone->IsPersonalDungeon())
				prob /= 2;

			if (GetRandom(1, 10000) <= prob)
			{

				int ran = GetRandom(1, 10000);
				
				int itemIndex = 1308;

				if( ran > 2000 )
					itemIndex++;
				if( ran > 7000 )
					itemIndex++;
				if( ran > 8000 )
					itemIndex++;

				CItem* item = npc->m_pArea->DropItem(itemIndex, npc, 0, 0, 1);
				if (item)
				{
					CNetMsg dropMsg;
					ItemDropMsg(dropMsg, npc, item);
					npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
					
					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;

					GAMELOG << init("EVENT_SAKURA_DROP")
							<< "NPC INDEX" << delim
							<< npc->m_proto->m_index << delim
							<< "NPC NAME" << delim
							<< npc->m_name << delim
							<< "ITEM" << delim
							<< itemlog(item)
							<< end;
				}
			}
		}
		pc->m_etcEvent = pc->m_etcEvent &~ EVENT_SAKURA_DROP;
	}
	else
	{
		if( (tm_now.tm_min%10) != 9)
			pc->m_etcEvent = pc->m_etcEvent | EVENT_SAKURA_DROP;
	}

	if(pc->m_etcEvent & EVENT_SAKURABUD_DROP && (tm_now.tm_min%10) == 0 )
	{
		if (level != -1 && level <= npc->m_level )
		{
			int prob = 5000;
			
			if (pc->m_pZone->IsPersonalDungeon())
				prob /= 2;

			if (GetRandom(1, 10000) <= prob)
			{

				CItem* item = npc->m_pArea->DropItem(1312, npc, 0, 0, 1);
				if (item)
				{
					CNetMsg dropMsg;
					ItemDropMsg(dropMsg, npc, item);
					npc->m_pArea->SendToCell(dropMsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
					
					if (tpc)
						item->m_preferenceIndex = tpc->m_index;
					else
						item->m_preferenceIndex = -1;

					GAMELOG << init("EVENT_SAKURABUD_DROP")
							<< "NPC INDEX" << delim
							<< npc->m_proto->m_index << delim
							<< "NPC NAME" << delim
							<< npc->m_name << delim
							<< "ITEM" << delim
							<< itemlog(item)
							<< end;
				}
			}
		}
		pc->m_etcEvent = pc->m_etcEvent &~ EVENT_SAKURABUD_DROP;
	}
	else
	{
		if( (tm_now.tm_min%10) != 0)
			pc->m_etcEvent = pc->m_etcEvent | EVENT_SAKURABUD_DROP;
	}
}
#endif

#ifdef MAGIC_MONSTER_DROP_ACC
void DropMagicMonsterAccesary(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
#if defined( LC_KOR )
	static int ITEM_DROP_PROB = 7;
	static int ITEM_DROP_PROB1 = 5;
#else 
	static int ITEM_DROP_PROB = 2;
	static int ITEM_DROP_PROB1 = 2;
#endif
	if (level != -1 && level - npc->m_level <= 5)
	{
		int prob;
		int itemIndex = -1;
		bool bDrop = false;
	
		switch(npc->m_proto->m_index)
		{
			// 켈베로스 드래곤 바알 웨이프필러, 데스나이트, 레전 슬레이어, 베런 아이즈, 블러드 테리언 0.4프로, 기사,남작,백작중 1
		case 215:
		case 238:
		case 152:
		case 250:
		case 190:
		case 264:
			{
				prob = GetRandom(0, 1000);

				if( prob <= ITEM_DROP_PROB )
				{
					itemIndex = 1717 + GetRandom(0, 2);
					bDrop = true;
				}
			}
			break;

			// 베런 아이즈, 후작의 목걸이 0.3프로
		case 266:
			{
				prob = GetRandom(0, 1000);
				if( prob <= ITEM_DROP_PROB1 )
				{
					itemIndex = 1720;
					bDrop = true;
				}
			}
			break;

			// 블러드 테리언 공작의 반지 1721 0.1프로
		case 268:
			{
				prob = GetRandom(0, 1000);

				if( prob <= 1 )
				{
					itemIndex = 1721;
					bDrop = true;
				}
			}
			break;
		default:
			return;
		}

		if( bDrop )
		{
			CItem* item = npc->m_pArea->DropItem(itemIndex, npc, 0, 0, 1);
			if (item)
			{
				switch( item->m_itemProto->m_index )
				{
				case 1717:
					item->m_nOption = 3;

					item->m_option[0].m_level = 3;
					item->m_option[0].m_type = 0;
					item->m_option[0].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[0].m_type);

					if( item->m_option[0].m_proto )
					{
						item->m_option[0].m_value = item->m_option[0].m_proto->m_levelValue[item->m_option[0].m_level - 1];
					}

					item->m_option[1].m_level = 3;
					item->m_option[1].m_type = 6;
					item->m_option[1].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[1].m_type);

					if( item->m_option[1].m_proto )
					{
						item->m_option[1].m_value = item->m_option[1].m_proto->m_levelValue[item->m_option[1].m_level - 1];
					}

					item->m_option[2].m_level = 3;
					item->m_option[2].m_type = 9;
					item->m_option[2].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[2].m_type);

					if( item->m_option[2].m_proto )
					{
						item->m_option[2].m_value = item->m_option[2].m_proto->m_levelValue[item->m_option[2].m_level - 1];
					}
					break;
				case 1718:
					item->m_nOption = 3;

					item->m_option[0].m_level = 3;
					item->m_option[0].m_type = 1;
					item->m_option[0].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[0].m_type);

					if( item->m_option[0].m_proto )
					{
						item->m_option[0].m_value = item->m_option[0].m_proto->m_levelValue[item->m_option[0].m_level - 1];
					}

					item->m_option[1].m_level = 3;
					item->m_option[1].m_type = 8;
					item->m_option[1].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[1].m_type);

					if( item->m_option[1].m_proto )
					{
						item->m_option[1].m_value = item->m_option[1].m_proto->m_levelValue[item->m_option[1].m_level - 1];
					}

					item->m_option[2].m_level = 3;
					item->m_option[2].m_type = 10;
					item->m_option[2].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[2].m_type);

					if( item->m_option[2].m_proto )
					{
						item->m_option[2].m_value = item->m_option[2].m_proto->m_levelValue[item->m_option[2].m_level - 1];
					}
					break;
				case 1719:
					item->m_nOption = 3;

					item->m_option[0].m_level = 3;
					item->m_option[0].m_type = 2;
					item->m_option[0].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[0].m_type);

					if( item->m_option[0].m_proto )
					{
						item->m_option[0].m_value = item->m_option[0].m_proto->m_levelValue[item->m_option[0].m_level - 1];
					}

					item->m_option[1].m_level = 3;
					item->m_option[1].m_type = 16;
					item->m_option[1].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[1].m_type);

					if( item->m_option[1].m_proto )
					{
						item->m_option[1].m_value = item->m_option[1].m_proto->m_levelValue[item->m_option[1].m_level - 1];
					}

					item->m_option[2].m_level = 3;
					item->m_option[2].m_type = 17;
					item->m_option[2].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[2].m_type);

					if( item->m_option[2].m_proto )
					{
						item->m_option[2].m_value = item->m_option[2].m_proto->m_levelValue[item->m_option[2].m_level - 1];
					}
					break;
				case 1720:
					item->m_nOption = 4;

					item->m_option[0].m_level = 3;
					item->m_option[0].m_type = 3;
					item->m_option[0].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[0].m_type);

					if( item->m_option[0].m_proto )
					{
						item->m_option[0].m_value = item->m_option[0].m_proto->m_levelValue[item->m_option[0].m_level - 1];
					}

					item->m_option[1].m_level = 3;
					item->m_option[1].m_type = 4;
					item->m_option[1].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[1].m_type);

					if( item->m_option[1].m_proto )
					{
						item->m_option[1].m_value = item->m_option[1].m_proto->m_levelValue[item->m_option[1].m_level - 1];
					}

					item->m_option[2].m_level = 3;
					item->m_option[2].m_type = 22;
					item->m_option[2].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[2].m_type);

					if( item->m_option[2].m_proto )
					{
						item->m_option[2].m_value = item->m_option[2].m_proto->m_levelValue[item->m_option[2].m_level - 1];
					}

					item->m_option[3].m_level = 3;
					item->m_option[3].m_type = 23;
					item->m_option[3].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[3].m_type);

					if( item->m_option[3].m_proto )
					{
						item->m_option[3].m_value = item->m_option[3].m_proto->m_levelValue[item->m_option[3].m_level - 1];
					}
					break;
				case 1721:
					item->m_nOption = 5;

					item->m_option[0].m_level = 3;
					item->m_option[0].m_type = 4;
					item->m_option[0].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[0].m_type);

					if( item->m_option[0].m_proto )
					{
						item->m_option[0].m_value = item->m_option[0].m_proto->m_levelValue[item->m_option[0].m_level - 1];
					}

					item->m_option[1].m_level = 3;
					item->m_option[1].m_type = 5;
					item->m_option[1].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[1].m_type);

					if( item->m_option[1].m_proto )
					{
						item->m_option[1].m_value = item->m_option[1].m_proto->m_levelValue[item->m_option[1].m_level - 1];
					}

					item->m_option[2].m_level = 3;
					item->m_option[2].m_type = 20;
					item->m_option[2].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[2].m_type);

					if( item->m_option[2].m_proto )
					{
						item->m_option[2].m_value = item->m_option[2].m_proto->m_levelValue[item->m_option[2].m_level - 1];
					}

					item->m_option[3].m_level = 3;
					item->m_option[3].m_type = 21;
					item->m_option[3].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[3].m_type);

					if( item->m_option[3].m_proto )
					{
						item->m_option[3].m_value = item->m_option[3].m_proto->m_levelValue[item->m_option[3].m_level - 1];
					}

					item->m_option[4].m_level = 3;
					item->m_option[4].m_type = 22;
					item->m_option[4].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[4].m_type);

					if( item->m_option[4].m_proto )
					{
						item->m_option[4].m_value = item->m_option[4].m_proto->m_levelValue[item->m_option[4].m_level - 1];
					}
					break;
				default:
					break;
				}

				CNetMsg rmsg;
				ItemDropMsg(rmsg, npc, item);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				if (tpc)
					item->m_preferenceIndex = tpc->m_index;
				else
					item->m_preferenceIndex = -1;
			}
		}
	}
}

#endif

#ifdef BLESS_WARRIOR
void RegenBlessWarrior(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	static const int		nProbWarrior		= 10;					// 전사의 손길 확률 : 10%
	static const int		nProbGuard			= 20 + nProbWarrior;	// 수호의 손길 확률 : 20%
	static const int		nProbBrave			= 35 + nProbGuard;		// 용맹의 손길 확률 : 35%
	static const int		nProbProtect		= 35 + nProbBrave;		// 보호의 손길 확률 : 35%

	static const int		nIndexWarrior		= 310;					// 전사의 손길
	static const int		nIndexGuard			= 311;					// 수호의 손길
	static const int		nIndexBrave			= 312;					// 용맹의 손길
	static const int		nIndexProtect		= 313;					// 보호의 손길

	if (pc == NULL && tpc == NULL)
		return ;
	if (tpc == NULL)
		tpc = pc;
	else if (tpc->m_level > level)
		level = tpc->m_level;

	if (tpc->m_pZone->IsPersonalDungeon())
		return ;

	int nProb = 6;		// 기본 확률 : 6/1000 : 0.6%

#ifdef LC_JPN
	nProb = 3;			// 일본 0.3%
#endif // LC_JPN	

#ifdef LC_RUS
	nProb = 30;
#endif // LC_RUS

#ifdef DOUBLE_BLESS_WARRIOR		// 풍성한 전사의 축복
	nProb = 2 * nProb;
#endif // DOUBLE_BLESS_WARRIOR

	if (npc->m_level < level)
		nProb -= level - npc->m_level;

	if (GetRandom(1, 1000) <= nProb)
	{
		nProb = GetRandom(1, 100);

		int nNPCIndex = 0;

		if (nProb <= nProbWarrior)			nNPCIndex = nIndexWarrior;
		else if (nProb <= nProbGuard)		nNPCIndex = nIndexGuard;
		else if (nProb <= nProbBrave)		nNPCIndex = nIndexBrave;
		else if (nProb <= nProbProtect)		nNPCIndex = nIndexProtect;

		if (nNPCIndex != 0)
		{
			CNPC*		pNPCBless = gserver.m_npcProtoList.Create(nNPCIndex, NULL);
			if (pNPCBless)
			{
				GET_X(pNPCBless)			= GET_X(npc);
				GET_Z(pNPCBless)			= GET_Z(npc);
				GET_R(pNPCBless)			= GET_R(npc);
				GET_YLAYER(pNPCBless)		= GET_YLAYER(npc);

				pNPCBless->m_regenX			= GET_X(pNPCBless);
				pNPCBless->m_regenZ			= GET_Z(pNPCBless);
				pNPCBless->m_regenY			= GET_YLAYER(pNPCBless);

				pNPCBless->m_recoverPulse	= gserver.m_pulse;
#ifndef NEW_DIVISION_EXPSP
				pNPCBless->m_totalDamage	= 0;
#endif // #ifndef NEW_DIVISION_EXPSP

				pNPCBless->InitPointsToMax();
				pNPCBless->m_disableTime	= 0;

				npc->m_pArea->AddNPC(pNPCBless);

				int		nCellX;
				int		nCellZ;
				npc->m_pArea->PointToCellNum(GET_X(pNPCBless), GET_Z(pNPCBless), &nCellX, &nCellZ);
				npc->m_pArea->CharToCell(pNPCBless, GET_YLAYER(pNPCBless), nCellX, nCellZ);

				CNetMsg rmsg;
				AppearMsg(rmsg, pNPCBless, true);
				npc->m_pArea->SendToCell(rmsg, GET_YLAYER(pNPCBless), nCellX, nCellZ);

				pNPCBless->m_aipulse		= gserver.m_pulse;
				pNPCBless->m_delay			= PULSE_REAL_SEC * 2;
				pNPCBless->m_nBlockRegen	= tpc->m_index;
			}
		}
	}
}

void DropBlessWarrior(CNPC* pNPC)
{
	CNetMsg		rmsg;
	int			i;

	for (i = 0; i < MAX_NPC_DROPITEM; i++)
	{
		if (pNPC->m_proto->m_item[i] > 0)
		{
			int			nItemIndex			= pNPC->m_proto->m_item[i];
			CItem*		pItem				= NULL;

			switch (nItemIndex)
			{
			case 84:		// 일반 제련석은 무시
				break;

			default:
				pItem = pNPC->m_pArea->DropItem(nItemIndex, pNPC, 0, 0, 1, true);
				break;
			}

			if (pItem)
			{
				pItem->m_preferenceIndex = pNPC->m_nBlockRegen;
				ItemDropMsg(rmsg, pNPC, pItem);
				pNPC->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

				GAMELOG << init("BLESS OF WARRIOR DROP")
						<< "PC INDEX" << delim
						<< pNPC->m_nBlockRegen << delim
						<< "NPC" << delim
						<< pNPC->m_idNum << delim
						<< pNPC->m_name << delim
						<< "ITEM" << delim
						<< itemlog(pItem)
						<< end;
			}
		}
	}

	pNPC->SendDisappearAllInCell(true);

	// 어택리스트에서 삭제
	DelAttackList(pNPC);

	// npc 삭제
	pNPC->m_pArea->CharFromCell(pNPC, true);
	pNPC->m_pArea->DelNPC(pNPC);
}
#endif // BLESS_WARRIOR

#ifdef	EVENT_HALLOWEEN_2006
 void DropHalloween2006Item( CNPC* npc, CPC* pc, CPC* tpc, int level )
 {
	 int		nProb = 0;

	 if ( !tpc )
		 return;
	 
	 if( tpc->m_pZone->IsPersonalDungeon() )
		 return;


	 if( npc->m_proto->m_index != 314 )
		 return;

	 //nProb = gsever.m_HalloweenEventPercent;
	nProb = GetRandom( 1, 1000 );
 	int nItemProb = GetRandom( 1, 1000 );

	CNetMsg rmsg;

#if defined( LC_KOR ) || defined( BWTEST )
	static const int GIVE_ITEM_COUNT = 3;
	static const int ItemTable[GIVE_ITEM_COUNT][3] = {
			{85, 1, 10}, // itemIndex, count, prob
			{1056, 5, 505},
			{1057, 5, 1000 },
		};
#elif defined ( LC_BRZ )
	static const int GIVE_ITEM_COUNT = 14;
	static const int ItemTable[GIVE_ITEM_COUNT][3] = {
			{723, 3, 100},		// 문스톤, flag, count, 확률
			{85, 1, 30},		// 고급제련석
			{498, 5, 50},		// 붉은 케르
			{499, 5, 100},		// 붉은 넨
			{500, 5, 50},		// 푸른 케르
			{501, 5, 100},		// 푸른 넨
			{19,  10000, 70},	// 나스
			{19,  20000, 20},	// 나스
			{19,  50000, 10},	// 나스
			{43,  10, 150},		// 소형 체력 회복 물약
			{44,  10, 100},		// 중형 체력 회복 물약
			{45,  10, 70},		// 대형 체력 회복 물약
			{786, 3, 50 },		// 부스터
			{284, 5, 100 },		// 블러드 잼
		};

#else
	static const int GIVE_ITEM_COUNT = 19;
	static const int ItemTable[GIVE_ITEM_COUNT][3] = {
			{723, 5, 100},		// 문스톤,  flag, count, 확률
			{85, 1, 155},		// 고급제련석  
			{498,  5, 205},		// 붉은 케르
			{499,  5, 305},		// 붉은 넨
			{500,  5, 355},		// 푸른 케르
			{501,  5, 455},		// 푸른 넨
			{19,  10000, 525},	// 나스
			{19,  20000, 595},	// 나스
			{19,  50000, 695},	// 나스
			{43,  10, 795},		// 소형 체력 회복 물약
			{44,  10, 895},		// 중형 체력 회복 물약
			{45,  10, 965},		// 대형 체력 회복 물약
			{1334,  1, 970},		// 타이트론 액스
			{1331,  1, 975},		// 젠티언 소드
			{1338,  1, 980},		// 소울메인 완드
			{1335,  1, 985},		// 에버럭키 보우
			{1336,  1, 990},		// 자이널 완드
			{1340,  1, 995},		// 건로즈 보우
			{1342,  1, 1000},	// 풋스테일 폴암
		};
#endif
	 
	 if( nProb <= 300 )
	 {
		for( int i = 0; i < GIVE_ITEM_COUNT; i++ )
		{
			if( nItemProb <= ItemTable[i][2] )
			{
				CItem* pItem = gserver.m_itemProtoList.CreateItem(ItemTable[i][0], -1, 0, 0, ItemTable[i][1]);
				if( !pItem )
				{
					return;
				}
				if( (pItem->m_idNum == 498) || (pItem->m_idNum == 499) || (pItem->m_idNum == 500) || (pItem->m_idNum == 501) )
				{
					pItem->m_flag = 12;
				}
				CNetMsg rmsg;
				pItem->m_preferenceIndex = tpc->m_index;
				npc->m_pArea->DropItem(pItem, npc);
				ItemDropMsg(rmsg, npc, pItem);
				pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);



				// 7. 게임 로그
				GAMELOG << init("EVENT HALLOWEEN DROP", tpc)
						<< "NPC" << delim
						<< npc->m_idNum << delim
						<< npc->m_name << delim
						<< "NPC LEVEL" << delim
						<< npc->m_level << delim
						<< "PC LEVEL" << delim
						<< tpc->m_level << delim
						<< "ITEM" << delim
						<< itemlog(pItem)
						<< end;
				return ;
			}
			else
			{
				nProb -= ItemTable[i][1];
			}			
		}
	 }
	 else if( nProb > 300 && nProb <= 600 )
	 {
		 CNPC* TempNPC = NULL;
		 if( tpc->m_pZone->m_index == ZONE_START ) 
		 {
			TempNPC = gserver.m_npcProtoList.Create( 315, NULL );
		 }
		 else if( tpc->m_pZone->m_index == ZONE_DRATAN )
		 {
			 TempNPC = gserver.m_npcProtoList.Create( 317, NULL );
		 }
		 else if( tpc->m_pZone->m_index == ZONE_MERAC )
		 {
			 TempNPC = gserver.m_npcProtoList.Create( 319, NULL );
		 }


		if( !npc )
			return;
		int nTry = 0;
		do
		{
			GET_X(TempNPC) = GET_X(npc);
			GET_Z(TempNPC) = GET_Z(npc);
			GET_YLAYER(TempNPC) = GET_YLAYER(npc);
			GET_R(TempNPC) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			if (npc->m_pArea->GetAttr(GET_YLAYER(TempNPC), GET_X(TempNPC), GET_Z(TempNPC)) == MAPATT_FIELD)
				break;
			nTry++;
		}while( nTry <= 10 );

		if (nTry > 10)
		{
			GET_X(TempNPC) = GET_X(npc);
			GET_Z(TempNPC) = GET_Z(npc);
			GET_YLAYER(TempNPC) = GET_YLAYER(npc);
			GET_R(TempNPC) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		}
		
		TempNPC->m_regenX = GET_X(TempNPC);
		TempNPC->m_regenY = GET_YLAYER(TempNPC);
		TempNPC->m_regenZ = GET_Z(TempNPC);

		int cx, cz;
		npc->m_pArea->AddNPC(TempNPC);
		npc->m_pArea->PointToCellNum(GET_X(TempNPC), GET_Z(TempNPC), &cx, &cz);
		npc->m_pArea->CharToCell(TempNPC, GET_YLAYER(TempNPC), cx, cz);
		
		CNetMsg appearNPCMsg;
		AppearMsg(appearNPCMsg, TempNPC, true);
		npc->m_pArea->SendToCell(appearNPCMsg, GET_YLAYER(TempNPC), cx, cz);


	 }
	 else 
	 {
		 CNPC* TempNPC = NULL;
		 if( tpc->m_pZone->m_index == ZONE_START ) 
		 {
			TempNPC = gserver.m_npcProtoList.Create( 316, NULL );
		 }
		 else if( tpc->m_pZone->m_index == ZONE_DRATAN )
		 {
			TempNPC = gserver.m_npcProtoList.Create( 318, NULL );
		 }
		 else if( tpc->m_pZone->m_index == ZONE_MERAC )
		 {
			TempNPC = gserver.m_npcProtoList.Create( 320, NULL );
		 }
		if( !npc )
			return;
		int nTry = 0;
		do
		{
			GET_X(TempNPC) = GET_X(npc);
			GET_Z(TempNPC) = GET_Z(npc);
			GET_YLAYER(TempNPC) = GET_YLAYER(npc);
			GET_R(TempNPC) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			if (npc->m_pArea->GetAttr(GET_YLAYER(TempNPC), GET_X(TempNPC), GET_Z(TempNPC)) == MAPATT_FIELD)
				break;
			nTry++;
		}while( nTry <= 10 );

		if (nTry > 10)
		{
			GET_X(TempNPC) = GET_X(npc);
			GET_Z(TempNPC) = GET_Z(npc);
			GET_YLAYER(TempNPC) = GET_YLAYER(npc);
			GET_R(TempNPC) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		}
		
		TempNPC->m_regenX = GET_X(TempNPC);
		TempNPC->m_regenY = GET_YLAYER(TempNPC);
		TempNPC->m_regenZ = GET_Z(TempNPC);

		int cx, cz;
		npc->m_pArea->AddNPC(TempNPC);
		npc->m_pArea->PointToCellNum(GET_X(TempNPC), GET_Z(TempNPC), &cx, &cz);
		npc->m_pArea->CharToCell(TempNPC, GET_YLAYER(TempNPC), cx, cz);
		
		CNetMsg appearNPCMsg;
		AppearMsg(appearNPCMsg, TempNPC, true);
		npc->m_pArea->SendToCell(appearNPCMsg, GET_YLAYER(TempNPC), cx, cz);
	 }		 

 }
#endif	// EVENT_HALLOWEEN_2006

#ifdef EVENT_VALENTINE_2007
 void DropEventValentine2007(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc)
		return ;

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
		return;

#if defined (WJKTEST) || defined (KYD_TEST)
	int nProbCacao[3] = { 3000, 3000, 3000 };					// 카카오 30%, 60%, 90% 확률	       : 1%, 0.5, 0.1%
//#elif defined(LC_GER)
//	int nProbCacao[3] = { 500, 200, 100 };				    	// 카카오 30%, 60%, 90% 확률	       : 5%, 2%, 1%
#else
	int nProbCacao[3] = { 100, 50, 10 };						// 카카오 30%, 60%, 90% 확률	       : 1%, 0.5, 0.1%
#endif // WJKTEST

#if defined ( EVENT_VALENTINE_2008_ITEM )
	static const int nIndexItem[3] = { 2629, 2630, 2631 };		// 파랑, 노랑, 빨강 복주머니
#else
	static const int nIndexItem[3] = { 2043, 2044, 2045 };		// 카카오 30%, 60%, 90% 아이템 인덱스
#endif 

	if( npc->m_level < tpc->m_level )			// 1레벨당 0.2% 드롭확률 감소
	{
		int diffLevel = tpc->m_level - npc->m_level;

		int k;
		for(k = 0; k < 3; ++k)
		{
			nProbCacao[k] -= 20 * diffLevel;
		}
	}

	CNetMsg rmsg;
	int i;
	for (i = 0; i < 3; i++)
	{
		if (GetRandom(1, 10000) <= nProbCacao[i])
		{
			CItem* item = gserver.m_itemProtoList.CreateItem(nIndexItem[i], -1, 0, 0, 1);
			if (item)
			{
				item->m_preferenceIndex = tpc->m_index;
				npc->m_pArea->DropItem(item, npc);
				ItemDropMsg(rmsg, npc, item);
				item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				GAMELOG << init("EVENT VALENTINE 2007 ITEM DROP", tpc)
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
			}
		}
	}
}
#endif	// EVENT_VALENTINE_2007

#ifdef EVENT_TLD_2007_SONGKRAN
void DropEventSongkran2007( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if (!tpc)
		return;

	if(tpc->m_pZone->IsPersonalDungeon() )	// 퍼스널 던전에서는 드롭하지 않는다
		return;

	static const int nIndexItem[2] = { 2146, 2147 };	// 금그릇, 은그릇
	int nProb[2] = { 100, 150 };			// 드롭율 1%, 1.5%

	if( npc->m_level< tpc->m_level )	// 1레벨당 0.15% 드롭 확률 감소
	{
		int diffLevel = tpc->m_level - npc->m_level;

		int k;
		for( k = 0; k < 2; ++k )
		{
			nProb[k] -= 15 * diffLevel;
		}
	}

	CNetMsg rmsg;
	int i;
	for( i = 0; i < 2; i++ )
	{
		if (GetRandom( 1, 10000 ) <= nProb[i] )
		{
			CItem* item = gserver.m_itemProtoList.CreateItem(nIndexItem[i], -1, 0, 0, 1 );
			if( item )
			{
				item->m_preferenceIndex = tpc->m_index;
				npc->m_pArea->DropItem( item, npc );
				ItemDropMsg(rmsg, npc, item);
				item->m_pArea->SendToCell( rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ );

				GAMELOG << init( "EVENT SONGKRAN 2007 ITEM DROP", tpc )
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
				break;  // 중복 드롭 안됨
			} // if( item )
		}//if (GetRandom( 1, 10000 ) <= nProb[i] )
	} // for( i = 0; i < 2; i++ )
}
#endif	// EVENT_TLD_2007_SONGKRAN

#ifdef EVENT_WHITEDAY_2007
void DropEventWhiteday2007(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc)
		return ;

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
		return;
#ifdef EVENT_EGGS_HUNT_2008_ITEM
	static const int nIndexItem[2] = { 2745, 2746 };		// 하늘색 달걀, 분홍색 달걀
#else
	static const int nIndexItem[2] = { 2133, 2134 };		// 하늘색 사탕, 분홍색 사탕
#endif // EVENT_EGG_HUNT_2008_ITEM

#if defined( LC_HBK )
	int nProb[2] = { 300, 300 };		// 홍콩 드롭률 3%
#else
	int nProb[2] = { 100, 100 };		// 1% 
#endif // LC_HBK
	if( npc->m_level < tpc->m_level )			// 1레벨당 0.1% 드롭확률 감소
	{
		int diffLevel = tpc->m_level - npc->m_level;

		int k;
		for(k = 0; k < 2; ++k)
		{
			nProb[k] -= 10 * diffLevel;
		}
	}

	CNetMsg rmsg;
	int i;
	for (i = 0; i < 2; i++)
	{
		if (GetRandom(1, 10000) <= nProb[i] )			// 1%의 확률
		{
			CItem* item = gserver.m_itemProtoList.CreateItem(nIndexItem[i], -1, 0, 0, 1);
			if (item)
			{
				item->m_preferenceIndex = tpc->m_index;
				npc->m_pArea->DropItem(item, npc);
				ItemDropMsg(rmsg, npc, item);
				item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				GAMELOG << init("EVENT WHITEDAY 2007 ITEM DROP", tpc)
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
			}
		}
	}
}
#endif //EVENT_WHITEDAY_2007
 

#ifdef MOB_SCROLL
void DropMobScrollSpecialStone(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc || !npc)
		return ;

	if (npc->m_MobScrollType != -1
		&& npc->m_UseCharIndex != -1)		
	{
		static const int patterns[2][13] =
		{ 
			{1, 297, 0, 9, 133, 132, 264, 125, 126, 158, 68, 69, 250,},	// 2144
			{2, 190, 267, 8, 250, 251, 252, 189, 188, 236, 190, 189, 0,},// 2145
		};

		int pattern_index;
		int npc_index;
		int temp;

		if (npc->m_NextMobIndex == 9)
		{	// 몬스터 소환서로 나온 보스 몬스터 
			if (GetRandom(1, 10000) <= 3000)	// 30% 확률
			{
				CItem* item = gserver.m_itemProtoList.CreateItem(85, -1, 0, 0, 1);	// 고급 제련석
				if (item)
				{
					item->m_preferenceIndex = npc->m_UseCharIndex;
					npc->m_pArea->DropItem(item, npc);

					CNetMsg rmsg;
					ItemDropMsg(rmsg, npc, item);
					item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

					GAMELOG << init("MOB SCROLL SPECAIL STONE ITEM DROP", tpc)
							<< "ITEM" << delim
							<< itemlog(item)
							<< end;
				}
			}

			npc->m_MobScrollType = -1;
			npc->m_NextMobIndex = -1;
			npc->m_UseCharIndex = -1;
			
			return;
		}
		else if (npc->m_NextMobIndex == 8)
		{
			pattern_index = npc->m_MobScrollType;
			temp = GetRandom(1, patterns[pattern_index][0]);
			npc_index = patterns[pattern_index][temp];
		}
		else
		{		
			pattern_index = npc->m_MobScrollType;
			temp = GetRandom(4, 4 + patterns[pattern_index][3] - 1);
			npc_index = patterns[pattern_index][temp];			
		}		
		
		CNPCProto * proto = gserver.m_npcProtoList.FindProto(npc_index);
		if (!proto)
		{
			return;
		}

		CNPC* npc_new = gserver.m_npcProtoList.Create(npc_index, NULL);
		if (!npc_new)
			return;

		npc_new->m_NextMobIndex = npc->m_NextMobIndex + 1;
		npc_new->m_UseCharIndex = npc->m_UseCharIndex;
		npc_new->m_MobScrollType = npc->m_MobScrollType;

		int nTry = 0;
		do
		{
			GET_X(npc_new) = GET_X(tpc) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
			GET_YLAYER(npc_new) = GET_YLAYER(tpc);
			GET_Z(npc_new) = GET_Z(tpc) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
			GET_R(npc_new) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			if (npc->m_pArea->GetAttr(GET_YLAYER(npc_new), GET_X(npc_new), GET_Z(npc_new)) == MAPATT_FIELD)
				break;
			nTry++;
		} while (nTry <= 10);
		if (nTry > 10)
		{
			GET_X(npc_new) = GET_X(npc);
			GET_Z(npc_new) = GET_Z(npc);
			GET_YLAYER(npc_new) = GET_YLAYER(npc);
			GET_R(npc_new) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		}

		npc_new->m_regenX = GET_X(npc_new);
		npc_new->m_regenY = GET_YLAYER(npc_new);
		npc_new->m_regenZ = GET_Z(npc_new);

		int cx, cz;
		tpc->m_pArea->AddNPC(npc_new);
		tpc->m_pArea->PointToCellNum(GET_X(npc_new), GET_Z(npc_new), &cx, &cz);
		tpc->m_pArea->CharToCell(npc_new, GET_YLAYER(npc_new), cx, cz);
		
		CNetMsg appearNPCMsg;
		AppearMsg(appearNPCMsg, npc_new, true);
		tpc->m_pArea->SendToCell(appearNPCMsg, GET_YLAYER(npc_new), cx, cz);

		npc_new->m_postregendelay += 2 * PULSE_REAL_SEC;
	}
}
#endif	// MOB_SCROLL

#ifdef EVENT_EGGS_HUNT_2007
void DropEventEggsHunt2007(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc)
		return ;

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
		return ;

	if( (tpc->m_level - npc->m_level) >= 6 )			// 자기보다 6Lv 이상 낮은 몹을 잡았을 경우
		return ;

#if defined (WJKTEST)
	const int nProbEggsHunt = 1000;
#elif defined (LC_GER) || defined (LC_EUR)
	const int nProbEggsHunt = 5;				// 드롭확률 0.5%
#elif defined (LC_USA)
	const int nProbEggsHunt = 15;				// 드롭확률 1.5%
#else
	const int nProbEggsHunt = 3;				// 드롭확률 0.3%
#endif

	if(GetRandom(1, 1000) <= nProbEggsHunt)
	{
		CItem* pItem = gserver.m_itemProtoList.CreateItem(2148, -1, 0, 0, 1);
		if(pItem)
		{
			CNetMsg rmsg;
			pItem->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(pItem, npc);
			ItemDropMsg(rmsg, npc, pItem);
			pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

			GAMELOG << init("EVENT EGGS HUNT ITEM DROP", tpc)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< end;
		}
	}
}
#endif // EVENT_EGGS_HUNT_2007

#ifdef EVENT_EASTER_DAY_BRZ
void DropEventEasterDay(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc)
		return ;

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
		return ;

	if((tpc->m_level - npc->m_level) >= 6 )			// 자기보다 6Lv 이상 낮은 몹을 잡았을 경우
		return ;

	static const int nItemDropCount = 5;
	static const int nProb = 8;						// 0.8%
	static const int ItemDropIndex[nItemDropCount] = {512, 556, 1226, 1227, 1307};

	int i;
	for(i = 0; i < nItemDropCount; ++i)
	{
		if(GetRandom(1, 1000) <= nProb)
		{
			CItem* pItem = gserver.m_itemProtoList.CreateItem(ItemDropIndex[i], -1, 0, 0, 1);
			if(pItem)
			{
				CNetMsg rmsg;
				pItem->m_preferenceIndex = tpc->m_index;
				npc->m_pArea->DropItem(pItem, npc);
				ItemDropMsg(rmsg, npc, pItem);
				pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

				GAMELOG << init("EVENT EASTER DAY ITEM DROP", tpc)
						<< "ITEM" << delim
						<< itemlog(pItem)
						<< end;
				break;
			}
		}
	}
}
#endif // EVENT_EASTER_DAY_BRZ

#ifdef EVENT_2007_PARENTSDAY
void DropEventParentsDay2007( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	// 1. tpc 검사
	// 2. 퍼던 제외
	// 3. 확률 계산 : 1/1000, 1레벨당 1감소 , 5레벨 이상일경우 드롭 안됨
	// 4. 확률 적용	
	// 5. 아이템 드롭
	// 6. 게임 로그

	// 1. tpc 검사
	if( !tpc )
		return;

	// 2. 퍼던 제외
	if( tpc->m_pZone == NULL || tpc->m_pZone->IsPersonalDungeon() )		// 퍼스널 던전에서는 드롭하지 않는다
		return;

	if( level == -1 )
		return;

	if( level - npc->m_level > 5 )
		return;
	
	
	// 드롭 확률 계산
	int nProb = 0;

	if( level > 0 && level < 21 )
		nProb = 50;
	else if ( level > 20 && level < 31 )
		nProb = 70;
	else if ( level > 30 && level < 51 )
		nProb = 100;
	else if ( level > 50 && level < 61 )
		nProb = 150;
	else if ( level > 60 && level < 81 )
		nProb = 200;
	else if( level > 80 )
		nProb = 250;

	if( tpc->m_level > npc->m_level )
		nProb -= tpc->m_level - npc->m_level;

	//4. 확률 적용
	if ( GetRandom( 1, 1000 ) <= nProb )
	{
		int ItemIndex = 2331;	// TODO: Item Index 결정 되면 변경 - whs25 ->070416
		// 5.아이템 드롭
		CItem * pItem = gserver.m_itemProtoList.CreateItem( ItemIndex, -1, 0, 0, 1 );
		if( !pItem )
			return;
		CNetMsg rmsg;
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem( pItem, npc );
		ItemDropMsg( rmsg, npc, pItem );
		pItem->m_pArea->SendToCell( rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ );

		GAMELOG << init( "EVENT PARENTS DAY 2007", tpc )
			<<	"NPC" << delim
			<< npc->m_idNum << delim
			<< npc->m_name << delim
			<< "NPC LEVEL" << delim
			<< npc->m_level << delim
			<< "PC LEVEL" << delim
			<< tpc->m_level << delim
			<< "ITEM" << delim
			<< itemlog( pItem )
			<< end;
		return;
	}	
}
#endif // EVENT_2007_PARENTSDAY

#ifdef EVENT_GOMDORI_2007
void DropEventGomdori2007(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
#ifdef LC_KOR
	return;				//곰돌이 이벤트 드롭 제거
#endif

#if defined (LC_HBK)
	const static int nProbHigh = 20;
	const static int nProbLowPerLevel = 10;
	const static int nProbLimit = 3;
#elif defined (LC_BRZ)
	const static int nProbHigh = 1;
	const static int nProbLowPerLevel = 0;
	const static int nProbLimit = 9;
#else
	const static int nProbHigh			= 80;		// 높은 레벨 잡으면 80%
	const static int nProbLowPerLevel	= 10;		// 낮은 레벨 잡으면 레벨당 10%
	const static int nProbLimit			= 9;		// 9레벨 이하 레벨 잡으면 0
#endif //

	const static int nTableGomdoriItem[9] = {
		2335,
		2336,
		2337,
		2338,
		2339,
		2340,
		2341,
		2342,
		2343
	};

	if (npc->m_pZone->IsPersonalDungeon())
		return ;

	// 우선순위 : tpc, level, pc
	if (tpc)
	{
		level = tpc->m_level;
	}
	else
		return ;

	// 드롭 결정
	// 1. 시간 검사 : 매시 x9분 또는 x4분
	// 2. 레벨에 따른 변경 : 높은 몹은 80%, 낮은 몹은 레벨당 -10%, 9레벨 이하 드롭 안함
#if defined (LC_BRZ)
	int nDropProb = 0;
	struct tm tmNow = NOW();
	switch (tmNow.tm_min % 10)
	{
	case 4:
	case 9:
		if(tpc->m_bEventGomdori2007Drop < gserver.m_pulse)
			nDropProb = nProbHigh;
		break;
	}
#else
	int nDropProb = 0;
	struct tm tmNow = NOW();
	switch (tmNow.tm_min % 10)
	{
	case 4:
	case 9:
		if (tpc && tpc->m_bEventGomdori2007Drop)
			nDropProb = 100;
		break;
	default:
		tpc->m_bEventGomdori2007Drop = true;
		break;
	}
#endif // LC_BRZ

	if (nDropProb == 0)
		return ;
	if (level < npc->m_level)
	{
		nDropProb = nProbHigh;
	}
	else if (level > npc->m_level)
	{
		if (level - npc->m_level >= nProbLimit)
			nDropProb = 0;
		else
			nDropProb -= (level - npc->m_level) * nProbLowPerLevel;
	}
	else if (level == npc->m_level)
	{
		nDropProb = nProbHigh;
#ifdef LC_HBK
		nDropProb = 5;
#endif // LC_HBK		
	}

	if (nDropProb >= GetRandom(1, 100))
	{
#ifdef LC_BRZ
		tpc->m_bEventGomdori2007Drop = gserver.m_pulse + PULSE_REAL_HOUR * 2;
#else
		tpc->m_bEventGomdori2007Drop = false;
#endif // LC_BRZ
		int nTableIndex = 0;
		nDropProb = GetRandom(1, 100);

#if defined (LC_BRZ)
		if (nDropProb <= 15)			nTableIndex = 0;
		else if (nDropProb <= 21)		nTableIndex = 1;
		else if (nDropProb <= 36)		nTableIndex = 2;
		else if (nDropProb <= 42)		nTableIndex = 3;
		else if (nDropProb <= 57)		nTableIndex = 4;
		else if (nDropProb <= 63)		nTableIndex = 5;
		else if (nDropProb <= 78)		nTableIndex = 6;
		else if (nDropProb <= 85)		nTableIndex = 7;
		else							nTableIndex = 8;
#else
		if (nDropProb <= 12)			nTableIndex = 0;
		else if (nDropProb <= 23)		nTableIndex = 1;
		else if (nDropProb <= 34)		nTableIndex = 2;
		else if (nDropProb <= 45)		nTableIndex = 3;
		else if (nDropProb <= 56)		nTableIndex = 4;
		else if (nDropProb <= 67)		nTableIndex = 5;
		else if (nDropProb <= 78)		nTableIndex = 6;
		else if (nDropProb <= 89)		nTableIndex = 7;
		else							nTableIndex = 8;
#endif // (LC_BRZ)

		CItem* pItem = npc->m_pArea->DropItem(nTableGomdoriItem[nTableIndex], npc, 0, 0, 1, false);
		if (pItem)
		{
			if (tpc)
				pItem->m_preferenceIndex = tpc->m_index;
			else
				pItem->m_preferenceIndex = -1;

			CNetMsg rmsg;
			ItemDropMsg(rmsg, npc, pItem);
			pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

			GAMELOG << init("EVENT GOMDORI 2007 DROP")
					<< "NPC" << delim
					<< npc->m_idNum << delim
					<< "PC LEVEL" << delim
					<< level << delim
					<< "OWNER" << delim;
			if (tpc)
			{
				GAMELOG << pc->m_index << delim
						<< pc->GetName() << delim;
			}
			else
			{
				GAMELOG << -1 << delim
						<< "NONE" << delim;
			}
			GAMELOG << "ITEM" << delim
					<< itemlog(pItem, true)
					<< end;
		}
	}
}
#endif // EVENT_GOMDORI_2007

#ifdef EVENT_CHILDRENSDAY_2007
void DropEventChildrensDay2007( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if (!tpc)
	{
		return ;
	}

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
	{
		return;
	}

	int nProb = 0;
	int nHighLevel = tpc->m_level;

	if( pc->IsParty() ) // 파티중이면 파티원중 최고 레벨 
	{
		for (int i=0; i < MAX_PARTY_MEMBER; i++)
		{
			CPC* pMemberPC = pc->m_party->GetNearMember(pc, i);
			if (!pMemberPC)
				continue;
		
			if ( pMemberPC->m_level > nHighLevel )
			{
				nHighLevel = pMemberPC->m_level;
			}
		}
	}

	if( nHighLevel > 25 || nHighLevel - npc->m_level > 5 )	// 최고레벨이 25이상이거나 레벨 차이가 5이상이면 drop하지 않는다.  
	{ 
		return;
	}

	if( nHighLevel > 0 && nHighLevel <= 3 )				//1~3Lv:100%
		nProb = 10000;
	else if ( nHighLevel >= 4 && nHighLevel <= 7 )		//4~7Lv:20%
		nProb = 2000;
	else if ( nHighLevel >= 8 && nHighLevel <= 11 )		//8~11Lv:10%
		nProb = 1000;
	else if ( nHighLevel >= 12 && nHighLevel <= 16 )	//12~16Lv:10%
		nProb = 1000;
	else if ( nHighLevel >= 17 && nHighLevel < 25 )		//17~24Lv:10%
		nProb = 1000;

	CNetMsg rmsg;
	
	if (GetRandom(1, 10000) <= nProb )
	{
		CItem* item = gserver.m_itemProtoList.CreateItem( 2344, -1, 0, 0, 1); //노란 개나리
		if (item)
		{
			item->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(item, npc);
			ItemDropMsg(rmsg, npc, item);
			item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

			GAMELOG << init("EVENT CHILDRENSDAY 2007 ITEM DROP", tpc)
					<< "ITEM" << delim
					<< itemlog(item)
					<< end;
		}
	}
}
#endif // EVENT_CHILDRENSDAY_2007

#ifdef EVENT_FLOWERTREE_2007
void DropEventFlowerTree2007( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
#ifdef EVENT_NODROP_FLOWERTREE_2007
	return;
#endif //EVENT_NODROP_FLOWERTREE_2007

	if (!tpc)
	{
		return ;
	}

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
	{
		return;
	}

	int nProb = 0;
	int nHighLevel = tpc->m_level;

	if( pc->IsParty() ) // 파티중이면 파티원중 최고 레벨 
	{
		nProb = 100;		// 파티 사냥시  +1%
		for (int i=0; i < MAX_PARTY_MEMBER; i++)
		{
			CPC* pMemberPC = pc->m_party->GetNearMember(pc, i);
			if (!pMemberPC)
				continue;
		
			if ( pMemberPC->m_level > nHighLevel )
			{
				nHighLevel = pMemberPC->m_level;
			}
		}
	}

	if( nHighLevel - npc->m_level > 5 )	// 레벨 차이가 5이상이면 drop하지 않는다.  
	{ 
		return;
	}

	switch( (nHighLevel -1) / 10 )
	{
	case 0:	case 1:		// 1~20 Lv 2.5%
		nProb += 250;
		break;
	case 2:				// 21~30 Lv	5.5%
		nProb += 550;
		break;
	case 3: case 4:		// 31~50 Lv 5%
		nProb += 200;
		break;
	case 5:				// 51~60 Lv 7.5%
		nProb += 750;
		break;
	case 6: case 7:		// 61~80 Lv 10%
		nProb += 1000;
		break;
	default:			// 80~ Lv 12.5%
		nProb += 1250;
		break;
	}

	if( npc->m_level < tpc->m_level )			// 1레벨당 0.1% 드롭확률 감소
	{
		int diffLevel = tpc->m_level - npc->m_level;
		nProb -= 10 * diffLevel;
	}

	CNetMsg rmsg;
	
	if (GetRandom(1, 10000) <= nProb )
	{
		CItem* item = gserver.m_itemProtoList.CreateItem( GetRandom(1, 100)%2? 2345:2346 , -1, 0, 0, 1); // 2345 퇴비  또는 2346 정화수 
		if (item)
		{
			item->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(item, npc);
			ItemDropMsg(rmsg, npc, item);
			item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

			GAMELOG << init("EVENT FLOWERTREE 2007 ITEM DROP", tpc)
					<< "ITEM" << delim
					<< itemlog(item)
					<< end;
		}
	}
}
#endif // EVENT_FLOWERTREE_2007

#ifdef EVENT_INDEPENDENCE_DAY_2007_USA
void DropEventIndependenceDay2007USA(CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if(!tpc)
		return ;

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
		return ;

	const static int nDropProbMax = 80;
	const static int nDiffDropLevel = 9;
#if defined (LC_USA)
	// 별무늬 성조기, 줄무늬 성조기, 종족별 엉클샘모자(타이탄, 나이트, 힐러, 메이지, 로그, 소서러)
	const static int DropItem[] = { 2353, 2354, 2352, 2362, 2363, 2364, 2365, 2366};
#elif defined(LC_HBK)
	const static int DropItem[] = { 2409, 1311, 2411, 2412, 2413, 2414, 2415, 2416};
#else
	const static int DropItem[] = { 2409, 2410, 2411, 2412, 2413, 2414, 2415, 2416};
#endif
	int nDropProb = 0;
	struct tm tmNow = NOW();
	switch (tmNow.tm_min % 10)
	{
	case 0:
	case 9:
		if (tpc && tpc->m_bEventIndependenceDay2007drop)
			nDropProb = 100;
		break;
	default:
		tpc->m_bEventIndependenceDay2007drop = true;
		break;
	}

	if(nDropProb == 0)
		return ;

	if(tpc->m_level < npc->m_level)
	{
		nDropProb = nDropProbMax;
	}
	else
	{
		if(tpc->m_level - npc->m_level < nDiffDropLevel)
			nDropProb -= (tpc->m_level - npc->m_level) * 10;
		else
			return ;
	}

#ifdef WJKTEST
	nDropProb = 100;
#endif // WJKTEST

	if(GetRandom(1, 100) <= nDropProb)
	{
		tpc->m_bEventIndependenceDay2007drop = false;

		int nItemArrayIndex = 0;
		int nItemProb = GetRandom(1, 100);
		if(nItemProb <= 60)			nItemArrayIndex = 0;		// 별무늬 성조기(쟈스민꽃)
		else if(nItemProb <= 95)	nItemArrayIndex = 1;		// 줄무늬 성조기(브라질 국기)
		else
		{
			switch(tpc->m_job)
			{
			case 0:		nItemArrayIndex = 2;		break;	// 타이탄 엉클 샘 모자(나폴레옹 모자)
			case 1:		nItemArrayIndex = 3;		break;	// 나이트 엉클 샘 모자(나폴레옹 모자)
			case 2:		nItemArrayIndex = 4;		break;	// 힐러   엉클 샘 모자(나폴레옹 모자)
			case 3:		nItemArrayIndex = 5;		break;	// 메이지 엉클 샘 모자(나폴레옹 모자)
			case 4:		nItemArrayIndex = 6;		break;	// 로그   엉클 샘 모자(나폴레옹 모자)
			case 5:		nItemArrayIndex = 7;		break;	// 소서러 엉클 샘 모자(나폴레옹 모자)
			}
		}
		CItem* pItem = gserver.m_itemProtoList.CreateItem(DropItem[nItemArrayIndex], -1, 0, 0, 1);
		if(pItem)
		{
			CNetMsg rmsg;
			pItem->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(pItem, npc);
			ItemDropMsg(rmsg, npc, pItem);
			pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

			GAMELOG << init("EVENT INDEPENDENCE DAY 2007 USA ITEM DROP", tpc)
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< end;

		}
	}
}
#endif // EVENT_INDEPENDENCE_DAY_2007_USA

#ifdef EVENT_SUMMER_VACATION_2007
void DropEventSummerVacation2007( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	// 1. tpc 검사
	// 2. 퍼던 제외
	// 3. 확률 계산 : 1/1000, 1레벨당 1감소 , 5레벨 이상일경우 드롭 안됨
	// 4. 확률 적용	
	// 5. 아이템 드롭
	// 6. 게임 로그

	// 1. tpc 검사
	if( !tpc )
		return;

	// 2. 퍼던 제외
	if( tpc->m_pZone == NULL || tpc->m_pZone->IsPersonalDungeon() )		// 퍼스널 던전에서는 드롭하지 않는다
		return;

	if( level == -1 )
		return;

	if( level - npc->m_level > 5 )
		return;
	
	
	// 드롭 확률 2% 확률로 드롭 되며 3종류의 색종이가 개별 확률로 드롭된다
	int nProb = 20;



	if( tpc->m_level > npc->m_level )
		nProb -= (tpc->m_level - npc->m_level) * (10 / 100) ;

	//4. 확률 적용
	if ( GetRandom( 1, 1000 ) <= nProb )
	{
		int ItemIndex = 0;	// TODO: Item Index 결정 되면 변경 - whs25 ->070702
		int dropProb = GetRandom( 1, 1000 );
		if( dropProb > 800 )
			ItemIndex = SILVER_PAPER_ITEM;
		else if( dropProb >= 400 && dropProb <=  900 )
			ItemIndex = RED_PAPER_ITEM;
		else if( dropProb < 200 )
			ItemIndex = GREEN_PAPER_ITEM;
		// 5.아이템 드롭
		CItem * pItem = gserver.m_itemProtoList.CreateItem( ItemIndex, -1, 0, 0, 1 );
		if( !pItem )
			return;
		CNetMsg rmsg;
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem( pItem, npc );
		ItemDropMsg( rmsg, npc, pItem );
		pItem->m_pArea->SendToCell( rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ );

		GAMELOG << init( "EVENT PARENTS DAY 2007", tpc )
			<<	"NPC" << delim
			<< npc->m_idNum << delim
			<< npc->m_name << delim
			<< "NPC LEVEL" << delim
			<< npc->m_level << delim
			<< "PC LEVEL" << delim
			<< tpc->m_level << delim
			<< "ITEM" << delim
			<< itemlog( pItem )
			<< end;
		return;
	}	
}
#endif // EVENT_SUMMER_VACATION_2007



#ifdef EVENT_TLD_MOTHERDAY_2007
void DropEventMotherday2007(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	// Mother's day event drop item 인덱스 및 확률
	static const int tableItem[3][2] = {
		{2385, 10},		// Mother's card
		{2386, 10},		// Jasmine
		{2387, 3},		// Garland	
	};

	int		i;

	// 1. tpc 검사
	// 2. 퍼던 제외
	// 3. 확률 계산 : 1/1000, 기본 50, 1레벨당 5감소
	// 4. 확률 적용
	// 5. 모래 봉투 결정
	// 6. 아이템 드롭
	// 7. 게임 로그

	// 1. tpc 검사
	if (!tpc)
		return ;

	// 2. 퍼던 제외
	if (tpc->m_pZone->IsPersonalDungeon())
		return ;

	if ( tpc->m_level - npc->m_level > 5 )	// 5렙 이상 차이 드롭 안함
		return;

	// 5. item 결정
	for (i = 0; i < 3; i++)
	{
		if ( GetRandom(1, 1000) <= tableItem[i][1])
		{
			// 6. 아이템 드롭
			CItem* pItem = gserver.m_itemProtoList.CreateItem(tableItem[i][0], -1, 0, 0, 1);
			if (!pItem)
				return ;
			CNetMsg rmsg;
			pItem->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(pItem, npc);
			ItemDropMsg(rmsg, npc, pItem);
			pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);

			// 7. 게임 로그
			GAMELOG << init("EVENT TLD MOTHER DAY  DROP", tpc)
					<< "NPC" << delim
					<< npc->m_idNum << delim
					<< npc->m_name << delim
					<< "NPC LEVEL" << delim
					<< npc->m_level << delim
					<< "PC LEVEL" << delim
					<< tpc->m_level << delim
					<< "ITEM" << delim
					<< itemlog(pItem)
					<< end;			
		}
	}
}
#endif // EVENT_TLD_MOTHERDAY_2007


#if defined(GIFT_EVENT_2007) || defined (EVENT_OPEN_ADULT_SERVER) || defined (EVENT_MAGIC_CARD)
void DropEventOpenAdultServer(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc)
		return ;

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
		return;

	static const int nIndexItem[2] = { 2383, 2384 };		// 스레이스 A , 하트 A 카드 

	int nProb[2] = { 200, 100 };		// 홍콩 드롭률 3%
	if( npc->m_level < tpc->m_level )			// 1레벨당 0.1% 드롭확률 감소
	{
		int diffLevel = tpc->m_level - npc->m_level;

		int k;
		for(k = 0; k < 2; ++k)
		{
			nProb[k] -= 10 * diffLevel;
		}
	}

	CNetMsg rmsg;
	int i;
	for (i = 0; i < 2; i++)
	{
		if (GetRandom(1, 10000) <= nProb[i] )			// 1%의 확률
		{
			CItem* item = gserver.m_itemProtoList.CreateItem(nIndexItem[i], -1, 0, 0, 1);
			if (item)
			{
				item->m_preferenceIndex = tpc->m_index;
				npc->m_pArea->DropItem(item, npc);
				ItemDropMsg(rmsg, npc, item);
				item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

				GAMELOG << init("EVENT OPEN ADULT SERVER ITEM DROP", tpc)
						<< "ITEM" << delim
						<< itemlog(item)
						<< end;
			}
		}
	}
}
#endif // GIFT_EVENT_2007 || EVENT_OPEN_ADULT_SERVER || defined (EVENT_MAGIC_CARD)


#ifdef EVENT_DROPITEM
void DropEventNpcDropItem(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if(!gserver.m_bDropItem)
		return ;

	if (!tpc)
		return ;

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
		return;

	if(npc->m_proto->m_index != gserver.m_bDropItemNpcIdx)
		return ;

	int prob;
	int diffProb = 1;
	prob = gserver.m_itemDropProb;

	if( npc->m_level < tpc->m_level )
	{
		int diffLevel = tpc->m_level - npc->m_level;
		prob -= diffLevel * diffProb;
	}

	CNetMsg rmsg;
	if (GetRandom(1, 1000) <= prob )			// 1%의 확률
	{
		CItem* item = gserver.m_itemProtoList.CreateItem(gserver.m_bDropItemIdx, -1, 0, 0, 1);
		if (item)
		{
			item->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(item, npc);
			ItemDropMsg(rmsg, npc, item);
			item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);

			GAMELOG << init("EVENT NPC DROPITEM", tpc)
					<< "ITEM" << delim
					<< itemlog(item)
					<< end;
		}
	}
}
#endif // EVENT_DROPITEM


#ifdef EVENT_RICHYEAR_2007
void DropEventRichYear2007(CNPC* npc, CPC* pc, CPC* tpc, int level)
{
	if (!tpc)
		return ;

	CNetMsg rmsg;
	int nSeedIndex = 876;		// 씨앗
	int nProbSeed = 2;		// 씨앗 획득확률
#ifdef WJKTEST
	nProbSeed = 100;
#endif // WJKTEST
	// 씨앗 획득
	int row, col;
	if(!tpc->m_invenEvent.FindItem(&row, &col, 876, 0, 0)		// 씨앗
		&& !tpc->m_invenEvent.FindItem(&row, &col, 877, 0, 0)		// 묘목
		&& !tpc->m_invenEvent.FindItem(&row, &col, 878, 0, 0)		// 분재
		&& !tpc->m_invenEvent.FindItem(&row, &col, 879, 0, 0)		// 나무
		&& !tpc->m_invenEvent.FindItem(&row, &col, 2422, 0, 0))		// 로고
	{
		if(GetRandom(1, 100) <= nProbSeed)
		{
			CItem* pSeedItem = gserver.m_itemProtoList.CreateItem(nSeedIndex, -1, 0, 0, 1);
			if(pSeedItem)
			{
				if(AddToInventory(tpc, pSeedItem, false, true))
				{
					ItemAddMsg(rmsg, pSeedItem);
					SEND_Q(rmsg, tpc->m_desc);

					GAMELOG << init("EVENT RICHYEAR GET SEED", tpc)
							<< "ITEM" << delim
							<< itemlog(pSeedItem)
							<< end;
				}
				else
				{
					delete pSeedItem;
					pSeedItem = NULL;
				}
			}
		}
	}

	if(tpc->m_pZone->IsPersonalDungeon())
		return ;

	// 재료 아이템 획득
	if(npc->m_level <= 5)
		return ;

	int nNeedItemProb = 100;

#ifdef WJKTEST
	nNeedItemProb = 10000;
#endif // WJKTEST
	
	if( npc->m_level < tpc->m_level )
		nNeedItemProb -= (tpc->m_level - npc->m_level) * 5;

	if(GetRandom(1, 10000) <= nNeedItemProb)
	{
		int nNeedItemIndex = -1;
#ifdef EVENT_RICHYEAR_CHANGE_2008
		switch( rand()%4 )
		{
		case 0:
			nNeedItemIndex = 881;		// 배양토
			break;
		case 1:
			nNeedItemIndex = 1058;		// 붉은 배양토
			break;
		case 2:
			nNeedItemIndex = 1059;		// 푸른 배양토
			break;
		case 3:
			nNeedItemIndex = 1060;		// 황금 배양토
			break;
		default:
			return ;
		}
#else // EVENT_RICHYEAR_CHANGE_2008
#ifdef LC_HBK
		switch( rand()%4 )
		{
		case 0:
			nNeedItemIndex = 881;		// 유저의 관심
			break;
		case 1:
			nNeedItemIndex = 1058;		// 유저의 관용
			break;
		case 2:
			nNeedItemIndex = 1059;		// 유저의 사랑
			break;
		case 3:
			nNeedItemIndex = 1060;		// 유저의 질타
			break;
		default:
			return ;
		}
#else
		switch( rand()%4 )
		{
		case 0:
			nNeedItemIndex = 2418;		// 유저의 관심
			break;
		case 1:
			nNeedItemIndex = 2419;		// 유저의 관용
			break;
		case 2:
			nNeedItemIndex = 2420;		// 유저의 사랑
			break;
		case 3:
			nNeedItemIndex = 2421;		// 유저의 질타
			break;
		default:
			return ;
		}
#endif // LC_HBK
#endif // EVENT_RICHYEAR_CHANGE_2008

		CItem* pNeedItem = gserver.m_itemProtoList.CreateItem(nNeedItemIndex, -1, 0, 0, 1);
		if(pNeedItem)
		{
			pNeedItem->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(pNeedItem, npc);
			ItemDropMsg(rmsg, npc, pNeedItem);
			pNeedItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pNeedItem), pNeedItem->m_cellX, pNeedItem->m_cellZ);

			GAMELOG << init("EVENT RICHYEAR DROP ITEM", tpc)
					<< "ITEM" << delim
					<< itemlog(pNeedItem)
					<< end;
		}
	}
}
#endif // EVENT_RICHYEAR_2007


#ifdef EVENT_LC_1000DAY
void DropEventLC1000Day( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	// 드롭 아이템 인덱스 및 확률 2435, 2436, 2437, 2438, 2439, 2440, 2441, 2442, 2443, 2444, 2445 
	const int DropItem[11][2] = {
		{2435, 100 },
		{2436, 100 },
		{2437, 100 },
		{2438, 100 },
		{2439, 100 },
		{2440, 100 },
		{2441, 100 },
		{2442, 100 },
		{2443, 100 },
		{2444, 100 },
		{2445, 100 }
	};

	// npc 레벨 검사 ( 1 ~ 5 레베 드롭 안됨 )
	if( npc->m_level >= 1 && npc->m_level <= 5 )
		return;

		// tpc 검사
	if( !tpc )
		return;

	if ( tpc->m_level - npc->m_level > 5 )	// 5렙 이상 차이 드롭 안함
		return;


	// 퍼던 드롭 안됨 
	if( tpc->m_pZone->IsPersonalDungeon() )
		return;

	// 드롭 확률 결정 
	for( int i = 0; i < 11 ; i++ )
	{
		if( GetRandom( 1, 10000) <= DropItem[i][1] )
		{
			CItem* pItem = gserver.m_itemProtoList.CreateItem( DropItem[i][0], -1, 0, 0, 1 );
			if( !pItem )
				return;
			CNetMsg rmsg;
			pItem->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem( pItem, npc );
			ItemDropMsg( rmsg, npc, pItem );
			pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );

			// 게임 로그
			GAMELOG << init("EVENT LC 1000 DAY", tpc )
					<< "NPC" << delim
					<< npc->m_idNum << delim
					<< npc->m_name << delim
					<< "NPC LEVEL" << delim
					<< npc->m_level << delim
					<< "PC LEVEL" << delim
					<< tpc->m_level << delim
					<< "ITEM" << delim
					<< itemlog( pItem )
					<< end;

		}
	}
}	
#endif // EVENT_LC_1000DAY

#ifdef EVENT_HALLOWEEN_2007
void DropEventHalloween2007( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	// tpc 검사
	if( !tpc )
		return;

	if( npc->m_proto->m_index == 454 ) // 호박 레벨에 상관 없음
	{
		// 호박 드랍
		if( GetRandom( 1, 1000) <= 500 ) 
		{
			CItem* pItem = gserver.m_itemProtoList.CreateItem( 2491 /*호박*/, -1, 0, 0, 1 );
			if( !pItem )
				return;
			CNetMsg rmsg;
			pItem->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem( pItem, npc );
			ItemDropMsg( rmsg, npc, pItem );
			pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );

			GAMELOG << init("EVENT HALLOWEEN -PUMPKIN- ", tpc )
						<< "NPC" << delim
						<< npc->m_idNum << delim
						<< npc->m_name << delim
						<< "NPC LEVEL" << delim
						<< npc->m_level << delim
						<< "PC LEVEL" << delim
						<< tpc->m_level << delim
						<< "ITEM" << delim
						<< itemlog( pItem )
						<< end;
		}
	}

	if ( tpc->m_level > npc->m_level )	// 동랩 이하 드랍 안함
		return;

	// 사탕 바구니 드랍
#if defined (LC_GER)
	if( GetRandom( 1, 1000) <= 75 ) 
#else
	if( GetRandom( 1, 1000) <= 150 ) 
#endif
	{
		CItem* pItem = gserver.m_itemProtoList.CreateItem( 2490 /*사탕바구니*/, -1, 0, 0, 1 );
		if( !pItem )
			return;
		CNetMsg rmsg;
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem( pItem, npc );
		ItemDropMsg( rmsg, npc, pItem );
		pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );

		GAMELOG << init("EVENT HALLOWEEN -CANDY BASKET- ", tpc )
					<< "NPC" << delim
					<< npc->m_idNum << delim
					<< npc->m_name << delim
					<< "NPC LEVEL" << delim
					<< npc->m_level << delim
					<< "PC LEVEL" << delim
					<< tpc->m_level << delim
					<< "ITEM" << delim
					<< itemlog( pItem )
					<< end;
	}

	return;
}
#endif // EVENT_HALLOWEEN_2007

#ifdef EVENT_XMAS_2007
void DropEventXmas2007( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if(pc == NULL || tpc == NULL)
	{
		return;
	}

	int	nItem = 0;
	if( npc->m_proto->m_index == 483 ) // XMAS BOX 레벨에 상관 없음
	{
#ifdef LC_HBK
//		from 2607 to 492
//		from 2608 to 495
//		from 2609 to 786
//		from 2610 to 676
		const int	nBoxItemCount = 20;
		const int	nBoxDropItem[nBoxItemCount][2] = {
			{ 2604, 100 },		// 루돌프 펫 변신 수문서
			{ 2599, 40 },		// 나이트 빛나는 빨강산타모자
			{ 2598, 40 },		// 타이탄 빛나는 빨강산타모자
			{ 2606, 20 },		// 크리스마스 써치라이프 1일
			{ 492, 20 },		// 대형 체력회복증속 물약
			{ 495, 20 },		// 대형 마나회복증속 물약
			{ 786, 20 },		// 부스터
			{ 676, 20 },		// 용서의눈물
			{ 1843, 160 },		// 빨강 산타모자
			{ 1844, 160 },		// 녹색 산타모자	
			{ 2617, 40 },		// 타이탄 빛나는 녹색산타모자
			{ 2618, 40 },		// 나이트 빛나는 녹색산타모자
			{ 2600, 40 },		// 힐러 빛나는 빨강산타모자
			{ 2601, 40 },		// 메이지 빛나는 빨강산타모자
			{ 2602, 40 },		// 로그 빛나는 빨강산타모자
			{ 2603, 40 },		// 소서러 빛나는 빨강산타모자
			{ 2613, 40 },		// 힐러 빛나는 녹색산타모자
			{ 2614, 40 },		// 메이지 빛나는 녹색산타모자
			{ 2615, 40 },		// 로그 빛나는 녹색산타모자
			{ 2616, 40 },		// 소서러 빛나는 녹색산타모자 
		};
#elif defined (LC_TLD)
		const int	nBoxItemCount = 18;
		const int	nBoxDropItem[nBoxItemCount][2] = {
			{ 2604, 100 },		// 루돌프 펫 변신 수문서
			{ 2599, 40 },		// 나이트 빛나는 빨강산타모자
			{ 2598, 40 },		// 타이탄 빛나는 빨강산타모자
			{ 2606, 20 },		// 크리스마스 써치라이프 1일
			{ 2607, 20 },		// 크리스마스 창고이용 주문서 1일
			{ 2608, 20 },		// 크리스마스 잡화상이용 주문서 1일
//			{ 2609, 20 },		// 크리스마스 광속 5일
//			{ 2610, 20 },		// 크리스마스 오토포션펜던트 1일
			{ 1843, 180 },		// 빨강 산타모자
			{ 1844, 180 },		// 녹색 산타모자	
			{ 2617, 40 },		// 타이탄 빛나는 녹색산타모자
			{ 2618, 40 },		// 나이트 빛나는 녹색산타모자
			{ 2600, 40 },		// 힐러 빛나는 빨강산타모자
			{ 2601, 40 },		// 메이지 빛나는 빨강산타모자
			{ 2602, 40 },		// 로그 빛나는 빨강산타모자
			{ 2603, 40 },		// 소서러 빛나는 빨강산타모자
			{ 2613, 40 },		// 힐러 빛나는 녹색산타모자
			{ 2614, 40 },		// 메이지 빛나는 녹색산타모자
			{ 2615, 40 },		// 로그 빛나는 녹색산타모자
			{ 2616, 40 },		// 소서러 빛나는 녹색산타모자 
		};
#elif defined (LC_JPN)
// 펫 변신 아이템		2604	1 	10%
// 루돌프 코			-	1	10% xxxxx
// 산타 모자(red)	1843	1	25%
// 산타 모자(green) 1844	1	25%
// 폭죽				2403	5	30%
		const int	nBoxItemCount = 4;
		const int	nBoxDropItem[nBoxItemCount][3] = {
			{ 2604, 200, 1},		// 루돌프 펫 변신 수문서
			{ 1843, 250, 1},		// 빨강 산타모자
			{ 1844, 250, 1},		// 녹색 산타모자
			{ 2403, 300, 5},		// 폭죽
		};
#elif defined (LC_USA)
		const int	nBoxItemCount = 18;
		const int	nBoxDropItem[nBoxItemCount][2] = {
			{ 2604, 100 },		// 루돌프 펫 변신 수문서
			{ 2599, 40 },		// 나이트 빛나는 빨강산타모자
			{ 2598, 40 },		// 타이탄 빛나는 빨강산타모자
			{ 2606, 20 },		// 크리스마스 써치라이프 1일
			{ 2607, 20 },		// 크리스마스 창고이용 주문서 1일
			{ 2608, 20 },		// 크리스마스 잡화상이용 주문서 1일
			{ 2609, 20 },		// 크리스마스 광속 5일
			{ 2610, 20 },		// 크리스마스 오토포션펜던트 1일
			{ 2617, 40 },		// 타이탄 빛나는 녹색산타모자
			{ 2618, 40 },		// 나이트 빛나는 녹색산타모자
			{ 2600, 40 },		// 힐러 빛나는 빨강산타모자
			{ 2601, 40 },		// 메이지 빛나는 빨강산타모자
			{ 2602, 40 },		// 로그 빛나는 빨강산타모자
			{ 2603, 40 },		// 소서러 빛나는 빨강산타모자
			{ 2613, 40 },		// 힐러 빛나는 녹색산타모자
			{ 2614, 40 },		// 메이지 빛나는 녹색산타모자
			{ 2615, 40 },		// 로그 빛나는 녹색산타모자
			{ 2616, 40 },		// 소서러 빛나는 녹색산타모자 
		};
#else
		const int	nBoxItemCount = 20;
		const int	nBoxDropItem[nBoxItemCount][2] = {
			{ 2604, 100 },		// 루돌프 펫 변신 수문서
			{ 2599, 40 },		// 나이트 빛나는 빨강산타모자
			{ 2598, 40 },		// 타이탄 빛나는 빨강산타모자
			{ 2606, 20 },		// 크리스마스 써치라이프 1일
			{ 2607, 20 },		// 크리스마스 창고이용 주문서 1일
			{ 2608, 20 },		// 크리스마스 잡화상이용 주문서 1일
			{ 2609, 20 },		// 크리스마스 광속 5일
			{ 2610, 20 },		// 크리스마스 오토포션펜던트 1일
			{ 1843, 160 },		// 빨강 산타모자
			{ 1844, 160 },		// 녹색 산타모자	
			{ 2617, 40 },		// 타이탄 빛나는 녹색산타모자
			{ 2618, 40 },		// 나이트 빛나는 녹색산타모자
			{ 2600, 40 },		// 힐러 빛나는 빨강산타모자
			{ 2601, 40 },		// 메이지 빛나는 빨강산타모자
			{ 2602, 40 },		// 로그 빛나는 빨강산타모자
			{ 2603, 40 },		// 소서러 빛나는 빨강산타모자
			{ 2613, 40 },		// 힐러 빛나는 녹색산타모자
			{ 2614, 40 },		// 메이지 빛나는 녹색산타모자
			{ 2615, 40 },		// 로그 빛나는 녹색산타모자
			{ 2616, 40 },		// 소서러 빛나는 녹색산타모자 
		};
#endif // LC_HBK
		
#ifdef LC_USA
		int prob = GetRandom(0, 680);
#else
		int prob = GetRandom(0, 1000 );
#endif 
		nItem = 0;
		while( prob >= 0 )
		{
			prob -= nBoxDropItem[nItem][1];
			nItem++;
		}

#ifdef LC_USA
		if( nItem < 19 )
#elif defined(LC_JPN)
		if( nItem < 5 )
#else
		if( nItem < 21 )
#endif // LC_USA
		{
#ifdef LC_JPN
			CItem* pItem = gserver.m_itemProtoList.CreateItem( nBoxDropItem[nItem-1][0] , -1, 0, 0, nBoxDropItem[nItem-1][2] );
#else
			CItem* pItem = gserver.m_itemProtoList.CreateItem( nBoxDropItem[nItem-1][0] , -1, 0, 0, 1 );
#endif // LC_JPN
			if( pItem )
			{
				CNetMsg rmsg;
				pItem->m_preferenceIndex = tpc->m_index;
				npc->m_pArea->DropItem( pItem, npc );
				ItemDropMsg( rmsg, npc, pItem );
				pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );

				GAMELOG << init("EVENT XMAS BOX DROP ITEM ", tpc ) 
						<< "NPC" << delim
						<< npc->m_idNum << delim
						<< npc->m_name << delim
						<< "NPC LEVEL" << delim
						<< npc->m_level << delim
						<< "PC LEVEL" << delim
						<< tpc->m_level << delim
						<< "ITEM" << delim
						<< itemlog( pItem )
						<< end;
			}
		}
	}

	const int ItemCount = 6;
	int nXmasItem[ItemCount] = { 2584, 2585, 2586, 2587, 2588, 2589 }; //양말,막대사탕 , 종, 리본, 구슬, 별

	if( tpc->m_level - npc->m_level > 5 )	// 5레벨 차이 이상 드랍 안함
		return;

	for( nItem=0; nItem<ItemCount; nItem++ )
	{
		if( GetRandom( 1, 1000) <= 10 ) 
		{
			CItem* pItem = gserver.m_itemProtoList.CreateItem( nXmasItem[nItem] , -1, 0, 0, 1 );
			if( pItem )
			{
				CNetMsg rmsg;
				pItem->m_preferenceIndex = tpc->m_index;
				npc->m_pArea->DropItem( pItem, npc );
				ItemDropMsg( rmsg, npc, pItem );
				pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );

				GAMELOG << init("EVENT XMAS DECO ", tpc ) 
						<< "NPC" << delim
						<< npc->m_idNum << delim
						<< npc->m_name << delim
						<< "NPC LEVEL" << delim
						<< npc->m_level << delim
						<< "PC LEVEL" << delim
						<< tpc->m_level << delim
						<< "ITEM" << delim
						<< itemlog( pItem )
						<< end;
			}
		}
	}
}
#endif // EVENT_XMAS_2007

//#ifdef ALTERNATE_MERCHANT
//bool CArea::AletenateMerchantRegen(float x, float z, float r, char y)
//{
//	void* pos = m_AlternateMerchantList.GetHead();
//	while (pos)
//	{
//		CNPCRegenInfo* p = m_AlternateMerchantList.GetData(pos);
//		pos = m_AlternateMerchantList.GetNext(pos);
//
//		if (p)
//		{
//			// totalnum이 -1이면 무한리젠
//			// totalnum이 0이면 스킵
//			if (p->m_totalNum == 0)
//				continue ;
//			//  totalNum이 -1 아니고 numRegen이 totalNum 보다 크면 NPC가 있으면 리젠안함
//			if ((p->m_totalNum != -1) && (p->m_numRegen >= p->m_totalNum))
//				continue;
//
//			CNPC* npc;
//			int cx, cz;
//			npc = gserver.m_npcProtoList.Create(p->m_npcIdx, p);
//			if(!npc)
//			{
//				p->m_totalNum = 0;
//				continue;
//			}
//
//			// 속성맵상 공성 지역이고 피스 NPC가 아니면 리젠안함
//			if (!npc->m_proto->CheckFlag(NPC_PEACEFUL))
//			{
//				switch (GetAttr(y, x, z))
//				{
//				case MAPATT_BLOCK:
//				case MAPATT_WARZONE:
//					delete npc;
//					continue ;
//				}
//			}
//
//			GET_X(npc) = x;
//			GET_YLAYER(npc) = y;
//			GET_Z(npc) = z;
// 			GET_R(npc) = r;
//
//			npc->m_regenX = GET_X(npc);
//			npc->m_regenY = GET_YLAYER(npc);
//			npc->m_regenZ = GET_Z(npc);
//			npc->m_recoverPulse = gserver.m_pulse;
//
//#ifndef NEW_DIVISION_EXPSP
//			npc->m_totalDamage = 0;
//#endif // #ifndef NEW_DIVISION_EXPSP
//
//			p->m_bAlive = true;
//			p->m_numRegen++;
//
//			p->m_bEventXMasMoveRandom = false;
//			
//			p->m_lastDieTime = 0;
//			npc->InitPointsToMax();
//			npc->m_disableTime = 1;
//			AddNPC(npc);
//			PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
//			CharToCell(npc, GET_YLAYER(npc), cx, cz);
//
//			return true;
//		}
//	}
//
//	return false;
//}
//#endif // ALTERNATE_MERCHANT

#ifdef MONSTER_COMBO
void DropComboGiftMob( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if( !tpc )
		return ;

	if( !tpc->m_pZone->IsComboZone() )
		return ;
	
	// 선물상자 npc인지 검사
	if(npc->m_proto->m_index == 492)
	{
		CNetMsg rmsg;
		CItem* item = gserver.m_itemProtoList.CreateItem(npc->m_coinidx, -1, 0, 0, npc->m_coincount);
		if (item)
		{
			item->m_preferenceIndex = tpc->m_index;
			npc->m_pArea->DropItem(item, npc);
			ItemDropMsg(rmsg, npc, item);
			item->m_pArea->SendToCell(rmsg, GET_YLAYER(item), item->m_cellX, item->m_cellZ);
			
			GAMELOG << init("DROP_MONSTER_COMBO_COIN", tpc)
				<< "ITEM" << delim
				<< itemlog(item)
				<< end;
		}
	}
}
#endif // MONSTER_COMBO

#ifdef SAKURA_EVENT_2008
void DropEventSakura2008( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	// 이벤트 아이템 드랍
	if (!tpc || !npc )
		return ;

	if(tpc->m_pZone->IsPersonalDungeon())		// 퍼스널 던전에서는 드롭하지 않는다.
		return;

	int nProb = 0;
	int nHighLevel = tpc->m_level;

	if( pc->IsParty() ) // 파티중이면 파티원중 최고 레벨 
	{
		nProb = 10;		// 파티 사냥시  +1%
		for (int i=0; i < MAX_PARTY_MEMBER; i++)
		{
			CPC* pMemberPC = pc->m_party->GetNearMember(pc, i);
			if (!pMemberPC)
				continue;
		
			if ( pMemberPC->m_level > nHighLevel )
			{
				nHighLevel = pMemberPC->m_level;
			}
		}
	}

	switch( (nHighLevel-1) / 10 )
	{
	case 0 : case 1: 		nProb = 10;		break;		// Lv	1~20	: 1%
	case 2 :				nProb = 20;		break;		// Lv	21~30	: 2%
	case 3 : case 4:		nProb = 30;		break;		// Lv	31~50	: 3%
	case 5 :				nProb = 40;		break;		// Lv	51~60	: 4%
	case 6 : case 7:		nProb = 50;		break;		// Lv	61~80	: 5%
	case 8 : default :		nProb = 60;		break;		// Lv	80~		: 6%
	}

	if( npc->m_level < nHighLevel )			// 1레벨당 0.1% 드롭확률 감소
	{
		int diffLevel = nHighLevel - npc->m_level;
		if( diffLevel > 5 )						// 5레벨 차이 이상 드롭 안함
			return;
		nProb -= diffLevel;
	}

	// 퇴비 드랍
	if( GetRandom( 1, 1000) <= nProb ) 
	{
		CItem* pItem = gserver.m_itemProtoList.CreateItem( 2345 /*퇴비*/, -1, 0, 0, 1 );
		if( !pItem )
			return;
		CNetMsg rmsg;
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem( pItem, npc );
		ItemDropMsg( rmsg, npc, pItem );
		pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );

		GAMELOG << init("EVENT SAKURA 2008 SEED ITEM ", tpc )
					<< "NPC" << delim
					<< npc->m_idNum << delim
					<< npc->m_name << delim
					<< "NPC LEVEL" << delim
					<< npc->m_level << delim
					<< "PC LEVEL" << delim
					<< tpc->m_level << delim
					<< "ITEM" << delim
					<< itemlog( pItem )
					<< end;
	}
}
#endif //SAKURA_EVENT_2008

#ifdef ATTACK_PET
void DropAPetLifeBook( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	if( !tpc )
		return ;

	if( tpc->m_pZone->IsComboZone() || tpc->m_pZone->IsPersonalDungeon() )
		return ;

	const int nChapterCount=2;
	int nLifeBookIndex[nChapterCount] = { 2947, 2948, };

	if( GetRandom( 1, 1000 ) == 777 )
	{
		CItem* pItem = gserver.m_itemProtoList.CreateItem( nLifeBookIndex[GetRandom(0,nChapterCount-1)] , -1, 0, 0, 1 );
		if( !pItem )
			return;
		CNetMsg rmsg;
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem( pItem, npc );
		ItemDropMsg( rmsg, npc, pItem );
		pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );

		GAMELOG << init("APET LIFE BOOK ITEM DROP ", tpc )
					<< "NPC" << delim
					<< npc->m_idNum << delim
					<< npc->m_name << delim
					<< "NPC LEVEL" << delim
					<< npc->m_level << delim
					<< "PC LEVEL" << delim
					<< tpc->m_level << delim
					<< "ITEM" << delim
					<< itemlog( pItem )
					<< end;
	}

}
#endif // ATTACK_PET

#ifdef EVENT_PHOENIX	// //피닉스 이벤트 가입권 드랍 확률 yhj	
void DropPhoenix_MembersTicket( CNPC* npc, CPC* pc, CPC* tpc, int level )
{
	// 이벤트 아이템 드랍
	if (!tpc || !npc )
		return ;

	// 동굴 존이 아니면 드랍하지 않는다.
	if( tpc->m_pZone->m_index != ZONE_SPRIT_CAVE
		&& tpc->m_pZone->m_index != ZONE_QUANIAN_CAVE
		&& tpc->m_pZone->m_index != ZONE_GOLEM_CAVE )		
		return;

	// 가입권 드랍
	if( GetRandom(1, 10000) <= DROP_PHOENIX_MEMBERS_TICKET ) 
	{
		CItem* pItem = gserver.m_itemProtoList.CreateItem( 3217, -1, 0, 0, 1 );
		if( !pItem )
			return;
		CNetMsg rmsg;
		pItem->m_preferenceIndex = tpc->m_index;
		npc->m_pArea->DropItem( pItem, npc );
		ItemDropMsg( rmsg, npc, pItem );
		pItem->m_pArea->SendToCell( rmsg, GET_YLAYER( pItem ), pItem->m_cellX, pItem->m_cellZ );

		GAMELOG << init("EVENT_PHOENIX TICKET ITEM DROP ", tpc )
					<< "NPC" << delim
					<< npc->m_idNum << delim
					<< npc->m_name << delim
					<< "NPC LEVEL" << delim
					<< npc->m_level << delim
					<< "PC LEVEL" << delim
					<< tpc->m_level << delim
					<< "ITEM" << delim
					<< itemlog( pItem )
					<< end;
	}

}
#endif // EVENT_PHOENIX