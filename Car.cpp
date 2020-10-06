#include "Car.h"
#include <string>
#include <cmath>
#include <Eigen/Dense>


Car::Car(Track* track, double x, double y, double vel, double angle, bool manualControl) : 
	Fl_Widget{ 0, 0, winW, winH }, track{track}, phys{ x,  y, vel, angle, 13, 13}, 
    brain{2, {6, 2}}, manualControl{manualControl}
{
	rayDistances = { 0, 0, 0, 0, 0 };
	for (int i = 0; i < 5; i++)
	{
		rayDistances[i] = (static_cast<double>(rayLengthSq));
	}

	// Setup rays
	std::pair<int, int> L, LF, F, RF, R;
	L.first = phys.x - rayLength * cos((90 - phys.angle) * DEGTORAD);
	L.second = phys.y - rayLength * sin((90 - phys.angle) * DEGTORAD);
	rays.push_back(L);
	LF.first = phys.x - rayLength * cos((90 - phys.angle + 45) * DEGTORAD);
	LF.second = phys.y - rayLength * sin((90 - phys.angle + 45) * DEGTORAD);
	rays.push_back(LF);
	F.first = phys.x - rayLength * cos((90 - phys.angle + 90) * DEGTORAD);
	F.second = phys.y - rayLength * sin((90 - phys.angle + 90) * DEGTORAD);
	rays.push_back(F);
	RF.first = phys.x - rayLength * cos((90 - phys.angle + 135) * DEGTORAD);
	RF.second = phys.y - rayLength * sin((90 - phys.angle + 135) * DEGTORAD);
	rays.push_back(RF);
	R.first = phys.x - rayLength * cos((90 - phys.angle + 180) * DEGTORAD);
	R.second = phys.y - rayLength * sin((90 - phys.angle + 180) * DEGTORAD);
	rays.push_back(R);


	brain.randomizeBiases();
    brain.randomizeWeights();
    setBrainInput();
}


Car::~Car()
{
    std::cout << "Car eliminated!" << std::endl;
}


void Car::draw()
{
	// Draw rays
	drawRays();
	// Calculate all the distances for the rays
	calculateRays();
	// Draw the car
    drawCar();
	

	// Choice of steering algorithm
	if (manualControl)
	{
		manualController();
	}
	else
	{
		AIController();
	}

	checkCollision();
	

	phys.x += phys.vel * cos(phys.angle * DEGTORAD);
	phys.y -= phys.vel * sin(phys.angle * DEGTORAD);
}


bool Car::checkCollision()
{
	// Check if any of the rays has a length less than the car with
	for (int i = 0; i < 5; i++)
	{
		if (rayDistances[i] < pow(phys.carWidth, 2))
		{
			isAlive = false;
			return true;
		}
	}

	return false;
}


void Car::AIController()
{
	// calculate movement from brain
    setBrainInput();
    brain.propagate();
    Eigen::VectorXd out = brain.getOutput();
    if (out(0) > 0.6)
    {
        forward();
		hasMoved = true;
    }
	else if (out(0) < 0.4)
	{
		backward();
		hasMoved = true;
	}
	
    if (out(1) > 0.6)
    {
        phys.angle -= 2;
    }
	else if (out(1) < 0.4)
	{
		phys.angle += 2;
	}
	
}


void Car::manualController()
{
	if(Fl::event_key(FL_Up))
	{
		forward();
	}
	if(Fl::event_key(FL_Down))
	{
		backward();
	}
	if(Fl::event_key(FL_Right))
	{
		phys.angle -= 2;
	}
	if(Fl::event_key(FL_Left))
	{
		phys.angle += 2;
	}
}


void Car::forward()
{
	if(phys.vel <= maxSpeed)
	{
		phys.vel += 0.01;
	}
}


void Car::backward()
{
	if(phys.vel >= -maxSpeed)
	{
		phys.vel -= 0.01;
	}
}


