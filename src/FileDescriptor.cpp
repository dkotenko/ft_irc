#include "FileDescriptor.hpp"
#include <strings.h>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

FileDescriptor::FileDescriptor()
{
	
}

FileDescriptor::FileDescriptor(int type, int fd)
{
	this->clean();
	this->type = type;
	this->fd = fd;
	this->buf_read = new char[BUF_SIZE + 1] ();
	this->buf_write = new char[BUF_SIZE + 1] ();
}

int FileDescriptor::GetType() const {
	return type;
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
