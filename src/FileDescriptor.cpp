#include "FileDescriptor.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

FileDescriptor::FileDescriptor(int fd) :
	port(0),
	fd(fd),
	connectStatus(NOT_REGISTERED),
	type(2),
	registered(false),
	hostname(""),
	ipAddress(""),
	userInfo({"", "", "", "", ""})
{}

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

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */