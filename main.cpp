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

	Controller controller;
	controller.run();
}

//------------------------------------------------------------------------------
