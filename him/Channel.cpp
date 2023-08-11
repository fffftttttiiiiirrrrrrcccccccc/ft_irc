#include "Channel.hpp"

Channel::Channel(){
		_channelName = "";
		_isInviteOnly = false;
		_isTopic = false;
		_topic = "";
		_isKey = false;
		_password = "";
		_isOperator = false;
		_isLimit = false;
		_LimitClientNum = 50;
}

Channel::Channel(std::string channelName){
		_channelName = channelName;
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

std::string Channel::getTopic(){
	return _topic;
}

std::string Channel::getPassword() {
	return _password;
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
		_clients[fd] = client;
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

void Channel::partClinet(int fd){
	removeClinetInChannel(fd);
}

bool Channel::isOpClient(int fd){
	std::vector<int>::iterator it = std::find(_opList.begin(), _opList.begin(), fd);
	if (it == _opList.end())
		return false;
	return true;
}

bool Channel::isInClinet(int fd) {
	std::map<int, Client *>::iterator it = _clients.find(fd);
	if (it == _clients.end())
		return false;
	return true;
}

bool	Channel::getIsLimitMode(){
	return _isLimit;
}
bool	Channel::getIsOperatorMode(){
	return _isOperator;
}
bool	Channel::getIsInviteMode(){
	return _isInviteOnly;
}

bool	Channel::getIsTopicMode(){
	return _isTopic;
}

bool	Channel::getIsKeyMode(){
	return _isKey;
}

bool	Channel::isJoinalbe() {
	if (_isLimit && _clients.size() >= _LimitClientNum)
		return false;
	return true;
}

std::vector<int> Channel::getInviteList(){
	return _inviteList;
}

void Channel::addInvieList(int fd){
	std::vector<int>::iterator it = std::find(_inviteList.begin(), _inviteList.end(), fd);
	if (it == _inviteList.end())
		_inviteList.push_back(fd);
}

bool Channel::isInviteClient(int fd){
	std::vector<int>::iterator it = std::find(_inviteList.begin(), _inviteList.end(), fd);
	if (it != _inviteList.end())
		return true;
	return false;
}

bool	Channel::inviteJoinClient(int fd, Client *client) {
	if (isInviteClient(fd)){
		_clients[fd] = client;
		std::vector<int>::iterator it = std::find(_inviteList.begin(), _inviteList.end(), fd);
		_inviteList.erase(it);
		return true;
	}
	return false;
		
}

std::string Channel::getChannelName(){
	return _channelName;
}

std::vector<int> Channel::getClientsFd(){
	std::vector<int> ret;
	std::cout << std::endl;
	std::cout << _channelName << " fd list : ";
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++ ){
		ret.push_back(it->first);
		std::cout << it->first;
	}
	std::cout << std::endl;
	return ret;
}