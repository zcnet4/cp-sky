/* -------------------------------------------------------------------------
//	FileName	£º	cp1/cp_main.cc
//	Creator		£º	(pkl)
//	CreateTime	£º	2013/9/1 21:49:39
//	Description	£º	
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
