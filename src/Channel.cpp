
#include "Channel.hpp"



Channel::Channel(std::string chname) {
    Channel::chname = chname;
}

void Channel::addUser(std::string username) {
    Channel::users.push_back(username);
}

void Channel::delUser(std::string username) {
    for(int i = 0; i < (int)Channel::users.size(); i++) {
        if (Channel::users[i] == username) {
            Channel::users.erase(Channel::users.begin() + i);
        }
    }
}

std::vector<std::string> Channel::getUsers() {
    return Channel::users;
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
    res = Channel::messages.back();
    //Channel::messages.pop_back();
    return res;
}

std::vector<std::string> Channel::getAllUsers() {
    return users;
}
