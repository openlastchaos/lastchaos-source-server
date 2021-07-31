#include "stdhdrs.h"
#include "Log.h"
#include "Zone.h"
#include "Server.h"
#include "DBCmd.h"
#include "CmdMsg.h"

CZone::CZone()
: m_remoteIP(HOST_LENGTH + 1)
{
	m_index = -1;
	m_bRemote = false;
	m_remotePort = 0;
	m_countY = 0;
	m_attrMap = NULL;
	m_area = NULL;
	m_countArea = 0;
	m_weather = 0;

	m_zonePos = NULL;

	m_shopList = NULL;
	m_nShopCount = 0;
	m_sellRate = 100;
	m_buyRate = 100;

	m_bCanMemPos = false;
#ifdef ENABLE_PET
	m_bCanSummonPet = false;
	m_bCanMountPet = false;
#endif
}

CZone::~CZone()
{
	if (m_area)
		delete[] m_area;
	if (m_attrMap)
		delete[] m_attrMap;
	if (m_zonePos)
	{
		for (int i = 0; i < m_countZonePos; i++)
		{
			delete [] m_zonePos[i];
		}
		delete [] m_zonePos;
		m_zonePos = NULL;
	}

	m_index = -1;
	m_bRemote = false;
	m_remotePort = 0;
	m_countY = 0;
	m_attrMap = NULL;
	m_area = NULL;

	if(m_shopList)
		delete [] m_shopList;
	m_shopList = NULL;

	m_nShopCount = 0;
	m_weather = 0;
	m_sellRate = 100;
	m_buyRate = 100;
}

bool CZone::LoadZone(int seq)
{
	if (seq < 0 || seq >= gserver.m_numZone)
		return false;

	GAMELOG << init("SYSTEM")
			<< "Loading zone ";

	CLCString zonegroup(100);
	zonegroup.Format("Zone_%d", seq);

	const char* p;
	int i;

	if (!(p = gserver.m_config.Find(zonegroup, "No")))
		return false;
	m_index = atoi(p);

	GAMELOG << m_index
			<< end;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbdata);
	strcpy(g_buf, "SELECT * FROM t_zonedata WHERE a_zone_index=");
	IntCat(g_buf, m_index, false);
	cmd.SetQuery(g_buf);
	if (!cmd.Open() || !cmd.MoveFirst())
		return false;

	p = gserver.m_config.Find(zonegroup, "Remote");
	if (!p || strcmp(p, "FALSE") == 0)
	{
		m_bRemote = false;

		int sizex, sizez;
		if (!cmd.GetRec("a_width", sizex))
			return false;
		if (!cmd.GetRec("a_height", sizez))
			return false;

		if (!cmd.GetRec("a_ylayer", m_countY))
			return false;

		CLCString attrmaps(65536);
		CLCString heightmaps(65536);
		if (!cmd.GetRec("a_attrmap", attrmaps))
			return false;
		if (!cmd.GetRec("a_heightmap", heightmaps))
			return false;
		const char* pattr = attrmaps;
		const char* pheight = heightmaps;

		m_attrMap = new CMapAttr[m_countY];
		for (i = 0; i < m_countY; i++)
		{
			pattr = AnyOneArg(pattr, g_buf);
			pheight = AnyOneArg(pheight, g_buf2);

			if (!m_attrMap[i].Load(seq, i, sizex, sizez, g_buf, g_buf2))
				return false;
		}

		if (!cmd.GetRec("a_areacount", m_countArea))
			return false;
		if (m_countArea < 1)
			return false;

		m_area = new CArea[m_countArea];
		for (i = 0; i < m_countArea; i++)
			m_area[i].Init(this, i, sizex, sizez, m_countY);
	}
	else
	{
		m_bRemote = true;

		p = gserver.m_config.Find(zonegroup, "Remote_IP");
		if (!p)
			return false;
		m_remoteIP = p;

		p = gserver.m_config.Find(zonegroup, "Remote_Port");
		if (!p)
			return false;
		m_remotePort = (unsigned short)atoi(p);
	}

	char canusemempos;
	if (!cmd.GetRec("a_canusemempos", canusemempos))
		return false;
	m_bCanMemPos = (canusemempos) ? true : false;

#ifdef ENABLE_PET
	char cansummonpet;
	if (!cmd.GetRec("a_cansummonpet", cansummonpet))
		return false;
	m_bCanSummonPet = (cansummonpet) ? true : false;

	char canmountpet;
	if (!cmd.GetRec("a_canmountpet", canmountpet))
		return false;
	m_bCanMountPet = (canmountpet) ? true : false;
