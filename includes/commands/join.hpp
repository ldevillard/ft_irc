#pragma once

#include "../command.hpp"

class Command;

class Join : public Command
{
	public:
		Join();
		void execute();
};