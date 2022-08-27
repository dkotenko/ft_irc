//
// Created by Redwyn Poetess on 19.08.2022.
//

#ifndef SERVER_H
#define SERVER_H
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
#include "utils.h"



class Server {
public:
    Server(int port);
    void create_socket();
    void listen_port();
    void mainloop();
    void s_close();
    int port;
    int sockfd;
    int connection;
private:
    void create();
    void check_fd(t_env *e);
    void do_select(t_env *e);
    void wait_connection();
    void init_env();
    void init_fd(t_env *e);
    std::string password;
    t_env *env;
    struct sockaddr_in sockaddr;
};


#endif //FT_IRC_SERVER_H
