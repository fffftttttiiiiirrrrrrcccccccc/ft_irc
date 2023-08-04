#include "Client.hpp"

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