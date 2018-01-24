// QuickLaunch.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "QuickLaunch.h"
#include "windowsx.h"
#define MAX_LOADSTRING 100

// Global Variables:	
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ViewStatitisticDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
//INT_PTR CALLBACK ScanProgramDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_QUICKLAUNCH, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUICKLAUNCH));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


HINSTANCE hInst;
HINSTANCE hInstHook;
HWND g_StatusBar;
HWND g_ListViewTop, g_EditText;
HWND g_Static, g_CheckBoxX86, g_ButtonBackup, g_StaticBackup;
HWND g_hwnd;
HHOOK hMyHook;
HFONT hFont;
int checkHotKey = 1;
RECT rect;
vector<Program> arrProgram;
vector<Program> arrDisplay;
vector<Program> arrProgramDeleted;
vector<Program> arrProgramFromDatabase;
WCHAR key[255];
HIMAGELIST hImageList;
HMENU hPopUp;
int sttRow;
int code;
NOTIFYICONDATA nti;
PAINTSTRUCT ps;
HDC hdc;
Pen* pen;
Brush *brush;
HWND hText;

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_3DFACE+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_QUICKLAUNCH);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Store instance handle in our global variable

   g_hwnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      300, 50, 700, 600, NULL, NULL, hInstance, NULL);

   if (!g_hwnd)
   {
      return FALSE;
   }

   ShowWindow(g_hwnd, nCmdShow);
   UpdateWindow(g_hwnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, onCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, onCommand);
		HANDLE_MSG(hWnd, WM_PAINT, onPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, onDestroy);
		HANDLE_MSG(hWnd, WM_NOTIFY, onListviewNotify);
		HANDLE_MSG(hWnd, WM_SYSCOMMAND, onSysCommand);		
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
	}

	if (message == WM_USER + 1) {
		switch (lParam) {
			case WM_RBUTTONDOWN: {
				HMENU hMenu = CreatePopupMenu();
				if (hMenu) {
					InsertMenu(hMenu, 1, MF_BYCOMMAND, ID_SCAN, L"Scan to build database");
					InsertMenu(hMenu, 2, MF_BYCOMMAND, ID_VIEW_STATITISTIC, L"View statitistics");
					InsertMenu(hMenu, 3, MF_BYCOMMAND, ID_EXIT, L"Exit");
					
					POINT pt;
					GetCursorPos(&pt);
					UINT clicked = TrackPopupMenu(hMenu, TPM_NONOTIFY | TPM_RETURNCMD | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);

					if (clicked == ID_SCAN) {
						doInstallHook(hWnd, g_ListViewTop);
						SendMessage(g_CheckBoxX86, BM_SETCHECK, BST_UNCHECKED, NULL);
						scanProgramCheckbox(arrProgram, arrDisplay, arrProgramDeleted);
						loadDataToListView(g_ListViewTop, arrDisplay, hImageList, hInst);
						ShowWindow(hWnd, SW_NORMAL);
					}
					else if (clicked == ID_VIEW_STATITISTIC) {
						DialogBox(hInst, MAKEINTRESOURCE(IDD_VIEW_STATITISTIC),
							hWnd, ViewStatitisticDialog);
					}
					else if (clicked == ID_EXIT) {
						writeFile(DATA_PATH, arrProgramFromDatabase);
						writeFileProgramDeleted(DATA_PATH_DELETED, arrProgramDeleted);
						DestroyWindow(hWnd);
					}
				}
			}
		}
	}
	else if (message == WM_KEYDOWN || message == WM_KEYUP) {
		SetFocus(g_ListViewTop);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// Message handler for about box.
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

BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) {
	hFont = CreateFont(16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, VIETNAMESE_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));

	GetClientRect(hwnd, &rect);
	arrProgram.clear();
	InitCommonControls();
	//add icon to notification area 
	addNotificationIcon(hwnd);

	hImageList = ImageList_Create(32, 32, ILC_COLOR16 | ILC_MASK, 0, 0);

	g_EditText = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 40, 45, rect.right - rect.left - 70, 30, hwnd, (HMENU)ID_EDITTEXT, hInst, NULL);
	SendMessage(g_EditText, WM_SETFONT, WPARAM(hFont), LPARAM(L"Nội dung"));

	g_ListViewTop = CreateWindow(WC_LISTVIEW, L"List View", WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS ,
		40, 110, (rect.right - rect.left - 70), rect.bottom - rect.top - 140, hwnd, (HMENU)ID_LVTOP, hInst, NULL);
	ListView_SetExtendedListViewStyle(g_ListViewTop, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	SendMessage(g_ListViewTop, LVM_SETEXTENDEDLISTVIEWSTYLE, WPARAM(hFont), LVS_EX_FULLROWSELECT);

	g_Static = CreateWindowEx(0, L"STATIC", L"Search Everywhere: ", WS_CHILD | WS_VISIBLE, 40, 20, rect.right - rect.left - 130, 
		20, hwnd, (HMENU)NULL, hInst, NULL);
	SendMessage(g_Static, WM_SETFONT, WPARAM(hFont), true);

	g_CheckBoxX86 = CreateWindowEx(0, L"BUTTON", L"Scan Program X86", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE,
		515, 20, 130, 20, hwnd, (HMENU)ID_CHECKBOXX86, hInst, NULL);
	SendMessage(g_CheckBoxX86, WM_SETFONT, WPARAM(hFont), true);
	SendMessage(g_CheckBoxX86, BM_SETCHECK, BST_UNCHECKED, NULL);

	g_StaticBackup = CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP,
		660, 0, 20, 20, hwnd, NULL, hInst, NULL);
	
	HBITMAP hBMPView = (HBITMAP)LoadImage(hInst, L"Scan.bmp",
		IMAGE_BITMAP, 20, 20,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	SendMessage(g_StaticBackup, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBMPView);

	createColumnListView(g_ListViewTop, (rect.right - rect.left - 70) * 50 / 100, (rect.right - rect.left - 70) * 50 / 100);
	
	scanProgramFile( arrProgram);
	readFileProgramDelected(DATA_PATH_DELETED, arrProgramDeleted);
	deleteProgramArray(arrProgram, arrProgramDeleted);
	sortList(arrProgram, ASCENDING);
	readFile(DATA_PATH, arrProgramFromDatabase);
	excuteFrequency(arrProgram, arrProgramFromDatabase);
	//Load data
	arrDisplay = arrProgram;
	loadDataToListView(g_ListViewTop, arrProgram, hImageList, hInst);

	doInstallHook(hwnd, g_ListViewTop);

	return TRUE;
}

