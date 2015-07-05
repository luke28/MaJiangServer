/*
 *	test.c  ---	Primary header file for 
 *				LCD Device Driver with Framebuffer
 *	(C)opyright 2004 Bit 920 Labs
 *
 *	Written by: Tangliting <dawn@bit.edu.cn>
 *	Created on: Sat. Mar 7 14:33:45 GMT +8:00 2004
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "lcd.h"
#include "ascii.lib"


#include "jpeglib.h"

#include "jerror.h"

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#pragma comment(lib,"ws2_32.lib")


#define SERVER_PORT 9210 //侦听端口
#define SERVER_IP "192.168.1.3"
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



FILE *hzkFile = NULL;

void DrawCharEN(short x, short y, unsigned char c, ColorType color)
{
	unsigned char codes[16];
	short i;

	for (i = 0; i < 16; i++)
		codes[i] = ascii_codes[c][i];
	
	fb_Text_8x16(x, y, codes, color);
}

void DrawCharCHS(int x, int y, unsigned char c[2], ColorType color)
{
	unsigned char codes[32];
	short i;
	unsigned char ch, cl;
	unsigned long offset;

	if (hzkFile == NULL)
	{
		printf("No Chinese Character Library opened.\n");

		exit(1);
	}

	ch = c[0];
	cl = c[1];

	offset = ((ch - 0xa1) * 94L + (cl - 0xa1)) * 32L;

	fseek(hzkFile, offset, SEEK_SET);
	fread(codes, 32, 1, hzkFile);	

	fb_Text_16x16(x, y, codes, color);
}







unsigned char fanxing[90][20]={"大四喜","大三元","绿一色","九莲宝灯","四杠","连七对","十三幺","清幺九",
"小四喜","小三元","字一色","四暗刻","一色双龙会","一色四同顺","一色四节高","一色四步高",
"三杠","混幺九","七对","七星不靠","全双刻","清一色","一色三节高","一色三同顺","全大",
"全中","全小","清龙","三色双龙会","一色三步高","全带五","三同刻","三暗刻","全不靠",
"组合龙","大于五","小于五","三风刻","花龙","推不倒","三色三同顺","三色三节高","无番和",
"妙手回春","海底捞月","杠上开花","抢杠和","碰碰和","混一色","三色三步高","五门齐","全求人",
"双暗杠","双箭刻","全带幺","不求人","双明杠","和绝张","箭刻","圈风刻","门风刻","门前清",
"平和","四归一","双同刻","双暗刻","暗杠","断幺","一般高","喜相逢","连六","老少副","明杠",
"幺九刻","缺一门","无字","边张","坎张","单钓将","自摸"};











void outasc(short x,short y,unsigned char * ascTxt)
{

int i;
ColorType color = SYS_WHITE;
    printf("text_16x8:\t\t");
	fb_Clear(SYS_BLACK);
	
	for (i = 0; i < strlen(ascTxt); i++)
	{
		DrawCharEN(x, y, ascTxt[i], color);
		x += 8;
	}
}

void outchs(short x,short y,unsigned char * chsTxt)
{

int i;
unsigned char bufferTxt[2];
ColorType color = SYS_WHITE;
    printf("text_16x16:\t");
	hzkFile = fopen("./hz16", "rb");
	
	for (i = 0; i < strlen(chsTxt); i += 2)
	{
		bufferTxt[0] = chsTxt[i];
		bufferTxt[1] = chsTxt[i + 1];
		DrawCharCHS(x, y, bufferTxt, color);
		x += 16;
	}
	fclose(hzkFile);

}

int min(int a,int b){
	if(a<b)
		return a;
	else 
		return b;
}

unsigned char chsfan[60];	
void output(int fanshu, int fanxingshu, int xiabiao[])
{
	short x=10,y=10;
	int i,j;
	
	//unsigned char * ascTxt="0000";
	//unsigned char * chsTxt="番番番番";
	unsigned char ascTxt[20]="0";
	unsigned char  chsTxt[20]="番";
	
	if(fanshu == -1)
	{
		outchs(10,10,"诈胡");
		return;
	}
	
	
	

	
	
	
	
	//itoa(fanshu,ascTxt,10);
	sprintf(ascTxt,"%d",fanshu);
	
       outasc(10,10,ascTxt);
	outchs(50,10,chsTxt);
	   
	//printf("Press any key to continue...\n");
	//getchar();
	

	strcpy(chsTxt,"番型：");
	outchs(10,40,chsTxt);
	x=64;
	y=40;
	for(i=0;i<fanxingshu;i+=3)
	{
		chsfan[0]=0;
		//printf("%s\n",chsfan);
		y=i*10+40;
		x=64;
		for(j=0;j<min(3,fanxingshu-i);j++)
		{
			// printf("%d\n",strlen(chsfan));
			strcat(chsfan,fanxing[xiabiao[i+j]]);
			if(j<min(3,fanxingshu-i)-1)
				strcat(chsfan,"，");
	        }
	 	printf("x=%d y=%d\n",x,y);
	 	outchs(x,y,chsfan);
	 	// puts("shfuwihefiuhwi");
	 }
	
            
	
}

int getKey()
{
	int fd=-1;
	int ret,i;
	unsigned int keys_value=0;
	fd = open("/dev/keys",0);
	if(fd<0)
	{
		printf("open /dev/KEYS error!\n");
		return -1;
	}
	for(;;)
	{
		ret=read(fd,&keys_value,sizeof(keys_value));
		if(ret<0)
		{
			printf("read err!\n");
			continue;
		}
		if(ret == 0)
			continue;
                switch(keys_value)
                {
                      case 0xEE:printf("S1 pressed!\n");return 1;
                      case 0xDE:printf("S2 pressed!\n");return 2;
                      case 0xBE:printf("S3 pressed!\n");return 3;
                      case 0x7E:printf("S4 pressed!\n");return 4;

                      case 0xED:printf("S5 pressed!\n");return 5;
                      case 0xDD:printf("S6 pressed!\n");return 6;
                      case 0xBD:printf("S7 pressed!\n");return 7;
                      case 0x7D:printf("S8 pressed!\n");return 8;

		      		  case 0xEB:printf("S9 pressed!\n");return 9;
                      case 0xDB:printf("S10 pressed!\n");return 10;
                      case 0xBB:printf("S11 pressed!\n");return 11;
                      case 0x7B:printf("S12 pressed!\n");return 12;

                      case 0xE7:printf("S13 pressed!\n");return 13;
                      case 0xD7:printf("S14 pressed!\n");return 14;
                      case 0xB7:printf("S15 pressed!\n");return 15;
                      case 0x77:printf("S16 pressed!\n");return 16;
		}
		keys_value=0;
	}
	
}

//int fan[5] = {43,43,25,13,0};
//int shu[5] ={4,4,2,4,1};
//int xing[5][10]={{22,28,63,69},{22,28,63,69},{19,75},{51,55,59,74},{0}};
//int fan,shu;
//int xing[25];



int  main()
{
	short x, y;
	ColorType color = SYS_WHITE;
	short colorPage;
	short endFlag;
	unsigned char * ascTxt = "Motolora MX1";
	unsigned char * chsTxt = "嵌入式开发板";
	unsigned char bufferTxt[2];
	
	short x_max, y_max;
	ColorType color_max;

	if (fb_Init() == -1)
	{
		printf("Initialize Framebuffer LCD failed.\n");
		exit(1);
	}
	
	//int fanshu=124;
	//int fanxingshu=7;
	//int xiabiao[7]={1,5,7,9,11,14,18};
	int T;
	//Camera Init
	camera_init(640, 480);
	
	
	//Network Init
	FILE* fp;
	
	//这里先定死了
	sendLoc = '0';
	int iResult = 0;
	int ConnectSocket = 0;
	struct hostent *host;
	//初始化套接字 
	
	//创建Socket,使用TCP协议
   	ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
   	if (ConnectSocket == -1)
   	{
        	perror("socket创建出错！");
       	 return -1;
   	}
   	
   	//构建服务器地址信息
   	struct sockaddr_in saServer; //地址信息 
   	saServer.sin_family = AF_INET; //地址家族
   	saServer.sin_port = htons(SERVER_PORT); //注意转化为网络节序
   	saServer.sin_addr.s_addr = inet_addr(SERVER_IP);


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
	
	
	
	
	int num;
	while(num = getKey()!=-1){
	
		capture();
		capture();
		
		//send
		fp=fopen("video.jpg","rb+");
		
		//发送位置
		sendLoc = (char)((int)('0') + num);
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
		fclose(fp);
		
		//receive
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
		
		
		fb_Init();
		output(data.fan,data.num,data.fanXing);
		
		
		
		
		
		
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		T = 2000000000;
		while(T--);
		
	}
	
	fb_Release();
	//结束处理
	// 关闭套接字
    	close(ConnectSocket);
	return 0;
}





