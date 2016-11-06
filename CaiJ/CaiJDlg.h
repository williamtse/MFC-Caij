// CaiJDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CCaiJDlg 对话框
class CCaiJDlg : public CDialog
{
// 构造
public:
	CCaiJDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CAIJ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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