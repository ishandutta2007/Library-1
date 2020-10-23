#pragma once
#include <bits/stdc++.h>
/**
 * @title Link-Cut-Tree(モノイド双対)
 * @category データ構造
 * @brief O(logN)
 */

// BEGIN CUT HERE

template <typename M>
struct LinkCutTree_MonoidDual {
  using T = typename M::T;
  using E = typename M::E;
  struct Node {
    Node *ch[2], *par;
    bool rev;
    T val;
    E laz;
    Node(T init = T()) : rev(false), val(init), laz(M::ei()) {
      ch[0] = ch[1] = par = nullptr;
    }
  };

 private:
  bool is_root(Node *x) {
    return !x->par || (x->par->ch[0] != x && x->par->ch[1] != x);
  }
  bool dir(Node *x) { return x->par && x->par->ch[1] == x; }
  void rot(Node *t) {
    Node *p = t->par;
    bool d = dir(t);
    if ((p->ch[d] = t->ch[!d])) p->ch[d]->par = p;
    t->ch[!d] = p;
    t->par = p->par;
    if (!is_root(p)) {
      p->par->ch[dir(p)] = t;
    }
    p->par = t;
  }
  void splay(Node *x) {
    eval(x);
    while (!is_root(x)) {
      if (!is_root(x->par)) eval(x->par->par);
      eval(x->par);
      eval(x);
      if (!is_root(x->par)) {
        if (dir(x) == dir(x->par))
          rot(x->par);
        else
          rot(x);
      }
      rot(x);
    }
  }
  Node *expose(Node *x) {
    Node *r = nullptr;
    for (Node *p = x; p; p = p->par) {
      splay(p);
      p->ch[1] = r;
      r = p;
    }
    splay(x);
    return r;
  }
  void propagate(Node *t, E v) {
    t->laz = M::h(t->laz, v);
    t->val = M::g(t->val, v);
  }
  void toggle(Node *t) {
    std::swap(t->ch[0], t->ch[1]);
    t->rev ^= 1;
  }
  Node *eval(Node *t) {
    if (t->laz != M::ei()) {
      if (t->ch[0]) propagate(t->ch[0], t->laz);
      if (t->ch[1]) propagate(t->ch[1], t->laz);
      t->laz = M::ei();
    }
    if (t->rev) {
      if (t->ch[0]) toggle(t->ch[0]);
      if (t->ch[1]) toggle(t->ch[1]);
      t->rev = false;
    }
    return t;
  }

 private:
  std::vector<Node> ns;
  size_t linkcnt;

 public:
  LinkCutTree_MonoidDual(int n, T init = T()) : ns(n, init), linkcnt(0) {}
  // make k the root
  void evert(int k) {
    expose(&ns[k]);
    toggle(&ns[k]);
    eval(&ns[k]);
  }
  // add link from c to p
  void link(int c, int p) {
    assert(linkcnt++ < ns.size() - 1);
    evert(c);
    expose(&ns[p]);
    ns[p].ch[1] = &ns[c];
    ns[c].par = &ns[p];
  }
  // cut link from c to p
  void cut(int c, int p) {
    linkcnt--;
    evert(p);
    expose(&ns[c]);
    Node *y = ns[c].ch[0];
    ns[c].ch[0] = y->par = nullptr;
  }
  int lca(int x, int y) {
    expose(&ns[x]);
    Node *u = expose(&ns[y]);
    return ns[x].par ? u - &ns[0] : -1;
  }
  T operator[](int k) {
    expose(&ns[k]);
    return ns[k].val;
  }
  void update(int a, int b, E v) {
    evert(a);
    expose(&ns[b]);
    propagate(&ns[b], v);
    eval(&ns[b]);
  }
  void set_val(int k, T v) {
    expose(&ns[k]);
    ns[k].val = v;
    eval(&ns[k]);
  }
};
