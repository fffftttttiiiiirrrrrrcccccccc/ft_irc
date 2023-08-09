#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>
#include "Client.hpp"

class Client;

class Channel {
	private:

		std::map<int , Client *> _clients; //<fd, cli 참조>
		std::string _channelName;

		bool	_isInviteOnly; // 초대한 사람만 들어갈 수 있음. /mode #채널명 +i

		bool		_isTopic; // op만 topic설정 가능 /mode #채널명 +t
		std::string _topic;

		bool	_isKey;
		std::string	_password; // 채널에 비밀번호 설정 /mode #채널명 +k password

		bool	_isOperator; // op권한을 줄 수있음 /mode #채널명 +o nickName
		std::vector<int> _opList; //op권한 클라이언트 리스트

		bool	_isLimit; // 제한된 인원수만 입장 가능 /mode #채널명 +l number
		unsigned long		_LimitClientNum; // 제한된 인원수만 입장 가능 /mode #채널명 +l number

	public:
		Channel();
		Channel(std::string channelName);
		std::map<int, Client *> getClients();
		std::string getChannelName();
		std::string getTopic();
		std::string getPassword();
		void	addClinetInChannel(int fd, Client* client, std::string password);
		void 	removeClinetInChannel(int fd);

		void	setIsInviteOnly(bool isInviteOnly);
		void	setIsTopic(bool isTopic);
		void	setIsKey(bool isKey);
		void	setIsOperator(bool isOperator);
		void	setIsLimit(bool isLimit);

		void 	addOpClinet(int fd);
		void	removeOpClient(int fd);
		void	setPassword(std::string password);
		void	setTopic(std::string topic);
		void	setLimitClientNum(int num);

		bool	getIsLimitMode();
		bool	getIsOperatorMode();
		bool	getIsInviteMode();
		bool	getIsTopicMode();
		bool	getIsKeyMode();

		bool	isJoinalbe();

		bool	isOpClient(int fd);
		bool	isInClinet(int fd);

		void	inviteClient(int fd, Client *client);

		void	partClinet(int fd, std::string msg); // 채널 메세지 보내기 추가해야함.

		//invite함수 추가해야함.


};

#endif