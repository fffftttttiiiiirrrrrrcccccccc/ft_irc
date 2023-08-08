#include "Channel.hpp"

Channel::Channel(){
		_isInviteOnly = false;
		_isTopic = false;
		_topic = "";
		_isKey = false;
		_password = "";
		_isOperator = false;
		_isLimit = false;
		_LimitClientNum = 50;
}

std::map<int, Client *> Channel::getClients() {
	return _clients;
}

void Channel::addClinetInChannel(int fd, Client* client, std::string password) {
	std::map<int, Client *>::iterator it = _clients.find(fd);
	if (it == _clients.end()) {
		if (_isInviteOnly)
			return ;
		if (_isKey && _password != password)
			return ;
		if (_isLimit && _clients.size() == _LimitClientNum)
			return ;
		std::cout << "!23" << std::endl;
		_clients[fd] = client;
		std::cout << "ccc" << std::endl;
		if (_clients.size() == 1)
			_opList.push_back(fd);
	}
}

void Channel::removeClinetInChannel(int fd) {
	std::map<int, Client *>::iterator it = _clients.find(fd);
    if (it != _clients.end())
		_clients.erase(it);
	removeOpClient(fd);
}

void	Channel::setIsInviteOnly(bool isInviteOnly){
	_isInviteOnly = isInviteOnly;
}
void	Channel::setIsTopic(bool isTopic){
	_isTopic = isTopic;
}
void	Channel::setIsKey(bool isKey){
	_isKey = isKey;
}
void	Channel::setIsOperator(bool isOperator){
	_isOperator = isOperator;
}
void	Channel::setIsLimit(bool isLimit){
	_isLimit = isLimit;
}
void	Channel::setPassword(std::string password){
	_password = password;
}
void	Channel::setTopic(std::string topic){
	_topic = topic;
}
void	Channel::setLimitClientNum(int num){
	_LimitClientNum = num;
}

void Channel::removeOpClient(int fd){
	std::vector<int>::iterator it = std::find(_opList.begin(), _opList.end(), fd);
	if (it != _opList.end())
		_opList.erase(it);
}

void Channel::addOpClinet(int fd){
	std::vector<int>::iterator it = std::find(_opList.begin(), _opList.end(), fd);
	if (it != _opList.end())
		_opList.push_back(fd);
}

void Channel::partClinet(int fd, std::string msg){
	removeClinetInChannel(fd);
	if (msg != "")
		;// 채널에 메세지 남기기

}