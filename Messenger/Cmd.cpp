#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"

CCmd::CCmd()
{
}

CCmd::~CCmd()
{

}

CCmdList::CCmdList()
{
	m_cnt = 0;

	Add(MSG_MSGR_REQ, do_Request, 0);
	Add(MSG_MSGR_REP, do_Reply, 0);
	
	Sort();
}

CCmdList::~CCmdList()	
{
	for (int i = 0; i < m_cnt; i++)
	{
		delete m_cmd[i];
	}
}

bool CCmdList::Add(int cmdType, void cmdFunc(CNetMsg&, CDescriptor*), int minlevel)
{
	if (m_cnt >= CMD_NUM)
		return FALSE;

	m_cmd[m_cnt] = new CCmd;
	m_cmd[m_cnt]->m_cmdType = cmdType;
	m_cmd[m_cnt]->m_cmdFunc = cmdFunc;
	m_cmd[m_cnt]->m_minLevel = minlevel;

	m_cnt++;
	return TRUE;
}

int CmdComp( const void* arg1, const void* arg2 )
{
	return (*(CCmd**)arg1)->m_cmdType - (*(CCmd**)arg2)->m_cmdType;
}

void CCmdList::Sort()
{
	qsort((void* ) m_cmd, (size_t) m_cnt, sizeof(CCmd* ), CmdComp);
}

int CCmdList::Find(int cmdType)
{
	int min, mid, max, test;

	min = 0;
	max = m_cnt;
	while (true) {
		mid = (min + max) / 2;
		test = m_cmd[mid]->m_cmdType - cmdType;
		if (test == 0)
			return mid;

		if ((mid - min) <= 0) break;

		if (test > 0)
			max = mid;
		else 
			min = mid;
	}
	return -1;
}

void CCmdList::Run(int cmd_num, CNetMsg& msg, CDescriptor* dest)
{
	((*m_cmd[cmd_num]->m_cmdFunc) (msg, dest));
}
