#include "CmdMsg.h"

CBillingClient::CBillingClient()
{
	m_brunning = false;
	m_btrying = false;
	m_desc = -1;
	m_idleTics = 0;
}

bool CBillingClient::IsRunning()
{
	return (!m_btrying && m_brunning);
}

bool CBillingClient::IsTrying()
{
	return m_btrying;
}

#ifdef CIRCLE_WINDOWS
void CBillingClient::BillingConnect(LPVOID p)
{
	CBillingClient* _this = (CBillingClient*)p;
	while (_this->m_btrying)
	{
		if (_this->Create())
		{
			_this->SendHeader();
			_this->m_btrying = false;
			break;
		}
		else
		{
			_this->Close();
		}
		Sleep(2000);
	}
	_endthread();
}
#else
void* CBillingClient::BillingConnect(void* p)
{
	CBillingClient* _this = (CBillingClient*)p;
	while (_this->m_btrying)
	{
		if (_this->Create())
		{
			_this->SendHeader();
			_this->m_btrying = false;
			break;
		}
		else
		{
			_this->Close();
		}
		sleep(2);
	}

	pthread_exit(NULL);
}
#endif

bool CBillingClient::Create()
{
	if (m_brunning)
	{
		Close();
		m_brunning = false;
	}

	m_desc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_desc < 0)
	{
		GAMELOG << init("SYS_ERR") << "Create Socket" << end;
		return false;
	}

	struct sockaddr_in msg_socket;
	memset (&msg_socket, 0, sizeof(msg_socket));
	msg_socket.sin_family = AF_INET;
	msg_socket.sin_addr.s_addr = inet_addr(gserver.m_config.Find("Billing Server", "IP"));
	msg_socket.sin_port = htons(atoi(gserver.m_config.Find("Billing Server", "Port")));

	if (connect(m_desc, (struct sockaddr*)&msg_socket, sizeof(msg_socket)) < 0)
	{
		//puts("Error: Cannot Connect to Billing System");
		return false;
	}

	m_idleTics = 0;

	BillConnectMsg(m_connmsg);

	gserver.Nonblock(m_desc);
	gserver.SetSendbuf(m_desc);

	m_brunning = true;

	return true;
}

bool CBillingClient::ProcessInput()
{
	bool bGetPacket = false;
	ssize_t bytes_read;

	do
	{
		if (m_inBuf.GetRemain() <= 0)
			return false;

		bytes_read = CDescriptor::PerformSocketRead(m_desc, (char*)m_inBuf.GetPoint(), m_inBuf.GetRemain());

		if (bytes_read < 0)	/* Error, disconnect them. */
			return false;
		else if (bytes_read == 0)	/* Just blocking, no problems. */
			return true;

		m_inBuf.SetPoint(bytes_read);

		/* at this point, we know we got some data from the read */

		// Packet 얻기
		CBPacket pack;
		bool bStop = false;
		bool bFail = false;
		while (!bStop)
		{
			switch (m_inBuf.GetMessage(pack))
			{
			case 0:
				m_inQ.Add(pack);
				bGetPacket = true;
				break;
			case 1:
				bStop = true;
				break;
			case -1:
				bFail = true;
				bStop = true;
				break;
			}
		}

		if (bFail)
			return false;

#if !defined(POSIX_NONBLOCK_BROKEN)
	} while (!bGetPacket);
#else
	} while (0);
#endif /* POSIX_NONBLOCK_BROKEN */
	return true;
}

bool CBillingClient::ProcessOutput()
{
	int result;
	unsigned char* p;

	while ((p = m_outBuf.GetNextPoint()))
	{
		gserver.m_serverAliveTime = PULSE_SERVER_ALIVE;
		result = CDescriptor::WriteToDescriptor(m_desc, (char*)p, m_outBuf.GetRemain());
		if (result < 0)
			return false;
		else if (result == 0 || m_outBuf.Update(result))
			return true;
	}

	return true;
}

bool CBillingClient::Login(const char* name, const char* pw, const char* ip)
{
	return true;
}

