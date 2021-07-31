#include "Raid.h"
#include "stdhdrs.h"
#include "Server.h"
#include "DBCmd.h"
#include "CmdMsg.h"
#include "Log.h"

#ifdef RAID

//CFieldRaid Class
//-->
CFieldRaid::CFieldRaid(int nRaidType, int nJoinType)
{
	m_nRaidType	= nRaidType;
	m_nJoinType = nJoinType;
	m_nRecycleType = -1;
}

CFieldRaid::~CFieldRaid()
{
	m_nRaidType	= -1;
	m_nJoinType = -1;
	m_nRecycleType = -1;
}
//<--
//CFieldRaid Class

#endif //RAID 