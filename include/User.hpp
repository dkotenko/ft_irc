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
    
    PingTimer timer;
private:
    std::string password;
};

#endif //FT_IRC_USER_H
