#pragma once
#include "Program.h"
#include "Helper.h"
#include <vector>
#include <string>
#include <Commctrl.h>
#pragma comment(lib, "comctl32.lib")

#include <Shobjidl.h>
#include <shellapi.h>
#include <Shlobj.h>
#pragma comment (lib,"Shell32.lib")
using namespace std;
#define PROGRAM_FILE_PATH L"C:\\Program Files"
#define PROGRAM_FILE_PATH_X86 L"C:\\Program Files (x86)"

void createColumnListView(HWND hwnd, int r1, int r2);
void loadDataToListView(HWND hwnd, vector<Program> arrProgram, HIMAGELIST hImageList, HINSTANCE hInst);
void scanProgramFileX86(vector<Program> &arrProgram);
void scanProgramFile(vector<Program> &arrProgram);
void deleteItem(HWND hwnd, int index);