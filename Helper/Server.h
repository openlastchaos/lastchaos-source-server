#ifndef __ASERVER_H__
#define __ASERVER_H__

#include "Party.h"
#include "ConfigFile.h"
#include "Descriptor.h"
#include "NetMsg.h"
#include "MsgList.h"
#include "Guild.h"
//#include "Friend.h"
#include "TradeAgent.h"
#include "Expedition.h"


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
	MYSQL		m_dbchar;
	int			m_serverno;

	int			m_maxplayers;		// 허용 가능 최대 플레이어 수
	bool		m_nameserverisslow; // 왜 쓰는 것일까?
	TIMESTRUCT	m_serverTime;		// 서버 시간
	bool		m_bshutdown;		// 서버 종료

	//////////
	// 틱 관련
	int			m_pulse;			// 틱 카운터
	int			m_pulseServerTime;	// 서버 시간

	CGuildList	m_guildlist;

	int			m_nMoonStoneNas;

#ifdef GUILD_MARK_TABLE
	int			m_nGuildMarkTable[3];
#endif // GUILD_MARK_TABLE

	///////////////////
	// 전체 파티 리스트
	CLCList<CParty*>	m_listParty;	// 서버내 전체 파티 리스트

#ifdef EXPEDITION_RAID
	// 전체 원정대 리스트
	CLCList<CExpedition*>	m_listExped;	// 서버내 전체 원정대 리스트
#endif //EXPEDITION_RAID
	
#ifdef PARTY_MATCHING
	CLCList<CPartyMatchMember*>		m_listPartyMatchMember;
	CLCList<CPartyMatchParty*>		m_listPartyMatchParty;
#endif // PARTY_MATCHING

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
	bool ConnectDB();
	void DisconnectDB();
	bool LoadSettings();


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
	void SendToAllGameServer(CNetMsg& msg);
	void SendToAllSubServer(CNetMsg& msg, int nSubNo);

	/////////////////
	// 명령 처리 관련
	void CommandInterpreter(CDescriptor* d, CNetMsg& msg); // 명렁 처리
	void ProcTimeout(CMsgListNode* msgNode);

	void HeartBeat();

	bool DeleteGuild(int guildindex);
	bool DeleteGuildMember(int guildindex, const char* guildname, int charindex, const char* charname, bool bKick);

	// 파티 리스트 노드 찾기 : 보스 인덱스 사용
	CParty* FindPartyByBossIndex(int nSubNo, int nBossIndex);

	// 파티 리스트 노드 찾기 : 멤버 인덱스 사용
	CParty* FindPartyByMemberIndex(int nSubNo, int nMemberIndex, bool bIncludeRequest);

#ifdef EXPEDITION_RAID
	// 파티 리스트 노드 찾기 : 보스 인덱스 사용
	CExpedition* FindExpedByBossIndex(int nSubNo, int nBossIndex);

	// 파티 리스트 노드 찾기 : 멤버 인덱스 사용
	CExpedition* FindExpedByMemberIndex(int nSubNo, int nMemberIndex, bool bIncludeRequest);
#endif //EXPEDITION_RAID

#ifdef PARTY_MATCHING
	CPartyMatchMember* FindPartyMatchMemberByCharIndex(int nSubNo, int nCharIndex);
	CPartyMatchParty* FindPartyMatchPartyByBossIndex(int nSubNo, int nBossIndex);
#endif // PARTY_MATCHING

#ifdef TRADE_AGENT
	CTRADEAGENTLIST	m_TradeAgentList;		//거래 대행
#endif // TRADE_AGENT
	
	void SendGuildMemberList(CGuild* guild, CDescriptor* desc);
	void SendExtendGuildMemberList(CGuild* guild, CDescriptor* desc);
};

extern volatile LC_THREAD_T		gThreadIDGameThread;
extern volatile LC_THREAD_T		gThreadIDDBThread;

extern CServer gserver;

#endif
