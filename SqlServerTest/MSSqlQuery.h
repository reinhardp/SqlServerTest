#pragma once
#include <tchar.h>
#include "framework.h"
#include "sql.h"
#include <sqlext.h>  

#define MAXBUFSIZE 31
#define NUMELEM(p)                          (sizeof(p)/sizeof((p)[0]))
#define DB_SECTION							L"Database"
#define CONFIG_FILE							L"sqlservertest.ini"


class MSSqlQuery
{
private:
	
	WCHAR m_Driver[40] = { 0 };
	WCHAR m_host[40] = {0};	//
	WCHAR m_port[40] = { 0 };	//
	WCHAR m_dbName[40] = { 0 };	//
	WCHAR m_user[40] = { 0 };
	WCHAR m_pwd[40] = { 0 };
	WCHAR m_table[40] = { 0 };
	WCHAR m_wszConfigurationFile[MAX_PATH] = { 0 };
	WCHAR m_SQLString[10240] = {0};
	unsigned int m_currentRow = 0;
	
	/*
	* SQL member variables
	*/
	SQLHENV m_henv;
	SQLHDBC m_hdbc;
	SQLHSTMT m_hstmt;
	


public:
	MSSqlQuery();
	MSSqlQuery(HWND hWnd); // hWnd is a window handle. The mainwindow 
	
	
	/*
	* destructor
	*/
	~MSSqlQuery();
	
	void freeConnection(void);

	bool init(void);

	/*
	*/
	int connect(HINSTANCE hInst, HWND hWnd);

	/*
	 *  get all rows stored in the database
	 * @param none
	 * @return array
	*/
	bool getAll(WCHAR **);
	
	/*
	 * get the last stored ID in the database
	 * @param none
	 * @return the ID
	*/
	unsigned int getLastID(void);
	
	/*
	 * get a person row 
	 * @param _IN_
	 * @param _OUT_
	 * @rturn none
	*/
	void getPerson(unsigned int id /* id of the person */, WCHAR** row /* array */);
	
	/*
	 * add a person to database
	 * 
	 * @param firstname
	 * @param lastname
	 * @param description
	 * @param date
	 * @param created_at
	 * @param created_by
	 * @param hwnd
	 * return BOOL
	*/
	BOOL addPerson(WCHAR* firstname, WCHAR* lastname, WCHAR* description, WCHAR* date, WCHAR* created_at, WCHAR* created_by, HWND hwnd);
	
	/*
	 * delete a person
	 * 
	 * @param id
	 * @return BOOL
	*/
	BOOL deletePerson(unsigned int id);
	
	/*
	 *  update a persons parameter
	 * @param
	 * @return BOOL
	*/
	BOOL updatePerson(unsigned int id, WCHAR* firstname, WCHAR* lastname, WCHAR* description, WCHAR* date, WCHAR* created_at, WCHAR* created_by);

	/*
	*/
	bool getFirstRow(OUT WCHAR** row);

	/*
	*/
	bool getNextRow(OUT WCHAR** row);
	/*
	*/
	void disconnect(void);

private:	// private functions


};

