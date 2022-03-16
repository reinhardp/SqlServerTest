#include "OLEDb.h"
#include "includes.h"
#include <tchar.h>
#include <strsafe.h>
#include <userenv.h>
#include <wtypes.h>
#include <Objbase.h>
#include <oledb.h>
#include <msoledbsql.h>
//#include <msdadc.h>   // for IDataConvert  
//#include <msdaguid.h>   // for IDataConvert  


#define DBINITCONSTANTS  
#define INITGUID  

/* protected/private methods

// there are no private methods

*/ //end protected/private methods

// public methods

/* destructor 
 * relese/destroy all allocated memory
*/
OLEDb::~OLEDb() {
/*
    if (m_colvalues)
        delete[] m_colvalues;
*/
}


/*
 * method to connect to the MS SQL? database.
 * It uses "global" information for HOST, database, username, password,..
 * These informations (HOST, database, username, password ..) comming from the calling method.
 *
 * @param none
 * @return int
*/
int OLEDb::connect(void) {
    ULONG i;
    HRESULT hr = E_FAIL;
    IDBProperties* pIDBProperties = NULL;
    DBPROP InitProperties[5];
    DBPROPSET rgInitPropSet[1];

    hr = CoInitialize(NULL);
    //hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED); // ?????
    if (hr != S_OK) {
        if (hr == RPC_E_CHANGED_MODE) {
            printErrorMSGBox(hr, 0);
        }
        else {
            printErrorMSGBox(hr, 4);
        }
        CoUninitialize();
        
        return E_FAIL;
    }
    hr = CoCreateInstance(CLSID_MSOLEDBSQL,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IDBInitialize,
        (void**)&m_pIDBInitialize);
    if (FAILED(hr)) {
        printErrorMSGBox(hr, 5);
		return E_FAIL;
    }

    // Initialize the property values needed to establish the connection.  
    for (i = 0; i < 5; i++)
        VariantInit(&InitProperties[i].vValue);

    InitProperties[0].dwPropertyID = DBPROP_INIT_DATASOURCE;
    InitProperties[0].vValue.vt = VT_BSTR;

    InitProperties[0].vValue.bstrVal = SysAllocString(m_host);
    InitProperties[0].dwOptions = DBPROPOPTIONS_REQUIRED;
    InitProperties[0].colid = DB_NULLID;

    // Database.  
    InitProperties[1].dwPropertyID = DBPROP_INIT_CATALOG;
    InitProperties[1].vValue.vt = VT_BSTR;
    InitProperties[1].vValue.bstrVal = SysAllocString(m_dbName);
    InitProperties[1].dwOptions = DBPROPOPTIONS_REQUIRED;
    InitProperties[1].colid = DB_NULLID;
    
    InitProperties[2].dwPropertyID = DBPROP_AUTH_USERID;
    InitProperties[2].vValue.vt = VT_BSTR;
    InitProperties[2].vValue.bstrVal = SysAllocString(m_user);
    InitProperties[2].dwOptions = DBPROPOPTIONS_REQUIRED;
    InitProperties[2].colid = DB_NULLID;

    InitProperties[3].dwPropertyID = DBPROP_AUTH_PASSWORD;
    InitProperties[3].vValue.vt = VT_BSTR;
    InitProperties[3].vValue.bstrVal = SysAllocString(m_pwd);
    InitProperties[3].dwOptions = DBPROPOPTIONS_REQUIRED;
    InitProperties[3].colid = DB_NULLID;

    // Properties are set, now construct the DBPROPSET structure (rgInitPropSet) used to pass   
   // an array of DBPROP structures (InitProperties) to the SetProperties method.  
    rgInitPropSet[0].guidPropertySet = DBPROPSET_DBINIT;
    rgInitPropSet[0].cProperties = 4;
    rgInitPropSet[0].rgProperties = InitProperties;

    hr = m_pIDBInitialize->QueryInterface(IID_IDBProperties, (void**)&pIDBProperties);
    if (FAILED(hr)) {
        printErrorMSGBox(hr, 2);
        return E_FAIL;
    }

    hr = pIDBProperties->SetProperties(1, rgInitPropSet);
    if (FAILED(hr)) {
        // Handle errors here.  
        printErrorMSGBox(hr, 3);
        return E_FAIL;
    }

    pIDBProperties->Release();

    // Now establish the connection to the data source.  
    hr = m_pIDBInitialize->Initialize();
    if (FAILED(hr)) {
        // Handle errors here.  
        printErrorMSGBox(hr, 1);
        return E_FAIL;
    }
    
    m_connected = true;
    return 0;
}


/*
 * method to release variables
 * only member-variables are released!
 * no parameter or return
 *
*/
void OLEDb::relaese(void) {
    if(m_pIDBCreateCommand != NULL)
        m_pIDBCreateCommand->Release();
    
    if(m_pIDBCreateSession != NULL)
        m_pIDBCreateSession->Release();
    
    if (FAILED(m_pIDBInitialize->Uninitialize())) {
        // Uninitialize is not required, but it fails if an interface  
        // has not been released.  This can be used for debugging.
    }
    if(m_pIDBInitialize != NULL)
        m_pIDBInitialize->Release();
    
    CoUninitialize();

}

/*
 * query get the last ID from the database 
 * set only the querystring!
*/
unsigned int OLEDb::getLastIDQuery(void) {

    m_lastID = 0;
    WCHAR format[255] = L"SELECT TOP(1) id FROM %s.dbo.%s ORDER BY id DESC";
    StringCchPrintf(m_QueryString, SQL_QUERYSTRING_LENGTH, format, m_dbName, m_table);
    
    return m_lastID;
}

/*
* method to disconnect from the MS SQL Server
*/
bool OLEDb::disconnect(void) {

    CoUninitialize();
    return true;
}


