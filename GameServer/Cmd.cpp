#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
CCmdList::CCmdList()
{
	m_cnt = 0;
}

void CCmdList::AddMessage()
{
	m_cnt = 0;

	// Chatting
	Add(MSG_CHAT, do_Chat, 0, true, true);			// 채팅

	// 이동 관련
	Add(MSG_MOVE, do_Move, 0, false, false);			// 이동
	Add(MSG_GO_ZONE, do_GoZone, 0, false, false);		// 존이동

//N1InPersonalDungeon : bw
	Add(MSG_PD_ATTACK, do_pd_Attack, 0, false, true);
	Add(MSG_PD_MOVE, do_pd_Move, 0, false, true);


	// 공격 관련
	Add(MSG_ATTACK, do_Attack, 0, false, false);

	// 아이템 관련
	Add(MSG_ITEM, do_Item, 0, false, false);

	// 교환 관련
	Add(MSG_EXCHANGE, do_Exchange, 0, false, false);

	// 장소 기억 관련
	Add(MSG_MEMPOS, do_MemPos, 0, false, false);

	// 운영자 관련
	Add(MSG_GM, do_GM, 1, true, true);

	// 파티 관련
	Add(MSG_PARTY, do_Party, 0, false, false);

	// 퀵슬롯관련
	Add(MSG_QUICKSLOT, do_QuickSlot, 0, false, true);

	// 싱글던전 - NPC 리젠
	Add(MSG_NPC_REGEN, do_NPCRegen, 0, false, true);

	// 스킬
	Add(MSG_SKILL, do_Skill, 0, false, false);

	// 부활메시지
	Add(MSG_PC_REBIRTH, do_Rebirth, 0, false, false);

	// 액션 메시지
	Add(MSG_ACTION, do_Action, 0, false, false);

	// 퀘스트
	Add(MSG_QUEST, do_Quest, 0, false, false);

	// 성장 포인트 관련
	Add(MSG_STATPOINT, do_StatPoint, 0, false, true);

	// 이동 장치 관련
	Add(MSG_WARP, do_Warp, 0, true, false);

	// 펄스 관련
	Add(MSG_PULSE, do_Pulse, 0, true, true);

	// 특수스킬 관련
	Add(MSG_SSKILL, do_SSkill, 0, false, false);

	Add(MSG_PK, do_PK, 0, false, false);

	Add(MSG_EVENT, do_Event, 0, false, false);

	Add(MSG_PERSONALSHOP, do_PersonalShop, 0, false, false);
	
	Add(MSG_STASH, do_Stash, 0, false, false);
	
	Add(MSG_GUILD, do_Guild, 0, false, false);

	Add(MSG_CHANGE, do_Change, 0, false, false);

	Add(MSG_UI, do_UI, 0, false, false);

	Add(MSG_TEACH,	do_Teach, 0, false, false);

#ifdef ENABLE_CHANGEJOB
	Add(MSG_CHANGEJOB, do_ChangeJob, 0, false, false);
#endif

#ifdef ENABLE_MESSENGER
	Add(MSG_FRIEND,	do_Friend, 0, false, false);
#endif

	Add(MSG_SELECT_PRODUCE, do_SelectProduce, 0, false, false);

	Add(MSG_EXTEND, do_Extend, 0, true, false);

	Add(MSG_RANDOM_PRODUCE, do_RandomProduce, 0, false, false);
	
#ifdef TRADE_AGENT
	Add(MSG_TRADEAGENT, do_TradeAgent, 0, false, false);
#endif
	
#ifdef EXPEDITION_RAID
	Add(MSG_EXPEDITION, do_Expedition, 0, false, false);
	Add(MSG_LOGOUT, do_Logout, 0, false, false);
#endif

#ifdef FACTORY_SYSTEM
	Add(MSG_FACTORY, do_Factory, 0, false, false);
#endif

	Sort();
}

