//
// Created by Redwyn Poetess on 22.08.2022.
//

#ifndef FT_IRC_USER_H
#define FT_IRC_USER_H
#include <string>

struct message {
    int fd_from;
    std::vector<int> fd_to;
    std::string operation;
    std::string data;
};

class User {
public:
    User::User(int fd, std::string nickname);

    std::string fd;
    std::string username;
    std::string nickname;
    std::string password;
    void setUserName(std::string username);
    void setNickName(std::string nickname);
    std::string User::getUserName();
    std::string User::getNickName();
    void delUser(std::string username);
};

#endif //FT_IRC_USER_H
