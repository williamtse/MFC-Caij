//DataBase类只提供数据库配置数据
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
	host = dbhost;
	user = dbuser;
	password = dbpasswd;
	database = dbname;
	port = _ttoi(dbport);
	charset = L"set names "+dbcharset;

	dbuser.ReleaseBuffer();
	dbpasswd.ReleaseBuffer();
	dbhost.ReleaseBuffer();
	dbname.ReleaseBuffer();
	dbport.ReleaseBuffer();
	dbcharset.ReleaseBuffer();

	return TRUE;
}
const char* DataBase::GetHost()
{
	return Helper::CTCC(host);
}
const char* DataBase::GetUser()
{
	return Helper::CTCC(user);
}
const char* DataBase::GetPassword()
{
	return Helper::CTCC(password);
}
const char* DataBase::GetCharset()
{
	return Helper::CTCC(charset);
}
const char* DataBase::GetDbName()
{
	return Helper::CTCC(database);
}
UINT DataBase::GetPort()
{
	return port;
}
