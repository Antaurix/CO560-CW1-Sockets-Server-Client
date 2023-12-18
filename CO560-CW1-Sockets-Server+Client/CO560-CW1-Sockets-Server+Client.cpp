// main.cpp
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread>
#include "Comms.h"
#include "Client.h"
#include "Server.h"

// Define the port number and the buffer size
#define PORT 55555
#define BUFFER_SIZE 200

// Declare the client and server functions
void client(int port, int bufferSize);
void server(int port, int bufferSize);

int main()
{
    // Create a thread for the client
    std::thread clientThread(client, PORT, BUFFER_SIZE);

    // Create a thread for the server
    std::thread serverThread(server, PORT, BUFFER_SIZE);

    // Wait for the threads to finish
    clientThread.join();
    serverThread.join();

    system("pause");
    return 0;
}

// Define the client function
void client(int port, int bufferSize) {

    // Create a Client object
    Client client;

    client.CheckSocket(client.wsaData);
    client.CreateClientSocket();
    client.ConnectToServer(client.clientSocket, client.clientService);

    // Chat to the server
    char buffer[200];
    client.ChatToServer(client.clientSocket, buffer, bufferSize);

    // Close the socket and clean up
    closesocket(client.clientSocket);
    WSACleanup();
}

// Define the server function
void server(int port, int bufferSize) {
    // Create a Server object
    Server server;

    server.CheckSocketServer(server.wsaData);
    server.CreateServerSocket();
    server.BindTheSocket(server.serverSocket, server.service);
    server.Listen(server.serverSocket);
    server.AcceptConnection(server.serverSocket, server.from, server.fromlen);

    // Get the connection details
    char hoststr[NI_MAXHOST];
    char servstr[NI_MAXSERV];
    server.GetConnectionInfo(server.from,server.fromlen,hoststr,servstr);
    cout << " Connection details - Host: " << hoststr << ", Service: " << servstr << endl;

    // Chat to the client
    char buffer[200];
    server.ChatToClient(server.acceptSocket, buffer);


    // Close the socket and clean up
    closesocket(server.serverSocket);
    WSACleanup();
}
