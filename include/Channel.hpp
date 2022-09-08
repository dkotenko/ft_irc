#ifndef CHANEL_H
#define CHANEL_H

#include <vector>
#include <string>
#include <map>

struct endmessage{
    std::string from_m;
    std::vector<std::string> to_m;
    std::string datamessage;
};

class Channel {
    public:
        Channel(std::string chname);
        void addUser(std::string username);
        void delUser(std::string username);
        void addMessage(std::string from_username, std::vector<std::string> to_usernames, std::string message);
        std::vector<std::string> getUsers();
        endmessage getMessage();
        std::vector<std::string> getAllUsers();
    private:
        std::vector<std::string> users;
        std::vector<endmessage> messages;
        std::string chname;
};

#endif //CHANEL_H
