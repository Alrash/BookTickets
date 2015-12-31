#include "MyDB.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstdlib>

using std::endl;
using std::cout;

MyDB::MyDB()
{
	mysql = mysql_init(NULL);
}

MyDB::~MyDB()
{
}

int MyDB::init()
{
	if (( mysql = mysql_real_connect(mysql, "localhost", "root", "cm2544843034LL", "BookTickets", 3306, NULL, 0)) == NULL)
		std::cout << mysql_error(mysql) << "\n";
	return 0;
}

string MyDB::query(string sql, string which)
{
	string str;

	if (!which.compare("s"))
		str = select(sql);
	else if (!which.compare("r"))
		str = reg(sql);
	else if (!which.compare("b"))
		str = book(sql);
	else
		str = others(sql);

	return str;
}

string MyDB::select(string sql)
{
	mysql_query(mysql, "set names gbk");					//编码问题
	string str;

	if (!mysql_real_query(mysql, sql.c_str(), sql.size()))	//执行成功返回0
	{
		if ((result = mysql_store_result(mysql)) != NULL)	//获取结果集
		{
			int num = mysql_num_fields(result);

			while (sql_row = mysql_fetch_row(result))		//获取具体的数据
			{
				for (int i = 0;i < num;i++)
				{
					if(sql_row[i] != NULL)
						str += sql_row[i];
					str += "|";
				}
				str.at(str.size() - 1) = ';';
			}
			mysql_free_result(result);						// 释放结果集的内存
		}
		else
			str = "Fail";
	}
	else
		str = "Fail";

	return str;
}

string MyDB::others(string sql)
{
	string str;

	//mysql_query(mysql, "set names gbk");					//编码问题
	
	if (!mysql_real_query(mysql, sql.c_str(), sql.size()))	//执行成功返回0
		if (mysql_affected_rows(mysql) > 0)
			str = "OK";
		else
			str = "Fail";
	else
		str = "Fail";
	
	return str;
}

string MyDB::book(string sql)
{//sql->非sql语句，而是想不出来词，直接沿用了╮(￣▽￣)╭
	string str("select MAX(TicketsId) from Tickets where SUBSTR(TicketsId, 12, 3) = '");

	str += sql.substr(0, sql.find("|"));
	str += "'";

	std::cout << str << "\n";

	MYSQL *tmysql;
	MYSQL_RES *tresult;
	MYSQL_ROW tsql_row;

	tmysql = mysql_init(NULL);

	if ((tmysql = mysql_real_connect(tmysql, "localhost", "root", "cm2544843034LL", "BookTickets", 3306, NULL, 0)) == NULL)
	{
		std::cout << mysql_error(mysql) << "\n";
		return "Fail";
	}

	mysql_query(tmysql, "set names gbk");					//编码问题

	if (!mysql_real_query(tmysql, str.c_str(), str.size()))	//执行成功返回0
	{
		if ((tresult = mysql_store_result(tmysql)) != NULL)	//获取结果集
		{
			tsql_row = mysql_fetch_row(tresult);
		//	std::cout << tsql_row[0] << "\n";
			if (tsql_row == NULL)
				str = "";
			else
				str = tsql_row[0];
			std::cout << "OK\n";
			mysql_free_result(tresult);						// 释放结果集的内存
		}
		else
			str = "Fail";
	}
	else
		str = "Fail";

	std::cout << "OK\n";
	if (str == "Fail")
		return str;

	time_t tt = time(NULL);
	char s[20];
	//std::cout << "OK\n";
	tm *t = localtime(&tt);

	srand((unsigned)time(NULL));
	//std::cout << "OK\n";

	if (str != "")
	{
		sprintf(s, "%.4d%.2d%.2d%.2d%s%s%.3d",
			rand() % 10000, t->tm_year % 100, t->tm_mon + 1, t->tm_mday, sql.substr(sql.find("|") + 1).c_str(),
			sql.substr(0, sql.find("|")).c_str(), (int)strtol(str.substr(15, 3).c_str(), NULL, 10) + 1);
	}
	else
		sprintf(s, "%.4d%.2d%.2d%.2d%s%s001",
			rand() % 10000, t->tm_year % 100, t->tm_mon + 1, t->tm_mday,
			sql.substr(sql.find("|") + 1).c_str(),sql.substr(0, sql.find("|")).c_str());

	str = s;

	return str;
}

string MyDB::reg(string sql)
{//sql->非sql语句，而是想不出来词，直接沿用了╮(￣▽￣)╭
	string str("select MAX(Id) from Info");

	MYSQL *tmysql;
	MYSQL_RES *tresult;
	MYSQL_ROW tsql_row;

	tmysql = mysql_init(NULL);
	
	if ((tmysql = mysql_real_connect(tmysql, "localhost", "root", "cm2544843034LL", "BookTickets", 3306, NULL, 0)) == NULL)
	{
		std::cout << mysql_error(mysql) << "\n";
		return "Fail";
	}

	mysql_query(tmysql, "set names gbk");					//编码问题

	if (!mysql_real_query(tmysql, str.c_str(), str.size()))	//执行成功返回0
	{
		if ((tresult = mysql_store_result(tmysql)) != NULL)	//获取结果集
		{
			tsql_row = mysql_fetch_row(tresult);
			str = tsql_row[0];
			mysql_free_result(tresult);						// 释放结果集的内存
		}
		else
			str = "Fail";
	}
	else
		str = "Fail";

	if (str == "Fail")
		return str;

	time_t tt = time(NULL);
	char s[15];
	tm *t = localtime(&tt);
	string tmp;

	sprintf(s, "%.2d%.2d%.2d00000000", t->tm_year % 100, t->tm_mon + 1, t->tm_mday);
	tmp = s;

	if (tmp > str)
		str = tmp;
	else
	{
		sprintf(s, "%.2d%.2d%.2d%.8d", t->tm_year % 100, t->tm_mon + 1, t->tm_mday, strtol(str.substr(6).c_str(), NULL, 10) + 1);
		str = s;
	}

	return str;
}

int MyDB::close()
{
	mysql_close(mysql);
	return 0;
}
