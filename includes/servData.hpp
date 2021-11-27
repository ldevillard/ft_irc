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

#define SOCKET_BUFFER_SIZE 2048

class ServData
{

public:
	ServData(id_t port);
	~ServData();
	int connect();

private:
	ServData();
	struct sockaddr_in _address;
	char _buffer[SOCKET_BUFFER_SIZE];
	std::string _msg;
	int _port;
	int _valread;
	int _opt;
	int _max_clients;
	int _client_sockets[5];
	int _sd;
	int _max_sd;
	int _activity;
	int _master_socket;
	size_t _addrlen;
	int _new_socket;
	fd_set _read_fds;
	void	setup();
};
