#pragma once
#include <windows.h>
#include "defines.h"
#include <mysql/jdbc.h>

class MYSql
{
protected:
    /*
    * MYSQL 
    */
    sql::Connection* m_con = NULL;
    sql::Statement*  m_stmt = NULL;
    sql::ResultSet*  m_res = NULL;
    sql::Driver*     m_driver = NULL;
    sql::PreparedStatement* m_prep_stmt = NULL;

    /*
    * All other
    *
    */
    WCHAR* m_host = NULL;
    WCHAR* m_port = NULL;
    WCHAR* m_user = NULL;
    WCHAR* m_password = NULL;
    WCHAR* m_database = NULL;
    WCHAR* m_table = NULL;
    int m_lastID = 0;
    bool m_connected = false;
    bool m_updated = false;
    bool m_hasError = false;
    size_t m_retval = 0;

private:
    wchar_t m_errstring[512] = { 0 };


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

    void init(WCHAR *host, WCHAR *port, WCHAR *database, WCHAR *user, WCHAR *password);
    bool connect();
    
    void disconnect() {
        if (m_res) {
            delete m_res;
            m_res = NULL;
        }
            
        if (m_stmt) {
            delete m_stmt;
            m_stmt = NULL;
        }
            
        if (m_con) {
            delete m_con;
            m_con = NULL;
        }

        m_connected = false;
    }

    bool select(WCHAR *query);
    bool select();
    bool insert(WCHAR* firstname, WCHAR* lastname, WCHAR* birthday, WCHAR* dtext);
    bool update(int id, WCHAR* firstname, WCHAR* lastname, WCHAR* birthday, WCHAR* dtext);
    bool deleteID(int id);
    
    int getLastID();
    int getNumberOfRows();
    bool isConnected() {
        return m_connected;
    }

    bool setNames();

    wchar_t* getErrString() {
        return m_errstring;
    }

    void getErrString(wchar_t* errstring) {
        errstring = m_errstring;
    }

    bool hasError() {
        return m_hasError;
    }

    bool isUpdated() {
        return m_updated;
    }
};

