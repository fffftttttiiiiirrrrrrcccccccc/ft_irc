#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include <iostream>
#include <map>

class Client;
class Channel;

class Server{
	private:
		std::map<Cli> _clients;

	public:
};

#endif