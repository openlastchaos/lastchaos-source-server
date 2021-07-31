//////////////////////////////////
// OLE DB를 사용하기 위한 헤더파일

#ifndef _MY_OLEDB_H_
#define _MY_OLEDB_H_

#include "Config.h"
//////////////////////////////////////////////////
// OLE DB를 사용하기 위해서 필요한 조건을 검사한다
#if !(_WIN32_WINNT >= 0x0400 ) && !defined(_WIN32_DCOM)
#error "Need: _WIN32_WINNT >= 0x0400 || _WIN32_DCOM"
#endif

extern int test_count_open_query;
extern int test_count_update_query;
extern int test_count_open_session;
extern int test_count_close_session;

#ifdef LC_TLD
#define LOCALE 874
#elif defined( LC_TWN )
#define LOCALE 950
#elif defined( LC_JPN )
#define LOCALE 932
#else
#define LOCALE 949
#endif

#ifdef BWTEST
#undef LOCALE
#define LOCALE 949
#endif



//////////////////////////////
// OLE DB 사용시 필요한 헤더들
#include <objbase.h>
#include <atldbcli.h>

#include "Utils.h"
//////////////
// 클래스 정의


////////////////
// class CDBConn
// SQL Server에 접속하기 위한 클래스
// 필요에 따라서 사용하는 초기화 문자열을 변경한다
// 클래스에서는 세션을 하나만 사용한다.
class CDBConn
{
	CDataSource			m_ds;			// 데이터소스
	HRESULT				m_hr;			// 최종 호출된 COM 함수의 반환 값

public:
	CDBConn()		{}
	~CDBConn()		{ m_ds.Close(); }


	////////////////////
	// Function name	: Open
	// Description	    : 소스에 연결하여 세션을 연다
	// Argument         : const char* datasource
	//                  : 데이터소스 이름
	// Argument         : const char* user
	//                  : 사용자 계정
	// Argument         : const char* password
	//                  : 사용자 암호
	// Argument         : const char* database
	//                  : 사용할 DB이름
	bool Open(const char* datasource, const char* user, const char* password, const char* database)
	{
		USES_CONVERSION;

		char buf[1024];
		char initstr[1024] = "Provider=SQLOLEDB.1;Persist Security Info=True";

		if (datasource)		{ sprintf(buf, ";Data Source=%s", datasource);		strcat(initstr, buf); }
		if (user)			{ sprintf(buf, ";User ID=%s", user);				strcat(initstr, buf); }
		if (password)		{ sprintf(buf, ";Password=%s", password);			strcat(initstr, buf); }
		if (database)		{ sprintf(buf, ";Initial Catalog=%s", database);	strcat(initstr, buf); }

		//Locale Identifier //LCID
		//{ sprintf(buf, ";Locale Identifier=0x41E");	strcat(initstr, buf); }

		m_hr = m_ds.OpenFromInitializationString(A2OLE(initstr));

		if (m_hr == S_OK)
		{
#ifdef BWTEST
			CComVariant var;
			m_ds.GetProperty(DBPROPSET_DBINIT, DBPROP_INIT_LCID, &var);
#endif
			return true;
		}

		return false;
	}


	////////////////////
	// Function name	: Close
	// Description	    : 소스와 세션 연결을 해제
	void Close()
	{
		m_ds.Close();
	}


	//////////////////
	// 연산자 오버로딩
	operator const CDataSource& ()	{ return m_ds; }							// 데이터소스 반환

	/////////////////
	// 기타 유틸 함수
	HRESULT GetLastResult()			{ return m_hr; }							// 마지막 수행 결과 반환
};


///////////////
// class CODBCmd
// 다이나믹 Accessor를 가진 CCommand 유틸 클래스
class CODBCmd : public CCommand<CDynamicAccessor, CRowset, CNoMultipleResults>
{
protected:
	CSession			m_session;
	HRESULT				m_hr;

public:
	CODBCmd()							{ m_hr = S_OK; }
	~CODBCmd()
	{
		test_count_close_session--;
		Close();
		m_session.Close();
	}

	bool Init(const CDataSource& ds)
	{
		test_count_open_session++;
		return (m_session.Open(ds) == S_OK);
	}


