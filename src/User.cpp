//
// Created by Redwyn Poetess on 22.08.2022.
//

#include "User.hpp"

User::User(int type, int fd)
{
	this->type = type;
	this->fd = fd;
	buf_read = new char[BUF_SIZE + 1] ();
	buf_write = new char[BUF_SIZE + 1] ();
	this->clean();
	this->nickname = "";
}

void User::clean() {
	this->type = FD_FREE;
	connectStatus = NOT_REGISTERED;
	registered = false;
	welcomeReceived = false;
	std::memset(buf_read, 0, BUF_SIZE + 1);
	std::memset(buf_write, 0, BUF_SIZE + 1);
}

void User::setUserName(std::string username) {
    this->username = username;
}

void User::setNickName(std::string nickname) {
    this->nickname = nickname;
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

void User::delUser(std::string username) {
    (void)username;
}

bool User::isRegistered() {
    return registered;
}

void User::setRegistered(bool b) {
	registered = b;
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
	//std::cout << timer.lastPingTimeMs << " " << username << " : ping updated" << std::endl;
	timer.reset();
}

bool User::isNeedsPing() {
	return timer.isNeedsPing();
}
