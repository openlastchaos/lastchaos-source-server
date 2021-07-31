#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"


void do_Teach(CPC* ch, CNetMsg& msg)
{
	if (DEAD(ch))	return;

	unsigned char subtype;

	msg.MoveFirst();

	msg >> subtype;

	switch (subtype)
	{
	case MSG_TEACH_TEACHER_LIST:
		do_TeachTeacherList(ch, msg);
		break;

	case MSG_TEACH_TEACHER_REQ:
		{
			unsigned char type;
			msg >> type;

			switch (type)
			{
			case MSG_TEACH_TEACHER_REQ_REQ:
				do_TeachTeacherReqReq(ch, msg);
				break;

			case MSG_TEACH_TEACHER_REQ_REJECT:
				do_TeachTeacherReqReject(ch, msg);
				break;

			case MSG_TEACH_TEACHER_REQ_ACCEPT:
				do_TeachTeacherReqAccept(ch, msg);
				break;

			case MSG_TEACH_TEACHER_GIVEUP:
				do_TeachTeacherGiveUP(ch, msg);
				break;

			default:
				break;
			}
		}
		break;

	case MSG_TEACH_INFO:
		do_TeachInfo(ch, msg);
		break;

	default:
		break;
	}
}

void do_TeachTeacherList(CPC* ch, CNetMsg& msg)
{
	unsigned char subtype;
	msg >> subtype;

	CNetMsg rmsg;

	switch (subtype)
	{
	case MSG_TEACH_TEACHER_LIST_UP:
		
		// 레벨 조건 미달
		if (ch->m_level < TEACH_LEVEL_TEACHER)
		{
			TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_LEVEL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		// 이미 선생리스트에 있음
		if (ch->m_bTeacher)
		{
			TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_LIST_ALREADY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		ch->m_bTeacher = true;

		TeachTeacherListMsg(rmsg, ch, MSG_TEACH_TEACHER_LIST_UP, NULL);
		SEND_Q(rmsg, ch->m_desc);

		if(IS_RUNNING_HELPER)
		{
			HelperTeachMsg(rmsg, ch->m_index, ch->m_bTeacher);
			SEND_Q(rmsg, gserver.m_helper);
		}
		break;

	case MSG_TEACH_TEACHER_LIST_DN:

		// 레벨 조건 미달
		if (ch->m_level < TEACH_LEVEL_TEACHER)
			return;

		// 선생리스트에 있지 않음
		if (!ch->m_bTeacher)
		{
			TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_LIST_NOT);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		ch->m_bTeacher = false;

		TeachTeacherListMsg(rmsg, ch, MSG_TEACH_TEACHER_LIST_DN, NULL);
		SEND_Q(rmsg, ch->m_desc);

		if(IS_RUNNING_HELPER)
		{
			HelperTeachMsg(rmsg, ch->m_index, ch->m_bTeacher);
			SEND_Q(rmsg, gserver.m_helper);
		}

		break;
		
	case MSG_TEACH_TEACHER_LIST_SHOW:
		{
			// 이미 신청중이다
			if (ch->m_teachWait != -1)
			{
				TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_LIST_NOT);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			 // 선생이 있으면 이 msg를 보낼 수 없다
			if (ch->m_teachIdx[0] != -1)
				return;

			// 선생이 있으면 이 msg를 보낼 수 없다
			if (ch->m_teachType == MSG_TEACH_STUDENT_TYPE) 
				return;

			if (ch->m_teachType == MSG_TEACG_NO_STUDENT_TYPE)
			{
				TeachErrMsg(rmsg, MSG_TEACH_ERR_NOMORE_STUDENT);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 제자 가능 레벨이 아님
			if (ch->m_level > TEACH_LEVEL_STUDENT)
			{
				TeachErrMsg(rmsg, MSG_TEACH_ERR_STUDENT_LEVEL);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			int* tlist = new int[gserver.m_playerList.m_max];
			memset(tlist, -1, sizeof(int) * gserver.m_playerList.m_max);
			int cnt = 0;

			int i;
			for (i = 0; i < gserver.m_playerList.m_max; i++)
			{
				if (gserver.m_playerList.m_pcList[i] && gserver.m_playerList.m_pcList[i]->m_bTeacher)
				{
					tlist[cnt] = i;
					cnt++;
				}
			}

			CPC* list[20];
			memset(list, 0, sizeof(CPC*) * 20);

			int idx;
			for (i = 0; i < 20; i++)
			{
				idx = GetRandom(0, cnt - 1);
				int j, k;
				for (j = 0; j < cnt; j++)
				{
					k = (idx + j) % cnt;

					if (tlist[k] != -1)
					{
						list[i] = gserver.m_playerList.m_pcList[tlist[k]];
						tlist[k] = -1;
						break;
					}
				}
				if (j == cnt)
					break;
			}
			
			TeachTeacherListMsg(rmsg, ch, MSG_TEACH_TEACHER_LIST_SHOW, list);
			SEND_Q(rmsg, ch->m_desc);
		}

		break;

	default:
		break;

	}
}

void do_TeachTeacherReqReq(CPC* ch, CNetMsg& msg)
{
	int charindex;

	msg >> charindex;

	CNetMsg rmsg;

	// 레벨 초과
	if (ch->m_level > TEACH_LEVEL_STUDENT)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_STUDENT_LEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 이미 선생 있음
	if (ch->m_teachIdx[0] != -1)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_HAVE_TEACHER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 이미 신청대기중인 선생이 있음
	if (ch->m_teachWait != -1)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_REQ_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 사제 관계 맺었다가 실패한 경우
	if (ch->m_teachType == MSG_TEACG_NO_STUDENT_TYPE)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_NOMORE_STUDENT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 선생이 없음
	CPC* tch = gserver.m_playerList.Find(charindex);

	if (!tch)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_NOT_CONNECT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 선생이 선생리스트에 없음
	if (!tch->m_bTeacher)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_LIST_NOT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 이미 대기중인 학생이 있음
	if (tch->m_teachWait != -1)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_REQ_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	bool bTeacher = false;

	int i;
	for (i=0; i < TEACH_MAX_STUDENTS; i++)
	{
		if (tch->m_teachIdx[i] == -1)
		{
			bTeacher = true;
			break;
		}
	}

	// 선생이 제자가 꽉 차 있음
	if (!bTeacher)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_FULL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 대기자 셋팅
	tch->m_teachWait = ch->m_index;
	ch->m_teachWait = tch->m_index;

	// 선생에게 요청
	TeachTeacherReqReqMsg(rmsg, ch);
	SEND_Q(rmsg, tch->m_desc);

}

void do_TeachTeacherReqReject(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	// 대기명단이 없음
	if (ch->m_teachWait == -1)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_NOE_REQ);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	CPC* tch = gserver.m_playerList.Find(ch->m_teachWait);
	if (!tch)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_SYSTEM);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		return;
	}
	
	// 대기 명단이 없거나 같지 않거나
	if (tch->m_teachWait == -1 || tch->m_teachWait != ch->m_index)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_NOE_REQ);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		return;
	}
	
	ch->m_teachWait = -1;
	tch->m_teachWait = -1;
	
	TeachTeacherReqRejectMsg(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);
	SEND_Q(rmsg, tch->m_desc);
}

