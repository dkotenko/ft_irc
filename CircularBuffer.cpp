#include "CircularBuffer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CircularBuffer::CircularBuffer(int size) :
	m_size(size),
	m_len(0),
	m_pos(0),
	m_buf = new char[size];
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CircularBuffer::~CircularBuffer()
{
	delete buf;
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

CircularBuffer &				CircularBuffer::operator=( CircularBuffer const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, CircularBuffer const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */