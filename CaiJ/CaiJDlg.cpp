// CaiJDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CaiJ.h"
#include "CaiJDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int tc=0;
bool keep=TRUE;
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
	, m_xml(_T(""))
	, m_strLog(_T(""))
	, s_progress(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCaiJDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT_UID, m_uid);
	DDX_Text(pDX, IDC_PG_TEXT, s_progress);
	DDX_Control(pDX, IDC_PROGRESS1, c_progress);
	DDX_Control(pDX, IDC_PG_TEXT, c_static_pro);
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
	//配置文件
	CString open_log,conf_log_dir;
	GetPrivateProfileString(L"Log",L"open_log",open_log,open_log.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	GetPrivateProfileString(L"Log",L"log_dir",conf_log_dir,conf_log_dir.GetBuffer(MAX_PATH),MAX_PATH,INIPATH);
	log_dir = BASE_DIR+L"\\"+conf_log_dir;
	if(!PathFileExists(log_dir))
	{
		AfxMessageBox(log_dir+L"不存在");
	}

	

	m_list.InsertColumn( 0, L"编号", LVCFMT_LEFT, 50 );
	m_list.InsertColumn( 1, L"名称", LVCFMT_LEFT, 110 );
    m_list.InsertColumn( 2, L"URL", LVCFMT_LEFT, 500 );
	m_list.InsertColumn( 3, L"页数", LVCFMT_LEFT, 50 );
	m_list.InsertColumn( 4, L"数量", LVCFMT_LEFT, 50 );
	m_list.InsertColumn( 5, L"刷新", LVCFMT_LEFT, 50 );

	
	UpdateData(FALSE);

	keep = TRUE;

	DbConnected = FALSE;
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);

	ShowLoginDlg();
	
	//初始化数据库连接
	db.init();
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void txtLog(CString pszFileName,CString logtext)
{
	CStdioFile file;
	CFileException fileException;
	if(file.Open(pszFileName, CFile::modeCreate|CFile::typeText | CFile::modeWrite,&fileException))
	{
		file.WriteString(logtext);
	}
	else
	{
		TRACE("Can't open file %s,error=%u\n",pszFileName,fileException.m_cause);
	}
	file.Close();
}


