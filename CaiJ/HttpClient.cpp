#include "StdAfx.h"
#include "HttpClient.h"
#include "Url.h"
#include "Helper.h"

CString szHeaders = _T("Accept: _T/*\r\nUser-Agent: LCD's Infobay Http Client\r\n");

//构造函数
HttpClient::HttpClient()
{
    PageCode = 65001; //因为我们的网址是UTF8格式，所以用65001；
    m_HttpCode = _T("");
}

//析构函数
HttpClient::~HttpClient()
{
    m_strError.ReleaseBuffer();
}
bool HttpClient::CheckNetIsOk()
{
	CInternetSession session(NULL, 0); 
	CInternetFile *file;
	try{
		session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 60000); // 3秒的连接超时
		session.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT ,30000);//3秒接收超时
		session.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT  ,30000);//3秒接收超时
		 session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 2);
		file=(CInternetFile*)session.OpenURL(SITE,1,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD);
	}catch(CInternetException* m_pException){
		file = NULL;
		m_pException->Delete();
		m_strError = _T("网络连接不正常，请检查网络连接！");
		return FALSE;
	}
}
//主要接口，输入网址，获取代码
//默认为GET请求，不带参数
bool HttpClient::GetHttpCode(CString &url,int strMethod=METHOD_GET,CString strParams=NULL)
{
	
	m_HttpCode.Empty();
    //获取网页的初始化工作
    CInternetSession session(NULL, 0); 
    CHttpFile *htmlFile = NULL;
	htmlFile = (CHttpFile*)session.OpenURL(url);//打开链接
    TCHAR sRecv[1024];  //接受缓存代码。

    //错误判断初始化
    DWORD dwServiceType = 0;
    DWORD  dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;
    CString strServerName = _T("");
    CString strObject = _T("");
    INTERNET_PORT nPort = 0;
    CString StrContent = _T("");
    DWORD dwRetcode = -1;

    CHttpConnection *pServer = NULL;

	CString headerCT = _T("Content-Type: application/x-www-form-urlencoded"); // 请求头

    try
    {
        if (!OnInitSession(session)) //判断链接是否成功；可以不要
        {
            return FALSE;
        }
        if (!AfxParseURL(url, dwServiceType, strServerName, strObject, nPort) || dwServiceType != INTERNET_SERVICE_HTTP)
        {
            m_strError = _T("非法的URL");
            return FALSE;
        }

        pServer = session.GetHttpConnection(strServerName, nPort);


        if (pServer == NULL)
        {
            m_strError = _T("无法与服务器建立连接");
            return FALSE;
        }
        //下面第一个可以为1  打开http链接
		switch(strMethod){
			case METHOD_GET:
				htmlFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject,NULL, 1, NULL, NULL, dwHttpRequestFlags);
				htmlFile->SendRequest(szHeaders,(LPVOID)(LPCTSTR)strParams, strParams.GetLength());
				break;
			case METHOD_POST:
				htmlFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, strObject,NULL, 1, NULL, NULL, dwHttpRequestFlags);
				const char *postData = Helper::CTCC(strParams);
				htmlFile->SendRequest(headerCT,(LPVOID)postData, strlen(postData));
				break;
		}



        if (htmlFile == NULL)
        {
            m_strError = _T("无法与服务器建立连接");
            return FALSE;
        }

      
        //////////////////////////////////////////////////////////////////////////

        if (!htmlFile->QueryInfoStatusCode(dwRetcode))
        {
            m_strError = _T("网络错误－无法查询反馈代码");
            return FALSE;
        }

        if (dwRetcode >= 200 && dwRetcode < 300)
        {
            try
            {
                
				
                while (htmlFile->ReadString(sRecv, 1024))
                {
                    int nBufferSize = MultiByteToWideChar(PageCode, 0, (LPCSTR)sRecv, -1, NULL, 0);

                    wchar_t *pBuffer = new wchar_t[nBufferSize + 1];
                    memset(pBuffer, 0, (nBufferSize + 1) *sizeof(wchar_t));

                    //gb2312转为unicode,则用CP_ACP  
                    //gbk转为unicode,也用CP_ACP  
                    //utf-8转为unicode,则用CP_UTF8  
                    MultiByteToWideChar(PageCode, 0, (LPCSTR)sRecv, -1, pBuffer, nBufferSize *sizeof(wchar_t));

                    m_HttpCode += pBuffer;
                    m_HttpCode += "\r\n";
                    delete pBuffer;
                }

                htmlFile->Close();
                session.Close();
                delete htmlFile;             
                return TRUE;
                //return m_HttpCode;

            }
            catch (CInternetException* pEx)
            {
                m_strError = _T("接收数据错误");
                pEx->Delete();

                //因为是CString,所以返回NULL，而不是0；
                return FALSE;
            }
            //return StrContent;
			return TRUE;

        }
        else
        {
            //读取失败，将buffer清空。
            StrContent.Empty();

            //发送错误。
            OnProcessError(dwRetcode, session, pServer, htmlFile);

            return FALSE;
        }

    }
    catch (CInternetException* pEx)
    {
        m_strError = _T("网络错误");

        pEx->Delete();
        return FALSE;
    }

    return FALSE;
}