/*
* query method to update a row (data)
*
* @param unsigned int
* @param struct LPDATA ( a struct to hold information of data to for updating the specified (by ID) row)
* @return int (can be -1 = error or 0 = success)
*
*/
int OLEDb::updateRow(unsigned int ID, LPDATA dbrow) {

    int ret = -1;
    WCHAR format[255] = L"UPDATE ... WHERE id = %d";
    StringCchPrintf(m_QueryString, SQL_QUERYSTRING_LENGTH, format, ID);

    return ret;
}


/*
 * query method to insert a new row (data)
 *
 * @param struct
 * @return int (can be -1 = error or 0 = success)
 *
*/
int OLEDb::insertRow(LPDATA dbrow) {
    WCHAR format[255] = L"INSERT INTO %s.dbo.%s ";
    StringCchPrintf(m_QueryString, SQL_QUERYSTRING_LENGTH, format, 1);

    return 0;
}

/*
 * From the prsample project
 *
 * query method to delete a row
 *
 * @param unsigned int
 * @return int (can be -1 = error or 0 = success)
*/
int OLEDb::deleteRow(unsigned int ID) {
    WCHAR format[255] = L"DELETE FROM %s.dbo.%s WHERE id = %d";
    StringCchPrintf(m_QueryString, SQL_QUERYSTRING_LENGTH, format, m_dbName, m_table, ID);
    return 0;
}

/*
 * From the prsample project
 * 
*/
int OLEDb::createSession(IUnknown* pUnkDataSource, IUnknown** ppUnkSession) {
    HRESULT hr;
    IDBCreateSession* pIDBCreateSession = NULL;

    //Create a Session Object from a Data Source Object
    hr = pUnkDataSource->QueryInterface(
        IID_IDBCreateSession, (void**)&pIDBCreateSession);
    if (FAILED(hr))
    {
        printErrorMSGBox(hr, 14);
        return E_FAIL;
    }
    
    hr = pIDBCreateSession->CreateSession(
        NULL,				//pUnkOuter
        IID_IOpenRowset,	//riid
        ppUnkSession		//ppSession
    );
    if (FAILED(hr))
    {
        printErrorMSGBox(hr, 15);
        return E_FAIL;
    }

    if (pIDBCreateSession)
        pIDBCreateSession->Release();
    return hr;
}

/*
 * From the prsample project
 * 
*/
int OLEDb::createCommand(IUnknown* pUnkSession, IUnknown** ppUnkCommand) {
    HRESULT	hr = E_FAIL;
    IDBCreateCommand* pIDBCreateCommand = NULL;

    // Attempt to create a Command object from the Session object
    hr = pUnkSession->QueryInterface(IID_IDBCreateCommand, (void**)&pIDBCreateCommand);
    if (FAILED(hr))
    {
        printErrorMSGBox(hr, 6);
        return E_FAIL;
    }
    hr = pIDBCreateCommand->CreateCommand(
        NULL,				//pUnkOuter
        IID_ICommand,		//riid
        ppUnkCommand		//ppCommand
    );
    if (FAILED(hr))
    {
        printErrorMSGBox(hr, 7);
        return E_FAIL;
    }
// cleanup
    if (pIDBCreateCommand)
        pIDBCreateCommand->Release();
    return hr;
}


/*
 * From the prsample project
 *
*/
int OLEDb::executeCommand(IUnknown* pUnkCommand, ULONG cPropSets, DBPROPSET* rgPropSets, IUnknown** ppUnkRowset) {
    HRESULT	hr = E_FAIL;

    ICommandText*       pICommandText = NULL;
    ICommandProperties* pICommandProperties = NULL;

    // Set the properties on the Command object
    hr = pUnkCommand->QueryInterface(
        IID_ICommandProperties, (void**)&pICommandProperties);
    if (FAILED(hr))
    {
        printErrorMSGBox(hr, 8);
        return E_FAIL;
    }

    hr = pICommandProperties->SetProperties(cPropSets, rgPropSets);
    if (FAILED(hr))
    {
        printErrorMSGBox(hr, 9);
        return E_FAIL;
    }

    // Set the text for this Command, using the default command text
    // dialect. All providers that support commands must support this
    // dialect and providers that support SQL must be able to recognize
    // an SQL command as SQL when this dialect is specified
    hr = pUnkCommand->QueryInterface(
        IID_ICommandText, (void**)&pICommandText);
    if (FAILED(hr))
    {
        printErrorMSGBox(hr, 10);
        return E_FAIL;
    }

    hr = pICommandText->SetCommandText(
        DBGUID_DEFAULT,		//guidDialect
        m_QueryString		//m_QueryString
    );
    if (FAILED(hr))
    {
        printErrorMSGBox(hr, 11);
        return E_FAIL;
    }

    // And execute the Command. Note that the user could have
    // entered a non-row returning command, so we will check for
    // that and return failure to prevent the display of the
    // non-existant rowset by the caller
    hr = pICommandText->Execute(
        NULL,			//pUnkOuter
        IID_IRowset,	//riid
        NULL,			//pParams
        NULL,			//pcRowsAffected
        ppUnkRowset		//ppRowset
    );

    if (!*ppUnkRowset)
    {
        printErrorMSGBox(hr, 12);
        return E_FAIL;
    }


CLEANUP:
    if (pICommandText)
        pICommandText->Release();
    if (pICommandProperties)
        pICommandProperties->Release();
    return hr;
}

