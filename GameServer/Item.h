// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__91327288_815B_4A26_AF31_0F9098AEECF3__INCLUDED_)
#define AFX_ITEM_H__91327288_815B_4A26_AF31_0F9098AEECF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CPos.h"
#include "Option.h"
#include "RareOptionProto.h"
#include "Mutex.h"
#ifdef SET_ITEM
#include "SetItemProto.h"
#endif

class CItemProto 
{
public:
	
	CItemProto();
	virtual ~CItemProto();

	//////////////////
	// DB 정보 변수
	int		m_index;			// 아이템번호
	int		m_typeIdx;			// 타입종류
	int		m_subtypeIdx;		// 서브타입종류
	CLCString m_name;			// 아이템이름
	int		m_jobFlag;			// 사용종족
	int		m_flag;				// 속성
	int		m_maxUse;			// 내구도
	int		m_fame;				// 명성치

	union
	{
		int m_num0;
	};
	union
	{
		int	m_num1;
	};
	union
	{
		int	m_num2;
	};
	union
	{
		int	m_num3;
	};
	union
	{
		int	m_num4;
	};
	int		m_weight;			// 무게
	int		m_price;			// 가격
	int		m_wearing;			// 착용 위치

	// 아이템 재료
	int		m_materialIndex[MAX_MAKE_ITEM_MATERIAL];
	int		m_materialCount[MAX_MAKE_ITEM_MATERIAL];

	int		m_needSSkillIndex;
	int		m_needSSkillLevel;

	int		m_needSSkillIndex2;
	int		m_needSSkillLevel2;

#if defined (MONSTER_RAID_SYSTEM) || defined (SET_ITEM)
	int		m_nRareIndex[MAX_RARE_OPTION_ITEM];				// 레어 Index, 옵션/스킬 Index(0 : 귀속 설정, 1~6 : 옵션, 7~9 : 스킬)
	int		m_nRareProb[MAX_RARE_OPTION_ITEM];				// 레어 확률,  옵션/스킬 Level(1~6 : 옵션, 7~9 : 스킬)
#endif // MONSTER_RAID_SYSTEM

private:
	int		m_level;			// 레벨

#ifdef SET_ITEM
	int		m_set4;				// 세트 아이템 Index
#endif

	// DB변수 여기까지

public:

	friend class CItemList;

#ifdef MONSTER_RAID_SYSTEM
	bool IsRareItem() const
	{
		return (m_flag & ITEM_FLAG_RARE) ? true : false;
	}
#endif // MONSTER_RAID_SYSTEM

	// 블러드젬 사용 가능 여부 : ITEM_FLAG_UPGRADE && !ITEM_FLAG_COMPOSITE
	bool CanBloodGem() const
	{
		if ((m_flag & ITEM_FLAG_UPGRADE) == 0)
			return false;
		if (m_flag & ITEM_FLAG_COMPOSITE)
			return false;
#ifdef MONSTER_RAID_SYSTEM
		if (IsRareItem())
			return false;
#endif // MONSTER_RAID_SYSTEM
#ifdef SET_ITEM
		if (IsOriginItem())
			return false;
#endif // SET_ITEM

		return true;
	}

	int GetItemProtoLevel() const
	{
		return m_level;
	}

#ifdef SET_ITEM
	bool IsOriginItem() const
	{
		return (m_flag & ITEM_FLAG_ORIGIN) ? true : false;
	}

	int GetSetIndex() const			
	{	
		return m_set4;					
	}
	int GetBelongType() const			{	return m_nRareIndex[0];			}
	int GetOriginIndex(int nPos) const	{   return m_nRareIndex[1 + nPos];	}
	int GetOriginLevel(int nPos) const	{   return m_nRareProb[1 + nPos];	}
	int GetOriginOptionCnt() const
	{   
		int nCount = 0;
		for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
		{
			if (GetOriginIndex(i) > -1)
				nCount += 1;
		}
		return nCount;
	}
	int GetOriginSkillCnt() const
	{   
		int nCount = 0;
		for (int i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
		{
			if (GetOriginIndex(i) > -1)
				nCount += 1;
		}
		return nCount;
	}

#endif // SET_ITEM

};


class CItem  
{
	friend class CInventoryRow;
	friend class CInventory;
	friend class CItemList;
	friend class CStash;