void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	switch (id)
	{
	case ID_EDITTEXT:
		if (codeNotify == EN_CHANGE) {
			deleteProgramArray(arrProgram, arrProgramDeleted);
			GetWindowText(g_EditText, key, 255);
			if (key[0] == 0)
			{
				sortList(arrProgram, ASCENDING);
				arrDisplay = arrProgram;
				loadDataToListView(g_ListViewTop, arrProgram, hImageList, hInst);
				break;
			}
			excuteFrequency(arrProgram, arrProgramFromDatabase);
			searchList(arrProgram, arrDisplay, key);
			if (arrDisplay.size() != 0) {
				sortList(arrDisplay, ASCENDING);
				sortListByFrequency(arrDisplay);
			}
			loadDataToListView(g_ListViewTop, arrDisplay, hImageList, hInst);
		}
		break;
	case ID_CHECKBOXX86:
		if (codeNotify == BN_CLICKED) {
			scanProgramCheckbox(arrProgram, arrDisplay, arrProgramDeleted);
			loadDataToListView(g_ListViewTop, arrDisplay, hImageList, hInst);
		}
		break;
	case ID_DELETE: {
						sttRow = ListView_GetSelectionMark(g_ListViewTop);
						Program p;
						p.name = arrDisplay[sttRow].name;
						arrProgramDeleted.push_back(p);
						arrDisplay.erase(arrDisplay.begin() + sttRow);
						deleteItem(g_ListViewTop, sttRow);
						break;
	}
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hwnd);
		break;
	}
}

void onPaint(HWND hwnd) {
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hwnd, &ps);
	// TODO: Add any drawing code here...
	EndPaint(hwnd, &ps);
}

void onDestroy(HWND hwnd) {
	Shell_NotifyIcon(NIM_DELETE, &nti);
	PostQuitMessage(0);
}


