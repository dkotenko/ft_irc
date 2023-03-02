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

void OutputMessage::addPrivMsg(std::string s, int fd, std::string fromusername, std::string fromhostname, std::string tousername) {
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
    std::stringstream ss;
    int len = 0;

    for(int i = 0; i < fd_to.size(); i++) {
        const char* const delim = "";
        std::ostringstream joined;
        std::copy(lines.begin(), lines.end(),
           std::ostream_iterator<std::string>(joined, delim));
        log_debug("message to fd %d was sent:\n%s", fd_to[i], joined.str().c_str());
        send(fd_to[i], joined.str().c_str(), joined.str().length(), 0);
    }
    
}

void OutputMessage::clear() {
    lines.clear();
}
