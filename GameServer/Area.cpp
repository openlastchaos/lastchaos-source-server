#include "stdhdrs.h"
#include "Log.h"
#include "Area.h"
#include "CmdMsg.h"
#include "Zone.h"
#include "Server.h"
#include "Env.h"
#include "DBCmd.h"
#include "WarCastle.h"

CArea::CArea()
: m_zone(NULL)
#ifdef MONSTER_RAID_SYSTEM
, m_raidNPCRegenList(NULL)
#endif // MONSTER_RAID_SYSTEM
#ifdef EVENT_XMAS_2006
, m_eventXMas2006RegenList(NULL)
#endif // EVENT_XMAS_2006
#ifdef ALTERNATE_MERCHANT
, m_AlternateMerchantList( NULL )
#endif // ALTERNATE_MERCHANT
{
	m_index = -1;
	m_size[0] = m_size[1] = 0;
	m_ycnt = 0;
	m_env = NULL;
	m_cell = NULL;
	m_npcList = NULL;
	m_nCountNPC = 0;
	m_nJunkItems = 0;
	m_nMakeNPC = 0;
	m_guildIndex = -1;
	m_bBoss = false;
	m_regenList = NULL;

#ifdef MONSTER_COMBO
	m_monsterCombo = NULL;
#endif // MONSTER_COMBO
	
#ifdef NIGHTSHADOW_SKILL	// 
	m_nNPC_Killed_NPC = 0;
#endif  // NIGHTSHADOW_SKILL
}

CArea::~CArea()
{
	if (m_env)
	{
		int i, y;
		for (y = 0; y < m_ycnt; y++)
		{
			for (i = 0; i < m_size[0]; i++)
				delete[] m_env[y][i];
			delete [] m_env[y];
		}
		delete[] m_env;
	}
	
	if (m_cell)
	{
		int i;
		for (i = 0; i < m_size[0]; i++)
			delete[] m_cell[i];
		delete[] m_cell;
	}
	
	if (m_npcList)
	{
		CNPC* p;
		CNPC* pn = m_npcList;
		while ((p = pn))
		{
			pn = p->m_pNPCNext;
			p->DeleteNPC();
			p = NULL;
		}
	}
	
	m_zone = NULL;
	m_size[0] = m_size[1] = 0;
	m_ycnt = 0;
	m_env = NULL;
	m_cell = NULL;

#ifdef MONSTER_RAID_SYSTEM
	while (m_raidNPCRegenList.GetCount() > 0)
	{
		void* pos = m_raidNPCRegenList.GetHead();
		CNPCRegenInfo* p = m_raidNPCRegenList.GetData(pos);
		m_raidNPCRegenList.Remove(pos);
		delete p;
	}
#endif // MONSTER_RAID_SYSTEM

#ifdef EVENT_XMAS_2006
	while (m_eventXMas2006RegenList.GetCount() > 0)
	{
		void* pos = m_eventXMas2006RegenList.GetHead();
		CNPCRegenInfo* p = m_eventXMas2006RegenList.GetData(pos);
		m_eventXMas2006RegenList.Remove(pos);
		delete p;
	}
#endif // EVENT_XMAS_2006
//
//#ifdef ALTERNATE_MERCHANT
//	while( m_AlternateMerchantList.GetCount() > 0 )
//	{
//		void* pos = m_AlternateMerchantList.GetHead();
//		CNPCRegenInfo* pAlterRegneInfo = m_AlternateMerchantList.GetData( pos );
//		m_AlternateMerchantList.Remove( pos );
//		delete pAlterRegneInfo;
//	}
//#endif // ALTERNATE_MERCHANT

#ifdef MONSTER_COMBO
	if(m_monsterCombo)
	{
		delete m_monsterCombo;
		m_monsterCombo = NULL;
	}
#endif // MONSTER_COMBO

#ifdef NIGHTSHADOW_SKILL	// 
	m_nNPC_Killed_NPC = 0;
#endif  // NIGHTSHADOW_SKILL
}

void CArea::Init(CZone* z, int index, int w, int h, int ycnt)
{
	int i, j;
	
	if(!z) 
		return;
	
	m_bEnable = false;
	m_zone = z;
	m_index = index;
	
	m_size[0] = (int)(w / CELL_SIZE);
	m_size[1] = (int)(h / CELL_SIZE);
	m_ycnt = ycnt;
	
	m_env = new CEnv**[m_ycnt];
	for (i = 0; i < m_ycnt; i++)
	{
		m_env[i] = new CEnv*[m_size[0]];
		for (j = 0; j < m_size[0]; j++)
			m_env[i][j] = new CEnv[m_size[1]];
	}
	
	// 싱글던전의 경우 한 Area에 Cell 하나
#ifdef MONSTER_COMBO
	if (z->IsGuildRoom() || z->IsPersonalDungeon() || z->IsComboZone() || z->IsExtremeCube())
#else
	if (z->IsGuildRoom() || z->IsPersonalDungeon())
#endif // MONSTER_COMBO
	{
		m_size[0] = 1;
		m_size[1] = 1;

		m_cell = new CCell*[m_size[0]];
		m_cell[0] = new CCell[m_size[1]];
	}
	else
	{
		m_cell = new CCell*[m_size[0]];
		for (i = 0; i < m_size[0]; i++)
			m_cell[i] = new CCell[m_size[1]];
	}

#ifdef MONSTER_COMBO
	if(z->IsComboZone())
	{
		m_monsterCombo = new CMonsterCombo;
		m_monsterCombo->SetArea(this);
	}
#endif // MONSTER_COMBO
}

void CArea::PointToCellNum(float x, float z, int* cx, int* cz)
{
	*cx = (int)(x / CELL_SIZE);
	*cz = (int)(z / CELL_SIZE);
	
	if (*cx < 0)			*cx = 0;
	if (*cx >= m_size[0])	*cx = m_size[0] - 1;
	if (*cz < 0)			*cz = 0;
	if (*cz >= m_size[1])	*cz = m_size[1] - 1;
}

void CArea::CharToCell(CCharacter* ch, char cy, int cx, int cz)
{
	if (IS_IN_CELL(ch))
		return ;
	
	//if (cy < 0 || cy >= m_ycnt)
	// yLayer 수정
	if (cy < 0 || cy > ((m_ycnt - 1) * 2))
		return ;
	
	m_cell[cx][cz].CharToCell(ch);
	
	ch->m_pZone = m_zone;
	ch->m_pArea = this;
	ch->m_cellX = cx;
	ch->m_cellZ = cz;
	GET_YLAYER(ch) = cy;
}

void CArea::CharFromCell(CCharacter* ch, bool bDeleteAttackList)
{
	if (!IS_IN_CELL(ch))
		return ;

	if (ch->m_pArea != this)
		return ;
	
	m_cell[ch->m_cellX][ch->m_cellZ].CharFromCell(ch);
	
	ch->m_cellX = NO_CELL;
	ch->m_cellZ = NO_CELL;

	if (bDeleteAttackList)
		DelAttackList(ch);
}

void CArea::ItemToCell(CItem* item, char cy, int cx, int cz)
{
	//if (cy < 0 || cy >= m_ycnt)
	// yLayer 수정
	if (cy < 0 || cy > ((m_ycnt - 1) * 2))
		return ;
	
	m_cell[cx][cz].ItemToCell(item);
	
	item->m_cellX =cx;
	item->m_cellZ = cz;
	GET_YLAYER(item) = cy;
}

void CArea::ItemFromCell(CItem* item)
{
	if (!IS_IN_CELL(item))
		return ;
	
	m_cell[item->m_cellX][item->m_cellZ].ItemFromCell(item);
	
	item->m_cellX = NO_CELL;
	item->m_cellZ = NO_CELL;
}

void CArea::SendCellInfo(CPC* ch, bool bExcludeOwnPetElemental)
{
	if (!IS_IN_CELL(ch))
		return ;
	
	int sx = ch->m_cellX - CELL_EXT;
	int ex = ch->m_cellX + CELL_EXT;
	int sz = ch->m_cellZ - CELL_EXT;
	int ez = ch->m_cellZ + CELL_EXT;
	
	int x, z;
	CNetMsg msg;
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= m_size[0])
			continue;
		
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= m_size[1])
				continue;
			
			CCharacter* pChar;
			CCharacter* pNext = m_cell[x][z].m_listChar;
			
			while ((pChar = pNext))
			{
				pNext = pChar->m_pCellNext;
				// 원래는 pc가 아니면 죽은 것은 Appear 안되나 무조건 되도록 수정 : bs : 050806
				if (pChar != ch)
				{
					if (bExcludeOwnPetElemental && IS_ELEMENTAL(pChar) && TO_ELEMENTAL(pChar)->GetOwner() == ch)
						continue ;
#ifdef ENABLE_PET
					if (bExcludeOwnPetElemental && IS_PET(pChar) && TO_PET(pChar)->GetOwner() == ch)
						continue ;
#endif // ENABLE_PET
					AppearMsg(msg, pChar);
					SEND_Q(msg, ch->m_desc);
#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE		
					CNetMsg GuildPointMsg;
					CPC* pc = TO_PC(pChar);
					if( pc )
						if( pc->m_guildInfo && pc->m_guildInfo->guild() )
						{
							GuildPointRankingMsg( GuildPointMsg, pc->m_index, pc->m_guildInfo->guild()->index(), pc->m_guildInfo->guild()->GetGuildPointRanking() );
							SEND_Q(GuildPointMsg, ch->m_desc);
						}
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD

#ifdef DYNAMIC_DUNGEON
					if( TO_PC(pChar) )
					{
						CNetMsg NameColorMsg;
						GuildNameColorStateMsg(NameColorMsg, TO_PC(pChar) ); 
						SEND_Q(NameColorMsg, ch->m_desc);
					}
#endif //DYNAMIC_DUNGEON

				}
			}
			
			CItem* pItem;
			CItem* pItemNext = m_cell[x][z].m_listItem;
			while ((pItem = pItemNext))
			{
				pItemNext = pItem->m_pCellNext;
				// 나타남
				ItemAppearMsg(msg, pItem);
				SEND_Q(msg, ch->m_desc);
			} // end while
		}
	}
}

bool CArea::IsNotBlocked(CCharacter* ch, CCharacter* tch, bool battack, char& outYlayer)
{
	return IsNotBlocked(ch, GET_YLAYER(tch), GET_X(tch), GET_Z(tch), battack, outYlayer);
}

