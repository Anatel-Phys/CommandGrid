#include "Controller.h"

void Controller::fill_sensor_vals()
{
	p_sensorsController->fill_sensor_vals();
}

bool Controller::gen_point_from_sensors()
{
	//for (float val : p_sensorsController->)

	bool some_sensor_is_obstructed = false;

	int n_obstr = 0;
	float mean_pos = 0.f;

	for (int i = 0; i < p_sensorsController->grid_width; i++)
	{
		if (p_sensorsController->obstructed(i))
		{
			std::cout << i << " obstructed\n";
			mean_pos += i;
			n_obstr++;
		}
	}

	if (n_obstr > 0)
	{
		mean_pos /= n_obstr;
		xData.push_back(TimePoint(mean_pos, patternTimer.getElapsedTime().asSeconds()));
		some_sensor_is_obstructed = true;
	}
	
	n_obstr = 0;
	mean_pos = 0.f;
	for (int j = 0; j < p_sensorsController->grid_height; j++)
	{
		if (p_sensorsController->obstructed(p_sensorsController->grid_width + j))
		{
			std::cout << j << " obstructed j\n";
			mean_pos += j;
			n_obstr++;
		}
	}

	if (n_obstr > 0)
	{
		mean_pos /= n_obstr;
		yData.push_back(TimePoint(mean_pos, patternTimer.getElapsedTime().asSeconds()));
		some_sensor_is_obstructed = true;
	}

	return some_sensor_is_obstructed;
}

void Controller::clear_current_data()
{
	std::cout << "Switching mode, clearing data\n";
	xData.clear();
	yData.clear();
}

std::vector<Point> Controller::elagate_points(std::vector<Point>& points)
{
	if (points.size() == 0)
		return std::vector<Point>();

	std::vector<Point> elagatedTimePoints;
	elagatedTimePoints.reserve(points.size());
	elagatedTimePoints.push_back(points.at(0));
	for (size_t i = 1; i < points.size(); i++)
	{
		if (std::abs(points.at(i).x - elagatedTimePoints.at(elagatedTimePoints.size() - 1).x) > difference_sensibility
			|| std::abs(points.at(i).y - elagatedTimePoints.at(elagatedTimePoints.size() - 1).y) > difference_sensibility)
			elagatedTimePoints.push_back(points.at(i));
	}

	return elagatedTimePoints;
}

void Controller::update_listen()
{

}

