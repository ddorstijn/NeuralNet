#pragma once

#include <vector>

typedef std::vector<class Neuron> Layer;

struct Connection
{
    double* weight;
    double deltaWeight;
};

class Neuron
{
public:
    Neuron(unsigned numOutputs, unsigned index, double* weight);
    void setOutputVal(double val) { outputVal = val; }
    double getOutputVal(void) const { return outputVal; }
    void feedForward(const Layer &prevLayer);
    void calcOutputGradients(double targetVal);
    void calcHiddenGradients(const Layer &nextLayer);
    void updateInputWeights(Layer &prevLayer);

private:
    static double eta;   // [0.0..1.0] overall net training rate
    static double alpha; // [0.0..n] multiplier of last weight change (momentum)
    static double transferFunction(double x);
    static double transferFunctionDerivative(double x);
    // static double randomWeight(void) { return rand() / double(RAND_MAX); }
    double sumDOW(const Layer &nextLayer) const;
    double outputVal;
    std::vector<Connection> outputWeights;
    unsigned myIndex;
    double gradient;
};