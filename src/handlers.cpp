#include "Server.hpp"

void Server::populateHandleMap() {
    handleMap[CMD_NICK] = &Server::handleNick;
    handleMap[CMD_USER] = &Server::handleUser;
    handleMap[CMD_PASS] = &Server::handlePass;
    handleMap[CMD_JOIN] = &Server::handleJoin;
    handleMap[CMD_PRIVMSG] = &Server::handlePrivMsg;
    handleMap[CMD_QUIT] = &Server::handleQuit;

}

void Server::handleQuit() {
    currUser->setConnected(false);
    /*
    delete(currUser);
    users[fd] = new User(FD_FREE, fd);
    */
    //quit
}

void Server::handleNick() {
    if (!currUser->isConnected()) {
        std::cout << "connect status " << currUser->connectStatus << std::endl;
        currUser->connectStatus |= NICK_PASSED;
        currUser->setConnected(currUser->connectStatus == CONNECTED);
        currUser->nickname = &messageInput->params[1];
        send_welcome(fd);
        return ;
    }
}

void Server::handleUser() {
    //TODO check user logic
    
    if (!currUser->isConnected()) {
        std::cout << "connect status " << currUser->connectStatus << std::endl;
        currUser->connectStatus |= USER_PASSED;
        currUser->setConnected(currUser->connectStatus == CONNECTED);
        currUser->username = &messageInput->params[0];
        send_welcome(fd);
        return ;
    }
}

void Server::handlePass() {
    //TODO check password logic
    std::cout << "connect status " << currUser->connectStatus << std::endl;
    std::cout << "Password passed: " << messageInput->params[0] << std::endl << "Password required: " << *password << std::endl;
    if (!currUser->isConnected() && messageInput->params[0] == *password) {
        currUser->connectStatus |= PASS_PASSED;
        currUser->setConnected(currUser->connectStatus == CONNECTED);
        send_welcome(fd);
        return ;
    }
}

#define WELCOME_REPL "001"

void Server::send_welcome(int i) {
    if (currUser->isConnected() && !currUser->welcomeReceived) {
        std::stringstream ss;
        ss << WELCOME_REPL << " " << serverName << " :" << SERVER_MESSAGE_OF_THE_DAY;
        std::cout << "Welcome was sent, curr status " << currUser->connectStatus << std::endl;
        messageOutput->add(ss.str());
        messageOutput->fd_to.push_back(i);
        currUser->welcomeReceived = true;
    }
}

void Server::handleJoin() {
    if (!currUser->isConnected()) {
        return ;
    }
    for (int i = 0; i < (int)messageInput->params.size(); i++) {
        if (messageInput->params[i][0] == '#') {
            serverData.addChannel(messageInput->params[i]);
            serverData.channels[messageInput->params[0]]->addUser(currUser->username);
            messageOutput->add("372 :Message of the Day");
            messageOutput->fd_to.push_back(fd);
        }
    }
}

void Server::handlePrivMsg() {
   serverData.printAllChannels();
    if (serverData.channels.count(messageInput->params[0])) {
        serverData.getChannel(messageInput->params[0])->addMessage
        (
            *users[messageInput->fd_from]->username,
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
        *users[messageInput->fd_from]->username) {
        return;
    }
}

void Server::handleTopic() {
    if (!users[fd]->isConnected) {
        return;
    }
    
    if (serverData.getChannel(messageInput->params[0])->getOperatorUsername() == 
        *users[messageInput->fd_from]->username) {
        serverData.getChannel(messageInput->params[0])->setTopic(messageInput->params[1]);
    }
}

void Server::handleInvite() {
    if (!users[fd]->isConnected) {
        return;
    }

    if (serverData.getChannel(messageInput->params[0])->getOperatorUsername() == 
        *users[messageInput->fd_from]->username) {
        serverData.getChannel(messageInput->params[1])->doInvite(messageInput->params[0]);
    }
}

void Server::handleKick() {
    if (!users[fd]->isConnected) {
        return;
    }
    
    if (serverData.getChannel(messageInput->params[1])->getOperatorUsername() == 
        *users[messageInput->fd_from]->username) {
        serverData.getChannel(messageInput->params[0])->doKick(messageInput->params[1]);
    } 
}