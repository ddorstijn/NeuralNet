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
	vector<vector<int>> GetTopology();
	void SaveTopology(vector<vector<int>> topology);
	vector<string> outputs;
private:
	void throughDirs();
	json RandomFile(string folder);
};

