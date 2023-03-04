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
        int fd_from;
        std::vector<int> fd_to;

        OutputMessage(std::string servername, std::string nickName);
        void add(std::string s, int replyCode);
        void add(std::string s, int replyCode, int fd);
        void addPrivMsg(std::string s, int fd, std::string fromusername, std::string fromhostname, std::string tousername);
        void addFd(int fd);
        void sendMsg(int fd);
    private:
        int length;
        std::queue<std::string> lines;
        std::string servername;
        std::string nickName;
        std::string getReplyCodeAsString(int replyCode);
};


#endif //FT_IRC_MESSAGEOUTPUT_H
