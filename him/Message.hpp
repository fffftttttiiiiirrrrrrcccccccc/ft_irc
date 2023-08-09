#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Server.hpp"
#include <iostream>
#include <map>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>
#include <vector>
#include <sstream>


class Server;
class Client;

class Message{
	private:
		int rpl_num;
		Client cl;
	public:
		Message(Client &client,int rpl);
		const char * message_sender();
		//구현중
		// void messageQuit(std::string argument, int fd); // 서버와 접속 끊기 /quit 메세지 (메세지는 서버에 남기는 용도)
		// void messageJoin(std::string argument, int fd); // 채널이 존재ㅇ하는지(어떤 타입 인지), 없다면 생성하고 오피 주기
		// void messageNick(std::string argument, int fd); // 다른 클라이언트 중에 닉네임 중복여부 확인
		// void messagePass(std::string argument, int fd); //
		// void messageUser(std::string argument, int fd); // 다른 클라이언트 중에 유저 중복여부 확인
		// void messagePart(std::string argument, int fd); //채널에서 나오면서  op인지도 확인. /part #채널명 메세지
		// //구현해야함
		
		// void messageExit(std::string argument, int fd); //접속끊고 클라이언트도 종료
		// void messageDisconnect(std::string argument, int fd); //quit와 동일하지만 메세지는 기본메세지 
		// void messagePrivmsg(std::string argument, int fd); // 이건 메세지 전달
		// void messageNotice(std::string argument, int fd); // 이건 공지로 채널에 메세지 전달
		// void messageKick(std::string argument, int fd); // /kick #채널명 닉네임 메시지
		// void messageInvite(std::string argument, int fd); // 초대하기  /invite 닉네임 #채널명
		// void messageTopic(std::string argument, int fd); // topic설정 채널 모드 확인  op인지 확인
		// void messageMode(std::string argument, int fd); // 채널 모드 설정 op권한 있어야 함.
		// void messageOp(std::string argument, int fd); //  op권한 주기 (여러명도 가능) 채널에 속해있는지 확인하고 주기 
		// void messageDop(std::string argument, int fd); // op권한 뺏기 (여러명도 가능)
		// void messageWho(std::string argument, int fd); //필수인가?
		// void messagePing(std::string argument, int fd); // 필수아님
		// void messagePong(std::string argument, int fd); // 필수아님

		// int insulta
		// int secret
		// int channel
};

#endif