bool CArea::IsNotBlocked(char sy, float sx, float sz, float tx, float tz, bool bCastleNPC, char& ty)
{
	ty = sy;

	// 영역 검사
	if (tx < 0 || tx >= m_zone->m_attrMap[0].m_size[0] || tz < 0 || tz >= m_zone->m_attrMap[0].m_size[1])
		return false;

	// 목표 지점의 속성
	unsigned char newatt;
	// 현재 지점의 속성
	unsigned char curattr;
	// 계단 검사 루틴에 들어갔었는지 검사
	bool bTestStair = false;

	char maxY = (m_zone->m_countY - 1) * 2;

RETRY:
	// 층 유효성 검사
	if (sy < 0 || sy > maxY)
		return false;

	// 목표 지점의 속성
	newatt = GetAttr(sy, tx, tz);
	// 현재 지점의 속성
	curattr = GetAttr(sy, sx, sz);

	// 목표 지점이 생산지역, 피스존이면 블럭과 같게 처리
	switch (newatt)
	{
	case MAPATT_PEACEZONE:
	case MAPATT_PRODUCT_PUBLIC:
	case MAPATT_PRODUCT_PRIVATE:
		newatt = MAPATT_BLOCK;
		break;
	}
#ifdef ENABLE_WAR_CASTLE
	// 공성 수호병이면 MAPATT_WARZONE이 아닌곳은 블럭
	if (bCastleNPC && newatt != MAPATT_WARZONE)
		newatt = MAPATT_BLOCK;
#endif

	// 블럭을 당하면
	if (newatt == MAPATT_BLOCK)
	{
		// 이미 계단을 적용했으면 블럭
		if (bTestStair)
			return false;

		// 현재 계단이면 윗층 검사
		if (sy % 2 == 1)
		{
			bTestStair = true;
			sy++;
			goto RETRY;
		}
		else
			return false;
	}

	// 검사는 정상 완료가 되었으므로 최종 y값을 구한다
	// 이동한 영역이 상향 계단
	if (newatt == MAPATT_STAIR_UP)
	{
		// 일반영역에서 간 경우
		if (ty % 2 == 0)
			ty++;
		// 계단이었으면
		else
			ty = sy - sy % 2 + 1;
	}
	// 이동한 영역이 하향 계단
	else if (newatt == MAPATT_STAIR_DOWN)
	{
		// 일반영역에서 간 경우
		if (ty % 2 == 0)
			ty--;
		// 계단이었으면
		else
			ty = sy - sy % 2 - 1;
	}
	// 그외의 경우는 계단이 아닌 층번호
	else
		ty = sy - sy % 2;

	return true;
}

bool CArea::IsNotBlocked(CCharacter* ch, char y, float x, float z, bool battack, char& outYlayer)
{
	outYlayer = y;

	// 영역 검사
	if (x < 0 || x >= m_zone->m_attrMap[y/2].m_size[0] || z < 0 || z >= m_zone->m_attrMap[y/2].m_size[1])
	{
		outYlayer = GET_YLAYER(ch);
		return false;
	}
	
	// 몹이 아니면 클라이언트를 믿고
	if (!IS_NPC(ch) && !battack)
		return true;

	bool bCastleNPC = false;
#ifdef ENABLE_WAR_CASTLE
	if (IS_NPC(ch))
	{
		CNPC* npc = TO_NPC(ch);
		bCastleNPC = (npc->m_proto->CheckFlag(NPC_CASTLE_GUARD | NPC_CASTLE_TOWER)) ? true  : false;
	}
#endif
	
	// TODO : DELETE 완전히 층이 다른 경우 무조건 false : ylayer 차이가 2이상
//	if (ABS(GET_YLAYER(ch) - y) > 1)
//	{
//		outYlayer = GET_YLAYER(ch);
//		return false;
//	}

	// 공격일때는 내가 있는 자리는 검사 안한다
	int inc_start = 0;
	if (battack && !IS_PC(ch))
		inc_start = 1;
	
	float x_diff = x - GET_X(ch);
	float z_diff = z - GET_Z(ch);
	float x_diff2 = ABS(x_diff);
	float z_diff2 = ABS(z_diff);
	
	if (x_diff2 < EPSILON && z_diff2 < EPSILON)
		return IsNotBlocked(GET_YLAYER(ch), GET_X(ch), GET_Z(ch), x, z, bCastleNPC, outYlayer);

	int i;
	float x_inc;
	float z_inc;

	float sx, sz, tx, tz;
	char sy, ty;

	sx = GET_X(ch);
	sz = GET_Z(ch);
	sy = GET_YLAYER(ch);
	ty = sy;
	
	if (x_diff2 > z_diff2)
	{
		x_inc = (x_diff < 0) ? -1.0f : 1.0f;
		z_inc = ((z_diff < 0) ? -1.0f : 1.0f) * z_diff2 / x_diff2;
		
		for (i = inc_start; (x_diff > 0 && (int)(GET_X(ch) + (x_inc * i)) <= (int)x) || (x_diff < 0 && (int)(GET_X(ch) + (x_inc * i)) >= (int)x); i++)
		{
			tx = GET_X(ch) + (x_inc * i);
			tz = GET_Z(ch) + (z_inc * i);
			if (!IsNotBlocked(sy, sx, sz, tx, tz, bCastleNPC, ty))
				return false;
			sx = tx;
			sz = tz;
			sy = ty;
		} // end for (i = inc_start; (x_diff > 0 && GET_X(ch) + (x_inc * i) < x) || (x_diff < 0 && GET_X(ch) + (x_inc * i) > x); i++)
	} // end if
	
	else
	{
		x_inc = ((x_diff < 0) ? -1.0f : 1.0f) * x_diff2 / z_diff2;
		z_inc = (z_diff < 0) ? -1.0f : 1.0f;
		
		for (i = inc_start; (z_diff > 0 && (int)(GET_Z(ch) + (z_inc * i)) <= (int)z) || (z_diff < 0 && (int)(GET_Z(ch) + (z_inc * i)) >= (int)z); i++)
		{
			tx = GET_X(ch) + (x_inc * i);
			tz = GET_Z(ch) + (z_inc * i);
			if (!IsNotBlocked(sy, sx, sz, tx, tz, bCastleNPC, ty))
				return false;
			sx = tx;
			sz = tz;
			sy = ty;
		} // end for (i = inc_start; (z_diff > 0 && GET_Z(ch) + (z_inc * i) < z) || (z_diff < 0 && GET_Z(ch) + (z_inc * i) > z); i++)
	} // end else

	outYlayer = ty;
	
	return IsNotBlocked(ty, x, z, x, z, bCastleNPC, sy);
}

unsigned char CArea::GetAttr(char y, float x, float z)
{
	int x_i = (int) x;
	int z_i = (int) z;
	
	y /= 2;

	if (y < 0 || y >= m_zone->m_countY)
		return MAPATT_BLOCK;
	
	if (x_i < 0 || x_i >= m_zone->m_attrMap[y].m_size[0] || z_i < 0 || z_i >= m_zone->m_attrMap[y].m_size[1])
		return MAPATT_BLOCK; 

	return m_zone->m_attrMap[y].m_attr[x_i][z_i];
}

float CArea::GetHeight(char y, float x, float z)
{
	int x_i = (int) x;
	int z_i = (int) z;

	int ty = y / 2;

	if (ty < 0 || ty >= m_zone->m_countY)
		return 0.0f;
	
	if (x_i < 0 || x_i >= m_zone->m_attrMap[ty].m_size[0] || z_i < 0 || z_i >= m_zone->m_attrMap[ty].m_size[1])
		return 0.0f; 

	if (y % 2 != 0 && GetAttr(y, x, z) == MAPATT_BLOCK)
		return GetHeight(y + 1, x, z);
	else
	{
		return m_zone->m_attrMap[ty].m_height[x_i][z_i];
	}
}

