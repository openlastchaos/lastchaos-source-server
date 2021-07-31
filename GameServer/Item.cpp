// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include "Item.h"
#include "DBCmd.h"
#include "Server.h"
#include "Log.h"
#include "Exp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItem::CItem()
: m_serial(MAX_SERIAL_LENGTH + 1)
{
	//////////////////
	// DB 정보 변수
	m_itemProto = NULL;
	
	/////////////////////
	// 인벤토리 위치 관련
	m_tab_idx = -1;								// 탭
	m_row_idx = -1;								// 줄번호
	m_col_idx = -1;								// 칸 번호
	
	//////////////
	// 아이템 속성
	m_index = -1;								// 가상 인덱스
	m_idNum = -1;								// DB 인덱스
	m_wearPos = -1;								// 착용 안한 상태
	m_plus = 0;									// + 수치
	m_flag = 0;									// 속성
	m_count = 0;								// 개수
	m_used = -1;								// 내구도
#ifdef COMPOSITE_TIME
	m_used_2 = -1;								// 결합 시간
#endif	// COMPOSITE_TIME
	
	////////////
	// 기타 속성
	m_pArea = NULL;
	m_pos = CPos(0, 0, 0, 0, 0);				// 좌표
	m_cellX = NO_CELL;							// 셀 위치
	m_cellZ = NO_CELL;
	m_pCellPrev = NULL;							// 셀 링크드 리스트
	m_pCellNext = NULL;
	
	m_groundPulse = 0;							// 아이템이 떨어진 시간 저장
	m_preferenceIndex = -1;						// 이 아이템에 우선권이 있는 pc의 인덱스 저장 없으면 -1

	m_nOption = 0;								// 아이템 옵션 수

	m_nCompositeItem = -1;

#ifdef MONSTER_RAID_SYSTEM
	m_pRareOptionProto = NULL;
	m_nRareOptionIndex = 0;
	m_nRareOptionBit = 0;
#endif // MONSTER_RAID_SYSTEM

#ifdef SET_ITEM
	m_plus2 = 0;
	m_pSetItemProto = 0;
	m_nSetWearCnt = 0;
	m_nSetOption = 0;
#endif
}

CItem::~CItem()
{
	m_itemProto = NULL;
#ifdef TEST_SERVER
	if (m_tab_idx >= 0)
	{
		printf("%d\n", ((CItem*)NULL)->m_index);
	}
#endif // TEST_SERVER
}

#ifdef MONSTER_RAID_SYSTEM
void CItem::IdentifyRareOption()
{
	if (m_nOption)
		return ;

	int nRandom = GetRandom(1, 10000);
	int i;
	for (i = 0; i < MAX_RARE_OPTION_ITEM; i++)
	{
		if (m_itemProto->m_nRareIndex[i] > 0)
		{
			if (nRandom <= m_itemProto->m_nRareProb[i])
			{
				const CRareOptionProto* pRareProto = gserver.m_rareOptionList.Find(m_itemProto->m_nRareIndex[i]);
				if (pRareProto)
				{
					int nRareIndex = pRareProto->GetIndex();
					int nRareBit = 0;
					int j;
					for (j = 0; j < MAX_RARE_OPTION_SETTING; j++)
					{
						const RAREOPTIONDATA* rod = pRareProto->GetData(j);
						if (rod && rod->optiontype != -1)
						{
							if (GetRandom(1, 10000) <= rod->optionprob)
							{
								nRareBit |= rod->rarebit;
							}
						}
					}

					m_nOption = 2;
					m_option[0].GetDBValue((short)nRareIndex);
					m_option[1].GetDBValue((short)nRareBit);
					m_pRareOptionProto = pRareProto;
					m_nRareOptionIndex = nRareIndex;
					m_nRareOptionBit = nRareBit;
					return ;
				}
			}
			nRandom -= m_itemProto->m_nRareProb[i];
		}
	}
}
#endif // MONSTER_RAID_SYSTEM

