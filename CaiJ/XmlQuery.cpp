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
	//初始化COM
	CoInitialize(NULL);
	//读取XML
	CComPtr<IXMLDOMDocument> spDoc; //DOM
	spDoc.CoCreateInstance(CLSID_DOMDocument);//创建文档对象
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
	handler->get_documentElement(&spRootEle); //根节点
}

void XmlQuery::GetNodes(XMLDOMELEMENT &root,XMLDOMNODELIST &spNodeList)
{
	root->get_childNodes(&spNodeList); //子节点列表
}

void XmlQuery::GetItem(XMLDOMNODELIST &spUrlParamList,int j,XMLDOMNODE &spParamNode)
{
	spUrlParamList->get_item(j,&spParamNode);
}

CString XmlQuery::GetNodeName(CComPtr<IXMLDOMNode> &spParamNode)
{
	CComBSTR name;
	spParamNode->get_nodeName(&name);//URL参数节点的标签名
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
