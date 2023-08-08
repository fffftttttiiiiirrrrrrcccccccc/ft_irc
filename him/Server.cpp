#include "Server.hpp"

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
			_clients[clientSocketfd].initClient(clientSocketfd);
		}
		for (unsigned long i = 1; i <= _fds.size(); i++) {
			if (_fds[i].revents && POLLIN) {
				char buffer[1024];
				memset(buffer, 0, sizeof(buffer));
				ret = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
				if (ret == -1 && errno != EAGAIN && errno != EWOULDBLOCK){
					close(_fds[i].fd);
					close(_serverSocket);
					_clients.erase(_fds[i].fd);
					std::cout << "recv function failed" << std::endl;
					exit(1);
				}
				else if (ret == 0){
					std::map<int, Client>::iterator it = _clients.find(_fds[i].fd);
					_clients.erase(it);
					close(_fds[i].fd);
					_fds.erase(_fds.begin() + i);
					std::cout << "Disconnet client" << _clients[_fds[i].fd].getNickName() << std::endl;
					continue;
					// client 삭제하는 함수 구현. exit 찍으면 안됨.
					// exit(1);
				}
				else {
					std::cout << buffer <<std::endl;
					buffer[ret] = '\0';
					get_command(buffer, _fds[i].fd);
					write(_fds[i].fd, buffer, ret);
					// std::cout << "Get socket data : " << buffer << std::endl;
				}
			}
		}
	}
}

void	Server::get_command(std::string buffer, int fd) {
	std::istringstream	str(buffer);
	std::string			command;
	std::string			argument;

	if (fd)
		;
	while (std::getline(str, command, ' ')){
		if (command == "CAP")
			continue;
		std::getline(str, argument);
		std::cout << "Command : " << command << std::endl;
		std::cout << "Argument : " << argument << std::endl;
		if(command == "quit" || command == "QUIT")
			commandQuit(argument, fd);
		else if(command == "join" || command == "JOIN")
			commandJoin(argument, fd);
		else if(command == "nick" || command == "NICK")
			commandNick(argument, fd);
		else if(command == "pass" || command == "PASS")
			commandPass(argument, fd);
		else if(command == "user" || command == "USER")
			commandUser(argument, fd);	
	}
	
	
}

void Server::exitClient(int fd) {
	Client client = _clients[fd];
	std::string nickName = client.getNickName();
	std::map<std::string, Channel *> channel = client.getChannels();
	// std::__1::map<int, Client>::iterator servetIt = _clients.find(fd);
	for (std::map<std::string, Channel *>::iterator ChannelIt = channel.begin();
		ChannelIt != channel.end(); ChannelIt++) {
			Channel *tmpChannel = ChannelIt->second;
			tmpChannel->removeClinetInChannel(fd);
	}
	removeClientInServer(fd);
}

void Server::removeClientInServer(int fd) {
	std::map<int, Client>::iterator it = _clients.find(fd);
	_clients.erase(it);
	close(fd);
	int i = findPollfdIndex(fd);
	_fds.erase(_fds.begin() + i);
}

// QUIT 시 해야할 것
// 1. Server에서 Client 삭제
// 2. Client에서 들어가있는 Channel돌면서 Client삭제
// 3. Client _fds 닫기
// 4. Vector <poolfd> _fds 에서 Client fd 삭제
int Server::findPollfdIndex(int targetFd) {
    for (size_t i = 0; i < _fds.size(); ++i) {
        if (_fds[i].fd == targetFd) {
            return static_cast<int>(i);
        }
    }
	std::cout << "can't find Pollfd Index" <<std::endl;
    return -1; // Return -1 if the targetFd is not found in the vector
}

void	Server::commandQuit(std::string argument, int fd) {
	exitClient(fd);
	std::cout << argument << std::endl;
}

void	Server::commandJoin(std::string argument, int fd) {
	std::istringstream	str(argument);
	
	std::string			input;
	std::string			channel;
	std::string			password;

	while (std::getline(str, input, ' ')) {
		std::stringstream ss(input);
		ss >> channel >> password;
		std::map<std::string, Channel>::iterator channelIt = _channels.find(channel);
		if (channelIt == _channels.end()) {
			Channel newChannel;
			_channels[channel] = newChannel;
			_channels[channel].addClinetInChannel(fd, &_clients[fd], password);
			std::cout << "127" << std::endl;
		}else { //채널이 있다면 채널의 isInvite가 트루인지 확인하여 접속
			channelIt->second.addClinetInChannel(fd, &_clients[fd], password);
		}
	}
}

void Server::commandNick(std::string argument, int fd) {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second.getNickName == argument)
			return ;
	}
	_clients[fd].setNickName(argument);
}

void Server::commandPass(std::string argument, int fd) {
	_clients[fd].setPassword(argument);
}

void Server::commandUser(std::string argument, int fd) {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second.getUserName == argument)
			return ;
	}
	_clients[fd].setUsername(argument);
}