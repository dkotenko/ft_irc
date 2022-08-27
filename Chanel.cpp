
#include "Chanel.hpp"
#include <string>
#include <map>


Chanel::Chanel(std::string chname) {
    this->chname = chname;
    Chanel::users = {};
    Chanel::messages = {};
}

void Chanel::addUser(std::string username) {
    Chanel::users.push_back(username);
}

void Chanel::delUser(std::string username) {
    for(int i = 0; i < Chanel::users.size(); i++) {
        if (Chanel::users[i] == username) {
            Chanel::users.erase(Chanel::users.begin() + i);
        }
    }
}

void Chanel::addMessage(std::string from_username, std::vector<std::string> to_usernames, std::string message) {
    endmessage new_message;
    new_message.from_m = from_username;
    new_message.to_m = to_usernames;
    new_message.datamessage = message;
    
    Chanel::messages.push_back(new_message);
}


endmessage Chanel::getMessage() {
    endmessage res;
    res = Chanel::messages.back();
    //Chanel::messages.pop_back();
    return res;
}