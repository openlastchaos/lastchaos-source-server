#include "stdhdrs.h"
#include "Server.h"
#include "DBCmd.h"
#include "Guild.h"
#include "CmdMsg.h"
#include "doFunc.h"
//#include "Friend.h"

void FailMsg(CNetMsg& msg, MSG_FAIL_TYPE failtype)
{
	msg.Init(MSG_FAIL);
	msg << (unsigned char)failtype;
}

void HelperWhisperNotfoundMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sidx, const char* sname)
{
	msg.Init(MSG_HELPER_REP);
	msg	<< seq
		<< server << subno << zone
		<< (unsigned char)MSG_HELPER_WHISPER_NOTFOUND
		<< sidx << sname;
}

void HelperWhisperRepMsg(CNetMsg& msg, int seq, int server, int subno, int zone, int sidx, const char* sname, const char* rname, const char* chat)
{
	msg.Init(MSG_HELPER_REP);
	msg	<< seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_HELPER_WHISPER_REP
		<< sidx << sname
		<< rname
		<< chat;
}

void HelperGuildCreateRepMsg(CNetMsg& msg, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_CREATE_REP
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildCreateNotifyMsg(CNetMsg& msg, CGuild* guild)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_CREATE_NTF
		<< guild->index()
		<< guild->level()
		<< guild->name()
		<< guild->boss()->charindex()
		<< guild->boss()->GetName();
}

void HelperGuildOnlineNotifyMsg(CNetMsg& msg, CGuildMember* member)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_ONLINE_NTF
		<< member->guild()->index()
		<< member->charindex()
		<< member->online();
}

void HelperGuildMemberListMsg(CNetMsg& msg, CGuild* guild, int startidx, int cutCount )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_MEMBER_LIST
		<< guild->index();

	int loopCount = startidx+cutCount<=guild->membercount()?cutCount:guild->membercount()%cutCount;
	msg	<< loopCount;

	int i;
	CGuildMember* member;
	for (i = startidx; i< startidx+loopCount ; i++)
	{
		member = guild->member(i);
		if (member)
		{
			msg << member->charindex()
				<< member->GetName()
				<< member->pos()
				<< member->online();
		}
	}
}

#ifdef GUILD_MARK_TABLE
void HelperGuildMarkTableMsg(CNetMsg & msg)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_MARK_TABLE;
	for(int i = 0; i < 3 ; i++)
	{
		msg << gserver.m_nGuildMarkTable[i];
	};
}
#endif // GUILD_MARK_TABLE

void HelperGuildLoadRepMsg(CNetMsg& msg, const char* idname, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_LOAD_REP
		<< idname
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildLevelUpRepMsg(CNetMsg& msg, int guildindex, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_LEVELUP_REP
		<< guildindex
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildLevelUpNotifyMsg(CNetMsg& msg, int guildindex, int level)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_LEVELUP_NTF
		<< guildindex
		<< level;
}

void HelperGuildBreakUpRepMsg(CNetMsg& msg, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_BREAKUP_REP
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildBreakUpNotifyMsg(CNetMsg& msg, int guildindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_BREAKUP_NTF
		<< guildindex;
}

void HelperGuildMemberAddRepMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_MEMBER_ADD_REP
		<< guildindex
		<< bossindex
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildMemberAddNotifyMsg(CNetMsg& msg, int guildindex, int charindex, const char* name, MSG_GUILD_POSITION_TYPE pos)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_MEMBER_ADD_NTF
		<< guildindex
		<< charindex
		<< name
		<< pos;
}

void HelperGuildMemberOutRepMsg(CNetMsg& msg, int guildindex, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_MEMBER_OUT_REP
		<< guildindex
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildMemberOutNotifyMsg(CNetMsg& msg, int guildindex, int charindex, const char* charname)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_MEMBER_OUT_NTF
		<< guildindex
		<< charindex
		<< charname;
}

void HelperGuildMemberKickRepMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_MEMBER_KICK_REP
		<< guildindex
		<< bossindex
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildMemberKickNotifyMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_MEMBER_KICK_NTF
		<< guildindex
		<< bossindex
		<< charindex;
}

void HelperGuildChangeBossRepMsg(CNetMsg& msg, int guildindex, int current, int change, MSG_GUILD_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_CHANGE_BOSS_REP
		<< guildindex
		<< current
		<< change
		<< (unsigned char)errcode;
}

void HelperGuildChangeBossNotifyMsg(CNetMsg& msg, int guildindex, int current, int change)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_CHANGE_BOSS_NTF
		<< guildindex
		<< current
		<< change;
}

void HelperGuildAppointOfficerRepMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_APPOINT_OFFICER_REP
		<< guildindex
		<< bossindex
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildAppointOfficerNotifyMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_APPOINT_OFFICER_NTF
		<< guildindex
		<< bossindex
		<< charindex;
}

void HelperGuildFireOfficerRepMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_FIRE_OFFICER_REP
		<< guildindex
		<< bossindex
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildFireOfficerNotifyMsg(CNetMsg& msg, int guildindex, int bossindex, int charindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_FIRE_OFFICER_NTF
		<< guildindex
		<< bossindex
		<< charindex;
}

void HelperGuildLoadNotifyMsg(CNetMsg& msg, CGuild* guild)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_LOAD_NTF
		<< guild->index()
		<< guild->level()
		<< guild->name()
		<< guild->boss()->charindex()
		<< guild->boss()->GetName()
		<< guild->battleIndex()
		<< guild->battlePrize()
		<< guild->battleTime() / 2
		<< guild->battleZone()
		<< guild->killCount()
		<< guild->battleState();
#ifdef NEW_GUILD
	msg << guild->maxmember();
#endif // NEW_GUILD
}

void HelperGuildBattleRepMsg(CNetMsg& msg, CGuild* g1, CGuild* g2, int prize)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_GUILD_BATTLE_REP
		<< g1->index()
		<< g1->name()
		<< g2->index()
		<< g2->name()
		<< prize
		<< g1->battleZone()
		<< g1->battleTime() / 2;
}

void HelperGuildBattleStartMsg(CNetMsg& msg, CGuild* g1, CGuild* g2)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_GUILD_BATTLE_START
		<< g1->index()
		<< g1->name()
		<< g2->index()
		<< g2->name()
		<< g1->battlePrize()
		<< g1->battleZone()
		<< g1->battleTime() / 2;
}

void HelperGuildBattleStopRepMsg(CNetMsg& msg, int winner_index, CGuild* g1, CGuild* g2)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_GUILD_BATTLE_STOP_REP
		<< winner_index
		<< g1->index()
		<< g1->name()
		<< g2->index()
		<< g2->name()
		<< g1->battlePrize()
		<< g1->battleZone();
}

