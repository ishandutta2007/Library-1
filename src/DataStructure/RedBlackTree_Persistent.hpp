/**
 * @title 赤黒木(永続)
 * @category データ構造
 * @brief O(logN)
 */
// verify用:
// https://atcoder.jp/contests/joisc2012/tasks/joisc2012_copypaste

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/RedBlackTree.hpp"
#undef call_from_test
#endif

template <typename M, size_t LIM = 1 << 22, size_t FULL = 1000>
struct RedBlackTree_Persistent : RedBlackTree<M, LIM> {
  using RBT = RedBlackTree<M, LIM>;
  using RBT::RedBlackTree;
  using Node = typename RBT::Node;
  using RBTP = RedBlackTree_Persistent;

 private:
  Node *clone(Node *t) override { return &(*RBT::pool.alloc() = *t); }

 public:
  // merge
  RBTP operator+(const RBTP &r) {
    if (!this->root || !r.root) return this->root ? *this : r;
    Node *c = RBT::submerge(this->root, r.root);
    c->color = RBT::BLACK;
    return RBTP(c);
  }
  // [0,k) [k,size)
  pair<RBTP, RBTP> split(int k) {
    auto tmp = RBT::split(this->root, k);
    return make_pair(RBTP(tmp.first), RBTP(tmp.second));
  }
  // [0,a) [a,b) [b,size)
  tuple<RBTP, RBTP, RBTP> split3(int a, int b) {
    auto x = RBT::split(this->root, a);
    auto y = RBT::split(x.second, b - a);
    return make_tuple(RBTP(x.first), RBTP(y.first), RBTP(y.second));
  }
  void rebuild() {
    auto ret = RBT::dump();
    RBT::pool.clear();
    RBT::build(ret);
  }
  static bool almost_full() { return RBT::pool.ptr < FULL; }
};
