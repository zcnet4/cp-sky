/* -------------------------------------------------------------------------
//	FileName	£∫	cp1/cpcheck/cp_av_check_impl.cc
//	Creator		£∫	(zc)
//	CreateTime	£∫	2013/9/1 17:00:53
//	Description	£∫	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "cp1/cpcheck/read_xml_file.h"
#include "common/xml/hm_xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////

struct ReadXmlFile::XML_CONTENT 
{
	string8 strName;
	string8 strUrl;
};

bool ReadXmlFile::loadXmlInfo(const char* pszXml,ReadXmlFile::t_vect &arrXml)
{
	if (!IsValidString(pszXml)) return false;
	//
	base::HMXmlDocument2 xmlDoc;
	xmlDoc.loadFromString(pszXml);
	if (!xmlDoc.isError()) return false;
	//
	base::HMXmlElement rootElem = xmlDoc.rootElement();
	//
	arrXml.clear();
	for (base::HMXmlElement nodeElem = rootElem.firstChildElement();
		nodeElem.isValid();
		nodeElem = nodeElem.nextSiblingElement())
	{
		ReadXmlFile::XML_CONTENT xc;

		xc.strName = nodeElem.attribute("name");
		xc.strUrl = nodeElem.attribute("url");

		//
		arrXml.push_back(xc);
	}
	//
	return !arrXml.empty();
}

/*
@func		: test
@brief		:  «∑Òø…”√°£
*/
void ReadXmlFile::test()
{

}