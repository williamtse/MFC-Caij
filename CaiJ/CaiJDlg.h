// CaiJDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CCaiJDlg �Ի���
class CCaiJDlg : public CDialog
{
// ����
public:
	CCaiJDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CAIJ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CString m_uid;
	afx_msg void OnBnClickedButtonStart();
	CString m_log;
	CEdit c_log;
	CListBox c_result;
};



UINT   CaijiThreadFunction(LPVOID pParam);

typedef struct {
	CString name;
	CString url;
	int count;
	int flush;
	CListCtrl* clist;
	CEdit* clog;
	int row;
	CListBox* cresult;
}CaijiTask;