/*
 * From the prsample project
 *
 * method to generate data for displaing he whole rowset
*/
int OLEDb::displayRowset(IUnknown* pUnkRowset, LPCWSTR pwszColToReturn, ULONG cchBuffer, LPWSTR pwszBuffer) {
	HRESULT					hr = E_FAIL;
	IRowset*				pIRowset					= NULL;
	DBORDINAL				cBindings;
	DBBINDING*				rgBindings					= NULL;	
	HACCESSOR				hAccessor					= DB_NULL_HACCESSOR;
	DBORDINAL				cbRowSize;
	void *					pData						= NULL;
	ULONG *					rgDispSize					= NULL;
	DBCOUNTITEM				cRowsObtained;
	HROW *					rghRows						= NULL;
	ULONG					iRow;
	LONG					cRows						= MAX_ROWS;
	LONG					iRetCol						= -1;
	BOOL					fCanFetchBackwards;
	DBORDINAL				iIndex;
	void *					pCurData;

	// Obtain the IRowset interface for use in fetching rows and data
	hr = pUnkRowset->QueryInterface(IID_IRowset, (void**)&pIRowset);
    if (FAILED(hr))
    {
        printErrorMSGBox(hr, 16);
        return E_FAIL;
    }
    
    // Determine whether this rowset supports fetching data backwards;
	// we use this to determine whether the rowset can support moving
	// to the previous set of rows, described in more detail below
	GetProperty(pUnkRowset, IID_IRowset, DBPROP_CANFETCHBACKWARDS, 
		DBPROPSET_ROWSET, &fCanFetchBackwards);
        
    hr = createAccessor(pUnkRowset, &hAccessor, &cBindings, &rgBindings, &cbRowSize);

	// Allocate enough memory to hold cRows rows of data; this is
	// where the actual row data from the provider will be placed
	pData = CoTaskMemAlloc(cbRowSize * MAX_ROWS);
	CHECK_MEMORY(hr, pData);
 	// Allocate memory for an array that we will use to calculate the
	// maximum display size used by each column in the current set of
	// rows
	rgDispSize = (ULONG *)CoTaskMemAlloc(cBindings * sizeof(ULONG));
	CHECK_MEMORY(hr, rgDispSize);
	// In this loop, we perform the following process:
	//  - reset the maximum display size array
	//  - try to get cRows row handles from the provider
	//  - these handles are then used to actually get the row data from the
	//    provider copied into our allocated buffer
	//  - calculate the maximum display size for each column
	//  - release the row handles to the rows we obtained
	//  - display the column names for the rowset
	//  - display the row data for the rows that we fetched
	//  - get user input
	//  - free the provider-allocated row handle array
	//  - repeat unless the user has chosen to quit or has selected a row
	while( hr == S_OK )
	{
		// Clear the maximum display size array
		memset(rgDispSize, 0, cBindings * sizeof(ULONG));

		// Attempt to get cRows row handles from the provider
		hr = pIRowset->GetNextRows(
					DB_NULL_HCHAPTER,	//hChapter
					0,					//lOffset
					cRows,				//cRows
					&cRowsObtained,		//pcRowsObtained
					&rghRows			//prghRows
					);			
        m_data = new DATA[cRowsObtained];

		// Loop over the row handles obtained from GetNextRows,
		// actually fetching the data for these rows into our buffer
		for( iRow = 0; iRow < cRowsObtained; iRow++ )
		{
			// Find the location in our buffer where we want to place
			// the data for this row. Note that if we fetched rows
			// backwards (cRows < 0), the row handles obtained from the
			// provider are reversed from the order in which we want to
			// actually display the data on the screen, so we will
			// account for this. This ensures that the resulting order
			// of row data in the pData buffer matches the order we
			// wish to use to display the data
			iIndex		= cRows > 0 ? iRow : cRowsObtained - iRow - 1;
			pCurData	= (BYTE*)pData + (cbRowSize * iIndex);
			
			// Get the data for this row handle. The provider will copy
			// (and convert, if necessary) the data for each of the
			// columns that are described in our Accessor into the given
			// buffer (pCurData)
			XCHECK_HR(hr = pIRowset->GetData(
					rghRows[iRow],	//hRow 
					hAccessor,		//hAccessor
					pCurData		//pData
					));

			// Update the maximum display size array, accounting for this row
			CHECK_HR(hr = updateDisplaySize(cBindings, rgBindings, pCurData, rgDispSize));
		}

		// If we obtained rows, release the row handles for the retrieved rows
		// and display the names of the rowset columns before we display the data
		if( cRowsObtained )
		{
			// Release the row handles that we obtained
			XCHECK_HR(hr = pIRowset->ReleaseRows(
						cRowsObtained,	//cRows
						rghRows,		//rghRows
						NULL,			//rgRowOptions
						NULL,			//rgRefCounts
						NULL			//rgRowStatus
						));

			
			// Display the names of the rowset columns
			//CHECK_HR(hr = myDisplayColumnNames(pIRowset, rgDispSize));
		}
		
		// For each row that we obtained the data for, display this data
		for( iRow = 0; iRow < cRowsObtained; iRow++ )
		{
			// Get a pointer to the data for this row
			pCurData = (BYTE*)pData + (cbRowSize* iRow);

			// And display the row data
			CHECK_HR(hr = DisplayRow(iRow, cBindings, rgBindings,
				pCurData, rgDispSize));
		}

		// Allow the user to navigate the rowset. This displays the appropriate
		// prompts, gets the user's input, may call IRowset::RestartPosition,
		// and may copy data from a selected row to the selection buffer, if so
		// directed. This will return S_OK if the user asked for more rows,
		// S_FALSE if the user selected a row, or E_FAIL if the user quits
		//hr = interactWithRowset(
		//	pIRowset,					// IRowset pointer, for RestartPosition
		//	&cRows,						// updated with fetch direction value
		//	cRowsObtained,				// to indicate selection range
		//	fCanFetchBackwards,			// whether [P]revious is supported
		//	pData,						// data pointer for copying selection
		//	cbRowSize,					// size of rows for copying selection
		//	iRetCol >= 0 ?				// bindings for the selection column,
		//		&rgBindings[iRetCol] :	//  or NULL if no selection column
		//		NULL,
		//	cchBuffer,					// size of the selection buffer
		//	pwszBuffer);				// pointer to the selection buffer

		// Since we are allowing the provider to allocate the memory for 
		// the row handle array, we will free this memory and reset the 
		// pointer to NULL. If this is not NULL on the next call to GetNextRows,
		// the provider will assume that it points to an allocated array of
		// the required size (which may not be the case if we obtained less
		// than cRows rows from this last call to GetNextRows
		CoTaskMemFree(rghRows);
		rghRows = NULL;
	}

CLEANUP:
	freeBindings(cBindings, rgBindings);
	CoTaskMemFree(rgDispSize);
	CoTaskMemFree(pData);
	if( pIRowset )
		pIRowset->Release();

	return hr;
   
}


