//
// Created by Redwyn Poetess on 22.08.2022.
//

#ifndef FT_IRC_USER_H
#define FT_IRC_USER_H
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include "OutputMessage.hpp"
#include "PingTimer.hpp"

# define BUF_SIZE	4096
# define FD_FREE	0
# define FD_SERV	1
# define FD_CLIENT	2

//111
enum e_connect_states {
    NOT_REGISTERED = 0,
    NICK_PASSED = 1,
    USER_PASSED = 2, //10
    PASS_PASSED = 4, //100
    REGISTERED = 7,
    CONNECT_STATES_NUM
};



// JOIN
class User {
public:
    User(int type, int fd);
    ~User();
    
    std::string username;
    std::string nickname;
    std::string password;
    std::string realusername;
    std::string hostname;
    std::string servername;
    int connectStatus;
    bool registered;
    bool welcomeReceived;
    int type;
    int fd;
    char *buf_read;
    char *buf_write;
    void clean();
    void setUserName(std::string username);
    void setNickName(std::string nickname);
    const std::string &getUsername() const;
    const std::string &getNickname() const;
    void delUser(std::string username);
    bool isDisconnected();
    void doPing();
    void updatePing();
    bool isNeedsPing();
    
    bool isRegistered();
    void setRegistered(bool b);

    OutputMessage *outputMessage;
    PingTimer timer;
};

#endif //FT_IRC_USER_H
