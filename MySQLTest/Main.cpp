#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <mysql.h>

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void CatString(const char* strRes, vector<string>& vecString)
{
	string strTemp;
	while (*strRes)
	{
		if (*strRes != '\n')
			strTemp += *strRes++;
		else
		{
			vecString.push_back(strTemp);
			strTemp.clear();
			strRes++;
		}
	}
}
 
int main()
{
	ifstream infile("Conf/DBConnect.conf");
	string strBuffer;
	char buffer[512];
	memset(buffer, 0, 512);
	infile.read(buffer, sizeof(buffer));
	vector<string> vecParam;
	CatString(buffer, vecParam);
	for (int i = 0 ; i < vecParam.size(); ++i)
	{
		cout<<vecParam[i].c_str()<<endl;
	}

	MYSQL * con; //= mysql_init((MYSQL*) 0); 
	MYSQL_RES *res;
	MYSQL_ROW row;  
  
	char tmp[400];
	//database configuartion
	char dbuser[30] = "root"; 
	char dbpasswd[30]="123456";
	char dbip[30]="localhost";
	char dbname[50]="test";
	char tablename[50]="test";
	char *query=NULL;

	while (1)
	{
		
	printf("input x,y\n");
	int x;
	int y;
	scanf("%d,%d",&x,&y);
	fflush(stdin);
	printf("input over\n");
	con = mysql_init((MYSQL*) 0); 

	int rt;//return value
	if ( con !=NULL && mysql_real_connect(con, dbip, dbuser, dbpasswd, dbname,3306/*TCP IP端口*/,NULL/*Unix Socket 连接类型*/,0/*运行成ODBC数据库标志*/) ) 
	{ 
		if (!mysql_select_db(con, dbname)) 
		{ 
			printf("Select successfully the database!\n"); 
			con->reconnect = 1; 
			query = "set names \'GBK\'";
			//mysql_query(con,"set names \'GBK\'"); 
			rt=mysql_real_query(con,query,strlen(query));
			if (rt)
			{
				printf("Error making query: %s !!!\n",mysql_error(con));
			}
			else
			{
				printf("query %s succeed!\n",query);
			}
		}
	}
	else
	{
		MessageBoxA(NULL, "Unable to connect the database,check your configuration!", "", NULL);
	}

	sprintf(tmp, "insert into %s values(%d,%d)", tablename, x, y);
	rt = mysql_real_query(con, tmp, strlen(tmp));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(con));
	}
	else
	{
		printf("%s executed!!!\n", tmp);
	}
	sprintf(tmp, "select * from %s", tablename);

	rt = mysql_real_query(con, tmp, strlen(tmp));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(con));
	} 
	else
	{
		printf("%s executed!!!\n", tmp);
	}
   
	int count = 0;
	res = mysql_store_result(con);//将结果保存在res结构体中
	while(row = mysql_fetch_row(res))  
	{  
		for(unsigned int t = 0;t<mysql_num_fields(res); ++t)  
		{  
			printf("%s  ", row[t]);  
		}  
		printf(".............\n");  
		count++;
	} 

	printf("number of rows %d\n",count);
	printf("mysql_free_result...\n");  
	mysql_free_result(res);

	}

	mysql_close(con);
	return 0;
}