void CArea::LoadNPC()
{
	// area가 속한 해당 존 넘버의 리젠정보 READ
	CDBCmd dbNpcRegen;
	dbNpcRegen.Init(&gserver.m_dbdata);
	strncpy(g_buf, "SELECT * FROM t_npc_regen WHERE a_zone_num=", 44);
	IntCat(g_buf, m_zone->m_index, false);
	StrCat(g_buf, "ORDER BY a_index");
	dbNpcRegen.SetQuery(g_buf);
	dbNpcRegen.Open();
	
	m_nCountNPC = dbNpcRegen.m_nrecords;
	
	int npcIdx, yLayer, regenSec, totalNum, index;
	float x, z, r;

	int nRealCountNPC = 0;

	int nNonPeachfulNPCIndex;
	nNonPeachfulNPCIndex = 0;

#ifdef EVENT_HALLOWEEN_2006
	if( m_zone->m_index == ZONE_START || m_zone->m_index == ZONE_DRATAN || m_zone->m_index == ZONE_MERAC )
#if defined( LC_KOR ) || defined( BWTEST )
		m_nCountNPC += 15;
#else
		m_nCountNPC += 5;
#endif	// LC_KOR

#endif	// EVENT_HALLOWEEN_2006
		
#ifdef DRATAN_CASTLE
	if(m_zone->m_index == ZONE_DRATAN)
	{	///=== 이부분 변경시 밑의 nRealCountNPC 로 변경 부분도 수정이 필요
		///=== 설계상의 잘못 리펙토링 필요!!!
		m_nCountNPC += 30;
	}	
#endif // DRATAN_CASTLE

#ifdef ADULT_SERVER_EXTRA
	int nCityNpcCount = 0;
	m_cityNpcRegenList.SetCount( 100 );
#endif ADULT_SERVER_EXTRA
	m_npcRegenList.SetCount( m_nCountNPC );

	while (dbNpcRegen.MoveNext())
	{
		dbNpcRegen.GetRec("a_index",		index);
		dbNpcRegen.GetRec("a_npc_idx",		npcIdx);
		dbNpcRegen.GetRec("a_pos_x",		x);
		dbNpcRegen.GetRec("a_y_layer",		yLayer);
		dbNpcRegen.GetRec("a_pos_z",		z);
		dbNpcRegen.GetRec("a_pos_r",		r);
		dbNpcRegen.GetRec("a_regen_sec",	regenSec);
		dbNpcRegen.GetRec("a_total_num",	totalNum);

		if( (gserver.m_subno != WAR_CASTLE_SUBNUMBER_MERAC) && (npcIdx==468) )
		{
			totalNum = -1;
		}

		m_npcRegenList.m_type = 0;
#ifdef ENABLE_FAST_REGEN
		if (gserver.m_serverno == ENABLE_FAST_REGEN && m_zone->m_index == ZONE_START && regenSec == 300)
		{
			regenSec = 100;
		}
#endif // ENABLE_FAST_REGEN

#ifdef EVENT_HALLOWEEN_2006 
		if (nNonPeachfulNPCIndex == 0)
		{
			CNPCProto* pNPCProto = gserver.m_npcProtoList.FindProto(npcIdx);
			if (pNPCProto && !pNPCProto->CheckFlag(NPC_PEACEFUL))
				nNonPeachfulNPCIndex = m_npcRegenList.m_wPos;
		}
#endif // EVENT_HALLOWEEN_2006

		const CNPCProto* pNPCProto = gserver.m_npcProtoList.FindProto(npcIdx);
		if ( pNPCProto == NULL )
			continue;

		if (pNPCProto && pNPCProto->CheckFlag(NPC_RAID))
		{
#ifdef MONSTER_RAID_SYSTEM
#if !defined (LC_JPN) && !defined (LC_TLD) && !defined (LC_BRZ) && !defined (LC_GER)  && !defined(LC_KOR)
			if ( m_zone->m_index != ZONE_START
#ifdef EVENT_HALLOWEEN_2007
				|| pNPCProto->m_index == 454
#endif //EVENT_HALLOWEEN_2007
				)
			{
#endif // !defined (LC_JPN) && !defined (LC_TLD)
				CNPCRegenInfo* pNewRegenInfo = new CNPCRegenInfo;
				pNewRegenInfo->m_index = index;
				pNewRegenInfo->m_npcIdx = npcIdx;
				pNewRegenInfo->m_zoneNo = m_zone->m_index;
				pNewRegenInfo->m_regenX = x;
				pNewRegenInfo->m_regenY = yLayer;
				pNewRegenInfo->m_regenZ = z;
				pNewRegenInfo->m_regenR = r;
				pNewRegenInfo->m_totalNum = totalNum;

				pNewRegenInfo->m_bAlive = false;
				pNewRegenInfo->m_lastDieTime = 0;
#ifdef BSTEST
				pNewRegenInfo->m_regenSec = PULSE_REAL_MIN;
#else // BSTEST
				pNewRegenInfo->m_regenSec = regenSec;
#endif // BSTEST
				m_raidNPCRegenList.AddToTail(pNewRegenInfo);
#if !defined (LC_JPN) && !defined (LC_TLD) && !defined (LC_BRZ) && !defined (LC_GER)  && !defined(LC_KOR)
			}
#endif // !defined (LC_JPN) && !defined (LC_TLD)
#endif // MONSTER_RAID_SYSTEM
		}
		else
		{
#ifdef EVENT_XMAS_2006
			if (pNPCProto->m_index == 334 || pNPCProto->m_index == 335)
			{
				
				CNPCRegenInfo* pNewRegenInfo = new CNPCRegenInfo;
				pNewRegenInfo->m_index = index;
				pNewRegenInfo->m_npcIdx = npcIdx;
				pNewRegenInfo->m_zoneNo = m_zone->m_index;
				pNewRegenInfo->m_regenX = x;
				pNewRegenInfo->m_regenY = yLayer;
				pNewRegenInfo->m_regenZ = z;
				pNewRegenInfo->m_regenR = r;
				pNewRegenInfo->m_totalNum = totalNum;

				pNewRegenInfo->m_bAlive = false;
				pNewRegenInfo->m_lastDieTime = 0;
				pNewRegenInfo->m_regenSec = 1;
				m_eventXMas2006RegenList.AddToTail(pNewRegenInfo);
				//pNPCProto = NULL;     //yhj...   상자 몬스터가 리젠이 안되서 임시로 주석처리
			}
#endif // EVENT_XMAS_2006
			if (pNPCProto)
			{
#ifdef ADULT_SERVER_EXTRA
				if ( pNPCProto->CheckFlag( NPC_CHAOCITY | NPC_HUNTERCITY ) )
				{
					m_cityNpcRegenList.AddRegenInfo(index, npcIdx, regenSec, x, yLayer, z, r, totalNum, m_zone->m_index);
					nCityNpcCount++;
				}
				else
#endif // ADULT_SERVER_EXTRA
				{
					m_npcRegenList.AddRegenInfo(index, npcIdx, regenSec, x, yLayer, z, r, totalNum, m_zone->m_index);
					nRealCountNPC++;
				}
			}
		}
	}

#ifdef EVENT_HALLOWEEN_2006
	if( m_zone->m_index == ZONE_START || m_zone->m_index == ZONE_DRATAN || m_zone->m_index == ZONE_MERAC )
	{
#if defined( LC_KOR ) 
		for ( int i = 1; i < 16 ; i++ )
#else
		for ( int i = 1; i < 6 ; i++ )
#endif
		{
			m_npcRegenList.AddRegenInfo( index+i, 314, 300,
				m_npcRegenList.m_infoList[nNonPeachfulNPCIndex].m_regenX,
				m_npcRegenList.m_infoList[nNonPeachfulNPCIndex].m_regenY,
				m_npcRegenList.m_infoList[nNonPeachfulNPCIndex].m_regenZ,
				m_npcRegenList.m_infoList[nNonPeachfulNPCIndex].m_regenR, -1, m_zone->m_index);
			nRealCountNPC++;
		}

	} // zone_Index 가 0, 4, 7, 일경우만 MPC 등록 - 쥬노, 드라탄, 메라크 지역 
#endif	// EVENT_HALLOWEEN_2006

#ifdef ADULT_SERVER_EXTRA
	m_cityNpcRegenList.m_nCount = nCityNpcCount;
#endif // ADULT_SERVER_EXTRA

	m_npcRegenList.m_nCount = nRealCountNPC;
#ifdef DRATAN_CASTLE
	if(m_zone->m_index == ZONE_DRATAN)
	{	// 드라탄이면 수호병의 최대치 만큼   
		///== SetCount()를 사용하지 않기 때문에 실제 사이즈를 늘리려면 
		///== SetCount()호출하는 곳에서도 늘려야 한다. 
		m_npcRegenList.m_nCount += 30;
	}
#endif // DRATAN_CASTLE

#ifdef NPC_REGEN_ADJUST
	int nIndexRegen;
	int nRegenCellX;
	int nRegenCellZ;
	for (nIndexRegen = 0; nIndexRegen < m_npcRegenList.m_nCount; nIndexRegen++)
	{
		PointToCellNum(m_npcRegenList.m_infoList[nIndexRegen].m_regenX, m_npcRegenList.m_infoList[nIndexRegen].m_regenZ, &nRegenCellX, &nRegenCellZ);
		m_cell[nRegenCellX][nRegenCellZ].m_nTotalRegen++;
	}
#endif // NPC_REGEN_ADJUST
}

int CArea::GetPCCount(CCharacter* ch, int findArea)
{
	return GetPCCount(ch->m_cellX, ch->m_cellZ, findArea);
}

int CArea::GetPCCount(int cx, int cz, int findArea)
{
	int i, j;
	int ret = 0;
	
	for (i = cx - findArea; i <= cx + findArea; i++)
	{
		if (i < 0 || i >= m_size[0]) continue ;
		for (j = cz - findArea; j <= cz + findArea; j++)
		{
			if (j < 0 || j >= m_size[1]) continue ;
			ret += m_cell[i][j].m_nCountPC;
		}
	}
	
	return ret;
}

CCharacter* CArea::FindChar(int idx, MSG_CHAR_TYPE tchType)
{
	CCharacter* ret;
	int i, j;
	for (i = 0; i < m_size[0]; i++)
	{
		for (j = 0; j < m_size[1]; j++)
		{
			ret = m_cell[i][j].m_listChar;
			while (ret)
			{
				if (ret->m_type == tchType && ret->m_index == idx && !DEAD(ret))
					return ret;
				ret = ret->m_pCellNext;
			}
		}
	}
	
	return NULL;
}

