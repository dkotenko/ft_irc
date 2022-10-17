#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include "client.hpp"
#include <ctime>
using namespace std;
//Client side
int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");
    //we need 2 things: ip address and port number, in that order
    if(argc != 3)
    {
        cerr << "Usage: ip_address port" << endl; exit(0); 
    } //grab the IP address and port number 
    char *serverIp = argv[1]; int port = atoi(argv[2]); 
    //create a message buffer 

    //setup a socket and connection tools 
    struct hostent* host = gethostbyname(serverIp); 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    //try to connect...
    int status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        cout<<"Error connecting to socket!"<<endl;
        return -1;
    }
    string message = "USER bot localhost localhost :bot\n";
    send(clientSd, message.c_str(), message.size(), 0);
    message = "NICK bot\n";
    send(clientSd, message.c_str(), message.size(), 0);
    message = "PASS 1234\n";
    send(clientSd, message.c_str(), message.size(), 0);
    while(1)
    {
        char msg[1500];
        memset(&msg, 0, sizeof(msg));//clear the buffer
        recv(clientSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "fact\n")) {
            std::srand(std::time(nullptr)); // use current time as seed for random generator
            int random_variable = std::rand() % 5;
            std::string message;
            switch (random_variable) {
                case 0:
                    message = "Саудовская Аравия не содержит рек.\n";
                    break;
                case 1:
                    message = "У медуз нет мозгов и кровеносных сосудов.\n";
                    break;
                case 2:
                    message = "До 17 века термометры заполняли коньяком.\n";
                    break;
                case 3:
                    message = "Ладожское озеро является самым большим в Европе.\n";
                    break;
                case 4:
                    message = "На Юпитере регулярно идут алмазные дожди.\n";
                    break;
            }
            cout << "Server: " << msg;
            send(clientSd, message.c_str(), message.size(), 0);
        }
        memset(&msg, 0, sizeof(msg));//clear the buffer
    }
    close(clientSd);
    cout << "********Session********" << endl;
    cout << "Connection closed" << endl;
    return 0;    
}