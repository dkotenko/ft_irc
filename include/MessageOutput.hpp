//
// Created by Redwyn Poetess on 04.09.2022.
//

#ifndef FT_IRC_MESSAGEOUTPUT_H
#define FT_IRC_MESSAGEOUTPUT_H
#include <string>

class MessageOutput {
public:
    std::string data;
    int fd_from;
    std::vector<int> fd_to;
};


#endif //FT_IRC_MESSAGEOUTPUT_H