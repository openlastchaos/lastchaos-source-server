#ifndef __AAREA_H__
#define __AAREA_H__

#include "Env.h"
#include "Cell.h"
#include "MessageType.h"
#include "MonsterCombo.h"

class CZone;

class CArea  
{
public:
	CArea();
	~CArea();

	bool		m_bEnable;			// 활성화 여부
	CZone*		m_zone;				// 소속 존
	int			m_index;			// 영역 번호

	int			m_size[2];			// 배열 크기: 0: width, 1: height: 셀 단위
	int			m_ycnt;
	CEnv***		m_env;				// 환경 배열
	CCell**		m_cell;				// 셀 배열

	CNPCRegenList m_npcRegenList;	// NPC 리젠 정보 리스트
#ifdef ADULT_SERVER_EXTRA
	CNPCRegenList m_cityNpcRegenList;	// 마을 속성에 따른 NPC 리젠 정보 리스트
#endif //ADULT_SERVER_EXTRA
#ifdef MONSTER_RAID_SYSTEM
	CLCList<CNPCRegenInfo*> m_raidNPCRegenList;		// RAID MONSTER 리젠 정보 리스트
#endif // MONSTER_RAID_SYSTEM
#ifdef EVENT_XMAS_2006
	CLCList<CNPCRegenInfo*> m_eventXMas2006RegenList;
#endif // EVENT_XMAS_2006

#ifdef ALTERNATE_MERCHANT
	CLCList<CNPCRegenInfo*> m_AlternateMerchantList;
#endif // ALTERNATE_MERCHANT

	CNPC*		m_npcList;			// NPC 리스트
	int			m_nCountNPC;		// NPC 수
	int			m_pulseNPCAI;		// NPC AI pulse
	int			m_pulseNPCRegen;	// NPC regen pulse
	// int			m_nMaxCountNPC;		// NPC 최대 수

	int			m_nJunkItems;		// 사라진 아이템 수

	int			m_nMakeNPC;		// 싱글던전에서 만든 몹 수
	bool		m_bBoss;		//  발록방 한번 터치패드 통과했는가? : 2번째 통과시 발록방, 첫번째는 2층 
	int*		m_regenList;	// 싱글던전에서 리젠되었던 리스트(regen의 a_index)

	int			m_guildIndex;		// 길드룸의 경우 길드 인덱스

#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
	int			m_nNPC_Killed_NPC;
#endif  // NIGHTSHADOW_SKILL

	// 초기화
	void Init(CZone* z, int index, int w, int h, int ycnt);

	// x, z에서 셀 번호 얻음
	void PointToCellNum(float x, float z, int* cx, int* cz);

	// 캐릭을 셀에 넣기
	void CharToCell(CCharacter* ch, char cy, int cx, int cz);

	// 셀에서 캐릭 빼기
	void CharFromCell(CCharacter* ch, bool bDeleteAttackList);

	// 캐릭 이동
	bool MoveChar(CCharacter* ch, char y, float x, float z, float h, float r, MSG_MOVE_TYPE movekind, CCharacter* target = NULL);

	// 캐릭 이동 시에 블럭되는지 검사
private:
	bool IsNotBlocked(char sy, float sx, float sz, float tx, float tz, bool bCastleNPC, char& ty);		// s에서 t로 이동 가능 여부 검사 : 밑의 캐릭터용 InNotBlocked()에서 사용
public:
	bool IsNotBlocked(CCharacter* ch, CCharacter* tch, bool battack, char& outYlayer);
	bool IsNotBlocked(CCharacter* ch, char y, float x, float z, bool battack, char& outYlayer);

	// 특정 위치의 Attribute를 얻음
	unsigned char GetAttr(char y, float x, float z);
	// 높이 값 얻기
	float GetHeight(char y, float x, float z);

	// ch 근처에서 tch_type 종류의 idx 캐릭을 반환
	CCharacter* FindCharInCell(CCharacter* ch, int idx, MSG_CHAR_TYPE tchType, bool bIncludeDead = false);
	CNPC* FindProtoNPCInCell(float x, float z, int nData, bool bIsFlagMask, int findNumCell, bool bIncludeDead = false);
	CNPC* FindProtoNPCInCell(CCharacter* ch, int nData, bool bIsFlagMask, int findNumCell, bool bIncludeDead = false);

	// area의 tch를 찾음
	CCharacter* FindChar(int idx, MSG_CHAR_TYPE tchType);

