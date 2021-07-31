#include "TradeAgent.h"
#include "stdhdrs.h"
#include "Server.h"
#include "DBCmd.h"
#include "CmdMsg.h"
#include "Log.h"

#ifdef TRADE_AGENT

// CTRADEAGENT
bool CTRADEAGENT::SetItemReg(int item_serial,int sell_charindex)
{
	m_item_serial		= item_serial;
	m_sell_charindex	= sell_charindex;
	m_state				= MSG_TRADEAGENT_STATE_SELL_BEFORE;	
	
	time_t ct;
	struct tm *ti;

	ct = time(0);
	ti = localtime(&ct);

	m_regist_date.year	= ti->tm_year + 1900;
	m_regist_date.month	= ti->tm_mon + 1;
	m_regist_date.day	= ti->tm_mday;
	m_regist_date.hour	= ti->tm_hour;
	m_regist_date.min	= ti->tm_min;
	m_regist_date.sec	= ti->tm_sec;	

	return true;
}

//마감일을 계산 세팅
bool CTRADEAGENT::SetItemRegFinishDay(void)
{
	CLCString sql(1024);
	
	char RegDate[20] = "";

	sprintf(RegDate,"%4d-%d-%d %d:%d:%d" 
			    ,m_regist_date.year
				,m_regist_date.month
				,m_regist_date.day
				,m_regist_date.hour
				,m_regist_date.min
				,m_regist_date.sec);

	sql.Format("SELECT UNIX_TIMESTAMP( DATE_ADD( '%s', INTERVAL 5 DAY)) AS unix_regist_date",RegDate);	

	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );
	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		return false;
	}

	cmd.GetRec( "unix_regist_date"	 , m_finish_date );

	return true;
}

//DB INDEX 설정
bool CTRADEAGENT::SetDbIndex(int& nDbIndex)
{
	CLCString sql(1024);
	
	sql.Format( "SELECT a_index FROM t_tradeagent WHERE a_item_serial = %d AND a_sell_charindex = %d ORDER BY a_regist_date DESC",
		         m_item_serial,m_sell_charindex);

	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );
	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		return false;
	}

	cmd.GetRec( "a_index"	 , m_dbIndex );

	nDbIndex = m_dbIndex;

	return true;
}

//마감일이 지났는지 체크
bool CTRADEAGENT::IsPassFinishDay(void)
{
	time_t Now;

	//상태 체크
	if(m_state != MSG_TRADEAGENT_STATE_SELL_BEFORE)
		return false;

	Now = time(NULL);

	if(Now > m_finish_date)
	{
		return true;
	}

	return false;
}

//DB 상태 변경 및 정산 필요(반송) 알림
bool CTRADEAGENT::ChangeStateCalcNotify()
{
	CLCString sql(1024);

	// 조회 
	//-->
	sql.Format("SELECT * FROM t_tradeagent WHERE a_index = %d",m_dbIndex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		return false;
	}
	//<--

	int nQuantity = 0;
	CLCString stItemName(MAX_ITEM_NAME_LENGTH + 1);

	cmd.GetRec( "a_item_name"		, stItemName );
	cmd.GetRec( "a_quantity"		, nQuantity );

	//갱신 
	sql.Format("UPDATE t_tradeagent SET a_state = %d , a_process_date = now() WHERE a_index = %d "
				,MSG_TRADEAGENT_STATE_SELL_RETURNED, m_dbIndex);

	cmd.SetQuery(sql);
	if(cmd.Update())
	{
		m_state = MSG_TRADEAGENT_STATE_SELL_RETURNED;

		CNetMsg rmsg;
		
		//정산 필요(반송) 알림 메세지
		HelperTradeAgentReturnedMsg(rmsg, m_sell_charindex,stItemName,nQuantity);

		//게임서버로 전송 
		gserver.SendToAllGameServer(rmsg);
		
		return true;
	}else
	{
		// 실패하면 로그 찍기
		GAMELOG << init("Fail  ChangeStateCalcNotify")
				<< "charindex" << delim
				<< m_sell_charindex << delim
				<< "dbIndex" << delim
				<< m_dbIndex << delim
				<< "state" << delim
				<< MSG_TRADEAGENT_STATE_SELL_RETURNED << end;

	}
	//<--	

	return false;
}


