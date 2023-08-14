#include "Server.hpp"
Server::Server(){}
Server::~Server(){}

Server::Server(int port, char *pass) {
	_port = port;
	_password = pass;
}
void Server::serverInit(int port, char* pass){
	_port = port;
	_password = pass;
}
std::map<int, Client> Server::getClients() {
	return _clients;
}


int Server::getSocket() {
	return _serverSocket;
}

void Server::sendMsg(std::string msg, int fd){
	send(fd, msg.c_str(), msg.size(), 0);
}

void Server::sendMsgVector(std::string msg, std::vector<int> fd){
	for (std::vector<int>::iterator it = fd.begin(); it != fd.end(); it++)
		send(*it, msg.c_str(), msg.size(), 0);
	std::cout << std::endl;
	std::cout << msg << std::endl;
}

void Server::printClientList(){
	std::cout << " ClientList : " ;
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++){
		Client &tmp = it->second;
		std::cout << tmp.getNickName() << " ";
	}
	std::cout << std::endl;
		
}
void Server::printChannelList(){
	std::cout << " ChannelList : " ;
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++){
		Channel &tmp = it->second;
		std::cout << tmp.getChannelName() << " ";
	}
	std::cout << std::endl;
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
			Client tmpClient;
			_clients[clientSocketfd] = tmpClient;
			_clients[clientSocketfd].initClient(clientSocketfd);
			
		}
		std::vector<pollfd>::iterator pollIt = _fds.begin();
		
		// for (; pollIt != _fds.end(); pollIt++) {
			for (unsigned long i = 1; i < _fds.size(); i++) {
				pollIt++;
				std::cout << "test : " << pollIt->fd << std::endl;
			if (pollIt->revents && POLLIN) {
				char buffer[1025];
				memset(buffer, 0, 1025);
				ret = recv(pollIt->fd, buffer, 1024, 0);
				if (ret == -1 && errno != EAGAIN && errno != EWOULDBLOCK){
					close(pollIt->fd);
					close(_serverSocket);
					_clients.erase(pollIt->fd);
					std::cout << "recv function failed" << std::endl;
					exit(1);
				}
				else if (ret == 0){
					exitClient(pollIt->fd);
					// std::map<int, Client>::iterator it = _clients.find(pollIt->fd);
					// std::cout << "Disconnet client" << _clients[pollIt->fd].getNickName() << std::endl;
					// _clients.erase(it);
					// close(pollIt->fd);
					// _fds.erase(pollIt);
					continue;
					// client 삭제하는 함수 구현. exit 찍으면 안됨.
					// exit(1);
				}
				else {
						buffer[ret] = '\0';
						get_command(buffer, pollIt->fd);
						// write(_fds[i].fd, buffer, ret);
						printClientList();
						printChannelList();
						// std::cout << "Get socket data : " << buffer << std::endl;
				}
			}
		}
	}
}

