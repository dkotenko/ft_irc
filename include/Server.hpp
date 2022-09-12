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
#include "utils.hpp"
#include <vector>
#include "Parser.hpp"
#include "ServerData.hpp"
#include "MessageOutput.hpp"
#include "const.hpp"


class Server {
public:
    Server(int port, std::string *password);
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
    int fd;
    fd_set	fd_read;
    fd_set	fd_write;
    std::vector<User *> users;
    ServerData serverData;
    std::map<std::string, void (Server::*)(MessageInput *, MessageOutput *)> handleMap;
private:
    std::string *password;
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
    MessageOutput *parse(std::string msg);
    void populatehandleMap();
    void send_welcome(int i, MessageOutput *messageOutput);

    void handleNick(MessageInput *messageInput, MessageOutput *output);
    void handleUser(MessageInput *messageInput, MessageOutput *output);
    void handlePass(MessageInput *messageInput, MessageOutput *output);
    void handleJoin(MessageInput *messageInput, MessageOutput *output);
    void handlePrivMsg(MessageInput *messageInput, MessageOutput *output);

};
#endif //FT_IRC_SERVER_H
