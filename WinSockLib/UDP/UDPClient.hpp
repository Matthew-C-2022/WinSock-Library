#pragma once
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")


class UDPClient {
private:
    SOCKET clientSocket;
    sockaddr_in serverAddr;
public:
    UDPClient(const char* ip, int port) {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

        serverAddr.sin_family = PF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ip);
        serverAddr.sin_port = htons(port);
    }

    void Send(char* msg, int Len) {
        sendto(clientSocket, msg, Len, 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    }

    char* Recv() {
        char* buffer = new char[4096];
        ZeroMemory(buffer, 4096);
        int serverAddrSize = sizeof(serverAddr);
        int bytesReceived = recvfrom(clientSocket, buffer, 4096, 0, (SOCKADDR*)&serverAddr, &serverAddrSize);
        return buffer;
    }

    void Close() {
        closesocket(clientSocket);
        WSACleanup();
    }
};