void	Server::get_command(std::string buffer, int fd) {

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
		std::istringstream	cmdStream(command);
		while (std::getline(cmdStream, command)){
			if (command == "LS")
			continue;
		}
		_command = command;
		if (command == "CAP")
			continue;
		std::getline(str, argument);
		std::cout << "Client : " << _clients[fd].getNickName() << std::endl;
		std::cout << "Command : " << command << std::endl;
		std::cout << "Argument : " << argument << std::endl;
		if (_password != _clients[fd].getPassword()){
			if (command == "pass" || command == "PASS")
				commandPass(argument, fd);
			else
				sendMsg(RPL_464(_clients[fd].getNickName()), fd);
		}else if (_clients[fd].getNickName() == "*"){
			if (command == "pass" || command == "PASS")
				commandPass(argument, fd);
			else if (command == "nick" || command == "NICK")
				commandNick(argument, fd);
		} else if(_clients[fd].getUserName() == "*") {
			if (command == "pass" || command == "PASS")
				commandPass(argument, fd);
			else if (command == "user" || command == "USER")
				commandUser(argument, fd);
			else if (command == "nick" || command == "NICK")
				commandNick(argument, fd);
		} else {
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
				commandPart(argument, fd);
			else if(command == "privmsg" || command == "PRIVMSG")
				commandPrivmsg(argument, fd);
			// else if(command == "notice" || command == "NOTICE")
			// 	commandNotice(argument, fd);
			else if(command == "kick" || command == "KICK")
				commandKick(argument, fd);
			else if(command == "invite" || command == "INVITE")
				commandInvite(argument, fd);
			else if(command == "topic" || command == "TOPIC")
				commandTopic(argument, fd);
			else if(command == "mode" || command == "MODE")
				commandMode(argument, fd);
			else if(command == "ping" || command == "PING")
				commandPing(argument, fd);
			else if(command == "pong" || command == "PONG")
				commandPong(argument, fd);
			_command = "";
		}
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
	
	std::string			channel;
	std::string			password;

	str >> channel >> password;//ERR_NEEDMOREPARAMS (461): 인자가 부족
	if (channel.length() <= 1){
		sendMsg(RPL_461(_clients[fd].getNickName(), "JOIN"), fd);
        return ;
	}
	std::vector<std::string> vecChannel = splitComma(channel);
	for (unsigned long i = 0 ; i < vecChannel.size(); i++) {
		if (vecChannel[i][0] != '#'){
			sendMsg(RPL_461(_clients[fd].getNickName(), "JOIN"), fd);
			continue;
		}
		std::map<std::string, Channel>::iterator chIt = _channels.find(vecChannel[i]);
		if (chIt == _channels.end()) { //채널이 없으므로 만들고 넣어줌
			Channel newChannel(vecChannel[i]);
			_channels[vecChannel[i]] = newChannel;
			_channels[vecChannel[i]].addClinetInChannel(fd, &_clients[fd], "");
			_clients[fd].addChannel(&_channels[vecChannel[i]]);
			std::string joinMsg = ":" +_clients[fd].getNickName() + " JOIN " + vecChannel[i] + "\r\n";
			sendMsg(joinMsg, fd);
			//353 366추가
			sendMsg(RPL_353(_clients[fd].getNickName(), chIt->second.getChannelName(), _clients[fd].getNickName() ), fd);
			sendMsg(RPL_366(_clients[fd].getNickName(), chIt->second.getChannelName()), fd);
			//353 366추가
			//sendMsg(RPL_332(_clients[fd].getNickName(), vecChannel[i], newChannel.getTopic()), fd);
			//새로 만든거라 토픽 확정적으로 없음
		}
		else{
			std::vector<std::string> vecPassword = splitComma(password);
			if (chIt->second.isInClinet(fd))//ERR_NOTONCHANNEL (442): 사용자가 이미 채널에 있음
                continue;
			if (chIt->second.getIsLimitMode() && !chIt->second.isJoinalbe()){ //ERR_CHANNELISFULL (471): 인원이 꽉참
				sendMsg(RPL_471(_clients[fd].getNickName(), chIt->second.getChannelName()), fd);
                continue;
			}
			if (chIt->second.getIsInviteMode()) {//ERR_INVITEONLYCHAN (473):초대만 가능모드
				if (chIt->second.inviteJoinClient(fd, &_clients[fd])){
					_clients[fd].addChannel(&chIt->second);
					std::string joinMsg = ":" +_clients[fd].getNickName() + " JOIN " + chIt->second.getChannelName() + "\r\n";
					sendMsgVector(joinMsg, chIt->second.getClientsFd());
					//353 366추가
					std::vector<int> tmpChannelList = chIt->second.getClientsFd();
					std::string clients_list = "";
					for (std::vector<int>::iterator it = tmpChannelList.begin(); it != tmpChannelList.end(); it++)
							clients_list = clients_list + _clients[*it].getNickName() + " ";
					sendMsg(RPL_353(_clients[fd].getNickName(), channel, clients_list), fd);
					sendMsg(RPL_366(_clients[fd].getNickName(), channel), fd);
					//353 366추가

					sendMsg(RPL_332(_clients[fd].getNickName(), chIt->second.getChannelName(), chIt->second.getTopic() == "" ? " * NONE * " : chIt->second.getTopic()), fd);
                    continue ;
				}
				sendMsg(RPL_473(_clients[fd].getNickName(), chIt->second.getChannelName()), fd);
                continue;
            }

			if (chIt->second.getIsKeyMode() ){
					if (vecPassword.size() < i){
                        sendMsg(RPL_475(_clients[fd].getNickName(), chIt->second.getChannelName()), fd);
                        continue;
                    }
					else if (chIt->second.getPassword() != vecPassword[i]){
							sendMsg(RPL_475(_clients[fd].getNickName(), chIt->second.getChannelName()), fd);
                        continue;
					}
			}
			chIt->second.addClinetInChannel(fd, &_clients[fd], "");
			_clients[fd].addChannel(&chIt->second);
			// :aa!aa@localhost JOIN :#zxcx
			std::string joinMsg = ":" +_clients[fd].getNickName() + " JOIN " + chIt->second.getChannelName() + "\r\n";
			sendMsgVector(joinMsg, chIt->second.getClientsFd());
			// sendMsg(joinMsg, fd);
			//353 366추가
			std::vector<int> tmpChannelList = chIt->second.getClientsFd();
			std::string clients_list = "";
			for (std::vector<int>::iterator it = tmpChannelList.begin(); it != tmpChannelList.end(); it++)
					clients_list = clients_list + _clients[*it].getNickName() + " ";
			

			sendMsg(RPL_353(_clients[fd].getNickName(), chIt->second.getChannelName(), clients_list), fd);
			sendMsg(RPL_366(_clients[fd].getNickName(), chIt->second.getChannelName()), fd);
			//353 366추가
			sendMsg(RPL_332(_clients[fd].getNickName(), chIt->second.getChannelName(), chIt->second.getTopic() == "" ? " * NONE * " : chIt->second.getTopic()), fd);
			//RPL_TOPIC (332): 채널의 토픽을 클라이언트에게 전달합니다. 채널의 토픽 정보를 알려주는 역할을 합니다.
		}
	}
}

