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

char* Helper::UnicodeToUtf8(CString unicode)  
{  
    int len;    
    len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)unicode, -1, NULL, 0, NULL, NULL);    
    char *szUtf8=new char[len + 1];  
    memset(szUtf8, 0, len * 2 + 2);  
    WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)unicode, -1, szUtf8, len, NULL,NULL);  
    return szUtf8;  
}  
void wcharTochar(const wchar_t *wchar, char *chr, int length)  
{  
    WideCharToMultiByte( CP_ACP, 0, wchar, -1,  
        chr, length, NULL, NULL );  
}  
//CString转const char*
const char * Helper::CTCC(CString theString){
	int    sizeOfString = (theString.GetLength() + 1); 
	LPTSTR  lpsz = new TCHAR[ sizeOfString ]; 
	_tcscpy_s(lpsz, sizeOfString, theString);
	char *chr = new char[sizeOfString];
	wcharTochar(lpsz, chr, sizeOfString);  
	return chr;
}

CString Helper::GetWorkDir() 
{  
	CString strExePath;  
	 GetModuleFileName(NULL, strExePath.GetBuffer(MAX_PATH), MAX_PATH);  
	 strExePath.ReleaseBuffer();  
	 strExePath = strExePath.Left(strExePath.ReverseFind(_T('\\')));  
	 return strExePath;  
}

CString Helper::ExtraUid(CString html)
{
	int pos1 = html.Find(L"||");
	CString r = html.Mid(pos1+2);
	int pos2 = r.Find(L"|");
	CString uid = r.Left(pos2);
	return uid;
}