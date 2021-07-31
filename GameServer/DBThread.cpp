/************************************
  수정사항 
  수정한 사람: bs
  수정일: 2004-12-15 오후 5:47:16
  설명: 
 ************************************/

#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"
#include "DBThread.h"
#include "DBCmd.h"
#include "CmdMsg.h"


#ifdef GAMELOG
#undef GAMELOG
#endif


static CLCString assistitem(256);
static CLCString assistskill(256);
static CLCString assistlevel(256);
static CLCString assistremain(256);
static CLCString assisthit0(256);
static CLCString assisthit1(256);
static CLCString assisthit2(256);

static char outassistitem[256];
static char outassistskill[256];
static char outassistlevel[256];
static char outassistremain[256];
static char outassisthit0[256];
static char outassisthit1[256];
static char outassisthit2[256];

// 옵션 없는 악세사리 옵션 붙이기
CItem* OptionSettingItem(CPC* ch, CItem* item);
// --- 옵션 없는 악세사리 옵션 붙이기


//////////////////
// CCharDBQ member
CCharDBQ::CCharDBQ()
{
	m_head = m_tail = 0;
}

CCharDBQ::~CCharDBQ()
{
	m_head = m_tail = 0;
}

CDescriptor* CCharDBQ::GetFromQ()
{
	CMutex mutex(&m_mutexobj);

	CDescriptor* ret = NULL;
	while(!IsEmpty() && !ret)
	{
		ret = m_desc[m_head];
		m_head = (m_head + 1) % MAX_DB_Q;
	}

	return ret;
}

bool CCharDBQ::AddToQ(CDescriptor* d)
{
	CMutex mutex(&m_mutexobj);

	if (IsFull())
		return false;

	d->m_dbrunning = true;
	m_desc[m_tail] = d;
	m_tail = (m_tail + 1) % MAX_DB_Q;

	return true;
}

void CCharDBQ::EraseQ(CDescriptor* d)
{
	CMutex mutex(&m_mutexobj);

	int point = m_head;

	while (point != m_tail) {
		if (m_desc[point] == d) {
			m_desc[point] = NULL;
		}
		point ++;
		if (point >= MAX_DB_Q) 
			point = 0;
	}
}

///////////////////
// CDBThread member
CDBThread::CDBThread()
{
}

CDBThread::~CDBThread()
{
}

bool CDBThread::CreateThread()
{
#ifdef CIRCLE_WINDOWS
	if (_beginthread(CDBThread::RunThread, sizeof(CCharDBQ*), this) == -1)
		return false;
#else
	pthread_t th;
	if (pthread_create(&th, NULL, CDBThread::RunThread, this) != 0)
		return false;
#endif
	return true;
}

#ifdef CIRCLE_WINDOWS
void CDBThread::RunThread(void* p)
{
	CDBThread* _this = (CDBThread*)p;
	_this->Run();
	_endthread();
}
#else
void* CDBThread::RunThread(void* p)
{
	CDBThread* _this = (CDBThread*)p;
	_this->Run();
	pthread_exit(NULL);
	return NULL;
}
#endif

void CDBThread::Run()
{
	gThreadIDDBThread = LC_CURRENT_THREAD_ID;

	CDescriptor* d;
	int count = 0;

	m_queue.m_dbend = m_queue.m_dbreqend = false;

	DBLOG	<< init("SYSTEM")
			<< "Enter DB Thread"
			<< end;

//	while (true)
	for(;;)
	{
		if (m_queue.IsEmpty())
		{
			if (!m_queue.m_dbreqend)
			{
				Wait300Millis();
			}
			else
			{
				break;
			}
		}

		d = m_queue.GetFromQ();
		if (!d)
		{
			count ++;
			if (count > 100) {	// 0.3 * 100 = 30초 마다 ping
				count = 0;
				PingDB();
			}
		}
		else
		{
			CMutex mutex(&d->m_mutexobj);

			count = 0;
			switch (STATE(d))
			{
			case CON_WAIT_USER_DB: 
				OnUserDB(d);
				WAITTING_DB(d) = false;
				break;
			case CON_WAIT_NEW_CHAR_DB:
				OnNewCharDB(d);
				WAITTING_DB(d) = false;
				break;
			case CON_WAIT_DEL_CHAR_DB:
				OnDelCharDB(d);
				WAITTING_DB(d) = false;
				break;
#ifdef ENABLE_CHARACTER_DELETE_DELAY
			case CON_WAIT_DEL_CANCEL_CHAR_DB:
				OnDelCancelCharDB(d);
				WAITTING_DB(d) = false;
				break;
#endif // ENABLE_CHARACTER_DELETE_DELAY
			case CON_WAIT_PLAY_CHAR_DB:
				OnPlayCharDB(d);
				WAITTING_DB(d) = false;
				break;
			case CON_DISCONNECT:
				UPDATING_DB(d) = DB_UPDATE;
				WAITTING_DB(d) = true;
				if (d->m_pChar)
				{
					if (d->m_pChar->m_bLoadChar)
					{
						UpdateChar(d);
						d->m_pChar->m_bLoadChar = false;
					}
					else
					{
						d->m_tryBug = true;
					}
				}
				m_queue.EraseQ(d);	// erase Descriptor from queue.
				UPDATING_DB(d) = DB_NOP;
				WAITTING_DB(d) = false;
				d->m_quitsave = true;
				break;
			case CON_PLAYING:
			case CON_PREPARE_PLAY:
				if (!d->m_pChar) break;
				if (!d->m_pChar->m_bLoadChar) break;
				UPDATING_DB(d) = DB_UPDATE;
				UpdateChar(d);
				UPDATING_DB(d) = DB_NOP;
				WAITTING_DB(d) = false;
				break;
			}
			d->m_dbrunning = false;
		}
	}
	m_queue.m_dbend = true;
}

void CDBThread::PingDB()
{
	mysql_ping(&gserver.m_dbchar);
}

bool CDBThread::UpdateChar(CDescriptor* d)
{
	CPC* pChar = d->m_pChar; 
	if (!pChar)	return false;

	int i;

	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);

	strcpy(m_buf, "UPDATE t_characters SET ");

#ifdef FEEITEM
	strcat(m_buf, "a_nick=");				strcat(m_buf, "'"); strcat(m_buf, pChar->m_nick); strcat(m_buf, "',");
#endif // FEEITEM
	StrCat(m_buf, "a_exp=");				IntCat(m_buf, pChar->m_exp);
	strcat(m_buf, ", a_str=");				IntCat(m_buf, pChar->m_dbStr);
	strcat(m_buf, ", a_dex=");				IntCat(m_buf, pChar->m_dbDex);
	strcat(m_buf, ", a_int=");				IntCat(m_buf, pChar->m_dbInt);
	strcat(m_buf, ", a_con=");				IntCat(m_buf, pChar->m_dbCon);
	strcat(m_buf, ", a_statpt_remain=");	IntCat(m_buf, pChar->m_statpt_remain);
	strcat(m_buf, ", a_statpt_str=");		IntCat(m_buf, pChar->m_statpt_str);
	strcat(m_buf, ", a_statpt_dex=");		IntCat(m_buf, pChar->m_statpt_dex);
	strcat(m_buf, ", a_statpt_con=");		IntCat(m_buf, pChar->m_statpt_con);
	strcat(m_buf, ", a_statpt_int=");		IntCat(m_buf, pChar->m_statpt_int);
	strcat(m_buf, ", a_hp=");				IntCat(m_buf, pChar->m_hp);
	strcat(m_buf, ", a_max_hp=");			IntCat(m_buf, pChar->m_dbHP);
	strcat(m_buf, ", a_mp=");				IntCat(m_buf, pChar->m_mp);
	strcat(m_buf, ", a_max_mp=");			IntCat(m_buf, pChar->m_dbMP);
	strcat(m_buf, ", a_loseexp=");			IntCat(m_buf, pChar->m_loseexp);
#ifdef ENABLE_CHANGEJOB
	strcat(m_buf, ", a_job2=");				IntCat(m_buf, pChar->m_job2);
#endif

	strcat(m_buf, ", a_hair_style=");		IntCat(m_buf, pChar->m_hairstyle);
	strcat(m_buf, ", a_level=");			IntCat(m_buf, pChar->m_level);

	strcat(m_buf, ", a_skill_point=");		IntCat(m_buf, pChar->m_skillPoint);
	strcat(m_buf, ", a_blood_point=");		IntCat(m_buf, pChar->m_bloodPoint);
	strcat(m_buf, ", a_datestamp='");		GetDateStr (m_te_buf);	strcat(m_buf, m_te_buf);
	strcat(m_buf, "'");
	strcat(m_buf, ", a_was_x=");			FloatCat(m_buf, GET_X(pChar), 4);
	strcat(m_buf, ", a_was_z=");			FloatCat(m_buf, GET_Z(pChar), 4);
	strcat(m_buf, ", a_was_h=");			FloatCat(m_buf, GET_H(pChar), 4);
	strcat(m_buf, ", a_was_r=");			FloatCat(m_buf, GET_R(pChar), 4);
	strcat(m_buf, ", a_was_zone=");			IntCat(m_buf, (pChar->m_pZone) ? pChar->m_pZone->m_index : -1);
	strcat(m_buf, ", a_was_area=");			IntCat(m_buf, (pChar->m_pArea) ? pChar->m_pArea->m_index : -1);
	strcat(m_buf, ", a_was_yLayer=");		IntCat(m_buf, GET_YLAYER(pChar));
	strcat(m_buf, ", a_silence_pulse=");	IntCat(m_buf, (pChar->m_silencePulse - gserver.m_pulse) < 0 ? 0 : pChar->m_silencePulse - gserver.m_pulse);
	strcat(m_buf, ", a_pkpenalty=");		IntCat(m_buf, pChar->m_pkPenalty);
	strcat(m_buf, ", a_pkcount=");			IntCat(m_buf, pChar->m_pkCount);
	strcat(m_buf, ", a_pkrecover=");		IntCat(m_buf, pChar->m_pkRecoverPulse);
	strcat(m_buf, ", a_pkpenaltyhp=");		IntCat(m_buf, pChar->m_pkPenaltyHP);
	strcat(m_buf, ", a_pkpenaltymp=");		IntCat(m_buf, pChar->m_pkPenaltyMP);
	strcat(m_buf, ", a_guildoutdate=");		IntCat(m_buf, pChar->m_guildoutdate);
	strcat(m_buf, ", a_pluseffect_option=");	IntCat(m_buf, (int)pChar->m_plusEffect);

#ifdef RESTRICT_PK
	strcat(m_buf, ", a_lastpktime=");		IntCat(m_buf, pChar->m_lastPKTime);
#endif // RESTRICT_PK

	strcat(m_buf, ", a_active_skill_index='");
	strcat(m_buf, pChar->m_activeSkillList.GetIndexString(m_te_buf));
	strcat(m_buf, "', a_active_skill_level='");
	strcat(m_buf, pChar->m_activeSkillList.GetLevelString(m_te_buf));
	strcat(m_buf, "'");

	strcat(m_buf, ", a_passive_skill_index='");
	strcat(m_buf, pChar->m_passiveSkillList.GetIndexString(m_te_buf));
	strcat(m_buf, "', a_passive_skill_level='");
	strcat(m_buf, pChar->m_passiveSkillList.GetLevelString(m_te_buf));
	strcat(m_buf, "'");

	strcat(m_buf, ", a_etc_skill_index='");
	strcat(m_buf, pChar->m_etcSkillList.GetIndexString(m_te_buf));
	strcat(m_buf, "', a_etc_skill_level='");
	strcat(m_buf, pChar->m_etcSkillList.GetLevelString(m_te_buf));
	strcat(m_buf, "'");

#ifdef FACTORY_SYSTEM
	strcat(m_buf, ", a_seal_skill_index='");
	strcat(m_buf, pChar->m_sealSkillList.GetIndexString(m_te_buf));
	strcat(m_buf, "', a_seal_skill_exp='");
	for (i = 0, m_te_buf[0] = '\0'; i < SEAL_TYPE_CONSUME + 1; i++)
	{
		if (pChar->m_sealSkillExp[i].nSkillIdx)
			IntCat(m_te_buf, pChar->m_sealSkillExp[i].llExp);
		else
			IntCat(m_te_buf, 0);
	}
	strcat(m_buf, "'");
#endif

	strcat(m_buf, ", a_wearing='");			
	for (i = 0, m_te_buf[0] = '\0'; i < MAX_WEARING; i++)
	{
		if (pChar->m_wearing[i])
			IntCat(m_te_buf, pChar->m_wearing[i]->m_idNum);
		else
			IntCat(m_te_buf, -1);
	}
// 050223 : bs : plus 효과를 위한 정보 추가
	for (i = 0; i < MAX_WEARING; i++)
	{
		if (pChar->m_wearing[i])
			IntCat(m_te_buf, pChar->m_wearing[i]->m_plus);
		else
			IntCat(m_te_buf, 0);
	}
// --- 050223 : bs : plus 효과를 위한 정보 추가
	strcat(m_buf, m_te_buf);
	strcat(m_buf, "'");

	// Special Skill Save
	strcat(m_buf, ", a_sskill='");
	strcat(m_buf, pChar->m_sSkillList.GetString(m_te_buf));
	strcat(m_buf, "'");

	// Quest Save
#ifdef QUEST_DATA_EXTEND
	strcat(m_buf, ", a_quest_index='', a_quest_value='', a_quest_complete='', a_quest_abandon=''");
#else
	strcat(m_buf, ", a_quest_index='");
	strcat(m_buf, pChar->m_questList.GetIndexString(m_te_buf));
	strcat(m_buf, "', a_quest_value='");
	strcat(m_buf, pChar->m_questList.GetValueString(m_te_buf));
	strcat(m_buf, "', a_quest_complete='");
	strcat(m_buf, pChar->m_questList.GetCompleteIndexString(m_te_buf));
	
	// 포기퀘스트 DB에 저장하기.
	strcat(m_buf, "', a_quest_abandon='");
	strcat(m_buf, pChar->m_questList.GetAbandonIndexString(m_te_buf));
	strcat(m_buf, "'");
#endif // QUEST_DATA_EXTEND

	/*
	// 사제 시스템 Save
	strcat(m_buf, ", a_teach_type=");		IntCat(m_buf, (int)pChar->m_teachType);
	strcat(m_buf, ", a_teach_idx='");

	for (i=0, m_te_buf[0] = '\0'; i < TEACH_MAX_STUDENTS; i++)
		IntCat(m_te_buf, pChar->m_teachIdx[i]);

	strcat(m_buf, m_te_buf);
	strcat(m_buf, "'");

	strcat(m_buf, ", a_teach_sec='");

	for (i=0, m_te_buf[0] = '\0'; i < TEACH_MAX_STUDENTS; i++)
		IntCat(m_te_buf, pChar->m_teachTime[i]);

	strcat(m_buf, m_te_buf);
	strcat(m_buf, "'");

	strcat(m_buf, ", a_fame=");			IntCat(m_buf, pChar->m_fame);
	strcat(m_buf, ", a_teach_list=");	IntCat(m_buf, (pChar->m_bTeacher) ? 1 : 0);

//0627
	strcat(m_buf, ", a_teach_complete=");	IntCat(m_buf, pChar->m_cntCompleteStudent);
	strcat(m_buf, ", a_teach_fail=");		IntCat(m_buf, pChar->m_cntFailStudent);
	strcat(m_buf, ", a_superstone=");		IntCat(m_buf, pChar->m_superstone);
	strcat(m_buf, ", a_guardian=");			IntCat(m_buf, pChar->m_guardian);
	// ---------- 사제 시스템
	*/

#ifdef ETC_EVENT
	strcat(m_buf, ", a_etc_event=");			IntCat(m_buf, pChar->m_etcEvent);
#endif

#ifdef EVENT_PHOENIX// yhj   081105  피닉스 
	strcat(m_buf, ", a_phoenix=");			IntCat(m_buf, pChar->m_bPhoenix_Char);
#endif  // EVENT_PHOENIX

	strcat(m_buf, " WHERE a_index=");		IntCat(m_buf, pChar->m_index);

	dbChar.SetQuery(m_buf);

	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : UPDATE CHARACTER", pChar)
				<< mysql_error(dbChar.m_dbconn) << delim
#ifdef FEEITEM
				<< "NICK" << delim << pChar->m_nick << delim
#endif // FEEITEM
				<< "EXP" << delim << pChar->m_exp << delim
				<< "STR/DEX/INT/CON" << delim << pChar->m_dbStr << "/" << pChar->m_dbDex << "/" << pChar->m_dbInt << "/" << pChar->m_dbCon << delim
				<< "STAT PT. REMAIN" << delim << pChar->m_statpt_remain << delim
				<< "STR/DEX/INT/CON" << delim << pChar->m_statpt_str << "/" << pChar->m_statpt_dex << "/" << pChar->m_statpt_int << "/" << pChar->m_statpt_con << delim
				<< "HP/MAX.HP" << delim << pChar->m_hp << "/" << pChar->m_dbHP << delim
				<< "MP/MAX.MP" << delim << pChar->m_mp << "/" << pChar->m_dbMP << delim
				<< "LOSE EXP" << delim << pChar->m_loseexp << delim
#ifdef ENABLE_CHANGEJOB
				<< "JOB2" << delim << pChar->m_job2 << delim
#endif
				<< "LEVEL" << delim << pChar->m_level << delim
				<< "SP" << delim << pChar->m_skillPoint << delim
				<< "BLOODPOINT" << delim << pChar->m_bloodPoint << delim
				<< "SLIENCE" << delim << pChar->m_silencePulse << delim
				<< "PK" << delim << pChar->m_pkPenalty << "/" << pChar->m_pkCount << delim
				<< "DATE OF GUILD OUT" << delim << pChar->m_guildoutdate << delim
				<< "ACTIVE SKILL" << delim << pChar->m_activeSkillList.GetIndexString(m_te_buf) << "/" << pChar->m_activeSkillList.GetLevelString(m_te_buf) << delim
				<< "PASSIVE SKILL" << delim << pChar->m_passiveSkillList.GetIndexString(m_te_buf) << "/" << pChar->m_passiveSkillList.GetLevelString(m_te_buf) << delim
				<< "ETC SKILL" << delim << pChar->m_etcSkillList.GetIndexString(m_te_buf) << "/" << pChar->m_etcSkillList.GetLevelString(m_te_buf) << delim
				<< "SSKILL" << delim << pChar->m_sSkillList.GetString(m_te_buf) << delim
#ifdef FACTORY_SYSTEM
				<< "SEAL SKILL" << delim << pChar->m_sealSkillList.GetIndexString(m_te_buf) << "/" 
				<< pChar->m_sealSkillExp[SEAL_TYPE_DETAIL].llExp << " "
				<< pChar->m_sealSkillExp[SEAL_TYPE_COLORFUL].llExp << " "
				<< pChar->m_sealSkillExp[SEAL_TYPE_SHARP].llExp << " "
				<< pChar->m_sealSkillExp[SEAL_TYPE_HARD].llExp << " "
				<< pChar->m_sealSkillExp[SEAL_TYPE_CONSUME].llExp << delim
#endif
#ifdef QUEST_DATA_EXTEND
				// 아래에서 별도 처리
#else
				<< "QUEST" << delim << pChar->m_questList.GetIndexString(m_te_buf) << "/" << pChar->m_questList.GetValueString(m_te_buf) << "/" << pChar->m_questList.GetCompleteIndexString(m_te_buf) << delim
				<< "QUEST ADANDON" << delim << pChar->m_questList.GetAbandonIndexString(m_te_buf) << delim
#endif // QUEST_DATA_EXTEND
#ifdef ETC_EVENT
				<< "ETC EVENT" << delim << pChar->m_etcEvent
#endif
				<< end;
	}
#ifdef NEW_GUILD
	sprintf( m_buf, "UPDATE t_characters_guildpoint SET a_guild_exp = %d, a_guild_fame = %d WHERE a_char_index = %d", pChar->m_GuildExp, pChar->m_GuildFame, pChar->m_index );
	dbChar.SetQuery( m_buf );
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : UPDATE CHARACTER", pChar)
				<< mysql_error(dbChar.m_dbconn) << delim
				<< "NICK" << delim << pChar->m_nick << delim
				<< "GUILD_EXP" << delim << pChar->m_GuildExp << delim
				<< "GUILD_FAME" << delim << pChar->m_GuildFame
				<< end;
	}

#endif // NEW_GUILD

#ifdef QUEST_DATA_EXTEND
	sprintf(m_buf, "DELETE FROM t_questdata%02d WHERE a_char_index=%d", pChar->m_index % 10, pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : DELETE QUEST DATA", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;
	}
	else
	{
		CQuest* pQuest;
		CQuest* pQuestNext = pChar->m_questList.GetNextQuest(NULL);
		while ((pQuest = pQuestNext))
		{
			pQuestNext = pChar->m_questList.GetNextQuest(pQuestNext);
			sprintf(m_buf,
					"INSERT INTO t_questdata%02d"
					" (a_char_index,a_quest_index,a_state,a_value0,a_value1,a_value2)"
					" VALUES"
					" (%d,%d,%d,%d,%d,%d)",
					pChar->m_index % 10,
					pChar->m_index,
					pQuest->GetQuestIndex(),
					pQuest->GetQuestState(),
					pQuest->GetQuestValue(0),
					pQuest->GetQuestValue(1),
					pQuest->GetQuestValue(2)
				);
			dbChar.SetQuery(m_buf);
			if (!dbChar.Update())
			{
				DBLOG	<< init("DB ERROR : INSERT QUEST DATA", pChar)
						<< mysql_error(dbChar.m_dbconn)	<< delim
						<< "QUEST INDEX"				<< delim
						<< pQuest->GetQuestIndex()		<< delim
						<< "QUEST STATE"				<< delim
						<< pQuest->GetQuestState()		<< delim
						<< "QUEST VALUES"				<< delim
						<< pQuest->GetQuestValue(0)		<< delim
						<< pQuest->GetQuestValue(1)		<< delim
						<< pQuest->GetQuestValue(2)
						<< end;
			}
		}
	}
#endif // QUEST_DATA_EXTEND

#ifdef EVENT_NEWYEAR_2006_TIME
	// 2006 신년 이벤트 저장
	sprintf(m_buf, "INSERT INTO t_event_newyear2006 (a_char_index, a_total_time) VALUES (%d, %d)", pChar->m_index, pChar->m_nTimeEventNewYear2006);
	dbChar.SetQuery(m_buf);
	if (!dbChar.Update())
	{
		sprintf(m_buf, "UPDATE t_event_newyear2006 SET a_total_time=%d WHERE a_char_index=%d", pChar->m_nTimeEventNewYear2006, pChar->m_index);
		dbChar.SetQuery(m_buf);
		if (!dbChar.Update())
		{
			DBLOG	<< init("DB ERROR : UPDATE EVENT NEWYEAR 2005", pChar)
					<< mysql_error(dbChar.m_dbconn) << delim
					<< pChar->m_nTimeEventNewYear2006
					<< end;
		}
	}
#endif // #ifdef EVENT_NEWYEAR_2006_TIME

#ifdef EVENT_SEARCHFRIEND_TIME
	//DB에 접속해서 처리.. 
	// 휴면 케릭 사냥 시간 저장
	sprintf(m_buf, "UPDATE t_event_searchfriend SET a_dormant_total_time =%d "
				   "WHERE a_dormant_index =%d and a_dormant_select = 1", pChar->m_nTimeEventSearchFriend
				   , pChar->m_index);	
	dbChar.SetQuery(m_buf);
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : UPDATE EVENT SEARCH FRIEND", pChar)
				<< mysql_error(dbChar.m_dbconn) << delim
				<< pChar->m_nTimeEventSearchFriend
				<< end;
	}

#endif // #ifdef EVENT_SEARCHFRIEND_TIME

#ifdef EX_GO_ZONE
	sprintf(m_buf, "DELETE FROM t_find_zone%02d WHERE a_char_idx = %d", pChar->m_index % 10, pChar->m_index);
	dbChar.SetQuery(m_buf);
	if( !dbChar.Update() )
	{
	}
	else
	{
		int npcidx;
		void* pos = pChar->m_listRegisterNpc.GetHead();
		while(pos)
		{
			npcidx = pChar->m_listRegisterNpc.GetData(pos);
			pos = pChar->m_listRegisterNpc.GetNext(pos);

			sprintf(m_buf, "INSERT INTO t_find_zone%02d (a_char_idx, a_search_npc) VALUES (%d, %d)", pChar->m_index%10, pChar->m_index, npcidx);
			dbChar.SetQuery(m_buf);
			if(!dbChar.Update())
			{

			}
		}
	}

	sprintf(m_buf, "DELETE FROM t_save_zone WHERE a_char_idx = %d", pChar->m_index);
	dbChar.SetQuery(m_buf);
	if( !dbChar.Update() )
	{

	}
	else
	{
		sprintf(m_buf, "INSERT INTO t_save_zone (a_char_idx, a_lastsavepos) VALUES (%d, %d)", 
			pChar->m_index, pChar->m_lastsavepos);
		dbChar.SetQuery(m_buf);
		if(!dbChar.Update())
		{
			
		}
	}