void Server::commandNick(std::string argument, int fd) {

	std::istringstream	str(argument);
	std::string nickName;
	std::string beforeNickName;
	beforeNickName = _clients[fd].getNickName();
	std::getline(str, nickName, ' ');
	if (nickName == ""){
		sendMsg(RPL_431(_clients[fd].getNickName()),fd);
		return ;
	}
	//string back 함수 수정 11버전 함수
	if (!nickName.empty() && nickName[nickName.length() - 1] == '\r') {
		nickName.erase(nickName.size() - 1);
	}
	if (nickName.length() > 9) {
		sendMsg(RPL_432(_clients[fd].getNickName(), nickName),fd);
		return ;
	}
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second.getNickName() == nickName){
			sendMsg(RPL_433(_clients[fd].getNickName(), nickName), fd);
			return ;
		}
	}
	_clients[fd].setNickName(nickName);
	// :aa!aa@localhost NICK zzz
	// _isLogin true면 출력
	if (_clients[fd].get_isLogin()){
		std::string nickMsg = ":" + beforeNickName + " NICK " + _clients[fd].getNickName() + "\r\n";
		sendMsg(nickMsg, fd);
	}
}

void Server::commandPass(std::string argument, int fd) {
	std::istringstream	str(argument);
	std::string password;
	if (_clients[fd].getPassword() == _password){
		sendMsg(RPL_462(_clients[fd].getNickName()), fd);
		return ;
	}
	str >> password;
	if (password == ""){
		sendMsg(RPL_461("*", "PASS"),fd);
		return ;
	}
	_clients[fd].setPassword(password);
	std::cout << "server password : " << _password << std::endl;
	std::cout << "client password : " << _clients[fd].getPassword() <<std::endl;
}

