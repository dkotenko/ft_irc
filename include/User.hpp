#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <map>
#include "OutputMessage.hpp"
#include "PingTimer.hpp"
#include <queue>

# define BUF_SIZE	4096

enum e_fds {
    FD_FREE     = 0,
    FD_SERV     = 1,
    FD_CLIENT   = 2,
    FD_NUM      = 3
};

enum e_connect_states {
    NOT_REGISTERED  = 0,
    NICK_PASSED     = 1, //0b001
    USER_PASSED     = 2, //0b010
    PASS_PASSED     = 4, //0b100
    REGISTERED      = 7,  //0b111
    CONNECT_STATES_NUM = 5
};



// JOIN
class User {
public:
    User(int type, int fd);
    ~User();
    
    std::string username;
    std::string nickname;
    std::string realname;
    std::string hostname;
    std::string ipAddress;
    int port;
    std::string servername;
    std::string awaytext;
    bool awaystatus;
    int connectStatus;
    bool registered;
    bool welcomeReceived;
    int type;
    int fd;
    char *buf_read;
    char *buf_write;
    PingTimer timer;
    std::deque<OutputMessage>messages;

    void clean();
    void setAwayText(std::string awaytext);
    void setAwayStatus(bool awaystatus);
    const std::string &getUsername() const;
    const std::string &getNickname() const;
    bool getAwayStatus();
    std::string getAwayText();
    void deleteUser(std::string username);
    bool isLost();
    void doPing();
    void updatePing();
    bool isNeedsPing();
    bool isRegistered();
    void setRegistered(bool b);
    void sendMessage();
private:
    std::string password;
};

