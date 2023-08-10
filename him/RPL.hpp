#ifndef RPL_HPP
#define RPL_HPP

const static std::string server_name = "irccc ";
const static std::string col_server_name = ":irccc ";
const static std::string version = "1_try ";

#define RPL_001(client) (":irccc 001 " + client + " :Welcome to the Internet Relay Network " + client + "\r\n")

#define RPL_002(client) (":irccc 002 " + client + \
" :Your host is irccc , running 1_try \r\n")

#define RPL_003(client) (":irccc 003 " + client + \
" :This server was created 2023 08 10 \r\n")

#define RPL_004(client) (":irccc 004 " + client + \
+ " irccc 1_try - itkol \r\n")


#define RPL_301(client, nick, message) (":irccc 301 " + client + \
" " + nick + ":" + message + "\r\n")

#define RPL_331(client, channel) (":irccc 331 " + client + \
" " + channel + " :No topic is set\r\n")

#define RPL_332(client, channel, topic) (":irccc 332 " + client + \
" " + channel + " :" + topic + "\r\n")

#define RPL_333(client, channel, nick, setat) (":irccc 333 " + client + \
" " + channel + " " + nick + " " + setat + "\r\n")

#define RPL_341(client, nick, channel) (":irccc 341 " + client + \
" " + nick + " " + channel + "\r\n")

#define RPL_353(client, channel, symbol) (":irccc 353 " + client + \
" " + channel + " :" + topic + "\r\n") // 구현 아직 못함, 채널에 있는 유저들 출력

#define RPL_366(client, channel) (":irccc 366 " + client + \
" " + channel + " :End of /NAMES list\r\n")

#define RPL_375(client) (":irccc 375 " + client + \
" :- irccc Message of the day - \r\n")

#define RPL_372(client) (":irccc 372 " + client + \
" :안녕하셨쎄요~\r\n")

#define RPL_376(client) (":irccc 376 " + client + \
" :End of /MOTD command.\r\n")

#define RPL_401(client, nick) (":irccc 401 " + client + \
" " + nick + " :No such nick/channel\r\n")

#define RPL_403(client, channel) (":irccc 403 " + client + \
" " + channel + " :No such channel\r\n")

#define RPL_404(client, channel) (":irccc 404 " + client + \
" " + channel + " :Cannot send to channel\r\n")

#define RPL_405(client, channel) (":irccc 405 " + client + \
" " + channel + " :You have joined too many channel\r\n")

#define RPL_411(client, command) (":irccc 411 " + client + \
" :No text to send\r\n")

#define RPL_412(client, command) (":irccc 412 " + client + \
" :No recipient given (<" + command + ">)\r\n")

#define RPL_431(client) (":irccc 431 " + client + \
" :No nickname given\r\n")

#define RPL_432(client, nick) (":irccc 432 " + client + \
" " + nick + " :Erroneus nickname\r\n")

#define RPL_433(client, nick) (":irccc 433 " + client + \
" " + nick + " :Nickname is already in use\r\n")

#define RPL_441(client, nick, channel) (":irccc 441 " + client + \
" " + nick + " " + channel + " :They aren't on that channel\r\n")

#define RPL_443(client, nick, channel) (":irccc 443 " + client + \
" " + nick + " " + channel + " :is already on channel\r\n")

#define RPL_442(client, channel) (":irccc 442 " + client + \
" " + channel + " :You're not on that channel\r\n")

#define RPL_461(client, command) (":irccc 461 " + client + \
" " + command + " :Not enough parameters\r\n")

#define RPL_462(client) (":irccc 462 " + client + \
" :You may not reregister\r\n")

#define RPL_464(client) (":irccc 464 " + client + \
" :Password incorrect\r\n")

#define RPL_467(client, channel) (":irccc 467 " + client + \
" " + channel + " :Channel key already set\r\n") //이제 안쓰이는듯

#define RPL_471(client, channel) (":irccc 471 " + client + \
" " + channel + " :Cannot join channel (+l)\r\n")

#define RPL_472(client, modechar) (":irccc 472 " + client + \
" " + modechar + " :is unknown mode char to me\r\n")

#define RPL_473(client, channel) (":irccc 473 " + client + \
" " + channel + " :Cannot join channel (+i)\r\n")

#define RPL_474(client, channel) (":irccc 474 " + client + \
" " + channel + " :Cannot join channel (+b)\r\n")

#define RPL_475(client, channel) (":irccc 475 " + client + \
" " + channel + " :Cannot join channel (+k)\r\n")

#define RPL_476(client) (":irccc 476 " + client + \
" " + channel + " :Bad Channel Mask\r\n")

#define RPL_477(client, channel) (":irccc 477 " + client + \
" " + channel + " :Channel doesn't support modes\r\n")

#define RPL_482(client, channel) (":irccc 482 " + client + \
" " + channel + " :You're not channel operator\r\n")

#define RPL_502(client) (":irccc 502 " + client + \
" :Cant change mode for other users\r\n")

#endif
