#pragma once
#include <functional>
#include <algorithm>
#include <cstddef>
#include "src/Internal/HAS_CHECK.hpp"
template <typename T, typename Compare= std::less<T>, typename M= void> struct SkewHeap {
 HAS_MEMBER(mapping);
 HAS_MEMBER(composition);
 HAS_TYPE(E);
 NULLPTR_OR(E);
 template <class L> using dual= std::conjunction<has_E<L>, has_mapping<L>, has_composition<L>>;
 template <class tDerived> struct Node_B {
  T key;
  tDerived *ch[2];
 };
 template <bool du_, typename tEnable= void> struct Node_D: Node_B<Node_D<du_>> {};
 template <bool du_> struct Node_D<du_, typename std::enable_if_t<du_>>: Node_B<Node_D<du_>> {
  typename M::E lazy;
  bool lazy_flg= false;
 };
 using Node= Node_D<dual<M>::value>;
 using E= nullptr_or_E_t<M>;
 Node *root;
 static inline void propagate(Node *&t, const E &x) {
  if (!t) return;
  t->lazy_flg ? (M::composition(t->lazy, x), x) : (t->lazy= x);
  M::mapping(t->key, x), t->lazy_flg= true;
 }
 static inline void push(Node *t) {
  if (t->lazy_flg) propagate(t->ch[0], t->lazy), propagate(t->ch[1], t->lazy), t->lazy_flg= false;
 }
 Node *merge(Node *a, Node *b) {
  if (!a || !b) return a ? a : b;
  if (Compare()(a->key, b->key)) std::swap(a, b);
  if constexpr (dual<M>::value) push(a);
  return std::swap(a->ch[0], a->ch[1]= merge(b, a->ch[1])), a;
 }
public:
 /* max heap */ SkewHeap(): root(nullptr) {}
 void push(T key) { root= merge(root, new Node{key}); }
 T pop() {
  T ret= root->key;
  if constexpr (dual<M>::value) push(root);
  return root= merge(root->ch[0], root->ch[1]), ret;
 }
 T top() { return root->key; }
 bool empty() { return !root; }
 void apply(E v) {
  static_assert(dual<M>::value, "\"apply\" is not available\n");
  propagate(root, v);
 }
 SkewHeap &operator+=(SkewHeap r) { return root= merge(root, r.root), *this; }
 SkewHeap operator+(SkewHeap r) { return SkewHeap(*this)+= r; }
};