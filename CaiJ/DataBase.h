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
	MYSQL Connect();
	const char* GetHost();
	const char* GetUser();
	const char* GetPassword();
	const char* GetCharset();
	const char* GetDbName();
	UINT GetPort();

protected:
	CString host;
	CString user;
	CString password;
	CString database;
	UINT port;
	CString charset;
};
