#if !defined(AFX_BINPUTBUFFER_H__A5EDE24B_050E_4EE6_81E1_94310CE034ED__INCLUDED_)
#define AFX_BINPUTBUFFER_H__A5EDE24B_050E_4EE6_81E1_94310CE034ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemPool.h"
#include "Mutex.h"	// Added by ClassView
#include "BPacket.h"

extern CInputBufferPool g_Binbufpool;

class CBInputBuffer
{
public:
	unsigned char*	m_buf;
	int				m_ptr;

	CBInputBuffer()
	{
		m_buf = g_Binbufpool.Get(false);
		m_ptr = 0;
	}

	virtual ~CBInputBuffer()
	{
		g_Binbufpool.Set(m_buf);
		m_buf = NULL;
		m_ptr = 0;
	}

	void Clear()
	{
		memset(m_buf, 0, MAX_PACKET_HEAD);
		m_ptr = 0;
	}

	unsigned char* GetPoint()
	{
		return m_buf + m_ptr;
	}

	int GetRemain()
	{
		return MAX_INPUTBUFFER_SIZE - m_ptr;
	}

	void SetPoint(int dval)
	{
		m_ptr += dval;
	}



	/////////////////////
	// Function name	: GetMessage
	// Description	    : 입력 스트림 버퍼에서 메시지를 얻어온다
	// Return type		: int 
	//            		: 0: 정상, 1: 메시지 없음, -1: 오류
	// Argument         : CNetMsg& msg
	//                  : 얻어온 메시지
	int GetMessage(CBPacket& pack)
	{

		if (m_ptr < (int)MAX_PACKET_HEAD)
			return 1;

		unsigned char* p = m_buf;

		unsigned short	pack_type;
		int				pack_sn;
		short				size;

		//p += 1;
		memcpy(&pack_type,	p, 2);			p += 2;
		memcpy(&pack_sn,		p, 4);			p += 4;
		memcpy(&size,		p, 2);			p += 2;

		NTOHS(pack_type);
		NTOHL(pack_sn);
		NTOHS(size);

		pack.Init(pack_type);
		pack.m_serial = pack_sn;
		pack.m_size = size;

		if (size + MAX_PACKET_HEAD > m_ptr + MAX_PACKET_HEAD)
			return 1;

		if (size > MAX_PACKET_SIZE || size < 0)
			return -1;

		pack.WriteRaw(p, size);
		p += size;

		m_ptr -= MAX_PACKET_HEAD + size;
		if (m_ptr > 0)
		{
			unsigned char* tmp = g_Binbufpool.Get(false);
			memcpy(tmp, p, m_ptr);
			g_Binbufpool.Set(m_buf);
			m_buf = tmp;
		}
		return 0;
	}
};

#endif // !defined(AFX_INPUTBUFFER_H__A5EDE24B_050E_4EE6_81E1_94310CE034ED__INCLUDED_)
