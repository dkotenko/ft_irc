#include "Server.hpp"

void Server::populateHandleMap() {
    handleMap[CMD_NICK] = &Server::handleNick;
    handleMap[CMD_USER] = &Server::handleUser;
    handleMap[CMD_PASS] = &Server::handlePass;
    handleMap[CMD_JOIN] = &Server::handleJoin;
    handleMap[CMD_PRIVMSG] = &Server::handlePrivMsg;
	handleMap[CMD_NOTICE] = &Server::handleNotice;
    handleMap[CMD_MODE] = &Server::handleMode;
    handleMap[CMD_TOPIC] = &Server::handleTopic;
    handleMap[CMD_INVITE] = &Server::handleInvite;
    handleMap[CMD_KICK] = &Server::handleKick;
    handleMap[CMD_NAMES] = &Server::handleNames;
	handleMap[CMD_QUIT] = &Server::handleQuit;
	handleMap[CMD_PONG] = &Server::handlePong;
}

void Server::handlePong() {
	;
}

void Server::doQuit(User *user) {
	if (user->isRegistered()) {
		serverData.deleteUser(user);
		close(user->fd);
		user->clean();
	}
}

void Server::handleQuit() {
	doQuit(currUser);
}

void Server::registerNewUser(User *user) {
	serverData.addUser(user);
	sendWelcome(user->fd);
}

void Server::handleNick() {
    if (!currUser->isRegistered()) {
        currUser->connectStatus |= NICK_PASSED;
        currUser->setRegistered(currUser->connectStatus == REGISTERED);
        currUser->nickname = inputMessage->getParams()[0];
		registerNewUser(currUser);
        return ;
    }
}

void Server::handleUser() {
    //TODO check user logic
    /*
	std::cout<<"SIZE "<<inputMessage->params.size()<<" "
		<<inputMessage->params[0]<<" "<<inputMessage->params[1]<<" "
		<<inputMessage->params[2]<<" "<< inputMessage->params[3]<<"";
	*/
	if (!currUser->isRegistered() && inputMessage->params.size() >= 4 
		&& inputMessage->params[3][0] == ':' && inputMessage->params[3].size() > 1) {
        std::cout << "connect status " << currUser->connectStatus << std::endl;
        currUser->connectStatus |= USER_PASSED;
        currUser->setRegistered(currUser->connectStatus == REGISTERED);
        currUser->username = inputMessage->params[0];
		currUser->hostname = inputMessage->params[1];
		currUser->hostname = inputMessage->params[2];
		for (int i = 3; i < inputMessage->params.size(); ++i) {
			if (i == 3) {
				currUser->realusername += inputMessage->params[i];
				currUser->realusername.erase(1, currUser->realusername.size() - 1);
			}
			else {
				currUser->realusername += " ";
				currUser->realusername += inputMessage->params[i];
			}
		}
		//std::cout<<"REAL: "<<currUser->realusername<<"";
		registerNewUser(currUser);
    }
	else
		handleError(ERR_NEEDMOREPARAMS, "USER", "");
		return ;
}

void Server::handlePass() {
    //std::cout << "connect status " << currUser->connectStatus << std::endl;
    std::cout << password << std::endl;
	
	std::cout << "Password required: " << password << std::endl;
	
	if (currUser->isRegistered()) {
        handleError(ERR_ALREADYREGISTRED, "", "");
		return ;
    }

    if (inputMessage->getParams().size() == 0) {
        handleError(ERR_NEEDMOREPARAMS, "PASS" , "");
		return ;
	}
	std::string passed = inputMessage->getParams()[0];
	if (passed[0] == ':') {
		passed = passed.substr(1, passed.size() - 1);
	}

	std::cout << "Password passed: " << passed << std::endl;
    
	
	if (passed != password) {
        handleError(ERR_PASSWDMISMATCH, "", "");
		return ;
	}
	currUser->connectStatus |= PASS_PASSED;
	currUser->setRegistered(currUser->connectStatus == REGISTERED);
	registerNewUser(currUser);
}

#define WELCOME_REPL "001"

void Server::sendWelcome(int i) {
    if (currUser->isRegistered() && !currUser->welcomeReceived) {
		
		//:FT_IRC 375 pidgin
		outputMessage->add(std::string("FT_IRC Message of the day"), RPL_MOTDSTART);
        
		//toAdd += ":FT_IRC 372 pidgin ";
		outputMessage->add(SERVER_MESSAGE_OF_THE_DAY, RPL_MOTD);

		//:FT_IRC 376 pidgin 
        outputMessage->add(std::string(":End of /MOTD command"), RPL_ENDOFMOTD);

		std::cout << "Welcome was sent, curr status \n" << currUser->connectStatus << std::endl;
        outputMessage->fd_to.push_back(i);
        currUser->welcomeReceived = true;
    }
}

