#include "stdhdrs.h"
#include "Log.h"


CLogFile g_log;
CLogFile g_logDB;
CLogBuffer g_gamelogbuffer;
CLogBuffer g_dblogbuffer;


////////////////////////////////
// operators in CLogBuffer class
CLogBuffer& CLogBuffer::operator << (const char* m)
{
	appendLog(m);
	return *this;
}

CLogBuffer& CLogBuffer::operator << (int m)
{
	__ltoa(m, m_tbuf);
	appendLog(m_tbuf);
	return *this;
}

CLogBuffer& CLogBuffer::operator << (LONGLONG m)
{
	__ltoa(m, m_tbuf);
	appendLog(m_tbuf);
	return *this;
}

CLogBuffer& CLogBuffer::operator << (float m)
{
	__ftoa(m, m_tbuf, 4);
	appendLog(m_tbuf);
	return *this;
}

CLogBuffer& CLogBuffer::operator << (const CLogHeader& lh)
{
	(*this) << init;

	switch (lh.m_type)
	{
	case LHT_MSG:
		(*this) << lh.m_msg << " > ";
		break;

	case LHT_MSG_ID:
		(*this) << lh.m_msg << " > " << lh.m_id << " : ";
		break;

	case LHT_MSG_ID_NAME:
		(*this) << lh.m_msg << " > " << lh.m_name << " : " <<  lh.m_id << " : ";
		break;
	case LHT_MSG_ID_NICK:
		(*this) << lh.m_msg << " > " << lh.m_name << " : " << lh.m_nick << " : " <<  lh.m_id << " : ";
		break;
#ifdef LOG_INDEX
	case LHT_MSG_ID_NICK_INDEX:
		(*this) << lh.m_msg << " > " << lh.m_name << " : " << lh.m_nick << " : " << lh.m_charindex
			<< " : " <<  lh.m_id << " : " << lh.m_userindex << " : ";
		break;
#endif // LOG_INDEX
	}

	return *this;
}
#ifdef __GAME_SERVER__
CLogBuffer& CLogBuffer::operator << (ITEMLOGMANI ilm)
{
	if (ilm.m_pItem == NULL)
	{
		(*this) << "(null)";
		return (*this);
	}

#ifdef MONSTER_RAID_SYSTEM
	if (ilm.m_pItem->IsRareItem() == true
		&& ilm.m_pItem->m_pRareOptionProto != NULL 
		&& ilm.m_pItem->m_pRareOptionProto->GetIndex() > 0)
	{
		(*this) << "[ rare index: " << ilm.m_pItem->m_pRareOptionProto->GetIndex() << delim
				<< "rare bit: " << ilm.m_pItem->m_nRareOptionBit << "] " ;
		
	}
#endif // MONSTER_RAID_SYSTEM
	(*this) << ilm.m_pItem->m_idNum << delim
			<< ilm.m_pItem->m_itemProto->m_name << delim
			<< ilm.m_pItem->m_serial << delim
			<< ilm.m_pItem->m_wearPos << delim
			<< ilm.m_pItem->m_plus << delim
			<< ilm.m_pItem->m_flag << delim
			<< ilm.m_pItem->m_used << delim
#ifdef COMPOSITE_TIME
			<< ilm.m_pItem->m_used_2 << delim
#endif	// COMPOSITE_TIME			
			<< ilm.m_pItem->Count();
	if (!ilm.m_bSimple)
	{
		(*this) << delim << "OPTION"
				<< delim << ilm.m_pItem->m_nOption;
		int i;
		for (i = 0; i < ilm.m_pItem->m_nOption; i++)
		{
			(*this) << delim << ilm.m_pItem->m_option[i].m_type
					<< delim << ilm.m_pItem->m_option[i].m_value;
		}
	}
	return (*this);
}
#endif // __GAME_SERVER__

void CLogBuffer::Write()
{
	m_log->Write(Get());
}

void CLogBuffer::Flush()
{
	m_log->Flush();
}


////////////////////////////
// methods in CLogFile class
CLogFile::CLogFile()
{
	m_fp = NULL;
	*m_servercode = '\0';
}

CLogFile::~CLogFile()
{
	Close();
}

void CLogFile::Close()
{
	if (m_fp && m_fp != stdout)
		fclose(m_fp);
	m_fp = NULL;
}

void CLogFile::InitLogFile(bool isDBLog, CLogBuffer* destBuffer, const char* servercode)
{
	m_logBuffer = destBuffer;
	destBuffer->SetLogFile(this);
	m_bIsDBLog = isDBLog;

	if (*m_servercode == '\0')
		strcpy(m_servercode, servercode);

	struct tm ti = NOW();
	char logstr[128];

	Close();

#ifdef __BILLING_SERVER__
	strcpy(logstr, "Log");
#else
	strcpy(logstr, "../LogFiles/Log");
#endif
	IntCat0(logstr, ti.tm_year % 10, 2, false);
	strcat(logstr, m_servercode);
	IntCat0(logstr, ti.tm_mon + 1, 2, false);
	IntCat0(logstr, ti.tm_mday, 2, false);
	if (m_bIsDBLog)
		strcat(logstr, "_DB");
	m_fp = fopen(logstr, "a+");
	if (m_fp == NULL && !m_bIsDBLog)
		m_fp = stdout;
}

void CLogFile::InitLogFile(int serverno, int subno, int zone, bool isDBLog, CLogBuffer* destBuffer)
{
	char tmp[100] = "";
	IntCat0(tmp, serverno, 2, false);
	IntCat0(tmp, subno, 2, false);
	IntCat0(tmp, zone, 2, false);
	InitLogFile(isDBLog, destBuffer, tmp);
}

void CLogFile::Write(CLogBuffer& message)
{
	Write(message.Get());
	message.Init();
}

void CLogFile::Write(const char* message)
{
	struct tm ti = NOW();
	static int day = -1;
	int wrote = 0;

	char buf[10240] = "";

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
	strcat(buf, " ::");

	strcat(buf, " ");
	strcat(buf, message);

	if (day == -1) day = ti.tm_mday;
	if (day != ti.tm_mday)
		InitLogFile();

	if (*buf)
	{
		strcat(buf, "\n");
		if (m_fp) fputs(buf, m_fp);
#ifdef __BILLING_SERVER__
		if( m_fp && m_fp != stdout )
			fputs(buf, stdout);
#endif
		wrote += strlen(buf) + 1;
	}

//	static int sec = 0;
//
//	if (abs(sec - ti.tm_sec) > 10 || wrote > 3072)
//	{
//		sec = ti.tm_sec;
//		wrote = 0;
		if (m_fp) fflush (m_fp);
//	}

}
