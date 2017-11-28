#include <vector>
#include <string>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;
#pragma once

struct file {
	string label;
	vector<double> pose;
	string filepath;
};

class DataLoader
{
public:
	DataLoader();
	~DataLoader();
	file LoadRandom();
	vector<vector<double>> GetTopology();
	void SaveTopology(vector<vector<double>> topology, wstring filename = L"topology.json");
	vector<string> outputs;
private:
	void throughDirs();
	json RandomFile(string folder);
};

