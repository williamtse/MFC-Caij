// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CaiJ.h"
#include "LoginDlg.h"
#include "CaiJDlg.h"

// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
	, m_username(_T(""))
	, m_passwd(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_username);
	DDX_Text(pDX, IDC_EDIT3, m_passwd);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RELOGIN, &CLoginDlg::OnBnClickedButtonRelogin)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序

void CLoginDlg::OnBnClickedOk()
{
	//OnOK();
	UpdateData(TRUE);
	CString user = m_username;
	CString passwd = m_passwd;
	CString login_url;
	login_url.Format(L"%s/app/member/new_login.php",IP_ADDR);
	//login_url=L"http://localhost/test.php";
	CString strParams;
	strParams.Format(L"username=%s&passwd=%s&langx=zh-cn&auto=CBCCCI&blackbox=undefined",
		user,passwd);
	HttpClient *hc = new HttpClient();
	if(!hc->GetHttpCode(login_url,METHOD_POST,strParams))
	{
		AfxMessageBox(L"网络连接不正常");
		return;
	}
	CString response = hc->GetHtml();
	CString uid = Helper::ExtraUid(response);
	long uidLen = uid.GetLength();
	delete hc;
	if(uidLen>24)
	{
		WritePrivateProfileString(L"Login",L"UserName",user,INIPATH);
		WritePrivateProfileString(L"Login",L"Password",passwd,INIPATH);
		WritePrivateProfileString(L"Login",L"Uid",uid,INIPATH);
		m_uid = uid;
		OnOK();
	}
	else
	{
		AfxMessageBox(L"登陆失败:"+response);
	}
	UpdateData(FALSE);
}

//使用上次登陆
void CLoginDlg::OnBnClickedButtonRelogin()
{
	CString uid;
	GetPrivateProfileString(L"Login",L"Uid",uid,uid.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	m_uid = uid;
	uid.ReleaseBuffer();
	OnOK();
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString username,passwd;
	GetPrivateProfileString(L"Login",L"UserName",username,username.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	GetPrivateProfileString(L"Login",L"Password",passwd,passwd.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	m_username = username;
	m_passwd=passwd;
	UpdateData(FALSE);
	username.ReleaseBuffer();
	passwd.ReleaseBuffer();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
