#pragma once
//C++ port ibrary built to be used with SimpleSerial https://github.com/dmicha16/simple_serial_port
//Port of the library arduino-python3 https://github.com/thearn/Python-Arduino-Command-API

#include <string>
#include <cstring>
#include <vector>
#include "SimpleSerial.h"
#include <cstdlib>

#define HIGH true
#define LOW false
#define INPUT true
#define OUTPUT false
static float o = 0.f;
static float e0 = 164.81f;
static float f0 = 174.61f;
static float f0d = 185.f;
static float g0 = 196.f;
static float g0d = 207.65f;
static float a0 = 220.f;
static float a0d = 233.08f;
static float b0 = 246.94f;
static float c0 = 261.63f;
static float c0d = 277.18f;
static float d0 = 293.66f;
static float d0d = 311.13f;
static float e = 329.63f;
static float f = 349.23f;
static float fd = 369.99;
static float g = 392.f;
static float gd = 415.30f;
static float a = 440.f;
static float ad = 466.16f;
static float b = 493.88f;
static float c = 523.25;
static float cd = 554.37;
static float d = 587.33f;
static float dd = 622.25;
static float e2 = 2 * e;
static float f2 = 2 * f;
static float f2d = 2 * fd;
static float g2 = 2 * g;
static float g2d = 2 * gd;
static float a2 = 2 * a;
static float a2d = 2 * ad;
static float b2 = 2 * b;
static float c2 = 2 * c;
static float c2d = 2 * cd;
static float d2 = 2 * d;
static float d2d = 2 * dd;
static float e3 = 2 * e2;
/*
Build a command string that can be sent to the arduino.

Input:
cmd(str) : the command to send to the arduino, must not
contain a% character
args(iterable) : the arguments to send to the command

@TODO : a strategy is needed to escape% characters in the args
*/
std::string build_cmd_str(std::string cmd, std::vector<std::string> args);

class Arduino
{
public:
	SimpleSerial* simpleSerial;

	void send_cmd_str(std::string cmd);
	Arduino(char com_port[], DWORD COM_BAUD_RATE = CBR_115200);
	void digitalWrite(int pin, bool val);
	void pinMode(int pin, bool mode);
	int analogRead(int pin);
	void tone(int pin, float freq);
	void noTone(int pin);
};

