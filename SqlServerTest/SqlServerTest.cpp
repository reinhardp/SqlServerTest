// SqlServerTest.cpp : Definiert den Einstiegspunkt für die Anwendung.
//

#include "framework.h"
#include "SqlServerTest.h"
#include "Calculation.h"
//#include "OLEDb.h"
#include <stdio.h>
#include <math.h>
#include "commctrl.h"
#include <strsafe.h>
#include <userenv.h>
#include "defines.h"
#include "includes.h"
#include <string.h>





// Globale Variablen: (implementiert in SqlServerTest.h)
DATA g_data;

// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Calculate(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    EditNewRow(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    EditDBSettings(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Hier Code einfügen.

    // Globale Zeichenfolgen initialisieren
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SQLSERVERTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Anwendungsinitialisierung ausführen:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SQLSERVERTEST));

    MSG msg;

    // Hauptnachrichtenschleife:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNKTION: MyRegisterClass()
//
//  ZWECK: Registriert die Fensterklasse.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DBICON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SQLSERVERTEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_DBICON));

    return RegisterClassExW(&wcex);
}

//
//   FUNKTION: InitInstance(HINSTANCE, int)
//
//   ZWECK: Speichert das Instanzenhandle und erstellt das Hauptfenster.
//
//   KOMMENTARE:
//
//        In dieser Funktion wird das Instanzenhandle in einer globalen Variablen gespeichert, und das
//        Hauptprogrammfenster wird erstellt und angezeigt.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   

   if (!g_hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(g_hWnd, nCmdShow);

   UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  FUNKTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ZWECK: Verarbeitet Meldungen für das Hauptfenster.
//
//  WM_COMMAND  - Verarbeiten des Anwendungsmenüs
//  WM_PAINT    - Darstellen des Hauptfensters
//  WM_DESTROY  - Ausgeben einer Beendenmeldung und zurückkehren
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    NMLVDISPINFO* plvdi;
    bool retcode = false;

    switch (message)
    {
    case WM_CREATE:
    {

        g_hdc = GetDC(hWnd);

        g_statusindow = DoCreateStatusBar(hWnd, hInst, g_cParts);
        if (g_statusindow != NULL) {
            SendMessage(g_statusindow, SB_SETTEXT, (WPARAM)0 | (g_cParts - 1), (LPARAM)L"App erstellt");
        }
            
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Menüauswahl analysieren:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case ID_DATEI_BERECHNEN:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_CALCULATIONBOX), hWnd, Calculate);
            break;
        case ID_EINSTELLUNGEN_DBSETTINGS:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DBSETTINGS), hWnd, EditDBSettings);
            break;
        case ID_DATEI_MSSQLSERVERDIALOG:
        {
            g_updaterow = FALSE;
            DialogBox(hInst, MAKEINTRESOURCE(IDD_EDITNEWROW), hWnd, EditNewRow);
            break;

        }
        case ID_CREATETABLE:
        {
            if (g_tableCreated == false) {
                readINIFile();
                g_tableClass = new Table(hWnd, hInst);
                g_tableClass->init(g_host, g_port, g_dbName, g_user, g_pwd);
                retcode = g_tableClass->connect();
                if (retcode == false) 
                    return false;

                g_rows = g_tableClass->getNumberOfRows();

                g_tableClass->CreateTable();
                g_tableClass->CreateHeader();
                g_tableClass->select();
                //g_tableClass->CreateBody();
                g_Body = new DATA[g_rows]{0};
                g_tableClass->CreateMySQLDBBody(g_rows, g_Body);
                g_Ids = g_tableClass->GetIds();
                g_hwndList = g_tableClass->getlistHandle();
                createTable();
                g_tableCreated = true;
                
                SendMessage(g_statusindow, SB_SETTEXT, (WPARAM)0 | (g_cParts - 1), (LPARAM)L"Tabelle erstellt!");
            }
            else {
                SendMessage(g_statusindow, SB_SETTEXT, (WPARAM)0 | (g_cParts - 1), (LPARAM)L"Tabelle wurde bereits erstellt!");
                MessageBox(NULL, L"Die Tabelle wurde bereits erstellt!", L"Tabelle erstellen", MB_OK | MB_ICONINFORMATION);
            }
            break;
        }
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        g_hdc = BeginPaint(hWnd, &ps);
        // TODO: Zeichencode, der hdc verwendet, hier einfügen...
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_SIZE:
    {
        RECT rc;
        GetClientRect(hWnd, &rc);
        SetWindowPos(g_statusindow, NULL, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER | SWP_NOACTIVATE);
        if (g_tableClass != NULL) {
            g_tableClass->ResizeTable(g_statusindow, hWnd, g_cParts);
        }
        
        break;
    }
    case WM_DESTROY:
    {

/*        if (g_Ids) {
            delete[] g_Ids;
            g_Ids = NULL;
        }
*/
        if (g_tableClass) {
            delete g_tableClass;
            g_tableClass = NULL;
        }

        PostQuitMessage(0);
        break;
    }
    case WM_NOTIFY:
    {
        switch (((LPNMHDR)lParam)->code)
        {
            case NM_DBLCLK:
            {
                LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;
                g_selectedId = g_Ids[lpnmitem->iItem]; // the selected ID from the Table
                //g_data = rgtBody[g_selectedId - 1];
                g_data = g_Body[lpnmitem->iItem];
                g_updaterow = TRUE;
                DialogBox(hInst, MAKEINTRESOURCE(IDD_EDITNEWROW), hWnd, EditNewRow);
                break;
            }
            case LVN_GETDISPINFO:
            {
                plvdi = (NMLVDISPINFO*)lParam;

                switch (plvdi->item.iSubItem)
                {
                case 0:
                {
                    StringCchPrintf(plvdi->item.pszText, 30, L"%s", g_Body[plvdi->item.iItem].firstname); 
                    //plvdi->item.pszText = (LPWSTR)g_rowdata[plvdi->item.iItem];
                    //plvdi->item.pszText = (LPWSTR)g_rowdata[1];
                    break;
                }
                case 1:
                {
                    
                    //StringCchPrintf(plvdi->item.pszText, 30, L"%s", rgtBody[plvdi->item.iItem].lastname); // fr "static" Werte!

                    StringCchPrintf(plvdi->item.pszText, 30, L"%s", g_Body[plvdi->item.iItem].lastname); 
                    //plvdi->item.pszText = (LPWSTR)g_rowdata[plvdi->item.iItem];
                    //plvdi->item.pszText = (LPWSTR)g_rowdata[2];
                    break;
                }
                case 2:
                {
                    
                    //StringCchPrintf(plvdi->item.pszText, 30, L"%s", rgtBody[plvdi->item.iItem].birthday); // fr "static" Werte!
                    StringCchPrintf(plvdi->item.pszText, 11, L"%s", g_Body[plvdi->item.iItem].birthday); 
                    //plvdi->item.pszText = (LPWSTR)g_rowdata[plvdi->item.iItem];
                    //plvdi->item.pszText = (LPWSTR)g_rowdata[3];
                    break;
                }
                case 3:
                {
                    
                    //StringCchPrintf(plvdi->item.pszText, 8000, L"%s", rgtBody[plvdi->item.iItem].dtext); // fr "static" Werte!
                    StringCchPrintf(plvdi->item.pszText, DTEXT_LENGTH, L"%s", g_Body[plvdi->item.iItem].dtext);
                    //plvdi->item.pszText = (LPWSTR)g_rowdata[plvdi->item.iItem];
                    //plvdi->item.pszText = (LPWSTR)g_rowdata[4];
                    break;
                }
                case 4:
                {
                    //StringCchPrintf(plvdi->item.pszText, 30, L"%s", rgtBody[plvdi->item.iItem].created_by); // fr "static" Werte!
                    StringCchPrintf(plvdi->item.pszText, 30, L"%s", g_Body[plvdi->item.iItem].created_by);

                    //plvdi->item.pszText = (LPWSTR)g_rowdata[plvdi->item.iItem];
                    //plvdi->item.pszText = (LPWSTR)g_rowdata[5];
                    break;
                }
                case 5:
                {
                    //StringCchPrintf(plvdi->item.pszText, 30, L"%s", rgtBody[plvdi->item.iItem].created_at); // fr "static" Werte!
                    StringCchPrintf(plvdi->item.pszText, 24, L"%s", g_Body[plvdi->item.iItem].created_at);

                    //plvdi->item.pszText = (LPWSTR)g_rowdata[plvdi->item.iItem];
                    //plvdi->item.pszText = (LPWSTR)g_rowdata[6];
                    break;
                }

                }
            }

        }

    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Meldungshandler für Infofeld.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK EditDBSettings(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    
    UINT uRetcode = 0;
    WCHAR pwd[MAX_INI_ENTRIES_LENGTH] = { '\0' };
    WCHAR pwdrepeated[MAX_INI_ENTRIES_LENGTH] = { '\0' };

    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
    {
        readINIFile();
        if(g_host[0] == '\0') {
            DWORD len = MAX_INI_ENTRIES_LENGTH;
            GetComputerNameW(g_host, &len);
        }
        SetDlgItemTextW(hDlg, IDC_HOSTNAME, g_host);
        SetDlgItemTextW(hDlg, IDC_DBNAME, g_dbName);
        SetDlgItemTextW(hDlg, IDC_DRIVER, g_driver);
        SetDlgItemTextW(hDlg, IDC_USERNAME, g_user);
        SetDlgItemTextW(hDlg, IDC_TABLENAME, g_table);
        SetDlgItemTextW(hDlg, IDC_PORT, g_port);

        SetDlgItemTextW(hDlg, IDC_PASSWORD, g_pwd);
        SetDlgItemTextW(hDlg, IDC_PASSWORD2, g_pwd);

        SendDlgItemMessage(hDlg, IDC_HOSTNAME, WM_SETFOCUS, 0, 0);
       
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDOK) {
            // get values from controls, check passwords equal,
            // and savethem back to Inifile
            
            uRetcode = GetDlgItemTextW(hDlg, IDC_PASSWORD, pwd, MAX_INI_ENTRIES_LENGTH);
            uRetcode = GetDlgItemTextW(hDlg, IDC_PASSWORD2, pwdrepeated, MAX_INI_ENTRIES_LENGTH);
            if (pwd[0] == '\0' || pwdrepeated[0] == '\0') {
                MessageBox(NULL, L"Kein(e) Passwo(ö)rt(er) angegeben !\n", L"Fehler bei den Passwörtern", MB_OK | MB_ICONERROR);
            }
            else if (wcscmp(pwd, pwdrepeated) == 0) {

                StringCbCopyW(g_pwd, MAX_INI_ENTRIES_LENGTH, pwd);  // copy 
                // get all other values from the dialog
                uRetcode = GetDlgItemTextW(hDlg, IDC_HOSTNAME, g_host, MAX_INI_ENTRIES_LENGTH);
                uRetcode = GetDlgItemTextW(hDlg, IDC_DBNAME, g_dbName, MAX_INI_ENTRIES_LENGTH);
                uRetcode = GetDlgItemTextW(hDlg, IDC_DRIVER, g_driver, MAX_INI_ENTRIES_LENGTH);
                uRetcode = GetDlgItemTextW(hDlg, IDC_USERNAME, g_user, MAX_INI_ENTRIES_LENGTH);
                uRetcode = GetDlgItemTextW(hDlg, IDC_TABLENAME, g_table, MAX_INI_ENTRIES_LENGTH);
                uRetcode = GetDlgItemTextW(hDlg, IDC_PORT, g_port, MAX_INI_ENTRIES_LENGTH);

                BOOL ret = writeINIFile();
                if (!ret) {
                    MessageBox(NULL, L"Die Datenbank Einstellungen konnten nicht geschrieben werden!", L"Datenbank Einstellungen:", MB_OK | MB_ICONERROR);
                }
                // end dialog
                EndDialog(hDlg, LOWORD(wParam));
            }
            else {
                MessageBox(NULL, L"Die Passwörter sind nicht identisch!\n", L"Fehler bei den Passwörtern", MB_OK | MB_ICONERROR);
            }


            return (INT_PTR)TRUE;
        }
        // gat all values from the controls and save them to the 
        return (INT_PTR)FALSE;
    }
    }
    return (INT_PTR)FALSE;

}

