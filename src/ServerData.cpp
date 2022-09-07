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

void ServerData::addChanel(std::string chanelname) {
    if (chanels.find(chanelname) == chanels.end()) {
        Chanel *chanel = new Chanel(chanelname);
        chanels[chanelname] = chanel;
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