// CTRADEAGENTLIST
bool CTRADEAGENTLIST::Load(void)
{
	CLCString sql(1024);
	
	//마감일이 지난 데이타 처리(반송 처리)
	//-->
	sql.Format("UPDATE t_tradeagent SET a_state = %d , a_process_date = NOW() "
		       "WHERE a_state = %d AND NOW() > DATE_ADD(a_regist_date, INTERVAL 5 DAY)"
			   ,MSG_TRADEAGENT_STATE_SELL_RETURNED 
			   ,MSG_TRADEAGENT_STATE_SELL_BEFORE);

	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );
	cmd.SetQuery(sql);
	if(!cmd.Update())
	{
		// 실패하면 로그 찍기
		GAMELOG << init("Fail Load Reg Overtime Set State")
				<< delim << end;	
	}
	//<--
	
	
	//마감일이 지나지 않은 데이타 처리 
	//-->
	sql.Format( "SELECT *,YEAR(a_regist_date) AS REG_YEAR,MONTH(a_regist_date) AS REG_MONTH,DAYOFMONTH(a_regist_date) AS REG_DAY,HOUR (a_regist_date) AS REG_HOUR, MINUTE(a_regist_date) AS REG_MINUTE, SECOND(a_regist_date) AS REG_SECOND FROM t_tradeagent WHERE a_state = %d ORDER BY a_index ASC"
				,MSG_TRADEAGENT_STATE_SELL_BEFORE);

	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		return false;
	}

	do 
	{
		CTRADEAGENT* pTradeAgent = new CTRADEAGENT();

		if(pTradeAgent)
		{
			cmd.GetRec( "a_index"			, pTradeAgent->m_dbIndex );

			cmd.GetRec( "a_item_serial"		, pTradeAgent->m_item_serial );
			cmd.GetRec( "a_sell_charindex"	, pTradeAgent->m_sell_charindex );

			cmd.GetRec( "REG_YEAR"			, pTradeAgent->m_regist_date.year);
			cmd.GetRec( "REG_MONTH"			, pTradeAgent->m_regist_date.month);
			cmd.GetRec( "REG_DAY"			, pTradeAgent->m_regist_date.day);
			cmd.GetRec( "REG_HOUR"			, pTradeAgent->m_regist_date.hour);
			cmd.GetRec( "REG_MINUTE"		, pTradeAgent->m_regist_date.min);
			cmd.GetRec( "REG_SECOND"		, pTradeAgent->m_regist_date.sec);
			
			cmd.GetRec( "a_state"			, pTradeAgent->m_state );

			pTradeAgent->SetItemRegFinishDay();

			//리스트 추가 
			Add(pTradeAgent);
		}		

	}while( cmd.MoveNext());
	//<--

	return false;
}


bool CTRADEAGENTLIST::Add(CTRADEAGENT *tra)
{
	if (!tra || tra->m_listIndex != -1 || m_List[tra->m_listIndex] == tra) 
		return false;

	int index = -1;
	
	if(FindInEmpty(index))
	{
		m_List[index] = tra;
		tra->m_listIndex = index;

		m_curList++;

		if(index > m_maxList)
			m_maxList =  index;

		if(m_curList > m_maxList)
			m_maxList =  m_curList;

		return true;
	}

	return false;
}

bool CTRADEAGENTLIST::FindInEmpty(int &index)
{
	for (int i = 0; i < MAX_TRADEAGENT_LIST; i++)
	{
		if (m_List[i] == NULL) 
		{
			index = i;
			return true;
		}
	}

	return false;
}

bool CTRADEAGENTLIST::Remove(int index)
{
	if (index < 0 || index >= MAX_TRADEAGENT_LIST)
		return false;

	if(m_List[index])
	{
		delete m_List[index]; 
		m_List[index] = NULL;

		m_curList--;
	}

	return true;
}

bool CTRADEAGENTLIST::Remove(CTRADEAGENT *tra)
{
	if (!tra || tra->m_listIndex < 0 || tra->m_listIndex >= MAX_TRADEAGENT_LIST)
		return false;

	//인덱스 보관
	int listindex = tra->m_listIndex;

	delete tra; 

	if(listindex >= 0)
	{
		m_List[listindex] = NULL;
	}

	m_curList--;

	return true;
}


CTRADEAGENT* CTRADEAGENTLIST::Find(int listIndex)
{
	for (int i = 0; i < MAX_TRADEAGENT_LIST; i++)
	{
		if (m_List[i] && m_List[i]->m_listIndex == listIndex)
			return m_List[i];
	}

	return NULL;
}

CTRADEAGENT* CTRADEAGENTLIST::FindDb(int dbIndex)
{
	for (int i = 0; i < MAX_TRADEAGENT_LIST; i++)
	{
		if (m_List[i] && m_List[i]->m_dbIndex == dbIndex)
			return m_List[i];
	}

	return NULL;
}

//상태 변경
bool CTRADEAGENTLIST::SetState(int dbIndex,int State)
{
	CTRADEAGENT* pTradeAgent = NULL;

	pTradeAgent = (CTRADEAGENT*) FindDb(dbIndex);
	if(pTradeAgent)
	{
		//상태가 바뀌면 리스트에서 삭제
		if(pTradeAgent->m_state == MSG_TRADEAGENT_STATE_SELL_BEFORE && pTradeAgent->m_state	!= State)
		{
			if(Remove(pTradeAgent))
				return true;
		}

		pTradeAgent->m_state = State;
	}

	return false;
}

//마감일 체크
void CTRADEAGENTLIST::CheckFinishday(void)
{
	for (int i = 0; i < m_maxList; i++)
	{
		if (m_List[i])
		{
		   CTRADEAGENT* pTradeAgent = (CTRADEAGENT*) m_List[i];	

		   if(pTradeAgent->IsPassFinishDay())
		   {
			  if(pTradeAgent->ChangeStateCalcNotify())
			  {
				  //상태가 바뀌면 리스트에서 삭제
				  Remove(pTradeAgent);
			  }
		   }
		}
	}
}

#endif	//TRADE_AGENT