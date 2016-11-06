#include "StdAfx.h"
#include "Url.h"

Url::Url(URLPARAMS urlParams)
{
	uid = urlParams.uid;
	stype = urlParams.stype;
	script = urlParams.script;
	rtype = urlParams.rtype;
	pageNum = urlParams.pageNum;
	is_future = urlParams.is_future;
}

Url::~Url(void)
{
}

Url::GenerateUrl(){
	CString url;
	switch(script){
		case L"body_var":
			url = SITE+L"/app/member/"+stype+L"_"+(is_future?L"future":L"browse")
				+L"/body_var.php?uid="
				+cuid+L"&rtype="+rtype+L"&langx=zh-cn&mtype=3&page_no="+pageNum;
			break;
		case L"reloadgame":
			url = SITE+L"/app/member/browse_FS/reloadgame_R.php?FStype="+stype+L"&rtype="+rtype+L"&uid="
						+uid+L"langx=zh-cn";
			break;
		case L"result":
			SYSTEMTIME st; 　　
			CString strDate,strTime; 　　
			GetLocalTime(&st); 　　
			strDate.Format("%4d-%2d-%2d",st.wYear,st.wMonth,st.wDay);
			CString file;
			switch(stype){
				case 'FT':
				case 'BK':
				case 'BS':
				case 'OP':
					file=L"result.php";
				break;
				default:
					file=L"result_"+stype+L".php";
					break;
			}
			url = SITE+L"/app/member/result/"+file+L"?game_type="+stype+L"&list_date="+strDate+L"&uid="
						+uid+L"langx=zh-cn";
			break;
	}
	return url;
}