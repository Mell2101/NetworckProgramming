#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H


#include<iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>


class SocketClient{
    int _sockfd;
public:
    struct sockaddr_in servaddr;

    int getSockfd(){
        return _sockfd;
    }

    void socketNetClient();

};


void SocketClient::socketNetClient(){

    _sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9877);

}

#endif // SOCKETCLIENT_H
