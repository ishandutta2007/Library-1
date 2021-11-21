#pragma once
#include <bits/stdc++.h>
/**
 * @title Splay木
 * @category データ構造
 * @brief O(logN)
 * 木の深さが10^5ぐらいになると再帰関数がスタックオーバーフロー起こすので注意
 * set_balance() でランダムにsplayするのでならすことができるはず
 * 単位元は必要なし（遅延側も）
 * 区間reverseができる。（半群の可換性を仮定していないので無駄にメモリと時間を使うけど）
 * 各ノードが部分木のサイズを保持しているのでmapping関数では引数としてsizeを渡せる
 */

// bgIN CUT HERE
#define HAS_CHECK(member, Dummy)                              \
  template <class T>                                          \
  struct has_##member {                                       \
    template <class U, Dummy>                                 \
    static std::true_type check(U *);                         \
    static std::false_type check(...);                        \
    static T *mClass;                                         \
    static const bool value = decltype(check(mClass))::value; \
  };
#define HAS_MEMBER(member) HAS_CHECK(member, int dummy = (&U::member, 0))
#define HAS_TYPE(member) HAS_CHECK(member, class dummy = typename U::member)

template <class M, bool reversible = false>
class SplayTree {
  HAS_MEMBER(op);
  HAS_MEMBER(mapping);
  HAS_MEMBER(composition)
  HAS_TYPE(T);
  HAS_TYPE(E);
  template <class L>
  using semigroup = std::conjunction<has_T<L>, has_op<L>>;
  template <class L>
  using dual =
      std::conjunction<has_T<L>, has_E<L>, has_mapping<L>, has_composition<L>>;
  template <class T, class tDerived, class F = std::nullptr_t>
  struct Node_B {
    using E = F;
    T val;
    tDerived *ch[2];
    std::size_t size;
  };
  template <bool sg_, bool du_, bool rev_, typename tEnable = void>
  struct Node_D : Node_B<M, Node_D<sg_, du_, rev_, tEnable>> {};
  template <bool sg_, bool du_, bool rev_>
  struct Node_D<sg_, du_, rev_, typename std::enable_if_t<sg_ && !du_ && !rev_>>
      : Node_B<typename M::T, Node_D<sg_, du_, rev_>> {
    typename M::T sum;
  };
  template <bool sg_, bool du_, bool rev_>
  struct Node_D<sg_, du_, rev_, typename std::enable_if_t<!sg_ && du_ && !rev_>>
      : Node_B<typename M::T, Node_D<sg_, du_, rev_>, typename M::E> {
    typename M::E lazy;
    bool lazy_flg = false;
  };
  template <bool sg_, bool du_, bool rev_>
  struct Node_D<sg_, du_, rev_, typename std::enable_if_t<sg_ && du_ && !rev_>>
      : Node_B<typename M::T, Node_D<sg_, du_, rev_>, typename M::E> {
    typename M::T sum;
    typename M::E lazy;
    bool lazy_flg = false;
  };
  template <bool sg_, bool du_, bool rev_>
  struct Node_D<sg_, du_, rev_, typename std::enable_if_t<!sg_ && !du_ && rev_>>
      : Node_B<M, Node_D<sg_, du_, rev_>> {
    bool rev_flg = false;
  };
  template <bool sg_, bool du_, bool rev_>
  struct Node_D<sg_, du_, rev_, typename std::enable_if_t<sg_ && !du_ && rev_>>
      : Node_B<typename M::T, Node_D<sg_, du_, rev_>> {
    typename M::T sum, rsum;
    bool rev_flg = false;
  };
  template <bool sg_, bool du_, bool rev_>
  struct Node_D<sg_, du_, rev_, typename std::enable_if_t<!sg_ && du_ && rev_>>
      : Node_B<typename M::T, Node_D<sg_, du_, rev_>, typename M::E> {
    typename M::E lazy;
    bool lazy_flg = false, rev_flg = false;
  };
  template <bool sg_, bool du_, bool rev_>
  struct Node_D<sg_, du_, rev_, typename std::enable_if_t<sg_ && du_ && rev_>>
      : Node_B<typename M::T, Node_D<sg_, du_, rev_>, typename M::E> {
    typename M::T sum, rsum;
    typename M::E lazy;
    bool lazy_flg = false, rev_flg = false;
  };
  using Node = Node_D<semigroup<M>::value, dual<M>::value, reversible>;
  using T = decltype(Node::val);
  using E = typename Node::E;
  Node *root;
  Node *build(T *bg, T *ed) {
    if (bg == ed) return nullptr;
    T *mid = bg + (ed - bg) / 2;
    return pushup(new Node{*mid, {build(bg, mid), build(mid + 1, ed)}});
  }
  Node *build(std::size_t bg, std::size_t ed, const T &val) {
    if (bg == ed) return nullptr;
    std::size_t mid = bg + (ed - bg) / 2;
    return pushup(
        new Node{val, {build(bg, mid, val), build(mid + 1, ed, val)}});
  }
  void dump(typename std::vector<T>::iterator itr, Node *t) {
    if (!t) return;
    if constexpr (dual<M>::value) eval_propagate(t);
    if constexpr (reversible) eval_toggle(t);
    std::size_t sz = t->ch[0] ? t->ch[0]->size : 0;
    *(itr + sz) = t->val, dump(itr, t->ch[0]), dump(itr + sz + 1, t->ch[1]);
  }
  template <bool b>
  void helper(Node *&t) {
    if (!t->ch[b]) return;
    t->size += t->ch[b]->size;
    if constexpr (semigroup<M>::value)
      if constexpr (b) {
        t->sum = M::op(t->sum, t->ch[1]->sum);
        if constexpr (reversible) t->rsum = M::op(t->ch[1]->rsum, t->rsum);
      } else {
        t->sum = M::op(t->ch[0]->sum, t->sum);
        if constexpr (reversible) t->rsum = M::op(t->rsum, t->ch[0]->rsum);
      }
  }
  inline Node *pushup(Node *t) {
    if (!t) return t;
    t->size = 1;
    if constexpr (semigroup<M>::value) {
      t->sum = t->val;
      if constexpr (reversible) t->rsum = t->val;
    }
    return helper<0>(t), helper<1>(t), t;
  }
  inline void propagate(Node *t, const E &x) {
    if (!t) return;
    t->lazy = t->lazy_flg ? M::composition(t->lazy, x) : x;
    if constexpr (semigroup<M>::value) {
      t->sum = M::mapping(t->sum, x, t->size);
      if constexpr (reversible) t->rsum = M::mapping(t->rsum, x, t->size);
    }
    t->val = M::mapping(t->val, x, 1), t->lazy_flg = true;
  }
  inline void toggle(Node *t) {
    if (!t) return;
    if constexpr (semigroup<M>::value) std::swap(t->sum, t->rsum);
    std::swap(t->ch[0], t->ch[1]), t->rev_flg = !t->rev_flg;
  }
  inline void eval_propagate(Node *t) {
    if (t->lazy_flg)
      propagate(t->ch[0], t->lazy), propagate(t->ch[1], t->lazy),
          t->lazy_flg = false;
  }
  inline void eval_toggle(Node *t) {
    if (t->rev_flg) toggle(t->ch[0]), toggle(t->ch[1]), t->rev_flg = false;
  }
  inline void rot(Node *&t, bool d) {
    Node *s = t->ch[d];
    t->ch[d] = s->ch[!d], s->ch[!d] = pushup(t), t = pushup(s);
  }
  inline void splay(Node *&t, std::size_t k) {
    if (!t) return;
    if constexpr (dual<M>::value) eval_propagate(t);
    if constexpr (reversible) eval_toggle(t);
    std::size_t sz = t->ch[0] ? t->ch[0]->size : 0;
    if (sz == k) return;
    bool d = sz < k;
    if (d) k -= sz + 1;
    if constexpr (dual<M>::value) eval_propagate(t->ch[d]);
    if constexpr (reversible) eval_toggle(t->ch[d]);
    sz = t->ch[d]->ch[0] ? t->ch[d]->ch[0]->size : 0;
    if (sz != k) {
      bool c = sz < k;
      if (c) k -= sz + 1;
      splay(t->ch[d]->ch[c], k);
      c == d ? rot(t, d) : rot(t->ch[d], !d);
    }
    rot(t, d);
  }
  template <class F>
  void query(std::size_t a, std::size_t b, const F &f) {
    if (size() == b) {
      a-- ? (splay(root, a), f(root->ch[1]), pushup(root)) : (f(root), root);
    } else {
      splay(root, b);
      a-- ? (splay(root->ch[0], a), f(root->ch[0]->ch[1]), pushup(root->ch[0]))
          : (f(root->ch[0]), root->ch[0]);
      pushup(root);
    }
  }
  static inline unsigned xor128() {
    static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
    unsigned t = (x ^ (x << 11));
    return x = y, y = z, z = w, (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
  }

 public:
  SplayTree(Node *t = nullptr) : root(t) {}
  SplayTree(std::size_t n, T val) { root = build(0, n, val); }
  SplayTree(T *bg, T *ed) { root = build(bg, ed); }
  SplayTree(const std::vector<T> &ar)
      : SplayTree(ar.data(), ar.data() + ar.size()) {}
  std::vector<T> dump() {
    std::vector<T> ret(size());
    return dump(ret.bgin(), root), ret;
  }
  static std::string which_available() {
    std::string ret = "";
    if constexpr (semigroup<M>::value)
      ret += "\"fold\" ";
    else
      ret += "\"at\" ";
    if constexpr (dual<M>::value) ret += "\"apply\" ";
    if constexpr (reversible) ret += "\"reverse\" ";
    return ret;
  }
  std::size_t size() { return root ? root->size : 0; }
  void clear() { root = nullptr; }
  template <class L = M,
            typename std::enable_if_t<semigroup<L>::value> * = nullptr>
  const T &operator[](id_t k) {
    return get(k);
  }
  template <class L = M,
            typename std::enable_if_t<!semigroup<L>::value> * = nullptr>
  T &operator[](id_t k) {
    return at(k);
  }
  const T &get(std::size_t k) { return splay(root, k), root->val; }
  T &at(std::size_t k) {
    static_assert(!semigroup<M>::value, "\"at\" is not available");
    return splay(root, k), root->val;
  }
  void set(std::size_t k, T val) {
    splay(root, k), root->val = val, pushup(root);
  }
  void set_balance() {
    if (root) splay(root, xor128() % size()), splay(root, xor128() % size());
  }
  T fold(std::size_t a, std::size_t b) {
    static_assert(semigroup<M>::value, "\"fold\" is not available");
    if (size() == b) {
      return a-- ? splay(root, a), root->ch[1]->sum : root->sum;
    } else {
      splay(root, b);
      return a-- ? (splay(root->ch[0], a), root->ch[0]->ch[1]->sum)
                 : root->ch[0]->sum;
    }
  }
  void apply(std::size_t a, std::size_t b, E x) {
    static_assert(dual<M>::value, "\"apply\" is not available");
    query(a, b, [&](Node *t) { propagate(t, x); });
  }
  void reverse(std::size_t a, std::size_t b) {
    static_assert(reversible, "\"reverse\" is not available");
    query(a, b, [&](Node *t) { toggle(t); });
  }
  std::pair<SplayTree, SplayTree> split(std::size_t k) {
    assert(k <= size());
    if (size() == k) return {*this, SplayTree()};
    splay(root, k);
    Node *l = root->ch[0];
    root->ch[0] = nullptr;
    return {SplayTree(l), SplayTree(pushup(root))};
  }
  std::tuple<SplayTree, SplayTree, SplayTree> split3(std::size_t a,
                                                     std::size_t b) {
    auto [tmp, right] = split(b);
    auto [left, center] = tmp.split(a);
    return {left, center, right};
  }
  SplayTree &operator+=(SplayTree rhs) {  // merge
    root ? (splay(root, root->size - 1), root->ch[1] = rhs.root, pushup(root))
         : root = rhs.root;
    return *this;
  }
  SplayTree &operator+(SplayTree rhs) { return *this += rhs; }
  void push_back(T val) { insert(size(), val); }
  void push_front(T val) { insert(0, val); }
  void insert(std::size_t k, T val) {
    assert(!k || (root && k <= root->size));
    if (size() == k) {
      root = pushup(new Node{val, {root, nullptr}});
    } else {
      splay(root, k), root = new Node{val, {root->ch[0], root}};
      root->ch[1]->ch[0] = nullptr, pushup(root->ch[1]), pushup(root);
    }
  }
  T pop_back() { return erase(root->size - 1); }
  T pop_front() { return erase(0); }
  T erase(std::size_t k) {
    assert(root && k < root->size);
    splay(root, k);
    T ret = root->val;
    splay(root->ch[1], 0);
    if (root->ch[1])
      root->ch[1]->ch[0] = root->ch[0], root = pushup(root->ch[1]);
    else
      root = root->ch[0];
    return ret;
  }
};