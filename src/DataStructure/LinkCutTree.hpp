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
 template <class L> using semigroup= std::conjunction<has_T<L>, has_op<L>>;
 template <class L> using dual= std::conjunction<has_T<L>, has_E<L>, has_mapping<L>, has_composition<L>>;
 template <class tDerived, class U= std::nullptr_t, class F= std::nullptr_t> struct Node_B {
  using T= U;
  using E= F;
  tDerived *ch[2], *par;
  bool rev_flg;
 };
 template <bool sg_, bool du_, typename tEnable= void> struct Node_D: Node_B<Node_D<sg_, du_, tEnable>> {};
 template <bool sg_, bool du_> struct Node_D<sg_, du_, typename std::enable_if_t<sg_ && !du_>>: Node_B<Node_D<sg_, du_>, typename M::T> { typename M::T val, sum, rsum; };
 template <bool sg_, bool du_> struct Node_D<sg_, du_, typename std::enable_if_t<!sg_ && du_>>: Node_B<Node_D<sg_, du_>, typename M::T, typename M::E> {
  typename M::T val;
  typename M::E lazy;
  bool lazy_flg;
 };
 template <bool sg_, bool du_> struct Node_D<sg_, du_, typename std::enable_if_t<sg_ && du_>>: Node_B<Node_D<sg_, du_>, typename M::T, typename M::E> {
  typename M::T val, sum, rsum;
  typename M::E lazy;
  bool lazy_flg;
 };
 using Node= Node_D<semigroup<M>::value, dual<M>::value>;
 using T= typename Node::T;
 using E= typename Node::E;
 static inline int dir(Node *&t) {
  if (t->par) {
   if (t->par->ch[0] == t) return 0;
   if (t->par->ch[1] == t) return 1;
  }
  return 2;
 }
 static inline void rot(Node *t) {
  Node *p= t->par;
  int d= dir(t);
  if ((p->ch[d]= t->ch[!d])) p->ch[d]->par= p;
  t->ch[!d]= p;
  if constexpr (semigroup<M>::value) pushup(p), pushup(t);
  t->par= p->par;
  if ((d= dir(p)) < 2) {
   p->par->ch[d]= t;
   if constexpr (semigroup<M>::value) pushup(t->par);
  }
  p->par= t;
 }
 static inline void splay(Node *t) {
  eval(t);
  for (int t_d= dir(t), p_d; t_d < 2; rot(t), t_d= dir(t)) {
   if ((p_d= dir(t->par)) < 2) eval(t->par->par);
   eval(t->par), eval(t);
   if (p_d < 2) rot(t_d == p_d ? t->par : t);
  }
 }
 static inline void pushup(Node *t) {
  t->rsum= t->sum= t->val;
  if (t->ch[0]) t->sum= M::op(t->ch[0]->sum, t->sum), t->rsum= M::op(t->rsum, t->ch[0]->rsum);
  if (t->ch[1]) t->sum= M::op(t->sum, t->ch[1]->sum), t->rsum= M::op(t->ch[1]->rsum, t->rsum);
 }
 static inline void propagate(Node *t, const E &x) {
  if (!t) return;
  t->lazy_flg ? (M::composition(t->lazy, x), x) : t->lazy= x;
  M::mapping(t->val, x);
  if constexpr (semigroup<M>::value) M::mapping(t->sum, x), M::mapping(t->rsum, x);
  t->lazy_flg= true;
 }
 static inline void toggle(Node *t) {
  if (!t) return;
  std::swap(t->ch[0], t->ch[1]);
  if constexpr (semigroup<M>::value) std::swap(t->sum, t->rsum);
  t->rev_flg= !t->rev_flg;
 }
 static inline void eval(Node *t) {
  if (t->rev_flg) toggle(t->ch[0]), toggle(t->ch[1]), t->rev_flg= false;
  if constexpr (dual<M>::value)
   if (t->lazy_flg) propagate(t->ch[0], t->lazy), propagate(t->ch[1], t->lazy), t->lazy_flg= false;
 }
 static inline Node *expose(Node *t) {
  Node *r= nullptr;
  for (Node *p= t; p; r= p, p= p->par) {
   splay(p), p->ch[1]= r;
   if constexpr (semigroup<M>::value) pushup(p);
  }
  return splay(t), r;
 }
 std::vector<Node> ns;
public:
 LinkCutTree(std::size_t n): ns(n) {}
 LinkCutTree(std::size_t n, T val): ns(n) {
  for (std::size_t i= n; i--;) ns[i].val= val;
 }
 void evert(std::size_t k) { expose(&ns[k]), toggle(&ns[k]), eval(&ns[k]); }
 void link(std::size_t c, std::size_t p) {
  evert(c), expose(&ns[p]), assert(!ns[c].par), ns[p].ch[1]= &ns[c], ns[c].par= &ns[p];
  if constexpr (semigroup<M>::value) pushup(&ns[p]);
 }
 void cut(std::size_t c, std::size_t p) {
  evert(p), expose(&ns[c]), assert(ns[c].ch[0] == &ns[p]), ns[c].ch[0]= ns[c].ch[0]->par= nullptr;
  if constexpr (semigroup<M>::value) pushup(&ns[c]);
 }
 int par(std::size_t x) {
  expose(&ns[x]);
  Node *t= ns[x].ch[0];
  if (!t) return -1;
  while (t->ch[1]) eval(t), t= t->ch[1];
  return splay(t), t - &ns[0];
 }
 int lca(std::size_t x, std::size_t y) {
  if (x == y) return x;
  expose(&ns[x]);
  Node *u= expose(&ns[y]);
  return ns[x].par ? u - &ns[0] : -1;
 }
 const T &operator[](std::size_t k) { return get(k); }
 const T &get(std::size_t k) {
  static_assert(semigroup<M>::value || dual<M>::value, "\"get\" is not available\n");
  return expose(&ns[k]), ns[k].val;
 }
 void set(std::size_t k, T v) {
  static_assert(semigroup<M>::value || dual<M>::value, "\"set\" is not available\n");
  expose(&ns[k]), ns[k].val= v;
  if constexpr (semigroup<M>::value) pushup(&ns[k]);
 }
 T fold(std::size_t a, std::size_t b) {  // [a,b] closed section
  static_assert(semigroup<M>::value, "\"fold\" is not available\n");
  if (a == b) return get(a);
  return evert(a), expose(&ns[b]), assert(ns[a].par), ns[b].sum;
 }
 void apply(std::size_t a, std::size_t b, E v) {  // [a,b] closed section
  static_assert(dual<M>::value, "\"apply\" is not available\n");
  evert(a), expose(&ns[b]), assert(a == b || ns[a].par), propagate(&ns[b], v), eval(&ns[b]);
 }
 static std::string which_available() {
  std::string ret= "";
  if constexpr (semigroup<M>::value || dual<M>::value) ret+= "\"set\" \"get\" ";
  if constexpr (semigroup<M>::value) ret+= "\"fold\" ";
  if constexpr (dual<M>::value) ret+= "\"apply\" ";
  return ret;
 }
};
