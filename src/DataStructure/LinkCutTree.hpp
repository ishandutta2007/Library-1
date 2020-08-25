/**
 * @title Link-Cut-Tree(シンプル)
 * @category データ構造
 * @brief O(logN)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct LinkCutTree {
  struct Node {
    Node *ch[2], *par;
    bool rev;
  };

 private:
  bool is_root(Node *x) {
    return !x->par || (x->par->ch[0] != x && x->par->ch[1] != x);
  }
  bool dir(Node *x) { return x->par && x->par->ch[1] == x; }
  void rot(Node *t) {
    Node *p = t->par;
    bool d = dir(t);
    if ((p->ch[d] = t->ch[!d])) t->ch[!d]->par = p;
    t->ch[!d] = p;
    t->par = p->par;
    if (!is_root(p)) p->par->ch[dir(p)] = t;
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
  void toggle(Node *t) {
    swap(t->ch[0], t->ch[1]);
    t->rev ^= true;
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
  int linkcnt;

 public:
  LinkCutTree(int n) : ns(n), linkcnt(0) {}
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
  // cut link from c (to the root direction)
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
};