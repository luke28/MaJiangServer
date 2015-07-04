#include "stdafx.h"
#include "Network.h"
 //�����˿�
#pragma comment(lib,"ws2_32.lib")


Network::Network(){
	int iResult = 0;
	struct sockaddr_in saServer;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);//ָ��2.2�İ汾
	if (iResult != 0){
		printf("WSAStastup failed with error: %d\n", iResult);
		return;
	}

	//����ListenSocket,ʹ��TCPЭ��
	ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket() failed with error: %ld\n", WSAGetLastError());
		return;
	}

	//�������ص�ַ��Ϣ
	saServer.sin_family = AF_INET; //��ַ����
	saServer.sin_port = htons(SERVER_PORT); //ע��ת��Ϊ�����ֽ���
	saServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //ʹ��INADDR_ANY ָʾ�����ַ 
	//��
	iResult = bind(ListenSocket, (struct sockaddr *)&saServer, sizeof(saServer));
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		return;
	}

	//����
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		return;
	}

	printf("TCP server starting\n");
	// ���ܿͻ����������󣬷��������׽���ClientSocket 
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		return;
	}

	// �ڱ���Ҫ�����׽��ֵ�������ͷŸ��׽���
	closesocket(ListenSocket);
	puts("�����ӳɹ���");
}

bool Network::ReceiveJPG(){
	//����1 
	int iResult = recv(ClientSocket, &recLoc, 1, 0);
	if (iResult > 0)
	{
		//���1���ɹ����յ�����
		printf("λ��Ϊ: %c\n", recLoc);

	}
	else if (iResult == 0)
	{
		//���2�����ӹر�
		printf("Connection closing...\n");
		return 0;
	}
	else
	{
		//���3�����շ�������
		printf("recv Loc failed with error: %d\n", WSAGetLastError());
		return 0;
	}

	//����2 
	iResult = recv(ClientSocket, (char*)&recLength, sizeof(recLength), 0);
	if (iResult > 0)
	{
		//���1���ɹ����յ�����
		printf("����Ϊ: %d\n", recLength);

	}
	else if (iResult == 0)
	{
		//���2�����ӹر�
		printf("Connection closing...\n");
		return 0;
	}
	else
	{
		//���3�����շ�������
		printf("recv Length failed with error: %d\n", WSAGetLastError());
		return 0;
	}

	//����3
	for (int i = recLength; i>0;){
		iResult = recv(ClientSocket, recJPG, PER, 0);
		if (iResult > 0)
		{
			//���1���ɹ����յ�����
			fwrite(recJPG, iResult, 1, fp);
			i -= iResult;
		}
		else if (iResult == 0)
		{
			//���2�����ӹر�
			printf("Connection closing...\n");
			return 0;
		}
		else
		{
			//���3�����շ�������
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
	puts("���ͳɹ���");
	return 1;
}