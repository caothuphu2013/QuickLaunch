#include "stdafx.h"
#include "ScanListView.h"

WIN32_FIND_DATA  wfd;
HANDLE hFile;

void createColumnListView(HWND hwnd, int r1, int r2) {
	LVCOLUMN lvCol;
	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.pszText = L"Name";
	lvCol.cx = r1;
	ListView_InsertColumn(hwnd, 0, &lvCol);

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.pszText = L"Path";
	lvCol.cx = r2;
	ListView_InsertColumn(hwnd, 1, &lvCol);
}

void loadDataToListView(HWND hwnd, vector<Program> arrDisplay, HIMAGELIST hImageList, HINSTANCE hInst) {
	ListView_DeleteAllItems(hwnd);
	for (int i = 0; i < arrDisplay.size(); i++) {
		LV_ITEM lv;
		lv.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
		lv.iSubItem = 0;
		lv.iItem = i;
		lv.pszText = (LPWSTR)arrDisplay[i].name.c_str();
		ListView_InsertItem(hwnd, &lv);

		
		WORD a;
		HICON hicon = ExtractAssociatedIcon(hInst, (LPWSTR)arrDisplay[i].icon.c_str(), &a);
		int icon = ImageList_AddIcon(hImageList, hicon);
		lv.iImage = icon;
		ListView_SetImageList(hwnd, hImageList, 1);
		ListView_SetItem(hwnd, &lv);
		

		lv.mask = LVIF_TEXT;
		lv.iSubItem = 1;
		lv.pszText = (LPWSTR)arrDisplay[i].path.c_str();
		ListView_SetItem(hwnd, &lv);

	}
}

void scanProgramFileX86( vector<Program> &arrProgram) {
	//WCHAR* programFileRoot = new WCHAR[wcslen(PROGRAM_FILE_PATH_X86) + 2];
	WCHAR programFileRoot[255];
	wcscpy(programFileRoot, PROGRAM_FILE_PATH_X86);
	wcscat(programFileRoot, L"\\*");

	hFile = FindFirstFile(programFileRoot, &wfd);
	bool found = true;

	if (hFile == INVALID_HANDLE_VALUE) {
		found = false;
	}
	while (found) {
		WCHAR path[255];
		if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (wcscmp(wfd.cFileName, L".") != 0) && (wcscmp(wfd.cFileName, L"..") != 0)) {
			wcscpy(path, PROGRAM_FILE_PATH_X86);
			wcscat(path, L"\\");
			wcscat(path, wfd.cFileName);
			SearchPathByDeep(path, 0, arrProgram);
		}
		found = FindNextFile(hFile, &wfd);
	}
}

void scanProgramFile(vector<Program> &arrProgram) {
	//WCHAR* programFileRoot = new WCHAR[wcslen(PROGRAM_FILE_PATH) + 2];
	WCHAR programFileRoot[255];
	wcscpy(programFileRoot, PROGRAM_FILE_PATH);
	wcscat(programFileRoot, L"\\*");

	hFile = FindFirstFile(programFileRoot, &wfd);
	bool found = true;

	if (hFile == INVALID_HANDLE_VALUE) {
		found = false;
	}
	while (found) {
		WCHAR path[255];
		if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (wcscmp(wfd.cFileName, L".") != 0) && (wcscmp(wfd.cFileName, L"..") != 0)) {
			wcscpy(path, PROGRAM_FILE_PATH);
			wcscat(path, L"\\");
			wcscat(path, wfd.cFileName);
			SearchPathByDeep( path, 0, arrProgram);
		}
		found = FindNextFile(hFile, &wfd);
	}
}


void deleteItem(HWND hwnd, int index) {
	int result = MessageBox(0, L"Bạn có chắc chắn muốn xoá?", L"Thông báo", MB_OKCANCEL | MB_ICONQUESTION);
	if (result == 1) {
		ListView_DeleteItem(hwnd, index);
	}
}