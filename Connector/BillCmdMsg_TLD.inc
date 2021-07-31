
void BillConnectMsg(CNetMsg& msg)
{
	msg.Init(BM_CONNECT);

	//BM_CONNECT,								// 目池磐 立加 夸没: protocolversion(str:17) serverNum(uc) gameId(uc)
	msg << BILL_SERVER_VERSION
		<< (unsigned char) gserver.m_serverno
		<< (unsigned char) GAMEID;
		//<< (unsigned char) 3;
}

void BillBalanceReqMsg(CNetMsg& msg, int userindex, const char* idname)
{
	msg.Init(MSG_BILLITEM_CASHBALANCE_REQ);

	msg << (unsigned char) GAMEID
		<< userindex
		 << idname
		 << (int) CPID;
}
void BillCanbuyReqMsg(CNetMsg& msg, int userindex, const char* idname, char count, int ctid[], int requestCash)
{
	msg.Init(MSG_BILLITEM_CANBUY_REQ);
	msg << (unsigned char) GAMEID
		 << userindex
		 << idname
		 << (int) CPID
		 << count;

	for(int i = 0; i < count && i < MAX_PURCHASEITEM; i++)
	{
		msg << ctid[i];
	}
	msg << requestCash;
}

void BillBuyReqMsg(CNetMsg& msg, int userindex, const char* userid, int serial, char count, int ctid[], const char* ip)
{
	msg.Init(MSG_BILLITEM_BUY_REQ);
	msg  << serial
		 << (unsigned char) GAMEID
		 << userindex
		 << userid
		 << (int) CPID
		 << ip
		 << count;

	for(int i = 0; i < count && i < MAX_PURCHASEITEM; i++)
	{
		msg << ctid[i];
	}
}