#endif // EX_GO_ZONE


	///////////
	// 보조효과
	strcpy(m_buf, "DELETE FROM t_assist WHERE a_char_index=");
	IntCat(m_buf, pChar->m_index, false);
	dbChar.SetQuery(m_buf);
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : DELETE ASSIST", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;
	}
	strcpy(m_buf,
			"INSERT INTO t_assist"
			" (a_char_index,"
			" a_help_item, a_help_skill, a_help_level, a_help_remain, a_help_hit0, a_help_hit1, a_help_hit2,"
			" a_curse_item, a_curse_skill, a_curse_level, a_curse_remain, a_curse_hit0, a_curse_hit1, a_curse_hit2)"
			" VALUES (");

	IntCat(m_buf, pChar->m_index);	strcat(m_buf, ",");

	pChar->m_assist.GetListString(true, outassistitem, outassistskill, outassistlevel, outassistremain, outassisthit0, outassisthit1, outassisthit2);
	StrCat(m_buf, "'");		strcat(m_buf, outassistitem);		strcat(m_buf, "',");
	StrCat(m_buf, "'");		strcat(m_buf, outassistskill);		strcat(m_buf, "',");
	StrCat(m_buf, "'");		strcat(m_buf, outassistlevel);		strcat(m_buf, "',");
	StrCat(m_buf, "'");		strcat(m_buf, outassistremain);	strcat(m_buf, "',");
	StrCat(m_buf, "'");		strcat(m_buf, outassisthit0);		strcat(m_buf, "',");
	StrCat(m_buf, "'");		strcat(m_buf, outassisthit1);		strcat(m_buf, "',");
	StrCat(m_buf, "'");		strcat(m_buf, outassisthit2);		strcat(m_buf, "',");

	pChar->m_assist.GetListString(false, outassistitem, outassistskill, outassistlevel, outassistremain, outassisthit0, outassisthit1, outassisthit2);
	StrCat(m_buf, "'");		strcat(m_buf, outassistitem);		strcat(m_buf, "',");
	StrCat(m_buf, "'");		strcat(m_buf, outassistskill);		strcat(m_buf, "',");
	StrCat(m_buf, "'");		strcat(m_buf, outassistlevel);		strcat(m_buf, "',");
	StrCat(m_buf, "'");		strcat(m_buf, outassistremain);	strcat(m_buf, "',");
	StrCat(m_buf, "'");		strcat(m_buf, outassisthit0);		strcat(m_buf, "',");
	StrCat(m_buf, "'");		strcat(m_buf, outassisthit1);		strcat(m_buf, "',");
	StrCat(m_buf, "'");		strcat(m_buf, outassisthit2);		strcat(m_buf, "')");

	dbChar.SetQuery(m_buf);
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : INSERT ASSIST", pChar)
				<< mysql_error(dbChar.m_dbconn) << delim;

		pChar->m_assist.GetListString(true, outassistitem, outassistskill, outassistlevel, outassistremain, outassisthit0, outassisthit1, outassisthit2);
		DBLOG	<< "HELP" << delim
				<< outassistitem << delim
				<< outassistskill << delim
				<< outassistlevel << delim
				<< outassistremain << delim
				<< outassisthit0 << delim
				<< outassisthit1 << delim
				<< outassisthit2 << delim;

		pChar->m_assist.GetListString(false, outassistitem, outassistskill, outassistlevel, outassistremain, outassisthit0, outassisthit1, outassisthit2);
		DBLOG	<< "CURSE" << delim
				<< outassistitem << delim
				<< outassistskill << delim
				<< outassistlevel << delim
				<< outassistremain << delim
				<< outassisthit0 << delim
				<< outassisthit1 << delim
				<< outassisthit2
				<< end;
	}


	////////////////////////////////////
	// 060227 : bs : 보조효과 : 절대시간
	sprintf(m_buf, "DELETE FROM t_assist_abstime WHERE a_char_index=%d", pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : DELETE ABS ASSIST", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;
	}
	int nAssistABSTypeCount = pChar->m_assist.GetABSTimeTypeList(NULL);
	if (nAssistABSTypeCount > 0)
	{
		int* nAssistABSItemIndex = new int[nAssistABSTypeCount];
		int* nAssistABSSkillIndex = new int[nAssistABSTypeCount];
		int* nAssistABSSkillLevel = new int[nAssistABSTypeCount];
		char* nAssistABSHit0 = new char[nAssistABSTypeCount];
		char* nAssistABSHit1 = new char[nAssistABSTypeCount];
		char* nAssistABSHit2 = new char[nAssistABSTypeCount];
		int* nAssistABSEndTime = new int[nAssistABSTypeCount];

		pChar->m_assist.GetABSTimeTypeList(nAssistABSItemIndex, nAssistABSSkillIndex, nAssistABSSkillLevel, nAssistABSHit0, nAssistABSHit1, nAssistABSHit2, nAssistABSEndTime);

		for (i = 0; i < nAssistABSTypeCount; i++)
		{
			sprintf(m_buf,
				"INSERT INTO t_assist_abstime ("
				" a_char_index,"
				" a_item_index,"
				" a_skill_index,"
				" a_skill_level,"
				" a_hit0,"
				" a_hit1,"
				" a_hit2,"
				" a_end_time"
				" ) VALUES ("
				" %d,"
				" %d,"
				" %d,"
				" %d,"
				" %d,"
				" %d,"
				" %d,"
				" %d"
				" )"
				,
				pChar->m_index,
				nAssistABSItemIndex[i],
				nAssistABSSkillIndex[i],
				nAssistABSSkillLevel[i],
				nAssistABSHit0[i],
				nAssistABSHit1[i],
				nAssistABSHit2[i],
				nAssistABSEndTime[i]
				);
			dbChar.SetQuery(m_buf);
			if (!dbChar.Update())
			{
				DBLOG	<< init("DB ERROR : INSERT ABS ASSIST", pChar)
						<< mysql_error(dbChar.m_dbconn) << delim
						<< nAssistABSItemIndex[i] << delim
						<< nAssistABSSkillIndex[i] << delim
						<< nAssistABSSkillLevel[i] << delim
						<< nAssistABSHit0[i] << delim
						<< nAssistABSHit1[i] << delim
						<< nAssistABSHit2[i] << delim
						<< nAssistABSEndTime[i]
						<< end;
			}
		}

		delete [] nAssistABSItemIndex;
		delete [] nAssistABSSkillIndex;
		delete [] nAssistABSSkillLevel;
		delete [] nAssistABSHit0;
		delete [] nAssistABSHit1;
		delete [] nAssistABSHit2;
		delete [] nAssistABSEndTime;
	}
	// 060227 : bs : 보조효과 : 절대시간
	////////////////////////////////////


	/////////
	// INVEN
	// 다 지우고 DELETE FROM t_inven00 WHERE a_char_idx=idx
	strcpy(m_buf, "DELETE FROM t_inven");
	IntCat0(m_buf, pChar->m_index % 10, 2, false);
	strcat(m_buf, " WHERE a_char_idx=");
	IntCat(m_buf, pChar->m_index, false);
	dbChar.SetQuery(m_buf);
	dbChar.BeginTrans();
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : DELETE INVENTORY", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;
		dbChar.Rollback();
	}
	else
	{
		bool bsuccess = true;

		// 새로 insert
		int tabidx[] = {INVENTORY_NORMAL, INVENTORY_QUEST, INVENTORY_EVENT};
		for (i = 0; bsuccess && i < 3; i++)
		{
			CInventory* inven = GET_INVENTORY(pChar, tabidx[i]);
			int row, col;

			for (row = 0; bsuccess && row < MAX_INVENTORY_ROWS; row++)
			{
				if (inven->m_rows[row] && !(inven->m_rows[row]->IsEmpty()))
				//if (inven->m_rows[row])
				{
					// INSERT INTO t_inven00 (a_char_idx, a_tab_idx, a_row_idx, a_item_idx, a_plus, a_wear_pos, a_flag, a_serial, a_count, ... )
					// VALUES ()
					strcpy(m_buf, "INSERT INTO t_inven");
					IntCat0(m_buf, pChar->m_index % 10, 2, false);
					strcat(m_buf, " (a_char_idx, a_tab_idx, a_row_idx,");
					strcat(m_buf, " a_item_idx0, a_plus0, a_wear_pos0, a_flag0, a_used0, "
#ifdef COMPOSITE_TIME
						"a_used0_2, "
#endif	// COMPOSITE_TIME
						"a_serial0, a_count0,");
					strcat(m_buf, " a_item0_option0, a_item0_option1, a_item0_option2, a_item0_option3, a_item0_option4,");

					strcat(m_buf, " a_item_idx1, a_plus1, a_wear_pos1, a_flag1, a_used1, "
#ifdef COMPOSITE_TIME
						"a_used1_2, "
#endif	// COMPOSITE_TIME
						"a_serial1, a_count1,");
					strcat(m_buf, " a_item1_option0, a_item1_option1, a_item1_option2, a_item1_option3, a_item1_option4,");

					strcat(m_buf, " a_item_idx2, a_plus2, a_wear_pos2, a_flag2, a_used2, "
#ifdef COMPOSITE_TIME
						"a_used2_2, "
#endif	// COMPOSITE_TIME
						"a_serial2, a_count2,");
					strcat(m_buf, " a_item2_option0, a_item2_option1, a_item2_option2, a_item2_option3, a_item2_option4,");

					strcat(m_buf, " a_item_idx3, a_plus3, a_wear_pos3, a_flag3, a_used3, "
#ifdef COMPOSITE_TIME
						"a_used3_2, "
#endif	// COMPOSITE_TIME
						"a_serial3, a_count3,");
					strcat(m_buf, " a_item3_option0, a_item3_option1, a_item3_option2, a_item3_option3, a_item3_option4,");

					strcat(m_buf, " a_item_idx4, a_plus4, a_wear_pos4, a_flag4, a_used4, "
#ifdef COMPOSITE_TIME
						"a_used4_2, "
#endif	// COMPOSITE_TIME
						"a_serial4, a_count4,");
					strcat(m_buf, " a_item4_option0, a_item4_option1, a_item4_option2, a_item4_option3, a_item4_option4)");

					strcat(m_buf, " VALUES (");
					IntCat(m_buf, pChar->m_index, false);
					strcat(m_buf, ",");
					IntCat(m_buf, tabidx[i]);
					strcat(m_buf, ",");
					IntCat(m_buf, row);

					for (col = 0; col < ITEMS_PER_ROW; col++)
					{
#ifdef ENABLE_WAR_CASTLE
						if (inven->m_rows[row]->m_items[col] && !(inven->m_rows[row]->m_items[col]->m_itemProto->m_flag & ITEM_FLAG_MIX))
#else
						if (inven->m_rows[row]->m_items[col])
#endif
						{
							CItem* item = inven->m_rows[row]->m_items[col];

							// , item, plus, wear, flag, serial, count
							strcat(m_buf, ",");
							IntCat(m_buf, item->m_idNum);
							strcat(m_buf, ",");
							IntCat(m_buf, item->GetItemPlus());
							strcat(m_buf, ",");
#ifndef LC_TLD		// 펫이면 접종시 벗기
							IntCat(m_buf, item->m_wearPos);
#else
							if( item->IsPet() )
								IntCat(m_buf, -1);
							else
								IntCat(m_buf, item->m_wearPos);
#endif
							strcat(m_buf, ",");
							IntCat(m_buf, item->m_flag);
							strcat(m_buf, ",");														
#ifdef COMPOSITE_TIME
							if (item->m_itemProto->m_flag & ITEM_FLAG_ABS)
							{	// 시간제 아이템
								IntCat(m_buf, item->m_used - gserver.m_gameTime);	// 남은 시간 저장							
							}
							else
							{
								IntCat(m_buf, item->m_used);
							}											

							strcat(m_buf, ",");
							IntCat(m_buf, item->m_used_2);
#else
							IntCat(m_buf, item->m_used);
#endif	// COMPOSITE_TIME
							strcat(m_buf, ", '");
							strcat(m_buf, item->m_serial);
							strcat(m_buf, "',");
							IntCat(m_buf, item->Count());

							if (   (item->m_itemProto->m_flag & ITEM_FLAG_COMPOSITE)
								&& (item->m_flag & FLAG_ITEM_COMPOSITION)
								)
							{
								// 결합된 의상 아이템은 결합된 일반 아이템의 row, col을 옵션에 저장한다
								// option[0] = row
								// option[1] = col
								CItem* pItemEquip = pChar->m_invenNormal.GetItem(item->m_nCompositeItem);
								if (pItemEquip)
								{
									strcat(m_buf, ",");		IntCat(m_buf, pItemEquip->row());		// option 0 : row
									strcat(m_buf, ",");		IntCat(m_buf, pItemEquip->col());		// option 1 : col
								}
								else
								{
									strcat(m_buf, ", 0");		// option 0
									strcat(m_buf, ", 0");		// option 1

									DBLOG	<< init("DB ERROR : NOT FOUND COMPOSITED EQUIP", pChar)
											<< "COMPOSITED ITEM" << delim
											<< itemlog(item)
											<< end;
								}
								int j;
								for (j=2; j < MAX_ITEM_OPTION; j++)
								{
									strcat(m_buf, ", 0");
								}
							}
							else
							{
								int j;
								bool bIsRare = false;
#ifdef MONSTER_RAID_SYSTEM
								if (item->IsRareItem())
									bIsRare = true;
#endif // MONSTER_RAID_SYSTEM
								if (bIsRare)
								{
#ifdef MONSTER_RAID_SYSTEM
									if (item->m_nOption == 2)
									{
										strcat(m_buf, ",");
										IntCat(m_buf, item->m_nRareOptionIndex);
										strcat(m_buf, ",");
										IntCat(m_buf, item->m_nRareOptionBit);
										for (j = 2; j < MAX_ITEM_OPTION; j++)
										{
											strcat(m_buf, ",");
											IntCat(m_buf, 0);
										}
									}
									else
#endif // MONSTER_RAID_SYSTEM
									{
										for (j=0; j < MAX_ITEM_OPTION; j++)
										{
											strcat(m_buf, ",");
											IntCat(m_buf, 0);
										}
									}
								}
								else
								{
									for (j=0; j < MAX_ITEM_OPTION; j++)
									{
										strcat(m_buf, ",");

										if (item->m_option[j].m_level > 0)
										{
											item->m_option[j].SetDBValue();
											IntCat(m_buf, item->m_option[j].m_dbValue);
										}
										else
											IntCat(m_buf, 0);
									}
								}
							}
						}
						else
						{
							// , -1, 0, -1, 0, '', 0
							strcat(m_buf, ", -1, 0, -1, 0, -1, "
#ifdef COMPOSITE_TIME
								"-1, "
#endif	// COMPOSITE_TIME
								"'', 0, 0, 0, 0, 0, 0");
						}
					}

					strcat(m_buf, ")");

					dbChar.SetQuery(m_buf);		
					if (!dbChar.Update())
					{
						DBLOG	<< init("DB ERROR : INSERT INVENTORY", pChar)
								<< mysql_error(dbChar.m_dbconn);
						for (col = 0; col < ITEMS_PER_ROW; col++)
						{
							if (inven->m_rows[row]->m_items[col])
								DBLOG	<< delim << "ITEM " << col
										<< delim << itemlog(inven->m_rows[row]->m_items[col]);
						}
						DBLOG	<< end;
						bsuccess = false;
					}
				}
			}
		}

		if (bsuccess)
			dbChar.Commit();
		else
			dbChar.Rollback();
	}


	// STASH
	if (d->m_pChar->m_bChangeStash)
	{
		d->m_pChar->m_bChangeStash = false;

		strcpy(m_buf, "DELETE FROM t_stash");
		IntCat0(m_buf, pChar->m_desc->m_index % 10, 2, false);
		strcat(m_buf, " WHERE a_user_idx=");
		IntCat(m_buf, pChar->m_desc->m_index, false);
		dbChar.SetQuery(m_buf);
		dbChar.BeginTrans();
		if (!dbChar.Update())
		{
			DBLOG	<< init("DB ERROR : DELETE STASH", pChar)
					<< mysql_error(dbChar.m_dbconn)
					<< end;
			dbChar.Rollback();
		}
		else
		{
			bool bsuccess = true;

			// 새로 insert
			for (i = 0; i < MAX_STASH_ITEM && bsuccess; i++)
			{
				const CItem* item = pChar->m_stash.item(i);
				if (item == NULL)
					continue ;

				strcpy(m_buf, "INSERT INTO t_stash");
				IntCat0(m_buf, pChar->m_desc->m_index % 10, 2, false);
				strcat(m_buf, " (a_user_idx,a_item_idx,a_plus,a_flag,a_serial,a_count,a_used, "
#ifdef COMPOSITE_TIME
					"a_used_2, "
#endif	// COMPOSITE_TIME
					"a_item_option0,a_item_option1,a_item_option2,a_item_option3,a_item_option4) VALUES (");

				IntCat(m_buf, pChar->m_desc->m_index, false);		strcat(m_buf, ", ");
				IntCat(m_buf, item->m_idNum, false);				strcat(m_buf, ", ");
				IntCat(m_buf, item->GetItemPlus(), false);			strcat(m_buf, ", ");
				IntCat(m_buf, item->m_flag, false);					strcat(m_buf, ", ");
				strcat(m_buf, "'"); strcat(m_buf, item->m_serial);	strcat(m_buf, "', ");
				IntCat(m_buf, item->Count(), false);				strcat(m_buf, ", ");
				IntCat(m_buf, item->m_used, false);					
#ifdef COMPOSITE_TIME
				strcat(m_buf, ", ");
				IntCat(m_buf, item->m_used_2, false);					
#endif	// COMPOSITE_TIME

				int j;
				bool bIsRare = false;
#ifdef MONSTER_RAID_SYSTEM
				if (item->IsRareItem())
					bIsRare = true;
#endif // MONSTER_RAID_SYSTEM
				if (bIsRare)
				{
#ifdef MONSTER_RAID_SYSTEM
					if (item->m_nOption == 2)
					{
						strcat(m_buf, ",");
						IntCat(m_buf, item->m_nRareOptionIndex);
						strcat(m_buf, ",");
						IntCat(m_buf, item->m_nRareOptionBit);
						for (j = 2; j < MAX_ITEM_OPTION; j++)
						{
							strcat(m_buf, ",");
							IntCat(m_buf, 0);
						}
					}
					else
#endif // MONSTER_RAID_SYSTEM
					{
						for (j=0; j < MAX_ITEM_OPTION; j++)
						{
							strcat(m_buf, ",");
							IntCat(m_buf, 0);
						}
					}
				}
				else
				{
					for (j=0; j < MAX_ITEM_OPTION; j++)
					{
						strcat(m_buf, ",");

						if (item->m_option[j].m_level > 0)
						{
							short dbval = 0;
							dbval |= (item->m_option[j].m_type << OPTION_VALUE_SHIFT) & 0xff00;
							dbval |= item->m_option[j].m_level & 0x00ff;
							IntCat(m_buf, dbval);
						}
						else
							IntCat(m_buf, 0);
					}
				}

				strcat(m_buf, ")");

				dbChar.SetQuery(m_buf);
				if (!dbChar.Update())
				{
					DBLOG	<< init("DB ERROR : INSERT STASH", pChar)
							<< mysql_error(dbChar.m_dbconn) << delim
							<< "ITEM" << delim
							<< itemlog(item)
							<< end;
					bsuccess = false;
				}
			}

			if (bsuccess)
				dbChar.Commit();
			else
				dbChar.Rollback();
		}
	}

	/////////
	// Quick Slot
	// 다 지우고 DELETE FROM t_quickslot0x WHERE a_char_idx=idx
	strcpy(m_buf, "DELETE FROM t_quickslot");
	IntCat0(m_buf, pChar->m_index % 10, 2, false);
	strcat(m_buf, " WHERE a_char_idx=");
	IntCat(m_buf, pChar->m_index, false);
	dbChar.SetQuery(m_buf);
	dbChar.BeginTrans();
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : DELETE QUICKSLOT", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;
		dbChar.Rollback();
	}
	else
	{
		bool bsuccess = true;
		int slot;
		
		// page : QUICKSLOT_PAGE_NUM
		for (i=0; i < QUICKSLOT_PAGE_NUM; i++)
		{
			// slot : QUICKSLOT_MAXSLOT
			for(slot=0, m_t_buf[0] = '\0'; slot < QUICKSLOT_MAXSLOT; slot++)
			{
				switch (pChar->m_quickSlot[i].m_slotType[slot])
				{
				case QUICKSLOT_TYPE_SKILL:
					IntCat(m_t_buf, pChar->m_quickSlot[i].m_slotType[slot], false);
					IntCat(m_t_buf, pChar->m_quickSlot[i].m_skillType[slot]);
					break;
					
				case QUICKSLOT_TYPE_ACTION:
					IntCat(m_t_buf, pChar->m_quickSlot[i].m_slotType[slot], false);
					IntCat(m_t_buf, pChar->m_quickSlot[i].m_actionType[slot]);
					break;
					
				case QUICKSLOT_TYPE_ITEM:
					if (pChar->m_quickSlot[i].m_item[slot])
					{
						IntCat(m_t_buf, pChar->m_quickSlot[i].m_slotType[slot], false);
						IntCat(m_t_buf, pChar->m_quickSlot[i].m_item[slot]->row());
						IntCat(m_t_buf, pChar->m_quickSlot[i].m_item[slot]->col());
					}
					else
						IntCat(m_t_buf, -1, false);
					break;
					
				case QUICKSLOT_TYPE_EMPTY:
				default:
					IntCat(m_t_buf, pChar->m_quickSlot[i].m_slotType[slot], false);
					break;
				}
				StrCat(m_t_buf, "");
			}
			
			strcpy(m_buf, "INSERT INTO t_quickslot");
			IntCat0(m_buf, pChar->m_index % 10, 2, false);
			strcat(m_buf, " (a_char_idx, a_page_idx, a_slot) VALUES (");
			IntCat(m_buf, pChar->m_index, false);				strcat(m_buf, ", ");
			IntCat(m_buf, i, false);							strcat(m_buf, ", '");
			strcat(m_buf, m_t_buf);
			strcat(m_buf, "')");
			
			dbChar.SetQuery(m_buf);
			
			if (!dbChar.Update())
			{
				DBLOG	<< init("DB ERROR : INSERT QUICKSLOT", pChar)
						<< mysql_error(dbChar.m_dbconn) << delim
						<< "QUICKSLOT " << i << delim
						<< m_t_buf
						<< end;
				bsuccess = false;
			}
		}
		
		if (bsuccess)
			dbChar.Commit();
		else
			dbChar.Rollback();
	}

	// MEMPOS
#ifdef KJTEST
	sprintf(m_buf, "select * from t_mempos where a_char_idx=%d", pChar->m_index);
	dbChar.SetQuery(m_buf);
	dbChar.Open();

	if( dbChar.m_nrecords < 1)
	{	// 해당 캐릭터의 레코드가 없는 경우
		strcpy(m_buf, "INSERT INTO t_mempos (a_char_idx " );

		for (i = 0; i < MAX_MEMPOS; i++)
		{
			StrCat(m_buf, ", a_mem_");
			IntCat(m_buf, i, false);
		}
		StrCat(m_buf, " ) VALUES ( " );
		IntCat(m_buf, pChar->m_index, false);
		for (i = 0; i < MAX_MEMPOS; i++)
		{
			strcat(m_buf, ", '");

			if (pChar->m_mempos.m_data[i])
			{
				IntCat(m_buf, pChar->m_mempos.m_data[i]->m_zone, false);
				FloatCat(m_buf, pChar->m_mempos.m_data[i]->m_x, 4);
				FloatCat(m_buf, pChar->m_mempos.m_data[i]->m_z, 4);
				IntCat(m_buf, pChar->m_mempos.m_data[i]->m_ylayer);
				StrCat(m_buf, pChar->m_mempos.m_data[i]->m_comment);
				StrCat(m_buf, "'");
			}
			else
			{
				strcat(m_buf, "'");
			}
		}

		StrCat(m_buf, ")");
	}
	else
	{	// 해당 캐릭터의 레코드가 있는 경우
		strcpy(m_buf, "UPDATE t_mempos SET");
		for (i = 0; i < MAX_MEMPOS; i++)
		{
			if (i != 0)
				strcat(m_buf, ", ");

			if (pChar->m_mempos.m_data[i])
			{
				StrCat(m_buf, "a_mem_");
				IntCat(m_buf, i, false);
				StrCat(m_buf, "='");
				IntCat(m_buf, pChar->m_mempos.m_data[i]->m_zone, false);
				FloatCat(m_buf, pChar->m_mempos.m_data[i]->m_x, 4);
				FloatCat(m_buf, pChar->m_mempos.m_data[i]->m_z, 4);
				IntCat(m_buf, pChar->m_mempos.m_data[i]->m_ylayer);
				StrCat(m_buf, pChar->m_mempos.m_data[i]->m_comment);
				StrCat(m_buf, "'");
			}
			else
			{
				StrCat(m_buf, "a_mem_");
				IntCat(m_buf, i, false);
				StrCat(m_buf, "=''");
			}
		}

		StrCat(m_buf, "WHERE a_char_idx=");
		IntCat(m_buf, pChar->m_index, false);
	}

	dbChar.SetQuery(m_buf);
	dbChar.Update();
#else
	strcpy(m_buf, "UPDATE t_mempos SET");
	for (i = 0; i < MAX_MEMPOS; i++)
	{
		if (i != 0)
			strcat(m_buf, ", ");

		if (pChar->m_mempos.m_data[i])
		{
			StrCat(m_buf, "a_mem_");
			IntCat(m_buf, i, false);
			StrCat(m_buf, "='");
			IntCat(m_buf, pChar->m_mempos.m_data[i]->m_zone, false);
			FloatCat(m_buf, pChar->m_mempos.m_data[i]->m_x, 4);
			FloatCat(m_buf, pChar->m_mempos.m_data[i]->m_z, 4);
			IntCat(m_buf, pChar->m_mempos.m_data[i]->m_ylayer);
			StrCat(m_buf, pChar->m_mempos.m_data[i]->m_comment);
			StrCat(m_buf, "'");
		}
		else
		{
			StrCat(m_buf, "a_mem_");
			IntCat(m_buf, i, false);
			StrCat(m_buf, "=''");
		}
	}

	StrCat(m_buf, "WHERE a_char_idx=");
	IntCat(m_buf, pChar->m_index, false);
	dbChar.SetQuery(m_buf);
	dbChar.Update();
	if (dbChar.m_nrecords < 1)
	{
		strcpy(m_buf, "INSERT INTO t_mempos (a_char_idx " );

		for (i = 0; i < MAX_MEMPOS; i++)
		{
			StrCat(m_buf, ", a_mem_");
			IntCat(m_buf, i, false);
		}
		StrCat(m_buf, " ) VALUES ( " );
		IntCat(m_buf, pChar->m_index, false);
		for (i = 0; i < MAX_MEMPOS; i++)
		{
			strcat(m_buf, ", '");

			if (pChar->m_mempos.m_data[i])
			{
				IntCat(m_buf, pChar->m_mempos.m_data[i]->m_zone, false);
				FloatCat(m_buf, pChar->m_mempos.m_data[i]->m_x, 4);
				FloatCat(m_buf, pChar->m_mempos.m_data[i]->m_z, 4);
				IntCat(m_buf, pChar->m_mempos.m_data[i]->m_ylayer);
				StrCat(m_buf, pChar->m_mempos.m_data[i]->m_comment);
				StrCat(m_buf, "'");
			}
			else
			{
				strcat(m_buf, "'");
			}
		}

		StrCat(m_buf, ")");
		dbChar.SetQuery(m_buf);
		
		
		if (!dbChar.Update() )
		{
			// 1062 에러 번호는 키가 중복입니다. 
			if( mysql_errno(dbChar.m_dbconn) != 1062 )
				DBLOG	<< init("DB ERROR : INSERT MEMPOS", pChar)
						<< mysql_error(dbChar.m_dbconn)
						<< end;
		}
	}
#endif // KJTEST

#ifdef PRIMIUM_MEMORYBOOK
	// MEMPOSPLUS
	strcpy(m_buf, "UPDATE t_mempos_plus SET");
	for (i = 0; i < MAX_MEMPOS_PRIMIUM; i++)
	{
		if (i != 0)
			strcat(m_buf, ", ");

		if (pChar->m_memposplus.m_data[i])
		{
			StrCat(m_buf, "a_mem_");
			IntCat(m_buf, i, false);
			StrCat(m_buf, "='");
			IntCat(m_buf, pChar->m_memposplus.m_data[i]->m_zone, false);
			FloatCat(m_buf, pChar->m_memposplus.m_data[i]->m_x, 4);
			FloatCat(m_buf, pChar->m_memposplus.m_data[i]->m_z, 4);
			IntCat(m_buf, pChar->m_memposplus.m_data[i]->m_ylayer);
			StrCat(m_buf, pChar->m_memposplus.m_data[i]->m_comment);
			StrCat(m_buf, "'");
		}
		else
		{
			StrCat(m_buf, "a_mem_");
			IntCat(m_buf, i, false);
			StrCat(m_buf, "=''");
		}
	}

	StrCat(m_buf, "WHERE a_char_idx=");
	IntCat(m_buf, pChar->m_index, false);
	dbChar.SetQuery(m_buf);
	dbChar.Update();
	if (dbChar.m_nrecords < 1)
	{
		strcpy(m_buf, "INSERT INTO t_mempos_plus (a_char_idx " );

		for (i = 0; i < MAX_MEMPOS_PRIMIUM; i++)
		{
			StrCat(m_buf, ", a_mem_");
			IntCat(m_buf, i, false);
		}
		StrCat(m_buf, " ) VALUES ( " );
		IntCat(m_buf, pChar->m_index, false);
		for (i = 0; i < MAX_MEMPOS_PRIMIUM; i++)
		{
			strcat(m_buf, ", '");

			if (pChar->m_memposplus.m_data[i])
			{
				IntCat(m_buf, pChar->m_memposplus.m_data[i]->m_zone, false);
				FloatCat(m_buf, pChar->m_memposplus.m_data[i]->m_x, 4);
				FloatCat(m_buf, pChar->m_memposplus.m_data[i]->m_z, 4);
				IntCat(m_buf, pChar->m_memposplus.m_data[i]->m_ylayer);
				StrCat(m_buf, pChar->m_memposplus.m_data[i]->m_comment);
				StrCat(m_buf, "'");
			}
			else
			{
				strcat(m_buf, "'");
			}
		}

		StrCat(m_buf, ")");
		dbChar.SetQuery(m_buf);
		
		
		if (!dbChar.Update() )
		{
			// 1062 에러 번호는 키가 중복입니다. 
			if( mysql_errno(dbChar.m_dbconn) != 1062 )
				DBLOG	<< init("DB ERROR : INSERT MEMPOSPLUS", pChar)
						<< mysql_error(dbChar.m_dbconn)
						<< end;
		}
	}
#endif	// PRIMIUM_MEMORYBOOK

#ifdef FEEITEM
	// 유료아이템 사용시간 업데이트
#ifdef KJTEST
	// 테이블에 해당 캐릭터의 레코드가 있는지 확인
	sprintf(m_buf, "select * from t_cashItemdate where a_portal_idx=%d", d->m_index);
	dbChar.SetQuery(m_buf);
	dbChar.Open();

	if( dbChar.m_nrecords < 1)
	{	// 해당 캐릭터의 레코드가 없는 경우
		sprintf(m_buf,
			"insert into t_cashItemdate(a_portal_idx, a_mempos_new, a_charslot0_new, a_charslot1_new, a_stashext_new) values(%d, %d, 0, 0, 0)", 
			d->m_index, pChar->m_memposTime);
	}
	else
	{	// 해당 캐릭터의 레코드가 있는 경우
		sprintf(m_buf,
			"UPDATE t_cashItemdate SET a_mempos_new=%d where a_portal_idx=%d",
			pChar->m_memposTime, d->m_index);
	}

	dbChar.SetQuery(m_buf);
	dbChar.Update();
#else
	sprintf(m_buf, "UPDATE t_cashItemdate SET a_mempos_new = %d where a_portal_idx = %d", pChar->m_memposTime, d->m_index);
	dbChar.SetQuery(m_buf);
	dbChar.Update();
	if (dbChar.m_nrecords < 1)
	{
		sprintf(m_buf, "insert into t_cashItemdate (a_portal_idx, a_mempos_new, a_charslot0_new, a_charslot1_new, a_stashext_new) values ( %d, %d, 0, 0, 0 ) ", d->m_index, pChar->m_memposTime);
		dbChar.SetQuery(m_buf);
		if (!dbChar.Update())
		{
			// 1062 에러 번호는 키가 중복입니다. 
			if( mysql_errno(dbChar.m_dbconn) != 1062 )
				DBLOG	<< init("DB ERROR : INSERT CASH ITEM DATE MEMPOS", pChar)
						<< mysql_error(dbChar.m_dbconn) << delim
						<< "TIME" << delim
						<< pChar->m_memposTime
						<< end;
		}
	}
#endif // KJTEST

#ifdef CHARSLOTEXT_CARD
	for(int j = 0; j < MAX_EXT_CHAR_COUNT; j++)
	{
		sprintf(m_buf, "UPDATE t_cashItemdate SET a_charslot%d_new = %d where a_portal_idx = %d", j, pChar->m_charslotTime[j], d->m_index);
		dbChar.SetQuery(m_buf);
		if (!dbChar.Update())
		{
			DBLOG	<< init("DB ERROR : UPDATE CASH ITEM DATE CHAR SLOT", pChar)
					<< mysql_error(dbChar.m_dbconn) << delim
					<< "TIME" << delim
					<< pChar->m_charslotTime[j]
					<< end;
		}
	}
#endif // CHARSLOTEXT_CARD

	sprintf(m_buf, "UPDATE t_cashItemdate SET a_stashext_new = %d where a_portal_idx = %d", pChar->m_stashextTime, d->m_index);
	dbChar.SetQuery(m_buf);
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : UPDATE CASH ITEM DATE STASH EXT", pChar)
				<< mysql_error(dbChar.m_dbconn) << delim
				<< "TIME" << delim
				<< pChar->m_stashextTime
				<< end;
	}

#endif // FEEITEM

#ifdef ENABLE_SINGLEDUNGEON_DATA
	// 퍼스널 던전 입장 데이터
#ifdef KJTEST
	// 테이블에 해당 캐릭터의 레코드가 있는지 확인
	sprintf(m_buf, "select * from t_pddata%02d where a_char_index=%d", pChar->m_index % 10, pChar->m_index);
	dbChar.SetQuery(m_buf);
	dbChar.Open();

	if( dbChar.m_nrecords < 1)
	{	// 해당 캐릭터의 레코드가 없는 경우
		sprintf(m_buf,
			"INSERT INTO t_pddata%02d(a_char_index, a_pd3_count, a_pd3_time, a_pd4_count, a_pd4_time) VALUES (%d, %d, %d, %d, %d)",
			pChar->m_index % 10, pChar->m_index, pChar->m_pd3Count, pChar->m_pd3Time, pChar->m_pd4Count, pChar->m_pd4Time);
	}
	else
	{	// 해당 캐릭터의 레코드가 있는 경우
		sprintf(m_buf,
			"UPDATE t_pddata%02d SET  a_pd3_count=%d, a_pd3_time=%d, a_pd4_count=%d, a_pd4_time=%d WHERE a_char_index=%d",
			pChar->m_index % 10, pChar->m_pd3Count, pChar->m_pd3Time, pChar->m_pd4Count, pChar->m_pd4Time, pChar->m_index);	
	}

	dbChar.SetQuery(m_buf);
	dbChar.Update();
#else
	sprintf(m_buf,

			"UPDATE t_pddata%02d SET"

			" a_pd3_count=%d,"
			" a_pd3_time=%d,"
			" a_pd4_count=%d,"
			" a_pd4_time=%d"

			" WHERE a_char_index=%d",

			pChar->m_index % 10,

			pChar->m_pd3Count,
			pChar->m_pd3Time,
			pChar->m_pd4Count,
			pChar->m_pd4Time,

			pChar->m_index);

	dbChar.SetQuery(m_buf);
	dbChar.Update();
	if (dbChar.m_nrecords < 1)
	{
		sprintf(m_buf,

				"INSERT INTO t_pddata%02d ("

				" a_char_index,"
				" a_pd3_count,"
				" a_pd3_time,"
				" a_pd4_count,"
				" a_pd4_time"

				") VALUES ("
				" %d,"
				" %d,"
				" %d,"
				" %d,"
				" %d"
				")",

				pChar->m_index % 10,

				pChar->m_index,
				pChar->m_pd3Count,
				pChar->m_pd3Time,
				pChar->m_pd4Count,
				pChar->m_pd4Time
				);
		dbChar.SetQuery(m_buf);
		if (!dbChar.Update())
		{
			// 1062 에러 번호는 키가 중복입니다. 
			if( mysql_errno(dbChar.m_dbconn) != 1062 )
				DBLOG	<< init("DB ERROR : INSERT PDDATA", pChar)
						<< mysql_error(dbChar.m_dbconn) << delim
						<< "PD3 DATA" << delim
						<< pChar->m_pd3Count << "/" << pChar->m_pd3Time << delim
						<< "PD4 DATA" << delim
						<< pChar->m_pd4Count << "/" << pChar->m_pd4Time
						<< end;
		}
	}
#endif // KJTEST
#endif

#ifdef ENABLE_PET
	// 애완동물 상태를 2로 변경후에
	// 지금 들고 있는 펫은 다시 1로 변경하면서 저장하고
	// 그외는 삭제
	sprintf(m_buf, "UPDATE t_pet SET a_enable=2 WHERE a_owner=%d", pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : UPDATE PET ENABLE", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;
	}

	// 060221 : bs : 펫 사망시 시간 저장
	// 애완동물 저장
	CPet* pet = pChar->m_petList;
	time_t curTime;
	time(&curTime);
	while (pet)
	{
		char petSkillIndex[256];
		char petSkillLevel[256];
		pet->m_skillList.GetIndexString(petSkillIndex);
		pet->m_skillList.GetLevelString(petSkillLevel);
		sprintf(m_buf,
			"UPDATE t_pet SET"
			" a_owner=%d,"
			" a_enable=1,"
			" a_lastupdate=%d,"
			" a_type=%d,"
			" a_level=%d,"
			" a_hp=%d,"
			" a_hungry=%d,"
			" a_sympathy=%d,"
#ifdef CIRCLE_WINDOWS
			" a_exp=%I64d,"
#else
			" a_exp=%lld,"
#endif
			" a_ability=%d,"
			" a_skill_index='%s',"
			" a_skill_level='%s',"
			" a_time_rebirth=%d"
#ifdef PET_DIFFERENTIATION_ITEM
			",a_color=%d"
#endif // PET_DIFFERENTIATION_ITEM
#ifdef PET_TURNTO_NPC
			",a_pet_turnto_npc= %d"
#endif //PET_TURNTO_NPC	
#ifdef PET_TURNTO_NPC_ITEM
			",a_pet_size=%d"
#endif //PET_TURNTO_NPC_ITEM
			" WHERE a_index=%d",

			pChar->m_index,
			(int)curTime,
			pet->m_petTypeGrade,
			pet->m_level,
			pet->m_hp,
			pet->m_hungry,
			pet->m_sympathy,
			pet->m_exp,
			pet->m_abilityPoint,
			petSkillIndex,
			petSkillLevel,
			pet->GetRemainRebirthTime(),
#ifdef PET_DIFFERENTIATION_ITEM
			pet->GetPetColor(),
#endif // PET_DIFFERENTIATION_ITEM
#ifdef PET_TURNTO_NPC
			pet->GetPetTurnToNpc(),
#endif //PET_TURNTO_NPC	
#ifdef PET_TURNTO_NPC_ITEM
			pet->GetPetTurnToNpcSize(),
#endif //PET_TURNTO_NPC_ITEM
			pet->m_index);

		dbChar.SetQuery(m_buf);
		if (!dbChar.Update() || dbChar.m_nrecords < 1)
		{
			sprintf(m_buf,
				"INSERT INTO t_pet ("
				"a_index,"
				"a_owner,"
				"a_enable,"
				"a_lastupdate,"
				"a_type,"
				"a_level,"
				"a_hp,"
				"a_hungry,"
				"a_sympathy,"
				"a_exp,"
				"a_ability,"
				"a_skill_index,"
				"a_skill_level,"
				"a_time_rebirth"
#ifdef PET_DIFFERENTIATION_ITEM
				",a_color"
#endif // PET_DIFFERENTIATION_ITEM
#ifdef PET_TURNTO_NPC
				",a_pet_turnto_npc"
#endif //PET_TURNTO_NPC			
#ifdef PET_TURNTO_NPC_ITEM
				",a_pet_size"
#endif //PET_TURNTO_NPC_ITEM
				")"
				"VALUES ("
				"%d,"
				"%d,"
				"1,"
				"%d,"
				"%d,"
				"%d,"
				"%d,"
				"%d,"
				"%d,"
#ifdef CIRCLE_WINDOWS
				"%I64d,"
#else
				"%lld,"
#endif
				"%d,"
				"'%s',"
				"'%s',"
				"%d"
#ifdef PET_DIFFERENTIATION_ITEM
				",%d"
#endif // PET_DIFFERENTIATION_ITEM
#ifdef PET_TURNTO_NPC
				",%d"
#endif //PET_TURNTO_NPC			
#ifdef PET_TURNTO_NPC_ITEM
				",%d"
#endif //PET_TURNTO_NPC_ITEM
				");"

				, pet->m_index
				, pChar->m_index
				, (int)curTime
				, pet->m_petTypeGrade
				, pet->m_level
				, pet->m_hp
				, pet->m_hungry
				, pet->m_sympathy
				, pet->m_exp
				, pet->m_abilityPoint
				, petSkillIndex
				, petSkillLevel
				, pet->GetRemainRebirthTime()
#ifdef PET_DIFFERENTIATION_ITEM
				, pet->GetPetColor()
#endif // PET_DIFFERENTIATION_ITEM
#ifdef PET_TURNTO_NPC
				, pet->GetPetTurnToNpc()
#endif //PET_TURNTO_NPC			
#ifdef PET_TURNTO_NPC_ITEM
				,pet->GetPetTurnToNpcSize()
#endif // PET_TURNTO_NPC_ITEM
				);
			dbChar.SetQuery(m_buf);
			if (!dbChar.Update())
			{
				// TODO : petlog
//				DBLOG	<< init("DB ERROR : INSERT PET", pChar)
//						<< mysql_error(dbChar.m_dbconn) << delim
//						<< "PET INFO" << delim
//						<< petlog(pet)
//						<< end;
			}

#ifdef PET_NAME_CHANGE
			if( pet->m_name.Length() != 0 )
			{
				sprintf( m_buf, "INSERT INTO t_pet_name ( a_pet_index, a_pet_owner, a_pet_name )"
					"VALUES ( %d, %d, '%s' )" , pet->m_index ,pChar->m_index, (const char* )pet->m_name );
				dbChar.SetQuery(m_buf);
				if (!dbChar.Update())
				{
				// TODO : petlog

				}
			}
#endif // PET_NAME_CHAGNE
		}		

		pet = pet->m_nextPet;
	}


	// 임시저장 애완동물 삭제 : a_enable == 2


	sprintf(m_buf, "DELETE FROM t_pet WHERE a_owner=%d AND a_enable=2", pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : DELETE PET DISABLED", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;
	}


#endif // ENABLE_PET


#ifdef ATTACK_PET
	// 애완동물 상태를 2로 변경후에 지금 들고 있는 펫은 다시 1로 변경하면서 저장하고 나머지 삭제
	sprintf(m_buf, "UPDATE t_apets SET a_enable=2 WHERE a_owner=%d", pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : UPDATE ATTACK PET ENABLE", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;
	}

	// 애완동물 저장
	CAPet* apet = pChar->m_pApetlist;
