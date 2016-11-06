// CaiJDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CaiJ.h"
#include "CaiJDlg.h"
#include "HttpClient.h"

#include <boost/regex.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CString2UTF8(CString strParams,char *c){

     LPTSTR  str=  strParams.GetBuffer(strParams.GetLength());

     int utf8Len = WideCharToMultiByte(CP_UTF8, 0, str, strParams.GetLength(), NULL, 0, NULL, NULL);

     c = new char[utf8Len];
     int nRtn = WideCharToMultiByte(CP_UTF8, 0, str, strParams.GetLength(), c, utf8Len, NULL, NULL);
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCaiJDlg �Ի���




CCaiJDlg::CCaiJDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaiJDlg::IDD, pParent)
	, m_uid(_T(""))
	, m_log(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCaiJDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT_UID, m_uid);
	DDX_Text(pDX, IDC_LOG, m_log);
	DDX_Control(pDX, IDC_LOG, c_log);
	DDX_Control(pDX, IDC_LIST2, c_result);
}

BEGIN_MESSAGE_MAP(CCaiJDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, &CCaiJDlg::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// CCaiJDlg ��Ϣ��������

BOOL CCaiJDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������
	m_list.InsertColumn( 0, L"����", LVCFMT_LEFT, 110 );// ������ 
    m_list.InsertColumn( 1, L"URL", LVCFMT_LEFT, 250 );
	m_list.InsertColumn( 2, L"ҳ��", LVCFMT_LEFT,50);
	m_list.InsertColumn( 3, L"����", LVCFMT_LEFT, 50 );
	m_list.InsertColumn( 4, L"ˢ��", LVCFMT_LEFT, 50 );

	m_uid = L"i4kjc7jfm15965600l4785579";
	UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCaiJDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCaiJDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCaiJDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wText,-1,NULL,0,NULL,FALSE);// WideCharToMultiByte������
	char *psText;  // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
	psText = new char[dwNum];
	WideCharToMultiByte (CP_OEMCP,NULL,wText,-1,psText,dwNum,NULL,FALSE);// WideCharToMultiByte���ٴ�����
	szDst = psText;// std::string��ֵ
	delete []psText;// psText�����
}

CString GenerateUrl(CString stype,CString script,CString rtype,CString pageNum){

}

UINT   CaijiThreadFunction(LPVOID pParam){
	CaijiTask *ctj = (CaijiTask *)pParam;
	HttpClient *hc = new HttpClient;
	int t=0;
	CString st;
	CString matchCount;
	while(1){
		if(t==0){
			CString html = hc->GetHttpCode(ctj->url,METHOD_GET,NULL);
			ctj->clog->SetWindowText(L"�ɼ��ɹ�");
			//CString test = L"g('aaa','bbbb','ccc');";
			std::string htmlstr= (CStringA)html;
			boost::regex expression1("t_page=(\d*);");
			boost::smatch page;
			std::string::const_iterator start1 = htmlstr.begin();  
			std::string::const_iterator end1 = htmlstr.end();
			int pageNum=1;
			while(boost::regex_search(start1, end1, page, expression1)){
				pageNum = atoi(page[0].str().c_str());
			}
			CString pageNumStr;
			pageNumStr.Format(L"%d",pageNum);
			ctj->clist->SetItemText(ctj->row,2,pageNumStr);
			if(pageNum>0){
				//htmlstr = "g(aaa);\ng(bbb);\ng(ccc);";
				boost::regex expression2("g\\(([^\n]*)\\);");  
				boost::smatch what;  
				std::string::const_iterator start2 = htmlstr.begin();  
				std::string::const_iterator end2 = htmlstr.end(); 
				int mc = 0;
				while ( boost::regex_search(start2, end2, what, expression2) )  
				{  
					mc++;
					CString matchstr;
					//matchstr.Format(L"%s",what[0].str().c_str());
					matchstr = CA2T(what[0].str().c_str());
					ctj->cresult->AddString(matchstr);
					start2 = what[0].second;
					matchCount.Format(_T("%d"),mc);
					ctj->clist->SetItemText(ctj->row,3,matchCount);
				}  
			}
			ctj->clog->SetWindowText(L"is match");
			t=ctj->flush;
		}
		Sleep(1000);
		t--;
		st.Format(_T("%d"), t);
		ctj->clist->SetItemText(ctj->row,4,st);
	}
	return 0;
};
//��ʼ�ɼ�
void CCaiJDlg::OnBnClickedButtonStart()
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	UpdateData(TRUE);
	CString cuid = m_uid;
	CString url = L"http://66.133.87.55/app/member/FT_browse/body_var.php?uid="+cuid+L"&rtype=r&langx=zh-cn&mtype=3&page_no=0&league_id=&hot_game=";
	CaijiTask *cjt = new CaijiTask;
	cjt->name= L"����-����-����";
	cjt->url=url;
	cjt->count=0;
	cjt->flush=20;
	cjt->clog = &c_log;
	cjt->cresult = &c_result;
	
	int row=m_list.InsertItem(0,cjt->name); //��insertitem ,��������
	m_list.SetItemText(0,1,cjt->url);
	CString count,flush;
	count.Format(_T("%d"), cjt->count);
	flush.Format(_T("%d"), cjt->flush);
	m_list.SetItemText(0,2,count);
	m_list.SetItemText(0,3,flush);
	cjt->clist = &m_list;
	cjt->row = row;
	CWinThread* mythread = AfxBeginThread(CaijiThreadFunction,(LPVOID)cjt); 
}