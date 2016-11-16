#pragma once


// CLoginInfo 对话框

class CLoginInfo : public CDialog
{
	DECLARE_DYNAMIC(CLoginInfo)

public:
	CLoginInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginInfo();

// 对话框数据
	enum { IDD = IDD_LOGININFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_username;
	CString m_passwd;
};
