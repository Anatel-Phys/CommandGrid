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

#define CMD_RESET "@rs$!"
#define CMD_COMMAND "@cm$!"
#define CMD_GREEN_LED "@gl$!"
#define CMD_RED_LED "@rl$!"

enum class Command
{
	Listen,
	Calibrate,
	EmptyCommands,
	RecordCommand0,
	RecordCommand1,
	Idle,
	ErrorCommand
};

class CommandReader
{
private:
public:
	HANDLE io_handler_;
	COMSTAT status_;
	DWORD errors_;

	char front_delimiter_;
	char end_delimiter_;

	std::vector<std::vector<char>> inc_bytes;

public:
	CommandReader(char* com_port, DWORD COM_BAUD_RATE);

	Command read_command(int reply_wait_time);
	bool send_cmd_str(std::string cmd);;
	void buzz();
	Command query_command();
	bool CloseSerialPort();
	~CommandReader();
	bool connected_;
};

