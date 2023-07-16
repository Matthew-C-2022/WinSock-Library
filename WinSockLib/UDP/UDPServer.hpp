#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <winsock2.h>
#include "UDPSClient.hpp"
#pragma comment(lib, "ws2_32.lib")

// This has a similar packet handler callback as TCP but this one also has the UDPSClient class as the 4th arg
template <typename T>
class UDPServer {
private:
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    std::unordered_map<int, SClient> AcceptedSockets;
    static int CIds;
    std::function<T> PacketHandleCallback;
public:
    inline void Init(const char* ip, int port, T Funcadd) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
            return;
        }

        serverSocket = socket(PF_INET, SOCK_DGRAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }

        serverAddr.sin_family = PF_INET;
        if (inet_pton(AF_INET, ip, &(serverAddr.sin_addr)) <= 0) {
            std::cerr << "inet_pton failed with error: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return;
        }

        serverAddr.sin_port = htons(port);
        PacketHandleCallback = Funcadd;

        if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return;
        }
    }

    inline void Send(char* msg,int Len , SOCKADDR_IN* clientAddr, int clientSize) {
        sendto(serverSocket, msg, Len, 0, (SOCKADDR*)clientAddr, clientSize);
    }


    inline void sendall(char* Msg, int Len)
    {
        for (auto& var : AcceptedSockets)
        {
            Send(Msg, Len, &var.second.ClientSock, var.second.clientsize);
        }
    }


    inline void sendallExcluding(char* Msg, int Len, int ClientID)
    {
        for (auto& var : AcceptedSockets)
        {
            if(var.second.ID != ClientID)
                Send(Msg, Len, &var.second.ClientSock, var.second.clientsize);
        }
    }


    inline void sendallExcludingUID(char* Msg, int Len, int Uid)
    {
        for (auto& var : AcceptedSockets)
        {
            if (var.second.UID != Uid)
                Send(Msg, Len, &var.second.ClientSock, var.second.clientsize);
        }
    }


    inline void sendallExcluding(char* Msg, int Len, SOCKADDR_IN* Clientadd)
    {
        for (auto& var : AcceptedSockets)
        {
            if(var.second.ClientSock != Clientadd)
                Send(Msg, Len, &var.second.ClientSock, var.second.clientsize);
        }
    }


    inline void Recv() {
        SOCKADDR_IN clientAddr;
        int clientSize = sizeof(clientAddr);
        char* buffer = new char[4096];
        ZeroMemory(buffer, 4096);
        int bytesReceived = recvfrom(serverSocket, buffer, 4096, 0, (SOCKADDR*)&clientAddr, &clientSize);

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "recvfrom failed with error: " << WSAGetLastError() << std::endl;
            // Handle error...
            return;
        }

        int clientId = clientAddr.sin_addr.S_un.S_addr + clientAddr.sin_port;

        if (AcceptedSockets.find(clientId) == AcceptedSockets.end()) {
            SClient newClient;
            newClient.clientsize = clientSize;
            newClient.ClientSock = clientAddr;
            newClient.UID = clientId;
            newClient.ID = CIds;
            AcceptedSockets[clientId] = newClient;
        }

        if (bytesReceived > 0)
            PacketHandleCallback(buffer, clientId, bytesReceived, AcceptedSockets[clientId]);
    }


    inline void Close() {
        closesocket(serverSocket);
        WSACleanup();
    }
};

template<typename T>
inline int UDPServer<T>::CIds;