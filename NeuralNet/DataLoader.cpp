#include "DataLoader.h"

#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

const wstring rootpath = L"Data\\";

void DataLoader::throughDirs() {
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile((LPWSTR)(rootpath + L"*").c_str(), &data);      // DIRECTORY
	int i = 0;

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (++i <= 2) { continue; } // skip . and .. dictionaries :/
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				wstring fName(data.cFileName);
				string foldername(fName.begin(), fName.end());
				outputs.push_back(foldername);
				/*foldername = foldername + "\\";
				string folder = rootpath + foldername;*/
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
}

DataLoader::DataLoader()
{
	this->throughDirs();
	cout << "start" << endl;
}


DataLoader::~DataLoader()
{
}

json DataLoader::RandomFile(string folder)
{
	wstring f(folder.begin(), folder.end());
	wstring path = rootpath + f + L"\\";
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile((path + L"*").c_str(), &data);      // DIRECTORY
	int i = 0;

	vector<string> files = vector<string>();

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (i++ < 2) { continue; } // skip . and .. dictionaries :/
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				wstring str = wstring(data.cFileName);
				wstring fullpath = path + str;
				string filepath(fullpath.begin(), fullpath.end());
				string ext = filepath.substr(filepath.find_last_of("."));
				if (ext == ".json") {
					files.push_back(filepath);
				}
				// _tprintf(TEXT(" %s   \n"), fullpath.c_str());
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
	/*for (int i = 0; i < files.size(); i++) {
		cout << "FILE " << i << files.at(i) << endl;
	}*/
	string randomfile = files.at(rand() % files.size());
	ifstream t(randomfile);
	stringstream buffer;
	buffer << t.rdbuf();
	json j = json::parse(buffer.str());
	j["filepath"] = randomfile;
	t.close();
	return j;
}

file DataLoader::LoadRandom() 
{
	string folder = this->outputs.at(rand() % this->outputs.size());
	cout << "random folder = " << folder << endl;
	json j = this->RandomFile(folder);
	file f{
		j["label"].get<string>(),
		vector<double>(),
		j["filepath"].get<string>(),
	};
	for (json::iterator it = j["pose"].begin(); it != j["pose"].end(); ++it) {
		f.pose.push_back(it->get<double>());
	}
	return f;
}

vector<vector<double>> DataLoader::GetTopology()
{
	vector<vector<double>> topology;
	ifstream file(rootpath + L"topology.json");
	if (!file.good()) {
		throw exception("loading failed");
	}
	stringstream buffer;
	buffer << file.rdbuf();
	json j = json::parse(buffer.str());
	file.close();
	return j;
}

void DataLoader::SaveTopology(vector<vector<double>> topology, wstring filename)
{
	json j = topology;
	ofstream file(rootpath + filename);
	file << j.dump();
	file.close();
}