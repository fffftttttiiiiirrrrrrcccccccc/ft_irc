#include "Message.hpp"

const static std::string server_name = "irccc"; //추가
const static std::string network_name = "BYEONHIM NETWORK"; //추가
const static std::string server_version = "1try!"; //추가
const static std::string server_created = "2023.08.09!"; //추가
const static std::string c_modes = "itkol"; //추가
const static std::string u_modes = "-"; //추가


Message::Message(Client &client ,int rpl):rpl_num(rpl),cl(client){};

const char * Message::message_sender(){
  std::string rpl_string = std::to_string(rpl_num);
  if (std::strlen(rpl_string.c_str()) == 1)
    rpl_string = "00" + rpl_string;
  std::string cl_nick = cl.getNickName();
	std::string print_msg = ":" + server_name + " " + rpl_string + " " + cl_nick + " "; //여기까지는 확정인데, rpl에 따라 살짝씩 다름

  if (rpl_num == 1)
    print_msg = print_msg + ":Welcome to the " + network_name + " " + cl_nick;

  else if (rpl_num == 2)
    print_msg = print_msg + ":Your host is " + server_name + ", running version " + server_version;

  else if (rpl_num == 3)
    print_msg = print_msg + ":This server was created " + server_created; // 1~4 합쳐도 될듯

  else if (rpl_num == 4)
    print_msg = print_msg + server_name + " " + server_version + " " + u_modes + " " + c_modes;

  else if (rpl_num == 375)
    print_msg = print_msg + ":- " + server_name + " Message of the day - ";

  else if (rpl_num == 372)
    print_msg = print_msg + ":- " + " 안녕하셨쎄요~";

  else if (rpl_num == 376)
    print_msg = print_msg + ":End of MOTD command";  
  const char * return_val = print_msg.c_str();
  return return_val;
}

