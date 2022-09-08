
#include "Channel.hpp"

Channel::Channel(std::string chname) {
    chname = chname;
}

void Channel::addUser(std::string username) {
    users.push_back(username);
}

void Channel::delUser(std::string username) {
    for(int i = 0; i < (int)users.size(); i++) {
        if (users[i] == username) {
            users.erase(users.begin() + i);
        }
    }
}

std::vector<std::string> Channel::getUsers() {
    return users;
}

void Channel::addMessage(std::string from_username, std::vector<std::string> &to_usernames, std::string message) {
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

std::vector<std::string> &Channel::getAllUsers() {
    return users;
}
