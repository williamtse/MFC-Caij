#include "StdAfx.h"
#include "Query.h"
#include "Helper.h"
Query::Query(void)
{
}

Query::~Query(void)
{
}

//解析赛事页面
void Query::queryBodyVar(CString html,CStringArray &matches){
	CString headerStr = getHeaderStr(html);
	CStringArray headerArr; 
	//Helper::StrExplode(_T(','),headerStr,headerArr);

	int nOffset,eOffset;
	CString row;
	nOffset = html.Find(L"g([");
	if(nOffset>0){
		html = html.Mid(nOffset+3);
		while(nOffset>0){
			nOffset = html.Find(L"g([");
			row = html.Left(nOffset);
			eOffset = row.Find(L"]);");
			row = row.Left(eOffset);
			matches.Add(row);
			html = html.Mid(nOffset+3);
		}
	}
}

int Query::getPageNum(CString html){
	CString pageNumStr;
	pageNumStr = GetFirstMatch(html,L"t_page=",L";");
	return _ttoi(pageNumStr);
}

CString Query::getHeaderStr(CString html){
	CString headerStr;
	headerStr = GetFirstMatch(html,L"_.GameHead=[",L"];");
	return headerStr;
}

CString Query::GetFirstMatch(CString html,CString begin,CString end){
	CString match;
	int nOffset = html.Find(begin);
	html = html.Mid(nOffset+begin.GetLength());
	nOffset = html.Find(end);
	match = html.Left(nOffset);
	return match;
}



