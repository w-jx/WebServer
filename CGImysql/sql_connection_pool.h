/*
 * @Author: your name
 * @Date: 2021-04-09 20:52:49
 * @LastEditTime: 2021-04-09 21:15:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/CGImysql/sql_connection_pool.h
 */
#ifndef _CONNECTION_POOL_
#define _CONNECTION_POOL_

#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>
#include "../lock/locker.h"
#include "../log/log.h"

using namespace std;

class connection_pool
{
public:
	/**
  * @description: 当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
  * @param {*}
  * @return {*}
  */
	MYSQL *GetConnection();				 //获取数据库连接
	bool ReleaseConnection(MYSQL *conn); //释放连接
	int GetFreeConn();					 //获取连接
	void DestroyPool();					 //销毁所有连接

	//单例模式
	static connection_pool *GetInstance();
	/**
  * @description: 数据库连接池初始化
  * @param {string} url 链接
  * @param {string} User 用户名
  * @param {string} PassWord 密码
  * @param {string} DataBaseName 数据库明
  * @param {int} Port 端口
  * @param {int} MaxConn 最大连接数
  * @param {int} close_log 是否关闭连接？
  * @return {*}
  */
	void init(string url, string User, string PassWord, string DataBaseName, int Port, int MaxConn, int close_log); 

private:
	connection_pool();
	~connection_pool();

	int m_MaxConn;  //最大连接数
	int m_CurConn;  //当前已使用的连接数
	int m_FreeConn; //当前空闲的连接数
	locker lock;
	list<MYSQL *> connList; //连接池 双链表
	sem reserve;

public:
	string m_url;			 //主机地址
	string m_Port;		 //数据库端口号
	string m_User;		 //登陆数据库用户名
	string m_PassWord;	 //登陆数据库密码
	string m_DatabaseName; //使用数据库名
	int m_close_log;	//日志开关
};

/**
 * @description: 连接的获得，离开作用域，连接销毁
 * @param {*}构造函数参数：1.MYSQL **
 * @return {*}
 */
class connectionRAII{

public:
	connectionRAII(MYSQL **con, connection_pool *connPool);
	~connectionRAII();
	
private:
	MYSQL *conRAII;
	connection_pool *poolRAII;
};

#endif