void CCaiJDlg::ShowLoginDlg()
{
	INT_PTR nResponse = m_loginDlg.DoModal();
	if( nResponse != IDOK)  
	{  
		OnCancel();  
	}
	else
	{
		m_uid = m_loginDlg.m_uid;
		SwitchBtn(TRUE,FALSE);
		UpdateData(FALSE);
	}
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
UINT CaijiThreadFunction(LPVOID pParam){
	tc++;
	Query *query = new Query;
	CAIJI_TASK *ctj = (CAIJI_TASK *)pParam;
	
	int t=0;
	CString st=L"0";
	CString matchCount=L"0";
	CString html=L" ";
	CString gameHead=L" ";
	CString url = ctj->url;
	CStringArray gameHeadArr,matches;
	MYSQL *m_sqlCon;
	m_sqlCon = mysql_init((MYSQL *)0);
	mysql_real_connect(m_sqlCon,//连接mysql
								ctj->db->GetHost(),
								ctj->db->GetUser(),
								ctj->db->GetPassword(),
								ctj->db->GetDbName(),
								ctj->db->GetPort(),
								NULL,
								0);
	mysql_query(m_sqlCon,"set  names utf8");
	while(keep){
		if(t==0){
			t=ctj->flush;
			try
			{
				html = Helper::GetHttpFileData(url);
			}
			catch(CInternetException *pEx)
			{
				pEx->Delete();
				delete ctj;
				return 0;
			}
			//多页数处理
			int pageNum = query->getPageNum(html);
			CString pageNumStr;
			pageNumStr.Format(L"%d",pageNum);
			ctj->threadList->SetItemText(ctj->row,3,pageNumStr);//页数
			if(pageNum>0)
			{
				CString realUrl;
				for(int n=1;n<pageNum;n++)
				{
					realUrl.Format(L"%s&page_no=%d",url,n);
					html+=Helper::GetHttpFileData(realUrl);
				}
				
				//解析提取数据
				query->queryBodyVar(html,matches);
				
				int matchLen = matches.GetCount();
				CString matchLenStr;
				
				if(matchLen>0){
					gameHead = query->getHeaderStr(html);
					
					Helper::StrExplode(',',gameHead,gameHeadArr);
					int glen = gameHeadArr.GetCount();
					
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
							
							
							const char *qstr= Helper::CStringTUtf8(qsql);//
							const char *dstr= Helper::CStringTUtf8(dsql);//
							CString logFile = ctj->logDir+L"error.TXT";
							if(mysql_query(m_sqlCon, qstr)!=0)
							{
								const char *err = mysql_error(m_sqlCon);
								CString cerr(err);
								//AfxMessageBox(cerr+qsql);
								txtLog(logFile,qsql);
								continue;
							}
							if(mysql_query(m_sqlCon, dstr)!=0)
							{
								const char *err = mysql_error(m_sqlCon);
								CString cerr(err);
								//txtLog(logFile,cerr+L"\r\n");
								continue;
							}
							matchLenStr.Format(L"%d",i);
							ctj->threadList->SetItemText(ctj->row,4,matchLenStr);
						}
					}
				}
				
			}
		}
		Sleep(1000);
		t--;
		st.Format(_T("%d"), t);
		ctj->threadList->SetItemText(ctj->row,5,st);//倒计时
	}
	mysql_close(m_sqlCon);//关闭连接
	ctj->pro->StepIt();
	int pos = ctj->pro->GetPos();
	CString strPos;
	strPos.Format(L"%d",pos);
	ctj->proText->SetWindowTextW(strPos);
	if(pos==tc)
	{
		CMenu *pMenu = ctj->dlg->GetSystemMenu(FALSE);  
		pMenu->EnableMenuItem(SC_CLOSE,MF_ENABLED);
	}
	delete ctj;
	return 0;
}




void CCaiJDlg::SwitchBtn(bool start,bool stop)
{
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(start);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(stop);
}

