#ifndef __ADESCRIPTOR_H__
#define __ADESCRIPTOR_H__

#include "InputBuffer.h"
#include "InputQueue.h"
#include "OutputBuffer.h"
#include "nProtectFunc.h"

#ifdef EVENT_2PAN4PAN_GOODS
typedef struct __tagEvent2pan4panGoodsData
{
	int			goodsindex;
	int			itemindex;
	LONGLONG	itemcount;
} EVENT_2PAN4PAN_GOODS_DATA;
#endif // #ifdef EVENT_2PAN4PAN_GOODS

class CDescriptor  
{
public:
	CDescriptor();

	~CDescriptor();

	////////////
	// 계정 관련
	int			m_index;						// 유저 인덱스
	CLCString	m_idname;						// 유저 아이디
	CLCString	m_passwd;						// 암호
	CLCString	m_ident;						// 주민번호 뒷자리
//	bool		m_bNovice;						//64시간 이내 초보자.//0627
	int			m_userFlag;
	bool		m_tryBug;						// 나스복사 시도?? : TODO : 테스트 이후 수정
//0704 
	int			m_notice[MAX_NOTICE];			//이벤트 공지 (최대 5개)

#ifdef LC_TLD
	//bool		m_bAdult;
	bool		m_bSendAdultNotice;
#endif
#ifdef NIGHT_SHADOW
	bool		m_bCreate90Lv;
#endif //NIGHT_SHADOW

#if defined(EVENT_PROMOTION_SITE)
	CLCString		m_proSite;
#endif
	
#if defined(EVENT_PCBANG) || defined(EVENT_PCBANG_2ND)
	int			m_location;
#endif



	//////////
	// DB 관련
	bool		m_bwaitdb;					// DB 통신을 위한 것
	int			m_updatedb;
	CInputQueue m_dbOut;					// DB output
	CMutexObject m_mutexobj;				// 동기화 개체

	////////////
	// 소켓 관련
	socket_t	m_desc;						// file descriptor for socket
	CLCString	m_host;						// client ip
	CInputBuffer m_inBuf;					// Input Buffer
	CInputQueue m_inQ;						// Input Packet Queue
	COutputBuffer m_outBuf;					// Output Network Message Queue
#ifdef CRYPT_NET_MSG
	bool		m_bCryptNetMsg;
	CNM_KEY_TYPE m_nCryptKey;				// 암호화에 사용되는 키값
#ifdef CRYPT_NET_MSG_MANUAL
	CNM_KEY_TYPE m_nOldCryptKey;			// 암호화에 사용되는 키값 : 이전 꺼
	int			m_pulseCryptKey;			// 최근 키값 설정한 시간
#else // CRYPT_NET_MSG_MANUAL
	CNM_KEY_TYPE m_nCryptKeyClient;			// 암호화에 사용되는 키값 : 클라이언트에서 보내오는 메시지 처리용
#endif // CRYPT_NET_MSG_MANUAL
	unsigned int m_nPrepareSeed;			// 처음 키를 생성하기 위한 Seed
#endif // #ifdef CRYPT_NET_MSG

#ifdef ENABLE_LOGOUT_DELAY
	int			m_logoutDelay;				// 게임 중 접속 종료 로그아웃 딜레이, -1이면 초기, 0이면 로그아웃, 그외는 대기
#endif // ENABLE_LOGOUT_DELAY

	//////////
	// 핵 관련
	int			m_hackCheckPulse;			// 최근 핵 검사 수행한 타임
	int			m_hackServerPulse;			// 서버 펄스		: 최초 캐릭터 선택에서 설정되고, 이후 한시간에 한번 갱신
	int			m_hackClientPulse;			// 클라이언트 펄스	:
	int			m_hackLastClientDiff;		// 최근 클라이언트의 펄스 간격
	int			m_hackCount;				// 핵 시도 회수

#ifdef USING_NPROTECT
	CNProtectAuth*	m_pGGAuth;				// nProtect 사용
#endif // USING_NPROTECT

