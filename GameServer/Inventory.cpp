/************************************
  수정사항 
  수정한 사람: bs
  수정일: 2004-11-18 오후 3:41:37
  설명: 인벤에서 제거될때 자동으로 착용정보 지우는 코드 추가
 ************************************/

#include "stdhdrs.h"
#include "Server.h"
#include "Item.h"
#include "Inventory.h"
#include "CmdMsg.h"
#include "Log.h"
#include "doFunc.h"

//////////////////////
// CInventoryRow class

CInventoryRow::CInventoryRow(CInventory* inven)
{
	memset(m_items, 0, sizeof(CItem*) * ITEMS_PER_ROW);
	m_inven = inven;
}

CInventoryRow::~CInventoryRow()
{
	int i;
	for (i = 0; i < ITEMS_PER_ROW; i++)
	{
		if (m_items[i])
		{
			m_items[i]->m_tab_idx = -1;
			m_items[i]->m_row_idx = -1;
			m_items[i]->m_col_idx = -1;
			delete m_items[i];
			m_items[i] = NULL;
		}
	}
}

// 빈자리 찾기
bool CInventoryRow::FindEmpty(int* col)
{
	int i;
	for (i = 0; i < ITEMS_PER_ROW; i++)
	{
		// 중간이 빈 자리 검사
		if (!m_items[i])
		{
			*col = i;
			return true;
		}
	}
	
	return false;
}

// 아이템 찾기 (조건에 만족하는 처음 아이템)
bool CInventoryRow::FindItem(int* col, int itemdbindex, int plus, int flag)
{
	int i;
	
	for (i = 0; i < ITEMS_PER_ROW; i++)
	{
		if (m_items[i])
		{
			if (m_items[i]->m_idNum == itemdbindex && (plus == -1 || m_items[i]->m_plus == plus) && (flag == -1 || m_items[i]->m_flag == flag))
			{
				*col = i;
				return true;
			}
		}
	}
	
	return false;
}

bool CInventoryRow::FindItem(int* col, int itemindex)
{
	int i;
	
	for (i = 0; i < ITEMS_PER_ROW; i++)
	{
		if (m_items[i] && m_items[i]->m_index == itemindex)
		{
			*col = i;
			return true;
		}
	}
	
	return false;
}

// 비었는지 검사
bool CInventoryRow::IsEmpty()
{
	int i;
	for (i = 0; i < ITEMS_PER_ROW; i++)
		if (m_items[i])
			return false;
		
		return true;
}

// 지우기
bool CInventoryRow::Remove(int col, bool bFreeMem, bool removeQuickSlot, bool bCalcStatus)
{
	if (col < 0 || col >= ITEMS_PER_ROW)
		return false;

	if (m_items[col])
	{
		if (m_inven->m_ch->m_moneyItem == m_items[col])
			m_inven->m_ch->m_moneyItem = NULL;
		if (removeQuickSlot)
		{
			m_inven->m_ch->RemoveItemFromQuickSlot(m_items[col]);
			if (m_items[col]->m_wearPos != WEARING_NONE)
			{
				CNetMsg rmsg;
				ItemWearMsg(rmsg, m_items[col]->m_wearPos, NULL, NULL);
				m_inven->m_ch->m_wearing[(int)m_items[col]->m_wearPos] = NULL;
				m_items[col]->m_wearPos = WEARING_NONE;
			}
		}

		m_inven->m_ch->CheckWeight(-1 * m_items[col]->m_itemProto->m_weight * m_items[col]->Count(), false, true, bCalcStatus);

		m_items[col]->m_tab_idx = -1;
		m_items[col]->m_row_idx = -1;
		m_items[col]->m_col_idx = -1;
		if (bFreeMem)
			delete m_items[col];
		m_items[col] = NULL;
	}

	return false;
}

// 빈공간 찾기
int CInventoryRow::GetSpace()
{
	int i;
	int ret = 0;
	for (i = 0; i < ITEMS_PER_ROW; i++)
	{
		if (m_items[i] == NULL)
			ret++;
	}

	return ret;
}



///////////////////
// CInventory class

CInventory::CInventory()
{
	memset(m_rows, 0, sizeof(CInventoryRow*) * MAX_INVENTORY_ROWS);
}

CInventory::~CInventory()
{
	int i;
	for (i = 0; i < MAX_INVENTORY_ROWS; i++)
	{
		if (m_rows[i])
		{
			delete m_rows[i];
			m_rows[i] = NULL;
		}
	}
}

