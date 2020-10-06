#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <vector>
#include "Tile.h"


class Track : public Fl_Widget
{
    //std::vector<Car*> cars;
    std::vector<Tile*> leftWalls;
    std::vector<Tile*> rightWalls;

    friend class Car;
public:
    Track();
    ~Track();

    void draw() override;
};