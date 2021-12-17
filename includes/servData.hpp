#pragma once

#include "./serverException.hpp"
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
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <poll.h>	  // Poll() (yeah no shit)
#include <iostream>
#include "client.hpp"
#include <vector>
#include <map>
#include "channel.hpp"
#include <sstream>

#define MAX_CLIENTS 5

class ServData
{

public:
	ServData(id_t port, std::string password);
	~ServData();
	int connect();
	Client *getUser(std::string name);
	std::map<std::string, Channel *> &getChannels() { return _chan_list; }

	Channel *findChannel(std::string name)
	{
		std::map<std::string, Channel *>::iterator it;

		for (it = _chan_list.begin(); it != _chan_list.end(); it++)
		{
			if (name == (*it).first)
				return (*it).second;
		}
		return (NULL);
	}

	void shutdownServer()
	{
		shutdown = true;
	}

	std::vector<Channel *> findChannelsOfUser(Client *user)
	{
		std::vector<Channel *> tab;

		for (std::map<std::string, Channel *>::iterator it = _chan_list.begin(); it != _chan_list.end(); it++)
		{
			if ((*it).second->isUserInChannel(user) == true)
				tab.push_back((*it).second);
		}
		return tab;
	}

	std::vector<Client *> getVectorUser()
	{
		std::vector<Client *> users;

		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			if (_clients[i] != NULL)
				users.push_back(_clients[i]);
		}

		return users;
	}

private:
	ServData();

	struct sockaddr_in _address;
	std::string _msg;
	size_t _addrlen;
	fd_set _read_fds;
	Client *_clients[MAX_CLIENTS];
	int _activity;
	int _master_socket;
	int _max_clients;
	int _max_sd;
	int _new_socket;
	int _opt;
	int _port;
	int _sd;
	std::string _password;
	std::map<std::string, Channel *> _chan_list;
	bool shutdown;

	void setup();
	void onInteraction();
	void onConnection();
	void setupFD();
};