int CItem::GetItemLevel(bool bOriginalLevel) const
{
	int nItemLevel = m_itemProto->GetItemProtoLevel();
	int i;

#ifdef LEVELDPWN_SCROLL		
	nItemLevel += m_nLevel;
	if (nItemLevel < 1)
		nItemLevel = 1;
#endif	// LEVELDPWN_SCROLL

	if (m_nOption == 0
#ifdef SET_ITEM
		&& !IsOriginItem()
#endif
		)
	{
		return nItemLevel;
	}


#ifdef MONSTER_RAID_SYSTEM
	if (IsRareItem())
	{
		for (i = 0; i < MAX_RARE_OPTION_SETTING; i++)
		{
			const RAREOPTIONDATA* rod = m_pRareOptionProto->GetData(i);
			if (m_nRareOptionBit & rod->rarebit)
			{
				switch (rod->optiontype)
				{
				case OPTION_DOWN_LIMITLEVEL:
					if (m_itemProto->GetItemProtoLevel() - rod->optionval < nItemLevel)
						nItemLevel = m_itemProto->GetItemProtoLevel() - rod->optionval;
					break;
				default:
					break;
				}
			}
		}
	}
	else
#endif // MONSTER_RAID_SYSTEM
#ifdef SET_ITEM
	if (IsOriginItem())
	{
		for (i = 0; i < MAX_ORIGIN_OPTION; i++)
		{
			switch (m_itemProto->GetOriginIndex(i))
			{
			case OPTION_DOWN_LIMITLEVEL:
				{
					COptionProto * pOptionProto = gserver.m_optionProtoList.FindProto(m_itemProto->GetOriginIndex(i));
					int nLevel = m_itemProto->GetOriginLevel(i);
					int nValue = 0;
					if (pOptionProto && pOptionProto->m_levelValue[nLevel - 1])
					{
						nValue = pOptionProto->m_levelValue[nLevel -1];
					}

					if (m_itemProto->GetItemProtoLevel() - nValue < nItemLevel)
						nItemLevel = m_itemProto->GetItemProtoLevel() - nValue;
				}
				break;
			default:
				break;
			}
		}
	}
	else
#endif // SET_ITEM
	{
		for (i = 0; i < MAX_WEAPON_OPTION; i++)
		{
			if (m_option[i].m_level <= 0)
				continue;
			switch (m_option[i].m_type)
			{
			case OPTION_DOWN_LIMITLEVEL:
				if (m_itemProto->GetItemProtoLevel() - m_option[i].m_value < nItemLevel)
					nItemLevel = m_itemProto->GetItemProtoLevel() - m_option[i].m_value;
				break;
			default:
				break;
			}
		}
	}

#ifdef LEVELDPWN_SCROLL		
	nItemLevel += m_nLevel;
#endif	// LEVELDPWN_SCROLL

	if (nItemLevel < 1)
		nItemLevel = 1;

	return nItemLevel;
}

CItemProto::CItemProto()
: m_name(MAX_ITEM_NAME_LENGTH + 1)
{
	m_index = -1;
	m_typeIdx = -1;
	m_subtypeIdx = -1;
	m_jobFlag = 0;
	m_flag = 0;
	m_maxUse = -1;
	m_fame = -1;

	m_num0 = 0;
	m_num1 = 0;
	m_num2 = 0;
	m_num3 = 0;
	m_num4 = 0;

	m_weight = 0;
	m_level = 0;
	m_price = 0;
	m_wearing = -1;

	memset(m_materialIndex, -1, sizeof(int) * MAX_MAKE_ITEM_MATERIAL);
	memset(m_materialCount, 0, sizeof(int) * MAX_MAKE_ITEM_MATERIAL);

	m_needSSkillIndex = -1;
	m_needSSkillLevel = 0;

	m_needSSkillIndex2 = -1;
	m_needSSkillLevel2 = 0;
	
#ifdef SET_ITEM
	m_set4 = 0;
#endif
}

CItemProto::~CItemProto()
{
	
}

//////////////////////////////////////////////////////////////////////
// CItemList Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemList::CItemList()
: m_listLendItem(NULL)
{
	m_protoItems = NULL;
	m_virtualIndex = 1;
	m_nCount = 0;
	
	m_moneyItem = NULL;
	m_normalRefineItem = NULL;
	m_specialRefineItem = NULL;

	m_sTicketItem1 = NULL;
	m_sTicketItem2 = NULL;
}

CItemList::~CItemList()
{
	if (m_protoItems)
		delete [] m_protoItems;
	
	m_protoItems = NULL;
	m_nCount = 0;
}

