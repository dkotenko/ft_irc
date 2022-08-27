#ifndef UTILS_H
# define UTILS_H

#include <string>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <cerrno>
#include <sys/select.h>
#include <sys/resource.h>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <unistd.h> // For read
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <netdb.h>
#include <arpa/inet.h>
#include "FileDescriptor.hpp"



# define Xv(err,res,str)	(x_void(err,res,str,__FILE__,__LINE__))
# define X(err,res,str)		(x_int(err,res,str,__FILE__,__LINE__))
# define MAX(a,b)	((a > b) ? a : b)

# define USAGE		"Usage: %s port\n"


typedef struct	s_env
{
  FileDescriptor fds[FD_SETSIZE];
  int		port;
  int		maxfd;
  int		max;
  int		r;
  fd_set	fd_read;
  fd_set	fd_write;
}		t_env;

int		xassert(int value, const std::string& input);
void	sigh(int n);
void fct_read(struct s_env *env, int fd);
void srv_accept(struct s_env *env, int s);
void client_read(struct s_env *env, int cs);
void fct_write(struct s_env *env, int cs);
void client_write(struct s_env *env, int cs);
#endif