	/////////////////////
	// 인벤토리 위치 관련
	int			m_tab_idx;			// 탭
	int			m_row_idx;			// 줄번호 ( 세로 )
	int			m_col_idx;			// 칸 번호 ( 가로 )

	LONGLONG	m_count;			// 개수

public:
	
	CItem();
	virtual ~CItem();

	//////////////////
	// DB 정보 변수
	const CItemProto*	m_itemProto;

	//////////////
	// 아이템 속성
	int			m_index;			// 가상 인덱스
	int			m_idNum;			// DB 인덱스
	char		m_wearPos;			// 착용위치/탭
									// 0 ~ 11	: 투구 상의 특수 무기 하의 방패 장갑 신발 액서서리1, 2, 3
									// -1		: 착용 안함
	int			m_plus;				// + 수치, 애완동물 아이템의 경우 해당 애완동물 INDEX
									// 곤충 채집 상자 : 채집 곤충 데이터 1 (4비트 * 8마리)
#ifdef SET_ITEM
	int			m_plus2;			// 세트 아이템 : "물리/마법&방어/공격" 수치 저장
#endif

	int			m_flag;				// 속성
	CLCString	m_serial;			// 시리얼 번호

	int			m_used;				// 내구도
									// 생산도구 내구도
									// 플래티늄 재련석 기간
									// 의상 기간
									// 의상 결합 기간
									// 곤충 채집 상자 : 채집 곤충 데이터 2 (4비트 * 8마리)
#ifdef COMPOSITE_TIME
	int			m_used_2;			// 결합용의상:	결합 주문서 사용 기간, 기간제 아이템 남은 시간 
									// 무기, 장비:	레벨 다운 주문서 사용 기간, 
#endif // COMPOSITE_TIME	

#ifdef LEVELDOWN_SCROLL
	int			m_nLevel;			// 변경된 레벨 차이
#endif	// LEVELDOWN_SCROLL

	COption		m_option[MAX_ITEM_OPTION];	// 아이템 옵션, 레어 아이템의 경우 0은 레어 인덱스 2바이트, 1은 레어 비트 필드 2바이트
	int			m_nOption;					// 아이템 옵션 수

#ifdef MONSTER_RAID_SYSTEM
	const CRareOptionProto* m_pRareOptionProto; // 레어옵션 프로토
	int			m_nRareOptionIndex;	// 레어 옵션 인덱스
	int			m_nRareOptionBit;	// 레어 옵션 비트 필드
#endif // MONSTER_RAID_SYSTEM

	int			m_nCompositeItem;	// 합성된 아이템 : 기간을 가진 아이템이 원본 아이템 서버인덱스를 가짐, 없으면 -1, DB 저장시에는 row, col로 구분하여 저장

#ifdef SET_ITEM
	const CSetItemProto* m_pSetItemProto;
	int			m_nSetWearCnt;		// 세트 아이템 착용 개수
	int			m_nSetOption;		// 세트 옵션 적용 여부 - 1: 옵션만 적용, 2: 스킬까지 적용
#endif

	////////////
	// 기타 속성
	CArea*		m_pArea;			// Area
	CPos		m_pos;				// 좌표
	int			m_cellX;			// 셀 위치
	int			m_cellZ;
	CItem*		m_pCellPrev;		// 셀 링크드 리스트
	CItem*		m_pCellNext;

	int			m_groundPulse;		// 아이템이 떨어진 시간 저장
	int			m_preferenceIndex;	// 아이템 줍는 우선권을 가진 pc의 인덱스


	int tab() { return m_tab_idx; }
	int row() { return m_row_idx; }
	int col() { return m_col_idx; }


	LONGLONG Count() const
	{
		return m_count;
	}


	bool IsRangeWeapon()
	{
		if (m_itemProto->m_typeIdx == ITYPE_WEAPON)
		{
			switch (m_itemProto->m_subtypeIdx)
			{
			case IWEAPON_CROSSBOW:
			case IWEAPON_BOW:
			case IWEAPON_SOUL:
				return true;

			default:
				return false;
			}
		}

		return false;
	}


