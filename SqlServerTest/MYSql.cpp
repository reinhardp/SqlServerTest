#include "MYSql.h"
#include <strsafe.h>

using namespace std;

/*
*
*/
void MYSql::init(WCHAR* host, WCHAR* port, WCHAR* database, WCHAR* table, WCHAR* user, WCHAR* password) {
	m_host = host;
	m_port = port;
	m_database = database;
	m_table = table;
	m_user = user;
	m_password = password;
}

/*
* 
*/
bool MYSql::connect() {

	WCHAR hostName[255] = { 0 };

	StringCbPrintfW(hostName, 255, L"tcp://%s:%s/%s", m_host, m_port, m_database);

	sql::ConnectOptionsMap connection_properties;
	connection_properties["hostName"] = hostName;	// tcp://127.0.0.1:port/db
	connection_properties["userName"] = m_user;
	connection_properties["password"] = m_password;
	connection_properties["OPT_RECONNECT"] = true;
	try {
		m_driver = get_driver_instance();
		m_con = m_driver->connect(connection_properties);
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		std::string errStr = e.getSQLState();

		return false;
	}

	return true;
}

/*
*
*/
bool MYSql::select(WCHAR* query) {
	try {
		m_stmt = m_con->createStatement();
		m_res = m_stmt->executeQuery((char*)query);
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		std::string errStr = e.getSQLState();
		//MessageBox();
		return false;
	}

	return true;
}

bool MYSql::select() {
	char query[1024] = "SELECT id, firstname, lastname, dtext, created_by, created_at, updated_by, updated_at from mstest";	
	try {
		m_stmt = m_con->createStatement();
		m_res = m_stmt->executeQuery(query);
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		std::string errStr = e.getSQLState();
		//MessageBox();
		return false;
	}

	return true;

}

/*
*
*/

bool MYSql::insert(WCHAR* firstname, WCHAR* lastname, WCHAR* birthday, WCHAR* dtext) {
	char statement[] = "INSERT INTO mstest(firstname, lastname, birthday, dtext, created_by) VALUES(?,?,?,?,?)";
	m_prep_stmt = m_con->prepareStatement(statement);
	
	m_prep_stmt->setString(1, (char*)firstname);
	m_prep_stmt->setString(2, (char*)lastname);
	m_prep_stmt->setString(3, (char*)birthday);
	m_prep_stmt->setString(4, (char*)dtext);
	m_prep_stmt->setString(5, (char*)CREATEDBY);
	m_prep_stmt->execute();

	delete m_prep_stmt;

	return true;
}

int MYSql::getLastID() {
	char query[] = "SELECT MAX(id) as lastID from mstest";
	int id = 0;
	try {
		m_stmt = m_con->createStatement();
		m_res = m_stmt->executeQuery(query);
		m_res->next();
		id = m_res->getInt("lastID");
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		std::string errStr = e.getSQLState();
		//MessageBox();
		return 0;
	}

	return id;
}

void MYSql::deleteID(int id) {
	char sql[] = "DELETE FROM mstest WHERE id = ?";

}

int MYSql::getNumberOfRows() {
	char sql[] = "SELECT count(id) as amount from mstest";
	int rows = 0;
	
	try {
		m_stmt = m_con->createStatement();
		m_res = m_stmt->executeQuery(sql);
		m_res->next();
		rows = m_res->getInt("amount");
	}
	catch (sql::SQLException& e) {
		int errNo = e.getErrorCode();
		std::string errStr = e.getSQLState();
		//MessageBox();
		return 0;
	}

	return rows;
}