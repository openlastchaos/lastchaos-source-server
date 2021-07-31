#ifdef USING_NPROTECT

#ifndef __NPROTECT_FUNC_H__
#define __NPROTECT_FUNC_H__

class CNProtectAuth
{
public:
	CCSAuth2*		m_pCSAuth;
	int				m_nTime;
	bool			m_bAnswer;

	CNProtectAuth();
	~CNProtectAuth();

	bool MakeGameGuardAuthQuery(CNetMsg& msg);
	bool CheckAuthMsg(CNetMsg& msg, char * buf);
	void SetNextAuthQueryTime(int nTime);
	bool CheckNextQueryTime();
};

bool InitNProtectGameGuard();
void ExNProtectMsg(CNetMsg& msg, GG_AUTH_DATA* pAuthData);

#endif // __NPROTECT_FUNC_H__

#endif // USING_NPROTECT
