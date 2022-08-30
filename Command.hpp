#pragma once
#include "MessageOp.hpp"
#include "User.hpp"
#include "Server.hpp"
#include "Chanel.hpp"

class User;
class Server;
class Chanel;

class Command{
	private:
	int				fd;
	int				i;
	string			message;
	string			username;
	string			nickname;
	string			command;

	vector<User>	users;
	vector<string>	arguments;

	public:
	Command();
	Command(string message, int fd, string nickname, string username, vector<User> &users);
	~Command();

	vector<User>	getVectorOfUsers();

	int		commandStart(Server &server, struct pollfd fds[]);
	void	checkCommand(Server &server, struct pollfd fds[]);
	void	doQuitCommand(Server &server, struct pollfd fds[]);
	void	doNickCommand(Server &server);
	void	doPrivmsgCommand(Server &server);
	void	doNoticeCommand(Server &server);
	void	doJoinCommand(Server &server);
	void	doKickCommand(Server &server);
	void	doHelpCommand();
	void	doInfoCommand();
	void	doShowuserCommand(Server &server);
	void	doShowtimeCommand();
	void	createNewChannel(Server &server);
	bool	checkUserInChannel(Chanel &channel);
};


