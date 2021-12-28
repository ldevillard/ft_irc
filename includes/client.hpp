#pragma once
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

class ServData;
class Channel;

class Client
{
public:
	Client(ServData *server);
	const std::string &getUserName() const;
	const std::string &getNickName() const;
	std::string &getRealName();
	const std::string &getAddress() const;
	const bool &isRegistered() const;
	const int &getSd() const;
	void setSd(int sd);
	std::string &getBufferLine();
	void setUserName(std::string name);
	void setNickName(std::string name);
	void setRealName(std::string name);
	void setAddress(std::string address);
	void setRegistered(bool state);
	void sendMsg(std::string msg);
	void disconnect();
	void setPsswdState(bool state);
	bool getPsswdState(void);
	void setKill(bool state);
	bool getKill(void);
	ServData *getServer(void);

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
	ServData *_server;
};

#include "servData.hpp"