#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include "Channel.hpp"

class Channel;

class Client{
	private:
		std::vector<Channel &> _cha;
		//vector<channel 객체> 채널리스트
		// nickname;
		// fd;
		Client &operator=(const Client& rhs);
		Client(const Client& src);

	public:
		Client();
		virtual ~Client();


		//join
		//

		


};

#endif