#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class Notice : public Command
{
public:
	Notice(Client *user);
	void execute();
};