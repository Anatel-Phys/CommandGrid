#pragma once
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <chrono>
#include <thread>
#include <time.h>
#include <fstream>
#include <vector>
#include "DynamicTimeWarping.h"
#include "Interpolation.h"
#include <SFML/Graphics.hpp>

class GridReader
{
private:
public:
	HANDLE io_handler_;
	COMSTAT status_;
	DWORD errors_;

	char front_delimiter_;
	char end_delimiter_;

	std::vector<int> sensors_val;
	size_t grid_width;
	size_t grid_height;
	std::vector<std::vector<char>> inc_bytes;
	std::vector<int> mean_sensor_vals;
	float tolerance = 0.3;
	DWORD time_between_record = 10;

public:
	GridReader(size_t width, size_t height, char* com_port, DWORD COM_BAUD_RATE);

	bool ReadSensorData(int reply_wait_time);
	void fill_sensor_vals();
	bool CloseSerialPort();
	~GridReader();
	bool connected_;
	bool obstructed(size_t idx);
	void calibrate(size_t n_iter);
};

//CONVENTION : THE X SENSOR DATA COMES FIRST
class Controller
{
private:
	GridReader* p_sensorsController;

	std::vector<Sketch> dataset;
	std::vector<void(*)()> commands;
	sf::Clock patternTimer;
	sf::Clock patternResetTimer;
	float timeBeforeReset;

	//helpers
	void fill_sensor_vals();
	bool gen_point_from_sensors();
	bool currentlyReadingPattern;

	std::vector<TimePoint> xData;
	std::vector<TimePoint> yData;
	void save_new_pattern(size_t class_idx);

public:

	Controller();
	void run(); //not to be included in my typical "while (engine.is_running()) run()"
};