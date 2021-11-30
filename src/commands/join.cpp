#include "../../includes/commands/join.hpp"
#include <iostream>

Join::Join(User *user) : Command("JOIN", "<channel> <password> : Join wanted channel.", user)
{
}

void Join::execute()
{
	std::cout << _user->getNick() + ": Executing Join!" << std::endl;
}