	bool IsMagicWeapon()
	{
		if (m_itemProto->m_typeIdx == ITYPE_WEAPON)
		{
			switch (m_itemProto->m_subtypeIdx)
			{
			case IWEAPON_SHORTSTAFF:
			case IWEAPON_STAFF:
			case IWEAPON_WAND:
			case IWEAPON_SCYTHE:
			case IWEAPON_POLEARM:
			case IWEAPON_SOUL:
				return true;

			default:
				return false;
			}
		}

		return false;
	}


// 050311 : bs : 무기가 어떤 스탯을 사용하는지 검사
	int GetUsingStat()
	{
		if (m_itemProto->m_typeIdx == ITYPE_WEAPON)
		{
			switch (m_itemProto->m_subtypeIdx)
			{
			case IWEAPON_CROSSBOW:
			case IWEAPON_BOW:
			case IWEAPON_SHORTGUM:
			case IWEAPON_WAND:
				return USING_DEX;

			default:
				return USING_STR;
			}
		}

		return -1;
	}
// --- 050311 : bs : 무기가 어떤 스탯을 사용하는지 검사


	bool IsWeaponType() const
	{
		if (m_itemProto->m_typeIdx == ITYPE_WEAPON)
		{
			switch (m_itemProto->m_subtypeIdx)
			{
			case IWEAPON_NIGHT:
			case IWEAPON_CROSSBOW:
			case IWEAPON_STAFF:
			case IWEAPON_BIGSWORD:
			case IWEAPON_AXE:
			case IWEAPON_SHORTSTAFF:
			case IWEAPON_BOW:
			case IWEAPON_SHORTGUM:
			case IWEAPON_TWOSWORD:
			case IWEAPON_WAND:
			case IWEAPON_SCYTHE:
			case IWEAPON_POLEARM:
			case IWEAPON_SOUL:
				return true;
			default:
				return false;
			}
		}
		return false;
	}

	bool IsArmorType() const
	{
		if (m_itemProto->m_typeIdx == ITYPE_WEAR)
		{
			switch (m_itemProto->m_subtypeIdx)
			{
			case		IWEAR_HELMET	:
			case		IWEAR_ARMOR		:
			case		IWEAR_PANTS		:
			case		IWEAR_GLOVE		:
			case		IWEAR_SHOES		:
			case		IWEAR_SHIELD	:
#ifdef ENABLE_BACKWING
			case		IWEAR_BACKWING	:
#endif //ENABLE_BACKWING
				return true;
			default:
				return false;
			}
		}

		return false;
	}

	bool IsAccessary()
	{
		return (m_itemProto->m_typeIdx == ITYPE_ACCESSORY && m_itemProto->m_subtypeIdx != IACCESSORY_PET );
	}

	bool IsPet()
	{
		return (m_itemProto->m_typeIdx == ITYPE_ACCESSORY && m_itemProto->m_subtypeIdx == IACCESSORY_PET );
	}
#ifdef ATTACK_PET
	bool IsAPet()
	{
		return (m_itemProto->m_typeIdx == ITYPE_ACCESSORY && m_itemProto->m_subtypeIdx == IACCESSORY_ATTACK_PET );
	}
#endif // ATTACK_PET

#ifdef PLATINUM
	bool IsUsedPlatinumSpecial() const
	{
		int platinumPlus;
		FLAG_ITEM_PLATINUM_GET(m_flag, platinumPlus);
		return ( (m_itemProto->m_typeIdx == ITYPE_WEAPON || m_itemProto->m_typeIdx == ITYPE_WEAR) && platinumPlus > 0 );
	}
#endif

	// 교환 가능한 아이템인가?
	bool CanExchange() const
	{
		// 프로토 검사
		if (!(m_itemProto->m_flag & ITEM_FLAG_EXCHANGE))
			return false;

		// 착용 아이템 불가능
		if (m_wearPos != WEARING_NONE)
			return false;

		// 봉인 아이템 불가능
		if (m_flag & FLAG_ITEM_SEALED)
			return false;

		// 결합 아이템 불가능
		if (m_flag & FLAG_ITEM_COMPOSITION)
			return false;

		// 대여 아이템 불능
		if (IsLent())
			return false;

#ifdef PLATINUM
		if (IsUsedPlatinumSpecial())
			return false;
#endif
#ifdef SET_ITEM
		// 귀속 아이템 불가능
		if (IsBelong())
			return false;
#endif
		return true;
	}

