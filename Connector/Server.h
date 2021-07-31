#ifndef __ASERVER_H__
#define __ASERVER_H__

#include "ConfigFile.h"
#include "Descriptor.h"
#include "User.h"
#include "BillingClient.h"

/*
typedef struct __tagLimitCtSell
{
	int			ct_id;
	int			sell;
} LIMIT_CT_CELL;
*/

class CLimitSell
{
	int m_ctid;
	int m_sell;
	bool m_bChange;

public:
	CLimitSell(int ctid, int sell, bool change = true) { m_ctid = ctid; m_sell = sell; m_bChange = change; }

	int GetIndex() { return m_ctid; }
	int GetSell() { return m_sell; }
	bool IsChange() { if( m_bChange ) { m_bChange = false; return true; } return false; }
	void IncreaseSell() { m_sell++; m_bChange = true; }
	void moreSell(int sell)		{ m_sell = m_sell - sell; if( m_sell < 0 ) m_sell = 0; }

	static int CompByIndex(CLimitSell* p1, CLimitSell* p2)
	{
		return p1->GetIndex() - p2->GetIndex();
	}
};

class CServer
{
public:
	CServer();
	~CServer();

	////////////
	// 소켓 관련
	socket_t	m_ssock;			// 서버 소켓
	CDescriptor* m_desclist;		// 연결된 소켓 리스트
	fd_set		m_input_set;		// 디스크립터 셋
	fd_set		m_output_set;
	fd_set		m_exc_set;
	fd_set		m_null_set;

	////////////
	// 서버 설정
	char*		m_serverpath;		// 모듈 경로
	CConfigFile	m_config;			// 서버 설정
	CBillingClient m_billing;		// 빌링 클라이언트
	int			m_serverno;			// 서버 번호
	int			m_maxSubServer;		// 하위 서버 최대 개수
	int			m_maxplayers;		// 허용 가능 최대 플레이어 수
	bool		m_bshutdown;		// 서버 종료
	bool		m_breboot;			// 서버 정상 종료 여부
	bool		m_nameserverisslow; // 왜 쓰는 것일까?
	TIMESTRUCT	m_serverTime;		// 서버 시간
	int			m_serverAliveTime;	// 빌링서버로 2분동안 보내는 메시지가 없을때 보내는 얼라이브 타임
#ifdef LC_TIME
	bool		bStartLctime;		// 
	int			m_perSecond;		// 1초 검사용
	int			m_lcTime;			// 라카력 
	int			m_startTime;			// 라카력 시작시 타임
	struct timeval m_last_time, m_cur_time;
	int			m_oldLcTime;
#endif
#ifdef EXTERNAL_COUNT
	char		m_extTableList[EXTERNAL_COUNT_MAX][3];	// 외부 카운터 리스트 포인터 
	int			m_extTableCount;	// 외부 카운터 리스트의 갯수 
#endif
#ifdef LIMIT_CATALOG
	CLCList<CLimitSell*>	m_limitSell;
	bool		m_changeLimit;		// 한정판매를 5분마다 디비에 저장할것인가.
#endif


#ifdef EVENT_GOMDORI_2007
	int			m_nEventGomdori2007LastUpdate;		// 마지막 업데이트 시간 : 단위 분
	int			m_nEventGomdori2007LastRead;		// 마지막 읽은 시간 : 단위 분
	int			m_nEventGomdori2007Count;			// 리스트 수
	int*		m_nEventGomdori2007Status;			// 데이터
	int			m_nEventGomdori2007StatusCurServer;	// 현재 서버군의 누적 수
#endif // EVENT_GOMDORI_2007


#ifdef EVENT_FLOWERTREE_2007
	int				m_nEventFlowerTree2007LastUpdate;		// 마지막 업데이트 시간 : 단위 분
	int				m_nEventFlowerTree2007LastRead;			// 마지막 읽은 시간 : 단위 분
	int				m_nEventFlowerTree2007Count;			// 서버 리스트 갯수
	unsigned int*	m_nEventFlowerTree2007Status;			// 데이터
	unsigned int	m_nEventFlowerTree2007StatusCurServer;	// 현재 서버군의 누적 포인트
#endif // EVENT_FLOWERTREE_2007
	
#ifdef EVENT_XMAS_2007
	bool			m_bEventXmasTimerStart;				// 타이머 작동 여부
	int				m_nEventXmasTreeOldTime;			// 시간 차이를 구하기 위한 변수
	int				m_nEventXmasTreeNoticeTime;			// 트리 아이템 드랍 공지 시간 : 단위 분
	int				m_nEventXmasTreeLastUpdate;			// 마지막 업데이트 시간 : 단위 분
	int				m_nEventXmasTreeLastRead;			// 마지막 읽은 시간 : 단위 분
	unsigned int	m_unEventXmasTreePoint;				// 현재 서버의 나무 포인트
#endif //EVENT_XMAS_2007


