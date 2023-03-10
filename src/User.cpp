//
// Created by Redwyn Poetess on 22.08.2022.
//

#include "User.hpp"

User::User() :
	fd(NO_FD)
{
	this->clean();
	this->nickname = "";
	this->username = "";
}

User::User(FileDescriptor *fileDescriptor) :
	fd(fileDescriptor->fd)
{
	this->clean();
	this->nickname = fileDescriptor->userInfo.nickname;
	this->username = fileDescriptor->userInfo.username;
}

void User::clean() {
}


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

User::~User() {
	this->clean();
}

bool User::isLost() {
	return timer.isNoResponce();
}

void User::doPing() {
	timer.doPing();
}

void User::updatePing() {
	timer.reset();
}

bool User::isNeedsPing() {
	return timer.isNeedsPing();
}

void User::sendMessage() {
	
}

bool User::hasMessage() {
	return outputMessage.lines.size() > 0;
}