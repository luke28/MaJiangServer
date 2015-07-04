#pragma once
#include "Define_Struct.h"


class Network
{
	static const int PER = 128;
public:
	Data data;
protected:
	FILE* fp;
	WSADATA wsaData;
	SOCKET ListenSocket, ClientSocket;

protected:
	//接收文件大小的buf
	int recLength;
	//接收jpg的buf 
	char recJPG[200];
	//接收位置的buf 
	char recLoc;

public:
	Network();
	~Network();
	bool init();
	bool init(char* str);
	bool ReceiveJPG();
	bool Send();
	void close();
};

