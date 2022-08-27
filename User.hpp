//
// Created by Redwyn Poetess on 22.08.2022.
//

#ifndef FT_IRC_USER_H
#define FT_IRC_USER_H
#include <string>
#include <vector>

# define BUF_SIZE	4096
# define FD_FREE	0
# define FD_SERV	1
# define FD_CLIENT	2

struct message {
    int fd_from;
    std::vector<int> fd_to;
    std::string operation;
    std::string data;
};

class User {
public:
    User(int type, int fd);
    std::string username;
    std::string nickname;
    std::string password;
    bool isConnected;
    int type;
    int fd;
    char *buf_read;
    char *buf_write;
    void clean();
    void setUserName(std::string username);
    void setNickName(std::string nickname);
    std::string getUserName();
    std::string getNickName();
    void delUser(std::string username);
};

#endif //FT_IRC_USER_H
