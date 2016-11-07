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
void static Helper::StrExplode(TCHAR szDelimiter, CString strContent, CStringArray &arrDest){
	arrDest.RemoveAll();
	int nOffset = 0;
	while (true)
	{
		nOffset = strContent.ReverseFind(szDelimiter);
		if (nOffset == -1)
		{
			if (!strContent.IsEmpty())
				arrDest.Add(strContent);
			break;
		}
		CString strItem = strContent.Mid(nOffset+1);
		if (!strItem.IsEmpty())
		{
			arrDest.Add(strItem);
		}
		strContent = strContent.Left(nOffset);
	}
}

void static Helper::Combine(CStringArray arr1,CStringArray arr2,std::map<CString,CString> &dest){
	int arr1Count = arr1.GetCount();
	int arr2Count = arr2.GetCount();
	int count;
	count = arr1Count>arr2Count ? arr2Count:arr1Count;
	for(int i;i<count;i++){
		dest.insert(pair<CString,CString>(arr1[i],arr2[i]));
	}
}
