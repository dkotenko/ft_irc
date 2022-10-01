//
// Created by Redwyn Poetess on 03.09.2022.
//

#include "InputMessage.hpp"

InputMessage::InputMessage() {
	

}

int InputMessage::getFd_from() {
	return fd_from;
}

void InputMessage::setFd_from(int fd_from) {
	this->fd_from = fd_from;
}

std::vector<int> InputMessage::getFd_to() {
	return this->fd_to;
}

void InputMessage::setFd_to(std::vector<int> fd_to) {
	this->fd_to = fd_to;
}

std::string InputMessage::getCommand() {
	return this->command;
}

void InputMessage::setCommand(std::string command) {
	this->command = command;
}

std::vector<std::string> InputMessage::getParams() {
	return this->params;
}

void InputMessage::setParams(std::vector<std::string> params) {
	this->params = params;
}

int InputMessage::getCountParams() {
	return this->params.size();
}