void CCmdList::AddExMessage()
{
	m_cnt = 0;

#ifdef ENABLE_PET
	Add(MSG_EX_PET_CALL, do_ExPetCall, 0, false, false);
	Add(MSG_EX_PET_LEARN, do_ExPetLearn, 0, false, false);
	Add(MSG_EX_PET_RESET_SKILL, do_ExPetResetSkill, 0, false, false);
	Add(MSG_EX_PET_CHANGE_MOUNT, do_ExPetChangeMount, 0, false, false);
	Add(MSG_EX_PET_COMMAND, do_ExPetCommand, 0, false, false);
	Add(MSG_EX_PET_MIX_ITEM, do_ExPetMixItem, 0, false, false);
	Add(MSG_EX_PET_WARPTOWN, do_ExPetWarpTown, 0, false, false);
	Add(MSG_EX_PET_CHANGE_ITEM, do_ExPetChangeItem, 0, false, false);
	Add(MSG_EX_PET_REBIRTH, do_ExPetRebirth, 0, false, false);
#ifdef PET_NAME_CHANGE
	Add( MSG_EX_PET_CHANGE_NAME, do_PetNameChange, 0, false, false );
#endif // PET_NAME_CHANGE
#endif // #ifdef ENABLE_PET

#ifdef FEEITEM
	Add(MSG_EX_NAMECHANGE,	do_Ex_NameChange, 0, false, false);		// 이름 변경 아이템	: tab_idx(uc) row_idx(uc) col_idx(uc) item_idx(n) name(str)
	Add(MSG_EX_CASHITEM,	do_Ex_CashItem, 0, false, false);
#endif // FEEITEM

#ifdef ENABLE_WAR_CASTLE
	Add(MSG_EX_CASTLE_MAP_RECENT, do_ExCastleMapRecent, 0, false, false);
	Add(MSG_EX_CASTLE_MAP_SIGNAL, do_ExCastleMapSignal, 0, false, false);
#endif // ENABLE_WAR_CASTLE

	Add(MSG_EX_EVOCATION_STOP, do_ExEvocationStop, 0, false, false);

	Add(MSG_EX_PARTY_RECALL, do_ExPartyRecall, 0, false, false);

#ifdef PARTY_MATCHING
	Add(MSG_EX_PARTY_MATCH, do_ExPartyMatch, 0, false, false);
#endif // PARTY_MATCHING
#ifdef MESSENGER_NEW
	Add(MSG_EX_MESSENGER, do_ExMessenger, 0, false, false);
#endif

#ifdef PRIMIUM_MEMORYBOOK
	Add(MSG_EX_MEMPOSPLUS, do_MemPosPlus, 0, false, false);
#endif	// PRIMIUM_MEMORYBOOK

#ifdef USING_NPROTECT
	Add(MSG_EX_NPROTECT, do_ExNProtect, 0, true, true);
#endif // USING_NPROTECT

#ifdef DRATAN_CASTLE
	Add(MSG_EX_CASTLE_WAR, do_ExCastleWar, 0, false, false);
#ifdef DYNAMIC_DUNGEON
	Add(MSG_EX_DVD, do_DVD, 0, false, false);
#endif //DYNAMIC_DUNGEON
#endif // DRATAN_CASTLE

#ifdef ALTERNATE_MERCHANT
	Add( MSG_EX_ALTERNATE_MERCHANT, do_AlternateMerchant, 0, false, false );
#endif // ALTERNATE_MERCHANT

#ifdef MONSTER_COMBO
	Add( MSG_EX_MONSTERCOMBO, do_MonsterCombo, 0, false, false );
#endif // MONSTER_COMBO

#ifdef CLIENT_RESTART
	Add( MSG_EX_RESTART, do_ClientRestart, 0 , false, false );
#endif //CLIENT_RESTART
	
#ifdef CHAOSBALL
	Add( MSG_EX_CHAOSBALL, do_ChaosBall, 0, false, false);
#endif
	
#ifdef IRIS_POINT
	Add( MSG_EX_UPDATE_PLAYTIME, do_UpdatePlayTime, 0, false, false );
#endif //IRIS_POINT
	
#ifdef ATTACK_PET
	Add( MSG_EX_ATTACK_PET, do_AttackPet, 0, false, false );
#endif //ATTACK_PET

#ifdef EXTREME_CUBE
	Add( MSG_EX_EXTREME_CUBE, do_ExExtremeCube, 0, false, false ); 
#endif // EXTREME_CUBE

//#ifdef INIT_SSKILL
	Add( MSG_EX_INIT_SSKILL, do_ExInitSSkill, 0, false, false );
//#endif // INIT_SSKILL

#ifdef DISCONNECT_HACK_CHARACTER
	// 펄스 관련
	Add(MSG_EX_DISCONNECT_HACK_CHARACTER, do_Disconnect_Hack_Character, 0, true, true);
#endif

#ifdef EX_GO_ZONE
	Add(MSG_EX_GO_ZONE, do_ExGoZone, 0, false, false);
#endif // EX_GO_ZONE

	Sort();
}

bool CCmdList::Add(int cmdType, LCCMDPROC cmdFunc, int minlevel, bool bCanDoWhileWarp, bool bCanDoWhileDisable)
{
	if (m_cnt >= CMD_NUM)
		return false;

	m_cmd[m_cnt] = new CCmd;
	m_cmd[m_cnt]->m_cmdType = cmdType;
	m_cmd[m_cnt]->m_cmdFunc = cmdFunc;
	m_cmd[m_cnt]->m_minLevel = minlevel;
	m_cmd[m_cnt]->m_bCanDoWhileWarp = bCanDoWhileWarp;
	m_cmd[m_cnt]->m_bCanDoWhileDisable = bCanDoWhileDisable;

	m_cnt++;
	return true;
}

void CCmdList::Run(int cmd_num, CPC* ch, CNetMsg& msg)
{
	// 이동 장치 사용중에 불가능한지 검사
	if (!m_cmd[cmd_num]->m_bCanDoWhileWarp && ch->IsSetPlayerState(PLAYER_STATE_WARP))
		return ;

	if( ch->IsSetPlayerState(PLAYER_STATE_CASHITEM) )
		return;

	if (!m_cmd[cmd_num]->m_bCanDoWhileDisable && ch->IsDisable())
		return ;

	((*m_cmd[cmd_num]->m_cmdFunc) (ch, msg));
}

CCmd::CCmd()
{
}

CCmd::~CCmd()
{

}

CCmdList::~CCmdList()	
{
	for (int i = 0; i < m_cnt; i++)
	{
		delete m_cmd[i];
		m_cmd[i] = NULL;
	}
}

int CmdComp( const void* arg1, const void* arg2 )
{
	return (*(CCmd**)arg1)->m_cmdType - (*(CCmd**)arg2)->m_cmdType;
}

void CCmdList::Sort()
{
	if (m_cnt == 0)
		return ;

	qsort((void* ) m_cmd, (size_t) m_cnt, sizeof(CCmd* ), CmdComp);
}

int CCmdList::Find(CPC* ch, int cmdType)
{
	if (m_cnt == 0)
		return -1;

	int min, mid, max, test;

	min = 0;
	max = m_cnt;
	while (true)
	{
		mid = (min + max) / 2;
		test = m_cmd[mid]->m_cmdType - cmdType;
		if (test == 0)
		{
			if (ch->m_admin >= m_cmd[mid]->m_minLevel)
				return mid;
			return -1;
		}
		if ((mid - min) <= 0) break;

		if (test > 0)
			max = mid;
		else 
			min = mid;
	}
	return -1;
}
