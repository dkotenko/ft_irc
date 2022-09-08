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
