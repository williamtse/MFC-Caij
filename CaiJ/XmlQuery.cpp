#include "StdAfx.h"
#include "XmlQuery.h"

XmlQuery::XmlQuery(void)
{
	handler = NULL;
}

XmlQuery::~XmlQuery(void)
{
}

bool XmlQuery::load(CString filePath)
{
	//��ʼ��COM
	CoInitialize(NULL);
	//��ȡXML
	CComPtr<IXMLDOMDocument> spDoc; //DOM
	spDoc.CoCreateInstance(CLSID_DOMDocument);//�����ĵ�����
	VARIANT_BOOL vb;
	if(spDoc->load(CComVariant(filePath), &vb) == S_OK)
	{
		handler = spDoc;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void XmlQuery::GetRoot(XMLDOMELEMENT &spRootEle)
{
	handler->get_documentElement(&spRootEle); //���ڵ�
}

void XmlQuery::GetNodes(XMLDOMELEMENT &root,XMLDOMNODELIST &spNodeList)
{
	root->get_childNodes(&spNodeList); //�ӽڵ��б�
}

void XmlQuery::GetItem(XMLDOMNODELIST &spUrlParamList,int j,XMLDOMNODE &spParamNode)
{
	spUrlParamList->get_item(j,&spParamNode);
}

CString XmlQuery::GetNodeName(CComPtr<IXMLDOMNode> &spParamNode)
{
	CComBSTR name;
	spParamNode->get_nodeName(&name);//URL�����ڵ�ı�ǩ��
	CString re(name);
	return re;
}

CString XmlQuery::GetNodeValue(CComPtr<IXMLDOMNode> &spParamNode)
{
	CComBSTR value;
	spParamNode->get_text(&value);
	CString re(value);
	return re;
}
