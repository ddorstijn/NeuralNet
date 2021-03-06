#include <vector>
#include <iostream>

#include "DataLoader.h"
#include "Net.hpp"
#include "Neuron.hpp"
#include <time.h>
#include <thread>

std::vector<double> randomNeurons(double amount) {
	std::vector<double> layer;
	for (int i = 0; i < amount; i++) {
		layer.push_back(rand() / double(RAND_MAX)); // insert randomweight
	}
	return layer;
}

bool stop = false;
DataLoader* dl;
std::vector<std::vector<double>> topology;

void train() {
	Net myNet(topology);
	while (!stop) {
		file f = dl->LoadRandom();

		/*std::cout << "label = " << f.label << std::endl;
		std::cout << "file = " << f.filepath << std::endl;*/
		/*for (int i = 0; i < f.pose.size(); i++) {
			std::cout << "P " << i << " : " << std::setprecision(15) << f.pose.at(i) << std::endl;
		}*/

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
		/*else if (f.label == "contempt") {
			targetVals.at(3) = 1;
		}*/
		/*else if (f.label == "happiness") {
			targetVals.at(4) = 1;
		}*/
		/*else if (f.label == "interest") {
			targetVals.at(5) = 1;
		}*/
		else if (f.label == "sadness") {
			targetVals.at(3) = 1;
		}
		else if (f.label == "fear") {
			targetVals.at(4) = 1;
		}
		else if (f.label == "neutral") {
			targetVals.at(5) = 1;
		}
		else if (f.label == "surprise") {
			targetVals.at(6) = 1;
		}

		myNet.backProp(targetVals);
		myNet.getResults(resultVals);

		// Report how well the training is working, average over recent samples:
		std::cout << "Net recent average error: "
			<< myNet.getRecentAverageError() << std::endl;

	}
}

int main()
{
	srand(time(NULL));
	dl = new DataLoader();
	
	std::vector<file> testSet, trainingSet;
	dl->Load(testSet, trainingSet);

	// layer -> neuron -> weight

    // e.g., { 3, 2, 1 }
    //std::vector<unsigned> topology;
	try
	{
		topology = dl->GetTopology();
		std::cout << "topology loaded succesfully " << std::endl;
		std::cout << "first weight = " << topology.at(0).at(0) << std::endl;
	}
	catch (const std::exception&)
	{
		// layer 1
		topology.push_back(randomNeurons(36)); // pose coordinates
		topology.push_back(randomNeurons(32)); // black box layer 1
		topology.push_back(randomNeurons(32)); // black box layer 2
		topology.push_back(randomNeurons(32)); // black box layer 3
		topology.push_back(randomNeurons(7));	// output emotions	
		std::cout << "using random topology" << std::endl;
	}
	std::cout << "press enter to start training" << std::endl;
	std::cin.get();
	std::thread t(train);
	std::cin.get();
	stop = true;
	t.join();
	std::cout << "training stopped and saved to topology.json " << std::endl;

	dl->SaveTopology(topology);

    std::cout << std::endl << "Done" << std::endl;
	std::cout << "first weight = " << topology.at(0).at(0) << std::endl;
	std::cin.get();
}

