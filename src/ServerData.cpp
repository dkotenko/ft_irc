#include "ServerData.hpp"

Channel *ServerData::addChannel(std::string &channelName) {
    if (channels.count(channelName) == 0) {
        Channel *channel = new Channel(channelName);
        channels[channelName] = channel;
    }
    return channels[channelName];
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
    else {
        std::cout << "NULL\n";
        return NULL;
    }
}

std::string ServerData::getUsernameByFd(int fd) {
    std::map<std::string, User*> :: iterator it;
    for (it=users.begin(); it != users.end(); ++it) {
        if (fd == it->second->fd) {
            return it->second->username;
        }
    }
    return NULL;
}

bool ServerData::checkChannel(std::string channelName) {
    if (channels.count(channelName))
        return true;
    return false;
}

std::string ServerData::doNames(std::vector<std::string> channelsList) {
    std::map<std::string ,Channel*> :: iterator it;
    std::string answer;
    for(it=channels.begin(); it != channels.end(); ++it) {
        //std::cout<<"OPERATOR "<<it->second->getOperatorUsername()<<"\n";
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
        answer += "= ";
        answer += it->first;
        answer += " :";
        //std::cout<<it->first<<": ";
        for (int i = 0; i < users.size(); i++) {
            if (i != 0) {
                //std::cout<<", ";
                answer += " ";
            }
            if (it->second->getOperatorUsername() == users[i]) {
                std::cout << it->second->getOperatorUsername() << " " << users[i] << "\n";
                answer += "@";
            }
            answer += users[i];
            //std::cout<<users[i];
        }
        //std::cout<<std::endl;
    }
    return answer;
}

void ServerData::addUser(FileDescriptor *fileDescriptor) {
    std::string &username = fileDescriptor->userInfo.username;
    if (username == "") {
        return ;
    }

    if (users.count(username) > 0) {
        User *existed = users[username];
        existed->nickname = fileDescriptor->userInfo.nickname;
        existed->realname = fileDescriptor->userInfo.realname;
        existed->hostname = fileDescriptor->userInfo.hostname;
        existed->fd = fileDescriptor->fd;
        log_debug("user %s exists, fd %d set to user", username.c_str(), fileDescriptor->fd);
    } else {
        users[username] = new User(fileDescriptor);
        log_debug("user %s added, users number = %d", username.c_str(), (int)users.size());
    }
}

void ServerData::deleteUser(User *user) {
    users.erase(user->username);
}
