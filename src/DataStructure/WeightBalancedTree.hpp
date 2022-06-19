#pragma once
#include <bits/stdc++.h>
/**
 * @title 永続化Weight-Balanced-Tree
 * @category データ構造
 * @brief O(logN)
 * 永続平衡二分木
 * ※これは永続化してます（逆に非永続にできるようにしてません）
 * 単位元は必要なし（遅延側も）
 * 各ノードが葉のサイズを保持しているのでmapping関数では引数としてsizeを渡せる
 */

// verify用:
// https://atcoder.jp/contests/joisc2012/tasks/joisc2012_copypaste (永続)
// https://atcoder.jp/contests/arc030/tasks/arc030_4 (永続遅延伝搬)

// BEGIN CUT HERE

#ifndef HAS_CHECK
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
#endif

template <typename M, std::size_t NODE_SIZE = 1 << 23>
class WeightBalancedTree {
  HAS_MEMBER(op);
  HAS_MEMBER(mapping);
  HAS_MEMBER(composition);
  HAS_TYPE(T);
  HAS_TYPE(E);
  template <class L>
  using semigroup = std::conjunction<has_T<L>, has_op<L>>;
  template <class L>
  using dual =
      std::conjunction<has_T<L>, has_E<L>, has_mapping<L>, has_composition<L>>;
  using node_id = std::int_least32_t;
  template <class T, class F = std::nullptr_t>
  struct Node_B {
    using E = F;
    T val;
    std::size_t size = 0;
    node_id ch[2] = {0, 0};
  };
  template <bool sg_, bool du_, typename tEnable = void>
  struct Node_D : Node_B<M> {};
  template <bool sg_, bool du_>
  struct Node_D<sg_, du_, typename std::enable_if_t<sg_ && !du_>>
      : Node_B<typename M::T> {};
  template <bool sg_, bool du_>
  struct Node_D<sg_, du_, typename std::enable_if_t<du_>>
      : Node_B<typename M::T, typename M::E> {
    typename M::E lazy;
    bool lazy_flg = false;
  };
  using Node = Node_D<semigroup<M>::value, dual<M>::value>;
  using T = decltype(Node::val);
  using E = typename Node::E;
  using WBT = WeightBalancedTree;
  static inline Node n[NODE_SIZE];
  static inline node_id ni = 1;
  node_id root;
  static inline void pushup(node_id t) {
    n[t].size = n[n[t].ch[0]].size + n[n[t].ch[1]].size;
    if constexpr (semigroup<M>::value)
      n[t].val = M::op(n[n[t].ch[0]].val, n[n[t].ch[1]].val);
  }
  static inline void propagate(node_id t, const E &x) {
    n[t].lazy = n[t].lazy_flg ? M::composition(n[t].lazy, x) : x;
    n[t].val = M::mapping(n[t].val, x, n[t].size), n[t].lazy_flg = true;
  }
  static inline void cp_node(node_id &t) { n[t = ni++] = Node(n[t]); }
  static inline void eval(node_id t) {
    if (!n[t].lazy_flg) return;
    cp_node(n[t].ch[0]), cp_node(n[t].ch[1]), n[t].lazy_flg = false;
    propagate(n[t].ch[0], n[t].lazy), propagate(n[t].ch[1], n[t].lazy);
  }
  template <bool b>
  static inline node_id helper(std::array<node_id, 2> &m) {
    if constexpr (dual<M>::value) eval(m[b]);
    node_id c;
    if constexpr (b)
      c = submerge({m[0], n[m[1]].ch[0]});
    else
      c = submerge({n[m[0]].ch[1], m[1]});
    if (cp_node(m[b]), n[n[m[b]].ch[b]].size * 4 >= n[c].size)
      return n[m[b]].ch[!b] = c, pushup(m[b]), m[b];
    return n[m[b]].ch[!b] = n[c].ch[b], pushup(n[c].ch[b] = m[b]), pushup(c), c;
  }
  static inline node_id submerge(std::array<node_id, 2> m) {
    if (n[m[0]].size > n[m[1]].size * 4) return helper<0>(m);
    if (n[m[1]].size > n[m[0]].size * 4) return helper<1>(m);
    return n[ni] = Node{T(), 0, {m[0], m[1]}}, pushup(ni), ni++;
  }
  static inline node_id merge(node_id l, node_id r) {
    return !l ? r : (!r ? l : submerge({l, r}));
  }
  static inline std::pair<node_id, node_id> split(node_id t, std::size_t k) {
    if (!t) return {0, 0};
    if (k == 0) return {0, t};
    if (k >= n[t].size) return {t, 0};
    if constexpr (dual<M>::value) eval(t);
    if (k == n[n[t].ch[0]].size) return {n[t].ch[0], n[t].ch[1]};
    if (k < n[n[t].ch[0]].size) {
      auto [ll, m] = split(n[t].ch[0], k);
      return {ll, merge(m, n[t].ch[1])};
    } else {
      auto [rl, rr] = split(n[t].ch[1], k - n[n[t].ch[0]].size);
      return {merge(n[t].ch[0], rl), rr};
    }
  }
  template <class S>
  node_id build(std::size_t l, std::size_t r, const S &bg) {
    if (r - l == 1) {
      if constexpr (std::is_same_v<S, T>)
        return n[ni] = Node{bg, 1}, ni++;
      else
        return n[ni] = Node{*(bg + l), 1}, ni++;
    }
    return merge(build(l, (l + r) >> 1, bg), build((l + r) >> 1, r, bg));
  }
  void dump(node_id t, typename std::vector<T>::iterator it) {
    if (!n[t].ch[0]) return *it = n[t].val, void();
    if constexpr (dual<M>::value) eval(t);
    dump(n[t].ch[0], it), dump(n[t].ch[1], it + n[n[t].ch[0]].size);
  }
  T fold(node_id t, const std::size_t &l, const std::size_t &r, std::size_t bl,
         std::size_t br) {
    if (l <= bl && br <= r) return n[t].val;
    if constexpr (dual<M>::value) eval(t);
    std::size_t m = bl + n[n[t].ch[0]].size;
    if (r <= m) return fold(n[t].ch[0], l, r, bl, m);
    if (m <= l) return fold(n[t].ch[1], l, r, m, br);
    return M::op(fold(n[t].ch[0], l, r, bl, m), fold(n[t].ch[1], l, r, m, br));
  }
  void apply(node_id &t, const std::size_t &l, const std::size_t &r,
             std::size_t bl, std::size_t br, const E &x) {
    if (r <= bl || br <= l) return;
    if (cp_node(t); l <= bl && br <= r) return propagate(t, x), void();
    eval(t);
    std::size_t m = bl + n[n[t].ch[0]].size;
    apply(n[t].ch[0], l, r, bl, m, x), apply(n[t].ch[1], l, r, m, br, x);
    if constexpr (semigroup<M>::value) pushup(t);
  }
  void set_val(node_id &t, std::size_t k, const T &x) {
    if (cp_node(t); !n[t].ch[0]) return n[t].val = x, void();
    if constexpr (dual<M>::value) eval(t);
    bool flg = n[n[t].ch[0]].size <= k;
    set_val(n[t].ch[flg], flg ? k - n[n[t].ch[0]].size : k, x);
    if constexpr (semigroup<M>::value) pushup(t);
  }
  T get_val(node_id t, std::size_t k) {
    if (!n[t].ch[0]) return n[t].val;
    if constexpr (dual<M>::value) eval(t);
    bool flg = n[n[t].ch[0]].size <= k;
    return get_val(n[t].ch[flg], flg ? k - n[n[t].ch[0]].size : k);
  }
  T &at_val(node_id t, std::size_t k) {
    if (cp_node(t); !n[t].ch[0]) return n[t].val;
    if constexpr (dual<M>::value) eval(t);
    bool flg = n[n[t].ch[0]].size <= k;
    return at_val(n[t].ch[flg], flg ? k - n[n[t].ch[0]].size : k);
  }