bool CItemList::Load()
{
	bool bRet = true;

	if (m_protoItems)
		delete [] m_protoItems;
	
	m_protoItems = NULL;
	m_nCount = 0;
	
	CDBCmd DBItem;
	DBItem.Init(&gserver.m_dbdata);
#ifdef LC_GER
	sprintf(g_buf, "SELECT * FROM t_item WHERE a_enable=1 and a_level <= %d ORDER BY a_index", MAX_LEVEL);
#else
	strcpy(g_buf, "SELECT * FROM t_item WHERE a_enable=1 ORDER BY a_index");
#endif // 
	DBItem.SetQuery(g_buf);
	if (!DBItem.Open())
		return false;
	if (DBItem.m_nrecords == 0)
		return false;
	
	m_nCount = DBItem.m_nrecords;
	m_protoItems = new CItemProto[m_nCount];
	
	int ridx = 0;
	while (DBItem.MoveNext())
	{
		int i;
		int nFlag;
		*g_buf2 = '\0';

		DBItem.GetRec("a_flag",			nFlag);
#ifdef MONSTER_RAID_SYSTEM
		if ( (nFlag & (ITEM_FLAG_RARE | ITEM_FLAG_COUNT)) == (ITEM_FLAG_RARE | ITEM_FLAG_COUNT) )
			continue ;
#else // MONSTER_RAID_SYSTEM
		if (nFlag & ITEM_FLAG_RARE)
			continue ;
#endif // MONSTER_RAID_SYSTEM
		m_protoItems[ridx].m_flag = nFlag;
		
		DBItem.GetRec("a_index",		m_protoItems[ridx].m_index);
		DBItem.GetRec("a_type_idx",		m_protoItems[ridx].m_typeIdx);
		DBItem.GetRec("a_subtype_idx",	m_protoItems[ridx].m_subtypeIdx);
		DBItem.GetRec(ITEM_NAME_FIELD,	m_protoItems[ridx].m_name, true);
		DBItem.GetRec("a_job_flag",		m_protoItems[ridx].m_jobFlag);
		DBItem.GetRec("a_num_0",		m_protoItems[ridx].m_num0);
		DBItem.GetRec("a_num_1",		m_protoItems[ridx].m_num1);
		DBItem.GetRec("a_num_2",		m_protoItems[ridx].m_num2);
		DBItem.GetRec("a_num_3",		m_protoItems[ridx].m_num3);
		DBItem.GetRec("a_num_4",		m_protoItems[ridx].m_num4);
		DBItem.GetRec("a_weight",		m_protoItems[ridx].m_weight);
		DBItem.GetRec("a_level",		m_protoItems[ridx].m_level);
		DBItem.GetRec(ITEM_PRICE_FIELD,	m_protoItems[ridx].m_price);
		DBItem.GetRec("a_wearing",		m_protoItems[ridx].m_wearing);
		DBItem.GetRec("a_max_use",		m_protoItems[ridx].m_maxUse);
		DBItem.GetRec("a_fame",			m_protoItems[ridx].m_fame);
#ifdef SET_ITEM
		DBItem.GetRec("a_set_4",		m_protoItems[ridx].m_set4);
#endif

		for (i=0; i < MAX_MAKE_ITEM_MATERIAL; i++)
		{
			strcpy(g_buf2, "a_need_item");				IntCat(g_buf2, i, false);
			DBItem.GetRec(g_buf2,		m_protoItems[ridx].m_materialIndex[i]);

			strcpy(g_buf2, "a_need_item_count");		IntCat(g_buf2, i, false);
			DBItem.GetRec(g_buf2,		m_protoItems[ridx].m_materialCount[i]);
		}

		DBItem.GetRec("a_need_sskill",			m_protoItems[ridx].m_needSSkillIndex);
		DBItem.GetRec("a_need_sskill_level",	m_protoItems[ridx].m_needSSkillLevel);

		DBItem.GetRec("a_need_sskill2",			m_protoItems[ridx].m_needSSkillIndex2);
		DBItem.GetRec("a_need_sskill_level2",	m_protoItems[ridx].m_needSSkillLevel2);

#if defined (MONSTER_RAID_SYSTEM)  || defined (SET_ITEM)
		for (i=0; i < MAX_RARE_OPTION_ITEM; i++)
		{
			sprintf(g_buf2, "a_rare_index_%d", i);
			if (!DBItem.GetRec(g_buf2,		m_protoItems[ridx].m_nRareIndex[i]))
				return false;

			sprintf(g_buf2, "a_rare_prob_%d", i);
			if (!DBItem.GetRec(g_buf2,		m_protoItems[ridx].m_nRareProb[i]))
				return false;
		}
#endif // MONSTER_RAID_SYSTEM

		// 특정 아이템 찾기 : 타입으로
		switch (m_protoItems[ridx].m_typeIdx)
		{
		case ITYPE_ETC:					// 기타
			{
				switch (m_protoItems[ridx].m_subtypeIdx)
				{
				case IETC_MONEY:		// 나스
					m_moneyItem = m_protoItems + ridx;
					break;
				}
			}
			break;
		}
		
		// 특정 아이템 찾기 : 인덱스로
		switch (m_protoItems[ridx].m_index)
		{
		case 84:	// 일반 제련석
			m_normalRefineItem = m_protoItems + ridx;
			break;

		case 85:	// 고급 제련석
			m_specialRefineItem = m_protoItems + ridx;
			break;

		case 141:	// 싱글던전2 입장권
			m_sTicketItem2 = m_protoItems + ridx;
			break;

		case 142:	// 싱글던전1 입장권
			m_sTicketItem1 = m_protoItems + ridx;
			break;
			
		default:
			break;
		}

		// 무기 대여용 아이템 리스트 만들기
		// 무기 타입 중
		// ITEM_FLAG_LENT
		// 가격이 설정 되어 있을 때
		// 레벨 37/41/45/49/53/57/61/65
		static const int nNormalWeaponFlag = ITEM_FLAG_LENT;
		if (m_protoItems[ridx].m_typeIdx == ITYPE_WEAPON && (m_protoItems[ridx].m_flag & nNormalWeaponFlag) == nNormalWeaponFlag && m_protoItems[ridx].m_price > 0)
		{
			switch (m_protoItems[ridx].m_subtypeIdx)
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
				switch (m_protoItems[ridx].m_level)
				{
				case 37:
				case 41:
				case 45:
				case 49:
				case 53:
				case 57:
				case 61:
				case 65:
					m_listLendItem.AddToTail(m_protoItems[ridx].m_index);
					break;

				default:
					break;
				} // 레벨
				break;

			case IWEAPON_MINING:
			case IWEAPON_GATHERING:
			case IWEAPON_CHARGE:
			default:
				break;
			} // 서브 타입
		} // 무기 && 일반플래그

		if (m_protoItems[ridx].m_price < 0)
		{
			GAMELOG << init("ERROR: ITEM PRICE")
					<< m_protoItems[ridx].m_index << delim
					<< m_protoItems[ridx].m_name << delim
					<< m_protoItems[ridx].m_price
					<< end;
			bRet = false;
		}
		
		ridx++;
	}

	m_nCount = ridx;

	return bRet;
}

