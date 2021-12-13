#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class Nick : public Command
{
	public:
		Nick(Client *user);
		void execute();
};