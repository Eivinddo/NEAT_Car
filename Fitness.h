#pragma once
#include <vector>
#include "Car.h"


void addFinishingPoints(std::vector<Car*> cars);

void addDistancePoints(std::vector<Car*> cars);

void addSpeedPoints(std::vector<Car*> cars);

void sortByFitness(std::vector<Car*> &cars);