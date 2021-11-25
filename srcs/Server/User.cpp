#include "User.hpp"
#include <iostream>

User::User()
{
	_user.clear();
	_nick.clear();
}

int User::recoverData(char *buf)
{
	if (check_info() == 2)
	{
		std::string nick(buf);
		return (setNickFirstTime(nick));
	}
	else if (check_info() == 1)
	{
		std::string username(buf);
		return (setUserFirstTime(username));
	}
	return 0;
}

int User::check_info()
{
	if (_nick.size() == 0)
		return 2;
	if (_user.size() == 0)
		return 1;
	return 0;
}

int User::setNickFirstTime(std::string &nick)
{
	int i = 0;

	while (isalnum(nick[i]) && i < (int)nick.size())
		i++;
	i++;

	while (i < (int)nick.size() - 2)
	{
		if (!isalnum(nick[i]))
		{
			_nick.clear();
			return 2;
		}
		_nick += nick[i];
		i++;
	}
	return 0;
}

int User::setUserFirstTime(std::string &user)
{
	int i = user.size() - 1;
	int j = 0;

	while (user[i] != '*')
		i--;
	i -= 3;

	while (isalnum(user[j]) && j < i)
		j++;
	j++;

	while (j < i)
	{
		if (!isalnum(user[j]))
		{
			_user.clear();
			return 1;
		}
		_user += user[j];
		j++;
	}
	return 0;
}