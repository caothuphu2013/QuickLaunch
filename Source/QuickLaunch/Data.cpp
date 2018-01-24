#include "stdafx.h"
#include "Data.h"

void writeFile(wstring filePath, vector<Program> arrProgramFromDB) {
	wofstream file(filePath);

	if (!file.is_open()) {
		MessageBox(0, L"Khong mo duoc file", 0, 0);
	}
	else {
		for (int i = 0; i < arrProgramFromDB.size(); i++) {
			if (arrProgramFromDB[i].frequency > 0) {
				file << arrProgramFromDB[i].name << endl;
				file << arrProgramFromDB[i].frequency << endl;
			}
		}

		file.close();
	}
}

void readFile(wstring filePath, vector<Program> &arrProgramFromDB) {
	wifstream file(filePath);
	wstring buff;

	if (file.is_open()) {
		while (!file.eof()) {
			Program p;
			getline(file, buff);
			if (buff.length() == 0)
				break;
			else
				p.name = buff;

			getline(file, buff);
			if (buff.length() == 0)
				p.frequency = 0;
			else
				p.frequency = _wtoi64(buff.c_str());

			arrProgramFromDB.push_back(p);
		}
		file.close();
	}
}

void readFileProgramDelected(wstring filePath, vector<Program> &arrProgramDeleted) {
	wifstream file(filePath);
	wstring buff;

	if (file.is_open()) {
		while (!file.eof()) {
			Program p;
			getline(file, buff);
			if (buff.length() == 0)
				break;
			else
				p.name = buff;
			arrProgramDeleted.push_back(p);
		}
		file.close();
	}
}

void writeFileProgramDeleted(wstring filePath, vector<Program> arrProgramDeleted) {
	wofstream file(filePath);

	if (!file.is_open()) {
		MessageBox(0, L"Khong mo duoc file", 0, 0);
	}
	else {
		for (int i = 0; i < arrProgramDeleted.size(); i++) {
			file << arrProgramDeleted[i].name << endl;
		}
	}

	file.close();
}
