//
// Created by Redwyn Poetess on 03.09.2022.
//

#ifndef FT_IRC_MESSAGE_H
#define FT_IRC_MESSAGE_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

class Message {
public:
    int fd_from;
    std::vector<int> fd_to;
    std::string command;
    std::vector<std::string> params;
    std::string src;

    Message();
    //~Message();
};


#endif //FT_IRC_MESSAGE_H