/*
 * From the prsample project
* 
*/
int OLEDb::createRowset(IUnknown* pUnkSession, IUnknown** ppUnkRowset) {

    HRESULT					hr = E_FAIL;
    IUnknown* pUnkCommand = NULL;
    IOpenRowset* pIOpenRowset = NULL;
    //WCHAR					wszTableName[MAX_NAME_LEN + 1] = { 0 };

    const ULONG				cProperties = 2;
    DBPROP					rgProperties[cProperties];
    DBPROPSET				rgPropSets[1];

    //hr = createSchemaRowset(DBSCHEMA_TABLES, pUnkSession, SQL_QUERYSTRING_LENGTH, m_QueryString);

    //addRowsetProperties(rgPropSets, cProperties, rgProperties);
}

/*
 * From the prsample project
 *
 * Create an Accessor. An Accessor is basically a handle to a
 * collection of bindings that describes to the provider how to
 * copy (and convert, if necessary) column data into our buffer.
 * The Accessor that this creates will bind all columns as either
 * DBTYPE_WSTR (a Unicode string) or as an ISequentialStream object
 * (used for BLOB data). This will also give us the size of the
 * row buffer that the Accessor describes to the providerCreate an Accessor. An Accessor is basically a handle to a
 * collection of bindings that describes to the provider how to
 * copy (and convert, if necessary) column data into our buffer.
 * The Accessor that this creates will bind all columns as either
 * DBTYPE_WSTR (a Unicode string) or as an ISequentialStream object
 * (used for BLOB data). This will also give us the size of the
 * row buffer that the Accessor describes to the provider
 *  
 * @param IUnknown*
 * @param HACCESSOR*
 * @param DBORDINAL*
 * @param DBBINDING**
 * @param DBORDINAL*
 * 
 * @return 
 */
int OLEDb::createAccessor(
    IUnknown*	pUnkRowset, 
    HACCESSOR* phAccessor, 
    DBORDINAL* pcBindings, 
    DBBINDING** prgBindings, 	
    DBORDINAL* pcbRowSize
)
{
	HRESULT hr = E_FAIL;
	IAccessor *pIAccessor = NULL;

	// An Accessor is basically a handle to a collection of bindings.
	// To create the Accessor, we need to first create an array of
	// bindings for the columns in the Rowset
	hr = setupBindings(pUnkRowset, pcBindings, prgBindings, pcbRowSize);
	if(FAILED(hr)) {
        // display error message
        
        goto CLEANUP;
    }
    hr = pUnkRowset->QueryInterface(IID_IAccessor, (void**)&pIAccessor);
	if(FAILED(hr)) {
        // display error message
        
        goto CLEANUP;
    }

	hr = pIAccessor->CreateAccessor(
				DBACCESSOR_ROWDATA,	//dwAccessorFlags
				*pcBindings,		//cBindings
				*prgBindings,		//rgBindings
				0,					//cbRowSize
				phAccessor,			//phAccessor	
				NULL				//rgStatus
				);
	if(FAILED(hr)) {
        // display error message
        
        goto CLEANUP;
    }

CLEANUP:
	if( pIAccessor )
		pIAccessor->Release();
	return hr;

}

