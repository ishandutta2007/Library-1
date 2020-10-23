#pragma once
#include <bits/stdc++.h>
/**
 * @title 赤黒木(遅延伝搬)
 * @category データ構造
 * @brief O(logN)
 */

// BEGIN CUT HERE

template <typename M, size_t LIM = 1 << 20>
struct RedBlackTree_Lazy {
  using T = typename M::T;
  using E = typename M::E;
  using RBTL = RedBlackTree_Lazy;

 public:
  enum COLOR { BLACK, RED };
  struct Node {
    Node *ch[2];
    COLOR color;
    int level, cnt;
    T val, dat;
    E lazy;
    Node() {}
    Node(const T &k)
        : val(k),
          dat(k),
          ch{nullptr, nullptr},
          color(BLACK),
          level(0),
          cnt(1),
          lazy(M::ei()) {}
    Node(Node *l, Node *r, const T &k)
        : val(k), color(RED), ch{l, r}, lazy(M::ei()) {}
  };
  struct nPool {
    std::vector<Node> pool;
    std::vector<Node *> stock;
    int ptr;
    nPool(int sz) : pool(sz), stock(sz) { clear(); }
    inline Node *alloc() { return stock[--ptr]; }
    inline void free(Node *t) { stock[ptr++] = t; }
    void clear() {
      ptr = (int)pool.size();
      for (int i = 0; i < pool.size(); i++) stock[i] = &pool[i];
    }
  };

 protected:
  inline Node *alloc(Node *l, Node *r) {
    auto t = &(*pool.alloc() = Node(l, r, M::ti()));
    return pushup(t);
  }
  virtual Node *clone(Node *t) { return t; }
  Node *propagate(Node *t) {
    t = clone(t);
    if (t->lazy != M::ei()) {
      if (!t->ch[0]) {
        t->val = M::g(t->val, t->lazy);
      } else {
        for (char b = 0; b < 2; b++)
          if (t->ch[b]) {
            t->ch[b] = clone(t->ch[b]);
            t->ch[b]->lazy = M::h(t->ch[b]->lazy, t->lazy);
            t->ch[b]->dat = M::g(t->ch[b]->dat, t->lazy);
          }
      }
      t->lazy = M::ei();
    }
    return pushup(t);
  }
  inline Node *rotate(Node *t, bool b) {
    Node *s;
    s = propagate(t->ch[!b]);
    t->ch[!b] = s->ch[b];
    s->ch[b] = t;
    pushup(t);
    return pushup(s);
  }
  Node *submerge(Node *l, Node *r) {
    if (l->level == r->level) return alloc(l, r);
    bool b = l->level < r->level;
    if (!b) swap(l, r);
    r = propagate(r);
    Node *c = (r->ch[!b] = b ? submerge(l, r->ch[0]) : submerge(r->ch[1], l));
    if (r->color == BLACK && c->color == RED && c->ch[!b]
        && c->ch[!b]->color == RED) {
      r->color = RED;
      c->color = BLACK;
      if (r->ch[b]->color == BLACK) return rotate(r, b);
      r->ch[b]->color = BLACK;
    }
    return pushup(r);
  }
  Node *build(int l, int r, const std::vector<T> &v) {
    if (l + 1 >= r) return alloc(v[l]);
    return merge(build(l, (l + r) >> 1, v), build((l + r) >> 1, r, v));
  }
  Node *pushup(Node *t) {
    t->cnt = count(t->ch[0]) + count(t->ch[1]) + !t->ch[0];
    t->level = t->ch[0] ? t->ch[0]->level + (t->ch[0]->color == BLACK) : 0;
    t->dat = M::f(M::f(dat(t->ch[0]), t->val), dat(t->ch[1]));
    return t;
  }
  void dump(Node *r, typename std::vector<T>::iterator &it, E lazy) {
    if (r->lazy != M::ei()) lazy = M::h(lazy, r->lazy);
    if (r->ch[0] == nullptr) {
      *it++ = M::g(r->val, lazy);
      return;
    }
    dump(r->ch[0], it, lazy);
    dump(r->ch[1], it, lazy);
  }
  inline Node *alloc(const T &val) { return &(*pool.alloc() = Node(val)); }
  inline int count(const Node *t) { return t ? t->cnt : 0; }
  inline const T dat(const Node *t) { return t ? t->dat : M::ti(); }
  std::pair<Node *, Node *> split(Node *t, int k) {
    if (!t) return {nullptr, nullptr};
    t = propagate(t);
    if (k == 0) return {nullptr, t};
    if (k >= count(t)) return {t, nullptr};
    Node *l = t->ch[0], *r = t->ch[1];
    pool.free(t);
    if (k < count(l)) {
      auto pp = split(l, k);
      return {pp.first, merge(pp.second, r)};
    }
    if (k > count(l)) {
      auto pp = split(r, k - count(l));
      return {merge(l, pp.first), pp.second};
    }
    return {l, r};
  }
  Node *merge(Node *l, Node *r) {
    if (!l || !r) return l ? l : r;
    Node *c = submerge(l, r);
    c->color = BLACK;
    return c;
  }
  void set_val(Node *&t, int k, const T &x) {
    if (!t->ch[0]) {
      t = clone(t);
      t->val = t->dat = x;
      return;
    }
    t = propagate(t);
    if (k < count(t->ch[0]))
      set_val(t->ch[0], k, x);
    else
      set_val(t->ch[1], k - count(t->ch[0]), x);
    t = pushup(t);
  }
  T get(Node *&t, int k) {
    if (!t->ch[0]) return t->val;
    return k < count(t->ch[0]) ? get(t->ch[0], k)
                               : get(t->ch[1], k - count(t->ch[0]));
  }
  RedBlackTree_Lazy(Node *t) : root(t) {}

