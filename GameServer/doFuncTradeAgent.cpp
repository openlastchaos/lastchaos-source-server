#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "hardcoding.h"
#include "Battle.h"
#include "Exp.h"
#include "WarCastle.h"
#include "DratanCastle.h"

#ifdef TRADE_AGENT

void do_TradeAgent(CPC* ch, CNetMsg& msg)
{
	// 죽거나 교환중이면 무시
	if (DEAD(ch) || ch->m_exchange != NULL || ch->m_currentSkill != NULL || ch->m_personalshop != NULL)
		return ;
	
	msg.MoveFirst();
	
	unsigned char subtype;
	msg >> subtype;
	
	switch (subtype)
	{
	case MSG_TRADEAGENT_REG_LIST_REQ:
		do_TradeAgent_RegListReq(ch, msg);
		break;		
	case MSG_TRADEAGENT_REG_REQ:
		do_TradeAgent_RegReq(ch, msg);
		break;
	case MSG_TRADEAGENT_REG_CANCEL_REQ:
		do_TradeAgent_RegCancelReq(ch, msg);
		break;		
	case MSG_TRADEAGENT_SEARCH_REQ:
		do_TradeAgent_SearchReq(ch, msg);
		break;
	case MSG_TRADEAGENT_BUY_REQ:
		do_TradeAgent_BuyReq(ch, msg);
		break;
	case MSG_TRADEAGENT_CALCLIST_REQ:
		do_TradeAgent_CalcListReq(ch, msg);
		break;
	case MSG_TRADEAGENT_CALCULATE_REQ:
		do_TradeAgent_CalculateReq(ch, msg);
		break;
	case MSG_TRADEAGENT_CHECKCALC_REQ:
		do_TradeAgent_CheckCalcReq(ch, msg);
		break;		
	}
}

// 등록 리스트 요청
void do_TradeAgent_RegListReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	int page_no,nAlignType;

	msg >> page_no >> nAlignType;

	if (IS_RUNNING_HELPER)
	{
		HelperTradeAgentRegListReqMsg(rmsg, ch->m_index,page_no, nAlignType);
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NORMAL,MSG_TRADEAGENT_ERROR_PART_REGIST);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

// 등록 요청
void do_TradeAgent_RegReq(CPC* ch, CNetMsg& msg)
{
	char			tab, row, col;
	int				nitem_idx;		//아이템 DB index
	int				nQuantity;		//수량
    LONGLONG		llTotalMoney;	//판매 총 금액 
    LONGLONG		llGuaranty;		//보증금
	LONGLONG		llCurCharHaveNas = 0;
	
	msg >> tab >> row >> col >> nitem_idx
		>> nQuantity >> llTotalMoney >> llGuaranty;
	
	CNetMsg rmsg;
	CInventory* inven;
	CItem* item;
	const CItemProto* itemproto;

	//현재 등록이 처리중인지 체크(에러 메세지 보내지 않고 차단)
	if(ch->m_bTradeAgentRegIng == true)
	{
//		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_REGIST_ING_NOREG,MSG_TRADEAGENT_ERROR_PART_REGIST);
//		SEND_Q(rmsg, ch->m_desc);
		return ;
	}	

	// 아이템 찾기
	inven = GET_INVENTORY(ch, tab);
	if (inven == NULL)
		return;

	item = inven->GetItem(row, col);
	if (item == NULL || item->m_idNum != nitem_idx || item->m_wearPos != -1)
		return;
	
	itemproto = item->m_itemProto;
	if (itemproto == NULL)
		return;

	//아이템 수량 체크 
	if(nQuantity <= 0)
	{
		// 에러 메세지: 아이템 개수 부정확 	
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NOT_ITEMCOUNT,MSG_TRADEAGENT_ERROR_PART_REGIST);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//총 금액 체크 
	if(llTotalMoney <= 0)
	{
		// 에러 메세지: 판매 총 금액 부정확		
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NOT_SELL_TOTALMONEY,MSG_TRADEAGENT_ERROR_PART_REGIST);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( !(itemproto->m_flag & ITEM_FLAG_EXCHANGE) || 
		 (itemproto->m_flag & ITEM_FLAG_COMPOSITE) ||
		 (item->m_flag & FLAG_ITEM_SEALED) ||
		 (item->m_flag & FLAG_ITEM_COMPOSITION) ||
		 (item->m_flag & FLAG_ITEM_LENT) ||
#ifdef SET_ITEM
		 (item->m_flag & FLAG_ITEM_BELONG) ||
#endif
		 ( item->m_used > 0 ) || ( item->m_used_2 > 0 ) 
	)
	{
		// 에러 메세지: 거래 불가능 아이템		
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NOT_TRADEITEM,MSG_TRADEAGENT_ERROR_PART_REGIST);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//악세사리 , 애완동물 거래 불가
	//악세사리 , 공격형 애완동물 거래 불가
	if( ((itemproto->m_typeIdx == 5) && (itemproto->m_subtypeIdx == 6)) ||
		((itemproto->m_typeIdx == 5) && (itemproto->m_subtypeIdx == 7)) )	
	{
		// 에러 메세지: 거래 불가능 아이템		
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NOT_TRADEITEM,MSG_TRADEAGENT_ERROR_PART_REGIST);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 수량 체크 
	if( item->Count() < nQuantity)
	{
		// 에러 메세지: 아이템 개수 부정확
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NOT_ITEMCOUNT,MSG_TRADEAGENT_ERROR_PART_REGIST);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	//보증금은 Helper에서 체크


	if (ch->m_moneyItem)
	{ 
		llCurCharHaveNas = ch->m_moneyItem->Count();	
	}	

	if (IS_RUNNING_HELPER)
	{
		HelperTradeAgentRegReqMsg(rmsg, ch->m_index, ch->GetName(), tab, row, col,itemproto->m_index,item->m_serial, item, itemproto->m_name, itemproto->GetItemProtoLevel(), itemproto->m_typeIdx, itemproto->m_subtypeIdx,itemproto->m_jobFlag, nQuantity,llTotalMoney,llGuaranty,llCurCharHaveNas);
		SEND_Q(rmsg, gserver.m_helper);

		//현재 등록 처리 중 설정 
		//-->
		ch->m_bTradeAgentRegIng = true;
		//<--	
	}
	else
	{
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NORMAL,MSG_TRADEAGENT_ERROR_PART_REGIST);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}	

}

