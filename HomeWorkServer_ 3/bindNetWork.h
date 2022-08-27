#ifndef BINDNETWORK_H
#define BINDNETWORK_H

#include <sys/stat.h>
#include <sys/types.h>

#include "socket.h"

#define SA struct sockaddr

class BindNetWork{
SocketNetWork _snw;
public:

    void bindNet(){
        int listendfd = _snw.getListendfd();
        bind(listendfd, (SA*)& _snw._servaddr, sizeof (_snw._servaddr));
    };
};

#endif // BINDNETWORK_H
