#pragma once

#include "defines.h"
#include "framework.h"
#include "includes.h"
#include <strsafe.h>
#include <msoledbsql.h>

// defines used only in OLEDb class
#define MAXBUFSIZE					31
#define NUMELEM(p)					(sizeof(p)/sizeof((p)[0]))
#define ISROWSET					100
#define ISROW						101
#define SQL_QUERYSTRING_LENGTH		1024
#define MAX_ROWS					20	//

class OLEDb
{

protected:	// variables
	unsigned int m_currentRow = 0;
	IDBInitialize* m_pIDBInitialize = NULL;
	IDBCreateCommand* m_pIDBCreateCommand = NULL;
	IDBCreateSession* m_pIDBCreateSession = NULL;
	
	ULONG m_numberOfColumns = 0;

	LPDATA m_data;	// pointer to struct to hold all rows gotten from the table

	WCHAR m_QueryString[SQL_QUERYSTRING_LENGTH] = { '\0' };	// The query string
	WCHAR m_host[MAX_INI_ENTRIES_LENGTH] = { '\0' };	//
	WCHAR m_port[MAX_INI_ENTRIES_LENGTH] = { '\0' };	//
	WCHAR m_dbName[MAX_INI_ENTRIES_LENGTH] = { '\0' };	//
	WCHAR m_user[MAX_INI_ENTRIES_LENGTH] = { '\0' };
	WCHAR m_pwd[MAX_INI_ENTRIES_LENGTH] = { '\0' };
	WCHAR m_table[MAX_INI_ENTRIES_LENGTH] = { '\0' };
	int m_lastID = 0;
    bool m_connected = false;
    SYSTEMTIME nowDate;
    
protected:	// methods

	/*
	* converts a WCHAR* to int
	* 
	* @param WCHAR*
	* @return nt
	*/
	int getNum(WCHAR* string) {
		int numeric_peek = 0;

		if (iswdigit(string[0])) {
			// store peek as numeric
			wchar_t s[2];
			s[0] = string[0];
			s[1] = 0;
			numeric_peek = _wtoi(s);
		}
		return numeric_peek;
	}

	void printErrorMSGBox(HRESULT hr, int flag);

	void FreeBindings(DBORDINAL cBindings, DBBINDING* rgBindings);

    
    /*
     * method to get the current date and time.
     * It uses GetSystemTime() to get the date/time
     *
     * @param none
     * @return pointer to struct (SYSTEMTIME struct)
    */
 /*   SYSTEMTIME& nowDateTime(void) 
    {
        SYSTEMTIME st = {0}; 
        GetSystemTime(&st);
        return st;
    }*/

