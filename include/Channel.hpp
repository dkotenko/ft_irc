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
        std::vector<std::string > const &getUsers() const;
        endmessage getMessage();
        void doKick(std::string username);
        void setTopic(std::string topic);
        std::string getTopic();
        void editMode();
        void doInvite(std::string nickname);
        bool checkUserInChannel(std::string username);

        std::string getOperatorUsername();

    private:
        std::vector<std::string> users;
        std::vector<endmessage> messages;
        std::string chname;
        std::string operator_username;
        std::string topic;
        std::vector<std::string> parameters;
};

#endif //CHANEL_H