void HelperGuildBattleStatusMsg(CNetMsg& msg, CGuild* g1, CGuild* g2)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_GUILD_BATTLE_STATUS
		<< g1->index()
		<< g1->name()
		<< g1->killCount()
		<< g2->index()
		<< g2->name()
		<< g2->killCount()
		<< g2->battleTime() / 2
		<< g2->battleZone();
}

void HelperGuildBattlePeaceRepMsg(CNetMsg& msg, CGuild* g)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_GUILD_BATTLE_PEACE_REP
		<< g->index();
}

void HelperEventMoonStoneLoadMsg(CNetMsg& msg, int moonstone)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_EVENT_MOONSTONE_LOAD
		<< moonstone;
}

void HelperEventMoonStoneUpdateRepMsg(CNetMsg& msg, int moonstone)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_EVENT_MOONSTONE_UPDATE_REP
		<< moonstone;
}

void HelperEventMoonStoneJackPotRepMsg(CNetMsg& msg, int moonstone, int chaindex)
{
	msg.Init(MSG_HELPER_COMMAND);

	msg << MSG_HELPER_EVENT_MOONSTONE_JACKPOT_REP
		<< moonstone
		<< chaindex;
}

void HelperFriendMemberAddRepMsg(CNetMsg& msg, int approvalindex, const char* appname, int appjob, int requestindex, const char* reqname, int reqjob, MSG_FRIEND_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_FRIEND_MEMBER_ADD_REP
		<< approvalindex
		<< appname
		<< appjob
		<< requestindex
		<< reqname
		<< reqjob
		<< (unsigned char)errcode;
}

void HelperFriendMemberDelRepMsg(CNetMsg& msg, int removerindex, int deleteindex, MSG_FRIEND_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_FRIEND_MEMBER_DEL_REP
		<< removerindex
		<< deleteindex		
		<< (unsigned char)errcode;
}

#ifdef MESSENGER_NEW
void HelperBlockCharRepMsg(CNetMsg& msg, int reqindex, int blockIndex, const char* blockName )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_BLOCKPC_REP
		<< reqindex
		<< blockIndex
		<< blockName;
}
#endif
#ifdef CASH_ITEM_GIFT
void HelperGiftCharRepMsg(CNetMsg& msg, int sendUserIdx, int sendCharIdx, int recvUserIdx, int recvCharIdx, const char* recvCharName, const char* sendMsg, int count, int idx[], int ctid[] )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GIFT_RECVCHARNAME_REP
		<< sendUserIdx
		<< sendCharIdx
		<< recvUserIdx
		<< recvCharIdx
		<< recvCharName
		<< sendMsg
		<< count;

	for(int i = 0; i < count; i++)
	{
		msg << idx[i]
			<< ctid[i];
	}
}
#endif

void HelperFriendSetConditionNotifyMsg(CNetMsg& msg, int chaindex, int condition, int reply, int sum, const int* index)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_FRIEND_SET_CONDITION_NOTIFY
		<< chaindex
		<< condition
		<< reply;

	if(reply == -1)
	{
		msg	<< sum;
		for(int i=0 ; i<sum ; i++)
		{
			msg << index[i];
		}
	}			
}

void HelperNameChangeRepMsg(CNetMsg& msg, char bguild, int charindex, const char* name, MSG_EX_NAMECHANGE_ERROR_TYPE error)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_NAME_CHANGE_REP
		<< bguild
		<< charindex
		<< name
		<< (unsigned char) error;
}

void HelperPetCreateRepMsg(CNetMsg& msg, int index, int owner, char typegrade)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PET_CREATE_REP
		<< index
		<< owner
		<< typegrade;
}

#ifdef NEW_UI
void HelperPetDeleteRepMsg(CNetMsg& msg, int index, int owner)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PET_DELETE_REP
		<< index
		<< owner;
}
#endif // NEW_UI

void HelperGuildStashHistoryRepMsg(CNetMsg& msg, int charindex, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, int month[7], int day[7], LONGLONG money[7])
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_STASH_HISTORY_REP
		<< charindex
		<< errcode;
	int i;
	for (i = 0; i < 7; i++)
	{
		msg << month[i]
			<< day[i]
			<< money[i];
	}
}

void HelperGuildStashViewRepMsg(CNetMsg& msg, int charindex, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, LONGLONG money)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_STASH_VIEW_REP
		<< charindex
		<< errcode
		<< money;
}

void HelperGuildStashTakeRepMsg(CNetMsg& msg, int guildindex, int charindex, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, LONGLONG money, LONGLONG balance)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_STASH_TAKE_REP
		<< guildindex
		<< charindex
		<< errcode
		<< money
		<< balance;
}

void HelperGuildStashSaveTaxRepMsg(CNetMsg& msg, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, int guildindex, int zoneindex, LONGLONG taxItem, LONGLONG taxProduce)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_GUILD_STASH_SAVE_TAX_REP
		<< errcode
		<< guildindex
		<< zoneindex
		<< taxItem
		<< taxProduce;
}

void HelperFameupRepMsg(CNetMsg& msg, int teachidx, int studentidx, const char* studentname, int fameup)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_TEACHER_FAMEUP_REP
		<< teachidx
		<< studentidx
		<< studentname
		<< fameup;
}

