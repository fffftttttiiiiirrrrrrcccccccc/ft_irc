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
			const char* motdStartMsg = "375 <redic> :- Welcome to our IRC server.\r\n";
			send(fd.fd, motdStartMsg, strlen(motdStartMsg), 0);
			const char* motdMsg1 = "372 <redic> :- This is the Message of the Day.\r\n";
			send(fd.fd, motdMsg1, strlen(motdMsg1), 0);
			const char* motdEndMsg = "376 <redic> :End of MOTD command.\r\n";
			send(fd.fd, motdEndMsg, strlen(motdEndMsg), 0);

		}
		for (unsigned long i = 1; i <= _fds.size(); i++) {
			if (_fds[i].revents && POLLIN) {
				char buffer[1025];
				memset(buffer, 0, 1025);
				ret = recv(_fds[i].fd, buffer, 1024, 0);
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

	if (fd)
		;
	std::cout << buffer.length() << std::endl;
	if (buffer.length() >= 2) {
		if (buffer[buffer.length() - 2] != '\r' && buffer[buffer.length() - 1] != '\n') {
			std::cout << buffer << std::endl;
			_clients[fd].addTmpCmd(buffer);
			return ;
		}
		else if (_clients[fd].getTmpCmd() != "") {
			_clients[fd].addTmpCmd(buffer);
			buffer = _clients[fd].getTmpCmd();
			_clients[fd].setTmpCmd("");
		}
	}
	else if (_clients[fd].getTmpCmd() != "") {
		_clients[fd].addTmpCmd(buffer);
		buffer = _clients[fd].getTmpCmd();
		_clients[fd].setTmpCmd("");
	}
	std::istringstream	str(buffer);
	std::string			command;
	std::string			argument;
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
		else if(command == "part" || command == "PART")
			commandUser(argument, fd);
		else if(command == "privmsg" || command == "PRIVMSG")
			commandUser(argument, fd);
		else if(command == "notice" || command == "NOTICE")
			commandUser(argument, fd);
		else if(command == "kick" || command == "KICK")
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
	std::cout << fd << ": " << argument << std::endl;
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
		if (it->second.getNickName() == argument)
			exit(1);
	}
	_clients[fd].setNickName(argument);
}

void Server::commandPass(std::string argument, int fd) {
	_clients[fd].setPassword(argument);
}

void Server::commandUser(std::string argument, int fd) {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second.getUserName() == argument)
			return ;
	}
	_clients[fd].setUserName(argument);
}


void Server::commandPart(std::string argument, int fd) {
	std::istringstream	str(argument);
	
	std::string			channel;
	std::string			msg;

	str >> channel >> msg;
	std::map<std::string, Channel>::iterator it = _channels.find(channel);
	if (it != _channels.end())
		it->second.partClinet(fd, msg);
	else {
		; //채널이 없을 때 예외처리
	}
}

void Server::commandPrivmsg(std::string argument, int fd) {
	std::istringstream	str(argument);
	
	std::string			target;
	std::string			msg;
	Client *tmpClient;
	std::string tmpMsg;

	str >> target >> msg;
	
	if (target == "" || msg == "")
		return ;
	if (target[0] == '#'){
		std::map<std::string, Channel>::iterator tmpChannel = _channels.find(target);
		if (tmpChannel == _channels.end())
			return ;
		
		for (std::map<int, Client *>::iterator it = tmpChannel->second.getClients().begin();
			it != tmpChannel->second.getClients().end(); it++){
				std::string tmpMsg = ":" + _clients[fd].getNickName() + " PRIVMSG " + target + " :" + msg + "\r\n";
				send(it->second->getFd(), &tmpMsg, tmpMsg.length(), 0);
			}
		
	}
	else {
		tmpClient = findClient(target);
		if (tmpClient == NULL)
			return ;
			// ":IRCserv 403 " + getList()[j].getNickname() + " " + nick + " :No such channel\r\n";
		tmpMsg = ":" + _clients[fd].getNickName() + " PRIVMSG " + target + " :" + msg + "\r\n";
		send(tmpClient->getFd(), &tmpMsg, tmpMsg.length(), 0);

	}
}

void Server::commandNotice(std::string argument, int fd){
	std::istringstream	str(argument);
	
	std::string			target;
	std::string			msg;
	Client *tmpClient;
	std::string tmpMsg;

	str >> target >> msg;
	
	if (target == "" || msg == "")
		return ;
	if (target[0] == '#'){
		std::map<std::string, Channel>::iterator tmpChannel = _channels.find(target);
		if (tmpChannel == _channels.end())
			return ;
		
		for (std::map<int, Client *>::iterator it = tmpChannel->second.getClients().begin();
			it != tmpChannel->second.getClients().end(); it++){
				std::string tmpMsg = ":" + _clients[fd].getNickName() + " NOTICE " + target + " :" + msg + "\r\n";
				send(it->second->getFd(), &tmpMsg, tmpMsg.length(), 0);
			}
		
	}
	else {
		tmpClient = findClient(target);
		if (tmpClient == NULL)
			return ;
			// ":IRCserv 403 " + getList()[j].getNickname() + " " + nick + " :No such channel\r\n";
		tmpMsg = ":" + _clients[fd].getNickName() + " NOTICE " + target + " :" + msg + "\r\n";
		send(tmpClient->getFd(), &tmpMsg, tmpMsg.length(), 0);

	}
}

void Server::commandKick(std::string argument, int fd){
	std::istringstream	str(argument);
	
	std::string			channel;
	std::string			client;
	std::string			comment;

	str >> channel >> client >> comment;
	if (channel == "" || client == "") //ERR_NEEDMOREPARAMS (461): 인자가 부족
		return ;

	std::vector<std::string> vecChannel = splitComma(channel);
	std::vector<std::string> vecClient = splitComma(client);
	for (int i = 0; i < vecChannel.size(); i++) {
		std::map<std::string, Channel>::iterator chIt = _channels.find(vecChannel[i]);
		if (chIt == _channels.end()) //ERR_NOSUCHCHANNEL (403) 채널이없음
			return ;
		if (!chIt->second.isInClinet(fd)) //ERR_NOTONCHANNEL (442): 사용자가 채널에 없음
			return ;
		if (!chIt->second.isOpClient(fd)) //ERR_CHANOPRIVSNEEDED (482) 사용자가 op가 아님
			return ; 
		Client *tmpClient = findClient(vecClient[i]);
		if (tmpClient == NULL) //ERR_USERNOTINCHANNEL (441): 클라이언트가 없음.
			return ;
		if (!chIt->second.isInClinet(tmpClient->getFd())) //ERR_USERNOTINCHANNEL (441): 채널에 클라이언트가 없음.
			return ;
		std::string cmd = vecChannel[i] + " " + vecClient[i] ; // + "\r\n"; 필요헐지?
		commandPart(cmd, fd);
		// send vecClient[i] 에 msg 보내기 추가
	}
	
}

std::vector<std::string> Server::splitComma(std::string argument){
	std::istringstream	str(argument);
	std::vector<std::string> ret;

	int i = 0;
	while (std::getline(str, ret[i], ','))
		i++;
	return (ret);
}

Client *Server::findClient(std::string nickName) {

	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++){
		if (it->second.getNickName() == nickName)
			return &it->second;
	}
	return NULL;
}
