#ifndef QUICKHULL_H
#define QUICKHULL_H

#include <vector>
#include <set>
#include <cmath>
#include <algorithm>
#include <iostream>

// Simple Point structure
struct Point {
    double x, y;

    // Overload < operator for use in std::set
    bool operator<(const Point& other) const {
        if (x != other.x){
            return x < other.x;
        }
        return y < other.y;
    }

    // Overload == operator for equality checks
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class QuickHull {
private:
    std::set<Point> hull;
    int max_depth;

    // Returns the side of line p1->p2 that point p is on
    // >0: Left, <0: Right, 0: Collinear
    int getSide(Point p1, Point p2, Point p);

    // Returns distance from p to line p1->p2
    double getDist(Point p1, Point p2, Point p);

    // Recursive helper function
    void quickHullRecursive(Point p1, Point p2, const std::vector<Point>& points, int side, int current_depth);

public:
    QuickHull();
    
    // Main execution function
    std::vector<Point> run(std::vector<Point>& points);
    
    // Function to return the max traversal depth
    int getMaxDepth() const;
};

#endif