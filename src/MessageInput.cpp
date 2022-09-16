//
// Created by Redwyn Poetess on 03.09.2022.
//

#include "MessageInput.hpp"

MessageInput::MessageInput() {
	

}

int MessageInput::getFd_from() {
	return fd_from;
}

void MessageInput::setFd_from(int fd_from) {
	this->fd_from = fd_from;
}

std::vector<int> MessageInput::getFd_to() {
	return this->fd_to;
}

void MessageInput::setFd_to(std::vector<int> fd_to) {
	this->fd_to = fd_to;
}

std::string MessageInput::getCommand() {
	return this->command;
}

void MessageInput::setCommand(std::string command) {
	this->command = command;
}

std::vector<std::string> MessageInput::getParams() {
	return this->params;
}

void MessageInput::setParams(std::vector<std::string> params) {
	this->params = params;
}

