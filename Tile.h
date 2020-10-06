#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <vector>
#include "utilities.h"


class Tile : public Fl_Widget
{
    Fl_Color col = FL_DARK_BLUE;

public:
    int x0;
    int y0;
    int x1;
    int y1;
    int thickness = 3;
    double angle;
    std::vector<std::pair<int, int>> corners;   // The four corners of the Tile, each point defined by a pair
    Tile(int x0, int y0, int x1, int y1);
    ~Tile();

    void draw() override;
    void setColor(Fl_Color c) { col = c; }
};