LRESULT onListviewNotify(HWND hWnd, int idFrom, NMHDR* pnm) {
	switch (pnm->code) {
	case NM_RCLICK: {
						sttRow = ListView_GetSelectionMark(g_ListViewTop);
						if (sttRow != -1) {
							SendMessage(g_ListViewTop, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
							hPopUp = CreatePopupMenu();
							InsertMenu(hPopUp, 0, MF_BYPOSITION, ID_DELETE, L"Xóa");
							POINT point;
							GetCursorPos(&point);
							TrackPopupMenu(hPopUp, TPM_TOPALIGN | TPM_LEFTALIGN, point.x, point.y, 0, hWnd, NULL);
						}
						break;
	}

	case NM_DBLCLK:
		if (pnm->hwndFrom == g_ListViewTop) {
			sttRow = ListView_GetSelectionMark(g_ListViewTop);
			loadProgram(arrDisplay, arrProgramFromDatabase, sttRow);
		}
		break;
	case NM_RETURN: //Xử lý nhấn Enter
		if (pnm->hwndFrom == g_ListViewTop) {
			sttRow = ListView_GetSelectionMark(g_ListViewTop);
			loadProgram(arrDisplay, arrProgramFromDatabase, sttRow);
		}
		break;
	}
	
	return 0;
}

void onSysCommand(HWND hwnd, UINT cmd, int x, int y) {
	if (SC_CLOSE == cmd)
	{
		doRemoveHook(hwnd, g_ListViewTop);
		ShowWindow(hwnd, SW_HIDE);
	}
	else
		DefWindowProc(hwnd, WM_SYSCOMMAND, cmd, MAKELPARAM((x), (y)));
}

void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
	if (x > 660 && x <= 680 && y > 0 && y <= 20) {
		int check = MessageBox(hwnd, L"Do you want RECOVERY data", L"Notification", MB_OKCANCEL | MB_ICONINFORMATION);
		if (check == true) {
			arrProgramDeleted.clear();
			arrProgramFromDatabase.clear();
			arrProgram.clear();
			arrDisplay.clear();

			scanProgramFile(arrProgram);
			SendMessage(g_CheckBoxX86, BM_SETCHECK, BST_UNCHECKED, NULL);
			SetWindowText(g_EditText, L"");
		}
	}
}

void addNotificationIcon(HWND hWnd)
{
	ZeroMemory(&nti, sizeof(NOTIFYICONDATA));
	nti.cbSize = sizeof(NOTIFYICONDATA);
	nti.uID = IDI_ICON1;
	nti.hIcon = (HICON)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	nti.hBalloonIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	nti.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nti.hWnd = hWnd;
	nti.uCallbackMessage = WM_USER + 1;
	wcscpy(nti.szTip, L"QuickLaunch");
	Shell_NotifyIcon(NIM_MODIFY, &nti);
	Shell_NotifyIcon(NIM_ADD, &nti);
}

void scanProgramCheckbox(vector<Program> &arrProgram, vector<Program> &arrDisplay, vector<Program> &arrProgramDelected) {
	arrDisplay.clear();
	arrProgram.clear();
	int check_x86 = SendMessage(g_CheckBoxX86, BM_GETCHECK, 0, 0);

	if (check_x86 == BST_CHECKED)
		scanProgramFileX86(arrProgram);

	scanProgramFile(arrProgram);
	GetWindowText(g_EditText, key, 255);
	if (key[0] == 0)
	{
		deleteProgramArray(arrProgram, arrProgramDelected);
		sortList(arrProgram, ASCENDING);
		arrDisplay = arrProgram;
		return;
	}

	excuteFrequency(arrProgram, arrProgramFromDatabase);
	deleteProgramArray(arrProgram, arrProgramDeleted);
	searchList(arrProgram, arrDisplay, key);
	sortList(arrDisplay, ASCENDING);
	sortListByFrequency(arrDisplay);
}