void Server::commandUser(std::string argument, int fd) {
	std::istringstream	str(argument);
	std::string userName;
	std::string hostName;
	std::string serverName;
	std::string realName;
	str >> userName >> hostName >> serverName >> realName;
	if (userName == "" || hostName == "" || serverName == "" || realName == ""){
		sendMsg(RPL_461(_clients[fd].getNickName(), "USER"),fd);
		return ;
	}
	if (_clients[fd].getUserName() != "*"){
		sendMsg(RPL_462(_clients[fd].getNickName()),fd);
		return ;
	}
	_clients[fd].setUserName(userName);
	_clients[fd].setHostName(hostName);
	_clients[fd].setServerName(serverName);
	_clients[fd].setRealName(realName);
	sendMsg(RPL_001(_clients[fd].getNickName()),fd);
	sendMsg(RPL_002(_clients[fd].getNickName()),fd);
	sendMsg(RPL_003(_clients[fd].getNickName()),fd);
	sendMsg(RPL_004(_clients[fd].getNickName()),fd);
	sendMsg(RPL_375(_clients[fd].getNickName()),fd);
	sendMsg(RPL_372(_clients[fd].getNickName()),fd);
	sendMsg(RPL_376(_clients[fd].getNickName()),fd);
}


void Server::commandPart(std::string argument, int fd) {
	std::istringstream	str(argument);
	
	std::string			channel;
	std::string			msg;

	str >> channel >> msg;//ERR_NEEDMOREPARAMS (461): 인자가 부족
	if (channel == ""){
		sendMsg(RPL_461(_clients[fd].getNickName(), "PART"), fd);
        return ;
	}
	std::vector<std::string> vecChannel = splitComma(channel);
	for (unsigned long i = 0 ; i < vecChannel.size(); i++) {
		std::map<std::string, Channel>::iterator chIt = _channels.find(vecChannel[i]);
		if (chIt == _channels.end()){
			sendMsg(RPL_403(_clients[fd].getNickName(), vecChannel[i]),fd);
			return ;
		}
		if (!_clients[fd].findChannel(vecChannel[i])){
			sendMsg(RPL_442(_clients[fd].getNickName(),vecChannel[i]),fd);
			return ;
		}
		// wada has left
		// :wada!wada@192.168.45.17 PART #1234 :asd
		std::cout << _clients[fd].getNickName() << std::endl;
		std::cout << _clients[fd].getUserName() << std::endl;
		std::cout << _clients[fd].getHostName() << std::endl;
		
		sendMsgVector(RPL_PARTMSG(_clients[fd].getNickName(), _clients[fd].getUserName(), _clients[fd].getHostName(), vecChannel[i], msg), chIt->second.getClientsFd());
		chIt->second.partClinet(fd);
		_clients[fd].removeChannel(vecChannel[i]);
		if (chIt->second.getClients().size() == 0)
			_channels.erase(chIt);
	}
}
// :wada!wada@192.168.45.17 PART #1234 :asd
void Server::commandPrivmsg(std::string argument, int fd) {
	std::istringstream	str(argument);
	
	std::string			target;
	std::string			msg;
	Client *tmpClient;
	std::string tmpMsg;

	str >> target;
	std::getline(str, msg);
	if (target == ""){
		sendMsg(RPL_411(_clients[fd].getNickName(), "PRIVMSG"),fd);
		return ;
	}
	if (msg.length() == 0){
		sendMsg(RPL_412(_clients[fd].getNickName(), "PRIVMSG"),fd);
		return ;
	}
	if (target[0] == '#'){
		std::map<std::string, Channel>::iterator tmpChannel = _channels.find(target);
		if (tmpChannel == _channels.end()){
			sendMsg(RPL_404(_clients[fd].getNickName(), target),fd);//404
			return ;
		}
		std::map<int, Client *> tmpChannelList = tmpChannel->second.getClients();
		for (std::map<int, Client *>::iterator it = tmpChannelList.begin(); it != tmpChannelList.end(); it++){
				std::string tmpMsg = ":" + _clients[fd].getNickName() + " PRIVMSG " + target + " :" + msg + "\r\n";
        if (it->second->getFd() != fd)
					sendMsg(tmpMsg, it->second->getFd());
			}
	}
	else {
		if (target == _clients[fd].getNickName()){
			sendMsg(RPL_404(_clients[fd].getNickName(), target), fd);
			return ;
		}
		tmpClient = findClient(target);
		if (tmpClient == NULL){
			sendMsg(RPL_401(_clients[fd].getNickName(), target), fd);
			return ;
		}
		// ":IRCserv 403 " + getList()[j].getNickname() + " " + nick + " :No such channel\r\n";
		tmpMsg = ":" + _clients[fd].getNickName() + " PRIVMSG " + target + " :" + msg + "\r\n";
		send(tmpClient->getFd(), &tmpMsg, tmpMsg.length(), 0);
	}
}

