/* -------------------------------------------------------------------------
//	FileName		：	mempool.h
//	Creator			：	(zc)
//	CreateTime	：	2015-2-28 16:44
//	Description	：	
//
// -----------------------------------------------------------------------*/
#ifndef C_POOL_H_
#define C_POOL_H_
#include "k2_api.h"
// -------------------------------------------------------------------------

class K2_API CPool
{
public:
  CPool(const char* name, uint32_t slices_size, uint32_t slices_count = 0);
  ~CPool();
  /*
  @func			: allocate
  @brief		: 
  */
  void* allocate();
  /*
  @func			: deallocate
  @brief		: 
  */
  void deallocate(void* p);
  /*
  @func			: clear
  @brief		: 
  */
  void clear();
private:
  const char* name_;
  struct _MEM_CHUNK* chunks_;
  struct _MEM_SLICE* free_slices_;
  const uint32_t slice_count_;    //内存切片个数。
  const uint32_t slice_size_;     //内存切片大小。
  int32_t allocated_total_;       //已分配的个数。
  int32_t free_total_;            //free个数。
};

template <class T>
class CPoolT
{
public:
  CPoolT() 
    : pool_("CPoolT", sizeof(T))
  {

  }
  T* allocate() {
    return static_cast<T*>(pool_.allocate());
  }
  void deallocate(T* p) {
    pool_.deallocate(p);
  }
  void clear() {
    pool_.clear();
  }
private:
  CPool   pool_;
};

//////////////////////////////////////////////////////////////////////////
#include "allocated_object.h"
//////////////////////////////////////////////////////////////////////////
template <class T, int N = 0>
class CPoolObjectT
  : public AllocatedObject<CPoolObjectT<T>>
{
public:
  static void* allocateBytes(size_t sz, const char* file, int line, const char* func)
  {
    assert(sz == sizeof(T));
    return kPool.allocate();
  }

  static void* allocateBytes(size_t sz)
  {
    assert(sz == sizeof(T));
    return kPool.allocate();
  }

  static void deallocateBytes(void* ptr) {
    kPool.deallocate(static_cast<T*>(ptr));
  }
  static void Reset() {
    kPool.clear();
  }
private:
  static CPool kPool;
};

#if 0
  #define DEFINE_CPOOL_ALLOCATED(theClass) CPoolT<theClass> CPoolObjectT<theClass>::kPool("", sizeof(theClass), 0);
#else
  #define DEFINE_CPOOL_ALLOCATED(theClass)
  template<typename T, int N>
  CPool CPoolObjectT<T, N>::kPool("", sizeof(T), N);
#endif
// -------------------------------------------------------------------------
#endif /* C_POOL_H_ */
