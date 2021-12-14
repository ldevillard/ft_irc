#include "../../includes/commands/die.hpp"
#include "../../includes/rpl_codes.hpp"

Die::Die(Client *user) : Command("DIE", "DIE - Shutdown the server.", user)
{
}

void Die::execute()
{
    _server->shutdownServer();
}