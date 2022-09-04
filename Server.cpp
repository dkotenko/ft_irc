#include "Server.hpp"
#include "utils.h"

Server::Server(int port) {
    struct rlimit rlp;
    parser = Parser();
    this->port = port;
    xassert(getrlimit(RLIMIT_NOFILE, &rlp) != -1, "getrlimit");
    maxfd = FD_SETSIZE - 1;

    for (int i = 0; i < maxfd; i++)
    {
        users.push_back(new User(FD_FREE, i));
    }
    
    this->create();
}

void Server::create()
{
  int			s;
  struct sockaddr_in	sin;

  s =  socket(PF_INET, SOCK_STREAM, 0);
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port);
  bind(s, (struct sockaddr*)&sin, sizeof(sin));
  listen(s, 42);
  users[s]->type = FD_SERV;
}

void Server::create_socket() {
// Create a socket (IPv4, TCP)
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    create_socket();
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
        /*
        // Read from the connection
        char buffer[100];
        int r = read(this->connection, buffer, 100);
        if (r < 0) {
            std::cout << "Read error: " << std::cout;
        }
        else if (r == 0) {
            std::cout << "Client disconnected" << std::endl;
            wait_connection();
        }
        if (strcmp(buffer, "QUIT\n") == 0) {
            break;
        }
        else if (r > 0) {
            std::cout << "The message was: " << buffer;
        }
        
        // Send a message to the connection
        //std::string response = "Good talking to you\n";
        //send(connection, response.c_str(), response.size(), 0);
        bzero(buffer, 100);
        */
    }
}

void Server::s_close() {
    // Close the connections
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

void Server::client_read(int cs)
{
  int	r;
  int	i;

  //parser.serverData = serverData;
  ServerData *serverdata = new ServerData;
  serverdata->addUser("name1", "pass");

  r = recv(cs, users[cs]->buf_read, BUF_SIZE, 0);
  if (r <= 0)
    {
      close(cs);
      users[cs]->clean();
      printf("client #%d gone away\n", cs);
    }
  else
    {
      i = 0;
      while (i < maxfd)
	{
	  if ((users[i]->type == FD_CLIENT) &&
	      (i == cs)) {

          std::string str(users[cs]->buf_read);

          Message *msg(parse(i, str));
          std::cout << users[i]->connectStatus << std::endl;

          std::string welcomeMessage = "001 :Welcome!";
          memcpy(users[cs]->buf_read, &welcomeMessage[0], welcomeMessage.length());
          send(i, users[cs]->buf_read , r, 0);

          if (users[i]->isConnected) {
              ;
          }
          for (int j = 0; j < (int)msg->params.size(); j++) {
              //send(i, &space[0] , r, 0);
              //memset(users[cs]->buf_read, 0, 100);
          }
      } else {
          send(i, users[cs]->buf_read , r, 0);
      }
	  i++;
	}
    }
}

Message *Server::parse(int fd, std::string src) {
    const char separator = ' ';
    Message *message = new Message();

    std::vector<std::string> outputArray;
    std::stringstream streamData(src);
    std::string val;
    int i = 0;
    while (std::getline(streamData, val, separator)) {
        if (i == 0)
            message->command = val;
        if (i != 0)
            message->params.push_back(val);
        i++;
    }

    if (!users[fd]->isConnected) {
        if (!message->command.compare("NICK")) {
            users[fd]->connectStatus |= NICK_PASSED;
        } else if (!message->command.compare("USER")) {
            users[fd]->connectStatus |= USER_PASSED;
        } else if (!message->command.compare("PASS")) {
            users[fd]->connectStatus |= PASS_PASSED;
        }
        users[fd]->isConnected = users[fd]->connectStatus & CONNECTED;
    } else {
        //other commands
    }


    return message;
}

void Server::fct_write(int cs)
{
	client_write(cs);
}

void Server::client_write(int cs)
{
    (void)cs;
}


