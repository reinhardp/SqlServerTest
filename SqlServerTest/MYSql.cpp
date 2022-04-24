#include "MYSql.h"
#include <strsafe.h>
#include <stdlib.h>
#include <windows.h>

/*
*
*/
#if USE_UNICODE == true
void MYSql::init(WCHAR* host, WCHAR* port, WCHAR* database,WCHAR* user, WCHAR* password) {
	m_host = host;
	m_port = port;
	m_database = database;
	m_user = user;
	m_password = password;
}
#endif

#if USE_UNICODE == false
void MYSql::initA(char* host, char* port, char* database, char* user, char* password) {
	m_host = host;
	m_port = port;
	m_database = database;
	m_user = user;
	m_password = password;
}
#endif
/*
* 
*/
#if USE_UNICODE == true
bool MYSql::connect() {

	char hostName[255] = { 0 };
	char host[100];
	char port[5];
	char user[255];
	char pwd[255];
	char database[100];
	size_t retval;
	char temperrstring[512] = { 0 };
	int on_off = 1;
	

	try {

		wcstombs_s(&retval, host, 100, m_host, 100);
		wcstombs_s(&retval, port, 5, m_port, 5);
		wcstombs_s(&retval, database, 100, m_database, 100);
		wcstombs_s(&retval, user, 255, m_user, 255);
		wcstombs_s(&retval, pwd, 255, m_password, 255);

		StringCbPrintfA(hostName, 255, "tcp://%s:%s?useUnicode=true", host, port);

		m_driver = get_driver_instance();
		m_con = m_driver->connect(hostName, user, pwd);
		m_con->setSchema(database);
		//m_con->setAutoCommit(1);

		m_con->setClientOption("useUnicode", "true");
		//m_con->setClientOption("libmysql_debug", "d:t:O,client.trace");
		//m_con->setClientOption("characterEncoding", "utf8mb4");
		m_con->setClientOption("clientTrace", &on_off);
		
		//m_con->setClientOption("character_set_server", "utf8mb4");

		//m_con->setClientOption("OPT_CHARSET_NAME", "utf8mb4");
		//m_con->setClientOption("OPT_SET_CHARSET_NAME", "utf8mb4");

		setNames();
		m_hasError = false;
		m_connected = true;
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "Connect", MB_OK | MB_ICONERROR);
		m_hasError = true;
		m_connected = false;

		return false;
	}

	return true;
}
#endif

#if USE_UNICODE == false

bool MYSql::connectA() {

	char hostName[255] = { 0 };
	char host[100];
	char port[5];
	char user[255];
	char pwd[255];
	char database[100];
	size_t retval;
	char temperrstring[512] = { 0 };


	try {


		StringCbPrintfA(hostName, 255, "tcp://%s:%s?useUnicode=true", m_host, m_port);

		m_driver = get_driver_instance();

		m_con = m_driver->connect(hostName, m_user, m_password);
		//m_con->setClientOption("OPT_CHARSET_NAME", "utf8mb4");
		//m_con->setClientOption("OPT_SET_CHARSET_NAME", "utf8mb4");
		m_con->setClientOption("useUnicode", "true");


		m_con->setSchema(m_database);

		//m_con->setClientOption("character_set_server", "");

		setNames();
		m_connected = true;
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "Connect", MB_OK | MB_ICONERROR);
		m_hasError = true;
		m_connected = false;

		return false;
	}

	return true;
}
#endif

/*
*
*/

bool MYSql::select(char* query) {
	char temperrstring[512] = { 0 };
	try {
		m_stmt = m_con->createStatement();
		m_res = m_stmt->executeQuery(query);
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "Select using query", MB_OK | MB_ICONERROR);
		m_hasError = true;
		return false;
	}

	return true;
}


