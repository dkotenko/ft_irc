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
	handleMap[CMD_QUIT] = &Server::handleQuit;
}

void Server::handleQuit() {
	if (currUser->isRegistered()) {
		close(currUser->fd);
		currUser->clean();
	}
}

void Server::handleNick() {
    if (!currUser->isRegistered()) {
        currUser->connectStatus |= NICK_PASSED;
        currUser->setRegistered(currUser->connectStatus == REGISTERED);
        currUser->nickname = inputMessage->params[0];
        send_welcome(fd);
        return ;
    }
}

void Server::handleUser() {
    //TODO check user logic
    
    if (!currUser->isRegistered()) {
        std::cout << "connect status " << currUser->connectStatus << std::endl;
        currUser->connectStatus |= USER_PASSED;
        currUser->setRegistered(currUser->connectStatus == REGISTERED);
        currUser->username = inputMessage->params[0];
        send_welcome(fd);
        return ;
    }
}

void Server::handlePass() {
    //std::cout << "connect status " << currUser->connectStatus << std::endl;
    std::cout << password << std::endl;
	
	std::cout << "Password required: " << password << std::endl;
	
    if (inputMessage->getParams().size() == 0) {
        handleError(ERR_NEEDMOREPARAMS, "PASS" , "");
		return ;
	}
	std::cout << "Password passed: " << inputMessage->params[0] << std::endl;
    if (currUser->isRegistered()) {
        handleError(ERR_ALREADYREGISTRED, "", "");
		return ;
    }
	if (inputMessage->params[0] != password) {
        handleError(ERR_PASSWDMISMATCH, "", "");
		return ;
	}
	currUser->connectStatus |= PASS_PASSED;
	currUser->setRegistered(currUser->connectStatus == REGISTERED);
	send_welcome(fd);
}

#define WELCOME_REPL "001"

void Server::send_welcome(int i) {
    if (currUser->isRegistered() && !currUser->welcomeReceived) {
        std::stringstream ss;
        ss << WELCOME_REPL << " " << serverName << " :" << SERVER_MESSAGE_OF_THE_DAY;
        std::cout << "Welcome was sent, curr status " << currUser->connectStatus << std::endl;
        outputMessage->add(ss.str());
        outputMessage->fd_to.push_back(i);
        currUser->welcomeReceived = true;
    }
}

void Server::handleJoin() {
    if (!currUser->isRegistered()) {
        return ;
    }
    for (int i = 0; i < (int)inputMessage->params.size(); i++) {
        if (inputMessage->params[i][0] == '#') {
            serverData.addChannel(inputMessage->params[i]);
            serverData.channels[inputMessage->params[0]]->addUser(currUser->username);
            outputMessage->add("372 :Message of the Day");
            outputMessage->fd_to.push_back(fd);
        }
    }
}

void Server::handlePrivMsg() {
   serverData.printAllChannels();
    if (serverData.channels.count(inputMessage->params[0])) {
        serverData.getChannel(inputMessage->params[0])->addMessage
        (
            users[inputMessage->fd_from]->username,
            serverData.getChannel(inputMessage->params[0])->getUsers(),
            inputMessage->params[1]
        );
   }
}

void Server::handleMode() {
    if (!currUser->isRegistered()) {
        return;
    }
    
    if (serverData.getChannel(inputMessage->params[0])->getOperatorUsername() == 
        users[inputMessage->fd_from]->username) {
        return;
    }
}

void Server::handleTopic() {
    if (!currUser->isRegistered()) {
        return;
    }
    
    if (serverData.getChannel(inputMessage->params[0])->getOperatorUsername() == 
        users[inputMessage->fd_from]->username) {
        serverData.getChannel(inputMessage->params[0])->setTopic(inputMessage->params[1]);
    }
}

void Server::handleInvite() {
    if (!currUser->isRegistered()) {
        return;
    }

    if (serverData.getChannel(inputMessage->params[0])->getOperatorUsername() == 
        users[inputMessage->fd_from]->username) {
        serverData.getChannel(inputMessage->params[1])->doInvite(inputMessage->params[0]);
    }
}

void Server::handleKick() {
    if (!currUser->isRegistered()) {
        return;
    }
    
    if (serverData.getChannel(inputMessage->params[1])->getOperatorUsername() == 
        users[inputMessage->fd_from]->username) {
        serverData.getChannel(inputMessage->params[0])->doKick(inputMessage->params[1]);
    }
	
    if (inputMessage->params.size() == 2) {
        if (serverData.checkChannel(inputMessage->params[0]) && 
        serverData.getChannel(inputMessage->params[0])->getOperatorUsername() == users[inputMessage->fd_from]->username) {
            //std::cout<<"1: "<<inputMessage->params[0]<<" 2: "<<inputMessage->params[1]<<"\n";
            serverData.getChannel(inputMessage->params[0])->doKick(inputMessage->params[1]);
        }
    }
}