void CArea::SetEnable()
{
	CNetMsg appearNPCMsg;
	
	if (m_bEnable)
		return ;
	
	m_bEnable = true;
	m_pulseNPCAI = gserver.m_pulse;
	m_pulseNPCRegen = gserver.m_pulse;
	
	int i;
	int cx, cz;
	
	m_nCountNPC = 0;
	
	// 싱글던전
	m_nMakeNPC = 0;
	
	CNPC* npc;

#ifdef MONSTER_RAID_SYSTEM
	void* posRaidNPC = m_raidNPCRegenList.GetHead();
	while (posRaidNPC)
	{
		CNPCRegenInfo* p = m_raidNPCRegenList.GetData(posRaidNPC);
		if (p)
		{
			CNPCProto* pNPCProto = gserver.m_npcProtoList.FindProto(p->m_npcIdx);
			if (pNPCProto)
			{
				p->m_lastDieTime = gserver.m_pulse + GetRandom(1, p->m_regenSec / 2);
				p->m_bAlive = false;
				p = NULL;
			}
		}
		posRaidNPC = m_raidNPCRegenList.GetNext(posRaidNPC);
	}
#endif // MONSTER_RAID_SYSTEM

#ifdef EVENT_XMAS_2006
	// 상자 몬스터가 자리에서 못떠나게 하기 위해 잠시 사라지면 죽은 것으로 처리
	void* posEventNPC = m_eventXMas2006RegenList.GetHead();
	while (posEventNPC)
	{
		CNPCRegenInfo* p = m_eventXMas2006RegenList.GetData(posEventNPC);
		if (p)
		{
			CNPCProto* pNPCProto = gserver.m_npcProtoList.FindProto(p->m_npcIdx);
			if (pNPCProto)
			{
				p->m_lastDieTime = 1;
				p->m_bAlive = false;
				p = NULL;
			}
		}
		posEventNPC = m_eventXMas2006RegenList.GetNext(posEventNPC);
	}
#endif // EVENT_XMAS_2006

//#ifdef ALTERNATE_MERCHANT
//	void* posAlternateMerchantNPC = m_AlternateMerchantList.GetHead();
//	while( posAlternateMerchantNPC )
//	{
//		CNPCRegenInfo* p = m_AlternateMerchantList.GetData( posAlternateMerchantNPC );
//		if( p )
//		{
//			CNPCProto* pNPCProto = gserver.m_npcProtoList.FindProto(p->m_npcIdx );		
//			if (pNPCProto)
//			{
//				p->m_lastDieTime = 1;
//				p->m_bAlive = false;
//				p = NULL;
//			}
//		}
//		posAlternateMerchantNPC = m_AlternateMerchantList.GetNext(posAlternateMerchantNPC);
//		}
//	}
//
//#endif // ALTERNATE_MERCHANT
	
	// 싱글던전이 아닌경우에만  일반 NPC 리젠
	// TODO : 추후 싱글던전에 Mob이 아닌 NPC가 들어갈 경우 수정 요.
	if (!m_zone->IsPersonalDungeon())
	{
		for (i = 0; i < m_npcRegenList.m_nCount; i++)
		{
			CNPCRegenInfo* p = m_npcRegenList.m_infoList + i;

			if (p->m_totalNum == 0)
				continue ;

			// 층적용 몹배치 확인
			npc = gserver.m_npcProtoList.Create(p->m_npcIdx, p);
			if (!npc)	continue ;

			// 속성맵상 피스존이고 피스 NPC가 아니면 리젠안함
			if (!npc->m_proto->CheckFlag(NPC_PEACEFUL))
			{
				switch (GetAttr(p->m_regenY, p->m_regenX, p->m_regenZ))
				{
				case MAPATT_BLOCK:
				case MAPATT_PEACEZONE:
					GAMELOG << init("REGEN ERROR")
							<< "NPC" << delim
							<< npc->m_name << delim
							<< p->m_npcIdx << delim
							<< "ZONE" << delim
							<< m_zone->m_index << delim
							<< "YLAYER" << delim
							<< p->m_regenY << delim
							<< "X" << delim
							<< p->m_regenX << delim
							<< "Z" << delim
							<< p->m_regenZ
							<< end;
					delete npc;
					continue ;
				}
			}
#ifdef ADULT_SERVER_EXTRA
			if( npc->m_proto->CheckFlag(NPC_CHAOCITY|NPC_HUNTERCITY) )
			{
				// 마을 속성이 있는 NPC는 리젠 보류, 속성과 함께 리젠
				delete npc;
				continue;				
			}
#endif // ADULT_SERVER_EXTRA
			
			GET_X(npc) = p->m_regenX;
			GET_YLAYER(npc) = p->m_regenY;
			GET_Z(npc) = p->m_regenZ;

			// 어택 가능한 NPC
			if (npc->m_proto->CheckFlag(NPC_ATTACK) && npc->m_proto->m_index != 339)
				GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			else
				GET_R(npc) = p->m_regenR;
			
			npc->m_regenX = GET_X(npc);
			npc->m_regenY = GET_YLAYER(npc);
			npc->m_regenZ = GET_Z(npc);
			
			p->m_bAlive = true;
			p->m_numRegen++;
			//p->m_lastDieTime = 0;

#if defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)
			if( npc->m_proto->m_index == 342 )
				npc->m_hp = 1;
#endif // defined (SAKURA_EVENT_2008) || defined (SAKURA_EVENT_2008_REWARD)

#ifdef EVENT_HALLOWEEN_2006
			if( npc->m_proto->m_index == 314 )
			{
				p->m_lastDieTime = gserver.m_pulse - p->m_regenSec;
				continue;
			}
			else
			{
				p->m_lastDieTime = 0;
			}
#else 
			p->m_lastDieTime = 0;
#endif	// EVENT_HALLOWEEN_2006

#ifdef DRATAN_CASTLE
#ifdef DYNAMIC_DUNGEON
			CDratanCastle * pCastle = CDratanCastle::CreateInstance();
			if(m_zone->m_index == ZONE_DRATAN_CASTLE_DUNGEON 
				&& pCastle != 0 && !npc->m_proto->CheckFlag(NPC_ZONEMOVER))
			{
				int nMobRate[10][4] = {	// 공격력, 방어력, 최대체력, 시야범위
					{20, 20, 30, 0},
					{10, 10, 15, 0},
					{5, 5, 10, 0},
					{0, 0, 0, 0},
					{-5, -5, -5, 0},
					{-7, -7, -7, 0},
					{-10, -10, -10, 0},
					{-15, -15, -15, 0},
					{-20, -20, -20, 0},
					{-25, -25, -25, -80},
				};

				int midx = pCastle->m_dvd.GetMobRate()/10;
				if(midx > 9)
				{
					midx = 9;
				}

				// 공격력 적용
				if(npc->m_attackType == MSG_DAMAGE_MELEE)
				{
					npc->m_eqMelee = npc->m_proto->m_attack + npc->m_proto->m_attack * nMobRate[midx][0] / 100;
					if(npc->m_eqMelee < 0)
					{
						npc->m_eqMelee = 1;
					}
				}
				else if(npc->m_attackType == MSG_DAMAGE_RANGE)
				{
					npc->m_eqRange = npc->m_proto->m_attack + npc->m_proto->m_attack * nMobRate[midx][0] / 100;
					if(npc->m_eqRange < 0)
					{
						npc->m_eqRange = 1;
					}
				}
				else if(npc->m_attackType == MSG_DAMAGE_MAGIC)
				{
					npc->m_eqMagic = npc->m_proto->m_magic + npc->m_proto->m_magic * nMobRate[midx][0] / 100;
					if(npc->m_eqMagic < 0)
					{
						npc->m_eqMagic = 1;
					}
				}			

				// 방어력 적용
				npc->m_eqDefense = npc->m_proto->m_defense + npc->m_proto->m_defense * nMobRate[midx][1] / 100;

				// 최대 체력 적용
				npc->m_maxHP = npc->m_proto->m_hp + npc->m_proto->m_hp * nMobRate[midx][2] / 100;
				if(npc->m_maxHP < 0)
				{
					npc->m_maxHP = 1;
				}
				npc->m_hp = npc->m_maxHP;

				// 시야 거리 적용
				if(nMobRate[midx][3] != 0 && npc->m_attackRange != 0)
				{
					npc->m_attackRange = npc->m_proto->m_attackArea + npc->m_proto->m_attackArea * nMobRate[midx][3] / 100;
				}
			}
#endif // DYNAMIC_DUNGEON
#endif // DRATAN_CASTLE

			AddNPC(npc);
			PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			CharToCell(npc, GET_YLAYER(npc), cx, cz);
			
			AppearMsg(appearNPCMsg, npc, true);
			SendToCell(appearNPCMsg, GET_YLAYER(npc), cx, cz);
		}
	}
	// 싱글 던전일 경우 리젠리스트세팅
	else
	{
		m_regenList = new int[m_npcRegenList.m_nCount];

		for(i = 0; i < m_npcRegenList.m_nCount; i++)
		{
			m_regenList[i] = m_npcRegenList.m_infoList[i].m_index;
		}
	}
	
	// 상점 리젠
	for (i=0; i < m_zone->m_nShopCount; i++)
	{
		if (GetAttr(m_zone->m_shopList[i].m_yLayer, m_zone->m_shopList[i].m_x, m_zone->m_shopList[i].m_z) == MAPATT_WARZONE)
			continue ;
		npc = gserver.m_npcProtoList.Create(m_zone->m_shopList[i].m_keeperIdx, NULL);
		if (!npc)	continue;
		
		GET_R(npc) = m_zone->m_shopList[i].m_r;
		npc->m_regenX = GET_X(npc) = m_zone->m_shopList[i].m_x;
		npc->m_regenZ = GET_Z(npc) = m_zone->m_shopList[i].m_z;
		npc->m_regenY = GET_YLAYER(npc) = m_zone->m_shopList[i].m_yLayer;
		
		AddNPC(npc);
		PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
		CharToCell(npc, GET_YLAYER(npc), cx, cz);
		
		AppearMsg(appearNPCMsg, npc, true);
		SendToCell(appearNPCMsg, GET_YLAYER(npc), cx, cz);
	}
}

void CArea::SetDisable()
{
	if (!m_bEnable)
		return ;
	
	m_bEnable = false;
	
	// cell
	int i, j;
	for (i = 0; i < m_size[0]; i++)
	{
		for (j = 0; j < m_size[1]; j++)
		{
			// 캐릭터 빼기
			while (m_cell[i][j].m_listChar)
				CharFromCell(m_cell[i][j].m_listChar, true);
			
			
			// 아이템 빼기 
			CItem* item;
			CItem* nItem;
			
			nItem = m_cell[i][j].m_listItem;
			
			while ((item = nItem))
			{
				nItem = nItem->m_pCellNext;
				
				// 셀 리스트에서 빼고 삭제
				ItemFromCell(item);
				item->m_pArea = NULL;
				delete item;
				item = NULL;
			}
		}
	}
	
	// TODO : ENV
	
	// npc
	CNPC* npc;
	while (m_npcList)
	{
		npc = m_npcList->m_pNPCNext;
		m_npcList->DeleteNPC();
		
		m_npcList = NULL;
		m_npcList = npc;
	}
	
	// npc regen
	for (i = 0; i < m_npcRegenList.m_nCount; i++)
	{
		m_npcRegenList.m_infoList[i].m_bAlive = false;
		m_npcRegenList.m_infoList[i].m_lastDieTime = 0;
		m_npcRegenList.m_infoList[i].m_numRegen = 0;
	}
	
	// 싱글 던전
	m_nMakeNPC = 0;
	m_bBoss = false;
	if( m_regenList)
		delete m_regenList;
}

CItem* CArea::DropItem(CItem* item, CCharacter* ch)
{
	if (item == NULL)
		return NULL;

	// 싱글던전 itemdrop
	if (m_zone->IsGuildRoom() || m_zone->IsPersonalDungeon())
	{
		item->m_pos = CPos(GET_X(ch), GET_Z(ch), ch->m_pos.m_h, GET_R(ch), GET_YLAYER(ch));
	}
	else
	{
		item->m_pos = CPos(GET_X(ch) + (((GetRandom(0, 1) == 0) ? -1.0f : 1.0f) * (ch->GetSize() + GetRandom(0, 100) * 0.01)),
			GET_Z(ch) + (((GetRandom(0, 1) == 0) ? -1.0f : 1.0f) * (ch->GetSize() + GetRandom(0, 100) * 0.01)),
			GET_H(ch),
			GET_R(ch),
			GET_YLAYER(ch));
	}
	
	// 좌표 보정
	if (GET_X(item) < 0)		GET_X(item) = 0;
	if (GET_Z(item) < 0)		GET_Z(item) = 0;
	// 050207 - bs : 층에 관계없이 사이즈는 0번 층에서 참조
	if (GET_X(item) >= m_zone->m_attrMap[0].m_size[0])
		GET_X(item) = m_zone->m_attrMap[0].m_size[0];
	if (GET_Z(item) >= m_zone->m_attrMap[0].m_size[1])
		GET_Z(item) = m_zone->m_attrMap[0].m_size[1];
	// 050207 - bs : 층에 관계없이 사이즈는 0번 층에서 참조
	
	if (GetAttr(GET_YLAYER(item), GET_X(item), GET_Z(item)) == MAPATT_BLOCK)
		item->m_pos = CPos(GET_X(ch), GET_Z(ch), GET_H(ch), GET_R(ch), GET_YLAYER(ch));
	
	// 셀에 넣기
	item->m_pArea = this;
	item->m_groundPulse = gserver.m_pulse;
	
	int cx, cz;
	PointToCellNum(GET_X(item), GET_Z(item), &cx, &cz);
	ItemToCell(item, GET_YLAYER(item), cx, cz);
	
	return item;
}

