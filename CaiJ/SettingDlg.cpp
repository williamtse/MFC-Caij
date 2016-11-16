// SettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CaiJ.h"
#include "SettingDlg.h"
#include "Helper.h"

// CSettingDlg �Ի���

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


// CSettingDlg ��Ϣ�������

BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_setting_tab.InsertItem(0,L"���ݿ�");  //��Ӳ���һѡ� 
    m_setting_tab.InsertItem(1,L"��½");  //��Ӳ�����ѡ�
	//�����Ի���,���ҽ�IDC_TABTEST�ؼ���Ϊ������ 
	m_dbdlg.Create(IDD_DBDLG,GetDlgItem(IDC_SETTINGTAB)); 
	m_loginInfoDlg.Create(IDD_LOGININFO,GetDlgItem(IDC_SETTINGTAB));
	CRect rs; 
	m_setting_tab.GetClientRect(&rs); 
	//�����ӶԻ����ڸ������е�λ�� 
	rs.top+=22; 
	rs.bottom-=3; 
	rs.left+=1; 
	rs.right-=3; 

	//�����ӶԻ���ߴ粢�ƶ���ָ��λ�� 
	m_dbdlg.MoveWindow(&rs); 
	m_loginInfoDlg.MoveWindow(&rs);
	m_dbdlg.ShowWindow(true); 
	m_loginInfoDlg.ShowWindow(false);
	m_setting_tab.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
