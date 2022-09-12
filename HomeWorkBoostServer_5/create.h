#pragma once
#include "inplementaion.h"

class CreateServer {
	Connection* connect;
public:
	void callServer();
	
};

void CreateServer::callServer() 
{
	connect->connect();
}