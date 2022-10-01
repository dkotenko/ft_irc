
#include "Channel.hpp"

Channel::Channel(std::string chname) {
    chname = chname;
}

void Channel::addUser(std::string username) {
    if (users.size() == 0) {
        operator_username = username;
    }
    users.push_back(username);
}

void Channel::delUser(std::string username) {
    for(int i = 0; i < (int)users.size(); i++) {
        if (users[i] == username) {
            users.erase(users.begin() + i);
        }
    }
}

std::vector<std::string> const &Channel::getUsers () const {
    return users;
}

void Channel::addMessage(std::string from_username, std::vector<std::string> to_usernames, std::string message) {
    endmessage new_message;
    new_message.from_m = from_username;
    new_message.to_m = to_usernames;
    new_message.datamessage = message;
    
    Channel::messages.push_back(new_message);
}


endmessage Channel::getMessage() {
    endmessage res;
    res = messages.back();
    //Channel::messages.pop_back();
    return res;
}

std::string Channel::getOperatorUsername() {
    return this->operator_username;
}

bool Channel::checkUserInChannel(std::string username) {
    for (int i = 0; i < this->users.size(); i++) {
        if (this->users[i] == username)
            return true;
    }
    return false;
}

void Channel::doKick(std::string username) {
    if (checkUserInChannel(username))
        this->delUser(username);
}

void Channel::setTopic(std::string topic) {
    this->topic = topic;
}

void Channel::editMode() {
    return;
}

void Channel::doInvite(std::string nickname) {
    users.push_back(nickname);
}
