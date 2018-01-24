#include "stdafx.h"
#include "Helper.h"

void SearchPathByDeep(WCHAR* path, int deeplevel , vector<Program> &arrProgram) {
	if (deeplevel >= 2)
		return;
	if (path == NULL || wcslen(path) == 0) {
		return;
	}

	WCHAR appContent[100];
	wcscpy(appContent, path);
	wcscat(appContent, L"\\*");

	WIN32_FIND_DATA wfd;
	HANDLE hFile = FindFirstFile(appContent, &wfd);
	bool isFound = true;
	if (hFile == INVALID_HANDLE_VALUE) {
		isFound = false;
	}
	while (isFound) {
		if (deeplevel == 1) {
			if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				isFound = FindNextFile(hFile, &wfd);
				continue;
			}
		}
		if ((wcscmp(wfd.cFileName, _T(".")) != 0) && (wcscmp(wfd.cFileName, _T("..")) != 0)) {
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				WCHAR childDir[255];
				wcscpy(childDir, path);
				wcscat(childDir, L"\\");
				wcscat(childDir, wfd.cFileName);
				SearchPathByDeep(childDir, deeplevel + 1, arrProgram);
			}
			else {
				if (isFileExe(wfd.cFileName))
				{
					WCHAR childFile[255];
					wcscpy(childFile, path);
					wcscat(childFile, L"\\");
					wcscat(childFile, wfd.cFileName);

					//Save data
					Program p;
					p.path = path;
					p.icon = childFile;
					p.name = wfd.cFileName;
					p.version = L"";
					arrProgram.push_back(p);
				}
			}
		}
		isFound = FindNextFile(hFile, &wfd);
	}
}

bool isFileExe(WCHAR file[]) {
	if (wcsstr(file, L".exe") != nullptr)
		return true;
	return false;
}

void sortList(vector<Program> &arrDisplay, int mode) {
	for (int i = 0; i < arrDisplay.size() - 1; i++) {
		for (int j = i + 1; j < arrDisplay.size(); j++) {
			if (mode == ASCENDING) {
				if (arrDisplay[i].name.compare(arrDisplay[j].name) > 0) {
					Program p = arrDisplay[i];
					arrDisplay[i] = arrDisplay[j];
					arrDisplay[j] = p;
				}
			}
			else {
				if (arrDisplay[i].name.compare(arrDisplay[j].name) < 0) {
					Program p = arrDisplay[i];
					arrDisplay[i] = arrDisplay[j];
					arrDisplay[j] = p;
				}
			}
		}
	}
}

void sortListByFrequency(vector<Program> &arrDisplay) {
	for (int i = 0; i < arrDisplay.size() - 1; i++) {
		for (int j = i + 1; j < arrDisplay.size(); j++) {
			if (arrDisplay[i].frequency < arrDisplay[j].frequency)
			{
				Program c = arrDisplay[i];
				arrDisplay[i] = arrDisplay[j];
				arrDisplay[j] = c;
			}
		}
	}
}


void searchList(vector<Program> arrProgram, vector<Program> &arrDisplay, wstring key) {
	arrDisplay.clear();
	wstring temp;
	wstring _key = wcslwr((WCHAR*)key.c_str());
	vector<Program> arrLower = arrProgram;
	for (int i = 0; i < arrProgram.size(); i++) {
		temp = wcslwr((WCHAR*)arrLower[i].name.c_str());
		if (temp.find(_key) != -1) {
			arrDisplay.push_back(arrProgram[i]);
		}
	}
}



void excuteFrequency(vector<Program> &arrProgram, vector<Program> arrProgramFromDatabase)
{
	int size_program = arrProgram.size();
	int size_programDatabase = arrProgramFromDatabase.size();
	for (int i = 0; i < size_program; i++)
	{
		arrProgram[i].index = i;
		for (int j = 0; j < size_programDatabase; j++)
		{
			if (arrProgram[i].name == arrProgramFromDatabase[j].name)
			{
				arrProgram.at(i).frequency = arrProgramFromDatabase[j].frequency;
				break;
			}
		}
	}
}


void loadProgram(vector<Program> arrDisplay, vector<Program> &arrProgramFromDB, int index) {
	wstring exepath = arrDisplay[index].icon.c_str();
	bool check = false;
	if (!exepath.empty()) {	

		for (int i = 0; i < arrProgramFromDB.size(); i++) {
			if (!arrProgramFromDB[i].name.compare(arrDisplay[index].name)) {
				arrProgramFromDB[i].frequency++;
				check = true;
				break;
			}
		}

		if (check == false)
		{
			arrDisplay[index].frequency = 1;
			arrProgramFromDB.push_back(arrDisplay[index]);
		}

		ShellExecute(NULL, _T("open"), exepath.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
}

void deleteProgramArray(vector<Program> &arrProgram, vector<Program> arrProgramDeleted) {
	for (int i = 0; i < arrProgramDeleted.size(); i++) {
		for (int j = 0; j < arrProgram.size(); j++) {
			if (!wcscmp(arrProgramDeleted[i].name.c_str(), arrProgram[j].name.c_str()))
				arrProgram.erase(arrProgram.begin() + j);
		}
	}
}

int caculateFrequency(vector<Program> arrProgramFromDB) {
	int sum = 0;
	for (int i = 0; i < arrProgramFromDB.size(); i++) {
		sum += arrProgramFromDB[i].frequency;
	}
	return sum;
}