bool MYSql::select(int maxRows = 20, int currentPage = 0) {
	char statement[] = "SELECT id, firstname, lastname, dtext, birthday, created_by, created_at, updated_by, updated_at from mstest LIMIT ? OFFSET ?;";
	char temperrstring[512] = { 0 };
	bool ret = false;

	try {
		
		int offset = currentPage * maxRows;
		m_prep_stmt = m_con->prepareStatement(statement);
		m_prep_stmt->setInt(1, maxRows);
		m_prep_stmt->setInt(2, offset);
		ret = m_prep_stmt->executeQuery();
		ret = true;
		
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "Select using query", MB_OK | MB_ICONERROR);
		m_hasError = true;
		ret = false;
	}

	return ret;
}
/*
* select method 
* 
* @param none
* @return bool
*/
bool MYSql::select() {
	char query[] = "SELECT id, firstname, lastname, dtext, birthday, created_by, created_at, updated_by, updated_at from mstest;";	
	char temperrstring[512] = { 0 };

	try {
		m_stmt = m_con->createStatement();
		m_res = m_stmt->executeQuery(query);
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		m_hasError = true;
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "Selec from table 'mttest'", MB_OK | MB_ICONERROR);

		return false;
	}

	return true;

}

/*
*
*/

#if USE_UNICODE == true
bool MYSql::insert(WCHAR* firstname, WCHAR* lastname, WCHAR* birthday, WCHAR* dtext) {
	char statement[] = "INSERT INTO mstest(firstname, lastname, birthday, dtext, created_by) VALUES(?,?,?,?,?);";
	
	char cdtext[DTEXT_LENGTH] ;
	char buffer[30] = {0};
	size_t retval = 0;
	bool ret = false;
	char temperrstring[512] = { 0 };

	try {
		sql::PreparedStatement* prep_stmt = m_con->prepareStatement(statement);

		wcstombs_s(&retval, buffer, 30, firstname, 30);
		prep_stmt->setString(1, buffer);

		wcstombs_s(&retval, buffer, 30, lastname, 30);
		prep_stmt->setString(2, buffer);

		wcstombs_s(&retval, buffer, 30, birthday, 30);
		prep_stmt->setString(3, buffer);

		wcstombs_s(&retval, cdtext, DTEXT_LENGTH, dtext, DTEXT_LENGTH);
		prep_stmt->setString(4, cdtext);

		prep_stmt->setString(5, CREATEDBY);

		ret = prep_stmt->executeUpdate();
		delete prep_stmt;

		ret = true;

	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		m_hasError = true;
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "Insert", MB_OK | MB_ICONERROR);
		ret = false;
	}

	return ret;
}
#endif
#if USE_UNICODE == false
bool MYSql::insertA(char* firstname, char* lastname, char* birthday, char* dtext) {

	char statement[] = "INSERT INTO mstest(firstname, lastname, birthday, dtext, created_by) VALUES(?,?,?,?,?);";

	bool ret = false;
	char temperrstring[512] = { 0 };

	try {
		sql::PreparedStatement* prep_stmt = m_con->prepareStatement(statement);

		prep_stmt->setString(1, firstname);

		prep_stmt->setString(2, lastname);

		prep_stmt->setString(3, birthday);

		prep_stmt->setString(4, dtext);

		prep_stmt->setString(5, CREATEDBY);

		ret = prep_stmt->executeUpdate();
		delete prep_stmt;

		ret = true;

	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		m_hasError = true;
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "Insert", MB_OK | MB_ICONERROR);
		ret = false;
	}

	return ret;
}
#endif

int MYSql::getLastID() {
	char query[] = "SELECT MAX(id) as lastID from mstest";
	int id = 0;
	char temperrstring[512] = { 0 };

	try {
		m_stmt = m_con->createStatement();
		m_res = m_stmt->executeQuery(query);
		m_res->next();
		id = m_res->getInt("lastID");
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "getLastID", MB_OK | MB_ICONERROR);
		m_hasError = true;

		return id;
	}

	return id;
}

