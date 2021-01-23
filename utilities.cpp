#include "utilities.h"
#include <cmath>

bool constrain(double &val, double min, double max)
{
    if (val > max)
    {
        val = max;
        return true;
    }
    else if (val < min)
    {
        val = min;
        return true;
    }
    return false;
}

// LINE/CIRCLE
bool lineCircle(double x1, double y1, double x2, double y2, double cx, double cy, double r)
{

    // is either end INSIDE the circle?
    // if so, return true immediately
    bool inside1 = pointCircle(x1, y1, cx, cy, r);
    bool inside2 = pointCircle(x2, y2, cx, cy, r);
    if (inside1 || inside2)
        return true;

    // get length of the line
    double distX = x1 - x2;
    double distY = y1 - y2;
    double len = sqrt((distX * distX) + (distY * distY));

    // get dot product of the line and circle
    double dot = (((cx - x1) * (x2 - x1)) + ((cy - y1) * (y2 - y1))) / pow(len, 2);

    // find the closest point on the line
    double closestX = x1 + (dot * (x2 - x1));
    double closestY = y1 + (dot * (y2 - y1));

    // is this point actually on the line segment?
    // if so keep going, but if not, return false
    bool onSegment = linePoint(x1, y1, x2, y2, closestX, closestY);
    if (!onSegment)
        return false;

    // optionally, draw a circle at the closest
    // point on the line
    //fill(255,0,0);
    //noStroke();
    //ellipse(closestX, closestY, 20, 20);

    // get distance to closest point
    distX = closestX - cx;
    distY = closestY - cy;
    double distance = sqrt((distX * distX) + (distY * distY));

    if (distance <= r)
    {
        return true;
    }
    return false;
}

// POINT/CIRCLE
bool pointCircle(double px, double py, double cx, double cy, double r)
{

    // get distance between the point and circle's center
    // using the Pythagorean Theorem
    double distance = dist(px, py, cx, cy);

    // if the distance is less than the circle's
    // radius the point is inside!
    if (distance <= r)
    {
        return true;
    }
    return false;
}

// Collision: LINE/POINT
bool linePoint(double x0, double y0, double x1, double y1, double px, double py)
{

    // get distance from the point to the two ends of the line
    double d1 = dist(px, py, x0, y0);
    double d2 = dist(px, py, x1, y1);

    // get the length of the line
    double lineLen = dist(x0, y0, x1, y1);

    // since floats are so minutely accurate, add
    // a little buffer zone that will give collision
    double buffer = 0.1; // higher # = less accurate

    // if the two distances are equal to the line's
    // length, the point is on the line!
    // note we use the buffer here to give a range,
    // rather than one #
    if (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer)
    {
        return true;
    }
    return false;
}

double dist(int x0, int y0, int x1, int y1)
{
    return sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
}

double map(double value, double istart, double istop, double ostart, double ostop)
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

// b2Vec2 coordPixelsToWorld(double pixelX, double pixelY)
// {
//     double transX = winWPix / 2.0;
//     double transY = winHPix / 2.0;
//     double worldX = map(pixelX, 0, winWPix, 0.0, winW);
//     double worldY = pixelY;
//     if (yFlip == -1.0f)
//         worldY = PApplet.map(pixelY, winHPix, 0f, 0f, winHPix);
//     worldY = PApplet.map(worldY, transY, transY + scaleFactor, 0f, 1f);
//     return new Vec2(worldX, worldY);
// }


double min(double val1, double val2)
{
    if (val1 < val2)
        return val1;
    return val2;
}


double max(double val1, double val2)
{
    if (val1 > val2)
        return val1;
    return val2;
}


void findMax(std::vector<double> vec, double& max, int& maxIndex)
{
    max = vec[0];   // Initialize maximum as vec[0]
    maxIndex = 0;   // Initialize index for maximum

    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i] > max)
        {
            max = vec[i];
            maxIndex = i;
        }
    }
}


double sigmoid(double x)
{
    return 1.0 / (1 + exp(-x));
}


// Given three colinear points p, q, r, the function checks if 
// point q lies on line segment 'pr' 
bool onSegment(Point p, Point q, Point r)
{ 
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && 
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) 
       return true; 
  
    return false; 
}


// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(Point p, Point q, Point r)
{ 
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
    // for details of below formula. 
    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // colinear 
  
    return (val > 0)? 1: 2; // clock or counterclock wise 
} 


// The main function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
bool doIntersect(std::pair<int, int> P1, std::pair<int, int> Q1, std::pair<int, int> P2, std::pair<int, int> Q2)
{
    // Converting to point type used by this and the other functions
    Point p1 = { P1.first, P1.second };
    Point q1 = { Q1.first, Q1.second };
    Point p2 = { P2.first, P2.second };
    Point q2 = { Q2.first, Q2.second };
    // Find the four orientations needed for general and 
    // special cases 
    int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
  
    // General case 
    if (o1 != o2 && o3 != o4) 
        return true; 
  
    // Special Cases 
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
  
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
  
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
  
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
  
    return false; // Doesn't fall in any of the above cases 
}


std::pair<int, int> lineLineIntersection(std::pair<int, int> A, std::pair<int, int> B, std::pair<int, int> C, std::pair<int, int> D)
{ 
    // Line AB represented as a1x + b1y = c1 
    double a1 = B.second - A.second; 
    double b1 = A.first - B.first; 
    double c1 = a1*(A.first) + b1*(A.second); 
  
    // Line CD represented as a2x + b2y = c2 
    double a2 = D.second - C.second; 
    double b2 = C.first - D.first; 
    double c2 = a2*(C.first)+ b2*(C.second); 
  
    double determinant = a1*b2 - a2*b1; 
  
    if (determinant == 0) 
    { 
        // The lines are parallel. This is simplified 
        // by returning a pair of large values 
        return {INT_MAX, INT_MAX}; 
    } 
    else
    { 
        int x = round((b2*c1 - b1*c2)/determinant); 
        int y = round((a1*c2 - a2*c1)/determinant); 
        return {x, y}; 
    } 
} 