// 대리 상인
#include "stdhdrs.h"
#include "Server.h"
#include "Character.h"
#include "CmdMsg.h"
#include "Log.h"
#include "doFunc.h"

#ifdef ALTERNATE_MERCHANT
// 생성자
//CAlternateMerchant::CAlternateMerchant( void ) : CCharacter()
//, m_strShopName(PS_MAX_SHOPNAME + 1)
////: m_strShopName(PS_MAX_SHOPNAME + 1)
//{
//	InitAlternateMerchant(); 
//	InitStat();
//}

CAlternateMerchant::CAlternateMerchant( char cShopType, const char* strShopName )
: m_strShopName(PS_MAX_SHOPNAME + 1)
{	

	InitAlternateMerchant();
	m_cShopType		= cShopType;
	m_strShopName	= strShopName;
}


// 아이템 추가
bool CAlternateMerchant::AddItem( bool bNormal, int nItemIndex, LONGLONG lItemCount, LONGLONG lItemdPrice )
{
	int i;

	if ( bNormal )
	{
		for ( i = 0; i < PS_MAX_NORMAL_ITEM; i++ )
		{
			if ( m_nNormalItemIndex[i] == -1 )
				break;
		}
		if ( i == PS_MAX_NORMAL_ITEM )
			return false;
		m_nNormalItemIndex[i] = nItemIndex;
		m_lNormalItemCount[i] = lItemCount;
		m_lNormalItemPrice[i] = lItemdPrice;
		return true;
	}
	else
	{
		for ( i = 0; i < PS_MAX_PACKAGE_ITEM; i++ )
		{
			if ( m_nPackageItemIndex[i] == -1 )
				break;
		}
		if ( i == PS_MAX_PACKAGE_ITEM )
			return false;
		m_nPackageItemIndex[i] = nItemIndex;
		m_lPackageItemCount[i] = lItemCount;
		m_lPackagePrice = lItemdPrice;
		return true;
	}
}

// 아이템 검색
CItem* CAlternateMerchant::FindItem( bool bNormal, int nItemIndex )
{
	int i;
	if ( bNormal )
	{
		for ( i = 0; i < PS_MAX_NORMAL_ITEM; i++ )
		{
			if ( m_NormalItems[i]->m_idNum == nItemIndex )
				return m_NormalItems[i];
		}
		return NULL;
	}
	else
	{
		for ( i = 0; i < PS_MAX_PACKAGE_ITEM; i++ )
		{
			if ( m_PackItems[i]->m_idNum == nItemIndex )
				return m_PackItems[i];
		}
		return NULL;
	}
	
}

// 아이템 삭제
void CAlternateMerchant::RemoveNormalItem( int nItemIndex, LONGLONG lItemCount )
{
	int i;
	for ( i = 0; i < PS_MAX_NORMAL_ITEM; i++ )
	{
		if ( m_nNormalItemIndex[i] == nItemIndex )
		{
			m_lNormalItemCount[i] -= lItemCount;
			if ( m_lNormalItemCount[i] <= 0 )
			{
				m_nNormalItemIndex[i] = -1;
				m_lNormalItemCount[i] = 0;
				m_lNormalItemPrice[i] = 0;
			}
			if ( GetNormalCount() == 0 && GetPackageCount() == 0 )
				m_cShopType = PST_NOSHOP;
			return ;
		}
	}
}

// 패키지 아이템 삭제
void CAlternateMerchant::RemovePackage( void )
{
	memset(m_nPackageItemIndex, -1, sizeof(int) * PS_MAX_PACKAGE_ITEM);
	memset(m_lPackageItemCount, 0, sizeof(LONGLONG) * PS_MAX_PACKAGE_ITEM);
	m_lPackagePrice = 0;
	if (GetNormalCount() == 0)
		m_cShopType = PST_NOSHOP;
	else
		m_cShopType = PST_SELL | PST_PREMIUM;
}

// 전체 판매 중인 일반 아이템 개수
char CAlternateMerchant::GetNormalCount( void )
{
	char cReturnValue = 0;
	int i;
	for (i = 0; i < PS_MAX_NORMAL_ITEM; i++)
	{
		if (m_nNormalItemIndex[i] != -1)
			cReturnValue++;
	}
	return cReturnValue;
}

