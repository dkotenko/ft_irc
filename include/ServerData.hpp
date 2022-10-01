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
		ServerData();
        ~ServerData();
        
        void addChannel(std::string channelName);
        std::map<std::string, User*> users;
        std::map<std::string, Channel*> channels;
        void printAllChannels();
        Channel* getChannel(std::string channelName);
        void doNames(std::vector<std::string> channelsList);
private:

};

#endif /* ****************************************************** SERVERDATA_H */
