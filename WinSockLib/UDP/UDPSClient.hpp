#pragma once
#include <iostream>
#include <winsock2.h>
#include <unordered_map>
#pragma comment(lib, "ws2_32.lib")

class SClient
{
public:
	SOCKADDR_IN ClientSock;
	int clientsize;
	int ID;
	int UID;
	char RecvBuf[4096];
};

