#include "NN.h"
#include <iostream>
#include <fstream>
#include <string>
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


// Returns a copy of a Neural Network consists of (this*weight) and (other*(1-weight))
NeuralNetwork NeuralNetwork::combine(NeuralNetwork& other, double weight)
{
    // Assuming that the dimensions of the two neural networks are the same
    NeuralNetwork combined = *this; // Make a copy
    for (int i = 0; i < weights.size(); i++)
    {
        combined.weights[i] = this->weights[i] * weight + other.weights[i] * (1-weight);
    }
    for (int i = 0; i < biases.size(); i++)
    {
        combined.biases[i] = this->biases[i] * weight + other.biases[i] * (1-weight);
    }
    return combined;
}


void NeuralNetwork::saveToFile()
{
    // Open/create file
    std::ofstream ofs;
    ofs.open("NN.txt");
    if (!ofs)
    {
        std::cerr << "*** Could not create file to save NN ***" << std::endl;
    }

    // Write number of layers
    ofs << "Layers " << layers << "\n";

    // Write number of nodes in each layer
    ofs << "NodesInLayers ";
    for (int i = 0; i < layers; i++)
    {
        ofs << activations[i].size() << " ";
    }
    ofs << "\n\n";

    // Write all activations
    ofs << "Activations" << "\n";
    for (int i = 0; i  < activations.size(); i++)
    {
        ofs << activations[i] << "\n\n";
    }

    // Write all weights
    ofs << "Weights" << "\n";
    for (int i = 0; i  < weights.size(); i++)
    {
        ofs << weights[i] << "\n\n";
    }

    // Write all biases
    ofs << "Biases" << "\n";
    for (int i = 0; i  < biases.size(); i++)
    {
        ofs << biases[i] << "\n\n";
    }

    ofs.close();
}


void NeuralNetwork::loadFromFile()
{
    // Reset excisting NN
    layers = 0;
    nodesInLayers.clear();
    activations.clear();
    weights.clear();
    biases.clear();

    std::ifstream ifs;
    ifs.open("NN.txt");
    if (!ifs)
    {
        std::cerr << "*** Could not open file to load NN ***" << std::endl;
    }

    // Dummy variable used to store text
    std::string dummy;

    // Read number of layers
    ifs >> dummy;   // = "Layers"
    ifs >> layers;

    // Find number of nodes in each layer
    ifs >> dummy;   // = "NodesInLayers"
    std::string line;
    std::getline(ifs, line);
    std::stringstream ss(line);
    int n;  // Nodes in current layes
    while (ss >> n)
    {
        nodesInLayers.push_back(n);
    }
    
    // Setup a zero-initialized NN of correct size/dimensions
    activations.push_back(Eigen::VectorXd::Zero(nodesInLayers[0]));
    for (int i = 1; i < layers; i++)
    {
        activations.push_back(Eigen::VectorXd::Zero(nodesInLayers[i]));
        biases.push_back(Eigen::VectorXd::Zero(nodesInLayers[i]));
        weights.push_back(Eigen::MatrixXd::Zero(nodesInLayers[i], nodesInLayers[i-1]));
    }
    
    // Find the actications
    ifs >> dummy;   // = "Activations"
    std::cout << dummy << std::endl;
    for (int i = 0; i < layers; i++)
    {
        for (int j = 0; j < nodesInLayers[i]; j++)
        {
            // Read values and put into activation-vectors
            double num;
            ifs >> num;
            activations[i][j] = num;
        }
    }

    // Find the weights
    ifs >> dummy;   // = "Weights"
    for (int i = 0; i < layers - 1; i++)
    {
        for (int row = 0; row < weights[i].rows(); row++)
        {
            for (int col = 0; col < weights[i].cols(); col++)
            {
                // Read values and put into weight-matrices
                double num;
                ifs >> num;
                weights[i](row, col) = num;
            }
        }
    }

    // Find the biases
    ifs >> dummy;   // = "Biases"
    std::cout << dummy << std::endl;
    for (int i = 0; i < layers - 1; i++)
    {
        for (int j = 0; j < nodesInLayers[i + 1]; j++)
        {
            // Read values and put into bias-vectors
            double num;
            ifs >> num;
            biases[i][j] = num;
        }
    }
    
    ifs.close();
    std::cout << dummy << std::endl;
}