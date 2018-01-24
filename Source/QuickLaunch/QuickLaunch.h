#pragma once
#include "resource.h"
#include <Commctrl.h>
#pragma comment(lib, "comctl32.lib")
#include "ScanListView.h"
#include "Program.h"
#include "Data.h"
#include <gdiplus.h>
using namespace Gdiplus;

#define DATA_PATH L"program_data.txt"
#define DATA_PATH_DELETED L"program_deleted.txt"
#pragma comment(lib, "shlwapi.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
#pragma comment (lib, "gdiplus.lib")

#include <locale>
#include <codecvt>

//Thống kê
Color listColor[6] = {
	Color(255, 0, 0),
	Color(0, 255, 0),
	Color(0, 0, 255),
	Color(255, 255, 0),
	Color(255, 0, 255),
	Color(0, 255, 255),
};

BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void onPaint(HWND hwnd);
void onDestroy(HWND hwnd);
void onKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
LRESULT onListviewNotify(HWND hWnd, int idFrom, NMHDR* pnm);
void onSysCommand(HWND hwnd, UINT cmd, int x, int y);
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);

void addNotificationIcon(HWND hWnd);
void scanProgramCheckbox(vector<Program> &arrProgram, vector<Program> &arrDisplay, vector<Program> &arrProgramDelected);

void doInstallHook(HWND hWnd, HWND g_lv);
void doRemoveHook(HWND hWnd, HWND g_lv);