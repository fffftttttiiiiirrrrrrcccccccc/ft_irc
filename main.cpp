#include "Server.hpp"

int main( void ){
	Server server;
	server.socket_create();
	server.set_nonblock();
	server.server_listen();
	server.poll_init();
	server.main_loop();
	}
