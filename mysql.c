#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mysql/mysql.h>

void do_select(MYSQL *db);
void do_insert(MYSQL *db);
void do_update(MYSQL *db);
void do_delete(MYSQL *db);

int main(int argc, char *argv[])
{
	MYSQL *db;

	//初始化
	db = mysql_init(NULL);
	if (db == NULL)
	{
		printf("mysql_init fail:%s\n", mysql_error(db));
		return -1;
	}

	//连接数据库
	db = mysql_real_connect(db, "127.0.0.1", "root", "1", "mydb",  3306, NULL, 0);
	if (db == NULL)
	{
		printf("mysql_real_connect fail:%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}


#if 0
	do_select(db);
	do_insert(db);

	my_ulonglong num;
	num = mysql_affected_rows(db);
	printf("----------num = %llu\n", num);

	do_update(db);
	do_delete(db);
	do_select(db);
#endif

	do_insert(db);
	do_select(db);

	//关闭数据库
	mysql_close(db);
	return 0;
}

void do_select(MYSQL *db)
{
	//执行查询语句
	char sql[128] = {0};
	strcpy(sql, "select * from stu");
	if (mysql_real_query(db, sql, strlen(sql) + 1) != 0)
	{
		printf("mysql_real_query fail:%s\n", mysql_error(db));
		return ;
	}

	//存储结果
	MYSQL_RES *res;
	res = mysql_store_result(db);
	if (res == NULL)
	{
		printf("mysql_store_result fail:%s\n", mysql_error(db));
		return ;
	}

	//统计执行结果集合中的行数
	my_ulonglong rows;
	rows = mysql_num_rows(res);
	printf("rows = %llu\n", rows);

	//统计执行结果集合中的列数
	unsigned int fields;
	fields = mysql_num_fields(res);
	printf("fields = %d\n", fields);

	puts("++++++++++++++++++++++++++");
	//得到字段名
	int i = 0;
	MYSQL_FIELD *mysql_filed;
	mysql_filed = mysql_fetch_fields(res);
	for (i = 0; i < fields ; i++)
	{
		printf("%s\n", mysql_filed[i].name);
	}
	puts("++++++++++++++++++++++++++");


	//访问每一条记录的值
	MYSQL_ROW  row;
	unsigned long  *len;
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		len = mysql_fetch_lengths(res);
		for (i = 0; i < fields ; i++)
		{
			printf("<td>[%.*s]</td>", (int)len[i], row[i]);
		}
		putchar(10);
	}

	//释放空间
	mysql_free_result(res);
}

void do_insert(MYSQL *db)
{
	char sql[128] = {0};
	int id;
	char name[32] = {0};

	printf("input id >> ");
	scanf("%d", &id);

	printf("input name >> ");
	scanf("%s", name);

	sprintf(sql, "insert into stu values(%d, '%s')", id, name);

	if (mysql_real_query(db, sql, strlen(sql) + 1) != 0)
	{
		printf("fail to insert :%s\n", mysql_error(db));
		return;
	}

	puts("insert OK!");
	return;
}

void do_update(MYSQL *db)
{
	char sql[128] = {0};

	int id;
	char name[32] = {0};

	printf("input update  id >> ");
	scanf("%d", &id);

	printf("input new name >> ");
	scanf("%s", name);

	sprintf(sql, "update stu set name = '%s' where id = %d", name, id);

	if (mysql_real_query(db, sql, strlen(sql) + 1) != 0)
	{
		printf("fail to update :%s\n", mysql_error(db));
		return ;
	}

	puts("update OK!");
	return;
}

void do_delete(MYSQL *db)
{
	char sql[128] = {0};
	int id;
	printf("input delete id >> ");
	scanf("%d", &id);

	sprintf(sql, "delete from stu  where id = %d", id);
	if (mysql_real_query(db, sql, strlen(sql) + 1) != 0)
	{
		printf("fail to delete:%s\n", mysql_error(db));
		return ;
	}

	puts("delete OK!");
	return;
}
