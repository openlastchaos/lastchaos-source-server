#ifndef __BATTLE_H__
#define __BATTLE_H__

bool IsPK(CPC* of, CCharacter* df);
bool IsAvailableAttack(CCharacter* of, CCharacter* df);
void ProcAfterHit(CCharacter* of, CCharacter* df);
void ProcDead(CNPC* df, CCharacter* of);
void ProcDead(CPC* df, CCharacter* of);
void ProcDead(CPet* df, CCharacter* of);
void ProcDead(CElemental* df, CCharacter* of);
#ifdef ATTACK_PET
void ProcDead(CAPet* df, CCharacter* of);
#endif //ATTACK_PET
void ProcDropItemAfterBattle(CNPC* df, CPC* opc, CPC* tpc, int level);
int ProcAttack(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE damageType, CSkill* skill, int magicindex, int damage = 0);

// 리더 사망시 처리부분
void ProcFollowNPC(CNPC* npc);
bool CheckInNearCellExt(CCharacter* ch, CCharacter* tch);
char GetHitType(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type);
#if defined (ENABLE_ROGUE_SKILL125_BRZ)
char SelectHitType(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel, bool nothelp, int skillLevel = 0);
#else
char SelectHitType(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel, bool nothelp);
#endif // ENABLE_ROGUE_SKILL125_BRZ
int GetDamage(const CCharacter* of, const CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel);
CAttackChar* FindMaxDamage(CNPC* npc);
#ifdef NEW_DIVISION_EXPSP
CPC* FindPreferencePC(CNPC* npc, int* level, LONGLONG* pnTotalDamage);
// 경험치 분배 : 051228 : BS 새로 작성
bool DivisionExpSP(CNPC* npc, CPC* pPreferencePC, LONGLONG nTotalDamage);
#else // #ifdef NEW_DIVISION_EXPSP
CPC* FindPreferencePC(CNPC* npc, int* level);
void DivisionExpSPParty(CParty* party, CNPC* npc, CPC* preferencePC);
bool DivisionExpSP(CNPC* npc, CPC* preferencePC);
#endif // #ifdef NEW_DIVISION_EXPSP
void DivisionPartyMoney(CPC* pc, CItem* item);
bool GetItemRandomParty(CPC* pc, CItem* item);
CAttackChar* GetMaxHateTarget(CNPC* npc);
CCharacter* CheckAttackPulse(CNPC* npc);
// tch의 attackchar에서 ch에 해당 하는 것 리턴
CAttackChar* AddAttackList(CCharacter* ch, CCharacter* tch, int hate);
void DelAttackList(CCharacter* ch);
void DelTargetFromAttackList(CCharacter* ch, CCharacter* tch);
void FindFamilyInNear(CCharacter* of, CNPC* npc);
bool GetSkillPrototypes(const CSkillProto* proto, int level, int magicindex, const CSkillLevelProto** levelproto, const CMagicProto** magic, const CMagicLevelProto** magiclevel);
void ApplyHateByDamage(CCharacter* of, CCharacter* df, char hittype, int damage);
// tch의 attackchar에서 ch에 해당 하는 것 리턴
CAttackChar* ApplyHate(CCharacter* of, CCharacter* df, int hate, bool bApplyFamily);
void ApplyDamage(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE damageType, const CSkillProto* proto, const CMagicProto* magic, int damage, char flag);
void CalcPKPoint(CPC* of, CPC* df, bool bDeadPet);

// 정당방위 리스트 서로 추가
void AddRaList(CPC* of, CPC* df);

// 정당방위 리스트에 있는가 확인
bool IsRaList(CPC* of, CPC* df);

// 060318 : bs : 정방에서 of가 df를 선공하였는지 검사
#ifdef NEW_PK
bool IsFirstAttackInAttackList(CPC* of, CPC* df);
#endif


#ifdef ENABLE_WAR_CASTLE
// 공성 포인트 계산하는 함수
void CalcWarPoint(CCharacter* of, CCharacter* df);
#endif // #ifdef ENABLE_WAR_CASTLE

#ifdef ENABLE_PET
void ProcDeathPet(CPet* pet, const char* attackerType, int attackerIndex, const char* attackerName);
bool DropPetItem(CPet* pet);
#endif // #ifdef ENABLE_PET

// base 기분으로 n1과 n2의 AI 우선순위 비교
// n1이 높으면 -1, n2가 높으면 1, 같으면 0
int AIComp(CNPC* base, CNPC* n1, CNPC* n2);

#ifdef EVENT_TEACH_2007
bool IsTeachAndStudent( CParty* pParty );
#endif //EVENT_TEACH_2007

#ifdef ADULT_SERVER_NEW_BALANCE
char GetHitType_PvP(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type);
char SelectHitType_PvP(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel, bool nothelp);
char GetHitType_Adult(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type);
char SelectHitType_Adult(CCharacter* of, CCharacter* df, MSG_DAMAGE_TYPE type, char flag, const CMagicProto* magic, const CMagicLevelProto* magiclevel, bool nothelp);
#endif // ADULT_SERVER_NEW_BALANCE

#endif // __BATTLE_H__
