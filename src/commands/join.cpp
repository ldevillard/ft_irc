#include "../../includes/commands/join.hpp"
#include <iostream>

Join::Join() : Command("JOIN", "<channel> <password> : Join wanted channel.")
{
}

void Join::execute()
{
	std::cout << "Executing Join!" << std::endl;
}