#define CATCH_CONFIG_MAIN // Tells Catch2 to provide a main() function
#include <catch2/catch_all.hpp>
#include <vector>
#include "QuickHull.h"

using namespace std;

// Helper function to check if a point exists in the result vector
auto containsPoint = [](const vector<Point>& hull, double x, double y) -> bool {
    for (const auto& p : hull) {
        if (abs(p.x - x) < 0.0001 && abs(p.y - y) < 0.0001){
			return true; 
		}
    }
    return false;
};

TEST_CASE("Standard Shape: Triangle", "[triangle]")
{
    QuickHull qh;

    SECTION("Triangle: All points should be in hull")
    {
        vector<Point> input = {{0, 0}, {5, 5}, {10, 0}};
        vector<Point> result = qh.run(input);

        REQUIRE(result.size() == 3);
        CHECK(containsPoint(result, 0, 0));
        CHECK(containsPoint(result, 5, 5));
        CHECK(containsPoint(result, 10, 0));
    }
}

TEST_CASE("Standard Shape: Square", "[square]")
{
    QuickHull qh;

    SECTION("Square with Internal Point: Internal point should be discarded")
    {
        // (5,5) is strictly inside the square defined by the other 4 points
        vector<Point> input = {{0, 0}, {0, 10}, {10, 10}, {10, 0}, {5, 5}};
        vector<Point> result = qh.run(input);

        REQUIRE(result.size() == 4);
        CHECK(containsPoint(result, 0, 0));
        CHECK(containsPoint(result, 10, 10));
        CHECK_FALSE(containsPoint(result, 5, 5)); // Middle point should be gone
    }
}

TEST_CASE("Edge Case: Collinear Points", "[collinear]")
{
    QuickHull qh;
    
    // Points falling exactly on a line. 
    // The hull of a line segment is just the two endpoints.
    vector<Point> input = {{0, 0}, {1, 1}, {2, 2}, {3, 3}};
    vector<Point> result = qh.run(input);

    // Must contain the extremes (0,0) and (3,3).
    CHECK(containsPoint(result, 0, 0));
    CHECK(containsPoint(result, 3, 3));
    
    // Ensure size is 2 or more 
    REQUIRE(result.size() >= 2);
}

TEST_CASE("Edge Case: Duplicate Inputs", "[duplicates]")
{
    QuickHull qh;

    // Input contains duplicate coordinates for the top corner (5,5)
    vector<Point> input = {{0, 0}, {5, 5}, {5, 5}, {10, 0}};
    vector<Point> result = qh.run(input);

    // Should behave like a normal triangle (size 3)
    REQUIRE(result.size() == 3);
    CHECK(containsPoint(result, 5, 5));
}

TEST_CASE("Edge Case: Insufficient Points", "[insufficient]")
{
    QuickHull qh;

    SECTION("Two Points")
    {
        vector<Point> input = {{1, 1}, {2, 2}};
        vector<Point> result = qh.run(input);
        
        // Cannot form a 2D hull, should return input as is
        REQUIRE(result.size() == 2);
    }

    SECTION("Single Point")
    {
        vector<Point> input = {{1, 1}};
        vector<Point> result = qh.run(input);

        REQUIRE(result.size() == 1);
    }
}