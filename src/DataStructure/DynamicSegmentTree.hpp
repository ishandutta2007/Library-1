#pragma once
#include <bits/stdc++.h>
/**
 * @title 動的構築Segment-Tree
 * @category データ構造
 * 永続化可
 * パトリシア木オプションでメモリ節約できるかも（スパースなら効果大）
 * O(logN)
 */

// verify用:
// https://codeforces.com/contest/464/problem/E (永続+遅延伝搬+find*2)
// https://codeforces.com/contest/947/problem/C (find+xor)
// https://codeforces.com/contest/966/problem/C (find+xor)
// https://codeforces.com/contest/295/problem/E (特殊モノイド+座圧サボり)

// BEGIN CUT HERE

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

template <typename M, bool persistent = false, std::uint8_t HEIGHT = 30,
          bool patricia = false>
class DynamicSegmentTree {
  HAS_MEMBER(op);
  HAS_MEMBER(ti);
  HAS_MEMBER(mapping);
  HAS_MEMBER(composition)
  HAS_TYPE(T);
  HAS_TYPE(E);
  template <class L>
  using monoid = std::conjunction<has_T<L>, has_op<L>, has_ti<L>>;
  template <class L>
  using dual =
      std::conjunction<has_T<L>, has_E<L>, has_mapping<L>, has_composition<L>>;
  using id_t = long long;
  template <class T, class tDerived, class F = std::nullptr_t>
  struct Node_B {
    using E = F;
    T val;
    tDerived *ch[2] = {nullptr, nullptr};
    Node_B(id_t b = 0, std::uint8_t l = 0, T v = def_val()) : val(v) {}
  };
  template <class T, class tDerived, class F = std::nullptr_t>
  struct Node_P : public Node_B<T, tDerived, F> {
    id_t bits;
    std::uint8_t len;
    Node_P(id_t b = 0, std::uint8_t l = 0, T v = def_val())
        : Node_B<T, tDerived, F>(b, l, v), bits(b), len(l) {}
  };
  template <bool mo_, bool du_, bool pa_, typename tEnable = void>
  struct Node_D : Node_B<M, Node_D<mo_, du_, pa_, tEnable>> {
    using Node_B<M, Node_D<mo_, du_, pa_, tEnable>>::Node_B;
  };
  template <bool mo_, bool du_, bool pa_>
  struct Node_D<mo_, du_, pa_, typename std::enable_if_t<mo_ && !du_ && !pa_>>
      : Node_B<typename M::T, Node_D<mo_, du_, pa_>> {
    using Node_B<typename M::T, Node_D<mo_, du_, pa_>>::Node_B;
  };
  template <bool mo_, bool du_, bool pa_>
  struct Node_D<mo_, du_, pa_, typename std::enable_if_t<du_ && !pa_>>
      : Node_B<typename M::T, Node_D<mo_, du_, pa_>, typename M::E> {
    typename M::E lazy;
    bool lazy_flg = false;
    using Node_B<typename M::T, Node_D<mo_, du_, pa_>, typename M::E>::Node_B;
  };
  template <bool mo_, bool du_, bool pa_>
  struct Node_D<mo_, du_, pa_, typename std::enable_if_t<!mo_ && !du_ && pa_>>
      : Node_P<M, Node_D<mo_, du_, pa_>> {
    using Node_P<M, Node_D<mo_, du_, pa_>>::Node_P;
  };
  template <bool mo_, bool du_, bool pa_>
  struct Node_D<mo_, du_, pa_, typename std::enable_if_t<mo_ && !du_ && pa_>>
      : Node_P<typename M::T, Node_D<mo_, du_, pa_>> {
    using Node_P<typename M::T, Node_D<mo_, du_, pa_>>::Node_P;
  };
  template <bool mo_, bool du_, bool pa_>
  struct Node_D<mo_, du_, pa_, typename std::enable_if_t<du_ && pa_>>
      : Node_P<typename M::T, Node_D<mo_, du_, pa_>, typename M::E> {
    typename M::E lazy;
    bool lazy_flg = false;
    using Node_P<typename M::T, Node_D<mo_, du_, pa_>, typename M::E>::Node_P;
  };
  using Node = Node_D<monoid<M>::value, dual<M>::value, patricia>;
  using T = decltype(Node::val);
  using E = typename Node::E;
  Node *root;
  static inline constexpr T def_val() {
    if constexpr (monoid<M>::value)
      return M::ti();
    else
      return T();
  }
  template <class S>
  void build(Node *&t, const id_t &n, std::array<id_t, 2> b, const S &bg) {
    if (n <= b[0]) return;
    id_t m = (b[0] + b[1]) >> 1;
    if constexpr (patricia)
      while (n <= m) b[1] = m, m = (b[0] + b[1]) >> 1;
    if (b[1] - b[0] == 1) {
      if constexpr (std::is_same_v<S, T>)
        t = new Node(b[0], HEIGHT + 1, bg);
      else
        t = new Node(b[0], HEIGHT + 1, *(bg + b[0]));
    } else {
      std::uint8_t h = __builtin_ctzll(b[1] - b[0]);
      t = new Node(m >> h, std::uint8_t(HEIGHT + 1 - h));
      build(t->ch[0], n, {b[0], m}, bg), build(t->ch[1], n, {m, b[1]}, bg);
      if constexpr (monoid<M>::value) pushup(t);
    }
  }
  void dump(Node *t, const id_t &l, const id_t &r, std::array<id_t, 2> b,
            typename std::vector<T>::iterator itr, std::uint8_t h) {
    if (r <= b[0] || b[1] <= l) return;
    if (l <= b[0] && b[1] <= r && !t) {
      for (id_t i = b[0]; i < b[1]; i++) *(itr + i) = def_val();
    } else if (b[1] - b[0] != 1) {
      if constexpr (dual<M>::value) eval(t, b[1] - b[0]);
      auto m = (b[0] + b[1]) >> 1;
      dump(next(t, h, 0), l, r, {b[0], m}, itr, h - 1);
      dump(next(t, h, 1), l, r, {m, b[1]}, itr, h - 1);
    } else
      *(itr + b[0]) = t->val;
  }
  static inline void pushup(Node *&t) {
    t->val = def_val();
    if (t->ch[0]) t->val = M::op(t->ch[0]->val, t->val);
    if (t->ch[1]) t->val = M::op(t->val, t->ch[1]->val);
  }
  static inline void separate(Node *&t, const std::uint8_t &len) {
    if (len <= 1) return;
    bool flg = (t->bits >> (len - 2)) & 1;
    t->ch[flg] = new Node{*t}, t->ch[!flg] = nullptr;
    t->len -= len - 1, t->bits >>= len - 1;
  }
  static inline void propagate(Node *&t, const E &x, const id_t &sz) {
    t->lazy = t->lazy_flg ? M::composition(t->lazy, x) : x;
    t->val = M::mapping(t->val, x, sz);
    t->lazy_flg = true;
  }
  template <bool r>
  static inline void cp_eval(Node *&t) {
    if (!t->ch[r]) {
      if constexpr (patricia)
        t->ch[r] = new Node((t->bits << 1) | r, std::uint8_t(t->len + 1));
      else
        t->ch[r] = new Node();
    } else if constexpr (persistent)
      t->ch[r] = new Node(*t->ch[r]);
  }
  static inline void eval(Node *&t, const id_t &sz) {
    if (!t->lazy_flg) return;
    cp_eval<0>(t), cp_eval<1>(t);
    if constexpr (patricia)
      separate(t->ch[0], t->ch[0]->len - t->len),
          separate(t->ch[1], t->ch[1]->len - t->len);
    propagate(t->ch[0], t->lazy, sz / 2), propagate(t->ch[1], t->lazy, sz / 2);
    t->lazy_flg = false;
  }
  T fold(Node *&t, const id_t &l, const id_t &r, std::array<id_t, 2> b,
         const id_t &bias) {
    if (!t || r <= b[0] || b[1] <= l) return def_val();
    if (l <= b[0] && b[1] <= r) return t->val;
    if constexpr (dual<M>::value) eval(t, b[1] - b[0]);
    id_t m = (b[0] + b[1]) >> 1;
    bool flg = (bias >> (__builtin_ctzll(b[1] - b[0]) - 1)) & 1;
    return M::op(fold(t->ch[flg], l, r, {b[0], m}, bias),
                 fold(t->ch[!flg], l, r, {m, b[1]}, bias));
  }
  T fold(Node *&t, const id_t &l, const id_t &r, const id_t &bias) {
    static id_t bits, b[2];
    if (!t) return def_val();
    std::uint8_t h = (HEIGHT + 1) - t->len;
    bits = (bias >> h) ^ t->bits, b[0] = bits << h, b[1] = (bits + 1) << h;
    if (r <= b[0] || b[1] <= l) return def_val();
    if (l <= b[0] && b[1] <= r) return t->val;
    if constexpr (dual<M>::value) eval(t, b[1] - b[0]);
    bool flg = (bias >> (h - 1)) & 1;
    return M::op(fold(t->ch[flg], l, r, bias), fold(t->ch[!flg], l, r, bias));
  }
  void apply(Node *&t, const id_t &l, const id_t &r, std::array<id_t, 2> b,
             const E &x) {
    if (r <= b[0] || b[1] <= l) return;
    std::uint8_t h = __builtin_ctzll(b[1] - b[0]);
    id_t m = (b[0] + b[1]) >> 1;
    if (!t) {
      t = new Node(m >> h, std::uint8_t(HEIGHT + 1 - h));
    } else if constexpr (persistent)
      t = new Node{*t};
    if constexpr (patricia) separate(t, h + t->len - HEIGHT);
    if (l <= b[0] && b[1] <= r) return propagate(t, x, b[1] - b[0]), void();
    eval(t, b[1] - b[0]);
    apply(t->ch[0], l, r, {b[0], m}, x), apply(t->ch[1], l, r, {m, b[1]}, x);
    if constexpr (monoid<M>::value) pushup(t);
  }
  void set_val(Node *&t, const id_t &k, const T &val, std::uint8_t h) {
    if (!t)
      t = new Node(0, 0, val);
    else if constexpr (persistent)
      t = new Node{*t};
    if (!h) return t->val = val, void();
    if constexpr (dual<M>::value) eval(t, 1LL << h);
    set_val(t->ch[(k >> (h - 1)) & 1], k, val, h - 1);
    if constexpr (monoid<M>::value) pushup(t);
  }
  void set_val(Node *&t, const id_t &k, const T &val) {
    if (!t) return t = new Node(k, HEIGHT + 1, val), void();
    if constexpr (persistent) t = new Node{*t};
    id_t bits = (k >> ((HEIGHT + 1) - t->len));
    if (bits != t->bits) {
      std::uint8_t i = 64 - __builtin_clzll(bits ^ t->bits);
      bool flg = (t->bits >> (i - 1)) & 1;
      t->ch[flg] = new Node{*t}, t->ch[!flg] = new Node(k, HEIGHT + 1, val);
      t->len -= i, t->bits >>= i;
    } else if (t->len != HEIGHT + 1) {
      if constexpr (dual<M>::value) eval(t, HEIGHT + 1 - t->len);
      set_val(t->ch[(k >> (HEIGHT - t->len)) & 1], k, val);
    } else
      return t->val = val, void();
    if constexpr (monoid<M>::value) pushup(t);
  }
  T &at_val(Node *&t, const id_t &k, std::uint8_t h) {
    if (!t)
      t = new Node();
    else if constexpr (persistent)
      t = new Node{*t};
    if (!h) return t->val;
    if constexpr (dual<M>::value) eval(t, 1LL << h);
    return at_val(t->ch[(k >> (h - 1)) & 1], k, h - 1);
  }
  T &at_val(Node *&t, const id_t &k) {
    if (!t) return t = new Node(k, HEIGHT + 1), t->val;
    if constexpr (persistent) t = new Node{*t};
    id_t bits = (k >> ((HEIGHT + 1) - t->len));
    if (bits != t->bits) {
      std::uint8_t i = 64 - __builtin_clzll(bits ^ t->bits);
      bool flg = (t->bits >> (i - 1)) & 1;
      t->ch[flg] = new Node{*t}, t->ch[!flg] = new Node(k, HEIGHT + 1);
      t->len -= i, t->bits >>= i;
      return t->val;
    } else if (t->len != HEIGHT + 1) {
      if constexpr (dual<M>::value) eval(t, HEIGHT + 1 - t->len);
      return at_val(t->ch[(k >> (HEIGHT - t->len)) & 1], k);
    }
    return t->val;
  }
  bool is_null(Node *&t, const id_t &k, std::uint8_t h) {
    if (!t) return true;
    if (!h) return false;
    if constexpr (dual<M>::value) eval(t, 1LL << h);
    return is_null(t->ch[(k >> (h - 1)) & 1], k, h - 1);
  }
  bool is_null(Node *&t, const id_t &k) {
    if (!t || (k >> ((HEIGHT + 1) - t->len)) != t->bits) return true;
    if (t->len == HEIGHT + 1) return false;
    if constexpr (dual<M>::value) eval(t, HEIGHT + 1 - t->len);
    return is_null(t->ch[(k >> (HEIGHT - t->len)) & 1], k);
  }
  T get_val(Node *&t, const id_t &k, std::uint8_t h) {
    if (!t) return def_val();
    if (!h) return t->val;
    if constexpr (dual<M>::value) eval(t, 1LL << h);
    return get_val(t->ch[(k >> (h - 1)) & 1], k, h - 1);
  }
  T get_val(Node *&t, const id_t &k) {
    if (!t || (k >> ((HEIGHT + 1) - t->len)) != t->bits) return def_val();
    if (t->len == HEIGHT + 1) return t->val;
    if constexpr (dual<M>::value) eval(t, HEIGHT + 1 - t->len);
    return get_val(t->ch[(k >> (HEIGHT - t->len)) & 1], k);
  }
  template <bool last>
  static inline T calc_op(Node *&t, const T &v) {
    if constexpr (last)
      return M::op((t ? t->val : def_val()), v);
    else
      return M::op(v, (t ? t->val : def_val()));
  }
  template <bool last>
  static inline bool is_in(const id_t &m, const id_t &k) {
    if constexpr (last)
      return k <= m;
    else
      return m <= k;
  }
  static inline Node *next(Node *&t, const std::uint8_t &h, const bool &f) {
    if constexpr (patricia) {
      if (!t) return nullptr;
      std::uint8_t len = h + t->len - (HEIGHT + 1);
      if (!len) return t->ch[f];
      return (f == ((t->bits >> (len - 1)) & 1)) ? t : nullptr;
    } else
      return t ? t->ch[f] : nullptr;
  }
  template <bool last, class C, std::size_t N>
  static id_t find(const id_t &k, std::array<id_t, 2> b, const id_t &bias,
                   std::uint8_t h, const C &check, std::array<Node *, N> &ts,
                   std::array<T, N> &sums) {
    static_assert(monoid<M>::value, "\"find\" is not available\n");
    static std::array<T, N> sums2;
    if (std::all_of(ts.begin(), ts.end(), [](Node *t) { return !t; }))
      return -1;
    if (!h) {
      for (std::size_t i = N; i--;) sums[i] = calc_op<last>(ts[i], sums[i]);
      return std::apply(check, sums) ? std::get<last>(b) : -1;
    } else if (is_in<last>(k, b[0])) {
      for (std::size_t i = N; i--;) sums2[i] = calc_op<last>(ts[i], sums[i]);
      if (!std::apply(check, sums2)) return sums = std::move(sums2), -1;
    }
    if constexpr (dual<M>::value)
      for (std::size_t i = N; i--;) eval(ts[i], b[1] - b[0]);
    std::array<Node *, N> ss;
    id_t m = (b[0] + b[1]) >> 1;
    bool flg = (bias >> (h - 1)) & 1;
    if (!is_in<last>(m, k)) {
      for (std::size_t i = N; i--;) ss[i] = next(ts[i], h, flg);
      id_t ret = find<last>(k, {b[0], m}, bias, h - 1, check, ss, sums);
      if (ret >= 0) return ret;
    }
    for (std::size_t i = N; i--;) ss[i] = next(ts[i], h, !flg);
    return find<last>(k, {m, b[1]}, bias, h - 1, check, ss, sums);
  }

