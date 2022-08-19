#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <cerrno>

void server() {
    // Create a socket (IPv4, TCP)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Listen to port 9999 on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(9999); // htons is necessary to convert a number to
    // network byte order
    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Start listening. Hold at most 10 connections in the queue
    if (listen(sockfd, 10) < 0) {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Grab a connection from the queue
    int addrlen = sizeof(sockaddr);
    int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
    if (connection < 0) {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    while (true) {
        // Read from the connection
        char buffer[100];
        read(connection, buffer, 100);
        if (strcmp(buffer, "QUIT\n") == 0) {
            break;
        }
        std::cout << "The message was: " << buffer;
        // Send a message to the connection
        //std::string response = "Good talking to you\n";
        //send(connection, response.c_str(), response.size(), 0);
        bzero(buffer, 100);
    }
    // Close the connections
    close(connection);
    close(sockfd);
}

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

int main(int argc, char **argv) {
    if (validation(argc, argv)) {
        exit(0);
        }
    server();
}
