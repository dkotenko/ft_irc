//
// Created by Redwyn Poetess on 03.09.2022.
//

#ifndef FT_IRC_PARSER_H
#define FT_IRC_PARSER_H
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "utils.hpp"
#include "InputMessage.hpp"
#include "ServerData.hpp"

class Parser {
public:
    Parser();
    InputMessage *parse(std::string src);
    ServerData serverData;
};


#endif //FT_IRC_PARSER_H