	// 셀에 메시지 전달 : 셀에서 CELL_EXT 반경안의 모든 PC에게 전달한다
	bool SendToCell(CNetMsg& msg, CCharacter* ch, bool withme = false, int cellext = CELL_EXT);
	bool SendToCell(CNetMsg& msg, char y, int cx, int cz, int cellext = CELL_EXT);
	// 해당 area의 모든 PC에게 msg 전달
	void SendToAllClient(CNetMsg& msg);

	// 셀의 정보를 ch에게 전달
	// bool bExcludeOwnPetElemental : ch의 소유인 펫과 소환수를 제외
	void SendCellInfo(CPC* ch, bool bExcludeOwnPetElemental);

	// 특정 셀의 인접 지역의 PC 수를 반환
	int GetPCCount(CCharacter* ch, int findArea = CELL_EXT);
	int GetPCCount(int cx, int cz, int findArea = CELL_EXT);

	// 리젠 정보 설정
	void LoadNPC();
	
	// NPC를 추가한다
	void AddNPC(CNPC* npc);

	// NPC를 제거
	void DelNPC(CNPC* npc);

	// Mob의 상태 이상 등을 적용
	CNPC* MobTimeCheck(CNPC* npc);

	// 영역 활성화
	void SetEnable();

	// 영역 비활성화
	void SetDisable();

	// 몹 행동
	void MobActivity();

	// 몹 리젠
	void MobRegen();
#ifdef MONSTER_RAID_SYSTEM
	// 레이드 몬스터 리젠, 하나라도 리젠되면 true, 아니면 false
	bool RaidMobRegen(float x, float z, float r, char y);
#endif // MONSTER_RAID_SYSTEM

#ifdef EVENT_XMAS_2006
	bool EventXMas2006NPCRegen(float x, float z, float r, char y);
#endif // EVENT_XMAS_2006

#ifdef ALTERNATE_MERCHANT
	bool AlternateMerchantRegen( float x, float z, float r, char y );
#endif // ALTERNATE_MERCHANT

	// npc와 가까운 공격대상을 찾기
	CCharacter* FindNearestTarget(CNPC* npc, float* dist);

#ifdef NIGHTSHADOW_SKILL	// 나이트 쉐도우의 몬스터 시스템
	// 테이밍된 npc가 가까운 npc를 찾기
	CCharacter*	FindNearestTarget_Taming_or_Confused(CNPC* npc, float* dist);

	// 몹의 행동 루틴 독립적으로 수행
	void	MobActivity_Taming(CNPC* npc);
	void	MobActivity_Confused(CNPC* npc);

	// m_npcList가 꼬이는 것을 위한 동기화
	void	Monster_synchronization( int nCount );
#endif  // NIGHTSHADOW_SKILL
	
#ifdef RAID_MONSTER_SKILL  // 레이드 몬스터 스킬
	// 호출 스킬을 사용한 npc찾는 함수
	void	MobActivity_Find_callingNPC( CNPC* npc );
#endif // RAID_MONSTER_SKILL

	// 아이템을 셀에 넣기
	void ItemToCell(CItem* item, char cy, int cx, int cz);

	// 아이템에서 캐릭 빼기
	void ItemFromCell(CItem* item);

	// 아이템 땅에 떨구기
	CItem* DropItem(int item_db_idx, CCharacter* ch, int plus, int flag, LONGLONG count, bool bOption = false);
	CItem* DropItem(CItem* item, CCharacter* ch);

	// 떨어진 아이템 정리
	void GroundItemControl();

	// aitype에 해당하는 동족 찾기, aitype이 -1이면 무시, hpParam미만의 체력이 남은 대상(단위는%)
	CNPC* FindNearestFamily(CNPC* npc, int aitype = -1, int hpParam = 100);

	// 영역안에 있는 캐릭터 모두 이동 시키기
	void MoveAllChar(int nIndex, int nExtra);

#ifdef MONSTER_COMBO
	CMonsterCombo* m_monsterCombo;
	void RunComboZone();
	int  CountPCInExtra(int extra, bool bDead);
	int	 CountMobInCombo();
	void CloseComboZone();
#endif // MONSTER_COMBO

};

void GoZone(CPC* ch, int zone, char ylayer, float x, float z);
void GoTo(CCharacter* ch, char y, float x, float z, float h, float r, char bHack = 0);
void GMGoZone(CPC* ch, CPC* pc);
#endif
