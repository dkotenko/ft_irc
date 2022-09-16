//
// Created by Redwyn Poetess on 22.08.2022.
//

#include "User.hpp"
#include <string>
#include <map>

User::User(int type, int fd)
{
	this->clean();
    connectStatus = NOT_CONNECTED;
	this->type = type;
	this->fd = fd;
	buf_read = new char[BUF_SIZE + 1] ();
	buf_write = new char[BUF_SIZE + 1] ();
	isConnected = false;
	welcomeReceived = false;
}

void User::clean() {
	this->type = FD_FREE;
}

void User::setUserName(std::string *username) {
    this->username = username;
}

void User::setNickName(std::string *nickname) {
    this->nickname = nickname;
}

const std::string &User::getUserName() const {
    return *this->username;
}

const std::string &User::getNickName() const {
    return *this->username;
}

void User::delUser(std::string username) {
    (void)username;
}
