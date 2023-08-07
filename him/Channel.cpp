#include "Channel.hpp"

std::map<std::string, Client &> Channel::getClients() {
	return _clients;
}

void Channel::removeClinetInChannel(std::string nickName) {
	std::map<std::string, Client &>::iterator it = _clients.find(nickName);
    if (it != _clients.end())
		_clients.erase(it);
}