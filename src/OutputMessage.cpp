//
// Created by Redwyn Poetess on 04.09.2022.
//

#include "OutputMessage.hpp"

OutputMessage::OutputMessage() {
    length = 0;
}

OutputMessage::~OutputMessage() {
    length = 0;
}

void OutputMessage::addFd(int fd) {
    fd_to.push_back(fd);
}

void OutputMessage::add(std::string s, int fd) {
    addFd(fd);
    add(s);
}

void OutputMessage::add(std::string s) {

    if (length >= MESSAGE_MAX_LEN) {
        return ;
    }

    if (length + s.length() > MESSAGE_MAX_LEN) {
        s = s.substr(0, MESSAGE_MAX_LEN - length);
    }

    messages.push_back(s);
    length += s.length();
}

std::string OutputMessage::toString() {
    std::stringstream ss;

    for(std::vector<std::string>::const_iterator itr = messages.begin();
        itr != messages.end();
        ++itr) {
        ss << *itr;
    }
    return ss.str();
}

void OutputMessage::sendMsg() {
    std::string s = toString();
    std::cout << "message to send: " << s << std::endl;
    for(int i = 0; i < fd_to.size(); i++) {
        send(fd_to[i], s.c_str(), length, 0);
    }
}