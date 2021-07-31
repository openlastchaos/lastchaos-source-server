#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"

////////////////////////////
// 싱글던전 NPC Regen 요청
void do_NPCRegen(CPC* ch, CNetMsg& msg)
{
	int index, dbIndex, yLayer;
	int	entityIndex;	// 클라이언트 전용 인덱스
	float x, z, h, r;

	msg.MoveFirst();

	msg >> index
		>> dbIndex
		>> x >> z >> h >> r >> yLayer
		>> entityIndex;

	// npc regen 요청이 아니면
	if (index != -1)
		return;

	// 싱글던전이 아니면
	if (!ch->m_pZone->IsPersonalDungeon())
		return;

	// 리젠 테이블 정보 보다 클라이언트에서 보내는 Regen Msg가 많이 온다면
	// 싱글던전에서 스타트 존으로 튕기다
	bool gohome = false;

	if( !ch->m_pArea->m_npcRegenList.SetRegened(ch->m_pArea, dbIndex, ch->m_pZone->m_index, x, yLayer, z) )
		gohome = true;

	CNPC* npc;
	// npc 생성
	npc = gserver.m_npcProtoList.Create(dbIndex, NULL);

	if (!npc)	return;

	if( gohome)
	{
		// 가까운 마을로
		int nearZone;
		int nearZonePos;
		int i = gserver.FindNearestZone(ch->m_pZone->m_index, GET_X(ch), GET_Z(ch), &nearZone, &nearZonePos);
		if (i == -1)
			return ;
		
		CZone* pZone = gserver.m_zones + i;

		GAMELOG << init("PD_BUG_HACK_REGEN", ch) << "ZONE" << ch->m_pZone->m_index << delim << "NPC" << delim << dbIndex << end;
		
		GoZone(ch, nearZone,
			pZone->m_zonePos[nearZonePos][0],															// ylayer
			GetRandom(pZone->m_zonePos[nearZonePos][1], pZone->m_zonePos[nearZonePos][3]) / 2.0f,		// x
			GetRandom(pZone->m_zonePos[nearZonePos][2], pZone->m_zonePos[nearZonePos][4]) / 2.0f);		// z

		if(npc)
			delete npc;

		return;
	}
	// TODO : 추후 퍼스널 던전의 몹 난이도 조절시 리젠 정보를 얻어와 리젠 정보에 파라미터를 넣고 리젠시킨다

	npc->m_regenX = GET_X(npc) = x;
	npc->m_regenZ = GET_Z(npc) = z;
	npc->m_regenY = GET_YLAYER(npc) = yLayer;
	GET_R(npc) = r;

	// TODO : 높이맵에 따른 height 조정
	npc->m_pos.m_h = h;

	int cx, cz;
	ch->m_pArea->AddNPC(npc);
	ch->m_pArea->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
	ch->m_pArea->CharToCell(npc, GET_YLAYER(npc), cx, cz);

	CNetMsg npcRegenMsg;
	NPCRegenMsg(npcRegenMsg, npc, entityIndex);
	ch->m_pArea->SendToCell(npcRegenMsg, ch, true);
}
