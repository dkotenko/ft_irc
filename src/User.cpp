//
// Created by Redwyn Poetess on 22.08.2022.
//

#include "User.hpp"

User::User() :
	fd(NO_FD),
	username(""),
	nickname(""),
    realname(""),
    hostname(""),
    awaytext(""),
    awaystatus(false),
    welcomeReceived(false),
    online(false)
{
}

User::User(FileDescriptor *fileDescriptor) :
	fd(fileDescriptor->fd),
	username(fileDescriptor->userInfo.username),
	nickname(fileDescriptor->userInfo.nickname),
    realname(fileDescriptor->userInfo.realname),
    hostname(fileDescriptor->userInfo.hostname),
    awaytext(""),
    awaystatus(false),
    welcomeReceived(false),
    online(false)
{}


void User::setAwayStatus(bool awaystatus) {
	this->awaystatus = awaystatus;
}

void User::setAwayText(std::string awaytext) {
	this->awaytext = awaytext;
}

const std::string &User::getUsername() const {
    return this->username;
}

const std::string &User::getNickname() const {
    return this->username;
}

bool User::getAwayStatus() {
	return this->awaystatus;
}

std::string User::getAwayText() {
	return this->awaytext;
}

void User::deleteUser(std::string username) {
    (void)username;
}

void User::sendMessage() {
	
}

bool User::hasMessage() {
	return outputMessage.lines.size() > 0;
}