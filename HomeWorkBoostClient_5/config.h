#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace boost::asio;
using namespace boost::system;
using namespace std::placeholders;

io_service service;
ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);

std::size_t readComplete(char* buf, const error_code& err, std::size_t bytes)
{
    if (err) return 0;

    bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;

    return found ? 0 : 1;
}