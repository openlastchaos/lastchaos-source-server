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
		CNetMsg pack;
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

	CNetMsg inpack;
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
	default:
		{
			GAMELOG << init("UNKNOWN PACKET") << inpack.m_mtype << end;
			return;
		}
		break;
	}

}

void CBillingClient::OnCashBalance(CNetMsg &inpack)
{
	unsigned char returnCode = 0;
	unsigned char gid = 0;
	int userindex;
	int cid = 0;
	int cashBalance = 0;
	inpack.MoveFirst();
	inpack >> returnCode
		   >> gid
		   >> userindex
		   >> cid
		   >> cashBalance;

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

	//MSG_CONN_CASHiTEM_BALANCE_REP 
	CNetMsg rmsg;
	ConnCashItemBalanceRep(rmsg, user->m_index, (MSG_EX_CASHITEM_ERROR_TYPE) returnCode, cashBalance);

	SEND_Q(rmsg, user->m_descserver);
}
void CBillingClient::OnCanBuy(CNetMsg &pack)
{
	//MSG_BILLITEM_CANBUY_REP,	// 응답	  : returnCode(uc) gameId(uc) userId(str:32) cpId(n) itemId(n) cashBalance(int)
	// returnCode에 따른 행동을 하시오
	unsigned char returnCode, gameId;
	int userindex;
	int cpId, cashBalance, ctId[MAX_PURCHASEITEM], i;
	char count;

	pack >> returnCode
		 >> gameId
		 >> userindex
		 >> cpId
		 >> count;

	for(i = 0; i < count && i < MAX_PURCHASEITEM; i++)
	{
		pack >> ctId[i];
	}

	pack >> cashBalance;

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

		for(i = 0; i < count && i < MAX_PURCHASEITEM && ctId[i] != -1; i++)
		{
			sprintf(g_buf2, "(%d, %d, %d, -1, now(), '%s'), ", user->m_index, gserver.m_serverno, ctId[i], (const char*) user->m_ip ) ;
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
			CNetMsg pack;
			BillBuyReqMsg(pack, userindex, user->m_name, serial, count, ctId, user->m_ip);
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
void CBillingClient::OnBuy(CNetMsg &pack)
{
	//MSG_BILLITEM_BUY_REP,		// 응답	  : returnCode(uc) gameId(uc) userId(str:32) cpId(n) itemId(n) cashBalance(int)
	unsigned char returnCode, gameId;
	int userindex;
	int cpId, cashBalance, ctId[MAX_PURCHASEITEM], i, serial;
	char count;

	pack >> serial
		 >> returnCode
		 >> gameId
		 >> userindex
		 >> cpId
		 >> count;

	void* pos;
	pos = NULL;
	CLimitSell* limit;
	limit = NULL;
	for(i = 0; i < count && i < MAX_PURCHASEITEM; i++)
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

	pack >> cashBalance;

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

	user->m_descserver->WaitBillReply(0);

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
		SEND_Q(rmsg, user->m_descserver);
		return;
	}

}

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

void CBillingClient::WriteToOutput(CNetMsg& pack)
{
	if (pack.m_mtype != BM_CONNECT && !IsRunning())
		return ;
	if (pack.m_mtype == -1)
		return ;
	if (!m_outBuf.Add(pack, false, 0))
	{
		GAMELOG << init("OUTPUT OVERFLOW")
				<< end;
		return ;
	}
	ProcessOutput();
}