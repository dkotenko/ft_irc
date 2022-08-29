//
// Created by Redwyn Poetess on 22.08.2022.
//

#include "User.hpp"
#include <string>
#include <map>

User::User(int type, int fd)
{
	this->clean();
    isConnected = false;
	this->type = type;
	this->fd = fd;
	this->buf_read = new char[BUF_SIZE + 1] ();
	this->buf_write = new char[BUF_SIZE + 1] ();
}

void User::clean() {
	this->type = FD_FREE;
}

void User::setUserName(std::string username) {
    this->username = username;
}

void User::setFd(int fd) {
    this->fd = fd;
}

void User::setNickName(std::string nickname) {
    this->nickname = nickname;
}


std::string User::getUserName() {
    return this->username;
}

int User::getFd(){
    return fd;
}

std::string User::getNickName() {
    return this->username;
}

void User::delUser(std::string username) {
    (void)username;
}

void		User::setPasswordPassed() {
    pswdPassed = 1; 
}

void		User::setNicknamePassed() {
    nnPassed = 1;
}

void		User::setUserPassed() {
    userPassed = 1;
}

bool		User::getAllPassedArgs() { return pswdPassed && nnPassed && userPassed ? true : false; }


int			User::getPasswordPassed() {
    return(this->pswdPassed);
}

int			User::getUserPassed() {
    return(this->userPassed);
}

int			User::getNickNamePassed() {
    return(this->nnPassed);
}

std::string getFirstWord(std::string msg){
    std::string fw;
    if (!msg.empty()){
        std::istringstream stringToSplit(message.c_str());
		std::string stringSplitted;
	
		while (getline(stringToSplit, stringSplitted, ' ' ) && stringSplitted != " "){
			fw = stringSplitted;
			break ;
		}
		fw.erase(std::remove(fw.begin(), fw.end(), '\r'), fw.end());
		fw.erase(std::remove(fw.begin(), fw.end(), '\n'), fw.end());
	}
	return fw;
}

vector<std::string>		getParametrs(std::string message){
	vector<string> parametrs;
	if (!message.empty()){	
		std::istringstream stringToSplit(message.c_str());
		string stringSplitted;
	
		while (getline(stringToSplit, stringSplitted, ' ' ) && stringSplitted != " ")
			parametrs.push_back(stringSplitted);
		parametrs.erase(parametrs.begin());
		
		for (vector<string>::iterator it = parametrs.begin(); it != parametrs.end(); it++)
			(*it).erase(std::remove((*it).begin(), (*it).end(), '\r'), (*it).end());
		
		for (vector<string>::iterator it = parametrs.begin(); it != parametrs.end(); it++)
			(*it).erase(std::remove((*it).begin(), (*it).end(), '\n'), (*it).end());
	}
	return parametrs;
}

void	User::sendErr(std::string err) {
    send(fd, err.c_str(), err.length() + 1, 0);
}

//парсинг команд
int User::cmdParse(Server &server, std::string msg, int i, struct pollfd filedescs[]){
    bool ifAllIsDone = server.getUserID(i).getAllPassedArgs();
    if (!ifAllIsDone)
		return server.getUserID(i).preparationCommands(server, msg, i);
	vector<User> newVector = server.getVectorOfUsers();
	Cmd command(msg, server.getUserID(i).getFd(), server.getUserID(i).getNickname(), server.getUserID(i).getUsername(), newVector);
	return command.commandStart(server, filedescs);
}

int			User::preparationCommands(Server &server, std::string message, int i){
    static int onlyOnce = 0;
	vector<std::string> parametrs = getParametrs(message);
	std::string fw = getFirstWord(message);

	if (!onlyOnce && parametrs.size() > 0){
		if (parametrs[0] == "LSPING") {
            std::string pong = "PONG " + parametrs[1] + "\r\n";
            send(fd, pong.c_str(), pong.length() + 1, 0);
        }
        ++onlyOnce;
    }
	
	if (server.getUserID(i).getPasswordPassed() == 0 && fw == "PASS"){
		server.getUserID(i).checkUserPassword(server, message, i);
		return (1);
	} else if (server.getUserID(i).getPasswordPassed() == 0 && fw != "PASS") {
		sendErr(ERR_NOTREGISTERED);
		return (1);
	}

//парсинг никнейма пользователя

//парсинг команды пользователя

