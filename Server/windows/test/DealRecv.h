#ifndef _DEALRECV_H

#define _DEALRECV_H

#include <string>
#include "MyDB.h"

using std::string;

class DealRecv
{
public:
	DealRecv();
	~DealRecv();

	string deal(string rev);
private:
	int count;
	MyDB *mydb;

	string RepaceSpace(string str);
	string RepaceValue(string str, string values);
};

#endif