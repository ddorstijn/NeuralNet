#include "Net.hpp"
#include <cassert>
#include <iostream>


double Net::recentAverageSmoothingFactor = 100.0; // Number of training samples to average over


void Net::getResults(std::vector<double> &resultVals) const
{
    resultVals.clear();

    for (unsigned n = 0; n < layers.back().size() - 1; ++n) {
        resultVals.push_back(layers.back()[n].getOutputVal());
    }
}

void Net::backProp(const std::vector<double> &targetVals)
{
    // Calculate overall net error (RMS of output neuron errors)

    Layer &outputLayer = layers.back();
    error = 0.0;

    for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
        double delta = targetVals[n] - outputLayer[n].getOutputVal();
        error += delta * delta;
    }
    error /= outputLayer.size() - 1; // get average error squared
    error = sqrt(error); // RMS

                             // Implement a recent average measurement

    recentAverageError =
        (recentAverageError * recentAverageSmoothingFactor + error)
        / (recentAverageSmoothingFactor + 1.0);

    // Calculate output layer gradients

    for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
        outputLayer[n].calcOutputGradients(targetVals[n]);
    }

    // Calculate hidden layer gradients

    for (unsigned layerNum = layers.size() - 2; layerNum > 0; --layerNum) {
        Layer &hiddenLayer = layers[layerNum];
        Layer &nextLayer = layers[layerNum + 1];

        for (unsigned n = 0; n < hiddenLayer.size(); ++n) {
            hiddenLayer[n].calcHiddenGradients(nextLayer);
        }
    }

    // For all layers from outputs to first hidden layer,
    // update connection weights

    for (unsigned layerNum = layers.size() - 1; layerNum > 0; --layerNum) {
        Layer &layer = layers[layerNum];
        Layer &prevLayer = layers[layerNum - 1];

        for (unsigned n = 0; n < layer.size() - 1; ++n) {
            layer[n].updateInputWeights(prevLayer);
        }
    }
}

void Net::feedForward(const std::vector<double> &inputVals)
{
    assert(inputVals.size() == layers[0].size() - 1);

    // Assign (latch) the input values into the input neurons
    for (unsigned i = 0; i < inputVals.size(); ++i) {
        layers[0][i].setOutputVal(inputVals[i]);
    }

    // forward propagate
    for (unsigned layerNum = 1; layerNum < layers.size(); ++layerNum) {
        Layer &prevLayer = layers[layerNum - 1];
        for (unsigned n = 0; n < layers[layerNum].size() - 1; ++n) {
            layers[layerNum][n].feedForward(prevLayer);
        }
    }
}

Net::Net(const std::vector<std::vector<double>> &topology)
{
    unsigned numLayers = topology.size();
    for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum) {
        layers.push_back(Layer());
        unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1].size();

        // We have a new layer, now fill it with neurons, and
        // add a bias neuron in each layer.
        for (unsigned neuronNum = 0; neuronNum <= topology[layerNum].size(); ++neuronNum) {
			double weight = neuronNum >= topology[layerNum].size() ? 1 : topology[layerNum][neuronNum];
            layers.back().push_back(Neuron(numOutputs, neuronNum, &weight));
            std::cout << "Made a Neuron!" << std::endl;
        }

        // Force the bias node's output to 1.0 (it was the last neuron pushed in this layer):
        // layers.back().back().setOutputVal(1.0);
    }
}
