#include "Fitness.h"
#include <algorithm>

void addFinishingPoints(std::vector<Car*> cars)
{
    for (auto i : cars)
    {
        if (i->isFinished)
        {
            i->fitnessScore += 15;      // 10 points for finishing the track
        }
    }
}


void addDistancePoints(std::vector<Car*> cars)
{
    // If no cars finished they get a high score for a high distance
    // If a car finished, then some points are given for shorter distance
    bool anyFinished = std::any_of(cars.begin(), cars.end(), [](Car* c){ return c->isFinished; } );

    // Create a vector of index,distance pairs and sort on increasing distance
    std::vector<std::pair<int, double>> distances;
    for (int i = 0; i < cars.size(); i++)
    {
        if (!anyFinished)
        {
            distances.push_back({i, cars[i]->phys.distance});
        }
        else if (cars[i]->isFinished)
        {
            distances.push_back({i, cars[i]->phys.distance});
        }
    }
    sort(distances.begin(), distances.end(), [](auto &left, auto &right) {
        return left.second < right.second;
    });

    // None finished: Reward long distance
    if (!anyFinished)
    {
        // Give points to long-distance cars. Points are 10,9,8,... for best cars
        for (int i = 0; i < 10 && i < distances.size(); i++)
        {
            cars[distances[distances.size() - i - 1].first]->fitnessScore += 10 - i;
        }
    }
    // Some finished: Small reward (3,2,1) for short distance for the finishing cars
    else
    {
        for (int i = 0; i < 3 && i < distances.size(); i++)
        {
            cars[distances[i].first]->fitnessScore += 3 - i;
        }
    }
}


void addSpeedPoints(std::vector<Car*> cars)
{
    // For all finishing cars, the ones finishing first (in the shortest amount of time)
    // will get extra points

    // Create a vector of index,avgSpeed pairs and sort on decreasing avgSpeed
    std::vector<std::pair<int, double>> speeds;
    for (int i = 0; i < cars.size(); i++)
    {
        if (cars[i]->isFinished)
        {
            double avgSpeed = cars[i]->phys.distance / (cars[i]->lifeTime.count() / (1.0*pow(10, 9)));
            speeds.push_back({i, avgSpeed});
        }
    }
    sort(speeds.begin(), speeds.end(), [](auto &left, auto &right) {
        return left.second > right.second;
    });
    
    // Give points (10,9,8,...) to the cars with highest avg speed
    for (int i = 0; i < 10 && i < speeds.size(); i++)
    {
        cars[speeds[i].first]->fitnessScore += 10 - i;
    }
}


void sortByFitness(std::vector<Car*> &cars)
{
    // This sorts the cars in decreasing order based on fitness-score
    std::sort(cars.begin(), cars.end(), [](auto &left, auto &right) {
        return left->fitnessScore > right->fitnessScore;
    });
}