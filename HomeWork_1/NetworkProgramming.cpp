#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <thread>

#pragma comment(lib, "ws2_32.lib")


class Server {

	int erStat;
	in_addr ip_to_num;
	SOCKET ServSock;
	SOCKET ClientConn;
public:
	//Server() { _keyConst(2, 1024);}
	int numberFormatForSocket();
	int winSockInitialization();
	int serverSocketInitialization();
	int serverSocketbinding();
	int startingListenClients();
	int clientSocketCreation();
	int exchangeTextDataBetweenServerandClient();
};

int Server::numberFormatForSocket() {
	erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);

	if (erStat <= 0) {
		std::cout << "Error in IP translation to special numeric format" << std::endl;
		return 1;
	}
}
int Server::winSockInitialization() {
	WSADATA wsData;

	erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (erStat != 0) {
		std::cout << "Error WinSock version initializaion #";
		std::cout << WSAGetLastError();
		return 1;
	}
	else
		std::cout << "WinSock initialization is OK" << std::endl;
}
int Server::serverSocketInitialization() {
	ServSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ServSock == INVALID_SOCKET) {
		std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else
		std::cout << "Server socket initialization is OK" << std::endl;
}
int Server::serverSocketbinding() {
	ServSock = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in servInfo;
	ZeroMemory(&servInfo, sizeof(servInfo));	// Initializing servInfo structure

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(1234);

	erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));

	if (erStat != 0) {
		std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else
		std::cout << "Binding socket to Server info is OK" << std::endl;
}
int Server::startingListenClients() {
	erStat = listen(ServSock, SOMAXCONN);

	if (erStat != 0) {
		std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Listening..." << std::endl;
	}
}
int Server::clientSocketCreation() {
	sockaddr_in clientInfo;
	ZeroMemory(&clientInfo, sizeof(clientInfo));	// Initializing clientInfo structure

	int clientInfo_size = sizeof(clientInfo);

	ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

	if (ClientConn == INVALID_SOCKET) {
		std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		closesocket(ClientConn);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Connection to a client established successfully" << std::endl;
		char clientIP[22];

		inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);	// Convert connected client's IP to standard string format

		std::cout << "Client connected with IP address " << clientIP << std::endl;

	}
}
int Server::exchangeTextDataBetweenServerandClient() {
	std::vector <char> servBuff(1024), clientBuff(1024);						
	short packet_size = 0;												

	while (true) {
		packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);					
		std::cout << "Client's message: " << servBuff.data() << std::endl;

		std::cout << "Your (host) message: ";
		fgets(clientBuff.data(), clientBuff.size(), stdin);

		// Check whether server would like to stop chatting 
		if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
			shutdown(ClientConn, SD_BOTH);
			closesocket(ServSock);
			closesocket(ClientConn);
			WSACleanup();
			return 0;
		}

		packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);

		if (packet_size == SOCKET_ERROR) {
			std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
			closesocket(ServSock);
			closesocket(ClientConn);
			WSACleanup();
			return 1;
		}

	}

	closesocket(ServSock);
	closesocket(ClientConn);
	WSACleanup();

}

class Client {
	int erStat;
	in_addr ip_to_num;
	SOCKET ClientSock;
	
public:
	void socketFunction() 
	{
		inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
	};

	int winSockInitialization();
	void socketInitialization();
	int establishingConnectionServer();
	int exchangeTextDataBetweenServerandClient();
};

int Client::winSockInitialization() {
	WSADATA wsData;
	erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (erStat != 0) {
		std::cout << "Error WinSock version initializaion #";
		std::cout << WSAGetLastError();
		return 1;
	}
	else
		std::cout << "WinSock initialization is OK" << std::endl;
}
void Client::socketInitialization() {
	 ClientSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ClientSock == INVALID_SOCKET) {
		std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
		closesocket(ClientSock);
		WSACleanup();
	}
	else
		std::cout << "Client socket initialization is OK" << std::endl;

}
int Client::establishingConnectionServer() {
	sockaddr_in servInfo;

	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(1234);

	erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));

	if (erStat != 0) {
		std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
		closesocket(ClientSock);
		WSACleanup();
		return 1;
	}
	else
		std::cout << "Connection established SUCCESSFULLY. Ready to send a message to Server" << std::endl;
}
int Client::exchangeTextDataBetweenServerandClient() {
	std::vector <char> servBuff(1024), clientBuff(1024);							
	short packet_size = 0;												

	while (true) {

		std::cout << "Your (Client) message to Server: ";
		fgets(clientBuff.data(), clientBuff.size(), stdin);

		
		if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
			shutdown(ClientSock, SD_BOTH);
			closesocket(ClientSock);
			WSACleanup();
			return 0;
		}

		packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);

		if (packet_size == SOCKET_ERROR) {
			std::cout << "Can't send message to Server. Error # " << WSAGetLastError() << std::endl;
			closesocket(ClientSock);
			WSACleanup();
			return 1;
		}

		packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);

		if (packet_size == SOCKET_ERROR) {
			std::cout << "Can't receive message from Server. Error # " << WSAGetLastError() << std::endl;
			closesocket(ClientSock);
			WSACleanup();
			return 1;
		}
		else
			std::cout << "Server message: " << servBuff.data() << std::endl;

	}

	closesocket(ClientSock);
	WSACleanup();
}

class Work {
	Server serv;
	Client cli;
public:
	void server();
	void client();
};

void Work::server() {
	
	serv.numberFormatForSocket();
	serv.winSockInitialization();
	serv.serverSocketInitialization();
	serv.serverSocketbinding();
	serv.startingListenClients();
	serv.clientSocketCreation();
	serv.exchangeTextDataBetweenServerandClient();
	
	
	/*
	serv->numberFormatForSocket();
	serv->winSockInitialization();
	serv->serverSocketInitialization();
	serv->serverSocketbinding();
	serv->startingListenClients();
	serv->clientSocketCreation();
	serv->exchangeTextDataBetweenServerandClient();
	*/
}
void Work::client() {
	/*
	cli->winSockInitialization();
	cli->socketInitialization();
	cli->establishingConnectionServer();
	cli->exchangeTextDataBetweenServerandClient();
	*/
	cli.socketFunction();
	cli.winSockInitialization();
	cli.socketInitialization();
	cli.establishingConnectionServer();
	cli.exchangeTextDataBetweenServerandClient();

	

}

int main() 
{
	Work wr;
	
	std::thread t([&]() {
		wr.server();
	});

	std::thread t2([&]() {
		wr.client();;
		});

	t.join();
	t2.join();

	
	
	
	

	return 0;

}