void HelperTeacherInfo(CNetMsg& rmsg, const char* idname, int charindex, char m_teachType, int m_fame, char bteacher, int m_cntCompleteStudent, 
		int m_cntFailStudent, int m_superstone, int m_guardian, const char* strTeachIdx, const char* strTeachSec)
{
	int m_teachIdx[TEACH_MAX_STUDENTS];
	char		m_teachJob[TEACH_MAX_STUDENTS];
#ifdef ENABLE_CHANGEJOB
	char		m_teachJob2[TEACH_MAX_STUDENTS];
#endif
	int		m_teachLevel[TEACH_MAX_STUDENTS];
	char	m_teachName[TEACH_MAX_STUDENTS][MAX_CHAR_NAME_LENGTH + 1];
	int		m_teachTime[TEACH_MAX_STUDENTS];
	int		m_cntTeachingStudent = 0;
	char	tbuf[255];

	memset(m_teachIdx, -1, sizeof(m_teachIdx) );
	memset(m_teachTime, 0, sizeof(m_teachTime) );

	if( m_teachType == MSG_TEACH_TEACHER_TYPE)
	{
		//strTeachIdx[0] = '\0';
		const char* pTeach = strTeachIdx;

		int cnt =0;
		int count = 0;
		while (*pTeach)
		{
			int idx;
			pTeach = AnyOneArg(pTeach, tbuf);
			idx = atoi(tbuf);

			if (idx == -1)
			{
				m_teachIdx[cnt] = -1;
				m_teachJob[cnt] = -1;
#ifdef ENABLE_CHANGEJOB
				m_teachJob2[cnt] = 0;
#endif
				m_teachLevel[cnt] = 0;
				m_teachName[cnt][0] = '\0';
				m_teachTime[cnt] = 0;
			}
			else
			{
				
				CDBCmd dbTmp;
				dbTmp.Init(&gserver.m_dbchar);

				CLCString sql(1024);

#ifdef ENABLE_CHANGEJOB
				sql.Format("SELECT a_index, a_name, a_nick, a_level, a_job, a_job2 FROM t_characters WHERE a_enable = 1 and a_index=%d", idx);
#else
				sql.Format("SELECT a_index, a_name, a_nick, a_level, a_job FROM t_characters WHERE a_enable = 1 and a_index=%d", idx);
#endif

				dbTmp.SetQuery(sql);
				if (dbTmp.Open() && dbTmp.MoveFirst())
				{
					int teach_index;
					int teach_level;
					CLCString teach_name(MAX_CHAR_NAME_LENGTH + 1);
					CLCString teach_nick(MAX_CHAR_NAME_LENGTH + 1);
					char teach_job;
#ifdef ENABLE_CHANGEJOB
					char teach_job2;
#endif

					dbTmp.GetRec("a_index",		teach_index);
					dbTmp.GetRec("a_name",		teach_name);
					dbTmp.GetRec("a_nick",		teach_nick);
					dbTmp.GetRec("a_level",		teach_level);
					dbTmp.GetRec("a_job",		teach_job);
#ifdef ENABLE_CHANGEJOB
					dbTmp.GetRec("a_job2",		teach_job2);
#endif
					m_teachIdx[cnt] = teach_index;
					m_teachJob[cnt] = teach_job;
#ifdef ENABLE_CHANGEJOB
					m_teachJob2[cnt] = teach_job2;
#endif
					m_teachLevel[cnt] = teach_level;
					strcpy(m_teachName[cnt], teach_nick);
					count++;
				}
				else
				{
					// 학생 케릭이 삭제되었을 경우. 캔슬
					CDBCmd tempCmd;
					tempCmd.Init(&gserver.m_dbchar);

					tempCmd.BeginTrans();
					do_TeachCancel(tempCmd, charindex, idx, CANCELTEACHER);
					tempCmd.Commit();
				}

			}

			cnt++;
		}

		m_cntTeachingStudent = count;

		//strTeachSec[0] = '\0';
		const char* pTeachSec = strTeachSec;
		cnt = 0;
		while (*pTeachSec)
		{
			int sec;
			pTeachSec = AnyOneArg(pTeachSec, tbuf);
			sec = atoi(tbuf);
			m_teachTime[cnt++] = sec;
		}

		// 사제 시스템을 읽고 난후 메세지를 보낸다.
		rmsg.Init(MSG_HELPER_COMMAND);

		rmsg << MSG_HELPER_TEACHER_LOAD_REP
			 << idname
			 << charindex
			 << m_cntTeachingStudent
			 << m_guardian
			 << m_superstone
			 << m_cntFailStudent
			 << m_cntCompleteStudent
			 << m_teachType
			 << m_fame
			 << bteacher;

		for(int i = 0; i < TEACH_MAX_STUDENTS; i++)
		{
			rmsg << m_teachIdx[i]
				 << m_teachJob[i]
	#ifdef ENABLE_CHANGEJOB
				 << m_teachJob2[i]
	#endif
				 << m_teachLevel[i]
				 << m_teachName[i]
				 << m_teachTime[i];
		}
	}
	else // 학생
	{
		const char* pTeach = strTeachIdx;

		int idx;
		pTeach = AnyOneArg(pTeach, tbuf);
		idx = atoi(tbuf);

		CDBCmd dbTmp;
		dbTmp.Init(&gserver.m_dbchar);

		CLCString sql(1024);

#ifdef ENABLE_CHANGEJOB
		sql.Format("SELECT a_index, a_name, a_nick, a_level, a_job, a_job2, a_teach_idx, a_fame, a_teach_complete, a_teach_fail "
			"FROM t_characters WHERE a_enable  = 1 and a_index=%d", idx);
#else
		sql.Format("SELECT a_index, a_name, a_nick, a_level, a_job, a_teach_idx, a_fame, a_teach_complete, a_teach_fail "
			"FROM t_characters WHERE a_enable  = 1 and a_index=%d", idx);
#endif

		dbTmp.SetQuery(sql);
		if (dbTmp.Open() && dbTmp.MoveFirst())
		{
			CLCString teachName(MAX_CHAR_NAME_LENGTH + 1);
			CLCString teachIdx(256);
			dbTmp.GetRec("a_index", m_teachIdx[0]);
			//dbTmp.GetRec("a_name", m_teachName[0]);
			dbTmp.GetRec("a_nick", teachName);
			strcpy(m_teachName[0], teachName);
			dbTmp.GetRec("a_level", m_teachLevel[0]);
			dbTmp.GetRec("a_job", m_teachJob[0]);
#ifdef ENABLE_CHANGEJOB
			dbTmp.GetRec("a_job2", m_teachJob2[0]);
#endif
			// 자신이 학생이면 idx[1]을 선생의 명성수치로, idx[2]를 선생의 양성중이 초보로, time[1]을 완료인원, time[2]을 실패인원으로 쓴다.
			dbTmp.GetRec("a_teach_idx", teachIdx);
			dbTmp.GetRec("a_fame", m_teachIdx[1]);
			dbTmp.GetRec("a_teach_complete", m_teachTime[1]);
			dbTmp.GetRec("a_teach_fail", m_teachTime[2]);

			const char* pTemp = teachIdx;

			m_teachIdx[2] = 0;
			while(*pTemp)
			{
				int idx;
				pTemp = AnyOneArg(pTemp, tbuf);
				idx = atoi(tbuf);
				
				if( idx != -1 )
				{
					m_teachIdx[2]++;
				}
			}
		}
		else
		{
			// 선생케릭이 삭제되었을 경우 캔슬.
			CDBCmd tempCmd;
			tempCmd.Init(&gserver.m_dbchar);

			tempCmd.BeginTrans();
			do_TeachCancel(tempCmd, idx, charindex, CANCELSTUDENT);
			tempCmd.Commit();
		}

		const char* pTeachSec = strTeachSec;
		
		int sec;
		pTeachSec = AnyOneArg(pTeachSec, tbuf);
		sec = atoi(tbuf);
		m_teachTime[0] = sec;

		rmsg.Init(MSG_HELPER_COMMAND);

		rmsg << MSG_HELPER_TEACHER_LOAD_REP
			 << idname
			 << charindex
			 << m_cntTeachingStudent
			 << m_guardian
			 << m_superstone
			 << m_cntFailStudent
			 << m_cntCompleteStudent
			 << m_teachType
			 << m_fame
			 << bteacher;

		rmsg << m_teachIdx[0]
			 << m_teachJob[0]
#ifdef ENABLE_CHANGEJOB
			 << m_teachJob2[0]
#endif
			 << m_teachLevel[0]
			 << m_teachName[0]
			 << m_teachTime[0];

		// 자신이 학생이면 idx[1]을 선생의 명성수치로, idx[2]를 선생의 양성중이 초보로, time[1]을 완료인원, time[2]을 실패인원으로 쓴다.
		rmsg << m_teachIdx[1]
			 << m_teachIdx[2]
			 << m_teachTime[1]
			 << m_teachTime[2];

	}

}
//휴면케릭 응답 보내는 부분.
void HelperEventSearchFriendMemberAddRepMsg(CNetMsg& msg
			, int approvalindex, 
			const char reqnick[MAX_CHAR_NAME_LENGTH + 1], MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_SEARCHFRIEND_ADD_REP
		<< approvalindex
		<< reqnick
		<< (unsigned char)errcode;
}

