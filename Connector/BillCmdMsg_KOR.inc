
void BillConnectMsg(CBPacket& msg)
{
	CLCString protocolVersion(17);

	protocolVersion.Format("%s", BILL_SERVER_VERSION);

	msg.Init(BM_CONNECT);
	msg << protocolVersion
		<< (unsigned char) gserver.m_serverno
		<< (unsigned char) GAMEID;
}

void BillBalanceReqMsg(CBPacket& msg, int userindex, const char* idname)
{
	CLCString id(51);
	id.Format("%s", idname);

	msg.Init(MSG_BILLITEM_CASHBALANCE_REQ);

	msg << id
		<< (unsigned char) CPID
		<< userindex;
}

void BillCanbuyReqMsg(CBPacket& msg, int userindex, const char* idname, int ctid[], char count, const char* ip)
{
	CLCString id(51);
	id.Format("%s", idname);
	CLCString lcIP(16);
	lcIP.Format("%s", ip);

	msg.Init(MSG_BILLITEM_CANBUY_REQ);

	msg  << id
		 << (unsigned char) CPID
		 << userindex
		 << lcIP
		 << (unsigned char) count;

	for(int i = 0; i < MAX_PURCHASEITEM && i < count; i++)
	{
		msg << ctid[i];
	}
}

void BillBuyReqMsg(CBPacket& msg, int userindex, const char* userid, int serial, int ctid[], const char* ip, char count)
{
	msg.Init(MSG_BILLITEM_BUY_REQ);
	msg.m_serial = serial;

	CLCString id(51);
	id.Format("%s", userid);
	CLCString lcIP(16);
	lcIP.Format("%s", ip);


	msg  << id
		 << (unsigned char) CPID
		 << userindex
		 << lcIP
		 << (unsigned char) count;

	for(int i = 0; i < MAX_PURCHASEITEM && i < count; i++)
	{
		msg << ctid[i];
	}
}

void BillCouponListReqMsg(CBPacket& msg, int userindex, const char* idname)
{
	CLCString id(51);
	id.Format("%s", idname);

	msg.Init(MSG_BILLITEM_COUPON_LIST_REQ);

	msg << id
		<< (unsigned char) CPID
		<< userindex;
}

void BillBuyByCouponRepMsg(CBPacket& msg, int userindex, const char* userid, int serial, int ctid[], const char* ip, char count, int couponID )
{
	msg.Init(MSG_BILLITEM_BUYBYCOUPON_REQ);
	msg.m_serial = serial;

	CLCString id(51);
	id.Format("%s", userid);
	CLCString lcIP(16);
	lcIP.Format("%s", ip);


	msg  << id
		 << (unsigned char) CPID
		 << userindex
		 << lcIP
		 << (unsigned char) count
		 << couponID;

	for(int i = 0; i < MAX_PURCHASEITEM && i < count; i++)
	{
		msg << ctid[i];
	}
}