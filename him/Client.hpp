#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include "Channel.hpp"
#include <sys/socket.h>

class Channel;

class Client{
	private:
		std::map<std::string, Channel *> _clientChannels;
		std::string _nickName;
		std::string _userName;
		std::string _hostName;
		std::string	_serverName;
		std::string _realName;
		std::string _password;
		int				_fd;
		std::string _tmpCmd;
		bool			_isLogin;
		// Client(const Client& src);

	public:
		Client();
		// Client &operator=(const Client& rhs);
		virtual ~Client();
		void initClient(int fd);
		void setFd(int fd);
		void setNickName(std::string str);
		void setUserName(std::string str);
		void setPassword(std::string str);
		void seHosttName(std::string str);
		void setServerName(std::string str);
		void setRealName(std::string str);
		void setTmpCmd(std::string str);
		void addTmpCmd(std::string str);
		int getFd();
		std::map<std::string, Channel *> getChannels();
		std::string getNickName();
		std::string getUserName();
		std::string geHosttName();
		std::string getServerName();
		std::string getRealName();
		std::string getPassword();
		std::string getTmpCmd();

		void welcomeMsg();
		void addChannel(Channel *channel);


		//join
		//

		


};

#endif