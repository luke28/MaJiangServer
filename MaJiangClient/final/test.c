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


#define SERVER_PORT 9210 //�����˿�
#define SERVER_IP "192.168.1.3"
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







unsigned char fanxing[90][20]={"����ϲ","����Ԫ","��һɫ","��������","�ĸ�","���߶�","ʮ����","���۾�",
"С��ϲ","С��Ԫ","��һɫ","�İ���","һɫ˫����","һɫ��ͬ˳","һɫ�Ľڸ�","һɫ�Ĳ���",
"����","���۾�","�߶�","���ǲ���","ȫ˫��","��һɫ","һɫ���ڸ�","һɫ��ͬ˳","ȫ��",
"ȫ��","ȫС","����","��ɫ˫����","һɫ������","ȫ����","��ͬ��","������","ȫ����",
"�����","������","С����","�����","����","�Ʋ���","��ɫ��ͬ˳","��ɫ���ڸ�","�޷���",
"���ֻش�","��������","���Ͽ���","���ܺ�","������","��һɫ","��ɫ������","������","ȫ����",
"˫����","˫����","ȫ����","������","˫����","�;���","����","Ȧ���","�ŷ��","��ǰ��",
"ƽ��","�Ĺ�һ","˫ͬ��","˫����","����","����","һ���","ϲ���","����","���ٸ�","����",
"�۾ſ�","ȱһ��","����","����","����","������","����"};











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
	//unsigned char * chsTxt="��������";
	unsigned char ascTxt[20]="0";
	unsigned char  chsTxt[20]="��";
	
	if(fanshu == -1)
	{
		outchs(10,10,"թ��");
		return;
	}
	
	
	

	
	
	
	
	//itoa(fanshu,ascTxt,10);
	sprintf(ascTxt,"%d",fanshu);
	
       outasc(10,10,ascTxt);
	outchs(50,10,chsTxt);
	   
	//printf("Press any key to continue...\n");
	//getchar();
	

	strcpy(chsTxt,"���ͣ�");
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
				strcat(chsfan,"��");
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
	unsigned char * chsTxt = "Ƕ��ʽ������";
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
	
	//�����ȶ�����
	sendLoc = '0';
	int iResult = 0;
	int ConnectSocket = 0;
	struct hostent *host;
	//��ʼ���׽��� 
	
	//����Socket,ʹ��TCPЭ��
   	ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
   	if (ConnectSocket == -1)
   	{
        	perror("socket��������");
       	 return -1;
   	}
   	
   	//������������ַ��Ϣ
   	struct sockaddr_in saServer; //��ַ��Ϣ 
   	saServer.sin_family = AF_INET; //��ַ����
   	saServer.sin_port = htons(SERVER_PORT); //ע��ת��Ϊ�������
   	saServer.sin_addr.s_addr = inet_addr(SERVER_IP);


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
	
	
	
	
	int num;
	while(num = getKey()!=-1){
	
		capture();
		capture();
		
		//send
		fp=fopen("video.jpg","rb+");
		
		//����λ��
		sendLoc = (char)((int)('0') + num);
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
		fclose(fp);
		
		//receive
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
	//��������
	// �ر��׽���
    	close(ConnectSocket);
	return 0;
}





