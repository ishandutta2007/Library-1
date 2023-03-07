#pragma once
#include "src/Graph/StronglyConnectedComponents.hpp"
class TwoSatisfiability {
 int sz;
 StronglyConnectedComponents scc;
public:
 TwoSatisfiability(int n): sz(n), scc(n + n) {}
 void add_if(int u, int v) { scc.add_edge(u, v), scc.add_edge(neg(v), neg(u)); }  // u -> v <=> !v -> !u
 void add_or(int u, int v) { add_if(neg(u), v); }                                 // u or v <=> !u -> v
 void add_nand(int u, int v) { add_if(u, neg(v)); }                               // u nand v <=> u -> !v
 void set_true(int u) { scc.add_edge(neg(u), u); }                                // u <=> !u -> u
 void set_false(int u) { scc.add_edge(u, neg(u)); }                               // !u <=> u -> !u
 inline int neg(int x) const { return x >= sz ? x - sz : x + sz; }
 std::vector<bool> solve() {
  scc.build();
  std::vector<bool> ret(sz);
  for (int i= 0, l, r; i<sz; ret[i++]= l> r)
   if (l= scc.belong(i), r= scc.belong(neg(i)); l == r) return {};  // no solution
  return ret;
 }
};