CItem* CArea::DropItem(int item_db_idx, CCharacter* ch, int plus, int flag, LONGLONG count, bool bOption)
{
	CItem* dropitem = gserver.m_itemProtoList.CreateItem(item_db_idx, WEARING_NONE, plus, flag, count);
	if (!dropitem)
		return NULL;
	
	// 싱글던전 itemdrop
	if (m_zone->IsGuildRoom() || m_zone->IsPersonalDungeon())
	{
		dropitem->m_pos = CPos(GET_X(ch), GET_Z(ch), ch->m_pos.m_h, GET_R(ch), GET_YLAYER(ch));
	}
	else
	{
		dropitem->m_pos = CPos(GET_X(ch) + (((GetRandom(0, 1) == 0) ? -1.0f : 1.0f) * (ch->GetSize() + GetRandom(0, 100) * 0.01)),
			GET_Z(ch) + (((GetRandom(0, 1) == 0) ? -1.0f : 1.0f) * (ch->GetSize() + GetRandom(0, 100) * 0.01)),
			GET_H(ch),
			GET_R(ch),
			GET_YLAYER(ch));
	}

#ifdef MONSTER_RAID_SYSTEM
	// 레어 아이템은 옵션 없음
	if (dropitem->IsRareItem())
		bOption = false;
#endif // MONSTER_RAID_SYSTEM

#ifdef SET_ITEM
	if (dropitem->IsOriginItem())
		bOption = false;
#endif

	// 악세사리 일때 Option Setting
	if (bOption && dropitem->IsAccessary() && dropitem->m_nOption < 1)
	{
		// 060227 : bs : 하드코딩 악세서리 검사, 옵션 없이
		bool bSkipAddOption = false;
		switch (dropitem->m_idNum)
		{
		case 1301:		// 하드코딩 악세서리
		case 1302:
		case 1303:
		case 1304:
		case 1305:
		case 1306:
			bSkipAddOption = true;
			break;

		default:
			break;
		}

		if (!bSkipAddOption)
		{
			// 순서대로 5,4,3,1개 붙을 확률
			static const int factor[] = {10, 40, 80, 100};
			static const int limit[] = {500, 2000, 4000, 5000};

			int prob[MAX_ACCESSORY_OPTION-1];
			int i, j;

			for (i=0; i < MAX_ACCESSORY_OPTION - 1; i++)
			{
				if ((prob[i] = ch->m_level * factor[i]) > limit[0])
					prob[i] = limit[i];
			}

			// 옵션 수 결정
			int result = GetRandom(1, 10000);
			int num = 2;	// default 2개
			int* type;
			
			if (result <= prob[0])
				num = 5;
			else if (result <= prob[1])
				num = 4;
			else if (result <= prob[2])
				num = 3;
			else if (result <= prob[3])
				num = 1;
			else 
				num = 2;

			type = new int[num];

			LONGLONG bitfield = 0;		// TODO : 옵션이 64개 넘으면 고쳐야 함!
			const int maxnumoption = 24;	// TODO : 나중에 MAX_NUM_OPTION으로 대체

			// 옵션 타입 결정
			i = 0;
			while (i < num)
			{
				// type[i] 결정
				type[i] = GetRandom(0, maxnumoption - 1);

				// type[i]가 중복인 검사
				// 중복이면 type[i]를 1씩 증가하면서 중복 검사
				for (j = 0; j < maxnumoption; j++)
				{
					if ((bitfield & ((LONGLONG)1 << ((type[i] + j) % maxnumoption))) == 0)
						break;
				}

				// 더이상 옵션을 붙일 수 없음
				if (j == maxnumoption)
					num = i;
				else
					type[i] = (type[i] + j) % maxnumoption;

				bitfield |= ((LONGLONG)1 << type[i]);

				// 옵션번호로 붙을 수 있는 옵션인지 검사
				COptionProto* proto = gserver.m_optionProtoList.FindProto(type[i]);
				if (!proto)
					continue;

				// 악세사리중에서 붙을 수 없으면 다시
				if (!((1 << dropitem->m_itemProto->m_subtypeIdx) & proto->m_accessoryType))
					continue ;

				i++;
			}

			// 옵션 부여
			for (i=0; i < num; i++)
			{
				COptionProto* proto = gserver.m_optionProtoList.FindProto(type[i]);
				
				if (!proto)
					continue;
				
				dropitem->m_nOption++;
				// Level, Value 셋팅
				dropitem->m_option[i].MakeOptionValue(proto, ch->m_level, num);
			}

			delete [] type;
		}
	}
	
	// 좌표 보정
	if (GET_X(dropitem) < 0)		GET_X(dropitem) = 0;
	if (GET_Z(dropitem) < 0)		GET_Z(dropitem) = 0;
	// 050207 - bs : 층에 관계없이 사이즈는 0번 층에서 참조
	if (GET_X(dropitem) >= m_zone->m_attrMap[0].m_size[0])
		GET_X(dropitem) = m_zone->m_attrMap[0].m_size[0];
	if (GET_Z(dropitem) >= m_zone->m_attrMap[0].m_size[1])
		GET_Z(dropitem) = m_zone->m_attrMap[0].m_size[1];
	// 050207 - bs : 층에 관계없이 사이즈는 0번 층에서 참조
	
	if (GetAttr(GET_YLAYER(dropitem), GET_X(dropitem), GET_Z(dropitem)) == MAPATT_BLOCK)
		dropitem->m_pos = CPos(GET_X(ch), GET_Z(ch), GET_H(ch), GET_R(ch), GET_YLAYER(ch));
	
	// 셀에 넣기
	dropitem->m_pArea = this;
	dropitem->m_groundPulse = gserver.m_pulse;
	
	int cx, cz;
	PointToCellNum(GET_X(dropitem), GET_Z(dropitem), &cx, &cz);
	ItemToCell(dropitem, GET_YLAYER(dropitem), cx, cz);
	
	return dropitem;
}

void CArea::GroundItemControl()
{
	int cx, cz;
	
	CItem* item;
	CItem* nItem;
	CNetMsg delMsg;
	
	for (cx=0; cx < m_size[0]; cx++)
	{
		for (cz=0; cz < m_size[1]; cz++)
		{
			nItem = this->m_cell[cx][cz].m_listItem;
			
			while ((item = nItem))
			{
				nItem = nItem->m_pCellNext;
#ifdef EVENT_EGGS_HUNT_2007
				// EGG 아이템 운영자가 떨어뜨린 것은 사라지지 않는다.
				if( item->m_idNum == 2148 && item->m_used == 1)
					continue;
#endif // EVENT_EGGS_HUNT_2007
				
#ifdef LC_TWN2
				// 대만은 생산도구가 딸에 떨어지자마자 사라진다
				if ( (gserver.m_pulse - item->m_groundPulse >= GROUND_ITEM_PULSE) 
					|| item->m_itemProto->m_index == 193
					|| item->m_itemProto->m_index == 256
					|| item->m_itemProto->m_index == 252
					|| item->m_itemProto->m_index == 253
					|| item->m_itemProto->m_index == 254
					|| item->m_itemProto->m_index == 194
					|| item->m_itemProto->m_index == 195
					|| item->m_itemProto->m_index == 258
					|| item->m_itemProto->m_index == 259
					|| item->m_itemProto->m_index == 260
					|| item->m_itemProto->m_index == 261
					|| item->m_itemProto->m_index == 196
					|| item->m_itemProto->m_index == 263
					|| item->m_itemProto->m_index == 264
					|| item->m_itemProto->m_index == 265 )
#else
				if (gserver.m_pulse - item->m_groundPulse >= GROUND_ITEM_PULSE)	// 5 minutes
#endif
				{
					// 없애는 Msg
					ItemDisappearMsg(delMsg, item);
					SendToCell(delMsg, GET_YLAYER(item), cx, cz);
					
					// ITEM_JUNK Log
					GAMELOG << init("ITEM_JUNK")
							<< itemlog(item)
							<< end;
					
					// 셀 리스트에서 빼고 삭제
					ItemFromCell(item);
					item->m_pArea = NULL;
					delete item;
					item = NULL;
					
					this->m_nJunkItems++;
				}
				
				// 우선권 해제
				else if (gserver.m_pulse - item->m_groundPulse >= ITEM_PREPERENCE_PULSE)
					item->m_preferenceIndex = -1;
			}
		}
	}
}

void CArea::SendToAllClient(CNetMsg& msg)
{
	int cx, cz;
	
	for (cx = 0 ; cx < m_size[0]; cx++)
	{
		for (cz = 0 ; cz < m_size[1]; cz++)
		{
			CCharacter* pChar = m_cell[cx][cz].m_listChar;
			while(pChar)
			{
				if (IS_PC(pChar))
					SEND_Q(msg, TO_PC(pChar)->m_desc);
				pChar = pChar->m_pCellNext;
			}
		}
	}
}

void CArea::MoveAllChar(int nIndex, int nExtra)
{
	int cx, cz;
	CZone * pZone = 0;

	int i = gserver.FindZone(nIndex);
	if (i == -1)
	{
		return;
	}

	pZone = gserver.m_zones + i;
	if (pZone->m_bRemote == true)
	{
		return;
	}

	if (nExtra < 0 || nExtra >= pZone->m_countZonePos)
	{
		return;
	}

	for (cx = 0 ; cx < m_size[0]; cx++)
	{
		for (cz = 0 ; cz < m_size[1]; cz++)
		{
			CCharacter* pChar = m_cell[cx][cz].m_listChar;
			while(pChar)
			{
				if (IS_PC(pChar))
				{
					GoZone(TO_PC(pChar), nIndex,
						pZone->m_zonePos[nExtra][0],														// ylayer
						GetRandom(pZone->m_zonePos[nExtra][1], pZone->m_zonePos[nExtra][3]) / 2.0f,			// x
						GetRandom(pZone->m_zonePos[nExtra][2], pZone->m_zonePos[nExtra][4]) / 2.0f);		// z

					pChar = pChar->m_pCellNext;
				}
				else
				{
					pChar = pChar->m_pCellNext;
				}	
			}
		}
	}
}

CNPC* CArea::FindNearestFamily(CNPC* npc, int aitype, int hpParam)
{
	if (!IS_IN_CELL(npc))
		return NULL;

	if (npc->m_proto->m_family == -1)
		return NULL;
	
	int sx = npc->m_cellX - CELL_EXT;
	int ex = npc->m_cellX + CELL_EXT;
	int sz = npc->m_cellZ - CELL_EXT;
	int ez = npc->m_cellZ + CELL_EXT;
	
	int x, z;

	float dist = 999.9f;
	CNPC* ret = NULL;
	
	// 일단 다른 층은 검사 안한다
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= m_size[0]) continue;
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= m_size[1]) continue;
			CCharacter *pChar;
			for (pChar = m_cell[x][z].m_listChar; pChar; pChar = pChar->m_pCellNext)
			{
				// 자신은 제외하고
				if (pChar == npc)
					continue ;

				// 죽은넘 제외
				if (DEAD(pChar))
					continue ;

				// 다른층 제외
				if (ABS(GET_YLAYER(npc) - GET_YLAYER(pChar)) >= 2)
					continue ;

				// NPC만
				if (!IS_NPC(pChar))
					continue ;

				CNPC* tnpc = TO_NPC(pChar);

				// 동족이고
				if (tnpc->m_proto->m_family != npc->m_proto->m_family)
					continue ;

				// aitype이 일치하고
				if (aitype != -1 && aitype != tnpc->m_proto->m_aitype)
					continue ;

				// 체력 검사
				if (tnpc->m_hp > tnpc->m_maxHP * hpParam / 100)
					continue ;

#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템

				// 테이밍 중인 몬스터나 혼란 중인 몬스터는 동족으로 보지 않는다.
				if ( tnpc->Check_MobFlag( STATE_MONSTER_TAMING ) || tnpc->Check_MobFlag( STATE_MONSTER_CONFUSION ) )
					continue;
#endif  // NIGHTSHADOW_SKILL	

				// 거리 제일 가까운 넘 찾기
				if (GetDistance(npc, tnpc) < dist)
				{
					dist = GetDistance(npc, tnpc);
					ret = tnpc;
				}
			}
		}
	}
	
	return ret;
}