// 아이템 추가
bool CInventory::AddItem(CItem* item, CPC* ch, bool bSendWarning, bool bCheckWeight, bool bCalcStatus, int row, int col)
{
	// 여기에 들어오는 item은 NULL일 수 있다.

// 050224 : bs : 인벤토리에 중복 검사 추가
	if (item)
	{
		if (GetItem(item->m_index))
			return true;

		// 시리얼 겹치면 겹쳐 넣은 척!
		if (GetItemBySerial(item->m_serial))
		{
			item->m_tab_idx = -1;
			item->m_row_idx = -1;
			item->m_col_idx = -1;

//			GAMELOG << init("HACK ITEM", m_ch->m_name, m_ch->m_nick, m_ch->m_desc->m_idname)
//					<< itemlog(item)
//					<< end;
//
//			return true;
		}
	}
// --- 050224 : bs : 인벤토리에 중복 검사 추가

	if (!bCheckWeight)
		bSendWarning = false;

	// 겹치는 아이템 찾기
	if (item && (item->m_itemProto->m_flag & ITEM_FLAG_COUNT))
	{
		int r, c;
		if (FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
		{
			// 겹치는 아이템 수량 변경하고 item의 tab, row, col 세팅해서 리턴
			CItem* p = GetItem(r, c);
			if (p)
			{
				// 무게 검사
				if (!m_ch->CheckWeight(item->m_itemProto->m_weight * item->m_count, bSendWarning, bCheckWeight, bCalcStatus))
					return false;

				p->m_count += item->m_count;
				item->m_tab_idx = -1;
				item->m_row_idx = -1;
				item->m_col_idx = -1;

				if (p->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index)
					m_ch->m_moneyItem = p;

				return true;
			}
		}
	}

	if (row < 0 || row >= MAX_INVENTORY_ROWS || col < 0 || col >= ITEMS_PER_ROW)
	{
		// 아무 빈자리에나 넣기
		if (!FindEmpty(&row, &col))
			return false;
	}

	else if (m_rows[row] && m_rows[row]->m_items[col])
	{
		// 이미 있으면 에러
		return false;
	}

	if (item)
	{
		// 무게 검사
		if (!m_ch->CheckWeight(item->m_itemProto->m_weight * item->m_count, bSendWarning, bCheckWeight, bCalcStatus))
			return false;

		item->m_tab_idx = m_tab;
		item->m_row_idx = row;
		item->m_col_idx = col;
	}

	if (!m_rows[row])
	{
		m_rows[row] = new CInventoryRow(this);
	}
	m_rows[row]->m_items[col] = item;

	if (item && item->m_idNum == gserver.m_itemProtoList.m_moneyItem->m_index)
		m_ch->m_moneyItem = item;
	
	return true;
}

// 아이템 수량 감소
bool CInventory::Decrease(CItem* item, LONGLONG count)
{
	if (!item || item->m_tab_idx != m_tab || GetItem(item->m_row_idx, item->m_col_idx) != item)
		return false;

	item->m_count -= count;

	m_ch->CheckWeight(-1 * item->m_itemProto->m_weight * count, false, true, true);

	return true;
}

// 빈자리 찾기
bool CInventory::FindEmpty(int* row, int* col)
{
	int i;
	for (i = 0; i < MAX_INVENTORY_ROWS; i++)
	{
		// 줄 전체가 비었을때
		if (!m_rows[i])
		{
			m_rows[i] = new CInventoryRow(this);
			*row = i;
			*col = 0;
			return true;
		}
		
		if (m_rows[i]->FindEmpty(col))
		{
			*row = i;
			return true;
		}
	}
	
	return false;
}

// 아이템 얻기
CItem* CInventory::GetItem(int row, int col)
{
	if (row < 0 || row >= MAX_INVENTORY_ROWS || col < 0 || col >= ITEMS_PER_ROW)
		return NULL;
	if (!m_rows[row])
		return NULL;
	if (!m_rows[row]->m_items[col])
		return NULL;
	return m_rows[row]->m_items[col];
}

// 아이템 찾기
bool CInventory::FindItem(int* row, int* col, int itemdbindex, int plus, int flag) const
{
	int i;
	for (i = 0; i < MAX_INVENTORY_ROWS; i++)
	{
		if (m_rows[i])
		{
			if (m_rows[i]->FindItem(col, itemdbindex, plus, flag))
			{
				*row = i;
				return true;
			}
		}
	}
	
	return false;
}

bool CInventory::FindItem(int* row, int* col, int itemidx) const
{
	int i;
	for (i = 0; i < MAX_INVENTORY_ROWS; i++)
	{
		if (m_rows[i])
		{
			if (m_rows[i]->FindItem(col, itemidx))
			{
				*row = i;
				return true;
			}
		}
	}
	
	return false;
}

// 아이템 자리 바꿈
bool CInventory::Swap(int row1, int col1, int itemidx1, int row2, int col2, int itemidx2)
{
	CItem* item1 = GetItem(row1, col1);
	CItem* item2 = GetItem(row2, col2);
	
	if (   (itemidx1 != -1 && (item1 == NULL || item1->m_index != itemidx1) )
		|| (itemidx1 == -1 && item1)
		|| (itemidx2 != -1 && (item2 == NULL || item2->m_index != itemidx2) )
		|| (itemidx2 == -1 && item2) )
		return false;

	if (item1 == item2)
		return true;

	Remove(row1, col1, false, false, false);
	Remove(row2, col2, false, false, false);

	AddItem(item1, m_ch, false, false, false, row2, col2);
	AddItem(item2, m_ch, false, false, false, row1, col1);
	
	return true;
}

// 자리 정렬
void CInventory::Arrange()
{
	CItem* items[MAX_INVENTORY_SIZE];
	CItem* tmp;
	memset(items, 0, sizeof(CItem*) * MAX_INVENTORY_SIZE);
	
	int i, j, k;
	for (i = 0, k = 0; i < MAX_INVENTORY_ROWS; i++)
	{
		for (j = 0; j < ITEMS_PER_ROW; j++)
		{
			tmp = GetItem(i, j);
			if (tmp)
				items[k++] = tmp;
			Remove(i, j, false, false, false);
		}
	}

	for (i = 0; i < k; i++)
		AddItem(items[i], m_ch, false, false, false);
}

// 지우기
bool CInventory::Remove(int row, int col, bool bFreeMem, bool removeQuickSlot, bool bCalcStatus)
{
	if (row < 0 || row >= MAX_INVENTORY_ROWS || !m_rows[row])
		return false;

	m_rows[row]->Remove(col, bFreeMem, removeQuickSlot, bCalcStatus);
	if (m_rows[row]->IsEmpty())
	{
		delete m_rows[row];
		m_rows[row] = NULL;
	}

	return true;
}

// 지우기
bool CInventory::Remove(CItem* item, bool bFreeMem, bool removeQuickSlot)
{
	if (!item)
		return false;

	if (m_tab == item->m_tab_idx && GetItem(item->m_row_idx, item->m_col_idx) == item)
		return Remove(item->m_row_idx, item->m_col_idx, bFreeMem, removeQuickSlot, true);
	else
	{
		// 인벤에 없으면
		// 겹치는 거 찾기
		if (item->m_itemProto->m_flag & ITEM_FLAG_COUNT)
		{
			int r, c;
			if (FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
			{
				CItem* p = GetItem(r, c);
				LONGLONG count = item->m_count;

				if (p->m_count < count)
					return false;

				m_ch->CheckWeight(-1 * item->m_itemProto->m_weight * item->m_count, false, true, true);

				// 새로 들어온 넘은 지우고
				if (bFreeMem)
				{
					delete item;
					item = NULL;
				}

				// 이전에 있던 넘 수량 변경
				if (p)
				{
					p->m_count -= count;
					if (p->m_count < 1)
					{
						p->m_count = 0;
						if (bFreeMem)
							Remove(p->m_row_idx, p->m_col_idx, bFreeMem, removeQuickSlot, true);
					}
				}

				return true;
			}
		}
	}

	return false;
}

// 빈공간 찾기
int CInventory::GetSpace()
{
	int ret = 0;
	int i;

	for (i = 0; i < MAX_INVENTORY_ROWS; i++)
	{
		if (m_rows[i] == NULL)
			ret += ITEMS_PER_ROW;
		else
			ret += m_rows[i]->GetSpace();
	}

	return ret;
}

CItem* CInventory::GetItemBySerial(const char* serial)
{
	int i, j;
	for (i = 0; i < MAX_INVENTORY_ROWS; i++)
	{
		if (m_rows[i])
		{
			for (j = 0; j < ITEMS_PER_ROW; j++)
			{
				if (m_rows[i]->m_items[j])
				{
					if (strcmp(m_rows[i]->m_items[j]->m_serial, serial) == 0)
						return m_rows[i]->m_items[j];
				}
			}
		}
	}
	return NULL;
}

void CInventory::CheckItemTime()
{
	CNetMsg				rmsg;
	CInventoryRow*		invenRow;		// FOR DEBUG
	CItem*				pItem;
	CPC*				pPC = m_ch;

	int i, j;
	for (i = 0; i < MAX_INVENTORY_ROWS; i++)
	{
		invenRow = m_rows[i];
		if (invenRow)
		{
			invenRow = m_rows[i];
			for (j = 0; invenRow && j < ITEMS_PER_ROW; j++)
			{
				pItem = invenRow->m_items[j];
				if (pItem)
				{
					if ((pItem->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE )
						&& pItem->m_itemProto->m_num4 == - 1)
					{	// 기간 무제한 아이템
						continue;
					}

					if (   pItem->m_itemProto->m_typeIdx == ITYPE_ONCE
						&& pItem->m_itemProto->m_subtypeIdx == IONCE_WARP
						&& pItem->m_itemProto->m_num0 == IONCE_WARP_PARTYRECALL)
					{
						if (
#ifdef PARTY_RECALL_NEW
							pItem->m_used != -1 &&
#endif	// PARTY_RECALL_NEW
							pItem->m_used < gserver.m_gameTime)
						{
							// 아이템 삭제 후 리턴
							GAMELOG << init("ITEM USE TIMEOUT", pPC)
									<< "ITEM" << delim
									<< itemlog(pItem)
									<< end;

							ItemDeleteMsg(rmsg, pItem);
							SEND_Q(rmsg, pPC->m_desc);
							RemoveFromInventory(pPC, pItem, true, true);
							return ;
						}
					} // 파티 리콜

					// 결합 의상 체크가 플래티늄보다 위에 있어야 함
					// 의상도 플래티늄 맥스 플러스 사용 -_-;
					// 결합용 의상 아이템
					else if (pItem->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE)
					{	// 결합된 아이템
#ifdef COMPOSITE_TIME						
						if (!(pItem->m_itemProto->m_flag & ITEM_FLAG_ABS))
						{	// 기간제					
							if (pItem->m_used_2 > 0 
								&& pItem->m_used_2 < gserver.m_gameTime)
							{	// 결합 주문서 시간 만료
								// 결합한 아이템이 있는가?
								// 있으면 결합을 해제하고
								// 착용 중이면 착용 해제하고
								// 인벤에 삽입
								if (pItem->m_flag & FLAG_ITEM_COMPOSITION)
								{								
									CItem * pItemEquip = pPC->m_invenNormal.GetItem(pItem->m_nCompositeItem);
									if (pItemEquip)
									{
										pItemEquip->m_flag &= ~FLAG_ITEM_COMPOSITION;

										ItemUpdateMsg(rmsg, pItemEquip, 0);
										SEND_Q(rmsg, pPC->m_desc);

										GAMELOG << init("ITEM SEPARATE", pPC)
												<< "CLOTHES" << delim
												<< itemlog(pItem) << delim
												<< "EQUIP" << delim
												<< itemlog(pItemEquip)
												<< end;
									}

									pItem->m_flag &= ~FLAG_ITEM_COMPOSITION;
									pItem->m_used_2 = 0;
									pItem->m_plus = 0;
									pItem->m_nCompositeItem = -1;

									ItemUpdateMsg(rmsg, pItem, 0);
									SEND_Q(rmsg, pPC->m_desc);
									

									if (pItem->m_wearPos != WEARING_NONE)
									{
										ItemWearMsg(rmsg, pItem->m_wearPos, NULL, NULL);
										do_Item(pPC, rmsg);
									}

									GAMELOG << init("COMPOSITE_TIME EXPIRE", pPC)
											<< itemlog(pItem)
											<< end;
		
									SysExpireItemcompositeMsg(rmsg, pItem->m_idNum);
									SEND_Q(rmsg, pPC->m_desc);
								
									pPC->SaveNow(false);	
								}													
							}	// 결합된 아이템
						}
#endif // COMPOSITE_TIME
						if (pItem->m_used < gserver.m_gameTime)	
						{	// 유료 의상 시간 만료
							// 결합한 아이템이 있는가?
							// 있으면 결합을 해제하고
							// 착용 중이면 착용 해제하고
							// 인벤에서 제거
							if (pItem->m_flag & FLAG_ITEM_COMPOSITION)
							{
								pItem->m_flag &= ~FLAG_ITEM_COMPOSITION;

								CItem* pItemEquip = pPC->m_invenNormal.GetItem(pItem->m_nCompositeItem);
								if (pItemEquip)
								{
									pItemEquip->m_flag &= ~FLAG_ITEM_COMPOSITION;

									ItemUpdateMsg(rmsg, pItemEquip, 0);
									SEND_Q(rmsg, pPC->m_desc);

									GAMELOG << init("ITEM SEPARATE", pPC)
											<< "CLOTHES" << delim
											<< itemlog(pItem) << delim
											<< "EQUIP" << delim
											<< itemlog(pItemEquip)
											<< end;
								}
							}
							if (pItem->m_wearPos != WEARING_NONE)
							{
								ItemWearMsg(rmsg, pItem->m_wearPos, NULL, NULL);
								do_Item(pPC, rmsg);
							}
							GAMELOG << init("CLOTHES EXPIRE", pPC)
									<< itemlog(pItem)
									<< end;
							SysExpireItemcompositeMsg(rmsg, pItem->m_idNum);
							SEND_Q(rmsg, pPC->m_desc);
							ItemDeleteMsg(rmsg, pItem);
							SEND_Q(rmsg, pPC->m_desc);
							RemoveFromInventory(pPC, pItem, true, true);
							pPC->SaveNow(false);
							return ;
						}
					} // 의상
#ifdef PLATINUM
					else if( pItem->IsUsedPlatinumSpecial() )
					{
						if (pItem->m_used < gserver.m_gameTime)
						{
							// 삭제 후 리턴
							int platinumPlus;
							FLAG_ITEM_PLATINUM_GET(pItem->m_flag, platinumPlus);

							pItem->m_plus -= platinumPlus;
							pItem->m_used = -1;

							GAMELOG << init("PLATINUM USE TIMEOUT", pPC)
									<< "ITEM" << delim
									<< itemlog(pItem) << delim
									<< platinumPlus << delim
									<< end;

							FLAG_ITEM_PLATINUM_SET(pItem->m_flag, 0);
							ItemUpdateMsg(rmsg, pItem, 0);
							SEND_Q(rmsg, pPC->m_desc);

							ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_PLATINUM_SPECIAL_EXPIRE);
							rmsg << pItem->m_itemProto->m_index;
							SEND_Q(rmsg, pPC->m_desc);

							CNetMsg wearMsg;
							WearingMsg(wearMsg, pPC, pItem->m_wearPos, pItem->m_idNum, pItem->m_plus);
							pPC->m_pArea->SendToCell(wearMsg, pPC, false);

							pPC->m_assist.CheckApplyConditions();
							pPC->CalcStatus(true);

							pPC->SaveNow(false);

							return ;
						}

					} // 플래티늄
#endif // PLATINUM
					// 골든볼 응모권
					else if (pItem->m_idNum == GOLDENBALL_CARD_INDEX)
					{
						time_t tItemTime = pItem->m_used;
						time_t tEndTime = gserver.m_clGoldenBall.GetVoteEndTime();
						if (gserver.m_clGoldenBall.GetStatus() == GOLDENBALL_STATUS_NOTHING || tItemTime != tEndTime)
						{
							GAMELOG << init("GOLDEN BALL EVENT RECALL CARD", pPC)
									<< itemlog(pItem)
									<< end;
							ItemDeleteMsg(rmsg, pItem);
							SEND_Q(rmsg, pPC->m_desc);
							RemoveFromInventory(pPC, pItem, true, true);
							pPC->CalcStatus(true);
							pPC->SaveNow(false);
							return ;
						}
					} // 골든볼				
					else if (pItem->IsLent())
					{	// 대여 무기
						if (pItem->m_used < gserver.m_gameTime)
						{
							// 아이템 삭제 후 리턴
							GAMELOG << init("ITEM USE TIMEOUT", pPC)
									<< "ITEM" << delim
									<< itemlog(pItem)
									<< end;

							if (pItem->m_wearPos != WEARING_NONE)
							{
								ItemWearMsg(rmsg, pItem->m_wearPos, NULL, NULL);
								do_Item(pPC, rmsg);
							}

							ItemLendWeaponMsg(rmsg, pItem->m_idNum, MSG_ITEM_LEND_TIMEOUT);
							SEND_Q(rmsg, pPC->m_desc);

							ItemDeleteMsg(rmsg, pItem);
							SEND_Q(rmsg, pPC->m_desc);
							RemoveFromInventory(pPC, pItem, true, true);
							return ;
						}
					} // // 대여 무기
#ifdef LEVELDOWN_SCROLL
					if (pItem->m_flag & FLAG_ITEM_LEVELDOWN)
					{
						if (pItem->m_used_2 < gserver.m_gameTime)
						{	// 레벨 제한 효과 만료
							pItem->m_flag &= ~FLAG_ITEM_LEVELDOWN;
							pItem->m_nLevel = 0;
							pItem->m_used_2 = -1;

							ItemUpdateMsg(rmsg, pItem, 0);
							SEND_Q(rmsg, pPC->m_desc);

							pPC->m_assist.CheckApplyConditions();
							pPC->CalcStatus(true);
						}
					}	// 레벨 다운					
#endif	// LEVELDOWN_SCROLL
					
#ifdef PRIMIUM_MEMORYBOOK
					if (pItem->m_itemProto->m_index == PRIMIUM_MEMORYBOOK)
					{
						if( pItem->m_used != -1 
							&& pItem->m_used < gserver.m_gameTime)
						{	// 기간 만료
							// 아이템 삭제 후 리턴
							GAMELOG << init("ITEM USE TIMEOUT", pPC)
									<< "ITEM" << delim
									<< itemlog(pItem)
									<< end;

							ItemDeleteMsg(rmsg, pItem);
							SEND_Q(rmsg, pPC->m_desc);
							RemoveFromInventory(pPC, pItem, true, true);
							return ;
						}
					}
#endif	// PRIMIUM_MEMORYBOOK
					
#ifdef AUTO_POTION
					if (pItem->m_itemProto->m_index == ONE_PERIOD_ITEM
						|| pItem->m_itemProto->m_index == SEVEN_PERIOD_ITEM
						|| pItem->m_itemProto->m_index == THIRTY_PERIOD_ITEM
						|| pItem->m_itemProto->m_index == 2610 )
					{
						if( pItem->m_used != -1 
							&& pItem->m_used < gserver.m_gameTime)
						{	// 기간 만료
							// 아이템 삭제 후 리턴
							GAMELOG << init("ITEM USE TIMEOUT", pPC)
									<< "ITEM" << delim
									<< itemlog(pItem)
									<< end;
							for( int k = WEARING_ACCESSORY1; k <= WEARING_ACCESSORY3; k++ )
							{
								if( pPC->m_wearing[k] )
								{
									if( pPC->m_wearing[k]->m_itemProto->m_index == ONE_PERIOD_ITEM 
										|| pPC->m_wearing[k]->m_itemProto->m_index == SEVEN_PERIOD_ITEM 
										|| pPC->m_wearing[k]->m_itemProto->m_index == THIRTY_PERIOD_ITEM
										|| pPC->m_wearing[k]->m_itemProto->m_index == 2610 )
									{
											char wear_pos = pPC->m_wearing[k]->m_wearPos;
											pPC->m_assist.CureByItemIndex(pPC->m_wearing[k]->m_itemProto->m_index);

											ItemWearMsg(rmsg,  wear_pos, NULL, pPC->m_wearing[k]);
											SEND_Q(rmsg, pPC->m_desc);
											pPC->m_wearing[k]->m_wearPos = WEARING_NONE;
											pPC->m_wearing[k] = NULL;
									}
								}
							}
							ItemDeleteMsg(rmsg, pItem);
							SEND_Q(rmsg, pPC->m_desc);
							RemoveFromInventory(pPC, pItem, true, true);
							return ;
						}
					}					
#endif // AUTO_POTION
/*
#ifdef USE_WAREHOUSE
					if (pItem->m_itemProto->m_index == USE_WAREHOUSE_ITEM1
						|| pItem->m_itemProto->m_index == USE_WAREHOUSE_ITEM2
						|| pItem->m_itemProto->m_index == USE_WAREHOUSE_ITEM3)
					{
						if( pItem->m_used != -1 
							&& pItem->m_used < gserver.m_gameTime)
						{	// 기간 만료
							// 아이템 삭제 후 리턴
							GAMELOG << init("ITEM USE TIMEOUT", pPC)
									<< "ITEM" << delim
									<< itemlog(pItem)
									<< end;

							ItemDeleteMsg(rmsg, pItem);
							SEND_Q(rmsg, pPC->m_desc);
							RemoveFromInventory(pPC, pItem, true, true);
							return ;
						}
					}
#endif // USE_WAREHOUSE


#ifdef USE_GROCERY
					if (pItem->m_itemProto->m_index == USE_GROCERY_ITEM1
						|| pItem->m_itemProto->m_index == USE_GROCERY_ITEM2
						|| pItem->m_itemProto->m_index == USE_GROCERY_ITEM3)
					{
						if( pItem->m_used != -1 
							&& pItem->m_used < gserver.m_gameTime)
						{	// 기간 만료
							// 아이템 삭제 후 리턴
							GAMELOG << init("ITEM USE TIMEOUT", pPC)
									<< "ITEM" << delim
									<< itemlog(pItem)
									<< end;

							ItemDeleteMsg(rmsg, pItem);
							SEND_Q(rmsg, pPC->m_desc);
							RemoveFromInventory(pPC, pItem, true, true);
							return ;
						}
					}
#endif // USE_GROCERY
*/

#if defined ( USE_WAREHOUSE ) || defined ( USE_GROCERY ) || defined ( SHOW_HP )
			switch( pItem->m_itemProto->m_index )
			{
			case 2455: case 2456: case 2457:
			case 2458: case 2459: case 2460:
			case 2607: case 2608:
				{
					if( pItem->m_used != -1 && pItem->m_used < gserver.m_gameTime)
						{	// 기간 만료
							// 아이템 삭제 후 리턴
							GAMELOG << init("ITEM USE TIMEOUT", pPC)
									<< "ITEM" << delim
									<< itemlog(pItem)
									<< end;

							ItemDeleteMsg(rmsg, pItem);
							SEND_Q(rmsg, pPC->m_desc);
							RemoveFromInventory(pPC, pItem, true, true);
							return ;
						}
					
				}
				break;
			case 2461: case 2462: case 2463: case 2606:
				{
					if( pItem->m_used < gserver.m_gameTime)
						{	// 기간 만료
							// 아이템 삭제 후 리턴
							GAMELOG << init("ITEM USE TIMEOUT", pPC)
									<< "ITEM" << delim
									<< itemlog(pItem)
									<< end;

							rmsg.Init(MSG_PK);
							rmsg << (unsigned char)MSG_PK_TARGET_SHOWHP 
								 << (char)0 ;
							SEND_Q( rmsg, pPC->m_desc);

							pPC->m_assist.CureByItemIndex(pItem->m_itemProto->m_index);

							ItemDeleteMsg(rmsg, pItem);
							SEND_Q(rmsg, pPC->m_desc);
							RemoveFromInventory(pPC, pItem, true, true);
							return ;
						}
				}
				break;
			}

#endif // #if defined ( USE_WAREHOUSE ) || defined ( USE_GROCERY ) || defined ( SHOW_HP )

#ifdef COMPETITION_ITEM
					if( pItem->m_itemProto->m_index == EXPERIENCE_COMPETITION_ITEM 
						|| pItem->m_itemProto->m_index == PERIOD_COMPETITION_ITEM 
#ifdef LC_JPN
						|| pItem->m_itemProto->m_index == 2500   // 일본은 2500번 아이템도 광속임
#endif // LC_JPN
						|| pItem->m_itemProto->m_index == 2609
						)
					{
						if( pItem->m_used != -1 && pItem->m_used < gserver.m_gameTime )
						{
							//기간 만료 아이템 삭제
							GAMELOG << init("COMPETITION ITEM USE TIEMOUT", pPC )
								<< "ITEM" << delim
								<< itemlog(pItem )
								<< end;
							if( pItem->Count() > 1 )
							{
								DecreaseFromInventory(pPC, pItem, 1);
								ItemUpdateMsg(rmsg, pItem, -1);
								SEND_Q(rmsg, pPC->m_desc);
								pItem->m_used = -1;
							}
							else 
							{
								ItemDeleteMsg(rmsg, pItem);
								SEND_Q(rmsg, pPC->m_desc);
								RemoveFromInventory(pPC, pItem, true, true);
								return ;							
							}
						}

						

					}
#endif // COMPETITION_ITEM

#ifdef LAKA_PRESSCORPS
					// 라카기자단 아이템
					if(pItem->m_itemProto->m_index == 2882)
					{
						if (pItem->m_used < gserver.m_gameTime)
						{
							// 아이템 삭제 후 리턴
							GAMELOG << init("ITEM USE TIMEOUT", pPC)
									<< "ITEM" << delim
									<< itemlog(pItem)
									<< end;

							ItemDeleteMsg(rmsg, pItem);
							SEND_Q(rmsg, pPC->m_desc);
							RemoveFromInventory(pPC, pItem, true, true);
							return ;
						}
					}
#endif // LAKA_PRESSCORPS
#ifdef ATTACK_PET
					// ATTACK_PET  관련 아이템 시간 체크
					if( ( pItem->m_itemProto->m_jobFlag&(1<<JOB_APET) ) &&
						( pItem->m_itemProto->m_flag&ITEM_FLAG_ABS) )
					{
						if (pItem->m_used < gserver.m_gameTime)
						{
#ifdef APET_AI
							// AI 아이템은 AI 리스트에서도 삭제
							if (pItem->m_itemProto->m_typeIdx == ITYPE_ETC && pItem->m_itemProto->m_subtypeIdx == IETC_PET_AI)
							{
								CAPet * pAPetNetxt = pPC->m_pApetlist;
								CAPet * pAPet;
								while((pAPet = pAPetNetxt))
								{
									pAPetNetxt = pAPet->m_pNextPet;
									pAPet->DelAIList(pItem);
								}

								pAPet = pPC->GetAPet();
								if (pAPet)
								{
									APetAIListMsg(rmsg, pAPet);
									SEND_Q(rmsg, pPC->m_desc);
								}
							}
#endif // APET_AI
							// 아이템 삭제 후 리턴
							GAMELOG << init("ITEM USE TIMEOUT ATTACK_PET", pPC)
								<< "ITEM" << delim
								<< itemlog(pItem)
								<< end;

							ItemDeleteMsg(rmsg, pItem);
							SEND_Q(rmsg, pPC->m_desc);
							RemoveFromInventory(pPC, pItem, true, true);
							return ;
						}
					}
#endif // ATTACK_PET
				} // if (invenRow->m_items[j])
			} // for (j = 0; j < ITEMS_PER_ROW; j++)
		} // if (invenRow)
	} // for (i = 0; i < MAX_INVENTORY_ROWS; i++)
}

void CInventory::CheckCompositeValidation()
{
	int i, j, k, l;
	for (i = 0; i < MAX_INVENTORY_ROWS; i++)
	{
		if (m_rows[i])
		{
			for (j = 0; j < ITEMS_PER_ROW; j++)
			{
				if (m_rows[i]->m_items[j])
				{
					// 합성된 아이템이면
					if (m_rows[i]->m_items[j]->m_flag & FLAG_ITEM_COMPOSITION)
					{
						if (m_rows[i]->m_items[j]->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE)
						{
							// 합성된 의상은 해당 장비를 찾아본다
							if (GetItem(m_rows[i]->m_items[j]->m_nCompositeItem) == NULL)
							{
								// 의상은 합성이 되어 있는데 해당 장비가 없으면 합성 상태 해제
								GAMELOG << init("COMPOSITE VALID", m_ch)
										<< itemlog(m_rows[i]->m_items[j])
										<< "COMPOSITE EQUIP" << delim
										<< m_rows[i]->m_items[j]->m_nCompositeItem
										<< end;
								m_rows[i]->m_items[j]->m_nCompositeItem = -1;
								m_rows[i]->m_items[j]->m_flag &= ~FLAG_ITEM_COMPOSITION;
							}
						}
						else
						{
							// 합성된 장비는 해당 의상을 찾는다
							bool bFound = false;
							for (k = 0; !bFound && k < MAX_INVENTORY_ROWS; k++)
							{
								if (m_rows[k])
								{
									for (l = 0; !bFound && l < ITEMS_PER_ROW; l++)
									{
										if (m_rows[k]->m_items[l])
										{
											if (   (m_rows[k]->m_items[l]->m_flag & FLAG_ITEM_COMPOSITION)
												&& (m_rows[k]->m_items[l]->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE)
												&& m_rows[k]->m_items[l]->m_nCompositeItem == m_rows[i]->m_items[j]->m_index)
											{
												bFound = true;
											}
										}
									}
								}
							}

							if (!bFound)
							{
								// 장비는 결합상태이나 해당 의상이 없을 경우
								GAMELOG << init("COMPOSITED EQUIP VALID", m_ch)
										<< itemlog(m_rows[i]->m_items[j])
										<< end;
								m_rows[i]->m_items[j]->m_flag &= ~FLAG_ITEM_COMPOSITION;
							}
						}
					}
				}
			}
		}
	}
}
