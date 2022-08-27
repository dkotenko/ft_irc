//
// Created by Redwyn Poetess on 22.08.2022.
//

#include "User.hpp"
#include <string>
#include <map>



User::User(int fd, std::string nickname) {
    this->fd = fd;
    this->nickname = nickname;
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
    
}
