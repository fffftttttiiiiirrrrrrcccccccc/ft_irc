#include "Client.hpp"

Client::Client(){};

Client::~Client(){};

void Client::setNickName(std::string str){
	_nickName = str;
}
void Client::setUserName(std::string str){
	_userName = str;
}
void Client::setPassword(std::string str){
	_password = str;
}

void Client::setSocketfd(int fd){
	_socketfd = fd;
}

std::map<std::string, Channel &> Client::getChannels() {
	return _clientChannels;
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
