#pragma once
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

class SClient
{
public:
	SOCKET ClientSock;
	int ID;
	int AmtRecv;
	char RecvBuf[4096];
};

