/* -------------------------------------------------------------------------
//	FileName	��	cp1/cp_main.cc
//	Creator		��	(pkl)
//	CreateTime	��	2013/9/1 21:49:39
//	Description	��	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "cpcheck/read_xml_file.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
int main() 
{
	ReadXmlFile xml;
	
	xml.test();

	return 0;
}


// -------------------------------------------------------------------------