void do_TeachTeacherReqAccept(CPC* ch, CNetMsg& msg)
{
	CNetMsg rmsg;

	// 대기 학생이 없음
	if (ch->m_teachWait == -1)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_NOE_REQ);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	// 선생리스트에 등록되어 있지 않음
	if (!ch->m_bTeacher)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_LIST_NOT);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		return;
	}
	
	CPC* tch = gserver.m_playerList.Find(ch->m_teachWait);
	if (!tch)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_STUDENT_NOT_CONNECT);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		return;
	}
	
	// 대기 선생이 없거나 같지 않거나
	if (tch->m_teachWait == -1 || tch->m_teachWait != ch->m_index)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_NOE_REQ);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		return;
	}

	if (tch->m_teachType == MSG_TEACG_NO_STUDENT_TYPE)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_NOMORE_STUDENT);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		tch->m_teachWait = -1;
		return;
	}
	
	bool bTeacher = false;
	
	int i;
	for (i=0; i < TEACH_MAX_STUDENTS; i++)
	{
		if (ch->m_teachIdx[i] == -1)
		{
			bTeacher = true;
			break;
		}
	}
	
	// 선생이 제자가 꽉 차 있음
	if (!bTeacher)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_FULL);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		tch->m_teachWait = -1;
		return;
	}
	
	time_t ti = time(0);

	ch->m_teachIdx[i] = tch->m_index;
	ch->m_teachJob[i] = tch->m_job;
