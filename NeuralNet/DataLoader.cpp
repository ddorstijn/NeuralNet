#include "DataLoader.h"

#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

const std::wstring rootpath = L"Data\\";

void DataLoader::throughDirs() {
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile((LPWSTR)(rootpath + L"*").c_str(), &data);      // DIRECTORY
	int i = 0;

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (++i <= 2) { continue; } // skip . and .. dictionaries :/
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				std::wstring fName(data.cFileName);
				std::string foldername(fName.begin(), fName.end());
				outputs.push_back(foldername);
				/*foldername = foldername + "\\";
				string folder = rootpath + foldername;*/
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
}

std::vector<std::string> DataLoader::LoadFiles(std::string folder)
{
	std::wstring f(folder.begin(), folder.end());
	std::wstring path = rootpath + f + L"\\";
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile((path + L"*").c_str(), &data);      // DIRECTORY
	int i = 0;

	std::vector<std::string> files = std::vector<std::string>();

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (i++ < 2) { continue; } // skip . and .. dictionaries :/
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				std::wstring str = std::wstring(data.cFileName);
				std::wstring fullpath = path + str;
				std::string filepath(fullpath.begin(), fullpath.end());
				std::string ext = filepath.substr(filepath.find_last_of("."));
				if (ext == ".json") {
					files.push_back(filepath);
				}
				// _tprintf(TEXT(" %s   \n"), fullpath.c_str());
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}

	return files;
}

DataLoader::DataLoader()
{
	this->throughDirs();
	std::cout << "start" << std::endl;
}


DataLoader::~DataLoader()
{
}

json DataLoader::RandomFile(std::string folder)
{
	std::vector<std::string> files = LoadFiles(folder);
	std::string randomfile = files.at(rand() % files.size());
	std::ifstream t(randomfile);
	std::stringstream buffer;
	buffer << t.rdbuf();
	json j = json::parse(buffer.str());
	j["filepath"] = randomfile;
	t.close();
	return j;
}

file DataLoader::LoadRandom() 
{	
	std::string folder = this->outputs.at(rand() % this->outputs.size());
	std::cout << "random folder = " << folder << std::endl;
	json j = this->RandomFile(folder);
	file f{
		j["label"].get<std::string>(),
		std::vector<double>(),
		j["filepath"].get<std::string>(),
	};
	std::cout << "Size pose:: " << j["pose"].size() << std::endl;
	if (j["pose"].size() != 36) {
		std::cout << "=== ERROR in file, pose points does not equal 36. There is likely more than 1 person in the file (" << j["pose"].size() / 36 << " people detected)" << std::endl;
		std::cout << "=== Loading other file" << std::endl;
		return LoadRandom();
	}
	for (json::iterator it = j["pose"].begin(); it != j["pose"].end(); ++it) {
		f.pose.push_back(it->get<double>());
	}
	return f;
}

void DataLoader::Load(std::vector<file> &testSet, std::vector<file> &trainingSet)
{
	std::vector<file> data;
	for (std::vector<std::string>::iterator o_it = outputs.begin(); o_it != outputs.end(); ++o_it) {
		auto files = LoadFiles(*o_it);
		for (std::vector<std::string>::iterator f_it = files.begin(); f_it != files.end(); ++f_it) {
			std::ifstream t(*f_it);
			std::stringstream buffer;
			buffer << t.rdbuf();
			json j = json::parse(buffer.str());
			j["filepath"] = *f_it;
			t.close();

			file f {
				j["label"].get<std::string>(),
				std::vector<double>(),
				j["filepath"].get<std::string>(),
			};
			for (json::iterator it = j["pose"].begin(); it != j["pose"].end(); ++it) {
				f.pose.push_back(it->get<double>());
			}

			data.push_back(f);
		}
	}

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(data.begin(), data.end(), std::default_random_engine(seed));

	for (unsigned i = 0; i < data.size(); i++) {
		if (i % 2) {
			trainingSet.push_back(data.at(i));
		} else {
			testSet.push_back(data.at(i));
		}
	}
}

std::vector<std::vector<double>> DataLoader::GetTopology()
{
	std::vector<std::vector<double>> topology;
	std::ifstream file(rootpath + L"topology.json");
	if (!file.good()) {
		throw std::exception("loading failed");
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	json j = json::parse(buffer.str());
	file.close();
	return j;
}

void DataLoader::SaveTopology(std::vector<std::vector<double>> topology, std::wstring filename)
{
	json j = topology;
	std::ofstream file(rootpath + filename);
	file << j.dump();
	file.close();
}