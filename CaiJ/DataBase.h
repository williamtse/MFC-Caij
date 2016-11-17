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
	CString host;
	CString user;
	CString password;
	CString database;
	UINT port;
	CString charset;
	CString errorMsg;
};
