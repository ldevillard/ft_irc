#pragma once

#include <string>
#include <iostream>
#include "servData.hpp"

class Command
{
	public:
		Command(std::string name, std::string desc);
		virtual ~Command();

		std::string getDesc() const { return _desc; }
		std::string getName() const { return _name; }

		void setServer(ServData *server) { _server = server; }

		virtual void execute() = 0;

	protected:
		std::string _name;
		std::string _desc;

		ServData *_server;
};