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

void ServerData::addChannel(std::string channelname) {
    if (channels.find(channelname) == channels.end()) {
        Channel *channel = new Channel(channelname);
        channels[channelname] = channel;
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
