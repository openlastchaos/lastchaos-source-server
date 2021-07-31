#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "DBCmd.h"

#ifdef MONSTER_COMBO
void do_MonsterCombo(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype;
	msg >> subtype;

	switch(subtype)
	{
	case MSG_EX_MONSTERCOMBO_EDIT_COMPLETE:					// 편집완료버튼 눌렀을 경우
		do_MonsterComboEditComplete(ch, msg);
		break;
	case MSG_EX_MONSTERCOMBO_EDIT_CANCEL:					// 편집취소버튼을 눌렀을 경우
		do_MonsterComboEditCancel(ch);
		break;
	case MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REQ:				// 콤보 미션케이스 리스트 요청한 경우(파티용)
		do_MonsterComboContextReq( ch );
		break;
	case MSG_EX_MONSTERCOMBO_GOTO_COMBO:					// 편집완료 후 대기방으로 감
		do_MonsterComboGotoCombo(ch, msg);
		break;
	case MSG_EX_MONSTERCOMBO_GOTO_COMBO_CONFIRM:			// 확인메시지 클릭했을 때 
		do_MonsterComboConfirm(ch, msg);
		break;
	case MSG_EX_MONSTERCOMBO_RECALL_TO_COMBO:				// npc를 이용해서 파티리콜 클릭했을 때
		do_MonsterComboRecallToCombo( ch );
		break;
	case MSG_EX_MONSTERCOMBO_GIVEUP:						// 몬스터 콤보 포기
		do_MonsterComboGiveUp(ch, msg);
		break;
	case MSG_EX_MONSTERCOMBO_GOTO_STAGE:					// 대기방에서 다음 스테이지로 감 미션 시작
		do_MonsterComboGotoStage( ch );
		break;
	case MSG_EX_MONSTERCOMBO_GOTO_WAITROOM:					// 스테이지에서 대기방으로 감
		do_MonsterComboGotoWaitRoom( ch );
		break;
	default:
		return ;
	}
}

void do_MonsterComboEditComplete(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int i;
	int index;
	int count;
	msg >> count;
	
	// 파티일 때 파장만 할 수 있음
	if(ch->IsParty())
	{
		if(ch->m_index != ch->m_party->GetBossIndex())
		{
			ch->m_listSaveComboData.RemoveAll();
			return ;
		}
	}

	// 편집개수 체크
	if( count < 1 || count > 20 || (count % 5 != 0 ))
		return ;
	
	// 기존에 있는 것 지움
	ch->m_listSaveComboData.RemoveAll();
	
	MONSTER_COMBO_DATA data;
	const CMissionCaseProto* proto = NULL;
	for(i = 0; i < count; ++i)
	{
		msg >> index;
		proto = gserver.m_missionCaseList.FindProto(index);
		if(!proto)
		{
			// 세팅 실패
			ch->m_listSaveComboData.RemoveAll();
			MonsterComboErrorMsg(rmsg, MSG_EX_MONSTERCOMBO_ERROR_EDIT_COMPLETE_FAIL);
			SEND_Q(rmsg, ch->m_desc);

			// 실패로그
			GAMELOG << init("COMBO EDIT FAILED", ch)
				<< "Party Index: ";
			
			if(ch->IsParty()) 
				GAMELOG << ch->m_party->GetBossIndex();
			else 
				GAMELOG << -1;
			
			GAMELOG << " CASE NUM: "
					<< index
					<< end;

			return;
		}

		data.nStep	= i;
		data.nIndex = index;

		ch->m_listSaveComboData.AddToTail(data);
	}

	// 세팅 완료했음을 알린다.
	MonsterComboErrorMsg(rmsg, MSG_EX_MONSTERCOMBO_ERROR_EDIT_COMPLETE_OK);
	SEND_Q(rmsg, ch->m_desc);

	// 성공로그
	GAMELOG << init("COMBO EDIT OK", ch)
		<< "Party Index: ";
	
	if(ch->IsParty()) 
		GAMELOG << ch->m_party->GetBossIndex();
	else 
		GAMELOG << -1;
	
	GAMELOG << " MISSION COUNT: "
		<< ch->m_listSaveComboData.GetCount();
	
	GAMELOG << " MISSION: ";

	void* pos;
	void* posNext = ch->m_listSaveComboData.GetHead();
	while((pos = posNext))
	{
		posNext = ch->m_listSaveComboData.GetNext(pos);

		data = (MONSTER_COMBO_DATA)(ch->m_listSaveComboData.GetData(pos));

		GAMELOG << data.nIndex << " ";
	}
	GAMELOG << end;
}

