#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <unistd.h> // For read
#include <string>
#include <iostream>
#include <cstdlib>
#include "Server.hpp"

Server::Server(int port) {
    this->port = port;
}
void Server::create_socket() {
// Create a socket (IPv4, TCP)
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::listen_port() {
    // Listen to port on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(port); // htons is necessary to convert a number to
    // network byte order
    if (bind(this->sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(this->sockfd, 10) < 0) {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    int addrlen = sizeof(sockaddr);
    this->connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
    if (this->connection < 0) {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    //std::string response = "001";
    //send(connection, response.c_str(), response.size(), 0);
    //write(this->sockfd, "001", 3);
}

void Server::mainloop() {
    while (true) {
        // Read from the connection
        char buffer[100];
        read(this->connection, buffer, 100);
        if (strcmp(buffer, "QUIT\n") == 0) {
            break;
        }
        std::cout << "The message was: " << buffer;
        // Send a message to the connection
        //std::string response = "Good talking to you\n";
        //send(connection, response.c_str(), response.size(), 0);
        bzero(buffer, 100);
    }
}

void Server::s_close() {
    // Close the connections
    close(this->connection);
    close(this->sockfd);
}
