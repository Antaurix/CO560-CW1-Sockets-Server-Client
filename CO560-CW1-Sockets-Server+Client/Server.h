#pragma once
#ifndef SERVER_H
#define SERVER_H
#include "Comms.h"

class Server : public Comms
{

public:

    SOCKADDR_STORAGE from;
    int fromlen;

    // ------------  Check the Socket Connection  --------------

    
    int CheckSocketServer(WSADATA& wsaData) {
        wVersionRequested = MAKEWORD(2, 2);
        wsaerr = WSAStartup(wVersionRequested, &wsaData);
        if (wsaerr != 0) {
            cout << "The Winsock dll not found!" << endl;
            return 0;
        }
        else {
            cout << " --------------- Server ----------------- " << endl;
            cout << "The Winsock dll found!" << endl;
            cout << "The status: " << wsaData.szSystemStatus << endl;
            cout << "---------------------------------------" << endl;
            cout << " " << endl;
            return 1;
        }
    }

    // ------------- Create a new socket -------------
    SOCKET CreateServerSocket() {
        serverSocket = INVALID_SOCKET;
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET) {
            cout << "Error at socket(): " << WSAGetLastError() << endl;
            WSACleanup();
            return INVALID_SOCKET;
        }
        else {
            cout << "socket() is OK!" << endl;
            return serverSocket;
        }
    }

    // ------------- Bind the socket to the server Ip address -------------
    int BindTheSocket(SOCKET serverSocket, sockaddr_in& service) {
        service.sin_family = AF_INET;
        InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
        service.sin_port = htons(port);
        if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
            cout << "bind() failed: \n" << WSAGetLastError() << endl;
            closesocket(serverSocket);
            WSACleanup();
            return 0;
        }
        else {
            cout << "bind() is OK!" << endl;
            return 1;
        }
    }

    // ------------- Set the server to listen for incoming requests ---------------
    int Listen(SOCKET serverSocket) {
        if (listen(serverSocket, 1) == SOCKET_ERROR)
            cout << "listen(): Error listening on socket " << WSAGetLastError() << endl;
        else
            cout << "listen() is OK, I'm waiting for connections..." << endl;
        return 1;

    }

    //-------------  Set the server to accept incoming connections ----------------
    SOCKET AcceptConnection(SOCKET serverSocket, SOCKADDR_STORAGE& from, int& fromlen) {

        acceptSocket;
        fromlen = sizeof(from);
        acceptSocket = accept(serverSocket, (SOCKADDR*)&from, &fromlen);
        return acceptSocket;
    }

    //-------------  Get the connection details ----------------
        int GetConnectionInfo(SOCKADDR_STORAGE& from, int fromlen, char* hoststr, char* servstr) {

            int retval;
            retval = getnameinfo((SOCKADDR*)&from,
                fromlen,
                hoststr,
                NI_MAXHOST,
                servstr,
                NI_MAXSERV,
                NI_NUMERICHOST | NI_NUMERICSERV);

            if (retval != 0) {
                cout << "getnameinfo failed: " << retval << endl;
                WSACleanup();
                return -1;
            }
        }

        //-------------  Chat to the client ----------------
        int ChatToClient(SOCKET acceptSocket, char* buffer) {
            while (true) {
                int byteCount = recv(acceptSocket, buffer, 200, 0);
                if (byteCount > 0) {
                    cout << "Client sent: " << buffer << endl;

                    byteCount = send(acceptSocket, buffer, 200, 0);
                    if (byteCount == SOCKET_ERROR) {
                        printf("Server send error %ld.\n", WSAGetLastError());
                        return -1;
                    }

                    if (strcmp(buffer, "QUIT") == 0) {
                        cout << "Client has disconnected." << endl;
                        closesocket(acceptSocket);
                        WSACleanup();
                        exit(0);
                    }
                }
                else {
                    cout << "Error receiving message: " << WSAGetLastError() << endl;
                    return -1;
                }
            }
        }


};

#endif