	// 드롭(유저가 버림) 가능한 아이템인가
	bool CanDrop() const
	{
		if (!(m_itemProto->m_flag & ITEM_FLAG_DROP))
			return false;

		// 착용 아이템 불가능
		if (m_wearPos != WEARING_NONE)
			return false;

		// 결합 아이템 불가능
		if (m_flag & FLAG_ITEM_COMPOSITION)
			return false;

		// 대여 아이템 불능
		if (IsLent())
			return false;

#ifdef PLATINUM
		if (IsUsedPlatinumSpecial())
			return false;
#endif
#ifdef SET_ITEM
		// 귀속 아이템 불가능
		if (IsBelong())
			return false;
#endif
		return true;
	}

	// 상점에 판매 가능 여부
	bool CanSellToShop() const
	{
		if (!(m_itemProto->m_flag & ITEM_FLAG_TRADE))
			return false;

		// 착용 아이템 불가능
		if (m_wearPos != WEARING_NONE)
			return false;

		// 결합 아이템 불가능
		if (m_flag & FLAG_ITEM_COMPOSITION)
			return false;

		// 대여 아이템 불능
		if (IsLent())
			return false;

#ifdef PLATINUM
		if (IsUsedPlatinumSpecial())
			return false;
#endif
#ifdef SET_ITEM
		// 귀속 아이템 불가능
		if (IsBelong())
			return false;
#endif
		return true;
	}

	// 창고 보관 가능 여부
	bool CanKeepStash() const
	{
		if (m_itemProto->m_flag & ITEM_FLAG_NO_STASH)
			return false;

		// 착용 아이템 불가능
		if (m_wearPos != WEARING_NONE)
			return false;

		// 결합 아이템 불가능
		if (m_flag & FLAG_ITEM_COMPOSITION)
			return false;

#ifdef COMPOSITE_TIME
		// 시간제 아이템 불가능
		if (m_itemProto->m_flag & ITEM_FLAG_ABS)
		{
			return false;
		}
#endif	// COMPOSITE_TIME

#ifdef PLATINUM
		if (IsUsedPlatinumSpecial())
			return false;
#endif
#ifdef SET_ITEM
		// 귀속 아이템 불가능
		if (IsBelong())
			return false;
#endif
		return true;
	}

	// 제련석 붙이기 가능 여부
	bool CanUpgrade() const
	{
		if ((m_itemProto->m_flag & ITEM_FLAG_UPGRADE) == 0)
			return false;
		if (IsLent())
			return false;
		return true;
	}

	// 블러드젬 사용 가능 여부 : ITEM_FLAG_UPGRADE && !ITEM_FLAG_COMPOSITE
	bool CanBloodGem() const
	{
		if (!m_itemProto->CanBloodGem())
			return false;
		return true;
	}

	// 착용 가능 검사
	bool CanWear() const
	{
		if (m_itemProto->m_wearing == -1)
			return false;

		// 착용 아이템 불가능
		if (m_wearPos != WEARING_NONE)
			return false;

		// 결합된 일반 아이템 불가능
		if ((m_flag & FLAG_ITEM_COMPOSITION) && !(m_itemProto->m_flag & ITEM_FLAG_COMPOSITE))
			return false;
		
		// 봉인된 아이템 작용 불가능
		if( m_flag & FLAG_ITEM_SEALED )		
			return false;

		return true;
	}

	bool IsLent() const
	{
		if (m_flag & FLAG_ITEM_LENT)
			return true;
		else
			return false;
	}

#ifdef SET_ITEM
	bool IsBelong() const
	{
		if (m_flag & FLAG_ITEM_BELONG)
			return true;
		return false;
	}
	bool IsOriginItem() const			{	return m_itemProto->IsOriginItem();			}
	int GetBelongType() const			{	return m_itemProto->GetBelongType();		}
	int GetOriginIndex(int nPos) const	{   return m_itemProto->GetOriginIndex(nPos);	}
	int GetOriginLevel(int nPos) const	{   return m_itemProto->GetOriginLevel(nPos);	}
	int GetOriginOptionCnt() const		{   return m_itemProto->GetOriginOptionCnt();	}
	int GetOriginSkillCnt() const		{   return m_itemProto->GetOriginSkillCnt();	}
#endif

#ifdef MONSTER_RAID_SYSTEM
	bool IsRareItem() const
	{
		return m_itemProto->IsRareItem();
	}

