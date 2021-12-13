#include "../../includes/commands/join.hpp"
#include <iostream>
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../includes/message.hpp"

Join::Join(User *user) : Command("JOIN", "<channel> <password> : Join wanted channel.", user)
{
}

void Join::execute()
{
	if (_args[1] != "#")
	{
		Channel *chan = _server->findChannel(_args[1]);

		if (chan == NULL)
		{
			chan = new Channel(_args[1]);
			_server->getChannels().insert(std::make_pair(_args[1], chan));
		}

		chan->broadcastMsg(_args[1]);
		chan->join(_user);
		//_user->sendMsg(message::response(RPL_INFO, _user->getNick(), "", ":joining " + _args[1] + " channel"));
		//_user->sendMsg(":" + _user->getNick() + "!" + _user->getNick() + "@" + _user->getAddress() + " JOIN " + _args[1]);

		//_user->sendMsg( ":"+_user->getAddress()+" 332 " + _user->getNick() + _args[1] + " :generic channel");
		// _user->sendMsg(":" + _user->getAddress() + " 353 " + _user->getNick() + " = " + _args[1] + " :@" + _user->getNick());
		//_user->sendMsg( ":"+_user->getAddress()+" 366 gauthier #salut :End of NAMES list");
	}
}