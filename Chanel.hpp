#ifndef CHANEL_H
#define CHANEL_H

#include <vector>

struct endmessage{
    std::string from_m;
    std::vector<std::string> to_m;
    std::string datamessage;
};

class Chanel {
    public:
        Chanel(std::string chname);
        void addUser(std::string username);
        void delUser(std::string username);
        void addMessage(std::string from_username, std::vector<std::string> to_usernames, std::string message);
        endmessage getMessage();
    private:
        std::vector<std::string> users;
        std::vector<endmessage> messages;
        std::string chname;
};

#endif //CHANEL_H