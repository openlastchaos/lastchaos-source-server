#include "stdhdrs.h"
#include "Server.h"
#include "Log.h"
#include "CmdMsg.h"
#include "ExchangeItems.h"

CExchangeItemsData::CExchangeItemsData()
{
	m_ch = NULL;
	m_regCount = 0;
	memset(m_itemindex, -1, sizeof(int) * MAX_EXCHANGE_ITEMS);
	memset(m_count, 0, sizeof(LONGLONG) * MAX_EXCHANGE_ITEMS);
}

void CExchangeItemsData::SetCharacter(CPC* pc)
{
	m_ch = pc;
}

int CExchangeItemsData::Add(int itemindex, LONGLONG count, bool* bUpdate)
{
	if (itemindex == -1)
		return -1;

	CItem* pitem = m_ch->m_invenNormal.GetItem(itemindex);
	if (pitem == NULL)
		return -1;

	if (count < 0)
		return -1;

	int i = Find(itemindex);
	if (i == -1)
	{
		// 추가
		if (pitem->Count() < count)
			return -1;

		// 개수 제한에 걸리면 리턴
		i = FindEmpty();
		if (i < 0)		return -1;

		// 추가
		m_regCount++;
		m_itemindex[i] = itemindex;
		m_count[i] = count;
		*bUpdate = false;

		return i;
	}
	else
	{
		// 갱신
		if (pitem->Count() < m_count[i] + count)
			return -1;

		m_count[i] += count;
		*bUpdate = true;

		return i;
	}
}

int CExchangeItemsData::Del(int itemindex, LONGLONG count)
{
	if (itemindex == -1)
		return -1;

	int i = Find(itemindex);
	if (i == -1)
	{
		// 없는걸 어쩌란 말이냐!!!
		return -1;
	}
	else if (count < 0)
	{
		// 이건 빼는 함수란 말이닷!!!!!
		return -1;
	}
	else if (m_count[i] < count)
	{
		// 넣은 것보다 많이 빼려는 심보!
		return -1;
	}
	else
	{
		// 갱신
		m_count[i] -= count;

		if (m_count[i] == 0)
		{
			// 다 뺀거면
			m_itemindex[i] = -1;
			m_regCount--;
		}

		return i;
	}
}

int CExchangeItemsData::Find(int item_idx)
{
	if (item_idx == -1)
		return -1;

	int i;
	for (i = 0; i < MAX_EXCHANGE_ITEMS; i++)
	{
		if (m_count[i])
		{
			if (m_itemindex[i] == item_idx)
				return i;
		}
	}

	return -1;
}

int CExchangeItemsData::FindEmpty()
{
	int i;
	for (i = 0; i < MAX_EXCHANGE_ITEMS; i++)
	{
		if (m_count[i] == 0)
			return i;
	}

	return -1;
}

CExchangeItems::CExchangeItems()
{
	m_status = MSG_EXCHANGE_REQ_REQ;
}

void CExchangeItems::SetCharacter(CPC* src, CPC* dest)
{
	m_src.SetCharacter(src);
	m_dest.SetCharacter(dest);
}

int CExchangeItems::Add(CPC* ch, int item_idx, LONGLONG count, bool* bUpdate)
{
	if (item_idx == -1)
		return -1;

	// 인벤에 있나 보고
	if (ch->m_invenNormal.GetItem(item_idx) == NULL)
		return -1;

	// 대상 검사
	CExchangeItemsData* data;
	if (ch == m_src.m_ch)
		data  = &m_src;
	else if (ch == m_dest.m_ch)
		data = &m_dest;
	else
		return -1;

	// 추가
	return data->Add(item_idx, count, bUpdate);
}

int CExchangeItems::Del(CPC* ch, int item_idx, LONGLONG count)
{
	if (item_idx == -1)
		return -1;

	// 인벤에 있나 보고
	// 인벤에 있나 보고
	if (ch->m_invenNormal.GetItem(item_idx) == NULL)
		return -1;

	// 대상 검사
	CExchangeItemsData* data;
	if (ch == m_src.m_ch)
		data  = &m_src;
	else if (ch == m_dest.m_ch)
		data = &m_dest;
	else
		return -1;

	// 제거
	return data->Del(item_idx, count);
}

int CExchangeItems::Find(CPC* ch, int item_idx)
{
	// 대상 검사
	CExchangeItemsData* data;
	if (ch == m_src.m_ch)
		data  = &m_src;
	else if (ch == m_dest.m_ch)
		data = &m_dest;
	else
		return -1;

	// 찾기
	return data->Find(item_idx);
}

