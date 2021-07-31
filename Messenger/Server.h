#ifndef __ASERVER_H__
#define __ASERVER_H__

#include "ConfigFile.h"
#include "Descriptor.h"
#include "NetMsg.h"
#include "MsgList.h"

#ifdef RECOMMEND_SERVER_SYSTEM
#define FILENAME_RECOMMEND		".recommend"
#endif // RECOMMEND_SERVER_SYSTEM

#define FILENAME_GOLDENBALL		".goldenball"

typedef struct _tagGoldenBallData
{
	int		nStatus;
	char	strTeam1[GOLDENBALL_TEAM_NAME_LENGTH + 1];
	char	strTeam2[GOLDENBALL_TEAM_NAME_LENGTH + 1];
	int		nTeam1Score;
	int		nTeam2Score;
	int		nYear;
	int		nMonth;
	int		nDay;
	int		nHour;
	int		nMin;
	time_t	timeEndVote;
} GOLDEN_BALL_DATA;

class CServer
{
public:
	CServer();
	~CServer();

	////////////
	// 소켓 관련
	socket_t	m_ssock;			// 서버 소켓
	CDescriptor* m_desclist;		// 연결된 소켓 리스트
	int			m_nDesc;			// 리스트의 소켓 개수
	fd_set		m_input_set;		// 디스크립터 셋
	fd_set		m_output_set;
	fd_set		m_exc_set;
	fd_set		m_null_set;

	////////////////////////////
	// 메시지 리스트
	CMsgList	m_msgList;
	int			m_seqGen;

	////////////
	// 서버 설정
	char*		m_serverpath;		// 모듈 경로
	CConfigFile	m_config;			// 서버 설정

	int			m_maxplayers;		// 허용 가능 최대 플레이어 수
	bool		m_bshutdown;		// 서버 종료
	bool		m_nameserverisslow; // 왜 쓰는 것일까?
	bool		m_breboot;			// 리부트
	TIMESTRUCT	m_serverTime;		// 서버 시간

	//////////
	// 틱 관련
	int			m_pulse;			// 틱 카운터
	int			m_pulseServerTime;	// 서버 시간

#ifdef RECOMMEND_SERVER_SYSTEM
	int			m_nRecommendServer;	// 서버 추천 설정
#endif // RECOMMEND_SERVER_SYSTEM

	GOLDEN_BALL_DATA	m_sGoldenBallData;	// 골든볼 데이터


	///////////////////
	// 메시지 번호 관련
	int GetNextSeqNo() { return m_seqGen++; }


	/////////////////
	// 서버 환경 관련
	char* GetServerPath();			// 경로 얻기
	bool LoadSettingFile();			// 설정 파일 읽기
	bool InitGame();				// 게임 초기화
	int GetMaxPlayers();			// 허용 가능 최대 플레이어 수 구하기
	void SetServerTime();			// 서버 시간 설정 -> serverTime


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

	/////////////////
	// 명령 처리 관련
	void CommandInterpreter(CDescriptor* d, CNetMsg& msg); // 명렁 처리
	void ProcTimeout(CMsgListNode* msgNode);

	void SendToLoginServer(CNetMsg& msg);
};

extern volatile LC_THREAD_T		gThreadIDGameThread;
extern volatile LC_THREAD_T		gThreadIDDBThread;

extern CServer gserver;

#endif
