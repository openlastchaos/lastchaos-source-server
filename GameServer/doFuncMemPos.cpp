#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"
#include "DratanCastle.h"

/////////////////
// 장소 기억 관련

void do_MemPos(CPC* ch, CNetMsg& msg)
{
#ifdef DRATAN_CASTLE
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle != NULL)
	{
		pCastle->CheckRespond(ch);
	}
#endif // DRATAN_CASTLE

	if (DEAD(ch))
		return ;

	msg.MoveFirst();

	unsigned char subtype;
	msg >> subtype;

	switch (subtype)
	{
	case MSG_MEMPOS_WRITE:
		{
			unsigned char slot;
			CLCString comment(150);

			msg >> slot
				>> comment;

			// 아이템 검사
			int r, c;
			if (!ch->m_invenNormal.FindItem(&r, &c, 47, 0, 0))
				return ;

			// 기억 불가능 존이면 오류
			if (!ch->m_pZone || !ch->m_pZone->m_bCanMemPos)
			{
				CNetMsg rmsg;
				SysMsg(rmsg, MSG_SYS_MEMPOS_CANT_WRITE);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// BLOCK에는 불가
			if (ch->m_pArea->GetAttr(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)) == MAPATT_BLOCK)
			{
				CNetMsg rmsg;
				SysMsg(rmsg, MSG_SYS_MEMPOS_CANT_WRITE);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 잘못된 문자 찾기
			if (strinc(g_buf2, "'") || strinc(g_buf2, "%"))
			{
				CNetMsg rmsg;
				SysMsg(rmsg, MSG_SYS_INVALID_CHAR);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			if ( ch->m_memposTime < gserver.m_gameTime )
			{
				if( slot >= MAX_MEMPOS_NORMAL )
					return;
			}
			else
			{
				if( slot >= MAX_MEMPOS )
					return;
			}

			if (ch->m_mempos.Write(slot, ch->m_pZone->m_index, GET_X(ch), GET_Z(ch), GET_YLAYER(ch), comment))
			{
				CNetMsg rmsg;
				MemPosWriteMsg(rmsg, ch, slot);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;

	case MSG_MEMPOS_MOVE:
		{
			char slot;
			msg >> slot;

			int canWarp = ch->CanWarp();
			if (canWarp != 0)
			{
				CNetMsg rmsg;
				SysMsg(rmsg, (MSG_SYS_TYPE)canWarp);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 슬롯 검사
			if (slot < 0)
				return ;

			if ( ch->m_memposTime < gserver.m_gameTime )
			{
				if( slot >= MAX_MEMPOS_NORMAL )
					return;
			}
			else
			{
				if( slot >= MAX_MEMPOS )
					return;
			}

			if (ch->m_mempos.m_data[(int)slot] == NULL)
				return ;

			// 존 검사 : 같은 존에서만
			if (!ch->m_pZone || ch->m_pZone->m_index != ch->m_mempos.m_data[(int)slot]->m_zone)
			{
				CNetMsg rmsg;
				SysMsg(rmsg, MSG_SYS_MEMPOS_OTHERZONE);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// BLOCK에는 불가
			if (ch->m_pArea->GetAttr(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)) == MAPATT_BLOCK)
				return ;

#ifdef ENABLE_WAR_CASTLE
			CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
			if (castle)
			{
				// 공성중이거나 시작 1분 전에는 공성지역으로 이동 불가
				if (castle->GetState() != WCSF_NORMAL || castle->IsWarCastleReady())
				{
					if (ch->m_pArea->GetAttr(ch->m_mempos.m_data[(int)slot]->m_ylayer, ch->m_mempos.m_data[(int)slot]->m_x, ch->m_mempos.m_data[(int)slot]->m_z ) == MAPATT_WARZONE 
						|| ch->m_pZone->IsWarZone(ch->m_mempos.m_data[(int)slot]->m_x, ch->m_mempos.m_data[(int)slot]->m_z) == true)
					{						
							CNetMsg rmsg;
							SysMsg(rmsg, MSG_SYS_MEMPOS_CASTLE);
							SEND_Q(rmsg, ch->m_desc);
							return ;
					}

					//공성시간에 매모리 스클롤 사용로그
					GAMELOG << init(" MEMORY SCROLL IN WARTIME " , ch )
						<< "CUR : " << ch->m_pos.m_yLayer << "," << ch->m_pos.m_x << "," << ch->m_pos.m_z << delim
						<< "MOV : " << ch->m_mempos.m_data[(int)slot]->m_ylayer << "," << ch->m_mempos.m_data[(int)slot]->m_x << ","
						<< ch->m_mempos.m_data[(int)slot]->m_z << end;

				}
			}
#endif

			// 아이템 검사
			int r, c;
			if (!ch->m_invenNormal.FindItem(&r, &c, 47, 0, 0))
				return ;

			// 카운트 줄이기
			CItem* item = ch->m_invenNormal.GetItem(r, c);
			CNetMsg rmsg;

			ItemUseMsg(rmsg, ch, item->tab(), item->row(), item->col(), item->m_index, 0);
			SEND_Q(rmsg, ch->m_desc);

			DecreaseFromInventory(ch, item, 1);
			if (item->Count() == 0)
			{
				ItemDeleteMsg(rmsg, item);
				SEND_Q(rmsg, ch->m_desc);
				RemoveFromInventory(ch, item, true, true);
			}
			else
			{
				ItemUpdateMsg(rmsg, item, -1);
				SEND_Q(rmsg, ch->m_desc);
			}

			// 10초후 이동하게 하기
			ch->m_reqWarpType = IONCE_WARP_MEMPOS;
			ch->m_reqWarpData = slot;
			ch->m_reqWarpTime = PULSE_WARPDELAY;
			ch->SetPlayerState(PLAYER_STATE_WARP);

			// 사용했음을 알리기
			WarpStartMsg(rmsg, ch);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}
		break;
	}
}

#ifdef PRIMIUM_MEMORYBOOK
void do_MemPosPlus(CPC* ch, CNetMsg& msg)
{
	if (DEAD(ch))
		return ;

	unsigned char subtype;
	msg >> subtype;

	// 아이템 확인
	int row = 0, col = 0;
	if (ch->m_invenNormal.FindItem(&row, &col, PRIMIUM_MEMORYBOOK, -1, -1) == false)
	{
		return;
	}

	// 아이템 가져 오기
	CItem * pItem = ch->m_invenNormal.GetItem(row, col);
	if (!pItem)
	{
		return;
	}
	
	CNetMsg rmsg;
	switch (subtype)
	{	
	case MSG_EX_MEMPOSPLUS_WRITE:
		{
			unsigned char slot;
			CLCString comment(150);

			msg >> slot
				>> comment;

			// 기억 불가능 존이면 오류
			if (!ch->m_pZone || !ch->m_pZone->m_bCanMemPos)	
			{				
				SysMsg(rmsg, MSG_SYS_MEMPOS_CANT_WRITE);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// BLOCK에는 불가
			if (ch->m_pArea->GetAttr(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)) == MAPATT_BLOCK)
			{
				SysMsg(rmsg, MSG_SYS_MEMPOS_CANT_WRITE);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 잘못된 문자 찾기
			if (strinc(g_buf2, "'") || strinc(g_buf2, "%"))
			{
				SysMsg(rmsg, MSG_SYS_INVALID_CHAR);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			if( slot >= MAX_MEMPOS_PRIMIUM )
				return;
			
			if (ch->m_memposplus.Write(slot, ch->m_pZone->m_index, GET_X(ch), GET_Z(ch), GET_YLAYER(ch), comment))
			{
				MemPosPlusWriteMsg(rmsg, ch, slot);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;

	case MSG_EX_MEMPOSPLUS_MOVE:
		{
			char slot;
			msg >> slot;

			int canWarp = ch->CanWarp();
			if (canWarp != 0)
			{
				SysMsg(rmsg, (MSG_SYS_TYPE)canWarp);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			
			// 슬롯 검사
			if (slot < 0)
				return ;

			if( slot >= MAX_MEMPOS_PRIMIUM )
				return;

			if (ch->m_memposplus.m_data[(int)slot] == NULL)
				return ;

			// BLOCK에는 불가
			if (ch->m_pArea->GetAttr(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)) == MAPATT_BLOCK)
				return ;

#ifdef ENABLE_WAR_CASTLE
			// 공성중에 공성 지역(MAPATT_WARZONE) 으로 이동 불가
			CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
			if (castle)
			{
				if (castle->GetState() != WCSF_NORMAL || castle->IsWarCastleReady())
				{
					if (ch->m_pArea->GetAttr(ch->m_memposplus.m_data[slot]->m_ylayer, ch->m_memposplus.m_data[slot]->m_x, ch->m_memposplus.m_data[slot]->m_z) == MAPATT_WARZONE
						|| ch->m_pZone->IsWarZone(ch->m_mempos.m_data[(int)slot]->m_x, ch->m_mempos.m_data[(int)slot]->m_z) == true )
					{
						SysMsg(rmsg, MSG_SYS_MEMPOS_CASTLE);
						SEND_Q(rmsg, ch->m_desc);
						return ;
					}

					//공성시간에 매모리 스클롤 사용로그
					GAMELOG << init(" PRIMIUM_MEMORYBOOK IN WARTIME " , ch )
						<< "CUR : " << ch->m_pos.m_yLayer << "," << ch->m_pos.m_x << "," << ch->m_pos.m_z << delim
						<< "MOV : " << ch->m_mempos.m_data[(int)slot]->m_ylayer << "," << ch->m_mempos.m_data[(int)slot]->m_x << ","
						<< ch->m_mempos.m_data[(int)slot]->m_z << end;
				}
			}
#endif

			CNetMsg rmsg;

			// 10초후 이동하게 하기
			ch->m_reqWarpType = IONCE_WARP_MEMPOSPLUS;
			ch->m_reqWarpData = slot;
			ch->m_reqWarpTime = PULSE_WARPDELAY;
			ch->SetPlayerState(PLAYER_STATE_WARP);

			// 사용했음을 알리기
			WarpStartMsg(rmsg, ch);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}
		break;
	}
}
#endif	// PRIMIUM_MEMORYBOOK