	//////////
	// 틱 관련
	int			m_pulse;			// 틱 카운터
	int			m_pulseDBPing;		// DB 연결 유지를 위한 변수
	
	/////////////////
	// 통계 주기 관련
	time_t		m_pulseSaveCount;	// t_connect_count 갱신 시간
	time_t		m_pulseSaveMax;		// t_connect_max 갱신 시간

	//////////
	// DB 연결
	MYSQL		m_dbuser;			// 사용자 데이터
	MYSQL		m_dbauth;			// 사용자 인증 데이터
#if defined (EVENT_2PAN4PAN_GOODS) || defined (ATTENDANCE_EVENT) || defined ( ATTENDANCE_EVENT_REWARD ) || defined ( IRIS_POINT ) || defined (EVENT_NOM_MOVIE)
	MYSQL		m_dbevent;			// 이벤트 DB 연결
#endif // defined (EVENT_2PAN4PAN_GOODS) || defined (ATTENDANCE_EVENT) || defined ( IRIS_POINT )

#if defined ( CHAR_LOG ) && defined ( LC_KOR )
	MYSQL		m_dblog;			// 캐릭터 로그 DB
#endif // #if defined ( CHAR_LOG ) && defined ( LC_KOR )

	//////////////
	// User 리스트
	CUserList*	m_userList;			// 유저 리스트


	/////////////////
	// 서버 환경 관련
	char* GetServerPath();			// 경로 얻기
	bool LoadSettingFile();			// 설정 파일 읽기
	bool ConnectDB();				// DB 연결
	bool InitGame();				// 게임 초기화
	int GetMaxPlayers();			// 허용 가능 최대 플레이어 수 구하기
	LONGLONG GetServerTime();		// 현재 시간 대비 서버시간 설정


	/////////////////
	// 소켓 설정 관련
	socket_t InitSocket();								// 서버 소켓 초기화
	static int SetSendbuf(socket_t s);					// 송신 버퍼 크기 설정
	struct in_addr* GetBindAddr();						// 반인드 할 주소 선택
	static void Nonblock(socket_t s);					// 비블럭 소켓


	/////////////////
	// 서버 수행 관련
	void Run();											// 서버 수행
	void Close();										// 수행 종료
	static void ServerSleep(struct timeval* timeout);	// 서버 잠시 재우기
	void HeartBeat();									// 1초에 10번씩 매번 틱마다 수행되는 함수
	void CheckPlayers();								// 각 플레이어의 상태를 검사
	void SavePlayersCount();							// 플레이어 수를 데이터베이스에 저장


	///////////////////////
	// 클라이언트 소켓 관련
	int NewDescriptor(int s);					// 새로운 클라이언트 받음
	void CloseSocket(CDescriptor* d);			// 디스크립터 종료
	void SendOutput(CDescriptor* d);			// 데이터 보내기
	bool DisConnectLogedIn(CDescriptor* my_d);	// 연결 끊어짐 처리
	void SendAllGameServer(CNetMsg& msg);

#ifdef EVENT_XMAS_2007
	///////////////////////
	// 게임서버 와의 주기적 통신
	void TimerToGameServer();
#endif //EVENT_XMAS_2007


	/////////////////
	// 명령 처리 관련
	void CommandInterpreter(CDescriptor* d, CNetMsg& msg); // 명렁 처리

	void ProcessLogout(CUser* user);					// 로그아웃 처리
	void InitUserData();
};


extern volatile LC_THREAD_T		gThreadIDGameThread;
extern volatile LC_THREAD_T		gThreadIDDBThread;

extern CServer gserver;

#endif
