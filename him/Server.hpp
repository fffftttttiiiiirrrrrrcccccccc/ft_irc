#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include <cstdlib>
#include <iostream>
#include <map>
#include <signal.h>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>
#include <vector>
#include <sstream>
#include "RPL.hpp"


class Client;
class Channel;

class Server{
	private:
		std::map<int, Client> _clients; // fd, client
		std::map<std::string, Channel> _channels; // channelName channel
		std::string _password;
		std::vector<struct pollfd> _fds;
		int _port;
		int _serverSocket;
		std::string _command;

	public:
		Server();
		virtual ~Server();
		Server(int port, char* pass);
		Server(const Server &src);
		std::map<int, Client> getClients();
		void serverInit(int port, char* pass);
		Server &operator=(const Server &rhs);
		void signal_handler(int signo);
		void sendMsg(std::string msg, int fd);
		void sendMsgVector(std::string msg, std::vector<int> fd);

		void	sockCreat();
		int		getSocket();
		void	addServerSocket();
		std::vector<struct pollfd> getPollfd();
		void	runServer();
		void	get_command(std::string buffer, int fd);

		void exitClient(int fd);
		int findPollfdIndex(int fd);
		void removeClientInServer(int fd);

		Client *findClient(std::string nickName);
		std::vector<std::string> splitComma(std::string str);


		//구현완료
		void commandMode(std::string argument, int fd); //
		void commandInvite(std::string argument, int fd); // 초대하기  /invite 닉네임 #채널명
		void commandTopic(std::string argument, int fd); //
		void commandJoin(std::string argument, int fd); //
		void commandPrivmsg(std::string argument, int fd); // 이건 메세지 전달
		void commandPart(std::string argument, int fd); //채널에서 나오면서  op인지도 확인. /part #채널명 메세지
		void commandUser(std::string argument, int fd); // 다른 클라이언트 중에 유저 중복여부 확인
		void commandPass(std::string argument, int fd); //
		void commandNick(std::string argument, int fd); //
		//구현중
		void commandQuit(std::string argument, int fd); // 서버와 접속 끊기 /quit 메세지 (메세지는 서버에 남기는 용도)
		

		// void commandNotice(std::string argument, int fd); // 이건 공지로 메세지 전달 이건 에러처리가 없음.
		void commandKick(std::string argument, int fd); // /kick #채널명 닉네임 메시지


		void commandPing(std::string argument, int fd);
		void commandPong(std::string argument, int fd);

		
		

		void printClientList();
		void printChannelList();
		// int insulta
		// int secret
		// int channel
};

#endif