/*
*
*/
void OLEDb::printErrorMSGBox(HRESULT hr, int flag) {
    switch (hr)
    {
    case E_FAIL:    // wenn E_FAIL dann eigene message für unterschiedliche Aufrufe!
    {
        if (flag == 1) // m_pIDBInitialize->Initialize()
        {
            MessageBox(NULL, L"IProblem in establishing connection to the data source!", L"Initialize", MB_OK | MB_ICONERROR);
        } else if (flag == 2) // m_pIDBInitialize->QueryInterface
        {
            MessageBox(NULL, L"Failed to get IDBProperties interface!", L"QueryInterface", MB_OK | MB_ICONERROR);
        } else if (flag == 3)	// pIDBProperties->SetProperties
        {
            MessageBox(NULL, L"Failed to set initialization properties!", L"SetProperties", MB_OK | MB_ICONERROR);
        } else if (flag == 4)	// CoInitialize
        {
            MessageBox(NULL, L"CoInitialize failed!", L"CoInitialize", MB_OK | MB_ICONERROR);
        } else if(flag == 5)   // CoCreateInstance
        {
            MessageBox(NULL, L"CoCreateInstance failed!", L"CoCreateInstance", MB_OK | MB_ICONERROR);
        } else if (flag == 6) //pUnkSession->QueryInterface
        {
            MessageBox(NULL, L"pUnkSession->QueryInterface failed!", L"pUnkSession->QueryInterface", MB_OK | MB_ICONERROR);
        } else if (flag == 7) 
        {   //
            MessageBox(NULL, L"pIDBCreateCommand->CreateCommand failed!", L"pUnkSession->QueryInterface", MB_OK | MB_ICONERROR);

        } else if (flag == 8) {
            MessageBox(NULL, L"pIDBCreateCommand->CreateCommand failed!", L"pUnkSession->QueryInterface", MB_OK | MB_ICONERROR);
        }
        else if (flag == 9) {
            MessageBox(NULL, L"pICommandProperties->SetProperties failed!", L"pICommandProperties->SetProperties!", MB_OK | MB_ICONERROR);
        }
        else if (flag == 10) {
            MessageBox(NULL, L"pUnkCommand->QueryInterface failed!", L"pUnkCommand->QueryInterface!", MB_OK | MB_ICONERROR);
        }
        else if (flag == 11) {
            MessageBox(NULL, L"pICommandText->SetCommandText failed!", L"pICommandText->SetCommandText!", MB_OK | MB_ICONERROR);
        }
        else if (flag == 12) {
            MessageBox(NULL, L"The command executed successfully, but did not return a rowset.\nNo rowset will be displayed.\n!", L"Error no rowset!", MB_OK | MB_ICONERROR);
        }
        else if (flag == 13) {
            MessageBox(NULL, L"Unknown Error!", L"Error!", MB_OK | MB_ICONERROR);
        }
        else if(flag == 14) {   //pUnkDataSource->QueryInterface
            MessageBox(NULL, L"pUnkDataSource->QueryInterface failed!", L"pUnkDataSource->QueryInterface!", MB_OK | MB_ICONERROR);
        } 
        else if(flag == 15) {
            MessageBox(NULL, L"pIDBCreateSession->CreateSession failed!", L"pIDBCreateSession->CreateSession!", MB_OK | MB_ICONERROR);
        }
        else if(flag == 16) {
            MessageBox(NULL, L"pUnkRowset->QueryInterface failed!", L"pUnkRowset->QueryInterface!", MB_OK | MB_ICONERROR);            
        }            
        else // nur dann wenn wenn sonst nichts gefunden
        {
            MessageBox(NULL, L"Unknown Error!\nDo not know what happens!", L"Error!", MB_OK | MB_ICONERROR);
        }

        break;
    }
    case RPC_E_CHANGED_MODE:
    {
        MessageBox(NULL, L"CoInitialize failed with RPC_E_CHANGED_MODE!", L"CoInitialize", MB_OK | MB_ICONERROR);
        break;
    }
    default:
        break;
    }
}

int OLEDb::createAccessor(IUnknown *pUnkRowset, HACCESSOR *phAccessor, DBORDINAL *pcBindings, DBBINDING **prgBindings, DBORDINAL *pcbRowSize)
{
	HRESULT hr = E_FAIL;
	IAccessor *pIAccessor = NULL;
	// An Accessor is basically a handle to a collection of bindings.
	// To create the Accessor, we need to first create an array of
	// bindings for the columns in the Rowset
	CHECK_HR(hr = setupBindings(pUnkRowset, pcBindings, prgBindings, pcbRowSize));
	
	// Now that we have an array of bindings, tell the provider to
	// create the Accessor for those bindings. We get back a handle
	// to this Accessor, which we will use when fetching data
	XCHECK_HR(hr = pUnkRowset->QueryInterface(IID_IAccessor, (void**)&pIAccessor));

	XCHECK_HR(hr = pIAccessor->CreateAccessor(
				DBACCESSOR_ROWDATA,	//dwAccessorFlags
				*pcBindings,		//cBindings
				*prgBindings,		//rgBindings
				0,					//cbRowSize
				phAccessor,			//phAccessor	
				NULL				//rgStatus
				));

CLEANUP:
	if( pIAccessor )
		pIAccessor->Release();
	return hr;    
}

