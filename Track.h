#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <vector>
#include <string>
#include "Tile.h"


class Track : public Fl_Widget
{
    std::vector<Tile*> leftWalls;
    std::vector<Tile*> rightWalls;
    
    // Dimensions of finish line
    int goalX = 1072;
    int goalY = 0;
    int goalW = 136;
    int goalH = 26;


    friend class Car;
public:
    Track();
    ~Track();

    void draw() override;
    
    void finishLine(int& x, int& y, int& w, int& h);
    void loadTrack(std::string filename);
};