void Server::handleJoin() {
    if (!currUser->isRegistered()) {
        return ;
    }
    for (int i = 0; i < (int)inputMessage->getParams().size(); i++) {
        if (inputMessage->getParams()[i][0] == '#') {
            serverData.addChannel(inputMessage->getParams()[i]);
            serverData.channels[inputMessage->getParams()[0]]->addUser(currUser->username);
            outputMessage->add("Message of the Day", RPL_MOTD);
            outputMessage->fd_to.push_back(fd);
        }
    }
}

void Server::handlePrivMsg() {
    //serverData.printAllChannels();
	if (inputMessage->getParams().size() < 2)
		return;
	if (inputMessage->getParams()[0][0] == '#') {
		if (serverData.channels.count(inputMessage->getParams()[0])) {
			std::string message;
			for (int i = 1; i < inputMessage->getParams().size(); i++) {
				if (i != 1) 
					message += " ";
				message += inputMessage->getParams()[i];
			}
	        serverData.getChannel(inputMessage->getParams()[0])->addMessage
			(
				users[inputMessage->fd_from]->username,
				serverData.getChannel(inputMessage->getParams()[0])->getUsers(),
				message
			);
   		}
		else {
			handleError(ERR_CANNOTSENDTOCHAN, "", "");
		}
	}
	else {
		if (serverData.users.count(inputMessage->getParams()[0])) {
			std::string message;
			outputMessage->fd_to.push_back(serverData.users[inputMessage->getParams()[0]]->fd);
			for (int i = 1; i < inputMessage->getParams().size(); i++) {
				if (i != 1) 
					message += " ";
				message += inputMessage->getParams()[i];
			}
			outputMessage->data = message;
		}
		else {
			handleError(ERR_NOSUCHNICK, inputMessage->getParams()[0], "");
		}
	}
}

void Server::handleNotice() {
	//serverData.printAllChannels();
    if (serverData.channels.count(inputMessage->getParams()[0])) {
        serverData.getChannel(inputMessage->getParams()[0])->addMessage
        (
            users[inputMessage->fd_from]->username,
            serverData.getChannel(inputMessage->getParams()[0])->getUsers(),
            inputMessage->getParams()[1]
        );
   }
}

void Server::handleMode() {
    if (!currUser->isRegistered()) {
        return;
    }
    
    if (serverData.getChannel(inputMessage->getParams()[0])->getOperatorUsername() == 
        users[inputMessage->fd_from]->username) {
        return;
    }
}

void Server::handleTopic() {
    if (!currUser->isRegistered()) {
        return;
    }
    
    if (serverData.getChannel(inputMessage->getParams()[0])->getOperatorUsername() == 
        users[inputMessage->fd_from]->username) {
        serverData.getChannel(inputMessage->getParams()[0])->setTopic(inputMessage->getParams()[1]);
    }
}

void Server::handleInvite() {
    if (!currUser->isRegistered()) {
        return;
    }

    if (serverData.getChannel(inputMessage->getParams()[0])->getOperatorUsername() == 
        users[inputMessage->fd_from]->username) {
        serverData.getChannel(inputMessage->getParams()[1])->doInvite(inputMessage->getParams()[0]);
    }
}