//	time_t curTime;
//	time(&curTime);
	while (apet)
	{
		char petSkillIndex[256] = {0,};
		char petSkillLevel[256] = {0,};
		apet->m_skillList.GetIndexString(petSkillIndex);
		apet->m_skillList.GetLevelString(petSkillLevel);
		memset(m_buf, 0, sizeof(m_buf));
		sprintf(m_buf, "UPDATE t_apets SET"
			" a_enable=1, a_seal=%d,"
			" a_name='%s',"
			" a_level=%d,"
#ifdef CIRCLE_WINDOWS
			" a_exp=%I64d,"
#else
			" a_exp=%lld,"
#endif
			" a_remain_stat=%d,"
			" a_plus_str=%d,"
			" a_plus_con=%d,"
			" a_plus_dex=%d," 
			" a_plus_int=%d,"
			" a_hp=%d, a_mp=%d,"
			" a_skill_point=%d,"
			" a_faith=%d, a_stm=%d, a_skill_level='%s', a_skill_index='%s' "
#ifdef APET_AI
			" ,a_ai_enable=%d, a_ai_slot=%d"
#endif
			" WHERE a_index=%d AND a_owner=%d;",
			apet->m_nSeal,
			(const char*)apet->m_name,
			apet->m_level,
			apet->m_exp,
			apet->m_nRemainStat,
			apet->m_nPlusStr,
			apet->m_nPlusCon,
			apet->m_nPlusDex,
			apet->m_nPlusInt,
			apet->m_hp,
			apet->m_mp,
			apet->m_nSP,
			apet->GetFaith(),
			apet->GetStamina(),
			(const char*)petSkillLevel,
			(const char*)petSkillIndex,
#ifdef APET_AI
			apet->m_cAI,
			apet->m_nAISlot,
#endif
			apet->m_index,
			pChar->m_index);
		dbChar.SetQuery(m_buf);

		if (!dbChar.Update() || dbChar.m_nrecords < 1)
		{
			sprintf( m_buf," INSERT INTO t_apets ("
			" a_index,"
			" a_owner,"
			" a_enable,"
			" a_seal,"
			" a_proto_index,"
			" a_name,"
			" a_create_date,"
			" a_lastupdate_date,"
			" a_level,"
			" a_exp,"
			" a_remain_stat,"
			" a_plus_str,"
			" a_plus_con,"
			" a_plus_dex,"
			" a_plus_int,"
			" a_skill_point,"
			" a_skill_index,"
			" a_skill_level,"
			" a_hp,"
			" a_mp,"
			" a_faith,"
			" a_stm"
#ifdef APET_AI
			", a_ai_enable"
			", a_ai_slot"
#endif
			") VALUES ("
			"%d,"	// a_index
			"%d,"	// a_owner
			"1,"	// a_enable
			"%d,"	// a_seal
			"%d,"	// a_proto_index
			"'%s',"	// a_name
			"NOW(),"// a_create_date
			"NOW(),"// a_lastupdate_date
			"%d,"	// a_level
#ifdef CIRCLE_WINDOWS
			"%I64d,"	// a_exp
#else
			"%lld,"
#endif
			"%d,"	// a_remain_stat
			"%d,"	// a_plus_str
			"%d,"	// a_plus_con
			"%d,"	// a_plus_dex
			"%d,"	// a_plus_int
			"%d,"	// a_skill_point
			"'%s','%s',"	// a_skill_index, a_skill_level
			"%d,"	// a_hp
			"%d,"	// a_mp
			"%d,"	// a_faith
			"%d "	// a_stm
#ifdef APET_AI
			", %d"	// a_ai_enable
			", %d"	// a_ai_slot
#endif
			");",
			apet->m_index,
			pChar->m_index,
			apet->m_nSeal,
			apet->m_pProto->Index(),
			(const char*)apet->m_name,
			apet->m_level,
			apet->m_exp,
			apet->m_nRemainStat,
			apet->m_nPlusStr,
			apet->m_nPlusCon,
			apet->m_nPlusDex,
			apet->m_nPlusInt,
			apet->m_nSP,
			(const char*)petSkillIndex,
			(const char*)petSkillLevel,
			apet->m_hp,
			apet->m_mp,
			apet->GetFaith(),
			apet->GetStamina() 
#ifdef APET_AI
			, apet->m_cAI
			, apet->m_nAISlot
#endif
			);

			dbChar.SetQuery(m_buf);
			if (!dbChar.Update())
			{
				// 주인 변경
				sprintf( m_buf," UPDATE t_apets SET a_owner=%d, a_enable=1 WHERE a_index=%d;", pChar->m_index, apet->m_index );
				dbChar.SetQuery(m_buf);
				dbChar.Update();
				DBLOG << init(" CHANGE APET OWNER " , pChar )
					  << "APET INDEX : " << apet->m_index
					  << end;
				// 생성 실패
			}
		}

		// 펫 인벤 4칸 저장 //////////////////////////////////////////////////////
		memset(m_buf, 0, sizeof(m_buf));
		sprintf( m_buf, "DELETE FROM t_apets_inven WHERE a_apet_idx=%d", apet->m_index );
		dbChar.SetQuery(m_buf);
		if( !dbChar.Update() )
		{
			// 실폐 에러 
			DBLOG	<< init("DB ERROR : DELETE PET INVEN", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;			
		}

		memset(m_buf, 0, sizeof(m_buf));
		sprintf( m_buf, "INSERT INTO t_apets_inven ( a_apet_idx , " );
		int	pet_inven =0;
		char	invenSql[2048];
		for( pet_inven =0 ; pet_inven < APET_WEARPOINT ; pet_inven++ )
		{			
			sprintf( invenSql, " a_item_idx%d, a_plus%d, a_wear_pos%d, a_flag%d, a_used%d, a_used%d_2, a_serial%d, a_count%d,"
						" a_item%d_option0, a_item%d_option1, a_item%d_option2, a_item%d_option3, a_item%d_option4",
						pet_inven, pet_inven, pet_inven, pet_inven, pet_inven, pet_inven, pet_inven, pet_inven,
						pet_inven, pet_inven, pet_inven, pet_inven, pet_inven );
			strcat( m_buf, invenSql );
			if( pet_inven < APET_WEARPOINT -1 )
				strcat( m_buf, "," );				
		}
		strcat( m_buf, " ) VALUES ( " );
		IntCat( m_buf, apet->m_index );
		strcat( m_buf, "," );

		for( pet_inven =0 ; pet_inven < APET_WEARPOINT ; pet_inven++ )
		{	
			if( apet->m_wearing[pet_inven] == NULL )
			{
				sprintf( invenSql, "-1, 0, -1, 0, -1, -1, 'NONE', 0, 0, 0, 0, 0, 0" );
			}
			else
			{
				sprintf( invenSql,	" %d,"		// a_item_idx
									" %d, "		// a_plus
									" %d, "		// a_wear_pos
									" %d, "		// a_flag
									" %d, "		// a_used
									" %d, "		// a_used2
									" '%s', "		// a_serial
									" %d "		// a_count
									,
									apet->m_wearing[pet_inven]->m_idNum,					// a_item_idx
									apet->m_wearing[pet_inven]->m_plus,						// a_plus
									apet->m_wearing[pet_inven]->m_wearPos,					// a_wear_pos
									apet->m_wearing[pet_inven]->m_flag,						// a_flag
									apet->m_wearing[pet_inven]->m_used,						// a_used
									apet->m_wearing[pet_inven]->m_used_2,					// a_used2
									(const char*)apet->m_wearing[pet_inven]->m_serial,		// a_serial
									(int)(apet->m_wearing[pet_inven]->Count())				// a_count
									);
#ifdef MONSTER_RAID_SYSTEM
				int option_index=0;
				if( apet->m_wearing[pet_inven]->IsRareItem() )
				{
					if ( apet->m_wearing[pet_inven]->m_nOption == 2 )
					{
						strcat(invenSql, ",");
						IntCat(invenSql, apet->m_wearing[pet_inven]->m_nRareOptionIndex);
						strcat(invenSql, ",");
						IntCat(invenSql, apet->m_wearing[pet_inven]->m_nRareOptionBit);
						for (option_index = 2; option_index < MAX_ITEM_OPTION; option_index++)
						{
							strcat(invenSql, ",");
							IntCat(invenSql, 0);
						}
					}
					else
					{
						strcat( invenSql,	" ,0 , 0, 0, 0, 0 "	);
					}
				}
				else
#endif
				{
					strcat( invenSql,	" ,0 , 0, 0, 0, 0 "	);								
				}	
			}
			strcat( m_buf, invenSql );
			if( pet_inven < APET_WEARPOINT -1 )
				strcat( m_buf, "," );	
		}
		strcat( m_buf, ")" );
		dbChar.SetQuery(m_buf);
		if( !dbChar.Update() )
		{
			DBLOG	<< init("DB ERROR : INSERT PET INVEN", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;			
			// 실폐 에러 
		}
		//////////////////////////////////////////////////////////////////////////

#ifdef APET_AI
		if (apet->m_nAICount)
		{
			int nIndex = 0;
			char szAISql[2048];

			// 모두 DELETE 후에 다시 INSERT를 한다.대신 로그로 지워진 정보는 남김.
			// 펫AI 저장 - DELETE
			//for (nIndex = 0; nIndex < apet->m_nAISlot; nIndex++)
			//{
			//	DBLOG << init("APET_AI DELETE", pChar) << "charIndex" << delim << 
			//}
			memset(m_buf, 0, sizeof(m_buf));
			sprintf(m_buf, "DELETE FROM t_apets_ai WHERE a_char_idx = %d and a_apet_idx = %d", pChar->m_index, apet->m_index);

			//OutputDebugString(m_buf);
			dbChar.SetQuery(m_buf);
			if(!dbChar.Update())
			{
				DBLOG << init("DB ERROR : DELETE APET AI", pChar)
					<< mysql_error(dbChar.m_dbconn)
					<< end;			
			}
			
			// 펫AI 저장 - INSERT
			memset(m_buf, 0, sizeof(m_buf));
			sprintf(m_buf, "INSERT INTO t_apets_ai (a_char_idx, a_apet_idx ");

			memset(szAISql, 0, sizeof(szAISql));
			for (nIndex = 0; nIndex < apet->m_nAISlot; nIndex++)
			{
				sprintf(szAISql, ", a_item_idx%d, a_skill_idx%d, a_skill_type%d, a_use%d", nIndex, nIndex, nIndex, nIndex);
				strcat(m_buf, szAISql);
			}
			strcat(m_buf, ") VALUES (");
			IntCat(m_buf, pChar->m_index);
			strcat(m_buf, ", ");
			IntCat(m_buf, apet->m_index);
			
			for (nIndex = 0; nIndex < apet->m_nAISlot; nIndex++)
			{
				APET_AI_DATA * pAIData = &(apet->m_tAIData[nIndex]);
				sprintf(szAISql, ", %d, %d, %d, %d", pAIData->nItemNum, pAIData->nSkillIdx, pAIData->nSkillType, pAIData->cUse);
				strcat(m_buf, szAISql);
			}
			strcat(m_buf, ") ");

			dbChar.SetQuery(m_buf);
			//OutputDebugString(m_buf);
			if(!dbChar.Update())
			{
				DBLOG << init("DB ERROR : INSERT APET AI", pChar)
					<< mysql_error(dbChar.m_dbconn)
					<< end;			
			}
		}
#endif	// APET_AI
		apet = apet->m_pNextPet;
	}

	// 임시저장 애완동물 삭제 : a_enable == 2
	sprintf(m_buf, "DELETE FROM t_apets WHERE a_owner=%d AND a_enable=2", pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : DELETE PET DISABLED", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;
	}
#endif // ATTACK_PET

#ifdef MESSENGER_NEW
	dbChar.BeginTrans();
	sprintf(m_buf, "Delete from t_messenger_opt where a_char_idx = %d ", pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (dbChar.Update())
	{
		CLCString gIndexList(255+1);
		CLCString gNameList(255+1);

		if(pChar->m_Friend)
		{
			pChar->m_Friend->GetGroupIndexString(gIndexList, gNameList);
			sprintf(m_buf, "Insert into t_messenger_opt values ( %d, ' %s', ' %s', %d ) ", pChar->m_index, (const char*) gIndexList, (const char*) gNameList, pChar->m_Friend->GetChatColor() );
		}
		else
			sprintf(m_buf, "Insert into t_messenger_opt values ( %d, '', '', %d ) ", pChar->m_index, 0 );


		dbChar.SetQuery(m_buf);

		if( dbChar.Update() )
		{
			dbChar.Commit();
		}
		else
		{
			DBLOG	<< init("DB ERROR : DELETE MESSEGNER OPT", pChar)
					<< mysql_error(dbChar.m_dbconn)
					<< end;

			dbChar.Rollback();
		}
	}
	else
	{
		DBLOG	<< init("DB ERROR : DELETE MESSEGNER OPT", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;

		dbChar.Rollback();
	}

	CFriendMember* fMember = NULL;

	bool bsucc = true;
	dbChar.BeginTrans();

	for(i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if( !pChar->m_Friend )
			continue;

		fMember = pChar->m_Friend->GetFriendMember(i);

		if( !fMember )
			continue;

		sprintf(m_buf, " update t_friend0%d set a_group_index = %d where a_char_index = %d and a_friend_index = %d "
			, pChar->m_index % 10, fMember->GetGroup(), pChar->m_index, fMember->GetChaIndex() );

		dbChar.SetQuery(m_buf);
		if( !dbChar.Update() )
		{
			bsucc = false;
		}
	}

	if(bsucc)
		dbChar.Commit();
	else
		dbChar.Rollback();

	CLCString blockIndexList(255+1);
	CLCString blockNameList(255+1);
	pChar->GetBlockListString(blockIndexList, blockNameList);

	dbChar.BeginTrans();
	
	sprintf(g_buf, " delete from t_block_friend where a_char_idx = %d ", pChar->m_index );
	dbChar.SetQuery(g_buf);
	dbChar.Update();

	sprintf(g_buf, " insert into t_block_friend (a_char_idx, a_block_idx_list, a_block_name_list) values (%d, '%s', '%s') ", pChar->m_index, (const char*)blockIndexList, (const char*) blockNameList);
	dbChar.SetQuery(g_buf);
	if( !dbChar.Update() )
	{
		dbChar.Rollback();
	}
	else
		dbChar.Commit();
#endif

#ifdef EVENT_JPN_2007_NEWSERVER
	if (gserver.m_bEventJPN2007NewServer)
	{
		sprintf(m_buf, "UPDATE t_event_2007_newserver SET a_give=1 WHERE a_userindex=%d AND a_charindex=%d", d->m_index, d->m_pChar->m_index);
		dbChar.SetQuery(m_buf);
		dbChar.Update();
	}
#endif // EVENT_JPN_2007_NEWSERVER
	
#ifdef EVENT_GOMDORI_2007
	sprintf(m_buf, "INSERT INTO t_event_gomdori_2007 (a_char_index) VALUES (%d)", d->m_pChar->m_index);
	dbChar.SetQuery(m_buf);
	dbChar.Update();
	if (d->m_pChar->m_nEventGomdori2007Total > 0)
	{
		sprintf(m_buf, "UPDATE t_event_gomdori_2007 SET a_total=%d WHERE a_char_index=%d", d->m_pChar->m_nEventGomdori2007Total, d->m_pChar->m_index);
		dbChar.SetQuery(m_buf);
		dbChar.Update();
	}
	if (d->m_pChar->m_nEventGomdori2007FirstLose == 0)
	{
		sprintf(m_buf, "UPDATE t_event_gomdori_2007 SET a_first_lose=UNIX_TIMESTAMP(NOW()) WHERE a_char_index=%d AND a_first_lose=0", d->m_pChar->m_index);
		dbChar.SetQuery(m_buf);
		dbChar.Update();
	}
	if (d->m_pChar->m_nEventGomdori2007FirstWin == 0)
	{
		sprintf(m_buf, "UPDATE t_event_gomdori_2007 SET a_first_win=UNIX_TIMESTAMP(NOW()) WHERE a_char_index=%d AND a_first_win=0", d->m_pChar->m_index);
		dbChar.SetQuery(m_buf);
		dbChar.Update();
	}
#endif // EVENT_GOMDORI_2007

	return true;
}

bool CDBThread::OnUserDB(CDescriptor* d)
{
	// DB 열기
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	int slot0, slot1;
	int remain0, remain1;
	slot0 = slot1 = 0;
	remain0 = remain1 = 0;

#ifdef CHARSLOTEXT_CARD
	sprintf(m_buf, "select a_charslot0_new, a_charslot1_new from t_cashItemdate "
		"where a_portal_idx = %d ", d->m_index);
	cmd.SetQuery(m_buf);

	if( !cmd.Open() ) return 0;

	if( cmd.m_nrecords < 1)
	{
		sprintf(m_buf, "insert into t_cashItemdate (a_portal_idx, a_mempos_new, a_charslot0_new, a_charslot1_new, a_stashext_new) "
			"values (%d, 0, 0, 0, 0) ", d->m_index);
		cmd.SetQuery(m_buf);
		cmd.Update();
	}
	else
	{
		if( !cmd.MoveFirst() )
		{
			return 0;
		}
		cmd.GetRec("a_charslot0_new", slot0);
		cmd.GetRec("a_charslot1_new", slot1);
	}

	remain0 = (slot0 - gserver.m_gameTime);
	remain1 = (slot1 - gserver.m_gameTime);

	if (remain0 <= 0)
	{
		remain0 = 0;
	}
	else
	{
		remain0 = remain0 / OUTPUTDAY;
		if (remain0 == 0)
		{
			remain0 = 1;
		}
	}

	if (remain1 <= 0)
	{
		remain1 = 0;
	}
	else
	{	// 최소 남은 시간 1일
		remain1 = remain1 / OUTPUTDAY;
		if (remain1 == 0)
		{
			remain1 = 1;
		}

	}
#else//#ifdef CHARSLOTEXT_CARD
	remain0 = 0;
	remain1 = 0;
#endif//#ifdef CHARSLOTEXT_CARD


#ifdef FEEITEM
	if (PLAYMODE(d) == MSG_LOGIN_RE)
	{
		// 서버 이동일 경우 케릭터 슬롯 정보는 안 보낸다
		CNetMsg msg;
		DBCharEndMsg(msg, remain0, remain1, d->m_nPrepareSeed);
		d->m_dbOut.Add(msg, 99999);
		return true;
	}
#else // FEEITEM
	if (PLAYMODE(d) == MSG_LOGIN_RE)
	{
		// 서버 이동일 경우 케릭터 슬롯 정보는 안 보낸다
		CNetMsg msg;
		DBCharEndMsg(msg);
		d->m_dbOut.Add(msg, 99999);
		return true;
	}
#endif // FEEITEM

	if (d->m_bclosed) return false;
	if (d->m_idname[0] =='\0' || d->m_index < 0) 
		return false;

	struct __tagCharSlot
	{
		int  m_index;
		CNetMsg m_msg;
	} CharSlot[MAX_CHAR_COUNT];

	int slot;
	for (slot = 0; slot < MAX_CHAR_COUNT; slot++) {
		CharSlot[slot].m_index = -1;
	}

#ifdef ENABLE_CHARACTER_DELETE_DELAY
	// 해당 유저의 삭제 대기 중 캐릭중에 삭제 될 녀석들 지우기
	GetDateStr(m_te_buf);

	memset(m_buf,0,sizeof(m_buf));
	sprintf(m_buf,
		"SELECT * FROM t_characters"
		" WHERE a_user_index=%d"
		"   AND a_server=%d"
		"   AND a_enable=1"
		"   AND a_deletedelay!=0"
		"   AND a_deletedelay<=UNIX_TIMESTAMP(NOW())"
		, d->m_index
		, gserver.m_serverno
		);
	cmd.SetQuery( m_buf );
	if(cmd.Open() && cmd.MoveFirst() )
	{
		int i = 0;
		int nCharIndex =0;
		do
		{
			cmd.GetRec( "a_index"		, nCharIndex );
		
			if (IS_RUNNING_HELPER)
			{
				CNetMsg rmsg;
				HelperCharDelMsg(rmsg, nCharIndex);
				SEND_Q(rmsg, gserver.m_helper);
			}

			i++;

			if(i >= cmd.m_nrecords) break;

		}while( cmd.MoveNext());	
	}


	sprintf(m_buf,
		"UPDATE t_characters"
		" SET a_enable=0, a_datestamp='%s'"
		" WHERE a_user_index=%d"
		"   AND a_server=%d"
		"   AND a_enable=1"
		"   AND a_deletedelay!=0"
		"   AND a_deletedelay<=UNIX_TIMESTAMP(NOW())"
		, m_te_buf
		, d->m_index
		, gserver.m_serverno
		);
	cmd.SetQuery(m_buf);
	cmd.Update();

#endif // ENABLE_CHARACTER_DELETE_DELAY

	strcpy(m_buf,
		"SELECT *"
#ifdef ENABLE_CHARACTER_DELETE_DELAY
		", IF(a_deletedelay-UNIX_TIMESTAMP(NOW()) < 0, 0, a_deletedelay-UNIX_TIMESTAMP(NOW())) AS `a_delete_delay_reamin`"
#endif // ENABLE_CHARACTER_DELETE_DELAY
		" FROM t_characters"
		" WHERE a_user_index=");
	IntCat(m_buf, d->m_index, false);
	StrCat(m_buf, "AND a_server=");
	IntCat(m_buf, gserver.m_serverno, false);
	StrCat(m_buf, "AND a_enable=1 ORDER BY a_index");
	cmd.SetQuery(m_buf);
	if (!cmd.Open())
		return false;

	// 루프 : 있는 만큼
	slot = 0;
	while (cmd.MoveNext() && slot < MAX_CHAR_COUNT)
	{
		cmd.GetRec("a_index", CharSlot[slot].m_index);

		int wear[MAX_WEARING];
		const char* p = cmd.GetRec("a_wearing");
		int i;
		for (i = 0; i < MAX_WEARING; i++)
		{
			strcpy(m_te_buf, "-1");
			p = AnyOneArg(p, m_te_buf);

			if (i >= WEARING_SHOW_START && i <= WEARING_SHOW_END)
			{
				wear[i - WEARING_SHOW_START] = atoi(m_te_buf);
				CItemProto* proto = gserver.m_itemProtoList.FindIndex(wear[i - WEARING_SHOW_START]);
				if (!proto || proto->m_wearing < WEARING_HELMET || proto->m_wearing >= MAX_WEARING)
					wear[i - WEARING_SHOW_START] = -1;
			}
		}
// 050223 : bs : plus 효과를 위한 정보 추가
		int plus[MAX_WEARING];
		for (i = 0; i < MAX_WEARING; i++)
		{
			strcpy(m_te_buf, "0");
			p = AnyOneArg(p, m_te_buf);

			if (i >= WEARING_SHOW_START && i <= WEARING_SHOW_END)
			{
				plus[i - WEARING_SHOW_START] = atoi(m_te_buf);
				CItemProto* proto = gserver.m_itemProtoList.FindIndex(wear[i - WEARING_SHOW_START]);
				if (!proto || proto->m_wearing < WEARING_HELMET || proto->m_wearing >= MAX_WEARING)
					plus[i - WEARING_SHOW_START] = 0;
			}
		}		
// --- 050223 : bs : plus 효과를 위한 정보 추가
#ifdef NIGHT_SHADOW
		// 등짝 보이기 추가 
#endif //NIGHT_SHADOW

		// 캐릭터 삭제 딜레이
		int			nDeleteDelayRemain = 0;
#ifdef ENABLE_CHARACTER_DELETE_DELAY
		cmd.GetRec("a_delete_delay_reamin", nDeleteDelayRemain);
		if (nDeleteDelayRemain < 0)
			nDeleteDelayRemain = 0;
#endif // ENABLE_CHARACTER_DELETE_DELAY

		DBCharExistMsg(CharSlot[slot].m_msg,
			CharSlot[slot].m_index,
			cmd.GetRec("a_name"),
			cmd.GetRec("a_nick"),
			(char)atoi(cmd.GetRec("a_job")),
#ifdef ENABLE_CHANGEJOB
			(char)atoi(cmd.GetRec("a_job2")),
#else
			0,
#endif
			(char)atoi(cmd.GetRec("a_hair_style")),
			(char)atoi(cmd.GetRec("a_face_style")),
			atoi(cmd.GetRec("a_level")),
			ATOLL(cmd.GetRec("a_exp")),
			atoi(cmd.GetRec("a_skill_point")),
			atoi(cmd.GetRec("a_hp")),
			atoi(cmd.GetRec("a_max_hp")),
			atoi(cmd.GetRec("a_mp")),
			atoi(cmd.GetRec("a_max_mp")),
			wear,
			plus,
			nDeleteDelayRemain);

#ifdef NIGHT_SHADOW	// 90 Level 캐릭터 검사
		int checklevel = atoi(cmd.GetRec("a_level"));
		if( checklevel >= 90 )
			d->m_bCreate90Lv = true;
#endif //NIGHT_SHADOW

		slot++;
	}
	// 읽은 것 쓰기
	if (slot > 0)
	{
		int i;
		for (i = 0; i < slot; i++)
		{
			if (CharSlot[i].m_index == -1) continue;
			d->m_dbOut.Add(CharSlot[i].m_msg, 99999);
		}
	}

	CNetMsg endmsg;
#ifdef FEEITEM
	DBCharEndMsg(endmsg, remain0, remain1, d->m_nPrepareSeed);
#else // FEEITEM
	DBCharEndMsg(endmsg);
#endif // FEEITEM
	d->m_dbOut.Add(endmsg, 99999);

	return true;
}

bool CDBThread::OnNewCharDB(CDescriptor* d)
{
	if (d->m_bclosed) return false;
	if (!d->m_pChar) return false;

	int maxCharCount = MAX_CHAR_COUNT;
	
	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);
#ifdef CHARSLOTEXT_CARD

	sprintf(m_buf, "select a_charslot0_new , a_charslot1_new from t_cashItemdate where a_portal_idx = %d", d->m_index);
	dbChar.SetQuery(m_buf);

	int slot0, slot1, remain0, remain1;
	slot0 = slot1 = 0;

	if( !dbChar.Open() ) return 0;

	if( dbChar.MoveFirst() )
	{
		dbChar.GetRec(CASH_ITEM_DATE_FIELD_CHAR_SLOT0, slot0);
		dbChar.GetRec(CASH_ITEM_DATE_FIELD_CHAR_SLOT1, slot1);
	}

	remain0 = slot0 - gserver.m_gameTime;
	remain1 = slot1 - gserver.m_gameTime;

	if( remain0 < 0) 
		remain0 = 0;
	if( remain1 < 0)
		remain1 = 0;

	if( !remain0 ) maxCharCount--;
	if( !remain1 ) maxCharCount--;
#else//#ifdef CHARSLOTEXT_CARD
	maxCharCount = 4;
#endif//#ifdef CHARSLOTEXT_CARD


	//char t_name[100];

	//BackSlash(t_name, d->m_pChar->m_name);

	strcpy(m_buf, "SELECT a_index FROM t_characters WHERE a_name='");
	strcat(m_buf, d->m_pChar->m_name);
	strcat(m_buf, "' AND a_server=");
	IntCat(m_buf, gserver.m_serverno, false);

	dbChar.SetQuery(m_buf);
	dbChar.Open();

	if (dbChar.m_nrecords > 0)
	{
		// 캐릭터 이름 중복시에
		CNetMsg dbmsg;
		FailMsg(dbmsg, MSG_FAIL_DB_ALREADY_NAME);
		d->m_dbOut.Add(dbmsg, 99999);
		return false;
	}

	//BackSlash(t_name, d->m_pChar->m_name);

	strcpy(m_buf, "SELECT a_index FROM t_characters WHERE a_nick='");
	strcat(m_buf, d->m_pChar->m_name);
	strcat(m_buf, "' AND a_server=");
	IntCat(m_buf, gserver.m_serverno, false);

	dbChar.SetQuery(m_buf);
	dbChar.Open();

	if (dbChar.m_nrecords > 0)
	{
		// 캐릭터 이름 중복시에
		CNetMsg dbmsg;
		FailMsg(dbmsg, MSG_FAIL_DB_ALREADY_NAME);
		d->m_dbOut.Add(dbmsg, 99999);
		return false;
	}

	strcpy(m_buf, "SELECT a_index, a_level FROM t_characters WHERE a_user_index=");
	IntCat(m_buf, d->m_index, false);
	strcat(m_buf, " AND a_server=");
	IntCat(m_buf, gserver.m_serverno, false);
	strcat(m_buf, " AND a_enable=1");
	dbChar.SetQuery(m_buf);

	bool ret = true;
	try
	{
		if (!dbChar.Open())	// 데이터베이스 오류
			throw MSG_FAIL_DB_NEW_CHAR;
		if (dbChar.m_nrecords >= maxCharCount)	// 캐릭터 가득 존재
			throw MSG_FAIL_DB_FULL_CHAR;

#ifdef EVENT_PHOENIX   // 피닉스 이벤트  // 계정 내에 100레벨 이상의 캐릭터가 하나라도 존재해야 피닉스 가능해지도록 생성
		// 100 레벨이상인 캐릭터가 있다는 것을 표시
		bool level_100_up = false;
		int query_count1 = dbChar.m_nrecords;
		if (query_count1)
		{
			while (dbChar.MoveNext())
			{
				int level;
				dbChar.GetRec("a_level", level);

				// 피닉스 이벤트 승락, 레벨이 100 이상인 캐릭터가 하나라도 있으면
				if( level >= 100 ) 
				{
					level_100_up = true;
					break;
				}

			}
		}
#endif  // EVENT_PHOENIX

		GetDateStr(m_te_buf);

		dbChar.BeginTrans();

		strcpy(m_buf,
			"INSERT INTO t_characters "
			"(a_user_index, a_server, "
			"a_name, "
			"a_nick, "
			"a_was_x, a_was_z, a_was_h, a_was_r, a_was_zone, a_was_area, "
			"a_active_skill_index, a_active_skill_level, a_passive_skill_index, a_passive_skill_level, a_etc_skill_index, a_etc_skill_level, "
			"a_wearing, "
#ifdef QUEST_DATA_EXTEND
			// 별도 저장
#else
			"a_quest_index, a_quest_value, "
#endif // QUEST_DATA_EXTEND
			"a_sskill, "
			"a_datestamp, a_createdate, "

#ifdef EVENT_PHOENIX   // 피닉스 이벤트  //신규 캐릭터이면 피닉스가 가능해진다. 기존의 캐릭터는 불가능
			"a_phoenix, "
#endif // EVENT_PHOENIX

			"a_job, a_hair_style, a_face_style, a_str, a_dex, a_int, a_con, "
			"a_hp, a_max_hp, a_mp, a_max_mp, "
			"a_skill_point) "
			
			"VALUES "
			
			"(");
		
		IntCat(m_buf, d->m_index);					strcat(m_buf, ",");
		IntCat(m_buf, gserver.m_serverno);			strcat(m_buf, ",");

		StrCat(m_buf, "'");	strcat(m_buf, d->m_pChar->m_name);	strcat(m_buf, "',");
		StrCat(m_buf, "'");	strcat(m_buf, d->m_pChar->m_name);	strcat(m_buf, "',");

		StrCat(m_buf, "0.0000, 0.0000, 0.0000, 0.0000, -1, -1,");

		// 기본 스킬 주기
		// 기본 착용 정보 : 투구 상의 무기 하의 방패 장갑 신발
		switch (d->m_pChar->m_job)
		{
		case JOB_TITAN:
			StrCat(m_buf, "' 1', ' 1', '', '', '', '',");
			StrCat(m_buf, " ' 72 2 12 3 -1 4 8', ");
			break;

		case JOB_KNIGHT:
			StrCat(m_buf, "' 14', ' 1', '', '', '', '',");
			StrCat(m_buf, " ' 75 34 48 38 49 39 41', ");
			break;

		case JOB_HEALER:
			StrCat(m_buf, "' 27 31', ' 1 1', '', '', '', '',");
			StrCat(m_buf, " ' 78 26 50 28 -1 30 32', ");
			break;

		case JOB_MAGE:
			StrCat(m_buf, "' 53', ' 1', '', '', '', '',");
			StrCat(m_buf, " '24 266 356 18 -1 22 20', ");
			break;

		case JOB_ROGUE:
			StrCat(m_buf, "' 116', ' 1', '', '', '', '',");
			StrCat(m_buf, " ' 552 524 528 525 -1 527 526', ");
			break;

		case JOB_SORCERER:
			StrCat(m_buf, "' 292', ' 1', '', '', '', '',");
			StrCat(m_buf, " ' 1040 1000 988 1010 -1 1020 1030', ");
			break;

#ifdef NIGHT_SHADOW
		case JOB_NIGHTSHADOW:			// 기본 스킬 , 착용장비
			StrCat(m_buf, "' 292', ' 1', '', '', '', '',");
			StrCat(m_buf, " ' 1040 1000 988 1010 -1 1020 1030', ");
			break;
#endif //NIGHT_SHADOW

		default:
			StrCat(m_buf, "'', '', '', '', '', '',");
			StrCat(m_buf, " '', ");
			break;
		}

#ifdef QUEST_DATA_EXTEND
		// 별도 저장
#else
		// a_quest For Tutorial
		strcat(m_buf, "' 45', ' 0 0 0', ");
#endif // QUEST_DATA_EXTEND

		// a_sskill
		strcat(m_buf, "' 1 1 5 1 9 1 13 1 18 1 23 1 44 1', '");

		strcat(m_buf, m_te_buf);	strcat(m_buf, "', '");
		strcat(m_buf, m_te_buf);	strcat(m_buf, "',");

#ifdef EVENT_PHOENIX   // 피닉스 이벤트  //신규 캐릭터이면 피닉스가 가능해진다. 기존의 캐릭터는 불가능
		if( level_100_up )
		{
			IntCat(m_buf, 0);			strcat(m_buf, ",");	
		}
		else
		{
			IntCat(m_buf, -1);			strcat(m_buf, ",");	
		}
#endif // EVENT_PHOENIX

		IntCat(m_buf, d->m_pChar->m_job);			strcat(m_buf, ",");
		IntCat(m_buf, d->m_pChar->m_hairstyle);		strcat(m_buf, ",");
		IntCat(m_buf, d->m_pChar->m_facestyle);		strcat(m_buf, ",");
		IntCat(m_buf, d->m_pChar->m_str);			strcat(m_buf, ",");
		IntCat(m_buf, d->m_pChar->m_dex);			strcat(m_buf, ",");
		IntCat(m_buf, d->m_pChar->m_int);			strcat(m_buf, ",");
		IntCat(m_buf, d->m_pChar->m_con);			strcat(m_buf, ",");

		IntCat(m_buf, d->m_pChar->m_dbHP);			strcat(m_buf, ",");
		IntCat(m_buf, d->m_pChar->m_dbHP);			strcat(m_buf, ",");
		IntCat(m_buf, d->m_pChar->m_dbMP);			strcat(m_buf, ",");
		IntCat(m_buf, d->m_pChar->m_dbMP);			strcat(m_buf, ",");

		// 초기 스킬 포인트
		IntCat(m_buf, 25 * 10000);					strcat(m_buf, ")");

		dbChar.SetQuery(m_buf);
		if (!dbChar.Update())
			throw MSG_FAIL_DB_UNKNOWN;

		int charidx = dbChar.insertid();
#ifdef NEW_GUILD
		sprintf( m_buf, "INSERT INTO t_characters_guildpoint ( a_char_index	) "
			" VALUES (%d )", charidx );
		dbChar.SetQuery( m_buf );
		if( !dbChar.Update() )
			throw MSG_FAIL_DB_UNKNOWN;		
#endif // NEW_GUILD

#ifdef QUEST_DATA_EXTEND
		sprintf(m_buf,
				"INSERT INTO t_questdata%02d"
				" (a_char_index,a_quest_index,a_state,a_value0,a_value1,a_value2)"
				" VALUES"
				" (%d,%d,%d,%d,%d,%d)",
				charidx % 10,
				charidx,
				45,
				QUEST_STATE_RUN,
				0,
				0,
				0
			);
		dbChar.SetQuery(m_buf);
		if (!dbChar.Update())
		{
			DBLOG	<< init("DB ERROR : INSERT QUEST DATA (NEW CHAR)", d->m_pChar)
					<< mysql_error(dbChar.m_dbconn)
					<< end;
		}
#endif // QUEST_DATA_EXTEND

		// 기본 아이템 주기
#if defined(LC_JPN) && defined(CREATE_PULS_ONE)
		static const int titancount = 9;
		static const int knightcount = 10;
		static const int healercount = 9;
		static const int magecount = 9;
		static const int roguecount = 9;
		static const int sorcerercount = 9;
		static int itemsForTitan[4][titancount] = {
			{ 72,  2, 12,  3,  4,  8, 19/*나스*/, 2494/* 잘익은 수박 */, 558/* 핸즈 엑스*/ },					// item_idx
			{  0,  1,  2,  3,  5,  6, -1,  -1, -1},														// wearpos
			{  1,  1,  1,  1,  1,  1, 100, 2, 1}														// count
		};

		static int itemsForKnight[3][knightcount] = {
			{ 75, 34, 48, 38, 49, 39, 41, 19/*나스*/, 2494/* 잘익은 수박 */, 459/* 나이트 듀얼소드*/  },		// item_idx
			{  0,  1,  2,  3,  4,  5,  6, -1, -1, -1 },													// wearpos
			{  1,  1,  1,  1,  1,  1,  1, 100, 2, 1}													// count
		};

		static int itemsForHealer[3][healercount] = {
			{ 78, 26, 50, 28, 30, 32, 19/*나스*/, 2494/* 잘익은 수박 */, 599/* 토트 완드*/  },					// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1, -1 },														// wearpos
			{  1,  1,  1,  1,  1,  1, 100, 2, 1 }														// count
		};

		static int itemsForMage[3][magecount] = {
			{ 24,266,356, 18, 22, 20, 19/*나스*/, 2494/* 잘익은 수박 */, 600/* 크루크 스테프 */  },				// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1, -1 },														// wearpos
			{  1,  1,  1,  1,  1,  1, 100, 2, 1 }														// count
		};

		static int itemsForRogue[3][roguecount] = {
			{552,524,528,525,527,526, 19/*나스*/, 2494/* 잘익은 수박 */, 530/* 블릿 보우 */ },					// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1, -1 },														// wearpos
			{  1,  1,  1,  1,  1,  1, 100, 2, 1 }														// count
		};

		static int itemsForSorcerer[3][sorcerercount] = {
			{ 1040,1000,988, 1010, 1020, 1030, 19/*나스*/, 2494/* 잘익은 수박 */,  976/* 실런 사이드 */ },		// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1, -1 },														// wearpos
			{  1,  1,  1,  1,  1,  1, 100, 2, 1 }														// count
		};
#elif defined(CREATE_LIGHT_USER_EVENT)
		static const int titancount = 9;
		static const int knightcount = 10;
		static const int healercount = 9;
		static const int magecount = 9;
		static const int roguecount = 9;
		static const int sorcerercount = 9;
		static int itemsForTitan[3][titancount] = {
			{ 72,  2, 12,  3,  4,  8, 19/*나스*/, 2658, 2659 },					// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1, -1 },								// wearpos
			{  1,  1,  1,  1,  1,  1, 100, 5, 5 }								// count
		};

		static int itemsForKnight[3][knightcount] = {
			{ 75, 34, 48, 38, 49, 39, 41, 19/*나스*/, 2658, 2659 },				// item_idx
			{  0,  1,  2,  3,  4,  5,  6, -1, -1, -1 },							// wearpos
			{  1,  1,  1,  1,  1,  1,  1, 100, 5, 5 }							// count
		};

		static int itemsForHealer[3][healercount] = {
			{ 78, 26, 50, 28, 30, 32, 19/*나스*/, 2658, 2659 },					// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1, -1 },								// wearpos
			{  1,  1,  1,  1,  1,  1, 100, 5, 5 }								// count
		};

		static int itemsForMage[3][magecount] = {
			{ 24,266,356, 18, 22, 20, 19/*나스*/, 2658, 2659 },					// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1, -1 },								// wearpos
			{  1,  1,  1,  1,  1,  1, 100, 5, 5 }								// count
		};

		static int itemsForRogue[3][roguecount] = {
			{552,524,528,525,527,526, 19/*나스*/, 2658, 2659 },					// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1, -1 },								// wearpos
			{  1,  1,  1,  1,  1,  1, 100, 5, 5 }								// count
		};

		static int itemsForSorcerer[3][sorcerercount] = {
			{ 1040,1000,988, 1010, 1020, 1030, 19/*나스*/, 2658, 2659 },		// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1, -1 },								// wearpos
			{  1,  1,  1,  1,  1,  1, 100, 5, 5 }								// count
		};
#else
		static const int titancount = 7;
		static const int knightcount = 8;
		static const int healercount = 7;
		static const int magecount = 7;
		static const int roguecount = 7;
		static const int sorcerercount = 7;
		static int itemsForTitan[4][titancount] = {
			{ 72,  2, 12,  3,  4,  8, 19/*나스*/ },					// item_idx
			{  0,  1,  2,  3,  5,  6, -1 },					// wearpos
			{  1,  1,  1,  1,  1,  1, 100 }					// count
		};

		static int itemsForKnight[3][knightcount] = {
			{ 75, 34, 48, 38, 49, 39, 41, 19/*나스*/  },				// item_idx
			{  0,  1,  2,  3,  4,  5,  6, -1 },				// wearpos
			{  1,  1,  1,  1,  1,  1,  1, 100}				// count
		};

		static int itemsForHealer[3][healercount] = {
			{ 78, 26, 50, 28, 30, 32, 19/*나스*/  },					// item_idx
			{  0,  1,  2,  3,  5,  6, -1 },					// wearpos
			{  1,  1,  1,  1,  1,  1, 100 }					// count
		};

		static int itemsForMage[3][magecount] = {
			{ 24,266,356, 18, 22, 20, 19/*나스*/  },					// item_idx
			{  0,  1,  2,  3,  5,  6, -1 },					// wearpos
			{  1,  1,  1,  1,  1,  1, 100 }					// count
		};

		static int itemsForRogue[3][roguecount] = {
			{552,524,528,525,527,526, 19/*나스*/  },					// item_idx
			{  0,  1,  2,  3,  5,  6, -1 },					// wearpos
			{  1,  1,  1,  1,  1,  1, 100 }					// count
		};

		static int itemsForSorcerer[3][sorcerercount] = {
			{ 1040,1000,988, 1010, 1020, 1030, 19/*나스*/  },		// item_idx
			{  0,  1,  2,  3,  5,  6, -1 },					// wearpos
			{  1,  1,  1,  1,  1,  1, 100 }					// count
		};
#endif // LC_JPN 

		int* itemidx = NULL;
		int* itemwearpos = NULL;
		int* itemcount = NULL;
		int defitemcount = 0;

		switch (d->m_pChar->m_job)
		{
		case JOB_TITAN:
			itemidx = itemsForTitan[0];
			itemwearpos = itemsForTitan[1];
			itemcount = itemsForTitan[2];
			defitemcount = titancount;
			break;

		case JOB_KNIGHT:
			itemidx = itemsForKnight[0];
			itemwearpos = itemsForKnight[1];
			itemcount = itemsForKnight[2];
			defitemcount = knightcount;
			break;

		case JOB_HEALER:
			itemidx = itemsForHealer[0];
			itemwearpos = itemsForHealer[1];
			itemcount = itemsForHealer[2];
			defitemcount = healercount;
			break;

		case JOB_MAGE:
			itemidx = itemsForMage[0];
			itemwearpos = itemsForMage[1];
			itemcount = itemsForMage[2];
			defitemcount = magecount;
			break;

		case JOB_ROGUE:
			itemidx = itemsForRogue[0];
			itemwearpos = itemsForRogue[1];
			itemcount = itemsForRogue[2];
			defitemcount = roguecount;
			break;

		case JOB_SORCERER:
			itemidx = itemsForSorcerer[0];
			itemwearpos = itemsForSorcerer[1];
			itemcount = itemsForSorcerer[2];
			defitemcount = sorcerercount;
			break;

		default:
			defitemcount = 0;
		}

		int itab, irow, icol;

		char sql1[400];		// INSERT INTO t_inven (a_char_index, a_tab_idx, a_row_idx
		char sql2[400];		// ) VALUES (char, tab, row
#ifdef PARTY_RECALL_NEW		
		char sql3[1024];		// a_item_idx, a_plus, a_wear_pos, a_flag, a_serial, a_count, a_used, a_used_2
#else
		char sql3[1024];		// a_item_idx, a_plus, a_wear_pos, a_flag, a_serial, a_count, a_used
#endif	// PARTY_RECALL_NEW	
		char sql4[1024];		// idx, 0, wearpos, 0, str, count, used
		char tmp[30];

		itab = irow = icol = 0;
		strcpy(sql1, "INSERT INTO t_inven");
		IntCat0(sql1, charidx % 10, 2, false);
		strcat(sql1, " (a_char_idx, a_tab_idx, a_row_idx");
		strcpy(sql2, ") VALUES (");
		IntCat(sql2, charidx, false); strcat(sql2, ", 0, 0");
		sql3[0] = '\0';
		sql4[0] = '\0';

		int i;
		for (i = 0; i < defitemcount; i++)
		{			
			CItemProto* itemproto = gserver.m_itemProtoList.FindIndex(itemidx[i]);
			int used = -1;
#ifdef COMPOSITE_TIME			
			int used_2 = 0;
#endif	// COMPOSITE_TIME
			if (itemproto != NULL)
				used = itemproto->m_maxUse;

			strcat(sql3, ", a_item_idx");	IntCat(sql3, icol, false);	strcat(sql3, ", ");
			strcat(sql3, "a_plus");			IntCat(sql3, icol, false);	strcat(sql3, ", ");
			strcat(sql3, "a_wear_pos");		IntCat(sql3, icol, false);	strcat(sql3, ", ");
			strcat(sql3, "a_flag");			IntCat(sql3, icol, false);	strcat(sql3, ", ");
			strcat(sql3, "a_serial");		IntCat(sql3, icol, false);	strcat(sql3, ", ");
			strcat(sql3, "a_count");		IntCat(sql3, icol, false);	strcat(sql3, ", ");
			strcat(sql3, "a_used");			IntCat(sql3, icol, false);
#ifdef COMPOSITE_TIME
			strcat(sql3, ", ");
			strcat(sql3, "a_used");			IntCat(sql3, icol, false);	strcat(sql3, "_2");
#endif	// COMPOSITE_TIME
			strcat(sql4, ", ");				IntCat(sql4, itemidx[i], false);
			strcat(sql4, ", ");				IntCat(sql4, 0, false);
			strcat(sql4, ", ");				IntCat(sql4, itemwearpos[i], false);
			strcat(sql4, ", ");				IntCat(sql4, 0, false);
			strcat(sql4, ", '");			GetSerial(tmp, itemidx[i]); strcat(sql4, tmp);
			strcat(sql4, "', ");			IntCat(sql4, itemcount[i], false);
			strcat(sql4, ", ");				IntCat(sql4, used, false);
#ifdef COMPOSITE_TIME
			strcat(sql4, ", ");				IntCat(sql4, used_2, false);
#endif	// COMPOSITE_TIME

			icol++;
			if (icol == 5)
			{
				irow++;
				icol = 0;

				strcpy(m_buf, sql1);
				strcat(m_buf, sql3);
				strcat(m_buf, sql2);
				strcat(m_buf, sql4);
				strcat(m_buf, ")");

				dbChar.SetQuery(m_buf);
				if (!dbChar.Update())
					throw MSG_FAIL_DB_UNKNOWN;
				strcpy(sql2, ") VALUES (");
				IntCat(sql2, charidx, false); strcat(sql2, ", 0, "); IntCat(sql2, irow, false);
				sql3[0] = '\0';
				sql4[0] = '\0';
			}
		}
		if (strlen(sql3) != 0)
		{
			irow++;
			icol = 0;

			strcpy(m_buf, sql1);
			strcat(m_buf, sql3);
			strcat(m_buf, sql2);
			strcat(m_buf, sql4);
			strcat(m_buf, ")");

			dbChar.SetQuery(m_buf);
			if (!dbChar.Update())
				throw MSG_FAIL_DB_UNKNOWN;
		}

//////////////////////////

#ifdef EVENT_TREASUREBOX_RED
		itab = irow = icol = 0;
		strcpy(sql1, "INSERT INTO t_inven");
		IntCat0(sql1, charidx % 10, 2, false);
		strcat(sql1, " (a_char_idx, a_tab_idx, a_row_idx");
		strcpy(sql2, ") VALUES (");
		IntCat(sql2, charidx, false); strcat(sql2, ", 1, 1");
		sql3[0] = '\0';
		sql4[0] = '\0';
		
		CItemProto * itemproto_red = gserver.m_itemProtoList.FindIndex(2660);
		int used_red = -1;
#ifdef COMPOSITE_TIME
		int used_2_red = 0;
#endif	// COMPOSITE_TIME
		if (itemproto_red != NULL)
			used_red = itemproto_red->m_maxUse;
		
		strcat(sql3, ", a_item_idx");	IntCat(sql3, icol, false);	strcat(sql3, ", ");
		strcat(sql3, "a_plus");			IntCat(sql3, icol, false);	strcat(sql3, ", ");
		strcat(sql3, "a_wear_pos");		IntCat(sql3, icol, false);	strcat(sql3, ", ");
		strcat(sql3, "a_flag");			IntCat(sql3, icol, false);	strcat(sql3, ", ");
		strcat(sql3, "a_serial");		IntCat(sql3, icol, false);	strcat(sql3, ", ");
		strcat(sql3, "a_count");		IntCat(sql3, icol, false);	strcat(sql3, ", ");
		strcat(sql3, "a_used");			IntCat(sql3, icol, false);
#ifdef COMPOSITE_TIME
			strcat(sql3, ", ");
			strcat(sql3, "a_used");			IntCat(sql3, icol, false);	strcat(sql3, "_2");
#endif	// COMPOSITE_TIME
			
		strcat(sql4, ", ");				IntCat(sql4, 2660, false);
		strcat(sql4, ", ");				IntCat(sql4, 0, false);
		strcat(sql4, ", ");				IntCat(sql4, -1, false);
		strcat(sql4, ", ");				IntCat(sql4, 0, false);
		strcat(sql4, ", '");			GetSerial(tmp, 2660); strcat(sql4, tmp);
		strcat(sql4, "', ");			IntCat(sql4, 1, false);
		strcat(sql4, ", ");				IntCat(sql4, used_red, false);
#ifdef COMPOSITE_TIME
		strcat(sql4, ", ");				IntCat(sql4, used_2_red, false);
#endif	// COMPOSITE_TIME
			
		if (strlen(sql3) != 0)
		{		
			strcpy(m_buf, sql1);
			strcat(m_buf, sql3);
			strcat(m_buf, sql2);
			strcat(m_buf, sql4);
			strcat(m_buf, ")");

			dbChar.SetQuery(m_buf);
			if (!dbChar.Update())
				throw MSG_FAIL_DB_UNKNOWN;
		}		
#endif // EVENT_TREASUREBOX_RED

#ifdef EVENT_TREASUREBOX
		itab = irow = icol = 0;
		strcpy(sql1, "INSERT INTO t_inven");
		IntCat0(sql1, charidx % 10, 2, false);
		strcat(sql1, " (a_char_idx, a_tab_idx, a_row_idx");
		strcpy(sql2, ") VALUES (");
		IntCat(sql2, charidx, false); strcat(sql2, ", 1, 0");
		sql3[0] = '\0';
		sql4[0] = '\0';
		
		CItemProto* itemproto = gserver.m_itemProtoList.FindIndex(760);
		int used = -1;
#ifdef COMPOSITE_TIME
		int used_2 = 0;
#endif	// COMPOSITE_TIME
		if (itemproto != NULL)
			used = itemproto->m_maxUse;
		
		strcat(sql3, ", a_item_idx");	IntCat(sql3, icol, false);	strcat(sql3, ", ");
		strcat(sql3, "a_plus");			IntCat(sql3, icol, false);	strcat(sql3, ", ");
		strcat(sql3, "a_wear_pos");		IntCat(sql3, icol, false);	strcat(sql3, ", ");
		strcat(sql3, "a_flag");			IntCat(sql3, icol, false);	strcat(sql3, ", ");
		strcat(sql3, "a_serial");		IntCat(sql3, icol, false);	strcat(sql3, ", ");
		strcat(sql3, "a_count");		IntCat(sql3, icol, false);	strcat(sql3, ", ");
		strcat(sql3, "a_used");			IntCat(sql3, icol, false);
#ifdef COMPOSITE_TIME
			strcat(sql3, ", ");
			strcat(sql3, "a_used");			IntCat(sql3, icol, false);	strcat(sql3, "_2");
#endif	// COMPOSITE_TIME
			
		strcat(sql4, ", ");				IntCat(sql4, 760, false);
		strcat(sql4, ", ");				IntCat(sql4, 0, false);
		strcat(sql4, ", ");				IntCat(sql4, -1, false);
		strcat(sql4, ", ");				IntCat(sql4, 0, false);
		strcat(sql4, ", '");			GetSerial(tmp, 760); strcat(sql4, tmp);
		strcat(sql4, "', ");			IntCat(sql4, 1, false);
		strcat(sql4, ", ");				IntCat(sql4, used, false);
#ifdef COMPOSITE_TIME
		strcat(sql4, ", ");				IntCat(sql4, used_2, false);
#endif	// COMPOSITE_TIME
			
		if (strlen(sql3) != 0)
		{		
			strcpy(m_buf, sql1);
			strcat(m_buf, sql3);
			strcat(m_buf, sql2);
			strcat(m_buf, sql4);
			strcat(m_buf, ")");

			dbChar.SetQuery(m_buf);
			if (!dbChar.Update())
				throw MSG_FAIL_DB_UNKNOWN;
		}		

#endif

#ifdef HANARO_EVENT
		{
			int* pItemIndex = NULL;
			int* pItemCount = NULL;
			int  nItemCount = 0;
			int  nItemUsed  = -1;
			
			if (strcmp(d->m_hanaroCode, "HF") == 0 || strcmp(d->m_hanaroCode, "hf") == 0)
			{
				// 기본 지급 : HP 스틸 포션, 플래티늄 아이리스 축복, 플레티늄 숙련의 묘약
				// 하나포스 : 수박, 참외
				static const int nHFCount = 5;
				static int nHFItem[2][nHFCount] = {
					{2667, 2658, 2659, 836, 837},
					{   5,    5,    5,   5,   5}
				};
				
				nItemCount = nHFCount;
				pItemIndex = nHFItem[0];
				pItemCount = nHFItem[1];
			} // HF
			else if (strcmp(d->m_hanaroCode, "SK") == 0 || strcmp(d->m_hanaroCode, "sk") == 0)
			{
				// 기본 지급 : HP 스틸 포션, 플래티늄 아이리스 축복, 플레티늄 숙련의 묘약
				// SK브로드밴드 : 결합 주문서, 각 클래스별 교복 코스튬 세트
				static const int nSKCount = 8;
				static int nTitanItem[2][nSKCount] = {
					{2667, 2658, 2659, 2664, 1422, 1423, 1424, 1425},
					{   5,    5,    5,    5,    1,    1,    1,    1}
				};
				static int nNightItem[2][nSKCount] = {
					{2667, 2658, 2659, 2664, 1426, 1427, 1428, 1429},
					{   5,    5,    5,    5,    1,    1,    1,    1}
				};
				static int nHealerItem[2][nSKCount] = {
					{2667, 2658, 2659, 2664, 1430, 1431, 1432, 1433},
					{   5,    5,    5,    5,    1,    1,    1,    1}
				};
				static int nMageItem[2][nSKCount] = {
					{2667, 2658, 2659, 2664, 1434, 1435, 1436, 1437},
					{   5,    5,    5,    5,    1,    1,    1,    1}
				};
				static int nRogueItem[2][nSKCount] = {
					{2667, 2658, 2659, 2664, 1438, 1439, 1440, 1441},
					{   5,    5,    5,    5,    1,    1,    1,    1}
				};
				static int nSorcererItem[2][nSKCount] = {
					{2667, 2658, 2659, 2664, 1442, 1443, 1444, 1445},
					{   5,    5,    5,    5,    1,    1,    1,    1}
				};
				
				nItemCount = nSKCount;
				nItemUsed  = gserver.m_gameTime + 30 * 24 * 60 * 60;	// 1개월
				switch (d->m_pChar->m_job)
				{
				case JOB_TITAN:
					pItemIndex = nTitanItem[0];
					pItemCount = nTitanItem[1];
					break;
				case JOB_KNIGHT:
					pItemIndex = nNightItem[0];
					pItemCount = nNightItem[1];
					break;
				case JOB_HEALER:
					pItemIndex = nHealerItem[0];
					pItemCount = nHealerItem[1];
					break;
				case JOB_MAGE:
					pItemIndex = nMageItem[0];
					pItemCount = nMageItem[1];
					break;
				case JOB_ROGUE:
					pItemIndex = nRogueItem[0];
					pItemCount = nRogueItem[1];
					break;
				case JOB_SORCERER:
					pItemIndex = nSorcererItem[0];
					pItemCount = nSorcererItem[1];
					break;
				default:
					nItemCount = 0;
				}
			} // SK
			else
			{
				// 기본 지급 : HP 스틸 포션, 플래티늄 아이리스 축복, 플레티늄 숙련의 묘약
				static int nItemList[2][3] = {
					{2667, 2658, 2659},
					{   5,    5,    5}
				};

				nItemCount = 3;
				pItemIndex = nItemList[0];
				pItemCount = nItemList[1];
			}
			
			itab = icol = 0;
			irow = 2;
			strcpy(sql1, "INSERT INTO t_inven");
			IntCat0(sql1, charidx % 10, 2, false);
			strcat(sql1, " (a_char_idx, a_tab_idx, a_row_idx");
			strcpy(sql2, ") VALUES (");
			IntCat(sql2, charidx, false); strcat(sql2, ", 0, "); IntCat(sql2, irow, false);
			
			memset(m_buf, 0x00, sizeof(m_buf));
			sql3[0] = '\0';
			sql4[0] = '\0';
			
			for (i = 0; i < nItemCount; i++)
			{
				CItemProto* itemproto = gserver.m_itemProtoList.FindIndex(pItemIndex[i]);

				if (!itemproto)
					continue;

#ifdef COMPOSITE_TIME
				int used_2 = 0;
#endif	// COMPOSITE_TIME

				strcat(sql3, ", a_item_idx");	IntCat(sql3, icol, false);	strcat(sql3, ", ");
				strcat(sql3, "a_plus");			IntCat(sql3, icol, false);	strcat(sql3, ", ");
				strcat(sql3, "a_wear_pos");		IntCat(sql3, icol, false);	strcat(sql3, ", ");
				strcat(sql3, "a_flag");			IntCat(sql3, icol, false);	strcat(sql3, ", ");
				strcat(sql3, "a_serial");		IntCat(sql3, icol, false);	strcat(sql3, ", ");
				strcat(sql3, "a_count");		IntCat(sql3, icol, false);	strcat(sql3, ", ");
				strcat(sql3, "a_used");			IntCat(sql3, icol, false);
#ifdef COMPOSITE_TIME
				strcat(sql3, ", ");
				strcat(sql3, "a_used");			IntCat(sql3, icol, false);	strcat(sql3, "_2");
#endif	// COMPOSITE_TIME
				
				strcat(sql4, ", ");				IntCat(sql4, pItemIndex[i], false);
				strcat(sql4, ", ");				IntCat(sql4, 0, false);
				strcat(sql4, ", ");				IntCat(sql4, -1, false);
				strcat(sql4, ", ");				IntCat(sql4, 0, false);
				strcat(sql4, ", '");			GetSerial(tmp, pItemIndex[i]); strcat(sql4, tmp);
				strcat(sql4, "', ");			IntCat(sql4, pItemCount[i], false);
				strcat(sql4, ", ");				IntCat(sql4, nItemUsed, false);
#ifdef COMPOSITE_TIME
				strcat(sql4, ", ");				IntCat(sql4, used_2, false);
#endif	// COMPOSITE_TIME

				icol++;
				if (icol == 5)
				{
					irow++;
					icol = 0;
					
					strcpy(m_buf, sql1);
					strcat(m_buf, sql3);
					strcat(m_buf, sql2);
					strcat(m_buf, sql4);
					strcat(m_buf, ")");
					
					dbChar.SetQuery(m_buf);
					if (!dbChar.Update())
						throw MSG_FAIL_DB_UNKNOWN;
					strcpy(sql2, ") VALUES (");
					IntCat(sql2, charidx, false); strcat(sql2, ", 0, "); IntCat(sql2, irow, false);

					memset(m_buf, 0x00, sizeof(m_buf));
					sql3[0] = '\0';
					sql4[0] = '\0';
				}
			} // for
			
			if (strlen(sql3) != 0)
			{		
				strcpy(m_buf, sql1);
				strcat(m_buf, sql3);
				strcat(m_buf, sql2);
				strcat(m_buf, sql4);
				strcat(m_buf, ")");
				
				dbChar.SetQuery(m_buf);
				if (!dbChar.Update())
					throw MSG_FAIL_DB_UNKNOWN;
			}
		} // HANARO_EVENT
#endif

		/////////////////////////

		// 디폴트 슬롯 설정
		strcpy(m_buf, "INSERT INTO t_quickslot");
		IntCat0(m_buf, charidx % 10, 2, false);
		StrCat(m_buf, "(a_char_idx, a_page_idx, a_slot) VALUES (");
		IntCat(m_buf, charidx, false);
		StrCat(m_buf, ", 0, ");

		switch (d->m_pChar->m_job)
		{
		case JOB_TITAN:
			strcat(m_buf, "' 0 1");			// 파워매그넘
			break;

		case JOB_KNIGHT:
			strcat(m_buf, "' 0 14");		// 너클스피어
			break;

		case JOB_HEALER:
			strcat(m_buf, "' 0 27 0 31");	// 샤이닝애로우, 셀프힐
			break;

		case JOB_MAGE:
			strcat(m_buf, "' 0 53");		// 에너지 볼트
			break;

		case JOB_ROGUE:
			strcat(m_buf, "' 0 116");		// 에너지 볼트
			break;

		case JOB_SORCERER:
			strcat(m_buf, "' 0 292");		// 파이어 스윙
			break;

		default:
			strcat(m_buf, "'");
			break;
		}
		StrCat(m_buf, "1 4 1 5 1 6 1 7 1 3 1 30')");		// ?기/교환/균등파티/우선파티/앉기
		dbChar.SetQuery(m_buf);
		if (!dbChar.Update())
			throw MSG_FAIL_DB_UNKNOWN;

		// 빈슬롯 주기 page 1 ~ 2
		int page;
		for (page = 1; page < QUICKSLOT_PAGE_NUM; page++)
		{
			strcpy(m_buf, "INSERT INTO t_quickslot");
			IntCat0(m_buf, charidx % 10, 2, false);
			StrCat(m_buf, "(a_char_idx, a_page_idx) VALUES(");
			IntCat(m_buf, charidx, false);
			StrCat(m_buf, ",");
			IntCat(m_buf, page);
			StrCat(m_buf, ")");
			dbChar.SetQuery(m_buf);

			if (!dbChar.Update())
				throw MSG_FAIL_DB_UNKNOWN;
		}

		dbChar.Commit();

#ifdef EX_GO_ZONE
		sprintf(m_buf, "INSERT INTO t_save_zone (a_char_idx, a_lastsavepos) VALUES (%d, 93)", charidx);
		dbChar.SetQuery(m_buf);
		if(!dbChar.Update())
		{
			DBLOG	<< init("DB ERROR : INSERT SAVEZONE DATA (NEW CHAR)", d->m_pChar)
					<< mysql_error(dbChar.m_dbconn)
					<< end;
		}

		sprintf(m_buf, "INSERT INTO t_find_zone%02d (a_char_idx, a_search_npc) VALUES (%d, 93)", charidx%10, charidx);
		dbChar.SetQuery(m_buf);
		if(!dbChar.Update())
		{
			DBLOG	<< init("DB ERROR : INSERT FINDZONE DATA (NEW CHAR)", d->m_pChar)
					<< mysql_error(dbChar.m_dbconn)
					<< end;
		}
#endif // EX_GO_ZONE

#ifdef EVENT_JPN_2007_NEWSERVER
		// 이벤트 진행 도중에는 무조건 insert 시도
		if (gserver.m_bEventJPN2007NewServer)
		{
			if (d->m_userFlag & IS_NEWUSER_JPN_2007_03)
			{
				sprintf(m_buf, "INSERT INTO t_event_2007_newserver (a_userindex, a_charindex, a_date) VALUES (%d, %d, NOW())", d->m_index, charidx);
				dbChar.SetQuery(m_buf);
				dbChar.Update();
			}
		}
#endif // EVENT_JPN_2007_NEWSERVER

		CNetMsg smsg;
		DBSuccessMsg(smsg);
		d->m_dbOut.Add(smsg, 99999);
	}
	catch (MSG_FAIL_TYPE failtype)
	{
		dbChar.Rollback();

		CNetMsg failmsg;
		FailMsg(failmsg, failtype);
		d->m_dbOut.Add(failmsg, 99999);
		ret = false;
	}
	catch (...)
	{
		dbChar.Rollback();

		CNetMsg failmsg;
		FailMsg(failmsg, MSG_FAIL_SYSTEM_ERROR);
		d->m_dbOut.Add(failmsg, 99999);
		ret = false;
	}

	return ret;
}

bool CDBThread::OnDelCharDB(CDescriptor* d)
{
	if (d->m_bclosed) return false;

	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);

#if defined (LC_TWN211111)
	// 대만과 태국은 레벨 15이상 삭제 금지
	int level;
	char sql[1024];

	dbChar.Init(&gserver.m_dbchar);

	sprintf(sql, "select a_level from t_characters where a_index = %d", d->m_pChar->m_index);

	dbChar.SetQuery(sql);
	if( !dbChar.Open() )
	{
		CNetMsg failmsg;
		FailMsg(failmsg, MSG_FAIL_DB_UNKNOWN);
		d->m_dbOut.Add(failmsg, 99999);
		return false;
	}
	if( !dbChar.MoveFirst() )
	{
		CNetMsg failmsg;
		FailMsg(failmsg, MSG_FAIL_DB_NOT_EXIST_CHAR);
		d->m_dbOut.Add(failmsg, 99999);
		return false;
	}

	if( !dbChar.GetRec("a_level", level) )
	{
		CNetMsg failmsg;
		FailMsg(failmsg, MSG_FAIL_DB_NOT_EXIST_CHAR);
		d->m_dbOut.Add(failmsg, 99999);
		return false;
	}

	if( level >= 32)
	{
		CNetMsg failmsg;
		FailMsg(failmsg, MSG_FAIL_NOTLEVEL_FORDELETE);
		d->m_dbOut.Add(failmsg, 99999);
		return false;
	}

#endif

	GetDateStr(m_te_buf);

#ifdef ENABLE_CHARACTER_DELETE_DELAY
	strcpy(m_t_buf, "UPDATE t_characters SET a_deletedelay=UNIX_TIMESTAMP(NOW())+24*60*60, a_datestamp='");
#else // ENABLE_CHARACTER_DELETE_DELAY
	strcpy(m_t_buf, "UPDATE t_characters SET a_enable=0, a_datestamp='");
#endif // ENABLE_CHARACTER_DELETE_DELAY
	strcat(m_t_buf, m_te_buf);
	strcat(m_t_buf, "' ");
#if defined (LC_GER)
	strcat(m_t_buf, " , a_guildoutdate=");
	IntCat(m_t_buf, d->m_pChar->m_guildoutdate, false);
#endif
	strcat(m_t_buf, " WHERE a_user_index=");
	IntCat(m_t_buf, d->m_index, false);
	strcat(m_t_buf, " AND a_server=");
	IntCat(m_t_buf, gserver.m_serverno, false);
	strcat(m_t_buf, " AND a_index=");
	IntCat(m_t_buf, d->m_pChar->m_index, false);
	strcat(m_t_buf, " AND a_enable=1");
	dbChar.SetQuery(m_t_buf);
	if (dbChar.Update())
	{
		if (dbChar.m_nrecords > 0)
		{
			CNetMsg smsg;
			DBSuccessMsg(smsg);
			d->m_dbOut.Add(smsg, 99999);
		}
		else
		{
			CNetMsg failmsg;
			FailMsg(failmsg, MSG_FAIL_DB_NOT_EXIST_CHAR);
			d->m_dbOut.Add(failmsg, 99999);
			return false;
		}

	}
	else
	{
		CNetMsg failmsg;
		FailMsg(failmsg, MSG_FAIL_DB_UNKNOWN);
		d->m_dbOut.Add(failmsg, 99999);
		return false;
	}

	return true;
}

#ifdef ENABLE_CHARACTER_DELETE_DELAY
bool CDBThread::OnDelCancelCharDB(CDescriptor* d)
{
	if (d->m_bclosed) return false;

	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);

	GetDateStr(m_te_buf);

	strcpy(m_t_buf, "UPDATE t_characters SET a_deletedelay=0, a_datestamp='");
	strcat(m_t_buf, m_te_buf);
	strcat(m_t_buf, "' WHERE a_user_index=");
	IntCat(m_t_buf, d->m_index, false);
	strcat(m_t_buf, " AND a_server=");
	IntCat(m_t_buf, gserver.m_serverno, false);
	strcat(m_t_buf, " AND a_index=");
	IntCat(m_t_buf, d->m_pChar->m_index, false);
	strcat(m_t_buf, " AND a_enable=1");
	dbChar.SetQuery(m_t_buf);
	if (dbChar.Update())
	{
		if (dbChar.m_nrecords > 0)
		{
			CNetMsg smsg;
			DBSuccessMsg(smsg);
			d->m_dbOut.Add(smsg, 99999);
		}
		else
		{
			CNetMsg failmsg;
			FailMsg(failmsg, MSG_FAIL_DB_NOT_EXIST_CHAR);
			d->m_dbOut.Add(failmsg, 99999);
			return false;
		}

	}
	else
	{
		CNetMsg failmsg;
		FailMsg(failmsg, MSG_FAIL_DB_UNKNOWN);
		d->m_dbOut.Add(failmsg, 99999);
		return false;
	}

	return true;
}
#endif // ENABLE_CHARACTER_DELETE_DELAY

