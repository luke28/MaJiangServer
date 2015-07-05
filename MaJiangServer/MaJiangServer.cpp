// MaJiangServer.cpp : 定义控制台应用程序的入口点。
//
#pragma once
#include "MaJiangServer.h"
#include "stdafx.h"
#include "Network.h"
#include "SuanFan.h"
#include "Identification.h"

#include <stdio.h>
#include <string.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

Network network;
SuanFan suanfan;
Identification idf;
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	
	
	//主循环
	while (1){
		if (network.init() == false)
		{
			puts("初始化失败！");
			return -1;
		}
		if (network.ReceiveJPG() == 0){
			puts("接收图片失败！");
			return -1;
		}
		network.close();

		//测试用
		vector<int> in;
		for (int i = 0; i < 9; i++){
			in.push_back(i);
		}
		for (int i = 0; i < 3; i++){
			in.push_back(2);
		}
		in.push_back(10);
		in.push_back(10);
		//调用识别模块
		//idf.getPai(data.number, in);
		
		//调用算番模块
		if (suanfan.init(in) != 0){
			puts("初始化算番模块出错！");
			getchar();
			return -1;
		}
		pair<int, vector<int> > val = suanfan.getFanShu();
		network.data.fan = val.first;
		network.data.num = val.second.size();
		for (int i = 0; i < network.data.num; i++){
			network.data.fanXing[i] = val.second[i];
		}
		//getchar();
		//发送数据
		if (network.Send() == 0){
			puts("发送番数信息出错！");
		}
		getchar();
		
	}
	
	

	return 0;
}