	void IdentifyRareOption();

#endif // MONSTER_RAID_SYSTEM

	int GetItemLevel(bool bOriginalLevel) const;
	int GetItemPlus() const
	{
#ifdef SET_ITEM
		return ((m_plus2 << 16) & 0xffff0000) | (m_plus & 0x0000ffff);
#else
		return m_plus;
#endif
	}

	// 의상과 결합 가능한 장비인가?
	bool CanCompositeEquipItem() const
	{
		if (!IsWeaponType() && !IsArmorType())
			return false;
#if defined (MONSTER_RAID_SYSTEM) && defined (COMPOSITE_RARE_ITEM)
		if (IsRareItem() && CanUpgrade())
			return true;
#endif // #if defined (MONSTER_RAID_SYSTEM) && defined (COMPOSITE_RARE_ITEM)
		if (CanBloodGem() && CanUpgrade())
			return true;
#ifdef SET_ITEM
		if (IsOriginItem() && CanUpgrade())
			return true;
#endif // SET_ITEM
		return false;
	}
};


class CItemList  
{
	CMutexObject	m_mutexobj;

public:
	CItemList();
	virtual ~CItemList();

	// 아이템 리스트 읽기
	bool Load();

	// 아이템 인덱스로 아이템 프로토타입 얻기
	CItemProto* FindIndex(int idx);

	// DB 의 인벤 등 아이템을 복원
	CItem* CreateDBItem(int idx, char wearPos, int plus, int flag, int used, 
#ifdef COMPOSITE_TIME
		int used_2,
#endif	// COMPOSITE_TIME
		const char* serial, LONGLONG count, short* option);

	CItem* CreateAutoGiveDBItem(int idx, char wearPos, int plus, int flag, int used, 
#ifdef COMPOSITE_TIME
		int used_2,
#endif	// COMPOSITE_TIME
		const char* serial, LONGLONG count, short* option);

	// 새로운 아이템 생성
	CItem* CreateItem(int idx, char wearPos, int plus, int flag, LONGLONG count);

	int				m_nCount;		// 개수
	int				m_virtualIndex;	// 가상
	CItemProto*		m_protoItems;	// 기본아이템

	// 특정 아이템 프로토타입들
	CItemProto*		m_moneyItem;			// 나스 아이템 (돈)
	CItemProto*		m_normalRefineItem;		// 일반 제련석
	CItemProto*		m_specialRefineItem;	// 고급 제련석
	CItemProto*		m_sTicketItem1;			// 싱글던전1 입장권
	CItemProto*		m_sTicketItem2;			// 싱글던전2 입장권

	// 무기 대여용 아이템 리스트
	CLCList<int>	m_listLendItem;

protected:
	static int CompIndex(const void* p1, const void* p2)
	{
		CItemProto* i1 = (CItemProto*)p1;
		CItemProto* i2 = (CItemProto*)p2;

		return i1->m_index - i2->m_index;
	}

	int GetNextVirtualIndex()
	{
		m_virtualIndex++;
		if (m_virtualIndex < 1)
			m_virtualIndex = 1;
		return m_virtualIndex;
	}
};

class CCtItemList
{
	friend class CItem;
public:
	CCtItemList() { ctItem = NULL; m_count = 0; }
	virtual ~CCtItemList()
	{
		if( ctItem )
			delete ctItem;
		ctItem = NULL;
	}

	bool Load(int ctIdx);

	CItem*			GetItem(int i)		
	{ 
		return this->ctItem[i].m_itemProto ? &this->ctItem[i] : NULL; 
	}

	CItem*			GetItem(CItem* pItem);
	int				GetCount()			{ return m_count; }

private:

