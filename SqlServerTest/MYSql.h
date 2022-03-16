#pragma once
#include <windows.h>
#include "defines.h"
//#include "mysql_connection.h"

//#include <cppconn/driver.h>
//#include <cppconn/exception.h>
//#include <cppconn/resultset.h>
//#include <cppconn/statement.h>
//#include <cppconn/prepared_statement.h>
#include <mysql/jdbc.h>

class MYSql
{
protected:
    /*
    * MYSQL 
    */
    sql::Connection* m_con;
    sql::Statement*  m_stmt;
    sql::ResultSet*  m_res;
    sql::Driver*     m_driver;
    sql::PreparedStatement* m_prep_stmt;

    WCHAR* m_host = NULL;
    WCHAR* m_port = NULL;
    WCHAR* m_user = NULL;
    WCHAR* m_password = NULL;
    WCHAR* m_database = NULL;
    WCHAR* m_table = NULL;
    int m_lastID = 0;

    /*
    * All other
    * 
    */


public:

    MYSql() {
        m_con = NULL;
        m_stmt = NULL;
        m_res = NULL;
        m_driver = NULL;
        m_prep_stmt = NULL;

    };

    ~MYSql() {
        if(m_res)
            delete m_res;

        if(m_stmt)
            delete m_stmt;
        
        if(m_con)
            delete m_con;
    }

    void init(WCHAR *host, WCHAR *port, WCHAR *database, WCHAR* table,WCHAR *user, WCHAR *password);
    bool connect();
    void disconnect() {
        if (m_res)
            delete m_res;

        if (m_stmt)
            delete m_stmt;

        if (m_con)
            delete m_con;

    }

    bool select(WCHAR *query);
    bool select();
    bool insert(WCHAR* firstname, WCHAR* lastname, WCHAR* birthday, WCHAR* dtext);
    void deleteID(int id);
    
    int getLastID();
    int getNumberOfRows();
};

