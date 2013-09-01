/* -------------------------------------------------------------------------
//	FileName	：	cp1/cpcheck/cp_av_check_impl.cc
//	Creator		：	(zc)
//	CreateTime	：	2013/9/1 17:00:53
//	Description	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "cp1/cpcheck/cp_av_check_impl.h"
#include "common/xml/hm_xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
// PRODUCT_INFO
struct CPAvCheckImpl::PRODUCT_INFO
{
	string8 strName;
	string8 strGUIDName;
	string8 strLanguage;
	string8 strExpression;
	string8 strUninstallCMD;
};

//////////////////////////////////////////////////////////////////////////
// COMPANY_INFO
struct CPAvCheckImpl::COMPANY_INFO 
{
	string8 strName;
	string8 strURL;
	string8 strExpression;
	std::vector<PRODUCT_INFO> ProductList;
};

//////////////////////////////////////////////////////////////////////////
// CPAvLibParse
class CPAvCheckImpl::CPAvLibParse
{
public:
	/*
	@func		: loadCompanyInfo
	@brief		: 是否可用。
	*/
	bool loadCompanyInfo(const char* pszXml, CPAvCheckImpl::t_vecCompanyInfos &arrCompany);
protected:
	/*
	@func		: GetProductInfo
	@brief		: 是否可用。
	*/
	bool GetProductInfo(base::HMXmlElement productsElement, std::vector<PRODUCT_INFO> &arrProduct);
};

/*
@func		: loadCompanyInfo
@brief		: 是否可用。
*/
bool CPAvCheckImpl::CPAvLibParse::loadCompanyInfo(const char* pszXml, CPAvCheckImpl::t_vecCompanyInfos &arrCompany)
{
	if (!IsValidString(pszXml)) return false;
	//
	base::HMXmlDocument2 xmlDoc;
	xmlDoc.loadFromString(pszXml);
	if (!xmlDoc.isError()) return false;
	//
	base::HMXmlElement rootElem = xmlDoc.rootElement();
	//	
	arrCompany.clear();
	for (base::HMXmlElement nodeElem = rootElem.firstChildElement();
		nodeElem.isValid();
		nodeElem = nodeElem.nextSiblingElement())
	{
		CPAvCheckImpl::COMPANY_INFO ci;

		ci.strName = nodeElem.attribute("name");
		ci.strURL = nodeElem.attribute("url");
		//
		base::HMXmlElement expressiondElem = nodeElem.firstChildElement("expression");
		if (expressiondElem.isValid()) {
			ci.strExpression = expressiondElem.getText();
		}
		//
		GetProductInfo(nodeElem.firstChildElement("products"), ci.ProductList );
		//
		arrCompany.push_back(ci);
	}
	//
	return !arrCompany.empty();
}

/*
@func		: GetProductInfo
@brief		: 是否可用。
*/
bool CPAvCheckImpl::CPAvLibParse::GetProductInfo(base::HMXmlElement productsElement, std::vector<PRODUCT_INFO> &arrProduct)
{
	if (!productsElement.isValid()) return false;
	//
	arrProduct.clear();
	for (base::HMXmlElement nodeElem = productsElement.firstChildElement();
		nodeElem.isValid();
		nodeElem = nodeElem.nextSiblingElement())
	{
		PRODUCT_INFO pi;

		pi.strName = nodeElem.attribute("name");
		pi.strGUIDName = nodeElem.attribute("guidname");
		pi.strLanguage = nodeElem.attribute("language");
		//
		base::HMXmlElement expressionElem = nodeElem.firstChildElement("expression");
		if (expressionElem.isValid()) {
			pi.strExpression = expressionElem.getText();
		}
		//
		base::HMXmlElement uninstallcmdElem = nodeElem.firstChildElement("uninstallcmd");
		if (uninstallcmdElem.isValid()) {
			pi.strUninstallCMD = uninstallcmdElem.getText();
		}
		//
		arrProduct.push_back(pi);
	}
	// 
	return !arrProduct.empty();
}

//////////////////////////////////////////////////////////////////////////
//
/*
@func		: loadCompanyInfo
@brief		: 是否可用。
*/
bool CPAvCheckImpl::LoadAvData(const FilePath& av_file_path)
{
	//
	string8 xml_data;
	//
	CPAvLibParse av_lib_pares;
	if (!av_lib_pares.loadCompanyInfo(xml_data.c_str(), m_CompanyList)) return false;

	//
	return LoadSoftGuid();
}


/*
@func		: LoadSoftGuid
@brief		: 是否可用。
*/
bool CPAvCheckImpl::LoadSoftGuid()
{
	return false;
}

// -------------------------------------------------------------------------
