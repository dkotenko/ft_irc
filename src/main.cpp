
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <cerrno>
#include <string>
#include "Server.hpp"

int validation(int argc, char **argv) {
    if (argc != 3) {
        std::cout<<"Incorrect format" << std::endl;
        std::cout<<"Usage: ./ft_irc <port> <password>" << std::endl;
        return 1;
    }
    //std::cout<<argv[1]<<" "<<argv[2]<<"\n";
    (void)argv;
    return 0;
}

int main(int argc, char **argv) {
    if (validation(argc, argv)) {
        exit(0);
    }
    
    int port = atoi(argv[1]);
    std::string *password = new std::string(argv[2]);
    // TODO port validation
    
    std::cout << "Init server, port - " << port << std::endl;
    Server server(port, password);
    server.run();
    return 0;
}
