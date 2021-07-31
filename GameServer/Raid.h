#ifndef __RAID_H__
#define __RAID_H__

#include "stdhdrs.h"

#ifdef RAID

#define MAX_INZONE_ROOM_COUNT	10		// 최대 INZONE 수
#define MAX_INZONE_PC_COUNT		32		// 32명

#define MSG_RAIDTYPE_FIELD		1		// 필드
#define MSG_RAIDTYPE_INZONE		2		// 인스던스존 	

//레이드 클래스
class CRaid 
{
protected:
	int	m_nRaidType;			// 필드,인존 구분(수행 장소)
	int	m_nJoinType;			// 파티,원정대 구분
	int	m_nRecycleType;			// 재가동 주기 구분(1일,1주일)

public:
	int	 GetRaidType(){ return m_nRaidType; };			// 레이드 타입 획득
	int  GetJoinType(){ return m_nJoinType; };			// 입장 타입 획득
	int  GetRecycleType(){ return m_nRecycleType; };	// 재가동 타입 획득

};

//일반 필드 레이드
//NPC 정보 ,PC 정보, 아이템 정보, 트리거 정보, 퀘스트 정보
//데이타 저장
class CFieldRaid: public CRaid
{
public:
	CFieldRaid(int nRaidType, int nJoinType);
	~CFieldRaid();
};



#endif //RAID

#endif	//__RAID_H__