#include "stdafx.h"
#include "Network.h"
 //侦听端口
#pragma comment(lib,"ws2_32.lib")


Network::Network(){
	int iResult = 0;
	struct sockaddr_in saServer;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);//指定2.2的版本
	if (iResult != 0){
		printf("WSAStastup failed with error: %d\n", iResult);
		return;
	}

	//创建ListenSocket,使用TCP协议
	ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket() failed with error: %ld\n", WSAGetLastError());
		return;
	}

	//构建本地地址信息
	saServer.sin_family = AF_INET; //地址家族
	saServer.sin_port = htons(SERVER_PORT); //注意转化为网络字节序
	saServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //使用INADDR_ANY 指示任意地址 
	//绑定
	iResult = bind(ListenSocket, (struct sockaddr *)&saServer, sizeof(saServer));
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		return;
	}

	//监听
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		return;
	}

	printf("TCP server starting\n");
	// 接受客户端连接请求，返回连接套接字ClientSocket 
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		return;
	}

	// 在必须要监听套接字的情况下释放该套接字
	closesocket(ListenSocket);
	puts("已连接成功！");
}

bool Network::ReceiveJPG(){
	//接收1 
	int iResult = recv(ClientSocket, &recLoc, 1, 0);
	if (iResult > 0)
	{
		//情况1：成功接收到数据
		printf("位置为: %c\n", recLoc);

	}
	else if (iResult == 0)
	{
		//情况2：连接关闭
		printf("Connection closing...\n");
		return 0;
	}
	else
	{
		//情况3：接收发生错误
		printf("recv Loc failed with error: %d\n", WSAGetLastError());
		return 0;
	}

	//接收2 
	iResult = recv(ClientSocket, (char*)&recLength, sizeof(recLength), 0);
	if (iResult > 0)
	{
		//情况1：成功接收到数据
		printf("长度为: %d\n", recLength);

	}
	else if (iResult == 0)
	{
		//情况2：连接关闭
		printf("Connection closing...\n");
		return 0;
	}
	else
	{
		//情况3：接收发生错误
		printf("recv Length failed with error: %d\n", WSAGetLastError());
		return 0;
	}

	//接收3
	for (int i = recLength; i>0;){
		iResult = recv(ClientSocket, recJPG, PER, 0);
		if (iResult > 0)
		{
			//情况1：成功接收到数据
			fwrite(recJPG, iResult, 1, fp);
			i -= iResult;
		}
		else if (iResult == 0)
		{
			//情况2：连接关闭
			printf("Connection closing...\n");
			return 0;
		}
		else
		{
			//情况3：接收发生错误
			printf("recv JPG failed with error: %d\n", WSAGetLastError());
			return 0;
		}
	}
	return 1;

}


bool Network::init(){
	if (!(fp = fopen("receive.jpg", "wb+")))
	{
		printf("open receive.jpg error");
		return 0;
	}
	return 1;
}

bool Network::init(char* str){
	if (!(fp = fopen(str, "wb+")))
	{
		printf("open receive.jpg error");
		return 0;
	}
	return 1;
}


void Network::close(){
	fclose(fp);
}

Network::~Network(){
	WSACleanup();
	if (ClientSocket != INVALID_SOCKET)
		closesocket(ClientSocket);
}

bool Network::Send(){
	int iResult = send(ClientSocket, (char*)&data, sizeof(data), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		return 0;
	}
	puts("发送成功！");
	return 1;
}