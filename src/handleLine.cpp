#include <string>
#include <iostream>
#include <sys/socket.h>
#include "../includes/channel.hpp"
#include "../includes/servData.hpp"
#include <sstream>
#include "../includes/rpl_codes.hpp"

std::string c_itoa(int n)
{
	std::stringstream str;
	str << n;
	std::string res;
	str >> res;
	return res;
}

// std::string create_msg(int code, User &user, ServData &server, std::string arg1, std::string arg2)
// {
// 	std::string sample;
// 	std::string trueCode;

// 	trueCode = c_itoa(code);

// 	switch (code)
// 	{
// 	case 1:

// 		break;

// 	default:
// 		break;
// 	}
// }

void send_to_chan(channel &channel)
{
	(void)channel;
}

std::string response(int response_code, std::string name, std::string command, std::string message)
{
	std::string code = c_itoa(response_code);
	command = command.size() ? command + " " : command;

	std::string response = ":127.0.0.1/6667 " + code + " " + name + " " + command + message + "\r\n";
	return response;
}

void handleLine(int sd, int uid, std::string line)
{
	std::cout << "* <" << uid << "> " << line << std::endl;

	// if (line == "JOIN #salut")
	// {
	// std::string msg = response(RPL_INFO, "gapoulain", "", ":pouet");
	// std::cout << "send \"" << msg << "\"" << std::endl;
	// send(sd, msg.c_str(), msg.length(), 0);
	// }

	send(sd, line.c_str(), line.length(), 0);
}
