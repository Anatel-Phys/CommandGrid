#pragma once
#include <vector>
#include <iostream>

struct TimePoint
{
	float val;
	float t;

	TimePoint(int i_val, float i_t);
};

struct Point
{
	float x, y;

	Point();
	Point(float _x, float _y);
};

float abs_mean_dist(Point a, Point b);
float lerp(float x1, float x2, float y1, float y2, float x); //should return middle of interval if x is too far from x1 of x2 bcs else it'll assume an extrapolation that would imply that a sensor has been crossed ->"rectified lerp"
float interpolate(std::vector<TimePoint>& v, float t);

struct TimeSerie
{
	std::vector<TimePoint> x, y;
	size_t xSize, ySize;

	TimeSerie(std::vector<TimePoint> i_x, std::vector<TimePoint> i_y);
};

struct Pattern
{
	std::vector<Point> points;
	size_t size;
	int classIdx;

	Pattern(std::vector<Point> i_points, int i_classIdx = 0);
};