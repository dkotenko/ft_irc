//
// Created by Redwyn Poetess on 04.09.2022.
//

#ifndef FT_IRC_MESSAGEOUTPUT_H
#define FT_IRC_MESSAGEOUTPUT_H
#include <string>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <iostream>

#include "const.hpp"


class OutputMessage {
    public:
        std::string data;
        int fd_from;
        std::vector<int> fd_to;

        OutputMessage();
        ~OutputMessage();
        void add(std::string s);
        void add(std::string s, int fd);
        void addFd(int fd);
        void sendMsg();
        std::string toString();
        //~OutputMessage();
    private:
        int length;
        std::vector<std::string> messages;

};


#endif //FT_IRC_MESSAGEOUTPUT_H
