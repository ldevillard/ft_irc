#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include "../includes/client.hpp"

class message
{
private:
public:
	static std::string response(int response_code, std::string name, std::string command, std::string message)
	{
		std::string code = std::to_string(response_code);
		command = command.size() ? command + " " : command;

		std::string res = ":127.0.0.1/6667 " + code + " " + name + " " + command + message;
		return res;
	}
	static void sendMsgToUser(Client *user, std::string str)
	{
		user->sendMsg(str);
	}
};
