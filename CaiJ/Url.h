#pragma once
#define SITE L"http://66.133.87.55"

struct URLPARAMS
{
	CString stype;
	CString script;
	CString uid;
	CString rtype;
	CString pageNum;
	bool is_future;
};

class Url
{
public:
	Url(URLPARAMS urlParams);
	~Url(void);
	CString GenerateUrl();
public:
	CString uid;
	CString stype;
	CString script;
	CString rtype;
	CString pageNum;
	bool is_future;
};
