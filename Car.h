#pragma once
#include <utility>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <iostream>
#include "Track.h"
#include "utilities.h"
#include "NN.h"


// Forward declearing the track-class
class Track;


struct Physics {
    double x;
    double y;
    double vel;
    double angle;

    double carWidth = 13;
    double carHeight = 13;
};


class Car : public Fl_Widget
{
	Fl_Color col = FL_BLUE;
    double maxSpeed = 3.0;
    int rayLength = 150;
    int rayLengthSq = rayLength * rayLength;
    bool manualControl;
    double distanceTravelled;

    Track* track;
    std::vector<std::pair<int, int>> rays;  // Distance-measuring rays from car
    std::vector<double> rayDistances;

    NeuralNetwork brain;

    void checkEvents();
    void drawCar();
    void drawRays();
    void calculateRays();
    void manualController();
    void AIController();
    bool checkCollision();

public:
    bool isAlive = true;
    bool hasMoved = false;
    Physics phys;

	Car(Track* track, double x, double y, double vel, double angle, bool manualControl = true);
	~Car();

	void draw() override;

    void forward();
    void backward();

	void setColor(Fl_Color _col) { col = _col; }
    int closestLineIndex();
    void setBrainInput();
    void setBrain(NeuralNetwork newBrain) { brain = newBrain; };

    Fl_Color getColor() const { return col; }
    double getX() { return phys.x; }
    double getY() { return phys.y; }
    int getWidth() { return phys.carWidth; }
    int getHeight() { return phys.carHeight; }
    std::pair<double, double> getPos() { return {phys.x, phys.y}; }
    NeuralNetwork getBrain() { return brain; }
};