#ifdef ENABLE_CHANGEJOB
	ch->m_teachJob2[i] = tch->m_job2;
#endif
	ch->m_teachLevel[i] = tch->m_level;
	strcpy(ch->m_teachName[i], tch->GetName() );
	ch->m_teachTime[i] = (int)ti;

	tch->m_teachIdx[0] = ch->m_index;
	tch->m_teachJob[0] = ch->m_job;
#ifdef ENABLE_CHANGEJOB
	tch->m_teachJob2[0] = ch->m_job2;
#endif
	tch->m_teachLevel[0] = ch->m_level;
	strcpy(tch->m_teachName[0], ch->GetName());
	tch->m_teachTime[0] = (int)ti;

	// 선생 리스트 검사해서 꽉 찾으면 후보에서 내림
	for (i=0; i < TEACH_MAX_STUDENTS; i++)
	{
		if (ch->m_teachIdx[i] == -1)
			break;
	}

	// 후보에서 제외
	if (i == TEACH_MAX_STUDENTS)
	{
		ch->m_bTeacher = false;
	}

	ch->m_teachWait = -1;
	ch->m_teachType = MSG_TEACH_TEACHER_TYPE;

	tch->m_teachWait = -1;
	tch->m_teachType = MSG_TEACH_STUDENT_TYPE;
//0627	
	tch->m_cntTeachingStudent++;
	// msg
	TeachTeacherReqAcceptMsg(rmsg, ch, tch);
	SEND_Q(rmsg, ch->m_desc);
	SEND_Q(rmsg, tch->m_desc);

	TeachInfoMsg(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);

	TeachInfoMsg(rmsg, tch);
	SEND_Q(rmsg, tch->m_desc);

	// 헬퍼에 등록 요청
	// teachidx bteacher studentidx 
	if(IS_RUNNING_HELPER)
	{
		HelperTeachRegisterMsg(rmsg, ch->m_index, ch->m_bTeacher, tch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		GAMELOG << init("TEACH_START_FAIL-not helper", tch)
				<< ch->m_name << delim
				<< ch->m_nick << delim
				<< ch->m_desc->m_idname << delim
				<< ch->m_fame << end;
		return;
	}

	GAMELOG << init("TEACH_START", tch)
			<< ch->m_name << delim
			<< ch->m_nick << delim
			<< ch->m_desc->m_idname << delim
			<< ch->m_fame << end;

}

void do_TeachInfo(CPC* ch, CNetMsg& msg)
{
	if (ch->m_teachType == MSG_TEACH_NO_TYPE)
		return;

	CNetMsg rmsg;
	TeachInfoMsg(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);
}

