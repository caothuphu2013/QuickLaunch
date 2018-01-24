#pragma once
#include "Program.h"
#include <string>
#include <vector>
#include <fstream>


using namespace std;

void writeFile(wstring filePath, vector<Program> arrProgramFromDB);
void readFile(wstring filePath, vector<Program> &arrProgramFromDB);
void writeFileProgramDeleted(wstring filePath, vector<Program> arrProgramDeleted);
void readFileProgramDelected(wstring filePath, vector<Program> &arrProgramDeleted);
