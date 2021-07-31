#define __DO_FUNC_RESERVED_GM_COMMAND_CPP__

#include "stdhdrs.h"

#include "GMCmdList.h"
#include "Character.h"
#include "Server.h"
#include "doFunc.h"
#include "../ShareLib/packetType/ptype_reserved_gm_command.h"
#include "DescManager.h"
#include "CmdMsg.h"

void do_ReservedGMCommand(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::reservedGMCommandList* packet = reinterpret_cast<RequestClient::reservedGMCommandList*>(msg->m_buf);
	packet->m_Index = ch->m_desc->m_index;

	SEND_Q(msg, gserver->m_subHelper);
}

//////////////////////////////////////////////////////////////////////////
void ProcSubHelperReservedGMCommand(CNetMsg::SP& msg)
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);

	switch (pBase->subType)
	{
	case MSG_SUB_RESERVED_GM_COMMAND_LIST:
		{
			ResponseClient::reservedGMCommandList* packet = reinterpret_cast<ResponseClient::reservedGMCommandList*>(msg->m_buf);
			CDescriptor* desc = DescManager::instance()->getDescByUserIndex(packet->m_Index);
			if (desc == NULL)
				return;

			SEND_Q(msg, desc);
		}
		break;

	case MSG_SUB_ACTION_RESERVED_GM_COMMAND:
		{
			ResposeGameServer::addReservedGMCommand* packet = reinterpret_cast<ResposeGameServer::addReservedGMCommand*>(msg->m_buf);
			// 기존의 GM명령어를 실행하기 위해 가상의 캐릭터를 생성
			CPC* dummyChar = new CPC;
			dummyChar->m_desc = NULL;
			dummyChar->m_admin = 10;
			gGMCmdList->run(dummyChar, packet->command);
			delete dummyChar;
		}
		break;

	case MSG_SUB_LIST_RESERVED_GM_COMMAND_BY_GMCOMMAND:
		{
			ResponseClient::reservedGMCommandListByGm* packet = reinterpret_cast<ResponseClient::reservedGMCommandListByGm*>(msg->m_buf);
			CPC* pc = PCManager::instance()->getPlayerByCharIndex(packet->char_index);

			if(pc == NULL)
				return;

			if(packet->count == 0)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", "List is empty.");
				SEND_Q(rmsg, pc->m_desc);
				return;
			}

			//리스트 출력
			time_t unix_time;
			struct tm* time_;
			std::string time_str;

			for( int i = 0; i < packet->count; ++i )
			{
				unix_time = packet->data[i].startTime;
				time_ = localtime(&unix_time);

				time_str = boost::str(boost::format("%d-%d-%d %d:%d") %(time_->tm_year + 1900) %(time_->tm_mon + 1) %(time_->tm_mday) %time_->tm_hour %time_->tm_min );

				std::string str = boost::str(boost::format("Index : %d, Date : %s, SubNo : %d Command : %s") % packet->data[i].index % time_str.c_str() %packet->data[i].subno % packet->data[i].command);
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str.c_str());
				SEND_Q(rmsg, pc->m_desc);
			}
		}
		break;

	case MSG_SUB_ACTION_RESERVED_GM_COMMAND_BY_GMCOMMAND:
		{
			ResposeGameServer::addReservedGMCommandByGm* packet = reinterpret_cast<ResposeGameServer::addReservedGMCommandByGm*>(msg->m_buf);

			if(packet->subno != 0 &&
				packet->subno != gserver->m_subno)
				break;

			LOG_INFO("RESERVE GM COMMAND RUN. Command : %s", packet->command);

			CPC* dummyChar = new CPC;
			dummyChar->m_desc = NULL;
			dummyChar->m_admin = 10;
			gGMCmdList->run(dummyChar, packet->command);
			delete dummyChar;
		}
		break;

	default:
		LOG_ERROR("reserved gm command : invalid subtype[%d]", pBase->subType);
		break;
	}
}