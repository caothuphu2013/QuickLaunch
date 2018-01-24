// DLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>

#define EXPORT __declspec(dllexport)

HHOOK hHook = NULL;
HINSTANCE hinstLib;
HWND g_hWnd, g_ListView;
BOOL checkState = FALSE;

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) // không xử lý message 
		return CallNextHookEx(hHook, nCode, wParam, lParam);

	if (wParam == WM_KEYDOWN)
	{
		KBDLLHOOKSTRUCT *hookData = (KBDLLHOOKSTRUCT*)(lParam);
		if (GetAsyncKeyState(VK_CONTROL) && hookData->vkCode == 'A')
		{
			if (checkState == FALSE) {
				ShowWindow(g_hWnd, SW_SHOWMINIMIZED);
				checkState = TRUE;
			}
			else {
				ShowWindow(g_hWnd, SW_SHOWDEFAULT);
				checkState = FALSE;
			}

		}
		else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(VK_UP)) {
			SetFocus(g_ListView);
		}
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

EXPORT void _doInstallHookKeyBoard(HWND hWnd, HWND listview)
{
	if (hHook != NULL) return;

	g_hWnd = hWnd;
	g_ListView = listview;
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardHookProc, hinstLib, 0);
}

EXPORT void _doRemoveHookKeyBoard(HWND hWnd)
{
	if (hHook == NULL) return;
	UnhookWindowsHookEx(hHook);
	hHook = NULL;
}