 protected:
  static nPool pool;
  Node *root;

 public:
  RedBlackTree_Lazy() : root(nullptr) {}
  RedBlackTree_Lazy(int n, T val = M::ti())
      : RedBlackTree_Lazy(std::vector<T>(n, val)) {}
  RedBlackTree_Lazy(const std::vector<T> &v) {
    root = build(0, (int)v.size(), v);
  }
  void build(const std::vector<T> &v) { root = build(0, (int)v.size(), v); }
  void push_front(const T &v) { root = merge(alloc(v), root); }
  void push_back(const T &v) { root = merge(root, alloc(v)); }
  T pop_front() {
    auto ret = split(root, 1);
    root = ret.second;
    return ret.first->val;
  }
  T pop_back() {
    auto ret = split(root, count(root) - 1);
    root = ret.first;
    return ret.second->val;
  }
  // merge
  RBTL operator+(const RBTL &r) {
    if (!this->root || !r.root) return this->root ? *this : r;
    Node *c = submerge(this->root, r.root);
    c->color = BLACK;
    return RBTL(c);
  }
  // [0,k) [k,size)
  std::pair<RBTL, RBTL> split(int k) {
    auto tmp = split(root, k);
    return std::make_pair(RBTL(tmp.first), RBTL(tmp.first));
  }
  // [0,a) [a,b) [b,size)
  std::tuple<RBTL, RBTL, RBTL> split3(int a, int b) {
    auto x = split(root, a);
    auto y = split(x.second, b - a);
    return std::make_tuple(RBTL(x.first), RBTL(y.first), RBTL(y.second));
  }
  void insert(int k, const T &v) {
    auto x = split(root, k);
    root = merge(merge(x.first, alloc(v)), x.second);
  }
  T erase(int k) {
    auto x = split(root, k);
    auto y = split(x.second, 1);
    T v = y.first->val;
    pool.free(y.first);
    root = merge(x.first, y.second);
    return v;
  }
  //[a,b)
  T query(int a, int b) {
    auto x = split(root, a);
    auto y = split(x.second, b - a);
    T ret = dat(y.first);
    root = merge(x.first, merge(y.first, y.second));
    return ret;
  }
  T operator[](int k) { return get(root, k); }
  void set_val(int k, const T &x) { set_val(root, k, x); }
  //[a,b)
  void update(int a, int b, E v) {
    auto x = split(root, a);
    auto y = split(x.second, b - a);
    y.first->lazy = M::h(y.first->lazy, v);
    root = merge(x.first, merge(propagate(y.first), y.second));
  }
  std::vector<T> dump() {
    std::vector<T> v((size_t)count(root));
    auto it = begin(v);
    dump(root, it, M::ei());
    return v;
  }
  int size() { return count(root); }
};
template <typename M, size_t LIM>
typename RedBlackTree_Lazy<M, LIM>::nPool RedBlackTree_Lazy<M, LIM>::pool(LIM);