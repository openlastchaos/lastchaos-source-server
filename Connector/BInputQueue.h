// InputQueue.h: interface for the CInputQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINPUTQUEUE_H__D2841812_61A5_4982_9D9A_47205BF43165__INCLUDED_)
#define AFX_BINPUTQUEUE_H__D2841812_61A5_4982_9D9A_47205BF43165__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BPacket.h"

class CBInputQueue  
{
	class CNode
	{
	public:
		CBPacket	m_pack;
		CNode*		m_next;
		CNode(CBPacket& pack)
		{
			m_pack.Init(pack);
			m_next = NULL;
		}
	};

	CMutexObject m_mutex;
	CNode* m_head;
	CNode* m_tail;
#ifdef __GAME_SERVER__
	int count;
#endif

public:
	CBInputQueue()
	{
		m_head = NULL;
		m_tail = NULL;
#ifdef __GAME_SERVER__
		count = 0;
#endif
	}

	virtual ~CBInputQueue()
	{
		Clear();
	}

	void Clear()
	{
		CMutex lock(&m_mutex);

		CNode* d = m_head;
		while (d)
		{
			m_head = d->m_next;
			delete d;
			d = m_head;
		}
		m_head = NULL;
		m_tail = NULL;
#ifdef __GAME_SERVER__
		count = 0;
#endif
	}

#ifdef __GAME_SERVER__
	bool Add(CBPacket& pack, int maxcount)
#else
	bool Add(CBPacket& pack)
#endif
	{
		CMutex lock(&m_mutex);

		CNode* node = new CNode(pack);
		if (!m_head)
			m_head = m_tail = node;
		else
		{
			m_tail->m_next = node;
			m_tail = node;
		}
#ifdef __GAME_SERVER__
		count++;
		// TODO : 나중에 슬롯수가 커지면 이것도 크게 잡아야 함
		if (count > maxcount)
		{
			return false;
		}
#endif
		return true;
	}

	bool Get(CBPacket& ret)
	{
		CMutex lock(&m_mutex);

		if (IsEmpty())
			return false;

		CNode* d = m_head;
		m_head = m_head->m_next;
		if (!m_head)
			m_tail = NULL;
		ret.Init(d->m_pack);
		delete d;
		d = NULL;
		ret.MoveFirst();
#ifdef __GAME_SERVER__
		count--;
#endif
		return true;
	}

	bool IsEmpty()
	{
		if (m_head)
			return false;
		else
			return true;
	}
};

#endif // !defined(AFX_INPUTQUEUE_H__D2841812_61A5_4982_9D9A_47205BF43165__INCLUDED_)
