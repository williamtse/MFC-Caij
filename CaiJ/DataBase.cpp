#include "StdAfx.h"
#include "DataBase.h"
#include "XmlQuery.h"
#include<string>
#include "Helper.h"
DataBase::DataBase(void)
{
}

DataBase::~DataBase(void)
{
}

//初始化数据库连接
bool DataBase::init()
{
	CString dbhost,dbuser,dbpasswd,dbname,dbport,dbcharset;
	GetPrivateProfileString(L"DataBase",L"user",dbuser,dbuser.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	GetPrivateProfileString(L"DataBase",L"password",dbpasswd,dbpasswd.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	GetPrivateProfileString(L"DataBase",L"host",dbhost,dbhost.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	GetPrivateProfileString(L"DataBase",L"dbname",dbname,dbname.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	GetPrivateProfileString(L"DataBase",L"port",dbport,dbport.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	GetPrivateProfileString(L"DataBase",L"charset",dbcharset,dbcharset.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);

	host = Helper::CTCC(dbhost);
	user = Helper::CTCC(dbuser);
	password = Helper::CTCC(dbpasswd);
	database = Helper::CTCC(dbname);
	port = atoi(Helper::CTCC(dbport));
	charset = Helper::CTCC(L"set names "+dbcharset);

	dbuser.ReleaseBuffer();
	dbpasswd.ReleaseBuffer();
	dbhost.ReleaseBuffer();
	dbname.ReleaseBuffer();
	dbport.ReleaseBuffer();
	dbcharset.ReleaseBuffer();

	return TRUE;
}

bool DataBase::Connect()
{
	MYSQL m_sqlCon;
	mysql_init(&m_sqlCon);
	if(!mysql_real_connect(&m_sqlCon,host,user,password,database,port,NULL,0))
	{
		const char *err = mysql_error(&m_sqlCon);
		CString cerr(err);
		errorMsg = cerr;
		return FALSE;
	}
	else
	{
		mysql_query(&m_sqlCon,charset);
		handler = m_sqlCon;
		return TRUE;
	}
}

MYSQL DataBase::getDbHandler()
{
	return handler;
}

CString DataBase::getErrorMsg()
{
	return errorMsg;
}

bool DataBase::Execute(CString sql)
{
	char * pstr= Helper::UnicodeToUtf8(sql);
	int res = mysql_query(&handler, pstr);
	delete pstr;
	if(res!=0){
		CString errStr;
		switch(res){
			case CR_COMMANDS_OUT_OF_SYNC:
				errStr = L"Commands were executed in an improper order.";
				break;
			case CR_SERVER_GONE_ERROR:
				errStr = L"The MySQL server has gone away.";
				break;
			case CR_SERVER_LOST:
				errStr = L"The connection to the server was lost during the query.";
				break;
			case CR_UNKNOWN_ERROR:
				errStr = L"An unknown error occurred.";
				break;
			default:
				errStr = L"mysql语句执行错误";
				break;
		}
		errorMsg = errStr;
		return FALSE;
	}
	return TRUE;
}