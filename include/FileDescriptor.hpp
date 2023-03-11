#ifndef FILEDESCRIPTOR_HPP
# define FILEDESCRIPTOR_HPP

# include <iostream>
# include <string>
# include "PingTimer.hpp"

enum e_connect_states {
    NOT_REGISTERED  = 0,
    NICK_PASSED     = 1, //0b001
    USER_PASSED     = 2, //0b010
    PASS_PASSED     = 4, //0b100
    REGISTERED      = 7,  //0b111
    CONNECT_STATES_NUM = 5
};

enum e_fds {
    FD_FREE     = 0,
    FD_SERV     = 1,
    FD_CLIENT   = 2,
    FD_NUM      = 3
};

typedef struct  s_userInfo
{
	std::string username;
	std::string nickname;
	std::string realname;
	std::string hostname;
	std::string servername;
} t_userInfo;


class FileDescriptor {

	public:
		FileDescriptor(int fd);
		FileDescriptor( FileDescriptor const & src );
		~FileDescriptor();
		FileDescriptor &		operator=( FileDescriptor const & rhs );

		int port;
		int fd;
		int connectStatus;
		int type;
		bool registered;
		char buf_read[MESSAGE_MAX_LEN + 1];
    	std::string hostname;
    	std::string ipAddress;
		t_userInfo userInfo;
    	PingTimer timer;

    	bool isRegistered();
    	void setRegistered(bool b);
		void clean();

	private:
		
};

std::ostream &			operator<<( std::ostream & o, FileDescriptor const & i );

#endif /* ************************************************** FILEDESCRIPTOR_H */