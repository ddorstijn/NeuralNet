#include <vector>
#include <iostream>

#include "DataLoader.h"
#include "Net.hpp"
#include "Neuron.hpp"
#include <time.h>

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
    // e.g., { 3, 2, 1 }
    std::vector<unsigned> topology;
 //   topology.push_back(36); // pose coordinates
 //   topology.push_back(32); // black box layer 1
 //   topology.push_back(32); // black box layer 2
	//topology.push_back(32); // black box layer 3
	//topology.push_back(3);	// output emotions
 //   Net myNet(topology);

 //   std::vector<double> inputVals, targetVals, resultVals;
 //   inputVals.push_back(1.2);
 //   inputVals.push_back(1.2);
 //   inputVals.push_back(1.2);
 //   myNet.feedForward(inputVals);

 //   targetVals.push_back(1);
 //   myNet.backProp(targetVals);
 //   myNet.getResults(resultVals);

 //   // Report how well the training is working, average over recent samples:
 //   std::cout << "Net recent average error: "
 //       << myNet.getRecentAverageError() << std::endl;

    std::cout << std::endl << "Done" << std::endl;
}

