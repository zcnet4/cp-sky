/* -------------------------------------------------------------------------
//	FileName	£º	base/port_zcnet4.h
//	Creator		£º	(zc)
//	CreateTime	£º	2013-08-17 14:13:25
//	Description	£º	
//
// -----------------------------------------------------------------------*/
#ifndef BASE_PORT_ZCNET4_H_
#define BASE_PORT_ZCNET4_H_

// -------------------------------------------------------------------------

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

#if defined(OS_POSIX)

#include <errno.h>

#define HANDLE_EINTR(x) ({ \
	typeof(x) __eintr_result__; \
	do { \
	__eintr_result__ = (x); \
	} while (__eintr_result__ == -1 && errno == EINTR); \
	__eintr_result__;\
})

#else

#define HANDLE_EINTR(x) (x)

#endif  // OS_POSIX

// -------------------------------------------------------------------------

#endif /* BASE_PORT_ZCNET4_H_ */