void do_MonsterComboEditCancel(CPC* ch)
{
	if(ch->m_pZone->IsComboZone()) 
		return ;

	ch->m_listSaveComboData.RemoveAll();

	GAMELOG << init("COMBO EDIT CANCEL", ch)
		<< "Party Index: "
		<< (ch->IsParty() ? ch->m_party->GetBossIndex() : -1)
		<< end;
}

void do_MonsterComboContextReq(CPC* ch)
{
	
	CNetMsg rmsg;

	if(ch->IsParty())				// 파티인 경우
	{
		int bossindex, count;
		bossindex = ch->m_party->GetBossIndex();
		CPC* pc = gserver.m_playerList.Find(bossindex);
		if(!pc)
			return ;

		count =  pc->m_listSaveComboData.GetCount();
		if(count > 0)			// 편집 중임
		{
			MonsterComboMsg(rmsg, MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REP);
			rmsg << count;

			MONSTER_COMBO_DATA data;
			void *pos;
			void *nextpos = pc->m_listSaveComboData.GetHead();
			int i = 0;
			while((pos = nextpos))
			{
				nextpos = pc->m_listSaveComboData.GetNext(pos);
				data = pc->m_listSaveComboData.GetData(pos);
				rmsg << data.nIndex;
				++i;
			}

			if(i != count)
				return;

			SEND_Q(rmsg, ch->m_desc);
		}
		else					// 콤보존에 있음
		{
			// 방을 검색함.
			int i;
			i = gserver.m_comboZone->FindComboArea(ch->m_party->m_comboAreaIndex);
			if(i == -1)
				return ;
			CArea* area;
			CMonsterCombo* combo;
			area = gserver.m_comboZone->m_area + i;
			combo = area->m_monsterCombo;
			
			MonsterComboMsg(rmsg, MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REP);
			rmsg << combo->m_nTotalMission;

			const MISSION* mission;
			for(i = 0; i < combo->m_nTotalMission; ++i)
			{
				mission = combo->FindMission(i + 1);
				if(!mission)
					return ;
				
				rmsg << mission->proto->m_nIndex;
			}

			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else			// 개인인 경우
	{
		int count;
		count = ch->m_listSaveComboData.GetCount();
		if(count > 0)
		{
			MonsterComboMsg(rmsg, MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REP);
			rmsg << count;

			int i = 0;
			MONSTER_COMBO_DATA data;
			void *pos;
			void *nextpos = ch->m_listSaveComboData.GetHead();
			while((pos = nextpos))
			{
				nextpos = ch->m_listSaveComboData.GetNext(pos);
				data = ch->m_listSaveComboData.GetData(pos);
				rmsg << data.nIndex;
				i++;
			}

			if( i != count)
				return ;

			SEND_Q(rmsg, ch->m_desc);
		}
		else if(ch->m_pZone->IsComboZone())
		{
			CMonsterCombo* combo;
			combo = ch->m_pArea->m_monsterCombo;

			MonsterComboMsg(rmsg, MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REP);
			rmsg << combo->m_nTotalMission;

			const MISSION* mission;
			int i;
			for(i = 0; i < combo->m_nTotalMission; ++i)
			{
				mission = combo->FindMission(i + 1);
				if(!mission)
					return ;
				
				rmsg << mission->proto->m_nIndex;
			}

			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

void do_MonsterComboGotoCombo(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	int zone, extra, npcidx;
	
	msg >> zone
		>> extra
		>> npcidx;
	
	if(ch->IsParty())
	{
		// 파장인지 확인
		if(ch->m_party->GetBossIndex() != ch->m_index)
			return ;
	}
	

	int count;
	count = ch->m_listSaveComboData.GetCount();

	// 편집 완료를 눌렀는지 확인
	if(count < 1)
		return ;

	// 나스 검사
	void* pos;
	void* nextpos = ch->m_listSaveComboData.GetHead();
	int totalnas = 0;
	const CMissionCaseProto* proto;
	MONSTER_COMBO_DATA data;
	while((pos = nextpos))
	{
		nextpos = ch->m_listSaveComboData.GetNext(pos);
		data = ch->m_listSaveComboData.GetData(pos);
		proto = gserver.m_missionCaseList.FindProto(data.nIndex);
		if(!proto)
			return ;

		totalnas += proto->m_nNas;
	}

	if( ch->m_moneyItem)
	{
		if( ch->m_moneyItem->Count() < totalnas )
		{
			// 현재 가지고 있는 나스 부족 에러메세지 전송
			MonsterComboErrorMsg(rmsg, MSG_EX_MONSTERCOMBO_ERROR_MONEY);
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
	}
	else
	{
		MonsterComboErrorMsg(rmsg, MSG_EX_MONSTERCOMBO_ERROR_MONEY);
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	rmsg.Init(MSG_GO_ZONE);
	rmsg << zone
		 << extra
		 << npcidx;
	
	do_GoZone(ch, rmsg);

	if(ch->m_pZone->IsComboZone())
		ch->AddMoney( -totalnas );
}

void do_MonsterComboConfirm(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	char yesno;
	int bossindex;
	int nas;
	
	msg >> yesno
		>> bossindex;
	
	if(yesno)
	{
		CParty* party = gserver.FindPartyByBossIndex(bossindex);
		if(!party)
			return ;
		
		// 멤버확인
		int ret;
		ret = party->FindMember(ch->m_index);
		if(ret == -1)
			return ;

		if(party->m_comboAreaIndex == -1)
			return ;
		
		if(ch->m_pZone->m_index == ZONE_COMBO_DUNGEON)
			return ;

		ret = gserver.m_comboZone->FindComboArea(party->m_comboAreaIndex);
		if(ret == -1)
			return ;
		

		CArea* area = gserver.m_comboZone->m_area + ret;

		nas = area->m_monsterCombo->GetTotalNas();
		
		if( ch->m_moneyItem)
		{
			if( ch->m_moneyItem->Count() < nas )
			{
				// 현재 가지고 있는 나스 부족 에러메세지 전송
				MonsterComboErrorMsg(rmsg, MSG_EX_MONSTERCOMBO_ERROR_MONEY);
				SEND_Q( rmsg, ch->m_desc );
				return;
			}
		}
		else
		{
			MonsterComboErrorMsg(rmsg, MSG_EX_MONSTERCOMBO_ERROR_MONEY);
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
		
		int extra = 0;
		GoZone(ch, gserver.m_comboZone->m_index,
			gserver.m_comboZone->m_zonePos[extra][0],														// ylayer
			GetRandom(gserver.m_comboZone->m_zonePos[extra][1], gserver.m_comboZone->m_zonePos[extra][3]) / 2.0f,		// x
			GetRandom(gserver.m_comboZone->m_zonePos[extra][2], gserver.m_comboZone->m_zonePos[extra][4]) / 2.0f);		// z
		
		if(ch->m_pZone->IsComboZone())
			ch->AddMoney( -nas );
		
		// 로그
		GAMELOG << init("RECALL TO COMBO AREA", ch)
			<< "AREA INDEX: " << area->m_index
			<< " COMBO INDEX: " << area->m_monsterCombo->m_nIndex
			<< " NAS: " << nas
			<< end;
	}
}

void do_MonsterComboRecallToCombo(CPC* ch)
{
	CNetMsg rmsg;

	// 콤보던전인지
	if(ch->m_pZone->m_index != ZONE_COMBO_DUNGEON)
		return ;
	
	// 파티인지
	if(!ch->IsParty())
		return ;

	GAMELOG << init("MONSTER COMBO PARTY RECALL", ch)
			<< end;
	
	if(IS_RUNNING_HELPER)
	{
		HelperRecallToWaitRoomPrompt(rmsg, ch->m_party->GetBossIndex());
		SEND_Q(rmsg, gserver.m_helper);
	}
}

void do_MonsterComboGiveUp(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	int zone, extra, npcidx;
	
	msg >> zone
		>> extra
		>> npcidx;
	
	rmsg.Init(MSG_GO_ZONE);
	rmsg << zone
		 << extra
		 << npcidx;

	do_GoZone(ch, rmsg);
	
	if(!ch->m_pZone->IsComboZone())
	{
		GAMELOG << init("GET OUT FROM COMBO", ch)
			<< end;
	}
}

void do_MonsterComboGotoStage(CPC* ch)
{
	CNetMsg rmsg;

	if(!ch->m_pZone->IsComboZone())
		return ;
	
	if(!ch->m_pArea->m_bEnable || ch->m_pArea->m_monsterCombo->m_nIndex == -1)
	{
		GAMELOG << init("CLOSE COMBO AREA")
			<< "BY Next Stage Error"
			<< "AREA INDEX: " 
			<< ch->m_pArea->m_index 
			<< " PARTY INDEX: " 
			<< ch->m_pArea->m_monsterCombo->m_nIndex
			<< end;
		
		ch->m_pArea->CloseComboZone();
		return ;
	}
	
	CMonsterCombo* combo;
	combo = ch->m_pArea->m_monsterCombo;
	if(combo->m_bIsBattle)
	{
		// 스테이지 완료안됐음
		MonsterComboErrorMsg(rmsg, MSG_EX_MONSTERCOMBO_ERROR_NOT_COMPLETE_STAGE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("MONSTER COMBO START", ch)
			<< end;
	
	if(!combo->StartStage())
	{
		GAMELOG << init("FAIL START STAGE")
			<< "BY StartStage() Error"
			<< "AREA INDEX: " 
			<< combo->m_area->m_index 
			<< " COMBO INDEX: " 
			<< combo->m_area->m_monsterCombo->m_nIndex
			<< end;

		return ;
	}
}

void do_MonsterComboGotoWaitRoom( CPC* ch )
{
	if(!ch->m_pZone->IsComboZone())
		return ;
	
	if(!ch->m_pArea->m_bEnable || ch->m_pArea->m_monsterCombo->m_nIndex == -1)
	{
		GAMELOG << init("CLOSE COMBO AREA")
			<< "BY GOTO WAITROON Error"
			<< "AREA INDEX: " 
			<< ch->m_pArea->m_index 
			<< " COMBO INDEX: " 
			<< ch->m_pArea->m_monsterCombo->m_nIndex
			<< end;
		
		ch->m_pArea->CloseComboZone();
		return ;
	}
	
	float x, z;
	char ylayer;
	int extra = 0;   // 대기방
	
	x = GetRandom(gserver.m_comboZone->m_zonePos[extra][1], gserver.m_comboZone->m_zonePos[extra][3]) / 2.0f;
	z = GetRandom(gserver.m_comboZone->m_zonePos[extra][2], gserver.m_comboZone->m_zonePos[extra][4]) / 2.0f;
	ylayer = gserver.m_comboZone->m_zonePos[extra][0];
	GET_R(ch) = 0.0f;
	GoTo(ch, ylayer, x, z, ch->m_pArea->GetHeight(ylayer, x, z), GET_R(ch));
}


#endif // MONSTER_COMBO


