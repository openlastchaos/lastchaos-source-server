#ifndef __AZONE_H__
#define __AZONE_H__

#include "MapAttr.h"
#include "Area.h"
#include "Shop.h"

class CZone
{
public:
	CZone();
	~CZone();

	int				m_index;		// 존 번호
	bool			m_bRemote;		// 원격 존
	CLCString		m_remoteIP;		// 원격지 주소
	unsigned short	m_remotePort;	// 원격지 포트
	int				m_countY;		// 층 수
	CMapAttr*		m_attrMap;		// 속성 맵
	CArea*			m_area;			// CArea
	int				m_countArea;	// Area 레이어 수 ( >= 0 )
	unsigned char	m_weather;		// 날씨
	bool			m_bCanMemPos;	// 장소 기억 장치 사용 가능 여부
#ifdef ENABLE_PET
	bool			m_bCanSummonPet;// 펫 소환 가능 여부
	bool			m_bCanMountPet;	// 펫 타기 가능 여부
#endif

	int				m_countZonePos;	// 시작 장소 수
	int**			m_zonePos;		// 시작 장소 위치 : int [count][5]
									// 0부터 y, left, top, right, bottom
									// left, top, right, bottom은 0.5m 단위의 정수

	// Shop 관련
	CShop*			m_shopList;		// Shop List
	int				m_nShopCount;	// shop count
	int				m_sellRate;		// sell rate
	int				m_buyRate;		// buy rate

	bool LoadZone(int seq);
	bool LoadNPC();
	bool LoadShop();
	CShop* FindShop(int npcIdx);
	void ChangeWeather();
	void ChangeTaxRate(int sellRate, int buyRate);

	int FindEmptyArea();
	int SetEnableArea();
	void SetDisableArea(int idx);

	bool IsPersonalDungeon()
	{
		switch (m_index)
		{
		case ZONE_SINGLE_DUNGEON1:
		case ZONE_SINGLE_DUNGEON2:
		case ZONE_SINGLE_DUNGEON_TUTORIAL:
		case ZONE_SINGLE_DUNGEON3:
		case ZONE_SINGLE_DUNGEON4:
			return true;
		default:
			return false;
		}
	}
	bool IsSummonZone()
	{
		switch(m_index)
		{
		case ZONE_SINGLE_DUNGEON1:
		case ZONE_SINGLE_DUNGEON2:
		case ZONE_SINGLE_DUNGEON_TUTORIAL:
		case ZONE_SINGLE_DUNGEON3:
		case ZONE_SINGLE_DUNGEON4:
		case ZONE_OXQUIZROOM:
		case ZONE_FREE_PK_DUNGEON:
		case ZONE_COMBO_DUNGEON:
#ifdef EXTREME_CUBE
		case ZONE_EXTREME_CUBE:
#endif // EXTREME_CUBE
			return true;
		default:
			return false;
		}
	}

	bool IsGuildRoom()
	{
		return (m_index == ZONE_GUILDROOM);
	}

	bool IsOXQuizRoom()
	{
		return (m_index == ZONE_OXQUIZROOM);
	}

	bool IsComboZone()
	{
		return (m_index == ZONE_COMBO_DUNGEON);
	}

	bool IsExtremeCube()
	{
		return (m_index == ZONE_EXTREME_CUBE);
	}

	// 공성지역이지만 계단이나 입구는 따로 체크
	bool IsWarZone(int x, int z)
	{
		int extra = GetExtra(x, z);
		
		switch(m_index)
		{
		case ZONE_MERAC:
			if (extra == 16)
				return true;
		case ZONE_DRATAN:
			if (extra == 26)
				return true;
		default:
			return false;
		}
	}

#ifdef MONSTER_COMBO
	int FindComboArea(int comboidx);
#endif 

	int GetExtra(int x, int z);

protected:
	static int CompIndex(const void* p1, const void* p2)
	{
		CShop* i1 = (CShop*)p1;
		CShop* i2 = (CShop*)p2;

		return i1->m_keeperIdx - i2->m_keeperIdx;
	}

};

#endif
