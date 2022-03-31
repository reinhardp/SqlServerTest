#include "Table.h"
#include <windows.h>
#include <Objbase.h>
#include <stdlib.h>
#include <commctrl.h>
//#include <oledb.h>
//#include <msoledbsql.h>
#include <strsafe.h>

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

bool Table::CreateTable() {
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
    int cItems = sizeof(g_Body);
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
* method to create the body from MySQL database
*/
bool Table::CreateMySQLDBBody(int rows, struct DATA *table) {

    WCHAR wc[30];
    WCHAR wcb[11];
    WCHAR wcc[24];
    WCHAR wcd[DTEXT_LENGTH];
    size_t retval;
    int row = 0;
    m_Ids = new int32_t[rows] {0};

    while (m_res->next())
    {
        m_Ids[row] = m_res->getInt("id");   // the ID from the database
        
        sql::SQLString c = m_res->getString("firstname");
        const char* y = c.c_str();
        mbstowcs_s(&retval, wc, 30, y, strlen(y));
        StringCbPrintfW(table[row].firstname, retval * sizeof(wchar_t), L"%s", wc);

        c = m_res->getString("lastname");
        y = c.c_str();
        mbstowcs_s(&retval, wc, 30, y, strlen(y));
        StringCbPrintfW(table[row].lastname, retval * sizeof(wchar_t), L"%s", wc);
        
        c = m_res->getString("birthday");
        y = c.c_str();
        mbstowcs_s(&retval, wcb, 11, y, strlen(y));
        StringCbPrintf(table[row].birthday, retval * sizeof(wchar_t), L"%s", wcb);

        c = m_res->getString("dtext");
        y = c.c_str();
        mbstowcs_s(&retval, wcd, DTEXT_LENGTH, y, strlen(y));
        StringCbPrintfW(table[row].dtext, DTEXT_LENGTH, L"%s", wcd);

        c = m_res->getString("created_at");
        y = c.c_str();
        mbstowcs_s(&retval, wcc, 24, y, strlen(y));
        StringCbPrintf(table[row].created_at, retval * sizeof(wchar_t), L"%s", wcc);

        c = m_res->getString("created_by");
        y = c.c_str();
        mbstowcs_s(&retval, wc, 30, y, strlen(y));
        StringCbPrintfW(table[row].created_by, retval * sizeof(wchar_t), L"%s", wc);

        row++;

    }


    return true;
}