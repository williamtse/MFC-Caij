#pragma once


// CLoginInfo �Ի���

class CLoginInfo : public CDialog
{
	DECLARE_DYNAMIC(CLoginInfo)

public:
	CLoginInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginInfo();

// �Ի�������
	enum { IDD = IDD_LOGININFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_username;
	CString m_passwd;
};
