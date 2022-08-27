#include "FileDescriptor.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

FileDescriptor::FileDescriptor()
{
	this->clean();
}

FileDescriptor::FileDescriptor(int type, int fd)
{
	this->clean();
	this->type = type;
	this->fd = fd;
}

int FileDescriptor::GetType() const {
	return type;
}

void FileDescriptor::clean() {
	this->type = FD_FREE;
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

/*

FileDescriptor &				FileDescriptor::operator=( FileDescriptor const & rhs )
{
	if ( this != &rhs )
	{
		this->type = rhs.GetType();
	}
	return this;
}
*/

std::ostream &			operator<<( std::ostream & o, FileDescriptor const & i )
{
	o << "Value = " << i.GetType();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
