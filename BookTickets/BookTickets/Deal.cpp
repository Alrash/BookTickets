#include "stdafx.h"
#include "Deal.h"

Deal::Deal()
{
}

Deal::~Deal()
{
}

string Deal::RepaceSpace(string str)
{//�滻�ո񣬷�ֹ����ض�
	int pos;

	while ((pos = str.find(" ")) != str.npos)
		str.replace(pos, 1, "&s");

	return str;
}