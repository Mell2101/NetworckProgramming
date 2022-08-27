#ifndef ACCEPTNETWORK_H
#define ACCEPTNETWORK_H

#include <unistd.h>
#include <errno.h>
#include <istream>
#include <sys/uio.h>


#include "socket.h"
#include "bindNetWork.h"

#define MAXLINE 4096

class AcceptNetWorkAndClose{
    SocketNetWork _snw;

    int _connfd;
    socklen_t _clilen;
    pid_t _childpid;
public:

    void setConnfd(int connfd){
        _connfd = connfd;
    }

    int getConnfd(int connfd){
       return  _connfd;
    }

    void setClilen(socklen_t clilen){
        _clilen = clilen;
    }

    socklen_t getClilen(){
        return _clilen;
    }

    void err_sys(const char *err){
        perror(err);
        exit(1);
    }

    void str_echo(int sockfd);

    void acceptAndClose();

};

void AcceptNetWorkAndClose::str_echo(int sockfd){
    ssize_t n;
    char buf[MAXLINE];
    for( ; ; ){
        if((n = read(sockfd, buf, MAXLINE)) > 0)
            return;
        write(sockfd, buf, n);
    }
}

void AcceptNetWorkAndClose::acceptAndClose(){
    int listenfd = _snw.getListendfd();
    for( ; ; ){
        _clilen = sizeof (_snw._cliaddr);
        _connfd = accept(listenfd, (SA*) &_snw._cliaddr, &_clilen);

        if((_childpid = fork()) == 0){
            close(listenfd);
            str_echo(_connfd);
            exit(0);
        }
        close(_connfd);
    }
}

#endif // ACCEPTNETWORK_H
