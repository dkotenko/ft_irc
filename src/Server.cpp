#include "Server.hpp"
#include "utils.hpp"

Server::Server(int port, std::string password) {
    struct rlimit rlp;
    parser = Parser();
    this->port = port;
    this->password = password;
    serverName = SERVER_NAME;
    xassert(getrlimit(RLIMIT_NOFILE, &rlp) != -1, "getrlimit");
    maxfd = FD_SETSIZE - 1;
    is_debug = false;

    for (int i = 0; i < maxfd; i++)
    {
        users.push_back(new User(FD_FREE, i));
    }
    populateHandleMap();
    this->create();
}

Server::~Server() {
    close(this->connection);
    close(this->sockfd);
}

void Server::create()
{
    struct sockaddr_in	sin;

    sockfd =  socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printError("Failed to create socket");
    }
    const int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        printError("setsockopt(SO_REUSEADDR) failed");
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        printError("setsockopt(SO_REUSEPORT) failed");
    }

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    bind(sockfd, (struct sockaddr*)&sin, sizeof(sin));
    listen(sockfd, 42);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    users[sockfd]->type = FD_SERV;
}

void	Server::init_fd()
{
  int	i;

  i = 0;
  max = 0;
  FD_ZERO(&fd_read);
  FD_ZERO(&fd_write);
  while (i < maxfd)
    {
      if (users[i]->type != FD_FREE)
	{
	  FD_SET(i, &fd_read); 
	  if (strlen(users[i]->buf_write) > 0)
	    {
	      FD_SET(i, &fd_write);
	    }
	  max = MAX(max, i);
	}
      i++;
    }
}

void	Server::do_select()
{
  r = select(max + 1, &fd_read, &fd_write, NULL, NULL);
}

void	Server::check_fd()
{
    for (int i = 0; (i < maxfd) && (r > 0) ; i++) {
        if (FD_ISSET(i, &fd_read)) {
            fct_read(i);
        }
        if (FD_ISSET(i, &fd_write)) {
            fct_write(i);
        }
        if (FD_ISSET(i, &fd_read) ||
        FD_ISSET(i, &fd_write)) {
            r--;
        }
    }
}

void Server::run() {
    while (true) {
        init_fd();
        do_select();
        check_fd();
    }
}



void Server::fct_read(int fd) {
	if (users[fd]->type == FD_SERV) {
		srv_accept(fd);
	} else if (users[fd]->type == FD_CLIENT) {
		client_read(fd);
	} else {
		std::cout << "Error: invalid fd type in fct_read: " << users[fd]->type << std::endl;
	}
}

void Server::srv_accept(int s)
{
  int			cs;
  struct sockaddr_in	csin;
  socklen_t		csin_len;

  csin_len = sizeof(csin);
  cs = accept(s, (struct sockaddr*)&csin, &csin_len);
  printf("New client #%d from %s:%d\n", cs,
	 inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));
  users[cs]->type = FD_CLIENT;
}





void Server::client_read(int cs)
{
    int	r;
    int	i;

    std::memset(users[cs]->buf_read, 0, BUF_SIZE);
    r = recv(cs, users[cs]->buf_read, MESSAGE_MAX_LEN, 0);
    if (r <= 0) {
      close(cs);
      users[cs]->clean();
      printf("client #%d gone away\n", cs);
    }
    else {
        i = 0;
        for (int i = 0; i < maxfd; i++) {
            if (users[i]->type == FD_CLIENT && i == cs) {
                std::stringstream streamData(users[cs]->buf_read);
                std::string str;
                std::getline(streamData, str, '\n');
                str.erase(std::remove(str.begin(), str.end(), '\r' ), str.end());
                str.erase(std::remove(str.begin(), str.end(), '\n' ), str.end());
                std::cout << "line received: " << str << std::endl;
                fd = i;
				User *currUser = users[fd];
                currUser->messageOutput = parse(str);
                if (!users[i]->isConnected) {
                    std::cout << "connect status " << users[i]->connectStatus << std::endl;
                }
                //TODO уточнить формат сообщений для клиента
                for(int i = 0; i < (int)messageOutput->fd_to.size(); i++) {
                    send(messageOutput->fd_to[i], &messageOutput->data[i], messageOutput->data.size(), 0);
                }
                delete(currUser->messageOutput);
            } else if (users[i]->type == FD_CLIENT) {
                send(i, users[cs]->buf_read , r, 0);
            }
        }

    }
}

MessageOutput *Server::parse(std::string src) {
    const char separator = ' ';
    messageInput = new MessageInput();
    messageOutput = new MessageOutput();

    std::vector<std::string> outputArray;
    std::stringstream streamData(src);
    std::string val;
    int i = 0;
    messageInput->fd_from = this->fd;
    while (std::getline(streamData, val, separator)) {
        
        if (i == 0)
            messageInput->command = val;
        if (i != 0)
            messageInput->params.push_back(val);
        i++;
    }

    if (handleMap.count(messageInput->command) == 1) {

        (this->*(handleMap[messageInput->command]))();
    }
    delete(messageInput);
    return messageOutput;
}

void Server::fct_write(int cs)
{
	client_write(cs);
}

void Server::client_write(int cs)
{
    (void)cs;
}

void Server::print_debug(std::string &s) {
    if (is_debug) {
        std::cout << "debug : " << s << std::endl;
    }
}

