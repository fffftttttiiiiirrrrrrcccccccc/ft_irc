#include "Server.hpp"
#include <string.h>

Server::Server(){}
Server::~Server(){}

Server::Server(int port, char *pass) {
	_port = port;
	_password = pass;
}

int Server::getSocket() {
	return _serverSocket;
}


void Server::sockCreat() {

	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0) {
		std::cout << "Create socket failed" << std::endl;
		exit(1);
	}


	sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(_port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(socketfd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		std::cout << "Socket bind failed" << std::endl;
		exit(1);
	}
	_serverSocket = socketfd;
	
}


void Server::addServerSocket() {
	struct pollfd fd;
	fd.events = POLLIN;
	fd.fd = _serverSocket;
	fd.revents = 0;
	_fds.push_back(fd);
}

std::vector<struct pollfd> Server::getPollfd() {
	return _fds;
}

void	Server::runServer() {
	int ret = 0;
	while (1) {
		// std::cout << "123" << std::endl;
		ret = poll(&_fds.front(), _fds.size(), -1);
		if (ret < 0) {
			std::cout << "Poll error" << std::endl;
			exit(1);
		}
		if (_fds[0].revents && POLLIN) {
			int clientSocketfd = accept(_serverSocket, NULL, NULL);
			if (clientSocketfd < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
					std::cout << "Socket accept failed" << std::endl;
					exit(1);
			}
			struct pollfd fd;
			fd.events = POLLIN;
			fd.fd = clientSocketfd;
			fd.revents = 0;
			_fds.push_back(fd);
			_clients[clientSocketfd].setSocketfd(clientSocketfd);
			_clients[clientSocketfd].setNickName("");
			_clients[clientSocketfd].setUserName("");
			_clients[clientSocketfd].setPassword("");
		}
		for (int i = 1; i <= _fds.size(); i++) {
			// std::cout << "123555"<<std::endl;
			if (_fds[i].revents && POLLIN) {
				char buffer[1024];
				memset(buffer, 0, sizeof(buffer));
				ret = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
				if (ret == -1){ //&& errno != EAGAIN && errno != EWOULDBLOCK){
					// std::cout << "123"<<std::endl;
					close(_fds[i].fd);
					_clients.erase(_fds[i].fd);
					std::cout << "recv function failed" << std::endl;
					exit(1);
				}
				else if (ret == 0){
					// std::cout << "1234"<<std::endl;
					std::cout << "Disconnet client" << _clients[_fds[i].fd].getNickName() << std::endl;
					// client 삭제하는 함수 구현. exit 찍으면 안됨.
					exit(1);
				}
				else {
					std::cout << buffer <<std::endl;
					buffer[ret] = '\0';
					write(_fds[i].fd, buffer, ret);
					// std::cout << "Get socket data : " << buffer << std::endl;
					
				}
			}
		}
	}
}