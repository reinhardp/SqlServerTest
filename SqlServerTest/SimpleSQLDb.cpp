#include "SimpleSQLDb.h"
#include <userenv.h>
#include <strsafe.h>




int SimpleSQLDb::init(void) {
    TCHAR  lpuser[255];
    DWORD  pcbBuffer = 255;
    DWORD  buffer = MAX_PATH;
    DWORD dw;
    WCHAR wszConfigurationFile[MAX_PATH];
    WCHAR host[40], dbName[40], port[40], user[40], pwd[40], table[40];
    GetUserName(lpuser, &pcbBuffer);
    GetProfilesDirectory(wszConfigurationFile, &buffer);
    StringCchCat(wszConfigurationFile, NUMELEM(wszConfigurationFile), L"\\");
    StringCchCat(wszConfigurationFile, MAX_PATH, lpuser);
    StringCchCat(wszConfigurationFile, MAX_PATH, L"\\");
    StringCchCat(wszConfigurationFile, MAX_PATH, CONFIG_FILE);

    //dw = GetPrivateProfileString(DB_SECTION, L"Driver", NULL, m_Driver, 40, m_wszConfigurationFile);
    dw = GetPrivateProfileString(DB_SECTION, L"DB_HOST", NULL, host, 40, wszConfigurationFile);
    dw = GetPrivateProfileString(DB_SECTION, L"DB_NAME", NULL, dbName, 40, wszConfigurationFile);
    dw = GetPrivateProfileString(DB_SECTION, L"DB_PORT", L"1433", port, 40, wszConfigurationFile);
    dw = GetPrivateProfileString(DB_SECTION, L"DB_USER", NULL, user, 40, wszConfigurationFile);
    dw = GetPrivateProfileString(DB_SECTION, L"DB_PWD", NULL, pwd, 40, wszConfigurationFile);
    dw = GetPrivateProfileString(DB_SECTION, L"DB_TABLE", NULL, table, 40, wszConfigurationFile);
    m_host = gcnew String(host);
    m_dbName = gcnew String(dbName);
    m_port = gcnew String(port);
    m_user = gcnew String(user);
    m_pwd = gcnew String(pwd);
    m_table = gcnew String(table);
    return 0;

}

void SimpleSQLDb::connect(void) {
    String^ connectString = String::Format("Data Source={0};Database={1};UID={2};Passord={3}", gcnew String(m_host), gcnew String(m_dbName), gcnew String(m_user), gcnew String(m_pwd));
    m_sqlconn = gcnew SqlConnection(connectString);
    m_sqlconn->Open();
   
}

unsigned int SimpleSQLDb::getLastID(void) {

    int rc = 0;
    String^ SQLString = String::Format("SELECT id from %s.dbo.%s ORDER BY id DESC	LIMIT 1", m_dbName, m_table);
    SqlCommand^ sqlCommand = gcnew SqlCommand(SQLString, m_sqlconn);
    SqlDataReader^ dataReader = sqlCommand->ExecuteReader();
    unsigned int counter = 0;
    while (dataReader->Read()) {

        counter++;
    }
    if (rc != 0)
        return 0;
    return counter;
}

/*
*get all rows stored in DB
*/
bool SimpleSQLDb::getAll(void* data) {

    String^ SQLString = String::Format("SELECT id, firstname, lastname, birthday, dtext, created_by, created_at FROM %s.dbo.%s ORDER BY `id`", m_dbName, m_table);
    //StringCchPrintf(m_SQLString, 1024, format, m_table);m_dbName
    SqlCommand^ sqlCommand = gcnew SqlCommand(SQLString, m_sqlconn);
    SqlDataReader^ dataReader = sqlCommand->ExecuteReader();
    unsigned int rows = 0;
    int numCols = (dataReader->FieldCount - 1);

    // get rows:
    while (dataReader->Read()) {
        rows++;
    }





    return true;
}