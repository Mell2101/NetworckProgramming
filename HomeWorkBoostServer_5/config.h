#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

boost::asio::io_service service;

std::size_t readComplete(char* buff, const std::error_code& err, std::size_t bytes)
{
    if (err) return 0;

    bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;

    return found ? 0 : 1;

}