#include "StdAfx.h"
#include "HttpClient.h"
#include "Url.h"
#include "Helper.h"

CString szHeaders = _T("Accept: _T/*\r\nUser-Agent: LCD's Infobay Http Client\r\n");

//���캯��
HttpClient::HttpClient()
{
    PageCode = 65001; //��Ϊ���ǵ���ַ��UTF8��ʽ��������65001��
    m_HttpCode = _T("");
}

//��������
HttpClient::~HttpClient()
{
    m_strError.ReleaseBuffer();
}
bool HttpClient::CheckNetIsOk()
{
	CInternetSession session(NULL, 0); 
	CInternetFile *file;
	try{
		session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 60000); // 3������ӳ�ʱ
		session.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT ,30000);//3����ճ�ʱ
		session.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT  ,30000);//3����ճ�ʱ
		 session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 2);
		file=(CInternetFile*)session.OpenURL(SITE,1,INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD);
	}catch(CInternetException* m_pException){
		file = NULL;
		m_pException->Delete();
		m_strError = _T("�������Ӳ������������������ӣ�");
		return FALSE;
	}
}
//��Ҫ�ӿڣ�������ַ����ȡ����
//Ĭ��ΪGET���󣬲�������
bool HttpClient::GetHttpCode(CString &url,int strMethod=METHOD_GET,CString strParams=NULL)
{
	
	m_HttpCode.Empty();
    //��ȡ��ҳ�ĳ�ʼ������
    CInternetSession session(NULL, 0); 
    CHttpFile *htmlFile = NULL;
	htmlFile = (CHttpFile*)session.OpenURL(url);//������
    TCHAR sRecv[1024];  //���ܻ�����롣

    //�����жϳ�ʼ��
    DWORD dwServiceType = 0;
    DWORD  dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;
    CString strServerName = _T("");
    CString strObject = _T("");
    INTERNET_PORT nPort = 0;
    CString StrContent = _T("");
    DWORD dwRetcode = -1;

    CHttpConnection *pServer = NULL;

	CString headerCT = _T("Content-Type: application/x-www-form-urlencoded"); // ����ͷ

    try
    {
        if (!OnInitSession(session)) //�ж������Ƿ�ɹ������Բ�Ҫ
        {
            return FALSE;
        }
        if (!AfxParseURL(url, dwServiceType, strServerName, strObject, nPort) || dwServiceType != INTERNET_SERVICE_HTTP)
        {
            m_strError = _T("�Ƿ���URL");
            return FALSE;
        }

        pServer = session.GetHttpConnection(strServerName, nPort);


        if (pServer == NULL)
        {
            m_strError = _T("�޷����������������");
            return FALSE;
        }
        //�����һ������Ϊ1  ��http����
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
            m_strError = _T("�޷����������������");
            return FALSE;
        }

      
        //////////////////////////////////////////////////////////////////////////

        if (!htmlFile->QueryInfoStatusCode(dwRetcode))
        {
            m_strError = _T("��������޷���ѯ��������");
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

                    //gb2312תΪunicode,����CP_ACP  
                    //gbkתΪunicode,Ҳ��CP_ACP  
                    //utf-8תΪunicode,����CP_UTF8  
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
                m_strError = _T("�������ݴ���");
                pEx->Delete();

                //��Ϊ��CString,���Է���NULL��������0��
                return FALSE;
            }
            //return StrContent;
			return TRUE;

        }
        else
        {
            //��ȡʧ�ܣ���buffer��ա�
            StrContent.Empty();

            //���ʹ���
            OnProcessError(dwRetcode, session, pServer, htmlFile);

            return FALSE;
        }

    }
    catch (CInternetException* pEx)
    {
        m_strError = _T("�������");

        pEx->Delete();
        return FALSE;
    }

    return FALSE;
}

CString HttpClient::GetHtml()
{
	return m_HttpCode;
}