//휴면케릭이 리스트에서 선택한 결과를 보내는 부분
void HelperEventSearchFriendMemberSelectAddRepMsg(CNetMsg& msg
			, int approvalindex, MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_SEARCHFRIEND_SELECT_ADD_REP
		<< approvalindex
		<< (unsigned char)errcode;
}

//휴면케릭이 한시간 단위로 사냥한 시간을 저장하고 상태를 GameServer로 보냄.
void HelperEventSearchFriendMemberOneTimeCheckRepMsg(CNetMsg& msg
			, int timesec, int appDormantindex, MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_SEARCHFRIEND_ONETIME_ADD_REP
		<< timesec
		<< appDormantindex 		
		<< (unsigned char)errcode;
}

//휴면케릭 이벤트 등록된 친구 보상가능 여부 데이터를 GameServer로 보냄.
void HelperEventSearchFriendMemberListGoodRepMsg(CNetMsg& msg, int approvalindex, int* requestindex, const char requestnick[][MAX_CHAR_NAME_LENGTH + 1], int* itemgood, int request_listmember, int startindex, int nTotal)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_SEARCHFRIEND_LISTGOOD_REP
		<< approvalindex
		<< request_listmember
		<< startindex
		<< nTotal;
	
	for (int i = 0; i < request_listmember; i++)
	{
		msg << requestindex[i]
			<< requestnick[i]
			<< itemgood[i];
	}
}

//휴면케릭 이벤트 등록된 친구 보상 결과 데이터를 GameServer로 보냄.
void HelperEventSearchFriendMemberGoodRepMsg(CNetMsg& msg, int approvalindex, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TYPE errcode)
{

	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_SEARCHFRIEND_GOOD_REP
		<< approvalindex
		<< (unsigned char)errcode;
}

void HelperPartyInviteReqMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nTargetIndex, char nPartyType)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_INVITE_REQ
		<< nBossIndex
		<< strBossName
		<< nBossLevel
		<< nTargetIndex
		<< nPartyType;
}

void HelperPartyInviteRepMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nTargetIndex, const char* strTargetName, char nPartyType, MSG_HELPER_PARTY_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_INVITE_REP
		<< nBossIndex
		<< strBossName
		<< nTargetIndex
		<< strTargetName
		<< nPartyType
		<< errcode;
}

void HelperPartyAllowRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, const char* strTargetName, MSG_HELPER_PARTY_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_ALLOW_REP
		<< nBossIndex
		<< nTargetIndex
		<< strTargetName
		<< errcode;
}

void HelperPartyRejectRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, MSG_HELPER_PARTY_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_REJECT_REP
		<< nBossIndex
		<< nTargetIndex
		<< errcode;
}

void HelperPartyQuitRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, MSG_HELPER_PARTY_ERROR_TYPE errcode, int nNewBossIndex, const char* strNewBossName)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_QUIT_REP
		<< nBossIndex
		<< nTargetIndex
		<< errcode
		<< nNewBossIndex
		<< strNewBossName;
}

void HelperPartyKickRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, MSG_HELPER_PARTY_ERROR_TYPE errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_KICK_REP
		<< nBossIndex
		<< nTargetIndex
		<< errcode;
}

void HelperPartyChangeBossRepMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nNewBossIndex, const char* strNewBossName)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_CHANGE_BOSS_REP
		<< nBossIndex
		<< strBossName
		<< nNewBossIndex
		<< strNewBossName;
}

#ifdef NEW_UI
void HelperPartyChangeTypeRepMsg(CNetMsg& msg, int nBossIndex, char cPartyType, char cDiviType)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_CHANGE_TYPE_REP
		<< nBossIndex
		<< cPartyType
		<< cDiviType;
}
#endif // NEW_UI

#ifdef PARTY_RAID
void HelperPartyEndPartyRepMsg(CNetMsg& msg, int nBossIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_END_PARTY_REP
		<< nBossIndex;
}
#endif //PARTY_RAID

#ifdef PARTY_MATCHING
void HelperPartyMatchRegMemberRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nCharIndex, const char* strCharName, int nCharLevel, int nZone, char cJob, char cPartyType)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_REG_MEMBER_REP
		<< nErrorCode
		<< nCharIndex
		<< strCharName
		<< nCharLevel
		<< nZone
		<< cJob
		<< cPartyType;
}

void HelperPartyMatchRegPartyRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, const char* strBossName, int nBossLevel, int nZone, int nJobFlag, char cLimitLevel, char cPartyType, const char* strComment)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_REG_PARTY_REP
		<< nErrorCode
		<< nBossIndex
		<< strBossName
		<< nBossLevel
		<< nZone
		<< nJobFlag
		<< cLimitLevel
		<< cPartyType;
	if (nErrorCode == MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_OK)
		msg << strComment;
	else
		msg << "";
}

void HelperPartyMatchDelRepMsg(CNetMsg& msg, int nCharIndex, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_DEL_REP
		<< nCharIndex
		<< nErrorCode;
}

void HelperPartyMatchInviteRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName, char cPartyType)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_INVITE_REP
		<< nErrorCode
		<< nBossIndex
		<< strBossName
		<< nCharIndex
		<< strCharName
		<< cPartyType;
}

void HelperPartyMatchInviteReqMsg(CNetMsg& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nCharIndex, char cPartyType)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_INVITE_REQ
		<< nBossIndex
		<< strBossName
		<< nBossLevel
		<< nCharIndex
		<< cPartyType;
}

void HelperPartyMatchJoinRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, char cPartyType, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName, char cCharJob)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_JOIN_REP
		<< nErrorCode
		<< cPartyType
		<< nBossIndex
		<< strBossName
		<< nCharIndex
		<< strCharName
		<< cCharJob;
}

void HelperPartyMatchJoinAllowRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, int nCharIndex, const char* strCharName)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_JOIN_ALLOW_REP
		<< nErrorCode
		<< nBossIndex
		<< nCharIndex
		<< strCharName;
}

void HelperPartyMatchJoinRejectRepMsg(CNetMsg& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nJoinCharIndex, int nRejectCharIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_JOIN_REJECT_REP
		<< nErrorCode
		<< nJoinCharIndex
		<< nRejectCharIndex;
}

