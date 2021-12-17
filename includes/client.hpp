#pragma once
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

class Client
{
public:
	Client()
	{
		_isRegistered = false;
		_correctPsswd = false;
		_user.clear();
		_nick.clear();
		_realName.clear();
	}

	int recoverData(std::string line);

	const std::string &getUserName() const { return _user; }
	const std::string &getNickName() const { return _nick; }
	std::string &getRealName() { return _realName; }
	const std::string &getAddress() const { return _address; }
	const bool &isRegistered() const { return _isRegistered; }

	const int &getSd() const { return _sd; }
	void setSd(int sd) { _sd = sd; }
	std::string &getBufferLine() { return _bufferLine; }

	void setUserName(std::string name)
	{
		if (name != "none")
			_user = name;
	}
	void setNickName(std::string name)
	{
		if (name != "none")
			_nick = name;
	}
	void setRealName(std::string name)
	{
		if (name != "none")
			_realName = name;
	}
	void setAddress(std::string address) { _address = address; }
	void setRegistered(bool state) { _isRegistered = state; }

	void sendMsg(std::string msg)
	{
		std::cout << "-> [" << _sd << "] " << msg << std::endl;
		send(_sd, (msg + "\r\n").c_str(), (msg + "\r\n").length(), 0);
	}

	int check_info(); //0 if OK, 1 if no user set, 2 if no nick set

	void disconnect() { close(_sd); }

	void setPsswdState(bool state)
	{
		_correctPsswd = state;
	}

	bool getPsswdState(void)
	{
		return _correctPsswd;
	}

	void setKill(bool state)
	{
		_getKilled = state;
	}

	bool getKill(void)
	{
		return _getKilled;
	}

private:
	std::string _user;
	std::string _nick;
	std::string _realName;
	int _sd;
	std::string _bufferLine;
	std::string _address;
	bool _isRegistered;
	bool _correctPsswd;
	bool _getKilled;
};
