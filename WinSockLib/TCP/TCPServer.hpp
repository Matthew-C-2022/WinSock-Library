#pragma once
#include <iostream>
#include <winsock2.h>
#include <unordered_map>
#include <thread>
#include <functional>
#include "TCPSClient.hpp"
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


// I use a template here to define a packet handle callback It has to have 3 args, char* buf and int Client ID and int packlen;
template <typename T>
class TCPServer {
private:
    SOCKET serverSocket;
    sockaddr_in serverAddr; 
    std::unordered_map<int, SClient> AcceptedSockets;
    static int CIds;
    std::function<T> PacketHandleCallback;
public:
    inline void Init(const char* ip, int port, T Funcadd) {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        serverAddr.sin_family = PF_INET;
        inet_pton(AF_INET, ip, &(serverAddr.sin_addr));
        serverAddr.sin_port = htons(port);
        CIds = 0;
        PacketHandleCallback = Funcadd;
        bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    }

    inline void Listen() {
        listen(serverSocket, SOMAXCONN);
    }

    inline void Accept() {
        SOCKADDR_IN clientAddr;
        int clientSize = sizeof(clientAddr);
        auto CSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientSize);
        SClient TempAlloc;
        TempAlloc.ClientSock = CSocket;
        TempAlloc.ID = CIds;
        ZeroMemory(TempAlloc.RecvBuf, 4096);
        AcceptedSockets[CIds] = TempAlloc;
        std::thread CThread(&TCPServer::RecvHandle, this,CIds);
        CThread.detach();
        CIds++;
    }

    inline void Send(int ClientID, char* msg, int Len) {
        send(AcceptedSockets[ClientID].ClientSock, msg, Len, 0);
    }


    inline void SendAllExcluding(char* msg, int Len, SOCKET Exclude) {
        for (auto& Key : AcceptedSockets)
        {
            if (Key.second.ClientSock != Exclude)
            {
                send(Key.second.ClientSock, msg, Len, 0);
            }
        }
    }

    inline void SendAll(char* msg, int Len, SOCKET Exclude) {
        for (auto& Key : AcceptedSockets)
        {
            if (Key.second.ClientSock != Exclude)
            {
                send(Key.second.ClientSock, msg, Len, 0);
            }
        }
    }

    inline void SendAll(char* msg, int Len, int Exclude) {
        for (auto& Key : AcceptedSockets)
        {
            if (Key.first != Exclude)
            {
                send(Key.second.ClientSock, msg, Len, 0);
            }
        }
    }

    inline void SendAllExcluding(char* msg, int Len, int CIDExclude) {
        for (auto& Key : AcceptedSockets)
        {
            if (Key.first != CIDExclude)
            {
                send(Key.second.ClientSock, msg, Len, 0);
            }
        }
    }



    inline void Send(SOCKET clientSocket, char* msg, int Len) {
        send(clientSocket, msg, Len, 0);
    }


    inline char* Recv(SOCKET clientSocket) {
        char* buffer = new char[4096];
        ZeroMemory(buffer, 4096);
        int bytesReceived = recv(clientSocket, buffer, 4096, 0);
        return buffer;
    }


    inline char* Recv(int ClientID) {
        char* buffer = new char[4096];
        ZeroMemory(buffer, 4096);
        int bytesReceived = recv(AcceptedSockets[ClientID].ClientSock, buffer, 4096, 0);
        return buffer;
    }


    inline void RecvHandle(int ClientID) {
        auto Client = AcceptedSockets[ClientID];
        while (true)
        {
            Client.AmtRecv = recv(AcceptedSockets[ClientID].ClientSock, Client.RecvBuf, 4096, 0);
            if (Client.AmtRecv > 0)
            {
                PacketHandleCallback(Client.RecvBuf, ClientID, Client.AmtRecv);
            }
            if (Client.AmtRecv == SOCKET_ERROR) {
                closesocket(AcceptedSockets[ClientID].ClientSock);
                AcceptedSockets.erase(ClientID);
                break;
            }
        }
    }

    inline void Close() {
        closesocket(serverSocket);
        WSACleanup();
    }
};


template<typename T>
inline int TCPServer<T>::CIds;