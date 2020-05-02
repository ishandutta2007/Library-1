/**
 * @title Segment-Tree(動的)
 * @category データ構造
 * @brief O(logN)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename M>
struct SegmentTree_Dynamic {
  using T = typename M::T;
  using ll = long long;
  using U = unsigned long long;
  struct Node {
    T dat;
    U xor_lazy;
    Node *ch[2];
    Node() : dat(M::ti()), xor_lazy(0), ch{nullptr, nullptr} {}
    void *operator new(size_t) {
      static vector<Node> pool(1 << 23);
      return &pool[node_count++];
    }
  };
  using np = Node *;

 protected:
  const int height;
  const ll n;
  static int node_count;
  np root;

 private:
  inline void push(np t, ll b) {
    if ((t->xor_lazy >> (U)b) & (U)1) swap(t->ch[0], t->ch[1]);
    if (t->ch[0] != nullptr) t->ch[0]->xor_lazy ^= t->xor_lazy;
    if (t->ch[1] != nullptr) t->ch[1]->xor_lazy ^= t->xor_lazy;
    t->xor_lazy = 0;
  }
  T value(np t) { return t ? t->dat : M::ti(); }
  np set_Node(np t, U pos, T val, ll b) {
    if (t == nullptr) t = new Node();
    if (b < 0) {
      t->dat = val;
      return t;
    }
    push(t, b);
    bool f = (pos >> (U)b) & (U)1;
    t->ch[f] = set_Node(t->ch[f], pos, val, b - 1);
    t->dat = M::f(value(t->ch[0]), value(t->ch[1]));
    return t;
  }
  T query_Node(ll l, ll r, np t, ll lb, ll ub, ll b) {
    if (t == nullptr || ub <= l || r <= lb) return M::ti();
    push(t, b);
    if (l <= lb && ub <= r) return t->dat;
    ll c = (lb + ub) / 2;
    T vl = query_Node(l, r, t->ch[0], lb, c, b - 1);
    T vr = query_Node(l, r, t->ch[1], c, ub, b - 1);
    return M::f(vl, vr);
  }
  template <typename C>
  ll find(np t, C &check, U bias) {
    if (!t) return -1;
    ll ret = 0;
    T acc = M::ti();
    for (ll b = height - 1; b >= 0; b--) {
      push(t, b);
      bool f = (bias >> (U)b) & (U)1;
      if (!check(M::f(acc, value(t->ch[f]))))
        acc = M::f(acc, value(t->ch[f])), f = !f;
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
    Node::node_count = 0;
    root = nullptr;
  }
  void xor_all(U key) {
    if (root != nullptr) root->xor_lazy ^= key;
  }
  void set_val(ll k, T x) { root = set_Node(root, k, x, height - 1); }
  //[a,b)
  T query(ll a, ll b) { return query_Node(a, b, root, 0, n, height - 1); }
  T operator[](ll k) { return query(k, k + 1); }
  // min { i : check(query(0,i+1)) = true }
  template <typename C>
  ll find_first(C &check, U bias = 0) {
    return find(root, check, bias);
  }
};
template <typename M>
int SegmentTree_Dynamic<M>::node_count = 0;