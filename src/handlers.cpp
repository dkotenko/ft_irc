#include "server.hpp"

void Server::populateHandleMap() {
    handleMap[CMD_NICK] = &Server::handleNick;
    handleMap[CMD_USER] = &Server::handleUser;
    handleMap[CMD_PASS] = &Server::handlePass;
    handleMap[CMD_JOIN] = &Server::handleJoin;
    handleMap[CMD_PRIVMSG] = &Server::handlePrivMsg;
}

void Server::handleNick() {
    if (!users[fd]->isConnected) {
        users[fd]->connectStatus |= NICK_PASSED;
        users[fd]->isConnected = users[fd]->connectStatus == CONNECTED;
        users[fd]->nickname = &messageInput->params[1];
        send_welcome(fd);
        return ;
    }
}

void Server::handleUser() {
    //TODO check user logic
    
    if (!users[fd]->isConnected) {
        users[fd]->connectStatus |= USER_PASSED;
        users[fd]->isConnected = users[fd]->connectStatus == CONNECTED;
        users[fd]->username = &messageInput->params[0];
        send_welcome(fd);
        return ;
    }
}

void Server::handlePass() {
    //TODO check password logic
    std::cout << messageInput->params[0] << " " << *password << std::endl;
    if (!users[fd]->isConnected && messageInput->params[0] == *password) {
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
    for (int i = 0; i < (int)messageInput->params.size(); i++) {
        if (messageInput->params[i][0] == '#') {
            serverData.addChannel(messageInput->params[i]);
            serverData.channels[messageInput->params[0]]->addUser(users[fd]->username);
            messageOutput->data = "372 :Message of the Day";
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
            &messageInput->params[1]
        );
   }
}