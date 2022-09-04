//
// Created by Redwyn Poetess on 03.09.2022.
//

#include "Parser.hpp"

Parser::Parser() {}

MessageInput *Parser::parse(std::string src) {
    const char separator = ' ';
    MessageInput *messageInput = new MessageInput();

    std::vector<std::string> outputArray;
    std::stringstream streamData(src);
    std::string val;
    int i = 0;
    while (std::getline(streamData, val, separator)) {
        if (i == 0)
            messageInput->command = val;
        if (i != 0)
            messageInput->params.push_back(val);
        i++;
    }
    if (!messageInput->command.compare("NICK")) {

    }
    return messageInput;
}
