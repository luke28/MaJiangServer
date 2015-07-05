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

FILE *hzkFile = NULL;

unsigned char fanxing[90][20]={"咋呼""大四喜","大三元","绿一色","九莲宝灯","四杠","连七对","十三幺","清幺九",
"小四喜","小三元","字一色","四暗刻","一色双龙会","一色四同顺","一色四节高","一色四步高",
"三杠","混幺九","七对","七星不靠","全双刻","清一色","一色三节高","一色三同顺","全大",
"全中","全小","清龙","三色双龙会","一色三步高","全带五","三同刻","三暗刻","全不靠",
"组合龙","大于五","小于五","三风刻","花龙","推不倒","三色三同顺","三色三节高","无番和",
"妙手回春","海底捞月","杠上开花","抢杠和","碰碰和","混一色","三色三步高","五门齐","全求人",
"双暗杠","双箭刻","全带幺","不求人","双明杠","和绝张","箭刻","圈风刻","门风刻","门前清",
"平和","四归一","双同刻","双暗刻","暗杠","断幺","一般高","喜相逢","连六","老少副","明杠",
"幺九刻","缺一门","无字","边张","坎张","单钓将","自摸"};



void DrawCharEN(short x, short y, unsigned char c, ColorType color)
{
	unsigned char codes[5];
	short i;

	for (i = 0; i < 5; i++)
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



void output(int fanshu, int fanxingshu, int xiabiao[])
{
    short x=10,y=10;
	int i,j;
	
	unsigned char * ascTxt="0000";
	unsigned char * chsTxt="番番番番";
	//unsigned char ascTxt[20]="0";
	//unsigned char  chsTxt[20]="番";
	
	outasc(10,10,ascTxt);
	outchs(10,40,chsTxt);
	
	

	
	
	
	/*
	//itoa(fanshu,ascTxt,10);
	sprintf(ascTxt,"%d",fanshu);
    outasc(10,10,ascTxt);
	outchs(10,40,chsTxt);
	   
	printf("Press any key to continue...\n");
	getchar();
	

	strcpy(chsTxt,"番型：");
	outchs(10,70,chsTxt);
	x=64;
	y=70;
	for(i=0;i<fanxingshu;i++)
	{
	for(j=0;j<4;j++)
	   {
	   outchs(x,y,fanxing[xiabiao[i]]);
        x+=20;	
	   }
	   y+=30;
	   x=64;
	}
	
            */
	
}

int main()
{
if (fb_Init() == -1)
	{
		printf("Initialize Framebuffer LCD failed.\n");
		exit(1);
	}

int fanshu=124;
int fanxingshu=5;
int xiabiao[5]={5,7,13,17,19};

  output(fanshu,fanxingshu,xiabiao);
  




return 0;

}
