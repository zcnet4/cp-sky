/* -------------------------------------------------------------------------
//	FileName		：	D:\yy_code\strife\trunk\src\k2\vector_pool.h
//	Creator			：	(zc)
//	CreateTime	：	2015-12-24 14:49
//	Description	：	
//
// -----------------------------------------------------------------------*/
#ifndef VECTOR_POOL_H_
#define VECTOR_POOL_H_
#include <vector>
// -------------------------------------------------------------------------
//句柄池，空间换时间。
template<typename T>
class VectorPoolT
{
public:
  VectorPoolT() : active_count_(0), next_node_id_(kInvalidId + 1)
  {
    clear();
  }
  void clear() {
    nodes_.resize(kReserved);
    nodes_[kUsedHead].next = kUsedHead;
    nodes_[kUsedHead].prev = kUsedHead;
    nodes_[kFreeHead].next = kFreeHead;
    nodes_[kFreeHead].prev = kFreeHead;
    active_count_ = 0;
  }
  uint32 allocate() {
    uint32 index = static_cast<uint32>(-1);
    // 从自由列表中取出一个结点。
    if (nodes_[kFreeHead].next != nodes_[kFreeHead].prev) {
      index = nodes_[kFreeHead].next;
      _remove_list(index); 
    } else {
      // 否则压入新结点。
      index = nodes_.size();
      nodes_.push_back(node());
    }
    _add_list(index, kUsedHead);
    active_count_++;
    //
    node& n = nodes_[index];
    n.data.~T();
    ::new (&n.data) T;
    n.uid = _allocate_id();             // 每分配个新node就分配个新node_id.
    //
    return index;
  }
  void free(size_t index) {
    assert(index < nodes_.size());
    node& n = nodes_[index];
    //
    assert(n.uid != kInvalidId);
    n.data.~T();
    ::new (&n.data) T;
    n.uid = kInvalidId;
    //
    _remove_list(index);
    _add_list(index, kFreeHead);
    active_count_--;
  }
  T* at(uint32 index) {
    assert(index < nodes_.size());
    node& n = nodes_[index];
    if (n.uid != kInvalidId) {
      return &n.data;
    }
    return nullptr;
  }
  const T* at(uint32 index) const {
    assert(index < nodes_.size());
    node& n = nodes_[index];
    if (n.uid != kInvalidId) {
      return &n.data;
    }
    return nullptr;
  }
  T& noncheck(uint32 index) {
    assert(index < nodes_.size());
    node& n = nodes_[index];
    return n.data;
  }
  bool check(uint32 index) const {
    if (index < nodes_.size()) {
      return nodes_[index].uid != kInvalidId;
    }
    return false;
  }
  uint32 size() const {
    return nodes_.size();
  }
  uint32 active_count() const {
    return active_count_;
  }
  void reserve(size_t new_size) {
    size_t current_size = nodes_.size();
    if (current_size >= new_size) return;

    nodes_.resize(new_size);
    for (; current_size < new_size; current_size++) {
      nodes_[current_size].uid = kInvalidId;
      _add_list(current_size, kFreeHead);
    }
  }
private:
  static const uint32 kInvalidId      = 0;
  static const uint32 kUsedHead       = 0;
  static const uint32 kFreeHead       = 1;
  static const uint32 kReserved       = 2;
  struct node {
    node() : next(static_cast<uint32>(-1)), prev(static_cast<uint32>(-1)), uid(kInvalidId)
    {
    }
    node(node&& src) 
    {
      next = std::move(src.next);
      prev = std::move(src.prev);
      uid = std::move(src.uid);
      data = std::move(src.data);
    }
    node& operator=(node&& src) {
      next = std::move(src.next);
      prev = std::move(src.prev);
      uid = std::move(src.uid);
      data = std::move(src.data);
    }
    T data;
    uint32 next;
    uint32 prev;
    uint32 uid;                   //node对象ID标识符,用于决定node的合法性。
  private:
    node(const node&);
    node& operator=(const node&);
  };
private:
  void _add_list(size_t index, size_t before_index) {
    assert(index < nodes_.size() && index >= kReserved);
    node& before_node = nodes_[before_index];
    //
    nodes_[index].next = before_index;
    nodes_[index].prev = before_node.prev;
    //
    nodes_[before_node.prev].next = index;
    before_node.prev = index;
  }
  void _remove_list(size_t index) {
    assert(index < nodes_.size() && index >= kReserved);
    node& n = nodes_[index];
    nodes_[n.prev].next = n.next;
    nodes_[n.next].prev = n.prev;
  }
  node* _node(size_t index) {
    return index < nodes_.size() ? &nodes_[index] : nullptr;
  }
  uint32 _allocate_id() {
    uint32 new_id = next_node_id_;
    next_node_id_++;
    if (next_node_id_ == kInvalidId) 
      next_node_id_++;
    //
    return new_id;
  }
private:
  VectorPoolT(const VectorPoolT&);
  VectorPoolT& operator=(const VectorPoolT&);
  typedef std::vector<node> nodes_t;
  nodes_t nodes_;
  uint32  active_count_;
  uint32 next_node_id_;
};


// -------------------------------------------------------------------------
#endif /* VECTOR_POOL_H_ */
