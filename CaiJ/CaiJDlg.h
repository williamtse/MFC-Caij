// CaiJDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "HttpClient.h"
#include "Query.h"
#include "Url.h"
#include "Helper.h"
#include<shlwapi.h>


#include "DataBase.h"
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
	void SwitchBtn(bool start,bool stop);

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
protected:
	virtual void OnOK();
	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnStop();
	CListBox m_result;
	CString m_xml;
	bool DbConnected;
	
	afx_msg void OnBnClickedButtonLogin();
	CString m_user;
	CString m_passwd;
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
	DataBase* db;
}CAIJI_TASK;