void Controller::update_record_command(size_t idx)
{
	PurgeComm(p_sensorsController->io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
	Sleep(20);
	if (!p_sensorsController->readSensorData(1))
	{
		std::cout << "Couldn't read sensor data\n";
	}
	fill_sensor_vals();

	if (gen_point_from_sensors())
	{
		if (!currentlyReadingPattern)
		{
			std::cout << "Started reading pattern\n";
			currentlyReadingPattern = true;
			patternResetTimer.restart();
			patternTimer.restart();
			if (xData.size() > 0) //caser le check du temps du 1er point > 0 ici;
				xData.at(0).t = 0.f;
			if (yData.size() > 0)
				yData.at(0).t = 0.f;
		}
		else
		{
			patternResetTimer.restart();
		}
	}
	else
	{
		if (currentlyReadingPattern)
		{
			if (patternResetTimer.getElapsedTime().asSeconds() > timeBeforeReset)
			{
				currentlyReadingPattern = false;
				process_new_pattern();
				std::cout << "Saved new pattern\n";
			}
		}
	}
}

Command Controller::get_command()
{
	return p_commandReader->query_command();
}

void Controller::clear_patterns_from_class(size_t classIdx)
{
	std::cout << "Clearing data from class " << classIdx << std::endl;
	std::vector<Pattern*> new_dataset;

	for (Pattern* p : dataset)
	{
		if (p->classIdx != classIdx)
		{
			new_dataset.push_back(p);
		}
		else
		{
			delete p;
		}
	}

	dataset = new_dataset;
}

void Controller::calibrate(size_t n_iter)
{
	PurgeComm(p_sensorsController->io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
	p_sensorsController->calibrate(n_iter);
}

void Controller::switch_mode(Mode mode)
{
	curMode = mode;
}

void Controller::led_green()
{
	std::cout << "trying to light green led\n";
	p_commandReader->send_cmd_str(CMD_GREEN_LED);
}

void Controller::led_red()
{
	std::cout << "trying to light red led\n";
	p_commandReader->send_cmd_str(CMD_RED_LED);
}

Pattern* Controller::stupid_interpolation(std::vector<TimePoint>& x, std::vector<TimePoint>& y)
{
	//if (x.size() <= 1 && y.size() <= 1)
	//{
	//	std::cout << "Pattern almost empty, returning empty pattern" << std::endl;
	//	return new Pattern(std::vector<Point>());
	//}
	if (x.size() == 0)
		x.push_back(TimePoint((float)p_sensorsController->grid_width / 2.f, 0.f));

	if (x.size() == 1)
		x.push_back(TimePoint(x.at(0).val, x.at(0).t + 1.f));

	if (y.size() == 0)
		y.push_back(TimePoint((float)p_sensorsController->grid_height / 2.f, 0.f));

	if (y.size() == 1)
		y.push_back(TimePoint(y.at(0).val, y.at(0).t + 1.f));

	std::vector<Point> points;


	float curTime;
	for (int i = 0; i < x.size(); i++)
	{
		curTime = x.at(i).t;
		points.emplace_back(x.at(i).val, interpolate(y, curTime));
	}

	for (int i = 0; i < y.size(); i++)
	{
		curTime = y.at(i).t;
		points.emplace_back(interpolate(x, curTime), y.at(i).val);
	}

	Pattern* p = new Pattern(points);
	return p;
}

Pattern* Controller::complicated_interpolation(std::vector<TimePoint>& x, std::vector<TimePoint>& y)
{
	float buf;
	std::vector<Point> points;

	if (x.size() == 0)
		x.push_back(TimePoint((float)p_sensorsController->grid_width / 2.f, 0.f));

	if (x.size() == 1)
		x.push_back(TimePoint(x.at(0).val, x.at(0).t + 1.f));

	if (y.size() == 0)
		y.push_back(TimePoint((float)p_sensorsController->grid_height / 2.f, 0.f));

	if (y.size() == 1)
		y.push_back(TimePoint(y.at(0).val, y.at(0).t + 1.f));

	int i = 1;
	int j = 1;

	if (x.at(0).t <= y.at(0).t) //val extrapolation step
	{
		while (i < x.size() && x.at(i).t < y.at(0).t)
		{
			std::cout << "extrapol left i : " << i << " " << x.size() << std::endl;
			points.emplace_back(x.at(i).val, lerp(y.at(0).t, y.at(1).t, y.at(0).val, y.at(1).val, x.at(i).t));
			i++;
		}
	}
	else
	{
		while (j < y.size() && y.at(j).t <= x.at(0).t)
		{
			std::cout << "extrapol left j : " << j << " " << y.size() << std::endl;
			points.emplace_back(lerp(x.at(0).t, x.at(1).t, x.at(0).val, x.at(1).val, y.at(j).t), y.at(j).val);
			j++;
		}
	}

	while (i < x.size() && j < y.size()) //main loop
	{
		while (j < y.size() && i < x.size() && x.at(i).t <= y.at(j).t) //if x[i].t < y[j].t AND i < xSize, after first step, it is guarenteed that y[j - 1] exists
		{
			std::cout << "interpol j : " << j << " " << y.size() << std::endl;
			points.emplace_back(x.at(i).val, lerp(y.at(j - 1).t, y.at(j).t, y.at(j - 1).val, y.at(j).val, x.at(i).t));
			i++;
		}

		while (i < x.size() && j < y.size() && y.at(j).t <= x.at(i).t)
		{
			std::cout << "interpol i : " << i << " " << x.size() << std::endl;
			points.emplace_back(lerp(x.at(i - 1).t, x.at(i).t, x.at(i - 1).val, x.at(i).val, y.at(j).t), y.at(j).val);
			j++;
		}
	}

	if (i < x.size()) //if not needed, actually. I'll let them here for clarity, for now
	{
		while (i < x.size())
		{
			points.emplace_back(x.at(i).val, lerp(y.at(y.size() - 2).t, y.at(y.size() - 1).t,
				y.at(y.size() - 2).val, y.at(y.size() - 1).val, x.at(i).t));
			i++;
		}
	}
	else if (j < y.size())
	{
		while (j < y.size())
		{
			points.emplace_back(lerp(x.at(x.size() - 2).t, x.at(x.size() - 1).t,
				x.at(x.size() - 2).val, x.at(x.size() - 1).val, y.at(j).t), y.at(j).val);
			j++;
		}
	}

	return new Pattern(points);
}

Pattern* Controller::interpolation(std::vector<TimePoint>& x, std::vector<TimePoint>& y)
{
	float buf;
	std::vector<Point> points;

	if (x.size() == 0 && y.size() == 0)
	{
		std::cout << "Interpolating null pattern, returning nullptr\n";
		return nullptr;
	}

	if (x.size() == 0)
		x.push_back(TimePoint((float)p_sensorsController->grid_width / 2.f, 0.f));

	if (x.size() == 1)
		x.push_back(TimePoint(x.at(0).val, x.at(0).t + 1.f));

	if (y.size() == 0)
		y.push_back(TimePoint((float)p_sensorsController->grid_height / 2.f, 0.f));

	if (y.size() == 1)
		y.push_back(TimePoint(y.at(0).val, y.at(0).t + 1.f));

	int i = 0;
	int j = 0;
	
	while (true)
	{
		if (x.at(i + 1).t < y.at(j + 1).t)
		{

		}
	}
}

Pattern* Controller::process_new_pattern()
{
	//if (xData.at(0).t > 0.f && yData.at(0).t > 0.f) //fix for when the timer isn't reset yet, implying a first val with t > 0
	//{
	//	bool xWasFirst = xData.at(0).t > yData.at(0).t;
	//	if (xWasFirst)
	//		xData.at(0).t = 0;
	//	else
	//		yData.at(0).t = 0;
	//}

	if (xData.size() > 0)
	{
		if (yData.size() > 0)
		{
			bool xWasFirst = xData.at(0).t > yData.at(0).t;
			if (xWasFirst)
				xData.at(0).t = 0.f;
			else
				yData.at(0).t = 0.f;
		}

		else
			xData.at(0).t = 0.f;
	}
	else
		yData.at(0).t = 0.f;

	//std::cout << "X DATA\n";
	//for (int i = 0; i < xData.size(); i++)
	//	std::cout << xData.at(i).t << "\t" << xData.at(i).val << std::endl;
	//std::cout << "Y DATA\n";
	//for (int i = 0; i < yData.size(); i++)
	//	std::cout << yData.at(i).t << "\t" << yData.at(i).val << std::endl;

	Pattern* pattern = complicated_interpolation(xData, yData);
	pattern->points = elagate_points(pattern->points);
	for (Point p : pattern->points)
		std::cout << p.x << "\t" << p.y << std::endl;


	pattern->size = pattern->points.size();

	xData.clear();
	yData.clear();

	return pattern;
}

Controller::Controller(size_t width, size_t height, char* com_port_grid, char* com_port_command, DWORD COM_BAUD_RATE)
{
	p_sensorsController = new GridReader(width, height, com_port_grid, COM_BAUD_RATE);
	p_commandReader = new CommandReader(com_port_command, COM_BAUD_RATE);
	currentlyReadingPattern = false;
	timeBeforeReset = 1.f;
	difference_sensibility = 0.05f;
	number_of_neighbours_computed = 5;

	commands.push_back(&Controller::led_green);
	commands.push_back(&Controller::led_red);

	//for (int i = 0; i < p_sensorsController->mean_sensor_vals.size(); i++)
	//{
	//	p_sensorsController->mean_sensor_vals.at(i) = 10;
	//}
	calibrate(30);
	patternResetTimer.restart();
	patternTimer.restart();
}

void Controller::run()
{
	while (true)
	{
		Command command = get_command();

		switch (command)
		{
		case Command::Listen:
			//std::cout << "Listening\n";
			if (curMode != Mode::Listening)
				clear_current_data();
			curMode = Mode::Listening;
			break;
		case Command::Calibrate:
			calibrate(10);
			//std::cout << "Calibrating\n";
			p_commandReader->send_cmd_str(CMD_RESET);
			break;
		case Command::EmptyCommands:
			//std::cout << "Empty\n";
			clear_patterns_from_class(1);
			clear_patterns_from_class(2);
			p_commandReader->send_cmd_str(CMD_RESET);
			break;
		case Command::Idle:
			//std::cout << "Idle\n";
			break;
		case Command::RecordCommand0:
			//std::cout << "Record1\n";
			if (curMode != Mode::RecordingCommand0)
				clear_current_data();
			curMode = Mode::RecordingCommand0;
			break;
		case Command::RecordCommand1:
			//std::cout << "Record2\n";
			if (curMode != Mode::RecordingCommand1)
				clear_current_data();
			curMode = Mode::RecordingCommand1;
			break;
		case Command::ErrorCommand:
			//std::cout << "Unable to understand command, skipping\n";
			p_commandReader->send_cmd_str(CMD_RESET);
			break;
		default:
			break;
		}
		
		PurgeComm(p_sensorsController->io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
		PurgeComm(p_commandReader->io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
		Sleep(5);
		if (!p_sensorsController->readSensorData(1))
			std::cout << "Couldn't read sensor data\n";

		fill_sensor_vals();

	/*	for (int val : p_sensorsController->sensors_val)
			std::cout << val << "\t";
		std::cout << std::endl;*/

		if (gen_point_from_sensors())
		{
			if (!currentlyReadingPattern)
			{
				std::cout << "Started reading pattern\n";
				currentlyReadingPattern = true;
				patternResetTimer.restart();
				patternTimer.restart();
			}
			else
			{
				std::cout << "reset reading pattern\n";
				patternResetTimer.restart();
			}
		}
		else
		{
			if (currentlyReadingPattern)
			{
				if (patternResetTimer.getElapsedTime().asSeconds() > timeBeforeReset)
				{
					std::cout << "Processing pattern\n";
					currentlyReadingPattern = false;
					Pattern* p = process_new_pattern();

					size_t k;
					switch (curMode)
					{
					case Mode::Listening:
						std::cout << "Trying to classify\n";
						if (dataset.size() < number_of_neighbours_computed)
						{
							std::cout << "Too few patterns in dataset, passing pattern\n";
							delete p;
						}
						else
						{
							k = knn_class(p, dataset, number_of_neighbours_computed);
							std::cout << "class " << k << " identified\n";
							(this->*commands.at(k))();
							delete p;
						}
						break;
					case Mode::RecordingCommand0:
						p->classIdx = 0;
						dataset.push_back(p);
						break;
					case Mode::RecordingCommand1:
						p->classIdx = 1;
						dataset.push_back(p);
						break;
					case Mode::ErrorMode:
						break;
					default:
						break;
					}
				}
			}
		}


		
	}
}

bool Controller::board_connected()
{
	return p_sensorsController->connected_;
}