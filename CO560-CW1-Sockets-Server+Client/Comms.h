#pragma once
#ifndef COMMS_H // Guards
#define COMMS_H
using namespace std;
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>



class Comms
{
public:


	SOCKET clientSocket;
	SOCKET serverSocket, acceptSocket;
	int port = 55555;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	sockaddr_in service;
	sockaddr_in clientService;
	char buffer[200];
	char receiveBuffer[200];
	char sendBuffer[200];
	int byteCount = 0;


};

#endif 