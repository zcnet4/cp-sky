/* -------------------------------------------------------------------------
//	FileName		��	allocated_object.h
//	Creator			��	(zc)
//	CreateTime	��	2015-2-28 15:07
//	Description	��	
//
// -----------------------------------------------------------------------*/
#ifndef ALLOCATED_OBJECT_H_
#define ALLOCATED_OBJECT_H_

// -------------------------------------------------------------------------
// Anything that has done a #define new <blah> will screw operator new definitions up
// so undefine
#ifdef new
#  undef new
#endif
#ifdef delete
#  undef delete
#endif

/** Superclass for all objects that wish to use custom memory allocators
when their new / delete operators are called.
Requires a template parameter identifying the memory allocator policy
to use (e.g. see StdAllocPolicy).
*/
template <class Alloc>
class AllocatedObject
{
public:
  explicit AllocatedObject()
  { }

  ~AllocatedObject()
  { }

  /// operator new, with debug line info
  void* operator new(size_t sz, const char* file, int line, const char* func)
  {
    return Alloc::allocateBytes(sz, file, line, func);
  }

  void* operator new(size_t sz)
  {
    return Alloc::allocateBytes(sz);
  }

    /// placement operator new
  void* operator new(size_t sz, void* ptr)
  {
    (void)sz;
    return ptr;
  }

    /// array operator new, with debug line info
  void* operator new[](size_t sz, const char* file, int line, const char* func)
  {
    return Alloc::allocateBytes(sz, file, line, func);
  }

  void* operator new[](size_t sz)
  {
    return Alloc::allocateBytes(sz);
  }

    void operator delete(void* ptr)
  {
    Alloc::deallocateBytes(ptr);
  }

  // Corresponding operator for placement delete (second param same as the first)
  void operator delete(void* ptr, void*)
  {
    Alloc::deallocateBytes(ptr);
  }

  // only called if there is an exception in corresponding 'new'
  void operator delete(void* ptr, const char*, int, const char*)
  {
    Alloc::deallocateBytes(ptr);
  }

  void operator delete[](void* ptr)
  {
    Alloc::deallocateBytes(ptr);
  }


  void operator delete[](void* ptr, const char*, int, const char*)
  {
    Alloc::deallocateBytes(ptr);
  }

};
// -------------------------------------------------------------------------
#endif /* ALLOCATED_OBJECT_H_ */
