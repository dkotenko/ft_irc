//
// Created by Redwyn Poetess on 03.09.2022.
//

#ifndef FT_IRC_MESSAGEINPUT_H
#define FT_IRC_MESSAGEINPUT_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

class InputMessage {
public:
    std::vector<int> fd_to;
    std::string command;
    std::vector<std::string> params;
    std::string *src;
    int countParams;

    int getParamsSize();
    std::vector<std::string> &getParams();
};


#endif //FT_IRC_MESSAGEINPUT_H
