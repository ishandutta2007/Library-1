#pragma once
#include <bits/stdc++.h>
/**
 * @title Li-Chao-Tree
 * @category データ構造
 * 直線挿入:O(log n)
 * 線分挿入:O(log^2 n)
 * 最小値取得:O(log n)
 * 最大値取得したいなら(-a,-b)を挿入して-最小値を取得
 * 浮動小数点も行けるはず
 */
// double型での使用 : https://atcoder.jp/contests/arc051/tasks/arc051_d
// BEGIN CUT HERE

template <typename T, std::size_t LIM = (1 << 23)>
struct LiChaoTree {
  struct Line {
    T a, b;
    Line(T a, T b) : a(a), b(b) {}
    inline T get(T x) const { return a * x + b; }
  };
  struct Node {
    Line f;
    Node *ch[2];
    Node() : f(0, INF) {}
    Node(const Line &f_) : f(f_), ch{nullptr, nullptr} {}
    void *operator new(size_t) {
      static std::vector<Node> pool(LIM);
      return &pool[node_count++];
    }
  };

 private:
  static constexpr T lower = -2e9, upper = 2e9;
  static constexpr T INF = std::numeric_limits<T>::max() / 2;
  static inline int node_count;
  Node *root;

 private:
  int sgn(const T &x) const {
    static constexpr T EPS = 1e-10;
    return x < -EPS ? -1 : x > +EPS ? 1 : 0;
  }
  Node *addl(Node *t, Line f, const T &x_l, const T &x_r) {
    if (!t) return new Node(f);
    int dif_l = sgn(t->f.get(x_l) - f.get(x_l)),
        dif_r = sgn(t->f.get(x_r) - f.get(x_r));
    if (dif_l <= 0 && dif_r <= 0) return t;
    if (dif_l >= 0 && dif_r >= 0) return t->f = f, t;
    T x_m = (x_l + x_r) / 2;
    int dif_m = sgn(t->f.get(x_m) - f.get(x_m));
    if (dif_m > 0) std::swap(t->f, f), dif_l = -dif_l;
    if (sgn(x_l - x_m) == 0) return t;
    if (dif_l > 0) t->ch[0] = addl(t->ch[0], f, x_l, x_m);
    if (dif_l < 0) t->ch[1] = addl(t->ch[1], f, x_m, x_r);
    return t;
  }
  Node *adds(Node *t, const Line &f, const T &l, const T &r, const T &x_l,
             const T &x_r) {
    if (sgn(x_r - l) <= 0 || 0 <= sgn(x_l - r)) return t;
    if (0 <= sgn(x_l - l) && sgn(x_r - r) <= 0) return addl(t, f, x_l, x_r);
    if (t && sgn(t->f.get(x_l) - f.get(x_l)) <= 0
        && sgn(t->f.get(x_r) - f.get(x_r)) <= 0)
      return t;
    if (!t) t = new Node(Line(0, INF));
    T x_m = (x_l + x_r) / 2;
    t->ch[0] = adds(t->ch[0], f, l, r, x_l, x_m);
    t->ch[1] = adds(t->ch[1], f, l, r, x_m, x_r);
    return t;
  }
  T query(const Node *t, const T &x_l, const T &x_r, const T &x) const {
    if (!t) return INF;
    if (sgn(x_l - x_r) == 0) return t->f.get(x);
    T x_m = (x_l + x_r) / 2;
    return std::min(t->f.get(x), sgn(x - x_m) < 0
                                     ? query(t->ch[0], x_l, x_m, x)
                                     : query(t->ch[1], x_m, x_r, x));
  }

 public:
  LiChaoTree() : root{nullptr} {}
  T get_inf() { return INF; }
  void clear() { node_count = 0, root = nullptr; }
  // ax+b
  void insert_line(T a, T b) { root = addl(root, Line(a, b), lower, upper); }
  // ax+b for x in [l,r)
  void insert_segment(T l, T r, T a, T b) {
    root = adds(root, Line(a, b), l, r, lower, upper);
  }
  T query(T x) const { return query(root, lower, upper, x); }
};
