#pragma once
#include "framework.h"


/*
 * the table header
 * 
 * will be used for test and real data
*/
const TCHAR tHeader[6][30] = {L"Vorname", L"Nachname", L"Geburtstag", L"Text", L"Erstellt von", L"Erstellt am" };	// Table header, es gibt auch noch updated von und updated wan Spalten


/*
* structure for the table 
* keine Ahnung ob notwendig!!!
*/
struct DATA {
	unsigned int ID;
	wchar_t firstname[30];
	wchar_t lastname[30];
	wchar_t birthday[11];
	wchar_t dtext[80000];
	wchar_t created_by[30];
	wchar_t created_at[24];
	wchar_t updated_by[30];
	wchar_t updated_at[24];
} ;

/*
 * Data for the Table for tests
*/
static DATA rgtBody[] = {
	{1,L"Reinhard", L"Pagitsch", L"1965-09-08", L"nur ein Test", L"2022-02-14 09:11:44.007", L"reinhardp", L"", L""},
	{2,L"Peter", L"Pagitsch", L"1971-09-06", L"nur ein Test 1", L"2022-02-14 09:11:44.008", L"reinhardp", L"", L""},
	{3,L"Gerti", L"Pagitsch", L"1943-01-03", L"nur ein Test 2", L"2022-02-14 09:11:44.009", L"reinhardp", L"", L""},
	{4,L"Martin", L"Pagitsch", L"1991-10-29", L"nur ein Test 3", L"2022-02-14 09:11:44.009", L"reinhardp", L"", L""},

	{5,L"Marina", L"Pagitsch", L"1969-10-08", L"nur ein Test 4", L"2022-02-14 09:11:44.008", L"reinhardp", L"", L""},
	{6,L"Petra", L"Pagitsch", L"1980-08-06", L"nur ein Test 5", L"2022-02-14 09:11:44.009", L"reinhardp", L"", L""},
	{7,L"Hilde", L"Pagitsch", L"1942-01-03", L"nur ein Test 6", L"2022-02-14 09:11:44.009", L"reinhardp", L"", L""},
	{8,L"Trude", L"Pagitsch", L"1940-11-10", L"nur ein Test 7", L"2022-02-14 09:11:44.007", L"reinhardp", L"", L""},
	{9,L"Paul", L"Pagitsch", L"1960-11-08", L"nur ein Test 8", L"2022-02-14 09:11:44.007", L"reinhardp", L"", L"" },
	{10,L"Manfred",	L"Pagitsch", L"1955-10-08", L"nur ein Test 9", L"2022-02-14 09:11:44.007", L"reinhardp", L"", L""}
};

static DATA* g_Body;