#include "StdAfx.h"
#include "Url.h"

Url::Url(LPVOID urlParams)
{
    URLPARAMS *up = (URLPARAMS *)urlParams;
	type = up->type;
	sport = up->sport;
	uid = up->uid;
	stype = up->stype;
	rtype = up->rtype;
	is_future = up->is_future;
}

Url::~Url(void)
{
}

CString Url::GenerateUrl(){
	CString url;
	CString browse=is_future?L"future":L"browse";
	switch(type){
		case TYPE_BODY_VAR:
			url.Format(L"%s/app/member/%s_%s/body_var.php?uid=%s&rtype=%s&langx=zh-cn&mtype=3",SITE,stype,browse,uid,rtype);
			break;
		case TYPE_RELOADGAME:
			url.Format(L"%s/app/member/browse_FS/reloadgame_R.php?FStype=%s&rtype=%s&uid=%s&langx=zh-cn",SITE,stype,rtype,uid);
			break;
		case TYPE_RESULT:
			SYSTEMTIME st;
            CString strDate,strTime;
            GetLocalTime(&st);
			strDate.Format(L"%4d-%2d-%2d",st.wYear,st.wMonth,st.wDay);
			CString file;
			switch(sport){
				case STYPE_FT:
				case STYPE_BK:
				case STYPE_BS:
				case STYPE_OP:
					file=L"result.php";
				break;
				default:
					file.Format(L"result_%s.php",stype);
					break;
			}
			url.Format(L"%s/app/member/result/%s?game_type=%s&list_date=%s&uid=%s&langx=zh-cn",SITE,file,stype,strDate,uid);
			break;
	}
	return url;
}