CCharacter* CArea::FindCharInCell(CCharacter* ch, int idx, MSG_CHAR_TYPE tchType, bool bIncludeDead)
{
	if (!IS_IN_CELL(ch))
		return NULL;

	int sx = ch->m_cellX - CELL_EXT;
	int ex = ch->m_cellX + CELL_EXT;
	int sz = ch->m_cellZ - CELL_EXT;
	int ez = ch->m_cellZ + CELL_EXT;
	int x, z;
	
	// 일단 다른 층은 검사 안한다
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= m_size[0]) continue;
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= m_size[1]) continue;
			CCharacter *pChar;
			for (pChar = m_cell[x][z].m_listChar; pChar; pChar = pChar->m_pCellNext)
			{
				if (pChar->m_type == tchType && pChar->m_index == idx && (!DEAD(pChar) || bIncludeDead))
					return pChar;
			}
		}
	}
	
	return NULL;
}

bool CArea::MoveChar(CCharacter* ch, char y, float x, float z, float h, float r, MSG_MOVE_TYPE movekind, CCharacter* target)
{
	// 몹인데 못 움직이거나, 리젠 후 얼마 지나지 않았으면 가만히
	if (IS_NPC(ch) && !ch->CanMove())
		return false;
	
	bool change_position = false; //좌표가 변경되었는지 여부
	
	char outY;
	if (!IsNotBlocked(ch, y, x, z, false, outY))
		return false;

	y = outY;
	
	int cx, cz;
	PointToCellNum(x, z, &cx, &cz);
	
	CPC* pc = (IS_PC(ch)) ? TO_PC(ch) : NULL;
	CNPC* npc = (IS_NPC(ch)) ? TO_NPC(ch) : NULL;
	
	// 몹일때
	if (IS_NPC(ch))
	{
		CCharacter* pChar;
		CCharacter* pnext = m_cell[cx][cz].m_listChar;
		while ((pChar = pnext))
		{
			pnext = pChar->m_pCellNext;
			if (GET_YLAYER(pChar) != y)
				continue ;
			
			if (pChar != ch && !DEAD(ch))
			{
				float p_X, p_Z, p_H;
				p_X = ABS(GET_X(pChar) - x);
				p_Z = ABS(GET_Z(pChar) - z);
				p_H = ABS(GET_H(pChar) - h);
				
				// pChar가 ch의 움직임에 걸리적 거린다
				if (p_X < 0.2 && p_Z < 0.2 && p_H < 0.2)
				{
					float nx, nz, nh;
					nx = GET_X(pChar) - (float)cos(r) * 0.5f;
					nz = GET_Z(pChar) - (float)sin(r) * 0.5f;
					nh = GetHeight(y, nx, nz);
					
					float dist= GetDistance(x, GET_X(ch), z, GET_Z(ch), h, GET_H(ch));
					float ndist = GetDistance(nx, GET_X(ch), nz, GET_Z(ch), nh, GET_H(ch));
					
					if ( dist > ndist )
					{
						r += GetRandom((int)(-PI_HALF * 1000), (int)(PI_HALF * 1000)) / 2000.0f;
						
						// 높이적용
						x = GET_X(ch) + (float)cos(r) * 0.5f;
						z = GET_Z(ch) + (float)sin(r) * 0.5f;
						h = GetHeight(GET_YLAYER(ch), x, z);
						
						float trh = (float)asin((h - GET_H(ch)) / (GetDistance(x, GET_X(ch), z, GET_Z(ch), h, GET_H(ch)) + 0.1f));
						
						x = GET_X(ch) + (float)cos(r) * 0.5 * (float)cos(trh);
						z = GET_Z(ch) + (float)sin(r) * 0.5 * (float)sin(trh);
						h = GetHeight(y, x, z);
						
						// delay를 반으루
						npc->m_delay = 0;
						change_position = true;
					}
					// 걸리적 거리는 넘이 타겟이면 이동한 척
					if (target == pChar)
						return true;
				}
			}
		}
		
		// 좌표 바끈데두 블럭인 경우
		if (change_position && !IsNotBlocked(npc, y, x, z, false, outY))
			return false;
		
		y = outY;
		
		// 안보이는 상태이면
		if (npc->m_disableTime != 0)
		{
			npc->Move(x, z, h, r, y);
			return true;
		}
	} // 몹일때

	// 이동시 스킬 해제되는 것 처리
	ch->m_assist.CancelFakeDeath();

	// 셀 안 이동과 셀 이동 구분 처리
	
	// 셀 안 이동
	if (ch->m_cellX == cx && ch->m_cellZ == cz)
	{
		CNetMsg msg;
		ch->Move(x, z, h, r, y);
		MoveMsg(msg, movekind, ch);
		SendToCell(msg, ch);

		if (IS_PC(ch))
		{
		
#ifdef FREE_PK_SYSTEM
			if( gserver.m_bFreePk )
			{
				if(!pc->IsSetPlayerState(PLAYER_STATE_PKMODE) && !pc->IsInPeaceZone(true))
				{
					pc->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
					pc->SetPlayerState(PLAYER_STATE_PKMODE);
					ActionMsg(msg, pc, 0, AGT_PKMODE);
					SendToCell(msg, ch, true);
				}

			}
#endif
				// 블럭아닌 피스존으로 들어가면 PKMODE 해제
			if (pc->IsSetPlayerState(PLAYER_STATE_PKMODE) && pc->IsInPeaceZone(false))
			{
				pc->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
				ActionMsg(msg, pc, 0, AGT_PKMODE);
				SendToCell(msg, ch, true);
			}

		}
	}
	else
	{
		int ocx = ch->m_cellX;
		int ocz = ch->m_cellZ;
		
		CharFromCell(ch, false);
		ch->Move(x, z, h, r, y);
		CharToCell(ch, y, cx, cz);
		
		CNetMsg movemsg;		// 캐릭터 이동
		CNetMsg appmsg;			// 캐릭터 나타남
		CNetMsg disappmsg;		// 캐릭터 사라짐
		CNetMsg tappmsg;		// 타 캐릭터 나타남
		CNetMsg tdisappmsg;		// 타 캐릭터 사라짐
		
		// 이동 메시지...
		MoveMsg(movemsg, movekind, ch);
		AppearMsg(appmsg, ch);
		DisappearMsg(disappmsg, ch);

#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
		CNetMsg GuildPointMsg;
		CNetMsg tGuildPointRankingMsg;
		if( pc )
		{
			if( pc->m_guildInfo && pc->m_guildInfo->guild() )
			{
				GuildPointRankingMsg( GuildPointMsg, pc->m_index, pc->m_guildInfo->guild()->index(), pc->m_guildInfo->guild()->GetGuildPointRanking() );			
			}
		}
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD

#ifdef DYNAMIC_DUNGEON
		CNetMsg NameColorMsg;
		if( pc )
		{
			GuildNameColorStateMsg(NameColorMsg, pc); 
		}
#endif //DYNAMIC_DUNGEON

		
		int i, j;
		int sx, tx, sz, tz;
		bool bwas, bnew;
		if (ocx < cx)	{ sx = ocx; tx = cx; }
		else			{ tx = ocx; sx = cx; }
		if (ocz < cz)	{ sz = ocz; tz = cz; }
		else			{ tz = ocz; sz = cz; }
		
		if( sz > 10000 || tx > 10000 || sz > 10000 || tz > 10000 )
		{
			GAMELOG << init("MOVE ERROR") << "sx,sz,tx,tx : " << sx << delim << sz << delim << tx << delim << tx << end;
			return false;
		}
		
		for (i = sx - CELL_EXT; i <= tx + CELL_EXT; i++)
		{
			if (i < 0 || i >= m_size[0]) continue ;
			
			for (j = sz - CELL_EXT; j <= tz + CELL_EXT; j++)
			{
				if (j < 0 || j >= m_size[1]) continue ;
				
				// 원래 있던 방
				bwas = (ABS(ocx - i) <= CELL_EXT && ABS(ocz - j) <= CELL_EXT);
				// 지금 있는 방
				bnew = (ABS(cx - i) <= CELL_EXT && ABS(cz - j) <= CELL_EXT);
				
				if (!bwas && !bnew)
					continue;
				
				CCharacter* pChar;
				CCharacter* pNext = m_cell[i][j].m_listChar;
				while ((pChar = pNext))
				{
					pNext = pChar->m_pCellNext;
					if (pChar == ch)
						continue ;
					
					// 원래 있던 셀
					if (bwas)
					{
						// 지금도 있는 셀
						if (bnew)
						{
							// 상대에게 내가 움직임을 알림
							if (IS_PC(pChar))
							{
									SEND_Q(movemsg, TO_PC(pChar)->m_desc);
							}
						}
						
						// 지금은 없는 셀
						else
						{
							// 상대에게 내가 사라짐을 알림
							if (IS_PC(pChar))
							{
									SEND_Q(disappmsg, TO_PC(pChar)->m_desc);
							}
							// 나에게 상대가 사라짐을 알림
							DisappearMsg(tdisappmsg, pChar);
							if (pc)
								SEND_Q(tdisappmsg, pc->m_desc);
						}
					}
					
					// 이전에 없다가 지금 있는 셀
					else if (bnew)
					{
						// 상대에게 내가 나타남을 알림
						if (IS_PC(pChar))
						{
								SEND_Q(appmsg, TO_PC(pChar)->m_desc);
#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
								SEND_Q(GuildPointMsg, TO_PC(pChar)->m_desc);
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD
#ifdef DYNAMIC_DUNGEON
								SEND_Q(NameColorMsg, TO_PC(pChar)->m_desc);
#endif //DYNAMIC_DUNGEON
						}
						// 나에게 상대의 등장을 알림
						AppearMsg(tappmsg, pChar);
	
						if (pc)
						{
							SEND_Q(tappmsg, pc->m_desc);
#ifdef NEW_GUILD
#ifdef NEW_GUILD_POINT_RANKING_NOTICE
							CNetMsg GuildPointMsg;
							CPC* tpc = TO_PC(pChar);
							if( tpc )
							{
								if( tpc->m_guildInfo && tpc->m_guildInfo->guild() )
								{
									GuildPointRankingMsg( tGuildPointRankingMsg, tpc->m_index, tpc->m_guildInfo->guild()->index(), tpc->m_guildInfo->guild()->GetGuildPointRanking() );
									SEND_Q(tGuildPointRankingMsg, pc->m_desc);
								}
							}
#endif // NEW_GUILD_POINT_RANKING_NOTICE
#endif // NEW_GUILD
#ifdef DYNAMIC_DUNGEON
							if( TO_PC(pChar) )
							{
								CNetMsg NameColorMsg;
								GuildNameColorStateMsg(NameColorMsg, TO_PC(pChar) ); 
								SEND_Q(NameColorMsg, pc->m_desc);
							}
#endif //DYNAMIC_DUNGEON
						}


					}
				} // end while
				
				CItem* pItem;
				CItem* pItemNext = m_cell[i][j].m_listItem;
				while (IS_PC(ch) && (pItem = pItemNext))
				{
					pItemNext = pItem->m_pCellNext;
					if (bnew && !bwas)
					{
						// 나타남
						ItemAppearMsg(tappmsg, pItem);
						SEND_Q(tappmsg, pc->m_desc);
					}
					else if (!bnew && bwas)
					{
						// 없어짐
						ItemDisappearMsg(tdisappmsg, pItem);
						SEND_Q(tdisappmsg, pc->m_desc);
					}
				} // end while
			} // end for z
		} // end for x

		if (IS_PC(ch))
		{
#ifdef FREE_PK_SYSTEM
			if( gserver.m_bFreePk )
			{
				if(!pc->IsSetPlayerState(PLAYER_STATE_PKMODE) && !pc->IsInPeaceZone(true))
				{
					pc->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
					pc->SetPlayerState(PLAYER_STATE_PKMODE);
					ActionMsg(movemsg, pc, 0, AGT_PKMODE);
					SendToCell(movemsg, ch, true);
				}

			}
#endif

			// 블럭아닌 피스존으로 들어가면 PKMODE 해제
			if (pc->IsSetPlayerState(PLAYER_STATE_PKMODE) && pc->IsInPeaceZone(false))
			{
				pc->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
				ActionMsg(movemsg, pc, 0, AGT_PKMODE);
				SendToCell(movemsg, ch, true);
			}
		}
	}
	
	if (IS_PC(ch))
	{
		if (pc->IsParty())
		{
			CNetMsg rmsg;
			PartyInfoMsg(rmsg, pc->m_index, pc->m_level, pc->m_hp, pc->m_maxHP, pc->m_mp, pc->m_maxMP, GET_X(pc), GET_Z(pc), GET_YLAYER(pc), pc->m_pZone->m_index);
			pc->m_party->SendToPCInSameZone(pc->m_pZone->m_index, pc->m_pArea->m_index, rmsg);
		}
	}

#ifdef ENABLE_PET
	if (IS_PET(ch))
	{
		CPet* pet = TO_PET(ch);
		if (pet->GetOwner())
		{
			CNetMsg rmsg;
			ExPetStatusMsg(rmsg, pet);
			SEND_Q(rmsg, pet->GetOwner()->m_desc);
		}
	}
#endif
	
	return true;
}

