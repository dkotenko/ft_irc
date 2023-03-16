#include "FileDescriptor.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

FileDescriptor::FileDescriptor(int fd) :
	port(0),
	fd(fd),
	connectStatus(NOT_REGISTERED),
	type(FD_FREE),
	registered(false),
	hostname(""),
	ipAddress(""),
	userInfo((t_userInfo){"", "", "", "", ""})
{
	std::memset(buf_read, 0, MESSAGE_MAX_LEN + 1);
	std::memset(buf_write, 0, MESSAGE_MAX_LEN + 1);
}

FileDescriptor::FileDescriptor( const FileDescriptor & src )
{
	port = src.port;
	fd = src.fd;
	connectStatus = src.connectStatus;
	type = src.type;
	registered = src.registered;
	hostname = src.hostname;
	ipAddress = src.ipAddress;
	userInfo = src.userInfo;
	std::memset(buf_read, 0, MESSAGE_MAX_LEN + 1);
	std::memset(buf_write, 0, MESSAGE_MAX_LEN + 1);
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

FileDescriptor::~FileDescriptor()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

FileDescriptor &				FileDescriptor::operator=( FileDescriptor const & rhs )
{
	if ( this != &rhs )
	{
		port = rhs.port;
		fd = rhs.fd;
		connectStatus = rhs.connectStatus;
		type = rhs.type;
		registered = rhs.registered;
    	hostname = rhs.hostname;
    	ipAddress = rhs.ipAddress;
		userInfo = rhs.userInfo;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, FileDescriptor const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

bool FileDescriptor::isRegistered() {
    return registered;
}

void FileDescriptor::setRegistered(bool b) {
	registered = b;
}

void FileDescriptor::clean() {
	*this = FileDescriptor(fd);
}

bool FileDescriptor::hasMessage() {
	return buf_write[0];
}

bool FileDescriptor::isLost() {
	return timer.isNoResponce();
}

void FileDescriptor::doPing() {
	timer.doPing();
}

void FileDescriptor::updatePing() {
	timer.reset();
}

bool FileDescriptor::isNeedsPing() {
	bool needs = timer.isNeedsPing();
	if (needs) {
		log_debug("fd %d needs ping", fd);
	}
	return needs;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */