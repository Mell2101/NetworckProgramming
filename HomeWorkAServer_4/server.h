#pragma once
#include "mainfunctional.h"

void handle_accept(TalkToClient::ptr client, const boost::system::error_code& err)
{
    client->start();
    TalkToClient::ptr new_client = TalkToClient::_new();
    acceptor.async_accept(new_client->get_sock(), std::bind(handle_accept, new_client, std::placeholders::_1));
}

void server()
{
    TalkToClient::ptr client = TalkToClient::_new();
    acceptor.async_accept(client->get_sock(), std::bind(handle_accept, client, std::placeholders::_1));
    service.run();
}