int	Server::sendError(const User &user, int err, const std::string &arg1, const std::string &arg2)
{
	std::string	msg = ":" + serverName + " ";
	std::stringstream	ss;
	ss << err;
	msg += ss.str() + " " + user.getNickName();
	switch (err) {
		case ERR_NOSUCHNICK:
			msg += " " + arg1 + " :No such nick/channel\n";
			break;
		case ERR_NOSUCHSERVER:
			msg += " " + arg1 + " :No such server\n";
			break;
		case ERR_NOSUCHCHANNEL:
			msg += " " + arg1 + " :No such channel\n";
			break;
		case ERR_CANNOTSENDTOCHAN:
			msg += " " + arg1 + " :Cannot send to channel\n";
			break;
		case ERR_TOOMANYCHANNELS:
			msg += " " + arg1 + " :You have joined too many channels\n";
			break;
		case ERR_WASNOSUCHNICK:
			msg += " " + arg1 + " :There was no such nickname\n";
			break;
		case ERR_TOOMANYTARGETS:
			msg += " " + arg1 + " :Duplicate recipients. No arg1 delivered\n";
			break;
		case ERR_NOORIGIN:
			msg += " :No origin specified\n";
			break;
		case ERR_NORECIPIENT:
			msg += " :No recipient given (" + arg1 + ")\n";
			break;
		case ERR_NOTEXTTOSEND:
			msg += " :No text to send\n";
			break;
		case ERR_NOTOPLEVEL:
			msg += " " + arg1 + " :No toplevel domain specified\n";
			break;
		case ERR_WILDTOPLEVEL:
			msg += " " + arg1 + " :Wildcard in toplevel domain\n";
			break;
		case ERR_UNKNOWNCOMMAND:
			msg += " " + arg1 + " :Unknown command\n";
			break;
		case ERR_NOMOTD:
			msg += " :MOTD File is missing\n";
			break;
		case ERR_NOADMININFO:
			msg += " " + arg1 + " :No administrative info available\n";
			break;
		case ERR_FILEERROR:
			msg += " :File error doing \n" + arg1 + " on " + arg2 + "\n";
			break;
		case ERR_NONICKNAMEGIVEN:
			msg += " :No nickname given\n";
			break;
		case ERR_ERRONEUSNICKNAME:
			msg += " " + arg1 + " :Erroneus nickname\n";
			break;
		case ERR_NICKNAMEINUSE:
			msg += " " + arg1 + " :Nickname is already in use\n";
			break;
		case ERR_NICKCOLLISION:
			msg += " " + arg1 + " :Nickname collision KILL\n";
			break;
		case ERR_USERNOTINCHANNEL:
			msg += " " + arg1 + " " + arg2 + " :They aren't on that channel\n";
			break;
		case ERR_NOTONCHANNEL:
			msg += " " + arg1 + " :You're not on that channel\n";
			break;
		case ERR_USERONCHANNEL:
			msg += " " + arg1 + " " + arg2 + " :is already on channel\n";
			break;
		case ERR_NOLOGIN:
			msg += " " + arg1 + " :User not logged in\n";
			break;
		case ERR_SUMMONDISABLED:
			msg += " :SUMMON has been disabled\n";
			break;
		case ERR_USERSDISABLED:
			msg += " :USERS has been disabled\n";
			break;
		case ERR_NOTREGISTERED:
			msg += " :You have not registered\n";
			break;
		case ERR_NEEDMOREPARAMS:
			msg += " " + arg1 + " :Not enough parameters\n";
			break;
		case ERR_ALREADYREGISTRED:
			msg += " :You may not reregister\n";
			break;
		case ERR_NOPERMFORHOST:
			msg += " :Your host isn't among the privileged\n";
			break;
		case ERR_PASSWDMISMATCH:
			msg += " :Password incorrect\n";
			break;
		case ERR_YOUREBANNEDCREEP:
			msg += " :You are banned from this server\n";
			break;
		case ERR_KEYSET:
			msg += " " + arg1 + " :Channel key already set\n";
			break;
		case ERR_CHANNELISFULL:
			msg += " " + arg1 + " :Cannot join channel (+l)\n";
			break;
		case ERR_UNKNOWNMODE:
			msg += " " + arg1 + " :is unknown mode char to me\n";
			break;
		case ERR_INVITEONLYCHAN:
			msg += " " + arg1 + " :Cannot join channel (+i)\n";
			break;
		case ERR_BANNEDFROMCHAN:
			msg += " " + arg1 + " :Cannot join channel (+b)\n";
			break;
		case ERR_BADCHANNELKEY:
			msg += " " + arg1 + " :Cannot join channel (+k)\n";
			break;
		case ERR_NOPRIVILEGES:
			msg += " :Permission Denied- You're not an IRC operator\n";
			break;
		case ERR_CHANOPRIVSNEEDED:
			msg += " " + arg1 + " :You're not channel operator\n";
			break;
		case ERR_CANTKILLSERVER:
			msg += " :You cant kill a server!\n";
			break;
		case ERR_NOOPERHOST:
			msg += " :No O-lines for your host\n";
			break;
		case ERR_UMODEUNKNOWNFLAG:
			msg += " :Unknown MODE flag\n";
			break;
		case ERR_USERSDONTMATCH:
			msg += " :Cant change mode for other users\n";
			break;
		default:
			msg += "UNKNOWN ERROR\n";
			break;
	}
	send(user.fd, msg.c_str(), msg.size(), IRC_NOSIGNAL);
	return (-1);
}
