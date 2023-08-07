#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include <iostream>
#include <map>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>
#include <vector>
#include <sstream>


class Client;
class Channel;

class Server{
	private:
		std::map<int, Client> _clients; // fd, client
		std::map<std::string, Channel> _channels; // channelName channel
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
		void	get_command(std::string buffer, int fd);

		void exitClient(int fd);
		int findPollfdIndex(int fd);
		void removeClientInServer(int fd);
		void commandQuit(std::string argument, int fd);
		void commandJoin(std::string argument, int fd);
};

#endif