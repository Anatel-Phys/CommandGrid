#pragma once
#include <SFML/Graphics.hpp>
#include "CommandReader.h"
#include "GridReader.h"
#include "DynamicTimeWarping.h"
#include "DataStruct.h"

enum class Mode
{
	Listening,
	RecordingCommand1,
	RecordingCommand2,
	ErrorMode
};

std::string build_cmd_str(std::string cmd, std::vector<std::string> args);

//class GridReaderent
//{
//private:
//public:
//	HANDLE io_handler_;
//	COMSTAT status_;
//	DWORD errors_;
//
//	char front_delimiter_;
//	char end_delimiter_;
//
//	std::vector<int> sensors_val;
//	size_t grid_width;
//	size_t grid_height;
//	std::vector<std::vector<char>> inc_bytes;
//	std::vector<int> mean_sensor_vals;
//	float tolerance = 0.3;
//	DWORD time_between_record = 10;
//
//public:
//	GridReaderent(size_t width, size_t height, char* com_port, DWORD COM_BAUD_RATE);
//
//	bool ReadSensorData(int reply_wait_time);
//	void fill_sensor_vals();
//	bool CloseSerialPort();
//	~GridReaderent();
//	bool connected_;
//	bool obstructed(size_t idx);
//	void calibrate(size_t n_iter);
//};

//class CommandReaderent
//{
//private:
//public:
//	HANDLE io_handler_;
//	COMSTAT status_;
//	DWORD errors_;
//
//	char front_delimiter_;
//	char end_delimiter_;
//
//	std::vector<std::vector<char>> inc_bytes;
//
//public:
//	CommandReaderent(char* com_port, DWORD COM_BAUD_RATE);
//
//	Command read_command(int reply_wait_time);
//	bool send_cmd_str(std::string cmd);
//	void light_red();
//	void light_green();
//	void buzz();
//	Command query_command();
//	bool CloseSerialPort();
//	~CommandReaderent();
//	bool connected_;
//};

//CONVENTION : THE X SENSOR DATA COMES FIRST
class Controller
{
private:
public:
	GridReader* p_sensorsController;
	CommandReader* p_commandReader;
	size_t number_of_neighbours_computed;

	std::vector<Pattern*> dataset;
	std::vector<void(Controller::*)()> commands;
	sf::Clock patternTimer;
	sf::Clock patternResetTimer;
	float timeBeforeReset;

	//helpers
	void fill_sensor_vals();
	bool gen_point_from_sensors();
	bool currentlyReadingPattern;
	void clear_current_data();
	std::vector<Point> elagate_points(std::vector<Point>& points);
	float difference_sensibility; //if difference between to consecutive points is too small, elagate the second one
	void update_listen();
	void update_record_command(size_t idx);
	Command get_command();

	//interpolation
	Pattern* stupid_interpolation(std::vector<TimePoint>& x, std::vector<TimePoint>& y);
	Pattern* complicated_interpolation(std::vector<TimePoint>& x, std::vector<TimePoint>& y);
	Pattern* interpolation(std::vector<TimePoint>& x, std::vector<TimePoint>& y);

	std::vector<TimePoint> xData;
	std::vector<TimePoint> yData;
	Pattern* process_new_pattern();
	Mode curMode;

public:
	//commands
	void clear_patterns_from_class(size_t classIdx);
	void calibrate(size_t n_iter);
	void switch_mode(Mode mode);

	Controller(size_t width, size_t height, char* com_port_grid, char* com_port_command, DWORD COM_BAUD_RATE);
	void run(); //not to be included in my typical "while (engine.is_running()) run()"
	bool board_connected();


};