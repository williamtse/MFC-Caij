#pragma once


//ץȡ��ҳ�����װ��

#include <afxinet.h> //����Ҫ�ĺ�����ͷ�ļ���

#define METHOD_GET 0
#define METHOD_POST 1
#define SafeDelete(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData=NULL; }

class HttpClient
{
public:
    HttpClient();
    virtual ~HttpClient();

    //��ӱ���
protected:
    CString        m_strError;  //���ܴ�����Ϣ
    CString        m_HttpCode;  //����ץȡ����ҳ����
    UINT           PageCode;   //CP_UTF8:65001 CP_ACP:0  ת��������  

    //����Զ��庯��
public:
    BOOL OnInitSession(CInternetSession &session);//�ж������Ƿ�ɹ������Բ�Ҫ
    
    CString GetHttpCode(CString &url,int strMethod,CString strParams); //��Ҫ�ӿڣ�������ַ����ȡ����
    
    int OnProcessError(int dwRetcode, CInternetSession &session,
                CHttpConnection *pServer, CHttpFile *pFile);//���Ӵ�����


};