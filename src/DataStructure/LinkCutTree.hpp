#pragma once
#include <algorithm>
#include <vector>
#include <string>
#include <cstddef>
#include <cassert>
#include "src/Internal/HAS_CHECK.hpp"
template <typename M= void> class LinkCutTree {
 HAS_MEMBER(op);
 HAS_MEMBER(mapping);
 HAS_MEMBER(composition);
 HAS_TYPE(T);
 HAS_TYPE(E);
 template <class L> static constexpr bool semigroup_v= std::conjunction_v<has_T<L>, has_op<L>>;
 template <class L> static constexpr bool dual_v= std::conjunction_v<has_T<L>, has_E<L>, has_mapping<L>, has_composition<L>>;
 using node_id= std::int_least32_t;
 template <class U= std::nullptr_t, class F= std::nullptr_t> struct Node_B {
  using T= U;
  using E= F;
  node_id ch[2]= {-1, -1}, par= -1;
  bool rev_flg;
 };
 template <class D, bool sg, bool du> struct Node_D: Node_B<> {};
 template <class D> struct Node_D<D, 1, 0>: Node_B<typename M::T> {
  typename M::T val, sum, rsum;
 };
 template <class D> struct Node_D<D, 0, 1>: Node_B<typename M::T, typename M::E> {
  typename M::T val;
  typename M::E lazy;
  bool lazy_flg;
 };
 template <class D> struct Node_D<D, 1, 1>: Node_B<typename M::T, typename M::E> {
  typename M::T val, sum, rsum;
  typename M::E lazy;
  bool lazy_flg;
 };
 using Node= Node_D<void, semigroup_v<M>, dual_v<M>>;
 using T= typename Node::T;
 using E= typename Node::E;
 inline int dir(node_id i) {
  if (ns[i].par != -1) {
   if (ns[ns[i].par].ch[0] == i) return 0;
   if (ns[ns[i].par].ch[1] == i) return 1;
  }
  return 2;
 }
 inline void rot(node_id i) {
  node_id p= ns[i].par;
  int d= dir(i);
  if ((ns[p].ch[d]= ns[i].ch[!d]) != -1) ns[ns[p].ch[d]].par= p;
  ns[i].ch[!d]= p, ns[i].par= ns[p].par;
  if ((d= dir(p)) < 2) ns[ns[p].par].ch[d]= i;
  ns[p].par= i;
  if constexpr (semigroup_v<M>) pushup(p);
 }
 inline void splay(node_id i) {
  eval(i);
  node_id p= ns[i].par, pp;
  for (int d= dir(i), c; d < 2; rot(i), d= dir(i), p= ns[i].par) {
   if (c= dir(p), pp= ns[p].par; c < 2) eval(pp), eval(p), eval(i), rot(d == c ? p : i);
   else eval(p), eval(i);
  }
  if constexpr (semigroup_v<M>) pushup(i);
 }
 inline void pushup(node_id i) {
  ns[i].rsum= ns[i].sum= ns[i].val;
  if (ns[i].ch[0] != -1) ns[i].sum= M::op(ns[ns[i].ch[0]].sum, ns[i].sum), ns[i].rsum= M::op(ns[i].rsum, ns[ns[i].ch[0]].rsum);
  if (ns[i].ch[1] != -1) ns[i].sum= M::op(ns[i].sum, ns[ns[i].ch[1]].sum), ns[i].rsum= M::op(ns[ns[i].ch[1]].rsum, ns[i].rsum);
 }
 inline void propagate(node_id i, const E &x) {
  if (i == -1) return;
  M::mapping(ns[i].val, x), ns[i].lazy_flg ? (M::composition(ns[i].lazy, x), x) : ns[i].lazy= x;
  if constexpr (semigroup_v<M>) M::mapping(ns[i].sum, x), M::mapping(ns[i].rsum, x);
  ns[i].lazy_flg= true;
 }
 inline void toggle(node_id i) {
  if (i == -1) return;
  std::swap(ns[i].ch[0], ns[i].ch[1]);
  if constexpr (semigroup_v<M>) std::swap(ns[i].sum, ns[i].rsum);
  ns[i].rev_flg= !ns[i].rev_flg;
 }
 inline void eval(node_id i) {
  if (ns[i].rev_flg) toggle(ns[i].ch[0]), toggle(ns[i].ch[1]), ns[i].rev_flg= false;
  if constexpr (dual_v<M>)
   if (ns[i].lazy_flg) propagate(ns[i].ch[0], ns[i].lazy), propagate(ns[i].ch[1], ns[i].lazy), ns[i].lazy_flg= false;
 }
 inline node_id expose(node_id i) {
  node_id r= -1;
  for (node_id p= i; p != -1; r= p, p= ns[p].par) {
   splay(p), ns[p].ch[1]= r;
   if constexpr (semigroup_v<M>) pushup(p);
  }
  return splay(i), r;
 }
 std::vector<Node> ns;
public:
 LinkCutTree(std::size_t n): ns(n) {}
 LinkCutTree(std::size_t n, T val): ns(n) {
  for (int i= n; i--;) ns[i].val= val;
 }
 void evert(int k) { expose(k), toggle(k), eval(k); }
 void link(int c, int p) {
  evert(c), expose(p), assert(ns[c].par == -1), ns[p].ch[1]= c, ns[c].par= p;
  if constexpr (semigroup_v<M>) pushup(p);
 }
 void cut(int c, int p) {
  evert(p), expose(c), assert(ns[c].ch[0] == p), ns[c].ch[0]= ns[p].par= -1;
  if constexpr (semigroup_v<M>) pushup(c);
 }
 int root(int x) {
  for (expose(x);; x= ns[x].ch[0])
   if (eval(x); ns[x].ch[0] == -1) return splay(x), x;
 }
 int parent(int x) {
  if (expose(x), x= ns[x].ch[0]; x == -1) return -1;
  for (;; x= ns[x].ch[1])
   if (eval(x); ns[x].ch[1] == -1) return splay(x), x;
 }
 int lca(int x, int y) { return x == y ? x : (expose(x), y= expose(y), ns[x].par == -1) ? -1 : y; }
 const T &operator[](int k) { return get(k); }
 const T &get(int k) {
  static_assert(semigroup_v<M> || dual_v<M>, "\"get\" is not available\n");
  return expose(k), ns[k].val;
 }
 void set(int k, T v) {
  static_assert(semigroup_v<M> || dual_v<M>, "\"set\" is not available\n");
  expose(k), ns[k].val= v;
  if constexpr (semigroup_v<M>) pushup(k);
 }
 T fold(int a, int b) {  // [a,b] closed section
  static_assert(semigroup_v<M>, "\"fold\" is not available\n");
  return a == b ? get(a) : (evert(a), expose(b), assert(ns[a].par != -1), ns[b].sum);
 }
 void apply(int a, int b, E v) {  // [a,b] closed section
  static_assert(dual_v<M>, "\"apply\" is not available\n");
  evert(a), expose(b), assert(a == b || ns[a].par != -1), propagate(b, v), eval(b);
 }
 static std::string which_available() {
  std::string ret= "";
  if constexpr (semigroup_v<M> || dual_v<M>) ret+= "\"set\" \"get\" ";
  if constexpr (semigroup_v<M>) ret+= "\"fold\" ";
  if constexpr (dual_v<M>) ret+= "\"apply\" ";
  return ret;
 }
};