	CItem *ctItem;
	int m_count;
};


class CCatalog
{
	friend class CCtItemList;
public:
	CCatalog()
	: m_name(MAX_ITEM_NAME_LENGTH + 1)
	{
		m_index = 0;
		m_type = -1;
		m_cash = 0;
		m_mileage = 0;
		m_flag = 0;
		m_ctItemList = NULL;
		m_enable = false;
	}

	CCatalog(int index)		// 바이너리 서치용 
	: m_name(MAX_ITEM_NAME_LENGTH + 1)
	{
		m_index = index;
		m_type = -1;
		m_cash = 0;
		m_mileage = 0;
		m_ctItemList = NULL;
		m_flag = 0;
		m_enable = false;
	}

	CCatalog(int index, const char* name, int type, int cash, int mileage, bool enable, int m_flag)
	: m_name(MAX_ITEM_NAME_LENGTH + 1)
	{
		m_index = index;
		m_name = name;
		m_type = type;
		m_cash = cash;
		m_mileage = mileage;
		m_ctItemList = NULL;
		m_flag = 0;
		m_enable = enable;
	}
	virtual ~CCatalog()
	{
		if( m_ctItemList)
			delete m_ctItemList;
		m_ctItemList = NULL;
	}

	void SetCatalog(int index, const char* name, int type, int cash, int mileage, bool enable, int flag, CCtItemList* ctItemList)
	{
		m_index = index;
		m_name = name;
		m_type = type;
		m_cash = cash;
		m_mileage = mileage;
		m_ctItemList = NULL;
		m_enable = enable;
		m_flag = flag;
		m_ctItemList = ctItemList;
#ifdef LIMIT_CATALOG
		CATALOG_FALG_TO_LIMIT(flag, m_toSell);
#endif
	}

	int		GetIndex()			{ return m_index; }
	const char*	GetName()		{ return m_name; }
	int		GetType()			{ return m_type; }
	int		GetCash()			{ return m_cash; }
	int		GetMileage()		{ return m_mileage; }
#ifdef LIMIT_CATALOG
	int		GetToSell()			{ return m_toSell; }
	void	SetToSell(int toSell) { m_toSell = toSell; }
	int		GetLimit()			{ int limit; CATALOG_FALG_TO_LIMIT(m_flag, limit); return limit;}
#endif
	bool	GetEnable()			{ return m_enable; }
	CCtItemList* GetCtItemList()	{return m_ctItemList; }

private:
	CCtItemList* m_ctItemList;

	int		m_index;		// 카탈로그 인덱스
	CLCString m_name;
	int		m_type;			// 타입
	int		m_cash;			// 가격
	int		m_mileage;		// 마일리지
#ifdef LIMIT_CATALOG
	int		m_toSell;			// 판매 남은 갯수
#endif
	int		m_flag;			
	bool	m_enable;
	
};

class CCatalogList
{
	friend class CCatalog;
public:
	CCatalogList() { catalog = NULL; m_cCount = 0; m_recoveryCtid = -1;}
	virtual ~CCatalogList()
	{
		this->catalog = NULL;
		delete catalog;
	}
	bool Load();

	CCatalog* Find(int idx) 
	{
		CCatalog s(idx);

		return (CCatalog*)bsearch(&s, catalog, m_cCount, sizeof(CCatalog), CompIndex);
	}

	CCatalog* FindItemToCatalog(CItem* pItem);
	CCatalog* FindItemToCatalog(int itemDBIndex);
	void GetCashMoonCatalogList(CLCList<int> &cashMoonCatalogList, double minCash, double maxCash);

	int GetCount() { return m_cCount; }
	CCatalog* getCatalog(int index) { return &catalog[index] ? &catalog[index] : NULL; }

	// 대형 퀵 체력 회복물약
	int m_recoveryCtid;
private:
	CCatalog* catalog;

	int m_cCount;

	static int CompIndex(const void* p1, const void* p2)
	{
		CCatalog * i1 = (CCatalog*) p1;
		CCatalog * i2 = (CCatalog*) p2;

		return i1->GetIndex() - i2->GetIndex();
	}
};


#endif // !defined(AFX_ITEM_H__91327288_815B_4A26_AF31_0F9098AEECF3__INCLUDED_)
