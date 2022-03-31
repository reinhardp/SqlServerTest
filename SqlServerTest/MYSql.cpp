#include "MYSql.h"
#include <strsafe.h>
#include <stdlib.h>


/*
*
*/
void MYSql::init(WCHAR* host, WCHAR* port, WCHAR* database,WCHAR* user, WCHAR* password) {
	m_host = host;
	m_port = port;
	m_database = database;
	m_user = user;
	m_password = password;
}

/*
* 
*/
bool MYSql::connect() {

	char hostName[255] = { 0 };
	char host[100];
	char port[5];
	char user[255];
	char pwd[255];
	char database[100];
	size_t retval;
	wchar_t temperrstring[512] = { 0 };

	wcstombs_s(&retval, host, 100, m_host, 100);
	wcstombs_s(&retval, port, 5 ,m_port, 5);
	wcstombs_s(&retval, database, 100, m_database, 100);
	wcstombs_s(&retval, user, 255, m_user, 255);
	wcstombs_s(&retval, pwd, 255, m_password, 255);

	try {


		StringCbPrintfA(hostName, 255, "tcp://%s:%s?useUnicode=true",host, port);

		m_driver = get_driver_instance();

		m_con = m_driver->connect(hostName, user, pwd);
		//m_con->setClientOption("OPT_CHARSET_NAME", "utf8mb4");
		//m_con->setClientOption("OPT_SET_CHARSET_NAME", "utf8mb4");
		m_con->setClientOption("useUnicode", "true");

		
		m_con->setSchema(database);
		
		//m_con->setClientOption("character_set_server", );

		setNames();
		m_connected = true;
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		mbstowcs_s(&m_retval, temperrstring, 512, errstring, strlen(errstring));
		StringCbPrintfW(m_errstring, 512, L"%d: %s", errNo, temperrstring);
		m_hasError = true;
		m_connected = false;

		return false;
	}

	return true;
}

/*
*
*/
bool MYSql::select(WCHAR* query) {
	wchar_t temperrstring[512] = { 0 };
	try {
		m_stmt = m_con->createStatement();
		m_res = m_stmt->executeQuery((char*)query);
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		mbstowcs_s(&m_retval, temperrstring, 512, errstring, strlen(errstring));
		StringCbPrintfW(m_errstring, 512, L"%d: %s", errNo, temperrstring);
		m_hasError = true;
		//MessageBox();
		return false;
	}

	return true;
}

bool MYSql::select() {
	char query[] = "SELECT id, firstname, lastname, dtext, birthday, created_by, created_at from mstest;";	
	wchar_t temperrstring[512] = { 0 };

	try {
		m_stmt = m_con->createStatement();
		m_res = m_stmt->executeQuery(query);
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		m_hasError = true;
		mbstowcs_s(&m_retval, temperrstring, 512, errstring, strlen(errstring));
		StringCbPrintfW(m_errstring, 512, L"%d: %s", errNo, temperrstring);

		//MessageBox();
		return false;
	}

	return true;

}

/*
*
*/

bool MYSql::insert(WCHAR* firstname, WCHAR* lastname, WCHAR* birthday, WCHAR* dtext) {
	char statement[] = "INSERT INTO mstest(firstname, lastname, birthday, dtext, created_by) VALUES(?,?,?,?,?);";
	
	char cdtext[DTEXT_LENGTH] ;
	char buffer[30] = {0};
	size_t retval = 0;
	bool ret = false;
	wchar_t temperrstring[512] = { 0 };

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

		wcstombs_s(&retval, buffer, 30, CREATEDBY, 30);
		prep_stmt->setString(5, buffer);

		//ret = prep_stmt->execute(); // 
		
		ret = prep_stmt->executeUpdate();
		delete prep_stmt;

		ret = true;

	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		m_hasError = true;
		mbstowcs_s(&m_retval, temperrstring, 512, errstring, strlen(errstring));
		StringCbPrintfW(m_errstring, 512, L"%d: %s", errNo, temperrstring);
		//MessageBox();
		ret = false;
	}

	return ret;
}

int MYSql::getLastID() {
	char query[] = "SELECT MAX(id) as lastID from mstest";
	int id = 0;
	wchar_t temperrstring[512] = { 0 };

	try {
		m_stmt = m_con->createStatement();
		m_res = m_stmt->executeQuery(query);
		m_res->next();
		id = m_res->getInt("lastID");
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		mbstowcs_s(&m_retval, temperrstring, 512, errstring, strlen(errstring));
		StringCbPrintfW(m_errstring, 512, L"%d: %s", errNo, temperrstring);
		m_hasError = true;

		return 0;
	}

	return id;
}

bool MYSql::deleteID(int id) {
	char sql[] = "DELETE FROM mstest WHERE id=?";
	wchar_t temperrstring[512] = { 0 };

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
		mbstowcs_s(&m_retval, temperrstring, 512, errstring, strlen(errstring));
		StringCbPrintfW(m_errstring, 512, L"%d: %s", errNo, temperrstring);

		return false;
	}

	return true;
}

int MYSql::getNumberOfRows() {
	char sql[] = "SELECT count(id) as amount from mstest";
	int rows = 0;
	wchar_t temperrstring[512] = { 0 };

	try {
		sql::Statement* stmt = m_con->createStatement();
		sql::ResultSet* res = stmt->executeQuery(sql);
		res->next();
		
		rows = res->getInt("amount");
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		mbstowcs_s(&m_retval, temperrstring, 512, errstring, strlen(errstring));
		StringCbPrintfW(m_errstring, 512, L"%d: %s", errNo, temperrstring);
		m_hasError = true;
		//MessageBox();
		return 0;
	}

	return rows;
}

bool MYSql::update(int id, WCHAR* firstname, WCHAR* lastname, WCHAR* birthday, WCHAR* dtext) {
	char sql[] = "UPDATE mstest SET firstname = ?, lastname = ?, birthday = ?, dtext = ?, updated_by = ?  WHERE id = ? ";
	char cdtext[DTEXT_LENGTH] = {0};
	char buffer[30] = { 0 };
	wchar_t temperrstring[512] = { 0 };

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

		wcstombs_s(&m_retval, buffer, 30, UPDATEDBY, 30);
		prep_stmt->setString(5, buffer);

		//affected_rows  = prep_stmt->executeUpdate();
		affected_rows = prep_stmt->execute();

		delete prep_stmt;
		m_updated = true;
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		m_hasError = true;
		mbstowcs_s(&m_retval, temperrstring, 512, errstring, strlen(errstring));
		StringCbPrintfW(m_errstring, 512, L"%d: %s", errNo, temperrstring);
		//MessageBox();
		return false;
	}

	return true;
}

bool MYSql::setNames() {
	char sql[] = "set names utf8mb4";
	wchar_t temperrstring[512] = { 0 };

	try {
		sql::PreparedStatement* prep_stmt = m_con->prepareStatement(sql);
		prep_stmt->executeUpdate();
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		const char* errstring = e.what();
		m_hasError = true;
		mbstowcs_s(&m_retval, temperrstring, 512, errstring, strlen(errstring));
		StringCbPrintfW(m_errstring, 512, L"%d: %s", errNo, temperrstring);
		//MessageBox();
		return false;
	}
	return true;
}