void do_TeachTeacherGiveUP(CPC* ch, CNetMsg& msg)
{
	int charindex;
	CNetMsg rmsg;

	msg >> charindex;

	// 학생이 아님
	if (ch->m_teachType != MSG_TEACH_STUDENT_TYPE)
	{	
		TeachErrMsg(rmsg, MSG_TEACH_ERR_SYSTEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 선생이 없음
	if (ch->m_teachIdx[0] == -1)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_SYSTEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CPC* tch = gserver.m_playerList.Find(charindex);

	// 선생이 접속중이 아님
	if (!tch)
	{
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_NOT_CONNECT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	TeachTeacherGiveUPMsg(rmsg, tch, ch);

//0627
	tch->m_cntFailStudent++;

	// 셋팅해제
	do_TeachSetCancel(tch, ch);

	SEND_Q(rmsg, ch->m_desc);
	SEND_Q(rmsg, tch->m_desc);

	// 더이상 학생 못함
	ch->m_teachType = MSG_TEACG_NO_STUDENT_TYPE;

	if(IS_RUNNING_HELPER)
	{
		HelperTeacherGiveup(rmsg, tch->m_index, ch->m_index);
		SEND_Q(rmsg, gserver.m_helper);
	}
	else
	{
		GAMELOG << init("TEACH_FAIL_GIVEUP-not_helper", tch)
				<< tch->m_name << delim
				<< tch->m_nick << delim
				<< tch->m_desc->m_idname << delim
				<< tch->m_fame << delim
				<< ch->m_name << delim
				<< ch->m_nick << delim
				<< ch->m_desc->m_idname << delim
				<< end;
		return;
	}

	GAMELOG << init("TEACH_FAIL_GIVEUP", tch)
			<< tch->m_name << delim
			<< tch->m_nick << delim
			<< tch->m_desc->m_idname << delim
			<< tch->m_fame << delim
			<< ch->m_name << delim
			<< ch->m_nick << delim
			<< ch->m_desc->m_idname << delim
			<< end;
}

// 셋팅 해제
void do_TeachSetCancel(CPC* teacher, CPC* student)
{
	if (!teacher || !student)
		return;

	// 학생이 있는가
	bool bStudent = false;
	int i;
	for (i=0; i < TEACH_MAX_STUDENTS; i++)
	{
		if (teacher->m_teachIdx[i] == -1)
			continue;

		if (teacher->m_teachIdx[i] == student->m_index)
		{
			bStudent = true;
			break;
		}
	}

	// 학생 목록에 있을때
	if (bStudent)
	{
		// 셋팅해제
		teacher->m_teachIdx[i] = -1;
		teacher->m_teachJob[i] = -1;
#ifdef ENABLE_CHANGEJOB
		teacher->m_teachJob2[i] = 0;
#endif
		teacher->m_teachLevel[i] = 0;
		teacher->m_teachName[i][0] = '\0';
		teacher->m_teachTime[i] = 0;
		
		// 남은 학생 체크 여부
		bool bTeacher = true;
		for (i=0; i < TEACH_MAX_STUDENTS; i++)
		{
			if (teacher->m_teachIdx[i] != -1)
				bTeacher = false;
		}
		
		// 선생박탈
		if (bTeacher)
			teacher->m_teachType = MSG_TEACH_NO_TYPE;
	}

	student->m_teachIdx[0] = -1;
	student->m_teachJob[0] = -1;
#ifdef ENABLE_CHANGEJOB
	student->m_teachJob2[0] = 0;
#endif
	student->m_teachLevel[0] = 0;
	student->m_teachName[0][0] = '\0';
	student->m_teachTime[0] = 0;
	student->m_teachType = MSG_TEACH_NO_TYPE;
}

void do_TeachSetCancel(CPC* teacher, int studentidx)
{
	if (!teacher || !studentidx)
		return;

	// 학생이 있는가
	bool bStudent = false;
	int i;
	for (i=0; i < TEACH_MAX_STUDENTS; i++)
	{
		if (teacher->m_teachIdx[i] == -1)
			continue;

		if (teacher->m_teachIdx[i] == studentidx)
		{
			bStudent = true;
			break;
		}
	}

	// 학생 목록에 있을때
	if (bStudent)
	{
		// 셋팅해제
		teacher->m_teachIdx[i] = -1;
		teacher->m_teachJob[i] = -1;
#ifdef ENABLE_CHANGEJOB
		teacher->m_teachJob2[i] = 0;
#endif
		teacher->m_teachLevel[i] = 0;
		teacher->m_teachName[i][0] = '\0';
		teacher->m_teachTime[i] = 0;
		
		// 남은 학생 체크 여부
		bool bTeacher = true;
		for (i=0; i < TEACH_MAX_STUDENTS; i++)
		{
			if (teacher->m_teachIdx[i] != -1)
				bTeacher = false;
		}
		
		// 선생박탈
		if (bTeacher)
			teacher->m_teachType = MSG_TEACH_NO_TYPE;
	}
}