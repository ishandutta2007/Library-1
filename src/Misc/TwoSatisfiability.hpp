#pragma once
#include "src/Graph/StronglyConnectedComponents.hpp"
class TwoSatisfiability {
 int n;
 Graph g;
public:
 TwoSatisfiability(int n): n(n), g(n + n) {}
 inline int neg(int x) const { return x >= n ? x - n : x + n; }
 void add_if(int u, int v) { g.add_edge(u, v), g.add_edge(neg(v), neg(u)); }  // u -> v <=> !v -> !u
 void add_or(int u, int v) { add_if(neg(u), v); }                             // u or v <=> !u -> v
 void add_nand(int u, int v) { add_if(u, neg(v)); }                           // u nand v <=> u -> !v
 void set_true(int u) { g.add_edge(neg(u), u); }                              // u <=> !u -> u
 void set_false(int u) { g.add_edge(u, neg(u)); }                             // !u <=> u -> !u
 std::vector<bool> solve() {
  StronglyConnectedComponents scc(g);
  std::vector<bool> ret(n);
  for (int i= 0, l, r; i<n; ret[i++]= l> r)
   if (l= scc(i), r= scc(neg(i)); l == r) return {};  // no solution
  return ret;
 }
};