bool MYSql::deleteID(int id) {
	char sql[] = "DELETE FROM mstest WHERE id=?";
	char temperrstring[512] = { 0 };

	try {
		m_prep_stmt = m_con->prepareStatement(sql);
		m_prep_stmt->setInt(1, id);
		m_prep_stmt->execute();

		delete m_prep_stmt;
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		m_hasError = true;
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "DeletID", MB_OK | MB_ICONERROR);

		return false;
	}

	return true;
}

int MYSql::getNumberOfRows() {
	char sql[] = "SELECT count(id) as amount from mstest";
	int rows = 0;
	char temperrstring[512] = { 0 };

	try {
		sql::Statement* stmt = m_con->createStatement();
		sql::ResultSet* res = stmt->executeQuery(sql);
		res->next();
		
		rows = res->getInt("amount");
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		m_hasError = true;
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "getNumberOfRows", MB_OK | MB_ICONERROR);
		return 0;
	}

	return rows;
}
#if USE_UNICODE == true
bool MYSql::update(int id, WCHAR* firstname, WCHAR* lastname, WCHAR* birthday, WCHAR* dtext) {
	char sql[] = "UPDATE mstest SET firstname = ?, lastname = ?, birthday = ?, dtext = ?, updated_by = ?  WHERE id = ? ";
	char cdtext[DTEXT_LENGTH] = {0};
	char buffer[30] = { 0 };
	char temperrstring[512] = { 0 };

	int affected_rows;

	try {
		sql::PreparedStatement* prep_stmt = m_con->prepareStatement(sql);
		prep_stmt->setInt(6, id);

		wcstombs_s(&m_retval, buffer, 30, firstname, 30);
		prep_stmt->setString(1, buffer);

		wcstombs_s(&m_retval, buffer, 30, lastname, 30);
		prep_stmt->setString(2, buffer);
		
		wcstombs_s(&m_retval, buffer, 30, birthday, 30);
		prep_stmt->setString(3, buffer);

		wcstombs_s(&m_retval, cdtext, DTEXT_LENGTH, dtext, DTEXT_LENGTH);
		prep_stmt->setString(4, cdtext);

		//wcstombs_s(&m_retval, buffer, 30, UPDATEDBY, 30);
		//prep_stmt->setString(5, buffer);
		prep_stmt->setString(5, UPDATEDBY);

		affected_rows  = prep_stmt->executeUpdate();
		//affected_rows = prep_stmt->execute();

		delete prep_stmt;
		m_updated = true;
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		m_hasError = true;
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "Update", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
#endif
/*
* ASCCI method to update the 
*/
#if USE_UNICODE == false
bool MYSql::updateA(int id, char* firstname, char* lastname, char* birthday, char* dtext) {
	char sql[] = "UPDATE mstest SET firstname = ?, lastname = ?, birthday = ?, dtext = ?, updated_by = ?  WHERE id = ? ";
	char temperrstring[512] = { 0 };
	int affected_rows;

	try {
		sql::PreparedStatement* prep_stmt = m_con->prepareStatement(sql);
		prep_stmt->setInt(6, id);

		prep_stmt->setString(1, firstname);

		prep_stmt->setString(2, lastname);

		prep_stmt->setString(3, birthday);

		prep_stmt->setString(4, dtext);

		prep_stmt->setString(5, UPDATEDBY);

		//affected_rows  = prep_stmt->executeUpdate();
		affected_rows = prep_stmt->execute();

		delete prep_stmt;
		m_updated = true;
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		m_hasError = true;
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "Update", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
#endif

bool MYSql::setNames() {
	//char sql[] = "set names utf8mb4 collate utf8mb4_unicode_ci";
	char sql[] = "set names 'utf8mb4'";

	char temperrstring[512] = { 0 };

	try {
		sql::PreparedStatement* prep_stmt = m_con->prepareStatement(sql);
		prep_stmt->executeUpdate();
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		m_hasError = true;
		StringCbPrintfA(temperrstring, 512, "%d: %s", errNo, errstring);
		MessageBoxA(NULL, temperrstring, "SetNames", MB_OK | MB_ICONERROR);
		//MessageBox();
		return false;
	}
	return true;
}