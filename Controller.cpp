#include "Controller.h"
#include <iostream>


Controller::Controller() : win{new Fl_Double_Window{50, 50, winW, winH}},
    track{new Track{}}
{
	// Start a timer
    startTime = std::chrono::steady_clock::now();

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
	// Using an iterator foor loop to erease dead cars or cars that hasn't moved first 3 seconds
	auto nowTime = std::chrono::steady_clock::now();
	auto elapsedTime = nowTime - startTime;
	bool lastDead = false;
	std::cout << "Cars left: " << elapsedTime.count() << std::endl;

	for (auto it = cars.begin(); it != cars.end();)
	{
		if ((!(*it)->isAlive) || ((elapsedTime.count() > 3000000000 && elapsedTime.count() < 3500000000) && ((*it)->phys.y > winH - 100)) || (*it)->getY() > winH)
		{
			if (cars.size() == 1)
			{
				lastDead = true;
				break;
			}
			delete *it;
			it = cars.erase(it);
		}
		else
		{
			it++;
		}
	}

	if (lastDead)
	{
		restart();
	}
}


void Controller::restart()
{
	Car* lastCar = cars[0];
	cars.clear();
	NeuralNetwork masterBrain = lastCar->getBrain();
	
	delete lastCar;

	for (int i = 0; i < 50; i++)
	{
		cars.push_back(new Car{track, 160, winH - 30, 0, 90, false});
		NeuralNetwork newBrain = masterBrain;
		newBrain.mutate();
		cars[cars.size() - 1]->setBrain(newBrain);
		win->add(cars[cars.size() - 1]);
	}
	win->show();
	startTime = std::chrono::steady_clock::now();
}