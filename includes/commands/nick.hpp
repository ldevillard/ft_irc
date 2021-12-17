#pragma once

#include "../command.hpp"
#include "../client.hpp"

class Command;

class Nick : public Command
{
private:
	bool charset(std::string &nick);

public:
	Nick(Client *user);
	void execute();
};