INT_PTR CALLBACK EditNewRow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    UNREFERENCED_PARAMETER(lParam);
    wchar_t firstname[30] = { '\0' };
    wchar_t lastname[30] = { '\0' };
    wchar_t birthday[11] = { '\0' };
    wchar_t dtext[8000] = { '\0' };

    HWND hCtrl = NULL;
    SYSTEMTIME st = {0};
    bool retcode;
    wchar_t wErrStr[512] = {0};
    size_t retval;

    switch (message)
    {
        case WM_INITDIALOG:
        {
            //Update Date-Time Picker with current date
            // 
            // if the dialog is used to edit (g_updaterow is TRUE) one row update all controls
            // with the data from the database
            //...

            

            if (g_updaterow == TRUE) { // Edit existing row
                SetWindowTextW(hDlg, L"Eintrag bearbeiten");
                SetDlgItemTextW(hDlg, IDC_FIRSTNAME, g_data.firstname);
                SetDlgItemTextW(hDlg, IDC_LASTNAME, g_data.lastname);
                
                hCtrl = GetDlgItem(hDlg, IDC_BIRTHDAY);
                int iy = 0, im = 0, id = 0;

                StringCchPrintf(birthday, 11, L"%s",g_data.birthday);

                st.wHour = st.wMinute = st.wSecond = st.wMilliseconds = 0;
                swscanf_s(birthday, L"%04d-%02d-%02d", &iy, &im, &id );

                st.wYear = (WORD)iy;
                st.wMonth = (WORD)im;
                st.wDay = (WORD)id;

                st.wHour = st.wMinute = st.wSecond = st.wMilliseconds = 0;
                DateTime_SetSystemtime(hCtrl, GDT_VALID, &st);
                
                
                SetDlgItemTextW(hDlg, IDC_DESCRIPTION, g_data.dtext);


            } else { // Create a new row
                SetWindowTextW(hDlg, L"Neuer Eintrag");
            }

            SendDlgItemMessage(hDlg, IDC_FIRSTNAME, WM_SETFOCUS, 0, 0);

            return (INT_PTR)TRUE;
        }
        case WM_COMMAND:
        {
            if (LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            if (LOWORD(wParam) == IDOK) {


                if (g_updaterow == TRUE) { // Edit existing row

                    GetDlgItemTextW(hDlg, IDC_FIRSTNAME, firstname, 30);

                    GetDlgItemTextW(hDlg, IDC_LASTNAME, lastname, 30);
                  
                    hCtrl = GetDlgItem(hDlg, IDC_BIRTHDAY);

                    DateTime_GetSystemtime(hCtrl, &st);
                    StringCchPrintf(birthday, 11, L"%04d-%02d-%02d", (signed int)st.wYear, (signed int)st.wMonth, (signed int)st.wDay);

                    GetDlgItemTextW(hDlg, IDC_DESCRIPTION, dtext, 8000);

                    if (g_tableClass != NULL) {
                        retcode = g_tableClass->update(g_selectedId, firstname, lastname, birthday, dtext);
                        if (retcode == false) {
                            g_tableClass->getErrString(wErrStr);
                            
                            MessageBox(NULL, wErrStr, L"Update", MB_OK| MB_ICONERROR);
                            //return (INT_PTR)TRUE;
                        }

                        g_rows = g_tableClass->getNumberOfRows();
                        if (g_rows == 0) {
                            if (g_tableClass->hasError() == true) {
                                g_tableClass->getErrString(wErrStr);
                                
                                MessageBox(NULL, wErrStr, L"getNumberOfRows", MB_OK| MB_ICONERROR);
                                //return (INT_PTR)TRUE;
                            }
                        }
                        delete[] g_Body;
                        g_Body = new DATA[g_rows]{ 0 };

                        g_Ids = g_tableClass->GetIds();
                        retcode = g_tableClass->select();
                        if (retcode == false) {
                            g_tableClass->getErrString(wErrStr);

                            MessageBox(NULL, wErrStr, L"Update table", MB_OK | MB_ICONERROR);
                            return (INT_PTR)TRUE;
                        }

                        g_tableClass->CreateMySQLDBBody(g_rows, g_Body);

                        g_hwndList = g_tableClass->getlistHandle();
                        createTable();

                    }

                }
                else { // neuer Eintrag!

                    GetDlgItemTextW(hDlg, IDC_FIRSTNAME, firstname, 30);

                    GetDlgItemTextW(hDlg, IDC_LASTNAME, lastname, 30);

                    hCtrl = GetDlgItem(hDlg, IDC_BIRTHDAY);
                    DateTime_GetSystemtime(hCtrl, &st);
                    StringCchPrintf(birthday, 11, L"%04d-%02d-%02d", (signed int)st.wYear, (signed int)st.wMonth, (signed int)st.wDay);

                    GetDlgItemTextW(hDlg, IDC_DESCRIPTION, dtext, 8000);

                    if (g_tableClass) {
                        
                        if(g_tableClass->isConnected()) {
                            g_tableClass->insert(firstname, lastname, birthday, dtext);
                        }
                        
                    }
                    else {

                        readINIFile();
                        g_tableClass = new Table(g_hWnd, hInst);
                        g_tableClass->init(g_host, g_port, g_dbName, g_user, g_pwd);
                        retcode = g_tableClass->connect();
                        if(retcode == false)
                            return (INT_PTR)FALSE;

                        retcode = g_tableClass->insert(firstname, lastname, birthday, dtext);
                        if (retcode == false)
                            return (INT_PTR)FALSE;
                        
                        g_tableClass->disconnect();
                    }
                        
                }

                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            // gat all values from the controls and save them to the 
            return (INT_PTR)FALSE;
        }
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Calculate(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    wchar_t bValue1[150];
    //memset(&bValue1, 0, sizeof(bValue1));
    wchar_t bValue2[150];
    wchar_t* stopstring;
    wchar_t find = ',';
    wchar_t *pdest;
    int position = 0;
    //memset(&bValue2, 0, sizeof(bValue2));
    LRESULT retcode;
    TCHAR lpOperator[2];
    double value1 = 0.00, value2 = 0.00;
    UINT uRetcode = 0;
    Calculation calc;
    double result = 0.00;
    TCHAR A[4];
    int  k = 0;
    memset(&A, 0, sizeof(A));

    switch (message)
    {
    case WM_INITDIALOG:
        // init Combobox
        for (k = 0; k <= 3; k += 1)
        {
            wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Operators[k]);

            // Add string to combobox.
            retcode = SendDlgItemMessage(hDlg, IDC_COMBO1, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
        }
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) // exit the dialog without todo anyhing
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDCALCULATE) // Calculate the values and send back to Dialog
        {
            
            uRetcode = GetDlgItemTextW(hDlg, IDC_EDIT1, bValue1, 150);
            if (uRetcode == 0)
                return (INT_PTR)FALSE;

            uRetcode = GetDlgItemTextW(hDlg, IDC_EDIT2, bValue2, 150);
            if(uRetcode == 0)
                return (INT_PTR)FALSE;
            /*
             * convert , to .:
            */
            pdest = wcschr(bValue1, find);
            position = (int)(pdest - bValue1);
            if (position > 0)
                bValue1[position] = '.';

            pdest = wcschr(bValue2, find);
            position = (int)(pdest - bValue2);
            if (position > 0)
                bValue2[position] = '.';
            //**** end convert
            
            value1 = wcstof(bValue1, &stopstring);
            value2 = wcstof(bValue2, &stopstring);

            UINT uOperator = GetDlgItemText(hDlg, IDC_COMBO1, lpOperator, 2);
            if(uOperator == 0)
                return (INT_PTR)FALSE;

            if (lpOperator[0] == '+') {
                result = calc.add(value1, value2);
            } else
            if (lpOperator[0] == '-') {
                result = calc.substract(value1, value2);
            } else
            if (lpOperator[0] == '*') {
                result = calc.multiply(value1, value2);
            } else
            if (lpOperator[0] == '/') {
                result = calc.devide(value1, value2);
            }
            else {  // if 
                result = 0.00;
            }
            

            result = Round(result, 4);
            char   buffer[1024];
            sprintf_s(buffer, "%.4f", result);

            // Set the Result in the Dialogbox
            SetDlgItemTextA(hDlg, IDC_CALCULATED, (LPCSTR)buffer);
        }
        return (INT_PTR)FALSE;
        break;
    }
    return (INT_PTR)FALSE;
}
/*

*/
double Round(double Zahl, int Stellen)
{
    return floor(Zahl * pow(10, Stellen) + 0.5) * pow(10, -Stellen);
}



/*
* Function to read the INI file to get database parameters
* if return code is false an message box popup and the
* 
*/
void readINIFile(void) {
    WCHAR iniFile[MAX_PATH] = {0};
    HANDLE hToken = NULL;
    DWORD len = MAX_PATH;
    DWORD dw;


    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        GetUserProfileDirectoryW(hToken, iniFile, &len);
        CloseHandle(hToken);
        
        StringCchCat(iniFile, NUMELEM(iniFile), L"/");
        StringCchCat(iniFile, NUMELEM(iniFile), CONFIG_DIRECTORY);
        if (GetFileAttributesW(iniFile) == INVALID_FILE_ATTRIBUTES) {
            CreateDirectoryW(iniFile, NULL);
            return;
        }

        StringCchCat(iniFile, NUMELEM(iniFile), L"/");
        StringCchCat(iniFile, NUMELEM(iniFile), CONFIG_FILE);

        dw = GetPrivateProfileString(DB_SECTION, L"DB_HOST", L"127.0.0.1", g_host, MAX_INI_ENTRIES_LENGTH, iniFile);
        dw = GetPrivateProfileString(DB_SECTION, L"Driver", L"", g_driver, MAX_INI_ENTRIES_LENGTH, iniFile);
        dw = GetPrivateProfileString(DB_SECTION, L"DB_NAME", L"", g_dbName, MAX_INI_ENTRIES_LENGTH, iniFile);
        dw = GetPrivateProfileString(DB_SECTION, L"DB_USER", L"", g_user, MAX_INI_ENTRIES_LENGTH, iniFile);
        dw = GetPrivateProfileString(DB_SECTION, L"DB_PWD", L"", g_pwd, MAX_INI_ENTRIES_LENGTH, iniFile);
        dw = GetPrivateProfileString(DB_SECTION, L"DB_TABLE", L"", g_table, MAX_INI_ENTRIES_LENGTH, iniFile);
        dw = GetPrivateProfileString(DB_SECTION, L"DB_PORT", L"", g_port, MAX_INI_ENTRIES_LENGTH, iniFile);

    }


}

