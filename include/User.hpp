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
        //TODO priv msg при отправке проверяет в сети ли юзер и отправляет "не в сети" если не в сети
        bool online;
        
        OutputMessage outputMessage;

        void clean();
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

