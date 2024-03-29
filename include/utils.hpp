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
#include <sstream>


# define Xv(err,res,str)	(x_void(err,res,str,__FILE__,__LINE__))
# define X(err,res,str)		(x_int(err,res,str,__FILE__,__LINE__))
# define MAX(a,b)	((a > b) ? a : b)

# define USAGE		"Usage: %s port\n"
# define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


int             xassert(int value, const std::string& input);
void		sigh(int n);
void    	printError(std::string s);
void    	here(int n);
std::string     ft_itoa(int n);
std::string     generateRandomString(const int len);
int             getBitByPosition(int n, int position);

#endif