int OLEDb::setupBindings(IUnknown *pUnkRowset, DBORDINAL *pcBindings, DBBINDING **prgBindings, DBORDINAL *pcbRowSize)
{
	HRESULT					hr = E_FAIL;
	DBORDINAL				cColumns;
	DBCOLUMNINFO *			rgColumnInfo				= NULL;
	LPWSTR					pStringBuffer				= NULL;
	IColumnsInfo *			pIColumnsInfo				= NULL;

	ULONG					iCol;
	DBORDINAL				dwOffset					= 0;
	DBBINDING *				rgBindings					= NULL;
	
	ULONG					cStorageObjs				= 0;
	BOOL					fMultipleObjs				= FALSE;

    hr = pUnkRowset->QueryInterface(IID_IColumnsInfo, (void**)&pIColumnsInfo);
    
	hr = pIColumnsInfo->GetColumnInfo(
		&cColumns,		//pcColumns
		&rgColumnInfo,	//prgColumnInfo
		&pStringBuffer	//ppStringBuffer
	);
                
    rgBindings = (DBBINDING*)CoTaskMemAlloc(cColumns * sizeof(DBBINDING));
	memset(rgBindings, 0, cColumns * sizeof(DBBINDING));
    
    getProperty(pUnkRowset, IID_IRowset, DBPROP_MULTIPLESTORAGEOBJECTS, 
		DBPROPSET_ROWSET, &fMultipleObjs);
 
	for( iCol = 0; iCol < cColumns; iCol++ )
	{
		// This binding applies to the ordinal of this column
		rgBindings[iCol].iOrdinal	= rgColumnInfo[iCol].iOrdinal;

		// We are asking the provider to give us the data for this column
		// (DBPART_VALUE), the length of that data (DBPART_LENGTH), and
		// the status of the column (DBPART_STATUS)
		rgBindings[iCol].dwPart		= DBPART_VALUE|DBPART_LENGTH|DBPART_STATUS;

		// The following values are the offsets to the status, length, and
		// data value that the provider will fill with the appropriate values
		// when we fetch data later. When we fetch data, we will pass a
		// pointer to a buffer that the provider will copy column data to,
		// in accordance with the binding we have provided for that column;
		// these are offsets into that future buffer
		rgBindings[iCol].obStatus	= dwOffset;
		rgBindings[iCol].obLength	= dwOffset + sizeof(DBSTATUS);
		rgBindings[iCol].obValue	= dwOffset + sizeof(DBSTATUS) + sizeof(ULONG);
		
		// Any memory allocated for the data value will be owned by us, the
		// client. Note that no data will be allocated in this case, as the
		// DBTYPE_WSTR bindings we are using will tell the provider to simply
		// copy data directly into our provided buffer
		rgBindings[iCol].dwMemOwner	= DBMEMOWNER_CLIENTOWNED;

		// This is not a parameter binding
		rgBindings[iCol].eParamIO	= DBPARAMIO_NOTPARAM;
		
		// We want to use the precision and scale of the column
		rgBindings[iCol].bPrecision	= rgColumnInfo[iCol].bPrecision;
		rgBindings[iCol].bScale		= rgColumnInfo[iCol].bScale;

		// Bind this column as DBTYPE_WSTR, which tells the provider to
		// copy a Unicode string representation of the data into our buffer,
		// converting from the native type if necessary
		rgBindings[iCol].wType		= DBTYPE_WSTR;

		// Initially, we set the length for this data in our buffer to 0;
		// the correct value for this will be calculated directly below
		rgBindings[iCol].cbMaxLen	= 0;
						
		// Determine the maximum number of bytes required in our buffer to
		// contain the Unicode string representation of the provider's native
		// data type, including room for the NULL-termination character
		switch( rgColumnInfo[iCol].wType )
		{
			case DBTYPE_NULL:
			case DBTYPE_EMPTY:
			case DBTYPE_I1:
			case DBTYPE_I2:
			case DBTYPE_I4:
			case DBTYPE_UI1:
			case DBTYPE_UI2:
			case DBTYPE_UI4:
			case DBTYPE_R4:
			case DBTYPE_BOOL:
			case DBTYPE_I8:
			case DBTYPE_UI8:
			case DBTYPE_R8:
			case DBTYPE_CY:
			case DBTYPE_ERROR:
				// When the above types are converted to a string, they
				// will all fit into 25 characters, so use that plus space
				// for the NULL-terminator
				rgBindings[iCol].cbMaxLen = (25 + 1) * sizeof(WCHAR);
				break;

			case DBTYPE_DECIMAL:
			case DBTYPE_NUMERIC:
			case DBTYPE_DATE:
			case DBTYPE_DBDATE:
			case DBTYPE_DBTIMESTAMP:
			case DBTYPE_GUID:
				// Converted to a string, the above types will all fit into
				// 50 characters, so use that plus space for the terminator
				rgBindings[iCol].cbMaxLen = (50 + 1) * sizeof(WCHAR);
				break;
			
			case DBTYPE_BYTES:
				// In converting DBTYPE_BYTES to a string, each byte
				// becomes two characters (e.g. 0xFF -> "FF"), so we
				// will use double the maximum size of the column plus
				// include space for the NULL-terminator
				rgBindings[iCol].cbMaxLen =
					(rgColumnInfo[iCol].ulColumnSize * 2 + 1) * sizeof(WCHAR);
				break;

			case DBTYPE_STR:
			case DBTYPE_WSTR:
			case DBTYPE_BSTR:
				// Going from a string to our string representation,
				// we can just take the maximum size of the column,
				// a count of characters, and include space for the
				// terminator, which is not included in the column size
				rgBindings[iCol].cbMaxLen = 
					(rgColumnInfo[iCol].ulColumnSize + 1) * sizeof(WCHAR);
				break;

			default:
				// For any other type, we will simply use our maximum
				// column buffer size, since the display size of these
				// columns may be variable (e.g. DBTYPE_VARIANT) or
				// unknown (e.g. provider-specific types)
				rgBindings[iCol].cbMaxLen = MAX_COL_SIZE;
				break;
		};
		
		// If the provider's native data type for this column is
		// DBTYPE_IUNKNOWN or this is a BLOB column and the user
		// has requested that we bind BLOB columns as ISequentialStream
		// objects, bind this column as an ISequentialStream object if
		// the provider supports our creating another ISequentialStream
		// binding
		if( (rgColumnInfo[iCol].wType == DBTYPE_IUNKNOWN ||
			 ((rgColumnInfo[iCol].dwFlags & DBCOLUMNFLAGS_ISLONG) &&
			  (g_dwFlags & USE_ISEQSTREAM))) &&
			(fMultipleObjs || !cStorageObjs) )
		{
			// To create an ISequentialStream object, we will
			// bind this column as DBTYPE_IUNKNOWN to indicate
			// that we are requesting this column as an object
			rgBindings[iCol].wType				= DBTYPE_IUNKNOWN;

			// We want to allocate enough space in our buffer for
			// the ISequentialStream pointer we will obtain from
			// the provider
			rgBindings[iCol].cbMaxLen			= sizeof(ISequentialStream *);

			// To specify the type of object that we want from the
			// provider, we need to create a DBOBJECT structure and
			// place it in our binding for this column
		    rgBindings[iCol].pObject			= 
								(DBOBJECT *)CoTaskMemAlloc(sizeof(DBOBJECT));
			CHECK_MEMORY(hr, rgBindings[iCol].pObject);

			// Direct the provider to create an ISequentialStream
			// object over the data for this column
			rgBindings[iCol].pObject->iid		= IID_ISequentialStream;

			// We want read access on the ISequentialStream
			// object that the provider will create for us
			rgBindings[iCol].pObject->dwFlags	= STGM_READ;

			// Keep track of the number of storage objects (ISequentialStream
			// is a storage interface) that we have requested, so that we
			// can avoid requesting multiple storage objects from a provider
			// that only supports a single storage object in our bindings
			cStorageObjs++;
		}	

		// Ensure that the bound maximum length is no more than the
		// maximum column size in bytes that we've defined
		rgBindings[iCol].cbMaxLen	
			= min(rgBindings[iCol].cbMaxLen, MAX_COL_SIZE);

		// Update the offset past the end of this column's data, so
		// that the next column will begin in the correct place in
		// the buffer
		dwOffset = rgBindings[iCol].cbMaxLen + rgBindings[iCol].obValue;
		
		// Ensure that the data for the next column will be correctly
		// aligned for all platforms, or, if we're done with columns,
		// that if we allocate space for multiple rows that the data
		// for every row is correctly aligned
		dwOffset = ROUNDUP(dwOffset);
	}

	// Return the row size (the current dwOffset is the size of the row),
	// the count of bindings, and the bindings array to the caller
	*pcbRowSize		= dwOffset;
	*pcBindings		= cColumns;
	*prgBindings	= rgBindings;


	CoTaskMemFree(rgColumnInfo);
	CoTaskMemFree(pStringBuffer);
	if( pIColumnsInfo )
		pIColumnsInfo->Release();
 
    return hr;
}

