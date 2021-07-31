// OutputBuffer.h: interface for the COutputBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUTPUTBUFFER_H__0A40D7F4_947A_4BBF_ACA4_9FB66CE4116E__INCLUDED_)
#define AFX_BOUTPUTBUFFER_H__0A40D7F4_947A_4BBF_ACA4_9FB66CE4116E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BPacket.h"
#include "MemPool.h"

extern COutputBufferPool g_Boutbufpool;

class CBOutputBuffer  
{
	unsigned char*	m_buf;
	int				m_ptr;
	int				m_len;

public:
	CBOutputBuffer()
	{
		m_buf = g_Boutbufpool.Get();
		m_ptr = 0;

	}

	virtual ~CBOutputBuffer()
	{
		g_Boutbufpool.Set(m_buf);
		m_buf = NULL;
		m_ptr = 0;
	}

	void Clear()
	{
		m_ptr = 0;
	}

	bool Add(CBPacket& pack)
	{
		if (pack.m_mtype == -1)
			return true;

		int len = pack.m_size + MAX_PACKET_HEAD;

		if (m_ptr + len >= MAX_OUTPUTBUFFER_SIZE)
			return false;

		short size = pack.m_size;

		HTONS(pack.m_mtype);
		HTONL(pack.m_serial);
		HTONS(pack.m_size);

		memcpy(m_buf + m_ptr, &pack.m_mtype, sizeof(pack.m_mtype) ); m_ptr += 2;
		memcpy(m_buf + m_ptr, &pack.m_serial, sizeof(pack.m_serial) ); m_ptr += 4;
		memcpy(m_buf + m_ptr, &pack.m_size, sizeof(pack.m_size) ); m_ptr += 2;
		memcpy(m_buf + m_ptr, pack.m_buf, (int) size);		m_ptr += size;


		return true;
	}

	unsigned char* GetNextPoint()
	{
		if (m_ptr == 0)
			return NULL;
		return m_buf;
	}

	int GetRemain()
	{
		if (m_ptr > MAX_OUTPUTBUFFER_OUTUNIT)
			return MAX_OUTPUTBUFFER_OUTUNIT;
		else
			return m_ptr;
	}

	bool Update(int val)
	{
		if (val < 0)
			return false;

		if (m_ptr <= val)
		{
			m_ptr = 0;
			return true;
		}

		else
		{
			unsigned char* buf = g_Boutbufpool.Get();
			memcpy(buf, m_buf + val, sizeof(unsigned char) * (m_ptr - val));
			m_ptr -= val;
			g_Boutbufpool.Set(m_buf);
			m_buf = buf;
			return false;
		}
	}
};

#endif // !defined(AFX_OUTPUTBUFFER_H__0A40D7F4_947A_4BBF_ACA4_9FB66CE4116E__INCLUDED_)
