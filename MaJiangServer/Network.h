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
	//�����ļ���С��buf
	int recLength;
	//����jpg��buf 
	char recJPG[200];
	//����λ�õ�buf 
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

