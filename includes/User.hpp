#pragma once

#include <string>

class User
{
	public:
		User();

		int recoverData(std::string line);

		const std::string &getUser() const { return _user; }
		const std::string &getNick() const { return _nick; }

		int setUserFirstTime(std::string &user);
		int setNickFirstTime(std::string &nick);

		int check_info(); //0 if OK, 1 if no user set, 2 if no nick set

	private:
		std::string _user;
		std::string _nick;

};