#include "GridReader.h"

GridReader::GridReader(size_t width, size_t height, char* com_port, DWORD COM_BAUD_RATE)
{
	connected_ = false;
	front_delimiter_ = '[';
	end_delimiter_ = ']';


	grid_width = width;
	grid_height = height;
	sensors_val.resize(width + height);
	mean_sensor_vals.resize(width + height);
	//inc_bytes = new char[]

	io_handler_ = CreateFileA(static_cast<LPCSTR>(com_port),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (io_handler_ == INVALID_HANDLE_VALUE) {

		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			printf("Warning: Handle was not attached. Reason: %s not available\n", com_port);
	}
	else {

		DCB dcbSerialParams = { 0 };

		if (!GetCommState(io_handler_, &dcbSerialParams)) {

			printf("Warning: Failed to get current serial params");
		}

		else {
			dcbSerialParams.BaudRate = COM_BAUD_RATE;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.StopBits = ONESTOPBIT;
			dcbSerialParams.Parity = NOPARITY;
			dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(io_handler_, &dcbSerialParams))
				printf("Warning: could not set serial port params\n");
			else {
				connected_ = true;
				PurgeComm(io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
			}
		}
	}

	Sleep(2000);
}

bool GridReader::ReadSensorData(int reply_wait_time)
{
	DWORD buf_bytes;
	inc_bytes.clear();
	inc_bytes.reserve(grid_height + grid_width);
	size_t bytes_read = 0;
	size_t n_sensors_read = 0;
	char inc_msg[1];
	bool began = false;

	unsigned long start_time = time(nullptr);

	ClearCommError(io_handler_, &errors_, &status_);

	std::vector<char> inc_buf;
	inc_buf.reserve(7); //2 for sensor idx, 1 for %, 4 for analog val

	while (n_sensors_read < grid_height + grid_width && (time(nullptr) - start_time) < reply_wait_time) {
		if (status_.cbInQue > 0) {
			if (ReadFile(io_handler_, inc_msg, 1, &buf_bytes, NULL)) {

				if (inc_msg[0] == front_delimiter_ || began) {
					began = true;


					if (inc_msg[0] == end_delimiter_)
					{
						inc_bytes.push_back(inc_buf);
						inc_buf.clear();
						n_sensors_read++;
					}

					else if (inc_msg[0] != front_delimiter_)
					{
						inc_buf.push_back(inc_msg[0]);
						bytes_read++;
					}
				}
			}
			else
				return false;
		}
	}
	return true;
}

void GridReader::fill_sensor_vals()
{
	int idx;

	for (std::vector<char> sens : inc_bytes)
	{
		idx = 0;

		//std::cout << sens.size() << std::endl;
		//for (char c : sens)
		//	std::cout << c;
		//std::cout << std::endl;

		while (sens.at(idx) != '%')
			idx++;

		char* sens_idx = new char[idx + 1];

		for (size_t i = 0; i < idx; i++)
			sens_idx[i] = sens.at(i);
		sens_idx[idx] = '\0';

		char* sens_data = new char[sens.size() - idx];

		for (size_t i = 0; i < sens.size() - idx - 1; i++)
			sens_data[i] = sens.at(i + idx + 1);
		sens_data[sens.size() - idx - 1] = '\0';

		//for (size_t i = 0; i < idx; i++)
		//	std::cout << sens_idx[i];
		//std::cout << " ";
		//for (size_t i = 0; i < sens.size() - idx - 1; i++)
		//	std::cout << sens_data[i];
		//std::cout << std::endl;

		int sens_idx_num = std::stoi(sens_idx);
		int sens_data_num = std::stoi(sens_data);

		sensors_val.at(sens_idx_num) = sens_data_num;

		delete[] sens_idx;
		delete[] sens_data;

	}
}

bool GridReader::CloseSerialPort()
{
	if (connected_) {
		connected_ = false;
		CloseHandle(io_handler_);
		return true;
	}
	else
		return false;
}

GridReader::~GridReader()
{
	if (connected_) {
		connected_ = false;
		CloseHandle(io_handler_);
	}
}

bool GridReader::obstructed(size_t idx)
{
	return sensors_val.at(idx) > (2 + tolerance) * mean_sensor_vals.at(idx);
}

void GridReader::calibrate(size_t n_iter)
{
	for (size_t i = 0; i < mean_sensor_vals.size(); i++)
	{
		mean_sensor_vals.at(i) = 0;
	}

	for (size_t n = 0; n < n_iter; n++)
	{
		PurgeComm(io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
		Sleep(20);

		if (!ReadSensorData(1))
			std::cout << "Couldn't read sensor data\n";

		fill_sensor_vals();

		for (size_t i = 0; i < sensors_val.size(); i++)
		{
			mean_sensor_vals.at(i) += sensors_val.at(i);
		}
	}

	for (size_t i = 0; i < mean_sensor_vals.size(); i++)
	{
		mean_sensor_vals.at(i) = static_cast<int>(static_cast<float>(mean_sensor_vals.at(i)) / n_iter);
	}
}