void Server::handleError(int err, const std::string &arg1, const std::string &arg2)
{
	std::string	msg = ":" + serverName + " ";
	std::stringstream	ss;
	ss << err;
	msg += ss.str() + " " + currUser->getNickname();
	switch (err) {
		case ERR_NOSUCHNICK:
			msg += " " + arg1 + " :No such nick/channel\n";
			break;
		case ERR_NOSUCHSERVER:
			msg += " " + arg1 + " :No such server\n";
			break;
		case ERR_NOSUCHCHANNEL:
			msg += " " + arg1 + " :No such channel\n";
			break;
		case ERR_CANNOTSENDTOCHAN:
			msg += " " + arg1 + " :Cannot send to channel\n";
			break;
		case ERR_TOOMANYCHANNELS:
			msg += " " + arg1 + " :You have joined too many channels\n";
			break;
		case ERR_WASNOSUCHNICK:
			msg += " " + arg1 + " :There was no such nickname\n";
			break;
		case ERR_TOOMANYTARGETS:
			msg += " " + arg1 + " :Duplicate recipients. No arg1 delivered\n";
			break;
		case ERR_NOORIGIN:
			msg += " :No origin specified\n";
			break;
		case ERR_NORECIPIENT:
			msg += " :No recipient given (" + arg1 + ")\n";
			break;
		case ERR_NOTEXTTOSEND:
			msg += " :No text to send\n";
			break;
		case ERR_NOTOPLEVEL:
			msg += " " + arg1 + " :No toplevel domain specified\n";
			break;
		case ERR_WILDTOPLEVEL:
			msg += " " + arg1 + " :Wildcard in toplevel domain\n";
			break;
		case ERR_UNKNOWNCOMMAND:
			msg += " " + arg1 + " :Unknown command\n";
			break;
		case ERR_NOMOTD:
			msg += " :MOTD File is missing\n";
			break;
		case ERR_NOADMININFO:
			msg += " " + arg1 + " :No administrative info available\n";
			break;
		case ERR_FILEERROR:
			msg += " :File error doing \n" + arg1 + " on " + arg2 + "\n";
			break;
		case ERR_NONICKNAMEGIVEN:
			msg += " :No nickname given\n";
			break;
		case ERR_ERRONEUSNICKNAME:
			msg += " " + arg1 + " :Erroneus nickname\n";
			break;
		case ERR_NICKNAMEINUSE:
			msg += " " + arg1 + " :Nickname is already in use\n";
			break;
		case ERR_NICKCOLLISION:
			msg += " " + arg1 + " :Nickname collision KILL\n";
			break;
		case ERR_USERNOTINCHANNEL:
			msg += " " + arg1 + " " + arg2 + " :They aren't on that channel\n";
			break;
		case ERR_NOTONCHANNEL:
			msg += " " + arg1 + " :You're not on that channel\n";
			break;
		case ERR_USERONCHANNEL:
			msg += " " + arg1 + " " + arg2 + " :is already on channel\n";
			break;
		case ERR_NOLOGIN:
			msg += " " + arg1 + " :User not logged in\n";
			break;
		case ERR_SUMMONDISABLED:
			msg += " :SUMMON has been disabled\n";
			break;
		case ERR_USERSDISABLED:
			msg += " :USERS has been disabled\n";
			break;
		case ERR_NOTREGISTERED:
			msg += " :You have not registered\n";
			break;
		case ERR_NEEDMOREPARAMS:
			msg += " " + arg1 + " :Not enough parameters\n";
			break;
		case ERR_ALREADYREGISTRED:
			msg += " :You may not reregister\n";
			break;
		case ERR_NOPERMFORHOST:
			msg += " :Your host isn't among the privileged\n";
			break;
		case ERR_PASSWDMISMATCH:
			msg += " :Password incorrect\n";
			break;
		case ERR_YOUREBANNEDCREEP:
			msg += " :You are banned from this server\n";
			break;
		case ERR_KEYSET:
			msg += " " + arg1 + " :Channel key already set\n";
			break;
		case ERR_CHANNELISFULL:
			msg += " " + arg1 + " :Cannot join channel (+l)\n";
			break;
		case ERR_UNKNOWNMODE:
			msg += " " + arg1 + " :is unknown mode char to me\n";
			break;
		case ERR_INVITEONLYCHAN:
			msg += " " + arg1 + " :Cannot join channel (+i)\n";
			break;
		case ERR_BANNEDFROMCHAN:
			msg += " " + arg1 + " :Cannot join channel (+b)\n";
			break;
		case ERR_BADCHANNELKEY:
			msg += " " + arg1 + " :Cannot join channel (+k)\n";
			break;
		case ERR_NOPRIVILEGES:
			msg += " :Permission Denied- You're not an IRC operator\n";
			break;
		case ERR_CHANOPRIVSNEEDED:
			msg += " " + arg1 + " :You're not channel operator\n";
			break;
		case ERR_CANTKILLSERVER:
			msg += " :You cant kill a server!\n";
			break;
		case ERR_NOOPERHOST:
			msg += " :No O-lines for your host\n";
			break;
		case ERR_UMODEUNKNOWNFLAG:
			msg += " :Unknown MODE flag\n";
			break;
		case ERR_USERSDONTMATCH:
			msg += " :Cant change mode for other users\n";
			break;
		default:
			msg += "UNKNOWN ERROR\n";
			break;
	}
	//send(currUser->fd, msg.c_str(), msg.size(), IRC_NOSIGNAL);
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
    if (!currUser->isRegistered()) {
        return;
    }
    std::vector<std::string> channelsList;
    if (inputMessage->params.size() == 1) {
        channelsList = split(inputMessage->params[0], channelsList, ',');
    }
    outputMessage->data = serverData.doNames(channelsList);
    outputMessage->fd_to.push_back(fd);
}
