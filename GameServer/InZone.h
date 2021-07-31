#ifndef __INZONE_H__
#define __INZONE_H__

#include "Zone.h"

#ifdef RAID

#define MAX_INZONE_PC_COUNT		32		// 32명

#define MSG_JOINTYPE_PARTY		1		// 파티 
#define MSG_JOINTYPE_EXPED		2		// 원정대 	

#define MSG_RECYCLETYPE_1DAY	1		// 주기 1일 
#define MSG_RECYCLETYPE_7DAY	2		// 주기 7일(1주일) 


class CInZone : public CZone
{
private:
	int		m_nZoneNo;							// 존 인덱스	
	int		m_nRoomNum;							// 인스턴스 번호(룸번호)

	int		m_nJoinType;							// 입장구분(파티,원정대)
	int		m_nRecycleType;							// 재가동 주기
	int		m_nCurPCCount;							// 현재 인존에 있는 pc 수 
	int		m_listCharIndex[MAX_INZONE_PC_COUNT];	// 캐릭터 인덱스
public:

	CInZone();
	CInZone(int nZoneNum, int nRoomNum, int nJoinType, int nRecycleType);
	~CInZone();

	int Join(CPC* ch);										// 인존 입장
	bool Quit(CPC* ch,int nTargeZone, int nTargetPosIndex);	// 인존 나감

 	bool LoadData();										// 저장 되어 있는 인존 데이타 로딩
 	bool SaveData();										// 인존 정보 저장(네이드 몬스터 킬링시)
 	bool ResetData();										// 인존 정보 초기화

	int GetCurPCCount(){ return m_nCurPCCount; }			// 인존 안에 PC 수 

};

#endif //RAID

#endif //__INZONE_H__