void OLEDb::FreeBindings
	(
	DBORDINAL				cBindings, 
	DBBINDING *				rgBindings
	)
{
	ULONG					iBind;

	// Free any memory used by DBOBJECT structures in the array
	for( iBind = 0; iBind < cBindings; iBind++ )
		CoTaskMemFree(rgBindings[iBind].pObject);

	// Now free the bindings array itself
	CoTaskMemFree(rgBindings);
}

int OLEDb::updateDisplaySize
	(
	DBORDINAL				cBindings, 
	DBBINDING *				rgBindings, 
	void *					pData, 
	ULONG *					rgDispSize
	)
{
	DBSTATUS				dwStatus;
	ULONG					cchLength;
	ULONG					iCol;

	// Loop through the bindings, comparing the size of each column
	// against the previously found maximum size for that column
	for( iCol = 0; iCol < cBindings; iCol++ )
	{
		dwStatus  = *(DBSTATUS *)((BYTE *)pData + rgBindings[iCol].obStatus);
		cchLength = ((*(ULONG *)((BYTE *)pData + rgBindings[iCol].obLength))
					/ sizeof(WCHAR));

		// The length that we need to display depends on the status
		// of this column and generally on the data in the column
		switch( dwStatus )
		{
			case DBSTATUS_S_ISNULL:
				cchLength = 6;						// "(null)"
				break;

			case DBSTATUS_S_TRUNCATED:
			case DBSTATUS_S_OK:
			case DBSTATUS_S_DEFAULT:
				if( rgBindings[iCol].wType == DBTYPE_IUNKNOWN )
					cchLength = 2 + 8;				// "0x%08lx"
				
				// Ensure that the length is at least the minimum display size
				cchLength = max(cchLength, MIN_DISPLAY_SIZE);
				break;

			default:
				cchLength = 14;						// "(error status)"
				break;
		}

		if( rgDispSize[iCol] < cchLength )
			rgDispSize[iCol] = cchLength;
	}

	return S_OK;
}

int DisplayRow
(
	ULONG					iRow, 
	DBORDINAL				cBindings, 
	DBBINDING *				rgBindings, 
	void *					pData, 
	ULONG *					rgDispSize
)
{
	HRESULT					hr							= S_OK;
	WCHAR					wszColumn[MAX_DISPLAY_SIZE + 1];
	DBSTATUS				dwStatus;
	ULONG					ulLength;
	void *					pvValue;
	ULONG					iCol;
	ULONG					cbRead;
	ISequentialStream *		pISeqStream					= NULL;
	size_t					cSpaces;
	ULONG					iSpace;
	
	// Display the row number
	//wprintf(L" [%d] | ", iRow);

	// For each column that we have bound, display the data
	for( iCol = 0; iCol < cBindings; iCol++ )
	{
		// We have bound status, length, and the data value for all
		// columns, so we know that these can all be used
		dwStatus	= *(DBSTATUS *)((BYTE *)pData + rgBindings[iCol].obStatus);
		ulLength	= *(ULONG *)((BYTE *)pData + rgBindings[iCol].obLength);
		pvValue		= (BYTE *)pData + rgBindings[iCol].obValue;

		// Check the status of this column. This decides
		// exactly what will be displayed for the column
		switch( dwStatus )
		{
			// The data is NULL, so don't try to display it
			case DBSTATUS_S_ISNULL:
				wcscpy_s(wszColumn, _countof(wszColumn), L"(null)");
				break;

			// The data was fetched, but may have been truncated.
			// Display string data for this column to the user
			case DBSTATUS_S_TRUNCATED:
			case DBSTATUS_S_OK:
			case DBSTATUS_S_DEFAULT:
			{
				// We have either bound the column as a Unicode string
				// (DBTYPE_WSTR) or as an ISequentialStream object
				// (DBTYPE_IUNKNOWN), and have to do different processing
				// for each one of these possibilities
				switch( rgBindings[iCol].wType )
				{
					case DBTYPE_WSTR:
					{	
						// Copy the string data
						wcsncpy_s(wszColumn, _countof(wszColumn),(WCHAR *)pvValue, MAX_DISPLAY_SIZE);
						wszColumn[MAX_DISPLAY_SIZE - 1] = L'\0';
						break;
					}

					case DBTYPE_IUNKNOWN:
					{
						// We've bound this as an ISequentialStream object,
						// therefore the data in our buffer is a pointer
						// to the object's ISequentialStream interface
						pISeqStream =  *(ISequentialStream**)pvValue;
						
						// We call ISequentialStream::Read to read bytes from
						// the stream blindly into our buffer, simply as a
						// demonstration of ISequentialStream. To display the
						// data properly, the native provider type of this
						// column should be accounted for; it could be
						// DBTYPE_WSTR, in which case this works, or it could
						// be DBTYPE_STR or DBTYPE_BYTES, in which case this
						// won't display the data correctly
						CHECK_HR(hr = pISeqStream->Read(
									wszColumn,			//pBuffer
									MAX_DISPLAY_SIZE,	//cBytes
									&cbRead				//pcBytesRead
									));
						
						// Since streams don't provide NULL-termination,
						// we'll NULL-terminate the resulting string ourselves
						wszColumn[cbRead / sizeof(WCHAR)] = L'\0';

						// Release the stream object, now that we're done
						pISeqStream->Release();
						pISeqStream = NULL;
						break;
					}
				}
				break;
			}

			// This is an error status, so don't try to display the data
			default:
				wcscpy_s(wszColumn, _countof(wszColumn), L"(error status)");
				break;
		}

		// Determine how many spaces we need to add after displaying this
		// data to align it with this column in other rows
		cSpaces = min(rgDispSize[iCol], MAX_DISPLAY_SIZE) - wcslen(wszColumn);

		// Print the column data
		//wprintf(L"%s", wszColumn);

		// Now print any spaces necessary
		// for(iSpace = 0; iSpace < cSpaces; iSpace++ )
			// _putch(' ');

		// Now end the column with a separator marker if necessary
		// if( iCol < cBindings - 1 )
			// wprintf(L" | ");
	}
	
CLEANUP:
	if( pISeqStream )
		pISeqStream->Release();

	// Print the row separator
	//wprintf(L"\n");
	return hr;
    
}

