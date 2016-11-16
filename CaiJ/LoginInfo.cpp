// LoginInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CaiJ.h"
#include "LoginInfo.h"
#include "Helper.h"

// CLoginInfo �Ի���

IMPLEMENT_DYNAMIC(CLoginInfo, CDialog)

CLoginInfo::CLoginInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginInfo::IDD, pParent)
	, m_username(_T(""))
	, m_passwd(_T(""))
{

}

CLoginInfo::~CLoginInfo()
{
}

void CLoginInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_username);
	DDX_Text(pDX, IDC_EDIT2, m_passwd);
}


BEGIN_MESSAGE_MAP(CLoginInfo, CDialog)
END_MESSAGE_MAP()


// CLoginInfo ��Ϣ�������

BOOL CLoginInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString username,passwd;
	GetPrivateProfileString(L"Login",L"UserName",username,username.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	GetPrivateProfileString(L"Login",L"Password",passwd,passwd.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	m_username = username;
	m_passwd = passwd;
	UpdateData(FALSE);
	username.ReleaseBuffer();
	passwd.ReleaseBuffer();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
