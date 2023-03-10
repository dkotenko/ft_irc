#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <map>
#include "OutputMessage.hpp"
#include <queue>
#include "const.hpp"
#include "FileDescriptor.hpp"





// JOIN
class User {
public:
    User();
    User(FileDescriptor *fileDescriptor);
    ~User();
    
    std::string username;
    std::string nickname;
    std::string realname;
    std::string hostname;

    std::string awaytext;
    bool awaystatus;
	bool welcomeReceived;
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
    void sendMessage();
    bool hasMessage();
private:
    std::string password;
};