void Server::handleKick() {
    if (!currUser->isRegistered()) {
        return;
    }
    
    if (serverData.getChannel(inputMessage->getParams()[1])->getOperatorUsername() == 
        users[inputMessage->fd_from]->username) {
        serverData.getChannel(inputMessage->getParams()[0])->doKick(inputMessage->getParams()[1]);
    }
	
    if (inputMessage->getParams().size() == 2) {
        if (serverData.checkChannel(inputMessage->getParams()[0]) && 
        serverData.getChannel(inputMessage->getParams()[0])->getOperatorUsername() == users[inputMessage->fd_from]->username) {
            //std::cout<<"1: "<<inputMessage->getParams[0]<<" 2: "<<inputMessage->getParams[1]<<"";
            serverData.getChannel(inputMessage->getParams()[0])->doKick(inputMessage->getParams()[1]);
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
			msg += " " + arg1 + " :No such nick/channel";
			break;
		case ERR_NOSUCHSERVER:
			msg += " " + arg1 + " :No such server";
			break;
		case ERR_NOSUCHCHANNEL:
			msg += " " + arg1 + " :No such channel";
			break;
		case ERR_CANNOTSENDTOCHAN:
			msg += " " + arg1 + " :Cannot send to channel";
			break;
		case ERR_TOOMANYCHANNELS:
			msg += " " + arg1 + " :You have joined too many channels";
			break;
		case ERR_WASNOSUCHNICK:
			msg += " " + arg1 + " :There was no such nickname";
			break;
		case ERR_TOOMANYTARGETS:
			msg += " " + arg1 + " :Duplicate recipients. No arg1 delivered";
			break;
		case ERR_NOORIGIN:
			msg += " :No origin specified";
			break;
		case ERR_NORECIPIENT:
			msg += " :No recipient given (" + arg1 + ")";
			break;
		case ERR_NOTEXTTOSEND:
			msg += " :No text to send";
			break;
		case ERR_NOTOPLEVEL:
			msg += " " + arg1 + " :No toplevel domain specified";
			break;
		case ERR_WILDTOPLEVEL:
			msg += " " + arg1 + " :Wildcard in toplevel domain";
			break;
		case ERR_UNKNOWNCOMMAND:
			msg += " " + arg1 + " :Unknown command";
			break;
		case ERR_NOMOTD:
			msg += " :MOTD File is missing";
			break;
		case ERR_NOADMININFO:
			msg += " " + arg1 + " :No administrative info available";
			break;
		case ERR_FILEERROR:
			msg += " :File error doing " + arg1 + " on " + arg2 + "";
			break;
		case ERR_NONICKNAMEGIVEN:
			msg += " :No nickname given";
			break;
		case ERR_ERRONEUSNICKNAME:
			msg += " " + arg1 + " :Erroneus nickname";
			break;
		case ERR_NICKNAMEINUSE:
			msg += " " + arg1 + " :Nickname is already in use";
			break;
		case ERR_NICKCOLLISION:
			msg += " " + arg1 + " :Nickname collision KILL";
			break;
		case ERR_USERNOTINCHANNEL:
			msg += " " + arg1 + " " + arg2 + " :They aren't on that channel";
			break;
		case ERR_NOTONCHANNEL:
			msg += " " + arg1 + " :You're not on that channel";
			break;
		case ERR_USERONCHANNEL:
			msg += " " + arg1 + " " + arg2 + " :is already on channel";
			break;
		case ERR_NOLOGIN:
			msg += " " + arg1 + " :User not logged in";
			break;
		case ERR_SUMMONDISABLED:
			msg += " :SUMMON has been disabled";
			break;
		case ERR_USERSDISABLED:
			msg += " :USERS has been disabled";
			break;
		case ERR_NOTREGISTERED:
			msg += " :You have not registered";
			break;
		case ERR_NEEDMOREPARAMS:
			msg += " " + arg1 + " :Not enough parameters";
			break;
		case ERR_ALREADYREGISTRED:
			msg += " :You may not reregister";
			break;
		case ERR_NOPERMFORHOST:
			msg += " :Your host isn't among the privileged";
			break;
		case ERR_PASSWDMISMATCH:
			msg += " :Password incorrect";
			break;
		case ERR_YOUREBANNEDCREEP:
			msg += " :You are banned from this server";
			break;
		case ERR_KEYSET:
			msg += " " + arg1 + " :Channel key already set";
			break;
		case ERR_CHANNELISFULL:
			msg += " " + arg1 + " :Cannot join channel (+l)";
			break;
		case ERR_UNKNOWNMODE:
			msg += " " + arg1 + " :is unknown mode char to me";
			break;
		case ERR_INVITEONLYCHAN:
			msg += " " + arg1 + " :Cannot join channel (+i)";
			break;
		case ERR_BANNEDFROMCHAN:
			msg += " " + arg1 + " :Cannot join channel (+b)";
			break;
		case ERR_BADCHANNELKEY:
			msg += " " + arg1 + " :Cannot join channel (+k)";
			break;
		case ERR_NOPRIVILEGES:
			msg += " :Permission Denied- You're not an IRC operator";
			break;
		case ERR_CHANOPRIVSNEEDED:
			msg += " " + arg1 + " :You're not channel operator";
			break;
		case ERR_CANTKILLSERVER:
			msg += " :You cant kill a server!";
			break;
		case ERR_NOOPERHOST:
			msg += " :No O-lines for your host";
			break;
		case ERR_UMODEUNKNOWNFLAG:
			msg += " :Unknown MODE flag";
			break;
		case ERR_USERSDONTMATCH:
			msg += " :Cant change mode for other users";
			break;
		default:
			msg += "UNKNOWN ERROR";
			break;
	}
	outputMessage->add(msg, fd);
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
    if (inputMessage->getParams().size() == 1) {
        channelsList = split(inputMessage->getParams()[0], channelsList, ',');
    }
	/*
    outputMessage->data = ;
    outputMessage->fd_to.push_back(fd);
	*/
	outputMessage->add(serverData.doNames(channelsList), fd);
}
