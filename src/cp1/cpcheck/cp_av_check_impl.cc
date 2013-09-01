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
#include <Msi.h>
#include "base/win/registry.h"
#include "base/string16.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
//
#define PRODUCT_GUID_PATH L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Installer\\UserData\\S-1-5-18\\Products"
//////////////////////////////////////////////////////////////////////////
// INSTALLED_SOFT_ITEM
struct CPAvCheckImpl::INSTALLED_SOFT_ITEM
{
	string16 strName;
	string16 strGUID;
};

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
	// 部分GUID会枚举不到
	for(int i = 0; ; i++) {
		WCHAR guid_buffer[MAX_GUID_CHARS+1] = { 0 };
		DWORD dwRet = MsiEnumProducts(i, guid_buffer);
		if( dwRet != ERROR_SUCCESS )
			break;

		WCHAR guid_name[MAX_PATH] = { 0 };
		DWORD dwLen = MAX_PATH;
		dwRet = MsiGetProductInfo(guid_buffer,
			INSTALLPROPERTY_INSTALLEDPRODUCTNAME ,
			guid_name,
			&dwLen  
			);
		if( dwRet == ERROR_SUCCESS ) {
			INSTALLED_SOFT_ITEM soft_item;
			//soft_item.strGUID = guid_buffer;
			//soft_item.strName = guid_name;
			m_GuidList.push_back(soft_item);
		}

	}

	//直接通过注册表枚举
	base::win::RegistryKeyIterator enumProduct(HKEY_LOCAL_MACHINE, PRODUCT_GUID_PATH);
	WCHAR szFullPath[MAX_PATH*2] = {0};
	WCHAR szValue[MAX_PATH] = {0};
	base::win::RegKey reg_key;
	while (enumProduct.Valid()) {
		INSTALLED_SOFT_ITEM soft_item;

		StringCbPrintf(szFullPath, sizeof(szFullPath), L"%s\\%s\\InstallProperties", PRODUCT_GUID_PATH, enumProduct.Name());
		reg_key.Open(HKEY_LOCAL_MACHINE, szFullPath, PERM_READ);
		reg_key.ReadValue(L"DisplayName", &soft_item.strName);

		string16 unistallString;
		reg_key.ReadValue(L"UninstallString", &unistallString);
		{
			WCHAR* pos1 = NULL;
			WCHAR* pos2 = NULL;
			pos1 = StrChr(szValue, L'{');
			pos2 = StrChr(szValue, L'}');
			if(pos1 && pos2)
			{
				DWORD dwLen = (DWORD)(pos2-pos1+1) * sizeof(WCHAR);

				soft_item.strGUID = CString(pos1, dwLen);
				if( soft_item.strGUID.GetLength() != 0 )
					m_GuidList.AddTail(soft_item);
			}
		}
		// next
		enumProduct++;
	}

	POSITION iPos = m_GuidList.GetHeadPosition();
	while ( iPos != NULL )
	{
		const INSTALLED_SOFT_ITEM& si = m_GuidList.GetNext(iPos);

		ATLTRACE(L"%s, %s\n", si.strName, si.strGUID);
	}

	return TRUE;
}

// -------------------------------------------------------------------------
