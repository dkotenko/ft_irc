//
// Created by Redwyn Poetess on 22.08.2022.
//

#include "User.hpp"
#include <string>
#include <map>

User::User(int type, int fd)
{
	this->clean();
    isConnected = false;
	this->type = type;
	this->fd = fd;
	this->buf_read = new char[BUF_SIZE + 1] ();
	this->buf_write = new char[BUF_SIZE + 1] ();
}

void User::clean() {
	this->type = FD_FREE;
}

void User::setUserName(std::string username) {
    this->username = username;
}

void User::setNickName(std::string nickname) {
    this->nickname = nickname;
}

std::string User::getUserName() {
    return this->username;
}

std::string User::getNickName() {
    return this->username;
}

void User::delUser(std::string username) {
    (void)username;
}
