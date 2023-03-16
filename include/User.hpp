#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <map>
#include "OutputMessage.hpp"
#include <queue>
#include "const.hpp"
#include "FileDescriptor.hpp"

class User {
    public:
        User();
        User(const User & src);
        User(FileDescriptor *fileDescriptor);
        ~User();
        
        std::string username;
        std::string nickname;
        std::string realname;
        std::string hostname;
        std::string awaytext;
        int fd;
        bool awaystatus;
        bool welcomeReceived;
        bool online;
        
        //TODO priv msg при отправке проверяет в сети ли юзер и отправляет "не в сети" если не в сети
        OutputMessage outputMessage;

        void setAwayText(std::string awaytext);
        void setAwayStatus(bool awaystatus);
        const std::string &getUsername() const;
        const std::string &getNickname() const;
        bool getAwayStatus();
        std::string getAwayText();
        void deleteUser(std::string username);
        void sendMessage();
        bool hasMessage();
    private:
        std::string password;
};

