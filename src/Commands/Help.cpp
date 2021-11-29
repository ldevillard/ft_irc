#include "../../includes/Commands/Help.hpp"

Help::Help() : Command("HELP", "Display all the commands and descriptions.")
{
}

void Help::execute() const
{
	std::cout << "Execute help command!" << std::endl;
}