#pragma once
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

class UDPServer {
private:
    SOCKET serverSocket;
    sockaddr_in serverAddr;
public:
    void Init(const char* ip, int port) {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        serverSocket = socket(PF_INET, SOCK_DGRAM, 0);

        serverAddr.sin_family = PF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ip);
        serverAddr.sin_port = htons(port);

        bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    }


    void Send(char* msg,int Len , SOCKADDR_IN* clientAddr, int clientSize) {
        sendto(serverSocket, msg, Len, 0, (SOCKADDR*)clientAddr, clientSize);
    }

    char* Recv(SOCKADDR_IN* clientAddr, int* clientSize) {
        char* buffer = new char[4096];
        ZeroMemory(buffer, 4096);
        int bytesReceived = recvfrom(serverSocket, buffer, 4096, 0, (SOCKADDR*)clientAddr, clientSize);
        return buffer;
    }

    void Close() {
        closesocket(serverSocket);
        WSACleanup();
    }
};