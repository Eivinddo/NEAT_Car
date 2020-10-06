#include "Tile.h"
#include <iostream>
#include <cmath>


Tile::Tile(int x0, int y0, int x1, int y1) : Fl_Widget{0, 0, winW, winH},
    x0{x0}, y0{y0}, x1{x1}, y1{y1}
{
    if (x0 == x1)
    {
        angle = -90 * DEGTORAD * (abs(y1-y0)/(y1-y0));
    }
    else
    {
        angle = -atan2(static_cast<double>(y1 - y0), static_cast<double>(x1 - x0));
    }
    std::cout << "Ang: " << angle << std::endl;

    corners.push_back( { x0 - thickness * cos(PI / 2 - angle), y0 - thickness * sin(PI / 2 - angle) } );
    corners.push_back( { x0 + thickness * cos(PI / 2 - angle), y0 + thickness * sin(PI / 2 - angle) } );
    corners.push_back( { x1 + thickness * cos(PI / 2 - angle), y1 + thickness * sin(PI / 2 - angle) } );
    corners.push_back( { x1 - thickness * cos(PI / 2 - angle), y1 - thickness * sin(PI / 2 - angle) } );
}


Tile::~Tile()
{
}


void Tile::draw() {
    
    fl_color(col);
    fl_begin_polygon();
    for (int i = 0; i < 4; i++)
    {
        fl_vertex(corners[i].first, corners[i].second);
    }
    fl_end_polygon();
}