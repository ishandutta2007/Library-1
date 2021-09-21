#pragma once
#include <bits/stdc++.h>
/**
 * @title Euler-Tour-Tree(モノイド遅延伝搬)
 * @category データ構造
 * @brief O(logN)
 */

// BEGIN CUT HERE

template <typename M>
class EulerTourTree_MonoidLazy {
  using T = typename M::T;
  using E = typename M::E;
  using node_id = std::int_least32_t;
  using vertex_id = std::int_least32_t;
  struct Node {
    vertex_id s, d;
    node_id ch[2], par;
    std::size_t sz;
    std::int_least8_t flag;
    T val, sum;
    E laz;
    Node(T init = M::ti()) : sz(1), val(init), sum(init), laz(M::ei()) {}
  };
  static constexpr int NODE_SIZE = 303030 * 4;
  static struct Node n[NODE_SIZE];
  static inline node_id ni = 1;
  node_id new_edge(int s, int d, bool hi) {
    int i = ni++, ri = ni++;
    return n[i].s = n[ri].d = s, n[i].d = n[ri].s = d, n[i].sz = n[ri].sz = 0,
           n[i].flag = hi, i;
  }
  static void pushup(node_id i) {
    n[i].sz = (n[i].s == n[i].d), n[i].flag &= 0b0101,
    n[i].flag |= n[i].flag << 1, n[i].sum = n[i].val;
    if (n[i].ch[0])
      n[i].sz += n[n[i].ch[0]].sz, n[i].flag |= n[n[i].ch[0]].flag & 0b1010,
          n[i].sum = M::op(n[n[i].ch[0]].sum, n[i].sum);
    if (n[i].ch[1])
      n[i].sz += n[n[i].ch[1]].sz, n[i].flag |= n[n[i].ch[1]].flag & 0b1010,
          n[i].sum = M::op(n[i].sum, n[n[i].ch[1]].sum);
  }
  static void propagate(node_id i, E v) {
    n[i].laz = M::composition(n[i].laz, v);
    n[i].val = M::mapping(n[i].val, v, n[i].s == n[i].d);
    n[i].sum = M::mapping(n[i].sum, v, n[i].sz);
  }
  static void eval(node_id i) {
    if (n[i].laz != M::ei()) {
      if (n[i].ch[0]) propagate(n[i].ch[0], n[i].laz);
      if (n[i].ch[1]) propagate(n[i].ch[1], n[i].laz);
      n[i].laz = M::ei();
    }
  }
  static int dir(node_id i) {
    if (n[i].par) {
      if (n[n[i].par].ch[0] == i)
        return 0;
      else if (n[n[i].par].ch[1] == i)
        return 1;
    }
    return 2;
  }
  static void rot(node_id x) {
    node_id p = n[x].par;
    int d = dir(x);
    if ((n[p].ch[d] = n[x].ch[!d])) n[n[p].ch[d]].par = p;
    n[x].ch[!d] = p, pushup(p), pushup(x), n[x].par = n[p].par;
    if ((d = dir(p)) < 2) n[n[p].par].ch[d] = x, pushup(n[p].par);
    n[p].par = x;
  }
  static void splay(node_id i) {
    eval(i);
    for (int i_dir = dir(i), p_dir; i_dir < 2; rot(i), i_dir = dir(i)) {
      if ((p_dir = dir(n[i].par)) < 2) eval(n[n[i].par].par);
      eval(n[i].par), eval(i);
      if (p_dir < 2) rot(i_dir == p_dir ? n[i].par : i);
    }
  }
  static node_id merge_back(node_id l, node_id r) {
    if (!l) return r;
    if (!r) return l;
    while (n[l].ch[1]) l = n[l].ch[1];
    return splay(l), n[n[r].par = l].ch[1] = r, pushup(l), l;
  }
  static std::pair<node_id, node_id> split(node_id i) {
    splay(i);
    node_id l = n[i].ch[0];
    return n[i].ch[0] = n[l].par = 0, pushup(i), std::make_pair(l, i);
  }

 public:
  static void reroot(node_id v) {
    auto p = split(v);
    merge_back(p.second, p.first), splay(v);
  }

