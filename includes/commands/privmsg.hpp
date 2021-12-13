#pragma once

#include "../command.hpp"
#include "../user.hpp"
#include <vector>

class Command;

class Privmsg : public Command
{
	public:
		Privmsg(User *user);
		void execute();
};