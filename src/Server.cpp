#include "Server.hpp"
#include "utils.hpp"

Server::Server(int port, std::string *password) {
    struct rlimit rlp;
    parser = Parser();
    port = port;
    password = password;
    serverName = SERVERNAME;
    xassert(getrlimit(RLIMIT_NOFILE, &rlp) != -1, "getrlimit");
    maxfd = FD_SETSIZE - 1;
    is_debug = false;

    for (int i = 0; i < maxfd; i++)
    {
        users.push_back(new User(FD_FREE, i));
    }
    populatehandleMap();
    create();
}



void Server::populatehandleMap() {
    handleMap[CMD_NICK] = &Server::handleNick;
    handleMap[CMD_USER] = &Server::handleUser;
    handleMap[CMD_PASS] = &Server::handlePass;
    handleMap[CMD_JOIN] = &Server::handleJoin;
    handleMap[CMD_PRIVMSG] = &Server::handlePrivMsg;
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
    users[sockfd]->type = FD_SERV;
}

void Server::create_socket() {
// Create a socket (IPv4, TCP)
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    //create_socket();
    listen_port();
}

void Server::wait_connection() {
    int addrlen = sizeof(sockaddr);
    this->connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
}

void Server::listen_port() {
    // Listen to port on any address
    
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(port); // htons is necessary to convert a number to
    // network byte order
    if (bind(this->sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "After bind" << std::endl;
    if (listen(this->sockfd, 10) < 0) {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    std::cout << "After listen" << std::endl;
    wait_connection();
    
    std::cout << "After accept" << std::endl;
    if (this->connection < 0) {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    //std::string response = "001";
    //send(connection, response.c_str(), response.size(), 0);
    //write(this->sockfd, "001", 3);
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

void Server::mainloop() {
    while (true) {
        init_fd();
        do_select();
        check_fd();
    }
}

void Server::s_close() {
    close(this->connection);
    close(this->sockfd);
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

void Server::handleNick(MessageInput *messageInput, MessageOutput *messageOutput) {
    // if (messageInput.getParams.size() == 0) {
    //     sendError(users[fd], )
    // }

    if (!users[fd]->isConnected) {
        users[fd]->connectStatus |= NICK_PASSED;
        users[fd]->isConnected = users[fd]->connectStatus == CONNECTED;
        users[fd]->nickname = &messageInput->params[1];
        send_welcome(fd, messageOutput);
        return ;
    }
    (void)messageInput;
    (void)messageOutput;
}

void Server::handleUser(MessageInput *messageInput, MessageOutput *messageOutput) {
    //TODO check user logic
    
    if (!users[fd]->isConnected) {
        users[fd]->connectStatus |= USER_PASSED;
        users[fd]->isConnected = users[fd]->connectStatus == CONNECTED;
        users[fd]->username = &messageInput->params[0];
        send_welcome(fd, messageOutput);
        return ;
    }
    (void)messageInput;
    (void)messageOutput;
}

void Server::handlePass(MessageInput *messageInput, MessageOutput *messageOutput) {
    //TODO check password logic
    std::cout << messageInput->params[0] << " " << *password << std::endl;
    if (!users[fd]->isConnected && messageInput->params[0] == *password) {
        users[fd]->connectStatus |= PASS_PASSED;
        users[fd]->isConnected = users[fd]->connectStatus == CONNECTED;
        send_welcome(fd, messageOutput);
        return ;
    }
    (void)messageInput;
    (void)messageOutput;
}


void Server::send_welcome(int i, MessageOutput *messageOutput) {
    if (users[i]->isConnected && !users[i]->welcomeReceived) {
        messageOutput->data = "001 :Welcome!\r\n";
        messageOutput->fd_to.push_back(i);
        users[i]->welcomeReceived = true;
    }

}

void Server::handleJoin(MessageInput *messageInput, MessageOutput *messageOutput) {
    //добавить обработку join с параметрами
    for (int i = 0; i < (int)messageInput->params.size(); i++) {
        if (messageInput->params[i][0] == '#') {
            //std::cout << "J PARAMS: " << messageInput->params[i];
            serverData.addChannel(messageInput->params[i]);
            serverData.channels[messageInput->params[0]]->addUser(users[fd]->username);
            messageOutput->data = "372 :Message of the Day";
            messageOutput->fd_to.push_back(fd);
        }
    }
}

void Server::handlePrivMsg(MessageInput *messageInput, MessageOutput *messageOutput) {
   //std::cout << "M PARAMS: " << messageInput->params[0] << std::endl;
   //std::cout << "Channels: " << std::endl;
   serverData.printAllChannels();
//   std::cout << "M FD FROM: " << messageInput->fd_from << std::endl;
//   std::cout << "M USER: " << users[messageInput->fd_from]->username << std::endl;
    //std::cout << serverData.channels[messageInput->params[0]] << std::endl;
    if (serverData.channels.count(messageInput->params[0])) {
    //   std::cout << serverData.channels[messageInput->params[0]] << std::endl;
    //    for (int i = 0; i < serverData.channels[messageInput->params[0]]->getAllUsers().size(); i++) {
    //        std::cout << serverData.channels[messageInput->params[0]]->getAllUsers()[0];
    //    }
        //std::cout << "CHECK" << std::endl;
        serverData.getChannel(messageInput->params[0])->addMessage
        (

            users[messageInput->fd_from]->username,
            serverData.getChannel(messageInput->params[0])->getUsers(),
            &messageInput->params[1]
        );
   }
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
                std::cout << str << std::endl;
                fd = i;
                MessageOutput *messageOutput = parse(str);
                if (!users[i]->isConnected) {
                    std::cout << "connect status " << users[i]->connectStatus << std::endl;
                }
                //TODO уточнить формат сообщений для клиента
                
                
                for(int i = 0; i < (int)messageOutput->fd_to.size(); i++) {
                    send(messageOutput->fd_to[i], &messageOutput->data[0], r, 0);
                }
                delete(messageOutput);
            } else if (users[i]->type == FD_CLIENT) {
                send(i, users[cs]->buf_read , r, 0);
            }
        }

    }
}

MessageOutput *Server::parse(std::string src) {
    const char separator = ' ';
    MessageInput *messageInput = new MessageInput();
    MessageOutput *messageOutput = new MessageOutput();

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

        (this->*(handleMap[messageInput->command]))(messageInput, messageOutput);
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
