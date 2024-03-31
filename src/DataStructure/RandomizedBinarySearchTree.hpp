#pragma once
#include <vector>
#include <string>
#include <array>
#include <utility>
#include <cstddef>
#include <cassert>
#include "src/Internal/detection_idiom.hpp"
#include "src/Misc/rng.hpp"
template <class M, bool reversible= false> class RandomizedBinarySearchTree {
 _DETECT_BOOL(semigroup, typename T::T, decltype(&T::op));
 _DETECT_BOOL(dual, typename T::T, typename T::E, decltype(&T::mp), decltype(&T::cp));
 _DETECT_BOOL(commute, typename T::commute);
 _DETECT_TYPE(nullptr_or_E, typename T::E, std::nullptr_t, typename T::E);
 _DETECT_TYPE(myself_or_T, typename T::T, T, typename T::T);
 using T= typename myself_or_T<M>::type;
 using E= typename nullptr_or_E<M>::type;
 using RBST= RandomizedBinarySearchTree;
 template <class D> struct NodeB {
  T val;
  D *l, *r;
  size_t sz;
 };
 template <class D, bool du> struct NodeD: NodeB<D> {};
 template <class D> struct NodeD<D, 1>: NodeB<D> {
  E laz;
 };
 template <class D, bool sg, bool rev, bool com> struct NodeS: NodeD<D, dual_v<M>> {};
 template <class D, bool rev, bool com> struct NodeS<D, 1, rev, com>: NodeD<D, dual_v<M>> {
  T sum;
 };
 template <class D> struct NodeS<D, 1, 1, 0>: NodeD<D, dual_v<M>> {
  T sum, rsum;
 };
 struct Node: NodeS<Node, semigroup_v<M>, reversible, commute_v<M>> {
  size_t size() const {
   if constexpr (dual_v<M> || reversible) return this->sz & 0x3fffffff;
   else return this->sz;
  }
 };
 using np= Node *;
 np rt;
 template <bool sz= 1> static inline void update(np t) {
  if constexpr (sz) t->sz= 1;
  if constexpr (semigroup_v<M>) {
   t->sum= t->val;
   if constexpr (reversible && !commute_v<M>) t->rsum= t->sum;
  }
  if (t->l) {
   if constexpr (sz) t->sz+= t->l->size();
   if constexpr (semigroup_v<M>) {
    t->sum= M::op(t->l->sum, t->sum);
    if constexpr (reversible && !commute_v<M>) t->rsum= M::op(t->rsum, t->l->rsum);
   }
  }
  if (t->r) {
   if constexpr (sz) t->sz+= t->r->size();
   if constexpr (semigroup_v<M>) {
    t->sum= M::op(t->sum, t->r->sum);
    if constexpr (reversible && !commute_v<M>) t->rsum= M::op(t->r->rsum, t->rsum);
   }
  }
 }
 static inline void propagate(np t, const E &x) {
  if (!t) return;
  if (t->sz >> 31) M::cp(t->laz, x);
  else t->laz= x;
  if constexpr (semigroup_v<M>) {
   M::mp(t->sum, x, t->size());
   if constexpr (reversible && !commute_v<M>) M::mp(t->rsum, x, t->size());
  }
  M::mp(t->val, x, 1), t->sz|= 0x80000000;
 }
 static inline void toggle(np t) {
  if (!t) return;
  if constexpr (semigroup_v<M> && !commute_v<M>) std::swap(t->sum, t->rsum);
  std::swap(t->l, t->r), t->sz^= 0x40000000;
 }
 static inline void push_prop(np t) {
  if (t->sz >> 31) propagate(t->l, t->laz), propagate(t->r, t->laz), t->sz^= 0x80000000;
 }
 static inline void push_tog(np t) {
  if (t->sz & 0x40000000) toggle(t->l), toggle(t->r), t->sz^= 0x40000000;
 }
 template <class S> static inline np build(size_t bg, size_t ed, const S &val) {
  if (bg == ed) return nullptr;
  size_t mid= bg + (ed - bg) / 2;
  np t;
  if constexpr (std::is_same_v<S, T>) t= new Node{val};
  else t= new Node{val[mid]};
  return t->l= build(bg, mid, val), t->r= build(mid + 1, ed, val), update(t), t;
 }
 static inline void dump(typename std::vector<T>::iterator itr, np t) {
  if (!t) return;
  if constexpr (dual_v<M>) push_prop(t);
  if constexpr (reversible) push_tog(t);
  size_t sz= t->l ? t->l->size() : 0;
  *(itr + sz)= t->val, dump(itr, t->l), dump(itr + sz + 1, t->r);
 }
 static inline np merge(np l, np r) {
  if (!l) return r;
  if (!r) return l;
  if (size_t lsz= l->size(), rsz= r->size(); rng(lsz + rsz) < lsz) {
   if constexpr (dual_v<M>) push_prop(l);
   if constexpr (reversible) push_tog(l);
   return l->r= merge(l->r, r), update(l), l;
  } else {
   if constexpr (dual_v<M>) push_prop(r);
   if constexpr (reversible) push_tog(r);
   return r->l= merge(l, r->l), update(r), r;
  }
 }
 static inline std::pair<np, np> split(np t, size_t k) {
  if (!t) return {nullptr, nullptr};
  if constexpr (dual_v<M>) push_prop(t);
  if constexpr (reversible) push_tog(t);
  if (size_t sz= t->l ? t->l->size() : 0; k <= sz) {
   auto [l, r]= split(t->l, k);
   t->l= r, update(t);
   return {l, t};
  } else {
   auto [l, r]= split(t->r, k - sz - 1);
   t->r= l, update(t);
   return {t, r};
  }
 }
 static inline T erase(np &t, size_t k) {
  if constexpr (dual_v<M>) push_prop(t);
  if constexpr (reversible) push_tog(t);
  size_t sz= t->l ? t->l->size() : 0;
  if (k == sz) {
   T v= t->val;
   return t= merge(t->l, t->r), v;
  } else {
   T v= k < sz ? erase(t->l, k) : erase(t->r, k - sz - 1);
   return update(t), v;
  }
 }
 static inline T fold(np t, size_t a, size_t b) {
  if (!a && b == t->size()) return t->sum;
  if constexpr (dual_v<M>) push_prop(t);
  if constexpr (reversible) push_tog(t);
  size_t l= t->l ? t->l->size() : 0, k= l + 1;
  if (b < k) return fold(t->l, a, b);
  if (a > l) return fold(t->r, a - k, b - k);
  T ret= t->val;
  if (a < l) ret= M::op(fold(t->l, a, l), ret);
  if (b > k) ret= M::op(ret, fold(t->r, 0, b - k));
  return ret;
 }
 static inline void apply(np t, size_t a, size_t b, const E &x) {
  if (!a && b == t->size()) return propagate(t, x);
  if constexpr (reversible) push_tog(t);
  push_prop(t);
  size_t l= t->l ? t->l->size() : 0, k= l + 1;
  if (b < k) apply(t->l, a, b, x);
  else if (a > l) apply(t->r, a - k, b - k, x);
  else {
   M::mp(t->val, x, 1);
   if (a < l) apply(t->l, a, l, x);
   if (b > k) apply(t->r, 0, b - k, x);
  }
  if constexpr (semigroup_v<M>) update<0>(t);
 }
 static inline void set_val(np t, size_t k, const T &v) {
  if constexpr (dual_v<M>) push_prop(t);
  if constexpr (reversible) push_tog(t);
  size_t l= t->l ? t->l->size() : 0;
  if (k < l) set_val(t->l, k, v);
  else if (k > l) set_val(t->r, k - l - 1, v);
  else t->val= v;
  if constexpr (semigroup_v<M>) update<0>(t);
 }
 static inline void mul_val(np t, size_t k, const T &v) {
  if constexpr (dual_v<M>) push_prop(t);
  if constexpr (reversible) push_tog(t);
  size_t l= t->l ? t->l->size() : 0;
  if (k < l) mul_val(t->l, k, v);
  else if (k > l) mul_val(t->r, k - l - 1, v);
  else t->val= M::op(t->val, v);
  update<0>(t);
 }
 static inline T get_val(np t, size_t k) {
  if constexpr (dual_v<M>) push_prop(t);
  if constexpr (reversible) push_tog(t);
  size_t l= t->l ? t->l->size() : 0;
  if (k < l) return get_val(t->l, k);
  if (k > l) return get_val(t->r, k - l - 1);
  return t->val;
 }
 static inline T &at_val(np t, size_t k) {
  if constexpr (dual_v<M>) push_prop(t);
  if constexpr (reversible) push_tog(t);
  size_t l= t->l ? t->l->size() : 0;
  if (k < l) return at_val(t->l, k);
  if (k > l) return at_val(t->r, k - l - 1);
  return t->val;
 }
 static inline RBST np_to_rbst(np t) {
  RBST ret;
  return ret.rt= t, ret;
 }
public:
 RandomizedBinarySearchTree(): rt(nullptr) {}
 RandomizedBinarySearchTree(size_t n, T val= T()): rt(n ? build(0, n, val) : nullptr) {}
 RandomizedBinarySearchTree(const T *bg, const T *ed): rt(bg == ed ? nullptr : build(0, ed - bg, bg)) {}
 RandomizedBinarySearchTree(const std::vector<T> &ar): RandomizedBinarySearchTree(ar.data(), ar.data() + ar.size()) {}
 size_t size() const { return rt ? rt->size() : 0; }
 void clear() { rt= nullptr; }
 std::vector<T> dump() {
  if (!rt) return std::vector<T>();
  std::vector<T> ret(size());
  return dump(ret.begin(), rt), ret;
 }
 static std::string which_unavailable() {
  std::string ret= "";
  if constexpr (semigroup_v<M>) ret+= "\"at\" ";
  else ret+= "\"fold\" ";
  if constexpr (!semigroup_v<M> || !commute_v<M>) ret+= "\"mul\" ";
  if constexpr (!dual_v<M>) ret+= "\"apply\" ";
  if constexpr (!reversible) ret+= "\"reverse\" ";
  return ret;
 }
 RBST &operator+=(RBST r) { return rt= merge(rt, r.rt), *this; }
 RBST operator+(RBST r) const { return RBST(*this)+= r; }
 std::pair<RBST, RBST> split(size_t k) {
  assert(k <= size());
  auto [l, r]= split(rt, k);
  return {np_to_rbst(l), np_to_rbst(r)};
 }
 std::tuple<SplayTree, SplayTree, SplayTree> split3(size_t a, size_t b) {
  assert(a < b), assert(b <= size());
  auto [tmp, r]= split(rt, b);
  auto [l, c]= split(tmp, a);
  return {np_to_rbst(l), np_to_rbst(c), np_to_rbst(r)};
 }
 void push_back(const T &v) {
  np t= new Node{v};
  update(t), rt= merge(rt, t);
 }
 void push_front(const T &v) {
  np t= new Node{v};
  update(t), rt= merge(t, rt);
 }
 void insert(size_t k, const T &v) {
  auto [l, r]= split(rt, k);
  rt= new Node{v}, update(rt), rt= merge(merge(l, rt), r);
 }
 T pop_back() {
  auto [l, t]= split(rt, size() - 1);
  return rt= l, t->val;
 }
 T pop_front() {
  auto [t, r]= split(rt, 1);
  return rt= r, t->val;
 }
 T erase(size_t k) { return assert(k < size()), erase(rt, k); }
 void set(size_t k, const T &val) { set_val(rt, k, val); }
 void mul(size_t k, const T &val) {
  static_assert(semigroup_v<M> && commute_v<M>, "\"mul\" is not available\n");
  mul_val(rt, k, val);
 }
 T get(size_t k) { return get_val(rt, k); }
 T &at(size_t k) {
  static_assert(!semigroup_v<M>, "\"at\" is not available\n");
  return at_val(rt, k);
 }
 template <class L= M> std::enable_if_t<semigroup_v<L>, T> operator[](size_t k) { return get(k); }
 template <class L= M> std::enable_if_t<!semigroup_v<L>, T> &operator[](size_t k) { return at(k); }
 T fold(size_t a, size_t b) {
  static_assert(semigroup_v<M>, "\"fold\" is not available\n");
  return fold(rt, a, b);
 }
 void apply(size_t a, size_t b, E x) {
  static_assert(dual_v<M>, "\"apply\" is not available\n");
  apply(rt, a, b, x);
 }
 void reverse() {
  static_assert(reversible, "\"reverse\" is not available\n");
  toggle(rt);
 }
 void reverse(size_t a, size_t b) {
  static_assert(reversible, "\"reverse\" is not available\n");
  assert(a < b), assert(b <= size());
  auto [tmp, r]= split(rt, b);
  auto [l, c]= split(tmp, a);
  toggle(c), rt= merge(merge(l, c), r);
 }
};