bool CBillingClient::Logout(const char* name)
{
	return true;
}

void CBillingClient::ProcessBilling()
{

	CBPacket inpack;
	if (!m_inQ.Get(inpack) )
		return;

	switch(inpack.m_mtype)
	{
	case MSG_BILLITEM_CASHBALANCE_REP:
		{		
			OnCashBalance(inpack);
		}
		break;
	case MSG_BILLITEM_CANBUY_REP:
		{
			
			OnCanBuy(inpack);
		}
		break;
	case MSG_BILLITEM_BUY_REP:
		{
			
			OnBuy(inpack);
		}
		break;
#ifdef CASH_ITEM_COUPON
	case MSG_BILLITEM_COUPON_LIST_REP:
		{
			OnCashCouponList(inpack);
		}
		break;
	case MSG_BILLITEM_BUYBYCOUPON_REP:
		{
			OnBuyByCoupon(inpack);
		}
		break;
#endif // CASH_ITEM_COUPON

	case MSG_BILLITEM_SERVERLIVE:	// 빌링 서버 접속 유지용 프로토콜
		GAMELOG << init("MSG_BILLITEM_SERVERLIVE") << end;
		break;

	default:
		{
			GAMELOG << init("UNKNOWN PACKET") << inpack.m_mtype << end;
			return;
		}
		break;
	}

}

void CBillingClient::OnCashBalance(CBPacket &inpack)
{
	unsigned char returnCode = 0;
	int userindex;
	unsigned char cid = 0;
	int cashBalance = 0;
	int bonusPoint;
	CLCString idname(51);
	//char idname[MAX_ID_NAME_LENGTH + 1];
	inpack.MoveFirst();
	inpack >> returnCode
		   >> idname
		   >> cid
		   >> userindex
		   >> cashBalance
		   >> bonusPoint;

	CUser * user = NULL;
	for(int i = 0; i < gserver.m_maxSubServer; i++)
	{
		user = gserver.m_userList[i].Find(userindex);
		if( user )
			break;
	}

	if( !user )
	{
		GAMELOG << init("CASH_BALANCE_ERROR")
				<< userindex;
		return;
	}

	GAMELOG << init("CASH_BALANCE_REP")
		<< returnCode << delim 
		<< idname << delim 
		<< cid << delim 
		<< userindex << delim 
		<< cashBalance << delim 
		<< bonusPoint << end;

	// MSG_CONN_CASHiTEM_BALANCE_REP 
	CNetMsg rmsg;
	ConnCashItemBalanceRep(rmsg, user->m_index, (MSG_EX_CASHITEM_ERROR_TYPE) returnCode, cashBalance);

	SEND_Q(rmsg, user->m_descserver);
}

