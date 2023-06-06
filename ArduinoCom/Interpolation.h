#pragma once
#include <vector>
#include "DynamicTimeWarping.h"
#include "DataModule.h"

static int nSensorX = 11;
static int nSensorY = 11;

//example main
//pattern acquisition test for testing the interpolation algorithm
//int main()
//{
//	PatternSimulation sim(nSensorX, nSensorY, 2., 0.02, 1000, 1000);
//
//	while (sim.is_running())
//		sim.run();
//
//	auto sens_x = sim.get_patternX();
//	auto sens_y = sim.get_patternY();
//
//	std::vector<TimeSerie> series = split_patterns(sens_x, sens_y);
//
//	TimeSerie ts = series.at(0);
//
//	Pattern pat = complicated_algorithm(ts);
//
//	std::ofstream f("data.txt", std::ios::trunc);
//	for (Point p : pat.points)
//	{
//		f << p.x << "\t" << p.y << std::endl;
//	}
//	f.close();
//
//	f = std::ofstream("x.txt", std::ios::trunc);
//	for (TimePoint tp : ts.x)
//	{
//		f << tp.t << "\t" << tp.val << std::endl;
//	}
//	f.close();
//
//	f = std::ofstream("y.txt", std::ios::trunc);
//	for (TimePoint tp : ts.y)
//	{
//		f << tp.t << "\t" << tp.val << std::endl;
//	}
//
//}

float lerp(float x1, float x2, float y1, float y2, float x); //should return middle of interval if x is too far from x1 of x2 bcs else it'll assume an extrapolation that would imply that a sensor has been crossed ->"rectified lerp"

struct TimePoint
{
	float val;
	float t;

	TimePoint(int i_val, float i_t)
	{
		val = i_val;
		t = i_t;
	}
};


struct TimeSerie
{
	std::vector<TimePoint> x, y;
	size_t xSize, ySize;

	TimeSerie(std::vector<TimePoint> i_x, std::vector<TimePoint> i_y)
	{
		xSize = i_x.size();
		ySize = i_y.size();

		x = i_x;
		y = i_y;
	}
};

struct Pattern
{
	std::vector<Point> points;
	size_t size;
	int classIdx;

	Pattern(std::vector<Point> i_points, int i_classIdx = 0)
	{
		points = i_points;
		size = i_points.size();
		classIdx = i_classIdx;
	}
};

float interpolate(std::vector<TimePoint>& v, float t);

Pattern stupid_algorithm(TimeSerie ts);

Pattern complicated_algorithm(TimeSerie ts);

std::vector<TimeSerie> split_patterns(std::vector<TimePoint> x, std::vector<TimePoint> y);