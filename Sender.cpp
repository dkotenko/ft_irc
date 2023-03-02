#include "Sender.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Sender::Sender()
{
}

Sender::Sender( const Sender & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Sender::~Sender()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Sender &				Sender::operator=( Sender const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Sender const & i )
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