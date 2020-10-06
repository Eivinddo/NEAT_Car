#include "NN.h"
#include <iostream>
#include "utilities.h"


NeuralNetwork::NeuralNetwork(int layers, std::vector<int> nodesInLayers)
    : layers{layers}, nodesInLayers{nodesInLayers}
{
    // Set inputs to 0
    activations.push_back(Eigen::VectorXd::Zero(nodesInLayers[0]));
    // Set all activations, biases and weights to 0
    for (int i = 1; i < layers; i++)
    {
        activations.push_back(Eigen::VectorXd::Zero(nodesInLayers[i]));
        biases.push_back(Eigen::VectorXd::Zero(nodesInLayers[i]));
        weights.push_back(Eigen::MatrixXd::Zero(nodesInLayers[i], nodesInLayers[i-1]));
    }
}


NeuralNetwork::~NeuralNetwork()
{
}


void NeuralNetwork::randomizeWeights()
{
    for (int i = 0; i < weights.size(); i++)
    {
        weights[i] = Eigen::MatrixXd::Random(nodesInLayers[i+1], nodesInLayers[i]);
    }
}


void NeuralNetwork::randomizeBiases()
{
    for (int i = 0; i < biases.size(); i++)
    {
        biases[i] = Eigen::VectorXd::Random(biases[i].size(), 1);
    }
}


void NeuralNetwork::propagate()
{
    // First layer is inputs so we start calculating the second
    for (int i = 1; i < activations.size(); i++)
    {
        activations[i] = weights[i-1]*activations[i-1] + biases[i-1];
        // Sigmoid everything:
        for (int j = 0; j < activations[i].size(); j++)
        {
            activations[i](j) = sigmoid(activations[i](j));
        }
    }
}


double mutateNumber(double num)
{
    if (rand()  < mutationRate * RAND_MAX)
    {
        // Generate random double in the range [0, 0.5]
        double nudge = static_cast<double>(rand()) / (2 * RAND_MAX);
        int sign = rand() % 2 ? 1 : -1;
        num += nudge * sign;
        constrain(num, -1, 1);
        return num;
    }
    else
    {
        return num;
    }
}


void NeuralNetwork::mutate()
{
    for (int i = 0; i < biases.size(); i++)
    {
        Eigen::MatrixXd& m = weights[i];
        m = m.unaryExpr(&mutateNumber);
    }
}