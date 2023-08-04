#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include <iostream>
#include <map>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>
#include <vector>

class Client;
class Channel;

class Server{
	private:
		std::map<int, Client> _clients;
		std::map<std::string, Channel> _channels;
		std::string _password;
		std::vector<struct pollfd> _fds;
		int _port;
		int _serverSocket;

	public:
		Server();
		virtual ~Server();
		Server(int port, char* pass);
		Server(const Server &src);
		Server &operator=(const Server &rhs);

		void	sockCreat();
		int		getSocket();
		void	addServerSocket();
		std::vector<struct pollfd> getPollfd();
		void	runServer();
};

#endif