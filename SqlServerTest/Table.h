#pragma once
#include "framework.h"
//#include "OLEDb.h"
#include "MYSql.h"

class Table : public MYSql
{
protected:
	HWND m_hWnd = NULL;			// main window handle
	HINSTANCE m_hinst = NULL;	// main window instance
	HWND m_hwndList = NULL;		// the list control to insert table header and body

	int m_x = 0, m_y = 0;

	/*
	* for pagination
	*/
	int m_totalPages = 0;
	int m_currentPage = 0;
	const int PAGE_SIZE = 10;

	int* m_Ids = 0; // a list of row ID's generated on the fly

public:

	/*
	*  constructors 
	*/
	Table() { }
	Table(HWND hWnd, HINSTANCE hinst) {
		m_hWnd = hWnd;
		m_hinst = hinst;
	}
	Table(HWND hWnd, HINSTANCE hinst, int x, int y) {
		m_hWnd = hWnd;
		m_hinst = hinst;
		m_x = x;
		m_y = y;
	}

	/*
	* destructor
	*/
	~Table() { }

	/*
	* method to get the DB ID list
	* 
	* @param none
	* @return int
	*/
	int* GetIds() {
		return m_Ids;
	}


	int Init(HWND hWnd, HINSTANCE hinst);
	bool CreateTable();
	bool ResizeTable(HWND hWndstatus, HWND mainhWnd, int cParts);

	bool CreateHeader();
	bool CreateBody();
	
	bool CreateGridTable();
	bool CreateGridHeader();
	bool CreateGridBody();

	bool CreateOLeDBBody();

	bool CreateMySQLDBBody(int rows);
	bool CreateMySQLDBBody();

	bool recalculateColumnwidth();
};