void Car::drawCar()
{
	double lineWidth = 2;
    fl_push_matrix();
	// Translate to center of car and rotate
    fl_translate(phys.x, phys.y);
    fl_rotate(phys.angle);
	// Black border for car
	fl_color(FL_BLACK);
	fl_begin_polygon();
	fl_vertex((-phys.carWidth / 2.0), (-phys.carWidth / 2.0));
    fl_vertex(( phys.carWidth / 2.0), (-phys.carWidth / 2.0));
    fl_vertex(( phys.carWidth / 2.0), ( phys.carWidth / 2.0));
    fl_vertex((-phys.carWidth / 2.0), ( phys.carWidth / 2.0));
	fl_end_polygon();
	// Blue fill for car
    fl_color(col);
    fl_begin_polygon();
	fl_vertex((-(phys.carWidth - lineWidth) / 2.0), (-(phys.carWidth - lineWidth) / 2.0));
    fl_vertex(( (phys.carWidth - lineWidth) / 2.0), (-(phys.carWidth - lineWidth) / 2.0));
    fl_vertex(( (phys.carWidth - lineWidth) / 2.0), ( (phys.carWidth - lineWidth) / 2.0));
    fl_vertex((-(phys.carWidth - lineWidth) / 2.0), ( (phys.carWidth - lineWidth) / 2.0));
	fl_end_polygon();
    fl_pop_matrix();


    /*
    fl_push_matrix();					// Begin matrix
	fl_translate(phys.x, phys.y);		// Translate to car position
	fl_rotate(phys.angle);				// Rotate to cars current angle

	// This is mostly a lot of hardcoded points, but in the end it looks like a car
	// Car body
	fl_color(FL_RED);
	fl_begin_polygon();
	fl_vertex(-phys.carWidth, phys.carWidth);
	fl_vertex(phys.carWidth, phys.carWidth);
	fl_vertex(phys.carWidth, -phys.carWidth);
	fl_vertex(-phys.carWidth, -phys.carWidth);
	fl_end_polygon();

	// Wheels
	fl_color(FL_BLACK);
	fl_begin_polygon();	// Back Left
	fl_vertex(-phys.carWidth + 2, -phys.carWidth);
	fl_vertex(-phys.carWidth + 10, -phys.carWidth);
	fl_vertex(-phys.carWidth + 10, -phys.carWidth + 2);
	fl_vertex(-phys.carWidth + 2, -phys.carWidth + 2);
	fl_end_polygon();
	fl_begin_polygon();	// Front Left
	fl_vertex(phys.carWidth - 2, -phys.carWidth);
	fl_vertex(phys.carWidth - 10, -phys.carWidth);
	fl_vertex(phys.carWidth - 10, -phys.carWidth + 2);
	fl_vertex(phys.carWidth - 2, -phys.carWidth + 2);
	fl_end_polygon();
	fl_begin_polygon(); // Back Right
	fl_vertex(-phys.carWidth + 2, phys.carWidth);
	fl_vertex(-phys.carWidth + 10, phys.carWidth);
	fl_vertex(-phys.carWidth + 10, phys.carWidth - 2);
	fl_vertex(-phys.carWidth + 2, phys.carWidth - 2);
	fl_end_polygon();
	fl_begin_polygon();	// Front Right
	fl_vertex(phys.carWidth - 2, phys.carWidth);
	fl_vertex(phys.carWidth - 10, phys.carWidth);
	fl_vertex(phys.carWidth - 10, phys.carWidth - 2);
	fl_vertex(phys.carWidth - 2, phys.carWidth - 2);
	fl_end_polygon();

	// Window
	fl_color(179, 255, 255); // Light blue
	fl_begin_polygon();
	fl_vertex(phys.carWidth - 3, -phys.carWidth + 4);
	fl_vertex(phys.carWidth - 3, phys.carWidth - 4);
	fl_vertex(phys.carWidth - 11, phys.carWidth - 4);
	fl_vertex(phys.carWidth - 11, -phys.carWidth + 4);
	fl_end_polygon();

	// Spoiler
	fl_color(140, 0, 0); // Dark red
	fl_begin_polygon();
	fl_vertex(-phys.carWidth + 3, -phys.carWidth + 4);
	fl_vertex(-phys.carWidth + 3, phys.carWidth - 4);
	fl_vertex(-phys.carWidth + 6, phys.carWidth - 4);
	fl_vertex(-phys.carWidth + 6, -phys.carWidth + 4);
	fl_end_polygon();
	fl_color(80, 0, 0);	// Darker red
	fl_begin_polygon();
	fl_vertex(-phys.carWidth + 6, -phys.carWidth + 8);
	fl_vertex(-phys.carWidth + 6, -phys.carWidth + 10);
	fl_vertex(-phys.carWidth + 10, -phys.carWidth + 8);
	fl_vertex(-phys.carWidth + 10, -phys.carWidth + 10);
	fl_end_polygon();
	fl_begin_polygon();
	fl_vertex(-phys.carWidth + 6, phys.carWidth - 8);
	fl_vertex(-phys.carWidth + 6, phys.carWidth - 10);
	fl_vertex(-phys.carWidth + 10, phys.carWidth - 8);
	fl_vertex(-phys.carWidth + 10, phys.carWidth - 10);
	fl_end_polygon();

	// Exhaust
	for (int i = 0; i < 15; i++) {
		int lim = abs(static_cast<int>(phys.vel * 15)) + 1;		// Max length of trailing exhaus
		int x = rand() % lim + phys.carWidth;					// Random placement behind car
		int y = rand() % 10 - 5;								// Random placement behind car
		fl_color(140 + (2*x/3), 140 + (2*x/3), 140 + (2*x/3));	// Lighter grey if distance is greater
		fl_begin_complex_polygon();
		fl_arc(-x, y, x/10.0, 0, 360);							// Cirle for smoke
		fl_end_complex_polygon();
	}

	fl_pop_matrix();	// End matrix
    */
}


