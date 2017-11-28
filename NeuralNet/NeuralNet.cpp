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

    std::vector<double> inputVals, targetVals, resultVals;
    inputVals = f.pose;
    myNet.feedForward(inputVals);

    if (f.label == "disgust") {
        targetVals.push_back(1);
        targetVals.push_back(0);
        targetVals.push_back(0);
    }

    myNet.backProp(targetVals);
    myNet.getResults(resultVals);

	dl->SaveTopology(topology);

    // Report how well the training is working, average over recent samples:
    std::cout << "Net recent average error: "
        << myNet.getRecentAverageError() << std::endl;

    std::cout << std::endl << "Done" << std::endl;
}

