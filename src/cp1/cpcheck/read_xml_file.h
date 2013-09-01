/* -------------------------------------------------------------------------
//	FileName	£∫	cp1/cpcheck/read_xml_file.h
//	Creator		£∫	(pkl)
//	CreateTime	£∫	2013/9/1 20:42:26
//	Description	£∫	
//
// -----------------------------------------------------------------------*/
#ifndef CP1_CPCHECK_READ_XML_FILE_H_
#define CP1_CPCHECK_READ_XML_FILE_H_
#include <vector>
#include "cpcheck_export.h"
// -------------------------------------------------------------------------

class CPCHECK_EXPORT ReadXmlFile
{
private:
	struct XML_CONTENT;
	typedef std::vector<XML_CONTENT> t_vect;

public:
	/*
	@func		: test
	@brief		:  «∑Òø…”√°£
	*/
	void test();

	bool loadXmlInfo(const char* pszXml, ReadXmlFile::t_vect &arrXml);


};

// -------------------------------------------------------------------------

#endif /* CP1_CPCHECK_READ_XML_FILE_H_ */
