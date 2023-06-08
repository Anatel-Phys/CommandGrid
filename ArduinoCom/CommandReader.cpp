#include "CommandReader.h"

bool CommandReader::send_cmd_str(std::string cmd)
{
	char* cmd_arr = &cmd[0];
	DWORD bytes_sent;

	unsigned int data_sent_length = strlen(cmd_arr);

	if (!WriteFile(io_handler_, (void*)cmd_arr, data_sent_length, &bytes_sent, NULL)) {
		ClearCommError(io_handler_, &errors_, &status_);
		return false;
	}
	else
		return true;
}

Command CommandReader::query_command()
{
	PurgeComm(io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
	send_cmd_str(CMD_COMMAND);
	Sleep(10);
	return read_command(1);
}

bool CommandReader::CloseSerialPort()
{
	if (connected_) {
		connected_ = false;
		CloseHandle(io_handler_);
		return true;
	}
	else
		return false;
}

CommandReader::~CommandReader()
{
	if (connected_) {
		connected_ = false;
		CloseHandle(io_handler_);
	}
}

CommandReader::CommandReader(char* com_port, DWORD COM_BAUD_RATE)
{
	connected_ = false;
	front_delimiter_ = '[';
	end_delimiter_ = ']';

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

Command CommandReader::read_command(int reply_wait_time)
{
	DWORD bytes_read;
	char inc_msg[1];
	std::string complete_inc_msg;
	bool began = false;

	unsigned long start_time = time(nullptr);

	ClearCommError(io_handler_, &errors_, &status_);

	while ((time(nullptr) - start_time) < reply_wait_time) {

		if (status_.cbInQue > 0) {

			if (ReadFile(io_handler_, inc_msg, 1, &bytes_read, NULL)) {
				if (inc_msg[0] == front_delimiter_ || began) {
					began = true;

					if (inc_msg[0] == end_delimiter_)
					{
						int command = std::stoi(complete_inc_msg);
						return Command(command);
					}

					if (inc_msg[0] != front_delimiter_)
						complete_inc_msg.append(inc_msg, 1);
				}
			}
			else
				return Command::ErrorCommand;
		}
	}
	return Command::ErrorCommand;
}