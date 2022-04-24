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
#if USE_UNICODE == true
    WCHAR* m_host = NULL;
    WCHAR* m_port = NULL;
    WCHAR* m_user = NULL;
    WCHAR* m_password = NULL;
    WCHAR* m_database = NULL;
    WCHAR* m_table = NULL;
#else
    char* m_host = NULL;
    char* m_port = NULL;
    char* m_user = NULL;
    char* m_password = NULL;
    char* m_database = NULL;
    char* m_table = NULL;
#endif
    int m_lastID = 0;
    bool m_connected = false;
    bool m_updated = false;
    bool m_hasError = false;
    size_t m_retval = 0;

private:


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
    void initA(char* host, char* port, char* database, char* user, char* password);

    bool connect();
    bool connectA();
    
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

    bool select();
    bool select(char* query);
    bool select(int maxRows, int currentPage);

    
    bool insert(WCHAR* firstname, WCHAR* lastname, WCHAR* birthday, WCHAR* dtext);
    bool insertA(char* firstname, char* lastname, char* birthday, char* dtext);
    bool update(int id, WCHAR* firstname, WCHAR* lastname, WCHAR* birthday, WCHAR* dtext);
    bool updateA(int id, char* firstname, char* lastname, char* birthday, char* dtext);
    bool deleteID(int id);
    
    int getLastID();
    int getNumberOfRows();
    bool isConnected() {
        return m_connected;
    }

    bool setNames();

    bool hasError() {
        return m_hasError;
    }

    bool isUpdated() {
        return m_updated;
    }
};

