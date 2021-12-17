#include "../../includes/commands/pass.hpp"
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../includes/message.hpp"

Pass::Pass(Client *user) : Command("PASS", "PASS - <password> : Authenticate to password protected servers.", user)
{
}

void Pass::execute()
{

	if (!_server->needPsswd())
	{
		_user->setPsswdState(true);
	}
	else if (_server->passwordAuth(_args[1]))
	{
		_user->setPsswdState(true);
	}
	else
	{
		_user->sendMsg(":server " + std::string(ERR_PASSWDMISMATCH) + " : Password incorrect!");
		_user->sendMsg(":server KILL " + _user->getAddress() + " :Bad password");
		_user->setKill(true);
	}
}