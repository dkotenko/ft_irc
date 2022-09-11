//
// Created by Redwyn Poetess on 03.09.2022.
//

#ifndef FT_IRC_MESSAGEINPUT_H
#define FT_IRC_MESSAGEINPUT_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

class MessageInput {
public:
    int fd_from;
    std::vector<int> fd_to;
    std::string command;
    std::vector<std::string> params;
    std::string *src;

    MessageInput();
    //~MessageInput();
};


#endif //FT_IRC_MESSAGEINPUT_H
