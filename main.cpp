#include <iostream>
#include "Controller.h"

//------------------------------------------------------------------------------'

// C++ programs start by executing the function main
int main()
{
	srand(time(NULL));					// Seed random
	std::cout.sync_with_stdio(false);	// Optimize printing
	std::cout << "Starting up..." << std::endl;

	Controller controller;
	controller.run();
}

//------------------------------------------------------------------------------