// 일반 아이템 위치
int CAlternateMerchant::GetNextNormalItem( int nPos )
{
	nPos++;
	if (nPos < 0)
		return -1;
	while (nPos < PS_MAX_NORMAL_ITEM)
	{
		if (m_nNormalItemIndex[nPos] != -1)
			return nPos;
		else
			nPos++;
	}

	return -1;
}

// 전체 패키지 아이템 갯수
char CAlternateMerchant::GetPackageCount( void )
{
	char cReturnValue = 0;
	int i;
	for ( i = 0; i < PS_MAX_PACKAGE_ITEM; i++ )
	{
		if ( m_nPackageItemIndex[i] != -1 )
			cReturnValue++;
	}
	return cReturnValue;
}

// 패키지 아이템의 다음 아이템 가져 오기
int CAlternateMerchant::GetNextPackageItem( int nPos )
{
	nPos++;
	if (nPos < 0)
		return -1;
	while (nPos < PS_MAX_PACKAGE_ITEM)
	{
		if (m_nPackageItemIndex[nPos] != -1)
			return nPos;
		else
			nPos++;
	}

	return -1;
}


// 대리 상인 초기화
void	CAlternateMerchant::InitAlternateMerchant( void )
{	
	m_cShopType		= PST_NOSHOP;
	m_strShopName	= NULL;	
//	memset( m_strShopName, NULL, PS_MAX_SHOPNAME );
	memset(m_nNormalItemIndex, -1, sizeof(int) * PS_MAX_NORMAL_ITEM);
	memset(m_lNormalItemCount, 0, sizeof(LONGLONG) * PS_MAX_NORMAL_ITEM);
	memset(m_lNormalItemPrice, 0, sizeof(LONGLONG) * PS_MAX_NORMAL_ITEM);
	memset(m_nPackageItemIndex, -1, sizeof(int) * PS_MAX_PACKAGE_ITEM);
	memset(m_lPackageItemCount, 0, sizeof(LONGLONG) * PS_MAX_PACKAGE_ITEM);
	m_lPackagePrice = 0;
	m_lNasCount		= 0;
	m_nlistIdx		= -1;
}



// 대리 상점 나타나게 함
void	CAlternateMerchant::AppearAlternateMerchant( bool bIncludeOwner )
{
	CNetMsg rmsg;

	if (m_pShopOwner == NULL)
		return ;

//	if (IsSummon())
//		return ;

	CArea* area = m_pShopOwner->m_pArea;
	if (area == NULL)
		return ;

	// 소환할 수 있는 존일때
//	if (!area->m_zone->m_bCanSummonPet)
//	{
//		SysMsg(rmsg, MSG_SYS_PET_CANT_SUMMON);
//		SEND_Q(rmsg, GetOwner()->m_desc);
//		return ;
//	}
//
//	// 셀에 넣기
//	m_pZone = area->m_zone;
//	m_pArea = area;
//	m_pos = m_owner->m_pos;
//	int cx, cz;
//	area->PointToCellNum(GET_X(this), GET_Z(this), &cx, &cz);
//	area->CharToCell(this, GET_YLAYER(this), cx, cz);
//
//	// 나타남을 알림
//	AppearMsg(rmsg, this, true);
//	area->SendToCell(rmsg, this, bIncludeOwner);
//
//	m_bSummon = true;
}

// 대리 상점 사라지게 함
void	CAlternateMerchant::DisappearAlternateMerchant( void )
{
}

int		CAlternateMerchant::FindItemPos( bool bNormal, int nItemIndex )
{
	int i;
	if (bNormal)
	{
		for (i = 0; i < PS_MAX_NORMAL_ITEM; i++)
		{
			if (m_nNormalItemIndex[i] == nItemIndex)
				return i;
		}
		return -1;
	}
	else
	{
		for (i = 0; i < PS_MAX_PACKAGE_ITEM; i++)
		{
			if (m_nPackageItemIndex[i] == nItemIndex)
				return i;
		}
		return -1;
	}
}
#endif // ALTERNATE_MERCHANT