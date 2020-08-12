/**
 * @title 赤黒木(永続遅延伝搬)
 * @category データ構造
 * @brief O(logN)
 */

// verify用: https://atcoder.jp/contests/arc030/tasks/arc030_4
#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/DataStructure/RedBlackTree_Lazy.hpp"
#undef call_from_test
#endif

template <typename M, size_t LIM = 1 << 22, size_t FULL = 1000>
struct RedBlackTree_PersistentLazy : RedBlackTree_Lazy<M, LIM> {
  using RBTL = RedBlackTree_Lazy<M, LIM>;
  using RBTL::RedBlackTree_Lazy;
  using Node = typename RBTL::Node;
  using RBTPL = RedBlackTree_PersistentLazy;

 private:
  Node *clone(Node *t) override { return &(*RBTL::pool.alloc() = *t); }

 public:
  // merge
  RBTPL operator+(const RBTPL &r) {
    if (!this->root || !r.root) return this->root ? *this : r;
    Node *c = RBTL::submerge(this->root, r.root);
    c->color = RBTL::BLACK;
    return RBTPL(c);
  }
  // [0,k) [k,size)
  pair<RBTPL, RBTPL> split(int k) {
    auto tmp = RBTL::split(this->root, k);
    return make_pair(RBTPL(tmp.first), RBTPL(tmp.second));
  }
  // [0,a) [a,b) [b,size)
  tuple<RBTPL, RBTPL, RBTPL> split3(int a, int b) {
    auto x = RBTL::split(this->root, a);
    auto y = RBTL::split(x.second, b - a);
    return make_tuple(RBTPL(x.first), RBTPL(y.first), RBTPL(y.second));
  }
  void rebuild() {
    auto ret = RBTL::dump();
    RBTL::pool.clear();
    RBTL::build(ret);
  }
  static bool almost_full() { return RBTL::pool.ptr < FULL; }
};
