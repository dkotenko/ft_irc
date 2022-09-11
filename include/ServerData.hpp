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
private:

};

#endif /* ****************************************************** SERVERDATA_H */
