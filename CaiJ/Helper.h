#pragma once
#include<map>
class Helper
{
public:
	Helper(void);
	~Helper(void);
	void static StrExplode(TCHAR szDelimiter, CString strContent, CStringArray& arrDest);
	void static Combine(CStringArray &arr1,CStringArray &arr2,std::map<CString,CString> &dest);
	CString static TrimSQuot(CString str);
	void static FiltKG(CString &html);
	static char* __stdcall Helper::UnicodeToUtf8( const WCHAR* wstr );
	static const char* CTCC(CString cstr);
	static CString GetWorkDir();
	static char* Helper::UnicodeToUtf8(CString unicode);
	static CString Helper::ExtraUid(CString html);
};
