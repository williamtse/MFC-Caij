#pragma once
#include <map>

typedef std::map<CString,CString> UDT_MAP_CSTRING_CSTRING;

class Query
{
public:
	Query(void);
	~Query(void);
	std::map<int,UDT_MAP_CSTRING_CSTRING> queryBodyVar(CString html);
	int getPageNum(CString html);
	CString getHeaderStr(CString html);
	std::string GetFirstMatch(CString html,std::string pattern);
};
