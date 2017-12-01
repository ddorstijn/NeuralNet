#pragma once

#include <vector>
#include "Neuron.hpp"

class Net
{
public:
	Net(const std::vector<std::vector<double>> &topology);
    void feedForward(const std::vector<double> &inputVals);
    void backProp(const std::vector<double> &targetVals);
    void getResults(std::vector<double> &resultVals) const;
    double getRecentAverageError(void) const { return recentAverageError; }

private:
    std::vector<Layer> layers;
    double error;
    double recentAverageError;
    static double recentAverageSmoothingFactor;
};