int CExchangeItems::CanExchange()
{
	int srcDel	= 0,			// src의 아이템 제거 수
		srcAdd	= 0,			// src의 아이템 추가 수
		destDel = 0,			// dest의 아이템 제거 수
		destAdd = 0;			// dest의 아이템 추가 수
	int s2dweight = 0;
	int d2sweight = 0;

	int srcAddPet = 0,
		srcDelPet = 0,
		destAddPet = 0,
		destDelPet = 0;
	
	int i;
	
	CItem* item;
	
	for (i = 0; i < MAX_EXCHANGE_ITEMS; i++)
	{
		// src -> dest
		if (m_src.m_itemindex[i] != -1)
		{
			item = m_src.m_ch->m_invenNormal.GetItem(m_src.m_itemindex[i]);
			if (item)
			{
				// 겹칠수 있는 아이템이나?
				if (item->m_itemProto->m_flag & ITEM_FLAG_COUNT)
				{
					// src에서 빠지나?
					if (item->Count() == m_src.m_count[i])
						srcDel++;
					if (item->Count() < m_src.m_count[i])
						return 1;
					
					// dest에 추가되나?
					int r, c;
					if (!m_dest.m_ch->m_invenNormal.FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
						destAdd++;
				}
				else
				{
					srcDel++;
					destAdd++;
				}

#ifdef ENABLE_PET
				if( item->IsPet() )
				{
					srcDelPet++;
					destAddPet++;
				}
#endif
				s2dweight += item->m_itemProto->m_weight * m_src.m_count[i];
			}
			else
				return 1;
		}
		
		// dest -> src
		if (m_dest.m_itemindex[i] != -1)
		{
			item = m_dest.m_ch->m_invenNormal.GetItem(m_dest.m_itemindex[i]);
			if (item)
			{
				// 겹칠수 있는 아이템이나?
				if (item->m_itemProto->m_flag & ITEM_FLAG_COUNT)
				{
					// dest에서 빠지나?
					if (item->Count() == m_dest.m_count[i])
						destDel++;
					if (item->Count() < m_dest.m_count[i])
						return 2;
					
					// src에 추가되나?
					int r, c;
					if (!m_src.m_ch->m_invenNormal.FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag))
						srcAdd++;
				}
				else
				{
					destDel++;
					srcAdd++;
				}
#ifdef ENABLE_PET
				if( item->IsPet() )
				{
					destDelPet++;
					srcAddPet++;
				}
#endif
				d2sweight += item->m_itemProto->m_weight * m_dest.m_count[i];
			}
			else
				return 2;
		}
	}
	
	// 빈칸 검사
	if (m_src.m_ch->m_invenNormal.GetSpace() < (srcAdd - srcDel))
		return 1;
	// 무게 검사
	if (m_src.m_ch->m_weight + d2sweight - s2dweight >= m_src.m_ch->m_maxWeight * 15 / 10)
		return 1;
	
	// 빈칸 검사
	if (m_dest.m_ch->m_invenNormal.GetSpace() < (destAdd - destDel))
		return 2;
	// 무게 검사
	if (m_dest.m_ch->m_weight + s2dweight - d2sweight >= m_dest.m_ch->m_maxWeight * 15 / 10)
		return 2;

#ifdef ENABLE_PET
	if( m_src.m_ch->m_petList )
	{
		int petCount = 0;
		CPet* p = m_src.m_ch->m_petList;
		while (p)
		{
			petCount++;
			p = p->m_nextPet;
		}

		if( petCount + srcAddPet - srcDelPet > MAX_OWN_PET )
			return 3;
	}

	if( m_dest.m_ch->m_petList )
	{
		int petCount = 0;
		CPet* p = m_dest.m_ch->m_petList;
		while (p)
		{
			petCount++;
			p = p->m_nextPet;
		}

		if( petCount + destAddPet - destDelPet > MAX_OWN_PET )
			return 4;
	}
#endif
	
	return 0;
}

