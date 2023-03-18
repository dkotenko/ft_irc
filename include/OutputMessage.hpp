//
// Created by Redwyn Poetess on 04.09.2022.
//

#ifndef FT_IRC_MESSAGEOUTPUT_H
#define FT_IRC_MESSAGEOUTPUT_H
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include "const.hpp"
#include "replies.hpp"
#include "logger.hpp"

class OutputMessage {
    public:
        std::string data;
        int fd;

        OutputMessage();
        OutputMessage(std::string nickName);
        OutputMessage(OutputMessage &src);
        void add(std::string s, int replyCode);
        void addPrivMsg(std::string &s, std::string &fromusername, std::string &fromhostname, std::string &tousername);
        void sendMsg(int fd);
        std::queue<std::string> lines;
    private:
        int length;
        std::string servername;
        std::string nickName;
        std::string getReplyCodeAsString(int replyCode);
};


#endif //FT_IRC_MESSAGEOUTPUT_H