    /*
     * method to get the current date and time.
     * It uses GetSystemTime() to get the date/time
     *
     * @param none
     * @return pointer to wchar_t (Date-Time)
    */
    inline wchar_t* nowDatetime(void) 
    {
        SYSTEMTIME st = {0}; 
        wchar_t now[DATE_TIME_LENGTH];
        
        GetSystemTime(&st);
        StringCchPrintf(now, DATE_TIME_LENGTH, L"%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        return now;
    }

    /*
     * method to get the current date and time
     *
     * @param wchar_t* (how to format the SYSTEMTIME struct)
     * @return pointer to wchar_t (Date-Time)
    */
    inline wchar_t* nowDateTime(wchar_t* format) 
    {
        SYSTEMTIME st = {0}; 
        wchar_t now[DATE_TIME_LENGTH];
        
        GetSystemTime(&st);
        StringCchPrintf(now, DATE_TIME_LENGTH, format, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        return now;
    }

public:

	/*
	*  constructor(s)
	*/
	inline OLEDb(void) {
		
	}


	/*
	* destructor
	*/
	~OLEDb();

	/*
	* connect to database
	* @param none
	* @return int
	*/
	int connect(void);

    /*
     * method to create the Accessor
     *
    */
    int createAccessor(IUnknown *pUnkRowset, HACCESSOR *phAccessor, DBORDINAL *pcBindings, DBBINDING **prgBindings, DBORDINAL *pcbRowSize);

    /*
     * method to setup bindings
    */
    int setupBindings(IUnknown *pUnkRowset, DBORDINAL *pcBindings, DBBINDING **prgBindings, DBORDINAL *pcbRowSize);
    
    /*
     * method to free bindings
     *
     * @param DBORDINAL
     * @param pinter to DBBINDING
    */
void freeBindings(DBORDINAL	cBindings, DBBINDING *rgBindings);

/*
	OLEDb& operator = (const WCHAR* sqlString)
	{
		
		StringCchPrintf(m_QueryString, MAX_INI_ENTRIES_LENGTH, L"%s", sqlString); // ??? WARUM?????
		return *this;
	}



	OLEDb& operator = (const OLEDb& oledb) {

		StringCchPrintf(m_QueryString, MAX_INI_ENTRIES_LENGTH, L"%s", oledb.m_QueryString);
		return *this;
	}

*/

	/*
	* init method. set the parameters for dbname, table...
	* @param WHAR*
	* @param WHAR*
	* @param WHAR*
	* @param WHAR*
	* @param WHAR*
	* 
	* @return void
	*/
	void init(WCHAR* host, WCHAR* dbName, WCHAR* user, WCHAR* pwd, WCHAR* table, WCHAR* port) {
		StringCchPrintfW(m_host, MAX_INI_ENTRIES_LENGTH, L"%s", host);
		StringCchPrintfW(m_port, MAX_INI_ENTRIES_LENGTH, L"%s", port);
		StringCchPrintfW(m_dbName, MAX_INI_ENTRIES_LENGTH, L"%s", dbName);
		StringCchPrintfW(m_user, MAX_INI_ENTRIES_LENGTH, L"%s", user);
		StringCchPrintfW(m_pwd, MAX_INI_ENTRIES_LENGTH, L"%s", pwd);
		StringCchPrintfW(m_table, MAX_INI_ENTRIES_LENGTH, L"%s", table);
	}

// **** set/get query strings ************
	// methods to set/get query strings
	/*
	* method to get the number of rows from a table
	* on
	*/
	int getNumberofRowsQuery(void);

	/*
	*
	*/
	unsigned int getLastIDQuery(void);

	/*
	* method to set a query string
	* 
	* @param WCHAR*, the query string
	*				 e.g.: SELECT id, firstname, ... FROM [dbname].dbo.[tablename] [ORDER BY ... ] ...
	* 
	* @return none
	*/
	void setQueryString(WCHAR* queryString) {
		StringCchPrintfW(m_QueryString, SQL_QUERYSTRING_LENGTH, L"%s", queryString);
	}

	/*
	* 
	*/
	int createSession(IUnknown* pUnkDataSource, IUnknown** ppUnkSession);

	/*
	*
	*/
	int createCommand(IUnknown* pUnkSession, IUnknown** ppUnkCommand);

	/*
	*
	*/
	int createRowset(IUnknown* pUnkSession, IUnknown** ppUnkRowset);


	/*
	*
	*/
	int executeCommand(IUnknown* pUnkCommand,
		ULONG					cPropSets,
		DBPROPSET* rgPropSets,
		IUnknown** ppUnkRowset
	);
    
    /*
     *
    */
    int displayRowset(IUnknown* pUnkRowset, LPCWSTR pwszColToReturn, ULONG cchBuffer, LPWSTR pwszBuffer);

    int updateDisplaySize
	(
	DBORDINAL				cBindings, 
	DBBINDING *				rgBindings, 
	void *					pData, 
	ULONG *					rgDispSize
	);

int GetProperty
	(
	IUnknown *				pIUnknown, 
	REFIID					riid, 
	DBPROPID				dwPropertyID, 
	REFGUID					guidPropertySet, 
	BOOL *					pbValue
	);
    

int DisplayRow
(
	ULONG					iRow, 
	DBORDINAL				cBindings, 
	DBBINDING *				rgBindings, 
	void *					pData, 
	ULONG *					rgDispSize
);

    
// ****** END set/get query strings ********

//
    //int 
// END

// **** methods to define methods for updating, insert delete ************
	/*
	* method to update a row
	* 
	* @param unsigned int
	* @param (struct DBPERSON)
	* @return int (can be -1 = error or 0 = success)
	* 
	*/
	int updateRow(unsigned int ID, LPDATA dbrow);

	/*
	* method to insert a new row
	* 
	* @param (struct) DBPERSON*
	* @return int (can be -1 = error or 0 = success)
	*
	*/
	int insertRow(LPDATA dbrow);

	/*
	* method to delete a row
	* 
	* @param unsigned int
	* @return int (can be -1 = error or 0 = success) 
	*/
	int deleteRow(unsigned int ID);

// END **** methods to define methods for updating, insert delete ************

// methods to relaese/disconnect
	/*
	* method to release handles
	*/
	void relaese(void);

	/*
	* method to disconnect from SQL server
	*/
	bool disconnect(void);

// END methods to relaese/disconnect


};

