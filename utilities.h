#pragma once

#include <utility>

constexpr int winW = 1280;			// Window Width
constexpr int winH = 720;			// Window Height
constexpr double PI = 3.14159265359;
constexpr double DEGTORAD = PI / 180.0;
constexpr double RADTODEG = 180.0 / PI;

struct Point 
{ 
    int x; 
    int y; 
};


bool constrain(double& val, double min, double max);

double dist(int x0, int y0, int x1, int y1);

bool lineCircle(double x1, double y1, double x2, double y2, double cx, double cy, double r);

bool pointCircle(double px, double py, double cx, double cy, double r);

bool linePoint(double x0, double y0, double x1, double y1, double px, double py);

double map(double value, double istart, double istop, double ostart, double ostop);

double min(double val1, double val2);

double max(double val1, double val2);

double sigmoid(double x);

bool onSegment(Point p, Point q, Point r);

int orientation(Point p, Point q, Point r);

bool doIntersect(std::pair<int, int> P1, std::pair<int, int> Q1, std::pair<int, int> P2, std::pair<int, int> Q2);

std::pair<int, int> lineLineIntersection(std::pair<int, int> A, std::pair<int, int> B, std::pair<int, int> C, std::pair<int, int> D);