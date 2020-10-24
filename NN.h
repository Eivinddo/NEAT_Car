#pragma once
#include <vector>
#include <Eigen/Dense>

constexpr double mutationRate = 0.3;

class NeuralNetwork
{
    int layers;
    std::vector<int> nodesInLayers;
    std::vector<Eigen::VectorXd> activations;
    std::vector<Eigen::MatrixXd> weights;
    std::vector<Eigen::VectorXd> biases;

public:
    NeuralNetwork(int layers, std::vector<int> nodesInLayers);
    ~NeuralNetwork();
    void randomizeWeights();
    void randomizeBiases();
    void addInput(Eigen::VectorXd inputs) { activations[0] = inputs; }
    void propagate();
    //friend double mutateNumber(double num);
    void mutate();
    void saveToFile();
    void loadFromFile();
    std::vector<Eigen::VectorXd> getActivations() { return activations; }
    std::vector<Eigen::MatrixXd> getWeights() { return weights; }
    std::vector<Eigen::VectorXd> getBiases() { return biases; }
    Eigen::VectorXd getOutput() { return activations[layers - 1]; }
};