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
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <algorithm>
#include <vector>
#include <stdbool.h>
#include "User.hpp"
#include "utils.hpp"

#include "ServerData.hpp"
#include "OutputMessage.hpp"
#include "InputMessage.hpp"
#include "const.hpp"
#include "replies.hpp"
#include "logger.hpp"
#include "FileDescriptor.hpp"

#ifdef __APPLE__
#define IRC_NOSIGNAL SO_NOSIGPIPE
#else
#define IRC_NOSIGNAL MSG_NOSIGNAL
#endif

class Server {
public:
    Server(int port, std::string password);
    ~Server();
    int port;
    int sockfd;
    int connection;
    int		maxfd;
    int		max;
    fd_set	fd_read;
    fd_set	fd_write;
    std::vector<FileDescriptor> fds;
    ServerData serverData;
    bool is_debug;
    std::map<std::string, void (Server::*)()> handleMap;
    void run();

private:
    std::string password;
    struct sockaddr_in sockaddr;
    InputMessage *inputMessage;
    OutputMessage *outputMessage;
    User *currUser;
    FileDescriptor *currFd;

    void disconnectDeadUsers();

    void create();
    void check_fd(int activeFdNumber);
    int do_select();
    void init_fd();
    void fct_read(int fd);
    void srv_accept(int s);
    void client_read(int fd);
    void fct_write(int cs);
    void client_write(int cs);
    OutputMessage *parse(std::string msg, int fd);
    void populateHandleMap();
    void sendWelcome();
    void pingUsers();
    void registerNewUser(FileDescriptor *FileDescriptor);
    void doQuit(User *user);
    std::string connectStatusAsString(int status);
    User *getUserByFd(int fd);

    void handleNick();
    void handleUser();
    void handlePass();
    void handleJoin();
    void handlePrivMsg();
    void handleQuit();
    void handleMode();
    void handleTopic();
    void handleInvite();
    void handleKick();
    void handleError(int err, const std::string &arg1, const std::string &arg2);
    void handleNames();
    void handlePong();
    void handleNotice();
    void handleList();
    void handleWhoIs();
    void handleAway();
    std::vector<User *> getUsersByWildcard(std::string &wildcard);
};
#endif //FT_IRC_SERVER_H