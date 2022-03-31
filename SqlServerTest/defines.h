#pragma once
// für Debug 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define MAX_INI_ENTRIES_LENGTH 100

/*
* defines for the config
*/
#define CONFIG_DIRECTORY	L"sqlservertest"	// the directory containing the INI file in the user home directory
#define CONFIG_FILE			L"sqlservertest.ini"
#define DB_SECTION			L"Database"

/*
* defines for ...????
*/
#define NUMELEM(p)          (sizeof(p)/sizeof((p)[0]))
#define MAX_LOADSTRING		150

/*
 * defines for the table
*/
#define CREATEDBY			L"reinhardp"	//  only for tests as a fixed value 
#define UPDATEDBY			L"irpag"        //  only for tests as a fixed value

#define IDB_STATUS			200

#define DATE_TIME_LENGTH    26
#define LASTNAME_LENGTH		50
#define FIRSTNAME_LENGTH	50
#define DTEXT_LENGTH		8000
#define CREATED_BY_LENGTH	50
#define UPDATED_BY_LENGTH	50


//ROUNDUP on all platforms pointers must be aligned properly
//#define ROUNDUP_AMOUNT	8
//#define ROUNDUP_(size,amount)	(((DBBYTEOFFSET)(size)+((amount)-1))&~((DBBYTEOFFSET)(amount)-1))
//#define ROUNDUP(size)			ROUNDUP_(size, ROUNDUP_AMOUNT)
//
//#define DISPLAY_METHODCALLS false
//
////Goes to CLEANUP on Failure
//#define CHECK_HR(hr)			\
//	if(FAILED(hr))				\
//		goto CLEANUP
//
////Goes to CLEANUP on Failure, and displays any ErrorInfo
//#define XCHECK_HR(hr)												\
//{																	\
//	if( g_dwFlags & DISPLAY_METHODCALLS )							\
//		fwprintf(stderr, LONGSTRING(#hr) L"\n");					\
//	if(FAILED(myHandleResult(hr, LONGSTRING(__FILE__), __LINE__)))	\
//		goto CLEANUP;												\
//}
//
//#define CHECK_MEMORY(hr, pv)	\
//{								\
//	if(!pv) 					\
//	{							\
//		hr = E_OUTOFMEMORY;		\
//		CHECK_HR(hr);			\
//	}							\
//}
//
#define MAX_ROWS				20
#define MIN_DISPLAY_SIZE		 3
#define MAX_DISPLAY_SIZE		20