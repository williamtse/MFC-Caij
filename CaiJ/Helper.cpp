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
 * @szDelimiter : �ָ��
 * @strContent  : �����ҵ��ַ���
 * @arrDest     : ����ָ�֮�������
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