bool CDBThread::OnPlayCharDB(CDescriptor* d)
{
	int i, j;
	const char* pch;
	char tbuf[256];
	char ibuf[128];

	if(d == NULL || d->m_bclosed) return false;

	bool bduplicate = false;
	if(d->m_pChar)
	{
		if (strlen(d->m_pChar->m_name) > 0)
		{
			bduplicate = true;
			CPC* newpc = new CPC;
			newpc->m_index = d->m_pChar->m_index;
			newpc->m_desc = d->m_pChar->m_desc;

			delete d->m_pChar;
			d->m_pChar = newpc;
		}
	}

	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);

	strcpy(m_buf, "SELECT * FROM t_characters WHERE a_user_index=");
	IntCat(m_buf, d->m_index, false);
	strcat(m_buf, " AND a_server=");
	IntCat(m_buf, gserver.m_serverno, false);
	strcat(m_buf, " AND a_index=");
	IntCat(m_buf, d->m_pChar->m_index, false);
	strcat(m_buf, " AND a_enable=1");

	dbChar.SetQuery(m_buf);
	dbChar.Open();

	if (!dbChar.MoveNext())
		return false;

#ifdef ENABLE_CHARACTER_DELETE_DELAY
	int nDeleteDelay = 0;
	dbChar.GetRec("a_deletedelay", nDeleteDelay);
	if (nDeleteDelay != 0)
	{
		CNetMsg rmsg;
		FailMsg(rmsg, MSG_FAIL_DB_DELETE_DELAY_CHAR);
		d->m_dbOut.Add(rmsg, 99999);
		return false;
	}
