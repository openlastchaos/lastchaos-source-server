#include "stdhdrs.h"
#include "Server.h"
#include "MemPos.h"

CMemPosData::CMemPosData(int zone, float x, float z, char ylayer, const char* comment)
: m_comment(150)
{
	m_zone = zone;
	m_x = (int)x;
	m_z = (int)z;
	m_ylayer = ylayer;
	m_comment = comment;
}

CMemPos::CMemPos()
{
	m_count = 0;
	memset(m_data, 0, sizeof(CMemPosData*) * MAX_MEMPOS);
}

CMemPos::~CMemPos()
{
	for (int i = 0; i < MAX_MEMPOS; i++)
	{
		if (m_data[i])
		{
			delete m_data[i];
			m_data[i] = NULL;
		}
	}
	m_count = 0;
}

bool CMemPos::Write(int slot, int zone, float x, float z, char ylayer, const char* comment)
{
	if (slot < 0 || slot >= MAX_MEMPOS)
		return false;

	if (m_data[slot])
	{
		// °»˝Ĺ
		m_data[slot]->m_zone = zone;
		m_data[slot]->m_x = (int)x;
		m_data[slot]->m_z = (int)z;
		m_data[slot]->m_ylayer = ylayer;
		m_data[slot]->m_comment = comment;
	}
	else
	{
		m_count++;
		m_data[slot] = new CMemPosData(zone, x, z, ylayer, comment);
	}

	return true;
}

bool CMemPos::Delete(int slot)
{
	if (slot < 0 || slot >= MAX_MEMPOS)
		return false;

	if (m_data[slot])
	{
		m_count--;
		delete m_data[slot];
		m_data[slot] = NULL;
	}

	return true;
}
#ifdef PRIMIUM_MEMORYBOOK
CMemPosPuls::CMemPosPuls()	
{	
	m_count = 0;
	memset(m_data, 0, sizeof(CMemPosData*) * MAX_MEMPOS_PRIMIUM);
}

CMemPosPuls::~CMemPosPuls()
{	
	for (int i = 0; i < MAX_MEMPOS_PRIMIUM; i++)
	{
		if (m_data[i])
		{
			delete m_data[i];
			m_data[i] = NULL;
		}
	}
	m_count = 0;
}

bool CMemPosPuls::Write(int slot, int zone, float x, float z, char ylayer, const char* comment)
{
	if (slot < 0 || slot >= MAX_MEMPOS_PRIMIUM)
		return false;

	if (m_data[slot])
	{
		// °»˝Ĺ
		m_data[slot]->m_zone = zone;
		m_data[slot]->m_x = (int)x;
		m_data[slot]->m_z = (int)z;
		m_data[slot]->m_ylayer = ylayer;
		m_data[slot]->m_comment = comment;
	}
	else
	{
		m_count++;
		m_data[slot] = new CMemPosData(zone, x, z, ylayer, comment);
	}

	return true;
}

bool CMemPosPuls::Delete(int slot)
{
	if (slot < 0 || slot >= MAX_MEMPOS_PRIMIUM)
		return false;

	if (m_data[slot])
	{
		m_count--;
		delete m_data[slot];
		m_data[slot] = NULL;
	}

	return true;
}
#endif	// PRIMIUM_MEMORYBOOK