#pragma once
#include <SFML/Graphics.hpp>
#include "CommandReader.h"
#include "GridReader.h"
#include "DynamicTimeWarping.h"
#include "DataStruct.h"

enum class Mode
{
	Listening,
	RecordingCommand0,
	RecordingCommand1,
	EmptyCommands,
	Idle,
	ErrorMode
};

std::string build_cmd_str(std::string cmd, std::vector<std::string> args);

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
	void led_green();
	void led_red();

	Controller(size_t width, size_t height, char* com_port_grid, char* com_port_command, DWORD COM_BAUD_RATE);
	void run(); //not to be included in my typical "while (engine.is_running()) run()"
	bool board_connected();


};