void HelperPartyMatchMemberChangeInfoMsg(CNetMsg& msg, int nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_TYPE nType, const char* strCharName, int nLevel, int nZone)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO
		<< nCharIndex
		<< nType;

	switch (nType)
	{
	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME:
		msg << strCharName;
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL:
		msg << nLevel;
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE:
		msg << nLevel
			<< nZone;
		break;
	}
}

#endif // PARTY_MATCHING

void HelperPartyInfoEndMsg(CNetMsg& msg)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_INFO_END;
}

void HelperPartyInfoPartyMsg(CNetMsg& msg, const CParty* pParty)
{
	if (pParty == NULL)
	{
		msg.Init(MSG_UNKNOWN);
		return ;
	}

	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_INFO_PARTY
		<< pParty->GetPartyType(MSG_DIVITYPE_EXP)
		<< pParty->GetRequestIndex()
		<< pParty->GetRequestName()
		<< pParty->GetMemberCount();
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		const CPartyMember* pMember = pParty->GetMemberByListIndex(i);
		if (pMember)
		{
			msg << pMember->GetCharIndex()
				<< pMember->GetCharName();
		}
	}
}

#ifdef PARTY_MATCHING
void HelperPartyInfoPartyMatchMemberMsg(CNetMsg& msg, const CPartyMatchMember* pMatchMember)
{
	if (pMatchMember == NULL)
	{
		msg.Init(MSG_UNKNOWN);
		return ;
	}

	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_INFO_PARTY_MATCH_MEMBER
		<< pMatchMember->GetIndex()
		<< pMatchMember->GetName()
		<< pMatchMember->GetLevel()
		<< pMatchMember->GetZone()
		<< pMatchMember->GetPartyType();
}

void HelperPartyInfoPartyMatchPartyMsg(CNetMsg& msg, const CPartyMatchParty* pMatchParty)
{
	if (pMatchParty == NULL)
	{
		msg.Init(MSG_UNKNOWN);
		return ;
	}

	char cLimitLevel = (pMatchParty->IsLimitLevel()) ? 1 : 0;

	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARTY_INFO_PARTY_MATCH_PARTY
		<< pMatchParty->GetBossIndex()
		<< pMatchParty->GetBossName()
		<< pMatchParty->GetBossLevel()
		<< pMatchParty->GetZone()
		<< pMatchParty->GetJobFlag()
		<< pMatchParty->GetPartyType()
		<< cLimitLevel
		<< pMatchParty->GetComment();
}
#endif // PARTY_MATCHING

#ifdef EVENT_CHILDRENSDAY_2007
void HelperChildrensDay2007Msg( CNetMsg& msg, int nCharIndex, int nItemIndex, int nNeedFlower, char cResult )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_CHILDRENSDAY_2007
		<< nCharIndex
		<< nItemIndex
		<< nNeedFlower
		<< cResult;
}

#endif //EVENT_CHILDRENSDAY_2007

#ifdef EVENT_2007_PARENTSDAY
void HelperEventAccmulatepointList( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE errorType, int CharIndex, int count,  int* Ranking, int* Accumulatepoint, const char GuildName[][MAX_CHAR_NAME_LENGTH+1] )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST
		<< errorType
		<< CharIndex
		<< count;
	for( int i = 0;  i < count; i++ )
	{
	msg	<< Ranking[i]
		<< Accumulatepoint[i]
		<< GuildName[i];
	}
}
void HelperEventParentsdayAddPoint( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE errorType, int CharIndex, int CarnationCount, int AccumulatePoint )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARENTSDAY_2007_ADD_POINT
		<< errorType
		<< CharIndex
		<< CarnationCount
		<< AccumulatePoint;
}
void HelperEventParentsdayExchangeTicket( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE errorType, int CharIndex, int GiftTicket )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_PARENTSDAY_2007_EXCHANGE_TICKET
		<< errorType
		<< CharIndex
		<< GiftTicket;
}

void HelperEventParentsdayExchangeItem( CNetMsg& msg, MSG_PARENTSDAY_2007_ERROR_TYPE errorType, int CharIndex, int GiftItemIndex, int GiftItemCount )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_PARENTSDAY_2007_EXCHANGE_ITEM
		<< errorType
		<< CharIndex
		<< GiftItemIndex
		<< GiftItemCount;
}

void HelperEventParentsdayGet( CNetMsg& msg, int CharIndex , int GiftItemIndex, int GiftItemCount )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_PARENTSDAY_2007_NOTICE
		<< CharIndex
		<< GiftItemIndex
		<< GiftItemCount;
}

#endif // EVENT_2007_PARENTSDAY


#ifdef EVENT_TEACH_2007
void HelperTeach2007Addflower(CNetMsg & msg, int charidx)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_TEACH_2007_ADDFLOWER
		<< charidx;
}
#endif // EVENT_TEACH_2007

#ifdef EVENT_FLOWERTREE_2007
void HelperFlowerTree2007Msg( CNetMsg& msg, MSG_HELPER_FLOWERTREE_2007_TYPE subtype ,int nCharIndex )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_FLOWERTREE_2007
		<< (unsigned char) subtype
		<< nCharIndex;
}
#endif //EVENT_FLOWERTREE_2007

#ifdef NEW_GUILD
void HelperGuildInclineEstablishRepMsg( CNetMsg& msg, int guildindex, int charindex, char guildincline, MSG_GUILD_ERROR_TYPE errcode )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_GUILD_INCLINE_ESTABLISH_REP
		<< guildindex
		<< charindex
		<< guildincline
		<< errcode;
}

void HelperGuildMemberAdjustRepMsg( CNetMsg& msg, int guildindex, int ownerindex, int charindex, const char* strPositionName, int contributeExp, int contributeFame,  MSG_GUILD_ERROR_TYPE errcode )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_GUILD_MEMBER_ADJUST_REP
		<< guildindex
		<< ownerindex
		<< charindex
		<< strPositionName
		<< contributeExp
		<< contributeFame
		<< errcode;
}

void HelperNewGuildInfoRepMsg( CNetMsg& msg, int charindex, MSG_GUILD_ERROR_TYPE errorcode )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_INFO_REP
		<< charindex
		<< errorcode;
}
void HelperNewGuildInfoNotifyMsg( CNetMsg& msg, int charindex, int guildindex, int avelevel, int usepoint, MSG_GUILD_ERROR_TYPE errorcode  )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_INFO_NTF
		<< charindex
		<< guildindex
		<< avelevel
		<< usepoint
		<< errorcode;
}
void HelperNewGuildMemberListRepMsg( CNetMsg& msg, int endcount, int & guildstart, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode,  int* membercharindex, int* cumulatePoint, const char CharName[][MAX_CHAR_NAME_LENGTH], const char positionName[][GUILD_POSITION_NAME+1], char* job, char* job2, int* level, int* position )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_MEMBERLIST_REP
		<< charindex
		<< guildindex
		<< errcode
		<< endcount - guildstart;

	for( ; guildstart < endcount; guildstart++ )
	{
		msg << membercharindex[guildstart]
			<< cumulatePoint[guildstart]
			<< CharName[guildstart]
			<< positionName[guildstart]
			<< job[guildstart]
			<< job2[guildstart]
			<< level[guildstart]
			<< position[guildstart];
	}
		

}





