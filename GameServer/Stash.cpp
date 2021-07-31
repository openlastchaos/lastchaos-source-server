#include "stdhdrs.h"
#include "Server.h"
#include "Stash.h"

CStash::CStash()
{
	memset(m_item, 0, sizeof(CItem*) * MAX_STASH_ITEM);
}

CStash::~CStash()
{
	int i;
	for (i = 0; i < MAX_STASH_ITEM; i++)
	{
		if (m_item[i])
		{
			delete m_item[i];
			m_item[i] = NULL;
		}
	}
}

int CStash::add(CItem* item)
{
	if (item == NULL)
		return -1;

	int i;
	for (i = 0; i < MAX_STASH_ITEM; i++)
	{
		if (m_item[i] == NULL)
		{
			m_item[i] = item;
			return i;
		}
	}

	return -1;
}

int CStash::add(int itemdbindex, int plus, int flag, LONGLONG count)
{
	if (itemdbindex < 0 || count < 1)
		return false;

	int idx = find(itemdbindex, plus, flag);
	if (idx == -1)
	{
		int i;
		for (i = 0; i < MAX_STASH_ITEM; i++)
		{
			if (m_item[i] == NULL)
			{
				m_item[i] = gserver.m_itemProtoList.CreateItem(itemdbindex, -1, plus, flag, count);
				return i;
			}
		}
	}
	else
	{
		m_item[idx]->m_count += count;
		return idx;
	}

	return -1;
}

int CStash::remove(int itemindex, LONGLONG count)
{
	if (itemindex < 1 || count < 1)
		return -2;

	int i;
	for (i = 0; i < MAX_STASH_ITEM; i++)
	{
		if (m_item[i])
		{
			if (m_item[i]->m_index == itemindex)
			{
				if (m_item[i]->Count() < count)
				{
					return -2;
				}
				else
				{
					m_item[i]->m_count -= count;
					if (m_item[i]->Count() == 0)
					{
						delete m_item[i];
						m_item[i] = NULL;
						return 0;
					}
					else
						return m_item[i]->Count();
				}
			}
		}
	}

	return -1;
}

CItem* CStash::remove(int itemindex)
{
	int i;
	for (i = 0; i < MAX_STASH_ITEM; i++)
	{
		if (m_item[i])
		{
			if (m_item[i]->m_index == itemindex)
			{
				CItem* ret = m_item[i];
				m_item[i] = NULL;
				return ret;
			}
		}
	}
	return NULL;
}

int CStash::find(int itemdbindex, int plus, int flag) const
{
	int i;
	for (i = 0; i < MAX_STASH_ITEM; i++)
	{
		if (m_item[i])
		{
			if (m_item[i]->m_idNum == itemdbindex && m_item[i]->m_plus == plus && m_item[i]->m_flag == flag)
			{
				return i;
			}
		}
	}

	return -1;
}

int CStash::find(int itemindex) const
{
	int i;
	for (i = 0; i < MAX_STASH_ITEM; i++)
	{
		if (m_item[i])
		{
			if (m_item[i]->m_index == itemindex)
			{
				return i;
			}
		}
	}

	return -1;
}

int CStash::space(bool bremain)
{
	int ret = 0;
	int i;
	int max_stash_item = MAX_STASH_ITEM_SPACE;

	if(bremain)
		max_stash_item = MAX_STASH_ITEM_SPACE;
	else
		max_stash_item = MAX_STASH_ITEM_NORMAL;
	
	for (i = 0; i < MAX_STASH_ITEM; i++)
	{
		if (m_item[i] != NULL)
			ret++;
	}

	ret = max_stash_item - ret;
	if( ret < 0 )
		ret = 0;
	
	return ret;
}
