//
// Created by Redwyn Poetess on 19.08.2022.
//

#ifndef SERVER_H
#define SERVER_H
#include <string>

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
    std::string password;
};


#endif //FT_IRC_SERVER_H