CItemProto* CItemList::FindIndex(int idx)
{
	CItemProto proto;
	proto.m_index = idx;
	
	return (CItemProto*)bsearch(&proto, m_protoItems, m_nCount, sizeof(CItemProto), CompIndex);
}

CItem* CItemList::CreateDBItem( int idx, char wearPos, int plus, int flag, int used, 
#ifdef COMPOSITE_TIME
								int used_2,
#endif	// COMPOSITE_TIME
							   const char* serial, LONGLONG count, short* option)
{
	CMutex mutex(&m_mutexobj);

	if (count == 0)
		return NULL;
	
	if (idx == 0)
		idx = gserver.m_itemProtoList.m_moneyItem->m_index;
	
	// 프로토 타입 찾기
	CItemProto* proto = FindIndex(idx);
	if (!proto)
	{
		return NULL;
	}
	
	// 아이템 설정
	CItem* item = new CItem;
	
	item->m_itemProto = proto;
	
	item->m_tab_idx = -1;
	item->m_row_idx = -1;
	item->m_col_idx = -1;
	
	item->m_index = GetNextVirtualIndex();
	item->m_idNum = proto->m_index;
	item->m_wearPos = wearPos;
#ifdef SET_ITEM
	item->m_plus  = (plus & 0x0000ffff);;
	item->m_plus2 = ((plus >> 16) & 0x0000ffff);;
#else
	item->m_plus = plus;
#endif
	item->m_flag = flag;
	item->m_used = used;
#ifdef COMPOSITE_TIME
	item->m_used_2 = used_2;
#endif	// COMPOSITE_TIME	
	item->m_serial = serial;
	item->m_count = count;

	bool bRareItem = false;

#ifdef MONSTER_RAID_SYSTEM
	if (item->IsRareItem())
		bRareItem = true;
#endif // MONSTER_RAID_SYSTEM

	if (bRareItem)
	{
#ifdef MONSTER_RAID_SYSTEM
		if (option[1] != 0)
		{
			item->m_pRareOptionProto = gserver.m_rareOptionList.Find(option[0]);
			if (item->m_pRareOptionProto)
			{
				item->m_nRareOptionIndex = option[0];
				item->m_nRareOptionBit = option[1];
				item->m_nOption = 2;
				item->m_option[0].GetDBValue(option[0]);
				item->m_option[1].GetDBValue(option[1]);
			}
		}
#endif // MONSTER_RAID_SYSTEM
	}
#ifdef SET_ITEM
	else if (item->IsOriginItem())
	{
		item->m_pSetItemProto = gserver.m_setItemProtoList.Find(item->m_itemProto->GetSetIndex());
	}
#endif // SET_ITEM

	else
	{
		// Option Setting
		int i;
		for (i=0; i < MAX_ITEM_OPTION; i++)
		{
			item->m_option[i].GetDBValue(option[i]);

			if (item->m_option[i].m_level <= 0)
				continue;

			item->m_option[i].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[i].m_type);
			
			if (!item->m_option[i].m_proto)
				continue;

			item->m_nOption++;
			item->m_option[i].m_value = item->m_option[i].m_proto->m_levelValue[item->m_option[i].m_level - 1];
		}
	}

	if( ( proto->m_index == 2047 || proto->m_index == 2633 ) 
		&& item->m_used == -1)					// 초코수집상자 or 새해 소망상자 : EVENT_VALENTINE_2007 LOVELOVE_2008
	{
		item->m_used = 0;
	}
	
#ifdef EVENT_COLLECT_BUG
	if(proto->m_index == 1577 || proto->m_index == 1578)
	{	// 곤충 채집 상자들은 m_used값을 다른방법으로 사용하므로 
		// -1로 생성시 0으로 초기화
		if(item->m_used == -1)
		{
			item->m_used = 0;
		}
	}
#endif // EVENT_COLLECT_BUG

	return item;
}

