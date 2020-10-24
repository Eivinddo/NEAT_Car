#include <iostream>
#include "Controller.h"

//------------------------------------------------------------------------------'

// C++ programs start by executing the function main
int main()
{
	srand(time(NULL));					// Seed random
	std::cout.sync_with_stdio(false);	// Optimize printing
	std::cout << "Starting up..." << std::endl;
	
	/*Fl_Double_Window* w = new Fl_Double_Window{50, 50, 500, 500, "Heyoo"};

	// Start window
	w->show();
	w->color(55u);
	w->end();*/

	NeuralNetwork nn{3, {6, 4, 2}};
	// Reset excisting NN
    nn.loadFromFile();

	std::cout << nn.getActivations()[0] << std::endl << std::endl;
	std::cout << nn.getActivations()[1] << std::endl;
	nn.saveToFile();

	std::cout << "Test" << std::endl;
	std::cout << "Test2" << std::endl;

	Controller controller;
	controller.run();
}

//------------------------------------------------------------------------------
