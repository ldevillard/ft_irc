#pragma once

#include <netdb.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>	   //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#include <map>
#include <sstream>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <iostream>
#include <vector>
#include <map>
#include "./serverException.hpp"
#include "channel.hpp"
#include "client.hpp"
#include "../server_config.hpp"

class ServData
{

public:
	ServData(id_t port, std::string password);
	~ServData();
	int connect();
	Client *getUserFromUsername(std::string name);
	Client *getUser(std::string name);
	std::map<std::string, Channel *> &getChannels();
	Channel *findChannel(std::string name);
	void shutdownServer();
	std::vector<Channel *> findChannelsOfUser(Client *user);
	std::vector<Client *> getVectorUser();
	std::vector<std::string> *getBanWordsList();
	bool passwordAuth(std::string check);
	bool needPsswd(void);

private:
	ServData();

	struct sockaddr_in _address;
	std::string _msg;
	size_t _addrlen;
	fd_set _read_fds;
	Client *_clients[CFG_MAXCLIENTS];
	int _activity;
	int _master_socket;
	int _max_sd;
	int _new_socket;
	int _opt;
	int _port;
	int _sd;
	std::string _password;
	bool _needPsswd;
	std::map<std::string, Channel *> _chan_list;
	bool shutdown;
	std::vector<std::string> _banWordsList;

	void setup();
	void onInteraction();
	void onConnection();
	void setupFD();
};
