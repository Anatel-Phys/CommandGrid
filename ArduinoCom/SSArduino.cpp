#include "SSArduino.h"


std::string build_cmd_str(std::string cmd, std::vector<std::string> args)
{
	std::string cmd_args = "@" + cmd;
	for (std::string s : args)
	{
		cmd_args.append("%");
		cmd_args.append(s);
	}
	cmd_args.append("$!");
	return cmd_args;
}

void Arduino::send_cmd_str(std::string cmd)
{
	char* cmd_arr = &cmd[0];
	simpleSerial->WriteSerialPort(cmd_arr);
}

Arduino::Arduino(char com_port[], DWORD COM_BAUD_RATE)
{
	simpleSerial = new SimpleSerial(com_port, COM_BAUD_RATE);
	Sleep(3000);
}

void Arduino::digitalWrite(int pin, bool val)
{
	std::string cmd;

	if (val)
		cmd = build_cmd_str("dw", { std::to_string(pin)});
	else
		cmd = build_cmd_str("dw", { "-" + std::to_string(pin)});

	send_cmd_str(cmd);
}

void Arduino::pinMode(int pin, bool mode)
{
	std::string cmd;

	if (mode)
		cmd = build_cmd_str("pm", { "-" + std::to_string(pin)});
	else
		cmd = build_cmd_str("pm", { std::to_string(pin)});

	send_cmd_str(cmd);
}

int Arduino::analogRead(int pin)
{
	std::string cmd = build_cmd_str("ar", { std::to_string(pin) });
	send_cmd_str(cmd);
	Sleep(10);

	std::string response = simpleSerial->ReadSerialPort(1000);
	try
	{		
		int resp = std::stoi(response);
		return resp;
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

void Arduino::tone(int pin, float freq)
{
	std::string cmd = build_cmd_str("to", { std::to_string(pin), std::to_string(freq) });
	send_cmd_str(cmd);
}

void Arduino::noTone(int pin)
{
	std::string cmd = build_cmd_str("nt", { std::to_string(pin) });
	send_cmd_str(cmd);
}
