#include "ServerData.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerData::ServerData()
{

}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ServerData::~ServerData()
{
}

void ServerData::addChannel(std::string channelName) {
    if (channels.count(channelName) == 0) {
        Channel *channel = new Channel(channelName);
        channels[channelName] = channel;
    }
}

void ServerData::printAllChannels() {
    std::map<std::string ,Channel*> :: iterator it;
    for(it=channels.begin(); it != channels.end(); ++it) {
        std::cout << it->first << std::endl;
    }
}

Channel *ServerData::getChannel(std::string channelName) {
    if (channels.count(channelName)) {
        return channels[channelName];
    }
    else
        return NULL;
}