 public:
  DynamicSegmentTree(Node *t = nullptr) : root(t) {}
  DynamicSegmentTree(std::size_t n, T val) {
    build(root, n, {0, 1LL << HEIGHT}, val);
  }
  DynamicSegmentTree(T *bg, T *ed) {
    build(root, ed - bg, {0, 1LL << HEIGHT}, bg);
  }
  DynamicSegmentTree(const std::vector<T> &ar) {
    build(root, ar.size(), {0, 1LL << HEIGHT}, ar.data());
  }
  void set(id_t k, T val) {
    if constexpr (patricia)
      set_val(root, k, val);
    else
      set_val(root, k, val, HEIGHT);
  }
  T get(id_t k) {
    if constexpr (patricia)
      return get_val(root, k);
    else
      return get_val(root, k, HEIGHT);
  }
  bool is_null(id_t k) {
    if constexpr (patricia)
      return is_null(root, k);
    else
      return is_null(root, k, HEIGHT);
  }
  T &at(id_t k) {
    static_assert(!monoid<M>::value, "\"at\" is not available\n");
    if constexpr (patricia)
      return at_val(root, k);
    else
      return at_val(root, k, HEIGHT);
  }
  template <class L = M,
            typename std::enable_if_t<monoid<L>::value> * = nullptr>
  T operator[](id_t k) {
    return get(k);
  }
  template <class L = M,
            typename std::enable_if_t<!monoid<L>::value> * = nullptr>
  T &operator[](id_t k) {
    return at(k);
  }
  T fold(id_t a, id_t b, id_t bias = 0) {
    static_assert(monoid<M>::value, "\"fold\" is not available\n");
    if constexpr (patricia)
      return fold(root, a, b, bias);
    else
      return fold(root, a, b, {0, 1LL << HEIGHT}, bias);
  }
  // find i s.t.
  //  check(fold(k,i)) == False, check(fold(k,i+1)) == True
  // return -1 if not found
  template <class C>
  id_t find_first(id_t a, C check, id_t bias = 0) {
    std::array<T, 1> sum{def_val()};
    std::array<Node *, 1> t{root};
    return find<0>(a, {0, 1LL << HEIGHT}, bias, HEIGHT, check, t, sum);
  }
  template <std::size_t N, class C>
  static id_t find_first(id_t a, C check,
                         std::array<DynamicSegmentTree, N> segs,
                         id_t bias = 0) {
    std::array<T, N> sums;
    sums.fill(def_val());
    std::array<Node *, N> ts;
    for (std::size_t i = 0; i < N; i++) ts[i] = segs[i].root;
    return find<0>(a, {0, 1LL << HEIGHT}, bias, HEIGHT, check, ts, sums);
  }
  // find i s.t.
  //  check(fold(i+1,k)) == False, check(fold(i,k)) == True
  // return -1 if not found
  template <class C>
  id_t find_last(id_t b, C check, id_t bias = 0) {
    std::array<T, 1> sum{def_val()};
    std::array<Node *, 1> t{root};
    return find<1>(b, {1LL << HEIGHT, 0}, ~bias, HEIGHT, check, t, sum);
  }
  template <std::size_t N, class C>
  static id_t find_last(id_t b, C check, std::array<DynamicSegmentTree, N> segs,
                        id_t bias = 0) {
    std::array<T, N> sums;
    sums.fill(def_val());
    std::array<Node *, N> ts;
    for (std::size_t i = 0; i < N; i++) ts[i] = segs[i].root;
    return find<1>(b, {1LL << HEIGHT, 0}, ~bias, HEIGHT, check, ts, sums);
  }
  void apply(id_t a, id_t b, E x) {
    static_assert(dual<M>::value, "\"apply\" is not available\n");
    apply(root, a, b, {0, 1LL << HEIGHT}, x);
  }
  std::vector<T> dump(id_t bg, id_t ed) {
    std::vector<T> ret(ed - bg);
    dump(root, bg, ed, {0, 1LL << HEIGHT}, ret.begin(), HEIGHT);
    return ret;
  }
  static std::string which_available() {
    std::string ret = "";
    if constexpr (monoid<M>::value)
      ret += "\"fold\" \"find\"";
    else
      ret += "\"at\" ";
    if constexpr (dual<M>::value) ret += "\"apply\" ";
    return ret;
  }
};
