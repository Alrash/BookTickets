#ifndef _MYDB_H

#define _MYDB_H

#include <winsock.h>
#include <mysql.h>
#include <string>

//#pragma comment(lib,"wsock32.lib")
//#pragma comment(lib,"libmysql.lib")

using std::string;

const int NUM = 10;

class MyDB
{
public:
	MyDB();
	~MyDB();

	int init();
	string query(string sql, string which);
	int close();
private:
	MYSQL *mysql;
	MYSQL_RES *result;		//½á¹û¼¯
	MYSQL_FIELD *fd;
	MYSQL_ROW sql_row;

	string select(string sql);
	string others(string sql);
	string book(string sql);
	string reg(string sql);

	string columns[NUM];
};

#endif
