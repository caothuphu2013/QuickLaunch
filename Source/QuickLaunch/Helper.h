#pragma once
#include <string>
#include "Program.h"
#include <vector>


#include <shellapi.h>
#pragma comment (lib,"Shell32.lib")


using namespace std;
#define ASCENDING 1
#define DESCENDING -1

bool isFileExe(WCHAR file[]);
void SearchPathByDeep(WCHAR* path, int deeplevel, vector<Program> &arrProgram);

void sortList(vector<Program> &arrDisplay, int mode);
void sortListByFrequency(vector<Program> &arrDisplay);

void searchList(vector<Program> arrProgram, vector<Program> &arrDisplay, wstring key);
void excuteFrequency(vector<Program> &arrProgram, vector<Program> arrProgramFromDatabase);
void loadProgram(vector<Program> arrDisplay, vector<Program> &arrProgram, int index);
void deleteProgramArray(vector<Program> &arrProgram, vector<Program> arrProgramDeleted);

int caculateFrequency(vector<Program> arrProgramFromDB);