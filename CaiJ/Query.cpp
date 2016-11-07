#include "StdAfx.h"
#include "Query.h"
#include <boost/regex.hpp>
#include "Helper.h"
Query::Query(void)
{
}

Query::~Query(void)
{
}

//解析赛事页面
std::map<int,UDT_MAP_CSTRING_CSTRING> Query::queryBodyVar(CString html){
	std::map<int,UDT_MAP_CSTRING_CSTRING> matches;
	UDT_MAP_CSTRING_CSTRING matchRow;

	std::string htmlstr= (CStringA)html;

	CString headerStr = getHeaderStr(html);
	CStringArray headerArr; 
	Helper::StrExplode(_T(','),headerStr,headerArr);
	
	boost::regex expression("g\\(([^\n]*)\\);");  
	boost::smatch what;  
	std::string::const_iterator start = htmlstr.begin();  
	std::string::const_iterator end = htmlstr.end(); 
	int mc = 0;
	int i=0;
	while ( boost::regex_search(start, end, what, expression) )  
	{  
		mc++;
		CString matchstr;
		matchstr = CA2T(what[0].str().c_str());
		//ctj->cresult->AddString(matchstr);
		CStringArray dataRow;
		Helper::StrExplode(_T(','), matchstr, dataRow);

		UDT_MAP_CSTRING_CSTRING dest;
		Helper::Combine(headerArr,dataRow,dest);
		matches.insert(std::pair<int,UDT_MAP_CSTRING_CSTRING>(i,dest));

		start = what[0].second;
		i++;
	}  

	 return matches;
}

int Query::getPageNum(CString html){
	std::string pageNumStr;
	pageNumStr = GetFirstMatch(html,"t_page=([\\d]*);");
	return atoi(pageNumStr.c_str());
}

CString Query::getHeaderStr(CString html){
	std::string headerStr;
	headerStr = GetFirstMatch(html,"_\\.GameHead = \\[(.*)\\];");
	return CA2T(headerStr.c_str());
}

std::string Query::GetFirstMatch(CString html,std::string pattern){
	std::string htmlstr= (CStringA)html;
	boost::regex expression(pattern);
	boost::smatch what;
	std::string::const_iterator start = htmlstr.begin();  
	std::string::const_iterator end = htmlstr.end();
	std::string matchstr;
	while(boost::regex_search(start, end, what, expression)){
		matchstr = what[0].str();
		break;
	}
	return matchstr;
}

