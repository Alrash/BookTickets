#ifndef _DEAL_H

#define _DEAL_H

#include <string>

using std::string;

class Deal
{
public:
	Deal();
	~Deal();

	string deal(string str, int &npos);
private:
	string (Deal::*fun)(string str, int fpos, int &npos);

	string insert(string str, int fpos, int &npos);
	string select(string str, int fpos, int &npos);
	string del(string str, int fpos, int &npos);
	string update(string str, int fpos, int &npos);
};

#endif