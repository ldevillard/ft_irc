#include "../includes/channel.hpp"
#include "../includes/message.hpp"

void Channel::join(Client *user)
{
	message::sendMsgToUser(user, ":" + user->getNickName() + "!" + user->getNickName() + "@" + user->getAddress() + " JOIN " + _channelName);
	_members.push_back(user);
}

void Channel::leave(Client *user)
{
	message::sendMsgToUser(user, ":" + user->getNickName() + "!" + user->getNickName() + "@" + user->getAddress() + " PART " + _channelName);

	std::vector<Client*>::iterator it = _members.begin();
	while ((*it) != user)
		it++;
	_members.erase(it);
}