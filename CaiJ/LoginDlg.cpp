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
	CString response = hc->GetHttpCode(login_url,METHOD_POST,strParams);
	CString uid = Helper::ExtraUid(response);
	long uidLen = uid.GetLength();
	delete hc;
	if(uidLen>24)
	{
		AfxMessageBox(L"登陆成功");
	}
	else
	{
		AfxMessageBox(L"登陆失败");
	}
	UpdateData(FALSE);
}
