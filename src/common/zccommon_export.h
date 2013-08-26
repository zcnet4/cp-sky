/* -------------------------------------------------------------------------
//	FileName	£º	common/zccommon_export.h
//	Creator		£º	(zc)
//	CreateTime	£º	2013-05-29 9:47:12
//	Description	£º	
//
// -----------------------------------------------------------------------*/
#ifndef COMMON_ZCCOMMON_EXPORT_H_
#define COMMON_ZCCOMMON_EXPORT_H_

// -------------------------------------------------------------------------
#if defined(COMPONENT_BUILD) && !defined(COMPILE_ZCCOMMON_STATICALLY)
#if defined(WIN32)

#if defined(ZCCOMMON_IMPLEMENTATION)
	#define ZCCOMMON_EXPORT __declspec(dllexport)
#else
	#define ZCCOMMON_EXPORT __declspec(dllimport)
#endif  // defined(YGCOMMON_IMPLEMENTATION)

#else // defined(WIN32)
	#if defined(ZCCOMMON_IMPLEMENTATION)
		#define ZCCOMMON_EXPORT __attribute__((visibility("default")))
	#else
		#define ZCCOMMON_EXPORT
	#endif
#endif

#else // defined(COMPONENT_BUILD)
	#define ZCCOMMON_EXPORT
#endif

#define ZCCOMMON_API ZCCOMMON_EXPORT
#include "base/basictypes.h"
// -------------------------------------------------------------------------

#endif /* COMMON_ZCCOMMON_EXPORT_H_ */
