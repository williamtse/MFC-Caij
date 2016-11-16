// CaiJDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CaiJ.h"
#include "CaiJDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool keep=TRUE;
DataBase *DbHandler = new DataBase();
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CCaiJDlg 对话框




CCaiJDlg::CCaiJDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaiJDlg::IDD, pParent)
	, m_uid(_T(""))
	, m_log(_T(""))
	, m_xml(_T(""))
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
	ON_COMMAND(MENU_SETTING, &CCaiJDlg::OnSetting)
END_MESSAGE_MAP()


// CCaiJDlg 消息处理程序

BOOL CCaiJDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_list.InsertColumn( 0, L"名称", LVCFMT_LEFT, 110 );// 插入列 
    m_list.InsertColumn( 1, L"URL", LVCFMT_LEFT, 400 );
	m_list.InsertColumn( 2, L"页数", LVCFMT_LEFT,50);
	m_list.InsertColumn( 3, L"数量", LVCFMT_LEFT, 50 );
	m_list.InsertColumn( 4, L"刷新", LVCFMT_LEFT, 50 );

	
	UpdateData(FALSE);

	keep = TRUE;
	m_result.SetHorizontalExtent(10000);

	DbConnected = FALSE;
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

	INT_PTR nResponse = m_loginDlg.DoModal();
	if( nResponse != IDOK)  
	{  
		OnCancel();  
	}
	else
	{
		m_uid = m_loginDlg.m_uid;
		UpdateData(FALSE);
	}
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCaiJDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCaiJDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//采集线程函数
UINT   CaijiThreadFunction(LPVOID pParam){
	CAIJI_TASK *ctj = (CAIJI_TASK *)pParam;
	HttpClient *hc = new HttpClient;
	int t=0;
	CString st,matchCount,html,ohtml;
	Query *qr = new Query();
	CString url = ctj->url;
	while(keep){
		if(t==0){
			html = hc->GetHttpCode(url,METHOD_GET,NULL);
			int pageNum = qr->getPageNum(html);
			CString pageNumStr;
			pageNumStr.Format(L"%d",pageNum);
			ctj->clist->SetItemText(ctj->row,2,pageNumStr);
			while(pageNum>1){
				pageNum--;
				CString page_no;
				page_no.Format(L"%d",pageNum);
				ctj->url.Format(L"%s&page_no=%s",url,page_no);
				ohtml = hc->GetHttpCode(ctj->url,METHOD_GET,NULL);
				html += ohtml;
			}

			Helper::FiltKG(html);

			//解析提取数据
			CStringArray matches;
			qr->queryBodyVar(html,matches);
			
			int matchLen = matches.GetCount();
			CString matchLenStr;
			
			if(matchLen>0){
				CString gameHead = qr->getHeaderStr(html);
				CStringArray gameHeadArr ;
				Helper::StrExplode(',',gameHead,gameHeadArr);
				int glen = gameHeadArr.GetCount();
				
				
				
				CFileException fileException;
				for(int i=0;i<matchLen;++i)
				{
					
					CString qsql = L"REPLACE INTO match_games SET ";
					CString dsql = L"REPLACE INTO match_games_data (`id`,`gid`,`ziduan`,`gid_ziduan`,`val`) VALUES";
					
					CString matchRow = matches.GetAt(i);
					
					CStringArray dest;
					CStringArray matchRowArr;
					Helper::StrExplode(',',matchRow,matchRowArr);
					
					int mlen = matchRowArr.GetCount();
					//单行分解后
					if(mlen>0)
					{
							int len = mlen>glen?glen:mlen;
							CString gid = Helper::TrimSQuot(matchRowArr.GetAt(0));
							CString key=L"",val=L"",set=L"";
							for(int j=0;j<len;j++)
							{
								key = Helper::TrimSQuot(gameHeadArr.GetAt(j));
								val = Helper::TrimSQuot(matchRowArr.GetAt(j));
								if(j<8){
									set.Format(L"`%s`='%s'",key,val);
									if(j==7) qsql+= set;
									else qsql+=set+L",";
								}else{
									CString valstr;
									valstr.Format(L"(NULL,'%s','%s','%s','%s')",gid,key,gid+L"#"+key,val);
									if(j==len-1) dsql+=valstr;
									else dsql+=valstr+L",";
								}
							}
							ctj->cresult->AddString(qsql);
							ctj->cresult->AddString(dsql);
							CString qLog = qsql+L"\n";
							CString dLog = dsql+L"\n";

							CFile  file(L"E:\\log.txt",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
							ULONGLONG fl = file.GetLength();
							WORD unicode = 0xFEFF; 
							file.SeekToBegin();
							file.Write(&unicode,2); 
							file.SeekToEnd();
							file.Write(qLog.GetBuffer(qsql.GetLength()),qLog.GetLength()*sizeof(TCHAR));
							file.Close();
							
							if(!ctj->db->Execute(qsql)){
								CString error = ctj->db->getErrorMsg();
								AfxMessageBox(error);
								delete qr;
								delete hc;
								delete ctj;
								return 0;
							}
							
							matchLenStr.Format(L"%d",i);
							ctj->clist->SetItemText(ctj->row,3,matchLenStr);
					}
				}
			}
			t=ctj->flush;
			html.Empty();
		}
		Sleep(1000);
		t--;
		st.Format(_T("%d"), t);
		ctj->clist->SetItemText(ctj->row,4,st);//倒计时
	}
	delete qr;
	delete hc;
	delete ctj;
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

void CCaiJDlg::SwitchBtn(bool start,bool stop)
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(start);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(stop);
}

//开始采集
void CCaiJDlg::OnBnClickedButtonStart()
{
	SwitchBtn(FALSE,FALSE);

	//检测网络连接
	HttpClient *hc = new HttpClient;
	CString testUrl = IP_ADDR;
	if(!hc->CheckNetIsOk())
	{
		AfxMessageBox(hc->m_strError);
		SwitchBtn(TRUE,FALSE);
		return;
	}
	//连接mysql数据库
	if(!DbConnected){
		if(!DbHandler->init()){
			AfxMessageBox(DbHandler->getErrorMsg());
			SwitchBtn(TRUE,FALSE);
			return ;
		}
		if(!DbHandler->Connect()){
			AfxMessageBox(DbHandler->getErrorMsg());
			SwitchBtn(TRUE,FALSE);
			return;
		}
		DbConnected = TRUE;
	}

	CString dir = Helper::GetWorkDir();
	CString urlXml = dir+L"\\conf\\urls.xml";
	if(!PathFileExists(urlXml)){
		AfxMessageBox(L"找不到"+urlXml+L"文件");
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		return;
	}
	
	keep = TRUE;
	UpdateData(TRUE);
	CString cuid = m_uid;

	//初始化COM
	CoInitialize(NULL);
	//读取XML
	CComPtr<IXMLDOMDocument> spDoc; //DOM
	spDoc.CoCreateInstance(CLSID_DOMDocument);//创建文档对象
	VARIANT_BOOL vb;
	if(spDoc->load(CComVariant(urlXml), &vb) != S_OK) //加载XML文件
	{
		AfxMessageBox(L"找不到urls.xml文件");
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		return;
	}

	CComPtr<IXMLDOMElement> spRootEle;
	spDoc->get_documentElement(&spRootEle); //根节点
	CComPtr<IXMLDOMNodeList> spNodeList;
	spRootEle->get_childNodes(&spNodeList); //子节点列表
	long nLen;
	spNodeList->get_length(&nLen); //子节点数

	
	for (long i = 0; i < nLen; ++i) //遍历子节点
	{
		URLPARAMS *urlParams = new URLPARAMS;
		CAIJI_TASK *cjt = new CAIJI_TASK;//采集任务结构体
		cjt->db = DbHandler;
		CComPtr<IXMLDOMNode> spNode;
		spNodeList->get_item(i, &spNode);
		CComPtr<IXMLDOMNodeList> spUrlParamList;
		spNode->get_childNodes(&spUrlParamList);
		long pLen;
		spUrlParamList->get_length(&pLen);

		//获取URL的参数
		for(long j=0;j<pLen;++j)
		{
			CComPtr<IXMLDOMNode> spParamNode;
			spUrlParamList->get_item(j,&spParamNode);
			CComBSTR value;
			spParamNode->get_nodeName(&value);//URL参数节点的标签名
			
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
			}else if(strValue == L"FLUSH_TIME"){
				spParamNode->get_text(&value);
				CString _flushTime = CString(value);
				cjt->flush = _ttoi(_flushTime);
			}
			urlParams->uid = cuid;

			spParamNode.Release();
		}
		Url *url = new Url((LPVOID)urlParams);
		CString urlstr = url->GenerateUrl();
		delete url;
		cjt->url=urlstr;
		cjt->count=0;
		cjt->cresult = &m_result;

		int row=m_list.InsertItem(i,cjt->name); //用insertitem ,返回行数
		m_list.SetItemText(row,1,cjt->url);
		CString count,flush;
		count.Format(_T("%d"), cjt->count);
		flush.Format(_T("%d"), cjt->flush);
		m_list.SetItemText(row,3,count);
		m_list.SetItemText(row,4,flush);
		cjt->clist = &m_list;
		cjt->row = row;

		AfxBeginThread(CaijiThreadFunction,(LPVOID)cjt); 

		spNode.Release();
		spUrlParamList.Release();
		//delete urlParams;
	}
	spNodeList.Release();
	spDoc.Release();
	
    ::CoUninitialize();
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
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
}





void CCaiJDlg::OnSetting()
{
	INT_PTR nResponse = m_settingDlg.DoModal();
	if( nResponse == IDOK)  
	{  
		  AfxMessageBox(L"保存");
	}
}
