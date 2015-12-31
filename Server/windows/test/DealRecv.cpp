#include "DealRecv.h"
#include <iostream>

using std::stoi;

DealRecv::DealRecv()
{
	count = 0;
	mydb = new MyDB();
}

DealRecv::~DealRecv()
{
	delete mydb;
}

string DealRecv::deal(string rev)
{
	int pos = 0, npos = rev.find(";");
	string str("");
	string tmp;

	rev = RepaceSpace(rev);
	count = atoi(rev.c_str());
	rev.erase(pos, npos + 1);

	mydb->init();
	while (count-- > 0)
	{
		pos = rev.find(";");
		npos = rev.find(";", pos + 1);

		if (npos == rev.npos)
			return "Fail";

		//substr(pos, len);
		std::cout << rev.substr(pos + 1, npos - pos - 1) << " " << rev.substr(0, pos) << "\n";
		tmp = mydb->query(rev.substr(pos + 1, npos - pos - 1), rev.substr(0, pos));

		if (!tmp.compare("Fail"))
			return "Fail";
		if (!tmp.compare("OK") && str.find("OK") != str.npos)
			tmp = "";

		if (rev.substr(0, pos) != "r" && rev.substr(0, pos) != "b")
			str += tmp;
		else
		{
			rev = this->RepaceValue(rev, tmp);
			if (rev.substr(0, pos) == "b")
				str += tmp + "|";
		}

		rev.erase(0, npos + 1);
	}
	mydb->close();

	return str;
}

string DealRecv::RepaceSpace(string str)
{
	int pos;
	
	while ((pos = str.find("&s")) != str.npos)
		str.replace(pos, 2, " ");

	return str;
}

string DealRecv::RepaceValue(string str, string values)
{
	int pos;
	
	while ((pos = str.find("&r")) != str.npos)
		str.replace(pos, 2, values);
	
	return str;
}