void CExchangeItems::DoExchange()
{
	int i;
	
	// LOG
	GAMELOG << init("ITEM_EXCHANGE", m_src.m_ch);
	
	for (i=0; i < MAX_EXCHANGE_ITEMS; i++)
	{
		if (m_src.m_itemindex[i] != -1)
		{
			CItem* item = m_src.m_ch->m_invenNormal.GetItem(m_src.m_itemindex[i]);
			if (item)
			{
				GAMELOG << itemlog(item) << delim
						<< m_src.m_count[i];
			}
		}
	}
	
	GAMELOG << " <=> "
		<< m_dest.m_ch->m_name << delim << m_dest.m_ch->m_nick << delim << m_dest.m_ch->m_desc->m_idname;
	
	for (i=0; i < MAX_EXCHANGE_ITEMS; i++)
	{
		if (m_dest.m_itemindex[i] != -1)
		{
			CItem* item = m_dest.m_ch->m_invenNormal.GetItem(m_dest.m_itemindex[i]);
			if (item)
			{
				GAMELOG << delim
						<< itemlog(item) << delim
						<< m_dest.m_count[i];
			}
		}
	}
	
	GAMELOG << end;
	
	CItem* item;
	CNetMsg rmsg;
	int r, c;
	CItem* s2ditem[MAX_EXCHANGE_ITEMS];
	CItem* d2sitem[MAX_EXCHANGE_ITEMS];
	memset(s2ditem, 0, sizeof(*s2ditem) * MAX_EXCHANGE_ITEMS);
	memset(d2sitem, 0, sizeof(*d2sitem) * MAX_EXCHANGE_ITEMS);

#ifdef ENABLE_PET
	CPet* petListToSrc = NULL;
	CPet* petListToDest = NULL;
	CPet* pet = NULL;
#endif
#ifdef ATTACK_PET
	CAPet* apetListToSrc = NULL;
	CAPet* apetListToDest = NULL;
	CAPet* apet = NULL;
#endif //ATTACK_PET
	// 아이템을 인벤에서 제거
	for (i = 0; i < MAX_EXCHANGE_ITEMS; i++)
	{
		// src -> dest
		item = m_src.m_ch->m_invenNormal.GetItem(m_src.m_itemindex[i]);
		if (item)
		{
#ifdef ENABLE_PET
			// 펫 아이템이면
			if (item->IsPet())
			{
				pet = m_src.m_ch->GetPet(item->m_plus);
				if (pet)
				{
					REMOVE_FROM_BILIST(pet, m_src.m_ch->m_petList, m_prevPet, m_nextPet);
					ADD_TO_BILIST(pet, petListToDest, m_prevPet, m_nextPet);
				}
			}
#endif
#ifdef ATTACK_PET
			if (item->IsAPet())
			{
				apet = m_src.m_ch->GetAPet(item->m_plus);
				if (apet)
				{
					REMOVE_FROM_BILIST(apet, m_src.m_ch->m_pApetlist, m_pPrevPet, m_pNextPet);
					ADD_TO_BILIST(apet, apetListToDest, m_pPrevPet, m_pNextPet);
				}
			}
#endif //ATTACK_PET

			// 겹치고 아이템 남은 수량이 생길 때
			if ((item->m_itemProto->m_flag & ITEM_FLAG_COUNT) && (item->Count() > m_src.m_count[i]))
			{
				DecreaseFromInventory(m_src.m_ch, item, m_src.m_count[i]);		// 감소
				ItemUpdateMsg(rmsg, item, -m_src.m_count[i]);
				s2ditem[i] = gserver.m_itemProtoList.CreateItem(item->m_idNum, -1, item->m_plus, item->m_flag, m_src.m_count[i]);
			}
			else
			{
				ItemDeleteMsg(rmsg, item);
				RemoveFromInventory(m_src.m_ch, item, false, true);					// 다 빠질때
				s2ditem[i] = item;
			}
			SEND_Q(rmsg, m_src.m_ch->m_desc);
		}
		
		// dest -> src
		item = m_dest.m_ch->m_invenNormal.GetItem(m_dest.m_itemindex[i]);
		if (item)
		{
#ifdef ENABLE_PET
			// 펫 아이템이면
			if (item->IsPet())
			{
				pet = m_dest.m_ch->GetPet(item->m_plus);
				if (pet)
				{
					REMOVE_FROM_BILIST(pet, m_dest.m_ch->m_petList, m_prevPet, m_nextPet);
					ADD_TO_BILIST(pet, petListToSrc, m_prevPet, m_nextPet);
				}
			}
#endif
#ifdef ATTACK_PET
			if (item->IsAPet())
			{
				apet = m_dest.m_ch->GetAPet(item->m_plus);
				if (apet)
				{
					REMOVE_FROM_BILIST(apet, m_dest.m_ch->m_pApetlist, m_pPrevPet, m_pNextPet);
					ADD_TO_BILIST(apet, apetListToSrc, m_pPrevPet, m_pNextPet);
				}
			}
#endif //ATTACK_PET

			// 겹치고 아이템 남은 수량이 생길 때
			if ((item->m_itemProto->m_flag & ITEM_FLAG_COUNT) && (item->Count() > m_dest.m_count[i]))
			{
				DecreaseFromInventory(m_dest.m_ch, item, m_dest.m_count[i]);	// 감소
				ItemUpdateMsg(rmsg, item, -m_dest.m_count[i]);
				d2sitem[i] = gserver.m_itemProtoList.CreateItem(item->m_idNum, -1, item->m_plus, item->m_flag, m_dest.m_count[i]);
			}
			else
			{
				ItemDeleteMsg(rmsg, item);
				RemoveFromInventory(m_dest.m_ch, item, false, true);					// 다 빠질때
				d2sitem[i] = item;
			}
			SEND_Q(rmsg, m_dest.m_ch->m_desc);
		}
	}

#ifdef ENABLE_PET
	// 펫 이동
	while (petListToSrc)
	{
		pet = petListToSrc;
		REMOVE_FROM_BILIST(pet, petListToSrc, m_prevPet, m_nextPet);
		pet->SetOwner(m_src.m_ch);
		ADD_TO_BILIST(pet, m_src.m_ch->m_petList, m_prevPet, m_nextPet);
		ExPetStatusMsg(rmsg, pet);
		SEND_Q(rmsg, m_src.m_ch->m_desc);
		ExPetSkillListMsg(rmsg, pet);
		SEND_Q(rmsg, m_src.m_ch->m_desc);
	}
	while (petListToDest)
	{
		pet = petListToDest;
		REMOVE_FROM_BILIST(pet, petListToDest, m_prevPet, m_nextPet);
		pet->SetOwner(m_dest.m_ch);
		ADD_TO_BILIST(pet, m_dest.m_ch->m_petList, m_prevPet, m_nextPet);
		ExPetStatusMsg(rmsg, pet);
		SEND_Q(rmsg, m_dest.m_ch->m_desc);
		ExPetSkillListMsg(rmsg, pet);
		SEND_Q(rmsg, m_dest.m_ch->m_desc);
	}
#endif

#ifdef ATTACK_PET
	// 펫 이동
	while (apetListToSrc)
	{
		apet = apetListToSrc;
		REMOVE_FROM_BILIST(apet, apetListToSrc, m_pPrevPet, m_pNextPet);
		apet->SetOwner(m_src.m_ch);
		apet->AddFaith(-10);
		ADD_TO_BILIST(apet, m_src.m_ch->m_pApetlist, m_pPrevPet, m_pNextPet);
	}
	while (apetListToDest)
	{
		apet = apetListToDest;
		REMOVE_FROM_BILIST(apet, apetListToDest, m_pPrevPet, m_pNextPet);
		apet->SetOwner(m_dest.m_ch);
		apet->AddFaith(-10);
		ADD_TO_BILIST(apet, m_dest.m_ch->m_pApetlist, m_pPrevPet, m_pNextPet);
	}
#endif // ATTACK_PET
	
	// 아이템을 인벤에 추가
	for (i = 0; i < MAX_EXCHANGE_ITEMS; i++)
	{
		// 아이템을 인벤에서 제거
		// src -> dest
		item = s2ditem[i];
		if (item)
		{
			// 인벤에 넣기
			AddToInventory(m_dest.m_ch, item, false, true);
			
			// 겹치면 메모리 해제
			if (item->tab() < 0)
			{
				m_dest.m_ch->m_invenNormal.FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag);
				delete item;
				item = m_dest.m_ch->m_invenNormal.GetItem(r, c);
				ItemUpdateMsg(rmsg, item, m_src.m_count[i]);
			}
			else
				ItemAddMsg(rmsg, item);
			SEND_Q(rmsg, m_dest.m_ch->m_desc);
		}
		
		// dest -> src
		item = d2sitem[i];
		if (item)
		{
			// 인벤에 넣기
			AddToInventory(m_src.m_ch, item, false, true);
			
			// 겹치면 메모리 해제
			if (item->tab() < 0)
			{
				m_src.m_ch->m_invenNormal.FindItem(&r, &c, item->m_idNum, item->m_plus, item->m_flag);
				delete item;
				item = m_src.m_ch->m_invenNormal.GetItem(r, c);
				ItemUpdateMsg(rmsg, item, m_dest.m_count[i]);
			}
			else
				ItemAddMsg(rmsg, item);
			SEND_Q(rmsg, m_src.m_ch->m_desc);
		}
	}
}