CItem* CItemList::CreateItem(int idx, char wearPos, int plus, int flag, LONGLONG count)
{
	CMutex mutex(&m_mutexobj);

	if (count == 0)
		return NULL;
	
	if (idx == 0)
		idx = gserver.m_itemProtoList.m_moneyItem->m_index;
	
	// 프로토 타입 찾기
	CItemProto* proto = FindIndex(idx);
	if (!proto)
	{
		GAMELOG << init("SYS_ERR")
			<< "Can't Create Item Index"
			<< idx << delim
			<< plus << delim
			<< flag << delim
			<< count
			<< end;
		return NULL;
	}
	
	CItem* item = new CItem;
	
	item->m_itemProto = proto;
	
	item->m_tab_idx = -1;
	item->m_row_idx = -1;
	item->m_col_idx = -1;
	
	item->m_index = GetNextVirtualIndex();
	item->m_idNum = proto->m_index;
	item->m_wearPos = wearPos;
	item->m_plus = plus;
	item->m_flag = flag;

	// 내구도 추가
	item->m_used = proto->m_maxUse;

#ifdef SET_ITEM
	if (item->IsOriginItem())
	{
		//int nRandom = GetRandom(item->m_itemProto->m_num0, item->m_itemProto->m_num1);
		//item->m_plus2 = nRandom - item->m_itemProto->m_num0;
		item->m_plus2 = 0;		// 제작 시스템 구현 시 랜덤 적용
	}
#endif // SET_ITEM

#ifdef EVENT_COLLECT_BUG
	if(proto->m_index == 1577 || proto->m_index == 1578)
	{	// 곤충 채집 상자들은 m_used값을 다른방법으로 사용하므로 
		// 생성시 0으로 초기화
		item->m_used = 0;
	}
#endif // EVENT_COLLECT_BUG


	GetSerial(item->m_serial, item->m_index);
	if (proto->m_flag & ITEM_FLAG_COUNT)
		item->m_count = count;
	else
		item->m_count = 1;
	// 060227 : bs : 파티리콜
	if (proto->m_typeIdx == ITYPE_ONCE && proto->m_subtypeIdx == IONCE_WARP && proto->m_num0 == IONCE_WARP_PARTYRECALL)
	{
#ifdef PARTY_RECALL_NEW
		item->m_used = -1;
#else
		item->m_used = gserver.m_gameTime + proto->m_num3 * 60 * 60;
#endif	// PARTY_RECALL_NEW
	}

// 창고 이용 주문서
#ifdef USE_WAREHOUSE
	if( proto->m_typeIdx == ITYPE_ONCE && proto->m_subtypeIdx == IONCE_CASH && proto->m_num3 > 0)
		item->m_used = gserver.m_gameTime + proto->m_num3 * 60 * 60;
#endif // USE_WAREHOUSE

	// 잡화상 이용 주문서
#ifdef USE_GROCERY
	if( proto->m_typeIdx == ITYPE_ONCE && proto->m_subtypeIdx == IONCE_CASH && proto->m_num3 > 0)
		item->m_used = gserver.m_gameTime + proto->m_num3 * 60 * 60;
#endif // USE_GROCERY

	// 써치 라이프
#ifdef SHOW_HP
	if( proto->m_typeIdx == ITYPE_ONCE && proto->m_subtypeIdx == IONCE_CASH && proto->m_num3 > 0)
		item->m_used = gserver.m_gameTime + proto->m_num3 * 60 * 60;
#endif // SHOW_HP

#ifdef	AUTO_POTION
	if (proto->m_index == ONE_PERIOD_ITEM || proto->m_index == 2610 )
	{
		item->m_used = gserver.m_gameTime + 1 * 24 * 60 * 60;
	}

	if (proto->m_index == SEVEN_PERIOD_ITEM)
	{
		item->m_used = gserver.m_gameTime + 7 * 24 * 60 * 60;
	}

	if (proto->m_index == THIRTY_PERIOD_ITEM)
	{
		item->m_used = gserver.m_gameTime + 30 * 24 * 60 * 60;
	}
#endif	// AUTO_POTION

	// 유료 아이템 : num4에 일단위
	if (proto->m_flag & ITEM_FLAG_COMPOSITE)
	{
#ifdef CHARGE_ITEM_PERIOD
		int nItemNo = item->m_idNum;
		if( ( 1631 < nItemNo && nItemNo < 1704 ) || ( 1721 < nItemNo && nItemNo < 1794 ) )
		{
			item->m_used = gserver.m_gameTime + 7 * 24 * 60 * 60;
		} 
		else
#endif	// CHARGE_ITEM_PERIOD
		{
			item->m_used = gserver.m_gameTime + proto->m_num4 * 24 * 60 * 60;
		}
	}
	
#ifdef COMPOSITE_TIME
	if (proto->m_flag & ITEM_FLAG_ABS)
	{	// 시간제 아이템은 m_num4에 시간이 들어간다
		item->m_used = gserver.m_gameTime + proto->m_num4 * 60 * 60;
	}
#endif // COMPOSITE_TIME

	// 골든볼 이벤트 응모권 : GOLDENBALL_CARD_INDEX
	// used에 경기 시작 시간(응모 마감 시간)을 저장
	if (proto->m_index == GOLDENBALL_CARD_INDEX)
	{
		item->m_used = gserver.m_clGoldenBall.GetVoteEndTime();
	}

	if (item->IsLent())
	{
		item->m_used = gserver.m_gameTime + 24 * 60 * 60;
	}

	if( proto->m_index == 2047 || proto->m_index == 2633 )	// 초코수집상자 or 새해 소망상자 : EVENT_VALENTINE_2007 
	{
		item->m_used = 0;
	}

	if (proto->m_index == 2137)		// gpara 악세
	{
		short nGparaOption[5] = {
			((20 << OPTION_VALUE_SHIFT) & 0xff00) | (1 & 0x00ff),
			((21 << OPTION_VALUE_SHIFT) & 0xff00) | (1 & 0x00ff),
			(( 7 << OPTION_VALUE_SHIFT) & 0xff00) | (1 & 0x00ff),
			((16 << OPTION_VALUE_SHIFT) & 0xff00) | (1 & 0x00ff),
			(( 8 << OPTION_VALUE_SHIFT) & 0xff00) | (1 & 0x00ff),
		};
		item->m_nOption = 0;
		int i;
		for (i=0; i < MAX_ITEM_OPTION; i++)
		{
			item->m_option[i].GetDBValue(nGparaOption[i]);

			if (item->m_option[i].m_level <= 0)
				continue;

			item->m_option[i].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[i].m_type);
			
			if (!item->m_option[i].m_proto)
				continue;

			item->m_nOption++;
			item->m_option[i].m_value = item->m_option[i].m_proto->m_levelValue[item->m_option[i].m_level - 1];
		}
	}

