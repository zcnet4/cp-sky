/* -------------------------------------------------------------------------
//	FileName	£º	cpcheck_export.h
//	Creator		£º	(zc)
//	CreateTime	£º	2013-05-29 9:47:12
//	Description	£º	
//
// -----------------------------------------------------------------------*/
#ifndef CPCHECK_EXPORT_H_
#define CPCHECK_EXPORT_H_

// -------------------------------------------------------------------------
#if defined(COMPONENT_BUILD) && !defined(COMPILE_CPCHECK_STATICALLY)
#if defined(WIN32)

#if defined(CPCHECK_IMPLEMENTATION)
	#define CPCHECK_EXPORT __declspec(dllexport)
#else
	#define CPCHECK_EXPORT __declspec(dllimport)
#endif  // defined(CPCHECK_IMPLEMENTATION)

#else // defined(WIN32)
	#if defined(CPCHECK_IMPLEMENTATION)
		#define CPCHECK_EXPORT __attribute__((visibility("default")))
	#else
		#define CPCHECK_EXPORT
	#endif
#endif

#else // defined(COMPONENT_BUILD)
	#define CPCHECK_EXPORT
#endif

// -------------------------------------------------------------------------

#endif /* CPCHECK_EXPORT_H_ */
