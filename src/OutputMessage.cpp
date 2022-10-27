//
// Created by Redwyn Poetess on 04.09.2022.
//

#include "OutputMessage.hpp"

OutputMessage::OutputMessage(std::string serverName, std::string nickName) {
    this->serverName = serverName;
    this->nickName = nickName;
}

OutputMessage::~OutputMessage() {
    clear();
}

void OutputMessage::add(std::string s, int replyCode, int fd) {
    add(s, replyCode);
}

std::string getPrefix(int replyCode) {
    std::stringstream ss;

    ss << ":" << serverName << " " << replyCode << " " << nickName << " :";
    return ss.str();
}

void OutputMessage::add(std::string s, int replyCode) {
    std::string line = getPrefix(replyCode);
    int maxLength = MESSAGE_MAX_LEN - std::strlen(ENDLINE) - line.length();
    if (s.length() > maxLength) {
        s = s.substr(0, maxLength);
    }
    line += s;
    line += ENDLINE;
    lines.push_back(s);
}

void OutputMessage::sendMsg() {
    for(int i = 0; i < fd_to.size(); i++) {
        for (int line = 0; line < lines.size(); line++) {
            send(fd_to[i], lines[line].c_str(), lines[line].length(), 0);
            std::cout << "message to fd " << fd_to[i] << " was sent: " << lines[line].c_str();
        }
    }
}

void OutputMessage::clear() {
    lines.clear();
}