void Car::drawRays()
{
	for (int i = 0; i < rays.size(); i++)
	{
		rays[i].first = phys.x - rayLength * cos((90 - phys.angle + i * 45) * DEGTORAD);
		rays[i].second = phys.y - rayLength * sin((90 - phys.angle + i * 45) * DEGTORAD);
	}

	fl_color(50, 50, 50);	// Grey
	for (int i = 0; i < rays.size(); i++)
	{
		fl_line(phys.x, phys.y, rays[i].first, rays[i].second);
	}
}


void Car::calculateRays()
{
	for (int i = 0; i < rays.size(); i++)
	{
		// Ray start and end
		std::pair<int, int> p1 = { phys.x, phys.y };				// Ray, start
		std::pair<int, int> q1 = { rays[i].first, rays[i].second };	// Ray, end
		
		bool intersectLeft, intersectRight;

		for (int j = 0; j < track->leftWalls.size(); j++)
		{
			// LEFT WALLS
			std::pair<int, int> p2L = { track->leftWalls[j]->x0, track->leftWalls[j]->y0 };	// Left wall, start
			std::pair<int, int> q2L = { track->leftWalls[j]->x1, track->leftWalls[j]->y1 };	// Left wall, end
			// RIGHT WALLS
			std::pair<int, int> p2R = { track->rightWalls[j]->x0, track->rightWalls[j]->y0 };	// Right wall, start
			std::pair<int, int> q2R = { track->rightWalls[j]->x1, track->rightWalls[j]->y1 };	// Right wall, end
			// Intersect?
			intersectLeft = doIntersect(p1, q1, p2L, q2L);
			intersectRight = doIntersect(p1, q1, p2R, q2R);
			if (intersectLeft && i == 2)
			{
				fl_color(FL_GREEN);
				fl_rect(10, 10, 10, 10);
			}
			if (intersectRight && i == 2)
			{
				fl_color(FL_RED);
				fl_rect(25, 10, 10, 10);
			}
			
			if (intersectLeft)
			{
				//track->leftWalls[i]->setColor(FL_GREEN);
				std::pair<int, int> collPoint = lineLineIntersection(p1, q1, p2L, q2L);
				fl_color(FL_RED);
				fl_circle(collPoint.first, collPoint.second, 5);

				double distSquared = pow(phys.x - collPoint.first, 2) + pow(phys.y - collPoint.second, 2);
				rayDistances[i] = distSquared;
				break;
			}

			if (intersectRight)
			{
				//track->rightWalls[i]->setColor(FL_GREEN);
				std::pair<int, int> collPoint = lineLineIntersection(p1, q1, p2R, q2R);
				fl_color(FL_RED);
				fl_circle(collPoint.first, collPoint.second, 5);

				double distSquared = pow(phys.x - collPoint.first, 2) + pow(phys.y - collPoint.second, 2);
				rayDistances[i] = distSquared;
				break;
			}
		}
		if (!(intersectLeft || intersectRight))
		{
			rayDistances[i] = static_cast<double>(rayLengthSq);
		}
	}
}


void Car::setBrainInput()
{
	// Inputs are velocity and the 5 ray distances
	// Subscript n means that the value is normalized (from 0 to 1)
	Eigen::VectorXd inputs(6);
	double vel_n = phys.vel / ( 2 * maxSpeed) + 0.5;
	double ray0_n = rayDistances[0] / static_cast<double>(rayLengthSq);
	double ray1_n = rayDistances[1] / static_cast<double>(rayLengthSq);
	double ray2_n = rayDistances[2] / static_cast<double>(rayLengthSq);
	double ray3_n = rayDistances[3] / static_cast<double>(rayLengthSq);
	double ray4_n = rayDistances[4] / static_cast<double>(rayLengthSq);

	inputs << vel_n, ray0_n, ray1_n, ray2_n, ray3_n, ray4_n;

	brain.addInput(inputs);
}