void HelperNewGuildManageRepMsg( CNetMsg& msg, int endcount, int & guildstart, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode,  int* membercharindex, int* contributeExp, int* contributeFame, const char CharName[][MAX_CHAR_NAME_LENGTH], const char positionName[][GUILD_POSITION_NAME+1], int* level, int* position, char first)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_MANAGE_REP
		<< first
		<< charindex
		<< guildindex
		<< errcode
		<< endcount - guildstart;

	for( ; guildstart < endcount; guildstart++ )
	{
		msg << membercharindex[guildstart]
			<< contributeExp[guildstart]
			<< contributeFame[guildstart]
			<< CharName[guildstart]
			<< positionName[guildstart]
			<< level[guildstart]
			<< position[guildstart];
	}
}



void HelperNewGuildNoticeRepMsg( CNetMsg& msg, int charindex, int guildindex, const char* title, const char* text )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_NOTICE_REP
		<< charindex
		<< guildindex;
	if( title != NULL && text != NULL )
	{
		msg << title
			<< text;
	}
}
void HelperNewGuildNoticeUpdateRepMsg( CNetMsg& msg, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_NOTICE_UPDATE_REP
		<< charindex
		<< guildindex
		<< errcode;
}

void HelperNewGuildNoticeTransMsg( CNetMsg& msg, int guildindex, const char* title, const char* text )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_NTF
		<< guildindex
		<< title
		<< text;	
}

void HelperNewGuildNoticeTransRep( CNetMsg& msg, int guildindex, int charindex, MSG_GUILD_ERROR_TYPE errcode )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_REP
		<< guildindex
		<< charindex
		<< errcode;
}

void HelperNewGuildSkillListRepMsg( CNetMsg& msg, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode, int skillcount, int* skillindex, int* skillLevel )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_NOTICE_SKILLLIST_REP
		<< charindex
		<< guildindex
		<< errcode
		<< skillcount;
	for( int i = 0; i < skillcount; i++ )
	{
		msg << skillindex[i]
			<< skillLevel[i];
	}

}
void HelperExtendGuildLoadNotifyMsg( CNetMsg& msg, CGuild* guild, int skillcount, int* skillindex, int* skillLevel )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_LOAD_NTF
		<< guild->index()
		<< guild->GetGuildPoint()
		<< guild->GetIncline()
		<< guild->maxmember()
		<< guild->GetLandCount();
	int landcount = guild->GetLandCount();
	if( landcount == 0 )
	{
		msg << -1;
	}
	else
	{
		for( int i = 0; i < landcount; i++ )
		{
			int* land = guild->GetLand();
			if( land[i] != -1 )
			{
				msg << land[i];				
			}
			else
				continue;
		}
	}
	msg	<< skillcount;
	for( int i = 0; i < skillcount; i++ )
	{
		msg << skillindex[i]
			<< skillLevel[i];
	}

}

void HelperExtendGuildMemberListMsg( CNetMsg& msg, int endcount, int & guildstart, CGuild* guild )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_NEW_GUILD_MEMBER_LIST
		<< guild->index()
		<< endcount - guildstart;
	
	
	CGuildMember* member;
	for(  ; guildstart < endcount; guildstart++ )
	{		
		member = guild->member(guildstart);
		if( member )
		{
			msg << member->charindex()
				<< member->GetcontributeExp()
				<< member->GetcontributeFame()
				<< member->GetcumulatePoint()
				<< member->GetPositionName()
				<< member->GetChannel()
				<< member->GetZoneNum();
		}		
	}
	
}

void HelperNewGuildPointUpdateMsg( CNetMsg& msg, int charindex, int guildindex, int guildpoint )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_UPDATE_GUILD_POINT
		<< charindex
		<< guildindex
		<< guildpoint;
}


void HelperNewGuildNotice( CNetMsg& msg, int charindex, int guildindex, const char* title, const char* text )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_GUILD_NOTICE
		<< charindex
		<< guildindex;
	if( title != NULL && text != NULL )
	{
		msg << title
			<< text;
	}	
}

void HelperNewGuildMemberPointSaveMsg( CNetMsg& msg, int charindex, int guildindex, int memberpoint )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_SAVE_GUILD_MEMBER_POINT
		<< guildindex
		<< charindex
		<< memberpoint;
}
#endif // NEW_GUILD

#ifdef PET_NAME_CHANGE
void HelperPetNameChange( CNetMsg& msg, MSG_EX_PET_CHANGE_NAME_ERROR_TYPE err,int charidx, int petidx, const char* strPetName )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_PET_NAME_CHANGE
		<< err
		<< charidx
		<< petidx
		<< strPetName;
}
#endif // PET_NAME_CHANGE

#ifdef REWARD_IDC2007
void HelperRewardIDC2007Msg( CNetMsg& msg, int userindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_REWARD_IDC2007_REP
		<< userindex;
}
#endif // REWARD_IDC2007

#ifdef EVENT_HALLOWEEN_2007
void HelperHalloween2007Msg( CNetMsg& msg, int charIndex, unsigned char error )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_HALLOWEEN_2007
		<< charIndex
		<< error;
}
#endif //EVENT_HALLOWEEN_2007

#ifdef DYNAMIC_DUNGEON
void HelperDVDRateChangeMsg( CNetMsg& msg, unsigned char subtype, int nRate )
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_DVD_RATE_CHANGE
		<< subtype
		<< nRate;
}
#endif //DYNAMIC_DUNGEON

#ifdef ALTERNATE_MERCHANT
void HelperMerchantErrorMsg( CNetMsg& msg, MSG_EX_ALTERNATE_MERCHANT_ERROR_TYPE errorType, int CharIndex )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_ALTERNATEMERCHANT
		<< errorType
		<< CharIndex;
}

void HelperMerchantProductRecoverMsg( CNetMsg& msg, int nCharIndex, int nItemCount, int* pItemDBIndex,  char strSerial[][50],  int* pItemCount, int* pItemWearPos, 
		int* pItemFlag, int*  pItemPlus, short* pItemOption0, short* pItemOption1, short* pItemOption2, short* pItemOption3, short* pItemOption4 )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_ALTERNATEMERCHANT_PRODUCT_RECOVERY
		<< nCharIndex
		<< nItemCount;

	for( int i = 0; i < nItemCount; i++ )
	{
		msg << pItemDBIndex[i]
			<< strSerial[i]
			<< pItemCount[i]
			<< pItemWearPos[i] << pItemFlag[i] << pItemPlus[i]
			<< pItemOption0[i] << pItemOption1[i] << pItemOption2[i]
			<< pItemOption3[i] << pItemOption4[i];
	}

}

