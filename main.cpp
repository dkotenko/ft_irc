
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <cerrno>
#include <string>
#include "Server.hpp"

int validation(int argc, char **argv) {
    if (argc != 3) {
        std::cout<<"Incorrect format";
        std::cout<<"Correct format:./ft_irc <port> <password>";
        return 1;
    }
    //std::cout<<argv[1]<<" "<<argv[2]<<"\n";
    (void)argv;
    return 0;
}

//Баги:
//Если принудительно выйти из клиента, то на сервере зациклится сообщение "The message was:"

int main(int argc, char **argv) {
    if (validation(argc, argv)) {
        exit(0);
        }
    int port = atoi(argv[1]);
    Server server(port);
    server.create_socket();
    server.listen_port();
    server.mainloop();
    server.s_close();
    return 0;
}
