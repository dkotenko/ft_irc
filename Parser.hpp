//
// Created by Redwyn Poetess on 03.09.2022.
//

#ifndef FT_IRC_PARSER_H
#define FT_IRC_PARSER_H
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "utils.h"
#include "Message.hpp"

class Parser {
public:
    Parser();
    Message *parse(std::string src);
};


#endif //FT_IRC_PARSER_H
