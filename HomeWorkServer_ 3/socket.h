#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>

class SocketNetWork
{
    int _listenfd;

public:
    struct sockaddr_in _cliaddr, _servaddr;

    int getListendfd(){
        return _listenfd;
    };

    void setListendfd(int listenfd){
        _listenfd = listenfd;
    };

    void initSocket();
};

void SocketNetWork::initSocket(){
   _listenfd = socket(AF_INET, SOCK_STREAM, 0);

   bzero(&_servaddr, sizeof (_servaddr));
   _servaddr.sin_family = AF_INET;
   _servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   _servaddr.sin_port = htons(9877);

}
#endif // SOCKET_H
