#pragma once


//抓取网页代码封装类

#include <afxinet.h> //所需要的函数的头文件。

#define METHOD_GET 0
#define METHOD_POST 1
#define SafeDelete(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData=NULL; }

class HttpClient
{
public:
    HttpClient();
    virtual ~HttpClient();

    //添加变量
protected:
    CString        m_strError;  //接受错误信息
    CString        m_HttpCode;  //接受抓取的网页代码
    UINT           PageCode;   //CP_UTF8:65001 CP_ACP:0  转换代码用  

    //添加自定义函数
public:
    BOOL OnInitSession(CInternetSession &session);//判断链接是否成功。可以不要
    
    CString GetHttpCode(CString &url,int strMethod,CString strParams); //主要接口，输入网址，获取代码
    
    int OnProcessError(int dwRetcode, CInternetSession &session,
                CHttpConnection *pServer, CHttpFile *pFile);//链接错误函数


};