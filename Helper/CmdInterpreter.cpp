#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"
#include "CmdMsg.h"
#include "DBCmd.h"
#include "doFunc.h"

void ServerSrandom(unsigned long initial_seed);

void CServer::CommandInterpreter(CDescriptor* d, CNetMsg& msg)
{
	switch (msg.m_mtype)
	{
	case MSG_HELPER_REQ:
		do_Request(msg, d);
		break;

	case MSG_HELPER_REP:
		do_Reply(msg, d);
		break;

	case MSG_HELPER_COMMAND:
		do_Command(msg, d);
		break;

	default:
		{
			unsigned char subtype = 0;
			msg.MoveFirst();
			msg >> subtype;
			
			GAMELOG << init("WARNING", d->m_host)
					<< "INVALID COMMAND" << delim
					<< msg.m_mtype << delim << msg.m_size << delim
					<< (int)subtype << end;
		}
	
		break;
	}
}
