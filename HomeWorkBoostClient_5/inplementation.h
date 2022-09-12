#pragma once
#include "config.h"

class Client 
{

public:

	void sysEcho(std::string msg);
    void theCallClient();
    
};

void Client::sysEcho(std::string msg)
{
    msg += '\n';
    ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(buffer(msg));

    char buf[1024];

    int bytes = read(sock, buffer(buf),
        std::bind(readComplete, buf, std::placeholders::_1, std::placeholders::_2));

    std::string copy(buf, bytes - 1);
    msg = msg.substr(0, msg.size() - 1);

    std::cout << "Server echoed our " << msg << ": "
        << (copy == msg ? "OK" : "FAIL")
        << std::endl;

    sock.close();

}

void Client::theCallClient()
{
    std::vector<std::string> messages{ "Test 1", "Test 2" };

    boost::thread_group threads;
    for (auto it = messages.begin(); it != messages.end(); ++it)
    {
        threads.create_thread(std::bind(sysEcho, *it));
        boost::this_thread::sleep(boost::posix_time::millisec(100));
    }

    threads.join_all();

}