#include "Server.hpp"
#include "utils.hpp"

Server::Server(int port, std::string password) {
    struct rlimit rlp;
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
  struct timeval timeout = {0, 0};
  r = select(max + 1, &fd_read, &fd_write, NULL, &timeout);
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
        disconnectDeadUsers();
        pingUsers();
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
    std::string ipAddress = inet_ntoa(csin.sin_addr);
    int port = ntohs(csin.sin_port);
    std::cout << "New client # " << cs << " from " << ipAddress << ":" << port << std::endl;

    /*
    printf("New client #%d from %s:%d\n", cs, 
        inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));
    */
    users[cs]->type = FD_CLIENT;
    users[cs]->ipAddress = ipAddress;
    users[cs]->port = port;
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
				std::cout << "line received: " << users[cs]->buf_read << std::endl;
				fd = i;
				currUser = users[fd];

				std::string str;
				while (std::getline(streamData, str, '\n')) {
					str.erase(std::remove(str.begin(), str.end(), '\r' ), str.end());
                	str.erase(std::remove(str.begin(), str.end(), '\n' ), str.end());
					outputMessage = parse(str);
					outputMessage->sendMsg();
					delete(outputMessage);
				}
            } else if (users[i]->type == FD_CLIENT) {
                send(i, users[cs]->buf_read , r, 0);
            }
        }

    }
}

OutputMessage *Server::parse(std::string src) {
    const char separator = ' ';
    inputMessage = new InputMessage();
    outputMessage = new OutputMessage(serverName, currUser->getNickname());

    std::vector<std::string> outputArray;
    std::stringstream streamData(src);
    std::string val;
    int i = 0;
    inputMessage->fd_from = this->fd;
    while (std::getline(streamData, val, separator)) {
        if (i == 0 && val[0] != ':') {
            inputMessage->command = val;
        }
        if (i != 0) {
            inputMessage->getParams().push_back(val);
        }
        i++;
    }

    if (handleMap.count(inputMessage->command) == 1) {

        (this->*(handleMap[inputMessage->command]))();
        currUser->updatePing();
    }
    delete(inputMessage);
    return outputMessage;
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

void Server::pingUsers() {
    std::map<std::string, User*>::iterator it;
    

    //std::cout << serverData.users.size() << std::endl;
    for (it = serverData.users.begin(); it != serverData.users.end(); it++)
    {
        //std::cout << it->second->username << std::endl;
        if (it->second->isNeedsPing()) {
            it->second->doPing();
            OutputMessage outputMessage(serverName, "");
            std::string toAdd("PING :");
            toAdd += serverName;
            outputMessage.add(toAdd, RPL_NONE, it->second->fd);
            outputMessage.sendMsg();
        }
    }
}

void Server::disconnectDeadUsers() {
    std::vector<User *> to_delete;

    for (std::map<std::string, User*>::iterator it = serverData.users.begin(); it != serverData.users.end(); ) {
        if (it->second->isRegistered() && it->second->isLost()) {
            std::cout << it->second->username << " disconnected" << std::endl;
            to_delete.push_back(it->second);
        }
        it++;
    }

    for (int i = 0; i < to_delete.size(); i++) {
        doQuit(to_delete[i]);
    }
}