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
	handleMap[CMD_LIST] = &Server::handleList;
	handleMap[CMD_WHOIS] = &Server::handleWhoIs;
	handleMap[CMD_AWAY] = &Server::handleAway;
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

void Server::registerNewUser(FileDescriptor *FileDescriptor) {
	serverData.addUser(user);
	sendWelcome();
}

void Server::handleNick() {
    if (!currFd->isRegistered()) {
        currFd->connectStatus |= NICK_PASSED;
        currFd->setRegistered(currFd->connectStatus == REGISTERED);
        currUser->nickname = inputMessage->getParams()[0];
		registerNewUser(currFd);
        return ;
    }
}

std::string Server::connectStatusAsString(int status) {
	std::stringstream ss;

	int checkPass = (status & ( 1 << PASS_PASSED )) >> PASS_PASSED;
	if (checkPass) {
		ss << "| Password passed |";
	}

	int checkNickName = (status & ( 1 << NICK_PASSED )) >> NICK_PASSED;
	if (checkNickName) {
		ss << "| Nickname passed |";
	}

	int checkUserName = (status & ( 1 << USER_PASSED )) >> USER_PASSED;
	if (checkUserName) {
		ss << "| Username passed |";
	}
	return ss.str();
}

void Server::handleUser() {
	if (!currFd->isRegistered() && inputMessage->params.size() >= 4 
		&& inputMessage->params[3][0] == ':' && inputMessage->params[3].size() > 1) {
        currFd->setRegistered(currUser->connectStatus == REGISTERED);
        currFd->userInfo.username = inputMessage->params[0];
		currFd->userInfo.hostname = inputMessage->params[1];
		currFd->userInfo.servername = inputMessage->params[2];
		for (int i = 3; i < inputMessage->params.size(); ++i) {
			if (i == 3) {
				currFd->userInfo.realname += inputMessage->params[i];
				currFd->userInfo.realname.erase(1, currFd->userInfo.realname.size() - 1);
			}
			else {
				currFd->userInfo.realname += " ";
				currFd->userInfo.realname += inputMessage->params[i];
			}
		}
		currFd->connectStatus |= USER_PASSED;
		log_debug("connect status: %s", connectStatusAsString(currFd->connectStatus).c_str());
		registerNewUser(currFd);
    }
	else
		handleError(ERR_NEEDMOREPARAMS, "USER", "");
		return ;
}

void Server::handlePass() {
	
	if (currFd->isRegistered()) {
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
	
	if (passed != password) {
        handleError(ERR_PASSWDMISMATCH, "", "");
		return ;
	}
	currFd->connectStatus |= PASS_PASSED;
	currFd->setRegistered(currUser->connectStatus == REGISTERED);
	registerNewUser(currFd);
}

#define WELCOME_REPL "001"

void Server::sendWelcome() {
    if (currFd->isRegistered() && !currUser->welcomeReceived) {
		
		//:FT_IRC 375 pidgin
		currUser->outputMessage.add(std::string("FT_IRC Message of the day"), RPL_MOTDSTART);
        
		//toAdd += ":FT_IRC 372 pidgin ";
		currUser->outputMessage.add(SERVER_MESSAGE_OF_THE_DAY, RPL_MOTD);

		//:FT_IRC 376 pidgin 
        currUser->outputMessage.add(std::string(":End of /MOTD command"), RPL_ENDOFMOTD);
        currUser->welcomeReceived = true;
    }
}

//:pidgin!pidgin@127.0.0.1 JOIN :#123
void Server::handleJoin() {
    if (!currUser->isRegistered()) {
		log_debug("User %s is not registered", currUser->username.c_str());
        return ;
    }
	for (int i = 0; i < (int)inputMessage->getParams().size(); i++) {
		if (inputMessage->getParams()[i][0] == '#') {
			std::string &channelName = inputMessage->getParams()[i];
			Channel *channel = serverData.addChannel(channelName);
			if (!channel->containsUser(currUser->username)) {
				channel->addUser(currUser->username);
				std::string res = inputMessage->getParams()[i];
				res += " :";
				if (channel->getTopic() == "") {
					outputMessage->add(res, RPL_NOTOPIC);
				}
				else {
					res += channel->getTopic();
					outputMessage->add(res, RPL_TOPIC);
				}
				handleNames();
			}
		} else {
			handleError(ERR_NOSUCHCHANNEL, "", "");
		}
	}
}

void Server::handlePrivMsg() {
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
			for (int i = 1; i < inputMessage->getParams().size(); i++) {
				if (i != 1) 
					message += " ";
				message += inputMessage->getParams()[i];
			}
			//outputMessage->data = message;
			//outputMessage->add(message, 0);
			//TODO как правильно отправлять сообщения??? Не приходят сообщения клиенту
			outputMessage->addPrivMsg(message, inputMessage->fd_from, serverData.getUsernameByFd(inputMessage->fd_from), "127.0.0.1", inputMessage->getParams()[0]);
		}
		else {
			handleError(ERR_NOSUCHNICK, inputMessage->getParams()[0], "");
		}
	}

	if (serverData.users[users[inputMessage->fd_from]->username]->getAwayStatus()) {
		std::string msg;
		msg += inputMessage->getParams()[0];
		msg += " :";
		msg += serverData.users[inputMessage->getParams()[0]]->getAwayText();
		//TODO дописать отправку msg
	}

}