// void Server::commandNotice(std::string argument, int fd){
// 	//pricvmsg와 에러처리 거의 똑같이 하면 될 것 같음
// 	std::istringstream	str(argument);
	
// 	std::string			target;
// 	std::string			msg;
// 	Client *tmpClient;
// 	std::string tmpMsg;

// 	str >> target;
// 	std::getline(str, msg);
	
// 	if (target == ""){
// 		sendMsg(RPL_411(_clients[fd].getNickName(), "NOTICE"),fd);
// 		return ;
// 	}
// 	if (msg.length() == 0){
// 		sendMsg(RPL_412(_clients[fd].getNickName(), "NOTICE"),fd);
// 		return ;
// 	}
// 	if (target[0] == '#'){
// 		std::map<std::string, Channel>::iterator tmpChannel = _channels.find(target);
// 		if (tmpChannel == _channels.end()){
// 			sendMsg(RPL_404(_clients[fd].getNickName(), target),fd);//404
// 			return ;
// 		}
// 		std::map<int, Client *> tmpChannelList = tmpChannel->second.getClients();
// 		for (std::map<int, Client *>::iterator it = tmpChannelList.begin(); it != tmpChannelList.end(); it++){
// 				std::string tmpMsg = ":" + _clients[fd].getNickName() + " PRIVMSG " + target + " :" + msg + "\r\n";
//         if (it->second->getFd() != fd)
// 					sendMsg(tmpMsg, it->second->getFd());
// 			}
// 	}
// 	else {
// 		if (target == _clients[fd].getNickName()){
// 			sendMsg(RPL_404(_clients[fd].getNickName(), target), fd);
// 			return ;
// 		}
// 		tmpClient = findClient(target);
// 		if (tmpClient == NULL){
// 			sendMsg(RPL_401(_clients[fd].getNickName(), target), fd);
// 			return ;
// 		}
// 		// ":IRCserv 403 " + getList()[j].getNickname() + " " + nick + " :No such channel\r\n";
// 		tmpMsg = ":" + _clients[fd].getNickName() + " NOTICE " + target + " :" + msg + "\r\n";
// 		send(tmpClient->getFd(), &tmpMsg, tmpMsg.length(), 0);
// 	}
// }

void Server::commandKick(std::string argument, int fd){
	std::istringstream	str(argument);
	
	std::string			channel;
	std::string			client;
	std::string			comment;

	str >> channel >> client >> comment;
	if (channel == "" || client == ""){
		sendMsg(RPL_461(_clients[fd].getNickName(),"KICK") , fd);
		return ;
	} //ERR_NEEDMOREPARAMS (461): 인자가 부족

	std::vector<std::string> vecChannel = splitComma(channel);
	std::vector<std::string> vecClient = splitComma(client);
	for (unsigned long i = 0; i < vecChannel.size(); i++) {
		std::map<std::string, Channel>::iterator chIt = _channels.find(vecChannel[i]);
		if (chIt == _channels.end()){
			sendMsg(RPL_403(_clients[fd].getNickName(),channel) , fd);
			return ;
		} //ERR_NOSUCHCHANNEL (403) 채널이없음

		if (!chIt->second.isInClinet(fd)){
			sendMsg(RPL_442(_clients[fd].getNickName(),channel) , fd);
			return ;
		} //ERR_NOTONCHANNEL (442): 사용자가 채널에 없음

		if (!chIt->second.isOpClient(fd)){
			sendMsg(RPL_482(_clients[fd].getNickName(),channel) , fd);
			return ;
		} //ERR_CHANOPRIVSNEEDED (482) 사용자가 op가 아님
 
		Client *tmpClient = findClient(vecClient[i]);
		if (tmpClient == NULL){
			sendMsg(RPL_441(_clients[fd].getNickName(), client, channel) , fd);
			return ;
		} //ERR_USERNOTINCHANNEL (441): 클라이언트가 없음.

		if (!chIt->second.isInClinet(tmpClient->getFd())){
			sendMsg(RPL_441(_clients[fd].getNickName(), client, channel) , fd);
			return ;
		} //ERR_USERNOTINCHANNEL (441): 채널에 클라이언트가 없음.

		std::string cmd = vecChannel[i] + " " + vecClient[i] ; // + "\r\n"; 필요헐지?
		commandPart(cmd, fd);
		// send vecClient[i] 에 msg 보내기 추가
	}
}

