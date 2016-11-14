#pragma once
typedef CComPtr<IXMLDOMElement> XMLDOMELEMENT;
typedef CComPtr<IXMLDOMNodeList> XMLDOMNODELIST;
typedef 	CComPtr<IXMLDOMNode> XMLDOMNODE;
class XmlQuery
{
public:
	XmlQuery(void);
	~XmlQuery(void);
protected:
CComPtr<IXMLDOMDocument>	handler;
public:
	bool load(CString filePath);
	void GetRoot(XMLDOMELEMENT &root);
	void GetNodes(XMLDOMELEMENT &root,XMLDOMNODELIST &spNodeList);
	void GetItem(XMLDOMNODELIST &spUrlParamList,int j,XMLDOMNODE &spParamNode);
	CString GetNodeName( XMLDOMNODE &spParamNode);
	CString GetNodeValue( XMLDOMNODE &spParamNode);
};