/*
* function to write information to the INI file
* 
* @param void
* 
* @return BOOL (TRUE or FALSE)
* 
*/
BOOL writeINIFile(void) {
    WCHAR iniFile[MAX_PATH] = { 0 };
    HANDLE hToken = NULL;
    DWORD len = MAX_PATH;
    DWORD dw;
    
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        GetUserProfileDirectoryW(hToken, iniFile, &len);
        CloseHandle(hToken);

        StringCchCat(iniFile, NUMELEM(iniFile), L"/");
        StringCchCat(iniFile, NUMELEM(iniFile), CONFIG_DIRECTORY);

        // überprüfen ob das Verzeichnis existiert?
        // Wenn nicht erstellen!?

        StringCchCat(iniFile, NUMELEM(iniFile), L"/");
        StringCchCat(iniFile, NUMELEM(iniFile), CONFIG_FILE);

        dw = WritePrivateProfileStringW(DB_SECTION, L"DB_HOST",     g_host, iniFile);
        dw = WritePrivateProfileStringW(DB_SECTION, L"Driver",      g_driver, iniFile);
        dw = WritePrivateProfileStringW(DB_SECTION, L"DB_PORT",     g_port, iniFile);
        dw = WritePrivateProfileStringW(DB_SECTION, L"DB_NAME",     g_dbName, iniFile);
        dw = WritePrivateProfileStringW(DB_SECTION, L"DB_TABLE",    g_table, iniFile);
        dw = WritePrivateProfileStringW(DB_SECTION, L"DB_USER",     g_user, iniFile);
        dw = WritePrivateProfileStringW(DB_SECTION, L"DB_PWD",      g_pwd, iniFile);

        return TRUE;
    }
    
    return FALSE;
}