void Server::handleNotice() {
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
        currUser->username) {
        serverData.getChannel(inputMessage->getParams()[0])->setTopic(inputMessage->getParams()[1]);
    }
}

void Server::handleInvite() {
    if (!currUser->isRegistered()) {
        return;
    }

    if (serverData.getChannel(inputMessage->getParams()[0])->getOperatorUsername() == 
        currUser->username) {
        serverData.getChannel(inputMessage->getParams()[1])->inviteUser(inputMessage->getParams()[0]);
    }
}

void Server::handleKick() {
    if (!currUser->isRegistered()) {
        return;
    }
    
    if (serverData.getChannel(inputMessage->getParams()[1])->getOperatorUsername() == 
        users[inputMessage->fd_from]->username) {
        serverData.getChannel(inputMessage->getParams()[0])->kickUser(inputMessage->getParams()[1]);
    }
	
    if (inputMessage->getParams().size() == 2) {
        if (serverData.checkChannel(inputMessage->getParams()[0]) && 
        serverData.getChannel(inputMessage->getParams()[0])->getOperatorUsername() == users[inputMessage->fd_from]->username) {
            serverData.getChannel(inputMessage->getParams()[0])->kickUser(inputMessage->getParams()[1]);
        }
    }
}

void Server::handleError(int err, const std::string &arg1, const std::string &arg2)
{
	std::string	msg = ":" + servername + " ";
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
	outputMessage->add(msg, RPL_NONE, fd);
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> res;
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
        channelsList = split(inputMessage->getParams()[0], ',');
    }
	outputMessage->add(serverData.doNames(channelsList), RPL_NAMREPLY, fd);
	std::string res;
	if (channelsList.size() > 0) {
		res += channelsList[0] + " ";
	}
	res += ":End of /NAMES list";
	outputMessage->add(res, RPL_ENDOFNAMES, fd);
}

