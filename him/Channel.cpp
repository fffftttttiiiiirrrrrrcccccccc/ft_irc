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

std::map<std::string, Client &> Channel::getClients() {
	return _clients;
}

void Channel::addClinetInChannel(std::string nickName, Client& client, std::string password) {
	std::map<std::string, Client &>::iterator it = _clients.find(nickName);
	if (it == _clients.end()) {
		if (_isInviteOnly)
			return ;
		if (_isKey && _password != password)
			return ;
		if (_isLimit && _clients.size() == _LimitClientNum)
			return ;
		
		_clients[nickName] = client;
		std::cout << "ccc" << std::endl;
	}
}

void Channel::removeClinetInChannel(std::string nickName) {
	std::map<std::string, Client &>::iterator it = _clients.find(nickName);
    if (it != _clients.end())
		_clients.erase(it);
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

void Channel::removeOpClient(std::string nickname){
	std::vector<std::string>::iterator it = std::find(_opList.begin(), _opList.end(), nickname);
	if (it != _opList.end())
		_opList.erase(it);
}

void Channel::addOpClinet(std::string nickname){
	std::vector<std::string>::iterator it = std::find(_opList.begin(), _opList.end(), nickname);
	if (it != _opList.end())
		_opList.push_back(nickname);
}