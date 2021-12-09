#pragma once
#include <string>
#include <sys/socket.h>

class User
{
public:
	User();

	int recoverData(std::string line);

	const std::string &getUser() const { return _user; }
	const std::string &getNick() const { return _nick; }
	const std::string &getAddress() const { return _address; }

	const int &getSd() const { return _sd; }
	void setSd(int sd) { _sd = sd; }
	std::string &getBufferLine() { return _bufferLine; }

	int setUserFirstTime(std::string &user);
	int setNickFirstTime(std::string &nick);

	void setUserName(std::string name) { _user = name; }
	void setNickName(std::string name) { _nick = name; }
	void setAddress(std::string address) { _address = address; }

	void sendMsg(std::string msg)
	{
		std::cout << "-> [" << _sd << "] " << msg << std::endl;
		send(_sd, (msg + "\r\n").c_str(), (msg + "\r\n").length(), 0);
	}

	int check_info(); //0 if OK, 1 if no user set, 2 if no nick set

private:
	std::string _user;
	std::string _nick;
	int _sd;
	std::string _bufferLine;
	std::string _address;
};
