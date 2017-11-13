#include <vector>
#include <iostream>

#include "Net.hpp"
#include "Neuron.hpp"

int main()
{
    // e.g., { 3, 2, 1 }
    std::vector<unsigned> topology;
    topology.push_back(3);
    topology.push_back(2);
    topology.push_back(1);
    Net myNet(topology);

    std::vector<double> inputVals, targetVals, resultVals;
    inputVals.push_back(1.2);
    inputVals.push_back(1.2);
    inputVals.push_back(1.2);
    myNet.feedForward(inputVals);

    targetVals.push_back(1);
    myNet.backProp(targetVals);
    myNet.getResults(resultVals);

    // Report how well the training is working, average over recent samples:
    std::cout << "Net recent average error: "
        << myNet.getRecentAverageError() << std::endl;

    std::cout << std::endl << "Done" << std::endl;
}