#ifdef LAKA_PRESSCORPS
	// hc 라카기자단 확성기 기간 30일
	if(proto->m_index == 2882)
	{
		item->m_used = gserver.m_gameTime + 30 * 24 * 60 * 60;
	}
#endif // LAKA_PRESSCORPS
	
#ifdef EVENT_PHOENIX   // 피닉스 이벤트  yhj
	// 피닉스 투구 아이템일 경우 기간 무제한
	if( proto->m_index >= 3006 && proto->m_index <= 3011 )
	{
		item->m_used = -1;
	}
#endif // EVENT_PHOENIX

// <--!
	/*
	if(NULL != item)
	{
		GAMELOG << init("ITEM CREATE")
			<< itemlog(item)
			<< end;
	}*/
// !-->	
	return item;
}

CItem* CItemList::CreateAutoGiveDBItem(int idx, char wearPos, int plus, int flag, int used, 
#ifdef COMPOSITE_TIME
		int used_2,
#endif	// COMPOSITE_TIME
		const char* serial, LONGLONG count, short* option)
{
	CMutex mutex(&m_mutexobj);

	if (count == 0)
		return NULL;
	
	if (idx == 0)
		idx = gserver.m_itemProtoList.m_moneyItem->m_index;
	
	// 프로토 타입 찾기
	CItemProto* proto = FindIndex(idx);
	if (!proto)
	{
		return NULL;
	}
	
	// 아이템 설정
	CItem* item = new CItem;
	
	item->m_itemProto = proto;
	
	item->m_tab_idx = -1;
	item->m_row_idx = -1;
	item->m_col_idx = -1;
	
	item->m_index = GetNextVirtualIndex();
	item->m_idNum = proto->m_index;
	item->m_wearPos = wearPos;
	item->m_plus = plus;
	item->m_flag = flag;
	item->m_used = used;
#ifdef COMPOSITE_TIME
	item->m_used_2 = used_2;
#endif	// COMPOSITE_TIME	
	item->m_serial = serial;
	item->m_count = count;
#ifdef SET_ITEM
	item->m_plus2 = 0;
#endif

	bool bRareItem = false;

#ifdef MONSTER_RAID_SYSTEM
	if (item->IsRareItem())
		bRareItem = true;
#endif // MONSTER_RAID_SYSTEM

	if (bRareItem)
	{
#ifdef MONSTER_RAID_SYSTEM
		if (option[1] != 0)
		{
			item->m_pRareOptionProto = gserver.m_rareOptionList.Find(option[0]);
			if (item->m_pRareOptionProto)
			{
				item->m_nRareOptionIndex = option[0];
				item->m_nRareOptionBit = option[1];
				item->m_nOption = 2;
				item->m_option[0].GetDBValue(option[0]);
				item->m_option[1].GetDBValue(option[1]);
			}
		}
#endif // MONSTER_RAID_SYSTEM
	}
	else
	{
		// Option Setting
		int i;
		for (i=0; i < MAX_ITEM_OPTION; i++)
		{
			item->m_option[i].GetDBValue(option[i]);

			if (item->m_option[i].m_level <= 0)
				continue;

			item->m_option[i].m_proto = gserver.m_optionProtoList.FindProto(item->m_option[i].m_type);
			
			if (!item->m_option[i].m_proto)
				continue;

			item->m_nOption++;
			item->m_option[i].m_value = item->m_option[i].m_proto->m_levelValue[item->m_option[i].m_level - 1];
		}
	}

#ifdef SET_ITEM
	if (item->IsOriginItem())
	{
		item->m_pSetItemProto = gserver.m_setItemProtoList.Find(item->m_itemProto->GetSetIndex());
	}
#endif

	if( ( proto->m_index == 2047 || proto->m_index == 2633 ) 
		&& item->m_used == -1)					// 초코수집상자 or 새해 소망상자 : EVENT_VALENTINE_2007 LOVELOVE_2008
	{
		item->m_used = 0;
	}
	
#ifdef EVENT_COLLECT_BUG
	if(proto->m_index == 1577 || proto->m_index == 1578)
	{	// 곤충 채집 상자들은 m_used값을 다른방법으로 사용하므로 
		// -1로 생성시 0으로 초기화
		if(item->m_used == -1)
		{
			item->m_used = 0;
		}
	}
#endif // EVENT_COLLECT_BUG

	// 유료 아이템 : num4에 일단위
	if (proto->m_flag & ITEM_FLAG_COMPOSITE)
	{
#ifdef CHARGE_ITEM_PERIOD
		int nItemNo = item->m_idNum;
		if( ( 1631 < nItemNo && nItemNo < 1704 ) || ( 1721 < nItemNo && nItemNo < 1794 ) )
		{
			item->m_used = gserver.m_gameTime + 7 * 24 * 60 * 60;
		} 
		else
#endif	// CHARGE_ITEM_PERIOD
		{
			item->m_used = gserver.m_gameTime + proto->m_num4 * 24 * 60 * 60;
		}
	}

#ifdef LAKA_PRESSCORPS
	// hc 라카기자단 확성기 기간 30일
	if(proto->m_index == 2882)
	{
		item->m_used = gserver.m_gameTime + 30 * 24 * 60 * 60;
	}
#endif // LAKA_PRESSCORPS

#ifdef EVENT_PHOENIX   // 피닉스 이벤트  yhj
	// 피닉스 투구 아이템일 경우 기간 무제한
	if( proto->m_index >= 3006 && proto->m_index <= 3011 )
	{
		item->m_used = -1;
	}
#endif // EVENT_PHOENIX

	return item;
}

