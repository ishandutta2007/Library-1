/**
 * @title 2-SAT
 * @category 数学
 * @brief 強連結成分分解を用いる
 * @brief solve():真偽値の割当を返す(充足不可能なら空)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Graph/StronglyConnectedComponents.hpp"
#undef call_from_test
#endif

struct TwoSatisfiability {
 private:
  int sz;
  StronglyConnectedComponents scc;

 public:
  TwoSatisfiability(int n) : sz(n), scc(2 * n) {}
  void add_if(int u, int v) {
    scc.add_edge(u, v);
    scc.add_edge(neg(v), neg(u));
  }                                                   // u -> v <=> !v -> !u
  void add_or(int u, int v) { add_if(neg(u), v); }    // u or v <=> !u -> v
  void add_nand(int u, int v) { add_if(u, neg(v)); }  // u nand v <=> u -> !v
  void set_true(int u) { scc.add_edge(neg(u), u); }   // u <=> !u -> u
  void set_false(int u) { scc.add_edge(u, neg(u)); }  // !u <=> u -> !u
  inline int neg(int x) { return x >= sz ? x - sz : x + sz; }
  vector<short> solve() {
    vector<int> I = scc.get_SCC().second;
    vector<short> ret(sz);
    for (int i = 0; i < sz; i++) {
      if (I[i] == I[neg(i)]) return {};
      ret[i] = I[i] > I[neg(i)];
    }
    return ret;
  }
};