 private:
  static bool same_root(node_id i, node_id j) {
    if (i) splay(i);
    if (j) splay(j);
    while (n[i].par) i = n[i].par;
    while (n[j].par) j = n[j].par;
    return i == j;
  }

 public:
  node_id n_st;
  std::unordered_map<std::uint64_t, node_id> emp;

 public:
  EulerTourTree_MonoidLazy() {}
  EulerTourTree_MonoidLazy(int N) : n_st(ni) {
    ni += N;
    for (int i = 0; i < N; i++) n[i + n_st].s = n[i + n_st].d = i;
  }
  T operator[](vertex_id x) const { return n[x + n_st].val; }
  bool edge_exist(vertex_id x, vertex_id y) {
    if (x > y) std::swap(x, y);
    return emp.count(((long long)x << 32) | (long long)y);
  }
  void link(vertex_id x, vertex_id y, bool hi = true) {
    if (x > y) std::swap(x, y);
    int ei = new_edge(x, y, hi);
    emp.insert(std::make_pair(((long long)x << 32) | (long long)y, ei));
    x += n_st, y += n_st, reroot(x), reroot(y);
    n[n[x].par = ei].ch[0] = x, n[n[y].par = ei].ch[1] = y;
    pushup(ei), merge_back(ei, ei + 1);
  }
  void cut(vertex_id x, vertex_id y) {
    if (x > y) std::swap(x, y);
    int ei = emp[((long long)x << 32) | (long long)y], rei = ei + 1;
    emp.erase(((long long)x << 32) | (long long)y);
    auto [pl, pr] = split(ei);
    node_id left, center, right;
    if (pl && same_root(pl, rei)) {
      auto [ql, qr] = split(rei);
      left = ql, center = n[qr].ch[1], right = n[pr].ch[1], n[center].par = 0;
    } else {
      splay(ei), n[ei = n[ei].ch[1]].par = 0;
      auto [ql, qr] = split(rei);
      splay(pl), left = pl, right = n[qr].ch[1];
    }
    n[right].par = 0, merge_back(left, right);
  }
  bool connected(vertex_id x, vertex_id y) {
    return same_root(x + n_st, y + n_st);
  }
  void subedge_set(vertex_id x, bool val) {
    splay(x += n_st);
    if (val)
      n[x].flag |= (0b0100);
    else
      n[x].flag &= ~(0b0100);
    pushup(x);
  }
  void set_val(vertex_id x, T val) {
    splay(x += n_st), n[x].val = val, eval(x);
  }
  void apply_tree(vertex_id x, E v) {
    splay(x += n_st), propagate(x, v), eval(x);
  }
  void apply_subtree(vertex_id x, vertex_id par, E v) {
    cut(x, par), apply_tree(x, v), link(x, par);
  }
  std::size_t tree_size(vertex_id x) { return splay(x += n_st), n[x].sz; }
  T fold_tree(vertex_id x) { return splay(x += n_st), n[x].sum; }
  T fold_subtree(vertex_id x, vertex_id par) {
    cut(x, par);
    T ret = fold_tree(x);
    link(x, par);
    return ret;
  }
  template <class Func>
  void hilevel_edges(vertex_id v, Func f) {
    splay(v += n_st);
    while (v && (n[v].flag & 0b0010)) {
      while (1) {
        if (n[v].flag & 0b0001) {
          f(n[v].s, n[v].d), splay(v), n[v].flag &= ~(0b0001), pushup(v);
          break;
        } else
          v = n[v].ch[!(n[v].ch[0] && (n[n[v].ch[0]].flag & 0b0010))];
      }
    }
  }
  template <class Func>
  int subedges(vertex_id v, Func f) {
    splay(v += n_st);
    while (v && (n[v].flag & 0b1000)) {
      for (bool loop = true; loop;) {
        if (n[v].flag & 0b0100) {
          if (f(n[v].s)) return 1;
          splay(v), loop = false;
        } else
          v = n[v].ch[!(n[v].ch[0] && (n[n[v].ch[0]].flag & 0b1000))];
      }
    }
    return 0;
  }
};
template <typename M>
typename EulerTourTree_MonoidLazy<M>::Node
    EulerTourTree_MonoidLazy<M>::n[NODE_SIZE];
