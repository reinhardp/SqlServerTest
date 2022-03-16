#include "Table.h"
#include <windows.h>
#include <Objbase.h>
#include <commctrl.h>
//#include <oledb.h>
//#include <msoledbsql.h>
#include <strsafe.h>
#include "includes.h"

int Table::Init(HWND hWnd, HINSTANCE hinst) 
{
    // TODO: Fügen Sie hier Ihren Implementierungscode ein..
    m_hWnd = hWnd;
    m_hinst = hinst;

    return 0;
}

bool Table::ResizeTable(HWND hWndstatus, HWND mainhWnd, int cParts = 1) {
    //m_hwndList
    RECT rc, rcMain;
    BOOL ret = FALSE;
    ret = GetClientRect(mainhWnd, &rcMain);
    ret = GetClientRect(m_hwndList, &rc);

    WCHAR statusText[150];
    if ((rcMain.bottom - rcMain.top) > 250 ) {
        ret = SetWindowPos(m_hwndList, /*NULL*/ HWND_BOTTOM, m_x, m_y,
            rcMain.right - rcMain.left - 200,
            rcMain.bottom - rcMain.top - 200,
            SWP_SHOWWINDOW);
    }

#ifdef _DEBUG

    bool isg = ((rcMain.bottom - rcMain.top - 0) > 250);
    StringCchPrintf(statusText, 150, L"Table width: %d, table height: %d. MainWindow width %d, height %d. (%d) (%d)", (rc.right - rc.left - 0), (rc.bottom - rc.top - 0), (rcMain.right - rcMain.left - 0), (rcMain.bottom - rcMain.top - 0), (rcMain.bottom - rcMain.top - 0), isg);
    SendMessage(hWndstatus, SB_SETTEXT, (WPARAM)0 | (cParts - 1), (LPARAM)statusText);

#endif // _DEBUG

    return ret;
}

bool Table::CreateTable()
{
    // TODO: Fügen Sie hier Ihren Implementierungscode ein..
    INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);
    RECT rcClient;                       // The parent window's client area.
    int x = 0, y = 0;

    if (m_x > 0) {
        x = m_x;
    }
    else {
        x = 20;
        m_x = x;
    }
    if (m_y > 0) {
        y = m_y;
    }
    else {
        y = 50;
        m_y = y;
    }

    GetClientRect(m_hWnd, &rcClient);

    m_hwndList = CreateWindowW(WC_LISTVIEW, L"",
        WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT,
        x, y,
        rcClient.right - rcClient.left - 280,
        rcClient.bottom - rcClient.top - 200,
        m_hWnd,
        0,
        m_hinst, NULL);

    ListView_SetExtendedListViewStyle(m_hwndList, LVS_EX_FULLROWSELECT | LVS_EX_AUTOSIZECOLUMNS);

    return true;
}

bool Table::CreateHeader(void) {

    LV_COLUMN LvCol;

    memset(&LvCol, 0, sizeof(LvCol));
    LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;    // Type of mask
    LvCol.fmt = LVCFMT_CENTER;
    LvCol.cx = 0x50;
    LvCol.cchTextMax = 255;


    for (int i = 0; i < sizeof(tHeader) / sizeof(tHeader[0]); i++) {
        LvCol.pszText = (LPWSTR)&tHeader[i];
        if (i == 3) {
            LvCol.cx = 0x95;
        }
        else {
            LvCol.cx = 0x50;
        }
        ListView_InsertColumn(m_hwndList, i, &LvCol);
        }

    return true;
}

bool Table::CreateBody(void) {

    /*
    * only for test!
    */
    int index = 0;
    LVITEM lvI;
    lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
    lvI.mask = LVIF_TEXT;
    lvI.stateMask = 0;
    lvI.iSubItem = 0;
    lvI.state = 0;

    int cItems = sizeof(rgtBody) / sizeof(rgtBody[0]);

    m_Ids = new int[cItems]{0};


    for (index; index < cItems; index++)
    {
        lvI.iItem = index;
        m_Ids[index] = rgtBody[index].ID;
        
        // Insert items into the list.
        if (ListView_InsertItem(m_hwndList, &lvI) == -1)
            return false;

        SendMessage(m_hwndList, LVM_SETCOLUMNWIDTH, index, LVSCW_AUTOSIZE_USEHEADER);
        ListView_MapIndexToID(m_hwndList, index);

    }

    return true;
}

bool Table::CreateGridTable() 
{

    INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);
    RECT rcClient;                       // The parent window's client area.
    int x = 0, y = 0;

    if (m_x > 0) {
        x = m_x;
    }
    else {
        x = 20;
        m_x = x;
    }
    if (m_y > 0) {
        y = m_y;
    }
    else {
        y = 50;
        m_y = y;
    }

    GetClientRect(m_hWnd, &rcClient);

    return true;
}

bool Table::CreateGridHeader() {
    return true;
}
bool Table::CreateGridBody() {
    return true;
}

/*
*  method to create the table body from the MS SQL database
* 
* @param none
* @return bool
*/
bool Table::CreateOLeDBBody() {
    bool ret = true;

    //StringCchPrintfW(m_QueryString, SQL_QUERYSTRING_LENGTH, L"SELECT id, firstname, lastname, birthdy, dtext, created_by, created_at FROM  %s.dbo.%s", m_dbName, m_table);

    return ret;
}

bool Table::recalculateColumnwidth() {
    int index = 0;
    int cItems = sizeof(rgtBody) / sizeof(rgtBody[0]);
    for (index; index < cItems; index++)
    {
        SendMessage(m_hwndList, LVM_SETCOLUMNWIDTH, index, LVSCW_AUTOSIZE_USEHEADER);
    }
    return true;
}

bool Table::CreateMySQLDBBody() {
    return true;
}
/*
* method to crete the body from MySQL database
*/
bool Table::CreateMySQLDBBody(int rows) {

    int index = 0;
    LVITEM lvI;
    lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
    lvI.mask = LVIF_TEXT;
    lvI.stateMask = 0;
    lvI.iSubItem = 0;
    lvI.state = 0;
    m_Ids = new int[rows] {0};

    for (index; index < rows; index++)
    {
        lvI.iItem = index;
        m_Ids[index] = rgtBody[index].ID;   // ??
        // Insert items into the list.
        if (ListView_InsertItem(m_hwndList, &lvI) == -1)
            return false;

        SendMessage(m_hwndList, LVM_SETCOLUMNWIDTH, index, LVSCW_AUTOSIZE_USEHEADER);
        ListView_MapIndexToID(m_hwndList, index);

    }


    return true;
}