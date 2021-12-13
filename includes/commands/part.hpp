#pragma once

#include "../command.hpp"
#include "../user.hpp"
#include <vector>

class Command;

class Part : public Command
{
	public:
		Part(User *user);
		void execute();
};