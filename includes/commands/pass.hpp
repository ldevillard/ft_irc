#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class Pass : public Command
{
public:
	Pass(Client *user);
	void execute();
};
