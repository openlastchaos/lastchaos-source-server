#ifndef __DRATAN_CASTLE_H__
#define __DRATAN_CASTLE_H__

#ifdef DRATAN_CASTLE

#include "stdhdrs.h"
#include "Server.h"
#include "Log.h"
#include "CmdMsg.h"
#include "WarCastle.h"

#ifdef DYNAMIC_DUNGEON
class CDungeon
{
private:
	int m_nEnterFeeRate;
	int m_nHuntRate;

	int m_nMobRate;
	int m_nEnvRate;
	
	CZone * m_pZone;
	int m_nChangeTime;	
public:
	CDungeon();
	~CDungeon();

	void SetFeeRate(int nRate)	{ m_nEnterFeeRate = nRate; }
	void SetHuntRate(int nRate) { m_nHuntRate = nRate; }

	void SetMobRate(int nRate)	{ m_nMobRate = nRate; }
	void SetEnvRate(int nRate)	{ m_nEnvRate = nRate; }
	
	int GetFeeRate()	{ return m_nEnterFeeRate; }
	int	GetHuntRate()	{ return m_nHuntRate; }

	int GetMobRate()	{ return m_nMobRate; }
	int GetEnvRate()	{ return m_nEnvRate; }

	void ChangeFeeRate(int nRate);
	void ChangeHuntRate(int nRate);

	void ChangeEnvRate(int nRate);
	void ChangeMobRate(int nRate);
	
	

	void SetChangeTime(int nTime);
	int GetChangeTime();

	void SetZone(CZone * pZone);
	CZone * GetZone();

	void ChangeSetting();
};
#endif // DYNAMIC_DUNGEON

typedef struct _tagGuard
{
	int mobindex;
	float pos_z;
	float pos_x;
	float pos_h;
} GUARD;

class CGuardList
{
public:
	CGuardList();
	~CGuardList();
	CGuardList(int kind, int count);

private:
	int maxkind;
	int maxcount;
	int m_cnt;
	int m_maxcnt;

	GUARD * m_pGardList;

public:
	void init(int kind, int count);
	bool InsertGuard(int idx, float x, float z, float h);
	int GetCount(int idx);
	int GetCount();
	GUARD * GetGuardList(int idx);
	int GetMaxCount();
	int GetMaxKindCount() { return maxcount;}
	void clean();
};

class CDratanCastle : public CWarCastle
{
public:
	static CDratanCastle * CreateInstance();
	
	enum 
	{
		TOWER_WARP = 6,
	};

private:
	CDratanCastle();
	CDratanCastle(const CDratanCastle & rhs);

	static CDratanCastle * pInstance;

	enum 
	{ 
		COUNT_DRATAN_GATE_NPC = 3,
#ifdef TEST_SERVER
		WAR_TIME = 60 * 60,
#else
		WAR_TIME = 2 * 60 * 60,
#endif // TEST_SERVER
//		WAR_TIME = 2 * PULSE_REAL_HOUR, 
	};
	

	int m_respondCnt;			// 크리스탈과 교감하고 있는 캐릭수
	int m_RespondMember[5];		// 교감 캐릭터 인덱스
	char m_nMasterTower[7];		// 
	// 공격형 타워	근거리 공격형, 원거리 공격형, 마법 공격형, 가드형 타워 속도 하락형, 능력치 하락형, 독 중독형, 워프용 워프 연결