void Server::commandInvite(std::string argument, int fd) {
	std::istringstream	str(argument);
	
	std::string			nickName;
	std::string			channel;

	str >> nickName >> channel;

	//std::cout << nickName << "1    1" << channel << std::endl; //--주석처리--
	
	if (nickName == "" || channel == "") {// ERR_NEEDMOREPARAMS (461): 인자부족
		sendMsg(RPL_461(_clients[fd].getNickName(),"INVITE"),fd);
		return ;
}
	Client *tmpClient = findClient(nickName);
	// std::map<int ,Client>::iterator it = _clients.find(nic)
//	std::cout << tmpClient->getNickName() << "1" << std::endl;
	if (tmpClient == NULL) {//ERR_NOSUCHNICK (401): 초대할 클라이언트 없음
		sendMsg(RPL_401(_clients[fd].getNickName(),nickName),fd);
		return ;
	}
	
	std::map<std::string, Channel>::iterator chIt = _channels.find(channel);
	std::cout << "channe : " << chIt->first <<std::endl;
	if (chIt == _channels.end()) {//ERR_NOSUCHCHANNEL (403) 채널이없음
		sendMsg(RPL_403(_clients[fd].getNickName(),channel),fd);
		return ;
	}
	if (!chIt->second.isInClinet(fd)) {//ERR_NOTONCHANNEL (442): 사용자가 채널에 없음
		sendMsg(RPL_442(_clients[fd].getNickName(),"INVITE"),fd);
		return ;
	}
	if (chIt->second.isInClinet(tmpClient->getFd())) {// ERR_USERONCHANNEL (443): 초대할 클라이언트가 이미 채널에 있음
		sendMsg(RPL_443(_clients[fd].getNickName(),nickName, channel),fd);
		return ;
	}
	if (!chIt->second.isOpClient(fd)) {//ERR_CHANOPRIVSNEEDED (482) 사용자가 op가 아님
			sendMsg(RPL_482(_clients[fd].getNickName(),channel),fd);
			return ;
		}
	//초대메세지 보내기
	// std::string tmpMsg = ":" + _clients[fd].getNickName() + " INVITE " + channel + "\r\n";
	sendMsg(RPL_341(_clients[fd].getNickName(), nickName, channel), tmpClient->getFd());
	sendMsg(RPL_341(_clients[fd].getNickName(), nickName, channel), fd);
	chIt->second.addInvieList(tmpClient->getFd());
}

void Server::commandTopic(std::string argument, int fd){
	std::istringstream	str(argument);

	std::string			channel;
	std::string			topic;

	str >>channel >> topic;
	if (channel == ""){
		sendMsg(RPL_461(_clients[fd].getNickName(),"TOPIC") , fd);
		return ;
	} //ERR_NEEDMOREPARAMS (461): 매개변수 부족
	std::map<std::string, Channel>::iterator chIt = _channels.find(channel);
	if (chIt == _channels.end()){
		sendMsg(RPL_403(_clients[fd].getNickName(),channel) , fd);
		return ;
	} //ERR_NOSUCHCHANNEL (403) 채널이없음
	if (!chIt->second.isInClinet(fd)) //ERR_NOTONCHANNEL (442): 사용자가 채널에 없음
		return ;
	if (topic == "") {
		if (chIt->second.getTopic() == ""){
		sendMsg(RPL_331(_clients[fd].getNickName(), channel) , fd);
		return ;
	}
			// RPL_NOTOPIC (331): 채널에 토픽이 설정되어 있지 않을 때, 해당 채널의 토픽이 없음을 클라이언트에게 알려줍니다.
		else {
			sendMsg(RPL_332(_clients[fd].getNickName(), channel, chIt->second.getTopic()) , fd);
			// RPL_TOPIC (332): 채널의 토픽을 클라이언트에게 전달합니다. 채널의 토픽 정보를 알려주는 역할을 합니다.
		}
	}
	else {
		if (!chIt->second.isOpClient(fd)){ //초대모드면 관리자 권한 필요{
			sendMsg(RPL_482(_clients[fd].getNickName(),channel) , fd);
			return ;
	}  //ERR_CHANOPRIVSNEEDED (482) 사용자가 op가 아님

		if (!chIt->second.getIsTopicMode()){ //채널 모드 없음음
			sendMsg(RPL_477(_clients[fd].getNickName(),channel) , fd);
			return ;
	}
		chIt->second.setTopic(topic);
	}
}

