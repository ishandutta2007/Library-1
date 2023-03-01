#pragma once
#include <vector>
#include <string>
#include <array>
#include <cstddef>
#include <cassert>
#include "src/Internal/HAS_CHECK.hpp"
template <class M, bool reversible= false> class SplayTree {
 HAS_MEMBER(op);
 HAS_MEMBER(mapping);
 HAS_MEMBER(composition);
 HAS_TYPE(T);
 HAS_TYPE(E);
 template <class L> static constexpr bool semigroup_v= std::conjunction_v<has_T<L>, has_op<L>>;
 template <class L> static constexpr bool dual_v= std::conjunction_v<has_T<L>, has_E<L>, has_mapping<L>, has_composition<L>>;
 template <class T, class tDerived, class F= std::nullptr_t> struct Node_B {
  using E= F;
  T val;
  tDerived *ch[2], *par;
  std::size_t size;
 };
 template <class D, bool sg, bool du, bool rev> struct Node_D: Node_B<M, Node_D<D, sg, du, rev>> {};
 template <class D> struct Node_D<D, 1, 0, 0>: Node_B<typename M::T, Node_D<D, 1, 0, 0>> {
  typename M::T sum;
 };
 template <class D> struct Node_D<D, 0, 1, 0>: Node_B<typename M::T, Node_D<D, 0, 1, 0>, typename M::E> {
  typename M::E lazy;
  bool lazy_flg= false;
 };
 template <class D> struct Node_D<D, 1, 1, 0>: Node_B<typename M::T, Node_D<D, 1, 1, 0>, typename M::E> {
  typename M::T sum;
  typename M::E lazy;
  bool lazy_flg= false;
 };
 template <class D> struct Node_D<D, 0, 0, 1>: Node_B<M, Node_D<D, 0, 0, 1>> {
  bool revflg= false;
 };
 template <class D> struct Node_D<D, 1, 0, 1>: Node_B<typename M::T, Node_D<D, 1, 0, 1>> {
  typename M::T sum, rsum;
  bool revflg= false;
 };
 template <class D> struct Node_D<D, 0, 1, 1>: Node_B<typename M::T, Node_D<D, 0, 1, 1>, typename M::E> {
  typename M::E lazy;
  bool lazy_flg= false, revflg= false;
 };
 template <class D> struct Node_D<D, 1, 1, 1>: Node_B<typename M::T, Node_D<D, 1, 1, 1>, typename M::E> {
  typename M::T sum, rsum;
  typename M::E lazy;
  bool lazy_flg= false, revflg= false;
 };
 using Node= Node_D<void, semigroup_v<M>, dual_v<M>, reversible>;
 friend std::ostream &operator<<(std::ostream &os, Node t) { return os << "{ch[2]:(" << t.ch[0] << "," << t.ch[1] << "),par:" << t.par << "}"; }
 using T= decltype(Node::val);
 using E= typename Node::E;
 Node *root;
 static inline Node *build(const T *bg, const T *ed, Node *par) {
  if (bg == ed) return nullptr;
  const T *mid= bg + (ed - bg) / 2;
  Node *t= new Node{*mid};
  t->par= par, t->ch[0]= build(bg, mid, t), t->ch[1]= build(mid + 1, ed, t);
  return pushup(t);
 }
 static inline Node *build(std::size_t bg, std::size_t ed, const T &val, Node *par) {
  if (bg == ed) return nullptr;
  std::size_t mid= bg + (ed - bg) / 2;
  Node *t= new Node{val};
  t->par= par, t->ch[0]= build(bg, mid, val, t), t->ch[1]= build(mid + 1, ed, val, t);
  return pushup(t);
 }
 static inline void dump(typename std::vector<T>::iterator itr, Node *t) {
  if (!t) return;
  if constexpr (dual_v<M>) eval_propagate(t);
  if constexpr (reversible) eval_toggle(t);
  std::size_t sz= t->ch[0] ? t->ch[0]->size : 0;
  *(itr + sz)= t->val, dump(itr, t->ch[0]), dump(itr + sz + 1, t->ch[1]);
 }
 template <bool b> static inline void helper(Node *t) {
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
 static inline Node *pushup(Node *t) {
  if (!t) return t;
  t->size= 1;
  if constexpr (semigroup_v<M>) {
   t->sum= t->val;
   if constexpr (reversible) t->rsum= t->sum;
  }
  return helper<0>(t), helper<1>(t), t;
 }
 static inline void propagate(Node *t, const E &x) {
  if (!t) return;
  t->lazy_flg ? (M::composition(t->lazy, x), x) : t->lazy= x;
  if constexpr (semigroup_v<M>) {
   M::mapping(t->sum, x, t->size);
   if constexpr (reversible) M::mapping(t->rsum, x, t->size);
  }
  M::mapping(t->val, x, 1), t->lazy_flg= true;
 }
 static inline void toggle(Node *t) {
  if (!t) return;
  if constexpr (semigroup_v<M>) std::swap(t->sum, t->rsum);
  std::swap(t->ch[0], t->ch[1]), t->revflg= !t->revflg;
 }
 static inline void eval_propagate(Node *t) {
  if (t->lazy_flg) propagate(t->ch[0], t->lazy), propagate(t->ch[1], t->lazy), t->lazy_flg= false;
 }
 static inline void eval_toggle(Node *t) {
  if (t->revflg) toggle(t->ch[0]), toggle(t->ch[1]), t->revflg= false;
 }
 static inline int dir(Node *t) { return t->par->ch[1] == t; }
 static inline void rot(Node *t) {
  Node *p= t->par;
  int d= dir(t);
  if ((p->ch[d]= t->ch[!d])) p->ch[d]->par= p;
  t->ch[!d]= p;
  if ((t->par= p->par)) p->par->ch[dir(p)]= t;
  p->par= t, pushup(p);
 }
 static inline void splay_(Node *t) {
  if constexpr (dual_v<M>) eval_propagate(t);
  if constexpr (reversible) eval_toggle(t);
  for (Node *p= t->par; p; rot(t), p= t->par)
   if (Node *pp= p->par; pp) rot(dir(t) == dir(p) ? p : t);
  pushup(t);
 }
 static inline void splay(Node *&t, std::size_t k) {
  if (!t) return;
  for (bool d;; t= t->ch[d]) {
   std::size_t sz= t->ch[0] ? t->ch[0]->size : 0;
   if (sz == k) break;
   if constexpr (dual_v<M>) eval_propagate(t);
   if constexpr (reversible) eval_toggle(t);
   if ((d= sz < k)) k-= sz + 1;
  }
  splay_(t);
 }
 static inline void between(Node *&t, std::size_t a, std::size_t b) {
  assert(a < b), assert(b <= t->size);
  if (!a && b == t->size) return;
  if (!a) splay(t, b), t= t->ch[0];
  else if (b == t->size) splay(t, a - 1), t= t->ch[1];
  else splay(t, b), t->ch[0]->par= nullptr, splay(t->ch[0], a - 1), t->ch[0]->par= t, t= t->ch[0]->ch[1];
 }
public:
 SplayTree(Node *t= nullptr): root(t) {}
 SplayTree(std::size_t n, T val= T()) { root= build(0, n, val, nullptr); }
 SplayTree(const T *bg, const T *ed) { root= build(bg, ed, nullptr); }
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
 std::size_t size() { return root ? root->size : 0; }
 void clear() { root= nullptr; }
 template <class L= M> const std::enable_if_t<semigroup_v<L>, T> &operator[](std::size_t k) { return get(k); }
 template <class L= M> std::enable_if_t<!semigroup_v<L>, T> &operator[](std::size_t k) { return at(k); }
 const T &get(std::size_t k) { return splay(root, k), root->val; }
 T &at(std::size_t k) {
  static_assert(!semigroup_v<M>, "\"at\" is not available");
  return splay(root, k), root->val;
 }
 void set(std::size_t k, T val) { splay(root, k), root->val= val, pushup(root); }
 T fold(std::size_t a, std::size_t b) {
  static_assert(semigroup_v<M>, "\"fold\" is not available");
  between(root, a, b);
  T ret= root->sum;
  return splay_(root), ret;
 }
 void apply(std::size_t a, std::size_t b, E x) {
  static_assert(dual_v<M>, "\"apply\" is not available");
  between(root, a, b), propagate(root, x), splay_(root);
 }
 void reverse(std::size_t a, std::size_t b) {
  static_assert(reversible, "\"reverse\" is not available");
  between(root, a, b), toggle(root), splay_(root);
 }
 std::array<SplayTree, 2> split(std::size_t k) {
  assert(k <= size());
  if (!k) return {SplayTree(), *this};
  if (size() == k) return {*this, SplayTree()};
  splay(root, k);
  Node *l= root->ch[0];
  root->ch[0]= l->par= nullptr;
  return {SplayTree(l), SplayTree(pushup(root))};
 }
 std::array<SplayTree, 3> split3(std::size_t a, std::size_t b) {
  auto [tmp, right]= split(b);
  auto [left, center]= tmp.split(a);
  return {left, center, right};
 }
 SplayTree &operator+=(SplayTree rhs) {  // merge
  if (!root) root= rhs.root;
  else if (rhs.root) splay(root, root->size - 1), root->ch[1]= rhs.root, rhs.root->par= root, pushup(root);
  return *this;
 }
 SplayTree &operator+(SplayTree rhs) { return *this+= rhs; }
 void push_back(T val) { insert(size(), val); }
 void push_front(T val) { insert(0, val); }
 void insert(std::size_t k, T val) {
  assert(k <= size());
  auto [l, r]= split(k);
  *this= l + SplayTree(pushup(new Node{val, {nullptr, nullptr}, nullptr})) + r;
 }
 T pop_back() { return erase(root->size - 1); }
 T pop_front() { return erase(0); }
 T erase(std::size_t k) {
  assert(root && k < root->size);
  auto [l, c, r]= split3(k, k + 1);
  return *this= l + r, c.root->val;
 }
};