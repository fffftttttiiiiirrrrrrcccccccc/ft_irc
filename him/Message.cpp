#include "Message.hpp"

const static std::string server_name = "irccc"; //추가

Message::Message(Client &client ,int rpl):rpl_num(rpl),cl(client){};

const char * Message::message_sender(){
	std::string print_msg = ":" + server_name + " " + std::to_string(rpl_num) + " " + cl.getNickName() + " "; //여기까지는 확정인데, rpl에 따라 살짝씩 다름
  if (rpl_num == 1)
    print_msg = print_msg + ":Welcome to the " + server_name;
  

  const char * return_val = print_msg.c_str();
  return return_val;
}