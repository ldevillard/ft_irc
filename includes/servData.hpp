#pragma once

#include "./ServerException.hpp"
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
#include "../includes/servData.hpp"
#include "../includes/User.hpp"
#include "../includes/Parser.hpp"
#include "User.hpp"

class ServData
{

public:
	ServData(id_t port, std::string password);
	~ServData();
	int connect();

private:
	ServData();

	struct sockaddr_in _address;
	char _buffer;
	std::string _msg;
	size_t _addrlen;
	fd_set _read_fds;
	int _client_sockets[5];
	int _activity;
	int _master_socket;
	int _max_clients;
	int _max_sd;
	int _new_socket;
	int _opt;
	int _port;
	int _sd;
	int _valread;
	std::string _password;
	std::vector<User> _users;

	void setup();
	void onInteraction();
	void onConnection();
	void setupFD();
};
