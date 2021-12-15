#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class Quit : public Command
{
public:
	Quit(Client *user);
	void execute();
};