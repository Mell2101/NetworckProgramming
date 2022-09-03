// This file keeps haeders and settings aserver.
//Headers Asynchronous Server - has
#pragma once

#include <iostream>
#include <memory>
#include <string>
//libboost
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#define MEM_FN(x)      std::bind(&selfType::x, shared_from_this())
#define MEM_FN1(x,y)   std::bind(&selfType::x, shared_from_this(), y)
#define MEM_FN2(x,y,z) std::bind(&selfType::x, shared_from_this(), y, z)

using namespace boost::asio;
using namespace boost::system;

io_service service;
ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));

class TalkToClient;
typedef std::shared_ptr<TalkToClient> client_ptr;
typedef std::vector<client_ptr> client_array;
client_array clients;

