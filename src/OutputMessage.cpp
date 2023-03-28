//
// Created by Redwyn Poetess on 04.09.2022.
//

#include "OutputMessage.hpp"

OutputMessage::OutputMessage() {}

OutputMessage::OutputMessage(std::string nickName) :
    servername(SERVER_NAME),
    nickName(nickName)
{}

OutputMessage::OutputMessage(OutputMessage &src) {}

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

void OutputMessage::addPrivMsg(std::string &s, std::string &fromusername, std::string &fromhostname, std::string &tousername) {
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
        while (lines.size() > 0) {
            if (toSend.length() + lines.front().length() > MESSAGE_MAX_LEN) {
                break ;
            }
            toSend += lines.front();
            lines.pop();
        }
        send(fd, toSend.c_str(), toSend.length(), 0);
	}
    log_debug("message to fd %d was sent:\n%s", fd, toSend.c_str());
}