CString HttpClient::GetHtml()
{
	return m_HttpCode;
}

//判断链接是否成功。可以不要
BOOL HttpClient::OnInitSession(CInternetSession &session)
{
    //超时设置很重要！如果设置太小回引起服务器超时，如果设置太大则回引起线程挂起。
    //在重试连接之间的等待的延时值在毫秒级。
    //网络连接请求时间超时值在数毫秒级。如果连接请求时间超过这个超时值，请求将被取消。缺省的超时值是无限的。
    //在网络连接请求时的重试次数。如果一个连接企图在指定的重试次数后仍失败，则请求被取消。缺省值为5。
    try
    {
        if (!session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 10000) ||  //超时
            !session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF, 1000) ||    //延迟
            !session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 3) ||       //重试
            !session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 60000) ||       //最大超时
            !session.EnableStatusCallback(TRUE))            //建立一个状态回调例程。异步操作需要
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    catch (CInternetException* pEx)
    {
        pEx->GetErrorMessage(m_strError.GetBuffer(0), 1023);
        pEx->Delete();

        return FALSE;
    }

}

//链接错误函数
int HttpClient::OnProcessError(int dwRetcode, CInternetSession &session,
    CHttpConnection *pServer, CHttpFile *pFile)
{
    switch (dwRetcode)
    {
    case 100:
        m_strError = _T("客户方错误-继续 [Continue]");
        break;
        //
    case 101:
        m_strError = _T("客户方错误-交换协议 [witching Protocols]");
        break;
        //
    case 204:
        m_strError = _T("网页内容为空 [No Content]");
        break;
        //------------------------------------------------------------
    case 400:
        m_strError = _T("错误请求 [Bad Request]");
        break;
        //
    case 401:
        m_strError = _T("网页需要验证信息 [Unauthorized]");
        break;
        //
    case 402:
        m_strError = _T("网页需要付费 [Payment Required]");
        break;
        //
    case 403:
        m_strError = _T("禁止访问 [Forbidden]");
        break;
        //
    case 404://
        m_strError = _T("没有找到网页 [Not Found]");
        break;
        //
    case 405:
        m_strError = _T("不允许Http访问该文件 [Method Not Allowed]");
        break;
        //
    case 406:
        m_strError = _T("该文件不允许访问 [Not Acceptable]");
        break;
        //
    case 407:
        m_strError = _T("该文件需要代理认证 [Proxy Authentication Required]");
        break;
        //
    case 408:
        m_strError = _T("对该文件请求超时 [Request Time-out]");
        break;
        //
    case 409:
        m_strError = _T("对该文件访问冲突 [Conflict]");
        break;
        //
    case 410:
        m_strError = _T("对该文件访问失败 [Gone]");
        break;
        //
    case 411:
        m_strError = _T("该文件需要长度信息 [Length Required]");
        break;
        //
    case 412:
        m_strError = _T("请求条件失败 [Precondition Failed]");
        break;
        //
    case 413:
        m_strError = _T("请求文件实体太大 [Request Entity Too Large]");
        break;
        //
    case 414:
        m_strError = _T("请求的URI太长 [Request-URI Too Large]");
        break;
        //
    case 415:
        m_strError = _T("不支持媒体类型 [Unsupported Media Type]");
        break;
        //
    case 416:
        m_strError = _T("队列请求失败 [Requested range not satisfiable]");
        break;
        //
    case 417:
        m_strError = _T("预期失败 [Expectation Failed]");
        break;
        //--------------------------------------------------------------
    case 500:
        m_strError = _T("服务器内部错误 [Internal Server Error]");
        break;
        //
    case 501:
        m_strError = _T("未实现请求 [Not Implemented]");
        break;
        //
    case 502:
        m_strError = _T("网关失败 [Bad Gateway]");
        break;
        //
    case 503:
        m_strError = _T("没有找到服务器 [Service Unavailable]");
        break;
        //
    case 504:
        m_strError = _T("网关超时 [Gateway Time-out]");
        break;
        //
    case 505:
        m_strError = _T("服务器不支持系统使用的HTTP版本 [HTTP Version not supported]");
        break;
        //
    }

    try
    {
        if (pFile != NULL) SafeDelete(pFile);
        if (pServer != NULL) SafeDelete(pServer);

        session.Close();

        return 0;
    }
    catch (CInternetException *pEx)
    {
        pEx->GetErrorMessage(m_strError.GetBuffer(0), 1024);
        pEx->Delete();

        return 0;
    }
}

