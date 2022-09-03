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
        void addUser(std::string username, std::string password);
        ~ServerData();

        std::map<std::string, User*> users;
private:

};

#endif /* ****************************************************** SERVERDATA_H */
