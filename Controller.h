#pragma once
#include <FL/Fl_Double_Window.H>
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include "Car.h"
#include "Track.h"
#include "utilities.h"
#include "NN.h"



class Controller
{
private:
    Fl_Double_Window* win;

    Track* track;

    std::vector<Car*> cars;
    std::pair<double, double> startPos = { 160, winH - 30};
    
    void checkCars();
    void restart();
    std::string pickTrack();

    std::chrono::steady_clock::time_point startTime;
    
public:
    Controller();
    ~Controller();

    void run();
};
