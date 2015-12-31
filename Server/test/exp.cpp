#include <iostream>
#include <string>
#include "MyDB.h"

using namespace std;

int main()
{
	string sql;
	MyDB mydb;

	mydb.init();
	sql = "select * from Area";
	cout << sql << endl;
	cout << mydb.query(sql, "s") << endl << endl;

	sql = "insert into Area values('011', 'lovelive')";
	cout << sql  << " and show changes " << endl;
	cout << mydb.query(sql, "i") << endl;
	cout << mydb.query("select * from Area", "s") << endl << endl;

	sql = "delete from Area where AreaId = '011'";
	cout << sql << " and show changes " << endl;
	cout << mydb.query(sql, "i") << endl;
	cout << mydb.query("select * from Area", "s") << endl << endl;
	
	return 0;
}