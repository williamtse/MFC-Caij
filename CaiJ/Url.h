#pragma once
#define SITE L"http://66.133.87.55"
#define TYPE_BODY_VAR 1
#define TYPE_RELOADGAME 2
#define TYPE_RESULT 3

#define STYPE_FT 1
#define STYPE_BK 2
#define STYPE_BS 3
#define STYPE_OP 4

struct URLPARAMS
{
	UINT type;
	UINT sport;
	CString stype;
	CString uid;
	CString rtype;
	UINT pageNum;
	bool is_future;
};

class Url
{
public:
	Url(LPVOID urlParams);
	~Url(void);
	CString GenerateUrl();
public:
	UINT type;
	UINT sport;
	CString uid;
	CString stype;
	CString rtype;
	UINT pageNum;
	bool is_future;
};
