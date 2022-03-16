#pragma once

#include "resource.h"
#include "framework.h"
//#include <msoledbsql.h>
//#include "OLEDb.h"
#include "defines.h"
#include "table.h"



HINSTANCE hInst;                                // Aktuelle Instanz
WCHAR szTitle[MAX_LOADSTRING];                  // Titelleistentext
WCHAR szWindowClass[MAX_LOADSTRING];            // Der Klassenname des Hauptfensters.
HDC g_hdc;
HWND g_statusindow;
WCHAR g_host[MAX_INI_ENTRIES_LENGTH] = { 0 };
WCHAR g_dbName[MAX_INI_ENTRIES_LENGTH] = { 0 };
WCHAR g_user[MAX_INI_ENTRIES_LENGTH] = { 0 };
WCHAR g_pwd[MAX_INI_ENTRIES_LENGTH] = { 0 };
WCHAR g_table[MAX_INI_ENTRIES_LENGTH] = { 0 };
WCHAR g_port[MAX_INI_ENTRIES_LENGTH] = { 0 };
WCHAR g_driver[MAX_INI_ENTRIES_LENGTH] = { 0 };;

WCHAR* g_rowdata = NULL;
BOOL g_updaterow = FALSE;
int g_selectedId = 0;
BOOL firstTime = TRUE;	// ????
Table* g_tableClass;
int g_cParts = 1;

bool g_tableCreated = false;

int* g_Ids;


// Operators for Calculator Dialog
TCHAR Operators[4][2] = {
	TEXT("+"), TEXT("-"),
	TEXT("/"), TEXT("*"),
};

HWND g_hwndList;
//MSSqlQuery* g_ODBCQuery;
//OLEDb* g_ODb;

HWND DoCreateStatusBar(HWND hwndParent, HINSTANCE hinst, int cParts);

/*
* functions for ????
*/
double Round(double Zahl, int Stellen);

void readINIFile(void);
BOOL writeINIFile(void);
void addTable(HWND hWnd, HINSTANCE hInstance);
void addHeader(void);
bool addTableBody(void);