void Server::handleList() {
	if (!currUser->isRegistered()) {
        return;
    }
	if (inputMessage->getParamsSize() == 0) {
		std::map<std::string ,Channel*> :: iterator it;
		outputMessage->add(std::string("Channel :Users Name"), RPL_LISTSTART, fd);
		
    	for(it=serverData.channels.begin(); it != serverData.channels.end(); ++it) {
			std::string res = it->first + " ";

			res += SSTR(it->second->getUsers().size()) + " ";
			res += it->second->getTopic();
			res += " :[+n]";
			outputMessage->add(res, RPL_LIST, fd);
    	}
		outputMessage->add(std::string(":End of /LIST"), RPL_LISTEND);
	}
	else if (inputMessage->getParamsSize() == 1) {
		std::vector<std::string> channelsVector = split(inputMessage->params[0], ',');
		outputMessage->add(std::string("Channel :Users Name"), RPL_LISTSTART, fd);
		for (int i = 0; i < channelsVector.size(); ++i) {
			if (serverData.checkChannel(channelsVector[i])) {
				std::string res = channelsVector[i] + " ";
				std::vector<std::string> usersVector = serverData.channels[channelsVector[i]]->getUsers();
				for (int j = 0; j < usersVector.size(); ++j) {
					if (j != 0)
						res += " ";
					if (usersVector[j] == serverData.channels[channelsVector[i]]->getOperatorUsername()) {
						res += "@";
					}
					res += usersVector[j];
				}
				res += " :[+n]";
				outputMessage->add(res, RPL_LIST, fd);
			}
		}
		outputMessage->add(std::string(":End of /LIST"), RPL_LISTEND, fd);
	}
}

bool match(const char *pattern, const char *candidate, int p, int c) {
  if (pattern[p] == '\0') {
    return candidate[c] == '\0';
  } else if (pattern[p] == '*') {
    for (; candidate[c] != '\0'; c++) {
      if (match(pattern, candidate, p+1, c))
        return true;
    }
    return match(pattern, candidate, p+1, c);
  } else if (pattern[p] != '?' && pattern[p] != candidate[c]) {
    return false;
  }  else {
    return match(pattern, candidate, p+1, c+1);
  }
}

std::vector<User *> Server::getUsersByWildcard(std::string &wildcard) {
	std::vector<User *> filteredUsers;

	std::map<std::string, User*>::iterator it;
    for (it = serverData.users.begin(); it != serverData.users.end(); ++it) {
		User *user = it->second;

		if (match(wildcard.c_str(), user->getNickname().c_str(), 0, 0)) {
			filteredUsers.push_back(user);
		}
    }
	return filteredUsers;
}

void Server::handleWhoIs() {
	
	if (!currUser->isRegistered()) {
		handleError(ERR_NOTREGISTERED, "", "");
        return;
    }

	if ((inputMessage->getParamsSize() == 0)) {
		handleError(ERR_NONICKNAMEGIVEN, "", "");
		return ;
	}

	std::string wildcard = inputMessage->getParams()[0];
	std::vector<User *> v = getUsersByWildcard(wildcard);
	outputMessage->addFd(currUser->fd);
	for(std::vector<User *>::iterator it = v.begin(); it != v.end(); ++it) {
		User *user = *it;
		outputMessage->add(user->getNickname() + " "  + \
			user->username + " " + \
			user->ipAddress + " * :" + \
			user->realname, RPL_WHOISUSER);

		std::string channelsResponse = user->getNickname() + " :";

		outputMessage->add(channelsResponse, RPL_WHOISCHANNELS);
		outputMessage->add(user->getNickname() + " "  + \
			user->servername + " :" + \
			SERVER_INFO, RPL_WHOISSERVER);
		outputMessage->add("todo", RPL_WHOISIDLE);
 	}
	outputMessage->add(wildcard + " :End of /WHOIS list", RPL_ENDOFWHOIS);
}

void Server::handleAway() {
	std::string username = serverData.getUsernameByFd(inputMessage->fd_from);
	if (inputMessage->getParamsSize() == 0) {
		serverData.users[username]->setAwayStatus(false);
		return;
	}
	else {
		std::string message;
		for (int i = 0; i < inputMessage->getParamsSize(); ++i) {
			if (i != 0)
				message += " ";
			message += inputMessage->params[i];
		}
		serverData.users[username]->setAwayText(message);
		serverData.users[username]->setAwayStatus(true);
	}
}
