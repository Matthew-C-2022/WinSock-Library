#pragma once
#include <thread>
#include <iostream>
#include <winsock2.h>
#include <functional>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


// I use a template here to define a packet handle callback It has to have two args, char* buf and int packLen;
template <typename T>
class TCPClient {
private:
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    std::function<T> PacketHandleCallback;
public:
    inline void Init(const char* ip, int port, T funcptr) {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        serverAddr.sin_family = PF_INET;
        inet_pton(AF_INET, ip, &(serverAddr.sin_addr));
        serverAddr.sin_port = htons(port);
        PacketHandleCallback = funcptr;
    }

    inline char* Recv() {
        char* buffer = new char[4096];
        ZeroMemory(buffer, 4096);
        int bytesReceived = recv(clientSocket, buffer, 4096, 0);
        return buffer;
    }


    inline void RecvHandle() {
        while (true)
        {
            char* buffer = new char[4096];
            ZeroMemory(buffer, 4096);
            int bytesReceived = recv(clientSocket, buffer, 4096, 0);
            if (bytesReceived > 0)
            {
                printf("RECV BYTES\n");
                PacketHandleCallback(buffer, bytesReceived);
            }
            if (bytesReceived == SOCKET_ERROR) {
                printf("Closed Socket\n");
                closesocket(clientSocket);
                break;
            }
            delete[] buffer;
        }

    }


    inline void Connect() {
        connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
        std::thread RecvLop(&TCPClient::RecvHandle, this);
        RecvLop.detach();
    }

    inline void Send(char* msg, int len) {
        send(clientSocket, msg, len, 0);
    }

    inline void Close() {
        closesocket(clientSocket);
        WSACleanup();
    }
};