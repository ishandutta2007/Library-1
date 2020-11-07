#pragma once
#include <bits/stdc++.h>
/**
 * @title Li-Chao-Tree
 * @category データ構造
 * 直線挿入:O(n log n)
 * 線分挿入:O(n log^2 n)
 * 最小値取得:O(n log n)
 * 最大値取得したいなら(-a,-b)を挿入して-最小値を取得
 */

// BEGIN CUT HERE

template <typename T, T lower = -(1 << 30), T upper = (1 << 30),
          std::size_t LIM = (1 << 23)>
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
  static constexpr T INF = std::numeric_limits<T>::max() / 2;
  static inline int node_count;
  Node *root;

 private:
  Node *insert_line(Node *t, Line f, const T &x_l, const T &x_r) {
    if (!t) return new Node(f);
    T y_l = f.get(x_l), y_r = f.get(x_r), ty_l = t->f.get(x_l),
      ty_r = t->f.get(x_r);
    if (ty_l <= y_l && ty_r <= y_r) return t;
    if (ty_l >= y_l && ty_r >= y_r) return t->f = f, t;
    T x_m = x_l == x_r - 1 ? x_l : (x_l + x_r) / 2, ty_m = t->f.get(x_m),
      y_m = f.get(x_m);
    if (ty_m > y_m) std::swap(t->f, f), std::swap(y_l, ty_l);
    if (ty_l >= y_l) t->ch[0] = insert_line(t->ch[0], f, x_l, x_m);
    if (ty_l < y_l) t->ch[1] = insert_line(t->ch[1], f, x_m + 1, x_r);
    return t;
  }
  Node *insert_segment(Node *t, const Line &f, const T &l, const T &r,
                       const T &x_l, const T &x_r) {
    if (x_r < l || r < x_l) return t;
    if (l <= x_l && x_r <= r) return insert_line(t, f, x_l, x_r);
    T y_l = f.get(x_l), y_r = f.get(x_r);
    if (t && t->f.get(x_l) <= y_l && t->f.get(x_r) <= y_r) return t;
    if (!t) t = new Node(Line(0, INF));
    T x_m = x_l == x_r - 1 ? x_l : (x_l + x_r) / 2;
    t->ch[0] = insert_segment(t->ch[0], f, l, r, x_l, x_m);
    t->ch[1] = insert_segment(t->ch[1], f, l, r, x_m + 1, x_r);
    return t;
  }
  T query(const Node *t, const T &x_l, const T &x_r, const T &x) const {
    if (!t) return INF;
    if (x_l == x_r) return t->f.get(x);
    T x_m = x_l == x_r - 1 ? x_l : (x_l + x_r) / 2;
    return std::min(t->f.get(x), x <= x_m ? query(t->ch[0], x_l, x_m, x)
                                          : query(t->ch[1], x_m + 1, x_r, x));
  }

 public:
  LiChaoTree() : root{nullptr} {}
  T get_inf() { return INF; }
  void clear() {
    node_count = 0;
    root = nullptr;
  }
  // ax+b
  void insert_line(T a, T b) {
    root = insert_line(root, Line(a, b), lower, upper);
  }
  // ax+b for x in [l,r)
  void insert_segment(T l, T r, T a, T b) {
    root = insert_segment(root, Line(a, b), l, r - 1, lower, upper);
  }
  T query(T x) const { return query(root, lower, upper, x); }
};