#pragma once
#include <vector>
using namespace std;
class Identification
{
public:
	Identification();
	~Identification();
	//����ֵΪʶ���Ƿ�ɹ��������ж��Ƶ������Ƿ����
	//direction��ʾ�ļҺ��ƣ�0~3��ʾ�������ҼҺ���
	//vec���������ʶ�����Ƶ�id����
	//id��������¹���
	//1~9����0~8  1~9����9~17  1~9����18~26  ������������27~30  �з�����31~33
	bool getPai(int direction, vector<int>& vec);
};

