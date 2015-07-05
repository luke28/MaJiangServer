#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#pragma comment(lib,"ws2_32.lib")


#define SERVER_PORT 9210 //侦听端口
#define SERVER_IP "192.168.1.4"
#define PER 128

//发送文件大小的buf
int sendLength; 
//发送jpg的buf 
char sendJPG[200];
//发送位置的buf 
char sendLoc;

//获取给定文件长度 
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
	//这里先定死了
	sendLoc = '0';
	int iResult = 0;
	int ConnectSocket = 0;
struct hostent *host;
	//初始化套接字 
	
	//puts("1");
	//创建Socket,使用TCP协议
   ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (ConnectSocket == -1)
   {
        perror("socket创建出错！");
        return -1;
   }
   //puts("2");
   //构建服务器地址信息
   struct sockaddr_in saServer; //地址信息 
   saServer.sin_family = AF_INET; //地址家族
   saServer.sin_port = htons(SERVER_PORT); //注意转化为网络节序
   saServer.sin_addr.s_addr = inet_addr(SERVER_IP);
   //saServer.sin_addr = *((struct in_addr *)host->h_addr);
   //puts("3");
   //连接服务器，直到成功
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

   //发送位置
   iResult = send(ConnectSocket, &sendLoc, 1, 0);
   if (iResult == -1) {
        perror("send位置出错！");
        close(ConnectSocket);
        return -1;
    } 
   
   //发送文件大小
   sendLength =  getFileLen(fp); 
   iResult = send(ConnectSocket, (char*)(&sendLength), sizeof(sendLength), 0);
   if (iResult == -1) {
        perror("send大小出错！");
        close(ConnectSocket);
        return -1;
    } 
    
    //发送文件，如果发送失败就重新发送！ 
    int i;
	for (i=sendLength; i > 0 ; )
	{
		if (i <= PER)
		{
			fread(sendJPG, i, 1, fp);
			iResult = send(ConnectSocket, sendJPG , i, 0);
			//如果这段发送失败则重新发送 
			if (iResult == -1) {
        		perror("send文件出错！");
        		printf("i=%d\n",i); 
    		}
			else {
				i = 0;
			} 
		}
		else{
			fread(sendJPG,PER,1,fp);
			iResult = send(ConnectSocket, sendJPG , PER, 0);
			//如果这段发送失败则重新发送 
			if (iResult == -1) {
        		perror("send文件出错！");
        		printf("i=%d\n",i); 
    		}
			else {
				i -= PER;
			} 
		}
	}
	puts("发送图片成功！");
	
   	
	//接收数据
	iResult = recv(ConnectSocket, (char*)&data, sizeof(data), 0);
	if (iResult > 0) 
	{ 
		//情况1：成功接收到数据
		printf("番数=%d\n",data.fan);
		for(i=0;i<data.num;i++){
			printf("%d ",data.fanXing[i]);
		}
        puts("");
 
    } 
	else if (iResult == 0) 
	{
		//情况2：连接关闭
        printf("Connection closed...\n");
		return 0; 
	}
    else 
	{ 
		//情况3：接收发生错误
		perror("recv文件出错！"); 
        return 0; 
    }
	
   	
	//结束处理
	// 关闭套接字
    close(ConnectSocket);
	// 释放资源
    
    return 0; 
}

