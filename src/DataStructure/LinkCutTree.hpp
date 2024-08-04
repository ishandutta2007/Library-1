#pragma once
#include <algorithm>
#include <vector>
#include <string>
#include <cstddef>
#include <cassert>
#include "src/Internal/detection_idiom.hpp"
template <class M= void> class LinkCutTree {
 _DETECT_BOOL(semigroup, typename T::T, decltype(&T::op));
 _DETECT_BOOL(dual, typename T::T, typename T::E, decltype(&T::mp), decltype(&T::cp));
 _DETECT_BOOL(commute, typename T::commute);
 _DETECT_TYPE(myself_or_T, typename T::T, T, typename T::T);
 _DETECT_TYPE(nullptr_or_E, typename T::E, std::nullptr_t, typename T::E);
 using T= std::conditional_t<std::is_void_v<M>, std::nullptr_t, typename myself_or_T<M>::type>;
 using E= typename nullptr_or_E<M>::type;
 struct NodeB {
  int ch[2]= {-1, -1}, par= -1;
  bool revf= 0;
 };
 template <class D, class A> struct NodeV: NodeB {
  T val;
 };
 template <class D> struct NodeV<D, void>: NodeB {};
 template <class D, bool du> struct NodeD: NodeV<D, M> {};
 template <class D> struct NodeD<D, 1>: NodeV<D, M> {
  E laz;
  bool lazf= 0;
 };
 template <class D, bool sg, bool com> struct NodeS: NodeD<D, dual_v<M>> {};
 template <class D> struct NodeS<D, 1, 1>: NodeD<D, dual_v<M>> {
  T sum;
 };
 template <class D> struct NodeS<D, 1, 0>: NodeD<D, dual_v<M>> {
  T sum, rsum;
 };
 using Node= NodeS<void, semigroup_v<M>, commute_v<M>>;
 std::vector<Node> n;
 inline void update(int i) {
  n[i].sum= n[i].val;
  if constexpr (!commute_v<M>) n[i].rsum= n[i].val;
  if (int l= n[i].ch[0]; l != -1) {
   n[i].sum= M::op(n[l].sum, n[i].sum);
   if constexpr (!commute_v<M>) n[i].rsum= M::op(n[i].rsum, n[l].rsum);
  }
  if (int r= n[i].ch[1]; r != -1) {
   n[i].sum= M::op(n[i].sum, n[r].sum);
   if constexpr (!commute_v<M>) n[i].rsum= M::op(n[r].rsum, n[i].rsum);
  }
 }
 inline void propagate(int i, const E &x) {
  if (i == -1) return;
  if (n[i].lazf) M::cp(n[i].laz, x);
  else n[i].laz= x;
  if constexpr (semigroup_v<M>) {
   M::mp(n[i].sum, x);
   if constexpr (!commute_v<M>) M::mp(n[i].rsum, x);
  }
  M::mp(n[i].val, x), n[i].lazf= 1;
 }
 inline void toggle(int i) {
  if (i == -1) return;
  std::swap(n[i].ch[0], n[i].ch[1]);
  if constexpr (semigroup_v<M> && !commute_v<M>) std::swap(n[i].sum, n[i].rsum);
  n[i].revf^= 1;
 }
 inline void push(int i) {
  if (n[i].revf) toggle(n[i].ch[0]), toggle(n[i].ch[1]), n[i].revf= 0;
  if constexpr (dual_v<M>)
   if (n[i].lazf) propagate(n[i].ch[0], n[i].laz), propagate(n[i].ch[1], n[i].laz), n[i].lazf= 0;
 }
 inline int dir(int i) {
  if (int p= n[i].par; p != -1) {
   if (n[p].ch[0] == i) return 0;
   if (n[p].ch[1] == i) return 1;
  }
  return 2;
 }
 inline void rot(int i) {
  int p= n[i].par, d= n[p].ch[1] == i;
  if (int c= n[p].ch[d]= std::exchange(n[i].ch[!d], p); c != -1) n[c].par= p;
  if (d= dir(p); d < 2) n[n[p].par].ch[d]= i;
  n[i].par= std::exchange(n[p].par, i);
  if constexpr (semigroup_v<M>) update(p);
 }
 inline void splay(int i) {
  push(i);
  for (int d; d= dir(i), d < 2; rot(i))
   if (int p= n[i].par, c= dir(p), pp= n[p].par; c < 2) push(pp), push(p), push(i), rot(d == c ? p : i);
   else push(p), push(i);
  if constexpr (semigroup_v<M>) update(i);
 }
 inline int expose(int i) {
  int r= -1;
  for (int p= i; p != -1; r= p, p= n[p].par) {
   splay(p), n[p].ch[1]= r;
   if constexpr (semigroup_v<M>) update(p);
  }
  return splay(i), r;
 }
public:
 LinkCutTree(size_t sz): n(sz) {}
 LinkCutTree(size_t sz, T val): n(sz) {
  for (int i= sz; i--;) n[i].val= val;
 }
 void evert(int k) { expose(k), toggle(k), push(k); }
 void link(int c, int p) {
  evert(c), expose(p), assert(n[c].par == -1), n[p].ch[1]= c, n[c].par= p;
  if constexpr (semigroup_v<M>) update(p);
 }
 void cut(int c, int p) {
  evert(p), expose(c), assert(n[c].ch[0] == p), n[c].ch[0]= n[p].par= -1;
  if constexpr (semigroup_v<M>) update(c);
 }
 int root(int x) {
  for (expose(x);; x= n[x].ch[0])
   if (push(x), n[x].ch[0] == -1) return splay(x), x;
 }
 int parent(int x) {
  if (expose(x), x= n[x].ch[0]; x == -1) return -1;
  for (;; x= n[x].ch[1])
   if (push(x), n[x].ch[1] == -1) return splay(x), x;
 }
 int lca(int x, int y) { return x == y ? x : (expose(x), y= expose(y), n[x].par == -1) ? -1 : y; }
 const T &get(int k) {
  static_assert(!std::is_void_v<M>, "\"get\" is not available\n");
  return expose(k), n[k].val;
 }
 T &at(int k) {
  static_assert(!std::is_void_v<M> && !semigroup_v<M>, "\"at\" is not available\n");
  return expose(k), n[k].val;
 }
 template <class L= M> const std::enable_if_t<semigroup_v<L>, T> &operator[](size_t k) { return get(k); }
 template <class L= M> std::enable_if_t<!semigroup_v<L>, T> &operator[](size_t k) { return at(k); }
 void set(int k, const T &v) {
  static_assert(!std::is_void_v<M>, "\"set\" is not available\n");
  expose(k), n[k].val= v;
  if constexpr (semigroup_v<M>) update(k);
 }
 void mul(int k, const T &v) {
  static_assert(semigroup_v<M> && commute_v<M>, "\"mul\" is not available\n");
  expose(k), n[k].val= M::op(n[k].val, v), update(k);
 }
 // [a,b] closed section
 T prod(int a, int b) {
  static_assert(semigroup_v<M>, "\"prod\" is not available\n");
  return a == b ? get(a) : (evert(a), expose(b), assert(n[a].par != -1), n[b].sum);
 }
 // [a,b] closed section
 void apply(int a, int b, const E &v) {
  static_assert(dual_v<M>, "\"apply\" is not available\n");
  evert(a), expose(b), assert(a == b || n[a].par != -1), propagate(b, v), push(b);
 }
 static std::string which_unavailable() {
  std::string ret= "";
  if constexpr (semigroup_v<M>) ret+= "\"at\" ";
  else ret+= "\"prod\" ";
  if constexpr (!semigroup_v<M> || !commute_v<M>) ret+= "\"mul\" ";
  if constexpr (!dual_v<M>) ret+= "\"apply\" ";
  if constexpr (std::is_void_v<M>) ret+= "\"get\" \"set\" ";
  return ret;
 }
};