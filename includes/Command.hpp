#pragma once

#include <string>
#include <iostream>

class Command
{
	public:
		Command(std::string name, std::string desc);
		virtual ~Command();

		std::string getDesc() const { return _desc; }
		std::string getName() const { return _name; }

		virtual void execute() const = 0;

	protected:
		std::string _name;
		std::string _desc;
};