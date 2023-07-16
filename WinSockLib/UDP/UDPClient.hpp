#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <winsock2.h>
#include <functional>
#include <ws2tcpip.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

template <typename T>
class UDPClient {
private:
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    int serverAddrSize = sizeof(serverAddr);
    std::function<T> PacketHandleCallback;
public:
    inline void init(const char* ip, int port, T funcptr) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
            return;
        }

        clientSocket = socket(PF_INET, SOCK_DGRAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }

        serverAddr.sin_family = PF_INET;
        if (inet_pton(AF_INET, ip, &(serverAddr.sin_addr)) <= 0) {
            std::cerr << "inet_pton failed with error: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return;
        }

        serverAddr.sin_port = htons(port);
        PacketHandleCallback = funcptr;
        std::thread RecvLop(&UDPClient::RecvHandle, this);
        RecvLop.detach();
    }

    inline void Send(char* msg, int Len) {
        sendto(clientSocket, msg, Len, 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    }

    inline void RecvHandle() {
        while (true)
        {
            char* buffer = new char[4096];
            ZeroMemory(buffer, 4096);
            int bytesReceived = recvfrom(clientSocket, buffer, 4096, 0, (SOCKADDR*)&serverAddr, &serverAddrSize);
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

    inline char* Recv() {
        char* buffer = new char[4096];
        ZeroMemory(buffer, 4096);
        int bytesReceived = recvfrom(clientSocket, buffer, 4096, 0, (SOCKADDR*)&serverAddr, &serverAddrSize);
        return buffer;
    }

    inline void Close() {
        closesocket(clientSocket);
        WSACleanup();
    }
};