#ifndef RPL_HPP
#define RPL_HPP

#include "Client.hpp"


const static std::string server_name = "irccc ";
const static std::string col_server_name = ":irccc ";
const static std::string version = "1_try ";

#define RPL_PARTMSG(nick, user, host, channel_name, part_msg_) ":" + nick + "!" + user + "@" + host + " " + "PART" + " " + channel_name + " :" + part_msg_ + "\r\n"

#define RPL_001(client) (col_server_name + "001 " + client + \
" :Welcome to the Internet Relay Network " + client + "\r\n")

#define RPL_002(client) (col_server_name + "002 " + client + \
" :Your host is " + server_name + ", running version " + version + "\r\n")

#define RPL_003(client) (col_server_name + "003 " + client + \
" :This server was created 2023 08 10 \r\n")

#define RPL_004(client) (col_server_name + "004 " + client + \
+ " " + server_name + version + "- itkol " + "\r\n")

#define RPL_301(client, nick, message) (col_server_name + "301 " + client + \
" " + nick + ":" + message + "\r\n")

#define RPL_331(client, channel) (col_server_name + "331 " + client + \
" " + channel + " :No topic is set\r\n")

#define RPL_332(client, channel, topic) (col_server_name + "332 " + client + \
" " + channel + " :" + topic + "\r\n")

#define RPL_333(client, channel, nick, setat) (col_server_name + "333 " + client + \
" " + channel + " " + nick + " " + setat + "\r\n")

#define RPL_341(client, nick, channel) (col_server_name + "341 " + client + \
" " + nick + " " + channel + "\r\n")

// :192.168.45.17 353 zzzz = #zaza :@zzzz aa 
// :192.168.45.17 366 zzzz #zaza :End of NAMES list
#define RPL_353(client, channel, clients) (col_server_name + " 353 " + client + \
" = " + channel + " :" + clients + "\r\n") // 구현 아직 못함, 채널에 있는 유저들 출력

#define RPL_366(client, channel) (col_server_name + " 366 " + client + \
" = " + channel + " :End of NAMES list\r\n")

#define RPL_375(client) (col_server_name + "375 " + client + \
" :- " + server_name + "Message of the day - " + "\r\n")

#define RPL_372(client) (col_server_name + "372 " + client + \
" :안녕하셨쎄요~\r\n")

#define RPL_376(client) (col_server_name + "376 " + client + \
" :End of /MOTD command.\r\n")

#define RPL_401(client, nick) (col_server_name + "401 " + client + \
" " + nick + " :No such nick/channel\r\n")

#define RPL_403(client, channel) (col_server_name + "403 " + client + \
" " + channel + " :No such channel\r\n")

#define RPL_404(client, channel) (col_server_name + "404 " + client + \
" " + channel + " :Cannot send to Channel OR Client\r\n")

#define RPL_405(client, channel) (col_server_name + "405 " + client + \
" " + channel + " :You have joined too many channel\r\n")

#define RPL_411(client, command) (col_server_name + "411 " + client + \
" :No text to send\r\n")

#define RPL_412(client, command) (col_server_name + "412 " + client + \
" :No recipient given (<" + command + ">)\r\n")

#define RPL_431(client) (col_server_name + "431 " + client + \
" :No nickname given\r\n")

#define RPL_432(client, nick) (col_server_name + "432 " + client + \
" " + nick + " :Erroneus nickname\r\n")

#define RPL_433(client, nick) (col_server_name + "433 " + client + \
" " + nick + " :Nickname is already in use\r\n")

#define RPL_441(client, nick, channel) (col_server_name + "441 " + client + \
" " + nick + " " + channel + " :They aren't on that channel\r\n")

#define RPL_443(client, nick, channel) (col_server_name + "443 " + client + \
" " + nick + " " + channel + " :is already on channel\r\n")

#define RPL_442(client, channel) (col_server_name + "442 " + client + \
" " + channel + " :You're not on that channel\r\n")


//:10.19.202.226 461 zcxzx :JOIN :Not enough parameters
#define RPL_461(client, command) (col_server_name + "461 " + client + \
" " + command + " :Not enough parameters\r\n")

#define RPL_462(client) (col_server_name + "462 " + client + \
" :You may not reregister\r\n")

#define RPL_464(client) (col_server_name + "464 " + client + \
" :Password incorrect\r\n")

#define RPL_467(client, channel) (col_server_name + "467 " + client + \
" :" + channel + " :Channel key already set\r\n") //이제 안쓰이는듯
// :10.19.202.226 471 zzzzz :JOIN :#1234 :Cannot join channel (+l)
#define RPL_471(client, channel) (col_server_name + "471 " + client + \
" :" + channel + " :Cannot join channel (+l)\r\n")

#define RPL_472(client, modechar) (col_server_name + "472 " + client + \
" " + modechar + " :is unknown mode char to me\r\n")

#define RPL_473(client, channel) (col_server_name + "473 " + client + \
" " + channel + " :Cannot join channel (+i)\r\n")

#define RPL_474(client, channel) (col_server_name + "474 " + client + \
" " + channel + " :Cannot join channel (+b)\r\n")

#define RPL_475(client, channel) (col_server_name + "475 " + client + \
" " + channel + " :Cannot join channel (+k)\r\n")

#define RPL_476(client) (col_server_name + "476 " + client + \
" " + channel + " :Bad Channel Mask\r\n")

#define RPL_477(client, channel) (col_server_name + "477 " + client + \
" " + channel + " :Channel doesn't support modes\r\n")

#define RPL_482(client, channel) (col_server_name + "482 " + client + \
" " + channel + " :You're not channel operator\r\n")

#define RPL_502(client) (col_server_name + "502 " + client + \
" :Cant change mode for other users\r\n")

#endif