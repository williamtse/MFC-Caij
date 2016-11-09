#pragma once
#include <string>
class Query
{
public:
	Query(void);
	~Query(void);
	void queryBodyVar(CString html,CStringArray &matches);
	int getPageNum(CString html);
	CString getHeaderStr(CString html);
	CString GetFirstMatch(CString html,CString begin,CString end);
};
