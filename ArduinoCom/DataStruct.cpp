#include "DataStruct.h"

TimePoint::TimePoint(int i_val, float i_t)
{
	val = i_val;
	t = i_t;
}

Point::Point()
{
	x = 0.f;
	y = 0.f;
}

Point::Point(float _x, float _y)
{
	x = _x;
	y = _y;
}

float abs_mean_dist(Point a, Point b)
{
	return 1. / 2. * ((fabs(a.x - b.x)) + fabs(a.y - b.y));
}

float lerp(float x1, float x2, float y1, float y2, float x)
{
	float val = y1 + (x - x1) * (y2 - y1) / (x2 - x1);

	if (x > x2)
	{
		if (y2 > y1)
		{
			if (val > y2 + 0.5f)
			{
				return y2 + 0.5f;
			}
		}
		else
		{
			if (val < y2 - 0.5f)
				return y2 - 0.5f;
		}
	}
	else if (x < x1)
	{
		if (y1 > y2)
		{
			if (val > y1 + 0.5f)
			{
				return y1 + 0.5f;
			}
		}
		else
		{
			if (val < y1 - 0.5f)
			{
				return y1 - 0.5f;
			}
		}
	}
	return val;
}

float interpolate(std::vector<TimePoint>& v, float t)
{
	int i;
	if (t < v.at(0).t)
		i = 0;
	else if (t > v.at(v.size() - 1).t)
		i = v.size() - 2; //bcs i and i + 1 will be used anyway
	else
	{
		int a = 0;
		int b = v.size() - 1;
		int mid;

		while (b - a > 1)
		{
			mid = (a + b) / 2;

			if (v.at(mid).t > t)
				b = mid;
			else
				a = mid;
		}
		i = a;
	}

	return lerp(v.at(i).t, v.at(i + 1).t, v.at(i).val, v.at(i + 1).val, t);
}

TimeSerie::TimeSerie(std::vector<TimePoint> i_x, std::vector<TimePoint> i_y)
{
	xSize = i_x.size();
	ySize = i_y.size();

	x = i_x;
	y = i_y;
}

Pattern::Pattern(std::vector<Point> i_points, int i_classIdx)
{
	points = i_points;
	size = i_points.size();
	classIdx = i_classIdx;
}