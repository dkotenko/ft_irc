#ifndef FILEDESCRIPTOR_HPP
# define FILEDESCRIPTOR_HPP

# include <iostream>
# include <string>
# include <cstring>
# include "PingTimer.hpp"
# include "logger.hpp"

enum e_connect_states {
    NOT_REGISTERED  = 0,
    NICK_PASSED     = 1, //0b001
    USER_PASSED     = 2, //0b010
    PASS_PASSED     = 4, //0b100
    REGISTERED      = 7  //0b111
};

enum e_fds {
    FD_FREE     = 0,
    FD_SERV     = 1,
    FD_CLIENT   = 2,
    FD_NUM      = 3
};

typedef struct  s_userInfo
{
	s_userInfo():
		username("NO USERNAME"),
		nickname("NO NICKNAME"),
		realname("NO REALNAME"),
		hostname("NO HOSTNAME"),
		servername("NO SERVERNAME") {}
	s_userInfo(const s_userInfo& src):
		username(src.username),
		nickname(src.nickname),
		realname(src.realname),
		hostname(src.hostname),
		servername(src.servername) {}
	s_userInfo &operator=( s_userInfo const & rhs ) {
		username = rhs.username;
		nickname = rhs.nickname;
		realname = rhs.realname;
		hostname = rhs.hostname;
		servername = rhs.servername;
		return *this;
	}
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
		char buf_write[MESSAGE_MAX_LEN + 1];
    	std::string hostname;
    	std::string ipAddress;
		t_userInfo userInfo;
    	

    	bool isRegistered();
    	void setRegistered(bool b);
		void clean();
		bool hasMessage();
		

	private:
		
};

std::ostream &			operator<<( std::ostream & o, FileDescriptor const & i );

#endif /* ************************************************** FILEDESCRIPTOR_H */