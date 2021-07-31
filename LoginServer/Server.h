#ifndef __ASERVER_H__
#define __ASERVER_H__

#include "ConfigFile.h"
#include "Descriptor.h"

typedef struct _tagAutoBlock
{
	char	ip[HOST_LENGTH + 1];
	char	id[MAX_ID_NAME_LENGTH + 1];

	struct _tagAutoBlock* prev;
	struct _tagAutoBlock* next;
} AUTO_BLOCK_INFO;

class CServer
{
public:
	CServer();
	~CServer();

	////////////
	// 소켓 관련
	socket_t	m_ssock;			// 서버 소켓
	CDescriptor* m_desclist;		// 연결된 소켓 리스트

	CDescriptor* m_messenger;		// 메신저 연결

	fd_set		m_input_set;		// 디스크립터 셋
	fd_set		m_output_set;
	fd_set		m_exc_set;
	fd_set		m_null_set;

	// 컨넥터 관련
	CDescriptor** m_connector;		// 컨넥터 서버
	int			m_nConnector;		// 컨넥터 서버 수

	////////////
	// 서버 설정
	char*		m_serverpath;		// 모듈 경로
	CConfigFile	m_config;			// 서버 설정
	int			m_subno;			// 로그인 서버 서브 번호
	int			m_clientversionMin;	// 클라이언트 버전
	int			m_clientversionMax;	// 클라이언트 버전
	bool		m_bOnlyLocal;		// 외부 접속 허용 여부
	unsigned int m_nInternalIPFilter;	// 내부 접속용 IP 필터
	unsigned int m_nInternalSubnetMask;	// 내부 접속용 서브넷 마스크
	int			m_national;			// 국가 

	int			m_maxplayers;		// 허용 가능 최대 플레이어 수
	bool		m_bshutdown;		// 서버 종료
	bool		m_nameserverisslow; // 왜 쓰는 것일까?
	bool		m_breboot;			// 리부트
	TIMESTRUCT	m_serverTime;		// 서버 시간

#ifdef CRYPT_NET_MSG
	unsigned char pDecryptBuffer[CNM_TEMP_BUFFER_LENGTH];
	unsigned char pTmpDecryptBuffer[CNM_TEMP_BUFFER_LENGTH];
#endif

#ifdef IP_BLOCK_AUTO
	/////////////////
	// 자동 블럭 관련
	AUTO_BLOCK_INFO*	m_autoBlockTable;	// 블럭 테이블
	int					m_autoBlockPulse;	// 최근 리셋한 시간
#endif

#ifdef RECOMMEND_SERVER_SYSTEM
	int					m_nRecommendServer;
#endif // RECOMMEND_SERVER_SYSTEM

	//////////
	// 틱 관련
	int			m_pulse;			// 틱 카운터
	int			m_pulseServerTime;	// 서버시간
	int			m_resetPlayerTime;	// 컨넥터에 사용자 수 요청
	int*		m_checkConnector;	// 컨넥터 타임아웃 체크 틱
	int			m_pulseDBPing;		// DB에 연결을 유지하기 위한 변수

#if defined (WJKTEST) || defined (LC_USA)
	int			m_pulseSaveTableCount;
#endif

	//////////
	// DB 연결
	MYSQL		m_dbuser;			// 사용자 데이터
	MYSQL		m_dbdata;			// 데이터
	MYSQL		m_dbAuth;			// 2판4판 인증 유저 DB

#ifdef CHECK_SECURE_CARD
	MYSQL		m_dbScard;			// 보안카드
#endif // CHECK_SECURE_CARD

	bool		ConnectDB();
	void		DisconnectDB();

	////////////////////////////
	// 메신저 컨넥터 수행 플래그
	bool*		m_brunconn;			// 컨넥터 수행 플래그
	bool		m_brunmessenger;	// 메신저 수행 플래그


	/////////////////
	// 서버 환경 관련
	char* GetServerPath();			// 경로 얻기
	bool LoadSettingFile();			// 설정 파일 읽기
	bool InitGame();				// 게임 초기화
	int GetMaxPlayers();			// 허용 가능 최대 플레이어 수 구하기
	void SetServerTime();
	void CheckInternalIP(const char* strData);			// 서버 내부오픈 IP 파싱


	/////////////////
	// 소켓 설정 관련
	socket_t InitSocket();								// 서버 소켓 초기화
	static int SetSendbuf(socket_t s);					// 송신 버퍼 크기 설정
	struct in_addr* GetBindAddr();						// 반인드 할 주소 선택
	static void Nonblock(socket_t s);					// 비블럭 소켓


	/////////////////
	// 서버 수행 관련
	void Run();				// 서버 수행
	void Close();			// 수행 종료
	static void ServerSleep(struct timeval* timeout);	// 서버 잠시 재우기


	///////////////////////
	// 클라이언트 소켓 관련
	int NewDescriptor(int s);					// 새로운 클라이언트 받음
	void CloseSocket(CDescriptor* d);			// 디스크립터 종료
	void SendOutput(CDescriptor* d);			// 데이터 보내기
	bool DisConnectLogedIn(CDescriptor* my_d);	// 연결 끊어짐 처리

	////////////
	// 컨넥터 관련
	bool MakeConnectorClient(int i);				// 컨넥터 클라이언트 생성
	void SendConnectorHeader(int i);				// 컨넥터에 헤더 전달
	void SendtoConnector(CDescriptor* d, int i);	// 컨넥터에 메시지 전달
#ifdef CIRCLE_WINDOWS
	static void ConnectorConnect(void* i);			// 컨넥터 연결 시도
#else
	static void* ConnectorConnect(void* i);
#endif
	void ProcConnector(CNetMsg& msg);				// 컨넥터 메시지 처리
	void SendConnectorReqPlayer();					// 사용자수 전송

	//////////////
	// 메신저 관련
	bool MakeMessengerClient();				// 메신저 클라이언트 생성
	void SendMessengerHeader();				// 메신저 헤더 보냄
#ifdef CIRCLE_WINDOWS						// 메신저 클라이언트 생성 쓰레드
	static void MessengerConnect(void* lpvoid);
#else
	static void* MessengerConnect(void* lpvoid);
#endif
	void ProcMessenger(CNetMsg& msg);				// 메신저 메시지 처리

#if defined (WJKTEST) || defined (LC_USA)
	void WriteDBPlayerCount();
	void BlockIPLog( const char* user_ip );
#endif 
};

extern volatile LC_THREAD_T		gThreadIDGameThread;
extern volatile LC_THREAD_T		gThreadIDDBThread;

extern CServer gserver;

#endif
