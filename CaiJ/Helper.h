#pragma once
#include<map>
class Helper
{
public:
	Helper(void);
	~Helper(void);
	void static StrExplode(TCHAR szDelimiter, CString strContent, CStringArray& arrDest);
	void static Combine(CStringArray &arr1,CStringArray &arr2,std::map<CString,CString> &dest);
	CString static Helper::TrimSQuot(CString str);
};
