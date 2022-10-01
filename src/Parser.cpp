//
// Created by Redwyn Poetess on 03.09.2022.
//

#include "Parser.hpp"

Parser::Parser() {}

InputMessage *Parser::parse(std::string src) {
    const char separator = ' ';
    InputMessage *inputMessage = new InputMessage();

    std::vector<std::string> outputArray;
    std::stringstream streamData(src);
    std::string val;
    int i = 0;
    while (std::getline(streamData, val, separator)) {
        if (i == 0)
            inputMessage->command = val;
        if (i != 0)
            inputMessage->params.push_back(val);
        i++;
    }
    if (!inputMessage->command.compare("NICK")) {

    }
    return inputMessage;
}
