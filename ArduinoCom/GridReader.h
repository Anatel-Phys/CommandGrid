#pragma once
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <chrono>
#include <thread>
#include <time.h>
#include <fstream>
#include <iostream>

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

	bool readSensorData(int reply_wait_time);
	void fill_sensor_vals();
	bool closeSerialPort();
	~GridReader();
	bool connected_;
	bool obstructed(size_t idx);
	void calibrate(size_t n_iter);

};

