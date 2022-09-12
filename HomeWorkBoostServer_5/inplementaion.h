#pragma once
#include "config.h"

class Connection 
{
    char buff[1024];
public:
   
    void connect();

};

void Connection::connect() 
{
    boost::asio::ip::tcp::acceptor acceptor(service,
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8001));

    while (true)
    {
        boost::asio::ip::tcp::socket sock(service);
        acceptor.accept(sock);

        int bytes = read(sock, boost::asio::buffer(buff),
            std::bind(readComplete, buff, std::placeholders::_1, std::placeholders::_2));

        std::string msg(buff, bytes);

        std::cout << "Received: " << msg << std::endl;

        sock.write_some(boost::asio::buffer(msg));
        sock.close();
    }
}

