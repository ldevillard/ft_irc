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

void sendMsg(User *user, std::string str)
{
	if (user)
	{

		std::cout << "-> [" << user->getSd() << "] " << str << std::endl;
		send(user->getSd(), str.c_str(), str.length(), 0);
	}
}

std::string response(int response_code, std::string name, std::string command, std::string message)
{
	std::string code = c_itoa(response_code);
	command = command.size() ? command + " " : command;

	std::string response = ":127.0.0.1/6667 " + code + " " + name + " " + command + message + "\r\n";
	return response;
}
