#ifndef TCPSRVER_H
#define TCPSRVER_H

#include "socket.h"
#include "bindNetWork.h"
#include "listenNetWorck.h"
#include "acceptNetwork.h"

class Server{
    SocketNetWork _snw;
    BindNetWork _bn;
    ListenNetWork _ln;
    AcceptNetWorkAndClose _anwac;
public:
    void tcpServer();
};

void Server::tcpServer(){
    _snw.initSocket();
    _bn.bindNet();
    _ln.listenN();
    _anwac.acceptAndClose();
};

#endif // TCPSRVER_H
