#include "Server.hpp"

Server::Server() {

}

Server::~Server(){

}

void Server::socket_create(){
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		perror("server socket");
		exit(1);
  }
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET; //소켓 주소 체계 넣는 변수. AF_INET - IPv4 주소 체계를 사용하는 인터넷 프로토콜
	server_addr.sin_port = htons(6667); // irc에서 포트번호는 일반적으로 6667
	server_addr.sin_addr.s_addr = INADDR_ANY; // 0 값이 들어가 있음. 
		// 특정 ip 값을 넣어주면 해당 ip와 통신을 한다는 의미. 0값을 넣으면, 모든 ip에 대한 통신요청을 허락한다는 의미
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("server bind");
		close(server_fd);
		exit(1);
	}

}

void Server::set_nonblock(){
	int flags = fcntl(server_fd, F_GETFL, 0);
	if (flags == -1) {
        perror("fcntl get");
        close(server_fd);
        exit(1);
  }
	flags |= O_NONBLOCK;
    if (fcntl(server_fd, F_SETFL, flags) == -1) {
        perror("fcntl set");
        close(server_fd);
        exit(1);
  }
}
void Server::server_listen(){
	int backlog = max_client/2;
	if (listen(server_fd, backlog) == -1) {
			perror("listen");
			close(server_fd);
			exit(EXIT_FAILURE);
	}
}

void Server::poll_init(){
	pollfds[0].fd = server_fd;
	pollfds[0].events = POLLIN;
	pollfds[0].revents = 0;
	int i = 0;
	while (++i < max_client){
			pollfds[i].fd = -1;
	}
}

void Server::main_loop(){
	while(1)
	{
		int poll_return = poll(pollfds, max_client, -1);
		if (poll_return > 0)
		{
			
		}
	}
}