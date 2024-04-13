#pragma once
#include <vector>
#include <string>
#include <array>
#include <tuple>
#include <utility>
#include <cstddef>
#include <cassert>
#include "src/Internal/detection_idiom.hpp"
template <class M, bool reversible= false> class SplayTree {
 _DETECT_BOOL(semigroup, typename T::T, decltype(&T::op));
 _DETECT_BOOL(dual, typename T::T, typename T::E, decltype(&T::mp), decltype(&T::cp));
 _DETECT_BOOL(commute, typename T::commute);
 _DETECT_TYPE(nullptr_or_E, typename T::E, std::nullptr_t, typename T::E);
 _DETECT_TYPE(myself_or_T, typename T::T, T, typename T::T);
 using T= typename myself_or_T<M>::type;
 using E= typename nullptr_or_E<M>::type;
 template <class D> struct NodeB {
  T val;
  D *ch[2], *par;
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
 template <class S> static inline np build(size_t bg, size_t ed, np par, const S &val) {
  if (bg == ed) return nullptr;
  size_t mid= bg + (ed - bg) / 2;
  np t;
  if constexpr (std::is_same_v<S, T>) t= new Node{val};
  else t= new Node{val[mid]};
  return t->par= par, t->ch[0]= build(bg, mid, t, val), t->ch[1]= build(mid + 1, ed, t, val), update(t), t;
 }
 static inline void dump(typename std::vector<T>::iterator itr, np t) {
  if (!t) return;
  if constexpr (dual_v<M>) push_prop(t);
  if constexpr (reversible) push_tog(t);
  size_t sz= t->ch[0] ? t->ch[0]->size() : 0;
  *(itr + sz)= t->val, dump(itr, t->ch[0]), dump(itr + sz + 1, t->ch[1]);
 }
 template <bool sz= true> static inline void update(np t) {
  if constexpr (sz) t->sz= 1;
  if constexpr (semigroup_v<M>) {
   t->sum= t->val;
   if constexpr (reversible && !commute_v<M>) t->rsum= t->sum;
  }
  if (t->ch[0]) {
   if constexpr (sz) t->sz+= t->ch[0]->size();
   if constexpr (semigroup_v<M>) {
    t->sum= M::op(t->ch[0]->sum, t->sum);
    if constexpr (reversible && !commute_v<M>) t->rsum= M::op(t->rsum, t->ch[0]->rsum);
   }
  }
  if (t->ch[1]) {
   if constexpr (sz) t->sz+= t->ch[1]->size();
   if constexpr (semigroup_v<M>) {
    t->sum= M::op(t->sum, t->ch[1]->sum);
    if constexpr (reversible && !commute_v<M>) t->rsum= M::op(t->ch[1]->rsum, t->rsum);
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
  std::swap(t->ch[0], t->ch[1]), t->sz^= 0x40000000;
 }
 static inline void push_prop(np t) {
  if (t->sz >> 31) propagate(t->ch[0], t->laz), propagate(t->ch[1], t->laz), t->sz^= 0x80000000;
 }
 static inline void push_tog(np t) {
  if (t->sz & 0x40000000) toggle(t->ch[0]), toggle(t->ch[1]), t->sz^= 0x40000000;
 }
 static inline void rot(np t) {
  np p= t->par;
  if (bool d= p->ch[1] == t; (p->ch[d]= std::exchange(t->ch[!d], p))) p->ch[d]->par= p;
  if ((t->par= std::exchange(p->par, t))) t->par->ch[t->par->ch[1] == p]= t;
  update(p);
 }
 static inline void splay(np &t, size_t k) {
  for (assert(t), assert(k < t->size());;) {
   size_t sz= t->ch[0] ? t->ch[0]->size() : 0;
   if constexpr (dual_v<M>) push_prop(t);
   if constexpr (reversible) push_tog(t);
   if (sz == k) break;
   if (sz < k) k-= sz + 1, t= t->ch[1];
   else t= t->ch[0];
  }
  for (np p; (p= t->par); rot(t))
   if (p->par) rot(p->par->ch[p->ch[1] == t] == p ? p : t);
  update(t);
 }
 inline np between(size_t a, size_t b) { return a ? b == rt->size() ? (splay(rt, a - 1), rt->ch[1]) : (splay(rt, b), rt->ch[0]->par= nullptr, splay(rt->ch[0], a - 1), rt->ch[0]->par= rt, rt->ch[0]->ch[1]) : b == rt->size() ? rt : (splay(rt, b), rt->ch[0]); }
 static inline SplayTree np_to(np t) {
  SplayTree ret;
  return ret.rt= t, ret;
 }
public:
 SplayTree(): rt(nullptr) {}
 SplayTree(size_t n, const T &val= T()): rt(n ? build(0, n, nullptr, val) : nullptr) {}
 SplayTree(const T *bg, const T *ed): rt(bg == ed ? nullptr : build(0, ed - bg, nullptr, bg)) {}
 SplayTree(const std::vector<T> &v): SplayTree(v.data(), v.data() + v.size()) {}
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
  else ret+= "\"prod\" ";
  if constexpr (!semigroup_v<M> || !commute_v<M>) ret+= "\"mul\" ";
  if constexpr (!dual_v<M>) ret+= "\"apply\" ";
  if constexpr (!reversible) ret+= "\"reverse\" ";
  return ret;
 }
 template <class L= M> const std::enable_if_t<semigroup_v<L>, T> &operator[](size_t k) { return get(k); }
 template <class L= M> std::enable_if_t<!semigroup_v<L>, T> &operator[](size_t k) { return at(k); }
 const T &get(size_t k) { return splay(rt, k), rt->val; }
 T &at(size_t k) {
  static_assert(!semigroup_v<M>, "\"at\" is not available");
  return splay(rt, k), rt->val;
 }
 void set(size_t k, const T &val) {
  splay(rt, k), rt->val= val;
  if constexpr (semigroup_v<M>) update<0>(rt);
 }
 void mul(size_t k, const T &val) {
  static_assert(semigroup_v<M> && commute_v<M>, "\"mul\" is not available");
  splay(rt, k), rt->val= M::op(rt->val, val), update<0>(rt);
 }
 const T &prod(size_t a, size_t b) {
  static_assert(semigroup_v<M>, "\"prod\" is not available");
  return between(a, b)->sum;
 }
 void apply(size_t a, size_t b, const E &x) {
  static_assert(dual_v<M>, "\"apply\" is not available");
  np t= between(a, b);
  propagate(t, x);
  if constexpr (semigroup_v<M>)
   if (np p= t->par; p)
    if (update<0>(p); p->par) update<0>(p->par);
 }
 void reverse() {
  static_assert(reversible, "\"reverse\" is not available");
  if (rt) toggle(rt);
 }
 void reverse(size_t a, size_t b) {
  static_assert(reversible, "\"reverse\" is not available");
  toggle(between(a, b));
 }
 std::pair<SplayTree, SplayTree> split(size_t k) {
  if (!k) return {SplayTree(), *this};
  if (size() == k) return {*this, SplayTree()};
  splay(rt, k);
  np l= rt->ch[0];
  rt->ch[0]= l->par= nullptr, update(rt);
  return {np_to(l), np_to(rt)};
 }
 std::tuple<SplayTree, SplayTree, SplayTree> split3(size_t a, size_t b) {
  auto [tmp, right]= split(b);
  auto [left, center]= tmp.split(a);
  return {left, center, right};
 }
 SplayTree &operator+=(SplayTree rhs) {
  if (!rt) rt= rhs.rt;
  else if (rhs.rt) splay(rhs.rt, 0), rhs.rt->ch[0]= rt, rt->par= rhs.rt, rt= rhs.rt, update(rt);
  return *this;
 }
 SplayTree operator+(SplayTree rhs) { return SplayTree(*this)+= rhs; }
 void push_back(const T &val) {
  rt= new Node{val, {rt, nullptr}, nullptr};
  if (rt->ch[0]) rt->ch[0]->par= rt;
  update(rt);
 }
 void push_front(const T &val) {
  rt= new Node{val, {nullptr, rt}, nullptr};
  if (rt->ch[1]) rt->ch[1]->par= rt;
  update(rt);
 }
 void insert(size_t k, const T &val) {
  assert(k <= size());
  if (!k) return push_front(val);
  if (k == rt->size()) return push_back(val);
  splay(rt, k);
  np l= std::exchange(rt->ch[0], nullptr);
  update(rt), rt= new Node{val, {l, rt}, nullptr}, l->par= rt->ch[1]->par= rt, update(rt);
 }
 T pop_back() {
  splay(rt, rt->size() - 1);
  T v= std::exchange(rt, rt->ch[0])->val;
  if (rt) rt->par= nullptr;
  return v;
 }
 T pop_front() {
  splay(rt, 0);
  T v= std::exchange(rt, rt->ch[1])->val;
  if (rt) rt->par= nullptr;
  return v;
 }
 T erase(size_t k) {
  if (!k) return pop_front();
  if (k == rt->size() - 1) return pop_back();
  splay(rt, k);
  np l= rt->ch[0];
  T v= std::exchange(rt, rt->ch[1])->val;
  return rt->par= nullptr, splay(rt, 0), l->par= rt, rt->ch[0]= l, update(rt), v;
 }
};