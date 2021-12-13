#include "../../includes/commands/user.hpp"
#include <iostream>
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../includes/message.hpp"

User::User(Client *user) : Command("USER", "<username> <hostname> <servername> <realname> : Join wanted channel.", user)
{
}

void User::execute()
{
}