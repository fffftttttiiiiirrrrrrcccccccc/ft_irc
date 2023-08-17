#include "Client.hpp"

Client::Client(){};

Client::~Client(){};

void Client::initClient(int fd) {
    _clientChannels.clear();
	_fd = fd;
	_nickName = "*";
	_userName = "*";
	_hostName = "*";
	_serverName = "*";
	_realName = "*";
	_password = "";
	_cmdBuffer = "";
	_isLogin = false;
}

void Client::setHostName(std::string str){
	_hostName = str;
}
void Client::setServerName(std::string str){
	_serverName = str;
}
void Client::setRealName(std::string str){
	_realName = str;
}
std::string Client::getHostName(){
	return _hostName;
}
std::string Client::getServerName(){
	return _serverName;
}
std::string Client::getRealName(){
	return _realName;
}

void Client::setTmpCmd(std::string str) {
	_cmdBuffer = str;
}

void Client::addTmpCmd(std::string str) {
	_cmdBuffer += str + " ";
}

void Client::setNickName(std::string str){
	_nickName = str;
	welcomeMsg();
}
void Client::setUserName(std::string str){
	_userName = str;
	welcomeMsg();
}
void Client::setPassword(std::string str){
	_password = str;
	welcomeMsg();
}

void Client::setFd(int fd){
	_fd = fd;
}

int Client::getFd(){
	return _fd;
}

std::map<std::string, Channel *> Client::getChannels() {
	return _clientChannels;
}
std::string Client::getTmpCmd(){
	return _cmdBuffer;
}

std::string Client::getNickName(){
	return _nickName;
}
std::string Client::getUserName(){
	return _userName;
}
std::string Client::getPassword(){
	return _password;
}
bool Client::get_isLogin(){
	return _isLogin;
}

void Client::welcomeMsg(){
	if (_nickName != "*" && _password != "" && _userName != "*" && !_isLogin){
		// const char* motdEndMsg = "001 <redic> :Welcome to the Internet Relay Network\r\n";
		// send(_fd, motdEndMsg, strlen(motdEndMsg), 0);
		_isLogin = true;
	}
}

void Client::addChannel(Channel *channel){
	_clientChannels[channel->getChannelName()] = channel;
}

bool Client::findChannel(std::string channelName){
	std::map<std::string, Channel *>::iterator it = _clientChannels.find(channelName);
	if (it == _clientChannels.end())
		return false;
	return true;
}

bool Client::removeChannel(std::string channelName){
	std::map<std::string, Channel *>::iterator it = _clientChannels.find(channelName);
	if (it == _clientChannels.end())
		return false;
	_clientChannels.erase(it);
	return true;
}