	//////////
	// 틱 관련
	int			m_idletics;					// 입력없이 지나간 틱 수
	int			m_checktics;				// 1초당 명령 입력된 수를 리셋하기 위해 사용됨
	short int	m_commandcount;				// 1초당 명령 입력된 개수
	int			m_timestamp;				// player: total playing time,  mob: last attacked pulse
	int			m_laststamp;				// player: last saved pulse

	////////////
	// 상태 관련
	int			m_connected;				// CON_ 상수 값을 저장 : 접속 상태
	int			m_connectreq;				// 접속 요청 틱 카운트
	int			m_logined;					// 로그인 결과
	MSG_LOGIN_TYPE m_playmode;				// login mode or play mode
	bool		m_quitsave;					// when all data saved then set true
	bool		m_bclosed;
	bool		m_dbrunning;
	bool		m_bGoZone;					// 존 이동시에 true로 설정
	bool		m_bGoZoneSent;				// 존 이동 메시지 전달 여부
	CNetMsg		m_msgGoZone;				// 존 이동 메시지, m_bGoZoneSent를 false로 설정할 때 설정
	bool		m_waithelper;				// 헬퍼에 요청 결과 기다림
	int			m_helpertimeout;			// 헬퍼 대기시간

	////////////
	// 링크 관련
	CPC*		m_pChar;					// linked to char
	CDescriptor* m_pPrev;					// 리스트 링크
	CDescriptor* m_pNext;
	
	int m_helperWaitType;

	//////////////
	// 이벤트 관련
#ifdef EVENT_2PAN4PAN_GOODS
	int							m_nEvent2pan4panGoods;	// 2판4판 카드 이벤트 : 개수
	EVENT_2PAN4PAN_GOODS_DATA*	m_event2pan4panGoods;	// 2판4판 카드 이벤트 : 데이터
#endif // #ifdef EVENT_2PAN4PAN_GOODS

#ifdef EVENT_2PAN4PAN_BOX
	int							m_nEvent2pan4panBox;	// 2판4판 상자 이벤트 : 개수
	EVENT_2PAN4PAN_GOODS_DATA*	m_event2pan4panBox;		// 2판4판 상자 이벤트 : 데이터
#endif // #ifdef EVENT_2PAN4PAN_BOX

#ifdef HANARO_EVENT
	CLCString m_hanaroCode;
#endif


	void CloseSocket();					// 소켓 닫기
	void FlushQueues();					// 큐 비우기

	void WriteToOutput(CNetMsg& msg);	// 쓰기
	int ProcessOutput();				// 쓰기 수행
	static int WriteToDescriptor(socket_t desc, const char* buf, int length);

	int ProcessInput(int maxqueuesize);		// 읽기 수행
	static ssize_t PerformSocketRead(socket_t desc, char* read_point, size_t space_left);

	bool GetLogin(CNetMsg& msg);			// 버전 아이디 비번 검사

	bool StartGame(CNetMsg& msg);		// 게임 시작
	bool CharNew(CNetMsg& msg);			// 캐릭터 생성
	bool CharDel(CNetMsg& msg);			// 캐릭터 삭제

	//////////
	// 핵 관련
	void SetHackCheckPulse();			// 핵 검사 펄스 값을 현재 펄스 값으로 설정
	bool IsHackResponseTimeout();		// 클라이언트의 펄스 전송 타임아웃 검사
	bool CheckHackPulse(int clientPulse);	// 클라이언트의 펄스 값으로 핵펄스 검사
	bool IncreaseHackCount(int n);			// 핵 카운트 증가, 잦은 핵 시도시 접속 종료 처리

	void WaitHelperReply(bool bSet, int type = 0) { if (bSet) { m_waithelper = true; m_helpertimeout = 0; m_helperWaitType = type;} else { m_waithelper = false; m_helperWaitType = 0;} }
};

#endif
