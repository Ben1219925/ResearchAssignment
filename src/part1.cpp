#include <iostream>
#include <fstream>
#include "alg_graphs.h"
#include <random>
#include <chrono>
#include <iomanip>
#include "QuickHull.h"

using namespace std;

// QuickHull Class Implementation
QuickHull::QuickHull() : max_depth(0) {}

// Returns the side of line p1->p2 that point p is on
int QuickHull::getSide(Point p1, Point p2, Point p) {
  // Cross product to determine the side
  double val = (p2.x - p1.x) * (p.y - p1.y) - (p2.y - p1.y) * (p.x - p1.x);
  // If val > 0, point is on left side
  if (val > 0)
  {
    return 1;
  }
  // If val < 0, point is on right side
  if (val < 0){
    return -1;
  }
  // Collinear
  return 0;
}

// Returns distance from p to line p1->p2
double QuickHull::getDist(Point p1, Point p2, Point p) {
  // Area of triangle * 2 (the absolute value of the cross product)
  return std::abs((p.y - p1.y) * (p2.x - p1.x) - (p2.y - p1.y) * (p.x - p1.x));
}

// Recursive QuickHull function
void QuickHull::quickHullRecursive(Point p1, Point p2, const std::vector<Point>& points, int side, int current_depth) {
  // Update max depth
  if (current_depth > max_depth) {
    max_depth = current_depth;
  }
  int ind = -1;
  double max_dist = 0;

  // Find the point with maximum distance on the specific side
  for (size_t i = 0; i < points.size(); i++) {
    double temp_dist = getDist(p1, p2, points[i]);
    // Check if point is on the correct side and has the maximum distance
    if (getSide(p1, p2, points[i]) == side && temp_dist > max_dist) {
      ind = i;
      max_dist = temp_dist;
    }
  }

  // Base Case: If no point found, add endpoints to hull
  if (ind == -1) {
    hull.insert(p1);
    hull.insert(p2);
    return;
  }

  // Recurse for the two parts divided by points[ind]
  quickHullRecursive(points[ind], p1, points, -getSide(points[ind], p1, p2), current_depth + 1);
  quickHullRecursive(points[ind], p2, points, -getSide(points[ind], p2, p1), current_depth + 1);
}

// Main execution function
std::vector<Point> QuickHull::run(std::vector<Point>& points) {
  // Reset hull and max depth
  hull.clear();
  max_depth = 0;
  
  // If less than 3 points, return all points as hull
  if (points.size() < 3) {
    return points;
  }

  // Find min_x and max_x
  int min_x = 0;
  int max_x = 0;
  for (size_t i = 1; i < points.size(); i++) {
    if (points[i].x < points[min_x].x){
      min_x = i;
    }
    if (points[i].x > points[max_x].x){
      max_x = i;
    }
  }

  // Recursively solve for both sides of the line min_x -> max_x
  quickHullRecursive(points[min_x], points[max_x], points, 1, 1);
  quickHullRecursive(points[min_x], points[max_x], points, -1, 1);

  // Convert set to vector for return
  return std::vector<Point>(hull.begin(), hull.end());
}

// Function to return the max traversal depth
int QuickHull::getMaxDepth() const {
  return max_depth;
}

// Helper to generate Uniform Random Points
std::vector<Point> generateUniformPoints(int n, double min_val, double max_val) {
  std::vector<Point> points;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(min_val, max_val);

  // Generate n random points
  for (int i = 0; i < n; i++) {
    points.push_back({dis(gen), dis(gen)});
  }
  return points;
}

// Helper to generate Circular Points (Worst Case)
std::vector<Point> generateCircularPoints(int n, double radius, Point center) {
  std::vector<Point> points;
  double pi = 3.14159265358979323846;
  
  for (int i = 0; i < n; i++) {
    double angle = (2 * pi * i) / n;
    double x = center.x + radius * std::cos(angle);
    double y = center.y + radius * std::sin(angle);
    points.push_back({x, y});
  }
  return points;
}

#ifndef IS_TESTING
int main() {
  // Set input size
  int input_size = 5000;
  // Create QuickHull instance
  QuickHull qh;

  // Uniform Distribution
  std::cout << "Dataset 1: Uniform Distribution (n=" << input_size << ")" << std::endl;
  auto uniform_points = generateUniformPoints(input_size, 0, 1000);

  auto start = std::chrono::high_resolution_clock::now();
  auto hull_uniform = qh.run(uniform_points);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff_uniform = end - start;

  std::cout << "  > Time Taken: " << diff_uniform.count() << " seconds" << std::endl;
  std::cout << "  > Max Recursion Depth: " << qh.getMaxDepth() << std::endl;
  std::cout << "  > Hull Points Found: " << hull_uniform.size() << std::endl;

  std::cout << "----------------------------------------" << std::endl;

  // Circular Distribution
  std::cout << "Dataset 2: Circular Distribution (n=" << input_size << ")" << std::endl;
  auto circular_points = generateCircularPoints(input_size, 500, {500, 500});

  start = std::chrono::high_resolution_clock::now();
  auto hull_circle = qh.run(circular_points);
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff_circle = end - start;

  std::cout << "  > Time Taken: " << diff_circle.count() << " seconds" << std::endl;
  std::cout << "  > Max Recursion Depth: " << qh.getMaxDepth() << std::endl;
  std::cout << "  > Hull Points Found: " << hull_circle.size() << std::endl;

  return 0;
}
#endif