CCatalog* CCatalogList::FindItemToCatalog(int itemDBIndex)
{
	int i;
	int count = this->GetCount();

	CCtItemList* itemList = NULL;
	for( i = 0; i < count; i++ )
	{
		if( !catalog[i].GetEnable() 
			|| catalog[i].GetType() == CTYPE_SET 
			|| catalog[i].GetType() == CTYPE_WEAPON
			|| catalog[i].GetType() == CTYPE_WEAR )
			continue;	

		itemList = this->catalog[i].GetCtItemList();
		if( itemList )
		{
			CItem* item = itemList->GetItem(0);
			if( !item || !item->m_itemProto )
				continue;

			if( item->m_itemProto->m_index == itemDBIndex && item->m_plus == 0 && item->m_flag == 0 && item->Count() == 1 )
				return &catalog[i];
		}
	}

	return NULL;
}

CCatalog* CCatalogList::FindItemToCatalog(CItem* pItem)
{
	int i;
	int count = this->GetCount();

	CCtItemList* itemList = NULL;
	for( i = 0; i < count; i++ )
	{

		if( !catalog[i].GetEnable() 
			|| catalog[i].GetType() == CTYPE_SET 
			|| catalog[i].GetType() == CTYPE_WEAPON
			|| catalog[i].GetType() == CTYPE_WEAR )
			continue;	

		itemList = this->catalog[i].GetCtItemList();
		if( itemList )
		{
			CItem* item = itemList->GetItem(0);

			if( !item || !item->m_itemProto || !pItem || !pItem->m_itemProto )
				continue;

			if( item->m_idNum == pItem->m_idNum && item->m_plus == pItem->m_plus && item->m_flag == pItem->m_flag && item->Count() == 1 )
				return &catalog[i];
		}
	}

	return NULL;
}

void CCatalogList::GetCashMoonCatalogList(CLCList<int> &cashMoonCatalogList, double minCash, double maxCash)
{
	if( minCash < 0 || maxCash < 0 || minCash > maxCash )
		return;

	int count = this->GetCount();

	for(int i = 0; i < count; i++)
	{
		if( !catalog[i].GetEnable() 
			|| catalog[i].GetType() == CTYPE_SET 
			|| catalog[i].GetType() == CTYPE_WEAPON
			|| catalog[i].GetType() == CTYPE_WEAR )
			continue;
		
		if( catalog[i].GetCash() >= minCash && catalog[i].GetCash() <= maxCash )
		{
			cashMoonCatalogList.AddToTail( catalog[i].GetIndex() );
		}
	}
}

