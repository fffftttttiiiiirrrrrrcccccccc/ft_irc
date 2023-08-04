#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <poll.h>
#include <fcntl.h>

class Client{
public:
	Client();
	~Client();

private:
	int Client_fd;
	struct sockaddr_in Client_addr;
	int max_client;

// 	struct sockaddr_in {
// 	short    sin_family;          // 주소 체계: AF_INET
// 	u_short  sin_port;            // 16 비트 포트 번호, network byte order
// 	struct   in_addr  sin_addr;   // 32 비트 IP 주소
// 	char     sin_zero[8];         // 전체 크기를 16 비트로 맞추기 위한 dummy
// };
// 구조체의 크기를 16바이트로 맞추기 위해 나머지 6바이트를 sin_zero로 할당. 한마디로 걍 잉여 쓰레기값

};