void CBillingClient::OnCanBuy(CBPacket &pack)
{
	// MSG_BILLITEM_CANBUY_REP,	
	// 응답	  : returnCode(uc) gameId(uc) userId(str:32) cpId(n) itemId(n) cashBalance(int)
	// returnCode에 따른 행동을 하시오
	unsigned char cpId, returnCode, count;
	int userindex;
	int  cashBalance, bonusPoint, ctId[MAX_PURCHASEITEM], i;
	CLCString idname(51);
	//char idname[MAX_ID_NAME_LENGTH + 1];

	pack >> returnCode
		 >> idname
		 >> cpId
		 >> userindex
		 >> count;

	for(i = 0; i < MAX_PURCHASEITEM && i < count; i++)
	{
		pack >> ctId[i];
	}

	pack >> cashBalance
		 >> bonusPoint;

	GAMELOG << init("CASH_ITEM_BUY");

	switch(returnCode)
	{
	case MSG_BILLITEM_ERROR_SUCCESS: // 성공
		GAMELOG << "MSG_BILLITEM_ERROR_SUCCESS" << delim;
		break;
	case MSG_BILLITEM_ERROR_LACKCASH: // 잔액부족
		GAMELOG << "MSG_BILLITEM_ERROR_LACKCASH" << delim;
		break;
	case MSG_BILLITEM_ERROR_NOTUSER: // 해당유저 없음
		GAMELOG << "MSG_BILLITEM_ERROR_NOTUSER" << delim;
		break;
	case MSG_BILLITEM_ERROR_NOTITEM: // 해당아이템 없음
		GAMELOG << "MSG_BILLITEM_ERROR_NOTITEM" << delim;
		break;
	case MSG_BILLITEM_ERROR_DATABASE: // DB 오류
		GAMELOG << "MSG_BILLITEM_ERROR_DATABASE" << delim;
		break;
	case MSG_BILLITEM_ERROR_PACKET: // 패킷 오류
		GAMELOG << "MSG_BILLITEM_ERROR_PACKET" << delim;
		break;
	case MSG_BILLITEM_ERROR_ETC: // 기타 오류
		GAMELOG << "MSG_BILLITEM_ERROR_ETC" << delim;
		break;
	case MSG_BILLITEM_ERROR_NOTACCOUNT: // 결재 권한 없음
		GAMELOG << "MSG_BILLITEM_ERROR_NOTACCOUNT" << delim;
		break;
	default:
		GAMELOG << "KNOWN_ERROR" << delim;
		break;
	}

	GAMELOG << idname << delim
		<< cpId << delim
		<< userindex << delim
		<< count << delim;


	for(i = 0; i < MAX_PURCHASEITEM && i < count; i++)
	{
		GAMELOG << ctId[i] << delim;
	}

	GAMELOG << cashBalance << delim
		 << bonusPoint << end;

	CUser* user = NULL;
	for(i = 0; i < gserver.m_maxSubServer; i++)
	{
		user = gserver.m_userList[i].Find(userindex);
		if(user) break;
	} 

	if(!user)
	{
		GAMELOG << init("CASH_PURCHASE_REP_ERROR-NotUser")
				<< userindex << end;
		return;
	}

	if( returnCode == MSG_BILLITEM_ERROR_SUCCESS)
	{
		// 차감 요청을 함
		// DB의 구매필드를 추가, 시리얼 번호 설정후 거시기 해서 거시기 하시오

		CDBCmd cmd;
		cmd.Init(&gserver.m_dbuser);

//		a_index,	a_user_idx,a_server,a_ctid,a_serial,a_pdate,a_use_char_idx, a_use_date
		sprintf(g_buf, "insert into t_purchase0%d ( a_user_idx, a_server, a_ctid, a_serial, a_pdate, a_ip) values ", userindex % 10);

		for(i = 0; i < MAX_PURCHASEITEM && i < count && ctId[i] != -1; i++)
		{
			sprintf(g_buf2, "(%d, %d, %d, -1, now(), '%s'), ", user->m_index, gserver.m_serverno, ctId[i], (const char*)user->m_ip ) ;
			strcat(g_buf,  g_buf2);
		}
		
		for(i = 0; g_buf[i] != '\0'; i++) ;
		g_buf[i-2] = ' ';

		cmd.BeginTrans();

		cmd.SetQuery(g_buf);
		if( !cmd.Update() )
		{
			CNetMsg rmsg;
			ConnCashItemPurchaseRep(rmsg, userindex, MSG_EX_CASHITEM_ERROR_CONN, cashBalance);
			SEND_Q(rmsg, user->m_descserver);

			GAMELOG << init("CASH_PURCHASE_REP_ERROR-NotUpdateDB")
					<< mysql_error(&gserver.m_dbuser) << delim
					<< userindex << end;

			cmd.Rollback();
			return;
		}

		int serial = cmd.insertid() * 10 + userindex % 10;
		sprintf(g_buf, "update t_purchase0%d set a_serial = %d where a_serial = -1 ", userindex % 10, serial);

		cmd.SetQuery(g_buf);
		if( !cmd.Update() )
		{
			CNetMsg rmsg;
			ConnCashItemPurchaseRep(rmsg, userindex, MSG_EX_CASHITEM_ERROR_CONN, cashBalance);
			SEND_Q(rmsg, user->m_descserver);

			GAMELOG << init("CASH_PURCHASE_REP_ERROR-NotUpdateDB")
					<< mysql_error(&gserver.m_dbuser) << delim
					<< userindex << end;

			cmd.Rollback();
			return;
		}

		cmd.Commit();

		if( gserver.m_billing.IsRunning() )
		{
			CBPacket pack;
			BillBuyReqMsg(pack, userindex, user->m_name, serial, ctId, user->m_ip, count);
			gserver.m_billing.WriteToOutput(pack);

			user->m_descserver->WaitBillReply(serial);


			return;
		}
		else
		{
			if(user)
			{
				CNetMsg rmsg;
				ConnCashItemPurchaseRep(rmsg, userindex, MSG_EX_CASHITEM_ERROR_BILL, cashBalance);
				SEND_Q(rmsg, user->m_descserver);
			}
			else
			{
				GAMELOG << init("CASH_PURCHASE_REP_ERROR-NotUser")
						<< userindex << end;
			}

			// 빌링 서버가 돌지 않고 있으므로  롤백 : 해당 시리얼을 가진 아이템 삭제

			sprintf(g_buf, "delete from t_purchase0%d where a_serial = %d", userindex % 10, serial);
			cmd.SetQuery(g_buf);

			if( !cmd.Update() )
			{
				GAMELOG << init("CASH_PURCHASE_REP_ERROR-RollBack-update fail")
						<< userindex << delim;

				for(i = 0; i < MAX_PURCHASEITEM; i++)
				{
					GAMELOG << ctId[i] << delim;
				}
				GAMELOG << end;
						

			}

		}

		return;
	}
	
	// 에러 메세지에 해당하는 리플라이
	CNetMsg rmsg;
	ConnCashItemPurchaseRep(rmsg, userindex, (MSG_EX_CASHITEM_ERROR_TYPE) returnCode, cashBalance);
	SEND_Q(rmsg, user->m_descserver);
}

