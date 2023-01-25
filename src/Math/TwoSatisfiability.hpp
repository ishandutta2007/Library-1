#pragma once
#include "src/Graph/StronglyConnectedComponents.hpp"
class TwoSatisfiability {
 int sz;
 StronglyConnectedComponents scc;
public:
 TwoSatisfiability(int n): sz(n), scc(2 * n) {}
 void add_if(int u, int v) { scc.add_edge(u, v), scc.add_edge(neg(v), neg(u)); }  // u -> v <=> !v -> !u
 void add_or(int u, int v) { add_if(neg(u), v); }                                 // u or v <=> !u -> v
 void add_nand(int u, int v) { add_if(u, neg(v)); }                               // u nand v <=> u -> !v
 void set_true(int u) { scc.add_edge(neg(u), u); }                                // u <=> !u -> u
 void set_false(int u) { scc.add_edge(u, neg(u)); }                               // !u <=> u -> !u
 inline int neg(int x) const { return x >= sz ? x - sz : x + sz; }
 std::vector<bool> solve() const {
  std::vector<int> I= scc.get_index(scc.get_block());
  std::vector<bool> ret(sz);
  for (int i= 0; i < sz; i++) {
   if (I[i] == I[neg(i)]) return {};  // no solution
   ret[i]= I[i] > I[neg(i)];
  }
  return ret;
 }
};