bool CCatalogList::Load()
{

	if (this->catalog)
		delete [] this->catalog;
	
	this->catalog = NULL;
	this->m_cCount = 0;

	CDBCmd cmd;

	CLCString sql(4096);

#ifdef USE_TENTER_BILLING
	cmd.Init(&gserver.m_dbcatal);
	sql.Format("select * from t_catalog_%d order by a_ctid", GAMEID);
#else
	cmd.Init(&gserver.m_dbdata);
	sql.Format("select * from t_catalog order by a_ctid");
#endif
	cmd.SetQuery(sql);

	if( !cmd.Open() )
	{
		GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
		return false;
	}

	this->m_cCount = cmd.m_nrecords;
	this->catalog = new CCatalog[m_cCount];

	if( !cmd.MoveFirst() )
	{
		GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
		return false;
	}

	int i = 0;
	char enable = 0;

	do
	{
		//int		m_index;		// 카탈로그 인덱스
		//CLCString m_name;
		//int		m_type;			// 타입
		//int		m_cash;			// 가격
		//int		m_mileage;		// 마일리지
		//int		m_toSell;			// 판매 남은 갯수
		//int		m_flag;			
		//bool		m_enable;
		int index, type, cash, mileage, flag;
		CLCString name(MAX_ITEM_NAME_LENGTH+1);

		if( !cmd.GetRec("a_ctid", index)
		|| !cmd.GetRec("a_ctname", name)
		|| !cmd.GetRec("a_type", type)
		|| !cmd.GetRec("a_cash", cash)
		|| !cmd.GetRec("a_mileage", mileage) 
		|| !cmd.GetRec("a_flag", flag) 
		|| !cmd.GetRec("a_enable", enable)
		)
		{
			GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
			return false;
		}

		CCtItemList* ctItemList = new CCtItemList;
		ctItemList->Load(index);

		if(type != CTYPE_SET && ctItemList && ctItemList->GetCount() == 1 && ctItemList->GetItem(0) && ctItemList->GetItem(0)->m_itemProto->m_index == 855 && ctItemList->GetItem(0)->Count() == 1)
			m_recoveryCtid = i;

		catalog[i].SetCatalog(index, name, type, cash, mileage, enable == 1 ? true : false, flag, ctItemList);

#ifdef LIMIT_CATALOG

		if(catalog[i].GetToSell() > CATALOG_FLAG_MAX_LIMIT || catalog[i].GetToSell() < 0 )
		{
			GAMELOG << init("CATALOGLIST_LOAD_FLAG_LIMIT_ERROR") << end;
			return false;
		}
#if defined( LC_USA ) || defined( LC_TLD )
		if( catalog[i].GetToSell() != 0 || catalog[i].GetType() == CTYPE_PLATINUM )
#else
		if(catalog[i].GetToSell() != 0 )
#endif
		{
			gserver.m_limitCT.AddToTail(catalog[i].GetIndex());
		}
#endif

		i++;

	} while( cmd.MoveNext() );

	return true;
}

bool CCtItemList::Load(int ctIdx)
{

	if( this->ctItem )
		delete [] this->ctItem;

	this->ctItem = NULL;
	this->m_count = 0;

	int itemindex, itemflag, itemplus, itemnum;

	CLCString sql(4096);
	CDBCmd cmd;
#ifdef USE_TENTER_BILLING
	cmd.Init(&gserver.m_dbcatal);
	sql.Format("select * from t_ct_item_%d where a_ctid=%d", GAMEID, ctIdx);
#else
	cmd.Init(&gserver.m_dbdata);
	sql.Format("select * from t_ct_item where a_ctid=%d", ctIdx);
#endif
	cmd.SetQuery(sql);

	if( !cmd.Open() )
	{
		GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
		return false;
	}

	this->m_count = cmd.m_nrecords;
	this->ctItem = new CItem[m_count];

	if( !cmd.MoveFirst() )
	{
		GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
		return false;
	}

	int i =0;
	do
	{
		if( !cmd.GetRec("a_item_idx", itemindex)
		|| !cmd.GetRec("a_item_flag", itemflag)
		|| !cmd.GetRec("a_item_plus", itemplus)
		|| !cmd.GetRec("a_item_num", itemnum) )
		{
			GAMELOG << init("CATALOGLIST_LOAD_ERROR") << end;
			return false;
		}

		CItem *item = gserver.m_itemProtoList.CreateItem(itemindex, -1, itemplus, itemflag, itemnum);
		if( item )
		{
			memcpy(&ctItem[i], item, sizeof(CItem) );
			i++;
		}
		else
		{
			memset(&ctItem[i], '\0', sizeof(CItem) );
		}

	} while( cmd.MoveNext() );

	return true;
}

CItem* CCtItemList::GetItem(CItem* pItem)
{
	int i;
	int count = this->GetCount();

	for(i = 0; i < count; i++)
	{
		if( this->GetItem(i) && GetItem(i) == pItem )
		{
			return GetItem(i);
		}
	}

	return NULL;
}