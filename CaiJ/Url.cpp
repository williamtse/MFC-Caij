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
			break;
		case L"result":
			switch(stype){
				case 'FT':
				case 'BK':
				case 'BS':
				case 'OP':
					url = SITE+L"/app/member/result/result.php?game_type="+stype+L"&list_date="
					break;
			}
			break;
	}
}