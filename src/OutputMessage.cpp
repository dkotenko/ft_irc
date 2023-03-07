//
// Created by Redwyn Poetess on 04.09.2022.
//

#include "OutputMessage.hpp"

OutputMessage::OutputMessage() {}

OutputMessage::OutputMessage(std::string nickName, int fd) :
    servername(SERVER_NAME)
{
    this->fd = fd;
    this->nickName = nickName;
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
    lines.push(line);
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
    lines.push(line);
}

void OutputMessage::sendMsg(int fd) {
    if (lines.size() == 0) {
		return ;
	}
    
    std::string toSend;

	if (lines.front().length() > MESSAGE_MAX_LEN) {
		toSend = lines.front().substr(0, MESSAGE_MAX_LEN);
		lines.front() = lines.front().substr(MESSAGE_MAX_LEN);
        send(fd, toSend.c_str(), toSend.length(), 0);
	} else {
		toSend = lines.front();
        send(fd, toSend.c_str(), toSend.length(), 0);
        lines.pop();
	}
    log_debug("message to fd %d was sent:\n%s", fd, toSend.c_str());
}
