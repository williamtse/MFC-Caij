#pragma once


// CDBDlg �Ի���

class CDBDlg : public CDialog
{
	DECLARE_DYNAMIC(CDBDlg)

public:
	CDBDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDBDlg();

// �Ի�������
	enum { IDD = IDD_DBDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_dbhost;
	CString m_dbuser;
	CString m_dbpasswd;
	CString m_dbname;
	virtual BOOL OnInitDialog();
};
