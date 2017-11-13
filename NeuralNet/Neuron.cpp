#include "Neuron.hpp"


double Neuron::eta = 0.15;    // overall net learning rate, [0.0..1.0]
double Neuron::alpha = 0.5;   // momentum, multiplier of last deltaWeight, [0.0..1.0]


void Neuron::updateInputWeights(Layer &prevLayer)
{
    // The weights to be updated are in the Connection container
    // in the neurons in the preceding layer

    for (unsigned n = 0; n < prevLayer.size(); ++n) {
        Neuron &neuron = prevLayer[n];
        double oldDeltaWeight = neuron.outputWeights[myIndex].deltaWeight;

        double newDeltaWeight =
            // Individual input, magnified by the gradient and train rate:
            eta
            * neuron.getOutputVal()
            * gradient
            // Also add momentum = a fraction of the previous delta weight;
            + alpha
            * oldDeltaWeight;

        neuron.outputWeights[myIndex].deltaWeight = newDeltaWeight;
        neuron.outputWeights[myIndex].weight += newDeltaWeight;
    }
}

double Neuron::sumDOW(const Layer &nextLayer) const
{
    double sum = 0.0;

    // Sum our contributions of the errors at the nodes we feed.

    for (unsigned n = 0; n < nextLayer.size() - 1; ++n) {
        sum += outputWeights[n].weight * nextLayer[n].gradient;
    }

    return sum;
}

void Neuron::calcHiddenGradients(const Layer &nextLayer)
{
    double dow = sumDOW(nextLayer);
    gradient = dow * Neuron::transferFunctionDerivative(outputVal);
}

void Neuron::calcOutputGradients(double targetVal)
{
    double delta = targetVal - outputVal;
    gradient = delta * Neuron::transferFunctionDerivative(outputVal);
}

double Neuron::transferFunction(double x)
{
    // tanh - output range [-1.0..1.0]

    return tanh(x);
}

double Neuron::transferFunctionDerivative(double x)
{
    // tanh derivative
    return 1.0 - x * x;
}

void Neuron::feedForward(const Layer &prevLayer)
{
    double sum = 0.0;

    // Sum the previous layer's outputs (which are our inputs)
    // Include the bias node from the previous layer.

    for (unsigned n = 0; n < prevLayer.size(); ++n) {
        sum += prevLayer[n].getOutputVal() *
            prevLayer[n].outputWeights[myIndex].weight;
    }

    outputVal = Neuron::transferFunction(sum);
}

Neuron::Neuron(unsigned numOutputs, unsigned index)
{
    for (unsigned c = 0; c < numOutputs; ++c) {
        outputWeights.push_back(Connection());
        outputWeights.back().weight = randomWeight();
    }

    myIndex = index;
}
