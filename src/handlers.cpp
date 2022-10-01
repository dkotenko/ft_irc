#include "Server.hpp"

void Server::populateHandleMap() {
    handleMap[CMD_NICK] = &Server::handleNick;
    handleMap[CMD_USER] = &Server::handleUser;
    handleMap[CMD_PASS] = &Server::handlePass;
    handleMap[CMD_JOIN] = &Server::handleJoin;
    handleMap[CMD_PRIVMSG] = &Server::handlePrivMsg;
    handleMap[CMD_MODE] = &Server::handleMode;
    handleMap[CMD_TOPIC] = &Server::handleTopic;
    handleMap[CMD_INVITE] = &Server::handleInvite;
    handleMap[CMD_KICK] = &Server::handleKick;
    handleMap[CMD_NAMES] = &Server::handleNames;
}

void Server::handleNick() {
    if (!users[fd]->isConnected) {
        users[fd]->connectStatus |= NICK_PASSED;
        users[fd]->isConnected = users[fd]->connectStatus == CONNECTED;
        users[fd]->nickname = messageInput->params[0];
        send_welcome(fd);
        return ;
    }
}

void Server::handleUser() {
    //TODO check user logic
    
    if (!users[fd]->isConnected) {
        users[fd]->connectStatus |= USER_PASSED;
        users[fd]->isConnected = users[fd]->connectStatus == CONNECTED;
        users[fd]->username = messageInput->params[0];
        send_welcome(fd);
        return ;
    }
}

void Server::handlePass() {
    //TODO check password logic
    std::cout << messageInput->params[0] << " " << password << std::endl;
    if (!users[fd]->isConnected && messageInput->params[0] == password) {
        users[fd]->connectStatus |= PASS_PASSED;
        users[fd]->isConnected = users[fd]->connectStatus == CONNECTED;
        send_welcome(fd);
        return ;
    }
}

void Server::send_welcome(int i) {
    if (users[i]->isConnected && !users[i]->welcomeReceived) {
        messageOutput->data = SERVER_MESSAGE_OF_THE_DAY;
        messageOutput->fd_to.push_back(i);
        users[i]->welcomeReceived = true;
    }
}

void Server::handleJoin() {
    if (!users[fd]->isConnected) {
        return;
    }
    for (int i = 0; i < (int)messageInput->params.size(); i++) {
        if (messageInput->params[i][0] == '#') {
            serverData.addChannel(messageInput->params[i]);
            serverData.channels[messageInput->params[0]]->addUser(users[fd]->username);
            messageOutput->data = "372 :Message of the Day\n";
            messageOutput->fd_to.push_back(fd);
        }
    }
}

void Server::handlePrivMsg() {
   serverData.printAllChannels();
    if (serverData.channels.count(messageInput->params[0])) {
        serverData.getChannel(messageInput->params[0])->addMessage
        (

            users[messageInput->fd_from]->username,
            serverData.getChannel(messageInput->params[0])->getUsers(),
            messageInput->params[1]
        );
   }
}

void Server::handleMode() {
    if (!users[fd]->isConnected) {
        return;
    }
    
    if (serverData.getChannel(messageInput->params[0])->getOperatorUsername() == 
        users[messageInput->fd_from]->username) {
        return;
    }
}

void Server::handleTopic() {
    if (!users[fd]->isConnected) {
        return;
    }
    
    if (serverData.getChannel(messageInput->params[0])->getOperatorUsername() == 
        users[messageInput->fd_from]->username) {
        serverData.getChannel(messageInput->params[0])->setTopic(messageInput->params[1]);
    }
}

void Server::handleInvite() {
    if (!users[fd]->isConnected) {
        return;
    }

    if (serverData.getChannel(messageInput->params[0])->getOperatorUsername() == 
        users[messageInput->fd_from]->username) {
        serverData.getChannel(messageInput->params[1])->doInvite(messageInput->params[0]);
    }
}

void Server::handleKick() {
    if (!users[fd]->isConnected) {
        return;
    }
    if (messageInput->params.size() == 2) {
        if (serverData.getChannel(messageInput->params[0])->getOperatorUsername() == 
            users[messageInput->fd_from]->username) {
            //std::cout<<"1: "<<messageInput->params[0]<<" 2: "<<messageInput->params[1]<<"\n";
            serverData.getChannel(messageInput->params[0])->doKick(messageInput->params[1]);
        }
    }
}

std::vector<std::string> split(const std::string& s, std::vector<std::string>& res, char delim) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        res.push_back(item);
    }
    return res;
}

void Server::handleNames() {
    if (!users[fd]->isConnected) {
        return;
    }
    std::vector<std::string> channelsList;
    if (messageInput->params.size() == 1) {
        channelsList = split(messageInput->params[0], channelsList, ',');
    }
    serverData.doNames(channelsList);
}