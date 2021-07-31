#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"

#ifdef EXTREME_CUBE
void do_ExExtremeCube(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	unsigned char subtype;
	msg >> subtype;

	if(!gserver.m_extremeCube.IsExtremeCubeServer())
		return ;

	switch(subtype)
	{
	case MSG_EX_EXTREME_CUBE_PARTY:
		{
			int i;
			int cubenum;
			bool bIsAllStanding = true;
			bool bfirst;
			CParty* party = NULL;
			CNetMsg rmsg;

			if(gserver.m_extremeCube.IsGuildCubeTime())
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_NOT_PARTY_CUBE_TIME);
				SEND_Q(rmsg, ch->m_desc);
				GAMELOG << init("PARTYCUBE ERROR", ch)
						<< "GUILDCUBETIME" << end;
				return ;
			}

			if(!ch->IsParty())
				return ;

			party = ch->m_party;
			if(party->m_cubeUniqueIdx >=0)
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("PARTYCUBE ERROR", ch);
				if(!party)
				{
					GAMELOG << "NO PARTY"
							<< end;
				}
				else
				{
					GAMELOG << "PARTY CUBEIDX" << delim
							<< party->m_cubeUniqueIdx
							<< end;
				}
				return ;
			}

			int row, col;
			if(!ch->m_invenNormal.FindItem(&row, &col, 2986, 0, 0))
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_NOITEM);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("PARTYCUBE ERROR", ch)
						<< "NO ITEM" << end;
				return ;
			}

			cubenum = gserver.m_extremeCube.GetAvailableBattleCube(bfirst);
			if(cubenum < 0 || cubenum > MAX_CUBESPACE - 1)
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_OVERCOUNT);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("PARTYCUBE ERROR", ch)
						<< "NOT FOUND CUBEIDX" << delim << cubenum << end;
				return ;
			}

			CCubeSpace* space = gserver.m_extremeCube.GetExtremeCube(cubenum);
			if(!space || !space->IsBattleCubeSpace())
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_OVERCOUNT);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("PARTYCUBE ERROR", ch)
						<< "NOT FOUND CUBESPACE" << delim << cubenum << end;
				return ;
			}

			CBattleSpace* cubespace = (CBattleSpace*)space;

		//	CPartyCubeMemList* memlist = new CPartyCubeMemList(party, cubenum);
			CPartyCubeMemList* memlist = gserver.m_extremeCube.GetAvailablePartyCubeMemList();
			if(!memlist)
			{
				GAMELOG << init("CANNOT CREATE PARTY MEMLIST")
						<< end;
				return ;
			}
			memlist->Enable(party, cubenum);
			if(!cubespace->SetMemList(memlist))
			{
				if(memlist)
				{
					memlist->Disable();
					memlist = NULL;

					GAMELOG << init("PARTYCUBE ERROR", ch)
							<< "FAILED SET MEMLIST" << delim 
							<< end;
				}
				return ;
			}

			for(i = 0; i < MAX_PARTY_MEMBER; ++i)
			{
				const CPartyMember* member = party->GetMemberByListIndex(i);
				if(member && (member->GetMemberPCPtr()) )
				{
					CPC* pc = member->GetMemberPCPtr();

					if(memlist->JoinCube(pc, cubespace->m_extra))
					{
						bIsAllStanding = false;
					}
				}
			}

			if(bIsAllStanding)
			{
				cubespace->DelMemList(memlist);

				if(memlist)
				{
					//delete memlist;
					memlist->Disable();
					memlist = NULL;

					GAMELOG << init("PARTYCUBE ERROR", ch)
							<< "NOT ALLOW ALL" << end;
				}
				return ;
			}

			if(bfirst)
			{
				cubespace->RegenMob();
			}

			GAMELOG << init("PARTYCUBE START", ch)
					<< "UniqueIdx" << delim 
					<< memlist->GetUniqueIdx() << delim
					<< "AreaIdx" << delim
					<< cubespace->m_area->m_index
					<< end;
		}
		break;
	case MSG_EX_EXTREME_CUBE_PARTY_PERSON:
		{
			if(gserver.m_extremeCube.IsGuildCubeTime())
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_NOT_PARTY_CUBE_TIME);
				SEND_Q(rmsg, ch->m_desc);
				GAMELOG << init("PARTYCUBE ERROR PERSONAL", ch)
						<< "GUILDCUBETIME" << end;
				return ;
			}

			if(!ch->IsParty())
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("PARTYCUBE ERROR PERSONAL", ch)
						<< "NO PARTY"
						<< end;
				return ;
			}

	
			if(ch->m_party->m_cubeUniqueIdx == -1)
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("PARTYCUBE ERROR PERSONAL", ch)
						<< "PARTY CUBEIDX" << delim
						<< ch->m_party->m_cubeUniqueIdx
						<< end;
				return ;
			}

			CCubeMemList* memlist = gserver.m_extremeCube.FindMemList(ch->m_party->m_cubeUniqueIdx);
			if(!memlist)
			{
				GAMELOG << init("PARTYCUBE ERROR PERSONAL", ch)
						<< "NOT FOUND MEMLIST" << delim
						<< "PARTY CUBEIDX" << delim
						<< ch->m_party->m_cubeUniqueIdx
						<< end;
				return ;
			}

			int cubenum;
			if(!memlist->GetCubeNum(cubenum))
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("PARTYCUBE ERROR PERSONAL", ch)
						<< "NOT FOUND CUBEIDX" << delim
						<< "PARTY CUBEIDX" << delim
						<< cubenum
						<< end;
				return ;
			}


			CCubeSpace* space = gserver.m_extremeCube.GetExtremeCube(cubenum);
			if(!space || !space->IsBattleCubeSpace())
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_OVERCOUNT);
				SEND_Q(rmsg, ch->m_desc);

				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_OVERCOUNT);
				SEND_Q(rmsg, ch->m_desc);

				GAMELOG << init("PARTYCUBE ERROR", ch)
						<< "NOT FOUND CUBESPACE" << delim << cubenum << end;

				return ;
			}

			CBattleSpace* cubespace = (CBattleSpace*)space;

			if(!cubespace->IsAvailableJoinPersonal())
			{
				GAMELOG << init("CANT MOVE CUBE PERSONAL", ch)
						<< "CUBESTATE" << delim << (int)(cubespace->GetState())
						<< end;
				return ;
			}

			memlist->JoinCube(ch, cubespace->m_extra);

			GAMELOG << init("ENTER PARTYCUBE PERSONAL", ch)
					<< "UniqueIdx" << delim 
					<< memlist->GetUniqueIdx() << delim
					<< "AreaIdx" << delim
					<< cubespace->m_area->m_index
					<< end;
		}
		break;

	case MSG_EX_EXTREME_CUBE_GUILD:
		{
			int i;
			int cubenum;
			bool bIsAllStanding = false;
			bool bfirst;
			CNetMsg rmsg;
			
			if(!gserver.m_extremeCube.IsGuildCubeTime())
			{
				// 길드큐브타임
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_NOT_GUILD_CUBE_TIME);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if(!ch->m_guildInfo || !ch->m_guildInfo->guild())
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			int row, col;
			if(!ch->m_invenNormal.FindItem(&row, &col, 2987, 0, 0))
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if(ch->m_guildInfo->guild()->m_cubeUniqueIdx >= 0)
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			cubenum = gserver.m_extremeCube.GetAvailableBattleCube(bfirst);
			if(cubenum < 0 || cubenum > MAX_CUBESPACE - 1)
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_OVERCOUNT);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			CCubeSpace* space = gserver.m_extremeCube.GetExtremeCube(cubenum);
			if(!space || !space->IsBattleCubeSpace())
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_OVERCOUNT);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			CBattleSpace* cubespace = (CBattleSpace*)space;

	//		CGuildCubeMemList* memlist = new CGuildCubeMemList(ch->m_guildInfo->guild(), cubenum);
			CGuildCubeMemList* memlist = gserver.m_extremeCube.GetAvailableGuildCubeMemList();
			memlist->Enable(ch->m_guildInfo->guild(), cubenum);

			if(!cubespace->SetMemList(memlist))
			{
				if(memlist)
				{
				//	delete memlist;
					memlist->Disable();
					memlist = NULL;
				}
				return ;
			}

			for(i = 0; i < GUILD_MAX_MEMBER; ++i)
			{
				CGuildMember* member = ch->m_guildInfo->guild()->member(i);
				if(member && (member->GetPC()) )
				{
					CPC* pc = member->GetPC();

					if(memlist->JoinCube(pc, cubespace->m_extra))
					{
						bIsAllStanding = true;
					}
				}
			}

			if(!bIsAllStanding)
			{
				if(memlist)
				{
				//	delete memlist;
					memlist->Disable();
					memlist = NULL;
				}
				return ;
			}

			if(bfirst)
			{
				cubespace->RegenMob();
			}

			GAMELOG << init("Enter GuildExCube", ch)
					<< "UniqueIdx" << delim 
					<< memlist->GetUniqueIdx() << delim
					<< "AreaIdx" << delim
					<< cubespace->m_area->m_index
					<< end;
		}
		break;
	case MSG_EX_EXTREME_CUBE_GUILD_PERSON:
		{
			if(!gserver.m_extremeCube.IsGuildCubeTime())
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_NOT_GUILD_CUBE_TIME);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if(!ch->m_guildInfo || !ch->m_guildInfo->guild())
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if(ch->m_guildInfo->guild()->m_cubeUniqueIdx == -1)
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			CCubeMemList* memlist = gserver.m_extremeCube.FindMemList(ch->m_guildInfo->guild()->m_cubeUniqueIdx);
			if(!memlist)
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			int cubenum;
			if(!memlist->GetCubeNum(cubenum))
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_OVERCOUNT);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			CCubeSpace* space = gserver.m_extremeCube.GetExtremeCube(cubenum);
			if(!space || !space->IsBattleCubeSpace())
			{
				ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_OVERCOUNT);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			CBattleSpace* cubespace = (CBattleSpace*)space;

			if(!cubespace->IsAvailableJoinPersonal())
				return ;

			if(memlist->JoinCube(ch, cubespace->m_extra))
			{
				GAMELOG << init("Enter GuildExCube Personal", ch)
						<< "UniqueIdx" << delim 
						<< memlist->GetUniqueIdx() << delim
						<< "AreaIdx" << delim
						<< cubespace->m_area->m_index
						<< end;
			}
		}
		break;
	case MSG_EX_EXTREME_CUBE_STATE_REQ:
		{
			if (IS_RUNNING_HELPER)
			{
				CNetMsg rmsg;
				HelperCubeStateReqMsg(rmsg, ch->m_index);
				SEND_Q(rmsg, gserver.m_helper);
			}
		}
		break;
	case MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REQ:
		{
			if (IS_RUNNING_HELPER)
			{
				CNetMsg rmsg;
				HelperCubeStatePersonalReqMsg(rmsg, ch->m_index);
				SEND_Q(rmsg, gserver.m_helper);
			}
		}
		break;
	default:
		break;
	}
}

#endif // EXTREME_CUBE
