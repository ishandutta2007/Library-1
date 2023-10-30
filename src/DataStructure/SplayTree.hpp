#pragma once
#include <vector>
#include <string>
#include <array>
#include <cstddef>
#include <cassert>
#include "src/Internal/HAS_CHECK.hpp"
template <class M, bool reversible= false> class SplayTree {
 HAS_MEMBER(op);
 HAS_MEMBER(mp);
 HAS_MEMBER(cp);
 HAS_TYPE(T);
 HAS_TYPE(E);
 NULLPTR_OR(E);
 template <class L> static constexpr bool semigroup_v= std::conjunction_v<has_T<L>, has_op<L>>;
 template <class L> static constexpr bool dual_v= std::conjunction_v<has_T<L>, has_E<L>, has_mp<L>, has_cp<L>>;
 template <class T, class tDerived> struct Node_B {
  T val;
  tDerived *ch[2], *par;
  size_t size;
 };
 template <class D, bool sg, bool du, bool rev> struct Node_D: Node_B<M, Node_D<D, sg, du, rev>> {};
 template <class D> struct Node_D<D, 1, 0, 0>: Node_B<typename M::T, Node_D<D, 1, 0, 0>> {
  typename M::T sum;
 };
 template <class D> struct Node_D<D, 0, 1, 0>: Node_B<typename M::T, Node_D<D, 0, 1, 0>> {
  typename M::E laz;
  bool laz_flg= false;
 };
 template <class D> struct Node_D<D, 1, 1, 0>: Node_B<typename M::T, Node_D<D, 1, 1, 0>> {
  typename M::T sum;
  typename M::E laz;
  bool laz_flg= false;
 };
 template <class D> struct Node_D<D, 0, 0, 1>: Node_B<M, Node_D<D, 0, 0, 1>> {
  bool revflg= false;
 };
 template <class D> struct Node_D<D, 1, 0, 1>: Node_B<typename M::T, Node_D<D, 1, 0, 1>> {
  typename M::T sum, rsum;
  bool revflg= false;
 };
 template <class D> struct Node_D<D, 0, 1, 1>: Node_B<typename M::T, Node_D<D, 0, 1, 1>> {
  typename M::E laz;
  bool laz_flg= false, revflg= false;
 };
 template <class D> struct Node_D<D, 1, 1, 1>: Node_B<typename M::T, Node_D<D, 1, 1, 1>> {
  typename M::T sum, rsum;
  typename M::E laz;
  bool laz_flg= false, revflg= false;
 };
 using Node= Node_D<void, semigroup_v<M>, dual_v<M>, reversible>;
 friend std::ostream &operator<<(std::ostream &os, Node t) { return os << "{ch[2]:(" << t.ch[0] << "," << t.ch[1] << "),par:" << t.par << "}"; }
 using T= decltype(Node::val);
 using E= nullptr_or_E_t<M>;
 using np= Node *;
 np root;
 template <class S> static inline np build(size_t bg, size_t ed, np par, const S &val) {
  if (bg == ed) return nullptr;
  size_t mid= bg + (ed - bg) / 2;
  np t;
  if constexpr (std::is_same_v<S, T>) t= new Node{val};
  else t= new Node{*(val + mid)};
  return t->par= par, t->ch[0]= build(bg, mid, t, val), t->ch[1]= build(mid + 1, ed, t, val), update(t);
 }
 static inline void dump(typename std::vector<T>::iterator itr, np t) {
  if (!t) return;
  if constexpr (dual_v<M>) push_propagate(t);
  if constexpr (reversible) push_toggle(t);
  size_t sz= t->ch[0] ? t->ch[0]->size : 0;
  *(itr + sz)= t->val, dump(itr, t->ch[0]), dump(itr + sz + 1, t->ch[1]);
 }
 template <bool b> static inline void helper(np t) {
  if (!t->ch[b]) return;
  t->size+= t->ch[b]->size;
  if constexpr (semigroup_v<M>)
   if constexpr (b) {
    t->sum= M::op(t->sum, t->ch[1]->sum);
    if constexpr (reversible) t->rsum= M::op(t->ch[1]->rsum, t->rsum);
   } else {
    t->sum= M::op(t->ch[0]->sum, t->sum);
    if constexpr (reversible) t->rsum= M::op(t->rsum, t->ch[0]->rsum);
   }
 }
 static inline np update(np t) {
  if (!t) return t;
  t->size= 1;
  if constexpr (semigroup_v<M>) {
   t->sum= t->val;
   if constexpr (reversible) t->rsum= t->sum;
  }
  return helper<0>(t), helper<1>(t), t;
 }
 static inline void propagate(np t, const E &x) {
  if (!t) return;
  if (t->laz_flg) M::cp(t->laz, x);
  else t->laz= x;
  if constexpr (semigroup_v<M>) {
   M::mp(t->sum, x, t->size);
   if constexpr (reversible) M::mp(t->rsum, x, t->size);
  }
  M::mp(t->val, x, 1), t->laz_flg= true;
 }
 static inline void toggle(np t) {
  if (!t) return;
  if constexpr (semigroup_v<M>) std::swap(t->sum, t->rsum);
  std::swap(t->ch[0], t->ch[1]), t->revflg= !t->revflg;
 }
 static inline void push_propagate(np t) {
  if (t->laz_flg) propagate(t->ch[0], t->laz), propagate(t->ch[1], t->laz), t->laz_flg= false;
 }
 static inline void push_toggle(np t) {
  if (t->revflg) toggle(t->ch[0]), toggle(t->ch[1]), t->revflg= false;
 }
 static inline int dir(np t) { return t->par->ch[1] == t; }
 static inline void rot(np t) {
  np p= t->par;
  int d= dir(t);
  if ((p->ch[d]= t->ch[!d])) p->ch[d]->par= p;
  t->ch[!d]= p;
  if ((t->par= p->par)) p->par->ch[dir(p)]= t;
  p->par= t, update(p);
 }
 static inline void splay_(np t) {
  if constexpr (dual_v<M>) push_propagate(t);
  if constexpr (reversible) push_toggle(t);
  for (np p= t->par; p; rot(t), p= t->par)
   if (np pp= p->par; pp) rot(dir(t) == dir(p) ? p : t);
  update(t);
 }
 static inline void splay(np &t, size_t k) {
  if (!t) return;
  for (bool d;; t= t->ch[d]) {
   size_t sz= t->ch[0] ? t->ch[0]->size : 0;
   if (sz == k) break;
   if constexpr (dual_v<M>) push_propagate(t);
   if constexpr (reversible) push_toggle(t);
   if ((d= sz < k)) k-= sz + 1;
  }
  splay_(t);
 }
 static inline void between(np &t, size_t a, size_t b) {
  assert(a < b), assert(b <= t->size);
  if (!a && b == t->size) return;
  if (!a) splay(t, b), t= t->ch[0];
  else if (b == t->size) splay(t, a - 1), t= t->ch[1];
  else splay(t, b), t->ch[0]->par= nullptr, splay(t->ch[0], a - 1), t->ch[0]->par= t, t= t->ch[0]->ch[1];
 }
public:
 SplayTree(np t= nullptr): root(t) {}
 SplayTree(size_t n, T val= T()): root(build(0, n, nullptr, val)) {}
 SplayTree(const T *bg, const T *ed): root(build(0, ed - bg, nullptr, bg)) {}
 SplayTree(const std::vector<T> &ar): SplayTree(ar.data(), ar.data() + ar.size()) {}
 std::vector<T> dump() {
  std::vector<T> ret(size());
  return dump(ret.begin(), root), ret;
 }
 static std::string which_available() {
  std::string ret= "";
  if constexpr (semigroup_v<M>) ret+= "\"fold\" ";
  else ret+= "\"at\" ";
  if constexpr (dual_v<M>) ret+= "\"apply\" ";
  if constexpr (reversible) ret+= "\"reverse\" ";
  return ret;
 }
 size_t size() { return root ? root->size : 0; }
 void clear() { root= nullptr; }
 template <class L= M> const std::enable_if_t<semigroup_v<L>, T> &operator[](size_t k) { return get(k); }
 template <class L= M> std::enable_if_t<!semigroup_v<L>, T> &operator[](size_t k) { return at(k); }
 const T &get(size_t k) { return splay(root, k), root->val; }
 T &at(size_t k) {
  static_assert(!semigroup_v<M>, "\"at\" is not available");
  return splay(root, k), root->val;
 }
 void set(size_t k, T val) { splay(root, k), root->val= val, update(root); }
 T fold(size_t a, size_t b) {
  static_assert(semigroup_v<M>, "\"fold\" is not available");
  between(root, a, b);
  T ret= root->sum;
  return splay_(root), ret;
 }
 void apply(size_t a, size_t b, E x) {
  static_assert(dual_v<M>, "\"apply\" is not available");
  between(root, a, b), propagate(root, x), splay_(root);
 }
 void reverse(size_t a, size_t b) {
  static_assert(reversible, "\"reverse\" is not available");
  between(root, a, b), toggle(root), splay_(root);
 }
 std::array<SplayTree, 2> split(size_t k) {
  assert(k <= size());
  if (!k) return {SplayTree(), *this};
  if (size() == k) return {*this, SplayTree()};
  splay(root, k);
  np l= root->ch[0];
  root->ch[0]= l->par= nullptr;
  return {SplayTree(l), SplayTree(update(root))};
 }
 std::array<SplayTree, 3> split3(size_t a, size_t b) {
  auto [tmp, right]= split(b);
  auto [left, center]= tmp.split(a);
  return {left, center, right};
 }
 SplayTree &operator+=(SplayTree rhs) {  // merge
  if (!root) root= rhs.root;
  else if (rhs.root) splay(root, root->size - 1), root->ch[1]= rhs.root, rhs.root->par= root, update(root);
  return *this;
 }
 SplayTree &operator+(SplayTree rhs) { return *this+= rhs; }
 void push_back(T val) { insert(size(), val); }
 void push_front(T val) { insert(0, val); }
 void insert(size_t k, T val) {
  assert(k <= size());
  auto [l, r]= split(k);
  *this= l + SplayTree(update(new Node{val, {nullptr, nullptr}, nullptr})) + r;
 }
 T pop_back() { return erase(root->size - 1); }
 T pop_front() { return erase(0); }
 T erase(size_t k) {
  assert(root && k < root->size);
  auto [l, c, r]= split3(k, k + 1);
  return *this= l + r, c.root->val;
 }
};