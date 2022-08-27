#ifndef LISTENNETWORCK_H
#define LISTENNETWORCK_H

#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>

#include "socket.h"
#include "bindNetWork.h"

#define LISTENQ 1024

class ListenNetWork{
    SocketNetWork _snw;

public:

    void listenN();

};

void ListenNetWork::listenN(){
    int listenfd = _snw.getListendfd();

    listen(listenfd, LISTENQ);

}

#endif // LISTENNETWORCK_H