#endif

	// 존의 마을 위치 읽기
	char poscount;
	CLCString posylayer(256);
	CLCString posleft(256);
	CLCString postop(256);
	CLCString posright(256);
	CLCString posbottom(256);

	if (!cmd.GetRec("a_poscount", poscount))
		return false;
	if (!cmd.GetRec("a_posylayer", posylayer))
		return false;
	if (!cmd.GetRec("a_posleft", posleft))
		return false;
	if (!cmd.GetRec("a_postop", postop))
		return false;
	if (!cmd.GetRec("a_posright", posright))
		return false;
	if (!cmd.GetRec("a_posbottom", posbottom))
		return false;

	const char* pylayer = posylayer;
	const char* pleft = posleft;
	const char* ptop = postop;
	const char* pright = posright;
	const char* pbottom = posbottom;

	m_countZonePos = poscount;
#ifdef ENABLE_WAR_CASTLE
	if (m_index == ZONE_MERAC && m_countZonePos < 17)
	{
		GAMELOG << init("ERROR")
				<< "ZONE MERAC MUST HAVE 17 REGEN POSITION"
				<< end;
		return false;
	}
#endif // ENABLE_WAR_CASTLE
	m_zonePos = new int*[m_countZonePos];
	for (i = 0; i < m_countZonePos; i++)
	{
		m_zonePos[i] = new int[5];

		char sy[256];
		char sl[256];
		char st[256];
		char sr[256];
		char sb[256];

		pylayer = AnyOneArg(pylayer, sy);
		pleft = AnyOneArg(pleft, sl);
		ptop = AnyOneArg(ptop, st);
		pright = AnyOneArg(pright, sr);
		pbottom = AnyOneArg(pbottom, sb);

		m_zonePos[i][0] = atoi(sy);
		m_zonePos[i][1] = (int)(atof(sl) * 2);
		m_zonePos[i][2] = (int)(atof(st) * 2);
		m_zonePos[i][3] = (int)(atof(sr) * 2);
		m_zonePos[i][4] = (int)(atof(sb) * 2);
	}

	return true;
}

bool CZone::LoadNPC()
{
	if (m_bRemote)
		return true;

	// 각 존의 Area마다 NPC 리젠 정보 읽기
	int i;
	for (i = 0; i < m_countArea; i++)
		m_area[i].LoadNPC();

	if (m_countArea == 1)
		m_area[0].SetEnable();

#ifdef EXTREME_CUBE
	if(IsExtremeCube())
	{
		for (i = 0; i < m_countArea; i++)
			m_area[i].SetEnable();
	}
#endif // EXTREME_CUBE

	return true;
}

bool CZone::LoadShop()
{
	bool bRet = true;

	if (m_bRemote)
		return true;

	if (m_shopList)
		delete [] m_shopList;
	m_shopList = NULL;

	GAMELOG << init("SYSTEM")
			<< "Shop Loading Zone "
			<< m_index
			<< end;

	// area가 속한 해당 존 넘버의 Shop정보 READ
	CDBCmd dbShop;
	dbShop.Init(&gserver.m_dbdata);
	strcpy(g_buf, "SELECT * FROM t_shop WHERE a_zone_num = ");
	IntCat(g_buf, m_index);
	StrCat(g_buf, "ORDER BY a_keeper_idx");
	dbShop.SetQuery(g_buf);
	dbShop.Open();

	m_nShopCount = dbShop.m_nrecords;

	if (m_nShopCount < 1)
		return true;

	m_shopList = new CShop[m_nShopCount];

	int idx = 0;

	CDBCmd dbShopItem;
	dbShopItem.Init(&gserver.m_dbdata);

	//CNPC* npc;
	while (dbShop.MoveNext())
	{
		dbShop.GetRec("a_keeper_idx",	m_shopList[idx].m_keeperIdx);
		dbShop.GetRec("a_sell_rate",	m_shopList[idx].m_sellRate);
		dbShop.GetRec("a_buy_rate",		m_shopList[idx].m_buyRate);

		dbShop.GetRec("a_pos_x",		m_shopList[idx].m_x);
		dbShop.GetRec("a_pos_z",		m_shopList[idx].m_z);
		dbShop.GetRec("a_pos_r",		m_shopList[idx].m_r);
		dbShop.GetRec("a_pos_h",		m_shopList[idx].m_h);
		dbShop.GetRec("a_y_layer",		m_shopList[idx].m_yLayer);

		CLCString sql(2048);
		
#ifdef LC_KOR
		sql.Format(
			"SELECT * FROM t_shopitem where a_keeper_idx = %d ORDER BY a_item_idx"
			, m_shopList[idx].m_keeperIdx);
#else
		sql.Format(
			"SELECT * FROM t_shopitem where a_keeper_idx = %d and !(a_national & (1 << %d) ) ORDER BY a_item_idx"
			, m_shopList[idx].m_keeperIdx, gserver.m_national);
#endif
		dbShopItem.SetQuery(sql);
		dbShopItem.Open();

		// 상점 보유 아이템
		m_shopList[idx].m_itemCount = dbShopItem.m_nrecords;
		m_shopList[idx].m_itemDBIdx = new int[m_shopList[idx].m_itemCount];

		int itemDBIdx = 0;
		while(dbShopItem.MoveNext())
			m_shopList[idx].m_itemDBIdx[itemDBIdx++] = atoi(dbShopItem.GetRec("a_item_idx"));

		if (m_shopList[idx].m_sellRate < 0 || m_shopList[idx].m_buyRate < 0)
		{
			GAMELOG << init("ERROR: SHOP SELL/BUY RATE")
					<< m_index << delim
					<< m_shopList[idx].m_keeperIdx << delim
					<< m_shopList[idx].m_sellRate << delim
					<< m_shopList[idx].m_buyRate
					<< end;
			bRet = false;
		}

		idx++;
	}

	return bRet;
}