// 등록 취소
void do_TradeAgent_RegCancelReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;	
	int	nCancelindex;

	msg >> nCancelindex;

	if (IS_RUNNING_HELPER)
	{
		HelperTradeAgentRegCancelReqMsg(rmsg, ch->m_index,nCancelindex);
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NORMAL,MSG_TRADEAGENT_ERROR_PART_REGIST);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}		
}

// 조회 
void do_TradeAgent_SearchReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;	
	int	nitemType,nitemSubType,nJobClass;
	CLCString stSearchWord(20 + 1);
	int nPageNo,nAlignType;

	msg >> nitemType >> nitemSubType >> nJobClass
		>> stSearchWord
		>> nPageNo
		>> nAlignType;

	if (IS_RUNNING_HELPER)
	{
 		HelperTradeAgentSearchReqMsg(rmsg, ch->m_index, nPageNo, nitemType,nitemSubType,nJobClass,stSearchWord, nAlignType);
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
 		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NORMAL,MSG_TRADEAGENT_ERROR_PART_SEARCH);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

// 구매 
void do_TradeAgent_BuyReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;	
	int	nList_idx;
    LONGLONG TotalMoney;	
	LONGLONG llCurCharHaveNas = 0;

	msg >> nList_idx 
		>> TotalMoney;

	if (ch->m_moneyItem)
	{ 
		//총금액 체크
		if ((ch->m_moneyItem->Count() < TotalMoney))
		{
			//보유 나스 부족 
			TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NOT_ENUGH_MONEY,MSG_TRADEAGENT_ERROR_PART_SEARCH);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}else
	{
		//보유 나스 부족 
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NOT_ENUGH_MONEY,MSG_TRADEAGENT_ERROR_PART_SEARCH);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_moneyItem)
	{ 
		llCurCharHaveNas = ch->m_moneyItem->Count();	
	}

	if (IS_RUNNING_HELPER)
	{
 		HelperTradeAgentBuyReqMsg(rmsg, ch->m_index,ch->GetName(),nList_idx,llCurCharHaveNas);
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
 		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NORMAL,MSG_TRADEAGENT_ERROR_PART_SEARCH);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

// 정산 리스트 요청 
void do_TradeAgent_CalcListReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;
	int page_no,nAlignType;

	msg >> page_no >> nAlignType;

	if (IS_RUNNING_HELPER)
	{
		//페이지 번호 저장
		ch->m_nCalcPageNo = page_no;

		HelperTradeAgentCalcListReqMsg(rmsg, ch->m_index,page_no, nAlignType);
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NORMAL,MSG_TRADEAGENT_ERROR_PART_CALCULATE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

// 정산 요청 
void do_TradeAgent_CalculateReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	//현재 정산이 처리중인지 체크(에러 메세지 보내지 않고 차단)
	if(ch->m_bTradeAgentCalcIng == true)
	{
//		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_CALCULATE_ING_NOCALC,MSG_TRADEAGENT_ERROR_PART_CALCULATE);
//		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (IS_RUNNING_HELPER)  
	{
		HelperTradeAgentCalculateReqMsg(rmsg, ch->m_index,ch->m_nCalcPageNo);
		SEND_Q(rmsg, gserver.m_helper);

		//현재 정산 처리 중 설정 
		//-->
		ch->m_bTradeAgentCalcIng = true;
		//<--
	}
	else
	{
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NORMAL,MSG_TRADEAGENT_ERROR_PART_CALCULATE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

//정산 필요 체크(로그인)
void do_TradeAgent_CheckCalcReq(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	if (IS_RUNNING_HELPER)  
	{
		HelperTradeAgentCheckCalcReqMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		TradeAgentErrorMsg(rmsg, MSG_TRADEAGENT_ERROR_NORMAL,MSG_TRADEAGENT_ERROR_PART_CALCULATE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}	
}

#endif	// TRADE_AGENT