#include "MSSqlQuery.h"
#include <tchar.h>
#include "sql.h"
#include <sqlext.h>  
#include <strsafe.h>
#include <userenv.h>

MSSqlQuery::MSSqlQuery() {
	m_hdbc = m_henv = m_hstmt = NULL;
}



bool MSSqlQuery::init(void) {
	TCHAR  lpuser[255];
	DWORD  pcbBuffer = 255;
	DWORD  buffer = MAX_PATH;
	DWORD dw;

	GetUserName(lpuser, &pcbBuffer);
	GetProfilesDirectory(m_wszConfigurationFile, &buffer);
	StringCchCat(m_wszConfigurationFile, NUMELEM(m_wszConfigurationFile), L"\\");
	StringCchCat(m_wszConfigurationFile, MAX_PATH, lpuser);
	StringCchCat(m_wszConfigurationFile, MAX_PATH, L"\\");
	StringCchCat(m_wszConfigurationFile, MAX_PATH, CONFIG_FILE);


	dw = GetPrivateProfileString(DB_SECTION, L"Driver", NULL, m_Driver, 40, m_wszConfigurationFile);
	dw = GetPrivateProfileString(DB_SECTION, L"DB_HOST", NULL, m_host, 40, m_wszConfigurationFile);
	dw = GetPrivateProfileString(DB_SECTION, L"DB_NAME", NULL, m_dbName, 40, m_wszConfigurationFile);
	dw = GetPrivateProfileString(DB_SECTION, L"DB_PORT", L"1433", m_port, 40, m_wszConfigurationFile);
	dw = GetPrivateProfileString(DB_SECTION, L"DB_USER", NULL, m_user, 40, m_wszConfigurationFile);
	dw = GetPrivateProfileString(DB_SECTION, L"DB_PWD", NULL, m_pwd, 40, m_wszConfigurationFile);
	dw = GetPrivateProfileString(DB_SECTION, L"DB_TABLE", NULL, m_table, 40, m_wszConfigurationFile);

	return true;
}

MSSqlQuery::~MSSqlQuery() {
}


/*
 * get all rows stored in DB
*/
bool MSSqlQuery::getAll(WCHAR **data) {
	
	WCHAR format[1024] = L"SELECT firstname, lastname, birthday, dtext, created_by, crated_at FROM %s.dbo.%s";
	
	StringCchPrintf(m_SQLString, 1024, format, m_dbName, m_table);


	return true;
}



/*
*/
int MSSqlQuery::connect(HINSTANCE hInst, HWND hWnd) {
	SQLRETURN rc;
	SQLRETURN s_ret;
	
	
	
    wchar_t dsn[1024] = { 0 };
	//wchar_t format[255] = L"Driver={ODBC Driver 17 for SQL Server};Server=%s;Database=%s;";
	//wchar_t format[255] = L"DRIVER=%s;SERVER=%s;DATABASE=%s;Trusted_Connection=yes;";
	//wchar_t format[255] = L"Data Source=%s;Database=%s;USER=%s;Password=%s;Command Logging=false";
	wchar_t message[255] = { 0 };
	

	//SQLTCHAR msg[256];
	SQLTCHAR sqlstate[6] = {0}, Msg[2048] = {0};
	SQLINTEGER NativeError;
	SQLSMALLINT MsgLen;


	//rc = SQLAllocEnv(&m_henv);
	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
	{
		MessageBox(NULL, L"SQLAllocEnv schlug fehl!", L"SQLAllocEnv", MB_OK| MB_ICONERROR);
		return -1;
	}

	rc = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
	{
		rc = SQLGetDiagRec(SQL_HANDLE_DBC, m_hdbc, 1, sqlstate, &NativeError, Msg, sizeof(Msg), &MsgLen);
		MessageBox(NULL, L"SQLSetEnvAttr schlug fehl!", L"SQLSetEnvAttr", MB_OK | MB_ICONERROR);
		return -2;
	}

	//rc = SQLAllocConnect(m_henv, &m_hdbc);
	rc = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
	{
		//rc = SQLGetDiagRecW(SQL_HANDLE_DBC, m_hdbc, 1, sqlstate, &NativeError, Msg, sizeof(Msg), &MsgLen);
		MessageBox(NULL, L"SQLAllocConnect schlug fehl!", L"SQLAllocConnect", MB_OK | MB_ICONERROR);
		return -3;
	}
	
	
	//StringCchPrintf(dsn, 1024, format, m_Driver, m_host, m_dbName);
	//wchar_t format[255] = L"Data Source=%s;Database=%s;User ID=%s;Password=%s;Command Logging=false";

	//wchar_t format[255] = L"DRIVER=%s;TCPIP=1;Server=%s;Port=3306;Database=%s;UID=%s;PWD=%s;PORT=%s"; // MariaDB
	wchar_t format[255] = L"DRIVER=%s;SERVER=%s;DATABASE=%s;";	// SQL Swrver 2019
	StringCchPrintf(dsn, 1024, format, m_Driver, m_host, m_dbName);


	SQLSetConnectAttr(m_hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
	
	s_ret = SQLConnectW(m_hdbc, dsn, (SQLSMALLINT)strlen((char*)dsn), m_user, (SQLSMALLINT)strlen((char*)m_user), m_pwd, (SQLSMALLINT)strlen((char*)m_pwd));

	if (!SQL_SUCCEEDED(s_ret)) {
		rc = SQLGetDiagRec(SQL_HANDLE_DBC, m_hdbc, 1, sqlstate, &NativeError, Msg, 2048, &MsgLen);
		wchar_t formatMsg[255] = L"Kann Verbindung zu Datenbank \"%s\" nicht herrstellen!\n\nFehler: %s: %s";
		StringCchPrintf(message, 255, formatMsg, m_dbName, sqlstate, Msg);
		
		SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
		//MessageBox(NULL, L"Kann keine Verbindung zur Datenbank herstellen!", L"SQL Connect", MB_OK | MB_ICONERROR);
		MessageBox(NULL, message, L"SQL Connect", MB_OK | MB_ICONERROR);
		return -4;
	}

	return 1;
}


/*
 * get the last ID from the database
*/

unsigned int MSSqlQuery::getLastID(void) {
	WCHAR format[255] = L"SELECT id from %s.dbo.%s ORDER BY id DESC	LIMIT 1";
	StringCchPrintf(m_SQLString, 255, format, m_dbName, m_table);

	return 1;
}


/*
*/
bool MSSqlQuery::getFirstRow(OUT WCHAR** row) {
	m_currentRow = 1;
	WCHAR format[255] = L"SELECT TOP (1) firstname, lastname, birthday, dtext, created_by, crated_at FROM %s.dbo.%s";
	

	StringCchPrintf(m_SQLString, 255, format, m_dbName, m_table);

	return true;
}

/*
*/
bool MSSqlQuery::getNextRow(OUT WCHAR** row) {
	WCHAR format[255] = L"SELECT TOP (1) firstname, lastname, birthday, dtext, created_by, crated_at FROM %s.dbo.%s";	// ??

	m_currentRow += 1;
	return true;
}

void MSSqlQuery::freeConnection(void) {
	SQLFreeConnect(m_henv);
	SQLFreeEnv(m_henv);
	SQLFreeConnect(m_hdbc);
}

void MSSqlQuery::disconnect(void) {
	if (m_hstmt) {
		SQLFreeStmt(m_hstmt, SQL_DROP);
	}
	SQLDisconnect(m_hdbc);
	SQLFreeConnect(m_hdbc);
	SQLFreeEnv(m_henv);
}
