// SettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CaiJ.h"
#include "SettingDlg.h"
#include "Helper.h"

// CSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTINGTAB, m_setting_tab);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SETTINGTAB, &CSettingDlg::OnTcnSelchangeSettingtab)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingDlg 消息处理程序

BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_setting_tab.InsertItem(0,L"数据库");  //添加参数一选项卡 
    m_setting_tab.InsertItem(1,L"登陆");  //添加参数二选项卡
	//关联对话框,并且将IDC_TABTEST控件设为父窗口 
	m_dbdlg.Create(IDD_DBDLG,GetDlgItem(IDC_SETTINGTAB)); 
	m_loginInfoDlg.Create(IDD_LOGININFO,GetDlgItem(IDC_SETTINGTAB));
	CRect rs; 
	m_setting_tab.GetClientRect(&rs); 
	//调整子对话框在父窗口中的位置 
	rs.top+=22; 
	rs.bottom-=3; 
	rs.left+=1; 
	rs.right-=3; 

	//设置子对话框尺寸并移动到指定位置 
	m_dbdlg.MoveWindow(&rs); 
	m_loginInfoDlg.MoveWindow(&rs);
	m_dbdlg.ShowWindow(true); 
	m_loginInfoDlg.ShowWindow(false);
	m_setting_tab.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSettingDlg::OnTcnSelchangeSettingtab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_setting_tab.GetCurSel();
	switch(CurSel)
	{
		case 0:
			m_dbdlg.ShowWindow(true); 
			m_loginInfoDlg.ShowWindow(false);
		break;
		case 1:
			m_dbdlg.ShowWindow(false); 
			m_loginInfoDlg.ShowWindow(true);
			break;
	}
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CSettingDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CString dbuser = m_dbdlg.GetDbUser();
	CString dbhost = m_dbdlg.GetDbHost();
	CString dbpasswd = m_dbdlg.GetDbPasswd();
	CString dbname = m_dbdlg.GetDbName();
	CString dbport = m_dbdlg.GetDbPort();
	CString dbcharset = m_dbdlg.GetDbCharset();

	CString username = m_loginInfoDlg.GetUserName();
	CString passwd = m_loginInfoDlg.GetPasswd();

	WritePrivateProfileString(L"DataBase",L"host",dbhost,INIPATH);
	WritePrivateProfileString(L"DataBase",L"user",dbuser,INIPATH);
	WritePrivateProfileString(L"DataBase",L"password",dbpasswd,INIPATH);
	WritePrivateProfileString(L"DataBase",L"dbname",dbname,INIPATH);
	WritePrivateProfileString(L"DataBase",L"port",dbport,INIPATH);
	WritePrivateProfileString(L"DataBase",L"charset",dbcharset,INIPATH);

	WritePrivateProfileString(L"Login",L"UserName",username,INIPATH);
	WritePrivateProfileString(L"Login",L"Passwd",passwd,INIPATH);

	OnOK();
}
