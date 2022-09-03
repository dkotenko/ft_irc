//
// Created by Redwyn Poetess on 03.09.2022.
//

#include "Parser.hpp"

Parser::Parser() {}

Message *Parser::parse(std::string src) {
    const char separator = ' ';
    Message *message = new Message();

    std::vector<std::string> outputArray;
    std::stringstream streamData(src);
    std::string val;
    int i = 0;
    while (std::getline(streamData, val, separator)) {
        if (i == 0)
            message->command = val;
        if (i != 0)
            message->params.push_back(val);
        i++;
    }
    if (!message->command.compare("NICK")) {

    }
    return message;
}