bool CArea::SendToCell(CNetMsg& msg, CCharacter* ch, bool withme, int cellext)
{
	if (!IS_IN_CELL(ch))
		return false;

	int sx = ch->m_cellX - cellext;
	int ex = ch->m_cellX + cellext;
	int sz = ch->m_cellZ - cellext;
	int ez = ch->m_cellZ + cellext;
	
	int x, z;
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= m_size[0])
			continue ;
		
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= m_size[1])
				continue ;
			
			CCharacter* pChar = m_cell[x][z].m_listChar;
			while (pChar)
			{
				if (IS_PC(pChar) && (pChar != ch || withme))
				{
#ifdef ENABLE_PET
					if (IS_PET(ch))
					{
						CPet* pet = TO_PET(ch);
						if (pet->GetOwner())
						{
							if (!withme && pet->GetOwner() == pChar)
								goto CONTINUE_LOOP;
						}
					}
#endif
					SEND_Q(msg, TO_PC(pChar)->m_desc);
				}
#ifdef ENABLE_PET
CONTINUE_LOOP:
#endif
				pChar = pChar->m_pCellNext;
			}
		}
	}
	return true;
}

bool CArea::SendToCell(CNetMsg& msg, char /* y */, int cx, int cz, int cellext)
{
	int sx = cx - cellext;
	int ex = cx + cellext;
	int sz = cz - cellext;
	int ez = cz + cellext;
	
	int x, z;
	
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= m_size[0])
			continue;
		
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= m_size[1])
				continue;
			
			CCharacter* pChar = m_cell[x][z].m_listChar;
			while (pChar)
			{
				if (IS_PC(pChar))
				{
						SEND_Q(msg, TO_PC(pChar)->m_desc);
				}
				pChar = pChar->m_pCellNext;
			}
		}
	}
	
	return true;
}

// 선공 npc의 경우 가까운 target 찾기
CCharacter* CArea::FindNearestTarget(CNPC* npc, float* dist)
{
	// 가까운 공격대상 찾기
	int cx, cz;
	PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
	
	int sx = (cx - CELL_EXT < 0) ? 0 : cx - CELL_EXT;
	int ex = (cx + CELL_EXT < m_size[0]) ? cx + CELL_EXT : m_size[0] - 1;
	int sz = (cz - CELL_EXT < 0) ? 0 : cz - CELL_EXT;
	int ez = (cz + CELL_EXT < m_size[1]) ? cz + CELL_EXT : m_size[1] - 1;
	CCell* cell;
	
	float tdist = npc->m_proto->m_sight;
	
	CCharacter* ret = NULL;
#ifdef ENABLE_WAR_CASTLE
	int towerSkillIndex = -1;	// 공성탑 스킬 인덱스
#endif
	
	int i, j;
	for (i = sx; i <= ex; i++)
	{
		for (j = sz; j <= ez; j++)
		{
			cell = &m_cell[i][j];
			
			CCharacter* pChar;
			CCharacter* pCharNext = cell->m_listChar;
			CPC* owner = NULL;			// pChar가 PC이면 자신, Pet이나 소환수면 owner
			
			while ((pChar = pCharNext))
			{
				pCharNext = pCharNext->m_pCellNext;

				// 죽은 것도 아니고
				if (DEAD(pChar))
					continue ;
#ifdef RAID_MONSTER_SKIP_FAKEDEATH
				// 레이드 몹일 경우  죽은척 하고 잇을때도 공격
				if (!npc->m_proto->CheckFlag(NPC_RAID))
				{				
#endif // RAID_MONSTER_SKIP_FAKEDEATH
				// 죽은 척 하고 있지 않으면
				if (pChar->m_assist.m_state & AST_FAKEDEATH)
					continue ;
#ifdef RAID_MONSTER_SKIP_FAKEDEATH
				}
#endif // RAID_MONSTER_SKIP_FAKEDEATH

				// 안보이면 skip
				if (!npc->CanVisible(pChar))
					continue ;

				// 시야 밖이거나 이전 값보다 멀면 스킵
				if (tdist < GetDistance(pChar, npc))
					continue ;

				// PC일 경우
				if (IS_PC(pChar))
				{
					CPC* pc = TO_PC(pChar);

					// immortal도 아니고
					if (pc->m_bImmortal || pc->IsDisable())
						continue ;
						
					// 변신도 스킵
					if (pc->IsSetPlayerState(PLAYER_STATE_CHANGE))
						continue ;

					owner = pc;
				}

#ifdef ENABLE_PET
				// Pet일 경우
				else if (IS_PET(pChar))
				{
					CPet* pet = TO_PET(pChar);

					// 주인 없는 펫은 스킵
					if (pet->GetOwner() == NULL)
						continue ;

					owner = pet->GetOwner();
				}
#endif // #ifdef ENABLE_PET

				// 소환수
				else if (IS_ELEMENTAL(pChar))
				{
					CElemental* pElemental = TO_ELEMENTAL(pChar);
					CPC* pc = pElemental->GetOwner();
					if (!pc)
						continue ;

					// immortal도 아니고
					if (pc->m_bImmortal || pc->IsDisable())
						continue ;
						
					owner = pc;
				}
#ifdef ATTACK_PET
				else if ( IS_APET(pChar) )
				{
					CAPet* apet = TO_APET(pChar);
					
					if( apet->GetOwner() == NULL )
						continue;
					owner = apet->GetOwner();
				}
#endif //ATTACK_PET

#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
				// npc 일때 각 state 에 따른 행동 
				else if ( IS_NPC(pChar) )
				{
					CNPC* pNpc = TO_NPC(pChar);

					// 일반 몬스터는 스킵
					if ( !pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION) && !pNpc->Check_MobFlag( STATE_MONSTER_TAMING ) )
					{
						continue;
					}

					// 혼란 몬스터도 스킵
					else if ( pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION) && !pNpc->Check_MobFlag( STATE_MONSTER_TAMING ) )
					{
						continue;
					}

					// 테이밍된 몬스터는 공격
					else if ( !pNpc->Check_MobFlag( STATE_MONSTER_CONFUSION) && pNpc->Check_MobFlag( STATE_MONSTER_TAMING ) )
					{

					}

					// 버그일 경우 이곳으로 오고 무조건 스킵
					else
					{
						// NPC_JUNK Log
						GAMELOG << init("NPC_JUNK")
								<< "Target_npc Index" << delim
								<< pNpc->m_index << delim		
								<< "Target_npc Name" << delim
								<< pNpc->m_name << delim		
								<< end;
								
						continue;
					}
				}
#endif  // NIGHTSHADOW_SKILL	
			
				// 이도 저도 아니면 스킵
				else
					continue ;

#ifdef ENABLE_WAR_CASTLE
				// 수호병은 공성 용병/길드만 공격
				if (npc->m_proto->CheckFlag(NPC_CASTLE_GUARD | NPC_CASTLE_TOWER))
				{
					bool bTarget = false;
					switch (owner->GetJoinFlag(m_zone->m_index))
					{
					case WCJF_ATTACK_GUILD:
					case WCJF_ATTACK_CHAR:
						bTarget = true;
						break;
					}
					if (!bTarget)
						continue ;
				}

				// 수호탑은 스킬 최소거리 이상되는 적에게만 공격 가능
				// 스킬 사용 가능 검사
				int sloop = -1;
				if (npc->m_proto->CheckFlag(NPC_CASTLE_TOWER))
				{
					for (sloop = 0; sloop < MAX_NPC_SKILL; sloop++)
					{
						// 스킬 사용 가능 검사
						if (npc->m_skills[sloop]
								&& (sloop == 0 || towerSkillIndex == -1 || npc->m_proto->m_skillProb[sloop] >= GetRandom(1, 10000))
								&& npc->m_skills[sloop]->IsReady(npc)
								&& npc->CanSpell()
								&& npc->m_hp > npc->m_skills[sloop]->m_proto->Level(npc->m_skills[sloop]->m_level)->m_needHP
								&& npc->m_mp > npc->m_skills[sloop]->m_proto->Level(npc->m_skills[sloop]->m_level)->m_needMP
								&& pChar->CanApplySkill(npc->m_skills[sloop]->m_proto, npc->m_skills[sloop]->m_proto->Level(npc->m_skills[sloop]->m_level))
								&& GetDistance(npc, pChar) <= npc->m_skills[sloop]->m_proto->GetFireRange(npc)
								&& GetDistance(npc, pChar) >= npc->m_skills[sloop]->m_proto->GetMinRange())
						{
							break ;
						}
					}
					// 스킬 없으면 수호탑은 타겟 없음
					if (sloop == MAX_NPC_SKILL)
						continue ;
				}
#endif // #ifdef ENABLE_WAR_CASTLE

				// 시야 안에 들어왔으면 타겟으로
				char outY;

				// npc -> pChar 시야안에서 발견 시 npc와 pChar의 직선경로에서 블럭이 있는 경우
				if (!IsNotBlocked(npc, pChar, true, outY))
					continue;
				
				tdist = GetDistance(pChar, npc);
				ret = pChar;

#ifdef ENABLE_WAR_CASTLE
				// 수호탑 아니면 타겟 잡히는대로 리턴, 수호탑은 스킬 번호 저장
				if (!npc->m_proto->CheckFlag(NPC_CASTLE_TOWER))
					return ret;
				else
					towerSkillIndex = sloop;
#endif // #ifdef ENABLE_WAR_CASTLE
			} // end : while ((pChar = pCharNext))
		}
	}


