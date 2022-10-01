//
// Created by Redwyn Poetess on 22.08.2022.
//

#ifndef FT_IRC_USER_H
#define FT_IRC_USER_H
#include <string>
#include <vector>
#include "MessageOutput.hpp"

# define BUF_SIZE	4096
# define FD_FREE	0
# define FD_SERV	1
# define FD_CLIENT	2

//111
enum e_connect_states {
    NOT_CONNECTED = 0,
    NICK_PASSED = 1,
    USER_PASSED = 2, //10
    PASS_PASSED = 4, //100
    CONNECTED = 7,
    CONNECT_STATES_NUM
};



// JOIN
class User {
public:
    User(int type, int fd);
    std::string username;
    std::string nickname;
    std::string password;
    int connectStatus;
    bool isConnected;
    bool welcomeReceived;
    int type;
    int fd;
    char *buf_read;
    char *buf_write;
    void clean();
    void setUserName(std::string username);
    void setNickName(std::string nickname);
    const std::string &getUserName() const;
    const std::string &getNickName() const;
    void delUser(std::string username);
    MessageOutput *messageOutput;
};

#endif //FT_IRC_USER_H
