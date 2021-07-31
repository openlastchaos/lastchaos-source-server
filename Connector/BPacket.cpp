// BPacket.cpp: implementation of the CBPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include "BPacket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBPacketPool g_Bpacket_pool;

CBPacket::CBPacket()
{
	m_mtype = -1;
	m_serial = 0;
	m_buf = NULL;
	m_ptr = 0;
	m_size = 0;

}

CBPacket::~CBPacket()
{

}

void CBPacket::Init()
{
	m_mtype = -1;
	if (!m_buf)
	{
		m_buf = g_Bpacket_pool.Get(false);
	}
	m_serial = 0;
	m_size = 0;
	m_ptr = 0;

}

void CBPacket::Init(short mtype)
{
	Init();
	m_mtype = mtype;
	if (!m_buf)
	{
		m_buf = g_Bpacket_pool.Get(false);
	}
	m_serial = 0;
	m_size = 0;
	m_ptr = 0;

	//short ubtype = m_ptype;
	//*this << ubtype;
}

void CBPacket::Init(const CBPacket& src)
{
	Init();
	m_mtype = src.m_mtype;
	m_size = src.m_size;
	m_serial = src.m_serial;
	memcpy(m_buf, src.m_buf, (int) m_size);
	m_ptr = src.m_ptr;
}


void CBPacket::Read(void* buf, long size)
{
	if (size < 1 || !CanRead(size))
		return ;
	memcpy(buf, m_buf + m_ptr, size);
	m_ptr += size;
}

void CBPacket::Write(void* buf, long size)
{
	if (size < 1 || !CanWrite(size))
		return ;
	memcpy(m_buf + m_ptr, buf, size);
	m_ptr += size;
	if ((short) m_ptr > m_size)
		m_size = (short) m_ptr;
}

void CBPacket::WriteRaw(void* buf, long size)
{
	Write(buf, size);
	m_size = (short) size;
	m_ptr = 0;
	MoveFirst();
}


CBPacket& CBPacket::operator >> (char* str)
{
	int i = 0;
	while (i <= 31)
	{
		*this >> (*str);
		str++;
		i++;
	}
	
	return *this;
}

CBPacket& CBPacket::operator >> (CLCString& str)
{
	char* tmp = new char[str.BufferSize()-1];

	memcpy(tmp, m_buf+m_ptr, str.BufferSize() - 1);
	m_ptr += str.BufferSize() -1;
	str = tmp;
	
	return *this;
}

CBPacket& CBPacket::operator << (CLCString& str)
{

	memcpy(m_buf + m_ptr, (const char*) str, str.BufferSize() - 1 );
	m_ptr += str.BufferSize() - 1;
	
	return *this;
}

CBPacket& CBPacket::operator << (const char* str)
{
	int i = 0;
	while (i <= 31)
	{
		*this << (*str);
		str++;
		i++;
	}
	
	return *this;
}