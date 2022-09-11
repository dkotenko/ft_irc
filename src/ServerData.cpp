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
    if (channels.find(channelName) == channels.end()) {
        Channel *channel = new Channel(channelName);
        channels[channelName] = channel;
    }
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/



/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/