void HelperMerchantNasMsg( CNetMsg& msg, int nCharIndex, int nItemDBIndex, 
						  int nItemPlus, int nItemWearPos, int nItemFlag, const char* pSerial, int nItemCount, 
						  short nItemOption0, short nItemOption1, short  nItemOption2, short  nItemOption3, short nItemOption4 )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_ALTERNATEMERCHANT_PRODUCT_RECOVERY
		<< nCharIndex
		<< nItemDBIndex << pSerial
		<< nItemWearPos << nItemFlag << nItemPlus
		<< nItemOption0 << nItemOption1 << nItemOption2
		<< nItemOption3 << nItemOption4;
}

void HelperMerchantStartMsg( CNetMsg& msg, int nCharIndex )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_ALTERNATEMERCHANT_START
		<< nCharIndex;
}
#endif //ALTERNATE_MERCHANT

#ifdef IRIS_POINT
void HelperToConnAddIpointMsg( CNetMsg& msg, int user_index, MSG_CONN_IPOINT_TYPE type, int nIpoint )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_TO_CONN_ADD_IPOINT
		<< user_index
		<< (int) type
		<< nIpoint;
}
#endif //IRIS_POINT

#ifdef ATTACK_PET
void HelperAttackPet( CNetMsg& msg, MSG_HELPER_APET_TYPE type, int charIndex )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_APET
		<< (unsigned char) type
		<< charIndex;
}
#endif // ATTACK_PET

#ifdef EVENT_PHOENIX	// //피닉스 이벤트 yhj	
void HelperEventPhoenixMsg(CNetMsg& msg, MSG_HELPER_EVENT_PHOENIX_ERROR_TYPE nError, int nCharIdx )
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_EVENT_PHOENIX
		<< (unsigned char) nError
		<< nCharIdx;
}
#endif  // EVENT_PHOENIX

#ifdef TRADE_AGENT
void HelperTradeAgentErrorMsg(CNetMsg& msg, MSG_TRADEAGENT_ERROR_TYPE errorType, int CharIndex,char errorPart)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_TRADEAGENT_ERROR
		<< (unsigned char)errorType
		<< CharIndex
		<< errorPart;
}

// 등록 리스트
void HelperTradeAgentRegListRepMsg(CNetMsg& msg, int nCharIndex, int nCharRegTotCount, int nMaxPageNo, int nPageNo, int nItemCount, int* pListindex, int* pItemSerial, int* pItemPlus, int* pItemFlag,S_ITEMOPTION* pItemOption,int* pQuantity,LONGLONG* ptotalmoney,int* pFinishDay,int* pFinishDayUnit)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_TRADEAGENT_REG_LIST_REP
		<< nCharIndex
		<< nCharRegTotCount
		<< nMaxPageNo
		<< nPageNo
		<< nItemCount;

	for( int i = 0; i < nItemCount; i++ )
	{
		msg << pListindex[i]
			<< pItemSerial[i]
			<< pItemPlus[i]
			<< pItemFlag[i]
			<< pItemOption[i].m_nOptionCnt;

		for(int j=0; j < pItemOption[i].m_nOptionCnt; j++)
		{
			msg << 	pItemOption[i].m_Option_type[j]
				<< 	pItemOption[i].m_Option_level[j];
		}

		msg << pQuantity[i]
			<< ptotalmoney[i]
			<< pFinishDay[i]
			<< pFinishDayUnit[i];
	}
}

// 정산 리스트
void HelperTradeAgentCalcListRepMsg(CNetMsg& msg, int nCharIndex, int nMaxPageNo, int nPageNo, int nItemCount, int* pItemSerial, int* pItemPlus, int* pItemFlag,S_ITEMOPTION* pItemOption, int* pQuantity,LONGLONG* ptotalmoney,int* pState,int* pPassDay,CLCString *pCharname)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_TRADEAGENT_CALCLIST_REP
		<< nCharIndex
		<< nMaxPageNo
		<< nPageNo
		<< nItemCount;

	for( int i = 0; i < nItemCount; i++ )
	{
		msg << pItemSerial[i]
			<< pItemPlus[i]
			<< pItemFlag[i]
			
			<< pItemOption[i].m_nOptionCnt;			

		for(int j=0; j < pItemOption[i].m_nOptionCnt; j++)
		{
			msg	<<	pItemOption[i].m_Option_type[j]
				<<	pItemOption[i].m_Option_level[j];
		}

		msg	<< pQuantity[i]
			<< ptotalmoney[i]
			<< pState[i]
			<< pPassDay[i]
			<< pCharname[i];
	}
}

// 정산 처리
void HelperTradeAgentCalculateRepMsg(CNetMsg& msg, int nCharIndex,int nMaxPageNo,int nPageNo,int nItemCount,int* pListindex, int* pItemSerial,CLCString* pItemSerial2,int* pItemPlus, int* pItemFlag,S_ITEMOPTION* pItemOption,int* pQuantity,LONGLONG* ptotalmoney,int* pState)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_TRADEAGENT_CALCULATE_REP
		<< nCharIndex
		<< nMaxPageNo
		<< nPageNo
		<< nItemCount;

	for( int i = 0; i < nItemCount; i++ )
	{
		msg << pListindex[i]
			<< pItemSerial[i]
			<< pItemSerial2[i]
			<< pItemPlus[i]
			<< pItemFlag[i]

			<< pItemOption[i].m_nOptionCnt;			

		for(int j=0; j < pItemOption[i].m_nOptionCnt; j++)
		{
			msg	<<	pItemOption[i].m_Option_type[j]
				<<	pItemOption[i].m_Option_level[j];
		}

		msg	<< pQuantity[i]
			<< ptotalmoney[i]
			<< pState[i];
	}

}

// 체크 정산
void HelperTradeAgentCheckCalcRepMsg(CNetMsg& msg, int nCharIndex, int nIsBeCalcdata)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_TRADEAGENT_CHECKCALC_REP
		<< nCharIndex
		<< nIsBeCalcdata;
}

