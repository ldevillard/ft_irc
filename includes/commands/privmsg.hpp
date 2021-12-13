#pragma once

#include "../command.hpp"
#include "../client.hpp"
#include <vector>

class Command;

class Privmsg : public Command
{
	public:
		Privmsg(Client *user);
		void execute();
};