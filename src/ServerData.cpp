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

void ServerData::addUser(std::string username, std::string password) {
    User *user = new User(12, 34);
    //TODO сделать проверку на наличие пользователя
    user->username = username;
    user->password = password;
    ServerData::users[username] = user;
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
