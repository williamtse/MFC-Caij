#pragma once


// CDBDlg 对话框

class CDBDlg : public CDialog
{
	DECLARE_DYNAMIC(CDBDlg)

public:
	CDBDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDBDlg();

// 对话框数据
	enum { IDD = IDD_DBDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_dbhost;
	CString m_dbuser;
	CString m_dbpasswd;
	CString m_dbname;
	virtual BOOL OnInitDialog();
};
