/**
 * @title Link-Cut-Tree(モノイド)
 * @category データ構造
 * @brief O(logN)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename M>
struct LinkCutTree_Monoid {
  using T = typename M::T;
  struct Node {
    Node *ch[2], *par;
    bool rev;
    T val, dat, rdat;
    Node(T init = M::ti()) : rev(false), val(init), dat(init), rdat(init) {
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
    pushup(p);
    pushup(t);
    t->par = p->par;
    if (!is_root(p)) {
      p->par->ch[dir(p)] = t;
      pushup(t->par);
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
  void pushup(Node *t) {
    t->rdat = t->dat = t->val;
    if (t->ch[0])
      t->dat = M::f(t->ch[0]->dat, t->dat),
      t->rdat = M::f(t->rdat, t->ch[0]->rdat);
    if (t->ch[1])
      t->dat = M::f(t->dat, t->ch[1]->dat),
      t->rdat = M::f(t->ch[1]->rdat, t->rdat);
  }
  Node *expose(Node *x) {
    Node *r = nullptr;
    for (Node *p = x; p; p = p->par) {
      splay(p);
      p->ch[1] = r;
      pushup(p);
      r = p;
    }
    splay(x);
    return r;
  }
  void toggle(Node *t) {
    swap(t->ch[0], t->ch[1]);
    swap(t->dat, t->rdat);
    t->rev ^= 1;
  }
  Node *eval(Node *t) {
    if (t->rev) {
      if (t->ch[0]) toggle(t->ch[0]);
      if (t->ch[1]) toggle(t->ch[1]);
      t->rev = false;
    }
    return t;
  }

 private:
  vector<Node> ns;
  size_t linkcnt;

 public:
  LinkCutTree_Monoid(int n, T init = M::ti()) : ns(n, init), linkcnt(0) {}
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
    pushup(&ns[p]);
  }
  // cut link from c to p
  void cut(int c, int p) {
    linkcnt--;
    evert(p);
    expose(&ns[c]);
    Node *y = ns[c].ch[0];
    ns[c].ch[0] = y->par = nullptr;
    pushup(&ns[c]);
  }
  int lca(int x, int y) {
    expose(&ns[x]);
    Node *u = expose(&ns[y]);
    return ns[x].par ? u - &ns[0] : -1;
  }
  T operator[](int k) { return ns[k].val; }
  // [a,b] closed section
  T query(int a, int b) {
    evert(a);
    expose(&ns[b]);
    return ns[b].dat;
  }
  void set_val(int k, T v) {
    expose(&ns[k]);
    ns[k].val = v;
    eval(&ns[k]);
  }
};