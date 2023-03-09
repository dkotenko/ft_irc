#ifndef FILEDESCRIPTOR_HPP
# define FILEDESCRIPTOR_HPP

# include <iostream>
# include <string>

class FileDescriptor
{
	typedef struct  s_userInfo
	{
		std::string username;
    	std::string nickname;
    	std::string realname;
    	std::string hostname;	
	} t_userInfo;
	

	public:

		FileDescriptor();
		FileDescriptor( FileDescriptor const & src );
		~FileDescriptor();

		FileDescriptor &		operator=( FileDescriptor const & rhs );
    	std::string hostname;
    	std::string ipAddress;
		int port;
		int fd;
		int connectStatus;
		bool registered;
		bool welcomeReceived;
		t_userInfo userInfo;
		char *buf_read;
    	PingTimer timer;
};

std::ostream &			operator<<( std::ostream & o, FileDescriptor const & i );

#endif /* ************************************************** FILEDESCRIPTOR_H */