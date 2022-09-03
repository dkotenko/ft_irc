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
#include <algorithm>
#include "User.hpp"
#include "utils.h"
#include <vector>
#include "Parser.hpp"
#include "ServerData.hpp"


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
    int		maxfd;
    int		max;
    int		r;
    fd_set	fd_read;
    fd_set	fd_write;
    std::vector<User *> users;
private:
    std::string password;
    struct sockaddr_in sockaddr;
    Parser parser;

    FileDescriptor *getByFd(int fd);
    void create();
    void check_fd();
    void do_select();
    void wait_connection();
    void init_env();
    void init_fd();
    void fct_read(int fd);
    void srv_accept(int s);
    void client_read(int cs);
    void fct_write(int cs);
    void client_write(int cs);
    Message *parse(int fd, std::string msg);
};


#endif //FT_IRC_SERVER_H