int CZone::SetEnableArea()
{
	int idx;
	for (idx = 0; idx < m_countArea; idx++)
	{
		if (!m_area[idx].m_bEnable)
			break;
	}
	if (idx == m_countArea)
		return -1;

	m_area[idx].SetEnable();

	return idx;
}

void CZone::SetDisableArea(int idx)
{
	if (idx < 0  || idx >= m_countArea)
		return ;
	m_area[idx].SetDisable();
}

// npc index에 따른 shop 찾기
CShop* CZone::FindShop(int npcIdx)
{
	CShop shop;
	shop.m_keeperIdx = npcIdx;

	return (CShop*)bsearch(&shop, m_shopList, m_nShopCount, sizeof(CShop), CompIndex);

}

void CZone::ChangeWeather()
{
	int i;
	unsigned char before = m_weather;
	
	switch (m_weather) {
	case 0:// 맑음
		if (!GetRandom(0, 5))
			m_weather = 1;
		break;
	case 1: // 흐림
		i = GetRandom(0, 2);
		if (i == 0)
			m_weather = 0;
		else if (i == 1)
			m_weather = 2;
		break;
	case 2: // 비 조금옴...
		if (!GetRandom(0, 1))
			m_weather = 1;
		else
			m_weather = 3;
		break;
	case 3: // 비 쪼까 옴...
		if (!GetRandom(0, 1))
			m_weather = 2;
		else
			m_weather = 4;
		break;
	case 4: //비 왕창 옴....
		if (!GetRandom(0, 1))
			m_weather = 3;
		break;
	default:
		m_weather = 2;
		break;
	}

	if (before != m_weather)
	{
		for (i=0; i < m_countArea; i++)
		{
			CNetMsg weatherMsg;
			EnvWeatherMsg(weatherMsg, this->m_index);
			this->m_area[i].SendToAllClient(weatherMsg);
		}
	}
}

void CZone::ChangeTaxRate(int sellRate, int buyRate)
{
	int i;

	m_sellRate = sellRate;
	m_buyRate = buyRate;

	for (i=0; i < m_nShopCount; i++)
	{
		m_shopList[i].m_sellRate = m_sellRate;
		m_shopList[i].m_buyRate = m_buyRate;
	}
}

int CZone::FindEmptyArea()
{
	int idx;

	for (idx = 0; idx < m_countArea; idx++)
	{
		if (!m_area[idx].m_bEnable)
			return idx;
	}

	return -1;
}


#ifdef MONSTER_COMBO
int CZone::FindComboArea(int comboidx)
{
	if(m_index != ZONE_COMBO_DUNGEON)
			return -1;
	
	int i;
	for(i = 0 ; i < m_countArea; ++i)
	{
		if(!m_area[i].m_bEnable)
			continue;

		if(!m_area[i].m_monsterCombo)
			continue;

		if(m_area[i].m_monsterCombo->m_nIndex == comboidx)
			return i;
	}

	return -1;
}
#endif 

int CZone::GetExtra(int x, int z)
{
	for (int i = 0; i < m_countZonePos; i++)
	{
		if ((m_zonePos[i][1] / 2) < x && (m_zonePos[i][2] / 2) < z && (m_zonePos[i][3] / 2) > x && (m_zonePos[i][4] / 2) > z)
		{
			return i;
		}
	}

	return -1;
}