#endif // ENABLE_CHARACTER_DELETE_DELAY

	CPC* pChar = d->m_pChar;

	dbChar.GetRec("a_name", pChar->m_name, true);
	dbChar.GetRec("a_nick", pChar->m_nick, true);
	dbChar.GetRec("a_index",			pChar->m_index);
	dbChar.GetRec("a_level",			pChar->m_level);
	dbChar.GetRec("a_skill_point",		pChar->m_skillPoint);
	dbChar.GetRec("a_admin",			pChar->m_admin);
	dbChar.GetRec("a_was_x",			GET_X(pChar));
	dbChar.GetRec("a_was_z",			GET_Z(pChar));
	dbChar.GetRec("a_was_r",			GET_R(pChar));
	dbChar.GetRec("a_blood_point",		pChar->m_bloodPoint);

	dbChar.GetRec("a_job",				pChar->m_job);
#ifdef ENABLE_CHANGEJOB
	dbChar.GetRec("a_job2",				pChar->m_job2);
#endif
	dbChar.GetRec("a_hair_style",		pChar->m_hairstyle);
	dbChar.GetRec("a_face_style",		pChar->m_facestyle);
	dbChar.GetRec("a_exp",				pChar->m_exp);
	dbChar.GetRec("a_str",				pChar->m_dbStr);			pChar->m_str = pChar->m_dbStr;
	dbChar.GetRec("a_dex",				pChar->m_dbDex);			pChar->m_dex = pChar->m_dbDex;
	dbChar.GetRec("a_int",				pChar->m_dbInt);			pChar->m_int = pChar->m_dbInt;
	dbChar.GetRec("a_con",				pChar->m_dbCon);			pChar->m_con = pChar->m_dbCon;
	dbChar.GetRec("a_statpt_remain",	pChar->m_statpt_remain);
	dbChar.GetRec("a_statpt_str",		pChar->m_statpt_str);
	dbChar.GetRec("a_statpt_dex",		pChar->m_statpt_dex);
	dbChar.GetRec("a_statpt_int",		pChar->m_statpt_int);
	dbChar.GetRec("a_statpt_con",		pChar->m_statpt_con);
	dbChar.GetRec("a_hp",				pChar->m_hp);
	dbChar.GetRec("a_max_hp",			pChar->m_dbHP);
	dbChar.GetRec("a_mp",				pChar->m_mp);
	dbChar.GetRec("a_max_mp",			pChar->m_dbMP);
	dbChar.GetRec("a_silence_pulse",	pChar->m_silencePulse);		pChar->m_silencePulse += gserver.m_pulse;
	dbChar.GetRec("a_pkpenalty",		pChar->m_pkPenalty);
	dbChar.GetRec("a_pkcount",			pChar->m_pkCount);
	dbChar.GetRec("a_pkrecover",		pChar->m_pkRecoverPulse);
	dbChar.GetRec("a_pkpenaltyhp",		pChar->m_pkPenaltyHP);
	dbChar.GetRec("a_pkpenaltymp",		pChar->m_pkPenaltyMP);
	dbChar.GetRec("a_guildoutdate",		pChar->m_guildoutdate);		
	dbChar.GetRec("a_pluseffect_option",	pChar->m_plusEffect);
	dbChar.GetRec("a_loseexp",			pChar->m_loseexp);

#ifdef RESTRICT_PK
	dbChar.GetRec("a_lastpktime",		pChar->m_lastPKTime);
#endif // RESTRICT_PK
	
#ifdef EVENT_PHOENIX// yhj   081105  피닉스 
	dbChar.GetRec("a_phoenix",			pChar->m_bPhoenix_Char);
#endif  // EVENT_PHOENIX

#ifdef NO_CHATTING
	dbChar.GetRec("a_flag",				pChar->m_nflag);
#endif	// NO_CHATTING

	int nZone, nArea, nYlayer;
	dbChar.GetRec("a_was_zone",			nZone);
	dbChar.GetRec("a_was_area",			nArea);
	dbChar.GetRec("a_was_yLayer",		nYlayer);

#ifdef NEW_GUILD
	CDBCmd dbGuild;
	dbGuild.Init(&gserver.m_dbchar);

	sprintf(m_buf, "SELECT * FROM t_characters_guildpoint WHERE a_char_index = %d ", d->m_pChar->m_index );
	dbGuild.SetQuery(m_buf);
	dbGuild.Open();
	if (!dbGuild.MoveNext())
	{
		sprintf(m_buf, "INSERT t_characters_guildpoint(a_char_index) VALUES ( %d )", d->m_pChar->m_index );
		dbGuild.SetQuery(m_buf);
		if( !dbGuild.Update() )
			return false;
		pChar->m_GuildExp = 0;
		pChar->m_GuildFame = 0;
	}
	else
	{
		dbGuild.GetRec( "a_guild_exp", pChar->m_GuildExp );
		dbGuild.GetRec( "a_guild_fame", pChar->m_GuildFame );
	}
#endif // NEW_GUILD

	
#ifdef FORCE_START_ZONE
	if (PLAYMODE(d) != MSG_LOGIN_RE)
	{
		nZone = -1;
		nArea = -1;
		nYlayer = -1;
	}
#endif //FORCE_START_ZONE

#ifdef EX_GO_ZONE
	CDBCmd dbSaveZone;
	dbSaveZone.Init(&gserver.m_dbchar);
	sprintf(m_buf, "SELECT * FROM t_save_zone WHERE a_char_idx=%d", pChar->m_index);
	dbSaveZone.SetQuery(m_buf);
	if(dbSaveZone.Open())
	{
		int lastsavepos;
		if(dbSaveZone.MoveNext())
		{
			dbSaveZone.GetRec("a_lastsavepos", lastsavepos);
			pChar->m_lastsavepos = lastsavepos;
			if(pChar->GetLastSaveZone(&nZone) == -1)
			{
				nZone = ZONE_START;
			}
		}
		else
		{
			nZone = ZONE_START;
			pChar->m_lastsavepos = 0;
		}
	}
	else
	{
		nZone = ZONE_START;
		pChar->m_lastsavepos = 0;
	}
#endif // EX_GO_ZONE

	bool bValidZone = false;
	int remoteIndex = -1;
	// 존을 찾아서
	i = gserver.FindZone(nZone);
	if (i != -1)
	{
		// 원격 서버면 설정
		if (gserver.m_zones[i].m_bRemote)
			remoteIndex = i;
		bValidZone = true;
	}
	if (!bValidZone)
	{
		// 잘못된 존 번호일때
		nZone = ZONE_START;		// TODO : 시작 존이 직업별/종족별로 다를 경우 수정 필요

		i = gserver.FindZone(nZone);
		// 시작 존을 찾아서
		bValidZone = true;
	}

	if (!bValidZone)
		return false;

	// 유효성 검사
	if (remoteIndex == -1)
	{
		if (i < 0 || i >= gserver.m_numZone)
		{
			// 시작 존 지점
			pChar->m_pZone		= gserver.m_zones + gserver.FindZone(ZONE_START);
			pChar->m_pArea		= pChar->m_pZone->m_area;
			GET_YLAYER(pChar)	= pChar->m_pZone->m_zonePos[0][0];
			GET_R(pChar)		= 0.0f;
			int nTry = 10;

			while (nTry--)
			{
				GET_X(pChar)		= GetRandom(pChar->m_pZone->m_zonePos[0][1], pChar->m_pZone->m_zonePos[0][3]) / 2.0f;
				GET_Z(pChar)		= GetRandom(pChar->m_pZone->m_zonePos[0][2], pChar->m_pZone->m_zonePos[0][4]) / 2.0f;
				if (pChar->m_pArea->GetAttr(GET_YLAYER(pChar), GET_X(pChar), GET_Z(pChar)) != MAPATT_BLOCK)
					break;
			}
		}
		// yLayer 수정
		else if (nYlayer < 0 || nYlayer > ((gserver.m_zones[i].m_countY - 1) * 2) || gserver.m_zones[i].m_countArea != 1)
		{
			// 가장 가까운 마을
			int nearZone;
			int nearPos;
			i = gserver.FindNearestZone(nZone, GET_X(pChar), GET_Z(pChar), &nearZone, &nearPos);
			if (i == -1) i = gserver.FindNearestZone(ZONE_START, -1.0f, -1.0f, &nearZone, &nearPos);

			pChar->m_pZone		= gserver.m_zones + i;
			pChar->m_pArea		= pChar->m_pZone->m_area;
			GET_YLAYER(pChar)	= pChar->m_pZone->m_zonePos[nearPos][0];
			GET_R(pChar)		= 0.0f;
			int nTry = 10;
			while (nTry--)
			{
				GET_X(pChar)		= GetRandom(pChar->m_pZone->m_zonePos[nearPos][1], pChar->m_pZone->m_zonePos[nearPos][3]) / 2.0f;
				GET_Z(pChar)		= GetRandom(pChar->m_pZone->m_zonePos[nearPos][2], pChar->m_pZone->m_zonePos[nearPos][4]) / 2.0f;
				if (pChar->m_pArea->GetAttr(GET_YLAYER(pChar), GET_X(pChar), GET_Z(pChar)) != MAPATT_BLOCK)
					break;
			}
		}
		else
		{
			// 원래 위치 그대로
			if (i == -1)
			{
				i = gserver.FindZone(ZONE_START);
				nYlayer = 0;
			}
			pChar->m_pZone		= gserver.m_zones + i;
			pChar->m_pArea		= pChar->m_pZone->m_area;
			GET_YLAYER(pChar)	= nYlayer;
		}

		// 원격 존이면 그쪽 서버로 다시 접속하라고 하고 넘기기
		if (pChar->m_pZone->m_bRemote)
			remoteIndex = pChar->m_pZone - gserver.m_zones;
		else
			remoteIndex = -1;
	}

	if (remoteIndex != -1)
	{
		// 다른 서버로 이동을 할 수 있도록 메시지 전달
		CNetMsg remotemsg;
		DBOtherServerMsg(remotemsg,
			(int)gserver.m_zones[remoteIndex].m_index,
			gserver.m_zones[remoteIndex].m_remoteIP,
			(int)gserver.m_zones[remoteIndex].m_remotePort);
		d->m_dbOut.Add(remotemsg, 99999);
		return true;
	}

	static CLCString skillIndex(256);
	static CLCString skillLevel(256);
	const char* pIndex = skillIndex;
	const char* pLevel = skillLevel;
	dbChar.GetRec("a_active_skill_index", skillIndex);
	dbChar.GetRec("a_active_skill_level", skillLevel);
	while (*pIndex && *pLevel)
	{
		int i, l;
		pIndex = AnyOneArg(pIndex, tbuf);
		i = atoi(tbuf);
		pLevel = AnyOneArg(pLevel, tbuf);
		l = atoi(tbuf);
		CSkill* s = gserver.m_skillProtoList.Create(i, l);
		if (s)
		{
			if (s->m_proto->IsActive())
				pChar->m_activeSkillList.Add(s);
			else if (s->m_proto->IsPassive())
				pChar->m_passiveSkillList.Add(s);
			else
				pChar->m_etcSkillList.Add(s);
		}
	}


	skillIndex = "";
	skillLevel = "";
	pIndex = skillIndex;
	pLevel = skillLevel;
	dbChar.GetRec("a_passive_skill_index", skillIndex);
	dbChar.GetRec("a_passive_skill_level", skillLevel);
	while (*pIndex && *pLevel)
	{
		int i, l;
		pIndex = AnyOneArg(pIndex, tbuf);
		i = atoi(tbuf);
		pLevel = AnyOneArg(pLevel, tbuf);
		l = atoi(tbuf);
		CSkill* s = gserver.m_skillProtoList.Create(i, l);
		if (s)
		{
			if (s->m_proto->IsActive())
				pChar->m_activeSkillList.Add(s);
			else if (s->m_proto->IsPassive())
				pChar->m_passiveSkillList.Add(s);
			else
				pChar->m_etcSkillList.Add(s);
		}
	}
	skillIndex = "";
	skillLevel = "";
	pIndex = skillIndex;
	pLevel = skillLevel;
	dbChar.GetRec("a_etc_skill_index", skillIndex);
	dbChar.GetRec("a_etc_skill_level", skillLevel);
	while (*pIndex && *pLevel)
	{
		int i, l;
		pIndex = AnyOneArg(pIndex, tbuf);
		i = atoi(tbuf);
		pLevel = AnyOneArg(pLevel, tbuf);
		l = atoi(tbuf);
		CSkill* s = gserver.m_skillProtoList.Create(i, l);
		if (s)
		{
			if (s->m_proto->IsActive())
				pChar->m_activeSkillList.Add(s);
			else if (s->m_proto->IsPassive())
				pChar->m_passiveSkillList.Add(s);
			else
				pChar->m_etcSkillList.Add(s);
		}
	}

#ifdef FACTORY_SYSTEM
	skillIndex = "";
	skillLevel = "";
	pIndex = skillIndex;
	pLevel = skillLevel;
	dbChar.GetRec("a_seal_skill_index", skillIndex);
	dbChar.GetRec("a_seal_skill_exp",	skillLevel);
	int nSeal = 0;
	while (*pIndex && *pLevel)
	{
		int i;
		LONGLONG l;
		pIndex = AnyOneArg(pIndex, tbuf);
		i = atoi(tbuf);
		pLevel = AnyOneArg(pLevel, tbuf);
		l = atoi(tbuf);

		CSkill* s = gserver.m_skillProtoList.Create(i);
		if (s && s->m_proto->IsSeal())
		{
			pChar->m_sealSkillList.Add(s);
			pChar->m_sealSkillExp[nSeal++].nSkillIdx = i;
			pChar->m_sealSkillExp[nSeal++].llExp	 = l;
		}
	}

	CDBCmd dbFactory;
	dbFactory.Init(&gserver.m_dbchar);

	sprintf(m_buf, "SELECT * FROM t_characters_factory WHERE a_char_index = %d ", d->m_pChar->m_index);
	dbFactory.SetQuery(m_buf);
	dbFactory.Open();
	if (dbFactory.MoveNext())
	{
		CLCString detail(256);
		CLCString colorful(256);
		CLCString sharp(256);
		CLCString hard(256);
		CLCString consume(256);

		const char * pDetail;
		const char * pColorful;
		const char * pSharp;
		const char * pHard;
		const char * pConsume;

		pDetail		= detail;
		pColorful	= colorful;
		pSharp		= sharp;
		pHard		= hard;
		pConsume	= consume;

		dbFactory.GetRec("a_detail",	detail	);
		dbFactory.GetRec("a_colorful",	colorful);
		dbFactory.GetRec("a_sharp",		sharp	);
		dbFactory.GetRec("a_hard",		hard	);
		dbFactory.GetRec("a_consume",	consume );

		while (*pDetail)
		{
			int nIndex;
			pIndex = AnyOneArg(pDetail, tbuf);
			nIndex = atoi(tbuf);
			
			CFactoryProto* f = gserver.m_factoryItemProtoList.Find(nIndex);
			pChar->m_listFactory.Add(f);
		}

		while (*pColorful)
		{
			int nIndex;
			pIndex = AnyOneArg(pColorful, tbuf);
			nIndex = atoi(tbuf);
			
			CFactoryProto* f = gserver.m_factoryItemProtoList.Find(nIndex);
			pChar->m_listFactory.Add(f);
		}

		while (*pSharp)
		{
			int nIndex;
			pIndex = AnyOneArg(pSharp, tbuf);
			nIndex = atoi(tbuf);
			
			CFactoryProto* f = gserver.m_factoryItemProtoList.Find(nIndex);
			pChar->m_listFactory.Add(f);
		}

		while (*pHard)
		{
			int nIndex;
			pIndex = AnyOneArg(pHard, tbuf);
			nIndex = atoi(tbuf);
			
			CFactoryProto* f = gserver.m_factoryItemProtoList.Find(nIndex);
			pChar->m_listFactory.Add(f);
		}

		while (*pConsume)
		{
			int nIndex;
			pIndex = AnyOneArg(pConsume, tbuf);
			nIndex = atoi(tbuf);
			
			CFactoryProto* f = gserver.m_factoryItemProtoList.Find(nIndex);
			pChar->m_listFactory.Add(f);
		}
	}
#endif // FACTORY_SYSTEM

	/////////////////////
	// 퀘스트 읽기

	// 하드코딩 : 튜토리얼 모드로 스타트존이 바뀌어야 하는가?
	bool bGotoTutorial = false;

#ifdef QUEST_DATA_EXTEND
	static CLCString		questIndex(256);
	static CLCString		questValue(256);
	static CLCString		questComplete(256);
	static CLCString		questAbandon(256);

	const char* pQIndex		= questIndex;
	const char* pQValue		= questValue;
	const char* pQComplete	= questComplete;
	const char* pQAbandon	= questAbandon;

	dbChar.GetRec("a_quest_index",		questIndex);
	dbChar.GetRec("a_quest_value",		questValue);
	dbChar.GetRec("a_quest_complete",	questComplete);
	dbChar.GetRec("a_quest_abandon",	questAbandon);

	// 이전 필드에서 읽기, 새로운 테이블에서 읽기는 별도
	int		nQuestIndex;
	char	nQuestState;
	int		nQuestValue[QUEST_MAX_CONDITION];
	while (*pQIndex)
	{
		pQIndex = AnyOneArg(pQIndex, tbuf);
		nQuestIndex = atoi(tbuf);

		CQuest* pQuest = pChar->m_questList.AddQuest(nQuestIndex);
		if (pQuest == NULL)
		{
			for (i = 0; *pQValue && i < QUEST_MAX_CONDITION; i++)
				pQValue = AnyOneArg(pQValue, tbuf);
			continue ;
		}

		// 하드코딩 : 싱글던전1 튜토리얼판
		if (pQuest->IsTutorialQuest())
			bGotoTutorial = true;
		
		int value;

		for (i = 0; *pQValue && i < QUEST_MAX_CONDITION; i++)
		{
			pQValue = AnyOneArg(pQValue, tbuf);
			value = atoi(tbuf);
			pQuest->SetQuestValue(i, value);
		}

		pChar->m_questList.SetQuestState(pQuest, QUEST_STATE_RUN);
	}
	while (*pQComplete)
	{
		pQComplete = AnyOneArg(pQComplete, tbuf);
		nQuestIndex = atoi(tbuf);
		CQuest* pQuest = pChar->m_questList.AddQuest(nQuestIndex);
		if (pQuest)
			pChar->m_questList.SetQuestState(pQuest, QUEST_STATE_DONE);
	}
	while (*pQAbandon)
	{
		pQAbandon = AnyOneArg(pQAbandon, tbuf);
		nQuestIndex = atoi(tbuf);
		CQuest* pQuest = pChar->m_questList.AddQuest(nQuestIndex);
		if (pQuest)
			pChar->m_questList.SetQuestState(pQuest, QUEST_STATE_ABANDON);
	}

#else // QUEST_DATA_EXTEND

	static CLCString questIndex(256);
	static CLCString questValue(256);
	static CLCString questComplete(256);
	
	static CLCString questAbandon(256);
	const char* pQAbandon = questAbandon;
	dbChar.GetRec("a_quest_abandon", questAbandon);


	const char* pQIndex = questIndex;
	const char* pQValue = questValue;
	const char* pQComplete = questComplete;

	dbChar.GetRec("a_quest_index", questIndex);
	dbChar.GetRec("a_quest_value", questValue);
	dbChar.GetRec("a_quest_complete", questComplete);

	while (*pQIndex)
	{
		int i;
		pQIndex = AnyOneArg(pQIndex, tbuf);
		i = atoi(tbuf);

		CQuestProto* proto = gserver.m_questProtoList.FindProto(i);
		if (!proto)
			continue;

		CQuest* quest = new CQuest(proto);

		int idx = pChar->m_questList.AddQuest(quest);

		if (idx == -1)
			continue;

		// 하드코딩 : 싱글던전1 튜토리얼판
		if (proto->m_type[0] == QTYPE_KIND_TUTORIAL)
			bGotoTutorial = true;
		
		int value;

		for (i=0; *pQValue && i < QUEST_MAX_CONDITION; i++)
		{
			pQValue = AnyOneArg(pQValue, tbuf);
			value = atoi(tbuf);
			pChar->m_questList.m_list[idx]->m_currentData[i] = value;
		}

		pChar->m_questList.m_bQuest[idx] = true;
	}
	j=0;
	while (*pQComplete)
	{
		pQComplete = AnyOneArg(pQComplete, tbuf);
		if (!pChar->m_questList.SetDoneQuest(atoi(tbuf)))
			break;
		j++;
	}
	pChar->m_questList.m_doneCnt = j;

	j=0;
	while (*pQAbandon && j < QUEST_MAX_PC_COMPLETE)
	{
		pQAbandon = AnyOneArg(pQAbandon, tbuf);
		i = atoi(tbuf);
		pChar->m_questList.m_abandonQuest[j] = i;
		j++;
	}
	pChar->m_questList.m_abandonCnt = j;
#endif // QUEST_DATA_EXTEND

	/////////////////////
	// Special Skill읽기

	static CLCString strSSkill(256);
	const char* psskill = strSSkill;

	dbChar.GetRec("a_sskill", strSSkill);

	while (*psskill)
	{
		int index, level;
		psskill = AnyOneArg(psskill, tbuf);
		index = atoi(tbuf);
		psskill = AnyOneArg(psskill, tbuf);
		level = atoi(tbuf);
		
#ifdef LC_HBK
		// 없는 생산 스킬 처리
		int totoal_recover_sp = 0;
		CheckSSkill(index, level, totoal_recover_sp);

		if(totoal_recover_sp != 0)
		{	// sp보상 지급
			pChar->m_skillPoint += totoal_recover_sp*10000;
			DBLOG << init("sskill sp recover ", pChar)
				<< delim << totoal_recover_sp << delim << pChar->m_skillPoint << delim
				<< end;
		}
#endif // LC_HBK

		CSSkillProto* proto = gserver.m_sSkillProtoList.FindProto(index);

		if (!proto)
		{	
			continue;
		}

		CSSkill* sskill = new CSSkill(proto, level);

		if (!sskill)
			continue;

		bool bAdd = true;
 		CSSkillNode* node;
 		CSSkillNode* nodeNext = pChar->m_sSkillList.m_head;
 		while ((node = nodeNext))
 		{
 			nodeNext = nodeNext->m_next;

			// 같은 레벨이 있으면 높은 레벨을 추가한다.
			if (node->m_sskill->m_proto->m_index == sskill->m_proto->m_index)
			{
				if (node->m_sskill->m_level >= sskill->m_level)
					bAdd = false;
				else
				{
					pChar->m_sSkillList.Remove(node->m_sskill);
					bAdd = true;
				}
			}
// 
// 			// 우선순위가 있고 타입이 같고
// 			if (sskill->m_proto->m_preference != -1 && node->m_sskill->m_proto->m_type == sskill->m_proto->m_type)
// 			{
// 				// 우선순위가 높은 특수스킬일 가지고 있다면 ?// 				if (node->m_sskill->m_proto->m_preference >= sskill->m_proto->m_preference)
// 				{
// 					bAdd = false;
// 				}
// 				else if (node->m_sskill->m_proto->m_preference < sskill->m_proto->m_preference)
// 				{					
// 					// 스킬 삭제
// 					pChar->m_sSkillList.Remove(node->m_sskill);
// 				}
// 			}
		}

		if (bAdd)
			pChar->m_sSkillList.Add(sskill);
	}

#ifdef EVENT_SEARCHFRIEND
	// 국내 통합전 인덱스와 서버군 가져오기
	dbChar.GetRec("a_server_old", pChar->m_serverOld);
#endif // #ifdef LC_KOR

// 이벤트 테이블 읽기
#ifdef ETC_EVENT
	dbChar.GetRec("a_etc_event", pChar->m_etcEvent);
#endif

	// 캐릭터 별 MAX 무게 셋팅
	pChar->SetMaxWeight(true);

