#include "Track.h"
#include <iostream>
#include <fstream>
#include <string>
#include "utilities.h"
#include "TrackPoints.h"


Track::Track() : Fl_Widget{0, 0, winW, winH}
{
    // Create two arrays with all the points for left and right walls
    std::vector<std::pair<int, int>> leftPoints;
    std::vector<std::pair<int, int>> rightPoints;
    for (int i = 0; i < 15; i++)
    {
        int x, y;
        x = leftBoundary[i].first;
        y = leftBoundary[i].second;
        leftPoints.push_back({x, y});

        x = rightBoundary[i].first;
        y = rightBoundary[i].second;
        rightPoints.push_back({x, y});
    }
    
    
    for (int i = 1; i < 15; i++)
    {
        int x0, y0, x1, y1;
        x0 = leftPoints[i - 1].first;
        y0 = leftPoints[i - 1].second;
        x1 = leftPoints[i].first;
        y1 = leftPoints[i].second;
        leftWalls.push_back(new Tile{x0, y0, x1, y1});

        x0 = rightPoints[i - 1].first;
        y0 = rightPoints[i - 1].second;
        x1 = rightPoints[i].first;
        y1 = rightPoints[i].second;
        rightWalls.push_back(new Tile{x0, y0, x1, y1});
    }
}


Track::~Track()
{
}


void Track::draw()
{
    return;
}