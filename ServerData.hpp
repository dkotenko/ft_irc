#ifndef SERVERDATA_HPP
# define SERVERDATA_HPP

# include <iostream>
# include <string>
#include "map"
#include "User.hpp"
#include "Chanel.hpp"

class ServerData
{

	public:
		ServerData();
        ~ServerData();
        void addUser(std::string username, std::string password);
        void addChanel(std::string chanelname);
        std::map<std::string, User*> users;
        std::map<std::string, Chanel*> chanels;
private:

};

#endif /* ****************************************************** SERVERDATA_H */
