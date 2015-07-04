#pragma once
#include <vector>
using namespace std;
class Identification
{
public:
	Identification();
	~Identification();
	//返回值为识别是否成功，不用判断牌的张数是否合理
	//direction表示哪家胡牌，0~3表示上下左右家胡牌
	//vec里是输出的识别后的牌的id序列
	//id编号是如下规则：
	//1~9万是0~8  1~9条是9~17  1~9饼是18~26  东南西北风是27~30  中发白是31~33
	bool getPai(int direction, vector<int>& vec);
};

