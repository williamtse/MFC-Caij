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
	XmlQuery xml;
	CString dir = Helper::GetWorkDir();
	CString dbcf = dir+L"\\conf\\database.xml";
	if(!xml.load(dbcf))
	{
		errorMsg = L"找不到数据库配置文件"+dbcf;
		return FALSE;
	}
	XMLDOMELEMENT root;
	xml.GetRoot(root);
	XMLDOMNODELIST list;
	xml.GetNodes(root,list);
	long len;
	list->get_length(&len);
	for(int i=0;i<len;i++)
	{
		XMLDOMNODE node ;
		xml.GetItem(list,i,node);
		CString nodeName =xml.GetNodeName(node);
		CString cv = xml.GetNodeValue(node);
		const char* nodeValue = Helper::CTCC(cv);
		if(nodeName==L"HOST")
		{
			host = nodeValue;
		}
		else if(nodeName == L"USERNAME")
		{
			user = nodeValue;
		}
		else if(nodeName == L"PASSWORD")
		{
			password = nodeValue;
		}
		else if(nodeName == L"DATABASE")
		{
			database = nodeValue;
		}
		else if(nodeName == L"PORT")
		{
			port = atoi(nodeValue);
		}
		else if(nodeName == L"CHARSET")
		{
			CString set = L"set names "+cv;
			charset = Helper::CTCC(set);
		}
		else
		{
			errorMsg = L"配置文件有错";
			return FALSE;
		}
		
	}
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