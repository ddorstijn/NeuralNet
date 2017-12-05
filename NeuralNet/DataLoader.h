#include <vector>
#include <string>
#include <chrono> 
#include <random>
#include "json.hpp"

using json = nlohmann::json;

#pragma once

struct file {
	std::string label;
	std::vector<double> pose;
	std::string filepath;
};

class DataLoader
{
public:
	DataLoader();
	~DataLoader();

	file LoadRandom();
	void Load(std::vector<file> &testSet, std::vector<file> &trainingSet);
	std::vector<std::vector<double>> GetTopology();
	void SaveTopology(std::vector<std::vector<double>> topology, std::wstring filename = L"topology.json");
	std::vector<std::string> outputs;
private:
	void throughDirs();
	std::vector<std::string> LoadFiles(std::string folder);
	json RandomFile(std::string folder);
};

