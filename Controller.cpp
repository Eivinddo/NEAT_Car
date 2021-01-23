#include "Controller.h"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include "Fitness.h"
namespace fs = std::filesystem;

Controller::Controller() : win{new Fl_Double_Window{50, 50, winW, winH}},
    track{new Track{}}
{
	// Start a timer
    startTime = std::chrono::steady_clock::now();

	// Find a random track among files
	track->loadTrack(pickTrack());

    for (int i = 0; i < 50; i++)
	{
		cars.push_back(new Car{track, startPos.first, startPos.second, 0, 90, false});
	}
	
    // Start window
	win->show();
	win->color(55u);
	//win->end();
}


Controller::~Controller()
{
}


void Controller::run()
{
    // Timestamp
    auto next = std::chrono::steady_clock::now();
	while (win->shown())
    {

		checkCars();

		// Update input and redraw elements
		Fl::check();
		Fl::redraw();
		// Sleep until next loop
		std::this_thread::sleep_until(next);
		next += std::chrono::microseconds(1000000 / 60);
	}
}


void Controller::checkCars()
{
	// Using an iterator for loop to erease dead cars or cars that hasn't moved first 3 seconds
	auto nowTime = std::chrono::steady_clock::now();
	auto elapsedTime = nowTime - startTime;
	bool lastDead = false;	// Is the last car dead
	int lastCarIndex = -1;	// Vector-index for last car
	int deadCars = 0;		// Counter for number of dead cars
	Car* lead = cars[0];	// Pointer used to find leading car
	lead->leading = true;	// Initialize first care as leading

	for (int i = 0; i < cars.size(); i++)
	{
		Car* c = cars[i];	// Define pointer to shorten notations
		if ((!c->isAlive) || ((elapsedTime.count() > 3000000000 && elapsedTime.count() < 3500000000) && (c->phys.y > winH - 100)) || c->getY() > winH || c->atFinishLine())
		{
			if (c->atFinishLine())
			{
				c->isFinished = true;
			}
			
			if (deadCars == cars.size() - 1)
			{
				lastDead = true;
				lastCarIndex = i;
				break;
			}

			c->lifeTime = elapsedTime;
			c->hide();
			c->isAlive = false;
			deadCars++;
		}
		
		if (cars[i]->phys.distance > lead->phys.distance)
		{
			lead->leading = false;
			lead = cars[i];
			lead->leading = true;
		}
	}


	if (lastDead)
	{
		cars[lastCarIndex]->getBrain().saveToFile();
		restart();
	}
}


void Controller::restart()
{
	// Calculate fitness score of all cars
	addFinishingPoints(cars);
	addDistancePoints(cars);
	addSpeedPoints(cars);

	sortByFitness(cars);	// Sorts them from highest fitness to lowest

	// Use the brains from the three best cars as basis for the next population

	NeuralNetwork NN1 = cars[0]->getBrain();	// Used for 1/2 of the cars
	NeuralNetwork NN2 = cars[1]->getBrain();	// Used for 1/4 of the cars
	NeuralNetwork NN3 = cars[2]->getBrain();	// Used for 1/4 of the cars

	for (int i = 0; i < cars.size() / 2; i++)
	{
		cars[i]->restart();					// Restart car
		NeuralNetwork newBrain = NN1;		// Copy brain
		newBrain.mutate();					// Mutate brain
		cars[i]->setBrain(newBrain);		// Give car mutated brain
	}
	for (int i = cars.size() / 2; i < 3 * cars.size() / 4; i++)
	{
		cars[i]->restart();					// Restart car
		NeuralNetwork newBrain = NN2;		// Copy brain
		newBrain.mutate();					// Mutate brain
		cars[i]->setBrain(newBrain);		// Give car mutated brain
	}
	for (int i = 3 * cars.size() / 4; i < cars.size(); i++)
	{
		cars[i]->restart();					// Restart car
		NeuralNetwork newBrain = NN3;		// Copy brain
		newBrain.mutate();					// Mutate brain
		cars[i]->setBrain(newBrain);		// Give car mutated brain
	}

	// Let one car have the original non-mutated version NN1
	cars[0]->setBrain(NN1);

	track->loadTrack(pickTrack());					// Load new track

	win->show();									// Update window
	startTime = std::chrono::steady_clock::now();	// Define start-time
}


// Return the filename of a random track that is to be used
std::string Controller::pickTrack()
{
	// Find path p of directory
	fs::path p = fs::current_path().generic_string();

	p += "/Tracks";
	auto directoryIt = fs::directory_iterator(p.generic_string());
	// Iterate through entries in directory and count
	int count = 0;
	for (auto &entryIt : directoryIt)
	{
		count++;
	}

	// Pick a random file-number
	int r = rand() % count;
	std::string filename;
	directoryIt = fs::directory_iterator(p.generic_string());
	int i = 0;
	for (auto &entryIt : directoryIt)
	{
		if (i == r)
		{
			filename = entryIt.path().filename().string();
			break;
		}
		i++;
	}
	return "Tracks/" + filename;
}