int OLEDb::GetProperty
(
	IUnknown *				pIUnknown, 
	REFIID					riid, 
	DBPROPID				dwPropertyID, 
	REFGUID					guidPropertySet, 
	BOOL *					pbValue
)
{
	HRESULT					hr;
	DBPROPID				rgPropertyIDs[1];
	DBPROPIDSET				rgPropertyIDSets[1];
	
	ULONG					cPropSets					= 0;
	DBPROPSET *				rgPropSets					= NULL;

	IDBProperties *			pIDBProperties				= NULL;
	ISessionProperties *	pISesProps					= NULL;
	ICommandProperties *	pICmdProps					= NULL;
	IRowsetInfo *			pIRowsetInfo				= NULL;

	// Initialize the output value
	*pbValue = FALSE;

	// Set up the property ID array
	rgPropertyIDs[0] = dwPropertyID;
	
	// Set up the Property ID Set
	rgPropertyIDSets[0].rgPropertyIDs	= rgPropertyIDs;
	rgPropertyIDSets[0].cPropertyIDs	= 1;
	rgPropertyIDSets[0].guidPropertySet	= guidPropertySet;

	// Get the property value for this property from the provider, but
	// don't try to display extended error information, since this may
	// not be a supported property: a failure is, in fact, expected if
	// the property is not supported
	if( riid == IID_IDBProperties )
	{
		XCHECK_HR(hr = pIUnknown->QueryInterface(IID_IDBProperties, 
					(void**)&pIDBProperties));
		CHECK_HR(hr = pIDBProperties->GetProperties(
					1,					//cPropertyIDSets
					rgPropertyIDSets,	//rgPropertyIDSets
					&cPropSets,			//pcPropSets
					&rgPropSets			//prgPropSets
					));
	}
	else if( riid == IID_ISessionProperties )
	{
		XCHECK_HR(hr = pIUnknown->QueryInterface(IID_ISessionProperties, 
					(void**)&pISesProps));
		CHECK_HR(hr = pISesProps->GetProperties(
					1,					//cPropertyIDSets
					rgPropertyIDSets,	//rgPropertyIDSets
					&cPropSets,			//pcPropSets
					&rgPropSets			//prgPropSets
					));
	}
	else if( riid == IID_ICommandProperties )
	{
		XCHECK_HR(hr = pIUnknown->QueryInterface(IID_ICommandProperties, 
					(void**)&pICmdProps));

		CHECK_HR(hr = pICmdProps->GetProperties(
					1,					//cPropertyIDSets
					rgPropertyIDSets,	//rgPropertyIDSets
					&cPropSets,			//pcPropSets
					&rgPropSets			//prgPropSets
					));
	}
	else
	{
		XCHECK_HR(hr = pIUnknown->QueryInterface(IID_IRowsetInfo, 
					(void**)&pIRowsetInfo));

		CHECK_HR(hr = pIRowsetInfo->GetProperties(
					1,					//cPropertyIDSets
					rgPropertyIDSets,	//rgPropertyIDSets
					&cPropSets,			//pcPropSets
					&rgPropSets			//prgPropSets
					));
	}

	// Return the value for this property to the caller if
	// it's a VT_BOOL type value, as expected
	if( V_VT(&rgPropSets[0].rgProperties[0].vValue) == VT_BOOL )
		*pbValue = V_BOOL(&rgPropSets[0].rgProperties[0].vValue);

CLEANUP:
	if( rgPropSets )
	{
		CoTaskMemFree(rgPropSets[0].rgProperties);
		CoTaskMemFree(rgPropSets);
	}
	if( pIDBProperties )
		pIDBProperties->Release();
	if( pISesProps )
		pISesProps->Release();
	if( pICmdProps )
		pICmdProps->Release();
	if( pIRowsetInfo )
		pIRowsetInfo->Release();
	return hr;
        
}
