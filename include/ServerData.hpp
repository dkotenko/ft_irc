#ifndef SERVERDATA_HPP
# define SERVERDATA_HPP

# include <iostream>
# include <string>
#include "map"
#include "User.hpp"
#include "Channel.hpp"

class ServerData
{

	public:
        void addChannel(std::string channelName);
        std::map<std::string, User*> users;
        std::map<std::string, Channel*> channels;
        void printAllChannels();
        Channel* getChannel(std::string channelName);
        std::string doNames(std::vector<std::string> channelsList);
        bool checkChannel(std::string channelName);
        void addUser(User *user);
        void deleteUser(User *user);
        std::string getUsernameByFd(int fd);
    private:

};

#endif /* ****************************************************** SERVERDATA_H */
