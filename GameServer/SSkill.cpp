// SSkill.cpp: implementation of the CSSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include "Server.h"
#include "DBCmd.h"
#include "CmdMsg.h"
#include "SSkill.h"
#include "Log.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSSkillProto::CSSkillProto()
{
	m_index = 0;
	m_job = -1;
	m_type = -1;
	m_maxLevel = 0;
	m_preference = -1;

	memset(m_needLevel, 0, sizeof(int) * SSKILL_MAX_LEVEL);
	memset(m_needSP, 0, sizeof(int) * SSKILL_MAX_LEVEL);
	memset(m_num0, 0, sizeof(int) * SSKILL_MAX_LEVEL);
	memset(m_num1, 0, sizeof(int) * SSKILL_MAX_LEVEL);
	memset(m_num2, 0, sizeof(int) * SSKILL_MAX_LEVEL);
	memset(m_num3, 0, sizeof(int) * SSKILL_MAX_LEVEL);
	memset(m_num4, 0, sizeof(int) * SSKILL_MAX_LEVEL);

	m_needSSkill = 0;
	m_needSSkillLevel = 0;
}

CSSkill::CSSkill(CSSkillProto* proto, char level)
{
	if (!proto)
		return;

	m_proto = proto;
	m_level = level;
}


CSSkillProtoList::CSSkillProtoList()
{
	m_proto = NULL;
	m_nCount = 0;
}

CSSkillProtoList::~CSSkillProtoList()
{
	if (m_proto)
		delete [] m_proto;

	m_proto = NULL;
}

bool CSSkillProtoList::Load()
{
	CDBCmd dbSSkill;
	dbSSkill.Init(&gserver.m_dbdata);
	dbSSkill.SetQuery("SELECT * FROM t_special_skill WHERE a_enable=1 ORDER BY a_index");

	if (!dbSSkill.Open())
		return false;

	m_nCount = dbSSkill.m_nrecords;
	m_proto = new CSSkillProto[m_nCount];

	// 스킬별 설정
	int i, j;

	if(!dbSSkill.MoveFirst())
		return true;

	for (i=0; i < m_nCount; i++)
	{
		dbSSkill.GetRec("a_index",			m_proto[i].m_index);
		dbSSkill.GetRec("a_job",			m_proto[i].m_job);
		dbSSkill.GetRec("a_type",			m_proto[i].m_type);
		dbSSkill.GetRec("a_max_level",		m_proto[i].m_maxLevel);
		dbSSkill.GetRec("a_preference",		m_proto[i].m_preference);

		for (j=0; j < SSKILL_MAX_LEVEL; j++)
		{
			strcpy(g_buf, "a_level");		IntCat(g_buf, j, false);	strcat(g_buf, "_need_level");
			dbSSkill.GetRec(g_buf,			m_proto[i].m_needLevel[j]);
			strcpy(g_buf, "a_level");		IntCat(g_buf, j, false);	strcat(g_buf, "_need_sp");
			dbSSkill.GetRec(g_buf,			m_proto[i].m_needSP[j]);
			strcpy(g_buf, "a_level");		IntCat(g_buf, j, false);	strcat(g_buf, "_num0");
			dbSSkill.GetRec(g_buf,			m_proto[i].m_num0[j]);
			strcpy(g_buf, "a_level");		IntCat(g_buf, j, false);	strcat(g_buf, "_num1");
			dbSSkill.GetRec(g_buf,			m_proto[i].m_num1[j]);
			strcpy(g_buf, "a_level");		IntCat(g_buf, j, false);	strcat(g_buf, "_num2");
			dbSSkill.GetRec(g_buf,			m_proto[i].m_num2[j]);
			strcpy(g_buf, "a_level");		IntCat(g_buf, j, false);	strcat(g_buf, "_num3");
			dbSSkill.GetRec(g_buf,			m_proto[i].m_num3[j]);
			strcpy(g_buf, "a_level");		IntCat(g_buf, j, false);	strcat(g_buf, "_num4");
			dbSSkill.GetRec(g_buf,			m_proto[i].m_num4[j]);

			strcpy(g_buf, "a_level");		IntCat(g_buf, j, false);	strcat(g_buf, "_num5");
			dbSSkill.GetRec(g_buf,			m_proto[i].m_num5[j]);
			strcpy(g_buf, "a_level");		IntCat(g_buf, j, false);	strcat(g_buf, "_num6");
			dbSSkill.GetRec(g_buf,			m_proto[i].m_num6[j]);
			strcpy(g_buf, "a_level");		IntCat(g_buf, j, false);	strcat(g_buf, "_num7");
			dbSSkill.GetRec(g_buf,			m_proto[i].m_num7[j]);
			strcpy(g_buf, "a_level");		IntCat(g_buf, j, false);	strcat(g_buf, "_num8");
			dbSSkill.GetRec(g_buf,			m_proto[i].m_num8[j]);
			strcpy(g_buf, "a_level");		IntCat(g_buf, j, false);	strcat(g_buf, "_num9");
			dbSSkill.GetRec(g_buf,			m_proto[i].m_num9[j]);
		}

		dbSSkill.GetRec("a_need_sskill",		m_proto[i].m_needSSkill);
		dbSSkill.GetRec("a_need_sskill_level",	m_proto[i].m_needSSkillLevel);

		dbSSkill.MoveNext();
	}

	return true;
}

CSSkillProto* CSSkillProtoList::FindProto(int index)
{
	CSSkillProto proto;
	proto.m_index = index;

	return (CSSkillProto*)bsearch(&proto, m_proto, m_nCount, sizeof(CSSkillProto), CompIndex);
}

CSSkill* CSSkillProtoList::Create(int index, int level)
{
	CSSkillProto* proto = FindProto(index);

	if (!proto)
		return NULL;

	return new CSSkill(proto, level);
}

CSSkillList::CSSkillList()
{
	m_nCount = 0;
	m_head = NULL;
}

CSSkillList::~CSSkillList()
{
	m_nCount = 0;
	
	CSSkillNode* node = NULL;

	while (m_head)
	{
		node = m_head->m_next;
		delete m_head;
		m_head = node;
	}

	m_head = NULL;
}

void CSSkillList::Init()
{
	m_nCount = 0;
	m_head = NULL;
}

bool CSSkillList::Add(CSSkill* sskill)
{
	if (!sskill)
		return false;

	CSSkillNode* node = new CSSkillNode(sskill);
	ADD_TO_BILIST(node, m_head, m_prev, m_next);
	m_nCount++;

	return true;
}

bool CSSkillList::Remove(CSSkill* sskill)
{
	if (!sskill)
		return false;

	CSSkillNode* node = m_head;
	while (node)
	{
		if (node->m_sskill->m_proto->m_index == sskill->m_proto->m_index)
		{
			REMOVE_FROM_BILIST(node, m_head, m_prev, m_next);
			m_nCount--;
			delete node;
			node = NULL;
			return true;
		}
		node = node->m_next;
	}

	return false;
}

CSSkill* CSSkillList::Find(int index)
{
	CSSkillNode* node = m_head;

	while (node)
	{
		if (node->m_sskill->m_proto->m_index == index)
			return node->m_sskill;
		else
			node = node->m_next;
	}

	return NULL;
}

char* CSSkillList::GetString(char* buf)
{
	*buf = '\0';
	CSSkillNode* node = m_head;

	while (node)
	{
		IntCat(buf, node->m_sskill->m_proto->m_index);
		IntCat(buf, node->m_sskill->m_level);

		node = node->m_next;
	}
	
	return buf;
}

