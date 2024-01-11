#pragma once
#ifndef CLIENT_H
#define CLIENT_H
#include "Comms.h"

class Client :public Comms
{
public:

	// ------------  Check the Socket Connection  --------------
	int CheckSocket(WSADATA& wsaData) {
		wVersionRequested = MAKEWORD(2, 2);
		wsaerr = WSAStartup(wVersionRequested, &wsaData);
		if (wsaerr != 0) {
			cout << "The Winsock dll not found!" << endl;
			return 0;
		}
		else {
			cout << "--------------- Client ----------------- " << endl;
			cout << " The Winsock dll found!" << endl;
			cout << " The status: " << wsaData.szSystemStatus << endl;
			cout << "---------------------------------------" << endl;
			cout << " " << endl;
			return 1;
		}

	}

	// ------------  Create a socket  --------------
	SOCKET CreateClientSocket() {
		clientSocket = INVALID_SOCKET;
		clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (clientSocket == INVALID_SOCKET) {
			cout << "Error at socket(): " << WSAGetLastError() << endl;
			WSACleanup();
			return INVALID_SOCKET;
		}
		else {
			cout << "socket() is OK!" << endl;
			return clientSocket;
		}
	}

	// ------------  Connect to the server  --------------
	int ConnectToServer(SOCKET clientSocket, sockaddr_in& clientService) {
		clientService.sin_family = AF_INET;
		InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
		clientService.sin_port = htons(port);

		if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
			cout << "Client: connect() - Failed to connect." << endl;
			
			return 0;
		}
		else {
			cout << "Client: connect() is OK." << endl;
			cout << "Client: Can start sending and receiving data..." << endl;
			return 1;
		}
	}

	// ------------  Chat to the server  --------------
	int ChatToServer(SOCKET clientSocket, char* buffer, int bufferSize) {
		while (true) {

			cout << "\n Enter a message: ";
			cin.getline(buffer, bufferSize);

			int byteCount = send(clientSocket, buffer, bufferSize, 0);
			if (byteCount == SOCKET_ERROR) {
				printf("Client send error %ld.\n", WSAGetLastError());
				return -1;
			}

			if (strcmp(buffer, "QUIT") == 0) {
				break;
			}

			byteCount = recv(clientSocket, buffer, bufferSize, 0);
			if (byteCount > 0) {
				cout << "Server echoed: " << buffer << endl;
			}
			else {
				cout << "Error receiving message: " << WSAGetLastError() << endl;
				return -1;
			}
		}
		return 0;
	}
};

#endif 