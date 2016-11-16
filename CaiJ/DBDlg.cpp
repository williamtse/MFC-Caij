// DBDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CaiJ.h"
#include "DBDlg.h"
#include "Helper.h"


// CDBDlg 对话框

IMPLEMENT_DYNAMIC(CDBDlg, CDialog)

CDBDlg::CDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBDlg::IDD, pParent)
	, m_dbhost(_T(""))
	, m_dbuser(_T(""))
	, m_dbpasswd(_T(""))
	, m_dbname(_T(""))
{

}

CDBDlg::~CDBDlg()
{
}

void CDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SET_DBHOST, m_dbhost);
	DDX_Text(pDX, IDC_DBUSER, m_dbuser);
	DDX_Text(pDX, IDC_DBPASSWD, m_dbpasswd);
	DDX_Text(pDX, IDC_DBNAME, m_dbname);
}


BEGIN_MESSAGE_MAP(CDBDlg, CDialog)
END_MESSAGE_MAP()


// CDBDlg 消息处理程序

BOOL CDBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString host,username,password,dbname;
	GetPrivateProfileString(L"DataBase",L"host",host,host.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	GetPrivateProfileString(L"DataBase",L"username",username,username.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	GetPrivateProfileString(L"DataBase",L"password",password,password.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	GetPrivateProfileString(L"DataBase",L"dbname",dbname,dbname.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	m_dbhost = host;
	m_dbuser = username;
	m_dbpasswd=password;
	m_dbname = dbname;
	UpdateData(FALSE);
	host.ReleaseBuffer();
	username.ReleaseBuffer();
	password.ReleaseBuffer();
	dbname.ReleaseBuffer();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
