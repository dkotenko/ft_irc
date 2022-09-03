#ifndef SERVERDATA_HPP
# define SERVERDATA_HPP

# include <iostream>
# include <string>
#include "map"
#include "User.hpp"

class ServerData
{

	public:
		ServerData();
        std::map<std::string, User*> users;
		ServerData &		operator=( ServerData const & rhs );
        void addUser(std::string username, std::string password);
        ~ServerData();
private:

};


std::ostream &			operator<<( std::ostream & o, ServerData const & i );

#endif /* ****************************************************** SERVERDATA_H */
