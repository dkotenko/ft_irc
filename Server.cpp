

#include "Server.hpp"
#include "utils.h"

Server::Server(int port) {
    this->port = port;
    init_env();
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
  env->fds[s].type = FD_SERV;
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

void   Server::init_env() {
    int		i;
    struct rlimit rlp;
	
    env = (t_env *)malloc(sizeof(t_env));
	xassert(getrlimit(RLIMIT_NOFILE, &rlp) != -1, "getrlimit");
    env->maxfd = FD_SETSIZE - 1;
    /*
	env->maxfd = rlp.rlim_cur;
	if (env->maxfd >= FD_SETSIZE)
		env->maxfd = FD_SETSIZE - 1;
    
    env->fds = (t_fd*)malloc(sizeof(*env->fds) * env->maxfd);
    */
    //xassert(e.fds > 0, "malloc error");
    i = 0;
    while (i < env->maxfd)
    {
        env->fds[i] = FileDescriptor();
        i++;
    }
}

void	Server::init_fd(t_env *e)
{
  int	i;

  i = 0;
  e->max = 0;
  FD_ZERO(&e->fd_read);
  FD_ZERO(&e->fd_write);
  while (i < e->maxfd)
    {
      if (e->fds[i].type != FD_FREE)
	{
	  FD_SET(i, &e->fd_read);
	  if (strlen(e->fds[i].buf_write) > 0)
	    {
	      FD_SET(i, &e->fd_write);
	    }
	  e->max = MAX(e->max, i);
	}
      i++;
    }
}

void	Server::do_select(t_env *e)
{
  e->r = select(e->max + 1, &e->fd_read, &e->fd_write, NULL, NULL);
}

void	Server::check_fd(t_env *e)
{
  int	i;

  i = 0;
  while ((i < e->maxfd) && (e->r > 0))
    {
      if (FD_ISSET(i, &e->fd_read)) {
        fct_read(e, i);
      }
      if (FD_ISSET(i, &e->fd_write))
	    fct_write(e, i);
      if (FD_ISSET(i, &e->fd_read) ||
	  FD_ISSET(i, &e->fd_write))
	e->r--;
      i++;
    }
}

void Server::mainloop() {
    while (true) {
        init_fd(env);
        do_select(env);
        check_fd(env);
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


