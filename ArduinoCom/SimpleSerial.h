#pragma once

//Credits : dmicha https://github.com/dmicha16/simple_serial_port
// the overloaded readSerialPort (with 1 arg) is handmade

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

using namespace std;

class SimpleSerial
{

private:
public:
	HANDLE io_handler_;
	COMSTAT status_;
	DWORD errors_;

	string syntax_name_;
	char front_delimiter_;
	char end_delimiter_;

	void CustomSyntax(string syntax_type);	

public:
	SimpleSerial(char* com_port, DWORD COM_BAUD_RATE);

	string ReadSerialPort(int reply_wait_time, string syntax_type);	
	string ReadSerialPort(int reply_wait_time);
	bool WriteSerialPort(char *data_sent);
	bool CloseSerialPort();
	int empty_serial_port();
	~SimpleSerial();
	bool connected_;
};

