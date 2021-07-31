#include "stdhdrs.h"
#include "Log.h"
#include "InZone.h"
#include "Server.h"
#include "DBCmd.h"
#include "CmdMsg.h"

#ifdef RAID

CInZone::CInZone()
{
	for(int i=0; i < MAX_INZONE_PC_COUNT; i++)
		m_listCharIndex[i] = -1;
}

CInZone::CInZone(int nZoneNum, int nRoomNum, int nJoinType, int nRecycleType)
{
	for(int i=0; i < MAX_INZONE_PC_COUNT; i++)
		m_listCharIndex[i] = -1;

	m_nZoneNo		= nZoneNum;
	m_nRoomNum		= nRoomNum;
	m_nJoinType		= nJoinType;				
	m_nRecycleType	= nRecycleType;
	m_nCurPCCount	= 0;
}

CInZone::~CInZone()
{
}

// 인존 입장
int CInZone::Join(CPC* ch)
{
	int nPosNum = 0;
	int nZoneNum = this->m_index;

	for(int i=0; i < MAX_INZONE_PC_COUNT; i++)
	{
		if(m_listCharIndex[i] == ch->m_index)	
			return MSG_RAID_ERROR_INZONE_JOIN_ALREADY;  //오류:이미 입장
	}

	for(i=0; i < MAX_INZONE_PC_COUNT; i++)
	{
		if(m_listCharIndex[i] == -1)
		{
			m_listCharIndex[i]	= ch->m_index;	
			m_nCurPCCount++;

			GoZone(ch, nZoneNum,
					m_zonePos[nPosNum][0],											// ylayer
					GetRandom(m_zonePos[nPosNum][1], m_zonePos[nPosNum][3]) / 2.0f,		// x
					GetRandom(m_zonePos[nPosNum][2], m_zonePos[nPosNum][4]) / 2.0f);	// z

			return MSG_RAID_ERROR_SUCESS;		//성공 
		}
	}

	return MSG_RAID_ERROR_INZONE_JOIN_FULL;	 //오류: 인원초과 
}


// 인존 나감
bool CInZone::Quit(CPC* ch,int nTargeZone, int nTargetPosIndex)
{
	int izoneindex = gserver.FindZone(nTargeZone);
	if (izoneindex == -1)
		return false;

	CZone* pZone = gserver.m_zones + izoneindex;

	if (nTargetPosIndex < 0 || nTargetPosIndex >= pZone->m_countZonePos)
		return false;

	for(int i=0; i < MAX_INZONE_PC_COUNT; i++)
	{
		if(m_listCharIndex[i] == ch->m_index)
		{
			m_listCharIndex[i]	= -1;	
			m_nCurPCCount--;

			GoZone(ch, nTargeZone,
					pZone->m_zonePos[nTargetPosIndex][0],														// ylayer
					GetRandom(pZone->m_zonePos[nTargetPosIndex][1], pZone->m_zonePos[nTargetPosIndex][3]) / 2.0f,		// x
					GetRandom(pZone->m_zonePos[nTargetPosIndex][2], pZone->m_zonePos[nTargetPosIndex][4]) / 2.0f);		// z

			return true;
		}
	}

	return false;
}

// 저장 되어 있는 인존 데이타 로딩
bool CInZone::LoadData()
{

	return true;
}

// 인존 정보 저장(네이드 몬스터 킬링시)
bool CInZone::SaveData()
{

	return true;
}

// 인존 정보 초기화
bool CInZone::ResetData()
{

	return true;
}

#endif  //RAID