void CBillingClient::OnBuy(CBPacket &pack)
{
	//MSG_BILLITEM_BUY_REP,		// 응답	  : returnCode(uc) gameId(uc) userId(str:32) cpId(n) itemId(n) cashBalance(int)
	unsigned char cpId, returnCode, count;
	int userindex;
	int  cashBalance, ctId[MAX_PURCHASEITEM], i, serial, bonuspoint;
	CLCString idname(51);

	serial = pack.m_serial;

	pack >> returnCode
		 >> idname
		 >> cpId
		 >> userindex
		 >> count;

	void* pos;
	pos = NULL;
	CLimitSell* limit;
	limit = NULL;
	for(i = 0; i < MAX_PURCHASEITEM && i < count; i++)
	{
		pack >> ctId[i];
#ifdef LIMIT_CATALOG
		// 현재 구입 성공한 상품 인덱스로 커넥터가 가지고 있는 한정판매 리스트를 찾아서
		// 있으면 팔린 갯수를 증가 시키고 모든 게임서버로 메세지를 보낸다.
		limit = new CLimitSell(ctId[i], 0);

		pos = gserver.m_limitSell.FindDataWithCompareFunction(limit, CLimitSell::CompByIndex);
		if( pos )
		{
			delete limit;
			limit = gserver.m_limitSell.GetData(pos);
			if( limit )
			{
				limit->IncreaseSell();
				CNetMsg smsg;
				LimitCatalogMsg(smsg, limit);
				gserver.SendAllGameServer(smsg);
				gserver.m_changeLimit = true;
				
			}
		}
#endif
	}

	pack >> cashBalance
		 >> bonuspoint;

	CUser* user = NULL;
	for(i = 0; i < gserver.m_maxSubServer; i++)
	{
		user = gserver.m_userList[i].Find(userindex);
		if(user) break;
	}

	if(user)
	{
		user->m_descserver->WaitBillReply(0);
		
	}
	else
	{
		GAMELOG << init("CASH_PURCHASE_REP_ERROR-NotUser")
				<< userindex << end;
	}


	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	if( returnCode == MSG_BILLITEM_ERROR_SUCCESS)
	{
		// 해당 시리얼을 가진 레코드를 컨펌한다.
		sprintf(g_buf, "update t_purchase0%d set a_confirm = 1 where a_serial = %d ", userindex % 10, serial);
		cmd.SetQuery(g_buf);

		if( !cmd.Update() )
		{
			GAMELOG << init("Confirm Error")
					<< serial
					<< end;
		}

		CNetMsg rmsg;
		ConnCashItemPurchaseRep(rmsg, userindex, (MSG_EX_CASHITEM_ERROR_TYPE) returnCode, cashBalance);
		if( user )
			SEND_Q(rmsg, user->m_descserver);
		return;
	}
	else
	{
		// 시리얼 값을 가진 t_purchase를 모두 롤백하고
		// 에러코드에 따른 값을 유저에게 전달한다
		
		sprintf(g_buf, "delete from t_purchase0%d where a_serial = %d", userindex % 10, serial);
		cmd.SetQuery(g_buf);

		if( !cmd.Update() )
		{
			GAMELOG << init("CASH_PURCHASE_REP_ERROR-RollBack-update fail")
					<< userindex << delim;

			for(i = 0; i < MAX_PURCHASEITEM; i++)
			{
				GAMELOG << ctId[i] << delim;
			}
			GAMELOG << end;
					

		}
		CNetMsg rmsg;
		ConnCashItemPurchaseRep(rmsg, userindex, (MSG_EX_CASHITEM_ERROR_TYPE) returnCode, cashBalance);

		if( user )
			SEND_Q(rmsg, user->m_descserver);
		return;
	}

}

