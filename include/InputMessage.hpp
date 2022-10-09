//
// Created by Redwyn Poetess on 03.09.2022.
//

#ifndef FT_IRC_MESSAGEINPUT_H
#define FT_IRC_MESSAGEINPUT_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

class InputMessage {
public:
    int fd_from;
    std::vector<int> fd_to;
    std::string command;
    std::vector<std::string> params;
    std::string *src;
    int countParams;

    int getFd_from();
    void setFd_from(int fd_from);
    std::vector<int> getFd_to();
    void setFd_to(std::vector<int> fd_to);
    std::string getCommand();
    void setCommand(std::string command);
    
    void setParams(std::vector<std::string> params);
    int getCountParams();
    //std::string get*Src();
    //void set*Src(std::string *src);
    InputMessage();
    //~InputMessage();
    

    std::vector<std::string> &getParams();
};


#endif //FT_IRC_MESSAGEINPUT_H
