#include "StdAfx.h"
#include "Helper.h"

Helper::Helper(void)
{
}

Helper::~Helper(void)
{
}

/*
 * Author : maben
 * Date   : 2014-08-06
 * @szDelimiter : 分割符
 * @strContent  : 待查找的字符串
 * @arrDest     : 保存分割之后的数组
 */
void Helper::StrExplode(TCHAR szDelimiter, CString strContent, CStringArray &arrDest){
	arrDest.RemoveAll();
	int nOffset = 0;
	while (true)
	{
		nOffset = strContent.Find(szDelimiter);
		if (nOffset == -1)
		{
			if (!strContent.IsEmpty())
				arrDest.Add(strContent);
			break;
		}
		CString strItem = strContent.Mid(0,nOffset);
		if (!strItem.IsEmpty())
		{
			arrDest.Add(strItem);
		}
		strContent = strContent.Mid(nOffset+1);
	}
}

void Helper::Combine(CStringArray &arr1,CStringArray &arr2,std::map<CString,CString> &dest){
	int arr1Count = arr1.GetCount();
	int arr2Count = arr2.GetCount();
	int count;
	count = arr1Count>arr2Count ? arr2Count:arr1Count;
	for(int i=0;i<count;i++){
		dest.insert(std::pair<CString,CString>(arr1[i],arr2[i]));
	}
}

CString Helper::TrimSQuot(CString str)
{
	CString re;
	re = str.Mid(1,str.GetLength()-2);
	return re;
}

void Helper::FiltKG(CString &html)
{
	html.Replace(_T(" "),_T(""));
}

char* __stdcall Helper::UnicodeToUtf8( const WCHAR* wstr )
{
    const WCHAR* w;
    // Convert unicode to utf8
    int len = 0;
    for ( w = wstr; *w; w++ ) {
        if ( *w <0x0080 ) len++;
        else if ( *w <0x0080 ) len += 2;
        else len += 3;
    }

    unsigned char* szOut = ( unsigned char* )malloc( len+1 );

    if ( szOut == NULL )
        return NULL;

    int i = 0;
    for ( w = wstr; *w; w++ ) {
        if ( *w <0x0080 )
            szOut[i++] = ( unsigned char ) *w;
        else if ( *w < 0x0080 ) {
            szOut[i++] = 0xc0 | (( *w ) >> 6 );
            szOut[i++] = 0x80 | (( *w ) & 0x3f );
        }
        else {
            szOut[i++] = 0xe0 | (( *w ) >> 12 );
            szOut[i++] =0x80 | (( ( *w ) >> 6 ) & 0x3f );
            szOut[i++] = 0x80 | (( *w ) & 0x3f );
        }    }

    szOut[ i ] = '\0';
    return ( char* )szOut;
}



const char * Helper::CTCC(CString cstr){
    // 先得到要转换为字符的长度
    const size_t strsize=(cstr.GetLength()+1)*2; // 宽字符的长度;
    char * pstr= new char[strsize]; //分配空间;
    size_t sz=0;
    wcstombs_s(&sz,pstr,strsize,cstr,_TRUNCATE);
    return pstr;
}

CString Helper::GetWorkDir() 
{  
	HMODULE module = GetModuleHandle(0);
	char pFileName[MAX_PATH];
	GetModuleFileName(module, (LPWCH)pFileName, MAX_PATH);
	CString csFullPath(pFileName);
	int nPos = csFullPath.ReverseFind( _T('\\') );
	if( nPos < 0 )
		return CString("");
	else
		return csFullPath.Left( nPos );
}