#ifdef CASH_ITEM_COUPON
void CBillingClient::OnCashCouponList(CBPacket &inpack)
{
	int userindex;
	unsigned char cid = 0;
	int couponCount = 0;
	CLCString idname(51);
	CLCString tempBuff(51), tempBuff2(51);
	CASH_COUPON *pCoupon;

	inpack.MoveFirst();
	inpack >> idname
		   >> cid
		   >> userindex
		   >> couponCount;
	
	pCoupon = new CASH_COUPON[couponCount];
	int count=0;
	for( count = 0; count < couponCount; count++ )
	{
		inpack	>> tempBuff
				>> pCoupon[count].id
				>> pCoupon[count].serial
				>> pCoupon[count].amount;

		int i=0;
		for(; i<51; i++ ) 
		{
			if( tempBuff[i] == '|' )
			{
				i++;
				break;
			}
		}
		pCoupon[count].name.CopyFrom( (const char*)tempBuff, i );
		tempBuff2.Format("%s", ((const char*)tempBuff)+i );

		pCoupon[count].limitAmount = atoi( (const char *) tempBuff2 );
	}
	
	CUser * user = NULL;
	for(int i = 0; i < gserver.m_maxSubServer; i++)
	{
		user = gserver.m_userList[i].Find(userindex);
		if( user )
			break;
	}

	if( !user )
	{
		GAMELOG << init("CASH_BALANCE_ERROR")
				<< userindex;
		return;
	}

	GAMELOG << init("CASH_BALANCE_REP")
		<< idname << delim 
		<< cid << delim 
		<< userindex << end; 

	// MSG_CONN_CASHiTEM_BALANCE_REP 
	CNetMsg rmsg;
	ConnCashItemCouponListRep(rmsg, user->m_index, (MSG_EX_CASHITEM_ERROR_TYPE) 0, couponCount, pCoupon );
	SEND_Q(rmsg, user->m_descserver);

	delete[] pCoupon;
}