	////////////////////
	// Function name	: Open
	// Description	    : 쿼리를 실행한다(SELECT)
	bool Open(const char* sql)
	{
		test_count_open_query++;
		Close();
		char temp[8192];
		BackSlash(temp, 8192, sql);
		return ((m_hr = CCommand<CDynamicAccessor, CRowset, CNoMultipleResults>::Open(m_session, temp)) == S_OK);
	}


	////////////////////
	// Function name	: Update
	// Description	    : 쿼리를 실행한다
	long Update(const char* sql)
	{
		test_count_update_query++;
		Close();

		long baffected;
		char temp[8192];
		BackSlash(temp, 8192, sql);//DBGUID_SQL  //DBGUID_DEFAULT
		if( (m_hr = CCommand<CDynamicAccessor, CRowset, CNoMultipleResults>::Open(m_session, temp, NULL, &baffected, DBGUID_DEFAULT, false)) == S_OK )
		{
			return baffected;
		}
		return 0;
	}


	///////////////////
	// Data 얻는 함수들
	bool Get(char* fname, int& val)
	{
		if (IsNULL(fname))				return false;
		void* p = GetValue(fname);
		if (!p)							return false;
		val = *((int*)p);
		return true;
	}

	bool Get(int fno, int& val)
	{
		if (IsNULL(fno))				return false;
		void* p = GetValue(fno);
		if (!p)							return false;
		val = *((int*)p);
		return true;
	}

	bool Get(char* fname, DBTIMESTAMP& val)
	{
		if (IsNULL(fname))				return false;
		void* p = GetValue(fname);
		if (!p)							return false;
		val = *((DBTIMESTAMP*)p);
		return true;
	}

	bool Get(int fno, DBTIMESTAMP& val)
	{
		if (IsNULL(fno))				return false;
		void* p = GetValue(fno);
		if (!p)							return false;
		val = *((DBTIMESTAMP*)p);
		return true;
	}

	bool Get(char* fname, char* val)
	{
		if (IsNULL(fname))				return false;
		void* p = GetValue(fname);
		if (!p)							return false;
		ULONG len;
		if (!GetLength(fname, &len))	return false;

		// 유니코드 일때를 위함 
		WideCharToMultiByte(LOCALE, WC_COMPOSITECHECK, (const unsigned short*) p, -1, val, len, NULL, NULL);
		//int nLen = WideCharToMultiByte(CP_ACP, 0, mm, -1, val, 0, NULL, NULL);
		//MultiByteToWideChar(m_locale, MB_COMPOSITE, cmd.GetRec(this->m_itemName), -1, w_buf, 4096);
		//WideCharToMultiByte(m_locale, WC_COMPOSITECHECK, sql, -1, g_buf, sql.GetLength()+1, NULL, NULL);

		return true;
	}

	bool Get(int fno, char* val)
	{
		if (IsNULL(fno))				return false;
		void* p = GetValue(fno);
		if (!p)							return false;
		ULONG len;
		if (!GetLength(fno, &len))		return false;
		strncpy(val, (char*)p, len);
		val[len] = '\0';
		return true;
	}

	///////////
	// 트랜잭션
	bool BeginTrans()				{ return ((m_hr = m_session.StartTransaction()) == S_OK); }
	bool CommitTrans()				{ return ((m_hr = m_session.Commit()) == S_OK); }
	bool RollbackTrans()			{ return ((m_hr = m_session.Abort()) == S_OK); }


	/////////////////
	// 기타 유틸 함수
	HRESULT GetLastResult()			{ return m_hr; }							// 마지막 수행 결과 반환

	bool IsNULL(char* fname)													// 널 검사
	{
		DBSTATUS dbstatus;
		if (!GetStatus(fname, &dbstatus))
			return true;
		if (dbstatus != DBSTATUS_S_OK)
			return true;
		return false;
	}

	bool IsNULL(ULONG fno)
	{
		DBSTATUS dbstatus;
		if (!GetStatus(fno, &dbstatus))
			return true;
		if (dbstatus != DBSTATUS_S_OK)
			return true;
		return false;
	}
};


////////////
// 함수 정의


/////////////////////
// COM library 초기화
inline bool InitCOM()
{
	if (CoInitializeEx(NULL, COINIT_MULTITHREADED) != S_OK)
		return false;
//	if (CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) != S_OK)
//		return false;
//	if (OleInitialize(NULL) != S_OK)
//		return false;

	return true;
}


//////////////
// 매크로 정의
#define DBCHECK(stm)		if (!stm) throw 0

#endif