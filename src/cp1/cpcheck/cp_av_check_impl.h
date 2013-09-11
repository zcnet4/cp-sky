/* -------------------------------------------------------------------------
//	FileName	：	cp1/cpcheck/cp_av_check_impl.h
//	Creator		：	(zc)
//	CreateTime	：	2013/9/1 17:00:47
//	Description	：	
//
// -----------------------------------------------------------------------*/
#ifndef CP1_CPCHECK_CP_AV_CHECK_IMPL_H_
#define CP1_CPCHECK_CP_AV_CHECK_IMPL_H_
#include <vector>
#include "common/zccommon.h"
// -------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
//
class FilePath;
//////////////////////////////////////////////////////////////////////////
// CPAvCheckImpl检测杀毒软件实现
class CPAvCheckImpl
{
public:
	/*
	@func		: LoadAVLib
	@brief		: 是否可用。
	*/
	bool LoadAvData(const FilePath& av_file_path);
	/*
	@func		: CheckAv
	@brief		: 是否可用。
	*/
	PCStr CheckAv();
protected:
	/*
	@func		: LoadSoftGuid
	@brief		: 是否可用。
	*/
	bool LoadSoftGuid();
private:
	struct PRODUCT_INFO;
	struct COMPANY_INFO;
	typedef std::vector<COMPANY_INFO> t_vecCompanyInfos;
	struct INSTALLED_SOFT_ITEM;
	typedef std::vector<INSTALLED_SOFT_ITEM> t_vecInstalledSoftInfos;
	class CPAvLibParse;
private:
	t_vecCompanyInfos m_CompanyList;
	t_vecInstalledSoftInfos m_GuidList;
};

// -------------------------------------------------------------------------

#endif /* CP1_CPCHECK_CP_AV_CHECK_IMPL_H_ */
