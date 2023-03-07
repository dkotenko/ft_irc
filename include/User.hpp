#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <map>
#include "OutputMessage.hpp"
#include "PingTimer.hpp"
#include <queue>
#include "const.hpp"



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
    User(int fd);
    ~User();
    
    std::string username;
    std::string nickname;
    std::string realname;
    std::string hostname;

    std::string awaytext;
    bool awaystatus;
    int fd;
    
    PingTimer timer;
    OutputMessage outputMessage;

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
    bool hasMessage();
private:
    std::string password;
};

