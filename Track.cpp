#include "Track.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <FL/Fl_Gl_Window.H>
#include "utilities.h"


Track::Track() : Fl_Widget{0, 0, winW, winH}
{
    
}


Track::~Track()
{
    for (int i = 0; i < leftWalls.size(); i++)
    {
        delete leftWalls[i];
    }
    for (int i = 0; i < rightWalls.size(); i++)
    {
        delete rightWalls[i];
    }
}


void Track::draw()
{
    // All the tiles/walls of the track are automatically drawn

    // Draw finish line
    fl_color(FL_BLACK);
    fl_rectf(goalX, goalY, goalW, goalH);
    fl_color(FL_WHITE);
    for (int i = goalX + 2; i < (goalX + 1) + 11 * 12; i += 11*2)
    {
        fl_rectf(i, 2, 11, 11);
    }
    for (int i = (goalX + 2) + 11; i < (goalX + 1) + 11 * 12; i += 11*2)
    {
        fl_rectf(i, 2 + 11, 11, 11);
    }
}


void Track::finishLine(int& x, int& y, int& w, int& h)
{
    x = goalX;
    y = goalY;
    w = goalW;
    h = goalH;
}


void Track::loadTrack(std::string filename)
{
    // Delete all existing tiles
    for (int i = 0; i < leftWalls.size(); i++)
    {
        delete leftWalls[i];
    }
    for (int i = 0; i < rightWalls.size(); i++)
    {
        delete rightWalls[i];
    }
    leftWalls.clear();
    rightWalls.clear();

    // Create two arrays with all the points for left and right walls
    std::vector<std::pair<int, int>> leftPoints;
    std::vector<std::pair<int, int>> rightPoints;

    // Open file
    std::ifstream ifs(filename);
    if (!ifs)
    {
        std::cerr << "***Error***\n\tCould not open \"" + filename << "\"" << std::endl;
        return;
    }

    // Read the left points (every line until blank line)
    for(std::string line; getline( ifs, line );)
    {
        // There is an empty line after all the left points
        if (line == "")
        {
            break;
        }
        std::stringstream ss(line);
        int x, y;
        ss >> x >> y;
        leftPoints.push_back({x, y});
    }

    // Read the right points (every line from blank line to end)
    for(std::string line; getline( ifs, line );)
    {
        std::stringstream ss(line);
        int x, y;
        ss >> x >> y;
        rightPoints.push_back({x, y});
    }
    
    
    // Create all the tiles from the left and right points
    for (int i = 1; i < leftPoints.size(); i++)
    {
        int x0, y0, x1, y1;
        x0 = leftPoints[i - 1].first;
        y0 = leftPoints[i - 1].second;
        x1 = leftPoints[i].first;
        y1 = leftPoints[i].second;
        leftWalls.push_back(new Tile{x0, y0, x1, y1});
        parent()->add(leftWalls[leftWalls.size() - 1]); // Add to window

        x0 = rightPoints[i - 1].first;
        y0 = rightPoints[i - 1].second;
        x1 = rightPoints[i].first;
        y1 = rightPoints[i].second;
        rightWalls.push_back(new Tile{x0, y0, x1, y1});
        parent()->add(rightWalls[rightWalls.size() - 1]); // Add to window
    }
}
