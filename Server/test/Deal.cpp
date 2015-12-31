#include <map>
#include "Deal.h"
#include "MyDB.h"

using std::map;

Deal::Deal()
{
}

Deal::~Deal()
{
}

string Deal::deal(string str, int & npos)
{
	char which = str.at(0);

	switch (which)
	{
	case 's':fun = &Deal::select;break;
	case 'i':fun = &Deal::insert;break;
	case 'd':fun = &Deal::del;break;
	case 'u':fun = &Deal::update;break;
	default:
		return string("");
	}

	str.erase(0, 2);					//like i;2;
	int n = 0, pos = str.find(';');
	string sub = str.substr(0, pos);
	map<string, string> dict;

	n = stoi(sub);
	str.erase(0, pos + 1);

	while (n-- > 0)
	{
	}

	return string();
}

string Deal::insert(string str, int fpos, int & npos)
{
	return string();
}

string Deal::select(string str, int fpos, int & npos)
{
	return string();
}

string Deal::del(string str, int fpos, int & npos)
{
	return string();
}

string Deal::update(string str, int fpos, int & npos)
{
	return string();
}