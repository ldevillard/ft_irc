#pragma once

#include "../command.hpp"
#include "../user.hpp"

class Command;

class Nick : public Command
{
	public:
		Nick(User *user);
		void execute();
};