void Server::commandMode(std::string argument, int fd) {
	std::istringstream	str(argument);

	std::string			channel;
	std::string			mode;
	std::string			arg;

	// 	 /mode #채널명 +i (초대한 사람만 들어올수 있게함)
	//   /mode #채널명 +t (op만 TOPIC을 변경할 수 있게 함)
	//   /mode #채널명 +k 비밀번호 (채널에 비밀번호를 설정)
	//   /mode #채널명 +o 닉네임 (op권한을 줌)
	//   /mode #채널명 +l number (number 만큼 입장가능 인원을 제한함)

	str >>channel >> mode >> arg;

	if (channel == "" || mode == ""){
		sendMsg(RPL_461(_clients[fd].getNickName(),"MODE") , fd);
		return ;
	} //ERR_NEEDMOREPARAMS (461): 매개변수 부족

	if (mode.length() > 2 || mode.length() < 2){
		sendMsg(RPL_461(_clients[fd].getNickName(),"MODE") , fd);
		return ;
	}

	//수정 != -> ==
	if (mode[0] == '+' && (mode[1] == 'k' || mode[1] == 'o' || mode[1] == 'l') && arg == "") {
		sendMsg(RPL_461(_clients[fd].getNickName(),"MODE") , fd);
		return ;
	} //ERR_NEEDMOREPARAMS (461): 매개변수 부족


	//수정 != -> ==
	if (mode[0] == '-' && mode[1] == 'o' && arg == "") {
		sendMsg(RPL_461(_clients[fd].getNickName(),"MODE") , fd);
		return ;
	} //ERR_NEEDMOREPARAMS (461): 매개변수 부족

	//추가 유저모드는 지원 안하니까 그냥 472로 에러처리
	if (channel[0] != '#'){
		sendMsg(RPL_472(_clients[fd].getNickName(),mode), fd);
		return ;
	}

	std::map<std::string, Channel>::iterator chIt = _channels.find(channel);
	if (chIt == _channels.end()) {
		sendMsg(RPL_403(_clients[fd].getNickName(),channel) , fd);
		return ;
	} //ERR_NOSUCHCHANNEL (403) 채널이없음

	if (!chIt->second.isInClinet(fd)){
			sendMsg(RPL_442(_clients[fd].getNickName(),channel) , fd);
			return ;
		} //ERR_NOTONCHANNEL (442): 사용자가 채널에 없음

	if (!chIt->second.isOpClient(fd)){
			sendMsg(RPL_482(_clients[fd].getNickName(),channel) , fd);
			return ;
		} //ERR_CHANOPRIVSNEEDED (482) 사용자가 op가 아님

	if (mode[0] == '+') {
		if (mode[1] == 'i'){
			chIt->second.setIsInviteOnly(true);
			// :zzzzz!a@10.19.202.226 MODE #ccc +l 2
			sendMsg(":irccc MODE " + channel + " " + mode + "\r\n", fd);
		}
		else if (mode[1] == 't'){
			chIt->second.setIsTopic(true);
			sendMsg(":irccc MODE " + channel + " " + mode + "\r\n", fd);
	}
		else if (mode[1] == 'k'){
			chIt->second.setIsKey(true);
			chIt->second.setPassword(arg);
			sendMsg(":irccc MODE " + channel + " " + mode + " " + arg + "\r\n", fd);
		}		//---------------------------------------------------
		else if (mode[1] == 'o'){
			Client *tmpClient = findClient(arg);
			if (tmpClient == NULL){
				sendMsg(RPL_441(_clients[fd].getNickName(), arg, channel) , fd);
				return ;
		} //ERR_USERNOTINCHANNEL (441): 클라이언트가 없음.
			if (!chIt->second.isInClinet(tmpClient->getFd())) {
				sendMsg(RPL_441(_clients[fd].getNickName(), arg, channel) , fd);
				return ;
		} //ERR_USERNOTINCHANNEL (441): 클라이언트가 없음.
			chIt->second.addOpClinet(tmpClient->getFd());
			sendMsg(":irccc MODE " + channel + " " + mode + " " + _clients[tmpClient->getFd()].getNickName() + "\r\n", fd);
		}
		else if (mode[1] == 'l') {
			chIt->second.setIsLimit(true);
			int num = atoi(arg.c_str());
			chIt->second.setLimitClientNum(num);
			sendMsg(":irccc MODE " + channel + " " + mode + " " + arg + "\r\n", fd);
		}else {
			sendMsg(RPL_461(_clients[fd].getNickName(),"MODE") , fd);
			return ;
		}
	}
	else if (mode[0] == '-'){
		if (mode[1] == 'i'){
			chIt->second.setIsInviteOnly(false);
			sendMsg(":irccc MODE " + channel + " " + mode + "\r\n", fd);
		}
		else if (mode[1] == 't'){
			chIt->second.setIsTopic(false);
			sendMsg(":irccc MODE " + channel + " " + mode + "\r\n", fd);
		}
		else if (mode[1] == 'k'){
			chIt->second.setIsKey(false);
			chIt->second.setPassword("");
			sendMsg(":irccc MODE " + channel + " " + mode + "\r\n", fd);
		}
		else if (mode[1] == 'o'){
			Client *tmpClient = findClient(arg);
			if (tmpClient == NULL){
				sendMsg(RPL_441(_clients[fd].getNickName(), arg, channel) , fd);
				return ;
		} //ERR_USERNOTINCHANNEL (441): 클라이언트가 없음.

			if (!chIt->second.isInClinet(tmpClient->getFd())) {
				sendMsg(RPL_441(_clients[fd].getNickName(), arg, channel) , fd);
				return ;
		} //ERR_USERNOTINCHANNEL (441): 클라이언트가 없음.
			chIt->second.removeOpClient(tmpClient->getFd());
			sendMsg(":irccc MODE " + channel + " " + mode + " " + _clients[tmpClient->getFd()].getNickName() + "\r\n", fd);
		}
		else if (mode[1] == 'l'){
			chIt->second.setIsLimit(false);
			sendMsg(":irccc MODE " + channel + " " + mode + "\r\n", fd);
		}
		else {
			sendMsg(RPL_461(_clients[fd].getNickName(),"MODE") , fd);
			return ;
		}
			
	} else {
		sendMsg(RPL_461(_clients[fd].getNickName(),"MODE") , fd);
		return ;
	}
	
}

void Server::commandPing(std::string argument, int fd){
	//Pong메세지 보내기
	std::istringstream	str(argument);

	std::string			pingmsg;

	str >>pingmsg;
	if (pingmsg == ""){
		sendMsg(RPL_461(_clients[fd].getNickName(), "PING"), fd);
		return ;
	}
	sendMsg(":" + server_name + " PONG " + server_name + " " + pingmsg, fd);
}

void Server::commandPong(std::string argument, int fd){
	//Pong메세지 보내기
	std::istringstream	str(argument);

	std::string			pongmsg;

	str >>pongmsg;
	if (pongmsg == ""){
		sendMsg(RPL_461(_clients[fd].getNickName(), "PONG"), fd);
		return ;
	}
	sendMsg(":" + server_name + " PONG " + server_name + " " + pongmsg, fd);
}


std::vector<std::string> Server::splitComma(std::string argument){
	std::istringstream	str(argument);
	std::string tmp;
	std::vector<std::string> ret;

	while (std::getline(str, tmp, ','))
		ret.push_back(tmp);
		
	return (ret);
}

Client *Server::findClient(std::string nickName) {

	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++){
		if (it->second.getNickName() == nickName)
			return &it->second;
	}
	return NULL;
}
