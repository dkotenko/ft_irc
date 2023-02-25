//
// Created by Redwyn Poetess on 04.09.2022.
//

#include "OutputMessage.hpp"

OutputMessage::OutputMessage(std::string servername, std::string nickName) {
    this->servername = servername;
    this->nickName = nickName;
}

OutputMessage::~OutputMessage() {
    clear();
}

void OutputMessage::addFd(int fd) {
    for (int i = 0; i < fd_to.size(); i++) {
        if (fd_to[i] == fd) {
            return ;
        }
    }
    fd_to.push_back(fd);
}

std::string OutputMessage::getReplyCodeAsString(int replyCode) {
    std::stringstream ss;

    if (replyCode == RPL_NONE) {
        return std::string("");
    }
    ss << ":" << servername << " " << replyCode << " " << nickName << " ";
    return ss.str();
}

void OutputMessage::add(std::string s, int replyCode) {
    std::string line = getReplyCodeAsString(replyCode);
    int maxLength = MESSAGE_MAX_LEN - std::strlen(ENDLINE) - line.length();
    if (s.length() > maxLength) {
        s = s.substr(0, maxLength);
    }
    line += s;
    line += ENDLINE;
    lines.push_back(line);
}

void OutputMessage::add(std::string s, int replyCode, int fd) {
    addFd(fd);
    add(s, replyCode);
}

void OutputMessage::addPrivmsg(std::string s, int fd, std::string fromusername, std::string fromhostname, std::string tousername) {
    addFd(fd);
    std::string line = ":";
    line += fromusername;
    line += "!";
    line += fromusername;
    line += "@";
    line += fromhostname;
    line += " PRIVMSG ";
    line += tousername;
    line += " ";
    line += s;
    line += ENDLINE;
    lines.push_back(line);

/*
    std::string line;
    line + s;
    lines.push_back(line);
*/
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
