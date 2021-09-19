#pragma once
#include <bits/stdc++.h>
/**
 * @title Segment-Tree(動的)
 * @category データ構造
 * O(logN)
 */

// BEGIN CUT HERE

template <typename M, std::size_t LIM = 1 << 23>
struct SegmentTree_Dynamic {
  using T = typename M::T;
  using ll = long long;
  using U = unsigned long long;
  struct Node {
    T dat;
    U xor_lazy;
    Node *ch[2];
    Node() : dat(M::ti()), xor_lazy(0), ch{nullptr, nullptr} {}
    void *operator new(std::size_t) {
      static std::vector<Node> pool(LIM);
      return &pool[node_count++];
    }
  };

 protected:
  int height;
  ll n;
  static int node_count;
  Node *root;

 private:
  inline void push(Node *t, ll b) {
    if ((t->xor_lazy >> (U)b) & (U)1) std::swap(t->ch[0], t->ch[1]);
    if (t->ch[0] != nullptr) t->ch[0]->xor_lazy ^= t->xor_lazy;
    if (t->ch[1] != nullptr) t->ch[1]->xor_lazy ^= t->xor_lazy;
    t->xor_lazy = 0;
  }
  T value(Node *t) { return t ? t->dat : M::ti(); }
  Node *set_Node(Node *t, const U &pos, const T &val, ll b) {
    if (t == nullptr) t = new Node();
    if (b < 0) {
      t->dat = val;
      return t;
    }
    push(t, b);
    bool f = (pos >> (U)b) & (U)1;
    t->ch[f] = set_Node(t->ch[f], pos, val, b - 1);
    t->dat = M::op(value(t->ch[0]), value(t->ch[1]));
    return t;
  }
  T fold_Node(const ll &l, const ll &r, Node *t, const ll &lb, const ll &ub,
               ll b) {
    if (t == nullptr || ub <= l || r <= lb) return M::ti();
    push(t, b);
    if (l <= lb && ub <= r) return t->dat;
    ll c = (lb + ub) / 2;
    T vl = fold_Node(l, r, t->ch[0], lb, c, b - 1);
    T vr = fold_Node(l, r, t->ch[1], c, ub, b - 1);
    return M::op(vl, vr);
  }
  template <typename C>
  ll find(Node *t, C &check, U bias) {
    if (!t) return -1;
    ll ret = 0;
    T acc = M::ti();
    for (ll b = height - 1; b >= 0; b--) {
      push(t, b);
      bool f = (bias >> (U)b) & (U)1;
      if (!check(M::op(acc, value(t->ch[f]))))
        acc = M::op(acc, value(t->ch[f])), f = !f;
      t = t->ch[f];
      if (!t) return -1;
      ret |= (U)f << (U)b;
    }
    return ret;
  }

 public:
  SegmentTree_Dynamic() {}
  SegmentTree_Dynamic(ll n_)
      : height(ceil(log2(n_))), n(1LL << height), root(nullptr) {}
  void clear() {
    node_count = 0;
    root = nullptr;
  }
  void xor_all(U key) {
    if (root != nullptr) root->xor_lazy ^= key;
  }
  void set_val(ll k, T x) { root = set_Node(root, k, x, height - 1); }
  //[l,r)
  T fold(ll l, ll r) { return fold_Node(l, r, root, 0, n, height - 1); }
  T operator[](ll k) { return fold(k, k + 1); }
  // min { i : check(fold(0,i+1)) = true }
  template <typename C>
  ll find_first(C &check, U bias = 0) {
    return find(root, check, bias);
  }
};
template <typename M, std::size_t LIM>
int SegmentTree_Dynamic<M, LIM>::node_count = 0;