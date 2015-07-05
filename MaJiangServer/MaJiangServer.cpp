// MaJiangServer.cpp : �������̨Ӧ�ó������ڵ㡣
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
	
	
	//��ѭ��
	while (1){
		if (network.init() == false)
		{
			puts("��ʼ��ʧ�ܣ�");
			return -1;
		}
		if (network.ReceiveJPG() == 0){
			puts("����ͼƬʧ�ܣ�");
			return -1;
		}
		network.close();

		//������
		vector<int> in;
		for (int i = 0; i < 9; i++){
			in.push_back(i);
		}
		for (int i = 0; i < 3; i++){
			in.push_back(2);
		}
		in.push_back(10);
		in.push_back(10);
		//����ʶ��ģ��
		//idf.getPai(data.number, in);
		
		//�����㷬ģ��
		if (suanfan.init(in) != 0){
			puts("��ʼ���㷬ģ�����");
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
		//��������
		if (network.Send() == 0){
			puts("���ͷ�����Ϣ����");
		}
		getchar();
		
	}
	
	

	return 0;
}