	char m_reinforce[3];	// 강화수치
	bool m_bMaster;

public:
	CNPC *  m_gateNPC[COUNT_DRATAN_GATE_NPC];		// 성문 지키는 수호상
	CGuardList  m_GuardList;	// 3종류, 종류당 10개이상 배치 불가
	CNPC *		m_pWarpNPC[5];		// 워프 NPC
	CNPC *		m_pRebrithNPC[7];	// 부활 진지 NPC
	int			m_nRebrithGuild[7];		// 부활 진지 점령 길드 인덱스
	char *		m_strRebrithGuild[7];	// 부활 진지 점령 길드 이름
	bool		m_bLoop;				// 운영자 명령으로 공성 멈춤
	CNPC*		m_pDungeonNPC;			// 지하던전 NPC

#ifdef DYNAMIC_DUNGEON
	CDungeon	m_dvd;					// 공성 던전
#endif // DYNAMIC_DUNGEON
	
// virtual function 구현
protected:
	void SetGateNPC(CNPC* npc);
	void InitGateNPC();
	void InitRegenPosition();

public:
	int GetZoneIndex() { return ZONE_DRATAN; }
	void SetNextWarTime(int nextWarTime);
	bool SetNextWarTime(int wday, int hour);
	bool IsJoinTime();
	void GetJoinTime(struct tm* nextJoinTime, bool bHumanable);
	int GetRegenPoint(int joinflag, CPC* ch);
	void UpdateGateState(CNPC* npc);
	bool IsOpenGate();
	bool GiveLordItem(CPC* pc);
	bool TakeLordItem(CPC* pc);
	int GetLordItemIndex(char job1, char job2);
	bool IsDefenseWarpPoint(int pos);
	int GetGateNPCCount() { return (GetGateNPC(0) == NULL) ? 0 : COUNT_DRATAN_GATE_NPC; }
	CNPC* GetGateNPC(int idx) { return (idx < 0 || idx >= COUNT_DRATAN_GATE_NPC) ? NULL : m_gateNPC[idx]; }
	void GetInnerCastleRect(char* nYlayer, int* nX0x2, int* nZ0x2, int* nX1x2, int* nZ1x2);
// --- virtual function 구현
	
	void CheckWarCastle();
	bool CheckSubServer();
	int GetCurSubServerCastleZoneIndex();
	int CheckJoin(CPC* ch);	
	void CheckWarCastleNotice();
	void CheckWarCastleStartWar();
	void CheckWarCastleStartWarCastle();
	void GetNextWarTime(struct tm* nextWarTime, bool bHumanable);
	int GetNextWarTime() { return m_nextWarTime; }
	void CheckWarCastleEnd();

	void CheckWarCastleNPC();
	void RegenCastleNPC();
	void RemoveCastleNPC();
	void RemoveDungeonNPC();

	void SetRegenPoint(int nRegenPos, int nGuildIndex);
	void RegenLordSymbol();

	void RegenCastleTower();
	void RegenShop();
	void RemoveCastleTower();
	void RemoveLordSymbol();
	void RegenGuardNPC();
	void RemoveGuardNPC();
	bool LoadCastleData();
	
	CArea * GerArea();
	
	int GetRespondCount() { return m_respondCnt; }
	void SetRespondCount(int cnt) { m_respondCnt = cnt;}

	void InitMasterTower();

	char GetTowerStatus(int i);
	bool SetTowerStatus(int i, char values);
	
	void SetOwner(CGuild* guild);
	void ResetOwner();

	void CheckRespond(CPC * ch);
	bool SetRespondMember(CPC * ch);
	void ResetRespondMeber(CPC * pc);
	void InitRespond();
	char GetReinforceStep(int i);
	void SetReinforceStep(char i, char v);
	bool CheckBuyTower(int index);
	void InitReinforce();
	void SetTowerReinforce(CNPC * npc);
	void CheckCastleControlNPC();
	void RegenCastleControlNPC();

	void InitWarpNPC();
	void SetWarpNPC(CNPC * npc);
	void SetRebrithNPC(CNPC* npc);
	void InitRebrithNPC();
	void ResetRebrithNPC();

	void ChangeSide(int gindex);
	void RegenCastleTowerHalf();
	int GetRemainWarTime();

	void GetOutNonDefense();
	bool IsInInnerCastle(CCharacter* pChar);
	void RemoveCastleTowerHalf();
	bool CanLordChat(CPC* pc);
	
	void ChangeAttackGuild(int gindex);

	bool AddAttackGuild(int index);
	bool AddDefenseGuild(int index);
	bool IsAttackGuild(int index);
	bool IsDefenseGuild(int index);
	
	void RemoveCastleControlNPC();
	void MoveToRegenPoint();
	
	void RemoveRebirthNPC();
	void RegenRebirthNPC();

	void StopCastleTower();

	// 공성이 끝났을 경우 수성만 빼고 전부 Dartan 마을로 강제 이동
	void EndWarRegenPoint();

#ifdef DYNAMIC_DUNGEON
	// 공성 던전의 캐릭 이동 시키기
	void MoveingDVD();

	// 공성 난이도 하락
	void CheckDVD();
#endif // DYNAMIC_DUNGEON
};
#endif // DRATAN_CASTLE
#endif // __DRATAN_CASTLE_H__