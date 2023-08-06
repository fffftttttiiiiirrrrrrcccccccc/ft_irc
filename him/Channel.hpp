#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>
#include "Client.hpp"

class Client;

class Channel {
	private:
		std::map<std::string, Client &> _clients; //<nickname, cli 참조>
		// 맵 <client& 객체, number 권한>
		// 맵 <
		// map["str"];
		// map["cli"];
	public:
		std::map<std::string, Client &> getClients();
		void removeClinet(std::string nickName);


};

#endif