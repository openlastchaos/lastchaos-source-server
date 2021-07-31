#ifndef __ADESCRIPTOR_H__
#define __ADESCRIPTOR_H__

#include "InputBuffer.h"
#include "InputQueue.h"
#include "OutputBuffer.h"

class CDescriptor  
{
public:
	CDescriptor();
	~CDescriptor();

	////////////
	// 계정 관련
	int			m_index;			// 유저 인덱스
	CLCString	m_idname;			// 유저 아이디
	CLCString	m_passwd;			// 암호

#ifdef CHECK_SECURE_CARD
	CLCString	m_cardstr;			// 보안 카드
	unsigned char	m_scard[4];			// 보안카드 번호 저장용 변수
#endif // CHECK_SECURE_CARD

	////////////
	// 서버군 관련
	int			m_serverNo;			// 서버군 번호
	int			m_nMaxServer;		// 서버군 별 최대 서브 서버 수

	int*		m_serverSubNo;		// 서브 번호
	int*		m_playerNum;		// 서버군별 사용자 수
	char**		m_ipAddr;			// 서버의 주소
	int*		m_portNumber;		// 서버의 포트 번호

	////////////
	// 소켓 관련
	socket_t	m_desc;						// file descriptor for socket
	CLCString	m_host;						// client ip
	CInputBuffer m_inBuf;					// Input Buffer
	CInputQueue m_inQ;						// Input Packet Queue
	COutputBuffer m_outBuf;					// Output Network Message Queue
#ifdef CRYPT_NET_MSG
	bool		m_bCryptNetMsg;
	CNM_KEY_TYPE m_nCryptKey;
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_KEY_TYPE m_nCryptKeyClient;			// 암호화에 사용되는 키값 : 클라이언트에서 보내오는 메시지 처리용
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG
	int			m_pulseConnected;			// 접속 당시 pulse

	//////////
	// 틱 관련
	int			m_timestamp;				// player: total playing time,  mob: last attacked pulse

	////////////
	// 상태 관련
	int			m_connected;				// CON_ 상수 값을 저장 : 접속 상태
	int			m_connectreq;				// 접속 요청 틱 카운트
	bool		m_bclosed;
	
	////////////
	// 링크 관련
	CDescriptor* m_pPrev;					// 리스트 링크
	CDescriptor* m_pNext;


	void CloseSocket();					// 소켓 닫기
	void FlushQueues();					// 큐 비우기

	void WriteToOutput(CNetMsg& msg);	// 쓰기
	int ProcessOutput();				// 쓰기 수행
	static int WriteToDescriptor(socket_t desc, const char* buf, int length);

	int ProcessInput();					// 읽기 수행
	static ssize_t PerformSocketRead(socket_t desc, char* read_point, size_t space_left);

	bool GetLogin(CNetMsg& msg);		// 버전 아이디 비번 검사

#ifdef CHECK_SECURE_CARD	
	bool IsCardUser(CLCString id);			// 보안 카드 검사
	bool GetSCard(CNetMsg & msg);
#endif // CHECK_SECURE_CARD
};

#endif
