#pragma once
#include "XmlQuery.h"
#include "winsock.h"
#include "mysql.h"
#include "errmsg.h"

class DataBase
{
public:
	DataBase(void);
	~DataBase(void);
protected:
	
public:
	bool init();
	int query(const char * sql);
	bool Connect();
	CString getErrorMsg();
	MYSQL getDbHandler();
	bool Execute(CString sql);

protected:
	MYSQL handler;
	const char * host;
	const char * user;
	const char * password;
	const char * database;
	UINT port;
	const char * charset;
	CString errorMsg;
};