INT_PTR CALLBACK ViewStatitisticDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	int wmId, wmEvent;
	WCHAR* buffText = NULL;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: {
							return (INT_PTR)TRUE;
	}
	case WM_CTLCOLORDLG:
		return (INT_PTR)CreateSolidBrush(RGB(250, 252, 186));
	case WM_CTLCOLORSTATIC:
	{
							  HDC hdcStatic = (HDC)wParam;
							  SetTextColor(hdcStatic, RGB(0, 0, 0));
							  SetBkColor(hdcStatic, RGB(250, 252, 186));
							  return (INT_PTR)CreateSolidBrush(RGB(250, 252, 186));
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_PAINT:
	{
					 hdc = BeginPaint(hDlg, &ps);
					 Graphics *graphics = new Graphics(hdc);
					 int totalFrequency = caculateFrequency(arrProgramFromDatabase);
					 int size = arrProgramFromDatabase.size();
					 float maxAngle = 360.0F;
					 float startAngle = -90.0F;
					 float sweepAngle = 0.0F;
					 float eachAngleNote = maxAngle / totalFrequency;

					 if (size == 0) {
						 break;
					 }

					 for (int i = 0; i < size - 1; i++) {
						 for (int j = i + 1; j < size; j++) {
							 if (arrProgramFromDatabase[i].frequency < arrProgramFromDatabase[j].frequency) {
								 Program p = arrProgramFromDatabase[i];
								 arrProgramFromDatabase[i] = arrProgramFromDatabase[j];
								 arrProgramFromDatabase[j] = p;
							 }
						 }
					 }

					 for (int i = 0; i < size && i < 5; i++) {
						 sweepAngle = arrProgramFromDatabase[i].frequency * eachAngleNote;
						 pen = new Pen(listColor[i]);
						 brush = new SolidBrush(listColor[i]);

						 if (startAngle >= 360)
							 startAngle -= 360;

						 Rect rect(20, 10, 210, 210);
						 graphics->FillPie(brush, rect, startAngle, sweepAngle);
						 graphics->DrawPie(pen, rect, startAngle, sweepAngle);
						 startAngle += sweepAngle;

						 Rect rectNote(270, 30 * i + 35, 20, 20);
						 graphics->FillRectangle(brush, rectNote);
						 graphics->DrawRectangle(pen, rectNote);

						 buffText = new WCHAR[255];
						 int percent = sweepAngle * 100 / 360;
						 swprintf(buffText, L"%s (%s%%)", arrProgramFromDatabase[i].name.c_str(), to_wstring(percent).c_str());
						 hText = CreateWindowEx(0, L"STATIC", buffText, WS_CHILD | WS_VISIBLE | SS_LEFT, 300, 30 * i + 35, 130, 20, hDlg, NULL, hInst, NULL);
						 SendMessage(hText, WM_SETFONT, (WPARAM)hFont, TRUE);
					 }

					 if (size >= 6) {
						 int totalFrequencyOther = 0;
						 for (int i = 5; i < size; i++) {
							 totalFrequencyOther += arrProgramFromDatabase[i].frequency;
						 }
						 sweepAngle = totalFrequencyOther* eachAngleNote;
						 pen = new Pen(listColor[5]);
						 brush = new SolidBrush(listColor[5]);

						 Rect rect(20, 10, 210, 210);
						 graphics->FillPie(brush, rect, startAngle, sweepAngle);
						 graphics->DrawPie(pen, rect, startAngle, sweepAngle);

						 Rect rectNote(270, 30 * 5 + 35, 20, 20);
						 graphics->FillRectangle(brush, rectNote);
						 graphics->DrawRectangle(pen, rectNote);

						 buffText = new WCHAR[255];
						 int percent = sweepAngle * 100 / 360;
						 swprintf(buffText, L"Khác (%s%%)", to_wstring(percent).c_str());
						 hText = CreateWindowEx(0, L"STATIC", buffText, WS_CHILD | WS_VISIBLE | SS_LEFT, 300, 30 * 5 + 35, 130, 20, hDlg, NULL, hInst, NULL);
						 SendMessage(hText, WM_SETFONT, (WPARAM)hFont, TRUE);
					 }
					 delete graphics;
					 EndPaint(hDlg, &ps);
					 break;
	}
	case WM_DESTROY:
	{
					   EndDialog(hDlg, LOWORD(wParam));
					   return (INT_PTR)TRUE;
	}
	}
	if (!buffText) {
		delete[] buffText;
	}
	return (INT_PTR)FALSE;
}

void doInstallHook(HWND hWnd, HWND g_lv)
{
	// gọi hàm DLL theo kiểu Run-time
	// Định nghĩa prototype của hàm
	typedef VOID(*MYPROC)(HWND,HWND);

	HINSTANCE hinstLib;
	MYPROC ProcAddr;

	// load DLL và lấy handle của DLL module
	hinstLib = LoadLibrary(L"DLL.dll");
	// Nếu load thành công, lấy địa chỉ của hàm DrawCircle trong DLL
	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_doInstallHookKeyBoard");
		// Nếu lấy được địa chỉ hàm, gọi thực hiện hàm
		if (ProcAddr != NULL)
			ProcAddr(hWnd,g_lv);
	}
}

void doRemoveHook(HWND hWnd, HWND g_lv)
{
	// gọi hàm DLL theo kiểu Run-time
	// Định nghĩa prototype của hàm
	typedef VOID(*MYPROC)(HWND,HWND);

	HINSTANCE hinstLib;
	MYPROC ProcAddr;

	// load DLL và lấy handle của DLL module
	hinstLib = LoadLibrary(L"DLL.dll");
	// Nếu load thành công, lấy địa chỉ của hàm DrawCircle trong DLL
	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_doRemoveHookKeyBoard");
		// Nếu lấy được địa chỉ hàm, gọi thực hiện hàm
		if (ProcAddr != NULL)
			ProcAddr(hWnd, g_lv);
	}
}