 public:
  WeightBalancedTree(node_id t = 0) : root(t) {}
  WeightBalancedTree(std::size_t n, T val) { root = build(0, n, val); }
  WeightBalancedTree(const T *bg, const T *ed) { root = build(0, ed - bg, bg); }
  WeightBalancedTree(const std::vector<T> &ar)
      : WeightBalancedTree(ar.data(), ar.data() + ar.size()){};
  WBT &operator+=(WBT rhs) { return root = merge(root, rhs.root), *this; }
  WBT operator+(WBT rhs) { return WBT(*this) += rhs; }
  std::pair<WBT, WBT> split(std::size_t k) {
    auto [l, r] = split(root, k);
    return {WBT(l), WBT(r)};
  }
  std::tuple<WBT, WBT, WBT> split3(std::size_t a, std::size_t b) {
    auto [tmp, r] = split(root, b);
    auto [l, c] = split(tmp, a);
    return {WBT(l), WBT(c), WBT(r)};
  }
  void push_back(T val) { n[ni] = Node{val, 1}, root = merge(root, ni++); }
  void push_front(T val) { n[ni] = Node{val, 1}, root = merge(ni++, root); }
  void insert(std::size_t k, T val) {
    auto [l, r] = split(root, k);
    n[ni] = Node{val, 1}, root = merge(merge(l, ni++), r);
  }
  T pop_back() {
    assert(root);
    auto [l, t] = split(root, size() - 1);
    return root = l, n[t].val;
  }
  T pop_front() {
    assert(root);
    auto [t, r] = split(root, 1);
    return root = r, n[t].val;
  }
  T erase(std::size_t k) {
    assert(k < size());
    auto [l, tmp] = split(root, k);
    auto [t, r] = split(tmp, 1);
    return root = merge(l, r), n[t].val;
  }
  void set(std::size_t k, T val) { set_val(root, k, val); }
  T get(std::size_t k) { return get_val(root, k); }
  T &at(std::size_t k) {
    static_assert(!semigroup<M>::value, "\"at\" is not available\n");
    return at_val(root, k);
  }
  template <class L = M,
            typename std::enable_if_t<semigroup<L>::value> * = nullptr>
  T operator[](std::size_t k) {
    return get(k);
  }
  template <class L = M,
            typename std::enable_if_t<!semigroup<L>::value> * = nullptr>
  T &operator[](std::size_t k) {
    return at(k);
  }
  T fold(std::size_t a, std::size_t b) {
    static_assert(semigroup<M>::value, "\"fold\" is not available\n");
    return fold(root, a, b, 0, size());
  }
  void apply(std::size_t a, std::size_t b, E x) {
    static_assert(dual<M>::value, "\"apply\" is not available\n");
    apply(root, a, b, 0, size(), x);
  }
  std::size_t size() { return n[root].size; }
  std::vector<T> dump() {
    if (!root) return std::vector<T>();
    std::vector<T> ret(size());
    return dump(root, ret.begin()), ret;
  }
  void clear() { root = 0; }
  static void reset() { ni = 1; }
  void rebuild() {
    auto dmp = dump();
    reset(), *this = WBT(dmp);
  }
  static std::string which_available() {
    std::string ret = "";
    if constexpr (semigroup<M>::value)
      ret += "\"fold\" ";
    else
      ret += "\"at\" ";
    if constexpr (dual<M>::value) ret += "\"apply\" ";
    return ret;
  }
  static double percentage_used() { return 100. * ni / NODE_SIZE; }
};