/*
* method to create the status bar, "private" function
* 
* @param HWND (the main window handle)
* @param HINSTANCE (the main window instance handle)
* @param int (how many status bar parts, normaly 1)
* 
* @return HWND (NULL or a window handle)
*/
HWND DoCreateStatusBar(HWND hwndParent, HINSTANCE hinst, int cParts)
{
    HWND hwndStatus;
    RECT rc;
    
    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    // Create the status bar.
    hwndStatus = CreateWindowExW(
        0,                       // no extended styles
        STATUSCLASSNAME,         // name of status bar class
        (LPCWSTR)NULL,           // no text when first created
        SBARS_SIZEGRIP |         // includes a sizing grip
        SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE,   // creates a visible child window
        0, 0, 0, 0,              // ignores size and position
        hwndParent,              // handle to parent window
        HMENU(IDB_STATUS),        // child window identifier
        hinst,                   // handle to application instance
        NULL);                   // no window creation data

    // Get the coordinates of the parent window's client area.
    GetClientRect(hwndParent, &rc);

    // Allocate an array for holding the right edge coordinates.
    int nHalf = rc.right / cParts;
    int paParts[1] = { nHalf };


    // Tell the status bar to create the window parts.
    SendMessage(hwndStatus, SB_SETPARTS, (WPARAM)cParts, (LPARAM)paParts);

    // Free the array, and return.
    return hwndStatus;
}

bool createTable() {
    
    LVITEM lvI;
    lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
    lvI.mask = LVIF_TEXT;
    lvI.stateMask = 0;
    lvI.iSubItem = 0;
    lvI.state = 0;
    int row = 0;

    // Insert items into the list.
    if (g_tableCreated == true) {
        ListView_DeleteAllItems(g_hwndList);
    }

    for (row = 0; row < g_rows; row++) {
        lvI.iItem = row;
        if (ListView_InsertItem(g_hwndList, &lvI) == -1)
            return false;

        SendMessage(g_hwndList, LVM_SETCOLUMNWIDTH, row, LVSCW_AUTOSIZE_USEHEADER);

        //ListView_MapIndexToID(g_hwndList, row);
    }
    return true;
}

void reloadTable() {
    if (g_updaterow == TRUE) {
        delete[] g_Body;
        g_Body = NULL;
        g_Body = new DATA[g_rows]{ 0 };
    }

    g_Ids = g_tableClass->GetIds();
    g_tableClass->select();
    g_tableClass->CreateMySQLDBBody(g_rows, g_Body);

    g_hwndList = g_tableClass->getlistHandle();
    createTable();

}