// 조회 
void HelperTradeAgentSearchRepMsg(CNetMsg& msg, int nCharIndex,int nMaxPageNo, int nPageNo, int nItemCount, int* pListindex, int* pItemSerial,int* pItemPlus,int* pItemFlag,S_ITEMOPTION* pItemOption,int* pQuantity,LONGLONG* ptotalmoney,int* pLevel,CLCString* pSellCharname)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_TRADEAGENT_SEARCH_REP
		<< nCharIndex
		<< nMaxPageNo
		<< nPageNo
		<< nItemCount;

	for( int i = 0; i < nItemCount; i++ )
	{
		msg << pListindex[i]
			<< pItemSerial[i]
			<< pItemPlus[i]
			<< pItemFlag[i]
			<< pItemOption[i].m_nOptionCnt;

		for(int j=0; j < pItemOption[i].m_nOptionCnt; j++)
		{
			msg << pItemOption[i].m_Option_type[j]
				<< pItemOption[i].m_Option_level[j];
				
		}

		msg	<< pQuantity[i]
			<< ptotalmoney[i]
			<< pLevel[i]
			<< pSellCharname[i];
	}
}

// 등록 
void HelperTradeAgentRegRepMsg(CNetMsg& msg, int nCharIndex,char tab, char row, char col, int nitem_serial,CLCString item_serial2, CLCString item_name,int Quantity,LONGLONG TotalMoney,LONGLONG Guaranty,int nDbIndex)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_TRADEAGENT_REG_REP
		<< nCharIndex
		<< tab
		<< row
		<< col
		<< nitem_serial
		<< item_serial2
		<< item_name	
		<< Quantity
		<< TotalMoney
		<< Guaranty
		<< nDbIndex;
}

//등록 취소 
void HelperTradeAgentRegCancelRepMsg(CNetMsg& msg, int nCharIndex,CLCString stItemname,int nItemCount)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_TRADEAGENT_REG_CANCEL_REP
		<< nCharIndex
		<< stItemname
		<< nItemCount;

}

//구매
void HelperTradeAgentBuyRepMsg(CNetMsg& msg, int nCharIndex, int nSellcharindex, LONGLONG TotalMoney,CLCString stItemname,int nItemCount)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_TRADEAGENT_BUY_REP
		<< nCharIndex
		<< nSellcharindex
		<< TotalMoney
		<< stItemname
		<< nItemCount;
}

//반송
void HelperTradeAgentReturnedMsg(CNetMsg& msg, int nSellcharindex,CLCString stItemName,int nItemCount)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_TRADEAGENT_RETURNED
		<< nSellcharindex
		<< stItemName
		<< nItemCount;
}

#endif  // TRADE_AGENT

#ifdef EXPEDITION_RAID

void HelperExpedErrorMsg(CNetMsg& msg, unsigned char errorType, int CharIndex)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_EXPED_ERROR
		<< errorType
		<< CharIndex;
}

void HelperExpedCreateRepMsg(CNetMsg& msg, char nExpedType1,char nExpedType2,char nExpedType3, int nBossIndex,CLCString BossCharName,int nMemberCount,int *pCharIdex,CLCString* pCharName,int* pGroupType,int* pMemberType,int *pSetLabelType,int* pQuitType)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_EXPED_CREATE_REP
		<< nExpedType1
		<< nExpedType2
		<< nExpedType3
		<< nBossIndex
		<< BossCharName
		<< nMemberCount;

	for(int i=0; i < nMemberCount; i++)
	{
		msg << pCharIdex[i]
			<< pCharName[i]
			<< pGroupType[i]
			<< pMemberType[i]
			<< pSetLabelType[i]
			<< pQuitType[i];
	}
}

void HelperExpedInviteRepMsg(CNetMsg& msg, int nBossIndex, CLCString strBossName, int nTargetIndex, CLCString strTargetName,char cExpedType1,char cExpedType2,char cExpedType3)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_EXPED_INVITE_REP
		<< nBossIndex
		<< strBossName
		<< nTargetIndex
		<< strTargetName
		<< cExpedType1
		<< cExpedType2
		<< cExpedType3;

}

void HelperExpedAllowRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, CLCString strTargetName,int nTargetGroup, int nTargetMember,int nTargetListIndex)
{
	msg.Init( MSG_HELPER_COMMAND );
	msg << MSG_HELPER_EXPED_ALLOW_REP
		<< nBossIndex
		<< nTargetIndex
		<< strTargetName
		<< nTargetGroup
		<< nTargetMember
		<< nTargetListIndex;
}

void HelperExpedRejectRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_REJECT_REP
		<< nBossIndex
		<< nTargetIndex;
}

void HelperExpedQuitRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, int nQuitMode, unsigned char errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_QUIT_REP
		<< nBossIndex
		<< nTargetIndex
		<< nQuitMode
		<< errcode;
}

void HelperExpedKickRepMsg(CNetMsg& msg, int nBossIndex, int nTargetIndex, unsigned char errcode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_KICK_REP
		<< nBossIndex
		<< nTargetIndex
		<< errcode;
}

void HelperExpedChangeBossRepMsg(CNetMsg& msg, int nBossIndex,  int nNewBossIndex, int nChangeMode)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_CHANGEBOSS_REP
		<< nBossIndex
		<< nNewBossIndex
		<< nChangeMode;
}
void HelperExpedChangeTypeRepMsg(CNetMsg& msg, int nBossIndex, char cDiviType, char cExpedType)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_CHANGETYPE_REP
		<< nBossIndex
		<< cDiviType
		<< cExpedType;
}

void HelperExpedEndExpedRepMsg(CNetMsg& msg, int nBossIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_ENDEXPED_REP
		<< nBossIndex;
}

void HelperExpedSetMBossRepMsg(CNetMsg& msg, int nBossIndex, int nNewMBossIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_SETMBOSS_REP
		<< nBossIndex
		<< nNewMBossIndex;
}

void HelperExpedResetMBossRepMsg(CNetMsg& msg, int nBossIndex, int nNewMBossIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_RESETMBOSS_REP
		<< nBossIndex
		<< nNewMBossIndex;
}

void HelperExpedMoveGroupRepMsg(CNetMsg& msg,int nBossIndex,int nSourceGroup, int nMoveCharIndex, int nTargetGroup,int nTargetListindex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_MOVEGROUP_REP
		<< nBossIndex
		<< nSourceGroup
		<< nMoveCharIndex
		<< nTargetGroup
		<< nTargetListindex;
}

void HelperExpedSetLabelRepMsg(CNetMsg& msg, int nBossIndex,int nType,int nMode,int nLabel,int nDestIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_SET_LABEL_REP
		<< nBossIndex
		<< nType
		<< nMode
		<< nLabel
		<< nDestIndex;
}

void HelperExpedRejoinRepMsg(CNetMsg& msg, int nBossIndex, int nJoinIndex)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_REJOIN_REP
		<< nBossIndex
		<< nJoinIndex;
}

void HelperExpedChatRepMsg(CNetMsg& msg, int nBossIndex, int nCharIndex, CLCString strName,int nGroupTyp,CLCString strChat)
{
	msg.Init(MSG_HELPER_COMMAND);
	msg << MSG_HELPER_EXPED_CHAT_REP
		<< nBossIndex
		<< nCharIndex
		<< strName
		<< nGroupTyp
		<< strChat;
}

#endif //EXPEDITION_RAID


