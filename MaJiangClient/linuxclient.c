#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#pragma comment(lib,"ws2_32.lib")


#define SERVER_PORT 9210 //�����˿�
#define SERVER_IP "192.168.1.4"
#define PER 128

//�����ļ���С��buf
int sendLength; 
//����jpg��buf 
char sendJPG[200];
//����λ�õ�buf 
char sendLoc;

//��ȡ�����ļ����� 
int getFileLen(FILE* fp){
	fseek(fp,0,SEEK_END);
	int end = ftell(fp);
	fseek(fp,0,0);
	return end;
}

struct Data{
	int fan,num;
	int fanXing[20];
}data; 

int main(){
	FILE* fp;
	fp=fopen("send.jpg","rb+");
	//�����ȶ�����
	sendLoc = '0';
	int iResult = 0;
	int ConnectSocket = 0;
struct hostent *host;
	//��ʼ���׽��� 
	
	//puts("1");
	//����Socket,ʹ��TCPЭ��
   ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (ConnectSocket == -1)
   {
        perror("socket��������");
        return -1;
   }
   //puts("2");
   //������������ַ��Ϣ
   struct sockaddr_in saServer; //��ַ��Ϣ 
   saServer.sin_family = AF_INET; //��ַ����
   saServer.sin_port = htons(SERVER_PORT); //ע��ת��Ϊ�������
   saServer.sin_addr.s_addr = inet_addr(SERVER_IP);
   //saServer.sin_addr = *((struct in_addr *)host->h_addr);
   //puts("3");
   //���ӷ�������ֱ���ɹ�
   while(1){
		iResult = connect(ConnectSocket, (struct sockaddr *)&saServer, sizeof(saServer));
   		if (iResult == -1) 
   		{	
   			close(ConnectSocket);
			ConnectSocket = -1;
			continue;
		}
		break;
   }
   //puts("4");
   //����λ��
   iResult = send(ConnectSocket, &sendLoc, 1, 0);
   if (iResult == -1) {
        perror("sendλ�ó���");
        close(ConnectSocket);
        return -1;
    } 
   
   //�����ļ���С
   sendLength =  getFileLen(fp); 
   iResult = send(ConnectSocket, (char*)(&sendLength), sizeof(sendLength), 0);
   if (iResult == -1) {
        perror("send��С����");
        close(ConnectSocket);
        return -1;
    } 
    
    //�����ļ����������ʧ�ܾ����·��ͣ� 
    int i;
	for (i=sendLength; i > 0 ; )
	{
		if (i <= PER)
		{
			fread(sendJPG, i, 1, fp);
			iResult = send(ConnectSocket, sendJPG , i, 0);
			//�����η���ʧ�������·��� 
			if (iResult == -1) {
        		perror("send�ļ�����");
        		printf("i=%d\n",i); 
    		}
			else {
				i = 0;
			} 
		}
		else{
			fread(sendJPG,PER,1,fp);
			iResult = send(ConnectSocket, sendJPG , PER, 0);
			//�����η���ʧ�������·��� 
			if (iResult == -1) {
        		perror("send�ļ�����");
        		printf("i=%d\n",i); 
    		}
			else {
				i -= PER;
			} 
		}
	}
	puts("����ͼƬ�ɹ���");
	
	// ���ݷ��ͽ���������shutdown()�����������ٷ������ݣ���ʱ�ͻ����Կ��Խ�������
    /*iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == -1) {
        printf("shutdown failed with error\n");
        close(ConnectSocket);
        return 1;
    } 
   	*/
   	
	//��������
	iResult = recv(ConnectSocket, (char*)&data, sizeof(data), 0);
	if (iResult > 0) 
	{ 
		//���1���ɹ����յ�����
		printf("����=%d\n",data.fan);
		for(i=0;i<data.num;i++){
			printf("%d ",data.fanXing[i]);
		}
        puts("");
 
    } 
	else if (iResult == 0) 
	{
		//���2�����ӹر�
        printf("Connection closed...\n");
		return 0; 
	}
    else 
	{ 
		//���3�����շ�������
		perror("recv�ļ�����"); 
        return 0; 
    }
	
   	
	//��������
	// �ر��׽���
    close(ConnectSocket);
	// �ͷ���Դ
    
    return 0; 
}

/* 
int main()
{
 //struct Data data;
   FILE * fp;
  
   WORD wVersionRequested;
   WSADATA wsaData;
   int ret;
   SOCKET sClient; //�����׽���
   struct sockaddr_in saServer; //��ַ��Ϣ
   char *ptr;
   BOOL fSuccess = TRUE;
 
   fseek(fp,0,SEEK_END);
   int end = ftell(fp);
   fseek(fp,0,0);
   //WinSock��ʼ��
   wVersionRequested = MAKEWORD(2, 2); //ϣ��ʹ�õ�WinSock DLL�İ汾
   ret = WSAStartup(wVersionRequested, &wsaData);
   if(ret!=0)
   {
        printf("WSAStartup() failed!\n");
        return -1;
   }
   //ȷ��WinSock DLL֧�ְ汾2.2
   if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2)
   {
        WSACleanup();
        printf("Invalid WinSock version!\n");
        return -1;
   }
   //����Socket,ʹ��TCPЭ��
   sClient = socket(AF_INET, SOCK_STREAM,0);
   if (sClient == INVALID_SOCKET)
   {
        WSACleanup();
        printf("socket() failed!\n");
        return -1;
   }
   //������������ַ��Ϣ
   saServer.sin_family = AF_INET; //��ַ����
   saServer.sin_port = htons(SERVER_PORT); //ע��ת��Ϊ�������
   saServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
   //saServer.sin_addr.S_un.S_addr = inet_addr("192.168.1.3");
   //���ӷ�����
   ret = connect(sClient, (struct sockaddr *)&saServer, sizeof(saServer));
   if (ret == SOCKET_ERROR)
   {
        printf("connect() failed!\n");
        closesocket(sClient); //�ر��׽���
        WSACleanup();
        return -1;
   }
 char sendMessage[2000];
 int sum=0;
 int times=0;
   while(end>0)
   {
    memset(data.sendMessage,'0',sizeof(data.sendMessage));
    fread(data.sendMessage,1024,1,fp);
    if(end>=1024) //ʣ��ܶ���Ϣ
    {
   		data.fin = 0;
   		data.length = 1024;  
    }
    else
    {
     data.fin = 1;
     data.length = end;
    }
    ret = send(sClient, (char *)&data,sizeof(struct Data),0); 
    if (ret == SOCKET_ERROR)
    {
  		printf("send() failed!\n");
    }
    else
   //printf("client info has been sent!");
    //sum+=1024;
    end -= 1024;
    //ret = send(sClient, (char *)&flag,1,0);
   }
	getchar();
   closesocket(sClient); //�ر��׽���
   WSACleanup();
}*/