#ifdef ENABLE_WAR_CASTLE
	if (npc->m_proto->CheckFlag(NPC_CASTLE_TOWER))
	{
		// 수호탑은 스킬 없으면 타겟도 없다
		if (towerSkillIndex == -1)
			return NULL;
		// 수호탑일 경우 스킬 세팅도 함께
		npc->m_currentSkill = npc->m_skills[towerSkillIndex];
		npc->m_currentSkill->m_targetType = ret->m_type;
		npc->m_currentSkill->m_targetIndex = ret->m_index;
	}
#endif

	if (ret)
	{
		*dist = GetDistance(ret, npc);
	}

	return ret;
}

#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
CCharacter* CArea::FindNearestTarget_Taming_or_Confused(CNPC* npc, float* dist) // 테이밍된 npc가 가까운 npc를 찾기
{
	// 가까운 공격대상 찾기
	int cx, cz;
	PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
	
	int sx = (cx - CELL_EXT < 0) ? 0 : cx - CELL_EXT;
	int ex = (cx + CELL_EXT < m_size[0]) ? cx + CELL_EXT : m_size[0] - 1;
	int sz = (cz - CELL_EXT < 0) ? 0 : cz - CELL_EXT;
	int ez = (cz + CELL_EXT < m_size[1]) ? cz + CELL_EXT : m_size[1] - 1;
	CCell* cell;
	
	float tdist = npc->m_proto->m_sight;
	
	int i, j;
	for (i = sx; i <= ex; i++)
	{
		for (j = sz; j <= ez; j++)
		{
			cell = &m_cell[i][j];
			
			CCharacter* pChar;
			CCharacter* pCharNext = cell->m_listChar;
			
			while ((pChar = pCharNext))
			{
				pCharNext = pCharNext->m_pCellNext;

				// 죽은 것도 아니고
				if (DEAD(pChar))
					continue ;

				// 안보이면 skip
				if (!npc->CanVisible(pChar))
					continue ;

				// 시야 밖이거나 이전 값보다 멀면 스킵
				if (tdist < GetDistance(pChar, npc))
					continue ;

				// 시야 안에 들어왔으면 타겟으로
				char outY;

				// npc -> pChar 시야안에서 발견 시 npc와 pChar의 직선경로에서 블럭이 있는 경우
				if (!IsNotBlocked(npc, pChar, true, outY))
					continue;

				// 만약 PC가 타겟이면 테이밍된 몬스터가 힐을 쓸 때만 사용
				if (IS_PC(pChar))
				{
					// 몬스터가 혼란 중이면 스킵
					if ( npc->Check_MobFlag(STATE_MONSTER_CONFUSION) )
					{
						continue;
					}
					else
					{
						*dist = GetDistance(pChar, npc);
						return pChar;
					}
				}

				// 테이밍 되지 않은 NPC만 공격
				else if (IS_NPC(pChar))   
				{
					CNPC* target_is_npc = NULL;
					target_is_npc = TO_NPC(pChar);

					// 안보이는 상태이면 스킵
					if ( npc->m_disableTime != 0 )
					{
						continue;
					}

					// 몬스터 자신이면 스킵
					if ( target_is_npc == npc )
					{
						continue;
					}

					// 몬스터가 혼란 중이라면 혼란 중인 몬스터만 공격
					else if ( target_is_npc->Check_MobFlag(STATE_MONSTER_CONFUSION)
							  && npc->Check_MobFlag(STATE_MONSTER_CONFUSION) )
					{
						*dist = GetDistance(pChar, npc);
						return pChar;
					}

					// 혼란 중이 아니면 테이밍 중이므로 일반 몬스터 공격
					else if ( !target_is_npc->Check_MobFlag(STATE_MONSTER_TAMING) && npc->Check_MobFlag(STATE_MONSTER_TAMING) )
					{
						*dist = GetDistance(pChar, npc);
						return pChar;
					}

					// 위의 조건을 만족 못하면 새로운 타겟을 찾는다
					else
					{
						continue;
					}
				}
				
				
			} // end : while ((pChar = pCharNext))
		}
	}
	return NULL;
}
#endif  // NIGHTSHADOW_SKILL

CNPC* CArea::FindProtoNPCInCell(CCharacter* ch, int nData, bool bIsFlagMask, int findNumCell, bool bIncludeDead)
{
	if (!IS_IN_CELL(ch))
		return NULL;

	return FindProtoNPCInCell(GET_X(ch), GET_Z(ch), nData, bIsFlagMask, findNumCell, bIncludeDead);
}

CNPC* CArea::FindProtoNPCInCell(float fx, float fz, int nData, bool bIsFlagMask, int findNumCell, bool bIncludeDead)
{
	int nCellX = 0;
	int nCellZ = 0;
	PointToCellNum(fx, fz, &nCellX, &nCellZ);

	int sx = nCellX - findNumCell;
	int ex = nCellX + findNumCell;
	int sz = nCellZ - findNumCell;
	int ez = nCellZ + findNumCell;
	
	int x, z;
	
	// 일단 다른 층은 검사 안한다
	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= m_size[0]) continue;
		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= m_size[1]) continue;
			CCharacter *pChar;
			for (pChar = m_cell[x][z].m_listChar; pChar; pChar = pChar->m_pCellNext)
			{
				if (!IS_NPC(pChar) )
					continue;

				CNPC* npc = TO_NPC(pChar);
				if (bIsFlagMask)
				{
					if ( npc->m_proto->CheckFlag(nData) && (!DEAD(npc) || bIncludeDead))
						return npc;
				}
				else
				{
					if ( npc->m_proto->m_index == nData && (!DEAD(npc) || bIncludeDead))
						return npc;
				}
			}
		}
	}
	
	return NULL;
}


#ifdef MONSTER_COMBO
void CArea::RunComboZone()
{
	if(!m_monsterCombo)
		return ;

	// ret: 0 정상완료  1 진행중 2 오류
	char ret;
	ret = m_monsterCombo->Run();
	
	switch(ret)
	{
	case 2:
		{
			GAMELOG << init("CLOSE COMBO AREA")
					<< "BY Combo Run() Error"
					<< "AREA INDEX: " << m_index
					<< "COMBO INDEX: " << m_monsterCombo->m_nIndex  
					<< end;

			CloseComboZone();
		}
		break;
	}
}

int CArea::CountPCInExtra(int extra, bool bIncludeDead)
{
	int count = 0;
	CCharacter* pChar;
	CCharacter* pCharNext;
	int i,j;
	for(i = 0; i < m_size[0]; ++i)
	{
		for(j = 0 ; j < m_size[1]; ++j)
		{
			pCharNext = m_cell[i][j].m_listChar;
			while((pChar = pCharNext))
			{
				pCharNext = pChar->m_pCellNext;
				if(!IS_PC(pChar))
					continue;

#ifdef GM_GO_ZONE
					if(TO_PC(pChar)->m_gmGoZoneType == GM_GOZONE_COMBO)
						continue;
#endif // GM_GO_ZONE

				if( (!bIncludeDead && DEAD(pChar)) )
					continue;

				if( ((int)(pChar->m_pos.m_x) >= pChar->m_pZone->m_zonePos[extra][1]/2) && ((int)(pChar->m_pos.m_x) <= pChar->m_pZone->m_zonePos[extra][3]/2) &&
					((int)(pChar->m_pos.m_z) >= pChar->m_pZone->m_zonePos[extra][2]/2) && ((int)(pChar->m_pos.m_z) <= pChar->m_pZone->m_zonePos[extra][4]/2) &&
					((int)(pChar->m_pos.m_yLayer) == pChar->m_pZone->m_zonePos[extra][0]))
					count++;
			}
		}
	}

	return count;
}


int CArea::CountMobInCombo()
{
	int count = 0;
	CNPC* npc;
	CNPC* npcNext = m_npcList;

	while((npc = npcNext))
	{
		npcNext = npc->m_pNPCNext;

		// 평화몹에 있는 npc 이거나 이펙트 npc 인경우 제외
		if(npc->m_proto->CheckFlag(NPC_PEACEFUL) || npc->m_proto->m_index == 491) 
			continue;

		count++;
	}
	return count;
}

void CArea::CloseComboZone()
{
	// 캐릭터 드라탄으로 이동
	if(m_zone->m_index != ZONE_COMBO_DUNGEON)
		return ;
	
	CNetMsg rmsg;
	CCharacter* pChar;
	CCharacter* pCharNext;
	CZone*		pZone;

	CLCList<CPC*> listPC(NULL);

	int zone	= ZONE_DRATAN;
	int extra	= 0;
	int i, j;

	for(i = 0 ; i < m_size[0]; ++i)
	{
		for(j = 0; j < m_size[1]; ++j)
		{
			pCharNext = m_cell[i][j].m_listChar;
			while((pChar = pCharNext))
			{
				pCharNext = pChar->m_pCellNext;

				if(IS_PC(pChar))
					listPC.AddToTail(TO_PC(pChar));
			}
		}
	}

	void* pos = NULL;
	pos = listPC.GetHead();
	while(pos)
	{
		pChar = listPC.GetData(pos);
		
		int k;
		k = gserver.FindZone(zone);
		
		if(k == -1)
			return ;
		
		pZone = gserver.m_zones + k;
		
		GoZone((CPC*)pChar, zone,
			pZone->m_zonePos[extra][0],														// ylayer
			GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
			GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z

		pos = listPC.GetNext(pos);
	}

	listPC.RemoveAll();
}
#endif // MONSTER_COMBO