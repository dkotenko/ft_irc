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
  std::cout << "New client # " << cs << " from " << inet_ntoa(csin.sin_addr) << ":" << ntohs(csin.sin_port) << std::endl;
  /*
  printf("New client #%d from %s:%d\n", cs, 
	 inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));
  */
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
				std::cout << "line received: " << users[cs]->buf_read << std::endl;
				fd = i;
				currUser = users[fd];

				std::string str;
				while (std::getline(streamData, str, '\n')) {
					str.erase(std::remove(str.begin(), str.end(), '\r' ), str.end());
                	str.erase(std::remove(str.begin(), str.end(), '\n' ), str.end());
					currUser->outputMessage = parse(str);
					//TODO уточнить формат сообщений для клиента
					currUser->outputMessage->sendMsg();
					delete(currUser->outputMessage);
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
    outputMessage = new OutputMessage();

    std::vector<std::string> outputArray;
    std::stringstream streamData(src);
    std::string val;
    int i = 0;
    inputMessage->fd_from = this->fd;
    while (std::getline(streamData, val, separator)) {
        if (val[0] == ':') {
            val = val.substr(1, val.size() - 1);
        }
        if (i == 0) {
            inputMessage->command = val;
        }
        if (i != 0) {
            inputMessage->getParams().push_back(val);
        }
        i++;
    }

    if (handleMap.count(inputMessage->command) == 1) {

        (this->*(handleMap[inputMessage->command]))();
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
