// CaiJDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CaiJ.h"
#include "CaiJDlg.h"
#include "HttpClient.h"
#include "Query.h"
#include "Url.h"
#include "Helper.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool keep=TRUE;

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
	DDX_Control(pDX, IDC_LIST2, m_result);
}

BEGIN_MESSAGE_MAP(CCaiJDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, &CCaiJDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CCaiJDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CCaiJDlg ��Ϣ�������

BOOL CCaiJDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_list.InsertColumn( 0, L"����", LVCFMT_LEFT, 110 );// ������ 
    m_list.InsertColumn( 1, L"URL", LVCFMT_LEFT, 790 );
	m_list.InsertColumn( 2, L"ҳ��", LVCFMT_LEFT,50);
	m_list.InsertColumn( 3, L"����", LVCFMT_LEFT, 50 );
	m_list.InsertColumn( 4, L"ˢ��", LVCFMT_LEFT, 50 );

	m_uid = L"ngz1eajim15965600l135988";
	UpdateData(FALSE);

	keep = TRUE;
	m_result.SetHorizontalExtent(10000);
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

// �����Ի��������С����ť������Ҫ����Ĵ���
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


//�ɼ��̺߳���
UINT   CaijiThreadFunction(LPVOID pParam){
	CAIJI_TASK *ctj = (CAIJI_TASK *)pParam;
	HttpClient *hc = new HttpClient;
	int t=0;
	CString st;
	CString matchCount;
	Query *qr = new Query();
	CString url = ctj->url;
	while(keep){
		if(t==0){
			CString html = hc->GetHttpCode(url,METHOD_GET,NULL);
			int pageNum = qr->getPageNum(html);
			CString pageNumStr;
			pageNumStr.Format(L"%d",pageNum);

			if(pageNum>1){
				while(pageNum>0){
					pageNum--;
					CString page_no;
					page_no.Format(L"%d",pageNum);
					ctj->url.Format(L"%s&page_no=%s",url,page_no);
					html+=hc->GetHttpCode(ctj->url,METHOD_GET,NULL);
				}
			}
			//������ȡ����
			CStringArray matches;
			qr->queryBodyVar(html,matches);
			
			int matchLen = matches.GetCount();
			CString matchLenStr;
			
			if(matchLen>0){
				for(int i=0;i<matchLen;i++)
				{
					ctj->cresult->AddString(matches.GetAt(i));
					matchLenStr.Format(L"%d",i);
					ctj->clist->SetItemText(ctj->row,3,matchLenStr);
				}
			}
			ctj->clist->SetItemText(ctj->row,2,pageNumStr);
			
			t=ctj->flush;
		}
		Sleep(1000);
		t--;
		st.Format(_T("%d"), t);
		ctj->clist->SetItemText(ctj->row,4,st);//����ʱ
	}
	return 0;
};


int getSTYPE(CString stype){
	int STYPE=0;
	if(stype==L"FT"){
		STYPE=STYPE_FT;
	}
	if(stype==L"BK"){
		STYPE=STYPE_BK;
	}
	if(stype==L"BS"){
		STYPE=STYPE_BS;
	}
	if(stype==L"TN"){
		STYPE=STYPE_TN;
	}
	if(stype==L"BM"){
		STYPE=STYPE_BM;
	}
	if(stype==L"TT"){
		STYPE=STYPE_TT;
	}
	if(stype==L"OP"){
		STYPE=STYPE_OP;
	}
	if(stype==L"BV"){
		STYPE=STYPE_BV;
	}
	return STYPE;
}

//��ʼ�ɼ�
void CCaiJDlg::OnBnClickedButtonStart()
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	keep = TRUE;
	UpdateData(TRUE);
	CString cuid = m_uid;

	//��ʼ��COM
	CoInitialize(NULL);
	//��ȡXML
	CComPtr<IXMLDOMDocument> spDoc; //DOM
	spDoc.CoCreateInstance(CLSID_DOMDocument);//�����ĵ�����
	VARIANT_BOOL vb;
	spDoc->load(CComVariant(OLESTR("urls.xml")), &vb); //����XML�ļ�
	CComPtr<IXMLDOMElement> spRootEle;
	spDoc->get_documentElement(&spRootEle); //���ڵ�
	CComPtr<IXMLDOMNodeList> spNodeList;
	spRootEle->get_childNodes(&spNodeList); //�ӽڵ��б�
	long nLen;
	spNodeList->get_length(&nLen); //�ӽڵ���

	
	for (long i = 0; i < nLen; ++i) //�����ӽڵ�
	{
		URLPARAMS *urlParams = new URLPARAMS;
		CAIJI_TASK *cjt = new CAIJI_TASK;//�ɼ�����ṹ��
		CComPtr<IXMLDOMNode> spNode;
		spNodeList->get_item(i, &spNode);
		CComPtr<IXMLDOMNodeList> spUrlParamList;
		spNode->get_childNodes(&spUrlParamList);
		long pLen;
		spUrlParamList->get_length(&pLen);

		//��ȡURL�Ĳ���
		for(long j=0;j<pLen;++j)
		{
			CComPtr<IXMLDOMNode> spParamNode;
			spUrlParamList->get_item(j,&spParamNode);
			CComBSTR value;
			spParamNode->get_nodeName(&value);//URL�����ڵ�ı�ǩ��
			
			CString strValue = CString(value);
			if(strValue==L"TYPE"){
				spParamNode->get_text(&value);
				CString _type =  CString(value);
				if(_type==L"game" || _type==L"gun"||_type==L"future")
				{
					urlParams->type = TYPE_BODY_VAR;
				}
			}else if(strValue == L"STYPE"){
				spParamNode->get_text(&value);
				CString _stype = CString(value);
				urlParams->stype = _stype;
				int STYPE = getSTYPE(_stype);
				urlParams->sport = STYPE;
			}else if(strValue == L"RTYPE"){
				spParamNode->get_text(&value);
				CString _rtype = CString(value);
				urlParams->rtype = _rtype;
			}else if(strValue == L"NAME"){
				spParamNode->get_text(&value);
				CString _name = CString(value);
				cjt->name = _name;
			}else if(strValue == L"FUTURE"){
				spParamNode->get_text(&value);
				CString _FUTURE = CString(value);
				urlParams->is_future = _FUTURE==L"Y"?TRUE:FALSE;
			}
			urlParams->uid = cuid;

			spParamNode.Release();
		}
		Url url = new Url((LPVOID)urlParams);
		CString urlstr = url.GenerateUrl();
		cjt->url=urlstr;
		cjt->count=0;
		cjt->flush=20;
		cjt->cresult = &m_result;

		int row=m_list.InsertItem(i,cjt->name); //��insertitem ,��������
		m_list.SetItemText(row,1,cjt->url);
		CString count,flush;
		count.Format(_T("%d"), cjt->count);
		flush.Format(_T("%d"), cjt->flush);
		m_list.SetItemText(row,2,count);
		m_list.SetItemText(row,3,flush);
		cjt->clist = &m_list;
		cjt->row = row;

		AfxBeginThread(CaijiThreadFunction,(LPVOID)cjt); 

		spNode.Release();
		spUrlParamList.Release();
	}
	spNodeList.Release();
	spDoc.Release();

    ::CoUninitialize();
}

void CCaiJDlg::OnOK()
{
	CCaiJDlg::OnBnClickedButtonStart();
}

BOOL CCaiJDlg::PreTranslateMessage(MSG* pMsg)
{

    return CDialog::PreTranslateMessage(pMsg);  
}

void CCaiJDlg::OnBnClickedBtnStop()
{
	keep = FALSE;
	m_list.DeleteAllItems();
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	m_result.ResetContent();
}
