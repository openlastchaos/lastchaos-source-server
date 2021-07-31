#ifndef __TRADEAGENT_H__
#define __TRADEAGENT_H__

#include "stdhdrs.h"

#ifdef TRADE_AGENT

#define MAX_TRADEAGENT_LIST			1024 * 10


//거래 대행(경매) 시스템
class CTRADEAGENT  
{
public:
	int				m_listIndex;

	int			    m_dbIndex;	
	int			    m_item_serial;
 	int				m_sell_charindex;

	TIMESTRUCT		m_regist_date;		//상품 등록 일	
	int				m_finish_date;

	int				 m_state;
public:
	CTRADEAGENT()
	{
		m_listIndex = -1;
		m_dbIndex = 0;
		m_item_serial = 0;
		m_sell_charindex = 0;
		m_state = 0;
	
		memset(&m_regist_date,0,sizeof(m_regist_date));
		m_finish_date = 0; 
	}

	~CTRADEAGENT()
	{

	}

	bool SetItemReg(int item_serial,int sell_charindex);	//등록 설정
	bool SetItemRegFinishDay(void);							//마감일을 설정 
	bool SetDbIndex(int& nDbIndex);							//DB INDEX 설정
	bool IsPassFinishDay(void);								//마감일이 지났는지 체크
	bool ChangeStateCalcNotify(void);						//DB 상태 변경 및 정산 필요 알림	
};

class CTRADEAGENTLIST  
{
public:
	CTRADEAGENT*	m_List[MAX_TRADEAGENT_LIST];	//리스트 : 현재 배열로 사용
	int				m_curList;						//현재 리스트 수
	int				m_maxList;						

	CTRADEAGENTLIST()
	{
		m_curList = 0;
		m_maxList = 0;

		for (int i = 0; i < MAX_TRADEAGENT_LIST; i++)
		{
			m_List[i] = NULL;
		}
	}

	virtual ~CTRADEAGENTLIST()
	{
	}

public:
	bool Load(void);							//서버 시작 DB에서 데이타 로드
	bool Add(CTRADEAGENT *tra);
	bool Remove(int index);
	bool Remove(CTRADEAGENT *tra);
	CTRADEAGENT* Find(int listIndex);
	CTRADEAGENT* FindDb(int dbIndex);
	bool FindInEmpty(int &index);
	bool SetState(int dbIndex,int State);		//상태 변경 
	void CheckFinishday(void);					//마감일 지남 체크
	int  GetCurList(void) { return m_curList; };
};

#endif  // TRADE_AGENT


#endif 