void CBillingClient::OnBuyByCoupon(CBPacket &pack)
{
// 에러 처리및 로그및 쿠폰 사용여부 저장

	unsigned char cpId, returnCode, count;
	int userindex;
	int  cashBalance, ctId[MAX_PURCHASEITEM], i, serial, bonuspoint;
	CLCString idname(51);

	serial = pack.m_serial;

	pack >> returnCode
		 >> idname
		 >> cpId
		 >> userindex
		 >> count;

	void* pos;
	pos = NULL;
	CLimitSell* limit;
	limit = NULL;
	for(i = 0; i < MAX_PURCHASEITEM && i < count; i++)
	{
		pack >> ctId[i];
#ifdef LIMIT_CATALOG
		// 현재 구입 성공한 상품 인덱스로 커넥터가 가지고 있는 한정판매 리스트를 찾아서
		// 있으면 팔린 갯수를 증가 시키고 모든 게임서버로 메세지를 보낸다.
		limit = new CLimitSell(ctId[i], 0);

		pos = gserver.m_limitSell.FindDataWithCompareFunction(limit, CLimitSell::CompByIndex);
		if( pos )
		{
			delete limit;
			limit = gserver.m_limitSell.GetData(pos);
			if( limit )
			{
				limit->IncreaseSell();
				CNetMsg smsg;
				LimitCatalogMsg(smsg, limit);
				gserver.SendAllGameServer(smsg);
				gserver.m_changeLimit = true;
				
			}
		}
#endif
	}

	pack >> cashBalance
		 >> bonuspoint;

	CUser* user = NULL;
	for(i = 0; i < gserver.m_maxSubServer; i++)
	{
		user = gserver.m_userList[i].Find(userindex);
		if(user) break;
	}

	if(user)
	{
		user->m_descserver->WaitBillReply(0);
		
	}
	else
	{
		GAMELOG << init("CASH_PURCHASE_REP_ERROR-NotUser")
				<< userindex << end;
	}


	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	if( returnCode == MSG_BILLITEM_ERROR_SUCCESS)
	{
		// 해당 시리얼을 가진 레코드를 컨펌한다.
		sprintf(g_buf, "update t_purchase0%d set a_confirm = 1 where a_serial = %d ", userindex % 10, serial);
		cmd.SetQuery(g_buf);

		if( !cmd.Update() )
		{
			GAMELOG << init("Confirm Error")
					<< serial
					<< end;
		}

		CNetMsg rmsg;
		ConnCashItemPurchaseByCouponRep(rmsg, userindex, (MSG_EX_CASHITEM_ERROR_TYPE) returnCode, cashBalance);
		if( user )
			SEND_Q(rmsg, user->m_descserver);
		return;
	}
	else
	{
		// 시리얼 값을 가진 t_purchase를 모두 롤백하고
		// 에러코드에 따른 값을 유저에게 전달한다
		
		sprintf(g_buf, "delete from t_purchase0%d where a_serial = %d", userindex % 10, serial);
		cmd.SetQuery(g_buf);

		if( !cmd.Update() )
		{
			GAMELOG << init("CASH_PURCHASE_REP_ERROR-RollBack-update fail")
					<< userindex << delim;

			for(i = 0; i < MAX_PURCHASEITEM; i++)
			{
				GAMELOG << ctId[i] << delim;
			}
			GAMELOG << end;
					

		}
		CNetMsg rmsg;
		ConnCashItemPurchaseByCouponRep(rmsg, userindex, (MSG_EX_CASHITEM_ERROR_TYPE) returnCode, cashBalance);

		if( user )
			SEND_Q(rmsg, user->m_descserver);
		return;
	}
}
#endif //CASH_ITEM_COUPON

void CBillingClient::SendHeader()
{
	m_inQ.Clear();
	m_inBuf.Clear();
	m_outBuf.Clear();

	WriteToOutput(m_connmsg);
}

void CBillingClient::Close()
{
	m_brunning = false;

	if (m_desc > 0)
	{
		CLOSE_SOCKET(m_desc);
		m_desc = -1;
	}
}

void CBillingClient::WriteToOutput(CBPacket& pack)
{
	if (pack.m_mtype != BM_CONNECT && !IsRunning())
		return ;
	if (pack.m_mtype == -1)
		return ;
	if (!m_outBuf.Add(pack))
	{
		GAMELOG << init("OUTPUT OVERFLOW")
				<< end;
		return ;
	}
	ProcessOutput();
}