#ifdef QUEST_DATA_EXTEND
	// 새로운 테이블에서 퀘스트 읽기
	sprintf(m_buf, "SELECT * FROM t_questdata%02d WHERE a_char_index=%d ORDER BY a_state, a_quest_index", pChar->m_index % 10, pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (!dbChar.Open())
	{
		DBLOG	<< init("DB ERROR : LOAD CHARACTER QUEST TABLE", pChar)
				<< mysql_error(dbChar.m_dbconn)
				<< end;
	}
	else
	{
		int nLoopValue;
		while (dbChar.MoveNext())
		{
			dbChar.GetRec("a_quest_index",			nQuestIndex);
			dbChar.GetRec("a_state",				nQuestState);

			CQuest* pQuest = pChar->m_questList.AddQuest(nQuestIndex);
			if (pQuest == NULL)
				continue ;

			// 하드코딩 : 싱글던전1 튜토리얼판
			if (nQuestState == QUEST_STATE_RUN && pQuest->IsTutorialQuest())
				bGotoTutorial = true;
			
			for (nLoopValue = 0; nQuestState == QUEST_STATE_RUN && nLoopValue < QUEST_MAX_CONDITION; nLoopValue++)
			{
				sprintf(m_te_buf, "a_value%d", nLoopValue);
				dbChar.GetRec(m_te_buf,				nQuestValue[nLoopValue]);
				pQuest->SetQuestValue(nLoopValue, nQuestValue[nLoopValue]);
			}

			pChar->m_questList.SetQuestState(pQuest, nQuestState);
		}
	}
#endif // QUEST_DATA_EXTEND

#ifdef EX_GO_ZONE
	sprintf(m_buf, "SELECT * FROM t_find_zone%02d WHERE a_char_idx = %d", pChar->m_index%10, pChar->m_index);
	dbChar.SetQuery(m_buf);
	if(!dbChar.Open())
	{
	}
	else
	{
		int savenpc;
		while(dbChar.MoveNext())
		{
			dbChar.GetRec("a_search_npc", savenpc);
			pChar->m_listRegisterNpc.AddToTail(savenpc);
		}
	}
#endif // EX_GO_ZONE

#ifdef EVENT_NEWYEAR_2006
	// 2006 신년 이벤트 읽기
	sprintf(m_buf, "SELECT a_total_time FROM t_event_newyear2006 WHERE a_char_index=%d", pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (dbChar.Open() && dbChar.MoveFirst())
	{
		int nTotalTime;
		if (dbChar.GetRec("a_total_time", nTotalTime))
			pChar->m_nTimeEventNewYear2006 = nTotalTime;
	}
#endif // #ifdef EVENT_NEWYEAR_2006


#ifdef EVENT_SEARCHFRIEND
	// 휴면 케릭 이벤트 읽기 (1개임)
	sprintf(m_buf, "SELECT * FROM t_event_searchfriend WHERE a_dormant_index = %d", pChar->m_index);
	dbChar.SetQuery(m_buf);

	CLCString friendName(MAX_CHAR_NAME_LENGTH + 1);
	int ntemp = 0, nSearchFriendSelect;
	
	if (dbChar.Open() && dbChar.MoveFirst())
	{
		pChar->m_bEventSearchFriendSelect = false;
		do
		{
			nSearchFriendSelect = 0;
			dbChar.GetRec("a_char_index", pChar->m_nEventSearchFriendIndex[ntemp]);
			dbChar.GetRec("a_char_nick", friendName);				strcpy(pChar->m_nEventSearchFriendNick[ntemp], friendName);
			dbChar.GetRec("a_dormant_select",nSearchFriendSelect);

			if(nSearchFriendSelect == 1)// 휴면 케릭이 등록하지 않았을경우. 선택한다.
			{
				dbChar.GetRec("a_dormant_total_time", pChar->m_nTimeEventSearchFriend);

				pChar->m_bEventSearchFriendSelect = true;
			}				
			ntemp++;

		}while (dbChar.MoveNext());
	}
	pChar->m_nEventSearchFriendListCount = ntemp;   //휴면 케릭 선택 할 리스트 개수 

#endif // #ifdef EVENT_SEARCHFRIEND


	////////////////
	// 보조효과 읽기
#ifndef CLEAR_ASSIST

	strcpy(m_buf, "SELECT * FROM t_assist WHERE a_char_index=");
	IntCat(m_buf, pChar->m_index, false);
	dbChar.SetQuery(m_buf);
	if (dbChar.Open() && dbChar.MoveFirst())
	{
		const char* p1;
		const char* p2;
		const char* p3;
		const char* p4;
		const char* p5;
		const char* p6;
		const char* p7;

		int nitem;
		int nskill;
		int nlevel;
		int nremain;
		bool bHit[MAX_SKILL_MAGIC];
		const CSkillProto* sp;

		// help
		dbChar.GetRec("a_help_item", assistitem);		p1 = assistitem;
		dbChar.GetRec("a_help_skill", assistskill);		p2 = assistskill;
		dbChar.GetRec("a_help_level", assistlevel);		p3 = assistlevel;
		dbChar.GetRec("a_help_remain", assistremain);	p4 = assistremain;
		dbChar.GetRec("a_help_hit0", assisthit0);		p5 = assisthit0;
		dbChar.GetRec("a_help_hit1", assisthit1);		p6 = assisthit1;
		dbChar.GetRec("a_help_hit2", assisthit2);		p7 = assisthit2;

		bool bStop = false;
		while (!bStop)
		{
			*m_t_buf = '\0';	p1 = AnyOneArg(p1, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		nitem = atoi(m_t_buf);
			*m_t_buf = '\0';	p2 = AnyOneArg(p2, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		nskill = atoi(m_t_buf);
			*m_t_buf = '\0';	p3 = AnyOneArg(p3, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		nlevel = atoi(m_t_buf);
			*m_t_buf = '\0';	p4 = AnyOneArg(p4, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		nremain = atoi(m_t_buf);
			*m_t_buf = '\0';	p5 = AnyOneArg(p5, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		bHit[0] = (atoi(m_t_buf)) ? true : false;
			*m_t_buf = '\0';	p6 = AnyOneArg(p6, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		bHit[1] = (atoi(m_t_buf)) ? true : false;
			*m_t_buf = '\0';	p7 = AnyOneArg(p7, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		bHit[2] = (atoi(m_t_buf)) ? true : false;

#ifdef FORCE_START_ZONE
			if (nskill == 408)
				continue ;
#endif // FORCE_START_ZONE
			
#ifdef EVENT_PCBANG_2ND
			if(nskill == 493)
			{	// PC방 버프는 저장 안됨 그러므로 읽어 올 때도 스킵
				continue;
			}
#endif // EVENT_PCBANG_2ND
			
#ifdef EVENT_PHOENIX   // 피닉스 이벤트  yhj
			if( nskill == 516 )
			{	// 피닉스 버프는 저장 안됨 그러므로 읽어 올 때도 스킵
				continue;
			}
#endif // EVENT_PHOENIX

			sp = gserver.m_skillProtoList.Find(nskill);
			if (sp == NULL)
				continue ;

			if (nitem > 0 && gserver.m_itemProtoList.FindIndex(nitem) == NULL)
				continue ;

			pChar->m_assist.Add(NULL, nitem, sp, nlevel, bHit, false, nremain, 10000, 0, 0);
		}

		// curse
		dbChar.GetRec("a_curse_item", assistitem);		p1 = assistitem;
		dbChar.GetRec("a_curse_skill", assistskill);	p2 = assistskill;
		dbChar.GetRec("a_curse_level", assistlevel);	p3 = assistlevel;
		dbChar.GetRec("a_curse_remain", assistremain);	p4 = assistremain;
		dbChar.GetRec("a_curse_hit0", assisthit0);		p5 = assisthit0;
		dbChar.GetRec("a_curse_hit1", assisthit1);		p6 = assisthit1;
		dbChar.GetRec("a_curse_hit2", assisthit2);		p7 = assisthit2;

		bStop = false;
		while (!bStop)
		{
			*m_t_buf = '\0';	p1 = AnyOneArg(p1, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		nitem = atoi(m_t_buf);
			*m_t_buf = '\0';	p2 = AnyOneArg(p2, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		nskill = atoi(m_t_buf);
			*m_t_buf = '\0';	p3 = AnyOneArg(p3, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		nlevel = atoi(m_t_buf);
			*m_t_buf = '\0';	p4 = AnyOneArg(p4, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		nremain = atoi(m_t_buf);
			*m_t_buf = '\0';	p5 = AnyOneArg(p5, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		bHit[0] = (atoi(m_t_buf)) ? true : false;
			*m_t_buf = '\0';	p6 = AnyOneArg(p6, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		bHit[1] = (atoi(m_t_buf)) ? true : false;
			*m_t_buf = '\0';	p7 = AnyOneArg(p7, m_t_buf);	if (!(*m_t_buf)) { bStop = true; break; }		bHit[2] = (atoi(m_t_buf)) ? true : false;

#ifdef FORCE_START_ZONE
			if (nskill == 408)
				continue ;
#endif // FORCE_START_ZONE

			sp = gserver.m_skillProtoList.Find(nskill);
			if (sp == NULL)
				continue ;

			if (nitem > 0 && gserver.m_itemProtoList.FindIndex(nitem) == NULL)
				continue ;

			pChar->m_assist.Add(NULL, nitem, sp, nlevel, bHit, false, nremain, 10000, 0, 0);
		}
	}

#endif // CLEAR_ASSIST


	//////////////////////////////////////////
	// 060227 : bs : 보조 효과 읽기 : 절대시간
	sprintf(m_buf, "SELECT * FROM t_assist_abstime WHERE a_char_index=%d", pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (dbChar.Open())
	{
		while (dbChar.MoveNext())
		{
			int nAssistABSItemIndex = 0;
			int nAssistABSSkillIndex = 0;
			int nAssistABSSkillLevel = 0;
			int nAssistABSHit0 = 0;
			int nAssistABSHit1 = 0;
			int nAssistABSHit2 = 0;
			int nAssistABSEndTime = 0;
			bool bAssistABSHit[3] = {false, false, false};

			if (   dbChar.GetRec("a_item_index", nAssistABSItemIndex)
				&& dbChar.GetRec("a_skill_index", nAssistABSSkillIndex)
				&& dbChar.GetRec("a_skill_level", nAssistABSSkillLevel)
				&& dbChar.GetRec("a_hit0", nAssistABSHit0)
				&& dbChar.GetRec("a_hit1", nAssistABSHit1)
				&& dbChar.GetRec("a_hit2", nAssistABSHit2)
				&& dbChar.GetRec("a_end_time", nAssistABSEndTime)
				)
			{
				bAssistABSHit[0] = (nAssistABSHit0) ? true : false;
				bAssistABSHit[1] = (nAssistABSHit1) ? true : false;
				bAssistABSHit[2] = (nAssistABSHit2) ? true : false;

				// 남은 시간 계산
				if (nAssistABSEndTime > gserver.m_gameTime)
				{
					int remain = (nAssistABSEndTime - gserver.m_gameTime) * PULSE_ASSIST_CHECK;

					const CSkillProto* sp = gserver.m_skillProtoList.Find(nAssistABSSkillIndex);
					if (sp)
					{
						if (sp->Level(nAssistABSSkillLevel))
						{
							if (nAssistABSItemIndex <= 0 || gserver.m_itemProtoList.FindIndex(nAssistABSItemIndex))
							{
								pChar->m_assist.Add(NULL, nAssistABSItemIndex, sp, nAssistABSSkillLevel, bAssistABSHit, false, remain, 10000, 0, 0);
							}
						}
					}
				}
			}
		}
	}
	// 060227 : bs : 보조 효과 읽기 : 절대시간
	//////////////////////////////////////////



#ifdef ENABLE_PET
	// 060221 : bs : 펫 사망 시간 읽기 추가
	// 애완동물 읽기
	// 펫 아이템 리스트 : 여기에 없는 펫은 지워버린다
	CLCList<CItem*> listPetItem(NULL);
#ifdef PET_NAME_CHANGE
	sprintf( m_buf, " SELECT p.*, pn.a_pet_name "
		" FROM t_pet AS p LEFT JOIN t_pet_name AS pn ON pn.a_pet_index = p.a_index "
		" WHERE p.a_owner = %d AND p.a_enable = 1 ORDER BY p.a_index ", pChar->m_index );
#else
	sprintf(m_buf, "SELECT * FROM t_pet WHERE a_owner=%d AND a_enable=1 ORDER BY a_index", pChar->m_index);
#endif //PET_NAME_CHANGE
	dbChar.SetQuery(m_buf);
	if (dbChar.Open())
	{
		int index;
		int owner;
		int lastupdate;
		char type;
		int level;
		int hp;
		int hungry;
		int sympathy;
		LONGLONG exp;
		int ability;
		CLCString petSkillIndex(256);
		CLCString petSkillLevel(256);
		const char* pPetSkillIndex;
		const char* pPetSkillLevel;
		char tmp[256];
		int nRemainRebirth;
#ifdef PET_NAME_CHANGE
		CLCString petName(30);
#endif //PET_NAME_CHANGE

#ifdef PET_DIFFERENTIATION_ITEM
		char colorType;
#endif // PET_DIFFERENTIATION_ITEM

#ifdef PET_TURNTO_NPC
		int nTurntoNpc;
#endif //PET_TURNTO_NPC
#ifdef PET_TURNTO_NPC_ITEM
		int nTurntoNpcSize;
#endif //PET_TURNTO_NPC_ITEM

		while (dbChar.MoveNext())
		{
			if (dbChar.GetRec("a_index", index) &&
				dbChar.GetRec("a_owner", owner) &&
				dbChar.GetRec("a_lastupdate", lastupdate) &&
				dbChar.GetRec("a_type", type) &&
				dbChar.GetRec("a_level", level) &&
				dbChar.GetRec("a_hp", hp) &&
				dbChar.GetRec("a_hungry", hungry) &&
				dbChar.GetRec("a_sympathy", sympathy) &&
				dbChar.GetRec("a_exp", exp) &&
				dbChar.GetRec("a_ability", ability) &&
				dbChar.GetRec("a_skill_index", petSkillIndex) &&
				dbChar.GetRec("a_skill_level", petSkillLevel) &&
				dbChar.GetRec("a_time_rebirth", nRemainRebirth)
#ifdef PET_DIFFERENTIATION_ITEM
				&& dbChar.GetRec("a_color", colorType)
#endif // PET_DIFFERENTIATION_ITEM
#ifdef PET_TURNTO_NPC
				&& dbChar.GetRec("a_pet_turnto_npc", nTurntoNpc)
#endif //PET_TURNTO_NPC
#ifdef PET_TURNTO_NPC_ITEM
				&& dbChar.GetRec("a_pet_size", nTurntoNpcSize )
#endif //PET_TURNTO_NPC_ITEM
				)
			{
#ifdef PET_NAME_CHANGE
				if( !dbChar.GetRec( "a_pet_name", petName ) )
					petName = "";
#endif // PET_NAME_CHANGE


// 060221 : bs : 펫 사망한 것도 읽기
//				if (hp == 0)
//					continue ;

				CPet* pet = new CPet(pChar, index, type, level);
				if (pet)
				{
					pet->m_hp = hp;
					pet->m_hungry = hungry;
					pet->m_sympathy = sympathy;
					pet->m_exp = exp;
					pet->m_abilityPoint = ability;
					pet->SetRemainRebirthTime(nRemainRebirth);

					pPetSkillIndex = petSkillIndex;
					pPetSkillLevel = petSkillLevel;
//					while (true)
					for(;;)
					{
						tmp[0] = '\0';
						pPetSkillIndex = AnyOneArg(pPetSkillIndex, tmp);
						if (strlen(tmp) < 1)
							break;
						int skillindex = atoi(tmp);
						tmp[0] = '\0';
						pPetSkillLevel = AnyOneArg(pPetSkillLevel, tmp);
						if (strlen(tmp) < 1)
							break;
						int skilllevel = atoi(tmp);
						CSkill* skill = gserver.m_skillProtoList.Create(skillindex, skilllevel);
						if (skill)
							pet->AddSkill(skill);
					}
#ifdef PET_NAME_CHANGE
					pet->m_name = petName;
#endif // PET_NAME_CHANGE

#ifdef PET_DIFFERENTIATION_ITEM
					pet->SetPetColor( colorType );
#endif // PET_DIFFERENTIATION_ITEM

#ifdef PET_TURNTO_NPC
					pet->SetPetTurnToNpc( nTurntoNpc );
#endif //PET_TURNTO_NPC

#ifdef PET_TURNTO_NPC_ITEM
					pet->SetPetTurnToNpcSize( nTurntoNpcSize );
#endif //PET_TURNTO_NPC_ITEM

					// 리스트에 추가
					ADD_TO_BILIST(pet, pChar->m_petList, m_prevPet, m_nextPet);
				}
			}
		}
	}

#endif // #ifdef ENABLE_PET

#ifdef ATTACK_PET
	// DB에서 PET Loading
	CDBCmd dbChar2;
	dbChar2.Init(&gserver.m_dbchar);

	CLCList<CItem*> listAPetItem(NULL);
	sprintf(m_buf, "SELECT * FROM t_apets WHERE a_owner=%d AND a_enable=1 ORDER BY a_index", pChar->m_index);
	dbChar.SetQuery(m_buf);
	if( dbChar.Open() )
	{
		int			apet_index;
		int			apet_seal;
		int			apet_proto_idx;
		CLCString	apet_name(21);
		int			apet_level;
		LONGLONG	apet_exp;
		int			apet_remain_stat;
		int			apet_plus_str;
		int			apet_plus_con;
		int			apet_plus_dex;
		int			apet_plus_int;
		int			apet_sp;
		CLCString	apet_skill(256);
		CLCString	apet_skillLevel(256);
		int			apet_hp;
		int			apet_mp;
		int			apet_faith;
		int			apet_stm;
		char		tmp[256];

#ifdef APET_AI
		char	apet_ai;
		int		apet_ai_slot;
#endif

		while (dbChar.MoveNext())
		{
			if (dbChar.GetRec("a_index", apet_index) &&
				dbChar.GetRec("a_seal", apet_seal) &&
				dbChar.GetRec("a_proto_index", apet_proto_idx) &&
				dbChar.GetRec("a_name", apet_name) &&
				dbChar.GetRec("a_level", apet_level) &&
				dbChar.GetRec("a_exp", apet_exp) &&
				dbChar.GetRec("a_remain_stat", apet_remain_stat) &&
				dbChar.GetRec("a_plus_str", apet_plus_str) &&
				dbChar.GetRec("a_plus_con", apet_plus_con) &&
				dbChar.GetRec("a_plus_dex", apet_plus_dex) &&
				dbChar.GetRec("a_plus_int", apet_plus_int) &&
				dbChar.GetRec("a_skill_point", apet_sp) &&
				dbChar.GetRec("a_skill_index", apet_skill) &&
				dbChar.GetRec("a_skill_level", apet_skillLevel) &&
				dbChar.GetRec("a_hp", apet_hp) &&
				dbChar.GetRec("a_mp", apet_mp) &&
				dbChar.GetRec("a_faith", apet_faith) &&
				dbChar.GetRec("a_stm", apet_stm)
#ifdef APET_AI				
				&& dbChar.GetRec("a_ai_enable", apet_ai)
				&& dbChar.GetRec("a_ai_slot", apet_ai_slot)
#endif
				)
			{
				CAPet* apet = gserver.m_pApetlist.Create( pChar, apet_index, apet_proto_idx );
				apet->m_name	= apet_name;
				apet->m_level	= apet_level;
				apet->m_exp		= apet_exp;

				apet->m_nSeal	= apet_seal;

				apet->m_nRemainStat	= apet_remain_stat;
				apet->m_nSP		= apet_sp;

				apet->m_nPlusStr	= apet_plus_str;
				apet->m_nPlusCon	= apet_plus_con;
				apet->m_nPlusDex	= apet_plus_dex;
				apet->m_nPlusInt	= apet_plus_int;
				apet->UpdateStatPlus( apet_plus_str, apet_plus_dex, apet_plus_int, apet_plus_con );
				
				apet->m_hp = apet_hp;
				apet->m_mp = apet_mp;
				apet->SetFaith(apet_faith);
				apet->SetStamina(apet_stm);
#ifdef APET_AI
				apet->m_cAI = apet_ai;
				apet->m_nAISlot = apet_ai_slot;
#endif

				for(;;)
				{
					tmp[0] = '\0';
					apet_skill = AnyOneArg(apet_skill, tmp);
					if (strlen(tmp) < 1)
						break;
					int skillindex = atoi(tmp);
					tmp[0] = '\0';
					apet_skillLevel = AnyOneArg(apet_skillLevel, tmp);
					if (strlen(tmp) < 1)
						break;
					int skilllevel = atoi(tmp);
					CSkill* skill = gserver.m_skillProtoList.Create(skillindex, skilllevel);
					if (skill)
						apet->m_skillList.Add(skill);
					// 스킬 확인
				}
				ADD_TO_BILIST(apet, pChar->m_pApetlist, m_pPrevPet, m_pNextPet);

				// 펫 인벤 확인	dbChar2
				sprintf(m_buf, "SELECT * FROM t_apets_inven WHERE a_apet_idx=%d ", apet_index);
				dbChar2.SetQuery(m_buf);
				if( dbChar2.Open() && dbChar2.MoveFirst() )
				{
					int pet_inven = 0;
					int idx;
					char wearPos;
					int plus;
					int flag;
					int used;
					int used_2;
					CLCString serial(MAX_SERIAL_LENGTH + 1);
					LONGLONG count;
					short option[MAX_ITEM_OPTION];

					for( pet_inven=0 ; pet_inven< APET_WEARPOINT; pet_inven++)
					{
						char field[128];
						sprintf(field,"a_item_idx%d", pet_inven );
						dbChar2.GetRec(field, idx );
						sprintf(field,"a_wear_pos%d", pet_inven );
						dbChar2.GetRec(field, wearPos );
						sprintf(field,"a_plus%d", pet_inven );
						dbChar2.GetRec(field, plus );
						sprintf(field,"a_flag%d", pet_inven );
						dbChar2.GetRec(field, flag );
						sprintf(field,"a_used%d", pet_inven );
						dbChar2.GetRec(field, used );
						sprintf(field,"a_used%d_2", pet_inven );
						dbChar2.GetRec(field, used_2 );
						sprintf(field,"a_serial%d", pet_inven );
						dbChar2.GetRec(field, serial );
						sprintf(field,"a_count%d", pet_inven );
						dbChar2.GetRec(field, count );

						int k;
						for (k = 0; k < MAX_ITEM_OPTION; k++)
						{
							// option
							ibuf[0] = '\0';					IntCat(ibuf, pet_inven, false);
							strcpy(tbuf, "a_item");			strcat(tbuf, ibuf);
							ibuf[0] ='\0';				IntCat(ibuf, k ,false);
							strcat(tbuf, "_option");	strcat(tbuf, ibuf);

							if (!dbChar2.GetRec(tbuf, option[k]))				continue;
						}
						CItem* item = gserver.m_itemProtoList.CreateDBItem(idx, wearPos, plus, flag, used, used_2, serial, count, option );
						apet->m_wearing[pet_inven] = item;
					}
					apet->ApplyItemValue();
				}
#ifdef APET_AI
				// 공격형 펫 AI
				sprintf(m_buf, "SELECT * FROM t_apets_ai WHERE a_apet_idx=%d and a_char_idx=%d", apet_index, pChar->m_index);
				dbChar2.SetQuery(m_buf);
				if( dbChar2.Open() && dbChar2.MoveFirst() )
				{
					char field[128];
					int nItemNum, nSkillIdx, nSkillType;
					char cUse;

					for (int i = 0; i < apet->m_nAISlot; i++)
					{
						sprintf(field,"a_item_idx%d", i);
						dbChar2.GetRec(field, nItemNum);
						sprintf(field,"a_skill_idx%d", i);
						dbChar2.GetRec(field, nSkillIdx);
						sprintf(field,"a_skill_type%d", i);
						dbChar2.GetRec(field, nSkillType);
						sprintf(field,"a_use%d", i);
						dbChar2.GetRec(field, cUse);
						
						apet->m_tAIData[i].nItemNum		= nItemNum;
						apet->m_tAIData[i].nSkillIdx	= nSkillIdx;
						apet->m_tAIData[i].nSkillType	= nSkillType;
						apet->m_tAIData[i].cUse			= cUse;

						if (apet->m_tAIData[i].cUse)
							apet->m_nAICount++;
					}
				}// dbChar2.Open() - t_apet_ai
#endif
			}
		}
	}
#endif //ATTACK_PET

	LONGLONG nBugFixMoney = 0;
	CLCList<int> listCompositedItem(NULL);

	////////////////
	// 인벤토리 읽기
	memset(m_buf, 0x00, sizeof(m_buf));
	strcpy(m_buf, "SELECT * FROM t_inven");
	IntCat0(m_buf, pChar->m_index % 10, 2, false);
	StrCat(m_buf, "WHERE a_char_idx=");
	IntCat(m_buf, pChar->m_index, false);
	StrCat(m_buf, "ORDER BY a_tab_idx, a_row_idx");

	dbChar.SetQuery(m_buf);

	if (dbChar.Open())
	{
		int tabidx;
		int rowidx;
		int itemidx;
		char wearPos;
		int dbPlus;
		int plus;
		int flag;
		int	used;
#ifdef COMPOSITE_TIME
		int used_2;
#endif // COMPOSITE_TIME
		
		CLCString serial(MAX_SERIAL_LENGTH + 1);
		LONGLONG count;
		short option[MAX_ITEM_OPTION];

		CInventory* inven;

		for (i = 0; i < dbChar.m_nrecords && dbChar.MoveNext(); i++)
		{
			// tab
			if (!dbChar.GetRec("a_tab_idx", tabidx))
				continue ;

			// row
			if (!dbChar.GetRec("a_row_idx", rowidx))
				continue ;

			// row loop
			for (j = 0; j < ITEMS_PER_ROW; j++)
			{
				ibuf[0] = '\0';					IntCat(ibuf, j, false);

				// itemidx
				strcpy(tbuf, "a_item_idx");		strcat(tbuf, ibuf);
				if (!dbChar.GetRec(tbuf, itemidx) || itemidx < 0)	continue ;

				// wear position
				strcpy(tbuf, "a_wear_pos");		strcat(tbuf, ibuf);
				if (!dbChar.GetRec(tbuf, wearPos))					continue ;

				// plus
				strcpy(tbuf, "a_plus");			strcat(tbuf, ibuf);
				if (!dbChar.GetRec(tbuf, dbPlus))					continue ;
#ifdef SET_ITEM
				plus = (dbPlus & 0x0000ffff);
#else
				plus = dbPlus;
#endif // SET_ITEM

				// flag
				strcpy(tbuf, "a_flag");			strcat(tbuf, ibuf);
				if (!dbChar.GetRec(tbuf, flag))						continue ;
				
				// used
				strcpy(tbuf, "a_used");			strcat(tbuf, ibuf);
				if (!dbChar.GetRec(tbuf, used))						continue ;

#ifdef COMPOSITE_TIME				
				strncpy(tbuf, "a_used", 7);			strcat(tbuf, ibuf);	strcat(tbuf, "_2");
				if (!dbChar.GetRec(tbuf, used_2))					continue ;			
#endif // COMPOSITE_TIME

				// serial
				strcpy(tbuf, "a_serial");		strcat(tbuf, ibuf);
				if (!dbChar.GetRec(tbuf, serial))						continue ;

				// count
				strcpy(tbuf, "a_count");		strcat(tbuf, ibuf);
				if (!dbChar.GetRec(tbuf, count))						continue ;
				if (count < 1)											continue ;

				int k;
				for (k = 0; k < MAX_ITEM_OPTION; k++)
				{
					// option
					ibuf[0] = '\0';					IntCat(ibuf, j, false);
					strcpy(tbuf, "a_item");			strcat(tbuf, ibuf);
					ibuf[0] ='\0';				IntCat(ibuf, k ,false);
					strcat(tbuf, "_option");	strcat(tbuf, ibuf);

					if (!dbChar.GetRec(tbuf, option[k]))				continue;
				}

				// 050521 : bs : 케르 넨 레벨 없는 것 12레벨로
				switch (itemidx)
				{
				case 498:
				case 499:
				case 500:
				case 501:
					if (flag == 0)
					{
						DBLOG	<< init("SET LEVEL KER/NEN", pChar)
								<< "ITEM INDEX" << delim
								<< itemidx << delim
								<< "SERIAL" << delim
								<< serial << delim
								<< "LEVEL" << delim
								<< 12
								<< end;
						flag = 12;
					}
					break;
				}
				// --- 050521 : bs : 케르 넨 레벨 없는 것 12레벨로
#ifdef EVENT_WORLDCUP_2006_GIFT
#else
				if( 1485 <= itemidx && itemidx <= 1516 )
				{
					DBLOG << init("FIFACUP END ITEM DELETE", pChar )
						  << "ITEM INDEX" << delim
						  << itemidx << delim
						  << "SERIAL" << delim
						  << serial
						  << end;
					continue;
				}
#endif//#ifdef EVENT_WORLDCUP_2006_GIFT

#ifdef EVENT_COLLECT_BUG
#else
				if( 1577 <= itemidx && itemidx <= 1578 )
				{
					DBLOG << init("COLLECT BUG END ITEM DELETE", pChar )
						  << "ITEM INDEX" << delim
						  << itemidx << delim
						  << "SERIAL" << delim
						  << serial
						  << end;
					continue;
				}
#endif // ifdef EVENT_COLLECT_BUG

#ifdef LC_KOR
				// 국내는 일본 아이템 없음
				switch (itemidx)
				{
				case 1228:
				case 1229:
				case 1230:
				case 1231:
				case 1232:
				case 1233:
				case 1234:
				case 1235:
				case 1236:
				case 1237:
				case 1239:
				case 1240:
				case 1241:
				case 1242:
				case 1243:
				case 1244:
				case 1245:
				case 1246:
				case 1247:
				case 1248:
				case 1249:
				case 1250:
				case 1251:
				case 1252:
				case 1253:
				case 1254:
				case 1255:
				case 1256:
				case 1257:
				case 1258:
				case 1259:
				case 1260:
				case 1261:
				case 1262:
				case 1263:
				case 1264:
				case 1265:
				case 1266:
				case 1267:
				case 1268:
				case 1269:
				case 1270:
				case 1271:
				case 1272:
				case 1273:
				case 1274:
				case 1275:
				case 1276:
				case 1277:
				case 1278:
					DBLOG	<< init("JAPAN ITEM DELETE", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue ;

				default:
					break;
				}
#endif // LC_KOR

#ifdef LC_KOR
				if( itemidx == 8551 )
				{
					DBLOG	<< init("VALENTINE RING ITEM DELETE ", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
					continue;
				}
#endif //LC_KOR

#ifdef LC_KOR // 9월 이벤트 아이템 제거
				if( (itemidx >= 876 && itemidx <= 879) || 
					(itemidx >= 2418 && itemidx <= 2422) ||
#ifndef EVENT_NOM_MOVIE
					(itemidx >= 836 && itemidx <= 838) ||
#endif
					(itemidx >= 2435 && itemidx <= 2445) ||
					itemidx == 2403 )
				{
					DBLOG	<< init("SEPTEMBER EVENT ITEM DELETE ", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
					continue ;
				}
#endif // LC_KOR
				
#ifndef EVENT_LC_1000DAY
				if( (itemidx >= 2435 && itemidx <= 2445) )
				{
					DBLOG	<< init("LC1000DAY EVENT ITEM DELETE ", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
					continue ;
				}
#endif 

//EVENT_RICHYEAR_CHANGE_2008
#if defined(LC_HBK) || defined(LC_MAL) || defined(LC_JPN)
				switch(itemidx)
				{
				case 876:		// 씨앗
				case 877:		// 묘목
				case 878:		// 분재
				case 879:		// 나무
				case 2422:		// 라스트카오스로고
				case 881:		// 배양토
				case 1058:		// 붉은 배양토
				case 1059:		// 푸른 배양토
				case 1060:		// 황금 배양토
					DBLOG << init("RICHYEAR 2008 EVENT ITEM DELETE", pChar)
						  << "ITEM INDEX" << delim
						  << itemidx << delim
						  << "SERIAL" << delim
						  << serial
						  << end;
					continue;
				default:
					break;
				}
#endif // LC_HBK

// #ifdef LC_TLD
// 				switch (itemidx)
// 				{
// 				case 876:
// 				case 877:
// 				case 878:
// 				case 879:
// 				case 2418:
// 				case 2419:
// 				case 2420:
// 				case 2421:
// 				case 2422:
// 					DBLOG << init("RICHYEAR EVENT ITEM DELETE", pChar)
// 						  << "ITEM INDEX" << delim
// 						  << itemidx << delim
// 					 	  << "SERIAL" << delim
// 						  << serial
// 						  << end;
// 					continue;
// 				default:
// 					break;
// 				}
// #endif // LC_TLD


/*
#ifdef LC_TLD
				// 태국 쟈스민 꽃 브로치 및 쟈스민꽃 삭제
				switch (itemidx)
				{
				case 1704:
				case 1705:
			
					DBLOG	<< init("TAI JASMIN  ITEM DELETE", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue ;

				default:
					break;
				}
#endif // LC_TLD
*/
				
#if defined (LC_TLD) || defined (LC_HBK) 
				if( itemidx >= 1571 && itemidx <= 1574 )
				{
					DBLOG << init( "EVENT TLD BUDDHIST", pChar )
						   << "ITEM INDEX" << delim
						   << itemidx << delim
						   << "SERIAL" << delim
						   << serial
						   << end;
					continue;
				}				
#endif // defined (LC_TLD) || defined (LC_HBK) 
/*
#ifdef LC_KOR
				if( itemidx > 2334 && itemidx < 2344 )
				{
					DBLOG << init( "Gomdori item delete", pChar )
						   << "ITEM INDEX" << delim
						   << itemidx << delim
						   << "SERIAL" << delim
						   << serial
						   << end;
					continue;
				}				
#endif // LC_KOR
*/

#if defined(LC_KOR) || defined(LC_MAL)
				switch( itemidx )		// 어린이날, 어버이날, 스승의날 이벤트 아이템 제거
				{
				case 2329:
				case 2330:
				case 2331:
				case 2344:
				case 2349:
//				case 2345:				// 퇴비		//상점에 팔 수 있음
				case 2346:				// 정화수
				case 2351:				// 교환권	// 꽃놀이이벤트 추가
					DBLOG << init( "May Event item delete", pChar )
						   << "ITEM INDEX" << delim
						   << itemidx << delim
						   << "SERIAL" << delim
						   << serial
						   << end;
					continue;
				}				
#endif // LC_KOR

#ifdef LC_BRZ
				switch( itemidx )		// 어린이날, 어버이날, 스승의날 이벤트 아이템 제거
				{
				case 2329:
				case 2330:
				case 2331:
				case 2344:
				case 2349:
				case 2347:				// 정화수
				case 2348:				// 교환권	// 꽃놀이이벤트 추가
					DBLOG << init( "May Event item delete", pChar )
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
					continue;
				}			
#endif // LC_BRZ

#ifdef LC_JPN
#ifndef EVENT_TEACH_2007				
				switch (itemidx)
				{
				case 2329:			// 노란 견장
				case 2330:			// 노란 카네이션
					DBLOG << init( "Teach Event 2007 item delete", pChar )
						   << "ITEM INDEX" << delim
						   << itemidx << delim
						   << "SERIAL" << delim
						   << serial
						   << end;
					continue;
				default:
					break;
				}
#endif // EVENT_TEACH_2007

#ifndef EVENT_SUMMER_VACATION_2007_RESULT
#ifndef EVENT_SUMMER_VACATION_2007		
				switch (itemidx)
				{
				case 2367:			// 파란 종이
				case 2368:			// 빨간 종이
				case 2369:			// 은색 종이
				case 2370:			// 금색 종이
					continue;
				default:
					break;
				}
#endif // EVENT_SUMMER_VACATION_2007
#endif // EVENT_SUMMER_VACATION_2007_RESULT		
#endif // LC_JPN
		
				/*
#ifdef LC_TLD
				switch( itemidx )
				{
				case 2146:
				case 2147:
					DBLOG << init( "Songkran item delete", pChar )
						   << "ITEM INDEX" << delim
						   << itemidx << delim
						   << "SERIAL" << delim
						   << serial
						   << end;
					continue;
				default:
					break;
				}
#endif // LC_TLD
				*/

#if defined ( LC_KOR ) || defined (LC_TLD)
				//편지지 삭제
				switch( itemidx )
				{
				case 2135:
				case 2136:
					DBLOG	<< init("DELETE LETTER PAPER", pChar)
								<< "ITEM INDEX" << delim
								<< itemidx << delim
								<< "SERIAL" << delim
								<< serial
								<< end;
						continue ;
				default:
					break;
				}
#elif defined (LC_GER) || defined (LC_HBK)

				switch( itemidx )
				{
				case 2133:
				case 2134:
				case 2135:
				case 2136:
					DBLOG	<< init("DELETE WHITEDAY EVENT ITEM", pChar)
								<< "ITEM INDEX" << delim
								<< itemidx << delim
								<< "SERIAL" << delim
								<< serial
								<< end;
						continue ;
				default:
					break;
				}
#endif //  if defined ( LC_KOR ) || defined ( LC_TLD )

#if defined ( LC_KOR )
				//붉은색 보석상자 제거
				switch( itemidx )
				{
				case 2660:
				case 2661:
				case 2662:
					DBLOG	<< init("DELETE EVENT TREASUREBOX RED", pChar)
								<< "ITEM INDEX" << delim
								<< itemidx << delim
								<< "SERIAL" << delim
								<< serial
								<< end;
						continue ;
				default:
					break;
				}
#endif // LC_KOR


#ifndef EVENT_HALLOWEEN_2007
#if defined LC_JPN

				if( itemidx >= 2464 && itemidx <= 2475)
				{	// 악마 날개 머리띠, wor dhfoscjs
					DBLOG	<< init("DELETE HALLOWEEN EVENT 2007", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue;
				}

				if(itemidx == 2488 || itemidx == 2489)
				{	// 맛 증표
					DBLOG	<< init("DELETE HALLOWEEN EVENT 2007", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue;
				}
#elif defined LC_TLD
				if(itemidx >= 2482 && itemidx <= 2491)
				{	// 할로윈 이벤트 아이템
					DBLOG	<< init("DELETE HALLOWEEN EVENT 2007", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue;
				}
#elif defined LC_HBK
				switch( itemidx )
				{
				case 2488:
				case 2489:
				case 2490:
				case 2491:
				case 2464:
				case 2465:
				case 2466:
				case 2467:
				case 2468:
				case 2469:
					DBLOG	<< init("DELETE HALLOWEEN EVENT 2007", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue;
				default:
					break;
				}
				
#elif defined (LC_GER)
				if(itemidx >= 2464 && itemidx <= 2491)
				{
					// 할로윈 이벤트 아이템
					DBLOG	<< init("DELETE HALLOWEEN EVENT 2007", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
					continue;
				}
#else
				switch( itemidx )
				{
				case 2488:
				case 2489:
				case 2490:
				case 2491:
					DBLOG	<< init("DELETE HALLOWEEN EVENT 2007", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue;
				default:
					break;
				}
#endif
#endif // #ifndef EVENT_HALLOWEEN_2007

#if defined(LC_HBK) || defined(LC_MAL) || defined (LC_USA) || defined (LC_TLD) || defined (LC_KOR)
			switch( itemidx )
			{
			case 2584:
			case 2585:
			case 2586:
			case 2587:
			case 2588:
			case 2589:
			case 2604:
			case 2606:
			case 2607:
			case 2608:
			case 2609:
			case 2610:
				DBLOG	<< init("DELETE XMAS EVENT 2007", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
				continue;
			default:
				break;
			}
#endif // LC_BRZ

#ifdef LC_BRZ
			switch( itemidx )
			{
			case 2619:
			case 2620:
			case 2621:
			case 2622:
			case 2623:
			case 2624:
			case 2625:
			case 2626:
			case 2627:
			case 2628:
			case 2637:
			case 2638:
			case 2639:
			case 2640:
			case 2641:
				DBLOG	<< init("DELETE CARNIVAL EVENT 2008", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
				continue;
			default:
				break;
			}
#endif // LC_BRZ

#if defined (LC_JPN) || defined (LC_EUR) || defined (LC_GER)
			switch( itemidx )
			{
			case 2148:
				DBLOG	<< init("DELETE EASTER EGG EVENT ", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
				continue;
			default:
				break;
			}
#endif // EVENT_EGGS_HUNT_2007

				// + 없는 버그 스톤보우(531)을 제거
				if (itemidx == 531 && plus < 1)
				{
					for (k = 0; k < MAX_ITEM_OPTION; k++)
					{
						if (option[k] != 0)
							break;
					}
					if (k == MAX_ITEM_OPTION)
					{
						DBLOG	<< init("DELETE BUG STONE BOW", pChar)
								<< "ITEM INDEX" << delim
								<< itemidx << delim
								<< "SERIAL" << delim
								<< serial
								<< end;
						continue ;
					}
				}
				// --- : + 없는 버그 스톤보우(531)을 제거

				CItemProto* pItemProto = gserver.m_itemProtoList.FindIndex(itemidx);				
				if (wearPos != WEARING_NONE)
				{
					if (pItemProto)
					{
						if (!(pItemProto->m_jobFlag & (1 << pChar->m_job)))
							wearPos = -1;
						else if (pItemProto->m_wearing != wearPos)
						{
							if (pItemProto->m_typeIdx != ITYPE_ACCESSORY || wearPos < WEARING_ACCESSORY1 || wearPos > WEARING_ACCESSORY3)
								wearPos = -1;
						}
					}
				}

#ifdef COMPOSITE_TIME
				if (pItemProto && pItemProto->m_flag & ITEM_FLAG_ABS)
				{	// 시간제 아이템
					used += gserver.m_gameTime;	// used는 남은 시간						
				}
#endif	// COMPOSITE_TIME

				// 아이템이 결합용 의상이고 결합된 상태이면 옵션을 row, col로 분리하여 사용한다
				// option[0] : row
				// option[1] : col
				int nIndexEquip = -1;
				if (pItemProto && pItemProto->m_flag & ITEM_FLAG_COMPOSITE)
				{
#ifdef REMOVE_JPN_CLOTHES_ITEM
					if( pItemProto->m_index >= JPN_CLOTHES_ITEM_BEGIN  && pItemProto->m_index <= JPN_CLOTHES_ITEM_END  )
					{
						used = 0;
					}
#endif // REMOVE_JPN_CLOTHES_ITEM

					if (flag & FLAG_ITEM_COMPOSITION)
					{
						nIndexEquip = ((option[0] << 16) & 0xffff0000) | (option[1] & 0x0000ffff);
					}
					memset(option, 0, sizeof(option));

#ifdef COMPOSITE_TIME
					// used_2가 -1인 경우(COMPOSITE_TIME을 위한 기존 아이템 시간 세팅)
					if ((flag & FLAG_ITEM_COMPOSITION)
						&& (used_2 == -1) )
					{
						static const int one_month = 60*60*24*30;						
						if (used - gserver.m_gameTime < one_month)
						{	// 1개월 미만인 경우 결합시간은 유료 아이템 만료 시간과 같다
							used_2 = used;
						}						
						else
						{	// 1개월 이상인 경우 결합시간은 1개월이다.
							used_2 = gserver.m_gameTime + one_month;
						}
					}				
#endif	// COMPOSITE_TIME
				}


				// upgrade 안되는 아이템에 블러드 옵션 제거 후 나스 지급
				if (   pItemProto
					&& (pItemProto->m_typeIdx == ITYPE_WEAPON || pItemProto->m_typeIdx == ITYPE_WEAR)
					&& !pItemProto->CanBloodGem()
#ifdef MONSTER_RAID_SYSTEM
					&& !pItemProto->IsRareItem()
#endif
#ifdef SET_ITEM
					&& !pItemProto->IsOriginItem()
#endif
					&& !( pItemProto->m_index >=2423 && pItemProto->m_index <= 2434 )
					&& !( pItemProto->m_index >=3012 && pItemProto->m_index <= 3017 )
					)
				{



					BLOOD_BUG_DATA bugdata;
					memset(&bugdata, 0, sizeof(bugdata));
					bugdata.nIndex = itemidx;
					strcpy(bugdata.strSerial, serial);
					bugdata.nFlag = (flag & FLAG_ITEM_OPTION_ENABLE);
					bugdata.nOptionCount = 0;
					for (k = 0; k < MAX_ITEM_OPTION; k++)
					{
						bugdata.nOption[k] = option[k];
						if (option[k] != 0)
						{
							bugdata.nOptionCount++;
#ifdef LC_KOR
							nBugFixMoney += 10000;
#else
							nBugFixMoney += 1000;
#endif
							option[k] = 0;
						}
					}
					if (bugdata.nOptionCount > 0)
					{
						pChar->m_listBloodBug.AddToTail(bugdata);
						DBLOG	<< init("UNIQIE OPTION BUG", pChar)
								<< "ITEM INDEX" << delim
								<< itemidx << delim
								<< "SERIAL" << delim
								<< serial << delim
								<< bugdata.nOptionCount
								<< end;
					}
					flag &= ~FLAG_ITEM_OPTION_ENABLE;
				}

				// Item 생성
				CItem* item = gserver.m_itemProtoList.CreateDBItem(itemidx, wearPos, dbPlus, flag, used, 
#ifdef COMPOSITE_TIME
					used_2,
#endif	// COMPOSITE_TIME
					serial, count, option);
				if (!item)
				{
					continue ;
				}
#ifdef MONSTER_RAID_SYSTEM
#ifdef COMPOSITE_RARE_ITEM
#ifndef NO_CHECK_RARE_LEVEL_AND_INDET	// 레벨 제한 해제가 아닌 경우
				if(item->IsRareItem() // 레어 장비이고
					&& item->GetItemLevel(false) > pChar->m_level	// 레벨이 맞지 않은 경우
					&& item->m_wearPos != -1)	// 착용 중
				{	// 아이템을 벗긴다.
					item->m_wearPos = -1;
				}
#endif // NO_CHECK_RARE_LEVEL_AND_INDET
#endif // COMPOSITE_RARE_ITEM
#endif // MONSTER_RAID_SYSTEM

				// 결합된 의상 아이템의 변수에 row, col 값을 설정하고, 인벤을 모두 읽고 나서 index로 변경한다
				item->m_nCompositeItem = nIndexEquip;
				if (nIndexEquip != -1)
				{
					listCompositedItem.AddToTail(item->m_index);
				}

#ifdef ENABLE_PET
				// 펫 리스트에 없으면 아이템 오류
				if (item->IsPet())
				{
					if (!pChar->GetPet(item->m_plus))
					{
						delete item;
						continue ;
					}
				}
#endif // #ifdef ENABLE_PET

#ifdef ATTACK_PET
				// APet 검사 
				if ( item->IsAPet() )
				{
					if (!pChar->GetAPet(item->m_plus))
					{
						delete item;
						continue ;
					}
				}	
#endif //ATTACK_PET



				// 옵션 없는 악세사리 옵션 붙이기
				if (item->IsAccessary() && item->m_nOption == 0)
				{
					OptionSettingItem(pChar, item);
				}
				// --- 옵션 없는 악세사리 옵션 붙이기

				// TODO : DELETE 싱글던전1,2 입장권 퀘스트 인벤에서 일반 인벤으로 이동 코드
				int	tab = tabidx;
				int row = rowidx;
				int col = j;

				if (itemidx == gserver.m_itemProtoList.m_sTicketItem1->m_index 
					|| itemidx == gserver.m_itemProtoList.m_sTicketItem2->m_index 
					|| itemidx == 723)//문스톤도 일반 인벤으로 변화
				{
					if (tab != INVENTORY_NORMAL)
					{
						tab = INVENTORY_NORMAL;
						row = -1;
						col = -1;
					}
				}
				// TODO : 여기까지

				// Item을 인벤으로
				inven = GET_INVENTORY(pChar, tab);
				if (!inven)
				{				
					delete item;
					item = NULL;
				}
				else
				{
#ifdef LC_TLD
					/*
#ifndef EVENT_OPEN_BETA_TLD
					// 
					if( (tab == INVENTORY_EVENT) 
						&& ((item->m_itemProto->m_index == 876)
							|| (item->m_itemProto->m_index == 877)
							|| (item->m_itemProto->m_index == 878)
							|| (item->m_itemProto->m_index == 879)
							|| (item->m_itemProto->m_index == 880)) )
					{
						delete item;
						item = NULL;
						continue;
					}
#endif
					*/
#endif

					if (AddToInventory(pChar, item, false, false, row, col))
					{
						// 겹쳐서 넣었으면
						if (item->tab() == -1)
						{
							// 이전거는 필요 없으니 지우고
							int idnum = item->m_idNum;
							int plus = item->m_plus;
							int flag = item->m_flag;
							delete item;
							item = NULL;

							// 겹쳐진 아이템 찾고
							int r, c;
							if (inven->FindItem(&r, &c, idnum, plus, flag))
								item = inven->GetItem(r, c);
						}

						if (item)
						{
#ifdef ENABLE_PET
							// 펫 아이템이면 리스트에 추가
							if (item->IsPet())
								listPetItem.AddToTail(item);
#endif // ENABLE_PET
#ifdef ATTACK_PET
							if (item->IsAPet())
								listAPetItem.AddToTail(item);
#endif //ATTACK_PET
							// 착용한 아이템 검사 -> 착용 상태 저장
							if (item->m_wearPos >= 0 && item->m_wearPos < MAX_WEARING	// 올바른 번호이고
								&& !pChar->m_wearing[(int)item->m_wearPos])					// 중복 착용이 아니면
							{
								pChar->m_wearing[(int)item->m_wearPos] = item;
#ifdef SET_ITEM
								// 세트 아이템 착용 개수 증가 및 옵션 적용
								if (item->IsOriginItem() && item->m_itemProto->GetSetIndex())
								{
									pChar->CountSetItem(item);
								}
#endif // SET_ITEM
							}
							else
							{
								item->m_wearPos = -1;
							}
							
							// 돈 검사
							if (item->m_itemProto->m_typeIdx == ITYPE_ETC &&
								item->m_itemProto->m_subtypeIdx == IETC_MONEY &&
								pChar->m_moneyItem == NULL)
							{
								pChar->m_moneyItem = item;
							}
						}
					}
					else
					{
						if(itemidx == 723)//문스톤을 일반탭에 넣으려다가 꽉 차있으면 다시 이벤트탭에 넣기.
						{							
							if(!AddToPreInventory(pChar, item, false, false, INVENTORY_EVENT, row, col))
							{
								delete item;
								item = NULL;
							}
						}
						else	
						{
							delete item;
							item = NULL;
						}
					}
				}

			} // for (j = 0; j < ITEMS_PER_ROW; j++)

		} // for (i = 0; i < dbChar.m_nrecords && dbChar.MoveNext(); i++)

	} // if (dbChar.Open())

	// 인벤토리에서 결합된 의상 아이템을 찾아 row, col 값을 index로 변경
	while (listCompositedItem.GetCount() > 0)
	{
		void* posHead = listCompositedItem.GetHead();
		int nIndexClothes = listCompositedItem.GetData(posHead);
		listCompositedItem.Remove(posHead);

		CItem* pItemClothes = pChar->m_invenNormal.GetItem(nIndexClothes);
		int nComposedItemRow = (pItemClothes->m_nCompositeItem >> 16) & 0x0000ffff;
		int nComposedItemCol = pItemClothes->m_nCompositeItem & 0x0000ffff;
		CItem* pItemEquip = pChar->m_invenNormal.GetItem(nComposedItemRow, nComposedItemCol);
		if (pItemEquip)
		{
			pItemClothes->m_nCompositeItem = pItemEquip->m_index;		
		}
		else
		{
			// 없으면 일반 의상으로 변경한다
			pItemClothes->m_nCompositeItem = -1;
			pItemClothes->m_flag &= ~FLAG_ITEM_COMPOSITION;

			// TODO : DBLOG
		}
	}

	// 자동 지급 테이블 읽기
	CLCList<int> listDeleteAutoGive(NULL);
	while (pChar->m_listAutoGive.GetCount() > 0)
		pChar->m_listAutoGive.Remove(pChar->m_listAutoGive.GetHead());

	sprintf(m_buf, "SELECT * FROM t_auto_give WHERE a_char_index=%d", pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (dbChar.Open())
	{
		int			nSeqIndex;
		int			nItemIndex;
		int			nItemPlus;
		int			nItemFlag;
		int			nItemUsed;
#ifdef COMPOSITE_TIME
		int			nItemUsed_2;
#endif	// COMPOSITE_TIME
		LONGLONG	nItemCount;
		short		nItemOption[MAX_ITEM_OPTION];
		CLCString	strSerial(MAX_SERIAL_LENGTH + 1);

		while (dbChar.MoveNext())
		{
			if (
				   dbChar.GetRec("a_index",			nSeqIndex)
				&& dbChar.GetRec("a_item_index",	nItemIndex)
				&& dbChar.GetRec("a_item_plus",		nItemPlus)
				&& dbChar.GetRec("a_item_flag",		nItemFlag)
				&& dbChar.GetRec("a_item_used",		nItemUsed)
#ifdef COMPOSITE_TIME
				&& dbChar.GetRec("a_item_used_2",		nItemUsed_2)
#endif	// COMPOSITE_TIME
				&& dbChar.GetRec("a_item_count",	nItemCount)
				&& dbChar.GetRec("a_item_option0",	nItemOption[0])
				&& dbChar.GetRec("a_item_option1",	nItemOption[1])
				&& dbChar.GetRec("a_item_option2",	nItemOption[2])
				&& dbChar.GetRec("a_item_option3",	nItemOption[3])
				&& dbChar.GetRec("a_item_option4",	nItemOption[4])
				)
			{
				GetSerial(strSerial, nSeqIndex);

				bool bDeleteAutoGive = true;

				// Item 생성
//				CItem* item = gserver.m_itemProtoList.CreateDBItem(nItemIndex, -1, nItemPlus, nItemFlag, nItemUsed, 
//#ifdef COMPOSITE_TIME
//					nItemUsed_2,
//#endif	// COMPOSITE_TIME
//					strSerial, nItemCount, nItemOption);

				CItem* item = gserver.m_itemProtoList.CreateAutoGiveDBItem(nItemIndex, -1, nItemPlus, nItemFlag, nItemUsed, 
#ifdef COMPOSITE_TIME
					nItemUsed_2,
#endif	// COMPOSITE_TIME
					strSerial, nItemCount, nItemOption);

				if (item)
				{
#ifdef ENABLE_PET
					// 펫 리스트에 없으면 아이템 오류
					if (item->IsPet())
					{
						delete item;
						continue ;
					}
#endif // #ifdef ENABLE_PET
#ifdef ATTACK_PET
					if (item->IsAPet())
					{
						delete item;
						continue ;
					}
#endif //ATTACK_PET

					// 옵션 없는 악세사리 옵션 붙이기
					if (item->IsAccessary() && item->m_nOption == 0)
					{
						OptionSettingItem(pChar, item);
					}
					// --- 옵션 없는 악세사리 옵션 붙이기

					// Item을 인벤으로
					CInventory* inven = GET_INVENTORY(pChar, GET_TAB(item->m_itemProto->m_typeIdx, item->m_itemProto->m_subtypeIdx));
					if (inven)
					{
						if (AddToInventory(pChar, item, false, false))
						{
							// 겹쳐서 넣었으면
							if (item->tab() < 0)
							{
								// 이전거는 필요 없으니 지우고
								int idnum = item->m_idNum;
								int plus = item->m_plus;
								int flag = item->m_flag;
								delete item;
								item = NULL;

								// 겹쳐진 아이템 찾고
								int r, c;
								if (inven->FindItem(&r, &c, idnum, plus, flag))
									item = inven->GetItem(r, c);
							}

							if (item)
							{
								// 돈 검사
								if (   item->m_itemProto->m_typeIdx == ITYPE_ETC
									&& item->m_itemProto->m_subtypeIdx == IETC_MONEY
									&& pChar->m_moneyItem == NULL)
								{
									pChar->m_moneyItem = item;
								}
							}
						}
						else
						{
							bDeleteAutoGive = false;
						}
					}
					else
						delete item;
				} // item

				if (bDeleteAutoGive)
				{
					AUTO_GIVE_DATA agdItem;
					agdItem.nIndex = nItemIndex;
					agdItem.nPlus = nItemPlus;
					agdItem.nUsed = nItemUsed;
					agdItem.nFlag = nItemFlag;
					agdItem.nCount = nItemCount;
					agdItem.nOption[0] = nItemOption[0];
					agdItem.nOption[1] = nItemOption[1];
					agdItem.nOption[2] = nItemOption[2];
					agdItem.nOption[3] = nItemOption[3];
					agdItem.nOption[4] = nItemOption[4];
					strcpy(agdItem.strSerial, strSerial);

					pChar->m_listAutoGive.AddToTail(agdItem);

					listDeleteAutoGive.AddToTail(nSeqIndex);
				}
			} // GetRec
		} // MoveNext
	} // Open

	while (listDeleteAutoGive.GetCount() > 0)
	{
		int nSeqIndex = listDeleteAutoGive.GetData(listDeleteAutoGive.GetHead());
		sprintf(m_buf, "DELETE FROM t_auto_give WHERE a_index=%d", nSeqIndex);
		dbChar.SetQuery(m_buf);
		dbChar.Update();
		listDeleteAutoGive.Remove(listDeleteAutoGive.GetHead());
	}

#ifdef ENABLE_PET
	if (pChar->m_petList)
	{
		CPet* pPet;
		CPet* pPetNext = pChar->m_petList;
		while ((pPet = pPetNext))
		{
			pPetNext = pPetNext->m_nextPet;
			CItem* pPetItem = NULL;
			void* pos = listPetItem.GetHead();
			while (pos)
			{
				pPetItem = listPetItem.GetData(pos);
				if (pPetItem && pPetItem->m_plus == pPet->m_index)
					break;
				pPetItem = NULL;
				pos = listPetItem.GetNext(pos);
			}
			if (!pPetItem)
			{
				DBLOG	<< init("PET DELETE (NOT FOUND ITEM)", pChar)
						<< "PET"					<< delim
						<< pPet->m_index			<< delim
						<< pPet->GetPetTypeGrade()	<< delim
						<< pPet->m_level
						<< end;
				REMOVE_FROM_BILIST(pPet, pChar->m_petList, m_prevPet, m_nextPet);
				delete pPet;
				pPet = NULL;
			}
		}
	}
#endif // ENABLE_PET

#ifdef ATTACK_PET
	if (pChar->m_pApetlist)
	{
		CAPet* pAPet;
		CAPet* pAPetNext = pChar->m_pApetlist;
		while ((pAPet = pAPetNext))
		{
			pAPetNext = pAPetNext->m_pNextPet;
			CItem* pPetItem = NULL;
			void* pos = listAPetItem.GetHead();
			while (pos)
			{
				pPetItem = listAPetItem.GetData(pos);
				if (pPetItem && pPetItem->m_plus == pAPet->m_index)
				{
#ifdef APET_AI
					pAPet->CheckAIList();
#endif// APET_AI
					break;
				}
				pPetItem = NULL;
				pos = listAPetItem.GetNext(pos);
			}
			if (!pPetItem)
			{
				DBLOG	<< init("PET DELETE (NOT FOUND ITEM)", pChar)
						<< "PET"					<< delim
						<< pAPet->m_index			<< delim
						<< pAPet->m_level
						<< end;
				REMOVE_FROM_BILIST(pAPet, pChar->m_pApetlist, m_pPrevPet, m_pNextPet);
				delete pAPet;
				pAPet = NULL;
			}
		}
	}
#endif // ATTACK_PET

	// 창고 읽기
	strcpy(m_buf, "SELECT * FROM t_stash");
	IntCat0(m_buf, pChar->m_desc->m_index % 10, 2, false);
	StrCat(m_buf, "WHERE a_user_idx=");
	IntCat(m_buf, pChar->m_desc->m_index, false);
	StrCat(m_buf, "ORDER BY a_index");

	dbChar.SetQuery(m_buf);

	if (dbChar.Open())
	{
		int itemidx;
		int dbPlus;
		int plus;
		int flag;
		CLCString serial(MAX_SERIAL_LENGTH + 1);
		LONGLONG count;
		int	used;
#ifdef COMPOSITE_TIME
		int	used_2;
#endif	// COMPOSITE_TIME
		short option[MAX_ITEM_OPTION];

		for (i = 0; i < dbChar.m_nrecords && dbChar.MoveNext(); i++)
		{
			// itemidx
			if (!dbChar.GetRec("a_item_idx", itemidx) || itemidx < 0)	continue ;

			// plus
			//int tplus = 0;
			if (!dbChar.GetRec("a_plus", dbPlus))						continue ;	
#ifdef SET_ITEM
			plus  = (dbPlus & 0x0000ffff);
#else
			plus = dbPlus;
#endif
			// flag
			if (!dbChar.GetRec("a_flag", flag))							continue ;

			// serial
			if (!dbChar.GetRec("a_serial", serial))						continue ;

			// count
			if (!dbChar.GetRec("a_count", count))						continue ;
			if (count < 1)												continue ;

			// used
			if (!dbChar.GetRec("a_used", used))							continue ;

#ifdef COMPOSITE_TIME			
			if (!dbChar.GetRec("a_used_2", used_2))							continue ;
#endif	// COMPOSITE_TIME

			int k;
			for (k = 0; k < MAX_ITEM_OPTION; k++)
			{
				// option
				strcpy(ibuf, "a_item_option");
				IntCat(ibuf, k, false);

				if (!dbChar.GetRec(ibuf, option[k]))					continue;
			}

			// 050521 : bs : 케르 넨 레벨 없는 것 12레벨로
			switch (itemidx)
			{
			case 498:
			case 499:
			case 500:
			case 501:
				if (flag == 0)
				{
					DBLOG	<< init("SET LEVEL KER/NEN", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial << delim
							<< "LEVEL" << delim
							<< 12
							<< end;
					flag = 12;
				}
				break;
			}
			// --- 050521 : bs : 케르 넨 레벨 없는 것 12레벨로

#ifdef LC_KOR
				// 국내는 일본 아이템 없음
				switch (itemidx)
				{
				case 1228:
				case 1229:
				case 1230:
				case 1231:
				case 1232:
				case 1233:
				case 1234:
				case 1235:
				case 1236:
				case 1237:
				case 1239:
				case 1240:
				case 1241:
				case 1242:
				case 1243:
				case 1244:
				case 1245:
				case 1246:
				case 1247:
				case 1248:
				case 1249:
				case 1250:
				case 1251:
				case 1252:
				case 1253:
				case 1254:
				case 1255:
				case 1256:
				case 1257:
				case 1258:
				case 1259:
				case 1260:
				case 1261:
				case 1262:
				case 1263:
				case 1264:
				case 1265:
				case 1266:
				case 1267:
				case 1268:
				case 1269:
				case 1270:
				case 1271:
				case 1272:
				case 1273:
				case 1274:
				case 1275:
				case 1276:
				case 1277:
				case 1278:
					DBLOG	<< init("JAPAN ITEM DELETE", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue ;

				default:
					break;
				}
#endif // LC_KOR

#ifdef LC_KOR
				if( itemidx == 8551 )
				{
					DBLOG	<< init("VALENTINE RING ITEM DELETE ", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
					continue;
				}
#endif //LC_KOR

#ifdef LC_KOR // 9월 이벤트 아이템 제거
				if( (itemidx >= 876 && itemidx <= 879) || 
					(itemidx >= 2418 && itemidx <= 2422) ||
					(itemidx >= 836 && itemidx <= 838) || 
					(itemidx >= 2435 && itemidx <= 2445) ||
					itemidx == 2403 )
				{
					DBLOG	<< init("SEPTEMBER EVENT ITEM DELETE ", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
					continue ;
				}
#endif // LC_KOR

#ifndef EVENT_LC_1000DAY
				if( (itemidx >= 2435 && itemidx <= 2445) )
				{
					DBLOG	<< init("LC1000DAY EVENT ITEM DELETE ", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
					continue ;
				}
#endif 

#if defined(LC_HBK) || defined(LC_MAL) || defined (LC_JPN)
				switch(itemidx)
				{
				case 876:		// 씨앗
				case 877:		// 묘목
				case 878:		// 분재
				case 879:		// 나무
				case 2422:		// 라스트카오스로고
				case 881:		// 배양토
				case 1058:		// 붉은 배양토
				case 1059:		// 푸른 배양토
				case 1060:		// 황금 배양토
					DBLOG << init("RICHYEAR 2008 EVENT ITEM DELETE", pChar)
						  << "ITEM INDEX" << delim
						  << itemidx << delim
						  << "SERIAL" << delim
						  << serial
						  << end;
					continue;
				default:
					break;
				}
#endif // LC_HBK

// #ifdef LC_TLD
// 				switch (itemidx)
// 				{
// 				case 876:
// 				case 877:
// 				case 878:
// 				case 879:
// 				case 2418:
// 				case 2419:
// 				case 2420:
// 				case 2421:
// 				case 2422:
// 					DBLOG << init("RICHYEAR EVENT ITEM DELETE", pChar)
// 						  << "ITEM INDEX" << delim
// 						  << itemidx << delim
// 					 	  << "SERIAL" << delim
// 						  << serial
// 						  << end;
// 					continue;
// 				default:
// 					break;
// 				}
//#endif // LC_TLD

/*
#ifdef LC_TLD
				// 태국 쟈스민 꽃 브로치 및 쟈스민꽃 삭제
				switch (itemidx)
				{
				case 1704:
				case 1705:
			
					DBLOG	<< init("TAI JASMIN  ITEM DELETE", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue ;

				default:
					break;
				}
#endif // LC_TLD
*/


#if defined (LC_TLD) || defined (LC_HBK) 
				if( itemidx >= 1571 && itemidx <= 1574 )
				{
					DBLOG << init( "EVENT TLD BUDDHIST", pChar )
						   << "ITEM INDEX" << delim
						   << itemidx << delim
						   << "SERIAL" << delim
						   << serial
						   << end;
					continue;
				}				
#endif // defined (LC_TLD) || defined (LC_HBK) 

/*
#ifdef LC_KOR
				if( itemidx > 2334 && itemidx < 2344 )
				{
					DBLOG << init( "Gomdori item delete", pChar )
						   << "ITEM INDEX" << delim
						   << itemidx << delim
						   << "SERIAL" << delim
						   << serial
						   << end;
					continue;
				}				
#endif // LC_KOR
*/
#ifdef LC_KOR
				switch( itemidx )		// 어린이날, 어버이날, 스승의날 이벤트 아이템 제거
				{
				case 2329:
				case 2330:
				case 2331:
				case 2344:
				case 2349:
//				case 2345:				// 퇴비
				case 2346:				// 정화수
				case 2351:				// 교환권	// 꽃놀이이벤트 추가
					DBLOG << init( "May Event item delete", pChar )
						   << "ITEM INDEX" << delim
						   << itemidx << delim
						   << "SERIAL" << delim
						   << serial
						   << end;
					continue;
				}							
#endif // LC_KOR

#if defined (LC_BRZ) || defined (LC_USA)
				switch( itemidx )		// 어린이날, 어버이날, 스승의날 이벤트 아이템 제거
				{
				case 2329:
				case 2330:
				case 2331:
				case 2344:
				case 2349:
				case 2347:				// 정화수
				case 2348:				// 교환권	// 꽃놀이이벤트 추가
					DBLOG << init( "May Event item delete", pChar )
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
					continue;
				}			
#endif // defined (LC_BRZ) || defined (LC_USA)

#ifdef LC_JPN
#ifndef EVENT_TEACH_2007				
				switch (itemidx)
				{
				case 2329:			// 노란 견장
				case 2330:			// 노란 카네이션
					DBLOG << init( "Teach Event 2007 item delete", pChar )
						   << "ITEM INDEX" << delim
						   << itemidx << delim
						   << "SERIAL" << delim
						   << serial
						   << end;
					continue;
				}
#endif // EVENT_TEACH_2007
#endif // LC_JPN

				/*
#ifdef LC_TLD
				switch( itemidx )
				{
				case 2146:
				case 2147:
					DBLOG << init( "Songkran item delete", pChar )
						   << "ITEM INDEX" << delim
						   << itemidx << delim
						   << "SERIAL" << delim
						   << serial
						   << end;
					continue;
				default:
					break;
				}
#endif // LC_TLD
				*/

#if defined ( LC_KOR ) || defined (LC_TLD)
				// 편지지 삭제
				switch( itemidx )
				{
				case 2135:
				case 2136:
					DBLOG	<< init("DELETE LETTER PAPER", pChar)
								<< "ITEM INDEX" << delim
								<< itemidx << delim
								<< "SERIAL" << delim
								<< serial
								<< end;
						continue ;
				default:
					break;
				}
#elif defined (LC_HBK)
				switch( itemidx )
				{
				case 2133:
				case 2134:
				case 2135:
				case 2136:
					DBLOG	<< init("DELETE WHITEDAY EVENT ITEM", pChar)
								<< "ITEM INDEX" << delim
								<< itemidx << delim
								<< "SERIAL" << delim
								<< serial
								<< end;
						continue ;
				default:
					break;
				}
#endif // #if  defined ( LC_KOR ) || defined ( LC_TLD )

#if defined ( LC_KOR )
				//붉은색 보석상자 제거
				switch( itemidx )
				{
				case 2660:
				case 2661:
				case 2662:
					DBLOG	<< init("DELETE EVENT TREASUREBOX RED", pChar)
								<< "ITEM INDEX" << delim
								<< itemidx << delim
								<< "SERIAL" << delim
								<< serial
								<< end;
						continue ;
				default:
					break;
				}
#endif // LC_KOR

#ifndef EVENT_HALLOWEEN_2007
#if defined LC_JPN
				if( itemidx >= 2464 && itemidx <= 2475)
				{	// 악마 날개 머리띠, wor dhfoscjs
					DBLOG	<< init("DELETE HALLOWEEN EVENT 2007", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue;
				}

				if(itemidx == 2488 || itemidx == 2489)
				{	// 맛 증표
					DBLOG	<< init("DELETE HALLOWEEN EVENT 2007", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue;
				}

#elif defined LC_TLD
				if(itemidx >= 2482 && itemidx <= 2491)
				{	// 할로윈 이벤트 아이템
					DBLOG	<< init("DELETE HALLOWEEN EVENT 2007", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
						
				<< "SERIAL" << delim
							<< serial
							<< end;
					continue;
				}
#elif defined LC_HBK
				switch( itemidx )
				{
				case 2488:
				case 2489:
				case 2490:
				case 2491:
				case 2464:
				case 2465:
				case 2466:
				case 2467:
				case 2468:
				case 2469:
					DBLOG	<< init("DELETE HALLOWEEN EVENT 2007", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue;
				default:
					break;
				}
#elif defined (LC_GER)
				if(itemidx >= 2464 && itemidx <= 2491)
				{
					// 할로윈 이벤트 아이템
					DBLOG	<< init("DELETE HALLOWEEN EVENT 2007", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
					continue;
				}
#else
				switch( itemidx )
				{
				case 2488:
				case 2489:
				case 2490:
				case 2491:
					DBLOG	<< init("DELETE HALLOWEEN EVENT 2007", pChar)
							<< "ITEM INDEX" << delim
							<< itemidx << delim
							<< "SERIAL" << delim
							<< serial
							<< end;
					continue;
				default:
					break;
				}
#endif
#endif // #ifndef EVENT_HALLOWEEN_2007

#if defined(LC_HBK) || defined (LC_MAL) || defined (LC_USA) || defined (LC_TLD) || defined (LC_KOR)
			switch( itemidx )
			{
			case 2584:
			case 2585:
			case 2586:
			case 2587:
			case 2588:
			case 2589:
			case 2604:
			case 2606:
			case 2607:
			case 2608:
			case 2609:
			case 2610:
				DBLOG	<< init("DELETE XMAS EVENT 2007", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
				continue;
			default:
				break;
			}
#endif // LC_BRZ

#ifndef EVENT_CARNIVAL
#ifdef LC_BRZ
			switch( itemidx )
			{
			case 2619:
			case 2620:
			case 2621:
			case 2622:
			case 2623:
			case 2624:
			case 2625:
			case 2626:
			case 2627:
			case 2628:
			case 2637:
			case 2638:
			case 2639:
			case 2640:
			case 2641:
				DBLOG	<< init("DELETE CARNIVAL EVENT 2008", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
				continue;
			default:
				break;
			}
#endif // LC_BRZ
#endif // EVENT_CARNIVAL


#if defined (LC_JPN) || defined (LC_EUR) || defined (LC_GER)
			switch( itemidx )
			{
			case 2148:
				DBLOG	<< init("DELETE EASTER EGG EVENT ", pChar)
						<< "ITEM INDEX" << delim
						<< itemidx << delim
						<< "SERIAL" << delim
						<< serial
						<< end;
				continue;
			default:
				break;
			}
#endif // EVENT_EGGS_HUNT_2007

				CItemProto* pItemProto = gserver.m_itemProtoList.FindIndex(itemidx);

				// upgrade 안되는 아이템에 블러드 옵션 제거 후 나스 지급
				if (   pItemProto
					&& (pItemProto->m_typeIdx == ITYPE_WEAPON || pItemProto->m_typeIdx == ITYPE_WEAR)
					&& !pItemProto->CanBloodGem()
#ifdef MONSTER_RAID_SYSTEM
					&& !pItemProto->IsRareItem()
#endif
#ifdef SET_ITEM
					&& !pItemProto->IsOriginItem()
#endif
					&& !( pItemProto->m_index >=2423 && pItemProto->m_index <= 2434 )		
					&& !( pItemProto->m_index >=3012 && pItemProto->m_index <= 3017 )
					)
				{
					BLOOD_BUG_DATA bugdata;
					memset(&bugdata, 0, sizeof(bugdata));
					bugdata.nIndex = itemidx;
					strcpy(bugdata.strSerial, serial);
					bugdata.nFlag = (flag & FLAG_ITEM_OPTION_ENABLE);
					bugdata.nOptionCount = 0;
					for (k = 0; k < MAX_ITEM_OPTION; k++)
					{
						bugdata.nOption[k] = option[k];
						if (option[k] != 0)
						{
							bugdata.nOptionCount++;
#ifdef LC_KOR
							nBugFixMoney += 10000;
#else
							nBugFixMoney += 1000;
#endif
							option[k] = 0;
						}
					}
					if (bugdata.nOptionCount > 0)
					{
						pChar->m_listBloodBug.AddToTail(bugdata);
						DBLOG	<< init("UNIQIE OPTION BUG", pChar)
								<< "ITEM INDEX" << delim
								<< itemidx << delim
								<< "SERIAL" << delim
								<< serial << delim
								<< bugdata.nOptionCount
								<< end;
					}
					flag &= ~FLAG_ITEM_OPTION_ENABLE;
				}

			// Item 생성
			CItem* item = gserver.m_itemProtoList.CreateDBItem(itemidx, -1, dbPlus, flag, used, 
#ifdef COMPOSITE_TIME
				used_2,
#endif	// COMPOSITE_TIME
				serial, count, option);
			if (!item)
			{
				continue ;
			}

			// 옵션 없는 악세사리 옵션 붙이기
			if (item->IsAccessary() && item->m_nOption == 0)
			{
				OptionSettingItem(pChar, item);
			}
			// --- 옵션 없는 악세사리 옵션 붙이기

			if (pChar->m_stash.add(item) == -1)
			{
			}
			else
			{
				item = NULL;
			}
		} // for (i = 0; i < dbChar.m_nrecords && dbChar.MoveNext(); i++)

	} // if (dbChar.Open())

	if (nBugFixMoney > 0)
	{
		if (pChar->AddMoney(nBugFixMoney))
		{
			DBLOG	<< init("UNIQUE OPTION BUG GIVE MONEY", pChar)
					<< nBugFixMoney
					<< end;
		}
		else
		{
			DBLOG	<< init("UNIQUE OPTION BUG GIVE MONEY FAIL", pChar)
					<< nBugFixMoney
					<< end;
		}
	}

	// Quick Slot 불러오기
	strcpy(m_buf, "SELECT * FROM t_quickslot");
	IntCat0(m_buf, pChar->m_index % 10, 2, false);
	StrCat(m_buf, "WHERE a_char_idx=");
	IntCat(m_buf, pChar->m_index, false);
	StrCat(m_buf, "ORDER BY a_char_idx, a_page_idx");

	int page, slot;

	dbChar.SetQuery(m_buf);

	if (dbChar.Open())
	{
		for (i = 0; i < dbChar.m_nrecords && dbChar.MoveNext(); i++)
		{
			// page read
			dbChar.GetRec("a_page_idx", page);
			// slot loop
			pch = dbChar.GetRec("a_slot");
			slot = 0;

			while (*pch && slot < QUICKSLOT_MAXSLOT)
			{
				// slot type
				pch = AnyOneArg(pch, tbuf);
				pChar->m_quickSlot[page].m_slotType[slot] = atoi(tbuf);
				
				switch (pChar->m_quickSlot[page].m_slotType[slot])
				{
					// 비었음
				case QUICKSLOT_TYPE_EMPTY:
					break;
					// Skill type
				case QUICKSLOT_TYPE_SKILL:
					pch = AnyOneArg(pch, tbuf);
					if (pChar->m_activeSkillList.Find(atoi(tbuf)) != NULL)
						pChar->m_quickSlot[page].m_skillType[slot] = atoi(tbuf);
					else
						pChar->m_quickSlot[page].m_slotType[slot] = QUICKSLOT_TYPE_EMPTY;
					break;
					// Action type
				case QUICKSLOT_TYPE_ACTION:
					pch = AnyOneArg(pch, tbuf);
					pChar->m_quickSlot[page].m_actionType[slot] = atoi(tbuf);
					break;
					// Item type : tab row col
				case QUICKSLOT_TYPE_ITEM:
					int row, col;
					pch = AnyOneArg(pch, tbuf);
					row = atoi(tbuf);
					pch = AnyOneArg(pch, tbuf);
					col = atoi(tbuf);

					pChar->m_quickSlot[page].m_item[slot] = pChar->m_invenNormal.GetItem(row, col);

					if (!pChar->m_quickSlot[page].m_item[slot])
						pChar->m_quickSlot[page].m_slotType[slot] = QUICKSLOT_TYPE_EMPTY;

					break;

				default:
					break;
				}
				slot++;
			} // end while

		} // for (i = 0; i < dbChar.m_nrecords && dbChar.MoveNext(); i++)

	} // if (dbQuickSlot.Open())

#ifdef FEEITEM
	// 유료 아이템 사용시간 읽기
	strcpy(m_buf, "SELECT a_mempos_new, a_charslot0_new, a_charslot1_new, a_stashext_new FROM t_cashItemdate WHERE a_portal_idx=");
	IntCat(m_buf, d->m_index, false);
	dbChar.SetQuery(m_buf);
	if (dbChar.Open())
	{
		if (dbChar.MoveNext())
		{
			dbChar.GetRec(CASH_ITEM_DATE_FIELD_MEMPOS, pChar->m_memposTime);
#ifdef CHARSLOTEXT_CARD
			dbChar.GetRec(CASH_ITEM_DATE_FIELD_CHAR_SLOT0, pChar->m_charslotTime[0]);
			dbChar.GetRec(CASH_ITEM_DATE_FIELD_CHAR_SLOT1, pChar->m_charslotTime[1]);
#endif // CHARSLOTEXT_CARD
			dbChar.GetRec(CASH_ITEM_DATE_FIELD_STASHEXT, pChar->m_stashextTime);
		}
	}
#endif // FEEITEM

	// 장소 기억 읽기
	strcpy(m_buf, "SELECT * FROM t_mempos WHERE a_char_idx=");
	IntCat(m_buf, pChar->m_index, false);
	dbChar.SetQuery(m_buf);
	if (dbChar.Open())
	{
		if (dbChar.MoveNext())
		{
			for (i = 0; i < MAX_MEMPOS; i++)
			{
				strcpy(tbuf, "a_mem_");
				IntCat(tbuf, i, false);
				pch = dbChar.GetRec(tbuf);
				if (strlen(pch) < 7)
					continue ;
				char tmp[256];
				int zone;
				float x;
				float z;
				char ylayer;
				char comment[150];

				pch = AnyOneArg(pch, tmp);			zone = atoi(tmp);			*tmp = '\0';
				pch = AnyOneArg(pch, tmp);			x = atof(tmp);				*tmp = '\0';
				pch = AnyOneArg(pch, tmp);			z = atof(tmp);				*tmp = '\0';
				pch = AnyOneArg(pch, tmp);			ylayer = (char)atoi(tmp);	*tmp = '\0';
				strcpy(comment, SkipSpaces(pch));								*tmp = '\0';

				// 해당 존이 없거나
				// 장소 기억 불가능 존이면
				// 설정을 지워버린다
				int zoneidx = gserver.FindZone(zone);
				if (zoneidx < 0 || !gserver.m_zones[zoneidx].m_bCanMemPos)
					continue ;

				pChar->m_mempos.Write(i, zone, x, z, ylayer, comment);
			}
		}
	}

#ifdef PRIMIUM_MEMORYBOOK
	// 장소 기억 읽기
	strcpy(m_buf, "SELECT * FROM t_mempos_plus WHERE a_char_idx=");
	IntCat(m_buf, pChar->m_index, false);
	dbChar.SetQuery(m_buf);
	if (dbChar.Open())
	{
		if (dbChar.MoveNext())
		{
			for (i = 0; i < MAX_MEMPOS; i++)
			{
				strcpy(tbuf, "a_mem_");
				IntCat(tbuf, i, false);
				pch = dbChar.GetRec(tbuf);
				if (strlen(pch) < 7)
					continue ;
				char tmp[256];
				int zone;
				float x;
				float z;
				char ylayer;
				char comment[150];

				pch = AnyOneArg(pch, tmp);			zone = atoi(tmp);			*tmp = '\0';
				pch = AnyOneArg(pch, tmp);			x = atof(tmp);				*tmp = '\0';
				pch = AnyOneArg(pch, tmp);			z = atof(tmp);				*tmp = '\0';
				pch = AnyOneArg(pch, tmp);			ylayer = (char)atoi(tmp);	*tmp = '\0';
				strcpy(comment, SkipSpaces(pch));								*tmp = '\0';

				// 해당 존이 없거나
				// 장소 기억 불가능 존이면
				// 설정을 지워버린다
				int zoneidx = gserver.FindZone(zone);
				if (zoneidx < 0 || !gserver.m_zones[zoneidx].m_bCanMemPos)
					continue ;

				pChar->m_memposplus.Write(i, zone, x, z, ylayer, comment);
			}
		}
	}
#endif // PRIMIUM_MEMORYBOOK

#ifdef ENABLE_SINGLEDUNGEON_DATA
	// 퍼스널 던전 입장 데이터 읽기
	sprintf(m_buf, "SELECT * FROM t_pddata%02d WHERE a_char_index=%d", pChar->m_index % 10, pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (dbChar.Open() && dbChar.MoveFirst())
	{
		int pd3count, pd3time;
		int pd4count, pd4time;

		if (dbChar.GetRec("a_pd3_count", pd3count) && dbChar.GetRec("a_pd3_time", pd3time))
		{
			pChar->m_pd3Count = pd3count;
			pChar->m_pd3Time = pd3time;
		}

		if (dbChar.GetRec("a_pd4_count", pd4count) && dbChar.GetRec("a_pd4_time", pd4time))
		{
			pChar->m_pd4Count = pd4count;
			pChar->m_pd4Time = pd4time;
		}
	}
#endif
// TODO : DELETE
//	// 아이템/스킬 효과 적용
//	pChar->CalcStatus(false);


#ifdef ENABLE_MESSENGER

#ifdef MESSENGER_NEW
	// 그룹 리스트 불러오기
	CLCString gIndexList(255+1);
	CLCString gNameList(255+1);
	int chatColor = 0;

	sprintf(m_buf, "Select a_group_index, a_group_name, a_chat_color from t_messenger_opt where a_char_idx = %d ", pChar->m_index );
	dbChar.SetQuery(m_buf);

	if( dbChar.Open() && dbChar.MoveFirst() )
	{
		dbChar.GetRec("a_group_index", gIndexList);
		dbChar.GetRec("a_group_name", gNameList);
		dbChar.GetRec("a_chat_color", chatColor);

	}
#endif
// 여기서 DB에서 친구리스트 불러오기.
	strcpy(m_buf, "SELECT * FROM t_friend");
	IntCat0(m_buf, pChar->m_index % 10, 2, false);
	StrCat(m_buf, "WHERE a_char_index=");
	IntCat(m_buf, pChar->m_index, false);
	
	
	dbChar.SetQuery(m_buf);
	
	if (dbChar.Open())
	{
		int friend_index;
		
		CLCString name(MAX_CHAR_NAME_LENGTH + 1);
		int job, condition;
		
		d->m_pChar->m_Friend = new CFriend;
		int gIndex = 0;
#ifdef MESSENGER_NEW		
		d->m_pChar->m_Friend->SetChatColor(chatColor);
		pChar->m_Friend->addGroup("NoGroup", 0);
		CLCString groupName(MAX_GROUP_NAME_LENGTH+1);
		const char* pgName = (const char*) gNameList;
		const char* pgIndex = (const char*) gIndexList;

		while(*pgIndex && *pgName)
		{
			pgIndex = AnyOneArg(pgIndex, m_buf);
			gIndex = atoi(m_buf); 
			m_buf[0] = '\0';
			pgName = AnyOneArg(pgName, m_buf); 
			if ( gIndex != 0 )
				pChar->m_Friend->addGroup(m_buf, gIndex);
			m_buf[0] = '\0';
		}
		CLCString Block(2);
#endif
		for (i = 0; i < dbChar.m_nrecords && dbChar.MoveNext(); i++)
		{
			//	if (!dbChar.GetRec("a_index", index) || index < 0)	continue ; //어쩌면 쓸모없을지도...	
			//친구 인덱스
			if (!dbChar.GetRec("a_friend_index", friend_index) || friend_index < 0)	
				continue ;
			
			// 이름
			if (!dbChar.GetRec("a_friend_name", name))					
				continue ;
			
			// 직업
			if (!dbChar.GetRec("a_friend_job", job))					
				continue ;

#ifdef MESSENGER_NEW
			// 그룹 인덱스
			if (!dbChar.GetRec("a_group_index", gIndex))					
				continue ;
#endif			
			
			condition = 0;
			CPC* pc = gserver.m_playerList.Find(friend_index);
			if (pc)
			{				
				condition = pc->m_nCondition;
			}
			
			CFriend* pFriend = d->m_pChar->m_Friend;
			if (pFriend == NULL)
			{
				continue;
			}

			pFriend->AddFriend(friend_index, name, job, MSG_FRIEND_CONDITION_OFFLINE, gIndex);
		}
	}
#ifdef MESSENGER_NEW
	// 차단 리스트 불러오기
	CLCString blockIndexList(255+1);
	CLCString blockNameList(255+1);

	sprintf(g_buf, "select * from t_block_friend where a_char_idx = %d ", pChar->m_index );
	dbChar.SetQuery(g_buf);

	if( dbChar.Open() && dbChar.MoveFirst() )
	{
		dbChar.GetRec("a_block_idx_list", blockIndexList);
		dbChar.GetRec("a_block_name_list", blockNameList);

		const char* pblockIndex = (const char*) blockIndexList;
		const char* pblockName = (const char*) blockNameList;

		int blockIndex = -1;
		while(*pblockIndex && *pblockName)
		{
			pblockIndex = AnyOneArg(pblockIndex, m_buf);
			blockIndex = atoi(m_buf); 
			m_buf[0] = '\0';
			pblockName = AnyOneArg(pblockName, m_buf); 

			pChar->AddBlockPC(blockIndex, m_buf);
			m_buf[0] = '\0';
		}
	}
#endif //#ifdef MESSENGER_NEW
#endif

//0704
#ifdef NOTICE_EVENT
	int cnt = 0;	
	for (i = 0; i < MAX_NOTICE; i++)
	{
		if (gserver.m_aNotice[i] != 0)
		{
#ifdef EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
			if (gserver.m_aNotice[i] == EVENT_JPN_2007_TREASUREBOX_ADD_ITEM)
			{
				d->m_notice[cnt] = gserver.m_aNotice[i];
				cnt++;//카운트 증가
				continue ;
			}
#endif // EVENT_JPN_2007_TREASUREBOX_ADD_ITEM
			sprintf(m_buf, "SELECT * FROM t_notice WHERE a_char_idx=%d AND a_event_idx=%d", pChar->m_index, gserver.m_aNotice[i]);
			
			bool bsuccess = false;
			dbChar.SetQuery(m_buf);

			if (dbChar.Open())
			{
				if(dbChar.MoveNext())
				{
					int count;

					if (!dbChar.GetRec("a_count", count))		continue ;

					if(count >= 3)
						continue;

					d->m_notice[cnt] = gserver.m_aNotice[i];
					cnt++;//카운트 증가

					count++;

					if (pChar->m_admin < 2)
					{
						sprintf(m_buf, "UPDATE t_notice SET"
										" a_count=%d"

										" WHERE a_char_idx=%d AND"
										" a_event_idx=%d",

										count,

										pChar->m_index,
										gserver.m_aNotice[i]);

						dbChar.SetQuery(m_buf);
						if (!dbChar.Update())
						{
							bsuccess = false;
						}
					}
				}
				else if (pChar->m_admin < 2) //테이블에 없다면 생성한다.
				{
					sprintf(m_buf,

						"INSERT INTO t_notice ("

						" a_char_idx,"
						" a_event_idx,"
						" a_count"

						") VALUES ("
						" %d,"
						" %d,"
						" %d"
						")",

						pChar->m_index,
						gserver.m_aNotice[i],
						1
						);

					dbChar.SetQuery(m_buf);
					if (!dbChar.Update())
					{
						bsuccess = false;
					}

					d->m_notice[cnt] = gserver.m_aNotice[i];
					cnt++;//카운트 증가
				}
				else
				{
					d->m_notice[cnt] = gserver.m_aNotice[i];
					cnt++;//카운트 증가
				}
			}
		}
	}
#endif

#ifdef EVENT_PROMOTION_SITE
#ifdef LC_JPN
	// 프로모션 사이트 가입자이면 아래와 같은 루틴으로 아이템 지급
	//if( strcmp(d->m_proSite, "LC") != 0 )
	{
		sprintf(m_buf, "select a_user_idx from t_proSite where a_user_idx = %d", d->m_index );
		dbChar.SetQuery(m_buf);
		if( dbChar.Open() && dbChar.m_nrecords < 1 )
		{
			gserver.m_bpSiteCount--;

			bool lucky = false;

/*			우선 럭키 따위 디스에이블
			// 300명이 받았고 행운을 아무도 안 줬으면 이번 넘을 행운으로 하고 변수 초기화
			// 300명이 받기전이고 이미 행운을 지급하지 않았으면 랜덤 0.3프로
			// 받았는 넘이 300명 넘으면 변수 초기화
			if ( gserver.m_bpSiteCount < 0 && !gserver.m_bpSiteGive )
			{
				lucky = true;
				gserver.m_bpSiteCount = 300;
				gserver.m_bpSiteGive = false;
			}
			else if ( gserver.m_bpSiteCount > 0 && !gserver.m_bpSiteGive )
			{
				// 랜덤 려서 lucky 여부 결정 : 0.3% 확률 
				if( GetRandom(0, 10000) < 30 )
				{
					lucky = true;
					gserver.m_bpSiteGive = true;
				}
			}
			else if ( gserver.m_bpSiteCount < 0 )
			{
				gserver.m_bpSiteCount = 300;
				gserver.m_bpSiteGive = false;
			}
*/ 
			// 4G = 0, NM = 1
			if( strcmp(d->m_proSite, "4G") == 0 )
			{
				this->PromotionGive(pChar, 0, lucky);
			}
			else if( /*strcmp(d->m_proSite, "NM") == 0 || */strcmp(d->m_proSite, "CL") == 0)
			{
				this->PromotionGive(pChar, 1, lucky);
			}
		}
	}
#endif
	
#ifdef GER_SERIAL_PROMOTION
	if( strcmp(d->m_proSite, GER_SERIAL_PROMOTION) == 0 )
	{
		sprintf(m_buf, "select a_user_idx from t_proSite where a_user_idx = %d", d->m_index );
		dbChar.SetQuery(m_buf);
		if( dbChar.Open() && dbChar.m_nrecords < 1 )
		{
			PromotionGive(pChar, 3, false);
		}
	}
#endif // GER_SERIAL_PROMOTION

#endif//#ifdef EVENT_PROMOTION_SITE

	pChar->InitStat();
	pChar->CalcStatus(false);

	CNetMsg smsg;

	// 하드코딩 : 튜토리얼 퀘스트를 수행중이면 스타트존을 ZONE_SINGLE_DUNGEON_TUTORIAL로 대체한다
	if (bGotoTutorial)
		DBOKMsg(smsg, ZONE_SINGLE_DUNGEON_TUTORIAL);
	else
		DBOKMsg(smsg, pChar->m_pZone->m_index);

	d->m_dbOut.Add(smsg, 99999);

#ifdef JP_OTAKU_SYSTEM
	if( pChar->m_etcEvent & OTAKU_SYSTEM )
	{
		pChar->m_maxHP		= 1;
		pChar->m_hp			= 1;
		pChar->m_dbHP	= 1;
		pChar->m_skillPoint = 1;
		pChar->m_maxMP		= 1;
		pChar->m_dbMP	= 1;
		pChar->m_mp		= 1;
	}
#endif // JP_OTAKU_SYSTEM

#ifdef EVENT_JPN_2007_NEWSERVER
	d->m_pChar->m_nEventJPN2007NewServerGift = 0;
	if (gserver.m_bEventJPN2007NewServer)
	{
		if (d->m_userFlag & IS_NEWUSER_JPN_2007_03)
		{
			sprintf(m_buf, "SELECT a_date, a_give FROM t_event_2007_newserver WHERE a_userindex=%d AND a_charindex=%d", d->m_index, d->m_pChar->m_index);
			dbChar.SetQuery(m_buf);
			if (dbChar.Open() && dbChar.MoveFirst())
			{
				int nGive = 0;
				dbChar.GetRec("a_give", nGive);
				if (nGive == 0)
				{
					CLCString strDate(50);
					dbChar.GetRec("a_date", strDate);
					sprintf(m_buf, "SELECT COUNT(a_index) AS a_cnt FROM t_event_2007_newserver WHERE a_date < '%s'", (const char *)strDate);
					dbChar.SetQuery(m_buf);
					if (dbChar.Open() && dbChar.MoveFirst())
					{
						dbChar.GetRec("a_cnt", d->m_pChar->m_nEventJPN2007NewServerGift);
						d->m_pChar->m_nEventJPN2007NewServerGift += 1;
						if (d->m_pChar->m_nEventJPN2007NewServerGift > EVENT_JPN_2007_NEWSERVER_NAS10K )
						{
							gserver.m_bEventJPN2007NewServer = false;
						}
					}
				}
			}
		}
	}
#endif // EVENT_JPN_2007_NEWSERVER

#ifdef JPN_GPARA_PROMOTION
	d->m_pChar->m_nGparaPromotionData = MSG_CONN_GPARA_PROMOTION_COMMIT;
	if (strcmp(d->m_proSite, JPN_GPARA_PROMOTION) == 0)
	{
		sprintf(m_buf,
				"SELECT"
				" a_index, UNIX_TIMESTAMP(a_createdate) AS a_create_time, UNIX_TIMESTAMP('2007-03-08 12:30') AS a_base_time"
				" FROM t_characters"
				" WHERE a_user_index=%d"
				" ORDER BY a_createdate "
				" LIMIT 1",
				d->m_index);
		dbChar.SetQuery(m_buf);
		dbChar.Open();
		dbChar.MoveFirst();
		int nCheckCharIndex = 0;
		int nCreateTime = 0;
		int nBaseTime = 0;
		dbChar.GetRec("a_index",		nCheckCharIndex);
		dbChar.GetRec("a_create_time",	nCreateTime);
		dbChar.GetRec("a_base_time",	nBaseTime);
		if (nCheckCharIndex == d->m_pChar->m_index && nBaseTime <= nCreateTime)
		{
			d->m_pChar->m_nGparaPromotionData = MSG_CONN_GPARA_PROMOTION_QUERY;
		}
	}
#endif // JPN_GPARA_PROMOTION
	
#ifdef JPN_OCN_GOO_PROMOTION
	d->m_pChar->m_nOCN_Goo_PromotionData = MSG_CONN_OCN_GOO_PROMOTION_COMMIT;
	if ( strcmp(d->m_proSite, "OL") == 0 
		  || strcmp(d->m_proSite, "GL") == 0 
		  || strcmp(d->m_proSite, "BC") == 0 
		  || strcmp(d->m_proSite, "JG") == 0 
		)
	{
		sprintf(m_buf,
				"SELECT"
				" a_index, UNIX_TIMESTAMP(a_createdate) AS a_create_time, UNIX_TIMESTAMP('2008-12-02 12:30') AS a_base_time"
				" FROM t_characters"
				" WHERE a_user_index=%d"
				" ORDER BY a_createdate "
				" LIMIT 1",
				d->m_index);
		dbChar.SetQuery(m_buf);
		dbChar.Open();
		dbChar.MoveFirst();
		int nCheckCharIndex = 0;
		int nCreateTime = 0;
		int nBaseTime = 0;
		dbChar.GetRec("a_index",		nCheckCharIndex);
		dbChar.GetRec("a_create_time",	nCreateTime);
		dbChar.GetRec("a_base_time",	nBaseTime);
		if (nCheckCharIndex == d->m_pChar->m_index && nBaseTime <= nCreateTime)
		{
			d->m_pChar->m_nGparaPromotionData = MSG_CONN_OCN_GOO_PROMOTION_QUERY;
		}
	}
#endif // JPN_OCN_GOO_PROMOTION

#ifdef JPN_MSN_PROMOTION
	d->m_pChar->m_nMSNPromotionData = MSG_CONN_MSN_PROMOTION_COMMIT;
	
	sprintf(m_buf,
			"SELECT"
			" a_index, UNIX_TIMESTAMP(a_createdate) AS a_create_time, UNIX_TIMESTAMP('2007-05-25 12:00') AS a_base_time"
//			" a_index, UNIX_TIMESTAMP(a_createdate) AS a_create_time, UNIX_TIMESTAMP('2007-05-18 12:00') AS a_base_time"
			" FROM t_characters"
			" WHERE a_user_index=%d"
			" ORDER BY a_createdate"
			" LIMIT 1",
			d->m_index);
	dbChar.SetQuery(m_buf);
	dbChar.Open();
	dbChar.MoveFirst();
	int nCheckCharIndex = 0;
	int nCreateTime = 0;
	int nBaseTime = 0;
	dbChar.GetRec("a_index",		nCheckCharIndex);
	dbChar.GetRec("a_create_time",	nCreateTime);
	dbChar.GetRec("a_base_time",	nBaseTime);
	if (nCheckCharIndex == d->m_pChar->m_index && nBaseTime <= nCreateTime)
	{
		d->m_pChar->m_nMSNPromotionData = MSG_CONN_MSN_PROMOTION_QUERY;
	}	
#endif // JPN_MSN_PROMOTION

#ifdef CREATE_EVENT
	if(gserver.m_bCreateEvent == true)
	{
		d->m_pChar->m_nCreateEventData = MSG_CONN_CREATEEVENT_COMMIT;		
		sprintf(m_buf,
				"SELECT a_index, UNIX_TIMESTAMP(a_createdate) AS a_create_time "
				" FROM t_characters WHERE a_user_index=%d ORDER BY a_createdate"
				" LIMIT 1", d->m_index);

		dbChar.SetQuery(m_buf);
		dbChar.Open();
		dbChar.MoveFirst();

		int nCheckCharIndex = 0;
		int nCreateTime = 0;
		
		dbChar.GetRec("a_index",		nCheckCharIndex);
		dbChar.GetRec("a_create_time",	nCreateTime);
		
		if (nCheckCharIndex == d->m_pChar->m_index 
			&& gserver.m_nCreateEventStartTime <= nCreateTime
			&& gserver.m_nCreateEventEndTime > nCreateTime)
		{
			d->m_pChar->m_nCreateEventData = MSG_CONN_CREATEEVENT_QUERY;
		}	
	}
#endif // CREATE_EVENT

#ifdef EVENT_TEACH_2007
	sprintf(m_buf, "SELECT a_addflower FROM t_event_teach2007 WHERE a_index=%d", d->m_pChar->m_index);
	dbChar.SetQuery(m_buf);
	if (dbChar.Open() == true)
	{
		// 레코드 추가
		if (dbChar.MoveFirst() == false)
		{
			sprintf(m_buf, "INSERT INTO t_event_teach2007(a_index, a_addflower) VALUES(%d,0)", 
				d->m_pChar->m_index);
			dbChar.SetQuery(m_buf);		
			dbChar.Open();
		}
		else
		{
			unsigned char bFlower = 0;
			dbChar.GetRec("a_addflower", bFlower);
				
			if (bFlower != 0)
			{	// 노란 카네이션 지급				
				if (d->m_pChar->GiveItem(2330, 0, 0, bFlower) == true)
				{
					sprintf(m_buf, "UPDATE t_event_teach2007 set a_addflower=0 WHERE a_index=%d", d->m_pChar->m_index);
					dbChar.SetQuery(m_buf);
					dbChar.Update();
					
					DBLOG << init("TEACH_EVENT_ITEM_GIVE", d->m_pChar)
						<< " give yellow flower";
				}
			}
		}
	}
#endif // EVENT_TEACH_2007

#ifdef EVENT_GOMDORI_2007
	sprintf(m_buf, "SELECT * FROM t_event_gomdori_2007 WHERE a_char_index=%d", d->m_pChar->m_index);
	dbChar.SetQuery(m_buf);
	d->m_pChar->m_nEventGomdori2007FirstLose = 0;
	d->m_pChar->m_nEventGomdori2007FirstWin = 0;
	if (dbChar.Open())
	{
		if (dbChar.MoveFirst())
		{
			int nLose = -1;
			int nWin = -1;
			dbChar.GetRec("a_first_lose", nLose);
			dbChar.GetRec("a_first_lose", nWin);
			dbChar.GetRec("a_total", d->m_pChar->m_nEventGomdori2007Total);
			if (nLose == 0)
			{
				d->m_pChar->m_nEventGomdori2007FirstLose = 1;
			}
			if (nWin == 0)
			{
				d->m_pChar->m_nEventGomdori2007FirstWin = 1;
			}
		}
		else
		{
			d->m_pChar->m_nEventGomdori2007FirstLose = 1;
			d->m_pChar->m_nEventGomdori2007FirstWin = 1;
		}
	}
#endif // EVENT_GOMDORI_2007

#ifdef LAKA_PRESSCORPS
	sprintf(m_buf, "SELECT * FROM t_event_presscorps WHERE a_charindex=%d", d->m_pChar->m_index);
	dbChar.SetQuery(m_buf);
	if(dbChar.Open() && dbChar.MoveFirst())
	{
		d->m_pChar->m_bPressCorps = 1;
	}
#endif // LAKA_PRESSCORPS

	return true;
}
/*
void CDBThread::BackSlash(char* des, const char* src)
{
	while (*src)
	{
		*des++ = *src;
		if (*src == '\\')
			*des++ = '\\';
		src++;
	}
	*des++ = '\0';
}
*/

void CDBThread::GetDateStr(char* buf)
{
	struct tm ti = NOW();
	buf[0] = '\0';
	IntCat0(buf, ti.tm_year + 1900, 4, false);
	strcat(buf, "/");
	IntCat0(buf, ti.tm_mon + 1, 2, false);
	strcat(buf, "/");
	IntCat0(buf, ti.tm_mday, 2, false);
	strcat(buf, " ");
	IntCat0(buf, ti.tm_hour, 2, false);
	strcat(buf, ":");
	IntCat0(buf, ti.tm_min, 2, false);
	strcat(buf, ":");
	IntCat0(buf, ti.tm_sec, 2, false);
}

#ifdef GER_SERIAL_PROMOTION
void CDBThread::PromotionGive(CPC* pChar, int partner_id, bool lucky)
{
	// 1706, 2360 지급
	int pro_item[2][2] = {
		{ 1706, 1, }, 
		{ 2360, 1, },
	};

	int i;
	for(i = 0; i < 2; i++)
	{
		if( !pChar->GiveItem(pro_item[i][0], 0, 0, pro_item[i][1]) )
		{
			// 롤백 
			int r, c;
			for( ; i > 0; i--)
			{
				if( pChar->m_invenNormal.FindItem(&r, &c, pro_item[i][0]) )
				{
					CItem* item = pChar->m_invenNormal.GetItem(r, c);
					if( item )
					{
						CNetMsg itemMsg;

						// Item 수량 변경
						DecreaseFromInventory(pChar, item, pro_item[i][1]);
						
						if (item->Count() > 0)
						{
							ItemUpdateMsg(itemMsg, item, -pro_item[i][1]);
							SEND_Q(itemMsg, pChar->m_desc);
						}
						else
						{
							ItemDeleteMsg(itemMsg, item);
							SEND_Q(itemMsg, pChar->m_desc);
							pChar->RemoveItemFromQuickSlot(item);
							RemoveFromInventory(pChar, item, true, true);
						}
					}
				}
			}

			break;
		}
	}

	//아이템 지급하고 , 해당 userIndex insert
	if( i == 2 )
	{
		CDBCmd dbdata;
		dbdata.Init(&gserver.m_dbdata);

		sprintf(m_buf, "insert into t_prosite_ger (a_user_idx, a_char_idx) values ( %d, %d) ", pChar->m_desc->m_index, pChar->m_index );
		dbdata.SetQuery(m_buf);

		if(!dbdata.Update() )
		{
			// 로르백을 해야징
			// 롤백 
			int r, c;
			for( ; i > 0; i--)
			{
				if( pChar->m_invenNormal.FindItem(&r, &c, pro_item[i][0]) )
				{
					CItem* item = pChar->m_invenNormal.GetItem(r, c);
					if( item )
					{
						CNetMsg itemMsg;

						// Item 수량 변경
						DecreaseFromInventory(pChar, item, pro_item[i][1]);
						
						if (item->Count() > 0)
						{
							ItemUpdateMsg(itemMsg, item, -pro_item[i][1]);
							SEND_Q(itemMsg, pChar->m_desc);
						}
						else
						{
							ItemDeleteMsg(itemMsg, item);
							SEND_Q(itemMsg, pChar->m_desc);
							pChar->RemoveItemFromQuickSlot(item);
							RemoveFromInventory(pChar, item, true, true);
						}
					}
				}
			}
		}
	}
}
#else
void CDBThread::PromotionGive(CPC* pChar, int partner_id, bool lucky)
{
	int i = 0;
	//4G 문스톤 1개 , 넷마블 5000nas 중급회복제 15개
	// 지급자 300명중 1명은 4G 문스톤3, 넷마블 10000nas 중급회복제 20 문스톤 1 
	// 파트너 아이디, 럭키, 아이템순
	// 다시 CL 이나 NM 상품 : 5000나스에 문스톤1개 중급회복약10개

	// 독일 시리얼 입력 한 계정 
		int pro_item[2][2][3] = { 
		{ { 723, -1, -1 }, { 723, -1, -1 } },
		{ { 19, 44, 723 }, { 19, 44, 723 } }
	};

	LONGLONG pro_count[2][2][3] = {
		{ { 1, 0, 0 }, { 3, 0, 0 } },
		{ { 5000, 10, 1 }, { 10000, 20, 1 } }
	};

	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);

	for(i = 0; i < 3; i++)
	{
		if( pro_item[partner_id][lucky][i] == -1 ) continue;

		if( !pChar->GiveItem(pro_item[partner_id][lucky][i], 0, 0, pro_count[partner_id][lucky][i]) )
		{
			// 롤백 
			int r, c;
			for( ; i > 0; i--)
			{
				if( pChar->m_invenNormal.FindItem(&r, &c, pro_count[partner_id][lucky][i]) )
				{
					CItem* item = pChar->m_invenNormal.GetItem(r, c);
					if( item )
					{
						CNetMsg itemMsg;

						// Item 수량 변경
						DecreaseFromInventory(pChar, item, pro_count[partner_id][lucky][i]);
						
						if (item->Count() > 0)
						{
							ItemUpdateMsg(itemMsg, item, -pro_count[partner_id][lucky][i]);
							SEND_Q(itemMsg, pChar->m_desc);
						}
						else
						{
							ItemDeleteMsg(itemMsg, item);
							SEND_Q(itemMsg, pChar->m_desc);
							pChar->RemoveItemFromQuickSlot(item);
							RemoveFromInventory(pChar, item, true, true);
						}
					}
				}
			}

//			DBLOG	<< init("PROMOTION_SITE_ITEM_GIVE_ERROR", pChar->m_name, pChar->m_desc->m_idname)
//					<< partner_id << delim
//					<< lucky << delim
//					<< i << end;

			break;
		}
	}
	//아이템 지급하고 , 해당 userIndex insert
	if( i == 3 )
	{
		sprintf(m_buf, "insert into t_proSite (a_user_idx, a_char_idx) values ( %d, %d) ", pChar->m_desc->m_index, pChar->m_index );
		dbChar.SetQuery(m_buf);

		if(!dbChar.Update() )
		{
			// 로르백을 해야징
			// 롤백 
			int r, c;
			for( ; i > 0; i--)
			{
				if( pChar->m_invenNormal.FindItem(&r, &c, pro_item[partner_id][lucky][i]) )
				{
					CItem* item = pChar->m_invenNormal.GetItem(r, c);
					if( item )
					{
						CNetMsg itemMsg;

						// Item 수량 변경
						DecreaseFromInventory(pChar, item, pro_count[partner_id][lucky][i]);
						
						if (item->Count() > 0)
						{
							ItemUpdateMsg(itemMsg, item, -pro_count[partner_id][lucky][i]);
							SEND_Q(itemMsg, pChar->m_desc);
						}
						else
						{
							ItemDeleteMsg(itemMsg, item);
							SEND_Q(itemMsg, pChar->m_desc);
							pChar->RemoveItemFromQuickSlot(item);
							RemoveFromInventory(pChar, item, true, true);
						}
					}
				}
			}

//			DBLOG	<< init("PROMOTION_SITE_ITEM_GIVE_ERROR_UPDATE", pChar->m_name, pChar->m_desc->m_idname)
//					<< partner_id << delim
//					<< lucky << end;
		}
		else
		{
//			DBLOG	<< init("PROMOTION_SITE_ITEM_GIVE_SUCCESS", pChar->m_name, pChar->m_desc->m_idname)
//					<< partner_id << delim
//					<< lucky << end;
		}
	}
}
#endif // GER_SERIAL_PROMOTION

void CDBThread::CheckSSkill( int & index, int & level, int & totoal_recover_sp)
{
	static int skill_sp[15][7] = {
//	a_index,a_level0_need_sp,a_level1_need_sp,a_level2_need_sp,a_preference,a_need_sskill,a_need_sskill_level
		{3,25,32,40,2,2,3,},
		{4,48,70,100,3,3,3,},
		{7,25,32,40,2,6,3,},
		{8,48,70,100,3,7,3,},
		{11,25,32,40,2,10,3,},
		{12,48,70,100,3,11,3,},
		{16,25,40,55,-1,15,1,},
		{17,54,70,90,-1,16,1,},
		{21,25,40,55,-1,20,1,},
		{22,54,70,90,-1,21,1,},
		{26,25,40,55,-1,25,1,},
		{27,54,70,90,-1,26,1,},
		{31,196,210,229,3,30,3,},
		{48,196,210,229,2,34,3,},
		{47,149,167,197,3,46,3,},
		};

	for(int i=0; i < 15; i++)
	{
		if(skill_sp[i][0] == index)
		{
			DBLOG << init("sskill delete ")
				<< delim << index << delim << level << delim
				<< end;

			for(int j=level; j > 0; j--)
			{
				totoal_recover_sp += skill_sp[i][j];
			}

			if(skill_sp[i][4] != -1)
			{	// 한단계 낮은 스킬로 복구
				index = skill_sp[i][5];
				level = skill_sp[i][6];
				CheckSSkill(index, level, totoal_recover_sp);
			}
			else
			{
				index = -1;
			}

			return;
		}
	}
}
