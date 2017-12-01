#include <vector>
#include <iostream>

#include "DataLoader.h"
#include "Net.hpp"
#include "Neuron.hpp"
#include <time.h>

std::vector<double> randomNeurons(double amount) {
	std::vector<double> layer;
	for (int i = 0; i < amount; i++) {
		layer.push_back(rand() / double(RAND_MAX)); // insert randomweight
	}
	return layer;
}

int main()
{
	srand(time(NULL));
	DataLoader* dl = new DataLoader();
	file f = dl->LoadRandom();
	cout << "label = " << f.label << endl;
	cout << "file = " << f.filepath << endl;
	for (int i = 0; i < f.pose.size(); i++) {
		cout << "P " << i << " : " << setprecision(15) << f.pose.at(i) << endl;
	}

	// layer -> neuron -> weight
	std::vector<std::vector<double>> topology;

    // e.g., { 3, 2, 1 }
    //std::vector<unsigned> topology;
	try
	{
		topology = dl->GetTopology();
		cout << "topology loaded succesfully " << endl;
		cout << "first weight = " << topology.at(0).at(0) << endl;
	}
	catch (const std::exception&)
	{
		// layer 1
		topology.push_back(randomNeurons(36)); // pose coordinates
		topology.push_back(randomNeurons(32)); // black box layer 1
		topology.push_back(randomNeurons(32)); // black box layer 2
		topology.push_back(randomNeurons(32)); // black box layer 3
		topology.push_back(randomNeurons(3));	// output emotions	
		cout << "using random topology" << endl;
	}

    Net myNet(topology);
	std::vector<double> inputVals, resultVals;
	std::vector<double> targetVals(10, 0.0);
	inputVals = f.pose;
	myNet.feedForward(inputVals);

	if (f.label == "disgust") {
		targetVals.at(0) = 1;
	}
	else if (f.label == "boredom") {
		targetVals.at(1) = 1;
	}
	else if (f.label == "anger") {
		targetVals.at(2) = 1;
	}
	else if (f.label == "contempt") {
		targetVals.at(3) = 1;
	}
	else if (f.label == "happiness") {
		targetVals.at(4) = 1;
	}
	else if (f.label == "interest") {
		targetVals.at(5) = 1;
	}
	else if (f.label == "sadness") {
		targetVals.at(6) = 1;
	}
	else if (f.label == "fear") {
		targetVals.at(7) = 1;
	}
	else if (f.label == "neutral") {
		targetVals.at(8) = 1;
	}
	else if (f.label == "surprise") {
		targetVals.at(9) = 1;
	}

    myNet.backProp(targetVals);
    myNet.getResults(resultVals);

	dl->SaveTopology(topology);

    // Report how well the training is working, average over recent samples:
    std::cout << "Net recent average error: "
        << myNet.getRecentAverageError() << std::endl;

    std::cout << std::endl << "Done" << std::endl;
}

