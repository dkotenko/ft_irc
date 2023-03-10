#include "FileDescriptor.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

FileDescriptor::FileDescriptor(int fd) :
	fd(fd),
	type(FD_FREE),
	connectStatus(NOT_REGISTERED),
	registered(false),
	ipAddress(""),
	port(0)
{}

FileDescriptor::FileDescriptor( const FileDescriptor & src )
{
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
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
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