#pragma once
#include <bits/stdc++.h>
/**
 * @title Skew-Heap
 * @category データ構造
 * @brief マージできるヒープ
 * @brief top: O(1), pop, push, merge: O(logN)
 * @brief apply(v): 全体に作用素vを適用
 */

// BEGIN CUT HERE

#define HAS_CHECK(member, Dummy)                              \
  template <class tClass>                                     \
  struct has_##member {                                       \
    template <class U, Dummy>                                 \
    static std::true_type check(U *);                         \
    static std::false_type check(...);                        \
    static tClass *mClass;                                    \
    static const bool value = decltype(check(mClass))::value; \
  };
#define HAS_MEMBER(member) HAS_CHECK(member, int dummy = (&U::member, 0))
#define HAS_TYPE(member) HAS_CHECK(member, class dummy = typename U::member)

template <typename T, typename Compare = std::less<T>, typename M = void>
struct SkewHeap {
  HAS_MEMBER(mapping);
  HAS_MEMBER(composition);
  HAS_TYPE(E);
  template <class L>
  using dual = std::conjunction<has_E<L>, has_mapping<L>, has_composition<L>>;
  template <class tDerived, class U = std::nullptr_t>
  struct Node_B {
    using E = U;
    T key;
    tDerived *ch[2];
  };
  template <bool du_, typename tEnable = void>
  struct Node_D : Node_B<Node_D<du_>> {};
  template <bool du_>
  struct Node_D<du_, typename std::enable_if_t<du_>>
      : Node_B<Node_D<du_>, typename M::E> {
    typename M::E lazy;
    bool lazy_flg = false;
  };
  using Node = Node_D<dual<M>::value>;
  using E = typename Node::E;
  Node *root;
  static inline void propagate(Node *&t, const E &x) {
    if (!t) return;
    t->lazy_flg ? (M::composition(t->lazy, x), x) : (t->lazy = x);
    M::mapping(t->key, x), t->lazy_flg = true;
  }
  static inline void eval(Node *t) {
    if (t->lazy_flg)
      propagate(t->ch[0], t->lazy), propagate(t->ch[1], t->lazy),
          t->lazy_flg = false;
  }
  Node *merge(Node *a, Node *b) {
    if (!a || !b) return a ? a : b;
    if (Compare()(a->key, b->key)) std::swap(a, b);
    if constexpr (dual<M>::value) eval(a);
    return std::swap(a->ch[0], a->ch[1] = merge(b, a->ch[1])), a;
  }

 public:
  /* max heap */
  SkewHeap() : root(nullptr) {}
  void push(T key) { root = merge(root, new Node{key}); }
  T pop() {
    T ret = root->key;
    if constexpr (dual<M>::value) eval(root);
    return root = merge(root->ch[0], root->ch[1]), ret;
  }
  T top() { return root->key; }
  bool empty() { return !root; }
  void apply(E v) {
    static_assert(dual<M>::value, "\"apply\" is not available\n");
    propagate(root, v);
  }
  SkewHeap &operator+=(SkewHeap r) { return root = merge(root, r.root), *this; }
  SkewHeap operator+(SkewHeap r) { return SkewHeap(*this) += r; }
};