//�ж������Ƿ�ɹ������Բ�Ҫ
BOOL HttpClient::OnInitSession(CInternetSession &session)
{
    //��ʱ���ú���Ҫ���������̫С�������������ʱ���������̫����������̹߳���
    //����������֮��ĵȴ�����ʱֵ�ں��뼶��
    //������������ʱ�䳬ʱֵ�������뼶�������������ʱ�䳬�������ʱֵ�����󽫱�ȡ����ȱʡ�ĳ�ʱֵ�����޵ġ�
    //��������������ʱ�����Դ��������һ��������ͼ��ָ�������Դ�������ʧ�ܣ�������ȡ����ȱʡֵΪ5��
    try
    {
        if (!session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 10000) ||  //��ʱ
            !session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF, 1000) ||    //�ӳ�
            !session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 3) ||       //����
            !session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 60000) ||       //���ʱ
            !session.EnableStatusCallback(TRUE))            //����һ��״̬�ص����̡��첽������Ҫ
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

//���Ӵ�����
int HttpClient::OnProcessError(int dwRetcode, CInternetSession &session,
    CHttpConnection *pServer, CHttpFile *pFile)
{
    switch (dwRetcode)
    {
    case 100:
        m_strError = _T("�ͻ�������-���� [Continue]");
        break;
        //
    case 101:
        m_strError = _T("�ͻ�������-����Э�� [witching Protocols]");
        break;
        //
    case 204:
        m_strError = _T("��ҳ����Ϊ�� [No Content]");
        break;
        //------------------------------------------------------------
    case 400:
        m_strError = _T("�������� [Bad Request]");
        break;
        //
    case 401:
        m_strError = _T("��ҳ��Ҫ��֤��Ϣ [Unauthorized]");
        break;
        //
    case 402:
        m_strError = _T("��ҳ��Ҫ���� [Payment Required]");
        break;
        //
    case 403:
        m_strError = _T("��ֹ���� [Forbidden]");
        break;
        //
    case 404://
        m_strError = _T("û���ҵ���ҳ [Not Found]");
        break;
        //
    case 405:
        m_strError = _T("������Http���ʸ��ļ� [Method Not Allowed]");
        break;
        //
    case 406:
        m_strError = _T("���ļ���������� [Not Acceptable]");
        break;
        //
    case 407:
        m_strError = _T("���ļ���Ҫ������֤ [Proxy Authentication Required]");
        break;
        //
    case 408:
        m_strError = _T("�Ը��ļ�����ʱ [Request Time-out]");
        break;
        //
    case 409:
        m_strError = _T("�Ը��ļ����ʳ�ͻ [Conflict]");
        break;
        //
    case 410:
        m_strError = _T("�Ը��ļ�����ʧ�� [Gone]");
        break;
        //
    case 411:
        m_strError = _T("���ļ���Ҫ������Ϣ [Length Required]");
        break;
        //
    case 412:
        m_strError = _T("��������ʧ�� [Precondition Failed]");
        break;
        //
    case 413:
        m_strError = _T("�����ļ�ʵ��̫�� [Request Entity Too Large]");
        break;
        //
    case 414:
        m_strError = _T("�����URI̫�� [Request-URI Too Large]");
        break;
        //
    case 415:
        m_strError = _T("��֧��ý������ [Unsupported Media Type]");
        break;
        //
    case 416:
        m_strError = _T("��������ʧ�� [Requested range not satisfiable]");
        break;
        //
    case 417:
        m_strError = _T("Ԥ��ʧ�� [Expectation Failed]");
        break;
        //--------------------------------------------------------------
    case 500:
        m_strError = _T("�������ڲ����� [Internal Server Error]");
        break;
        //
    case 501:
        m_strError = _T("δʵ������ [Not Implemented]");
        break;
        //
    case 502:
        m_strError = _T("����ʧ�� [Bad Gateway]");
        break;
        //
    case 503:
        m_strError = _T("û���ҵ������� [Service Unavailable]");
        break;
        //
    case 504:
        m_strError = _T("���س�ʱ [Gateway Time-out]");
        break;
        //
    case 505:
        m_strError = _T("��������֧��ϵͳʹ�õ�HTTP�汾 [HTTP Version not supported]");
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

