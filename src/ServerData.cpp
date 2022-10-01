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



void ServerData::doNames(std::vector<std::string> channelsList) {
    std::map<std::string ,Channel*> :: iterator it;
    for(it=channels.begin(); it != channels.end(); ++it) {
        std::cout<<"OPERATOR "<<it->second->getOperatorUsername()<<"\n";
        if (channelsList.size() != 0) {
            bool checkChannel = false;
            for(int i = 0; i < channels.size(); i++) {
                if (channelsList[i] == it->first)
                    checkChannel = true;
            }
            if (!checkChannel)
                continue;
        }
        std::vector<std::string> users = it->second->getUsers();
        std::cout<<it->first<<": ";
        for (int i = 0; i < users.size(); i++) {
            if (i != 0)
                std::cout<<", ";
            std::cout<<users[i];
        }
        std::cout<<std::endl;
    }
}