//开始采集
void CCaiJDlg::OnBnClickedButtonStart()
{
	if(!CheckDbConnect())
	{
		return;
	}
	SwitchBtn(FALSE,FALSE);
	//解析urls.xml
	CString urlXml = BASE_DIR+L"\\conf\\urls.xml";
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
		SwitchBtn(TRUE,FALSE);
		return;
	}

	CComPtr<IXMLDOMElement> spRootEle;
	spDoc->get_documentElement(&spRootEle); //根节点
	CComPtr<IXMLDOMNodeList> spNodeList;
	spRootEle->get_childNodes(&spNodeList); //子节点列表
	long nLen;
	spNodeList->get_length(&nLen); //子节点数
	
	c_progress.SetRange(0,(short)nLen);
	c_progress.SetStep(1);
	CString proText;
	proText.Format(L"0/%d",nLen);
	GetDlgItem(IDC_PG_TEXT)->SetWindowTextW(proText);
	//UpdateData(FALSE);
	//列表的行数
	int rowCount=0;
	taskCount = nLen;
	for (long i = 0; i < nLen; ++i) //遍历子节点
	{
		proText.Format(L"%d/%d",i,nLen);
		GetDlgItem(IDC_PG_TEXT)->SetWindowTextW(proText);
		c_progress.StepIt();
		URLPARAMS *urlParams = new URLPARAMS;
		CComPtr<IXMLDOMNode> spNode;
		spNodeList->get_item(i, &spNode);
		CComPtr<IXMLDOMNodeList> spUrlParamList;
		spNode->get_childNodes(&spUrlParamList);
		long pLen;
		spUrlParamList->get_length(&pLen);
		CString cjName;
		int cjReflushTime;
		
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
				 cjName = CString(value);
				 
			}else if(strValue == L"FUTURE"){
				spParamNode->get_text(&value);
				CString _FUTURE = CString(value);
				urlParams->is_future = _FUTURE==L"Y"?TRUE:FALSE;
			}else if(strValue == L"FLUSH_TIME"){
				spParamNode->get_text(&value);
				cjReflushTime = _ttoi(CString(value));
			}
			urlParams->uid = cuid;

			spParamNode.Release();
			
		}
		
		//采集url
		url.init((LPVOID)urlParams);
		CString urlstr = url.GenerateUrl();
		if(!hasCheckNet)
		{
			hasCheckNet = TRUE;
			//提取页数
			CString htmlData;
			try
			{
				htmlData = Helper::GetHttpFileData(urlstr);
			}
			catch(...)
			{
				AfxMessageBox(L"网络请求失败！");
				SwitchBtn(TRUE,FALSE);
				return;
			}
			int pos = htmlData.Find(L"window.open");
			if(pos>0)
			{
				AfxMessageBox(L"用户在另外一个地方登陆，请重新登陆");
				ShowLoginDlg();
				return ;
			}
		}
		
		CMenu *pMenu = this->GetSystemMenu(FALSE);  
		pMenu->EnableMenuItem(SC_CLOSE,MF_DISABLED);     //关闭按钮变灰
		
		rowCount = i;
		CAIJI_TASK *cjt = new CAIJI_TASK;//采集任务结构体
		CString realUrl;
		//realUrl.Format(L"%s&page_no=%d",urlstr,k);
		cjt->db = &db;
		cjt->threadList = &m_list;
		cjt->url=urlstr;
		cjt->count=0;
		cjt->name = cjName;
		cjt->flush = cjReflushTime;
		cjt->logDir = log_dir;
		cjt->pro = &c_progress;
		cjt->proText = &c_static_pro;
		cjt->dlg = this;
		cjt->taskCount = taskCount;
		m_list.SetRedraw(FALSE);
		  
		CString ID;
		ID.Format(L"%d",(i+1));
		int row=m_list.InsertItem(rowCount,ID); //编号
		m_list.SetItemText(row,2,cjt->url);//url
		CString count,flush;
		count.Format(_T("%d"), cjt->count);
		flush.Format(_T("%d"), cjt->flush);
		m_list.SetItemText(row,1,cjt->name);//名称
		m_list.SetItemText(row,4,count); //采集到的数量
		m_list.SetItemText(row,5,flush);//刷新倒计时
		cjt->row = row;
		m_list.SetRedraw(TRUE);
		m_list.Invalidate();
		m_list.UpdateWindow();
		int nCount =m_list.GetItemCount();
		if (nCount > 0)
		   m_list.EnsureVisible(nCount-1, FALSE);
		//开启一个采集线程
		AfxBeginThread(CaijiThreadFunction,(LPVOID)cjt); 
		spNode.Release();
		spUrlParamList.Release();
		//delete urlParams;
	}
	proText=L"启动完成";
	GetDlgItem(IDC_PG_TEXT)->SetWindowTextW(proText);
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
	SwitchBtn(FALSE,FALSE);
	CMenu *pMenu = this->GetSystemMenu(FALSE);  
    pMenu->EnableMenuItem(SC_CLOSE,MF_DISABLED);     //关闭按钮变灰

	c_progress.SetRange(0,tc);
	c_progress.SetStep(1);
	CString proText;
	proText.Format(L"关闭 0/%d",tc);
	c_static_pro.SetWindowTextW(proText);

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
		db.init();
		  AfxMessageBox(L"保存成功");
	}
}

int CCaiJDlg::getSTYPE(CString stype){
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



bool CCaiJDlg::CheckDbConnect()
{
	const char *host = db.GetHost();
	const char *user = db.GetUser();
	const char *passwd = db.GetPassword();
	const char *dbName = db.GetDbName();
	UINT port = db.GetPort();
	MYSQL * con;
	con = mysql_init((MYSQL*) 0);
	if(!mysql_real_connect(con,//连接mysql
							host,
							user,
							passwd,
							dbName,
							port,
							NULL,
							0))
	{
		const char *err = mysql_error(con);
		CString errStr(err);
		mysql_close(con);
		AfxMessageBox(errStr);
		return FALSE;
	}
	mysql_close(con);
	return TRUE;
}
